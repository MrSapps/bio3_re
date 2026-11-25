#ifndef SCA_H
#define SCA_H
/*
 * RE2 SCA.C: room collision (boxes, slopes, circles, ...). RE3 stores each
 * shape as two corners instead of X/Z/W/D. Layouts taken from the RE3 code.
 */
#include "main.h"
#include "em_sub.h"
#include "oba.h"

/* collision header (RE2 struct tagSCA_HEAD), followed by SCA_DATA[Num - 1] */
typedef struct tagSCA_HEAD {
    SHORT Num;                         /* 0x0 */
    SHORT pad2;
    SHORT Cx0;                         /* 0x4 area split point */
    SHORT Cz0;                         /* 0x6 */
    SHORT Cx1;                         /* 0x8 */
    SHORT Cz1;                         /* 0xA */
    SHORT padC;
    SHORT Ceiling;                     /* 0xE */
} SCA_HEAD;

/* collision shape (RE2 struct tagSCA_DATA, 0x10 bytes) */
typedef struct tagSCA_DATA {
    SHORT X0;                          /* 0x0 */
    SHORT Z0;                          /* 0x2 */
    SHORT X1;                          /* 0x4 */
    SHORT Z1;                          /* 0x6 */
    USHORT Id;                         /* 0x8 & 0xF: shape */
    USHORT Type;                       /* 0xA area bits, 0x1000 rotated 45 deg */
    signed char Floor_lo;              /* 0xC lowest floor (y = -1800 * floor) */
    signed char Floor_hi;              /* 0xD highest floor (Sca_search) */
    SHORT High;                        /* 0xE */
} SCA_DATA;

/* line / shape test work (0x800DEB58) */
typedef struct tagSCA_WORK {
    ULONG Pt0;                         /* 0x00 line end points, (x << 16) | (z & 0xFFFF) */
    ULONG Pt1;                         /* 0x04 */
    SHORT Unk8;                        /* 0x08 (Sca_set_ground) */
    SHORT padA;
    LONG X;                            /* 0x0C position rotated into a 45 degree shape's frame */
    LONG Z;                            /* 0x10 */
    LONG Old_x;                        /* 0x14 */
    LONG Old_z;                        /* 0x18 */
    UCHAR pad1C[0x24 - 0x1C];
    USHORT Unk24;                      /* 0x24 0x1000 when Sca_get_ground_obj finds a character under the point */
    UCHAR pad26[0x28 - 0x26];
    struct tagENEMY_WORK *pEm_hit;     /* 0x28 character hit by Sca_ck_line-like scans (Sca_ck_line_oba) */
    struct tagENEMY_WORK *pEm_hit2;    /* 0x2C same, for objects of type >= 3 */
} SCA_WORK;
extern SCA_WORK Sca_work;

extern LONG (*Sca_line_tbl[])(ULONG P0, ULONG P1, SCA_DATA *pT); /* line test per shape (Id & 0xF) */
extern SCA_DATA Tmp_Sca; /* RE2 Tmp_Sca: temporary shape passed to the hit routines */

void Sca_set_ground(ENEMY_WORK *pEm, struct tagCOL_OBJ *pObj, LONG Y);
ULONG Sca_hit_line(COL_OBJ *pObj, LONG R, ULONG Arg2);
ULONG Sca_hit_hishi(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_hit_circle(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_hit_koban_x(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_hit_koban_z(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_hit_box2(COL_OBJ *pObj, LONG R, SCA_DATA *pT);
ULONG Sca_hit_box(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_hosei_box(COL_OBJ *pObj, LONG Dx, LONG Dz, SCA_DATA *pT);
ULONG Sca_hit_slope(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_hit_naname(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_ck_circle(VECTOR *pPos, LONG R, SCA_DATA *pT);
LONG Sca_hit_obj_circle(COL_OBJ *pObj, LONG W, SHORT X, SHORT Z, SHORT R, ULONG Old);
ULONG Sca_ck_pass_line(COL_OBJ *pObj, LONG R, ULONG P0);
LONG Sca_hit_dummy(void);
ULONG Sca_get_area(LONG X, LONG Z); /* area bits of a point */
LONG Sca_get_ground(LONG *pPos, ULONG a1, ULONG a2, ULONG a3); /* ground height */
LONG Sca_get_ground_obj(VECTOR *pPos, LONG R, ULONG Att, ULONG Flg, LONG Ground);
ULONG Sca_get_block(VECTOR *pPos, LONG Arg1, UCHAR *pTbl, UCHAR *pOut);
ULONG Sca_ck_circle2(VECTOR *pPos, LONG R, SCA_DATA *pT);
LONG Sca_high_dummy(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_box(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_circle(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_naname(SCA_DATA *pT, VECTOR *pPos, LONG Unused, LONG High);
LONG Sca_high_naname_a(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_b(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_c(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_d(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_slope(SCA_DATA *pT, VECTOR *pPos, LONG High, LONG Unused);
LONG Sca_high_stairs(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_koban_x(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_koban_z(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_hishi(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_cone(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);
LONG Sca_ck_line_oba(VECTOR *pA, VECTOR *pB, ULONG Att, ULONG Flg);
LONG Sca_ck_cross(ULONG P0, ULONG P1, ULONG P2, ULONG P3); /* do segments P0-P1 and P2-P3 cross (GTE nclip) */
LONG Sca_line_dummy(void);
LONG Sca_line_diag(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG Sca_line_circle(ULONG P0, ULONG P1, struct tagSCA_DATA *pT);
LONG Sca_line_wall(ULONG P0, ULONG P1, SCA_DATA *pT); /* segment P0-P1 crosses the shape */
LONG Sca_line_box(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG Sca_line_koban_x(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG Sca_line_koban_z(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG Sca_line_hishi(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG Sca_line_naname(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG Sca_line_xy(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
LONG Sca_line_zy(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
LONG Sca_line_xy_box(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
LONG Sca_line_zy_box(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
void Sca_search(ULONG Type, ULONG Id, ULONG Floor);
USHORT Sca_get_dir(VECTOR *pPos, SCA_DATA *pT); /* RE2 Sca_get_dir: 2 floor/corner, else wall side */
USHORT Sca_get_quadrant(VECTOR *pPos, SCA_DATA *pT); /* quadrant of the shape the point is in */
LONG Sca_ck_wall_range(COL_OBJ *pObj, SCA_DATA *pT);

#endif /* SCA_H */
