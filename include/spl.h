#ifndef SPL_H
#define SPL_H
/* RE2 SPL.C: sub player (partner) main loop and routine table */
#include "bio3.h"
#include "em_sub.h"

extern USHORT D_8009E374[][4]; /* collision size by Id: [0] Unk26, [1] W0/D0 */
extern void (*D_8009E6EC[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

void Spl(ENEMY_WORK *pEm);
void Spl_Init(ENEMY_WORK *pEm);
void func_80059310(ENEMY_WORK *pEm);
void Spl_set(void);
void func_800594D4(ENEMY_WORK *pEm);

#endif /* SPL_H */
