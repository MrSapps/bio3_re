#include "sub.h"
#include "joint.h"
#include "sce0.h"
#include "md_sub.h"
#include "sld.h"

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
#define gte_ReadRotMatrix(r0) __asm__ volatile("cfc2 $12, $0; cfc2 $13, $1; sw $12, 0(%0); sw $13, 4(%0); cfc2 $12, $2; cfc2 $13, $3; cfc2 $14, $4; sw $12, 8(%0); sw $13, 12(%0); sw $14, 16(%0); cfc2 $12, $5; cfc2 $13, $6; cfc2 $14, $7; sw $12, 20(%0); sw $13, 24(%0); sw $14, 28(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_ldlv0(r0) __asm__ volatile("lhu $13, 4(%0); lhu $12, 0(%0); sll $13, $13, 16; or $12, $12, $13; mtc2 $12, $0; lwc2 $1, 8(%0)" : : "r"(r0) : "$12", "$13")
#define gte_stlvnl(r0) __asm__ volatile("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(r0) : "memory")
#define gte_rtv0tr() __asm__ volatile("nop; nop; .word 0x4A480012")
#define gte_rtv0tr_b() __asm__ volatile(".word 0x4A480012")
#define gte_rtv1tr() __asm__ volatile("nop; nop; .word 0x4A488012")
#define gte_rtv2tr() __asm__ volatile("nop; nop; .word 0x4A490012")
#define gte_llv0() __asm__ volatile("nop; nop; .word 0x4A4A6412") /* mvmva 1,1,0,3,1: light matrix * v0, lm */

/* 7 diffs (draft in scratchpad): giv base must be &n_prim3, gcc picks the last pObj field read (pPrim4) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", Mapping_md);

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

/* floor panel GT4 sort (Mizu_trans): one colour, screen xy / z from the projected tables */
void SortGT4_lno(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz)
{
    ULONG IdxMask;
    LONG Otz;
    ULONG n;
    ULONG *ot;
    LONG *p;
    ULONG Rgb;
    ULONG i0;
    LONG i1;
    ULONG i2;
    LONG i3;
    LONG k;
    ULONG *xy0;
    ULONG *xy1;
    ULONG *xy2;
    USHORT *z0;
    USHORT *z1;
    USHORT *z2;
    USHORT *z3;
    ULONG *tmp;
    ULONG Mask;                        /* both masks are kept in registers */

    Mask = 0xFFFFFF;
    IdxMask = 0xFF0000;
    n = pPrm->pObj->n_prim4;
    if (n == 0) {
        return;
    }
    p = (LONG *)pPrm->pObj->pPrim4;
    ot = pPrm->pOt;
    Rgb = pPrm->u.Rgb;
    i0 = (p[2] & IdxMask) >> 16;
    i1 = p[2] >> 24;
    i2 = (p[3] & IdxMask) >> 16;
    i3 = p[3] >> 24;
    xy0 = &pSxy[i0];
    xy1 = &pSxy[i1];
    xy2 = &pSxy[i2];
    z0 = &pSz[i0];
    z1 = &pSz[i1];
    z2 = &pSz[i2];
    z3 = &pSz[i3];
loop:
    gte_ldsxy3(*xy0, *xy1, *xy2);
    gte_ldsz4(*z0, *z1, *z2, *z3);
    p += 4;
    n--;
    gte_nclip_b();
    k = i3;
    i0 = (p[2] & IdxMask) >> 16;
    i1 = p[2] >> 24;
    i2 = (p[3] & IdxMask) >> 16;
    i3 = p[3] >> 24;
    xy0 = &pSxy[i0];
    xy1 = &pSxy[i1];
    xy2 = &pSxy[i2];
    z0 = &pSz[i0];
    z1 = &pSz[i1];
    z2 = &pSz[i2];
    z3 = &pSz[i3];
    gte_stopz(&Otz);
    if (Otz <= 0) {
        goto next;
    }
    gte_avsz4_b();
    *(ULONG *)&pPacket->x3 = pSxy[k];
    *(ULONG *)&pPacket->r0 = Rgb;
    *(ULONG *)&pPacket->r1 = Rgb;
    *(ULONG *)&pPacket->r2 = Rgb;
    *(ULONG *)&pPacket->r3 = Rgb;
    gte_stsxy3_gt3(pPacket);
    gte_stotz(&Otz);
    if (Otz & ~0x1FFF) {
        Otz = 0x1FFF;
    }
    tmp = &ot[Otz >> 3];
    Otz = *tmp;
    *tmp = (ULONG)pPacket & Mask;
    pPacket->tag = (Otz & Mask) | 0xC000000;
next:
    pPacket += 2;
    if (n != 0) {
        goto loop;
    }
}

void Parts_hokan(ULONG n, PARTS_W *pP, ULONG Late, LONG Rate)
{
    SVECTOR Sv;
    SVECTOR *pSv;
    SHORT Hokan;

    pSv = &Sv;
    Hokan = 0x1000 - G.pEm->Hokan_flg * (SHORT)Rate;
    do {
        pSv->vx = pP->Rot_x + ((ULONG)(pP->Rot_spd_x * Late) >> 8);
        pSv->vy = pP->Rot_y + ((ULONG)(pP->Rot_spd_y * Late) >> 8);
        pSv->vz = pP->Rot_z + ((ULONG)(pP->Rot_spd_z * Late) >> 8);
        hokan_svec(CDIR_SV(pP), pSv, CDIR_SV(pP), Hokan);
        if (!(pP->Be_flg & 0x10)) {
            RotMatrix_gte(CDIR_SV(pP), PW_M(pP));
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

/* Rot_spd_x/Rot_spd_y written as one word */
#define ROT_XY(p) (*(ULONG *)&(p)->Rot_spd_x)

void Hokan_key_set(ULONG *pKan, ULONG *Seq_t_ptr, USHORT *pSeq, ULONG Late)
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
    Base = *(USHORT *)((UCHAR *)Seq_t_ptr + pEm->Move_no * 8 + 4);
    pFrame = (ULONG *)((UCHAR *)pKan + ((KAN_HEAD *)pKan)->Bone_ofs);
    pF0 = (KAN_FRAME3 *)(pFrame + (((KAN_HEAD *)pKan)->Rec_sz >> 18) * (Base + *(UCHAR *)pSeq));
    pP = pEm->pSin_parts_ptr;
    pP->Pos_base_y = (pF0->Pos_y * pEm->Mot_scale_y) >> 12;
    pP->Pos_base_x = pF0->Pos_x;
    pP->Pos_base_z = pF0->Pos_z;
    if (Late != 0) {
        do {
            pSeq--;
        } while (*pSeq & 0x100);
    } else {
        do {
            pSeq++;
        } while (*pSeq & 0x100);
    }
    pFrame += (((KAN_HEAD *)pKan)->Rec_sz >> 18) * (Base + *(UCHAR *)pSeq);
    pF = (KAN_FRAME3 *)pFrame;
    pP->Pos_step_y = ((pF->Pos_y * pEm->Mot_scale_y) >> 12) - pP->Pos_base_y;
    pP->Pos_step_x = pF->Pos_x - pP->Pos_base_x;
    pP->Pos_step_z = pF->Pos_z - pP->Pos_base_z;
    Parts_cnt = *(USHORT *)&((KAN_HEAD *)pKan)->Rec_sz;
    pFrame = pF->Rot;
    n = Parts_cnt;
loop:
    ROT_XY(&pP[0]) = (pFrame[0] & 0xFFF) | ((pFrame[0] & 0xFFF000) << 4);
    n--;
    pP[0].Rot_spd_z = ((UCHAR *)pFrame)[3] | (*(USHORT *)&pFrame[1] << 8);
    if (n == 0) {
        goto end;
    }
    n--;
    ROT_XY(&pP[1]) = ((pFrame[1] & 0xFFF0) >> 4) | (pFrame[1] & 0xFFF0000);
    pP[1].Rot_spd_z = (pFrame[1] >> 28) | (*(USHORT *)&pFrame[2] << 4);
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[2]) = ((pFrame[2] & 0xFFF00) >> 8) | ((pFrame[2] & 0xFFF00000) >> 4);
    n--;
    pP[2].Rot_spd_z = *(USHORT *)&pFrame[3];
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[3]) = ((pFrame[3] & 0xFFF000) >> 12) | (((pFrame[3] & 0xFF000000) >> 8) | (pFrame[4] << 24));
    n--;
    pP[3].Rot_spd_z = pFrame[4] >> 4;
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[4]) = (pFrame[4] >> 16) | (((pFrame[4] & 0xF0000000) >> 12) | (pFrame[5] << 20));
    n--;
    pP[4].Rot_spd_z = pFrame[5] >> 8;
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[5]) = (pFrame[5] >> 20) | (pFrame[6] << 16);
    n--;
    pP[5].Rot_spd_z = pFrame[6] >> 12;
    if (n == 0) {
        goto end;
    }
    ROT_XY(&pP[6]) = ((UCHAR *)pFrame)[0x1B] | ((pFrame[7] & 0xF) << 8) | ((pFrame[7] & 0xFFF0) << 12);
    n--;
    pP[6].Rot_spd_z = ((USHORT *)pFrame)[0xF];
    if (n == 0) {
        goto end;
    }
    n--;
    ROT_XY(&pP[7]) = (pFrame[7] >> 28) | ((pFrame[8] & 0xFFF) << 4) | ((pFrame[8] & 0xFFF00) << 8);
    pP[7].Rot_spd_z = pFrame[8] >> 20;
    pFrame += 9;
    pP += 8;
    if (n != 0) {
        goto loop;
    }
end:
    Hokan_spd_set(pEm->pSin_parts_ptr, Parts_cnt);
}

/* end of Hokan_key_set: Rot_spd = key angle - Rot, wrapped to +-0x800 per part (no RE2 equivalent) */
void Hokan_spd_set(PARTS_W *pP, ULONG Cnt)
{
    LONG w;
    LONG w2;

    do {
        *(ULONG *)&pP->Rot_spd_x &= 0xFFF0FFF;
        pP->Rot_spd_x -= pP->Rot_x;
        w = pP->Rot_spd_x;
        w += 0x800;
        if ((ULONG)w > 0x1000) {
            pP->Rot_spd_x += !(w & 0x80000000) ? -0x1000 : 0x1000;
        }
        pP->Rot_spd_y -= pP->Rot_y;
        w2 = pP->Rot_spd_y;
        w2 += 0x800;
        if ((ULONG)w2 > 0x1000) {
            pP->Rot_spd_y += !(w2 & 0x80000000) ? -0x1000 : 0x1000;
        }
        pP->Rot_spd_z &= 0xFFF;
        pP->Rot_spd_z -= pP->Rot_z;
        w = pP->Rot_spd_z;
        w += 0x800;
        if ((ULONG)w > 0x1000) {
            pP->Rot_spd_z += !(w & 0x80000000) ? -0x1000 : 0x1000;
        }
        pP++;
    } while (--Cnt);
}

/* The GT3/GT4 sort family (B6FC, B86C, CF90, D6D8, D898, DC98, DE0C, DFE0, E160): D6D8/D898/DE0C/DFE0/E160
   have frames of 0x1C/0x24 (gcc 2.8 always rounds to 8; no other function in the exe does this) and
   D6D8/E160 leave a hole in the s-reg save area, so those are hand-edited. The parameter copies into
   t-regs (`addu t0,a1,zero`) are not: B6FC is plain C with a goto loop whose next vertex numbers are read
   ahead in the loop body (the index temps then take the a-regs and the parameters move to t-regs), like
   80079EC0.c Skin_sort_gt3/Skin_sort_gt4. */

/* sort the GT3 prims of pPrm->pObj (one colour for all vertices) into packets 0x68 bytes apart:
   back faces culled with nclip, OT depth avsz3 >> 3; returns the next free packet */
POLY_GT4 *SortGT3_lno_uv(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz)
{
    ULONG *pPrim;
    ULONG *pOt;
    ULONG Rgb;
    ULONG Mask;
    ULONG Vmask;
    ULONG n;
    LONG v0;
    LONG v1;
    LONG v2;
    LONG Z;
    USHORT *pZ0;
    USHORT *pZ1;
    USHORT *pZ2;
    ULONG *pO;

    Mask = 0xFFFFFF;
    Vmask = 0xFF0000;
    n = pPrm->pObj->n_prim3;
    if (n == 0) {
        goto end;
    }
    pPrim = pPrm->pObj->pPrim3;
    pOt = pPrm->pOt;
    Rgb = pPrm->u.Rgb;
    v0 = (LONG)pPrim[1] >> 24;
    v1 = (pPrim[2] & Vmask) >> 16;
    v2 = (LONG)pPrim[2] >> 24;
loop:
    gte_ldsxy3(pSxy[v0], pSxy[v1], pSxy[v2]);
    pPrim += 3;
    n--;
    gte_nclip_b();
    pZ0 = &pSz[v0];
    pZ1 = &pSz[v1];
    v0 = (LONG)pPrim[1] >> 24;
    v1 = (pPrim[2] & Vmask) >> 16;
    pZ2 = &pSz[v2];
    v2 = (LONG)pPrim[2] >> 24;
    gte_stopz(&Z);
    if (Z > 0) {
        gte_ldsz3(*pZ0, *pZ1, *pZ2);
        *(ULONG *)&pPacket->u0 = pPrim[-3];
        gte_avsz3_b();
        *(ULONG *)&pPacket->r0 = Rgb;
        *(ULONG *)&pPacket->r1 = Rgb;
        *(ULONG *)&pPacket->r2 = Rgb;
        *(ULONG *)&pPacket->u1 = pPrim[-2] & Mask;
        *(ULONG *)&pPacket->u2 = pPrim[-1];
        gte_stotz(&Z);
        gte_stsxy3_gt3(pPacket);
        pO = &pOt[Z >> 3];
        Z = *pO;
        *pO = (ULONG)pPacket & Mask;
        pPacket->tag = Z | 0x09000000;
        pPacket = (POLY_GT4 *)((UCHAR *)pPacket + 0x68);
    }
    if (n != 0) {
        goto loop;
    }
end:
    return pPacket;
}

/* the same for the GT4 prims; the OT depth is the sum of the four z values >> 7 */
POLY_GT4 *SortGT4_lno_uv(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz)
{
    ULONG *pPrim;
    ULONG *pOt;
    ULONG Rgb;
    ULONG Mask;
    ULONG Vmask;
    ULONG n;
    LONG v0;
    LONG v1;
    LONG v2;
    LONG v3;
    LONG Last; /* fourth vertex of this prim (v3 already holds the next prim's) */
    LONG Z;
    USHORT *pZ0;
    USHORT *pZ1;
    USHORT *pZ2;
    ULONG *pO;

    Mask = 0xFFFFFF;
    Vmask = 0xFF0000;
    n = pPrm->pObj->n_prim4;
    if (n == 0) {
        return pPacket;
    }
    pPrim = pPrm->pObj->pPrim4;
    pOt = pPrm->pOt;
    Rgb = pPrm->u.Rgb;
    v0 = (pPrim[2] & Vmask) >> 16;
    v1 = (LONG)pPrim[2] >> 24;
    v2 = (pPrim[3] & Vmask) >> 16;
    v3 = (LONG)pPrim[3] >> 24;
loop:
    gte_ldsxy3(pSxy[v0], pSxy[v1], pSxy[v2]);
    pPrim += 4;
    n--;
    gte_nclip_b();
    pZ0 = &pSz[v0];
    pZ1 = &pSz[v1];
    pZ2 = &pSz[v2];
    Last = v3;
    v0 = (pPrim[2] & Vmask) >> 16;
    v1 = (LONG)pPrim[2] >> 24;
    v2 = (pPrim[3] & Vmask) >> 16;
    v3 = (LONG)pPrim[3] >> 24;
    gte_stopz(&Z);
    if (Z > 0) {
        Z = (SHORT)*pZ0 + (SHORT)*pZ1 + (SHORT)*pZ2 + (SHORT)pSz[Last];
        *(ULONG *)&pPacket->x3 = pSxy[Last];
        *(ULONG *)&pPacket->r0 = Rgb;
        *(ULONG *)&pPacket->r1 = Rgb;
        *(ULONG *)&pPacket->r2 = Rgb;
        *(ULONG *)&pPacket->r3 = Rgb;
        *(ULONG *)&pPacket->u0 = pPrim[-4];
        *(ULONG *)&pPacket->u1 = pPrim[-3];
        *(ULONG *)&pPacket->u2 = pPrim[-2];
        *(ULONG *)&pPacket->u3 = pPrim[-1];
        gte_stsxy3_gt3(pPacket);
        pO = &pOt[Z >> 7];
        Z = *pO;
        *pO = (ULONG)pPacket & Mask;
        pPacket->tag = Z | 0x0C000000;
        pPacket = (POLY_GT4 *)((UCHAR *)pPacket + 0x68);
    }
    if (n != 0) {
        goto loop;
    }
    return pPacket;
}

/* RE2 Bomb_Parts_SortGT3: every polygon is pushed out along its normal */
POLY_GT4 *Bomb_Parts_SortGT3(MD_OBJ *pObj, POLY_GT4 *pPacket, PARTS_W *pP)
{
    MATRIX bbm1;
    MATRIX bbm0;
    VECTOR Vec;
    LONG D2;
    SVECTOR *sp;
    SVECTOR *np;
    SVECTOR *s0;
    SVECTOR *s1;
    SVECTOR *s2;
    SVECTOR *s3;
    ULONG *p_prim;
    ULONG *ot;
    ULONG *tmp;
    VECTOR *v;
    ULONG cnt0;
    ULONG rgb;
    ULONG Tb_no;
    ULONG Tb_lo;
    ULONG Tb_hi;
    LONG bomb;
    SHORT Abr;
    ULONG i0;
    ULONG i1;
    ULONG i2;

    RotMatrix(&Bomb_rot, &Bomb_m);
    Bomb_rot.vy += 0x80;
    Bomb_rot.vx += 0x80;
    cnt0 = pObj->n_prim3;
    if (cnt0 == 0) {
        return pPacket;
    }
    ot = G.pEm->pUnk7C;
    p_prim = pObj->pPrim3;
    np = (SVECTOR *)pObj->pNorm;
    sp = (SVECTOR *)pObj->pVert;
    i0 = p_prim[1] >> 24;
    i1 = (p_prim[2] >> 16) & 0xFF;
    i2 = p_prim[2] >> 24;
    s0 = &np[i0];
    Tb_no = (USHORT)pP->Unk7C;
    Tb_lo = Tb_no & 0x7FFF;
    Tb_hi = Tb_no >> 15;
    gte_ReadRotMatrix(&bbm0);
    v = (VECTOR *)bbm1.t;
    bomb = Bomb_tbl[Tb_hi][Tb_lo];
    v->vx = (s0->vx >> 10) * bomb;
    v->vy = (s0->vy >> 10) * bomb;
    v->vz = (s0->vz >> 10) * bomb;
    gte_ldlv0(v);
    rgb = pP->Poly_rgb;
    rgb &= 0xFFFFFF;
    rgb |= 0x34000000;
    rgb |= (SHORT)((pP->UnkA2 & 0x100) >> 7) << 24;
    gte_rtv0tr_b();
    s1 = &sp[i0];
    s2 = &sp[i1];
    s3 = &sp[i2];
    do {
        gte_stlvnl(v);
        Bomb_m.t[0] = (s1->vx + s2->vx + s3->vx) / 3;
        Bomb_m.t[1] = (s1->vy + s2->vy + s3->vy) / 3;
        Bomb_m.t[2] = (s1->vz + s2->vz + s3->vz) / 3;
        gte_SetRotMatrix(&Bomb_m);
        gte_SetTransMatrix(&Bomb_m);
        Vec.vx = -Bomb_m.t[0];
        Vec.vy = -Bomb_m.t[1];
        Vec.vz = -Bomb_m.t[2];
        gte_ldlv0(&Vec);
        gte_rtv0tr();
        gte_stlvnl(Bomb_m.t);
        Bomb_m.t[0] += (s0->vx >> 10) * bomb;
        Bomb_m.t[1] += (s0->vy >> 10) * bomb;
        Bomb_m.t[2] += (s0->vz >> 10) * bomb;
        CompM(&bbm0, &Bomb_m, &bbm1);
        gte_SetRotMatrix(&bbm1);
        gte_SetTransMatrix(&bbm1);
        gte_ldv3(s1, s2, s3);
        p_prim += 3;
        cnt0--;
        gte_rtpt_b();
        i0 = p_prim[1] >> 24;
        i1 = (p_prim[2] >> 16) & 0xFF;
        i2 = p_prim[2] >> 24;
        s1 = &sp[i0];
        s2 = &sp[i1];
        s3 = &sp[i2];
        s0 = &np[i0];
        gte_avsz3_b();
        *(ULONG *)&pPacket->r0 = rgb;
        *(ULONG *)&pPacket->r1 = rgb;
        *(ULONG *)&pPacket->r2 = rgb;
        gte_stsxy3_gt3(pPacket);
        gte_stotz(&D2);
        gte_SetTransMatrix(&bbm0);
        gte_SetRotMatrix(&bbm0);
        bomb = Bomb_tbl[Tb_hi][Tb_lo];
        v->vx = (s0->vx >> 10) * bomb;
        v->vy = (s0->vy >> 10) * bomb;
        v->vz = (s0->vz >> 10) * bomb;
        gte_ldlv0(v);
        gte_rtv0tr();
        *(ULONG *)&pPacket->u0 = p_prim[-3];
        *(ULONG *)&pPacket->u1 = p_prim[-2] & 0xFFFFFF;
        *(ULONG *)&pPacket->u2 = p_prim[-1];
        if (D2 > 0x1FFF) {
            D2 = 0x1FFF;
        }
        tmp = (ULONG *)((UCHAR *)ot + (((ULONG)D2 >> 1) & 0xFFC));
        D2 = *tmp;
        *tmp = (ULONG)pPacket & 0xFFFFFF;
        pPacket->tag = D2 | 0x9000000;
        pPacket += 2;
    } while (cnt0 != 0);
    return pPacket;
}

/* RE2 Bomb_Parts_SortGT4 */
POLY_GT4 *Bomb_Parts_SortGT4(MD_OBJ *pObj, POLY_GT4 *pPacket, PARTS_W *pP)
{
    MATRIX bbm0;
    MATRIX bbm1;
    VECTOR Vec;
    LONG D2;
    SVECTOR *sp;
    SVECTOR *np;
    SVECTOR *s0;
    SVECTOR *s1;
    SVECTOR *s2;
    SVECTOR *s3;
    ULONG *p_prim;
    ULONG *ot;
    ULONG *tmp;
    VECTOR *v;
    ULONG cnt0;
    ULONG rgb;
    ULONG Tb_no;
    ULONG Tb_lo;
    ULONG Tb_hi;
    LONG bomb;
    ULONG i0;
    ULONG i1;
    ULONG i2;
    ULONG i3;

    cnt0 = pObj->n_prim4;
    if (cnt0 == 0) {
        return pPacket;
    }
    sp = (SVECTOR *)pObj->pVert;
    np = (SVECTOR *)pObj->pNorm;
    p_prim = pObj->pPrim4;
    ot = G.pEm->pUnk7C;
    Tb_no = (USHORT)pP->Unk7C;
    Tb_lo = Tb_no & 0x7FFF;
    Tb_hi = Tb_no >> 15;
    gte_ReadRotMatrix(&bbm0);
    i0 = (p_prim[2] & 0xFF0000) >> 16;
    i1 = p_prim[2] >> 24;
    i2 = (p_prim[3] & 0xFF0000) >> 16;
    i3 = p_prim[3] >> 24;
    s0 = &np[i0];
    v = (VECTOR *)bbm1.t;
    bomb = Bomb_tbl[Tb_hi][Tb_lo];
    v->vx = (s0->vx >> 10) * bomb;
    v->vy = (s0->vy >> 10) * bomb;
    v->vz = (s0->vz >> 10) * bomb;
    gte_ldlv0(v);
    rgb = pP->Poly_rgb;
    rgb &= 0xFFFFFF;
    rgb |= 0x3C000000;
    rgb |= (SHORT)((pP->UnkA2 & 0x100) >> 7) << 24;
    gte_rtv0tr_b();
    s1 = &sp[i0];
    s2 = &sp[i1];
    s3 = &sp[i2];
    do {
        gte_stlvnl(v);
        Bomb_m.t[0] = (s1->vx + s3->vx) / 2;
        Bomb_m.t[1] = (s1->vy + s3->vy) / 2;
        Bomb_m.t[2] = (s1->vz + s3->vz) / 2;
        gte_SetRotMatrix(&Bomb_m);
        gte_SetTransMatrix(&Bomb_m);
        Vec.vx = -Bomb_m.t[0];
        Vec.vy = -Bomb_m.t[1];
        Vec.vz = -Bomb_m.t[2];
        gte_ldlv0(&Vec);
        gte_rtv0tr();
        gte_stlvnl(Bomb_m.t);
        Bomb_m.t[0] += (s0->vx >> 10) * bomb;
        Bomb_m.t[1] += (s0->vy >> 10) * bomb;
        Bomb_m.t[2] += (s0->vz >> 10) * bomb;
        CompM(&bbm0, &Bomb_m, &bbm1);
        gte_SetRotMatrix(&bbm1);
        gte_SetTransMatrix(&bbm1);
        gte_ldv3(s1, s2, s3);
        p_prim += 4;
        cnt0--;
        gte_rtpt_b();
        s0 = &sp[i3];
        i0 = (p_prim[2] & 0xFF0000) >> 16;
        i1 = p_prim[2] >> 24;
        i2 = (p_prim[3] & 0xFF0000) >> 16;
        i3 = p_prim[3] >> 24;
        s1 = &sp[i0];
        s2 = &sp[i1];
        s3 = &sp[i2];
        gte_stsxy3_gt3(pPacket);
        gte_ldv0(s0);
        gte_rtps();
        *(ULONG *)&pPacket->r0 = rgb;
        *(ULONG *)&pPacket->r1 = rgb;
        *(ULONG *)&pPacket->r2 = rgb;
        *(ULONG *)&pPacket->r3 = rgb;
        gte_avsz4_b();
        gte_stsxy(&pPacket->x3);
        gte_stotz(&D2);
        s0 = &np[i0];
        gte_SetTransMatrix(&bbm0);
        gte_SetRotMatrix(&bbm0);
        bomb = Bomb_tbl[Tb_hi][Tb_lo];
        v->vx = (s0->vx >> 10) * bomb;
        v->vy = (s0->vy >> 10) * bomb;
        v->vz = (s0->vz >> 10) * bomb;
        gte_ldlv0(v);
        gte_rtv0tr();
        *(ULONG *)&pPacket->u0 = p_prim[-4];
        *(ULONG *)&pPacket->u1 = p_prim[-3];
        *(ULONG *)&pPacket->u2 = p_prim[-2];
        *(ULONG *)&pPacket->u3 = p_prim[-1];
        if (D2 > 0x1FFF) {
            D2 = 0x1FFF;
        }
        tmp = (ULONG *)((UCHAR *)ot + (((ULONG)D2 >> 1) & 0xFFC));
        D2 = *tmp;
        *tmp = (ULONG)pPacket & 0xFFFFFF;
        pPacket->tag = D2 | 0xC000000;
        pPacket += 2;
    } while (cnt0 != 0);
    return pPacket;
}

/* add tpage (Ctp), clut (Ccl), v offset and Add to every prim of an MD_OBJ; Em34_Init (RE2 does this inside Mapping_tmd) */
void Md_tex_ofs(MD_OBJ *pObj, ULONG Ctp, ULONG Ccl, ULONG V_ofs, ULONG Add)
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

/* lit GT3 sort: flat-ish light from the normals, back faces and unlit faces skipped */
POLY_GT4 *SortGT3_plus_light(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz)
{
    VECTOR Lv;
    LONG Otz;
    ULONG n;
    ULONG *ot;
    SVECTOR *np;
    LONG *p;
    ULONG Uv0;
    ULONG Uv1;
    ULONG Uv2;
    LONG i0;
    LONG i1;
    LONG i2;
    USHORT *z0;
    USHORT *z1;
    USHORT *z2;
    SVECTOR *n0;
    SVECTOR *n1;
    SVECTOR *n2;
    LONG c0;
    LONG c1;
    LONG c2;
    ULONG *tmp;

    n = pPrm->pObj->n_prim3;
    if (n == 0) {
        return pPacket;
    }
    ReadLightMatrix(&Plus_lm_save);
    SetLightMatrix((MATRIX *)&Model_col); /* the light matrix starts with the MODEL_COL fields */
    ot = pPrm->pOt;
    np = (SVECTOR *)pPrm->pObj->pNorm;
    p = (LONG *)pPrm->pObj->pPrim3;
    i0 = p[1] >> 24;
    i1 = (p[2] >> 16) & 0xFF;
    i2 = p[2] >> 24;
    Uv2 = pPrm->u.c.Code;              /* the code byte selects the uv set */
    Uv0 = Plus_uv_tbl[Uv2][0];
    Uv1 = Plus_uv_tbl[Uv2][1];
    Uv2 = Plus_uv_tbl[Uv2][2];
loop:
    gte_ldsxy3(pSxy[i0], pSxy[i1], pSxy[i2]);
    p += 3;
    n--;
    gte_nclip_b();
    z0 = &pSz[i0];
    z1 = &pSz[i1];
    z2 = &pSz[i2];
    n0 = &np[i0];
    n1 = &np[i1];
    n2 = &np[i2];
    i0 = p[1] >> 24;
    i1 = (p[2] >> 16) & 0xFF;
    i2 = p[2] >> 24;
    gte_stopz(&Otz);
    if (Otz <= 0) {
        goto next;
    }
    gte_ldv0(n0);
    gte_llv0();
    gte_stlvl(&Lv);
    c0 = Lv.vx;
    gte_ldv0(n1);
    gte_llv0();
    gte_stlvl(&Lv);
    c1 = Lv.vx;
    gte_ldv0(n2);
    gte_llv0();
    gte_stlvl(&Lv);
    c2 = Lv.vx;
    if (c0 + c1 + c2 <= 0) {
        goto next;
    }
    gte_ldsz3(*z0, *z1, *z2);
    *(ULONG *)&pPacket->r0 = ((c0 >> 4) * 0x10101) | 0x36000000;
    gte_avsz3_b();
    *(ULONG *)&pPacket->r1 = (c1 >> 4) * 0x10101;
    *(ULONG *)&pPacket->r2 = (c2 >> 4) * 0x10101;
    *(ULONG *)&pPacket->u0 = Uv0;
    *(ULONG *)&pPacket->u1 = Uv1;
    *(ULONG *)&pPacket->u2 = Uv2;
    gte_stotz(&Otz);
    gte_stsxy3_gt3(pPacket);
    tmp = &ot[Otz >> 3];
    Otz = *tmp;
    *tmp = (ULONG)pPacket & 0xFFFFFF;
    pPacket->tag = Otz | 0x9000000;
    pPacket += 2;
next:
    if (n != 0) {
        goto loop;
    }
    return pPacket;
}

/* lit GT4 sort (see SortGT3_plus_light), z from the summed vertex depths; restores the light matrix */
POLY_GT4 *SortGT4_plus_light(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, SHORT *pSz)
{
    VECTOR Lv;
    LONG Otz;
    ULONG n;
    ULONG *ot;
    SVECTOR *np;
    LONG *p;
    ULONG Uv0;
    ULONG Uv1;
    ULONG Uv2;
    ULONG Uv3;
    LONG i0;
    LONG i1;
    LONG i2;
    LONG i3;
    LONG k;
    SHORT *z0;
    SHORT *z1;
    SHORT *z2;
    SVECTOR *n0;
    SVECTOR *n1;
    SVECTOR *n2;
    SVECTOR *n3;
    LONG c0;
    LONG c1;
    LONG c2;
    LONG c3;
    ULONG *tmp;

    n = pPrm->pObj->n_prim4;
    if (n == 0) {
        return pPacket;
    }

    np = (SVECTOR *)pPrm->pObj->pNorm;
    p = (LONG *)pPrm->pObj->pPrim4;
    ot = pPrm->pOt;
    Uv3 = pPrm->u.c.Code;
    Uv0 = Plus_uv_tbl[Uv3][0];
    Uv1 = Plus_uv_tbl[Uv3][1];
    Uv2 = Plus_uv_tbl[Uv3][2];
    Uv3 = Plus_uv_tbl[Uv3][3];
    i0 = (p[2] & 0xFF0000) >> 16;
    i1 = p[2] >> 24;
    i2 = (p[3] & 0xFF0000) >> 16;
    i3 = p[3] >> 24;

loop:
    gte_ldsxy3(pSxy[i0], pSxy[i1], pSxy[i2]);
    p += 4;
    n--;
    gte_nclip_b();
    z0 = &pSz[i0];
    z1 = &pSz[i1];
    z2 = &pSz[i2];
    k = i3;
    n0 = &np[i0];
    n1 = &np[i1];
    n2 = &np[i2];
    n3 = &np[k];
    i0 = (p[2] & 0xFF0000) >> 16;
    i1 = p[2] >> 24;
    i2 = (p[3] & 0xFF0000) >> 16;
    i3 = p[3] >> 24;
    gte_stopz(&Otz);
    if (Otz <= 0) {
        goto next;
    }
    gte_ldv0(n0);
    gte_llv0();
    gte_stlvl(&Lv);
    c0 = Lv.vx;
    gte_ldv0(n1);
    gte_llv0();
    gte_stlvl(&Lv);
    c1 = Lv.vx;
    gte_ldv0(n2);
    gte_llv0();
    gte_stlvl(&Lv);
    c2 = Lv.vx;
    gte_ldv0(n3);
    gte_llv0();
    gte_stlvl(&Lv);
    c3 = Lv.vx;
    if (c0 + c1 + c2 + c3 <= 0) {
        goto next;
    }
    Otz = *z0 + *z1 + *z2 + pSz[k];
    *(ULONG *)&pPacket->x3 = pSxy[k];
    *(ULONG *)&pPacket->r0 = ((c0 >> 4) * 0x10101) | 0x3E000000;
    *(ULONG *)&pPacket->r1 = (c1 >> 4) * 0x10101;
    *(ULONG *)&pPacket->r2 = (c2 >> 4) * 0x10101;
    *(ULONG *)&pPacket->r3 = (c3 >> 4) * 0x10101;
    *(ULONG *)&pPacket->u0 = Uv0;
    *(ULONG *)&pPacket->u1 = Uv1;
    *(ULONG *)&pPacket->u2 = Uv2;
    *(ULONG *)&pPacket->u3 = Uv3;
    gte_stsxy3_gt3(pPacket);
    tmp = &ot[Otz >> 7];
    Otz = *tmp;
    *tmp = (ULONG)pPacket & 0xFFFFFF;
    pPacket->tag = Otz | 0xC000000;
    pPacket += 2;
next:
    if (n != 0) {
        goto loop;
    }
    SetLightMatrix(&Plus_lm_save);
    return pPacket;
}

/* unlit GT3 sort with one colour (semi-transparent code 0x36) */
POLY_GT4 *SortGT3_plus_col(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz)
{
    LONG Unused[4];
    LONG Otz;
    ULONG n;
    ULONG *ot;
    LONG *p;
    ULONG Rgb;
    ULONG w;
    ULONG Uv0;
    ULONG Uv1;
    ULONG Uv2;
    LONG i0;
    LONG i1;
    LONG i2;
    USHORT *z0;
    USHORT *z1;
    USHORT *z2;
    ULONG *tmp;

    n = pPrm->pObj->n_prim3;
    if (n == 0) {
        return pPacket;
    }
    p = (LONG *)pPrm->pObj->pPrim3;
    w = pPrm->u.Rgb;
    ot = pPrm->pOt;
    Uv2 = w >> 24;
    Rgb = (w & 0xFFFFFF) | 0x36000000;
    i0 = p[1] >> 24;
    i1 = (p[2] >> 16) & 0xFF;
    i2 = p[2] >> 24;
    Uv0 = Plus_uv_tbl[Uv2][0];
    Uv1 = Plus_uv_tbl[Uv2][1];
    Uv2 = Plus_uv_tbl[Uv2][2];
loop:
    gte_ldsxy3(pSxy[i0], pSxy[i1], pSxy[i2]);
    p += 3;
    n--;
    gte_nclip_b();
    z0 = &pSz[i0];
    z1 = &pSz[i1];
    z2 = &pSz[i2];
    i0 = p[1] >> 24;
    i1 = (p[2] >> 16) & 0xFF;
    i2 = p[2] >> 24;
    gte_stopz(&Otz);
    if (Otz <= 0) {
        goto next;
    }
    gte_ldsz3(*z0, *z1, *z2);
    gte_avsz3();
    *(ULONG *)&pPacket->r0 = Rgb;
    *(ULONG *)&pPacket->r1 = Rgb;
    *(ULONG *)&pPacket->r2 = Rgb;
    *(ULONG *)&pPacket->u0 = Uv0;
    *(ULONG *)&pPacket->u1 = Uv1;
    *(ULONG *)&pPacket->u2 = Uv2;
    gte_stotz(&Otz);
    gte_stsxy3_gt3(pPacket);
    tmp = &ot[Otz >> 3];
    Otz = *tmp;
    *tmp = (ULONG)pPacket & 0xFFFFFF;
    pPacket->tag = Otz | 0x9000000;
    pPacket += 2;
next:
    if (n != 0) {
        goto loop;
    }
    return pPacket;
}

/* sort the GT4 prims of pPrm->pObj with one colour (code 0x3E) and the fixed uv words Plus_uv_tbl[code]
   (packets 0x68 bytes apart, OT depth = sum of the four z >> 7); returns the next free packet */
POLY_GT4 *SortGT4_plus_col(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz)
{
    SVECTOR Unused[2]; /* frame padding */
    LONG Z;
    LONG *pPrim;
    ULONG *pOt;
    ULONG (*pUv_tbl)[4];
    ULONG *pUv;
    ULONG Rgb;
    ULONG Uv0;
    ULONG Uv1;
    ULONG Uv2;
    ULONG Uv3;
    ULONG n;
    LONG v0;
    LONG v1;
    LONG v2;
    LONG v3;
    LONG Last;
    USHORT *pZ0;
    USHORT *pZ1;
    USHORT *pZ2;
    ULONG *pO;

    n = pPrm->pObj->n_prim4;
    if (n == 0) {
        return pPacket;
    }
    pPrim = (LONG *)pPrm->pObj->pPrim4;
    Uv3 = pPrm->u.Rgb >> 24; /* the primitive code picks the uv set */
    Rgb = (pPrm->u.Rgb & 0xFFFFFF) | 0x3E000000;
    pUv_tbl = Plus_uv_tbl;
    pOt = pPrm->pOt;
    pUv = pUv_tbl[Uv3];
    Uv0 = pUv[0];
    Uv1 = pUv[1];
    Uv2 = pUv[2];
    Uv3 = pUv[3];
    v0 = (pPrim[2] & 0xFF0000) >> 16;
    v1 = pPrim[2] >> 24;
    v2 = (pPrim[3] & 0xFF0000) >> 16;
    v3 = pPrim[3] >> 24;
loop:
    gte_ldsxy3(pSxy[v0], pSxy[v1], pSxy[v2]);
    pPrim += 4;
    n--;
    gte_nclip_b();
    pZ0 = &pSz[v0];
    pZ1 = &pSz[v1];
    pZ2 = &pSz[v2];
    Last = v3;
    v0 = (pPrim[2] & 0xFF0000) >> 16;
    v1 = pPrim[2] >> 24;
    v2 = (pPrim[3] & 0xFF0000) >> 16;
    v3 = pPrim[3] >> 24;
    gte_stopz(&Z);
    if (Z > 0) {
        Z = (SHORT)*pZ0 + (SHORT)*pZ1 + (SHORT)*pZ2 + (SHORT)pSz[Last];
        *(ULONG *)&pPacket->x3 = pSxy[Last];
        *(ULONG *)&pPacket->r0 = Rgb;
        *(ULONG *)&pPacket->r1 = Rgb;
        *(ULONG *)&pPacket->r2 = Rgb;
        *(ULONG *)&pPacket->r3 = Rgb;
        *(ULONG *)&pPacket->u0 = Uv0;
        *(ULONG *)&pPacket->u1 = Uv1;
        *(ULONG *)&pPacket->u2 = Uv2;
        *(ULONG *)&pPacket->u3 = Uv3;
        gte_stsxy3_gt3(pPacket);
        pO = &pOt[Z >> 7];
        Z = *pO;
        *pO = (ULONG)pPacket & 0xFFFFFF;
        pPacket->tag = Z | 0x0C000000;
        pPacket = (POLY_GT4 *)((UCHAR *)pPacket + 0x68);
    }
    if (n != 0) {
        goto loop;
    }
    return pPacket;
}

/* project the vertices flattened onto the floor (y clamped to <= 0) into screen xy / z tables */
void Md_rtp_floor(ULONG *pXy, USHORT *pZ, MD_OBJ *pObj, MATRIX *pM)
{
    LONG Z[3];
    SVECTOR Sv[3];
    SVECTOR *pSv;
    LONG *pz1;
    LONG *pz2;
    SVECTOR *pV;
    ULONG n;
    SHORT *pZ1;                        /* previous triple's z slots, clamped one iteration late */
    SHORT *pZ2;

    pXy -= 3;
    pZ -= 3;
    pz1 = &Z[1];
    pz2 = &Z[2];
    pSv = Sv;
    pV = (SVECTOR *)pObj->pVert;
    n = pObj->n_vert >> 16;
    Z[0] = 0;
    *pz1 = 0;
    *pz2 = 0;
loop:
    gte_ldv3c(pV);
    pV += 3;
    n--;
    gte_SetRotMatrix(pM);
    gte_SetTransMatrix(pM);
    gte_rtv0tr();
    gte_stsv(pSv);
    gte_rtv1tr();
    gte_stsv(pSv + 1);
    gte_rtv2tr();
    gte_stsv(pSv + 2);
    if (pSv[0].vy > 0) {
        pSv[0].vy = 0;
    }
    if (pSv[1].vy > 0) {
        pSv[1].vy = 0;
    }
    if (pSv[2].vy > 0) {
        pSv[2].vy = 0;
    }
    gte_ldv3c(pSv);
    gte_SetRotMatrix(&GsWSMATRIX);
    gte_SetTransMatrix(&GsWSMATRIX);
    gte_rtpt();
    if (Z[0] > 0x7FFF) {
        *pZ = 0x7FFF;
    }
    if (*pz1 > 0x7FFF) {
        *pZ1 = 0x7FFF;
    }
    if (*pz2 > 0x7FFF) {
        *pZ2 = 0x7FFF;
    }
    pXy += 3;
    pZ += 3;
    pZ1 = pZ + 1;
    pZ2 = pZ + 2;
    gte_stsxy3c(pXy);
    gte_stsz3c(Z);
    pZ[0] = Z[0];
    pZ[1] = *pz1;
    pZ[2] = *pz2;
    if (n != 0) {
        goto loop;
    }
    if (Z[0] > 0x7FFF) {
        pZ[0] = 0x7FFF;
    }
    if (*pz1 > 0x7FFF) {
        pZ[1] = 0x7FFF;
    }
    if (*pz2 > 0x7FFF) {
        pZ[2] = 0x7FFF;
    }
}

/* average the colours of the prims p..pEnd with Rgb */
void Rgb_blend(RGB_BLEND_WORK *p, RGB_BLEND_WORK *pEnd, ULONG Data, ULONG Rgb)
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

/* m2 = m0 * m1 incl. translation (hand-written GTE); already aliased as CompM = 0x8002D4B0 in syms/re2_names.txt, so this only renames the INCLUDE_ASM label (RE2 MD_SUB.C CompM) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", CompM);

INCLUDE_ASM("asm/nonmatchings/game/md_sub", Md_rtp);

/* lit GT3 sort (ncct with per-vertex normals), packet built from the MD prim, Parts_sort / Parts_sort_simple Flg8 (RE2 SortGT3_neo variant) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", SortGT3_neo_uv);

/* GT4 counterpart of SortGT3_neo_uv (nccs + ncct), OT = sum of 4 z >> 7 (RE2 SortGT4_neo variant) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", SortGT4_neo_uv);

/* hand-scheduled: Otz is loaded (lw v0,0(sp)) between the lwc2 $0 and lwc2 $1 of one gte_ldv3 asm */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", SortGT3_neo);

/* GT3 sort with per-polygon vertex colours read sequentially from Prm.UnkC (3 words/prim, pointer written back), Parts_sort_ex (no RE2 equivalent) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", SortGT3_col);

/* GT4 counterpart of SortGT3_col (4 colour words/prim) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", SortGT4_col);

/* GT3 sort with per-vertex colours Prm.UnkC[vertex] (= Skin_rgb), Parts_sort_sabun (no RE2 equivalent; cf. Skin_sort_gt3) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", SortGT3_sabun);

/* GT4 counterpart of SortGT3_sabun */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", SortGT4_sabun);

/* dead hand-written twin of Skin_vtx0 (CompM per bone + rtpt of index triplets, z clamp 0x7FFF) using an older work layout (parts at +0x100 stride 0xC8, pos table +0x110) (no RE2 equivalent) */
INCLUDE_ASM("asm/nonmatchings/game/md_sub", Skin_vtx0_old);

/* preset double-buffered GT4 packets of a water object (Om Id 1), grey 0x808080, abr from Attr (RE2 uses PresetObjectGT4_neo here) */
POLY_GT4 *PresetObjectGT4_mizu(MD_WORK2 *pW, POLY_GT4 *pPrim)
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

/* sort the water surface and show/hide each cell by Hit_ck_point4 against the cut, then the ripple split (RE2 MIZU_SYS.C Mizu_trans) */
void Mizu_trans(ENEMY_WORK *pEm, ULONG Abr)
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
    Prm.pOt = pEm->pUnk7C;
    Prm.u.Rgb = pMd->Poly_rgb;
    Md_rtp(Sxy, Sz, pMd->pObj);
    Prm.u.c.Code = (Abr << 1) | 0x3C;
    SortGT4_lno(&Prm, (POLY_GT4 *)pEm->pUnk188_seq_t_ptr + G.Cbuf_id, Sxy, Sz);
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
    pPrim = (POLY_GT4 *)pEm->pUnk188_seq_t_ptr;
    pPrim += G.Cbuf_id;
    pBits = &Mizu_struct_tbl[Mizu_tbl[pEm->Type]];
    W = Mizu_size_tbl[pEm->Type][0];
    H = Mizu_size_tbl[pEm->Type][1];
    switch (Dir) {
    case 0:
    case 2:
        for (j = 0; j < H; j++) {
            for (i = 0; i < W; i++) {
                if (*pBits & (0x8000 >> i)) {
                    if (Hit_ck_point4(&Pos.vx, G.Cccut) != 0) {
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
                    if (Hit_ck_point4(&Pos.vx, G.Cccut) != 0) {
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
    Draw_list_all(pEm, W, H);
}

void Draw_list_all(ENEMY_WORK *pFloor, ULONG W, ULONG H)
{
    ENEMY_WORK **ppEm;

    Mizu_div_main(pFloor, W, H, G.pEnemy[0]);
    if (G.pEnemy[1]->Be_flg & 1) {
        Mizu_div_main(pFloor, W, H, G.pEnemy[1]);
    }
    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        if ((*ppEm)->Be_flg & 1) {
            Mizu_div_main(pFloor, W, H, *ppEm);
        }
    }
}

/* replace the water cell under one model by 13 subdivided GT4s in Sce.pMizu_div (RE2 MIZU_SYS.C Mizu_div_main) */
void Mizu_div_main(ENEMY_WORK *pFloor, ULONG W, ULONG H, ENEMY_WORK *pEm)
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
    if (Hit_ck_box(&Pos.vx, Area) == 0) {
        return;
    }
    Pz = Pos.vz / 0x708;
    Px = Pos.vx / 0x708;
    k = 0;
    Cnt = 0;
    pBits = &Mizu_struct_tbl[Mizu_tbl[pFloor->Type]];
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
    pPrim = (POLY_GT4 *)pFloor->pUnk188_seq_t_ptr;
    pPrim += pG->Cbuf_id;
    pPrim += Cnt * 2;
    Tag = pPrim->tag;
    V0 = pPrim->v0;
    if (!(Tag & 0x0C000000)) {
        return;
    }
    if (Sce.Mizu_div_ctr >= Sce.Mizu_div_max) {
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
    pPrim += Sce.Mizu_div_ctr * 26;
    Sce.Mizu_div_ctr++;
    M = GsIDMATRIX;
    RotMatrix(CDIR_SV(EM_MD(pFloor)), &Lm);
    Lm.t[0] = EM_MD(pFloor)->Workm.t[0];
    Lm.t[1] = EM_MD(pFloor)->Workm.t[1];
    Lm.t[2] = EM_MD(pFloor)->Workm.t[2];
    CompM(&GsWSMATRIX, &Lm, &M);
    SetRotMatrix(&M);
    SetTransMatrix(&M);
    pOt = G.Ot1[G.Cbuf_id];
    pTbl = Div_prim_coord[0];
    j = 0;
    do {
        pPrim->u0 = Div_prim_coord[j][0] * 63 / 18;
        pPrim->v0 = V0 + Div_prim_coord[j][1] * 63 / 18;
        *(ULONG *)&pPrim->r1 = 0x808080;
        pPrim->u1 = Div_prim_coord[j][2] * 63 / 18;
        pPrim->v1 = V0 + Div_prim_coord[j][3] * 63 / 18;
        *(ULONG *)&pPrim->r2 = 0x808080;
        pPrim->u2 = Div_prim_coord[j][4] * 63 / 18;
        pPrim->v2 = V0 + Div_prim_coord[j][5] * 63 / 18;
        *(ULONG *)&pPrim->r3 = 0x808080;
        pPrim->u3 = Div_prim_coord[j][6] * 63 / 18;
        pPrim->v3 = V0 + Div_prim_coord[j][7] * 63 / 18;
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
