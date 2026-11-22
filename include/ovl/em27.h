#ifndef OVL_EM27_H
#define OVL_EM27_H
/*
 * Enemy module em27 (enemy id 0x27). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
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
    SHORT Pop;                         /* 0x04 (0x1D0) part 2 pop-up state (func_8010158C) */
    SHORT Floor;                       /* 0x06 (0x1D2) last Pl_work.Unk122 (func_801014B0) */
} EM27_FREE;

#define EM27_W(p) ((EM27_FREE *)(p)->Unk1CC)
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
/* the player work inside G (RE2 G.Player; Pl_work is the same address) */
#define G_PLAYER ((ENEMY_WORK *)&G.Pl_be_flg)

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em27_routine_0[])() __asm__("D_80101A34");
extern void (*Em27_mv_br[])() __asm__("D_80101A54");
extern void (*Em27_mv[])() __asm__("D_80101A6C");

void func_80100378(ENEMY_WORK *pEm); /* parts reset (live) */
void func_80100518(ENEMY_WORK *pEm); /* hide (parts reset) */
void func_8010158C(ENEMY_WORK *pEm); /* part 2 pop-up */
void func_801014B0(ENEMY_WORK *pEm, SHORT Dist, SHORT Dir, VECTOR *pPos); /* place relative to pPos */
void func_801018D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq); /* player damage routine */
LONG func_801019BC(ENEMY_WORK *pEm); /* other id 0x23/0x28 enemies */

void func_8010001C(ENEMY_WORK *pEm);
void func_80100060(ENEMY_WORK *pEm);
void func_801000E0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801005F8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100688(ENEMY_WORK *pEm);
void func_801006E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801008EC(ENEMY_WORK *pEm);
void func_80100A50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100BAC(ENEMY_WORK *pEm);
void func_80100CD8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100DDC(void);
void func_80100DE4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100FFC(ENEMY_WORK *pEm);
void func_801010D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010131C(void);
void func_80101324(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101344(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101660(void);
void func_80101668(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801017FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

#endif /* OVL_EM27_H */
