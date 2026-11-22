#ifndef LINE_SCR_H
#define LINE_SCR_H
#include "bio3.h"

/* Screen line / quake effect work (RE2 struct tagLINE_SCR, LINE_SCR.C).
 * RE3 keeps RE2's file statics (rect, speed, mode, ...) at the end of the struct. */
typedef struct tagLINE_SCR {
    UCHAR r_no_0;                      /* 0x0000 */
    UCHAR r_no_1;                      /* 0x0001 */
    UCHAR Line_ctr0;                   /* 0x0002 */
    UCHAR Quake_ctr0;                  /* 0x0003 */
    ULONG Keep_tag0;                   /* 0x0004 */
    ULONG *pPrim_keep0;                /* 0x0008 */
    ULONG *pOt;                        /* 0x000C */
    DR_MOVE Line_move[2][240];         /* 0x0010 */
    ULONG Null_prim[2];                /* 0x2D10 */
    RECT Rect;                         /* 0x2D18 */
    UCHAR Unk2D20[6];                  /* 0x2D20 effect params (RE2 file statics speed/mode/...?) */
} LINE_SCR;
extern LINE_SCR Line;                  /* 0x800C79F8 */
/* quake offsets: 16 (X, Y) byte pairs each, indexed by Quake_ctr0 & 0xF */
extern UCHAR quake_tbl1[32];           /* 0x800989B0 */
extern UCHAR quake_tbl2[32];           /* 0x800989D0 */

void Scr_effect(void); /* Scr_effect */
void Scr_quake(ULONG Amp); /* Scr_quake */
void line_trans(LONG Line_no, LONG Slide_ofs, LONG Copy_ofs, LONG Draw_ofs); /* line_trans */
void line_scr_end(void); /* line_scr_end */
void line_mode_set(ULONG Ot_no, ULONG Prio); /* line_mode_set */
void Line_work_init(void); /* Line_work_init */

#endif /* LINE_SCR_H */
