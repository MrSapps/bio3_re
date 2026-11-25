#ifndef OVL_EM37_H
#define OVL_EM37_H
/*
 * Enemy module em37 (enemy ids 0x37, 0x3B): Nemesis' tentacle. A separate work
 * that hangs on part 5 of its parent (Nemesis, em34/em36, ids 0x35/0x36) and
 * plays the motion the parent's attacks start (Routine_1), growing part 0
 * towards Size; mv 11 grabs the player.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "joint.h"

/* em37's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM37_FREE {
    ENEMY_WORK *pParent;               /* 0x00 (0x1CC) the enemy (id 0x35 or 0x36) this part belongs to */
    UCHAR pad4[2];
    SHORT Size;                        /* 0x06 (0x1D2) target Size_y of part 1 */
} EM37_FREE;

#define EM37_W(p) ((EM37_FREE *)(p)->Unk1CC)

/* random idle motions, copied to the stack by the move routines */
typedef struct tagEM37_MOTION {
    UCHAR No[8];
} EM37_MOTION;

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em37_routine_0[])();
extern void (*Em37_move_type[])();    /* by Type: Em37_Move_type00 */
extern void (*Em37_mv_br[])();         /* brain slots, unreferenced */
extern void (*Em37_mv[])();
extern void (*Em37_sce[])();


extern const EM37_MOTION Em37_motion;

void Em37_main(ENEMY_WORK *pEm);
void Em37_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_Move_sce(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv00_br(ENEMY_WORK *pEm);
void Em37_idle(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv01_br(ENEMY_WORK *pEm);
void Em37_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv02_br(ENEMY_WORK *pEm);
void Em37_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv03_br(ENEMY_WORK *pEm);
void Em37_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv04_br(ENEMY_WORK *pEm);
void Em37_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv05_br(ENEMY_WORK *pEm);
void Em37_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv06_br(ENEMY_WORK *pEm);
void Em37_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv07_br(ENEMY_WORK *pEm);
void Em37_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv08_br(ENEMY_WORK *pEm);
void Em37_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv09_br(ENEMY_WORK *pEm);
void Em37_mv09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_mv10_br(ENEMY_WORK *pEm);
void Em37_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_grab_br(ENEMY_WORK *pEm);
void Em37_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_Damage(void);
void Em37_Die(void);
void Em37_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_Retract(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em37_dummy0(void);
void Em37_dummy1(void);
LONG Em37_splash(ENEMY_WORK *pEm);

#endif /* OVL_EM37_H */
