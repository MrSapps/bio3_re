#ifndef SPL_SCE_H
#define SPL_SCE_H
/* RE2 SPL_SCE.C: sub player scenario moves (Spl_Scenario, Spl_sce00-09) */
#include "bio3.h"
#include "em_sub.h"

extern UCHAR D_8009ED88[]; /* move offset by Spl_mot_set_no result */
extern SHORT D_8009EDA4[];
extern SHORT D_8009EDCC[];
extern SHORT D_8009EDF4[];
extern SHORT D_8009EE1C[];
extern void (*Spl_sce[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern void (*Spl_sce0a_tbl[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern SHORT Spl_slope_dir_tbl[]; /* direction by Col.Unk39 & 7 */

void Spl_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Spl_sce_hokan(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void Spl_sce_hokan_set(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce_foot_se(ENEMY_WORK *pEm);
void Spl_sce00(ENEMY_WORK *pEm);
void Spl_sce01(ENEMY_WORK *pEm);
void Spl_sce02(ENEMY_WORK *pEm);
void Spl_sce03(ENEMY_WORK *pEm);
void Spl_sce04(ENEMY_WORK *pEm);
void Spl_sce0c(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce05(ENEMY_WORK *pEm);
void Spl_sce0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce06(ENEMY_WORK *pEm);
void Spl_sce07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq); /* pKan, pSeq unused; Spl_sce06 passes them */
void Spl_sce0e(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce09(ENEMY_WORK *pEm);
void Spl_sce0a_init(ENEMY_WORK *pEm);
void Spl_sce0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce0a_00(ENEMY_WORK *pEm);
void Spl_sce0a_01(ENEMY_WORK *pEm);
void Spl_sce0a_02(ENEMY_WORK *pEm);
void Spl_sce0a_07(ENEMY_WORK *pEm);
void Spl_sce0a_03(ENEMY_WORK *pEm);
void Spl_sce0a_04(ENEMY_WORK *pEm);
void Spl_sce0b(ENEMY_WORK *pEm);
void Spl_sce0b_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce14(void);
void Spl_sce15(ENEMY_WORK *pEm);
void Spl_sce_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Spl_sce_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Spl_sce_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d);
UCHAR Spl_sce_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce_mv00_sp(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Spl_sce_mv02_sp(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d);
LONG Spl_slope_dir(ENEMY_WORK *pEm, LONG f);
LONG Spl_slope_dir_run(ENEMY_WORK *pEm, LONG f);
LONG Spl_mot_set_no(ENEMY_WORK *pEm);

#endif /* SPL_SCE_H */
