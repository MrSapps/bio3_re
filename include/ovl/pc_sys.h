#ifndef OVL_PC_SYS_H
#define OVL_PC_SYS_H
/*
 * PC_SYS.BIN overlay (loaded at 0x801C2000): the in-game computer terminals.
 * A window zooms open, text is typed out on a 30 x 12 character screen, and a
 * 10 x 3 on-screen keyboard reads passwords. The AOT (G.pAot_dat) word at +2
 * picks the terminal: 0 the R.P.D. STARS notice (safe password), 1 the hospital
 * "Umbrella Security System" (medical storage room lock).
 */
#include "bio3.h"
#include "em_sub.h"
#include "main.h"
#include "monitor.h"
#include "om.h"
#include "pl_wep.h"
#include "sub.h"

#define PC_COL_NUM 30
#define PC_ROW_NUM 12
#define PC_SPR_NUM 0x16C               /* 4 + PC_COL_NUM * PC_ROW_NUM */

/* sprite (0x10 bytes): the window box, the key cursor, the text cursor, then one
 * per character; also the format of the definition table Pc_spr_def */
typedef union tagPC_SPR {
    struct {
        SHORT X;                       /* 0x00 */
        SHORT Y;                       /* 0x02 */
        SHORT W;                       /* 0x04 */
        SHORT H;                       /* 0x06 */
        UCHAR U;                       /* 0x08 */
        UCHAR V;                       /* 0x09 */
        USHORT Clut;                   /* 0x0A clut row (definition: added to 0x7B40) */
        UCHAR Ot;                      /* 0x0C G.Ot0 slot */
        UCHAR Code;                    /* 0x0D added to the SPRT code (2: semi-transparent) */
        UCHAR Col;                     /* 0x0E grey level */
        UCHAR On;                      /* 0x0F */
    } s;
    struct {
        ULONG Xy;                      /* 0x00 */
        ULONG Wh;                      /* 0x04 */
        ULONG Uv_clut;                 /* 0x08 */
        ULONG Attr;                    /* 0x0C */
    } w;
} PC_SPR;

/* SPRT packet written word by word */
typedef struct tagPC_SPRT {
    ULONG Tag;                         /* 0x00 */
    ULONG Rgbc;                        /* 0x04 */
    ULONG Xy;                          /* 0x08 */
    ULONG Uv_clut;                     /* 0x0C */
    ULONG Wh;                          /* 0x10 */
} PC_SPRT;

/* LINE_F2 with the colour as one word */
typedef struct tagPC_LINE {
    ULONG Tag;                         /* 0x00 */
    ULONG Rgbc;                        /* 0x04 */
    SHORT X0;                          /* 0x08 */
    SHORT Y0;                          /* 0x0A */
    SHORT X1;                          /* 0x0C */
    SHORT Y1;                          /* 0x0E */
} PC_LINE;

/* overlay work (0x53E0 bytes, allocated from G.pEm_mem) */
typedef struct tagPC_WORK {
    UCHAR Col;                         /* 0x000 text cursor column */
    UCHAR Row;                         /* 0x001 text cursor row */
    signed char Key_x;                 /* 0x002 keyboard cursor 0..9 */
    signed char Key_y;                 /* 0x003 keyboard cursor 0..2 */
    UCHAR Text[PC_ROW_NUM][PC_COL_NUM];  /* 0x004 */
    UCHAR Attr[PC_ROW_NUM][PC_COL_NUM];  /* 0x16C colour of each character */
    UCHAR pad2D4;
    UCHAR Blink;                       /* 0x2D5 text cursor blink counter */
    UCHAR Wait;                        /* 0x2D6 typing delay counter */
    UCHAR Color;                       /* 0x2D7 current colour ('|n') */
    UCHAR Input_num;                   /* 0x2D8 characters typed */
    UCHAR Key;                         /* 0x2D9 key pressed this frame: 1 cancel, 2..29 keys */
    UCHAR Kido;                        /* 0x2DA key cursor brightness */
    signed char Kido_add;              /* 0x2DB */
    UCHAR Zoom;                        /* 0x2DC window zoom counter 0..12 */
    UCHAR Cut_bak;                     /* 0x2DD G.Next_cut_no on entry */
    UCHAR Push;                        /* 0x2DE key cursor pressed (drawn 1 pixel lower) */
    UCHAR pad2DF[0x2E4 - 0x2DF];
    UCHAR Input[0x14];                 /* 0x2E4 typed password */
    UCHAR *pStr;                       /* 0x2F8 string being printed */
    UCHAR *pCur;                       /* 0x2FC read position in pStr */
    PC_SPR Spr[PC_SPR_NUM];            /* 0x300 */
    DR_MODE Dm[2][8];                  /* 0x19C0 */
    PC_SPRT Sprt[2][PC_SPR_NUM];       /* 0x1A80 */
    PC_LINE Line[2][4];                /* 0x5360 window zoom frame */
} PC_WORK;

#define PC_SPR_BOX  0
#define PC_SPR_KEY  1
#define PC_SPR_CUR  2
#define PC_SPR_TEXT 4

/* overlay .data (still in asm) */
extern PC_SPR Pc_spr_def[PC_SPR_NUM];
extern UCHAR Pc_voice_data[]; /* byte streams per message, purpose unknown (only stored in Pc0/Pc1_voice) */
extern USHORT Pc_voice_ofs[20]; /* offsets into Pc_voice_data */
extern LONG Pc0_skip; /* terminal 0: page key pressed */
extern LONG Pc0_done; /* terminal 0: current string printed */
extern UCHAR Pc0_mode; /* 0: page with button 0x1000, 1: any button */
extern LONG Pc1_skip;
extern LONG Pc1_done;
extern UCHAR Pc1_mode;
extern PC_WORK *Pc_wk;
extern ULONG Pc0_pass_no; /* which safe password is shown (0..3) */
extern UCHAR *Pc0_voice2;
extern UCHAR *Pc0_voice;
extern ULONG Pc1_pass_no; /* which password is accepted (0..2) */
extern UCHAR *Pc1_voice2;
extern UCHAR *Pc1_voice;

void Pc_main(void);
ULONG Pc_window_ctrl(void);
ULONG Pc_zoom(ULONG X, ULONG Y, ULONG W, ULONG H);
void Pc_zoom_frame(LONG X0, LONG Y0, LONG X1, LONG Y1);
ULONG Pc_key_char(UCHAR Key);
ULONG Pc_input(ULONG Max, ULONG Mode);
void Pc_box_set(UCHAR On, SHORT W, SHORT H, UCHAR Col);
ULONG Pc_key_move(void);
void Pc_spr_init(PC_SPR *pDst, PC_SPR *pSrc);
void Pc_spr_trans(ULONG Num);
void Pc_dr_mode_trans(void);
void Pc_put_char(UCHAR Code);
void Pc_scroll(void);
void Pc_clear(void);
void Pc_text_set(LONG On);
ULONG Pc_print(LONG Wait, UCHAR *pStr, LONG *pDone);
void Pc_notice(void);
void Pc_security(void);
void Pc_open(ULONG Cut, LONG Zoom);
void Pc_close(LONG Zoom);

#endif /* OVL_PC_SYS_H */
