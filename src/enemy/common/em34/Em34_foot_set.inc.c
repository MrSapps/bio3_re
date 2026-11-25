/* shared by em34, em36 */
/* RE2 Em21_foot_set: keeps the planted foot (parts 10-12 + 3 * Leg) in place */
void Em34_foot_set(ENEMY_WORK *pEm, LONG Unused, LONG Leg)
{
    PARTS_W *pP;
    MATRIX M;

    pP = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
    CompM((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &M);
    CompM(&M, (MATRIX *)pP[9].M, &M);
    pP += Leg * 3 + 12;
    CompM(&M, (MATRIX *)pP[-2].M, &M);
    CompM(&M, (MATRIX *)pP[-1].M, &M);
    CompM(&M, (MATRIX *)pP->M, &M);
    pEm->Pos_x -= M.t[0] - pP->Workm.t[0];
    pEm->Pos_z -= M.t[2] - pP->Workm.t[2];
}
