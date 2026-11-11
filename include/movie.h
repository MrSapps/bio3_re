#ifndef MOVIE_H
#define MOVIE_H
#include "bio3.h"
#include "main.h"

/* Movie playback work (RE2 struct tagVM_WORK with 4 extra bytes in front) */
typedef struct tagVM_WORK {
    ULONG Unk00;                       /* 0x00 */
    ULONG *Rl_buff[2];                 /* 0x04 */
    ULONG *pImage_buff;                /* 0x0C */
    USHORT End_frame;                  /* 0x10 */
    USHORT Movie_volume;               /* 0x12 */
    UCHAR Rno;                         /* 0x14 */
    UCHAR Cbuf_id;                     /* 0x15 */
    USHORT Movie_w;                    /* 0x16 */
    UCHAR Movie_sync;                  /* 0x18 */
    UCHAR pad19[0x2E - 0x19];
    UCHAR Movie_Isrgb24;               /* 0x2E RE2 G.Movie_Isrgb24 */
} VM_WORK;
extern VM_WORK Vm_work;                /* 0x800DCD88 */

/* Movie display work (RE2 struct tagVM_DISP, same layout) */
typedef struct tagVM_DISP {
    RECT Rect;                         /* 0x00 MDEC strip upload rect */
    TILE Cinesco[2][2];                /* 0x08 */
    ULONG Cine_switch;                 /* 0x48 */
    USHORT Pos_x;                      /* 0x4C */
    USHORT Cur_pos_x;                  /* 0x4E */
    USHORT Pos_y;                      /* 0x50 */
    USHORT Width;                      /* 0x52 */
    USHORT Size;                       /* 0x54 */
    USHORT Count;                      /* 0x56 */
    USHORT Loop;                       /* 0x58 */
    USHORT Attribute;                  /* 0x5A */
} VM_DISP;
extern VM_DISP Vm_disp;                /* 0x800DCD20 */

extern ULONG StCdIntrFlag;             /* 0x800E15FC libds stream interrupt pending flag */

extern DslATV *pAtv;                   /* 0x800DCDC8 */
extern UCHAR Stereo_flg;               /* 0x800E1185 RE2 G.Stereo_flg: 0 stereo, else mono */

void Close_movie(void);
void Movie_xa_start(void);
void Movie_xa_stop(void);
void Set_movie_volume2(ULONG Volume);
void RuntoImage(void);
void Mdec_callback(void);
void func_8008F9FC(ULONG *buf, LONG mode); /* RE2 DecDCTin */
void func_8008FA78(ULONG *buf, LONG size); /* RE2 DecDCTout */
void func_8008FA98(LONG mode);         /* RE2 DecDCTinSync */
void func_8008FB34(void (*func)());    /* RE2 Close_movie calls DecDCToutCallback(NULL) here */
void func_80074658(LONG Arg);          /* RE2 Close_movie: SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SOFF) */

#endif /* MOVIE_H */
