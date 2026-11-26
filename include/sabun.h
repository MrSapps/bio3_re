#ifndef SABUN_H
#define SABUN_H
/* RE2 SABUN.C: model morph (vertex difference) animation */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"

void Sa_dat_set(ENEMY_WORK *pEm, ULONG *pData);
void Sa_morph_step(ENEMY_WORK *pEm, PARTS_W *pP);
void Sa_morph_blend(ENEMY_WORK *pEm, PARTS_W *pP); /* apply the morph keys */
void Sa_joint_blend(ENEMY_WORK *pEm, PARTS_W *pP);
LONG Sca_hit_all(ENEMY_WORK *pEm, LONG R, ULONG Tbl, COL_OBJ *pObj);
LONG Pl_floor_ck(COL_ENT *pEnt, ULONG Type, ULONG Arg2);

#endif /* SABUN_H */
