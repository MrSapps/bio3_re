#ifndef OVL_EM37_H
#define OVL_EM37_H
/*
 * Enemy module em37 (enemy ids 0x37, 0x3B). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "joint.h"

/* em37's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM37_FREE {
    ENEMY_WORK *pParent;               /* 0x00 (0x1CC) the enemy (id 0x35 or 0x36) this part belongs to */
    UCHAR pad4[2];
    SHORT Size;                        /* 0x06 (0x1D2) target Size_y of part 1 */
} EM37_FREE;

#define EM37_W(p) ((EM37_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)

/* random idle motions, copied to the stack by the move routines */
typedef struct tagEM37_MOTION {
    UCHAR No[8];
} EM37_MOTION;

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em37_routine_0[])();
extern void (*Em37_move_type[])();
extern void (*Em37_mv[])();
extern void (*Em37_sce[])();

LONG func_80101854(ENEMY_WORK *pEm); /* splash effects */

extern const EM37_MOTION Em37_motion;

void func_8010003C(ENEMY_WORK *pEm);
void func_80100370(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801005A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801005DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100618(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100654(void);
void func_8010065C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100894(void);
void func_8010089C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010094C(void);
void func_80100954(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801009E4(void);
void func_801009EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100AB4(void);
void func_80100ABC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100B8C(void);
void func_80100B94(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100CBC(void);
void func_80100CC4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100D5C(void);
void func_80100D64(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100DF0(void);
void func_80100DF8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100EB0(void);
void func_80100EB8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100F58(void);
void func_80100F60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100FD8(void);
void func_80100FE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101598(void);
void func_801015A8(void);
void func_801015B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010160C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101844(void);
void func_8010184C(void);

#endif /* OVL_EM37_H */
