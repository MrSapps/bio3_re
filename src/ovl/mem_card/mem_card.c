#include "common.h"
#include "main.h"
#include "monitor.h"
#include "mojidisp.h"
#include "pl_wep.h"
#include "snd.h"
#include "cd.h"
#include "em_sub.h"
#include "sub.h"
#include <strings.h>
#include "ovl/mem_card.h"

/* Overlay .data (still asm; names in ovl/mem_card.syms.txt) */
extern char D_801C5EDC[][29];           /* Mc_area_name: save area names (Shift-JIS) */
extern UCHAR D_801C6244[];              /* Mess_text: message pool */
extern USHORT D_801C6760[];             /* Mess_offs: message offsets into Mess_text */
extern MWORK *D_801C6834;               /* pMwork (-> D_801C69F8) */
extern char D_801C6838[];               /* Mc_fname_tmpl: "bu?0:" + file name */
extern UCHAR D_801C6858[];              /* Mc_wait_tbl: frames per step of the "saved" animation */
extern UCHAR D_801C6870[6];             /* Mess_ctrl: control codes; [0] ends a message, the others take one operand */
extern void (*D_801C6878[])(MWORK *);   /* Card_rno_tbl: state routines */
extern void (*D_801C68C8[])(MWORK *);   /* Card_disp_tbl: state displays */
extern LONG D_801C6918;                 /* sound already played for the revealed character */
extern UCHAR D_801C6920[0x28];          /* item work backup while saving */
extern char D_801C6948[];               /* name of the file being overwritten */
extern char D_801C6960[0x48];           /* card file title (Shift-JIS) */
extern UCHAR D_801C69A8[0x50];          /* slot line: area name */
extern MWORK D_801C69F8;                /* the card work itself */
extern UCHAR D_801C7CB8[16];            /* slot line: save count / clear count */
extern CFILE_INFO D_801C7CC8;           /* directory of the RE3 files on the card */

#define MESS(n) (&D_801C6244[D_801C6760[n]])

/* Print14 called with int coordinates (no USHORT truncation in the caller) */
void Print14_l(LONG X, LONG Y, UCHAR *pFmt, ...) __asm__("Print14");

/* libapi BIOS stubs linked into the overlay (after the game code) */
long func_801C5E64(long chan);          /* _card_info */
long func_801C5E74(long chan);          /* _card_load */
long func_801C5E84(long chan);          /* _card_clear */

/* Mem_card: task entry */
void func_801C20AC(void)
{
    if (Task_status(0) == 1) {
        Task_suspend(0);
        func_801C20F8();
        Task_signal(0);
        Task_exit();
    }
}

/* Card_access: main loop of the card screen */
void func_801C20F8(void)
{
    MWORK *pMw = D_801C6834;

    func_801C2504(pMw);
    pMw->Saved_flg = 0;
    for (;;) {
        Print_color(0xFF, 0, 0);
        pMw->Poll_flg = 0;
        pMw->Key_trg = G.Key_trg;
        pMw->Key_rep = G.Unk20FC;
        func_801C227C(pMw);
        do {
            pMw->Rno_loop = 0;
            D_801C6878[pMw->Rno](pMw);
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
        D_801C68C8[pMw->Rno](pMw);
        pMw->Card_chg = 0;
        if (pMw->Poll_flg) {
            if (func_801C5060() == 1) {
                pMw->Card_chg = 1;
            }
        }
        G.Key_trg = pMw->Key_trg;
        G.Unk20FC = pMw->Key_rep;
        Task_sleep(1);
    }
    func_801C25FC(pMw);
}

/* Card_cursor: move the cursor with up/down, with key repeat */
LONG func_801C227C(MWORK *pMw)
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
void func_801C2504(MWORK *pMw)
{
    func_80010150(pMw, sizeof(MWORK));
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
void func_801C25FC(MWORK *pMw)
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
void func_801C26A0(MWORK *pMw)
{
    LONG Ret0, Ret1;

    switch (pMw->Rno1) {
    case 0:
        pMw->Wait = 6;
        pMw->Rno1 = 1;
    case 1:
        if (--pMw->Wait == 0) {
            Ret0 = func_801C4F1C(0);
            Ret1 = func_801C4F1C(1);
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
void func_801C2788(MWORK *pMw)
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
void func_801C2820(MWORK *pMw)
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
void func_801C28D4(MWORK *pMw)
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
void func_801C294C(MWORK *pMw)
{
    Mess_print(func_801C5CC0(0x1A, 1), 0xB2, MESS(9), 0x102);
    Mess_print(func_801C5CC0(0x1A, 1), 0xC0, MESS(26), 0x102);
    Task_sleep(1);
    pMw->Rno = 5;
}

/* Rno 5: read the card directory and the save summaries */
void func_801C29EC(MWORK *pMw)
{
    pMw->Cursor_max = 0;
    pMw->Key_lock = 0;
    if ((pMw->Err = func_801C4F1C(pMw->Port)) != 0) {
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
    pMw->Err = func_801C51F0(pMw->Port);
    func_80010150(&pMw->File_info, sizeof(CFILE_INFO));
    if (pMw->Err == 0) {
        pMw->Err = func_801C5260(pMw->F_num);
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
void func_801C2BE0(MWORK *pMw)
{
    signed char Cursor;

    pMw->Cursor_max = 14;
    G.Save_slot = pMw->Cursor;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        Cursor = pMw->Cursor;
        if (!(pMw->F_num[Cursor] & 0x40)) {
            pMw->Slot = Cursor;
            Snd_se_stad(6, 0, 0, 0);
            func_801C5384(pMw->Port, &D_801C7CC8, "BASLUS-00923*");
            if (D_801C7CC8.Num == 0) {
                VSync(0);
                if ((pMw->Err = func_801C4F1C(pMw->Port)) != 0) {
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
        if (func_801C4F1C(pMw->Port ^ 1) != 0) {
            pMw->Exit_flg = 1;
        } else {
            pMw->Cursor = 0;
            pMw->Rno = 0;
            pMw->Rno1 = 0;
        }
        return;
    }
    if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 17: menu after saving */
void func_801C2D54(MWORK *pMw)
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
            func_800100A4(D_800C7048.Save, G.Card_head, sizeof(D_800C7048.Save));
            D_800C7048.UnkA = G.Pl_id;
            D_800C7048.Pos_x = G.Pl_pos_x;
            D_800C7048.Pos_y = G.Pl_pos_y;
            D_800C7048.Pos_z = G.Pl_pos_z;
            D_800C7048.Cdir_y = G.Pl_cdir_y;
            D_800C7048.Life = G.Pl_life;
            if (G.Pl_status_flg & 0x200) {
                D_800C7048.UnkB = G.Pl_poison_timer | 0x80;
            } else {
                D_800C7048.UnkB = 0;
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
    } else if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 7: slot chosen */
void func_801C2FA8(MWORK *pMw)
{
    ULONG i;
    signed char Cursor;

    pMw->Cursor = pMw->Slot;
    if (pMw->Load_flg == 0) {
        strcpy(D_801C6948, D_801C7CC8.Title[pMw->Cursor]);
        for (i = 0; i < 22; i++) {
            D_801C7CC8.Title[pMw->Cursor][i] = 0;
        }
        __builtin_memcpy(D_801C7CC8.Title[pMw->Cursor], "BASLUS-0092300", 15);
        D_801C7CC8.Title[pMw->Cursor][12] += pMw->Cursor / 10;
        D_801C7CC8.Title[pMw->Cursor][13] += pMw->Cursor % 10;
        D_801C7CC8.Title[pMw->Cursor][14] = 0;
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
        Mess_print(func_801C5CC0(0x1A, 1), 0xB2, MESS(25), 0x102);
        Mess_print(func_801C5CC0(0x1A, 1), 0xC0, MESS(26), 0x102);
        pMw->Timer = 30;
    } else {
        if (pMw->F_num[pMw->Cursor] & 0xC0) {
            pMw->Err = 3;
            pMw->Rno = 14;
            pMw->Key_lock = 0;
            pMw->Cursor_bak = pMw->Cursor;
        } else {
            Mess_print(func_801C5CC0(0x1A, 1), 0xBA, MESS(19), 0x102);
            Mess_print(func_801C5CC0(0x1A, 1), 0xCA, MESS(26), 0x102);
            pMw->Timer = 30;
            pMw->Rno = 19;
        }
    }
    if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 8: "saving" message before the write */
void func_801C3340(MWORK *pMw)
{
    Mess_print(func_801C5CC0(0x1A, 1), 0xB2, MESS(25), 0x102);
    Mess_print(func_801C5CC0(0x1A, 1), 0xC0, MESS(26), 0x102);
    if (--pMw->Timer == 0xFF) {
        pMw->Rno = 9;
    }
    if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 19: "loading" message before the read */
void func_801C3414(MWORK *pMw)
{
    Mess_print(func_801C5CC0(0x1A, 1), 0xBA, MESS(19), 0x102);
    Mess_print(func_801C5CC0(0x1A, 1), 0xCA, MESS(26), 0x102);
    if (--pMw->Timer == 0xFF) {
        pMw->Rno = 11;
    }
    if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 9: write the save file */
void func_801C34E8(MWORK *pMw)
{
    UCHAR Save_cnt;

    pMw->Cursor = pMw->Slot;
    func_800100A4(D_801C6920, G.pUnk7C7C, sizeof(D_801C6920));
    pMw->Item_bak = G.pUnk7C7C[0x128];
    if (!(G.System_flg & 0x4080)) {
        func_8006CCF0(Search_item_id(0x81, 0), 1, 0);
    }
    Save_cnt = G.Save_cnt;
    if ((Save_cnt < 99 || (G.System_flg & 0x4000)) && !(G.System_flg & 0x80)) {
        G.Save_cnt = Save_cnt + 1;
    }
    func_801C56E0();
    func_800100A4(&pMw->Save_info[pMw->Cursor], &G.Game_time, sizeof(SAVE_INFO));
    func_801C58FC(D_801C6960, G.Card_head, pMw->Cursor);
    pMw->Err = func_801C5B20(G.Card_head, pMw->Port, D_801C7CC8.Title[pMw->Cursor], D_801C6960, 0x980);
    Mess_print(func_801C5CC0(0x1A, 1), 0xB2, MESS(25), 0x102);
    Mess_print(func_801C5CC0(0x1A, 1), 0xC0, MESS(26), 0x102);
    pMw->Rno = 9;
    if (pMw->Err == 0) {
        pMw->F_num[pMw->Cursor] = 0;
        pMw->Moji_no = -1;
        pMw->Anim_step = 0;
        pMw->Anim_wait = D_801C6858[0];
        pMw->Rno = 10;
        pMw->Anim_mode = 0;
        pMw->Rno_loop = 1;
        pMw->Saved_flg = 1;
    } else {
        if (!(G.System_flg & 0x80)) {
            G.Save_cnt = Save_cnt;
        }
        func_800100A4(G.pUnk7C7C, D_801C6920, sizeof(D_801C6920));
        G.pUnk7C7C[0x128] = pMw->Item_bak;
        pMw->Err = 5;
        pMw->Key_lock = 0;
        pMw->Rno = 3;
    }
}

/* Rno 10: "saved" animation of the new slot title */
void func_801C3754(MWORK *pMw)
{
    USHORT *pOfs = D_801C6760;
    UCHAR *pText = D_801C6244;

    Mess_print(func_801C5CC0(0x1A, 1), 0xB2, &pText[pOfs[25]], 0x102);
    Mess_print(func_801C5CC0(0x1A, 1), 0xC0, &pText[pOfs[26]], 0x102);
    if (--pMw->Anim_wait == 0) {
        if (pMw->Anim_mode == 1) {
            pMw->Anim_mode = 2;
            pMw->Moji_no = 0;
        } else {
            pMw->Moji_no++;
        }
        D_801C6918 = 0;
        if (pMw->Moji_no < 0) {
            pMw->Err = 0;
            if (G.System_flg & 0x80) {
                pMw->Rno = 18;
            } else {
                pMw->Exit_flg = 1;
            }
        } else {
            pMw->Anim_step++;
            pMw->Anim_wait = D_801C6858[pMw->Anim_step];
        }
    }
}

/* Rno 11: read the save file */
void func_801C389C(MWORK *pMw)
{
    pMw->Cursor = pMw->Slot;
    if ((pMw->Err = func_801C55B0(pMw->Card_buf, pMw->File_info.Title[pMw->F_num[pMw->Cursor]], sizeof(pMw->Card_buf), 0)) != 0) {
        pMw->Err = 6;
        pMw->Rno = 14;
        pMw->Key_lock = 0;
        pMw->Cursor_bak = pMw->Cursor;
        return;
    }
    func_800100A4(G.Card_head, pMw->Card_buf, 0x908);
    func_801C57E0();
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
void func_801C39AC(MWORK *pMw)
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
    if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 13: format the card */
void func_801C3A9C(MWORK *pMw)
{
    if ((pMw->Err = func_801C4F1C(pMw->Port)) == 0) {
        pMw->Err = func_801C51F0(pMw->Port);
        if (pMw->Err == 2 && func_801C5C90(pMw->Port) == 0) {
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
void func_801C3B50(MWORK *pMw)
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
    Mess_print(func_801C5CC0(pMw->Err + 0x21, 1), 0xC4, MESS(pMw->Err + 0x21), 0x112);
    if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 15: "overwrite this file?" */
void func_801C3C64(MWORK *pMw)
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
    if (func_801C5698(pMw->Port)) {
        pMw->Rno = 3;
        pMw->Port = 0;
        pMw->Err = 4;
    }
}

/* Rno 16: "continue the game?" after a save on the way out */
void func_801C3D88(MWORK *pMw)
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
void func_801C3E48(MWORK *pMw)
{
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800) || (G.Key_trg & 0x2000)) {
        pMw->Exit_flg = 1;
    }
}

/* Disp: nothing */
void func_801C3E90(MWORK *pMw) {
}

/* Disp 0: "now checking" */
void func_801C3E98(MWORK *pMw)
{
    Mess_print(func_801C5CC0(0x1A, 1), 0xB2, MESS(9), 0x102);
}

/* Disp 6..11, 14, 15, 19: list of the save slots */
void func_801C3EE4(MWORK *pMw)
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
                func_8001012C(D_801C7CB8, MESS(93), 10);
                if (pInfo->Unk16 == 0) {
                    D_801C7CB8[3] = 0xFE;
                } else {
                    D_801C7CB8[4] += pInfo->Unk16 / 10;
                    D_801C7CB8[5] += pInfo->Unk16 % 10;
                }
            } else {
                func_8001012C(D_801C7CB8, MESS(94), 10);
            }
            D_801C7CB8[1] += pInfo->Save_cnt / 10;
            D_801C7CB8[2] += pInfo->Save_cnt % 10;
            func_8001012C(D_801C69A8, MESS(pInfo->Save_area + 0x3F), 0x18);
            Col = (Flg_ck(&pInfo->Flg, 0x1F) != 0) << 3;
            if (pMw->Rno == 10 && Slot == pMw->Cursor) {
                Last = 0;
                pStr = D_801C69A8;
                if (pMw->Anim_mode) {
                    pStr = D_801C7CB8;
                }
                if (pMw->Moji_no >= 0 && (UCHAR)(pStr[pMw->Moji_no] - 0xEA) < 7 && D_801C6918 == 0) {
                    pMw->Moji_no++;
                    D_801C6918 = 1;
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
                    Mess_print(0x45, i * 0x14 + 0x38, D_801C69A8, (Col << 3) | 0x102);
                    if (pMw->Anim_mode == 2) {
                        Mess_print(0xD9, i * 0x14 + 0x38, D_801C7CB8, 0x102);
                    }
                }
                if (Last) {
                    pMw->Moji_no = -2;
                }
            } else {
                Mess_print(0x45, i * 0x14 + 0x38, D_801C69A8, (Col << 3) | 0x102);
                Mess_print(0xD9, i * 0x14 + 0x38, D_801C7CB8, 0x102);
            }
        } else if (pMw->F_num[Slot] & 0x80) {
            Mess_print(0x45, i * 0x14 + 0x38, MESS(104), 0x102);
        } else {
            Mess_print(0x45, i * 0x14 + 0x38, MESS(105), 0x132);
        }
    }
    func_801C4EA4(pMw->Port);
    if (--pMw->Wait & 0x18) {
        D_801C69A8[1] = 0xFE;
        if (pMw->Scrl_top != 0) {
            D_801C69A8[0] = 0x19;
            Mess_print(0x99, 0x2F, D_801C69A8, 0x102);
        }
        if (pMw->Scrl_top < 10) {
            D_801C69A8[0] = 0x0B;
            Mess_print(0x99, 0x91, D_801C69A8, 0x102);
        }
    }
    Rc.x = 0x17;
    Rc.y = 0x32;
    Rc.w = 0x114;
    Rc.h = 0x6A;
    Fade_set(1, 4, 0, 1, 0x101010, 0x101010, 0);
    Fade_rect_set(1, &Rc);
    if (!(pMw->Rno == 14 || pMw->Rno == 15) && pMw->Rno != 10) {
        func_801C4E3C(0x14, (pMw->Cursor - pMw->Scrl_top) * 0x14 + 0x38);
    }
    if (pMw->Rno == 14) {
        if (pMw->Err == 2 || pMw->Err == 3 || pMw->Err == 7 || pMw->Err == 8) {
            Print_color(0xFF, 0, 0);
            Print14_l(0x14, (pMw->Cursor_bak - pMw->Scrl_top) * 0x14 + 0x38, "&");
        }
    }
    if (pMw->Rno == 15) {
        Mess_print(func_801C5CC0(10, 1), 0xAA, MESS(10), 0x112);
        Mess_print(0x8B, 0xBC, MESS(11), 0x102);
        Mess_print(0x8B, 0xCE, MESS(12), 0x102);
        func_801C4E3C(0x7B, pMw->Cursor * 0x12 + 0xBC);
        Print_color(0xFF, 0, 0);
        Print14_l(0x14, (pMw->Cursor_bak - pMw->Scrl_top) * 0x14 + 0x38, "&");
    }
}

/* Disp 2: slot select (memory card slot 1 / slot 2 / exit) */
void func_801C4580(MWORK *pMw)
{
    LONG Msg;

    if (pMw->Load_flg == 0) {
        Mess_print(func_801C5CC0(5, 1), 0x30, MESS(5), 0x102);
        Mess_print(func_801C5CC0(5, 1), 0x40, MESS(6), 0x102);
        Mess_print(func_801C5CC0(0, 1), 0x90, MESS(0), 0x102);
        Mess_print(func_801C5CC0(0, 1), 0x60, MESS(2), 0x102);
        Mess_print(func_801C5CC0(0, 1), 0x78, MESS(3), 0x102);
        Msg = 0;
    } else {
        Mess_print(func_801C5CC0(7, 1), 0x30, MESS(7), 0x102);
        Mess_print(func_801C5CC0(7, 1), 0x40, MESS(8), 0x102);
        Mess_print(func_801C5CC0(1, 1), 0x90, MESS(1), 0x102);
        Mess_print(func_801C5CC0(1, 1), 0x60, MESS(2), 0x102);
        Mess_print(func_801C5CC0(1, 1), 0x78, MESS(3), 0x102);
        Msg = 1;
    }
    func_801C4E3C(func_801C5CC0(Msg, 1) - 0x10, pMw->Cursor * 0x18 + 0x60);
}

/* Disp 12: "format the card?" */
void func_801C47AC(MWORK *pMw)
{
    func_801C4EA4(pMw->Port);
    Mess_print(func_801C5CC0(13, 1) + 0xE, 0x40, MESS(13), 0x10A);
    Mess_print(0x37, 0x60, MESS(14), 0x102);
    Mess_print(0x37, 0x78, MESS(15), 0x102);
    Mess_print(0x37, 0x88, MESS(16), 0x102);
    func_801C4E3C(0x27, pMw->Cursor * 0x18 + 0x60);
}

/* Disp 1 */
void func_801C4884(MWORK *pMw)
{
    Mess_print(func_801C5CC0(18, 1), 0x60, MESS(17), 0x102);
    Mess_print(func_801C5CC0(18, 1), 0x78, MESS(18), 0x102);
    func_801C4E3C(func_801C5CC0(18, 1) - 0x10, pMw->Cursor * 0x18 + 0x60);
}

/* Disp 3: error message */
void func_801C4944(MWORK *pMw)
{
    if (pMw->Err != 4) {
        func_801C4EA4(pMw->Port);
    }
    Mess_print(func_801C5CC0(pMw->Err + 0x21, 1), 0x60, MESS(pMw->Err + 0x21), 0x112);
}

/* Disp 16: "continue?" */
void func_801C49CC(MWORK *pMw)
{
    Mess_print(func_801C5CC0(21, 1), 0x42, MESS(21), 0x10A);
    Mess_print(func_801C5CC0(21, 1), 0x54, MESS(22), 0x10A);
    Mess_print(func_801C5CC0(12, 1), 0x74, MESS(11), 0x102);
    Mess_print(func_801C5CC0(12, 1), 0x8C, MESS(12), 0x102);
    func_801C4E3C(func_801C5CC0(12, 1) - 0x10, pMw->Cursor * 0x18 + 0x74);
}

/* Disp 17: menu after saving */
void func_801C4ADC(MWORK *pMw)
{
    if (Flg_ck(G.Unk77F8, 0x1F)) {
        Mess_print(func_801C5CC0(27, 1), 0x40, MESS(27), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0x60, MESS(20), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0x78, MESS(24), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0x90, MESS(23), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0xA8, MESS(1), 0x102);
        func_801C4E3C(func_801C5CC0(23, 1) - 8, pMw->Cursor * 0x18 + 0x60);
    } else {
        Mess_print(func_801C5CC0(27, 1), 0x36, MESS(27), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0x56, MESS(20), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0x6C, MESS(24), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0x82, MESS(23), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0x98, MESS(40), 0x102);
        Mess_print(func_801C5CC0(23, 1) + 8, 0xAE, MESS(1), 0x102);
        func_801C4E3C(func_801C5CC0(23, 1) - 8, pMw->Cursor * 0x16 + 0x56);
    }
}

/* Disp 18: end message */
void func_801C4D84(MWORK *pMw)
{
    Mess_print(func_801C5CC0(29, 1) + 8, 0x56, MESS(28), 0x102);
    Mess_print(func_801C5CC0(29, 1) + 8, 0x6A, MESS(29), 0x102);
    Mess_print(func_801C5CC0(29, 1) + 8, 0x7E, MESS(30), 0x102);
}

/* Cursor_disp: blinking cursor glyph */
void func_801C4E3C(ULONG X, ULONG Y)
{
    if (D_801C6834->Blink & 0x18) {
        Print_color(0xFF, 0, 0);
        Print14_l(X, Y, "&");
    }
}

/* Slot_disp: "slot 1" / "slot 2" title */
void func_801C4EA4(LONG Port)
{
    if (Port) {
        Mess_print(func_801C5CC0(2, 1), 0x14, MESS(3), 0x102);
    } else {
        Mess_print(func_801C5CC0(3, 1), 0x14, MESS(2), 0x102);
    }
}

/* Card_check: poll the card status until it settles; 2 (new card) clears the card */
LONG func_801C4F1C(LONG Port)
{
    LONG Ret;
    LONG Cnt;
    LONG Old;

    Cnt = 16;
    Old = 0xFF;
    func_801C4FC0();
    do {
        if (func_801C5E64(Port << 4)) {
            Ret = func_801C5060();
            if (Ret == 3 || Ret == Old) {
                break;
            }
            Old = Ret;
        }
    } while (--Cnt);
    if (Ret == 2) {
        func_801C5184(Port);
        Ret = 0;
    }
    return Ret;
}

/* Clear_event_wait */
void func_801C4FC0(void)
{
    TestEvent(Ev0);
    TestEvent(Ev1);
    TestEvent(Ev2);
    TestEvent(Ev3);
}

/* Clear_event_wait2 */
void func_801C5018(void)
{
    TestEvent(Ev10);
    TestEvent(Ev11);
    TestEvent(Ev13);
}

/* Get_card_event: wait for one of the four card events */
LONG func_801C5060(void)
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
LONG func_801C50F4(void)
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
LONG func_801C5184(LONG Port)
{
    LONG Cnt;
    LONG Ret;

    Cnt = 10;
    Ret = 2;
    func_801C5018();
    do {
        if (func_801C5E84(Port << 4)) {
            Ret = func_801C50F4();
            if (Ret == 0) {
                break;
            }
        }
    } while (--Cnt);
    return Ret;
}

/* Card_format_check: 0 when the card loads cleanly, else the last event */
LONG func_801C51F0(LONG Port)
{
    LONG Cnt;
    LONG Ret;

    Cnt = 3;
    func_801C4FC0();
    do {
        if (func_801C5E74(Port << 4)) {
            Ret = func_801C5060();
            if (Ret == 0) {
                return 0;
            }
        }
    } while (--Cnt);
    return Ret;
}

/* Check_card: list the card and read the summary of each RE3 save */
LONG func_801C5260(UCHAR *F_num)
{
    MWORK *pMw = D_801C6834;
    ULONG i;
    LONG Ret;

    func_801C5384(pMw->Port, &pMw->File_info, "*");
    func_801C5478(F_num);
    if (pMw->File_info.Game_num != 0) {
        for (i = 0; i < 15; i++) {
            if (!(F_num[i] & 0xC0)) {
                if (func_801C55B0(&pMw->Card_buf[0x200], pMw->File_info.Title[F_num[i]], sizeof(SAVE_INFO), 0x200)) {
                    goto error;
                }
                func_800100A4(&pMw->Save_info[i], &pMw->Card_buf[0x200], sizeof(SAVE_INFO));
            }
        }
        return 0;
    }
error:
    Ret = func_801C4F1C(pMw->Port);
    if (Ret != 0) {
        if (Ret == 3) {
            return 3;
        }
        return func_801C51F0(pMw->Port);
    }
    return 0;
}

/* Card_search: list the files matching pPattern on the card in port Port */
void func_801C5384(LONG Port, CFILE_INFO *pInfo, char *pPattern)
{
    struct DIRENTRY Dir;

    strcpy(&D_801C6838[5], pPattern);
    D_801C6838[2] = Port + '0';
    pInfo->Num = 0;
    pInfo->Total_size = 0;
    if (firstfile(D_801C6838, &Dir) == &Dir) {
        do {
            strcpy(pInfo->Title[pInfo->Num], Dir.name);
            pInfo->Size[pInfo->Num] = Dir.size;
            pInfo->Total_size += Dir.size;
            pInfo->Num += Dir.size / 8192;
        } while (nextfile(&Dir) == &Dir);
    }
}

/* Search_f_num: slot -> card file index of the RE3 saves; other games' files take free slots from the end */
void func_801C5478(UCHAR *F_num)
{
    MWORK *pMw = D_801C6834;
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
LONG func_801C55B0(void *pBuf, char *pName, LONG Size, LONG Offset)
{
    ULONG Try;
    LONG Fd;

    D_801C6838[2] = D_801C69F8.Port + '0';
    strcpy(&D_801C6838[5], pName);
    Try = 0;
    do {
        if ((Fd = open(D_801C6838, 1)) != -1 && lseek(Fd, Offset, 0) != -1 && read(Fd, pBuf, Size) == Size) {
            close(Fd);
            return 0;
        }
    } while (++Try < 3);
    return 2;
}

/* Card_poll: 1 when the card in Port was changed */
LONG func_801C5698(LONG Port)
{
    MWORK *pMw = D_801C6834;

    if (func_801C5E64(Port << 4) == 0) {
        return 0;
    }
    pMw->Poll_flg = 1;
    return pMw->Card_chg != 0;
}

/* Save_push: copy the game state into the save area before writing */
void func_801C56E0(void)
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
    pG->Save_pl_id = pG->Pl_id;
    pG->Save_pos_x = pG->Pl_pos_x;
    pG->Save_pos_y = pG->Pl_pos_y;
    pG->Save_pos_z = pG->Pl_pos_z;
    pG->Save_cdir_y = pG->Pl_cdir_y;
    pG->Save_life = pG->Pl_life;
    if (G.Pl_status_flg & 0x200) {
        pG->Save_poison = G.Pl_poison_timer | 0x80;
    } else {
        pG->Save_poison = 0;
    }
    pG->Save_vol = Snd_work.Unk7C8;
    pG->Save_vol2 = Snd_work.Unk7CA;
    pG->Save_mono = Snd_work.UnkB75;
    pG->Save_unk780C = Main.Unk195;
    pG->Save_unk780D = Main.Unk196;
}

INCLUDE_ASM("asm/nonmatchings/ovl/mem_card/mem_card", func_801C57E0);

/* Card_mess_make: build the Shift-JIS title of the card file: "RE3  NN.[area]/save count[clear count]E|H" */
void func_801C58FC(char *pTitle, void *pHead, UCHAR Slot)
{
    tagGLOBAL *pG = &G;
    UCHAR No = Slot + 1;

    func_8001012C(pTitle, "\x82\x71\x82\x64\x82\x52\x81\x40\x82\x4F\x82\x4F\x81\x44\x81\x75", 16);
    func_8001012C(pTitle + 0x10, D_801C5EDC[pG->Save_area], 0x1C);
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
LONG func_801C5B20(void *pHead, LONG Port, char *pName, char *pTitle, ULONG Size)
{
    _CARD *pCard = pHead;
    UCHAR Verify[0x200];
    ULONG Try;
    LONG Fd;
    LONG Cnt;
    LONG Got;
    LONG Ret;

    pCard->Block_entry = Size / 8192 + (Size % 8192 != 0);
    D_801C6838[2] = Port + '0';
    strcpy(&D_801C6838[5], pName);
    strcpy(pCard->Title, pTitle);
    close(open(D_801C6838, (pCard->Block_entry << 16) | 0x200));
    for (Try = 0; ; ) {
        for (Cnt = 3; (Fd = open(D_801C6838, 2)) == -1; ) {
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
        Fd = open(D_801C6838, 1);
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
LONG func_801C5C90(LONG Port)
{
    D_801C6838[2] = Port + '0';
    return format(D_801C6838) ^ 1;
}

/* Mess_center_x: x that centres message Msg on the 320 pixel screen (Mode 0: fixed 14 pixel glyphs) */
SHORT func_801C5CC0(LONG Msg, LONG Mode)
{
    LONG Width;
    ULONG i;
    UCHAR *p;

    Width = 0;
    p = MESS(Msg);
    for (;;) {
        if (*p >= 0xEE) {
            for (i = 0; i < 6; i++) {
                if (*p == D_801C6870[i]) {
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
SHORT func_801C5D98(LONG Msg)
{
    LONG Width;
    ULONG i;
    UCHAR *p;

    Width = 0;
    p = MESS(Msg);
    for (;;) {
        if (*p >= 0xEE) {
            for (i = 0; i < 6; i++) {
                if (*p == D_801C6870[i]) {
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

INCLUDE_ASM("asm/nonmatchings/ovl/mem_card/mem_card", func_801C5E64);

INCLUDE_ASM("asm/nonmatchings/ovl/mem_card/mem_card", func_801C5E74);

INCLUDE_ASM("asm/nonmatchings/ovl/mem_card/mem_card", func_801C5E84);
