/* shared by em30, em33 */
/* morph key 0 of part 8 one step towards 0 (Dir != 0) or 0x1000 */
void Em30_morph_step(ENEMY_WORK *pEm, LONG Step, LONG Dir)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

    if (Dir) {
        EM30_SABUN_P(pP)->Key[0].Rate -= Step;
        if (EM30_SABUN_P(pP)->Key[0].Rate < 0)
            EM30_SABUN_P(pP)->Key[0].Rate = 0;
    } else {
        EM30_SABUN_P(pP)->Key[0].Rate += Step;
        if (EM30_SABUN_P(pP)->Key[0].Rate > 0x1000)
            EM30_SABUN_P(pP)->Key[0].Rate = 0x1000;
    }
    func_8004AC54(pEm, pP);
}
