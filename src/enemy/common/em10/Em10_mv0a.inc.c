/* shared by em10, em16 */
/* RE2 Em10_mv0a: eating a corpse */
void Em10_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[8] = { 19, 20, 21, 19, 20, 21, 19, 20 };
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, Mot[Rnd_l() & 7], 0, 7);
        pEm->Damage_cnt |= 0x80;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            EM_SET_MOVE(pEm, Mot[Rnd_l() & 7], 3, 0);
        if (!pEm->Hokan_flg) {
            if (EM10_W(pEm)->Etc_flg & 0x8000)
                Em10_foot_set(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        if (*pEm->pNow_seq & 0x200) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 200;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x13D00000, &pP[1].Workm, &Sv, 0);
            Snd_se_enem((Rnd_l() & 1) | 4, pEm);
        }
        if (!(pEm->Type & 0x4000))
            pEm->Routine_2 = 3;
        break;
    case 2:
        EM_SET_MOVE(pEm, Mot[Rnd_l() & 7], 3, 0);
        pEm->Routine_2 = 1;
        break;
    case 3:
        pEm->Routine_2 = 4;
        EM_SET_MOVE(pEm, 0x16, 0, 0xF);
        EM10_W(pEm)->Mot_dm = 2;
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Etc_flg |= 0x10;
        if (!EM10_W(pEm)->Dam_s) {
            if (Rnd_l() & 1)
                Snd_se_enem(14, pEm);
            else if (Rnd_l() & 1)
                Snd_se_enem(13, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 5;
            EM10_W(pEm)->Etc_flg &= 0xFFEF;
        }
        break;
    case 5:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Etc_flg &= 0xFFEF;
        break;
    case 6:
        pEm->Routine_2 = 7;
        EM_SET_MOVE(pEm, 0x16, Rnd_l() & 7, 0xF);
    case 7:
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x10100);
        break;
    case 8:
        pEm->Routine_2 = 0;
        break;
    }
}
