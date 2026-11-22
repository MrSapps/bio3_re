#include "common.h"
#include "ovl/opening.h"

/* task entry: run the script until it ends or start/select is pressed */
void func_801C2024(void)
{
    OPN_WORK *pWk = &Opn_work;
    LONG Ret;

    func_801C2160();
    Fade_set(0, 1, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    while (1) {
        Ret = D_801C2F70[*pWk->pScript](pWk);
        if (Ret == 1) {
            continue;
        }
        if (Ret == 2) {
            break;
        }
        if (*(UCHAR *)&Main.Bg_mode != 2) {
            switch (pWk->Scene) {
            case 0:
                func_801C2488(pWk);
                func_801C2538(pWk);
                break;
            case 1:
                func_801C2618(pWk);
                func_801C2788(pWk);
                break;
            }
        }
        if (*(ULONG *)&G.Unk20FC & 0x900) {
            break;
        }
        Task_sleep(1);
    }
    func_801C23F4();
}

void func_801C2160(void)
{
    G.Next_cut_no = G.Cut_no;
    Task_suspend(0);
    Main.Vloop = 2;
    Bg_set_mode(1, 0);
    Main.Unk191 = 1;
    Opn_work.pScript = (UCHAR *)0x8014B02A;
    Opn_work.Rno3 = 3;
    Opn_work.Rno2 = 0;
    func_801C21E4();
    func_801C22A0();
}

/* load the opening pictures */
void func_801C21E4(void)
{
    ULONG i;

    Cd_read(0x3D, (ULONG *)0x80100000, 0, "OPENING0_DAT");
    i = 0;
    G.Ctp.Tpage_clut = 5;
    do {
        Mapping_tim(D_801C2F14[i]);
        i++;
    } while (i < 6);
    G.Ctp.Tpage_clut = (i << 8) + 0x15;
    do {
        Mapping_tim(D_801C2F14[i]);
        i++;
    } while (i < 9);
    Cd_read(0x3E, (ULONG *)0x80100000, 0, "OPENING1_DAT");
}

void func_801C22A0(void)
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
            pSp->x0 = D_801C301C[i][0];
            pSp->y0 = D_801C301C[i][1];
            pSp->w = D_801C2FA4[i].W;
            pSp->h = D_801C2FA4[i].H;
            pSp->u0 = D_801C2FA4[i].U;
            pSp->v0 = D_801C2FA4[i].V;
            pSp->clut = GetClut(0, D_801C2FA4[i].Clut + 0x1E0);
            SetDrawTPage(pTp, 0, 0, GetTPage(1, 1, (D_801C2FA4[i].Tpage & 0xF) << 6, (D_801C2FA4[i].Tpage >> 4) << 8));
            pSp++;
            pTp++;
        }
    }
}

void func_801C23F4(void)
{
    Main.Vloop = 1;
    Fade_set(0, 1, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Task_sleep(1);
    if (CD.Xa_rtn_no) {
        CD.Xa_rtn_no = 3;
    }
    D_800DBB58 = 0;
    Task_sleep(10);
    Task_signal(0);
    Task_exit();
}

/* scene 0: scroll up */
void func_801C2488(OPN_WORK *pWk)
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

void func_801C2538(OPN_WORK *pWk)
{
    SPRT *pSp;
    DR_TPAGE *pTp;
    ULONG i;

    pSp = pWk->Sprt[G.Cbuf_id];
    pTp = pWk->Tpage[G.Cbuf_id];
    for (i = 0; i < 4; i++) {
        pSp->y0 = pWk->Ofs0 + D_801C301C[i][1];
        AddPrim(G.pOt0 + 5, pSp);
        AddPrim(G.pOt0 + 5, pTp);
        pSp++;
        pTp++;
    }
}

/* scene 1: two layers scroll sideways and cross fade */
void func_801C2618(OPN_WORK *pWk)
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

void func_801C2788(OPN_WORK *pWk)
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
        pSp->x0 = D_801C301C[i + 4][0] - 0x80 - pWk->Ofs0;
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
        pSp->x0 = D_801C301C[i + 6][0] - 0x80 - pWk->Ofs0;
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
    pSp->x0 = pWk->Ofs1 + (D_801C301C[8][0] + 0x80);
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
    pSp->x0 = pWk->Ofs1 + (D_801C301C[9][0] + 0x80);
    AddPrim(G.pOt0 + pWk->Ot1 - 2, pSp);
    AddPrim(G.pOt0 + pWk->Ot1 - 2, pTp);
}

/* script ops */
LONG func_801C2B38(OPN_WORK *pWk)
{
    pWk->pScript++;
    return 1;
}

LONG func_801C2B50(OPN_WORK *pWk)
{
    pWk->pScript++;
    return 0;
}

LONG func_801C2B68(OPN_WORK *pWk)
{
    pWk->pScript++;
    return 2;
}

/* set the wait counter (halfword at +2), then fall into op 4 */
LONG func_801C2B80(OPN_WORK *pWk)
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
LONG func_801C2BD4(OPN_WORK *pWk)
{
    if (pWk->Wait != 0) {
        pWk->Wait--;
        return 0;
    }
    pWk->pScript += 3;
    return 1;
}

/* wait for the XA voice */
LONG func_801C2C04(OPN_WORK *pWk)
{
    if ((G.Status_flg & STAT_XA_VOICE) || (D_800DBB58 & 0x80)) {
        return 0;
    }
    pWk->pScript++;
    return 1;
}

/* show picture */
LONG func_801C2C4C(OPN_WORK *pWk)
{
    Memcpy_r((void *)0x8019C000, D_801C2F0C[pWk->pScript[1]], 0x25814);
    Bg_set_mode(2, 0);
    pWk->pScript += 2;
    return 1;
}

/* start scene */
LONG func_801C2CBC(OPN_WORK *pWk)
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
LONG func_801C2D0C(OPN_WORK *pWk)
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
LONG func_801C2D8C(OPN_WORK *pWk)
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
LONG func_801C2E0C(OPN_WORK *pWk)
{
    Xa_play(3, pWk->pScript[1] + 0x13);
    G.Status_flg |= STAT_XA_VOICE;
    pWk->pScript += 2;
    return 1;
}

/* message */
LONG func_801C2E70(OPN_WORK *pWk)
{
    Mess_set(0xB90022, 0x3000, (ULONG)&D_801C2F3C[D_801C2F68[pWk->pScript[1]]], 0);
    pWk->pScript += 2;
    return 1;
}

/* frame rate */
LONG func_801C2EE0(OPN_WORK *pWk)
{
    Main.Vloop = pWk->pScript[1];
    pWk->pScript += 2;
    return 1;
}
