/* shared by em30, em33 */
/* RE2 Em30_Damage */
void Em30_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    pEm->Routine_1 = 0;
    if (pEm->Damage_no == 30 && !(pEm->Damage_flg & 4))
        pEm->Routine_1 = 1;
    Em30_dm[pEm->Damage_no - 1][pEm->Routine_1](pEm, pKan, pSeq);
}
