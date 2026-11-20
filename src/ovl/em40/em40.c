#include "common.h"
#include "ovl/em40.h"

/* routine table (module .data), unprototyped like RE2's: routines take (pEm, Kan, Seq) */
extern void (*Em40_routine_0[])() __asm__("D_801000D4");

/* RE2 EM_MAIN */
void func_80100004(ENEMY_WORK *pEm)
{
    Em40_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* Em40_init */
void func_80100048(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    PARTS_W *pP;

    EM_MOVE(pEm) = 0x70000;
    EM_RTN(pEm) = 1;
    Joint_move(pEm, Kan, Seq, 0x100);
    pEm->Neck_no = 3;
    pEm->Unk120 = 0;
    pP = &pEm->pSin_parts_ptr[3];
    PW_FREE(pP)[2] = 0x40;
    PW_FREE(pP)[3] = 0x40;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[5] = 0x138;
    pEm->Life = -1;
    pEm->Max_life = -1;
    pEm->Damage_cnt = 0x80;
}

void func_801000CC(void)
{
}
