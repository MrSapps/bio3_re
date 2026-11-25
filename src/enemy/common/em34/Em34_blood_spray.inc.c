/* shared by em34, em36 */
/* blood spray of a hit (Unk132 & 0x100 / 0x400) */
void Em34_blood_spray(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    MATRIX *pM2;
    SVECTOR Ofs;

    pP = pEm->pSin_parts_ptr;
    pEm->Status_flg |= 1;
    Esp_call(0x205, (Rnd() * 16 + 0x1000) << 16, &pP[13].Workm, NULL, NULL);
    Esp_call(0x1001B03, (Rnd() * 16 + 0xA00) << 16, &pP[13].Workm, NULL, NULL);
    if ((pEm->Unk132 & 3) == 1) {
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[11].Workm;
        Esp_call(0x205, (Rnd() * 16 + 0x1000) << 16, pM, NULL, NULL);
        Esp_call(0x1001B03, (Rnd() * 16 + 0xA00) << 16, pM, NULL, NULL);
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[14].Workm;
        Esp_call(0x205, (Rnd() * 16 + 0x1000) << 16, pM, NULL, NULL);
        Esp_call(0x1001B03, (Rnd() * 16 + 0xA00) << 16, pM, NULL, NULL);
    } else {
        pM2 = (MATRIX *)pEm->pSin_parts_ptr;
        pM2 = &((PARTS_W *)pM2)[9].Workm;
        Ofs.vx = 0x80 - Rnd();
        Ofs.vy = -300;
        Ofs.vz = 0x80 - Rnd();
        Esp_call(0x205, (Rnd() * 16 + 0x1F00) << 16, pM2, &Ofs, NULL);
        Esp_call(0x1001B03, (Rnd() * 16 + 0x1000) << 16, pM2, &Ofs, NULL);
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[3].Workm;
        Esp_call(0x205, (Rnd() * 16 + 0x1000) << 16, pM, NULL, NULL);
        Esp_call(0x1001B03, (Rnd() * 16 + 0xA00) << 16, pM, NULL, NULL);
    }
}
