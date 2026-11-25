/* shared by em30, em33 */
/* dirt effect Kind around part No */
void Em30_dirt_set(ENEMY_WORK *pEm, LONG No, LONG Kind)
{
    SVECTOR Sv;
    ULONG Dir;

    Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
    Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
    Sv.vy = -50;
    Sv.vx += pEm->pSin_parts_ptr[No].Workm.t[0];
    Sv.vz += pEm->pSin_parts_ptr[No].Workm.t[2];
    Dir = (Rnd_l() & 3) << 9;
    Esp_call((Kind << 8) | 0x1000003, ((Dir | 0x2000) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Sv, NULL);
}
