/* shared by em10, em16 */
/* RE2 Em10_mv03: grabbing and biting the player */
void Em10_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Type_mot[4][2] = { { 12, 12 }, { 15, 15 }, { 12, 12 }, { 15, 15 } };
    SVECTOR Sv;
    MATRIX M;                          /* unused (frame size) */
    UCHAR Atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } };
    VECTOR V;                          /* unused (frame size) */
    ENEMY_WORK *pT;
    PARTS_W *pP;
    PARTS_W *pW;
    ENEMY_WORK *pPl;
    ULONG Flg;
    ULONG Life_ck;
    ULONG Up;
    SHORT Old_x;
    SHORT Old_z;

    pPl = &Pl_work;
    Flg = pEm->Type & 1;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 0xF0000;
        pEm->Routine_2 = 1;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        func_8002646C_(pEm, 200, 20, pEm->Col.Unk39);
        EM_RTN(pPl) = (Cdir_ck_l(pEm, pPl) << 8) | 5;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x800;
        pPl->Be_flg |= 0x800;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        Pad_vib_fade_set(20, 0, 0xFA, 0);
        pEm->Damage_cnt &= 0x7F;
        if (Flg & 1) {
            pPl->Routine_1 += 2;
            pEm->Damage_cnt |= 0x80;
        }
    case 1:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 1;
        pEm->Routine_2 = 3;
        pEm->Timer0 = 148;
        if (G.Unk75EA & 1)
            pEm->Timer0 = 74;
    case 3:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Timer0 = pEm->Timer0 - 2 - Gacha_ch() * 5;
        if (pEm->Timer0 < 0) {
            pEm->Routine_2 = 4;
            pPl->Routine_2 = 4;
        }
        if (pEm->Move_cnt == Atck[Flg][0]) {
            Pad_vib_set1(6, 0xFA, 0);
            Pad_vib_set0(10, 0);
            Pad_vib_set1(4, 0xFA, 0xF);
            Pad_vib_set0(4, 0xD);
            Snd_se_enem((Rnd_l() & 1) | 4, pEm);
            Life_ck = Pl_life_down(Atck[Flg][1], Flg);
            if (Life_ck & 1) {
                pEm->Routine_2 = 4;
                pPl->Routine_2 = 4;
                pPl->UnkE2 = 1;
            }
            if (Life_ck & 2) {
                EM_SET_RTN(pEm, 1, 6, 0, 0);
                EM_RTN(&Pl_work) = (Cdir_ck_l(pEm, pPl) << 8) | 6;
            }
        }
        break;
    case 4:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 0x70002;
        pEm->Routine_2 = 5;
        pEm->Timer0 = 0;
        pEm->UnkE2 = 0;
        Pad_vib_set1(4, 0xFA, 10);
    case 5:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 3;
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        if (Flg & 1) {
            if (pPl->Id >= 8) {
                if (pEm->Move_cnt == 0x16) {
                    pW = &pEm->pSin_parts_ptr[1];
                    pEm->Unk120 |= 1;
                    pW->UnkA2 |= 4;
                    PW_FREEL(pW)[0] = 0x40;
                    PW_FREEL(pW)[1] = 0x202030;
                    pW = pEm->pSin_parts_ptr;
                    Sv.vx = 0;
                    Sv.vy = -600;
                    Sv.vz = 0;
                    Esp_call(8, (USHORT)pEm->Cdir_y | 0x10000000, &pW->Workm, &Sv, 0);
                    Snd_se_enem(2, pEm);
                    Pad_vib_set1(4, 0xFA, 0);
                }
            } else {
                if (pEm->Move_cnt == 0x1C) {
                    pW = &pEm->pSin_parts_ptr[1];
                    pEm->Unk120 |= 1;
                    pW->UnkA2 |= 4;
                    PW_FREEL(pW)[0] = 0x40;
                    PW_FREEL(pW)[1] = 0x202030;
                    Snd_se_enem(10, pEm);
                    Pad_vib_set1(4, 0xFA, 0);
                }
            }
            if (pEm->Move_cnt >= 0x17 && pEm->Move_cnt < 0x2A && (pEm->Move_cnt & 1)) {
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 0;
                Sv.vy = 100;
                Sv.vz = 0;
                Esp_call(0, (USHORT)(pEm->Cdir_y + (Rnd_l() << 2) - 0x200) | 0x10000000, &pP[1].Workm, &Sv, 0);
            }
        }
        if (!(Flg & 1) && pEm->Move_cnt == 7)
            pEm->Routine_2 = 7;
        break;
    case 6:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        pEm->Life = -1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Damage_cnt |= 0x80;
        break;
    case 7:
        Spd_get2(pEm, pKan, pSeq, 0);
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 = 8;
    case 8:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x -= 10;
        Add_speedXZ(pEm, 0);
        Up = 0;
        if (pEm->Move_cnt == 0x16 && !(pEm->Status_flg & 0x400))
            Up = (Rnd_l() % 4) != 0;
        if (Up) {
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            EM10_W(pEm)->Unk0B = (Rnd_l() & 0xF) + 0x10;
            EM10_W(pEm)->Flg &= 0xFFF7;
            pEm->UnkE2 = 1;
            EM10_W(pEm)->Flg &= 0xFFFF;
            EM10_W(pEm)->Flg |= 4;
            EM_MOVE(pEm) = pEm->UnkE2 * 0xA00 + 0xF0A03;
            pEm->Routine_2 = 1;
            EM10_W(pEm)->Flg &= 0xFFFF;
            EM10_W(pEm)->Flg |= 0x202;
            pEm->Damage_cnt |= 0x80;
            EM10_W(pEm)->Unk0A = 0;
            pEm->UnkE3 = 0;
            pEm->Type |= 0x2000;
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            pEm->Timer0 = 1;
            if (!(G.Unk75EA & 1)) {
                EM10_W(pEm)->Unk18 = 150;
                G.Unk75EA |= 1;
            }
            break;
        }
        if (pEm->Move_cnt >= 5 && pEm->Move_cnt < 25 && pEm->Col.Unk3 != 0xFF) {
            pT = G.pEnemy[pEm->Col.Unk3];
            if ((pT->Id & 0x70) == 0x10 && pT->Life >= 0 && !(pT->Type & 1) && !pT->Damage_cnt
                && !(EM10_W(pT)->Flg & 8) && !(pT->Type & 0x2000)) {
                EM_SET_RTN(pT, 1, 9, 0, 0);
                pT->UnkE3 = Cdir_ck(pEm, pT);
                Snd_se_enem(6, pEm);
            }
        }
        break;
    case 9:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Be_flg |= 0x80000;
        if (!(G.Unk75EA & 1)) {
            EM10_W(pEm)->Unk18 = 150;
            G.Unk75EA |= 1;
        }
        break;
    }
}
