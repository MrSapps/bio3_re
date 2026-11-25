/*
 * SABUN.C (RE2 SABUN.C lineage): vertex morphing (sabun = difference) data of enemy parts; RE3 adds two
 * collision helpers at its end
 */
#include "sabun.h"
#include "md_sub.h"

#define RND() ((signed char)Rnd())

/* RE2 Sa_dat_set: links the morph section of an EMD to the parts flagged in its bit mask (PARTS_W.pUnk98) */
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
            pP->pSa_dat_head = NULL;
            if (*pHead & (1 << i)) {
                pP->pSa_dat_head = pData;
                pData += pData[2] + 3;
            }
            pP++;
            i++;
        } while (--n != 0);
    }
}

/* morph step: blends a part's vertices between key Key_no - 1 and Key_no (em34/em36 bodies) */
INCLUDE_ASM("asm/nonmatchings/game/sabun", Sa_morph_step);

/* morph (sabun) data at PARTS_W.pSa_dat_head */
typedef struct {
    ULONG Ofs;                         /* 0x00 offset of the shapes from ENEMY_WORK.pUnk118 */
    ULONG Size;                        /* 0x04 bytes per shape */
    struct {
        USHORT Rate;                   /* weight of shape n (0x1000 = full); Key[0].Rate = shape count */
        USHORT pad;
    } Key[1];                          /* 0x08 */
} SA_DATA;

/* weighted morph: base shape plus every target scaled by its Rate (0x1000 = full) */
void Sa_morph_blend(ENEMY_WORK *pEm, PARTS_W *pP)
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
    pSa = (SA_DATA *)pP->pSa_dat_head;
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

/* interpolates the parts' joint offsets between two morph keys with the GTE (gpf/gpl) */
INCLUDE_ASM("asm/nonmatchings/game/sabun", Sa_joint_blend);

/* collision pass over the room's SCA blocks: calls each shape's hit routine (D_8009DFEC table), then
 * Pl_floor_ck */
INCLUDE_ASM("asm/nonmatchings/game/sabun", Sca_hit_all);

/* player-only check: floor heights at points around the player (Rot_vector + floor height), sets the hit flags */
INCLUDE_ASM("asm/nonmatchings/game/sabun", Pl_floor_ck);
