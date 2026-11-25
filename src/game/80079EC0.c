/*
 * 80079EC0.c: PC-host STR movie streaming (Pcstr_*, PCopen/PCread) and the EMD skin vertex-blending sort
 * routines (Skin_*)
 */
#include "sub.h"
#include "pcstr.h"

/* ---- merged helpers (forks + parent) ---- */
typedef struct { UCHAR b[0xA0]; } FA_A0[2][2];

/* sets the two frame buffers (size in 1 KB units) */
void Pcstr_set_buf(ULONG *pBuf, LONG size)
{
    Pcstr.pBuf[0] = pBuf;
    Pcstr.pBuf[1] = pBuf + (size << 8);
}

/* opens an STR file on the PC host, detects raw interleaved sectors and reads up to the start frame; -1 on error */
LONG Pcstr_open(char *pName)
{
    ULONG Sub0;
    ULONG Sub1;

    if ((Pcstr.Fd = PCopen(pName, 0, 0)) < 0)
        return -1;
    Pcstr.Interleave = 0;
    PCread(Pcstr.Fd, (char *)Pcstr.Sub_head, 8);
    Pcstr.Sub_head[0] = Sub0 = Pcstr.Sub_head[0] & 0xFF0000;
    Pcstr.Sub_head[1] = Sub1 = Pcstr.Sub_head[1] & 0xFF0000;
    /* sector subheaders of a raw (interleaved) STR: video or data submode in both copies */
    if ((Sub0 == 0x420000 && Sub1 == 0x420000) || (Sub0 == 0x480000 && Sub1 == 0x480000)
        || (Sub0 == 0x640000 && Sub1 == 0x640000))
        Pcstr.Interleave = 1;
    PClseek(Pcstr.Fd, 0, 0);
    Pcstr.Head[Pcstr.Buf_no].Frame_count = 0;
    do {
        Pcstr_read_frame(Pcstr.pBuf[Pcstr.Buf_no]);
    } while (Pcstr.Head[Pcstr.Buf_no].Frame_count < Pcstr.Start_frame);
    if (Pcstr.Start_func != NULL)
        Pcstr.Start_func();
    return 0;
}

/* sets the start/end frames and the per-frame start/end callbacks of the PC stream */
void Pcstr_set(LONG unused, ULONG start, ULONG end, void (*start_func)(void), void (*end_func)(void))
{
    Pcstr.End_flg = 0;
    Pcstr.Buf_no = 0;
    Pcstr.End_frame = end;
    Pcstr.Start_func = start_func;
    Pcstr.Head[1].Frame_count = 0;
    Pcstr.Head[0].Frame_count = 0;
    Pcstr.Start_frame = start;
    Pcstr.End_func = end_func;
}

/* closes the PC stream file */
void Pcstr_close(void)
{
    PCclose(Pcstr.Fd);
}

/* reads the next frame into the other buffer and calls the callbacks (end callback past the end frame) */
LONG Pcstr_next(void)
{
    PCSTR_WORK *p = &Pcstr;

    p->Buf_no ^= 1;
    Pcstr_read_frame(p->pBuf[p->Buf_no]);
    if (p->Start_func != NULL) {
        p->Start_func();
    }
    if ((p->Head[p->Buf_no].Frame_count >= p->End_frame || p->End_flg != 0) && p->End_func != NULL) {
        p->End_func();
    }
    return 0;
}

/* returns the current frame buffer and its STR header */
LONG Pcstr_get(ULONG **ppBuf, PCSTR_HEADER **ppHead)
{
    *ppBuf = Pcstr.pBuf[Pcstr.Buf_no];
    *ppHead = &Pcstr.Head[Pcstr.Buf_no];
    return 0;
}

/* reads the sectors of one frame (skipping raw subheaders/EDC), sets End_flg at the end */
void Pcstr_read_frame(ULONG *pBuf)
{

    if (Pcstr.End_flg != 0) {
        return;
    }
    do {
        if (Pcstr.Interleave != 0) {
            while (1) {
                if (PCread(Pcstr.Fd, (char *)Pcstr.Sub_head, 8) == 0) {
                    Pcstr.End_flg = 1;
                    return;
                }
                Pcstr.Sub_head[0] &= 0xFF0000;
                Pcstr.Sub_head[1] &= 0xFF0000;
                if (Pcstr.Sub_head[0] == 0x420000 || Pcstr.Sub_head[0] == 0x480000) {
                    break;
                }
                PClseek(Pcstr.Fd, 0x918, 1);
            }
        }
        if (PCread(Pcstr.Fd, (char *)(Pcstr.Head + Pcstr.Buf_no), 0x14) == 0) {
            Pcstr.End_flg = 1;
            return;
        }
        PClseek(Pcstr.Fd, 0xC, 1);
        PCread(Pcstr.Fd, (char *)pBuf, 0x7E0);
        pBuf += 0x7E0 / 4;
        if (Pcstr.Interleave != 0) {
            PClseek(Pcstr.Fd, 0x118, 1);
        }
    } while (Pcstr.Head[Pcstr.Buf_no].Sec_count != Pcstr.Head[Pcstr.Buf_no].Sec_num - 1);
    if (Pcstr.Head[Pcstr.Buf_no].Frame_count >= Pcstr.End_frame) {
        Pcstr.End_flg = 1;
    }
}

/* links the skin section of an EMD (bit mask of parts, then per part a record pair) to the parts
 * (PARTS_W.pUnkA8) */
void Skin_set(ENEMY_WORK *pEm, ULONG *pD)
{
    ULONG *pA = pD + 2;
    LONG i = 0;
    ULONG f;
    ULONG n;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    pEm->pUnk11C = (ULONG *)((UCHAR *)pD + ((USHORT *)pD)[2]);
    f = *pD;
    n = ((USHORT *)pD)[3];
loop:
    do {
        if (f & 1) {
            pP[i].pUnkA8 = pA;
            pA += 2;
            pP[i].UnkA2 |= 0x10;
        }
        f >>= 1;
        i++;
    } while (f != 0);
    if (--n != 0) {
        goto loop;
    }
}

/* joint offset of a model part (first EMD section, ENEMY_WORK pUnk11C) */
typedef struct tagJOINT_OFS {
    SHORT x;
    SHORT y;
    SHORT z;
} JOINT_OFS;

/* sorts the skin polygons of part pP (vertices blended between two parts' matrices, then lit) */
void Skin_sort(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP)
{
    UCHAR *pHead;
    ULONG Ofs;
    UCHAR *pData;
    SORT_ENV *pEnv = &Skin_env;

    pHead = (UCHAR *)pP->pUnkA8;
    Ofs = ((ULONG *)pHead)[0];
    pEnv->pEm = G.pEm;
    pEnv->pMdl = (SORT_MDL *)Arg2;
    pEnv->pWs = &GsWSMATRIX;
    pEnv->pM = D_800C79F0;
    pEnv->pV = D_800C79F4;
    pData = (UCHAR *)((ULONG)pHead + (Ofs & 0xFFFF));
    Skin_vtx0(pWork0, pWork1, pEnv, pData);
    Skin_light0(Skin_rgb, pP, pEnv, pData);
    if ((Ofs >>= 16) != 0) {
        Skin_vtx1(pWork0, pWork1, pEnv, pHead + Ofs);
        Skin_light1(Skin_rgb, pP, pEnv, pHead + Ofs);
    }
    if ((Ofs = ((ULONG *)pHead)[1]) != 0) {
        Skin_vtx2(pWork0, pWork1, pEnv, pHead + Ofs);
        Skin_light2(Skin_rgb, pP, pEnv, pHead + Ofs);
    }
}

/* Skin_sort for parts with the alternate (Flg 0x400) routines */
void Skin_sort_alt(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP)
{
    SORT_ENV *pEnv = &Skin_env;
    UCHAR *pHead;
    ULONG Ofs;

    pSkin_head = pHead = (UCHAR *)pP->pUnkA8;
    Ofs = ((ULONG *)pHead)[0];
    pEnv->pEm = G.pEm;
    pEnv->pMdl = (SORT_MDL *)Arg2;
    pEnv->pWs = &GsWSMATRIX;
    pEnv->pM = D_800C79F0;
    pEnv->pV = D_800C79F4;
    Skin_ofs = Ofs;
    Skin_vtx0_alt(pWork0, pWork1, pEnv, (UCHAR *)((ULONG)pHead + (Ofs & 0xFFFF)));
    Skin_light0(Skin_rgb, pP, pEnv, pSkin_head + (USHORT)Skin_ofs);
    if ((Skin_ofs >>= 16) != 0) {
        Skin_vtx1_alt(pWork0, pWork1, pEnv, pSkin_head + Skin_ofs);
        Skin_light1(Skin_rgb, pP, pEnv, pSkin_head + Skin_ofs);
    }
    if ((Skin_ofs = ((ULONG *)pSkin_head)[1]) != 0) {
        Skin_vtx2_alt(pWork0, pWork1, pEnv, pSkin_head + Skin_ofs);
        Skin_light2(Skin_rgb, pP, pEnv, pSkin_head + Skin_ofs);
    }
}

/* GTE inline macros (PsyQ inline_c.h with the DMPSX-converted opcodes) */
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetLightMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $8; ctc2 $13, $9; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $10; ctc2 $13, $11; ctc2 $14, $12" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stclmv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_rtir() __asm__ volatile("nop; nop; .word 0x4A49E012")
#define gte_ldv3(r0, r1, r2) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ncct_b() __asm__ volatile(".word 0x4B18043F")
#define gte_rtpt_b() __asm__ volatile(".word 0x4A280030")
#define gte_strgb3(r0, r1, r2) __asm__ volatile("swc2 $20, 0(%0); swc2 $21, 0(%1); swc2 $22, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile("swc2 $12, 0(%0); swc2 $13, 0(%1); swc2 $14, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsz3(r0, r1, r2) __asm__ volatile("swc2 $17, 0(%0); swc2 $18, 0(%1); swc2 $19, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_ldlv0(r0) __asm__ volatile("lhu $13, 4(%0); lhu $12, 0(%0); sll $13, $13, 16; or $12, $12, $13; mtc2 $12, $0; lwc2 $1, 8(%0)" : : "r"(r0) : "$12", "$13")
#define gte_stlvnl(r0) __asm__ volatile("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(r0) : "memory")
#define gte_rtv0tr() __asm__ volatile("nop; nop; .word 0x4A480012")
#define gte_rtv1tr() __asm__ volatile("nop; nop; .word 0x4A488012")
#define gte_rtv2tr() __asm__ volatile("nop; nop; .word 0x4A490012")
#define gte_rtpt() __asm__ volatile("nop; nop; .word 0x4A280030")
/* average of two 0x00BBGGRR colours */
#define RGB_AVG(a, b) (((((a) & 0xFF00) + ((b) & 0xFF00)) >> 1 & 0xFF00) | ((((a) & 0xFF00FF) + ((b) & 0xFF00FF)) >> 1 & 0xFF00FF))
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_ldv3c(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 8(%0); lwc2 $3, 12(%0); lwc2 $4, 16(%0); lwc2 $5, 20(%0)" : : "r"(r0))
#define gte_rtv0() __asm__ volatile("nop; nop; .word 0x4A486012")
#define gte_rtv1() __asm__ volatile("nop; nop; .word 0x4A48E012")
#define gte_rtv2() __asm__ volatile("nop; nop; .word 0x4A496012")
#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stsv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 2(%0); sh $14, 4(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_gpl12() __asm__ volatile("nop; nop; .word 0x4BA8003E")

/* parameters copied into t-regs at entry (`addu t2,a0,zero` / `addu t3,a1,zero`): not proof of hand editing, Skin_sort_gt3 has it too (NOTES agentF) */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", Skin_vtx0);

/* blended vertices of the second skin list */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", Skin_vtx1);

/* parameters copied into t-regs at entry (`addu t3,a3,zero`): not proof of hand editing, Skin_sort_gt3 has it too (NOTES agentF) */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", Skin_vtx2);

/* parameters copied into t-regs at entry (`addu t8,a0,zero`): not proof of hand editing, Skin_sort_gt3 has it too (NOTES agentF) */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", Skin_light0);

/* lit colours of the second skin list */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", Skin_light1);

/* lit colours of the third skin list: normals rotated by both parts, blended by weight */
void Skin_light2(ULONG *pRgb, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData)
{
    PARTS_W *pParts;
    SVECTOR *pNor;
    SVECTOR *pN;
    ULONG i0;
    ULONG i1;
    ULONG i2;
    LONG w0;
    LONG w1;
    LONG w2;
    ULONG *p0;
    ULONG *p1;
    ULONG *p2;

    pN = Skin_nor;
    pParts = pEnv->pEm->pSin_parts_ptr;
    pNor = pEnv->pMdl->pNor;
    gte_SetLightMatrix(&LIGHTMATRIX);
    i0 = pData[2];
    i1 = pData[4];
    w0 = pData[3] * 32;
    i2 = pData[6];
    w1 = pData[5] * 32;
    w2 = pData[7] * 32;
    do {
        gte_SetRotMatrix(&pParts[pData[0]].Workm);
        gte_ldv3(&pNor[i0], &pNor[i1], &pNor[i2]);
        gte_rtv0();
        gte_stsv(&pN[0]);
        gte_rtv1();
        gte_stsv(&pN[1]);
        gte_rtv2();
        gte_stsv(&pN[2]);
        gte_SetRotMatrix(&pParts[pData[1]].Workm);
        gte_ldv0(&pNor[i0]);
        gte_rtv0();
        gte_stsv(&pN[3]);
        gte_rtv1();
        gte_stsv(&pN[4]);
        gte_rtv2();
        gte_stsv(&pN[5]);
        gte_lddp(0x1000 - w0);
        gte_ldsv(&pN[0]);
        gte_gpf12();
        gte_lddp(w0);
        gte_ldsv(&pN[3]);
        gte_gpl12();
        gte_stsv(&pN[0]);
        gte_lddp(0x1000 - w1);
        gte_ldsv(&pN[1]);
        gte_gpf12();
        gte_lddp(w1);
        gte_ldsv(&pN[4]);
        gte_gpl12();
        gte_stsv(&pN[1]);
        gte_lddp(0x1000 - w2);
        gte_ldsv(&pN[2]);
        gte_gpf12();
        gte_lddp(w2);
        gte_ldsv(&pN[5]);
        gte_gpl12();
        gte_stsv(&pN[2]);
        gte_ldv3c(pN);
        pData += 8;
        gte_ncct_b();
        p0 = &pRgb[i0];
        p1 = &pRgb[i1];
        p2 = &pRgb[i2];
        i0 = pData[2];
        i1 = pData[4];
        w0 = pData[3] * 32;
        i2 = pData[6];
        w1 = pData[5] * 32;
        w2 = pData[7] * 32;
        gte_strgb3(p0, p1, p2);
    } while (*pData != 0xFF);
}

#define gte_ldsxy3(r0, r1, r2) __asm__ volatile("mtc2 %0, $12; mtc2 %2, $14; mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldsz3(r0, r1, r2) __asm__ volatile("mtc2 %0, $17; mtc2 %1, $18; mtc2 %2, $19" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_nclip_b() __asm__ volatile(".word 0x4B400006")
#define gte_avsz3() __asm__ volatile("nop; nop; .word 0x4B58002D")
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile("swc2 $12, 8(%0); swc2 $13, 20(%0); swc2 $14, 32(%0)" : : "r"(r0) : "memory")

/* textured gouraud triangle of a sort list: the uv words of POLY_GT3, vertex numbers in the top byte of
 * W[1] and the top two bytes of W[2] */
typedef struct tagSORT_GT3 {
    LONG W[3];
} SORT_GT3;

typedef struct tagSORT_HDR {
    UCHAR pad0[0xC];
    SORT_GT3 *pGt3;                    /* 0x0C */
    struct tagSORT_GT4 *pGt4;           /* 0x10 */
    USHORT N_gt3;                      /* 0x14 */
    USHORT N_gt4;                      /* 0x16 */
} SORT_HDR;

typedef struct tagSORT_PRM {
    SORT_HDR *pHdr;                    /* 0x00 */
    ULONG Unk4;                        /* 0x04 */
    ULONG *pOt;                        /* 0x08 */
    ULONG *pRgb;                       /* 0x0C per-vertex colours */
} SORT_PRM;

/* sort list triangles into GT3 packets: back faces culled (nclip), colours from pPrm->pRgb, uv from the list,
 * packets 0x68 bytes apart; returns the next free packet */
POLY_GT3 *Skin_sort_gt3(SORT_PRM *pPrm, POLY_GT3 *pPrim, ULONG *pSxy, USHORT *pSz)
{
    SORT_GT3 *pGt3;
    ULONG *pOt;
    ULONG *pRgb;
    LONG Z;
    ULONG n;
    LONG v0;
    LONG v1;
    LONG v2;
    USHORT *pZ0;
    USHORT *pZ1;
    USHORT *pZ2;
    ULONG *pO;

    n = pPrm->pHdr->N_gt3;
    if (n == 0) {
        return pPrim;
    }
    pGt3 = pPrm->pHdr->pGt3;
    pOt = pPrm->pOt;
    pRgb = pPrm->pRgb;
loop:
    v0 = pGt3->W[1] >> 24;
    v1 = (pGt3->W[2] & 0xFF0000) >> 16;
    v2 = pGt3->W[2] >> 24;
    gte_ldsxy3(pSxy[v0], pSxy[v1], pSxy[v2]);
    pGt3++;
    n--;
    gte_nclip_b();
    pZ0 = &pSz[v0];
    pZ1 = &pSz[v1];
    pZ2 = &pSz[v2];
    gte_stopz(&Z);
    if (Z > 0) {
        gte_ldsz3(*pZ0, *pZ1, *pZ2);
        gte_avsz3();
        *(ULONG *)&pPrim->r0 = pRgb[v0] | 0x34000000;
        *(ULONG *)&pPrim->r1 = pRgb[v1];
        *(ULONG *)&pPrim->r2 = pRgb[v2];
        *(ULONG *)&pPrim->u0 = pGt3[-1].W[0];
        *(ULONG *)&pPrim->u1 = pGt3[-1].W[1] & 0xFFFFFF;
        *(ULONG *)&pPrim->u2 = pGt3[-1].W[2];
        gte_stotz(&Z);
        gte_stsxy3_gt3(pPrim);
        /* Z is reused for the old OT entry */
        pO = &pOt[Z >> 3];
        Z = *pO;
        *pO = (ULONG)pPrim & 0xFFFFFF;
        pPrim->tag = Z | 0x09000000;
        pPrim = (POLY_GT3 *)((UCHAR *)pPrim + 0x68);
    }
    if (n != 0) {
        goto loop;
    }
    return pPrim;
}

/* textured gouraud quad of a sort list: the vertex numbers are the top two bytes of W[2] and W[3] */
typedef struct tagSORT_GT4 {
    LONG W[4];
} SORT_GT4;

/* the same for quads (GT4 packets); the OT depth is the sum of the four z values */
POLY_GT4 *Skin_sort_gt4(SORT_PRM *pPrm, POLY_GT4 *pPrim, ULONG *pSxy, SHORT *pSz)
{
    SORT_GT4 *pGt4;
    ULONG *pOt;
    ULONG *pRgb;
    LONG Z;
    ULONG n;
    LONG v0;
    LONG v1;
    LONG v2;
    LONG v3;
    SHORT *pZ0;
    SHORT *pZ1;
    SHORT *pZ2;
    ULONG *pO;

    n = pPrm->pHdr->N_gt4;
    if (n == 0) {
        return pPrim;
    }
    pGt4 = pPrm->pHdr->pGt4;
    pOt = pPrm->pOt;
    pRgb = pPrm->pRgb;
loop:
    v0 = (pGt4->W[2] & 0xFF0000) >> 16;
    v1 = pGt4->W[2] >> 24;
    v2 = (pGt4->W[3] & 0xFF0000) >> 16;
    v3 = pGt4->W[3] >> 24;
    gte_ldsxy3(pSxy[v0], pSxy[v1], pSxy[v2]);
    pGt4++;
    n--;
    gte_nclip_b();
    pZ0 = &pSz[v0];
    pZ1 = &pSz[v1];
    pZ2 = &pSz[v2];
    gte_stopz(&Z);
    if (Z > 0) {
        Z = *pZ0 + *pZ1 + *pZ2 + pSz[v3];
        *(ULONG *)&pPrim->x3 = pSxy[v3];
        *(ULONG *)&pPrim->r0 = pRgb[v0] | 0x3C000000;
        *(ULONG *)&pPrim->r1 = pRgb[v1];
        *(ULONG *)&pPrim->r2 = pRgb[v2];
        *(ULONG *)&pPrim->r3 = pRgb[v3];
        *(ULONG *)&pPrim->u0 = pGt4[-1].W[0];
        *(ULONG *)&pPrim->u1 = pGt4[-1].W[1];
        *(ULONG *)&pPrim->u2 = pGt4[-1].W[2];
        *(ULONG *)&pPrim->u3 = pGt4[-1].W[3];
        gte_stsxy3_gt3(pPrim); /* x0..x2 are at the same offsets in POLY_GT4 */
        pO = &pOt[Z >> 7];
        Z = *pO;
        *pO = (ULONG)pPrim & 0xFFFFFF;
        pPrim->tag = Z | 0x0C000000;
        pPrim = (POLY_GT4 *)((UCHAR *)pPrim + 0x68);
    }
    if (n != 0) {
        goto loop;
    }
    return pPrim;
}

/* Skin_vtx0 for the alternate routines */
void Skin_vtx0_alt(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData)
{
    SVECTOR W[3];
    LONG Sz[3];
    MATRIX *pM;
    MATRIX *pWs;
    VECTOR *pV;
    ENEMY_WORK *pEm;
    JOINT_OFS *pOfs;
    SVECTOR *pVtx;
    LONG n;
    ULONG i0;
    ULONG i1;
    ULONG i2;
    SHORT *pZ0;
    SHORT *pZ1;
    SHORT *pZ2;

    pM = pEnv->pM;
    pWs = pEnv->pWs;
    pV = pEnv->pV;
    pEm = pEnv->pEm;
    pOfs = (JOINT_OFS *)pEm->pUnk11C;
    pVtx = pEnv->pMdl->pVtx;
    Sz[0] = 0;
    Sz[1] = 0;
    Sz[2] = 0;
    do {
        n = *pData;
        *pM = pEm->pSin_parts_ptr[n].Workm;
        gte_SetRotMatrix(pM);
        gte_SetTransMatrix(pM);
            pV->vx = pOfs[n].x;
        pV->vy = pOfs[n].y;
        pV->vz = pOfs[n].z;
        gte_ldlv0(pV);
        gte_rtv0tr();
        n = pData[1];
        pData += 2;
        gte_stlvnl(pM->t);
    loop:
        gte_SetRotMatrix(pM);
        gte_SetTransMatrix(pM);
        i0 = pData[0];
        i1 = pData[1];
        i2 = pData[2];
        gte_ldv3(&pVtx[i0], &pVtx[i1], &pVtx[i2]);
        pData += 3;
        n--;
        gte_rtv0tr();
        gte_stsv(&W[0]);
        gte_rtv1tr();
        gte_stsv(&W[1]);
        gte_rtv2tr();
        gte_stsv(&W[2]);
        if (W[0].vy > 0) {
            W[0].vy = 0;
        }
        if (W[1].vy > 0) {
            W[1].vy = 0;
        }
        if (W[2].vy > 0) {
            W[2].vy = 0;
        }
        gte_ldv3c(W);
        gte_SetRotMatrix(pWs);
        gte_SetTransMatrix(pWs);
        gte_rtpt();
        if (Sz[0] > 0x7FFF) {
            *pZ0 = 0x7FFF;
        }
        if (Sz[1] > 0x7FFF) {
            *pZ1 = 0x7FFF;
        }
        if (Sz[2] > 0x7FFF) {
            *pZ2 = 0x7FFF;
        }
        pZ0 = &pSz[i0];
        pZ1 = &pSz[i1];
        pZ2 = &pSz[i2];
        gte_stsxy3(&pSxy[i0], &pSxy[i1], &pSxy[i2]);
        gte_stsz3(&Sz[0], &Sz[1], &Sz[2]);
        *pZ0 = Sz[0];
        *pZ1 = Sz[1];
        *pZ2 = Sz[2];
        if (n != 0)
            goto loop;
    } while (*pData != 0xFF);
    if (Sz[0] > 0x7FFF) {
        *pZ0 = 0x7FFF;
    }
    if (Sz[1] > 0x7FFF) {
        *pZ1 = 0x7FFF;
    }
    if (Sz[2] > 0x7FFF) {
        *pZ2 = 0x7FFF;
    }
}

/* Skin_vtx1 for the alternate routines */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", Skin_vtx1_alt);

/* parameters copied into t-regs at entry (`addu t3,a3,zero`): not proof of hand editing, Skin_sort_gt3 has it too (NOTES agentF) */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", Skin_vtx2_alt);
