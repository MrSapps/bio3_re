/* shared by em10, em16 */
/* RE2 Em10_Move */
void Em10_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em10_Move_type[pEm->Type & 0xF](pEm, pKan, pSeq);
}
