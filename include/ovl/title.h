#ifndef OVL_TITLE_H
#define OVL_TITLE_H
/*
 * TITLE.BIN overlay (loaded at 0x80194000): Capcom logo, title menu (new game,
 * load, option) and the extra menu ("The Mercenaries" character select).
 * The routines take the title work (Title_wk) as argument, like RE2's TITLE.C.
 */
#include "bio3.h"
#include "em_sub.h"
#include "main.h"
#include "monitor.h"
#include "pad.h"
#include "pl_wep.h"
#include "sub.h"
#include "snd.h"

/* one draw buffer of primitives (0x144 bytes); the title menu and the extra
 * menu use the area differently */
typedef union tagTITLE_BUF {
    struct {
        UCHAR pad0[0x50];
        SPRT Logo;                     /* 0x050 */
        SPRT Item[8];                  /* 0x064 menu items, then their shadows */
        UCHAR pad104[0x10C - 0x104];
        DR_TPAGE Tpage;                /* 0x10C */
        UCHAR pad114[0x144 - 0x114];
    } Menu;
    struct {
        UCHAR pad0[0x3C];
        SPRT Pic;                      /* 0x03C picture of the selected character */
        UCHAR pad50[0x104 - 0x50];
        DR_TPAGE Tpage;                /* 0x104 */
        UCHAR pad10C[0x114 - 0x10C];
        TILE Shade[3];                 /* 0x114 dark tiles over the other three */
    } Ex;
} TITLE_BUF;

/* title work (RE2 struct TITLE_WORK Ti), 0x2A0 bytes */
typedef struct tagTITLE_WORK {
    UCHAR Main_rtn;                    /* 0x00 index into Title_main_tbl */
    UCHAR Sub_rtn;                     /* 0x01 index into Title_sub_tbl */
    UCHAR Page;                        /* 0x02 */
    UCHAR pad3;
    signed char Cur;                   /* 0x04 cursor */
    signed char Cur_old;               /* 0x05 */
    UCHAR pad6[3];
    UCHAR Disp_add;                    /* 0x09 RE2 Disp_add (set, never read) */
    UCHAR Disp_cnt;                    /* 0x0A RE2 Disp_cnt */
    UCHAR padB;
    UCHAR Mode;                        /* 0x0C menu draw mode */
    UCHAR Fade_cnt;                    /* 0x0D menu item fade, 0..5 */
    UCHAR Kido;                        /* 0x0E cursor brightness */
    UCHAR Kido_cnt;                    /* 0x0F index into the sine table */
    UCHAR pad10[4];
    SHORT Moji_y;                      /* 0x14 */
    USHORT Timer;                      /* 0x16 demo timer */
    TITLE_BUF Buf[2];                  /* 0x18 */
} TITLE_WORK;

/* Main_rtn, Sub_rtn, Page and pad3 written as one word */
#define TITLE_RTN_SET(pTi, Rtn) (*(ULONG *)&(pTi)->Main_rtn = (Rtn))

/* overlay .data (still in asm) */
extern ULONG Title_pdemo_cd[3]; /* 0x801974B4 demo file per demo */
extern void (*Title_main_tbl[6])(TITLE_WORK *pTi); /* 0x801974C0 */
extern void (*Title_sub_tbl[12])(TITLE_WORK *pTi); /* 0x801974D8 */
extern TITLE_WORK Title_wk; /* 0x80197508 */

extern signed char Title_sin_tbl[256] __asm__("Rsin_tbl"); /* 0x80098828 0..255 sine wave */

/* overlay functions */
void Title(void); /* main loop */
void Capcom_logo(TITLE_WORK *pTi); /* Capcom logo, intro movie (not split by splat) */
void Title_init(TITLE_WORK *pTi); /* title init */
void Title_sprt_init(TITLE_WORK *pTi); /* menu sprites */
void Title_main_wait(TITLE_WORK *pTi);
void Title_logo_slide(TITLE_WORK *pTi);
void Title_menu_disp(TITLE_WORK *pTi); /* menu draw */
void Title_menu_disp_nml(TITLE_WORK *pTi);
void Title_menu_disp_ex(TITLE_WORK *pTi);
void Title_main_game(TITLE_WORK *pTi); /* start the game */
void Title_main(TITLE_WORK *pTi); /* menu */
void Title_main_select(TITLE_WORK *pTi);
void Title_sel_menu(TITLE_WORK *pTi);
void Title_sel_menu_ex(TITLE_WORK *pTi);
void Title_sel_start(TITLE_WORK *pTi);
void Title_sel_mode(TITLE_WORK *pTi);
void Title_sub_nop3(TITLE_WORK *pTi);
void Title_game_mode_sel(TITLE_WORK *pTi);
void Title_main_load(TITLE_WORK *pTi);
void Title_main_movie(TITLE_WORK *pTi);
void Title_main_option(TITLE_WORK *pTi);
void Title_sub_nop8(TITLE_WORK *pTi);
void Title_ex_off(TITLE_WORK *pTi);
void Title_main_demo(TITLE_WORK *pTi);
void Title_ex_init(TITLE_WORK *pTi); /* extra menu */
void Title_ex_disp(TITLE_WORK *pTi); /* extra menu draw */
void Title_bg_load(void); /* load TITLE_DAT */
void Capcom_logo_load(void); /* load Capcom.tim */
void Title_bg_reload(void);

/* exe prototypes not in a shared header yet (game .c files disagree on some of them) */

extern const char Title_str_init_sub[];
extern const char Title_str_omake_bgm[];
extern const char Title_str_omake_vb[];
extern const char Title_str_init_tbl[];

#endif /* OVL_TITLE_H */
