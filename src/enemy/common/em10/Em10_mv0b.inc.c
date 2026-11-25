/* shared by em10, em16 */
/* RE2 Em10_mv0b: knocked down by a push, then getting up (or falling as in Em10_mv06) */
void Em10_mv0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Down_mot[2][2] = { { 4, 4 }, { 5, 14 } };
    UCHAR Mot[3][2] = { { 1, 3 }, { 24, 23 }, { 9, 10 } };  /* as Em10_mv06 */

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd_l() & 1;
        EM_MOVE(pEm) = ((Rnd_l() & 3) << 8) + Down_mot[pEm->UnkE3][pEm->UnkE2];
        if (pEm->UnkE3 && pEm->UnkE2)
            pEm->Move_cnt = (Rnd_l() & 3) + 20;
        pEm->Routine_2 = 1;
        pEm->Spd_x = 400;
        pEm->Spd_z = 0;
        pEm->UnkDA = 0;
        pEm->Timer0 = pEm->UnkE3 * 2048;
        EM10_W(pEm)->Flg &= 0xFFEF;
        EM10_W(pEm)->Flg |= 8;
        if (!EM10_W(pEm)->Unk21 && (Rnd_l() & 1)) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
    case 1:
        Add_speedXZ(pEm, pEm->Timer0);
        if ((pEm->Spd_x -= 30) < 0) {
            pEm->Spd_x = 0;
            pEm->UnkDA++;
        }
        if (pEm->UnkDA == 1 && (Rnd_l() >> (Rnd_l() & 3)) & 7) {
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            EM10_W(pEm)->Unk0B = (Rnd_l() & 0xF) + 0x10;
            EM10_W(pEm)->Flg &= 0xFFF7;
            pEm->UnkE2 = pEm->UnkE3;
            EM10_W(pEm)->Flg &= 0xFFFB;
            if (pEm->UnkE2)
                EM10_W(pEm)->Flg |= 4;
            EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[0][pEm->UnkE2];
            pEm->Routine_2 = 1;
            EM10_W(pEm)->Flg |= 0x200;
            EM10_W(pEm)->Flg |= 2;
            pEm->Damage_cnt |= 0x80;
            EM10_W(pEm)->Unk0A = 0;
            pEm->UnkE3 = 0;
            pEm->Type |= 0x2000;
            if (!EM10_W(pEm)->Unk21) {
                if (Rnd_l() & 1)
                    Snd_se_enem(12, pEm);
                else
                    Snd_se_enem(15, pEm);
                EM10_W(pEm)->Unk21 = 150;
            }
            pEm->Timer0 = 0;
            break;
        }
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        EM10_W(pEm)->Flg &= 0xFFF7;
        break;
    }
}
