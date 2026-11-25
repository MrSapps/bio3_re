/* shared by em30, em33 */
/* hang part pP1 from part pP0 and turn it towards pP0 */
void Em30_part_hang(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate)
{
    VECTOR Pos;
    VECTOR Tip;
    SVECTOR Rot;
    LONG Dx;
    LONG Dz;
    LONG a;

    pP1->Be_flg |= 2;
    ApplyMatrixLV(&pP0->Workm, (VECTOR *)&pP1->Pos_x, &Pos);
    Pos.vx = pP0->Workm.t[0] + Pos.vx * Rate / 4096;
    Pos.vy = pP0->Workm.t[1] + Pos.vy * Rate / 4096;
    Pos.vz = pP0->Workm.t[2] + Pos.vz * Rate / 4096;
    Tip.vx = -800;
    Tip.vy = 0;
    Tip.vz = 0;
    ApplyMatrixLV(&pP1->Workm, &Tip, &Tip);
    Tip.vx = pP1->Workm.t[0] + Tip.vx;
    Tip.vy = pP1->Workm.t[1] + Tip.vy;
    Tip.vz = pP1->Workm.t[2] + Tip.vz;
    Dx = Pos.vx - Tip.vx;
    Dz = Pos.vz - Tip.vz;
    if (Dx) {
        a = catan((Dz << 12) / Dx);
        if (Dx < 0)
            Rot.vy = 0x800 - a;
        else
            Rot.vy = 0x1000 - a;
    } else if (Dz > 0)
        Rot.vy = 0xC00;
    else
        Rot.vy = 0x400;
    Rot.vx = 0;
    Rot.vz = EM30_PW(pP1)->Roll_z;
    RotMatrix_gte(&Rot, &pP1->Workm);
    pP1->Workm.t[0] = Pos.vx;
    pP1->Workm.t[1] = Pos.vy;
    pP1->Workm.t[2] = Pos.vz;
}
