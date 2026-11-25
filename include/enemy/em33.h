#ifndef OVL_EM33_H
#define OVL_EM33_H
/*
 * Enemy module em33 (enemy id 0x33): the Grave Digger again, in a room
 * with water and holes in the walls. Same work layout as em30 (enemy/em30_free.h)
 * and a dozen shared routines (src/enemy/common/em30): the head work
 * (Type & 1 clear) allocates seven segment works (Type & 1 set) that follow
 * parts of the head's model and carry their own collision nodes and shadows.
 * Em33_mv_br/Em33_mv by Routine_1: 0/2/5 wait, 1 rise, 3 swim, 4, 6 dead,
 * 7, 8 leap out and bite, 9, 10 thrash. Em33_clip_tbl[Clip_no] hides the parts
 * that are inside a wall. RE2's EM33.C (licker family) is unrelated.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em30_free.h"
#include "em_sub.h"
#include "oba.h"
#include "player.h"
#include "main.h"
#include "sub.h"
#include "pad.h"
#include "pl_wep.h"

/* the player work as G lays it out, followed by the work list: em33 reaches G.pEnemy[] from
   pPl = &Pl_work (one base register; G.pEnemy itself is a second address) */
typedef struct tagEM33_PL {
    PLAYER_WORK Player;                /* Pl_work = G.Player */
    SHORT Pl_poison_timer;             /* 0x1CC G.Pl_poison_timer */
    UCHAR pad1CE[0x1D0 - 0x1CE];
    ENEMY_WORK *pWork[42];             /* 0x1D0 = G.pEnemy */
} EM33_PL;

/* Spd_z by Move_cnt / 10 while surfacing (copied to the stack as one block) */
typedef struct tagEM33_SPD_TBL {
    SHORT Spd[8];
} EM33_SPD_TBL;

extern const EM33_SPD_TBL Em33_spd_z_tbl; /* { -10, 14, -20, 28, -24, 20, -16, 8 } */
extern USHORT Em33_vib_tbl[]; /* pad vibration scripts */
extern void (*Em33_routine_0[8])();
extern USHORT Em33_col_size[10]; /* body collision W0/D0 by segment */
extern void (*Em33_mv_br[11])();
extern void (*Em33_mv[11])();
extern void (*Em33_clip_tbl[5])();

void Em33_main(ENEMY_WORK *pEm);
void Em33_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_sub_init(ENEMY_WORK *pEm, LONG No, ENEMY_WORK *pOya, LONG Parts);
void Em30_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_Move_type(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_wait_br(ENEMY_WORK *pEm);
void Em33_nop(ENEMY_WORK *pEm);
void Em30_body_chain(ENEMY_WORK *pEm);
void Em30_part_hang(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
void Em30_part_hang_sink(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
void Em33_body_sink(ENEMY_WORK *pEm);
LONG Em33_morph_body(ENEMY_WORK *pEm);
void Em30_morph_step(ENEMY_WORK *pEm, LONG Step, LONG Dir);
void Em33_fade_in(ENEMY_WORK *pEm, LONG Cnt);
void Em33_rise_br(ENEMY_WORK *pEm);
void Em33_rise(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_swim_br(ENEMY_WORK *pEm);
void Em33_swim(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_mv04_br(ENEMY_WORK *pEm);
void Em33_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_splash(ENEMY_WORK *pEm);
void Em33_dead_br(ENEMY_WORK *pEm);
void Em33_dead(ENEMY_WORK *pEm);
void Em33_parts_reset(ENEMY_WORK *pEm);
void Em33_parts_hide(ENEMY_WORK *pEm);
void Em33_mv07_br(ENEMY_WORK *pEm);
void Em33_zone_set(ENEMY_WORK *pEm, LONG Flg);
LONG Em33_zone_ck(ENEMY_WORK *pEm);
void Em33_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_leap_br(ENEMY_WORK *pEm);
void Em33_leap(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_mv09_br(ENEMY_WORK *pEm);
void Em33_mv09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_thrash_br(ENEMY_WORK *pEm);
void Em33_thrash(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_clip_wall(ENEMY_WORK *pEm);
void Em33_clip(ENEMY_WORK *pEm);
void Em33_clip_x_min(ENEMY_WORK *pEm);
void Em33_clip_x_max(ENEMY_WORK *pEm);
void Em33_clip_z_max(ENEMY_WORK *pEm);
void Em33_clip_z_min(ENEMY_WORK *pEm);
void Em33_clip_none(ENEMY_WORK *pEm);
void Em33_part_col_set(ENEMY_WORK *pEm, LONG No);
void Em30_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_dm_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_dm_dive(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_dm_heavy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dm_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em33_blood_set(ENEMY_WORK *pEm);
void Em30_dirt_hit(ENEMY_WORK *pEm);
void Em30_dirt_line(ENEMY_WORK *pEm);
void Em30_body_number(ENEMY_WORK *pEm);
void Em30_dirt_steps(ENEMY_WORK *pEm);
void Em30_dirt_set(ENEMY_WORK *pEm, LONG No, LONG Type);
void Em33_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_se_set(ULONG No, ENEMY_WORK *pEm, LONG *pPos);
void Em33_vib_rise(void);
void Em33_vib_leap(void);

#endif /* OVL_EM33_H */
