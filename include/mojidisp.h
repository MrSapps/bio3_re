#ifndef MOJIDISP_H
#define MOJIDISP_H
#include "bio3.h"

/* Message / font display work (RE2 struct tagMOJIDISP; RE3 uses DR_TPAGE and
 * different primitive counts). Only the fields proven so far. */
typedef struct tagMOJIDISP {
    DR_TPAGE Moji_mode[2][16];         /* 0x0000 */
    SPRT_8 Prim_sprt_8[2][128];        /* 0x0100 */
    SPRT Prim_sprt[2][640];            /* 0x1100 */
    SPRT_8 *pPrim8_cur;                /* 0x7500 */
    SPRT *pPrim14_cur;                 /* 0x7504 */
    UCHAR pad7508[0x75C8 - 0x7508];
    UCHAR Mess_flg;                    /* 0x75C8 */
    UCHAR Unk75C9;                     /* 0x75C9 set with the print attr (Print_color) */
    UCHAR Mess_r_no0;                  /* 0x75CA RE2 Mess_r_no0 */
    UCHAR Mess_item;                   /* 0x75CB RE2 Mess_item */
    UCHAR pad75CC[0x75D0 - 0x75CC];
    UCHAR *Mess_top;                   /* 0x75D0 RE2 Mess_top */
    UCHAR pad75D4[0x75E0 - 0x75D4];
    USHORT Pos_x_def;                  /* 0x75E0 RE2 Pos_x_def */
    USHORT Pos_y_def;                  /* 0x75E2 RE2 Pos_y_def */
    USHORT Mess_attr;                  /* 0x75E4 print attr: bits 3-7 colour, 0x8000 */
    USHORT Mess_attr2;                 /* 0x75E6 Mess_set Attr | 0x100 */
    UCHAR pad75E8[0x75F0 - 0x75E8];
    ULONG Stop_keep;                   /* 0x75F0 RE2 Stop_keep */
    ULONG Stop_data;                   /* 0x75F4 RE2 Stop_data */
} MOJIDISP;
extern MOJIDISP Moji;                  /* 0x800D4590 */

/* Glyph spacing table, 2 bytes per glyph: [0] = left bearing, [1] = width (RE2 msg_space) */
extern signed char msg_space[];        /* 0x80098DD0 */

void Moji_init(void);
void Moji_mode_init(void);

/* 0x8002F2F8.. (chunk 80020298) */
void Print_color(UCHAR A0, UCHAR Col, USHORT Attr);
void Print8(USHORT X, USHORT Y, UCHAR *pFmt, ...);
void Print14(USHORT X, USHORT Y, UCHAR *pFmt, ...);
void Print8_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...);
void Print14_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...);
long Print_main(ULONG Pos, USHORT Attr, void *pArgs);
void Moji_trans_main(void);
void Mess_disp(MOJIDISP *pMoji);
void mess_trans(MOJIDISP *pMoji);

#endif /* MOJIDISP_H */
