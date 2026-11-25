/* shared by em10, em16 */
/* RE2 Em10_mv00_br */
void Em10_mv00_br(ENEMY_WORK *pEm)
{
    SHORT Dir0;
    SHORT Dir1;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);
    if (pEm->Type & 0x10)
        return;
    if (LL < 5000 && !Dir0 && (pEm->Status_flg & 0x2000))
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dir1 && LL < 2000 && !pPl->Damage_cnt && !EM10_W(pEm)->Unk26)
        EM_SET_RTN(pEm, 1, 0xC, 0, 0);
}
