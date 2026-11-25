/* shared by em10, em16 */
/* RE2 Em10_pldm00: the player (pEm) is bitten from the front or back (Routine_1 & 1) */
void Em10_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    UCHAR Mot[2] = { 0, 3 };
    SHORT Blo[2][3] = { { 0, 200, 100 }, { -100, 100, -200 } };
    ENEMY_WORK *pAi;
    ENEMY_WORK *pT;
    PARTS_W *pP;
    SHORT Old_x;
    SHORT Old_z;
    SHORT Ck_flg;
    LONG i;

    pAi = pEm->pEnemy_ptr;
    if (pAi->Routine_0 == 2 || pAi->Routine_0 == 3) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        G.Player.Damage_cnt &= 0x7F;
        pEm->Unk120 &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        pEm->Be_flg |= 0x80000;
        return;
    }
    pEm->Be_flg |= 0x8000;
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 200, 20, pEm->Col.Unk39);
        EM_SET_MOVE(pEm, Mot[pEm->Routine_1], 0, 3);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        pEm->UnkE2 = 0;
        if (pEm->Routine_1 & 1)
            pEm->Timer0 = 0;
        Snd_se_stad(((Rnd_l() & 1) << 1) | 0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->UnkDA = 0;
        pEm->Unk12C = 0;
        pEm->Unk120 |= 0x12;
    case 1:
        Goto01(pEm, pAi->Cdir_y + pEm->Timer0, 0xD0);
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 2;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 2;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        EM_SET_MOVE(pEm, Mot[pEm->Routine_1] + 1, 0, 0);
        pEm->Routine_2 = 3;
    case 3:
        if (pEm->Move_cnt == 15) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            Esp_call(0, (USHORT)(pEm->Cdir_y + (Rnd_l() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0x17D00000,
                          &pP[1].Workm, &Sv, 0);
            pEm->UnkE2 = 0;
        }
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        Joint_move2(pEm, pKan, pSeq, 0x800);
        break;
    case 4:
        EM_SET_MOVE(pEm, Mot[pEm->Routine_1] + 2, 0, 7);
        pEm->Routine_2 = 5;
        if (pEm->UnkE2) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            Esp_call(0, (USHORT)(pEm->Cdir_y + (Rnd_l() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0x17D00000,
                          &pP[1].Workm, &Sv, 0);
        }
    case 5:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 3;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        if ((UCHAR)(pEm->Move_cnt - 9) < 5) {
            Ck_flg = pEm->Col.Unk3A;
            Ck_flg <<= 2;
            i = 2;
            while (Ck_flg) {
                if ((Ck_flg & 0x8000) && !((pEm->UnkDA >> i) & 1)) {
                    pT = G.pEnemy[i];
                    if ((pT->Id & 0x70) == 0x10 && pT->Life >= 0 && !(pT->Type & 1) && !pT->Damage_cnt
                        && !(EM10_W(pT)->Etc_flg & 8) && !(pT->Type & 0x2000)) {
                        EM_SET_RTN(pT, 1, 9, 0, 0);
                        pT->UnkE3 = ~Cdir_ck2(pEm, pT) & 1;
                    }
                }
                i++;
                Ck_flg <<= 1;
            }
            pEm->UnkDA |= pEm->Col.Unk3A;
        }
        break;
    case 6:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        pEm->Be_flg |= 0x80000;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk120 &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        pEm->Damage_cnt = 3;
        pEm->Be_flg |= 0x10000000;
        break;
    }
}
