#ifndef MD_SUB_H
#define MD_SUB_H
/* RE2 MD_SUB.C: RE3 model (MD) files, their relocation and the primitive sorters */
#include "bio3.h"
#include "em_sub.h"

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

/* 0x68 byte prim work (Rgb_blend) */
typedef struct tagRGB_BLEND_WORK {
    UCHAR pad0[0x10];
    ULONG Rgb0;                        /* 0x10 */
    UCHAR pad14[0x1C - 0x14];
    ULONG Unk1C;                       /* 0x1C */
    UCHAR pad20[0x28 - 0x20];
    ULONG Rgb1;                        /* 0x28 */
    UCHAR pad2C[0x68 - 0x2C];
} RGB_BLEND_WORK;

/* primitive sort parameters (Parts_sort_ex -> func_8002DC98 / func_8002DE0C) */
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

void Md_adr_cnv(MD_HEAD *pMd); /* relocate MD_OBJ offsets */
ULONG Mapping_md(ULONG *pMd_adr, ULONG Ctp, ULONG Ccl); /* RE2 Mapping_tmd counterpart */
void Md_set_abr(MD_OBJ *pObj, ULONG Abr); /* set semi-trans rate of all prims */

extern SHORT Bomb_tbl[2][30]; /* RE2 Bomb_tbl: push-out distance per frame */
extern SVECTOR D_80098B00; /* spin of the exploding parts, +0x80 per call */
extern ULONG D_80098B28[][4]; /* uv words per primitive code */
extern UCHAR D_80098B58[][2]; /* panel columns, rows */
extern UCHAR D_80098B8C[]; /* first row bitmask in D_80098BA8 */
extern USHORT D_80098BA8[]; /* one bit per panel, MSB first */
extern UCHAR D_80098D68[13][8];
extern MATRIX D_800D4550; /* light matrix saved by func_8002C824, restored by func_8002CABC */
extern MATRIX D_800D4570; /* rotation of D_80098B00, translation = polygon centre */

POLY_GT3 *PresetObjectGT3_neo(MD_WORK *pW, POLY_GT3 *pPrim);
POLY_GT4 *PresetObjectGT4_neo(MD_WORK *pW, POLY_GT4 *pPrim);
void MulLMatrix(MATRIX *lwm, MATRIX *klwm, MATRIX *nlm); /* libgte (not in libgte.h) */
void func_8002AF9C(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void Parts_hokan(ULONG n, PARTS_W *pP, ULONG Late, LONG Rate);
void Hokan_key_set(ULONG *pKan, ULONG *Seq_t_ptr, USHORT *pSeq, ULONG Late);
void func_8002B5FC(PARTS_W *pP, ULONG Cnt);
POLY_GT4 *func_8002B6FC(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002B86C(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *Bomb_Parts_SortGT3(MD_OBJ *pObj, POLY_GT4 *pPacket, PARTS_W *pP);
POLY_GT4 *Bomb_Parts_SortGT4(MD_OBJ *pObj, POLY_GT4 *pPacket, PARTS_W *pP);
void func_8002C680(MD_OBJ *pObj, ULONG Ctp, ULONG Ccl, ULONG V_ofs, ULONG Add);
POLY_GT4 *func_8002C824(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002CABC(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, SHORT *pSz);
POLY_GT4 *func_8002CE08(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002CF90(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void Md_rtp_floor(ULONG *pXy, USHORT *pZ, MD_OBJ *pObj, MATRIX *pM);
void Rgb_blend(RGB_BLEND_WORK *p, RGB_BLEND_WORK *pEnd, ULONG Data, ULONG Rgb);
void CompM(MATRIX *m0, MATRIX *m1, MATRIX *m2);
void Md_rtp(ULONG *pSxy, USHORT *pSz, MD_OBJ *pObj);
POLY_GT4 *func_8002D6D8(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002D898(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void func_8002DAAC(ULONG *Arg0, LONG Arg1, LONG Arg2, LONG Arg3); /* args from the asm */
POLY_GT4 *func_8002DC98(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002DE0C(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002DFE0(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *func_8002E160(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void func_8002E318(LONG Arg0, LONG Arg1, ULONG *Arg2, LONG Arg3); /* args from the asm */
POLY_GT4 *func_8002E61C(MD_WORK2 *pW, POLY_GT4 *pPrim);
void func_8002E74C(ENEMY_WORK *pEm, ULONG Abr);
void Draw_list_all(ENEMY_WORK *pFloor, ULONG W, ULONG H);
void func_8002EB9C(ENEMY_WORK *pFloor, ULONG W, ULONG H, ENEMY_WORK *pEm);

#endif /* MD_SUB_H */
