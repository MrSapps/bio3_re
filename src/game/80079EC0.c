#include "sub.h"
#include "pcstr.h"
#include "snd.h"
#include "em_sub.h"
#include "cd.h"
#include "status.h"
#include "spl_sce.h"

#define FLD(p, ofs, type) (*(type *)((UCHAR *)(p) + (ofs)))  /* unnamed struct fields */

/* ---- merged helpers (forks + parent) ---- */
typedef struct { UCHAR b[0xA0]; } FA_A0[2][2];

void Pcstr_set_buf(ULONG *pBuf, LONG size)
{
    Pcstr.pBuf[0] = pBuf;
    Pcstr.pBuf[1] = pBuf + (size << 8);
}

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

void Pcstr_close(void)
{
    PCclose(Pcstr.Fd);
}

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

LONG Pcstr_get(ULONG **ppBuf, PCSTR_HEADER **ppHead)
{
    *ppBuf = Pcstr.pBuf[Pcstr.Buf_no];
    *ppHead = &Pcstr.Head[Pcstr.Buf_no];
    return 0;
}

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

void func_8007A2D4(UCHAR *pW, UCHAR *pD)
{
    ULONG *pA = (ULONG *)(pD + 8);
    LONG i = 0;
    ULONG f;
    ULONG n;
    PARTS_W *pP;

    pP = FLD(pW, 0x108, PARTS_W *);
    FLD(pW, 0x11C, UCHAR *) = pD + *(USHORT *)(pD + 4);
    f = *(ULONG *)pD;
    n = *(USHORT *)(pD + 6);
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

void func_8007A350(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP)
{
    UCHAR *pHead;
    ULONG Ofs;
    UCHAR *pData;
    SORT_ENV *pEnv = &D_800E15A0;

    pHead = (UCHAR *)pP->pUnkA8;
    Ofs = ((ULONG *)pHead)[0];
    pEnv->pEm = G.pEm;
    pEnv->pMdl = (SORT_MDL *)Arg2;
    pEnv->pWs = &GsWSMATRIX;
    pEnv->pM = D_800C79F0;
    pEnv->pV = D_800C79F4;
    pData = (UCHAR *)((ULONG)pHead + (Ofs & 0xFFFF));
    func_8007A600(pWork0, pWork1, pEnv, pData);
    func_8007B2FC(D_800E1388, pP, pEnv, pData);
    if ((Ofs >>= 16) != 0) {
        func_8007A918(pWork0, pWork1, pEnv, pHead + Ofs);
        func_8007B518(D_800E1388, pP, pEnv, pHead + Ofs);
    }
    if ((Ofs = ((ULONG *)pHead)[1]) != 0) {
        func_8007AC8C(pWork0, pWork1, pEnv, pHead + Ofs);
        func_8007B824(D_800E1388, pP, pEnv, pHead + Ofs);
    }
}

/* func_8007A350 for parts drawn with the alternate (Flg 0x400) routines */
void func_8007A490(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP)
{
    SORT_ENV *pEnv = &D_800E15A0;
    UCHAR *pHead;
    ULONG Ofs;

    D_800E15C8 = pHead = (UCHAR *)pP->pUnkA8;
    Ofs = ((ULONG *)pHead)[0];
    pEnv->pEm = G.pEm;
    pEnv->pMdl = (SORT_MDL *)Arg2;
    pEnv->pWs = &GsWSMATRIX;
    pEnv->pM = D_800C79F0;
    pEnv->pV = D_800C79F4;
    D_800E1598 = Ofs;
    func_8007BFBC(pWork0, pWork1, pEnv, (UCHAR *)((ULONG)pHead + (Ofs & 0xFFFF)));
    func_8007B2FC(D_800E1388, pP, pEnv, D_800E15C8 + (USHORT)D_800E1598);
    if ((D_800E1598 >>= 16) != 0) {
        func_8007C434(pWork0, pWork1, pEnv, D_800E15C8 + D_800E1598);
        func_8007B518(D_800E1388, pP, pEnv, D_800E15C8 + D_800E1598);
    }
    if ((D_800E1598 = ((ULONG *)D_800E15C8)[1]) != 0) {
        func_8007C91C(pWork0, pWork1, pEnv, D_800E15C8 + D_800E1598);
        func_8007B824(D_800E1388, pP, pEnv, D_800E15C8 + D_800E1598);
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

/* hand-edited compiler output (stays asm): parameters copied into t-regs at entry, `addu t2,a0,zero` / `addu t3,a1,zero` */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007A600);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007A918);

/* hand-edited compiler output (stays asm): parameters copied into t-regs at entry, `addu t3,a3,zero` */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007AC8C);

/* hand-edited compiler output (stays asm): parameters copied into t-regs at entry, `addu t8,a0,zero` */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007B2FC);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007B518);

void func_8007B824(ULONG *pRgb, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData)
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

    pN = D_800E1568;
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

/* textured gouraud triangle of a sort list: uv words with the vertex numbers in the top bytes */
typedef union tagSORT_GT3 {
    ULONG W[3];
    struct {
        UCHAR pad0[7];
        signed char V0;                /* 0x07 */
        UCHAR pad8[2];
        UCHAR V1;                      /* 0x0A */
        signed char V2;                /* 0x0B */
    } I;
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

/* hand-edited compiler output (stays asm): parameters copied into t-regs at entry, `addu t0,a1,zero`, t7/t8 from a2/a3 */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007BC7C);

/* textured gouraud quad of a sort list: the vertex numbers are the top two bytes of W[2] and W[3] */
typedef struct tagSORT_GT4 {
    LONG W[4];
} SORT_GT4;

/* hand-edited compiler output (stays asm): parameters copied into t-regs at entry, `addu t0,a1,zero`, t8/t9 from a2/a3 */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007BDF8);

void func_8007BFBC(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData)
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

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007C434);

/* hand-edited compiler output (stays asm): parameters copied into t-regs at entry, `addu t3,a3,zero` */
INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007C91C);
