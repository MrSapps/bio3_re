#ifndef OVL_EM39_H
#define OVL_EM39_H
/*
 * Enemy module em39 (enemy id 0x39): a tentacle-like part attached to the work
 * of enemy id 0x38 (its parent); it grows out of the parent (part Size_y),
 * sways and lashes at the player. Loaded at 0x80100000 (slot 1) or
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

void func_80100004(ENEMY_WORK *pEm);
void func_801003B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100588(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801005C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100648(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801006CC(ENEMY_WORK *pEm);
void func_8010079C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801008E0(void);
void func_801008E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100DB8(void);
void func_80100DC0(void);
void func_80100DC8(void);
void func_80100DD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101094(void);
void func_8010109C(void);
void func_801010A4(void);
void func_801010AC(void);
void func_801010B4(void);
void func_801010BC(void);
void func_801010C4(void);
void func_801010CC(void);
void func_801010D4(void);
void func_801010DC(void);
void func_801010E4(void);
void func_801010EC(void);
void func_801010F4(void);
void func_801010FC(void);
void func_80101104(void);
void func_80101114(void);
void func_8010111C(void);
void func_80101124(void);
void func_8010112C(void);
void func_80101134(void);
LONG func_8010113C(ENEMY_WORK *pEm);
SHORT func_801012A4(ENEMY_WORK *pEm, PARTS_W *pP);
void func_8010133C(ENEMY_WORK *pEm, PARTS_W *pP, LONG Size);

#endif /* OVL_EM39_H */
