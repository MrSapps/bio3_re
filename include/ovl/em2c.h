#ifndef OVL_EM2C_H
#define OVL_EM2C_H
/*
 * Enemy module em2c (enemy id 0x2C), the successor of RE2's EM2C.C: a static
 * model whose visible parts and shadow size depend on Type & 0xF.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"

/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 1) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

extern void (*Em2c_routine_0[])() __asm__("D_801004E4");

void func_8010001C(ENEMY_WORK *pEm);
void func_80100060(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801004B0(ENEMY_WORK *pEm);

#endif /* OVL_EM2C_H */
