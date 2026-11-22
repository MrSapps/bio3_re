#include "ovl/em33.h"
#include "joint.h"
#include "sabun.h"
#include "sca.h"
#include "sld.h"
#include "snd.h"


LONG func_8004AC54_l(ENEMY_WORK *pEm, PARTS_W *pP) __asm__("func_8004AC54"); /* int-returning alias */

/* EM_MAIN */
void func_801000D4(ENEMY_WORK *pEm)
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
        if (EM33_W(pEm)->Unk14)
            func_80101454(pEm);
        if (EM33_W(pEm)->Unk22)
            EM33_W(pEm)->Unk22--;
        if (EM33_W(pEm)->Unk24) {
            EM33_W(pEm)->Unk24--;
            func_80102D30(pEm);
        } else if (EM33_W(pEm)->Unk19)
            EM33_W(pEm)->Unk19 = 0;
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
        pOya = ppWork[EM33_B(pEm)->Oya];
        pP = &pOya->pSin_parts_ptr[EM33_B(pEm)->Parts];
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
    func_80106668(pEm);
}

/* Em33_init: Routine_0 0 */
void func_80100424(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pB;
    PARTS_W *pP;
    UCHAR i;

    EM_RTN(pEm) = 1;
    EM33_W(pEm)->Side = 1;
    EM33_W(pEm)->Rtn = 1;
    pEm->Unk16B = 0;
    pEm->Unk16A = 0;
    pEm->Damage_cnt = 0;
    EM33_W(pEm)->Unk00 = 0;
    EM33_W(pEm)->Unk01 = 0;
    EM33_W(pEm)->Unk02 = 0;
    EM33_W(pEm)->Morph_dir = 0;
    EM33_W(pEm)->Unk0A = 0;
    EM33_W(pEm)->Unk0B = 0;
    EM33_W(pEm)->Unk0C = 0;
    EM33_W(pEm)->Unk11 = 0;
    EM33_W(pEm)->Unk12 = 0;
    EM33_W(pEm)->Unk24 = 0;
    EM33_W(pEm)->Unk0F = 0;
    EM33_W(pEm)->Unk14 = 0x210;
    EM33_W(pEm)->Unk16 = 0;
    EM33_W(pEm)->Unk28 = 0;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
    pEm->Be_flg |= 0x8000;
    pEm->Col.pEm = pEm;
    if (pEm->Id == 0x33)
        pEm->Be_flg |= 0x2010;
    else
        pEm->Be_flg |= 0x1000;
    if (pEm->Id == 0x33) {
        pEm->Col.W0 = pEm->Col.D0 = 1200;
        EM33_W(pEm)->Unk12 = 1;
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
    EM33_W(pEm)->Col_id = pEm->Col.Id |= 0x80;
    {
        PARTS_W *pS = pEm->pSin_parts_ptr;

        pEm->pUnk13C = (UCHAR *)&pS->Workm;
        pP = &pS[8];
    }
    PW_FREE(pP)[2] = 0x40;
    PW_FREE(pP)[4] = 0x300;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[3] = 0;
    PW_FREE(pP)[5] = 0;
    pEm->Pos_x = -4000;
    pEm->Pos_z = -19000;
    pEm->Be_flg |= 0xC00000;
    pEm->Cdir_y = 0x800;
    pEm->Be_flg |= 0x2000;
    if (G.System_flg & 0x100)
        EM33_W(pEm)->Old_life = pEm->Max_life = pEm->Life = 400;
    else
        EM33_W(pEm)->Old_life = pEm->Max_life = pEm->Life = 500;
    EM_MOVE(pEm) = 0;
    pEm->Max_life += 100;
    pEm->Life += 100;
    Joint_move(pEm, pKan, pSeq, 0x400);
    pB = func_8010138C(pEm);
    pB->Be_flg |= 0xC00010;
    func_801009DC(pB, 1, pEm, 8);
    Kage_work_set(&pB->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[8].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = func_8010138C(pEm);
    pB->Be_flg |= 0xC00010;
    func_801009DC(pB, 2, pEm, 1);
    Kage_work_set(&pB->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[1].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = func_8010138C(pEm);
    pB->Be_flg |= 0xC00010;
    func_801009DC(pB, 3, pEm, 2);
    Kage_work_set(&pB->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[2].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = func_8010138C(pEm);
    pB->Be_flg |= 0xC00010;
    func_801009DC(pB, 4, pEm, 3);
    Kage_work_set(&pB->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = func_8010138C(pEm);
    pB->Be_flg |= 0xC00010;
    func_801009DC(pB, 5, pEm, 4);
    Kage_work_set(&pB->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = func_8010138C(pEm);
    pB->Be_flg |= 0xC00010;
    func_801009DC(pB, 6, pEm, 5);
    Kage_work_set(&pB->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[5].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    pB = func_8010138C(pEm);
    pB->Be_flg |= 0xC00010;
    func_801009DC(pB, 7, pEm, 6);
    Kage_work_set(&pB->Kage.pKw, 0, 0x3E803E8, 0, (VECTOR *)pEm->pSin_parts_ptr[6].Workm.t);
    pB->Kage.pKw->Be_flg |= 0x80;
    i = 0;
    do {
        pP = &pEm->pSin_parts_ptr[i];
        pP->Be_flg |= 0x20;
    } while (++i < 17);
    if (pEm->Id == 0x33)
        pEm->Type = 2;
    EM33_W(pEm)->Unk20 = 0;
    EM33_W(pEm)->Unk22 = 0;
    pEm->Add_dir = 0;
    pP = &pEm->pSin_parts_ptr[17];
    pP->UnkA2 &= ~1;
}

/* set up body segment pEm following part Parts of the head pOya */
void func_801009DC(ENEMY_WORK *pEm, LONG No, ENEMY_WORK *pOya, LONG Parts)
{
    COL_OBJ *pCol = &pEm->Col;

    pEm->Type = 1;
    pEm->Unk16B = 0;
    pEm->Unk16A = 0;
    EM33_W(pEm)->Unk0A = 0;
    EM33_W(pEm)->Rtn = 0;
    EM33_W(pEm)->Unk00 = No;
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
    func_80033580(pCol, EM33_W(pOya)->Col_id);
    pCol->pM = &pOya->pSin_parts_ptr[Parts].Workm;
    pEm->pUnk13C = (UCHAR *)&pOya->pSin_parts_ptr[Parts].Workm;
    pEm->Unk134 = 1800;
    pEm->Unk136 = Parts == 8 ? -1800 : -1400;
    pEm->Unk138 = 0x78;
    pEm->Unk139 = 0xB4;
    pEm->Unk13A = Parts == 8 ? 1400 : 1000;
    pEm->Unk140 = 0;
    EM33_W(pEm)->Old_life = pEm->Max_life = pEm->Life = 30000;
    EM33_B(pEm)->Parts = Parts;
    EM33_B(pEm)->Oya = func_800336B4(pOya);
    PW_FREE(&pOya->pSin_parts_ptr[Parts])[7] = func_800336B4(pEm);
}

/* Em33_move: Routine_0 1 */
void func_80100B5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    func_80100B7C(pEm, pKan, pSeq);
}

void func_80100B7C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em33_move_tbl0[pEm->Routine_1](pEm);
    Em33_move_tbl1[pEm->Routine_1](pEm, pKan, pSeq);
}

/* move 0, 2, 5: wait for flag 31, then come out */
void func_80100C00(ENEMY_WORK *pEm)
{
    if (Flg_ck(&G.Unk7888, 31)) {
        if (++pEm->Timer0 > 30) {
            pEm->Timer0 = 180;
            pEm->Routine_1 = 1;
            EM33_W(pEm)->Unk19 = 1;
            EM33_W(pEm)->Unk24 = 100;
            pEm->Be_flg &= ~0x10;
            func_80107204();
        }
    }
}

void func_80100C88(void) {
}

/* chain the body parts: accumulated roll, each part hangs from the previous one */
void func_80100C90(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    PW_FREE(pP)[0] = pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[1];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[2];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[3];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[4];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[5];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[6];
    PW_FREE(pP)[0] = PW_FREE(pP - 1)[0] + pP->Cdir_z;
    func_80100E0C(pEm, &pEm->pSin_parts_ptr[0], &pEm->pSin_parts_ptr[1], 0x1000);
    func_80100E0C(pEm, &pEm->pSin_parts_ptr[1], &pEm->pSin_parts_ptr[2], 0x1000);
    func_80100E0C(pEm, &pEm->pSin_parts_ptr[2], &pEm->pSin_parts_ptr[3], 0x1000);
    func_80100E0C(pEm, &pEm->pSin_parts_ptr[3], &pEm->pSin_parts_ptr[4], 0x1000);
    func_80100E0C(pEm, &pEm->pSin_parts_ptr[4], &pEm->pSin_parts_ptr[5], 0x1000);
    func_80100E0C(pEm, &pEm->pSin_parts_ptr[5], &pEm->pSin_parts_ptr[6], 0x1000);
}

/* hang part pP1 from part pP0 and turn it towards pP0 */
void func_80100E0C(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate)
{
    VECTOR Pos;
    VECTOR Tip;
    SVECTOR Rot;
    LONG Dx;
    LONG Dz;
    LONG a;

    pP1->Be_flg |= 2;
    ApplyMatrixLV(&pP0->Workm, (VECTOR *)&pP1->Pos_x, &Pos);
    Pos.vx = pP0->Workm.t[0] + Pos.vx * Rate / 4096;
    Pos.vy = pP0->Workm.t[1] + Pos.vy * Rate / 4096;
    Pos.vz = pP0->Workm.t[2] + Pos.vz * Rate / 4096;
    Tip.vx = -800;
    Tip.vy = 0;
    Tip.vz = 0;
    ApplyMatrixLV(&pP1->Workm, &Tip, &Tip);
    Tip.vx = pP1->Workm.t[0] + Tip.vx;
    Tip.vy = pP1->Workm.t[1] + Tip.vy;
    Tip.vz = pP1->Workm.t[2] + Tip.vz;
    Dx = Pos.vx - Tip.vx;
    Dz = Pos.vz - Tip.vz;
    if (Dx) {
        a = catan((Dz << 12) / Dx);
        if (Dx < 0)
            Rot.vy = 0x800 - a;
        else
            Rot.vy = 0x1000 - a;
    } else if (Dz > 0)
        Rot.vy = 0xC00;
    else
        Rot.vy = 0x400;
    Rot.vx = 0;
    Rot.vz = PW_FREE(pP1)[0];
    RotMatrix_gte(&Rot, &pP1->Workm);
    pP1->Workm.t[0] = Pos.vx;
    pP1->Workm.t[1] = Pos.vy;
    pP1->Workm.t[2] = Pos.vz;
}

/* as func_80100E0C, sinking towards z -19000 with Move_cnt */
void func_80100FC8(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate)
{
    VECTOR Pos;
    VECTOR Tip;
    SVECTOR Rot;
    LONG Dx;
    LONG Dz;
    LONG a;

    pP1->Be_flg |= 2;
    ApplyMatrixLV(&pP0->Workm, (VECTOR *)&pP1->Pos_x, &Pos);
    Pos.vx = pP0->Workm.t[0] + Pos.vx * Rate / 4096;
    Pos.vy = pP0->Workm.t[1] + Pos.vy * Rate / 4096;
    Pos.vz = pP0->Workm.t[2] + Pos.vz * Rate / 4096;
    Pos.vz += (-19000 - Pos.vz) * pEm->Move_cnt / 50;
    Tip.vx = -800;
    Tip.vy = 0;
    Tip.vz = 0;
    ApplyMatrixLV(&pP1->Workm, &Tip, &Tip);
    Tip.vx = pP1->Workm.t[0] + Tip.vx;
    Tip.vy = pP1->Workm.t[1] + Tip.vy;
    Tip.vz = pP1->Workm.t[2] + Tip.vz;
    Dx = Pos.vx - Tip.vx;
    Dz = Pos.vz - Tip.vz;
    if (Dx) {
        a = catan((Dz << 12) / Dx);
        if (Dx < 0)
            Rot.vy = 0x800 - a;
        else
            Rot.vy = 0x1000 - a;
    } else if (Dz > 0)
        Rot.vy = 0xC00;
    else
        Rot.vy = 0x400;
    Rot.vx = 0;
    Rot.vz = PW_FREE(pP1)[0];
    RotMatrix_gte(&Rot, &pP1->Workm);
    pP1->Workm.t[0] = Pos.vx;
    pP1->Workm.t[1] = Pos.vy;
    pP1->Workm.t[2] = Pos.vz;
}

/* sink the body: shift the parts sideways and re-chain them from the tail */
void func_801011C8(ENEMY_WORK *pEm)
{
    SVECTOR Unused; /* frame size */
    PARTS_W *pP;

    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[6];
        LONG X;

        PW_FREE(pS)[0] = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM33_W(pEm)->Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[5];
        LONG X;

        PW_FREE(pS)[0] = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM33_W(pEm)->Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[4];
        LONG X;

        PW_FREE(pS)[0] = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM33_W(pEm)->Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[3];
        LONG X;

        PW_FREE(pS)[0] = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM33_W(pEm)->Side ? X + 400 : X - 400;
    }
    {
        PARTS_W *pS = &pEm->pSin_parts_ptr[2];
        LONG X;

        PW_FREE(pS)[0] = pS->Cdir_z;
        X = pS->Workm.t[0];
        pS->Workm.t[0] = EM33_W(pEm)->Side ? X + 400 : X - 400;
    }
    pP = &pEm->pSin_parts_ptr[1];
    PW_FREE(pP)[0] = pP->Cdir_z;
    pP = &pEm->pSin_parts_ptr[0];
    PW_FREE(pP)[0] = pP->Cdir_z;
    func_80100FC8(pEm, &pEm->pSin_parts_ptr[5], &pEm->pSin_parts_ptr[6], 0x1000);
    func_80100FC8(pEm, &pEm->pSin_parts_ptr[4], &pEm->pSin_parts_ptr[5], 0x1000);
    func_80100FC8(pEm, &pEm->pSin_parts_ptr[3], &pEm->pSin_parts_ptr[4], 0x1000);
    func_80100FC8(pEm, &pEm->pSin_parts_ptr[2], &pEm->pSin_parts_ptr[3], 0x1000);
    func_80100FC8(pEm, &pEm->pSin_parts_ptr[1], &pEm->pSin_parts_ptr[2], 0x1000);
    func_80100FC8(pEm, &pEm->pSin_parts_ptr[0], &pEm->pSin_parts_ptr[1], 0x1000);
}

/* allocate a new work (copy of pEm) at the end of the work list (RE2 Enemy_work_set style) */
ENEMY_WORK *func_8010138C(ENEMY_WORK *pEm)
{
    tagGLOBAL *pG = &G;
    ULONG n = 0;

    while (&pG->pEnemy[n] != pG->ppEnemy_end)
        n++;
    pG->Em_num++;
    pG->pEnemy[n] = pG->pEm_mem;
    pG->ppEnemy_end = &pG->pEnemy[n + 1];
    pG->pEm_mem++;
    *pG->pEnemy[n] = *pEm;
    return pG->pEnemy[n];
}

/* cycle the morph keys 0..2 of parts 1-3 (Unk01 = phase). Returns the last sabun call's value:
   that explicit v0 use is what makes reload put the /50 mfhi in a3 (see NOTES.md). */
LONG func_80101454(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG Step;

    Step = EM33_W(pEm)->Unk14 * pEm->Spd_x / 50;
    pP = &pEm->pSin_parts_ptr[1];
    if (Step < 0)
        Step = 0;
    switch (EM33_W(pEm)->Unk01) {
    case 0:
        EM33_SABUN_P(&pP[0])->Key[0].Rate += Step;
        EM33_SABUN_P(&pP[1])->Key[0].Rate += Step;
        EM33_SABUN_P(&pP[2])->Key[0].Rate += Step;
        if (EM33_SABUN_P(&pP[0])->Key[0].Rate > 0xC00) {
            EM33_SABUN_P(&pP[0])->Key[0].Rate = 0xC00;
            EM33_SABUN_P(&pP[1])->Key[0].Rate = 0xC00;
            EM33_SABUN_P(&pP[2])->Key[0].Rate = 0xC00;
            EM33_W(pEm)->Unk01 = 1;
        }
        break;
    case 1:
        EM33_SABUN_P(&pP[0])->Key[0].Rate -= Step;
        EM33_SABUN_P(&pP[1])->Key[0].Rate -= Step;
        EM33_SABUN_P(&pP[2])->Key[0].Rate -= Step;
        if (EM33_SABUN_P(&pP[0])->Key[0].Rate < 0) {
            EM33_SABUN_P(&pP[0])->Key[0].Rate = 0;
            EM33_SABUN_P(&pP[1])->Key[0].Rate = 0;
            EM33_SABUN_P(&pP[2])->Key[0].Rate = 0;
        }
        EM33_SABUN_P(&pP[0])->Key[1].Rate += Step;
        EM33_SABUN_P(&pP[1])->Key[1].Rate += Step;
        EM33_SABUN_P(&pP[2])->Key[1].Rate += Step;
        if (EM33_SABUN_P(&pP[0])->Key[1].Rate > 0xC00) {
            EM33_SABUN_P(&pP[0])->Key[1].Rate = 0xC00;
            EM33_SABUN_P(&pP[1])->Key[1].Rate = 0xC00;
            EM33_SABUN_P(&pP[2])->Key[1].Rate = 0xC00;
            EM33_W(pEm)->Unk01 = 2;
        }
        break;
    case 2:
        EM33_SABUN_P(&pP[0])->Key[1].Rate -= Step;
        EM33_SABUN_P(&pP[1])->Key[1].Rate -= Step;
        EM33_SABUN_P(&pP[2])->Key[1].Rate -= Step;
        if (EM33_SABUN_P(&pP[0])->Key[1].Rate < 0) {
            EM33_SABUN_P(&pP[0])->Key[1].Rate = 0;
            EM33_SABUN_P(&pP[1])->Key[1].Rate = 0;
            EM33_SABUN_P(&pP[2])->Key[1].Rate = 0;
        }
        EM33_SABUN_P(&pP[0])->Key[2].Rate += Step;
        EM33_SABUN_P(&pP[1])->Key[2].Rate += Step;
        EM33_SABUN_P(&pP[2])->Key[2].Rate += Step;
        if (EM33_SABUN_P(&pP[0])->Key[2].Rate > 0xC00) {
            EM33_SABUN_P(&pP[0])->Key[2].Rate = 0xC00;
            EM33_SABUN_P(&pP[1])->Key[2].Rate = 0xC00;
            EM33_SABUN_P(&pP[2])->Key[2].Rate = 0xC00;
            EM33_W(pEm)->Unk01 = 3;
        }
        break;
    case 3:
        EM33_SABUN_P(&pP[0])->Key[2].Rate -= Step;
        EM33_SABUN_P(&pP[1])->Key[2].Rate -= Step;
        EM33_SABUN_P(&pP[2])->Key[2].Rate -= Step;
        if (EM33_SABUN_P(&pP[0])->Key[2].Rate < 0) {
            EM33_SABUN_P(&pP[0])->Key[2].Rate = 0;
            EM33_SABUN_P(&pP[1])->Key[2].Rate = 0;
            EM33_SABUN_P(&pP[2])->Key[2].Rate = 0;
            EM33_W(pEm)->Unk01 = 0;
        }
        break;
    }
    func_8004AC54(pEm, &pP[0]);
    func_8004AC54(pEm, &pP[1]);
    return func_8004AC54_l(pEm, &pP[2]);
}

/* morph key 0 of part 8 one step towards 0 (Dir != 0) or 0x1000 */
void func_8010183C(ENEMY_WORK *pEm, LONG Step, LONG Dir)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

    if (Dir) {
        EM33_SABUN_P(pP)->Key[0].Rate -= Step;
        if (EM33_SABUN_P(pP)->Key[0].Rate < 0)
            EM33_SABUN_P(pP)->Key[0].Rate = 0;
    } else {
        EM33_SABUN_P(pP)->Key[0].Rate += Step;
        if (EM33_SABUN_P(pP)->Key[0].Rate > 0x1000)
            EM33_SABUN_P(pP)->Key[0].Rate = 0x1000;
    }
    func_8004AC54(pEm, pP);
}

/* fade a body part in: Cnt counts down from 0x40, 8 steps per part (parts 3 and 5 skipped) */
void func_801018DC(ENEMY_WORK *pEm, LONG Cnt)
{
    PARTS_W *pP;
    LONG Fade;
    SHORT Lv;
    ULONG Rgb;

    EM33_W(pEm)->Unk28--;
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

void func_80101AE8(ENEMY_WORK *pEm)
{
    if (EM33_W(pEm)->Unk28)
        func_801018DC(pEm, EM33_W(pEm)->Unk28);
    func_8010183C(pEm, 0x400, EM33_W(pEm)->Morph_dir);
}

/* move 1: rise out of the ground (Unk11 == 0) or dive back (cases 2, 3) */
void func_80101B30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    VECTOR Pos;
    LONG i;
    LONG No;
    ULONG Rgb;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x1F0005;
        EM33_W(pEm)->Unk04 = 10;
        EM33_W(pEm)->Unk06 = 20;
        EM33_W(pEm)->Unk08 = 2;
        pEm->Routine_2 = 1;
        pEm->Spd_z = 0;
        pEm->Timer0 = 0;
        pEm->Cdir_y = (EM33_W(pEm)->Side & 1) << 11;
        if (EM33_W(pEm)->Unk11 == 0) {
            EM33_W(pEm)->Unk28 = 0x40;
            pEm->Damage_cnt &= 0x7F;
        }
        Pos.vx = pEm->Pos_x;
        Pos.vy = pEm->Pos_y;
        Pos.vz = pEm->Pos_z;
        Snd_se_stad(0x10206, &Pos, 0, 0);
    case 1:
        if (EM33_W(pEm)->Unk11 == 0) {
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
            if (EM33_W(pEm)->Unk11 == 0) {
                pEm->Unk120 &= ~1;
                PW_FREE(pP)[0] = 0;
                PW_FREE(pP)[1] = 0;
                EM_RTN(pEm) = 0x901;
            } else
                pEm->Routine_2++;
        }
        pEm->Spd_x = EM33_W(pEm)->Unk04;
        Add_speedXZ(pEm, 0);
        EM33_W(pEm)->Unk04 += EM33_W(pEm)->Unk08;
        if (EM33_W(pEm)->Unk08 > 0 && EM33_W(pEm)->Unk04 > EM33_W(pEm)->Unk06)
            EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
        if (pEm->Move_cnt != 1)
            func_80100C90(pEm);
        if (EM33_W(pEm)->Unk20 == 0) {
            Snd_se_enem(9, pEm);
            EM33_W(pEm)->Unk20 = 25;
        } else
            EM33_W(pEm)->Unk20--;
        break;
    case 2:
        EM33_W(pEm)->Unk24 = 60;
        pEm->Timer0 = 0;
        pEm->Routine_2++;
        pEm->Be_flg |= 0x2000;
        pEm->Unk120 |= 1;
        EM33_W(pEm)->Side ^= 1;
    case 3:
        if (++pEm->Timer0 > 150) {
            EM33_W(pEm)->Unk11 = 0;
            EM33_W(pEm)->Unk12 = 1;
            if (EM33_W(pEm)->Unk0F) {
                EM_RTN(pEm) = 0x601;
                pEm->Be_flg |= 0x10;
            } else
                EM_RTN(pEm) = 0x701;
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
            if (EM33_W(pEm)->Unk20 == 0) {
                Snd_se_enem(9, pEm);
                EM33_W(pEm)->Unk20 = 25;
            } else
                EM33_W(pEm)->Unk20--;
        }
        break;
    }
}

/* move 3: pick the destination from the player's zone */
void func_8010201C(ENEMY_WORK *pEm)
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
        if (!(EM33_W(pEm)->Side & 1))
            pEm->Dest_x = -18500;
        else
            pEm->Dest_x = -25500;
        pEm->Dest_z = -16500;
    }
    Zone[0] = -11100;
    Zone[1] = -25800;
    if (func_800101C8(&Pos.vx, Zone)) {
        if (!(EM33_W(pEm)->Side & 1))
            pEm->Dest_x = -6500;
        else
            pEm->Dest_x = -13500;
        pEm->Dest_z = -21500;
    }
    if (EM33_W(pEm)->Unk20 == 0) {
        Snd_se_enem(9, pEm);
        EM33_W(pEm)->Unk20 = 25;
    } else
        EM33_W(pEm)->Unk20--;
    if (EM33_W(pEm)->Unk28)
        func_801018DC(pEm, EM33_W(pEm)->Unk28);
}

/* swim to Dest, splashing where the fins (parts 10, 12) cut the water */
void func_80102130(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        pEm->Spd_x = EM33_W(pEm)->Unk04;
        Add_speedXZ(pEm, 0);
        break;
    case 1:
        EM_MOVE(pEm) = 0x1F0003;
        pEm->Routine_2 = 2;
        EM33_W(pEm)->Unk04 = 10;
        EM33_W(pEm)->Unk06 = 40;
        EM33_W(pEm)->Unk08 = 1;
        pEm->Spd_z = 0;
        pEm->Be_flg &= ~0x2000;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            EM_RTN(pEm) = 0x401;
        pEm->Spd_x = EM33_W(pEm)->Unk04;
        Add_speedXZ(pEm, 0);
        EM33_W(pEm)->Unk04 += EM33_W(pEm)->Unk08;
        if (EM33_W(pEm)->Unk08 > 0 && EM33_W(pEm)->Unk04 > EM33_W(pEm)->Unk06)
            EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
        if (EM33_W(pEm)->Unk22 == 0) {
            Spd.vx = -280;
            Spd.vz = 100;
            Spd.vy = 0;
            pP = &pEm->pSin_parts_ptr[10];
            Pos.vx = pP->Workm.t[0];
            Pos.vz = pP->Workm.t[2];
            if (func_8004D720(&Pos.vx, 0, 0x8000, 0)) {
                EM33_W(pEm)->Unk22 = 6;
                Esp_call(0x16, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[10].Workm, &Spd, 0);
            }
            pP = &pEm->pSin_parts_ptr[12];
            Pos.vx = pP->Workm.t[0];
            Pos.vz = pP->Workm.t[2];
            if (func_8004D720(&Pos.vx, 0, 0x8000, 0)) {
                EM33_W(pEm)->Unk22 = 6;
                Esp_call(0x16, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[12].Workm, &Spd, 0);
            }
        }
        break;
    default:
        return;
    }
    func_80100C90(pEm);
}

/* move 4 */
void func_801023A0(ENEMY_WORK *pEm)
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
    if (EM33_W(pEm)->Unk20 == 0) {
        Snd_se_enem(9, pEm);
        EM33_W(pEm)->Unk20 = 25;
    } else
        EM33_W(pEm)->Unk20--;
}

INCLUDE_ASM("asm/nonmatchings/ovl/em33/em33", func_80102484);

/* splashes: around the hole while Unk19 is set, and along the body side */
void func_80102D30(ENEMY_WORK *pEm)
{
    SVECTOR Sv;

    Sv.vx = -6500;
    if (EM33_W(pEm)->Unk19) {
        if (EM33_W(pEm)->Unk24 == 99) {
            Sv.vy = -2000;
            Sv.vz = -18400;
            Esp_call(0x41, 0x10000000, &GsIDMATRIX, &Sv, 0);
            Sv.vy = -1800;
            Sv.vz = -18800;
            Esp_call(0x141, 0xE000000, &GsIDMATRIX, &Sv, 0);
        }
        if (EM33_W(pEm)->Unk24 == 97) {
            Sv.vy = -1200;
            Sv.vz = -18000;
            Esp_call(0x141, 0xC000000, &GsIDMATRIX, &Sv, 0);
            Sv.vy = -1400;
            Sv.vz = -19400;
            Esp_call(0x41, 0x10000000, &GsIDMATRIX, &Sv, 0);
        }
        if (EM33_W(pEm)->Unk24 == 92) {
            Sv.vy = -1700;
            Sv.vz = -19000;
            Esp_call(0x241, 0x10000000, &GsIDMATRIX, &Sv, 0);
        }
        if (EM33_W(pEm)->Unk24 % 3 == 0) {
            Sv.vy = -((Rnd() & 7) * 300);
            Sv.vz = -((Rnd() & 7) * 200) - 18000;
            Esp_call(0x341, 0x10000000, &GsIDMATRIX, &Sv, 0);
        }
    }
    if (EM33_W(pEm)->Rtn == 4)
        return;
    switch (EM33_W(pEm)->Rtn) {
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

void func_801030D8(void) {
}

/* Em33_dead body: wait and remove */
void func_801030E0(ENEMY_WORK *pEm)
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

void func_8010316C(ENEMY_WORK *pEm)
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

void func_80103208(ENEMY_WORK *pEm)
{
    LONG i = 0;
    PARTS_W *pP = pEm->pSin_parts_ptr;

    do {
        pP->UnkA2 &= ~1;
        pP->Poly_rgb = 0;
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* Em33 sub: water noise, breaking off (Unk0F / damage), the player leaving through the west door */
void func_8010324C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Se_pos;
    VECTOR Pos;
    SHORT Zone[4];

    if (pEm->Routine_2 == 1) {
        if (EM33_W(pEm)->Unk20 == 0) {
            Se_pos.vx = pPl->Pos_x + (Rnd() & 0x7F) - 0x40;
            Se_pos.vz = -10000;
            Se_pos.vy = -500;
            func_8010718C(9, pEm, &Se_pos.vx);
            EM33_W(pEm)->Unk20 = 25;
        } else
            EM33_W(pEm)->Unk20--;
    }
    if (pEm->Routine_2 < 2 && EM33_W(pEm)->Unk0F)
        EM_RTN(pEm) = 0x601;
    if (pEm->Routine_2 == 4) {
        if (EM33_W(pEm)->Unk2A - pEm->Life > 80 || EM33_W(pEm)->Unk0F) {
            EM33_W(pEm)->Unk04 = -10;
            EM33_W(pEm)->Unk06 = -10;
            EM33_W(pEm)->Unk08 = 0;
            Snd_se_enem(3, pEm);
            EM_MOVE(pEm) = 0x1F0008;
            G.Status_flg |= 4;
            EM33_W(pEm)->Unk24 = 60;
            pEm->Routine_2 = 6;
        }
        if (EM33_W(pEm)->Rtn == 0 && pEm->Pos_x < -25500) {
            Zone[0] = -23650;
            Zone[1] = -16000;
            Zone[2] = 2400;
            Zone[3] = 4400;
            Pos.vx = pPl->Pos_x;
            Pos.vz = pPl->Pos_z;
            if (func_800101C8(&Pos.vx, Zone)) {
                EM33_W(pEm)->Unk04 = 150;
                EM33_W(pEm)->Unk06 = 150;
                EM33_W(pEm)->Unk08 = 10;
                G.Status_flg &= ~2;
                EM_RTN(pEm) = 0x401;
            }
        }
    }
}

/* Side = zone of the room the player is in (0..3, 4 = none); with Flg, pick the hole to come out of */
void func_8010342C(ENEMY_WORK *pEm, LONG Flg)
{
    ENEMY_WORK *pPl = G_PLAYER;
    VECTOR Pos;
    SHORT Zone[4];

    Pos.vx = pPl->Pos_x;
    Pos.vz = pPl->Pos_z;
    EM33_W(pEm)->Side = 4;
    Zone[2] = 6500;
    Zone[3] = 2770;
    Zone[0] = -27420;
    Zone[1] = -20300;
    if (func_800101C8(&Pos.vx, Zone))
        EM33_W(pEm)->Side = 0;
    Zone[0] = -12000;
    Zone[1] = -20090;
    if (func_800101C8(&Pos.vx, Zone))
        EM33_W(pEm)->Side = 1;
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
        EM33_W(pEm)->Side = 2;
    Zone[2] = 2400;
    Zone[3] = 7500;
    Zone[0] = -11100;
    Zone[1] = -24800;
    if (func_800101C8(&Pos.vx, Zone))
        EM33_W(pEm)->Side = 3;
    if (Flg) {
        EM33_W(pEm)->Unk2A = pEm->Life;
        if (Flg_ck(&G.Unk7888, 0)) {
            if (Flg_ck(&G.Unk7888, 1)) {
                if (EM33_W(pEm)->Side == 2 || EM33_W(pEm)->Side == 4)
                    EM33_W(pEm)->Side = 0;
            } else if (EM33_W(pEm)->Side == 0 || EM33_W(pEm)->Side == 4)
                EM33_W(pEm)->Side = 2;
        } else if (Flg_ck(&G.Unk7888, 1)) {
            if (EM33_W(pEm)->Side == 1 || EM33_W(pEm)->Side == 4)
                EM33_W(pEm)->Side = 3;
        } else if (EM33_W(pEm)->Side < 2)
            EM33_W(pEm)->Side += 2;
    }
}

/* is the player still in the zone of Side */
LONG func_80103660(ENEMY_WORK *pEm)
{
    VECTOR Pos;
    SHORT Zone[4];

    Pos.vx = Pl_work.Pos_x;
    Pos.vz = Pl_work.Pos_z;
    switch (EM33_W(pEm)->Side) {
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

INCLUDE_ASM("asm/nonmatchings/ovl/em33/em33", func_80103778);

void func_801042DC(ENEMY_WORK *pEm)
{
    if (pEm->Routine_2 < 2 && EM33_W(pEm)->Unk0F)
        EM_RTN(pEm) = 0x601;
    if (pEm->Routine_2 == 3 && (EM33_W(pEm)->Unk2A - pEm->Life > 80 || EM33_W(pEm)->Unk0F)) {
        EM33_W(pEm)->Unk04 = -10;
        EM33_W(pEm)->Unk06 = -10;
        EM33_W(pEm)->Unk08 = 0;
        EM_MOVE(pEm) = 0x1F000C;
        Snd_se_enem(3, pEm);
        EM33_W(pEm)->Unk24 = 60;
        pEm->Routine_2 = 5;
        pEm->Unk120 |= 1;
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl/em33/em33", Em33_spd_z_tbl);

/* move 8: surface, bite the player and dive again (Em33_move_tbl1) */
void func_801043A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        func_8010342C(pEm, 1);
        if (EM33_W(pEm)->Side == 4)
            EM33_W(pEm)->Side = Rnd() & 3;
        if (EM33_W(pEm)->Side != 1) {
            EM_RTN(pEm) = 0x701;
            return;
        }
        switch (EM33_W(pEm)->Side & 1) {
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
        EM_MOVE(pEm) = 0x1F000A;
        EM33_W(pEm)->Morph_dir = 1;
        EM33_W(pEm)->Unk04 = 10;
        pEm->Routine_2++;
        pEm->Timer0 = 0;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        EM33_W(pEm)->Unk06 = 60;
        EM33_W(pEm)->Unk08 = 4;
        pEm->Col.D0 = 2000;
        pEm->Col.W0 = 2000;
        {
            ENEMY_WORK *pW = G.pEnemy[pEm->pSin_parts_ptr[8].Unk82];
        pW->Col.D0 = 2000;
        pW->Col.W0 = 2000;
        }
        EM33_W(pEm)->Unk24 = 60;
        Snd_se_enem(4, pEm);
        G.Status_flg |= 2;
        func_8010722C();
        break;
    case 2:
        EM33_W(pEm)->Rtn = EM33_W(pEm)->Side;
        do {
            if (Joint_move(pEm, pKan, pSeq, 0x80)) {
                n = 1;
                EM_MOVE(pEm) = 0x1F000B;
                EM33_W(pEm)->Unk04 = -10;
                EM33_W(pEm)->Unk06 = -70;
                EM33_W(pEm)->Unk08 = -10;
                pEm->Spd_z = 0;
                pEm->UnkE2 = 0;
                pEm->UnkE3 = 0;
                pEm->Routine_2++;
            }
            func_8010183C(pEm, 0x80, EM33_W(pEm)->Morph_dir);
            pEm->Spd_x = EM33_W(pEm)->Unk04;
            pEm->Spd_z = Spd_z.Spd[pEm->Move_cnt / 10];
            Add_speedXZ(pEm, 0);
            EM33_W(pEm)->Unk04 += EM33_W(pEm)->Unk08;
            if (EM33_W(pEm)->Unk08 > 0 && EM33_W(pEm)->Unk04 > EM33_W(pEm)->Unk06)
                EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
            if (EM33_W(pEm)->Unk08 < 0 && EM33_W(pEm)->Unk04 < EM33_W(pEm)->Unk06)
                EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
            if (pEm->Move_cnt == 10)
                G.Status_flg &= ~2;
        } while (--n);
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_MOVE(pEm) = 0x1F000A;
            EM33_W(pEm)->Unk04 = -70;
            EM33_W(pEm)->Unk06 = -200;
            EM33_W(pEm)->Unk08 = -1;
            EM33_W(pEm)->Unk24 = 60;
            pEm->Routine_2++;
            if (EM33_W(pEm)->Unk0F)
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
            EM33_W(pEm)->Unk04 -= EM33_W(pEm)->Unk08;
            if (EM33_W(pEm)->Unk04 < 30)
                EM33_W(pEm)->Unk04 = 30;
        }
        pEm->Spd_z = 0;
        pEm->Spd_x = EM33_W(pEm)->Unk04;
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
                    EM_RTN(&Pl_work) = 3;
                pEm->UnkE2 = 1;
                pEm->UnkE3 = -100;
            }
        }
        if (pEm->Move_cnt == 24 && !pEm->UnkE2)
            Snd_se_enem(0, pEm);
        if (pEm->Move_cnt == 30) {
            EM33_W(pEm)->Unk04 = -70;
            EM33_W(pEm)->Unk06 = 350;
            EM33_W(pEm)->Unk08 = 80;
        }
        if (pEm->Move_cnt == 36) {
            EM33_W(pEm)->Unk04 = 350;
            EM33_W(pEm)->Unk06 = -70;
            EM33_W(pEm)->Unk08 = -80;
        }
        EM33_W(pEm)->Unk04 += EM33_W(pEm)->Unk08;
        if (EM33_W(pEm)->Unk08 > 0 && EM33_W(pEm)->Unk04 > EM33_W(pEm)->Unk06)
            EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
        if (EM33_W(pEm)->Unk08 < 0 && EM33_W(pEm)->Unk04 < EM33_W(pEm)->Unk06)
            EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
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
                EM33_W(pEm)->Rtn = 4;
                EM_RTN(pEm) = 0x701;
            }
            pEm->Spd_z = 0;
            pEm->Spd_x = EM33_W(pEm)->Unk04;
            Add_speedXZ(pEm, 0);
            EM33_W(pEm)->Unk04 += EM33_W(pEm)->Unk08;
            if (EM33_W(pEm)->Unk04 < EM33_W(pEm)->Unk06)
                EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
            func_8010183C(pEm, 0x80, EM33_W(pEm)->Morph_dir);
            if (!EM33_W(pEm)->Unk0F && (pEm->Move_cnt == 2 || pEm->Move_cnt == 26))
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
            EM33_W(pEm)->Unk0B = 1;
            EM_MOVE(pEm) = 0x1F000A;
            pEm->Unk120 = 0;
            EM33_W(pEm)->Unk04 = -10;
            EM33_W(pEm)->Unk06 = -70;
            EM33_W(pEm)->Unk08 = -10;
            pEm->Spd_z = 0;
            pEm->UnkE2 = 0;
            pEm->UnkE3 = 0;
            pEm->Routine_2--;
        }
        pEm->Spd_z = 0;
        pEm->Spd_x = EM33_W(pEm)->Unk04;
        Add_speedXZ(pEm, 0);
        EM33_W(pEm)->Unk04 += EM33_W(pEm)->Unk08;
        if (EM33_W(pEm)->Unk04 < EM33_W(pEm)->Unk06)
            EM33_W(pEm)->Unk04 = EM33_W(pEm)->Unk06;
        func_8010183C(pEm, 0x80, EM33_W(pEm)->Morph_dir);
        break;
    }
    func_801053FC(pEm);
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

void func_80104CFC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em33/em33", func_80104D04);

void func_801051A4(void) {
}

/* damage: thrash for a random time at double speed, then back off */
void func_801051AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF000D;
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
            EM_MOVE(pEm) = 0x1F1809;
            EM_RTN(pEm) = 0x40701;
        }
        pEm->Spd_x = -50;
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* hide the parts that went past x -29000 (into the wall), fade the ones close to it */
void func_8010532C(ENEMY_WORK *pEm)
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
            func_80105A48(pEm, i);
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

void func_801053FC(ENEMY_WORK *pEm)
{
    Em33_sub_tbl[EM33_W(pEm)->Rtn](pEm);
}

/* as func_8010532C while sinking: part 7 follows part 8 */
void func_80105438(ENEMY_WORK *pEm)
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
                func_80105A48(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* as func_80105438 for the parts that came out of the ground (x below -903) */
void func_801055AC(ENEMY_WORK *pEm)
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
                func_80105A48(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* as func_801055AC on z: the parts past z -8952 */
void func_80105704(ENEMY_WORK *pEm)
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
                func_80105A48(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* as func_80105438 on z: the parts past z -28024 */
void func_8010582C(ENEMY_WORK *pEm)
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
                func_80105A48(pEm, i);
        }
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

void func_80105980(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    LONG i = 0;

    do {
        pP->UnkA2 &= ~1;
        pP->Be_flg &= ~2;
        pP->Poly_rgb = 0x808080;
        if (i < 9 && i != 7)
            func_80105A48(pEm, i);
        pP++;
    } while (++i < pEm->Parts_num - 1);
}

/* collision of part No follows its visibility */
void func_80105A48(ENEMY_WORK *pEm, LONG No)
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
        pW->Col.Id = EM33_W(pEm)->Col_id;
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

/* Em33_damage: Routine_0 2 */
void func_80105B40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused; /* frame size */

    pEm->Routine_1 = 0;
    if (pEm->Unk12C == 30 && !(pEm->Unk132 & 4))
        pEm->Routine_1 = 1;
    Em33_damage_tbl[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
}

/* damage 0: the head slows down; a body segment passes the damage on to the head */
void func_80105BB8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pH;
    LONG Dmg;

    if (!(pEm->Type & 1)) {
        EM33_W(pEm)->Unk0C++;
        if (EM33_W(pEm)->Unk04 > 10)
            EM33_W(pEm)->Unk04 -= 10;
        EM_RTN(pEm) = pEm->Unk1C4;
        func_80100B5C(pEm, pKan, pSeq);
        Snd_se_enem(2, pEm);
    } else {
        pH = G.pEnemy[EM33_B(pEm)->Oya];
        if (EM33_B(pEm)->Parts == 8) {
            if (EM33_W(pH)->Morph_dir) {
                EM33_W(pH)->Unk0C += 5;
                if (!pH->Damage_cnt) {
                    Dmg = EM33_B(pEm)->Old_life - pEm->Life;
                    pH->Damage_cnt = pEm->Damage_cnt;
                    pH->Life -= Dmg;
                }
            } else if (!pH->Damage_cnt) {
                Dmg = EM33_B(pEm)->Old_life - pEm->Life;
                pH->Damage_cnt = pEm->Damage_cnt;
                pH->Life -= Dmg;
            }
        } else if (!pH->Damage_cnt) {
            Dmg = EM33_B(pEm)->Old_life - pEm->Life;
            pH->Damage_cnt = pEm->Damage_cnt;
            pH->Life -= Dmg * 2 / 3;
        }
        if (pH->Life < 0)
            EM33_W(pH)->Unk0F = 1;
        EM33_W(pH)->Unk0C++;
        Snd_se_enem(2, pH);
        EM_RTN(pEm) = pEm->Unk1C4;
    }
    if (pEm->Unk12C == 5) {
        pEm->Unk16A = 1;
        EM33_B(pEm)->Old_life = pEm->Life;
    } else
        func_801061C4(pEm);
}

/* damage 1: dive away (a body segment hands the damage to the head) */
void func_80105D74(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Unused[4] = { 0x12, 0x13, 0x14, 0x15 };
    UCHAR Move[2] = { 7, 8 };
    ENEMY_WORK *pH;

    if (pEm->Type & 1) {
        pH = G.pEnemy[EM33_B(pEm)->Oya];
        pH->Unk12C = pEm->Unk12C;
        if (pH->Routine_0 == 1)
            pH->Unk1C4 = EM_RTN(pH);
        EM_RTN(pH) = EM_RTN(pEm);
        EM_RTN(pEm) = pEm->Unk1C4;
        func_801061C4(pEm);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Move[EM33_W(pEm)->Morph_dir] + 0x1F0000;
        func_801061C4(pEm);
        EM33_W(pEm)->Unk04 = 40;
        EM33_W(pEm)->Unk06 = 80;
        EM33_W(pEm)->Unk08 = 5;
        pEm->Spd_z = 0;
        Snd_se_enem(3, pEm);
        pEm->Routine_2++;
    case 1:
        if (pEm->Move_cnt == 3 || pEm->Move_cnt == 9)
            func_801061C4(pEm);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            EM_RTN(pEm) = EM33_W(pEm)->Rtn_save;
        func_801011C8(pEm);
        break;
    }
}

/* damage 2 (grenade): first hit breaks the head (Life / 3), the second one kills it */
void func_80105F10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pH;

    if (pEm->Type & 1) {
        pH = G.pEnemy[EM33_B(pEm)->Oya];
        if (!Flg_ck(&G.Unk7888, 0) && EM33_W(pH)->Unk0B != 1) {
            pH->Unk12C = pEm->Unk12C;
            pH->Unk132 = pEm->Unk132;
            pH->Damage_cnt = 10;
            pH->Unk1C4 = EM_RTN(pH);
            EM_RTN(pH) = EM_RTN(pEm);
            func_80106C48(pEm);
            Flg_on(&G.Unk7888, 0);
        }
        pEm->Damage_cnt = 10;
        EM_RTN(pEm) = pEm->Unk1C4;
        return;
    }
    if (EM33_W(pEm)->Unk0B != 1) {
        if (EM33_W(pEm)->Unk0A == 1) {
            func_80106084(pEm, pKan, pSeq);
            return;
        }
        EM33_W(pEm)->Unk0A = 1;
        pEm->Damage_cnt = 10;
        pEm->Life /= 3;
        func_80106C48(pEm);
        Flg_on(&G.Unk7888, 0);
    }
    EM_RTN(pEm) = pEm->Unk1C4;
    func_80100B5C(pEm, pKan, pSeq);
}

/* damage 2, second hit: kill the head */
void func_80106084(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pH;

    if (pEm->Type & 1) {
        pH = G.pEnemy[EM33_B(pEm)->Oya];
        if (!Flg_ck(&G.Unk7888, 0) && EM33_W(pH)->Unk0B != 1) {
            pH->Unk12C = pEm->Unk12C;
            pH->Unk132 = pEm->Unk132;
            pH->Unk1C4 = EM_RTN(pH);
            EM_RTN(pH) = EM_RTN(pEm);
            func_80106C48(pEm);
        }
        pEm->Damage_cnt = 10;
        EM_RTN(pEm) = pEm->Unk1C4;
        return;
    }
    if (EM33_W(pEm)->Unk0B == 1) {
        EM_RTN(pEm) = pEm->Unk1C4;
        func_80100B5C(pEm, pKan, pSeq);
        return;
    }
    pEm->Life = -1;
    Flg_on(&G.Unk7888, 0);
    if (EM33_W(pEm)->Rtn == 0)
        func_80106C48(pEm);
    EM_RTN(pEm) = pEm->Unk1C4;
    func_80100B5C(pEm, pKan, pSeq);
    EM_RTN(pEm) = 3;
}

/* blood: more drops the bigger the life loss since the last call */
void func_801061C4(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pH;
    SVECTOR Sv;
    LONG Dmg;
    LONG n;
    LONG Kind;

    Dmg = EM33_W(pEm)->Old_life - pEm->Life;
    EM33_W(pEm)->Old_life = pEm->Life;
    n = 2;
    if (Dmg > 10)
        n = 4;
    if (Dmg > 40)
        n += 2;
    if (Dmg > 100)
        n += 2;
    if (pEm->Type & 1) {
        pH = G.pEnemy[EM33_B(pEm)->Oya];
        while (n--) {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = (Rnd() & 0xFF) - 0x7F;
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vy <<= 2;
            Sv.vz <<= 2;
            Kind = (Rnd() & 3) * 0x300 + 0x1200;
            Esp_call(0x7000700, (Kind << 16) | (USHORT)pH->Cdir_y,
                          &pH->pSin_parts_ptr[EM33_B(pEm)->Parts].Workm, &Sv, 0);
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

INCLUDE_ASM("asm/nonmatchings/ovl/em33/em33", func_801063CC);

/* trail of 8 splashes behind the work, one call per frame while Unk16A is 2 */
void func_80106668(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    SVECTOR Ofs;
    SVECTOR Step;
    MATRIX M;
    PARTS_W *pP;

    switch (pEm->Unk16A) {
    case 0:
        break;
    case 1:
        pEm->Unk16A = 2;
        pEm->Unk16B = 0;
    case 2:
        Sv.vx = 0;
        Sv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
        Sv.vz = 0;
        RotMatrix(&Sv, &M);
        Sv.vx = -(pEm->Unk16B * 120);
        Sv.vy = -600;
        ApplyMatrixSV(&M, &Sv, &Ofs);
        Sv.vx = -120;
        Sv.vy = -600;
        Sv.vz = 0;
        ApplyMatrixSV(&M, &Sv, &Step);
        if (!(pEm->Type & 1))
            pP = pEm->pSin_parts_ptr;
        else
            pP = &G.pEnemy[EM33_B(pEm)->Oya]->pSin_parts_ptr[EM33_B(pEm)->Parts];
        Sv.vx = pP->Workm.t[0] + Ofs.vx;
        Sv.vy = pP->Workm.t[1] + Ofs.vy;
        Sv.vz = pP->Workm.t[2] + Ofs.vz;
        Esp_call(0x7001900, (0x1280 - pEm->Unk16B * 70 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        pEm->Unk16B++;
        Sv.vx = Step.vx + (pP->Workm.t[0] + Ofs.vx) - (SHORT)(Rnd() - 0x80);
        Sv.vz = Step.vz + (pP->Workm.t[2] + Ofs.vz) - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1300 - pEm->Unk16B * 70 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + (Step.vx << 1);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + (Step.vz << 1);
        Esp_call(0x7001900, (0x1380 - pEm->Unk16B * 128 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 3 - (SHORT)(Rnd() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 3 - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1400 - pEm->Unk16B * 128 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + (Step.vx << 2);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + (Step.vz << 2);
        Esp_call(0x7001900, (0x1480 - pEm->Unk16B * 128 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 5 - (SHORT)(Rnd() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 5 - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1500 - pEm->Unk16B * 128 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 6;
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 6;
        Esp_call(0x7001900, (0x1580 - pEm->Unk16B * 128 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 7 - (SHORT)(Rnd() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 7 - (SHORT)(Rnd() - 0x80);
        Esp_call(0x7001900, (0x1600 - pEm->Unk16B * 128 - Rnd()) << 16, &GsIDMATRIX, &Sv, 0);
        if (pEm->Unk16B++ > 16)
            pEm->Unk16A = 0;
        break;
    }
}

/* set the sub routine of the head and of every segment: 10 at the hit part, +3 per part away from it
   (for a segment, pEm is replaced by its head) */
void func_80106C48(ENEMY_WORK *pEm)
{
    LONG Up;
    LONG Dn;
    LONG Rtn = 10;
    PARTS_W *pP;

    if (pEm->Type & 1) {
        Up = Dn = EM33_B(pEm)->Parts;
        pEm = G.pEnemy[EM33_B(pEm)->Oya];
        if (Dn >= 7)
            Dn = 6;
    } else
        Up = Dn = 0;
    while (Up < 7 || Dn >= 0) {
        if (Up < 7 && Up) {
            pP = &pEm->pSin_parts_ptr[Up];
            EM33_W(G.pEnemy[pP->Unk82])->Rtn = Rtn;
        }
        if (Up == 0)
            EM33_W(pEm)->Rtn = Rtn;
        if (Dn > 0) {
            pP = &pEm->pSin_parts_ptr[Dn];
            EM33_W(G.pEnemy[pP->Unk82])->Rtn = Rtn;
        }
        if (Dn == 0)
            EM33_W(pEm)->Rtn = Rtn;
        Up++;
        Dn--;
        Rtn += 3;
    }
}

/* blood spray while the sub routine counts down (sound at 8) */
void func_80106D64(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    VECTOR Pos;
    ENEMY_WORK *pH;
    LONG n = 1;

    if (EM33_W(pEm)->Rtn) {
        EM33_W(pEm)->Rtn--;
        if (EM33_W(pEm)->Rtn == 1 || EM33_W(pEm)->Rtn == 3 || EM33_W(pEm)->Rtn == 8) {
            if (EM33_W(pEm)->Rtn == 8) {
                if (pEm->Type & 1) {
                    pH = G.pEnemy[EM33_B(pEm)->Oya];
                    Pos.vx = pH->pSin_parts_ptr[EM33_B(pEm)->Parts].Workm.t[0];
                    Pos.vy = pH->pSin_parts_ptr[EM33_B(pEm)->Parts].Workm.t[1];
                    Pos.vz = pH->pSin_parts_ptr[EM33_B(pEm)->Parts].Workm.t[2];
                } else {
                    Pos.vx = pEm->pSin_parts_ptr[8].Workm.t[0];
                    Pos.vy = pEm->pSin_parts_ptr[8].Workm.t[1];
                    Pos.vz = pEm->pSin_parts_ptr[8].Workm.t[2];
                }
                if (Rnd() & 3)
                    Snd_se_stad(0x1020C, &Pos, 0, 0);
                else
                    Snd_se_stad(0x1020D, &Pos, 0, 0);
            }
            while (n--) {
                Sv.vx = (Rnd() & 0xFF) - 0x7F;
                Sv.vy = (Rnd() & 0xFF) - 0x7F;
                Sv.vz = (Rnd() & 0xFF) - 0x7F;
                Sv.vx <<= 2;
                Sv.vy <<= 2;
                Sv.vz <<= 2;
                if (pEm->Type & 1) {
                    pH = G.pEnemy[EM33_B(pEm)->Oya];
                    Esp_call(0x2B, (USHORT)pH->Cdir_y | 0x40000000, &pH->pSin_parts_ptr[EM33_B(pEm)->Parts].Workm, &Sv, 0);
                } else {
                    Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x40000000, &pEm->pSin_parts_ptr[8].Workm, &Sv, 0);
                    Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, 0);
                    Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[12].Workm, &Sv, 0);
                    Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[14].Workm, &Sv, 0);
                    Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[16].Workm, &Sv, 0);
                }
            }
        }
    }
}

/* water splash under part No */
void func_80107074(ENEMY_WORK *pEm, LONG No, LONG Type)
{
    SVECTOR Sv;
    LONG Kind;

    Sv.vx = (Rnd() & 0xFF) - 0x7F;
    Sv.vz = (Rnd() & 0xFF) - 0x7F;
    Sv.vy = -50;
    Sv.vx += pEm->pSin_parts_ptr[No].Workm.t[0];
    Sv.vz += pEm->pSin_parts_ptr[No].Workm.t[2];
    Kind = (Rnd() & 3) << 9;
    Esp_call((Type << 8) | 0x1000003, ((Kind | 0x2000) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Sv, 0);
}

/* Routine_0 3 */
void func_80107160(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM33_W(pEm)->Unk0F = 1;
    EM_RTN(pEm) = pEm->Unk1C4;
    func_80100B5C(pEm, pKan, pSeq);
}

void func_8010718C(ULONG No, ENEMY_WORK *pEm, LONG *pPos)
{
    VECTOR Pos;

    Pos.vx = pPos[0];
    Pos.vy = pPos[1];
    Pos.vz = pPos[2];
    if (pEm->Unk12F == D_800BA6E8[3])
        No += 0x10;
    Snd_se_stad(No | 0x10300, &Pos, 0, 0);
}

void func_80107204(void)
{
    func_8003884C(Em33_vib_tbl, 3);
}

void func_8010722C(void)
{
    func_8003884C(Em33_vib_tbl, 5);
}
