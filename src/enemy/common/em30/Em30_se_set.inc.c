/* shared by em30, em33 */
/* positional sound No at pPos, from the second bank when the room uses pEm's */
void Em30_se_set(ULONG No, ENEMY_WORK *pEm, LONG *pPos)
{
    VECTOR Pos;

    Pos.vx = pPos[0];
    Pos.vy = pPos[1];
    Pos.vz = pPos[2];
    if (pEm->Unk12F == Snd_edt[3])
        No += 0x10;
    Snd_se_stad(No | 0x10300, &Pos, 0, 0);
}
