#include "sca.h"
#include "root_ck.h"
#include "sld.h"

#define RND() ((signed char)func_800102E8())

/* floor block (pRoom->pBlock: count, then 0x30 byte records from +8) */
typedef struct {
    SHORT X;                           /* 0x00 */
    SHORT Z;                           /* 0x02 */
    USHORT W;                          /* 0x04 */
    USHORT D;                          /* 0x06 */
    UCHAR pad8[0x2E - 0x8];
    SHORT Floor;                       /* 0x2E 0 = any floor, else floor + 0x11 */
} BLOCK;


LONG Root_ck(ENEMY_WORK *pEm, VECTOR *pDest, UCHAR Dest_area, UCHAR Mode)
{
    tagGLOBAL *pG = &G;
    ROOT_REQ Req;
    BLOCK *pB;
    LONG Target_x;
    LONG Target_z;
    ULONG From_area;
    ULONG Area;
    USHORT *pStatus;
    UCHAR *pCnt;

    pCnt = &pEm->Root_ck_cnt;
    pStatus = &pEm->Status_flg;

    if (pG->pRoom->pBlock == NULL || pEm->Routine_0 == 0) {
        *pStatus |= 0x2000;
        return 0;
    }
    if (*pCnt == 0) {
        *pCnt = 8;
    }
    (*pCnt)--;
    if (pEm->Life < 0 && pEm->Id != 0x3A) {
        return 0;
    }
    if ((*pCnt & 0xF) == 0) {
        if (pEm->Type & 0x800) {
            *pStatus |= 0x2000;
        } else {
            *pStatus &= ~0x2000;
            if (Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t,
                            (VECTOR *)pG->Pl_pSin_parts_ptr[pG->Pl_neck_no].Workm.t, 0x40, 1) == 0) {
                *pStatus |= 0x2000;
            }
        }
    }
    if (*pStatus & 0x8000) {
        pEm->Dest_x = pDest->vx;
        pEm->Dest_z = pDest->vz;
        return 2;
    }
    if (pEm->Type & 0x5000) {
        return 2;
    }
    From_area = (UCHAR)Guide_check(pEm->Pos_x, pEm->Pos_z, pEm->nFloor);
    Target_x = pDest->vx;
    Target_z = pDest->vz;
    Area = (UCHAR)Guide_check(pDest->vx, pDest->vz, pDest->vy / -1800);
    if ((From_area != 0xFF && Area != 0xFF) || Mode) {
        if (Mode) {
            Area = Dest_area;
            pB = (BLOCK *)(pG->pRoom->pBlock + Area * 24 + 4);
            Target_x = (pB->X + (SHORT)pB->W) >> 1;
            Target_z = (pB->Z + (SHORT)pB->D) >> 1;
        }
        if (From_area == Area) {
            pEm->Dest_x = Target_x;
            pEm->Dest_z = Target_z;
            return 1;
        }
        if (*pCnt == 0) {
            Req.From_area = From_area;
            Req.To_area = Area;
            Req.Target_x = Target_x;
            Req.Target_z = Target_z;
            kara_rootck2(pEm, pG->pRoom->pBlock, &Req, &pEm->Pos_x);
            return 1;
        }
        return 2;
    }
}

LONG Rnd_area(void)
{
    USHORT n = *D_800CC86C[14];

    if (n == 0) {
        return 0xFF;
    }
    return (UCHAR)(func_800102E8() % n);
}

/* area (floor block) containing (X, Z) on floor Floor, or 0xFF */
LONG Guide_check(SHORT X, SHORT Z, UCHAR Floor)
{
    USHORT *pBlock;
    BLOCK *pB;
    SHORT Cnt;
    USHORT Depth;

    pBlock = D_800CC86C[14];
    Cnt = *pBlock;
    pB = (BLOCK *)(pBlock + Cnt * 24 + 4);
    do {
        pB--;
        Cnt--;
        if (pB->Floor == 0 || pB->Floor == Floor + 0x11) {
            if ((USHORT)(X - pB->X) < pB->W) {
                Depth = pB->D;
                if ((USHORT)(Z - pB->Z) < Depth) {
                    return (UCHAR)Cnt;
                }
            }
        }
    } while (Cnt != 0);
    return 0xFF;
}

ULONG func_80049F80(SHORT *pA, SHORT *pB)
{
    LONG dx1, dy2, dy1, dx2;
    LONG Den, dy, dx, S, T;

    dx1 = pA[2] - pA[0];
    dy2 = pB[3] - pB[1];
    dy1 = pA[3] - pA[1];
    dx2 = pB[2] - pB[0];
    Den = dx1 * dy2 - dy1 * dx2;
    if (Den == 0) {
        return 0;
    }
    dy = pA[1] - pB[1];
    dx = pA[0] - pB[0];
    S = dy * dx2 - dx * dy2;
    T = dy * dx1 - dx * dy1;
    if (((Den > 0 && S >= 0 && S <= Den) || (Den < 0 && S >= Den && S <= 0))
        && ((Den > 0 && T >= 0 && T <= Den) || (Den < 0 && T >= Den && T <= 0))) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/root_ck", kara_rootck2);

void func_8004A7F4(VECTOR *pPos, SVECTOR *pT, LONG Spd, SVECTOR *pOut)
{
    LONG dx;
    LONG dz;
    LONG d;

    dz = (pT->vz - pPos->vz) >> 1;
    dx = (pT->vx - pPos->vx) >> 1;
    d = dz * dz + dx * dx;
    dz <<= 8;
    d = SquareRoot0(d) + 1;
    dz = dz / d;
    dx <<= 8;
    dx = dx / d;
    dz *= Spd;
    dx *= Spd;
    pOut->vx = dz >> 8;
    pOut->vz = dx >> 8;
}
