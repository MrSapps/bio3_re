#ifndef SUB_H
#define SUB_H
#include "bio3.h"
#include "main.h"
#include "em_sub.h"

/* G fields kept across Init_global when System_flg 0x80 (retry) is set (Keep_data_save/Keep_data_load) */
typedef struct tagKEEP_DATA {
    UCHAR Save_cnt;                    /* 0x00 G.Save_cnt */
    UCHAR Save_slot;                   /* 0x01 G.Save_slot */
    USHORT G_7806;                     /* 0x02 G.Unk7806 */
    LONG Game_flg;                     /* 0x04 G.Unk77F8[0] (game mode / unlock flags) */
    LONG Merc_time[3];                 /* 0x08 G.Merc_time[] */
    UCHAR Merc_grade[4];               /* 0x14 G.Merc_grade[] */
    LONG Merc_money[3];                /* 0x18 G.Merc_money[] */
} KEEP_DATA;

/* two-bone joint lengths (Leg_ik) */
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

/* player state kept by Retry_load (0x800C7048) */
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
extern PL_SAVE Retry_data;

ULONG Flg_ck(ULONG *pFlg, ULONG Id); /* bit Id of a flag array */
void Flg_on(ULONG *pFlg, ULONG Id); /* set bit Id of a flag array */

void Retry_file_load(ULONG *pAdr); /* load and run the INIT_SUB code at pAdr */
void Retry_load(void); /* restore the saved game state (load) */
void Init_sub_load(void); /* read INIT_SUB to 0x80101000 */

extern KEEP_DATA Keep_data;
extern LONG Keep_money;
extern MATRIX Ik_mat[];
extern MATRIX Ik_mat1;
extern MATRIX Ik_mat3;
extern MATRIX Ik_mat4;

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
void Keep_data_save(void);
void Keep_data_load(void);
void Leg_ik(MATRIX *pM0, MATRIX *pM1, LONG *pT, MY_JNT *pJ);
void Copy_rot_matrix(MATRIX *m0, MATRIX *m1);
void Copy_matrix(MATRIX *m0, MATRIX *m1);
LONG Leg_ik_kan(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void Leg_ik_set(ENEMY_WORK *pEm, LONG rate);
void Sub_matrix_mul(UCHAR *p);
void Leg_ik_floor(ENEMY_WORK *pEm, LONG h);
void Sub_dummy(void);

#endif /* SUB_H */
