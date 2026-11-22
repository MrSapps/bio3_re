#ifndef OVL_EM37_H
#define OVL_EM37_H
/*
 * Enemy module em37 (enemy ids 0x37, 0x3B). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "joint.h"

/* em37's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM37_FREE {
    ENEMY_WORK *pParent;               /* 0x00 (0x1CC) the enemy (id 0x35 or 0x36) this part belongs to */
    UCHAR pad4[2];
    SHORT Size;                        /* 0x06 (0x1D2) target Size_y of part 1 */
} EM37_FREE;

#define EM37_W(p) ((EM37_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)

/* random idle motions, copied to the stack by the move routines */
typedef struct tagEM37_MOTION {
    UCHAR No[8];
} EM37_MOTION;

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em37_routine_0[])() __asm__("D_801019BC");
extern void (*Em37_move_type[])() __asm__("D_801019E4");
extern void (*Em37_mv[])() __asm__("D_80101A20");
extern void (*Em37_sce[])() __asm__("D_80101A50");

/* exe functions without a shared prototype */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, result used as int */
LONG Rnd(void) __asm__("func_800102E8");                                 /* RE2 Rnd (sld.s) */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);        /* ground height */
void func_80016600(ENEMY_WORK *pEm, LONG Kan, LONG Seq);                /* door.c */

LONG func_80101854(ENEMY_WORK *pEm);                                    /* splash effects */

#endif /* OVL_EM37_H */
