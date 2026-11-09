#ifndef JOINT_H
#define JOINT_H
/* RE2 JOINT.C: model parts setup / joint animation */
#include "em_sub.h"

ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top);  /* PartsWork_set */
void Parts_work_pos_init(ENEMY_WORK *pEm, SHORT *pKan);      /* part positions from kan data */

#endif /* JOINT_H */
