/*
 * DIEDEMO.BIN (RE2 DIEDEMO.C / DIESPR.C lineage): the player death demo (killer camera spiral, "YOU DIED"
 * picture, blood)
 */
#include "ovl/diedemo.h"
#include "mojidisp.h"
#include "sld.h"
#include "status.h"

LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck"); /* result used without the UCHAR mask */

/* task entry */
void Die_demo(void)
{
    Diedemo.Rno = 0;
    Diedemo.Ctr = 0;
    while (1) {
        if (Die_skip_ok) {
            if (G.Unk20FC & 0x100) {
                Fade_set(0, 0, 2, 2, 0, 0xFFFFFFFF, 0x20);
                Die_skip_ok = 0;
                if ((G.System_flg & 0x200) && (G.Status_flg & STAT_XA_VOICE)) {
                    Xa_error();
                }
                G.Unk2468 |= 0x1000000;
            }
        } else if (Fade_status(0) & FADE_END) {
            Diedemo.Rno = 7;
        }
        Die_demo_move();
        Task_sleep(1);
    }
}

/* RE2 Die_demo_move: runs Die_move_tbl[Diedemo.Rno] */
void Die_demo_move(void)
{
    Die_move_tbl[Diedemo.Rno]();
}

/* routine 0: set up the demo */
void Die_move_set(void)
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
        Id = G.Player.pEnemy_ptr->Id;
        if (Id < 0x20) {
            Diedemo.Id_ctr = 1;
        } else {
            while (Die_em_id[Diedemo.Id_ctr] != Id && Diedemo.Id_ctr != 0) {
                Diedemo.Id_ctr--;
            }
        }
        Diedemo.Wait_ctr = Die_wait_tbl[Diedemo.Id_ctr];
    } else {
        Diedemo.Id_ctr = 0;
        Diedemo.Wait_ctr = Die_wait_tbl[0];
    }
    G.Unk2468 |= 0x2000000;
    G.Unk2468 |= 0x1000000;
    G.Status_flg |= 0x180;
    G.Status_flg &= ~0x1000;
    G.Status_flg &= ~4;
    G.Status_flg &= ~2;
    Pl_wep_eff_parts_init(G_PLAYER);
    func_80046C90();
    if (G.Status_flg & STAT_XA_VOICE) {
        Xa_error();
    }
    Moji.Mess_flg = 0;
    G.Player.Damage_cnt = 0x80;
    G.Next_cut_no = G.Cut_no;
    G.pEnemy[1]->Damage_cnt = 0x80;
    if (G.System_flg & 0x200) {
        if (G.System_flg & 0x80) {
            Xa_play(3, 0x18);
            G.Status_flg |= STAT_XA_VOICE;
            func_8003884C(Die_vib_tbl, 0);
        } else {
            switch (G.Unk783E + 1) {
            case 4:
                Snd_se_stad(0x208, NULL, 0, 0);
                Xa_play(3, 0x19);
                G.Status_flg |= STAT_XA_VOICE;
                func_8003884C(Die_vib_tbl, 1);
                break;
            case 5:
                if (G.Unk7840 == 9) {
                    Xa_play(3, 0x1F);
                    G.Status_flg |= STAT_XA_VOICE;
                } else {
                    Xa_play(3, 0x1A);
                    G.Status_flg |= STAT_XA_VOICE;
                    func_8003884C(Die_vib_tbl, 2);
                }
                break;
            }
        }
        Diedemo.Id_ctr = 10;
        Diedemo.Wait_ctr = Die_wait_first[0];
    } else {
        G.Status_flg &= ~0x4000000;
        G.Status_flg &= ~0x2000000;
    }
    if (Flg_ck(G.Unk7868, 0xC2)) {
        Diedemo.Unk3B = 1;
        Diedemo.Id_ctr = 9;
        Cutinfo[9] = Die_cut_c2;
        Diedemo.Wait_ctr = 0x14;
        Diedemo.Em_flg = 1;
        G.Player.pEnemy_ptr = G.pEnemy[2];
        G.System_flg |= 0x800;
    }
    Main.Vloop = 3;
    Diedemo.Rno++;
}

/* routine 1 */
void Die_move_delay(void)
{
    Diedemo.Wait_ctr--;
    if (Diedemo.Wait_ctr < 0) {
        Diedemo.Rno++;
    }
}

/* routine 2 */
void Die_move_fade(void)
{
    if (Flg_ck(G.Unk7868, 0xC2)) {
        Snd_se_stad(0x20A, NULL, 0, 0);
    }
    Fade_set(1, 1, 2, 1, 0, 0xFFFFFF, 0x1E);
    Diedemo.Rno++;
}

/* routine 3: slow motion, then set up the camera */
void Die_move_slow(void)
{
    SND_WORK *pSnd = &Snd_work;
    tagGLOBAL *pG;
    tagGLOBAL *pGw;
    ULONG i, j, n;

    if (Fade_status(1)) {
        Main.Vloop = 2;
    }
    if (Diedemo.Free < 0x7FFF) {
        Diedemo.Ctr += Diedemo.Addpos;
        Diedemo.Addpos += 2;
        if (++Diedemo.Phase < 3) {
            if (Die_wait[Diedemo.Id_ctr].Flg & 1) {
                G.Unk2468 |= 0xFF000000;
            }
        } else {
            if (Die_wait[Diedemo.Id_ctr].Flg & 1) {
                G.Unk2468 ^= 0xFC000000;
            }
            Diedemo.Phase = 0;
            pSnd->Seq[0].Fade_mode = 2;
            pSnd->Seq[0].Fade_vol = 0x3F00;
            pSnd->Seq[0].Fade_step = 0x100;
            pSnd->Seq[1].Fade_mode = 2;
            pSnd->Seq[1].Fade_vol = 0x3F00;
            pSnd->Seq[1].Fade_step = 0x100;
            pSnd->Seq[2].Fade_mode = 2;
            pSnd->Seq[2].Fade_vol = 0x3F00;
            pSnd->Seq[2].Fade_step = 0x100;
            pSnd->Fade_mask = 0x80;
            pSnd->Fade_vol = 0x3FFF;
            pSnd->Fade_step = 0x100;
        }
        Diedemo.Free += Diedemo.Ctr;
        if (Diedemo.Free >= 0x7FFF) {
            Diedemo.Free = 0x7FFF;
            if (G.System_flg & 0x200) {
                if (G.Status_flg & STAT_XA_VOICE) {
                    Pad_vib_set1(1, 0xFA, 0);
                    Diedemo.Ctr = 1;
                    Diedemo.Addpos = 0;
                    Diedemo.Free = 0x7FFE;
                } else {
                    Pad_vib_fade_set(0x3C, 0xFA, 0x78, 0);
                }
            }
        }
    } else {
        G.Player.Be_flg &= ~0x20;
        Die_skip_ok = 1;
        Diedemo.Ctr = Die_wait[Diedemo.Id_ctr].Wait;
        while (Diedemo.Ctr-- != 0) {
            Task_sleep(1);
        }
        G.pEnemy[0]->Be_flg |= 0x80000;
        G.pEnemy[0]->Unk1BC[0] = 0;
        G.pEnemy[0]->Unk1BC[1] = 0;
        G.pEnemy[1]->Be_flg |= 0x80000;
        G.pEnemy[1]->Unk1BC[0] = 0;
        G.pEnemy[1]->Unk1BC[1] = 0;
        if (Die_wait[Diedemo.Id_ctr].Flg & 1) {
            G.Unk2468 |= 0x20000000;
        }
        Diedemo.Phase = 0;
        Die_spr_load();
        if (!(Diedemo.Em_flg & 1)) {
            G.Unk2468 |= 0x40000000;
        } else {
            G.Unk2468 |= 0x80000000;
        }
        G.pObj_work_end = &G.Ob_model[0];
        G.System_flg |= 0x80000000;
        pG = &G;
        for (i = 0; i < 32; i++) {
            pG->Ob_model[i].Be_flg = 0;
        }
        Bg_set_mode(1, 0);
        func_8001B894(0x2000000, NULL);
        pGw = &G;
        pGw->Player.Be_flg |= 0x2000;
        n = pGw->pRoom->nOmodel;
        for (j = 0; j < n; j++) {
            pGw->Ob_model[j].Be_flg |= 0x10;
        }
        if (G.System_flg & 0x800) {
            Die_move_set_cut();
        } else {
            Die_move_set_cut_usual();
        }
        Task_sleep(1);
        Diedemo.Ctr = 0;
        Diedemo.Rno += (G.System_flg & 0x800) ? 1 : 2;
        if (Die_wait[Diedemo.Id_ctr].Flg & 1) {
            G.Unk2468 &= 0x3FFFFFF;
        }
        G.Unk2468 &= ~0x1000000;
        Fade_set(1, 1, 0, 1, 0xFFFFFF, 0, 0x1E);
        Die_spr_init();
    }
}

/* camera set up from the cut info of the killer */
void Die_move_set_cut(void)
{
    CUTINFO *pCut;
    MATRIX *pM;
    RCUT *pRcut;

    Diedemo.pCutinfo = pCut = &Cutinfo[Diedemo.Id_ctr];
    Diedemo.Radius = pCut->Radius;
    Diedemo.P_radius = pCut->P_radius;
    if (!(Diedemo.Em_flg & 1)) {
        Diedemo.Theta = G.Player.Cdir_y;
    } else {
        Diedemo.Theta = G.pEnemy[1]->Cdir_y;
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
    } else if (Cdir_ck_l(G.pEnemy[1], G.Player.pEnemy_ptr)) {
        Diedemo.Theta += pCut->P_theta;
        Diedemo.Prad = pCut->Prad;
    } else {
        Diedemo.Theta += pCut->M_theta;
        Diedemo.Prad = pCut->Mrad;
    }
    switch (pCut->R_obj) {
    case 0:
        if (!(Diedemo.Em_flg & 1)) {
            pM = &G.Player.pSin_parts_ptr[pCut->R_ptr].Workm;
        } else {
            pM = &G.pEnemy[1]->pSin_parts_ptr[pCut->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Player.pEnemy_ptr->pSin_parts_ptr[pCut->R_ptr].Workm;
        break;
    }
    pRcut = &Diedemo.Rcut;
    pRcut->View_p[1] = pM->t[1] + pCut->P_viewp;
    Diedemo.Addpos = pCut->Addpos;
    Diedemo.Daddpos = 0;
    pRcut->View_r[0] = pM->t[0];
    pRcut->View_r[2] = pM->t[2];
    pRcut->View_r[1] = pM->t[1] - 300;
    Die_enemy_set();
    Die_light_set();
    Spiral_move();
    Spiral_move2();
}

/* default camera (no killer) */
void Die_move_set_cut_usual(void)
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
        Diedemo.Theta = G.Player.Cdir_y;
    } else {
        Diedemo.Theta = G.pEnemy[1]->Cdir_y;
    }
    Diedemo.pCutinfo = Cutinfo;
    if (!(Diedemo.Em_flg & 1)) {
        pM = &G.Player.pSin_parts_ptr->Workm;
    } else {
        pM = &G.pEnemy[1]->pSin_parts_ptr->Workm;
    }
    pRcut = &Diedemo.Rcut;
    pRcut->View_p[1] = pM->t[1] - 1000;
    pRcut->View_r[0] = pM->t[0];
    pRcut->View_r[2] = pM->t[2];
    pRcut->View_r[1] = pM->t[1] - 300;
    Die_enemy_set();
    Die_light_set();
    Spiral_move();
    Spiral_move2();
}

/* hide the models that are not part of the demo */
void Die_enemy_set(void)
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
        pG->pEnemy[0]->Be_flg |= 0x10;
        pG->pEnemy[0]->Type |= 0x8000;
    } else {
        pG->pEnemy[1]->Be_flg |= 0x10;
        pG->pEnemy[1]->Type |= 0x8000;
    }
    pG->pEnemy[0]->Water = 0;
    pG->pEnemy[1]->Water = 0;
    for (ppEm = &pG->pEnemy[2]; ppEm != pG->ppEnemy_end; ppEm++) {
        pG->pEm = pEm = *ppEm;
        if (pEm->Be_flg & 1) {
            pEm->Be_flg |= 0x10;
            pEm->Water = 0;
            pEm->Type |= 0x8000;
        }
    }
    if (G.System_flg & 0x800) {
        if (!(Diedemo.Unk3B & 1)) {
            Die_enemy_set_em();
        } else {
            Die_enemy_set_sub();
        }
    }
    if (G.System_flg & 0x200) {
        pG->pEnemy[0]->Be_flg |= 0x10;
        pG->pEnemy[1]->Be_flg |= 0x10;
    }
    pKill = pG->Player.pEnemy_ptr;
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

/* hides/shows the killer for the demo: Nemesis (0x34..0x36, and his 0x37 tentacles) or enemy 0x24 */
void Die_enemy_set_em(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pKill = pG->Player.pEnemy_ptr;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;

    switch ((ULONG)pKill->Id) {
    case 0x36:
        pG->Player.Be_flg |= 0x2000;
        pKill->Pos_x += pG->Player.Pos_x;
        pKill->Pos_z += pG->Player.Pos_z;
        pG->Player.Pos_x = pG->Player.Pos_z = 0;
    case 0x34:
    case 0x35:
        pKill->Be_flg ^= 0x10;
        pKill->Type ^= 0x8000;
        pKill->Be_flg |= 0x2000;
        for (ppEm = &pG->pEnemy[2]; ppEm != pG->ppEnemy_end; ppEm++) {
            pG->pEm = pEm = *ppEm;
            if (pEm->Id == 0x37) {
                pEm->Be_flg ^= 0x10;
                pEm->Type ^= 0x8000;
            }
        }
        break;
    case 0x24:
        pG->pEnemy[0]->Be_flg |= 0x10;
        pG->pEnemy[1]->Be_flg |= 0x10;
        break;
    }
}

/* Unk7868 flag 0xC2 case: resets the partner and two enemies to their demo routines, then runs D_80198228[Unk44] */
void Die_enemy_set_sub(void)
{
    tagGLOBAL *pG = &G;

    if (Flg_ck(pG->Unk7868, 0xC2)) {
        pG->pEnemy[1]->Be_flg &= 0xFFEF;
        pG->pEnemy[1]->Be_flg |= 0x2000;
        pG->pEnemy[1]->Type &= 0x7FFF;
        pG->pEnemy[2]->Be_flg &= 0xFFEF;
        pG->pEnemy[2]->Type &= 0x7FFF;
        pG->pEnemy[3]->Be_flg &= 0xFFEF;
        pG->pEnemy[3]->Type &= 0x7FFF;
        pG->pEnemy[1]->Routine_0 = 6;
        pG->pEnemy[1]->Routine_1 = 0;
        pG->pEnemy[1]->Routine_2 = 0;
        pG->pEnemy[1]->Routine_3 = 0;
        pG->pEnemy[1]->Pos_x = 0;
        pG->pEnemy[1]->Pos_z = 0;
        pG->pEnemy[1]->Old_x = 0;
        pG->pEnemy[1]->Old_z = 0;
        pG->pEnemy[2]->Pos_x = 0;
        pG->pEnemy[2]->Pos_z = 0;
        pG->pEnemy[2]->Old_x = 0;
        pG->pEnemy[2]->Old_z = 0;
        pG->pEnemy[2]->Routine_0 = 4;
        pG->pEnemy[2]->Routine_1 = 0xB;
        pG->pEnemy[2]->Routine_2 = 0;
        pG->pEnemy[2]->Routine_3 = 0;
        pG->pEnemy[2]->Type ^= 0x100;
        pG->Unk2468 = (pG->Unk2468 & ~0x40000000) | 0x80000000;
        Flg_off(pG->Unk7868, 0xC2);
        Diedemo.Unk44 = 0;
    }
    Die_sub_tbl[Diedemo.Unk44]();
}

/* death sounds 0x20B, then 0x20C 50 frames later */
void Die_se_set(void)
{
    Snd_se_stad(0x20B, NULL, 0, 0);
    Task_sleep(0x32);
    Snd_se_stad(0x20C, NULL, 0, 0);
}

/* routine 4: camera move */
void Die_move_main(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;

    if (CD.Cd_rtn_no == 0 && Diedemo.S_trans_flg == 0) {
        Diedemo.S_trans_flg = 1;
    }
    if (Diedemo.Unk3B & 1) {
        Die_cut_em36_sub();
    } else {
        Die_cut_em36();
    }
    Spiral_move();
    Spiral_move2();
    if (++Diedemo.Ctr > Diedemo.pCutinfo->Wait_ctr) {
        Diedemo.P_radius = 0;
        Diedemo.Addpos = 1;
        Diedemo.Daddpos = 1;
        Diedemo.Prad = 0x10;
        Diedemo.Ctr = 0;
        for (ppEm = &pG->pEnemy[2]; ppEm != pG->ppEnemy_end; ppEm++) {
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

/* Nemesis (0x36) kill: at frame 0x73 sets the spiral camera on the player's body and the lights */
void Die_cut_em36(void)
{
    MATRIX *pM;
    RCUT *pRcut;

    if (G.Player.pEnemy_ptr->Id == 0x36) {
        if (Diedemo.Ctr == 0x73) {
            Diedemo.Radius = 3000;
            Diedemo.P_radius = 0;
            Diedemo.Addpos = 8;
            Diedemo.Daddpos = 1;
            Diedemo.Prad = 0x10;
            if (!(Diedemo.Em_flg & 1)) {
                Diedemo.Theta = G.Player.Cdir_y;
            } else {
                Diedemo.Theta = G.pEnemy[1]->Cdir_y;
            }
            Diedemo.pCutinfo = Cutinfo;
            Cutinfo[0].Wait_ctr = Cutinfo[Diedemo.Id_ctr].Wait_ctr;
            if (!(Diedemo.Em_flg & 1)) {
                pM = &G.Player.pSin_parts_ptr->Workm;
            } else {
                pM = &G.pEnemy[1]->pSin_parts_ptr->Workm;
            }
            pRcut = &Diedemo.Rcut;
            pRcut->View_p[1] = pM->t[1] - 1000;
            pRcut->View_r[0] = pM->t[0];
            pRcut->View_r[2] = pM->t[2];
            pRcut->View_r[1] = pM->t[1] - 300;
            Die_light_set();
            Main.Vloop = 3;
        }
        if (Diedemo.Ctr == 0x78) {
            Main.Vloop = 2;
        }
    }
}

/* the same for the partner's death (camera on pEnemy[1]) at frame 0xA0 */
void Die_cut_em36_sub(void)
{
    MATRIX *pM;
    RCUT *pRcut;

    if (G.Player.pEnemy_ptr->Id == 0x36) {
        if (Diedemo.Ctr == 0xA0) {
            Diedemo.Radius = 3000;
            Diedemo.P_radius = 0;
            Diedemo.Addpos = 8;
            Diedemo.Daddpos = 1;
            Diedemo.Prad = 0x10;
            if (!(Diedemo.Em_flg & 1)) {
                Diedemo.Theta = G.Player.Cdir_y;
            } else {
                Diedemo.Theta = G.pEnemy[1]->Cdir_y;
            }
            Diedemo.pCutinfo = Cutinfo;
            Cutinfo[0].Wait_ctr = Cutinfo[Diedemo.Id_ctr].Wait_ctr + 0x14;
            if (!(Diedemo.Em_flg & 1)) {
                pM = &G.Player.pSin_parts_ptr->Workm;
            } else {
                pM = &G.pEnemy[1]->pSin_parts_ptr->Workm;
            }
            pRcut = &Diedemo.Rcut;
            pRcut->View_p[1] = pM->t[1] - 1000;
            pRcut->View_r[0] = pM->t[0];
            pRcut->View_r[2] = pM->t[2];
            pRcut->View_r[1] = pM->t[1] - 300;
            Die_light_set();
            Main.Vloop = 3;
        }
        if (Diedemo.Ctr == 0xA5) {
            Main.Vloop = 2;
        }
    }
}

/* routine 5 */
void Die_move_out(void)
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
        Die_spr_move();
    }
    if (Diedemo.Ctr < 0x20) {
        Die_em_fade();
    }
    if (Diedemo.Ctr < 0x50) {
        Spiral_move();
    } else {
        Spiral_move2();
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
void Die_move_dark(void)
{
    LIGHT_CUT_DATA *pLc;
    ULONG L;

    pLc = Get_light_cut(&G.Player.Pos_x);
    Die_spr_move();
    Spiral_move();
    Spiral_move2();
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
    Set_light(pLc, (VECTOR *)&G.pEnemy[0]->Pos_x);
}

/* routine 7: end of the demo */
void Die_move_end(void)
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
void Die_em_fade(void)
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
void Spiral_move(void)
{
    DIEDEMO *pDie = &Diedemo;
    MATRIX *pM;
    RCUT *pRcut;

    pDie->Radius -= pDie->P_radius;
    pRcut = &pDie->Rcut;
    switch (pDie->pCutinfo->R_obj) {
    case 0:
        if (!(pDie->Em_flg & 1)) {
            pM = &G.Player.pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        } else {
            pM = &G.pEnemy[1]->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Player.pEnemy_ptr->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
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

/* same as Spiral_move */
void Spiral_move2(void)
{
    DIEDEMO *pDie = &Diedemo;
    MATRIX *pM;
    RCUT *pRcut;

    pDie->Radius -= pDie->P_radius;
    pRcut = &pDie->Rcut;
    switch (pDie->pCutinfo->R_obj) {
    case 0:
        if (!(pDie->Em_flg & 1)) {
            pM = &G.Player.pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        } else {
            pM = &G.pEnemy[1]->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Player.pEnemy_ptr->pSin_parts_ptr[pDie->pCutinfo->R_ptr].Workm;
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
void Die_light_set(void)
{
    LIGHT_CUT_DATA *pLc;
    MATRIX *pM;
    RCUT *pRcut;

    switch (Diedemo.pCutinfo->R_obj) {
    case 0:
        if (!(Diedemo.Em_flg & 1)) {
            pLc = Get_light_cut(&G.Player.Pos_x);
        } else {
            pLc = Get_light_cut(&G.pEnemy[1]->Pos_x);
        }
        break;
    case 1:
        pLc = Get_light_cut((LONG *)&G.Player.pEnemy_ptr->pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm);
        break;
    }
    pRcut = &Diedemo.Rcut;
    switch (Diedemo.pCutinfo->R_obj) {
    case 0:
        if (!(Diedemo.Em_flg & 1)) {
            pM = &G.Player.pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm;
        } else {
            pM = &G.pEnemy[1]->pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm;
        }
        break;
    case 1:
        pM = &G.Player.pEnemy_ptr->pSin_parts_ptr[Diedemo.pCutinfo->R_ptr].Workm;
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
            Set_light(pLc, (VECTOR *)&G.Player.Pos_x);
        } else {
            Set_light(pLc, (VECTOR *)&G.pEnemy[1]->Pos_x);
        }
        break;
    case 1:
        Set_light(pLc, (VECTOR *)&G.Player.pEnemy_ptr->Pos_x);
        break;
    }
}

/* RE2 Die_spr_init */
void Die_spr_init(void)
{
    G.Ctp.Tpage_clut = 0x1F15;
    Mapping_tim((ULONG *)0x8019C000);
    pDiespr = (DIESPR *)0x801B1000;
    pDiespr->Rno = 0;
    pDiespr->Ctr = 0;
    Die_spr_move_init();
}

/* RE2 Die_spr_move */
void Die_spr_move(void)
{
    Set_view(Die_break_cam[0], Die_break_cam[1]);
    SetGeomScreen(0xD0);
    gte_SetRotMatrix(&GsWSMATRIX);
    gte_SetTransMatrix(&GsWSMATRIX);
    Die_spr_tbl[pDiespr->Rno]();
}

/* RE2 Die_spr_move_init: the 2 x 16x16 POLY_GT4 grid, its vertices and distances */
void Die_spr_move_init(void)
{
    SHORT Row;
    SHORT Grp;
    SHORT Col;
    ULONG Wx;
    UCHAR Wy;
    LONG Tx;
    UCHAR Ty;
    LONG Dx;
    LONG Dz;
    LONG Vz;
    POLY_GT4 *p;

    for (Grp = 0; Grp < 2; Grp++) {
        Wy = 0;
        p = pDiespr->Pgt4[Grp];
        for (Row = 0; Row < 17; Row++) {
            for (Col = Wx = 0; Col < 17; Col++) {
                if (Grp != 0) {
                    pDiespr->Pos[Col][Row].vx = Col * 30;
                    pDiespr->Pos[Col][Row].vz = Row * 11;
                    Vz = rcos(Col << 12);
                    Dx = (SHORT)(0xF0 - Col * 30);
                    pDiespr->Pos[Col][Row].vz += Vz >> 9;
                    Dz = (SHORT)(0x58 - pDiespr->Pos[Col][Row].vz);
                    pDiespr->Pos[Col][Row].vy = 0x212;
                    pDiespr->Dist[Col][Row] = SquareRoot0(Dx * Dx + Dz * Dz);
                }
                if (Row != 16 && Col != 16) {
                    SetPolyGT4(p);
                    p->tpage = 0xB5;
                    p->clut = 0x7FC0;
                    *(ULONG *)&p->r0 = 0x3E202020;
                    *(ULONG *)&p->r1 = 0x3E202020;
                    *(ULONG *)&p->r2 = 0x3E202020;
                    *(ULONG *)&p->r3 = 0x3E202020;
                    Tx = 0x10;
                    if (Col == 15) {
                        Tx = 15;
                    }
                    Ty = 4;
                    if (Row == 15) {
                        Ty = 3;
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
                    Wx += 0x10;
                }
            }
            Wy += 4;
        }
    }
    pDiespr->Free2 = 0;
    pDiespr->Blood_flg = 0;
    pDiespr->Free4 = 0xF;
    pDiespr->Free5 = 1;
    pDiespr->Move_start = (G.System_flg & 0x800) ? 0x34 : 0x16;
}

/* RE2 Die_blood_move_init (not called in RE3) */
void Die_blood_move_init(void)
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

/* RE2 Die_spr_move_main: falling 16x16 grid of the death picture, GTE-projected and brightened by distance */
INCLUDE_ASM("asm/nonmatchings/ovl/diedemo", Die_spr_move_main);

/* RE2 Die_spr_move_hold: shake the lower rows, draw the grid */
void Die_spr_move_hold(void)
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
                pDiespr->Pos[Col][Row].vz += Rnd() & 1;
            }
            if (Row == 12 && !(Col & 1) && (pDiespr->Free2 & 1)) {
                for (Row2 = Row; Row2 < 17; Row2++) {
                    pDiespr->Pos[Col][Row2].vz += Rnd() & 1;
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

/* RE2 Die_spr_move_erase: wave the grid and darken it */
void Die_spr_move_erase(void)
{
    SVECTOR V[4];
    SHORT Row;
    SHORT Col;
    LONG Dist;
    LONG Rgb;
    LONG Wave;
    UCHAR Val;
    POLY_GT4 *p;

    for (Row = 0; Row < 17; Row++) {
        for (Col = 0; Col < 17; Col++) {
            pDiespr->Pos[Col][Row].vy = 0;
            Dist = pDiespr->Dist[Col][Row];
            Wave = rsin(Dist * 0x40 - pDiespr->Free2 * 0x80) >> 6;
            Wave *= pDiespr->Free2;
            Wave = Wave / (Dist * 20 / 255 + 80) + 0x29F;
            pDiespr->Pos[Col][Row].vy += Wave;
        }
    }
    p = pDiespr->Pgt4[G.Cbuf_id];
    for (Row = 0; Row < 16; Row++) {
        for (Col = 0; Col < 16; Col++) {
            /* [17] is the next column */
            V[0] = (&pDiespr->Pos[Col][Row])[0];
            V[1] = (&pDiespr->Pos[Col][Row])[17];
            V[2] = (&pDiespr->Pos[Col][Row + 1])[0];
            V[3] = (&pDiespr->Pos[Col][Row + 1])[17];
            RotTransPers3(&V[0], &V[1], &V[2], (long *)&p->x0, (long *)&p->x1, (long *)&p->x2, NULL, NULL);
            RotTransPers(&V[3], (long *)&p->x3, NULL, NULL);
            Rgb = (255 - pDiespr->Dist[Col][Row]) >> 5;
            Rgb = p->r0 - Rgb;
            Val = Rgb;
            if (Rgb < 0)
                Val = 0;
            p->r0 = Val;
            Rgb = (255 - pDiespr->Dist[Col + 1][Row]) >> 5;
            Rgb = p->r1 - Rgb;
            Val = Rgb;
            if (Rgb < 0)
                Val = 0;
            p->r1 = Val;
            Rgb = (255 - pDiespr->Dist[Col][Row + 1]) >> 5;
            Rgb = p->r2 - Rgb;
            Val = Rgb;
            if (Rgb < 0)
                Val = 0;
            p->r2 = Val;
            Rgb = (255 - pDiespr->Dist[Col + 1][Row + 1]) >> 5;
            Rgb = p->r3 - Rgb;
            Val = Rgb;
            if (Rgb < 0)
                Val = 0;
            p->r3 = Val;
            AddPrim(G.pOt0 + 5, p);
            p++;
        }
    }
    if (++pDiespr->Free2 > 200) {
        pDiespr->Free2 = 0;
        pDiespr->Rno++;
    }
}

/* RE2 Die_spr_move_end */
void Die_spr_move_end(void)
{
    pDiespr->Free2++;
    if (pDiespr->Free2 >= 0xB5) {
        pDiespr->Free2 = 0;
        pDiespr->Rno = 0;
    }
}

/* RE2 Die_blood_move_main: projects and fades the blood quads, adds them to the OT */
INCLUDE_ASM("asm/nonmatchings/ovl/diedemo", Die_blood_move_main);

/* RE2 Die_spr_load: RE3 has one picture for every player */
void Die_spr_load(void)
{
    UCHAR Die_type;

    Die_type = G.Player.Id;
    if (!(G.System_flg & 0x800) && (G.Status_flg & 0x80000000) &&
        (G.pEnemy[1]->Routine_0 == 3 || G.pEnemy[1]->Routine_0 == 7)) {
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
