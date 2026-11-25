#include "monitor.h"
#include "snd.h"
#include "cd.h"
#include "ovl/mem_card.h"
#include "sce1.h"
#include "sld.h"

LONG Search_item_id_(LONG Id, LONG i) __asm__("Search_item_id"); /* returns long here (canonical signed char); arg 1 long (canonical UC); arg 2 long (canonical signed char) */
LONG func_8006CCF0_(LONG No, LONG N, LONG F) __asm__("Item_decrease"); /* returns long here (canonical signed char); arg 1 long (canonical UC); arg 2 long (canonical signed char); arg 3 long (canonical UC) */
#include <strings.h>

#define MESS(n) (&Mess_text[Mess_offs[n]])

/* Print14 called with int coordinates (no USHORT truncation in the caller) */
void Print14_l(LONG X, LONG Y, UCHAR *pFmt, ...) __asm__("Print14"); /* arg 1 long (canonical unsigned short); arg 2 long (canonical unsigned short) */

/* Mem_card: task entry */
void Mem_card(void)
{
    if (Task_status(0) == 1) {
        Task_suspend(0);
        Card_access();
        Task_signal(0);
        Task_exit();
    }
}

/* Card_access: main loop of the card screen */
void Card_access(void)
{
    MWORK *pMw = pMwork;

    Cardaccess_init(pMw);
    pMw->Saved_flg = 0;
    for (;;) {
        Print_color(0xFF, 0, 0);
        pMw->Poll_flg = 0;
        pMw->Key_trg = G.Key_trg;
        pMw->Key_rep = G.Unk20FC;
        Card_cursor(pMw);
        do {
            pMw->Rno_loop = 0;
            Card_rno_tbl[pMw->Rno](pMw);
        } while (pMw->Rno_loop);
        if (pMw->Exit_flg) {
            if (!(G.System_flg & 0x4000) || pMw->Load_flg || pMw->Saved_flg) {
                break;
            }
            pMw->Rno = 0x10;
            pMw->Exit_flg = 0;
            pMw->Cursor_old = 1;
            pMw->Cursor = 1;
        }
        Card_disp_tbl[pMw->Rno](pMw);
        pMw->Card_chg = 0;
        if (pMw->Poll_flg) {
            if (Get_card_event() == 1) {
                pMw->Card_chg = 1;
            }
        }
        G.Key_trg = pMw->Key_trg;
        G.Unk20FC = pMw->Key_rep;
        Task_sleep(1);
    }
    Cardaccess_exit(pMw);
}

/* Card_cursor: move the cursor with up/down, with key repeat */
LONG Card_cursor(MWORK *pMw)
{
    if (pMw->Key_lock == 0 && pMw->Cursor_max != 0) {
        if ((USHORT)G.Unk20F8 & 0x5000) {
            if (G.Unk20FC & 0x5000) {
                pMw->Key_wait = 10;
            } else {
                if (pMw->Key_wait > 0) {
                    goto dec;
                }
                pMw->Key_wait = 2;
            }
            if ((USHORT)G.Unk20F8 & 0x1000) {
                pMw->Cursor--;
            }
            if ((USHORT)G.Unk20F8 & 0x4000) {
                pMw->Cursor++;
            }
            switch (pMw->Rno) {
            case 6:
                if (pMw->Cursor == -1) {
                    if (G.Unk20FC & 0x1000) {
                        pMw->Cursor = 14;
                        pMw->Scrl_top = 10;
                    } else {
                        pMw->Cursor = 0;
                        pMw->Scrl_top = 0;
                    }
                }
                if (pMw->Cursor == 15) {
                    if (G.Unk20FC & 0x4000) {
                        pMw->Cursor = 0;
                        pMw->Scrl_top = 0;
                    } else {
                        pMw->Cursor = 14;
                        pMw->Scrl_top = 10;
                    }
                }
                if (pMw->Cursor < pMw->Scrl_top) {
                    pMw->Scrl_top--;
                }
                if (pMw->Scrl_top + 4 < pMw->Cursor) {
                    pMw->Scrl_top++;
                }
                break;
            case 17:
                if (pMw->Cursor < 0) {
                    pMw->Cursor = pMw->Cursor_max;
                }
                if (pMw->Cursor > pMw->Cursor_max) {
                    pMw->Cursor = 0;
                }
                break;
            default:
                pMw->Cursor = (pMw->Cursor < 0) ? 0 : (pMw->Cursor > pMw->Cursor_max) ? pMw->Cursor_max : pMw->Cursor;
                break;
            }
            if (pMw->Cursor != pMw->Cursor_old) {
                G.Key_trg &= ~0x1000;
                G.Unk20FC &= ~0x800;
                pMw->Blink = 0;
                pMw->Cursor_old = pMw->Cursor;
                Snd_se_stad(4, 0, 0, 0);
            }
            goto end;
        dec:
            pMw->Key_wait--;
        }
    end:
        pMw->Blink--;
    }
}

/* Cardaccess_init */
void Cardaccess_init(MWORK *pMw)
{
    Memclr(pMw, sizeof(MWORK));
    if (G.System_flg & 0x40000) {
        pMw->Load_flg = 1;
    } else {
        pMw->Load_flg = 0;
    }
    pMw->Vloop_bak = Main.Vloop;
    Main.Vloop = 2;
    Cd_read(0x68, (ULONG *)0x8019C000, 0, "MEMORY CARD BG");
    Main.Unk191 = 0;
    Bg_set_mode(2, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 12);
    while (Fade_status(0) != 0) {
        Task_sleep(1);
    }
    pMw->Key_lock = 1;
    pMw->Exit_flg = 0;
    pMw->Card_chg = 0;
    pMw->Rno = 0;
    pMw->Rno1 = 0;
}

/* Cardaccess_exit */
void Cardaccess_exit(MWORK *pMw)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 12);
    while (!(Fade_status(0) & FADE_END)) {
        Task_sleep(1);
    }
    Main.Vloop = pMw->Vloop_bak;
    if (pMw->Load_flg == 0 && !(G.System_flg & 0x4000)) {
        Main.Unk18E = 2;
    }
}

/* Rno 0: check both card slots */
void Card_rno00_slot_check(MWORK *pMw)
{
    LONG Ret0, Ret1;

    switch (pMw->Rno1) {
    case 0:
        pMw->Wait = 6;
        pMw->Rno1 = 1;
    case 1:
        if (--pMw->Wait == 0) {
            Ret0 = Card_check(0);
            Ret1 = Card_check(1);
            if (Ret0 == 3 || Ret1 == 3) {
                pMw->Rno = 3;
                pMw->Err = 1;
            } else if (Ret0 != 0 && Ret1 != 0) {
                pMw->Port = 0;
                pMw->Rno = 3;
                pMw->Err = 4;
            } else if (Ret0 != 0 || Ret1 != 0) {
                pMw->Port = Ret0;
                pMw->Rno = 4;
            } else {
                pMw->Rno = 2;
                pMw->Key_lock = 0;
                pMw->Cursor = 0;
            }
        }
        break;
    }
}

/* Rno 1 */
void Card_rno01(MWORK *pMw)
{
    ULONG Key;

    pMw->Cursor_max = 1;
    Key = G.Key_trg;
    if (Key & 0x1000) {
        Snd_se_stad(6, 0, 0, 0);
        if (pMw->Cursor != 1) {
            pMw->Rno = 0;
            pMw->Rno1 = 0;
            pMw->Key_lock = 1;
            pMw->Cursor = 0;
            return;
        }
    } else if (Key & 0x2000) {
        Snd_se_stad(5, 0, 0, 0);
    } else {
        return;
    }
    pMw->Exit_flg = 1;
}

/* Rno 2: select the card slot */
void Card_rno02_slot_sel(MWORK *pMw)
{
    pMw->Cursor_max = 2;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        Snd_se_stad(6, 0, 0, 0);
        {
            signed char c = pMw->Cursor;
            if (c != 2) {
                pMw->Key_lock = 1;
                pMw->Port = c;
                pMw->Rno = 4;
            } else {
                pMw->Exit_flg = 1;
            }
        }
    } else if (G.Key_trg & 0x2000) {
        Snd_se_stad(5, 0, 0, 0);
        pMw->Exit_flg = 1;
    }
}

/* Rno 3: error message */
void Card_rno03_err(MWORK *pMw)
{
    pMw->Cursor_max = 0;
    pMw->Cursor = 0;
    if ((G.Key_trg & 0x3000) || (G.Unk20FC & 0x800)) {
        Snd_se_stad(5, 0, 0, 0);
        pMw->Rno = 1;
        pMw->Key_lock = 0;
        pMw->Cursor = 0;
    }
}

/* Rno 4: "now checking" */
void Card_rno04_checking(MWORK *pMw)
{
    Mess_print(Mess_center_x(0x1A, 1), 0xB2, MESS(9), 0x102);
    Mess_print(Mess_center_x(0x1A, 1), 0xC0, MESS(26), 0x102);
    Task_sleep(1);
    pMw->Rno = 5;
}

/* Rno 5: read the card directory and the save summaries */
void Card_rno05_read_dir(MWORK *pMw)
{
    pMw->Cursor_max = 0;
    pMw->Key_lock = 0;
    if ((pMw->Err = Card_check(pMw->Port)) != 0) {
        switch (pMw->Err) {
        case 1:
        case 2:
            pMw->Rno = 3;
            pMw->Err = 4;
            break;
        case 3:
            pMw->Rno = 3;
            pMw->Err = 1;
            break;
        }
        return;
    }
    pMw->Err = Card_format_check(pMw->Port);
    Memclr(&pMw->File_info, sizeof(CFILE_INFO));
    if (pMw->Err == 0) {
        pMw->Err = Check_card(pMw->F_num);
        pMw->Total_size = pMw->File_info.Total_size;
    }
    switch (pMw->Err) {
    case 0:
        if (pMw->Load_flg == 1 && pMw->File_info.Game_num == 0) {
            pMw->Err = 3;
            pMw->Rno = 3;
            break;
        }
        if (pMw->Load_flg == 0 && pMw->File_info.Game_num == 0 && pMw->Total_size == 0x1E000) {
            pMw->Err = 2;
            pMw->Rno = 3;
            break;
        }
        pMw->Cursor = pMw->Cursor_old = G.Save_slot;
        if (pMw->Cursor >= 5) {
            pMw->Scrl_top = pMw->Cursor - 4;
        } else {
            pMw->Scrl_top = 0;
        }
        pMw->Blink = 0;
        pMw->Rno = 6;
        break;
    case 1:
        pMw->Rno = 3;
        pMw->Err = 4;
        break;
    case 2:
        if (pMw->Load_flg == 1) {
            pMw->Rno = 3;
            pMw->Err = 9;
        } else {
            pMw->Cursor = 1;
            pMw->Rno = 12;
        }
        break;
    case 3:
        pMw->Rno = 3;
        pMw->Err = 1;
        break;
    }
}

/* Rno 6: select a save slot */
void Card_rno06_file_sel(MWORK *pMw)
{
    signed char Cursor;

    pMw->Cursor_max = 14;
    G.Save_slot = pMw->Cursor;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        Cursor = pMw->Cursor;
        if (!(pMw->F_num[Cursor] & 0x40)) {
            pMw->Slot = Cursor;
            Snd_se_stad(6, 0, 0, 0);
            Card_search(pMw->Port, &Mc_file_info, "BASLUS-00923*");
            if (Mc_file_info.Num == 0) {
                VSync(0);
                if ((pMw->Err = Card_check(pMw->Port)) != 0) {
                    pMw->Rno = 3;
                    pMw->Err = 4;
                    return;
                }
            }
            pMw->Key_lock = 1;
            pMw->Rno = 7;
            return;
        }
    } else if (G.Key_trg & 0x2000) {
        Snd_se_stad(5, 0, 0, 0);
        pMw->Blink = 0;
        if (Card_check(pMw->Port ^ 1) != 0) {
            pMw->Exit_flg = 1;
        } else {
            pMw->Cursor = 0;
            pMw->Rno = 0;
            pMw->Rno1 = 0;
        }
        return;
    }
    if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 17: menu after saving */
void Card_rno11_after_save(MWORK *pMw)
{
    if (Flg_ck(G.Unk77F8, 0x1F)) {
        pMw->Cursor_max = 3;
    } else {
        pMw->Cursor_max = 4;
    }
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        Snd_se_stad(6, 0, 0, 0);
        G.System_flg &= ~0x40000;
        G.System_flg &= ~0x20;
        switch (pMw->Cursor) {
        case 0:
            if (G.Save_area == 0x1C) {
                G.System_flg |= 0x20;
            }
            break;
        case 1:
            G.System_flg |= 0x20;
            break;
        case 2:
            G.System_flg |= 0x80;
            Memcpy(D_800C7048.Save, G.Card_head, sizeof(D_800C7048.Save));
            D_800C7048.Pl_id = G.Player.Id;
            D_800C7048.Pos_x = G.Player.Pos_x;
            D_800C7048.Pos_y = G.Player.Pos_y;
            D_800C7048.Pos_z = G.Player.Pos_z;
            D_800C7048.Cdir_y = G.Player.Cdir_y;
            D_800C7048.Life = G.Player.Life;
            if (G.Player.Status_flg & 0x200) {
                D_800C7048.Poison = G.Pl_poison_timer | 0x80;
            } else {
                D_800C7048.Poison = 0;
            }
            break;
        case 3:
            if (Flg_ck(G.Unk77F8, 0x1F) == 0) {
                G.Unk2115 = 1;
                break;
            }
        case 4:
            goto cancel;
        }
        pMw->Err = 0;
        pMw->Exit_flg = 1;
    } else if (G.Key_trg & 0x2000) {
        Snd_se_stad(5, 0, 0, 0);
    cancel:
        G.System_flg |= 0x40000;
        pMw->Blink = 0;
        pMw->Cursor = pMw->Cursor_old = G.Save_slot;
        pMw->Rno = 6;
        pMw->Rno1 = 0;
    } else if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 7: slot chosen */
void Card_rno07_file_ok(MWORK *pMw)
{
    ULONG i;
    signed char Cursor;

    pMw->Cursor = pMw->Slot;
    if (pMw->Load_flg == 0) {
        strcpy(Mc_old_name, Mc_file_info.Title[pMw->Cursor]);
        for (i = 0; i < 22; i++) {
            Mc_file_info.Title[pMw->Cursor][i] = 0;
        }
        __builtin_memcpy(Mc_file_info.Title[pMw->Cursor], "BASLUS-0092300", 15);
        Mc_file_info.Title[pMw->Cursor][12] += pMw->Cursor / 10;
        Mc_file_info.Title[pMw->Cursor][13] += pMw->Cursor % 10;
        Mc_file_info.Title[pMw->Cursor][14] = 0;
        Cursor = pMw->Cursor;
        if (pMw->F_num[Cursor] < 15) {
            pMw->Cursor = pMw->Cursor_old = 1;
            pMw->Cursor_bak = Cursor;
            pMw->Key_lock = 0;
            pMw->Rno = 15;
            return;
        }
        if (pMw->Total_size == 0x1E000) {
            pMw->Err = 2;
            pMw->Rno = 14;
            pMw->Cursor_bak = pMw->Cursor;
            return;
        }
        pMw->Rno = 8;
        Mess_print(Mess_center_x(0x1A, 1), 0xB2, MESS(25), 0x102);
        Mess_print(Mess_center_x(0x1A, 1), 0xC0, MESS(26), 0x102);
        pMw->Timer = 30;
    } else {
        if (pMw->F_num[pMw->Cursor] & 0xC0) {
            pMw->Err = 3;
            pMw->Rno = 14;
            pMw->Key_lock = 0;
            pMw->Cursor_bak = pMw->Cursor;
        } else {
            Mess_print(Mess_center_x(0x1A, 1), 0xBA, MESS(19), 0x102);
            Mess_print(Mess_center_x(0x1A, 1), 0xCA, MESS(26), 0x102);
            pMw->Timer = 30;
            pMw->Rno = 19;
        }
    }
    if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 8: "saving" message before the write */
void Card_rno08_save_wait(MWORK *pMw)
{
    Mess_print(Mess_center_x(0x1A, 1), 0xB2, MESS(25), 0x102);
    Mess_print(Mess_center_x(0x1A, 1), 0xC0, MESS(26), 0x102);
    if (--pMw->Timer == 0xFF) {
        pMw->Rno = 9;
    }
    if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 19: "loading" message before the read */
void Card_rno13_load_wait(MWORK *pMw)
{
    Mess_print(Mess_center_x(0x1A, 1), 0xBA, MESS(19), 0x102);
    Mess_print(Mess_center_x(0x1A, 1), 0xCA, MESS(26), 0x102);
    if (--pMw->Timer == 0xFF) {
        pMw->Rno = 11;
    }
    if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 9: write the save file */
void Card_rno09_save(MWORK *pMw)
{
    UCHAR Save_cnt;

    pMw->Cursor = pMw->Slot;
    Memcpy(Mc_item_bak, G.pItem_set, sizeof(Mc_item_bak));
    pMw->Item_bak = G.pItem_set->Equip_no;
    if (!(G.System_flg & 0x4080)) {
        func_8006CCF0_(Search_item_id_(0x81, 0), 1, 0);
    }
    Save_cnt = G.Save_cnt;
    if ((Save_cnt < 99 || (G.System_flg & 0x4000)) && !(G.System_flg & 0x80)) {
        G.Save_cnt = Save_cnt + 1;
    }
    Save_push();
    Memcpy(&pMw->Save_info[pMw->Cursor], &G.Game_time, sizeof(SAVE_INFO));
    Card_mess_make(Mc_title, G.Card_head, pMw->Cursor);
    pMw->Err = Card_write(G.Card_head, pMw->Port, Mc_file_info.Title[pMw->Cursor], Mc_title, 0x980);
    Mess_print(Mess_center_x(0x1A, 1), 0xB2, MESS(25), 0x102);
    Mess_print(Mess_center_x(0x1A, 1), 0xC0, MESS(26), 0x102);
    pMw->Rno = 9;
    if (pMw->Err == 0) {
        pMw->F_num[pMw->Cursor] = 0;
        pMw->Moji_no = -1;
        pMw->Anim_step = 0;
        pMw->Anim_wait = Mc_wait_tbl[0];
        pMw->Rno = 10;
        pMw->Anim_mode = 0;
        pMw->Rno_loop = 1;
        pMw->Saved_flg = 1;
    } else {
        if (!(G.System_flg & 0x80)) {
            G.Save_cnt = Save_cnt;
        }
        Memcpy(G.pItem_set, Mc_item_bak, sizeof(Mc_item_bak));
        G.pItem_set->Equip_no = pMw->Item_bak;
        pMw->Err = 5;
        pMw->Key_lock = 0;
        pMw->Rno = 3;
    }
}

/* Rno 10: "saved" animation of the new slot title */
void Card_rno0A_saved(MWORK *pMw)
{
    USHORT *pOfs = Mess_offs;
    UCHAR *pText = Mess_text;

    Mess_print(Mess_center_x(0x1A, 1), 0xB2, &pText[pOfs[25]], 0x102);
    Mess_print(Mess_center_x(0x1A, 1), 0xC0, &pText[pOfs[26]], 0x102);
    if (--pMw->Anim_wait == 0) {
        if (pMw->Anim_mode == 1) {
            pMw->Anim_mode = 2;
            pMw->Moji_no = 0;
        } else {
            pMw->Moji_no++;
        }
        Mc_moji_se = 0;
        if (pMw->Moji_no < 0) {
            pMw->Err = 0;
            if (G.System_flg & 0x80) {
                pMw->Rno = 18;
            } else {
                pMw->Exit_flg = 1;
            }
        } else {
            pMw->Anim_step++;
            pMw->Anim_wait = Mc_wait_tbl[pMw->Anim_step];
        }
    }
}

/* Rno 11: read the save file */
void Card_rno0B_load(MWORK *pMw)
{
    pMw->Cursor = pMw->Slot;
    if ((pMw->Err = Card_read(pMw->Card_buf, pMw->File_info.Title[pMw->F_num[pMw->Cursor]], sizeof(pMw->Card_buf), 0)) != 0) {
        pMw->Err = 6;
        pMw->Rno = 14;
        pMw->Key_lock = 0;
        pMw->Cursor_bak = pMw->Cursor;
        return;
    }
    Memcpy(G.Card_head, pMw->Card_buf, 0x908);
    Load_pop();
    if (Flg_ck(G.Unk77F8, 5)) {
        pMw->Key_lock = 0;
        pMw->Cursor = pMw->Cursor_old = 0;
        pMw->Rno = 17;
    } else {
        G.System_flg &= ~0x40000;
        pMw->Err = 0;
        pMw->Exit_flg = 1;
    }
}

/* Rno 12: "format the card?" */
void Card_rno0C_format_ask(MWORK *pMw)
{
    pMw->Cursor_max = 1;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        if (pMw->Cursor != 0) {
            Snd_se_stad(5, 0, 0, 0);
            pMw->Cursor = 0;
            pMw->Rno = 1;
        } else {
            Snd_se_stad(6, 0, 0, 0);
            pMw->Rno = 13;
        }
        return;
    }
    if (G.Key_trg & 0x2000) {
        Snd_se_stad(5, 0, 0, 0);
        pMw->Cursor = 0;
        pMw->Rno = 1;
    }
    if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 13: format the card */
void Card_rno0D_format(MWORK *pMw)
{
    if ((pMw->Err = Card_check(pMw->Port)) == 0) {
        pMw->Err = Card_format_check(pMw->Port);
        if (pMw->Err == 2 && Card_format(pMw->Port) == 0) {
            pMw->Rno = 4;
            G.Save_slot = 0;
            pMw->Key_lock = 1;
            return;
        }
        pMw->Err = 0;
        pMw->Rno = 3;
    } else {
        if (pMw->Err == 1) {
            pMw->Err = 4;
        } else {
            pMw->Err = 0;
        }
        pMw->Cursor = 0;
        pMw->Rno = 3;
    }
}

/* Rno 14: error message for the selected slot */
void Card_rno0E_file_err(MWORK *pMw)
{
    pMw->Cursor_max = 0;
    pMw->Cursor = 0;
    if ((G.Key_trg & 0x3000) || (G.Unk20FC & 0x800)) {
        pMw->Key_lock = 0;
        Snd_se_stad(4, 0, 0, 0);
        if (pMw->Err == 2) {
            pMw->Cursor = 0;
            pMw->Rno = 1;
        } else {
            pMw->Rno = 6;
            pMw->Blink = 0;
            pMw->Cursor_max = pMw->Cursor = pMw->Cursor_bak;
        }
    }
    Mess_print(Mess_center_x(pMw->Err + 0x21, 1), 0xC4, MESS(pMw->Err + 0x21), 0x112);
    if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 15: "overwrite this file?" */
void Card_rno0F_overwrite(MWORK *pMw)
{
    pMw->Cursor_max = 1;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        if (pMw->Cursor == 0) {
            Snd_se_stad(6, 0, 0, 0);
            pMw->Rno = 8;
            pMw->Blink = 0;
            pMw->Key_lock = 1;
            pMw->Timer = 30;
        } else {
            Snd_se_stad(5, 0, 0, 0);
            pMw->Rno = 6;
            pMw->Blink = 0;
        }
        pMw->Cursor_max = pMw->Cursor = pMw->Cursor_bak;
        return;
    }
    if (G.Key_trg & 0x2000) {
        Snd_se_stad(5, 0, 0, 0);
        pMw->Rno = 6;
        pMw->Blink = 0;
        pMw->Cursor_max = pMw->Cursor = pMw->Cursor_bak;
    }
    if (Card_poll(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 16: "continue the game?" after a save on the way out */
void Card_rno10_continue(MWORK *pMw)
{
    pMw->Cursor_max = 1;
    if (G.Key_trg & 0x1000) {
        if (pMw->Cursor == 0) {
            Snd_se_stad(6, 0, 0, 0);
            pMw->Saved_flg = 1;
            pMw->Exit_flg = 1;
            return;
        }
        pMw->Cursor = 0;
        Snd_se_stad(6, 0, 0, 0);
        pMw->Rno = 0;
        pMw->Rno1 = 0;
    }
    if (G.Key_trg & 0x2000) {
        Snd_se_stad(5, 0, 0, 0);
        pMw->Rno = 0;
        pMw->Rno1 = 0;
    }
}

/* Rno 18: wait for a key, then leave */
void Card_rno12_end(MWORK *pMw)
{
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800) || (G.Key_trg & 0x2000)) {
        pMw->Exit_flg = 1;
    }
}

/* Disp: nothing */
void Card_disp_none(MWORK *pMw) {
}

/* Disp 0: "now checking" */
void Card_disp_checking(MWORK *pMw)
{
    Mess_print(Mess_center_x(0x1A, 1), 0xB2, MESS(9), 0x102);
}

/* Disp 6..11, 14, 15, 19: list of the save slots */
void Card_disp_file_list(MWORK *pMw)
{
    RECT Rc;
    LONG Unused[4];
    ULONG i;
    LONG Slot;
    LONG Col;
    LONG Last;
    SAVE_INFO *pInfo;
    UCHAR *pStr;

    for (i = 0; i < 5; i++) {
        Slot = pMw->Scrl_top + i;
        Print_color(0xFF, 0, 0);
        Print14_l(0x1D, i * 0x14 + 0x38, "%2d%c", Slot + 1, '%');
        if (pMw->F_num[Slot] < 15) {
            pInfo = &pMw->Save_info[Slot];
            if (pInfo->Unk16 < 100) {
                func_8001012C(Mc_slot_line2, MESS(93), 10);
                if (pInfo->Unk16 == 0) {
                    Mc_slot_line2[3] = 0xFE;
                } else {
                    Mc_slot_line2[4] += pInfo->Unk16 / 10;
                    Mc_slot_line2[5] += pInfo->Unk16 % 10;
                }
            } else {
                func_8001012C(Mc_slot_line2, MESS(94), 10);
            }
            Mc_slot_line2[1] += pInfo->Save_cnt / 10;
            Mc_slot_line2[2] += pInfo->Save_cnt % 10;
            func_8001012C(Mc_slot_line, MESS(pInfo->Save_area + 0x3F), 0x18);
            Col = (Flg_ck(&pInfo->Flg, 0x1F) != 0) << 3;
            if (pMw->Rno == 10 && Slot == pMw->Cursor) {
                Last = 0;
                pStr = Mc_slot_line;
                if (pMw->Anim_mode) {
                    pStr = Mc_slot_line2;
                }
                if (pMw->Moji_no >= 0 && (UCHAR)(pStr[pMw->Moji_no] - 0xEA) < 7 && Mc_moji_se == 0) {
                    pMw->Moji_no++;
                    Mc_moji_se = 1;
                }
                if (pMw->Anim_wait == 1 && pStr[pMw->Moji_no + 1] == 0xFE) {
                    if (pMw->Anim_mode == 0) {
                        pMw->Anim_mode = 1;
                    } else {
                        Last = 1;
                    }
                }
                if (pMw->Moji_no >= 0) {
                    if (pMw->Moji_no_old != pMw->Moji_no) {
                        if (pStr[pMw->Moji_no]) {
                            Snd_se_stad(0x222, 0, 0, 0);
                        } else {
                            Snd_se_stad(0x224, 0, 0, 0);
                            pMw->Anim_wait = 10;
                        }
                    }
                    pMw->Moji_no_old = pMw->Moji_no;
                    pStr[pMw->Moji_no + 1] = 0xFE;
                } else {
                    pStr[0] = 0xFE;
                }
                if (pMw->Moji_no >= 0) {
                    Mess_print(0x45, i * 0x14 + 0x38, Mc_slot_line, (Col << 3) | 0x102);
                    if (pMw->Anim_mode == 2) {
                        Mess_print(0xD9, i * 0x14 + 0x38, Mc_slot_line2, 0x102);
                    }
                }
                if (Last) {
                    pMw->Moji_no = -2;
                }
            } else {
                Mess_print(0x45, i * 0x14 + 0x38, Mc_slot_line, (Col << 3) | 0x102);
                Mess_print(0xD9, i * 0x14 + 0x38, Mc_slot_line2, 0x102);
            }
        } else if (pMw->F_num[Slot] & 0x80) {
            Mess_print(0x45, i * 0x14 + 0x38, MESS(104), 0x102);
        } else {
            Mess_print(0x45, i * 0x14 + 0x38, MESS(105), 0x132);
        }
    }
    Slot_disp(pMw->Port);
    if (--pMw->Wait & 0x18) {
        Mc_slot_line[1] = 0xFE;
        if (pMw->Scrl_top != 0) {
            Mc_slot_line[0] = 0x19;
            Mess_print(0x99, 0x2F, Mc_slot_line, 0x102);
        }
        if (pMw->Scrl_top < 10) {
            Mc_slot_line[0] = 0x0B;
            Mess_print(0x99, 0x91, Mc_slot_line, 0x102);
        }
    }
    Rc.x = 0x17;
    Rc.y = 0x32;
    Rc.w = 0x114;
    Rc.h = 0x6A;
    Fade_set(1, 4, 0, 1, 0x101010, 0x101010, 0);
    Fade_rect_set(1, &Rc);
    if (!(pMw->Rno == 14 || pMw->Rno == 15) && pMw->Rno != 10) {
        Cursor_disp(0x14, (pMw->Cursor - pMw->Scrl_top) * 0x14 + 0x38);
    }
    if (pMw->Rno == 14) {
        if (pMw->Err == 2 || pMw->Err == 3 || pMw->Err == 7 || pMw->Err == 8) {
            Print_color(0xFF, 0, 0);
            Print14_l(0x14, (pMw->Cursor_bak - pMw->Scrl_top) * 0x14 + 0x38, "&");
        }
    }
    if (pMw->Rno == 15) {
        Mess_print(Mess_center_x(10, 1), 0xAA, MESS(10), 0x112);
        Mess_print(0x8B, 0xBC, MESS(11), 0x102);
        Mess_print(0x8B, 0xCE, MESS(12), 0x102);
        Cursor_disp(0x7B, pMw->Cursor * 0x12 + 0xBC);
        Print_color(0xFF, 0, 0);
        Print14_l(0x14, (pMw->Cursor_bak - pMw->Scrl_top) * 0x14 + 0x38, "&");
    }
}

/* Disp 2: slot select (memory card slot 1 / slot 2 / exit) */
void Card_disp_slot_sel(MWORK *pMw)
{
    LONG Msg;

    if (pMw->Load_flg == 0) {
        Mess_print(Mess_center_x(5, 1), 0x30, MESS(5), 0x102);
        Mess_print(Mess_center_x(5, 1), 0x40, MESS(6), 0x102);
        Mess_print(Mess_center_x(0, 1), 0x90, MESS(0), 0x102);
        Mess_print(Mess_center_x(0, 1), 0x60, MESS(2), 0x102);
        Mess_print(Mess_center_x(0, 1), 0x78, MESS(3), 0x102);
        Msg = 0;
    } else {
        Mess_print(Mess_center_x(7, 1), 0x30, MESS(7), 0x102);
        Mess_print(Mess_center_x(7, 1), 0x40, MESS(8), 0x102);
        Mess_print(Mess_center_x(1, 1), 0x90, MESS(1), 0x102);
        Mess_print(Mess_center_x(1, 1), 0x60, MESS(2), 0x102);
        Mess_print(Mess_center_x(1, 1), 0x78, MESS(3), 0x102);
        Msg = 1;
    }
    Cursor_disp(Mess_center_x(Msg, 1) - 0x10, pMw->Cursor * 0x18 + 0x60);
}

/* Disp 12: "format the card?" */
void Card_disp_format_ask(MWORK *pMw)
{
    Slot_disp(pMw->Port);
    Mess_print(Mess_center_x(13, 1) + 0xE, 0x40, MESS(13), 0x10A);
    Mess_print(0x37, 0x60, MESS(14), 0x102);
    Mess_print(0x37, 0x78, MESS(15), 0x102);
    Mess_print(0x37, 0x88, MESS(16), 0x102);
    Cursor_disp(0x27, pMw->Cursor * 0x18 + 0x60);
}

/* Disp 1 */
void Card_disp_retry(MWORK *pMw)
{
    Mess_print(Mess_center_x(18, 1), 0x60, MESS(17), 0x102);
    Mess_print(Mess_center_x(18, 1), 0x78, MESS(18), 0x102);
    Cursor_disp(Mess_center_x(18, 1) - 0x10, pMw->Cursor * 0x18 + 0x60);
}

/* Disp 3: error message */
void Card_disp_err(MWORK *pMw)
{
    if (pMw->Err != 4) {
        Slot_disp(pMw->Port);
    }
    Mess_print(Mess_center_x(pMw->Err + 0x21, 1), 0x60, MESS(pMw->Err + 0x21), 0x112);
}

/* Disp 16: "continue?" */
void Card_disp_continue(MWORK *pMw)
{
    Mess_print(Mess_center_x(21, 1), 0x42, MESS(21), 0x10A);
    Mess_print(Mess_center_x(21, 1), 0x54, MESS(22), 0x10A);
    Mess_print(Mess_center_x(12, 1), 0x74, MESS(11), 0x102);
    Mess_print(Mess_center_x(12, 1), 0x8C, MESS(12), 0x102);
    Cursor_disp(Mess_center_x(12, 1) - 0x10, pMw->Cursor * 0x18 + 0x74);
}

/* Disp 17: menu after saving */
void Card_disp_after_save(MWORK *pMw)
{
    if (Flg_ck(G.Unk77F8, 0x1F)) {
        Mess_print(Mess_center_x(27, 1), 0x40, MESS(27), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0x60, MESS(20), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0x78, MESS(24), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0x90, MESS(23), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0xA8, MESS(1), 0x102);
        Cursor_disp(Mess_center_x(23, 1) - 8, pMw->Cursor * 0x18 + 0x60);
    } else {
        Mess_print(Mess_center_x(27, 1), 0x36, MESS(27), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0x56, MESS(20), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0x6C, MESS(24), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0x82, MESS(23), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0x98, MESS(40), 0x102);
        Mess_print(Mess_center_x(23, 1) + 8, 0xAE, MESS(1), 0x102);
        Cursor_disp(Mess_center_x(23, 1) - 8, pMw->Cursor * 0x16 + 0x56);
    }
}

/* Disp 18: end message */
void Card_disp_end(MWORK *pMw)
{
    Mess_print(Mess_center_x(29, 1) + 8, 0x56, MESS(28), 0x102);
    Mess_print(Mess_center_x(29, 1) + 8, 0x6A, MESS(29), 0x102);
    Mess_print(Mess_center_x(29, 1) + 8, 0x7E, MESS(30), 0x102);
}

/* Cursor_disp: blinking cursor glyph */
void Cursor_disp(ULONG X, ULONG Y)
{
    if (pMwork->Blink & 0x18) {
        Print_color(0xFF, 0, 0);
        Print14_l(X, Y, "&");
    }
}

/* Slot_disp: "slot 1" / "slot 2" title */
void Slot_disp(LONG Port)
{
    if (Port) {
        Mess_print(Mess_center_x(2, 1), 0x14, MESS(3), 0x102);
    } else {
        Mess_print(Mess_center_x(3, 1), 0x14, MESS(2), 0x102);
    }
}

/* Card_check: poll the card status until it settles; 2 (new card) clears the card */
LONG Card_check(LONG Port)
{
    LONG Ret;
    LONG Cnt;
    LONG Old;

    Cnt = 16;
    Old = 0xFF;
    Clear_event_wait();
    do {
        if (_card_info(Port << 4)) {
            Ret = Get_card_event();
            if (Ret == 3 || Ret == Old) {
                break;
            }
            Old = Ret;
        }
    } while (--Cnt);
    if (Ret == 2) {
        Card_clear(Port);
        Ret = 0;
    }
    return Ret;
}

/* Clear_event_wait */
void Clear_event_wait(void)
{
    TestEvent(Ev0);
    TestEvent(Ev1);
    TestEvent(Ev2);
    TestEvent(Ev3);
}

/* Clear_event_wait2 */
void Clear_event_wait2(void)
{
    TestEvent(Ev10);
    TestEvent(Ev11);
    TestEvent(Ev13);
}

/* Get_card_event: wait for one of the four card events */
LONG Get_card_event(void)
{
    LONG Cnt;

    for (Cnt = 250000; Cnt != 0; Cnt--) {
        if (TestEvent(Ev0)) {
            return 0;
        }
        if (TestEvent(Ev1)) {
            return 1;
        }
        if (TestEvent(Ev2)) {
            return 2;
        }
        if (TestEvent(Ev3)) {
            return 3;
        }
    }
    return 3;
}

/* Get_card_event2: wait for one of the three card events */
LONG Get_card_event2(void)
{
    LONG Cnt;

    Cnt = 250000;
    do {
        if (TestEvent(Ev10)) {
            return 0;
        }
        if (TestEvent(Ev11)) {
            return 1;
        }
        if (TestEvent(Ev13)) {
            return 2;
        }
        Cnt--;
    } while (Cnt != 0);
    return 3;
}

/* Card_clear: erase the card (new card), up to ten tries */
LONG Card_clear(LONG Port)
{
    LONG Cnt;
    LONG Ret;

    Cnt = 10;
    Ret = 2;
    Clear_event_wait2();
    do {
        if (_card_clear(Port << 4)) {
            Ret = Get_card_event2();
            if (Ret == 0) {
                break;
            }
        }
    } while (--Cnt);
    return Ret;
}

/* Card_format_check: 0 when the card loads cleanly, else the last event */
LONG Card_format_check(LONG Port)
{
    LONG Cnt;
    LONG Ret;

    Cnt = 3;
    Clear_event_wait();
    do {
        if (_card_load(Port << 4)) {
            Ret = Get_card_event();
            if (Ret == 0) {
                return 0;
            }
        }
    } while (--Cnt);
    return Ret;
}

/* Check_card: list the card and read the summary of each RE3 save */
LONG Check_card(UCHAR *F_num)
{
    MWORK *pMw = pMwork;
    ULONG i;
    LONG Ret;

    Card_search(pMw->Port, &pMw->File_info, "*");
    Search_f_num(F_num);
    if (pMw->File_info.Game_num != 0) {
        for (i = 0; i < 15; i++) {
            if (!(F_num[i] & 0xC0)) {
                if (Card_read(&pMw->Card_buf[0x200], pMw->File_info.Title[F_num[i]], sizeof(SAVE_INFO), 0x200)) {
                    goto error;
                }
                Memcpy(&pMw->Save_info[i], &pMw->Card_buf[0x200], sizeof(SAVE_INFO));
            }
        }
        return 0;
    }
error:
    Ret = Card_check(pMw->Port);
    if (Ret != 0) {
        if (Ret == 3) {
            return 3;
        }
        return Card_format_check(pMw->Port);
    }
    return 0;
}

/* Card_search: list the files matching pPattern on the card in port Port */
void Card_search(LONG Port, CFILE_INFO *pInfo, char *pPattern)
{
    struct DIRENTRY Dir;

    strcpy(&Mc_fname_tmpl[5], pPattern);
    Mc_fname_tmpl[2] = Port + '0';
    pInfo->Num = 0;
    pInfo->Total_size = 0;
    if (firstfile(Mc_fname_tmpl, &Dir) == &Dir) {
        do {
            strcpy(pInfo->Title[pInfo->Num], Dir.name);
            pInfo->Size[pInfo->Num] = Dir.size;
            pInfo->Total_size += Dir.size;
            pInfo->Num += Dir.size / 8192;
        } while (nextfile(&Dir) == &Dir);
    }
}

/* Search_f_num: slot -> card file index of the RE3 saves; other games' files take free slots from the end */
void Search_f_num(UCHAR *F_num)
{
    MWORK *pMw = pMwork;
    ULONG i;
    LONG Other;
    LONG Slot;

    for (i = 0; i < 15; i++) {
        F_num[i] = 0x80;
    }
    pMw->File_info.Game_num = 0;
    Other = 0;
    for (i = 0; i < pMw->File_info.Num; i++) {
        if (strncmp(pMw->File_info.Title[i], "BASLUS-00923", 12) == 0) {
            Slot = pMw->File_info.Title[i][12] - '0';
            F_num[Slot * 10 + pMw->File_info.Title[i][13] - '0'] = i;
            pMw->File_info.Game_num++;
        } else {
            Other++;
        }
    }
    for (i = 14; Other != 0; i--) {
        if (F_num[i] & 0x80) {
            F_num[i] = 0x40;
            Other--;
        }
    }
}

/* Card_read: read Size bytes at Offset from file pName, three tries */
LONG Card_read(void *pBuf, char *pName, LONG Size, LONG Offset)
{
    ULONG Try;
    LONG Fd;

    Mc_fname_tmpl[2] = Mwork.Port + '0';
    strcpy(&Mc_fname_tmpl[5], pName);
    Try = 0;
    do {
        if ((Fd = open(Mc_fname_tmpl, 1)) != -1 && lseek(Fd, Offset, 0) != -1 && read(Fd, pBuf, Size) == Size) {
            close(Fd);
            return 0;
        }
    } while (++Try < 3);
    return 2;
}

/* Card_poll: 1 when the card in Port was changed */
LONG Card_poll(LONG Port)
{
    MWORK *pMw = pMwork;

    if (_card_info(Port << 4) == 0) {
        return 0;
    }
    pMw->Poll_flg = 1;
    return pMw->Card_chg != 0;
}

/* Save_push: copy the game state into the save area before writing */
void Save_push(void)
{
    tagGLOBAL *pG = &G;
    LONG Now;

    if (!(pG->System_flg & 0x80)) {
        if (pG->System_flg & 0x4000) {
            pG->Game_time = 0;
        } else {
            Now = VSync(-1);
            pG->Game_time += Now - pG->Game_time_offset;
            pG->Game_time_offset = Now;
        }
    }
    pG->Save_pl_id = pG->Player.Id;
    pG->Save_pos_x = pG->Player.Pos_x;
    pG->Save_pos_y = pG->Player.Pos_y;
    pG->Save_pos_z = pG->Player.Pos_z;
    pG->Save_cdir_y = pG->Player.Cdir_y;
    pG->Save_life = pG->Player.Life;
    if (G.Player.Status_flg & 0x200) {
        pG->Save_poison = G.Pl_poison_timer | 0x80;
    } else {
        pG->Save_poison = 0;
    }
    pG->Save_vol = Snd_work.Vol.Se;
    pG->Save_vol2 = Snd_work.Vol.Bgm;
    pG->Save_mono = Snd_work.UnkB75;
    pG->Save_unk780C = Main.Unk195;
    pG->Save_unk780D = Main.Unk196;
}

/* Load_pop: restore the game state from the loaded save area */
void Load_pop(void)
{
    tagGLOBAL *pG = &G;

    pG->Next_cut_no = pG->Cut_no;
    pG->Player.Id = pG->Save_pl_id;
    pG->Player.Pos_x = pG->Save_pos_x;
    pG->Player.Pos_y = pG->Save_pos_y;
    pG->Player.Pos_z = pG->Save_pos_z;
    pG->Player.Cdir_y = pG->Save_cdir_y;
    pG->Player.Life = pG->Save_life;
    pG->Player.nFloor = pG->Save_pos_y / -1800;
    if (pG->Save_poison & 0x80) {
        pG->Pl_poison_timer = pG->Save_poison & 0x7F;
        pG->Player.Status_flg |= 0x200;
    } else {
        pG->Pl_poison_timer = 0;
        pG->Player.Status_flg &= ~0x200;
    }
    Snd_work.Vol.Se = pG->Save_vol;
    Snd_work.Vol.Bgm = pG->Save_vol2;
    Snd_work.UnkB75 = pG->Save_mono;
    Main.Unk195 = pG->Save_unk780C;
    Main.Unk196 = pG->Save_unk780D;
    G.pItem_set = &G.Item_work[G.Item_work_no];
    Set_volume(CD.Vol);
}

/* Card_mess_make: build the Shift-JIS title of the card file: "RE3  NN.[area]/save count[clear count]E|H" */
void Card_mess_make(char *pTitle, void *pHead, UCHAR Slot)
{
    tagGLOBAL *pG = &G;
    UCHAR No = Slot + 1;

    func_8001012C(pTitle, "\x82\x71\x82\x64\x82\x52\x81\x40\x82\x4F\x82\x4F\x81\x44\x81\x75", 16);
    func_8001012C(pTitle + 0x10, Mc_area_name[pG->Save_area], 0x1C);
    func_8001012C(pTitle + 0x2C, "\x81\x5E\x82\x4F\x82\x4F\x81\x7C\x82\x4F\x82\x4F\x81\x76", 15);
    if (No >= 10) {
        pTitle[9] += No / 10;
    } else {
        func_8001012C(pTitle + 8, "\x81\x40", 2);
    }
    pTitle[11] += No % 10;
    pTitle[0x2F] += pG->Save_cnt / 10;
    pTitle[0x31] += pG->Save_cnt % 10;
    if (pG->Unk7806 != 0) {
        if (pG->Unk7806 < 100) {
            pTitle[0x35] += pG->Unk7806 / 10;
            pTitle[0x37] += pG->Unk7806 % 10;
        } else {
            func_8001012C(pTitle + 0x34, "\x82\x77\x82\x77\x81\x76", 7);
        }
        pTitle += 0x3A;
    } else {
        func_8001012C(pTitle + 0x32, "\x81\x76", 3);
        pTitle += 0x34;
    }
    if (Flg_ck(G.Unk77F8, 0x1F)) {
        func_8001012C(pTitle, "\x81\x40\x82\x64", 5);
    } else {
        func_8001012C(pTitle, "\x81\x40\x82\x67", 5);
    }
}

/* The original .rodata ends with these two bytes after the last string (unreferenced; probably padding junk) */
__asm__(".section .rodata\n\t.byte 0, 0x10\n\t.section .text");

/* Card_write: create file pName, write Size bytes from pHead and read the header back; 1 open error, 2 write error */
LONG Card_write(void *pHead, LONG Port, char *pName, char *pTitle, ULONG Size)
{
    _CARD *pCard = pHead;
    UCHAR Verify[0x200];
    ULONG Try;
    LONG Fd;
    LONG Cnt;
    LONG Got;
    LONG Ret;

    pCard->Block_entry = Size / 8192 + (Size % 8192 != 0);
    Mc_fname_tmpl[2] = Port + '0';
    strcpy(&Mc_fname_tmpl[5], pName);
    strcpy(pCard->Title, pTitle);
    close(open(Mc_fname_tmpl, (pCard->Block_entry << 16) | 0x200));
    for (Try = 0; ; ) {
        for (Cnt = 3; (Fd = open(Mc_fname_tmpl, 2)) == -1; ) {
            if (--Cnt == 0) {
                return 1;
            }
        }
        for (Cnt = 3; write(Fd, pHead, Size) != Size; ) {
            if (--Cnt == 0) {
                return 2;
            }
        }
        close(Fd);
        Fd = open(Mc_fname_tmpl, 1);
        if (Fd == -1) {
            return 2;
        }
        Got = read(Fd, Verify, sizeof(Verify));
        close(Fd);
        Try++;
        if (Got == sizeof(Verify)) {
            Ret = 0;
            break;
        }
        if (Try >= 3) {
            Ret = 2;
            break;
        }
    }
    return Ret;
}

/* Card_format: 0 on success */
LONG Card_format(LONG Port)
{
    Mc_fname_tmpl[2] = Port + '0';
    return format(Mc_fname_tmpl) ^ 1;
}

/* Mess_center_x: x that centres message Msg on the 320 pixel screen (Mode 0: fixed 14 pixel glyphs) */
SHORT Mess_center_x(LONG Msg, LONG Mode)
{
    LONG Width;
    ULONG i;
    UCHAR *p;

    Width = 0;
    p = MESS(Msg);
    for (;;) {
        if (*p >= 0xEE) {
            for (i = 0; i < 6; i++) {
                if (*p == Mess_ctrl[i]) {
                    if (i == 0) {
                        return (0x140 - Width) >> 1;
                    }
                    p++;
                    break;
                }
            }
        } else if (Mode != 0 && *p < 0x57) {
            Width += msg_space[*p * 2 + 1];
        } else {
            Width += 14;
        }
        p++;
    }
}

/* Mess_width: pixel width of message Msg */
SHORT Mess_width(LONG Msg)
{
    LONG Width;
    ULONG i;
    UCHAR *p;

    Width = 0;
    p = MESS(Msg);
    for (;;) {
        if (*p >= 0xEE) {
            for (i = 0; i < 6; i++) {
                if (*p == Mess_ctrl[i]) {
                    if (i == 0) {
                        return Width;
                    }
                    p++;
                    break;
                }
            }
        } else if (*p < 0x57) {
            Width += msg_space[*p * 2 + 1];
        } else {
            Width += 14;
        }
        p++;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/mem_card", _card_info);

INCLUDE_ASM("asm/nonmatchings/ovl/mem_card", _card_load);

INCLUDE_ASM("asm/nonmatchings/ovl/mem_card", _card_clear);
