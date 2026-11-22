#ifndef MD_SUB_H
#define MD_SUB_H
/* RE2 MD_SUB.C: RE3 model (MD) files, their relocation and the primitive sorters */
#include "bio3.h"

/* RE3 model object entry (0x18 bytes; RE2 Mapping_tmd works on TMD objects instead).
 * The pointer fields hold file offsets until Md_adr_cnv relocates them. */
typedef struct tagMD_OBJ {
    ULONG *pVert;                      /* 0x00 */
    ULONG *pNorm;                      /* 0x04 */
    ULONG n_vert;                      /* 0x08 low half count, high half (count + 2) / 3 */
    ULONG *pPrim3;                     /* 0x0C 0xC byte entries */
    ULONG *pPrim4;                     /* 0x10 0x10 byte entries */
    USHORT n_prim3;                    /* 0x14 */
    USHORT n_prim4;                    /* 0x16 */
} MD_OBJ;

typedef struct tagMD_HEAD {
    ULONG n_obj;                       /* 0x00 */
    MD_OBJ Obj[1];                     /* 0x04 */
} MD_HEAD;

/* model work (pW[1] = MD_OBJ, then the double-buffered prims built by PresetObjectGT3_neo / PresetObjectGT4_neo) */
typedef struct tagMD_WORK {
    ULONG Attr;                        /* 0x00 bits 2-3: semi-trans rate */
    MD_OBJ *pObj;                      /* 0x04 */
    POLY_GT3 *pPrim3;                  /* 0x08 */
    POLY_GT4 *pPrim4;                  /* 0x0C */
} MD_WORK;

/* second model work layout (func_8002E61C) */
typedef struct tagMD_WORK2 {
    UCHAR pad0[0x10];
    ULONG Attr;                        /* 0x010 bits 2-3: semi-trans rate */
    MD_OBJ *pObj;                      /* 0x014 */
    UCHAR pad18[0x188 - 0x18];
    POLY_GT4 *pPrim4;                  /* 0x188 */
} MD_WORK2;

/* 0x68 byte prim work (func_8002D41C) */
typedef struct tagRGB_BLEND_WORK {
    UCHAR pad0[0x10];
    ULONG Rgb0;                        /* 0x10 */
    UCHAR pad14[0x1C - 0x14];
    ULONG Unk1C;                       /* 0x1C */
    UCHAR pad20[0x28 - 0x20];
    ULONG Rgb1;                        /* 0x28 */
    UCHAR pad2C[0x68 - 0x2C];
} RGB_BLEND_WORK;

/* primitive sort parameters (func_80025FB0 -> func_8002DC98 / func_8002DE0C) */
typedef struct tagSORT_PARAM {
    MD_OBJ *pObj;                      /* 0x0 */
    union {
        ULONG Rgb;                     /* 0x4 colour, code in the top byte */
        struct {
            UCHAR pad4[3];
            UCHAR Code;                /* 0x7 primitive code (0x34 GT3, 0x3C GT4) */
        } c;
    } u;
    ULONG *pOt;                        /* 0x8 */
    ULONG UnkC;                        /* 0xC */
    ULONG Unk10;                       /* 0x10 */
    ULONG Unk14;                       /* 0x14 */
} SORT_PARAM;

void Md_adr_cnv(MD_HEAD *pMd);      /* relocate MD_OBJ offsets */
ULONG func_8002AB34(ULONG *pMd_adr, ULONG Ctp, ULONG Ccl); /* RE2 Mapping_tmd counterpart */
void Md_set_abr(MD_OBJ *pObj, ULONG Abr);           /* set semi-trans rate of all prims */

#endif /* MD_SUB_H */
