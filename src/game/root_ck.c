/*
 * ROOT_CK.C (RE2 ROOT_CK.C lineage): enemy route finding over the room's floor blocks (areas)
 */
#include "sca.h"
#include "root_ck.h"
#include "sld.h"

#define RND() ((signed char)Rnd())

/* floor block (pRoom->pBlock: count, then 0x30 byte records from +8) */
typedef struct {
    SHORT X;                           /* 0x00 */
    SHORT Z;                           /* 0x02 */
    USHORT W;                          /* 0x04 */
    USHORT D;                          /* 0x06 */
    UCHAR pad8[0x2E - 0x8];
    SHORT Floor;                       /* 0x2E 0 = any floor, else floor + 0x11 */
} BLOCK;


/* RE2 Root_ck: steers pEm towards pDest (or area Dest_area), through the block graph when not in the same area */
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
            if (Sca_ck_line(WORKM_POS_V(&pEm->pSin_parts_ptr[pEm->Neck_no]),
                            WORKM_POS_V(&pG->Player.pSin_parts_ptr[pG->Player.Neck_no]), 0x40, 1) == 0) {
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

/* RE2 Rnd_area: a random floor block (area) of the room, 0xFF if the room has none */
LONG Rnd_area(void)
{
    USHORT n = *G_pRoom[14];

    if (n == 0) {
        return 0xFF;
    }
    return (UCHAR)(Rnd() % n);
}

/* area (floor block) containing (X, Z) on floor Floor, or 0xFF */
LONG Guide_check(SHORT X, SHORT Z, UCHAR Floor)
{
    USHORT *pBlock;
    BLOCK *pB;
    SHORT Cnt;
    USHORT Depth;

    pBlock = G_pRoom[14];
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

/* do segments pA and pB ({x0, y0, x1, y1}) intersect? (1 = yes) */
ULONG Line_cross_ck(SHORT *pA, SHORT *pB)
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

typedef struct {
    UCHAR pad0[8];
    UCHAR Pt[16];                      /* 0x08 shared point with node n */
    UCHAR Link[16];                    /* 0x18 */
    USHORT Links;                      /* 0x28 neighbour node bits */
    USHORT Cost_ofs;                   /* 0x2A */
    UCHAR Mul;                         /* 0x2C */
    UCHAR pad2D[3];
} ROOT_NODE;
typedef struct {
    SHORT X;
    SHORT Z;
    USHORT W;
    UCHAR Flg;
    UCHAR pad7;
} ROOT_PT;
typedef struct {
    LONG X;
    LONG Z;
    USHORT Open;
    USHORT Visit;
    USHORT Cost;
    UCHAR Node;
    UCHAR Pt;
} ROOT_FRAME;
/* RE2 kara_rootck2: best-path search (DFS over the block graph) from one area to the target area.
 * Returns 0xFFFF0000 when no path, else Best << 16 | 1 (Root_ck ignores it). */
LONG kara_rootck2(ENEMY_WORK *pEm, USHORT *pBlock, ROOT_REQ *pReq, LONG *pPos)
{
    SVECTOR V;
    SVECTOR Dir;
    SHORT A[4];
    SHORT B[4];
    SVECTOR Unused;                    /* never used; the frame layout needs it */
    ROOT_FRAME Stack[16];
    UCHAR Dist_to[16];
    UCHAR Dist_from[16];
    ROOT_NODE *pNode;
    UCHAR *pCost;
    UCHAR To;
    UCHAR From;
    SHORT Tx;
    SHORT Tz;
    LONG Best;
    SHORT Bx;
    SHORT Bz;
    SHORT Wx;
    SHORT Wz;
    UCHAR Wp;
    ROOT_NODE *pN;
    ROOT_PT *pPt;
    ROOT_PT *pP;
    ROOT_PT *pQ;
    ROOT_FRAME *pS;
    USHORT Links;
    ULONG i;
    LONG Dx;
    LONG Dz;
    LONG C;
    LONG D;
    UCHAR Node;
    UCHAR Next;
    LONG R;
    SHORT Sx;
    SHORT Sz;
    UCHAR Area;
    UCHAR Out_link;

    Best = 0x7FFFFFFF;
    pNode = (ROOT_NODE *)((UCHAR *)pBlock + pBlock[1]);
    To = pReq->To_area;
    pN = &pNode[To];
    Links = pN->Links;
    pPt = (ROOT_PT *)((UCHAR *)pBlock + pBlock[2]);
    pCost = (UCHAR *)pBlock + pBlock[3];
    From = pReq->From_area;
    Tx = pReq->Target_x;
    Tz = pReq->Target_z;
    pS = Stack;
    i = 0;
    while (Links) {
        if (Links & 1) {
            Dx = pPt[pN->Pt[(UCHAR)i]].X - Tx;
            Dx *= Dx;
            Dz = pPt[pN->Pt[(UCHAR)i]].Z - Tz;
            Dist_to[(UCHAR)i] = SquareRoot0(Dx + Dz * Dz) >> 8;
        }
        Links >>= 1;
        i++;
    }
    pN = &pNode[From];
    Links = pN->Links;
    i = 0;
    while (Links) {
        if (Links & 1) {
            Dx = pPt[pN->Pt[(UCHAR)i]].X - pPos[0];
            Dx *= Dx;
            Dz = pPt[pN->Pt[(UCHAR)i]].Z - pPos[2];
            Dist_from[(UCHAR)i] = SquareRoot0(Dx + Dz * Dz) >> 8;
        }
        Links >>= 1;
        i++;
    }
    pS->Visit = 1 << From;
    pS->Open = pN->Links;
    pS->X = pPos[0];
    pS->Z = pPos[2];
    pS->Cost = 0;
    pS->Pt = 0xFF;
    pS->Node = From;
    Next = From;
    while (Stack[0].Open != 0 || pS != Stack) {
        Node = Next;
        C = 0;
        if (Node == To) {
            C = Dist_to[pS[-1].Node] + pS->Cost;
            if (C < Best) {
                Best = C;
                pS[1].X = Tx;
                pS[1].Z = Tz;
                Bx = Stack[1].X;
                Bz = Stack[1].Z;
                Wp = Stack[1].Pt;
                Wx = Stack[2].X;
                Wz = Stack[2].Z;
            }
        }
        if (pS->Open == 0 || C != 0) {
            pS--;
            Next = pS->Node;
            continue;
        }
        Next = 0;
        while (!((pS->Open >> Next) & 1))
            Next++;
        pS->Open &= ~(1 << Next);
        if (Node == From) {
            pS[1].Cost = Dist_from[Next];
        } else {
            pN = &pNode[Node];
            i = pN->Link[pS[-1].Node] * pN->Mul;
            /* the out link is read first (sched order keeps its address add ahead of the (UCHAR)i extend) */
            Out_link = pN->Link[Next];
            pS[1].Cost = *(pCost + pN->Cost_ofs + (UCHAR)i + Out_link);
        }
        pN = &pNode[Next];
        pS[1].Cost += pS->Cost;
        pS++;
        pS->Visit = pS[-1].Visit | (1 << Next);
        pS->Open = pN->Links & ~pS->Visit;
        pS->Pt = pN->Pt[Node];
        pS->X = pPt[pS->Pt].X;
        pS->Z = pPt[pS->Pt].Z;
        pS->Node = Next;
    }
    if (Best == 0x7FFFFFFF)
        return 0xFFFF0000;
    V.vx = Wx;
    V.vz = Wz;
    Root_step((VECTOR *)pPos, &V, pEm->Col.W0, &Dir);
    pP = &pPt[Wp];
    R = pP->W - pEm->Col.W0;
    if (R < 0)
        R = 100;
    if (pP->Flg) {
        A[0] = Bx;
        A[2] = Bx;
        A[1] = Bz + R;
        A[3] = Bz - R;
    } else {
        A[0] = Bx + R;
        A[1] = Bz;
        A[2] = Bx - R;
        A[3] = Bz;
    }
    B[0] = pPos[0] + Dir.vx;
    B[1] = pPos[2] + Dir.vz;
    B[2] = V.vx;
    B[3] = V.vz;
    if ((UCHAR)Line_cross_ck(A, B)) {
        B[0] = B[0] - (USHORT)Dir.vx * 2;
        B[1] = B[1] - (USHORT)Dir.vz * 2;
        if ((UCHAR)Line_cross_ck(A, B)) {
            pEm->Dest_x = Wx;
            pEm->Dest_z = Wz;
            return Best << 16 | 1;
        }
    }
    if (pP->Flg) {
        D = A[1] - pPos[2];
        if ((ULONG)D < R * 2) {
            pEm->Dest_x = Bx;
            pEm->Dest_z = pPos[2];
            return Best << 16 | 1;
        }
    } else {
        D = A[0] - pPos[0];
        if ((ULONG)D < R * 2) {
            pEm->Dest_x = pPos[0];
            pEm->Dest_z = Bz;
            return Best << 16 | 1;
        }
    }
    V.vx = Bx;
    V.vz = Bz;
    Root_step((VECTOR *)pPos, &V, pEm->Col.W0, &Dir);
    Sx = -Dir.vx;
    Sz = -Dir.vz;
    Area = Guide_check(pPos[0] + Dir.vx, pPos[2] + Dir.vz, pEm->nFloor);
    if (Area == From) {
        if ((UCHAR)Guide_check(pPos[0] - Dir.vx, pPos[2] - Dir.vz, pEm->nFloor) == Area) {
            pEm->Dest_x = Bx;
            pEm->Dest_z = Bz;
            return Best << 16 | 1;
        }
        Sx = Dir.vx;
        Sz = Dir.vz;
    }
    pEm->Dest_x = Bx + Sx;
    pEm->Dest_z = Bz + Sz;
    return Best << 16 | 1;
}

/* step of Spd towards pT from pPos: pOut gets the normalised direction (z into vx, x into vz) * Spd */
void Root_step(VECTOR *pPos, SVECTOR *pT, LONG Spd, SVECTOR *pOut)
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
