/* shared by em34, em36 */
/* blood effect of a hit (RE2 Em2a_blood_set) */
void Em34_blood_set(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG Dir;
    LONG Dir2;
    SVECTOR Ofs;

    Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - (USHORT)pEm->Cdir_y;
    if (pEm->Unk132 & 0xA00)
        return;
    if ((pEm->Unk132 & 3) == 1) {
        pP = pEm->pSin_parts_ptr;
        Ofs.vx = 0x40 - (Rnd() & 0x7F);
        Ofs.vy = 1500;
        Ofs.vz = 0x40 - (Rnd() & 0x7F);
        Esp_call(0x700, (Dir & 0xFFFF) | 0x10000000, &pP->Workm, &Ofs, NULL);
    } else if ((pEm->Unk132 & 3) == 3) {
        pP = &pEm->pSin_parts_ptr[2];
        Ofs.vx = 0x40 - (Rnd() & 0x7F);
        Ofs.vy = 0;
        Ofs.vz = 0x40 - (Rnd() & 0x7F);
        Esp_call(0x700, (Dir & 0xFFFF) | 0x10000000, &pP->Workm, &Ofs, NULL);
    } else {
        pP = pEm->pSin_parts_ptr;
        Ofs.vx = 0x40 - (Rnd() & 0x7F);
        Ofs.vy = 200;
        Ofs.vz = 0x40 - (Rnd() & 0x7F);
        Esp_call(0x700, ((Rnd() * 2 + 0x1000) << 16) | (Dir & 0xFFFF), &pP->Workm, &Ofs, NULL);
    }
    if (pEm->Unk12C == 5) {
        Dir2 = Dir + 0x80;
        Esp_call(0x100, ((Rnd() * 2 + 0x1800) << 16) | ((Dir2 - Rnd()) & 0xFFFF), &pP->Workm, &Ofs, NULL);
        Esp_call(0x100, ((Rnd() * 2 + 0x1800) << 16) | ((Dir2 - Rnd()) & 0xFFFF), &pP->Workm, &Ofs, NULL);
    }
}
