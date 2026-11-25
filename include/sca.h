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
    SHORT Unk8;                        /* 0x08 (func_8004B7E8) */
    SHORT padA;
    LONG X;                            /* 0x0C position rotated into a 45 degree shape's frame */
    LONG Z;                            /* 0x10 */
    LONG Old_x;                        /* 0x14 */
    LONG Old_z;                        /* 0x18 */
    UCHAR pad1C[0x28 - 0x1C];
    struct tagENEMY_WORK *pEm_hit;     /* 0x28 character hit by Sca_ck_line-like scans (func_8004ED4C) */
    struct tagENEMY_WORK *pEm_hit2;    /* 0x2C same, for objects of type >= 3 */
} SCA_WORK;
extern SCA_WORK D_800DEB58;

extern LONG (*D_8009E088[])(ULONG P0, ULONG P1, SCA_DATA *pT); /* line test per shape (Id & 0xF) */
extern SCA_DATA Tmp_Sca; /* RE2 Tmp_Sca: temporary shape passed to the hit routines */

void func_8004B7E8(ENEMY_WORK *pEm, struct tagCOL_OBJ *pObj, LONG Y);
ULONG func_8004B874(COL_OBJ *pObj, LONG R, ULONG Arg2);
ULONG func_8004BB4C(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG func_8004C408(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG func_8004C57C(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG func_8004C6EC(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG func_8004C85C(COL_OBJ *pObj, LONG R, SCA_DATA *pT);
ULONG func_8004C960(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG func_8004CC54(COL_OBJ *pObj, LONG Dx, LONG Dz, SCA_DATA *pT);
ULONG func_8004CE2C(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG func_8004D194(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3);
ULONG Sca_ck_circle(VECTOR *pPos, LONG R, SCA_DATA *pT);
LONG func_8004D404(COL_OBJ *pObj, LONG W, SHORT X, SHORT Z, SHORT R, ULONG Old);
ULONG func_8004D590(COL_OBJ *pObj, LONG R, ULONG P0);
LONG func_8004D6B0(void);
ULONG func_8004D6B8(LONG X, LONG Z); /* area bits of a point */
LONG Sca_get_ground(LONG *pPos, ULONG a1, ULONG a2, ULONG a3); /* ground height */
void func_8004DB10(LONG Arg0, LONG Arg1, LONG Arg2, LONG Arg3); /* args from the asm */
ULONG func_8004DDC4(VECTOR *pPos, LONG Arg1, UCHAR *pTbl, UCHAR *pOut);
ULONG Sca_ck_circle2(VECTOR *pPos, LONG R, SCA_DATA *pT);
LONG func_8004DEA8(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_box(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_high_circle(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG func_8004DF24(SCA_DATA *pT, VECTOR *pPos, LONG Unused, LONG High);
LONG Sca_high_naname_a(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_b(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_c(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_d(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG func_8004E10C(SCA_DATA *pT, VECTOR *pPos, LONG High, LONG Unused);
LONG func_8004E230(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG func_8004E38C(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG func_8004E4C4(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG func_8004E5FC(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG func_8004E750(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);
LONG func_8004ED4C(VECTOR *pA, VECTOR *pB, ULONG Att, ULONG Flg);
LONG func_8004EF74(ULONG P0, ULONG P1, ULONG P2, ULONG P3); /* do segments P0-P1 and P2-P3 cross (GTE nclip) */
LONG func_8004F02C(void);
LONG func_8004F034(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG func_8004F098(ULONG P0, ULONG P1, struct tagSCA_DATA *pT);
LONG func_8004F258(ULONG P0, ULONG P1, SCA_DATA *pT); /* segment P0-P1 crosses the shape */
LONG func_8004F3C8(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG func_8004F498(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG func_8004F5B4(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG func_8004F6CC(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG func_8004F7A8(ULONG P0, ULONG P1, SCA_DATA *pT);
LONG func_8004F928(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
LONG func_8004FBD4(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
LONG func_8004FE70(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
LONG func_8005003C(VECTOR *pA, VECTOR *pB, SCA_DATA *pT);
void Sca_search(ULONG Type, ULONG Id, ULONG Floor);
USHORT func_800502B8(VECTOR *pPos, SCA_DATA *pT); /* RE2 Sca_get_dir: 2 floor/corner, else wall side */
LONG func_800503E8(LONG Arg0, SCA_DATA *pT); /* args from the asm */
LONG func_80050524(COL_OBJ *pObj, SCA_DATA *pT);

#endif /* SCA_H */
