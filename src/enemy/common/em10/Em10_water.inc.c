/* shared by em10, em16 */
/* RE2 Em10_water: splashes */
void Em10_water(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    SVECTOR Sv1;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    CompM((MATRIX *)pEm->Workm_m, PW_M(pP), &pP->Workm);
    Sv.vx = pP->Workm.t[0];
    Sv.vy = pEm->Water;
    Sv.vz = pP->Workm.t[2];
    Sv1.vx = 300;
    Sv1.vy = 0;
    Sv1.vz = 0;
    ApplyMatrixSV((MATRIX *)pEm->Workm_m, &Sv1, &Sv1);
    Sv1.vx += Sv.vx;
    Sv1.vy += Sv.vy;
    Sv1.vz += Sv.vz;
    if (pEm->Water == 1) {
        Sv1.vx = 0x400;
        Sv1.vy = 0;
        Sv1.vz = 0;
        Esp_call(0x20E, (USHORT)pEm->Cdir_y | 0x20000000, &GsIDMATRIX, &Sv, &Sv1);
    } else {
        Esp_call(0x61C, (((Rnd_l() << 2) + 0xC00) << 16) | (USHORT)(pEm->Cdir_y - (Rnd_l() << 2)), &GsIDMATRIX, &Sv1, 0);
        Esp_call(0x61C, (((Rnd_l() << 2) + 0xC00) << 16) | (USHORT)(0x800 + pEm->Cdir_y - (Rnd_l() << 2)), &GsIDMATRIX, &Sv, 0);
        Esp_call(0x161C, (((Rnd_l() << 2) + 0x800) << 16) | (USHORT)(Rnd_l() << 4), &GsIDMATRIX, &Sv, 0);
        Esp_call(0x161C, (((Rnd_l() << 2) + 0xC00) << 16) | (USHORT)(Rnd_l() << 4), &GsIDMATRIX, &Sv, 0);
        Sv1.vx = 0x400;
        Sv1.vy = 0;
        Sv1.vz = 0;
        Esp_call(0x20E, (USHORT)pEm->Cdir_y | 0x20000000, &GsIDMATRIX, &Sv, &Sv1);
    }
}
