#ifndef OVL_EM10_H
#define OVL_EM10_H
/*
 * Enemy module em10 (zombie family, enemy ids 0x10-0x15, 0x17, 0x2F), the
 * successor of RE2's EM10.C + EM10_S0.C. Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2); the routine tables are in the module's .data.
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
/* Joint_move (joint.h) seen with the kan/seq pointers and its result (frame-end flag) */

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
void func_80101260(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010139C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce04(void);
void Em10_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldamege_Em10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldie_Em10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldie00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Kahannsin_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Kubigoro(ENEMY_WORK *pEm);
void Em10_ude_down(ENEMY_WORK *pEm);
void Em10_mv00_br(ENEMY_WORK *pEm);
void Em10_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv01_br(ENEMY_WORK *pEm);
void Em10_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv02_br(ENEMY_WORK *pEm);
void Em10_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104FC0(void);
void Em10_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv04_br(ENEMY_WORK *pEm);
void Em10_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105C10(void);
void func_80105C18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105E64(void);
void Em10_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801063CC(void);
void func_801063D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106654(void);
void func_8010665C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106770(ENEMY_WORK *pEm);
void func_80106844(void);
void func_8010684C(void);
void Em10_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106B7C(void);
void Em10_mv0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106E90(void);
void func_80106E98(void);
void Em10_mv0d_br(ENEMY_WORK *pEm);
void Em10_mv0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv0e_br(ENEMY_WORK *pEm);
void Em10_mv0e(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801074E4(void);
void func_801074EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107594(void);
void Em10_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801076A0(void);
void Em10_mv11(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801082B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Firedm(ENEMY_WORK *pEm);
void func_801089A8(ENEMY_WORK *pEm);
void func_80108B00(ENEMY_WORK *pEm);
void func_80108CBC(ENEMY_WORK *pEm);
void Em10_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109758(void);
void func_80109760(void);
void func_80109768(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109E54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A1D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A3DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A730(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B278(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B6D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010BB08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010BD60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010C3E8(void);
void func_8010C3F0(ENEMY_WORK *pEm, LONG Mode, LONG Foot);
void func_8010C57C(ENEMY_WORK *pEm);
void Em10_water(ENEMY_WORK *pEm);

#endif /* OVL_EM10_H */
