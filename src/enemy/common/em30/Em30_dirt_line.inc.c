/* shared by em30, em33 */
/* while Unk16A is set: a line of dirt puffs from the hit part towards the player */
void Em30_dirt_line(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    SVECTOR Ofs;
    SVECTOR Step;
    MATRIX M;
    PARTS_W *pP;

    switch (pEm->Unk16A) {
    case 0:
        break;
    case 1:
        pEm->Unk16A = 2;
        pEm->Unk16B = 0;
    case 2:
        Sv.vx = 0;
        Sv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        Sv.vz = 0;
        RotMatrix(&Sv, &M);
        Sv.vx = -(pEm->Unk16B * 120);
        Sv.vy = -600;
        ApplyMatrixSV(&M, &Sv, &Ofs);
        Sv.vx = -120;
        Sv.vy = -600;
        Sv.vz = 0;
        ApplyMatrixSV(&M, &Sv, &Step);
        if (!(pEm->Type & 1))
            pP = pEm->pSin_parts_ptr;
        else
            pP = &G.pEnemy[EM30_SUB(pEm)->Oya_no]->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no];
        Sv.vx = pP->Workm.t[0] + Ofs.vx;
        Sv.vy = pP->Workm.t[1] + Ofs.vy;
        Sv.vz = pP->Workm.t[2] + Ofs.vz;
        Esp_call(0x7001900, (0x1280 - pEm->Unk16B * 70 - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx - (SHORT)(Rnd() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1300 - pEm->Unk16B * 70 - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + (Step.vx << 1);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + (Step.vz << 1);
        Esp_call(0x7001900, (0x1380 - (pEm->Unk16B << 7) - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 3 - (SHORT)(Rnd() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 3 - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1400 - (pEm->Unk16B << 7) - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + (Step.vx << 2);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + (Step.vz << 2);
        Esp_call(0x7001900, (0x1480 - (pEm->Unk16B << 7) - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 5 - (SHORT)(Rnd() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 5 - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1500 - (pEm->Unk16B << 7) - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 6;
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 6;
        Esp_call(0x7001900, (0x1580 - (pEm->Unk16B << 7) - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 7 - (SHORT)(Rnd() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 7 - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1600 - (pEm->Unk16B << 7) - Rnd()) << 16, &GsIDMATRIX, &Sv, NULL);
        if (pEm->Unk16B++ > 16)
            pEm->Unk16A = 0;
    }
}
