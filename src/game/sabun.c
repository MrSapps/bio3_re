#include "sabun.h"
#include "md_sub.h"

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

/* morph (sabun) data at PARTS_W.pUnk98 */
typedef struct {
    ULONG Ofs;                         /* 0x00 offset of the shapes from ENEMY_WORK.pUnk118 */
    ULONG Size;                        /* 0x04 bytes per shape */
    struct {
        USHORT Rate;                   /* weight of shape n (0x1000 = full); Key[0].Rate = shape count */
        USHORT pad;
    } Key[1];                          /* 0x08 */
} SA_DATA;

void func_8004AC54(ENEMY_WORK *pEm, PARTS_W *pP)
{
    SA_DATA *pSa;
    SVECTOR *pVert;
    SHORT *pBase;
    SVECTOR *pV;
    SHORT *pS;
    ULONG nVert;
    ULONG n;
    ULONG k;
    ULONG i;
    ULONG Half;
    LONG Rate;
    SVECTOR Unused;
    ULONG Head;

    Head = (ULONG)pEm->pUnk118;
    i = 1;
    pSa = (SA_DATA *)pP->pUnk98;
    n = (USHORT)pP->pTmd->n_vert;
    pV = (SVECTOR *)pP->pTmd->pVert;
    nVert = n;
    pVert = pV;
    pS = (SHORT *)(pSa->Ofs + Head);
    pBase = pS;
    /* base shape */
    do {
        pV->vx = pS[0];
        pV->vy = pS[1];
        pV->vz = pS[2];
        pS += 3;
        pV++;
    } while (--n != 0);
    /* add each weighted morph target (Key[0].Rate is the target count) */
    Half = pSa->Size >> 1;
    k = pSa->Key[0].Rate;
    do {
        Rate = (&pSa->Key[0] + i)->Rate;
        if (Rate == 0) {
            i++;
            continue;
        }
        pS = pBase + i * Half;
        pV = pVert;
        n = nVert;
        do {
            pV->vx += (pS[0] * Rate) >> 12;
            pV->vy += (pS[1] * Rate) >> 12;
            pV->vz += (pS[2] * Rate) >> 12;
            pS += 3;
            pV++;
        } while (--n != 0);
        i++;
    } while (--k != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004AD94);

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004AF04);

INCLUDE_ASM("asm/nonmatchings/game/sabun", func_8004B374);
