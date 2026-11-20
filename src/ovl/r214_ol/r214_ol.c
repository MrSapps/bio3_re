#include "common.h"
#include "ovl/r214_ol.h"

/* main task */
void func_801C2020(void)
{
    tagGLOBAL *pG = &G;
    R214_EVT *pEvt = (R214_EVT *)pG->Unk21DC;
    R214_WORK *pRw;
    ULONG i;

    Task_suspend(0);
    pRw = &R214_wk;
    pG->Status_flg |= 0x80;
    pRw->Cut_old = pG->Cut_no;
    pG->Cut_no = pEvt->Cut_no;
    Main.Unk18E = 1;
    Cut_change(pEvt->Cut_no);
    pRw->Unk0 = 0;
    pRw->Routine = 0;
    pRw->Num = 0;
    pRw->Unk3 = 0;
    pRw->Num++;
    pRw->Timer = 0;
    pG->Status_flg |= 0x400000;
    for (i = 0; i < R214_VAL_NUM; i++)
        pRw->Val[i] = 0;
    pRw->Val[0] = 50;
    for (i = 0; i < 4; i++)
        pRw->Sel[i] = 0;
    pRw->Run = 1;
    func_80038634(0xF00C, 0x60A);
    SetDrawTPage(&pRw->Tpage[0], 0, 0, 0x85);
    SetDrawTPage(&pRw->Tpage[1], 0, 0, 0x85);
    while (pRw->Run) {
        func_801C2210();
        Esp_task_main();
        func_801C275C();
        Eff_sprt_trans();
        if (!(G.System_flg & 0x80000000))
            Psp_trans();
        Task_sleep(1);
    }
    Main.Unk18E = 1;
    G.Cut_no = pRw->Cut_old;
    Cut_change(pRw->Cut_old);
    G.Status_flg |= 0x400000;
    G.Status_flg &= ~0x80;
    func_8001B894(0x1030000, NULL);
    Task_signal(0);
    Task_exit();
}

/* puzzle state machine */
void func_801C2210(void)
{
    R214_WORK *pRw = &R214_wk;
    tagGLOBAL *pG;
    LONG Up;
    LONG Down;
    LONG Sel;
    SHORT Val;
    UCHAR i;
    ULONG Id;
    ULONG Mode;
    SVECTOR *pPos;
    SVECTOR Pos;
    SVECTOR Spd;

    switch (pRw->Routine) {
    case 0:
        Mess_set_p(0xB90022, 0x3400, &R214_mess[R214_mess_ofs[pRw->Num - 1]], 0);
        pRw->Routine++;
        break;
    case 1:
        if (D_800DBB58 & 0x80)
            break;
        if (Flg_ck(G.Unk7868, 0x33)) {
            Up = 15;
            Down = 10;
        } else {
            Up = 15;
            Down = 5;
        }
        pG = &G;
        Sel = pG->Unk7848;
        switch (Sel) {
        case 0:
            Snd_se_stad(0x207, NULL, 0, 0);
            pRw->Val[pRw->Num] = pRw->Val[pRw->Num - 1];
            pRw->Val[pRw->Num] += Up * pRw->Num;
            pRw->Sel[pRw->Num - 1] = 0;
            pRw->Num++;
            break;
        case 1:
            Snd_se_stad(0x207, NULL, 0, 0);
            pRw->Val[pRw->Num] = pRw->Val[pRw->Num - 1];
            pRw->Val[pRw->Num] -= Down * pRw->Num;
            pRw->Sel[pRw->Num - 1] = Sel;
            pRw->Num++;
            break;
        case 2:
            Snd_se_stad(0x206, NULL, 0, 0);
            pRw->Num = 0;
            pRw->Unk0 = 0;
            pRw->Routine = 0;
            pRw->Unk3 = 0;
            pRw->Timer = 0;
            pRw->Num++;
            for (i = 0; i < R214_VAL_NUM; i++)
                pRw->Val[i] = 0;
            pRw->Val[0] = 50;
            for (i = 0; i < 4; i++)
                pRw->Sel[i] = 0;
            func_8001B894(0x1030000, NULL);
            return;
        case 3:
            Snd_se_stad(5, NULL, 0, 0);
            Flg_on(&pG->Unk7888, 4);
            pRw->Run = 0;
            return;
        }
        if (pRw->Num >= R214_VAL_NUM) {
            Val = pRw->Val[pRw->Num - 1];
            pRw->Routine = 4;
            if (Val >= 115 && Val <= 125)
                pRw->Routine = 2;
            if (Val >= 15 && Val <= 25)
                pRw->Routine = 3;
            if (Val < 1 || Val > 129)
                pRw->Routine = 5;
        } else {
            pRw->Routine = 0;
        }
        Mode = 0;
        Spd.vx = 0x400;
        Spd.vy = 0x300;
        Spd.vz = 0x400;
        i = pRw->Num - 2;
        pPos = &R214_pos[i][0];
        if (pRw->Sel[i]) {
            pPos++;
            Mode = 1;
        }
        Id = (Mode << 24) | 0x30215;
        Pos.vx = pPos->vx;
        Pos.vy = pPos->vy;
        Pos.vz = pPos->vz;
        func_8001B484(Id, 0x3000000, &GsIDMATRIX, &Pos, &Spd);
        break;
    case 2:
        Snd_se_stad(0x204, NULL, 0, 0);
        Flg_on(&G.Unk7888, 0);
        pRw->Routine = 6;
        func_8001B894(0x1010000, NULL);
        pPos = R214_pos_ok0;
        Spd.vx = 0x400;
        Spd.vy = 0x300;
        Spd.vz = 0x400;
        Pos.vx = pPos->vx;
        Pos.vy = pPos->vy;
        Pos.vz = pPos->vz;
        func_8001B484(0x2010215, 0x2400000, &GsIDMATRIX, &Pos, &Spd);
        break;
    case 3:
        Snd_se_stad(0x204, NULL, 0, 0);
        Flg_on(&G.Unk7888, 1);
        pRw->Routine = 6;
        func_8001B894(0x1020000, NULL);
        pPos = R214_pos_ok1;
        Spd.vx = 0x400;
        Spd.vy = 0x300;
        Spd.vz = 0x400;
        Pos.vx = pPos->vx;
        Pos.vy = pPos->vy;
        Pos.vz = pPos->vz;
        func_8001B484(0x2020215, 0x2400000, &GsIDMATRIX, &Pos, &Spd);
        break;
    case 4:
        Snd_se_stad(0x205, NULL, 0, 0);
        Flg_on(&G.Unk7888, 2);
        pRw->Routine = 6;
        break;
    case 5:
        Snd_se_stad(0x205, NULL, 0, 0);
        Flg_on(&G.Unk7888, 3);
        pRw->Routine = 6;
        break;
    case 6:
        if (pRw->Timer >= 30)
            pRw->Run = 0;
        else
            pRw->Timer++;
        break;
    }
}

/* draw the values: steps 0..2 as two digits, the rest as signed three digits */
void func_801C275C(void)
{
    R214_WORK *pRw = &R214_wk;
    DR_TPAGE *pTp;
    SPRT *pSp;
    UCHAR i;
    UCHAR Lead;
    SHORT Val;
    SHORT Dig;

    pTp = &pRw->Tpage[0];
    pSp = pRw->Sprt[0];
    if (G.Cbuf_id) {
        pSp = &pRw->Sprt[0][1];
        pTp = &pRw->Tpage[1];
    }
    AddPrim(G.pOt0, pTp);
    Lead = 0;
    for (i = 0; i < pRw->Num; i++) {
        if (i >= 3)
            break;
        Val = pRw->Val[i];
        Dig = Val / 10;
        if (Dig)
            func_801C2A38((ULONG *)pTp, &pSp, R214_digit_x[i], Dig);
        func_801C2A38((ULONG *)pTp, &pSp, R214_digit_x[i] + 14, Val % 10);
    }
    for (i = 3; i < pRw->Num; i++) {
        Val = pRw->Val[i];
        if (Val < 0) {
            Val = -Val;
            func_801C2A38((ULONG *)pTp, &pSp, R214_digit_x[i], 10);
        }
        Dig = Val / 100;
        if (Dig) {
            func_801C2A38((ULONG *)pTp, &pSp, R214_digit_x[i], Dig);
            Lead = 1;
        }
        Val %= 100;
        Dig = Val / 10;
        if (Dig || Lead)
            func_801C2A38((ULONG *)pTp, &pSp, R214_digit_x[i] + 14, Dig);
        func_801C2A38((ULONG *)pTp, &pSp, R214_digit_x[i] + 28, Val % 10);
    }
}

/* one digit sprite (n: 0..9, 10 minus) linked into pOt; *ppSp moves to the next row */
void func_801C2A38(ULONG *pOt, SPRT **ppSp, USHORT x, SHORT n)
{
    setSprt(*ppSp);
    setRGB0(*ppSp, 0x80, 0x80, 0x80);
    (*ppSp)->x0 = x;
    (*ppSp)->y0 = 0x6C;
    if (n >= 5) {
        (*ppSp)->u0 = (n - 5) * 14;
        (*ppSp)->v0 = 16;
    } else {
        (*ppSp)->u0 = n * 14;
        (*ppSp)->v0 = 0;
    }
    setWH(*ppSp, 14, 14);
    (*ppSp)->clut = 0x7B40;
    addPrim(pOt, *ppSp);
    *ppSp += 2;
}
