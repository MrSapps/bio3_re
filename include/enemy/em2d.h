#ifndef OVL_EM2D_H
#define OVL_EM2D_H
/*
 * Enemy module em2d (enemy id 0x2D, hands reaching out of a wall/floor), the
 * successor of RE2's EM2D.C: same routine tables (Em2d_routine_0, Move_type00/01
 * br/mv tables of 8, Em2d_dm, Em2d_die, Pldamage_Em2d_tbl) and the P_hand[2][7]
 * part lists. RE3 adds Type & 0x20 (tilted, Cdir_z = -0x400: speeds are rotated
 * by Em2d_spd_rot) and a part colour fade (Em2d_hand_rgb).
 * RE2 -> RE3: Room_enemy_flg is G.Room_enemy_flg, Timer1 UnkDA, Timer2 UnkE2, Timer3 UnkE3.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "joint.h"
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

extern UCHAR P_hand[2][7];
extern void (*Em2d_routine_0[])();
extern void (*Em2d_Move_type[])();
extern void (*Em2d_mv_br_type00[])();
extern void (*Em2d_mv_type00[])();
extern void (*Em2d_mv_br_type01[])();
extern void (*Em2d_mv_type01[])();
/* RE2's Em2d_sce and Pldie_Em2d_tbl are empty, so they share the address of Pldamage_Em2d_tbl */
extern void (*Pldamage_Em2d_tbl[])();
extern void (*Em2d_sce[])() __asm__("Pldamage_Em2d_tbl");
extern void (*Pldie_Em2d_tbl[])() __asm__("Pldamage_Em2d_tbl");
extern UCHAR Se_call;

void Em2d_main(ENEMY_WORK *pEm);
void Em2d_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv00_br(ENEMY_WORK *pEm);
void Em2d_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv01_br(ENEMY_WORK *pEm);
void Em2d_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv02_br(ENEMY_WORK *pEm);
void Em2d_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv03_br(ENEMY_WORK *pEm);
void Em2d_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv04_br(ENEMY_WORK *pEm);
void Em2d_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv05_br(ENEMY_WORK *pEm);
void Em2d_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv06_br(ENEMY_WORK *pEm);
void Em2d_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_mv07_br(ENEMY_WORK *pEm);
void Em2d_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void PLDIE_EM(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Hand_display(ENEMY_WORK *pEm, UCHAR L_r, UCHAR On_off);
void Em2d_hand_rgb(ENEMY_WORK *pEm, UCHAR L_r, LONG Rgb);
void PLDAMEGE_EM(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em2d_spd_rot(ENEMY_WORK *pEm, LONG Add_y, LONG Add_z);

#endif /* OVL_EM2D_H */
