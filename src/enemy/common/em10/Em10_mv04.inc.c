/* shared by em10, em16 */
/* RE2 Em10_mv04: crawling */
void Em10_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF0006;
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() & 7) + 7;
        pEm->Move_cnt = Rnd_l() & 0xF;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->UnkDA = 0;
    case 1:
        if (pEm->Spd_x > 20) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
            if (pEm->Timer0-- == 0) {
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
                pEm->Timer0 = (Rnd_l() & 7) + 7;
            }
        }
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        if (pEm->Col.Hit_flg & 0x30) {
            if (++pEm->UnkDA > 150) {
                pEm->Routine_2 = 2;
                pEm->Timer0 = (Rnd_l() & 7) + 7;
            }
        }
        break;
    case 2:
        if (pEm->Spd_x > 20) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x18);
            if (pEm->Timer0-- == 0) {
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x18);
                pEm->Timer0 = (Rnd_l() & 7) + 7;
            }
        }
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkDA-- == 0) {
            pEm->Routine_2 = 1;
            pEm->Timer0 = (Rnd_l() & 7) + 7;
        }
        break;
    }
}
