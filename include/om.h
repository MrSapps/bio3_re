#ifndef OM_H
#define OM_H
#include "bio3.h"
#include "md_sub.h"

/* per object-model type (ENEMY_WORK.Type & 0x1F) */
typedef struct {
    UCHAR Unk0;                        /* 0x0 non-zero: part Unk1 starts hidden (func_8003589C) */
    UCHAR Unk1;                        /* 0x1 */
    UCHAR Num;                         /* 0x2 */
    UCHAR Idx;                         /* 0x3 */
} OM_BREAK_TBL;

/* OBJ_MODEL_WORK (G.Ob_model[]) is in bio3.h */

/* Colour / texture animation part (RE2 struct tagCC_PARTS_WORK; RE3 adds a flag word
 * and 0x40 unknown bytes, 0x94 bytes) */
typedef struct tagCC_PARTS_WORK {
    ULONG Cc_flg;                      /* 0x00 0x20 colour change, 0x40 texture change */
    UCHAR Col_ctr;                     /* 0x04 colour-change Cc_ctr (Col_chg) */
    UCHAR Col_cnt;                     /* 0x05 */
    UCHAR Col_wait;                    /* 0x06 */
    UCHAR Col_num;                     /* 0x07 */
    UCHAR Cc_ctr;                      /* 0x08 */
    UCHAR Cc_cnt;                      /* 0x09 */
    UCHAR Cc_wait;                     /* 0x0A */
    UCHAR Cc_num;                      /* 0x0B */
    USHORT Cc_pos_x;                   /* 0x0C */
    USHORT Cc_pos_y;                   /* 0x0E */
    DR_MOVE Col_dr_move[2];            /* 0x10 */
    RECT Col_dr_rect[2];               /* 0x40 */
    DR_MOVE Cc_dr_move[2];             /* 0x50 */
    RECT Cc_dr_rect[2];                /* 0x80 */
    OBJ_MODEL_WORK *pOm;               /* 0x90 */
} CC_PARTS_WORK;

typedef struct tagCC_WORK {
    CC_PARTS_WORK Cc_parts[30];        /* 0x0000 */
    USHORT Ccol_old;                   /* 0x1158 */
    UCHAR Ccol_no;                     /* 0x115A */
    UCHAR Ctex_old;                    /* 0x115B */
} CC_WORK;
extern CC_WORK Cc_work;                /* 0x800DD4B8 */

void Tex_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);
ULONG Tex_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr);
void Om_chg_init_80036234(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);
void Col_chg_ctr_80036248(CC_PARTS_WORK *pCcp);
void Tex_ctr(void);
void Om_trans(void);
void func_80037C38(void); /* set the OT pointer of every enemy and object model */

extern OM_BREAK_TBL D_8009CBEC[];
extern struct tagHIT_ATTR *D_8009CC54[]; /* attack data per object type */
extern void (*D_8009CC64[])(OBJ_MODEL_WORK *pOm);
extern UCHAR D_800CC84B;
extern ULONG D_800D20D4[];
extern SHORT D_800DD492;
extern SHORT D_800DD494;
extern SHORT D_800DD496;
extern SHORT D_800DD498;
extern SHORT D_800DD49A;
extern SHORT D_800DD49C;
extern SVECTOR D_800DD4A0;
extern UCHAR D_800DD4A8;
extern LONG D_800DD4AC; /* number of pushable object models touched this frame */
extern OBJ_MODEL_WORK *D_800DD4B0; /* the one on the player's floor */
extern SHORT D_800DE614;
extern SHORT D_800DE618;
extern SHORT D_800DE61A;
extern SHORT D_800DE61E;

void func_80035790(ENEMY_WORK *pEm, ULONG *pData);
PARTS_W *func_80035868(ENEMY_WORK *pEm, PARTS_W *pParts);
void func_8003589C(OBJ_MODEL_WORK *pOm, ULONG *pKan);
void func_80035B84(OBJ_MODEL_WORK *pOm, PARTS_W *pParts, ULONG Flg, LONG *pPos);
void Col_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);
void Col_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr);
void Om_move(void);
void func_80036570(void);
void Em_routine0_set1_80036640(OBJ_MODEL_WORK *pOm);
void Om_set_fall_dir(ENEMY_WORK *pEm);
void func_800366EC(OBJ_MODEL_WORK *pOm, ULONG Arg1, ULONG Arg2);
void func_80036C60(OBJ_MODEL_WORK *pOm);
void func_80036D94(OBJ_MODEL_WORK *pOm);
void func_80036F78(OBJ_MODEL_WORK *pOm);
void func_8003749C(OBJ_MODEL_WORK *pOm);
void func_80037680(OBJ_MODEL_WORK *pOm);
void func_8003783C(OBJ_MODEL_WORK *pOm);
void func_80037A90(ENEMY_WORK *pEm, ULONG Arg1);
ULONG *func_80037D50(LONG *pPos); /* ordering table for a position */
void func_80037E80(void);

#endif /* OM_H */
