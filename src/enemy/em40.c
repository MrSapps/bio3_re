#include "enemy/em40.h"
#include "joint.h"

/* per-frame entry (RE2 EM_MAIN): routine by Routine_0 */
void Em40_main(ENEMY_WORK *pEm)
{
    Em40_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* Routine_0 0: invulnerable; part 3 turns towards the player (neck) */
void Em40_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    EM_SET_MOVE(pEm, 0, 0, 7);
    EM_SET_RTN(pEm, 1, 0, 0, 0);
    Joint_move(pEm, pKan, pSeq, 0x100);
    pEm->Neck_no = 3;
    pEm->Unk120 = 0;
    pP = &pEm->pSin_parts_ptr[3];
    PW_NECK(pP)->Add_dir_y = 0x40;
    PW_NECK(pP)->Add_dir_z = 0x40;
    PW_NECK(pP)->L_rot_y = 0x200;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    PW_NECK(pP)->L_rot_z = 0x138;
    pEm->Life = -1;
    pEm->Max_life = -1;
    pEm->Damage_cnt = 0x80;
}

/* Routine_0 1: nothing */
void Em40_Move(ENEMY_WORK *pEm)
{
}
