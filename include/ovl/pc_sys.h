#ifndef OVL_PC_SYS_H
#define OVL_PC_SYS_H
/*
 * PC_SYS.BIN overlay (loaded at 0x801C2000): the in-game computer terminals.
 * A window zooms open, text is typed out on a 30 x 12 character screen, and a
 * 10 x 3 on-screen keyboard reads passwords. The AOT (G.Unk21DC) word at +2
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
extern PC_SPR Pc_spr_def[PC_SPR_NUM] __asm__("D_801C46E4");
extern UCHAR Pc_voice_data[] __asm__("D_801C5DA4");  /* byte streams per message, purpose unknown (only stored in Pc0/Pc1_voice) */
extern USHORT Pc_voice_ofs[20] __asm__("D_801C5F78");  /* offsets into Pc_voice_data */
extern LONG Pc0_skip __asm__("D_801C5FA0");          /* terminal 0: page key pressed */
extern LONG Pc0_done __asm__("D_801C5FA4");          /* terminal 0: current string printed */
extern UCHAR Pc0_mode __asm__("D_801C5FA8");         /* 0: page with button 0x1000, 1: any button */
extern LONG Pc1_skip __asm__("D_801C5FAC");
extern LONG Pc1_done __asm__("D_801C5FB0");
extern UCHAR Pc1_mode __asm__("D_801C5FB4");
extern PC_WORK *Pc_wk __asm__("D_801C5FB8");
extern ULONG Pc0_pass_no __asm__("D_801C5FC0");      /* which safe password is shown (0..3) */
extern UCHAR *Pc0_voice2 __asm__("D_801C5FC4");
extern UCHAR *Pc0_voice __asm__("D_801C5FC8");
extern ULONG Pc1_pass_no __asm__("D_801C5FD0");      /* which password is accepted (0..2) */
extern UCHAR *Pc1_voice2 __asm__("D_801C5FD4");
extern UCHAR *Pc1_voice __asm__("D_801C5FD8");

/* exe functions without a shared prototype */
void func_80038634(ULONG Flg, SHORT Val);           /* pad key repeat set */
void Flg_off(ULONG *pFlg, ULONG Id);
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSv2); /* RE2 Esp_call */
void Om_move(void);
void Esp_task_main(void);
void Eff_sprt_trans(void);

void func_801C2354(void);
ULONG func_801C23D0(void);
ULONG func_801C24E0(ULONG X, ULONG Y, ULONG W, ULONG H);
void func_801C2608(LONG X0, LONG Y0, LONG X1, LONG Y1);
ULONG func_801C270C(UCHAR Key);
ULONG func_801C2718(ULONG Max, ULONG Mode);
void func_801C2998(UCHAR On, SHORT W, SHORT H, UCHAR Col);
ULONG func_801C29F0(void);
void func_801C2D34(PC_SPR *pDst, PC_SPR *pSrc);
void func_801C2D94(ULONG Num);
void func_801C2EBC(void);
void func_801C2F80(UCHAR Code);
void func_801C2FB4(void);
void func_801C3074(void);
void func_801C30B8(LONG On);
ULONG func_801C31A4(LONG Wait, UCHAR *pStr, LONG *pDone);
void func_801C347C(void);
void func_801C3A0C(void);
void func_801C44F4(ULONG Cut, LONG Zoom);
void func_801C4624(LONG Zoom);

#endif /* OVL_PC_SYS_H */
