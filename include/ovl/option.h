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
    UCHAR Id;                          /* 0x01 shape (index into Config_piece_shape) */
    UCHAR Type;                        /* 0x02 animation script (index into Config_piece_anim) */
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
    signed char Routine;               /* 0x00 page: index into Config_padd (main) / Config_padd_disp (draw) */
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
extern OPT_WORK Opt;

extern OPT_FILE Config_file_tbl[4]; /* Option.dat pieces */
extern UCHAR Config_file_sum[4]; /* their check sums */
extern signed char Config_moji_space[]; /* glyph left/right spacing, 2 per glyph */
extern char Config_moji_tbl[][3]; /* glyph table: fullwidth chars in font order */
extern UCHAR Config_pad_icon[4]; /* button icon per pad type */
extern SHORT Config_pad_icon_pos[2]; /* pad type icon position */
/* background piece shape (Config_piece_shape) */
typedef struct tagOPT_SHAPE {
    UCHAR Size;                        /* 0x0 width = height, also the u step per animation frame */
    UCHAR pad1;
    UCHAR U;                           /* 0x2 texture position of frame 0 */
    UCHAR V;                           /* 0x3 */
} OPT_SHAPE;
extern OPT_SHAPE Config_piece_shape[9];
extern UCHAR *Config_piece_anim[3]; /* piece animation scripts by type */
extern void (*Config_padd[6])(void); /* page main routines by Opt.Routine */
extern void (*Config_padd_disp[6])(void); /* page draw routines by Opt.Routine */
extern ULONG Config_se_arrow_r; /* SE volume arrow right lit */
extern ULONG Config_se_arrow_l; /* SE volume arrow left lit */
extern ULONG Config_pad_arrow_r; /* pad type arrow right lit */
extern ULONG Config_pad_arrow_l; /* pad type arrow left lit */

void Config(void);
void Config_padd_opt(void);
void Config_opt_disp(void);
void Config_init(void);
void Config_prim_init(void);
void Config_exit(void);
void Config_bg_disp(void);
void Config_bg_load(void);
void Config_pad_tim_load(void);
void Config_padd_con(void);
void Config_con_disp(void);
void Config_pad_disp(void);
void Config_padd_vib(void);
void Config_vib_disp(void);
void Config_padd_snd(void);
void Config_snd_disp(void);
void Config_padd_clr(void);
void Config_clr_disp(void);
void Config_padd_res(void);
void Config_res_disp(void);
void Config_moji_disp(LONG X, LONG Y, char *pStr, LONG Dark, LONG Mode);
void Config_suuji(LONG X, LONG Y, ULONG Num);
LONG Config_moji_cmp(char *pA, char *pB);
void Config_arrow_disp(LONG X, LONG Y, ULONG Pad, ULONG On);
void Config_page_exit(void);
void Config_piece_move(void);
void Config_piece_disp(void);
void Config_piece_set(LONG Id, LONG Type, LONG X, LONG Y, SHORT Rot_y);

#endif /* OVL_OPTION_H */
