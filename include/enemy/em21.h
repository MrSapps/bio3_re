#ifndef OVL_EM21_H
#define OVL_EM21_H
/*
 * Enemy module em21 (crow, enemy id 0x21), the successor of RE2's EM21.C:
 * same routine tables (Em21_routine_0, indoor/outdoor br/mv tables of 15,
 * Em21_dm, Em21_sce_mv, Em21_dead_mv, Pldamage_Em21_tbl).
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "mojidisp.h"
#include "pad.h"
#include "player.h"
#include "pl_wep.h"

/* em21's view of ENEMY_WORK.Unk1CC[] (RE2 struct tagEM21F at ENEMY_FREE(pEm), EF->) */
typedef struct tagEM21_FREE {
    UCHAR Tmp;                         /* 0x00 (0x1CC) */
    UCHAR Cnt;                         /* 0x01 (0x1CD) */
    UCHAR Cnt2;                        /* 0x02 (0x1CE) */
    UCHAR Cnt3;                        /* 0x03 (0x1CF) */
    UCHAR Na_cnt;                      /* 0x04 (0x1D0) */
    UCHAR Sca_cnt;                     /* 0x05 (0x1D1) */
    UCHAR Obj_cnt;                     /* 0x06 (0x1D2) */
    UCHAR Idly_flg;                    /* 0x07 (0x1D3) */
    UCHAR Idly_cnt;                    /* 0x08 (0x1D4) */
    signed char Pl_dead_cnt;           /* 0x09 (0x1D5) */
    UCHAR Atck;                        /* 0x0A (0x1D6) RE2 Dummy1: non-zero while the peck hit area is live */
    UCHAR Atck_se;                     /* 0x0B (0x1D7) attack cry played (once per attack run) */
    SHORT Tmp_0;                       /* 0x0C (0x1D8) */
    SHORT Tmp_1;                       /* 0x0E (0x1DA) */
    SHORT Tmp_2;                       /* 0x10 (0x1DC) */
    USHORT Flg;                        /* 0x12 (0x1DE) */
    SHORT Old_x;                       /* 0x14 (0x1E0) RE2 Padd1: position of the last frame */
    SHORT Old_y;                       /* 0x16 (0x1E2) */
    SHORT Old_z;                       /* 0x18 (0x1E4) */
} EM21_FREE;

#define EF(p) ((EM21_FREE *)(p)->Unk1CC)

/* keep a coordinate inside +-l (scenario flight paths) */
#define EM21_LIMIT(v, l) ((v) < -(l) ? -(l) : ((v) > (l) ? (l) : (v)))

extern SHORT Em21_peck_box0[4];
extern SHORT Em21_peck_box1[4];
extern void (*Em21_routine_0[])();
extern void (*Em21_indoor_br[])();
extern void (*Em21_indoor_mv[])();
extern void (*Em21_outdoor_br[])();
extern void (*Em21_outdoor_mv[])();
extern void (*Em21_dm[])();
extern void (*Em21_sce_mv[])();
extern void (*Em21_dead_mv[])();
extern void (*Pldamage_Em21_tbl[])();
extern HIT_AREA Em21_hit_area;

void Em21_main(ENEMY_WORK *pEm);
void Em21_kage_set(ENEMY_WORK *pEm);
void Em21_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_wait_i(ENEMY_WORK *pEm);
void Em21_mv_wait_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_go_wait_i(ENEMY_WORK *pEm);
void Em21_mv_go_wait_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_take_off_i(ENEMY_WORK *pEm);
void Em21_mv_take_off_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_glide_i(ENEMY_WORK *pEm);
void Em21_mv_glide_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_flight_i(ENEMY_WORK *pEm);
void Em21_mv_flight_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_up_i(ENEMY_WORK *pEm);
void Em21_mv_up_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_down_i(ENEMY_WORK *pEm);
void Em21_mv_down_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_landing_i(ENEMY_WORK *pEm);
void Em21_mv_landing_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_quick_turn_i(ENEMY_WORK *pEm);
void Em21_mv_quick_turn_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_jump_i(ENEMY_WORK *pEm);
void Em21_mv_jump_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_fall_i(ENEMY_WORK *pEm);
void Em21_mv_fall_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_crash_f_i(ENEMY_WORK *pEm);
void Em21_mv_crash_f_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_crash_i(ENEMY_WORK *pEm);
void Em21_mv_crash_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_peck_f_i(ENEMY_WORK *pEm);
void Em21_mv_peck_f_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_br_peck_i(ENEMY_WORK *pEm);
void Em21_mv_peck_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_parts_bobm_set(PARTS_W *pP, LONG B_y);
void Em21_parts_fall_set(PARTS_W *pP, LONG D_y);
void Em21_dm03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_sce_wait(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_sce_crash(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_sce_fly_away(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_sce_circle(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_sce_hop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_sce_fly_up(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_dead_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_dead_bomb(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_dead_instant(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_foot_set(ENEMY_WORK *pEm);
void Em21_r0_set(ENEMY_WORK *pEm, ULONG Rno0, ULONG Rno1);
void Em21_r1_set(ENEMY_WORK *pEm, ULONG Rno1);
void Em21_sca_avoid_ck(ENEMY_WORK *pEm);
void Em21_attack_ck(ENEMY_WORK *pEm);
void Em21_run_away_ck(ENEMY_WORK *pEm);
void Em21_pl_dead_ck(ENEMY_WORK *pEm);
void Em21_lock_off_ck(ENEMY_WORK *pEm);
void Em21_set_flg_all(ULONG Mask);
void Em_21_blood_call(ENEMY_WORK *pEm);
void Em_21_feather_call(ENEMY_WORK *pEm, LONG Num);
void Em21_atd_set(ENEMY_WORK *pEm, LONG Size);
void Em21_attacker_off(void);
void Pldamege_Em21(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em21_dummy_br(ENEMY_WORK *pEm);
void Em21_dummy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

#endif /* OVL_EM21_H */
