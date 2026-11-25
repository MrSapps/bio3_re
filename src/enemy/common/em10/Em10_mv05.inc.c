/* shared by em10, em16 */
/* RE2 Em10_mv05: empty in RE3 (only the locals are left) */
void Em10_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Type_mot[4][2] = { { 12, 12 }, { 15, 15 }, { 12, 12 }, { 15, 15 } }; /* as Em10_mv03 */
    SVECTOR Sv;                        /* unused */
    MATRIX M;                          /* unused */
    UCHAR Atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } }; /* unused */
    ENEMY_WORK *pPl;
    ULONG Flg;

    pPl = G_PLAYER;
    Flg = pEm->Type & 1;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, Type_mot[Flg][0], 0, 0xF);
        pEm->Routine_2 = 1;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->Routine_0 = 1;
        pPl->Routine_1 = 13;
        pPl->Routine_2 = 0;
        pPl->Damage_cnt |= 0x80;
        if (!EM10_W(pEm)->Dam_s) {
            if (Rnd_l() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        pEm->Damage_cnt &= 0x7F;
    case 1:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_SET_MOVE(pEm, Type_mot[Flg][0] + 1, 0, 0);
        pEm->Routine_2 = 3;
        pEm->Timer0 = 13;
    case 3:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        if (pEm->Timer0-- == 0)
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        break;
    }
}
