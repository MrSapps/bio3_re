/* shared by em10, em16 */
/* RE2 Em10_sce00: standing still under scenario control */
void Em10_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}
