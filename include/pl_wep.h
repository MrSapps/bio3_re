#ifndef PL_WEP_H
#define PL_WEP_H
/*
 * RE2 PL_WEP.C / PL_WEP2.C: player weapon handling (aim / fire / reload, the
 * weapon model parts, effects and sounds). RE3 layouts were taken from the RE3 code.
 */
#include "bio3.h"
#include "em_sub.h"

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
extern PL_WEP_POLY Pl_wep_poly[6];      /* 0x800DE8F8 */

extern ULONG *Pl_wep_tmd;                /* 0x8009CD38 weapon TMD; [-1] = parts count */
extern PARTS_W *Pl_wep_parts;           /* 0x800DE8F0 weapon part (pSin_parts_ptr[22]) */
extern PARTS_W *Pl_wep_oya;             /* 0x800DE8F4 part the weapon hangs from (pSin_parts_ptr[4]) */
typedef void (*PL_WEP_FN)(PLAYER_WORK *pEm);
extern PL_WEP_FN Pl_weapon_init[];      /* 0x8009DCD4 index Type */
typedef void (*PL_MV05_FN)(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key);
extern PL_MV05_FN pl_mv05_t2_spr[];     /* 0x8009CED8 index Type - 1: pl_wXX_t2_move */
extern PL_MV05_FN pl_mv05_w_tbl[];      /* 0x8009CE88 index Type - 1 */
extern PL_MV05_FN pl_mv05_w0b_tbl[];    /* 0x8009D088 index Routine_2 */

LONG Pl_wep_eff_parts_init(PLAYER_WORK *pEm);
void func_80046C90(void);
void func_8001B894(ULONG Id, MATRIX *pM);
void func_800746C0(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);

#endif /* PL_WEP_H */
