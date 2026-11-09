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
    UCHAR pad75CA[0x75E4 - 0x75CA];
    USHORT Mess_attr;                  /* 0x75E4 print attr: bits 3-7 colour, 0x8000 */
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
long func_8002F440(ULONG Pos, USHORT Attr, void *pArgs);
void Moji_trans_main(void);
void func_8002FEE0(MOJIDISP *pMoji);
void func_80030CB8(MOJIDISP *pMoji);

#endif /* MOJIDISP_H */
