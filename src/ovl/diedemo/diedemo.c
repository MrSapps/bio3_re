#include "common.h"
#include "ovl/diedemo.h"

/* task entry */
void func_80194010(void)
{
    Diedemo.Rno = 0;
    Diedemo.Ctr = 0;
    while (1) {
        if (D_80198204) {
            if (G.Unk20FC & 0x100) {
                Fade_set(0, 0, 2, 2, 0, 0xFFFFFFFF, 0x20);
                D_80198204 = 0;
                if ((G.System_flg & 0x200) && (G.Status_flg & STAT_XA_VOICE)) {
                    Xa_error();
                }
                G.Unk2468 |= 0x1000000;
            }
        } else if (Fade_status(0) & FADE_END) {
            Diedemo.Rno = 7;
        }
        func_80194100();
        Task_sleep(1);
    }
}

void func_80194100(void)
{
    D_80198208[Diedemo.Rno]();
}

/* routine 0: set up the demo */
void func_80194140(void)
{
    UCHAR Id;

    Diedemo.Free = 0x400;
    Diedemo.Addpos = 0;
    Diedemo.Ctr = 0;
    Diedemo.Phase = 0;
    Diedemo.Daddpos = 0;
    Diedemo.S_trans_flg = 0;
    Diedemo.Id_ctr = 10;
    if (G.System_flg & 0x800) {
        Id = G.Pl_pEnemy_ptr->Id;
        if (Id < 0x20) {
            Diedemo.Id_ctr = 1;
        } else {
            while (D_801981F8[Diedemo.Id_ctr] != Id && Diedemo.Id_ctr != 0) {
                Diedemo.Id_ctr--;
            }
        }
        Diedemo.Wait_ctr = D_80197F8C[Diedemo.Id_ctr];
    } else {
        Diedemo.Id_ctr = 0;
        Diedemo.Wait_ctr = D_80197F8C[0];
    }
    G.Unk2468 |= 0x2000000;
    G.Unk2468 |= 0x1000000;
    G.Status_flg |= 0x180;
    G.Status_flg &= ~0x1000;
    G.Status_flg &= ~4;
    G.Status_flg &= ~2;
    Pl_wep_eff_parts_init((PLAYER_WORK *)&G.Pl_be_flg);
    func_80046C90();
    if (G.Status_flg & STAT_XA_VOICE) {
        Xa_error();
    }
    D_800DBB58 = 0;
    G.Pl_damage_cnt = 0x80;
    G.Next_cut_no = G.Cut_no;
    G.pUnk2660->Damage_cnt = 0x80;
    if (G.System_flg & 0x200) {
        if (G.System_flg & 0x80) {
            Xa_play(3, 0x18);
            G.Status_flg |= STAT_XA_VOICE;
            func_8003884C(D_80197FA4, 0);
        } else {
            switch (G.Unk783E + 1) {
            case 4:
                Snd_se_stad(0x208, NULL, 0, 0);
                Xa_play(3, 0x19);
                G.Status_flg |= STAT_XA_VOICE;
                func_8003884C(D_80197FA4, 1);
                break;
            case 5:
                if (G.Unk7840 == 9) {
                    Xa_play(3, 0x1F);
                    G.Status_flg |= STAT_XA_VOICE;
                } else {
                    Xa_play(3, 0x1A);
                    G.Status_flg |= STAT_XA_VOICE;
                    func_8003884C(D_80197FA4, 2);
                }
                break;
            }
        }
        Diedemo.Id_ctr = 10;
        Diedemo.Wait_ctr = D_80197FA0[0];
    } else {
        G.Status_flg &= ~0x4000000;
        G.Status_flg &= ~0x2000000;
    }
    if (Flg_ck(G.Unk7868, 0xC2)) {
        Diedemo.Unk3B = 1;
        Diedemo.Id_ctr = 9;
        D_8019807C[9] = D_801981B0;
        Diedemo.Wait_ctr = 0x14;
        Diedemo.Em_flg = 1;
        G.Pl_pEnemy_ptr = G.pEnemy[0];
        G.System_flg |= 0x800;
    }
    Main.Vloop = 3;
    Diedemo.Rno++;
}

/* routine 1 */
void func_801944B4(void)
{
    Diedemo.Wait_ctr--;
    if (Diedemo.Wait_ctr < 0) {
        Diedemo.Rno++;
    }
}

/* routine 2 */
void func_801944F0(void)
{
    if (Flg_ck(G.Unk7868, 0xC2)) {
        Snd_se_stad(0x20A, NULL, 0, 0);
    }
    Fade_set(1, 1, 2, 1, 0, 0xFFFFFF, 0x1E);
    Diedemo.Rno++;
}

INCLUDE_ASM("asm/nonmatchings/ovl/diedemo/diedemo", func_80194574);

/* camera set up from the cut info of the killer */
void func_80194A3C(void)
{
    CUTINFO *pCut;
    MATRIX *pM;
    RCUT *pRcut;

    Diedemo.pCutinfo = pCut = &D_8019807C[Diedemo.Id_ctr];
    Diedemo.Radius = pCut->Radius;
    Diedemo.P_radius = pCut->P_radius;
    if (!(Diedemo.Em_flg & 1)) {
        Diedemo.Theta = G.Pl_cdir_y;
    } else {
        Diedemo.Theta = G.pUnk2660->Cdir_y;
    }
    Diedemo.Theta &= 0xFFF;
    if (!(Diedemo.Em_flg & 1)) {
        if (Cdir_ck_l(&Pl_work, Pl_work.pEnemy_ptr)) {
            Diedemo.Theta += pCut->P_theta;
            Diedemo.Prad = pCut->Prad;
        } else {
            Diedemo.Theta += pCut->M_theta;
            Diedemo.Prad = pCut->Mrad;
        }
    } else if (Cdir_ck_l(G.pUnk2660, G.Pl_pEnemy_ptr)) {
        Diedemo.Theta += pCut->P_theta;
        Diedemo.Prad = pCut->Prad;
    } else {
        Diedemo.Theta += pCut->M_theta;
        Diedemo.Prad = pCut->Mrad;
    }
    switch (pCut->R_obj) {
    case 0:
        if (!(Diedemo.Em_flg & 1)) {
            pM = &G.Pl_pSin_parts_ptr[pCut->R_ptr].Workm;
        } else {
            pM = &G.pUnk2660->pSin_parts_ptr[pCut->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Pl_pEnemy_ptr->pSin_parts_ptr[pCut->R_ptr].Workm;
        break;
    }
    pRcut = &Diedemo.Rcut;
    pRcut->View_p[1] = pM->t[1] + pCut->P_viewp;
    Diedemo.Addpos = pCut->Addpos;
    Diedemo.Daddpos = 0;
    pRcut->View_r[0] = pM->t[0];
    pRcut->View_r[2] = pM->t[2];
    pRcut->View_r[1] = pM->t[1] - 300;
    func_80194DB4();
    func_801960CC();
    func_80195C04();
    func_80195E68();
}

/* default camera (no killer) */
void func_80194C98(void)
{
    MATRIX *pM;
    RCUT *pRcut;

    Diedemo.Radius = 3000;
    Diedemo.Ctr = 0;
    Diedemo.P_radius = 0;
    Diedemo.Addpos = 8;
    Diedemo.Daddpos = 1;
    Diedemo.Prad = 0x10;
    if (!(Diedemo.Em_flg & 1)) {
        Diedemo.Theta = G.Pl_cdir_y;
    } else {
        Diedemo.Theta = G.pUnk2660->Cdir_y;
    }
    Diedemo.pCutinfo = D_8019807C;
    if (!(Diedemo.Em_flg & 1)) {
        pM = &G.Pl_pSin_parts_ptr->Workm;
    } else {
        pM = &G.pUnk2660->pSin_parts_ptr->Workm;
    }
    pRcut = &Diedemo.Rcut;
    pRcut->View_p[1] = pM->t[1] - 1000;
    pRcut->View_r[0] = pM->t[0];
    pRcut->View_r[2] = pM->t[2];
    pRcut->View_r[1] = pM->t[1] - 300;
    func_80194DB4();
    func_801960CC();
    func_80195C04();
    func_80195E68();
}

/* hide the models that are not part of the demo */
void func_80194DB4(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;
    ENEMY_WORK *pKill;
    VCUT *pVcut;
    ULONG i;

    pVcut = &Dievcut;
    for (i = 0; i < 4; i++) {
        pVcut->Xz[i][0] = 0;
        pVcut->Xz[i][1] = 0;
    }
    G.Cccut = pVcut;
    if (Diedemo.Em_flg & 1) {
        pG->pUnk265C->Be_flg |= 0x10;
        pG->pUnk265C->Type |= 0x8000;
    } else {
        pG->pUnk2660->Be_flg |= 0x10;
        pG->pUnk2660->Type |= 0x8000;
    }
    pG->pUnk265C->Water = 0;
    pG->pUnk2660->Water = 0;
    for (ppEm = pG->pEnemy; ppEm != pG->ppEnemy_end; ppEm++) {
        pG->pEm = pEm = *ppEm;
        if (pEm->Be_flg & 1) {
            pEm->Be_flg |= 0x10;
            pEm->Water = 0;
            pEm->Type |= 0x8000;
        }
    }
    if (G.System_flg & 0x800) {
        if (!(Diedemo.Unk3B & 1)) {
            func_80194FE4();
        } else {
            func_80195124();
        }
    }
    if (G.System_flg & 0x200) {
        pG->pUnk265C->Be_flg |= 0x10;
        pG->pUnk2660->Be_flg |= 0x10;
    }
    pKill = pG->Pl_pEnemy_ptr;
    switch (pKill->Id) {
    case 0x34:
    case 0x35:
        Diedemo.Fade_parts = 0x11;
        Diedemo.pEm = pKill;
        Diedemo.Fade_kido = 0x80;
        break;
    case 0x36:
        Diedemo.Fade_parts = 0x17;
        Diedemo.pEm = pKill;
        Diedemo.Fade_kido = 0x80;
        break;
    case 0x38:
        Diedemo.Fade_parts = 0x1E;
        Diedemo.pEm = pKill;
        Diedemo.Fade_kido = 0x80;
        break;
    default:
        Diedemo.Fade_parts = 0;
        break;
    }
}

void func_80194FE4(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pKill = pG->Pl_pEnemy_ptr;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;

    switch ((ULONG)pKill->Id) {
    case 0x36:
        pG->Pl_be_flg |= 0x2000;
        pKill->Pos_x += pG->Pl_pos_x;
        pKill->Pos_z += pG->Pl_pos_z;
        pG->Pl_pos_x = pG->Pl_pos_z = 0;
    case 0x34:
    case 0x35:
        pKill->Be_flg ^= 0x10;
        pKill->Type ^= 0x8000;
        pKill->Be_flg |= 0x2000;
        for (ppEm = pG->pEnemy; ppEm != pG->ppEnemy_end; ppEm++) {
            pG->pEm = pEm = *ppEm;
            if (pEm->Id == 0x37) {
                pEm->Be_flg ^= 0x10;
                pEm->Type ^= 0x8000;
            }
        }
        break;
    case 0x24:
        pG->pUnk265C->Be_flg |= 0x10;
        pG->pUnk2660->Be_flg |= 0x10;
        break;
    }
}

void func_80195124(void)
{
    tagGLOBAL *pG = &G;

    if (Flg_ck(pG->Unk7868, 0xC2)) {
        pG->pUnk2660->Be_flg &= 0xFFEF;
        pG->pUnk2660->Be_flg |= 0x2000;
        pG->pUnk2660->Type &= 0x7FFF;
        pG->pEnemy[0]->Be_flg &= 0xFFEF;
        pG->pEnemy[0]->Type &= 0x7FFF;
        pG->pEnemy[1]->Be_flg &= 0xFFEF;
        pG->pEnemy[1]->Type &= 0x7FFF;
        pG->pUnk2660->Routine_0 = 6;
        pG->pUnk2660->Routine_1 = 0;
        pG->pUnk2660->Routine_2 = 0;
        pG->pUnk2660->Routine_3 = 0;
        pG->pUnk2660->Pos_x = 0;
        pG->pUnk2660->Pos_z = 0;
        pG->pUnk2660->Unk40[0] = 0;
        pG->pUnk2660->Unk40[2] = 0;
        pG->pEnemy[0]->Pos_x = 0;
        pG->pEnemy[0]->Pos_z = 0;
        pG->pEnemy[0]->Unk40[0] = 0;
        pG->pEnemy[0]->Unk40[2] = 0;
        pG->pEnemy[0]->Routine_0 = 4;
        pG->pEnemy[0]->Routine_1 = 0xB;
        pG->pEnemy[0]->Routine_2 = 0;
        pG->pEnemy[0]->Routine_3 = 0;
        pG->pEnemy[0]->Type ^= 0x100;
        pG->Unk2468 = (pG->Unk2468 & ~0x40000000) | 0x80000000;
        Flg_off(pG->Unk7868, 0xC2);
        Diedemo.Unk44 = 0;
    }
    D_80198228[Diedemo.Unk44]();
}

void func_80195330(void)
{
    Snd_se_stad(0x20B, NULL, 0, 0);
    Task_sleep(0x32);
    Snd_se_stad(0x20C, NULL, 0, 0);
}

/* routine 4: camera move */
void func_80195378(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;

    if (CD.Cd_rtn_no == 0 && Diedemo.S_trans_flg == 0) {
        Diedemo.S_trans_flg = 1;
    }
    if (Diedemo.Unk3B & 1) {
        func_80195640();
    } else {
        func_801954D4();
    }
    func_80195C04();
    func_80195E68();
    if (++Diedemo.Ctr > Diedemo.pCutinfo->Wait_ctr) {
        Diedemo.P_radius = 0;
        Diedemo.Addpos = 1;
        Diedemo.Daddpos = 1;
        Diedemo.Prad = 0x10;
        Diedemo.Ctr = 0;
        for (ppEm = pG->pEnemy; ppEm != pG->ppEnemy_end; ppEm++) {
            pG->pEm = pEm = *ppEm;
            if (pEm->Be_flg & 1) {
                if (pEm->Id == 0x37 || pEm->Id == 0x39) {
                    pEm->Be_flg |= 0x10;
                    pEm->Type |= 0x8000;
                }
            }
        }
        Diedemo.Rno++;
    }
}

void func_801954D4(void)
{
    MATRIX *pM;
    RCUT *pRcut;

    if (G.Pl_pEnemy_ptr->Id == 0x36) {
        if (Diedemo.Ctr == 0x73) {
            Diedemo.Radius = 3000;
            Diedemo.P_radius = 0;
            Diedemo.Addpos = 8;
            Diedemo.Daddpos = 1;
            Diedemo.Prad = 0x10;
            if (!(Diedemo.Em_flg & 1)) {
                Diedemo.Theta = G.Pl_cdir_y;
            } else {
                Diedemo.Theta = G.pUnk2660->Cdir_y;
            }
            Diedemo.pCutinfo = D_8019807C;
            D_8019807C[0].Wait_ctr = D_8019807C[Diedemo.Id_ctr].Wait_ctr;
            if (!(Diedemo.Em_flg & 1)) {
                pM = &G.Pl_pSin_parts_ptr->Workm;
            } else {
                pM = &G.pUnk2660->pSin_parts_ptr->Workm;
            }
            pRcut = &Diedemo.Rcut;
            pRcut->View_p[1] = pM->t[1] - 1000;
            pRcut->View_r[0] = pM->t[0];
            pRcut->View_r[2] = pM->t[2];
            pRcut->View_r[1] = pM->t[1] - 300;
            func_801960CC();
            Main.Vloop = 3;
        }
        if (Diedemo.Ctr == 0x78) {
            Main.Vloop = 2;
        }
    }
}

void func_80195640(void)
{
    MATRIX *pM;
    RCUT *pRcut;

    if (G.Pl_pEnemy_ptr->Id == 0x36) {
        if (Diedemo.Ctr == 0xA0) {
            Diedemo.Radius = 3000;
            Diedemo.P_radius = 0;
            Diedemo.Addpos = 8;
            Diedemo.Daddpos = 1;
            Diedemo.Prad = 0x10;
            if (!(Diedemo.Em_flg & 1)) {
                Diedemo.Theta = G.Pl_cdir_y;
            } else {
                Diedemo.Theta = G.pUnk2660->Cdir_y;
            }
            Diedemo.pCutinfo = D_8019807C;
            D_8019807C[0].Wait_ctr = D_8019807C[Diedemo.Id_ctr].Wait_ctr + 0x14;
            if (!(Diedemo.Em_flg & 1)) {
                pM = &G.Pl_pSin_parts_ptr->Workm;
            } else {
                pM = &G.pUnk2660->pSin_parts_ptr->Workm;
            }
            pRcut = &Diedemo.Rcut;
            pRcut->View_p[1] = pM->t[1] - 1000;
            pRcut->View_r[0] = pM->t[0];
            pRcut->View_r[2] = pM->t[2];
            pRcut->View_r[1] = pM->t[1] - 300;
            func_801960CC();
            Main.Vloop = 3;
        }
        if (Diedemo.Ctr == 0xA5) {
            Main.Vloop = 2;
        }
    }
}

/* routine 5 */
void func_801957AC(void)
{
    if (Diedemo.Ctr == 0) {
        while (CD.Cd_rtn_no != 0) {
            Task_sleep(1);
        }
        if ((G.System_flg & 0x800) && Diedemo.S_trans_flg == 0) {
            Diedemo.S_trans_flg = 1;
        }
    }
    if (Main.Fade[1].Cnt >= 0x10 || Fade_status(1)) {
        func_80196380();
    }
    if (Diedemo.Ctr < 0x20) {
        func_80195B38();
    }
    if (Diedemo.Ctr < 0x50) {
        func_80195C04();
    } else {
        func_80195E68();
    }
    if (++Diedemo.Ctr == 0xAA) {
        Diedemo.Addpos = 10;
        Diedemo.Daddpos = 1;
        Diedemo.Prad = 0x10;
        Diedemo.Ctr = 0;
        Diedemo.Rno++;
    }
}

/* routine 6: darken the light and fade out */
void func_801958FC(void)
{
    LIGHT_CUT_DATA *pLc;
    ULONG L;

    pLc = Get_light_cut(&G.Pl_pos_x);
    func_80196380();
    func_80195C04();
    func_80195E68();
    if (Diedemo.Ctr == 0) {
        Snd_se_stad(0xE, NULL, 0, 0);
    }
    if (++Diedemo.Ctr == 0x5A) {
        Fade_set(1, 1, 2, 0, 0xA0A0A, 0, 0x1E);
    }
    if (Diedemo.Ctr >= 0xA3) {
        Diedemo.Rno++;
    }
    if (Diedemo.Ctr >= 0x33) {
        L = pLc->L[0];
            pLc->Col[0][0] -= pLc->Col[0][0] != 0;
            pLc->Col[0][1] -= pLc->Col[0][1] != 0;
            pLc->Col[0][2] -= pLc->Col[0][2] != 0;
            pLc->Col[1][0] -= pLc->Col[1][0] != 0;
            pLc->Col[1][1] -= pLc->Col[1][1] != 0;
            pLc->Col[1][2] -= pLc->Col[1][2] != 0;
            pLc->Col[2][0] -= pLc->Col[2][0] != 0;
            pLc->Col[2][1] -= pLc->Col[2][1] != 0;
            pLc->Col[2][2] -= pLc->Col[2][2] != 0;
            pLc->Ambient[0] -= pLc->Ambient[0] != 0;
            pLc->Ambient[1] -= pLc->Ambient[1] != 0;
            pLc->Ambient[2] -= pLc->Ambient[2] != 0;
        if (L > 100) {
            L -= 100;
        }
        pLc->L[0] = L;
    }
    Set_light(pLc, (VECTOR *)&G.pUnk265C->Pos_x);
}

/* routine 7: end of the demo */
void func_80195AC8(void)
{
    Bg_set_mode(1, 0);
    Diedemo.Rno = 0;
    G.System_flg &= ~0x800;
    G.System_flg &= ~0x80000000;
    G.System_flg &= ~0x8000000;
    G.System_flg &= ~0x200;
    Snd_reset();
    Task_exit();
}

/* fade out the killer */
void func_80195B38(void)
{
    PARTS_W *pP;
    ULONG i;

    if (Diedemo.Fade_parts != 0) {
        for (i = 0; i <= Diedemo.Fade_parts; i++) {
            pP = &Diedemo.pEm->pSin_parts_ptr[i];
            pP->Poly_rgb = (UCHAR)Diedemo.Fade_kido + ((UCHAR)Diedemo.Fade_kido << 8) + ((UCHAR)Diedemo.Fade_kido << 16);
        }
        Diedemo.Fade_kido -= 4;
        if (Diedemo.Fade_kido <= 0x40) {
            Diedemo.pEm->Type |= 0x8000;
        }
        if (Diedemo.Fade_kido == 0) {
            Diedemo.pEm->Be_flg |= 0x10;
        }
    }
}

/* camera: spiral around the look-at part */
void func_80195C04(void)
{
    DIEDEMO *pDie = &Diedemo;
    MATRIX *pM;
    RCUT *pRcut;

    pDie->Radius -= pDie->P_radius;
    pRcut = &pDie->Rcut;
    switch (pDie->pCutinfo->R_obj) {
    case 0:
        if (!(pDie->Em_flg & 1)) {
            pM = &G.Pl_pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        } else {
            pM = &G.pUnk2660->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Pl_pEnemy_ptr->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        break;
    }
    pRcut->View_p[1] -= pDie->Addpos;
    pDie->Addpos += pDie->Daddpos;
    pRcut->View_p[0] = pM->t[0] + (pDie->Radius * rcos(pDie->Theta) >> 12);
    pRcut->View_p[2] = pM->t[2] - (pDie->Radius * rsin(pDie->Theta) >> 12);
    pDie->Theta = (pDie->Theta - pDie->Prad) & 0xFFF;
    pRcut->View_r[0] += (pM->t[0] - pRcut->View_r[0]) / 30;
    pRcut->View_r[2] += (pM->t[2] - pRcut->View_r[2]) / 30;
    pRcut->View_r[1] += (pM->t[1] - 300 - pRcut->View_r[1]) / 5;
    Set_view(pRcut->View_p, pRcut->View_r);
    SetGeomScreen(0xD0);
    gte_SetRotMatrix(&GsWSMATRIX);
    gte_SetTransMatrix(&GsWSMATRIX);
}

/* same as func_80195C04 */
void func_80195E68(void)
{
    DIEDEMO *pDie = &Diedemo;
    MATRIX *pM;
    RCUT *pRcut;

    pDie->Radius -= pDie->P_radius;
    pRcut = &pDie->Rcut;
    switch (pDie->pCutinfo->R_obj) {
    case 0:
        if (!(pDie->Em_flg & 1)) {
            pM = &G.Pl_pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        } else {
            pM = &G.pUnk2660->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Pl_pEnemy_ptr->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        break;
    }
    pRcut->View_p[1] -= pDie->Addpos;
    pDie->Addpos += pDie->Daddpos;
    pRcut->View_p[0] = pM->t[0] + (pDie->Radius * rcos(pDie->Theta) >> 12);
    pRcut->View_p[2] = pM->t[2] - (pDie->Radius * rsin(pDie->Theta) >> 12);
    pDie->Theta = (pDie->Theta - pDie->Prad) & 0xFFF;
    pRcut->View_r[0] += (pM->t[0] - pRcut->View_r[0]) / 30;
    pRcut->View_r[2] += (pM->t[2] - pRcut->View_r[2]) / 30;
    pRcut->View_r[1] += (pM->t[1] - 300 - pRcut->View_r[1]) / 5;
    Set_view(pRcut->View_p, pRcut->View_r);
    SetGeomScreen(0xD0);
    gte_SetRotMatrix(&GsWSMATRIX);
    gte_SetTransMatrix(&GsWSMATRIX);
}

/* light the look-at object */
void func_801960CC(void)
{
    LIGHT_CUT_DATA *pLc;
    MATRIX *pM;
    RCUT *pRcut;

    switch (Diedemo.pCutinfo->R_obj) {
    case 0:
        if (!(Diedemo.Em_flg & 1)) {
            pLc = Get_light_cut(&G.Pl_pos_x);
        } else {
            pLc = Get_light_cut(&G.pUnk2660->Pos_x);
        }
        break;
    case 1:
        pLc = Get_light_cut((LONG *)&G.Pl_pEnemy_ptr->pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm);
        break;
    }
    pRcut = &Diedemo.Rcut;
    switch (Diedemo.pCutinfo->R_obj) {
    case 0:
        if (!(Diedemo.Em_flg & 1)) {
            pM = &G.Pl_pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm;
        } else {
            pM = &G.pUnk2660->pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Pl_pEnemy_ptr->pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm;
        break;
    }
    pLc->Mode[2] = 0;
    pLc->Pos[2][0] = pRcut->View_r[0] + 500;
    pLc->Pos[2][1] = pRcut->View_r[1] + 3000;
    pLc->Pos[2][2] = pRcut->View_r[2] + 500;
    pLc->Col[2][0] = 0x84;
    pLc->Col[2][1] = 0x84;
    pLc->Col[2][2] = 0x84;
    pLc->L[2] = 0x1F40;
    pLc->Mode[0] = 0;
    pLc->Pos[0][0] = pM->t[0];
    pLc->Pos[0][1] = pM->t[1] - 300;
    pLc->Pos[0][2] = pM->t[2];
    pLc->Col[0][0] = 0x78;
    pLc->Col[0][1] = 0x78;
    pLc->Col[0][2] = 0x78;
    pLc->L[0] = 0xFA0;
    pLc->Mode[0] = 0;
    pLc->Col[1][0] = 0;
    pLc->Col[1][1] = 0;
    pLc->Col[1][2] = 0;
    switch (Diedemo.pCutinfo->R_obj) {
    case 0:
        if (!(Diedemo.Em_flg & 1)) {
            Set_light(pLc, (VECTOR *)&G.Pl_pos_x);
        } else {
            Set_light(pLc, (VECTOR *)&G.pUnk2660->Pos_x);
        }
        break;
    case 1:
        Set_light(pLc, (VECTOR *)&G.Pl_pEnemy_ptr->Pos_x);
        break;
    }
}

/* RE2 Die_spr_init */
void func_80196334(void)
{
    G.Unk2484 = 0x1F15;
    Mapping_tim((ULONG *)0x8019C000);
    pDiespr = (DIESPR *)0x801B1000;
    pDiespr->Rno = 0;
    pDiespr->Ctr = 0;
    func_8019642C();
}

/* RE2 Die_spr_move */
void func_80196380(void)
{
    Set_view(D_80198230[0], D_80198230[1]);
    SetGeomScreen(0xD0);
    gte_SetRotMatrix(&GsWSMATRIX);
    gte_SetTransMatrix(&GsWSMATRIX);
    D_8019824C[pDiespr->Rno]();
}

INCLUDE_ASM("asm/nonmatchings/ovl/diedemo/diedemo", func_8019642C);

/* RE2 Die_blood_move_init (not called in RE3) */
void func_80196720(void)
{
    SHORT Grp;
    SHORT Half;
    SHORT Row;
    SHORT Col;
    SHORT Wx;
    UCHAR Wy;
    UCHAR Tx;
    UCHAR Ty;
    POLY_FT4 *p;
    SHORT Col_last = 7;

    for (Grp = 0; Grp < 3; Grp++) {
        for (Half = 0; Half < 2; Half++) {
            Wy = 0x40;
            p = pDiespr->Blood[Grp][Half][0];
            for (Row = 0; Row < 16; Row++) {
                for (Col = Wx = 0; Col < 8; Col++) {
                    SetPolyFT4(p);
                    p->tpage = 0x95;
                    p->clut = 0x7FC0;
                    *(ULONG *)&p->r0 = 0x2C000000;
                    Tx = 0x20;
                    if (Col == Col_last) {
                        Tx = 0x1F;
                    }
                    Ty = 8;
                    if (Row == 15) {
                        Ty = 7;
                    }
                    p->u0 = Wx;
                    p->v0 = Wy;
                    p->u1 = Wx + Tx;
                    p->v1 = Wy;
                    p->u2 = Wx;
                    p->v2 = Wy + Ty;
                    p->u3 = Wx + Tx;
                    p->v3 = Wy + Ty;
                    p++;
                    Wx += 0x20;
                }
                Wy += 8;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/diedemo/diedemo", func_801968D4);

/* RE2 Die_spr_move_hold: shake the lower rows, draw the grid */
void func_80196F00(void)
{
    SVECTOR V[4];
    SHORT Row;
    SHORT Col;
    SHORT Row2;
    POLY_GT4 *p;

    p = pDiespr->Pgt4[G.Cbuf_id];
    for (Row = 0; Row < 17; Row++) {
        for (Col = 0; Col < 17; Col++) {
            if (Row == 15 && (Col & 1)) {
                pDiespr->Pos[Col][Row].vz += func_800102E8() & 1;
            }
            if (Row == 12 && !(Col & 1) && (pDiespr->Free2 & 1)) {
                for (Row2 = Row; Row2 < 17; Row2++) {
                    pDiespr->Pos[Col][Row2].vz += func_800102E8() & 1;
                }
            }
        }
    }
    for (Row = 0; Row < 16; Row++) {
        for (Col = 0; Col < 16; Col++) {
            /* [17] is the next column */
            V[0] = (&pDiespr->Pos[Col][Row])[0];
            V[1] = (&pDiespr->Pos[Col][Row])[17];
            V[2] = (&pDiespr->Pos[Col][Row + 1])[0];
            V[3] = (&pDiespr->Pos[Col][Row + 1])[17];
            RotTransPers3(&V[0], &V[1], &V[2], (long *)&p->x0, (long *)&p->x1, (long *)&p->x2, NULL, NULL);
            RotTransPers(&V[3], (long *)&p->x3, NULL, NULL);
            AddPrim(G.pOt0 + 5, p);
            p++;
        }
    }
    if (++pDiespr->Free2 >= 0x3D) {
        pDiespr->Free2 = 0;
        pDiespr->Rno++;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/diedemo/diedemo", func_80197248);

/* RE2 Die_spr_move_end */
void func_8019768C(void)
{
    pDiespr->Free2++;
    if (pDiespr->Free2 >= 0xB5) {
        pDiespr->Free2 = 0;
        pDiespr->Rno = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/diedemo/diedemo", func_801976CC);

/* RE2 Die_spr_load: RE3 has one picture for every player */
void func_80197EF8(void)
{
    UCHAR Die_type;

    Die_type = G.Pl_id;
    if (!(G.System_flg & 0x800) && (G.Status_flg & 0x80000000) &&
        (G.pUnk2660->Routine_0 == 3 || G.pUnk2660->Routine_0 == 7)) {
        Diedemo.Em_flg = 1;
        Die_type = 8;
    }
    /* RE2 read one file per player type here; RE3 reads the same one in every case */
    switch (Die_type) {
    default:
        Cd_read(0x16, (ULONG *)0x8019C000, 1, "DIEDEMO.TIM");
        break;
    case 1:
    case 3:
        Cd_read(0x16, (ULONG *)0x8019C000, 1, "DIEDEMO.TIM");
        break;
    case 8:
        Cd_read(0x16, (ULONG *)0x8019C000, 1, "DIEDEMO.TIM");
        break;
    }
}
