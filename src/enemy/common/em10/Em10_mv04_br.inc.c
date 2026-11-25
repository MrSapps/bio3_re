/* shared by em10, em16 */
/* RE2 Em10_mv04_br (crawling) */
void Em10_mv04_br(ENEMY_WORK *pEm)
{
    ULONG LL;
    ENEMY_WORK *pPl;
    SHORT Dir;
    LONG Y;
    LONG Hit;

    pPl = &Pl_work;
    LL = pEm->L_pl;
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Y = pPl->Pos_y;
    Hit = Y - 400 < pEm->pSin_parts_ptr[1].Workm.t[1] && pEm->pSin_parts_ptr[1].Workm.t[1] < Y + 200;
    if (!Dir && LL < 1300 && !pPl->Damage_cnt && (pEm->nFloor == pPl->nFloor || Hit)
        && !((pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40)) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Damage_cnt |= 0x80;
        pPl->Damage_no = 0;
        pPl->Damage_cnt |= 0x80;
    }
    if (pEm->Col.Hit_flg & 0x40)
        func_8002646C_(pEm, 400, 40, pEm->Col.Unk39);
    else
        pEm->Be_flg |= 0x80000;
}
