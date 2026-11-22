#include "sabun.h"

#define RND() ((signed char)Rnd())

void Sa_dat_set(ENEMY_WORK *pEm, ULONG *pData)
{
    PARTS_W *pP;
    ULONG *pHead;
    ULONG n;
    ULONG i;

    pEm->pUnk118 = pData;
    pHead = pData;
    if (*pData != 0) {
        i = 0;
        pData += 3;
        pP = pEm->pSin_parts_ptr;
        n = pEm->pTmd[-1];
        do {
            pP->pUnk98 = NULL;
            if (*pHead & (1 << i)) {
                pP->pUnk98 = pData;
                pData += pData[2] + 3;
            }
            pP++;
            i++;
        } while (--n != 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004A934);

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004AC54);

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004AD94);

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004AF04);

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004B374);
