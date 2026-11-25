/* shared by em30, em33 */
/* start the blood countdown of the head and its segments, later the further from the hit part (10, 13, 16, ...) */
void Em30_body_number(ENEMY_WORK *pEm)
{
    LONG Up, Dn;
    UCHAR Val = 10;
    LONG No;
    PARTS_W *pP;

    if (pEm->Type & 1) {
        Dn = EM30_SUB(pEm)->Parts_no;
        pEm = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        Up = Dn;
        if (Dn >= 7)
            Dn = 6;
    } else {
        Dn = 0;
        Up = 0;
    }
    for (; Up < 7 || Dn >= 0; Up++, Dn--, Val += 3) {
        if (Up < 7 && Up) {
            pP = &pEm->pSin_parts_ptr[Up];
            No = pP->Unk82;
            EM30_W(G.pEnemy[No])->u13.Blood_cnt = Val;
        }
        if (!Up)
            EM30_W(pEm)->u13.Blood_cnt = Val;
        if (Dn > 0) {
            pP = &pEm->pSin_parts_ptr[Dn];
            No = pP->Unk82;
            EM30_W(G.pEnemy[No])->u13.Blood_cnt = Val;
        }
        if (!Dn)
            EM30_W(pEm)->u13.Blood_cnt = Val;
    }
}
