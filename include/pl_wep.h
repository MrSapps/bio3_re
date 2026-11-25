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

/* attack data, Esp_at_tbl[] entries */
typedef struct tagHIT_ATTR {
    UCHAR Unk0;                        /* 0x00 */
    UCHAR Unk1;                        /* 0x01 */
    SHORT Top;                         /* 0x02 */
    SHORT Bottom;                      /* 0x04 */
    UCHAR Damage_cnt2;                 /* 0x06 Damage_cnt for the second box (Pos_em_at_ck) */
    UCHAR Damage_cnt;                  /* 0x07 */
    SHORT W2;                          /* 0x08 second, inner hit box (Pos_em_at_ck) */
    SHORT D2;                          /* 0x0A */
    SHORT Damage2;                     /* 0x0C damage for the second box */
    SHORT W;                           /* 0x0E */
    SHORT D;                           /* 0x10 */
    SHORT Damage;                      /* 0x12 subtracted from Life */
} HIT_ATTR;

extern USHORT Pl_wep_rot_tbl0[];        /* 0x8009D0B4 Cdir_z sequence, 0x8000 ends */
extern USHORT Pl_wep_rot_tbl1[];        /* 0x8009D0DC */

/* weapon sprite effect, two POLY_FT4 buffers (0x58 bytes, Pl_wep_poly[6]) */
typedef struct tagPL_WEP_POLY {
    UCHAR Be_flg;                      /* 0x00 */
    UCHAR pad1;
    SHORT Unk2;                        /* 0x02 */
    LONG *pPos;                        /* 0x04 position the poly sits at (x, y, z) */
    POLY_FT4 Poly[2];                  /* 0x08 */
} PL_WEP_POLY;
extern PL_WEP_POLY Pl_wep_poly[6];
extern UCHAR Pl_aim_dir;                /* 0x8009CD3D 0 level, 1 up, 2 down */      /* 0x800DE8F8 */

extern struct tagMD_OBJ *Pl_wep_tmd;    /* 0x8009CD38 weapon model objects (md_sub.h); n_obj in the word before */
extern PARTS_W *Pl_wep_parts;           /* 0x800DE8F0 weapon part (pSin_parts_ptr[22]) */
extern PARTS_W *Pl_wep_oya;             /* 0x800DE8F4 part the weapon hangs from (pSin_parts_ptr[4]) */
typedef void (*PL_WEP_FN)(ENEMY_WORK *pEm);
extern PL_WEP_FN Pl_weapon_init[];      /* 0x8009DCD4 index Type */
typedef void (*PL_MV05_FN)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
extern PL_MV05_FN pl_mv05_t2_spr[];     /* 0x8009CED8 index Type - 1: pl_wXX_t2_move */
extern PL_MV05_FN pl_mv05_w_tbl[];      /* 0x8009CE88 index Type - 1 */
extern PL_MV05_FN pl_mv05_w0b_tbl[];    /* 0x8009D088 index Routine_2 */

/* hit-check request (Pos_em_at_ck, Pos_om_at_ck): a box W x H x D turned by R around *pPos */
typedef struct tagHIT_REQ {
    VECTOR *pPos;                      /* 0x0 */
    SHORT W;                           /* 0x4 */
    SHORT H;                           /* 0x6 */
    SHORT D;                           /* 0x8 */
    SHORT padA;
    SHORT R;                           /* 0xC rotation y */
    SHORT padE;
} HIT_REQ;

/* hit area handed to Hit_area_em_ck (is a work inside the box) */
typedef struct tagHIT_AREA {
    VECTOR *pPos;                      /* 0x00 centre */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} HIT_AREA;

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

/* per-weapon hit data, 0x20 bytes, Pl_wep_data[Id][Type - 1] */
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

LONG Pl_wep_eff_parts_init(ENEMY_WORK *pEm);
void Pl_wep_poly_init(void);

extern PARTS_W *pPl_emd_top;
extern UCHAR Pl_neck_timer; /* RE2 Pl_neck_timer */
extern UCHAR Pl_wep_frame_tbl[][3];
extern UCHAR Pl_wep_turn_tbl[];
extern PL_WEP_DATA *Pl_wep_data[]; /* per-weapon hit data, index player Id */
extern USHORT Plw_no[];
extern UCHAR Em_score_tbl[][2];
extern UCHAR Chaser_kill_score;
extern UCHAR Em_kill_combo;
extern UCHAR Pl_aim_cnt;

void Pl_neck(ENEMY_WORK *pEm, ULONG LL, SHORT Dir);
void Pl_water(ENEMY_WORK *pEm);
void Pl_weapon_vib(ENEMY_WORK *pEm); /* RE2 PL_WEP.C Pl_weapon_vib */
void pl_mv05_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void pl_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv05_w0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0_t6(ENEMY_WORK *pEm);
void pl_mv05_w1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w1_t0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w1_t1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w1_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w1_t3(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w1_t4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w1_t5(ENEMY_WORK *pEm);
void Pl_target_next(ENEMY_WORK *pEm);
void pl_mv05_w_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0_t8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0_t9(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0_t10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w_end(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2_t1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2_t4(ENEMY_WORK *pEm);
void pl_mv05_w0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0b_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void Pl_wep_ready_reset(ENEMY_WORK *pEm);
void Pl_wep_recoil_6(ENEMY_WORK *pEm);
void Pl_wep_recoil_10(ENEMY_WORK *pEm);
void Pl_wep_flag_set(ENEMY_WORK *pEm);
void Pl_wep_spin_0b(ENEMY_WORK *pEm);
void pl_w01_t2_move(ENEMY_WORK *pEm);
void pl_w02_t2_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_w03_t2_move(ENEMY_WORK *pEm);
void pl_w04_t2_move(ENEMY_WORK *pEm);
void pl_w05_t2_move(ENEMY_WORK *pEm);
void pl_w06_t2_move(ENEMY_WORK *pEm);
void pl_w07_t2_move(ENEMY_WORK *pEm);
void pl_w08_t2_move(ENEMY_WORK *pEm);
void pl_w09_t2_move(ENEMY_WORK *pEm);
void pl_w0a_t2_move(ENEMY_WORK *pEm);
void pl_w0b_t2_move(ENEMY_WORK *pEm);
void pl_w0c_t2_move(ENEMY_WORK *pEm);
void pl_w0d_t2_move(ENEMY_WORK *pEm);
void pl_w0e_t2_move(ENEMY_WORK *pEm);
void pl_w0f_t2_move(ENEMY_WORK *pEm);
void pl_w10_t2_move(ENEMY_WORK *pEm);
void pl_w11_t2_move(ENEMY_WORK *pEm);
void pl_w12_t2_move(ENEMY_WORK *pEm);
void Pl_weapon_ch(ENEMY_WORK *pEm);
void Weapon_cls(ENEMY_WORK *pEm);
void Weapon_parts_clr(ENEMY_WORK *pEm);
void Pl_weapon_ch_init(ENEMY_WORK *pEm);
void Non_init(ENEMY_WORK *pEm);
void Plw02_init(ENEMY_WORK *pEm);
void Plw06_init(ENEMY_WORK *pEm);
void Plw0b_init(ENEMY_WORK *pEm);
void Plw0c_init(ENEMY_WORK *pEm);
void Plw0d_init(ENEMY_WORK *pEm);
void Plw0e_init(ENEMY_WORK *pEm);
void Plw10_init(ENEMY_WORK *pEm);
ULONG Em_ck(ENEMY_WORK *pEm, ULONG LL);
LONG Hit_area_ck(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);
LONG Hit_area_em_ck(LONG *pPos0, LONG *pPos1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pEm inside the box */
LONG Wedge_ck_w(ULONG A, ULONG B, VECTOR *pV, LONG W);
LONG Wedge_ck(ULONG A, ULONG B, VECTOR *pV);
ENEMY_WORK *Weapon_at_ck(MATRIX *pM, PL_WEP_DATA *pW, ULONG Type);
void Matrix_rel_pos(MATRIX *pM0, MATRIX *pM1, SVECTOR *pOut);
void Mag_set(ENEMY_WORK *pEm);
void Mag_down(ENEMY_WORK *pEm);
void Pl_wep_eff_init(ENEMY_WORK *pEm);
void Pl_wep_dummy(void);
void Pl_wep_parts_drop(ENEMY_WORK *pEm);
void Pl_wep_dummy2(void);
LONG Pl_wep_eff_parts_set(ENEMY_WORK *pEm);
UCHAR Pl_wep_eff_parts_cnt(void);
ENEMY_WORK *Em_near_dir_ck(VECTOR *pPos, SHORT Dir, ULONG LL, SHORT Lim);
void Pl_wep_eff_move(ENEMY_WORK *pEm);
ULONG Parts_near_get(ENEMY_WORK *pEm, MATRIX *pM, UCHAR *pTbl);
LONG Dir_limit(LONG Dir, LONG Base, LONG Lim, SHORT *pDir);
void Vec_dir_get(SVECTOR *pV, SVECTOR *pOut);
SHORT Get_dir_xz(LONG X, LONG Z);
void Pl_wep_poly_set(PL_WEP_POLY *pWp, SHORT Unk2, LONG *pPos);
void Pl_wep_poly_rgb(PL_WEP_POLY *pWp, ULONG Rgb);
void Pl_wep_poly_off(PL_WEP_POLY *pWp);
void Pl_wep_poly_ck(PL_WEP_POLY *pWp, SVECTOR *pV);
void Pl_wep_poly_trans(void);
ULONG Em_near_pos_ck(VECTOR *pPos, LONG Ll0, LONG Ll1);
LONG Pos_em_at_ck(HIT_REQ *pReq, HIT_ATTR *pAttr);
LONG Weapon_om_at_ck(MATRIX *pM, PL_WEP_DATA *pW, ULONG Type);
LONG Pos_om_at_ck(HIT_REQ *pReq, HIT_ATTR *pAttr);
VECTOR *Om_target_near(ENEMY_WORK *pEm, ULONG LL);
ULONG Pos_em1_at_ck(ENEMY_WORK *pEm, HIT_REQ *pReq, HIT_ATTR *pAttr);
void Quake_add(LONG X, LONG Y);
void Em_kill_score(ULONG Id, LONG Flg);

/* RE2 PL_WEP2.H: player weapon move routine tables */
extern PL_MV05_FN pl_mv05_w0_tbl[]; /* 0x8009D004 index Routine_2 */
extern PL_MV05_FN pl_mv05_w1_tbl[]; /* 0x8009D030 index Routine_2 */
extern PL_MV05_FN pl_mv05_w2_tbl[]; /* 0x8009D05C index Routine_2 */

#endif /* PL_WEP_H */
