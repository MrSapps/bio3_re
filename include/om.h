#ifndef OM_H
#define OM_H
#include "bio3.h"
#include "md_sub.h"

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
void Tex_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr);
void Om_chg_init_80036234(ULONG Col_wk_no, CC_PARTS_WORK *pCcp);
void Col_chg_ctr_80036248(CC_PARTS_WORK *pCcp);
void Tex_ctr(void);
void Om_trans(void);
void func_80037C38(void);                /* set the OT pointer of every enemy and object model */

#endif /* OM_H */
