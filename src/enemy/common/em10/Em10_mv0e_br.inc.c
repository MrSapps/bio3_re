/* shared by em10, em16 */
/* RE2 Em10_mv0e_br */
void Em10_mv0e_br(ENEMY_WORK *pEm)
{
    SHORT Dir;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    if (!Dir && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40)) {
        EM_RTN(pEm) = 0x301;
        if (Pl_work.Be_flg & 0x10000000)
            EM_RTN(pEm) = 0x401;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
}
