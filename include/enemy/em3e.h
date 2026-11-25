#ifndef OVL_EM3E_H
#define OVL_EM3E_H
/*
 * Enemy module em3e (enemy ids 0x3E, 0x3F): a hovering machine with two
 * rotors and two side guns that flies around a fixed zone, fires when the
 * player is level with it, and crashes on a scenario request. RE2's
 * EM3E.C/EM3F.C are unrelated room props, so names are RE3 only.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "joint.h"
#include "oba.h"
#include "sub.h"

/* the enemy's free work at 0x1CC */
typedef struct tagEM3E_FREE {
    UCHAR Parts_num;                   /* 0x00 (0x1CC) parts flagged 0x400 at death */
    UCHAR Rotor0;                      /* 0x01 (0x1CD) rotor part */
    UCHAR Abr0;                        /* 0x02 (0x1CE) semi-transparent part */
    UCHAR Rotor1;                      /* 0x03 (0x1CF) rotor part (also semi-transparent) */
    UCHAR Sway_rtn;                    /* 0x04 (0x1D0) body pitch sway */
    UCHAR Bob_rtn;                     /* 0x05 (0x1D1) height bob */
    UCHAR Sway_cnt;                    /* 0x06 (0x1D2) */
    UCHAR Bob_cnt;                     /* 0x07 (0x1D3) */
    UCHAR Sway_time;                   /* 0x08 (0x1D4) */
    UCHAR Bob_time;                    /* 0x09 (0x1D5) */
    UCHAR pad0A[0x0C - 0x0A];
    USHORT Timer;                      /* 0x0C (0x1D8) explosion timer / fall speed << 4 */
    SHORT Turn;                        /* 0x0E (0x1DA) spin speed (damage); start Pos_y (crash) */
    SHORT Start_x;                     /* 0x10 (0x1DC) crash: start Pos_x */
    SHORT Fall_x;                      /* 0x12 (0x1DE) crash: x distance covered */
    USHORT Time;                       /* 0x14 (0x1E0) frames alive (+60 per hit) */
    USHORT Dir;                        /* 0x16 (0x1E2) flight direction */
    USHORT Shot_wait;                  /* 0x18 (0x1E4) */
    SHORT Tilt;                        /* 0x1A (0x1E6) body pitch from the x speed */
    SHORT Old_life;                    /* 0x1C (0x1E8) Life of the last frame */
} EM3E_FREE;

#define EM3E_W(p) ((EM3E_FREE *)(p)->Unk1CC)

extern void (*Em3e_routine_0[8])();
extern void (*Em3e_move_tbl[2])();
extern void (*Em3e_fly_tbl[2])();
extern void (*Em3e_damage_tbl[31])(); /* index weapon Damage_no - 1 */
extern void (*Em3e_sce_tbl[2])();

void Em3e_main(ENEMY_WORK *pEm);
void Em3e_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em3e_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em3e_dest_set(ENEMY_WORK *pEm);
void Em3e_fly_nop(ENEMY_WORK *pEm);
void Em3e_fly(ENEMY_WORK *pEm);
void Em3e_fly_dest(ENEMY_WORK *pEm);
void Em3e_Damage(ENEMY_WORK *pEm);
void Em3e_dm_spark0(ENEMY_WORK *pEm);
void Em3e_dm_spark(ENEMY_WORK *pEm);
void Em3e_dm_spin(ENEMY_WORK *pEm);
void Em3e_dm_none(ENEMY_WORK *pEm);
void Em3e_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em3e_sce_hover(ENEMY_WORK *pEm);
void Em3e_sce_crash(ENEMY_WORK *pEm);
void Em3e_Dead(ENEMY_WORK *pEm);
void Em3e_parts_hide(ENEMY_WORK *pEm);
void Em3e_parts_size(ENEMY_WORK *pEm, ULONG No, LONG Size);
void Em3e_parts_show(ENEMY_WORK *pEm);
void Em3e_parts_trans(ENEMY_WORK *pEm);
void Em3e_parts_trans_off(ENEMY_WORK *pEm);
void Em3e_parts_abr(ENEMY_WORK *pEm, ULONG No);
void Em3e_rotor(ENEMY_WORK *pEm);
void Em3e_shot_check(ENEMY_WORK *pEm, LONG Mode);
void Em3e_shot_r(ENEMY_WORK *pEm);
void Em3e_shot_l(ENEMY_WORK *pEm);
void Em3e_explode(ENEMY_WORK *pEm);
void Em3e_hover(ENEMY_WORK *pEm);
void Em3e_rtn_set(ENEMY_WORK *pEm, LONG Rtn0, LONG Rtn1);
void Em3e_rtn1_set(ENEMY_WORK *pEm, LONG Rtn1);
void Em3e_add_speed(ENEMY_WORK *pEm);
void Em3e_retarget(ENEMY_WORK *pEm);

#endif /* OVL_EM3E_H */
