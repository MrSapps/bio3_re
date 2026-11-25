#ifndef OVL_EM23_H
#define OVL_EM23_H
/*
 * Enemy module em23 (enemy ids 0x23 and 0x28: Drain Deimos / Brain Sucker, the
 * flea-like enemy that walks on floors, walls and ceilings). RE3-specific, but
 * built on the RE2 enemy skeleton (EM_MAIN with Root_ck/Guide_check as RE2
 * EM10.C, Em_routine_0[8] = Init/Move/Damage/Die/Scenario/-/-/Dead, a pair of
 * br/mv tables of 22 routines selected by Routine_1, damage/die tables indexed
 * by the hit kind (Damage_no) and Routine_1, PLDAMEGE_EM/PLDIE_EM for the player
 * held by it). Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep.h"
#include "player.h"
#include "main.h"
#include "sca.h"

/* em23's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM23_FREE {
    signed char Side;                  /* 0x00 (0x1CC) wall side: bit 0 wall along x (SCA test), bit 1 far side */
    signed char Unk01;                 /* 0x01 (0x1CD) */
    signed char Mode;                  /* 0x02 (0x1CE) surface: 0 floor .. 4 ceiling */
    signed char Unk03;                 /* 0x03 (0x1CF) */
    SHORT Wall_dir;                    /* 0x04 (0x1D0) direction of travel in the wall plane */
    SHORT Base_dir_x;                  /* 0x06 (0x1D2) base body direction (Cdir_x) on a wall along z */
    SHORT Base_dir_y;                  /* 0x08 (0x1D4) base body direction (Cdir_y) on a wall along x */
    SHORT Unk0A;                       /* 0x0A (0x1D6) */
    SHORT Floor_y;                     /* 0x0C (0x1D8) room floor height (SCA_HEAD +0xC) */
    SHORT Ground;                      /* 0x0E (0x1DA) ground height under the enemy */
    signed char Flg;                   /* 0x10 (0x1DC) 8: root search active, & 7: next state */
    UCHAR Unk11;                       /* 0x11 (0x1DD) */
    SHORT Wall_x;                      /* 0x12 (0x1DE) wall point to stick to */
    SHORT Wall_z;                      /* 0x14 (0x1E0) */
    SHORT Step_y;                      /* 0x16 (0x1E2) vertical step of a fall / jump (doubles each frame up to 800) */
    SHORT Timer;                       /* 0x18 (0x1E4) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    struct tagSCA_DATA *pSca;          /* 0x1C (0x1E8) wall shape to climb (copy of pSca_hit_data) */
    signed char Unk20;                 /* 0x20 (0x1EC) */
    signed char Unk21;                 /* 0x21 (0x1ED) */
    SHORT Jump_x;                      /* 0x22 (0x1EE) target of the jump onto the wall (Em23_mv12) */
    SHORT Jump_z;                      /* 0x24 (0x1F0) */
    SHORT Step_x;                      /* 0x26 (0x1F2) per-frame step of a jump */
    SHORT Step_z;                      /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
    USHORT Leg_off;                    /* 0x2C (0x1F8) legs torn off (bits 0 and 1, one per side) */
    SHORT Atck_wait;                   /* 0x2E (0x1FA) frames before the next attack (id 0x28) */
} EM23_FREE;

#define EF(p) ((EM23_FREE *)(p)->Unk1CC)
/* bytes 1..2 of the free area tested as one word */
#define EF_W0(p) ((p)->Unk1CC[0])
/* Size/Status_flg read as one word */
#define EM_D0W(p) (*(ULONG *)&(p)->Size)
/* SCA_HEAD +0xC (sca.h padC): floor height of the room */
#define SCA_FLOOR_Y(p) (((SHORT *)(p))[6])
/* x/z pair packed for the SCA line tests (sca.c func_8004F258) */
#define SCA_XZ(x, z) ((((x) >> 1) << 16) | (((z) >> 1) & 0xFFFF))
/* body direction on the wall from the base direction (Base_dir_x/Base_dir_y) and the wall direction Wall_dir;
   the _T form also clears Timer0 */
#define EM23_WALL_DIR_(p, T)                                                  \
    {                                                                         \
        UCHAR Side_ = EF(p)->Side;                                           \
        T;                                                                    \
        if (Side_ & 1) {                                                      \
            if (Side_ & 2)                                                    \
                (p)->Cdir_y = EF(p)->Base_dir_y + EF(p)->Wall_dir;                    \
            else                                                              \
                (p)->Cdir_y = EF(p)->Base_dir_y - EF(p)->Wall_dir;                    \
        } else {                                                              \
            (p)->Cdir_x = EF(p)->Base_dir_x + EF(p)->Wall_dir;                        \
        }                                                                     \
    }
#define EM23_WALL_DIR(p) EM23_WALL_DIR_(p, (void)0)
#define EM23_WALL_DIR_T(p) EM23_WALL_DIR_(p, (p)->Timer0 = 0)

extern USHORT Em23_life[16];
extern USHORT Em23_life_easy[16];
extern void (*Em23_routine_0[])();
extern void (*Em23_mv_br[])();
extern void (*Em23_mv[])();
extern void (*Em23_mv01_br_tbl[])();
extern void (*Em23_mv02_br_tbl[])();
extern void (*Em23_dm[][3])();
extern void (*Em23_die[][3])();
extern void (*Em23_sce[])();
extern void (*Pldamage_Em23_tbl[])();
extern void (*Pldie_Em23_tbl[])();

void Em23_main(ENEMY_WORK *pEm);
void Em23_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_bubble_set(ENEMY_WORK *pEm);
void Em23_seq_sound(ENEMY_WORK *pEm);
void Em23_kage_set(ENEMY_WORK *pEm);
void Em23_mv00_br(ENEMY_WORK *pEm);
void Em23_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv01_br(ENEMY_WORK *pEm);
void Em23_goto_dest(ENEMY_WORK *pEm);
void Em23_mv01_br_lead(ENEMY_WORK *pEm);
void Em23_mv01_br_follow(ENEMY_WORK *pEm);
void Em23_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv02_br(ENEMY_WORK *pEm);
void Em23_mv02_br_lead(ENEMY_WORK *pEm);
void Em23_mv02_br_follow(ENEMY_WORK *pEm);
void Em23_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em23_wall_climb_ck(ENEMY_WORK *pEm, SCA_DATA *pS);
LONG Em23_wall_probe_up(ENEMY_WORK *pEm, LONG Dist, LONG Up, LONG Side);
LONG Em23_edge_probe(ENEMY_WORK *pEm, LONG Dist, LONG Down);
LONG Em23_wall_probe_turn(ENEMY_WORK *pEm, LONG Dist, LONG Dir, LONG Flg);
LONG Em23_wall_probe_ahead(ENEMY_WORK *pEm, LONG Dist, LONG Dir);
LONG Em23_floor_probe(ENEMY_WORK *pEm, LONG Dist);
LONG Em23_floor_probe_box(ENEMY_WORK *pEm, LONG Dist);
LONG Em23_front_probe(ENEMY_WORK *pEm, LONG Dist);
void Em23_wall_base_dir(ENEMY_WORK *pEm);
void Em23_wall_stick(ENEMY_WORK *pEm);
void Em23_wall_stick_at(ENEMY_WORK *pEm, LONG Dir);
void Em23_wall_point(ENEMY_WORK *pEm);
void Em23_wall_move(ENEMY_WORK *pEm, LONG Add);
void Em23_wall_turn(ENEMY_WORK *pEm, LONG X, LONG Z, LONG Add);
void Em23_mv03_br(ENEMY_WORK *pEm);
void Em23_mv03_br_root(ENEMY_WORK *pEm);
void Em23_mv03_br_free(ENEMY_WORK *pEm);
void Em23_wall_ofs_set(ENEMY_WORK *pEm);
void Em23_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_ceil_base_dir(ENEMY_WORK *pEm);
void Em23_mv04_br(ENEMY_WORK *pEm);
void Em23_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv05_br(ENEMY_WORK *pEm);
void Em23_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv06_br(ENEMY_WORK *pEm);
void Em23_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv07_br(ENEMY_WORK *pEm);
void Em23_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv08_br(ENEMY_WORK *pEm);
void Em23_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv09_br(void);
void Em23_mv09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv0a_br(void);
void Em23_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv0b_br(ENEMY_WORK *pEm);
void Em23_mv0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv0c_br(void);
void Em23_mv0c(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_jump_target_set(ENEMY_WORK *pEm);
void Em23_mv0d_br(ENEMY_WORK *pEm);
void Em23_mv0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv0e_br(ENEMY_WORK *pEm);
void Em23_mv0e(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv0e_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv0e_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv0f_br(void);
void Em23_mv0f(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv10_br(void);
void Em23_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv11_br(ENEMY_WORK *pEm);
void Em23_mv11(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv12_br(void);
void Em23_mv12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv13_br(void);
void Em23_mv13(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv14_br(void);
void Em23_mv14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_mv15_br(void);
void Em23_mv15(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_esp_front(ENEMY_WORK *pEm, SHORT Dir);
LONG Em23_rtn_free_ck(ENEMY_WORK *pEm, USHORT Rtn0, USHORT Rtn1);
void Em23_root_leader_set(ENEMY_WORK *pEm);
void Em23_root_stop(ENEMY_WORK *pEm); /* takes pEm; callers pass it implicitly (a0 still holds it) */
void Em23_water(ENEMY_WORK *pEm);
void Em23_col_ofs_set(ENEMY_WORK *pEm);
void Em23_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_dm_status(ENEMY_WORK *pEm);
void Em23_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_dm_flash(ENEMY_WORK *pEm);
void Em23_dm_blood3(ENEMY_WORK *pEm);
void Em23_dm_blood_side(ENEMY_WORK *pEm);
void Em23_leg_off(ENEMY_WORK *pEm);
void Em23_claw_off(ENEMY_WORK *pEm);
void Em23_die_blood(ENEMY_WORK *pEm);
void Em23_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_die_flash(ENEMY_WORK *pEm);
void Em23_die_blood3(ENEMY_WORK *pEm);
void Em23_die_blood_side(ENEMY_WORK *pEm);
void Em23_die_leg_off(ENEMY_WORK *pEm);
void Em23_die01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pldamege_Em23(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void Em23_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_pldm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_pldm02(void);
void Pldie_Em23(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_pldie00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em23_child_set(ENEMY_WORK *pEm);
ENEMY_WORK **Em23_child_find(ENEMY_WORK **ppWork);

#endif /* OVL_EM23_H */
