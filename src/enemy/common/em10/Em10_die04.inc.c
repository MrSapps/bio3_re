/* shared by em10, em16 */
/* RE2 Em10_die04: blown away by a close shot */
void Em10_die04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG n;
    LONG T;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Life = -1;
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        n = (Rnd_l() & 3) + 1;
        pEm->UnkE3 = 0;
        if (pEm->Damage_flg & 0x200)
            pEm->UnkE3 = 5;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -300;
        Sv.vz = 0;
        Esp_call((pEm->UnkE3 & 0xF) << 24, (USHORT)pEm->Timer0 | 0x1FA00000, &pP->Workm, &Sv, 0);
        Sv.vx = 0;
        Sv.vy = -100;
        Sv.vz = 0;
        Esp_call((pEm->UnkE3 & 0xF) << 24, (USHORT)pEm->Timer0 | 0x17D00000, &pP->Workm, &Sv, 0);
        if (!(pEm->Damage_flg & 0xA00)) {
            do {
                n--;
                Sv.vx = 0x100 - (Rnd_l() << 1);
                Sv.vy = 0x38 - (Rnd_l() << 1);
                Sv.vz = 0x100 - (Rnd_l() << 1);
                Esp_call(0x209, (((Rnd_l() << 2) + 0x600) << 16) | (USHORT)(pEm->Timer0 + 0x200 - (Rnd_l() << 2)),
                              &pP->Workm, &Sv, 0);
            } while (n);
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 4;
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
        }
        pEm->UnkE2 = ~Cdir_ck(pEm, &Pl_work) & 1;
        EM10_W(pEm)->Etc_flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Etc_flg |= 4;
        EM_SET_MOVE(pEm, Mot[pEm->UnkE2], 0xF, 0xF);
        pEm->Routine_2 = 1;
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
        pEm->Spd_x = 200;
        pEm->Spd_z = 0;
    case 1:
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
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, pEm->UnkE2 << 11);
        if ((pEm->Spd_x -= 50) < 0)
            pEm->Spd_x = 0;
        if (!pEm->Spd_x) {
            if (EM10_W(pEm)->Etc_flg & 0x8000)
                Em10_foot_set(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}
