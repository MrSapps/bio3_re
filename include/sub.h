#ifndef SUB_H
#define SUB_H
#include "bio3.h"
#include "main.h"
#include "em_sub.h"

typedef struct {
    UCHAR Unk0;
    UCHAR Unk1;
    USHORT Unk2;
    LONG Unk4;
    LONG Unk8[3];
    UCHAR Unk14[4];
    LONG Unk18[3];
} FI_SAVE;

/* two-bone joint lengths (func_80079374) */
typedef struct tagMY_JNT {
    SVECTOR a;
    SVECTOR b;
    SHORT c[2];
} MY_JNT;

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
extern MATRIX GsWSMATRIX;              /* 0x80098990 */
extern MATRIX LIGHTMATRIX;             /* 0x80098950 */
extern MATRIX COLORMATRIX;             /* 0x80098930 */

/* RE2 SUB.C */
extern ULONG *pStack_bak;              /* 0x800E11F8 */
extern MATRIX Tmp_matrix[2];           /* 0x800E1190 */
extern SVECTOR Rot_vector_Svec;        /* 0x800A11C0, static in RE2 Rot_vector */
extern SVECTOR Rot_vector_super_Svec;  /* 0x800A11C8, static in RE2 Rot_vector_super */

LIGHT_CUT_DATA *Get_light_cut(LONG *pPos);
void Set_light(LIGHT_CUT_DATA *pLightCut, VECTOR *pV);
void Set_Light_data(ULONG No, VECTOR *pLig, UCHAR *pRgb, LIGHT_CUT_DATA *pLight);

/* player state kept by func_800791F0 (0x800C7048) */
typedef struct tagPL_SAVE {
    USHORT Cdir_y;                     /* 0x0 */
    SHORT Pos_x;                       /* 0x2 */
    SHORT Pos_y;                       /* 0x4 */
    SHORT Pos_z;                       /* 0x6 */
    USHORT Life;                       /* 0x8 */
    UCHAR Pl_id;                       /* 0xA */
    UCHAR Poison;                      /* 0xB 0x80 | poison timer */
    UCHAR Save[0x908];                 /* 0xC copy of G.Card_head.. */
} PL_SAVE;
extern PL_SAVE D_800C7048;

ULONG Flg_ck(ULONG *pFlg, ULONG Id); /* bit Id of a flag array */
void Flg_on(ULONG *pFlg, ULONG Id); /* set bit Id of a flag array */

void func_800790F0(ULONG *pAdr); /* load and run the INIT_SUB code at pAdr */
void func_800791F0(void); /* restore the saved game state (load) */
void func_800792FC(void); /* read INIT_SUB to 0x80101000 */

extern FI_SAVE D_800E11D0;
extern LONG D_800E11F4;
extern MATRIX D_800E1200[];
extern MATRIX D_800E1220;
extern MATRIX D_800E1260;
extern MATRIX D_800E1280;

void Mapping_tim(ULONG *pAdr); /* RE2 Mapping_tim */
void SetSpadStack(void);
void ResetSpadStack(void);
void Vram_clr(USHORT x, USHORT y, USHORT w, USHORT h);
void Rot_vector(SHORT dir, SVECTOR *p, SVECTOR *q); /* RE2 Rot_vector */
void Rot_vector_super(SHORT dir, MATRIX *pM, SVECTOR *p, SVECTOR *q);
void Rot_add_matrix(LONG *pPos, SHORT *pDir, MATRIX *pM, SVECTOR *Add_dir);
void Flg_off(ULONG *p, ULONG x);
void Set_view(LONG *pVp, LONG *pVr);
void Set_light_pos(VECTOR *pV);
void Set_light_default(void);
void func_80078FE8(void);
void func_8007906C(void);
void func_80079374(MATRIX *pM0, MATRIX *pM1, LONG *pT, MY_JNT *pJ);
void Copy_rot_matrix(MATRIX *m0, MATRIX *m1);
void Copy_matrix(MATRIX *m0, MATRIX *m1);
LONG func_80079690(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, LONG rate);
void func_80079C70(UCHAR *p);
void func_80079CC0(ENEMY_WORK *pEm, LONG h);
void func_80079EB8(void);

#endif /* SUB_H */
