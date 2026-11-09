#include "common.h"
#include "main.h"
#include "mojidisp.h"
#include "monitor.h"
#include "pad.h"
#include "trans.h"
#include "line_scr.h"
#include "em_sub.h"
#include "pl_wep.h"
#include "sub.h"
#include "joint.h"

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020298);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800204B8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020700);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020820);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800209D8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020B6C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020C0C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020DBC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80020EB8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80021548);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002174C);

void func_80021784(void) {
}

void func_8002178C(void) {
}

void func_80021794(void) {
}

void func_8002179C(void) {
}

void func_800217A4(void) {
}

void func_800217AC(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800217B4);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800218BC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002198C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80021D58);

void func_80021EA4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80021EAC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800221F0);

void func_80022398(void) {
}

void func_800223A0(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800223A8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022500);

void func_80022660(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022668);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002289C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022990);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022CCC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80022FA8);

void Trans_work_init(void)
{
    POLY_FT4 *pP2;
    ULONG D2;

    pP2 = (POLY_FT4 *)Trans.Prim_poly_ft4;
    D2 = 0x300;
    do {
        pP2->tag = 0x9000000;
        *(ULONG *)&pP2->r0 = 0x2C808080;
        pP2++;
        D2--;
    } while (D2 != 0);
    Trans_pointer_set();
}

void Trans_pointer_set(void)
{
    Trans.pPoly_ft4_keep = Trans.Prim_poly_ft4[G.Cbuf_id];
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80023268);

void Gun_light_set(void)
{
    LIGHT_CUT_DATA *pLight;
    SVECTOR v;

    pLight = Get_light_cut(&G.Pl_pos_x);
    func_800100A4(Game.Plight_bak, pLight, 0x28);
    pLight->Mode[2] = 0;
    pLight->L[2] = 1;
    v.vx = 0x4B0;
    v.vz = 0;
    Rot_vector(G.Pl_cdir_y, &v, &v);
    Game.Plight_col[0] = 0xFF;
    Game.Plight_col[1] = 0xE6;
    Game.Plight_col[2] = 0xA0;
    Game.Plight_pos.vx = G.Pl_pos_x + v.vx;
    Game.Plight_pos.vy = G.Pl_pos_y - 0x960;
    Game.Plight_pos.vz = G.Pl_pos_z + v.vz;
    switch (Game.Plight_cnt) {
    case 0:
        {
            UCHAR Col_lvl = 0xFF;

            if (pLight->Col[2][0] < Col_lvl) {
                pLight->Col[2][0] = Col_lvl;
            }
        }
        if (pLight->Col[2][1] < Game.Plight_col[1]) {
            pLight->Col[2][1] = Game.Plight_col[1];
        }
        if (pLight->Col[2][2] < Game.Plight_col[2]) {
            pLight->Col[2][2] = Game.Plight_col[2];
        }
        Game.Plight_cnt++;
        break;
    case 1:
        {
            UCHAR Col_lvl = 0x7F;

            if (pLight->Col[2][0] < Col_lvl) {
                pLight->Col[2][0] = Col_lvl;
            }
        }
        if (pLight->Col[2][1] < Game.Plight_col[1] >> 1) {
            pLight->Col[2][1] = Game.Plight_col[1] >> 1;
        }
        if (pLight->Col[2][2] < Game.Plight_col[2] >> 1) {
            pLight->Col[2][2] = Game.Plight_col[2] >> 1;
        }
        Game.Plight_cnt++;
        break;
    }
    pLight->Pos[2][0] = Game.Plight_pos.vx;
    pLight->Pos[2][1] = Game.Plight_pos.vy;
    pLight->Pos[2][2] = Game.Plight_pos.vz;
    pLight->L[2] = 6000;
}

void Gun_light_reset(void)
{
    func_800100A4(Get_light_cut(&G.Pl_pos_x), Game.Plight_bak, 0x28);
    if (Game.Plight_cnt >= 2) {
        Game.Plight_flg = 0;
        Game.Plight_cnt = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800245A0);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800248E4);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80024B34);

void func_80024C50(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80024C58);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80024CE8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80024F74);

void Flash_fade_in(void)
{
    func_8003884C(Vib_data_tbl, 0);
    func_800746C0(0x22E, NULL, 0, 0);
    Fade_set(2, 0, 0, 1, 0, 0xFFFFFF, 4);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
}

void Flash_fade_out(void)
{
    ULONG i;

    Fade_set(2, 0, 0, 2, 0xFFFFFF, 0, 3);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
    Fade_set(2, 0, FADE_HOLD, 1, 0, 0x606060, 1);
    i = 0x22;
    do {
        Task_sleep(1);
    } while (i-- != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800251FC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800253F0);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800254AC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80025610);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80025A28);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80025C04);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80025DE8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80025FB0);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002606C);

ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top)
{
    pEm->Parts_num = ((UCHAR *)pEm->pTmd)[-4];
    pEm->pSin_parts_ptr = (PARTS_W *)work_top;
    return work_top + pEm->Parts_num * sizeof(PARTS_W);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800261AC);

void Parts_work_pos_init(ENEMY_WORK *pEm, SHORT *pKan)
{
    SHORT *pPos;
    ULONG n;
    PARTS_W *pCur;

    pPos = &pKan[4];
    n = pEm->Parts_num;
    pCur = pEm->pSin_parts_ptr;
    while (n-- != 0) {
        pCur->Old_x = pCur->Pos_x = pPos[0];
        pCur->Old_y = pCur->Pos_y = pPos[1];
        pCur->Old_z = pCur->Pos_z = pPos[2];
        pPos += 3;
        pCur++;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002646C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80026498);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80026648);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80026728);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80026900);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80026BE8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80026CF4);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002720C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80027338);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80027568);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002783C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80027940);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80027A54);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80027F8C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800281B4);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80028308);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800288C8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80028B1C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80028BE8);

void line_trans(LONG Line_no, LONG Slide_ofs, LONG Copy_ofs, LONG Draw_ofs)
{
    LINE_SCR *pLine;
    DR_MOVE *pMove;
    LONG Pos;
    LONG D0;
    RECT Rect;

    pLine = &Line;
    if ((Slide_ofs | Copy_ofs) == 0) {
        return;
    }
    if (Slide_ofs < 0 || Slide_ofs >= pLine->Rect.w) {
        return;
    }
    Rect.w = pLine->Rect.w - Slide_ofs;
    Rect.h = 1;
    Rect.x = pLine->Rect.x + Slide_ofs;
    Pos = Line_no + Draw_ofs;
    pMove = &pLine->Line_move[G.Cbuf_id][Line_no];
    if (Line_no + Copy_ofs < 0xF0) {
        D0 = Pos + Copy_ofs;
        if (Line_no + Copy_ofs < 0) {
            Rect.y = Draw_ofs;
            goto set;
        }
    } else {
        D0 = Draw_ofs + 0xEF;
    }
    Rect.y = D0;
set:
    SetDrawMove(pMove, &Rect, pLine->Rect.x, Pos);
    if (Copy_ofs < 0) {
        addPrim(pLine->pOt, pMove);
    } else {
        *pLine->pPrim_keep0 = (*pLine->pPrim_keep0 & 0xFF000000) | ((ULONG)pMove & 0xFFFFFF);
        pLine->pPrim_keep0 = (ULONG *)pMove;
    }
}

void line_scr_end(void)
{
    *Line.pPrim_keep0 = (*Line.pPrim_keep0 & 0xFF000000) | (Line.Keep_tag0 & 0xFFFFFF);
}

void line_mode_set(ULONG Ot_no, ULONG Prio)
{
    LINE_SCR *pLine;
    ULONG *pOt;
    ULONG *pNull;
    ULONG Id;
    tagGLOBAL *pG;

    pG = &G;
    pLine = &Line;
    Id = pG->Cbuf_id;
    switch (Ot_no) {
    case 0:
        pOt = pG->pOt0 + Prio;
        break;
    case 1:
        pOt = pG->pOt1 + Prio;
        break;
    case 2:
        pOt = pG->pOt2 + Prio;
        break;
    }
    pNull = &pLine->Null_prim[Id];
    addPrim(pOt, pNull);
    pLine->Keep_tag0 = *pNull;
    pLine->pPrim_keep0 = pNull;
    pLine->pOt = pOt;
}

void Line_work_init(void)
{
    UCHAR *p = Line.Unk2D20;

    Line.Null_prim[0] = 0;
    Line.Null_prim[1] = 0;
    p[0] = 2;
    p[1] = 0x40;
    p[2] = 2;
    p[3] = 2;
    p[4] = 0x40;
    p[5] = 2;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80028F38);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029294);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029700);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_800297FC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029988);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029B94);

void func_80029D00(void) {
}

void System_trans_all(void)
{
    func_8002A49C();
    Curtain_trans();
    Moji_trans_main();
}

void Curtain_trans(void)
{
    tagMAIN *pMain = &Main;
    DBUF *pDb = pMain->pDb;

    if (G.Status_flg & STAT_CINESCO) {
        if (pMain->Curtain_kido < 0xF0) {
            pMain->Curtain_kido += 0x10;
        } else {
            pMain->Curtain_kido = 0xFF;
        }
    } else if (pMain->Curtain_kido >= 0x10) {
        pMain->Curtain_kido -= 0x10;
    } else {
        pMain->Curtain_kido = 0;
    }
    pDb->Curtain[0].r0 = pDb->Curtain[0].g0 = pDb->Curtain[0].b0 =
        pDb->Curtain[1].r0 = pDb->Curtain[1].g0 = pDb->Curtain[1].b0 = pMain->Curtain_kido;
    if (!(G.System_flg & SYS_DOOR_INIT) && pMain->Curtain_kido != 0) {
        AddPrim(G.pOt0 + 4, &pDb->Curtain[0]);
        AddPrim(G.pOt0 + 4, &pDb->Curtain[1]);
        AddPrim(G.pOt0 + 4, &pDb->Curtain_Dm);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_80029E3C);

void Soft_reset(void)
{
    RECT rect;

    Pad_vib_init();
    func_800522D8();
    rect.w = 0x140;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1E0;
    DrawSync(0);
    VSync(0);
    ResetCallback();
    ClearImage(&rect, 0, 0, 0);
    Bg_set_mode(1, 0);
    Task_kill(0);
    Task_kill(1);
    Task_kill(2);
    Snd_reset();
    func_80029988();
    VSync(0);
    G.Unk20F8 = -1;
    Task_lexecute(0, NULL);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002A2B8);

void Bg_set_mode(ULONG Mode, ULONG Rgb)
{
    Main.Bg_mode = Mode;
    if (Mode == 1) {
        *(ULONG *)&Main.Db[1].Bg_clr.r0 = Rgb;
        *(ULONG *)&Main.Db[0].Bg_clr.r0 = Rgb;
    }
}

void Fade_set(ULONG No, UCHAR Pri, ULONG Flg, ULONG Abr, ULONG Rgb0, ULONG Rgb1, USHORT Cnt_max)
{
    FADE *pF;
    USHORT tpage;

    pF = &Main.Fade[No];
    pF->Rgb0[0] = Rgb0;
    pF->Rgb0[1] = Rgb0 >> 8;
    pF->Rgb0[2] = Rgb0 >> 16;
    pF->Rgb1[0] = Rgb1;
    pF->Rgb1[1] = Rgb1 >> 8;
    pF->Rgb1[2] = Rgb1 >> 16;
    tpage = GetTPage(0, Abr, 0, 0);
    SetDrawTPage(&pF->Tpage[0], 1, 0, tpage);
    SetDrawTPage(&pF->Tpage[1], 1, 0, tpage);
    Flg |= FADE_ON;
    pF->Rect.w = 0x140;
    pF->Pri = Pri;
    pF->Flg = Flg;
    pF->Cnt_max = Cnt_max;
    pF->Cnt = 0;
    pF->Rect.x = 0;
    pF->Rect.y = 0;
    pF->Rect.h = 0xF0;
}

void Fade_rect_set(ULONG No, RECT *Rect)
{
    FADE *pF = &Main.Fade[No];

    pF->Rect.x = Rect->x;
    pF->Rect.y = Rect->y;
    pF->Rect.w = Rect->w;
    pF->Rect.h = Rect->h;
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002A49C);

void Fade_off(ULONG No)
{
    Main.Fade[No].Flg = 0;
}

ULONG Fade_status(ULONG No)
{
    return Main.Fade[No].Flg;
}

void Fade_out_reset(void)
{
    tagGLOBAL *pG = &G;
    FADE *pF = &Main.Fade[0];
    TILE *pT = &pF->T[pG->Cbuf_id];

    switch (pF->Flg) {
    case 0:
        DrawSync(0);
        PutDispEnv(&Main.pDb->Disp);
        *(ULONG *)&pT->tag = 0x3000000;
        *(ULONG *)&pT->r0 = 0x62202020;
        *(ULONG *)&pT->x0 = 0;
        *(ULONG *)&pT->w = 0xF00140;
        SetDrawTPage(&pF->Tpage[pG->Cbuf_id], 1, 0, GetTPage(0, 2, 0, 0));
        pF->Cnt_max = 8;
        pF->Flg = 1;
    case 1:
        AddPrim(pG->pOt0, pT);
        AddPrim(pG->pOt0, &pF->Tpage[pG->Cbuf_id]);
        DrawSync(0);
        VSync(0);
        DrawOTag(pG->pOt0 + 7);
        DrawSync(0);
        if (--pF->Cnt_max == 0) {
            pF->Flg = 0;
            G.System_flg &= ~SYS_FADE_OUT;
        }
        break;
    }
}

ULONG Cut_check(ULONG Flg)
{
    tagGLOBAL *pG = &G;
    VCUT *cp;
    ULONG next_cut;

    for (cp = pG->Cccut_next + 1; cp->Fcut == pG->Next_cut_no; cp++) {
        if (cp->Be_flg != 0 && (cp->nFloor == -0x80 || cp->nFloor == pG->Pl_nFloor) &&
            func_8001020C(&pG->Pl_pos_x, cp) != 0) {
            next_cut = cp->Tcut;
            goto apply_cut;
        }
    }
    if (Flg == 0) {
        return 0;
    }
    next_cut = pG->Next_cut_no;
apply_cut:
    Cut_change(next_cut);
    return 1;
}

void Cut_change(ULONG Fc)
{
    G.Next_cut_no = Fc;
    G.Cccut_next = Ccut_search(Fc);
}

VCUT *Ccut_search(ULONG Fc)
{
    VCUT *pCcut;

    pCcut = G.pRoom->pVcut;
    while (pCcut->Fcut != Fc) {
        pCcut++;
    }
    return pCcut;
}

void Card_event_set(void)
{
    EnterCriticalSection();
    Ev0 = OpenEvent(0xF4000001, 4, 0x2000, NULL);
    Ev1 = OpenEvent(0xF4000001, 0x100, 0x2000, NULL);
    Ev2 = OpenEvent(0xF4000001, 0x2000, 0x2000, NULL);
    Ev3 = OpenEvent(0xF4000001, 0x8000, 0x2000, NULL);
    Ev10 = OpenEvent(0xF0000011, 4, 0x2000, NULL);
    Ev11 = OpenEvent(0xF0000011, 0x100, 0x2000, NULL);
    Ev13 = OpenEvent(0xF0000011, 0x8000, 0x2000, NULL);
    ExitCriticalSection();
    EnableEvent(Ev0);
    EnableEvent(Ev1);
    EnableEvent(Ev2);
    EnableEvent(Ev3);
    EnableEvent(Ev10);
    EnableEvent(Ev11);
    EnableEvent(Ev13);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002AB34);

void Md_adr_cnv(MD_HEAD *pMd)
{
    ULONG n = pMd->n_obj;
    MD_OBJ *pObj = pMd->Obj;
    ULONG base = (ULONG)pObj;

    do {
        n--;
        pObj->pVert = (ULONG *)((ULONG)pObj->pVert + base);
        pObj->pNorm = (ULONG *)((ULONG)pObj->pNorm + base);
        pObj->pPrim3 = (ULONG *)((ULONG)pObj->pPrim3 + base);
        pObj->pPrim4 = (ULONG *)((ULONG)pObj->pPrim4 + base);
        pObj++;
    } while (n != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002AC98);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002ADB8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002AEE8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002AF9C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B190);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B2BC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B5FC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B6FC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002B86C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002BA3C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002C068);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002C680);

void Md_set_abr(MD_OBJ *pObj, ULONG Abr)
{
    ULONG n;
    ULONG *p;

    n = pObj->n_prim3;
    Abr <<= 21;
    p = pObj->pPrim3;
    while (n != 0) {
        n--;
        p[1] = (p[1] & ~0x600000) | Abr;
        p += 3;
    }
    n = pObj->n_prim4;
    p = pObj->pPrim4;
    while (n != 0) {
        n--;
        p[1] = (p[1] & ~0x600000) | Abr;
        p += 4;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002C824);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002CABC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002CE08);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002CF90);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D184);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D41C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D4B0);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D5D8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D6D8);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002D898);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DAAC);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DC98);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DE0C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002DFE0);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002E160);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002E318);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002E61C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002E74C);

void Draw_list_all(ULONG Arg0, ULONG Arg1, ULONG Arg2)
{
    ENEMY_WORK **ppEm;

    func_8002EB9C(Arg0, Arg1, Arg2, G.pUnk265C);
    if (G.pUnk2660->Be_flg & 1) {
        func_8002EB9C(Arg0, Arg1, Arg2, G.pUnk2660);
    }
    for (ppEm = G.pEnemy; ppEm != G.ppEnemy_end; ppEm++) {
        if ((*ppEm)->Be_flg & 1) {
            func_8002EB9C(Arg0, Arg1, Arg2, *ppEm);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002EB9C);

void Print_color(UCHAR A0, UCHAR Col, USHORT Attr)
{
    Moji.Unk75C9 = A0;
    Moji.Mess_attr = Attr | (Col << 3);
}

void Print8(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), Moji.Mess_attr, &pFmt);
}

void Print14(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), Moji.Mess_attr | 0x8000, &pFmt);
}

void Print8_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), (Moji.Mess_attr & 0xFF07) | (Col << 3), &pFmt);
}

void Print14_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    func_8002F440(X | (Y << 16), ((SHORT)(Moji.Mess_attr & 0xFF07) | 0x8000) | (Col << 3), &pFmt);
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002F440);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002F98C);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002FB14);

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002FD30);

void Moji_trans_main(void)
{
    MOJIDISP *pMoji = &Moji;

    if ((pMoji->Mess_flg & 0x40) && !(G.Key & 0x80)) {
        pMoji->Mess_flg &= ~0x40;
    }
    if (pMoji->Mess_flg & 0x80) {
        func_8002FEE0(pMoji);
        func_80030CB8(pMoji);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80020298", func_8002FEE0);
