/* shared by em10, em16 */
/* RE2 Em10_mv0d: staggering about */
void Em10_mv0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG LL;
    SHORT Dir;
    SHORT Spd;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, (Rnd_l() & 1) + 6, 0, 0xF);
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() & 0x3F) + 300;
        if (!EM10_W(pEm)->Unk21) {
            if (!(Rnd_l() & 1))
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Move_cnt = Rnd_l() & 0xF;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x = 0;
        pEm->UnkE3 = Rnd_l() & 0xF;
        pEm->UnkDA = Em10_mv0d_rnd[pEm->UnkE3] & 0x7FFF;
    case 1:
        if (pEm->UnkDA-- == 0) {
            if (++pEm->UnkE3 >= 16)
                pEm->UnkE3 = 0;
            pEm->UnkDA = Em10_mv0d_rnd[pEm->UnkE3] & 0x7FFF;
            pEm->Cdir_y -= 0x10 - ((Rnd_l() & 1) << 5);
        }
        if (Em10_mv0d_rnd[pEm->UnkE3] & 0x8000)
            Dir = -0x20;
        else
            Dir = 0x20;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Dir);
        if (LL < 3000)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x30);
        Spd = pEm->Spd_x;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd) * pEm->Hokan_flg) >> 4;
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
        if (pEm->Timer0-- == 0 || (pPl->Damage_cnt && (LONG)pEm->Unk154 < 3500))
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        break;
    }
}
