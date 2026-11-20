#include "common.h"
#include "main.h"
#include "mojidisp.h"
#include "monitor.h"
#include "pad.h"
#include "trans.h"
#include "line_scr.h"
#include "em_sub.h"
#include "pl_wep.h"
#include "sub.h"
#include "joint.h"
#include "sca.h"
#include "cd.h"
#include "sce0.h"

/* raw access to ENEMY_WORK fields past the known layout */
#define GW(T, Ofs) (*(T *)((UCHAR *)&G + (Ofs)))
#define GA(T, Ofs) ((T *)((UCHAR *)&G + (Ofs)))
#define EMA(p, T, Ofs) ((T *)((UCHAR *)(p) + (Ofs)))
/* player work (RE2 G.Player; tagGLOBAL only has individual Pl_* fields) */
#define PLAYER ((ENEMY_WORK *)((UCHAR *)&G + 0x248C))

/* player state kept at the end of the save block (Game.pSave, func_80024B34) */
typedef struct tagSAVE_POS {
    UCHAR Data[0x708];                 /* 0x000 copied to G+0x77F0 */
    SHORT Pos_x;                       /* 0x708 */
    SHORT Pos_y;                       /* 0x70A */
    SHORT Pos_z;                       /* 0x70C */
    SHORT Cdir_y;                      /* 0x70E */
    SHORT Life;                        /* 0x710 */
    UCHAR Id;                          /* 0x712 */
    UCHAR Unk713;                      /* 0x713 */
    ULONG Unk714;                      /* 0x714 random seed (Set_door) */
    USHORT Unk718;                     /* 0x718 */
} SAVE_POS;

/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((USHORT *)&(p)->Unk74)
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)
/* RE2 ENEMY_WORK.M: rotation matrix at +0x20, its t[] is Pos_x..Pos_z */
#define EM_M(p) ((MATRIX *)((UCHAR *)(p) + 0x20))
#define EMW(p, T, Ofs) (*(T *)((UCHAR *)(p) + (Ofs)))

/* GTE inline macros (PsyQ inline_c.h with the DMPSX-converted opcodes) */
#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stsv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 2(%0); sh $14, 4(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_gpf12_b() __asm__ volatile(".word 0x4B98003D")
#define gte_gpl12() __asm__ volatile("nop; nop; .word 0x4BA8003E")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_ldsxy3(r0, r1, r2) __asm__ volatile("mtc2 %0, $12; mtc2 %2, $14; mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldsz3(r0, r1, r2) __asm__ volatile("mtc2 %0, $17; mtc2 %1, $18; mtc2 %2, $19" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile("swc2 $12, 8(%0); swc2 $13, 20(%0); swc2 $14, 32(%0)" : : "r"(r0) : "memory")
#define gte_nclip_b() __asm__ volatile(".word 0x4B400006")
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_avsz3() __asm__ volatile("nop; nop; .word 0x4B58002D")
#define gte_avsz4() __asm__ volatile("nop; nop; .word 0x4B68002E")
#define gte_rtpt() __asm__ volatile("nop; nop; .word 0x4A280030")
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetLightMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $8; ctc2 $13, $9; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $10; ctc2 $13, $11; ctc2 $14, $12" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stclmv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_rtir() __asm__ volatile("nop; nop; .word 0x4A49E012")
#define gte_MulMatrix0(r1, r2, r3) { gte_SetRotMatrix(r1); gte_ldclmv(r2); gte_rtir(); gte_stclmv(r3); gte_ldclmv((char *)(r2) + 2); gte_rtir(); gte_stclmv((char *)(r3) + 2); gte_ldclmv((char *)(r2) + 4); gte_rtir(); gte_stclmv((char *)(r3) + 4); }
#define gte_ldlvl(r0) __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0)" : : "r"(r0))
#define gte_stlvl(r0) __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(r0) : "memory")

/* RE3 effect work (ESP_R.C area; layout differs from RE2 ESP_WORK) */
typedef struct tagESP3_WORK {
    UCHAR Be_flg;                      /* 0x00 */
    UCHAR Routine_1;                   /* 0x01 */
    UCHAR Routine_2;                   /* 0x02 */
    UCHAR pad3;
    USHORT Unk4;                       /* 0x04 */
    UCHAR pad6[0xA - 0x6];
    SHORT UnkA;                        /* 0x0A */
    UCHAR padC;
    signed char UnkD;                  /* 0x0D */
    UCHAR padE[0x10 - 0xE];
    SHORT Unk10;                       /* 0x10 */
    SHORT Unk12;                       /* 0x12 */
    SHORT Unk14;                       /* 0x14 */
    UCHAR pad16[0x18 - 0x16];
    UCHAR Unk18;                       /* 0x18 */
    UCHAR Unk19;                       /* 0x19 */
    UCHAR Unk1A;                       /* 0x1A */
    UCHAR Unk1B;                       /* 0x1B */
    USHORT Unk1C;                      /* 0x1C */
    USHORT Unk1E;                      /* 0x1E */
    UCHAR pad20[0x22 - 0x20];
    USHORT Unk22;                      /* 0x22 */
    USHORT Unk24;                      /* 0x24 */
    USHORT Unk26;                      /* 0x26 */
    UCHAR pad28;
    UCHAR Unk29;                       /* 0x29 */
    UCHAR pad2A;
    UCHAR Unk2B;                       /* 0x2B */
    USHORT Unk2C;                      /* 0x2C */
    USHORT Unk2E;                      /* 0x2E */
    SHORT Unk30;                       /* 0x30 */
    SHORT Unk32;                       /* 0x32 */
    SHORT Unk34;                       /* 0x34 */
    UCHAR pad36[0x38 - 0x36];
    VECTOR Unk38;                      /* 0x38 */
    VECTOR Pos;                        /* 0x48 */
    UCHAR pad58[0x68 - 0x58];
    SHORT Rot[3];                      /* 0x68 rotation (Eff_sprt_trans) */
    UCHAR pad6E;
    UCHAR Anm_cnt;                     /* 0x6F index into pAnmseq */
    MATRIX M70;                        /* 0x70 local matrix (Eff_sprt_trans) */
    MATRIX Mat;                        /* 0x90 */
    UCHAR padB0[0xB3 - 0xB0];
    UCHAR UnkB3;                       /* 0xB3 */
    SHORT UnkB4;                       /* 0xB4 */
    SHORT UnkB6;                       /* 0xB6 */
    SHORT UnkB8;                       /* 0xB8 */
    SHORT UnkBA;                       /* 0xBA */
    UCHAR padBC[0xC0 - 0xBC];
    MATRIX *pMat;                      /* 0xC0 */
    struct tagESP3_SEQ *pAnmseq;       /* 0xC4 */
    UCHAR padC8[0xD4 - 0xC8];
} ESP3_WORK;

/* effect animation sequence entry (8 bytes) */
typedef struct tagESP3_SEQ {
    UCHAR Unk0;                        /* 0x0 */
    UCHAR nSpr;                        /* 0x1 */
    UCHAR Unk2;                        /* 0x2 */
    UCHAR Unk3;                        /* 0x3 */
    ULONG Unk4;                        /* 0x4 */
} ESP3_SEQ;
extern ESP3_WORK D_800BA8A8[96];       /* effect works */

/* 0x68 byte prim work (func_8002D41C) */
typedef struct tagRGB_BLEND_WORK {
    UCHAR pad0[0x10];
    ULONG Rgb0;                        /* 0x10 */
    UCHAR pad14[0x1C - 0x14];
    ULONG Unk1C;                       /* 0x1C */
    UCHAR pad20[0x28 - 0x20];
    ULONG Rgb1;                        /* 0x28 */
    UCHAR pad2C[0x68 - 0x2C];
} RGB_BLEND_WORK;

/* model work (pW[1] = MD_OBJ, then the double-buffered prims built by PresetObjectGT3_neo / PresetObjectGT4_neo) */
typedef struct tagMD_WORK {
    ULONG Attr;                        /* 0x00 bits 2-3: semi-trans rate */
    MD_OBJ *pObj;                      /* 0x04 */
    POLY_GT3 *pPrim3;                  /* 0x08 */
    POLY_GT4 *pPrim4;                  /* 0x0C */
} MD_WORK;

/* second model work layout (func_8002E61C) */
typedef struct tagMD_WORK2 {
    UCHAR pad0[0x10];
    ULONG Attr;                        /* 0x010 bits 2-3: semi-trans rate */
    MD_OBJ *pObj;                      /* 0x014 */
    UCHAR pad18[0x188 - 0x18];
    POLY_GT4 *pPrim4;                  /* 0x188 */
} MD_WORK2;

/* model colour parameters set from ENEMY_WORK+0x178 (func_800254AC) */
typedef struct tagMODEL_COL {
    USHORT Unk0[3];                    /* 0x0 */
    SHORT Unk6;                        /* 0x6 */
    ULONG Rgb;                         /* 0x8 */
} MODEL_COL;
extern MODEL_COL D_80098B08;
void func_80025610(ENEMY_WORK *pEm, PARTS_W *pP, SHORT Flg, MATRIX *pM);

/* flash state (func_80024F74) */
typedef struct tagFLASH_WORK {
    UCHAR Flg;                         /* 0x0 */
    UCHAR pad1[3];
    UCHAR Cnt;                         /* 0x4 */
} FLASH_WORK;
extern FLASH_WORK D_800C703C;

/* Init_system callees owned by other modules */
void func_80087F6C(void);
void func_80073E84(void);
void func_800101AC(void);
void func_80094374(LONG Arg);
void func_800943C8(void);
void _bu_init(void);
void func_800912C4(void);
void Init_main(void);
void Vram_clr(USHORT x, USHORT y, USHORT w, USHORT h);
extern volatile ULONG D_800D454C;      /* right after Main.Fade[]: vsync counter */
extern UCHAR D_800DBB58;
extern UCHAR D_800996E4[];             /* message text banks (Mess_set) */
extern USHORT D_8009BDB4[];
extern UCHAR D_80098E88[];
extern USHORT D_80099654[];
extern ULONG D_80098700;
void func_80078FE8(void);
void func_8007906C(void);
void func_80010150(void *p, ULONG Size);    /* RE2 Memclr */

/* primitive sort parameters (func_80025FB0 -> func_8002DC98 / func_8002DE0C) */
typedef struct tagSORT_PARAM {
    ULONG Unk0;                        /* 0x0 */
    union {
        ULONG Unk4;                    /* 0x4 */
        struct {
            UCHAR pad4[3];
            UCHAR Code;                /* 0x7 primitive code (0x34 GT3, 0x3C GT4) */
        } c;
    } u;
    ULONG Unk8;                        /* 0x8 */
    ULONG UnkC;                        /* 0xC */
    ULONG Unk10;                       /* 0x10 */
    ULONG Unk14;                       /* 0x14 */
} SORT_PARAM;
ULONG func_80037D50(VECTOR *pPos);
void func_8002D41C(RGB_BLEND_WORK *p, RGB_BLEND_WORK *pEnd, ULONG Data, ULONG Rgb);
void func_8002D184(void *pWork0, void *pWork1, ULONG Arg2, MATRIX *pM);
void func_8007A490(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP);
void func_8007A350(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP);
ULONG func_8002C824(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002CABC(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002CE08(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002CF90(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002DFE0(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002E160(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
extern UCHAR D_800E1388[];

void func_8002D5D8(void *pWork0, void *pWork1, ULONG Arg2);
ULONG func_8002DC98(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002DE0C(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002D6D8(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002D898(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002B6FC(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
ULONG func_8002B86C(SORT_PARAM *pPrm, ULONG Packet, void *pWork0, void *pWork1);
void func_80027568(PARTS_W *pP_ptr, MATRIX *wm);
void func_8002783C(PARTS_W *pP_ptr);
void func_800253F0(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG Be_flg, MATRIX *wm);
void func_80026648(ENEMY_WORK *pEm);
void func_80026498(ENEMY_WORK *pEm);
void HMatrix(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, SHORT Rate);
void hokan_svec(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, SHORT Rate);
void func_80027338(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG No, ULONG Late_flg);
void J_seq(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late_flg);
void func_8002B2BC(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late);
void func_80027F8C(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG No, ULONG Late_flg);
void J_seq2(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late_flg);
ULONG func_800102E8(void);   /* Rnd */
ULONG func_8002BA3C(ULONG *pTmd, ULONG Packet, PARTS_W *pP);
ULONG func_8002C068(ULONG *pTmd, ULONG Packet, PARTS_W *pP);
void func_80025FB0(ULONG *pObj, ULONG Arg1, ULONG *pObj2);
void func_80025C04(ULONG *pObj, ULONG Flg, PARTS_W *pP);
void func_80025DE8(ULONG *pObj, ULONG Flg, PARTS_W *pP);
void Set_light_pos(VECTOR *pPos);
extern MATRIX *D_800C79F0;            /* current work matrices (func_80025A28) */
extern VECTOR *D_800C79F4;
LONG Water_ck(LONG Pos_x, LONG Pos_z);
extern LONG D_800CA720;
extern UCHAR D_80098728[];
extern ULONG D_800986C4[];             /* scale curve (func_80021D58) */
void func_8001CDA4(ESP3_WORK *pEsp, ULONG n);
void func_8001CE24(ESP3_WORK *pEsp, ULONG a1, ULONG a2);             /* random byte table, indexed by Rnd() */
ESP3_WORK *func_8001B35C(ESP3_WORK *pEsp, ULONG Attr, ULONG Ofs, MATRIX *pMat, VECTOR *pPos, SVECTOR *pDir);

/* declarations from earlier parts of the original address-split file */
typedef struct tagESP_PRM4B8 {
    SHORT Cnt;
    SHORT Flg;
    SHORT Attr;
    SHORT Base10;
    SHORT Mul10;
    SHORT Base12;
    SHORT Mul12;
    SHORT pad;
} ESP_PRM4B8;

typedef struct tagESP_HIT {
    VECTOR *pPos;                      /* 0x0 */
    SHORT Unk4[3];                     /* 0x4 */
    UCHAR pad[2];
    SHORT Unk0C;                       /* 0xC */
} ESP_HIT;
extern ULONG D_80097FCC[];
LONG func_800472EC(ESP_HIT *pHit, ULONG Arg);

/* door AOT (RE2 SCE_AOT_DOOR next position part) */
typedef struct tagDOOR_NEXT {
    SHORT Next_pos_x;                  /* 0x0 */
    SHORT Next_pos_y;                  /* 0x2 */
    SHORT Next_pos_z;                  /* 0x4 */
    SHORT Next_cdir_y;                 /* 0x6 */
    UCHAR Next_stage;                  /* 0x8 */
    UCHAR Next_room;                   /* 0x9 */
    UCHAR Next_cut;                    /* 0xA */
    UCHAR Next_nfloor;                 /* 0xB */
} DOOR_NEXT;
void func_80024C58(void);
void Door_main(void);
void Door_snd_trans(void);
void Set_stage(void);
void func_80077C4C(void);
void Set_room(void);
void func_80077F60(void);
extern UCHAR D_800E0186;

void func_80025610_(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM) __asm__("func_80025610");

void MatrixNormal_0(MATRIX *m, MATRIX *n);
void func_80026728_(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate) __asm__("HMatrix");

void MatrixNormal_2(MATRIX *m, MATRIX *n);

void func_8002720C_(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, LONG Rate) __asm__("hokan_svec");

void func_8002B190_(ULONG n, PARTS_W *pP, LONG Rate, ULONG Late) __asm__("func_8002B190");
ULONG func_80027338_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg) __asm__("func_80027338");

void func_800281B4(ULONG n, PARTS_W *pP, ULONG Late, SHORT Rate);
ULONG func_80027F8C_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg) __asm__("func_80027F8C");

void func_8002606C_(ULONG *pObj, ULONG Flg, PARTS_W *pP) __asm__("func_8002606C");

void Pad_set(void);
void func_8007745C(ULONG No);
void Snd_seq_play_ck(ULONG No);
void func_800741A0(void);
void Swap_Cbuff(void);
void func_80029700(void);
void Movie(void);
void Bg_draw(void);
void func_8002A2B8(void);

void main(void)
{
    tagMAIN *pMain;
    tagGLOBAL *pG;
    ULONG i;

    Init_system();
    pMain = &Main;
    pG = &G;
    while (1) {
        if (Main.Pollhost_flg != 0) {
            __asm__ volatile(".word 0x0001000D"); /* break 1 (RE2 trap(0x400)) */
        }
        pG->pOt0 = pG->Ot0[pG->Cbuf_id];
        pG->pOt1 = pG->Ot1[pG->Cbuf_id];
        pG->pOt2 = pG->Ot2[pG->Cbuf_id];
        pMain->pDb = &pMain->Db[pG->Cbuf_id];
        ClearOTagR(pG->pOt0, 8);
        ClearOTagR(pG->pOt1, 0x400);
        ClearOTagR(pG->pOt2, 8);
        if (pG->Unk2470) {
            i = 0;
            do {
                ClearOTagR(pG->pOt_sub[pG->Cbuf_id][i + 1], 0x400);
                i++;
            } while (i < pG->Unk2470);
        }
        Moji_mode_init();
        if (pG->Padd_data[0][0] != 0) {
            pG->Padd_data[0][3] = 0xFF;
            pG->Padd_data[0][2] = 0xFF;
            pG->Padd_data[1][3] = 0xFF;
            pG->Padd_data[1][2] = 0xFF;
        }
        if (pG->Padd_data[1][0] != 0) {
            pG->Padd_data[1][3] = 0xFF;
            pG->Padd_data[1][2] = 0xFF;
        }
        Pad_set();
        if (G.System_flg & 0x200000) {
            Soft_reset();
            continue;
        }
        if ((G.System_flg & 0x8000000) && Main.Unk18E == 0 && *(USHORT *)&Game.Doordemo_flg == 0
            && *(USHORT *)&CD.Cd_rtn_no == 0 && !(G.Unk2468 & 0x1000000)
            && !(G.System_flg & 0x10594240) && !(G.Status_flg & 0x400208) && Main.Unk194 == 0) {
            if ((G.Unk20FC & 0x800) && G.Unk246C >= 0) {
                G.Unk20FC ^= 0x800;
                Main.Unk194 = 2;
            } else if (pG->Padd_data[0][0] == 0xFF) {
                Main.Unk194 = 1;
            } else {
                goto no_pause;
            }
            func_8007745C(0);
            func_8007745C(1);
            func_8007745C(2);
            G.Unk2488 = 1;
        }
    no_pause:
        if (Main.Unk194 != 0) {
            func_8002A2B8();
            if ((Main.Unk194 == 1 && pG->Padd_data[0][0] != 0xFF) || (Main.Unk194 == 2 && (G.Unk20FC & 0x800))) {
                Snd_seq_play_ck(0);
                Snd_seq_play_ck(1);
                Snd_seq_play_ck(2);
                G.Unk2488 = 0;
                Main.Unk194 = 0;
            }
        }
        Scheduler();
        pMain->Unk198 = pG->Status_flg;
        pG->Status_flg &= 0xFF3FFFFF;
        if (G.System_flg & 0x800000) {
            Fade_out_reset();
            continue;
        }
        func_800741A0();
        Cd_system_control();
        System_trans_all();
        Swap_Cbuff();
        func_800102E8();
    }
}

INCLUDE_ASM("asm/nonmatchings/game/main", Swap_Cbuff);

void func_80029700(void)
{
    RECT Rect;
    tagMAIN *pM;
    ULONG *p;
    ULONG i;
    ULONG j;
    LONG y;

    pM = &Main;
    pM->Vloop = 4;
    if (G.Unk20FC & 0x20) {
        D_800CA720 = 0;
    }
    if (!(pM->Unk198 & 0x400000)) {
        y = pM->pDb->Draw.ofs[1];
        if (y == 0) {
            y = 240;
        } else {
            y = 0;
        }
        Rect.y = y;
        Rect.x = 0;
        Rect.w = 320;
        Rect.h = 120;
        for (i = 0; i < 2; i++) {
            DrawSync(0);
            StoreImage(&Rect, (ULONG *)0x801C1814);
            p = (ULONG *)0x801C1814;
            for (j = 0; j < 0x4B00; j++) {
                *p = ~*p;
                p++;
            }
            LoadImage(&Rect, (ULONG *)0x801C1814);
            Rect.y += 120;
        }
    }
}

void Init_system(void)
{
    tagMAIN *pM;
    tagGLOBAL *pG;

    pM = &Main;
    pG = &G;
    ResetCallback();
    SetGraphDebug(0);
    func_80087F6C();
    Cd_init();
    func_80073E84();
    Init_scheduler();
    VSyncCallback(func_800101AC);
    pM->Vloop = 1;
    D_800D454C = 0;
    SetDefDrawEnv(&pM->Db[0].Draw, 0, 0, 320, 240);
    SetDefDrawEnv(&pM->Db[1].Draw, 0, 240, 320, 240);
    SetDefDispEnv(&pM->Db[0].Disp, 0, 240, 320, 240);
    SetDefDispEnv(&pM->Db[1].Disp, 0, 0, 320, 240);
    pM->Db[1].Draw.dfe = 1;
    pM->Db[0].Draw.dfe = 1;
    pM->Db[1].Draw.dtd = 0;
    pM->Db[0].Draw.dtd = 0;
    pM->Rect.x = 0;
    pM->Rect.y = 0;
    pM->Rect.w = 320;
    pM->Rect.h = 240;
    func_80094374(0);
    func_800943C8();
    _bu_init();
    PadInitDirect(pG->Padd_data[0], pG->Padd_data[1]);
    func_800912C4();
    Game.Unk4F = 0;
    Init_global();
    Moji_init();
    Line_work_init();
    pG->Unk2114 = 0;
    Task_execute(0, Init_main);
    ResetGraph(0);
    Vram_clr(0, 0, 0x400, 0x200);
    D_800D454C = 0;
}

void Init_global(void)
{
    tagGLOBAL *pG;
    TILE *pT;
    ULONG i;
    ULONG j;

    pG = &G;
    if (pG->System_flg & 0x80) {
        func_80078FE8();
    }
    func_80010150(&pG->Unk2468, 0x5A90);
    if (pG->System_flg & 0x80) {
        func_8007906C();
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            pT = &Main.Db[i].Curtain[j];
            SetTile(pT);
            pT->x0 = 0;
            pT->y0 = j * 216;
            pT->w = 320;
            pT->h = 24;
            setSemiTrans(pT, 1);
        }
        SetDrawTPage(&Main.Db[i].Curtain_Dm, 0, 0, GetTPage(0, 2, 0, 0));
    }
    G.Unk246C = 0;
    *(ULONG *)pG->Padd_data[0] = 0xFFFF0000;
    *(ULONG *)pG->Padd_data[1] = 0xFFFF0000;
    pG->Room_no_old = -1;
    pG->Cut_no_old = -1;
    D_800DBB58 = 0;
    Main.Unk194 = 0;
    G.System_flg &= 0x80;
    D_80098700 = 0xD2706CA4;
    for (i = 0; i < 4; i++) {
        Fade_off(i);
    }
    for (i = 0; i < 2; i++) {
        pT = &Main.Db[i].Pause;
        SetTile(pT);
        SetSemiTrans(pT, 1);
        pT->w = 320;
        pT->r0 = 0;
        pT->g0 = 0;
        pT->b0 = 0;
        pT->x0 = 0;
        pT->y0 = 0;
        pT->h = 240;
        SetDrawTPage(&Main.Db[i].Pause_Dm, 0, 0, 0);
    }
}

extern SHORT D_800CCBBC;
void Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
void Mapping_tim(ULONG *p);
void Task_lchain(ULONG Task_no);
int PadGetState(int port);
int PadSetMainMode(int socket, int offs, int lock);

void Init_main(void)
{
    tagMAIN *pMain;
    DBUF *pDb;
    LONG i;

    pMain = &Main;
    Card_event_set();
    Cd_read(0x63, (ULONG *)0x8011A000, 1, "TEX_TIM");
    D_800CCBBC = 0x1C;
    Mapping_tim((ULONG *)0x8011A000);
    for (i = 0; i < 2; i++) {
        pDb = &pMain->Db[i];
        SetTile(&pDb->Bg_clr);
        *(ULONG *)&pDb->Bg_clr.r0 = 0x440000;
        *(ULONG *)&pDb->Bg_clr.x0 = 0;
        *(ULONG *)&pDb->Bg_clr.w = 0xF00140;
    }
    pMain->Bg_mode = 1;
    pMain->Unk196 = 0;
    if (PadGetState(0) != 0) {
        while (PadGetState(0) != 2 && PadSetMainMode(0, 1, 2) == 0) {
        }
    }
    DrawSync(0);
    SetDispMask(1);
    Bg_set_mode(1, 0);
    Main.Unk193 = 0;
    Task_lexecute(1, (void *)1);
    while (Main.Unk193 == 0) {
        Task_sleep(1);
    }
    G.System_flg |= 0x20000000;
    Task_lchain(0);
}

void func_80029D00(void) {
}

void System_trans_all(void)
{
    func_8002A49C();
    Curtain_trans();
    Moji_trans_main();
}

void Curtain_trans(void)
{
    tagMAIN *pMain = &Main;
    DBUF *pDb = pMain->pDb;

    if (G.Status_flg & STAT_CINESCO) {
        if (pMain->Curtain_kido < 0xF0) {
            pMain->Curtain_kido += 0x10;
        } else {
            pMain->Curtain_kido = 0xFF;
        }
    } else if (pMain->Curtain_kido >= 0x10) {
        pMain->Curtain_kido -= 0x10;
    } else {
        pMain->Curtain_kido = 0;
    }
    pDb->Curtain[0].r0 = pDb->Curtain[0].g0 = pDb->Curtain[0].b0 =
        pDb->Curtain[1].r0 = pDb->Curtain[1].g0 = pDb->Curtain[1].b0 = pMain->Curtain_kido;
    if (!(G.System_flg & SYS_DOOR_INIT) && pMain->Curtain_kido != 0) {
        AddPrim(G.pOt0 + 4, &pDb->Curtain[0]);
        AddPrim(G.pOt0 + 4, &pDb->Curtain[1]);
        AddPrim(G.pOt0 + 4, &pDb->Curtain_Dm);
    }
}

void Psp_set(void);
void func_80048FF4(void);
void Set_light_default(void);
void Set_view(LONG *pView_p, LONG *pView_r);
void func_80010000(ULONG *pSrc, ULONG *pDst);
void func_8008F8A4(LONG mode);
void func_8008FAD4(LONG mode);
extern USHORT *D_8009DFD0[];           /* per stage room file numbers */

void Bg_draw(void)
{
    RECT Rect;
    volatile UCHAR pad_save0;
    volatile UCHAR pad_save1;
    tagGLOBAL *pG;
    USHORT *pBs_size;
    ULONG Size;
    USHORT Ctpage_save;
    LONG i;
    SHORT n;

    pad_save0 = PadActData[0];
    pad_save1 = PadActData[1];
    PadActData[1] = 0;
    PadActData[0] = 0;
    pG = &G;
    Ctpage_save = pG->Unk2484;
    if (Main.Unk18E != 2) {
        n = pG->Next_cut_no;
        pG->Room_no_old = pG->Cut_no;
        pG->Cut_no = n;
        pG->Cccut = Ccut_search(n);
        if (!(pG->System_flg & 0x80000000)) {
            Psp_set();
        }
        pBs_size = (USHORT *)(pG->Unk7840 * 2 + (ULONG)pG->pBs_size);
        Size = pG->pUnk2128[*pBs_size + pG->Cut_no];
        CD.Check_sum_flg = 2;
        CD.Fsum = pG->pBs_check_sum[*pBs_size + pG->Cut_no];
        CD.Fsize = Size;
        CD.Fno = D_8009DFD0[pG->Unk783E][pG->Unk7840 + 1] - 1;
        CD.Fsector = (floc[CD.Fno].sec_high << 16) + floc[CD.Fno].sector + pG->Cut_no * 32;
        Cd_read(CD.Fno, (ULONG *)0x80194000, 2, "BS");
        CD.Check_sum_flg = 0;
        if (*(ULONG *)(0x80193FFC + Size) != 0) {
            func_80010000((ULONG *)(*(ULONG *)(0x80193FF8 + Size) + 0x80194000), (ULONG *)0x801AC000);
            pG->Unk2484 = 0x15;
            Mapping_tim((ULONG *)0x801AC000);
            pG->Unk2484 = Ctpage_save;
        }
    }
    func_8008F8A4(0);
    func_8008FB34(0);
    DecDCTvlc((ULONG *)0x80194000, (ULONG *)0x801AA000);
    func_8008F9FC((ULONG *)0x801AA000, 0);
    func_8008FA78((ULONG *)0x8019C000, 0x780);
    Rect.x = 0;
    Rect.y = Main.Rect.y = Main.pDb->Draw.ofs[1];
    Rect.w = 16;
    Rect.h = 240;
    SetGeomScreen(pG->pRoom->pRcut[pG->Cut_no].ViewR >> 7);
    Set_view(pG->pRoom->pRcut[pG->Cut_no].View_p, pG->pRoom->pRcut[pG->Cut_no].View_r);
    Sce.Cut = pG->pRoom->pRcut[pG->Cut_no];
    if (G.Status_flg & 0x1000) {
        func_80048FF4();
    }
    Set_light_default();
    func_8008FAD4(0);
    for (i = 1; i < 20;) {
        func_8008FA78((ULONG *)0x8019C000 + (i % 2) * 0x780, 0x780);
        LoadImage(&Rect, (ULONG *)0x8019C000 + ((i - 1) % 2) * 0x780);
        i++;
        Rect.x += 16;
        func_8008FAD4(0);
    }
    LoadImage(&Rect, (ULONG *)0x8019DE00);
    StoreImage(&Main.Rect, (ULONG *)0x8019C000);
    PadActData[0] = pad_save0;
    PadActData[1] = pad_save1;
}

void Soft_reset(void)
{
    RECT rect;

    Pad_vib_init();
    Sce_work_clr_set();
    rect.w = 0x140;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1E0;
    DrawSync(0);
    VSync(0);
    ResetCallback();
    ClearImage(&rect, 0, 0, 0);
    Bg_set_mode(1, 0);
    Task_kill(0);
    Task_kill(1);
    Task_kill(2);
    Snd_reset();
    Init_global();
    VSync(0);
    G.Unk20F8 = -1;
    Task_lexecute(0, NULL);
}

void func_8002A2B8(void)
{
    tagGLOBAL *pG;
    tagMAIN *pMain;

    Print_color(0xFF, 0, 0x100);
    Print14(0x87, 0x6B, "Paused");
    pG = &G;
    pMain = &Main;
    AddPrim(pG->pOt0 + 1, &pMain->pDb->Pause);
    AddPrim(pG->pOt0 + 1, &pMain->pDb->Pause_Dm);
}

void Bg_set_mode(ULONG Mode, ULONG Rgb)
{
    Main.Bg_mode = Mode;
    if (Mode == 1) {
        *(ULONG *)&Main.Db[1].Bg_clr.r0 = Rgb;
        *(ULONG *)&Main.Db[0].Bg_clr.r0 = Rgb;
    }
}

void Fade_set(ULONG No, UCHAR Pri, ULONG Flg, ULONG Abr, ULONG Rgb0, ULONG Rgb1, USHORT Cnt_max)
{
    FADE *pF;
    USHORT tpage;

    pF = &Main.Fade[No];
    pF->Rgb0[0] = Rgb0;
    pF->Rgb0[1] = Rgb0 >> 8;
    pF->Rgb0[2] = Rgb0 >> 16;
    pF->Rgb1[0] = Rgb1;
    pF->Rgb1[1] = Rgb1 >> 8;
    pF->Rgb1[2] = Rgb1 >> 16;
    tpage = GetTPage(0, Abr, 0, 0);
    SetDrawTPage(&pF->Tpage[0], 1, 0, tpage);
    SetDrawTPage(&pF->Tpage[1], 1, 0, tpage);
    Flg |= FADE_ON;
    pF->Rect.w = 0x140;
    pF->Pri = Pri;
    pF->Flg = Flg;
    pF->Cnt_max = Cnt_max;
    pF->Cnt = 0;
    pF->Rect.x = 0;
    pF->Rect.y = 0;
    pF->Rect.h = 0xF0;
}

void Fade_rect_set(ULONG No, RECT *Rect)
{
    FADE *pF = &Main.Fade[No];

    pF->Rect.x = Rect->x;
    pF->Rect.y = Rect->y;
    pF->Rect.w = Rect->w;
    pF->Rect.h = Rect->h;
}

INCLUDE_ASM("asm/nonmatchings/game/main", func_8002A49C);

void Fade_off(ULONG No)
{
    Main.Fade[No].Flg = 0;
}

ULONG Fade_status(ULONG No)
{
    return Main.Fade[No].Flg;
}

void Fade_out_reset(void)
{
    tagGLOBAL *pG = &G;
    FADE *pF = &Main.Fade[0];
    TILE *pT = &pF->T[pG->Cbuf_id];

    switch (pF->Flg) {
    case 0:
        DrawSync(0);
        PutDispEnv(&Main.pDb->Disp);
        *(ULONG *)&pT->tag = 0x3000000;
        *(ULONG *)&pT->r0 = 0x62202020;
        *(ULONG *)&pT->x0 = 0;
        *(ULONG *)&pT->w = 0xF00140;
        SetDrawTPage(&pF->Tpage[pG->Cbuf_id], 1, 0, GetTPage(0, 2, 0, 0));
        pF->Cnt_max = 8;
        pF->Flg = 1;
    case 1:
        AddPrim(pG->pOt0, pT);
        AddPrim(pG->pOt0, &pF->Tpage[pG->Cbuf_id]);
        DrawSync(0);
        VSync(0);
        DrawOTag(pG->pOt0 + 7);
        DrawSync(0);
        if (--pF->Cnt_max == 0) {
            pF->Flg = 0;
            G.System_flg &= ~SYS_FADE_OUT;
        }
        break;
    }
}

ULONG Cut_check(ULONG Flg)
{
    tagGLOBAL *pG = &G;
    VCUT *cp;
    ULONG next_cut;

    for (cp = pG->Cccut_next + 1; cp->Fcut == pG->Next_cut_no; cp++) {
        if (cp->Be_flg != 0 && (cp->nFloor == -0x80 || cp->nFloor == pG->Pl_nFloor) &&
            func_8001020C(&pG->Pl_pos_x, cp) != 0) {
            next_cut = cp->Tcut;
            goto apply_cut;
        }
    }
    if (Flg == 0) {
        return 0;
    }
    next_cut = pG->Next_cut_no;
apply_cut:
    Cut_change(next_cut);
    return 1;
}

void Cut_change(ULONG Fc)
{
    G.Next_cut_no = Fc;
    G.Cccut_next = Ccut_search(Fc);
}

VCUT *Ccut_search(ULONG Fc)
{
    VCUT *pCcut;

    pCcut = G.pRoom->pVcut;
    while (pCcut->Fcut != Fc) {
        pCcut++;
    }
    return pCcut;
}

void Card_event_set(void)
{
    EnterCriticalSection();
    Ev0 = OpenEvent(0xF4000001, 4, 0x2000, NULL);
    Ev1 = OpenEvent(0xF4000001, 0x100, 0x2000, NULL);
    Ev2 = OpenEvent(0xF4000001, 0x2000, 0x2000, NULL);
    Ev3 = OpenEvent(0xF4000001, 0x8000, 0x2000, NULL);
    Ev10 = OpenEvent(0xF0000011, 4, 0x2000, NULL);
    Ev11 = OpenEvent(0xF0000011, 0x100, 0x2000, NULL);
    Ev13 = OpenEvent(0xF0000011, 0x8000, 0x2000, NULL);
    ExitCriticalSection();
    EnableEvent(Ev0);
    EnableEvent(Ev1);
    EnableEvent(Ev2);
    EnableEvent(Ev3);
    EnableEvent(Ev10);
    EnableEvent(Ev11);
    EnableEvent(Ev13);
}
