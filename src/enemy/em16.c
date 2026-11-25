#include "enemy/em16.h"
#include "door.h"
#include "sca.h"
#include "snd.h"

/* Capcom's EM10.C called these without a prototype in scope (RE2 EM10.H lists them), so they return
 * int and take unnarrowed int arguments here; the shared prototypes would add the masking. */
LONG Rnd_l(void) __asm__("Rnd");
void Root_ck_(ENEMY_WORK *pEm, VECTOR *pV, LONG Dest_area, LONG Mode) __asm__("Root_ck"); /* Mode is Etc_flg & 0x400 */
void func_8002646C_(ENEMY_WORK *pEm, LONG A, LONG B, LONG C) __asm__("Em_tilt_set");
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");

/* RE2 EM_MAIN: per-frame entry of the zombie: route check, timers, routine dispatch, splashes, neck and sounds */
void Em10_main(ENEMY_WORK *pEm)
{
    VECTOR *pTv;
    USHORT *pFlg;
    SVECTOR Sv;                        /* unused (frame size) */
    tagGLOBAL *pG = &G;

    pTv = POS_V(&pG->Player);
    if ((LONG)pG->Status_flg < 0) {
        if ((pEm->Unk158 & 0xC0000000) == 0x40000000)
            pTv = POS_V(pG->pEnemy[1]);
        if (pEm->Type & 0x200)
            pTv = POS_V(pG->pEnemy[1]);
    }
    pFlg = &EM10_W(pEm)->Etc_flg;
    Root_ck_(pEm, pTv, EM10_W(pEm)->Rnd_area, EM10_W(pEm)->Etc_flg & 0x400);
    if (EM10_W(pEm)->Dam_s)
        EM10_W(pEm)->Dam_s--;
    if (EM10_W(pEm)->Catch_timer)
        EM10_W(pEm)->Catch_timer--;
    if (EM10_W(pEm)->u2C.w.Mv10_timer)
        EM10_W(pEm)->u2C.w.Mv10_timer--;
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (EM10_W(pEm)->Hanare_cnt) {
        if (--EM10_W(pEm)->Hanare_cnt == 0)
            G.Room_enemy_flg &= ~1;
    }
    Em10_routine_0[pEm->Routine_0](pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    if (pEm->Water) {
        if (pEm->pSin_parts_ptr->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 0xF) == 1 && pEm->Spd_x >= 11)
            Em10_water(pEm);
        if ((*pEm->pNow_seq & 0xF000) == 0x1000)
            Em10_water(pEm);
        if ((*pEm->pNow_seq & 0xF000) == 0x7000)
            Em10_water(pEm);
    }
    Hit_area_parts_set(pEm, 0xFDA80001, 0x4B00006);
    EM10_W(pEm)->A_timer++;
    if (*pFlg & 0x180)
        Em10_Kubigoro(pEm);
    EM10_W(pEm)->Old_r = EM_RTN(pEm);
    Em_seq_sound(pEm);
}

/* RE2 Em10_Init: life, collision, neck limits and the start routine picked by Type & 0xF */
void Em10_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG Tex;
    LONG i;

    EM_SET_MOVE(pEm, 0, 0, 0);
    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Spd_x = 0;
    EM10_W(pEm)->Walk_no = Rnd_l() % 3;
    EM10_W(pEm)->Walk_no2 = Rnd_l() % 3 + 3;
    EM10_W(pEm)->u2C.w.Unk2E = (Rnd_l() & 1) + 6;
    EM10_W(pEm)->Etc_flg = 0;
    EM10_W(pEm)->Kahan_r0 = 0;
    EM10_W(pEm)->Dm_r0 = 0;
    EM10_W(pEm)->Dm_cnt = (Rnd_l() & 0xF) + 0x10;
    EM10_W(pEm)->Hanare_cnt = 0;
    EM10_W(pEm)->Die_r0 = 0;
    EM10_W(pEm)->A_timer = 0;
    EM10_W(pEm)->Dam_s = 0;
    EM10_W(pEm)->Fire_c = 0;
    EM10_W(pEm)->Ude_timer = 0;
    EM10_W(pEm)->Ude_down = 0;
    EM10_W(pEm)->u2C.w.Mv10_timer = 0;
    EM10_W(pEm)->Catch_timer = 0;
    EM10_W(pEm)->Hanare_cnt = 0;
    if (!(Rnd_l() & 7))
        EM10_W(pEm)->Etc_flg |= 0x4000;
    EM10_W(pEm)->Etc_flg |= 0x8000;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x2580190, 0, pEm->pSin_parts_ptr->Workm.t);
    pEm->Col.D0 = pEm->Col.W0 = 500;
    pEm->Col.H = 3000;
    pEm->Col.pEm = pEm;
    pEm->Col.Unk24 = 0;
    pEm->Col.Unk28 = 0;
    pEm->Col.Unk26 = 0;
    pEm->Col.Flg = 0;
    pEm->Col.Type = 2;
    Oba_set(&pEm->Col, 0);
    pEm->Hit_area.Top = 0x7D0;
    pEm->Hit_area.Bottom = -0x3E8;
    pEm->Hit_area.Rate0 = 0x6E;
    pEm->Hit_area.Rate1 = 0xC8;
    pEm->Hit_area.R = 0x190;
    pEm->Unk140 = 0;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 1;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[1];
    PW_NECK(pP)->Add_dir_y = 0x60;
    PW_NECK(pP)->Add_dir_z = 0x60;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    PW_NECK(pP)->L_rot_y = 0x200;
    PW_NECK(pP)->L_rot_z = 0x200;
    i = (Rnd_l() >> (Rnd_l() & 3)) & 0xF;
    pEm->Max_life = pEm->Life = Em10_life[i];
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = Em10_life_easy[i];
    pEm->Max_life += 10;
    pEm->Life += 10;
    if (pEm->Type & 0x80) {
        pEm->Status_flg |= 1;
        pEm->Type &= 0xFF7F;
        Em10_Firedm(pEm);
    }
    EM_SET_MOVE(pEm, 0, 0, 0);
    if ((pEm->Type & 0xF) == 6) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Type &= 0xFFF0;
    }
    if ((pEm->Type & 0xF) == 1) {
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        EM_SET_RTN(pEm, 1, 2, 0, 0);
        pEm->Hit_area.Top = 0xC8;
        pEm->Hit_area.Bottom = -0x12C;
    }
    if ((pEm->Type & 0xF) == 3) {
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        pEm->Hit_area.Top = 0xC8;
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        pEm->Hit_area.Bottom = -0x12C;
        pEm->Type = (pEm->Type & 0xFFF0) | 1;
    }
    if ((pEm->Type & 0xF) == 2) {
        EM_SET_MOVE(pEm, 0x17, 0, 0);
        EM_SET_RTN(pEm, 1, 7, 0, 0);
        EM10_W(pEm)->Etc_flg |= 4;
        if (pEm->Add_dir)
            EM_SET_MOVE(pEm, 0xA, pEm->Add_dir, 0);
    }
    if ((pEm->Type & 0xF) == 4) {
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        EM_SET_RTN(pEm, 1, 7, 0, 0);
        if (pEm->Add_dir)
            EM_SET_MOVE(pEm, 9, pEm->Add_dir, 0);
    }
    if ((pEm->Type & 0xF) == 5) {
        EM_SET_MOVE(pEm, 0x17, 0, 0);
        EM_SET_RTN(pEm, 1, 7, 0, 0);
        pEm->Type = 0x4002;
        pEm->Life = -1;
        EM10_W(pEm)->Etc_flg |= 4;
    }
    if ((pEm->Type & 0xF) == 7) {
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        EM_SET_RTN(pEm, 1, 7, 0, 0);
        pEm->Type = 0x4004;
        pEm->Life = -1;
    }
    if ((pEm->Type & 0xF) == 8) {
        EM_SET_MOVE(pEm, 0x13, 0, 0);
        EM_SET_RTN(pEm, 1, 8, 0, 0);
    }
    if ((pEm->Type & 0xF) == 9) {
        pEm->Cdir_z = -0x400;
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    if ((pEm->Type & 0xF) == 0xA) {
        EM_SET_MOVE(pEm, 0, 0, 0);
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
        pP = pEm->pSin_parts_ptr;
        for (i = 11; i != 0; i--, pP++) {
            pP->UnkA2 = (pP->UnkA2 | 0x400) & ~1;
        }
    }
    if (pEm->Type & 1)
        pEm->Type |= 0x2000;
    if (pEm->Id == 0x1F) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() & 7;
        if (Tex == 1 || Tex == 3) {
            pP[0].pTmd = &pEm->pUnk174[1];
            pP[2].pTmd = &pEm->pUnk174[2];
            pP[4].pTmd = &pEm->pUnk174[3];
            pP[1].pTmd = &pEm->pUnk174[0];
        }
        if (Tex == 4 || Tex == 5) {
            pP[0].pTmd = &pEm->pUnk174[9];
            pP[2].pTmd = &pEm->pUnk174[10];
            pP[4].pTmd = &pEm->pUnk174[11];
            if (Tex == 5)
                pP[1].pTmd = &pEm->pUnk174[0];
        }
        if (Tex == 6 || Tex == 7) {
            pP[0].pTmd = &pEm->pUnk174[12];
            pP[2].pTmd = &pEm->pUnk174[14];
            pP[4].pTmd = &pEm->pUnk174[16];
            pP[1].pTmd = &pEm->pUnk174[13];
            pP[3].pTmd = &pEm->pUnk174[15];
            pP[5].pTmd = &pEm->pUnk174[17];
            pP[6].pTmd = &pEm->pUnk174[18];
            pP[7].pTmd = &pEm->pUnk174[19];
            pP[8].pTmd = &pEm->pUnk174[20];
            pP[9].pTmd = &pEm->pUnk174[21];
            pP[10].pTmd = &pEm->pUnk174[22];
            pEm->Mot_scale_y = 0xED4;
            Parts_work_pos_init(pEm, (SHORT *)pEm->pSub0_kan_t_ptr);
        }
        if (Tex == 7) {
            pP[0].pTmd = &pEm->pUnk174[23];
            pP[2].pTmd = &pEm->pUnk174[24];
            pP[4].pTmd = &pEm->pUnk174[25];
        }
    }
    if (pEm->Id == 0x1E || pEm->Id == 0x16) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() & 7;
        if (Tex == 2 || Tex == 3) {
            pP[0].pTmd = &pEm->pUnk174[0];
            pP[2].pTmd = &pEm->pUnk174[1];
            pP[4].pTmd = &pEm->pUnk174[2];
            if (Tex == 3)
                pP[1].pTmd = &pEm->pUnk174[18];
        }
        if (Tex == 1 || Tex == 3 || Tex == 5) {
            pP[6].pTmd = &pEm->pUnk174[3];
            pP[7].pTmd = &pEm->pUnk174[4];
            pP[8].pTmd = &pEm->pUnk174[5];
            pP[9].pTmd = &pEm->pUnk174[6];
            pP[10].pTmd = &pEm->pUnk174[7];
            if (Tex == 1)
                pP[1].pTmd = &pEm->pUnk174[18];
        }
        if (Tex == 4 || Tex == 5) {
            pP[0].pTmd = &pEm->pUnk174[8];
            pP[2].pTmd = &pEm->pUnk174[9];
            pP[3].pTmd = &pEm->pUnk174[10];
            pP[4].pTmd = &pEm->pUnk174[11];
            pP[5].pTmd = &pEm->pUnk174[12];
            if (Tex == 5)
                pP[1].pTmd = &pEm->pUnk174[18];
        }
        if (Tex == 6 || Tex == 7) {
            pP[0].pTmd = &pEm->pUnk174[13];
            pP[2].pTmd = &pEm->pUnk174[14];
            pP[3].pTmd = &pEm->pUnk174[15];
            pP[4].pTmd = &pEm->pUnk174[16];
            pP[5].pTmd = &pEm->pUnk174[17];
            pP[1].pTmd = &pEm->pUnk174[18];
        }
    }
    if (pEm->Id == 0x19) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() & 7;
        if (Tex == 1 || Tex == 3) {
            pP[6].pTmd = &pEm->pUnk174[4];
            pP[7].pTmd = &pEm->pUnk174[5];
            pP[8].pTmd = &pEm->pUnk174[6];
            pP[9].pTmd = &pEm->pUnk174[7];
            pP[10].pTmd = &pEm->pUnk174[8];
            if (Tex == 1)
                pP[1].pTmd = &pEm->pUnk174[1];
        }
        if (Tex == 2) {
            pP[0].pTmd = &pEm->pUnk174[0];
            pP[1].pTmd = &pEm->pUnk174[1];
            pP[2].pTmd = &pEm->pUnk174[2];
            pP[4].pTmd = &pEm->pUnk174[3];
        }
        if (Tex == 6) {
            pP[0].pTmd = &pEm->pUnk174[19];
            pP[1].pTmd = &pEm->pUnk174[20];
            pP[2].pTmd = &pEm->pUnk174[21];
            pP[3].pTmd = &pEm->pUnk174[22];
            pP[4].pTmd = &pEm->pUnk174[23];
            pP[5].pTmd = &pEm->pUnk174[24];
            pP[6].pTmd = &pEm->pUnk174[25];
            pP[7].pTmd = &pEm->pUnk174[26];
            pP[8].pTmd = &pEm->pUnk174[27];
            pP[9].pTmd = &pEm->pUnk174[28];
            pP[10].pTmd = &pEm->pUnk174[29];
            pEm->Mot_scale_y = 0xED4;
            Parts_work_pos_init(pEm, (SHORT *)pEm->pSub0_kan_t_ptr);
        }
        if (Tex == 7) {
            pP[0].pTmd = &pEm->pUnk174[9];
            pP[1].pTmd = &pEm->pUnk174[30];
            pP[2].pTmd = &pEm->pUnk174[10];
            pP[3].pTmd = &pEm->pUnk174[11];
            pP[4].pTmd = &pEm->pUnk174[12];
            pP[5].pTmd = &pEm->pUnk174[13];
            pP[6].pTmd = &pEm->pUnk174[14];
            pP[7].pTmd = &pEm->pUnk174[15];
            pP[8].pTmd = &pEm->pUnk174[16];
            pP[9].pTmd = &pEm->pUnk174[17];
            pP[10].pTmd = &pEm->pUnk174[18];
        }
    }
    if (pEm->Id == 0x1B) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() & 7;
        if (Tex == 2 || Tex == 3) {
            pP[0].pTmd = &pEm->pUnk174[1];
            pP[2].pTmd = &pEm->pUnk174[2];
            pP[4].pTmd = &pEm->pUnk174[3];
            if (Tex == 3)
                pP[1].pTmd = &pEm->pUnk174[0];
        }
        if (Tex == 1 || Tex == 3 || Tex == 5) {
            pP[6].pTmd = &pEm->pUnk174[15];
            pP[7].pTmd = &pEm->pUnk174[16];
            pP[8].pTmd = &pEm->pUnk174[12];
            pP[9].pTmd = &pEm->pUnk174[18];
            pP[10].pTmd = &pEm->pUnk174[14];
            if (Tex == 1)
                pP[1].pTmd = &pEm->pUnk174[23];
        }
        if (Tex == 4 || Tex == 5) {
            pP[0].pTmd = &pEm->pUnk174[17];
            pP[2].pTmd = &pEm->pUnk174[19];
            pP[3].pTmd = &pEm->pUnk174[20];
            pP[4].pTmd = &pEm->pUnk174[21];
            pP[5].pTmd = &pEm->pUnk174[22];
            if (Tex == 4)
                pP[1].pTmd = &pEm->pUnk174[23];
            if (Tex == 5)
                pP[1].pTmd = &pEm->pUnk174[0];
        }
        if (Tex == 6 || Tex == 7) {
            pP[0].pTmd = &pEm->pUnk174[4];
            pP[1].pTmd = &pEm->pUnk174[5];
            pP[2].pTmd = &pEm->pUnk174[6];
            pP[3].pTmd = &pEm->pUnk174[7];
            pP[4].pTmd = &pEm->pUnk174[8];
            pP[5].pTmd = &pEm->pUnk174[9];
            pP[6].pTmd = &pEm->pUnk174[10];
            pP[7].pTmd = &pEm->pUnk174[11];
            pP[8].pTmd = &pEm->pUnk174[12];
            pP[9].pTmd = &pEm->pUnk174[13];
            pP[10].pTmd = &pEm->pUnk174[14];
        }
    }
    if (pEm->Id == 0x1D) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() & 3;
        if (Tex == 2)
            pP[1].pTmd = &pEm->pUnk174[11];
        if (Tex == 1) {
            pP[0].pTmd = &pEm->pUnk174[0];
            pP[1].pTmd = &pEm->pUnk174[1];
            pP[2].pTmd = &pEm->pUnk174[2];
            pP[3].pTmd = &pEm->pUnk174[3];
            pP[4].pTmd = &pEm->pUnk174[4];
            pP[5].pTmd = &pEm->pUnk174[5];
            pP[6].pTmd = &pEm->pUnk174[6];
            pP[7].pTmd = &pEm->pUnk174[7];
            pP[8].pTmd = &pEm->pUnk174[8];
            pP[9].pTmd = &pEm->pUnk174[9];
            pP[10].pTmd = &pEm->pUnk174[10];
            pEm->Mot_scale_y = 0xED4;
            Parts_work_pos_init(pEm, (SHORT *)pEm->pSub0_kan_t_ptr);
        }
    }
    if (pEm->Id == 0x18) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() & 1;
        if (Tex == 1) {
            pP[0].pTmd = &pEm->pUnk174[0];
            pP[1].pTmd = &pEm->pUnk174[10];
            pP[2].pTmd = &pEm->pUnk174[1];
            pP[3].pTmd = &pEm->pUnk174[2];
            pP[4].pTmd = &pEm->pUnk174[3];
            pP[5].pTmd = &pEm->pUnk174[4];
            pP[6].pTmd = &pEm->pUnk174[5];
            pP[7].pTmd = &pEm->pUnk174[6];
            pP[8].pTmd = &pEm->pUnk174[7];
            pP[9].pTmd = &pEm->pUnk174[8];
            pP[10].pTmd = &pEm->pUnk174[9];
        }
    }
    if (pEm->Id == 0x1C) {
        Parts_work_pos_init(pEm, (SHORT *)pEm->pSub0_kan_t_ptr);
        Tex = pEm->Unk4B & 7;
        if (Tex == 1) {
            pP = pEm->pSin_parts_ptr;
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[1];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[4];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[2];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[6];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[5];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 |= 4;
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP = &pEm->pSin_parts_ptr[7];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[9];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101020;
            pP->UnkA2 |= 4;
        }
    }
    if (pEm->Id == 0x1A) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() % 5;
        if (Tex == 1 || Tex == 3)
            pP[1].pTmd = &pEm->pUnk174[1];
        if (Tex == 2 || Tex == 3) {
            pP[0].pTmd = &pEm->pUnk174[0];
            pP[2].pTmd = &pEm->pUnk174[2];
            pP[3].pTmd = &pEm->pUnk174[3];
            pP[4].pTmd = &pEm->pUnk174[4];
            pP[5].pTmd = &pEm->pUnk174[5];
            pP[6].pTmd = &pEm->pUnk174[6];
            pP[7].pTmd = &pEm->pUnk174[7];
            pP[8].pTmd = &pEm->pUnk174[8];
            pP[9].pTmd = &pEm->pUnk174[9];
            pP[10].pTmd = &pEm->pUnk174[10];
        }
        if (Tex == 4) {
            pP[0].pTmd = &pEm->pUnk174[11];
            pP[1].pTmd = &pEm->pUnk174[12];
            pP[2].pTmd = &pEm->pUnk174[13];
            pP[3].pTmd = &pEm->pUnk174[14];
            pP[4].pTmd = &pEm->pUnk174[15];
            pP[5].pTmd = &pEm->pUnk174[16];
            pP[6].pTmd = &pEm->pUnk174[17];
            pP[7].pTmd = &pEm->pUnk174[18];
            pP[8].pTmd = &pEm->pUnk174[19];
            pP[9].pTmd = &pEm->pUnk174[20];
            pP[10].pTmd = &pEm->pUnk174[21];
            pEm->Mot_scale_y = 0xED4;
            Parts_work_pos_init(pEm, (SHORT *)pEm->pSub0_kan_t_ptr);
        }
    }
    G.pPldamage_Em[pEm->Id - 0x10] = Pldamege_Em10;
    G.pPldie_Em[pEm->Id - 0x10] = Pldie_Em10;
    pP = &pEm->pSin_parts_ptr[3];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[5];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[7];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[8];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[9];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[10];
    pP->UnkA2 |= 0x200;
    Joint_move(pEm, pKan, pSeq, 0x100);
}

#include "enemy/common/em10/Em10_Move.inc.c"

#include "enemy/common/em10/Em10_Move_type00.inc.c"

#include "enemy/common/em10/Em10_Move_type01.inc.c"

#include "enemy/common/em10/Em10_Damage.inc.c"

/* standing hit: stagger (dm01), recoil (dm00, Damage_flg & 8) or, 1 in 4, a knock-down (dm02) */
void Em10_dm_sel00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_3 = 0;
        if (pEm->Damage_flg & 8)
            pEm->Routine_3 = 1;
    }
    switch (pEm->Routine_3) {
    case 0:
        Em10_dm01(pEm, pKan, pSeq);
        break;
    case 1:
        Em10_dm00(pEm, pKan, pSeq);
        break;
    }
}

#include "enemy/common/em10/Em10_dm_sel01.inc.c"

/* RE2 Em10_Die: status effects of the killing hit, then the Em10_die routine of the weapon and state */
void Em10_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        if ((pEm->Damage_flg & 0x100) && !(pEm->Status_flg & 1))
            Em10_Firedm(pEm);
        if ((pEm->Damage_flg & 0x400) && !(pEm->Status_flg & 1))
            Em10_Tudm(pEm);
        if ((pEm->Damage_flg & 0x200) && !(pEm->Status_flg & 2))
            Em10_Spdm(pEm);
        if ((pEm->Damage_flg & 0x800) && !(pEm->Status_flg & 2))
            Em10_Ryudm(pEm);
    }
    if (pEm->Type & 1) {
        Em10_die01(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Etc_flg & 0x10) {
        Em10_die05(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Etc_flg & 2) {
        Esp_call(0, 0x1F400000, &pEm->pSin_parts_ptr->Workm, 0, 0);
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        EM_SET_RTN(pEm, 7, 9, 0, 0);
        return;
    }
    Em10_die[pEm->Damage_no - 1][pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em10_Scenario: scenario-driven routines (Em10_sce, or the common one for Type & 0x100) */
void Em10_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG i;

    if (pEm->Status_flg & 1) {
        pP = pEm->pSin_parts_ptr;
        i = 11;
        if ((UCHAR)pP->Poly_rgb != 0x20 && !(pEm->UnkDA & 3)) {
            for (; i != 0; i--, pP++)
                pP->Poly_rgb -= 0x10101;
        }
    }
    if (pEm->Type & 0x100)
        Em_sce_main(pEm, pKan, pSeq);
    else
        Em10_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

#include "enemy/common/em10/Em10_sce00.inc.c"

/* RE2 Em10_sce01: walking to Dest_x/Dest_z under scenario control */
void Em10_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, EM10_W(pEm)->Walk_no2, 0, 0x1F);
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_sce02: scripted attack on another character (G.pEnemy[Unk14A]) */
void Em10_sce02(void) {
}

/* RE2 Em10_sce03: plays the scenario's motion (Add_dir / Unk14C), then picks the routine from Unk14A */
void Em10_sce03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        EM_MOVE(pEm) = (pEm->Unk14C << 8) + 0x70000 + pEm->Add_dir;
    case 1:
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (pEm->Unk14C && !pEm->UnkE3) {
            pEm->UnkE3 = 1;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        pEm->Routine_2 += Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        if (pEm->Unk14A & 1) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (pEm->Unk14A & 2)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (pEm->Unk14A & 4)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            if (pEm->Unk14A & 8) {
                EM_SET_RTN(pEm, 1, 7, 2, 0);
                pEm->Add_dir = 0;
            }
            if (pEm->Type & 1)
                EM_SET_RTN(pEm, 1, 0, 0, 0);
        }
        break;
    }
}
#include "enemy/common/em10/Em10_sce04.inc.c"

/* RE2 Em10_Dead: the corpse: fades acid/fire colours, sinks the shadow, removes the enemy when done */
void Em10_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pKage;
    PARTS_W *pP;
    LONG i;

    pKage = pEm->Kage.pKw;
    if (pEm->Be_flg & 8) {
        if (pEm->Add_col[2] > 0) {
            if ((pEm->Add_col[2] -= 16) < 0)
                pEm->Add_col[2] = 0;
        }
        if (pEm->Add_col[0] > 0) {
            if ((pEm->Add_col[0] -= 16) < 0)
                pEm->Add_col[0] = 0;
        }
        if (pEm->Add_col[2] < 0) {
            if ((pEm->Add_col[2] += 16) > 0)
                pEm->Add_col[2] = 0;
        }
        if (pEm->Add_col[0] < 0) {
            if ((pEm->Add_col[0] += 16) > 0)
                pEm->Add_col[0] = 0;
        }
        if (!pEm->Add_col[0] && !pEm->Add_col[2]) {
            pEm->Be_flg &= ~8;
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 &= ~0x40;
            pP = &pEm->pSin_parts_ptr[6];
            pP->UnkA2 &= ~0x40;
        }
    }
    switch (pEm->Routine_1) {
    case 0:
        EM_SET_MOVE(pEm, 0x18, 0, 7);
        pEm->Neck_flg |= 1;
        pEm->Neck_no = 0;
        if (EM10_W(pEm)->Etc_flg & 4)
            EM_SET_MOVE(pEm, 0x17, 0, 7);
        pEm->Life = -1;
        pEm->Col.Id = 0;
        pEm->Routine_1 = 1;
        *(ULONG *)&pKage->Prim[0].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[0].r0 |= 0xBFBF10;
        *(ULONG *)&pKage->Prim[1].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[1].r0 |= 0xBFBF10;
        pEm->UnkE2 = 120;
        pEm->Timer0 = 40;
        EM10_W(pEm)->W_timer = 0;
        pEm->Spd_x = 0;
        Em_dead_flg_on_cand(pEm);
        if (!(pEm->Type & 0x4000) && Rnd_l() % 3) {
            pEm->Routine_1 = 8;
            return;
        }
        pEm->UnkDA = 0;
    case 1:
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pKage->Half_x += 5;
            pKage->Half_z += 5;
        }
        pEm->Routine_1 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        pEm->UnkE3 = (Rnd_l() & 0xF) + 1;
        pEm->Routine_1 = 3;
        if (pEm->Timer0-- == 0) {
            pEm->Routine_1 = 4;
            pEm->Timer0 = 30;
            break;
        }
    case 3:
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pKage->Half_x += 5;
            pKage->Half_z += 5;
        }
        if (pEm->UnkE3-- == 0)
            pEm->Routine_1 = 1;
        break;
    case 4:
        if (pEm->UnkE3-- == 0)
            pEm->Routine_1 = 5;
        break;
    case 6:
        pEm->UnkE3 = (Rnd_l() & 0x3F) + 60;
        pEm->Routine_1 = 4;
        if (pEm->Timer0-- == 0)
            pEm->Routine_1 = 7;
        break;
    case 8:
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pKage->Half_x += 5;
            pKage->Half_z += 5;
        }
        if (!pEm->UnkE2)
            pEm->Routine_1 = 7;
        break;
    case 9:
        EM_SET_MOVE(pEm, 8, 0, 7);
        if (EM10_W(pEm)->Etc_flg & 4)
            EM_SET_MOVE(pEm, 0x17, 0, 7);
        pEm->Routine_1 = 10;
        Em_dead_flg_on_cand(pEm);
    case 5:
    case 10:
        pEm->Routine_1 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 11:
        pEm->Routine_1 = 0;
    }
    pEm->UnkDA++;
    if (pEm->Status_flg & 1) {
        pP = pEm->pSin_parts_ptr;
        i = 11;
        if ((UCHAR)pP->Poly_rgb != 0x20 && !(pEm->UnkDA & 3)) {
            do {
                pP->Poly_rgb -= 0x10101;
                pP++;
            } while (--i);
        }
    }
    if ((USHORT)pEm->Water > 1) {
        if (!(EM10_W(pEm)->W_timer & 0x80)) {
            pEm->Pos_y -= 5;
            if (pEm->Pos_y < pEm->Water + 300) {
                pEm->Pos_y = pEm->Water + 300;
                EM10_W(pEm)->W_timer = 0x80;
            }
        } else {
            if (EM10_W(pEm)->W_timer < 0xA0)
                pEm->Pos_y -= 2;
            else {
                pEm->Pos_y += 2;
                if (EM10_W(pEm)->W_timer == 0xBF)
                    EM10_W(pEm)->W_timer = 0x80;
            }
            EM10_W(pEm)->W_timer++;
        }
    }
}

#include "enemy/common/em10/Pldamege_Em10.inc.c"

#include "enemy/common/em10/Em10_pldm00.inc.c"

/* RE2 Em10_pldm02: the held player (pEm) is bitten while lying on the floor */
void Em10_pldm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    UCHAR Mot[2] = { 6, 10 };
    SHORT Blo[2][3] = { { -400, -100, 0 }, { 200, -100, 0 } };
    ENEMY_WORK *pAi;
    PARTS_W *pP;
    SHORT Old_x;
    SHORT Old_z;

    pAi = pEm->pEnemy_ptr;
    if (pAi->Routine_0 == 2 || pAi->Routine_0 == 3) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        G.Player.Damage_cnt &= 0x7F;
        pEm->Neck_flg &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        return;
    }
    pEm->Be_flg |= 0x8000;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, Mot[pEm->Routine_1 - 2], 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        Goto00(pEm, pAi->Pos_x, pAi->Pos_z, 0x800);
        if (pEm->Routine_1 & 1) {
            pEm->Timer0 = 0;
            pEm->Cdir_y += 0x800;
        }
        Snd_se_stad(0x10000, POS_V(pEm), 0, 0);
        pEm->Damage_no = 0;
        pEm->Neck_flg |= 0x12;
    case 1:
        if (pEm->Move_cnt < 8)
            Goto01(pEm, pAi->Cdir_y + pEm->Timer0, 0x80);
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 3;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_SET_MOVE(pEm, Mot[pEm->Routine_1 - 2] + 1, 0, 0);
        pEm->Routine_2 = 3;
    case 3:
        if (!pEm->Move_cnt) {
            pP = &pEm->pSin_parts_ptr[10];
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            Esp_call(0, (USHORT)(pEm->Cdir_y + (Rnd_l() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0xFD00000,
                          &pP->Workm, &Sv, 0);
        }
        if (pEm->Move_cnt == 3) {
            pP = &pEm->pSin_parts_ptr[10];
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            Esp_call(0, (USHORT)(pEm->Cdir_y + (Rnd_l() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0x17D00000,
                          &pP->Workm, &Sv, 0);
        }
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        Joint_move2(pEm, pKan, pSeq, 0x800);
        break;
    case 4:
        EM_SET_MOVE(pEm, Mot[pEm->Routine_1 - 2] + 2, 0, 7);
        pEm->Routine_2 = 5;
    case 5:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 3;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 6:
        EM_SET_MOVE(pEm, 9, 0, 0);
        pEm->Routine_2 = 7;
        if (pEm->Routine_1 & 1) {
            pEm->Timer0 = 0x800;
            pEm->Cdir_y += 0x800;
        }
    case 7:
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 8:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        pEm->Damage_cnt &= 0x7F;
        pEm->Neck_flg &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        pEm->Damage_cnt = 3;
        pEm->Be_flg |= 0x10000000;
        break;
    }
}

#include "enemy/common/em10/Pldie_Em10.inc.c"

#include "enemy/common/em10/Em10_pldie00.inc.c"

#include "enemy/common/em10/Em10_Kubigoro.inc.c"

void Em10_ude_down(void) {
}

#include "enemy/common/em10/Em10_mv00_br.inc.c"

#include "enemy/common/em10/Em10_mv00.inc.c"

#include "enemy/common/em10/Em10_mv01_br.inc.c"

#include "enemy/common/em10/Em10_mv01.inc.c"

#include "enemy/common/em10/Em10_mv02_br.inc.c"

#include "enemy/common/em10/Em10_mv02.inc.c"

#include "enemy/common/em10/Em10_mv03_br.inc.c"

#include "enemy/common/em10/Em10_mv03.inc.c"

#include "enemy/common/em10/Em10_mv04_br.inc.c"

#include "enemy/common/em10/Em10_mv04.inc.c"

#include "enemy/common/em10/Em10_mv05_br.inc.c"

#include "enemy/common/em10/Em10_mv05.inc.c"

#include "enemy/common/em10/Em10_mv06_br.inc.c"

#include "enemy/common/em10/Em10_mv06.inc.c"

#include "enemy/common/em10/Em10_mv07_br.inc.c"

#include "enemy/common/em10/Em10_mv07.inc.c"

#include "enemy/common/em10/Em10_mv08_br.inc.c"

#include "enemy/common/em10/Em10_mv08.inc.c"

#include "enemy/common/em10/Em10_mv09_br.inc.c"

#include "enemy/common/em10/Em10_mv09.inc.c"

#include "enemy/common/em10/Em10_mv0a_br.inc.c"

#include "enemy/common/em10/Em10_mv0a.inc.c"

#include "enemy/common/em10/Em10_mv0b_br.inc.c"

#include "enemy/common/em10/Em10_mv0b.inc.c"

#include "enemy/common/em10/Em10_mv0c_br.inc.c"

#include "enemy/common/em10/Em10_mv0c.inc.c"

#include "enemy/common/em10/Em10_mv0d_br.inc.c"

#include "enemy/common/em10/Em10_mv0d.inc.c"

#include "enemy/common/em10/Em10_mv0e_br.inc.c"

#include "enemy/common/em10/Em10_mv0e.inc.c"

#include "enemy/common/em10/Em10_mv0f_br.inc.c"

#include "enemy/common/em10/Em10_mv0f.inc.c"

#include "enemy/common/em10/Em10_mv10_br.inc.c"

#include "enemy/common/em10/Em10_mv10.inc.c"

#include "enemy/common/em10/Em10_mv11_br.inc.c"

/* RE2 Em10_mv11: a scripted motion with effects (Routine_3 counts the frames) */
void Em10_mv11(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    LONG Parts_num;
    LONG *pIdx;
    EM10_ESP *pE;
    MATRIX *pM;
    PARTS_W *pP;

    Parts_num = 15;
    if (pEm->Id >= 0x18)
        Parts_num = 11;
    pIdx = &EM10_W(pEm)->u2C.Esp_idx;
    if (!pEm->Routine_3)
        EM10_W(pEm)->u2C.Esp_idx = 0;
    if (Em10_mv11_esp[EM10_W(pEm)->u2C.Esp_idx].Cnt != 0xFF) {
        while (pEm->Routine_3 == Em10_mv11_esp[*pIdx].Cnt) {
            pE = &Em10_mv11_esp[*pIdx];
            if (pE->Parts_no == 0xEE) {
                pEm->Routine_2++;
                (*pIdx)++;
                continue;
            }
            if (pE->Parts_no == 0xFF) {
                pM = &GsIDMATRIX;
                Sv.vx = pE->Ofs_x + pEm->Pos_x + ((signed char)Rnd_l() >> 2);
                Sv.vy = pE->Ofs_y + pEm->Pos_y;
                Sv.vz = pE->Ofs_z + pEm->Pos_z + ((signed char)Rnd_l() >> 2);
            } else {
                pP = &pEm->pSin_parts_ptr[pE->Parts_no];
                pM = &pP->Workm;
                Sv.vx = pE->Ofs_x;
                Sv.vy = pE->Ofs_y;
                Sv.vz = pE->Ofs_z;
            }
            Esp_call((((UCHAR)pEm->Unk14C & 0xF) << 24) | (pE->Id_hi << 8) | pE->Id_lo,
                          ((pE->Esp_no + (Rnd_l() >> 1)) << 16) | (USHORT)pEm->Cdir_y, pM, &Sv, 0);
            (*pIdx)++;
        }
        switch (++pEm->Routine_3) {
        case 0:
            pEm->Be_flg |= 0x20;
            break;
        case 0x4B:
            pEm->Be_flg &= ~0x20;
            break;
        }
        if (Em10_mv11_esp[*pIdx].Cnt == 0xFF)
            *pIdx = 0;
    }
    switch (pEm->Routine_2) {
    case 1:
        EM_SET_MOVE(pEm, pEm->Add_dir, 0, 0);
        pEm->Routine_2++;
        EM10_W(pEm)->Mot_dm = 0;
        EM10_W(pEm)->Etc_flg |= 0x10;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[0];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[2];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[3];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[4];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 1;
        if (Parts_num == 15) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[6];
            pP->UnkA2 |= 1;
        }
    case 2:
        if (pEm->Move_cnt == 0x46) {
            pP = &pEm->pSin_parts_ptr[6];
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 0x4B) {
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            if (Parts_num == 15) {
                pP = &pEm->pSin_parts_ptr[9];
                pP->UnkA2 |= 1;
                pP = &pEm->pSin_parts_ptr[11];
                pP->UnkA2 |= 1;
            }
        }
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (pEm->Unk14A & 2)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (pEm->Unk14A & 4)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            if (pEm->Unk14A & 8) {
                EM_SET_RTN(pEm, 1, 7, 2, 0);
                pEm->Add_dir = 0;
            }
            if (pEm->Type & 1)
                EM_SET_RTN(pEm, 1, 0, 0, 0);
            EM10_W(pEm)->Etc_flg &= 0xFFEF;
            pEm->Damage_cnt &= 0x7F;
            pP = pEm->pSin_parts_ptr;
            do {
                pP->UnkA2 &= ~0x400;
                pP++;
            } while (--Parts_num);
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_dm00: hit, recoiling on the spot */
void Em10_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    MATRIX M, M1;
    VECTOR V;                          /* unused (frame size) */
    signed char Tbl[8] = { 24, 0, -24, 0, -12, 0, 12, 0 };
    ENEMY_WORK *pPl;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pT;
    ULONG LL;
    UCHAR Cnt;

    pPl = &Pl_work;
    LL = pEm->L_pl;
    if (EM10_W(pEm)->Dm_r0 == 1) {
        Em10_dm01(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        if (pEm->Move_no >= 8)
            EM_SET_MOVE(pEm, EM10_W(pEm)->Walk_no, 0, 0x1F);
        if (!EM10_W(pEm)->Dam_s) {
            if (!(Rnd_l() & 1))
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        pEm->UnkE3 = 12;
        pEm->UnkE2 = Tbl[Rnd_l() & 7];
        Cnt = pEm->Move_cnt;
        pEm->Move_cnt = 0;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Move_cnt = Cnt;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Routine_2 = 1;
        pEm->UnkDA = 1;
        pEm->Timer0 = 0;
        EM10_W(pEm)->Dm_cnt -= Em10_dm00_usonum[pEm->Damage_no - 1];
        if (pEm->Damage_flg & 0x10)
            pEm->UnkDA = -1;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Em10_blood_set(pEm);
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -(pEm->Timer0 * pEm->UnkE2 * 8) * pEm->UnkDA;
        Sv.vz = -(pEm->Timer0 * pEm->UnkE3 * 8) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix(PW_M(pP), &M);
        pS = pEm->pSin_parts_ptr;
        pT = &pS[6];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, PW_M(pT));
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = -(pEm->Timer0 << 6) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix(PW_M(pT), &M);
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pS[7].M);
        MulMatrix2(&M1, (MATRIX *)pEm->pSin_parts_ptr[9].M);
        if (pEm->Timer0++ >= 3) {
            pEm->Routine_2 = 2;
            pEm->Timer0 = 16;
        }
        break;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -(pEm->Timer0 * pEm->UnkE2) * pEm->UnkDA;
        Sv.vz = -(pEm->Timer0 * pEm->UnkE3) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix(PW_M(pP), &M);
        pP = &pEm->pSin_parts_ptr[6];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, PW_M(pP));
        if (pEm->Timer0-- == 0) {
            if (pEm->Move_no < 8) {
                if (pEm->Move_no < 3)
                    EM_SET_RTN(pEm, 1, 1, 1, 0);
                else if (pEm->Move_no < 6) {
                    EM_SET_RTN(pEm, 1, 2, 1, 0);
                    if (EM10_W(pEm)->Etc_flg & 0x4000)
                        EM_SET_RTN(pEm, 1, 0xB, 0, 0);
                } else
                    EM_SET_RTN(pEm, 1, 0xB, 1, 0);
                pEm->UnkDA = (Rnd_l() >> 3) + 100;
                pEm->UnkE2 = (Rnd_l() & 0x1F) + 30;
                pEm->UnkE3 = (Rnd_l() & 0xF) * 2;
                pEm->Timer0 = Rnd_l();
                if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x44C) && EM10_W(pEm)->Catch_timer < 60)
                    EM_SET_RTN(pEm, 1, 0xC, 0, 0);
                if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400) && EM10_W(pEm)->Catch_timer < 60 && LL < 3000 && (Rnd_l() & 1))
                    EM_SET_RTN(pEm, 1, 0xC, 0, 0);
                break;
            }
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x44C) && EM10_W(pEm)->Catch_timer < 30)
                EM_SET_RTN(pEm, 1, 0xC, 0, 0);
            if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400) && EM10_W(pEm)->Catch_timer < 60 && LL < 3000 && (Rnd_l() & 1))
                EM_SET_RTN(pEm, 1, 0xC, 0, 0);
        }
        if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x320) && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor
            && (LONG)G.Status_flg >= 0 && !((pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40)) {
            EM_SET_RTN(pEm, 1, 3, 0, 0);
            pEm->Damage_cnt |= 0x80;
            pPl->Damage_cnt |= 0x80;
            pPl->Damage_no = 0;
            if (pPl->Be_flg & 0x10000000)
                EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

#include "enemy/common/em10/Em10_dm01.inc.c"

/* RE2 Em10_Firedm: set on fire */
void Em10_Firedm(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pW;

    pEm->Status_flg |= 1;
    if ((pEm->Damage_flg & 3) == 1) {
        PARTS_W *pP;

        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd_l();
        Sv.vy = 0x1AC - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(0x305, ((Rnd_l() << 4) + 0x1A00) << 16, &pP[6].Workm, &Sv, 0);
        pW = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pW[10].Workm, 0, 0);
        pW = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pW[7].Workm, 0, 0);
    } else {
        PARTS_W *pP;

        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd_l();
        Sv.vy = 0x1AC - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(5, ((Rnd_l() << 4) + 0x1A00) << 16, &pP[6].Workm, &Sv, 0);
        pW = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pW[1].Workm, 0, 0);
        pW = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pW[4].Workm, 0, 0);
        pEm->pSin_parts_ptr[1].Poly_rgb = 0x404040;
    }
    pEm->pSin_parts_ptr[6].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[0].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[4].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[2].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[9].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[7].Poly_rgb = 0x606060;
}

/* acid hit, the head melts */
void Em10_Ryudm(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pW;
    PARTS_W *pT;

    pEm->Add_col[2] = 0xFFF;
    pEm->Col_flg = 0x230;
    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    pP = pEm->pSin_parts_ptr;
    pEm->Add_col[0] = 0;
    pEm->Add_col[1] = 0;
    Sv.vx = 0x80 - Rnd_l();
    Sv.vy = -0xAC - Rnd_l();
    Sv.vz = 0x80 - Rnd_l();
    Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pP[6].Workm, &Sv, 0);
    if (Rnd_l() & 1) {
        pW = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pW[7].Workm, 0, 0);
    } else {
        pW = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pW[1].Workm, 0, 0);
    }
    pT = &pEm->pSin_parts_ptr[6];
    pT->UnkA2 |= 0x40;
    pT = pEm->pSin_parts_ptr;
    pT->UnkA2 |= 0x40;
}

/* hit by the grenade blast */
void Em10_Tudm(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;

    pEm->Status_flg |= 1;
    if (pEm->Type & 1) {
        Em10_Firedm(pEm);
        return;
    }
    pP = pEm->pSin_parts_ptr;
    Sv.vx = 0x80 - Rnd_l();
    Sv.vy = -300;
    Sv.vz = 0x80 - Rnd_l();
    Esp_call(0x205, ((Rnd_l() << 4) + 0x1A00) << 16, &pP[6].Workm, &Sv, 0);
    pS = pEm->pSin_parts_ptr;
    Esp_call(0x205, ((Rnd_l() << 4) + 0xA00) << 16, &pS[1].Workm, 0, 0);
    Sv.vx = 0x40 - (Rnd_l() & 0x7F);
    Sv.vy = 0;
    Sv.vz = 0x40 - (Rnd_l() & 0x7F);
    if (Rnd_l() & 1) {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x205, ((Rnd_l() << 4) + 0x1400) << 16, &pS[9].Workm, &Sv, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x205, ((Rnd_l() << 4) + 0x1000) << 16, &pS[4].Workm, &Sv, 0);
    }
    pEm->pSin_parts_ptr[6].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[0].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[1].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[4].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[2].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[9].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[7].Poly_rgb = 0x606060;
}

/* acid hit from the front, the head is pushed back */
void Em10_Spdm(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;

    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    Sv.vx = 0;
    Sv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - pEm->Cdir_y + 0x100;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    Sv.vx = 4000;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Add_col);
    pP = pEm->pSin_parts_ptr;
    pEm->Col_flg = 0x40;
    Sv.vx = 0x80 - Rnd_l();
    Sv.vy = -0xAC - Rnd_l();
    Sv.vz = 0x80 - Rnd_l();
    Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pP[6].Workm, &Sv, 0);
    if (Rnd_l() & 1) {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x1803, ((Rnd_l() << 3) + 0x800) << 16, &pS[7].Workm, 0, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x1803, ((Rnd_l() << 3) + 0x800) << 16, &pS[1].Workm, 0, 0);
    }
    pW = &pEm->pSin_parts_ptr[6];
    pW->UnkA2 |= 0x40;
    pW = pEm->pSin_parts_ptr;
    pW->UnkA2 |= 0x40;
}

/* RE2 Em10_dm02: knocked down (falls on the back, may get up again) */
void Em10_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -125, 50 };
    SVECTOR Sv;                        /* unused (frame size) */
    MATRIX M, M1;
    PARTS_W *pP;
    PARTS_W *pS;
    ULONG i;
    SHORT Dir;
    LONG Tmp;

    if (EM10_W(pEm)->Die_r0 == 1) {
        Em10_die04(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Die_r0 == 2) {
        Em10_die03(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        if ((Rnd_l() & 3) && pEm->Routine_0 == 3) {
            EM10_W(pEm)->Die_r0 = 1;
            Em10_die04(pEm, pKan, pSeq);
            return;
        }
        Dir = pEm->Damage_flg & 0x10;
        EM_MOVE(pEm) = 0x30005 - (Dir = Dir != 0);
        pEm->Routine_2 = 1;
        pEm->Add_dir = 0;
        pEm->Unk14A = 0;
        pEm->Unk14C = 0;
        pEm->Timer0 = 0;
        pEm->UnkE3 = 0;
        pEm->Spd_x = Spd[(pEm->Damage_flg >> 4) & 1];
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x17700000, &pP->Workm, 0, 0);
        Esp_call(8, ((Rnd_l() + 0x7D0) << 17) | (USHORT)(Rnd_l() << 4), &pP->Workm, 0, 0);
        i = (Rnd_l() & 3) + 2;
        do {
            i--;
            Esp_call(0x209, ((Rnd_l() + 0xC00) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, 0, 0);
        } while (i);
        if (!EM10_W(pEm)->Dam_s) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        Snd_se_enem(11, pEm);
        EM10_W(pEm)->Ground = pEm->Ground;
        Em_dead_flg_on_cand(pEm);
        pEm->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x400;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->Routine_2 == 2) {
            pP = pEm->pSin_parts_ptr;
            RotMatrix(SUPER_VECTOR(pEm), &M);
            MulMatrix(PW_M(pP), &M);
            pS = &pEm->pSin_parts_ptr[6];
            TransposeMatrix(&M, &M1);
            MulMatrix2(&M1, PW_M(pS));
        }
        if (pEm->Move_cnt) {
            if (!(pEm->Move_cnt & 3)) {
                pP = pEm->pSin_parts_ptr;
                Esp_call(0, (((Rnd_l() << 3) + 0x1770) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm,
                              EM_SPD_SV(pEm), 0);
            }
        }
        break;
    case 2:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        EM10_W(pEm)->Etc_flg &= 0xFFFB;
        pEm->Damage_cnt &= 0x7F;
        pP = pEm->pSin_parts_ptr;
        pP->Old_y = -200;
        pP->Cdir_x = 0;
        pP->Cdir_y = 0;
        pP->Cdir_z = 0x400;
        pEm->Pos_y = EM10_W(pEm)->Ground;
        EM10_W(pEm)->Etc_flg |= 1;
        pEm->Life >>= 1;
        if (pEm->Routine_0 == 3)
            pEm->Life = 10;
        pEm->Type = 0x2001;
        pEm->Be_flg &= ~0x400;
        if ((pEm->Type & 0x20) || (pEm->Status_flg & 0x400)) {
            pEm->Life = -1;
            EM_SET_RTN(pEm, 7, 0, 0, 0);
            EM10_W(pEm)->Etc_flg &= 0xFFFB;
            return;
        }
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        EM_SET_MOVE(pEm, 6, 0, 0xF);
        Joint_move(pEm, pKan, pSeq, 0x100);
        return;
    }
    if (pEm->Routine_2 < 2) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Move_cnt < 10) {
            pEm->Add_dir -= pEm->UnkE3 * 16;
            pEm->Unk14A = 0;
            pEm->Unk14C += pEm->UnkE3 * 24;
            pEm->UnkE3++;
        }
        if ((ULONG)(pEm->Move_cnt - 11) < 19) {
            pEm->Add_dir += pEm->UnkE3 * 8;
            pEm->Unk14A -= pEm->Timer0 * 2;
            pEm->Unk14C -= pEm->UnkE3 * 2;
            pEm->UnkE3--;
            pEm->Timer0++;
        }
        if ((ULONG)(pEm->Move_cnt - 31) < 19) {
            pEm->Add_dir -= pEm->UnkE3 * 4;
            pEm->Unk14A += pEm->Timer0 * 2;
            pEm->Unk14C -= pEm->UnkE3 * 2;
            pEm->UnkE3++;
            pEm->Timer0--;
        }
        RotMatrix(SUPER_VECTOR(pEm), &M);
        MulMatrix(PW_M(pP), &M);
        pS = &pEm->pSin_parts_ptr[6];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, PW_M(pS));
    }
    if (pEm->Move_cnt == 20 && pEm->Routine_2 == 1) {
        pEm->pSin_parts_ptr[6].pSuper = &pEm->Super_matrix;
        pEm->Super_matrix = pEm->pSin_parts_ptr->Workm;
        pEm->Super_matrix.t[0] = pEm->Pos_x;
        pEm->Super_matrix.t[2] = pEm->Pos_z;
        pEm->UnkDA = 10;
        pEm->UnkE2 = 2;
        pP = &pEm->pSin_parts_ptr[6];
        Esp_call(0x300, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Snd_se_enem(11, pEm);
        pP = &pEm->pSin_parts_ptr[6];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[7];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[8];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[9];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[10];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        if (pEm->Parts_num == 15) {
            pP = &pEm->pSin_parts_ptr[9];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[11];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
        }
    }
    if (pEm->Move_cnt >= 20 && pEm->Routine_2 == 1) {
        pP = pEm->pSin_parts_ptr;
        RotMatrix(CDIR_SV(pEm), (MATRIX *)pEm->Workm_m);
        MulMatrix0((MATRIX *)pEm->Workm_m, PW_M(pP), &pEm->Super_matrix);
        pEm->Super_matrix.t[1] = pP->Pos_y + EM10_W(pEm)->Ground;
        if (pEm->UnkE2) {
            pEm->Pos_y += pEm->UnkDA;
            pEm->UnkDA += 5;
            if (pEm->Move_cnt >= 35)
                pEm->UnkDA += 50;
        }
        if (EM10_W(pEm)->Ground - (200 + pP->Pos_y) < pEm->Pos_y && pEm->UnkE2) {
            Tmp = pEm->UnkDA;
            pEm->UnkDA = -(Tmp >> 3);
            pEm->Pos_y = EM10_W(pEm)->Ground - (300 + pP->Pos_y);
            pEm->UnkE2--;
            Snd_se_enem(6, pEm);
        }
    }
}

#include "enemy/common/em10/Em10_dm03.inc.c"

#include "enemy/common/em10/Em10_dm04.inc.c"

/* RE2 Em10_dm05: a leg is blown off (the zombie may go on crawling) */
void Em10_dm05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 125 };      /* unused */
    SVECTOR Sv;
    PARTS_W *pP;
    ULONG Id;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0xF);
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Move_cnt = Rnd_l() & 7;
        Spd_get2(pEm, pKan, pSeq, 0);
        Id = 0;
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        if (pEm->Damage_flg & 0x200)
            Id = 5;
        pEm->UnkE3 = Rnd_l() & 1;
        if (pEm->UnkE3 && !(EM10_W(pEm)->Etc_flg & 0x8000)) {
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_BOMB(pP)->V0_x = pEm->Cdir_y + 0x800;
            PW_BOMB(pP)->Cnt0 = 0;
            PW_BOMB(pP)->V0_z = 0;
            PW_BOMB(pP)->V0_y = -10;
            PW_BOMB(pP)->G = 0;
            PW_BOMB(pP)->Gxz = 0;
            pP->Poly_rgb = 0x10104F;
            pEm->pSin_parts_ptr[9].Be_flg = 0;
            pP = &pEm->pSin_parts_ptr[7];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
            Snd_se_enem(2, pEm);
            if (Rnd_l() & 1) {
                if (pEm->Id != 0x11 && pEm->Id != 0x13) {
                    pP = &pEm->pSin_parts_ptr[9];
                    pP->UnkA2 |= 0x20;
                    pP->Be_flg |= 2;
                    PW_BOMB(pP)->V0_x = pEm->Cdir_y + 0x800;
                    PW_BOMB(pP)->Cnt0 = 0;
                    PW_BOMB(pP)->V0_z = 0;
                    PW_BOMB(pP)->V0_y = -10;
                    PW_BOMB(pP)->G = 0;
                    PW_BOMB(pP)->Gxz = 0;
                    pP->Poly_rgb = 0x10104F;
                    Esp_call(Id << 24, 0x17700000, &pP->Workm, 0, 0);
                    Esp_call(8, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
                }
                pP = &pEm->pSin_parts_ptr[10];
                pP->UnkA2 |= 0x20;
                pP->Be_flg |= 2;
                PW_BOMB(pP)->V0_x = pEm->Cdir_y + 0x800;
                PW_BOMB(pP)->Cnt0 = 0;
                PW_BOMB(pP)->V0_z = 0;
                PW_BOMB(pP)->V0_y = -10;
                PW_BOMB(pP)->G = 0;
                PW_BOMB(pP)->Gxz = 0;
                pP->Poly_rgb = 0x10104F;
                Esp_call(Id << 24, 0x17700000, &pP->Workm, 0, 0);
                pEm->pSin_parts_ptr[11].Be_flg = 0;
            } else {
                pP = &pEm->pSin_parts_ptr[9];
                PW_RGB(pP)->Rgb0 = 0x40;
                PW_RGB(pP)->Rgb1 = 0x202030;
                pP->UnkA2 |= 4;
                pP = &pEm->pSin_parts_ptr[10];
                PW_RGB(pP)->Rgb0 = 0x40;
                PW_RGB(pP)->Rgb1 = 0x202030;
                pP->UnkA2 |= 4;
            }
        } else {
            pP = &pEm->pSin_parts_ptr[7];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[8];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[9];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[10];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
        }
        pP = &pEm->pSin_parts_ptr[8];
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(Id << 24, 0x13880000, &pP->Workm, &Sv, 0);
        Sv.vy = 400;
        Esp_call(Id << 24, (USHORT)(Rnd_l() << 2) | 0x17700000, &pP->Workm, &Sv, 0);
        Sv.vy = 800;
        Esp_call(Id << 24, (USHORT)(Rnd_l() << 4) | 0xFA00000, &pP->Workm, &Sv, 0);
        Esp_call((Id << 24) | 8, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        if (!EM10_W(pEm)->Dam_s) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        Em_dead_flg_on_cand(pEm);
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        if (pEm->Routine_0 == 3) {
            if ((Rnd_l() & 3) || !pEm->UnkE3)
                pEm->Life = 10;
            else {
                EM10_W(pEm)->Etc_flg &= 0xFFFB;
                EM_SET_RTN(pEm, 7, 0, 0, 0);
                break;
            }
        }
        if ((pEm->Type & 0x20) || (pEm->Status_flg & 0x400)) {
            pEm->Life = -1;
            EM10_W(pEm)->Etc_flg &= 0xFFFB;
            EM_SET_RTN(pEm, 7, 0, 0, 0);
            break;
        }
        pEm->Type = 0x2001;
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        else
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        pEm->Damage_cnt &= 0x7F;
    }
}

#include "enemy/common/em10/Em10_dm06.inc.c"

/* RE2 Em10_dm07: the head is hit (shakes the head part, then back to the saved routine Next_r) */
void Em10_dm07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    SVECTOR Sv;
    MATRIX M, M1;
    VECTOR V;                          /* unused (frame size) */
    signed char Tbl[8] = { 24, -8, -24, 0, -12, 0, 12, 8 };

    switch (pEm->Routine_2) {
    case 0:
        pEm->Tilt_spd = 30;
        pEm->Be_flg |= 0x80000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->UnkDA = 1;
        pEm->UnkE2 = 0;
        if (pEm->Damage_flg & 0x10)
            pEm->UnkDA = -1;
        Em10_blood_set(pEm);
        pEm->Hokan_flg = 15;
        if ((EM10_W(pEm)->Old_r & 0xFF) == 1)
            EM10_W(pEm)->Next_r = EM10_W(pEm)->Old_r;
        if (pEm->Life < 0) {
            pEm->Life = 0;
            pEm->UnkE2 = 1;
            pP = &pEm->pSin_parts_ptr[1];
            Esp_call(0x300, (USHORT)(Rnd_l() << 4) | 0x1F400000, &pP->Workm, &Sv, 0);
            pEm->Hokan_flg = 15;
        }
        pEm->UnkE3 = 12;
        pEm->UnkE2 = Tbl[Rnd_l() & 7];
    case 1:
        if (!Joint_move(pEm, pKan, pSeq, 0x100)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = -(pEm->Timer0 * pEm->UnkE2 * 8) * pEm->UnkDA;
            Sv.vz = -(pEm->Timer0 * pEm->UnkE3 * 8) * pEm->UnkDA;
            RotMatrix(&Sv, &M);
            MulMatrix(PW_M(pP), &M);
            pP = &pEm->pSin_parts_ptr[6];
            TransposeMatrix(&M, &M1);
            MulMatrix2(&M1, PW_M(pP));
            if (pEm->Timer0++ >= 3) {
                pEm->Routine_2 = 2;
                pEm->Timer0 = 16;
            }
            break;
        } else {
            EM_RTN(pEm) = EM10_W(pEm)->Next_r;
            pEm->Routine_2++;
            EM10_W(pEm)->Etc_flg &= 0xFFEF;
            EM10_W(pEm)->Etc_flg &= 0xFFFD;
        }
        break;
    case 2:
        if (!Joint_move(pEm, pKan, pSeq, 0x100)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = -(pEm->Timer0 * pEm->UnkE2) * pEm->UnkDA;
            Sv.vz = -(pEm->Timer0 * pEm->UnkE3) * pEm->UnkDA;
            RotMatrix(&Sv, &M);
            MulMatrix(PW_M(pP), &M);
            pP = &pEm->pSin_parts_ptr[6];
            TransposeMatrix(&M, &M1);
            MulMatrix2(&M1, PW_M(pP));
            if (pEm->Timer0-- == 0)
                pEm->Routine_2 = 3;
            break;
        } else {
            EM_RTN(pEm) = EM10_W(pEm)->Next_r;
            pEm->Routine_2++;
            EM10_W(pEm)->Etc_flg &= 0xFFEF;
            EM10_W(pEm)->Etc_flg &= 0xFFFD;
        }
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_RTN(pEm) = EM10_W(pEm)->Next_r;
            pEm->Routine_2++;
            EM10_W(pEm)->Etc_flg &= 0xFFEF;
            EM10_W(pEm)->Etc_flg &= 0xFFFD;
        }
    }
}

#include "enemy/common/em10/Em10_dm08.inc.c"

/* hit while crawling (Em10_haidm), the upper body may burst */
void Em10_dm09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -450, 250 };      /* unused */
    PARTS_W *pP;
    SVECTOR Sv;                        /* unused (frame size) */
    ULONG Id;

    switch (pEm->Routine_2) {
    case 0:
        Id = 0;
        EM_SET_MOVE(pEm, 7, Rnd_l() & 7, 3);
        pEm->Routine_2 = 1;
        if (pEm->Damage_flg & 0x200)
            Id = 5;
        pP = pEm->pSin_parts_ptr;
        Esp_call(Id << 24, 0x17700000, &pP->Workm, 0, 0);
        pP = &pEm->pSin_parts_ptr[1];
        Esp_call((Id << 24) | 0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pP->Workm, 0, 0);
        if (!EM10_W(pEm)->Dam_s) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Dam_s = 150;
        }
        pEm->UnkE3 = 0;
        if ((!(pEm->Damage_flg & 0xC) && (Rnd_l() & 3) == 1) || (pEm->Damage_flg & 0x400))
            pEm->UnkE3 = 1;
        if (pEm->UnkE3) {
            pP = pEm->pSin_parts_ptr;
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[6];
            pP->Be_flg |= 0x22;
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 0x1004;
            pP = &pEm->pSin_parts_ptr[7];
            pP->Be_flg |= 0x22;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[9];
            pP->Be_flg |= 0x22;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[8];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[10];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            if (pEm->Parts_num == 15) {
                pP = &pEm->pSin_parts_ptr[9];
                pP->Be_flg |= 0x20;
                pP->UnkA2 |= 0x1000;
                pP = &pEm->pSin_parts_ptr[11];
                pP->Be_flg |= 0x20;
                pP->UnkA2 |= 0x1000;
            }
        }
    case 1:
        if (pEm->UnkE3) {
            if (pEm->Move_cnt & 1) {
                pP = pEm->pSin_parts_ptr;
                Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
            }
        }
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        pEm->Type = 1;
        EM_SET_RTN(pEm, 1, 0, 0, 0);
    }
}

#include "enemy/common/em10/Em10_die00.inc.c"

#include "enemy/common/em10/Em10_die01.inc.c"

/* RE2 Em10_die02: falling dead on the back */
void Em10_die02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 125 };
    PARTS_W *pP;
    SVECTOR Sv;
    UCHAR Em10_die02_mot[2] = { 3, 1 };

    if (EM10_W(pEm)->Die_r0 == 1) {
        Em10_die04(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Life = -1;
        pEm->UnkE3 = (pEm->Damage_flg & 0x10) >> 4;
        EM10_W(pEm)->Etc_flg &= 0xFFFB;
        if (!pEm->UnkE3)
            EM10_W(pEm)->Etc_flg |= 4;
        EM_MOVE(pEm) = 0x30005 - pEm->UnkE3;
        pEm->Routine_2 = 1;
        pEm->UnkDA = 10;
        pEm->Spd_x = Spd[pEm->UnkE3];
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        pEm->Neck_flg = 1;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        pP->Poly_rgb = 0x303030;
        if (Rnd_l() & 1) {
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_BOMB(pP)->V0_x = pEm->Cdir_y + 0x800;
            PW_BOMB(pP)->V0_z = 400;
            PW_BOMB(pP)->V0_y = -100;
            PW_BOMB(pP)->Cnt0 = 0;
            PW_BOMB(pP)->G = 10;
            PW_BOMB(pP)->Gxz = -50;
            pP->Poly_rgb = 0x101030;
        }
        pEm->UnkE2 = 0;
        if (pEm->Damage_flg & 0x200)
            pEm->UnkE2 = 5;
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        Esp_call((pEm->UnkE2 & 0xF) << 24, (USHORT)(Rnd_l() << 4) | 0xBB80000, &pP->Workm, &Sv, 0);
        Esp_call(8, ((Rnd_l() + 0xFA0) << 18) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
        pP = &pEm->pSin_parts_ptr[2];
        if (!(EM10_W(pEm)->Etc_flg & 0x20)) {
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + 0x800;
            pP->Timer3 = 0;
            pP->Old_z = 100;
            pP->Old_y = 0;
            pP->Timer1 = 30;
            pP->Rot_spd_x = 0;
            pP->Rot_spd_y = pEm->Cdir_y;
            pP->Rot_spd_z = 0x400;
        } else {
            pP[0].Be_flg = 0;
            pP[1].Be_flg = 0;
        }
        pP = &pEm->pSin_parts_ptr[4];
        if (!(EM10_W(pEm)->Etc_flg & 0x40) && !(Rnd_l() & 1)) {
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + 0x800;
            pP->Timer3 = 0;
            pP->Old_z = 100;
            pP->Old_y = 0;
            pP->Timer1 = 30;
            pP->Rot_spd_x = 0;
            pP->Rot_spd_y = pEm->Cdir_y;
            pP->Rot_spd_z = 0x400;
        }
        pP = pEm->pSin_parts_ptr;
        PW_RGB(pP)->Rgb0 = 0x40;
        PW_RGB(pP)->Rgb1 = 0x202030;
        pP->UnkA2 |= 4;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -500;
        Sv.vz = 0;
        Esp_call((pEm->UnkE2 & 0xF) << 24, (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x1F400000,
                      &pP->Workm, &Sv, 0);
        Esp_call(8, ((Rnd_l() + 0xFA0) << 18) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
        Snd_se_enem(2, pEm);
        Em_dead_flg_on_cand(pEm);
        pEm->Spd_x = 11;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkDA) {
            if (!(pEm->UnkDA & 1)) {
                pP = pEm->pSin_parts_ptr;
                Esp_call((pEm->UnkE2 & 0xF) << 24,
                              (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x1F400000, &pP->Workm,
                              EM_SPD_SV(pEm), 0);
                Esp_call((pEm->UnkE2 & 0xF) << 24,
                              (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x1F400000, &pP->Workm,
                              EM_SPD_SV(pEm), 0);
            }
            pEm->UnkDA--;
        }
        break;
    case 2:
        if (Rnd_l() & 1) {
            pEm->Routine_2 = 5;
            break;
        }
        EM_MOVE(pEm) = ((pEm->UnkE3 * 15 + 10) << 8) + 0x70000 + Em10_die02_mot[pEm->UnkE3];
        pEm->Routine_2 = 3;
    case 3:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        if (!pEm->Hokan_flg) {
            if (EM10_W(pEm)->Etc_flg & 0x8000)
                Em10_foot_set(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 4:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    case 5:
        EM_SET_MOVE(pEm, EM10_W(pEm)->Walk_no, 0, 0xF);
        pEm->Routine_2 = 6;
        pEm->Timer0 = (Rnd_l() & 0x1F) + 15;
    case 6:
        if (pEm->Timer0-- == 0) {
            pEm->Routine_2 = 3;
            EM_SET_MOVE(pEm, 1, 0, 7);
            EM10_W(pEm)->Etc_flg &= 0xFFFB;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
    }
}

/* RE2 Em10_die03: the head is blown off */
void Em10_die03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG Dir;
    ULONG Id;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Life = -1;
        pEm->UnkE2 = Rnd_l() & 1;
        EM10_W(pEm)->Etc_flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Etc_flg |= 4;
        Id = 0;
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        if (pEm->Damage_flg & 0x200)
            Id = 5;
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        Esp_call((Id << 24) | 0x200, (USHORT)pEm->Cdir_y | 0x10000000, &pP->Workm, &Sv, 0);
        Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        Sv.vy -= 400;
        Esp_call(8, (USHORT)Dir | 0x10000000, &pP->Workm, &Sv, 0);
        pEm->Neck_flg = 1;
        if (G.Status_flg & 0x8000) {
            pP = &pEm->pSin_parts_ptr[1];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
        } else {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_BOMB(pP)->V0_x = Dir + 0x800;
            PW_BOMB(pP)->V0_z = 400;
            PW_BOMB(pP)->V0_y = -100;
            PW_BOMB(pP)->G = 10;
            PW_BOMB(pP)->Cnt0 = 0;
            PW_BOMB(pP)->Gxz = -50;
            pP->Poly_rgb = 0x101040;
        }
        pP = pEm->pSin_parts_ptr;
        PW_RGB(pP)->Rgb0 = 0x40;
        PW_RGB(pP)->Rgb1 = 0x202030;
        pP->UnkA2 |= 4;
        pP = &pEm->pSin_parts_ptr[2];
        PW_RGB(pP)->Rgb0 = 0x40;
        PW_RGB(pP)->Rgb1 = 0x202030;
        pP->UnkA2 |= 4;
        pP = &pEm->pSin_parts_ptr[4];
        PW_RGB(pP)->Rgb0 = 0x40;
        PW_RGB(pP)->Rgb1 = 0x202030;
        pP->UnkA2 |= 4;
        Snd_se_enem(2, pEm);
        Em_dead_flg_on_cand(pEm);
        if (Rnd_l() & 1) {
            pEm->Routine_2 = 3;
            break;
        }
        EM_MOVE(pEm) = ((pEm->UnkE2 * 10) << 8) + 0xF0000 + Mot[pEm->UnkE2];
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (!pEm->Hokan_flg) {
            func_8002646C_(pEm, 400, 40, pEm->Col.Unk39);
            if (EM10_W(pEm)->Etc_flg & 0x8000)
                Em10_foot_set(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    case 3:
        if (pEm->Move_no < 8)
            pEm->Hokan_flg = 15;
        else
            EM_SET_MOVE(pEm, EM10_W(pEm)->Walk_no, 0, 0xF);
        pEm->Routine_2 = 4;
        pEm->Timer0 = (Rnd_l() & 0x3F) + 30;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
    case 4:
        if (pEm->Timer0-- == 0) {
            pEm->Routine_2 = 1;
            EM_SET_MOVE(pEm, 1, 0, 0xF);
            EM10_W(pEm)->Etc_flg &= 0xFFFB;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    }
}

#include "enemy/common/em10/Em10_die04.inc.c"

#include "enemy/common/em10/Em10_die05.inc.c"

/* death by explosion: head, arms and legs fly off (die table, damage types 0x14-0x16) */
void Em10_die06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    UCHAR R;

    if (EM10_W(pEm)->Die_r0 == 1) {
        Em10_die02(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        if (!(Rnd_l() & 1)) {
            EM10_W(pEm)->Die_r0 = 1;
            Em10_die02(pEm, pKan, pSeq);
            return;
        }
        pEm->Life = -1;
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        Em_dead_flg_on_cand(pEm);
        EM_SET_MOVE(pEm, 3, 0, 7);
        R = Rnd_l();
        if (R & 1) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->Be_flg |= 0xA;
            pP->Old_x = R << 4;
            pP->Old_z = 100;
            pP->Old_y = -300 - (R & 0x1F);
            pP->Timer1 = 25;
            pP->Timer3 = 0;
            pP->Rot_spd_x = 0xFF;
            pP->Rot_spd_y = 0;
            pP->Rot_spd_z = 0;
            Esp_call(0, 0x20000000, &pP->Workm, 0, 0);
            Esp_call(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        R = Rnd_l();
        if (!(R & 1)) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + (SHORT)(R - 0x480);
            pP->Timer3 = 0;
            pP->Old_z = 130;
            pP->Old_y = -200 - (R & 0x1F);
            pP->Timer1 = 25;
            pP->Rot_spd_x = 0xFF;
            pP->Rot_spd_y = 0;
            pP->Rot_spd_z = 0;
            Esp_call(0, 0x20000000, &pP->Workm, 0, 0);
            Esp_call(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        pP = &pEm->pSin_parts_ptr[3];
        pP->Be_flg |= 0xA;
        pP->Old_x = pEm->Cdir_y + (R + 0x380);
        pP->Timer3 = 0;
        pP->Old_z = 150;
        pP->Old_y = -350 - (R & 0x1F);
        pP->Timer1 = 30;
        pP->Rot_spd_x = 0xFF;
        pP->Rot_spd_y = 0;
        pP->Rot_spd_z = 0;
        Esp_call(0, 0x20000000, &pP->Workm, 0, 0);
        Esp_call(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        R = Rnd_l();
        if (!(R & 1)) {
            pP = &pEm->pSin_parts_ptr[8];
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + (R + 0x380);
            pP->Timer3 = 0;
            pP->Old_z = 150;
            pP->Old_y = -400 - (R & 0x1F);
            pP->Timer1 = 30;
            pP->Rot_spd_x = 0xFF;
            pP->Rot_spd_y = 0;
            pP->Rot_spd_z = 0;
            Esp_call(0, 0x20000000, &pP->Workm, 0, 0);
            Esp_call(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        pP = &pEm->pSin_parts_ptr[10];
        pP->Be_flg |= 0xA;
        pP->Old_x = pEm->Cdir_y + (R + 0x380);
        pP->Timer3 = 0;
        pP->Old_z = 150;
        pP->Old_y = -400 - (R & 0x1F);
        pP->Timer1 = 30;
        pP->Rot_spd_x = 0xFF;
        pP->Rot_spd_y = 0;
        pP->Rot_spd_z = 0;
        Esp_call(0, 0x20000000, &pP->Workm, 0, 0);
        Esp_call(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        R = Rnd_l();
        if (R & 3) {
            pP = &pEm->pSin_parts_ptr[6];
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + (SHORT)((R << 2) + 0x600);
            pP->Timer3 = 0;
            pP->Old_z = 100;
            pP->Old_y = -400;
            pP->Timer1 = 30;
            pP->Rot_spd_x = 0xFF;
            pP->Rot_spd_y = 0;
            pP->Rot_spd_z = 0;
            Esp_call(0, 0x20000000, &pP->Workm, 0, 0);
            Esp_call(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        Esp_call(8, 0x10000000, &pP->Workm, 0, 0);
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Etc_flg |= 4;
        Em10_Tudm(pEm);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -200;
        Sv.vz = 0;
        Esp_call(5, 0x1A000000, &pP->Workm, &Sv, 0);
        Esp_call(0x1B03, 0x10000000, &pP->Workm, &Sv, 0);
        pP = pEm->pSin_parts_ptr;
        PW_RGB(pP)->Rgb0 = 0x40;
        PW_RGB(pP)->Rgb1 = 0x202030;
        pP->UnkA2 |= 4;
    case 1:
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = 0x80;
        Sv.vy = 0;
        Sv.vz = 0;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[4];
        Sv.vx = 0x80;
        Sv.vy = 0x40;
        Sv.vz = 0;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[3];
        Sv.vx = 0x80;
        Sv.vy = 0;
        Sv.vz = 0x80;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[8];
        Sv.vx = 0x40;
        Sv.vy = 0;
        Sv.vz = 0x40;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[10];
        Sv.vx = -0x40;
        Sv.vy = 0;
        Sv.vz = 0x40;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[6];
        Sv.vx = 0;
        Sv.vy = 0x80;
        Sv.vz = 0;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}

#include "enemy/common/em10/Em10_dm_dummy.inc.c"

/* keeps foot Foot (0/1, parts 8.. / 10..) on the ground by moving the body (RE2 Foot_set_pl for the zombie) */
void Em10_foot_set(ENEMY_WORK *pEm, ULONG *pKan, LONG No)
{
    MATRIX M;
    SVECTOR Ofs = { 0, 500, 0 };
    SVECTOR Sv0;
    SVECTOR Sv1;
    SVECTOR Rot;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    RotMatrix(CDIR_SV(pEm), (MATRIX *)pEm->Workm_m);
    if (pEm->Be_flg & 0xC0000) {
        Rot.vy = 0;
        Rot.vx = pEm->Tilt[0];
        Rot.vz = pEm->Tilt[1];
        RotMatrix_gte(&Rot, &M);
        MulMatrix2(&M, (MATRIX *)pEm->Workm_m);
    }
    CompM((MATRIX *)pEm->Workm_m, PW_M(pP), &M);
    CompM(&M, (MATRIX *)pP[6].M, &M);
    pP = &pP[No * 2 + 8];
    CompM(&M, (MATRIX *)pP[-1].M, &M);
    CompM(&M, PW_M(pP), &M);
    ApplyMatrixSV(&M, &Ofs, &Sv0);
    ApplyMatrixSV(&pP->Workm, &Ofs, &Sv1);
    pEm->Pos_x -= (M.t[0] + Sv0.vx) - (pP->Workm.t[0] + Sv1.vx);
    pEm->Pos_z -= (M.t[2] + Sv0.vz) - (pP->Workm.t[2] + Sv1.vz);
}

/* blood spray when hit (on the head for a head shot, else on the chest) */
void Em10_blood_set(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    ULONG Id;

    Id = 0;
    if (pEm->Damage_no == 0x1A)
        return;
    if (pEm->Damage_flg & 0x200)
        Id = 5;
    if ((pEm->Damage_flg & 3) == 1) {
        pP = &pEm->pSin_parts_ptr[6];
        Sv.vx = 0;
        Sv.vy = 800;
        Sv.vz = 0;
        Esp_call(Id << 24, (USHORT)pEm->Cdir_y | 0x17D00000, &pP->Workm, &Sv, 0);
    } else {
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x + (0x40 - (Rnd_l() & 0x7F));
        Sv.vy = pP->Pos_y - (SHORT)(Rnd_l() - 0xE4);
        Sv.vz = pP->Pos_z + (0x40 - (Rnd_l() & 0x7F));
        pP = pEm->pSin_parts_ptr;
        Esp_call(Id << 24, (((Rnd_l() << 4) + 0x800) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, &Sv, 0);
    }
}

#include "enemy/common/em10/Em10_water.inc.c"
