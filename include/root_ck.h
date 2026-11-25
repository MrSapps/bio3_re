#ifndef ROOT_CK_H
#define ROOT_CK_H
/* RE2 ROOT_CK.C: enemy path finding (Root_ck, Rnd_area, Guide_check) */
#include "bio3.h"
#include "em_sub.h"

/* route request passed to kara_rootck2 */
typedef struct {
    UCHAR From_area;                   /* 0x00 */
    UCHAR To_area;                     /* 0x01 */
    UCHAR pad2[2];
    SHORT Target_x;                    /* 0x04 */
    SHORT Target_z;                    /* 0x06 */
} ROOT_REQ;

extern USHORT **D_800CC86C;

LONG Root_ck(ENEMY_WORK *pEm, VECTOR *pDest, UCHAR Dest_area, UCHAR Mode);
LONG Rnd_area(void);
LONG Guide_check(SHORT X, SHORT Z, UCHAR Floor);
ULONG Line_cross_ck(SHORT *pA, SHORT *pB);
void kara_rootck2(ENEMY_WORK *pEm, USHORT *pBlock, ROOT_REQ *pReq, LONG *pPos);
void Root_step(VECTOR *pPos, SVECTOR *pT, LONG Spd, SVECTOR *pOut);

#endif /* ROOT_CK_H */
