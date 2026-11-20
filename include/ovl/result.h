#ifndef OVL_RESULT_H
#define OVL_RESULT_H
/*
 * RESULT.BIN overlay (loaded at 0x801C2000): result screens of the main game
 * and of "The Mercenaries" mini game (times, grade, money, shop).
 * Data/bss of the overlay stay in asm (asm/data/ovl/result/data.data.s).
 */
#include "bio3.h"
#include "cd.h"
#include "main.h"
#include "monitor.h"
#include "mojidisp.h"
#include "snd.h"

/* piece of the packed OMBG.TIM file (offset/size inside the disc file) */
typedef struct tagRESULT_BG {
    ULONG Ofs;                         /* 0x0 */
    ULONG Size;                        /* 0x4 */
} RESULT_BG;

/* SPRT with its colour+code word addressable as one (fades add to all three components) */
typedef union tagRESULT_SPRT {
    SPRT Prim;
    struct {
        ULONG Tag;
        ULONG Rgbc;                    /* r0 g0 b0 code */
    } W;
} RESULT_SPRT;

/* 2D sprite object: one SPRT + DR_MODE per display buffer (0x50 bytes) */
typedef struct tagRESULT_SPR {
    UCHAR Flg;                         /* 0x00 RSPR_* bits */
    UCHAR Timer;                       /* 0x01 blink timer */
    UCHAR Ofs_x;                       /* 0x02 offset from the parent's right edge */
    UCHAR Ofs_y;                       /* 0x03 offset from the parent's top */
    UCHAR Mode;                        /* 0x04 0 fade in, 1 fade out, 2/3 blink, 0xFF none */
    UCHAR pad5[3];
    SHORT X;                           /* 0x08 */
    SHORT Y;                           /* 0x0A */
    RESULT_SPRT Sprt[2];               /* 0x0C */
    DR_MODE Dm[2];                     /* 0x34 */
    struct tagRESULT_SPR *pParent;     /* 0x4C position is relative to this one */
} RESULT_SPR;

#define RSPR_ON     0x01
#define RSPR_BLINK  0x02
#define RSPR_PULSE  0x04
#define RSPR_RESET  0x08

/* sprite init data (0xE bytes) */
typedef struct tagRESULT_SPR_DATA {
    SHORT X;                           /* 0x0 */
    SHORT Y;                           /* 0x2 */
    SHORT W;                           /* 0x4 */
    SHORT H;                           /* 0x6 */
    UCHAR U;                           /* 0x8 */
    UCHAR V;                           /* 0x9 */
    UCHAR Semi;                        /* 0xA */
    UCHAR Abr;                         /* 0xB */
    UCHAR Ofs_x;                       /* 0xC */
    UCHAR Ofs_y;                       /* 0xD */
} RESULT_SPR_DATA;

/* screen position for a printed value */
typedef struct tagRESULT_POS {
    SHORT X;
    SHORT Y;
} RESULT_POS;

/* result screen work (D_801C76C0) */
typedef struct tagRESULT_WORK {
    UCHAR Routine_0;                   /* 0x00 */
    UCHAR Routine_1;                   /* 0x01 */
    UCHAR Routine_2;                   /* 0x02 */
    UCHAR pad3;
    UCHAR Grade;                       /* 0x04 */
    UCHAR pad5;
    SHORT Timer;                       /* 0x06 */
    ULONG *pBuf;                       /* 0x08 */
    UCHAR Saves;                       /* 0x0C number of saves (main game result) */
    UCHAR padD;
    USHORT Score;                      /* 0x0E */
    USHORT Flg;                        /* 0x10 RWK_* bits: what is displayed */
    SHORT Count;                       /* 0x12 money left to count up/down */
    UCHAR pad14[0x16 - 0x14];
    signed char Cursor;                /* 0x16 */
    UCHAR Vloop;                       /* 0x17 saved Main.Vloop */
    SHORT Bonus;                       /* 0x18 */
    UCHAR pad1A[0x1C - 0x1A];
} RESULT_WORK;

/* G fields with other types than in tagGLOBAL, and past its end (The Mercenaries records) */
typedef struct tagGLOBAL_RESULT {
    UCHAR pad0[0x20FC];
    ULONG Key_rep;                     /* 0x20FC key repeat (tagGLOBAL.Unk20FC is USHORT) */
    UCHAR pad2100[0x2108 - 0x2100];
    ULONG Key_trg;                     /* 0x2108 same as tagGLOBAL.Key_trg (here for one base register) */
    UCHAR pad210C[0x211A - 0x210C];
    USHORT Score;                      /* 0x211A Mercenaries score (tagGLOBAL.Unk211A is SHORT) */
    UCHAR pad211C[0x24D6 - 0x211C];
    UCHAR Pl_id;                       /* 0x24D6 same as tagGLOBAL.Pl_id */
    UCHAR pad24D7[0x77F8 - 0x24D7];
    ULONG Unk77F8[3];                  /* 0x77F8 same as tagGLOBAL.Unk77F8 */
    UCHAR pad7804[0x7848 - 0x7804];
    SHORT Unk7848;                     /* 0x7848 same as tagGLOBAL.Unk7848 (message answer) */
    UCHAR pad784A[0x7860 - 0x784A];
    USHORT Time;                       /* 0x7860 Mercenaries time left (tagGLOBAL.Unk7860 is SHORT) */
    UCHAR pad7862[0x7868 - 0x7862];
    ULONG Unk7868[8];                  /* 0x7868 same as tagGLOBAL.Unk7868 */
    UCHAR pad7888[0x7E94 - 0x7888];
    ULONG Unk7E94;                     /* 0x7E94 */
    LONG Merc_time[3];                 /* 0x7E98 best time per character, -1 none */
    UCHAR Merc_grade[3];               /* 0x7EA4 best grade per character, 0xFF none */
    UCHAR pad7EA7;
    ULONG Merc_money[3];               /* 0x7EA8 best money per character */
    ULONG Money;                       /* 0x7EB4 money for the shop */
} GLOBAL_RESULT;
extern GLOBAL_RESULT Gr __asm__("G");

/* exe functions not in a shared header yet (game .c files declare some with other types) */
ULONG Cd_read(LONG Fno, ULONG *pAdr, LONG Mode, char *pName);
LONG Cd_read_s(LONG Fno, LONG a1, ULONG *pAdr, LONG a3, char *pName);
ULONG Flg_ck(ULONG *pFlg, ULONG Bit);
void Flg_on(ULONG *pFlg, ULONG Bit);
void Mapping_tim(ULONG *pTim);
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);
LONG Mess_set(ULONG Pos_xy, USHORT Attr, ULONG Mess_no, ULONG Stop_data);
/* Print14 with signed coordinates (the overlay passes them sign-extended) */
void Print14_s(LONG X, LONG Y, char *pFmt, ...) __asm__("Print14");
void func_8007809C(LONG a0, LONG a1, ULONG a2, ULONG a3);
void func_800783BC(ULONG no, ULONG size, ULONG a2);
void func_800782F4(ULONG no, ULONG id, ULONG *pAdr);

#endif /* OVL_RESULT_H */
