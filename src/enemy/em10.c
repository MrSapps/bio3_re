#include "enemy/em10.h"
#include "door.h"
#include "sca.h"
#include "sld.h"
#include "snd.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
void Root_ck_(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3) __asm__("Root_ck"); /* returns void here (canonical long); arg 3 long (canonical UC); arg 4 long (canonical UC) */
void func_8002646C_(ENEMY_WORK *pEm, LONG A, LONG B, LONG C) __asm__("Em_tilt_set"); /* arg 2 long (canonical short); arg 3 long (canonical short); arg 4 long (canonical UC) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");   /* Cdir_ck, result used unmasked */

/* RE2 EM_MAIN */
void Em10_main(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl;
    VECTOR *pTv;
    USHORT *pFlg;
    SVECTOR Sv;                        /* unused (frame size) */
    tagGLOBAL *pG = &G;

    pTv = (VECTOR *)&pG->Player.Pos_x;
    pPl = PLAYER_EM(pG);
    if ((LONG)pG->Status_flg < 0) {
        if ((pEm->Unk158 & 0xC0000000) == 0x40000000)
            pTv = (VECTOR *)&pG->pEnemy[1]->Pos_x;
        if (pEm->Type & 0x200)
            pTv = (VECTOR *)&pG->pEnemy[1]->Pos_x;
    }
    pFlg = &EM10_W(pEm)->Flg;
    Root_ck_(pEm, pTv, EM10_W(pEm)->Root_no, EM10_W(pEm)->Flg & 0x400);
    if (EM10_W(pEm)->Unk21)
        EM10_W(pEm)->Unk21--;
    if (EM10_W(pEm)->Unk26)
        EM10_W(pEm)->Unk26--;
    if (EM10_W(pEm)->u2C.w.Unk2F)
        EM10_W(pEm)->u2C.w.Unk2F--;
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (EM10_W(pEm)->Unk18) {
        if (--EM10_W(pEm)->Unk18 == 0)
            G.Unk75EA &= ~1;
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
    Hit_area_parts_set(pEm, 0xFDA80001, 0x190000B);
    if (EM10_W(pEm)->Ude_down)
        Em10_ude_down(pEm);
    EM10_W(pEm)->Cnt++;
    pEm->pEnemy_ptr = pPl;
    pEm->Neck_flg &= 0xFD;
    if ((pEm->Type & 0x4000) || (*pFlg & 0x200) || !(pEm->Status_flg & 0x2000))
        pEm->Neck_flg |= 2;
    Rot_neck_em(pEm, pEm->Cdir_y);
    if (*pFlg & 1)
        Em10_Kahannsin_walk(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    if (*pFlg & 0x180)
        Em10_Kubigoro(pEm);
    EM10_W(pEm)->Routine = *(ULONG *)&pEm->Routine_0;
    Em_seq_sound(pEm);
}

/* RE2 Em10_Init */
void Em10_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG Tex;
    LONG i;

    EM_SET_MOVE(pEm, 0, 0, 0);
    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Damage_cnt = 0;
    pEm->Spd_x = 0;
    EM10_W(pEm)->Walk_no = Rnd_l() % 3;
    EM10_W(pEm)->Unk27 = Rnd_l() % 3 + 3;
    EM10_W(pEm)->u2C.w.Unk2E = (Rnd_l() & 1) + 6;
    EM10_W(pEm)->Flg = 0;
    EM10_W(pEm)->Unk01 = 0;
    EM10_W(pEm)->Unk0A = 0;
    EM10_W(pEm)->Unk0B = (Rnd_l() & 0xF) + 0x10;
    EM10_W(pEm)->Unk18 = 0;
    EM10_W(pEm)->Unk19 = 0;
    EM10_W(pEm)->Cnt = 0;
    EM10_W(pEm)->Unk21 = 0;
    EM10_W(pEm)->Unk22 = 0;
    EM10_W(pEm)->Unk23 = 0;
    EM10_W(pEm)->Ude_down = 0;
    EM10_W(pEm)->u2C.w.Unk2F = 0;
    EM10_W(pEm)->Unk26 = 0;
    EM10_W(pEm)->Unk18 = 0;
    if (!(Rnd_l() & 7) && !(pEm->Status_flg & 0x400))
        EM10_W(pEm)->Flg |= 0x4000;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x2580190, 0, pEm->pSin_parts_ptr->Workm.t);
    pEm->Col.D0 = pEm->Col.W0 = 500;
    pEm->Col.H = 3000;
    pEm->Col.pEm = pEm;
    pEm->Col.Unk24 = 0;
    pEm->Col.Unk28 = 0;
    pEm->Col.Unk26 = 0;
    pEm->Col.Flg = 0;
    pEm->Col.Type = 2;
    func_80033580(&pEm->Col, 0);
    pEm->Hit_area.Top = 0x708;
    pEm->Hit_area.Bottom = -0x4B0;
    pEm->Hit_area.Rate0 = 0x78;
    pEm->Hit_area.Rate1 = 0xC8;
    pEm->Hit_area.R = 0x190;
    pEm->Unk140 = 0;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 1;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[1];
    PW_FREE(pP)[2] = 0x60;
    PW_FREE(pP)[3] = 0x60;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[5] = 0x138;
    i = (Rnd_l() >> (Rnd_l() & 3)) & 0xF;
    pEm->Max_life = pEm->Life = Em10_life[i];
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = Em10_life_easy[i];
    pEm->Max_life += 10;
    pEm->Life += 10;
    if (pEm->Id == 0x11 || pEm->Id == 0x12)
        pEm->Unk12A = 0xED4;
    if (pEm->Id == 0x2F)
        pEm->Unk12A = 0xE86;
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
        EM10_W(pEm)->Flg |= 4;
        if (pEm->Add_dir)
            EM_MOVE(pEm) = (pEm->Add_dir << 8) + 0xA;
    }
    if ((pEm->Type & 0xF) == 4) {
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        EM_SET_RTN(pEm, 1, 7, 0, 0);
        if (pEm->Add_dir)
            EM_MOVE(pEm) = (pEm->Add_dir << 8) + 9;
    }
    if ((pEm->Type & 0xF) == 5) {
        EM_SET_MOVE(pEm, 0x17, 0, 0);
        EM_SET_RTN(pEm, 1, 7, 0, 0);
        pEm->Type = 0x4002;
        pEm->Life = -1;
        EM10_W(pEm)->Flg |= 4;
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
        for (i = 15; i != 0; i--, pP++) {
            pP->UnkA2 = (pP->UnkA2 | 0x400) & ~1;
        }
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
    if (pEm->Type & 1)
        pEm->Type |= 0x2000;
    if (pEm->Id == 0x10) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() % 3;
        Tex = pEm->Unk4B & 7;
        if (Tex == 1 || Tex == 3) {
            pP[0].pTmd = &pEm->pUnk174[0];
            pP[2].pTmd = &pEm->pUnk174[1];
            pP[5].pTmd = &pEm->pUnk174[2];
        }
        if (Tex == 2 || Tex == 3) {
            pP[8].pTmd = &pEm->pUnk174[3];
            pP[9].pTmd = &pEm->pUnk174[4];
            pP[10].pTmd = &pEm->pUnk174[5];
            pP[12].pTmd = &pEm->pUnk174[6];
            pP[13].pTmd = &pEm->pUnk174[7];
        }
    }
    if (pEm->Id == 0x17) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() % 3;
        Tex = pEm->Unk4B & 7;
        if (Tex == 1 || Tex == 3) {
            pP[0].pTmd = &pEm->pUnk174[0];
            pP[2].pTmd = &pEm->pUnk174[1];
            pP[3].pTmd = &pEm->pUnk174[2];
            pP[5].pTmd = &pEm->pUnk174[3];
            pP[6].pTmd = &pEm->pUnk174[4];
        }
        if (Tex == 2 || Tex == 3) {
            pP[8].pTmd = &pEm->pUnk174[5];
            pP[9].pTmd = &pEm->pUnk174[6];
            pP[10].pTmd = &pEm->pUnk174[7];
            pP[12].pTmd = &pEm->pUnk174[8];
            pP[13].pTmd = &pEm->pUnk174[9];
        }
    }
    if (pEm->Id == 0x13) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd_l() & 1;
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
            pP[11].pTmd = &pEm->pUnk174[11];
            pP[12].pTmd = &pEm->pUnk174[12];
            pP[13].pTmd = &pEm->pUnk174[13];
            pP[14].pTmd = &pEm->pUnk174[14];
        }
    }
    G.pPldamage_Em[pEm->Id - 0x10] = Pldamege_Em10;
    G.pPldie_Em[pEm->Id - 0x10] = Pldie_Em10;
    pP = &pEm->pSin_parts_ptr[3];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[4];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[6];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[7];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[9];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[10];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[11];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[12];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[13];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[14];
    pP->UnkA2 |= 0x200;
}

#include "enemy/common/em10/Em10_Move.inc.c"

#include "enemy/common/em10/Em10_Move_type00.inc.c"

#include "enemy/common/em10/Em10_Move_type01.inc.c"

/* RE2 Em10_Damage */
void Em10_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;                        /* unused (frame size) */
    LONG Lim;

    if (!pEm->Routine_2) {
        if ((pEm->Unk132 & 0x100) && !(pEm->Status_flg & 1))
            Em10_Firedm(pEm);
        if ((pEm->Unk132 & 0x400) && !(pEm->Status_flg & 1))
            func_80108B00(pEm);
        if ((pEm->Unk132 & 0x200) && !(pEm->Status_flg & 2))
            func_80108CBC(pEm);
        if ((pEm->Unk132 & 0x800) && !(pEm->Status_flg & 2))
            func_801089A8(pEm);
    }
    if (pEm->Type & 1) {
        Em10_haidm[pEm->Unk12C - 1](pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Flg & 0x10) {
        func_80109E54(pEm, pKan, pSeq);
        return;
    }
    if (pEm->Status_flg & 0x400) {
        EM10_W(pEm)->Unk0B = 0xF;
        EM10_W(pEm)->Unk0A = 0;
    }
    Lim = 0;
    if (!pEm->Routine_2 && EM10_W(pEm)->Unk0B <= Lim) {
        if (EM10_W(pEm)->Unk0A == 1 || (pEm->Unk12C == 1 && (LONG)G.Status_flg >= 0)) {
            func_8010C57C(pEm);
            EM10_W(pEm)->Unk0A = 1;
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            EM10_W(pEm)->Unk0B = (Rnd_l() & 0xF) + 0x10;
            return;
        }
        EM10_W(pEm)->Unk0A = 1;
    }
    if (EM10_W(pEm)->Flg & 2) {
        pEm->UnkE3 = 1;
        EM_SET_RTN(pEm, 1, 5, 6, 0);
        return;
    }
    Em10_dm[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80101260(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_3 = 0;
        if (pEm->Unk132 & 8)
            pEm->Routine_3 = 1;
        if (!(pEm->Unk132 & 0xC) && !(Rnd_l() & 3) && !(pEm->Col.Hit_flg & 0x40) && !(pEm->Type & 0x20)
            && !(pEm->Status_flg & 0x400))
            pEm->Routine_3 = 2;
    }
    switch (pEm->Routine_3) {
    case 0:
        func_801082B0(pEm, pKan, pSeq);
        break;
    case 1:
        Em10_dm00(pEm, pKan, pSeq);
        break;
    case 2:
        Em10_dm02(pEm, pKan, pSeq);
        break;
    }
}

void func_8010139C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_3 = 0;
        if (!(pEm->Unk132 & 0xC))
            pEm->Routine_3 = 1;
    }
    switch (pEm->Routine_3) {
    case 0:
        func_801082B0(pEm, pKan, pSeq);
        break;
    case 1:
        func_8010B278(pEm, pKan, pSeq);
        break;
    }
}

/* RE2 Em10_Die */
void Em10_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        if ((pEm->Unk132 & 0x100) && !(pEm->Status_flg & 1))
            Em10_Firedm(pEm);
        if ((pEm->Unk132 & 0x400) && !(pEm->Status_flg & 1))
            func_80108B00(pEm);
        if ((pEm->Unk132 & 0x200) && !(pEm->Status_flg & 2))
            func_80108CBC(pEm);
        if ((pEm->Unk132 & 0x800) && !(pEm->Status_flg & 2))
            func_801089A8(pEm);
    }
    EM10_W(pEm)->Flg &= 0xFDFF;
    if (pEm->Type & 1) {
        Em10_die01(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Flg & 0x10) {
        func_8010BB08(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Flg & 2) {
        Esp_call(0, 0x1F400000, &pEm->pSin_parts_ptr->Workm, 0, 0);
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        EM_SET_RTN(pEm, 7, 9, 0, 0);
        return;
    }
    Em10_die[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em10_Scenario */
void Em10_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG i;

    if (pEm->Status_flg & 1) {
        pP = pEm->pSin_parts_ptr;
        i = 15;
        if ((UCHAR)pP->Poly_rgb != 0x20 && !(pEm->UnkDA & 3)) {
            for (; i != 0; i--, pP++)
                pP->Poly_rgb -= 0x10101;
        }
    }
    if (pEm->Type & 0x100)
        func_80016600(pEm, pKan, pSeq);
    else
        Em10_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

#include "enemy/common/em10/Em10_sce00.inc.c"

/* RE2 Em10_sce01 */
void Em10_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0x1F0000;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_sce02 */
void Em10_sce02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Type_mot[4][2] = { { 12, 12 }, { 15, 15 }, { 12, 12 }, { 15, 15 } }; /* as Em10_mv03 */
    SVECTOR Sv;
    MATRIX M;                          /* unused (frame size) */
    ENEMY_WORK *pAt = G.pEnemy[pEm->Unk14A];
    UCHAR Atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } };
    VECTOR V;                          /* unused (frame size) */
    PARTS_W *pP;
    ULONG Hai;

    Hai = pEm->Type & 1;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Type_mot[Hai][0] + 0xF0000;
        pEm->Routine_2 = 1;
        Goto00(pEm, pAt->Pos_x, pAt->Pos_z, 0x800);
        pAt->pEnemy_ptr = pEm;
        Set_base_pos(pAt, pKan, pSeq, 0);
        if (Hai & 1)
            pAt->Routine_1 += 2;
        pAt->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pAt->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pEm->Be_flg |= 0x800;
        pAt->Be_flg |= 0x800;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
    case 1:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_MOVE(pEm) = Type_mot[Hai][0] + 1;
        pEm->Routine_2 = 3;
    case 3:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        if (pEm->Add_dir)
            pEm->Routine_2 = 4;
        if (pEm->Move_cnt == Atck[Hai][0])
            Snd_se_enem((Rnd_l() & 1) | 4, pEm);
        break;
    case 4:
        EM_MOVE(pEm) = Type_mot[Hai][0] + 0x70002;
        pEm->Routine_2 = 5;
    case 5:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        if (Hai & 1) {
            if (pEm->Move_cnt == 0x1C) {
                EM10_W(pEm)->Flg |= 0x80;
                Snd_se_enem(10, pEm);
                Pad_vib_set1(4, 0xFA, 0);
            }
            if (pEm->Move_cnt >= 0x17 && pEm->Move_cnt < 0x2A && (pEm->Move_cnt & 1)) {
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 0;
                Sv.vy = 100;
                Sv.vz = 0;
                Esp_call(0, (USHORT)(pEm->Cdir_y + (Rnd_l() << 2) - 0x200) | 0x10000000, &pP[1].Workm, &Sv, 0);
            }
        }
        if (!(Hai & 1) && pEm->Move_cnt == 7)
            pEm->Routine_2 = 7;
        break;
    case 6:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        pEm->Life = -1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Damage_cnt |= 0x80;
        break;
    case 7:
        Spd_get2(pEm, pKan, pSeq, 0);
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 = 8;
    case 8:
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x -= 10;
        Add_speedXZ(pEm, 0);
        break;
    case 9:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        break;
    }
}

/* RE2 Em10_sce03 */
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
        if (pEm->Unk14A) {
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

/* RE2 Em10_Dead */
void Em10_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pKage;
    PARTS_W *pP;
    LONG i;

    pKage = pEm->Kage.pKw;
    if (pEm->Be_flg & 8) {
        if (pEm->Unk178[2] > 0) {
            if ((pEm->Unk178[2] -= 16) < 0)
                pEm->Unk178[2] = 0;
        }
        if (pEm->Unk178[0] > 0) {
            if ((pEm->Unk178[0] -= 16) < 0)
                pEm->Unk178[0] = 0;
        }
        if (pEm->Unk178[2] < 0) {
            if ((pEm->Unk178[2] += 16) > 0)
                pEm->Unk178[2] = 0;
        }
        if (pEm->Unk178[0] < 0) {
            if ((pEm->Unk178[0] += 16) > 0)
                pEm->Unk178[0] = 0;
        }
        if (!pEm->Unk178[0] && !pEm->Unk178[2]) {
            pEm->Be_flg &= ~8;
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 &= ~0x40;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 &= ~0x40;
        }
    }
    switch (pEm->Routine_1) {
    case 0:
        EM_SET_MOVE(pEm, 0x18, 0, 7);
        pEm->Neck_flg |= 1;
        pEm->Neck_no = 0;
        if (EM10_W(pEm)->Flg & 4)
            EM_SET_MOVE(pEm, 0x17, 0, 7);
        if (pEm->Life == -30000) {
            pEm->Hokan_flg = 0;
            Joint_move(pEm, pKan, pSeq, 0x200);
        }
        pEm->Life = -1;
        pEm->Col.Id = 0;
        pEm->Routine_1 = 1;
        *(ULONG *)&pKage->Prim[0].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[0].r0 |= 0xBFBF10;
        *(ULONG *)&pKage->Prim[1].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[1].r0 |= 0xBFBF10;
        pEm->UnkE2 = 120;
        pEm->Timer0 = 40;
        EM10_W(pEm)->Unk20 = 0;
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
        if (EM10_W(pEm)->Flg & 4)
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
        i = 15;
        if ((UCHAR)pP->Poly_rgb != 0x20 && !(pEm->UnkDA & 3)) {
            do {
                pP->Poly_rgb -= 0x10101;
                pP++;
            } while (--i);
        }
    }
    if ((USHORT)pEm->Water > 1) {
        if (!(EM10_W(pEm)->Unk20 & 0x80)) {
            pEm->Pos_y -= 5;
            if (pEm->Pos_y < pEm->Water + 300) {
                pEm->Pos_y = pEm->Water + 300;
                EM10_W(pEm)->Unk20 = 0x80;
            }
        } else {
            if (EM10_W(pEm)->Unk20 < 0xA0)
                pEm->Pos_y -= 2;
            else {
                pEm->Pos_y += 2;
                if (EM10_W(pEm)->Unk20 == 0xBF)
                    EM10_W(pEm)->Unk20 = 0x80;
            }
            EM10_W(pEm)->Unk20++;
        }
    }
}

#include "enemy/common/em10/Pldamege_Em10.inc.c"

#include "enemy/common/em10/Em10_pldm00.inc.c"

/* RE2 Em10_pldm02 */
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
        EM_MOVE(pEm) = Mot[pEm->Routine_1 - 2] + 0x70000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        if (pEm->Routine_1 & 1)
            pEm->Timer0 = 0;
        Snd_se_stad(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk12C = 0;
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
        EM_MOVE(pEm) = Mot[pEm->Routine_1 - 2] + 1;
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
        EM_MOVE(pEm) = Mot[pEm->Routine_1 - 2] + 0x70002;
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

/* RE2 Em10_Kahannsin_walk: the lower half crawling on its own */
void Em10_Kahannsin_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    JOINT_P Jp = { 0, 0, 0, 0, 0x7F00 };
    SHORT Tmp;
    SVECTOR Spd;
    SVECTOR Sv;
    MATRIX M;
    SHORT Ground;
    KAGE_WORK *pKage;
    PARTS_W *pP;

    switch (EM10_W(pEm)->Unk01) {
    case 0:
        EM10_W(pEm)->Unk06 = 7;
        pP = &pEm->pSin_parts_ptr[8];
        EM10_W(pEm)->Unk05 = 0;
        EM10_W(pEm)->Unk04 = EM10_W(pEm)->Walk_no;
        pEm->Super_matrix.t[0] = pP->Workm.t[0];
        pEm->Super_matrix.t[2] = pP->Workm.t[2];
        EM10_W(pEm)->Unk01 = 1;
        EM10_W(pEm)->Unk08 = pEm->Cdir_y;
        EM10_W(pEm)->Unk07 = (Rnd_l() & 0x1F) + 20;
        Kage_work_set(&EM10_W(pEm)->pKage, 0, 0x2BC01F4, 0, pEm->Super_matrix.t);
    case 1:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = EM10_W(pEm)->Unk05;
        Jp.Hokan_flg = EM10_W(pEm)->Unk06;
        Joint_move_p(&Jp, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
        EM10_W(pEm)->Unk04 = Jp.Move_no;
        EM10_W(pEm)->Unk05 = Jp.Move_cnt;
        EM10_W(pEm)->Unk06 = Jp.Hokan_flg;
        Spd.vx = 0;
        Spd.vy = EM10_W(pEm)->Unk08;
        Spd.vz = 0;
        RotMatrix(&Spd, &M);
        Spd.vx = (Rnd_l() & 7) + 10;
        Spd.vy = 0;
        ApplyMatrixSV(&M, &Spd, &Sv);
        Ground = func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1);
        pEm->Super_matrix.t[0] += Sv.vx;
        pEm->Super_matrix.t[2] += Sv.vz;
        if (Ground != (SHORT)func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1)) {
            pEm->Super_matrix.t[0] -= Sv.vx;
            pEm->Super_matrix.t[2] -= Sv.vz;
        }
        MulKage(EM10_W(pEm)->pKage, Ground, EM10_W(pEm)->Unk08, 0);
        if (EM10_W(pEm)->Unk07-- == 0)
            EM10_W(pEm)->Unk01 = 2;
        break;
    case 2:
        EM10_W(pEm)->Unk04 = 2;
        EM10_W(pEm)->Unk05 = 0;
        EM10_W(pEm)->Unk06 = 7;
        EM10_W(pEm)->Unk01 = 3;
    case 3:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = EM10_W(pEm)->Unk05;
        Jp.Hokan_flg = EM10_W(pEm)->Unk06;
        EM10_W(pEm)->Unk01 += Joint_move_p(&Jp, pKan, pSeq, 0x200) & 1;
        EM10_W(pEm)->Unk04 = Jp.Move_no;
        EM10_W(pEm)->Unk05 = Jp.Move_cnt;
        EM10_W(pEm)->Unk06 = Jp.Hokan_flg;
        Spd.vx = 0;
        Spd.vy = EM10_W(pEm)->Unk08;
        Spd.vz = 0;
        RotMatrix(&Spd, &M);
        Spd.vx = 30;
        Spd.vy = 0;
        ApplyMatrixSV(&M, &Spd, &Sv);
        Ground = func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1);
        pEm->Super_matrix.t[0] += Sv.vx;
        pEm->Super_matrix.t[2] += Sv.vz;
        if (Ground != (SHORT)func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1)) {
            pEm->Super_matrix.t[0] -= Sv.vx;
            pEm->Super_matrix.t[2] -= Sv.vz;
        }
        MulKage(EM10_W(pEm)->pKage, Ground, EM10_W(pEm)->Unk08, 0);
        break;
    case 4:
        pEm->pSin_parts_ptr[8].Be_flg |= 2;
        EM10_W(pEm)->Unk01 = 5;
        EM10_W(pEm)->Unk04 = 0x17;
        EM10_W(pEm)->Unk05 = 0;
        EM10_W(pEm)->Unk06 = 7;
        EM10_W(pEm)->Unk07 = (Rnd_l() & 0x1F) + 10;
        pKage = EM10_W(pEm)->pKage;
        *(ULONG *)&pKage->Prim[0].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[0].r0 |= 0xBFBF10;
        *(ULONG *)&pKage->Prim[1].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[1].r0 |= 0xBFBF10;
        if (Rnd_l() & 1) {
            EM10_W(pEm)->Unk01 = 8;
            return;
        }
    case 5:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = EM10_W(pEm)->Unk05;
        Jp.Hokan_flg = EM10_W(pEm)->Unk06;
        EM10_W(pEm)->Unk01 += Joint_move_p(&Jp, pKan, pSeq, 0x200);
        EM10_W(pEm)->Unk04 = Jp.Move_no;
        EM10_W(pEm)->Unk05 = Jp.Move_cnt;
        EM10_W(pEm)->Unk06 = Jp.Hokan_flg;
        break;
    case 6:
        EM10_W(pEm)->Unk07 = (Rnd_l() & 0x1F) + 60;
        EM10_W(pEm)->Unk01 = 7;
        if (!(Rnd_l() & 0xF)) {
            EM10_W(pEm)->Unk01 = 8;
            return;
        }
    case 7:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = 0;
        Jp.Hokan_flg = 0;
        Joint_move_p(&Jp, pKan, pSeq, 0x200);
        if (EM10_W(pEm)->Unk07-- == 0)
            EM10_W(pEm)->Unk01 = 5;
        break;
    case 8:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = 0;
        Jp.Hokan_flg = 0;
        Joint_move_p(&Jp, pKan, pSeq, 0x200);
        break;
    }
    if (EM10_W(pEm)->Unk01 < 4) {
        Tmp = pEm->Cdir_y;
        pEm->Cdir_y = EM10_W(pEm)->Unk08;
        RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
        pEm->Cdir_y = Tmp;
        MulMatrix0((MATRIX *)pEm->Workm_m, &Jp.P0_m, &pEm->Super_matrix);
        pEm->Super_matrix.t[1] = Jp.P0_m.t[1] + EM10_W(pEm)->Unk1A;
    }
    pP = &pEm->pSin_parts_ptr[8];
    pP[0].UnkA2 |= 1;
    pP[1].UnkA2 |= 1;
    pP[2].UnkA2 |= 1;
    pP[3].UnkA2 |= 1;
    pP[4].UnkA2 |= 1;
    pP[5].UnkA2 |= 1;
    pP[6].UnkA2 |= 1;
    if (!Hit_ck_point4(pEm->Super_matrix.t, G.Cccut)) {
        pP[0].UnkA2 &= ~1;
        pP[1].UnkA2 &= ~1;
        pP[2].UnkA2 &= ~1;
        pP[3].UnkA2 &= ~1;
        pP[4].UnkA2 &= ~1;
        pP[5].UnkA2 &= ~1;
        pP[6].UnkA2 &= ~1;
    }
}

#include "enemy/common/em10/Em10_Kubigoro.inc.c"

/* RE2 Em10_ude_down: an arm (parts 2..4 or 5..7) torn off by the player */
void Em10_ude_down(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;

    pP = &pEm->pSin_parts_ptr[(EM10_W(pEm)->Unk25 >> 1) * 3 + 2];
    switch (EM10_W(pEm)->Ude_down) {
    case 1:
        pP->Be_flg |= 0x10;
        pP[2].Be_flg |= 0x10;
        pP->pSuper = &G.Player.pSin_parts_ptr->Workm;
        pP->pOya_parts = G.Player.pSin_parts_ptr;
        MulLMatrix(&G.Player.pSin_parts_ptr->Workm, &pP->Workm, (MATRIX *)pP->M);
        MulLMatrix(&G.Player.pSin_parts_ptr->Workm, &pP[2].Workm, &M);
        EM10_W(pEm)->Unk28 = M.t[0] >> 1;
        EM10_W(pEm)->Unk2A = M.t[1] + 200;
        EM10_W(pEm)->u2C.w.Unk2C = M.t[2] >> 1;
        EM10_W(pEm)->Ude_down = 2;
        EM10_W(pEm)->Unk23 = 90;
        Snd_se_enem(11, pEm);
        if (SquareRoot0(EM10_W(pEm)->Unk28 * EM10_W(pEm)->Unk28 + EM10_W(pEm)->u2C.w.Unk2C * EM10_W(pEm)->u2C.w.Unk2C) > 800) {
            EM10_W(pEm)->Ude_down = 3;
            break;
        }
    case 2:
        if (EM10_W(pEm)->Unk23-- == 0)
            EM10_W(pEm)->Ude_down = 3;
        MulLMatrix(&G.Player.pSin_parts_ptr->Workm, &pP[2].Workm, &M);
        pP->Pos_x += EM10_W(pEm)->Unk28 - M.t[0];
        pP->Pos_y += EM10_W(pEm)->Unk2A - M.t[1];
        pP->Pos_z += EM10_W(pEm)->u2C.w.Unk2C - M.t[2];
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = (Rnd_l() & 7) * (1 - ((EM10_W(pEm)->Unk23 & 1) << 1)) - 12;
        RotMatrix(&Sv, &M);
        MulMatrix((MATRIX *)pP->M, &M);
        if (!(EM10_W(pEm)->Unk23 & 3)) {
            Esp_call((Rnd_l() & 1) << 8, ((Rnd_l() + 0xC18) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, 0, 0);
            Sv.vx = 0;
            Sv.vy = -500;
            Sv.vz = (EM10_W(pEm)->Unk25 >> 1) * 500 - 250;
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (((Rnd_l() & 1) + 0xC18) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, &Sv, 0);
        }
        break;
    case 3:
        pP->Be_flg |= 0xA;
        pP->Old_x = pEm->Cdir_y + 0x800;
        pP->Timer3 = 0;
        pP->Old_z = 0;
        pP->Old_y = -20;
        pP->Timer1 = 30;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = G.Player.Cdir_y + ((~EM10_W(pEm)->Unk25 & 1) << 11);
        pP->Rot_spd_z = -0x400;
        EM10_W(pEm)->Ude_down = 4;
        break;
    case 0:
    case 4:
        break;
    }
}

#include "enemy/common/em10/Em10_mv00_br.inc.c"

#include "enemy/common/em10/Em10_mv00.inc.c"

#include "enemy/common/em10/Em10_mv01_br.inc.c"

#include "enemy/common/em10/Em10_mv01.inc.c"

#include "enemy/common/em10/Em10_mv02_br.inc.c"

#include "enemy/common/em10/Em10_mv02.inc.c"

void func_80104FC0(void) {
}

#include "enemy/common/em10/Em10_mv03.inc.c"

#include "enemy/common/em10/Em10_mv04_br.inc.c"

#include "enemy/common/em10/Em10_mv04.inc.c"

void func_80105C10(void) {
}

/* RE2 Em10_mv05 slot: grabbing the player and letting go */
void func_80105C18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Type_mot[4][2] = { { 12, 12 }, { 15, 15 }, { 12, 12 }, { 15, 15 } }; /* as Em10_mv03 */
    SVECTOR Sv;                        /* unused */
    MATRIX M;                          /* unused */
    UCHAR Atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } }; /* unused */
    ENEMY_WORK *pPl;
    ULONG Flg;

    pPl = G_PLAYER;
    Flg = pEm->Type & 1;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 0xF0000;
        pEm->Routine_2 = 1;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->Routine_0 = 1;
        pPl->Routine_1 = 13;
        pPl->Routine_2 = 0;
        pPl->Damage_cnt |= 0x80;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Damage_cnt &= 0x7F;
    case 1:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 1;
        pEm->Routine_2 = 3;
        pEm->Timer0 = 13;
    case 3:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        if (pEm->Timer0-- == 0)
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        break;
    }
}

void func_80105E64(void) {
}

/* RE2 Em10_mv06: falling down and getting up again */
void Em10_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[3][2] = { { 1, 3 }, { 24, 23 }, { 9, 10 } };
    SVECTOR Sv;
    USHORT Timer[16] = { 100, 30, 200, 160, 10, 40, 150, 50, 150, 60, 35, 110, 30, 180, 50, 300 };
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd_l() & 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[0][pEm->UnkE2];
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Flg |= 0x200;
        EM10_W(pEm)->Flg |= 2;
        pEm->Damage_cnt |= 0x80;
        EM10_W(pEm)->Unk0A = 0;
        pEm->UnkE3 = 0;
        pEm->Type |= 0x2000;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Timer0 = 0;
    case 1:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (!pEm->Hokan_flg || pEm->Timer0) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        pEm->Routine_2 = 3;
        EM_MOVE(pEm) = Mot[1][pEm->UnkE2] + 0xF0000;
        pEm->UnkDA = Timer[(Rnd_l() >> (Rnd_l() & 3)) & 0xF] + (Rnd_l() & 0xF);
        pEm->Damage_cnt &= 0x7F;
        pEm->Spd_x = Rnd_l() & 3;
        pEm->Col.W0 = pEm->Col.D0 = 100;
    case 3:
        if (!pEm->Spd_x)
            pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (pEm->UnkDA-- == 0)
            pEm->Routine_2 = 6;
        break;
    case 4:
        pEm->Timer0 = (Rnd_l() & 0xF) + 3;
        pEm->Routine_2 = 5;
    case 5:
        if (pEm->Timer0-- == 0)
            pEm->Routine_2 = 3;
        if (pEm->UnkDA-- == 0)
            pEm->Routine_2 = 6;
        break;
    case 6:
        EM_MOVE(pEm) = Mot[2][pEm->UnkE2] + 0xF0000;
        pEm->Routine_2 = 7;
        EM10_W(pEm)->Flg |= 0x10;
        EM10_W(pEm)->Unk1C = 1;
        if (EM10_W(pEm)->Flg & 4)
            EM10_W(pEm)->Unk1C = 0;
        if (pEm->UnkE3 == 1 && pEm->Unk12C != 0x1A) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0, (USHORT)pEm->Cdir_y | 0x1F400000, &pP->Workm, &Sv, 0);
        }
        if (!EM10_W(pEm)->Unk21 && !(Rnd_l() & 3)) {
            Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        EM10_W(pEm)->Flg &= 0xFDFF;
        pEm->Unk132 |= 0x8000;
    case 7:
        if (pEm->Move_cnt < 0x37)
            func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM10_W(pEm)->Flg &= 0xFFEF;
            pEm->Routine_2 = 8;
            EM10_W(pEm)->Flg &= 0xFFFD;
        }
        pEm->Col.D0 += 10;
        pEm->Col.W0 += 10;
        if (pEm->Col.D0 > 500)
            pEm->Col.W0 = pEm->Col.D0 = 500;
        if (pEm->Move_cnt == 0x37) {
            pEm->Tilt_spd = 30;
            pEm->Unk132 &= 0x7FFF;
            pEm->Be_flg |= 0x80000;
        }
        break;
    case 8:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Col.W0 = pEm->Col.D0 = 500;
        pEm->Type &= 0xDFFF;
        EM10_W(pEm)->Flg &= 0xFFED;
        break;
    }
}

void func_801063CC(void) {
}

/* RE2 Em10_mv07 slot: lying down, getting up when the player comes near */
void func_801063D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Col.W0 = pEm->Col.D0 = 100;
        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 1;
    case 1:
        if (!(pEm->Type & 0x4000))
            pEm->Routine_2 = 2;
        break;
    case 2:
        if (!pEm->Add_dir) {
            EM_MOVE(pEm) = ((Rnd_l() & 0xF) << 8) + 0xF0009;
            if (EM10_W(pEm)->Flg & 4)
                EM_MOVE(pEm) = ((Rnd_l() & 0xF) << 8) + 0xF000A;
        } else {
            EM_MOVE(pEm) = (pEm->Add_dir << 8) + 0xF0009;
            if (EM10_W(pEm)->Flg & 4)
                EM_MOVE(pEm) = (pEm->Add_dir << 8) + 0xF000A;
        }
        pEm->Routine_2 = 3;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else if (Rnd_l() & 1)
                Snd_se_enem(14, pEm);
            else
                Snd_se_enem(13, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        EM10_W(pEm)->Unk1C = 1;
        EM10_W(pEm)->Flg |= 0x10;
        pEm->Damage_cnt &= 0x7F;
        if (EM10_W(pEm)->Flg & 4)
            EM10_W(pEm)->Unk1C = 0;
        pEm->Unk132 |= 0x8000;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 4;
            EM10_W(pEm)->Flg &= 0xFFEF;
        }
        pEm->Col.D0 += 10;
        pEm->Col.W0 += 10;
        if (pEm->Col.D0 > 500)
            pEm->Col.W0 = pEm->Col.D0 = 500;
        if (pEm->Move_cnt == 0x37)
            pEm->Unk132 &= 0x7FFF;
        break;
    case 4:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Col.W0 = pEm->Col.D0 = 500;
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Flg &= 0xFFEF;
        pEm->Neck_flg &= 0xFE;
        break;
    }
}

void func_80106654(void) {
}

/* RE2 Em10_mv08 slot: eating the dead player */
void func_8010665C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x19, 0, 0xF);
        pEm->Routine_2 = 1;
        Set_base_pos(&Pl_work, pKan, pSeq, 0);
        if (Rnd_l() & 1)
            Snd_se_enem(13, pEm);
        else
            Snd_se_enem(14, pEm);
        pEm->Damage_cnt |= 0x80;
    case 1:
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 8, 0, 0);
        pEm->Type |= 0x4000;
        break;
    }
}

/* RE2 Em10_mv09_br */
void func_80106770(ENEMY_WORK *pEm)
{
    SHORT Dir;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    if (!Dir && LL < 1300 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40)) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
}

void func_80106844(void) {
}

void func_8010684C(void) {
}

/* RE2 Em10_mv0a: eating a corpse */
void Em10_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[8] = { 19, 20, 21, 19, 20, 21, 19, 20 };
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = Mot[Rnd_l() & 7] + 0x70000;
        pEm->Damage_cnt |= 0x80;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            EM_MOVE(pEm) = Mot[Rnd_l() & 7] + 0x300;
        if (!pEm->Hokan_flg) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        if (*pEm->pNow_seq & 0x200) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 200;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x13D00000, &pP[1].Workm, &Sv, 0);
            Snd_se_enem((Rnd_l() & 1) | 4, pEm);
        }
        if (!(pEm->Type & 0x4000))
            pEm->Routine_2 = 3;
        break;
    case 2:
        EM_MOVE(pEm) = Mot[Rnd_l() & 7] + 0x300;
        pEm->Routine_2 = 1;
        break;
    case 3:
        pEm->Routine_2 = 4;
        EM_SET_MOVE(pEm, 0x16, 0, 0xF);
        EM10_W(pEm)->Unk1C = 2;
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Flg |= 0x10;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(14, pEm);
            else if (Rnd_l() & 1)
                Snd_se_enem(13, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 5;
            EM10_W(pEm)->Flg &= 0xFFEF;
        }
        break;
    case 5:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Flg &= 0xFFEF;
        break;
    case 6:
        pEm->Routine_2 = 7;
        EM_MOVE(pEm) = ((Rnd_l() & 7) << 8) + 0xF0016;
    case 7:
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x10100);
        break;
    case 8:
        pEm->Routine_2 = 0;
        break;
    }
}

void func_80106B7C(void) {
}

#include "enemy/common/em10/Em10_mv0b.inc.c"

void func_80106E90(void) {
}

void func_80106E98(void) {
}

#include "enemy/common/em10/Em10_mv0d_br.inc.c"

#include "enemy/common/em10/Em10_mv0d.inc.c"

#include "enemy/common/em10/Em10_mv0e_br.inc.c"

#include "enemy/common/em10/Em10_mv0e.inc.c"

void func_801074E4(void) {
}

/* RE2 Em10_mv0f slot: standing still while the scenario holds it (Type 0x4000) */
void func_801074EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 6, 0, 0xF);
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() & 7) + 7;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x100);
        if (!(pEm->Type & 0x4000))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
}

void func_80107594(void) {
}

#include "enemy/common/em10/Em10_mv10.inc.c"

void func_801076A0(void) {
}

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
        EM_MOVE(pEm) = pEm->Add_dir;
        pEm->Routine_2++;
        EM10_W(pEm)->Unk1C = 0;
        EM10_W(pEm)->Flg |= 0x10;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[0];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[2];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[3];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[6];
        pP->UnkA2 |= 1;
        if (Parts_num == 15) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
        }
    case 2:
        if (pEm->Move_cnt == 0x46) {
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 0x4B) {
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[13];
            pP->UnkA2 |= 1;
            if (Parts_num == 15) {
                pP = &pEm->pSin_parts_ptr[11];
                pP->UnkA2 |= 1;
                pP = &pEm->pSin_parts_ptr[14];
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
            EM10_W(pEm)->Flg &= 0xFFEF;
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

/* RE2 Em10_dm00 (shared with em16 func_801076B8) */
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
    LL = pEm->Unk154;
    if (EM10_W(pEm)->Unk0A == 1) {
        func_801082B0(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        if (pEm->Move_no >= 8)
            EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0x1F0000;
        if (!EM10_W(pEm)->Unk21) {
            if (!(Rnd_l() & 1))
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
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
        EM10_W(pEm)->Unk0B -= Em10_dm00_usonum[pEm->Unk12C - 1];
        if (pEm->Unk132 & 0x10)
            pEm->UnkDA = -1;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        func_8010C57C(pEm);
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -(pEm->Timer0 * pEm->UnkE2 * 8) * pEm->UnkDA;
        Sv.vz = -(pEm->Timer0 * pEm->UnkE3 * 8) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix((MATRIX *)pP->M, &M);
        pS = pEm->pSin_parts_ptr;
        pT = &pS[8];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pT->M);
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = -(pEm->Timer0 << 6) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix((MATRIX *)pT->M, &M);
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pS[9].M);
        MulMatrix2(&M1, (MATRIX *)pEm->pSin_parts_ptr[12].M);
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
        MulMatrix((MATRIX *)pP->M, &M);
        pP = &pEm->pSin_parts_ptr[8];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pP->M);
        if (pEm->Timer0-- == 0) {
            if (pEm->Move_no < 8) {
                if (pEm->Move_no < 3)
                    EM_SET_RTN(pEm, 1, 1, 1, 0);
                else if (pEm->Move_no < 6) {
                    EM_SET_RTN(pEm, 1, 2, 1, 0);
                    if (EM10_W(pEm)->Flg & 0x4000)
                        EM_SET_RTN(pEm, 1, 0xB, 0, 0);
                } else
                    EM_SET_RTN(pEm, 1, 0xB, 1, 0);
                pEm->UnkDA = (Rnd_l() >> 3) + 100;
                pEm->UnkE2 = (Rnd_l() & 0x1F) + 30;
                pEm->UnkE3 = (Rnd_l() & 0xF) * 2;
                pEm->Timer0 = Rnd_l();
                if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x44C) && EM10_W(pEm)->Unk26 < 60)
                    EM_SET_RTN(pEm, 1, 0xC, 0, 0);
                if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400) && EM10_W(pEm)->Unk26 < 60 && LL < 3000 && (Rnd_l() & 1))
                    EM_SET_RTN(pEm, 1, 0xC, 0, 0);
                break;
            }
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x44C) && EM10_W(pEm)->Unk26 < 30)
                EM_SET_RTN(pEm, 1, 0xC, 0, 0);
            if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400) && EM10_W(pEm)->Unk26 < 60 && LL < 3000 && (Rnd_l() & 1))
                EM_SET_RTN(pEm, 1, 0xC, 0, 0);
        }
        if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x320) && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor
            && (LONG)G.Status_flg >= 0 && !((pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40)) {
            EM_SET_RTN(pEm, 1, 3, 0, 0);
            pEm->Damage_cnt |= 0x80;
            pPl->Damage_cnt |= 0x80;
            pPl->Unk12C = 0;
            if (pPl->Be_flg & 0x10000000)
                EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

/* RE2 Em10_dm01 slot: hit, staggering back (may fall over) */
void func_801082B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 0 };
    PARTS_W *pP;
    SVECTOR Sv;                        /* unused (frame size) */
    UCHAR Mot[2] = { 1, 3 };
    SHORT Dir;
    LONG R;

    switch (pEm->Routine_2) {
    case 0:
        Dir = pEm->Unk132 & 0x10;
        EM_MOVE(pEm) = 0x30005 - (Dir = Dir != 0);
        pEm->Routine_2 = 1;
        pEm->UnkDA = 10;
        pEm->Spd_x = Spd[(pEm->Unk132 >> 4) & 1];
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->UnkE2 = 1;
        if (pEm->Unk132 & 0x10)
            pEm->UnkE2 = -1;
        if (pEm->Unk12C == 0x1A)
            pEm->UnkE2 = 0;
        else {
            func_8010C57C(pEm);
            if ((pEm->Unk132 & 3) == 1)
                pEm->UnkE2 = 0;
        }
        pEm->UnkE3 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE3 = 5;
        if (pEm->Unk12C == 0x1A && !(Rnd_l() & 1)) {
            EM10_W(pEm)->Unk0B = 0;
            EM10_W(pEm)->Unk0A = 1;
        }
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Unk12C == 0x1A && pEm->Move_cnt < 10)
            pEm->Spd_x <<= 1;
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkDA && pEm->UnkE2) {
            if (!(pEm->UnkDA & 1)) {
                pP = pEm->pSin_parts_ptr;
                if (!EM10_W(pEm)->Unk0A) {
                    Esp_call((pEm->UnkE3 & 0xF) << 24,
                                  (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x1F400000, &pP->Workm,
                                  (SVECTOR *)&pEm->Spd_x, 0);
                } else {
                    pEm->Spd_y -= 500;
                    Esp_call((pEm->UnkE3 & 0xF) << 24,
                                  (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x13E80000, &pP->Workm,
                                  (SVECTOR *)&pEm->Spd_x, 0);
                }
            }
            pEm->UnkDA--;
        }
        if (pEm->Unk12C == 0x1A && pEm->Move_cnt == 13) {
            R = Rnd_l() & 3;
            if (R == 1 && !(pEm->Status_flg & 0x400)) {
                EM_SET_RTN(pEm, 1, 5, 0, 0);
                EM10_W(pEm)->Unk0B = (Rnd_l() & 0xF) + 0x10;
                EM10_W(pEm)->Flg &= 0xFFF7;
                pEm->UnkE2 = R;
                EM10_W(pEm)->Flg |= 4;
                if (pEm->Unk132 & 0x10) {
                    pEm->UnkE2 = 0;
                    EM10_W(pEm)->Flg &= 0xFFFB;
                }
                EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[pEm->UnkE2];
                pEm->Routine_2 = R;
                EM10_W(pEm)->Unk0A = 0;
                EM10_W(pEm)->Flg |= 0x202;
                pEm->UnkE3 = 0;
                pEm->Damage_cnt |= 0x80;
                pEm->Type |= 0x2000;
                if (!EM10_W(pEm)->Unk21) {
                    if (Rnd_l() & 1)
                        Snd_se_enem(12, pEm);
                    else
                        Snd_se_enem(15, pEm);
                    EM10_W(pEm)->Unk21 = 150;
                }
                pEm->Timer0 = 0;
            }
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Rnd_l() & 1) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (EM10_W(pEm)->Flg & 0x4000)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        if (EM10_W(pEm)->Unk0B <= 0) {
            EM10_W(pEm)->Unk0B = (Rnd_l() & 0x10) + 0xF;
            EM10_W(pEm)->Unk0A = 0;
        }
        if ((LONG)pEm->Unk154 < 3000 && Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 900) && (Rnd_l() & 1))
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
}

/* RE2 Em10_Firedm: set on fire */
void Em10_Firedm(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pQ;

    pEm->Status_flg |= 1;
    if ((pEm->Unk132 & 3) == 1) {
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd_l();
        Sv.vy = 0x1AC - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(0x305, ((Rnd_l() << 4) + 0x1A00) << 16, &pP[8].Workm, &Sv, 0);
        pS = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pS[13].Workm, 0, 0);
        pS = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pS[9].Workm, 0, 0);
    } else {
        pQ = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd_l();
        Sv.vy = 0x1AC - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(5, ((Rnd_l() << 4) + 0x1A00) << 16, &pQ[8].Workm, &Sv, 0);
        pS = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pS[1].Workm, 0, 0);
        pS = pEm->pSin_parts_ptr;
        Esp_call(5, ((Rnd_l() << 4) + 0x800) << 16, &pS[5].Workm, 0, 0);
        pEm->pSin_parts_ptr[1].Poly_rgb = 0x404040;
    }
    pEm->pSin_parts_ptr[8].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[0].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[5].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[2].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[12].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[9].Poly_rgb = 0x606060;
}

/* RE2 Em10_Ryudm slot: acid hit, the head melts */
void func_801089A8(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;

    pEm->Unk178[2] = 0xFFF;
    pEm->Unk17E = 0x230;
    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    pP = pEm->pSin_parts_ptr;
    pEm->Unk178[0] = 0;
    pEm->Unk178[1] = 0;
    Sv.vx = 0x80 - Rnd_l();
    Sv.vy = -0xAC - Rnd_l();
    Sv.vz = 0x80 - Rnd_l();
    Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pP[8].Workm, &Sv, 0);
    if (Rnd_l() & 1) {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pS[9].Workm, 0, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pS[1].Workm, 0, 0);
    }
    pW = &pEm->pSin_parts_ptr[8];
    pW->UnkA2 |= 0x40;
    pW = pEm->pSin_parts_ptr;
    pW->UnkA2 |= 0x40;
}

/* RE2 Em10_Tudm slot: hit by the grenade blast */
void func_80108B00(ENEMY_WORK *pEm)
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
    Esp_call(0x205, ((Rnd_l() << 4) + 0x1A00) << 16, &pP[8].Workm, &Sv, 0);
    pS = pEm->pSin_parts_ptr;
    Esp_call(0x205, ((Rnd_l() << 4) + 0xA00) << 16, &pS[1].Workm, 0, 0);
    Sv.vx = 0x40 - (Rnd_l() & 0x7F);
    Sv.vy = 0;
    Sv.vz = 0x40 - (Rnd_l() & 0x7F);
    if (Rnd_l() & 1) {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x205, ((Rnd_l() << 4) + 0x1400) << 16, &pS[12].Workm, &Sv, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x205, ((Rnd_l() << 4) + 0x1000) << 16, &pS[5].Workm, &Sv, 0);
    }
    pEm->pSin_parts_ptr[8].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[0].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[1].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[5].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[2].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[12].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[9].Poly_rgb = 0x606060;
}

/* RE2 Em10_Spdm slot: acid hit from the front, the head is pushed back */
void func_80108CBC(ENEMY_WORK *pEm)
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
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Unk178);
    pP = pEm->pSin_parts_ptr;
    pEm->Unk17E = 0x40;
    Sv.vx = 0x80 - Rnd_l();
    Sv.vy = -0xAC - Rnd_l();
    Sv.vz = 0x80 - Rnd_l();
    Esp_call(0x1C03, ((Rnd_l() << 3) + 0x800) << 16, &pP[8].Workm, &Sv, 0);
    if (Rnd_l() & 1) {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x1803, ((Rnd_l() << 3) + 0x800) << 16, &pS[9].Workm, 0, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x1803, ((Rnd_l() << 3) + 0x800) << 16, &pS[1].Workm, 0, 0);
    }
    pW = &pEm->pSin_parts_ptr[8];
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

    if (EM10_W(pEm)->Unk19 == 1) {
        func_8010B6D4(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Unk19 == 2) {
        func_8010B278(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        if ((Rnd_l() & 3) && pEm->Routine_0 == 3) {
            EM10_W(pEm)->Unk19 = 1;
            func_8010B6D4(pEm, pKan, pSeq);
            return;
        }
        Dir = pEm->Unk132 & 0x10;
        EM_MOVE(pEm) = 0x30005 - (Dir = Dir != 0);
        pEm->Routine_2 = 1;
        pEm->Add_dir = 0;
        pEm->Unk14A = 0;
        pEm->Unk14C = 0;
        pEm->Timer0 = 0;
        pEm->UnkE3 = 0;
        pEm->Spd_x = Spd[(pEm->Unk132 >> 4) & 1];
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
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd_l() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        Snd_se_enem(11, pEm);
        EM10_W(pEm)->Unk1A = pEm->Unk122;
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
            MulMatrix((MATRIX *)pP->M, &M);
            pS = &pEm->pSin_parts_ptr[8];
            TransposeMatrix(&M, &M1);
            MulMatrix2(&M1, (MATRIX *)pS->M);
        }
        if (pEm->Move_cnt) {
            if (!(pEm->Move_cnt & 3)) {
                pP = pEm->pSin_parts_ptr;
                Esp_call(0, (((Rnd_l() << 3) + 0x1770) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm,
                              (SVECTOR *)&pEm->Spd_x, 0);
            }
        }
        break;
    case 2:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Damage_cnt &= 0x7F;
        pP = pEm->pSin_parts_ptr;
        pP->Old_y = -200;
        pP->Cdir_x = 0;
        pP->Cdir_y = 0;
        pP->Cdir_z = 0x400;
        pEm->Pos_y = EM10_W(pEm)->Unk1A;
        EM10_W(pEm)->Flg |= 1;
        pEm->Life >>= 1;
        if (pEm->Routine_0 == 3)
            pEm->Life = 10;
        pEm->Type = 0x2001;
        pEm->Be_flg &= ~0x400;
        if ((pEm->Type & 0x20) || (pEm->Status_flg & 0x400)) {
            pEm->Life = -1;
            EM_SET_RTN(pEm, 7, 0, 0, 0);
            EM10_W(pEm)->Flg &= 0xFFFB;
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
        MulMatrix((MATRIX *)pP->M, &M);
        pS = &pEm->pSin_parts_ptr[8];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pS->M);
    }
    if (pEm->Move_cnt == 20 && pEm->Routine_2 == 1) {
        pEm->pSin_parts_ptr[8].pSuper = &pEm->Super_matrix;
        pEm->Super_matrix = pEm->pSin_parts_ptr->Workm;
        pEm->Super_matrix.t[0] = pEm->Pos_x;
        pEm->Super_matrix.t[2] = pEm->Pos_z;
        pEm->UnkDA = 10;
        pEm->UnkE2 = 2;
        pP = &pEm->pSin_parts_ptr[8];
        Esp_call(0x300, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Snd_se_enem(11, pEm);
        pP = &pEm->pSin_parts_ptr[8];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[9];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[10];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[12];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[13];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        if (pEm->Parts_num == 15) {
            pP = &pEm->pSin_parts_ptr[11];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[14];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
        }
    }
    if (pEm->Move_cnt >= 20 && pEm->Routine_2 == 1) {
        pP = pEm->pSin_parts_ptr;
        RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
        MulMatrix0((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &pEm->Super_matrix);
        pEm->Super_matrix.t[1] = pP->Pos_y + EM10_W(pEm)->Unk1A;
        if (pEm->UnkE2) {
            pEm->Pos_y += pEm->UnkDA;
            pEm->UnkDA += 5;
            if (pEm->Move_cnt >= 35)
                pEm->UnkDA += 50;
        }
        if (EM10_W(pEm)->Unk1A - (200 + pP->Pos_y) < pEm->Pos_y && pEm->UnkE2) {
            Tmp = pEm->UnkDA;
            pEm->UnkDA = -(Tmp >> 3);
            pEm->Pos_y = EM10_W(pEm)->Unk1A - (300 + pP->Pos_y);
            pEm->UnkE2--;
            Snd_se_enem(6, pEm);
        }
    }
}

void func_80109758(void) {
}

void func_80109760(void) {
}

/* RE2 Em10_die slot: a leg is blown off (the zombie may go on crawling) */
void func_80109768(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 125 };      /* unused */
    SVECTOR Sv;
    PARTS_W *pP;
    ULONG Mode;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0xF);
        Spd_get2(pEm, pKan, pSeq, 0);
        pEm->Move_cnt = Rnd_l() & 7;
        Spd_get2(pEm, pKan, pSeq, 0);
        Mode = 0;
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pEm->UnkE3 = Rnd_l() & 1;
        if (pEm->UnkE3 && !(EM10_W(pEm)->Flg & 0x8000)) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[2] = 0;
            PW_FREE(pP)[1] = -10;
            PW_FREE(pP)[3] = 0;
            PW_FREE(pP)[6] = 0;
            pP->Poly_rgb = 0x10104F;
            pEm->pSin_parts_ptr[11].Be_flg = 0;
            pP = &pEm->pSin_parts_ptr[9];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            Snd_se_enem(2, pEm);
            if (Rnd_l() & 1) {
                if (pEm->Id != 0x11 && pEm->Id != 0x13) {
                    pP = &pEm->pSin_parts_ptr[12];
                    pP->UnkA2 |= 0x20;
                    pP->Be_flg |= 2;
                    PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
                    PW_FREE(pP)[4] = 0;
                    PW_FREE(pP)[2] = 0;
                    PW_FREE(pP)[1] = -10;
                    PW_FREE(pP)[3] = 0;
                    PW_FREE(pP)[6] = 0;
                    pP->Poly_rgb = 0x10104F;
                    Esp_call(Mode << 24, 0x17700000, &pP->Workm, 0, 0);
                    Esp_call(8, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
                }
                pP = &pEm->pSin_parts_ptr[13];
                pP->UnkA2 |= 0x20;
                pP->Be_flg |= 2;
                PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
                PW_FREE(pP)[4] = 0;
                PW_FREE(pP)[2] = 0;
                PW_FREE(pP)[1] = -10;
                PW_FREE(pP)[3] = 0;
                PW_FREE(pP)[6] = 0;
                pP->Poly_rgb = 0x10104F;
                Esp_call(Mode << 24, 0x17700000, &pP->Workm, 0, 0);
                pEm->pSin_parts_ptr[14].Be_flg = 0;
            } else {
                pP = &pEm->pSin_parts_ptr[12];
                PW_FREEL(pP)[0] = 0x40;
                PW_FREEL(pP)[1] = 0x202030;
                pP->UnkA2 |= 4;
                pP = &pEm->pSin_parts_ptr[13];
                PW_FREEL(pP)[0] = 0x40;
                PW_FREEL(pP)[1] = 0x202030;
                pP->UnkA2 |= 4;
            }
        } else {
            pP = &pEm->pSin_parts_ptr[9];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[10];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[12];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[13];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
        }
        pP = &pEm->pSin_parts_ptr[10];
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(Mode << 24, 0x13880000, &pP->Workm, &Sv, 0);
        Sv.vy = 400;
        Esp_call(Mode << 24, (USHORT)(Rnd_l() << 2) | 0x17700000, &pP->Workm, &Sv, 0);
        Sv.vy = 800;
        Esp_call(Mode << 24, (USHORT)(Rnd_l() << 4) | 0xFA00000, &pP->Workm, &Sv, 0);
        Esp_call((Mode << 24) | 8, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        if (!EM10_W(pEm)->Unk21) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
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
                EM10_W(pEm)->Flg &= 0xFFFB;
                EM_SET_RTN(pEm, 7, 0, 0, 0);
                break;
            }
        }
        if ((pEm->Type & 0x20) || (pEm->Status_flg & 0x400)) {
            pEm->Life = -1;
            EM10_W(pEm)->Flg &= 0xFFFB;
            EM_SET_RTN(pEm, 7, 0, 0, 0);
            break;
        }
        pEm->Type = 0x2001;
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        else
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        pEm->Damage_cnt &= 0x7F;
        break;
    }
}

#include "enemy/common/em10/Em10_dm06.inc.c"

/* RE2 Em10_dm07 slot: the head is hit (shakes the head part and goes back to the saved routine) */
void func_80109E54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    SVECTOR Sv;
    MATRIX M, Mt;
    VECTOR V;                          /* unused (frame size) */
    signed char Shake[8] = { 24, -8, -24, 0, -12, 0, 12, 8 };

    switch (pEm->Routine_2) {
    case 0:
        pEm->Tilt_spd = 30;
        pEm->Be_flg |= 0x80000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->UnkDA = 1;
        pEm->UnkE2 = 0;
        if (pEm->Unk132 & 0x10)
            pEm->UnkDA = -1;
        func_8010C57C(pEm);
        pEm->Hokan_flg = 15;
        if ((EM10_W(pEm)->Routine & 0xFF) == 1)
            EM10_W(pEm)->Unk14 = EM10_W(pEm)->Routine;
        if (pEm->Life < 0) {
            pEm->Life = 0;
            pEm->UnkE2 = 1;
            pP = &pEm->pSin_parts_ptr[1];
            Esp_call(0x300, (USHORT)(Rnd_l() << 4) | 0x1F400000, &pP->Workm, &Sv, 0);
            pEm->Hokan_flg = 15;
        }
        pEm->UnkE3 = 12;
        pEm->UnkE2 = Shake[Rnd_l() & 7];
    case 1:
        if (!Joint_move(pEm, pKan, pSeq, 0x100)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = -(pEm->Timer0 * pEm->UnkE2 * 8) * pEm->UnkDA;
            Sv.vz = -(pEm->Timer0 * pEm->UnkE3 * 8) * pEm->UnkDA;
            RotMatrix(&Sv, &M);
            MulMatrix((MATRIX *)pP->M, &M);
            pP = &pEm->pSin_parts_ptr[8];
            TransposeMatrix(&M, &Mt);
            MulMatrix2(&Mt, (MATRIX *)pP->M);
            if (pEm->Timer0++ >= 3) {
                pEm->Routine_2 = 2;
                pEm->Timer0 = 16;
            }
            break;
        } else {
            EM_RTN(pEm) = EM10_W(pEm)->Unk14;
            EM10_W(pEm)->Flg &= 0xFFED;
            pEm->Routine_2++;
        }
        break;
    case 2:
        if (!Joint_move(pEm, pKan, pSeq, 0x100)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = -(pEm->Timer0 * pEm->UnkE2) * pEm->UnkDA;
            Sv.vz = -(pEm->Timer0 * pEm->UnkE3) * pEm->UnkDA;
            RotMatrix(&Sv, &M);
            MulMatrix((MATRIX *)pP->M, &M);
            pP = &pEm->pSin_parts_ptr[8];
            TransposeMatrix(&M, &Mt);
            MulMatrix2(&Mt, (MATRIX *)pP->M);
            if (pEm->Timer0-- == 0)
                pEm->Routine_2 = 3;
            break;
        } else {
            EM_RTN(pEm) = EM10_W(pEm)->Unk14;
            EM10_W(pEm)->Flg &= 0xFFED;
            pEm->Routine_2++;
        }
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_RTN(pEm) = EM10_W(pEm)->Unk14;
            EM10_W(pEm)->Flg &= 0xFFED;
            pEm->Routine_2++;
        }
    }
}

/* RE2 Em10_dm slot: knocked back by a blast */
void func_8010A1D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;                        /* unused */
    UCHAR Mot[8] = { 27, 28, 29, 28, 27, 29, 27, 28 };
    LONG Idx;
    LONG No;
    LONG Mv;

    switch (pEm->Routine_2) {
    case 0:
        Idx = Rnd_l() & 7;
        Mv = Mot[Idx];
        No = pEm->Move_no;
        if (No != Mv)
            EM_MOVE(pEm) = Mv + 0x60000;
        else
            EM_MOVE(pEm) = (ULONG)(No - 26 + (Rnd_l() & 1)) % 3 + 0x6001B;
        pEm->Routine_2 = 1;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        func_8010C57C(pEm);
        if (!EM10_W(pEm)->Unk21) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        pEm->Spd_x = (Rnd_l() & 0x1F) + 60;
        pEm->Spd_z = 0x40 - (Rnd_l() & 0x7F);
        Add_speedXZ(pEm, pEm->Timer0 - pEm->Cdir_y + 0x800);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x10);
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (Rnd_l() & 1)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        break;
    }
}

/* RE2 Em10_haidm slot: hit while crawling, the upper body may burst */
void func_8010A3DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Ofs[2] = { -450, 250 };      /* unused */
    SVECTOR Sv;                        /* unused */
    PARTS_W *pP;
    LONG Mode;
    PARTS_W *pS;

    switch (pEm->Routine_2) {
    case 0:
        Mode = 0;
        EM_MOVE(pEm) = ((Rnd_l() & 7) << 8) + 0x30007;
        pEm->Routine_2 = 1;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pP = pEm->pSin_parts_ptr;
        Esp_call(Mode << 24, 0x17700000, &pP->Workm, 0, 0);
        pS = &pEm->pSin_parts_ptr[1];
        Esp_call((Mode << 24) | 0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pS->Workm, 0, 0);
        if (!EM10_W(pEm)->Unk21) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->UnkE3 = 0;
        if ((!(pEm->Unk132 & 0xC) && (Rnd_l() & 3) == 1) || (pEm->Unk132 & 0x400))
            pEm->UnkE3 = 1;
        if (pEm->UnkE3) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 4;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP = &pEm->pSin_parts_ptr[8];
            pP->Be_flg |= 0x22;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 0x1004;
            pP = &pEm->pSin_parts_ptr[9];
            pP->Be_flg |= 0x22;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[12];
            pP->Be_flg |= 0x22;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[10];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[13];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            if (pEm->Parts_num == 15) {
                pP = &pEm->pSin_parts_ptr[11];
                pP->Be_flg |= 0x20;
                pP->UnkA2 |= 0x1000;
                pP = &pEm->pSin_parts_ptr[14];
                pP->Be_flg |= 0x20;
                pP->UnkA2 |= 0x1000;
            }
        }
    case 1:
        if (pEm->UnkE3 && (pEm->Move_cnt & 1)) {
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        }
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        pEm->Type = 1;
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
}

/* RE2 Em10_die slot: falling over dead (sometimes gets up again as a crawler) */
void func_8010A730(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG T;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd_l() & 1;
        if (EM10_W(pEm)->Flg & 0x2000)
            pEm->UnkE2 = 0;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        EM_MOVE(pEm) = pEm->UnkE2 * 0xA00 + 0xF0000 + Mot[pEm->UnkE2];
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE3 = 5;
        func_8010C57C(pEm);
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        if ((pEm->Unk132 & 3) != 1) {
            T = pEm->UnkE2 * 10;
            if (pEm->Move_cnt >= T + 4 && pEm->Move_cnt <= T + 16 && (pEm->Move_cnt & 1)) {
                pP = &pEm->pSin_parts_ptr[1];
                Sv.vx = pP->Pos_x;
                Sv.vy = pP->Pos_y + 500;
                Sv.vz = pP->Pos_z;
                pP = pEm->pSin_parts_ptr;
                Esp_call((pEm->UnkE3 & 0xF) << 24, (((Rnd_l() << 3) + 0xFD0) << 16) | (USHORT)(Rnd_l() << 4),
                              &pP->Workm, &Sv, 0);
            }
        }
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (!pEm->Hokan_flg) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        if (!(Rnd_l() & 3) && !(EM10_W(pEm)->Flg & 4) && !(pEm->Type & 0x20) && !(pEm->Status_flg & 0x400)
            && !pEm->Water && (LONG)G.Status_flg >= 0) {
            pEm->Life = 1;
            pEm->Type = 0x2001;
            Rnd_l();
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            do {                       /* (a macro in the original? the loop notes keep the load below the store) */
                pEm->Damage_cnt &= 0x7F;
            } while (0);
        }
        break;
    }
}

#include "enemy/common/em10/Em10_die01.inc.c"

/* RE2 Em10_die02 */
void Em10_die02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 125 };
    PARTS_W *pP;
    SVECTOR Sv;
    UCHAR Em10_die02_mot[2] = { 3, 1 };

    if (EM10_W(pEm)->Unk19 == 1) {
        func_8010B6D4(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Life = -1;
        pEm->UnkE3 = (pEm->Unk132 & 0x10) >> 4;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (!pEm->UnkE3)
            EM10_W(pEm)->Flg |= 4;
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
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[2] = 400;
            PW_FREE(pP)[1] = -100;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[3] = 10;
            PW_FREE(pP)[6] = -50;
            pP->Poly_rgb = 0x101030;
        }
        pEm->UnkE2 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE2 = 5;
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        Esp_call((pEm->UnkE2 & 0xF) << 24, (USHORT)(Rnd_l() << 4) | 0xBB80000, &pP->Workm, &Sv, 0);
        Esp_call(8, ((Rnd_l() + 0xFA0) << 18) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
        pP = &pEm->pSin_parts_ptr[2];
        if (!(EM10_W(pEm)->Flg & 0x20)) {
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
        pP = &pEm->pSin_parts_ptr[5];
        if (!(EM10_W(pEm)->Flg & 0x40) && !(Rnd_l() & 1)) {
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
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
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
                              (SVECTOR *)&pEm->Spd_x, 0);
                Esp_call((pEm->UnkE2 & 0xF) << 24,
                              (USHORT)(pEm->Cdir_y + (Rnd_l() << 4) - (Rnd_l() << 4)) | 0x1F400000, &pP->Workm,
                              (SVECTOR *)&pEm->Spd_x, 0);
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
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 4:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    case 5:
        EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0xF0000;
        pEm->Routine_2 = 6;
        pEm->Timer0 = (Rnd_l() & 0x1F) + 15;
    case 6:
        if (pEm->Timer0-- == 0) {
            pEm->Routine_2 = 3;
            EM_SET_MOVE(pEm, 1, 0, 7);
            EM10_W(pEm)->Flg &= 0xFFFB;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
    }
}

/* RE2 Em10_die slot: the head is blown off */
void func_8010B278(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG Mode;
    LONG Dir;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Life = -1;
        pEm->UnkE2 = Rnd_l() & 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        Mode = 0;
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        Esp_call((Mode << 24) | 0x200, (USHORT)pEm->Cdir_y | 0x10000000, &pP->Workm, &Sv, 0);
        Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        Sv.vy -= 400;
        Esp_call(8, (USHORT)Dir | 0x10000000, &pP->Workm, &Sv, 0);
        pEm->Neck_flg = 1;
        if (G.Status_flg & 0x8000) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 4;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
        } else {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_FREE(pP)[0] = Dir + 0x800;
            PW_FREE(pP)[2] = 400;
            PW_FREE(pP)[1] = -100;
            PW_FREE(pP)[3] = 10;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[6] = -50;
            pP->Poly_rgb = 0x101040;
        }
        pP = pEm->pSin_parts_ptr;
        pP->UnkA2 |= 4;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        pP = &pEm->pSin_parts_ptr[2];
        pP->UnkA2 |= 4;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 4;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        Snd_se_enem(2, pEm);
        Em_dead_flg_on_cand(pEm);
        if (Rnd_l() & 1) {
            pEm->Routine_2 = 3;
            break;
        }
        EM_MOVE(pEm) = pEm->UnkE2 * 0xA00 + 0xF0000 + Mot[pEm->UnkE2];
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (!pEm->Hokan_flg) {
            func_8002646C_(pEm, 400, 40, pEm->Col.Unk39);
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
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
            EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0xF0000;
        pEm->Routine_2 = 4;
        pEm->Timer0 = (Rnd_l() & 0x3F) + 30;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
    case 4:
        if (pEm->Timer0-- == 0) {
            pEm->Routine_2 = 1;
            EM_SET_MOVE(pEm, 1, 0, 0xF);
            EM10_W(pEm)->Flg &= 0xFFFB;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_die slot: blown away by a close shot */
void func_8010B6D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG n;
    LONG T;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        pEm->Life = -1;
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        n = (Rnd_l() & 3) + 1;
        pEm->UnkE3 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE3 = 5;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -300;
        Sv.vz = 0;
        Esp_call((pEm->UnkE3 & 0xF) << 24, (USHORT)pEm->Timer0 | 0x1FA00000, &pP->Workm, &Sv, 0);
        Sv.vx = 0;
        Sv.vy = -100;
        Sv.vz = 0;
        Esp_call((pEm->UnkE3 & 0xF) << 24, (USHORT)pEm->Timer0 | 0x17D00000, &pP->Workm, &Sv, 0);
        if (!(pEm->Unk132 & 0xA00)) {
            do {
                n--;
                Sv.vx = 0x100 - (Rnd_l() << 1);
                Sv.vy = 0x38 - (Rnd_l() << 1);
                Sv.vz = 0x100 - (Rnd_l() << 1);
                Esp_call(0x209, (((Rnd_l() << 2) + 0x600) << 16) | (USHORT)(pEm->Timer0 + 0x200 - (Rnd_l() << 2)),
                              &pP->Workm, &Sv, 0);
            } while (n);
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 4;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
        }
        pEm->UnkE2 = ~Cdir_ck(pEm, &Pl_work) & 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        EM_MOVE(pEm) = Mot[pEm->UnkE2] + 0xF0F00;
        pEm->Routine_2 = 1;
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
        pEm->Spd_x = 200;
        pEm->Spd_z = 0;
    case 1:
        T = pEm->UnkE2 * 10;
        if (pEm->Move_cnt >= T + 4 && pEm->Move_cnt <= T + 16 && (pEm->Move_cnt & 1)) {
            pP = &pEm->pSin_parts_ptr[1];
            Sv.vx = pP->Pos_x;
            Sv.vy = pP->Pos_y + 500;
            Sv.vz = pP->Pos_z;
            pP = pEm->pSin_parts_ptr;
            Esp_call((pEm->UnkE3 & 0xF) << 24, (((Rnd_l() << 3) + 0xFD0) << 16) | (USHORT)(Rnd_l() << 4),
                          &pP->Workm, &Sv, 0);
        }
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, pEm->UnkE2 << 11);
        if ((pEm->Spd_x -= 50) < 0)
            pEm->Spd_x = 0;
        if (!pEm->Spd_x) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}

/* RE2 Em10_die slot: falling down dead (Unk1C picks the motion) */
void func_8010BB08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Tbl[3][3] = { { 0x37, 0x17, 0x01 }, { 0x28, 0x18, 0x01 }, { 0x00, 0x01, 0x01 } };
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        EM10_W(pEm)->Flg |= 4;
        if (EM10_W(pEm)->Unk1C == 1)
            EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->Move_cnt < Tbl[EM10_W(pEm)->Unk1C][0]) {
            EM_MOVE(pEm) = Tbl[EM10_W(pEm)->Unk1C][1] + 0x1F0000;
            pEm->UnkE3 = 0;
        } else {
            EM10_W(pEm)->Flg &= 0xFFFB;
            EM_MOVE(pEm) = Tbl[EM10_W(pEm)->Unk1C][2] + 0x1F1E00;
            pEm->UnkE3 = 1;
        }
        pP = &pEm->pSin_parts_ptr[1];
        pEm->Routine_2 = 1;
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0, (USHORT)pEm->Cdir_y | 0x1FA00000, &pP->Workm, &Sv, 0);
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if (!pEm->UnkE3)
                pEm->Routine_2 = 2;
            else
                pEm->Routine_2 = 3;
        }
        break;
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x80);
        if (!pEm->Hokan_flg)
            pEm->Routine_2 = 3;
        break;
    case 3:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}

/* death by explosion: head, arms and legs fly off (die table, damage types 0x14-0x16) */
void func_8010BD60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    UCHAR R;

    if (EM10_W(pEm)->Unk19 == 1) {
        Em10_die02(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
        if (!(Rnd_l() & 1)) {
            EM10_W(pEm)->Unk19 = 1;
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
            pP = &pEm->pSin_parts_ptr[5];
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
        }
        pP = &pEm->pSin_parts_ptr[13];
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
            pP = &pEm->pSin_parts_ptr[8];
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
        EM10_W(pEm)->Flg |= 4;
        func_80108B00(pEm);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -200;
        Sv.vz = 0;
        Esp_call(5, 0x1A000000, &pP->Workm, &Sv, 0);
        Esp_call(0x1B03, 0x10000000, &pP->Workm, &Sv, 0);
        pP = pEm->pSin_parts_ptr;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        pP->UnkA2 |= 4;
    case 1:
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = 0x80;
        Sv.vy = 0;
        Sv.vz = 0;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[5];
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
        pP = &pEm->pSin_parts_ptr[10];
        Sv.vx = 0x40;
        Sv.vy = 0;
        Sv.vz = 0x40;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[13];
        Sv.vx = -0x40;
        Sv.vy = 0;
        Sv.vz = 0x40;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[8];
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

void func_8010C3E8(void) {
}

/* Foot_set_pl for the zombie: keeps foot Foot (0/1, parts 8.. / 10..) on the ground */
void func_8010C3F0(ENEMY_WORK *pEm, LONG Mode, LONG Foot)
{
    MATRIX M;
    SVECTOR Sv = { 0, 500, 0 };
    SVECTOR Sv0;
    SVECTOR Sv1;
    SVECTOR Rot;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
    if (pEm->Be_flg & 0xC0000) {
        Rot.vy = 0;
        Rot.vx = pEm->Tilt[0];
        Rot.vz = pEm->Tilt[1];
        RotMatrix_gte(&Rot, &M);
        MulMatrix2(&M, (MATRIX *)pEm->Workm_m);
    }
    CompM((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &M);
    CompM(&M, (MATRIX *)pP[6].M, &M);
    pP = &pP[Foot * 2 + 8];
    CompM(&M, (MATRIX *)pP[-1].M, &M);
    CompM(&M, (MATRIX *)pP->M, &M);
    ApplyMatrixSV(&M, &Sv, &Sv0);
    ApplyMatrixSV(&pP->Workm, &Sv, &Sv1);
    pEm->Pos_x -= (M.t[0] + Sv0.vx) - (pP->Workm.t[0] + Sv1.vx);
    pEm->Pos_z -= (M.t[2] + Sv0.vz) - (pP->Workm.t[2] + Sv1.vz);
}

/* blood when hit */
void func_8010C57C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pW;
    LONG Mode;

    Mode = 0;
    if (pEm->Unk12C == 0x1A)
        return;
    if (pEm->Unk132 & 0x200)
        Mode = 5;
    if ((pEm->Unk132 & 3) == 1) {
        pW = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = 800;
        Sv.vz = 0;
        Esp_call(Mode << 24, (USHORT)pEm->Cdir_y | 0x17D00000, &pW[8].Workm, &Sv, 0);
    } else {
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x - ((Rnd_l() & 0x7F) - 0x40);
        Sv.vy = pP->Pos_y + 0xE4 - Rnd_l();
        Sv.vz = pP->Pos_z - ((Rnd_l() & 0x7F) - 0x40);
        pP = pEm->pSin_parts_ptr;
        Esp_call(Mode << 24, (((Rnd_l() << 4) + 0x800) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, &Sv, 0);
    }
}

#include "enemy/common/em10/Em10_water.inc.c"
