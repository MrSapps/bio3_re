/*
 * TRANS.C (RE2 TRANS.C lineage): builds the POLY_FT4 sprites of the active effects (Esp[]) each frame
 */
#include "trans.h"
#include "pl_wep.h"
#include "sub.h"
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

#define gte_rtps_b() __asm__ volatile(".word 0x4A180001")
#define gte_rtpt_b() __asm__ volatile(".word 0x4A280030")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_stsxy(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile("swc2 $12, 0(%0); swc2 $13, 0(%1); swc2 $14, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsz(r0) __asm__ volatile("swc2 $19, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsz3(r0, r1, r2) __asm__ volatile("swc2 $17, 0(%0); swc2 $18, 0(%1); swc2 $19, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")

/* RE2 Eff_sprt_trans: effect works to POLY_FT4 sprites (flat, or 3D with their own matrix) */
void Eff_sprt_trans(void)
{
    POLY_FT4 *pP;
    POLY_FT4 *pP_limit;
    ESP_WORK *pEff;
    SSEQUENCE *pAseq;
    VCUT *pCcut;
    ULONG nSpr;
    ULONG *pOt;
    LONG Proj;
    LONG D0;
    LONG D1;
    LONG D2;
    ULONG Be_flg;
    ULONG Flg;
    MATRIX m;
    MATRIX workm;
    MATRIX workm2;
    VECTOR Vec;
    MATRIX *pWs;

    Trans_pointer_set();
    pWs = &GsWSMATRIX;
    pEff = &Esp[96];
    pP = Trans.pPoly_ft4_keep;
    D2 = G.Cbuf_id;
    pP_limit = Trans.Prim_poly_ft4[D2] + 384;
    Proj = G.pRoom->Cut[G.Cut_no].ViewR >> 7;
    pCcut = G.Cccut;
    SetRotMatrix(pWs);
    SetTransMatrix(pWs);
loop:
    pEff--;
    D2 = pEff->Pos.vx;
    D0 = pEff->Pos.vy;
    D1 = pEff->Pos.vz;
    workm.t[0] = D2;
    workm.t[1] = D0;
    workm.t[2] = D1;
    Be_flg = pEff->Be_flg;
    pAseq = pEff->pSeq + pEff->Seq_no;
    nSpr = pAseq->nSpr;
    if ((Be_flg & 0xA000) == 0xA000 && Hit_ck_point4(workm.t, pCcut) != 0 && nSpr != 0) {
        pOt = Ot_get_pos(workm.t);
        D2 = 0x2C;
        if (Be_flg & 0x1000) {
            D2 = 0x2E;
        }
        D0 = pAseq->Unk3;
        D1 = pAseq->Unk0;
        if (Be_flg & 0x200) {
            workm.t[0] = pEff->Base[0] + pEff->Mov_x;
            workm.t[1] = pEff->Base[1] + pEff->Mov_y;
            workm.t[2] = pEff->Base[2] + pEff->Mov_z;
            RotMatrix((SVECTOR *)pEff->Rot, &workm);
            CompM(&pEff->Workm, &workm, &workm2);
            CompM(&GsWSMATRIX, &workm2, &m);
            gte_SetRotMatrix(&m);
            gte_SetTransMatrix(&m);
            if (pP + nSpr < pP_limit) {
                Esp_trans_3d(pP, pEff, D0, nSpr, Proj, pOt, D2, D1);
                pP += nSpr;
                Flg = 1;
            } else {
                goto kill;
            }
        } else {
            if (Flg != 0) {
                gte_SetRotMatrix(pWs);
                gte_SetTransMatrix(pWs);
                Flg = 0;
            }
            if (pP_limit <= pP + nSpr) {
            kill:
                pEff->Be_flg = 0;
            } else {
                Esp_trans_main(pP, pEff, D0, nSpr, Proj, pOt, D2, D1);
                pP += nSpr;
            }
        }
    }
    if (pEff != Esp) {
        goto loop;
    }
    Trans.pPoly_ft4_keep = pP;
}

/* RE2 Esp_trans_main: billboard sprites of one effect, projected from its position with the world matrix */
void Esp_trans_main(POLY_FT4 *pP, ESP_WORK *pEff, ULONG Def_size, ULONG nSpr, LONG Proj, ULONG *pOt, ULONG H_clarity, ULONG pG_offset)
{
    SVECTOR Pos;
    LONG D0;
    LONG D1;
    LONG D2;
    SPR_GP *pSpGp;
    ULONG Def_size_s;
    LONG Mag;
    ULONG Clut;
    ULONG Tpage;
    ULONG Xsize;
    ULONG Hot_x;
    ULONG Ysize;
    ULONG Hot_y;
    ULONG Sx;
    ULONG Sy;
    ULONG x1;

    Pos.vx = pEff->Pos.vx;
    Pos.vy = pEff->Pos.vy;
    Pos.vz = pEff->Pos.vz;
    gte_ldv0(&Pos);
    Def_size_s = Def_size << 8;
    D1 = pEff->Size;
    gte_rtps_b();
    Mag = Def_size * D1 * Proj;
    D2 = nSpr;
    pSpGp = pEff->pSprg + pG_offset;
    Clut = pEff->Clut << 16;
    Tpage = pEff->Tpage << 16;
    gte_stsz(&D0);
    gte_stsxy(&D1);
    if ((SHORT)D1 >= 0x3FF) {
        return;
    }
    if ((SHORT)D1 < -0x3FE) {
        return;
    }
    if ((SHORT)(D1 >> 16) >= 0x3FF) {
        return;
    }
    if ((SHORT)(D1 >> 16) < -0x3FE) {
        return;
    }
    D1 = (LONG)pP; /* D0..D2 are the gte scratch words, reused as temps (each assignment is stored) */
    if (D0 >> 9 == 0) {
        return;
    }
    if ((ULONG)D0 > 0x7FFF) {
        D0 = 0x7FFF;
    }
    /* the OT entry pointer lives in Ysize (one register for both in the target) */
    Ysize = (ULONG)&pOt[D0 >> 5];
ot_loop:
    *(ULONG *)D1 = (*(ULONG *)Ysize & 0xFFFFFF) | 0x9000000;
    *(ULONG *)Ysize = D1 & 0xFFFFFF;
    D1 += sizeof(POLY_FT4);
    if (--D2 != 0) {
        goto ot_loop;
    }
    gte_stsxy(&D2);
    Sx = D2 << 16;
    Sy = D2 & 0xFFFF0000;
    D2 = Mag / (D0 << 4);
    Xsize = D2 * pEff->Unk08;
    Hot_x = Xsize / Def_size;
    Ysize = D2 * pEff->Unk0A;
    Def_size_s -= 0x100;
    Hot_y = Ysize / Def_size;
    Def_size--;
loop:
    x1 = pSpGp->Ofsx;
    x1 *= Hot_x;
    D1 = pSpGp->Ofsy;
    D2 = Sx + x1;
    x1 = D2 + Xsize;
    x1 >>= 16;
    D0 = Sy + D1 * Hot_y;
    D2 = (ULONG)D2 >> 16;
    D1 = D0 + Ysize;
    D0 &= 0xFFFF0000;
    D1 &= 0xFFFF0000;
    *(ULONG *)&pP->x0 = D2 | D0;
    *(ULONG *)&pP->x1 = x1 | D0;
    *(ULONG *)&pP->x2 = D2 | D1;
    *(ULONG *)&pP->x3 = x1 | D1;
    D2 = pSpGp->U;
    nSpr--;
    D0 = pSpGp->V << 8;
    x1 = D2 + Def_size;
    D1 = D0 + Def_size_s;
    *(ULONG *)&pP->u0 = D2 | D0 | Clut;
    *(ULONG *)&pP->u1 = x1 | D0 | Tpage;
    *(ULONG *)&pP->u2 = D2 | D1;
    *(ULONG *)&pP->u3 = x1 | D1;
    *(ULONG *)&pP->r0 = *(ULONG *)pEff->Col;
    pSpGp++;
    pP->code = H_clarity;
    pP++;
    if (nSpr != 0) {
        goto loop;
    }
}

/* sprites of an effect with its own rotation (matrix set by the caller), projected in 3D */
void Esp_trans_3d(POLY_FT4 *pP, ESP_WORK *pEff, ULONG Def_size, ULONG nSpr, LONG Proj, ULONG *pOt, ULONG H_clarity, ULONG pG_offset)
{
    SPR_GP *pSpGp;
    LONG D0;
    LONG D1;
    LONG D2;
    ULONG Clut;
    ULONG Tpage;
    ULONG Otz;
    ULONG *pOtz;
    SVECTOR Svec[4];
    LONG Z[4];
    LONG Mag_x;
    LONG Mag_y;
    LONG Px;
    LONG Py;
    LONG X;
    LONG Y;
    LONG X1;
    LONG Y1;

    D2 = pEff->Size * (Proj >> 4);
    Mag_x = (LONG)(D2 * pEff->Unk08) >> 12;
    pSpGp = pEff->pSprg + pG_offset;
    Mag_y = (LONG)(D2 * pEff->Unk0A) >> 12;
    Clut = pEff->Clut << 16;
    D0 = (Def_size * Mag_x) >> 12;
    Tpage = pEff->Tpage << 16;
    D1 = (Def_size * Mag_y) >> 12;
    Svec[0].vz = 0;
    Svec[1].vz = 0;
    Svec[2].vz = 0;
    Svec[3].vz = 0;
    Def_size--;
loop:
    Px = pSpGp->Ofsx * Mag_x;
    Py = pSpGp->Ofsy * Mag_y;
    X = Px >> 12;
    Svec[0].vx = X;
    Y = Py >> 12;
    Svec[0].vy = Y;
    gte_ldv0(&Svec[0]);
    X1 = X + D0;
    Y1 = Y + D1;
    gte_rtps_b();
    Svec[1].vx = X1;
    Svec[1].vy = Y;
    Svec[2].vx = X;
    Svec[2].vy = Y1;
    Svec[3].vx = X1;
    Svec[3].vy = Y1;
    gte_stsxy(&pP->x0);
    gte_stsz(&Z[0]);
    gte_ldv3(&Svec[1], &Svec[2], &Svec[3]);
    /* the uv corners reuse X/Y/X1/Y1 (the target keeps them in the same registers) */
    X = pSpGp->U;
    Y = pSpGp->V << 8;
    X1 = X + Def_size;
    Y1 = Y + (Def_size << 8);
    gte_rtpt_b();
    *(ULONG *)&pP->u0 = X | Y | Clut;
    *(ULONG *)&pP->u1 = X1 | Y | Tpage;
    *(ULONG *)&pP->u2 = X | Y1;
    *(ULONG *)&pP->u3 = X1 | Y1;
    *(ULONG *)&pP->r0 = *(ULONG *)pEff->Col;
    pP->code = H_clarity;
    gte_stsz3(&Z[1], &Z[2], &Z[3]);
    Otz = Z[0] + Z[1] + Z[2] + Z[3];
    Otz >>= 4;
    if (Otz >= 0x2000) {
        Otz = 0x1FFF;
    }
    if (Otz >> 7 == 0) {
        return;
    }
    pOtz = pOt + (Otz >> 3);
    pP->tag = (*pOtz & 0xFFFFFF) | 0x9000000;
    *pOtz = (ULONG)pP & 0xFFFFFF;
    pSpGp++;
    gte_stsxy3(&pP->x1, &pP->x2, &pP->x3);
    pP++;
    if (--nSpr != 0) {
        goto loop;
    }
}

/* RE2 Trans_work_init: sets the tag and colour of every effect POLY_FT4, then Trans_pointer_set */
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

/* RE2 Trans_pointer_set: starts the effect prim cursor at the current buffer's POLY_FT4 pool */
void Trans_pointer_set(void)
{
    Trans.pPoly_ft4_keep = Trans.Prim_poly_ft4[G.Cbuf_id];
}
