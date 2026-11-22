#ifndef SABUN_H
#define SABUN_H
/* RE2 SABUN.C: model morph (vertex difference) animation */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"

void Sa_dat_set(ENEMY_WORK *pEm, ULONG *pData);
void func_8004A934(ENEMY_WORK *pEm, PARTS_W *pP);
void func_8004AC54(ENEMY_WORK *pEm, PARTS_W *pP); /* apply the morph keys */
void func_8004AD94(ENEMY_WORK *pEm, PARTS_W *pP);
LONG func_8004AF04(ENEMY_WORK *pEm, ULONG W, ULONG Tbl, COL_OBJ *pObj);
LONG func_8004B374(COL_ENT *pEnt, ULONG Type, ULONG Arg2);

#endif /* SABUN_H */
