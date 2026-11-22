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

/* attack data, D_80097FCC[] entries */
typedef struct tagHIT_ATTR {
    UCHAR Unk0;                        /* 0x00 */
    UCHAR Unk1;                        /* 0x01 */
    SHORT Top;                         /* 0x02 */
    SHORT Bottom;                      /* 0x04 */
    UCHAR Damage_cnt2;                 /* 0x06 Damage_cnt for the second box (func_800472EC) */
    UCHAR Damage_cnt;                  /* 0x07 */
    SHORT W2;                          /* 0x08 second, inner hit box (func_800472EC) */
    SHORT D2;                          /* 0x0A */
    SHORT Damage2;                     /* 0x0C damage for the second box */
    SHORT W;                           /* 0x0E */
    SHORT D;                           /* 0x10 */
    SHORT Damage;                      /* 0x12 subtracted from Life */
} HIT_ATTR;

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
typedef void (*PL_MV05_FN)(PLAYER_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
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

extern PARTS_W *D_8009CD30;
extern UCHAR Pl_neck_timer; /* RE2 Pl_neck_timer */
extern UCHAR D_8009CF28[][3];
extern UCHAR D_8009CF64[];
extern PL_WEP_DATA *D_8009DBB4[]; /* per-weapon hit data, index player Id */
extern USHORT D_8009DCB4[];
extern UCHAR D_8009DDC4[][2];
extern UCHAR D_8009DE10;
extern UCHAR D_8009DE24;
extern UCHAR D_800DE8E8;

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
void func_8003F79C(ENEMY_WORK *pEm);
void func_8003FA38(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void func_8003FB78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0_t9(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0_t10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void func_8003FEFC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2_t1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2_t2(PLAYER_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w2_t4(PLAYER_WORK *pEm);
void pl_mv05_w0b(PLAYER_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv05_w0b_t2(PLAYER_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void Pl_wep_ready_reset(PLAYER_WORK *pEm);
void Pl_wep_recoil_6(PLAYER_WORK *pEm);
void Pl_wep_recoil_10(PLAYER_WORK *pEm);
void Pl_wep_flag_set(PLAYER_WORK *pEm);
void Pl_wep_spin_0b(PLAYER_WORK *pEm);
void pl_w01_t2_move(PLAYER_WORK *pEm);
void pl_w02_t2_move(PLAYER_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void func_800410F8(PLAYER_WORK *pEm);
void func_80041594(PLAYER_WORK *pEm);
void func_800418CC(PLAYER_WORK *pEm);
void func_80041A80(PLAYER_WORK *pEm);
void func_80041C34(PLAYER_WORK *pEm);
void func_80041DE8(PLAYER_WORK *pEm);
void func_80041F9C(PLAYER_WORK *pEm);
void func_800421F8(PLAYER_WORK *pEm);
void func_80042474(PLAYER_WORK *pEm);
void func_80042660(PLAYER_WORK *pEm);
void func_800427D8(PLAYER_WORK *pEm);
void func_80042AE4(PLAYER_WORK *pEm);
void func_80042E34(PLAYER_WORK *pEm);
void func_80043328(PLAYER_WORK *pEm);
void func_800434F8(PLAYER_WORK *pEm);
void func_800439F8(PLAYER_WORK *pEm);
void Pl_weapon_ch(PLAYER_WORK *pEm);
void Weapon_cls(PLAYER_WORK *pEm);
void Weapon_parts_clr(PLAYER_WORK *pEm);
void Pl_weapon_ch_init(PLAYER_WORK *pEm);
void Non_init(PLAYER_WORK *pEm);
void Plw02_init(PLAYER_WORK *pEm);
void Plw_init_43FF0(PLAYER_WORK *pEm);
void Plw_init_44054(PLAYER_WORK *pEm);
void Plw_init_440AC(PLAYER_WORK *pEm);
void Plw_init_440F4(PLAYER_WORK *pEm);
void Plw_init_44134(PLAYER_WORK *pEm);
void Plw_init_4417C(PLAYER_WORK *pEm);
ULONG Em_ck(ENEMY_WORK *pEm, ULONG LL);
LONG func_80044420(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);
LONG func_800445C8(LONG *pPos0, LONG *pPos1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pEm inside the box */
LONG func_8004470C(ULONG A, ULONG B, VECTOR *pV, LONG W);
LONG func_8004479C(ULONG A, ULONG B, VECTOR *pV);
ENEMY_WORK *func_80044804(MATRIX *pM, PL_WEP_DATA *pW, ULONG Type);
void func_80044E90(MATRIX *pM0, MATRIX *pM1, SVECTOR *pOut);
void Mag_set(PLAYER_WORK *pEm);
void Mag_down(ENEMY_WORK *pEm);
void Pl_wep_eff_init(PLAYER_WORK *pEm);
void func_800452E0(void);
void func_800452E8(ENEMY_WORK *pEm);
void func_80045514(void);
LONG func_800455B4(PLAYER_WORK *pEm);
UCHAR Pl_wep_eff_parts_cnt(void);
ENEMY_WORK *func_800457A4(VECTOR *pPos, SHORT Dir, ULONG LL, SHORT Lim);
void func_80045950(ENEMY_WORK *pEm);
ULONG func_80046A0C(ENEMY_WORK *pEm, MATRIX *pM, UCHAR *pTbl);
LONG Dir_limit(LONG Dir, LONG Base, LONG Lim, SHORT *pDir);
void func_80046B94(SVECTOR *pV, SVECTOR *pOut);
SHORT Get_dir_xz(LONG X, LONG Z);
void Pl_wep_poly_set(PL_WEP_POLY *pWp, SHORT Unk2, LONG Unk4);
void Pl_wep_poly_rgb(PL_WEP_POLY *pWp, ULONG Rgb);
void func_80046DF8(PL_WEP_POLY *pWp);
void func_80046E00(PL_WEP_POLY *pWp, SVECTOR *pV);
void func_80046EC4(void);
ULONG func_800470F4(VECTOR *pPos, LONG Ll0, LONG Ll1);
LONG func_800472EC(HIT_REQ *pReq, HIT_ATTR *pAttr);
LONG func_80047860(MATRIX *pM, PL_WEP_DATA *pW, ULONG Type);
LONG func_80047BD0(HIT_REQ *pReq, HIT_ATTR *pAttr);
VECTOR *func_80047DD0(PLAYER_WORK *pEm, ULONG LL);
ULONG func_80047F70(ENEMY_WORK *pEm, HIT_REQ *pReq, HIT_ATTR *pAttr);
void Quake_add(LONG X, LONG Y);
void func_8004834C(ULONG Id, LONG Flg);

/* RE2 PL_WEP2.H: player weapon move routine tables */
extern PL_MV05_FN pl_mv05_w0_tbl[]; /* 0x8009D004 index Routine_2 */
extern PL_MV05_FN pl_mv05_w1_tbl[]; /* 0x8009D030 index Routine_2 */
extern PL_MV05_FN pl_mv05_w2_tbl[]; /* 0x8009D05C index Routine_2 */

#endif /* PL_WEP_H */
