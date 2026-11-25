/* shared by jill_sel, select */
/* RE2 Select_pl_trans */
void Jill_pl_trans(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Be_flg, LONG *pPos)
{
    ULONG n;

    if (!(Be_flg & 0x10)) {
        n = pEm->Parts_num;
    loop:
        if ((pParts->Be_flg & 1) && (pParts->UnkA2 & 1)) {
            ko_joint_trans2(pEm, pParts, pParts->UnkA2, &pParts->Workm);
        }
        n--;
        pParts++;
        if (n != 0) {
            goto loop;
        }
    }
}
