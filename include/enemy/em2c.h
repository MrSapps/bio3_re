#ifndef OVL_EM2C_H
#define OVL_EM2C_H
/*
 * Enemy module em2c (enemy id 0x2C), the successor of RE2's EM2C.C: an
 * invulnerable static body (most likely a corpse) whose missing parts and pose
 * depend on Type & 0xF; Type 0 has a dark red shadow that spreads like a pool.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "joint.h"

extern void (*Em2c_routine_0[])();

void Em2c_main(ENEMY_WORK *pEm);
void Em2c_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2c_Move(ENEMY_WORK *pEm);

#endif /* OVL_EM2C_H */
