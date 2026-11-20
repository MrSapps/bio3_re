#include "common.h"
#include "ovl/em30.h"

/* module .data; routine tables are unprototyped like RE2's: routines take (pEm, Kan, Seq) */
extern void (*Em30_routine_0[])() __asm__("D_80108C74");
extern USHORT Em30_col_w[] __asm__("D_80108C98");
extern void (*Em30_mv_br[])() __asm__("D_80108CC0");
extern void (*Em30_mv[])() __asm__("D_80108D00");
extern void (*Em30_dm[][2])() __asm__("D_80108D40");
extern void (*Em30_die[])() __asm__("D_80108E40");

void func_801053CC(ENEMY_WORK *pEm);
void Em30_Move_type(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq) __asm__("func_80100CF4");
void func_8010258C(ENEMY_WORK *pEm);
void func_80104920(ENEMY_WORK *pEm);
ENEMY_WORK *func_80104E58(ENEMY_WORK *pEm);
void func_80100B34(ENEMY_WORK *pSub, LONG No, ENEMY_WORK *pEm, LONG Parts_no);
void Em30_Pldie(ENEMY_WORK *pPl) __asm__("func_80108A28");
void func_80104F20(ENEMY_WORK *pEm);
void func_8010682C(ENEMY_WORK *pEm);
void func_80106130(ENEMY_WORK *pEm);

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
            pEm->Col_unk26 = -y;
            pEm->Col_h = 800 - y;
        } else {
            pEm->Col_unk26 = 0;
            pEm->Col_h = 800;
        }
    } else {
        pG = &G;
        pOya = (&pG->pUnk265C)[EM30_SUB(pEm)->Oya_no];
        if (pOya->Routine_0 != 3 && !(pG->System_flg & 0x800)) {
            pP = &pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no];
            if (pP->UnkA2 & 1) {
                pEm->Damage_cnt &= 0x7F;
                pEm->pKage_work->Be_flg |= 0x80;
                pEm->Type &= ~0x4000;
            } else {
                pEm->Damage_cnt |= 0x80;
                pEm->pKage_work->Be_flg &= 0x7F;
                pEm->Type |= 0x4000;
            }
        }
        if (pEm->Routine_0 == 2)
            Em30_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        else if (pEm->Damage_cnt & 0x7F)
            pEm->Damage_cnt--;
        M = GsIDMATRIX;
        MulMatrix2(pEm->pCol_m, &M);
        pEm->Col_unk26 = 0;
        ApplyMatrix(&M, (SVECTOR *)&pEm->Col_unk24, &Out);
        y = pEm->pCol_m->t[1] + Out.vy;
        if (y < 0) {
            pEm->Col_unk26 = -y;
            pEm->Col_h = 800 - y;
        } else {
            pEm->Col_h = 800;
        }
        pEm->Pos_x = pEm->Col_pos_x;
        pEm->Pos_y = pEm->Col_pos_y;
        pEm->Pos_z = pEm->Col_pos_z;
    }
    func_8010682C(pEm);
    func_80106130(pEm);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80100544);

/* a collision sub-work No following part Parts_no of the body pEm */
void func_80100B34(ENEMY_WORK *pSub, LONG No, ENEMY_WORK *pEm, LONG Parts_no)
{
    COL_OBJ *pCol;

    pCol = EM_COL(pSub);
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
void func_80100CBC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em30_Move_type(pEm, Kan, Seq);
}

/* RE2 Em30_Move_type00 */
void func_80100CF4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Em30_mv_br[pEm->Routine_1](pEm);
    Em30_mv[pEm->Routine_1](pEm, Kan, Seq);
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
        if (Pl_work.Life < 80 && !((Rnd() >> (Rnd() & 3)) & 3)) {
            EM_RTN(pEm) = 0x501;
            if (!EM30_W(pEm)->Unk02)
                EM_RTN(pEm) = 0x201;
        } else if (EM30_W(pEm)->Unk02) {
            EM_RTN(pEm) = 0x401;
        }
    }
    if ((Pl_work.UnkBA & 0x3FFF) || (LONG)pEm->Unk154 < 1200) {
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
void func_80100FA8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
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
        Joint_move(pEm, Kan, Seq, 0x80);
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
void func_801010FC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    UCHAR Mot[2] = {0xB, 0xA};
    SVECTOR Sv;
    UCHAR Se[2] = {9, 6};
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM30_W(pEm)->Unk03 = Rnd() & 1;
        if (EM30_W(pEm)->Unk02 && EM30_W(pEm)->Unk03)
            EM30_W(pEm)->Unk03 = 0;
        EM_MOVE(pEm) = Mot[EM30_W(pEm)->Unk03] + 0x1F0000;
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
        if (Joint_move(pEm, Kan, Seq, (EM30_W(pEm)->Unk02 << 16) | 0x80)) {
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
                func_8001B484(0x39, 0x14000000, &GsIDMATRIX, &Sv, NULL);
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
                func_8001B484(0x39, 0x12000000, &GsIDMATRIX, &Sv, NULL);
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

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80101388);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80102250);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801023F4);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_8010252C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_8010258C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801025E0);

void func_801029AC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801029B4);

void func_80102E68(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80102E70);

void func_801032D4(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801032DC);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801035DC);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80103684);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80104590);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80104724);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80104920);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80104A9C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80104C58);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80104E58);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80104F20);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_8010532C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801053CC);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_8010549C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80105514);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80105740);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801059A8);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80105B4C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80105C8C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80105E94);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80106130);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80106710);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_8010682C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80106B3C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80106C28);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80106C8C);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80107EE0);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_801089B0);

INCLUDE_ASM("asm/nonmatchings/ovl/em30/em30", func_80108A28);
