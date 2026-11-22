#include "ovl/staff_r.h"
#include "mojidisp.h"
#include "sld.h"
#include "sub.h"

/* main task */
void func_801C206C(void)
{
    STAFF_WORK *pW;

    Fade_set(0, 1, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    pW = &Staff_wk;
    pW->Rtn0 = 0;
    pW->Rtn1 = 0;
    pW->Rtn2 = 0;
    pW->Rtn3 = 0;
    pW->Timer = 0;
    pW->Timer2 = 0;
    pW->End_flg = 0;
    for (;;) {
        Staff_main_tbl[pW->Rtn0]();
        Task_sleep(1);
    }
}

/* init: load the data, narrow the curtains */
void func_801C2104(void)
{
    STAFF_WORK *pW = &Staff_wk;
    ULONG i;
    ULONG j;
    SHORT Y;

    Task_suspend(0);
    Main.Vloop = 1;
    Bg_set_mode(1, 0);
    Cd_read(0x61, (ULONG *)0x801064E0, 0, "STR_BG_DAT");
    func_801C3704();
    Cd_read(0x56, (ULONG *)0x80100000, 0, "STAFF_U_DAT");
    for (j = 0; j < 2; j++) {
        for (i = 0, Y = 0; i < 2; i++) {
            Main.Db[j].Curtain[i].y0 = Y;
            Y += 200;
            Main.Db[j].Curtain[i].h = 40;
        }
    }
    pW->Rtn0++;
}

void func_801C21E8(void)
{
    STAFF_WORK *pW = &Staff_wk;

    Staff_roll_tbl[pW->Rtn1]();
}

/* exit: restore the curtains */
void func_801C2228(void)
{
    ULONG i;
    ULONG j;
    SHORT Y;

    for (j = 0; j < 2; j++) {
        for (i = 0, Y = 0; i < 2; i++) {
            Main.Db[j].Curtain[i].y0 = Y;
            Y += 216;
            Main.Db[j].Curtain[i].h = 24;
        }
    }
    Main.Vloop = 2;
    Task_signal(0);
    Task_exit();
}

/* staff roll init. The first colour set sits two do/while(0) levels deep (a macro inside a
 * macro in the original, judging by the register allocation: the loop depth raises the 0x80
 * constant's global-alloc priority above the counter's) */
#define STAFF_GREY(p, c) do { setRGB0(p, c, c, c); } while (0)
#define STAFF_GREY2(p, c) do { STAFF_GREY(p, c); } while (0)
void func_801C22AC(void)
{
    ULONG i;
    SPRT *p;
    STAFF_WORK *pW;

    pW = &Staff_wk;

    Fade_set(0, 1, 2, 2, 0xFFFFFF, 0, 0x3C);
    CD.Vol = 100;
    Xa_play(3, 0x14);
    G.Status_flg |= STAT_XA_VOICE;
    for (i = 0; i < 2; i++) {
        p = &pW->Page[0].L[i];
        STAFF_GREY2(p, 0x80);
        p = &pW->Page[0].R[i];
        setRGB0(p, 0x80, 0x80, 0x80);
        SetDrawTPage(&pW->Page[0].Tl[i], 0, 0,
                     getTPage(1, 1, (Staff_tex[0].Tp & 0xF) << 6, (Staff_tex[0].Tp >> 4) << 8));
        SetDrawTPage(&pW->Page[0].Tr[i], 0, 0,
                     getTPage(1, 1, ((Staff_tex[0].Tp & 0xF) + 2) << 6, ((Staff_tex[0].Tp >> 4) + 2) << 8));
    }
    for (i = 0; i < 2; i++) {
        p = &pW->Page[1].L[i];
        setRGB0(p, 0, 0, 0);
        p = &pW->Page[1].L[i];
        setRGB0(p, 0, 0, 0);
        p = &pW->Page[1].R[i];
        setRGB0(p, 0, 0, 0);
        p = &pW->Page[1].R[i];
        setRGB0(p, 0, 0, 0);
        SetDrawTPage(&pW->Page[1].Tl[i], 0, 0,
                     getTPage(1, 1, (Staff_tex[1].Tp & 0xF) << 6, (Staff_tex[1].Tp >> 4) << 8));
        SetDrawTPage(&pW->Page[1].Tr[i], 0, 0,
                     getTPage(1, 1, ((Staff_tex[1].Tp & 0xF) + 2) << 6, ((Staff_tex[1].Tp >> 4) + 2) << 8));
    }
    pW->Ot0 = 6;
    pW->Ot1 = 7;
    pW->Ot2 = 5;
    pW->Timer = 0;
    pW->Timer2 = 0;
    pW->Rtn2 = 0;
    pW->Rtn3 = 0;
    pW->Mess_no = 0;
    Main.Vloop = 2;
    pW->Rtn1++;
}

void func_801C256C(void)
{
    STAFF_WORK *pW = &Staff_wk;

    Staff_page_tbl[pW->Rtn2]();
    if (!(G.Status_flg & STAT_XA_VOICE) && pW->End_flg == 1) {
        pW->Timer = 0;
        pW->Timer2 = 0;
        pW->Rtn2 = 0;
        pW->Rtn3 = 0;
        pW->Mess_no = 0;
        pW->Rtn1++;
    }
}

void func_801C25FC(void)
{
}

void func_801C2604(void)
{
    STAFF_WORK *pW = &Staff_wk;

    switch (pW->Rtn3) {
    case 0:
        if (++pW->Timer == 250) {
            pW->Kido = 0x40;
            pW->Timer2 = 0;
            pW->Bg_no0 = -1;
            pW->Bg_no1 = 0;
            pW->Rtn3 = 1;
            pW->Mess_no++;
        }
        break;
    case 1:
        if (++pW->Timer2 >= Staff_mess[pW->Mess_no].Time) {
            pW->Kido = 0x30;
            pW->Timer2 = 0;
            if (pW->Mess_no == 0)
                pW->Mess_no = 1;
        }
        if (pW->Kido < 0x80)
            pW->Kido += 2;
        if (++pW->Timer == 0x618) {
            pW->Rtn3 = 2;
            Staff_fade.Rtn = 0;
            pW->Timer = 0;
        }
        break;
    case 2:
        if (Staff_fade.End == 0)
            func_801C3A98();
        if (++pW->Timer == 200) {
            Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x3C);
            pW->Rtn3 = 3;
            pW->Timer = 0;
        }
        break;
    case 3:
        if (++pW->Timer == 150) {
            pW->Timer = 0;
            pW->Rtn2++;
        }
        break;
    }
    if (pW->Mess_no == 0)
        Mess_set(Staff_mess[0].X | (Staff_mess[0].Y << 16), 0x3200, (ULONG)&Staff_mess_data[Staff_mess_ofs[0]], 0);
    AddPrim(G.pOt0 + pW->Ot0, &pW->Page[0].L[G.Cbuf_id]);
    AddPrim(G.pOt0 + pW->Ot0, &pW->Page[0].Tl[G.Cbuf_id]);
    AddPrim(G.pOt0 + pW->Ot0, &pW->Page[0].R[G.Cbuf_id]);
    AddPrim(G.pOt0 + pW->Ot0, &pW->Page[0].Tr[G.Cbuf_id]);
    AddPrim(G.pOt0 + pW->Ot1, &pW->Page[1].L[G.Cbuf_id]);
    AddPrim(G.pOt0 + pW->Ot1, &pW->Page[1].Tl[G.Cbuf_id]);
    AddPrim(G.pOt0 + pW->Ot1, &pW->Page[1].R[G.Cbuf_id]);
    AddPrim(G.pOt0 + pW->Ot1, &pW->Page[1].Tr[G.Cbuf_id]);
}

void func_801C2974(void)
{
    Staff_wk.End_flg = 1;
}

void func_801C2984(void)
{
    STAFF_WORK *pW = &Staff_wk;
    ULONG i;
    ULONG j;
    SPRT *p;

    pW->pStaff_u = (ULONG *)0x80100000;
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 2; j++) {
            p = &pW->Bg[i].Sp[j];
            SetSprt(p);
            p->x0 = 0x20;
            p->y0 = 0xF0;
            p->w = 0x100;
            p->h = 0x100;
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = 0;
            p->v0 = 0;
            if (i & 1) {
                p->clut = GetClut(0, 0x1E0);
                SetDrawTPage(&pW->Bg[i].Tp[j], 0, 0, 0xF);
            } else {
                p->clut = GetClut(0, 0x1E0);
                SetDrawTPage(&pW->Bg[i].Tp[j], 0, 0, 0xE);
            }
        }
    }
    pW->Ot0 = 6;
    pW->Ot1 = 7;
    pW->Timer = 0;
    pW->Timer2 = 0;
    pW->Ot2 = 5;
    pW->End_flg = 0;
    Bg_set_mode(1, 0);
    CD.Vol = 100;
    Xa_play(3, 0x15);
    pW->Timer = 0;
    pW->Timer2 = 0;
    pW->Rtn2 = 0;
    Main.Vloop = 2;
    G.Status_flg |= STAT_XA_VOICE;
    pW->Rtn1++;
}

void func_801C2B30(void)
{
    STAFF_WORK *pW = &Staff_wk;

    Staff_bg_tbl[pW->Rtn2]();
    if (!(G.Status_flg & STAT_XA_VOICE) && pW->End_flg == 1)
        Staff_wk.Rtn0++;
}

void func_801C2BB8(void)
{
    if (++Staff_wk.Timer >= 2) {
        G.Status_flg |= STAT_CINESCO;
        Fade_set(0, 1, 2, 2, 0xFFFFFF, 0, 0x3C);
        Staff_seq.Rtn0 = 0;
        Staff_wk.Rtn2++;
        G.Ctp.Tpage_clut = Staff_tex[5].Tp + (Staff_tex[5].Clut_y << 8);
        Mapping_tim(Staff_wk.pBuf);
        G.Ctp.Tpage_clut = Staff_tex[6].Tp + (Staff_tex[6].Clut_y << 8);
        Mapping_tim(Staff_wk.pStr_bg);
        G.Ctp.Tpage_clut = 0xE;
        Mapping_tim(Staff_wk.pStaff_u);
        Staff_wk.pStaff_u += 0x8040 / 4;
    }
}

void func_801C2CBC(void)
{
    func_801C2F8C();
    func_801C2CE4();
}

void func_801C2CE4(void)
{
    STAFF_SEQ *pS = &Staff_seq;
    STAFF_WORK *pW = &Staff_wk;

    switch (pS->Rtn0) {
    case 0:
        pS->Page = 2;
        pS->Ot0 = 6;
        pS->Ot1 = 7;
        pS->Timer = 0;
        pS->Timer2 = 0;
        pS->Unk0A = 0;
        pS->Rtn1 = 0;
        pS->Rtn2 = 0;
        pS->Rtn3 = 0;
        pS->Rtn0 = 1;
        break;
    case 1:
        if (++pS->Timer > 0x2B0) {
            pS->Timer = 0;
            pS->Rtn0 = 2;
        }
        break;
    case 2:
        Fade_set(0, 6, 2, 2, 0, 0xFFFFFF, 0x40);
        pS->Rtn0 = 3;
        break;
    case 3:
        if (++pS->Timer2 > 0x40) {
            pS->Rtn0 = 1;
            pS->Page++;
            pS->Timer2 = 0;
            if (pS->Page == 6) {
                pS->Rtn0 = 4;
                Fade_set(0, 6, 2, 2, 0xFFFFFF, 0, 0x40);
            } else {
                pS->Timer2 = 0;
                Fade_set(0, 6, 2, 2, 0xFFFFFF, 0, 0x40);
            }
        }
        break;
    case 4:
        break;
    }
    if (pS->Rtn0 != 4) {
        AddPrim(G.pOt0 + pS->Ot1, &pW->Page[pS->Page].L[G.Cbuf_id]);
        AddPrim(G.pOt0 + pS->Ot1, &pW->Page[pS->Page].Tl[G.Cbuf_id]);
        AddPrim(G.pOt0 + pS->Ot1, &pW->Page[pS->Page].R[G.Cbuf_id]);
        AddPrim(G.pOt0 + pS->Ot1, &pW->Page[pS->Page].Tr[G.Cbuf_id]);
    }
}

void func_801C2F8C(void)
{
    STAFF_WORK *pW = &Staff_wk;
    switch (pW->Rtn3) {
    case 0:
        pW->Timer = 0;
        pW->Timer2 = 0;
        pW->Kido = 0;
        pW->Unk0A = 2;
        pW->Ot0 = 6;
        pW->Ot1 = 7;
        pW->Bg_no0 = -1;
        pW->Bg_no1 = 0;
        pW->Rtn3 = 2;
        break;
    case 1:
        pW->Bg[pW->Bg_no0].Sp[0].y0--;
        pW->Bg[pW->Bg_no0].Sp[1].y0--;
    case 2:
        pW->Bg[pW->Bg_no1].Sp[0].y0--;
        pW->Bg[pW->Bg_no1].Sp[1].y0--;
        /* the byte stores below make gcc reload Bg_no1 / Timer2 (QImode aliases everything) */
        pW->Rtn3_ret = pW->Rtn3;
        pW->Timer2++;
        pW->Rtn3 = pW->Rtn3_ret;
        if (pW->Timer2 == 0xFF) {
            pW->Rtn3_ret = 1;
            pW->Timer2 = 0;
            pW->Bg_no0++;
            pW->Bg_no1++;
            pW->Rtn3 = 1;
            if (pW->Bg_no1 == 13) {
                pW->Rtn3 = 4;
                G.Status_flg &= ~STAT_CINESCO;
                Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 1);
            } else if (pW->Bg_no1 < 12) {
                if (pW->Bg_no1 % 2) /* '% 2', not '& 1': the target reloads the field with lhu */
                    G_tpage_clut = 0x10F;
                else
                    G_tpage_clut = 0xE;
                Mapping_tim(pW->pStaff_u);
                pW->pStaff_u += 0x8040 / 4;
            }
        }
        break;
    case 3:
        pW->Rtn3 = pW->Rtn3_ret;
        break;
    case 4:
        pW->Rtn3 = 0;
        pW->Rtn2++;
        break;
    }
    if (pW->Bg_no0 >= 0) {
        AddPrim(G.pOt0 + 5, &pW->Bg[pW->Bg_no0].Sp[G.Cbuf_id]);
        AddPrim(G.pOt0 + 5, &pW->Bg[pW->Bg_no0].Tp[G.Cbuf_id]);
    }
    if (pW->Bg_no1 < 12) {
        AddPrim(G.pOt0 + 5, &pW->Bg[pW->Bg_no1].Sp[G.Cbuf_id]);
        AddPrim(G.pOt0 + 5, &pW->Bg[pW->Bg_no1].Tp[G.Cbuf_id]);
    }
}

void func_801C32DC(void)
{
    STAFF_WORK *pW = &Staff_wk;
    ULONG i;
    SPRT *p;

    switch (pW->Rtn3) {
    case 0:
        if (++pW->Timer2 >= 2) {
            G.Ctp.Tpage_clut = 0x1D0B;
            Mapping_tim(pW->pStaff_u);
            pW->pStaff_u += 0x3220 / 4;
            for (i = 0; i < 2; i++) {
                p = &pW->Logo.Sp[i];
                SetSprt(p);
                p->x0 = 0x20;
                p->y0 = 0x60;
                p->w = 0x100;
                setRGB0(p, 0x80, 0x80, 0x80);
                p->h = 0x30;
                p->u0 = 0;
                p->v0 = 0;
                p->clut = GetClut(0, 0x1FD);
                SetDrawTPage(&pW->Logo.Tp[i], 0, 0, 0xAB);
            }
            pW->Rtn3 = 1;
            pW->Timer2 = 0;
            Fade_set(0, 1, 2, 2, 0xFFFFFF, 0, 4);
            Fade_set(1, 4, 2, 2, 0, 0xFFFFFF, 1);
            Fade_set(2, 6, 2, 2, 0, 0xFFFFFF, 1);
        }
        break;
    case 1:
        if (++pW->Timer2 >= 5) {
            Fade_set(1, 4, 2, 2, 0xFFFFFF, 0, 0x20);
            pW->Timer2 = 0;
            pW->Rtn3 = 2;
        }
        break;
    case 2:
        if (++pW->Timer2 >= 0x41) {
            pW->Rtn3 = 3;
            pW->Timer2 = 0;
            Fade_set(2, 6, 2, 2, 0xFFFFFF, 0, 0x40);
        }
        break;
    case 3:
        pW->Rtn3 = 4;
    case 4:
        pW->Rtn3 = 5;
    case 5:
        if (++pW->Timer2 >= 0x81) {
            Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x5A);
            pW->Rtn3 = 6;
            pW->Timer2 = 0;
        }
        break;
    case 6:
        if (++pW->Timer2 >= 0x81) {
            pW->Rtn3 = 7;
            pW->Timer2 = 0;
        }
        break;
    case 7:
        pW->Rtn2++;
        break;
    }
    if (pW->Rtn3 != 0) {
        AddPrim(G.pOt0 + pW->Ot2, &pW->Logo.Sp[G.Cbuf_id]);
        AddPrim(G.pOt0 + pW->Ot2, &pW->Logo.Tp[G.Cbuf_id]);
        AddPrim(G.pOt0 + pW->Ot1, &pW->Page[6].L[G.Cbuf_id]);
        AddPrim(G.pOt0 + pW->Ot1, &pW->Page[6].Tl[G.Cbuf_id]);
        AddPrim(G.pOt0 + pW->Ot1, &pW->Page[6].R[G.Cbuf_id]);
        AddPrim(G.pOt0 + pW->Ot1, &pW->Page[6].Tr[G.Cbuf_id]);
    }
}

void func_801C36F4(void)
{
    Staff_wk.End_flg = 1;
}

void func_801C3704(void)
{
    STAFF_WORK *pW = &Staff_wk;
    ULONG i;
    ULONG j;
    SPRT *p;

    pW->pStr_bg = (ULONG *)0x801064E0;
    for (i = 0; i < 5; i++) {
        G_tpage_clut = Staff_tex[i].Tp + (Staff_tex[i].Clut_y << 8);
        Mapping_tim(pW->pStr_bg);
        pW->pStr_bg += 0x12E20 / 4;
    }
    G_tpage_clut = 0xE;
    Mapping_tim(pW->pStr_bg);
    pW->pStr_bg += 0x5840 / 4;
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 2; j++) {
            p = &pW->Line[i].Sp[j];
            SetSprt(p);
            p->x0 = 0x20;
            p->w = 0x100;
            p->h = 0x10;
            setRGB0(p, 0x30, 0x30, 0x30);
            p->y0 = i * 20 + 0x20;
            p->u0 = 0;
            p->v0 = i << 4;
            setSemiTrans(p, 1);
            p->clut = GetClut(0, 0x1E0);
            SetDrawTPage(&pW->Line[i].Tp[j], 0, 0, 0xE);
        }
    }
    pW->pBuf = (ULONG *)0x8019C000;
    func_800100A4((void *)0x8019C000, pW->pStr_bg, 0x12E20);
    pW->pStr_bg += 0x12E20 / 4;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 2; j++) {
            p = &pW->Page[i].L[j];
            SetSprt(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            p->x0 = 0;
            p->y0 = 0;
            p->w = 0x100;
            p->h = 0xF0;
            p->u0 = 0;
            p->v0 = 0;
            p->clut = GetClut(0, Staff_tex[i].Clut_y + 0x1E0);
            SetDrawTPage(&pW->Page[i].Tl[j], 0, 0,
                         getTPage(1, 1, (Staff_tex[i].Tp & 0xF) << 6, (Staff_tex[i].Tp >> 4) << 8));
            p = &pW->Page[i].R[j];
            SetSprt(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            p->x0 = 0x100;
            p->y0 = 0;
            p->w = 0x40;
            p->h = 0xF0;
            p->u0 = 0;
            p->v0 = 0;
            p->clut = GetClut(0, Staff_tex[i].Clut_y + 0x1E0);
            SetDrawTPage(&pW->Page[i].Tr[j], 0, 0,
                         getTPage(1, 1, ((Staff_tex[i].Tp + 2) & 0xF) << 6, ((Staff_tex[i].Tp + 2) >> 4) << 8));
        }
    }
}

void func_801C3A98(void)
{
    STAFF_WORK *pW = &Staff_wk;
    STAFF_FADE *pF = &Staff_fade;
    ULONG i;
    ULONG j;
    SPRT *p;
    USHORT Ot;

    switch (pF->Rtn) {
    case 0:
        for (j = 0; j < 3; j++) {
            pF->Col0[j] = 0x80;
            pF->Col1[j] = 0;
        }
        pF->End = 0;
        pF->Rtn = 1;
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            p = &pW->Page[0].L[i];
            setRGB0(p, pF->Col0[0], pF->Col0[1], pF->Col0[2]);
            setSemiTrans(p, 1);
            p = &pW->Page[0].R[i];
            setRGB0(p, pF->Col0[0], pF->Col0[1], pF->Col0[2]);
            setSemiTrans(p, 1);
            for (j = 0; j < 1; j++) {
                p = &pW->Line[j].Sp[i];
                setRGB0(p, pF->Col0[0], pF->Col0[1], pF->Col0[2]);
                setSemiTrans(p, 1);
            }
        }
        for (i = 0; i < 2; i++) {
            p = &pW->Page[1].L[i];
            setRGB0(p, pF->Col1[0], pF->Col1[1], pF->Col1[2]);
            setSemiTrans(p, 0);
            p = &pW->Page[1].R[i];
            setRGB0(p, pF->Col1[1], pF->Col1[1], pF->Col1[2]);
            setSemiTrans(p, 0);
        }
        for (j = 0; j < 3; j++) {
            pF->Col0[j] -= 2;
            pF->Col1[j] += 2;
        }
        if (pF->Col0[0] == 0x40) {
            pF->Rtn = 2;
            Ot = pW->Ot0;
            pW->Ot0 = pW->Ot1;
            pW->Ot1 = Ot;
        }
        break;
    case 2:
        for (i = 0; i < 2; i++) {
            p = &pW->Page[0].L[i];
            setRGB0(p, pF->Col0[0], pF->Col0[1], pF->Col0[2]);
            setSemiTrans(p, 0);
            p = &pW->Page[0].R[i];
            setRGB0(p, pF->Col0[0], pF->Col0[1], pF->Col0[2]);
            setSemiTrans(p, 0);
            for (j = 0; j < 1; j++) {
                p = &pW->Line[j].Sp[i];
                setRGB0(p, pF->Col0[0], pF->Col0[1], pF->Col0[2]);
                setSemiTrans(p, 0);
            }
        }
        for (i = 0; i < 2; i++) {
            p = &pW->Page[1].L[i];
            setRGB0(p, pF->Col1[0], pF->Col1[1], pF->Col1[2]);
            setSemiTrans(p, 1);
            p = &pW->Page[1].R[i];
            setRGB0(p, pF->Col1[1], pF->Col1[1], pF->Col1[2]);
            setSemiTrans(p, 1);
        }
        for (j = 0; j < 3; j++) {
            pF->Col0[j] -= 2;
            pF->Col1[j] += 2;
        }
        if (pF->Col1[0] == 0x60)
            pW->Ot2 = 7;
        if (pF->Col1[0] == 0x80) {
            pF->End = 1;
            pF->Rtn = 3;
        }
        break;
    }
}
