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
void func_80049898(void);
void func_80077C4C(void);
void func_800493EC(void);
void func_80077F60(void);
extern UCHAR D_800E0186;

void func_80025610_(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM) __asm__("func_80025610");

void MatrixNormal_0(MATRIX *m, MATRIX *n);
void func_80026728_(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate) __asm__("func_80026728");

void MatrixNormal_2(MATRIX *m, MATRIX *n);

void func_8002720C_(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, LONG Rate) __asm__("func_8002720C");

void func_8002B190_(ULONG n, PARTS_W *pP, LONG Rate, ULONG Late) __asm__("func_8002B190");
ULONG func_80027338_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg) __asm__("func_80027338");

void func_800281B4(ULONG n, PARTS_W *pP, ULONG Late, SHORT Rate);
ULONG func_80027F8C_(ULONG Kan_t_ptr, ULONG Seq_t_ptr, LONG No, ULONG Late_flg) __asm__("func_80027F8C");

void func_8002606C_(ULONG *pObj, ULONG Flg, PARTS_W *pP) __asm__("func_8002606C");

extern SHORT D_800CCBBC;
void func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
void func_800784E0(ULONG *p);
void Task_lchain(ULONG Task_no);
int PadGetState(int port);
int PadSetMainMode(int socket, int offs, int lock);

void func_800486F0(void);
void func_80048FF4(void);
void Set_light_default(void);
void Set_view(LONG *pView_p, LONG *pView_r);
void func_80010000(ULONG *pSrc, ULONG *pDst);
void func_8008F8A4(LONG mode);
void func_8008FAD4(LONG mode);
extern USHORT *D_8009DFD0[];           /* per stage room file numbers */

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002AB34);

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

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002AF9C);

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

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002B2BC);

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

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002B6FC);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002B86C);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002BA3C);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002C068);

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

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002C824);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002CABC);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002CE08);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002CF90);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002D184);

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

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002D4B0);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002D5D8);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002D6D8);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002D898);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002DAAC);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002DC98);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002DE0C);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002DFE0);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002E160);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002E318);

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

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002E74C);

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

INCLUDE_ASM("asm/nonmatchings/game/md_sub", func_8002EB9C);
