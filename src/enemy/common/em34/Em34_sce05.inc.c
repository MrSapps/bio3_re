/* shared by em34, em36 */
/* scripted motion 5: plays the motion number set by the script (Add_dir) */
void Em34_sce05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->Add_dir;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        if (pEm->Unk14A) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        break;
    }
}
