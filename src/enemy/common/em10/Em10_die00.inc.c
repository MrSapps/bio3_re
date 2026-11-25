/* shared by em10, em16 */
/* RE2 Em10_die00: falling over dead (sometimes gets up again as a crawler) */
void Em10_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG T;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd_l() & 1;
        if (EM10_W(pEm)->Etc_flg & 0x2000)
            pEm->UnkE2 = 0;
        EM10_W(pEm)->Etc_flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Etc_flg |= 4;
        EM_MOVE(pEm) = pEm->UnkE2 * 0xA00 + 0xF0000 + Mot[pEm->UnkE2];
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        if (pEm->Damage_flg & 0x200)
            pEm->UnkE3 = 5;
        Em10_blood_set(pEm);
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        if ((pEm->Damage_flg & 3) != 1) {
            T = pEm->UnkE2 * 10;
            if (pEm->Move_cnt >= T + 4 && pEm->Move_cnt <= T + 16 && (pEm->Move_cnt & 1)) {
                pP = &pEm->pSin_parts_ptr[1];
                Sv.vx = pP->Pos_x;
                Sv.vy = pP->Pos_y + 500;
                Sv.vz = pP->Pos_z;
                pP = pEm->pSin_parts_ptr;
                Esp_call((pEm->UnkE3 & 0xF) << 24, (((Rnd_l() << 3) + 0xFD0) << 16) | (USHORT)(Rnd_l() << 4),
                              &pP->Workm, &Sv, 0);
            }
        }
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (!pEm->Hokan_flg) {
            if (EM10_W(pEm)->Etc_flg & 0x8000)
                Em10_foot_set(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        if (!(Rnd_l() & 3) && !(EM10_W(pEm)->Etc_flg & 4) && !(pEm->Type & 0x20) && !(pEm->Status_flg & 0x400)
            && !pEm->Water && (LONG)G.Status_flg >= 0) {
            pEm->Life = 1;
            pEm->Type = 0x2001;
            Rnd_l();
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            do {                       /* (a macro in the original? the loop notes keep the load below the store) */
                pEm->Damage_cnt &= 0x7F;
            } while (0);
        }
        break;
    }
}
