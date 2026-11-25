/* shared by em30, em33 */
/* chain the body parts: accumulated roll, each part hangs from the previous one */
void Em30_body_chain(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    PW_FREE(pP)[0] = pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[1];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[2];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[3];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[4];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[5];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[6];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[0], &pEm->pSin_parts_ptr[1], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[1], &pEm->pSin_parts_ptr[2], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[2], &pEm->pSin_parts_ptr[3], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[3], &pEm->pSin_parts_ptr[4], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[4], &pEm->pSin_parts_ptr[5], 0x1000);
    Em30_part_hang(pEm, &pEm->pSin_parts_ptr[5], &pEm->pSin_parts_ptr[6], 0x1000);
}
