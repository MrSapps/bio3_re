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
    SHORT Unk2C;                       /* 0x2C (0x1F8) */
    signed char Unk2E;                 /* 0x2E (0x1FA) 0 when Type & 0x80: no jump attack */
    signed char Unk2F;                 /* 0x2F (0x1FB) 0 when Type & 0x100: no ceiling drop */
} EM25_FREE;

#define EF(p) ((EM25_FREE *)(p)->Unk1CC)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

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
void func_80102C7C(ENEMY_WORK *pEm);
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

void func_801000C8(ENEMY_WORK *pEm);
void func_80100658(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100758(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100A10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100B18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100C80(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101028(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101254(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801013EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101468(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801015B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101764(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101A40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101C58(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101D54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101D90(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101DDC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101FD8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801020B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010219C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102338(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102578(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801027C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010295C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102B3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102B78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102BA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102CE8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102ED4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102F88(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103134(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801032A4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801033CC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103490(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801035A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103658(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010374C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801037FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801038C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801039E0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103B30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103D30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103D94(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103EB8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103FC4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104144(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801041B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104340(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801043AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801043E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801044AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801046D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104804(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104888(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801048C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104AE4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104C0C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104D74(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104E18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105010(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801050A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801050A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106578(ENEMY_WORK *pEm);
void func_80106640(ENEMY_WORK *pEm);
LONG func_80106744(ENEMY_WORK *pEm, SHORT Dist, LONG Dist2);

#endif /* OVL_EM25_H */
