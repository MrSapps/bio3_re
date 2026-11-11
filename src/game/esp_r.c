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

INCLUDE_ASM("asm/nonmatchings/game/esp_r", func_80020298);

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

void func_800204B8(ESP3_WORK *pEsp)
{
    ESP_PRM4B8 Tbl[4] = {
        { 15, 1, 11, 10, 2, -200, 4, 0 },
        { 25, 1, 11, 10, 2, -200, 5, 0 },
        { 5, 0, 0, 0, 0, 0, 0, 0 },
        { 6, 1, 10, 40, 3, -100, 10, 0 },
    };
    ESP3_WORK *pNew;
    ESP_PRM4B8 *pT;
    UCHAR r;

    r = pEsp->Routine_1;
    if (!(pEsp->UnkB4 & 1)) {
        pT = &Tbl[r];
        if (pT->Flg != 0) {
            pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pT->Attr << 8), (pEsp->Unk2E << 16) | (D_80098728[func_800102E8()] << 4), pEsp->pMat, &pEsp->Unk38, 0);
            if (pNew != (ESP3_WORK *)-1) {
                pNew->Unk10 = pT->Base10 + pEsp->UnkB4 * pT->Mul10 + (D_80098728[func_800102E8()] & 0xF);
                pNew->Unk12 = pT->Base12 + pEsp->UnkB4 * pT->Mul12 + (D_80098728[func_800102E8()] & 0x1F);
            }
        }
    }
    if (Tbl[r].Cnt < ++pEsp->UnkB4) {
        pEsp->Routine_1++;
        if (r != 0) {
            pEsp->UnkB4 = 0;
        }
    }
    if (pEsp->Routine_1 == 4) {
        pEsp->Unk24 = 0;
    }
}

void func_80020700(ESP3_WORK *pEsp)
{
    ESP3_WORK *pNew;
    ULONG i;

    for (i = 0; i < 3; i++) {
        pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pEsp->Unk19 << 8) | pEsp->Unk18, (pEsp->Unk2E << 16) | (D_80098728[func_800102E8()] << 4), pEsp->pMat, &pEsp->Unk38, 0);
        if (pNew != (ESP3_WORK *)-1) {
            pNew->Unk10 = (D_80098728[func_800102E8()] & 0x3F) + 0x1E;
            pNew->Unk12 = (D_80098728[func_800102E8()] & 0x7F) - 0x8C;
        }
    }
    if (++pEsp->UnkB4 >= 3) {
        pEsp->Unk24 = 0;
    }
}

void func_80020820(ESP3_WORK *pEsp)
{
    SHORT Tbl[3][4] = { { 2, 0, 50, -50 }, { 4, 0xE00, 60, -80 }, { 2, 0xA00, 70, -100 } };
    ESP3_WORK *pNew;
    ULONG n;
    ULONG i;

    n = pEsp->Routine_1;
    pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xC00, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + (signed char)D_80098728[func_800102E8()] * Tbl[n][0] + Tbl[n][1]), pEsp->pMat, &pEsp->Unk38, 0);
    if (pNew != (ESP3_WORK *)-1) {
        pNew->Unk10 = Tbl[n][2];
        pNew->Unk12 = Tbl[n][3] + (D_80098728[func_800102E8()] & 0xF);
        if (pNew->Unk18 != 0) {
            pNew->Be_flg = 0;
            pNew->Unk24 &= ~0x800;
        }
        for (i = 0; i < pEsp->Unk19; i++) {
            func_8001BC80(pNew);
        }
    }
    if (++pEsp->Routine_1 == 3) {
        pEsp->Unk24 = 0;
    }
}

void func_800209D8(ESP3_WORK *pEsp)
{
    ESP3_WORK *pNew;

    switch (pEsp->Routine_1) {
    case 0:
        pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xC00, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + ((signed char)D_80098728[func_800102E8()] * 2 + 0x800)), pEsp->pMat, &pEsp->Unk38, 0);
        if (pNew != (ESP3_WORK *)-1) {
            pNew->Unk10 = (D_80098728[func_800102E8()] & 0x3F) + 0x32;
            pNew->Unk12 = (D_80098728[func_800102E8()] & 0x1F) - 0x1E;
            if (pNew->Unk18 != 0) {
                pNew->Be_flg = 0;
                pNew->Unk24 &= ~0x800;
            }
        }
        pEsp->Routine_1++;
    case 1:
        pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xC00, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + 0x800), pEsp->pMat, &pEsp->Unk38, 0);
        if (pNew != (ESP3_WORK *)-1) {
            pNew->Unk10 = 0xB4;
            pNew->UnkD = 0;
            if (pNew->Unk18 != 0) {
                pNew->Be_flg = 0;
                pNew->Unk24 &= ~0x800;
            }
        }
        pEsp->Unk24 = 0;
        break;
    }
}

void func_80020B6C(ESP3_WORK *pEsp)
{
    if (pEsp->UnkBA < 0x14) {
        pEsp->UnkB8 += 0x50;
        pEsp->UnkA += pEsp->UnkB8;
        pEsp->UnkBA++;
    }
    if (pEsp->Unk26 & 0x30) {
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0x139, pEsp->Unk2E << 16, &GsIDMATRIX, &pEsp->Pos, 0);
        pEsp->Unk24 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/esp_r", func_80020C0C);

void func_80020DBC(ESP3_WORK *pEsp)
{
    UCHAR Tbl[5] = { 4, 5, 0, 0, 7 };
    SVECTOR Dir;

    if (pEsp->Unk26 & 0x30) {
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xD00, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, 0);
        Dir.vx = 0x400;
        Dir.vy = 0;
        Dir.vz = 0;
        func_8001B35C(pEsp, ((Tbl[pEsp->UnkB3] & 0xF) << 24) | 0xA05, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, &Dir);
        pEsp->Unk24 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/esp_r", func_80020EB8);

typedef struct tagESP_HIT {
    VECTOR *pPos;                      /* 0x0 */
    SHORT Unk4[3];                     /* 0x4 */
    UCHAR pad[2];
    SHORT Unk0C;                       /* 0xC */
} ESP_HIT;
extern ULONG D_80097FCC[];
LONG func_800472EC(ESP_HIT *pHit, ULONG Arg);

void func_80021548(ESP3_WORK *pEsp)
{
    ESP_HIT Hit;
    VECTOR v;
    LONG r1;
    LONG a;
    LONG r2;

    pEsp->Unk2E += pEsp->Unk1C;
    if (pEsp->Unk29 < 2) {
        if ((SHORT)(pEsp->UnkB4 % 3) == 0) {
            r1 = (signed char)func_800102E8() << (pEsp->UnkB4 / (100 - pEsp->Unk29 * 30) + 1);
            r2 = (signed char)func_800102E8() << (pEsp->UnkB4 / (100 - pEsp->Unk29 * 30) + 1);
            v.vx = pEsp->Pos.vx + r1;
            v.vy = pEsp->Pos.vy;
            v.vz = pEsp->Pos.vz + r2;
            a = ((func_800102E8() & 1) << 8) | 0x2D;
            func_8001B35C(pEsp, a, (((signed char)func_800102E8() << 1) + 0xC00) << 16, &GsIDMATRIX, &v, 0);
        }
        if (pEsp->UnkB4 >= 4 && pEsp->UnkB6 == 0) {
            Hit.pPos = &pEsp->Pos;
            Hit.Unk4[0] = 0;
            Hit.Unk4[1] = 0;
            Hit.Unk4[2] = 0;
            Hit.Unk0C = pEsp->Unk2C;
            if (func_800472EC(&Hit, D_80097FCC[pEsp->Unk18]) < 0) {
                pEsp->UnkB6 = 1;
                func_8001B35C(pEsp, 0x1503, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, 0);
            }
        }
    }
    pEsp->UnkB4++;
}

void func_8002174C(ESP3_WORK *pEsp)
{
    if (pEsp->UnkB4 >= 3) {
        pEsp->Be_flg = 0;
        pEsp->Unk24 &= ~0x800;
    }
    pEsp->UnkB4++;
}

void func_80021784(void) {
}

void func_8002178C(void) {
}

void func_80021794(void) {
}

void func_8002179C(void) {
}

void func_800217A4(void) {
}

void func_800217AC(void) {
}

void func_800217B4(ESP3_WORK *pEsp)
{
    switch (pEsp->Routine_1) {
    case 0:
        func_8001B35C(pEsp, (((pEsp->UnkB3 + 1) & 0xF) << 24) | 0x905, (pEsp->Unk2E << 16) | pEsp->Unk2C, pEsp->pMat, &pEsp->Unk38, 0);
        pEsp->UnkB6 = (UCHAR)(D_80098728[func_800102E8()] % 50) + 40;
        pEsp->UnkB4 = 0;
        pEsp->Routine_1++;
        break;
    case 1:
        if (pEsp->UnkB6 < ++pEsp->UnkB4) {
            pEsp->Routine_1 = 0;
        }
        break;
    }
}

void func_800218BC(ESP3_WORK *pEsp)
{
    ULONG Cnt;

    if (pEsp->Unk26 & 0x30) {
        pEsp->Unk24 = 0;
    }
    pEsp->Unk2E -= 0x50;
    if (pEsp->Unk2E < 0x100) {
        pEsp->Unk24 = 0;
    }
    Cnt = (USHORT)pEsp->UnkB4;
    if ((Cnt & 1) && Cnt - 2 < 0x17) {
        func_8001B35C(pEsp, 0x1000003, (pEsp->Unk2E << 17) | (USHORT)((signed char)func_800102E8() << 4), &GsIDMATRIX, &pEsp->Pos, 0);
    }
    pEsp->UnkB4++;
}

INCLUDE_ASM("asm/nonmatchings/game/esp_r", func_8002198C);

INCLUDE_ASM("asm/nonmatchings/game/esp_r", func_80021D58);

void func_80021EA4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/esp_r", func_80021EAC);

void func_800221F0(ESP3_WORK *pEsp)
{
    VECTOR v;
    ESP3_WORK *pNew;

    if (!(pEsp->UnkB4 & 1)) {
        v.vx = (signed char)func_800102E8();
        v.vy = (signed char)func_800102E8();
        v.vz = (signed char)func_800102E8();
        pNew = func_8001B35C(pEsp, 0x81C, ((pEsp->Unk2E + pEsp->UnkB4 * 0x300 + func_800102E8()) << 16) | pEsp->Unk2C, &pEsp->Mat, &v, 0);
        if (pNew != (ESP3_WORK *)-1) {
            pNew->Unk12 = pEsp->UnkB4 * 30 - 300;
        }
    } else {
        v.vx = (signed char)func_800102E8();
        v.vy = (signed char)func_800102E8() - pEsp->UnkB4 * 300 - 50;
        v.vz = (signed char)func_800102E8();
        pNew = func_8001B35C(pEsp, 0x81C, ((pEsp->Unk2E + 0x1300 - (pEsp->UnkB4 << 8) + func_800102E8()) << 16) | pEsp->Unk2C, &pEsp->Mat, &v, 0);
        if (pNew != (ESP3_WORK *)-1) {
            pNew->Unk12 = -30;
        }
    }
    if (pEsp->UnkB4 < 8) {
        pEsp->UnkB4++;
    } else {
        pEsp->Unk24 = 0;
    }
}

void func_80022398(void) {
}

void func_800223A0(void) {
}

void func_800223A8(ESP3_WORK *pEsp)
{
    ESP3_WORK *pNew;

    if (pEsp->UnkB4 < 5) {
        pNew = func_8001B35C(pEsp, 0x3001F03, ((((0x3000 - pEsp->UnkB4 * 2000) * pEsp->Unk2E) >> 12) << 16) | (USHORT)(pEsp->Unk2C + (signed char)D_80098728[func_800102E8()]), &GsIDMATRIX, &pEsp->Pos, 0);
        if (pNew != (ESP3_WORK *)-1) {
            pNew->UnkD = (pNew->UnkD * pNew->Unk2E) >> 12;
            pNew->Unk12 = (pNew->Unk12 * pNew->Unk2E) >> 12;
            if (pEsp->Unk18 != 0) {
                pNew->Unk4 &= ~0x10;
                if (pEsp->Unk18 != 0) {
                    pNew->Unk24 &= ~0x10;
                }
            }
        }
    } else {
        pEsp->Unk24 = 0;
    }
    pEsp->UnkB4++;
}

void func_80022500(ESP3_WORK *pEsp)
{
    SVECTOR v;
    UCHAR Flg;

    Flg = 0;
    switch (pEsp->Routine_2) {
    case 0:
        func_8001CDA4(pEsp, pEsp->Unk18);
        func_8001CE24(pEsp, pEsp->Unk1A, pEsp->Unk1B);
        pEsp->Routine_2++;
        break;
    case 1:
        pEsp->Unk2E += ((SHORT)pEsp->Unk1C * (SHORT)pEsp->Unk2E) >> 12;
        pEsp->Unk1C -= pEsp->Unk1E;
        switch (pEsp->Unk22) {
        case 0:
            if (pEsp->Pos.vy >= 0) {
                Flg = 1;
            }
            break;
        case 3:
            if (pEsp->Pos.vy >= -0x5A0) {
                Flg = 1;
            }
            break;
        }
        if (Flg) {
            if (pEsp->Unk22 == 0) {
                v.vx = 0x400;
                v.vy = 0;
                v.vz = 0;
                func_8001B35C(pEsp, 0x10E, ((pEsp->Unk2E - (pEsp->Unk2E >> 2)) << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, &v);
            }
            pEsp->Unk24 = 0;
            pEsp->Be_flg = 0;
        }
        break;
    }
}

void func_80022660(void) {
}

void func_80022668(ESP3_WORK *pEsp)
{
    SHORT Tbl[6][3] = { { 0, 2, 4 }, { 0xFF, 0xFF, 0xFF }, { 1, 3, 5 }, { 0xFF, 0xFF, 0xFF }, { 0, 2, 4 }, { 0xFF, 0xFF, 0xFF } };
    ESP3_WORK *pNew;
    UCHAR i;
    UCHAR j;
    ULONG n;

    n = (UCHAR)pEsp->UnkB4;
    if (Tbl[n][0] != 0xFF) {
        for (i = 0; i < 3; i++) {
            pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pEsp->Unk19 << 8) | pEsp->Unk18, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + Tbl[n][i] * 0x2AA + D_80098728[func_800102E8()]), pEsp->pMat, &pEsp->Unk38, 0);
            if (pEsp->Unk1A != 0 && pNew != (ESP3_WORK *)-1) {
                for (j = 0; j < 6; j++) {
                    func_8001BC80(pNew);
                }
            }
        }
    }
    if (++pEsp->UnkB4 == 5) {
        pEsp->Unk24 = 0;
    }
}

void func_8002289C(ESP3_WORK *pEsp)
{
    VECTOR v;
    LONG H;
    UCHAR Flg;

    Flg = 0;
    H = Water_ck(pEsp->Pos.vx, pEsp->Pos.vz);
    if (H >= pEsp->Pos.vy) {
        Flg = (H != 0);
    }
    if (G.pRoom->pSca->padC >= pEsp->Pos.vy) {
        H = G.pRoom->pSca->padC;
        Flg = 1;
    }
    if (Flg) {
        v.vx = pEsp->Pos.vx;
        v.vy = H;
        v.vz = pEsp->Pos.vz;
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pEsp->Unk19 << 8) | pEsp->Unk18, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &v, 0);
        pEsp->Unk24 = 0;
    }
}
