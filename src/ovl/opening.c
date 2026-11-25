/*
 * OPENING.BIN: the opening narration: picture scenes (scroll, cross-fade) driven by a small byte script with XA
 * voice
 */
#include "ovl/opening.h"
#include "mojidisp.h"
#include "status.h"
#include "sub.h"


/* task entry: run the script until it ends or start/select is pressed */
void Opening_main(void)
{
    OPN_WORK *pWk = &Opn_work;
    LONG Ret;

    Opening_init();
    Fade_set(0, 1, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    while (1) {
        Ret = Opening_op_tbl[*pWk->pScript](pWk);
        if (Ret == 1) {
            continue;
        }
        if (Ret == 2) {
            break;
        }
        if (*(UCHAR *)&Main.Bg_mode != 2) {
            switch (pWk->Scene) {
            case 0:
                Opening_scene0_move(pWk);
                Opening_scene0_draw(pWk);
                break;
            case 1:
                Opening_scene1_move(pWk);
                Opening_scene1_draw(pWk);
                break;
            }
        }
        if (*(ULONG *)&G.Padd_trg & 0x900) {
            break;
        }
        Task_sleep(1);
    }
    Opening_exit();
}

/* suspends the game task, sets the script pointer, loads the pictures and sets up the sprites */
void Opening_init(void)
{
    G.Next_cut_no = G.Cut_no;
    Task_suspend(0);
    Main.Vloop = 2;
    Bg_set_mode(1, 0);
    Main.Unk191 = 1;
    Opn_work.pScript = (UCHAR *)0x8014B02A;
    Opn_work.Rno3 = 3;
    Opn_work.Rno2 = 0;
    Opening_load();
    Opening_sprt_init();
}

/* load the opening pictures */
void Opening_load(void)
{
    ULONG i;

    Cd_read(0x3D, (ULONG *)0x80100000, 0, "OPENING0_DAT");
    i = 0;
    G.Ctp.Tpage_clut = 5;
    do {
        Mapping_tim(Opn_tim_tbl[i]);
        i++;
    } while (i < 6);
    G.Ctp.Tpage_clut = (i << 8) + 0x15;
    do {
        Mapping_tim(Opn_tim_tbl[i]);
        i++;
    } while (i < 9);
    Cd_read(0x3E, (ULONG *)0x80100000, 0, "OPENING1_DAT");
}

/* sets up the 10 picture sprites and tpages of both buffers */
void Opening_sprt_init(void)
{
    SPRT *pSp;
    DR_TPAGE *pTp;
    ULONG i, j;

    pSp = Opn_work.Sprt[0];
    pTp = Opn_work.Tpage[0];
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 10; i++) {
            SetSprt(pSp);
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->x0 = Opn_spr_pos[i][0];
            pSp->y0 = Opn_spr_pos[i][1];
            pSp->w = Opn_spr[i].W;
            pSp->h = Opn_spr[i].H;
            pSp->u0 = Opn_spr[i].U;
            pSp->v0 = Opn_spr[i].V;
            pSp->clut = GetClut(0, Opn_spr[i].Clut + 0x1E0);
            SetDrawTPage(pTp, 0, 0, GetTPage(1, 1, (Opn_spr[i].Tpage & 0xF) << 6, (Opn_spr[i].Tpage >> 4) << 8));
            pSp++;
            pTp++;
        }
    }
}

/* white fade, stops the XA voice, closes the message and resumes the game task */
void Opening_exit(void)
{
    Main.Vloop = 1;
    Fade_set(0, 1, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Task_sleep(1);
    if (CD.Xa_rtn_no) {
        CD.Xa_rtn_no = 3;
    }
    Moji.Mess_flg = 0;
    Task_sleep(10);
    Task_signal(0);
    Task_exit();
}

/* scene 0: scroll up */
void Opening_scene0_move(OPN_WORK *pWk)
{
    switch (pWk->Rno0) {
    case 0:
        G.Status_flg |= STAT_CINESCO;
        pWk->Cnt = 0;
        pWk->Rno0++;
        break;
    case 1:
        pWk->Ofs0 = 0x18 - (pWk->Cnt - 0x1E) * 0x120 / 0x208;
        pWk->Cnt++;
        if (pWk->Ofs0 < -0x107) {
            pWk->Rno0++;
        }
        break;
    case 2:
        break;
    }
}

/* scene 0: draws the four scrolling picture strips */
void Opening_scene0_draw(OPN_WORK *pWk)
{
    SPRT *pSp;
    DR_TPAGE *pTp;
    ULONG i;

    pSp = pWk->Sprt[G.Cbuf_id];
    pTp = pWk->Tpage[G.Cbuf_id];
    for (i = 0; i < 4; i++) {
        pSp->y0 = pWk->Ofs0 + Opn_spr_pos[i][1];
        AddPrim(G.pOt0 + 5, pSp);
        AddPrim(G.pOt0 + 5, pTp);
        pSp++;
        pTp++;
    }
}

/* scene 1: two layers scroll sideways and cross fade */
void Opening_scene1_move(OPN_WORK *pWk)
{
    switch (pWk->Rno0) {
    case 0:
        pWk->Rno0++;
        pWk->Kido = 0x80;
        pWk->Ot0 = 5;
        pWk->Semi = 0;
        pWk->Cnt = 0;
        pWk->Ot1 = 6;
        break;
    case 1:
        pWk->Ofs0 = -(pWk->Cnt * 128) / 660;
        pWk->Ofs1 = -((pWk->Cnt + 1) * 128) / 660;
        pWk->Cnt++;
        break;
    }
    switch (pWk->Rno1) {
    case 0:
        if (pWk->Cnt >= 0xF9) {
            pWk->Rno1 = 1;
            pWk->Semi = 1;
        }
        break;
    case 1:
        pWk->Kido -= 2;
        if (pWk->Cnt >= 0x119) {
            pWk->Ot0 = 6;
            pWk->Ot1 = 5;
            pWk->Semi = 2;
            pWk->Rno1++;
        }
        break;
    case 2:
        pWk->Kido -= 2;
        if (pWk->Cnt >= 0x139) {
            pWk->Semi = 0;
            pWk->Rno1++;
        }
        break;
    }
}

/* scene 1: draws the two sideways-scrolling layers with their cross-fade brightness */
void Opening_scene1_draw(OPN_WORK *pWk)
{
    SPRT *pSp;
    DR_TPAGE *pTp;
    ULONG i;

    pSp = &Opn_work.Sprt[G.Cbuf_id][4];
    pTp = &Opn_work.Tpage[G.Cbuf_id][4];
    for (i = 0; i < 2; i++) {
        pSp->r0 = pWk->Kido;
        pSp->g0 = pWk->Kido;
        pSp->b0 = pWk->Kido;
        if (pWk->Semi == 1) {
            pSp->code |= 2;
        } else {
            pSp->code &= ~2;
        }
        pSp->x0 = Opn_spr_pos[i + 4][0] - 0x80 - pWk->Ofs0;
        AddPrim(G.pOt0 + pWk->Ot0, pSp);
        pSp++;
        AddPrim(G.pOt0 + pWk->Ot0, pTp);
        pTp++;
    }
    for (i = 0; i < 2; i++) {
        pSp->r0 = 0x80 - pWk->Kido;
        pSp->g0 = 0x80 - pWk->Kido;
        pSp->b0 = 0x80 - pWk->Kido;
        if (pWk->Semi == 2) {
            pSp->code |= 2;
        } else {
            pSp->code &= ~2;
        }
        pSp->x0 = Opn_spr_pos[i + 6][0] - 0x80 - pWk->Ofs0;
        AddPrim(G.pOt0 + pWk->Ot1, pSp);
        pSp++;
        AddPrim(G.pOt0 + pWk->Ot1, pTp);
        pTp++;
    }
    pSp->r0 = pWk->Kido;
    pSp->g0 = pWk->Kido;
    pSp->b0 = pWk->Kido;
    if (pWk->Semi == 1) {
        pSp->code |= 2;
    } else {
        pSp->code &= ~2;
    }
    pSp->x0 = pWk->Ofs1 + (Opn_spr_pos[8][0] + 0x80);
    AddPrim(G.pOt0 + pWk->Ot0 - 2, pSp);
    pSp++;
    AddPrim(G.pOt0 + pWk->Ot0 - 2, pTp);
    pTp++;
    pSp->r0 = 0x80 - pWk->Kido;
    pSp->g0 = 0x80 - pWk->Kido;
    pSp->b0 = 0x80 - pWk->Kido;
    if (pWk->Semi == 2) {
        pSp->code |= 2;
    } else {
        pSp->code &= ~2;
    }
    pSp->x0 = pWk->Ofs1 + (Opn_spr_pos[9][0] + 0x80);
    AddPrim(G.pOt0 + pWk->Ot1 - 2, pSp);
    AddPrim(G.pOt0 + pWk->Ot1 - 2, pTp);
}

/* script ops */
LONG Opening_op_next(OPN_WORK *pWk)
{
    pWk->pScript++;
    return 1;
}

/* script op: end of this frame's commands (returns 0) */
LONG Opening_op_frame(OPN_WORK *pWk)
{
    pWk->pScript++;
    return 0;
}

/* script op: end of the script (returns 2) */
LONG Opening_op_end(OPN_WORK *pWk)
{
    pWk->pScript++;
    return 2;
}

/* set the wait counter (halfword at +2), then fall into op 4 */
LONG Opening_op_wait_set(OPN_WORK *pWk)
{
    if (Main.Vloop == 1) {
        pWk->Wait = *(SHORT *)(pWk->pScript + 2) * 2;
    } else {
        pWk->Wait = *(USHORT *)(pWk->pScript + 2);
    }
    pWk->pScript++;
    return 1;
}

/* wait */
LONG Opening_op_wait(OPN_WORK *pWk)
{
    if (pWk->Wait != 0) {
        pWk->Wait--;
        return 0;
    }
    pWk->pScript += 3;
    return 1;
}

/* wait for the XA voice */
LONG Opening_op_xa_wait(OPN_WORK *pWk)
{
    if ((G.Status_flg & STAT_XA_VOICE) || (Moji.Mess_flg & 0x80)) {
        return 0;
    }
    pWk->pScript++;
    return 1;
}

/* show picture */
LONG Opening_op_pic(OPN_WORK *pWk)
{
    Memcpy((void *)0x8019C000, Opn_pic_tbl[pWk->pScript[1]], 0x25814);
    Bg_set_mode(2, 0);
    pWk->pScript += 2;
    return 1;
}

/* start scene */
LONG Opening_op_scene(OPN_WORK *pWk)
{
    UCHAR Scene;

    Bg_set_mode(1, 0);
    Scene = pWk->pScript[1];
    pWk->Rno0 = 0;
    pWk->Rno1 = 0;
    pWk->pScript += 2;
    pWk->Scene = Scene;
    return 1;
}

/* fade in */
LONG Opening_op_fade_in(OPN_WORK *pWk)
{
    ULONG Cnt = pWk->pScript[1];

    if (Main.Vloop == 1) {
        Cnt <<= 1;
    }
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, Cnt);
    pWk->pScript += 2;
    return 1;
}

/* fade out */
LONG Opening_op_fade_out(OPN_WORK *pWk)
{
    ULONG Cnt = pWk->pScript[1];

    if (Main.Vloop == 1) {
        Cnt <<= 1;
    }
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, Cnt);
    pWk->pScript += 2;
    return 1;
}

/* XA voice */
LONG Opening_op_xa(OPN_WORK *pWk)
{
    Xa_play(3, pWk->pScript[1] + 0x13);
    G.Status_flg |= STAT_XA_VOICE;
    pWk->pScript += 2;
    return 1;
}

/* message */
LONG Opening_op_mess(OPN_WORK *pWk)
{
    Mess_set(0xB90022, 0x3000, (ULONG)&Opn_mess[Opn_mess_ofs[pWk->pScript[1]]], 0);
    pWk->pScript += 2;
    return 1;
}

/* frame rate */
LONG Opening_op_vloop(OPN_WORK *pWk)
{
    Main.Vloop = pWk->pScript[1];
    pWk->pScript += 2;
    return 1;
}
