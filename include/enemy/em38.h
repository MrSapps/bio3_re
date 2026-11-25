#ifndef OVL_EM38_H
#define OVL_EM38_H
/*
 * Enemy module em38 (enemy id 0x38): a large crawling many-part monster (most
 * likely one of Nemesis' last mutated forms) that walks between fixed targets,
 * claws, bites and swallows, spits acid, climbs into the walls, and carries the
 * two tentacles of em39 (enemy id 0x39). It rises out of the ground, swells
 * when it transforms and bursts when it dies (scenario routines).
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 * The routine frame is RE2's G-type enemy (EM34.C / EM37.C): Em_routine_0 =
 * Init/Move/Damage/Die/Scenario/-/-/Dead, Move picks Em38_Move_type[Type & 3],
 * which runs a brain routine (mv_br) and a motion routine (mv) of 16, both
 * indexed by Routine_1. Damage is indexed by the hit kind (Damage_no), the
 * scenario routines by Routine_1; PLDAMEGE_EM/PLDIE_EM tables for the player.
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "joint.h" /* PW_BOMB */
#include "em_sub.h"
#include "oba.h"
#include "main.h"
#include "pad.h"
#include "player.h"
#include "pl_wep.h"
#include "sub.h"

/* em38's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM38_FREE {
    struct tagENEMY_WORK *pTentacle0;  /* 0x00 (0x1CC) first em39 work (id 0x39) of the room */
    struct tagENEMY_WORK *pTentacle1;  /* 0x04 (0x1D0) second em39 work */
    USHORT Flg;                        /* 0x08 (0x1D4) 2 walking a root, 4/8 turning, 0x10 scenario
                                                       (no part chain), 0x20 player in sight, 0x40 dying,
                                                       0x80 outside the camera cut */
    UCHAR Atk_timer;                   /* 0x0A (0x1D6) no claw / grab / lunge while set */
    UCHAR Turn_timer;                  /* 0x0B (0x1D7) no turn while set */
    SHORT Head_tgt;                    /* 0x0C (0x1D8) head turn wanted (towards the player) */
    SHORT Head_dir;                    /* 0x0E (0x1DA) head turn */
    UCHAR Shake_no;                    /* 0x10 (0x1DC) head shake after a hit: 1 start, 2 out, 3 back */
    signed char Shake_cnt;             /* 0x11 (0x1DD) */
    signed char Shake_side;            /* 0x12 (0x1DE) +1 / -1 */
    signed char Shake_amp;             /* 0x13 (0x1DF) */
    SHORT Old_dir;                     /* 0x14 (0x1E0) Cdir_y before the routine */
    UCHAR Root_no;                     /* 0x16 (0x1E2) walk target (Em38_root[]) */
    UCHAR Wall_spit;                   /* 0x17 (0x1E3) spits left before dropping from the wall */
    UCHAR Breath;                      /* 0x18 (0x1E4) breath state (Em38_breath) */
    signed char Puff_cnt;              /* 0x19 (0x1E5) breath puff counter */
    SHORT Breath_dir;                  /* 0x1A (0x1E6) breath direction */
    UCHAR Bite_timer;                  /* 0x1C (0x1E8) no swallow while set */
    UCHAR Spit_timer;                  /* 0x1D (0x1E9) no spit while set */
    SHORT Wall_timer;                  /* 0x1E (0x1EA) no climbing while set; time on the wall */
    SHORT Lost_cnt;                    /* 0x20 (0x1EC) frames the player has been out of sight */
} EM38_FREE;

/* morph (sabun) data at PARTS_W.pSa_dat_head (sabun.c) */
typedef struct tagEM38_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;                      /* 0x0A */
    struct {
        SHORT Rate;                    /* weight of morph target n (0x1000 = full) */
        SHORT pad;
    } Key[7];                          /* 0x0C */
} EM38_SABUN;
#define EM38_SA(p) ((EM38_SABUN *)(p)->pSa_dat_head)

#define EM38_W(p) ((EM38_FREE *)(p)->Unk1CC)
/* Spd_x/Spd_y cleared as one word */
#define EM_SPD_XY(p) (*(ULONG *)&(p)->Spd_x)
/* parts swelling with the body in the transformation (module .data Em38_swell) */
typedef struct tagEM38_SWELL {
    UCHAR Num0;                        /* 0x00 parts given the size with Em38_part_scale_pos */
    UCHAR Num1;                        /* 0x01 parts given the size directly */
    UCHAR Parts[14];                   /* 0x02 part numbers */
} EM38_SWELL;

/* walk targets (module .data Em38_root, 0x20 bytes each) */
typedef struct tagEM38_ROOT {
    LONG Pos_x;                        /* 0x00 */
    LONG Pos_y;                        /* 0x04 */
    LONG Pos_z;                        /* 0x08 also the flag word of the zone below */
    SHORT Area[4][2];                  /* 0x0C x, z corners (a VCUT starting at Pos_z) */
    SHORT Dir;                         /* 0x1C direction to walk in the zone */
    SHORT pad1E;                       /* 0x1E */
} EM38_ROOT;
#define EM38_ROOT_CUT(r) ((VCUT *)&(r)->Pos_z)

extern USHORT Em38_cnt; /* counter bumped by the head sway */
extern EM38_ROOT Em38_root[];
extern SHORT Em38_box0[4]; /* hit boxes of the attacks */
extern SHORT Em38_box1[4];
extern SHORT Em38_box2[4];
extern SHORT Em38_box3[4];
extern SHORT Em38_box4[4];
extern SHORT Em38_box5[4];
extern SHORT Em38_box6[4];
extern SHORT Em38_box7[4];
extern SHORT Em38_box8[4];
extern SHORT Em38_box9[4];
extern VECTOR Em38_goal_pos;
extern VECTOR Em38_goal_pos2;
extern VCUT Em38_cut; /* zone that sets flag 0x1B */
extern void (*Em38_routine_0[])();
extern void (*Em38_Move_type[])();
extern void (*Em38_mv_br_type00[])();
extern void (*Em38_mv_type00[])();
extern void (*Em38_dm[])();
extern void (*Em38_sce[])();
extern EM38_SWELL Em38_swell;
extern signed char Em38_pulse[64]; /* size pulse of the dying body */
extern void (*Pldamage_Em38_tbl[])();
extern void (*Pldie_Em38_tbl[])();
extern KAGE_WORK *Em38_kage[]; /* shadow works of the parts */
extern COL_OBJ Em38_col[5]; /* extra collision objects */
extern MATRIX Em38_spit_m; /* effect matrix of sce04 */
extern UCHAR Em38_pulse_no; /* index into Em38_pulse (module BSS) */

void Em38_main(ENEMY_WORK *pEm);
void Em38_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_wait_br(ENEMY_WORK *pEm);
void Em38_wait(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_walk_br(ENEMY_WORK *pEm);
void Em38_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_back_br(ENEMY_WORK *pEm);
void Em38_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_turn_br(ENEMY_WORK *pEm);
void Em38_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_claw_br(ENEMY_WORK *pEm);
void Em38_claw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_swallow_br(ENEMY_WORK *pEm);
void Em38_swallow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_spit_br(ENEMY_WORK *pEm);
void Em38_spit(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_spit_far_br(ENEMY_WORK *pEm);
void Em38_spit_far(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_grab_br(ENEMY_WORK *pEm);
void Em38_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_climb_br(ENEMY_WORK *pEm);
void Em38_climb(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_drop_br(ENEMY_WORK *pEm);
void Em38_drop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_burrow_br(ENEMY_WORK *pEm);
void Em38_burrow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_wall_br(ENEMY_WORK *pEm);
void Em38_wall(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_lunge_br(ENEMY_WORK *pEm);
void Em38_lunge(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_dm_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_dm_big(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_dm_head(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_die_retreat(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_sce_rise(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_sce_swell(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_sce_burst(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_sce03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_sce_spit(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_sce_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_Dead(ENEMY_WORK *pEm);
void Pldamege_Em38(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_pldm_slam(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_pldm_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_pldm_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_pldm_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldie_Em38(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_pldie00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_pldie01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em38_chain_set(ENEMY_WORK *pEm);
void Em38_chain_turn(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Lim);
void Em38_chain_slope_end(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1);
void Em38_chain_slope(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1);
void Em38_morph_body(ENEMY_WORK *pEm);
void Em38_foot_dust(ENEMY_WORK *pEm);
void Em38_head_move(ENEMY_WORK *pEm);
void Em38_blood_head(ENEMY_WORK *pEm);
void Em38_dummy(void);
void Em38_fire_smoke(ENEMY_WORK *pEm);
void Em38_acid_spark(ENEMY_WORK *pEm);
void Em38_blast_light(ENEMY_WORK *pEm);
void Em38_breath(ENEMY_WORK *pEm);
void Em38_pustule(ENEMY_WORK *pEm);
void Em38_pustule_shell(ENEMY_WORK *pEm);
void Em38_part_scale_pos(ENEMY_WORK *pEm, PARTS_W *pP, LONG Rate);

#endif /* OVL_EM38_H */
