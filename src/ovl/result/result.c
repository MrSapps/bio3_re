#include "ovl/result.h"
#include "sub.h"

void Print14_s(LONG X, LONG Y, char *pFmt, ...) __asm__("Print14"); /* arg 1 long (canonical unsigned short); arg 2 long (canonical unsigned short); arg 3 char* (canonical UC*) */
void func_800783BC_(ULONG no, ULONG size, ULONG a2) __asm__("func_800783BC"); /* 3 args here (canonical 2) */

LONG Print14_l(LONG X, LONG Y, char *pFmt, ...) __asm__("Print14"); /* int-returning alias */

void func_801C2154(ULONG *pAdr, UCHAR No)
{
    CD.Fno = 0x3C;
    CD.Fsize = D_801C6800[No].Size;
    CD.Fsector = (floc[0x3C].sec_high << 16) + floc[0x3C].sector + D_801C6800[No].Ofs;
    CD.Fsum = D_801C6850[No];
    Cd_read(0x3C, pAdr, 2, "OMBG.TIM");
}

/* overlay task: runs the routine selected by G.Unk2115 every frame */
void func_801C21EC(void)
{
    D_801C76C0.Vloop = Main.Vloop;
    while (1) {
        D_801C69BC[G.Unk2115]();
        Task_sleep(1);
    }
}

/* routine 0: Mercenaries result screen */
void func_801C224C(void)
{
    RESULT_WORK *pWk = &D_801C76C0;

    D_801C69D4[pWk->Routine_0](pWk);
    func_801C229C(pWk);
}

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2010);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2018);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2024);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C202C);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2038);

/* draws the Mercenaries result screen. Returns the last Print14's value (only one path returns):
   that explicit v0 use is what keeps the /3600 and /60 mfhi out of v0 (see NOTES.md). */
LONG func_801C229C(RESULT_WORK *pWk)
{
    LONG Time;
    LONG Hour;
    LONG Min;
    LONG Sec;

    Print_color(0xFF, 0, 0x107);
    func_801C62B0(&D_801C6D60[0], 7);
    func_801C62B0(&D_801C6D60[1], 7);
    if (!(pWk->Flg & 0x20)) {
        func_801C62B0(&D_801C6D60[2], 7);
        func_801C62B0(&D_801C6D60[3], 7);
        func_801C62B0(&D_801C6D60[4], 7);
        func_801C62B0(&D_801C6D60[6], 7);
        func_801C62B0(&D_801C6D60[13], 7);
        func_801C62B0(&D_801C6D60[14], 7);
        func_801C62B0(&D_801C6D60[15], 7);
        func_801C62B0(&D_801C6D60[16], 7);
        func_801C62B0(&D_801C6D60[17], 7);
        func_801C62B0(&D_801C6D60[18], 7);
        func_801C62B0(&D_801C6D60[19], 7);
        Print14_s(D_801C69E0[0].X, D_801C69E0[0].Y, D_801C2010);
        Print14_s(D_801C69E0[1].X, D_801C69E0[1].Y, D_801C2018);
        Print14_s(D_801C69E0[2].X, D_801C69E0[2].Y, D_801C2024);
        if (Gr.Merc_time[0] == -1) {
            Print14_s(D_801C69E0[3].X, D_801C69E0[3].Y, D_801C202C);
        } else {
            Time = Gr.Merc_time[0];
            Hour = Time / 3600;
            Time -= Hour * 3600;
            Min = Time / 60;
            Sec = Time - Min * 60;
            Print14_s(D_801C69E0[3].X, D_801C69E0[3].Y, D_801C2038, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
        }
        if (Gr.Merc_time[2] == -1) {
            Print14_s(D_801C69E0[4].X, D_801C69E0[4].Y, D_801C202C);
        } else {
            Time = Gr.Merc_time[2];
            Hour = Time / 3600;
            Time -= Hour * 3600;
            Min = Time / 60;
            Sec = Time - Min * 60;
            Print14_s(D_801C69E0[4].X, D_801C69E0[4].Y, D_801C2038, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
        }
        if (Gr.Merc_time[1] == -1) {
            Print14_s(D_801C69E0[5].X, D_801C69E0[5].Y, D_801C202C);
        } else {
            Time = Gr.Merc_time[1];
            Hour = Time / 3600;
            Time -= Hour * 3600;
            Min = Time / 60;
            Sec = Time - Min * 60;
            Print14_s(D_801C69E0[5].X, D_801C69E0[5].Y, D_801C2038, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
        }
        if (Gr.Merc_grade[0] == 0xFF) {
            Print14_s(D_801C69E0[6].X, D_801C69E0[6].Y, "_");
        } else {
            Print14_s(D_801C69E0[6].X, D_801C69E0[6].Y, D_801C2054, D_801C69B4[Gr.Merc_grade[0]]);
        }
        if (Gr.Merc_grade[2] == 0xFF) {
            Print14_s(D_801C69E0[7].X, D_801C69E0[7].Y, "_");
        } else {
            Print14_s(D_801C69E0[7].X, D_801C69E0[7].Y, D_801C2054, D_801C69B4[Gr.Merc_grade[2]]);
        }
        if (Gr.Merc_grade[1] == 0xFF) {
            Print14_s(D_801C69E0[8].X, D_801C69E0[8].Y, "_");
        } else {
            Print14_s(D_801C69E0[8].X, D_801C69E0[8].Y, D_801C2054, D_801C69B4[Gr.Merc_grade[1]]);
        }
        Print_color(0xFF, 0, 0x4107);
        if (Gr.Merc_money[0] == 0) {
            Print14_s(D_801C69E0[9].X, D_801C69E0[9].Y, D_801C2058);
        } else {
            Print14_s(D_801C69E0[9].X, D_801C69E0[9].Y, D_801C2060, Gr.Merc_money[0]);
        }
        if (Gr.Merc_money[2] == 0) {
            Print14_s(D_801C69E0[10].X, D_801C69E0[10].Y, D_801C2058);
        } else {
            Print14_s(D_801C69E0[10].X, D_801C69E0[10].Y, D_801C2060, Gr.Merc_money[2]);
        }
        if (Gr.Merc_money[1] == 0) {
            Print14_s(D_801C69E0[11].X, D_801C69E0[11].Y, D_801C2058);
        } else {
            Print14_s(D_801C69E0[11].X, D_801C69E0[11].Y, D_801C2060, Gr.Merc_money[1]);
        }
        Print14_s(D_801C69E0[13].X + 7, D_801C69E0[13].Y, D_801C2068);
        Print14_s(D_801C69E0[14].X, D_801C69E0[14].Y, D_801C206C);
    }
    func_801C62B0(&D_801C6D60[20], 5);
    func_801C62B0(&D_801C6D60[5], 5);
    func_801C62B0(&D_801C6D60[7], 5);
    func_801C62B0(&D_801C6D60[8], 5);
    func_801C62B0(&D_801C6D60[9], 5);
    func_801C62B0(&D_801C6D60[10], 5);
    func_801C62B0(&D_801C6D60[11], 5);
    func_801C62B0(&D_801C6D60[12], 2);
    if (pWk->Flg & 8) {
        func_801C62B0(&D_801C6D60[21], 2);
        func_801C62B0(&D_801C6D60[22], 2);
        func_801C62B0(&D_801C6D60[23], 2);
        func_801C62B0(&D_801C6D60[24], 2);
        Print_color(0xFF, 0, 0x4105);
        return Print14_l(D_801C69E0[12].X, D_801C69E0[12].Y, D_801C2060, Gr.Money);
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2054);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2058);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2060);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C2068);

INCLUDE_RODATA("asm/nonmatchings/ovl/result/result", D_801C206C);

/* Mercenaries result: init */
void func_801C2ABC(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    pWk->Cursor = 0;
    pWk->Flg = 0;
    func_801C3294();
    Bg_set_mode(2, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    func_801C6140(&D_801C6D60[0], &D_801C6A1C[0], 0x01EF0015, 0x808080);
    func_801C6140(&D_801C6D60[1], &D_801C6A1C[1], 0x01EF0015, 0x808080);
    D_801C6D60[1].pParent = &D_801C6D60[0];
    func_801C6140(&D_801C6D60[2], &D_801C6A1C[5], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[3], &D_801C6A1C[6], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[4], &D_801C6A1C[7], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[5], &D_801C6A1C[8], 0x01F00016, 0x808080);
    D_801C6D60[5].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[20], &D_801C6A1C[17], 0x01F00016, 0x808080);
    D_801C6D60[20].pParent = &D_801C6D60[5];
    D_801C6D60[20].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[6], &D_801C6A1C[9], 0x01F00016, 0x808080);
    D_801C6D60[6].Flg |= RSPR_PULSE | RSPR_RESET;
    func_801C6140(&D_801C6D60[21], &D_801C6A1C[25], 0x01F10017, 0x808080);
    D_801C6D60[21].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[22], &D_801C6A1C[26], 0x01F10017, 0x808080);
    D_801C6D60[22].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[23], &D_801C6A1C[27], 0x01F10017, 0x808080);
    D_801C6D60[23].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[24], &D_801C6A1C[28], 0x01F10017, 0x808080);
    D_801C6D60[24].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[7], &D_801C6A1C[10], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 8)) {
        func_801C64E4(&D_801C6D60[7], 0x404040);
        D_801C6D60[21].Flg |= RSPR_ON;
    }
    D_801C6D60[7].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[8], &D_801C6A1C[11], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 7)) {
        func_801C64E4(&D_801C6D60[8], 0x404040);
        D_801C6D60[22].Flg |= RSPR_ON;
    }
    D_801C6D60[8].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[9], &D_801C6A1C[12], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 6)) {
        func_801C64E4(&D_801C6D60[9], 0x404040);
        D_801C6D60[23].Flg |= RSPR_ON;
    }
    D_801C6D60[9].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[10], &D_801C6A1C[13], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 9)) {
        func_801C64E4(&D_801C6D60[10], 0x404040);
        D_801C6D60[24].Flg |= RSPR_ON;
    }
    D_801C6D60[10].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[11], &D_801C6A1C[14], 0x01F10017, 0x808080);
    D_801C6D60[11].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[12], &D_801C6A1C[15], 0x01F10017, 0x808080);
    D_801C6D60[12].Flg = (D_801C6D60[12].Flg | RSPR_BLINK | RSPR_RESET) & ~RSPR_ON;
    func_801C6140(&D_801C6D60[13], &D_801C6A1C[19], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[14], &D_801C6A1C[20], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[15], &D_801C6A1C[21], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[16], &D_801C6A1C[22], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[17], &D_801C6A1C[23], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[18], &D_801C6A1C[24], 0x01F00016, 0x808080);
    D_801C6A1C[18].Y++;
    func_801C6140(&D_801C6D60[19], &D_801C6A1C[18], 0x01F00016, 0x808080);
    D_801C6D60[19].Flg |= RSPR_BLINK | RSPR_RESET;
}

/* Mercenaries result: BUY / EXIT selection */
void func_801C2FD0(RESULT_WORK *pWk)
{
    LONG i;

    switch (pWk->Routine_1) {
    case 0:
        pWk->Routine_1 = 1;
        pWk->Timer = 0;
    case 1:
        if (Gr.Key_rep & 0xA000) {
            pWk->Timer = (pWk->Timer + 1) & 1;
            D_801C6D60[19].Flg |= RSPR_BLINK | RSPR_RESET;
            Snd_se_stad(4, 0, 0, 0);
        }
        D_801C6D60[19].X = pWk->Timer * 0x74 + 0x4A;
        if (Gr.Key_trg & 0x2000) {
            Snd_se_stad(5, 0, 0, 0);
            pWk->Routine_0 = 2;
        } else if (Gr.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            if (pWk->Timer != 0) {
                pWk->Routine_0 = 2;
            } else {
                pWk->Routine_1 = 2;
                D_801C6D60[19].Flg &= ~RSPR_ON;
                D_801C6D60[5].Flg |= RSPR_ON;
                D_801C6D60[20].Flg |= RSPR_ON;
                for (i = 6; i != 0; i--) {
                    D_801C6D60[i + 6].Flg |= RSPR_ON;
                }
                pWk->Flg |= 0x28;
            }
        }
        break;
    case 2:
        func_801C4188(pWk);
        if (pWk->Routine_0 == 2) {
            pWk->Routine_0 = 1;
            pWk->Routine_1 = 0;
            pWk->Routine_2 = 0;
            D_801C6D60[19].Flg |= RSPR_ON;
            D_801C6D60[5].Flg &= ~RSPR_ON;
            D_801C6D60[20].Flg &= ~RSPR_ON;
            for (i = 6; i != 0; i--) {
                D_801C6D60[i + 6].Flg &= ~RSPR_ON;
            }
            pWk->Flg &= ~0x28;
        }
        break;
    }
}

void func_801C31FC(RESULT_WORK *pWk)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C229C(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

void func_801C3294(void)
{
    Cd_read(0x46, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 1: Mercenaries shop */
void func_801C32F4(void)
{
    RESULT_WORK *pWk = &D_801C76C0;

    D_801C6BB4[pWk->Routine_0](pWk);
    func_801C3344(pWk);
}

/* draws the Mercenaries shop. Returns the last Print14's value: the explicit v0 use places the
   /3600 and /60 mfhi (see NOTES.md). */
LONG func_801C3344(RESULT_WORK *pWk)
{
    tagGLOBAL *pG = &G;
    LONG Time = (SHORT)pG->Time;
    LONG Hour;
    LONG Min;

    Print_color(0xFF, 0, 0x102);
    func_801C62B0(&D_801C6D60[0], 7);
    func_801C62B0(&D_801C6D60[1], 7);
    func_801C62B0(&D_801C6D60[2], 7);
    func_801C62B0(&D_801C6D60[3], 7);
    func_801C62B0(&D_801C6D60[4], 7);
    func_801C62B0(&D_801C6D60[5], 7);
    func_801C62B0(&D_801C6D60[6], 7);
    func_801C62B0(&D_801C6D60[7], 7);
    func_801C62B0(&D_801C6D60[8], 7);
    func_801C62B0(&D_801C6D60[9], 7);
    func_801C62B0(&D_801C6D60[10], 7);
    func_801C62B0(&D_801C6D60[11], 7);
    func_801C62B0(&D_801C6D60[12], 6);
    func_801C62B0(&D_801C6D60[13], 7);
    func_801C62B0(&D_801C6D60[14], 7);
    func_801C62B0(&D_801C6D60[15], 7);
    func_801C62B0(&D_801C6D60[16], 7);
    func_801C62B0(&D_801C6D60[21], 2);
    func_801C62B0(&D_801C6D60[22], 2);
    func_801C62B0(&D_801C6D60[23], 2);
    func_801C62B0(&D_801C6D60[24], 2);
    switch (pG->Pl_id) {
    case 8:
        Print14_s(D_801C6BC0[0].X, D_801C6BC0[0].Y, D_801C2010);
        break;
    case 10:
        Print14_s(D_801C6BC0[0].X, D_801C6BC0[0].Y, D_801C2018);
        break;
    case 9:
        Print14_s(D_801C6BC0[0].X, D_801C6BC0[0].Y, D_801C2024);
        break;
    }
    if (pWk->Flg & 2) {
        Hour = Time / 3600;
        Time -= Hour * 3600;
        Min = Time / 60;
        Time -= Min * 60;
        Print14_s(D_801C6BC0[1].X, D_801C6BC0[1].Y, D_801C2038, Hour / 10, Hour % 10, Min / 10, Min % 10, Time / 10, Time % 10);
    }
    if (pWk->Flg & 0x10) {
        Print14_s(D_801C6BC0[2].X, D_801C6BC0[2].Y, D_801C2054, D_801C69B4[pWk->Grade]);
    }
    if (pWk->Flg & 4) {
        Print_color(0xFF, 0, 0x4102);
        Print14_s(D_801C6BC0[3].X, D_801C6BC0[3].Y, D_801C2060, Gr.Score + (SHORT)Gr.Time / 2 + pWk->Bonus);
    }
    Print_color(0xFF, 0, 0x4102);
    return Print14_l(D_801C6BC0[4].X, D_801C6BC0[4].Y, D_801C2060, Gr.Money);
}

/* Mercenaries shop: init */
void func_801C3724(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    pWk->Cursor = 0;
    func_801C4128();
    Bg_set_mode(2, 0);
    Cd_read(0x125, (ULONG *)0x801F7E00, 0, "RESULT BGM");
    func_800782F4(0, 0x3D, (ULONG *)0x801F7E00);
    func_800783BC_(5, Cd_read_s(0x126, 2, (ULONG *)0x80180000, 5, "RESULT VB"), 0);
    func_8007809C(0, 0xD, (ULONG *)0x801FBC00, (ULONG *)0x80180000);
    Snd_se_stad(0x500, 0, 0, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    pWk->Flg = 0;
    func_801C6140(&D_801C6D60[0], &D_801C6BD4[0], 0x01EF0015, 0x808080);
    func_801C6140(&D_801C6D60[1], &D_801C6BD4[1], 0x01EF0015, 0x808080);
    D_801C6D60[1].pParent = &D_801C6D60[0];
    func_801C6140(&D_801C6D60[2], &D_801C6BD4[5], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[3], &D_801C6BD4[6], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[4], &D_801C6BD4[7], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[5], &D_801C6BD4[8], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[6], &D_801C6BD4[2], 0x01EF0015, 0x808080);
    D_801C6D60[6].Flg = (D_801C6D60[6].Flg | RSPR_PULSE | RSPR_RESET) & ~RSPR_ON;
    func_801C6140(&D_801C6D60[21], &D_801C6BD4[18], 0x01F10017, 0x808080);
    D_801C6D60[21].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[22], &D_801C6BD4[19], 0x01F10017, 0x808080);
    D_801C6D60[22].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[23], &D_801C6BD4[20], 0x01F10017, 0x808080);
    D_801C6D60[23].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[24], &D_801C6BD4[21], 0x01F10017, 0x808080);
    D_801C6D60[24].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[7], &D_801C6BD4[10], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 8)) {
        func_801C64E4(&D_801C6D60[7], 0x404040);
        D_801C6D60[21].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[8], &D_801C6BD4[11], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 7)) {
        func_801C64E4(&D_801C6D60[8], 0x404040);
        D_801C6D60[22].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[9], &D_801C6BD4[12], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 6)) {
        func_801C64E4(&D_801C6D60[9], 0x404040);
        D_801C6D60[23].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[10], &D_801C6BD4[13], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 9)) {
        func_801C64E4(&D_801C6D60[10], 0x404040);
        D_801C6D60[24].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[11], &D_801C6BD4[14], 0x01F10017, 0x808080);
    func_801C6140(&D_801C6D60[12], &D_801C6BD4[15], 0x01F10017, 0x808080);
    D_801C6D60[12].Flg = (D_801C6D60[12].Flg | RSPR_BLINK | RSPR_RESET) & ~RSPR_ON;
    func_801C6140(&D_801C6D60[13], &D_801C6BD4[16], 0x01F00016, 0x808080);
    D_801C6D60[13].pParent = &D_801C6D60[2];
    func_801C6140(&D_801C6D60[14], &D_801C6BD4[16], 0x01F00016, 0x808080);
    D_801C6D60[14].pParent = &D_801C6D60[3];
    func_801C6140(&D_801C6D60[15], &D_801C6BD4[16], 0x01F00016, 0x808080);
    D_801C6D60[15].pParent = &D_801C6D60[4];
    func_801C6140(&D_801C6D60[16], &D_801C6BD4[17], 0x01F00016, 0x808080);
    D_801C6D60[16].pParent = &D_801C6D60[5];
    pWk->Bonus = 0;
    if (Flg_ck(G.Unk7868, 0x18)) {
        pWk->Bonus = 100;
    }
    func_801C678C(pWk);
}

/* Mercenaries shop: count the money earned, then buy */
void func_801C3C50(RESULT_WORK *pWk)
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
        D_801C6D60[12].Flg |= RSPR_ON;
        Snd_se_stad(0xD, 0, 0, 0);
        Idx = Gr.Pl_id - 8;
        if (Gr.Merc_money[Idx] < Gr.Score + (SHORT)Gr.Time / 2 + pWk->Bonus) {
            D_801C6D60[6].Flg |= RSPR_ON;
            Gr.Merc_time[Idx] = (SHORT)Gr.Time;
            Gr.Merc_grade[Idx] = pWk->Grade;
            Gr.Merc_money[Idx] = Gr.Score + (SHORT)Gr.Time / 2 + pWk->Bonus;
        }
    case 8:
        func_801C4188(pWk);
        break;
    }
}

/* Mercenaries shop: exit */
void func_801C4070(RESULT_WORK *pWk)
{
    Snd_work.Seq[0].Unk38 = 2;
    Snd_work.Seq[0].Unk3C = 0x3DAF;
    Snd_work.Seq[0].Unk40 = 0x250;
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C3344(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

void func_801C4128(void)
{
    Cd_read(0x46, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* Mercenaries shop: item selection and buying */
void func_801C4188(RESULT_WORK *pWk)
{
    switch (pWk->Routine_2) {
    case 0:
        Mess_set(0xB90000, 0x3402, (ULONG)&D_801C685C[D_801C69A0[pWk->Cursor + 1]], 0);
        pWk->Routine_2 = 1;
    case 1:
        if (G.Unk20FC & 0x8000) {
            if (--pWk->Cursor < 0) {
                pWk->Cursor = 4;
            }
            Moji.Mess_flg = 0;
            Mess_set(0xB90000, 0x3402, (ULONG)&D_801C685C[D_801C69A0[pWk->Cursor + 1]], 0);
            D_801C6D60[12].X = pWk->Cursor * 0x38 + 0x14;
            D_801C6D60[12].Flg |= RSPR_BLINK | RSPR_RESET;
            Snd_se_stad(4, 0, 0, 0);
        }
        if (G.Unk20FC & 0x2000) {
            if (++pWk->Cursor >= 5) {
                pWk->Cursor = 0;
            }
            Moji.Mess_flg = 0;
            Mess_set(0xB90000, 0x3402, (ULONG)&D_801C685C[D_801C69A0[pWk->Cursor + 1]], 0);
            D_801C6D60[12].X = pWk->Cursor * 0x38 + 0x14;
            D_801C6D60[12].Flg |= RSPR_BLINK | RSPR_RESET;
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
            } else if (Flg_ck(Gr.Unk77F8, D_801C6D10[pWk->Cursor])) {
                Moji.Mess_flg = 0;
                Mess_set(0xB90000, 0x3402, (ULONG)&D_801C685C[D_801C69A0[7]], 0);
                Snd_se_stad(7, 0, 0, 0);
            } else if (Gr.Money < D_801C6D08[pWk->Cursor]) {
                Moji.Mess_flg = 0;
                Mess_set(0xB90000, 0x3402, (ULONG)&D_801C685C[D_801C69A0[6]], 0);
                Snd_se_stad(7, 0, 0, 0);
            } else {
                pWk->Routine_2 = 2;
                Moji.Mess_flg = 0;
                Mess_set(0xB90000, 0x3002, (ULONG)&D_801C685C[D_801C69A0[0]], 0);
                Snd_se_stad(6, 0, 0, 0);
            }
        }
        break;
    case 2:
        if (!(Moji.Mess_flg & 0x80)) {
            switch (Gr.Unk7848) {
            case 0:
                pWk->Routine_2 = 3;
                pWk->Count = D_801C6D08[pWk->Cursor];
                pWk->Timer = 0;
                Flg_on(Gr.Unk77F8, D_801C6D10[pWk->Cursor]);
                func_801C64E4(&D_801C6D60[pWk->Cursor + 7], 0x404040);
                D_801C6D60[pWk->Cursor + 21].Flg |= RSPR_ON;
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

/* routine 2 */
void func_801C46EC(void)
{
    RESULT_WORK *pWk = &D_801C76C0;

    D_801C6D14[pWk->Routine_0](pWk);
    func_801C473C(pWk);
}

void func_801C473C(RESULT_WORK *pWk)
{
    tagGLOBAL *pG = &G;

    Print_color(0xFF, 0, 0x102);
    func_801C62B0(&D_801C6D60[0], 7);
    func_801C62B0(&D_801C6D60[1], 7);
    func_801C62B0(&D_801C6D60[2], 7);
    func_801C62B0(&D_801C6D60[3], 7);
    func_801C62B0(&D_801C6D60[4], 7);
    func_801C62B0(&D_801C6D60[5], 7);
    func_801C62B0(&D_801C6D60[6], 7);
    func_801C62B0(&D_801C6D60[7], 7);
    func_801C62B0(&D_801C6D60[8], 7);
    func_801C62B0(&D_801C6D60[9], 7);
    func_801C62B0(&D_801C6D60[10], 7);
    func_801C62B0(&D_801C6D60[11], 7);
    func_801C62B0(&D_801C6D60[12], 6);
    func_801C62B0(&D_801C6D60[13], 7);
    func_801C62B0(&D_801C6D60[14], 7);
    func_801C62B0(&D_801C6D60[15], 7);
    func_801C62B0(&D_801C6D60[16], 7);
    func_801C62B0(&D_801C6D60[21], 2);
    func_801C62B0(&D_801C6D60[22], 2);
    func_801C62B0(&D_801C6D60[23], 2);
    func_801C62B0(&D_801C6D60[24], 2);
    switch (pG->Pl_id) {
    case 8:
        Print14_s(D_801C6BC0[0].X, D_801C6BC0[0].Y, D_801C2010);
        break;
    case 10:
        Print14_s(D_801C6BC0[0].X, D_801C6BC0[0].Y, D_801C2018);
        break;
    case 9:
        Print14_s(D_801C6BC0[0].X, D_801C6BC0[0].Y, D_801C2024);
        break;
    }
    if (pWk->Flg & 2) {
        Print14_s(D_801C6BC0[1].X, D_801C6BC0[1].Y, D_801C202C);
    }
    if (pWk->Flg & 0x10) {
        Print14_s(D_801C6BC0[2].X, D_801C6BC0[2].Y, D_801C2054, D_801C69B4[pWk->Grade]);
    }
    if (pWk->Flg & 4) {
        Print_color(0xFF, 0, 0x4102);
        Print14_s(D_801C6BC0[3].X, D_801C6BC0[3].Y, D_801C2060, Gr.Score / 2);
    }
    Print_color(0xFF, 0, 0x4102);
    Print14_s(D_801C6BC0[4].X, D_801C6BC0[4].Y, D_801C2060, Gr.Money);
}

/* routine 2: init */
void func_801C49FC(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    pWk->Cursor = 0;
    func_801C5324();
    Bg_set_mode(2, 0);
    Cd_read(0x125, (ULONG *)0x801F7E00, 0, "RESULT BGM");
    func_800782F4(0, 0x3D, (ULONG *)0x801F7E00);
    func_800783BC_(5, Cd_read_s(0x126, 2, (ULONG *)0x80180000, 5, "RESULT VB"), 0);
    func_8007809C(0, 0xD, (ULONG *)0x801FBC00, (ULONG *)0x80180000);
    Snd_se_stad(0x500, 0, 0, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    pWk->Flg = 0;
    func_801C6140(&D_801C6D60[0], &D_801C6BD4[0], 0x01EF0015, 0x808080);
    func_801C6140(&D_801C6D60[1], &D_801C6BD4[1], 0x01EF0015, 0x808080);
    D_801C6D60[1].pParent = &D_801C6D60[0];
    func_801C6140(&D_801C6D60[2], &D_801C6BD4[5], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[3], &D_801C6BD4[6], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[4], &D_801C6BD4[7], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[5], &D_801C6BD4[8], 0x01F00016, 0x808080);
    func_801C6140(&D_801C6D60[6], &D_801C6BD4[2], 0x01EF0015, 0x808080);
    D_801C6D60[6].Flg = (D_801C6D60[6].Flg | RSPR_PULSE | RSPR_RESET) & ~RSPR_ON;
    func_801C6140(&D_801C6D60[21], &D_801C6BD4[18], 0x01F10017, 0x808080);
    D_801C6D60[21].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[22], &D_801C6BD4[19], 0x01F10017, 0x808080);
    D_801C6D60[22].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[23], &D_801C6BD4[20], 0x01F10017, 0x808080);
    D_801C6D60[23].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[24], &D_801C6BD4[21], 0x01F10017, 0x808080);
    D_801C6D60[24].Flg &= ~RSPR_ON;
    func_801C6140(&D_801C6D60[7], &D_801C6BD4[10], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 8)) {
        func_801C64E4(&D_801C6D60[7], 0x404040);
        D_801C6D60[21].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[8], &D_801C6BD4[11], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 7)) {
        func_801C64E4(&D_801C6D60[8], 0x404040);
        D_801C6D60[22].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[9], &D_801C6BD4[12], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 6)) {
        func_801C64E4(&D_801C6D60[9], 0x404040);
        D_801C6D60[23].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[10], &D_801C6BD4[13], 0x01F10017, 0x808080);
    if (Flg_ck(G.Unk77F8, 9)) {
        func_801C64E4(&D_801C6D60[10], 0x404040);
        D_801C6D60[24].Flg |= RSPR_ON;
    }
    func_801C6140(&D_801C6D60[11], &D_801C6BD4[14], 0x01F10017, 0x808080);
    func_801C6140(&D_801C6D60[12], &D_801C6BD4[15], 0x01F10017, 0x808080);
    D_801C6D60[12].Flg = (D_801C6D60[12].Flg | RSPR_BLINK | RSPR_RESET) & ~RSPR_ON;
    func_801C6140(&D_801C6D60[13], &D_801C6BD4[16], 0x01F00016, 0x808080);
    D_801C6D60[13].pParent = &D_801C6D60[2];
    func_801C6140(&D_801C6D60[14], &D_801C6BD4[16], 0x01F00016, 0x808080);
    D_801C6D60[14].pParent = &D_801C6D60[3];
    func_801C6140(&D_801C6D60[15], &D_801C6BD4[16], 0x01F00016, 0x808080);
    D_801C6D60[15].pParent = &D_801C6D60[4];
    func_801C6140(&D_801C6D60[16], &D_801C6BD4[17], 0x01F00016, 0x808080);
    D_801C6D60[16].pParent = &D_801C6D60[5];
    func_801C678C(pWk);
}

/* routine 2: count the money earned, then buy */
void func_801C4F0C(RESULT_WORK *pWk)
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
        D_801C6D60[12].Flg |= RSPR_ON;
        Snd_se_stad(0xD, 0, 0, 0);
    case 8:
        func_801C4188(pWk);
        break;
    }
}

/* routine 2: exit */
void func_801C526C(RESULT_WORK *pWk)
{
    Snd_work.Seq[0].Unk38 = 2;
    Snd_work.Seq[0].Unk3C = 0x3DAF;
    Snd_work.Seq[0].Unk40 = 0x250;
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C473C(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

void func_801C5324(void)
{
    Cd_read(0x46, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 3: main game result */
void func_801C5384(void)
{
    RESULT_WORK *pWk = &D_801C76C0;

    D_801C6D20[pWk->Routine_0](pWk);
    func_801C53D4(pWk);
}

/* draws the main game result: time, saves and grade. Returns the last Print14's value (only one path
   returns): that explicit v0 use is what puts the /10 mfhi in t2 (see NOTES.md). */
LONG func_801C53D4(RESULT_WORK *pWk)
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
    Print14_s(D_801C6D2C[1].X, D_801C6D2C[1].Y, "Time");
    Print14_s(D_801C6D2C[2].X, D_801C6D2C[2].Y, D_801C2038, Hour / 10, Hour % 10, Min / 10, Min % 10, Sec / 10, Sec % 10);
    if (pWk->Saves == 1) {
        Print14_s(D_801C6D2C[3].X, D_801C6D2C[3].Y, "Number of Save");
        Print14_s(D_801C6D2C[4].X, D_801C6D2C[4].Y, "%1d%1d", (UCHAR)(pWk->Saves / 10), (UCHAR)(pWk->Saves % 10));
    } else {
        Print14_s(D_801C6D2C[3].X, D_801C6D2C[3].Y, "Number of Saves");
        Print14_s(D_801C6D2C[4].X, D_801C6D2C[4].Y, "%1d%1d", (UCHAR)(pWk->Saves / 10), (UCHAR)(pWk->Saves % 10));
    }
    if (!(G.System_flg & 0x100)) {
        Print14_s(D_801C6D2C[5].X, D_801C6D2C[5].Y, "Grade");
        return Print14_l(D_801C6D2C[6].X, D_801C6D2C[6].Y, D_801C2054, D_801C69B4[pWk->Grade]);
    }
}

/* routine 3: init */
void func_801C567C(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    Cd_read(0x129, (ULONG *)0x801F2600, 0, "RESULT BGM");
    func_800782F4(1, 0x2A, (ULONG *)0x801F2600);
    func_800783BC_(6, Cd_read_s(0x12A, 2, (ULONG *)0x80180000, 6, "RESULT VB"), 0);
    func_801C5854();
    Bg_set_mode(2, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    pWk->Saves = G.Save_cnt;
    func_801C650C(pWk);
}

/* routine 3: wait for a button */
void func_801C577C(RESULT_WORK *pWk)
{
    if (G.Key_trg & 0x3000) {
        pWk->Routine_0 = 2;
    }
    if (G.Unk20FC & 0x800) {
        pWk->Routine_0 = 2;
    }
}

/* routine 3: exit */
void func_801C57BC(RESULT_WORK *pWk)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C53D4(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

void func_801C5854(void)
{
    Cd_read(!Flg_ck(Gr.Unk7868, 0x58) ? 0x50 : 0x4C, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 4 */
void func_801C58CC(void)
{
    RESULT_WORK *pWk = &D_801C76C0;

    D_801C6D48[pWk->Routine_0](pWk);
    func_801C591C(pWk);
}

void func_801C591C(RESULT_WORK *pWk)
{
    Print_color(0xFF, 0, 0x102);
}

/* routine 4: init */
void func_801C5944(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    func_801C5AF4();
    Bg_set_mode(2, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
    Snd_se_stad(0x600, 0, 0, 0);
    Flg_on(G.Unk77F8, 5);
    Moji.Mess_flg = 0;
    Mess_set(0xB90000, 0x3402, (ULONG)&D_801C685C[D_801C69A0[8]], 0);
}

/* routine 4: wait for a button */
void func_801C5A1C(RESULT_WORK *pWk)
{
    if (G.Key_trg & 0x3000) {
        pWk->Routine_0 = 2;
    }
    if (G.Unk20FC & 0x800) {
        pWk->Routine_0 = 2;
    }
}

/* routine 4: exit */
void func_801C5A5C(RESULT_WORK *pWk)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C591C(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

void func_801C5AF4(void)
{
    Cd_read(0x4E, (ULONG *)0x8019C000, 0, "RESULT BG");
    Cd_read(0x48, (ULONG *)0x80100000, 0, "RESULT");
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* routine 5 */
void func_801C5B54(void)
{
    RESULT_WORK *pWk = &D_801C76C0;

    D_801C6D54[pWk->Routine_0](pWk);
    func_801C5BA4(pWk);
}

void func_801C5BA4(RESULT_WORK *pWk)
{
    Print_color(0xFF, 0, 0x102);
}

/* routine 5: init (epilogue) */
void func_801C5BCC(RESULT_WORK *pWk)
{
    Task_suspend(0);
    Main.Vloop = 2;
    Main.Unk191 = 1;
    pWk->pBuf = (ULONG *)0x80100000;
    pWk->Routine_0 = 1;
    func_801C6114();
    Cd_read(0x11F, (ULONG *)0x801F7E00, 0, "RESULT BGM");
    func_800782F4(0, 0x33, (ULONG *)0x801F7E00);
    func_800783BC_(5, Cd_read_s(0x120, 2, (ULONG *)0x80180000, 5, "RESULT VB"), 0);
    Snd_se_stad(0x500, 0, 0, 0);
    Bg_set_mode(2, 0);
    Fade_set(1, 5, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Flg_on(G.Unk77F8, 5);
}

/* routine 5: "EPILOGUE" title, then the epilogue pictures */
void func_801C5CD8(RESULT_WORK *pWk)
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
        func_801C2154((ULONG *)0x8019C000, 8);
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
void func_801C605C(RESULT_WORK *pWk)
{
    Snd_work.Seq[0].Unk38 = 2;
    Snd_work.Seq[0].Unk3C = 0x3DAF;
    Snd_work.Seq[0].Unk40 = 0x250;
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x1E);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C591C(pWk);
        Task_sleep(1);
    }
    Moji.Mess_flg = 0;
    Main.Vloop = pWk->Vloop;
    Task_signal(0);
    Task_exit();
}

void func_801C6114(void)
{
    func_801C2154((ULONG *)0x8019C000, G.Unk7806);
}

/* sets up a sprite object; Tp_clut: clut row << 16 | texture page */
void func_801C6140(RESULT_SPR *pSpr, RESULT_SPR_DATA *pData, ULONG Tp_clut, ULONG Rgb)
{
    RESULT_SPRT *pSprt;
    DR_MODE *pDm;
    ULONG Tp;
    LONG Lower;
    LONG Tx;

    pSpr->Flg = RSPR_ON;
    pSpr->Timer = 0;
    pSpr->pParent = NULL;
    pSpr->X = pData->X;
    pSpr->Y = pData->Y;
    pSpr->Ofs_x = pData->Ofs_x;
    pSpr->Ofs_y = pData->Ofs_y;
    pSpr->Mode = 0xFF;
    pSprt = &pSpr->Sprt[0];
    pDm = &pSpr->Dm[0];
    pSprt->W.Rgbc = Rgb;
    pSprt->Prim.x0 = pData->X;
    pSprt->Prim.y0 = pData->Y;
    pSprt->Prim.w = pData->W;
    pSprt->Prim.h = pData->H;
    pSprt->Prim.u0 = pData->U;
    pSprt->Prim.v0 = pData->V;
    pSprt->Prim.clut = (Tp_clut >> 16) << 6;
    SetSprt(&pSprt->Prim);
    SetSemiTrans(&pSprt->Prim, pData->Semi);
    Tp = Tp_clut & 0xFFFF;
    Lower = Tp < 16;
    Tx = Tp << 6;
    if (!Lower) {
        Tx -= 0x400;
    }
    SetDrawMode(pDm, 0, 0, GetTPage(1, pData->Abr, Tx, (Tp >= 16) << 8), NULL);
    pSpr->Sprt[1] = pSpr->Sprt[0];
    pSpr->Dm[1] = pSpr->Dm[0];
}

/* updates the fade / blink state of a sprite object and adds it to OT entry Pri */
void func_801C62B0(RESULT_SPR *pSpr, ULONG Pri)
{
    RESULT_SPR *pParent;
    SPRT *pSprt;
    DR_MODE *pDm;

    if (!(pSpr->Flg & RSPR_ON)) {
        return;
    }
    if (pSpr->Flg & RSPR_RESET) {
        pSpr->Flg &= ~RSPR_RESET;
        if (pSpr->Flg & RSPR_PULSE) {
            pSpr->Mode = 0;
        }
        if (pSpr->Flg & RSPR_BLINK) {
            pSpr->Mode = 2;
            pSpr->Timer = 10;
        }
    }
    switch (pSpr->Mode) {
    case 0:
        pSpr->Sprt[0].W.Rgbc += 0x80808;
        pSpr->Sprt[1].W.Rgbc += 0x80808;
        if (pSpr->Sprt[1].Prim.r0 == 0xB0) {
            pSpr->Mode = 1;
        }
        break;
    case 1:
        pSpr->Sprt[0].W.Rgbc -= 0x80808;
        pSpr->Sprt[1].W.Rgbc -= 0x80808;
        if (pSpr->Sprt[1].Prim.r0 == 0x50) {
            pSpr->Mode = 0;
        }
        break;
    case 2:
        if (--pSpr->Timer == 0xFF) {
            pSpr->Timer = 10;
            pSpr->Mode = 3;
        }
        break;
    case 3:
        if (--pSpr->Timer == 0xFF) {
            pSpr->Timer = 10;
            pSpr->Mode = 2;
        }
        return;
    }
    pSprt = &pSpr->Sprt[G.Cbuf_id].Prim;
    pDm = &pSpr->Dm[G.Cbuf_id];
    pParent = pSpr->pParent;
    if (pParent != NULL) {
        pSpr->X = pSpr->Ofs_x + (pParent->X + pParent->Sprt[0].Prim.w);
        pSpr->Y = pParent->Y + pSpr->Ofs_y;
    }
    pSprt->x0 = pSpr->X;
    pSprt->y0 = pSpr->Y;
    AddPrim(&G.Ot0[G.Cbuf_id][Pri], pSprt);
    AddPrim(&G.Ot0[G.Cbuf_id][Pri], pDm);
}

void func_801C64E4(RESULT_SPR *pSpr, ULONG Rgb)
{
    pSpr->Sprt[0].W.Rgbc = (pSpr->Sprt[0].W.Rgbc & 0xFF000000) | Rgb;
    pSpr->Sprt[1].W.Rgbc = (pSpr->Sprt[1].W.Rgbc & 0xFF000000) | Rgb;
}

/* main game grade from the number of saves, the play time and Unk7E94 */
void func_801C650C(RESULT_WORK *pWk)
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
void func_801C678C(RESULT_WORK *pWk)
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

