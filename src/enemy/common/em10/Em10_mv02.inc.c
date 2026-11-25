/* shared by em10, em16 */
/* RE2 Em10_mv02: walking towards the player */
void Em10_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, EM10_W(pEm)->Unk27, 0, 0x1F);
        pEm->Timer0 = (Rnd_l() & 0x7F) + 300;
        if (!EM10_W(pEm)->Unk21) {
            if (!(Rnd_l() & 1))
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Move_cnt = Rnd_l() & 0x1F;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x = 0;
        pEm->UnkDA = (Rnd_l() & 0x1F) + 45;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = (Rnd_l() & 0xF) + 8;
    case 1:
        if (!pEm->UnkE2)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->UnkE3);
        else {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x10);
            pEm->UnkE2--;
        }
        if (pEm->Col.Hit_flg & 0x30) {
            if (pEm->UnkDA-- == 0) {
                pEm->UnkDA = (Rnd_l() & 0x1F) + 45;
                pEm->UnkE2 = (Rnd_l() & 0x1F) + 45;
            }
        }
        if (!pEm->Move_cnt && (Rnd_l() & 7) == 1)
            pEm->UnkE2 = (Rnd_l() & 0x1F) + 15;
        Spd = pEm->Spd_x;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pEm->Spd_x += pEm->Spd_x * 3 / 10 + 3;
        pEm->Spd_x -= ((pEm->Spd_x - Spd) * pEm->Hokan_flg) >> 5;
        Add_speedXZ(pEm, 0);
        if (pEm->Timer0-- == 0 || (pPl->Damage_cnt && (LONG)pEm->Unk154 < 3000)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (!pPl->Damage_cnt && (LONG)pEm->Unk154 > 4000 && (Rnd_l() & 7) == 2)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        break;
    }
}
