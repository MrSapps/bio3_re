#ifndef OVL_R214_OL_H
#define OVL_R214_OL_H
/*
 * R214_OL.BIN overlay (loaded at 0x801C2000): the room 214 mixing puzzle.
 * Each step the player adds (Val[n] = Val[n - 1] + 15 * n) or removes
 * (- 5 * n, - 10 * n with G.Scenario_flg bit 0x33) and an effect shows the choice;
 * after five steps the final value picks the result (G.Room_flg[0] bits 0..3).
 * The values are drawn as sprite digits.
 */
#include "bio3.h"
#include "main.h"
#include "monitor.h"
#include "pad.h"
#include "pl_wep.h"
#include "sub.h"

#define R214_VAL_NUM 5

/* overlay work (module .data + BSS past the file end, 0x34C bytes) */
typedef struct tagR214_WORK {
    UCHAR Unk0;                        /* 0x000 */
    UCHAR Routine;                     /* 0x001 */
    UCHAR Num;                         /* 0x002 steps done + 1 (values in use) */
    UCHAR Unk3;                        /* 0x003 */
    UCHAR Run;                         /* 0x004 cleared to end the task */
    UCHAR Cut_old;                     /* 0x005 camera cut before the close-up */
    SHORT Timer;                       /* 0x006 */
    UCHAR pad8[2];
    SHORT Val[R214_VAL_NUM];           /* 0x00A */
    UCHAR pad14[2];
    UCHAR Sel[4];                      /* 0x016 choice of each step (1: removed) */
    UCHAR pad1A[2];
    SPRT Sprt[20][2];                  /* 0x01C digit sprites, [n][Cbuf_id] */
    DR_TPAGE Tpage[2];                 /* 0x33C OT entry the digits are linked into */
} R214_WORK;

/* the event data at G.pAot_dat */
typedef struct tagR214_EVT {
    UCHAR pad0[4];
    USHORT Cut_no;                     /* 0x4 close-up camera cut */
} R214_EVT;

/* overlay .data (still in asm) */
extern UCHAR R214_mess[]; /* step messages */
extern USHORT R214_mess_ofs[]; /* offset of each message in R214_mess */
extern SVECTOR R214_pos_ok0[2]; /* effect position, result 0 */
extern SVECTOR R214_pos_ok1[2]; /* effect position, result 1 */
extern SVECTOR R214_pos[4][2]; /* effect position of each step: added, removed */
extern USHORT R214_digit_x[]; /* x of each value */
extern R214_WORK R214_wk;

void R214_main(void); /* main task */
void R214_ctr(void); /* puzzle state machine */
void R214_draw(void); /* draw the values */
void R214_digit_set(ULONG *pOt, SPRT **ppSp, USHORT x, SHORT n); /* one digit sprite */

#endif /* OVL_R214_OL_H */
