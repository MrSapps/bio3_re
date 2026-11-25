/* shared by em10, em16 */
/* standing still while the scenario holds it (Type 0x4000) */
void Em10_mv0f(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 6, 0, 0xF);
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() & 7) + 7;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x100);
        if (!(pEm->Type & 0x4000))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
}
