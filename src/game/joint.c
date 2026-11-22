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
#define EM_M(p) ((MATRIX *)(p)->Workm_m)
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
ULONG J_seq2(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late_flg);
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

ULONG func_800261AC(ENEMY_WORK *pEm, ULONG work_top, ULONG *pKan)
{
    LONG Unused[28];
    ULONG Link_cnt;
    ULONG Part_no;
    PARTS_W *pTop;
    PARTS_W *pCur;
    UCHAR *pLink_top;
    UCHAR *pChild;
    ULONG *pTmd_cur;
    ULONG *pLink;
    ULONG n;
    ULONG Cnt;
    SHORT *pPos;

    Part_no = 0;
    pPos = (SHORT *)pKan + 4;
    n = pEm->Parts_num;
    pCur = pEm->pSin_parts_ptr;
    pTmd_cur = pEm->pTmd;
    pLink_top = (UCHAR *)pKan + (*(USHORT *)pKan & 0xFFFC);
    pLink = (ULONG *)pLink_top;
    Link_cnt = ((USHORT *)pKan)[2];
    pTop = pCur;
    pCur->pSuper = (MATRIX *)pEm->Workm_m;
    pCur->pOya_parts = (PARTS_W *)pEm;
    while (n-- != 0) {
        Copy_matrix(&GsIDMATRIX, &pCur->Workm);
        Copy_rot_matrix(&GsIDMATRIX, (MATRIX *)pCur->M);
        pCur->Poly_rgb = 0x808080;
        pCur->Attribute = 0;
        *(ULONG *)&pCur->Cdir_x = 0;
        pCur->Cdir_z = 0;
        PW_FREE(pCur)[0] = 0;
        PW_FREE(pCur)[1] = 0;
        PW_FREE(pCur)[2] = 0;
        PW_FREE(pCur)[3] = 0;
        PW_FREE(pCur)[4] = 0;
        PW_FREE(pCur)[5] = 0;
        pCur->Be_flg = 1;
        *(SHORT *)&pCur->Workm.t[0] = 0;
        pCur->Unk84 = Part_no;
        pCur->UnkA2 = 0xB;
        pCur->pTmd = pTmd_cur;
        if ((USHORT)pTmd_cur[2] < 4) {
            pCur->UnkA2 = 0;
            pTmd_cur[2] = 0;
        }
        pCur->Old_x = pCur->Pos_x = pPos[0];
        pCur->Old_y = pCur->Pos_y = pPos[1];
        pCur->Old_z = pCur->Pos_z = pPos[2];
        pPos += 3;
        if (Link_cnt != 0) {
            pChild = pLink_top + (*pLink >> 16);
            Cnt = *pLink & 0xFFFF;
            while (Cnt-- != 0) {
                pTop[*pChild].pSuper = &pCur->Workm;
                pTop[*pChild].pOya_parts = pCur;
                pChild++;
            }
            pLink++;
            Link_cnt--;
        } else {
            pCur->pSuper = &GsIDMATRIX;
            pCur->pOya_parts = NULL;
        }
        pCur++;
        pTmd_cur += 6;
        Part_no++;
    }
    return work_top;
}

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
void func_80026728_(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate) __asm__("HMatrix");

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

void HMatrix1(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate)
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

void Joint_move(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
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
        J_seq(Kan_t_ptr, Seq_t_ptr, p, Late_flg);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/joint", J_seq);

void func_8002720C_(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, LONG Rate) __asm__("hokan_svec");

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

LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3); /* RE2 Sca_get_ground */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, ULONG a4); /* RE2 Esp_call */

void func_80027568(PARTS_W *pP_ptr, MATRIX *wm)
{
    MATRIX M;
    SVECTOR Sv;
    VECTOR Pos;
    LONG Floor_y;

    if (pP_ptr->Unk96 == 0) {
        Sv.vy = pP_ptr->Old_x;
        Sv.vz = 0;
        Sv.vx = 0;
        RotMatrix(&Sv, &M);
        Sv.vx = pP_ptr->Old_z;
        Sv.vy = pP_ptr->Old_y;
        ApplyMatrixSV(&M, &Sv, (SVECTOR *)&pP_ptr->Old_x);
        pP_ptr->Unk96 = 3;
        pP_ptr->Unk86 = 0xF;
    }
    if (pP_ptr->Unk96 > 1) {
        Pos = *(VECTOR *)pP_ptr->Workm.t;
        Floor_y = func_8004D720(pP_ptr->Workm.t, 100, 0x2000, 1);
        wm->t[0] += pP_ptr->Old_x;
        pP_ptr->Old_y += (signed char)pP_ptr->Unk85;
        wm->t[1] += pP_ptr->Old_y;
        wm->t[2] += pP_ptr->Old_z;
        if (func_8004D720(pP_ptr->Workm.t, 100, 0x2000, 1) < Floor_y) {
            *(VECTOR *)pP_ptr->Workm.t = Pos;
            pP_ptr->Old_x >>= 2;
            pP_ptr->Old_z >>= 2;
            pP_ptr->Old_x *= -1;
            pP_ptr->Old_z *= -1;
        }
        if (Floor_y < wm->t[1]) {
            pP_ptr->Unk96--;
            wm->t[1] -= pP_ptr->Old_y;
            pP_ptr->Old_y = (SHORT)(pP_ptr->Old_y * -1) >> 2;
            pP_ptr->Old_x >>= 2;
            pP_ptr->Old_z >>= 2;
            func_8001B484(0x1900, 0x7D00000 | (USHORT)(func_800102E8() << 4), &pP_ptr->Workm, NULL, 0);
            if (pP_ptr->Unk96 == 1) {
                pP_ptr->Be_flg &= ~1;
                func_8001B894(0xFF05, &pP_ptr->Workm);
            }
        }
    }
    if (pP_ptr->Unk90 != 0xFF && pP_ptr->Unk86 >= 0) {
        Sv.vx = pP_ptr->Unk90;
        Sv.vy = pP_ptr->Unk92;
        Sv.vz = pP_ptr->Unk8E;
        RotMatrix(&Sv, &M);
        func_80026728_(&pP_ptr->Workm, &M, &pP_ptr->Workm, 0x1000 - pP_ptr->Unk86 * 0x100);
        pP_ptr->Unk86--;
    }
}

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

void Joint_move2(ENEMY_WORK *pEm, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
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
        J_seq2(Kan_t_ptr, Seq_t_ptr, p, (Late_flg & 0xFFFF) | (f_num << 16));
    }
}

ULONG J_seq2(ULONG Kan_t_ptr, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late_flg)
{
    ENEMY_WORK *pEm;
    ULONG *pRot;
    PARTS_W *pP;
    LONG w;
    ULONG Hokan;
    ULONG n;
    ULONG Max;
    VECTOR V;
    MATRIX M;

    pEm = G.pEm;
    pRot = (ULONG *)(Kan_t_ptr + ((KAN_HEAD *)Kan_t_ptr)->Bone_ofs);
    pRot = (ULONG *)((UCHAR *)pRot + (((KAN_HEAD *)Kan_t_ptr)->Rec_sz >> 18) * (((USHORT *)(Seq_t_ptr + pEm->Move_no * 8))[2] + *(UCHAR *)pSeq) * 4);
    pP = pEm->pSin_parts_ptr;
    w = ((SHORT *)pRot)[3];
    w *= pEm->Unk12A;
    pP->Pos_y = w >> 12;
    Hokan = pEm->UnkCA;
    n = *(USHORT *)(Kan_t_ptr + 4);
    Max = Late_flg >> 16;
    if (Hokan != 0) {
        V.vx = pP->Old_x;
        V.vy = pP->Old_y;
        V.vz = pP->Old_z;
        w = (Late_flg & 0xFFFF) * Hokan;
        LoadAverage12((VECTOR *)&pP->Pos_x, &V, 0x1000 - w, w, (VECTOR *)&pP->Pos_x);
    }
    pRot += 2;
    pP->Old_x = pP->Pos_x;
    pP->Old_y = pP->Pos_y;
    pP->Old_z = pP->Pos_z;
    *(ULONG *)&pP[0].Unk88 = (((USHORT *)pRot)[0] & 0xFFF) | ((pRot[0] & 0xFFF000) << 4);
    pP[0].Unk8C = ((UCHAR *)pRot)[3] | ((((USHORT *)pRot)[2] & 0xF) << 8);
    *(ULONG *)&pP[1].Unk88 = ((pRot[1] & 0xFFF0) >> 4) | (pRot[1] & 0xFFF0000);
    pP[1].Unk8C = (pRot[1] >> 28) | (((UCHAR *)pRot)[8] << 4);
    *(ULONG *)&pP[2].Unk88 = ((pRot[2] & 0xFFF00) >> 8) | ((pRot[2] & 0xFFF00000) >> 4);
    pP[2].Unk8C = ((USHORT *)pRot)[6] & 0xFFF;
    *(ULONG *)&pP[3].Unk88 = ((pRot[3] & 0xFFF000) >> 12) | (((pRot[3] & 0xFF000000) >> 8) | ((pRot[4] & 0xF) << 24));
    pP[3].Unk8C = (pRot[4] & 0xFFF0) >> 4;
    *(ULONG *)&pP[4].Unk88 = ((LONG)(pRot[4] & 0xFFF0000) >> 16) | (((pRot[4] & 0xF0000000) >> 12) | (((UCHAR *)pRot)[20] << 20));
    pP[4].Unk8C = (pRot[5] & 0xFFF00) >> 8;
    *(ULONG *)&pP[5].Unk88 = (pRot[5] >> 20) | ((pRot[6] & 0xFFF) << 16);
    pP[5].Unk8C = (pRot[6] & 0xFFF000) >> 12;
    *(ULONG *)&pP[6].Unk88 = ((UCHAR *)pRot)[27] | ((((USHORT *)pRot)[14] & 0xF) << 8) | ((pRot[7] & 0xFFF0) << 12);
    pP[6].Unk8C = (pRot[7] & 0xFFF0000) >> 16;
    *(ULONG *)&pP[7].Unk88 = (pRot[7] >> 28) | (((UCHAR *)pRot)[32] << 4) | ((pRot[8] & 0xFFF00) << 8);
    pP[7].Unk8C = pRot[8] >> 20;
    *(ULONG *)&pP[8].Unk88 = (((USHORT *)pRot)[18] & 0xFFF) | ((pRot[9] & 0xFFF000) << 4);
    pP[8].Unk8C = ((UCHAR *)pRot)[39] | ((((USHORT *)pRot)[20] & 0xF) << 8);
    *(ULONG *)&pP[9].Unk88 = ((pRot[10] & 0xFFF0) >> 4) | (pRot[10] & 0xFFF0000);
    pP[9].Unk8C = (pRot[10] >> 28) | (((UCHAR *)pRot)[44] << 4);
    *(ULONG *)&pP[10].Unk88 = ((pRot[11] & 0xFFF00) >> 8) | ((pRot[11] & 0xFFF00000) >> 4);
    pP[10].Unk8C = ((USHORT *)pRot)[24] & 0xFFF;
    *(ULONG *)&pP[11].Unk88 = ((pRot[12] & 0xFFF000) >> 12) | (((pRot[12] & 0xFF000000) >> 8) | ((pRot[13] & 0xF) << 24));
    pP[11].Unk8C = (pRot[13] & 0xFFF0) >> 4;
    *(ULONG *)&pP[12].Unk88 = ((LONG)(pRot[13] & 0xFFF0000) >> 16) | (((pRot[13] & 0xF0000000) >> 12) | (((UCHAR *)pRot)[56] << 20));
    pP[12].Unk8C = (pRot[14] & 0xFFF00) >> 8;
    *(ULONG *)&pP[13].Unk88 = (pRot[14] >> 20) | ((pRot[15] & 0xFFF) << 16);
    pP[13].Unk8C = (pRot[15] & 0xFFF000) >> 12;
    *(ULONG *)&pP[14].Unk88 = ((UCHAR *)pRot)[63] | ((((USHORT *)pRot)[32] & 0xF) << 8) | ((pRot[16] & 0xFFF0) << 12);
    pP[14].Unk8C = (pRot[16] & 0xFFF0000) >> 16;
    if (Hokan == 0) {
    copy:
        *(ULONG *)&pP->Cdir_x = *(ULONG *)&pP->Unk88;
        pP->Cdir_z = pP->Unk8C;
        RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        pP++;
        if (--n != 0) {
            goto copy;
        }
    } else {
        pEm->UnkCA--;
    hokan:
        if (n >= 7) {
            func_8002720C_((SVECTOR *)&pP->Unk88, (SVECTOR *)&pP->Cdir_x, (SVECTOR *)&pP->Cdir_x, w);
            RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        } else {
            RotMatrix_gte((SVECTOR *)&pP->Unk88, &M);
            func_80026728_((MATRIX *)pP->M, &M, (MATRIX *)pP->M, 0xFFF - w);
        }
        pP++;
        n--;
        if (n != 0) {
            goto hokan;
        }
    }
    if (++pEm->Move_cnt >= Max) {
        pEm->Move_cnt = 0;
        return 1;
    }
    return 0;
}

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
            hokan_svec((SVECTOR *)&pP->Cdir_x, pSv, (SVECTOR *)&pP->Cdir_x, Hokan);
            RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        } else {
            RotMatrix_gte(pSv, &M);
            HMatrix((MATRIX *)pP->M, &M, (MATRIX *)pP->M, Hokan);
        }
    } while (--n != 0);
}

/* RE2 Joint_move_p; the header prototype takes the tables as pointers */
ULONG func_80028308_(JOINT_P *pJp, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg) __asm__("func_80028308");

ULONG func_80028308_(JOINT_P *pJp, ULONG Kan_t_ptr, ULONG Seq_t_ptr, ULONG Late_flg)
{
    ULONG *pRot;
    USHORT *pSeq;
    ULONG f_base;
    PARTS_W *pP;
    PARTS_W *pP_walk;
    ULONG Hokan;
    ULONG P0_no;
    ULONG On_flg;
    ULONG Max;
    ULONG n;
    ULONG Cnt;
    LONG w;
    LONG dx;
    LONG dy;
    LONG dz;
    SVECTOR Unused[2];                 /* frame space only */

    pSeq = (USHORT *)(Seq_t_ptr + pJp->Move_no * 8);
    pSeq = (USHORT *)(Seq_t_ptr + pSeq[1]) + pJp->Move_cnt;
    f_base = ((USHORT *)(Seq_t_ptr + pJp->Move_no * 8))[2];
    pRot = (ULONG *)(Kan_t_ptr + ((KAN_HEAD *)Kan_t_ptr)->Bone_ofs);
    pRot += (((KAN_HEAD *)Kan_t_ptr)->Rec_sz >> 18) * (f_base + *(UCHAR *)pSeq);
    w = ((SHORT *)pRot)[3];
    w *= G.pEm->Unk12A;
    Hokan = pJp->Hokan_flg;
    pRot += 2;
    P0_no = pJp->P0_parts_no;
    pP = G.pEm->pSin_parts_ptr;
    On_flg = pJp->On_parts_flg;
    Max = *(USHORT *)(Seq_t_ptr + pJp->Move_no * 8);
    pP_walk = pP;
    pJp->P0_m.t[1] = w >> 12;
    Cnt = *(USHORT *)(Kan_t_ptr + 4);
    w = Late_flg & 0xFFFF;
    n = Cnt;
unpack:
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[0].Unk88 = (((USHORT *)pRot)[0] & 0xFFF) | ((pRot[0] & 0xFFF000) << 4);
        pP_walk[0].Unk8C = ((UCHAR *)pRot)[3] | ((((USHORT *)pRot)[2] & 0xF) << 8);
    }
    if (P0_no == 0) {
        P0_no = -1;
        *(ULONG *)&pJp->P0_sv.vx = (((USHORT *)pRot)[0] & 0xFFF) | ((pRot[0] & 0xFFF000) << 4);
        pJp->P0_sv.vz = ((UCHAR *)pRot)[3] | ((((USHORT *)pRot)[2] & 0xF) << 8);
        RotMatrix(&pJp->P0_sv, &pJp->P0_m);
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[1].Unk88 = ((pRot[1] & 0xFFF0) >> 4) | (pRot[1] & 0xFFF0000);
        pP_walk[1].Unk8C = (pRot[1] >> 28) | (((UCHAR *)pRot)[8] << 4);
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[2].Unk88 = ((pRot[2] & 0xFFF00) >> 8) | ((pRot[2] & 0xFFF00000) >> 4);
        pP_walk[2].Unk8C = ((USHORT *)pRot)[6] & 0xFFF;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[3].Unk88 = ((pRot[3] & 0xFFF000) >> 12) | (((pRot[3] & 0xFF000000) >> 8) | ((pRot[4] & 0xF) << 24));
        pP_walk[3].Unk8C = (pRot[4] & 0xFFF0) >> 4;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[4].Unk88 = ((LONG)(pRot[4] & 0xFFF0000) >> 16) | (((pRot[4] & 0xF0000000) >> 12) | (((UCHAR *)pRot)[20] << 20));
        pP_walk[4].Unk8C = (pRot[5] & 0xFFF00) >> 8;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[5].Unk88 = (pRot[5] >> 20) | ((pRot[6] & 0xFFF) << 16);
        pP_walk[5].Unk8C = (pRot[6] & 0xFFF000) >> 12;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[6].Unk88 = ((UCHAR *)pRot)[27] | ((((USHORT *)pRot)[14] & 0xF) << 8) | ((pRot[7] & 0xFFF0) << 12);
        pP_walk[6].Unk8C = (pRot[7] & 0xFFF0000) >> 16;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[7].Unk88 = (pRot[7] >> 28) | (((UCHAR *)pRot)[32] << 4) | ((pRot[8] & 0xFFF00) << 8);
        pP_walk[7].Unk8C = pRot[8] >> 20;
    }
    On_flg >>= 1;
    pRot += 9;
    pP_walk += 8;
    if (--n != 0) {
        goto unpack;
    }
unpacked:
    On_flg = pJp->On_parts_flg;
    if (Hokan == 0) {
        do {
            if (On_flg & 1) {
                *(ULONG *)&pP->Cdir_x = *(ULONG *)&pP->Unk88;
                pP->Cdir_z = pP->Unk8C;
                RotMatrix_gte((SVECTOR *)&pP->Unk88, (MATRIX *)pP->M);
            }
            On_flg >>= 1;
            pP++;
        } while (--Cnt != 0);
    } else {
        do {
            if (On_flg & 1) {
                *(ULONG *)&pP->Cdir_x &= 0xFFF0FFF;
                pP->Cdir_z &= 0xFFF;
                gte_lddp(w);
                gte_ldsv(&pP->Cdir_x);
                dx = pP->Unk88 - pP->Cdir_x + 0x800;
                dy = pP->Unk8A - pP->Cdir_y + 0x800;
                dz = pP->Unk8C - pP->Cdir_z + 0x800;
                if ((ULONG)dx > 0x1000) {
                    pP->Unk88 += (dx & 0x80000000) ? 0x1000 : -0x1000;
                }
                if ((ULONG)dy > 0x1000) {
                    pP->Unk8A += (dy & 0x80000000) ? 0x1000 : -0x1000;
                }
                if ((ULONG)dz > 0x1000) {
                    pP->Unk8C += (dz & 0x80000000) ? 0x1000 : -0x1000;
                }
                gte_gpf12_b();
                gte_lddp(0x1000 - w);
                gte_ldsv(&pP->Unk88);
                gte_gpl12();
                gte_stsv(&pP->Cdir_x);
                RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
            }
            On_flg >>= 1;
            pP++;
            pJp->Hokan_flg--;
        } while (--Cnt != 0);
    }
    if (++pJp->Move_cnt >= Max) {
        pJp->Move_cnt = 0;
        return 1;
    }
    return 0;
}

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
