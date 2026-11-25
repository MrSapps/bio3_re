/* shared by em30, em33 */
/* chain the body parts: accumulated roll, each part hangs from the previous one */
void Em30_body_chain(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    EM30_PW(pP)->Roll_z = pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[1];
    EM30_PW(pP)->Roll_z = EM30_PW(pP - 1)->Roll_z + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[2];
    EM30_PW(pP)->Roll_z = EM30_PW(pP - 1)->Roll_z + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[3];
    EM30_PW(pP)->Roll_z = EM30_PW(pP - 1)->Roll_z + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[4];
    EM30_PW(pP)->Roll_z = EM30_PW(pP - 1)->Roll_z + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[5];
    EM30_PW(pP)->Roll_z = EM30_PW(pP - 1)->Roll_z + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[6];
    EM30_PW(pP)->Roll_z = EM30_PW(pP - 1)->Roll_z + pP->Cdir_z;
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[0], &pEm->pSin_parts_ptr[1], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[1], &pEm->pSin_parts_ptr[2], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[2], &pEm->pSin_parts_ptr[3], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[3], &pEm->pSin_parts_ptr[4], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[4], &pEm->pSin_parts_ptr[5], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[5], &pEm->pSin_parts_ptr[6], 0x1000);
}
