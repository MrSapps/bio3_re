#ifndef PL_WEP_H
#define PL_WEP_H
/*
 * RE2 PL_WEP.C / PL_WEP2.C: player weapon handling (aim / fire / reload, the
 * weapon model parts, effects and sounds). RE3 layouts were taken from the RE3 code.
 */
#include "bio3.h"
#include "em_sub.h"
#include "om.h"
#include "esp.h"

/* the player is an ENEMY_WORK too (G.Player at G+0x248C), as in RE2 PL_WEP.C */
typedef ENEMY_WORK PLAYER_WORK;

extern USHORT Pl_wep_rot_tbl0[];        /* 0x8009D0B4 Cdir_z sequence, 0x8000 ends */
extern USHORT Pl_wep_rot_tbl1[];        /* 0x8009D0DC */

/* weapon sprite effect, two POLY_FT4 buffers (0x58 bytes, Pl_wep_poly[6]) */
typedef struct tagPL_WEP_POLY {
    UCHAR Be_flg;                      /* 0x00 */
    UCHAR pad1;
    SHORT Unk2;                        /* 0x02 */
    LONG Unk4;                         /* 0x04 */
    POLY_FT4 Poly[2];                  /* 0x08 */
} PL_WEP_POLY;
extern PL_WEP_POLY Pl_wep_poly[6];
extern UCHAR Pl_aim_dir;                /* 0x8009CD3D 0 level, 1 up, 2 down */      /* 0x800DE8F8 */

extern ULONG *Pl_wep_tmd;                /* 0x8009CD38 weapon TMD; [-1] = parts count */
extern PARTS_W *Pl_wep_parts;           /* 0x800DE8F0 weapon part (pSin_parts_ptr[22]) */
extern PARTS_W *Pl_wep_oya;             /* 0x800DE8F4 part the weapon hangs from (pSin_parts_ptr[4]) */
typedef void (*PL_WEP_FN)(PLAYER_WORK *pEm);
extern PL_WEP_FN Pl_weapon_init[];      /* 0x8009DCD4 index Type */
typedef void (*PL_MV05_FN)(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
extern PL_MV05_FN pl_mv05_t2_spr[];     /* 0x8009CED8 index Type - 1: pl_wXX_t2_move */
extern PL_MV05_FN pl_mv05_w_tbl[];      /* 0x8009CE88 index Type - 1 */
extern PL_MV05_FN pl_mv05_w0b_tbl[];    /* 0x8009D088 index Routine_2 */

/* hit-check request (func_800472EC, func_80047BD0): a box W x H x D turned by R around *pPos */
typedef struct tagHIT_REQ {
    VECTOR *pPos;                      /* 0x0 */
    SHORT W;                           /* 0x4 */
    SHORT H;                           /* 0x6 */
    SHORT D;                           /* 0x8 */
    SHORT padA;
    SHORT R;                           /* 0xC rotation y */
    SHORT padE;
} HIT_REQ;

/* hit area handed to func_800445C8 (is a work inside the box) */
typedef struct tagHIT_AREA {
    VECTOR *pPos;                      /* 0x00 centre */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} HIT_AREA;

/* hit area: a box Top..Bottom high, radius R, around pM->t (ENEMY_WORK + 0x134, OM_TARGET + 0x8) */
typedef struct tagEM_HIT_AREA {
    SHORT Top;                         /* 0x00 */
    SHORT Bottom;                      /* 0x02 */
    UCHAR Rate0;                       /* 0x04 */
    UCHAR Rate1;                       /* 0x05 */
    SHORT R;                           /* 0x06 */
    MATRIX *pM;                        /* 0x08 */
} EM_HIT_AREA;

/* aim target of an object model, 0x18 bytes from G.pEm_mem, listed in G.pUnk75AC (Obj_model_set) */
typedef struct tagOM_TARGET {
    USHORT Flg;                        /* 0x00 0xFF: off */
    SHORT Unk2;                        /* 0x02 */
    OBJ_MODEL_WORK *pOm;               /* 0x04 */
    EM_HIT_AREA Area;                  /* 0x08 pM = &pOm->Workm */
    ULONG Unk14;                       /* 0x14 */
} OM_TARGET;

/* magazine drop (RE2 struct tagPL_MAG_TBL, MAIN.H) */
typedef struct tagPL_MAG_TBL {
    UCHAR Parts_no;                    /* 0x0 */
    signed char V_y;                   /* 0x1 */
    UCHAR V_xz;                        /* 0x2 */
    UCHAR Flg;                         /* 0x3 */
    SHORT Dir;                         /* 0x4 */
} PL_MAG_TBL;
extern UCHAR Pl_wep_mag_type[];         /* 0x8009DD54 magazine kind + 1, index Type - 1 */
extern PL_MAG_TBL Pl_mag[];             /* 0x8009DD6C */

/* damage per enemy kind: three 10-bit damages, 5-bit Damage_cnt and kind values (low/mid/high hit) */
typedef struct tagPL_WEP_DMG {
    ULONG Damage;                      /* 0x0 */
    USHORT Cnt;                        /* 0x4 */
    USHORT Kind;                       /* 0x6 */
} PL_WEP_DMG;

/* per-weapon hit data, 0x20 bytes, D_8009DBB4[Id][Type - 1] */
typedef struct tagPL_WEP_DATA {
    SHORT Unk0;                        /* 0x00 */
    USHORT Unk2;                       /* 0x02 */
    SHORT Unk4;                        /* 0x04 */
    UCHAR pad6[0xC - 0x6];
    USHORT UnkC;                       /* 0x0C */
    SHORT UnkE;                        /* 0x0E */
    SHORT Unk10;                       /* 0x10 */
    SHORT Dist1;                       /* 0x12 distance for the second damage */
    SHORT Dist2;                       /* 0x14 distance for the third damage */
    SHORT Range;                       /* 0x16 */
    UCHAR pad18[0x1C - 0x18];
    PL_WEP_DMG **pDmg;                 /* 0x1C index enemy Id - 0x10, then Type - 1 */
} PL_WEP_DATA;

LONG Pl_wep_eff_parts_init(PLAYER_WORK *pEm);
void func_80046C90(void);
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);

#endif /* PL_WEP_H */
