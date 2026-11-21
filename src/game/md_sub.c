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
/* _b variants: no leading nops, the code fills the two GTE load slots itself */
#define gte_rtpt_b() __asm__ volatile(".word 0x4A280030")
#define gte_rtps_b() __asm__ volatile(".word 0x4A180001")
#define gte_avsz3_b() __asm__ volatile(".word 0x4B58002D")
#define gte_avsz4_b() __asm__ volatile(".word 0x4B68002E")
#define gte_ldv3c(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 8(%0); lwc2 $3, 12(%0); lwc2 $4, 16(%0); lwc2 $5, 20(%0)" : : "r"(r0))
#define gte_stsz3c(r0) __asm__ volatile("swc2 $17, 0(%0); swc2 $18, 4(%0); swc2 $19, 8(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3c(r0) __asm__ volatile("swc2 $12, 0(%0); swc2 $13, 4(%0); swc2 $14, 8(%0)" : : "r"(r0) : "memory")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_stsxy(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile("swc2 $12, 0(%0); swc2 $13, 0(%1); swc2 $14, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stszotz(r0) __asm__ volatile("mfc2 $12, $19; nop; sra $12, $12, 2; sw $12, 0(%0)" : : "r"(r0) : "$12", "memory")
#define gte_ldsz4(r0, r1, r2, r3) __asm__ volatile("mtc2 %0, $16; mtc2 %1, $17; mtc2 %2, $18; mtc2 %3, $19" : : "r"(r0), "r"(r1), "r"(r2), "r"(r3))

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

/* primitive sort parameters (func_80025FB0 -> func_8002DC98 / func_8002DE0C) */
typedef struct tagSORT_PARAM {
    MD_OBJ *pObj;                      /* 0x0 */
    union {
        ULONG Rgb;                     /* 0x4 colour, code in the top byte */
        struct {
            UCHAR pad4[3];
            UCHAR Code;                /* 0x7 primitive code (0x34 GT3, 0x3C GT4) */
        } c;
    } u;
    ULONG *pOt;                        /* 0x8 */
    ULONG UnkC;                        /* 0xC */
    ULONG Unk10;                       /* 0x10 */
    ULONG Unk14;                       /* 0x14 */
} SORT_PARAM;
void func_8002D41C(RGB_BLEND_WORK *p, RGB_BLEND_WORK *pEnd, ULONG Data, ULONG Rgb);
void func_8002D184(void *pWork0, void *pWork1, ULONG Arg2, MATRIX *pM);
POLY_GT4 *func_8002C824(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002CABC(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002CE08(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002CF90(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002DFE0(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002E160(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);

void func_8002D5D8(ULONG *pSxy, USHORT *pSz, MD_OBJ *pObj);
POLY_GT4 *func_8002DC98(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002DE0C(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002D6D8(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002D898(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002B6FC(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002B86C(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void hokan_svec(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, SHORT Rate);
void func_8002B2BC(KAN_HEAD *pKan, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late);
ULONG func_8002C068(ULONG *pTmd, ULONG Packet, PARTS_W *pP);

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

POLY_GT3 *PresetObjectGT3_neo(MD_WORK *pW, POLY_GT3 *pPrim)
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

POLY_GT4 *PresetObjectGT4_neo(MD_WORK *pW, POLY_GT4 *pPrim)
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

void MulLMatrix(MATRIX *lwm, MATRIX *klwm, MATRIX *nlm)
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

void func_8002B5FC(PARTS_W *pP, ULONG Cnt);

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
        hokan_svec((SVECTOR *)&pP->Cdir_x, pSv, (SVECTOR *)&pP->Cdir_x, Hokan);
        if (!(pP->Be_flg & 0x10)) {
            RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        }
        pP++;
    } while (--n != 0);
}

/* animation frame record of the RE3 J_seq3 format */
typedef struct tagKAN_FRAME3 {
    SHORT Pos_x;                       /* 0x0 */
    SHORT Unk2;                        /* 0x2 */
    SHORT Pos_z;                       /* 0x4 */
    SHORT Pos_y;                       /* 0x6 */
    ULONG Rot[1];                      /* 0x8 12 bit x/y/z per part, packed */
} KAN_FRAME3;

/* target rotation x/y (Unk90/Unk92) written as one word */
#define ROT_XY(p) (*(ULONG *)&(p)->Unk90)

void func_8002B2BC(KAN_HEAD *pKan, ULONG Seq_t_ptr, USHORT *pSeq, ULONG Late)
{
    ENEMY_WORK *pEm;
    PARTS_W *pP;
    ULONG *pFrame;
    KAN_FRAME3 *pF0;
    KAN_FRAME3 *pF;
    ULONG Base;
    ULONG Parts_cnt;
    ULONG n;

    pEm = G.pEm;
    if (pEm->Be_flg & 0x4000000) {
        return;
    }
    pEm->Be_flg |= 0x4000000;
    Base = *(USHORT *)(Seq_t_ptr + pEm->Move_no * 8 + 4);
    pFrame = (ULONG *)((UCHAR *)pKan + pKan->Bone_ofs);
    pF0 = (KAN_FRAME3 *)(pFrame + (pKan->Rec_sz >> 18) * (Base + *(UCHAR *)pSeq));
    pP = pEm->pSin_parts_ptr;
    pP->UnkAE = (pF0->Pos_y * pEm->Unk12A) >> 12;
    pP->UnkAC = pF0->Pos_x;
    pP->UnkB0 = pF0->Pos_z;
    if (Late != 0) {
        do {
            pSeq--;
        } while (*pSeq & 0x100);
    } else {
        do {
            pSeq++;
        } while (*pSeq & 0x100);
    }
    pFrame += (pKan->Rec_sz >> 18) * (Base + *(UCHAR *)pSeq);
    pF = (KAN_FRAME3 *)pFrame;
    pP->UnkB6 = ((pF->Pos_y * pEm->Unk12A) >> 12) - pP->UnkAE;
    pP->UnkB4 = pF->Pos_x - pP->UnkAC;
    pP->UnkB2 = pF->Pos_z - pP->UnkB0;
    Parts_cnt = *(USHORT *)&pKan->Rec_sz;
    pFrame = pF->Rot;
    n = Parts_cnt;
loop:
    ROT_XY(&pP[0]) = (pFrame[0] & 0xFFF) | ((pFrame[0] & 0xFFF000) << 4);
    n--;
    pP[0].Unk8E = ((UCHAR *)pFrame)[3] | (*(USHORT *)&pFrame[1] << 8);
    if (n == 0) {
        goto end;
    }
    n--;
    ROT_XY(&pP[1]) = ((pFrame[1] & 0xFFF0) >> 4) | (pFrame[1] & 0xFFF0000);
    pP[1].Unk8E = (pFrame[1] >> 28) | (*(USHORT *)&pFrame[2] << 4);
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[2]) = ((pFrame[2] & 0xFFF00) >> 8) | ((pFrame[2] & 0xFFF00000) >> 4);
    n--;
    pP[2].Unk8E = *(USHORT *)&pFrame[3];
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[3]) = ((pFrame[3] & 0xFFF000) >> 12) | (((pFrame[3] & 0xFF000000) >> 8) | (pFrame[4] << 24));
    n--;
    pP[3].Unk8E = pFrame[4] >> 4;
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[4]) = (pFrame[4] >> 16) | (((pFrame[4] & 0xF0000000) >> 12) | (pFrame[5] << 20));
    n--;
    pP[4].Unk8E = pFrame[5] >> 8;
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[5]) = (pFrame[5] >> 20) | (pFrame[6] << 16);
    n--;
    pP[5].Unk8E = pFrame[6] >> 12;
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[6]) = ((UCHAR *)pFrame)[0x1B] | ((pFrame[7] & 0xF) << 8) | ((pFrame[7] & 0xFFF0) << 12);
    n--;
    pP[6].Unk8E = ((USHORT *)pFrame)[0xF];
    if (n == 0) {
        goto end;
    }
    n--;
    ROT_XY(&pP[7]) = (pFrame[7] >> 28) | ((pFrame[8] & 0xFFF) << 4) | ((pFrame[8] & 0xFFF00) << 8);
    pP[7].Unk8E = pFrame[8] >> 20;
    pFrame += 9;
    pP += 8;
    if (n != 0) {
        goto loop;
    }
end:
    func_8002B5FC(pEm->pSin_parts_ptr, Parts_cnt);
}

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

/* The GT3/GT4 sort family (B6FC, B86C, CF90, D6D8, D898, DC98, DE0C, DFE0, E160) is hand-edited
   compiler output: D6D8/D898/DE0C/DFE0/E160 have frames of 0x1C/0x24 (gcc 2.8 always rounds to 8;
   no other function in the exe does this), D6D8/E160 leave a hole in the s-reg save area, D898
   copies a1 to t0 twice, and every one copies params into t-regs (`addu t0,a1,zero`) to free the
   a-regs as temporaries. gcc 2.8.1 keeps pPacket in a1 (its copy preference outranks pZ0's two
   refs), and no PsyQ inline_c.h macro (4.3-4.5) uses fixed registers, so the copy has no clean
   C source. */
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

/* common head of ENEMY_WORK / PARTS_W from Attribute on (joint.c passes &p->Attribute) */
typedef struct tagMODEL_HEAD {
    ULONG Attribute;                   /* 0x00 */
    MD_OBJ *pObj;                      /* 0x04 pTmd */
    UCHAR pad8[0x3C - 0x8];
    MATRIX Workm;                      /* 0x3C */
    SHORT Cdir_x;                      /* 0x5C */
    SHORT Cdir_y;                      /* 0x5E */
    SHORT Cdir_z;                      /* 0x60 */
    USHORT dummy01;                    /* 0x62 */
    ULONG Poly_rgb;                    /* 0x64 */
} MODEL_HEAD;
#define EM_MD(p) ((MODEL_HEAD *)&(p)->Attribute)

/* floor panel tables, indexed by ENEMY_WORK.Type */
extern UCHAR D_80098B58[][2];          /* panel columns, rows */
extern UCHAR D_80098B8C[];             /* first row bitmask in D_80098BA8 */
extern USHORT D_80098BA8[];            /* one bit per panel, MSB first */

void func_8002AF9C(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);

void func_8002E74C(ENEMY_WORK *pEm, ULONG Abr)
{
    ULONG Sxy[0x3C];
    USHORT Sz[0x3C];
    VECTOR Pos;
    SORT_PARAM Prm;
    MODEL_HEAD *pMd;
    POLY_GT4 *pPrim;
    USHORT *pBits;
    ULONG Dir;
    ULONG W;
    ULONG H;
    ULONG i;
    ULONG j;
    LONG Dx;
    LONG Dz;

    pMd = (MODEL_HEAD *)&pEm->Attribute;
    Dir = pEm->Cdir_y;
    Prm.pObj = pMd->pObj;
    Prm.pOt = (ULONG *)pEm->Unk7C;
    Prm.u.Rgb = pMd->Poly_rgb;
    func_8002D5D8(Sxy, Sz, pMd->pObj);
    Prm.u.c.Code = (Abr << 1) | 0x3C;
    func_8002AF9C(&Prm, (POLY_GT4 *)pEm->Unk180[2] + G.Cbuf_id, Sxy, Sz);
    Dir = (Dir & 0xFFF) >> 10;
    switch (Dir) {
    case 0:
        Dx = 0x708;
        Pos.vx = pEm->Pos_x + 0x384;
        Dz = 0x708;
        Pos.vz = pEm->Pos_z + 0x384;
        break;
    case 1:
        Dx = 0x708;
        Pos.vx = pEm->Pos_x + 0x384;
        Dz = -0x708;
        Pos.vz = pEm->Pos_z - 0x384;
        break;
    case 2:
        Dx = -0x708;
        Pos.vx = pEm->Pos_x - 0x384;
        Dz = Dx;
        Pos.vz = pEm->Pos_z - 0x384;
        break;
    case 3:
        Dx = -0x708;
        Pos.vx = pEm->Pos_x - 0x384;
        Dz = 0x708;
        Pos.vz = pEm->Pos_z + 0x384;
        break;
    }
    pPrim = (POLY_GT4 *)pEm->Unk180[2];
    pPrim += G.Cbuf_id;
    pBits = &D_80098BA8[D_80098B8C[pEm->Type]];
    W = D_80098B58[pEm->Type][0];
    H = D_80098B58[pEm->Type][1];
    switch (Dir) {
    case 0:
    case 2:
        for (j = 0; j < H; j++) {
            for (i = 0; i < W; i++) {
                if (*pBits & (0x8000 >> i)) {
                    if (func_8001020C(&Pos.vx, G.Cccut) != 0) {
                        pPrim->tag |= 0x0C000000;
                    } else {
                        pPrim->tag &= 0xFFFFFF;
                    }
                    pPrim += 2;
                }
                Pos.vx += Dx;
            }
            pBits++;
            Pos.vz += Dz;
            Pos.vx -= W * Dx;
        }
        break;
    case 1:
    case 3:
        for (j = 0; j < H; j++) {
            for (i = 0; i < W; i++) {
                if (*pBits & (0x8000 >> i)) {
                    if (func_8001020C(&Pos.vx, G.Cccut) != 0) {
                        pPrim->tag |= 0x0C000000;
                    } else {
                        pPrim->tag &= 0xFFFFFF;
                    }
                    pPrim += 2;
                }
                Pos.vz += Dz;
            }
            pBits++;
            Pos.vx += Dx;
            Pos.vz -= W * Dz;
        }
        break;
    }
    Draw_list_all((ULONG)pEm, W, H);
}

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

/* water ripple quads, 13 per panel: u0 v0 .. u3 v3 in 1/18 panel units */
extern UCHAR D_80098D68[13][8];

void func_8002EB9C_(ENEMY_WORK *pFloor, ULONG W, ULONG H, ENEMY_WORK *pEm) __asm__("func_8002EB9C");

void func_8002EB9C_(ENEMY_WORK *pFloor, ULONG W, ULONG H, ENEMY_WORK *pEm)
{
    SHORT Area[4];
    VECTOR Pos;
    LONG V[4][2];
    MATRIX M;
    MATRIX Lm;
    LONG Otz;
    POLY_GT4 *pPrim;
    USHORT *pBits;
    UCHAR *pTbl;
    UCHAR V0;
    ULONG *pOt;
    ULONG *pTag;
    tagGLOBAL *pG;
    ULONG Tag;
    ULONG Cnt;
    ULONG Idx;
    ULONG k;
    ULONG i;
    ULONG j;
    LONG Px;
    LONG Pz;

    Area[2] = W * 0x708 - 1;
    Area[0] = 0;
    Area[1] = 0;
    Area[3] = H * 0x708 - 1;
    /* V doubles as the SVECTOR of the hit position */
    ((SVECTOR *)V)->vx = pEm->Pos_x - EM_MD(pFloor)->Workm.t[0];
    ((SVECTOR *)V)->vy = 0;
    ((SVECTOR *)V)->vz = pEm->Pos_z - EM_MD(pFloor)->Workm.t[2];
    M = GsIDMATRIX;
    RotMatrixY(-pFloor->Cdir_y, &M);
    ApplyMatrix(&M, (SVECTOR *)V, &Pos);
    if (func_800101C8(&Pos.vx, Area) == 0) {
        return;
    }
    Pz = Pos.vz / 0x708;
    Px = Pos.vx / 0x708;
    k = 0;
    Cnt = 0;
    pBits = &D_80098BA8[D_80098B8C[pFloor->Type]];
    Idx = Px + Pz * W;
    for (j = 0; j < H; j++) {
        for (i = 0; i < W; i++) {
            if (*pBits & (0x8000 >> i)) {
                Cnt++;
            }
            if (k == Idx) {
                goto found;
            }
            k++;
        }
        pBits++;
    }
found:
    if (!(*pBits & (0x8000 >> i))) {
        return;
    }
    Cnt--;
    pG = &G;
    pPrim = (POLY_GT4 *)pFloor->Unk180[2];
    pPrim += pG->Cbuf_id;
    pPrim += Cnt * 2;
    Tag = pPrim->tag;
    V0 = pPrim->v0;
    if (!(Tag & 0x0C000000)) {
        return;
    }
    if (Sce.Unk1496 >= Sce.Mizu_div_max) {
        return;
    }
    pPrim->tag = Tag & 0xFFFFFF;
    Px *= 0x708;
    Pz *= 0x708;
    Px += 0x708;
    Idx = pPrim->clut;                 /* Idx/k are reused for clut/tpage */
    k = pPrim->tpage;
    pPrim = (POLY_GT4 *)Sce.pMizu_div;
    pPrim += pG->Cbuf_id;
    pPrim += Sce.Unk1496 * 26;
    Sce.Unk1496++;
    M = GsIDMATRIX;
    RotMatrix((SVECTOR *)&EM_MD(pFloor)->Cdir_x, &Lm);
    Lm.t[0] = EM_MD(pFloor)->Workm.t[0];
    Lm.t[1] = EM_MD(pFloor)->Workm.t[1];
    Lm.t[2] = EM_MD(pFloor)->Workm.t[2];
    func_8002D4B0(&GsWSMATRIX, &Lm, &M);
    SetRotMatrix(&M);
    SetTransMatrix(&M);
    pOt = G.Ot1[G.Cbuf_id];
    pTbl = D_80098D68[0];
    j = 0;
    do {
        pPrim->u0 = D_80098D68[j][0] * 63 / 18;
        pPrim->v0 = V0 + D_80098D68[j][1] * 63 / 18;
        *(ULONG *)&pPrim->r1 = 0x808080;
        pPrim->u1 = D_80098D68[j][2] * 63 / 18;
        pPrim->v1 = V0 + D_80098D68[j][3] * 63 / 18;
        *(ULONG *)&pPrim->r2 = 0x808080;
        pPrim->u2 = D_80098D68[j][4] * 63 / 18;
        pPrim->v2 = V0 + D_80098D68[j][5] * 63 / 18;
        *(ULONG *)&pPrim->r3 = 0x808080;
        pPrim->u3 = D_80098D68[j][6] * 63 / 18;
        pPrim->v3 = V0 + D_80098D68[j][7] * 63 / 18;
        V[0][0] = Px - pTbl[0] * 100;
        V[0][1] = Pz + pTbl[1] * 100;
        V[1][0] = Px - pTbl[2] * 100;
        gte_ldv0(V[0]);
        V[1][1] = Pz + pTbl[3] * 100;
        gte_rtps_b();
        V[2][0] = Px - pTbl[4] * 100;
        V[2][1] = Pz + pTbl[5] * 100;
        V[3][0] = Px - pTbl[6] * 100;
        V[3][1] = Pz + pTbl[7] * 100;
        gte_stsxy(&pPrim->x0);
        gte_stszotz(&Otz);
        gte_ldv3(V[1], V[2], V[3]);
        pPrim->clut = Idx;
        pPrim->tpage = k;
        gte_rtpt_b();
        pTbl += 8;
        j++;
        gte_stsxy3(&pPrim->x1, &pPrim->x2, &pPrim->x3);
        pTag = (ULONG *)((Otz >> 3) * 4 + (ULONG)pOt);
        addPrim(pTag, pPrim);
        pPrim += 2;
    } while (j < 13);
}
