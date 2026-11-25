/* shared by em10, em16 */
/* RE2 Em10_mv0e: lunging forward */
void Em10_mv0e(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1A, 0, 0xF);
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Unk26 = (Rnd_l() & 0xF) + 70;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Spd_x = 30;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0xD4);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (EM10_W(pEm)->Flg & 0x4000)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}
