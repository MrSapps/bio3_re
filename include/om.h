#ifndef OM_H
#define OM_H
#include "bio3.h"

/* Object model work (RE2 struct tagOBJ_MODEL_WORK, OMA.H). Only proven fields. */
typedef struct tagOM_TMD_HEAD {        /* model header at OBJ_MODEL_WORK.pTmd_head (Col_chg_init) */
    UCHAR pad0[0xC];
    ULONG *pPacket;                    /* 0x0C GT3-like packets, 3 words each */
    ULONG *pPacket2;                   /* 0x10 GT4-like packets, 4 words each */
    USHORT N_poly;                     /* 0x14 */
    USHORT N_poly2;                    /* 0x16 */
} OM_TMD_HEAD;

typedef struct tagOBJ_MODEL_WORK {
    ULONG Be_flg;                      /* 0x00 */
    UCHAR Routine_0;                   /* 0x04 */
    UCHAR Routine_1;                   /* 0x05 */
    UCHAR Routine_2;                   /* 0x06 */
    UCHAR Routine_3;                   /* 0x07 */
    UCHAR Parts_num;                   /* 0x08 */
    signed char nFloor;                /* 0x09 */
    UCHAR padA[0x14 - 0xA];
    OM_TMD_HEAD *pTmd_head;            /* 0x14 */
    UCHAR pad18[0x20 - 0x18];
    MATRIX M;                          /* 0x20 M.t = Pos_x/y/z */
    SHORT Old_x;                       /* 0x40 */
    SHORT Old_y;                       /* 0x42 */
    SHORT Old_z;                       /* 0x44 */
    USHORT Type;                       /* 0x46 */
    UCHAR Tpage;                       /* 0x48 */
    UCHAR Unk49;                       /* 0x49 */
    UCHAR Unk4A;                       /* 0x4A Id; 1: packets at pPacket188 */
    UCHAR pad4B;
    MATRIX Workm;                      /* 0x4C */
    SVECTOR Cdir;                      /* 0x6C */
    ULONG Poly_rgb;                    /* 0x74 */
    MATRIX *pSuper;                    /* 0x78 */
    UCHAR pad7C[0xAE - 0x7C];
    USHORT UnkAE;                      /* 0xAE 0x10: pushable, 0x20: keeps being pushed (func_80036570) */
    UCHAR padB0[0xBA - 0xB0];
    USHORT UnkBA;                      /* 0xBA */
    UCHAR padBC[0xC0 - 0xBC];
    UCHAR Push_cnt;                    /* 0xC0 frames pushed (func_80036570) */
    UCHAR UnkC1;                       /* 0xC1 index into G.pUnk75AC */
    UCHAR UnkC2;                       /* 0xC2 flag id set when done (func_80036C60) */
    UCHAR UnkC3;                       /* 0xC3 0x80: kirakira effect, 0x60: its kind (Sce_kirakira_set) */
    struct tagPARTS_W *pParts;         /* 0xC4 */
    UCHAR padC8[0x130 - 0xC8];
    ULONG Unk130;                      /* 0x130 */
    UCHAR pad134[0x188 - 0x134];
    ULONG *pPacket188;                 /* 0x188 POLY_GT4 packets (Col_chg_init) */
    SHORT Unk18C;                      /* 0x18C fall speed (func_80036D94) */
    SHORT Unk18E;                      /* 0x18E */
    SHORT Unk190;                      /* 0x190 */
    SHORT Unk192;                      /* 0x192 ground y */
} OBJ_MODEL_WORK;                      /* 0x194, G.Ob_model[] */

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
