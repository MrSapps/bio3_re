#ifndef OVL_EM22_H
#define OVL_EM22_H
/*
 * Enemy module em22 (enemy ids 0x22 and 0x24: the hunters). Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2). RE2 has no hunter: the module keeps RE2's
 * enemy frame (EM_MAIN, Em_routine_0 = Init/Move/Damage/Die/.../Dead, Free[]
 * work at 0x1CC) but its move logic is new: Move runs a think routine
 * (em22_think_tbl) and then an action routine (em22_move_tbl), both indexed
 * by Routine_1.
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "sca.h"
#include "sub.h"
#include "pl_wep.h"

/* em22's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM22_FREE {
    SHORT Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    UCHAR Unk03;                       /* 0x03 (0x1CF) timer, (Rnd() & 0x1F) + 30 at init */
    SHORT Start_x;                     /* 0x04 (0x1D0) jump start position */
    SHORT Start_z;                     /* 0x06 (0x1D2) */
    SHORT Down_flg;                    /* 0x08 (0x1D4) the next hit knocks it down */
    SHORT Way1_x;                      /* 0x0A (0x1D6) second way point / target of a jump */
    SHORT Way1_z;                      /* 0x0C (0x1D8) */
    UCHAR Unk0E;                       /* 0x0E (0x1DA) */
    UCHAR Parts_off;                   /* 0x0F (0x1DB) parts lost (1, 2, 4), no blood from them */
    UCHAR Dm_r2;                       /* 0x10 (0x1DC) damage reaction: Routine_2 picked by Em22_dm00 (1 flinch, 2 down) */
    UCHAR Dm_r3;                       /* 0x11 (0x1DD) damage reaction: Routine_3 */
    UCHAR Unk12;                       /* 0x12 (0x1DE) */
    UCHAR Unk13;                       /* 0x13 (0x1DF) */
    UCHAR Unk14;                       /* 0x14 (0x1E0) */
    UCHAR pad15;
    UCHAR Unk16;                       /* 0x16 (0x1E2) */
    UCHAR Unk17;                       /* 0x17 (0x1E3) */
    UCHAR Unk18;                       /* 0x18 (0x1E4) */
    signed char Cnt;                   /* 0x19 (0x1E5) frame counter of the current routine */
    UCHAR Unk1A;                       /* 0x1A (0x1E6) */
    signed char Unk1B;                 /* 0x1B (0x1E7) timer */
    SHORT Start_y;                     /* 0x1C (0x1E8) Pos_y at the start of a jump */
    SHORT Spd_add;                     /* 0x1E (0x1EA) speed added to Spd_x or Pos_y each frame (accel, gravity) */
    USHORT Unk20;                      /* 0x20 (0x1EC) */
    UCHAR Unk22;                       /* 0x22 (0x1EE) */
    UCHAR Unk23;                       /* 0x23 (0x1EF) */
    SHORT Old_spd_x;                   /* 0x24 (0x1F0) Spd_x of the last frame */
    USHORT Roar_wait;                  /* 0x26 (0x1F2) frames before the next roar (Em22_mv_roar) */
    UCHAR Unk28;                       /* 0x28 (0x1F4) */
    UCHAR Side;                        /* 0x29 (0x1F5) side picked for a jump (0/1) */
    UCHAR Unk2A;                       /* 0x2A (0x1F6) */
    UCHAR Unk2B;                       /* 0x2B (0x1F7) */
    SHORT Way0_x;                      /* 0x2C (0x1F8) first way point of a jump */
    SHORT Way0_z;                      /* 0x2E (0x1FA) */
} EM22_FREE;

#define EM22_W(p) ((EM22_FREE *)(p)->Unk1CC)
/* Routine_0..1 read as one halfword */

extern USHORT Em22_life[16];
extern USHORT Em22_life_easy[16];
extern void (*Em22_routine_0[8])();
extern void (*Em22_think_tbl[16])();
extern void (*Em22_move_tbl[16])();
extern UCHAR Em22_jump_tbl[12];
extern void (*Em22_sce_tbl[4])();
extern void (*Em22_dm_tbl[3])();
extern UCHAR Em22_dm_mask[32];
extern UCHAR Em22_dm_down_tbl[32]; /* per damage kind: 1 = knocked down (Em22_dm02) instead of a flinch */
extern UCHAR Em22_blood_parts[4];
extern signed char Em22_drop_frame[8];
extern signed char Em22_drop_parts[8];

void Em22_main(ENEMY_WORK *pEm);
void Em22_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_stand(ENEMY_WORK *pEm);
void Em22_mv_stand(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_walk(ENEMY_WORK *pEm);
void Em22_mv_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_chase(ENEMY_WORK *pEm);
void Em22_mv_chase(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_side_pos_set(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG Em22_ceil_ck(ENEMY_WORK *pEm);
LONG Em22_floor_ok_ck(ENEMY_WORK *pEm);
void Em22_th_claw(ENEMY_WORK *pEm);
void Em22_mv_claw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_front_pos_set(ENEMY_WORK *pEm, ENEMY_WORK *pT);
void Em22_th_jump_atck(ENEMY_WORK *pEm);
void Em22_mv_jump_atck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_turn(ENEMY_WORK *pEm);
void Em22_mv_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_jump(ENEMY_WORK *pEm);
void Em22_mv_jump(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em22_dir_quarter(ENEMY_WORK *pEm, LONG Flg);
void Em22_wall_way_set(ENEMY_WORK *pEm, LONG Flg, VECTOR *pV);
LONG Em22_wall_search(ENEMY_WORK *pEm);
void Em22_th_wall_jump(ENEMY_WORK *pEm);
void Em22_mv_wall_jump(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_roar(ENEMY_WORK *pEm);
void Em22_mv_roar(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_crouch(ENEMY_WORK *pEm);
void Em22_mv_crouch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_dash_atck(ENEMY_WORK *pEm);
void Em22_mv_dash_atck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_wall_climb(ENEMY_WORK *pEm);
void Em22_mv_wall_climb(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_hold_bleed(ENEMY_WORK *pEm);
void Em22_mv_hold_bleed(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_hold(ENEMY_WORK *pEm);
void Em22_mv_hold(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_th_side_jump(ENEMY_WORK *pEm);
void Em22_mv_side_jump(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em22_side_space_ck(ENEMY_WORK *pEm);
void Em22_th_throw(ENEMY_WORK *pEm);
void Em22_mv_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_sce_drop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_sce_jump_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_sce_turn_jump_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_sce_jump_path(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_land_dust(ENEMY_WORK *pEm);
void Em22_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_dm_blood(ENEMY_WORK *pEm);
void Em22_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_dm_fire(ENEMY_WORK *pEm);
void Em22_dm_acid(ENEMY_WORK *pEm);
void Em22_dm_shock(ENEMY_WORK *pEm);
void Em22_dummy(ENEMY_WORK *pEm);
void Em22_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em22_die_part_off(ENEMY_WORK *pEm);
void Em22_die_blood22(ENEMY_WORK *pEm);
void Em22_die_blood24(ENEMY_WORK *pEm);
void Em22_die_blood_side(ENEMY_WORK *pEm);
void Em22_die_limbs22(ENEMY_WORK *pEm);
void Em22_die_limbs24(ENEMY_WORK *pEm);
void Em22_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldie_Em22(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Pldamege_Em24(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Pldie_Em24(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Em22_water(ENEMY_WORK *pEm);
UCHAR Em22_pl_move_ck(void);
LONG Em22_rtn_free_ck(ENEMY_WORK *pEm, ULONG Rtn);
LONG Em22_pl_weak_ck(void);
LONG Em24_esp_seq(ENEMY_WORK *pEm);
void Em22_foot_se(ENEMY_WORK *pEm);
LONG Em22_floor_ck4(ENEMY_WORK *pEm);

#endif /* OVL_EM22_H */
