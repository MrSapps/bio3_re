#include "ovl/em30.h"
#include "joint.h"
#include "root_ck.h"
#include "sabun.h"
#include "sce_at.h"
#include "snd.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");          /* em_sub.c, UCHAR result used as int */

LONG func_8004AC54_l(ENEMY_WORK *pEm, PARTS_W *pP) __asm__("func_8004AC54"); /* returns long here (canonical void) */

/* RE2 EM_MAIN */
void func_801000AC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl;
    ENEMY_WORK *pOya;
    VECTOR *pTg;
    tagGLOBAL *pG;
    PARTS_W *pP;
    SVECTOR Unused0;
    VECTOR Tg;
    MATRIX M;
    MATRIX Unused1;
    VECTOR Out;
    SVECTOR Unused2;
    LONG y;

    pPl = G_PLAYER;
    if (!(pEm->Type & 1)) {
        func_801053CC(pEm);
        if ((ULONG)(pEm->Routine_1 - 7) < 2 || (EM_RTN(pEm) & 0xFFFF00) == 0x20300) {
            if (EM30_W(pEm)->Unk19 == 1) {
                Tg.vx = -0x4BD4;
                Tg.vz = -0x2C88;
                if (SquareRoot0((pPl->Pos_x + 0x4BD4) * (pPl->Pos_x + 0x4BD4) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                    Tg.vz = -0x1F40;
                pTg = &Tg;
            } else if (EM30_W(pEm)->Unk19 == 9) {
                Tg.vx = -0x86B;
                Tg.vz = -0x2C88;
                if (SquareRoot0((pPl->Pos_x + 0x86B) * (pPl->Pos_x + 0x86B) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                    Tg.vz = -0x1F40;
                pTg = &Tg;
            } else {
                pTg = (VECTOR *)&pPl->Pos_x;
            }
            if ((EM_RTN(pEm) & 0xFFFF00) == 0x20300 && pEm->Spd_x < 0 &&
                (EM30_W(pEm)->Unk19 == 1 || EM30_W(pEm)->Unk19 == 9))
                pEm->Spd_x = -pEm->Spd_x;
        } else {
            pTg = (VECTOR *)&pPl->Pos_x;
        }
        pEm->Type &= ~0x4000;
        Root_ck(pEm, pTg, 0, 0);
        Em30_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        pEm->pEnemy_ptr = pPl;
        if (pEm->Id != 0x30)
            Rot_neck_em(pEm, pEm->Cdir_y);
        Em_seq_sound(pEm);
        if (EM30_W(pEm)->Unk14)
            func_80104F20(pEm);
        if (pEm->Routine_0 != 3 && !(G.System_flg & 0x800)) {
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
        pG = &G;
        pOya = (&pG->pEnemy[0])[EM30_SUB(pEm)->Oya_no];
        if (pOya->Routine_0 != 3 && !(pG->System_flg & 0x800)) {
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
        }
        if (pEm->Routine_0 == 2)
            Em30_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        else if (pEm->Damage_cnt & 0x7F)
            pEm->Damage_cnt--;
        M = GsIDMATRIX;
        MulMatrix2(pEm->Col.pM, &M);
        pEm->Col.Unk26 = 0;
        ApplyMatrix(&M, (SVECTOR *)&pEm->Col.Unk24, &Out);
        y = pEm->Col.pM->t[1] + Out.vy;
        if (y < 0) {
            pEm->Col.Unk26 = -y;
            pEm->Col.H = 800 - y;
        } else {
            pEm->Col.H = 800;
        }
        pEm->Pos_x = pEm->Col.Pos_x;
        pEm->Pos_y = pEm->Col.Pos_y;
        pEm->Pos_z = pEm->Col.Pos_z;
    }
    func_8010682C(pEm);
    func_80106130(pEm);
}

/* RE2 Em30_Init: the body, its seven collision sub-works and their shadows */
void func_80100544(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    COL_OBJ *pCol;
    PARTS_W *pP;
    ENEMY_WORK *pSub;
    PARTS_W *pBody;
    UCHAR i;

    EM_RTN(pEm) = 1;
    EM30_W(pEm)->Unk13 = 0;
    pBody = pEm->pSin_parts_ptr;
    EM30_W(pEm)->Unk0F = 10;
    EM30_W(pEm)->Unk13 = 1;
    pEm->Unk16B = 0;
    pEm->Unk16A = 0;
    pEm->Damage_cnt = 0;
    EM30_W(pEm)->Unk00 = 0;
    EM30_W(pEm)->Unk01 = 0;
    EM30_W(pEm)->Unk02 = 0;
    EM30_W(pEm)->Unk03 = 0;
    EM30_W(pEm)->Unk0A = 0;
    EM30_W(pEm)->Unk0B = 0;
    EM30_W(pEm)->Unk10 = 0;
    EM30_W(pEm)->Unk19 = 0;
    EM30_W(pEm)->Unk0C = 0;
    EM30_W(pEm)->Unk11 = 0;
    EM30_W(pEm)->Unk12 = 0;
    EM30_W(pEm)->Unk24 = 0;
    EM30_W(pEm)->Unk0F = 0;
    EM30_W(pEm)->Unk14 = 0x210;
    EM30_W(pEm)->Unk16 = 0;
    EM30_W(pEm)->Unk28 = 0;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pBody->Workm.t);
    pEm->Be_flg |= 0x9000;
    pEm->Status_flg |= 0x100;
    pCol = &pEm->Col;
    pCol->W0 = pCol->D0 = 1100;
    pCol->Unk24 = 400;
    pCol->pEm = pEm;
    pCol->H = 800;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    func_80033580(pCol, 0);
    pEm->Unk134 = 1800;
    pEm->Unk136 = -1200;
    pEm->Unk138 = 0x78;
    pEm->Unk139 = 0xB4;
    pEm->Unk13A = 1000;
    pEm->Neck_no = 8;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    EM30_W(pEm)->Unk0E = pEm->Col.Id |= 0x80;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[8];
    PW_FREE(pP)[2] = 0x40;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[3] = 0;
    PW_FREE(pP)[4] = 0x300;
    PW_FREE(pP)[5] = 0;
    pEm->Be_flg |= 0xC00000;
    if (G.System_flg & 0x100) {
        pEm->Life = 600;
        pEm->Max_life = 600;
        EM30_W(pEm)->Unk26 = 600;
    } else {
        pEm->Life = 800;
        pEm->Max_life = 800;
        EM30_W(pEm)->Unk26 = 800;
    }
    pEm->Max_life += 100;
    pEm->Life += 100;
    if (Flg_ck(G.Unk7890, 0xDC))
    {
        pEm->Life *= 3;
        pEm->Life /= 4;
    }
    EM_MOVE(pEm) = 0;
    Joint_move(pEm, pKan, pSeq, 0x400);
    pSub = func_80104E58(pEm);
    pSub->Be_flg |= 0xC00010;
    func_80100B34(pSub, 1, pEm, 8);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[8].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = func_80104E58(pEm);
    pSub->Be_flg |= 0xC00010;
    func_80100B34(pSub, 2, pEm, 1);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[1].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = func_80104E58(pEm);
    pSub->Be_flg |= 0xC00010;
    func_80100B34(pSub, 3, pEm, 2);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x5140514, 0, (VECTOR *)pEm->pSin_parts_ptr[2].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = func_80104E58(pEm);
    pSub->Be_flg |= 0xC00010;
    func_80100B34(pSub, 4, pEm, 3);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = func_80104E58(pEm);
    pSub->Be_flg |= 0xC00010;
    func_80100B34(pSub, 5, pEm, 4);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = func_80104E58(pEm);
    pSub->Be_flg |= 0xC00010;
    func_80100B34(pSub, 6, pEm, 5);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x44C044C, 0, (VECTOR *)pEm->pSin_parts_ptr[5].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = func_80104E58(pEm);
    pSub->Be_flg |= 0xC00010;
    func_80100B34(pSub, 7, pEm, 6);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x3E803E8, 0, (VECTOR *)pEm->pSin_parts_ptr[6].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    G.pPldie_Em[pEm->Id - 0x10] = Em30_Pldie;
    i = 0;
    do {
        pP = pEm->pSin_parts_ptr + i;
        pP->Be_flg |= 0x20;
    } while (++i < 17);
    EM30_W(pEm)->Unk20 = 0;
    EM30_W(pEm)->Unk22 = 0;
    pEm->Add_dir = 0;
    i = 0;
    do {
        pP = pEm->pSin_parts_ptr + i;
        pP->UnkA2 &= ~1;
    } while (++i < 18);
    EM_MOVE(pEm) = 5;
    pEm->Pos_y += 0x1518;
    G.Unk75EA = 0;
}

/* a collision sub-work No following part Parts_no of the body pEm */
void func_80100B34(ENEMY_WORK *pSub, LONG No, ENEMY_WORK *pEm, LONG Parts_no)
{
    COL_OBJ *pCol;

    pCol = &pSub->Col;
    pSub->Type = 1;
    pSub->Unk16B = 0;
    pSub->Unk16A = 0;
    EM30_W(pSub)->Unk0A = 0;
    EM30_W(pSub)->Unk13 = 0;
    EM30_W(pSub)->Unk00 = No;
    pCol->pEm = pSub;
    pCol->W0 = pCol->D0 = Em30_col_w[Parts_no];
    pCol->H = 800;
    if (Parts_no == 8)
        pCol->Unk24 = 600;
    else
        pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    func_80033580(pCol, EM30_W(pEm)->Unk0E);
    pCol->pM = &pEm->pSin_parts_ptr[Parts_no].Workm;
    pSub->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr[Parts_no].Workm;
    pSub->Unk134 = 1800;
    pSub->Unk136 = -1200;
    pSub->Unk138 = 0x78;
    pSub->Unk139 = 0xB4;
    if (Parts_no == 8)
        pSub->Unk13A = 1400;
    else
        pSub->Unk13A = 800;
    pSub->Unk140 = 0;
    pSub->Life = 30000;
    pSub->Max_life = 30000;
    EM30_W(pSub)->Unk26 = 30000;
    EM30_SUB(pSub)->Parts_no = Parts_no;
    EM30_SUB(pSub)->Oya_no = func_800336B4(pEm);
    pEm->pSin_parts_ptr[Parts_no].Unk82 = func_800336B4(pSub);
    pSub->Status_flg |= 0x100;
}

/* RE2 Em30_Move */
void func_80100CBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em30_Move_type(pEm, pKan, pSeq);
}

/* RE2 Em30_Move_type00 */
void Em30_Move_type(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em30_mv_br[pEm->Routine_1](pEm);
    Em30_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* mv_br[0] */
void func_80100D78(ENEMY_WORK *pEm)
{
    EM30_W(pEm)->Unk1A = 0;
    pEm->Timer0 = 60;
    func_8010258C(pEm);
    EM_RTN(pEm) = 0x30301;
}

void func_80100DB4(void) {
}

/* mv_br[1]: decide the next move */
void func_80100DBC(ENEMY_WORK *pEm)
{
    UCHAR Se[2] = {9, 6};

    if ((LONG)pEm->Unk154 < 4500 && !EM30_W(pEm)->Unk03)
        EM_RTN(pEm) = 0x601;
    if ((LONG)pEm->Unk154 < 4500 && !EM30_W(pEm)->Unk02)
        EM_RTN(pEm) = 0x201;
    if ((LONG)pEm->Unk154 > 7000 && (LONG)pEm->Unk154 < 8000 && (Pl_work.Life > 80 || !EM30_W(pEm)->Unk02))
        EM_RTN(pEm) = 0x401;
    if ((LONG)pEm->Unk154 < 5000) {
        if (Pl_work.Life < 80 && !((Rnd_l() >> (Rnd_l() & 3)) & 3)) {
            EM_RTN(pEm) = 0x501;
            if (!EM30_W(pEm)->Unk02)
                EM_RTN(pEm) = 0x201;
        } else if (EM30_W(pEm)->Unk02) {
            EM_RTN(pEm) = 0x401;
        }
    }
    if ((Pl_work.Col.Unk3A & 0x3FFF) || (LONG)pEm->Unk154 < 1200) {
        EM30_W(pEm)->Unk1A++;
        if (EM30_W(pEm)->Unk04 >= 2)
            EM30_W(pEm)->Unk04 -= 2;
        if (EM30_W(pEm)->Unk1A > 60) {
            EM30_W(pEm)->Unk1A = 0;
            EM_RTN(pEm) = 0x301;
        }
    } else {
        EM30_W(pEm)->Unk1A = 0;
    }
    if (!EM30_W(pEm)->Unk20) {
        Snd_se_enem(Se[EM30_W(pEm)->Unk0B], pEm);
        EM30_W(pEm)->Unk20 = 25;
    } else {
        EM30_W(pEm)->Unk20--;
    }
}

/* mv[1]: crawl towards the target */
void func_80100FA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[4] = {0, 1, 2, 3};
    UCHAR Se[2] = {9, 6};

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Mot[EM30_W(pEm)->Unk03 * 2 + EM30_W(pEm)->Unk02] + 0x1F0000;
        pEm->Routine_2 = 1;
        EM30_W(pEm)->Unk04 = 40;
        EM30_W(pEm)->Unk06 = 80;
        EM30_W(pEm)->Unk08 = 2;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Joint_move(pEm, pKan, pSeq, 0x80);
        pEm->Spd_x = EM30_W(pEm)->Unk04;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Unk04 += EM30_W(pEm)->Unk08;
        if (EM30_W(pEm)->Unk08 > 0 && EM30_W(pEm)->Unk04 > EM30_W(pEm)->Unk06)
            EM30_W(pEm)->Unk04 = EM30_W(pEm)->Unk06;
        func_80104920(pEm);
    }
}

void func_801010F4(void) {
}

/* mv[2] */
void func_801010FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = {0xB, 0xA};
    SVECTOR Sv;
    UCHAR Se[2] = {9, 6};
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM30_W(pEm)->Unk03 = Rnd_l() & 1;
        if (EM30_W(pEm)->Unk02 && EM30_W(pEm)->Unk03)
            EM30_W(pEm)->Unk03 = 0;
        EM_MOVE(pEm) = Mot[EM30_W(pEm)->Unk03] + 0x1F0000;
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
        if (Joint_move(pEm, pKan, pSeq, (EM30_W(pEm)->Unk02 << 16) | 0x80)) {
            EM30_W(pEm)->Unk02 ^= 1;
            EM_RTN(pEm) = 0x101;
            if (EM30_W(pEm)->Unk02) {
                Snd_se_enem(4, pEm);
                Sv.vx = 850;
                Sv.vy = 660;
                Sv.vz = -420;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x14000000, &GsIDMATRIX, &Sv, NULL);
                Sv.vx = 850;
                Sv.vy = 670;
                Sv.vz = -440;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x12000000, &GsIDMATRIX, &Sv, NULL);
            }
        }
        pEm->Spd_x = 5;
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        func_80104920(pEm);
    }
    if (!EM30_W(pEm)->Unk20) {
        Snd_se_enem(Se[EM30_W(pEm)->Unk0B], pEm);
        EM30_W(pEm)->Unk20 = 25;
    } else {
        EM30_W(pEm)->Unk20--;
    }
}

void func_80101380(void) {
}

/* mv[3]: dive into the ground and come up again somewhere else */
void func_80101388(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    VECTOR Pos;
    UCHAR Se[2] = {9, 6};
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG i;
    LONG Flg;
    LONG Zone;
    LONG Dist;
    LONG n;

    switch (pEm->Routine_2) {
    case 0:
        EM30_W(pEm)->Unk0B = 1;
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = 0x1F0004;
        pEm->Spd_x = 10;
        pEm->Spd_z = 0;
        pP = pEm->pSin_parts_ptr;
        for (i = 17; i != 0; i--) {
            pP->UnkA2 |= 0x400;
            pP++;
        }
        EM30_W(pEm)->Unk18 = 8;
        if (G.System_flg & 0x100)
            EM30_W(pEm)->Unk30 = (Rnd_l() & 1) + 2;
        else
            EM30_W(pEm)->Unk30 = (Rnd_l() & 1) + (Rnd_l() & 1) + 3;
        pEm->UnkE2 = 0;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pP = &pEm->pSin_parts_ptr[5];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pP = &pEm->pSin_parts_ptr[6];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pEm->Routine_2 = 2;
            pEm->Timer0 = 50;
            EM_MOVE(pEm) = 0;
            pEm->Spd_x = 150;
            pEm->Spd_z = 0;
            pEm->Pos_y += 0x1518;
            G.Status_flg &= ~4;
            if (++pEm->Add_dir > 100)
                pEm->Add_dir = 100;
        }
        Add_speedXZ(pEm, 0);
        func_80104920(pEm);
        if (pEm->Move_cnt == 60) {
            func_8003884C(Em30_vib_tbl, 4);
            G.Status_flg |= 4;
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 &= ~1;
        }
        if (pEm->Move_cnt == 70) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 &= ~1;
            EM30_W(pEm)->Unk18 = 0;
            pEm->Be_flg |= 0x20;
        }
        if (pEm->Move_cnt == 77) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Unk18 = 1;
        }
        if (pEm->Move_cnt == 86) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Unk18 = 2;
            Snd_se_enem(10, pEm);
        }
        if (pEm->Move_cnt == 96) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Unk18 = 3;
        }
        if (pEm->Move_cnt == 40) {
            func_80102250(pEm, EM30_W(pEm)->Unk18, 100);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 150);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 200);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 250);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 300);
            Snd_se_enem(7, pEm);
            Snd_se_enem(10, pEm);
        }
        if (pEm->Move_cnt >= 41)
            func_80102250(pEm, EM30_W(pEm)->Unk18, 100);
        break;
    case 2:
        EM30_W(pEm)->Unk11 = 0;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (!pEm->Timer0) {
            Flg = 0;
            if (Flg_ck(&G.Unk7888, 4))
                Flg = pEm->Pos_z < -15000;
            if (EM30_W(pEm)->Unk30) {
                EM30_W(pEm)->Unk19 = 0;
                EM30_W(pEm)->Unk24 = 0;
                EM_RTN(pEm) = 0x40701;
            } else {
                if ((LONG)pEm->Unk154 > 2500 && !Flg) {
                    pEm->Routine_2 = 3;
                    EM_MOVE(pEm) = 5;
                    EM30_W(pEm)->Unk18 = 8;
                    pEm->Timer0 = 45;
                } else {
                    if (EM30_W(pEm)->Unk19 != 1 && EM30_W(pEm)->Unk19 != 9) {
                        if (pEm->Spd_x < 0) {
                            if (pEm->Spd_x > -100)
                                pEm->Spd_x = -300;
                        } else if (pEm->Spd_x < 100) {
                            pEm->Spd_x = 300;
                        }
                        Dist = pEm->Unk154;
                        if (Dist < 2500)
                            Zone = 1;
                        else
                            Zone = (Dist > 6500) * 2;
                        if ((ULONG)(Dist - 2500) > 4000 && Zone == EM30_W(pEm)->Unk31) {
                            if (Dist > 6500) {
                                pEm->Spd_x += (Rnd_l() & 0x1F) - 15;
                                if ((LONG)pEm->Unk154 >= EM30_W(pEm)->Unk2E)
                                    pEm->Spd_x = -pEm->Spd_x;
                            }
                            if ((LONG)pEm->Unk154 < 2500) {
                                pEm->Spd_x += (Rnd_l() & 0x1F) - 15;
                                if (EM30_W(pEm)->Unk2E >= (LONG)pEm->Unk154)
                                    pEm->Spd_x = -pEm->Spd_x;
                            }
                        }
                        EM30_W(pEm)->Unk31 = Zone;
                        EM30_W(pEm)->Unk2E = pEm->Unk154;
                    }
                    pEm->Timer0 = (Rnd_l() + 30) & 0x1F;
                }
            }
        } else {
            pEm->Timer0--;
        }
        func_80104920(pEm);
        if (!EM30_W(pEm)->Unk20) {
            Snd_se_enem(Se[EM30_W(pEm)->Unk0B], pEm);
            EM30_W(pEm)->Unk20 = 25;
        } else {
            EM30_W(pEm)->Unk20--;
        }
        break;
    case 3:
        if (pEm->Timer0 == 45) {
            Pos.vx = pEm->Pos_x;
            Pos.vy = 0;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x10210, &Pos, 0, 0);
            G.Status_flg |= 2;
        }
        if (pEm->Timer0 == 42)
            G.Status_flg &= ~2;
        if (pEm->Timer0) {
            if (pEm->Timer0 >= 39 && pEm->Timer0 < 45)
                func_801023F4(pEm, EM30_W(pEm)->Unk18, 100);
            else if (pEm->Timer0 < 10)
                func_80102250(pEm, EM30_W(pEm)->Unk18, 100);
            pEm->Timer0--;
        } else {
            pEm->Pos_y -= 0x1518;
            Joint_move(pEm, pKan, pSeq, 0x80);
            Snd_se_enem(8, pEm);
            Snd_se_enem(10, pEm);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 100);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 150);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 200);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 250);
            func_80102250(pEm, EM30_W(pEm)->Unk18, 300);
            pEm->Spd_x = 40;
            pEm->Timer0 = 0;
            pEm->UnkE2 = 0;
            pEm->Routine_2++;
            func_8003884C(Em30_vib_tbl, 3);
            G.Status_flg |= 4;
        }
        break;
    case 4:
        if (pEm->Move_cnt < 10)
            func_8010252C(pEm, 14);
        func_80102250(pEm, EM30_W(pEm)->Unk18, 100);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM30_W(pEm)->Unk0B = 0;
            EM30_W(pEm)->Unk03 = 0;
            EM_RTN(pEm) = 0x101;
            pP = pEm->pSin_parts_ptr;
            for (i = 17; i != 0; i--) {
                pP->UnkA2 &= ~0x400;
                pP++;
            }
            G.Status_flg &= ~4;
        }
        Add_speedXZ(pEm, 0);
        func_80104920(pEm);
        if (pEm->Move_cnt == 2) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 6) {
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
        }
        if (pEm->Move_cnt == 7) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 1;
            EM30_W(pEm)->Unk18 = 0;
            pEm->Be_flg &= ~0x20;
        }
        if (pEm->Move_cnt == 15) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 1;
        }
        if (pEm->Move_cnt == 34) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 2;
        }
        if (pEm->Move_cnt == 35)
            Snd_se_enem(4, pEm);
        if (pEm->Move_cnt == 50) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 3;
        }
        if (pEm->Move_cnt == 71) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 5;
        }
        if (pEm->Move_cnt == 40 || pEm->Move_cnt == 80)
            Snd_se_enem(10, pEm);
        if (pEm->Move_cnt == 99) {
            pP = &pEm->pSin_parts_ptr[6];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 6;
        }
        if (pEm->Move_cnt >= 31) {
            Sv.vx = 600;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 1400;
            if (!pPl->Damage_cnt && func_80044420(&pEm->pSin_parts_ptr[9].Workm, &Sv, pPl) && !pEm->Timer0) {
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                pEm->Timer0 = 1;
                pEm->UnkE2 = 5;
            }
        }
        if (pEm->UnkE2) {
            n = pEm->UnkE2;
            pPl->Spd_x = Cdir_ck_l(pEm, pPl) ? n * 20 : -(n * 20);
            pPl->Spd_z = 0;
            Add_speedXZ(pPl, 0);
            pEm->UnkE2--;
        }
        break;
    }
}

/* dirt (or a splash in water) around part No */
void func_80102250(ENEMY_WORK *pEm, LONG No, LONG Big)
{
    SVECTOR Sv;
    LONG Col;
    LONG Kind;
    LONG Size;

    Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
    Sv.vy = -((Rnd_l() & 0xFF) + Big);
    Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
    Sv.vx <<= 3;
    Sv.vz <<= 3;
    Col = (Rnd_l() & 0xFF) << 4;
    if (Big)
        Kind = Rnd_l() & 0xF;
    else
        Kind = (Rnd_l() & 3) + (Rnd_l() & 1);
    Size = ((Rnd_l() & 3) + 2) * 5;
    Sv.vx += pEm->pSin_parts_ptr[No].Workm.t[0];
    Sv.vz += pEm->pSin_parts_ptr[No].Workm.t[2];
    if (Water_ck(Sv.vx, Sv.vz)) {
        Esp_call(0xE1C, (Size << 24) | Col, &GsIDMATRIX, &Sv, NULL);
    } else {
        Esp_call((Kind << 8) | 0x36, (Size << 24) | Col, &GsIDMATRIX, &Sv, NULL);
        Esp_call(0x4003103, (Size << 24) | Col, &GsIDMATRIX, &Sv, NULL);
    }
}

/* dirt effect around part No; Big: larger and higher */
void func_801023F4(ENEMY_WORK *pEm, LONG No, LONG Big)
{
    SVECTOR Sv;
    LONG Size;
    LONG Col;

    Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
    Sv.vy = -((Rnd_l() & 0xFF) + Big);
    Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
    if (Big) {
        Sv.vz <<= 3;
        Sv.vx <<= 3;
        Size = ((Rnd_l() & 3) + 2) * 5 << 8;
    } else {
        Sv.vz <<= 2;
        Sv.vx <<= 2;
        Size = ((Rnd_l() & 3) + 2) * 3 << 8;
    }
    Col = (Rnd_l() & 0xFF) << 4;
    Sv.vx += pEm->pSin_parts_ptr[No].Workm.t[0];
    Sv.vz += pEm->pSin_parts_ptr[No].Workm.t[2];
    Esp_call(0x4003103, (Size << 16) | Col, &GsIDMATRIX, &Sv, NULL);
}

/* blood effect on part No */
void func_8010252C(ENEMY_WORK *pEm, LONG No)
{
    SVECTOR Sv;

    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x36, (USHORT)pEm->Cdir_y | 0x14000000, &pEm->pSin_parts_ptr[No].Workm, &Sv, NULL);
}

/* flag 0x400 on every part but the odd ones from 9 on */
void func_8010258C(ENEMY_WORK *pEm)
{
    LONG i;
    PARTS_W *pP;

    for (i = 0; i < 17; i++) {
        if (i >= 9 && (i & 1))
            i++;
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 |= 0x400;
    }
}

/* the body comes out: parts 0..8 shown (dirt where a part breaks the surface) and their shadows on */
void func_801025E0(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[1];
    pEm->Be_flg &= ~0x20;
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 1, 100);
        func_801023F4(pEm, 1, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[2];
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 2, 100);
        func_801023F4(pEm, 2, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[3];
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 3, 100);
        func_801023F4(pEm, 3, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[4];
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 4, 100);
        func_801023F4(pEm, 4, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[5];
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 5, 100);
        func_801023F4(pEm, 5, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[6];
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 6, 100);
        func_801023F4(pEm, 6, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[7];
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 7, 100);
        func_801023F4(pEm, 7, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[8];
    if (!(pP->UnkA2 & 1)) {
        func_801023F4(pEm, 8, 100);
        func_801023F4(pEm, 8, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[10];
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[12];
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[14];
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[16];
    pP->UnkA2 |= 1;
}

void func_801029AC(void) {
}

/* mv[4]: bite at the player (Unk02: second bite) */
void func_801029B4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    UCHAR Mot[2] = {0xC, 0xD};
    UCHAR Se[2] = {9, 6};
    HIT_AREA Area;
    ENEMY_WORK *pPl = &Pl_work;
    LONG n;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Mot[EM30_W(pEm)->Unk02] + 0x1F0000;
        pEm->Routine_2 = 1;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Spd_x = 100;
        pEm->Spd_z = 0;
        if (!EM30_W(pEm)->Unk02) {
            EM30_W(pEm)->Unk06 = 200;
            EM30_W(pEm)->Unk04 = 100;
            EM30_W(pEm)->Unk08 = 10;
        }
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if (EM30_W(pEm)->Unk02 == 1) {
                EM30_W(pEm)->Unk02 = 0;
                EM_RTN(pEm) = 0x301;
            } else {
                EM_RTN(pEm) = 0x101;
            }
        }
        if ((pEm->Move_cnt == 24 || pEm->Move_cnt == 42) && !pEm->UnkDA)
            Snd_se_enem(0, pEm);
        if ((*pEm->pNow_seq >> 10) & 1) {
            if (EM30_W(pEm)->Unk02 == 1) {
                Sv.vx = 2000;
                Sv.vy = 0;
                Sv.vz = 1400;
                Sv.pad = 1400;
            } else {
                Sv.vx = 1800;
                Sv.vy = 0;
                Sv.vz = 1400;
                Sv.pad = 1400;
            }
            Area.pPos = (VECTOR *)pEm->pSin_parts_ptr[8].Workm.t;
            Area.pBox = (SHORT *)&Sv;
            Area.Dir = pEm->Cdir_y;
            Area.Id = 0x85;
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        }
        if ((*pEm->pNow_seq >> 9) & 1) {
            Sv.vx = 400;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 800;
            if (!pPl->Damage_cnt && func_80044420(&pEm->pSin_parts_ptr[9].Workm, &Sv, pPl) && !pEm->UnkDA) {
                Snd_se_enem(1, pEm);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl) + 2;
                pPl->Routine_2 = 0;
                pEm->UnkDA = 1;
                if (Pl_life_down(35, 0) & 2)
                    EM_RTN(&Pl_work) = 3;
                pEm->UnkE2 = 5;
            }
        }
        if (!EM30_W(pEm)->Unk02) {
            if (pEm->Move_cnt == 12) {
                EM30_W(pEm)->Unk08 = -20;
                EM30_W(pEm)->Unk06 = 100;
            }
            if (pEm->Move_cnt == 28) {
                if (pEm->UnkDA) {
                    EM30_W(pEm)->Unk02 = 0;
                    EM_RTN(pEm) = 0x301;
                }
                EM30_W(pEm)->Unk08 = 40;
                EM30_W(pEm)->Unk06 = 350;
            }
            if (pEm->Move_cnt >= 29 && pEm->UnkDA) {
                EM30_W(pEm)->Unk08 = -50;
                EM30_W(pEm)->Unk06 = 50;
            }
            if (pEm->Move_cnt == 38 && !pEm->UnkDA) {
                EM30_W(pEm)->Unk08 = -50;
                EM30_W(pEm)->Unk06 = 100;
            }
            pEm->Spd_x = EM30_W(pEm)->Unk04;
            EM30_W(pEm)->Unk04 += EM30_W(pEm)->Unk08;
            if (EM30_W(pEm)->Unk08 > 0) {
                if (EM30_W(pEm)->Unk04 > EM30_W(pEm)->Unk06)
                    EM30_W(pEm)->Unk04 = EM30_W(pEm)->Unk06;
            } else if (EM30_W(pEm)->Unk04 < EM30_W(pEm)->Unk06) {
                EM30_W(pEm)->Unk04 = EM30_W(pEm)->Unk06;
            }
        }
        Add_speedXZ(pEm, 0);
        func_80104920(pEm);
        if (pEm->UnkE2) {
            EM30_W(pEm)->Unk04 = 10;
            EM30_W(pEm)->Unk08 = 0;
            Sv.vx = Rnd_l() & 0x3F;
            Sv.vy = 150;
            Sv.vz = 0;
            Esp_call(0, (USHORT)pPl->Cdir_y | 0x14000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, NULL);
            n = pEm->UnkE2;
            pPl->Spd_x = Cdir_ck_l(pEm, pPl) ? n * 30 : -(n * 30);
            pPl->Spd_z = 0;
            Add_speedXZ(pPl, 0);
            pEm->UnkE2--;
        }
    }
    if (!EM30_W(pEm)->Unk20) {
        Snd_se_enem(Se[EM30_W(pEm)->Unk0B], pEm);
        EM30_W(pEm)->Unk20 = 25;
    } else {
        EM30_W(pEm)->Unk20--;
    }
}

void func_80102E68(void) {
}

/* mv[5]: snap up the player in front (or miss) */
void func_80102E70(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x1F000E;
        pEm->Routine_2 = 1;
        pEm->UnkDA = 0;
        pEm->Spd_x = 100;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            pEm->Routine_2 = 4;
        Sv.vx = 800;
        Sv.vy = 0;
        Sv.vz = 0;
        Sv.pad = 1000;
        if (func_80044420(&pEm->pSin_parts_ptr[8].Workm, &Sv, pPl) && !pEm->UnkDA)
            pEm->UnkDA = 1;
        if (pEm->Move_cnt == 42) {
            if (pEm->UnkDA) {
                Snd_se_enem(1, pEm);
                pPl->Be_flg |= 0x800;
                pPl->Damage_cnt |= 0x80;
                pEm->Be_flg |= 0x800;
                pEm->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[8].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[1].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[2].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[3].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[4].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[6].Unk82]->Damage_cnt |= 0x80;
                pPl->Routine_0 = 6;
                pPl->Routine_1 = 0;
                pPl->Routine_2 = 0;
                pPl->pEnemy_ptr = pEm;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                pPl->Unk120 |= 1;
                G.System_flg |= 0x20800;
                Pad_vib_set0(4, 0);
                Pad_vib_set0(4, 0x1B);
                Pad_vib_fade_set(0x1A, 0xE4, 0x3C, 0);
                Pad_vib_fade_set(0x1C, 0xDC, 0xC, 0x1B);
                Pad_vib_fade_set(0xC, 0xB8, 0xD8, 0x3D);
                Pad_vib_fade_set(0x2A, 0xD8, 0x10, 0x49);
            } else {
                pEm->Routine_2++;
            }
        }
        if (pEm->UnkDA && ++pEm->UnkDA < 25) {
            Sv.vx = 700;
            Sv.vy = Rnd_l() - 0x80;
            Sv.vz = Rnd_l() - 0x80;
            Esp_call(0, ((((Rnd_l() & 7) << 7) + 0x1400) << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[8].Workm, &Sv, NULL);
        }
        Add_speedXZ(pEm, 0);
    case 4:
        func_80104920(pEm);
        break;
    case 2:
        EM_MOVE(pEm) = 0x1F000F;
        pEm->Routine_2 = 3;
        pEm->UnkDA = 0;
        pEm->Spd_x = 100;
        pEm->Spd_z = 0;
    case 3:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM30_W(pEm)->Unk02 = 0;
            EM_RTN(pEm) = 0x101;
        }
        Add_speedXZ(pEm, 0);
        func_80104920(pEm);
    }
}

void func_801032D4(void) {
}

/* mv[6]: lunge, snapping the jaws (with dirt) at frame 55 */
void func_801032DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = {9, 10};
    SVECTOR Sv;
    UCHAR Se[2] = {9, 6};

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Mot[EM30_W(pEm)->Unk02] + 0x1F0000;
        pEm->Routine_2 = 1;
        EM30_W(pEm)->Unk04 = 40;
        EM30_W(pEm)->Unk08 = -5;
        pEm->Timer0 = EM30_W(pEm)->Unk03;
        EM30_W(pEm)->Unk06 = 0;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        if (pEm->Move_cnt == 55) {
            if (EM30_W(pEm)->Unk03 ^= 1) {
                Snd_se_enem(4, pEm);
                Sv.vx = 850;
                Sv.vy = 660;
                Sv.vz = -420;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x14000000, &GsIDMATRIX, &Sv, NULL);
                Sv.vy = 670;
                Sv.vx = 850;
                Sv.vz = -440;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x12000000, &GsIDMATRIX, &Sv, NULL);
            }
        }
        if (pEm->Move_cnt >= 56)
            func_8010532C(pEm, 0x80, EM30_W(pEm)->Unk03);
        if (Joint_move(pEm, pKan, pSeq, (pEm->Timer0 << 16) | 0x80))
            EM_RTN(pEm) = 0x101;
        pEm->Spd_x = EM30_W(pEm)->Unk04;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Unk04 += EM30_W(pEm)->Unk08;
        if (EM30_W(pEm)->Unk08 > 0) {
            if (EM30_W(pEm)->Unk04 > EM30_W(pEm)->Unk06)
                EM30_W(pEm)->Unk04 = EM30_W(pEm)->Unk06;
            if (!EM30_W(pEm)->Unk20) {
                Snd_se_enem(Se[EM30_W(pEm)->Unk0B], pEm);
                EM30_W(pEm)->Unk20 = 25;
            } else {
                EM30_W(pEm)->Unk20--;
            }
        } else if (EM30_W(pEm)->Unk04 < EM30_W(pEm)->Unk06) {
            EM30_W(pEm)->Unk04 = EM30_W(pEm)->Unk06;
        }
        func_80104920(pEm);
    }
}

/* mv_br[8]: growl, then go back to mv 8 after a while */
void func_801035DC(ENEMY_WORK *pEm)
{
    UCHAR Se[2] = {9, 6};

    if (pEm->Routine_2 == 4) {
        if (!EM30_W(pEm)->Unk20) {
            Snd_se_enem(Se[EM30_W(pEm)->Unk0B], pEm);
            EM30_W(pEm)->Unk20 = 25;
        } else {
            EM30_W(pEm)->Unk20--;
        }
    }
    if (EM30_W(pEm)->Unk24 < 500)
        EM30_W(pEm)->Unk24++;
    else if (pEm->Routine_2 == 4)
        EM_RTN(pEm) = 0x801;
}

/* mv[7]: wait underground, then burst out under the player */
void func_80103684(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    VECTOR Unused;
    HIT_AREA Area;
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG i;
    LONG Dist;
    LONG Zone;
    LONG Flg;
    LONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = (Rnd_l() + 15) & 0xF;
        EM30_W(pEm)->Unk2E = pEm->Unk154;
        {
            LONG Area_no;

            if ((LONG)pEm->Unk154 >= 4500)
                Area_no = ((LONG)pEm->Unk154 > 10000) * 2;
            else
                Area_no = 1;
            EM30_W(pEm)->Unk31 = Area_no;
        }
        EM30_W(pEm)->Unk11 = 0;
        pEm->Routine_2++;
    case 1:
        if (pEm->Timer0 == 15)
            G.Status_flg |= 2;
        if (pEm->Timer0 == 5)
            G.Status_flg &= ~2;
        if (pEm->Timer0 >= 9)
            func_801023F4(pEm, EM30_W(pEm)->Unk18, 100);
        if (!pEm->Timer0 || !--pEm->Timer0) {
            pEm->UnkE3 = 0;
            pEm->Routine_2++;
        }
        break;
    case 2:
        EM_MOVE(pEm) = 0x1F0011;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Routine_2++;
        pEm->Pos_y -= 0x1518;
        Snd_se_enem(8, pEm);
        Snd_se_enem(10, pEm);
        func_80102250(pEm, EM30_W(pEm)->Unk18, 100);
        func_80102250(pEm, EM30_W(pEm)->Unk18, 150);
        func_80102250(pEm, EM30_W(pEm)->Unk18, 200);
        func_80102250(pEm, EM30_W(pEm)->Unk18, 250);
        func_80102250(pEm, EM30_W(pEm)->Unk18, 300);
        for (i = 0; i < 17; i++) {
            pP = &pEm->pSin_parts_ptr[i];
            pP->Be_flg &= ~2;
        }
        EM30_W(pEm)->Unk18 = 8;
        func_8003884C(Em30_vib_tbl, 2);
    case 3:
        if (pEm->Move_cnt < 10)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
        func_80102250(pEm, EM30_W(pEm)->Unk18, 100);
        if (pEm->Move_cnt == 0) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 4) {
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
        }
        if (pEm->Move_cnt == 7) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 1;
            EM30_W(pEm)->Unk18 = 0;
            pEm->Be_flg &= ~0x20;
        }
        if (pEm->Move_cnt == 8)
            Snd_se_enem(4, pEm);
        if (pEm->Move_cnt == 11) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 1;
        }
        if (pEm->Move_cnt == 17) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 2;
        }
        if (pEm->Move_cnt == 30) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Unk18 = 3;
        }
        if (pEm->Move_cnt == 49) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Unk18 = 2;
        }
        if (pEm->Move_cnt == 51) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Unk18 = 1;
            Snd_se_enem(10, pEm);
        }
        if (pEm->Move_cnt == 54) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Unk18 = 0;
        }
        if (pEm->Move_cnt == 61) {
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Unk18 = 8;
        }
        if (pEm->Move_cnt == 63) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 &= ~1;
            pEm->Be_flg |= 0x20;
        }
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 &= ~1;
            pEm->Pos_y += 0x1518;
            if (--EM30_W(pEm)->Unk30) {
                EM_MOVE(pEm) = 0;
                EM30_W(pEm)->Unk10 = Rnd_l() & 3;
                pEm->Routine_2++;
            } else {
                pEm->Timer0 = 90;
                EM_RTN(pEm) = 0x20301;
                EM30_W(pEm)->Unk19 = 0;
                EM_MOVE(pEm) = 0;
                pEm->Spd_x = 150;
                pEm->Spd_z = 0;
                G.Status_flg &= ~4;
            }
        }
        if (pEm->Move_cnt && !pEm->UnkDA) {
            if (pEm->Move_cnt < 7) {
                Sv.vx = 3000;
                Sv.vz = 1700;
                Sv.vy = 0;
                Sv.pad = 1500;
                Area.pPos = (VECTOR *)pEm->pSin_parts_ptr[8].Workm.t;
                Area.pBox = (SHORT *)&Sv;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x85;
                func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
            } else if (pEm->Move_cnt < 18) {
                Sv.vx = 2000;
                Sv.vz = 1200;
                Sv.vy = 0;
                Sv.pad = 1400;
                Area.pPos = (VECTOR *)pEm->pSin_parts_ptr[8].Workm.t;
                Area.pBox = (SHORT *)&Sv;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x85;
                func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
            }
        }
        if (pEm->Move_cnt >= 8 && pEm->Move_cnt < 22) {
            Sv.vx = 1000;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 950;
            if (func_80044420(&pEm->pSin_parts_ptr[8].Workm, &Sv, pPl) && !pPl->Damage_cnt && !pEm->UnkDA) {
                Snd_se_enem(1, pEm);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl) + 2;
                pPl->Routine_2 = 0;
                pEm->UnkDA = 1;
                if (Pl_life_down(50, 0) & 2)
                    EM_RTN(&Pl_work) = 3;
                pEm->UnkE2 = 5;
            }
        }
        Add_speedXZ(pEm, 0);
        if (pEm->UnkE2) {
            Sv.vx = Rnd_l() & 0x3F;
            Sv.vy = 150;
            Sv.vz = 0;
            if (pEm->UnkE2 == 5)
                Esp_call(0x700, (USHORT)pPl->Cdir_y | 0x14000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, NULL);
            Esp_call(0, (USHORT)pPl->Cdir_y | 0x18000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, NULL);
            n = pEm->UnkE2;
            pPl->Spd_x = Cdir_ck_l(pEm, pPl) ? n * 100 : -(n * 100);
            pPl->Spd_z = 0;
            Add_speedXZ(pPl, 0);
            pEm->UnkE2--;
        }
        break;
    case 4:
        Dist = pEm->Unk154;
        if (Dist > 8000 || (!pEm->Timer0 && ((EM30_W(pEm)->Unk0F == 1 && Dist < 2000) || (EM30_W(pEm)->Unk0F == 9 && (LONG)pEm->Unk154 < 2000)))) {
            if (pEm->Spd_x < 0)
                pEm->Spd_x = -pEm->Spd_x;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (!pEm->Timer0) {
            Flg = Flg_ck(&G.Unk7888, 4) != 0;
            if ((LONG)pEm->Unk154 > 3450 && (LONG)pEm->Unk154 < 5000 && !Flg && !pPl->Damage_cnt) {
                pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
                pEm->Routine_2 = 0;
            } else if (Flg_ck(&G.Unk7888, 4)) {
                EM_RTN(pEm) = 0x801;
                break;
            } else {
                if ((LONG)pEm->Unk154 < 3450) {
                    EM30_W(pEm)->Unk11++;
                } else {
                    EM30_W(pEm)->Unk11 = 0;
                    EM30_W(pEm)->Unk19 = 0;
                }
                if (EM30_W(pEm)->Unk19 == 1 || EM30_W(pEm)->Unk19 == 9) {
                    if (pEm->Spd_x < 0)
                        pEm->Spd_x = -pEm->Spd_x;
                } else {
                    if (pEm->Spd_x < 0) {
                        if (pEm->Spd_x > -100)
                            pEm->Spd_x = -300;
                    } else if (pEm->Spd_x < 100) {
                        pEm->Spd_x = 300;
                    }
                    Dist = pEm->Unk154;
                    if (Dist < 3450)
                        Zone = 1;
                    else
                        Zone = (Dist > 5000) * 2;
                }
                if (!EM30_W(pEm)->Unk19) {
                    Dist = pEm->Unk154;
                    if ((ULONG)(Dist - 3450) > 1550 && Zone == EM30_W(pEm)->Unk31) {
                        if (Dist > 5000) {
                            pEm->Spd_x += (Rnd_l() & 0x1F) - 15;
                            if ((LONG)pEm->Unk154 >= EM30_W(pEm)->Unk2E)
                                pEm->Spd_x = -pEm->Spd_x;
                        }
                        if ((LONG)pEm->Unk154 < 3450) {
                            pEm->Spd_x += (Rnd_l() & 0x1F) - 15;
                            if (EM30_W(pEm)->Unk2E >= (LONG)pEm->Unk154)
                                pEm->Spd_x = -pEm->Spd_x;
                        }
                    }
                    EM30_W(pEm)->Unk31 = Zone;
                    EM30_W(pEm)->Unk2E = pEm->Unk154;
                }
                if (Rnd_l() & 3)
                    pEm->Timer0 = (Rnd_l() + 30) & 0x1F;
                else
                    pEm->Timer0 = (Rnd_l() + 10) & 0xF;
                pEm->UnkE3 = 30;
                EM30_W(pEm)->Unk10 = Rnd_l() & 3;
            }
        } else {
            pEm->Timer0--;
            if (pEm->Timer0 >= 36 && pEm->Timer0 < 40) {
                if (EM30_W(pEm)->Unk10)
                    func_80102250(pEm, EM30_W(pEm)->Unk18, 0);
                func_801023F4(pEm, EM30_W(pEm)->Unk18, 0);
            if (pEm->Timer0 == 39)
                G.Status_flg |= 4;
            if (pEm->Timer0 == 36)
                G.Status_flg &= ~4;
            }
            if (pEm->Timer0 >= 16 && pEm->Timer0 < 20) {
                if (EM30_W(pEm)->Unk10) {
                    func_80102250(pEm, EM30_W(pEm)->Unk18, 0);
                    func_801023F4(pEm, EM30_W(pEm)->Unk18, 0);
                }
            if (pEm->Timer0 == 19)
                G.Status_flg |= 4;
            if (pEm->Timer0 == 16)
                G.Status_flg &= ~4;
            }
            if (pEm->Timer0 < 10) {
                if (EM30_W(pEm)->Unk10) {
                    func_80102250(pEm, EM30_W(pEm)->Unk18, 0);
                    func_801023F4(pEm, EM30_W(pEm)->Unk18, 0);
                }
            if (pEm->Timer0 == 9)
                G.Status_flg |= 4;
            if (pEm->Timer0 == 6)
                G.Status_flg &= ~4;
            }
        }
        func_80104920(pEm);
        break;
    case 5:
        break;
    }
}

/* mv_br[8]: wait at the hole until the em30 is back at it */
void func_80104590(ENEMY_WORK *pEm)
{
    UCHAR Se[2] = {9, 6};
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Tg;

    if (!EM30_W(pEm)->Unk20) {
        Snd_se_enem(Se[EM30_W(pEm)->Unk0B], pEm);
        EM30_W(pEm)->Unk20 = 25;
    } else {
        EM30_W(pEm)->Unk20--;
    }
    if (pEm->Routine_2 == 1) {
        if (EM30_W(pEm)->Unk19 == 1) {
            Tg.vx = -0x4BD4;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x4BD4) * (pPl->Pos_x + 0x4BD4) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        } else {
            Tg.vx = -0x86B;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x86B) * (pPl->Pos_x + 0x86B) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        }
        if (SquareRoot0((Tg.vx - pEm->Pos_x) * (Tg.vx - pEm->Pos_x) + (Tg.vz - pEm->Pos_z) * (Tg.vz - pEm->Pos_z)) < 200) {
            EM_RTN(pEm) = 0x20301;
            EM30_W(pEm)->Unk30 = 0;
            pEm->Timer0 = 0;
            EM_MOVE(pEm) = 0;
            pEm->Be_flg &= ~0x2000;
            G.Status_flg &= ~4;
        }
    }
}

/* mv[8]: crawl back to the hole */
void func_80104724(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Tg;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Be_flg |= 0x2000;
        pEm->Routine_2++;
    case 1:
        if (EM30_W(pEm)->Unk19 == 1) {
            Tg.vx = -0x4BD4;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x4BD4) * (pPl->Pos_x + 0x4BD4) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        } else {
            Tg.vx = -0x86B;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x86B) * (pPl->Pos_x + 0x86B) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        }
        Goto00(pEm, Tg.vx, Tg.vz, 0x180);
        if (SquareRoot0((Tg.vx - pEm->Pos_x) * (Tg.vx - pEm->Pos_x) + (Tg.vz - pEm->Pos_z) * (Tg.vz - pEm->Pos_z)) < 900) {
            pEm->Spd_x = 50;
            pEm->Spd_z = 0;
        } else {
            pEm->Spd_x = 150;
            pEm->Spd_z = 0;
        }
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (++pEm->Timer0 == 3)
            G.Status_flg |= 4;
        if (pEm->Timer0 == 6)
            G.Status_flg &= ~4;
        if (pEm->Timer0 == 20)
            pEm->Timer0 = 0;
    }
}

/* chain the body parts: accumulated roll, each part hangs from the previous one */
void func_80104920(ENEMY_WORK *pEm)
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
    func_80104A9C(pEm, &pEm->pSin_parts_ptr[0], &pEm->pSin_parts_ptr[1], 0x1000);
    func_80104A9C(pEm, &pEm->pSin_parts_ptr[1], &pEm->pSin_parts_ptr[2], 0x1000);
    func_80104A9C(pEm, &pEm->pSin_parts_ptr[2], &pEm->pSin_parts_ptr[3], 0x1000);
    func_80104A9C(pEm, &pEm->pSin_parts_ptr[3], &pEm->pSin_parts_ptr[4], 0x1000);
    func_80104A9C(pEm, &pEm->pSin_parts_ptr[4], &pEm->pSin_parts_ptr[5], 0x1000);
    func_80104A9C(pEm, &pEm->pSin_parts_ptr[5], &pEm->pSin_parts_ptr[6], 0x1000);
}

/* hang part pP1 from part pP0 and turn it towards pP0 */
void func_80104A9C(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate)
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

/* as func_80104A9C, sinking towards z -19000 with Move_cnt */
void func_80104C58(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate)
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

/* allocate a new work (copy of pEm) at the end of the work list (RE2 Enemy_work_set style) */
ENEMY_WORK *func_80104E58(ENEMY_WORK *pEm)
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

/* morph the body (keys 0..2 of parts 1..3) in a cycle, faster with the speed.
   Returns the last sabun call's value (only one path returns): that explicit v0 use is what
   makes reload put the /50 mfhi in a3. */
LONG func_80104F20(ENEMY_WORK *pEm)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[1];
    LONG Step;

    if (EM_RTN01(pEm) != 0x301 || pEm->Routine_2 == 2) {
        Step = EM30_W(pEm)->Unk14 * pEm->Spd_x / 50;
        if (Step < 0)
            Step = 0;
        switch (EM30_W(pEm)->Unk01) {
        case 0:
            EM30_SABUN_P(&pP[0])->Key[0].Rate += Step;
            EM30_SABUN_P(&pP[1])->Key[0].Rate += Step;
            EM30_SABUN_P(&pP[2])->Key[0].Rate += Step;
            if (EM30_SABUN_P(&pP[0])->Key[0].Rate > 0xC00) {
                EM30_SABUN_P(&pP[0])->Key[0].Rate = 0xC00;
                EM30_SABUN_P(&pP[1])->Key[0].Rate = 0xC00;
                EM30_SABUN_P(&pP[2])->Key[0].Rate = 0xC00;
                EM30_W(pEm)->Unk01 = 1;
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
                EM30_W(pEm)->Unk01 = 2;
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
                EM30_W(pEm)->Unk01 = 3;
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
                EM30_W(pEm)->Unk01 = 0;
            }
            break;
        }
        func_8004AC54(pEm, &pP[0]);
        func_8004AC54(pEm, &pP[1]);
        return func_8004AC54_l(pEm, &pP[2]);
    }
}

/* morph key 0 of part 8 one step towards 0 (Dir != 0) or 0x1000 */
void func_8010532C(ENEMY_WORK *pEm, LONG Step, LONG Dir)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

    if (Dir) {
        EM30_SABUN_P(pP)->Key[0].Rate -= Step;
        if (EM30_SABUN_P(pP)->Key[0].Rate < 0)
            EM30_SABUN_P(pP)->Key[0].Rate = 0;
    } else {
        EM30_SABUN_P(pP)->Key[0].Rate += Step;
        if (EM30_SABUN_P(pP)->Key[0].Rate > 0x1000)
            EM30_SABUN_P(pP)->Key[0].Rate = 0x1000;
    }
    func_8004AC54(pEm, pP);
}

/* pick the hole the em30 heads for (Unk19 1 or 9) by its x */
void func_801053CC(ENEMY_WORK *pEm)
{
    LONG Flg = 0;

    if ((ULONG)(pEm->Routine_1 - 7) < 2 || (EM_RTN(pEm) & 0xFFFF00) == 0x20300) {
        if (Flg_ck(&G.Unk7888, 4))
            Flg = 1;
    }
    if (Flg || EM30_W(pEm)->Unk11 > 10) {
        if (EM30_W(pEm)->Unk19 != 1 && EM30_W(pEm)->Unk19 != 9) {
            if (pEm->Pos_x < -10000)
                EM30_W(pEm)->Unk19 = 1;
            else
                EM30_W(pEm)->Unk19 = 9;
        }
    }
}

/* RE2 Em30_Damage */
void func_8010549C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    pEm->Routine_1 = 0;
    if (pEm->Unk12C == 30 && !(pEm->Unk132 & 4))
        pEm->Routine_1 = 1;
    Em30_dm[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
}

/* dm[0..28]: ordinary hit; a sub-work passes the damage on to the body */
void func_80105514(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOya;
    LONG Dm;

    if (!(pEm->Type & 1)) {
        if (++EM30_W(pEm)->Unk0C >= 16 && EM30_W(pEm)->Unk0B != 1) {
            EM30_W(pEm)->Unk0C = 0;
            pEm->Unk12C = 0x20;
            func_8010549C(pEm, pKan, pSeq);
            EM30_W(pEm)->Unk03 = 0;
        } else {
            if (pEm->Life < 0) {
                pEm->Life = 0;
                EM30_W(pEm)->Unk26 = 0;
            }
            EM_RTN(pEm) = pEm->Unk1C4;
            func_80100CBC(pEm, pKan, pSeq);
            Snd_se_enem(2, pEm);
        }
    } else {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (EM30_SUB(pEm)->Parts_no == 8) {
            if (EM30_W(pOya)->Unk03) {
                EM30_W(pOya)->Unk0C += 5;
                if (!pOya->Damage_cnt) {
                    Dm = EM30_W(pEm)->Unk26 - pEm->Life;
                    pOya->Damage_cnt = pEm->Damage_cnt;
                    pOya->Life -= Dm;
                }
            } else if (!pOya->Damage_cnt) {
                Dm = EM30_W(pEm)->Unk26 - pEm->Life;
                pOya->Damage_cnt = pEm->Damage_cnt;
                pOya->Life -= Dm;
            }
        } else if (!pOya->Damage_cnt) {
            Dm = EM30_W(pEm)->Unk26 - pEm->Life;
            pOya->Damage_cnt = pEm->Damage_cnt;
            pOya->Life -= Dm * 2 / 3;
        }
        if (pOya->Life < 0) {
            if (pOya->Routine_0 == 1 && EM30_W(pOya)->Unk0B != 1) {
                pOya->Unk12C = pEm->Unk12C;
                EM_RTN(pOya) = 3;
            } else {
                pOya->Life = 0;
                EM30_W(pOya)->Unk26 = 0;
            }
        }
        EM30_W(pOya)->Unk0C++;
        Snd_se_enem(2, pOya);
        EM_RTN(pEm) = pEm->Unk1C4;
    }
    if (pEm->Unk12C == 5) {
        pEm->Unk16A = 1;
        EM30_W(pEm)->Unk26 = pEm->Life;
    } else {
        func_80105C8C(pEm);
    }
}

/* dm[31]: flinch back (a sub-work hands the hit to the body) */
void func_80105740(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[4] = {0x12, 0x13, 0x14, 0x15};
    UCHAR Se[2] = {7, 8};
    ENEMY_WORK *pOya;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        pOya->Unk12C = pEm->Unk12C;
        if (pOya->Routine_0 == 1)
            pOya->Unk1C4 = EM_RTN(pOya);
        EM_RTN(pOya) = EM_RTN(pEm);
        EM_RTN(pEm) = pEm->Unk1C4;
        func_80105C8C(pEm);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        pEm->Damage_cnt |= 0x80;
        EM_MOVE(pEm) = Mot[EM30_W(pEm)->Unk02 * 2 + EM30_W(pEm)->Unk03] + 0x1F0000;
        func_80105C8C(pEm);
        EM30_W(pEm)->Unk04 = 40;
        EM30_W(pEm)->Unk06 = 80;
        EM30_W(pEm)->Unk08 = 5;
        pEm->Spd_z = 0;
        Snd_se_enem(3, pEm);
        func_801025E0(pEm);
        pEm->Routine_2++;
    case 1:
        if (pEm->Move_cnt == 3 || pEm->Move_cnt == 9)
            func_80105C8C(pEm);
        func_801025E0(pEm);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_RTN(pEm) = 0x301;
            pEm->Damage_cnt &= 0x7F;
        }
        pEm->Spd_x = EM30_W(pEm)->Unk04;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Unk04 += EM30_W(pEm)->Unk08;
        if (EM30_W(pEm)->Unk08 > 0) {
            if (EM30_W(pEm)->Unk04 > EM30_W(pEm)->Unk06) {
                EM30_W(pEm)->Unk04 = EM30_W(pEm)->Unk06;
                EM30_W(pEm)->Unk08 = -8;
                EM30_W(pEm)->Unk06 = 40;
            }
        } else if (EM30_W(pEm)->Unk04 < EM30_W(pEm)->Unk06) {
            EM30_W(pEm)->Unk04 = EM30_W(pEm)->Unk06;
            EM30_W(pEm)->Unk08 = 5;
            EM30_W(pEm)->Unk06 = 80;
        }
        func_80104920(pEm);
    }
}

/* dm[30]: hit by the magnum / grenade (knocks the em30 back into the ground) */
void func_801059A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOya;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (!Flg_ck(&G.Unk7888, 0) && EM30_W(pOya)->Unk0B != 1) {
            pOya->Unk12C = pEm->Unk12C;
            pOya->Unk132 = pEm->Unk132;
            pOya->Damage_cnt = 10;
            pOya->Unk1C4 = EM_RTN(pOya);
            EM_RTN(pOya) = EM_RTN(pEm);
            func_80106710(pEm);
            Flg_on(&G.Unk7888, 0);
            G.Unk75EA = 1;
        }
        pEm->Damage_cnt = 10;
        EM_RTN(pEm) = pEm->Unk1C4;
    } else {
        if (EM30_W(pEm)->Unk0B != 1) {
            if (EM30_W(pEm)->Unk0A == 1) {
                func_80105B4C(pEm, pKan, pSeq);
                return;
            }
            EM30_W(pEm)->Unk0A = 1;
            pEm->Damage_cnt = 10;
            pEm->Life /= 3;
            func_80106710(pEm);
            if (G.Unk75EA)
                G.Unk75EA = 0;
            else
                Flg_on(&G.Unk7888, 0);
        }
        EM_RTN(pEm) = pEm->Unk1C4;
        func_80100CBC(pEm, pKan, pSeq);
    }
}

/* dm[29]: hit by the grenade launcher's flame (and the like) */
void func_80105B4C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOya;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (!Flg_ck(&G.Unk7888, 0) && EM30_W(pOya)->Unk0B != 1) {
            pOya->Unk12C = pEm->Unk12C;
            pOya->Unk132 = pEm->Unk132;
            pOya->Unk1C4 = EM_RTN(pOya);
            EM_RTN(pOya) = EM_RTN(pEm);
            func_80106710(pEm);
        }
        pEm->Damage_cnt = 10;
        EM_RTN(pEm) = pEm->Unk1C4;
    } else if (EM30_W(pEm)->Unk0B == 1) {
        EM_RTN(pEm) = pEm->Unk1C4;
        func_80100CBC(pEm, pKan, pSeq);
    } else {
        pEm->Life = -1;
        Flg_on(&G.Unk7888, 0);
        if (!EM30_W(pEm)->Unk13)
            func_80106710(pEm);
        EM_RTN(pEm) = pEm->Unk1C4;
        func_80100CBC(pEm, pKan, pSeq);
        EM_RTN(pEm) = 3;
    }
}

/* blood from the hit part, more for a bigger hit */
void func_80105C8C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pOya;
    SVECTOR Sv;
    LONG Dm;
    LONG n;
    ULONG Dir;

    Dm = EM30_W(pEm)->Unk26 - pEm->Life;
    EM30_W(pEm)->Unk26 = pEm->Life;
    n = 2;
    if (Dm > 20)
        n = 4;
    if (Dm > 50)
        n += 2;
    if (Dm > 100)
        n += 2;
    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        while (n--) {
            Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vy = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd_l() & 3) * 0x300 + 0x1200;
            Esp_call(0x7000700, (Dir << 16) | (USHORT)pOya->Cdir_y, &pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm, &Sv, NULL);
        }
    } else {
        while (n--) {
            Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vy = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd_l() & 3) * 0x300 + 0x1200;
            Esp_call(0x7000700, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[8].Workm, &Sv, NULL);
        }
    }
}

/* dirt thrown up at the hit part, with its sound */
void func_80105E94(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pOya;
    SVECTOR Sv;
    VECTOR Pos;
    LONG n = 2;
    ULONG Dir;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        do {
            Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vy = -(Rnd_l() & 0xFF);
            Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd_l() & 3) * 0x300 + 0x1000;
            Sv.vx += pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[0];
            Sv.vz += pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[2];
            if (Rnd_l() & 1)
                Esp_call(0x7000700, (Dir << 16) | (USHORT)pOya->Cdir_y, &GsIDMATRIX, &Sv, NULL);
            else
                Esp_call(0x7000100, (Dir << 16) | (USHORT)pOya->Cdir_y, &GsIDMATRIX, &Sv, NULL);
        } while (n--);
    } else {
        do {
            Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vy = -(Rnd_l() & 0xFF) - 0x7F;
            Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd_l() & 3) * 0x300 + 0x1000;
            Sv.vx += pEm->pSin_parts_ptr[8].Workm.t[0];
            Sv.vz += pEm->pSin_parts_ptr[8].Workm.t[2];
            if (Rnd_l() & 1)
                Esp_call(0x7000700, (Dir << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Sv, NULL);
            else
                Esp_call(0x7000100, (Dir << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Sv, NULL);
        } while (n--);
    }
    Pos.vx = Sv.vx;
    Pos.vy = Sv.vy;
    Pos.vz = Sv.vz;
    Snd_se_stad(0x1020F, &Pos, 0, 0);
}

/* Unk16A: a line of dirt puffs from the hit part towards the player */
void func_80106130(ENEMY_WORK *pEm)
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
            pP = &G.pEnemy[EM30_SUB(pEm)->Oya_no]->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no];
        Sv.vx = pP->Workm.t[0] + Ofs.vx;
        Sv.vy = pP->Workm.t[1] + Ofs.vy;
        Sv.vz = pP->Workm.t[2] + Ofs.vz;
        Esp_call(0x7001900, (0x1280 - pEm->Unk16B * 70 - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x7001900, (0x1300 - pEm->Unk16B * 70 - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + (Step.vx << 1);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + (Step.vz << 1);
        Esp_call(0x7001900, (0x1380 - (pEm->Unk16B << 7) - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 3 - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 3 - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x7001900, (0x1400 - (pEm->Unk16B << 7) - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + (Step.vx << 2);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + (Step.vz << 2);
        Esp_call(0x7001900, (0x1480 - (pEm->Unk16B << 7) - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 5 - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 5 - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x7001900, (0x1500 - (pEm->Unk16B << 7) - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 6;
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 6;
        Esp_call(0x7001900, (0x1580 - (pEm->Unk16B << 7) - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        pEm->Unk16B++;
        Sv.vx = pP->Workm.t[0] + Ofs.vx + Step.vx * 7 - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + Ofs.vz + Step.vz * 7 - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x7001900, (0x1600 - (pEm->Unk16B << 7) - Rnd_l()) << 16, &GsIDMATRIX, &Sv, NULL);
        if (pEm->Unk16B++ > 16)
            pEm->Unk16A = 0;
    }
}

/* number the body and its sub-works outwards from the hit one (Unk13 = 10, 13, 16, ...) */
void func_80106710(ENEMY_WORK *pEm)
{
    LONG Up, Dn;
    UCHAR Val = 10;
    LONG No;
    PARTS_W *pP;

    if (pEm->Type & 1) {
        Dn = EM30_SUB(pEm)->Parts_no;
        pEm = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        Up = Dn;
        if (Dn >= 7)
            Dn = 6;
    } else {
        Dn = 0;
        Up = 0;
    }
    for (; Up < 7 || Dn >= 0; Up++, Dn--, Val += 3) {
        if (Up < 7 && Up) {
            pP = &pEm->pSin_parts_ptr[Up];
            No = pP->Unk82;
            EM30_W(G.pEnemy[No])->Unk13 = Val;
        }
        if (!Up)
            EM30_W(pEm)->Unk13 = Val;
        if (Dn > 0) {
            pP = &pEm->pSin_parts_ptr[Dn];
            No = pP->Unk82;
            EM30_W(G.pEnemy[No])->Unk13 = Val;
        }
        if (!Dn)
            EM30_W(pEm)->Unk13 = Val;
    }
}

/* while Unk13 counts down: dirt at a few steps, with a sound at 8 */
void func_8010682C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pOya;
    SVECTOR Sv;
    VECTOR Pos;
    LONG n = 1;

    if (!EM30_W(pEm)->Unk13)
        return;
    EM30_W(pEm)->Unk13--;
    if (EM30_W(pEm)->Unk13 == 1 || EM30_W(pEm)->Unk13 == 3 || EM30_W(pEm)->Unk13 == 8) {
        if (EM30_W(pEm)->Unk13 == 8) {
            if (pEm->Type & 1) {
                pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
                Pos.vx = pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[0];
                Pos.vy = pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[1];
                Pos.vz = pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm.t[2];
            } else {
                Pos.vx = pEm->pSin_parts_ptr[8].Workm.t[0];
                Pos.vy = pEm->pSin_parts_ptr[8].Workm.t[1];
                Pos.vz = pEm->pSin_parts_ptr[8].Workm.t[2];
            }
            if (Rnd_l() & 3)
                Snd_se_stad(0x1020C, &Pos, 0, 0);
            else
                Snd_se_stad(0x1020D, &Pos, 0, 0);
        }
        while (n--) {
            Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vy = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            if (pEm->Type & 1) {
                pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
                Esp_call(0x2B, (USHORT)pOya->Cdir_y | 0x40000000, &pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm, &Sv, NULL);
            } else {
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x40000000, &pEm->pSin_parts_ptr[8].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[12].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[14].Workm, &Sv, NULL);
                Esp_call(0x2B, (USHORT)pEm->Cdir_y | 0x20000000, &pEm->pSin_parts_ptr[16].Workm, &Sv, NULL);
            }
        }
    }
}

/* dirt effect Kind around part No */
void func_80106B3C(ENEMY_WORK *pEm, LONG No, LONG Kind)
{
    SVECTOR Sv;
    ULONG Dir;

    Sv.vx = (Rnd_l() & 0xFF) - 0x7F;
    Sv.vz = (Rnd_l() & 0xFF) - 0x7F;
    Sv.vy = -50;
    Sv.vx += pEm->pSin_parts_ptr[No].Workm.t[0];
    Sv.vz += pEm->pSin_parts_ptr[No].Workm.t[2];
    Dir = (Rnd_l() & 3) << 9;
    Esp_call((Kind << 8) | 0x1000003, ((Dir | 0x2000) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Sv, NULL);
}

/* RE2 Em30_Die */
void func_80106C28(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (EM30_W(pEm)->Unk0B == 1) {
        pEm->Life = 1;
        EM_RTN(pEm) = pEm->Unk1C4;
        func_80100CBC(pEm, pKan, pSeq);
    } else {
        Em30_die[pEm->Unk12C - 1](pEm, pKan, pSeq);
    }
}

/* die[0..29]: shot to pieces (by the weapon in Blow[]) and sinking */
void func_80106C8C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    signed char Blow[32] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0};
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    PARTS_W *pS;
    KAGE_WORK *pK;
    LONG Dir;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2++;
        pEm->Damage_cnt |= 0x80;
        Snd_se_enem(5, pEm);
        EM_MOVE(pEm) = 0x1F0017;
        func_801025E0(pEm);
        pP = pEm->pSin_parts_ptr;
        for (i = 17; i != 0; i--) {
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP++;
        }
        pEm->Life = -1;
        pEm->Unk120 |= 1;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[8];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[2];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[3];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[4];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[5];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[6];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        EM30_W(pEm)->Unk12 = pPl->Type;
        if (Blow[EM30_W(pEm)->Unk12]) {
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[17];
            PW_FREE_L(pP)[0] = 0x20;
            PW_FREE_L(pP)[1] = 0x101030;
            pP->UnkA2 |= 0x405;
        }
        G.Status_flg &= ~4;
        G.Status_flg &= ~2;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 2500;
        Sv.vy = 0;
        Sv.vz = -700;
        Esp_call(0x7001500, 0x2E000938, &pP->Workm, &Sv, NULL);
        Sv.vz = 700;
        Esp_call(0x7001500, 0x32000550, &pP->Workm, &Sv, NULL);
        Sv.vy = 700;
        Sv.vz = 0;
        Esp_call(0x7001500, 0x20000800, &pP->Workm, &Sv, NULL);
        Sv.vy = -700;
        Esp_call(0x7001500, 0x18000800, &pP->Workm, &Sv, NULL);
        Sv.vy = 0;
        Esp_call(0x7001500, 0x22000800, &pP->Workm, &Sv, NULL);
        Sv.vy = 0;
        Sv.vz = -600;
        Esp_call(0x7000000, 0x34000938, &pP->Workm, &Sv, NULL);
        Sv.vz = 550;
        Esp_call(0x7000000, 0x25000550, &pP->Workm, &Sv, NULL);
        Sv.vy = 750;
        Sv.vz = 0;
        Esp_call(0x7000000, 0x18000800, &pP->Workm, &Sv, NULL);
        Sv.vy = -700;
        Esp_call(0x7000000, 0x42000800, &pP->Workm, &Sv, NULL);
        pEm->Timer0 = 0;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        func_8003884C(Em30_vib_tbl, 9);
    case 1:
        Dir = pEm->Cdir_y + (((UCHAR)(pEm->Move_cnt % 7) - 3) << 8);
        if (pEm->Move_cnt < 50 && Blow[EM30_W(pEm)->Unk12]) {
            if (pEm->Move_cnt < 32) {
                pP = &pEm->pSin_parts_ptr[8];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[10];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[12];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[14];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[16];
                pP->Poly_rgb -= 0x40202;
            }
            if (pEm->Move_cnt == 33) {
                pP = &pEm->pSin_parts_ptr[8];
                Sv.vx = 0;
                Sv.vy = 0;
                Sv.vz = 0;
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[10];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[12];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[14];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[16];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
            }
            if (!(pEm->Move_cnt & 3)) {
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 2500;
                Sv.vy = 0;
                Sv.vz = -700;
                Esp_call(0x7001500, (USHORT)(Dir + 0x938) | 0x2E000000, &pP->Workm, &Sv, NULL);
                Sv.vz = 700;
                Esp_call(0x7001500, (USHORT)(Dir + 0x550) | 0x32000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 700;
                Sv.vz = 0;
                Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x20000000, &pP->Workm, &Sv, NULL);
                Sv.vy = -700;
                Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x18000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 0;
                Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x22000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 0;
                Sv.vz = -600;
                Esp_call(0x7000000, (USHORT)(Dir + 0x938) | 0x14000000, &pP->Workm, &Sv, NULL);
                Sv.vz = 550;
                Esp_call(0x7000000, (USHORT)(Dir + 0x550) | 0x25000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 750;
                Sv.vz = 0;
                Esp_call(0x7000000, (USHORT)(Dir + 0x800) | 0x18000000, &pP->Workm, &Sv, NULL);
                Sv.vy = -700;
                Esp_call(0x7000000, (USHORT)(Dir + 0x800) | 0x32000000, &pP->Workm, &Sv, NULL);
            }
        } else if (!(pEm->Move_cnt & 7)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 2500;
            Sv.vy = 0;
            Sv.vz = -700;
            Esp_call(0x7001500, (USHORT)(Dir + 0x938) | 0x24000000, &pP->Workm, &Sv, NULL);
            Sv.vz = 700;
            Esp_call(0x7001500, (USHORT)(Dir + 0x550) | 0x22000000, &pP->Workm, &Sv, NULL);
            Sv.vy = 700;
            Sv.vz = 0;
            Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x18000000, &pP->Workm, &Sv, NULL);
            Sv.vy = -700;
            Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x12000000, &pP->Workm, &Sv, NULL);
            Sv.vy = 0;
            Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x1E000000, &pP->Workm, &Sv, NULL);
        }
        Sv.vx = 500;
        if (pEm->Move_cnt == 30) {
            pP = pEm->pSin_parts_ptr;
            Sv.vy = 300;
            Sv.vz = -700;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x20000000, &pP->Workm, &Sv, NULL);
        }
        if (pEm->Move_cnt == 45) {
            pS = pEm->pSin_parts_ptr;
            Sv.vy = 200;
            Sv.vz = -700;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x1E000000, &pS[3].Workm, &Sv, NULL);
        }
        if (pEm->Move_cnt == 60) {
            pS = pEm->pSin_parts_ptr;
            Sv.vy = 0;
            Sv.vz = -100;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x20000000, &pS[1].Workm, &Sv, NULL);
        }
        if (pEm->Move_cnt == 75) {
            pS = pEm->pSin_parts_ptr;
            Sv.vy = 0;
            Sv.vz = -400;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x18000000, &pS[4].Workm, &Sv, NULL);
        }
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x80);
        func_80104920(pEm);
        break;
    case 2:
        Em_dead_flg_on_cand(pEm);
        for (i = 0; i < 9; i++) {
            if (i == 7)
                i = 8;
            if (!i) {
                pK = pEm->Kage.pKw;
            } else {
                pP = &pEm->pSin_parts_ptr[i];
                pK = G.pEnemy[pP->Unk82]->Kage.pKw;
            }
            *(ULONG *)&pK->Prim[0].r0 = (*(ULONG *)&pK->Prim[0].r0 & 0xFF000000) | 0xBF3030;
            *(ULONG *)&pK->Prim[1].r0 = (*(ULONG *)&pK->Prim[1].r0 & 0xFF000000) | 0xBF3030;
        }
        pEm->Timer0 = 20;
        pEm->Routine_2++;
        break;
    case 3:
        if (--pEm->Timer0) {
            for (i = 0; i < 9; i++) {
                if (i == 7)
                    i = 8;
                if (!i) {
                    pK = pEm->Kage.pKw;
                } else {
                    pP = &pEm->pSin_parts_ptr[i];
                    pK = G.pEnemy[pP->Unk82]->Kage.pKw;
                }
                    pK->Half_x++;
                pK->Half_z++;
            }
        } else {
            pEm->Routine_2++;
        }
        break;
    case 4:
        break;
    }
    if (pEm->Pos_y < 1200) {
        if (pEm->Pos_y > 1100) {
            for (i = 0; i < 18; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb -= 0x201;
            }
            if (pEm->Pos_y & 2) {
                for (i = 0; i < 18; i++) {
                    pP = &pEm->pSin_parts_ptr[i];
                    pP->Poly_rgb -= 0x10000;
                }
            }
        }
    }
    if (pEm->Pos_y > 1100 && pEm->Col.Id) {
        pEm->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[8].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[1].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[2].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[3].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[4].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[6].Unk82]->Col.Id = 0;
    }
    if (pEm->Pos_y < 1400) {
        pEm->Pos_y = pEm->Pos_y < 1000 ? pEm->Pos_y + 6 : pEm->Pos_y + 2;
        pP = &pEm->pSin_parts_ptr[8];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[1];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[2];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[3];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[4];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[5];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[6];
        pP->Workm.t[1] += 2;
        {
            ENEMY_WORK *pB = G.pEnemy[(Rnd_l() & 7) + 2];

            if (++pEm->UnkE2 == 8) {
                pEm->UnkE2 = 0;
                if ((pB->Be_flg & 1) && pB->Id == 0x30)
                    func_80105E94(pB);
            }
        }
    } else if (!pEm->UnkE3) {
        pEm->UnkE3 = 1;
        func_8001B894(0x1403, NULL);
        Dir = pEm->Cdir_y + (((UCHAR)(pEm->Move_cnt % 7) - 3) << 8);
        Sv.vx = 500;
        pP = pEm->pSin_parts_ptr;
        Sv.vy = 300;
        Sv.vz = -700;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x20000000, &pP->Workm, &Sv, NULL);
        pS = pEm->pSin_parts_ptr;
        Sv.vy = 200;
        Sv.vz = -700;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x1E000000, &pS[3].Workm, &Sv, NULL);
        pS = pEm->pSin_parts_ptr;
        Sv.vy = 0;
        Sv.vz = -100;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x20000000, &pS[1].Workm, &Sv, NULL);
        pS = pEm->pSin_parts_ptr;
        Sv.vy = 0;
        Sv.vz = -400;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x18000000, &pS[4].Workm, &Sv, NULL);
    }
    if (EM30_W(pEm)->Unk14)
        EM30_W(pEm)->Unk14 -= 4;
}

/* die[30]: killed by the grenade launcher: thrash about and sink into the ground */
void func_80107EE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR Pos;
    PARTS_W *pP;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        Flg_on(&G.Unk7888, 0);
        Flg_on(&G.Unk7888, 1);
        func_801025E0(pEm);
        pEm->Life = -1;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[8];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[2];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[3];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[4];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[5];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[6];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pEm->Routine_2++;
        func_8003884C(Em30_vib_tbl, 8);
        pEm->Timer0 = 0;
        pEm->UnkE3 = 0;
        break;
    case 1:
        pEm->UnkDA = 0;
        Snd_se_enem(5, pEm);
        EM_MOVE(pEm) = 0x1F0018;
        pP = pEm->pSin_parts_ptr;
        for (i = 17; i != 0; i--) {
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP++;
        }
        pEm->Unk120 |= 1;
        G.Status_flg &= ~4;
        G.Status_flg &= ~2;
        pEm->Pos_x = -0x1D00;
        pEm->Pos_z = -0x3E6E;
        pEm->Cdir_y = 0x98B;
        pEm->UnkE2 = 0;
        pEm->Routine_2++;
        func_80034124(&pEm->Col);
    case 2:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x80);
        if (!EM30_W(pEm)->Unk13)
            func_80106710(pEm);
        if (pEm->Move_cnt == 37) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            func_80106B3C(pEm, 0, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            func_801023F4(pEm, EM30_W(pEm)->Unk18, 100);
            func_801023F4(pEm, EM30_W(pEm)->Unk18, 100);
            func_801023F4(pEm, EM30_W(pEm)->Unk18, 100);
        }
        if (pEm->Move_cnt == 49) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            func_801023F4(pEm, 0, 100);
            func_801023F4(pEm, 2, 100);
            func_801023F4(pEm, 4, 100);
        }
        if (pEm->Move_cnt == 56) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            func_80106B3C(pEm, 3, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            func_801023F4(pEm, 1, 100);
            func_801023F4(pEm, 2, 100);
            func_801023F4(pEm, 5, 100);
        }
        if (pEm->Move_cnt == 74) {
            G.Status_flg |= 2;
            pEm->UnkE2 = 10;
            func_80106B3C(pEm, 1, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x10210, &Pos, 0, 0);
            func_801023F4(pEm, 0, 100);
            func_801023F4(pEm, 1, 100);
            func_801023F4(pEm, 2, 100);
            func_801023F4(pEm, 3, 100);
            func_801023F4(pEm, 4, 100);
        }
        if (pEm->Move_cnt == 97) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            func_80106B3C(pEm, 4, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            func_801023F4(pEm, 0, 100);
            func_801023F4(pEm, 2, 100);
            func_801023F4(pEm, 4, 100);
        }
        if (pEm->Move_cnt == 115) {
            G.Status_flg |= 2;
            pEm->UnkE2 = 10;
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x10210, &Pos, 0, 0);
            func_801023F4(pEm, 0, 100);
            func_801023F4(pEm, 1, 100);
            func_801023F4(pEm, 2, 100);
            func_801023F4(pEm, 3, 100);
            func_801023F4(pEm, 4, 100);
        }
        if (pEm->UnkE2 && !--pEm->UnkE2) {
            G.Status_flg &= ~4;
            G.Status_flg &= ~2;
        }
        if (pEm->Move_cnt & 1) {
            pP = pEm->pSin_parts_ptr;
            i = 0;
            do {
                pP->Poly_rgb -= 0x10101;
                pP++;
            } while (++i < pEm->Parts_num);
        }
        break;
    case 3:
        EM_MOVE(pEm) = 0x70019;
        pEm->Routine_2++;
        break;
    case 4:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 5:
        if (!(Rnd_l() & 0xF))
            pEm->Routine_2 -= 2;
        break;
    }
    if (pEm->Routine_2 >= 4) {
        if (pEm->Pos_y < 600)
            Em_dead_flg_on_cand(pEm);
        if (pEm->Pos_y < 900 && pEm->Col.Id) {
            pEm->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[8].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[1].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[2].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[3].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[4].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[6].Unk82]->Col.Id = 0;
        }
        if (pEm->Pos_y < 1400) {
            pEm->Pos_y += 2;
            if (++pEm->Timer0 == 8) {
                ENEMY_WORK *pB;

                pEm->Timer0 = 0;
                pB = G.pEnemy[(Rnd_l() & 7) + 2];
                if ((pB->Be_flg & 1) && pB->Id == 0x30)
                    func_80105E94(pB);
            }
        } else if (!pEm->UnkE3) {
            pEm->UnkE3 = 1;
            func_8001B894(0x1203, NULL);
            func_80106B3C(pEm, 0, 0x1B);
            func_80106B3C(pEm, 1, 0x1B);
            func_80106B3C(pEm, 3, 0x1B);
            func_80106B3C(pEm, 4, 0x1B);
        }
    }
    if (EM30_W(pEm)->Unk14)
        EM30_W(pEm)->Unk14 -= 4;
}

/* positional sound No at pPos, from the second bank when the room uses pEm's */
void func_801089B0(ULONG No, ENEMY_WORK *pEm, LONG *pPos)
{
    VECTOR Pos;

    Pos.vx = pPos[0];
    Pos.vy = pPos[1];
    Pos.vz = pPos[2];
    if (pEm->Unk12F == D_800BA6E8[3])
        No += 0x10;
    Snd_se_stad(No | 0x10300, &Pos, 0, 0);
}

/* G.pPldie_Em hook: the player dies to the em30 */
void Em30_Pldie(ENEMY_WORK *pPl)
{
    pPl->Be_flg |= 0x10;
}
