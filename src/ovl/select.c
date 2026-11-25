#include "ovl/select.h"
#include "cd.h"

void func_800251FC_(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg, LONG *pPos) __asm__("func_800251FC"); /* 4 args here (canonical 3) */

/* RE2 Select_main */
void func_801C2094(void)
{
    SELECT_WORK *pSel = &Select_wk;

    pSel->Main_no = 0;
    pSel->Pad001 = 0;
    pSel->Moji_no = 0;
    pSel->Pad003 = 0;
    do {
        (*Select_main_proc_tbl[pSel->Main_no])();
        func_801C20FC();
        Task_sleep(1);
    } while (1);
}

/* RE2 Select_pl_main: sprites, players, message window */
void func_801C20FC(void)
{
    tagGLOBAL *pG = &G;
    SELECT_WORK *pSel;
    ULONG i;
    ULONG n;
    ENEMY_WORK *pEm;

    Jill_obj_draw(&Select_obj[0], 5);
    Jill_obj_draw(&Select_obj[1], 5);
    Jill_obj_draw(&Select_obj[2], 5);
    Jill_obj_draw(&Select_obj[3], 5);
    Jill_obj_draw(&Select_obj[4], 5);
    Jill_obj_draw(&Select_obj[5], 5);
    Jill_obj_draw(&Select_obj[21], 6);
    Jill_obj_draw(&Select_obj[22], 6);
    Jill_obj_draw(&Select_obj[23], 6);
    func_801C2E1C();
    pSel = &Select_wk;
    if (pSel->Main_no < 3) {
        func_801C41E0();
        Jill_obj_draw(&Select_obj[6], 5);
        Jill_obj_draw(&Select_obj[7], 5);
        for (n = 6, i = 0; n != 0; i++, n--) {
            func_801C4BAC(&Select_obj[8 + i], &Select_name_def[Select_name_idx[pSel->Cursor_no][i]], 0x1F10017);
        }
        Jill_obj_draw(&Select_obj[8], 3);
        Jill_obj_draw(&Select_obj[9], 3);
        Jill_obj_draw(&Select_obj[10], 3);
        Jill_obj_draw(&Select_obj[11], 3);
        Jill_obj_draw(&Select_obj[12], 3);
        Jill_obj_draw(&Select_obj[13], 3);
    } else {
        Select_obj[3].Y += 10;
    }
    pG->pEm = pG->pEnemy[0];
    pG->pUnk208C = (POLY_GT4 *)((ULONG *)pSel->pHeap + pG->Cbuf_id * 13);
    for (n = 3, i = 0; n != 0; i++, n--) {
        pEm = (&pG->pEnemy[0])[i];
        pG->pEm = pEm;
        pEm->pUnk7C = pG->pOt1;
        func_801C28D0(pEm);
        func_800251FC_(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, &pEm->Pos_x);
        Jill_pl_trans(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, &pEm->Pos_x);
    }
}

/* RE2 Select_init */
void func_801C235C(void)
{
    tagGLOBAL *pG = &G;
    SELECT_WORK *pSel = &Select_wk;
    SELECT_VIEW *pView = &Select_view;
    ENEMY_WORK *pEm;
    ENEMY_WORK *pEmTop;
    UCHAR *pWork;
    ULONG i;
    ULONG n;
    ULONG Size;

    Task_suspend(0);
    pSel->Vloop_bak = Main.Vloop;
    Main.Vloop = 2;
    Main.Unk191 = 1;
    func_801C4774();
    func_8007809C(0, 0xC, (ULONG *)0x801FBC00, (ULONG *)0x80180000);
    Bg_set_mode(2, 0);
    func_801C2DE0();
    Set_view(pView->Eye, pView->Ref);
    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(300);
    pSel->pHeap = (UCHAR *)0x80100000;
    pSel->Cursor_no = 1;
    Jill_obj_init(&Select_obj[0], &Select_sprt_def[0], 0x1EF0015, 0x808080);
    Jill_obj_init(&Select_obj[1], &Select_sprt_def[1], 0x1EF0015, 0x808080);
    Select_obj[1].pOya = &Select_obj[0];
    Jill_obj_init(&Select_obj[2], &Select_sprt_def[2], 0x1EF0015, 0x808080);
    Select_obj[2].pOya = &Select_obj[1];
    Jill_obj_init(&Select_obj[3], &Select_sprt_def[3], 0x1EF0015, 0x808080);
    Jill_obj_init(&Select_obj[4], &Select_sprt_def[4], 0x1EF0016, 0x808080);
    Select_obj[4].pOya = &Select_obj[3];
    Jill_obj_init(&Select_obj[5], &Select_sprt_def[5], 0x1EF0016, 0x808080);
    Select_obj[5].pOya = &Select_obj[4];
    Jill_obj_init(&Select_obj[6], &Select_sprt_def[6], 0x1EF0015, 0x808080);
    Select_obj[6].Flg |= 0xC;
    Jill_obj_init(&Select_obj[7], &Select_sprt_def[7], 0x1EF0015, 0x808080);
    Select_obj[7].Flg |= 0xC;
    Jill_obj_init(&Select_obj[8], &Select_sprt_def[8], 0x1F10017, 0x808080);
    Jill_obj_init(&Select_obj[9], &Select_sprt_def[9], 0x1F10017, 0x808080);
    Jill_obj_init(&Select_obj[10], &Select_sprt_def[10], 0x1F10017, 0x808080);
    Jill_obj_init(&Select_obj[11], &Select_sprt_def[11], 0x1F10017, 0x808080);
    Jill_obj_init(&Select_obj[12], &Select_sprt_def[12], 0x1F10017, 0x808080);
    Jill_obj_init(&Select_obj[13], &Select_sprt_def[13], 0x1F10017, 0x808080);
    Jill_obj_init(&Select_obj[21], &Select_sprt_def[21], 0x1EF0015, 0x808080);
    Jill_obj_init(&Select_obj[22], &Select_sprt_def[22], 0x1EF0015, 0x808080);
    Jill_obj_init(&Select_obj[23], &Select_sprt_def[23], 0x1EF0015, 0x808080);

    /* message window work, then the three player works */
    pWork = pSel->pHeap + sizeof(SELECT_MOJI);
    Select_pMoji = (SELECT_MOJI *)pSel->pHeap;
    pSel->pHeap = pSel->pHeap + sizeof(SELECT_MOJI) + sizeof(ENEMY_WORK) * 3;
    Size = sizeof(ENEMY_WORK);
    for (n = 3, i = 0; n != 0; i++, n--) {
        (&pG->pEnemy[0])[i] = (ENEMY_WORK *)pWork;
        ((ENEMY_WORK *)pWork)->Be_flg = 1;
        pWork += Size;
        (&pG->pEnemy[0])[i]->Id = 0;
    }

    for (n = 3, i = 0; n != 0; i++, n--) {
        pEm = pG->pEnemy[i];
        pG->pEm = pEm;
        pEm->Id = Select_pl_init[i].Id;
        pEm->Type = Select_pl_init[i].Type;
        pEm->Pos_y = Select_pl_init[i].Pos_y;
        pEm->Cdir_x = Select_pl_init[i].Cdir_x;
        pEm->Cdir_z = Select_pl_init[i].Cdir_z;
        pEm->Unk12A = Select_pl_init[i].Unk18;
        *(ULONG *)&pEm->Move_no = 0x70000;
        pWork = func_801C3830(pEm, (5 + i * 2) | ((i * 2) << 16), (ULONG *)pWork);
        pWork = Jill_wep_load(pEm, (ULONG *)pWork, i);
        pEm->Routine_0 = i;
        pEm->Routine_1 = 0;
        pEm->Routine_2 = 0;
        pEm->Unk154 = Select_dir_L[pSel->Cursor_no][i];
        pEm->Unk158 = Select_dir_L[pSel->Cursor_no][i];
        pEm->Cdir_y = Select_dir_fpos[pSel->Cursor_no][i];
        pEm->Pos_x = (rsin(pEm->Unk154) * 2000) >> 12;
        pEm->Pos_z = (rcos(pEm->Unk154) * 2000) >> 12;
        pEm->Spd_y = 0;
        pEm->Unk1BC[0] = Select_dir_fpos[1][i];
        func_801C2B34(pEm);
    }
    pSel->pHeap = pWork;
    pSel->Blink_cnt = 0;
    pSel->Blink_dir = 0;
    pSel->Flg = 0;
    pSel->Main_no = 2;
    pSel->Pad001 = 0;
    pSel->Moji_no = 0;
    Snd_se_stad(0x500, NULL, 0, 0);
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 15);
    while (Fade_status(0) != 0) {
        func_801C20FC();
        Task_sleep(1);
    }
}

/* RE2 Select_light_ctrl */
void func_801C28D0(ENEMY_WORK *pEm)
{
    SELECT_LIGHT *pLt;

    pLt = &Select_wk.Light[pEm->Routine_0];
    Select_light_proc_tbl[pLt->Wait](pEm, pLt);
    pLt->X = pEm->Pos_x + 0x4B0;
    pLt->Y = pEm->Pos_y - 1000;
    pLt->Z = pEm->Pos_z;
    func_801C2B94(pEm->pSin_parts_ptr->Workm.t, pEm);
    SetColorMatrix(&Select_light_col[pEm->Routine_0]);
    LIGHTMATRIX = Select_light_mtx[pEm->Routine_0];
    COLORMATRIX = Select_light_col[pEm->Routine_0];
}

/* RE2 Select_light_wait */
void func_801C2A34(ENEMY_WORK *pEm, SELECT_LIGHT *pLt)
{
    if (pEm->Routine_0 == Select_wk.Cursor_no) {
        pLt->Wait = 1;
    }
}

/* RE2 Select_light_up */
void func_801C2A58(ENEMY_WORK *pEm, SELECT_LIGHT *pLt)
{
    if (pEm->Routine_0 != Select_wk.Cursor_no) {
        pLt->Wait = 3;
        return;
    }
    if (pLt->R < 0xDD) {
        pLt->R += 0x14;
        pLt->G += 0x14;
        pLt->B += 0x14;
        return;
    }
    pLt->Wait = 2;
}

/* RE2 Select_light_on */
void func_801C2AB8(ENEMY_WORK *pEm, SELECT_LIGHT *pLt)
{
    if (pEm->Routine_0 != Select_wk.Cursor_no) {
        pLt->Wait = 3;
    }
}

/* RE2 Select_light_down */
void func_801C2ADC(ENEMY_WORK *pEm, SELECT_LIGHT *pLt)
{
    if (pEm->Routine_0 == Select_wk.Cursor_no) {
        pLt->Wait = 1;
        return;
    }
    if (pLt->R != 0) {
        pLt->R -= 0x14;
        pLt->G -= 0x14;
        pLt->B -= 0x14;
        return;
    }
    pLt->Wait = 0;
}

/* RE2 Select_light_init */
void func_801C2B34(ENEMY_WORK *pEm)
{
    SELECT_LIGHT *pLt;

    pLt = &Select_wk.Light[pEm->Routine_0];
    pLt->On = 1;
    pLt->Wait = 0;
    pLt->Range = 8000;
    pLt->R = 0;
    pLt->G = 0;
    pLt->B = 0;
    pLt->X = pEm->Pos_x + 0x4B0;
    pLt->Y = pEm->Pos_y - 1000;
    pLt->Z = pEm->Pos_z;
}

/* RE2 Select_light_calc */
void func_801C2B94(LONG *pPos, ENEMY_WORK *pEm)
{
    SELECT_LIGHT *pLt;
    VECTOR vec;
    UCHAR col[4];
    LONG k;

    pLt = &Select_wk.Light[pEm->Routine_0];
    vec.vx = (pPos[0] - pLt->X) >> 4;
    vec.vy = (pPos[1] - pLt->Y) >> 4;
    vec.vz = (pPos[2] - pLt->Z) >> 4;
    k = (pLt->Range >> 4) - SquareRoot0(vec.vx * vec.vx + vec.vz * vec.vz);
    if (k < 0) {
        k = 0;
    }
    if (pLt->Range != 0) {
        col[0] = pLt->R * k / (pLt->Range >> 4);
        col[1] = pLt->G * k / (pLt->Range >> 4);
        col[2] = pLt->B * k / (pLt->Range >> 4);
    } else {
        *(ULONG *)col = 0;
    }
    func_801C2CF4(pEm->Routine_0, &vec, col);
    SetBackColor(0x40, 0x40, 0x40);
}

/* RE2 Select_light_set */
void func_801C2CF4(ULONG No, VECTOR *pVec, UCHAR *pCol)
{
    SELECT_LIGHT *pLt;
    MATRIX *pMtx;
    MATRIX *pCl;
    VECTOR nrm;

    pLt = &Select_wk.Light[No];
    pMtx = &Select_light_mtx[No];
    pCl = &Select_light_col[No];
    VectorNormal(pVec, &nrm);
    pMtx->m[0][0] = nrm.vx * -pLt->On;
    pMtx->m[0][1] = nrm.vy * -pLt->On;
    pMtx->m[0][2] = nrm.vz * -pLt->On;
    pCl->m[0][0] = pCol[0] << 4;
    pCl->m[1][0] = pCol[1] << 4;
    pCl->m[2][0] = pCol[2] << 4;
}

/* RE2 Select_view_init */
void func_801C2DE0(void)
{
    Select_view.Unk = 0;
    Select_view.Eye[0] = 0x105E;
    Select_view.Eye[1] = -0xA8C;
    Select_view.Eye[2] = -0xAA0;
    Select_view.Ref[1] = -0x8FC;
    Select_view.Ref[0] = 0;
    Select_view.Ref[2] = 0x82;
}

/* RE2 Select_pl_move: per-character animation scripts */
void func_801C2E1C(void)
{
    tagGLOBAL *pG;
    ENEMY_WORK *pEm;
    ULONG i;

    for (i = 0; i < 3; i++) {
        pG = &G;
        pEm = (&pG->pEnemy[0])[i];
        pG->pEm = pEm;
        switch (pEm->Id) {
        case 0:
            break;
        case 8:
            switch (pEm->Routine_2) {
            case 0:
                *(ULONG *)&pEm->Move_no = 1;
                pEm->Timer0 = 0;
                pEm->Routine_2 = 1;
            case 1:
                Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
                break;
            case 2:
                *(ULONG *)&pEm->Move_no = 0x70002;
                pEm->Routine_2 = 3;
                pEm->Timer0 = 25;
                pEm->Spd_x = 10;
                pEm->Spd_z = -10;
                pG->pEnemy[0]->Be_flg |= 0x10;
                pG->pEnemy[2]->Be_flg |= 0x10;
            case 3:
                if (pEm->Timer0 != 0) {
                    pEm->Cdir_y += 0x10;
                    pEm->Timer0--;
                    pEm->Pos_y -= 5;
                    Add_speedXZ(pEm, 0);
                }
                if (pEm->Move_cnt == 0x1C) {
                    Snd_se_stad(5, NULL, 0, 0);
                }
                if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200) != 0) {
                    pEm->Routine_2 = 4;
                }
                break;
            case 4:
                break;
            }
            break;
        case 9:
            switch (pEm->Routine_2) {
            case 0:
                *(ULONG *)&pEm->Move_no = 0;
                pEm->Timer0 = 0;
                pEm->Routine_2 = 1;
            case 1:
                Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
                break;
            case 2:
                *(ULONG *)&pEm->Move_no = 0x70001;
                pEm->Routine_2 = 3;
                pEm->Timer0 = 60;
                pG->pEnemy[0]->Be_flg |= 0x10;
                pG->pEnemy[1]->Be_flg |= 0x10;
                pEm->Spd_x = 10;
                pEm->Spd_z = 0;
                pEm->UnkDA = 60;
            case 3:
                if (pEm->Timer0 != 0) {
                    pEm->Cdir_y += 8;
                    pEm->Timer0--;
                }
                if (pEm->UnkDA != 0) {
                    pEm->Pos_y -= 3;
                    Add_speedXZ(pEm, 0);
                    pEm->UnkDA--;
                }
                if (pEm->Move_cnt == 0x14) {
                    Snd_se_stad(6, NULL, 0, 0);
                }
                if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200) != 0) {
                    pEm->Routine_2 = 4;
                }
                break;
            case 4:
                break;
            }
            break;
        case 10:
            switch (pEm->Routine_2) {
            case 0:
                *(ULONG *)&pEm->Move_no = 0;
                pEm->Timer0 = 0;
                pEm->Routine_2 = 1;
            case 1:
                Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
                break;
            case 2:
                *(ULONG *)&pEm->Move_no = 0x70001;
                pEm->Routine_2 = 3;
                pEm->Timer0 = 15;
                pG->pEnemy[1]->Be_flg |= 0x10;
                pG->pEnemy[2]->Be_flg |= 0x10;
            case 3:
                if (pEm->Timer0 != 0) {
                    pEm->Cdir_y += 0x10;
                    pEm->Timer0--;
                }
                if (pEm->Move_cnt == 0x20) {
                    Snd_se_stad(7, NULL, 0, 0);
                }
                if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200) != 0) {
                    pEm->Routine_2 = 4;
                }
                break;
            case 4:
                break;
            }
            break;
        }
    }
}

/* RE2 Select_fade_in */
void func_801C31C4(void)
{
    Select_wk.Main_no = 2;
}

/* RE2 Select_fade_out */
void func_801C31D4(void)
{
    Select_wk.Main_no = 5;
}

/* RE2 Select_cursor_move */
void func_801C31E4(void)
{
    SELECT_WORK *pSel = &Select_wk;

    if (!(pSel->Flg & 1)) {
        if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
            Select_wk.Main_no = 3;
            Snd_se_stad(1, NULL, 0, 0);
            return;
        }
        if ((USHORT)G.Unk20F8 & 0x8000) {
            if (--pSel->Cursor_no < 0) {
                pSel->Cursor_no = 2;
            }
            func_801C35C8();
            Snd_se_stad(2, NULL, 0, 0);
        }
        if ((USHORT)G.Unk20F8 & 0x2000) {
            if (++pSel->Cursor_no >= 3) {
                pSel->Cursor_no = 0;
            }
            func_801C35C8();
            Snd_se_stad(2, NULL, 0, 0);
        }
    }
    func_801C3660();
}

/* RE2 Select_decide */
void func_801C331C(void)
{
    SELECT_WORK *pSel = &Select_wk;
    tagGLOBAL *pG = &G;

    (&pG->pEnemy[0])[pSel->Cursor_no]->Routine_2 = 2;
    Pad_vib_set0(3, 0);
    Pad_vib_set1(5, 150, 0);
    Fade_set(0, 1, 0, 1, 0, 0xFFFFFF, 2);
    while (Fade_status(0) != 0) {
        func_801C20FC();
        Task_sleep(1);
    }
    Pad_vib_set0(3, 0);
    Pad_vib_set1(8, 200, 0);
    Fade_set(0, 1, 0, 1, 0, 0xFFFFFF, 4);
    while (Fade_status(0) != 0) {
        func_801C20FC();
        Task_sleep(1);
    }
    Fade_set(0, 1, FADE_HOLD, 1, 0, 0, 40);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C20FC();
        Task_sleep(1);
    }
    Snd_work.Seq[0].Unk38 = 2;
    Snd_work.Seq[0].Unk3C = 0x3DAF;
    Snd_work.Seq[0].Unk40 = 0x250;
    Fade_set(0, 1, FADE_HOLD, 1, 0, 0xFFFFFF, 30);
    while (!(Fade_status(0) & FADE_END)) {
        func_801C20FC();
        Task_sleep(1);
    }
    Fade_set(1, 0, FADE_HOLD, 2, 0, 0xFFFFFF, 15);
    while (!(Fade_status(1) & FADE_END)) {
        Task_sleep(1);
    }
    pSel->Main_no = 5;
}

/* RE2 Select_exit */
void func_801C354C(void)
{
    SELECT_WORK *pSel = &Select_wk;
    tagGLOBAL *pG = &G;
    UCHAR Id;

    Id = Select_pl_init[pSel->Cursor_no].Id;
    pG->Unk2118 = 0;
    pG->Score = 0;
    pG->Unk2116 = 0;
    pG->Player.Id = Id;
    Main.Vloop = pSel->Vloop_bak;
    pG->Unk784E = pG->Player.Id;
    Task_signal(0);
    Task_exit();
}

/* RE2 Select_pl_dir_set */
void func_801C35C8(void)
{
    SELECT_WORK *pSel = &Select_wk;
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pEm;
    ULONG i;

    for (i = 0; i < 3; i++) {
        pEm = (&pG->pEnemy[0])[i];
        pEm->Unk158 = Select_dir_L[pSel->Cursor_no][i];
        pEm->Unk1BC[0] = Select_dir_fpos[pSel->Cursor_no][i];
        pEm->Spd_y = 0;
    }
    pSel->Moji_no = 0;
    pSel->Flg |= 1;
}

/* RE2 Select_pl_rot: turntable (Unk154 angle -> Unk158) and Cdir_y -> Unk1BC[0] */
void func_801C3660(void)
{
    SELECT_WORK *pSel = &Select_wk;
    tagGLOBAL *pG;
    ENEMY_WORK *pEm;
    ULONG L;
    ULONG Dir;
    ULONG i;

    for (i = 0; i < 3; i++) {
        pG = &G;
        pEm = (&pG->pEnemy[0])[i];
        pEm->Unk154 &= 0xFFF;
        pEm->Unk158 &= 0xFFF;
        if (pEm->Unk154 == pEm->Unk158) {
            pSel->Flg &= 0xFE;
        } else {
            L = pEm->Unk154;
            Dir = ((USHORT)pEm->Unk158 - (USHORT)pEm->Unk154) & 0xFFF;
            pEm->Unk154 = L - 0x40;
            if (Dir <= 0x800) {
                pEm->Unk154 = L + 0x40;
            }
            pEm->Pos_x = (rsin(pEm->Unk154) * 2000) >> 12;
            pEm->Pos_z = (rcos(pEm->Unk154) * 2000) >> 12;
        }
        if (pEm->Cdir_y == pEm->Unk1BC[0]) {
            if (pEm->Cdir_y == 0x1000) {
                pEm->Cdir_y = 0;
                pEm->Unk1BC[0] = 0;
            }
            pEm->Spd_y = 0;
        } else {
            if (pEm->Unk1BC[0] == 0 && pEm->Cdir_y > 3000) {
                pEm->Unk1BC[0] = 0x1000;
            }
            if (pEm->Cdir_y == 0 && pEm->Unk1BC[0] > 3000) {
                pEm->Cdir_y = 0x1000;
            }
            if (pEm->Spd_y == 0) {
                pEm->Spd_y = (pEm->Unk1BC[0] - pEm->Cdir_y) / 16;
            }
            pEm->Cdir_y += pEm->Spd_y;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/select", func_801C3830);

#include "ovl/common/jill_sel/Jill_pl_trans.inc.c"

#include "ovl/common/jill_sel/Jill_light_set.inc.c"

INCLUDE_ASM("asm/nonmatchings/ovl/select", Jill_wep_load);

/* RE2 Select_moji_set */
void func_801C3DB8(ULONG *pDst, SELECT_MOJI_REC *pSrc)
{
    pDst[0] = pSrc->Word0;
    pDst[1] = pSrc->Word1;
    pDst[2] = pSrc->Word2 + (GetClut(0, 0x1F0) << 16);
    pDst[3] = pSrc->Ot + (pSrc->R << 8) + (pSrc->G << 16) + (pSrc->Vis << 24);
}

/* RE2 Select_moji_dm_set */
void func_801C3E3C(void)
{
    SELECT_MOJI *pM;
    LONG i;

    pM = Select_pMoji;
    i = 8;
    do {
        i--;
        SetDrawMode(&pM->Dm[G.Cbuf_id][i], 0, 0, 0x96, NULL);
        AddPrim(&G.Ot0[G.Cbuf_id][i], &pM->Dm[G.Cbuf_id][i]);
    } while (i != 0);
}

/* RE2 Select_moji_init */
void func_801C3F00(void)
{
    SELECT_MOJI *pM;
    ULONG i;

    pM = Select_pMoji;
    func_801C44A4();
    for (i = 0; i < 141; i++) {
        func_801C3DB8((ULONG *)&pM->Rec[i], &Select_moji_tbl[i]);
    }
    pM->Blink = 0;
}

/* RE2 Select_moji_print: types pStr into the window, Wait frames per character;
 * ']' new line, '[ddd' pause, '|d' colour. Returns 1 and sets *pEnd at the end of the string. */
ULONG func_801C3F74(LONG Wait, UCHAR *pStr, ULONG *pEnd)
{
    SELECT_MOJI *pM;
    UCHAR Printed;
    UCHAR *pChr;

    pM = Select_pMoji;
    if (*pEnd != 0) {
        return 0;
    }
    if (pM->pStr != pStr) {
        pM->pStr = pStr;
        pM->pCur = pStr;
        pM->Wait = 1;
    }
    if (--pM->Wait != 0) {
        return 0;
    }
    pM->Wait = Wait;
    Printed = 0;
    while (*pM->pCur != 0) {
        switch (*pM->pCur) {
        case ']':
            pM->Col = 0;
            pM->Row++;
            break;
        case '[':
            pM->Wait = (*++pM->pCur - '0') * 100;
            pM->Wait += (*++pM->pCur - '0') * 10;
            pM->Wait += *++pM->pCur - '0';
            pM->pCur++;
            return 0;
        case '|':
            pM->Color = *++pM->pCur - '0';
            break;
        default:
            Printed = 1;
            pChr = &pM->Text[pM->Row][pM->Col];
            *pChr = *pM->pCur;
            pChr = &pM->Attr[pM->Row][pM->Col];
            *pChr = pM->Color;
            pM->Blink = 0x10;
            pM->Col++;
            break;
        }
        if (pM->Col >= 20) {
            pM->Col = 0;
            pM->Row++;
        }
        Snd_se_stad(3, NULL, 0, 0);
        pM->pCur++;
        if (Wait != 0 && Printed) {
            return 0;
        }
    }
    pM->pStr = NULL;
    *pEnd = 1;
    return 1;
}

/* RE2 Select_moji_ctrl: the message script of the selected player */
void func_801C41E0(void)
{
    SELECT_WORK *pSel = &Select_wk;

    switch (pSel->Moji_no) {
    case 0:
        func_801C3F00();
        Select_moji_end = 0;
        pSel->Moji_no = 1;
        break;
    case 1:
        func_801C3F74(1, Select_str[pSel->Cursor_no][0], &Select_moji_end);
        if (Select_moji_end == 0) {
            goto update;
        }
        pSel->Moji_no = 2;
        break;
    case 2:
        func_801C3F74(1, Select_str[pSel->Cursor_no][1], &Select_moji_end);
        if (Select_moji_end == 0) {
            goto update;
        }
        pSel->Moji_no = 3;
        break;
    case 3:
        func_801C3F74(1, Select_str[pSel->Cursor_no][2], &Select_moji_end);
        if (Select_moji_end == 0) {
            goto update;
        }
        pSel->Moji_no = 4;
        break;
    case 4:
        func_801C3F74(1, Select_str[pSel->Cursor_no][3], &Select_moji_end);
        if (Select_moji_end == 0) {
            goto update;
        }
        pSel->Moji_no = 5;
        break;
    case 5:
        func_801C3F74(1, Select_str[pSel->Cursor_no][4], &Select_moji_end);
        if (Select_moji_end == 0) {
            goto update;
        }
        pSel->Moji_no = 6;
        break;
    case 6:
        func_801C3F74(1, Select_str[pSel->Cursor_no][5], &Select_moji_end);
        if (Select_moji_end == 0) {
            goto update;
        }
        if (pSel->Cursor_no == 3 || pSel->Cursor_no == 0) {
            pSel->Moji_no = 7;
        } else {
            pSel->Moji_no = 7;
        }
        break;
    case 7:
        func_801C3F74(1, Select_str[pSel->Cursor_no][6], &Select_moji_end);
        if (Select_moji_end == 0) {
            goto update;
        }
        pSel->Moji_no = 8;
        break;
    case 8:
        break;
    }
    if (Select_moji_end != 0) {
        Select_moji_end = 0;
    }
update:
    func_801C44E8(1);
    func_801C4644(141);
}

/* RE2 Select_moji_clear */
void func_801C44A4(void)
{
    SELECT_MOJI *pM;
    UCHAR *pText;
    UCHAR *pAttr;
    ULONG k;

    pM = Select_pMoji;
    pText = pM->Text[0];
    pAttr = pM->Attr[0];
    for (k = 0; k < 140; k++) {
        *pText++ = 0x20;
        *pAttr++ = 0;
    }
    pM->Color = 0;
    pM->Row = 0;
    pM->Col = 0;
}

/* RE2 Select_moji_update: glyph of each character, then the cursor */
void func_801C44E8(LONG Disp)
{
    SELECT_MOJI *pM;
    UCHAR *pText;
    SELECT_MOJI_REC *pR;
    SELECT_MOJI_REC *pEnd;
    ULONG uVal;
    ULONG c;

    pM = Select_pMoji;
    pText = pM->Text[0];
    pR = &pM->Rec[1];
    pEnd = &pM->Rec[141];
    do {
        if (*pText != 0x20) {
            uVal = *pText - 0x20;
            pR->Word1 = 8 | (10 << 16);
            c = (USHORT)uVal;
            uVal = (((c >> 4) * 10 + 0xB0) << 8) | ((c & 0xF) << 3);
            pR->Word2 = (uVal & 0xFFFF) + (GetClut(0, 0x1F0) << 16);
            pR->Vis = Disp;
        }
        pR++;
        pText++;
    } while (pR < pEnd);
    pM->Rec[0].Word1 = 8 | (10 << 16);
    *(SHORT *)&pM->Rec[0].Word0 = pM->Col * 8 + 0xC;
    *((SHORT *)&pM->Rec[0].Word0 + 1) = pM->Row * 10 + 0x9B;
    pM->Blink = (pM->Blink + 1) & 0x1F;
    pM->Rec[0].Word2 = (GetClut(0, 0x1F0) << 16) | 0xCE78;
    if (Disp != 0) {
        pM->Rec[0].Vis = pM->Blink & 0x10;
    } else {
        pM->Rec[0].Vis = 0;
    }
}

/* RE2 Select_moji_trans: one SPRT packet per visible glyph */
void func_801C4644(LONG Num)
{
    SELECT_MOJI *pM;
    ULONG *p;
    SELECT_MOJI_REC *pR;
    SELECT_MOJI_REC *pEnd;

    pM = Select_pMoji;
    p = pM->Packet[G.Cbuf_id];
    pR = pM->Rec;
    pEnd = pR + Num;
    do {
        if (pR->Vis != 0) {
            p[0] = 0x04000000;
            p[1] = 0x64000000 + (pR->R << 24) + pR->G + (pR->G << 8) + (pR->G << 16);
            p[2] = pR->Word0;
            p[4] = pR->Word1;
            p[3] = pR->Word2;
            AddPrim(&G.Ot0[G.Cbuf_id][3], p);
            p += 5;
        }
        pR++;
    } while (pR < pEnd);
    func_801C3E3C();
}

/* RE2 Select_bg_load */
void func_801C4774(void)
{
    Cd_read(0x52, (ULONG *)0x8019C000, 0, "Select BG");
    Cd_read(0x54, (ULONG *)0x80100000, 0, (char *)Select_str_sel);
    G.Ctp.Tpage_clut = 0xF15;
    Mapping_tim((ULONG *)0x80100000);
}

/* "Select"; the 0x11 is the original linker's fill before .text, kept for the byte match */
const char Select_str_sel[8] = { 'S', 'e', 'l', 'e', 'c', 't', 0, 0x11 };

#include "ovl/common/jill_sel/Jill_obj_init.inc.c"

#include "ovl/common/jill_sel/Jill_obj_draw.inc.c"
#include "ovl/common/jill_sel/Jill_obj_rgb.inc.c"

void func_801C4BA0(SEL_OBJ *pObj, SHORT X, SHORT Y)
{
    pObj->X = X;
    pObj->Y = Y;
}

void func_801C4BAC(SEL_OBJ *pObj, SEL_SPRT_DEF *pDef, ULONG TpClut)
{
    SEL_SPRT *pSp;

    pSp = &pObj->Sp[0];
    pSp->W = pDef->W;
    pSp->H = pDef->H;
    pSp->U = pDef->U;
    pSp->V = pDef->V;
    pSp = &pObj->Sp[1];
    pSp->W = pDef->W;
    pSp->H = pDef->H;
    pSp->U = pDef->U;
    pSp->V = pDef->V;
}
