#include "ovl/em20.h"
#include "door.h"
#include "joint.h"
#include "root_ck.h"
#include "snd.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
void func_8002646C_(ENEMY_WORK *pEm, LONG A, LONG B, LONG C) __asm__("func_8002646C"); /* arg 2 long (canonical short); arg 3 long (canonical short); arg 4 long (canonical UC) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");      /* em_sub.c, result used as int */
void Add_speedXZ3(ENEMY_WORK *pEm, LONG muki, LONG a2) __asm__("Add_speedXZ"); /* 3 args here (canonical 2) */

/* RE2 EM_MAIN */
void EM_MAIN(ENEMY_WORK *pEm)
{
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (EM20_W(pEm)->No_fire)
        EM20_W(pEm)->No_fire--;
    Em20_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if ((pEm->Pos_x == pEm->Old_x && pEm->Pos_z == pEm->Old_z) || pEm->Col.Unk3 != 0xFF)
        EM20_W(pEm)->Stay++;
    else
        EM20_W(pEm)->Stay = 0;
    if ((pEm->Col.Hit_flg & 0x40) && !EM20_W(pEm)->Jump_flg)
        func_8002646C_(pEm, 400, 50, pEm->Col.Unk39);
    else
        pEm->Be_flg |= 0x80000;
}

/* RE2 Em20_Init */
void Em20_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    EM20_FREE *pFp;
    COL_OBJ *pCol;
    ULONG i;

    pFp = EM20_W(pEm);
    EM_RTN(pEm) = 1;
    pEm->Unk144 = 0;
    for (i = 0; i < 12; i++)
        pEm->Unk1CC[i] = 0;
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = Em20_life_easy[(Rnd_l() >> (Rnd_l() & 3)) & 0xF];
    else
        pEm->Max_life = pEm->Life = Em20_life[(Rnd_l() >> (Rnd_l() & 3)) & 0xF];
    pEm->Max_life += 10;
    pEm->Life += 10;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x25804B0, 0, (VECTOR *)&pEm->Pos_x);
    pCol = &pEm->Col;
    pCol->pEm = pEm;
    pCol->W0 = pCol->D0 = 600;
    pCol->H = 2000;
    pCol->Unk24 = 500;
    pCol->Unk26 = 0;
    pCol->Unk28 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    func_80033580(pCol, 0);
    pEm->Unk136 = -1000;
    pEm->Unk134 = 1200;
    pEm->Unk138 = 0x54;
    pEm->Unk139 = 0xA0;
    pEm->Unk13A = 600;
    pEm->Unk140 = 0;
    pEm->pEnemy_ptr = &Pl_work;
    pEm->Unk120 = 0;
    pEm->Neck_no = 2;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr[1].Workm;
    for (i = 2; i < 4; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        PW_FREE(pP)[0] = 0;
        PW_FREE(pP)[1] = 0;
        PW_FREE(pP)[2] = 0x20;
        PW_FREE(pP)[3] = 0;
        PW_FREE(pP)[4] = 0x180;
        PW_FREE(pP)[5] = 0x80;
    }
    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Routine_1 = em20_routine_tbl[pEm->Type & 0x7F];
    switch (pEm->Type & 0x7F) {
    case 0:
    case 1:
    case 4:
        EM_MOVE(pEm) = ((Rnd_l() & 0x3F) << 8) + 0xF0001;
        pEm->Unk132 &= 0x3FFF;
        break;
    case 2:
    case 6:
        EM_MOVE(pEm) = ((Rnd_l() & 0xF) << 8) + 0xF0002;
        pEm->Unk132 &= 0x3FFF;
        break;
    case 3:
        EM_MOVE(pEm) = 0xF0006;
        pEm->Unk132 |= 0x8000;
        break;
    case 7:
        EM_MOVE(pEm) = ((Rnd_l() & 0x1F) << 8) + 0xF000B;
        pEm->UnkE2 = (Rnd_l() & 3) + 1;
        pEm->Unk120 &= 0xFD;
        pFp->House_flg = 1;
        pEm->Routine_3 = 1;
    case 8:
        EM_MOVE(pEm) = 0xF0000;
        pFp->House_flg = 1;
        pEm->Unk132 &= 0x3FFF;
        break;
    case 9:
        EM_MOVE(pEm) = ((Rnd_l() & 0x3F) << 8) + 0xF0001;
        pFp->Outside = 1;
    case 10:
        EM_MOVE(pEm) = 0xF0000;
        EM_RTN(pEm) = 4;
        pEm->UnkDA = 0;
        pEm->Type |= 0xC000;
        pEm->Be_flg |= 0x6030;
        pEm->Unk132 &= 0x3FFF;
        Em20_Col_set(pEm, 0);
        break;
    case 5:
        EM_MOVE(pEm) = ((Rnd_l() & 0x3F) << 8) + 0x1F000E;
        EM_RTN(pEm) = 0xC01;
        break;
    default:
        pEm->Unk132 &= 0x3FFF;
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
    if ((pEm->Type & 0x80) && (pEm->Type & 0x7F) != 10)
        Em20_Esp_set(pEm);
    G.pPldie_Em[16] = PLDIE_EM;
}

/* RE3 only: burning/smoking effects on four parts (Type & 0x80) */
void Em20_Esp_set(ENEMY_WORK *pEm)
{
    EM20_ESP_TBL *pT;
    PARTS_W *pP;
    ULONG Col;
    LONG i;

    i = 0;
    pT = em20_esp_tbl;
    Col = 0xA0D0E0;
    for (; i < 4; i++, pT++) {
        pP = &pEm->pSin_parts_ptr[pT->Parts];
        pP->Poly_rgb = Col;
        Esp_call(5, (pT->Type << 16) | (USHORT)pT->Ofs.pad, &pP->Workm, &pT->Ofs, NULL);
    }
}

/* RE2 Em20_Move */
void Em20_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    em20_move_tbl[pEm->Routine_1](pEm, pKan, pSeq);
    Em_seq_sound(pEm);
}

/* RE2 Em20_rndwalk */
LONG Em20_rndwalk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;
    LONG ck = 0;

    pFp = EM20_W(pEm);
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Spd_x = 30;
        pEm->Spd_z = 0;
        pEm->Unk120 |= 2;
        pEm->Unk132 &= 0x3FFF;
        pFp->Jump_flg = 0;
        pFp->House_flg = 0;
        pEm->Damage_cnt &= 0x7F;
        break;
    case 1:
        if (pEm->Col.Hit_flg & 0x40)
            pEm->Spd_x = 48;
        else
            pEm->Spd_x = 30;
        Add_speedXZ(pEm, 0);
        if ((EM20_SCA_INFO(pEm) & 0x600000) == 0x200000) {
            pEm->Routine_2 = !(Rnd_l() & 0x1F) ? 2 : 3;
            pFp->Area_keep = Rnd_area();
            if (pEm->Routine_2 == 2) {
                pEm->UnkDA = 90;
                EM_MOVE(pEm) = 0xF0000;
            }
        }
        break;
    case 2:
        if (--pEm->UnkDA == 0) {
            pEm->Routine_2 = 3;
            EM_MOVE(pEm) = 0xF0001;
        }
        break;
    case 3:
        if (pEm->Col.Hit_flg & 0x40)
            pEm->Spd_x = 48;
        else
            pEm->Spd_x = 30;
        ck = Root_ck(pEm, NULL, pFp->Area_keep, 1);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Add_speedXZ(pEm, 0);
        if ((EM20_SCA_INFO(pEm) & 0x600000) == 0x200000)
            pEm->Routine_2 = 1;
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
    Dog_rot_neck(pEm);
    if (!pFp->Blood && !(Rnd_l() & 0x3F))
        pFp->Blood = em20_time_tbl00[Rnd_l() & 7];
    Set_blood(pEm, 4, 6);
    return ck;
}

/* RE2 Em20_Move_walk */
void Em20_Move_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG ck;

    ck = Em20_rndwalk(pEm, pKan, pSeq);
    if (__builtin_abs(G.Player.nFloor - pEm->nFloor) < 2 || (pEm->Status_flg & 0x2000)) {
        if ((LONG)pEm->Unk154 < 4000 || ck == 1 || (G.Unk75EA & 0x20)) {
            pEm->Routine_1 = 1;
            EM_RTN23(pEm) = 0;
        } else if ((G.Unk75E8 & 1) && !(pEm->Type & 0x100)) {
            pEm->Routine_1 = 2;
            EM_RTN23(pEm) = 0;
        }
    }
    Ck_Died(pEm);
}

/* RE2 Em20_Move_yori */
void Em20_Move_yori(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pTg;
    VECTOR targ;

    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Spd_x = 30;
        pEm->Spd_z = 0;
        pEm->UnkE2 = 0;
        EM20_W(pEm)->Jump_flg = 0;
        EM20_W(pEm)->House_flg = 0;
        pEm->Unk120 &= 0xFD;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk132 &= 0x3FFF;
        EM20_W(pEm)->Yori_fx = yori_ofs[Rnd_l() & 7];
        EM20_W(pEm)->Yori_fz = yori_ofs[Rnd_l() & 7];
    }
    pTg = Get_target_pl(pEm);
    targ.vx = pTg->Pos_x + EM20_W(pEm)->Yori_fx;
    targ.vy = pTg->Pos_y;
    targ.vz = pTg->Pos_z + EM20_W(pEm)->Yori_fz;
    Root_ck(pEm, &targ, 0, 0);
    pEm->Spd_x = 30;
    pEm->Cdir_y += Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
    Add_speedXZ(pEm, 0);
    if ((LONG)pEm->Unk154 < 3000) {
        pEm->UnkE2++;
        if (G.Unk75E8 & 4)
            pEm->UnkE2 += 20;
        if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pTg->Pos_x, pEm->Cdir_y, 0x80) && (LONG)pEm->Unk154 > 600) {
            pEm->Routine_1 = 3;
            EM_RTN23(pEm) = 0;
        }
    } else {
        if (pEm->UnkE2)
            pEm->UnkE2--;
        if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pTg->Pos_x, pEm->Cdir_y, 0x80) && (G.Unk75E8 & 4))
            pEm->UnkE2 += 20;
    }
    if (pEm->UnkE2 > 64 || ((G.Unk75E8 & 1) && !(pEm->Type & 0x100)) || (G.Unk75EA & 0x20)) {
        pEm->Routine_1 = 2;
        EM_RTN23(pEm) = 0;
        pEm->UnkE2 = 0;
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
    Dog_rot_neck(pEm);
    if (!EM20_W(pEm)->Blood && !(Rnd_l() & 0x3F))
        EM20_W(pEm)->Blood = em20_time_tbl00[Rnd_l() & 7];
    Set_blood(pEm, 4, 6);
    Ck_Floor(pEm);
    Ck_Died(pEm);
}

/* RE2 Em20_Move_run */
void Em20_Move_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_run_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Joint_move(pEm, pKan, pSeq, 0x100);
    Dog_rot_neck(pEm);
    Ck_Floor(pEm);
    Ck_Died(pEm);
}

/* RE2 Em20_Mv_run00 */
void Em20_Mv_run00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    pEm->Routine_2 = 1;
    EM_MOVE(pEm) = 0xF0002;
    pFp->No_hit = 0;
    pFp->Jump_flg = 0;
    pFp->House_flg = 0;
    pEm->Damage_cnt &= 0x7F;
    pEm->Unk120 &= 0xFD;
    pEm->Unk132 &= 0x3FFF;
    pFp->Dash_turn = 0x6C;
    if (Ck_em_id(pEm, NULL, 0x20, 0))
        pFp->Dash_turn = dash_turn_rnd[Rnd_l() & 7];
    if (pFp->Outside) {
        EM_RTN23(pEm) = 3;
        pFp->Area_keep = Rnd_area();
        pFp->No_hit = 1;
        pFp->Stay = 0;
        pEm->UnkDA = 0;
    }
}

/* RE2 Em20_Mv_run10 */
void Em20_Mv_run10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;
    ENEMY_WORK *pTg;
    LONG ck;
    LONG d_tmp = 0;

    pFp = EM20_W(pEm);
    Rate_speed_up(pEm, 40, 240);
    if (!pFp->Fix_cnt) {
        pTg = Get_target_pl(pEm);
        Root_ck(pEm, (VECTOR *)&pTg->Pos_x, 0, 0);
        if (pEm->Status_flg & 0x2000)
            d_tmp = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, pFp->Dash_turn);
        else
            d_tmp = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, pFp->Dash_turn);
        if (pFp->Stay >= 16 && !d_tmp) {
            d_tmp = pFp->Dash_turn;
            pFp->Spin_flg = 1;
            pFp->Stay = 0;
        }
        pEm->Cdir_y += d_tmp;
    } else if (!--pFp->Fix_cnt && !(pEm->Col.Hit_flg & 0x20)) {
        pFp->Fix_cnt = 0;
        pFp->Em_next = 0;
    }
    Add_speedXZ(pEm, 0);
    Set_run_motion(pEm, d_tmp);
    if ((ck = Atck_range_ck(pEm))) {
        if (ck < 0) {
            pFp->Stress += 10;
            if (ck == -2) {
                pEm->Routine_1 = 8;
                EM_RTN23(pEm) = 0;
                return;
            }
            if (ck == -4)
                pFp->Spin_flg = 1;
        } else {
            pEm->Routine_1 = 3;
            EM_RTN23(pEm) = 0;
            return;
        }
    }
    if (pFp->Stress > 450) {
        if (pTg->Life > 12)
            pFp->Stress = 0;
        pFp->Spin_flg = 1;
    }
    if (pFp->Spin_flg) {
        pFp->Spin_flg = 0;
        EM_RTN23(pEm) = 2;
    }
}

/* RE2 Em20_Mv_run20 */
void Em20_Mv_run20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT dir;

    if (!pEm->Routine_3) {
        pEm->Routine_3 = 1;
        pEm->Timer0 = 0;
    }
    Root_ck(pEm, (VECTOR *)&Get_target_pl(pEm)->Pos_x, 0, 0);
    dir = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x60);
    Add_speedXZ(pEm, -pEm->Timer0);
    Set_run_motion(pEm, dir);
    if (pEm->Routine_3 == 1) {
        Rate_speed_down(pEm, 6, 10);
        if (pEm->Spd_x < 11)
            pEm->Routine_3++;
    } else
        Rate_speed_up(pEm, 6, 240);
    pEm->Cdir_y += dir;
    pEm->Timer0 += dir;
    if (__builtin_abs(pEm->Timer0) >= 0x800 || !dir)
        pEm->Routine_2 = 1;
}

/* RE2 Em20_Mv_run30 (RE3 always chases the player) */
void Em20_Mv_run30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;
    ENEMY_WORK *pTg;
    LONG ck, d0, d1;

    pFp = EM20_W(pEm);
    pTg = &Pl_work;
    if (pFp->No_atck)
        pFp->No_atck--;
    Rate_speed_up(pEm, 40, 240);
    ck = Root_ck(pEm, NULL, pFp->Area_keep, 1);
    d0 = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);
    d1 = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x80);
    if (pEm->UnkDA) {
        d0 = pEm->UnkE2;
        pEm->UnkDA--;
    } else if (EM20_W(pEm)->Stay >= 16) {
        pEm->UnkDA = 30;
        if (d0)
            pEm->UnkE2 = -d0;
        else
            pEm->UnkE2 = -(d1 >> 1);
    }
    pEm->Cdir_y += d0;
    Add_speedXZ(pEm, 0);
    Set_run_motion(pEm, d0);
    if (!pEm->Routine_3 || (ck == 1 && SquareRoot0((pEm->Pos_x - pEm->Dest_x) * (pEm->Pos_x - pEm->Dest_x) + (pEm->Pos_z - pEm->Dest_z) * (pEm->Pos_z - pEm->Dest_z)) < 1000)) {
        pEm->Routine_3 = 1;
        ck = pFp->Area_keep;
        pFp->Area_keep = Rnd_area();
        if (ck == pFp->Area_keep)
            pFp->Area_keep = ck + 1;
        pFp->Area_keep %= (SHORT)*G.pRoom->pBlock;
    }
    if (!pFp->No_atck && (Atck_range_ck(pEm) || !d1)) {
        pEm->Routine_1 = 3;
        EM_RTN23(pEm) = 0;
        pFp->No_atck = em20_time_tbl02[Rnd_l() & 7] + 30;
    }
}

/* RE2 Em20_Move_atck */
void Em20_Move_atck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_atck_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Dog_rot_neck(pEm);
    Set_blood(pEm, 4, 13);
}

/* RE2 Em20_Mv_atck00 (RE3 adds hit boxes in front of the dog while it leaps) */
void Em20_Mv_atck00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    HIT_AREA Hit0;
    HIT_AREA Hit1;
    SHORT Box0[4];
    SHORT Box1[4];
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    if (pPl->nFloor != pEm->nFloor) {
        Box0[0] = 1200;
        Box0[1] = 600;
        Box0[2] = 900;
        Box0[3] = 600;
        Box1[0] = 1200;
        Box1[1] = -600;
        Box1[2] = 900;
        Box1[3] = 600;
        Hit0.Id = 0x80;
        Hit1.Id = 0x80;
    } else {
        Box0[0] = 1300;
        Box0[1] = 600;
        Box0[2] = 1200;
        Box0[3] = 600;
        Box1[0] = 1300;
        Box1[1] = -600;
        Box1[2] = 1200;
        Box1[3] = 600;
        Hit0.Id = 0x81;
        Hit1.Id = 0x82;
    }
    Hit0.pPos = (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t;
    Hit0.pBox = Box0;
    Hit0.Dir = pEm->Cdir_y;
    Hit1.pPos = (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t;
    Hit1.pBox = Box1;
    Hit1.Dir = pEm->Cdir_y;
    if (!pEm->Routine_3) {
        pEm->Routine_3 = 1;
        Jump_set(pEm, 0x41, -0x118);
        pEm->Unk120 |= 2;
        if (pEm->Move_no == 12)
            pEm->Cdir_y -= 0x20;
        else if (pEm->Move_no == 13)
            pEm->Cdir_y += 0x20;
    }
    Add_speedXZ(pEm, 0);
    Joint_move(pEm, pKan, pSeq, 0x200);
    if (!--pEm->Timer0) {
        EM_RTN23(pEm) = 1;
        pEm->Spd_x = 0x118;
        EM20_W(pEm)->Jump_flg = 1;
        EM20_W(pEm)->Bang_ok = 1;
        Snd_se_enem(3, pEm);
        G.Unk75EA |= 0x20;
    }
    if (pEm->Timer0 < 6 && __builtin_abs(G.Player.nFloor - pEm->nFloor) < 2) {
        func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Hit0, pPl);
        func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Hit1, pPl);
    }
}

/* RE2 Jump_set */
void Jump_set(ENEMY_WORK *pEm, LONG spd_x, LONG spd_y)
{
    pEm->Spd_x = spd_x;
    pEm->Spd_y = spd_y;
    pEm->Timer0 = 8;
    EM_MOVE(pEm) = 0x70014;
    EM20_W(pEm)->Hit_type = 0;
    EM20_W(pEm)->Blood = 0;
}

/* RE2 Em20_Mv_atck10 */
void Em20_Mv_atck10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;
    LONG ck;

    pFp = EM20_W(pEm);
    ck = Em20_Mv_jump10(pEm, pKan, pSeq);
    if (ck > 0) {
        Pad_vib_set0(5, 0);
        Pad_vib_set1(5, 200, 0);
    } else if (ck < 0) {
        if (pFp->Hit_type == 2) {
            pEm->Routine_1 = 7;
            EM_RTN23(pEm) = 0;
            return;
        }
        EM_RTN23(pEm) = 2;
        EM_MOVE(pEm) = 0xF0015;
        Joint_move(pEm, pKan, pSeq, 0x100);
        pFp->Bang_ok = 0;
        pFp->Stress += 20;
    }
}

/* RE2 Em20_Mv_jump10 */
LONG Em20_Mv_jump10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG d0;

    Add_speedXYZ(pEm, 0, 0);
    pEm->Spd_y += 40;
    d0 = Atck_hit_ck(pEm) == 1;
    if (pEm->Spd_y >= 0 && !d0)
        d0 = -1;
    else if (pEm->Move_cnt)
        Joint_move(pEm, pKan, pSeq, 0x200);
    return d0;
}

/* RE2 Em20_Mv_atck20 */
void Em20_Mv_atck20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (Em20_Mv_jump20(pEm, pKan, pSeq)) {
        pEm->Routine_1 = 2;
        EM_RTN23(pEm) = 0;
        EM20_W(pEm)->Fix_cnt = 0;
        EM20_W(pEm)->Hit_type = 0;
    }
}

/* RE2 Em20_Mv_jump20 */
LONG Em20_Mv_jump20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_3) {
        Add_speedXYZ(pEm, 0, 0);
        pEm->Spd_y += 40;
        if (pEm->Pos_y >= pEm->Unk122) {
            pEm->Routine_3 = 1;
            EM20_W(pEm)->Jump_flg = 0;
            pEm->Pos_y = pEm->Unk122;
            Snd_se_enem(9, pEm);
        }
        if (pEm->Move_cnt != 8)
            Joint_move(pEm, pKan, pSeq, 0x100);
    } else {
        Add_speedXZ(pEm, 0);
        Rate_speed_down(pEm, 0x40, 240);
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            return 1;
    }
    return 0;
}

/* RE2 Em20_Mv_atck30 */
void Em20_Mv_atck30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_MOVE(pEm) = 0xF0016;
        pEm->Timer0 = 0;
        pEm->Spd_x >>= 2;
        pFp->Hit_type = 0;
        pFp->Blood = 4;
    case 1:
        Add_speedXYZ(pEm, 0x800, 0);
        pEm->Spd_y += 40;
        if (pEm->Pos_y >= pEm->Unk122) {
            pEm->Routine_3 = 2;
            EM20_W(pEm)->Jump_flg = 0;
            pEm->Pos_y = pEm->Unk122;
            Snd_se_enem(9, pEm);
        }
        if (pEm->Move_cnt <= 16) {
            Joint_move(pEm, pKan, pSeq, 0x100);
            break;
        }
        pEm->Spd_x = 0;
        break;
    case 2:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_1 = 2;
            EM_RTN23(pEm) = 0;
            pFp->Fix_cnt = em20_time_tbl00[Rnd_l() & 7];
        }
    }
    if (pEm->Timer0 < 0x800) {
        pEm->Cdir_y += 0x80;
        pEm->Timer0 += 0x80;
    }
}

/* RE2 Em20_Move_sleep */
void Em20_Move_sleep(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_sleep_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em20_Mv_sleep00 */
void Em20_Mv_sleep00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl;
    ULONG range;

    pPl = G_PLAYER;
    range = (EM_RTN01(pPl) == 0x201) ? 5000 : 3500;
    if (pEm->Unk154 <= range) {
        pEm->Routine_2 = 2;
        EM_MOVE(pEm) = 0xF0008;
    } else if (((G.Unk75E8 & 1) && !(pEm->Type & 0x100)) || (G.Unk75EA & 0x20)) {
        if (!(G.Unk75EA & 0x80)) {
            pEm->Routine_2 = 2;
            EM_MOVE(pEm) = 0xF0008;
            G.Unk75EA |= 0x80;
        } else {
            pEm->Timer0 = em20_time_tbl02[Rnd_l() & 7];
            pEm->Routine_2 = 1;
        }
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
}

/* RE2 Em20_Mv_sleep10 */
void Em20_Mv_sleep10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!--pEm->Timer0) {
        pEm->Routine_2 = 2;
        EM_MOVE(pEm) = 0xF0008;
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
}

/* RE2 Em20_Mv_sleep20 */
void Em20_Mv_sleep20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (Joint_move(pEm, pKan, pSeq, 0x100)) {
        pEm->Type &= 0xDFFF;
        EM_RTN23(pEm) = 3;
    }
}

/* RE2 Em20_Mv_sleep30 */
void Em20_Mv_sleep30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_3) {
        pEm->Routine_3 = 1;
        pEm->Timer0 = em20_time_tbl01[Rnd_l() & 7];
        EM_MOVE(pEm) = 0x1F0000;
    }
    if (!--pEm->Timer0) {
        EM_MOVE(pEm) = 0xF0001;
        pEm->Routine_1 = 1;
        EM_RTN23(pEm) = 0;
    }
    Joint_move(pEm, pKan, pSeq, 0x80);
    Dog_rot_neck(pEm);
}

/* RE2 Em20_Move_ikaku */
void Em20_Move_ikaku(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = 0x30003;
        EM20_W(pEm)->Blood = 0x20;
        pEm->Unk120 &= 0xFD;
    }
    if (Joint_move(pEm, pKan, pSeq, 0x400)) {
        if (pEm->Move_no != 4) {
            EM_MOVE(pEm) = pEm->Move_no + 0x30001;
            if (pEm->Move_no == 4)
                Snd_se_enem(4, pEm);
        } else
            EM_RTN(pEm) = 0x201;
    }
    Dog_rot_neck(pEm);
    Set_blood(pEm, 4, 6);
}

/* RE2 Em20_Move_back */
void Em20_Move_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pTg;

    pTg = &Pl_work;
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Spd_x = 50;
        EM_MOVE(pEm) = 0x30009;
        pEm->Unk120 &= 0xFD;
    }
    if (pEm->Move_cnt < 15) {
        if (pEm->Move_cnt >= 8)
            Rate_speed_down(pEm, 32, 30);
        Add_speedXZ(pEm, 0x800);
        Goto00(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
    }
    if (Joint_move(pEm, pKan, pSeq, 0x400))
        EM_RTN(pEm) = 0x201;
    Dog_rot_neck(pEm);
}

/* RE2 Em20_Move_critical */
void Em20_Move_critical(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2)
        Em20_Mv_critical00(pEm, pKan, pSeq);
    else
        Em20_Mv_critical10(pEm, pKan, pSeq);
}

/* RE2 Em20_Mv_critical00: the bite that kills the player */
void Em20_Mv_critical00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl;
    EM20_FREE *pFp;

    pPl = &Pl_work;
    pFp = EM20_W(pEm);
    if (!pEm->Routine_3) {
        pEm->Routine_3 = 1;
        pFp->Em_next = 0;
        pEm->Spd_x = 0;
        pEm->UnkE2 = 12;
        EM_MOVE(pEm) = 0x1F0017;
        Joint_move(pEm, pKan, pSeq, 0x80);
        pFp->Blood = 0;
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        EM_RTN(pPl) = 6;
        pPl->Cdir_y = pEm->Cdir_y + 0x800;
        Pad_vib_fade_set(0x1E, 0x32, 0x96, 0);
        Pad_vib_set1(8, 0xFA, 0x1E);
        Pad_vib_set0(5, 0x32);
        Pad_vib_set1(0x3C, 0x78, 0x32);
        Pad_vib_set0(5, 0x46);
        Pad_vib_set0(5, 0x55);
        Pad_vib_set0(0x14, 0x64);
        Pad_vib_fade_set(8, 0x64, 0xFA, 0x71);
        Pad_vib_fade_set(0x3C, 0xFA, 0, 0x79);
    } else {
        Set_base_pos(pPl, pKan, pSeq, 0);
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_RTN23(pEm) = 1;
            EM_MOVE(pEm) = 0x1F0018;
            Spd_get(pEm, pKan, pSeq, 0);
            Joint_move(pEm, pKan, pSeq, 0x80);
            EM20_W(pEm)->Blood = 0;
            return;
        }
        if (pEm->Move_cnt == 0x62)
            pFp->Blood = 0x12;
    }
    if (!pEm->UnkE2 || (pPl->Id & 1)) {
        if (!pFp->Em_next) {
            pFp->Em_next = 1;
            G.System_flg |= 0x20800;
            G.Unk75EA |= 0x40;
        }
        Add_speedXYZ(pEm, 0, 0);
        pEm->Spd_y += 40;
        if (pEm->Pos_y >= pEm->Unk122) {
            pEm->Pos_y = pEm->Unk122;
            pEm->Spd_y = 0;
            EM20_W(pEm)->Jump_flg = 0;
        }
        if (pPl->Id & 1)
            pEm->UnkE2--;
    } else
        pEm->UnkE2--;
    Set_blood(pEm, 4, 13);
    Set_blood(pEm, 8, 4);
    Set_blood(pEm, 8, 4);
}

/* RE2 Em20_Mv_critical10 */
void Em20_Mv_critical10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG eos;

    if (!pEm->Routine_3) {
        Spd_get(pEm, pKan, pSeq, 0);
        eos = Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (eos) {
            pEm->Routine_3 = 1;
            pEm->Timer0 = 2;
            EM_MOVE(pEm) = 0x1F0019;
            Spd_get(pEm, pKan, pSeq, 0);
            Joint_move(pEm, pKan, pSeq, 0x80);
        }
    } else {
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80) && !--pEm->Timer0) {
            pEm->Routine_1 = 12;
            EM_RTN23(pEm) = 0;
        }
    }
}

/* RE2 Em20_Move_escape */
void Em20_Move_escape(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;
    SHORT d_tmp;

    pFp = EM20_W(pEm);
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pFp->Area_keep = Rnd_area();
        pEm->Timer0 = em20_time_tbl01[Rnd_l() & 7] + 30;
        pFp->Stress += 300;
    }
    Get_target_pl(pEm);
    Root_ck(pEm, NULL, pFp->Area_keep, 1);
    d_tmp = Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, pFp->Dash_turn);
    pEm->Cdir_y += d_tmp;
    Add_speedXZ(pEm, 0);
    if (!--pEm->Timer0) {
        pEm->Routine_1 = 2;
        EM_RTN23(pEm) = 0;
        pFp->Fix_cnt = 0;
    }
    Set_run_motion(pEm, d_tmp);
    Joint_move(pEm, pKan, pSeq, 0x100);
    Dog_rot_neck(pEm);
    Ck_Floor(pEm);
}

/* RE2 Em20_Move_floorout */
void Em20_Move_floorout(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Timer0 = 60;
        EM_MOVE(pEm) = 0xF0000;
    }
    if (!--pEm->Timer0) {
        pEm->Routine_1 = 0;
        EM_RTN23(pEm) = 0;
        EM_MOVE(pEm) = 0xF0001;
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
    Dog_rot_neck(pEm);
}

/* RE2 Em20_Move_walk2 */
void Em20_Move_walk2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em20_rndwalk(pEm, pKan, pSeq);
    if ((G.Player.nFloor == pEm->nFloor || (pEm->Status_flg & 0x2000))
        && ((LONG)pEm->Unk154 < 4000 || (G.Unk75EA & 0x20) || ((G.Unk75E8 & 1) && !(pEm->Type & 0x100)))) {
        pEm->Routine_1 = 2;
        EM_RTN23(pEm) = 0;
    }
}

/* RE2 Em20_Move_died: walks up to the dead player */
void Em20_Move_died(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pTg;

    pTg = G.pEnemy[1];
    if ((LONG)G.Status_flg >= 0 || pTg->Life)
        pTg = G_PLAYER;
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Spd_x = 30;
        pEm->Spd_z = 0;
        EM20_W(pEm)->Jump_flg = 0;
        EM20_W(pEm)->House_flg = 0;
        pEm->Unk120 &= 0xFD;
        EM_MOVE(pEm) = ((Rnd_l() & 0x3F) << 8) + 0xF0001;
    case 1:
        Root_ck(pEm, (VECTOR *)&pTg->Pos_x, 0, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Add_speedXZ(pEm, 0);
        if ((!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pTg->Pos_x, pEm->Cdir_y, 0x100) && (LONG)pEm->Unk154 < 2500) || pEm->Col.Unk3 != 0xFF) {
            pEm->Routine_2 = 2;
            EM_MOVE(pEm) = 0xF0000;
        }
    case 2:
        break;
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
    Dog_rot_neck(pEm);
}

/* RE2 Em20_Move_eat */
void Em20_Move_eat(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2)
        Em20_mv_eat00(pEm, pKan, pSeq);
    else
        Em20_Mv_sleep30(pEm, pKan, pSeq);
}

/* RE2 Em20_mv_eat00 */
void Em20_mv_eat00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl;
    ULONG range;

    pPl = G_PLAYER;
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM20_W(pEm)->Jump_flg = 0;
        EM20_W(pEm)->House_flg = 0;
        EM_MOVE(pEm) = ((Rnd_l() & 0x3F) << 8) + 0x1F000E;
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    case 1:
        if (G.Player.Life >= 0) {
            range = (EM_RTN01(pPl) == 0x201) ? 5000 : 3500;
            if (pEm->Unk154 <= range)
                EM_RTN23(pEm) = 1;
            else if ((G.Unk75E8 & 1) && !(pEm->Type & 0x100)) {
                if (!(G.Unk75EA & 0x80)) {
                    EM_RTN23(pEm) = 1;
                    G.Unk75EA |= 0x80;
                } else {
                    pEm->Routine_3 = 2;
                    pEm->Timer0 = em20_time_tbl02[Rnd_l() & 7];
                }
            }
        }
        break;
    case 2:
        if (!--pEm->Timer0)
            EM_RTN23(pEm) = 1;
    }
    if (*pEm->pNow_seq & 0x200) {
        EM20_W(pEm)->Blood = 1;
        Set_blood(pEm, 4, 13);
        if (!(Rnd_l() & 3)) {
            EM20_W(pEm)->Blood = 1;
            Set_blood(pEm, 4, (Rnd_l() & 1) + 1);
        }
        Snd_se_enem(6, pEm);
    }
    Spd_get(pEm, pKan, pSeq, 0);
    Joint_move(pEm, pKan, pSeq, 0x80);
    Add_speedXZ(pEm, 0);
}

/* RE2 Em20_Move_house */
void Em20_Move_house(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2)
        Em20_mv_house00(pEm, pKan, pSeq);
    else
        Em20_mv_house10(pEm, pKan, pSeq);
    Dog_rot_neck(pEm);
}

/* RE2 Em20_mv_house00 */
void Em20_mv_house00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Unk120 &= 0xFD;
        EM_MOVE(pEm) = house_mot_rnd[Rnd_l() & 7] + 0xF0000;
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->UnkE2 = (Rnd_l() & 3) + 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            if (!--pEm->UnkE2)
                EM_RTN23(pEm) = 1;
            else
                EM_MOVE(pEm) = house_mot_rnd[Rnd_l() & 7] + 0xF0000;
        }
        if (pEm->Move_no == 11)
            Add_speedXZ(pEm, 0);
    }
}

/* RE2 Em20_mv_house10 */
void Em20_mv_house10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Spd_x = 240;
        pEm->Timer0 = em20_time_tbl01[Rnd_l() & 7];
        pEm->UnkDA = (Rnd_l() & 1) ? 0x7F : -0x7F;
        if (Rnd_l() & 1)
            pEm->UnkDA = -pEm->UnkDA;
        EM_MOVE(pEm) = 0xF0002;
    case 1:
        pEm->Cdir_y += pEm->UnkDA;
        if (!--pEm->Timer0)
            pEm->Routine_3 = 2;
        break;
    case 2:
        pEm->Cdir_y += pEm->UnkDA;
        pEm->Cdir_y &= 0xFFF;
        if (pEm->Cdir_y >= 0x780 && pEm->Cdir_y <= 0x880) {
            if ((0x1B68 >> (Rnd_l() & 0xF)) & 1) {
                pEm->Routine_1 = 15;
                EM_RTN23(pEm) = 0x100;
            } else {
                pEm->Routine_3 = 3;
                pEm->UnkDA = 0;
            }
        }
        break;
    case 3:
        if (pEm->Col.Hit_flg & 0x20)
            EM_RTN23(pEm) = 0;
    }
    Add_speedXZ(pEm, 0);
    Set_run_motion(pEm, pEm->UnkDA);
    Joint_move(pEm, pKan, pSeq, 0x100);
}

/* RE2 Em20_Move_rush */
void Em20_Move_rush(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_rush_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Dog_rot_neck(pEm);
}

/* RE2 Em20_Mv_rush00 */
void Em20_Mv_rush00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_3) {
    case 0:
        EM20_W(pEm)->Jump_flg = 0;
        EM20_W(pEm)->House_flg = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 2;
        break;
    case 1:
        pEm->Routine_3 = 2;
        Jump_set(pEm, 0x41, -0x140);
        pEm->Unk120 &= 0xFD;
    case 2:
        Add_speedXZ(pEm, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!--pEm->Timer0) {
            EM_RTN23(pEm) = 1;
            pEm->Spd_x = 0x118;
            EM20_W(pEm)->Jump_flg = 1;
            Snd_se_enem(3, pEm);
        }
    }
}

/* RE2 Em20_Mv_rush10 */
void Em20_Mv_rush10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_W(pEm)->No_hit = 1;
    if (Em20_Mv_jump10(pEm, pKan, pSeq) < 0) {
        EM_RTN23(pEm) = 2;
        EM_MOVE(pEm) = 0xF0015;
        Joint_move(pEm, pKan, pSeq, 0x100);
    }
}

/* RE2 Em20_Mv_rush20 */
void Em20_Mv_rush20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    pEm->Damage_cnt &= 0x7F;
    if (Em20_Mv_jump20(pEm, pKan, pSeq)) {
        pEm->Routine_1 = 2;
        EM_RTN23(pEm) = 0;
        EM20_W(pEm)->Fix_cnt = 0;
        EM20_W(pEm)->No_hit = 0;
    }
}

/* RE2 Em20_Move_bodyatck */
void Em20_Move_bodyatck(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_bodyatck_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Dog_rot_neck(pEm);
}

/* RE2 Em20_Mv_bodyatck10 */
void Em20_Mv_bodyatck10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_W(pEm)->No_hit = 1;
    if (Em20_Mv_jump10(pEm, pKan, pSeq) < 0) {
        EM_RTN23(pEm) = 2;
        EM_MOVE(pEm) = 0xF0015;
        Joint_move(pEm, pKan, pSeq, 0x100);
    }
    if (pEm->Col.Hit_flg & 0x20) {
        EM_RTN23(pEm) = 3;
        EM_MOVE(pEm) = 0xF0016;
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Spd_x = -(pEm->Spd_x >> 1);
        pEm->Timer0 = 0;
        pEm->Add_dir = 1;
    }
}

/* RE2 Em20_Mv_bodyatck20 */
void Em20_Mv_bodyatck20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (Em20_Mv_jump20(pEm, pKan, pSeq)) {
        pEm->Routine_1 = 13;
        EM_RTN23(pEm) = 1;
        pEm->Damage_cnt &= 0x7F;
    }
}

/* RE2 Em20_Mv_bodyatck30 */
void Em20_Mv_bodyatck30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_3) {
        Add_speedXYZ(pEm, 0x800, 0);
        pEm->Spd_y += 40;
        if (pEm->Pos_y >= pEm->Unk122) {
            pEm->Routine_3 = 1;
            EM20_W(pEm)->Jump_flg = 0;
            pEm->Pos_y = pEm->Unk122;
            Snd_se_enem(9, pEm);
        }
        if (pEm->Move_cnt <= 16)
            Joint_move(pEm, pKan, pSeq, 0x100);
        else
            pEm->Spd_x = 0;
    } else if (Joint_move(pEm, pKan, pSeq, 0x100)) {
        pEm->Routine_1 = 13;
        EM_RTN23(pEm) = 1;
        pEm->Damage_cnt &= 0x7F;
    }
    if (pEm->Timer0 < 0x400) {
        pEm->Cdir_y += 0x100;
        pEm->Timer0 += 0x100;
    }
}

/* RE2 Em20_Move_jumpin */
void Em20_Move_jumpin(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_jumpin_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em20_Mv_jumpin00 */
void Em20_Mv_jumpin00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->Routine_3 & 0xF)) {
        pEm->Routine_3 = 1;
        Jump_set(pEm, 0x41, -0x140);
        EM20_W(pEm)->Outside = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 &= 0xFD;
        pEm->Be_flg |= 0x2000;
    }
    Add_speedXZ(pEm, 0);
    Joint_move(pEm, pKan, pSeq, 0x200);
    if (!--pEm->Timer0) {
        EM_RTN23(pEm) = 1;
        pEm->Spd_x = 0x118;
        EM20_W(pEm)->Jump_flg = 1;
        Snd_se_enem(3, pEm);
    }
}

/* RE2 Em20_Mv_jumpin20 */
void Em20_Mv_jumpin20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Pos_y >= pEm->Unk122)
        pEm->Be_flg &= ~0x2000;
    if (Em20_Mv_jump20(pEm, pKan, pSeq)) {
        EM_RTN(pEm) = 0x201;
        EM20_W(pEm)->Fix_cnt = 0;
        EM20_W(pEm)->No_hit = 0;
        pEm->Type &= 0xBFFF;
        pEm->Damage_cnt &= 0x7F;
        pEm->nFloor = G.Player.nFloor;
    }
}

#include "ovl/shared/em20/Ck_em_id.inc.c"

/* RE2 Em20_Damage (RE3 indexes the damage tables by the damage kind Unk12C) */
void Em20_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(EM20_W(pEm)->Combo_flg & 0x80))
        em20_dmg_tbl[pEm->Unk12C - 1](pEm, pKan, pSeq);
    else
        Em20_Dmg_combo(pEm, pKan, pSeq);
}

/* RE2 Em20_Dmg_nml */
void Em20_Dmg_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    dmg_nml_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em20_Dmg_nml00 */
void Em20_Dmg_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    pEm->Timer0 = (G.Player.Cdir_y & 0xFFF) - (pEm->Cdir_y & 0xFFF);
    pEm->Timer0 += dir_hosei[Rnd_l() & 7];
    pEm->Unk120 |= 2;
    pEm->Type &= 0xDFFF;
    if (pEm->Routine_0 == 2)
        Snd_se_enem(1, pEm);
    if (pFp->Combo_flg && !pFp->House_flg) {
        pFp->Combo_flg |= 0x80;
        Em20_Dmg_combo00(pEm, pKan, pSeq);
        return;
    }
    if (pFp->Jump_flg) {
        EM_RTN23(pEm) = 1;
        pEm->Spd_x >>= 1;
    } else {
        EM_RTN23(pEm) = 2;
        pEm->Spd_x = 240;
        Set_dmghit(pEm, 0);
    }
    pFp->Em_next = 0;
    EM_MOVE(pEm) = 0x70011;
    EM20_W(pEm)->Combo_flg = 1;
    pEm->UnkDA = 0x200;
    Joint_move(pEm, pKan, pSeq, 0x200);
    pFp->Blood = 1;
    Set_blood(pEm, 0, 0);
}

/* RE2 Em20_Dmg_nml10 */
void Em20_Dmg_nml10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    Add_speedXYZ(pEm, pEm->Timer0, 0);
    pEm->Spd_y += 40;
    if (pEm->Pos_y >= pEm->Unk122) {
        pEm->Pos_y = pEm->Unk122;
        pEm->UnkDA = 0x200;
        pFp->Jump_flg = 0;
        pFp->Em_next = 1;
        Set_dmghit(pEm, 0);
        if (((0x404 >> (Rnd_l() & 0xF)) & 1) && !(pFp->Combo_flg & 0x80) && pEm->Life >= 0 && !pFp->House_flg) {
            Snd_se_enem(9, pEm);
            EM_RTN23(pEm) = 3;
        } else {
            Snd_se_enem(10, pEm);
            EM_RTN23(pEm) = 2;
        }
    }
    if (pEm->Move_cnt != 3)
        Joint_move(pEm, pKan, pSeq, 0x200);
}

/* RE2 Em20_Dmg_nml20 */
void Em20_Dmg_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    if (!EM20_W(pEm)->Em_next) {
        Add_speedXZ(pEm, pEm->Timer0);
        Rate_speed_down(pEm, 10, 0);
    }
    if (Joint_move(pEm, pKan, pSeq, pEm->UnkDA)) {
        switch (pEm->Routine_3) {
        case 0:
            pEm->Routine_3 = 1;
            EM20_W(pEm)->Combo_flg &= 0xC0;
            EM20_W(pEm)->Combo_flg |= 2;
            EM_MOVE(pEm) = (pEm->UnkCA << 16) + 0x12;
            break;
        case 1:
            pEm->Routine_3 = 2;
            EM_MOVE(pEm) = 0xF0007;
            pEm->UnkDA = 0x100;
            break;
        case 2:
            Set_dmghit(pEm, 1);
            pEm->Type &= 0xDFFF;
            EM20_W(pEm)->Combo_flg = 0;
            pEm->Damage_cnt &= 0x7F;
            if (EM20_W(pEm)->House_flg)
                EM_RTN(pEm) = 0x10D01;
            else if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200))
                EM_RTN(pEm) = 0x501;
            else
                EM_RTN(pEm) = 0x601;
        }
    }
}

/* RE2 Em20_Dmg_nml30 */
void Em20_Dmg_nml30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_3) {
        pEm->Routine_3 = 1;
        EM_MOVE(pEm) = 0x30F16;
        pEm->Damage_cnt &= 0x7F;
    }
    if (Joint_move(pEm, pKan, pSeq, 0x400)) {
        Set_dmghit(pEm, 1);
        EM20_W(pEm)->Combo_flg = 0;
        EM_RTN(pEm) = 0x201;
    }
}

/* RE2 Em20_Dmg_nml40 */
void Em20_Dmg_nml40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_3) {
        pEm->Routine_3 = 1;
        pEm->Unk132 |= 0xC000;
        pEm->Damage_cnt |= 0x80;
    }
}

/* RE2 Em20_Dmg_combo */
void Em20_Dmg_combo(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    dmg_combo_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Set_blood(pEm, Rnd_l() & 0xF, 0);
    Set_blood(pEm, Rnd_l() & 0xF, 0);
    Set_blood(pEm, Rnd_l() & 0xF, 5);
}

/* RE2 Em20_Dmg_combo00 */
void Em20_Dmg_combo00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    if (pEm->Unk12C == 14 || pEm->Unk12C == 15) {
        pFp->Blood += 3;
        if (!(Rnd_l() & 3)) {
            pFp->Blood++;
            Set_blood(pEm, Rnd_l() & 0xF, (Rnd_l() & 1) | 2);
        }
    } else {
        pFp->Blood = 1;
        Set_blood(pEm, 0, 0);
        Snd_se_enem(1, pEm);
    }
    if ((pFp->Combo_flg & 0x7F) == 1) {
        if (pFp->Jump_flg)
            EM_RTN23(pEm) = 1;
        else
            EM_RTN23(pEm) = 2;
    } else {
        EM_RTN23(pEm) = 0x103;
        if (!(pFp->Combo_flg & 0x40)) {
            EM_MOVE(pEm) = (pEm->UnkCA << 16) + 0x12;
            Snd_se_enem(1, pEm);
            pFp->Combo_flg |= 0x40;
        }
        pEm->UnkE2 = 0;
        pFp->Em_next = 1;
    }
    pEm->UnkDA = 0;
}

/* RE2 Em20_Dmg_combo10 */
void Em20_Dmg_combo10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_3) {
        pEm->Routine_3 = 1;
        Add_speedXZ3(pEm, pEm->Timer0 + (Rnd_l() << 1) - 0x100, 0);
        pEm->Cdir_y += (Rnd_l() & 0xFF) - 0x80;
    }
    Em20_Dmg_nml10(pEm, pKan, pSeq);
}

/* RE2 Em20_Dmg_combo20 */
void Em20_Dmg_combo20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    if (!pFp->Em_next) {
        Add_speedXZ(pEm, pEm->Timer0);
        Rate_speed_down(pEm, 10, 0);
        if (!pEm->Routine_3) {
            pEm->Routine_3 = 1;
            pEm->Cdir_y += (Rnd_l() & 0xFF) - 0x80;
        }
    }
    if (Joint_move(pEm, pKan, pSeq, pEm->UnkDA)) {
        EM_RTN23(pEm) = 3;
        pFp->Combo_flg &= 0xC0;
        pFp->Combo_flg |= 2;
        pEm->UnkE2 = 0;
        EM_MOVE(pEm) = (pEm->UnkCA << 16) + 0x12;
    }
}

/* RE2 Em20_Dmg_combo30 */
void Em20_Dmg_combo30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->UnkE2) {
        pEm->UnkE2 = 1;
        pEm->Spd_x = 40;
        Add_speedXZ(pEm, pEm->Timer0);
        pEm->Cdir_y += (Rnd_l() & 0xFF) - 0x80;
    }
    Em20_Dmg_nml20(pEm, pKan, pSeq);
    if (pEm->Routine_3 >= 2)
        EM20_W(pEm)->Combo_flg &= 0xBF;
}

/* RE2 Em20_Dmg_gnml */
void Em20_Dmg_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG i;

    EM20_W(pEm)->Blood = 2 - (pEm->Unk132 & 3) / 3;
    for (i = 0; i < EM20_W(pEm)->Blood; i++)
        Set_blood(pEm, Rnd_l() & 0xF, 8);
    pEm->Unk12C = 1;
    Em20_Dmg_nml(pEm, pKan, pSeq);
}

/* RE2 Em20_Dmg_fire */
void Em20_Dmg_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    ULONG i;

    if (!(pEm->Status_flg & 1)) {
        EM20_W(pEm)->Blood = 4 - (pEm->Unk132 & 3) / 3;
        for (i = 0; i < EM20_W(pEm)->Blood; i++)
            Set_blood(pEm, Rnd_l() & 0xF, 8);
        pEm->Damage_cnt |= 0x80;
        pEm->Status_flg |= 1;
    }
    for (i = 0; i < 17; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
    }
    pEm->Unk12C = 1;
    Em20_Dmg_nml(pEm, pKan, pSeq);
}

/* RE2 Em20_Dmg_H2SO4 */
void Em20_Dmg_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG d0;

    d0 = Rnd_l() & 0xF;
    pEm->pSin_parts_ptr[d0].Poly_rgb = 0x3F2F;
    EM20_W(pEm)->Blood = 1;
    Set_blood(pEm, d0, 9);
    pEm->Unk12C = 1;
    Em20_Dmg_nml(pEm, pKan, pSeq);
}

/* RE3 only: damage kind 5, marks seven parts (UnkA2 0x40) and spins the light colour (Unk178) */
void func_80103F54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    if (!(pEm->Status_flg & 2)) {
        pEm->Be_flg |= 8;
        if (Rnd_l() & 1) {
            pEm->Unk178[0] = 0x1000;
            pEm->Unk178[1] = 0;
            pEm->Unk178[2] = 0x1000;
        } else {
            pEm->Unk178[0] = -0x1000;
            pEm->Unk178[1] = 0;
            pEm->Unk178[2] = -0x1000;
        }
        pEm->Unk17E = 0x40;
        pP = pEm->pSin_parts_ptr;
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[1];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[2];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[3];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[10];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[11];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[14];
        pP->UnkA2 |= 0x40;
        EM20_W(pEm)->Blood = 2;
        pEm->Damage_cnt |= 0x80;
        Set_blood(pEm, Rnd_l() & 7, 14);
        Set_blood(pEm, (Rnd_l() & 7) | 8, 14);
        pEm->Status_flg |= 2;
    }
    pEm->Unk12C = 1;
    Em20_Dmg_nml(pEm, pKan, pSeq);
}

/* RE2 Em20_Dmg_spark (no table refers to it in RE3) */
void Em20_Dmg_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        Em20_Dmg_nml00(pEm, pKan, pSeq);
        EM20_W(pEm)->Blood = 8;
        Set_blood(pEm, Rnd_l() & 0xF, 11);
        pEm->UnkE3 = 0;
    }
    pEm->Unk12C = 1;
    Em20_Dmg_nml(pEm, pKan, pSeq);
    if (!(++pEm->UnkE3 % 3)) {
        Set_blood(pEm, Rnd_l() & 0xF, 11);
        Set_blood(pEm, Rnd_l() & 0xF, 11);
    }
}

/* RE2 Em20_Dmg_nml2 */
void Em20_Dmg_nml2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    dmg_nml2_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em20_Dmg_nml2_00 */
void Em20_Dmg_nml2_00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->Unk132 & 0x8000) && !EM20_W(pEm)->Jump_flg) {
        EM20_W(pEm)->Jump_flg = 1;
        pEm->Spd_y = -0x8C;
        pEm->Spd_x = 0xF0;
    }
    Em20_Dmg_nml00(pEm, pKan, pSeq);
}

/* RE2 Set_dmghit: hit box height while down / standing */
void Set_dmghit(ENEMY_WORK *pEm, ULONG Type)
{
    if (!Type) {
        pEm->Col.H = 1000;
        pEm->Unk136 = -500;
        pEm->Unk134 = 1700;
        pEm->Unk132 &= 0xBFFF;
    } else {
        pEm->Col.H = 2000;
        pEm->Unk136 = -1000;
        pEm->Unk134 = 1200;
        pEm->Unk132 &= 0xBFFF;
    }
}

/* RE2 Em20_Die */
void Em20_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    em20_die_tbl[pEm->Unk12C - 1](pEm, pKan, pSeq);
}

/* RE2 Em20_Die_nml */
void Em20_Die_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2)
        die_nml_init[pEm->Unk12C - 1](pEm, pKan, pSeq);
    else
        die_nml_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em20_Die_nml00 */
void Em20_Die_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG tmp;

    Em_dead_flg_on_cand(pEm);
    tmp = EM20_W(pEm)->Combo_flg & 0x3F;
    EM20_W(pEm)->Combo_flg = 0;
    Em20_Dmg_nml00(pEm, pKan, pSeq);
    if (!EM20_W(pEm)->No_voice)
        Snd_se_enem(7, pEm);
    if (tmp == 2)
        pEm->Spd_x = 0;
}

/* RE2 Em20_Die_nml20 */
void Em20_Die_nml20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!EM20_W(pEm)->Em_next) {
        Add_speedXZ(pEm, pEm->Timer0);
        Rate_speed_down(pEm, 10, 0);
    }
    if (Joint_move(pEm, pKan, pSeq, pEm->UnkDA)) {
        if (!pEm->Routine_3) {
            pEm->Routine_3 = 1;
            EM_MOVE(pEm) = (pEm->UnkCA << 16) + 0x12;
        } else
            EM_RTN(pEm) = 7;
    }
}

/* RE2 Em20_Die_bang */
void Em20_Die_bang(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    die_bang_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Set_blood(pEm, 3, 0);
    Set_blood(pEm, 2, 0);
    if (!(G.Status_flg & 0x8000))
        Set_blood(pEm, 2, (Rnd_l() & 1) + 1);
}

/* RE2 Em20_Die_bang00 */
void Em20_Die_bang00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    if (!pFp->Jump_flg || !pFp->Bang_ok || !(Rnd_l() & 5) || (pEm->Unk132 & 0xC)) {
        pEm->Unk12C = 1;
        Em20_Die_nml00(pEm, pKan, pSeq);
    } else {
        Em_dead_flg_on_cand(pEm);
        EM_RTN23(pEm) = 1;
        if (!(G.Status_flg & 0x8000))
            Set_head_bomb(pEm);
        if (!EM20_W(pEm)->Target_pl)
            Em20_Pl_to_dmg(0, Cdir_ck(pEm, G_PLAYER));
        EM20_W(pEm)->Blood = 0x12;
        EM_MOVE(pEm) = 0x70011;
        Joint_move(pEm, pKan, pSeq, 0x200);
    }
}

/* RE2 Set_head_bomb */
void Set_head_bomb(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pEm->Unk120 |= 1;
    if (pEm->Type & 0x80)
        func_8001B894(5, &pEm->pSin_parts_ptr[7].Workm);
    for (i = 0; i < 7; i++) {
        pP = &pEm->pSin_parts_ptr[bang_parts_no[i]];
        pP->UnkA2 |= 0x20;
        pP->Be_flg |= 2;
        PW_FREE(pP)[0] = pEm->Cdir_y;
        PW_FREE(pP)[4] = 0;
        PW_FREE(pP)[2] = 800;
        PW_FREE(pP)[1] = -150;
        PW_FREE(pP)[3] = 10;
        PW_FREE(pP)[6] = -100;
        pP->Poly_rgb = 0x101040;
    }
}

/* RE3 only: the tail parts fly off spinning (Em20_Die_bang2) */
void Set_tail_bomb(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    for (i = 0; i < 6; i++) {
        pP = &pEm->pSin_parts_ptr[tail_parts_no[i]];
        pP->Poly_rgb = 0x101040;
        pP->Be_flg |= 0xA;
        pP->Old_x = pEm->Cdir_y;
        pP->Unk96 = 0;
        pP->Old_z = 50;
        pP->Old_y = 0;
        pP->Unk85 = 20;
        pP->Unk90 = 0;
        pP->Unk92 = pEm->Cdir_y;
        pP->Unk8E = 0x100;
    }
}

/* RE2 Em20_Die_bang10 */
void Em20_Die_bang10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM20_FREE *pFp;

    pFp = EM20_W(pEm);
    Add_speedXYZ(pEm, 0, 0);
    if (pFp->Jump_flg) {
        if (!pEm->Routine_3 && (LONG)pEm->Unk154 < 1400) {
            pEm->Spd_x = 0;
            pEm->Routine_3 = 1;
        }
        pEm->Spd_y += 40;
        if (pEm->Pos_y >= pEm->Unk122) {
            pEm->Pos_y = pEm->Unk122;
            pEm->Spd_y = 0;
            pFp->Jump_flg = 0;
            pEm->Routine_3 = 0;
        }
        if (pEm->Move_cnt != 3)
            Joint_move(pEm, pKan, pSeq, 0x200);
    } else if (Joint_move(pEm, pKan, pSeq, 0x200)) {
        if (!pEm->Routine_3) {
            pEm->Routine_3 = 1;
            EM_MOVE(pEm) = (pEm->UnkCA << 16) + 0x12;
        } else
            EM_RTN(pEm) = 7;
    }
}

/* RE2 Em20_Die_gnml */
void Em20_Die_gnml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    die_gnml_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Set_blood(pEm, 3, 0);
    Set_blood(pEm, 2, 0);
    Set_blood(pEm, 2, (Rnd_l() & 1) + 1);
}

/* RE2 Em20_Die_gnml00 */
void Em20_Die_gnml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    ULONG d0;

    if (pEm->Unk12C == 6 && (pEm->Unk132 & 0xC)) {
        Em20_Die_nml00(pEm, pKan, pSeq);
        return;
    }
    EM20_W(pEm)->No_voice = 1;
    Em20_Die_nml00(pEm, pKan, pSeq);
    Set_head_bomb(pEm);
    if (pEm->Unk12C == 6) {
        EM20_W(pEm)->Blood = 1;
        d0 = Rnd_l() & 0xF;
        pP = &pEm->pSin_parts_ptr[d0];
        if (!(pP->UnkA2 & 0x20))
            Set_blood(pEm, d0, 7);
    }
    EM20_W(pEm)->Blood = 0x12;
}

/* RE2 Em20_Die_fire */
void Em20_Die_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    ULONG i;

    Em20_Die_nml00(pEm, pKan, pSeq);
    EM20_W(pEm)->Blood = 6;
    for (i = 0; i < 6; i++)
        Set_blood(pEm, Rnd_l() & 0xF, 7);
    for (i = 0; i < 17; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
        pP->Poly_rgb = 0x202020;
    }
}

/* RE2 Em20_Die_H2SO4 */
void Em20_Die_H2SO4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    ULONG i;

    Em20_Die_nml00(pEm, pKan, pSeq);
    if (pEm->Unk132 & 0xC)
        return;
    for (i = 0; i < 17; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->Poly_rgb = 0x3F2F;
    }
    EM20_W(pEm)->Blood = 2;
    Set_blood(pEm, Rnd_l() & 7, 9);
    Set_blood(pEm, (Rnd_l() & 7) | 8, 10);
}

/* RE3 only: death of damage kind 9 (cf. func_80103F54) */
void func_80104B78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    Em20_Die_nml00(pEm, pKan, pSeq);
    pEm->Unk17E = 0x40;
    pEm->Be_flg |= 8;
    pP = pEm->pSin_parts_ptr;
    pP->UnkA2 |= 0x80;
    pP = &pEm->pSin_parts_ptr[1];
    pP->UnkA2 |= 0x80;
    pP = &pEm->pSin_parts_ptr[10];
    pP->UnkA2 |= 0x80;
    pP = &pEm->pSin_parts_ptr[11];
    pP->UnkA2 |= 0x80;
    pP = &pEm->pSin_parts_ptr[14];
    pP->UnkA2 |= 0x80;
    EM20_W(pEm)->Blood = 2;
    Set_blood(pEm, Rnd_l() & 7, 9);
    Set_blood(pEm, (Rnd_l() & 7) | 8, 10);
}

/* RE2 Em20_Die_spark */
void Em20_Die_spark(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    die_spark_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    if (++pEm->UnkE3 & 1) {
        Set_blood(pEm, Rnd_l() & 0xF, 11);
        Set_blood(pEm, Rnd_l() & 0xF, 11);
    }
}

/* RE2 Em20_Die_spark00 */
void Em20_Die_spark00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    ULONG i;

    Em20_Die_nml00(pEm, pKan, pSeq);
    for (i = 0; i < 17; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->Poly_rgb = 0x3F203F;
    }
    EM20_W(pEm)->Blood = 0x14;
    pEm->UnkE3 = 0;
    Set_blood(pEm, Rnd_l() & 0xF, 11);
    Set_blood(pEm, Rnd_l() & 0xF, 12);
    Set_blood(pEm, 1, 12);
}

/* RE3 only: like Em20_Die_gnml with the tail parts flying off (die_bang2_tbl) */
void Em20_Die_bang2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    die_bang2_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    Set_blood(pEm, 3, 0);
    Set_blood(pEm, 2, 0);
    Set_blood(pEm, 2, (Rnd_l() & 1) + 1);
}

/* RE3 only: Em20_Die_gnml00 variant for Em20_Die_bang2 */
void Em20_Die_bang2_00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Unk12C == 6 && (pEm->Unk132 & 0xC)) {
        Em20_Die_nml00(pEm, pKan, pSeq);
        return;
    }
    EM20_W(pEm)->No_voice = 1;
    Em20_Die_nml00(pEm, pKan, pSeq);
    Set_head_bomb(pEm);
    Set_tail_bomb(pEm);
    EM20_W(pEm)->Blood = 0x12;
}

/* RE2 Em20_Scenario */
void Em20_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Type & 0x100)
        func_80016600(pEm, pKan, pSeq);
    else
        Em20_sce_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE3 only: scenario 0, fades in and jumps in (move_jumpin_tbl) */
void Em20_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->Type & 0x8000)) {
        if (!pEm->UnkDA && (pEm->Type & 0x80))
            Em20_Esp_set(pEm);
        Em20_Col_set(pEm, ++pEm->UnkDA);
        pEm->Be_flg &= ~0x4030;
        move_jumpin_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    }
}

/* RE3 only: grey level n (0..16) on the 16 first parts */
void Em20_Col_set(ENEMY_WORK *pEm, LONG n)
{
    PARTS_W *pP;
    LONG i;

    i = 16;
    pP = pEm->pSin_parts_ptr;
    if (n < 17) {
        do {
            pP->Poly_rgb = (n << 3) | (n << 11) | (n << 19);
            pP++;
        } while (i--);
    }
}

/* RE2 Em20_Dead */
void Em20_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_1) {
        pEm->Routine_1 = 1;
        pEm->Be_flg |= 0x4000;
    }
    Em20_Blood_move(pEm);
}

/* RE2 Em20_Blood_move: the shadow turns into a growing blood pool */
void Em20_Blood_move(ENEMY_WORK *pEm)
{
    EM20_FREE *pFp;
    KAGE_WORK *pKage;

    pFp = EM20_W(pEm);
    pKage = pEm->Kage.pKw;
    switch (pFp->Kage_routine) {
    case 0:
        pFp->Kage_routine = 1;
        *(ULONG *)&pKage->Prim[0].r0 = (*(ULONG *)&pKage->Prim[0].r0 & 0xFF000000) | 0xBFBF10;
        *(ULONG *)&pKage->Prim[1].r0 = (*(ULONG *)&pKage->Prim[1].r0 & 0xFF000000) | 0xBFBF10;
        pEm->UnkE3 = 90;
    case 1:
        pKage->Half_x += 8;
        pKage->Half_z += 8;
        if (!--pEm->UnkE3)
            pFp->Kage_routine = 2;
        break;
    case 2:
        break;
    }
}

/* RE2 PLDIE_EM: the player's death routine while bitten (G.pPldie_Em[16]) */
void PLDIE_EM(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    USHORT i;

    switch (pEm->Routine_1) {
    case 0:
        pEm->Routine_1 = 1;
        EM_MOVE(pEm) = 0x1F0000;
        pEm->Unk120 |= 1;
        Snd_se_stad(0x10001, (VECTOR *)&pEm->Pos_x, 0, 0);
        break;
    case 1:
        pEm->Cdir_y += 0x800;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        pEm->Pos_y = pEm->Unk122;
        if (Joint_move2(pEm, pKan, pSeq, 0x80))
            pEm->Routine_1 = 2;
        if (pEm->Move_cnt == 0x62) {
            Snd_se_stad(0x10003, (VECTOR *)&pEm->Pos_x, 0, 0);
            for (i = 0; i < 4; i++) {
                pP = &G.Player.pSin_parts_ptr[pl_chi_parts[i]];
                PW_FREEL(pP)[0] = 0x20;
                PW_FREEL(pP)[1] = 0x101030;
                pP->UnkA2 |= 4;
            }
        }
    }
}

/* RE2 Dog_rot_neck */
void Dog_rot_neck(ENEMY_WORK *pEm)
{
    pEm->Neck_no = 2;
    Rot_neck_em(pEm, pEm->Cdir_y);
    pEm->Neck_no = 3;
    Rot_neck_em(pEm, pEm->Cdir_y);
    pEm->Neck_no = 2;
}

/* RE2 Rate_speed_up */
void Rate_speed_up(ENEMY_WORK *pEm, LONG g, LONG limit)
{
    if (pEm->Spd_x < limit) {
        pEm->Spd_x += g;
        if (pEm->Spd_x > limit)
            pEm->Spd_x = limit;
    }
}

/* RE2 Rate_speed_down */
void Rate_speed_down(ENEMY_WORK *pEm, LONG g, LONG limit)
{
    if (pEm->Spd_x > limit) {
        pEm->Spd_x -= g;
        if (pEm->Spd_x < limit)
            pEm->Spd_x = limit;
    }
}

/* RE2 Set_run_motion */
void Set_run_motion(ENEMY_WORK *pEm, SHORT d_tmp)
{
    UCHAR t_tmp;

    t_tmp = mot_tbl[d_tmp ? ((d_tmp > 0) ? 2 : 0) : 1];
    if (pEm->Move_no != t_tmp) {
        pEm->Move_no = t_tmp;
        if (pEm->UnkCA < 7)
            pEm->UnkCA = 15;
    }
}

/* RE2 Atck_range_ck */
LONG Atck_range_ck(ENEMY_WORK *pEm)
{
    EM20_FREE *pFp;
    ENEMY_WORK *pTg;
    LONG dist;

    pFp = EM20_W(pEm);
    pTg = (pFp->Target_pl) ? G.pEnemy[1] : &Pl_work;
    if (pTg->Life < 0)
        return -2;
    dist = (pFp->Target_pl) ? (pEm->Unk158 & 0x3FFFFFFF) : pEm->Unk154;
    if (dist < 4000 && (pTg->nFloor == pEm->nFloor || (pEm->Status_flg & 0x2000))) {
        if (!Dir_pos_ck((VECTOR *)&pEm->Pos_x, (VECTOR *)&pTg->Pos_x, pEm->Cdir_y, 0x100)) {
            if (pTg->Damage_cnt & 0x80)
                return -4;
            if (pTg->Life < 13 && Cdir_ck_l(pEm, pTg) && pFp->Stress < 600)
                return -2;
            return 1;
        }
        return -1;
    }
    return 0;
}

/* RE2 hit_ck_main: the bite (RE3 also checks the height of the target's parts 1 and 11) */
ULONG hit_ck_main(ENEMY_WORK *pEm, LONG pl)
{
    EM20_FREE *pFp;
    PARTS_W *pPt;
    PARTS_W *pP;
    ENEMY_WORK *pPl;
    LONG range, ck, d0, Low;

    pFp = EM20_W(pEm);
    pPt = pEm->pSin_parts_ptr;
    pPt += 4;
    pPl = (pl) ? G.pEnemy[1] : &Pl_work;
    d0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    range = (pPl->Life >= 13) ? 1000 : 700;
    pP = pPl->pSin_parts_ptr;
    pP += 1;
    Low = 0;
    if (__builtin_abs(pP->Workm.t[1] - pPt->Workm.t[1]) >= 800) {
        Low = 1;
        if (__builtin_abs(pP[10].Workm.t[1] - pPt->Workm.t[1]) >= 300)
            return pFp->Hit_type;
    }
    if (!pFp->Hit_type && !(pPl->Damage_cnt & 0x80)
        && L_pos_ckXZ((VECTOR *)&pPl->Pos_x, (VECTOR *)pPt->Workm.t, range)
        && pEm->Unk122 - 1800 < pEm->Pos_y
        && (pPl->nFloor == pEm->nFloor || (pEm->Status_flg & 0x2000))) {
        pFp->Stress = 0;
        if (!pl)
            ck = Pl_life_down(12, 0);
        if (ck == 2 && !Cdir_ck_l(pEm, pPl) && !Low && !d0 && !pFp->Target_pl && pPl->Id != 15) {
            pFp->Hit_type = ck;
            pEm->Spd_x = 0;
            pEm->Be_flg |= 0x4000;
            pPl->Damage_cnt = 0xFF;
            pPl->Be_flg |= 0x6000;
            pPl->Cdir_y = pEm->Cdir_y + 0x800;
            G.System_flg &= 0xFFFDFFFF;
            pEm->Damage_cnt |= 0x80;
        } else {
            EM_RTN23(pEm) = 3;
            pFp->Hit_type = 1;
            if (pPl->Life >= 0)
                Em20_Pl_to_dmg(pl, Cdir_ck(pEm, pPl));
            else {
                Em20_Pl_to_die(pl);
                pEm->Be_flg |= 0x4000;
            }
        }
    }
    return pFp->Hit_type;
}

/* RE2 Atck_hit_ck */
ULONG Atck_hit_ck(ENEMY_WORK *pEm)
{
    if (EM20_W(pEm)->No_hit)
        return 0;
    if (hit_ck_main(pEm, 0))
        return EM20_W(pEm)->Hit_type;
    if ((LONG)G.Status_flg < 0 && hit_ck_main(pEm, 1))
        return EM20_W(pEm)->Hit_type;
    return 0;
}

/* RE2 Set_blood: blood / effect type `type` on part `parts` */
void Set_blood(ENEMY_WORK *pEm, UCHAR parts, ULONG type)
{
    EM20_FREE *pFp;
    EM20_BLOOD_TBL *tbl;
    ENEMY_WORK *pTg;
    PARTS_W *pP_ptr;
    SVECTOR Sv;
    LONG dir;
    USHORT zoom;
    ULONG Id;
    ULONG Col;

    pFp = EM20_W(pEm);
    if (pFp->Blood) {
        tbl = &blood_id_tbl[(UCHAR)type];
        pTg = pEm;
        if ((UCHAR)type == 4)
            pTg = &Pl_work;
        zoom = tbl->Zoom;
        if ((UCHAR)(type - 1) < 2) {
            dir = Rnd_l() << 4;
            zoom = (tbl->Zoom + Rnd_l()) << 1;
        } else
            dir = pTg->Cdir_y + (Rnd_l() & (tbl->Dir * 2 - 1)) - tbl->Dir;
        pP_ptr = pTg->pSin_parts_ptr;
        pP_ptr += parts & 0x7F;
        Col = tbl->Unk6;
        Id = tbl->Id & 0x7F;
        if (Col == 0xFF) {
            Col = 0;
            if (pEm->Unk132 & 0x200)
                Col = 5;
        }
        if (tbl->Id & 0x80) {
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Esp_call(((Col & 0xF) << 24) | (tbl->Type << 8) | Id, (zoom << 16) | (dir & 0xFFFF), &pP_ptr->Workm, &Sv, NULL);
        } else {
            Sv.vx = pP_ptr->Workm.t[0];
            Sv.vy = pP_ptr->Workm.t[1];
            Sv.vz = pP_ptr->Workm.t[2];
            Esp_call(((Col & 0xF) << 24) | (tbl->Type << 8) | Id, (zoom << 16) | (dir & 0xFFFF), &GsIDMATRIX, &Sv, NULL);
        }
        pFp->Blood--;
    }
}

/* RE2 Em20_Pl_to_dmg */
void Em20_Pl_to_dmg(ULONG pl, UCHAR dir)
{
    ENEMY_WORK *pPl;

    pPl = (pl) ? G.pEnemy[1] : &Pl_work;
    pPl->Routine_0 = 2;
    pPl->Routine_1 = dir + 2;
    pPl->Routine_2 = 0;
    pPl->Routine_3 = 0;
    pPl->Damage_cnt |= 0x80;
}

/* RE2 Em20_Pl_to_die */
void Em20_Pl_to_die(ULONG pl)
{
    ENEMY_WORK *pPl;

    pPl = (pl) ? G.pEnemy[1] : &Pl_work;
    pPl->Routine_0 = 3;
    pPl->Routine_1 = 0;
    pPl->Routine_2 = 0;
    pPl->Routine_3 = 0;
}

/* RE2 Ck_Floor */
void Ck_Floor(ENEMY_WORK *pEm)
{
    if (__builtin_abs(G.Player.nFloor - pEm->nFloor) >= 2 && !(pEm->Status_flg & 0x2000)) {
        pEm->Routine_1 = 9;
        EM_RTN23(pEm) = 0;
        G.Unk75EA &= 0xFFDF;
    }
}

/* RE2 Ck_Died */
void Ck_Died(ENEMY_WORK *pEm)
{
    if (G.Unk75EA & 0x40) {
        pEm->Routine_1 = 11;
        EM_RTN23(pEm) = 0;
    }
}

/* RE2 Get_target_pl (RE3: the sub player whenever there is one) */
ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pTg;

    pTg = G_PLAYER;
    EM20_W(pEm)->Target_pl = 0;
    if ((LONG)G.Status_flg < 0) {
        pTg = G.pEnemy[1];
        EM20_W(pEm)->Target_pl = 1;
        if (G.pEnemy[1]->Life >= 21) {
            pTg = G.pEnemy[1];
            EM20_W(pEm)->Target_pl = 1;
        }
    }
    return pTg;
}
