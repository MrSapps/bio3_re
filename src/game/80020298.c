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
    ULONG Unk714;                      /* 0x714 random seed (func_800248E4) */
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
    SHORT Rot[3];                      /* 0x68 rotation (func_80022990) */
    UCHAR pad6E;
    UCHAR Anm_cnt;                     /* 0x6F index into pAnmseq */
    MATRIX M70;                        /* 0x70 local matrix (func_80022990) */
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

/* model work (pW[1] = MD_OBJ, then the double-buffered prims built by func_8002AC98 / func_8002ADB8) */
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
void func_80029B94(void);
void Vram_clr(USHORT x, USHORT y, USHORT w, USHORT h);
extern LONG D_800D454C;                /* right after Main.Fade[] */
extern UCHAR D_800DBB58;
extern UCHAR D_800996E4[];             /* message text banks (func_8002FD30) */
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
void func_80026728(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, SHORT Rate);
void func_8002720C(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, SHORT Rate);
void func_80027338(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG No, ULONG Late_flg);
void func_80026CF4(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late_flg);
void func_8002B2BC(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late);
void func_80027F8C(ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG No, ULONG Late_flg);
void func_80027A54(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late_flg);
ULONG func_800102E8(void);   /* Rnd */
ULONG func_8002BA3C(ULONG *pTmd, ULONG Packet, PARTS_W *pP);
ULONG func_8002C068(ULONG *pTmd, ULONG Packet, PARTS_W *pP);
void func_80025FB0(ULONG *pObj, ULONG Arg1, ULONG *pObj2);
void func_80025C04(ULONG *pObj, ULONG Flg, PARTS_W *pP);
void func_80025DE8(ULONG *pObj, ULONG Flg, PARTS_W *pP);
void Set_light_pos(VECTOR *pPos);
extern MATRIX *D_800C79F0;            /* current work matrices (func_80025A28) */
extern VECTOR *D_800C79F4;
LONG func_80051E48(LONG Pos_x, LONG Pos_z);
extern LONG D_800CA720;
extern UCHAR D_80098728[];
extern ULONG D_800986C4[];             /* scale curve (func_80021D58) */
void func_8001CDA4(ESP3_WORK *pEsp, ULONG n);
void func_8001CE24(ESP3_WORK *pEsp, ULONG a1, ULONG a2);             /* random byte table, indexed by Rnd() */
ESP3_WORK *func_8001B35C(ESP3_WORK *pEsp, ULONG Attr, ULONG Ofs, MATRIX *pMat, VECTOR *pPos, SVECTOR *pDir);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020298);

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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020C0C);

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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020EB8);

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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002198C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80021D58);

void func_80021EA4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80021EAC);

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
    H = func_80051E48(pEsp->Pos.vx, pEsp->Pos.vz);
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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022990);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022CCC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022FA8);

void Trans_work_init(void)
{
    POLY_FT4 *pP2;
    ULONG D2;

    pP2 = (POLY_FT4 *)Trans.Prim_poly_ft4;
    D2 = 0x300;
    do {
        pP2->tag = 0x9000000;
        *(ULONG *)&pP2->r0 = 0x2C808080;
        pP2++;
        D2--;
    } while (D2 != 0);
    Trans_pointer_set();
}

void Trans_pointer_set(void)
{
    Trans.pPoly_ft4_keep = Trans.Prim_poly_ft4[G.Cbuf_id];
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80023268);

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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800245A0);

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
void func_80049898(void);
void func_80077C4C(void);
void func_800493EC(void);
void func_80077F60(void);
extern UCHAR D_800E0186;

void func_800248E4(void)
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
        func_80049898();
    }
    func_80077C4C();
    while (pG->Status_flg & 0x800) {
        Task_sleep(1);
    }
    func_800493EC();
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
        D_800E0186 = 0;
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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80024CE8);

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
    func_800746C0(0x22E, NULL, 0, 0);
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
    func_800746C0(0x22E, NULL, 0, 0);
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

void func_800251FC(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg)
{
    VECTOR Dummy;
    SVECTOR Sv;
    MATRIX M;
    VECTOR v;
    ULONG i;

    if (Be_flg & 0x10) {
        return;
    }
    RotMatrix_gte((SVECTOR *)&pEm->Cdir_x, EM_M(pEm));
    if (pEm->Be_flg & 0x100000) {
        v.vx = 0x1000;
        v.vy = pEm->Size;
        v.vz = 0x1000;
        ScaleMatrix(EM_M(pEm), &v);
    }
    if (pEm->Be_flg & 0x80000) {
        if (!(G.Status_flg & 0x200) && !(G.Unk246C & 0x40000000)) {
            func_80026648(pEm);
        }
    } else if (pEm->Be_flg & 0x40000) {
        if (!(G.Status_flg & 0x200) && !(G.Unk246C & 0x40000000)) {
            func_80026498(pEm);
        }
    } else {
        goto parts;
    }
    Sv.vy = 0;
    Sv.vx = pEm->Unk1BC[0];
    Sv.vz = pEm->Unk1BC[1];
    RotMatrix_gte(&Sv, &M);
    MulMatrix2(&M, EM_M(pEm));
parts:
    i = pEm->Parts_num;
    do {
        if (pP->Be_flg & 1) {
            func_800253F0(pEm, pP, pP->Be_flg, &pP->Workm);
        }
        pP++;
    } while (--i != 0);
    if (pEm->Be_flg & 0x200000) {
        i = pEm->Parts_num;
        pP = pEm->pSin_parts_ptr;
loop:
        if ((pP->Be_flg & 4) && ((pP->UnkA2 & 0x10) || pP->UnkA2 == 0)) {
            v.vx = pP->Size_x;
            v.vy = pP->Size_y;
            v.vz = pP->Size_z;
            ScaleMatrix(&pP->Workm, &v);
        }
        pP++;
        if (--i != 0) {
            goto loop;
        }
    }
}

void func_800253F0(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG Be_flg, MATRIX *wm)
{
    MATRIX *pSuper;
    MATRIX *pM;
    MATRIX lm;
    VECTOR sv;
    USHORT Flg;

    Flg = pP_ptr->UnkA2;
    pSuper = pP_ptr->pSuper;
    pM = (MATRIX *)pP_ptr->M;
    if ((pP_ptr->pOya_parts->Be_flg & 9) == 8) {
        pP_ptr->Be_flg = (pP_ptr->Be_flg & ~1) | 8;
        return;
    }
    if (!(Be_flg & 2)) {
        func_8002D4B0(pSuper, pM, wm);
    }
    if (Be_flg & 8) {
        func_80027568(pP_ptr, wm);
        return;
    }
    if (Flg & 0x20) {
        func_8002783C(pP_ptr);
    }
}

void func_800254AC(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg, VECTOR *pPos)
{
    ULONG i;

    if (Be_flg & 0x10) {
        return;
    }
    if (!(Be_flg & 0x40) && func_8001020C((LONG *)pPos, G.Cccut) == 0) {
        i = pEm->Parts_num;
loop0:
        if ((pP->Be_flg & 0x21) == 0x21 && (pP->UnkA2 & 1)) {
            func_80025610(pEm, pP, pP->UnkA2, &pP->Workm);
        }
        pP++;
        if (--i != 0) {
            goto loop0;
        }
        return;
    }
    if (Be_flg & 8) {
        D_80098B08.Unk0[0] = pEm->Unk178[0];
        D_80098B08.Unk0[1] = pEm->Unk178[1];
        D_80098B08.Unk0[2] = pEm->Unk178[2];
        D_80098B08.Unk6 = (pEm->Unk17E & 0xF00) >> 8;
        i = (UCHAR)pEm->Unk17E;
        D_80098B08.Rgb = i | (i << 8) | (i << 16);
    }
    i = pEm->Parts_num;
loop1:
    if ((pP->Be_flg & 1) && (pP->UnkA2 & 1)) {
        func_80025610(pEm, pP, pP->UnkA2, &pP->Workm);
    }
    pP++;
    if (--i != 0) {
        goto loop1;
    }
}

void func_80025610_(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM) __asm__("func_80025610");

void func_80025610_(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM)
{
    MATRIX M;
    VECTOR Scale;
    tagGLOBAL *pG;
    ULONG Be;

    pG = &G;
    Be = pP->Be_flg;
    if (Be & 0x20) {
        if (func_8001020C(pM->t, pG->Cccut) == 0) {
            return;
        }
    }
    if (!(Flg & 0x210)) {
        if (!(Flg & 0x1000)) {
            gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
            gte_SetLightMatrix(&M);
        } else {
            Set_light_pos((VECTOR *)pM->t);
            gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
            gte_SetLightMatrix(&M);
            SetColorMatrix(&COLORMATRIX);
            Set_light_pos((VECTOR *)&pEm->Pos_x);
        }
    }
    if (!(Flg & 0x10)) {
        func_8002D4B0(&GsWSMATRIX, pM, &M);
        if (Be & 4) {
            Scale.vx = pP->Size_x;
            Scale.vy = pP->Size_y;
            Scale.vz = pP->Size_z;
            ScaleMatrix(&M, &Scale);
        }
        gte_SetRotMatrix(&M);
        gte_SetTransMatrix(&M);
    }
    if (Flg & 0x20) {
        pG->Unk208C = func_8002BA3C(pP->pTmd, pG->Unk208C, pP);
        pG->Unk208C = func_8002C068(pP->pTmd, pG->Unk208C, pP);
        func_8002783C(pP);
    } else if (Flg & 0x10) {
        D_800C79F0 = &M;
        D_800C79F4 = &Scale;
        func_80025DE8(&pP->Attribute, Flg, pP);
    } else if (Flg & 2) {
        func_80025C04(&pP->Attribute, Flg, pP);
    }
}


void func_80025A28(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM)
{
    MATRIX M;
    VECTOR M2;

    D_800C79F0 = &M;
    D_800C79F4 = &M2;
    gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
    gte_SetLightMatrix(&M);
    func_8002D4B0(&GsWSMATRIX, pM, &M);
    gte_SetRotMatrix(&M);
    gte_SetTransMatrix(&M);
    if (Flg & 1) {
        if (Flg & 2) {
            func_80025FB0(&pP->Attribute, (SHORT)pP->UnkA2, (ULONG *)pP);
        }
    }
}

void func_80025C04(ULONG *pObj, ULONG Flg, PARTS_W *pP)
{
    UCHAR Work0[0x1C0];
    UCHAR Work1[0xE0];
    SORT_PARAM Prm;
    tagGLOBAL *pG;
    ULONG Abr;
    ULONG Top;

    pG = &G;
    Prm.Unk0 = pObj[1];
    Prm.Unk8 = pG->pEm->Unk7C;
    Abr = (Flg & 0x100) >> 7;
    if (Flg & 0x2000) {
        Prm.Unk8 = func_80037D50((VECTOR *)pP->Workm.t);
    }
    if (Flg & 0x400) {
        func_8002D184(Work0, Work1, pObj[1], &pP->Workm);
    } else {
        func_8002D5D8(Work0, Work1, pObj[1]);
    }
    if (Flg & 0x40) {
        Prm.u.Unk4 = D_80098B08.Unk6 << 24;
        pG->Unk208C = func_8002C824(&Prm, pG->Unk208C, Work0, Work1);
        pG->Unk208C = func_8002CABC(&Prm, pG->Unk208C, Work0, Work1);
    }
    if (Flg & 0x80) {
        Prm.u.Unk4 = *(ULONG *)((UCHAR *)&D_80098B08 + 8) | (*(SHORT *)((UCHAR *)&D_80098B08 + 6) << 24);
        pG->Unk208C = func_8002CE08(&Prm, pG->Unk208C, Work0, Work1);
        pG->Unk208C = func_8002CF90(&Prm, pG->Unk208C, Work0, Work1);
    }
    Prm.u.Unk4 = pObj[25];
    Top = pG->Unk208C;
    Prm.u.c.Code = Abr | 0x34;
    pG->Unk208C = func_8002D6D8(&Prm, pG->Unk208C, Work0, Work1);
    Prm.u.c.Code = Abr | 0x3C;
    pG->Unk208C = func_8002D898(&Prm, pG->Unk208C, Work0, Work1);
    if (Flg & 4) {
        func_8002D41C((RGB_BLEND_WORK *)Top, (RGB_BLEND_WORK *)pG->Unk208C, PW_FREEL(pP)[0], PW_FREEL(pP)[1]);
    }
}

void func_80025DE8(ULONG *pObj, ULONG Flg, PARTS_W *pP)
{
    UCHAR Work0[0x1C0];
    UCHAR Work1[0xE0];
    SORT_PARAM Prm;
    tagGLOBAL *pG;
    ULONG Top;

    pG = &G;
    Prm.Unk0 = pObj[1];
    Prm.Unk8 = pG->pEm->Unk7C;
    if (Flg & 0x400) {
        func_8007A490(Work0, Work1, pObj[1], pP);
    } else {
        func_8007A350(Work0, Work1, pObj[1], pP);
    }
    Prm.UnkC = (ULONG)&D_800E1388;
    if (Flg & 0x40) {
        Prm.u.Unk4 = D_80098B08.Unk6 << 24;
        pG->Unk208C = func_8002C824(&Prm, pG->Unk208C, Work0, Work1);
        pG->Unk208C = func_8002CABC(&Prm, pG->Unk208C, Work0, Work1);
    }
    if (Flg & 0x80) {
        Prm.u.Unk4 = *(ULONG *)((UCHAR *)&D_80098B08 + 8) | (*(SHORT *)((UCHAR *)&D_80098B08 + 6) << 24);
        pG->Unk208C = func_8002CE08(&Prm, pG->Unk208C, Work0, Work1);
        pG->Unk208C = func_8002CF90(&Prm, pG->Unk208C, Work0, Work1);
    }
    Top = pG->Unk208C;
    Prm.u.Unk4 = pObj[25];
    pG->Unk208C = func_8002DFE0(&Prm, Top, Work0, Work1);
    pG->Unk208C = func_8002E160(&Prm, pG->Unk208C, Work0, Work1);
    if (Flg & 4) {
        func_8002D41C((RGB_BLEND_WORK *)Top, (RGB_BLEND_WORK *)pG->Unk208C, PW_FREEL(pP)[0], PW_FREEL(pP)[1]);
    }
}

void func_80025FB0(ULONG *pObj, ULONG Arg1, ULONG *pObj2)
{
    UCHAR Work0[0x1C8];
    UCHAR Work1[0xE8];
    SORT_PARAM Prm;
    ULONG D0;

    Prm.Unk0 = D0 = pObj[1];
    Prm.u.Unk4 = pObj[25];
    Prm.Unk8 = G.pEm->Unk7C;
    func_8002D5D8(Work0, Work1, D0);
    Prm.u.c.Code = 0x34;
    Prm.UnkC = pObj2[46];
    G.Unk208C = func_8002DC98(&Prm, G.Unk208C, Work0, Work1);
    Prm.u.c.Code = 0x3C;
    G.Unk208C = func_8002DE0C(&Prm, G.Unk208C, Work0, Work1);
}

void func_8002606C(ULONG *pObj, ULONG Flg)
{
    UCHAR Work0[0x1C0];
    UCHAR Work1[0xE0];
    SORT_PARAM Prm;
    ULONG Abr;

    Prm.Unk0 = pObj[1];
    Prm.Unk8 = G.pEm->Unk7C;
    Prm.u.Unk4 = pObj[25];
    Abr = (Flg & 0x100) >> 7;
    func_8002D5D8(Work0, Work1, pObj[1]);
    if (Flg & 8) {
        Prm.u.c.Code = Abr | 0x34;
        G.Unk208C = func_8002D6D8(&Prm, G.Unk208C, Work0, Work1);
        Prm.u.c.Code = Abr | 0x3C;
        G.Unk208C = func_8002D898(&Prm, G.Unk208C, Work0, Work1);
    } else {
        Prm.u.c.Code = Abr | 0x34;
        G.Unk208C = func_8002B6FC(&Prm, G.Unk208C, Work0, Work1);
        Prm.u.c.Code = Abr | 0x3C;
        G.Unk208C = func_8002B86C(&Prm, G.Unk208C, Work0, Work1);
    }
}

ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top)
{
    pEm->Parts_num = ((UCHAR *)pEm->pTmd)[-4];
    pEm->pSin_parts_ptr = (PARTS_W *)work_top;
    return work_top + pEm->Parts_num * sizeof(PARTS_W);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800261AC);

void Parts_work_pos_init(ENEMY_WORK *pEm, SHORT *pKan)
{
    SHORT *pPos;
    ULONG n;
    PARTS_W *pCur;

    pPos = &pKan[4];
    n = pEm->Parts_num;
    pCur = pEm->pSin_parts_ptr;
    while (n-- != 0) {
        pCur->Old_x = pCur->Pos_x = pPos[0];
        pCur->Old_y = pCur->Pos_y = pPos[1];
        pCur->Old_z = pCur->Pos_z = pPos[2];
        pPos += 3;
        pCur++;
    }
}

void func_8002646C(ENEMY_WORK *pEm, SHORT A, SHORT B, UCHAR C)
{
    pEm->Unk1BA = A;
    pEm->Unk1C1 = C;
    pEm->Unk1C2 = B;
    pEm->Be_flg = (pEm->Be_flg | 0x40000) & ~0x80000;
}

void func_80026498(ENEMY_WORK *pEm)
{
    UCHAR Flg;
    LONG d;
    SHORT a;

    Flg = pEm->Unk1C1;
    if (Flg & 8) {
        pEm->Be_flg = (pEm->Be_flg & ~0x40000) | 0x80000;
        return;
    }
    if (Flg & 2) {
        if (Flg & 4) {
            if (Flg & 1) {
                pEm->Unk1BC[1] += pEm->Unk1C2;
            } else {
                pEm->Unk1BC[1] -= pEm->Unk1C2;
            }
        } else {
            pEm->Unk1BC[1] = 0;
        }
        if (pEm->Unk1C1 & 1) {
            pEm->Unk1BC[0] -= pEm->Unk1C2;
        } else {
            pEm->Unk1BC[0] += pEm->Unk1C2;
        }
    } else {
        if (Flg & 4) {
            if (Flg & 1) {
                pEm->Unk1BC[0] += pEm->Unk1C2;
            } else {
                pEm->Unk1BC[0] -= pEm->Unk1C2;
            }
        } else {
            pEm->Unk1BC[0] = 0;
        }
        if (pEm->Unk1C1 & 1) {
            pEm->Unk1BC[1] += pEm->Unk1C2;
        } else {
            pEm->Unk1BC[1] -= pEm->Unk1C2;
        }
    }
    d = pEm->Unk1BC[0];
    if (pEm->Unk1BA < __builtin_abs(d)) {
        a = pEm->Unk1BA;
        if (d <= 0) {
            a = -a;
        }
        pEm->Unk1BC[0] = a;
    }
    d = pEm->Unk1BC[1];
    if (pEm->Unk1BA < __builtin_abs(d)) {
        a = pEm->Unk1BA;
        if (d <= 0) {
            a = -a;
        }
        pEm->Unk1BC[1] = a;
    }
}

void func_80026648(ENEMY_WORK *pEm)
{
    pEm->Be_flg &= ~0x40000;
    if (*(ULONG *)pEm->Unk1BC != 0) {
        if (pEm->Unk1BC[0] > 0) {
            if (pEm->Unk1C2 < pEm->Unk1BC[0]) {
                pEm->Unk1BC[0] -= pEm->Unk1C2;
            } else {
                pEm->Unk1BC[0] = 0;
            }
        } else {
            if (pEm->Unk1C2 < -pEm->Unk1BC[0]) {
                pEm->Unk1BC[0] += pEm->Unk1C2;
            } else {
                pEm->Unk1BC[0] = 0;
            }
        }
        if (pEm->Unk1BC[1] > 0) {
            if (pEm->Unk1C2 < pEm->Unk1BC[1]) {
                pEm->Unk1BC[1] -= pEm->Unk1C2;
            } else {
                pEm->Unk1BC[1] = 0;
            }
        } else {
            if (pEm->Unk1C2 < -pEm->Unk1BC[1]) {
                pEm->Unk1BC[1] += pEm->Unk1C2;
            } else {
                pEm->Unk1BC[1] = 0;
            }
        }
    } else {
        pEm->Be_flg &= ~0x80000;
    }
}

void MatrixNormal_0(MATRIX *m, MATRIX *n);
void func_80026728_(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate) __asm__("func_80026728");

void func_80026728_(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate)
{
    SVECTOR Sv;
    LONG Rate0;

    Rate0 = 0x1000 - Rate;
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[0][0]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[0][0]);
    gte_gpl12();
    gte_stsv(&pM2->m[0][0]);
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[1][1]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[1][1]);
    gte_gpl12();
    gte_stsv(&pM2->m[1][1]);
    Sv.vx = pM0->m[1][0];
    Sv.vy = pM0->m[2][1];
    Sv.vz = pM0->m[2][2];
    gte_lddp(Rate0);
    gte_ldsv(&Sv);
    gte_gpf12();
    Sv.vx = pM1->m[1][0];
    Sv.vy = pM1->m[2][1];
    Sv.vz = pM1->m[2][2];
    gte_lddp(Rate);
    gte_ldsv(&Sv);
    gte_gpl12();
    gte_stsv(&Sv);
    pM2->m[1][0] = Sv.vx;
    pM2->m[2][1] = Sv.vy;
    pM2->m[2][2] = Sv.vz;
    MatrixNormal_0(pM2, pM2);
}

void MatrixNormal_2(MATRIX *m, MATRIX *n);

void func_80026900(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate)
{
    SVECTOR Sv;
    LONG Rate0;

    Rate0 = 0x1000 - Rate;
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[0][0]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[0][0]);
    gte_gpl12();
    gte_stsv(&pM2->m[0][0]);
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[1][1]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[1][1]);
    gte_gpl12();
    gte_stsv(&pM2->m[1][1]);
    Sv.vx = pM0->m[1][0];
    Sv.vy = pM0->m[2][1];
    Sv.vz = pM0->m[2][2];
    gte_lddp(Rate0);
    gte_ldsv(&Sv);
    gte_gpf12();
    Sv.vx = pM1->m[1][0];
    Sv.vy = pM1->m[2][1];
    Sv.vz = pM1->m[2][2];
    gte_lddp(Rate);
    gte_ldsv(&Sv);
    gte_gpl12();
    gte_stsv(&Sv);
    pM2->m[1][0] = Sv.vx;
    pM2->m[2][1] = Sv.vy;
    pM2->m[2][2] = Sv.vz;
    MatrixNormal_2(pM2, pM2);
    Sv.vx = pM0->t[0];
    Sv.vy = pM0->t[1];
    Sv.vz = pM0->t[2];
    gte_lddp(Rate0);
    gte_ldsv(&Sv);
    gte_gpf12();
    Sv.vx = pM1->t[0];
    Sv.vy = pM1->t[1];
    Sv.vz = pM1->t[2];
    gte_lddp(Rate);
    gte_ldsv(&Sv);
    gte_gpl12();
    gte_stsv(&Sv);
    pM2->t[0] = Sv.vx;
    pM2->t[1] = Sv.vy;
    pM2->t[2] = Sv.vz;
}

void func_80026BE8(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
{
    USHORT *p;
    ULONG f_num;

    p = (USHORT *)(Seq_t_ptr + pEm->Move_no * 8);
    f_num = *p++;
    if (Late_flg & 0xFFFF0000) {
        p = (USHORT *)(Seq_t_ptr + *p) + (f_num - pEm->Move_cnt - 1);
    } else {
        p = (USHORT *)(Seq_t_ptr + *p) + pEm->Move_cnt;
    }
    if (*p & 0x100) {
        if (!(*pEm->pNow_seq & 0x100)) {
            func_8002B2BC(Kan_t_ptr, Seq_t_ptr, pEm->pNow_seq, Late_flg >> 16);
        }
        pEm->pNow_seq = p;
        func_80027338(Kan_t_ptr, Seq_t_ptr, *(UCHAR *)p, Late_flg);
    } else {
        pEm->pNow_seq = p;
        func_80026CF4(Kan_t_ptr, Seq_t_ptr, p, Late_flg);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80026CF4);

void func_8002720C_(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, LONG Rate) __asm__("func_8002720C");

void func_8002720C_(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, LONG Rate)
{
    LONG dx;
    LONG dy;
    LONG dz;

    *(ULONG *)&pCur->vx &= 0xFFF0FFF;
    pCur->vz &= 0xFFF;
    gte_lddp(0x1000 - Rate);
    gte_ldsv(pCur);
    dx = pDst->vx - pCur->vx + 0x800;
    dy = pDst->vy - pCur->vy + 0x800;
    dz = pDst->vz - pCur->vz + 0x800;
    if ((ULONG)dx > 0x1000) {
        pDst->vx += (dx < 0) ? 0x1000 : -0x1000;
    }
    if ((ULONG)dy > 0x1000) {
        pDst->vy += (dy >= 0) ? -0x1000 : 0x1000;
    }
    if ((ULONG)dz > 0x1000) {
        pDst->vz += (dz >= 0) ? -0x1000 : 0x1000;
    }
    gte_gpf12_b();
    gte_lddp(Rate);
    gte_ldsv(pDst);
    gte_gpl12();
    gte_stsv(pOut);
}

void func_8002B190_(ULONG n, PARTS_W *pP, LONG Rate, ULONG Late) __asm__("func_8002B190");
ULONG func_80027338_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg) __asm__("func_80027338");

ULONG func_80027338_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg)
{
    ENEMY_WORK *pEm;
    PARTS_W *pP;
    PARTS_W *pP_walk;
    ULONG Max;
    ULONG n;
    LONG w;
    VECTOR V;

    pEm = G.pEm;
    Max = *(USHORT *)(Seq_t_ptr + pEm->Move_no * 8);
    if (Late_flg & 0xFFFF0000) {
        No = ~No;
        No &= 0xFF;
    }
    pP = pEm->pSin_parts_ptr;
    pP->Pos_y = pP->UnkAE + ((pP->UnkB6 * No) >> 8);
    n = *(USHORT *)(Kan_t_ptr + 4);
    pP_walk = pP;
    if (pEm->UnkCA == 0) {
        do {
            pP_walk->Cdir_x = pP_walk->Unk88 + ((pP_walk->Unk90 * No) >> 8);
            pP_walk->Cdir_y = pP_walk->Unk8A + ((pP_walk->Unk92 * No) >> 8);
            pP_walk->Cdir_z = pP_walk->Unk8C + ((pP_walk->Unk8E * No) >> 8);
            if (!(pP_walk->Be_flg & 0x10)) {
                RotMatrix_gte((SVECTOR *)&pP_walk->Cdir_x, (MATRIX *)pP_walk->M);
            }
            pP_walk++;
        } while (--n);
    } else {
        Late_flg &= 0xFFFF;
        w = Late_flg * pEm->UnkCA;
        V.vx = pP->Old_x;
        V.vy = pP->Old_y;
        V.vz = pP->Old_z;
        gte_lddp(0x1000 - w);
        gte_ldlvl(&pP->Pos_x);
        gte_gpf12();
        gte_lddp(w);
        gte_ldlvl(&V);
        gte_gpl12();
        gte_stlvl(&pP->Pos_x);
        pEm->UnkCA--;
        func_8002B190_(n, pP, No, Late_flg);
    }
    pP->Old_x = pP->Pos_x;
    pP->Old_y = pP->Pos_y;
    pP->Old_z = pP->Pos_z;
    if (++pEm->Move_cnt < Max) {
        return 0;
    }
    pEm->Move_cnt = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80027568);

void func_8002783C(PARTS_W *pP_ptr)
{
    MATRIX M;
    SVECTOR Sv;
    SVECTOR Sv1;
    SHORT new_cnt;
    USHORT r;

    if ((PW_FREE(pP_ptr)[4] & 0x7FFF) >= 0x1D) {
        pP_ptr->Be_flg = 0;
        func_8001B894(0xFF05, &pP_ptr->Workm);
    } else {
        PW_FREE(pP_ptr)[4]++;
    }
    new_cnt = PW_FREE(pP_ptr)[1] + PW_FREE(pP_ptr)[3];
    PW_FREE(pP_ptr)[1] = new_cnt;
    pP_ptr->Workm.t[1] += new_cnt;
    Sv.vy = PW_FREE(pP_ptr)[0];
    Sv.vz = 0;
    Sv.vx = 0;
    RotMatrix(&Sv, &M);
    r = PW_FREE(pP_ptr)[2] + PW_FREE(pP_ptr)[6];
    PW_FREE(pP_ptr)[2] = r;
    if ((LONG)(r << 16) < 0) {
        PW_FREE(pP_ptr)[2] = 0;
    }
    Sv.vx = PW_FREE(pP_ptr)[2];
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, &Sv1);
    pP_ptr->Workm.t[0] += Sv1.vx;
    pP_ptr->Workm.t[2] += Sv1.vz;
}

void func_80027940(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
{
    USHORT *p;
    ULONG f_num;

    p = (USHORT *)(Seq_t_ptr + pEm->Move_no * 8);
    f_num = *p++;
    if (Late_flg & 0xFFFF0000) {
        p = (USHORT *)(Seq_t_ptr + *p) + (f_num - pEm->Move_cnt - 1);
    } else {
        p = (USHORT *)(Seq_t_ptr + *p) + pEm->Move_cnt;
    }
    if (*p & 0x100) {
        if (!(*pEm->pNow_seq & 0x100)) {
            func_8002B2BC(Kan_t_ptr, Seq_t_ptr, pEm->pNow_seq, Late_flg >> 16);
        }
        pEm->pNow_seq = p;
        func_80027F8C(Kan_t_ptr, Seq_t_ptr, *(UCHAR *)p, Late_flg);
    } else {
        pEm->pNow_seq = p;
        func_80027A54(Kan_t_ptr, Seq_t_ptr, p, (Late_flg & 0xFFFF) | (f_num << 16));
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80027A54);

void func_800281B4(ULONG n, PARTS_W *pP, ULONG Late, SHORT Rate);
ULONG func_80027F8C_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg) __asm__("func_80027F8C");

ULONG func_80027F8C_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg)
{
    ENEMY_WORK *pEm;
    PARTS_W *pP;
    PARTS_W *pP_walk;
    ULONG Max;
    ULONG n;
    LONG w;
    VECTOR V;

    pEm = G.pEm;
    Max = *(USHORT *)(Seq_t_ptr + pEm->Move_no * 8);
    if (Late_flg & 0xFFFF0000) {
        No = ~No;
        No &= 0xFF;
    }
    pP = pEm->pSin_parts_ptr;
    pP->Pos_y = pP->UnkAE + ((pP->UnkB6 * No) >> 8);
    n = 15;
    pP_walk = pP;
    if (pEm->UnkCA == 0) {
        do {
            pP_walk->Cdir_x = pP_walk->Unk88 + ((pP_walk->Unk90 * No) >> 8);
            pP_walk->Cdir_y = pP_walk->Unk8A + ((pP_walk->Unk92 * No) >> 8);
            pP_walk->Cdir_z = pP_walk->Unk8C + ((pP_walk->Unk8E * No) >> 8);
            RotMatrix_gte((SVECTOR *)&pP_walk->Cdir_x, (MATRIX *)pP_walk->M);
            pP_walk++;
        } while (--n);
    } else {
        Late_flg &= 0xFFFF;
        w = Late_flg * pEm->UnkCA;
        V.vx = pP->Old_x;
        V.vy = pP->Old_y;
        V.vz = pP->Old_z;
        gte_lddp(0x1000 - w);
        gte_ldlvl(&pP->Pos_x);
        gte_gpf12();
        gte_lddp(w);
        gte_ldlvl(&V);
        gte_gpl12();
        gte_stlvl(&pP->Pos_x);
        pEm->UnkCA--;
        func_800281B4(15, pP, No, Late_flg);
    }
    pP->Old_x = pP->Pos_x;
    pP->Old_y = pP->Pos_y;
    pP->Old_z = pP->Pos_z;
    if (++pEm->Move_cnt < Max) {
        return 0;
    }
    pEm->Move_cnt = 0;
    return 1;
}

void func_800281B4(ULONG n, PARTS_W *pP, ULONG Late, SHORT Rate)
{
    SVECTOR Sv;
    MATRIX M;
    SVECTOR *pSv;
    SHORT Hokan;

    pSv = &Sv;
    Hokan = 0x1000 - G.pEm->UnkCA * Rate;
    do {
        pSv->vx = pP->Unk88 + ((ULONG)(pP->Unk90 * Late) >> 8);
        pSv->vy = pP->Unk8A + ((ULONG)(pP->Unk92 * Late) >> 8);
        pSv->vz = pP->Unk8C + ((ULONG)(pP->Unk8E * Late) >> 8);
        if (n >= 7) {
            func_8002720C((SVECTOR *)&pP->Cdir_x, pSv, (SVECTOR *)&pP->Cdir_x, Hokan);
            RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        } else {
            RotMatrix_gte(pSv, &M);
            func_80026728((MATRIX *)pP->M, &M, (MATRIX *)pP->M, Hokan);
        }
    } while (--n != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80028308);

void func_8002606C_(ULONG *pObj, ULONG Flg, PARTS_W *pP) __asm__("func_8002606C");

void func_800288C8(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM)
{
    MATRIX M;
    VECTOR Scale;
    tagGLOBAL *pG;
    ULONG Be;

    pG = &G;
    Be = pP->Be_flg;
    if (!(Flg & 0x200)) {
        gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
        gte_SetLightMatrix(&M);
    }
    func_8002D4B0(&GsWSMATRIX, pM, &M);
    if (Be & 4) {
        Scale.vx = pP->Size_x;
        Scale.vy = pP->Size_y;
        Scale.vz = pP->Size_z;
        ScaleMatrix(&M, &Scale);
    }
    gte_SetRotMatrix(&M);
    gte_SetTransMatrix(&M);
    if (Flg & 0x20) {
        pG->Unk208C = func_8002BA3C(pP->pTmd, pG->Unk208C, pP);
        pG->Unk208C = func_8002C068(pP->pTmd, pG->Unk208C, pP);
        func_8002783C(pP);
    } else if (Flg & 2) {
        func_8002606C_(&pP->Attribute, Flg, pP);
    }
}

void func_80028B1C(void)
{
    LINE_SCR *pLine;
    ULONG Amp;

    pLine = &Line;
    if (!(G.Status_flg & 7)) {
        Line.r_no_0 = 0;
        return;
    }
    if (Line.r_no_0 == 0) {
        pLine->Rect.x = 0;
        pLine->Rect.w = 320;
        pLine->Rect.y = 0;
        pLine->Unk2D20[0] = 0;
        pLine->Rect.h = 240;
        pLine->Unk2D20[1] = 0;
        pLine->Unk2D20[2] = 0;
        pLine->Unk2D20[3] = 2;
        pLine->Unk2D20[4] = 0x40;
        pLine->Unk2D20[5] = 2;
        Line.r_no_0++;
    }
    if (G.Status_flg & 4) {
        line_mode_set(0, 7);
        Amp = 1;
    } else if (G.Status_flg & 2) {
        line_mode_set(0, 7);
        Amp = 2;
    } else {
        return;
    }
    func_80028BE8(Amp);
    line_scr_end();
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80028BE8);

void line_trans(LONG Line_no, LONG Slide_ofs, LONG Copy_ofs, LONG Draw_ofs)
{
    LINE_SCR *pLine;
    DR_MOVE *pMove;
    LONG Pos;
    LONG D0;
    RECT Rect;

    pLine = &Line;
    if ((Slide_ofs | Copy_ofs) == 0) {
        return;
    }
    if (Slide_ofs < 0 || Slide_ofs >= pLine->Rect.w) {
        return;
    }
    Rect.w = pLine->Rect.w - Slide_ofs;
    Rect.h = 1;
    Rect.x = pLine->Rect.x + Slide_ofs;
    Pos = Line_no + Draw_ofs;
    pMove = &pLine->Line_move[G.Cbuf_id][Line_no];
    if (Line_no + Copy_ofs < 0xF0) {
        D0 = Pos + Copy_ofs;
        if (Line_no + Copy_ofs < 0) {
            Rect.y = Draw_ofs;
            goto set;
        }
    } else {
        D0 = Draw_ofs + 0xEF;
    }
    Rect.y = D0;
set:
    SetDrawMove(pMove, &Rect, pLine->Rect.x, Pos);
    if (Copy_ofs < 0) {
        addPrim(pLine->pOt, pMove);
    } else {
        *pLine->pPrim_keep0 = (*pLine->pPrim_keep0 & 0xFF000000) | ((ULONG)pMove & 0xFFFFFF);
        pLine->pPrim_keep0 = (ULONG *)pMove;
    }
}

void line_scr_end(void)
{
    *Line.pPrim_keep0 = (*Line.pPrim_keep0 & 0xFF000000) | (Line.Keep_tag0 & 0xFFFFFF);
}

void line_mode_set(ULONG Ot_no, ULONG Prio)
{
    LINE_SCR *pLine;
    ULONG *pOt;
    ULONG *pNull;
    ULONG Id;
    tagGLOBAL *pG;

    pG = &G;
    pLine = &Line;
    Id = pG->Cbuf_id;
    switch (Ot_no) {
    case 0:
        pOt = pG->pOt0 + Prio;
        break;
    case 1:
        pOt = pG->pOt1 + Prio;
        break;
    case 2:
        pOt = pG->pOt2 + Prio;
        break;
    }
    pNull = &pLine->Null_prim[Id];
    addPrim(pOt, pNull);
    pLine->Keep_tag0 = *pNull;
    pLine->pPrim_keep0 = pNull;
    pLine->pOt = pOt;
}

void Line_work_init(void)
{
    UCHAR *p = Line.Unk2D20;

    Line.Null_prim[0] = 0;
    Line.Null_prim[1] = 0;
    p[0] = 2;
    p[1] = 0x40;
    p[2] = 2;
    p[3] = 2;
    p[4] = 0x40;
    p[5] = 2;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80028F38);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029294);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029700);

void func_800297FC(void)
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
    func_80029988();
    Moji_init();
    Line_work_init();
    pG->Unk2114 = 0;
    Task_execute(0, func_80029B94);
    ResetGraph(0);
    Vram_clr(0, 0, 0x400, 0x200);
    D_800D454C = 0;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029988);

extern SHORT D_800CCBBC;
void func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
void func_800784E0(ULONG *p);
void Task_lchain(ULONG Task_no);
int PadGetState(int port);
int PadSetMainMode(int socket, int offs, int lock);

void func_80029B94(void)
{
    tagMAIN *pMain;
    DBUF *pDb;
    LONG i;

    pMain = &Main;
    Card_event_set();
    func_80012818(0x63, (ULONG *)0x8011A000, 1, "TEX_TIM");
    D_800CCBBC = 0x1C;
    func_800784E0((ULONG *)0x8011A000);
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

void func_800486F0(void);
void func_80048FF4(void);
void Set_light_default(void);
void Set_view(LONG *pView_p, LONG *pView_r);
void func_80010000(ULONG *pSrc, ULONG *pDst);
void func_8008F8A4(LONG mode);
void func_8008FAD4(LONG mode);
extern USHORT *D_8009DFD0[];           /* per stage room file numbers */

void func_80029E3C(void)
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
            func_800486F0();
        }
        pBs_size = (USHORT *)(pG->Unk7840 * 2 + (ULONG)pG->pBs_size);
        Size = pG->pUnk2128[*pBs_size + pG->Cut_no];
        CD.Check_sum_flg = 2;
        CD.Fsum = pG->pBs_check_sum[*pBs_size + pG->Cut_no];
        CD.Fsize = Size;
        CD.Fno = D_8009DFD0[pG->Unk783E][pG->Unk7840 + 1] - 1;
        CD.Fsector = (floc[CD.Fno].sec_high << 16) + floc[CD.Fno].sector + pG->Cut_no * 32;
        func_80012818(CD.Fno, (ULONG *)0x80194000, 2, "BS");
        CD.Check_sum_flg = 0;
        if (*(ULONG *)(0x80193FFC + Size) != 0) {
            func_80010000((ULONG *)(*(ULONG *)(0x80193FF8 + Size) + 0x80194000), (ULONG *)0x801AC000);
            pG->Unk2484 = 0x15;
            func_800784E0((ULONG *)0x801AC000);
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
    func_800522D8();
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
    func_80029988();
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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002A49C);

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

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002AB34);

void Md_adr_cnv(MD_HEAD *pMd)
{
    ULONG n = pMd->n_obj;
    MD_OBJ *pObj = pMd->Obj;
    ULONG base = (ULONG)pObj;

    do {
        n--;
        pObj->pVert = (ULONG *)((ULONG)pObj->pVert + base);
        pObj->pNorm = (ULONG *)((ULONG)pObj->pNorm + base);
        pObj->pPrim3 = (ULONG *)((ULONG)pObj->pPrim3 + base);
        pObj->pPrim4 = (ULONG *)((ULONG)pObj->pPrim4 + base);
        pObj++;
    } while (n != 0);
}

POLY_GT3 *func_8002AC98(MD_WORK *pW, POLY_GT3 *pPrim)
{
    MD_OBJ *pObj;
    ULONG n;
    ULONG *p;
    ULONG Abr;
    POLY_GT3 *pP;

    pObj = pW->pObj;
    pP = pPrim;
    pW->pPrim3 = pPrim;
    n = pObj->n_prim3;
    p = pObj->pPrim3;
    pPrim += n * 2;
    Abr = (pW->Attr & 0xC) << 19;
    while (n-- != 0) {
        setPolyGT3(pP);
        *(ULONG *)&pP->r0 = 0x34808080;
        *(ULONG *)&pP->u0 = p[0];
        *(ULONG *)&pP->r1 = 0x808080;
        *(ULONG *)&pP->u1 = (p[1] | Abr) & 0xFFFFFF;
        *(ULONG *)&pP->r2 = 0x808080;
        *(ULONG *)&pP->u2 = *(USHORT *)&p[2];
        pP[1] = pP[0];
        pP += 2;
        p += 3;
    }
    return pPrim;
}

POLY_GT4 *func_8002ADB8(MD_WORK *pW, POLY_GT4 *pPrim)
{
    MD_OBJ *pObj;
    ULONG n;
    ULONG *p;
    ULONG Abr;
    POLY_GT4 *pP;

    pObj = pW->pObj;
    pP = pPrim;
    pW->pPrim4 = pPrim;
    n = pObj->n_prim4;
    p = pObj->pPrim4;
    pPrim += n * 2;
    Abr = (pW->Attr & 0xC) << 19;
    while (n-- != 0) {
        setPolyGT4(pP);
        *(ULONG *)&pP->r0 = 0x34808080;
        *(ULONG *)&pP->u0 = p[0];
        *(ULONG *)&pP->r1 = 0x808080;
        *(ULONG *)&pP->u1 = (p[1] | Abr) & 0xFFFFFF;
        *(ULONG *)&pP->r2 = 0x808080;
        *(ULONG *)&pP->u2 = *(USHORT *)&p[2];
        *(ULONG *)&pP->r3 = 0x808080;
        *(ULONG *)&pP->u3 = *(USHORT *)&p[3];
        pP[1] = pP[0];
        pP += 2;
        p += 4;
    }
    return pPrim;
}

void func_8002AEE8(MATRIX *lwm, MATRIX *klwm, MATRIX *nlm)
{
    MATRIX tmp;
    VECTOR vec;

    TransposeMatrix(lwm, &tmp);
    MulMatrix0(&tmp, klwm, nlm);
    vec.vx = klwm->t[0] - lwm->t[0];
    vec.vy = klwm->t[1] - lwm->t[1];
    vec.vz = klwm->t[2] - lwm->t[2];
    ApplyRotMatrixLV(&vec, &vec);
    nlm->t[0] = vec.vx;
    nlm->t[1] = vec.vy;
    nlm->t[2] = vec.vz;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002AF9C);

void func_8002B190(ULONG n, PARTS_W *pP, ULONG Late, SHORT Rate)
{
    SVECTOR Sv;
    SVECTOR *pSv;
    SHORT Hokan;

    pSv = &Sv;
    Hokan = 0x1000 - G.pEm->UnkCA * Rate;
    do {
        pSv->vx = pP->Unk88 + ((ULONG)(pP->Unk90 * Late) >> 8);
        pSv->vy = pP->Unk8A + ((ULONG)(pP->Unk92 * Late) >> 8);
        pSv->vz = pP->Unk8C + ((ULONG)(pP->Unk8E * Late) >> 8);
        func_8002720C((SVECTOR *)&pP->Cdir_x, pSv, (SVECTOR *)&pP->Cdir_x, Hokan);
        if (!(pP->Be_flg & 0x10)) {
            RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        }
        pP++;
    } while (--n != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B2BC);

void func_8002B5FC(PARTS_W *pP, ULONG Cnt)
{
    LONG w;
    LONG w2;

    do {
        *(ULONG *)&pP->Unk90 &= 0xFFF0FFF;
        pP->Unk90 -= pP->Unk88;
        w = pP->Unk90;
        w += 0x800;
        if ((ULONG)w > 0x1000) {
            pP->Unk90 += !(w & 0x80000000) ? -0x1000 : 0x1000;
        }
        pP->Unk92 -= pP->Unk8A;
        w2 = pP->Unk92;
        w2 += 0x800;
        if ((ULONG)w2 > 0x1000) {
            pP->Unk92 += !(w2 & 0x80000000) ? -0x1000 : 0x1000;
        }
        pP->Unk8E &= 0xFFF;
        pP->Unk8E -= pP->Unk8C;
        w = pP->Unk8E;
        w += 0x800;
        if ((ULONG)w > 0x1000) {
            pP->Unk8E += !(w & 0x80000000) ? -0x1000 : 0x1000;
        }
        pP++;
    } while (--Cnt);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B6FC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B86C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002BA3C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002C068);

void func_8002C680(MD_OBJ *pObj, ULONG Ctp, ULONG Ccl, ULONG V_ofs, ULONG Add)
{
    ULONG *p;
    ULONG n;

    Ctp <<= 16;
    Ccl <<= 22;
    V_ofs <<= 8;
    p = pObj->pPrim3;
    for (n = pObj->n_prim3; n != 0; n--) {
        p[1] += Ctp;
        p[0] = p[0] + Ccl + V_ofs;
        p[1] += V_ofs;
        p[2] += V_ofs;
        p[0] += Add;
        p[1] += Add;
        p[2] += Add;
        p += 3;
    }
    p = pObj->pPrim4;
    for (n = pObj->n_prim4; n != 0; n--) {
        p[1] += Ctp;
        p[0] = p[0] + Ccl + V_ofs;
        p[1] += V_ofs;
        p[2] += V_ofs;
        p[3] += V_ofs;
        p[0] += Add;
        p[1] += Add;
        p[2] += Add;
        p[3] += Add;
        p += 4;
    }
}

void Md_set_abr(MD_OBJ *pObj, ULONG Abr)
{
    ULONG n;
    ULONG *p;

    n = pObj->n_prim3;
    Abr <<= 21;
    p = pObj->pPrim3;
    while (n != 0) {
        n--;
        p[1] = (p[1] & ~0x600000) | Abr;
        p += 3;
    }
    n = pObj->n_prim4;
    p = pObj->pPrim4;
    while (n != 0) {
        n--;
        p[1] = (p[1] & ~0x600000) | Abr;
        p += 4;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002C824);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002CABC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002CE08);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002CF90);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D184);

void func_8002D41C(RGB_BLEND_WORK *p, RGB_BLEND_WORK *pEnd, ULONG Data, ULONG Rgb)
{
    ULONG c0;
    ULONG c1;
    ULONG Rb;

    Rb = Rgb & 0xFF00FF;
    Rgb &= 0xFF00;
    do {
        c0 = p->Rgb0;
        c1 = p->Rgb1;
        p->Unk1C = Data;
        p->Rgb0 = ((((c0 & 0xFF00FF) + Rb) >> 1) & 0xFF00FF) | ((((c0 & 0xFF00) + Rgb) >> 1) & 0xFF00);
        p->Rgb1 = ((((c1 & 0xFF00FF) + Rb) >> 1) & 0xFF00FF) | ((((c1 & 0xFF00) + Rgb) >> 1) & 0xFF00);
        p++;
    } while (p <= pEnd);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D4B0);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D5D8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D6D8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D898);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DAAC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DC98);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DE0C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DFE0);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002E160);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002E318);

POLY_GT4 *func_8002E61C(MD_WORK2 *pW, POLY_GT4 *pPrim)
{
    MD_OBJ *pObj;
    ULONG n;
    ULONG *p;
    ULONG Abr;
    POLY_GT4 *pP;

    pObj = pW->pObj;
    pP = pPrim;
    pW->pPrim4 = pPrim;
    n = pObj->n_prim4;
    p = pObj->pPrim4;
    pPrim += n * 2;
    Abr = (pW->Attr & 0xC) << 19;
    while (n-- != 0) {
        setPolyGT4(pP);
        *(ULONG *)&pP->r0 = 0x34808080;
        *(ULONG *)&pP->u0 = p[0];
        *(ULONG *)&pP->r1 = 0x808080;
        *(ULONG *)&pP->u1 = (p[1] | Abr) & 0xFFFFFF;
        *(ULONG *)&pP->r2 = 0x808080;
        *(ULONG *)&pP->u2 = *(USHORT *)&p[2];
        *(ULONG *)&pP->r3 = 0x808080;
        *(ULONG *)&pP->u3 = *(USHORT *)&p[3];
        pP[1] = pP[0];
        pP += 2;
        p += 4;
    }
    return pPrim;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002E74C);

void Draw_list_all(ULONG Arg0, ULONG Arg1, ULONG Arg2)
{
    ENEMY_WORK **ppEm;

    func_8002EB9C(Arg0, Arg1, Arg2, G.pUnk265C);
    if (G.pUnk2660->Be_flg & 1) {
        func_8002EB9C(Arg0, Arg1, Arg2, G.pUnk2660);
    }
    for (ppEm = G.pEnemy; ppEm != G.ppEnemy_end; ppEm++) {
        if ((*ppEm)->Be_flg & 1) {
            func_8002EB9C(Arg0, Arg1, Arg2, *ppEm);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002EB9C);

void Print_color(UCHAR A0, UCHAR Col, USHORT Attr)
{
    Moji.Unk75C9 = A0;
    Moji.Mess_attr = Attr | (Col << 3);
}

void Print8(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), Moji.Mess_attr, &pFmt);
}

void Print14(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), Moji.Mess_attr | 0x8000, &pFmt);
}

void Print8_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), (Moji.Mess_attr & 0xFF07) | (Col << 3), &pFmt);
}

void Print14_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), ((SHORT)(Moji.Mess_attr & 0xFF07) | 0x8000) | (Col << 3), &pFmt);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002F440);

void func_8002F98C(MOJIDISP *pMoji, UCHAR *pStr, ULONG Xy, ULONG Attr)
{
    ULONG Col;
    ULONG Mode;
    ULONG Clut;
    ULONG Tag;
    ULONG c;
    SPRT_8 *pPrim;
    SPRT_8 *pEnd;
    ULONG *pPrev;

    if (*pStr == 0) {
        return;
    }
    Col = (Attr & 0xF8) >> 3;
    Mode = Attr & 7;
    pPrim = pMoji->pPrim8_cur;
    pEnd = &pMoji->Prim_sprt_8[G.Cbuf_id][128];
    Clut = GetClut(0x100, Col | 0x1E0) << 16;
    Col = (Col & 1) << 25;
    pPrev = (ULONG *)&pMoji->Moji_mode[G.Cbuf_id][Mode * 2];
    Tag = *pPrev;
    do {
        if (*pStr != ' ') {
            *(ULONG *)&pPrim->r0 = Col | 0x74808080;
            *(ULONG *)&pPrim->x0 = Xy;
            c = *pStr - 0x20;
            *(ULONG *)&pPrim->u0 = ((c & 0x1F) << 3) + ((c & 0xE0) << 6) + Clut;
            *pPrev = (*pPrev & 0xFF000000) | ((ULONG)pPrim & 0xFFFFFF);
            pPrev = (ULONG *)pPrim;
            pPrim++;
            if (pPrim == pEnd) {
                break;
            }
        }
        pStr++;
        Xy += 8;
    } while (*pStr);
    *pPrev = (*pPrev & 0xFF000000) | (Tag & 0xFFFFFF);
    pMoji->pPrim8_cur = pPrim;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002FB14);

LONG func_8002FD30(ULONG Pos_xy, USHORT Attr, ULONG Mess_no, ULONG Stop_data)
{
    MOJIDISP *pMoji;
    tagGLOBAL *pG;
    UCHAR *pTop;

    pMoji = &Moji;
    pG = &G;
    if (pMoji->Mess_flg & 0x80) {
        return -1;
    }
    pMoji->Mess_flg = 0x80;
    pG->Unk7848 = 0;
    pMoji->Stop_data = Stop_data;
    pMoji->Mess_r_no0 = 0;
    pMoji->Mess_attr2 = Attr | 0x100;
    pG->Status_flg |= 0x100;
    pMoji->Stop_keep = pG->Unk2468;
    switch (Attr & 0x3000) {
    case 0:
        pMoji->Pos_x_def = 0x22;
        pMoji->Pos_y_def = 0xB9;
        pMoji->Mess_top = D_80098E88 + D_80099654[Mess_no];
        break;
    case 0x1000:
        pMoji->Pos_x_def = 0xE;
        pMoji->Pos_y_def = 0xAD;
        pMoji->Mess_top = D_800996E4 + D_8009BDB4[Mess_no];
        break;
    case 0x2000:
        pMoji->Pos_x_def = 0x22;
        pMoji->Pos_y_def = 0xB9;
        pTop = pG->pRoom->pMessage;
        pMoji->Mess_top = pTop;
        pMoji->Mess_top = pTop + *(USHORT *)(pTop + (Mess_no << 1));
        break;
    default:
        pMoji->Pos_x_def = Pos_xy;
        pMoji->Pos_y_def = Pos_xy >> 16;
        pMoji->Mess_top = (UCHAR *)Mess_no;
        break;
    }
    return 0;
}

void Moji_trans_main(void)
{
    MOJIDISP *pMoji = &Moji;

    if ((pMoji->Mess_flg & 0x40) && !(G.Key & 0x80)) {
        pMoji->Mess_flg &= ~0x40;
    }
    if (pMoji->Mess_flg & 0x80) {
        func_8002FEE0(pMoji);
        func_80030CB8(pMoji);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002FEE0);
