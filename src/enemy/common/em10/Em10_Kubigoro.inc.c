/* shared by em10, em16 */
/* RE2 Em10_Kubigoro: the head rolling after it was shot off */
void Em10_Kubigoro(ENEMY_WORK *pEm)
{
    MATRIX M;
    SVECTOR Tmp_sv = { 300, -300, 0 };
    VECTOR Tv0;
    SHORT Ground;
    SHORT Ground2;
    PARTS_W *pP;

    pP = &pEm->pSin_parts_ptr[1];
    if (EM10_W(pEm)->Flg & 0x80) {
        pEm->Neck_flg = 1;
        pP->Be_flg |= 0x22;
        EM10_W(pEm)->Flg &= 0xFF7F;
        EM10_W(pEm)->Flg |= 0x100;
        M = GsIDMATRIX;
        RotMatrixY(pEm->Cdir_y + 0x800, &M);
        ApplyMatrixSV(&M, &Tmp_sv, (SVECTOR *)&pP->Old_x);
        pP->Unk85 = 2;
    }
    M = GsIDMATRIX;
    RotMatrixZ(0x80, &M);
    RotMatrixX(0x40, &M);
    CompM(&pP->Workm, &M, &pP->Workm);
    Tv0 = *(VECTOR *)pP->Workm.t;
    Ground = func_8004D720(pP->Workm.t, 100, 0x2000, 1);
    pP->Workm.t[0] += pP->Old_x;
    pP->Workm.t[1] += pP->Old_y;
    pP->Workm.t[2] += pP->Old_z;
    Ground2 = func_8004D720(pP->Workm.t, 100, 0x2000, 1);
    if (Ground > Ground2) {
        *(VECTOR *)pP->Workm.t = Tv0;
        Ground2 = Ground;
        pP->Old_x *= -1;
        pP->Old_z *= -1;
        Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x3E80000, &pP->Workm, 0, 0);
        if (pP->Unk85-- == 0) {
            EM10_W(pEm)->Flg &= 0xFEFF;
            pP->Be_flg = 0;
            Esp_kill(0x305, &pP->Workm);
            return;
        }
    }
    pP->Old_y += 50;
    if (Ground2 - 200 < pP->Workm.t[1]) {
        pP->Workm.t[1] = Ground2 - 200;
        pP->Old_x >>= 1;
        pP->Old_z >>= 1;
        pP->Old_y = -(pP->Old_y >> 2);
        Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x3E80000, &pP->Workm, 0, 0);
        Snd_se_enem(9, pEm);
        if (pP->Unk85-- == 0) {
            EM10_W(pEm)->Flg &= 0xFEFF;
            pP->Be_flg = 0;
            Esp_kill(0x305, &pP->Workm);
        }
    }
}
