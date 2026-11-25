#ifndef OVL_EM40_H
#define OVL_EM40_H
/*
 * Enemy module em40 (enemy id 0x40): an invulnerable, unanimated character
 * model that only turns its head (part 3) towards the player. Two routines
 * (init, empty move). Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "joint.h"

extern void (*Em40_routine_0[])();

void Em40_main(ENEMY_WORK *pEm);
void Em40_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em40_Move(ENEMY_WORK *pEm);

#endif /* OVL_EM40_H */
