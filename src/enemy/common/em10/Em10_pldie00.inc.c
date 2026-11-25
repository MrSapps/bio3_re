/* shared by em10, em16 */
/* RE2 Em10_pldie00: the player (pEm) is eaten */
void Em10_pldie00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    SVECTOR Sv;
    UCHAR Mot[2] = { 13, 15 };

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, Mot[pEm->Routine_1], 0, 0xF);
        func_8002646C_(pEm, 200, 20, pEm->Col.Unk39);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        if (pEm->Routine_1 & 1)
            pEm->Timer0 = 0;
        pEm->Unk120 |= 1;
        Snd_se_stad(0x10003, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk120 |= 0x12;
        Pad_vib_fade_set(0x21, 0x32, 0x96, 0);
        Pad_vib_set1(8, 0xFA, 0x12);
        Pad_vib_set0(0x32, 0x32);
        Pad_vib_set1(6, 0xFA, 0x3C);
        Pad_vib_set1(6, 0xFA, 0x47);
        Pad_vib_set1(6, 0xFA, 0x64);
        Pad_vib_fade_set(0x5A, 0xFA, 0, 0x6A);
    case 1:
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x100);
        if (pEm->Move_cnt >= 41 && pEm->Move_cnt < 99 && !(pEm->Move_cnt & 1)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0, ((Rnd_l() << 3) + 0xBD0) << 16 | (USHORT)(Rnd_l() << 4), &pP[1].Workm, &Sv, 0);
        }
        if (pEm->Move_cnt == 50) {
            PARTS_W *pW;

            pW = &pEm->pSin_parts_ptr[1];
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
            pW = pEm->pSin_parts_ptr;
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
            pW = &pEm->pSin_parts_ptr[2];
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
            pW = &pEm->pSin_parts_ptr[5];
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
        }
        if (pEm->Move_cnt >= 100 && pEm->Move_cnt < 105) {
            PARTS_W *pS;

            pS = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x17D00000, &pS[1].Workm, &Sv, 0);
            pS = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x1BD00000, &pS[1].Workm, &Sv, 0);
        }
        break;
    case 2:
        pEm->Routine_2 = 3;
        pEm->Move_no++;
        pEm->Move_cnt = 0;
        pEm->Life = -0x8000;
    case 3:
        pEm->Hokan_flg = 10;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x100);
        break;
    case 4:
        pEm->Routine_2 = 5;
        pEm->UnkE3 = (Rnd_l() & 0x1F) + 3;
    case 5:
        if (pEm->UnkE3-- == 0)
            pEm->Routine_2 = 3;
        break;
    case 6:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}
