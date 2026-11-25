/* shared by em10, em16 */
/* RE2 Em10_mv01_br */
void Em10_mv01_br(ENEMY_WORK *pEm)
{
    SHORT Dir0;
    SHORT Dir1;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);
    if (!EM10_W(pEm)->u2C.w.Unk2F) {
        if ((pEm->Unk158 & 0xC0000000) && (pEm->Unk158 & 0x3FFFFFFF) < 1500 && !Goto01_ck(pEm, pEm->Unk130, 0x100)) {
            EM_RTN(pEm) = 0xE01;
            return;
        }
        if (!EM10_W(pEm)->u2C.w.Unk2F && !Dir0 && LL < 1500 && pEm->Pos_y != pPl->Pos_y && !pPl->Damage_cnt
            && !(pEm->Status_flg & 0x800))
            EM_RTN(pEm) = 0xE01;
    }
    if (!Dir0 && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40)) {
        EM_RTN(pEm) = 0x301;
        if (pPl->Be_flg & 0x10000000)
            EM_RTN(pEm) = 0x401;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
    if (Dir1 && LL < 2000 && !pPl->Damage_cnt && !EM10_W(pEm)->Unk26)
        EM_RTN(pEm) = 0xC01;
}
