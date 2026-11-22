#include "pl_wep.h"
#include "sca.h"
#include "monitor.h"
#include "pl_wep2.h"
#include "pad.h"
#include "esp.h"

ESP_WORK *func_8001B484(ULONG Id, ULONG Dir, MATRIX *pM, SVECTOR *pV, SVECTOR *pDsv);
SHORT func_8006D030(ULONG Arg0);
void func_80044804(MATRIX *pM, ULONG Arg1, ULONG Type);
void func_80047860(MATRIX *pM, ULONG Arg1, ULONG Type);
extern ULONG D_8009DBB4[];
ULONG func_800102E8(void);                 /* rand */
SHORT Get_dir_xz(LONG X, LONG Z);
LONG func_800455B4();
LONG func_8004EF74(ULONG P0, ULONG P1, ULONG P2, ULONG P3);

extern UCHAR D_8009CD3D;
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
signed char func_8006CF0C_r(ULONG Arg0) __asm__("func_8006CF0C");
void Pl_water(ENEMY_WORK *pEm);

/* declarations from earlier parts of the original address-split file */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
void func_8006CF0C(ULONG Arg0);
void Mag_set(PLAYER_WORK *pEm);

void Pl_wep_flag_set(PLAYER_WORK *pEm);

#define RND() ((signed char)func_800102E8())

extern PARTS_W *D_8009CD30;
extern USHORT D_8009DCB4[];
void Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
void func_8007809C(LONG a0, LONG a1, ULONG a2, ULONG a3);

extern ENEMY_WORK Pl_work;
extern UCHAR D_8009DDC4[][2];
extern UCHAR D_8009DE10;
extern UCHAR D_8009DE24;

extern UCHAR D_8009DE24;
extern USHORT D_8009DE26;
extern signed char D_8009DE28;

extern LONG D_800D204C[];

void func_80010150(void *p, ULONG Size);
void func_80048FF4(void);

/* floor block (pRoom->pBlock: count, then 0x30 byte records from +8) */
typedef struct {
    SHORT X;                           /* 0x00 */
    SHORT Z;                           /* 0x02 */
    USHORT W;                          /* 0x04 */
    USHORT D;                          /* 0x06 */
    UCHAR pad8[0x2E - 0x8];
    SHORT Floor;                       /* 0x2E 0 = any floor, else floor + 0x11 */
} BLOCK;

LONG Guide_check(SHORT X, SHORT Z, UCHAR Floor);

/* pEm->pad12E[0] is RE2 Root_ck_cnt (em33.c still uses the pad name) */
#define ROOT_CK_CNT(pEm) ((pEm)->pad12E[0])

LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);

/* route request passed to kara_rootck2 */
typedef struct {
    UCHAR From_area;                   /* 0x00 */
    UCHAR To_area;                     /* 0x01 */
    UCHAR pad2[2];
    SHORT Target_x;                    /* 0x04 */
    SHORT Target_z;                    /* 0x06 */
} ROOT_REQ;

void kara_rootck2(ENEMY_WORK *pEm, USHORT *pBlock, ROOT_REQ *pReq, LONG *pPos);

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

    pCnt = &ROOT_CK_CNT(pEm);
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

ULONG func_800102E8(void);
extern USHORT **D_800CC86C;

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
