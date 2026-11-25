/* shared by em10, em16 */
/* RE2 Em10_mv01: walking */
void Em10_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Dir;
    SHORT Spd;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, EM10_W(pEm)->Walk_no, 0, 0x1F);
        pEm->UnkDA = (Rnd_l() >> 3) + 100;
        pEm->UnkE2 = (Rnd_l() & 0xF) + 30;
        pEm->UnkE3 = (Rnd_l() & 0xF) << 1;
        pEm->Timer0 = (Em10_walk_rnd[pEm->UnkE3] & 0x7FFF) + (Rnd_l() & 0x3F);
        pEm->Move_cnt = Rnd_l() & 0x1F;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x = 0;
    case 1:
        if (pEm->Timer0-- == 0) {
            if (++pEm->UnkE3 >= 32)
                pEm->UnkE3 = 0;
            pEm->Timer0 = (Em10_walk_rnd[pEm->UnkE3] & 0x7FFF) + (Rnd_l() & 0x1F);
            pEm->Cdir_y -= 0x10 - ((Rnd_l() & 1) << 5);
        }
        if ((LONG)pEm->Unk154 > 5000)
            Dir = 8 - ((Em10_walk_rnd[pEm->UnkE3] >> 15) << 4);
        else
            Dir = 8;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Dir);
        if (pEm->UnkE2-- == 0) {
            pEm->UnkE2 = (Rnd_l() & 0xF) + 30;
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
            if (!EM10_W(pEm)->Dam_s) {
                if (!(Rnd_l() & 0x1F)) {
                    Snd_se_enem(13, pEm);
                    EM10_W(pEm)->Dam_s = 150;
                } else if (!(Rnd_l() & 0x1F)) {
                    Snd_se_enem(14, pEm);
                    EM10_W(pEm)->Dam_s = 150;
                }
            }
        }
        if ((LONG)pEm->Unk154 < 3000)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Dir * 2);
        Spd = pEm->Spd_x;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pEm->Spd_x += pEm->Spd_x * 3 / 10 + 3;
        pEm->Spd_x -= ((pEm->Spd_x - Spd) * pEm->Hokan_flg) >> 5;
        Add_speedXZ(pEm, 0);
        if (--pEm->UnkDA == 0) {
            pEm->UnkDA = (Rnd_l() >> 3) + 100;
            if (!(Rnd_l() & 3) && (LONG)pEm->Unk154 < 5000)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        if ((LONG)pEm->Unk154 < 4000 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400) && !(G.Player.Damage_cnt & 0x80)) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (EM10_W(pEm)->Etc_flg & 0x4000)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        break;
    }
}
