#include "line_scr.h"

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

void Scr_effect(void)
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
    Scr_quake(Amp);
    line_scr_end();
}

INCLUDE_ASM("asm/nonmatchings/game/line_scr", Scr_quake);

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
