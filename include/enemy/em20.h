#ifndef OVL_EM20_H
#define OVL_EM20_H
/*
 * Enemy module em20 (enemy id 0x20, zombie dog / Cerberus), the successor of
 * RE2's EM20.C. Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2); the
 * routine tables are in the module's .data. RE2 -> RE3 ENEMY_WORK fields:
 * Timer0 0xD8, Timer1 UnkDA, Timer2 UnkE2, Timer3 UnkE3, Neck_flg Unk120,
 * Ground Unk122, L_pl Unk154, L_spl Unk158, Hokan_flg Hokan_flg, Sca_info word at
 * 0xAC (EM20_SCA_INFO), At_em_no Col.Unk3, Be_flg 0x4000000/0x8000000/0x10000000
 * moved to Unk132 0x4000/0x8000 and Be_flg 0x4000.
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "pl_wep.h"

/* RE2 EM20_FREE (EM_SUB.H), the enemy's Free[] at 0x1CC */
typedef struct tagEM20_FREE {
    UCHAR Area_keep;                   /* 0x00 (0x1CC) Rnd_area() target area */
    UCHAR Jump_flg;                    /* 0x01 (0x1CD) */
    SHORT Stress;                      /* 0x02 (0x1CE) */
    UCHAR Spin_flg;                    /* 0x04 (0x1D0) */
    UCHAR Fix_cnt;                     /* 0x05 (0x1D1) */
    UCHAR Hit_type;                    /* 0x06 (0x1D2) */
    UCHAR Blood;                       /* 0x07 (0x1D3) blood effects left to spawn (Set_blood) */
    UCHAR Em_next;                     /* 0x08 (0x1D4) */
    UCHAR Kage_routine;                /* 0x09 (0x1D5) */
    UCHAR Bang_ok;                     /* 0x0A (0x1D6) */
    UCHAR Combo_flg;                   /* 0x0B (0x1D7) */
    SHORT Dash_turn;                   /* 0x0C (0x1D8) */
    UCHAR Kill_ok;                     /* 0x0E (0x1DA) */
    UCHAR Target_pl;                   /* 0x0F (0x1DB) 1: target is the sub player (G.pEnemy[1]) */
    SHORT Yori_fx;                     /* 0x10 (0x1DC) */
    SHORT Yori_fz;                     /* 0x12 (0x1DE) */
    UCHAR No_hit;                      /* 0x14 (0x1E0) */
    UCHAR House_flg;                   /* 0x15 (0x1E1) */
    UCHAR Outside;                     /* 0x16 (0x1E2) */
    UCHAR No_atck;                     /* 0x17 (0x1E3) */
    UCHAR Stay;                        /* 0x18 (0x1E4) frames without moving */
    UCHAR No_voice;                    /* 0x19 (0x1E5) */
    UCHAR No_fire;                     /* 0x1A (0x1E6) */
} EM20_FREE;

#define EM20_W(p) ((EM20_FREE *)(p)->Unk1CC)
/* RE2 Sca_info: the collision node's Unk2C/Hit_flg pair read as one word */
#define EM20_SCA_INFO(p) (*(ULONG *)&(p)->Col.Unk2C)

/* RE2 EM20_BLOOD_TBL (RE3 adds a word) */
typedef struct tagEM20_BLOOD_TBL {
    UCHAR Id;                          /* 0x0 0x80: effect follows the part matrix */
    UCHAR Type;                        /* 0x1 */
    USHORT Dir;                        /* 0x2 random direction range */
    USHORT Zoom;                       /* 0x4 */
    USHORT Unk6;                       /* 0x6 */
} EM20_BLOOD_TBL;

/* RE3 only: effect records started on some parts at init (Type & 0x80, Em20_Esp_set) */
typedef struct tagEM20_ESP_TBL {
    SHORT Parts;                       /* 0x0 part index */
    USHORT Type;                       /* 0x2 Esp type (code bits 16..31) */
    SVECTOR Ofs;                       /* 0x4 offset from the part; pad = code bits 0..15 */
} EM20_ESP_TBL;


extern USHORT Em20_life[16];
extern USHORT Em20_life_easy[16];
extern UCHAR em20_time_tbl00[8];
extern UCHAR em20_time_tbl01[8];
extern UCHAR em20_time_tbl02[8];
extern void (*Em20_routine_0[])();
extern UCHAR em20_routine_tbl[12];
extern EM20_ESP_TBL em20_esp_tbl[4];
extern void (*em20_move_tbl[])();
extern LONG yori_ofs[8];
extern void (*move_run_tbl[])();
extern USHORT dash_turn_rnd[8];
extern void (*move_atck_tbl[])();
extern void (*move_sleep_tbl[])();
extern UCHAR house_mot_rnd[8];
extern void (*move_rush_tbl[])();
extern void (*move_bodyatck_tbl[])();
extern void (*move_jumpin_tbl[])();
extern void (*em20_dmg_tbl[])();
extern void (*dmg_nml_tbl[])();
extern SHORT dir_hosei[8];
extern void (*dmg_combo_tbl[])();
extern void (*dmg_nml2_tbl[])();
extern void (*em20_die_tbl[])();
extern void (*die_nml_init[])();
extern void (*die_nml_tbl[])();
extern void (*die_bang_tbl[])();
extern UCHAR bang_parts_no[8];
extern UCHAR tail_parts_no[8];
extern void (*die_gnml_tbl[])();
extern void (*die_spark_tbl[])();
extern void (*die_bang2_tbl[])();
extern void (*Em20_sce_tbl[])();
extern UCHAR pl_chi_parts[4];
extern UCHAR mot_tbl[4];
extern EM20_BLOOD_TBL blood_id_tbl[16];

void EM_MAIN(ENEMY_WORK *pEm);
void Em20_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Esp_set(ENEMY_WORK *pEm);
void Em20_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em20_rndwalk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_yori(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_run00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_run10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_run20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_run30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_atck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_atck00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Jump_set(ENEMY_WORK *pEm, LONG spd_x, LONG spd_y);
void Em20_Mv_atck10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em20_Mv_jump10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_atck20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em20_Mv_jump20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_atck30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_sleep(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_sleep00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_sleep10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_sleep20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_sleep30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_ikaku(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_critical(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_critical00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_critical10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_escape(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_floorout(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_walk2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_died(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_eat(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_mv_eat00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_house(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_mv_house00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_mv_house10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_rush(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_rush00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_rush10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_rush20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_bodyatck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_bodyatck10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_bodyatck20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_bodyatck30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Move_jumpin(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_jumpin00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Mv_jumpin20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Ck_em_id(ENEMY_WORK *pEm, ENEMY_WORK **ppList, LONG Id, LONG Num);
void Em20_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_combo(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_combo00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_combo10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_combo20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_combo30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_freeze(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_nml2_00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Set_dmghit(ENEMY_WORK *pEm, ULONG Type);
void Em20_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_bang(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_bang00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Set_head_bomb(ENEMY_WORK *pEm);
void Set_tail_bomb(ENEMY_WORK *pEm);
void Em20_Die_bang10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_gnml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_freeze(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_spark00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_bang2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_bang2_00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Col_set(ENEMY_WORK *pEm, LONG n);
void Em20_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Blood_move(ENEMY_WORK *pEm);
void PLDIE_EM(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Dog_rot_neck(ENEMY_WORK *pEm);
void Rate_speed_up(ENEMY_WORK *pEm, LONG g, LONG limit);
void Rate_speed_down(ENEMY_WORK *pEm, LONG g, LONG limit);
void Set_run_motion(ENEMY_WORK *pEm, SHORT d_tmp);
LONG Atck_range_ck(ENEMY_WORK *pEm);
ULONG hit_ck_main(ENEMY_WORK *pEm, LONG pl);
ULONG Atck_hit_ck(ENEMY_WORK *pEm);
void Set_blood(ENEMY_WORK *pEm, UCHAR parts, ULONG type);
void Em20_Pl_to_dmg(ULONG pl, UCHAR dir);
void Em20_Pl_to_die(ULONG pl);
void Ck_Floor(ENEMY_WORK *pEm);
void Ck_Died(ENEMY_WORK *pEm);
ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm);

#endif /* OVL_EM20_H */
