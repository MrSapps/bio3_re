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

#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldlvl(r0) __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0)" : : "r"(r0))
#define gte_stlvl(r0) __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(r0) : "memory")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_gpl12() __asm__ volatile("nop; nop; .word 0x4BA8003E")
#define gte_gpl12_b() __asm__ volatile(".word 0x4BA8003E") /* no leading nops */
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

/* morph (sabun) data at PARTS_W.pSa_dat_head */
typedef struct {
    ULONG Ofs;                         /* 0x00 offset of the shapes from ENEMY_WORK.pUnk118 */
    ULONG Size;                        /* 0x04 bytes per shape */
    struct {
        USHORT Rate;                   /* weight of shape n (0x1000 = full); Key[0].Rate = shape count */
        USHORT Shape;                  /* frame block of the KAN parts for this key */
    } Key[1];                          /* 0x08 */
} SA_DATA;

/* morph step: blends a part's vertices between key Key_no - 1 and Key_no (em34/em36 bodies) */
void Sa_morph_step(ENEMY_WORK *pEm, PARTS_W *pP)
{
    SVECTOR Tmp;
    SA_DATA *pSa;
    ULONG *pHead;
    UCHAR *pKan;
    UCHAR *pKan_base;
    UCHAR *pIdx;
    UCHAR *pIdx_top;
    SHORT *pBase;
    SHORT *pS;
    SHORT *pFrm;
    SHORT *pBlk;
    SVECTOR *pVert;
    SVECTOR *pV;
    PARTS_W *pParts;
    PARTS_W *pW;
    ULONG nVert;
    ULONG n;
    ULONG j;
    ULONG k;
    ULONG Half;
    ULONG Cnt_top;
    ULONG Parts_no;
    ULONG Parts_cmp;
    ULONG Kan_cnt;
    ULONG Out;
    ULONG Stride;
    ULONG Idx;
    LONG Rate;
    LONG Inv;

    j = 1;
    pSa = (SA_DATA *)pP->pSa_dat_head;
    pHead = pEm->pUnk118;
    n = (USHORT)pP->pTmd->n_vert;
    pV = (SVECTOR *)pP->pTmd->pVert;
    nVert = n;
    pVert = pV;
    pS = (SHORT *)(pSa->Ofs + (ULONG)pHead);
    pBase = pS;
    do {
        pV->vx = *pS++;
        pV->vy = *pS++;
        pV->vz = *pS++;
        pV++;
    } while (--n != 0);
    pKan = (UCHAR *)pEm->pKan_t_ptr;
    Parts_no = pP->Parts_no;
    k = pSa->Key[0].Rate;
    Half = pSa->Size >> 1;
    pParts = pEm->pSin_parts_ptr;
    pKan_base = pKan + (*(USHORT *)pKan & 0xFFFC);
    n = *(ULONG *)(pKan_base + (Parts_no << 2)); /* entry: index offset << 16 | part count */
    Kan_cnt = (USHORT)((KAN_HEAD *)pKan)->Rec_sz;
    pIdx = pKan_base + (n >> 16);
    pIdx_top = pIdx;
    pFrm = (SHORT *)(pHead[1] + (ULONG)pHead);
    /* compared through a copy: Parts_no itself stays block-local (a2) */
    Parts_cmp = Parts_no;
    if (Parts_cmp < Kan_cnt) {
        Cnt_top = n = n & 0xFFFF;
        while (n--) {
            Idx = *pIdx;
            pW = &pParts[Idx];
            pW->Pos_x = pFrm[Idx * 3];
            pW->Pos_y = pFrm[Idx * 3 + 1];
            pW->Pos_z = pFrm[Idx * 3 + 2];
            pIdx++;
        }
    }
    Stride = pHead[2];
    Out = Kan_cnt < Parts_cmp + 1;
    do {
        Rate = pSa->Key[j].Rate;
        do { Inv = 0x1000 - Rate; } while (0); /* do/while: Inv must outrank k in global-alloc (t8) */
        if (Rate != 0) {
            pS = pBase + j * Half;
            pV = pVert;
            n = nVert;
            do {
                pV->vx += (*pS++ * Rate) >> 12;
                pV->vy += (*pS++ * Rate) >> 12;
                pV->vz += (*pS++ * Rate) >> 12;
                pV++;
            } while (--n != 0);
            if (!Out) {
                pBlk = (SHORT *)((UCHAR *)pFrm + Stride * pSa->Key[j].Shape);
                pIdx = pIdx_top;
                n = Cnt_top;
                Idx = *pIdx;
                if (n--) {
                loop:
                    pW = &pParts[Idx];
                    gte_lddp(Inv);
                    gte_ldlvl(&pW->Pos_x);
                    gte_gpf12();
                    Tmp.vx = pBlk[Idx * 3];
                    Tmp.vy = pBlk[Idx * 3 + 1];
                    Tmp.vz = pBlk[Idx * 3 + 2];
                    gte_lddp(Rate);
                    gte_ldsv(&Tmp);
                    pIdx++;
                    Idx = *pIdx;
                    gte_gpl12_b();
                    gte_stlvl(&pW->Pos_x);
                    if (n--)
                        goto loop;
                }
            }
        }
        /* nested do/while: the j * Half giv step lands here, its refs put Half first (t7) in global-alloc */
        do { do { do { do { j++; } while (0); } while (0); } while (0); } while (0);
    } while (--k != 0);
}

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
void Sa_joint_blend(ENEMY_WORK *pEm, PARTS_W *pP)
{
    ULONG *pHead;
    SHORT *pBase;
    SHORT *pS;
    SHORT *pFrom;
    LONG *pPos;
    PARTS_W *pParts;
    PARTS_W *pW;
    ULONG *pSa;
    ULONG n;
    ULONG i;
    ULONG Cnt;
    ULONG k;
    ULONG Key;
    ULONG Rate;
    ULONG Inv;
    ULONG j;
    ULONG Shape_sz;
    SVECTOR Tmp;

    j = 1;
    pHead = pEm->pUnk118;
    pSa = pP->pSa_dat_head;
    pBase = (SHORT *)(pHead[1] + (LONG)pHead);
    pParts = pEm->pSin_parts_ptr;
    n = (USHORT)((KAN_HEAD *)pEm->pKan_t_ptr)->Rec_sz;
    if (pP->Parts_no + 1 < n) {
        Cnt = ~-n;                     /* n - 1; written so cse does not fold the first Cnt-- into n - 2 */
        if (Cnt--) {
            pFrom = pBase + 3;
            pPos = &pParts[1].Pos_z;
        loop1:
            pPos[-2] = pFrom[0];
            pPos[-1] = pFrom[1];
            pPos[0] = pFrom[2];
            pFrom += 3;
            pPos += sizeof(PARTS_W) / sizeof(LONG);
            if (Cnt--)
                goto loop1;
        }
    }
    k = ((USHORT *)pSa)[4];
    Shape_sz = pHead[2];
    do {
        Key = pSa[j + 2];
        Rate = Key & 0xFFFF;
        Inv = 0xFFF - Rate;
        Key = (ULONG)pBase + Shape_sz * (Key >> 16);
        pW = pParts + 1;
        i = n - 1;
        if (i--) {
            pS = (SHORT *)Key + 3;
        loop2:
            do {                       /* do/while(0) raises Inv's allocation priority (t0) */
                gte_lddp(Inv);
            } while (0);
            gte_ldlvl(&pW->Pos_x);
            gte_gpf12();
            Tmp.vx = pS[0];
            Tmp.vy = pS[1];
            Tmp.vz = pS[2];
            gte_lddp(Rate);
            gte_ldsv(&Tmp);
            gte_gpl12();
            gte_stlvl(&pW->Pos_x);
            do {                       /* nested do/while(0) raise pW's priority over i (a1/a2) */
                do {
                    do {
                        pW++;
                    } while (0);
                } while (0);
            } while (0);
            pS += 3;
            if (i--)
                goto loop2;
        }
        j++;
    } while (--k != 0);
}

/* collision pass of a COL_OBJ (radius R) over the room's SCA shapes whose Id has a bit of Tbl, in its area
 * and floor range: shape 5 goes through Sca_hit_line, the others through Sca_rtn_tbl (45 degree shapes test the
 * rotated position kept in Sca_work.X/Z). Pushes the owner by the move and records the shape; shapes 7/8 also
 * try the player's climb (Pl_floor_ck). Skipped when the object did not move */
LONG Sca_hit_all(ENEMY_WORK *pEm, LONG R, ULONG Tbl, COL_OBJ *pObj)
{
    SCA_DATA *pT;
    ULONG Area;
    SCA_DATA *pEnd;
    ULONG P1;
    SCA_WORK *pW;
    LONG X;
    LONG Old_x;
    LONG Old_z;
    LONG X0;
    LONG Z0;
    LONG X1;
    LONG Z1;
    LONG Cx;
    LONG Cz;
    LONG Dx;
    LONG Rx;
    LONG Rz;
    LONG Corner;
    LONG Dz;
    LONG Shape;
    LONG Ret;
    LONG Lo;
    LONG Hi;

    pT = (SCA_DATA *)G.pRoom->pSca;
    pEnd = pT + ((SCA_HEAD *)pT)->Num;
    pW = &Sca_work;
    if (((SCA_HEAD *)pT)->Num == 0)
        return 0;
    if (pEm->Be_flg & 0x2000)
        return 0;
    do {
        X = pObj->Pos_x;
    } while (0); /* loop notes keep the load first (sched1 would sink it to the call) */
    Shape = pObj->Pos_z; /* one variable for the z move and the shape number */
    P1 = (pObj->Old_x << 16) | (USHORT)pObj->Old_z;
    Area = Sca_get_area((SHORT)pObj->Pos_x, (SHORT)pObj->Pos_z);
    Shape -= pObj->Old_z;
    pObj->pSca_hit_data = NULL;
    if (X == pObj->Old_x && Shape == 0 && pObj->W0 == pObj->W1)
        return Ret; /* (not set yet: Capcom bug) */
    pObj->Hit_flg &= 0xFF3F;
    for (pT++; pT != pEnd; pT++) {
        Shape = pT->Id & 0xF;
        Ret = 0;
        if ((pT->Type & Area) != Area)
            goto next;
        if (!(pT->Id & Tbl))
            goto next;
        if (pT->Floor_lo > pEm->nFloor || pT->Floor_hi < pEm->nFloor)
            goto next;
        X0 = pT->X0;
        X1 = pT->X1;
        Z0 = pT->Z0;
        Z1 = pT->Z1;
        Old_x = pObj->Pos_x;
        Old_z = pObj->Pos_z;
        if (Shape != 5) {
            if (pT->Type & 0x1000) {
                Rx = Old_x - X0;
                Rz = Old_z - Z0;
                Cx = (Rx - Rz) * 181 >> 8;
                Cz = (Rx + Rz) * 181 >> 8;
                Cx += X0 + R;
                Cz += Z0 + R;
            } else {
                Cx = Old_x + R;
                Cz = Old_z + R;
            }
            X1 += R * 2;
            Z1 += R * 2;
            pW->X = Cx - R;
            pW->Z = Cz - R;
            if ((ULONG)(Cx - X0) >= (ULONG)(X1 - X0))
                goto next;
            if ((ULONG)(Cz - Z0) >= (ULONG)(Z1 - Z0))
                goto next;
            do {
                Ret = Sca_rtn_tbl[Shape](pObj, R, pT, P1);
                if (Shape < 7)
                    goto next;
            } while (0); /* raises the refs of Shape over pObj (s2/s3) */
            if (Shape >= 9) {
                pObj->Hit_flg |= 0x80;
                if (Shape == 10) {
                    pObj->Hit_flg |= 0xC0;
                    Corner = (pT->Id >> 4) & 3;
                    if (pT->Type & 0x1000)
                        Corner += 4;
                    pObj->Unk39 = Corner;
                }
                goto next;
            }
            if (pObj->Type != 0)
                goto next;
            if (Ret == 0)
                continue;
            if (!(pObj->Hit_flg & 0x600)) {
                if (Sca_ck_wall_range(pObj, pT))
                    Pl_floor_ck((COL_ENT *)pT, Shape, 1);
                goto next;
            }
            goto hit;
        } else {
            R <<= 1;
            pW->Pt0 = (X0 << 16) | (Z0 & 0xFFFF);
            pW->Pt1 = (X1 << 16) | (Z1 & 0xFFFF);
            X0 -= R;
            X1 += R;
            Hi = Z1;
            if (Z1 < Z0)
                Hi = Z0;
            Lo = Z1;
            if (Z0 < Z1)
                Lo = Z0;
            Z0 = Lo - R;
            Z1 = Hi + R;
            R >>= 1;
            if ((ULONG)(pObj->Pos_x - X0) >= (ULONG)(X1 - X0))
                goto next;
            if ((ULONG)(pObj->Pos_z - Z0) >= (ULONG)(Z1 - Z0))
                goto next;
            Ret = Sca_hit_line(pObj, R, P1);
        }
    next:
        if (Ret == 0)
            continue;
    hit:
        Dx = pObj->Pos_x - Old_x;
        Dz = pObj->Pos_z - Old_z;
        pObj->pEm->Pos_x += Dx;
        pObj->pEm->Pos_z += Dz;
        if (pObj->Type < 3) {
            pObj->pEm->Base_pos_x += Dx;
            pObj->pEm->Base_pos_z += Dz;
        }
        pObj->Hit_flg |= 0x20;
        pObj->pSca_hit_data = pT;
    }
    return Ret;
}

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
