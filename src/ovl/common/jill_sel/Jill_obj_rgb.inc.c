/* shared by jill_sel, select, result */

void Jill_obj_rgb(SEL_OBJ *pObj, ULONG Rgb)
{
    pObj->Sp[0].Rgb = (pObj->Sp[0].Rgb & 0xFF000000) | Rgb;
    pObj->Sp[1].Rgb = (pObj->Sp[1].Rgb & 0xFF000000) | Rgb;
}
