#ifndef OVL_EM25_H
#define OVL_EM25_H
/*
 * Enemy module em25 (enemy id 0x25, spider), the successor of RE2's EM25.C +
 * EM25_S0.C (one object): same routine tables (Em25_routine_0, Em25_move_tbl,
 * floor/ceiling/wall/wallset move tables, ceil_turn_tbl, Em25_dm, die tables,
 * float_fy, blood_id_tbl, foot_se_tbl, biripos_tbl). Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2).
 * RE2 -> RE3 ENEMY_WORK fields: Timer0 0xD8, Timer1 UnkDA, L_pl Unk154,
 * L_spl Unk158, Neck_flg Unk120.
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "main.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "sca.h"
#include "pl_wep.h"

/* RE2 EM25_FREE (EM_SUB.H), the enemy's Free[] at 0x1CC (RE2 retail Free[1]) */
typedef struct tagEM25_FREE {
    SHORT at_cnt;                      /* 0x00 (0x1CC) */
    SHORT Flip;                        /* 0x02 (0x1CE) 0x800 upside down */
    SHORT sch_flg;                     /* 0x04 (0x1D0) */
    SHORT sc_timer;                    /* 0x06 (0x1D2) */
    UCHAR Foot_flg;                    /* 0x08 (0x1D4) */
    UCHAR Foot_cnt;                    /* 0x09 (0x1D5) */
    UCHAR Place;                       /* 0x0A (0x1D6) 0 floor, 1 ceiling, 2 wall down, 3 wall */
    UCHAR Jump_flg;                    /* 0x0B (0x1D7) */
    UCHAR Die_weak;                    /* 0x0C (0x1D8) */
    UCHAR Kage_routine;                /* 0x0D (0x1D9) */
    SHORT Sc_dir;                      /* 0x0E (0x1DA) */
    SCA_DATA *pWall;                   /* 0x10 (0x1DC) */
    LONG Down_point;                   /* 0x14 (0x1E0) */
    UCHAR Wall_pos;                    /* 0x18 (0x1E4) */
    UCHAR Em_next;                     /* 0x19 (0x1E5) */
    UCHAR Nowall_cnt;                  /* 0x1A (0x1E6) */
    UCHAR Rno1_bak;                    /* 0x1B (0x1E7) */
    UCHAR Rno2_bak;                    /* 0x1C (0x1E8) */
    UCHAR Rno3_bak;                    /* 0x1D (0x1E9) */
    UCHAR Cont_flg;                    /* 0x1E (0x1EA) */
    UCHAR Target_pl;                   /* 0x1F (0x1EB) 1: target is the sub player */
    UCHAR Foot_se;                     /* 0x20 (0x1EC) */
    UCHAR No_redie;                    /* 0x21 (0x1ED) */
    signed char Hitline;               /* 0x22 (0x1EE) */
    UCHAR Spark_cnt;                   /* 0x23 (0x1EF) */
    UCHAR Dmg_type;                    /* 0x24 (0x1F0) */
    UCHAR No_fire;                     /* 0x25 (0x1F1) */
    UCHAR Water_rno;                   /* 0x26 (0x1F2) */
    UCHAR Unk27;                       /* 0x27 (0x1F3) RE2 retail-only byte */
    ENEMY_WORK *pPair;                 /* 0x28 (0x1F4) another em25 found by Ck_em_id */
    SHORT Float_y;                     /* 0x2C (0x1F8) Em25_float_move: height the dead body has risen */
    signed char Jump_ok;               /* 0x2E (0x1FA) 0 when Type & 0x80: no jump attack */
    signed char Drop_ok;               /* 0x2F (0x1FB) 0 when Type & 0x100: no ceiling drop */
} EM25_FREE;

#define EF(p) ((EM25_FREE *)(p)->Unk1CC)

/* RE2 SCA_DATA.Floor: RE3 splits the word into Floor_lo/Floor_hi/High, em25 still reads it whole */
#define SCA_FLOOR(p) (*(ULONG *)&(p)->Floor_lo)

/* RE2 EM25_BLOOD_TBL (RE3 adds the palette word) */
typedef struct tagEM25_BLOOD_TBL {
    UCHAR Id;                          /* 0x0 0x80: effect follows the part matrix */
    UCHAR Type;                        /* 0x1 */
    USHORT Dir;                        /* 0x2 */
    USHORT Zoom;                       /* 0x4 */
    USHORT Clut;                       /* 0x6 code bits 24..27; 0xFF: 1, or 6 when Unk132 & 0x200 */
} EM25_BLOOD_TBL;

/* room collision header as em25 reads it (RE2 SCA_HEAD layout: shape count word at 4) */
typedef struct tagEM25_SCA_HEAD {
    SHORT Cx;                          /* 0x0 */
    SHORT Cz;                          /* 0x2 */
    ULONG Num;                         /* 0x4 shapes + 1 (the header slot) */
} EM25_SCA_HEAD;
#define SCA_HEAD_NUM(p) (((EM25_SCA_HEAD *)(p))->Num)


/* module functions (RE2 EM25.C / EM25_S0.C names) */
void Em25_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv_taiki(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em25_mv08sp(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall000(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_wall_init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl110(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml1010(ENEMY_WORK *pEm);
void Em25_Die_nml30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_ftl(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_ftl30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Die_break_set(ENEMY_WORK *pEm);
LONG Ck_land(ENEMY_WORK *pEm, LONG Fe, LONG Fg, LONG Fw);
void Chg_modelcol(ENEMY_WORK *pEm, ULONG Rgb);
SHORT Get_wallshot(ENEMY_WORK *pEm);
void Em25_wall_hit_ofs(ENEMY_WORK *pEm);
void Em25_kage_move(ENEMY_WORK *pEm);
void Em25_float_move(ENEMY_WORK *pEm);
void Em25_foot_set(ENEMY_WORK *pEm, UCHAR Flg);
SHORT Em25_goto00(ENEMY_WORK *pEm, SHORT rr);
ULONG Ck_lock_wall(ENEMY_WORK *pEm);
void Em25_wall_rot(ENEMY_WORK *pEm, SHORT rot);
void Em25_add_speed(ENEMY_WORK *pEm, LONG rx, LONG ry, LONG rz);
LONG Ck_wall_edge(ENEMY_WORK *pEm, SHORT Scdir);
LONG Ck_near_wall(ENEMY_WORK *pEm, SCA_DATA *pSc, SHORT dir);
LONG Ck_wall_other(ENEMY_WORK *pEm, SHORT r);
SHORT Ck_wall_to_pl(ENEMY_WORK *pEm, SHORT rot);
void Set_blood(ENEMY_WORK *pEm, UCHAR parts, UCHAR type);
void Set_poison(ENEMY_WORK *pEm, ULONG Type, SHORT Dir);
ULONG atck_ck_main(ENEMY_WORK *pEm, ULONG pl, LONG R);
ULONG Em25_atck_ck(ENEMY_WORK *pEm, LONG R, LONG H);
LONG Foot_break_set(ENEMY_WORK *pEm);
void Em25_Set_child(ENEMY_WORK *pEm, USHORT type, UCHAR cnt);
ENEMY_WORK **Get_child_work(ENEMY_WORK **ppEm);
void Em25_Pl_to_dmg(ULONG pl, UCHAR dir);
void Em25_Pl_to_die(ULONG pl);
ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm);
void Set_foot_se(ENEMY_WORK *pEm);
LONG Sca_get_pos(LONG no, SCA_DATA *pSc);
LONG Ck_em_id(ENEMY_WORK *pEm, ENEMY_WORK **ppList, LONG Id, LONG Num);

extern USHORT Em25_life[16];
extern USHORT Em25_life_easy[16];
extern void (*Em25_routine_0[8])();
extern void (*Em25_move_tbl[4])();
extern void (*Em25_floor_tbl[10])();
extern ULONG Em25_mv01_tbl[3][3];
extern void (*Em25_ceiling_tbl[13])();
extern void (*Em25_mv0c_tbl[3])();
extern void (*move_wall_tbl[8])();
extern SHORT ceil_turn_tbl[8];
extern void (*move_wallset_tbl[8])();
extern void (*Em25_dm[32])();
extern void (*dmg_nml_tbl[4])();
extern UCHAR next_rtn_tbl[8];
extern void (*dmg_ftl_tbl[4])();
extern void (*dm_ftl11_tbl[3])();
extern void (*dm_ftl21_tbl[3])();
extern UCHAR hitmark_tbl00[4];
extern void (*em25_die_tbl[32])();
extern void (*die_nml_tbl[4])();
extern void (*die_nml10_tbl[3])();
extern SVECTOR shibuki_tbl[8];
extern void (*die_nml20_tbl[3])();
extern void (*die_ftl_tbl[4])();
extern void (*die_ftl20_tbl[3])();
extern UCHAR float_fy[8];
extern EM25_BLOOD_TBL blood_id_tbl[11];
extern UCHAR child_rtn_tbl[8];
extern UCHAR foot_se_tbl[4];
extern UCHAR biripos_tbl[8];

void Em25_main(ENEMY_WORK *pEm);
void Em25_Move_floor(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv000(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Move_ceiling(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv001(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv011(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv071(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_mv0c(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_fall_start(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Move_walldown(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall200(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall70(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Move_wallset(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall001(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Mv_wall201(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_nml10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl111(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl112(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl210(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_ftl211(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_combo(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_freeze(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_dm_fire2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml100(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml101(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml102(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml200(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_nml201(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_ftl00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_ftl10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_ftl20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_ftl200(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_freeze(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Die_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em25_spark_move(ENEMY_WORK *pEm);
void Set_gunrange(ENEMY_WORK *pEm);
LONG Em25_box_ray_ck(ENEMY_WORK *pEm, SHORT Dist, LONG Dist2);

#endif /* OVL_EM25_H */
