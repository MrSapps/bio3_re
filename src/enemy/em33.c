#include "enemy/em33.h"
#include "joint.h"
#include "sabun.h"
#include "sca.h"
#include "sld.h"
#include "snd.h"


/* Capcom declared the sabun call as returning a value in this module (Em33_morph_body returns it) */
LONG func_8004AC54_l(ENEMY_WORK *pEm, PARTS_W *pP) __asm__("func_8004AC54");

/* per-frame entry (RE2 EM_MAIN): the head runs its routine; a segment follows its part */
void Em33_main(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ENEMY_WORK **ppWork;
    ENEMY_WORK *pOya;
    PARTS_W *pP;
    SVECTOR Unused0[3];
    MATRIX M;
    MATRIX Unused1;
    VECTOR V;
    SVECTOR Unused2;
    LONG y;

    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (!(pEm->Type & 1)) {
        if (*(SHORT *)&pEm->Routine_0 == 0x601)
            pEm->Col.Id = 0;
        Em33_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        pEm->pEnemy_ptr = pPl;
        Rot_neck_em(pEm, pEm->Cdir_y);
        Em_seq_sound(pEm);
        if (EM30_W(pEm)->Morph_spd)
            Em33_morph_body(pEm);
        if (EM30_W(pEm)->Splash_timer)
            EM30_W(pEm)->Splash_timer--;
        if (EM30_W(pEm)->Timer) {
            EM30_W(pEm)->Timer--;
            Em33_splash(pEm);
        } else if (EM30_W(pEm)->Hole)
            EM30_W(pEm)->Hole = 0;
        pP = pEm->pSin_parts_ptr;
        if (pP->UnkA2 & 1) {
            pEm->Damage_cnt &= 0x7F;
            pEm->Be_flg &= ~0x20;
            pEm->Type &= ~0x4000;
        } else {
            pEm->Damage_cnt |= 0x80;
            pEm->Be_flg |= 0x20;
            pEm->Type |= 0x4000;
        }
        y = pEm->Pos_y;
        if (y < 0) {
            pEm->Col.Unk26 = -y;
            pEm->Col.H = 800 - y;
        } else {
            pEm->Col.Unk26 = 0;
            pEm->Col.H = 800;
        }
    } else {
        ppWork = ((EM33_PL *)pPl)->pWork;
        pOya = ppWork[EM30_SUB(pEm)->Oya_no];
        pP = &pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no];
        if (pP->UnkA2 & 1) {
            pEm->Damage_cnt &= 0x7F;
            pEm->Kage.pKw->Be_flg |= 0x80;
            pEm->Type &= ~0x4000;
        } else {
            pEm->Damage_cnt |= 0x80;
            pEm->Kage.pKw->Be_flg &= 0x7F;
            pEm->Type |= 0x4000;
        }
        if (pEm->Routine_0 == 2)
            Em33_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        if (*(SHORT *)&pOya->Routine_0 == 0x602)
            pEm->Col.Id = 0;
        M = GsIDMATRIX;
        MulMatrix2((&pEm->Col)->pM, &M);
        pEm->Col.Unk26 = 0;
        ApplyMatrix(&M, (SVECTOR *)&pEm->Col.Unk24, &V);
        y = (&pEm->Col)->pM->t[1] + V.vy;
        if (y < 0) {
            pEm->Col.Unk26 = -y;
            pEm->Col.H = 800 - y;
        } else
            pEm->Col.H = 800;
        pEm->Pos_x = (&pEm->Col)->Pos_x;
        pEm->Pos_y = (&pEm->Col)->Pos_y;
        pEm->Pos_z = (&pEm->Col)->Pos_z;
    }
    Em30_dirt_line(pEm);
}

/* Routine_0 0: set up the head, its seven segment works and their shadows */
void Em33_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pB;
    PARTS_W *pP;
    UCHAR i;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    EM30_W(pEm)->u10.Side = 1;
    EM30_W(pEm)->u13.Clip_no = 1;
    pEm->Unk16B = 0;
    pEm->Unk16A = 0;
    pEm->Damage_cnt = 0;
    EM30_W(pEm)->No = 0;
    EM30_W(pEm)->Morph_phase = 0;
    EM30_W(pEm)->Head_up = 0;
    EM30_W(pEm)->Morph_dir = 0;
    EM30_W(pEm)->Heavy_hit = 0;
    EM30_W(pEm)->Under = 0;
    EM30_W(pEm)->Hit_cnt = 0;
    EM30_W(pEm)->Near_cnt = 0;
    EM30_W(pEm)->Pl_type = 0;
    EM30_W(pEm)->Timer = 0;
    EM30_W(pEm)->Die_flg = 0;
    EM30_W(pEm)->Morph_spd = 0x210;
    EM30_W(pEm)->Unk16 = 0;
    EM30_W(pEm)->Fade_cnt = 0;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
    pEm->Be_flg |= 0x8000;
    pEm->Col.pEm = pEm;
    if (pEm->Id == 0x33)
        pEm->Be_flg |= 0x2010;
    else
        pEm->Be_flg |= 0x1000;
    if (pEm->Id == 0x33) {
        pEm->Col.W0 = pEm->Col.D0 = 1200;
        EM30_W(pEm)->Pl_type = 1;
        pEm->Pos_y = 200;
    } else {
        pEm->Col.W0 = pEm->Col.D0 = 1100;
        pEm->Status_flg |= 0x100;
    }
    pEm->Col.H = 800;
    pEm->Col.Unk24 = 400;
    pEm->Col.Unk28 = 0;
    pEm->Col.Unk26 = 0;
    pEm->Col.Flg = 0;
    pEm->Col.Type = 2;
    func_80033580(&pEm->Col, 0);
    pEm->Unk134 = 1800;
    pEm->Unk136 = -1400;
    pEm->Unk138 = 0x78;
    pEm->Unk139 = 0xB4;
    pEm->Unk13A = 1000;
    pEm->Neck_no = 8;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    EM30_W(pEm)->Col_id = pEm->Col.Id |= 0x80;
    {
        PARTS_W *pS = pEm->pSin_parts_ptr;

        pEm->pUnk13C = (UCHAR *)&pS->Workm;
        pP = &pS[8];
    }
    PW_NECK(pP)->Add_dir_y = 0x40;
    PW_NECK(pP)->L_rot_y = 0x300;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    PW_NECK(pP)->Add_dir_z = 0;
    PW_NECK(pP)->L_rot_z = 0;
    pEm->Pos_x = -4000;
    pEm->Pos_z = -19000;
    pEm->Be_flg |= 0xC00000;
    pEm->Cdir_y = 0x800;
    pEm->Be_flg |= 0x2000;
    if (G.System_flg & 0x100)
        EM30_W(pEm)->Old_life = pEm->Max_life = pEm->Life = 400;
    else
        EM30_W(pEm)->Old_life = pEm->Max_life = pEm->Life = 500;
    EM_SET_MOVE(pEm, 0, 0, 0);
    pEm->Max_life += 100;
    pEm->Life += 100;
    Joint_move(pEm, pKan, pSeq, 0x400);
    pB = Em30_work_set(pEm);
    pB->Be_flg |= 0xC00010;
    Em33_sub_init(pB, 1, pEm, 8);
    Kage_work_set(&pB->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[8].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = Em30_work_set(pEm);
    pB->Be_flg |= 0xC00010;
    Em33_sub_init(pB, 2, pEm, 1);
    Kage_work_set(&pB->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[1].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = Em30_work_set(pEm);
    pB->Be_flg |= 0xC00010;
    Em33_sub_init(pB, 3, pEm, 2);
    Kage_work_set(&pB->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[2].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = Em30_work_set(pEm);
    pB->Be_flg |= 0xC00010;
    Em33_sub_init(pB, 4, pEm, 3);
    Kage_work_set(&pB->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = Em30_work_set(pEm);
    pB->Be_flg |= 0xC00010;
    Em33_sub_init(pB, 5, pEm, 4);
    Kage_work_set(&pB->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = Em30_work_set(pEm);
    pB->Be_flg |= 0xC00010;
    Em33_sub_init(pB, 6, pEm, 5);
    Kage_work_set(&pB->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[5].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = Em30_work_set(pEm);
    pB->Be_flg |= 0xC00010;
    Em33_sub_init(pB, 7, pEm, 6);
    Kage_work_set(&pB->Kage.pKw, 0, 0x3E803E8, 0, (VECTOR *)pEm->pSin_parts_ptr[6].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    i = 0;
    do {
        pP = &pEm->pSin_parts_ptr[i];
        pP->Be_flg |= 0x20;
    } while (++i < 17);
    if (pEm->Id == 0x33)
        pEm->Type = 2;
    EM30_W(pEm)->Se_timer = 0;
    EM30_W(pEm)->Splash_timer = 0;
    pEm->Add_dir = 0;
    pP = &pEm->pSin_parts_ptr[17];
    pP->UnkA2 &= ~1;
}

/* set up segment work No following part Parts of the head pOya */
void Em33_sub_init(ENEMY_WORK *pEm, LONG No, ENEMY_WORK *pOya, LONG Parts)
{
    COL_OBJ *pCol = &pEm->Col;

    pEm->Type = 1;
    pEm->Unk16B = 0;
    pEm->Unk16A = 0;
    EM30_W(pEm)->Heavy_hit = 0;
    EM30_W(pEm)->u13.Clip_no = 0;
    EM30_W(pEm)->No = No;
    pCol->pEm = pEm;
    pCol->W0 = pCol->D0 = Em33_col_size[Parts];
    pCol->H = 800;
    if (Parts == 8)
        pCol->Unk24 = 600;
    else
        pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    func_80033580(pCol, EM30_W(pOya)->Col_id);
    pCol->pM = &pOya->pSin_parts_ptr[Parts].Workm;
    pEm->pUnk13C = (UCHAR *)&pOya->pSin_parts_ptr[Parts].Workm;
    pEm->Unk134 = 1800;
    pEm->Unk136 = Parts == 8 ? -1800 : -1400;
    pEm->Unk138 = 0x78;
    pEm->Unk139 = 0xB4;
    pEm->Unk13A = Parts == 8 ? 1400 : 1000;
    pEm->Unk140 = 0;
    EM30_W(pEm)->Old_life = pEm->Max_life = pEm->Life = 30000;
    EM30_SUB(pEm)->Parts_no = Parts;
    EM30_SUB(pEm)->Oya_no = func_800336B4(pOya);
    pOya->pSin_parts_ptr[Parts].Unk82 = func_800336B4(pEm);
}

/* Routine_0 1 */
void Em30_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em33_Move_type(pEm, pKan, pSeq);
}

/* brain routine, then motion routine, by Routine_1 */
void Em33_Move_type(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em33_mv_br[pEm->Routine_1](pEm);
    Em33_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* move 0, 2, 5: wait for flag 31, then come out */
void Em33_wait_br(ENEMY_WORK *pEm)
{
    if (Flg_ck(&G.Unk7888, 31)) {
        if (++pEm->Timer0 > 30) {
            pEm->Timer0 = 180;
            pEm->Routine_1 = 1;
            EM30_W(pEm)->Hole = 1;
            EM30_W(pEm)->Timer = 100;
            pEm->Be_flg &= ~0x10;
            Em33_vib_rise();
        }
    }
}

/* mv[0], mv[2], mv[5] */
void Em33_nop(ENEMY_WORK *pEm)
{
}

#include "enemy/common/em30/Em30_body_chain.inc.c"

#include "enemy/common/em30/Em30_part_hang.inc.c"

#include "enemy/common/em30/Em30_part_hang_sink.inc.c"

/* sink the body: shift the parts sideways and re-chain them from the tail */
void Em33_body_sink(ENEMY_WORK *pEm)
{
    SVECTOR Unused; /* frame size */
    PARTS_W *pP;

    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[6];
        LONG X;

        EM30_PW(pS)->Roll_z = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM30_W(pEm)->u10.Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[5];
        LONG X;

        EM30_PW(pS)->Roll_z = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM30_W(pEm)->u10.Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[4];
        LONG X;

        EM30_PW(pS)->Roll_z = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM30_W(pEm)->u10.Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[3];
        LONG X;

        EM30_PW(pS)->Roll_z = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM30_W(pEm)->u10.Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[2];
        LONG X;

        EM30_PW(pS)->Roll_z = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM30_W(pEm)->u10.Side ? X + 400 : X - 400;
    }
    pP = &pEm->pSin_parts_ptr[1];
    EM30_PW(pP)->Roll_z = pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[0];
    EM30_PW(pP)->Roll_z = pP->Cdir_z;
    Em30_part_hang_sink(pEm, &pEm->pSin_parts_ptr[5], &pEm->pSin_parts_ptr[6], 0x1000);
    Em30_part_hang_sink(pEm, &pEm->pSin_parts_ptr[4], &pEm->pSin_parts_ptr[5], 0x1000);
    Em30_part_hang_sink(pEm, &pEm->pSin_parts_ptr[3], &pEm->pSin_parts_ptr[4], 0x1000);
    Em30_part_hang_sink(pEm, &pEm->pSin_parts_ptr[2], &pEm->pSin_parts_ptr[3], 0x1000);
    Em30_part_hang_sink(pEm, &pEm->pSin_parts_ptr[1], &pEm->pSin_parts_ptr[2], 0x1000);
    Em30_part_hang_sink(pEm, &pEm->pSin_parts_ptr[0], &pEm->pSin_parts_ptr[1], 0x1000);
}

#include "enemy/common/em30/Em30_work_set.inc.c"

/* cycle the morph keys 0..2 of parts 1-3 (Morph_phase). Returns the last sabun call's value:
   that explicit v0 use is what makes reload put the /50 mfhi in a3 (see NOTES.md). */
LONG Em33_morph_body(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG Step;

    Step = EM30_W(pEm)->Morph_spd * pEm->Spd_x / 50;
    pP = &pEm->pSin_parts_ptr[1];
    if (Step < 0)
        Step = 0;
    switch (EM30_W(pEm)->Morph_phase) {
    case 0:
        EM30_SABUN_P(&pP[0])->Key[0].Rate += Step;
        EM30_SABUN_P(&pP[1])->Key[0].Rate += Step;
        EM30_SABUN_P(&pP[2])->Key[0].Rate += Step;
        if (EM30_SABUN_P(&pP[0])->Key[0].Rate > 0xC00) {
            EM30_SABUN_P(&pP[0])->Key[0].Rate = 0xC00;
            EM30_SABUN_P(&pP[1])->Key[0].Rate = 0xC00;
            EM30_SABUN_P(&pP[2])->Key[0].Rate = 0xC00;
            EM30_W(pEm)->Morph_phase = 1;
        }
        break;
    case 1:
        EM30_SABUN_P(&pP[0])->Key[0].Rate -= Step;
        EM30_SABUN_P(&pP[1])->Key[0].Rate -= Step;
        EM30_SABUN_P(&pP[2])->Key[0].Rate -= Step;
        if (EM30_SABUN_P(&pP[0])->Key[0].Rate < 0) {
            EM30_SABUN_P(&pP[0])->Key[0].Rate = 0;
            EM30_SABUN_P(&pP[1])->Key[0].Rate = 0;
            EM30_SABUN_P(&pP[2])->Key[0].Rate = 0;
        }
        EM30_SABUN_P(&pP[0])->Key[1].Rate += Step;
        EM30_SABUN_P(&pP[1])->Key[1].Rate += Step;
        EM30_SABUN_P(&pP[2])->Key[1].Rate += Step;
        if (EM30_SABUN_P(&pP[0])->Key[1].Rate > 0xC00) {
            EM30_SABUN_P(&pP[0])->Key[1].Rate = 0xC00;
            EM30_SABUN_P(&pP[1])->Key[1].Rate = 0xC00;
            EM30_SABUN_P(&pP[2])->Key[1].Rate = 0xC00;
            EM30_W(pEm)->Morph_phase = 2;
        }
        break;
    case 2:
        EM30_SABUN_P(&pP[0])->Key[1].Rate -= Step;
        EM30_SABUN_P(&pP[1])->Key[1].Rate -= Step;
        EM30_SABUN_P(&pP[2])->Key[1].Rate -= Step;
        if (EM30_SABUN_P(&pP[0])->Key[1].Rate < 0) {
            EM30_SABUN_P(&pP[0])->Key[1].Rate = 0;
            EM30_SABUN_P(&pP[1])->Key[1].Rate = 0;
            EM30_SABUN_P(&pP[2])->Key[1].Rate = 0;
        }
        EM30_SABUN_P(&pP[0])->Key[2].Rate += Step;
        EM30_SABUN_P(&pP[1])->Key[2].Rate += Step;
        EM30_SABUN_P(&pP[2])->Key[2].Rate += Step;
        if (EM30_SABUN_P(&pP[0])->Key[2].Rate > 0xC00) {
            EM30_SABUN_P(&pP[0])->Key[2].Rate = 0xC00;
            EM30_SABUN_P(&pP[1])->Key[2].Rate = 0xC00;
            EM30_SABUN_P(&pP[2])->Key[2].Rate = 0xC00;
            EM30_W(pEm)->Morph_phase = 3;
        }
        break;
    case 3:
        EM30_SABUN_P(&pP[0])->Key[2].Rate -= Step;
        EM30_SABUN_P(&pP[1])->Key[2].Rate -= Step;
        EM30_SABUN_P(&pP[2])->Key[2].Rate -= Step;
        if (EM30_SABUN_P(&pP[0])->Key[2].Rate < 0) {
            EM30_SABUN_P(&pP[0])->Key[2].Rate = 0;
            EM30_SABUN_P(&pP[1])->Key[2].Rate = 0;
            EM30_SABUN_P(&pP[2])->Key[2].Rate = 0;
            EM30_W(pEm)->Morph_phase = 0;
        }
        break;
    }
    func_8004AC54(pEm, &pP[0]);
    func_8004AC54(pEm, &pP[1]);
    return func_8004AC54_l(pEm, &pP[2]);
}

#include "enemy/common/em30/Em30_morph_step.inc.c"

/* fade a body part in: Cnt counts down from 0x40, 8 steps per part (parts 3 and 5 skipped) */
void Em33_fade_in(ENEMY_WORK *pEm, LONG Cnt)
{
    PARTS_W *pP;
    LONG Fade;
    SHORT Lv;
    ULONG Rgb;

    EM30_W(pEm)->Fade_cnt--;
    Fade = 0x40 - Cnt;
    Cnt = Fade >> 3;
    if (Cnt == 3 || Cnt == 5)
        return;
    Lv = Fade & 7;
    switch (Cnt) {
    case 0:
        pP = &pEm->pSin_parts_ptr[8];
        break;
    case 1:
        pP = &pEm->pSin_parts_ptr[1];
        break;
    case 2:
        pP = &pEm->pSin_parts_ptr[2];
        break;
    case 4:
        pP = &pEm->pSin_parts_ptr[3];
        break;
    case 6:
        pP = &pEm->pSin_parts_ptr[4];
        break;
    case 7:
        pP = &pEm->pSin_parts_ptr[5];
        break;
    case 8:
        pP = &pEm->pSin_parts_ptr[6];
        break;
    }
    Lv = (Lv << 4) + 0x10;
    Rgb = (Lv << 16) | (Lv << 8) | Lv;
    pP->Poly_rgb = Rgb;
    if (Rgb == 0x101010)
        pP->UnkA2 |= 1;
    if (Cnt == 0) {
        pP = &pEm->pSin_parts_ptr[0];
        pP->Poly_rgb = Rgb;
        pEm->pSin_parts_ptr[9].Poly_rgb = Rgb;
        pEm->pSin_parts_ptr[7].Poly_rgb = Rgb;
        pEm->pSin_parts_ptr[10].Poly_rgb = Rgb;
        pEm->pSin_parts_ptr[12].Poly_rgb = Rgb;
        pEm->pSin_parts_ptr[14].Poly_rgb = Rgb;
        pEm->pSin_parts_ptr[16].Poly_rgb = Rgb;
        if (Rgb == 0x101010) {
            pP = &pEm->pSin_parts_ptr[0];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 |= 1;
        }
    }
}

/* mv_br[1]: fade the parts in, open or close the mouth */
void Em33_rise_br(ENEMY_WORK *pEm)
{
    if (EM30_W(pEm)->Fade_cnt)
        Em33_fade_in(pEm, EM30_W(pEm)->Fade_cnt);
    Em30_morph_step(pEm, 0x400, EM30_W(pEm)->Morph_dir);
}

/* mv[1]: rise out of the water (Near_cnt == 0) or dive back (steps 2, 3) */
void Em33_rise(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    VECTOR Pos;
    LONG i;
    LONG No;
    ULONG Rgb;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 0x1F);
        EM30_W(pEm)->Spd = 10;
        EM30_W(pEm)->Spd_lim = 20;
        EM30_W(pEm)->Spd_add = 2;
        pEm->Routine_2 = 1;
        pEm->Spd_z = 0;
        pEm->Timer0 = 0;
        pEm->Cdir_y = (EM30_W(pEm)->u10.Side & 1) << 11;
        if (EM30_W(pEm)->Near_cnt == 0) {
            EM30_W(pEm)->Fade_cnt = 0x40;
            pEm->Damage_cnt &= 0x7F;
        }
        Pos.vx = pEm->Pos_x;
        Pos.vy = pEm->Pos_y;
        Pos.vz = pEm->Pos_z;
        Snd_se_stad(0x10206, &Pos, 0, 0);
    case 1:
        if (EM30_W(pEm)->Near_cnt == 0) {
            pP = pEm->pSin_parts_ptr;
            i = 0;
            if (pEm->Move_cnt == 0) {
                do {
                    i++;
                    pP->Be_flg &= ~2;
                    pP++;
                } while (i < pEm->Parts_num);
            }
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if (EM30_W(pEm)->Near_cnt == 0) {
                pEm->Unk120 &= ~1;
                EM30_PW(pP)->Roll_z = 0;
                EM30_PW(pP)->Unk76 = 0;
                EM_SET_RTN(pEm, 1, 9, 0, 0);
            } else
                pEm->Routine_2++;
        }
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
        if (EM30_W(pEm)->Spd_add > 0 && EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim)
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
        if (pEm->Move_cnt != 1)
            Em30_body_chain(pEm);
        if (EM30_W(pEm)->Se_timer == 0) {
            Snd_se_enem(9, pEm);
            EM30_W(pEm)->Se_timer = 25;
        } else
            EM30_W(pEm)->Se_timer--;
        break;
    case 2:
        EM30_W(pEm)->Timer = 60;
        pEm->Timer0 = 0;
        pEm->Routine_2++;
        pEm->Be_flg |= 0x2000;
        pEm->Unk120 |= 1;
        EM30_W(pEm)->u10.Side ^= 1;
    case 3:
        if (++pEm->Timer0 > 150) {
            EM30_W(pEm)->Near_cnt = 0;
            EM30_W(pEm)->Pl_type = 1;
            if (EM30_W(pEm)->Die_flg) {
                EM_SET_RTN(pEm, 1, 6, 0, 0);
                pEm->Be_flg |= 0x10;
            } else
                EM_SET_RTN(pEm, 1, 7, 0, 0);
        }
        if (pEm->Timer0 < 0x40) {
            No = pEm->Timer0 >> 3;
            i = pEm->Timer0 & 7;
            switch (No) {
            case 0:
                pP = &pEm->pSin_parts_ptr[8];
                break;
            case 1:
                pP = &pEm->pSin_parts_ptr[0];
                break;
            case 2:
                pP = &pEm->pSin_parts_ptr[1];
                break;
            case 3:
                pP = &pEm->pSin_parts_ptr[2];
                break;
            case 4:
                pP = &pEm->pSin_parts_ptr[3];
                break;
            case 5:
                pP = &pEm->pSin_parts_ptr[4];
                break;
            case 6:
                pP = &pEm->pSin_parts_ptr[5];
                break;
            case 7:
                pP = &pEm->pSin_parts_ptr[6];
                break;
            }
            i = 0x80 - (i << 4);
            pP->Poly_rgb = (i << 16) | (i << 8) | i;
            if (pP->Poly_rgb == 0x101010) {
                pP->UnkA2 &= ~1;
                if (No == 7)
                    pEm->Damage_cnt |= 0x80;
            }
            if (No == 0) {
                Rgb = (i << 16) | (i << 8) | i;
                pEm->pSin_parts_ptr[9].Poly_rgb = Rgb;
                pEm->pSin_parts_ptr[7].Poly_rgb = Rgb;
                pEm->pSin_parts_ptr[10].Poly_rgb = Rgb;
                pEm->pSin_parts_ptr[12].Poly_rgb = Rgb;
                pEm->pSin_parts_ptr[14].Poly_rgb = Rgb;
                pEm->pSin_parts_ptr[16].Poly_rgb = Rgb;
                if (Rgb == 0x101010) {
                    pP = &pEm->pSin_parts_ptr[9];
                    pP->UnkA2 &= ~1;
                    pP = &pEm->pSin_parts_ptr[7];
                    pP->UnkA2 &= ~1;
                    pP = &pEm->pSin_parts_ptr[10];
                    pP->UnkA2 &= ~1;
                    pP = &pEm->pSin_parts_ptr[12];
                    pP->UnkA2 &= ~1;
                    pP = &pEm->pSin_parts_ptr[14];
                    pP->UnkA2 &= ~1;
                    pP = &pEm->pSin_parts_ptr[16];
                    pP->UnkA2 &= ~1;
                }
            }
            if (EM30_W(pEm)->Se_timer == 0) {
                Snd_se_enem(9, pEm);
                EM30_W(pEm)->Se_timer = 25;
            } else
                EM30_W(pEm)->Se_timer--;
        }
        break;
    }
}

/* mv_br[3]: pick the destination from the player's zone */
void Em33_swim_br(ENEMY_WORK *pEm)
{
    VECTOR Pos;
    SHORT Zone[4];

    Zone[2] = 2400;
    Zone[3] = 4400;
    Zone[0] = -23650;
    Zone[1] = -16000;
    Pos.vx = Pl_work.Pos_x;
    Pos.vz = Pl_work.Pos_z;
    if (func_800101C8(&Pos.vx, Zone)) {
        if (!(EM30_W(pEm)->u10.Side & 1))
            pEm->Dest_x = -18500;
        else
            pEm->Dest_x = -25500;
        pEm->Dest_z = -16500;
    }
    Zone[0] = -11100;
    Zone[1] = -25800;
    if (func_800101C8(&Pos.vx, Zone)) {
        if (!(EM30_W(pEm)->u10.Side & 1))
            pEm->Dest_x = -6500;
        else
            pEm->Dest_x = -13500;
        pEm->Dest_z = -21500;
    }
    if (EM30_W(pEm)->Se_timer == 0) {
        Snd_se_enem(9, pEm);
        EM30_W(pEm)->Se_timer = 25;
    } else
        EM30_W(pEm)->Se_timer--;
    if (EM30_W(pEm)->Fade_cnt)
        Em33_fade_in(pEm, EM30_W(pEm)->Fade_cnt);
}

/* mv[3]: swim to Dest, splashing where the fins (parts 10, 12) cut the water */
void Em33_swim(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Spd;
    VECTOR Pos;
    PARTS_W *pP;
    LONG d;

    switch (pEm->Routine_2) {
    case 0:
        d = pEm->Pos_x - pEm->Dest_x;
        if (d < 0)
            d = -d;
        if (d < 800)
            pEm->Routine_2 = 1;
        Joint_move(pEm, pKan, pSeq, 0x80);
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        break;
    case 1:
        EM_SET_MOVE(pEm, 3, 0, 0x1F);
        pEm->Routine_2 = 2;
        EM30_W(pEm)->Spd = 10;
        EM30_W(pEm)->Spd_lim = 40;
        EM30_W(pEm)->Spd_add = 1;
        pEm->Spd_z = 0;
        pEm->Be_flg &= ~0x2000;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
        if (EM30_W(pEm)->Spd_add > 0 && EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim)
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
        if (EM30_W(pEm)->Splash_timer == 0) {
            Spd.vx = -280;
            Spd.vz = 100;
            Spd.vy = 0;
            pP = &pEm->pSin_parts_ptr[10];
            Pos.vx = pP->Workm.t[0];
            Pos.vz = pP->Workm.t[2];
            if (func_8004D720(&Pos.vx, 0, 0x8000, 0)) {
                EM30_W(pEm)->Splash_timer = 6;
                Esp_call(0x16, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[10].Workm, &Spd, 0);
            }
            pP = &pEm->pSin_parts_ptr[12];
            Pos.vx = pP->Workm.t[0];
            Pos.vz = pP->Workm.t[2];
            if (func_8004D720(&Pos.vx, 0, 0x8000, 0)) {
                EM30_W(pEm)->Splash_timer = 6;
                Esp_call(0x16, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[12].Workm, &Spd, 0);
            }
        }
        break;
    default:
        return;
    }
    Em30_body_chain(pEm);
}

/* mv_br[4]: pick the destination (UnkDA: east or west side) from the player's zone */
void Em33_mv04_br(ENEMY_WORK *pEm)
{
    VECTOR Pos;
    SHORT Zone[4];

    Zone[2] = 2400;
    Zone[3] = 4400;
    Zone[0] = -23650;
    Zone[1] = -16000;
    Pos.vx = Pl_work.Pos_x;
    Pos.vz = Pl_work.Pos_z;
    if (func_800101C8(&Pos.vx, Zone)) {
        pEm->Dest_x = -22400;
        pEm->Dest_z = -16500;
        pEm->UnkDA = 0;
    }
    Zone[0] = -11100;
    Zone[1] = -25300;
    if (func_800101C8(&Pos.vx, Zone)) {
        pEm->Dest_x = -10000;
        pEm->Dest_z = -21500;
        pEm->UnkDA = 1;
    }
    if (EM30_W(pEm)->Se_timer == 0) {
        Snd_se_enem(9, pEm);
        EM30_W(pEm)->Se_timer = 25;
    } else
        EM30_W(pEm)->Se_timer--;
}

INCLUDE_ASM("asm/nonmatchings/enemy/em33", Em33_mv04);

/* splashes: around the hole while Hole is set, and where the body goes through the wall (Clip_no) */
void Em33_splash(ENEMY_WORK *pEm)
{
    SVECTOR Sv;

    Sv.vx = -6500;
    if (EM30_W(pEm)->Hole) {
        if (EM30_W(pEm)->Timer == 99) {
            Sv.vy = -2000;
            Sv.vz = -18400;
            Esp_call(0x41, 0x10000000, &GsIDMATRIX, &Sv, 0);
            Sv.vy = -1800;
            Sv.vz = -18800;
            Esp_call(0x141, 0xE000000, &GsIDMATRIX, &Sv, 0);
        }
        if (EM30_W(pEm)->Timer == 97) {
            Sv.vy = -1200;
            Sv.vz = -18000;
            Esp_call(0x141, 0xC000000, &GsIDMATRIX, &Sv, 0);
            Sv.vy = -1400;
            Sv.vz = -19400;
            Esp_call(0x41, 0x10000000, &GsIDMATRIX, &Sv, 0);
        }
        if (EM30_W(pEm)->Timer == 92) {
            Sv.vy = -1700;
            Sv.vz = -19000;
            Esp_call(0x241, 0x10000000, &GsIDMATRIX, &Sv, 0);
        }
        if (EM30_W(pEm)->Timer % 3 == 0) {
            Sv.vy = -((Rnd() & 7) * 300);
            Sv.vz = -((Rnd() & 7) * 200) - 18000;
            Esp_call(0x341, 0x10000000, &GsIDMATRIX, &Sv, 0);
        }
    }
    if (EM30_W(pEm)->u13.Clip_no == 4)
        return;
    switch (EM30_W(pEm)->u13.Clip_no) {
    case 0:
        Sv.vx = -26500;
        Sv.vy = -((Rnd() & 7) * 300);
        Sv.vz = -((Rnd() & 7) * 200) - 18000;
        break;
    case 1:
        Sv.vx = -6500;
        Sv.vy = -((Rnd() & 7) * 400);
        Sv.vz = -((Rnd() & 7) * 200) - 18000;
        break;
    case 2:
        Sv.vx = -((Rnd() & 7) * 200) - 21500;
        Sv.vy = -((Rnd() & 7) * 500);
        Sv.vz = -12420;
        break;
    case 3:
        Sv.vx = -((Rnd() & 7) * 200) - 9000;
        Sv.vy = -((Rnd() & 7) * 500);
        Sv.vz = -25950;
        break;
    }
    Esp_call(0x4B03, ((Rnd() * 4 + 0x400) & 0xFFFF) | 0x18000000, &GsIDMATRIX, &Sv, 0);
}

/* no brain step for this move */
void Em33_dead_br(ENEMY_WORK *pEm)
{
}

/* mv[6]: dead: wait and remove the work */
void Em33_dead(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Routine_2++;
    case 1:
        if (++pEm->Timer0 > 70) {
            Em_dead_flg_on_cand(pEm);
            pEm->Routine_2++;
        }
        break;
    case 2:
        break;
    }
}

/* show every part at full colour but parts 3..6 */
void Em33_parts_reset(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;

    do {
        pP->UnkA2 |= 1;
        pP->Poly_rgb = 0x808080;
        if (pP->Be_flg & 2)
            pP->Be_flg &= ~2;
        pP++;
    } while (++i < pEm->Parts_num - 1);
    i = 0;
    pP = &pEm->pSin_parts_ptr[3];
    do {
        pP->UnkA2 &= ~1;
        pP->Poly_rgb = 0x808080;
        pP++;
    } while (++i < 4);
}

/* hide every part, colour 0 */
void Em33_parts_hide(ENEMY_WORK *pEm)
{
    LONG i = 0;
    PARTS_W *pP = pEm->pSin_parts_ptr;

    do {
        pP->UnkA2 &= ~1;
        pP->Poly_rgb = 0;
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* mv_br[7]: water noise, break off (Die_flg or 80 damage since the dive), follow the player out west */
void Em33_mv07_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Se_pos;
    VECTOR Pos;
    SHORT Zone[4];

    if (pEm->Routine_2 == 1) {
        if (EM30_W(pEm)->Se_timer == 0) {
            Se_pos.vx = pPl->Pos_x + (Rnd() & 0x7F) - 0x40;
            Se_pos.vz = -10000;
            Se_pos.vy = -500;
            Em30_se_set(9, pEm, &Se_pos.vx);
            EM30_W(pEm)->Se_timer = 25;
        } else
            EM30_W(pEm)->Se_timer--;
    }
    if (pEm->Routine_2 < 2 && EM30_W(pEm)->Die_flg)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    if (pEm->Routine_2 == 4) {
        if (EM30_W(pEm)->Dive_life - pEm->Life > 80 || EM30_W(pEm)->Die_flg) {
            EM30_W(pEm)->Spd = -10;
            EM30_W(pEm)->Spd_lim = -10;
            EM30_W(pEm)->Spd_add = 0;
            Snd_se_enem(3, pEm);
            EM_SET_MOVE(pEm, 8, 0, 0x1F);
            G.Status_flg |= 4;
            EM30_W(pEm)->Timer = 60;
            pEm->Routine_2 = 6;
        }
        if (EM30_W(pEm)->u13.Clip_no == 0 && pEm->Pos_x < -25500) {
            Zone[0] = -23650;
            Zone[1] = -16000;
            Zone[2] = 2400;
            Zone[3] = 4400;
            Pos.vx = pPl->Pos_x;
            Pos.vz = pPl->Pos_z;
            if (func_800101C8(&Pos.vx, Zone)) {
                EM30_W(pEm)->Spd = 150;
                EM30_W(pEm)->Spd_lim = 150;
                EM30_W(pEm)->Spd_add = 10;
                G.Status_flg &= ~2;
                EM_SET_RTN(pEm, 1, 4, 0, 0);
            }
        }
    }
}

/* Side = zone of the room the player is in (0..3, 4 = none); with Flg, pick the hole to come out of */
void Em33_zone_set(ENEMY_WORK *pEm, LONG Flg)
{
    ENEMY_WORK *pPl = G_PLAYER;
    VECTOR Pos;
    SHORT Zone[4];

    Pos.vx = pPl->Pos_x;
    Pos.vz = pPl->Pos_z;
    EM30_W(pEm)->u10.Side = 4;
    Zone[2] = 6500;
    Zone[3] = 2770;
    Zone[0] = -27420;
    Zone[1] = -20300;
    if (func_800101C8(&Pos.vx, Zone))
        EM30_W(pEm)->u10.Side = 0;
    Zone[0] = -12000;
    Zone[1] = -20090;
    if (func_800101C8(&Pos.vx, Zone))
        EM30_W(pEm)->u10.Side = 1;
    if (Flg_ck(&G.Unk7888, 0) && Flg_ck(&G.Unk7888, 1)) {
        Zone[2] = 2400;
        Zone[3] = 4400;
        Zone[0] = -23650;
        Zone[1] = -17200;
    } else {
        Zone[2] = 2400;
        Zone[3] = 7500;
        Zone[0] = -23650;
        Zone[1] = -20300;
    }
    if (func_800101C8(&Pos.vx, Zone))
        EM30_W(pEm)->u10.Side = 2;
    Zone[2] = 2400;
    Zone[3] = 7500;
    Zone[0] = -11100;
    Zone[1] = -24800;
    if (func_800101C8(&Pos.vx, Zone))
        EM30_W(pEm)->u10.Side = 3;
    if (Flg) {
        EM30_W(pEm)->Dive_life = pEm->Life;
        if (Flg_ck(&G.Unk7888, 0)) {
            if (Flg_ck(&G.Unk7888, 1)) {
                if (EM30_W(pEm)->u10.Side == 2 || EM30_W(pEm)->u10.Side == 4)
                    EM30_W(pEm)->u10.Side = 0;
            } else if (EM30_W(pEm)->u10.Side == 0 || EM30_W(pEm)->u10.Side == 4)
                EM30_W(pEm)->u10.Side = 2;
        } else if (Flg_ck(&G.Unk7888, 1)) {
            if (EM30_W(pEm)->u10.Side == 1 || EM30_W(pEm)->u10.Side == 4)
                EM30_W(pEm)->u10.Side = 3;
        } else if (EM30_W(pEm)->u10.Side < 2)
            EM30_W(pEm)->u10.Side += 2;
    }
}

/* is the player still in the zone of Side */
LONG Em33_zone_ck(ENEMY_WORK *pEm)
{
    VECTOR Pos;
    SHORT Zone[4];

    Pos.vx = Pl_work.Pos_x;
    Pos.vz = Pl_work.Pos_z;
    switch (EM30_W(pEm)->u10.Side) {
    case 0:
        Zone[2] = 6500;
        Zone[3] = 2740;
        Zone[0] = -27420;
        Zone[1] = -20300;
        return func_800101C8(&Pos.vx, Zone);
    case 1:
        Zone[2] = 6500;
        Zone[3] = 2740;
        Zone[0] = -12000;
        Zone[1] = -20000;
        return func_800101C8(&Pos.vx, Zone);
    case 2:
        Zone[2] = 2400;
        Zone[3] = 4400;
        Zone[0] = -23650;
        Zone[1] = -17200;
        return func_800101C8(&Pos.vx, Zone);
    case 3:
        Zone[2] = 2400;
        Zone[3] = 4400;
        Zone[0] = -11100;
        Zone[1] = -24800;
        return func_800101C8(&Pos.vx, Zone);
    }
}

INCLUDE_ASM("asm/nonmatchings/enemy/em33", Em33_mv07);

/* mv_br[8]: break off (Die_flg or 80 damage since the dive) */
void Em33_leap_br(ENEMY_WORK *pEm)
{
    if (pEm->Routine_2 < 2 && EM30_W(pEm)->Die_flg)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    if (pEm->Routine_2 == 3 && (EM30_W(pEm)->Dive_life - pEm->Life > 80 || EM30_W(pEm)->Die_flg)) {
        EM30_W(pEm)->Spd = -10;
        EM30_W(pEm)->Spd_lim = -10;
        EM30_W(pEm)->Spd_add = 0;
        EM_SET_MOVE(pEm, 0xC, 0, 0x1F);
        Snd_se_enem(3, pEm);
        EM30_W(pEm)->Timer = 60;
        pEm->Routine_2 = 5;
        pEm->Unk120 |= 1;
    }
}

INCLUDE_RODATA("asm/nonmatchings/enemy/em33", Em33_spd_z_tbl);

/* mv[8]: leap out of the water, bite the player and dive again */
void Em33_leap(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    HIT_AREA Area;
    EM33_SPD_TBL Spd_z = Em33_spd_z_tbl;
    ENEMY_WORK *pPl = &Pl_work;
    LONG n = 3;

    switch (pEm->Routine_2) {
    case 0:
        if (G.System_flg & 0x100)
            pEm->Timer0 = (Rnd() & 0xF) + 45;
        else
            pEm->Timer0 = (Rnd() & 0x1F) + 15;
        pEm->Routine_2++;
    case 1:
        if (--pEm->Timer0)
            break;
        Em33_zone_set(pEm, 1);
        if (EM30_W(pEm)->u10.Side == 4)
            EM30_W(pEm)->u10.Side = Rnd() & 3;
        if (EM30_W(pEm)->u10.Side != 1) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
            return;
        }
        switch (EM30_W(pEm)->u10.Side & 1) {
        case 0:
            pEm->Pos_x = -28400;
            pEm->Pos_y = 200;
            pEm->Pos_z = -19000;
            pEm->Cdir_y = 0;
            break;
        case 1:
            pEm->Pos_x = -3000;
            pEm->Pos_y = 200;
            pEm->Pos_z = -19000;
            pEm->Cdir_y = 0x800;
            break;
        }
        EM_SET_MOVE(pEm, 0xA, 0, 0x1F);
        EM30_W(pEm)->Morph_dir = 1;
        EM30_W(pEm)->Spd = 10;
        pEm->Routine_2++;
        pEm->Timer0 = 0;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        EM30_W(pEm)->Spd_lim = 60;
        EM30_W(pEm)->Spd_add = 4;
        pEm->Col.D0 = 2000;
        pEm->Col.W0 = 2000;
        {
            ENEMY_WORK *pW = G.pEnemy[pEm->pSin_parts_ptr[8].Unk82];
        pW->Col.D0 = 2000;
        pW->Col.W0 = 2000;
        }
        EM30_W(pEm)->Timer = 60;
        Snd_se_enem(4, pEm);
        G.Status_flg |= 2;
        Em33_vib_leap();
        break;
    case 2:
        EM30_W(pEm)->u13.Clip_no = EM30_W(pEm)->u10.Side;
        do {
            if (Joint_move(pEm, pKan, pSeq, 0x80)) {
                n = 1;
                EM_SET_MOVE(pEm, 0xB, 0, 0x1F);
                EM30_W(pEm)->Spd = -10;
                EM30_W(pEm)->Spd_lim = -70;
                EM30_W(pEm)->Spd_add = -10;
                pEm->Spd_z = 0;
                pEm->UnkE2 = 0;
                pEm->UnkE3 = 0;
                pEm->Routine_2++;
            }
            Em30_morph_step(pEm, 0x80, EM30_W(pEm)->Morph_dir);
            pEm->Spd_x = EM30_W(pEm)->Spd;
            pEm->Spd_z = Spd_z.Spd[pEm->Move_cnt / 10];
            Add_speedXZ(pEm, 0);
            EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
            if (EM30_W(pEm)->Spd_add > 0 && EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim)
                EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
            if (EM30_W(pEm)->Spd_add < 0 && EM30_W(pEm)->Spd < EM30_W(pEm)->Spd_lim)
                EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
            if (pEm->Move_cnt == 10)
                G.Status_flg &= ~2;
        } while (--n);
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_MOVE(pEm, 0xA, 0, 0x1F);
            EM30_W(pEm)->Spd = -70;
            EM30_W(pEm)->Spd_lim = -200;
            EM30_W(pEm)->Spd_add = -1;
            EM30_W(pEm)->Timer = 60;
            pEm->Routine_2++;
            if (EM30_W(pEm)->Die_flg)
                Snd_se_enem(5, pEm);
        }
        if ((*pEm->pNow_seq >> 10) & 1) {
            Sv.vx = 1800;
            Sv.vy = 0;
            Sv.vz = 1200;
            Sv.pad = 1200;
            Area.pPos = (VECTOR *)pEm->pSin_parts_ptr[8].Workm.t;
            Area.pBox = &Sv.vx;
            Area.Dir = pEm->Cdir_y;
            Area.Id = 0x85;
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
            EM30_W(pEm)->Spd -= EM30_W(pEm)->Spd_add;
            if (EM30_W(pEm)->Spd < 30)
                EM30_W(pEm)->Spd = 30;
        }
        pEm->Spd_z = 0;
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        if ((*pEm->pNow_seq >> 9) & 1) {
            Sv.vx = 2000;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 1200;
            if (!pPl->Damage_cnt && func_80044420(&pEm->pSin_parts_ptr[8].Workm, &Sv, pPl) && !pEm->UnkE2) {
                Snd_se_enem(1, pEm);
                Pad_vib_set0(7, 0);
                Pad_vib_set1(3, 250, 4);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                if (Pl_life_down(30, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
                pEm->UnkE2 = 1;
                pEm->UnkE3 = -100;
            }
        }
        if (pEm->Move_cnt == 24 && !pEm->UnkE2)
            Snd_se_enem(0, pEm);
        if (pEm->Move_cnt == 30) {
            EM30_W(pEm)->Spd = -70;
            EM30_W(pEm)->Spd_lim = 350;
            EM30_W(pEm)->Spd_add = 80;
        }
        if (pEm->Move_cnt == 36) {
            EM30_W(pEm)->Spd = 350;
            EM30_W(pEm)->Spd_lim = -70;
            EM30_W(pEm)->Spd_add = -80;
        }
        EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
        if (EM30_W(pEm)->Spd_add > 0 && EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim)
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
        if (EM30_W(pEm)->Spd_add < 0 && EM30_W(pEm)->Spd < EM30_W(pEm)->Spd_lim)
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
        break;
    case 4:
        pEm->Col.D0 = 1200;
        pEm->Col.W0 = 1200;
        {
            ENEMY_WORK *pW = G.pEnemy[pEm->pSin_parts_ptr[8].Unk82];
        pW->Col.D0 = 1400;
        pW->Col.W0 = 1400;
        }
        do {
            if (Joint_move(pEm, pKan, pSeq, 0x10080)) {
                n = 1;
                EM30_W(pEm)->u13.Clip_no = 4;
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            }
            pEm->Spd_z = 0;
            pEm->Spd_x = EM30_W(pEm)->Spd;
            Add_speedXZ(pEm, 0);
            EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
            if (EM30_W(pEm)->Spd < EM30_W(pEm)->Spd_lim)
                EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
            Em30_morph_step(pEm, 0x80, EM30_W(pEm)->Morph_dir);
            if (!EM30_W(pEm)->Die_flg && (pEm->Move_cnt == 2 || pEm->Move_cnt == 26))
                Snd_se_enem(3, pEm);
        } while (--n);
        break;
    case 5:
        pEm->Col.D0 = 1200;
        pEm->Col.W0 = 1200;
        {
            ENEMY_WORK *pW = G.pEnemy[pEm->pSin_parts_ptr[8].Unk82];
        pW->Col.D0 = 1400;
        pW->Col.W0 = 1400;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM30_W(pEm)->Under = 1;
            EM_SET_MOVE(pEm, 0xA, 0, 0x1F);
            pEm->Unk120 = 0;
            EM30_W(pEm)->Spd = -10;
            EM30_W(pEm)->Spd_lim = -70;
            EM30_W(pEm)->Spd_add = -10;
            pEm->Spd_z = 0;
            pEm->UnkE2 = 0;
            pEm->UnkE3 = 0;
            pEm->Routine_2--;
        }
        pEm->Spd_z = 0;
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
        if (EM30_W(pEm)->Spd < EM30_W(pEm)->Spd_lim)
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
        Em30_morph_step(pEm, 0x80, EM30_W(pEm)->Morph_dir);
        break;
    }
    Em33_clip(pEm);
    if (pEm->UnkE2 && pEm->UnkE3) {
        Sv.vx = Rnd() & 0x3F;
        Sv.vy = 150;
        Sv.vz = 0;
        if (pEm->UnkE3 == -100)
            Esp_call(0x700, (USHORT)pPl->Cdir_y | 0x14000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, 0);
        Esp_call(0, (USHORT)pPl->Cdir_y | 0x18000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, 0);
        pPl->Spd_x = pEm->UnkE3;
        pPl->Spd_z = 0;
        Add_speedXZ(pPl, 0);
        pEm->UnkE3 += 10;
    }
}

/* no brain step for this move */
void Em33_mv09_br(ENEMY_WORK *pEm)
{
}

INCLUDE_ASM("asm/nonmatchings/enemy/em33", Em33_mv09);

/* no brain step for this move */
void Em33_thrash_br(ENEMY_WORK *pEm)
{
}

/* mv[10]: thrash for a random time at double speed, then back off */
void Em33_thrash(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xD, 0, 0xF);
        pEm->UnkDA = pEm->Timer0 = (Rnd() + 15) & 7;
        pEm->UnkE2 = 1;
        pEm->Unk120 |= 2;
        pEm->Routine_2++;
    case 1:
        if (pEm->UnkE2) {
            if (pEm->Timer0) {
                pEm->Timer0--;
                Joint_move(pEm, pKan, pSeq, 0x10080);
                Joint_move(pEm, pKan, pSeq, 0x10080);
            } else {
                pEm->UnkE2 = 0;
                pEm->Unk120 &= ~2;
                pEm->Move_cnt = 0x76 - pEm->UnkDA * 2;
            }
        } else if (Joint_move(pEm, pKan, pSeq, 0x80) || Joint_move(pEm, pKan, pSeq, 0x80)) {
            pEm->UnkE2 = 0;
            pEm->UnkDA = 0;
            EM_SET_MOVE(pEm, 9, 0x18, 0x1F);
            EM_SET_RTN(pEm, 1, 7, 4, 0);
        }
        pEm->Spd_x = -50;
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* hide the parts that went past x -29000 (into the wall), fade the ones close to it */
void Em33_clip_wall(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;
    LONG x;
    LONG d;

    do {
        x = pP->Workm.t[0];
        if (x < -29000)
            pP->UnkA2 &= ~1;
        else {
            pP->Poly_rgb = 0x808080;
            d = x + 29000;
            pP->UnkA2 |= 1;
            if (d < 0x100) {
                pP->Poly_rgb = (d >> 1) | (((d >> 1) << 16) | ((d >> 1) << 8));
            }
        }
        if (i < 9 && i != 7)
            Em33_part_col_set(pEm, i);
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* hide the parts inside the wall the body goes through (Em33_clip_tbl[Clip_no]) */
void Em33_clip(ENEMY_WORK *pEm)
{
    Em33_clip_tbl[EM30_W(pEm)->u13.Clip_no](pEm);
}

/* as Em33_clip_wall while sinking: part 7 follows part 8 */
void Em33_clip_x_min(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;
    LONG x;
    LONG d;

    do {
        if (i != 7) {
            x = pP->Workm.t[0];
            if (x < -29000) {
                pP->UnkA2 &= ~1;
                if (i == 8)
                    pP[-1].UnkA2 &= ~1;
            } else {
                pP->Poly_rgb = 0x808080;
                pP->UnkA2 |= 1;
                if (i == 8) {
                    pP[-1].Poly_rgb = 0x808080;
                    pP[-1].UnkA2 |= 1;
                }
                x += 29000;
                if (x < 0x100) {
                    d = x >> 1;
                    d |= (d << 16) | (d << 8);
                    pP->Poly_rgb = d;
                    if (i == 8)
                        pP[-1].Poly_rgb = d;
                }
            }
            pP->Be_flg &= ~2;
            if (i == 8)
                pP[-1].Be_flg &= ~2;
            if (i < 9 && i != 7)
                Em33_part_col_set(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* as Em33_clip_x_min for the parts that came out of the ground (x below -903) */
void Em33_clip_x_max(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;
    LONG x;
    LONG d;

    do {
        if (i != 7) {
            x = pP->Workm.t[0];
            if (x >= -903) {
                pP->UnkA2 &= ~1;
                if (i == 8)
                    pP[-1].UnkA2 &= ~1;
            } else {
                pP->Poly_rgb = 0x808080;
                pP->UnkA2 |= 1;
                if (i == 8) {
                    pP[-1].Poly_rgb = 0x808080;
                    pP[-1].UnkA2 |= 1;
                }
                x = -904 - x;
                if (x < 0x1000) {
                    d = x >> 6;
                    d |= (d << 16) | (d << 8);
                    pP->Poly_rgb = d;
                    if (i == 8)
                        pP[-1].Poly_rgb = d;
                }
            }
            pP->Be_flg &= ~2;
            if (i < 9 && i != 7)
                Em33_part_col_set(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* as Em33_clip_x_max on z: the parts past z -8952 */
void Em33_clip_z_max(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;
    LONG z;
    LONG d;

    do {
        if (i != 7) {
            z = pP->Workm.t[2];
            if (z >= -8951)
                pP->UnkA2 &= ~1;
            else {
                pP->Poly_rgb = 0x808080;
                pP->UnkA2 |= 1;
                if (i == 8) {
                    pP[-1].Poly_rgb = 0x808080;
                    pP[-1].UnkA2 |= 1;
                }
                z = -8952 - z;
                if (z < 0x800) {
                    d = z >> 4;
                    d |= (d << 16) | (d << 8);
                    pP->Poly_rgb = d;
                    if (i == 8)
                        pP[-1].Poly_rgb = d;
                }
            }
            pP->Be_flg &= ~2;
            if (i < 9)
                Em33_part_col_set(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* as Em33_clip_x_min on z: the parts past z -28024 */
void Em33_clip_z_min(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;
    LONG z;
    LONG d;

    do {
        if (i != 7) {
            z = pP->Workm.t[2];
            if (z < -28024) {
                pP->UnkA2 &= ~1;
                if (i == 8)
                    pP[-1].UnkA2 &= ~1;
            } else {
                pP->Poly_rgb = 0x808080;
                pP->UnkA2 |= 1;
                if (i == 8) {
                    pP[-1].Poly_rgb = 0x808080;
                    pP[-1].UnkA2 |= 1;
                }
                z += 28024;
                if (z < 0x400) {
                    d = z >> 3;
                    d |= (d << 16) | (d << 8);
                    pP->Poly_rgb = d;
                    if (i == 8)
                        pP[-1].Poly_rgb = d;
                }
            }
            pP->Be_flg &= ~2;
            if (i < 9 && i != 7)
                Em33_part_col_set(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* every part visible */
void Em33_clip_none(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;

    do {
        pP->UnkA2 &= ~1;
        pP->Be_flg &= ~2;
        pP->Poly_rgb = 0x808080;
        if (i < 9 && i != 7)
            Em33_part_col_set(pEm, i);
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* collision of part No follows its visibility */
void Em33_part_col_set(ENEMY_WORK *pEm, LONG No)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    ENEMY_WORK *pW;

    pP += No;
    if (No)
        pW = G.pEnemy[pP->Unk82];
    else
        pW = pEm;
    if (pP->UnkA2 & 1) {
        pP->UnkA2 &= ~0x400;
        pW->Kage.pKw->Be_flg |= 0x80;
        pW->Col.Id = EM30_W(pEm)->Col_id;
        pW->Type &= ~0x4000;
        if (No == 0)
            pW->Be_flg &= ~0x20;
    } else {
        pP->UnkA2 |= 0x400;
        pW->Kage.pKw->Be_flg &= 0x7F;
        pW->Col.Id = Pl_work.Col.Id;
        pW->Type |= 0x4000;
        if (No == 0)
            pW->Be_flg |= 0x20;
    }
}

#include "enemy/common/em30/Em30_Damage.inc.c"

/* dm[0..28]: ordinary hit: the head slows down; a segment passes the damage on to the head */
void Em33_dm_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pH;
    LONG Dmg;

    if (!(pEm->Type & 1)) {
        EM30_W(pEm)->Hit_cnt++;
        if (EM30_W(pEm)->Spd > 10)
            EM30_W(pEm)->Spd -= 10;
        EM_RTN(pEm) = pEm->Unk1C4;
        Em30_Move(pEm, pKan, pSeq);
        Snd_se_enem(2, pEm);
    } else {
        pH = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (EM30_SUB(pEm)->Parts_no == 8) {
            if (EM30_W(pH)->Morph_dir) {
                EM30_W(pH)->Hit_cnt += 5;
                if (!pH->Damage_cnt) {
                    Dmg = EM30_W(pEm)->Old_life - pEm->Life;
                    pH->Damage_cnt = pEm->Damage_cnt;
                    pH->Life -= Dmg;
                }
            } else if (!pH->Damage_cnt) {
                Dmg = EM30_W(pEm)->Old_life - pEm->Life;
                pH->Damage_cnt = pEm->Damage_cnt;
                pH->Life -= Dmg;
            }
        } else if (!pH->Damage_cnt) {
            Dmg = EM30_W(pEm)->Old_life - pEm->Life;
            pH->Damage_cnt = pEm->Damage_cnt;
            pH->Life -= Dmg * 2 / 3;
        }
        if (pH->Life < 0)
            EM30_W(pH)->Die_flg = 1;
        EM30_W(pH)->Hit_cnt++;
        Snd_se_enem(2, pH);
        EM_RTN(pEm) = pEm->Unk1C4;
    }
    if (pEm->Unk12C == 5) {
        pEm->Unk16A = 1;
        EM30_W(pEm)->Old_life = pEm->Life;
    } else
        Em33_blood_set(pEm);
}

/* dm[31]: dive away (a segment hands the routine to the head) */
void Em33_dm_dive(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Unused[4] = { 0x12, 0x13, 0x14, 0x15 };
    UCHAR Move[2] = { 7, 8 };
    ENEMY_WORK *pH;

    if (pEm->Type & 1) {
        pH = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        pH->Unk12C = pEm->Unk12C;
        if (pH->Routine_0 == 1)
            pH->Unk1C4 = EM_RTN(pH);
        EM_RTN(pH) = EM_RTN(pEm);
        EM_RTN(pEm) = pEm->Unk1C4;
        Em33_blood_set(pEm);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Move[EM30_W(pEm)->Morph_dir] + 0x1F0000;
        Em33_blood_set(pEm);
        EM30_W(pEm)->Spd = 40;
        EM30_W(pEm)->Spd_lim = 80;
        EM30_W(pEm)->Spd_add = 5;
        pEm->Spd_z = 0;
        Snd_se_enem(3, pEm);
        pEm->Routine_2++;
    case 1:
        if (pEm->Move_cnt == 3 || pEm->Move_cnt == 9)
            Em33_blood_set(pEm);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            EM_RTN(pEm) = EM30_W(pEm)->Rtn_save;
        Em33_body_sink(pEm);
        break;
    }
}

/* dm[29][0] (Unk12C 30, Unk132 bit 2): the first such hit takes Life / 3, the second kills */
void Em33_dm_heavy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pH;

    if (pEm->Type & 1) {
        pH = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (!Flg_ck(&G.Unk7888, 0) && EM30_W(pH)->Under != 1) {
            pH->Unk12C = pEm->Unk12C;
            pH->Unk132 = pEm->Unk132;
            pH->Damage_cnt = 10;
            pH->Unk1C4 = EM_RTN(pH);
            EM_RTN(pH) = EM_RTN(pEm);
            Em30_body_number(pEm);
            Flg_on(&G.Unk7888, 0);
        }
        pEm->Damage_cnt = 10;
        EM_RTN(pEm) = pEm->Unk1C4;
        return;
    }
    if (EM30_W(pEm)->Under != 1) {
        if (EM30_W(pEm)->Heavy_hit == 1) {
            Em30_dm_fire(pEm, pKan, pSeq);
            return;
        }
        EM30_W(pEm)->Heavy_hit = 1;
        pEm->Damage_cnt = 10;
        pEm->Life /= 3;
        Em30_body_number(pEm);
        Flg_on(&G.Unk7888, 0);
    }
    EM_RTN(pEm) = pEm->Unk1C4;
    Em30_Move(pEm, pKan, pSeq);
}

#include "enemy/common/em30/Em30_dm_fire.inc.c"

/* blood: more drops the bigger the life loss since the last call */
void Em33_blood_set(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pH;
    SVECTOR Sv;
    LONG Dmg;
    LONG n;
    LONG Kind;

    Dmg = EM30_W(pEm)->Old_life - pEm->Life;
    EM30_W(pEm)->Old_life = pEm->Life;
    n = 2;
    if (Dmg > 10)
        n = 4;
    if (Dmg > 40)
        n += 2;
    if (Dmg > 100)
        n += 2;
    if (pEm->Type & 1) {
        pH = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        while (n--) {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = (Rnd() & 0xFF) - 0x7F;
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vy <<= 2;
            Sv.vz <<= 2;
            Kind = (Rnd() & 3) * 0x300 + 0x1200;
            Esp_call(0x7000700, (Kind << 16) | (USHORT)pH->Cdir_y,
                          &pH->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm, &Sv, 0);
        }
    } else {
        while (n--) {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = (Rnd() & 0xFF) - 0x7F;
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vy <<= 2;
            Sv.vz <<= 2;
            Kind = (Rnd() & 3) * 0x300 + 0x1200;
            Esp_call(0x7000700, (Kind << 16) | (USHORT)pEm->Cdir_y,
                          &pEm->pSin_parts_ptr[8].Workm, &Sv, 0);
        }
    }
}

#include "enemy/common/em30/Em30_dirt_hit.inc.c"

#include "enemy/common/em30/Em30_dirt_line.inc.c"

#include "enemy/common/em30/Em30_body_number.inc.c"

#include "enemy/common/em30/Em30_dirt_steps.inc.c"

#include "enemy/common/em30/Em30_dirt_set.inc.c"

/* Routine_0 3: flag the death for the move routines */
void Em33_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM30_W(pEm)->Die_flg = 1;
    EM_RTN(pEm) = pEm->Unk1C4;
    Em30_Move(pEm, pKan, pSeq);
}
#include "enemy/common/em30/Em30_se_set.inc.c"

/* pad vibration scripts */
void Em33_vib_rise(void)
{
    func_8003884C(Em33_vib_tbl, 3);
}

void Em33_vib_leap(void)
{
    func_8003884C(Em33_vib_tbl, 5);
}
