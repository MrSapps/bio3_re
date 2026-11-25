#include "ovl/ending.h"
#include "status.h"

/* task entry */
void Ending_main(void)
{
    Print_color(1, 0, 1);
    Ending_status_clr();
    if (G.System_flg & 0x80) {
        Ending_load();
    } else {
        Ending_game();
    }
    G.System_flg |= 0x200000;
    Task_exit();
}

void Ending_status_clr(void)
{
    G.Status_flg &= 0xFFFF0000;
}

void Ending_game(void)
{
    G.Game_time = G.Game_time + VSync(-1) - G.Game_time_offset;
    Ending_movie();
    Task_lexecute(1, 0xB);
    Task_sleep(1);
    Moji.Mess_flg = 0;
    G.Unk2115 = 3;
    Task_lexecute(1, 8);
    Task_sleep(1);
    if (!Flg_ck(G.Unk77F8, 5)) {
        G.Unk2115 = 4;
        Task_lexecute(1, 8);
        Task_sleep(1);
    }
    if (!Flg_ck(G.Unk77F8, 1) || !Flg_ck(G.Unk77F8, 2) || !Flg_ck(G.Unk77F8, 3) ||
        !Flg_ck(G.Unk77F8, 4) || !Flg_ck(G.Unk77F8, 0)) {
        if (G.Save_cnt != 0 && (!(G.System_flg & 0x100) || !Flg_ck(G.Unk77F8, 10))) {
            Task_lexecute(1, 7);
            Task_sleep(1);
        }
    }
    if (!(G.System_flg & 0x100) && G.Unk7806 < 8) {
        G.Unk2115 = 5;
        Task_lexecute(1, 8);
        Task_sleep(1);
    }
    Fade_set(0, 0, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Ending_next_game();
    Fade_set(0, 0, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    func_800792FC();
    G.System_flg |= 0x4000;
    Task_lexecute(1, 2);
    Task_sleep(1);
    G.System_flg &= ~0x4000;
    G.System_flg &= ~0x40000;
}

void Ending_movie(void)
{
    if (!Flg_ck(G.Unk7868, 0x58)) {
        Init_movie_work(10);
        while (G.System_flg & 0x10000) {
            Task_sleep(1);
        }
    } else {
        Init_movie_work(11);
        while (G.System_flg & 0x10000) {
            Task_sleep(1);
        }
    }
}

void Ending_load(void)
{
    G.Unk2115 = 1;
    Task_lexecute(1, 8);
    Task_sleep(1);
    func_800791F0();
}

void Ending_next_game(void)
{
    G.Save_cnt = 0xFF;
    G.Save_area = 0x1C;
    G.Next_cut_no = G.Cut_no;
    func_800790F0((ULONG *)0x80100000);
    G.Unk7806++;
}
