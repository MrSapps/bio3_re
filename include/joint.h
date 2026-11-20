#ifndef JOINT_H
#define JOINT_H
/* RE2 JOINT.C: model parts setup / joint animation */
#include "em_sub.h"

/* joint state of a separately animated model piece (RE2 struct tagJOINT_P, JOINT.H; 0x30 bytes) */
typedef struct tagJOINT_P {
    UCHAR Move_no;                     /* 0x00 */
    UCHAR Move_cnt;                    /* 0x01 */
    UCHAR Hokan_flg;                   /* 0x02 */
    UCHAR P0_parts_no;                 /* 0x03 */
    ULONG On_parts_flg;                /* 0x04 bit per part */
    MATRIX P0_m;                       /* 0x08 root rotation */
    SVECTOR P0_sv;                     /* 0x28 */
} JOINT_P;

ULONG func_80028308(JOINT_P *pJp, ULONG *pKan, ULONG *pSeq, ULONG Late_flg); /* RE2 Joint_move_p */
ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top);  /* PartsWork_set */
void Parts_work_pos_init(ENEMY_WORK *pEm, SHORT *pKan);      /* part positions from kan data */

#endif /* JOINT_H */
