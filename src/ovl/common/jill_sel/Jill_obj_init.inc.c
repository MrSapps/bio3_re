/* shared by jill_sel, select */
/* sprite init from a definition; TpClut = texture page | clut row << 16 */
void Jill_obj_init(SEL_OBJ *pObj, SEL_SPRT_DEF *pDef, ULONG TpClut, ULONG Rgb)
{
    SEL_SPRT *pSp;
    DR_MODE *pDr;
    ULONG Tp;
    LONG X;

    pObj->Flg = 1;
    pObj->Timer = 0;
    pObj->pOya = NULL;
    pObj->X = pDef->X;
    pObj->Y = pDef->Y;
    pSp = &pObj->Sp[0];
    pObj->Ofs_x = pDef->Ofs_x;
    pObj->Ofs_y = pDef->Ofs_y;
    pObj->Mode = 0xFF;
    pSp->Rgb = Rgb;
    pSp->X = pDef->X;
    pSp->Y = pDef->Y;
    pSp->W = pDef->W;
    pSp->H = pDef->H;
    pSp->U = pDef->U;
    pDr = &pObj->Dr[0];
    pSp->V = pDef->V;
    pSp->Clut = (TpClut >> 16) << 6;
    SetSprt((SPRT *)pSp);
    SetSemiTrans(pSp, pDef->Abr);
    Tp = TpClut & 0xFFFF;
    X = Tp << 6;
    if (Tp >= 16) {
        X -= 0x400;
    }
    SetDrawMode(pDr, 0, 0, GetTPage(1, pDef->Tp, X, (Tp >= 16) << 8), NULL);
    pObj->Sp[1] = pObj->Sp[0];
    pObj->Dr[1] = pObj->Dr[0];
}
