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
    UCHAR Unk1;                        /* 0x1 */
    UCHAR Mess_no;                     /* 0x2 message shown by func_80024CE8 */
    UCHAR Unk3;                        /* 0x3 */
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
extern LONG D_800D454C;                /* right after Main.Fade[] */
extern UCHAR D_800DBB58;
extern UCHAR D_800996E4[];             /* message text banks (Mess_set) */
extern USHORT D_8009BDB4[];
extern UCHAR D_80098E88[];
extern USHORT D_80099654[];
extern ULONG D_80098700;
void func_80078FE8(ULONG *p);
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

INCLUDE_ASM("asm/nonmatchings/game/game", Game_loop);

void Gun_light_set(void)
{
    LIGHT_CUT_DATA *pLight;
    SVECTOR v;

    pLight = Get_light_cut(&G.Pl_pos_x);
    func_800100A4(Game.Plight_bak, pLight, 0x28);
    pLight->Mode[2] = 0;
    pLight->L[2] = 1;
    v.vx = 0x4B0;
    v.vz = 0;
    Rot_vector(G.Pl_cdir_y, &v, &v);
    Game.Plight_col[0] = 0xFF;
    Game.Plight_col[1] = 0xE6;
    Game.Plight_col[2] = 0xA0;
    Game.Plight_pos.vx = G.Pl_pos_x + v.vx;
    Game.Plight_pos.vy = G.Pl_pos_y - 0x960;
    Game.Plight_pos.vz = G.Pl_pos_z + v.vz;
    switch (Game.Plight_cnt) {
    case 0:
        {
            UCHAR Col_lvl = 0xFF;

            if (pLight->Col[2][0] < Col_lvl) {
                pLight->Col[2][0] = Col_lvl;
            }
        }
        if (pLight->Col[2][1] < Game.Plight_col[1]) {
            pLight->Col[2][1] = Game.Plight_col[1];
        }
        if (pLight->Col[2][2] < Game.Plight_col[2]) {
            pLight->Col[2][2] = Game.Plight_col[2];
        }
        Game.Plight_cnt++;
        break;
    case 1:
        {
            UCHAR Col_lvl = 0x7F;

            if (pLight->Col[2][0] < Col_lvl) {
                pLight->Col[2][0] = Col_lvl;
            }
        }
        if (pLight->Col[2][1] < Game.Plight_col[1] >> 1) {
            pLight->Col[2][1] = Game.Plight_col[1] >> 1;
        }
        if (pLight->Col[2][2] < Game.Plight_col[2] >> 1) {
            pLight->Col[2][2] = Game.Plight_col[2] >> 1;
        }
        Game.Plight_cnt++;
        break;
    }
    pLight->Pos[2][0] = Game.Plight_pos.vx;
    pLight->Pos[2][1] = Game.Plight_pos.vy;
    pLight->Pos[2][2] = Game.Plight_pos.vz;
    pLight->L[2] = 6000;
}

void Gun_light_reset(void)
{
    func_800100A4(Get_light_cut(&G.Pl_pos_x), Game.Plight_bak, 0x28);
    if (Game.Plight_cnt >= 2) {
        Game.Plight_flg = 0;
        Game.Plight_cnt = 0;
    }
}

void func_80024B34(void);
void Esp_init_C(void);
void Trans_work_init(void);
void func_8006D720(void);
void Em_move_tbl_set(void);
void Spl_set(void);
void Set_stage(void);
void func_80077C4C(void);
void Set_room(void);
void func_80077F60(void);
void Player_set(ENEMY_WORK *pEm);

extern UCHAR D_800E0186[];             /* an array: stores to it stay after loads of struct fields */
extern UCHAR D_800D442C;

/* game start: place the player (new game, demo or loaded save), load the stage and room */
void func_800245A0(void)
{
    tagGLOBAL *pG = &G;
    tagMAIN *pMain = &Main;
    ENEMY_WORK *pPl;
    ULONG Be;

    Bg_set_mode(1, 0);
    func_80038634(0, 0);
    pG->pUnk265C = PLAYER;
    pPl = PLAYER;
    if (pG->System_flg & 0x1000) {
        if (pG->System_flg & 0x80) {
            pPl->Max_life = pPl->Life = 200;
            pPl->Pos_x = pPl->Unk40[0] = -18415;
            pPl->Pos_z = pPl->Unk40[2] = -22736;
            pPl->Cdir_y = 0x4A8;
            pPl->Pos_y = pPl->Unk40[1] = 0;
            pPl->Unk122 = 0;
            pPl->nFloor = 0;
            pG->Unk783E = 6;
            pG->Unk7840 = 12;
            pG->Cut_no = 2;
        } else {
            pPl->Max_life = pPl->Life = 200;
            pPl->Pos_x = pPl->Unk40[0] = 13262;
            pPl->Pos_z = pPl->Unk40[2] = -12537;
            pPl->Cdir_y = 0x720;
            pPl->Pos_y = pPl->Unk40[1] = 0;
            pPl->Unk122 = 0;
            pPl->nFloor = 0;
            pPl->Status_flg = 0;
            pG->Unk7840 = 13;
            pG->Unk783E = 0;
            pG->Cut_no = 11;
            pG->Unk7864 = 0;
            pG->Unk784E = 0;
        }
    } else if (pG->System_flg & 0x10000000) {
        func_80024B34();
        pG->pUnk7C7C = G.Item_work[G.Item_work_no];
        goto set;
    } else {
        pPl->Max_life = 200;
        pPl->Unk40[0] = pPl->Pos_x;
        pPl->Unk40[1] = pPl->Pos_y;
        pPl->Unk40[2] = pPl->Pos_z;
        pPl->Unk122 = pPl->Pos_y;
        pPl->nFloor = (SHORT)pPl->Pos_y / -1800;
    }
set:
    pPl->Be_flg = 1;
    ((ENEMY_WORK *)pG->pad2708)->Col_id = 0;
    Esp_init_C();
    Trans_work_init();
    func_8006D720();
    Em_move_tbl_set();
    Spl_set();
    Set_stage();
    func_80077C4C();
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
    }
    Set_room();
    func_80077F60();
    pG->pEm = (ENEMY_WORK *)&pG->Pl_be_flg;
    Player_set(pG->pEm);
    SetBackColor(0x80, 0x80, 0x80);
    pMain->Unk191 = 0;
    Bg_set_mode(2, 0);
    pMain->Unk18E = 1;
    SetGeomOffset(160, 120);
    while (Main.Unk193) {
        Task_sleep(1);
    }
    pG->Game_time_offset = VSync(-1);
    D_800D442C = 2;
    Game.Unk1 = 0;
    G.System_flg |= 0x8000000;
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
        Game.Unk4E = 1;
    }
    Task_sleep(1);
}

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

void Set_door(void)
{
    tagGLOBAL *pG;
    DOOR_NEXT *pAot;
    UCHAR n;

    pG = &G;
    pG->Unk2470 = 0;
    if (pG->Status_flg & 0x40000000) {
        func_80024C58();
    }
    pG->Pl_damage_cnt = 0;
    pG->Pl_be_flg &= 0x9F4F;
    *(USHORT *)&pG->Unk24D2 &= 0xBEFF;
    pG->System_flg |= 0x400000;
    Bg_set_mode(1, 0);
    Task_execute(1, Door_main);
    Task_sleep(1);
    Door_snd_trans();
    pAot = (DOOR_NEXT *)pG->pSce_aot_on;
    pG->Pl_pos_x = pG->Pl_old_x = pAot->Next_pos_x;
    pG->Unk25AE = pG->Pl_pos_y = pG->Pl_old_y = pAot->Next_pos_y;
    pG->Pl_pos_z = pG->Pl_old_z = pAot->Next_pos_z;
    pG->Pl_cdir_y = pAot->Next_cdir_y;
    pG->Pl_nFloor = pAot->Next_nfloor;
    n = pAot->Next_cut;
    pG->Next_cut_no = n;
    pG->Cut_no_old = pG->Unk7840 + (pG->Unk783E + 1) * 0x100;
    pG->Cut_no = n;
    pG->Unk7840 = pAot->Next_room;
    if (pG->Unk783E != pAot->Next_stage % 9) {
        pG->Unk783E = pAot->Next_stage % 9;
        Set_stage();
    }
    func_80077C4C();
    while (pG->Status_flg & 0x800) {
        Task_sleep(1);
    }
    Set_room();
    while (G.System_flg & 0x400000) {
        Task_sleep(1);
    }
    func_80077F60();
    Bg_set_mode(2, 0);
    Cut_check(1);
    Main.Unk18E = 1;
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
        Game.Unk4E = 1;
    }
    Task_sleep(1);
}

void func_80024B34(void)
{
    ENEMY_WORK *pPl;
    SAVE_POS *pSave;
    tagMAIN *pMain;
    tagGAME *pGame;
    tagGLOBAL *pG;

    pG = &G;
    pMain = &Main;
    pGame = &Game;
    pPl = (ENEMY_WORK *)((UCHAR *)pG + 0x248C);
    pSave = pGame->pSave;
    func_800100A4((UCHAR *)pG + 0x77F0, pSave, 0x708);
    pPl->Pos_x = pPl->Unk40[0] = pSave->Pos_x;
    pPl->Pos_y = pPl->Unk40[1] = pSave->Pos_y;
    pPl->Pos_z = pPl->Unk40[2] = pPl->Unk122 = pSave->Pos_z;
    pPl->nFloor = pSave->Pos_y / -1800;
    pPl->Cdir_y = pSave->Cdir_y;
    pPl->Life = pSave->Life;
    pPl->Id = pSave->Id;
    pGame->Unk4D = pMain->Unk196;
    pMain->Unk196 = 0;
    pGame->Unk4C = pMain->Unk195;
    pMain->Unk195 = pSave->Unk713;
}

void func_80024C50(void) {
}

void func_80024C58(void)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;
    LONG i;

    for (ppEm = G.pEnemy; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        if ((ULONG)(pEm->Id - 0x34) < 3) {
            G.Unk7856 = pEm->Life;
            G.Unk7854 = pEm->Type & 1;
            for (i = 0; (ULONG)i < 12; i++) {
                G.Unk7D90[i] = pEm->Unk1CC[i];
            }
        }
    }
}

LONG Mess_set(ULONG Pos_xy, USHORT Attr, ULONG Mess_no, ULONG Stop_data);
void Pad_vib_init(void);
ULONG func_80024F74(void);

void func_80024CE8(void)
{
    ULONG i;

    G.System_flg |= 0x400;
    D_800D442C = 3;
    G.Status_flg &= ~4;
    G.Status_flg &= ~2;
    G.Status_flg &= ~1;
    Pad_vib_init();
    Flash_fade_in();
    Fade_set(2, 0, 0, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Task_sleep(1);
    D_800C703C.Flg = 1;
    Flash_fade_out();
    Flash_fade_in();
    Mess_set(0, 0x2001, D_800C703C.Mess_no, 0);
    D_800C703C.Unk3 = 0x62;
    D_800C703C.Cnt = 0;
    Flash_fade_out();
    Flash_fade_in();
    Fade_set(2, 0, 0, 1, 0xFFFFFF, 0xFFFFFF, 1);
    D_800C703C.Unk3 = 0xB3;
    Task_sleep(1);
    D_800C703C.Flg = 0;
    Fade_set(2, 0, 0, 1, 0xFFFFFF, 0, 4);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
    D_800C703C.Unk1 = 1;
    D_800D442C = 2;
    G.System_flg &= ~0x400;
    if (!func_80024F74() && !func_80024F74()) {
        D_800C703C.Cnt++;
        while (G.Status_flg & 0x10000000) {
            Task_sleep(1);
        }
    }
    G.System_flg |= 0x400;
    D_800C703C.Flg = 1;
    func_8003884C(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    i = 7;
    do {
        Task_sleep(1);
    } while (i-- != 0);
    D_800C703C.Flg = 0;
    D_800D442C = 2;
    D_800C703C.Unk1 = 0;
    G.System_flg &= ~0x400;
    Task_exit();
}

ULONG func_80024F74(void)
{
    ULONG i;

    D_800C703C.Cnt++;
    if (Main.Unk18E == 0) {
        do {
            Task_sleep(1);
            if (!(G.Status_flg & 0x10000000)) {
                return 1;
            }
        } while (Main.Unk18E == 0);
    }
    Main.Unk18E = 0;
    D_800C703C.Flg = 1;
    G.System_flg |= 0x400;
    func_8003884C(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    i = 7;
    do {
        Task_sleep(1);
        if (!(G.Status_flg & 0x10000000)) {
            return 1;
        }
    } while (i-- != 0);
    Main.Unk18E = 1;
    G.System_flg &= ~0x400;
    Task_sleep(1);
    if (!(G.Status_flg & 0x10000000)) {
        return 1;
    }
    D_800C703C.Flg = 0;
    return 0;
}

void Flash_fade_in(void)
{
    func_8003884C(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    Fade_set(2, 0, 0, 1, 0, 0xFFFFFF, 4);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
}

void Flash_fade_out(void)
{
    ULONG i;

    Fade_set(2, 0, 0, 2, 0xFFFFFF, 0, 3);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
    Fade_set(2, 0, FADE_HOLD, 1, 0, 0x606060, 1);
    i = 0x22;
    do {
        Task_sleep(1);
    } while (i-- != 0);
}
