/*
 * RESULT.BIN: end-of-game result screens. G.Unk2115 picks the routine (Result_proc_tbl): 0 Mercenaries records
 * and shop, 1/2 Mercenaries run cleared/failed, 3 main game result (grade), 4 bonus message, 5 epilogue.
 */
#include "ovl/result.h"
#include "sub.h"

void Print14_s(LONG X, LONG Y, char *pFmt, ...) __asm__("Print14"); /* arg 1 long (canonical unsigned short); arg 2 long (canonical unsigned short); arg 3 char* (canonical UC*) */
LONG Print14_l(LONG X, LONG Y, char *pFmt, ...) __asm__("Print14"); /* int-returning alias */

/* reads piece No of OMBG.TIM (CD file 0x3C) to pAdr */
void Result_ombg_load(ULONG *pAdr, UCHAR No)
{
    CD.Fno = 0x3C;
    CD.Fsize = Result_ombg_tbl[No].Size;
    CD.Fsector = (floc[0x3C].sec_high << 16) + floc[0x3C].sector + Result_ombg_tbl[No].Ofs;
    CD.Fsum = Result_ombg_sum[No];
    Cd_read(0x3C, pAdr, 2, "OMBG.TIM");
}

/* overlay task: runs the routine selected by G.Unk2115 every frame */
void Result_task(void)
{
    Result_wk.Vloop = Main.Vloop;
    while (1) {
        Result_proc_tbl[G.Unk2115]();
        Task_sleep(1);
    }
}

/* routine 0: Mercenaries records (best time, grade, money per character) with BUY / EXIT */
void Result_merc_menu(void)
{
    RESULT_WORK *pWk = &Result_wk;

    Result_merc_menu_tbl[pWk->Routine_0](pWk);
    Result_merc_menu_disp(pWk);
}

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_carlos);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_nicholai);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_mikhail);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_no_time);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_time);

/* draws the Mercenaries result screen. Returns the last Print14's value (only one path returns):
   that explicit v0 use is what keeps the /3600 and /60 mfhi out of v0 (see NOTES.md). */
LONG Result_merc_menu_disp(RESULT_WORK *pWk)
{
    LONG Time;
    LONG Hour;
    LONG Min;
    LONG Sec;

    Print_color(0xFF, 0, 0x107);
    Jill_obj_draw(&Result_obj[0], 7);
    Jill_obj_draw(&Result_obj[1], 7);
    if (!(pWk->Flg & 0x20)) {
        Jill_obj_draw(&Result_obj[2], 7);
        Jill_obj_draw(&Result_obj[3], 7);
        Jill_obj_draw(&Result_obj[4], 7);
        Jill_obj_draw(&Result_obj[6], 7);
        Jill_obj_draw(&Result_obj[13], 7);
        Jill_obj_draw(&Result_obj[14], 7);
        Jill_obj_draw(&Result_obj[15], 7);
        Jill_obj_draw(&Result_obj[16], 7);
        Jill_obj_draw(&Result_obj[17], 7);
        Jill_obj_draw(&Result_obj[18], 7);
        Jill_obj_draw(&Result_obj[19], 7);
        Print14_s(Result_merc_menu_pos[0].X, Result_merc_menu_pos[0].Y, Result_str_carlos);
        Print14_s(Result_merc_menu_pos[1].X, Result_merc_menu_pos[1].Y, Result_str_nicholai);
        Print14_s(Result_merc_menu_pos[2].X, Result_merc_menu_pos[2].Y, Result_str_mikhail);
        if (Gr.Merc_time[0] == -1) {
            Print14_s(Result_merc_menu_pos[3].X, Result_merc_menu_pos[3].Y, Result_str_no_time);
        } else {
            Time = Gr.Merc_time[0];
            Hour = Time / 3600;
            Time -= Hour * 3600;
            Min = Time / 60;
            Sec = Time - Min * 60;
            Print14_s(Result_merc_menu_pos[3].X, Result_merc_menu_pos[3].Y, Result_str_time, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
        }
        if (Gr.Merc_time[2] == -1) {
            Print14_s(Result_merc_menu_pos[4].X, Result_merc_menu_pos[4].Y, Result_str_no_time);
        } else {
            Time = Gr.Merc_time[2];
            Hour = Time / 3600;
            Time -= Hour * 3600;
            Min = Time / 60;
            Sec = Time - Min * 60;
            Print14_s(Result_merc_menu_pos[4].X, Result_merc_menu_pos[4].Y, Result_str_time, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
        }
        if (Gr.Merc_time[1] == -1) {
            Print14_s(Result_merc_menu_pos[5].X, Result_merc_menu_pos[5].Y, Result_str_no_time);
        } else {
            Time = Gr.Merc_time[1];
            Hour = Time / 3600;
            Time -= Hour * 3600;
            Min = Time / 60;
            Sec = Time - Min * 60;
            Print14_s(Result_merc_menu_pos[5].X, Result_merc_menu_pos[5].Y, Result_str_time, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
        }
        if (Gr.Merc_grade[0] == 0xFF) {
            Print14_s(Result_merc_menu_pos[6].X, Result_merc_menu_pos[6].Y, "_");
        } else {
            Print14_s(Result_merc_menu_pos[6].X, Result_merc_menu_pos[6].Y, Result_str_grade, Result_grade_chr[Gr.Merc_grade[0]]);
        }
        if (Gr.Merc_grade[2] == 0xFF) {
            Print14_s(Result_merc_menu_pos[7].X, Result_merc_menu_pos[7].Y, "_");
        } else {
            Print14_s(Result_merc_menu_pos[7].X, Result_merc_menu_pos[7].Y, Result_str_grade, Result_grade_chr[Gr.Merc_grade[2]]);
        }
        if (Gr.Merc_grade[1] == 0xFF) {
            Print14_s(Result_merc_menu_pos[8].X, Result_merc_menu_pos[8].Y, "_");
        } else {
            Print14_s(Result_merc_menu_pos[8].X, Result_merc_menu_pos[8].Y, Result_str_grade, Result_grade_chr[Gr.Merc_grade[1]]);
        }
        Print_color(0xFF, 0, 0x4107);
        if (Gr.Merc_money[0] == 0) {
            Print14_s(Result_merc_menu_pos[9].X, Result_merc_menu_pos[9].Y, Result_str_no_money);
        } else {
            Print14_s(Result_merc_menu_pos[9].X, Result_merc_menu_pos[9].Y, Result_str_money, Gr.Merc_money[0]);
        }
        if (Gr.Merc_money[2] == 0) {
            Print14_s(Result_merc_menu_pos[10].X, Result_merc_menu_pos[10].Y, Result_str_no_money);
        } else {
            Print14_s(Result_merc_menu_pos[10].X, Result_merc_menu_pos[10].Y, Result_str_money, Gr.Merc_money[2]);
        }
        if (Gr.Merc_money[1] == 0) {
            Print14_s(Result_merc_menu_pos[11].X, Result_merc_menu_pos[11].Y, Result_str_no_money);
        } else {
            Print14_s(Result_merc_menu_pos[11].X, Result_merc_menu_pos[11].Y, Result_str_money, Gr.Merc_money[1]);
        }
        Print14_s(Result_merc_menu_pos[13].X + 7, Result_merc_menu_pos[13].Y, Result_str_buy);
        Print14_s(Result_merc_menu_pos[14].X, Result_merc_menu_pos[14].Y, Result_str_exit);
    }
    Jill_obj_draw(&Result_obj[20], 5);
    Jill_obj_draw(&Result_obj[5], 5);
    Jill_obj_draw(&Result_obj[7], 5);
    Jill_obj_draw(&Result_obj[8], 5);
    Jill_obj_draw(&Result_obj[9], 5);
    Jill_obj_draw(&Result_obj[10], 5);
    Jill_obj_draw(&Result_obj[11], 5);
    Jill_obj_draw(&Result_obj[12], 2);
    if (pWk->Flg & 8) {
        Jill_obj_draw(&Result_obj[21], 2);
        Jill_obj_draw(&Result_obj[22], 2);
        Jill_obj_draw(&Result_obj[23], 2);
        Jill_obj_draw(&Result_obj[24], 2);
        Print_color(0xFF, 0, 0x4105);
        return Print14_l(Result_merc_menu_pos[12].X, Result_merc_menu_pos[12].Y, Result_str_money, Gr.Money);
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_grade);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_no_money);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_money);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_buy);

INCLUDE_RODATA("asm/nonmatchings/ovl/result", Result_str_exit);

/* routine 0: init */
void Result_merc_menu_init(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    pWk->Cursor = 0;
    pWk->Flg = 0;
    Result_merc_bg_load();
    Bg_set_mode(2, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    Jill_obj_init(&Result_obj[0], &Result_merc_menu_spr[0], 0x01EF0015, 0x808080);
    Jill_obj_init(&Result_obj[1], &Result_merc_menu_spr[1], 0x01EF0015, 0x808080);
    Result_obj[1].pOya = &Result_obj[0];
    Jill_obj_init(&Result_obj[2], &Result_merc_menu_spr[5], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[3], &Result_merc_menu_spr[6], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[4], &Result_merc_menu_spr[7], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[5], &Result_merc_menu_spr[8], 0x01F00016, 0x808080);
    Result_obj[5].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[20], &Result_merc_menu_spr[17], 0x01F00016, 0x808080);
    Result_obj[20].pOya = &Result_obj[5];
    Result_obj[20].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[6], &Result_merc_menu_spr[9], 0x01F00016, 0x808080);
    Result_obj[6].Flg |= SEL_OBJ_PULSE | SEL_OBJ_REQ;
    Jill_obj_init(&Result_obj[21], &Result_merc_menu_spr[25], 0x01F10017, 0x808080);
    Result_obj[21].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[22], &Result_merc_menu_spr[26], 0x01F10017, 0x808080);
    Result_obj[22].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[23], &Result_merc_menu_spr[27], 0x01F10017, 0x808080);
    Result_obj[23].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[24], &Result_merc_menu_spr[28], 0x01F10017, 0x808080);
    Result_obj[24].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[7], &Result_merc_menu_spr[10], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 8)) {
        Jill_obj_rgb(&Result_obj[7], 0x404040);
        Result_obj[21].Flg |= SEL_OBJ_ON;
    }
    Result_obj[7].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[8], &Result_merc_menu_spr[11], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 7)) {
        Jill_obj_rgb(&Result_obj[8], 0x404040);
        Result_obj[22].Flg |= SEL_OBJ_ON;
    }
    Result_obj[8].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[9], &Result_merc_menu_spr[12], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 6)) {
        Jill_obj_rgb(&Result_obj[9], 0x404040);
        Result_obj[23].Flg |= SEL_OBJ_ON;
    }
    Result_obj[9].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[10], &Result_merc_menu_spr[13], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 9)) {
        Jill_obj_rgb(&Result_obj[10], 0x404040);
        Result_obj[24].Flg |= SEL_OBJ_ON;
    }
    Result_obj[10].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[11], &Result_merc_menu_spr[14], 0x01F10017, 0x808080);
    Result_obj[11].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[12], &Result_merc_menu_spr[15], 0x01F10017, 0x808080);
    Result_obj[12].Flg = (Result_obj[12].Flg | SEL_OBJ_BLINK | SEL_OBJ_REQ) & ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[13], &Result_merc_menu_spr[19], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[14], &Result_merc_menu_spr[20], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[15], &Result_merc_menu_spr[21], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[16], &Result_merc_menu_spr[22], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[17], &Result_merc_menu_spr[23], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[18], &Result_merc_menu_spr[24], 0x01F00016, 0x808080);
    Result_merc_menu_spr[18].Y++;
    Jill_obj_init(&Result_obj[19], &Result_merc_menu_spr[18], 0x01F00016, 0x808080);
    Result_obj[19].Flg |= SEL_OBJ_BLINK | SEL_OBJ_REQ;
}

/* routine 0: BUY / EXIT selection */
void Result_merc_menu_sel(RESULT_WORK *pWk)
{
    LONG i;

    switch (pWk->Routine_1) {
    case 0:
        pWk->Routine_1 = 1;
        pWk->Timer = 0;
    case 1:
        if (Gr.Key_rep & 0xA000) {
            pWk->Timer = (pWk->Timer + 1) & 1;
            Result_obj[19].Flg |= SEL_OBJ_BLINK | SEL_OBJ_REQ;
            Snd_se_stad(4, 0, 0, 0);
        }
        Result_obj[19].X = pWk->Timer * 0x74 + 0x4A;
        if (Gr.Key_trg & 0x2000) {
            Snd_se_stad(5, 0, 0, 0);
            pWk->Routine_0 = 2;
        } else if (Gr.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            if (pWk->Timer != 0) {
                pWk->Routine_0 = 2;
            } else {
                pWk->Routine_1 = 2;
                Result_obj[19].Flg &= ~SEL_OBJ_ON;
                Result_obj[5].Flg |= SEL_OBJ_ON;
                Result_obj[20].Flg |= SEL_OBJ_ON;
                for (i = 6; i != 0; i--) {
                    Result_obj[i + 6].Flg |= SEL_OBJ_ON;
                }
                pWk->Flg |= 0x28;
            }
        }
        break;
    case 2:
        Result_shop(pWk);
        if (pWk->Routine_0 == 2) {
            pWk->Routine_0 = 1;
            pWk->Routine_1 = 0;
            pWk->Routine_2 = 0;
            Result_obj[19].Flg |= SEL_OBJ_ON;
            Result_obj[5].Flg &= ~SEL_OBJ_ON;
            Result_obj[20].Flg &= ~SEL_OBJ_ON;
            for (i = 6; i != 0; i--) {
                Result_obj[i + 6].Flg &= ~SEL_OBJ_ON;
            }
            pWk->Flg &= ~0x28;
        }
        break;
    }
}

/* routine 0: exit */
void Result_merc_menu_exit(RESULT_WORK *pWk)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        Result_merc_menu_disp(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

/* routine 0: background */
void Result_merc_bg_load(void)
{
    Cd_read(0x46, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 1: result of a successful Mercenaries run: time, grade, score counted into money, then the shop */
void Result_merc_clear(void)
{
    RESULT_WORK *pWk = &Result_wk;

    Result_merc_clear_tbl[pWk->Routine_0](pWk);
    Result_merc_clear_disp(pWk);
}

/* draws the Mercenaries shop. Returns the last Print14's value: the explicit v0 use places the
   /3600 and /60 mfhi (see NOTES.md). */
LONG Result_merc_clear_disp(RESULT_WORK *pWk)
{
    tagGLOBAL *pG = &G;
    LONG Time = (SHORT)pG->Time;
    LONG Hour;
    LONG Min;

    Print_color(0xFF, 0, 0x102);
    Jill_obj_draw(&Result_obj[0], 7);
    Jill_obj_draw(&Result_obj[1], 7);
    Jill_obj_draw(&Result_obj[2], 7);
    Jill_obj_draw(&Result_obj[3], 7);
    Jill_obj_draw(&Result_obj[4], 7);
    Jill_obj_draw(&Result_obj[5], 7);
    Jill_obj_draw(&Result_obj[6], 7);
    Jill_obj_draw(&Result_obj[7], 7);
    Jill_obj_draw(&Result_obj[8], 7);
    Jill_obj_draw(&Result_obj[9], 7);
    Jill_obj_draw(&Result_obj[10], 7);
    Jill_obj_draw(&Result_obj[11], 7);
    Jill_obj_draw(&Result_obj[12], 6);
    Jill_obj_draw(&Result_obj[13], 7);
    Jill_obj_draw(&Result_obj[14], 7);
    Jill_obj_draw(&Result_obj[15], 7);
    Jill_obj_draw(&Result_obj[16], 7);
    Jill_obj_draw(&Result_obj[21], 2);
    Jill_obj_draw(&Result_obj[22], 2);
    Jill_obj_draw(&Result_obj[23], 2);
    Jill_obj_draw(&Result_obj[24], 2);
    switch (pG->Player.Id) {
    case 8:
        Print14_s(Result_merc_pos[0].X, Result_merc_pos[0].Y, Result_str_carlos);
        break;
    case 10:
        Print14_s(Result_merc_pos[0].X, Result_merc_pos[0].Y, Result_str_nicholai);
        break;
    case 9:
        Print14_s(Result_merc_pos[0].X, Result_merc_pos[0].Y, Result_str_mikhail);
        break;
    }
    if (pWk->Flg & 2) {
        Hour = Time / 3600;
        Time -= Hour * 3600;
        Min = Time / 60;
        Time -= Min * 60;
        Print14_s(Result_merc_pos[1].X, Result_merc_pos[1].Y, Result_str_time, Hour / 10, Hour % 10, Min / 10, Min % 10, Time / 10, Time % 10);
    }
    if (pWk->Flg & 0x10) {
        Print14_s(Result_merc_pos[2].X, Result_merc_pos[2].Y, Result_str_grade, Result_grade_chr[pWk->Grade]);
    }
    if (pWk->Flg & 4) {
        Print_color(0xFF, 0, 0x4102);
        Print14_s(Result_merc_pos[3].X, Result_merc_pos[3].Y, Result_str_money, Gr.Score + (SHORT)Gr.Time / 2 + pWk->Bonus);
    }
    Print_color(0xFF, 0, 0x4102);
    return Print14_l(Result_merc_pos[4].X, Result_merc_pos[4].Y, Result_str_money, Gr.Money);
}

/* routine 1: init */
void Result_merc_clear_init(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    pWk->Cursor = 0;
    Result_merc_clear_bg_load();
    Bg_set_mode(2, 0);
    Cd_read(0x125, (ULONG *)0x801F7E00, 0, "RESULT BGM");
    Snd_seq_data_set(0, 0x3D, (ULONG *)0x801F7E00);
    Snd_bank_set(5, Cd_read_s(0x126, 2, (ULONG *)0x80180000, 5, "RESULT VB"), 0);
    Snd_vab_load(0, 0xD, (ULONG *)0x801FBC00, (ULONG *)0x80180000);
    Snd_se_stad(0x500, 0, 0, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    pWk->Flg = 0;
    Jill_obj_init(&Result_obj[0], &Result_merc_spr[0], 0x01EF0015, 0x808080);
    Jill_obj_init(&Result_obj[1], &Result_merc_spr[1], 0x01EF0015, 0x808080);
    Result_obj[1].pOya = &Result_obj[0];
    Jill_obj_init(&Result_obj[2], &Result_merc_spr[5], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[3], &Result_merc_spr[6], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[4], &Result_merc_spr[7], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[5], &Result_merc_spr[8], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[6], &Result_merc_spr[2], 0x01EF0015, 0x808080);
    Result_obj[6].Flg = (Result_obj[6].Flg | SEL_OBJ_PULSE | SEL_OBJ_REQ) & ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[21], &Result_merc_spr[18], 0x01F10017, 0x808080);
    Result_obj[21].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[22], &Result_merc_spr[19], 0x01F10017, 0x808080);
    Result_obj[22].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[23], &Result_merc_spr[20], 0x01F10017, 0x808080);
    Result_obj[23].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[24], &Result_merc_spr[21], 0x01F10017, 0x808080);
    Result_obj[24].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[7], &Result_merc_spr[10], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 8)) {
        Jill_obj_rgb(&Result_obj[7], 0x404040);
        Result_obj[21].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[8], &Result_merc_spr[11], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 7)) {
        Jill_obj_rgb(&Result_obj[8], 0x404040);
        Result_obj[22].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[9], &Result_merc_spr[12], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 6)) {
        Jill_obj_rgb(&Result_obj[9], 0x404040);
        Result_obj[23].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[10], &Result_merc_spr[13], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 9)) {
        Jill_obj_rgb(&Result_obj[10], 0x404040);
        Result_obj[24].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[11], &Result_merc_spr[14], 0x01F10017, 0x808080);
    Jill_obj_init(&Result_obj[12], &Result_merc_spr[15], 0x01F10017, 0x808080);
    Result_obj[12].Flg = (Result_obj[12].Flg | SEL_OBJ_BLINK | SEL_OBJ_REQ) & ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[13], &Result_merc_spr[16], 0x01F00016, 0x808080);
    Result_obj[13].pOya = &Result_obj[2];
    Jill_obj_init(&Result_obj[14], &Result_merc_spr[16], 0x01F00016, 0x808080);
    Result_obj[14].pOya = &Result_obj[3];
    Jill_obj_init(&Result_obj[15], &Result_merc_spr[16], 0x01F00016, 0x808080);
    Result_obj[15].pOya = &Result_obj[4];
    Jill_obj_init(&Result_obj[16], &Result_merc_spr[17], 0x01F00016, 0x808080);
    Result_obj[16].pOya = &Result_obj[5];
    pWk->Bonus = 0;
    if (Flg_ck(G.Unk7868, 0x18)) {
        pWk->Bonus = 100;
    }
    Result_merc_grade_set(pWk);
}

/* routine 1: count the money earned, then the shop */
void Result_merc_clear_count(RESULT_WORK *pWk)
{
    LONG Idx;

    switch (pWk->Routine_1) {
    case 0:
        pWk->Timer = 30;
        pWk->Routine_1 = 1;
    case 1:
        if (--pWk->Timer == -1) {
            pWk->Timer = 20;
            pWk->Routine_1 = 2;
            pWk->Flg |= 1;
        }
        break;
    case 2:
        if (--pWk->Timer == -1) {
            pWk->Timer = 20;
            pWk->Routine_1 = 3;
            pWk->Flg |= 2;
            Snd_se_stad(0xC, 0, 0, 0);
        }
        break;
    case 3:
        if (--pWk->Timer == -1) {
            pWk->Timer = 20;
            pWk->Routine_1 = 4;
            pWk->Flg |= 0x10;
            Snd_se_stad(0xC, 0, 0, 0);
        }
        break;
    case 4:
        if (--pWk->Timer == -1) {
            pWk->Timer = 30;
            pWk->Routine_1 = 5;
            pWk->Flg |= 4;
            Snd_se_stad(0xC, 0, 0, 0);
        }
        break;
    case 5:
        if (--pWk->Timer == -1) {
            pWk->Routine_1 = 6;
            pWk->Flg |= 8;
            pWk->Count = pWk->Bonus + (Gr.Score + (SHORT)Gr.Time / 2);
            if (pWk->Count == 0) {
                pWk->Routine_1 = 7;
            }
            pWk->Timer = 0;
        }
        break;
    case 6:
        if (++Gr.Money > 999999) {
            Gr.Money = 999999;
            pWk->Count = 1;
        }
        if (--pWk->Count == 0) {
            pWk->Routine_1 = 7;
            break;
        }
        if (++Gr.Money > 999999) {
            Gr.Money = 999999;
            pWk->Count = 1;
        }
        if (--pWk->Count == 0) {
            pWk->Routine_1 = 7;
            break;
        }
        if (++Gr.Money > 999999) {
            Gr.Money = 999999;
            pWk->Count = 1;
        }
        if (--pWk->Count == 0) {
            pWk->Routine_1 = 7;
            break;
        }
        if (++pWk->Timer & 1) {
            Snd_se_stad(0xA, 0, 0, 0);
        }
        if (Gr.Key_trg & 0x3000) {
            pWk->Routine_1 = 7;
            Gr.Money += pWk->Count;
            if (Gr.Money > 999999) {
                Gr.Money = 999999;
            }
        }
        break;
    case 7:
        pWk->Routine_1 = 8;
        Result_obj[12].Flg |= SEL_OBJ_ON;
        Snd_se_stad(0xD, 0, 0, 0);
        Idx = Gr.Pl_id - 8;
        if (Gr.Merc_money[Idx] < Gr.Score + (SHORT)Gr.Time / 2 + pWk->Bonus) {
            Result_obj[6].Flg |= SEL_OBJ_ON;
            Gr.Merc_time[Idx] = (SHORT)Gr.Time;
            Gr.Merc_grade[Idx] = pWk->Grade;
            Gr.Merc_money[Idx] = Gr.Score + (SHORT)Gr.Time / 2 + pWk->Bonus;
        }
    case 8:
        Result_shop(pWk);
        break;
    }
}

/* routine 1: exit */
void Result_merc_clear_exit(RESULT_WORK *pWk)
{
    Snd_work.Seq[0].Fade_mode = 2;
    Snd_work.Seq[0].Fade_vol = 0x3DAF;
    Snd_work.Seq[0].Fade_step = 0x250;
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        Result_merc_clear_disp(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

/* routine 1: background */
void Result_merc_clear_bg_load(void)
{
    Cd_read(0x46, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* Mercenaries shop: item selection and buying (routines 0..2) */
void Result_shop(RESULT_WORK *pWk)
{
    switch (pWk->Routine_2) {
    case 0:
        Mess_set(0xB90000, 0x3402, (ULONG)&Result_mess[Result_mess_ofs[pWk->Cursor + 1]], 0);
        pWk->Routine_2 = 1;
    case 1:
        if (G.Unk20FC & 0x8000) {
            if (--pWk->Cursor < 0) {
                pWk->Cursor = 4;
            }
            Moji.Mess_flg = 0;
            Mess_set(0xB90000, 0x3402, (ULONG)&Result_mess[Result_mess_ofs[pWk->Cursor + 1]], 0);
            Result_obj[12].X = pWk->Cursor * 0x38 + 0x14;
            Result_obj[12].Flg |= SEL_OBJ_BLINK | SEL_OBJ_REQ;
            Snd_se_stad(4, 0, 0, 0);
        }
        if (G.Unk20FC & 0x2000) {
            if (++pWk->Cursor >= 5) {
                pWk->Cursor = 0;
            }
            Moji.Mess_flg = 0;
            Mess_set(0xB90000, 0x3402, (ULONG)&Result_mess[Result_mess_ofs[pWk->Cursor + 1]], 0);
            Result_obj[12].X = pWk->Cursor * 0x38 + 0x14;
            Result_obj[12].Flg |= SEL_OBJ_BLINK | SEL_OBJ_REQ;
            Snd_se_stad(4, 0, 0, 0);
        }
        if (Gr.Key_trg & 0x2000) {
            Snd_se_stad(5, 0, 0, 0);
            pWk->Routine_0++;
            Moji.Mess_flg = 0;
        } else if (Gr.Key_trg & 0x1000) {
            if (pWk->Cursor == 4) {
                pWk->Routine_0++;
                Snd_se_stad(6, 0, 0, 0);
                Moji.Mess_flg = 0;
            } else if (Flg_ck(Gr.Unk77F8, Result_shop_flg[pWk->Cursor])) {
                Moji.Mess_flg = 0;
                Mess_set(0xB90000, 0x3402, (ULONG)&Result_mess[Result_mess_ofs[7]], 0);
                Snd_se_stad(7, 0, 0, 0);
            } else if (Gr.Money < Result_shop_price[pWk->Cursor]) {
                Moji.Mess_flg = 0;
                Mess_set(0xB90000, 0x3402, (ULONG)&Result_mess[Result_mess_ofs[6]], 0);
                Snd_se_stad(7, 0, 0, 0);
            } else {
                pWk->Routine_2 = 2;
                Moji.Mess_flg = 0;
                Mess_set(0xB90000, 0x3002, (ULONG)&Result_mess[Result_mess_ofs[0]], 0);
                Snd_se_stad(6, 0, 0, 0);
            }
        }
        break;
    case 2:
        if (!(Moji.Mess_flg & 0x80)) {
            switch (Gr.Unk7848) {
            case 0:
                pWk->Routine_2 = 3;
                pWk->Count = Result_shop_price[pWk->Cursor];
                pWk->Timer = 0;
                Flg_on(Gr.Unk77F8, Result_shop_flg[pWk->Cursor]);
                Jill_obj_rgb(&Result_obj[pWk->Cursor + 7], 0x404040);
                Result_obj[pWk->Cursor + 21].Flg |= SEL_OBJ_ON;
                Snd_se_stad(0xB, 0, 0, 0);
                break;
            case 1:
                pWk->Routine_2 = 0;
                break;
            }
        }
        break;
    case 3:
        Gr.Money--;
        if (--pWk->Count == 0) {
            pWk->Routine_2 = 0;
            break;
        }
        Gr.Money--;
        if (--pWk->Count == 0) {
            pWk->Routine_2 = 0;
            break;
        }
        Gr.Money--;
        if (--pWk->Count == 0) {
            pWk->Routine_2 = 0;
            break;
        }
        if (++pWk->Timer & 1) {
            Snd_se_stad(0xA, 0, 0, 0);
        }
        if (Gr.Key_trg & 0x3000) {
            pWk->Routine_2 = 0;
            Gr.Money -= pWk->Count;
        }
        break;
    }
}

/* routine 2: result of a failed Mercenaries run (no time, half the score as money) */
void Result_merc_fail(void)
{
    RESULT_WORK *pWk = &Result_wk;

    Result_merc_fail_tbl[pWk->Routine_0](pWk);
    Result_merc_fail_disp(pWk);
}

/* routine 2: draws the result */
void Result_merc_fail_disp(RESULT_WORK *pWk)
{
    tagGLOBAL *pG = &G;

    Print_color(0xFF, 0, 0x102);
    Jill_obj_draw(&Result_obj[0], 7);
    Jill_obj_draw(&Result_obj[1], 7);
    Jill_obj_draw(&Result_obj[2], 7);
    Jill_obj_draw(&Result_obj[3], 7);
    Jill_obj_draw(&Result_obj[4], 7);
    Jill_obj_draw(&Result_obj[5], 7);
    Jill_obj_draw(&Result_obj[6], 7);
    Jill_obj_draw(&Result_obj[7], 7);
    Jill_obj_draw(&Result_obj[8], 7);
    Jill_obj_draw(&Result_obj[9], 7);
    Jill_obj_draw(&Result_obj[10], 7);
    Jill_obj_draw(&Result_obj[11], 7);
    Jill_obj_draw(&Result_obj[12], 6);
    Jill_obj_draw(&Result_obj[13], 7);
    Jill_obj_draw(&Result_obj[14], 7);
    Jill_obj_draw(&Result_obj[15], 7);
    Jill_obj_draw(&Result_obj[16], 7);
    Jill_obj_draw(&Result_obj[21], 2);
    Jill_obj_draw(&Result_obj[22], 2);
    Jill_obj_draw(&Result_obj[23], 2);
    Jill_obj_draw(&Result_obj[24], 2);
    switch (pG->Player.Id) {
    case 8:
        Print14_s(Result_merc_pos[0].X, Result_merc_pos[0].Y, Result_str_carlos);
        break;
    case 10:
        Print14_s(Result_merc_pos[0].X, Result_merc_pos[0].Y, Result_str_nicholai);
        break;
    case 9:
        Print14_s(Result_merc_pos[0].X, Result_merc_pos[0].Y, Result_str_mikhail);
        break;
    }
    if (pWk->Flg & 2) {
        Print14_s(Result_merc_pos[1].X, Result_merc_pos[1].Y, Result_str_no_time);
    }
    if (pWk->Flg & 0x10) {
        Print14_s(Result_merc_pos[2].X, Result_merc_pos[2].Y, Result_str_grade, Result_grade_chr[pWk->Grade]);
    }
    if (pWk->Flg & 4) {
        Print_color(0xFF, 0, 0x4102);
        Print14_s(Result_merc_pos[3].X, Result_merc_pos[3].Y, Result_str_money, Gr.Score / 2);
    }
    Print_color(0xFF, 0, 0x4102);
    Print14_s(Result_merc_pos[4].X, Result_merc_pos[4].Y, Result_str_money, Gr.Money);
}

/* routine 2: init */
void Result_merc_fail_init(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    pWk->Cursor = 0;
    Result_merc_fail_bg_load();
    Bg_set_mode(2, 0);
    Cd_read(0x125, (ULONG *)0x801F7E00, 0, "RESULT BGM");
    Snd_seq_data_set(0, 0x3D, (ULONG *)0x801F7E00);
    Snd_bank_set(5, Cd_read_s(0x126, 2, (ULONG *)0x80180000, 5, "RESULT VB"), 0);
    Snd_vab_load(0, 0xD, (ULONG *)0x801FBC00, (ULONG *)0x80180000);
    Snd_se_stad(0x500, 0, 0, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    pWk->Flg = 0;
    Jill_obj_init(&Result_obj[0], &Result_merc_spr[0], 0x01EF0015, 0x808080);
    Jill_obj_init(&Result_obj[1], &Result_merc_spr[1], 0x01EF0015, 0x808080);
    Result_obj[1].pOya = &Result_obj[0];
    Jill_obj_init(&Result_obj[2], &Result_merc_spr[5], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[3], &Result_merc_spr[6], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[4], &Result_merc_spr[7], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[5], &Result_merc_spr[8], 0x01F00016, 0x808080);
    Jill_obj_init(&Result_obj[6], &Result_merc_spr[2], 0x01EF0015, 0x808080);
    Result_obj[6].Flg = (Result_obj[6].Flg | SEL_OBJ_PULSE | SEL_OBJ_REQ) & ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[21], &Result_merc_spr[18], 0x01F10017, 0x808080);
    Result_obj[21].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[22], &Result_merc_spr[19], 0x01F10017, 0x808080);
    Result_obj[22].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[23], &Result_merc_spr[20], 0x01F10017, 0x808080);
    Result_obj[23].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[24], &Result_merc_spr[21], 0x01F10017, 0x808080);
    Result_obj[24].Flg &= ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[7], &Result_merc_spr[10], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 8)) {
        Jill_obj_rgb(&Result_obj[7], 0x404040);
        Result_obj[21].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[8], &Result_merc_spr[11], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 7)) {
        Jill_obj_rgb(&Result_obj[8], 0x404040);
        Result_obj[22].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[9], &Result_merc_spr[12], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 6)) {
        Jill_obj_rgb(&Result_obj[9], 0x404040);
        Result_obj[23].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[10], &Result_merc_spr[13], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 9)) {
        Jill_obj_rgb(&Result_obj[10], 0x404040);
        Result_obj[24].Flg |= SEL_OBJ_ON;
    }
    Jill_obj_init(&Result_obj[11], &Result_merc_spr[14], 0x01F10017, 0x808080);
    Jill_obj_init(&Result_obj[12], &Result_merc_spr[15], 0x01F10017, 0x808080);
    Result_obj[12].Flg = (Result_obj[12].Flg | SEL_OBJ_BLINK | SEL_OBJ_REQ) & ~SEL_OBJ_ON;
    Jill_obj_init(&Result_obj[13], &Result_merc_spr[16], 0x01F00016, 0x808080);
    Result_obj[13].pOya = &Result_obj[2];
    Jill_obj_init(&Result_obj[14], &Result_merc_spr[16], 0x01F00016, 0x808080);
    Result_obj[14].pOya = &Result_obj[3];
    Jill_obj_init(&Result_obj[15], &Result_merc_spr[16], 0x01F00016, 0x808080);
    Result_obj[15].pOya = &Result_obj[4];
    Jill_obj_init(&Result_obj[16], &Result_merc_spr[17], 0x01F00016, 0x808080);
    Result_obj[16].pOya = &Result_obj[5];
    Result_merc_grade_set(pWk);
}

/* routine 2: count the money earned, then the shop */
void Result_merc_fail_count(RESULT_WORK *pWk)
{
    switch (pWk->Routine_1) {
    case 0:
        pWk->Timer = 30;
        pWk->Routine_1 = 1;
    case 1:
        if (--pWk->Timer == -1) {
            pWk->Timer = 20;
            pWk->Routine_1 = 2;
            pWk->Flg |= 1;
        }
        break;
    case 2:
        if (--pWk->Timer == -1) {
            pWk->Timer = 20;
            pWk->Routine_1 = 3;
            pWk->Flg |= 2;
            Snd_se_stad(0xC, 0, 0, 0);
        }
        break;
    case 3:
        if (--pWk->Timer == -1) {
            pWk->Timer = 20;
            pWk->Routine_1 = 4;
            pWk->Flg |= 0x10;
            Snd_se_stad(0xC, 0, 0, 0);
        }
        break;
    case 4:
        if (--pWk->Timer == -1) {
            pWk->Timer = 30;
            pWk->Routine_1 = 5;
            pWk->Flg |= 4;
            Snd_se_stad(0xC, 0, 0, 0);
        }
        break;
    case 5:
        if (--pWk->Timer == -1) {
            pWk->Routine_1 = 6;
            pWk->Flg |= 8;
            if ((pWk->Count = Gr.Score / 2) == 0) {
                pWk->Routine_1 = 7;
            }
            pWk->Timer = 0;
        }
        break;
    case 6:
        if (++Gr.Money > 999999) {
            Gr.Money = 999999;
            pWk->Count = 1;
        }
        if (--pWk->Count == 0) {
            pWk->Routine_1 = 7;
            break;
        }
        if (++Gr.Money > 999999) {
            Gr.Money = 999999;
            pWk->Count = 1;
        }
        if (--pWk->Count == 0) {
            pWk->Routine_1 = 7;
            break;
        }
        if (++Gr.Money > 999999) {
            Gr.Money = 999999;
            pWk->Count = 1;
        }
        if (--pWk->Count == 0) {
            pWk->Routine_1 = 7;
            break;
        }
        if (++pWk->Timer & 1) {
            Snd_se_stad(0xA, 0, 0, 0);
        }
        if (Gr.Key_trg & 0x3000) {
            pWk->Routine_1 = 7;
            Gr.Money += pWk->Count;
            if (Gr.Money > 999999) {
                Gr.Money = 999999;
            }
        }
        break;
    case 7:
        pWk->Routine_1 = 8;
        Result_obj[12].Flg |= SEL_OBJ_ON;
        Snd_se_stad(0xD, 0, 0, 0);
    case 8:
        Result_shop(pWk);
        break;
    }
}

/* routine 2: exit */
void Result_merc_fail_exit(RESULT_WORK *pWk)
{
    Snd_work.Seq[0].Fade_mode = 2;
    Snd_work.Seq[0].Fade_vol = 0x3DAF;
    Snd_work.Seq[0].Fade_step = 0x250;
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        Result_merc_fail_disp(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

/* routine 2: background */
void Result_merc_fail_bg_load(void)
{
    Cd_read(0x46, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 3: main game result */
void Result_main(void)
{
    RESULT_WORK *pWk = &Result_wk;

    Result_main_tbl[pWk->Routine_0](pWk);
    Result_disp(pWk);
}

/* draws the main game result: time, saves and grade. Returns the last Print14's value (only one path
   returns): that explicit v0 use is what puts the /10 mfhi in t2 (see NOTES.md). */
LONG Result_disp(RESULT_WORK *pWk)
{
    LONG Time;
    LONG Hour;
    LONG Min;
    LONG Sec;

    Time = G.Game_time;
    if (Time <= 0x14996FF) {
        Hour = Time / 216000;
        Time -= Hour * 216000;
        Min = Time / 3600;
        Time -= Min * 3600;
        Sec = Time / 60;
    } else {
        Hour = 99;
        Min = 59;
        Sec = 59;
    }
    Print_color(0xFF, 0, 0x102);
    Print14_s(Result_main_pos[1].X, Result_main_pos[1].Y, "Time");
    Print14_s(Result_main_pos[2].X, Result_main_pos[2].Y, Result_str_time, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
    if (pWk->Saves == 1) {
        Print14_s(Result_main_pos[3].X, Result_main_pos[3].Y, "Number of Save");
        Print14_s(Result_main_pos[4].X, Result_main_pos[4].Y, "%1d%1d", (UCHAR)(pWk->Saves / 10), (UCHAR)(pWk->Saves % 10));
    } else {
        Print14_s(Result_main_pos[3].X, Result_main_pos[3].Y, "Number of Saves");
        Print14_s(Result_main_pos[4].X, Result_main_pos[4].Y, "%1d%1d", (UCHAR)(pWk->Saves / 10), (UCHAR)(pWk->Saves % 10));
    }
    if (!(G.System_flg & 0x100)) {
        Print14_s(Result_main_pos[5].X, Result_main_pos[5].Y, "Grade");
        return Print14_l(Result_main_pos[6].X, Result_main_pos[6].Y, Result_str_grade, Result_grade_chr[pWk->Grade]);
    }
}

/* routine 3: init */
void Result_init(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    Cd_read(0x129, (ULONG *)0x801F2600, 0, "RESULT BGM");
    Snd_seq_data_set(1, 0x2A, (ULONG *)0x801F2600);
    Snd_bank_set(6, Cd_read_s(0x12A, 2, (ULONG *)0x80180000, 6, "RESULT VB"), 0);
    Result_bg_load();
    Bg_set_mode(2, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    pWk->Saves = G.Save_cnt;
    Result_grade_set(pWk);
}

/* routine 3: wait for a button */
void Result_wait(RESULT_WORK *pWk)
{
    if (G.Key_trg & 0x3000) {
        pWk->Routine_0 = 2;
    }
    if (G.Unk20FC & 0x800) {
        pWk->Routine_0 = 2;
    }
}

/* routine 3: exit */
void Result_exit(RESULT_WORK *pWk)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        Result_disp(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

/* routine 3: background (0x50, or 0x4C when G.Unk7868 bit 0x58 is set) */
void Result_bg_load(void)
{
    Cd_read(!Flg_ck(Gr.Unk7868, 0x58) ? 0x50 : 0x4C, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 4: one message, sets G.Unk77F8 bit 5 */
void Result_bonus(void)
{
    RESULT_WORK *pWk = &Result_wk;

    Result_bonus_tbl[pWk->Routine_0](pWk);
    Result_bonus_disp(pWk);
}

/* routine 4: draws the message */
void Result_bonus_disp(RESULT_WORK *pWk)
{
    Print_color(0xFF, 0, 0x102);
}

/* routine 4: init */
void Result_bonus_init(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    Result_bonus_bg_load();
    Bg_set_mode(2, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    Snd_se_stad(0x600, 0, 0, 0);
    Flg_on(G.Unk77F8, 5);
    Moji.Mess_flg = 0;
    Mess_set(0xB90000, 0x3402, (ULONG)&Result_mess[Result_mess_ofs[8]], 0);
}

/* routine 4: wait for a button */
void Result_bonus_wait(RESULT_WORK *pWk)
{
    if (G.Key_trg & 0x3000) {
        pWk->Routine_0 = 2;
    }
    if (G.Unk20FC & 0x800) {
        pWk->Routine_0 = 2;
    }
}

/* routine 4: exit */
void Result_bonus_exit(RESULT_WORK *pWk)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        Result_bonus_disp(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

/* routine 4: background */
void Result_bonus_bg_load(void)
{
    Cd_read(0x4E, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 5: epilogue */
void Result_epilogue(void)
{
    RESULT_WORK *pWk = &Result_wk;

    Result_epilogue_tbl[pWk->Routine_0](pWk);
    Result_epilogue_disp(pWk);
}

/* routine 5: draws the epilogue */
void Result_epilogue_disp(RESULT_WORK *pWk)
{
    Print_color(0xFF, 0, 0x102);
}

/* routine 5: init (epilogue) */
void Result_epilogue_init(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    Result_epilogue_bg_load();
    Cd_read(0x11F, (ULONG *)0x801F7E00, 0, "RESULT BGM");
    Snd_seq_data_set(0, 0x33, (ULONG *)0x801F7E00);
    Snd_bank_set(5, Cd_read_s(0x120, 2, (ULONG *)0x80180000, 5, "RESULT VB"), 0);
    Snd_se_stad(0x500, 0, 0, 0);
    Bg_set_mode(2, 0);
    Fade_set(1, 5, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Flg_on(G.Unk77F8, 5);
}

/* routine 5: "EPILOGUE" title, then the epilogue pictures */
void Result_epilogue_main(RESULT_WORK *pWk)
{
    switch (pWk->Routine_1) {
    case 0:
        Fade_set(0, 1, 2, 2, 0xFFFFFF, 0, 0x5A);
        while (!(Fade_status(0) & FADE_END)) {
            Print_color(0xFF, 0, 0x102);
            Print14(0x79, 0x6B, "EPILOGUE");
            Task_sleep(1);
        }
        pWk->Routine_1 = 1;
        pWk->Timer = 0x5A;
    case 1:
        if (--pWk->Timer == 0) {
            pWk->Routine_1 = 2;
        }
        Print_color(0xFF, 0, 0x102);
        Print14(0x79, 0x6B, "EPILOGUE");
        break;
    case 2:
        Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x5A);
        while (!(Fade_status(0) & FADE_END)) {
            Print_color(0xFF, 0, 0x102);
            Print14(0x79, 0x6B, "EPILOGUE");
            Task_sleep(1);
        }
        pWk->Routine_1 = 3;
        Fade_set(1, 5, 0, 2, 0, 0, 1);
    case 3:
        Fade_set(0, 1, 2, 2, 0xFFFFFF, 0, 0x3C);
        while (!(Fade_status(0) & FADE_END)) {
            Task_sleep(1);
        }
        pWk->Routine_1 = 4;
        break;
    case 4:
        if (G.Key_trg & 0x3000) {
            if (G.Unk7806 != 7) {
                pWk->Routine_0 = 2;
            } else {
                pWk->Routine_1 = 5;
            }
        }
        if (G.Unk20FC & 0x800) {
            if (G.Unk7806 == 7) {
                pWk->Routine_1 = 5;
            } else {
                pWk->Routine_0 = 2;
            }
        }
        break;
    case 5:
        Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x5A);
        while (!(Fade_status(0) & FADE_END)) {
            Task_sleep(1);
        }
        pWk->Routine_1 = 6;
        Result_ombg_load((ULONG *)0x8019C000, 8);
        break;
    case 6:
        Fade_set(0, 1, 2, 2, 0xFFFFFF, 0, 0x3C);
        while (!(Fade_status(0) & FADE_END)) {
            Task_sleep(1);
        }
        pWk->Routine_1 = 7;
    case 7:
        if (G.Key_trg & 0x3000) {
            pWk->Routine_0 = 2;
        }
        if (G.Unk20FC & 0x800) {
            pWk->Routine_0 = 2;
        }
        break;
    }
}

/* routine 5: exit */
void Result_epilogue_exit(RESULT_WORK *pWk)
{
    Snd_work.Seq[0].Fade_mode = 2;
    Snd_work.Seq[0].Fade_vol = 0x3DAF;
    Snd_work.Seq[0].Fade_step = 0x250;
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        Result_bonus_disp(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

/* routine 5: background */
void Result_epilogue_bg_load(void)
{
    Result_ombg_load((ULONG *)0x8019C000, G.Unk7806);
}

#include "ovl/common/jill_sel/Jill_obj_init.inc.c"

#include "ovl/common/jill_sel/Jill_obj_draw.inc.c"

#include "ovl/common/jill_sel/Jill_obj_rgb.inc.c"

/* main game grade from the number of saves, the play time and Unk7E94 */
void Result_grade_set(RESULT_WORK *pWk)
{
    ULONG Time_pt;
    ULONG Unk_pt;
    ULONG Score;
    ULONG Val;

    Val = G.Save_cnt;
    Score = 0;
    if (Val < 31) {
        Score = 10;
    }
    if (Val < 24) {
        Score = 20;
    }
    if (Val < 18) {
        Score = 30;
    }
    if (Val < 13) {
        Score = 40;
    }
    if (Val < 9) {
        Score = 50;
    }
    if (Val < 6) {
        Score = 60;
    }
    if (Val < 4) {
        Score = 70;
    }
    if (Val < 3) {
        Score = 80;
    }
    if (Val < 2) {
        Score = 90;
    }
    if (Val == 0) {
        Score = 100;
    }
    Val = G.Game_time;
    Time_pt = 0;
    if (Val <= 0x20F544) {
        Time_pt = 10;
    }
    if (Val <= 0x1C03A4) {
        Time_pt = 20;
    }
    if (Val <= 0x179EA4) {
        Time_pt = 30;
    }
    if (Val <= 0x13C644) {
        Time_pt = 40;
    }
    if (Val <= 0x107A84) {
        Time_pt = 50;
    }
    if (Val <= 0xDBB64) {
        Time_pt = 60;
    }
    if (Val <= 0xB88E4) {
        Time_pt = 70;
    }
    if (Val <= 0x9E304) {
        Time_pt = 80;
    }
    if (Val <= 0x8C9C4) {
        Time_pt = 90;
    }
    if (Val <= 0x83D24) {
        Time_pt = 100;
    }
    Val = Gr.Unk7E94;
    Unk_pt = 0;
    if (Val < 0xCE5) {
        Unk_pt = 10;
    }
    if (Val < 0xC1D) {
        Unk_pt = 20;
    }
    if (Val < 0xB55) {
        Unk_pt = 30;
    }
    if (Val < 0xA8D) {
        Unk_pt = 40;
    }
    if (Val < 0x9C5) {
        Unk_pt = 50;
    }
    if (Val < 0x8FD) {
        Unk_pt = 60;
    }
    if (Val < 0x835) {
        Unk_pt = 70;
    }
    if (Val < 0x76D) {
        Unk_pt = 80;
    }
    if (Val < 0x6A5) {
        Unk_pt = 90;
    }
    if (Val < 0x5DD) {
        Unk_pt = 100;
    }
    Score += Unk_pt + Time_pt;
    pWk->Grade = 0;
    if (Score >= 50) {
        pWk->Grade = 1;
    }
    if (Score >= 120) {
        pWk->Grade = 2;
    }
    if (Score >= 180) {
        pWk->Grade = 3;
    }
    if (Score >= 230) {
        pWk->Grade = 4;
    }
    if (Score >= 270) {
        pWk->Grade = 5;
    }
    G.Save_cnt = pWk->Grade;
    if (G.System_flg & 0x100) {
        G.Save_cnt = 2;
    }
    pWk->Score = Score;
}

/* Mercenaries grade from the time left */
void Result_merc_grade_set(RESULT_WORK *pWk)
{
    ULONG Score = Gr.Score;

    pWk->Grade = 0;
    if (Score >= 300) {
        pWk->Grade = 1;
    }
    if (Score >= 600) {
        pWk->Grade = 2;
    }
    if (Score >= 900) {
        pWk->Grade = 3;
    }
    if (Score >= 1200) {
        pWk->Grade = 4;
    }
    if (Score >= 1500) {
        pWk->Grade = 5;
    }
}

