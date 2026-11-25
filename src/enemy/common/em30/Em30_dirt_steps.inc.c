/* shared by em30, em33 */
/* while Blood_cnt counts down: blood spray at a few steps, with a sound at 8 */
void Em30_dirt_steps(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pOya;
    SVECTOR Sv;
    VECTOR Pos;
    LONG n = 1;

    if (!EM30_W(pEm)->u13.Blood_cnt)
        return;
    EM30_W(pEm)->u13.Blood_cnt--;
    if (EM30_W(pEm)->u13.Blood_cnt == 1 || EM30_W(pEm)->u13.Blood_cnt == 3 || EM30_W(pEm)->u13.Blood_cnt == 8) {
        if (EM30_W(pEm)->u13.Blood_cnt == 8) {
            if (pEm->Type & 1) {
                pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
                Pos.vx = pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[0];
                Pos.vy = pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[1];
                Pos.vz = pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[2];
            } else {
                Pos.vx = pEm->pSin_parts_ptr[8].Workm.t[0];
                Pos.vy = pEm->pSin_parts_ptr[8].Workm.t[1];
                Pos.vz = pEm->pSin_parts_ptr[8].Workm.t[2];
            }
            if (Rnd() & 3)
                Snd_se_stad(0x1020C, &Pos, 0, 0);
            else
                Snd_se_stad(0x1020D, &Pos, 0, 0);
        }
        while (n--) {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = (Rnd() & 0xFF) - 0x7F;
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            if (pEm->Type & 1) {
                pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
                Esp_call(0x2B, (USHORT)pOya->Cdir_y | 0x40000000, &pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm, &Sv, NULL);
            } else {
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x40000000, &pEm->pSin_parts_ptr[8].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[12].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[14].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[16].Workm, &Sv, NULL);
            }
        }
    }
}
