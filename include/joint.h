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

/* PARTS_W free work (RE2 PARTS_W.Free[]; RE3 has it at PARTS_W + 0x74) as the neck parts use it
 * (RE2 struct tagPARTS_NECK_FREE_WORK, MAIN.H) */
typedef struct tagPARTS_NECK_FREE_WORK {
    SHORT N_rot_y;                     /* 0x74 */
    SHORT N_rot_z;                     /* 0x76 */
    SHORT Add_dir_y;                   /* 0x78 */
    SHORT Add_dir_z;                   /* 0x7A */
    SHORT L_rot_y;                     /* 0x7C */
    SHORT L_rot_z;                     /* 0x7E */
} PARTS_NECK_FREE_WORK;
#define PW_NECK(p) ((PARTS_NECK_FREE_WORK *)&(p)->Unk74)

/* PARTS_W free work of colour-faded parts (em34, em37) */
typedef struct tagPARTS_RGB_FREE_WORK {
    ULONG Rgb0;                        /* 0x74 */
    ULONG Rgb1;                        /* 0x78 */
} PARTS_RGB_FREE_WORK;
#define PW_RGB(p) ((PARTS_RGB_FREE_WORK *)&(p)->Unk74)

ULONG func_80028308(JOINT_P *pJp, ULONG *pKan, ULONG *pSeq, ULONG Late_flg); /* RE2 Joint_move_p */
ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top);  /* PartsWork_set */
void Parts_work_pos_init(ENEMY_WORK *pEm, SHORT *pKan);      /* part positions from kan data */

#endif /* JOINT_H */
