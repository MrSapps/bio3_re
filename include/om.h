#ifndef OM_H
#define OM_H
#include "bio3.h"
#include "md_sub.h"

/* per object-model type (ENEMY_WORK.Type & 0x1F) */
typedef struct {
    UCHAR Unk0;                        /* 0x0 non-zero: part Unk1 starts hidden (Om_parts_link) */
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
void Uv_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);
void Uv_chg_ctr(CC_PARTS_WORK *pCcp);
void Tex_ctr(void);
void Om_trans(void);
void Ot_set_all(void); /* set the OT pointer of every enemy and object model */

extern OM_BREAK_TBL Om_bomb_parts[];
extern struct tagHIT_ATTR *Om_at_tbl[]; /* attack data per object type */
extern void (*Om_routine[])(OBJ_MODEL_WORK *pOm);
extern UCHAR D_800CC84B;
extern ULONG D_800D20D4[];
extern SHORT Om_pos_spd;
extern SHORT Om_pos_spd_add;
extern SHORT Om_dir_spd;
extern SHORT Om_dir_spd_add;
extern SHORT Om_fall_spd;
extern SHORT Om_dir_sum;
extern SVECTOR Om_add_dir;
extern UCHAR Om_in_water_flg;
extern LONG Om_push_num; /* number of pushable object models touched this frame */
extern OBJ_MODEL_WORK *pOm_push; /* the one on the player's floor */
extern SHORT Om_fall_cdir_x;
extern SHORT Om_fall_cdir_z;
extern SHORT Om_fall_dir_x;
extern SHORT Om_fall_dir_z;

void Om_init(ENEMY_WORK *pEm, ULONG *pData);
PARTS_W *Om_parts_work_set(ENEMY_WORK *pEm, PARTS_W *pParts);
void Om_parts_link(OBJ_MODEL_WORK *pOm, ULONG *pKan);
void Om_joint_trans2(OBJ_MODEL_WORK *pOm, PARTS_W *pParts, ULONG Flg, LONG *pPos);
void Col_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);
void Col_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr);
void Om_move(void);
void Om_push_ck(void);
void Om_move_start(OBJ_MODEL_WORK *pOm);
void Om_set_fall_dir(ENEMY_WORK *pEm);
void Om_explosion(OBJ_MODEL_WORK *pOm, ULONG Arg1, ULONG Arg2);
void Om_rtn_wait(OBJ_MODEL_WORK *pOm);
void Om_rtn_drop(OBJ_MODEL_WORK *pOm);
void Om_rtn_fall(OBJ_MODEL_WORK *pOm);
void Om_move_box(OBJ_MODEL_WORK *pOm);
void Om_explosion_set(OBJ_MODEL_WORK *pOm);
void Om_rtn_target_off(OBJ_MODEL_WORK *pOm);
void Om_drop_set(ENEMY_WORK *pEm, ULONG Arg1);
ULONG *Ot_get_pos(LONG *pPos); /* ordering table for a position */
void Ot_sub_init(void);

#endif /* OM_H */
