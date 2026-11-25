/* shared by em10, em16 */
/* eating the dead player */
void Em10_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x19, 0, 0xF);
        pEm->Routine_2 = 1;
        Set_base_pos(&Pl_work, pKan, pSeq, 0);
        if (Rnd_l() & 1)
            Snd_se_enem(13, pEm);
        else
            Snd_se_enem(14, pEm);
        pEm->Damage_cnt |= 0x80;
    case 1:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 8, 0, 0);
        pEm->Type |= 0x4000;
        break;
    }
}
