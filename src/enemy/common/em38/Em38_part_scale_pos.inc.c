/* shared by em38, em39 */
/* part position from the animation's bone offsets scaled by Rate */
void Em38_part_scale_pos(ENEMY_WORK *pEm, PARTS_W *pP, LONG Rate)
{
    SHORT *pOfs = (SHORT *)(pEm->pKan_t_ptr + 2);

    pOfs += pP->Parts_no * 3;
    pP->Pos_x = (pOfs[0] * Rate) >> 12;
    pP->Pos_y = (pOfs[1] * Rate) >> 12;
    pP->Pos_z = (pOfs[2] * Rate) >> 12;
}
