#ifndef OVL_EM39_H
#define OVL_EM39_H
/*
 * Enemy module em39 (enemy id 0x39): a tentacle-like part attached to the work
 * of enemy id 0x38 (its parent); it grows out of the parent (part Size_y),
 * sways and lashes at the player. Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "pad.h"
#include "player.h"

/* em39's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM39_FREE {
    ENEMY_WORK *pParent;               /* 0x00 (0x1CC) the enemy (id 0x38) this part belongs to */
    USHORT Wait;                       /* 0x04 (0x1D0) frames before the next attack */
    USHORT Flg;                        /* 0x06 (0x1D2) 1: growing, size is not updated */
    SHORT Dir;                         /* 0x08 (0x1D4) sway angle, -0x100..0x100 */
} EM39_FREE;

#define EM39_W(p) ((EM39_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)

/* hit box test around a point (pl_wep.c func_800445C8) */
typedef struct tagHIT_AREA {
    VECTOR *pPos;                      /* 0x00 */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} HIT_AREA;

/* module .data */
extern SHORT Em39_box0[4] __asm__("D_801013A4");        /* hit boxes of the lash */
extern SHORT Em39_box1[4] __asm__("D_801013AC");
extern SHORT Em39_box2[4] __asm__("D_801013B4");
extern void (*Em39_routine_0[])() __asm__("D_801013BC");
extern void (*Em39_move_type[])() __asm__("D_801013E8");
extern void (*Em39_mv_br[])() __asm__("D_801013F0");
extern void (*Em39_mv[])() __asm__("D_8010141C");
extern void (*Em39_mv_br_unk[])() __asm__("D_80101448"); /* unused routine; points at Em39_move_rnd */
extern UCHAR Em39_move_rnd[8] __asm__("D_80101448");    /* random idle motions */

/* exe functions without a shared prototype */
LONG Dir_pos_ck_l(VECTOR *pV, VECTOR *pP, LONG Hed_dir, LONG Hani_dir) __asm__("Dir_pos_ck"); /* em_sub.c, called with int args */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");      /* em_sub.c, result used as int */
LONG Rnd(void) __asm__("func_800102E8");                                 /* RE2 Rnd (sld.s) */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, result used as int */
LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);        /* ground height */
LONG func_80044420(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);          /* pl_wep.c */
LONG func_800445C8(void *pA0, void *pA1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c: pEm inside the box */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */

#endif /* OVL_EM39_H */
