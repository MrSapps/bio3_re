#ifndef OVL_RESULT_H
#define OVL_RESULT_H
/*
 * RESULT.BIN overlay (loaded at 0x801C2000): result screens of the main game
 * and of "The Mercenaries" mini game (times, grade, money, shop).
 * Data/bss of the overlay stay in asm (asm/data/ovl/result/data.data.s).
 */
#include "bio3.h"
#include "ovl/jill_obj.h"
#include "cd.h"
#include "main.h"
#include "monitor.h"
#include "mojidisp.h"
#include "snd.h"

/* piece of the packed OMBG.TIM file (offset/size inside the disc file) */
typedef struct tagRESULT_BG {
    ULONG Ofs;                         /* 0x0 */
    ULONG Size;                        /* 0x4 */
} RESULT_BG;


/* screen position for a printed value */
typedef struct tagRESULT_POS {
    SHORT X;
    SHORT Y;
} RESULT_POS;

/* result screen work (Result_wk) */
typedef struct tagRESULT_WORK {
    UCHAR Routine_0;                   /* 0x00 */
    UCHAR Routine_1;                   /* 0x01 */
    UCHAR Routine_2;                   /* 0x02 */
    UCHAR pad3;
    UCHAR Grade;                       /* 0x04 */
    UCHAR pad5;
    SHORT Timer;                       /* 0x06 */
    ULONG *pBuf;                       /* 0x08 */
    UCHAR Saves;                       /* 0x0C number of saves (main game result) */
    UCHAR padD;
    USHORT Score;                      /* 0x0E */
    USHORT Flg;                        /* 0x10 RWK_* bits: what is displayed */
    SHORT Count;                       /* 0x12 money left to count up/down */
    UCHAR pad14[0x16 - 0x14];
    signed char Cursor;                /* 0x16 */
    UCHAR Vloop;                       /* 0x17 saved Main.Vloop */
    SHORT Bonus;                       /* 0x18 */
    UCHAR pad1A[0x1C - 0x1A];
} RESULT_WORK;

/* G fields with other types than in tagGLOBAL, and past its end (The Mercenaries records) */
typedef struct tagGLOBAL_RESULT {
    UCHAR pad0[0x20FC];
    ULONG Padd_trg;                    /* 0x20FC tagGLOBAL.Padd_trg read as a ULONG */
    UCHAR pad2100[0x2108 - 0x2100];
    ULONG Key_trg;                     /* 0x2108 same as tagGLOBAL.Key_trg (here for one base register) */
    UCHAR pad210C[0x211A - 0x210C];
    USHORT Score;                      /* 0x211A same as tagGLOBAL.Score */
    UCHAR pad211C[0x24D6 - 0x211C];
    UCHAR Pl_id;                       /* 0x24D6 same as tagGLOBAL.Player.Id */
    UCHAR pad24D7[0x77F8 - 0x24D7];
    ULONG Game_flg[3];                 /* 0x77F8 same as tagGLOBAL.Game_flg */
    UCHAR pad7804[0x7848 - 0x7804];
    SHORT Mess_sel;                    /* 0x7848 same as tagGLOBAL.Mess_sel (message answer) */
    UCHAR pad784A[0x7860 - 0x784A];
    USHORT Time;                       /* 0x7860 same as tagGLOBAL.Time */
    UCHAR pad7862[0x7868 - 0x7862];
    ULONG Scenario_flg[8];             /* 0x7868 same as tagGLOBAL.Scenario_flg */
    UCHAR pad7888[0x7E94 - 0x7888];
    ULONG Cure_total;                  /* 0x7E94 same as tagGLOBAL.Cure_total */
    LONG Merc_time[3];                 /* 0x7E98 best time per character, -1 none */
    UCHAR Merc_grade[3];               /* 0x7EA4 best grade per character, 0xFF none */
    UCHAR pad7EA7;
    ULONG Merc_money[3];               /* 0x7EA8 best money per character */
    ULONG Money;                       /* 0x7EB4 money for the shop */
} GLOBAL_RESULT;
extern GLOBAL_RESULT Gr __asm__("G");

/* exe functions not in a shared header yet (game .c files declare some with other types) */

extern char Result_str_carlos[]; /* "CARLOS" */
extern char Result_str_nicholai[]; /* "NICHOLAI" */
extern char Result_str_mikhail[]; /* "MIKHAIL" */
extern char Result_str_no_time[]; /* "]]:]]:]]" no time */
extern char Result_str_time[]; /* "%1d%1d:%1d%1d:%1d%1d" */
extern char Result_str_grade[]; /* "%c" */
extern char Result_str_no_money[]; /* "@]]]]]]" no money */
extern char Result_str_money[]; /* "@%6d" */
extern char Result_str_buy[]; /* "BUY" */
extern char Result_str_exit[]; /* "EXIT" */
extern RESULT_BG Result_ombg_tbl[]; /* OMBG.TIM pieces */
extern UCHAR Result_ombg_sum[]; /* OMBG.TIM piece check sums */
extern UCHAR Result_mess[]; /* shop messages */
extern USHORT Result_mess_ofs[]; /* shop message offsets into Result_mess */
extern UCHAR Result_grade_chr[]; /* grade letters "FEDCBA" */
extern void (*Result_proc_tbl[])(void); /* task routines, index G.Ovl_mode */
extern void (*Result_merc_menu_tbl[])(RESULT_WORK *pWk); /* Mercenaries result routines */
extern RESULT_POS Result_merc_menu_pos[]; /* Mercenaries result text positions */
extern SEL_SPRT_DEF Result_merc_menu_spr[]; /* Mercenaries result sprites */
extern void (*Result_merc_clear_tbl[])(RESULT_WORK *pWk); /* Mercenaries shop routines */
extern RESULT_POS Result_merc_pos[]; /* shop text positions */
extern SEL_SPRT_DEF Result_merc_spr[]; /* shop sprites */
extern SHORT Result_shop_price[]; /* shop item prices */
extern signed char Result_shop_flg[]; /* shop item flag bits (G.Game_flg) */
extern void (*Result_merc_fail_tbl[])(RESULT_WORK *pWk); /* routine 2 routines */
extern void (*Result_main_tbl[])(RESULT_WORK *pWk); /* routine 3 routines */
extern RESULT_POS Result_main_pos[]; /* main game result text positions */
extern void (*Result_bonus_tbl[])(RESULT_WORK *pWk); /* routine 4 routines */
extern void (*Result_epilogue_tbl[])(RESULT_WORK *pWk); /* routine 5 routines */
extern SEL_OBJ Result_obj[]; /* sprite objects (30) */
extern RESULT_WORK Result_wk; /* result screen work */

void Result_ombg_load(ULONG *pAdr, UCHAR No);
void Result_task(void);
void Result_merc_menu(void);
LONG Result_merc_menu_disp(RESULT_WORK *pWk);
void Result_merc_menu_init(RESULT_WORK *pWk);
void Result_merc_menu_sel(RESULT_WORK *pWk);
void Result_merc_menu_exit(RESULT_WORK *pWk);
void Result_merc_bg_load(void);
void Result_merc_clear(void);
LONG Result_merc_clear_disp(RESULT_WORK *pWk);
void Result_merc_clear_init(RESULT_WORK *pWk);
void Result_merc_clear_count(RESULT_WORK *pWk);
void Result_merc_clear_exit(RESULT_WORK *pWk);
void Result_merc_clear_bg_load(void);
void Result_shop(RESULT_WORK *pWk);
void Result_merc_fail(void);
void Result_merc_fail_disp(RESULT_WORK *pWk);
void Result_merc_fail_init(RESULT_WORK *pWk);
void Result_merc_fail_count(RESULT_WORK *pWk);
void Result_merc_fail_exit(RESULT_WORK *pWk);
void Result_merc_fail_bg_load(void);
void Result_main(void);
LONG Result_disp(RESULT_WORK *pWk);
void Result_init(RESULT_WORK *pWk);
void Result_wait(RESULT_WORK *pWk);
void Result_exit(RESULT_WORK *pWk);
void Result_bg_load(void);
void Result_bonus(void);
void Result_bonus_disp(RESULT_WORK *pWk);
void Result_bonus_init(RESULT_WORK *pWk);
void Result_bonus_wait(RESULT_WORK *pWk);
void Result_bonus_exit(RESULT_WORK *pWk);
void Result_bonus_bg_load(void);
void Result_epilogue(void);
void Result_epilogue_disp(RESULT_WORK *pWk);
void Result_epilogue_init(RESULT_WORK *pWk);
void Result_epilogue_main(RESULT_WORK *pWk);
void Result_epilogue_exit(RESULT_WORK *pWk);
void Result_epilogue_bg_load(void);
void Result_grade_set(RESULT_WORK *pWk);
void Result_merc_grade_set(RESULT_WORK *pWk);

#endif /* OVL_RESULT_H */
