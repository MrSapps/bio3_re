#ifndef OVL_EM16_H
#define OVL_EM16_H
/*
 * Enemy module em16 (zombie variants, enemy ids 0x16, 0x18-0x1F). Its code is a
 * near copy of em10 (RE2 EM10.C + EM10_S0.C): same routine tables at the same
 * .data offsets, mostly identical routines; Init (Em10_Init) differs.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep.h"
#include "joint.h"
#include "main.h"
#include "player.h"

#include "ovl/em10_free.h"

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
void func_80101BBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101C38(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010208C(void);
void Em10_sce03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_sce04(void);
void Em10_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldamege_Em10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldie_Em10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_pldie00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Kubigoro(ENEMY_WORK *pEm);
void func_80103CE4(void);
void Em10_mv00_br(ENEMY_WORK *pEm);
void Em10_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv01_br(ENEMY_WORK *pEm);
void Em10_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv02_br(ENEMY_WORK *pEm);
void Em10_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104A68(void);
void Em10_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv04_br(ENEMY_WORK *pEm);
void Em10_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801056B8(void);
void func_801056C0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010590C(void);
void Em10_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105E74(void);
void func_80105E7C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801060FC(void);
void func_80106104(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106218(ENEMY_WORK *pEm);
void func_801062EC(void);
void func_801062F4(void);
void Em10_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106624(void);
void Em10_mv0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106938(void);
void func_80106940(void);
void Em10_mv0d_br(ENEMY_WORK *pEm);
void Em10_mv0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_mv0e_br(ENEMY_WORK *pEm);
void Em10_mv0e(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106F8C(void);
void func_80106F94(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010703C(void);
void Em10_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107148(void);
void Em10_mv11(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107D58(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_Firedm(ENEMY_WORK *pEm);
void func_80108450(ENEMY_WORK *pEm);
void func_801085A8(ENEMY_WORK *pEm);
void func_80108764(ENEMY_WORK *pEm);
void Em10_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109200(void);
void func_80109208(void);
void func_80109210(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_dm06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801098FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109C78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109E84(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A1D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em10_die02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010AD20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B17C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B5B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B808(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010BE90(void);
void func_8010BE98(ENEMY_WORK *pEm, ULONG *pKan, LONG No);
void func_8010C024(ENEMY_WORK *pEm);
void Em10_water(ENEMY_WORK *pEm);

#endif /* OVL_EM16_H */
