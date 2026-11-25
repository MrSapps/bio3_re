/* shared by em10, em16 */
/* hit that may kill outright: stagger (dm01), or blow the head off (die03) unless Unk132 & 0xC */
void Em10_dm_sel01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_3 = 0;
        if (!(pEm->Unk132 & 0xC))
            pEm->Routine_3 = 1;
    }
    switch (pEm->Routine_3) {
    case 0:
        Em10_dm01(pEm, pKan, pSeq);
        break;
    case 1:
        Em10_die03(pEm, pKan, pSeq);
        break;
    }
}
