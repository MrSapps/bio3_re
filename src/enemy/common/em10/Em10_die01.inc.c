/* shared by em10, em16 */
/* RE2 Em10_haidie: death while crawling (Type & 1) */
void Em10_die01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };           /* unused */
    SVECTOR Sv;
    LONG Mode;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 8, 0, 0xF);
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        Mode = 0;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = 300;
        Sv.vz = 0;
        Esp_call(Mode << 24, (USHORT)pEm->Cdir_y | 0x1FA00000, &pP->Workm, &Sv, 0);
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}
