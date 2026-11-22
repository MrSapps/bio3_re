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
    UCHAR Unk9;                        /* 0x09 */
    UCHAR UnkA;                        /* 0x0A */
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
extern ULONG Title_pdemo_cd[3] __asm__("D_801974B4");                  /* 0x801974B4 demo file per demo */
extern void (*Title_main_tbl[6])(TITLE_WORK *pTi) __asm__("D_801974C0"); /* 0x801974C0 */
extern void (*Title_sub_tbl[12])(TITLE_WORK *pTi) __asm__("D_801974D8");  /* 0x801974D8 */
extern TITLE_WORK Title_wk __asm__("D_80197508");                      /* 0x80197508 */

extern signed char Title_sin_tbl[256] __asm__("D_80098828");                 /* 0x80098828 0..255 sine wave */

/* overlay functions */
void func_801940E8(void);                          /* main loop */
void func_80194160(TITLE_WORK *pTi);               /* Capcom logo, intro movie (not split by splat) */
void func_80194444(TITLE_WORK *pTi);               /* title init */
void func_801945E4(TITLE_WORK *pTi);               /* menu sprites */
void func_80194ADC(TITLE_WORK *pTi);
void func_80194B08(TITLE_WORK *pTi);
void func_80194C4C(TITLE_WORK *pTi);               /* menu draw */
void func_80194C8C(TITLE_WORK *pTi);
void func_801952D8(TITLE_WORK *pTi);
void func_801954D0(TITLE_WORK *pTi);               /* start the game */
void func_80195564(TITLE_WORK *pTi);               /* menu */
void func_801955F4(TITLE_WORK *pTi);
void func_80195634(TITLE_WORK *pTi);
void func_801957F0(TITLE_WORK *pTi);
void func_80195988(TITLE_WORK *pTi);
void func_80195C68(TITLE_WORK *pTi);
void func_801961D4(TITLE_WORK *pTi);
void func_801961DC(TITLE_WORK *pTi);
void func_80196378(TITLE_WORK *pTi);
void func_8019644C(TITLE_WORK *pTi);
void func_80196528(TITLE_WORK *pTi);
void func_8019662C(TITLE_WORK *pTi);
void func_80196634(TITLE_WORK *pTi);
void func_801966CC(TITLE_WORK *pTi);
void func_80196800(TITLE_WORK *pTi);               /* extra menu */
void func_801971D8(TITLE_WORK *pTi);               /* extra menu draw */
void func_801973E4(void);                          /* load TITLE_DAT */
void func_80197414(void);                          /* load Capcom.tim */
void func_80197448(void);

/* exe prototypes not in a shared header yet (game .c files disagree on some of them) */
ULONG Cd_read(LONG Fno, ULONG *pAdr, LONG Mode, char *pName);
LONG Cd_read_s(LONG Fno, LONG a1, ULONG *pAdr, LONG a3, char *pName);
void Mapping_tim(ULONG *pTim);
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);
void func_8007809C(LONG a0, LONG a1, ULONG a2, ULONG a3);
void func_800783BC(ULONG no, ULONG size, ULONG a2);
void func_800782F4(ULONG no, ULONG id, ULONG *pAdr);
void func_80038634(ULONG Flg, SHORT Val);          /* pad.c */
LONG Init_movie_work(LONG No);
void Game_loop(void);
void D_80031BDC(void);                             /* game task entry (Task_execute) */
/* Task_lexecute's second argument is an overlay task number here */
void Task_lexecute_no(ULONG Level, ULONG No) __asm__("Task_lexecute");
void func_800791F0(void);
void func_8006D0D8(void);
void func_80078FE8(void);
void func_8007906C(void);
void Flg_on(ULONG *pFlg, ULONG Id);
extern LONG D_800C79C0;                            /* status screen state, cleared before the extra game */
extern LONG D_800C79CC;                            /* extra game character (func_8006D0D8) */

#endif /* OVL_TITLE_H */
