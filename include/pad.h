#ifndef PAD_H
#define PAD_H
#include "bio3.h"

/* key repeat / stop state (D_800DE628) */
typedef struct tagPAD_WORK {
    ULONG Stop_bak;                    /* 0x00 G.Stop_flg saved while the game is stopped (RE2 static Stp_bak) */
    union {
        SHORT Both;                    /* set with one sh by Pad_rep_set */
        UCHAR Time[2];                 /* [0] first repeat delay, [1] repeat interval */
    } Rep;                             /* 0x04 */
    UCHAR Num;                         /* 0x06 */
    UCHAR pad7;
    UCHAR Tbl[8][2];                   /* 0x08 {Padd bit no, countdown} */
} PAD_WORK;

/* 10-byte vibration request record (RE2 VIB_REQ, same layout in RE3) */
typedef struct VIB_REQ {
    UCHAR Flg;                         /* 0x0 active flag */
    UCHAR Pow;                         /* 0x1 motor power */
    USHORT Delay;                      /* 0x2 start delay countdown */
    USHORT Time;                       /* 0x4 remaining time countdown */
    SHORT Step;                        /* 0x6 power change per frame (fade) */
    USHORT Pow_cur;                    /* 0x8 power accumulator (power << 7) */
} VIB_REQ;

extern VIB_REQ Vib_req0[33];           /* 0x800DE648 small motor queue */
extern VIB_REQ Vib_req1[33];           /* 0x800DE798 large motor queue */
extern VIB_REQ *Vib_cur0;              /* 0x8009CCFC */
extern VIB_REQ *Vib_cur1;              /* 0x8009CD00 */
extern UCHAR PadActData[2];            /* 0x800C79C8 */
extern UCHAR Pl_vib_tbl[][6];          /* 0x8009CF90 RE2 PL_WEP.C Pl_vib_tbl, row = weapon - 2 */

void Pad_vib_set0(LONG Time, USHORT Attr);
void Pad_vib_set1(LONG Time, ULONG Power, ULONG Attr);
void Pad_vib_fade_set(LONG Time, LONG Pow_st, LONG Pow_ed, ULONG Attr);
void Pad_vib_init(void);
VIB_REQ *Pad_vib_get_free(VIB_REQ *pReq);
LONG Pad_vib_count(VIB_REQ *pReq);
UCHAR SetPadActData(VIB_REQ *pReq);
void Pad_rep_set(ULONG Flg, SHORT Val); /* pad key repeat set */

extern UCHAR Pad_act_align[]; /* 0x80098928 */
extern USHORT Key_type_demo[16]; /* 0x8009CC7C used while a demo plays back */
extern LONG D_800C79C0; /* status screen state, cleared before the extra game */
extern PAD_WORK Pad_work; /* 0x800DE628 */
extern UCHAR Pad_type_old; /* 0x800DE640 */
extern UCHAR Pad_act_set; /* 0x800DE641 RE2 UnknownPadSetFlag: 1 PadSetAct done, 2 aligned */

void Pad_set(void);
void func_8003884C(USHORT *pTbl, ULONG No); /* pad: play vibration script No of pTbl */

#endif /* PAD_H */
