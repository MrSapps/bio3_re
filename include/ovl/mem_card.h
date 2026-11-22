#ifndef OVL_MEM_CARD_H
#define OVL_MEM_CARD_H
/*
 * MEM_CARD.BIN overlay (0x801C2000): memory card save/load screen.
 * RE2 predecessor: ref/bio2-ds/Bio2/prog/MEM_CARD.C (names follow it where the code matches).
 */
#include "bio3.h"
#include "sub.h"

/* memory card file header (RE2 struct tag_CARD, 0x200 bytes) */
typedef struct tag_CARD {
    UCHAR Magic[2];                    /* 0x000 "SC" */
    UCHAR Type;                        /* 0x002 icon frames */
    UCHAR Block_entry;                 /* 0x003 blocks used */
    UCHAR Title[64];                   /* 0x004 Shift-JIS title */
    UCHAR Reserve[28];                 /* 0x044 */
    UCHAR Clut[32];                    /* 0x060 */
    UCHAR Icon[3][128];                /* 0x080 */
} _CARD;

/* Directory listing of the save files on a card (RE2 struct tagCFILE_INFO, 0x18C bytes).
 * Titles are stored at the index of the file's first block. */
typedef struct tagCFILE_INFO {
    UCHAR Title[15][22];               /* 0x000 file names */
    UCHAR Num;                         /* 0x14A blocks used (sum of size / 8 KB) */
    UCHAR Game_num;                    /* 0x14B number of RE3 save files (Search_f_num) */
    ULONG Total_size;                  /* 0x14C */
    ULONG Size[15];                    /* 0x150 */
} CFILE_INFO;

/* Save summary: the 0x80 bytes of G from 0x77F0 (game time, flags, player state), one per slot */
typedef struct tagSAVE_INFO {
    ULONG Game_play_time;              /* 0x00 G+0x77F0 */
    ULONG Status_flg;                  /* 0x04 G+0x77F4 */
    ULONG Flg;                         /* 0x08 G+0x77F8 flag bits (Flg_ck) */
    SHORT Cdir_y;                      /* 0x0C */
    SHORT Pos_x;                       /* 0x0E */
    SHORT Pos_y;                       /* 0x10 */
    SHORT Pos_z;                       /* 0x12 */
    SHORT Life;                        /* 0x14 */
    USHORT Unk16;                      /* 0x16 shown as two digits when < 100 */
    UCHAR Save_cnt;                    /* 0x18 */
    UCHAR Unk19;                       /* 0x19 */
    UCHAR Poison;                      /* 0x1A */
    UCHAR Save_area;                   /* 0x1B area name index */
    UCHAR Rest[0x80 - 0x1C];
} SAVE_INFO;

/* Memory card work (Mwork, 0x12C0 bytes; RE2 struct tagMWORK, reworked).
 * Rno: 0 slot check, 1 retry/quit, 2 slot select, 3 error, 4 checking, 5 read directory,
 * 6 file select, 7 file chosen, 8 save wait, 9 save, 10 saved animation, 11 load,
 * 12 format?, 13 format, 14 file error, 15 overwrite?, 16 continue?, 17 after save menu,
 * 18 end, 19 load wait. */
typedef struct tagMWORK {
    UCHAR Rno;                         /* 0x000 state, index into Card_rno_tbl / Card_disp_tbl */
    UCHAR Rno1;                        /* 0x001 sub state */
    UCHAR Load_flg;                    /* 0x002 1: load screen, 0: save screen */
    UCHAR Vloop_bak;                   /* 0x003 Main.Vloop on entry */
    signed char Cursor;                /* 0x004 */
    UCHAR Cursor_old;                  /* 0x005 */
    UCHAR Cursor_bak;                  /* 0x006 slot to return to */
    UCHAR Cursor_max;                  /* 0x007 */
    signed char Moji_no;               /* 0x008 character being revealed in the saved slot */
    signed char Moji_no_old;           /* 0x009 */
    UCHAR Slot;                        /* 0x00A selected slot */
    signed char Key_wait;              /* 0x00B key repeat delay */
    UCHAR Blink;                       /* 0x00C cursor blink counter */
    UCHAR Wait;                        /* 0x00D */
    UCHAR Port;                        /* 0x00E card slot 0/1 */
    UCHAR Err;                         /* 0x00F error / message number */
    UCHAR Scrl_top;                    /* 0x010 first visible slot */
    UCHAR Poll_flg;                    /* 0x011 card status poll done */
    UCHAR Card_chg;                    /* 0x012 card changed */
    UCHAR Key_lock;                    /* 0x013 no cursor moves */
    UCHAR Item_bak;                    /* 0x014 */
    UCHAR Anim_step;                   /* 0x015 index into Mc_wait_tbl */
    UCHAR Rno_loop;                    /* 0x016 run the state routine again this frame */
    UCHAR Anim_wait;                   /* 0x017 */
    UCHAR Exit_flg;                    /* 0x018 */
    UCHAR Anim_mode;                   /* 0x019 */
    UCHAR Saved_flg;                   /* 0x01A */
    UCHAR Timer;                       /* 0x01B */
    USHORT Key_trg;                    /* 0x01C G.Key_trg saved over a frame */
    USHORT Key_rep;                    /* 0x01E G.Unk20FC saved over a frame */
    UCHAR F_num[16];                   /* 0x020 card file index per slot; 0x80 empty, 0x40 used by another game */
    ULONG Total_size;                  /* 0x030 */
    CFILE_INFO File_info;              /* 0x034 */
    UCHAR Card_buf[0x980];             /* 0x1C0 save image (0x200 card header, then the game data) */
    SAVE_INFO Save_info[15];           /* 0xB40 */
} MWORK;

/* overlay functions (RE2 names where the code matches) */
void Mem_card(void); /* Mem_card */
void Card_access(void); /* Card_access */
LONG Card_cursor(MWORK *pMw); /* Card_cursor */
void Cardaccess_init(MWORK *pMw); /* Cardaccess_init */
void Cardaccess_exit(MWORK *pMw); /* Cardaccess_exit */
void Cursor_disp(ULONG X, ULONG Y); /* Cursor_disp */
void Slot_disp(LONG Port); /* Slot_disp */
LONG Card_check(LONG Port); /* Card_check */
void Clear_event_wait(void); /* Clear_event_wait */
void Clear_event_wait2(void); /* Clear_event_wait2 */
LONG Get_card_event(void); /* Get_card_event */
LONG Get_card_event2(void); /* Get_card_event2 */
LONG Card_clear(LONG Port); /* Card_clear */
LONG Card_format_check(LONG Port); /* Card_format_check */
LONG Check_card(UCHAR *F_num); /* Check_card */
void Card_search(LONG Port, CFILE_INFO *pInfo, char *pPattern); /* Card_search */
void Search_f_num(UCHAR *F_num); /* Search_f_num */
LONG Card_read(void *pBuf, char *pName, LONG Size, LONG Offset); /* Card_read */
LONG Card_poll(LONG Port); /* Card_poll */
void Save_push(void); /* Save_push */
void Load_pop(void); /* Load_pop */
void Card_mess_make(char *pTitle, void *pHead, UCHAR Slot); /* Card_mess_make */
LONG Card_write(void *pHead, LONG Port, char *pName, char *pTitle, ULONG Size); /* Card_write */
LONG Card_format(LONG Port); /* Card_format */
SHORT Mess_center_x(LONG Msg, LONG Mode); /* Mess_center_x */
SHORT Mess_width(LONG Msg); /* Mess_width */

/* 800636D4.c (no module header yet). It defines them with char types; the overlay calls
 * them as taking and returning int (no andi / sign extension at the call). */

extern char Mc_area_name[][29]; /* Mc_area_name: save area names (Shift-JIS) */
extern UCHAR Mess_text[]; /* Mess_text: message pool */
extern USHORT Mess_offs[]; /* Mess_offs: message offsets into Mess_text */
extern MWORK *pMwork; /* pMwork (-> Mwork) */
extern char Mc_fname_tmpl[]; /* Mc_fname_tmpl: "bu?0:" + file name */
extern UCHAR Mc_wait_tbl[]; /* Mc_wait_tbl: frames per step of the "saved" animation */
extern UCHAR Mess_ctrl[6]; /* Mess_ctrl: control codes; [0] ends a message, the others take one operand */
extern void (*Card_rno_tbl[])(MWORK *); /* Card_rno_tbl: state routines */
extern void (*Card_disp_tbl[])(MWORK *); /* Card_disp_tbl: state displays */
extern LONG Mc_moji_se; /* sound already played for the revealed character */
extern UCHAR Mc_item_bak[0x28]; /* item work backup while saving */
extern char Mc_old_name[]; /* name of the file being overwritten */
extern char Mc_title[0x48]; /* card file title (Shift-JIS) */
extern UCHAR Mc_slot_line[0x50]; /* slot line: area name */
extern MWORK Mwork; /* the card work itself */
extern UCHAR Mc_slot_line2[16]; /* slot line: save count / clear count */
extern CFILE_INFO Mc_file_info; /* directory of the RE3 files on the card */

void Card_rno00_slot_check(MWORK *pMw);
void Card_rno01(MWORK *pMw);
void Card_rno02_slot_sel(MWORK *pMw);
void Card_rno03_err(MWORK *pMw);
void Card_rno04_checking(MWORK *pMw);
void Card_rno05_read_dir(MWORK *pMw);
void Card_rno06_file_sel(MWORK *pMw);
void Card_rno11_after_save(MWORK *pMw);
void Card_rno07_file_ok(MWORK *pMw);
void Card_rno08_save_wait(MWORK *pMw);
void Card_rno13_load_wait(MWORK *pMw);
void Card_rno09_save(MWORK *pMw);
void Card_rno0A_saved(MWORK *pMw);
void Card_rno0B_load(MWORK *pMw);
void Card_rno0C_format_ask(MWORK *pMw);
void Card_rno0D_format(MWORK *pMw);
void Card_rno0E_file_err(MWORK *pMw);
void Card_rno0F_overwrite(MWORK *pMw);
void Card_rno10_continue(MWORK *pMw);
void Card_rno12_end(MWORK *pMw);
void Card_disp_none(MWORK *pMw);
void Card_disp_checking(MWORK *pMw);
void Card_disp_file_list(MWORK *pMw);
void Card_disp_slot_sel(MWORK *pMw);
void Card_disp_format_ask(MWORK *pMw);
void Card_disp_retry(MWORK *pMw);
void Card_disp_err(MWORK *pMw);
void Card_disp_continue(MWORK *pMw);
void Card_disp_after_save(MWORK *pMw);
void Card_disp_end(MWORK *pMw);
long _card_info(long chan); /* _card_info */
long _card_load(long chan); /* _card_load */
long _card_clear(long chan); /* _card_clear */

#endif /* OVL_MEM_CARD_H */
