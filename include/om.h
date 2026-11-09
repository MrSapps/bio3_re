#ifndef OM_H
#define OM_H
#include "bio3.h"

/* Object model work (RE2 struct tagOBJ_MODEL_WORK, OMA.H). Only proven fields. */
typedef struct tagOBJ_MODEL_WORK {
    UCHAR pad0[0x48];
    UCHAR Tpage;                       /* 0x48 */
} OBJ_MODEL_WORK;

/* Colour / texture animation part (RE2 struct tagCC_PARTS_WORK; RE3 adds a flag word
 * and 0x40 unknown bytes, 0x94 bytes) */
typedef struct tagCC_PARTS_WORK {
    ULONG Cc_flg;                      /* 0x00 0x20 colour change, 0x40 texture change */
    UCHAR pad4[4];
    UCHAR Cc_ctr;                      /* 0x08 */
    UCHAR Cc_cnt;                      /* 0x09 */
    UCHAR Cc_wait;                     /* 0x0A */
    UCHAR Cc_num;                      /* 0x0B */
    USHORT Cc_pos_x;                   /* 0x0C */
    USHORT Cc_pos_y;                   /* 0x0E */
    UCHAR pad10[0x50 - 0x10];
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
void func_80036248(CC_PARTS_WORK *pCcp);
void Tex_ctr(void);

#endif /* OM_H */
