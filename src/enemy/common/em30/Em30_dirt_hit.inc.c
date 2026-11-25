/* shared by em30, em33 */
/* dirt thrown up at the hit part, with its sound */
void Em30_dirt_hit(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pOya;
    SVECTOR Sv;
    VECTOR Pos;
    LONG n = 2;
    ULONG Dir;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        do {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = -(Rnd() & 0xFF);
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd() & 3) * 0x300 + 0x1000;
            Sv.vx += pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[0];
            Sv.vz += pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[2];
            if (Rnd() & 1)
                Esp_call(0x7000700, (Dir << 16) | (USHORT)pOya->Cdir_y, &GsIDMATRIX, &Sv, NULL);
            else
                Esp_call(0x7000100, (Dir << 16) | (USHORT)pOya->Cdir_y, &GsIDMATRIX, &Sv, NULL);
        } while (n--);
    } else {
        do {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = -(Rnd() & 0xFF) - 0x7F;
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd() & 3) * 0x300 + 0x1000;
            Sv.vx += pEm->pSin_parts_ptr[8].Workm.t[0];
            Sv.vz += pEm->pSin_parts_ptr[8].Workm.t[2];
            if (Rnd() & 1)
                Esp_call(0x7000700, (Dir << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Sv, NULL);
            else
                Esp_call(0x7000100, (Dir << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Sv, NULL);
        } while (n--);
    }
    Pos.vx = Sv.vx;
    Pos.vy = Sv.vy;
    Pos.vz = Sv.vz;
    Snd_se_stad(0x1020F, &Pos, 0, 0);
}
