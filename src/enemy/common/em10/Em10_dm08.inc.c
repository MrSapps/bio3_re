/* shared by em10, em16 */
/* RE2 Em10_dm08: knocked back by a blast */
void Em10_dm08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;                        /* unused */
    UCHAR Mot[8] = { 27, 28, 29, 28, 27, 29, 27, 28 };
    LONG Idx;
    LONG No;
    LONG Mv;

    switch (pEm->Routine_2) {
    case 0:
        Idx = Rnd_l() & 7;
        Mv = Mot[Idx];
        No = pEm->Move_no;
        if (No != Mv)
            EM_SET_MOVE(pEm, Mv, 0, 6);
        else
            EM_MOVE(pEm) = (ULONG)(No - 26 + (Rnd_l() & 1)) % 3 + 0x6001B;
        pEm->Routine_2 = 1;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        Em10_blood_set(pEm);
        if (!EM10_W(pEm)->Dam_s) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        pEm->Spd_x = (Rnd_l() & 0x1F) + 60;
        pEm->Spd_z = 0x40 - (Rnd_l() & 0x7F);
        Add_speedXZ(pEm, pEm->Timer0 - pEm->Cdir_y + 0x800);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (Rnd_l() & 1)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        break;
    }
}
