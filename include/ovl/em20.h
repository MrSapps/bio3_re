#ifndef OVL_EM20_H
#define OVL_EM20_H
/*
 * Enemy module em20 (enemy id 0x20, zombie dog / Cerberus), the successor of
 * RE2's EM20.C. Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2); the
 * routine tables are in the module's .data. RE2 -> RE3 ENEMY_WORK fields:
 * Timer0 0xD8, Timer1 UnkDA, Timer2 UnkE2, Timer3 UnkE3, Neck_flg Unk120,
 * Ground Unk122, L_pl Unk154, L_spl Unk158, Hokan_flg UnkCA, Sca_info word at
 * 0xAC (EM20_SCA_INFO), At_em_no Col_unk3, Be_flg 0x4000000/0x8000000/0x10000000
 * moved to Unk132 0x4000/0x8000 and Be_flg 0x4000.
 */
#include "bio3.h"
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
/* Routine_0..3 / Move_no..Hokan_flg written as one word (RE2 *(long *)&pEm->Routine_0 = 0x201) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* RE2 Sca_info: the collision node's Unk2C/Hit_flg pair read as one word */
#define EM20_SCA_INFO(p) (*(ULONG *)&(p)->Col_unk2C)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)

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

/* exe functions without a shared prototype (signatures as defined in src/game) */

extern USHORT Em20_life[16] __asm__("D_80105AD0");
extern USHORT Em20_life_easy[16] __asm__("D_80105AF0");
extern UCHAR em20_time_tbl00[8] __asm__("D_80105B10");
extern UCHAR em20_time_tbl01[8] __asm__("D_80105B18");
extern UCHAR em20_time_tbl02[8] __asm__("D_80105B20");
extern void (*Em20_routine_0[])() __asm__("D_80105B28");
extern UCHAR em20_routine_tbl[12] __asm__("D_80105B48");
extern EM20_ESP_TBL em20_esp_tbl[4] __asm__("D_80105B54");
extern void (*em20_move_tbl[])() __asm__("D_80105B84");
extern LONG yori_ofs[8] __asm__("D_80105BC8");
extern void (*move_run_tbl[])() __asm__("D_80105BE8");
extern USHORT dash_turn_rnd[8] __asm__("D_80105BF8");
extern void (*move_atck_tbl[])() __asm__("D_80105C08");
extern void (*move_sleep_tbl[])() __asm__("D_80105C18");
extern UCHAR house_mot_rnd[8] __asm__("D_80105C28");
extern void (*move_rush_tbl[])() __asm__("D_80105C30");
extern void (*move_bodyatck_tbl[])() __asm__("D_80105C3C");
extern void (*move_jumpin_tbl[])() __asm__("D_80105C4C");
extern void (*em20_dmg_tbl[])() __asm__("D_80105C58");
extern void (*dmg_nml_tbl[])() __asm__("D_80105CD4");
extern SHORT dir_hosei[8] __asm__("D_80105CE8");
extern void (*dmg_combo_tbl[])() __asm__("D_80105CF8");
extern void (*dmg_nml2_tbl[])() __asm__("D_80105D08");
extern void (*em20_die_tbl[])() __asm__("D_80105D1C");
extern void (*die_nml_init[])() __asm__("D_80105D98");
extern void (*die_nml_tbl[])() __asm__("D_80105E14");
extern void (*die_bang_tbl[])() __asm__("D_80105E24");
extern UCHAR bang_parts_no[8] __asm__("D_80105E2C");
extern UCHAR tail_parts_no[8] __asm__("D_80105E34");
extern void (*die_gnml_tbl[])() __asm__("D_80105E3C");
extern void (*die_spark_tbl[])() __asm__("D_80105E4C");
extern void (*die_bang2_tbl[])() __asm__("D_80105E5C");
extern void (*Em20_sce_tbl[])() __asm__("D_80105E6C");
extern UCHAR pl_chi_parts[4] __asm__("D_80105E70");
extern UCHAR mot_tbl[4] __asm__("D_80105E74");
extern EM20_BLOOD_TBL blood_id_tbl[16] __asm__("D_80105E78");

void EM_MAIN(ENEMY_WORK *pEm) __asm__("func_80100030");
void Em20_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100144");
void Em20_Esp_set(ENEMY_WORK *pEm) __asm__("func_80100544");
void Em20_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801005FC");
LONG Em20_rndwalk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100648");
void Em20_Move_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801008B4");
void Em20_Move_yori(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100988");
void Em20_Move_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100C88");
void Em20_Mv_run00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100D0C");
void Em20_Mv_run10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100DF4");
void Em20_Mv_run20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100FD4");
void Em20_Mv_run30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101100");
void Em20_Move_atck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101338");
void Em20_Mv_atck00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101394");
void Jump_set(ENEMY_WORK *pEm, LONG spd_x, LONG spd_y) __asm__("func_801015E8");
void Em20_Mv_atck10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101610");
LONG Em20_Mv_jump10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801016D8");
void Em20_Mv_atck20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101780");
LONG Em20_Mv_jump20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801017BC");
void Em20_Mv_atck30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801018B0");
void Em20_Move_sleep(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101A4C");
void Em20_Mv_sleep00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101A88");
void Em20_Mv_sleep10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101BBC");
void Em20_Mv_sleep20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101C08");
void Em20_Mv_sleep30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101C48");
void Em20_Move_ikaku(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101D04");
void Em20_Move_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101DCC");
void Em20_Move_critical(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101EB8");
void Em20_Mv_critical00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101EF8");
void Em20_Mv_critical10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801021EC");
void Em20_Move_escape(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102308");
void Em20_Move_floorout(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010244C");
void Em20_Move_walk2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801024D0");
void Em20_Move_died(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010257C");
void Em20_Move_eat(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801026E0");
void Em20_mv_eat00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102720");
void Em20_Move_house(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010296C");
void Em20_mv_house00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801029BC");
void Em20_mv_house10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102B1C");
void Em20_Move_rush(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102CD8");
void Em20_Mv_rush00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102D24");
void Em20_Mv_rush10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102E3C");
void Em20_Mv_rush20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102EAC");
void Em20_Move_bodyatck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102EF8");
void Em20_Mv_bodyatck10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102F44");
void Em20_Mv_bodyatck20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010300C");
void Em20_Mv_bodyatck30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103050");
void Em20_Move_jumpin(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103170");
void Em20_Mv_jumpin00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801031AC");
void Em20_Mv_jumpin20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103288");
LONG Ck_em_id(ENEMY_WORK *pEm, ENEMY_WORK **ppList, LONG Id, LONG Num) __asm__("func_80103314");
void Em20_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010339C");
void Em20_Dmg_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801033FC");
void Em20_Dmg_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103438");
void Em20_Dmg_nml10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801035BC");
void Em20_Dmg_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801036E4");
void Em20_Dmg_nml30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103850");
void Em20_Dmg_nml40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801038C8");
void Em20_Dmg_combo(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801038FC");
void Em20_Dmg_combo00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103988");
void Em20_Dmg_combo10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103ACC");
void Em20_Dmg_combo20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103B64");
void Em20_Dmg_combo30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103C48");
void Em20_Dmg_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103CF4");
void Em20_Dmg_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103DAC");
void Em20_Dmg_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103EC0");
void func_80103F54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Dmg_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801040FC");
void Em20_Dmg_nml2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801041F4");
void Em20_Dmg_nml2_00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104230");
void Set_dmghit(ENEMY_WORK *pEm, ULONG Type) __asm__("func_80104288");
void Em20_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801042D0");
void Em20_Die_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104310");
void Em20_Die_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010436C");
void Em20_Die_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801043F0");
void Em20_Die_bang(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010449C");
void Em20_Die_bang00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104534");
void Set_head_bomb(ENEMY_WORK *pEm) __asm__("func_80104660");
void Set_tail_bomb(ENEMY_WORK *pEm) __asm__("func_8010473C");
void Em20_Die_bang10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801047D4");
void Em20_Die_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801048FC");
void Em20_Die_gnml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010497C");
void Em20_Die_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104A50");
void Em20_Die_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104AE4");
void func_80104B78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em20_Die_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104C6C");
void Em20_Die_spark00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104CFC");
void Em20_Die_bang2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104D94");
void Em20_Die_bang2_00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104E14");
void Em20_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104E90");
void Em20_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104EEC");
void Em20_Col_set(ENEMY_WORK *pEm, LONG n) __asm__("func_80104FAC");
void Em20_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104FEC");
void Em20_Blood_move(ENEMY_WORK *pEm) __asm__("func_8010502C");
void PLDIE_EM(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801050CC");
void Dog_rot_neck(ENEMY_WORK *pEm) __asm__("func_8010524C");
void Rate_speed_up(ENEMY_WORK *pEm, LONG g, LONG limit) __asm__("func_8010529C");
void Rate_speed_down(ENEMY_WORK *pEm, LONG g, LONG limit) __asm__("func_801052D4");
void Set_run_motion(ENEMY_WORK *pEm, SHORT d_tmp) __asm__("func_8010530C");
LONG Atck_range_ck(ENEMY_WORK *pEm) __asm__("func_8010536C");
ULONG hit_ck_main(ENEMY_WORK *pEm, LONG pl) __asm__("func_80105498");
ULONG Atck_hit_ck(ENEMY_WORK *pEm) __asm__("func_80105738");
void Set_blood(ENEMY_WORK *pEm, UCHAR parts, ULONG type) __asm__("func_801057A4");
void Em20_Pl_to_dmg(ULONG pl, UCHAR dir) __asm__("func_8010597C");
void Em20_Pl_to_die(ULONG pl) __asm__("func_801059C0");
void Ck_Floor(ENEMY_WORK *pEm) __asm__("func_801059F4");
void Ck_Died(ENEMY_WORK *pEm) __asm__("func_80105A58");
ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm) __asm__("func_80105A80");

#endif /* OVL_EM20_H */
