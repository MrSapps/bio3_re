/*
 * SABUN.C (RE2 SABUN.C lineage): vertex morphing (sabun = difference) data of enemy parts; RE3 adds two
 * collision helpers at its end
 */
#include "sabun.h"
#include "md_sub.h"
#include "sca.h"
#include "player.h"
#include "enemy/em_common.h"

/* SABUN.C has no prototype for Rot_vector (sub.h): the direction is passed as an int, unconverted */
void Rot_vector();

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

/* collision pass over the room's SCA blocks: calls each shape's hit routine (Sca_rtn_tbl table), then
 * Pl_floor_ck */
INCLUDE_ASM("asm/nonmatchings/game/sabun", Sca_hit_all);

/* player climb / jump-down check against shape pEnt (Type 7: a step up, the floor 1800 above one point in front
 * must be at that height; Type 8: a drop, five points in front must be at least 1800 below). With Arg2 the player
 * must face the side of the shape Sca_get_dir returns. On success sets the climb (0xB01) or jump-down (0x20B01)
 * routine and returns 1 */
LONG Pl_floor_ck(COL_ENT *pEnt, ULONG Type, ULONG Arg2)
{
    SVECTOR V;
    SVECTOR Unused[2]; /* never used (keeps Pos at sp + 0x28) */
    VECTOR Pos;
    ENEMY_WORK *pPl;
    LONG Dir;
    LONG Side;

    pPl = (ENEMY_WORK *)&G.Player; /* lets cse address G as pPl - 0x248C */
    Dir = pPl->Cdir_y + 0x200;
    Side = Dir & 0xFFF; /* quadrant the player faces, as Sca_get_dir returns it */
    Side >>= 10;
    Side ^= 2;
    if (EM_RTN01(pPl) == 0x501)
        return 0;
    if (pPl->Damage_cnt)
        return 0;
    if (G.Status_flg & STAT_PL_BUSY)
        return 0;
    if (!(G.Key & 0x80))
        return 0;
    if (Arg2 && Sca_get_dir(POS_V(pPl), (SCA_DATA *)pEnt) != Side)
        return 0;
    if (Type == 7) {
        Dir &= 0xFC00;
        V.vx = 1000;
        V.vz = 0;
        Rot_vector(Dir, &V, &V);
        Pos.vx = V.vx + pPl->Pos_x;
        Pos.vy = pPl->Pos_y - 1800;
        Pos.vz = V.vz + pPl->Pos_z;
        if (Sca_get_ground(&Pos.vx, pPl->Col.W0, 0x8000, 0x10) != Pos.vy)
            return 0;
        if (Oba_pos_em_ck(&Pos, pPl))
            return 0;
        if (pPl->Routine_0 == 1 && pPl->Routine_1 != 11 && pPl->Routine_1 != 12) {
            G.Status_flg = (G.Status_flg & 0xFF7FFFFF) | STAT_PL_BUSY;
            EM_RTN(pPl) = 0xB01;
            pPl->Col.Hit_flg |= 0x280;
            return 1;
        }
    }
    if (Type == 8) {
        Dir &= 0xFC00;
        V.vx = 1000;
        V.vz = 0;
        Rot_vector(Dir, &V, &V);
        Pos.vx = V.vx + pPl->Pos_x;
        Pos.vy = pPl->Pos_y + 1800;
        Pos.vz = V.vz + pPl->Pos_z;
        if (Sca_get_ground(&Pos.vx, pPl->Col.W0, 0x8000, 0x10) < Pos.vy)
            return 0;
        V.vx = 1518;
        V.vz = 0;
        Rot_vector(Dir + 0xC4, &V, &V);
        Pos.vx = V.vx + pPl->Pos_x;
        Pos.vy = pPl->Pos_y + 1800;
        Pos.vz = V.vz + pPl->Pos_z;
        if (Sca_get_ground(&Pos.vx, pPl->Col.W0, 0x8000, 0x10) < Pos.vy)
            return 0;
        V.vx = 1518;
        V.vz = 0;
        Rot_vector(Dir - 0xC4, &V, &V);
        Pos.vx = V.vx + pPl->Pos_x;
        Pos.vy = pPl->Pos_y + 1800;
        Pos.vz = V.vz + pPl->Pos_z;
        if (Sca_get_ground(&Pos.vx, pPl->Col.W0, 0x8000, 0x10) < Pos.vy)
            return 0;
        Dir &= 0xFC00;
        V.vx = pPl->Col.W0 + 1000;
        V.vz = 0;
        Rot_vector(Dir, &V, &V);
        Pos.vx = V.vx + pPl->Pos_x;
        Pos.vy = pPl->Pos_y + 1800;
        Pos.vz = V.vz + pPl->Pos_z;
        if (Sca_get_ground(&Pos.vx, pPl->Col.W0, 0x8000, 0x10) < Pos.vy)
            return 0;
        V.vx = 1000 - pPl->Col.W0;
        V.vz = 0;
        Rot_vector(Dir & 0xFC00, &V, &V);
        Pos.vx = V.vx + pPl->Pos_x;
        Pos.vy = pPl->Pos_y + 1800;
        Pos.vz = V.vz + pPl->Pos_z;
        if (Sca_get_ground(&Pos.vx, pPl->Col.W0, 0x8000, 0x10) < Pos.vy)
            return 0;
        if (Oba_pos_em_ck(&Pos, pPl))
            return 0;
        if (pPl->Routine_0 == 1 && pPl->Routine_1 != 11 && pPl->Routine_1 != 12) {
            G.Status_flg = (G.Status_flg & 0xFF7FFFFF) | STAT_PL_BUSY;
            EM_RTN(pPl) = 0x20B01;
            pPl->Col.Hit_flg |= 0x480;
            return 1;
        }
    }
    return 0;
}
