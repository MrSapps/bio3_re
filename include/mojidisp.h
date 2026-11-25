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
    LINE_F3 Cursor_line[2][4];         /* 0x7508 message cursor triangle, index G.Cbuf_id (only [0] drawn) */
    UCHAR Mess_flg;                    /* 0x75C8 */
    UCHAR Unk75C9;                     /* 0x75C9 set with the print attr (Print_color) */
    UCHAR Mess_r_no0;                  /* 0x75CA RE2 Mess_r_no0 */
    UCHAR Mess_item;                   /* 0x75CB RE2 Mess_item */
    UCHAR Mess_type;                   /* 0x75CC */
    UCHAR Mess_wait;                   /* 0x75CD */
    UCHAR Mess_time;                   /* 0x75CE cursor blink counter */
    UCHAR Unk75CF;                     /* 0x75CF */
    UCHAR *Mess_top;                   /* 0x75D0 RE2 Mess_top */
    UCHAR *Mess_end;                   /* 0x75D4 RE2 Mess_end */
    UCHAR *Mess_svp;                   /* 0x75D8 RE2 Mess_svp */
    USHORT Mess_pos_x;                 /* 0x75DC RE2 Mess_pos_x */
    USHORT Mess_pos_y;                 /* 0x75DE RE2 Mess_pos_y */
    USHORT Pos_x_def;                  /* 0x75E0 RE2 Pos_x_def */
    USHORT Pos_y_def;                  /* 0x75E2 RE2 Pos_y_def */
    USHORT Mess_attr;                  /* 0x75E4 print attr: bits 3-7 colour, 0x8000 */
    USHORT Mess_attr2;                 /* 0x75E6 Mess_set Attr | 0x100 */
    ULONG Unk75E8;                     /* 0x75E8 */
    ULONG Timer;                       /* 0x75EC RE2 Timer (select input delay) */
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
void Mess_print(ULONG x, ULONG y, UCHAR *pStr, ULONG Attr);

extern UCHAR Mess_sel_text[];
extern UCHAR Mess_sys_text[];
extern USHORT Mess_sys_ofs[];
extern UCHAR Mess_st_text[]; /* message text banks (Mess_set) */
extern USHORT Mess_st_ofs[];
extern UCHAR Item_name_text[];
extern USHORT Item_name_ofs[];
extern UCHAR D_800C703F;
extern ULONG Item_chk_flg[];
extern UCHAR D_800D4433;

void Print8_put(MOJIDISP *pMoji, UCHAR *pStr, ULONG Xy, ULONG Attr);
void Print14_put(MOJIDISP *pMoji, UCHAR *pStr, ULONG Xy, ULONG Attr);
LONG Mess_set(ULONG Pos_xy, USHORT Attr, ULONG Mess_no, ULONG Stop_data);
ULONG Moji_recip(ULONG Val);
void Branch_disp(ULONG Arg0, UCHAR *pStr); /* RE2 has 4 args, RE3 reads only a0/a1 */
void Live_sel_disp(MOJIDISP *pMoji, UCHAR *pData);
void Name_disp(LONG x, LONG y, ULONG Attr, ULONG Item);
UCHAR *name_ptr_set(UCHAR No);
LONG Mess_char_width(ULONG Mode, ULONG Code, LONG *pPos);
LONG Moji_char_width(ULONG Mode, ULONG Code, LONG *pPos);
void Mess_hint_task(void);

#endif /* MOJIDISP_H */
