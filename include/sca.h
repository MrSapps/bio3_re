#ifndef SCA_H
#define SCA_H
/*
 * RE2 SCA.C: room collision (boxes, slopes, circles, ...). RE3 stores each
 * shape as two corners instead of X/Z/W/D. Layouts taken from the RE3 code.
 */
#include "main.h"

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

#endif /* SCA_H */
