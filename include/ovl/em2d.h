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

extern UCHAR P_hand[2][7] __asm__("D_80101514");
extern void (*Em2d_routine_0[])() __asm__("D_80101524");
extern void (*Em2d_Move_type[])() __asm__("D_80101544");
extern void (*Em2d_mv_br_type00[])() __asm__("D_8010154C");
extern void (*Em2d_mv_type00[])() __asm__("D_8010156C");
extern void (*Em2d_mv_br_type01[])() __asm__("D_8010158C");
extern void (*Em2d_mv_type01[])() __asm__("D_801015AC");
/* RE2's Em2d_sce and Pldie_Em2d_tbl are empty, so they share the address of Pldamage_Em2d_tbl */
extern void (*Em2d_sce[])() __asm__("D_80101614");
extern void (*Pldie_Em2d_tbl[])() __asm__("D_80101614");
extern void (*Pldamage_Em2d_tbl[])() __asm__("D_80101614");
extern UCHAR Se_call __asm__("D_80101624");

void func_80100020(ENEMY_WORK *pEm);
void Em2d_Init(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq) __asm__("func_801000DC");
void func_80100334(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80100374(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801003F8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010047C(ENEMY_WORK *pEm);
void func_80100554(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80100780(ENEMY_WORK *pEm);
void func_80100800(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Em2d_mv02_br(ENEMY_WORK *pEm) __asm__("func_80100A00");
void Em2d_mv02(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq) __asm__("func_80100A08");
void func_80100A10(ENEMY_WORK *pEm);
void func_80100AEC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80100BB4(ENEMY_WORK *pEm);
void func_80100BBC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80100E70(ENEMY_WORK *pEm);
void func_80100E78(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80101008(ENEMY_WORK *pEm);
void func_80101010(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801010AC(ENEMY_WORK *pEm);
void func_801010B4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801010BC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Em2d_dm00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq) __asm__("func_801010DC");
void func_8010112C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Em2d_die00(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq) __asm__("func_8010114C");
void func_801011B0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801011EC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801011F4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801011FC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Hand_display(ENEMY_WORK *pEm, UCHAR L_r, UCHAR On_off) __asm__("func_80101238");
void Em2d_hand_rgb(ENEMY_WORK *pEm, UCHAR L_r, LONG Rgb) __asm__("func_801012F4");
void PLDAMEGE_EM(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq) __asm__("func_80101350");
void func_8010138C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Em2d_spd_rot(ENEMY_WORK *pEm, LONG Add_y, LONG Add_z) __asm__("func_801014AC");

#endif /* OVL_EM2D_H */
