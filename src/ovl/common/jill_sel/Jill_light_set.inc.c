/* shared by jill_sel, select */
/* light No of LIGHTMATRIX/COLORMATRIX from a direction and a 0xBBGGRR colour (unused) */
void Jill_light_set(ULONG No, VECTOR *pVec, ULONG Rgb)
{
    SHORT *pL;
    SHORT *pC;
    VECTOR nrm;

    pL = LIGHTMATRIX.m[No];
    pC = &COLORMATRIX.m[0][No];
    VectorNormal(pVec, &nrm);
    pL[0] = nrm.vx * -1;
    pL[1] = nrm.vy * -1;
    pL[2] = nrm.vz * -1;
    pC[0] = (Rgb & 0xFF) << 4;
    pC[3] = (Rgb & 0xFF00) >> 4;
    pC[6] = (Rgb >> 12) & 0xFF0;
}
