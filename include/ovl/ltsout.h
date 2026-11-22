#ifndef OVL_LTSOUT_H
#define OVL_LTSOUT_H
/*
 * LTSOUT.BIN overlay (loaded at 0x801C2000): the lamp ("lights out") puzzle.
 * Five switches toggle four lamps (a switch flips its lamp and both
 * neighbours); a random pattern from Lts_tbl must be turned into the one lamp
 * Lts_tbl names. Three rounds (G.Unk7888 bits 8, 9, 10), then the overlay sets
 * G.Unk7868 bit Flg_no. The lamps are shown through the cut flags
 * G.Cut_flg[G.Cut_no] (bits 0..3 lamp on, 4..7 answer lamp).
 */
#include "bio3.h"
#include "main.h"
#include "mojidisp.h"
#include "monitor.h"
#include "pad.h"
#include "pl_wep.h"
#include "sub.h"

#define LTS_SW_NUM 5                   /* switches 1..5, lamps 1..4 */

/* overlay work (module .data, 0x50 bytes) */
typedef struct tagLTS_WORK {
    UCHAR Routine;                     /* 0x00 */
    UCHAR Cnt;                         /* 0x01 */
    UCHAR Cnt2;                        /* 0x02 blink frame counter */
    UCHAR Esp_on;                      /* 0x03 effect already started for this round */
    UCHAR Run;                         /* 0x04 cleared to end the task */
    UCHAR Cut_old;                     /* 0x05 camera cut before the close-up */
    signed char Cursor;                /* 0x06 switch 1..5 */
    UCHAR Blink;                       /* 0x07 0 all off, 1 normal, 2 all on (bit flags while blinking) */
    UCHAR Pattern[4];                  /* 0x08 pattern of each round (index Stage + 1) */
    UCHAR Flg_no;                      /* 0x0C G.Unk7868 bit set when solved */
    UCHAR Mess;                        /* 0x0D first switch label */
    UCHAR Stage;                       /* 0x0E round 0..2 */
    UCHAR Answer;                      /* 0x0F lamp that must stay on */
    TILE Tile[2];                      /* 0x10 dark background, per draw buffer */
    DR_MODE Dr_mode[2];                /* 0x30 */
    UCHAR Lamp[8];                     /* 0x48 lamp state, index 1..4 (0, 5 are padding for the switch toggle) */
} LTS_WORK;

/* the event data at G.Unk21DC */
typedef struct tagLTS_EVT {
    UCHAR pad0[2];
    UCHAR Flg_no;                      /* 0x2 */
    UCHAR pad3;
    USHORT Cut_no;                     /* 0x4 close-up camera cut */
} LTS_EVT;

/* overlay .data (still in asm) */
extern UCHAR Lts_mess[] __asm__("D_801C2AD8"); /* switch labels */
extern USHORT Lts_mess_ofs[] __asm__("D_801C2AEC"); /* offset of each label in Lts_mess */
extern UCHAR Lts_tbl[10][8] __asm__("D_801C2AF8"); /* random patterns: lamps 1..4, answer lamp */
extern LTS_WORK Lts_wk __asm__("D_801C2B48");

void func_801C2018(void); /* main task */
void func_801C2370(void); /* puzzle state machine */
void func_801C28BC(void); /* draw */

#endif /* OVL_LTSOUT_H */
