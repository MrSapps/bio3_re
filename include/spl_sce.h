#ifndef SPL_SCE_H
#define SPL_SCE_H
/* RE2 SPL_SCE.C: sub player scenario moves (Spl_Scenario, Spl_sce00-09) */
#include "bio3.h"
#include "em_sub.h"

extern LONG D_8009DBD4;
extern UCHAR D_8009ED88[]; /* move offset by func_80063690 result */
extern SHORT D_8009EDA4[];
extern SHORT D_8009EDCC[];
extern SHORT D_8009EDF4[];
extern SHORT D_8009EE1C[];
extern void (*D_8009EE44[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern void (*D_8009EEA4[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern SHORT D_8009EEC8[]; /* direction by Col_unk39 & 7 */
extern ULONG D_800D1FC0[];

void Spl_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_8006037C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void func_80060464(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8006055C(ENEMY_WORK *pEm);
void Spl_sce00(ENEMY_WORK *pEm);
void Spl_sce01(ENEMY_WORK *pEm);
void Spl_sce02(ENEMY_WORK *pEm);
void Spl_sce03(ENEMY_WORK *pEm);
void Spl_sce04(ENEMY_WORK *pEm);
void func_800608C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce05(ENEMY_WORK *pEm);
void func_80060E90(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce06(ENEMY_WORK *pEm);
void Spl_sce07(ENEMY_WORK *pEm);
void func_80061390(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce09(ENEMY_WORK *pEm);
void func_80061964(ENEMY_WORK *pEm);
void func_80061970(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_800619F0(ENEMY_WORK *pEm);
void func_80061AD0(ENEMY_WORK *pEm);
void func_80061BC4(ENEMY_WORK *pEm);
void func_80061DF4(ENEMY_WORK *pEm);
void func_800621F0(ENEMY_WORK *pEm);
void func_800622D8(ENEMY_WORK *pEm);
void func_8006236C(ENEMY_WORK *pEm);
void func_800623C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80062970(void);
void func_80062978(ENEMY_WORK *pEm);
void func_80062A28(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_80062B68(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_80062BFC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d);
UCHAR func_80062CD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_sce_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80062F64(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_80063198(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d);
LONG func_800632B8(ENEMY_WORK *pEm, LONG f);
LONG func_800634F4(ENEMY_WORK *pEm, LONG f);
LONG func_80063690(ENEMY_WORK *pEm);

#endif /* SPL_SCE_H */
