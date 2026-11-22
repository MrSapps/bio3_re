#ifndef OVL_OPTION_H
#define OVL_OPTION_H
/* OPTION.BIN overlay (loaded at 0x801C2000): option / config screen (RE2 CONFIG.C, rewritten for RE3) */
#include "bio3.h"
#include "cd.h"
#include "main.h"
#include "monitor.h"
#include "pad.h"
#include "snd.h"

/* piece of the packed Option.dat file (offset in sectors, size in bytes) */
typedef struct tagOPT_FILE {
    ULONG Ofs;                         /* 0x0 */
    ULONG Size;                        /* 0x4 */
} OPT_FILE;

/* animated background piece (0x60 bytes, 20 of them) */
typedef struct tagOPT_PIECE {
    UCHAR Be_flg;                      /* 0x00 */
    UCHAR Id;                          /* 0x01 shape (index into D_801C5500) */
    UCHAR Type;                        /* 0x02 animation script (index into D_801C5548) */
    UCHAR Anim_no;                     /* 0x03 frame */
    UCHAR *pAnim;                      /* 0x04 {frame, time} pairs, 0xFF ends */
    SHORT Pos_x;                       /* 0x08 x << 6 */
    SHORT Pos_y;                       /* 0x0A y << 6 */
    USHORT Rot_y;                      /* 0x0C */
    SHORT Timer;                       /* 0x0E */
    POLY_FT4 Poly[2];                  /* 0x10 */
} OPT_PIECE;

/* option screen work (0x801C55A0, 0x1E04 bytes) */
typedef struct tagOPT_WORK {
    signed char Routine;               /* 0x00 page: index into D_801C5554 (main) / D_801C556C (draw) */
    signed char Step;                  /* 0x01 sub step of the current routine */
    signed char Routine_next;          /* 0x02 */
    UCHAR Vloop_bak;                   /* 0x03 Main.Vloop */
    signed char Cur;                   /* 0x04 cursor row */
    signed char Cur_old;               /* 0x05 */
    signed char Sub_cur;               /* 0x06 cursor inside a page */
    signed char Sub_cur_old;           /* 0x07 */
    UCHAR Bg_mode_bak;                 /* 0x08 Main.Bg_mode */
    UCHAR Unk191_bak;                  /* 0x09 Main.Unk191 */
    UCHAR Flg;                         /* 0x0A copy of Main.Unk196 (0x80 vibration off, 0x40 pad type C) */
    UCHAR Stereo;                      /* 0x0B copy of Stereo_flg */
    UCHAR Arrow;                       /* 0x0C pad type arrows lit */
    UCHAR No;                          /* 0x0D reset page: 0 YES, 1 NO */
    UCHAR No_old;                      /* 0x0E */
    UCHAR Reset;                       /* 0x0F game reset chosen */
    UCHAR Kido;                        /* 0x10 cursor brightness */
    UCHAR Timer;                       /* 0x11 */
    UCHAR Pad_type;                    /* 0x12 G.Pad_type seen last */
    signed char Pad_cfg;               /* 0x13 copy of Main.Unk195 (pad type A/B/C) */
    UCHAR Fade_mode;                   /* 0x14 */
    UCHAR Shade_on;                    /* 0x15 draw the dark panel and its lines */
    USHORT Hold;                       /* 0x16 frames a volume key is held */
    SHORT Vol[2];                      /* 0x18 BGM, SE volume 0..100 */
    SHORT Vol_bak[2];                  /* 0x1C Snd_work.Vol.Bgm/Unk7C8 on entry */
    SHORT Vol_old[2];                  /* 0x20 */
    SPRT Bg[2][3];                     /* 0x24 */
    DR_TPAGE Bg_tpage[2][2];           /* 0x9C */
    UCHAR Moji_prim[2][0xA00];         /* 0xBC SPRT pool for the text */
    DR_TPAGE Moji_tpage[2];            /* 0x14BC */
    SPRT *pMoji;                       /* 0x14CC */
    SPRT Icon[2][4];                   /* 0x14D0 */
    SPRT *pIcon;                       /* 0x1570 */
    SPRT Button[2][4];                 /* 0x1574 pad button picture (key config page), [0] used */
    OPT_PIECE Piece[20];               /* 0x1614 */
    TILE Shade[2];                     /* 0x1D94 */
    DR_TPAGE Shade_tpage[2];           /* 0x1DB4 */
    LINE_F2 Line[2][2];                /* 0x1DC4 */
} OPT_WORK;

/* the overlay .data is still asm, so data names are bound to its labels */
extern OPT_WORK Opt __asm__("D_801C55A0");

/* exe functions without a shared prototype (game .c files declare these locally, some with other types) */
ULONG Cd_read(LONG Fno, ULONG *pAdr, LONG Mode, char *pName);
void Snd_se_stad(ULONG code, VECTOR *pPos, SHORT a2, SHORT a3);
ULONG func_800102E8(void);             /* Rnd */
void func_80038634(ULONG Flg, SHORT Val); /* pad key repeat set */
void Mapping_tim(ULONG *pTim);
extern MATRIX GsIDMATRIX;              /* 0x80098970 libgs unit matrix */

#endif /* OVL_OPTION_H */
