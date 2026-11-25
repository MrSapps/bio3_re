#include "ovl/gearbox.h"
#include "snd.h"

/* main task: move gears until the lever is pulled, then check and turn them */
void Gear_main(void)
{
    tagGLOBAL *pG = &G;

    Gear_init();
    for (;;) {
        Gear_ctrl();
        Gear_om_set();
        if (Gear_wk.Exit)
            break;
        Ot_set_all();
        pG->pUnk208C = (POLY_GT4 *)((ULONG *)pG->pEm_mem + pG->Cbuf_id * 13);
        Om_trans();
        Task_sleep(1);
    }
    Gear_check();
    Gear_turn();
    Gear_exit();
}

void Gear_init(void)
{
    Task_suspend(0);
    Gear_wk.Exit = 0;
    Gear_wk.Cursor = 1;
    Gear_wk.Move_cnt = 6;
    Gear_wk.Blink = 0;
    Gear_wk.Slot[0] = 0;
    Gear_wk.Slot[1] = 2;
    Gear_wk.Slot[2] = 2;
    Gear_wk.Slot[3] = 1;
    Gear_wk.Slot[4] = 1;
    Gear_wk.Om_s[0] = 6;
    Gear_wk.Om_s[1] = 7;
    Gear_wk.Om_l[0] = 8;
    Gear_wk.Om_l[1] = 9;
    Gear_wk.Om_s_turn[0] = 10;
    Gear_wk.Om_s_turn[1] = 11;
    Gear_wk.Om_l_turn[0] = 12;
    Gear_wk.Om_l_turn[1] = 13;
    G.Cut_no = 8;
    Main.Unk18E = 1;
    G.Status_flg |= 0x80;
    OM(14)->Be_flg |= 0x10;
    G.Player.Be_flg |= 0x10;
    Cut_change(8);
    G.Status_flg |= 0x400000;
}

void Gear_line_init(void)
{
    ULONG i;
    LINE_F3 *pLine;

    do {
        pLine = &Gear_wk.Line[i][0];
        SetLineF3(pLine);
        setRGB0(pLine, 0xFF, 0xFF, 0xFF);
        pLine = &Gear_wk.Line[i][1];
        SetLineF3(pLine);
        setRGB0(pLine, 0xFF, 0xFF, 0xFF);
    } while (++i < 2);
}

void Gear_exit(void)
{
    G.Cut_no = 2;
    Main.Unk18E = 1;
    OM(14)->Be_flg &= ~0x10;
    G.Player.Be_flg &= ~0x10;
    Cut_change(2);
    G.Status_flg |= 0x400000;
    Flg_on(&G.Unk7888, 0x1F);
    Task_signal(0);
    Task_exit();
}

/* gearbox: move the selected gear along its link table to the first free slot */
void Gear_ctrl(void)
{
    VECTOR Pos;
    ULONG i;

    Pos.vx = Gear_pos_tbl[2][0];
    Pos.vy = Gear_pos_tbl[2][1];
    Pos.vz = Gear_pos_tbl[2][2];
    Gear_cursor_move();
    if (G.Key_trg & 0x1000) {
        if (Gear_wk.Cursor == GEAR_LEVER) {
            Snd_se_stad(0x10208, &Pos, 0, 0);
            Gear_wk.Exit = 1;
            return;
        }
        if (Gear_wk.Slot[Gear_wk.Cursor] == 0)
            return;
        for (i = 0;; i++) {
            if (Gear_link_tbl[Gear_wk.Cursor][i] == -1)
                goto Move_end;
            if (Gear_wk.Slot[Gear_link_tbl[Gear_wk.Cursor][i]] == 0) {
                Snd_se_stad(0x10207, &Pos, 0, 0);
                Gear_wk.Slot[Gear_link_tbl[Gear_wk.Cursor][i]] = Gear_wk.Slot[Gear_wk.Cursor];
                Gear_wk.Slot[Gear_wk.Cursor] = 0;
                Gear_wk.Move_cnt--;
                break;
            }
        }
    Move_end:
        if (Gear_wk.Move_cnt == 0)
            Gear_wk.Cursor = GEAR_LEVER;
    }
    if (G.Key_trg & 0x2000)
        Gear_wk.Cursor = GEAR_LEVER;
}

void Gear_cursor_move(void)
{
    GEAR_WORK *pGw = &Gear_wk;
    ULONG Dir;

    if (pGw->Move_cnt != 0) {
        Dir = (G.Unk20FC & 0x1000) ? 0 : 0x80;
        if (G.Unk20FC & 0x4000)
            Dir = 1;
        if (G.Unk20FC & 0x2000)
            Dir = 2;
        if (G.Unk20FC & 0x8000)
            Dir = 3;
        if (Dir != 0x80 && Gear_dir_tbl[pGw->Cursor][Dir] != -1) {
            Snd_se_stad(4, NULL, 0, 0);
            pGw->Cursor = Gear_dir_tbl[pGw->Cursor][Dir];
            pGw->Blink = 0;
        }
    }
}

void Gear_om_set(void)
{
    ULONG i;
    ULONG Ns = 0;
    ULONG Nl = 0;
    OBJ_MODEL_WORK *pOm;

    for (i = 0; i < GEAR_SLOT_NUM; i++) {
        switch (Gear_wk.Slot[i]) {
        case 0:
            break;
        case 1:
            pOm = OM(Gear_wk.Om_s[Ns]);
            pOm->M.t[0] = Gear_pos_tbl[i][0];
            pOm->M.t[1] = Gear_pos_tbl[i][1];
            pOm->M.t[2] = Gear_pos_tbl[i][2];
            Ns++;
            break;
        case 2:
            pOm = OM(Gear_wk.Om_l[Nl]);
            pOm->M.t[0] = Gear_pos_tbl[i][0];
            pOm->M.t[1] = Gear_pos_tbl[i][1];
            pOm->M.t[2] = Gear_pos_tbl[i][2];
            Nl++;
            break;
        }
    }
    Gear_cursor_draw();
    Gear_count_print();
}

void Gear_cursor_draw(void)
{
    LINE_F3 *p0;
    LINE_F3 *p1;
    LONG X;
    LONG Y;
    LONG Size;

    if (--Gear_wk.Blink & 0x18) {
        p0 = &Gear_wk.Line[G.Cbuf_id][0];
        p1 = &Gear_wk.Line[G.Cbuf_id][1];
        Size = Gear_cursor_tbl[Gear_wk.Cursor][2];
        X = Gear_cursor_tbl[Gear_wk.Cursor][0] - Size;
        Y = Gear_cursor_tbl[Gear_wk.Cursor][1] - Size;
        Size *= 2;
        SetLineF3(p0);
        setRGB0(p0, 0xFF, 0xFF, 0xFF);
        p0->x0 = X;
        p0->x1 = X + Size;
        p0->x2 = X + Size;
        p1->x0 = X + Size;
        p1->x1 = X;
        p1->x2 = X;
        SetLineF3(p1);
        setRGB0(p1, 0xFF, 0xFF, 0xFF);
        p0->y0 = Y;
        p0->y1 = Y;
        p0->y2 = Y + Size;
        p1->y0 = Y + Size;
        p1->y1 = Y + Size;
        p1->y2 = Y;
        AddPrim(G.pOt0, p0);
        AddPrim(G.pOt0, p1);
    }
}

void Gear_count_print(void)
{
    Print_color(0xFF, 6, 0);
    Print14(0x4F, 0x6A, "%d", Gear_wk.Move_cnt);
}
/* the original .rodata ends with a junk byte after "%d" */
__asm__(".section .rodata\n\t.byte 0x2F\n\t.section .text");

void Gear_check(void)
{
    ULONG i;

    for (i = 0; i < 5; i++) {
        if (Gear_answer_tbl[i][0] == Gear_wk.Slot[0] && Gear_answer_tbl[i][1] == Gear_wk.Slot[1]
            && Gear_answer_tbl[i][2] == Gear_wk.Slot[2] && Gear_answer_tbl[i][3] == Gear_wk.Slot[3]
            && Gear_answer_tbl[i][4] == Gear_wk.Slot[4]) {
            G.Sce_work[4] = i + 1;
            return;
        }
    }
    G.Sce_work[4] = 0;
}

void Gear_turn(void)
{
    VECTOR Pos;
    ULONG i;
    ULONG Ns;
    ULONG Nl;
    ULONG t;
    LONG Spd;
    OBJ_MODEL_WORK *pOm;

    for (t = 0; t < 10; ) {
        t++;
        Ot_set_all();
        G.pUnk208C = (POLY_GT4 *)((ULONG *)G.pEm_mem + G.Cbuf_id * 13);
        Om_trans();
        Task_sleep(1);
    }
    Pos.vx = Gear_pos_tbl[2][0];
    Pos.vy = Gear_pos_tbl[2][1];
    Pos.vz = Gear_pos_tbl[2][2];
    if (G.Sce_work[4] != 0) {
        Snd_se_stad(0x10203, &Pos, 0, 0);
        Spd = 0x20;
    } else {
        Snd_se_stad(0x10202, &Pos, 0, 0);
        Spd = 0;
    }
    for (t = 0; t < 60; ) {
        Spd = -Spd;
        Nl = Ns = 0;
        for (i = 0; i < GEAR_SLOT_NUM; i++) {
            switch (Gear_wk.Slot[i]) {
            case 0:
                break;
            case 1:
                pOm = OM(Gear_wk.Om_s_turn[Ns]);
                Ns++;
                pOm->Cdir.vx += Spd;
                break;
            case 2:
                pOm = OM(Gear_wk.Om_l_turn[Nl]);
                Nl++;
                pOm->Cdir.vx += Spd;
                break;
            }
            if (i == 2)
                Spd = -Spd;
        }
        t++;
        Ot_set_all();
        G.pUnk208C = (POLY_GT4 *)((ULONG *)G.pEm_mem + G.Cbuf_id * 13);
        Om_trans();
        Task_sleep(1);
    }
}
