#ifndef OVL_STAFF_R_H
#define OVL_STAFF_R_H
/*
 * STAFF_R.BIN overlay (loaded at 0x801C2000): staff roll after the ending.
 * Credit pages (two 256-wide sprites each, STAFF_U_DAT) fade in and out over
 * scrolling backgrounds (STR_BG_DAT), with a closing message and logo.
 */
#include "bio3.h"
#include "cd.h"
#include "main.h"
#include "monitor.h"

/* credit page: left / right half, per draw buffer (0x70 bytes) */
typedef struct tagSTAFF_PAGE {
    SPRT L[2];                         /* 0x00 */
    SPRT R[2];                         /* 0x28 */
    DR_TPAGE Tl[2];                    /* 0x50 */
    DR_TPAGE Tr[2];                    /* 0x60 */
} STAFF_PAGE;

/* one sprite, per draw buffer (0x38 bytes) */
typedef struct tagSTAFF_SPR {
    SPRT Sp[2];                        /* 0x00 */
    DR_TPAGE Tp[2];                    /* 0x28 */
} STAFF_SPR;

/* overlay work (0x810 bytes) */
typedef struct tagSTAFF_WORK {
    UCHAR Rtn0;                        /* 0x000 Staff_main_tbl */
    UCHAR Rtn1;                        /* 0x001 Staff_roll_tbl */
    UCHAR Rtn2;                        /* 0x002 Staff_page_tbl / Staff_bg_tbl */
    UCHAR Rtn3;                        /* 0x003 */
    USHORT Timer;                      /* 0x004 */
    USHORT Timer2;                     /* 0x006 */
    USHORT Kido;                       /* 0x008 */
    USHORT Unk0A;                      /* 0x00A */
    ULONG *pStr_bg;                    /* 0x00C STR_BG_DAT (0x801064E0): page and line images */
    ULONG *pStaff_u;                   /* 0x010 STAFF_U_DAT (0x80100000): background images */
    ULONG *pBuf;                       /* 0x014 copy of the page images (0x8019C000) */
    SHORT Bg_no0;                      /* 0x018 background scrolling out, -1 none */
    SHORT Bg_no1;                      /* 0x01A background scrolling in */
    USHORT Rtn3_ret;                   /* 0x01C */
    USHORT End_flg;                    /* 0x01E */
    USHORT Ot0;                        /* 0x020 */
    USHORT Ot1;                        /* 0x022 */
    USHORT Ot2;                        /* 0x024 */
    UCHAR pad26[2];
    ULONG Mess_no;                     /* 0x028 */
    STAFF_PAGE Page[7];                /* 0x02C */
    STAFF_SPR Bg[12];                  /* 0x33C */
    STAFF_SPR Logo;                    /* 0x5DC */
    STAFF_SPR Line[1];                 /* 0x614 */
    UCHAR pad64C[0x810 - 0x64C];
} STAFF_WORK;

/* page sequencer (0x18 bytes) */
typedef struct tagSTAFF_SEQ {
    UCHAR Rtn0;                        /* 0x00 */
    UCHAR Rtn1;                        /* 0x01 */
    UCHAR Rtn2;                        /* 0x02 */
    UCHAR Rtn3;                        /* 0x03 */
    USHORT Timer;                      /* 0x04 */
    USHORT Timer2;                     /* 0x06 */
    USHORT Page;                       /* 0x08 */
    USHORT Unk0A;                      /* 0x0A */
    USHORT Ot0;                        /* 0x0C */
    USHORT Ot1;                        /* 0x0E */
    UCHAR pad10[8];
} STAFF_SEQ;

/* colour fade of the first two pages (0x10 bytes) */
typedef struct tagSTAFF_FADE {
    UCHAR Rtn;                         /* 0x00 */
    UCHAR pad1[3];
    ULONG End;                         /* 0x04 */
    UCHAR Col0[4];                     /* 0x08 fading out */
    UCHAR Col1[4];                     /* 0x0C fading in */
} STAFF_FADE;

/* texture page / clut row of the credit pages */
typedef struct tagSTAFF_TEX {
    ULONG Tp;                          /* 0x0 */
    ULONG Clut_y;                      /* 0x4 */
} STAFF_TEX;

/* message position and display time */
typedef struct tagSTAFF_MESS {
    LONG X;                            /* 0x0 */
    LONG Y;                            /* 0x4 */
    ULONG Time;                        /* 0x8 */
} STAFF_MESS;

/* G.Ctp.Tpage_clut (texture page / clut row used by Mapping_tim) referenced as a scalar,
 * as other game files do with D_800CCBBC: the code addresses it by symbol, not via &G */
extern USHORT G_tpage_clut __asm__("D_800CCBBC");

/* overlay .data (still in asm) */
extern UCHAR Staff_mess_data[] __asm__("D_801C3E24");
extern USHORT Staff_mess_ofs[] __asm__("D_801C404C");
extern void (*Staff_main_tbl[])(void) __asm__("D_801C4050");
extern STAFF_TEX Staff_tex[7] __asm__("D_801C405C");
extern STAFF_MESS Staff_mess[] __asm__("D_801C4094");
extern void (*Staff_roll_tbl[])(void) __asm__("D_801C40A0");
extern void (*Staff_page_tbl[])(void) __asm__("D_801C40B0");
extern void (*Staff_bg_tbl[])(void) __asm__("D_801C40BC");
extern STAFF_SEQ Staff_seq __asm__("D_801C40D0");
extern STAFF_WORK Staff_wk __asm__("D_801C40E8");
extern STAFF_FADE Staff_fade __asm__("D_801C48F8");

void func_801C206C(void);              /* main task */
void func_801C2104(void);              /* init */
void func_801C21E8(void);              /* roll */
void func_801C2228(void);              /* exit */
void func_801C22AC(void);              /* roll 0: first pages init */
void func_801C256C(void);              /* roll 1: first pages */
void func_801C25FC(void);
void func_801C2604(void);
void func_801C2974(void);
void func_801C2984(void);              /* roll 2: backgrounds init */
void func_801C2B30(void);              /* roll 3: backgrounds and credit pages */
void func_801C2BB8(void);
void func_801C2CBC(void);
void func_801C2CE4(void);              /* credit page sequencer */
void func_801C2F8C(void);              /* background scroll */
void func_801C32DC(void);              /* logo */
void func_801C36F4(void);
void func_801C3704(void);              /* load and set up the credit pages */
void func_801C3A98(void);              /* cross-fade of the first two pages */

/* exe prototypes not in a shared header yet */
ULONG Cd_read(LONG Fno, ULONG *pAdr, LONG Mode, char *pName);
void Mapping_tim(ULONG *pTim);
LONG Mess_set(ULONG Pos_xy, USHORT Attr, UCHAR *pMess, ULONG Stop_data);
void Xa_play(ULONG Mode, ULONG No);
void func_800100A4(void *pDst, void *pSrc, ULONG Size); /* RE2 Memcpy */

#endif /* OVL_STAFF_R_H */
