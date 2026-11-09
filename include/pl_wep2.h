#ifndef PL_WEP2_H
#define PL_WEP2_H
#include "pl_wep.h"

/* RE2 PL_WEP2.H: player weapon move routines (pl_mv05_*). */
extern PL_MV05_FN pl_mv05_w0_tbl[]; /* 0x8009D004 index Routine_2 */
extern PL_MV05_FN pl_mv05_w1_tbl[]; /* 0x8009D030 index Routine_2 */
extern PL_MV05_FN pl_mv05_w2_tbl[]; /* 0x8009D05C index Routine_2 */

ULONG func_80027940(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg); /* RE2 JOINT.C Joint_move */
void pl_mv05_w0_t9(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w0_t10(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void Pl_weapon_vib(ENEMY_WORK *pEm);  /* RE2 PL_WEP.C Pl_weapon_vib */
void pl_mv05_w0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
void pl_mv05_w2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);

#endif /* PL_WEP2_H */
