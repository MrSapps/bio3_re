/* shared by em10, em16 */
/* RE2 Em10_Move_type00: standing zombie, decision (Em10_mv_br_type00) then action (Em10_mv_type00) */
void Em10_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em10_mv_br_type00[pEm->Routine_1](pEm);
    Em10_mv_type00[pEm->Routine_1](pEm, pKan, pSeq);
}
