#ifndef JOINT_H
#define JOINT_H
/* RE2 JOINT.C: model parts setup / joint animation */
#include "em_sub.h"

/* model colour parameters set from ENEMY_WORK+0x178 (Joint_trans2) */
typedef struct tagMODEL_COL {
    USHORT Unk0[3];                    /* 0x0 */
    SHORT Unk6;                        /* 0x6 */
    ULONG Rgb;                         /* 0x8 */
} MODEL_COL;

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
    SHORT N_rot_y;                     /* 0x74 current neck turn (y) */
    SHORT N_rot_z;                     /* 0x76 current neck turn (z) */
    SHORT Add_dir_y;                   /* 0x78 turn speed (y) */
    SHORT Add_dir_z;                   /* 0x7A turn speed (z) */
    SHORT L_rot_y;                     /* 0x7C turn limit (y) */
    SHORT L_rot_z;                     /* 0x7E turn limit (z) */
    UCHAR Sce_add_dir_y;               /* 0x80 turn speed used while Neck_flg 0x80 (scenario) */
    UCHAR Sce_add_dir_z;               /* 0x81 */
} PARTS_NECK_FREE_WORK;
#define PW_NECK(p) ((PARTS_NECK_FREE_WORK *)&(p)->Unk74)

/* PARTS_W free work of a part thrown off the body (a severed head or arm flying away; PARTS_W.Be_flg 0x40)
 * (RE2 struct tagPARTS_BOMB_FREE_WORK, MAIN.H) */
typedef struct tagPARTS_BOMB_FREE_WORK {
    SHORT V0_x;                        /* 0x74 direction it flies off in (Cdir_y + 0x800: backwards) */
    SHORT V0_y;                        /* 0x76 initial vertical speed (negative: up) */
    SHORT V0_z;                        /* 0x78 initial horizontal speed */
    SHORT G;                           /* 0x7A gravity */
    USHORT Cnt0;                       /* 0x7C frame counter */
    SHORT Offset_y;                    /* 0x7E */
    SHORT Gxz;                         /* 0x80 horizontal drag */
} PARTS_BOMB_FREE_WORK;
#define PW_BOMB(p) ((PARTS_BOMB_FREE_WORK *)&(p)->Unk74)

/* PARTS_W free work of colour-faded parts (PARTS_W.Be_flg 0x80; RE2 struct tagPARTS_COLOR_CH_FREE_WORK
 * Color1/Color2) */
typedef struct tagPARTS_RGB_FREE_WORK {
    ULONG Rgb0;                        /* 0x74 */
    ULONG Rgb1;                        /* 0x78 */
} PARTS_RGB_FREE_WORK;
#define PW_RGB(p) ((PARTS_RGB_FREE_WORK *)&(p)->Unk74)

/* PARTS_W Rot_x/y/z (+ Rot_spd_z as pad) as an SVECTOR */
#define PW_ROT_SV(p) ((SVECTOR *)&(p)->Rot_x)

ULONG Joint_move_p(JOINT_P *pJp, ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, ULONG Late_flg);
ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top); /* PartsWork_set */
void Parts_work_pos_init(ENEMY_WORK *pEm, SHORT *pKan); /* part positions from kan data */

extern MODEL_COL Model_col; /* light matrix of the lit sorts */

void Joint_trans(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg); /* RE2 Joint_trans */
void ko_joint_trans(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG Be_flg, MATRIX *wm);
void Joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg, VECTOR *pPos);
void ko_joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM);
void ko_joint_trans3(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM);
void Parts_sort(ULONG *pObj, ULONG Flg, PARTS_W *pP);
void Parts_sort_sabun(ULONG *pObj, ULONG Flg, PARTS_W *pP);
void Parts_sort_ex(ULONG *pObj, ULONG Flg, PARTS_W *pP);
void Parts_sort_simple(ULONG *pObj, ULONG Flg, PARTS_W *pP);
ULONG PartsWork_link(ENEMY_WORK *pEm, ULONG work_top, ULONG *pKan);
void Em_tilt_set(ENEMY_WORK *pEm, SHORT Max, SHORT Spd, UCHAR Flg);
void Em_tilt_move(ENEMY_WORK *pEm);
void Em_tilt_return(ENEMY_WORK *pEm);
void HMatrix(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate);
void HMatrix1(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate);
ULONG Joint_move(ENEMY_WORK *pEm, ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, ULONG Late_flg);
ULONG J_seq(ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, USHORT *pSeq, ULONG Late_flg);
void hokan_svec(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, LONG Rate);
ULONG J_seq_hokan(ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, LONG No, ULONG Late_flg);
void Parts_down(PARTS_W *pP_ptr, MATRIX *wm);
void Parts_bomb(PARTS_W *pP_ptr);
ULONG Joint_move2(ENEMY_WORK *pEm, ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, ULONG Late_flg); /* RE2 JOINT.C Joint_move */
ULONG J_seq2(ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, USHORT *pSeq, ULONG Late_flg);
ULONG J_seq2_hokan(ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, LONG No, ULONG Late_flg);
void Parts_hokan2(ULONG n, PARTS_W *pP, ULONG Late, SHORT Rate);
void Om_joint_trans(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM);

#endif /* JOINT_H */
