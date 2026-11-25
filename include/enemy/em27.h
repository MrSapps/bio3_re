#ifndef OVL_EM27_H
#define OVL_EM27_H
/*
 * Enemy module em27 (enemy id 0x27: the larva that em23's Drain Deimos / Brain Sucker
 * spawns, Em23_child_set; it crawls, leaps onto the player and bites). Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "joint.h"

/* em27's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM27_FREE {
    SHORT Dir;                         /* 0x00 (0x1CC) hold angle relative to the player's Cdir_y */
    SHORT Wait;                        /* 0x02 (0x1CE) frames before the next move */
    SHORT Pop;                         /* 0x04 (0x1D0) part 2 pop-up state (Em27_popup) */
    SHORT Floor;                       /* 0x06 (0x1D2) last Pl_work.Ground (Em27_pos_set) */
} EM27_FREE;

#define EM27_W(p) ((EM27_FREE *)(p)->Unk1CC)

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em27_routine_0[])();
extern void (*Em27_mv_br[])();
extern void (*Em27_mv[])();

void Em27_parts_reset(ENEMY_WORK *pEm); /* parts reset (live) */
void Em27_parts_set(ENEMY_WORK *pEm); /* hide (parts reset) */
void Em27_popup(ENEMY_WORK *pEm); /* part 2 pop-up */
void Em27_pos_set(ENEMY_WORK *pEm, SHORT Dist, SHORT Dir, VECTOR *pPos); /* place relative to pPos */
void Em27_hop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq); /* player damage routine */
LONG Em27_parent_cnt(ENEMY_WORK *pEm); /* other id 0x23/0x28 enemies */

void Em27_main(ENEMY_WORK *pEm);
void Em27_ground_ck(ENEMY_WORK *pEm);
void Em27_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_mv00_br(ENEMY_WORK *pEm);
void Em27_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_mv01_br(ENEMY_WORK *pEm);
void Em27_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_mv02_br(ENEMY_WORK *pEm);
void Em27_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_mv03_br(void);
void Em27_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_mv04_br(ENEMY_WORK *pEm);
void Em27_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_mv05_br(void);
void Em27_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_Hide(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_Damage(void);
void Em27_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em27_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

#endif /* OVL_EM27_H */
