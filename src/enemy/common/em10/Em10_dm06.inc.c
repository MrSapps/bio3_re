/* shared by em10, em16 */
/* RE2 Em10_haidm: damage while crawling (Type & 1) */
void Em10_dm06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Ofs[2] = { -450, 250 };      /* unused */
    SVECTOR Sv;
    LONG Mode;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 7, Rnd_l() & 7, 3);
        pEm->Routine_2 = 1;
        Mode = 0;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        if (pEm->Unk12C != 0x1A) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(Mode << 24, 0x17700000, &pP->Workm, &Sv, 0);
        }
        if (!EM10_W(pEm)->Dam_s) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        pEm->Type = 1;
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
}
