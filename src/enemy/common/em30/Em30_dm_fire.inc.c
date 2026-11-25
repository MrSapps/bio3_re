/* shared by em30, em33 */
/* dm[29]: hit by the grenade launcher's flame (and the like) */
void Em30_dm_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOya;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (!Flg_ck(&G.Unk7888, 0) && EM30_W(pOya)->Under != 1) {
            pOya->Unk12C = pEm->Unk12C;
            pOya->Unk132 = pEm->Unk132;
            pOya->Unk1C4 = EM_RTN(pOya);
            EM_RTN(pOya) = EM_RTN(pEm);
            Em30_body_number(pEm);
        }
        pEm->Damage_cnt = 10;
        EM_RTN(pEm) = pEm->Unk1C4;
    } else if (EM30_W(pEm)->Under == 1) {
        EM_RTN(pEm) = pEm->Unk1C4;
        Em30_Move(pEm, pKan, pSeq);
    } else {
        pEm->Life = -1;
        Flg_on(&G.Unk7888, 0);
        if (!EM30_W(pEm)->u13.Blood_cnt)
            Em30_body_number(pEm);
        EM_RTN(pEm) = pEm->Unk1C4;
        Em30_Move(pEm, pKan, pSeq);
        EM_SET_RTN(pEm, 3, 0, 0, 0);
    }
}
