#ifndef OVL_EM39_H
#define OVL_EM39_H
/*
 * Enemy module em39 (enemy id 0x39): one of the two tentacles of em38 (enemy id
 * 0x38, its parent, which finds them in Em38_Init): it grows out of the parent
 * (part Size_y), sways (Em39_wait) and lashes at the player (Em39_lash). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "pad.h"
#include "player.h"
#include "pl_wep.h"

/* em39's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM39_FREE {
    ENEMY_WORK *pParent;               /* 0x00 (0x1CC) the enemy (id 0x38) this part belongs to */
    USHORT Wait;                       /* 0x04 (0x1D0) frames before the next attack */
    USHORT Flg;                        /* 0x06 (0x1D2) 1: growing, size is not updated */
    SHORT Dir;                         /* 0x08 (0x1D4) sway angle, -0x100..0x100 */
} EM39_FREE;

#define EM39_W(p) ((EM39_FREE *)(p)->Unk1CC)

/* module .data */
extern SHORT Em39_box0[4]; /* hit boxes of the lash */
extern SHORT Em39_box1[4];
extern SHORT Em39_box2[4];
extern void (*Em39_routine_0[])();
extern void (*Em39_move_type[])();
extern void (*Em39_mv_br[])();
extern void (*Em39_mv[])();
extern void (*Em39_mv_br_unk[])() __asm__("Em39_move_rnd"); /* unused routine; points at Em39_move_rnd */
extern UCHAR Em39_move_rnd[8]; /* random idle motions */

void Em39_main(ENEMY_WORK *pEm);
void Em39_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_Move_type_unk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_wait_br(ENEMY_WORK *pEm);
void Em39_wait(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_lash_br(ENEMY_WORK *pEm);
void Em39_lash(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv02_br(ENEMY_WORK *pEm);
void Em39_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_grow_br(ENEMY_WORK *pEm);
void Em39_grow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv04_br(ENEMY_WORK *pEm);
void Em39_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv05_br(ENEMY_WORK *pEm);
void Em39_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv06_br(ENEMY_WORK *pEm);
void Em39_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv07_br(ENEMY_WORK *pEm);
void Em39_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv08_br(ENEMY_WORK *pEm);
void Em39_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv09_br(ENEMY_WORK *pEm);
void Em39_mv09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_mv10_br(ENEMY_WORK *pEm);
void Em39_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em39_Damage(void);
void Em39_Die(void);
void Em39_Scenario(void);
void Em39_Dead(void);
void Em39_dummy0(void);
void Em39_dummy1(void);
LONG Em39_splash(ENEMY_WORK *pEm);
SHORT Em39_part_dir(ENEMY_WORK *pEm, PARTS_W *pP);
void Em39_part_scale_pos(ENEMY_WORK *pEm, PARTS_W *pP, LONG Size);

#endif /* OVL_EM39_H */
