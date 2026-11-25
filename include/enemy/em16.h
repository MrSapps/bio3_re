#ifndef OVL_EM16_H
#define OVL_EM16_H
/*
 * Enemy module em16 (zombie variants, enemy ids 0x16, 0x18-0x1F). Its code is a
 * near copy of em10 (RE2 EM10.C + EM10_S0.C): same routine tables at the same
 * .data offsets, mostly identical routines; Init (Em10_Init) differs.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep.h"
#include "joint.h"
#include "main.h"
#include "player.h"

#include "enemy/em10_free.h"

/* exe functions without a shared prototype (signatures as defined in src/game) */

extern USHORT Em10_life[16];
extern USHORT Em10_life_easy[16];
extern void (*Em10_routine_0[])();
extern void (*Em10_Move_type[])();
extern void (*Em10_mv_br_type00[])();
extern void (*Em10_mv_type00[])();
extern void (*Em10_mv_br_type01[])();
extern void (*Em10_mv_type01[])();
extern void (*Em10_dm[][3])();
extern void (*Em10_haidm[])();
extern void (*Em10_die[][3])();
extern void (*Em10_sce[])();
extern void (*Pldamage_Em10_tbl[])();
extern void (*Pldie_Em10_tbl[])();
extern USHORT Em10_walk_rnd[32]; /* walk time, 0x8000: turn slower */
extern USHORT Em10_mv0d_rnd[16]; /* time, 0x8000: turn the other way */
extern EM10_ESP Em10_mv11_esp[];
extern UCHAR Em10_dm00_usonum[]; /* Unk0B decrement per damage type */

void Em10_main(ENEMY_WORK *pEm);
void Em10_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm_sel00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm_sel01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce02(void);
void Em10_sce03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce04(void);
void Em10_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldamege_Em10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldie_Em10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldie00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Kubigoro(ENEMY_WORK *pEm);
void Em10_ude_down(void);
void Em10_mv00_br(ENEMY_WORK *pEm);
void Em10_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv01_br(ENEMY_WORK *pEm);
void Em10_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv02_br(ENEMY_WORK *pEm);
void Em10_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv03_br(void);
void Em10_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv04_br(ENEMY_WORK *pEm);
void Em10_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv05_br(void);
void Em10_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv06_br(void);
void Em10_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv07_br(void);
void Em10_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv08_br(void);
void Em10_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv09_br(ENEMY_WORK *pEm);
void Em10_mv09(void);
void Em10_mv0a_br(void);
void Em10_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv0b_br(void);
void Em10_mv0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv0c_br(void);
void Em10_mv0c(void);
void Em10_mv0d_br(ENEMY_WORK *pEm);
void Em10_mv0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv0e_br(ENEMY_WORK *pEm);
void Em10_mv0e(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv0f_br(void);
void Em10_mv0f(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv10_br(void);
void Em10_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv11_br(void);
void Em10_mv11(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Firedm(ENEMY_WORK *pEm);
void Em10_Ryudm(ENEMY_WORK *pEm);
void Em10_Tudm(ENEMY_WORK *pEm);
void Em10_Spdm(ENEMY_WORK *pEm);
void Em10_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm03(void);
void Em10_dm04(void);
void Em10_dm05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm_dummy(void);
void Em10_foot_set(ENEMY_WORK *pEm, ULONG *pKan, LONG No);
void Em10_blood_set(ENEMY_WORK *pEm);
void Em10_water(ENEMY_WORK *pEm);

#endif /* OVL_EM16_H */
