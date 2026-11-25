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
        pOt = func_80037D50(workm.t);
        D2 = 0x2C;
        if (Be_flg & 0x1000) {
            D2 = 0x2E;
        }
        D0 = pAseq->Unk3;
        D1 = pAseq->Unk0;
        if (Be_flg & 0x200) {
            workm.t[0] = pEff->Unk38[0] + pEff->Unk30;
            workm.t[1] = pEff->Unk38[1] + pEff->Unk32;
            workm.t[2] = pEff->Unk38[2] + pEff->Unk34;
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

INCLUDE_ASM("asm/nonmatchings/game/trans", Esp_trans_main);

INCLUDE_ASM("asm/nonmatchings/game/trans", Esp_trans_3d);

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
