#ifndef PAD_H
#define PAD_H
#include "bio3.h"

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

#endif /* PAD_H */
