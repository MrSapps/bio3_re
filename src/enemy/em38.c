#include "enemy/em38.h"
#include "joint.h"
#include "root_ck.h"
#include "sabun.h"
#include "sca.h"
#include "sld.h"
#include "snd.h"

/* Capcom's declarations in this module differ from the exe's: Rnd and Cdir_ck return int (signed
   arithmetic on Rnd, no andi after Cdir_ck), Dir_pos_ck takes int directions (no narrowing at the
   call), and func_800251FC (RE2 Joint_trans) is called with a fourth argument */
LONG Rnd_l(void) __asm__("Rnd");
LONG Dir_pos_ck_l(LONG *pPos, LONG *pTg, LONG Hed_dir, LONG Hani_dir) __asm__("Dir_pos_ck");
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");
void func_800251FC_(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg, LONG *pPos) __asm__("func_800251FC");

/* per-frame entry (RE2 EM_MAIN): sight check, routine, timers, effects, part chain and shadows */
void Em38_main(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Unused;                    /* frame size */
    VECTOR *pTv;
    PARTS_W *pP;
    PARTS_W *pW;
    KAGE_WORK **ppK;
    LONG i;
    SHORT Work;
    SHORT Bite;

    EM38_W(pEm)->Flg &= ~0x20;
    if (!Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[2].Workm.t, (VECTOR *)pPl->pSin_parts_ptr[1].Workm.t, 0x40, 1)) {
        EM38_W(pEm)->Lost_cnt = 0;
        EM38_W(pEm)->Flg |= 0x20;
    } else {
        EM38_W(pEm)->Lost_cnt++;
    }
    pTv = (VECTOR *)&pPl->Pos_x;
    if (EM38_W(pEm)->Flg & 2)
        pTv = (VECTOR *)&Em38_root[EM38_W(pEm)->Root_no];
    if (pEm->Type & 1) {
        pTv = &Em38_goal_pos;
        if (pEm->Pos_x >= -11999 && !Flg_ck(&G.Unk7888, 0x1D))
            pTv = &Em38_goal_pos2;
    }
    Root_ck(pEm, pTv, 0, 0);
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    EM38_W(pEm)->Old_dir = pEm->Cdir_y;
    Em38_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    EM38_W(pEm)->Flg &= ~0x80;
    if (!Hit_ck_point4(&pEm->Pos_x, G.Cccut))
        EM38_W(pEm)->Flg |= 0x80;
    Flg_off(&G.Unk7888, 0x1B);
    if (Hit_ck_point4(&pEm->Pos_x, &Em38_cut) && (pEm->Life < 203 || (pEm->Type & 1)))
        Flg_on(&G.Unk7888, 0x1B);
    if (EM38_W(pEm)->Atk_timer)
        EM38_W(pEm)->Atk_timer--;
    if (EM38_W(pEm)->Turn_timer)
        EM38_W(pEm)->Turn_timer--;
    if (EM38_W(pEm)->Bite_timer)
        EM38_W(pEm)->Bite_timer--;
    if (EM38_W(pEm)->Spit_timer)
        EM38_W(pEm)->Spit_timer--;
    if (EM38_W(pEm)->Wall_timer)
        EM38_W(pEm)->Wall_timer--;
    Em_seq_sound(pEm);
    Em38_morph_body(pEm);
    Em38_pustule_shell(pEm);
    Em38_foot_dust(pEm);
    Em38_breath(pEm);
    Em38_pustule(pEm);
    if (pEm->Be_flg & 8) {
        if (pEm->Unk178[2]) {
            pEm->Unk178[2] -= 16;
            if (pEm->Unk178[2] < 0)
                pEm->Unk178[2] = 0;
        }
        if (pEm->Unk178[0]) {
            pEm->Unk178[0] -= 16;
            if (pEm->Unk178[0] < 0)
                pEm->Unk178[0] = 0;
        }
        if (!pEm->Unk178[0] && !pEm->Unk178[2]) {
            pEm->Be_flg &= ~8;
            pW = pEm->pSin_parts_ptr;
            pW->UnkA2 &= ~0x40;
            pW = &pEm->pSin_parts_ptr[12];
            pW->UnkA2 &= ~0x40;
            pW = &pEm->pSin_parts_ptr[2];
            pW->UnkA2 &= ~0x40;
        }
    }
    if (!(EM38_W(pEm)->Flg & 0x10) && !(pEm->Type & 2)) {
        Em38_chain_set(pEm);
        Em38_chain_turn(pEm, &pEm->pSin_parts_ptr[16], &pEm->pSin_parts_ptr[17], 0x200);
        Em38_chain_turn(pEm, &pEm->pSin_parts_ptr[17], &pEm->pSin_parts_ptr[18], 0x2D4);
        Em38_chain_turn(pEm, &pEm->pSin_parts_ptr[18], &pEm->pSin_parts_ptr[19], 0x2D4);
    }
    if (!(pEm->Type & 2))
        Em38_head_move(pEm);
    if (!(EM38_W(pEm)->Flg & 0x10)) {
        Bite = Work = 0;
        if (*(USHORT *)&pEm->Routine_0 == 0x501 || *(USHORT *)&pEm->Routine_0 == 0x901)
            Bite = 1;
        pP = pEm->pSin_parts_ptr;
        pW = &pP[6];
        if ((SHORT)func_8004D720(pP[6].Workm.t, 500, 0x2000, 0) < pEm->Pos_y)
            Work += Bite + 1;
        if ((SHORT)func_8004D720(pP[7].Workm.t, 500, 0x2000, 0) < pEm->Pos_y)
            Work += Bite + 1;
        if ((SHORT)func_8004D720(pP[8].Workm.t, 500, 0x2000, 0) < pEm->Pos_y)
            Work += Bite + 1;
        if (pEm->Pos_y)
            Work = 0;
        pW->Size_y = pW->Size_y + (0x80 - Work * 128);
        if (pW->Size_y > 0x1000)
            pW->Size_y = 0x1000;
        if (pW->Size_y < 0x200)
            pW->Size_y = 0x200;
        Work = pW->Size_y;
        pW[1].Size_y = Work;
        pW[1].Pos_y = Work * 1125 / 4096;
        pW[2].Size_y = Work;
        pW[2].Pos_y = Work * 1124 / 4096;
    }
    for (i = 8, ppK = Em38_kage; i != 0; ppK++, i--)
        MulKage(*ppK, pEm->Unk122, 0, pEm->Be_flg & 0x30);
}

/* Routine_0 0: set up the body, shadows, extra collisions and parts, find the two tentacles,
   then start in the rising scenario (Routine_0 4) */
void Em38_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    tagGLOBAL *pG;
    PARTS_W *pP;
    PARTS_W *pO;
    ENEMY_WORK **ppE;
    ENEMY_WORK *pE;
    UCHAR Id;
    UCHAR i;

    pG = &G;
    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Damage_cnt = 0;
    EM38_W(pEm)->Atk_timer = 0;
    EM38_W(pEm)->Turn_timer = 0;
    EM38_W(pEm)->Bite_timer = 0;
    EM38_W(pEm)->Wall_timer = 90;
    EM38_W(pEm)->Spit_timer = 0;
    EM38_W(pEm)->Flg = 0;
    EM38_W(pEm)->Root_no = 0;
    EM38_W(pEm)->Lost_cnt = 0;
    pEm->Spd_x = 0;
    pEm->Spd_z = 0;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x6A406A4, 0, (VECTOR *)pEm->pSin_parts_ptr[1].Workm.t);
    pEm->Kage.pKw->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[0], 0, 0x4B004B0, 0, (VECTOR *)pEm->pSin_parts_ptr[2].Workm.t);
    Em38_kage[0]->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[1], 0, 0x4B004B0, 0, (VECTOR *)pEm->pSin_parts_ptr[16].Workm.t);
    Em38_kage[1]->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[2], 0, 0x4B004B0, 0, (VECTOR *)pEm->pSin_parts_ptr[13].Workm.t);
    Em38_kage[2]->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[3], 0, 0x3840384, 0, (VECTOR *)pEm->pSin_parts_ptr[9].Workm.t);
    Em38_kage[3]->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[4], 0, 0x3840384, 0, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t);
    Em38_kage[4]->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[5], 0, 0x3E803E8, 0, (VECTOR *)pEm->pSin_parts_ptr[17].Workm.t);
    Em38_kage[5]->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[6], 0, 0x2BC02BC, 0, (VECTOR *)pEm->pSin_parts_ptr[18].Workm.t);
    Em38_kage[6]->Be_flg |= 0x40;
    Kage_work_set(&Em38_kage[7], 0, 0x3200320, 0, (VECTOR *)pEm->pSin_parts_ptr[14].Workm.t);
    Em38_kage[7]->Be_flg |= 0x40;
    pEm->Col.H = 2500;
    pEm->Col.pEm = pEm;
    pEm->Col.W0 = 1800;
    pEm->Col.D0 = 1800;
    pEm->Col.Unk24 = 0;
    pEm->Col.Unk28 = 0;
    pEm->Col.Unk26 = 0;
    pEm->Col.Flg = 0;
    pEm->Col.Type = 2;
    pEm->Be_flg |= 0x8000;
    Id = func_80033580(&pEm->Col, 0);
    Em38_col[3].pEm = pEm;
    Em38_col[3].W0 = 900;
    Em38_col[3].D0 = 900;
    Em38_col[3].H = 200;
    Em38_col[3].Unk24 = 0;
    Em38_col[3].Unk28 = 0;
    Em38_col[3].Unk26 = 0;
    Em38_col[3].Flg = 0x80;
    Em38_col[3].Type = 2;
    func_80033580(&Em38_col[3], Id);
    Em38_col[3].pM = &pEm->pSin_parts_ptr[4].Workm;
    Em38_col[4].W0 = 900;
    Em38_col[4].D0 = 900;
    Em38_col[4].pEm = pEm;
    Em38_col[4].H = 800;
    Em38_col[4].Unk24 = 0;
    Em38_col[4].Unk28 = 0;
    Em38_col[4].Unk26 = 0;
    Em38_col[4].Flg = 0x80;
    Em38_col[4].Type = 2;
    func_80033580(&Em38_col[4], Id);
    Em38_col[4].pM = &pEm->pSin_parts_ptr[2].Workm;
    pEm->Unk136 = -800;
    pEm->Unk138 = 100;
    pEm->Unk139 = 230;
    pEm->Unk134 = 1800;
    pEm->Unk13A = 1600;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    pEm->Neck_no = 1;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr[1].Workm;
    pP = &pEm->pSin_parts_ptr[1];
    PW_NECK(pP)->Add_dir_y = 0x30;
    PW_NECK(pP)->Add_dir_z = 0x30;
    PW_NECK(pP)->L_rot_y = 0x19C;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    PW_NECK(pP)->L_rot_z = 0x138;
    pEm->Life = 800;
    pEm->Max_life = 800;
    if (pG->System_flg & 0x100) {
        pEm->Life = 600;
        pEm->Max_life = 600;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        EM_SET_MOVE(pEm, 0, 0, 0);
        pP = &pB[22];
        pO = &pB[1];
        pEm->Max_life += 100;
        pEm->Life += 100;
        pP->pSuper = &pO->Workm;
        pP->Pos_x = -0xF0;
        pP->Pos_y = -0x19F;
        pP->Pos_z = -0x3A2;
        pP->pOya_parts = pO;
        pP->Cdir_x = -0x2AA;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        EM38_SA(pP)->Key[0].Rate = 0;
        EM38_SA(pP)->Key[1].Rate = 0;
        EM38_SA(pP)->Key[2].Rate = 0;
        EM38_SA(pP)->Key[3].Rate = 0;
        EM38_SA(pP)->Key[4].Rate = 0;
        EM38_SA(pP)->Key[5].Rate = 0;
        EM38_SA(pP)->Key[6].Rate = 0;
        pB[22].Be_flg = (pB[22].Be_flg & ~1) | 4;
        pP->Size_x = 0x1000;
        pP->Size_y = 0;
        pP->Size_z = 0x1000;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[23];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->Pos_x = -3;
        pP->Pos_y = -0x66C;
        pP->pOya_parts = pO;
        pP->Pos_z = -0x21F;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        pP->UnkB4 = 0;
        pB[23].Be_flg &= ~1;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[24];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->Pos_x = 0xC2;
        pP->Pos_y = -0x3D6;
        pP->pOya_parts = pO;
        pP->Pos_z = -0x405;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        pP->UnkB4 = 0;
        pB[24].Be_flg &= ~1;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[25];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->Pos_x = -0x355;
        pP->Pos_y = -0x381;
        pP->pOya_parts = pO;
        pP->Pos_z = -0x34F;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        pP->UnkB4 = 0;
        pB[25].Be_flg &= ~1;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[26];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->Pos_x = -0x35D;
        pP->Pos_y = -0x520;
        pP->pOya_parts = pO;
        pP->Pos_z = -0x37;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        pP->UnkB4 = 0;
        pB[26].Be_flg &= ~1;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[27];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->pOya_parts = pO;
        pP->Pos_x = 0;
        pP->Pos_y = 0;
        pP->Pos_z = 0;
        pP->UnkA2 |= 0x100;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        Md_set_abr(pP->pTmd, 1);
        pP->UnkB4 = 0;
        pB[27].Be_flg &= ~1;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[28];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->pOya_parts = pO;
        pP->Pos_x = 0;
        pP->Pos_y = 0;
        pP->Pos_z = 0;
        pP->UnkA2 |= 0x100;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        Md_set_abr(pP->pTmd, 1);
        pP->UnkB4 = 0;
        pB[28].Be_flg &= ~1;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[29];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->pOya_parts = pO;
        pP->Pos_x = 0;
        pP->Pos_y = 0;
        pP->Pos_z = 0;
        pP->UnkA2 |= 0x100;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        Md_set_abr(pP->pTmd, 1);
        pP->UnkB4 = 0;
        pB[29].Be_flg &= ~1;
    }
    {
        PARTS_W *pB = pEm->pSin_parts_ptr;

        pP = &pB[30];
        pO = &pB[1];
        pP->pSuper = &pO->Workm;
        pP->pOya_parts = pO;
        pP->Pos_x = 0;
        pP->Pos_y = 0;
        pP->Pos_z = 0;
        pP->UnkA2 |= 0x100;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        Md_set_abr(pP->pTmd, 1);
        pP->UnkB4 = 0;
        pB[30].Be_flg &= ~1;
    }
    pEm->Be_flg |= 0x200000;
    pG->pPldamage_Em[pEm->Id - 0x10] = Pldamege_Em38;
    pG->pPldie_Em[pEm->Id - 0x10] = Pldie_Em38;
    EM38_W(pEm)->pTentacle0 = NULL;
    pEm->Timer0 = 0;
    ppE = &pG->pEnemy[2];
    do {
        pE = *ppE;
        if (pEm->Timer0 == 1 && pE->Id == 0x39) {
            EM38_W(pEm)->pTentacle1 = pE;
            pEm->Timer0 = 2;
        }
        if (pEm->Timer0 == 0 && pE->Id == 0x39) {
            EM38_W(pEm)->pTentacle0 = pE;
            pEm->Timer0 = 1;
        }
        ppE++;
    } while (ppE != pG->ppEnemy_end);
    EM_SET_RTN(pEm, 4, 0, 0, 0);
    pEm->Timer0 = 0;
    EM38_W(pEm)->Flg |= 0x10;
    pEm->Damage_cnt |= 0x80;
    pEm->Be_flg |= 0x20;
    EM38_W(pEm)->pTentacle0->Be_flg |= 0x10;
    EM38_W(pEm)->pTentacle1->Be_flg |= 0x10;
    pP = pEm->pSin_parts_ptr;
    i = 22;
    do {
        pP->Be_flg |= 4;
        pP->Size_x = 0x999;
        pP->Size_y = 0x999;
        pP->Size_z = 0x999;
        Em38_part_scale_pos(pEm, pP, 0x999);
        pP++;
    } while (--i);
    pP = &pEm->pSin_parts_ptr[4];
    pP->Size_x = 0x999;
    pP->Size_y = 0x400;
    pP->Size_z = 0x999;
    pP = &pEm->pSin_parts_ptr[5];
    pP->Size_x = 0x999;
    pP->Size_y = 0x400;
    pP->Size_z = 0x999;
    Em38_part_scale_pos(pEm, pP, 0x400);
    pP = &pEm->pSin_parts_ptr[6];
    pP->Size_x = 0x999;
    pP->Size_y = 0x400;
    pP->Size_z = 0x999;
    Em38_part_scale_pos(pEm, pP, 0x400);
    pP = &pEm->pSin_parts_ptr[7];
    pP->Size_x = 0x999;
    pP->Size_y = 0x400;
    pP->Size_z = 0x999;
    Em38_part_scale_pos(pEm, pP, 0x400);
    pP = &pEm->pSin_parts_ptr[8];
    pP->Size_x = 0x999;
    pP->Size_y = 0x400;
    pP->Size_z = 0x999;
    Em38_part_scale_pos(pEm, pP, 0x400);
    pP = &pEm->pSin_parts_ptr[16];
    pP->Size_x = 0;
    pP->Size_y = 0;
    pP->Size_z = 0;
    pP = &pEm->pSin_parts_ptr[17];
    pP->Size_x = 0;
    pP->Size_y = 0;
    pP->Size_z = 0;
    Em38_part_scale_pos(pEm, pP, 0);
    pP = &pEm->pSin_parts_ptr[18];
    pP->Size_x = 0;
    pP->Size_y = 0;
    pP->Size_z = 0;
    Em38_part_scale_pos(pEm, pP, 0);
    pP = &pEm->pSin_parts_ptr[19];
    pP->Size_x = 0;
    pP->Size_y = 0;
    pP->Size_z = 0;
    Em38_part_scale_pos(pEm, pP, 0);
    pEm->Unk12A = 0x999;
    EM_SET_MOVE(pEm, 0x18, 0, 0);
    Joint_move(pEm, pKan, pSeq, 0x80);
    pP = pEm->pSin_parts_ptr;
    EM38_SA(pP)->Key[0].Rate = 0x1000;
    func_8004AC54(pEm, pP);
    pP = &pEm->pSin_parts_ptr[12];
    EM38_SA(pP)->Key[0].Rate = 0x1000;
    func_8004AC54(pEm, pP);
    pP = &pEm->pSin_parts_ptr[2];
    EM38_SA(pP)->Key[0].Rate = 0x1000;
    func_8004AC54(pEm, pP);
    pP = &pEm->pSin_parts_ptr[20];
    EM38_SA(pP)->Key[0].Rate = 0;
    EM38_SA(pP)->Key[1].Rate = 0;
    func_8004AC54(pEm, pP);
}

/* Routine_0 1: move routine set by Type */
void Em38_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em38_Move_type[pEm->Type & 3](pEm, pKan, pSeq);
}

/* brain routine, then motion routine, by Routine_1 */
void Em38_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em38_mv_br_type00[pEm->Routine_1](pEm);
    Em38_mv_type00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* mv_br[0]: start walking when the player comes within 5000 */
void Em38_wait_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG L_pl;

    if (pEm->Routine_2 == 0) {
        pEm->Col.W0 = 1800;
        pEm->Col.D0 = 1800;
        pEm->Unk134 = 1800;
        pEm->Be_flg &= ~0x2000;
        EM38_W(pEm)->Flg &= ~0xC;
        pEm->Be_flg &= ~0x20;
    }
    L_pl = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    if (L_pl < 5000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* mv[0]: stand */
void Em38_wait(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 1;
        EM38_W(pEm)->Flg &= ~2;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    }
}

/* mv_br[1], mv_br[2]: walking towards the player, pick an attack */
void Em38_walk_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG L_pl;
    SHORT Ck_100, Ck_280, Ck_600, Ck_400;
    SHORT Ck_l, Ck_r;

    if (pEm->Routine_2 == 0) {
        pEm->Col.W0 = 1800;
        pEm->Col.D0 = 1800;
        pEm->Unk134 = 1800;
        pEm->Be_flg &= ~0x2000;
        EM38_W(pEm)->Flg &= ~0xC;
        pEm->Be_flg &= ~0x20;
    }
    L_pl = pEm->Unk154;
    Ck_100 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Ck_280 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    Ck_600 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Ck_l = Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x600, 0x200);
    Ck_r = Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y - 0x600, 0x200);
    Ck_400 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    if (L_pl < 2500 && !Ck_280)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (!Ck_l && !EM38_W(pEm)->Turn_timer && !EM38_W(pEm)->Lost_cnt) {
        EM_SET_RTN(pEm, 1, 4, 0, 0);
        pEm->UnkE3 = 1;
    }
    if (!Ck_r && !EM38_W(pEm)->Turn_timer && !EM38_W(pEm)->Lost_cnt) {
        EM_SET_RTN(pEm, 1, 4, 0, 0);
        pEm->UnkE3 = 0;
    }
    if (L_pl < 4000 && !Ck_280 && !EM38_W(pEm)->Atk_timer && pEm->Move_cnt > 20) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (!(Rnd_l() & 3) || (pPl->Life < 20 && (Rnd_l() & 3)))
            EM_SET_RTN(pEm, 1, 9, 0, 0);
    }
    if (L_pl < 3000 && !Ck_400 && !EM38_W(pEm)->Atk_timer && pEm->Move_cnt > 20)
        EM_SET_RTN(pEm, 1, 9, 0, 0);
    if (L_pl > 6000 && !Ck_100 && !EM38_W(pEm)->Spit_timer && !EM38_W(pEm)->Lost_cnt)
        EM_SET_RTN(pEm, 1, 8, 0, 0);
    if (L_pl < 5000 && Ck_600 && !EM38_W(pEm)->Bite_timer)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    if (EM38_W(pEm)->Lost_cnt > 60 && !EM38_W(pEm)->Wall_timer && L_pl > 5000 && !Flg_ck(&G.Unk7888, 0x1C))
        EM_SET_RTN(pEm, 1, 0xA, 0, 0);
}

/* mv[1], mv[2]: walk towards Dest */
void Em38_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x7F);
        pEm->Routine_2 = 1;
        EM38_W(pEm)->Flg &= ~2;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 7;
        Joint_move(pEm, pKan, pSeq, 0x20);
        break;
    }
    Add = 2;
    if (pEm->Spd_x > 30)
        Add = 6;
    if (pEm->Spd_x > 20)
        Add += 4;
    if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x10))
        Add = 0x10;
    if (EM38_W(pEm)->Flg & 0x80) {
        Add += 0x40;
        pEm->Spd_x += 30;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

/* mv_br[3]: stop backing off when far, swallow when close */
void Em38_back_br(ENEMY_WORK *pEm)
{
    ULONG L_pl;
    SHORT Ck;

    L_pl = pEm->Unk154;
    Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x400);
    Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x200);
    Ck = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x600);
    if (L_pl > 4000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (L_pl < 3000 && Ck && !EM38_W(pEm)->Atk_timer)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
}

/* mv[3]: back off for 90 frames */
void Em38_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 0x7F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 90;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 7;
        Joint_move(pEm, pKan, pSeq, 0x20);
        if (--pEm->Timer0 == -1)
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        break;
    }
    Add = (pEm->Spd_x < -10) * 2;
    if (pEm->Spd_x < -20)
        Add += 2;
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

/* no brain step for this move */
void Em38_turn_br(ENEMY_WORK *pEm)
{
}

/* mv[4]: turn a quarter to the side the player is on (UnkE3) */
void Em38_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Old_x, Old_z;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE_V(pEm, pEm->UnkE3 + 3, 0, 0x1F);
        EM38_W(pEm)->Flg |= 8;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            pEm->Cdir_y -= 0x400;
            if (pEm->UnkE3)
                pEm->Cdir_y += 0x800;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM38_W(pEm)->Turn_timer = 60;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 4);
            EM38_W(pEm)->Flg &= ~0xC;
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

/* no brain step for this move */
void Em38_claw_br(ENEMY_WORK *pEm)
{
}

/* mv[5]: claw attack */
void Em38_claw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    SVECTOR Sv;
    HIT_AREA Area;
    SHORT Old_x, Old_z;
    LONG Hit;
    LONG Dir_old;
    LONG Dir;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        EM38_W(pEm)->Flg &= ~2;
        Snd_se_enem((Rnd_l() & 1) ? 10 : 9, pEm);
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Atk_timer = 30;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (pEm->Move_cnt < 16)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
        if ((*pEm->pNow_seq & 0x200) && !pPl->Damage_cnt) {
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 1300;
            Hit = func_80044420(&pP[5].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[6].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[7].Workm, &Sv, pPl);
            if (Hit) {
                pEm->UnkE3 = -0x80;
                Dir_old = pEm->Cdir_y;
                pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z) - 0x400;
                Dir = pEm->Cdir_y;
                Hit = Cdir_ck_l(pEm, pPl);
                pEm->Cdir_y = Dir_old;
                pPl->pEnemy_ptr = pEm;
                EM_RTN(pPl) = (Hit << 8) | 5;
                pPl->Damage_cnt |= 0x80;
                Dir -= 0x800;
                if (Hit)
                    Dir += 0x800;
                Goto01(pPl, Dir, 0x200);
                Snd_se_stad(0x1020D, (VECTOR *)&pEm->Pos_x, 0, 0);
                Pl_life_down(30, 1);
                Pad_vib_set0(3, 0);
                Pad_vib_set1(10, 250, 0);
            }
        }
        break;
    }
    Add_speedXZ(pEm, 0);
    if (pEm->Move_cnt >= 30 && pEm->Move_cnt < 35) {
        Area.pPos = (VECTOR *)&pEm->Pos_x;
        Area.pBox = Em38_box0;
        Area.Dir = pEm->Cdir_y;
        Area.Id = 0x67;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
    }
    if (pEm->Move_cnt >= 34 && pEm->Move_cnt < 36) {
        Area.pPos = (VECTOR *)&pEm->Pos_x;
        Area.pBox = Em38_box1;
        Area.Dir = pEm->Cdir_y;
        Area.Id = 0x67;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
    }
    if (pEm->Move_cnt == 36) {
        Area.pPos = (VECTOR *)&pEm->Pos_x;
        Area.pBox = Em38_box2;
        Area.Dir = pEm->Cdir_y;
        Area.Id = 0x67;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
    }
    if (pEm->Move_cnt >= 36 && pEm->Move_cnt < 39) {
        Area.pPos = (VECTOR *)&pEm->Pos_x;
        Area.pBox = Em38_box3;
        Area.Dir = pEm->Cdir_y;
        Area.Id = 0x41;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
    }
}

/* no brain step for this move */
void Em38_swallow_br(ENEMY_WORK *pEm)
{
}

/* mv[6]: bite and swallow */
void Em38_swallow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    HIT_AREA Area;
    PARTS_W *pP;
    SHORT Old_x, Old_z;
    LONG Hit;
    LONG Dir_old;
    LONG Dir;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 6, 0, 0x1F);
        EM38_W(pEm)->Flg |= 0xC;
        pEm->Routine_2 = 1;
        pEm->Col.W0 = 1000;
        pEm->Col.D0 = 1000;
        EM38_W(pEm)->Flg &= ~2;
        pEm->UnkE3 = 0;
        Snd_se_enem(8, pEm);
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Bite_timer = 30;
            if (pPl->Life < 20)
                EM38_W(pEm)->Bite_timer = 5;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Col.W0 = 1800;
            pEm->Col.D0 = 1800;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            EM38_W(pEm)->Flg &= ~0xC;
        }
        if ((*pEm->pNow_seq & 0x200) && !pPl->Damage_cnt) {
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, -0x10);
            pP = pEm->pSin_parts_ptr;
            Sv.vx = -1800;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 1300;
            Hit = func_80044420(&pP->Workm, &Sv, pPl);
            Sv.vx = -2200;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 800;
            Hit |= func_80044420(&pP->Workm, &Sv, pPl);
            if (Hit) {
                pEm->UnkE3 = -0x58;
                Dir_old = pEm->Cdir_y;
                pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z) - 0x800;
                Dir = pEm->Cdir_y;
                Hit = Cdir_ck_l(pEm, pPl);
                pEm->Cdir_y = Dir_old;
                pPl->pEnemy_ptr = pEm;
                pPl->Damage_cnt |= 0x80;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                EM_RTN(pPl) = ((Hit + 4) << 8) | 5;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                Dir -= 0x800;
                if (Hit)
                    Dir += 0x800;
                Goto01(pPl, Dir, 0x200);
                Snd_se_enem(2, pEm);
                if (Pl_life_down(101, 0) == 2) {
                    EM_SET_RTN(pPl, 6, 1, 0, 0);
                    G.System_flg |= 0x800;
                }
                Pad_vib_set0(4, 0);
                Pad_vib_set0(2, 8);
                Pad_vib_set1(5, 250, 0);
                Pad_vib_fade_set(30, 250, 40, 5);
                Pad_vib_set0(2, 12);
                Pad_vib_set0(3, 15);
                Pad_vib_set0(2, 20);
                pP = pPl->pSin_parts_ptr;
                Sv.vx = 0;
                Sv.vy = -700;
                Sv.vz = 0;
                Esp_call(0x300, (((Rnd_l() << 3) + 0xBD0) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
                Esp_call(0x300, (((Rnd_l() << 3) + 0xBD0) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
                Esp_call(0x1900, (((Rnd_l() << 3) + 0xBD0) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
                Esp_call(0x300, (((Rnd_l() << 3) + 0xBD0) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, 0, 0);
                Esp_call(0x300, (((Rnd_l() << 3) + 0xBD0) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, 0, 0);
                Esp_call(0x1900, (((Rnd_l() << 3) + 0xBD0) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, 0, 0);
            }
        }
        if (pEm->UnkE3 & 0x7F) {
            pP = pEm->pSin_parts_ptr;
            pEm->UnkE3--;
            Sv.vx = -0x368 - Rnd_l();
            Sv.vy = 0x80 - Rnd_l();
            Sv.vz = 0;
            Esp_call(0x1900, (((Rnd_l() << 3) + 0x1000) << 16) | (USHORT)(Rnd_l() << 4), &pP[20].Workm, &Sv, 0);
            Sv.vy = 0x80 - Rnd_l();
            Esp_call(0x1600, (((Rnd_l() << 3) + 0x800) << 16) | (USHORT)(Rnd_l() << 4), &pP[20].Workm, &Sv, 0);
            pP = &pEm->pSin_parts_ptr[21];
            Sv.vx = -0x368 - Rnd_l();
            Sv.vy = 0x80 - Rnd_l();
            Sv.vz = 0;
            Esp_call(0x1900, (((Rnd_l() << 3) + 0x1000) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
            Sv.vy = 0x80 - Rnd_l();
            Esp_call(0x1600, (((Rnd_l() << 3) + 0x800) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
        }
        pP = &pEm->pSin_parts_ptr[20];
        if (pEm->Move_cnt <= 32) {
            if (pEm->Move_cnt == 20)
                Snd_se_enem(11, pEm);
            EM38_SA(pP)->Key[0].Rate += 0x48;
            if (pEm->Move_cnt & 1)
                EM38_SA(pP)->Key[0].Rate = EM38_SA(pP)->Key[0].Rate + 0x36;
            else
                EM38_SA(pP)->Key[0].Rate = EM38_SA(pP)->Key[0].Rate - 0x36;
            if (EM38_SA(pP)->Key[0].Rate > 0x800)
                EM38_SA(pP)->Key[0].Rate = 0x800;
        }
        if (pEm->Move_cnt >= 40 && pEm->Move_cnt < 45) {
            EM38_SA(pP)->Key[0].Rate += 0x140;
            if (EM38_SA(pP)->Key[0].Rate > 0x1000)
                EM38_SA(pP)->Key[0].Rate = 0x1000;
        }
        if (pEm->Move_cnt >= 34 && pEm->Move_cnt < 48)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, -0x20);
        if (pEm->Move_cnt < 40 && Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x59C)) {
            EM38_W(pEm)->pTentacle0->Spd_z = 30;
            EM38_W(pEm)->pTentacle1->Spd_z = 30;
        }
        if (pEm->Move_cnt >= 76) {
            EM38_SA(pP)->Key[0].Rate -= 0x100;
            if (EM38_SA(pP)->Key[0].Rate < 0)
                EM38_SA(pP)->Key[0].Rate = 0;
        }
        if (pEm->Move_cnt >= 66) {
            pEm->Col.W0 += 50;
            if (pEm->Col.W0 > 1800)
                pEm->Col.W0 = 1800;
            pEm->Col.D0 = pEm->Col.W0;
        }
        func_8004A934(pEm, pP);
        break;
    }
    Add_speedXZ(pEm, 0);
    if (pEm->Move_cnt >= 39 && pEm->Move_cnt < 48) {
        Area.pPos = (VECTOR *)&pEm->Pos_x;
        Area.pBox = Em38_box6;
        Area.Dir = pEm->Cdir_y;
        Area.Id = 0x42;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
        Area.pBox = Em38_box7;
        Area.Id = 0x41;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
    }
}

/* no brain step for this move */
void Em38_spit_br(ENEMY_WORK *pEm)
{
}

/* mv[7]: spit acid */
void Em38_spit(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pW;
    MATRIX *pM;
    SHORT Old_x, Old_z;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 7, 0, 0x1F);
        pEm->Routine_2 = 1;
        Snd_se_enem(10, pEm);
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (pEm->Move_cnt < 48)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x10);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Spit_timer = (Rnd_l() & 0xF) + 15;
            EM_SET_RTN(pEm, 1, 0xD, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            if (EM38_W(pEm)->Wall_spit)
                EM38_W(pEm)->Wall_spit--;
        }
        if (pEm->Move_cnt == 48) {
            pP = pEm->pSin_parts_ptr;
            pW = &pP[2];
            Sv.vx = pW->Pos_x + 900;
            Sv.vy = pW->Pos_y - 400;
            Sv.vz = pW->Pos_z;
            pM = &pP[1].Workm;
            Esp_call(0xB40, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
            Esp_call(0xD40, (USHORT)(pEm->Cdir_y - (Rnd_l() << 2) + 0x300 - (Rnd_l() << 1)) | 0x10000000, pM, &Sv, 0);
            Esp_call(0xB40, (USHORT)(pEm->Cdir_y - (Rnd_l() << 2) + 0x300 - (Rnd_l() << 1)) | 0x0A000000, pM, &Sv, 0);
            Esp_call(0xF40, (USHORT)(pEm->Cdir_y - (Rnd_l() << 2) + 0x300 - (Rnd_l() << 1)) | 0x10000000, pM, &Sv, 0);
            Esp_call(0xB40, (USHORT)(pEm->Cdir_y - (Rnd_l() << 2) + 0x300 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0xD40, (USHORT)(pEm->Cdir_y - (Rnd_l() << 2) + 0x300 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0x1240, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0xE40, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0E000000, pM, &Sv, 0);
            Esp_call(0x1240, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0xE40, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0E000000, pM, &Sv, 0);
            Esp_call(0xE40, (USHORT)(pEm->Cdir_y + 0x364 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0x1240, (USHORT)(pEm->Cdir_y - 0x100 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0xE40, (USHORT)(pEm->Cdir_y - 0x100 - (Rnd_l() << 1)) | 0x0E000000, pM, &Sv, 0);
            Esp_call(0x1240, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0xE40, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0E000000, pM, &Sv, 0);
            Esp_call(0xE40, (USHORT)(pEm->Cdir_y + 0x364 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Snd_se_enem(6, pEm);
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

/* no brain step for this move */
void Em38_spit_far_br(ENEMY_WORK *pEm)
{
}

/* mv[8]: spit acid, long range */
void Em38_spit_far(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;
    SHORT Old_x, Old_z;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 8, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 0;
        EM38_W(pEm)->Flg &= ~2;
        Snd_se_enem(10, pEm);
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (pEm->Move_cnt < 16)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
        if (pEm->Move_cnt >= 20 && pEm->Move_cnt < 31)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 12);
        if (pEm->Move_cnt == 49) {
            pW = &pEm->pSin_parts_ptr[2];
            Sv.vx = 0;
            Sv.vy = pEm->Cdir_y;
            Sv.vz = 0;
            RotMatrix(&Sv, &Em38_spit_m);
            Em38_spit_m.t[0] = pW->Workm.t[0];
            Em38_spit_m.t[1] = pW->Workm.t[1];
            Em38_spit_m.t[2] = pW->Workm.t[2];
            Esp_call(0x440, (USHORT)pEm->Cdir_y | 0x18000000, &Em38_spit_m, 0, 0);
            Snd_se_enem(6, pEm);
        }
        if (pEm->Move_cnt == 50) {
            pP = pEm->pSin_parts_ptr;
            pW = &pP[2];
            Sv.vx = pW->Pos_x + 900;
            Sv.vy = pW->Pos_y - 400;
            Sv.vz = pW->Pos_z;
            Esp_call(0xB40, (USHORT)(pEm->Cdir_y + 0x200 - (Rnd_l() << 2)) | 0x0C000000, &pP[1].Workm, &Sv, 0);
            Esp_call(0xB40, (USHORT)(pEm->Cdir_y + 0x200 - (Rnd_l() << 2)) | 0x0C000000, &pP[1].Workm, &Sv, 0);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Spit_timer = 90;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (pEm->Move_cnt == 55 && !(Rnd_l() & 1) && !EM38_W(pEm)->Lost_cnt) {
            pEm->Move_cnt = 20;
            pEm->Routine_2 = 2;
            pEm->Hokan_flg = 31;
        }
        break;
    case 2:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 20) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (pEm->UnkE2 == 1 && pEm->Move_cnt == 30) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        pEm->Spd_x -= pEm->Spd_x >> 2;
        if (pEm->Move_cnt >= 20 && pEm->Move_cnt < 45)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 12);
        if (pEm->Move_cnt == 49) {
            pW = &pEm->pSin_parts_ptr[2];
            Sv.vx = 0;
            Sv.vy = pEm->Cdir_y;
            Sv.vz = 0;
            RotMatrix(&Sv, &Em38_spit_m);
            Em38_spit_m.t[0] = pW->Workm.t[0];
            Em38_spit_m.t[1] = pW->Workm.t[1];
            Em38_spit_m.t[2] = pW->Workm.t[2];
            Esp_call(0x440, (USHORT)pEm->Cdir_y | 0x18000000, &Em38_spit_m, 0, 0);
            Snd_se_enem(6, pEm);
        }
        if (pEm->Move_cnt == 50) {
            pS = pEm->pSin_parts_ptr;
            pW = &pS[2];
            Sv.vx = pW->Pos_x + 900;
            Sv.vy = pW->Pos_y - 400;
            Sv.vz = pW->Pos_z;
            Esp_call(0xC40, (USHORT)(pEm->Cdir_y + 0x200 - (Rnd_l() << 2)) | 0x0C000000, &pS[1].Workm, &Sv, 0);
            Esp_call(0xC40, (USHORT)(pEm->Cdir_y + 0x200 - (Rnd_l() << 2)) | 0x0C000000, &pS[1].Workm, &Sv, 0);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Spit_timer = 120;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (!pEm->UnkE2 && pEm->Move_cnt == 55 && !(Rnd_l() & 7) && !EM38_W(pEm)->Lost_cnt) {
            pEm->Move_cnt = 30;
            pEm->Hokan_flg = 31;
            pEm->UnkE2 = 1;
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

/* no brain step for this move */
void Em38_grab_br(ENEMY_WORK *pEm)
{
}

/* mv[9]: grab and bite */
void Em38_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    SVECTOR Sv;
    HIT_AREA Area;
    SHORT Old_x, Old_z;
    LONG Hit;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 9, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        Snd_se_enem(9, pEm);
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (pEm->Move_cnt < 20)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
        if (pEm->Move_cnt >= 41 && pEm->Move_cnt < 50)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x40);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Atk_timer = 30;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (pEm->UnkE3 & 0x80)
                EM38_W(pEm)->Atk_timer = 80;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if ((*pEm->pNow_seq & 0x200) && !pPl->Damage_cnt) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 800;
            Hit = func_80044420(&pP[5].Workm, &Sv, pPl) | func_80044420(&pP[6].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[7].Workm, &Sv, pPl);
            Sv.pad = 1000;
            Hit |= func_80044420(&pEm->pSin_parts_ptr[5].Workm, &Sv, pPl);
            if (Hit) {
                pEm->UnkE3 = -0x80;
                pPl->pEnemy_ptr = pEm;
                pPl->Damage_cnt |= 0x80;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                EM_SET_RTN(pPl, 5, 2, 0, 0);
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                Snd_se_stad(0x1020D, (VECTOR *)&pEm->Pos_x, 0, 0);
                Pad_vib_set0(4, 0);
                Pad_vib_set1(10, 250, 0);
                if (Pl_life_down(50, 0) == 2) {
                    EM_SET_RTN(pPl, 6, 0, 0, 0);
                    G.System_flg |= 0x800;
                    Pad_vib_fade_set(45, 250, 40, 10);
                }
            }
        }
        break;
    }
    Add_speedXZ(pEm, 0);
    if (pEm->Move_cnt >= 48 && pEm->Move_cnt < 55) {
        Area.pPos = (VECTOR *)&pEm->Pos_x;
        Area.pBox = Em38_box4;
        Area.Dir = pEm->Cdir_y;
        Area.Id = 0x42;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
        Area.pBox = Em38_box5;
        Area.Id = 0x41;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
    }
}

/* mv_br[10]: attack on the way to the wall if the player comes close */
void Em38_climb_br(ENEMY_WORK *pEm)
{
    ULONG L_pl;
    SHORT Ck0, Ck1;

    L_pl = pEm->Unk154;
    Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x80);
    Ck0 = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x200);
    Ck1 = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x600);
    if (pEm->Routine_2 < 2) {
        if (L_pl < 4000 && !Ck0 && !EM38_W(pEm)->Atk_timer) {
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            if (!(Rnd_l() & 3))
                EM_SET_RTN(pEm, 1, 9, 0, 0);
        }
        if (L_pl < 5000 && Ck1 && !EM38_W(pEm)->Bite_timer)
            EM_SET_RTN(pEm, 1, 6, 0, 0);
    }
}

/* mv[10]: walk to the nearest zone and climb into the wall */
void Em38_climb(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs = { 0x898 };
    SVECTOR Sv;
    MATRIX M;
    VECTOR V;
    LONG Base;
    LONG Min;
    LONG L;
    LONG Dx, Dz;
    LONG n;
    LONG k;
    LONG Near;
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x7F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        EM38_W(pEm)->Flg |= 2;
        Base = !Flg_ck(&G.Unk7888, 0x1D) << 2;
        k = Base;
        for (n = 4; n != 0; n--) {
            if (Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[k]))) {
                k |= 0x80;
                break;
            }
            k++;
        }
        if (k & 0x80) {
            EM38_W(pEm)->Root_no = k & 0x7F;
        } else {
            n = 4;
            k = Base;
            Min = 0x7FFFFFFF;
            do {
                Dx = (pEm->Pos_x - Em38_root[k].Pos_x) >> 1;
                Dz = (pEm->Pos_z - Em38_root[k].Pos_z) >> 1;
                L = SquareRoot0(Dx * Dx + Dz * Dz) * 2;
                if (L < Min) {
                    Near = k;
                    Min = L;
                }
                k++;
                n--;
            } while (n);
            EM38_W(pEm)->Root_no = Near;
        }
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x += 5;
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 7;
        Joint_move(pEm, pKan, pSeq, 0x20);
        Add = 8;
        if (pEm->Spd_x > 30)
            Add = 16;
        if (pEm->Spd_x > 20)
            Add += 8;
        if (EM38_W(pEm)->Flg & 0x80) {
            Add += 0x40;
            pEm->Spd_x += 30;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
        Add_speedXZ(pEm, 0);
        if (Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[EM38_W(pEm)->Root_no])))
            pEm->Routine_2 = 2;
        if (pEm->Timer0)
            pEm->Timer0--;
        if (!EM38_W(pEm)->Lost_cnt && (LONG)pEm->Unk154 < 8000 && !pEm->Timer0) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Timer0 = 30;
            EM38_W(pEm)->Flg &= ~2;
        }
        break;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x20);
        Add = 8;
        if (pEm->Spd_x > 30)
            Add = 16;
        if (pEm->Spd_x > 20)
            Add += 8;
        if (EM38_W(pEm)->Flg & 0x80)
            Add += 0x40;
        Goto01(pEm, Em38_root[EM38_W(pEm)->Root_no].Dir, Add);
        Add_speedXZ(pEm, 0);
        if (pEm->Timer0)
            pEm->Timer0--;
        if (!Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[EM38_W(pEm)->Root_no])) && !pEm->Timer0) {
            pEm->Routine_2 = 1;
            pEm->Timer0 = 30;
            Base = !Flg_ck(&G.Unk7888, 0x1D) << 2;
            n = 4;
            k = Base;
            for (; n != 0; n--) {
                if (Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[k]))) {
                    k |= 0x80;
                    break;
                }
                k++;
            }
            if (k & 0x80) {
                EM38_W(pEm)->Root_no = k & 0x7F;
            } else {
                n = 4;
                k = Base;
                Min = 0x7FFFFFFF;
                do {
                    Dx = (pEm->Pos_x - Em38_root[k].Pos_x) >> 1;
                    Dz = (pEm->Pos_z - Em38_root[k].Pos_z) >> 1;
                    L = SquareRoot0(Dx * Dx + Dz * Dz) * 2;
                    if (L < Min) {
                        Near = k;
                        Min = L;
                    }
                    k++;
                    n--;
                } while (n);
                EM38_W(pEm)->Root_no = Near;
            }
            break;
        }
        RotMatrix((SVECTOR *)&pEm->Cdir_x, &M);
        ApplyMatrixSV(&M, &Ofs, &Sv);
        V.vx = pEm->Pos_x + Sv.vx;
        V.vy = pEm->Pos_y;
        V.vz = pEm->Pos_z + Sv.vz;
        func_8004D720(&V.vx, 1, 0x200, 0);
        if (!Goto01_ck(pEm, Em38_root[EM38_W(pEm)->Root_no].Dir, 0x40) && (pEm->Col.Hit_flg & 0x20)
            && Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[EM38_W(pEm)->Root_no]))) {
            pEm->Routine_2 = 3;
            pEm->nFloor = 2;
            EM_SET_MOVE(pEm, 0xC, 0, 0x1F);
            pEm->Be_flg |= 0x2000;
            EM38_W(pEm)->Flg |= 0xC;
        }
        break;
    case 3:
        if (pEm->Move_cnt < 16)
            Goto01(pEm, Em38_root[EM38_W(pEm)->Root_no].Dir, 0x10);
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 0xD, 0, 0);
            EM38_W(pEm)->Wall_timer = 90;
            pEm->Be_flg &= ~0x2000;
            EM38_W(pEm)->Flg &= ~0xC;
            EM38_W(pEm)->Wall_spit = (Rnd_l() & 3) + 1;
        }
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 46) {
            pEm->Pos_y = -3600;
            pEm->Col.W0 = 2000;
            pEm->Col.D0 = 2000;
            pEm->Unk122 = -3600;
            pEm->Unk134 = 0;
            pEm->Be_flg |= 0x20;
        }
        break;
    case 4:
        break;
    }
}

/* no brain step for this move */
void Em38_drop_br(ENEMY_WORK *pEm)
{
}

/* mv[11]: walk to the zone nearest the player and drop down from the wall */
void Em38_drop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Ofs = { 0x960 };
    SVECTOR Sv;
    MATRIX M;
    VECTOR V;
    EM38_ROOT *pR;
    LONG Base;
    LONG Min;
    LONG L;
    LONG Dx, Dz;
    LONG n;
    LONG k;
    LONG Near;
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x7F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        EM38_W(pEm)->Flg |= 2;
        Base = !Flg_ck(&G.Unk7888, 0x1D) << 2;
        k = Base;
        for (n = 4; n != 0; n--) {
            if (Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[k + 8]))) {
                k |= 0x80;
                break;
            }
            k++;
        }
        if (k & 0x80) {
            EM38_W(pEm)->Root_no = (k & 0x7F) + 8;
        } else {
            n = 4;
            k = Base;
            Min = 0x7FFFFFFF;
            do {
                Dx = (pPl->Pos_x - Em38_root[k + 8].Pos_x) >> 1;
                Dz = (pPl->Pos_z - Em38_root[k + 8].Pos_z) >> 1;
                L = SquareRoot0(Dx * Dx + Dz * Dz) * 2;
                if (L < Min) {
                    Near = k + 8;
                    Min = L;
                }
                k++;
                n--;
            } while (n);
            EM38_W(pEm)->Root_no = Near;
        }
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 7;
        if (pEm->Timer0)
            pEm->Timer0--;
        pEm->Spd_x += 20;
        Joint_move(pEm, pKan, pSeq, 0x20);
        Add = 16;
        pR = &Em38_root[EM38_W(pEm)->Root_no];
        if (pEm->Spd_x > 30)
            Add = 24;
        if (pEm->Spd_x > 20)
            Add += 8;
        Goto00(pEm, pR->Pos_x, pR->Pos_z, Add);
        Add_speedXZ(pEm, 0);
        if (Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[EM38_W(pEm)->Root_no])) && !pEm->Timer0) {
            pEm->Routine_2 = 2;
            pEm->Timer0 = 30;
        }
        break;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x20);
        if (pEm->Timer0)
            pEm->Timer0--;
        Add = 16;
        if (pEm->Spd_x > 30)
            Add = 24;
        if (pEm->Spd_x > 20)
            Add += 16;
        if (pEm->Spd_x > 10)
            Add += 8;
        Goto01(pEm, Em38_root[EM38_W(pEm)->Root_no].Dir, Add);
        Add_speedXZ(pEm, 0);
        if (!Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[EM38_W(pEm)->Root_no])) && !pEm->Timer0) {
            pEm->Routine_2 = 1;
            pEm->Timer0 = 30;
            Base = !Flg_ck(&G.Unk7888, 0x1D) << 2;
            n = 4;
            k = Base;
            for (; n != 0; n--) {
                if (Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[k + 8]))) {
                    k |= 0x80;
                    break;
                }
                k++;
            }
            if (k & 0x80) {
                EM38_W(pEm)->Root_no = (k & 0x7F) + 8;
            } else {
                n = 4;
                k = Base;
                Min = 0x7FFFFFFF;
                do {
                    Dx = (pPl->Pos_x - Em38_root[k + 8].Pos_x) >> 1;
                    Dz = (pPl->Pos_z - Em38_root[k + 8].Pos_z) >> 1;
                    L = SquareRoot0(Dx * Dx + Dz * Dz) * 2;
                    if (L < Min) {
                        Near = k + 8;
                        Min = L;
                    }
                    k++;
                    n--;
                } while (n);
                EM38_W(pEm)->Root_no = Near;
            }
            break;
        }
        RotMatrix((SVECTOR *)&pEm->Cdir_x, &M);
        ApplyMatrixSV(&M, &Ofs, &Sv);
        V.vx = pEm->Pos_x + Sv.vx;
        V.vy = pEm->Pos_y;
        V.vz = pEm->Pos_z + Sv.vz;
        func_8004D720(&V.vx, 1, 0x200, 0);
        if (!Goto01_ck(pEm, Em38_root[EM38_W(pEm)->Root_no].Dir, 0x80) && (pEm->Col.Hit_flg & 0x20)
            && Hit_ck_point4(&pEm->Pos_x, EM38_ROOT_CUT(&Em38_root[EM38_W(pEm)->Root_no]))) {
            pEm->Routine_2 = 3;
            EM_SET_MOVE(pEm, 0xD, 0, 0x1F);
            pEm->Be_flg |= 0x2000;
            EM38_W(pEm)->Flg |= 0xC;
            Snd_se_enem(9, pEm);
        }
        break;
    case 3:
        if (pEm->Move_cnt < 16)
            Goto01(pEm, Em38_root[EM38_W(pEm)->Root_no].Dir, 0x10);
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM38_W(pEm)->Wall_timer = 90;
            pEm->Be_flg &= ~0x2000;
            EM38_W(pEm)->Flg &= ~0xC;
        }
        if (pEm->Move_cnt > 38 && pEm->Spd_x > 30)
            Goto00(pEm, pEm->Pos_x, pEm->Pos_z, 0x10);
        if (pEm->Move_cnt == 38)
            pEm->Be_flg &= ~0x2000;
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 37) {
            pEm->Col.W0 = 1800;
            pEm->Col.D0 = 1800;
            pEm->Unk134 = 1800;
            pEm->Pos_y = 0;
            pEm->Unk122 = 0;
            pEm->nFloor = 0;
            pEm->Be_flg &= ~0x20;
        }
        break;
    case 4:
        break;
    }
}

/* mv_br[12]: flag 0x1F while at the goal position */
void Em38_burrow_br(ENEMY_WORK *pEm)
{
    VECTOR *pV = &Em38_goal_pos;
    LONG Dx, Dz;

    pEm->Life = 10000;
    if (pEm->Routine_2 == 1) {
        Dx = (pV->vx - pEm->Pos_x) >> 1;
        Dz = (pV->vz - pEm->Pos_z) >> 1;
        if (SquareRoot0(Dx * Dx + Dz * Dz) * 2 < 901) {
            Flg_on(&G.Unk7888, 0x1F);
            pEm->Routine_2 = 2;
        }
    }
    if (pEm->Routine_2 == 4) {
        Dx = (pV->vx - pEm->Pos_x) >> 1;
        Dz = (pV->vz - pEm->Pos_z) >> 1;
        if (SquareRoot0(Dx * Dx + Dz * Dz) * 2 > 2499) {
            Flg_off(&G.Unk7888, 0x1F);
            pEm->Routine_2 = 0;
        }
    }
}

/* mv[12]: retreat to the goal position and burrow */
void Em38_burrow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pP, *pS, *pT;
    MATRIX *pM;
    PARTS_W *pW;
    SHORT Old_x, Old_z;
    SHORT Add;
    LONG X, Z;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x19, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        Add = 4;
        if (pEm->Spd_x > 30)
            Add = 8;
        if (pEm->Spd_x > 20)
            Add += 4;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_MOVE(pEm, 0x19, 0, 0x1F);
            if (Rnd_l() & 3)
                EM_SET_MOVE(pEm, 0xE, 0, 0x1F);
        }
        if (!(pEm->Move_cnt & 0x1F)) {
            pP = pEm->pSin_parts_ptr;
            pW = &pP[2];
            Sv.vx = pW->Pos_x + 200;
            Sv.vy = pW->Pos_y - 200;
            Sv.vz = pW->Pos_z;
            pM = &pP[1].Workm;
            Esp_call(0xB40, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
            Esp_call(0xD40, (USHORT)(pEm->Cdir_y - (Rnd_l() << 2) + 0x300 - (Rnd_l() << 1)) | 0x10000000, pM, &Sv, 0);
            Esp_call(0x1240, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0C000000, pM, &Sv, 0);
            Esp_call(0xE40, (USHORT)(pEm->Cdir_y + 0x300 - (Rnd_l() << 1)) | 0x0E000000, pM, &Sv, 0);
            if ((Rnd_l() & 3) == 1)
                Snd_se_enem(6, pEm);
        }
        break;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        Add = 2;
        if (pEm->Spd_x > 30)
            Add = 6;
        if (pEm->Spd_x > 20)
            Add += 4;
        Goto01(pEm, 0xAA0, Add);
        pEm->Spd_x >>= 1;
        pEm->Spd_z >>= 1;
        if (!Goto01_ck(pEm, 0xAA0, 8))
            pEm->Routine_2 = 3;
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    case 3:
        EM_SET_MOVE(pEm, 0x1D, 0, 0x1F);
        pEm->Routine_2 = 4;
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 0;
    case 4:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->UnkE2) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->UnkE2 = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if ((Rnd_l() & 3) == 1) {
                EM_SET_MOVE(pEm, 0x1D, 0x2D, 0x1F);
                pEm->UnkE2 = 1;
            }
        }
        if (*pEm->pNow_seq & 0xF000) {
            pEm->UnkE3 = 15;
            if (Rnd_l() & 1) {
                Sv.vx = 0x3A0 - Rnd_l();
                Sv.vy = 0;
                Sv.vz = 0x80 - Rnd_l();
                pS = pEm->pSin_parts_ptr;
                Esp_call(9, (USHORT)(Rnd_l() << 4) | 0x10000000, &pS[2].Workm, &Sv, 0);
                Esp_call(0x109, (USHORT)(Rnd_l() << 4) | 0x10000000, &pS[2].Workm, &Sv, 0);
            }
        }
        if (pEm->UnkE3 && (--pEm->UnkE3 & 1)) {
            X = 0x3A0;
            Z = 0x80;
            Sv.vx = X - Rnd_l();
            Sv.vy = 0;
            Sv.vz = Z - Rnd_l();
            pT = pEm->pSin_parts_ptr;
            Esp_call(0x1900, (((Rnd_l() << 2) + 0x1800) << 16) | (USHORT)pEm->Cdir_y, &pT[2].Workm, &Sv, 0);
            Sv.vx = X - Rnd_l();
            Sv.vz = Z - Rnd_l();
            pS = pEm->pSin_parts_ptr;
            Esp_call(0x1900, (((Rnd_l() << 2) + 0x1400) << 16) | (USHORT)pEm->Cdir_y, &pS[3].Workm, &Sv, 0);
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

/* mv_br[13]: on the wall: spit or lunge at the player, drop down after the spits */
void Em38_wall_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG L_pl;
    SHORT Ck0, Ck1;

    if (pEm->Routine_2 == 0) {
        pEm->Be_flg &= ~0x2000;
        pEm->Col.W0 = 2000;
        pEm->Col.D0 = 2000;
        pEm->Unk134 = 0;
        pEm->Be_flg |= 0x20;
    }
    L_pl = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Ck0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Ck1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80);
    if (L_pl < 9000 && !Ck0 && !EM38_W(pEm)->Spit_timer && (pEm->Col.Hit_flg & 0x20))
        EM_SET_RTN(pEm, 1, 7, 0, 0);
    if (L_pl < 15000 && !Ck1 && !EM38_W(pEm)->Spit_timer && (pEm->Col.Hit_flg & 0x20))
        EM_SET_RTN(pEm, 1, 7, 0, 0);
    if (L_pl < 6000 && !Ck0 && !EM38_W(pEm)->Atk_timer && (pEm->Col.Hit_flg & 0x20))
        EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    if (!EM38_W(pEm)->Wall_timer && !EM38_W(pEm)->Wall_spit)
        EM_SET_RTN(pEm, 1, 0xB, 0, 0);
}

/* mv[13]: crawl along the wall towards the player for 150 frames */
void Em38_wall(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x7F);
        EM38_W(pEm)->Flg |= 2;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 150;
        EM38_W(pEm)->Flg |= 0xC;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x += 40;
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 7;
        Joint_move(pEm, pKan, pSeq, 0x20);
        Add = 8;
        if (pEm->Spd_x > 30)
            Add = 0x10;
        if (pEm->Spd_x > 20)
            Add += 8;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, Add);
        Add_speedXZ(pEm, 0);
        pEm->Timer0--;
        break;
    }
}

/* no brain step for this move */
void Em38_lunge_br(ENEMY_WORK *pEm)
{
}

/* mv[14]: lunge */
void Em38_lunge(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    SVECTOR Sv;
    HIT_AREA Area;
    SHORT Old_x, Old_z;
    LONG Hit;
    LONG Dir_old;
    LONG Dir;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xF, 0, 0x1F);
        pEm->Routine_2 = 1;
        Snd_se_enem(9, pEm);
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Atk_timer = (Rnd_l() & 0xF) + 20;
            EM_SET_RTN(pEm, 1, 0xD, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            if (EM38_W(pEm)->Wall_spit)
                EM38_W(pEm)->Wall_spit--;
        }
        if (pEm->Move_cnt < 32)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x10);
        if ((*pEm->pNow_seq & 0x200) && !pPl->Damage_cnt) {
            pP = pEm->pSin_parts_ptr;
            pEm->UnkE3 = -0x80;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 1400;
            Hit = func_80044420(&pP[6].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[7].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[8].Workm, &Sv, pPl);
            if (Hit) {
                Dir_old = pEm->Cdir_y;
                pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z) - 0x400;
                Dir = pEm->Cdir_y;
                Hit = Cdir_ck_l(pEm, pPl);
                pEm->Cdir_y = Dir_old;
                pPl->pEnemy_ptr = pEm;
                EM_RTN(pPl) = ((Hit + 4) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                Dir -= 0x800;
                if (Hit)
                    Dir += 0x800;
                Goto01(pPl, Dir, 0x200);
                Snd_se_stad(0x1020D, (VECTOR *)&pEm->Pos_x, 0, 0);
                Pl_life_down(30, 1);
                Pad_vib_set0(3, 0);
                Pad_vib_set1(10, 250, 0);
            }
        }
        break;
    }
    Add_speedXZ(pEm, 0);
    if (pEm->Move_cnt >= 23 && pEm->Move_cnt < 29) {
        Area.pPos = (VECTOR *)&pEm->Pos_x;
        Area.pBox = Em38_box8;
        Area.Dir = pEm->Cdir_y;
        Area.Id = 0x40;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
        Area.pBox = Em38_box9;
        Area.Id = 0x45;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Area, &Pl_work);
    }
}

/* Routine_0 2: damage routine by the hit kind (Unk12C) */
void Em38_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    EM38_SABUN *pSa;
    SVECTOR Unused;                    /* frame size */

    if (pEm->Routine_2 == 0) {
        if (!pEm->Pos_y) {
            pP = &pEm->pSin_parts_ptr[20];
            pSa = (EM38_SABUN *)pP->pUnk98;
            if (pSa->Key[0].Rate) {
                pSa->Key[0].Rate = 0;
                func_8004A934(pEm, pP);
            }
            if ((pEm->Type & 1) && pEm->Unk12C == 24)
                pEm->Unk12C = 3;
            if (pEm->Unk132 & 0x500)
                Em38_acid_spark(pEm);
            if (pEm->Unk132 & 0x200)
                Em38_blast_light(pEm);
            if (pEm->Unk132 & 0x800)
                Em38_fire_smoke(pEm);
            if (pEm->Unk12C == 5)
                EM38_W(pEm)->Breath = 1;
            if (!(EM38_W(pEm)->Flg & 8) && !(pEm->Type & 1) && Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x72C))
                pEm->Unk12C = 32;
        }
        if (pEm->Routine_2 == 0 && pEm->Unk12C == 24 && (pEm->nFloor || (pEm->Be_flg & 0x2000)))
            pEm->Unk12C = 3;
    }
    Em38_dm[pEm->Unk12C - 1](pEm, pKan, pSeq);
}

/* dm00: hit, keep moving */
void Em38_dm_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    EM38_W(pEm)->Shake_no = 1;
    if (pEm->Type & 1) {
        EM_RTN(pEm) = pEm->Unk1C4;
        Em38_blood_head(pEm);
        if (pEm->Routine_0 == 2)
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
        Em38_Move(pEm, pKan, pSeq);
        if (!pEm->Pos_y) {
            pP = &pEm->pSin_parts_ptr[27 + (Rnd_l() & 3)];
            if (!pP->UnkB4) {
                pP->UnkB4 = 1;
                Snd_se_enem(0, pEm);
            }
        }
    } else {
        EM_RTN(pEm) = pEm->Unk1C4;
        Em38_blood_head(pEm);
        if (pEm->Routine_0 == 2)
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        Em38_Move(pEm, pKan, pSeq);
        if (!pEm->Pos_y) {
            pP = &pEm->pSin_parts_ptr[27 + (Rnd_l() & 3)];
            if (!pP->UnkB4) {
                pP->UnkB4 = 1;
                Snd_se_enem(0, pEm);
            }
            if ((Rnd_l() & 3) == 1)
                Snd_se_enem(8, pEm);
        } else {
            Snd_se_enem(8, pEm);
        }
    }
}

/* dm32 */
void Em38_dm_big(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs;
    PARTS_W *pP;
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xA, 3, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        EM38_W(pEm)->Flg &= 0xD;
        pP = EM38_W(pEm)->pTentacle0->pSin_parts_ptr;
        Ofs.vx = 0;
        Ofs.vy = -800;
        Ofs.vz = 0;
        Esp_call(0x200, 0x28000000, &pP->Workm, &Ofs, 0);
        Esp_call(0x2000200, 0x20000800, &pP->Workm, &Ofs, 0);
        EM38_W(pEm)->Flg &= ~2;
        Snd_se_enem(8, pEm);
        if (pEm->Life > 50)
            pEm->Life -= 40;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (pEm->Move_cnt == 20) {
            pP = EM38_W(pEm)->pTentacle0->pSin_parts_ptr;
            Ofs.vx = 0;
            Ofs.vy = -800;
            Ofs.vz = 0;
            Esp_call(0x200, 0x28000000, &pP->Workm, &Ofs, 0);
            Esp_call(0x2000200, 0x20000800, &pP->Workm, &Ofs, 0);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Atk_timer = 30;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Damage_cnt &= 0x7F;
        }
        break;
    }
    Add = 2;
    if (pEm->Spd_x > 30)
        Add = 6;
    if (pEm->Spd_x > 20)
        Add += 4;
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

/* dm24: hit on the head */
void Em38_dm_head(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs;
    PARTS_W *pP;
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xB, 3, 0x1F);
        pEm->Routine_2 = 1;
        EM38_W(pEm)->Flg &= 0xD;
        pP = EM38_W(pEm)->pTentacle0->pSin_parts_ptr;
        Ofs.vx = 0;
        Ofs.vy = -800;
        Ofs.vz = 0;
        Esp_call(0x200, 0x28000000, &pP->Workm, &Ofs, 0);
        Esp_call(0x2000200, 0x20000800, &pP->Workm, &Ofs, 0);
        EM38_W(pEm)->Flg &= ~2;
        Snd_se_enem(8, pEm);
        pP = pEm->pSin_parts_ptr;
        Esp_call(9, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Esp_call(0x109, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Esp_call(9, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Esp_call(0x209, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Esp_call(9, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Esp_call(0x109, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Esp_call(9, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Esp_call(0x209, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (pEm->Move_cnt == 20) {
            pP = EM38_W(pEm)->pTentacle0->pSin_parts_ptr;
            Ofs.vx = 0;
            Ofs.vy = -800;
            Ofs.vz = 0;
            Esp_call(0x200, 0x28000000, &pP->Workm, &Ofs, 0);
            Esp_call(0x2000200, 0x20000800, &pP->Workm, &Ofs, 0);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Atk_timer = 30;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Damage_cnt &= 0x7F;
            if (pEm->Type & 1)
                EM_SET_RTN(pEm, 1, 0xC, 0, 0);
        }
        break;
    }
    Add = 2;
    if (pEm->Spd_x > 30)
        Add = 6;
    if (pEm->Spd_x > 20)
        Add += 4;
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

/* Routine_0 3: lose the effects; keep moving while on a floor or in a scenario, else retreat */
void Em38_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    EM38_SABUN *pSa;

    if (pEm->Routine_2 == 0 && !pEm->Pos_y) {
        pP = &pEm->pSin_parts_ptr[20];
        pSa = (EM38_SABUN *)pP->pUnk98;
        if (pSa->Key[0].Rate) {
            pSa->Key[0].Rate = 0;
            func_8004A934(pEm, pP);
        }
        if (pEm->Unk132 & 0x500)
            Em38_acid_spark(pEm);
        if (pEm->Unk132 & 0x200)
            Em38_blast_light(pEm);
        if (pEm->Unk132 & 0x800)
            Em38_fire_smoke(pEm);
        if (pEm->Unk12C == 5)
            EM38_W(pEm)->Breath = 1;
    }
    if ((pEm->nFloor || (pEm->Be_flg & 0x2000)) && pEm->Routine_2 == 0) {
        pEm->Life = 0;
        pEm->Damage_cnt &= 0x7F;
        EM_RTN(pEm) = pEm->Unk1C4;
        Em38_blood_head(pEm);
        Em38_Move(pEm, pKan, pSeq);
        if (!pEm->Pos_y) {
            pP = &pEm->pSin_parts_ptr[27 + (Rnd_l() & 3)];
            if (!pP->UnkB4) {
                pP->UnkB4 = 1;
                Snd_se_enem(0, pEm);
            }
        }
    } else {
        Em38_die_retreat(pEm, pKan, pSeq);
    }
}

/* dying: stagger away, then retreat and burrow (mv 12) */
void Em38_die_retreat(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;                    /* frame size */
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xA, 3, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        EM38_W(pEm)->Flg |= 0x40;
        Snd_se_enem(9, pEm);
        pEm->Type |= 1;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->Hokan_flg) >> 5;
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
            pEm->Life = 10000;
            pEm->Damage_cnt &= 0x7F;
            pEm->Type |= 1;
        }
        break;
    }
    Add = 2;
    if (pEm->Spd_x > 30)
        Add = 6;
    if (pEm->Spd_x > 20)
        Add += 4;
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

/* Routine_0 4: scenario routine by Routine_1 */
void Em38_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em38_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

/* sce00: rising out of the ground */
void Em38_sce_rise(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pP, *pS, *pW;
    LONG X, Z;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE3 = 0;
        pEm->Move_cnt++;
        pEm->Damage_cnt |= 0x80;
        if (!(pEm->Unk14A & 1))
            break;
        pEm->Move_cnt = 0;
        pEm->Routine_2 = 1;
        Snd_se_enem(10, pEm);
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_MOVE(pEm, 0x10, 0, 0x1F);
            pEm->Routine_2 = 2;
            pEm->UnkE2 = 2;
            pEm->Be_flg &= ~0x20;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if (--pEm->UnkE2 == 0) {
                pEm->Routine_2 = 3;
                EM_SET_MOVE(pEm, 0x14, 0, 0x1F);
            }
        }
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0xF000) {
            pEm->UnkE3 = 5;
            Sv.vx = 0x3A0 - Rnd_l();
            Sv.vy = 0;
            Sv.vz = 0x80 - Rnd_l();
            pP = pEm->pSin_parts_ptr;
            Esp_call(9, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP[2].Workm, &Sv, 0);
            Esp_call(0x109, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP[2].Workm, &Sv, 0);
        }
        break;
    case 3:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            EM_SET_RTN(pEm, 4, 1, 0, 0);
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0xF000) {
            pEm->UnkE3 = 15;
            Sv.vx = 0x3A0 - Rnd_l();
            Sv.vy = 0;
            Sv.vz = 0x80 - Rnd_l();
            pP = pEm->pSin_parts_ptr;
            Esp_call(9, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP[2].Workm, &Sv, 0);
            Esp_call(0x109, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP[2].Workm, &Sv, 0);
        }
        break;
    }
    if (pEm->UnkE3 && (--pEm->UnkE3 & 1)) {
        X = 0x3A0;
        Z = 0x80;
        Sv.vx = X - Rnd_l();
        Sv.vy = 0;
        Sv.vz = Z - Rnd_l();
        pS = pEm->pSin_parts_ptr;
        Esp_call(0x1900, (((Rnd_l() << 2) + 0x1800) << 16) | (USHORT)pEm->Cdir_y, &pS[2].Workm, &Sv, 0);
        Sv.vx = X - Rnd_l();
        Sv.vz = Z - Rnd_l();
        pW = pEm->pSin_parts_ptr;
        Esp_call(0x1900, (((Rnd_l() << 2) + 0x1400) << 16) | (USHORT)pEm->Cdir_y, &pW[3].Workm, &Sv, 0);
        Sv.vx = X - Rnd_l();
        Sv.vz = Z - Rnd_l();
        pP = pEm->pSin_parts_ptr;
        Esp_call(0, (((Rnd_l() << 2) + 0x1400) << 16) | (USHORT)pEm->Cdir_y, &pP[2].Workm, &Sv, 0);
    }
}

/* sce: transformation (the body swells, grows the new parts and sheds the tentacles) */
void Em38_sce_swell(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pB;
    PARTS_W *pP;
    LONG Size;
    LONG i, n;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x12, 0, 0x1F);
        pEm->Routine_2 = 1;
        Em38_pulse_no = 0;
    case 1:
        if ((pEm->Move_cnt > 5 && pEm->Move_cnt < 75) || (pEm->Move_cnt > 100 && pEm->Move_cnt < 170)) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            Size = pB->Size_x;
            if (Size < 0x400 || Size > 0xC00) {
                if (pEm->Move_cnt & 1)
                    Size += 0x2A;
                else
                    Size -= 0x28;
            } else {
                if (pEm->Move_cnt & 1)
                    Size += 0x34;
                else
                    Size -= 0x32;
            }
            if (pEm->Move_cnt > 20 && pEm->Move_cnt < 45)
                Size += 8;
            if (pEm->Move_cnt > 130 && pEm->Move_cnt < 160)
                Size += 0x18;
            Size += Em38_pulse[Em38_pulse_no];
            Em38_pulse_no = (Em38_pulse_no + 1) & 0x1F;
            if (Size > 0x1000)
                Size = 0x1000;
            n = Em38_swell.Num0;
            i = 0;
            do {
                Em38_part_scale_pos(pEm, &pB[Em38_swell.Parts[i]], Size);
                i++;
            } while (--n);
            n = Em38_swell.Num1;
            i = 0;
            do {
                PARTS_W *pQ = &pB[Em38_swell.Parts[i]];

                pQ->Size_x = Size;
                pQ->Size_y = Size;
                pQ->Size_z = Size;
                i++;
            } while (--n);
            pEm->Unk12A = Size;
        }
        if (pEm->Move_cnt > 130 && pEm->Move_cnt < 164) {
            pP = pEm->pSin_parts_ptr;
            Size = EM38_SA(pP)->Key[0].Rate;
            Size += ((pEm->Move_cnt & 1) << 7) - 0xC0;
            if (Size < 0)
                Size = 0;
            if (pEm->Move_cnt == 163)
                Size = 0;
            EM38_SA(pP)->Key[0].Rate = Size;
            func_8004AC54(pEm, pP);
            pP = &pEm->pSin_parts_ptr[12];
            EM38_SA(pP)->Key[0].Rate = Size;
            func_8004AC54(pEm, pP);
            pP = &pEm->pSin_parts_ptr[2];
            EM38_SA(pP)->Key[0].Rate = Size;
            func_8004AC54(pEm, pP);
            pP = &pEm->pSin_parts_ptr[20];
            EM38_SA(pP)->Key[1].Rate = Size;
            func_8004AC54(pEm, pP);
        }
        if (pEm->Move_cnt > 70 && pEm->Move_cnt < 90) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            pP = &pB[2];
            Size = pP->Size_x;
            Size += 0x5C;
            if (Size > 0x1000)
                Size = 0x1000;
            pP[0].Size_x = Size;
            pP[0].Size_y = Size;
            pP[0].Size_z = Size;
            pP[1].Size_x = Size;
            pP[1].Size_y = Size;
            pP[1].Size_z = Size;
            Em38_part_scale_pos(pEm, &pB[3], Size);
        }
        if (pEm->Move_cnt > 90 && pEm->Move_cnt < 110) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            pP = &pB[4];
            Size = pP->Size_x;
            Size += 0x3C;
            if (Size > 0x1000)
                Size = 0x1000;
            pP[0].Size_x = Size;
            pP[0].Size_z = Size;
            pP[1].Size_x = Size;
            pP[1].Size_z = Size;
            pP[2].Size_x = Size;
            pP[2].Size_z = Size;
            pP[3].Size_x = Size;
            pP[3].Size_z = Size;
            pP[4].Size_x = Size;
            pP[4].Size_z = Size;
            Em38_part_scale_pos(pEm, &pB[8], Size);
            if (pEm->Move_cnt == 109 || pEm->Move_cnt == 99 || pEm->Move_cnt == 95) {
                Esp_call(0x1900, (((Rnd_l() << 1) + 0x2000) << 16) | (USHORT)(Rnd_l() << 4), &pP[2].Workm, 0, 0);
                Esp_call(0x1900, (((Rnd_l() << 1) + 0x2000) << 16) | (USHORT)(Rnd_l() << 4), &pP[3].Workm, 0, 0);
                Esp_call(0x1900, (((Rnd_l() << 1) + 0x2000) << 16) | (USHORT)(Rnd_l() << 4), &pP[4].Workm, 0, 0);
            }
        }
        if (pEm->Move_cnt > 80 && pEm->Move_cnt < 110) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            pP = &pB[4];
            Size = pP->Size_y;
            Size += 0x80;
            if (Size > 0x1000)
                Size = 0x1000;
            pP[0].Size_y = Size;
            pP[1].Size_y = Size;
            Em38_part_scale_pos(pEm, &pB[5], Size);
            pP[2].Size_y = Size;
            Em38_part_scale_pos(pEm, &pB[6], Size);
            pP[3].Size_y = Size;
            Em38_part_scale_pos(pEm, &pB[7], Size);
            pP[4].Size_y = Size;
            Em38_part_scale_pos(pEm, &pB[8], Size);
            if (pEm->Move_cnt == 81 || pEm->Move_cnt == 88) {
                Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &pB[6].Workm, 0, 0);
                Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &pB[7].Workm, 0, 0);
                Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &pB[8].Workm, 0, 0);
            }
        }
        if (pEm->Move_cnt == 29)
            pEm->pSin_parts_ptr[16].Poly_rgb = 0x404020;
        if (pEm->Move_cnt > 30 && pEm->Move_cnt < 160 && !(pEm->Move_cnt & 3)) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            Sv.vx = 0x100 - (Rnd_l() << 1);
            Sv.vy = 0x100 - (Rnd_l() << 1);
            Sv.vz = 0x100 - (Rnd_l() << 1);
            {
                MATRIX *pM = &pB[16].Workm;

                Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), pM, &Sv, 0);
                Esp_call(0x1800, 0x10000000, pM, &Sv, 0);
                Sv.vx = 0x100 - (Rnd_l() << 1);
                Sv.vy = 0x100 - (Rnd_l() << 1);
                Sv.vz = 0x100 - (Rnd_l() << 1);
                Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), pM, &Sv, 0);
                Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1800, 0x10000000, pM, &Sv, 0);
            }
            pP = &pEm->pSin_parts_ptr[17 + Rnd_l() % 3];
            Sv.vx = 0x100 - (Rnd_l() << 1);
            Sv.vy = 0x100 - (Rnd_l() << 1);
            Sv.vz = 0x100 - (Rnd_l() << 1);
            Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1900, (((Rnd_l() << 1) + 0x2000) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
            pP = &pEm->pSin_parts_ptr[17 + Rnd_l() % 3];
            Sv.vx = 0x100 - (Rnd_l() << 1);
            Sv.vy = 0x100 - (Rnd_l() << 1);
            Sv.vz = 0x100 - (Rnd_l() << 1);
            Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1900, (((Rnd_l() << 1) + 0x2000) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, 0);
        }
        if (pEm->Move_cnt > 30 && pEm->Move_cnt < 100) {
            pP = &pEm->pSin_parts_ptr[16];
            Size = pP->Size_x;
            Size += 0x40;
            if (Size > 0x1000)
                Size = 0x1000;
            pP[0].Size_x = Size;
            pP[0].Size_z = Size;
            pP[1].Size_x = Size;
            pP[1].Size_z = Size;
            pP[2].Size_x = Size;
            pP[2].Size_z = Size;
            pP[3].Size_x = Size;
            pP[3].Size_z = Size;
        }
        if (pEm->Move_cnt > 90 && pEm->Move_cnt < 180) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            pP = &pB[16];
            Size = pP->Size_y;
            Size += 0x80;
            if (Size > 0x1000)
                Size = 0x1000;
            pP[0].Size_y = Size;
            pP[1].Size_y = Size;
            Em38_part_scale_pos(pEm, &pB[17], Size);
        }
        if (pEm->Move_cnt > 70 && pEm->Move_cnt < 180) {
            pP = &pEm->pSin_parts_ptr[18];
            Size = pP->Size_y;
            Size += 0x80;
            if (Size > 0x1000)
                Size = 0x1000;
            pP->Size_y = Size;
            Em38_part_scale_pos(pEm, pP, Size);
        }
        if (pEm->Move_cnt > 50 && pEm->Move_cnt < 180) {
            pP = &pEm->pSin_parts_ptr[19];
            Size = pP->Size_y;
            Size += 0x40;
            if (Size > 0x1000)
                Size = 0x1000;
            pP->Size_y = Size;
            Em38_part_scale_pos(pEm, pP, Size);
        }
        if (pEm->Move_cnt > 120 && pEm->Move_cnt < 190) {
            pP = &pEm->pSin_parts_ptr[16];
            if ((pP->Poly_rgb & 0xFF0000) != 0x800000)
                pP->Poly_rgb += 0x10100;
        }
        if (pEm->Move_cnt > 90 && pEm->Move_cnt < 190) {
            pP = &pEm->pSin_parts_ptr[16];
            if ((pP->Poly_rgb & 0xFF) != 0x80)
                pP->Poly_rgb += 1;
        }
        if (pEm->Move_cnt > 150 && pEm->Move_cnt < 180) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            pP = &pB[20];
            Size = pP->Size_x;
            Size += 0x40;
            if (Size > 0x1000)
                Size = 0x1000;
            pP[0].Size_x = Size;
            pP[0].Size_y = Size;
            pP[0].Size_z = Size;
            pP[1].Size_x = Size;
            pP[1].Size_y = Size;
            pP[1].Size_z = Size;
            if (!(pEm->Move_cnt & 3)) {
                Sv.vx = 0x100 - (Rnd_l() << 1);
                Sv.vy = 0x100 - (Rnd_l() << 1);
                Sv.vz = 0x100 - (Rnd_l() << 1);
                {
                    MATRIX *pM = &pB[20].Workm;

                    Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), pM, &Sv, 0);
                    Sv.vx = 0x100 - (Rnd_l() << 1);
                    Sv.vy = 0x100 - (Rnd_l() << 1);
                    Sv.vz = 0x100 - (Rnd_l() << 1);
                    Esp_call(0x1600, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), pM, &Sv, 0);
                }
            }
        }
        if (pEm->Move_cnt > 90 && pEm->Move_cnt < 180) {
            PARTS_W *pB = pEm->pSin_parts_ptr;

            pP = &pB[22];
            if (pEm->Move_cnt == 91)
                pP->Poly_rgb = 0x505030;
            if ((pP->Poly_rgb & 0xFF0000) != 0x800000)
                pP->Poly_rgb += 0x10100;
            if ((pP->Poly_rgb & 0xFF) != 0x80)
                pP->Poly_rgb += 1;
            pB[22].Be_flg |= 1;
            pP->Size_y += 0x40;
            if (pP->Size_y > 0x1000)
                pP->Size_y = 0x1000;
            if (!(pEm->Move_cnt & 3)) {
                Sv.vx = 0x100 - (Rnd_l() << 1);
                Sv.vy = 0x100 - (Rnd_l() << 1);
                Sv.vz = 0x100 - (Rnd_l() << 1);
                {
                    MATRIX *pM = &pB[22].Workm;

                    Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), pM, &Sv, 0);
                    Sv.vx = 0x100 - (Rnd_l() << 1);
                    Sv.vy = 0x100 - (Rnd_l() << 1);
                    Sv.vz = 0x100 - (Rnd_l() << 1);
                    Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), pM, &Sv, 0);
                }
            }
        }
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            pEm->Routine_2 = 2;
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        EM_SET_MOVE(pEm, 0x17, 0, 3);
        pEm->Routine_2 = 3;
    case 3:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            EM38_W(pEm)->pTentacle0->Be_flg &= ~0x10;
            EM38_W(pEm)->pTentacle0->Routine_1 = 3;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 4;
            pEm->pSin_parts_ptr[22].Be_flg &= ~4;
            EM38_W(pEm)->pTentacle1->Be_flg &= ~0x10;
            EM38_W(pEm)->pTentacle1->Routine_1 = 3;
            pP = pEm->pSin_parts_ptr;
            for (n = 22; n != 0; n--) {
                pP->Be_flg &= ~4;
                pP->Size_x = 0x1000;
                pP->Size_y = 0x1000;
                pP->Size_z = 0x1000;
                if (n != 22)
                    Em38_part_scale_pos(pEm, pP, 0x1000);
                pP++;
            }
            pP = &pEm->pSin_parts_ptr[6];
            pP[0].Be_flg |= 4;
            pP[1].Be_flg |= 4;
            pP[2].Be_flg |= 4;
            func_800251FC_(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, &pEm->Pos_x);
            EM38_W(pEm)->Flg &= ~0x10;
            pB = pEm->pSin_parts_ptr;
            Em38_chain_slope_end(pEm, &pB[12], &pB[16]);
            pB = pEm->pSin_parts_ptr;
            Em38_chain_slope_end(pEm, &pB[16], &pB[17]);
            pB = pEm->pSin_parts_ptr;
            Em38_chain_slope_end(pEm, &pB[17], &pB[18]);
            pB = pEm->pSin_parts_ptr;
            Em38_chain_slope_end(pEm, &pB[18], &pB[19]);
            pEm->Unk12A = 0x1000;
        }
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 30) {
            pEm->pSin_parts_ptr[23].Be_flg |= 1;
            pEm->pSin_parts_ptr[24].Be_flg |= 1;
            pEm->pSin_parts_ptr[25].Be_flg |= 1;
            pEm->pSin_parts_ptr[26].Be_flg |= 1;
            pP = &pEm->pSin_parts_ptr[27];
            pP->UnkB4 = 3;
            pP->Be_flg |= 1;
            pP = &pEm->pSin_parts_ptr[28];
            pP->UnkB4 = 3;
            pP->Be_flg |= 1;
            pP = &pEm->pSin_parts_ptr[29];
            pP->UnkB4 = 3;
            pP->Be_flg |= 1;
            pP = &pEm->pSin_parts_ptr[30];
            pP->UnkB4 = 3;
            pP->Be_flg |= 1;
        }
        break;
    case 4:
        EM_SET_MOVE(pEm, 0, 0, 0x40);
        pEm->Routine_2 = 5;
        pEm->Timer0 = 150;
        EM38_W(pEm)->Spit_timer = 120;
    case 5:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x3F);
        if (--pEm->Timer0 == -1) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt &= 0x7F;
        }
        if (pEm->Timer0 == 100)
            Snd_se_enem(9, pEm);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* sce: death throes (the tentacles fall, the body swells and bursts) */
void Em38_sce_burst(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x13, 0, 3);
        pEm->Routine_2 = 1;
        Snd_se_enem(8, pEm);
        pEm->Life = -1;
        EM_SET_RTN(EM38_W(pEm)->pTentacle0, 3, 0, 0, 0);
        EM_SET_RTN(EM38_W(pEm)->pTentacle1, 3, 0, 0, 0);
        EM38_W(pEm)->pTentacle0->Be_flg |= 0x10;
        EM38_W(pEm)->pTentacle1->Be_flg |= 0x10;
        pEm->Type |= 3;
        pEm->pSin_parts_ptr[16].Be_flg &= ~2;
        pEm->pSin_parts_ptr[17].Be_flg &= ~2;
        pEm->pSin_parts_ptr[18].Be_flg &= ~2;
        pEm->pSin_parts_ptr[19].Be_flg &= ~2;
        pS = &pEm->pSin_parts_ptr[27];
        for (i = 4; i != 0; i--) {
            if (!pS->UnkB4) {
                pS->UnkB4 = 1;
                Snd_se_enem(0, pEm);
            }
            pS++;
        }
        pP = &pEm->pSin_parts_ptr[4];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FLY(pP)->Dir_y = 0;
        PW_FLY(pP)->Cnt = 0;
        PW_FLY(pP)->Spd_xz = 0;
        PW_FLY(pP)->Spd_y = 0;
        PW_FLY(pP)->Gravity = 3;
        PW_FLY(pP)->Spd_xz_add = 0;
        pP->Poly_rgb = 0x101030;
        pP = &pEm->pSin_parts_ptr[22];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FLY(pP)->Dir_y = 0;
        PW_FLY(pP)->Cnt = 0;
        PW_FLY(pP)->Spd_xz = 0;
        PW_FLY(pP)->Spd_y = 0;
        PW_FLY(pP)->Gravity = 3;
        PW_FLY(pP)->Spd_xz_add = 0;
        pP->Poly_rgb = 0x101030;
        Em38_acid_spark(pEm);
        pP = &pEm->pSin_parts_ptr[20];
        EM38_SA(pP)->Key[0].Rate = 0;
        EM38_SA(pP)->Key[1].Rate = 0x1000;
        func_8004AC54(pEm, pP);
        for (i = 8; i != 0; i--)
            Em38_kage[i - 1]->Be_flg = 0;
        pEm->Col.W0 = 900;
        pEm->Col.D0 = 900;
        pEm->Col.H = 1000;
        Em38_col[1].Id = 0;
        Em38_col[3].Id = 0;
        Em38_col[4].Id = 0;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            EM_SET_MOVE(pEm, 0x15, 0, 3);
            pEm->Routine_2 = 2;
            Snd_se_enem(9, pEm);
            Esp_call(0x205, 0x20000000, &pEm->pSin_parts_ptr[20].Workm, 0, 0);
            {
                MATRIX *pM = &pEm->pSin_parts_ptr[21].Workm;

                Esp_call(0x205, 0x18000000, pM, 0, 0);
                Esp_call(0x1001B03, 0x10000000, pM, 0, 0);
            }
            {
                MATRIX *pM = &pEm->pSin_parts_ptr[2].Workm;

                Sv.vx = 400;
                Sv.vy = -400;
                Sv.vz = 0;
                Esp_call(0x205, 0x20000000, pM, &Sv, 0);
                Esp_call(0x1001B03, 0x0A000000, pM, &Sv, 0);
            }
            pW = &pEm->pSin_parts_ptr[16];
            Sv.vx = 0;
            Sv.vy = 400;
            Sv.vz = 0;
            Esp_call(0x205, 0x20000000, &pW->Workm, &Sv, 0);
            pP = pEm->pSin_parts_ptr;
            {
                MATRIX *pM = &pP->Workm;

                Sv.vx = -500;
                Sv.vy = -400;
                Sv.vz = 0;
                Esp_call(0x205, 0x28000000, pM, &Sv, 0);
                Esp_call(0x1001B03, 0x10000000, pM, 0, 0);
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 3;
            pEm->Timer0 = 150;
            pEm->UnkDA = 0;
            pP = pEm->pSin_parts_ptr;
            for (i = 22; i != 0; i--) {
                pP->Be_flg |= 4;
                pP->Size_x = 0x1000;
                pP->Size_y = 0x1000;
                pP->Size_z = 0x1000;
                pP++;
            }
            pEm->UnkE2 = 5;
            EM_SET_MOVE(pEm, 0x16, 0, 0xF);
            pP = &pEm->pSin_parts_ptr[23];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[24];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[25];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[26];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[27];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[28];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[29];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[30];
            pP->UnkA2 &= ~1;
        }
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 5) {
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_FLY(pP)->Dir_y = 0;
            PW_FLY(pP)->Cnt = 0;
            PW_FLY(pP)->Spd_xz = 0;
            PW_FLY(pP)->Spd_y = 0;
            PW_FLY(pP)->Gravity = 3;
            PW_FLY(pP)->Spd_xz_add = 0;
            pP->Poly_rgb = 0x101030;
            pP = &pEm->pSin_parts_ptr[13];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_FLY(pP)->Dir_y = 0;
            PW_FLY(pP)->Cnt = 0;
            PW_FLY(pP)->Spd_xz = 0;
            PW_FLY(pP)->Spd_y = 0;
            PW_FLY(pP)->Gravity = 3;
            PW_FLY(pP)->Spd_xz_add = 0;
            pP->Poly_rgb = 0x101030;
        }
        if (pEm->Move_cnt == 30) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = -800;
            Sv.vz = 0;
            Esp_call(0x1403, 0x18000000, &pP->Workm, &Sv, 0);
            pW = &pEm->pSin_parts_ptr[12];
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(0x1403, 0x18000000, &pW->Workm, &Sv, 0);
            pW = &pEm->pSin_parts_ptr[16];
            Sv.vx = 0x80;
            Sv.vy = 200;
            Sv.vz = 0x40;
            Esp_call(0x1403, 0x14000000, &pW->Workm, &Sv, 0);
            Esp_call(0x1403, 0x10000000, &pEm->pSin_parts_ptr[3].Workm, &Sv, 0);
            pW = &pEm->pSin_parts_ptr[17];
            Sv.vx = 0x22;
            Sv.vy = 0x41;
            Sv.vz = 0x100;
            Esp_call(0x1403, 0x10000000, &pW->Workm, &Sv, 0);
        }
        if (pEm->Move_cnt == 18) {
            Pad_vib_set0(4, 0);
            Pad_vib_set1(6, 250, 0);
        }
        if (pEm->Move_cnt == 33) {
            Pad_vib_set0(3, 0);
            Pad_vib_set1(5, 200, 0);
        }
        if (pEm->Move_cnt == 42) {
            Pad_vib_set0(3, 0);
            Pad_vib_fade_set(45, 200, 0, 0);
        }
        break;
    case 3:
        if (!(pEm->UnkE2 & 0x80) && Joint_move(pEm, pKan, pSeq, 0x100))
            pEm->UnkE2 |= 0x80;
        if (!(--pEm->UnkE2 & 0x7F)) {
            pEm->UnkE2 = (Rnd_l() & 0xF) + 3;
            EM_SET_MOVE(pEm, 0x16, 0, 0xF);
        }
        if (--pEm->Timer0 == -1)
            pEm->Routine_2 = 4;
        if (!(pEm->Timer0 % 45)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0x200 - (Rnd_l() << 2);
            Sv.vy = 0x200 - (Rnd_l() << 2);
            Sv.vz = 0x200 - (Rnd_l() << 2);
            Esp_call(0x1C03, 0x18000000, &pP->Workm, &Sv, 0);
            Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[12].Workm, &Sv, 0);
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0x100 - (Rnd_l() << 1);
            Sv.vy = 0x100 - (Rnd_l() << 1);
            Sv.vz = 0x100 - (Rnd_l() << 1);
            Esp_call(0x1C03, 0x14000000, &pP[16].Workm, &Sv, 0);
            Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[3].Workm, &Sv, 0);
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0x100 - (Rnd_l() << 1);
            Sv.vy = 0x100 - (Rnd_l() << 1);
            Sv.vz = 0x100 - (Rnd_l() << 1);
            Esp_call(0x1C03, 0x10000000, &pP[17].Workm, &Sv, 0);
            Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[18].Workm, &Sv, 0);
            Snd_se_stad(0x10214, (VECTOR *)&pEm->Pos_x, 0, 0);
        }
        pP = pEm->pSin_parts_ptr;
        if ((pP->Poly_rgb & 0xFFFFFF) != 0x505050) {
            pS = pP;
            for (i = 22; i != 0; i--) {
                pS->Poly_rgb -= 0x10101;
                pS++;
            }
            pP = pEm->pSin_parts_ptr;
        }
        if (pP->Size_x > 0xC00) {
            for (i = 22; i != 0; i--) {
                pP->Size_x -= 4 + (Em38_pulse[pEm->UnkDA & 0x3F] >> 1);
                pP->Size_y -= 4 + (Em38_pulse[pEm->UnkDA & 0x3F] >> 1);
                pP->Size_z -= 4 + (Em38_pulse[pEm->UnkDA & 0x3F] >> 1);
                if (i != 22)
                    Em38_part_scale_pos(pEm, pP, pP->Size_x);
                pP++;
            }
            pEm->UnkDA++;
        }
        break;
    case 4:
        if (!(pEm->UnkE2 & 0x80) && Joint_move(pEm, pKan, pSeq, 0x100))
            pEm->UnkE2 |= 0x80;
        if (!(--pEm->UnkE2 & 0x7F)) {
            pEm->UnkE2 = (Rnd_l() & 0x1F) + 3;
            EM_SET_MOVE(pEm, 0x16, 0, 0xF);
        }
        if (!(pEm->Timer0 % 45))
            Snd_se_stad(0x10214, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Timer0++;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/enemy/em38", Em38_sce03);

/* sce04: spitting acid / dying in the scenario */
void Em38_sce_spit(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pP, *pS, *pW;
    LONG C;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = ((Rnd_l() & 0x1F) << 8) + 0x7001A; /* motion 0x1A from a random frame, Hokan_flg 7 */
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (Rnd_l() & 3)
            Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_SET_MOVE(pEm, 7, 0, 0x1F);
        pEm->Routine_2 = 3;
        pEm->UnkE2 = 0;
        Snd_se_enem(10, pEm);
        pEm->Pos_y += 800;
    case 3:
        if (pEm->Move_cnt == 49) {
            pP = &pEm->pSin_parts_ptr[2];
            Sv.vx = 0;
            Sv.vy = pEm->Cdir_y;
            Sv.vz = 0;
            RotMatrix(&Sv, &Em38_spit_m);
            Em38_spit_m.t[0] = pP->Workm.t[0];
            Em38_spit_m.t[1] = pP->Workm.t[1];
            Em38_spit_m.t[2] = pP->Workm.t[2];
            Esp_call(0x440, (USHORT)pEm->Cdir_y | 0x18000000, &Em38_spit_m, 0, 0);
            Snd_se_enem(6, pEm);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            pEm->Routine_2 = 4;
        if (!(pEm->Move_cnt & 3)) {
            C = 0x100;
            pS = pEm->pSin_parts_ptr;
            Sv.vx = C - (Rnd_l() << 1);
            Sv.vy = C - (Rnd_l() << 1);
            Sv.vz = C - (Rnd_l() << 1);
            Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &pS[1].Workm, &Sv, 0);
            pW = pEm->pSin_parts_ptr;
            Sv.vx = C - (Rnd_l() << 1);
            Sv.vy = C - (Rnd_l() << 1);
            Sv.vz = C - (Rnd_l() << 1);
            Esp_call(0x1900, (((Rnd_l() << 1) + 0x1500) << 16) | (USHORT)(Rnd_l() << 4), &pW[2].Workm, &Sv, 0);
            pS = pEm->pSin_parts_ptr;
            Sv.vx = C - (Rnd_l() << 1);
            Sv.vy = C - (Rnd_l() << 1);
            Sv.vz = C - (Rnd_l() << 1);
            Esp_call(0x1900, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &pS[21].Workm, &Sv, 0);
        }
        break;
    case 5:
        pEm->Routine_2 = 6;
        pEm->Timer0 = 3;
        pP = pEm->pSin_parts_ptr;
        EM_SET_MOVE(pEm, 0x1C, 0, 0x1F);
        EM38_W(pEm)->Breath = 1;
        Sv.vx = pP->Workm.t[0];
        Sv.vy = pP->Workm.t[1];
        Sv.vz = pP->Workm.t[2];
        Esp_call(0x200, 0x20000000, &GsIDMATRIX, &Sv, 0);
        Snd_se_enem(8, pEm);
    case 6:
        Joint_move(pEm, pKan, pSeq, 0x80);
        if (--pEm->Timer0 == -1)
            pEm->Routine_2 = 0;
        break;
    }
}

/* sce: knocked down / lying */
void Em38_sce_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    EM38_SABUN *pSa;
    SVECTOR Unused;                    /* frame size */

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xA, 3, 0x1F);
        pEm->Routine_2 = 1;
        Snd_se_enem(8, pEm);
        if (!pEm->nFloor) {
            pEm->Col.W0 = 1800;
            pEm->Col.D0 = 1800;
            pEm->Unk134 = 1800;
            pEm->Be_flg &= ~0x2000;
            EM38_W(pEm)->Flg &= ~0xC;
            pEm->Be_flg &= ~0x20;
        } else {
            pEm->Col.W0 = 2000;
            pEm->Col.D0 = 2000;
            pEm->Unk134 = 0;
            pEm->Be_flg &= ~0x2000;
            EM38_W(pEm)->Wall_timer = 90;
            pEm->Be_flg |= 0x20;
            EM38_W(pEm)->Wall_spit = (Rnd_l() & 3) + 1;
        }
        pP = &pEm->pSin_parts_ptr[20];
        pSa = (EM38_SABUN *)pP->pUnk98;
        if (pSa->Key[0].Rate) {
            pSa->Key[0].Rate = 0;
            func_8004A934(pEm, pP);
        }
        pEm->Damage_cnt |= 0x80;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            pEm->Routine_2 = 2;
        Add_speedXZ(pEm, 0);
        pEm->Spd_x = 0;
        break;
    case 2:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 3;
    case 3:
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    }
}

/* Routine_0 7 */
void Em38_Dead(ENEMY_WORK *pEm)
{
}

/* G.pPldamage_Em hook (RE2 PLDAMEGE_EM): the player's damage routine by Routine_1 */
void Pldamege_Em38(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldamage_Em38_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* pldm: player thrown up and slammed down */
void Em38_pldm_slam(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *p1, *p2, *p3, *p4, *p5;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xA, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Spd_x = -1000;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10001, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Be_flg &= ~0x8000;
    case 1:
        if (pEm->Move_cnt == 7) {
            EM_SET_MOVE(pEm, 0xC, 0, 3);
            pEm->Routine_2 = 2;
        }
        Add_speedXZ(pEm, 0);
        Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
        pEm->Spd_x += 70;
        if ((pEm->Col.Hit_flg & 0x20) && !pEm->Hokan_flg) {
            EM_SET_MOVE(pEm, 8, 0, 3);
            pEm->Routine_2 = 3;
            Snd_se_stad(0x10002, (VECTOR *)&pEm->Pos_x, 0, 0);
            Pad_vib_set0(1, 0);
            Pad_vib_set1(5, 250, 0);
            Sv.vx = -300;
            Sv.vy = -600;
            Sv.vz = 0;
            p1 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1400) << 16, &p1[8].Workm, &Sv, 0);
            p2 = pEm->pSin_parts_ptr;
            Sv.vy = 0;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1400) << 16, &p2[9].Workm, &Sv, 0);
            p3 = pEm->pSin_parts_ptr;
            Sv.vy = 500;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1400) << 16, &p3[12].Workm, &Sv, 0);
        }
        break;
    case 2:
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= ~0x12;
        }
        pEm->Spd_x += 30;
        if (pEm->Spd_x > 0)
            pEm->Spd_x = 0;
        if (!pEm->Hokan_flg && pEm->Spd_x < -50 && !(pEm->Move_cnt & 1)) {
            p4 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, (Rnd_l() + 0xA00) << 16, &p4[11].Workm, 0, 0);
            p5 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, (Rnd_l() + 0xA00) << 16, &p5[14].Workm, 0, 0);
        }
        break;
    case 3:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400)) {
            EM_SET_MOVE(pEm, 9, 0, 3);
            pEm->Routine_2 = 4;
            pEm->Damage_cnt = 0;
        }
        break;
    case 4:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400)) {
            pEm->Routine_2 = 5;
            EM_SET_MOVE(pEm, 5, 0, 3);
        }
        break;
    case 5:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= ~0x12;
        }
        break;
    }
}

/* pldm: player thrown back */
void Em38_pldm_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *p1, *p2, *p3, *p4;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xB, 0, 3);
        pEm->Spd_x = 700;
        pEm->Routine_2 = 1;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10002, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Be_flg &= ~0x8000;
    case 1:
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400)) {
            EM_SET_MOVE(pEm, 9, 0, 3);
            pEm->Routine_2 = 2;
            pEm->Damage_cnt = 0;
        }
        if (pEm->Hokan_flg)
            pEm->Spd_x -= 30;
        else
            pEm->Spd_x -= 60;
        if (pEm->Spd_x < 0)
            pEm->Spd_x = 0;
        if (pEm->Move_cnt == 7) {
            p1 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p1[13].Workm, 0, 0);
            p2 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p2[10].Workm, 0, 0);
        }
        if (pEm->Move_cnt == 11) {
            p3 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p3[4].Workm, 0, 0);
            p4 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p4[7].Workm, 0, 0);
        }
        break;
    case 2:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400)) {
            pEm->Routine_2 = 3;
            EM_SET_MOVE(pEm, 5, 0, 3);
        }
        break;
    case 3:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= ~0x12;
        }
        break;
    }
}

/* pldm: player knocked down */
void Em38_pldm_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *p1, *p2, *p3;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10002, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Be_flg &= ~0x8000;
    case 1:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            EM_SET_MOVE(pEm, 9, 7, 7);
            pEm->Routine_2 = 2;
            pEm->Damage_cnt = 0;
        }
        if (pEm->Move_cnt == 4) {
            p1 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p1[13].Workm, 0, 0);
            p2 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p2[10].Workm, 0, 0);
        }
        if (pEm->Move_cnt == 5) {
            p3 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p3[8].Workm, 0, 0);
        }
        break;
    case 2:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200)) {
            pEm->Routine_2 = 3;
            EM_SET_MOVE(pEm, 5, 0, 3);
        }
        break;
    case 3:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= ~0x12;
        }
        break;
    }
}

/* pldm: player knocked back */
void Em38_pldm_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10002, (VECTOR *)&pEm->Pos_x, 0, 0);
        Esp_call(0, 0x10000000, &pEm->pSin_parts_ptr[1].Workm, 0, 0);
        Esp_call(0x100, 0x10000000, &pEm->pSin_parts_ptr[0].Workm, 0, 0);
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= ~0x12;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* G.pPldie_Em hook (RE2 PLDIE_EM): the player's death routine by Routine_1 */
void Pldie_Em38(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldie_Em38_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* pldie: player killed */
void Em38_pldie00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pW;
    PARTS_W *p1, *p2, *p3;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10002, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Be_flg &= ~0x8000;
        pW = &pEm->pSin_parts_ptr[1];
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        Esp_call(0, 0x10000000, &pW->Workm, 0, 0);
        pW = pEm->pSin_parts_ptr;
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        Esp_call(0x100, 0x18000000, &pW->Workm, 0, 0);
        pW = &pEm->pSin_parts_ptr[2];
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        pW = &pEm->pSin_parts_ptr[5];
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        Esp_call(0x100, 0x10000000, &pEm->pSin_parts_ptr[8].Workm, 0, 0);
    case 1:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            EM_SET_MOVE(pEm, 9, 0, 3);
            pEm->Routine_2 = 2;
            pEm->Damage_cnt = 0;
        }
        if (pEm->Move_cnt == 4) {
            p1 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p1[13].Workm, 0, 0);
            p2 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p2[10].Workm, 0, 0);
        }
        if (pEm->Move_cnt == 5) {
            p3 = pEm->pSin_parts_ptr;
            Esp_call(0x1003603, ((Rnd_l() << 1) + 0x1000) << 16, &p3[8].Workm, 0, 0);
        }
        if (pEm->Move_cnt == 12)
            EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}

/* pldie: player killed (second variant) */
void Em38_pldie01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pW;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10002, (VECTOR *)&pEm->Pos_x, 0, 0);
        pW = &pEm->pSin_parts_ptr[1];
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        Esp_call(0, 0x10000000, &pW->Workm, 0, 0);
        Esp_call(0x200, 0x10000000, &pW->Workm, 0, 0);
        pW = pEm->pSin_parts_ptr;
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        Esp_call(0x100, 0x18000000, &pW->Workm, 0, 0);
        Esp_call(0x200, 0x18000000, &pW->Workm, 0, 0);
        pW = &pEm->pSin_parts_ptr[2];
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        pW = &pEm->pSin_parts_ptr[5];
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        pP = pEm->pSin_parts_ptr;
        pW = &pP[8];
        pW->UnkA2 |= 4;
        PW_RGB(pW)->Rgb0 = 0x20;
        PW_RGB(pW)->Rgb1 = 0x101030;
        Esp_call(0x100, 0x10000000, &pP[8].Workm, 0, 0);
    case 1:
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 2;
            pW = pEm->pSin_parts_ptr;
            Esp_call(0x200, 0x10000000, &pW->Workm, 0, 0);
        }
        break;
    case 2:
        EM_SET_MOVE(pEm, 1, 6, 7);
        pEm->Routine_2 = 3;
        EM_SPD_XY(pEm) = 0;
        pEm->Spd_z = 0;
        Snd_se_stad(0x10003, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk120 = 1;
    case 3:
        func_80018EC8(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Routine_2 += Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}

/* body matrices of parts 0, 12, 16..19, then the slope of the part chain 12-16-17-18-19 */
void Em38_chain_set(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    PARTS_W *pP1;

    RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
    pP = pEm->pSin_parts_ptr;
    CompM(pP[0].pSuper, (MATRIX *)pP[0].M, &pP[0].Workm);
    CompM(pP[12].pSuper, (MATRIX *)pP[12].M, &pP[12].Workm);
    CompM(pP[16].pSuper, (MATRIX *)pP[16].M, &pP[16].Workm);
    CompM(pP[17].pSuper, (MATRIX *)pP[17].M, &pP[17].Workm);
    CompM(pP[18].pSuper, (MATRIX *)pP[18].M, &pP[18].Workm);
    CompM(pP[19].pSuper, (MATRIX *)pP[19].M, &pP[19].Workm);
    pP1 = pEm->pSin_parts_ptr;
    pP = &pP1[16];
    Em38_chain_slope(pEm, &pP1[12], pP);
    Em38_chain_slope(pEm, pP, &pP1[17]);
    Em38_chain_slope(pEm, &pP1[17], &pP1[18]);
    Em38_chain_slope(pEm, &pP1[18], &pP1[19]);
}

/* chain joint: turn pP1 (around its y axis, at most Lim from pP0 and 0x20 per frame) towards where its end was */
void Em38_chain_turn(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Lim)
{
    VECTOR V0;
    VECTOR V1;
    SVECTOR Rot;
    MATRIX M;
    LONG Dx, Dz;
    LONG Dir;
    USHORT B6;
    LONG D;
    ULONG A;
    LONG Ang, Add_y;

    ApplyMatrixLV(&pP0->Workm, (VECTOR *)&pP1->Pos_x, &V0);
    V0.vx = pP0->Workm.t[0] + V0.vx;
    V0.vy = pP0->Workm.t[1] + V0.vy;
    V0.vz = pP0->Workm.t[2] + V0.vz;
    V1.vx = pP1->UnkAC;
    V1.vy = pP1->UnkAE;
    V1.vz = pP1->UnkB0;
    Dx = V0.vx - V1.vx;
    Dz = V0.vz - V1.vz;
    if (Dx) {
        Ang = catan((Dz << 12) / Dx);
        if (Dx < 0)
            Dir = 0x800 - Ang;
        else
            Dir = 0x1000 - Ang;
    } else {
        Dir = 0x400;
        if (Dz > 0)
            Dir = 0xC00;
    }
    B6 = pP0->UnkB6;
    D = (USHORT)(Lim + (Dir - B6)) & 0xFFF;
    if (D < Lim * 2) {
        Rot.vx = 0;
        Rot.vy = Dir - (USHORT)pP1->UnkB6;
        Rot.vz = 0;
    } else {
        Dir = (SHORT)B6 - Lim;
        if ((ULONG)D <= 0x800)
            Dir += Lim * 2;
        Rot.vx = 0;
        Add_y = Dir - (USHORT)pP1->UnkB6;
        Rot.vy = Add_y;
        Rot.vz = 0;
        A = (Add_y - (USHORT)pP1->UnkB4 + 0x20) & 0xFFF;
        if (A >= 0x40) {
            Rot.vy = (USHORT)pP1->UnkB4 - 0x20;
            if (A <= 0x800)
                Rot.vy = pP1->UnkB4 + 0x20;
        }
    }
    pP1->UnkB4 = Rot.vy;
    RotMatrix(&Rot, &M);
    MulMatrix0(&M, &pP1->Workm, &M);
    Copy_rot_matrix(&M, &pP1->Workm);
    pP1->Workm.t[0] = V0.vx;
    pP1->Workm.t[1] = V0.vy;
    pP1->Workm.t[2] = V0.vz;
    V1.vx = 0;
    V1.vy = 1000;
    V1.vz = 0;
    ApplyMatrixLV(&pP1->Workm, &V1, &V1);
    pP1->UnkAC = pP1->Workm.t[0] + V1.vx;
    pP1->UnkAE = pP1->Workm.t[1] + V1.vy;
    pP1->UnkB0 = pP1->Workm.t[2] + V1.vz;
    Em38_chain_slope(pEm, pP0, pP1);
}

/* like Em38_chain_slope, also stores the end point of pP1's axis */
void Em38_chain_slope_end(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1)
{
    VECTOR V0;
    VECTOR V1;
    LONG Dx, Dz;
    SHORT Dir;

    pP1->Be_flg |= 2;
    ApplyMatrixLV(&pP0->Workm, (VECTOR *)&pP1->Pos_x, &V0);
    V0.vx = pP0->Workm.t[0] + V0.vx;
    V0.vy = pP0->Workm.t[1] + V0.vy;
    V0.vz = pP0->Workm.t[2] + V0.vz;
    V1.vx = 0;
    V1.vy = 1000;
    V1.vz = 0;
    ApplyMatrixLV(&pP1->Workm, &V1, &V1);
    V1.vx = pP1->Workm.t[0] + V1.vx;
    V1.vy = pP1->Workm.t[1] + V1.vy;
    V1.vz = pP1->Workm.t[2] + V1.vz;
    pP1->UnkAC = V1.vx;
    pP1->UnkAE = V1.vy;
    pP1->UnkB0 = V1.vz;
    Dx = V0.vx - V1.vx;
    Dz = V0.vz - V1.vz;
    if (Dx) {
        Dir = catan((Dz << 12) / Dx);
        if (Dx < 0)
            Dir = 0x800 - Dir;
        else
            Dir = 0x1000 - Dir;
    } else {
        Dir = 0xC00;
        if (Dz <= 0)
            Dir = 0x400;
    }
    pP1->UnkB6 = Dir;
}

/* x-z slope angle of pP1's axis (1000 along y) as seen from pP0 */
void Em38_chain_slope(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1)
{
    VECTOR V0;
    VECTOR V1;
    LONG Dx, Dz;
    SHORT Dir;

    ApplyMatrixLV(&pP0->Workm, (VECTOR *)&pP1->Pos_x, &V0);
    V0.vx = pP0->Workm.t[0] + V0.vx;
    V0.vy = pP0->Workm.t[1] + V0.vy;
    V0.vz = pP0->Workm.t[2] + V0.vz;
    V1.vx = 0;
    V1.vy = 1000;
    V1.vz = 0;
    ApplyMatrixLV(&pP1->Workm, &V1, &V1);
    V1.vx = pP1->Workm.t[0] + V1.vx;
    V1.vy = pP1->Workm.t[1] + V1.vy;
    V1.vz = pP1->Workm.t[2] + V1.vz;
    Dx = V0.vx - V1.vx;
    Dz = V0.vz - V1.vz;
    if (Dx) {
        Dir = catan((Dz << 12) / Dx);
        if (Dx < 0)
            Dir = 0x800 - Dir;
        else
            Dir = 0x1000 - Dir;
    } else {
        Dir = 0xC00;
        if (Dz <= 0)
            Dir = 0x400;
    }
    pP1->UnkB6 = Dir;
}

/* body morph (part 22): blend between the seven morph targets in turn */
void Em38_morph_body(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG Spd;

    pP = &pEm->pSin_parts_ptr[22];
    Spd = 0x200;
    if (pEm->Routine_0 == 2 || pEm->Routine_0 == 3)
        Spd = 0x400;
    if (pEm->Routine_0 == 7)
        Spd = 0x40;
    pP->Cdir_y += 0x40;
    switch (pP->Unk74) {
    case 0:
        EM38_SA(pP)->Key[4].Rate -= Spd;
        if (EM38_SA(pP)->Key[4].Rate <= 0)
            EM38_SA(pP)->Key[4].Rate = 0;
        EM38_SA(pP)->Key[6].Rate -= Spd;
        if (EM38_SA(pP)->Key[6].Rate <= 0)
            EM38_SA(pP)->Key[6].Rate = 0;
        EM38_SA(pP)->Key[0].Rate += Spd;
        if (EM38_SA(pP)->Key[0].Rate >= 0x1000) {
            EM38_SA(pP)->Key[0].Rate = 0x1000;
            pP->Unk74 = 1;
            if (Rnd_l() & 3)
                pP->Unk74 = 3;
        }
        break;
    case 1:
        EM38_SA(pP)->Key[5].Rate -= Spd;
        if (EM38_SA(pP)->Key[5].Rate <= 0)
            EM38_SA(pP)->Key[5].Rate = 0;
        EM38_SA(pP)->Key[0].Rate -= Spd;
        if (EM38_SA(pP)->Key[0].Rate <= 0)
            EM38_SA(pP)->Key[0].Rate = 0;
        EM38_SA(pP)->Key[1].Rate += Spd;
        if (EM38_SA(pP)->Key[1].Rate >= 0x1000) {
            EM38_SA(pP)->Key[1].Rate = 0x1000;
            pP->Unk74 = 2;
            if (Rnd_l() & 3)
                pP->Unk74 = 4;
        }
        break;
    case 2:
        EM38_SA(pP)->Key[6].Rate -= Spd;
        if (EM38_SA(pP)->Key[6].Rate <= 0)
            EM38_SA(pP)->Key[6].Rate = 0;
        EM38_SA(pP)->Key[1].Rate -= Spd;
        if (EM38_SA(pP)->Key[1].Rate <= 0)
            EM38_SA(pP)->Key[1].Rate = 0;
        EM38_SA(pP)->Key[2].Rate += Spd;
        if (EM38_SA(pP)->Key[2].Rate >= 0x1000) {
            EM38_SA(pP)->Key[2].Rate = 0x1000;
            pP->Unk74 = 3;
            if (Rnd_l() & 3)
                pP->Unk74 = 5;
        }
        break;
    case 3:
        EM38_SA(pP)->Key[0].Rate -= Spd;
        if (EM38_SA(pP)->Key[0].Rate <= 0)
            EM38_SA(pP)->Key[0].Rate = 0;
        EM38_SA(pP)->Key[2].Rate -= Spd;
        if (EM38_SA(pP)->Key[2].Rate <= 0)
            EM38_SA(pP)->Key[2].Rate = 0;
        EM38_SA(pP)->Key[3].Rate += Spd;
        if (EM38_SA(pP)->Key[3].Rate >= 0x1000) {
            EM38_SA(pP)->Key[3].Rate = 0x1000;
            pP->Unk74 = 4;
            if (Rnd_l() & 3)
                pP->Unk74 = 6;
        }
        break;
    case 4:
        EM38_SA(pP)->Key[1].Rate -= Spd;
        if (EM38_SA(pP)->Key[1].Rate <= 0)
            EM38_SA(pP)->Key[1].Rate = 0;
        EM38_SA(pP)->Key[3].Rate -= Spd;
        if (EM38_SA(pP)->Key[3].Rate <= 0)
            EM38_SA(pP)->Key[3].Rate = 0;
        EM38_SA(pP)->Key[4].Rate += Spd;
        if (EM38_SA(pP)->Key[4].Rate >= 0x1000) {
            EM38_SA(pP)->Key[4].Rate = 0x1000;
            pP->Unk74 = 5;
            if (Rnd_l() & 3)
                pP->Unk74 = 0;
        }
        break;
    case 5:
        EM38_SA(pP)->Key[2].Rate -= Spd;
        if (EM38_SA(pP)->Key[2].Rate <= 0)
            EM38_SA(pP)->Key[2].Rate = 0;
        EM38_SA(pP)->Key[4].Rate -= Spd;
        if (EM38_SA(pP)->Key[4].Rate <= 0)
            EM38_SA(pP)->Key[4].Rate = 0;
        EM38_SA(pP)->Key[5].Rate += Spd;
        if (EM38_SA(pP)->Key[5].Rate >= 0x1000) {
            EM38_SA(pP)->Key[5].Rate = 0x1000;
            pP->Unk74 = 6;
            if (Rnd_l() & 3)
                pP->Unk74 = 1;
        }
        break;
    case 6:
        EM38_SA(pP)->Key[3].Rate -= Spd;
        if (EM38_SA(pP)->Key[3].Rate <= 0)
            EM38_SA(pP)->Key[3].Rate = 0;
        EM38_SA(pP)->Key[5].Rate -= Spd;
        if (EM38_SA(pP)->Key[5].Rate <= 0)
            EM38_SA(pP)->Key[5].Rate = 0;
        EM38_SA(pP)->Key[6].Rate += Spd;
        if (EM38_SA(pP)->Key[6].Rate >= 0x1000) {
            EM38_SA(pP)->Key[6].Rate = 0x1000;
            pP->Unk74 = 0;
            if (Rnd_l() & 3)
                pP->Unk74 = 2;
        }
        break;
    }
    func_8004A934(pEm, pP);
}

/* dust from the feet on footstep frames */
void Em38_foot_dust(ENEMY_WORK *pEm)
{
    SVECTOR Sv;

    Sv.vy = 0;
    if ((pEm->Move_cnt & 0xF) == 1) {
        Sv.vx = 0x3A0 - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(0x2001800, 0x10000000, &pEm->pSin_parts_ptr[2].Workm, &Sv, 0);
    }
    if ((pEm->Move_cnt & 0xF) == 4 && (Rnd_l() & 1)) {
        Sv.vx = 0x3A0 - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(0x2001800, 0x10000000, &pEm->pSin_parts_ptr[3].Workm, &Sv, 0);
    }
    if (pEm->Routine_0 != 4) {
        if ((pEm->Move_cnt & 0xF) == 6) {
            Sv.vx = 0x3A0 - Rnd_l();
            Sv.vz = 0x80 - Rnd_l();
            Esp_call(0x2001800, 0x10000000, &pEm->pSin_parts_ptr[3].Workm, &Sv, 0);
        }
        if ((pEm->Move_cnt & 0xF) == 12 && (Rnd_l() & 1)) {
            Sv.vx = 0x3A0 - Rnd_l();
            Sv.vz = 0x80 - Rnd_l();
            Esp_call(0x2001800, 0x10000000, &pEm->pSin_parts_ptr[2].Workm, &Sv, 0);
        }
    }
}

/* head sway / shake and turning the head (part 1, 2) towards the player */
void Em38_head_move(ENEMY_WORK *pEm)
{
    SVECTOR Rot;
    MATRIX M;
    MATRIX Unused_m;                   /* frame size */
    VECTOR Unused_v;
    UCHAR Amp[8] = { 4, 6, 0xFC, 0xFA, 0xFE, 0, 2, 0 };
    LONG Shake;
    ENEMY_WORK *pPl;
    PARTS_W *pP;
    LONG Dir;
    ULONG D;
    USHORT Tgt, Cur;

    Shake = 0;
    pPl = &Pl_work;
    switch (EM38_W(pEm)->Shake_no) {
    case 0:
        break;
    case 1:
        EM38_W(pEm)->Shake_amp = Amp[Rnd_l() & 7];
        EM38_W(pEm)->Shake_no = 2;
        EM38_W(pEm)->Shake_cnt = 0;
        EM38_W(pEm)->Shake_side = 1;
        if (pEm->Unk132 & 0x10)
            EM38_W(pEm)->Shake_side = -1;
    case 2:
        Rot.vx = 0;
        Rot.vy = -(EM38_W(pEm)->Shake_cnt * EM38_W(pEm)->Shake_amp * 8) * EM38_W(pEm)->Shake_side;
        Rot.vz = -(EM38_W(pEm)->Shake_cnt * 32) * EM38_W(pEm)->Shake_side;
        RotMatrix(&Rot, &M);
        if (EM38_W(pEm)->Shake_cnt++ >= 3) {
            EM38_W(pEm)->Shake_no = 3;
            EM38_W(pEm)->Shake_cnt = 16;
        }
        Shake = 1;
        break;
    case 3:
        Rot.vx = 0;
        Rot.vy = -(EM38_W(pEm)->Shake_cnt * EM38_W(pEm)->Shake_amp) * EM38_W(pEm)->Shake_side;
        Rot.vz = -(EM38_W(pEm)->Shake_cnt * 4) * EM38_W(pEm)->Shake_side;
        RotMatrix(&Rot, &M);
        if (--EM38_W(pEm)->Shake_cnt == -1)
            EM38_W(pEm)->Shake_no = 0;
        Shake = 1;
        break;
    }
    EM38_W(pEm)->Old_dir = (EM38_W(pEm)->Old_dir - pEm->Cdir_y) & 0xFFF;
    Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z) - (USHORT)pEm->Cdir_y;
    D = (Dir + 0x19C) & 0xFFF;
    Dir &= 0xFFF;
    if (D < 0x338) {
        EM38_W(pEm)->Head_tgt = Dir;
        if (Dir > 0x800)
            EM38_W(pEm)->Head_tgt = 0x1000 - Dir;
    } else {
        EM38_W(pEm)->Head_tgt = 0x19C;
        if (D > 0x800)
            EM38_W(pEm)->Head_tgt = -0x19C;
    }
    if (!EM38_W(pEm)->Old_dir)
        EM38_W(pEm)->Head_tgt = 0;
    Tgt = EM38_W(pEm)->Head_tgt;
    Cur = EM38_W(pEm)->Head_dir;
    D = (Tgt - Cur + 8) & 0xFFF;
    if (D < 0x10) {
        EM38_W(pEm)->Head_dir = Tgt;
    } else {
        EM38_W(pEm)->Head_dir = Cur - 8;
        if (D <= 0x800)
            EM38_W(pEm)->Head_dir = Cur + 8;
    }
    if (!(pEm->Type & 3)) {
        pP = &pEm->pSin_parts_ptr[1];
        Em38_cnt += 0x40;
        pP->Cdir_x += EM38_W(pEm)->Head_dir;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        pP->Cdir_x -= EM38_W(pEm)->Head_dir;
        pP = &pEm->pSin_parts_ptr[2];
        pP->Cdir_y += EM38_W(pEm)->Head_dir >> 1;
        RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        pP->Cdir_y -= EM38_W(pEm)->Head_dir >> 1;
    }
    if (Shake)
        MulMatrix((MATRIX *)pEm->pSin_parts_ptr[1].M, &M);
}

/* hit effect (blood) at part 1 towards the player */
void Em38_blood_head(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    SVECTOR Rot;
    MATRIX M;
    PARTS_W *pP;
    SHORT Dir;

    Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
    if (!(pEm->Unk132 & 0xA00)) {
        if ((pEm->Unk132 & 3) == 1) {
            pP = &pEm->pSin_parts_ptr[1];
            Sv.vx = 0x578;
            Sv.vy = 0x1F4;
            Sv.vz = 0;
            Rot.vx = 0;
            Rot.vy = Dir;
            Rot.vz = 0;
            RotMatrix(&Rot, &M);
            ApplyMatrixSV(&M, &Sv, &Sv);
            Sv.vx += pP->Workm.t[0];
            Sv.vy += pP->Workm.t[1];
            Sv.vz += pP->Workm.t[2];
            Esp_call(0x1900, ((Rnd_l() << 1) + 0x2000) << 16, &GsIDMATRIX, &Sv, 0);
            Esp_call(0x700, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &GsIDMATRIX, &Sv, 0);
        } else {
            pP = &pEm->pSin_parts_ptr[1];
            Sv.vx = 0x578;
            Sv.vy = -0x2BC;
            Sv.vz = 0;
            Rot.vx = 0;
            Rot.vy = Dir;
            Rot.vz = 0;
            RotMatrix(&Rot, &M);
            ApplyMatrixSV(&M, &Sv, &Sv);
            Sv.vx += pP->Workm.t[0];
            Sv.vy += pP->Workm.t[1];
            Sv.vz += pP->Workm.t[2];
            Esp_call(0x1900, ((Rnd_l() << 1) + 0x2000) << 16, &GsIDMATRIX, &Sv, 0);
            Esp_call(0x700, (((Rnd_l() << 1) + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &GsIDMATRIX, &Sv, 0);
        }
    }
}

/* unused */
void Em38_dummy(void)
{
}

/* hit by fire: smoke from the body parts */
void Em38_fire_smoke(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;

    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    Sv.vy = 0;
    Sv.vx = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - pEm->Cdir_y + 0x100;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    Sv.vx = 0xEFA;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Unk178);
    pP = pEm->pSin_parts_ptr;
    pEm->Unk17E = 0x230;
    Esp_call(0x1C03, ((Rnd_l() << 4) + 0x1000) << 16, &pP[20].Workm, 0, 0);
    pS = pEm->pSin_parts_ptr;
    Sv.vx = 400;
    Sv.vy = -400;
    Sv.vz = 0;
    Esp_call(0x1C03, ((Rnd_l() << 4) + 0x1000) << 16, &pS[2].Workm, &Sv, 0);
    pS = pEm->pSin_parts_ptr;
    Sv.vx = 0;
    Sv.vy = 400;
    Sv.vz = 0;
    Esp_call(0x1C03, ((Rnd_l() << 4) + 0x1000) << 16, &pS[9].Workm, &Sv, 0);
    pW = pEm->pSin_parts_ptr;
    pW->UnkA2 |= 0x40;
    pW = &pEm->pSin_parts_ptr[2];
    pW->UnkA2 |= 0x40;
}

/* hit by acid: sparks from the body parts */
void Em38_acid_spark(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;

    pEm->Status_flg |= 1;
    pP = pEm->pSin_parts_ptr;
    Esp_call(0xB05, ((Rnd_l() << 4) + 0x1000) << 16, &pP[20].Workm, 0, 0);
    Esp_call(0x1001B03, ((Rnd_l() << 4) + 0xA00) << 16, &pP[20].Workm, 0, 0);
    pS = pEm->pSin_parts_ptr;
    Sv.vx = 400;
    Sv.vy = -400;
    Sv.vz = 0;
    Esp_call(0xB05, ((Rnd_l() << 4) + 0x1800) << 16, &pS[2].Workm, &Sv, 0);
    pW = pEm->pSin_parts_ptr;
    Sv.vx = 0;
    Sv.vy = 400;
    Sv.vz = 0;
    Esp_call(0xB05, ((Rnd_l() << 4) + 0x2000) << 16, &pW[9].Workm, &Sv, 0);
}

/* hit by a blast (Unk132 0x200): light the body (Unk178, faded in Em38_main) and burst effects */
void Em38_blast_light(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;

    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    Sv.vy = 0;
    Sv.vx = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - pEm->Cdir_y + 0x100;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    Sv.vx = 0xF00;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Unk178);
    pP = pEm->pSin_parts_ptr;
    pEm->Unk17E = 0x40;
    Esp_call(0x1803, ((Rnd_l() << 4) + 0x1000) << 16, &pP[20].Workm, 0, 0);
    pS = pEm->pSin_parts_ptr;
    Sv.vx = 400;
    Sv.vy = -400;
    Sv.vz = 0;
    Esp_call(0x1803, ((Rnd_l() << 4) + 0x1000) << 16, &pS[2].Workm, &Sv, 0);
    pS = pEm->pSin_parts_ptr;
    Sv.vx = 0;
    Sv.vy = 400;
    Sv.vz = 0;
    Esp_call(0x1803, ((Rnd_l() << 4) + 0x1000) << 16, &pS[9].Workm, &Sv, 0);
    pW = pEm->pSin_parts_ptr;
    pW->UnkA2 |= 0x40;
    pW = &pEm->pSin_parts_ptr[2];
    pW->UnkA2 |= 0x40;
}

/* breath: a row of puffs from part 1 towards the player */
void Em38_breath(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    SVECTOR V0;
    SVECTOR V1;
    MATRIX M;
    PARTS_W *pP;
    LONG Y;
    LONG Sz;

    switch (EM38_W(pEm)->Breath) {
    case 0:
        break;
    case 1:
        EM38_W(pEm)->Breath = 2;
        EM38_W(pEm)->Puff_cnt = 0;
        EM38_W(pEm)->Breath_dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z);
    case 2:
        Sv.vx = 0;
        Sv.vy = EM38_W(pEm)->Breath_dir;
        Sv.vz = 0;
        RotMatrix(&Sv, &M);
        Y = -600;
        Sv.vx = -(EM38_W(pEm)->Puff_cnt * 120);
        Sv.vy = Y;
        ApplyMatrixSV(&M, &Sv, &V0);
        Sv.vx = -120;
        Sv.vy = Y;
        Sv.vz = 0;
        ApplyMatrixSV(&M, &Sv, &V1);
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Workm.t[0] + V0.vx;
        Sv.vy = pP->Workm.t[1] + V0.vy;
        Sv.vz = pP->Workm.t[2] + V0.vz;
        Sz = 0x1280;
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 70) << 16, &GsIDMATRIX, &Sv, 0);
        EM38_W(pEm)->Puff_cnt++;
        Sv.vx = pP->Workm.t[0] + V0.vx + V1.vx - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + V0.vz + V1.vz - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 70) << 16, &GsIDMATRIX, &Sv, 0);
        EM38_W(pEm)->Puff_cnt++;
        Sv.vx = pP->Workm.t[0] + V0.vx + (V1.vx << 1);
        Sv.vz = pP->Workm.t[2] + V0.vz + (V1.vz << 1);
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 128) << 16, &GsIDMATRIX, &Sv, 0);
        EM38_W(pEm)->Puff_cnt++;
        Sv.vx = pP->Workm.t[0] + V0.vx + V1.vx * 3 - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + V0.vz + V1.vz * 3 - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 128) << 16, &GsIDMATRIX, &Sv, 0);
        EM38_W(pEm)->Puff_cnt++;
        Sv.vx = pP->Workm.t[0] + V0.vx + (V1.vx << 2);
        Sv.vz = pP->Workm.t[2] + V0.vz + (V1.vz << 2);
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 128) << 16, &GsIDMATRIX, &Sv, 0);
        EM38_W(pEm)->Puff_cnt++;
        Sv.vx = pP->Workm.t[0] + V0.vx + V1.vx * 5 - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + V0.vz + V1.vz * 5 - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 128) << 16, &GsIDMATRIX, &Sv, 0);
        EM38_W(pEm)->Puff_cnt++;
        Sv.vx = pP->Workm.t[0] + V0.vx + V1.vx * 6;
        Sv.vz = pP->Workm.t[2] + V0.vz + V1.vz * 6;
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 128) << 16, &GsIDMATRIX, &Sv, 0);
        EM38_W(pEm)->Puff_cnt++;
        Sv.vx = pP->Workm.t[0] + V0.vx + V1.vx * 7 - (SHORT)(Rnd_l() - 0x80);
        Sv.vz = pP->Workm.t[2] + V0.vz + V1.vz * 7 - (SHORT)(Rnd_l() - 0x80);
        Esp_call(0x1900, (Sz - Rnd_l() - EM38_W(pEm)->Puff_cnt * 128) << 16, &GsIDMATRIX, &Sv, 0);
        if (EM38_W(pEm)->Puff_cnt++ > 16)
            EM38_W(pEm)->Breath = 0;
        break;
    }
}

/* the four pustules (parts 27-30): burst when hit, regrow */
void Em38_pustule(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pW;
    PARTS_W *pB;
    MATRIX *pM;
    LONG i;

    pB = pEm->pSin_parts_ptr;
    pP = &pB[27];
    pW = &pB[1];
    pM = &pB[1].Workm;
    for (i = 4; i != 0; pP++, i--) {
        switch (pP->UnkB4) {
        case 0:
            break;
        case 1:
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            pP->Poly_rgb = 0x808080;
            PW_FLY(pP)->Gravity = 3;
            pP->UnkB4 = 2;
            PW_FLY(pP)->Dir_y = 0;
            PW_FLY(pP)->Cnt = 0;
            PW_FLY(pP)->Spd_xz = 0;
            PW_FLY(pP)->Spd_y = 0;
            PW_FLY(pP)->Spd_xz_add = 0;
            pP->UnkAC = 90;
            Sv.vx = pP[-4].Pos_x;
            Sv.vy = pP[-4].Pos_y;
            Sv.vz = pP[-4].Pos_z;
            Esp_call(0x2000200, 0x20000000, pM, &Sv, 0);
            Esp_call(0x1000200, 0x20000800, pM, &Sv, 0);
            Esp_call(0xB40, (USHORT)(Rnd_l() << 4) | 0x10000000, pM, &Sv, 0);
            Esp_call(0xB40, (USHORT)(Rnd_l() << 4) | 0x10000000, pM, &Sv, 0);
            Esp_call(0xF40, (USHORT)(Rnd_l() << 4) | 0x10000000, pM, &Sv, 0);
            pP -= 4;
            if (!pP->UnkB4)
                pP->UnkB4 = 1;
            pP += 4;
            break;
        case 2:
            if (--pP->UnkAC == -1)
                pP->UnkB4 = 3;
            break;
        case 3:
            pP->UnkB4 = 4;
            pP->Poly_rgb = 0;
            if (!(pEm->Type & 3)) {
                pP->UnkA2 &= ~0x20;
                pP->Be_flg = (pP->Be_flg | 1) & ~2;
                Sv.vx = pP[-4].Pos_x;
                Sv.vy = pP[-4].Pos_y;
                Sv.vz = pP[-4].Pos_z;
                Esp_call(0x1903, 0x18000000, pM, &Sv, 0);
                Esp_call(0x2D03, (USHORT)(Rnd_l() << 4) | 0x14000000, pM, &Sv, 0);
                Esp_call(0x2D03, (USHORT)(Rnd_l() << 4) | 0x14000000, pM, &Sv, 0);
                Esp_call(0x2D03, (USHORT)(Rnd_l() << 4) | 0x14000000, pM, &Sv, 0);
                pP->UnkAC = 0;
            }
        case 4:
            pP->Poly_rgb += 0x10101;
            if (pP->Poly_rgb == 0x808080) {
                pP->UnkB4 = 0;
                Snd_se_enem(12, pEm);
            }
            if (!(++pP->UnkAC & 3) && pP->UnkAC < 15) {
                Sv.vx = pP[-4].Pos_x;
                Sv.vy = pP[-4].Pos_y;
                Sv.vz = pP[-4].Pos_z;
                Esp_call(0x2D03, (USHORT)(Rnd_l() << 4) | 0x14000000, &pW->Workm, &Sv, 0);
            }
            break;
        }
    }
}

/* the four pustule shells (parts 23-26): spin, fly off when burst, fall and explode */
void Em38_pustule_shell(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    MATRIX M;
    SVECTOR Sv;
    SVECTOR Rot;
    PARTS_W *pP;
    LONG i;
    SHORT Dir;

    pP = &pEm->pSin_parts_ptr[23];
    for (i = 4; i != 0; pP++, i--) {
        switch (pP->UnkB4) {
        case 0:
            pP->Cdir_y += 0x20;
            pP->Cdir_z += 0x10;
            RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
            break;
        case 1:
            pP->UnkB2 = -200;
            pP->UnkB4 = 2;
            pP->Be_flg |= 6;
            pP->Size_x = 0x1C00;
            pP->Size_y = 0x1C00;
            pP->Size_z = 0x1C00;
            Sv.vx = 250;
            Sv.vy = 0;
            Sv.vz = 0;
            Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
            Rot.vx = 0;
            Rot.vy = Dir;
            Rot.vz = 0;
            RotMatrix(&Rot, &M);
            ApplyMatrixSV(&M, &Sv, &Sv);
            pP->UnkB6 = Sv.vx;
            pP->UnkAE = Sv.vz;
            pP->UnkB0 = 0;
            pP->UnkAC = 0;
            if (pEm->Type & 1) {
                pP->UnkB4 = 5;
                break;
            }
        case 2:
            pP->Workm.t[1] += pP->UnkB2;
            pP->UnkB2 += 30;
            if (pP->Workm.t[1] >= -99) {
                pP->Workm.t[1] = -100;
                pP->UnkB4 = 3;
                Snd_se_enem(15, pEm);
            }
            pP->Workm.t[0] += pP->UnkB6;
            pP->Workm.t[2] += pP->UnkAE;
            if (func_8004D720(pP->Workm.t, 1, 0x2000, 0)) {
                pP->Workm.t[0] -= pP->UnkB6;
                pP->Workm.t[2] -= pP->UnkAE;
                pP->UnkB6 = 0;
                pP->UnkAE = 0;
            }
            if (++pP->UnkAC & 1)
                Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1900, ((Rnd_l() << 1) + 0x1400) << 16, &pP->Workm, 0, 0);
            Rot.vx = 0;
            Rot.vy = 0x20;
            Rot.vz = 0x10;
            RotMatrix(&Rot, &M);
            MulMatrix(&pP->Workm, &M);
            if ((pP->UnkAC & 0xF) < 8) {
                pP->Size_x -= 0x100;
                pP->Size_y -= 0x100;
                pP->Size_z -= 0x100;
            } else {
                pP->Size_x += 0x100;
                pP->Size_y += 0x100;
                pP->Size_z += 0x100;
            }
            break;
        case 3:
            pP->UnkB4 = 4;
            pP->UnkAC = 30;
        case 4:
            if (--pP->UnkAC == -1) {
                pP->UnkB4 = 5;
                Esp_call(0x1240, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
                Esp_call(0x1240, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
                Esp_call(0x1240, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
                Esp_call(0x1240, (USHORT)(Rnd_l() << 4) | 0x10000000, &pP->Workm, 0, 0);
                Snd_se_enem(12, pEm);
            }
            if (!(pP->UnkAC & 7))
                Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1900, ((Rnd_l() << 1) + 0x1000) << 16, &pP->Workm, 0, 0);
            if (pP->UnkB6 || pP->UnkAE) {
                Rot.vx = 0;
                Rot.vy = 0x60;
                Rot.vz = 0x20;
                RotMatrix(&Rot, &M);
                MulMatrix(&pP->Workm, &M);
            }
            pP->UnkB6 -= pP->UnkB6 >> 3;
            pP->UnkAE -= pP->UnkAE >> 3;
            pP->Workm.t[0] += pP->UnkB6;
            pP->Workm.t[2] += pP->UnkAE;
            if (func_8004D720(pP->Workm.t, 1, 0x2000, 0)) {
                pP->Workm.t[0] -= pP->UnkB6;
                pP->Workm.t[2] -= pP->UnkAE;
                pP->UnkB6 = 0;
                pP->UnkAE = 0;
            }
            if ((pP->UnkAC & 0xF) < 8) {
                pP->Size_x -= 0x100;
                pP->Size_y -= 0x100;
                pP->Size_z -= 0x100;
            } else {
                pP->Size_x += 0x100;
                pP->Size_y += 0x100;
                pP->Size_z += 0x100;
            }
            break;
        case 5:
            pP->UnkB4 = 0;
            pP->Be_flg &= ~6;
            pP->Poly_rgb = 0x808080;
            pP->Be_flg &= ~1;
            if (!(pEm->Type & 3)) {
                pP->UnkA2 &= ~0x20;
                pP->Be_flg |= 1;
            }
            break;
        }
    }
}

/* part position from the animation's bone offsets scaled by Rate */
void Em38_part_scale_pos(ENEMY_WORK *pEm, PARTS_W *pP, LONG Rate)
{
    SHORT *pOfs = (SHORT *)(pEm->pKan_t_ptr + 2);

    pOfs += pP->Unk84 * 3;
    pP->Pos_x = (pOfs[0] * Rate) >> 12;
    pP->Pos_y = (pOfs[1] * Rate) >> 12;
    pP->Pos_z = (pOfs[2] * Rate) >> 12;
}
