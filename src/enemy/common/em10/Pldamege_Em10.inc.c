/* shared by em10, em16 */
/* RE2 PLDAMEGE_EM (Pldamege_Em110), called for the player held by this enemy */
void Pldamege_Em10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    Pldamage_Em10_tbl[pEm->Routine_1](pEm, pKan, pSeq);
    if (pEm->Water) {
        pP = pEm->pSin_parts_ptr;
        if (pP->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 7) == 1)
            Em10_water(pEm);
    }
}
