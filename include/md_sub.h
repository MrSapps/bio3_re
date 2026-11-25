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
    ULONG n_prim;                      /* 0x14 low half GT3 count, high half GT4 count (one word, like n_vert) */
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

/* second model work layout (PresetObjectGT4_mizu) */
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

/* primitive sort parameters (Parts_sort_ex -> SortGT3_col / SortGT4_col) */
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
extern SVECTOR Bomb_rot; /* spin of the exploding parts, +0x80 per call */
extern ULONG Plus_uv_tbl[][4]; /* uv words per primitive code */
extern UCHAR Mizu_size_tbl[][2]; /* panel columns, rows */
extern UCHAR Mizu_tbl[]; /* first row bitmask in Mizu_struct_tbl */
extern USHORT Mizu_struct_tbl[]; /* one bit per panel, MSB first */
extern UCHAR Div_prim_coord[13][8];
extern MATRIX Plus_lm_save; /* light matrix saved by SortGT3_plus_light, restored by SortGT4_plus_light */
extern MATRIX Bomb_m; /* rotation of Bomb_rot, translation = polygon centre */

POLY_GT3 *PresetObjectGT3_neo(MD_WORK *pW, POLY_GT3 *pPrim);
POLY_GT4 *PresetObjectGT4_neo(MD_WORK *pW, POLY_GT4 *pPrim);
void MulLMatrix(MATRIX *lwm, MATRIX *klwm, MATRIX *nlm); /* libgte (not in libgte.h) */
void SortGT4_lno(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void Parts_hokan(ULONG n, PARTS_W *pP, ULONG Late, LONG Rate);
void Hokan_key_set(ULONG *pKan, ULONG *Seq_t_ptr, USHORT *pSeq, ULONG Late);
void Hokan_spd_set(PARTS_W *pP, ULONG Cnt);
POLY_GT4 *SortGT3_lno_uv(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *SortGT4_lno_uv(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *Bomb_Parts_SortGT3(MD_OBJ *pObj, POLY_GT4 *pPacket, PARTS_W *pP);
POLY_GT4 *Bomb_Parts_SortGT4(MD_OBJ *pObj, POLY_GT4 *pPacket, PARTS_W *pP);
void Md_tex_ofs(MD_OBJ *pObj, ULONG Ctp, ULONG Ccl, ULONG V_ofs, ULONG Add);
POLY_GT4 *SortGT3_plus_light(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *SortGT4_plus_light(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, SHORT *pSz);
POLY_GT4 *SortGT3_plus_col(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *SortGT4_plus_col(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void Md_rtp_floor(ULONG *pXy, USHORT *pZ, MD_OBJ *pObj, MATRIX *pM);
void Rgb_blend(RGB_BLEND_WORK *p, RGB_BLEND_WORK *pEnd, ULONG Data, ULONG Rgb);
void CompM(MATRIX *m0, MATRIX *m1, MATRIX *m2);
void Md_rtp(ULONG *pSxy, USHORT *pSz, MD_OBJ *pObj);
POLY_GT4 *SortGT3_neo_uv(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *SortGT4_neo_uv(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void SortGT3_neo(ULONG *Arg0, LONG Arg1, LONG Arg2, LONG Arg3); /* args from the asm */
POLY_GT4 *SortGT3_col(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *SortGT4_col(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *SortGT3_sabun(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
POLY_GT4 *SortGT4_sabun(SORT_PARAM *pPrm, POLY_GT4 *pPacket, ULONG *pSxy, USHORT *pSz);
void Skin_vtx0_old(LONG Arg0, LONG Arg1, ULONG *Arg2, LONG Arg3); /* args from the asm */
POLY_GT4 *PresetObjectGT4_mizu(MD_WORK2 *pW, POLY_GT4 *pPrim);
void Mizu_trans(ENEMY_WORK *pEm, ULONG Abr);
void Draw_list_all(ENEMY_WORK *pFloor, ULONG W, ULONG H);
void Mizu_div_main(ENEMY_WORK *pFloor, ULONG W, ULONG H, ENEMY_WORK *pEm);

#endif /* MD_SUB_H */
