/* shared by em10, em16 */
/* RE2 Em10_Damage: status effects of the hit (fire, acid, ...), then the Em10_dm routine of the weapon and state */
void Em10_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;                        /* unused (frame size) */
    LONG Lim;

    if (!pEm->Routine_2) {
        if ((pEm->Unk132 & 0x100) && !(pEm->Status_flg & 1))
            Em10_Firedm(pEm);
        if ((pEm->Unk132 & 0x400) && !(pEm->Status_flg & 1))
            Em10_Tudm(pEm);
        if ((pEm->Unk132 & 0x200) && !(pEm->Status_flg & 2))
            Em10_Spdm(pEm);
        if ((pEm->Unk132 & 0x800) && !(pEm->Status_flg & 2))
            Em10_Ryudm(pEm);
    }
    if (pEm->Type & 1) {
        Em10_haidm[pEm->Unk12C - 1](pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Etc_flg & 0x10) {
        Em10_dm07(pEm, pKan, pSeq);
        return;
    }
    if (pEm->Status_flg & 0x400) {
        EM10_W(pEm)->Dm_cnt = 0xF;
        EM10_W(pEm)->Dm_r0 = 0;
    }
    Lim = 0;
    if (!pEm->Routine_2 && EM10_W(pEm)->Dm_cnt <= Lim) {
        if (EM10_W(pEm)->Dm_r0 == 1 || (pEm->Unk12C == 1 && (LONG)G.Status_flg >= 0)) {
            Em10_blood_set(pEm);
            EM10_W(pEm)->Dm_r0 = 1;
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            EM10_W(pEm)->Dm_cnt = (Rnd_l() & 0xF) + 0x10;
            return;
        }
        EM10_W(pEm)->Dm_r0 = 1;
    }
    if (EM10_W(pEm)->Etc_flg & 2) {
        pEm->UnkE3 = 1;
        EM_SET_RTN(pEm, 1, 5, 6, 0);
        return;
    }
    Em10_dm[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
}
