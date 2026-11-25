#ifndef OVL_EM32_H
#define OVL_EM32_H
/*
 * Enemy module em32 (enemy id 0x32): small six-part leaping creatures (most
 * likely the Grave Digger's young worms) that come out of the ground or the
 * water, hop around the player, jump up and down floors, leap onto the
 * player's back and bite; three on the back bring the player down. They
 * respawn at their start position (Type & 0xF != 0).
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2). Structure as RE2's EM23.C: routine table
 * Init/Move/-/Damage/(exe)/Die/-/Dead, paired move tables mv_br[]/mv[]
 * (Routine_1), a player-damage table of 4 entries hooked into
 * G.pPldamage_Em[Id - 0x10].
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "joint.h"
#include "oba.h"
#include "player.h"
#include "sca.h"
#include "pad.h"
#include "pl_wep.h"

/* the module's private work in ENEMY_WORK.Unk1CC[] (RE2 Free[]) */
typedef struct tagEM32_FREE {
    SHORT Type_bak;                    /* 0x00 (0x1CC) Type & 0xF (0 with Type & 0x100): respawn type */
    SHORT Pos_x_bak;                   /* 0x02 (0x1CE) start position */
    SHORT Pos_y_bak;                   /* 0x04 (0x1D0) */
    SHORT Pos_z_bak;                   /* 0x06 (0x1D2) */
    SHORT Ground_bak;                  /* 0x08 (0x1D4) Ground */
    SHORT Cdir_y_bak;                  /* 0x0A (0x1D6) */
    SHORT Timer0_bak;                  /* 0x0C (0x1D8) */
    SHORT nFloor_bak;                  /* 0x0E (0x1DA) */
    ULONG Be_flg_bak;                  /* 0x10 (0x1DC) Be_flg & ~0x10 */
    UCHAR Floor_old;                   /* 0x14 (0x1E0) nFloor before a jump up/down */
    UCHAR pad15;
    SHORT Back_dir;                    /* 0x16 (0x1E2) place on the player's back, relative to Cdir_y */
    SHORT Red;                         /* 0x18 (0x1E4) hit flash: subtracted from the part colours */
    UCHAR Red_cnt;                     /* 0x1A (0x1E6) */
    signed char On_back;               /* 0x1B (0x1E7) on the player's back */
    SHORT Back_num;                    /* 0x1C (0x1E8) em32 works on the player's back */
    SHORT Atck_cnt;                    /* 0x1E (0x1EA) frames near the player, attack at 31 */
    SHORT Turn_timer;                  /* 0x20 (0x1EC) no turning leap while set */
    SHORT Wait_pl;                     /* 0x22 (0x1EE) wait for the player (cleared for Type 3) */
    SHORT Near_cnt;                    /* 0x24 (0x1F0) frames hopping next to the player */
    SHORT Pl_ground;                   /* 0x26 (0x1F2) player's ground height while on the back */
    signed char Can_jump;              /* 0x28 (0x1F4) may jump floors (cleared for Type 4) */
    signed char Rev;                   /* 0x29 (0x1F5) turn the other way */
} EM32_FREE;

#define EM32_W(p) ((EM32_FREE *)(p)->Unk1CC)

/* shadow polygons: colour + code word of POLY_FT4 */
#define PRIM_RGBC(p) (*(ULONG *)&(p).r0)

/* morph (sabun) data at PARTS_W.pSa_dat_head: key weights, 0x1000 = full (sabun.c Sa_morph_blend) */
typedef struct tagEM32_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C, 0x10 */
        SHORT pad;
    } Key[2];
} EM32_SABUN;
#define EM32_SABUN_P(pP) ((EM32_SABUN *)(pP)->pSa_dat_head)

extern void (*Em32_routine_0[])();
extern void (*Em32_mv_br[])();
extern void (*Em32_mv[])();
extern SHORT Em32_leap_dir[];
extern SHORT Em32_leap_spd[];
extern SHORT Em32_leap2_spd[];
extern void (*Em32_appear_tbl[])();   /* Routine_0 5 by Routine_1: out of the ground, of the water */
extern void (*Em32_pldm_tbl[])();

void Em32_main(ENEMY_WORK *pEm);
void Em32_red(ENEMY_WORK *pEm);
void Em32_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_hop_br(ENEMY_WORK *pEm);
void Em32_hop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em32_fall_ck(ENEMY_WORK *pEm);
void Em32_jump_down_spd(ENEMY_WORK *pEm);
void Em32_jump_down_br(ENEMY_WORK *pEm);
void Em32_jump_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em32_rise_ck(ENEMY_WORK *pEm);
void Em32_jump_up_spd(ENEMY_WORK *pEm);
void Em32_jump_up_br(ENEMY_WORK *pEm);
void Em32_jump_up(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_grab_br(ENEMY_WORK *pEm);
void Em32_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_leap_br(ENEMY_WORK *pEm);
void Em32_leap(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_turn_leap_br(ENEMY_WORK *pEm);
void Em32_turn_leap(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_back_br(ENEMY_WORK *pEm);
void Em32_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_on_back(ENEMY_WORK *pEm, SHORT L, SHORT Dir, VECTOR *pPos);
void Em32_drop_br(ENEMY_WORK *pEm);
void Em32_drop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_morph(ENEMY_WORK *pEm);
void Em32_part_follow(ENEMY_WORK *pEm, LONG Spd_flg, LONG Parts);
void Em32_Appear(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_appear_ground(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_appear_water(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em32_alone_ck(ENEMY_WORK *pEm, USHORT Rtn);
void Em32_Pldm(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Em32_pldm_shake(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Em32_pldm_knock(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Em32_pldm_down(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Em32_knock_off_all(void);
void Em32_back_num_set(SHORT Num);

#endif /* OVL_EM32_H */
