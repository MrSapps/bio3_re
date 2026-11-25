#ifndef OVL_EM36_H
#define OVL_EM36_H
/*
 * Enemy module em36 (enemy ids 0x36 and 0x3A: Nemesis). Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2). The predecessor is RE2's Tyrant module
 * EM2A.C: Move runs a "brain" routine (Em2a_mv_br) and then a motion routine
 * (Em2a_mv), both indexed by Routine_1; RE3 picks the brain/motion tables by
 * Type (Em36_Move_type).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "sub.h"
#include "pl_wep.h"
#include "joint.h"

/* em36's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM36_FREE {
    struct tagENEMY_WORK *pSub;        /* 0x00 (0x1CC) second work moved with this one (Routine_0..2, Move_cnt, Timer0 set by the attacks) */
    USHORT Flg;                        /* 0x04 (0x1D0) 0x800: player in sight, 0x1000: on a route */
    UCHAR Mv12_timer;                  /* 0x06 (0x1D2) no move 12 while set */
    UCHAR Dodge_timer;                 /* 0x07 (0x1D3) side step timer, bit 7: armed (Em36_dodge) */
    UCHAR Punch_timer;                 /* 0x08 (0x1D4) no punch while set */
    UCHAR Grab_timer;                  /* 0x09 (0x1D5) no grab while set */
    SHORT Type_timer;                  /* 0x0A (0x1D6) frames until Type changes */
    UCHAR Shake_no;                    /* 0x0C (0x1D8) head shake after a hit (Em36_head_shake) */
    signed char Shake_cnt;             /* 0x0D (0x1D9) */
    signed char Shake_side;            /* 0x0E (0x1DA) +1 / -1 */
    signed char Shake_amp;             /* 0x0F (0x1DB) */
    UCHAR Mouth;                       /* 0x10 (0x1DC) mouth morph state (Em36_mouth_morph) */
    UCHAR Unk11;                       /* 0x11 (0x1DD) */
    UCHAR pad12[0x14 - 0x12];
    UCHAR Lunge_timer;                 /* 0x14 (0x1E0) no lunge while set */
    UCHAR Dm_stage;                    /* 0x15 (0x1E1) parts lost so far (Em36_dm_part) */
    SHORT Morph_spd;                   /* 0x16 (0x1E2) body morph speed (Em36_main) */
    SHORT Stage_life;                  /* 0x18 (0x1E4) Life of the next damage stage */
    SHORT Mv06_timer;                  /* 0x1A (0x1E6) */
    UCHAR Lost_time;                   /* 0x1C (0x1E8) frames left going to the last seen player position */
    UCHAR Combo_no;                    /* 0x1D (0x1E9) step in the combo (Em36_combo_tbl) */
    SHORT Lost_dir;                    /* 0x1E (0x1EA) direction to Dest_x/z */
    UCHAR Combo_set;                   /* 0x20 (0x1EC) combo row (Em36_combo_tbl) */
    UCHAR Route_no;                    /* 0x21 (0x1ED) waypoint index (Em36_route) */
    SHORT Hit_cnt;                     /* 0x22 (0x1EE) attack counter */
    SHORT Route_dist;                  /* 0x24 (0x1F0) distance to the waypoint */
    SHORT Route_dist2;                 /* 0x26 (0x1F2) distance to the route point (Em36_main) */
    UCHAR Route_pt;                    /* 0x28 (0x1F4) point on the route (Em36_route[][Route_pt]) */
    UCHAR Sweep_timer;                 /* 0x29 (0x1F5) no arm sweep while set */
    UCHAR Run_timer;                   /* 0x2A (0x1F6) no running while set */
} EM36_FREE;

/* morph (sabun) data at PARTS_W.pSa_dat_head: key weights, 0x1000 = full (sabun.c Sa_morph_blend) */
typedef struct tagEM36_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C + 4 * n */
        SHORT pad;
    } Key[7];
} EM36_SABUN;
#define EM36_SABUN_P(pP) ((EM36_SABUN *)(pP)->pSa_dat_head)

/* PARTS_W free work of the morphing body parts (Sa_morph_step blends keys Key_no - 1 and Key_no) */
typedef struct tagEM36_PARTS_FREE {
    UCHAR Key_no;                      /* 0x74 */
    UCHAR Unk01;                       /* 0x75 */
    SHORT Unk02;                       /* 0x76 */
    SHORT Unk04;                       /* 0x78 */
} EM36_PARTS_FREE;
#define EM36_PF(p) ((EM36_PARTS_FREE *)&(p)->Unk74)
#define EM36_W(p) ((EM36_FREE *)(p)->Unk1CC)
/* Flg read as a word together with the bytes after it */
#define EM36_FLGW(p) (*(ULONG *)&EM36_W(p)->Flg)
/* colour word (r, g, b, code) of the shadow quads */
#define KAGE_RGB(pK, n) (*(ULONG *)&(pK)->Prim[n].r0)

extern SHORT Em36_punch_box0[4];
extern SHORT Em36_punch_box1[4];
extern SHORT Em36_lunge_box0[4];
extern SHORT Em36_lunge_box1[4];
extern SHORT Em36_grab_box0[4];
extern SHORT Em36_grab_box1[4];
extern SHORT Em36_sweep_box0[4];
extern SHORT Em36_sweep_box1[4];
extern SHORT Em36_sweep_box2[4];
extern SHORT Em36_sweep_box3[4];
extern SHORT Em36_swing_box0[4];
extern SHORT Em36_swing_box1[4];
extern VECTOR Em36_route_pos[2];
extern VECTOR Em36_route[][3];
extern void (*Em36_routine_0[8])();
extern VECTOR Em36_lost_pos;
extern void (*Em36_Move_type[14])();
extern void (*Em36_mv_br00[21])();
extern void (*Em36_mv00[21])();
extern void (*Em36_mv_br01[21])();
extern void (*Em36_mv01[21])();
extern void (*Em36_mv_br02[21])();
extern void (*Em36_mv_br03[21])();
extern void (*Em36_mv03[21])();
extern void (*Em36_mv_br04[21])();
extern void (*Em36_mv_br05[21])();
extern UCHAR Em36_combo_tbl[8][8];
extern void (*Em36_dm[32][3])();
extern void (*Em36_dm2[33])();
extern void (*Em36_sce[12])();
extern void (*Pldamage_Em36_tbl[4])();
extern void (*Pldie_Em36_tbl[1])();
extern HIT_AREA Em36_hit_area;

void Em36_main(ENEMY_WORK *pEm);
void Em36_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Move_type12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_stand_br(ENEMY_WORK *pEm);
void Em36_stand(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_walk_br(ENEMY_WORK *pEm);
void Em36_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_walk_br01(ENEMY_WORK *pEm);
void Em36_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_run_br(ENEMY_WORK *pEm);
void Em36_combo_br(ENEMY_WORK *pEm);
void Em36_run_lost(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_route_br04(ENEMY_WORK *pEm);
void Em36_route_br05(ENEMY_WORK *pEm);
void Em36_turn_br(ENEMY_WORK *pEm);
void Em36_turn_br02(ENEMY_WORK *pEm);
void Em36_turn_br04(ENEMY_WORK *pEm);
void Em36_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_back_br(ENEMY_WORK *pEm);
void Em36_back_br04(ENEMY_WORK *pEm);
void Em36_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_turn_around_br(ENEMY_WORK *pEm);
void Em36_turn_around(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_punch_br(ENEMY_WORK *pEm);
void Em36_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_mv06_br(ENEMY_WORK *pEm);
void Em36_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_punch_effect(ENEMY_WORK *pEm);
void Em36_roar_br(ENEMY_WORK *pEm);
void Em36_roar(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_stand_up_br(ENEMY_WORK *pEm);
void Em36_stand_up(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_side_step_br(ENEMY_WORK *pEm);
void Em36_side_step(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_lunge_br(ENEMY_WORK *pEm);
void Em36_lunge(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_squeeze_br(ENEMY_WORK *pEm);
void Em36_squeeze(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_mv12_br(ENEMY_WORK *pEm);
void Em36_mv12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_grab_br(ENEMY_WORK *pEm);
void Em36_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_mv14_br(ENEMY_WORK *pEm);
void Em36_mv14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sweep_br(ENEMY_WORK *pEm);
void Em36_sweep(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_mv16_br(ENEMY_WORK *pEm);
void Em36_mv16(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_swing_br(ENEMY_WORK *pEm);
void Em36_swing(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_swing_arm_br(ENEMY_WORK *pEm);
void Em36_swing_arm(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_run_punch_br(ENEMY_WORK *pEm);
void Em36_run_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_route_br(ENEMY_WORK *pEm);
void Em36_route_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_dm_flinch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_dm_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_dm_light(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_dm_part(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_dm_knock(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_parts_effect(ENEMY_WORK *pEm);
void Em36_blow_off6(ENEMY_WORK *pEm);
void Em36_blow_off22(ENEMY_WORK *pEm);
void Em36_blow_off23(ENEMY_WORK *pEm);
void Em36_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_die_burn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_stand(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_motion(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_motion_stop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em34_sce05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_jump(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_leap(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_jump_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_die_fall(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_die_blow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_sce_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldamege_Em36(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_pldm_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_pldm_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_dummy0(void);
void Pldie_Em36(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em36_dummy1(void);
void Em36_dodge(ENEMY_WORK *pEm);
void Em36_head_shake(ENEMY_WORK *pEm);
void Em34_foot_set(ENEMY_WORK *pEm, LONG Unused, LONG Leg);
void Em34_blood_set(ENEMY_WORK *pEm);
void Em36_dummy2(void);
void Em36_knock_back(ENEMY_WORK *pEm);
void Em34_blood_spray(ENEMY_WORK *pEm);
void Em36_knock_back2(ENEMY_WORK *pEm);
void Em36_mouth_morph(ENEMY_WORK *pEm);
void Em36_body_morph(ENEMY_WORK *pEm);
void Em36_wound_fx(ENEMY_WORK *pEm);
void Em36_wound_fx2(ENEMY_WORK *pEm);
void Em36_wound_fx3(ENEMY_WORK *pEm);

#endif /* OVL_EM36_H */
