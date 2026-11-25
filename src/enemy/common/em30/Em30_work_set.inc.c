/* shared by em30, em33 */
/* allocate a new work (copy of pEm) at the end of the work list (RE2 Enemy_work_set style) */
ENEMY_WORK *Em30_work_set(ENEMY_WORK *pEm)
{
    tagGLOBAL *pG = &G;
    ULONG n = 0;

    while (&pG->pEnemy[n] != pG->ppEnemy_end)
        n++;
    pG->Em_num++;
    pG->pEnemy[n] = pG->pEm_mem;
    pG->ppEnemy_end = &pG->pEnemy[n + 1];
    pG->pEm_mem++;
    *pG->pEnemy[n] = *pEm;
    return pG->pEnemy[n];
}
