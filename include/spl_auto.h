#ifndef SPL_AUTO_H
#define SPL_AUTO_H
/* RE2 SPL_AUTO.C: sub player AI (Spl_Auto, Aida_*), damage and death */
#include "bio3.h"
#include "em_sub.h"

typedef void (*EM_FUNC)(ENEMY_WORK *pEm);

typedef void (*EM_FUNC3)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

extern UCHAR D_8009DD34[];
extern UCHAR Spl_life_u[];
extern UCHAR D_8009E70D[];
extern UCHAR D_8009E710[];
extern void (*Spl_shot_eff_tbl[])(ENEMY_WORK *pEm);
extern SHORT Spl_soba_ofs[6];
extern void (*Aida_Move_type[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern UCHAR D_8009E7C8[];
extern UCHAR Spl_ai_wait[];
extern EM_FUNC3 Aida_mv_br_type00[];
extern EM_FUNC3 Aida_mv_type00[];
extern EM_FUNC3 Aida_mv_br_type01[];
extern EM_FUNC3 Aida_mv_br_type02[];
extern EM_FUNC3 Aida_mv_br_type03[];
extern EM_FUNC3 Aida_mv_br_type04[];
extern EM_FUNC3 Aida_mv_br_type05[];
extern EM_FUNC3 Aida_mv_br_type06[];
extern EM_FUNC3 Aida_mv_br_type07[];
extern EM_FUNC3 Aida_mv_br_type08[];
extern EM_FUNC3 Aida_mv_br_type09[];
extern EM_FUNC3 Aida_mv_br_type0a[];
extern EM_FUNC3 Aida_mv_br_type0b[];
extern EM_FUNC3 Aida_mv_br_type0c[];
extern UCHAR Aida_em_at_idx[];
extern void (*Aida_em_at00_pl[])(ENEMY_WORK *pEm, ULONG Type);
extern void (*Aida_em_at00_spl[])(ENEMY_WORK *pEm, ULONG Type);
extern void (*Aida_em_at01[])(ENEMY_WORK *pEm, ULONG Type);
extern EM_FUNC Aida_mv05_w1_br_tbl[];
extern EM_FUNC Aida_mv05_w1_tbl[];
extern void (*Aida_em_at05[])(ENEMY_WORK *pEm, ULONG Type);
extern EM_FUNC3 Spl_dm[];
extern EM_FUNC3 Spl_die[];
extern SHORT Em_dir[3]; /* RE2 Em_dir: direction to the three nearest enemies */
extern ENEMY_WORK *pSpl_near_em[3];    /* the three nearest enemies (Spl_enemy_ck), RE2 SPL.H */
extern ENEMY_WORK *D_800E01AC;
extern USHORT Em_l[3]; /* RE2 Em_l: distance to the three nearest enemies */
extern USHORT D_800E01B6;

void Spl_Auto(UCHAR *p);
void Spl_Root_ck_sys(ENEMY_WORK *pEm);
void Aida_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_Move_type0f(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_em10_at00_pl(ENEMY_WORK *pEm, ULONG Type);
void Aida_em10_at00_spl(ENEMY_WORK *pEm);
void Aida_em25_at00_pl(ENEMY_WORK *pEm, ULONG Type);
void Aida_em25_at00_spl(ENEMY_WORK *pEm);
void Aida_em_at00_pl_2(void);
void Aida_em_at00_spl_2(void);
void Aida_em_at00_pl_3(void);
void Aida_em_at00_spl_3(void);
void Aida_mv00_br(ENEMY_WORK *pEm);
void Aida_mv00_br_1(ENEMY_WORK *pEm);
void Aida_mv00_br_t0b(ENEMY_WORK *pEm);
void Aida_mv00_br_2(ENEMY_WORK *pEm);
void Aida_mv00(ENEMY_WORK *pEm);
void Aida_em10_at01(ENEMY_WORK *pEm);
void Aida_em25_at01(ENEMY_WORK *pEm);
void Aida_em_at01_2(void);
void Aida_em_at01_3(void);
void Aida_mv01_br(ENEMY_WORK *pEm);
void Aida_mv01_br_1(ENEMY_WORK *pEm);
void Aida_mv01_br_t0b(ENEMY_WORK *pEm);
void Aida_mv01(ENEMY_WORK *pEm);
void Aida_mv02_br(ENEMY_WORK *pEm);
void Aida_mv02_br_1(ENEMY_WORK *pEm);
void Aida_mv02_br_t0b(ENEMY_WORK *pEm);
void Aida_mv02(ENEMY_WORK *pEm);
void Aida_mv03_br(ENEMY_WORK *pEm);
void Aida_mv03(ENEMY_WORK *pEm);
void Aida_mv04_br(ENEMY_WORK *pEm);
void Aida_mv04(ENEMY_WORK *pEm);
void Aida_mv05_br(ENEMY_WORK *pEm);
void Aida_mv05(ENEMY_WORK *pEm);
void Aida_mv05_w1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_mv05_w1_t0_br(void);
void Aida_mv05_w1_t0(ENEMY_WORK *pEm);
void Aida_em10_at05(ENEMY_WORK *pEm);
void Aida_em_at05_3(ENEMY_WORK *pEm);
void Aida_mv05_w1_t1_br(ENEMY_WORK *pEm);
void Aida_mv05_w1_t1(ENEMY_WORK *pEm);
void Aida_mv05_w1_t2_br(void);
void Aida_mv05_w1_t2(ENEMY_WORK *pEm);
void Aida_mv05_w1_t3_br(void);
void Aida_mv05_w1_t3(ENEMY_WORK *pEm);
void Aida_mv05_w1_t4_br(void);
void Aida_mv05_w1_t4(ENEMY_WORK *pEm);
void Aida_mv05_w1_t5_br(void);
void Aida_mv05_w1_t5(ENEMY_WORK *pEm);
void Aida_mv05_w1_t6_br(void);
void Aida_mv05_w1_t6(ENEMY_WORK *pEm);
void Aida_mv06_br(void);
void Aida_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_mv07_br(void);
void Aida_mv07(void);
void Aida_mv08_br(void);
void Aida_mv08(ENEMY_WORK *pEm);
void Aida_mv09_br(void);
void Aida_mv09(ENEMY_WORK *pEm);
void Aida_mv0a_br(void);
void Aida_mv_nop_br(void);
void Aida_mv0a(ENEMY_WORK *pEm);
void Aida_mv0b_br(ENEMY_WORK *pEm);
void Aida_mv0b_br_t0b(ENEMY_WORK *pEm);
void func_8005D290(void);
void Aida_mv0b_br_1(ENEMY_WORK *pEm);
void Aida_mv0b(ENEMY_WORK *pEm);
void Aida_mv0c_br(ENEMY_WORK *pEm);
void Aida_mv0e_br(ENEMY_WORK *pEm);
void Aida_mv0f_br(ENEMY_WORK *pEm);
void Aida_mv10_br(void);
void Aida_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_mv11_br(ENEMY_WORK *pEm);
void Aida_mv11(ENEMY_WORK *pEm);
void Aida_mv12_br(ENEMY_WORK *pEm);
void Aida_mv12(ENEMY_WORK *pEm);
void Aida_mv13_br(ENEMY_WORK *pEm);
void Aida_mv13(ENEMY_WORK *pEm);
void Spl_Damage(ENEMY_WORK *pEm);
void Spl_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_dm03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_Die(ENEMY_WORK *pEm);
void Spl_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_Dead(ENEMY_WORK *pEm);
LONG Spl_soba_pos_set(LONG Len, LONG Dir1, LONG Dir2, SVECTOR *pOut);
void Spl_enemy_ck(ENEMY_WORK *pEm);
ULONG Spl_em_pos_ck(ENEMY_WORK *pEm, ULONG Lim);
ENEMY_WORK *Spl_em_cdir_ck(ENEMY_WORK *pEm);
ENEMY_WORK *Spl_pllock_get(ENEMY_WORK *pEm, ENEMY_WORK *pEx);
LONG Spl_life_sub(LONG Dmg, ULONG Flg); /* sub player life down */
LONG Spl_life_down(ENEMY_WORK *pEm, LONG Dmg, ULONG Flg);
void Spl_shot_eff(ENEMY_WORK *pEm);
void Spl_shot_eff_rapid(ENEMY_WORK *pEm);
void Spl_shot_eff_auto(ENEMY_WORK *pEm);

#endif /* SPL_AUTO_H */
