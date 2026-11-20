#ifndef OVL_EM27_H
#define OVL_EM27_H
/*
 * Enemy module em27 (enemy id 0x27). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"

/* em27's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM27_FREE {
    UCHAR Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk01;                       /* 0x01 (0x1CD) */
    SHORT Wait;                        /* 0x02 (0x1CE) frames before the next move */
} EM27_FREE;

#define EM27_W(p) ((EM27_FREE *)(p)->Unk1CC)
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em27_routine_0[])() __asm__("D_80101A34");

/* exe functions without a shared prototype */
LONG Rnd(void) __asm__("func_800102E8");                                 /* RE2 Rnd (sld.s) */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, result used as int */
LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);        /* ground height under pV */

#endif /* OVL_EM27_H */
