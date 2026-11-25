/* shared by em10, em16 */
/* RE2 Em10_mv10: vomiting */
void Em10_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x12, 0, 0xF);
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Move_cnt == 20) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 300;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0x18, (USHORT)pEm->Cdir_y | 0x10000000, &pP[1].Workm, &Sv, 0);
            Snd_se_enem(7, pEm);
        }
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            EM10_W(pEm)->u2C.w.Unk2F = 30;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}
