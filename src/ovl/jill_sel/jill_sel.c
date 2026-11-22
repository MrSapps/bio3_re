#include "ovl/jill_sel.h"
#include "cd.h"
#include "mojidisp.h"
#include "pcstr.h"
#include "status.h"

/* main task */
void Jill_main(void)
{
    JILL_WORK *pW = &Jill_wk;

    for (;;) {
        Jill_proc_tbl[pW->Routine](pW);
        Jill_sprt_draw(pW);
        Task_sleep(1);
    }
}

void Jill_sprt_draw(JILL_WORK *pW)
{
    Print_color(0xFF, 0, 0x103);
    Jill_obj_draw(&Jill_obj[0], 3);
    Jill_obj_draw(&Jill_obj[3], 3);
    Jill_obj_draw(&Jill_obj[4], 3);
    Jill_obj_draw(&Jill_obj[1], 3);
    Jill_obj_draw(&Jill_obj[2], 3);
}

void Jill_pl_move(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pEm;
    ULONG i;
    ULONG n;

    for (n = 5, i = 0; n != 0; i++, n--) {
        pEm = (&pG->pEnemy[0])[i];
        pG->pEm = pEm;
        Jill_pl_idle(pEm);
    }
}

void Jill_pl_idle(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_0) {
    case 0:
        pEm->Routine_0 = 1;
        *(ULONG *)&pEm->Move_no = 0x70000;
    case 1:
        Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
        break;
    }
}

void Jill_init(JILL_WORK *pW)
{
    tagGLOBAL *pG = &G;
    ULONG *pFlg;
    ULONG n;
    UCHAR *pHeap;
    ULONG Size;
    ULONG i;

    Task_suspend(0);
    pW->Vloop_bak = Main.Vloop;
    Main.Vloop = 2;
    Main.Unk191 = 1;
    Jill_bg_load();
    Bg_set_mode(2, 0);
    Snd_se_stad(0x700, NULL, 0, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 15);
    Set_view(Jill_view_eye, Jill_view_ref);
    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(300);
    pW->pHeap = (UCHAR *)0x80100000;
    Jill_obj_init(&Jill_obj[0], &Jill_sprt_def[0], 0x1EF0015, 0x808080);
    Jill_obj[0].Flg &= ~1;
    pW->UnkC = 0;
    pW->Open_num = 0;
    pFlg = pG->Unk77F8;
    for (n = pG->Save_cnt; n != 0;) {
        if (!Flg_ck(pFlg, 0)) {
            Flg_on(pFlg, 0);
            n--;
            pW->Open_num++;
        } else if (!Flg_ck(pFlg, 1)) {
            Flg_on(pFlg, 1);
            n--;
            pW->Open_num++;
        } else if (!Flg_ck(pFlg, 2)) {
            Flg_on(pFlg, 2);
            n--;
            pW->Open_num++;
        } else if (!Flg_ck(pFlg, 3)) {
            Flg_on(pFlg, 3);
            n--;
            pW->Open_num++;
        } else if (!Flg_ck(pFlg, 4)) {
            Flg_on(pFlg, 4);
            n--;
            pW->Open_num++;
        } else {
            n--;
        }
    }
    D_800DBB58 = 0;
    pW->Routine = 1;
    pW->Flg = 0;
    if (!Flg_ck(G.Unk77F8, 10)) {
        pW->Flg |= 1;
        Jill_obj[0].Flg |= 1;
        Jill_obj_init(&Jill_obj[1], &Jill_sprt_def[9], 0x1F00016, 0x808080);
        Jill_obj_init(&Jill_obj[2], &Jill_sprt_def[10], 0x1F00016, 0x808080);
        switch (pW->Open_num) {
        case 1:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[11], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[11], 0x1F00016, 0x808080);
            Jill_obj[4].Flg &= ~1;
            break;
        case 2:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[12], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[12], 0x1F00016, 0x808080);
            Jill_obj[4].Flg &= ~1;
            break;
        case 3:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[13], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[14], 0x1F00016, 0x808080);
            break;
        case 4:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[15], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[16], 0x1F00016, 0x808080);
            break;
        case 5:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[17], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[17], 0x1F00016, 0x808080);
            Jill_obj[4].Flg &= ~1;
            break;
        }
        Mess_set(0xB90000, 0x3403, (ULONG)&Jill_mess[Jill_mess_ofs[pW->Open_num - 1]], 0);
    } else {
        Jill_obj_init(&Jill_obj[1], &Jill_sprt_def[1], 0x1F00016, 0x808080);
        Jill_obj_init(&Jill_obj[2], &Jill_sprt_def[1], 0x1F00016, 0x808080);
        Jill_obj[2].Flg &= ~1;
        switch (pW->Open_num) {
        case 1:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[2], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[2], 0x1F00016, 0x808080);
            Jill_obj[4].Flg &= ~1;
            break;
        case 2:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[3], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[3], 0x1F00016, 0x808080);
            Jill_obj[4].Flg &= ~1;
            break;
        case 3:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[4], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[5], 0x1F00016, 0x808080);
            break;
        case 4:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[6], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[7], 0x1F00016, 0x808080);
            break;
        case 5:
            Jill_obj_init(&Jill_obj[3], &Jill_sprt_def[8], 0x1F00016, 0x808080);
            Jill_obj_init(&Jill_obj[4], &Jill_sprt_def[8], 0x1F00016, 0x808080);
            Jill_obj[4].Flg &= ~1;
            break;
        }
        Mess_set(0xB90000, 0x3403, (ULONG)&Jill_mess[Jill_mess_ofs[pW->Open_num + 4]], 0);
    }
    Flg_on(G.Unk77F8, 10);
    Size = sizeof(ENEMY_WORK);         /* a variable: the step is kept in a register */
    pHeap = pW->pHeap;
    pW->pHeap = pHeap + sizeof(ENEMY_WORK) * 5;
    for (n = 5, i = 0; n != 0; i++, n--) {
        (&pG->pEnemy[0])[i] = (ENEMY_WORK *)pHeap;
        ((ENEMY_WORK *)pHeap)->Be_flg = 1;
        pHeap += Size;
        (&pG->pEnemy[0])[i]->Id = 0;
    }
    pW->pHeap = pHeap;
}

/* wait for a button */
void Jill_wait(JILL_WORK *pW)
{
    if (G.Key_trg & 0x3000)
        pW->Routine = 2;
    if (G.Unk20FC & 0x800)
        pW->Routine = 2;
}

/* fade out and exit */
void Jill_exit(JILL_WORK *pW)
{
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 30);
    while (!(Fade_status(0) & 4)) {
        Jill_sprt_draw(pW);
        Task_sleep(1);
    }
    D_800DBB58 = 0;
    Main.Vloop = pW->Vloop_bak;
    Task_signal(0);
    Task_exit();
}

/* RE2 Select_pl_load: model file, texture, parts work; returns the end of the parts work */
UCHAR *Jill_pl_load(ENEMY_WORK *pEm, ULONG TpClut, UCHAR *pWork)
{
    UCHAR *p;
    LONG *pHdr;
    UCHAR Cclut;
    UCHAR Ctpage;
    LONG Ofs;

    Cd_read(Jill_pl_cd[pEm->Id - 2], (ULONG *)(p = pWork), 1, "PLD");
    pHdr = (LONG *)(p + *(LONG *)p);
    pEm->pSeq_t_ptr = (ULONG *)(p + pHdr[0]);
    pEm->pKan_t_ptr = (ULONG *)(p + pHdr[1]);
    pEm->Clut = TpClut >> 16;
    pEm->Tpage = TpClut;
    pEm->pTmd = (ULONG *)(p + pHdr[2]);
    Cclut = G.Ctp.u.Cclut;
    Ctpage = G.Ctp.u.Ctpage;
    G.Ctp.u.Cclut = pEm->Clut;
    G.Ctp.u.Ctpage = pEm->Tpage;
    Mapping_tim((ULONG *)(p + pHdr[4]));
    G.Ctp.u.Cclut = Cclut;
    G.Ctp.u.Ctpage = Ctpage;
    DrawSync(0);
    func_8002AB34(pEm->pTmd, pEm->Tpage, pEm->Clut);
    pEm->pTmd += 2;
    Ofs = pHdr[3];
    p = (UCHAR *)func_800261AC(pEm, PartsWork_set(pEm, (ULONG)(p + pHdr[4])), pEm->pKan_t_ptr);
    if (*(ULONG *)(0x801D5C00 + Ofs) != 0) {   /* fixed address of the model file (select reads pWork + Ofs) */
        func_8007A2D4(pEm, (ULONG *)(pWork + Ofs));
    }
    pEm->Be_flg = 1;
    pEm->Unk12A = 0x1000;
    return p;
}

/* RE2 Select_pl_trans */
void Jill_pl_trans(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Be_flg, LONG *pPos)
{
    ULONG n;

    if (!(Be_flg & 0x10)) {
        n = pEm->Parts_num;
    loop:
        if ((pParts->Be_flg & 1) && (pParts->UnkA2 & 1)) {
            func_80025610(pEm, pParts, pParts->UnkA2, &pParts->Workm);
        }
        n--;
        pParts++;
        if (n != 0) {
            goto loop;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/jill_sel/jill_sel", Jill_wep_load);

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

/* background load */
void Jill_bg_load(void)
{
    Cd_read(0x36, (ULONG *)0x8019C000, 0, "Jill BG");
    Cd_read(0x38, (ULONG *)0x80100000, 0, (char *)Jill_str);
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* "JIll"; the bytes after it are the original linker's fill before .text */
const char Jill_str[8] = { 'J', 'I', 'l', 'l', 0, 0x0E, 0x03, 0xD3 };

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

void Jill_obj_rgb(SEL_OBJ *pObj, ULONG Rgb)
{
    pObj->Sp[0].Rgb = (pObj->Sp[0].Rgb & 0xFF000000) | Rgb;
    pObj->Sp[1].Rgb = (pObj->Sp[1].Rgb & 0xFF000000) | Rgb;
}
