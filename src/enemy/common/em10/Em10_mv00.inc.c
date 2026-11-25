/* shared by em10, em16 */
/* RE2 Em10_mv00: standing / swaying in place */
void Em10_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = Rnd_l() + 60;
        pEm->Move_cnt = (Rnd_l() & 0xF) << 1;
        Spd_get(pEm, pKan, pSeq, 0);
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if (!pEm->Move_no) {
                if (!(Rnd_l() & 0xF)) {
                    EM_SET_MOVE(pEm, 0x12, 0, 0x1F);
                    if (!EM10_W(pEm)->Unk21 && (Rnd_l() & 1)) {
                        Snd_se_enem(14, pEm);
                        EM10_W(pEm)->Unk21 = 150;
                    }
                }
            } else
                EM_SET_MOVE(pEm, 0, 0, 0x1F);
        }
        Add_speedXZ(pEm, 0);
        if (!(pEm->Type & 0x10)) {
            if (!pEm->Timer0 && (LONG)pEm->Unk154 < 5000 && (pEm->Status_flg & 0x2000))
                EM_SET_RTN(pEm, 1, 1, 0, 0);
            if ((pEm->Type & 0x40) && !pEm->Timer0)
                EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        if (pEm->Timer0)
            pEm->Timer0--;
        break;
    }
}
