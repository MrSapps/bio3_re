#ifndef OVL_EM40_H
#define OVL_EM40_H
/*
 * Enemy module em40 (enemy id 0x40): a static model with a two-entry routine
 * table (init, empty move). Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"

/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 1) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

extern void (*Em40_routine_0[])() __asm__("D_801000D4");

void func_80100004(ENEMY_WORK *pEm);
void func_80100048(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801000CC(void);

#endif /* OVL_EM40_H */
