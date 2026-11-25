/* shared by em10, em16 */
/* RE2 Em10_Move_type01 */
void Em10_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em10_mv_br_type01[pEm->Routine_1](pEm);
    Em10_mv_type01[pEm->Routine_1](pEm, pKan, pSeq);
}
