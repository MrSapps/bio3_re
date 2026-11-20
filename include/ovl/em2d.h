#ifndef OVL_EM2D_H
#define OVL_EM2D_H
/*
 * Enemy module em2d (enemy id 0x2D, hands reaching out of a wall/floor), the
 * successor of RE2's EM2D.C: same routine tables (Em2d_routine_0, Move_type00/01
 * br/mv tables of 8, Em2d_dm, Em2d_die, Pldamage_Em2d_tbl) and the P_hand[2][7]
 * part lists. RE3 adds Type & 0x20 (tilted, Cdir_z = -0x400: speeds are rotated
 * by Em2d_spd_rot) and a part colour fade (Em2d_hand_rgb).
 * RE2 -> RE3: Room_enemy_flg is G.Unk75EA, Timer1 UnkDA, Timer2 UnkE2, Timer3 UnkE3.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "pad.h"
#include "pl_wep.h"

/* RE2 EM2d_FREE, the enemy's Free[] at 0x1CC */
typedef struct tagEM2D_FREE {
    SHORT Dir_y;                       /* 0x00 (0x1CC) */
    SHORT Se_first;                    /* 0x02 (0x1CE) */
    SHORT Pos_x;                       /* 0x04 (0x1D0) */
    SHORT Pos_z;                       /* 0x06 (0x1D2) */
    SHORT Pos_y;                       /* 0x08 (0x1D4) RE3 */
    SHORT pad0A;                       /* 0x0A (0x1D6) */
    LONG Rgb;                          /* 0x0C (0x1D8) RE3: hand colour (Em2d_hand_rgb) */
} EM2D_FREE;

#define EF(p) ((EM2D_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 1) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
ULONG Gacha_ch(void);                                                   /* em_sub.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, frame-end flag */

#endif /* OVL_EM2D_H */
