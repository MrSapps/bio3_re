/* shared by em10, em16 */
/* RE2 Em10_sce00 */
void Em10_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x1F0000;
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}
