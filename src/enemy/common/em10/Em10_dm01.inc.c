/* shared by em10, em16 */
/* hit, staggering back (may fall over) */
void Em10_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 0 };
    PARTS_W *pP;
    SVECTOR Sv;                        /* unused (frame size) */
    UCHAR Mot[2] = { 1, 3 };
    SHORT Dir;
    LONG R;

    switch (pEm->Routine_2) {
    case 0:
        Dir = pEm->Damage_flg & 0x10;
        EM_MOVE(pEm) = 0x30005 - (Dir = Dir != 0);
        pEm->Routine_2 = 1;
        pEm->UnkDA = 10;
        pEm->Spd_x = Spd[(pEm->Damage_flg >> 4) & 1];
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        if (!EM10_W(pEm)->Dam_s) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        pEm->UnkE2 = 1;
        if (pEm->Damage_flg & 0x10)
            pEm->UnkE2 = -1;
        if (pEm->Damage_no == 0x1A)
            pEm->UnkE2 = 0;
        else {
            Em10_blood_set(pEm);
            if ((pEm->Damage_flg & 3) == 1)
                pEm->UnkE2 = 0;
        }
        pEm->UnkE3 = 0;
        if (pEm->Damage_flg & 0x200)
            pEm->UnkE3 = 5;
        if (pEm->Damage_no == 0x1A && !(Rnd_l() & 1)) {
            EM10_W(pEm)->Dm_cnt = 0;
            EM10_W(pEm)->Dm_r0 = 1;
        }
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Damage_no == 0x1A && pEm->Move_cnt < 10)
            pEm->Spd_x <<= 1;
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkDA && pEm->UnkE2) {
            if (!(pEm->UnkDA & 1)) {
                pP = pEm->pSin_parts_ptr;
                if (!EM10_W(pEm)->Dm_r0) {
                    Esp_call((pEm->UnkE3 & 0xF) << 24,
                                  (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x1F400000, &pP->Workm,
                                  (SVECTOR *)&pEm->Spd_x, 0);
                } else {
                    pEm->Spd_y -= 500;
                    Esp_call((pEm->UnkE3 & 0xF) << 24,
                                  (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x13E80000, &pP->Workm,
                                  (SVECTOR *)&pEm->Spd_x, 0);
                }
            }
            pEm->UnkDA--;
        }
        if (pEm->Damage_no == 0x1A && pEm->Move_cnt == 13) {
            R = Rnd_l() & 3;
            if (R == 1 && !(pEm->Status_flg & 0x400)) {
                EM_SET_RTN(pEm, 1, 5, 0, 0);
                EM10_W(pEm)->Dm_cnt = (Rnd_l() & 0xF) + 0x10;
                EM10_W(pEm)->Etc_flg &= 0xFFF7;
                pEm->UnkE2 = R;
                EM10_W(pEm)->Etc_flg |= 4;
                if (pEm->Damage_flg & 0x10) {
                    pEm->UnkE2 = 0;
                    EM10_W(pEm)->Etc_flg &= 0xFFFB;
                }
                EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[pEm->UnkE2];
                pEm->Routine_2 = R;
                EM10_W(pEm)->Dm_r0 = 0;
                EM10_W(pEm)->Etc_flg |= 0x202;
                pEm->UnkE3 = 0;
                pEm->Damage_cnt |= 0x80;
                pEm->Type |= 0x2000;
                if (!EM10_W(pEm)->Dam_s) {
                    if (Rnd_l() & 1)
                        Snd_se_enem(12, pEm);
                    else
                        Snd_se_enem(15, pEm);
                    EM10_W(pEm)->Dam_s = 150;
                }
                pEm->Timer0 = 0;
            }
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Rnd_l() & 1) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (EM10_W(pEm)->Etc_flg & 0x4000)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        if (EM10_W(pEm)->Dm_cnt <= 0) {
            EM10_W(pEm)->Dm_cnt = (Rnd_l() & 0x10) + 0xF;
            EM10_W(pEm)->Dm_r0 = 0;
        }
        if ((LONG)pEm->L_pl < 3000 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 900) && (Rnd_l() & 1))
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
}
