/* shared by em10, em16 */
/* RE2 Em10_die05: falling down dead after a head hit (Mot_dm picks the motion) */
void Em10_die05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Tbl[3][3] = { { 0x37, 0x17, 0x01 }, { 0x28, 0x18, 0x01 }, { 0x00, 0x01, 0x01 } };
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        EM10_W(pEm)->Etc_flg |= 4;
        if (EM10_W(pEm)->Mot_dm == 1)
            EM10_W(pEm)->Etc_flg &= 0xFFFB;
        if (pEm->Move_cnt < Tbl[EM10_W(pEm)->Mot_dm][0]) {
            EM_SET_MOVE(pEm, Tbl[EM10_W(pEm)->Mot_dm][1], 0, 0x1F);
            pEm->UnkE3 = 0;
        } else {
            EM10_W(pEm)->Etc_flg &= 0xFFFB;
            EM_SET_MOVE(pEm, Tbl[EM10_W(pEm)->Mot_dm][2], 0x1E, 0x1F);
            pEm->UnkE3 = 1;
        }
        pP = &pEm->pSin_parts_ptr[1];
        pEm->Routine_2 = 1;
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0, (USHORT)pEm->Cdir_y | 0x1FA00000, &pP->Workm, &Sv, 0);
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if (!pEm->UnkE3)
                pEm->Routine_2 = 2;
            else
                pEm->Routine_2 = 3;
        }
        break;
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x80);
        if (!pEm->Hokan_flg)
            pEm->Routine_2 = 3;
        break;
    case 3:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}
