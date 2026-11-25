/* shared by em20, em25 */
/* RE2 Ck_em_id (EM_SUB): number of live works of kind Id other than pEm, listed into ppList */
LONG Ck_em_id(ENEMY_WORK *pEm, ENEMY_WORK **ppList, LONG Id, LONG Num)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pT;
    LONG Cnt = 0;

    for (ppEm = &G.pEnemy[0]; ppEm != G.ppEnemy_end; ppEm++) {
        pT = *ppEm;
        if ((pT->Be_flg & 1) && pT->Id == Id && pT != pEm) {
            Cnt++;
            if (ppList && Num) {
                *ppList++ = pT;
                Num--;
            }
        }
    }
    return Cnt;
}
