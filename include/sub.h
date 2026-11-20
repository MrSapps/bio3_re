#ifndef SUB_H
#define SUB_H
#include "bio3.h"
#include "main.h"

/* RE2 SUB.H: one camera cut's lighting (same layout as RE2) */
typedef struct tagLIGHT_CUT_DATA {
    UCHAR Mag;                         /* 0x00 */
    UCHAR Mode[3];                     /* 0x01 */
    UCHAR Col[3][3];                   /* 0x04 */
    UCHAR Ambient[3];                  /* 0x0D */
    SHORT Pos[3][3];                   /* 0x10 */
    USHORT L[3];                       /* 0x22 */
} LIGHT_CUT_DATA;                      /* size 0x28 */

/* RE3 light selection zone: G.pRoom->pLight[1 + cut] = (offset << 16) | count of
 * these 0xC byte entries; the last entry of a cut is the default (zone not checked) */
typedef struct tagLIGHT_ZONE {
    USHORT No;                         /* 0x00 LIGHT_CUT_DATA index */
    USHORT pad2;
    SHORT Xz[2];                       /* 0x04 */
    USHORT W;                          /* 0x08 */
    USHORT D;                          /* 0x0A */
} LIGHT_ZONE;

/* libgs / GTE globals */
extern MATRIX GsIDMATRIX;              /* 0x80098970 */
extern MATRIX GsWSMATRIX;              /* 0x80098990 */
extern MATRIX LIGHTMATRIX;             /* 0x80098950 */
extern MATRIX COLORMATRIX;             /* 0x80098930 */

/* RE2 SUB.C */
extern ULONG *pStack_bak;              /* 0x800E11F8 */
extern MATRIX Tmp_matrix[2];           /* 0x800E1190 */
extern SVECTOR Rot_vector_Svec;        /* 0x800A11C0, static in RE2 Rot_vector */
extern SVECTOR Rot_vector_super_Svec;  /* 0x800A11C8, static in RE2 Rot_vector_super */

LONG func_800101C8(LONG *pPos, SHORT *pXz); /* sld.s: point in zone */
LIGHT_CUT_DATA *Get_light_cut(LONG *pPos);
void Set_light(LIGHT_CUT_DATA *pLightCut, VECTOR *pV);

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

void Md_adr_cnv(MD_HEAD *pMd);      /* relocate MD_OBJ offsets */
ULONG func_8002AB34(ULONG *pMd_adr, ULONG Ctp, ULONG Ccl); /* RE2 Mapping_tmd counterpart */
void Md_set_abr(MD_OBJ *pObj, ULONG Abr);           /* set semi-trans rate of all prims */
ULONG Flg_ck(ULONG *pFlg, ULONG Id);                 /* bit Id of a flag array */

#endif /* SUB_H */
