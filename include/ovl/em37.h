#ifndef OVL_EM37_H
#define OVL_EM37_H
/*
 * Enemy module em37 (enemy ids 0x37, 0x3B). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em37_routine_0[])() __asm__("D_801019BC");
extern void (*Em37_move_type[])() __asm__("D_801019E4");
extern void (*Em37_mv[])() __asm__("D_80101A20");
extern void (*Em37_sce[])() __asm__("D_80101A50");

/* exe functions without a shared prototype */
void func_80016600(ENEMY_WORK *pEm, LONG Kan, LONG Seq);                /* door.c */

#endif /* OVL_EM37_H */
