/* shared by em10, em16 */
/* lying down, getting up when the player comes near */
void Em10_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Col.W0 = pEm->Col.D0 = 100;
        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 1;
    case 1:
        if (!(pEm->Type & 0x4000))
            pEm->Routine_2 = 2;
        break;
    case 2:
        if (!pEm->Add_dir) {
            EM_SET_MOVE(pEm, 9, Rnd_l() & 0xF, 0xF);
            if (EM10_W(pEm)->Etc_flg & 4)
                EM_SET_MOVE(pEm, 0xA, Rnd_l() & 0xF, 0xF);
        } else {
            EM_SET_MOVE(pEm, 9, pEm->Add_dir, 0xF);
            if (EM10_W(pEm)->Etc_flg & 4)
                EM_SET_MOVE(pEm, 0xA, pEm->Add_dir, 0xF);
        }
        pEm->Routine_2 = 3;
        if (!EM10_W(pEm)->Dam_s) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else if (Rnd_l() & 1)
                Snd_se_enem(14, pEm);
            else
                Snd_se_enem(13, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        EM10_W(pEm)->Mot_dm = 1;
        EM10_W(pEm)->Etc_flg |= 0x10;
        pEm->Damage_cnt &= 0x7F;
        if (EM10_W(pEm)->Etc_flg & 4)
            EM10_W(pEm)->Mot_dm = 0;
        pEm->Damage_flg |= 0x8000;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 4;
            EM10_W(pEm)->Etc_flg &= 0xFFEF;
        }
        pEm->Col.D0 += 10;
        pEm->Col.W0 += 10;
        if (pEm->Col.D0 > 500)
            pEm->Col.W0 = pEm->Col.D0 = 500;
        if (pEm->Move_cnt == 0x37)
            pEm->Damage_flg &= 0x7FFF;
        break;
    case 4:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Col.W0 = pEm->Col.D0 = 500;
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Etc_flg &= 0xFFEF;
        pEm->Neck_flg &= 0xFE;
        break;
    }
}
