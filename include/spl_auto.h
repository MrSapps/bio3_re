#ifndef SPL_AUTO_H
#define SPL_AUTO_H
/* RE2 SPL_AUTO.C: sub player AI (Spl_Auto, Aida_*), damage and death */
#include "bio3.h"
#include "em_sub.h"

typedef void (*EM_FUNC)(ENEMY_WORK *pEm);

typedef void (*EM_FUNC3)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

extern UCHAR D_8009DD34[];
extern UCHAR D_8009E70C[];
extern UCHAR D_8009E70D[];
extern UCHAR D_8009E710[];
extern void (*D_8009E720[])(ENEMY_WORK *pEm);
extern SHORT D_8009E770[6];
extern void (*D_8009E788[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern UCHAR D_8009E7C8[];
extern UCHAR D_8009E7D8[];
extern EM_FUNC3 D_8009E7DC[];
extern EM_FUNC3 D_8009E82C[];
extern EM_FUNC3 D_8009E87C[];
extern EM_FUNC3 D_8009E8CC[];
extern EM_FUNC3 D_8009E91C[];
extern EM_FUNC3 D_8009E96C[];
extern EM_FUNC3 D_8009E9BC[];
extern EM_FUNC3 D_8009EA0C[];
extern EM_FUNC3 D_8009EA5C[];
extern EM_FUNC3 D_8009EAAC[];
extern EM_FUNC3 D_8009EAFC[];
extern EM_FUNC3 D_8009EB4C[];
extern EM_FUNC3 D_8009EB9C[];
extern EM_FUNC3 D_8009EBEC[];
extern UCHAR D_8009EC3C[];
extern void (*D_8009EC9C[])(ENEMY_WORK *pEm, ULONG Type);
extern void (*D_8009ECAC[])(ENEMY_WORK *pEm, ULONG Type);
extern void (*D_8009ECCC[])(ENEMY_WORK *pEm, ULONG Type);
extern EM_FUNC D_8009ECDC[];
extern EM_FUNC D_8009ECF8[];
extern void (*D_8009ED14[])(ENEMY_WORK *pEm, ULONG Type);
extern EM_FUNC3 D_8009ED24[];
extern EM_FUNC3 D_8009ED84[];
extern SHORT Em_dir[3] __asm__("D_800E01A0"); /* RE2 Em_dir */
extern ENEMY_WORK *D_800E01A8; /* D_800E01A8 seen as an array (Spl_em_pos_ck) */
extern ENEMY_WORK *D_800E01AC;
extern USHORT D_800E01B4; /* RE2 Em_l */
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
void func_8005A16C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8005A1F0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Aida_em10_at00_pl(ENEMY_WORK *pEm, ULONG Type);
void Aida_em10_at00_spl(ENEMY_WORK *pEm);
void Aida_em25_at00_pl(ENEMY_WORK *pEm, ULONG Type);
void Aida_em25_at00_spl(ENEMY_WORK *pEm);
void func_8005A5AC(void);
void func_8005A5B4(void);
void func_8005A5BC(void);
void func_8005A5C4(void);
void Aida_mv00_br(ENEMY_WORK *pEm);
void Aida_mv00_br_1(ENEMY_WORK *pEm);
void func_8005A948(ENEMY_WORK *pEm);
void func_8005AB14(ENEMY_WORK *pEm);
void Aida_mv00(ENEMY_WORK *pEm);
void Aida_em10_at01(ENEMY_WORK *pEm);
void Aida_em25_at01(ENEMY_WORK *pEm);
void func_8005ADBC(void);
void func_8005ADC4(void);
void Aida_mv01_br(ENEMY_WORK *pEm);
void Aida_mv01_br_1(ENEMY_WORK *pEm);
void func_8005B2CC(ENEMY_WORK *pEm);
void Aida_mv01(ENEMY_WORK *pEm);
void Aida_mv02_br(ENEMY_WORK *pEm);
void Aida_mv02_br_1(ENEMY_WORK *pEm);
void func_8005B9D0(ENEMY_WORK *pEm);
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
void func_8005C210(ENEMY_WORK *pEm);
void func_8005C2F8(ENEMY_WORK *pEm);
void Aida_mv05_w1_t1_br(ENEMY_WORK *pEm);
void Aida_mv05_w1_t1(ENEMY_WORK *pEm);
void func_8005C730(void);
void func_8005C738(ENEMY_WORK *pEm);
void func_8005C81C(void);
void func_8005C824(ENEMY_WORK *pEm);
void func_8005C8D0(void);
void func_8005C8D8(ENEMY_WORK *pEm);
void func_8005C96C(void);
void func_8005C974(ENEMY_WORK *pEm);
void func_8005CA0C(void);
void func_8005CA14(ENEMY_WORK *pEm);
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
void func_8005D208(ENEMY_WORK *pEm);
void func_8005D290(void);
void Aida_mv0b_br_1(ENEMY_WORK *pEm);
void Aida_mv0b(ENEMY_WORK *pEm);
void Aida_mv0c_br(ENEMY_WORK *pEm);
void func_8005D4D8(ENEMY_WORK *pEm);
void func_8005D554(ENEMY_WORK *pEm);
void func_8005D704(void);
void func_8005D70C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8005DA44(ENEMY_WORK *pEm);
void func_8005DBCC(ENEMY_WORK *pEm);
void func_8005DE3C(ENEMY_WORK *pEm);
void func_8005DF4C(ENEMY_WORK *pEm);
void func_8005E1D0(ENEMY_WORK *pEm);
void func_8005E40C(ENEMY_WORK *pEm);
void Spl_Damage(ENEMY_WORK *pEm);
void Spl_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8005E90C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8005EBC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_Die(ENEMY_WORK *pEm);
void Spl_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_Dead(ENEMY_WORK *pEm);
LONG Spl_soba_pos_set(LONG Len, LONG Dir1, LONG Dir2, SVECTOR *pOut);
void Spl_enemy_ck(ENEMY_WORK *pEm);
ULONG Spl_em_pos_ck(ENEMY_WORK *pEm, ULONG Lim);
ENEMY_WORK *Spl_em_cdir_ck(ENEMY_WORK *pEm);
ENEMY_WORK *Spl_pllock_get(ENEMY_WORK *pEm, ENEMY_WORK *pEx);
LONG func_8005F984(LONG Dmg, ULONG Flg); /* sub player life down */
LONG Spl_life_down(ENEMY_WORK *pEm, LONG Dmg, ULONG Flg);
void func_8005FAEC(ENEMY_WORK *pEm);
void func_8005FC18(ENEMY_WORK *pEm);
void func_8005FEE0(ENEMY_WORK *pEm);

#endif /* SPL_AUTO_H */
