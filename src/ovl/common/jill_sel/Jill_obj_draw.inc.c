/* shared by jill_sel, select, result */
/* sprite update and draw: Mode 0/1 pulse up/down, 2 shown, 3 hidden (blink) */
void Jill_obj_draw(SEL_OBJ *pObj, LONG Ot)
{
    SEL_SPRT *pSp;
    DR_MODE *pDr;
    SEL_OBJ *pOya;
    UCHAR Flg;

    Flg = pObj->Flg;
    if (!(Flg & 1)) {
        return;
    }
    if (Flg & 8) {
        pObj->Flg = Flg & ~8;
        if (Flg & 4) {
            pObj->Mode = 0;
        }
        if (pObj->Flg & 2) {
            pObj->Mode = 2;
            pObj->Timer = 10;
        }
    }
    switch (pObj->Mode) {
    case 0:
        pObj->Sp[0].Rgb += 0x80808;
        pObj->Sp[1].Rgb += 0x80808;
        if ((UCHAR)pObj->Sp[1].Rgb == 0xB0) {
            pObj->Mode = 1;
        }
        break;
    case 1:
        pObj->Sp[0].Rgb -= 0x80808;
        pObj->Sp[1].Rgb -= 0x80808;
        if ((UCHAR)pObj->Sp[1].Rgb == 0x50) {
            pObj->Mode = 0;
        }
        break;
    case 2:
        if (--pObj->Timer == 0xFF) {
            pObj->Timer = 10;
            pObj->Mode = 3;
        }
        break;
    case 3:
        if (--pObj->Timer == 0xFF) {
            pObj->Timer = 10;
            pObj->Mode = 2;
        }
        return;
    }
    pSp = &pObj->Sp[G.Cbuf_id];
    pDr = &pObj->Dr[G.Cbuf_id];
    pOya = pObj->pOya;
    if (pOya != NULL) {
        pObj->X = pObj->Ofs_x + (pOya->X + pOya->Sp[0].W);
        pObj->Y = pOya->Y + pObj->Ofs_y;
    }
    pSp->X = pObj->X;
    pSp->Y = pObj->Y;
    AddPrim(&G.Ot0[G.Cbuf_id][Ot], pSp);
    AddPrim(&G.Ot0[G.Cbuf_id][Ot], pDr);
}
