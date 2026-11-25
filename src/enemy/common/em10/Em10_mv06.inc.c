/* shared by em10, em16 */
/* RE2 Em10_mv06: falling down and getting up again */
void Em10_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[3][2] = { { 1, 3 }, { 24, 23 }, { 9, 10 } };
    SVECTOR Sv;
    USHORT Timer[16] = { 100, 30, 200, 160, 10, 40, 150, 50, 150, 60, 35, 110, 30, 180, 50, 300 };
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd_l() & 1;
        EM10_W(pEm)->Etc_flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Etc_flg |= 4;
        EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[0][pEm->UnkE2];
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Etc_flg |= 0x200;
        EM10_W(pEm)->Etc_flg |= 2;
        pEm->Damage_cnt |= 0x80;
        EM10_W(pEm)->Dm_r0 = 0;
        pEm->UnkE3 = 0;
        pEm->Type |= 0x2000;
        if (!EM10_W(pEm)->Dam_s) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        pEm->Timer0 = 0;
    case 1:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (!pEm->Hokan_flg || pEm->Timer0) {
            if (EM10_W(pEm)->Etc_flg & 0x8000)
                Em10_foot_set(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        pEm->Routine_2 = 3;
        EM_SET_MOVE(pEm, Mot[1][pEm->UnkE2], 0, 0xF);
        pEm->UnkDA = Timer[(Rnd_l() >> (Rnd_l() & 3)) & 0xF] + (Rnd_l() & 0xF);
        pEm->Damage_cnt &= 0x7F;
        pEm->Spd_x = Rnd_l() & 3;
        pEm->Col.W0 = pEm->Col.D0 = 100;
    case 3:
        if (!pEm->Spd_x)
            pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (pEm->UnkDA-- == 0)
            pEm->Routine_2 = 6;
        break;
    case 4:
        pEm->Timer0 = (Rnd_l() & 0xF) + 3;
        pEm->Routine_2 = 5;
    case 5:
        if (pEm->Timer0-- == 0)
            pEm->Routine_2 = 3;
        if (pEm->UnkDA-- == 0)
            pEm->Routine_2 = 6;
        break;
    case 6:
        EM_SET_MOVE(pEm, Mot[2][pEm->UnkE2], 0, 0xF);
        pEm->Routine_2 = 7;
        EM10_W(pEm)->Etc_flg |= 0x10;
        EM10_W(pEm)->Mot_dm = 1;
        if (EM10_W(pEm)->Etc_flg & 4)
            EM10_W(pEm)->Mot_dm = 0;
        if (pEm->UnkE3 == 1 && pEm->Damage_no != 0x1A) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0, (USHORT)pEm->Cdir_y | 0x1F400000, &pP->Workm, &Sv, 0);
        }
        if (!EM10_W(pEm)->Dam_s && !(Rnd_l() & 3)) {
            Snd_se_enem(14, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        EM10_W(pEm)->Etc_flg &= 0xFDFF;
        pEm->Damage_flg |= 0x8000;
    case 7:
        if (pEm->Move_cnt < 0x37)
            func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM10_W(pEm)->Etc_flg &= 0xFFEF;
            pEm->Routine_2 = 8;
            EM10_W(pEm)->Etc_flg &= 0xFFFD;
        }
        pEm->Col.D0 += 10;
        pEm->Col.W0 += 10;
        if (pEm->Col.D0 > 500)
            pEm->Col.W0 = pEm->Col.D0 = 500;
        if (pEm->Move_cnt == 0x37) {
            pEm->Tilt_spd = 30;
            pEm->Damage_flg &= 0x7FFF;
            pEm->Be_flg |= 0x80000;
        }
        break;
    case 8:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Col.W0 = pEm->Col.D0 = 500;
        pEm->Type &= 0xDFFF;
        EM10_W(pEm)->Etc_flg &= 0xFFED;
        break;
    }
}
