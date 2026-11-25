#include "enemy/em25.h"
#include "joint.h"
#include "sce_at.h"
#include "snd.h"
#include "sub.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */

/* RE2 EM_MAIN */
void func_801000C8(ENEMY_WORK *pEm)
{
    EM25_FREE *pFp = EF(pEm);

    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (EF(pEm)->No_fire)
        EF(pEm)->No_fire--;
    Em25_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Em25_kage_move(pEm);
    if (pFp->Place == 1)
        pEm->Be_flg |= 0x400;
    else
        pEm->Be_flg &= ~0x400;
    if ((ULONG)(pFp->Place - 2) < 2) {
        pEm->Be_flg |= 0x8000;
        func_80102C7C(pEm);
    } else {
        pEm->Col.Unk24 = 0;
        pEm->Col.Unk28 = 0;
        pEm->Be_flg &= ~0x8000;
    }
}

/* RE2 Em25_Init */
void Em25_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;
    COL_OBJ *pCol;
    PARTS_W *pP;
    ULONG i;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Be_flg |= 0x400000;
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = Em25_life_easy[(Rnd_l() >> (Rnd_l() & 3)) & 0xF];
    else
        pEm->Max_life = pEm->Life = Em25_life[Rnd_l() & 0xF] + (Rnd_l() & 3);
    pEm->Unk144 = 0;
    pEm->Max_life += 10;
    pEm->Life += 10;
    for (i = 0; i < 12; i++)
        pEm->Unk1CC[i] = 0;
    EM_SET_MOVE(pEm, 1, 0, 0);
    Joint_move(pEm, pKan, pSeq, 0x100);
    pEm->Neck_flg = 0;
    pEm->Neck_no = 0;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x3E803E8, 0, &pEm->Pos_x);
    pCol = &pEm->Col;
    pFp = EF(pEm);
    pFp->Foot_cnt = 8;
    pFp->Water_rno = 0;
    pEm->Water = 0;
    pEm->Hit_area.Bottom = -1500;
    pEm->Hit_area.Top = 800;
    pEm->Hit_area.Rate0 = 0x50;
    pEm->Hit_area.Rate1 = 0xA0;
    pEm->Hit_area.R = 800;
    pEm->Unk140 = 0;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pCol->pEm = pEm;
    pCol->W0 = pEm->Col.D0 = 1000;
    pCol->H = 1800;
    pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    func_80033580(pCol, 0);
    if (!Ck_em_id(pEm, &pFp->pPair, 0x25, 1))
        pFp->pPair = NULL;
    switch (pEm->Type & 0xF) {
    case 0:
        pFp->Place = 0;
        pFp->Flip = 0;
        break;
    case 2:
    case 3:
        pFp->Place = 1;
        pFp->Flip = 0x800;
        pEm->Cdir_z = 0x800;
        pEm->Col.Unk26 = -1990;
        pEm->Be_flg |= 0x400;
        pFp->Down_point = pEm->Pos_y + 1250;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        pFp->Place = 3;
        pFp->Flip = 0x800;
        pFp->Wall_pos = ((pEm->Type & 0x7F) - 4) >> 1;
        pEm->Cdir_y = pFp->Wall_pos << 10;
        pEm->Col.W0 = pEm->Col.D0 = 600;
        pEm->Col.Unk26 = -600;
        pEm->Be_flg |= 0x400;
        Ck_lock_wall(pEm);
        if (!(pEm->Type & 0x70))
            pFp->Down_point = -3960;
        else
            pFp->Down_point = ((pEm->Type & 0x70) >> 4) * -1800 + 1250;
        pFp->Kage_routine = 1;
        Em25_wall_init(pEm, pKan, pSeq);
        func_80034124(&pEm->Col);
        break;
    }
    if (pEm->Type & 0x100)
        EF(pEm)->Unk2F = 0;
    else
        EF(pEm)->Unk2F = 1;
    if (pEm->Type & 0x80)
        EF(pEm)->Unk2E = 0;
    else
        EF(pEm)->Unk2E = 1;
    for (i = 0; i < 20; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->Be_flg |= 0x20;
    }
}

/* RE2 Em25_Move */
void Em25_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp = EF(pEm);

    Get_target_pl(pEm);
    Em25_move_tbl[EF(pEm)->Place](pEm, pKan, pSeq);
    if (EF(pEm)->at_cnt)
        EF(pEm)->at_cnt--;
    if (pFp->Nowall_cnt)
        pFp->Nowall_cnt--;
    if (pFp->sch_flg) {
        pFp->sc_timer++;
    } else {
        pFp->sc_timer = 0;
        pFp->Rno1_bak = pEm->Routine_1;
        pFp->Rno2_bak = pEm->Routine_2;
        pFp->Rno3_bak = pEm->Routine_3;
    }
}

/* RE2 Em25_Move_floor */
void func_80100658(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em25_floor_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em25_mv_taiki */
void Em25_mv_taiki(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() + 15) & 0x1F;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (pEm->Timer0-- == 0) {
            EM_SET_RTN(pEm, 1, 3, 0, 0);
            if (Rnd_l() & 1)
                EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        break;
    }
}

/* RE2 Em25_mv000 */
void func_80100758(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Dir0;
    USHORT LL;

    Em25_mv_taiki(pEm, pKan, pSeq);
    LL = EF(pEm)->Target_pl ? pEm->Unk158 : pEm->L_pl;
    Dir0 = Em25_goto00(pEm, 0x80);
    Em25_goto00(pEm, 0x100);
    if (Em25_goto00(pEm, 0x400) && LL < 8000)
        EM_SET_RTN(pEm, 1, 5, 0, 0);
    if (G.Unk75E8 & 1) {
        EM_SET_RTN(pEm, 1, 9, 0, 0);
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
    }
    if (EF(pEm)->at_cnt == 0) {
        if (LL < 7500 && !Dir0 && LL > 3000)
            EM_SET_RTN(pEm, 1, 7, 0, 0);
        if (LL < 6000 && !Dir0 && LL > 1000 && EF(pEm)->Unk2E) {
            if ((0x340 >> (Rnd_l() & 0xF)) & 1)
                EM_SET_RTN(pEm, 1, 8, 0, 0);
            else
                EM_SET_RTN(pEm, 1, 7, 0, 0);
        }
    }
}

/* RE2 Em25_mv01 */
void Em25_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Em25_mv01_tbl[pEm->Routine_1 - 1][0] + 0x70000;
        pEm->Routine_2 = 1;
        pEm->Spd_z = 0;
        pEm->Timer0 = Rnd_l() & 0x3D;
    case 1:
        pEm->Cdir_y += Em25_goto00(pEm, Em25_mv01_tbl[pEm->Routine_1 - 1][1]);
        Joint_move(pEm, pKan, pSeq, 0x200);
        Set_foot_se(pEm);
        Em25_foot_set(pEm, 1);
        pEm->UnkDA = pEm->Spd_x;
        pEm->Spd_x += Em25_mv01_tbl[pEm->Routine_1 - 1][2];
        if (pEm->Timer0-- == 0)
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
    Add_speedXZ(pEm, EF(pEm)->Flip);
}

/* RE2 Em25_mv04 */
void func_80100A10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() + 10) & 0x3F;
        pEm->UnkDA = 0x18;
        if (Rnd_l() & 1)
            pEm->UnkDA = -0x18;
        if (EF(pEm)->Place)
            pEm->UnkDA = -pEm->UnkDA;
    case 1:
        pEm->Cdir_y += pEm->UnkDA;
        Joint_move(pEm, pKan, pSeq, 0x200);
        Set_foot_se(pEm);
        if (pEm->Timer0-- == 0)
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
}

/* RE2 Em25_mv05 */
void func_80100B18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Add_dir;
    ULONG Em25_mv03_tbl[2][2] = { { 2, 0x80 }, { 3, 0x20 } };

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Em25_mv03_tbl[pEm->Routine_1 - 5][0] + 0x70000;
        pEm->Routine_2 = 1;
        pEm->UnkDA = (Rnd_l() & 0x1F) + 0x3C;
    case 1:
        Add_dir = Em25_goto00(pEm, Em25_mv03_tbl[pEm->Routine_1 - 5][1]);
        pEm->Cdir_y += Add_dir;
        if (pEm->UnkDA-- == 0 || !Add_dir)
            pEm->Routine_2 = 2;
        Joint_move(pEm, pKan, pSeq, 0x200);
        Set_foot_se(pEm);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
    }
}

/* RE2 Em25_mv07 (RE3 adds a hit box in front while it charges) */
void func_80100C80(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    HIT_AREA Hit;
    SHORT Box[4];
    ENEMY_WORK *pTg;
    SHORT Dir;

    pTg = EF(pEm)->Target_pl ? G.pEnemy[1] : &Pl_work;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Spd_z = 0;
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x = 100;
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Timer0 = (Rnd_l() & 0x1F) + 20;
        pEm->Routine_2 = 3;
    case 3:
        Dir = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x40);
        pEm->Cdir_y += Dir;
        Box[0] = 2000;
        Box[1] = 0;
        Box[2] = 2200;
        Box[3] = 1200;
        Hit.pPos = WORKM_POS_V(pEm->pSin_parts_ptr);
        Hit.pBox = Box;
        Hit.Dir = pEm->Cdir_y;
        Hit.Id = 0x81;
        func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Hit, pTg);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!Dir)
            pEm->Routine_2 = 4;
        if (pEm->Timer0-- == 0 || pTg->Routine_1 == 7)
            pEm->Routine_2 = 6;
        break;
    case 4:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Routine_2 = 5;
        pEm->Timer0 = (Rnd_l() & 0x1F) + 10;
        pEm->Spd_x = 300;
    case 5:
        Box[0] = 2000;
        Box[1] = 0;
        Box[2] = 2200;
        Box[3] = 1200;
        Hit.pPos = WORKM_POS_V(pEm->pSin_parts_ptr);
        Hit.pBox = Box;
        Hit.Dir = pEm->Cdir_y;
        Hit.Id = 0x81;
        func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Hit, pTg);
        Goto00(pEm, pTg->Pos_x, pTg->Pos_z, 0x10);
        Add_speedXZ(pEm, EF(pEm)->Flip);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (pTg->Routine_1 == 7 || pEm->Timer0-- == 0 || pEm->Col.Unk3 != 0xFF || (pEm->Col.Hit_flg & 0x20)
            || Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x200))
            pEm->Routine_2 = 6;
        if (Em25_atck_ck(pEm, 1500, 4300))
            pEm->Routine_2 = 6;
        break;
    case 6:
        EM_SET_MOVE(pEm, 7, 0, 7);
        pEm->Spd_x = 100;
        pEm->Routine_2 = 7;
    case 7:
        Add_speedXZ(pEm, EF(pEm)->Flip);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 8:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        EF(pEm)->at_cnt = (Rnd_l() & 0x1F) + 60;
        break;
    }
    Set_foot_se(pEm);
}

/* RE2 Em25_mv08 (spit) */
void func_80101028(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Off_sv;
    HIT_AREA Hit;
    SHORT Box[4];
    ENEMY_WORK *pPl;

    pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xB, 0, 7);
        pEm->Routine_2 = 1;
        Set_poison(pEm, EF(pEm)->Place, pEm->Cdir_y + EF(pEm)->Flip);
    case 1:
        if (!EF(pEm)->Place) {
            Box[0] = 3500;
            Box[1] = 0;
            Box[2] = 1800;
            Box[3] = 1200;
            Hit.pPos = WORKM_POS_V(pEm->pSin_parts_ptr);
            Hit.pBox = Box;
            Hit.Dir = pEm->Cdir_y;
            Hit.Id = 0x81;
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Hit, pPl);
        }
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        EF(pEm)->at_cnt = 15;
        break;
    case 3:
        if (Em25_mv08sp(pEm, pKan, pSeq)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            EF(pEm)->at_cnt = 15;
        }
    }
    Set_foot_se(pEm);
    Em_seq_sound(pEm);
}

/* RE2 Em25_mv08sp: the RE2 spit test (*pNow_seq & 0x10000) is never true with RE3's 16-bit seq flags */
LONG Em25_mv08sp(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG type;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_SET_MOVE(pEm, 0xA, 0, 7);
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (*pEm->pNow_seq & 0x10000) {
            pEm->Routine_3 = 2;
            Snd_se_enem(3, pEm);
            if (EF(pEm)->Place < 2) {
                Set_poison(pEm, EF(pEm)->Place + 8, pEm->Cdir_y + EF(pEm)->Flip);
                return 0;
            }
            type = ((EF(pEm)->Target_pl ? (pEm->Unk158 & 0x3FFFFFFF) : (LONG)pEm->L_pl) < 3000) ? 0x89 : 0x88;
        } else
            break;
        Set_poison(pEm, type, Get_wallshot(pEm));
        return 0;
    case 2:
        if (Joint_move(pEm, pKan, pSeq, 0x400))
            pEm->Routine_3 = 3;
        break;
    case 3:
        return 1;
    }
    return 0;
}

/* RE2 Em25_mv09 */
void func_80101254(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX Tm;
    SVECTOR Tsv;
    ENEMY_WORK *pTg;

    pTg = EF(pEm)->Target_pl ? G.pEnemy[1] : &Pl_work;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 8, 0, 3);
        pEm->Routine_2 = 1;
        Tsv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, pTg->Pos_x, pTg->Pos_z);
        Tsv.vx = 0;
        Tsv.vy += Rnd_l() + 0x380 + (Rnd_l() & 1) * 0x800;
        Tsv.vz = 0;
        pEm->Timer0 = Tsv.vy;
        RotMatrix(&Tsv, &Tm);
        Tsv.vx = (Rnd_l() & 0x3F) + 400;
        Tsv.vy = 0;
        Tsv.vz = 0;
        ApplyMatrixSV(&Tm, &Tsv, EM_SPD_SV(pEm));
    case 1:
        pEm->Cdir_y += Em25_goto00(pEm, 0x60);
        Add_speedXZ(pEm, EF(pEm)->Flip + pEm->Timer0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        EF(pEm)->sc_timer = 0;
    }
}

/* RE2 Em25_Move_ceiling */
void func_801013EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em25_ceiling_tbl[pEm->Routine_1](pEm, pKan, pSeq);
    if (pEm->Routine_1 != 10 && pEm->Col.D0 < 1000)
        pEm->Col.D0 = pEm->Col.W0 += 100;
}

/* RE2 Em25_mv001 */
void func_80101468(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Dir0;
    USHORT LL;

    Em25_mv_taiki(pEm, pKan, pSeq);
    LL = EF(pEm)->Target_pl ? pEm->Unk158 : pEm->L_pl;
    Dir0 = Em25_goto00(pEm, 0x80);
    Em25_goto00(pEm, 0x100);
    Em25_goto00(pEm, 0x400);
    if (G.Unk75E8 & 1) {
        EM_SET_RTN(pEm, 1, 9, 0, 0);
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
    }
    if (EF(pEm)->at_cnt == 0) {
        if (LL > 3000 && LL < 7500) {
            if (!Dir0 && ((0x340 >> (Rnd_l() & 0xF)) & 1) && EF(pEm)->Unk2E)
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            else
                EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        if (LL < 3000 && !Dir0) {
            if (((0x340 >> (Rnd_l() & 0xF)) & 1) && EF(pEm)->Unk2E)
                EM_SET_RTN(pEm, 1, 8, 0, 0);
            else
                EM_SET_RTN(pEm, 1, 7, 0, 0);
        }
    }
}

/* RE2 Em25_mv011 */
void func_801015B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOther;
    LONG dist;

    dist = 9999;
    Em25_mv01(pEm, pKan, pSeq);
    pOther = EF(pEm)->pPair;
    if (pOther && pOther->Routine_0 != 7) {
        dist = SquareRoot0((pEm->Pos_x - pOther->Pos_x) * (pEm->Pos_x - pOther->Pos_x)
                           + (pEm->Pos_z - pOther->Pos_z) * (pEm->Pos_z - pOther->Pos_z));
        if (EF(pOther)->Place < 2)
            dist = 9999;
    }
    if (pEm->Col.pSca_hit_data) {
        if (pEm->Col.pSca_hit_data->Id & 0x100) {
            if (dist < 2500) {
                if (pOther)
                    pEm->Routine_1 = 11;
                else
                    pEm->Routine_1 = 4;
                EM_RTN23(pEm) = 0;
                return;
            }
            if (!EF(pEm)->Nowall_cnt) {
                if ((EF(pEm)->Wall_pos = Ck_lock_wall(pEm))) {
                    EF(pEm)->Wall_pos--;
                    if (!Ck_wall_edge(pEm, 0x400) && !Ck_wall_edge(pEm, -0x400)
                        && Ck_near_wall(pEm, EF(pEm)->pWall, EF(pEm)->Wall_pos)) {
                        pEm->Routine_1 = 10;
                        EM_RTN23(pEm) = 0;
                        EF(pEm)->Cont_flg = 1;
                    }
                }
            } else {
                pEm->Routine_1 = 4;
                EM_RTN23(pEm) = 0;
            }
        }
        if (EF(pEm)->Unk2F && (LONG)pEm->L_pl < 3500 && !(Rnd_l() & 3))
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
}

/* RE2 Em25_mv071 */
void func_80101764(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Add_dir;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Spd_z = 0;
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x = 100;
        Add_speedXZ(pEm, EF(pEm)->Flip);
        break;
    case 2:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Timer0 = (Rnd_l() & 0x1F) + 45;
        pEm->Routine_2 = 3;
    case 3:
        Add_dir = Em25_goto00(pEm, 0x40);
        pEm->Cdir_y += Add_dir;
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!Add_dir)
            pEm->Routine_2 = 4;
        if (pEm->Timer0-- == 0)
            pEm->Routine_2 = 6;
        break;
    case 4:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Routine_2 = 5;
        pEm->Timer0 = (Rnd_l() & 0x1F) + 10;
        pEm->UnkE2 = 1;
        pEm->Spd_x = 300;
    case 5:
        pEm->Cdir_y += Em25_goto00(pEm, 0x10);
        Add_speedXZ(pEm, EF(pEm)->Flip);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!--pEm->UnkE2 && EF(pEm)->Unk2E) {
            Snd_se_enem(3, pEm);
            Set_poison(pEm, 2, pEm->Cdir_y + EF(pEm)->Flip);
            pEm->UnkE2 = 12;
        }
        if (pEm->Timer0-- == 0 || Em25_goto00(pEm, 0x200) || (pEm->Col.Hit_flg & 0x20) || pEm->Col.Unk3 != 0xFF)
            pEm->Routine_2 = 6;
        break;
    case 6:
        EM_SET_MOVE(pEm, 7, 0, 7);
        pEm->Spd_x = 100;
        pEm->Routine_2 = 7;
    case 7:
        Add_speedXZ(pEm, EF(pEm)->Flip);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 8:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        EF(pEm)->at_cnt = 60;
    }
    Set_foot_se(pEm);
}

/* RE2 Em25_mv0a: turn to the wall and climb from the ceiling onto it */
void func_80101A40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Cdir_y &= 0xFFF;
        pEm->Timer0 = (EF(pEm)->Sc_dir - pEm->Cdir_y) & 0xFFF;
        pEm->UnkDA = 0x18;
        if (pEm->Timer0 & 0x800) {
            pEm->Timer0 = (pEm->Cdir_y - EF(pEm)->Sc_dir) & 0xFFF;
            pEm->UnkDA = -pEm->UnkDA;
        }
        EM_SET_MOVE(pEm, 3, 0, 7);
    case 1:
        if (pEm->Timer0 > 0) {
            pEm->Cdir_y += pEm->UnkDA;
            pEm->Timer0 -= 0x18;
        } else {
            pEm->Routine_2 = 2;
            pEm->Cdir_y = EF(pEm)->Sc_dir;
        }
        Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        pEm->Routine_2 = 3;
        pEm->Spd_x = 25;
        pEm->Spd_z = 0;
        EM_SET_MOVE(pEm, 0xE, 0, 7);
    case 3:
        Add_speedXZ(pEm, EF(pEm)->Flip);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            EF(pEm)->Place = 2;
            pEm->Spd_x = 450;
            Add_speedXZ(pEm, EF(pEm)->Flip);
            pEm->Pos_y += 1250;
            pEm->Col.W0 = pEm->Col.D0 = 600;
            pEm->Col.Unk26 = -600;
            EF(pEm)->Cont_flg = 0;
            EF(pEm)->Down_point = pEm->Pos_y;
            Em25_Mv_wall000(pEm, pKan, pSeq);
        } else {
            if (pEm->Col.W0 > 200)
                pEm->Col.D0 = pEm->Col.W0 -= 100;
            if (pEm->Col.Unk26 > -1800)
                pEm->Col.Unk26 -= 50;
        }
    }
    Set_foot_se(pEm);
}

/* RE2 Em25_mv0b (retail RE2 ceiling routine 11): turn toward the other em25 */
void func_80101C58(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOther;

    pOther = EF(pEm)->pPair;
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Timer0 = 60;
        pEm->UnkDA = Goto00_ck(pEm, pOther->Pos_x, pOther->Pos_z, 0x40);
    }
    pEm->Cdir_y += pEm->UnkDA;
    Joint_move(pEm, pKan, pSeq, 0x200);
    Set_foot_se(pEm);
    Em25_foot_set(pEm, 1);
    pEm->Spd_x += Em25_mv01_tbl[0][2];
    if (!--pEm->Timer0)
        EM_SET_RTN(pEm, 1, 0, 0, 0);
    if (!(pEm->Col.Hit_flg & 0x20))
        Add_speedXZ(pEm, EF(pEm)->Flip);
}

/* RE3 ceiling routine 12: drop from the ceiling (Em25_mv0c_tbl) */
void func_80101D54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em25_mv0c_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE3: start falling from the ceiling */
void func_80101D90(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pEm->Routine_2 = 1;
    pEm->Spd_y = 40;
    pEm->Timer0 = 0x800;
    pFp = EF(pEm);
    pEm->Spd_x = 0;
    pEm->Spd_z = 0;
    EM_SET_MOVE(pEm, 1, 0, 3);
    pFp->Jump_flg = 1;
    pFp->Flip = 0;
    pEm->Damage_cnt |= 0x80;
}

/* RE2 Em25_Move_walldown */
void func_80101DDC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_wall_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em25_wall_init */
void Em25_wall_init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (EF(pEm)->Wall_pos & 1) {
        pEm->Cdir_x = pEm->Cdir_y;
        pEm->Cdir_z = 0;
    } else {
        pEm->Cdir_x = 0;
        pEm->Cdir_z = 0x400;
    }
    pEm->Col.W0 = pEm->Col.D0 = 600;
    pEm->Col.Unk26 = -600;
    EF(pEm)->Kage_routine = 1;
    EF(pEm)->Sc_dir = 0;
    EM_SET_MOVE(pEm, 0, 0, 3);
    Joint_move(pEm, pKan, pSeq, 0x400);
}

/* RE2 Em25_Mv_wall000 */
void Em25_Mv_wall000(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG dist, water;
    ENEMY_WORK *pTg;

    Em25_wall_init(pEm, pKan, pSeq);
    pTg = EF(pEm)->Target_pl ? G.pEnemy[1] : &Pl_work;
    dist = pTg->Pos_y - pEm->Pos_y;
    if (dist > 3500) {
        water = Water_ck(pEm->Pos_x, pEm->Pos_z);
        if (water >= pEm->Ground) {
            pEm->Timer0 = (dist - 3500) / 100;
        } else {
            if ((pEm->Timer0 = (-(pEm->Pos_y + 2200) + water) / 100) < 0)
                pEm->Timer0 = 0;
        }
        if (pEm->Timer0) {
            pEm->Timer0 += Rnd_l() & 3;
            pEm->Routine_1 = 1;
            EM_RTN23(pEm) = 0;
            EF(pEm)->Em_next = 2;
            return;
        }
    }
    pEm->Routine_1 = 2;
    EM_RTN23(pEm) = 1;
    pEm->Timer0 = (Rnd_l() & 0xF) + 30;
}

/* RE2 Em25_Mv_wall10 */
void func_80101FD8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Spd_x = 100;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        EM_SET_MOVE(pEm, 2, 0, 3);
    }
    if (pEm->Timer0) {
        Em25_add_speed(pEm, 0, 0, 0);
        pEm->Timer0--;
    }
    Joint_move(pEm, pKan, pSeq, 0x400);
    Set_foot_se(pEm);
    if (!pEm->Timer0) {
        pEm->Routine_1 = EF(pEm)->Em_next;
        EM_RTN23(pEm) = 0;
        pEm->Timer0 = (Rnd_l() & 0xF) + 15;
    }
}

/* RE2 Em25_Mv_wall200 */
void func_801020B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 0, 0, 3);
    }
    Joint_move(pEm, pKan, pSeq, 0x400);
    if (!--pEm->Timer0) {
        if (((EF(pEm)->Target_pl ? (pEm->Unk158 & 0x3FFFFFFF) : (LONG)pEm->L_pl) < 3000)
            && ((0x340 >> (Rnd_l() & 0xF)) & 1) && EF(pEm)->Unk2E)
            pEm->Routine_1 = 7;
        else
            pEm->Routine_1 = 3;
        EM_RTN23(pEm) = 0;
    }
}

/* RE2 Em25_Mv_wall30 */
void func_8010219C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->UnkDA = Ck_wall_to_pl(pEm, 0x20);
        if (!pEm->UnkDA && EF(pEm)->Unk2E) {
            pEm->Routine_1 = 7;
            EM_RTN23(pEm) = 0;
            break;
        }
        pEm->Timer0 = (pEm->UnkDA > 0) ? 0x400 : -0x400;
        if (!EF(pEm)->Wall_pos)
            pEm->Timer0 = -pEm->Timer0;
    case 1:
        pEm->Routine_2 = 2;
        pEm->UnkE2 = (Rnd_l() & 0xF) + 15;
        EM_SET_MOVE(pEm, 2, 0, 3);
    case 2:
        Em25_wall_rot(pEm, pEm->UnkDA);
        if ((EF(pEm)->Sc_dir & 0xFFF) == (pEm->Timer0 & 0xFFF)) {
            pEm->Routine_1 = 6;
            EM_RTN23(pEm) = 0;
            break;
        }
        if (!--pEm->UnkE2) {
            pEm->Routine_2 = 3;
            pEm->UnkE2 = (Rnd_l() & 0xF) + 1;
            EM_SET_MOVE(pEm, 0, 0, 3);
        }
        break;
    case 3:
        if (!--pEm->UnkE2)
            pEm->Routine_2 = 1;
    }
    Joint_move(pEm, pKan, pSeq, 0x400);
    Set_foot_se(pEm);
}

/* RE2 Em25_Mv_wall40 */
void func_80102338(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT dd;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->UnkDA = !EF(pEm)->Em_next ? ceil_turn_tbl[Rnd_l() & 7] : 0x40;
        EF(pEm)->Cont_flg = 1;
    case 1:
        pEm->Routine_2 = 2;
        if (!EF(pEm)->Em_next)
            pEm->UnkE2 = (Rnd_l() & 0xF) + 15;
        else
            pEm->UnkE2 = -1;
        if (__builtin_abs(pEm->UnkDA) == 0x20)
            EM_SET_MOVE(pEm, 2, 0, 3);
        else
            EM_SET_MOVE(pEm, 1, 0, 3);
    case 2:
        Em25_wall_rot(pEm, pEm->UnkDA);
        dd = __builtin_abs(pEm->UnkDA);
        if ((((EF(pEm)->Sc_dir & 0xFFF) - 0x800 + dd) & 0xFFF) <= dd * 2)
            pEm->Routine_2 = 5;
        if (!--pEm->UnkE2) {
            pEm->Routine_2 = 3;
            pEm->UnkE2 = (Rnd_l() & 0xF) + 15;
            EM_SET_MOVE(pEm, 0, 0, 3);
        }
        break;
    case 3:
        if (!--pEm->UnkE2)
            pEm->Routine_2 = 1;
        break;
    case 4:
    case 5:
        pEm->Routine_2 = 6;
        EF(pEm)->Sc_dir = 0x800;
        EF(pEm)->Cont_flg = 1;
        pEm->Spd_x = 100;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        EM_SET_MOVE(pEm, 2, 0, 3);
        break;
    case 6:
        Em25_add_speed(pEm, 0, 0, 0);
        if (pEm->Pos_y <= EF(pEm)->Down_point) {
            pEm->Pos_y = EF(pEm)->Down_point;
            pEm->Routine_1 = 5;
            EM_RTN23(pEm) = 0;
        }
    }
    Joint_move(pEm, pKan, pSeq, 0x400);
    Set_foot_se(pEm);
}

/* RE2 Em25_Mv_wall50: climb from the wall onto the ceiling */
void func_80102578(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOther;
    LONG dist;

    dist = 9999;
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        if (EF(pEm)->Wall_pos & 1) {
            pEm->Spd_x = 0;
            pEm->Spd_y = 25;
        } else {
            pEm->Spd_x = 25;
            pEm->Spd_y = 0;
        }
        pEm->Col.W0 = pEm->Col.D0 = 600;
        pEm->Col.Unk26 = -600;
        pEm->Spd_z = 0;
        EM_SET_MOVE(pEm, 0, 0, 3);
    case 1:
        pOther = EF(pEm)->pPair;
        if (pOther && pOther->Routine_0 != 7) {
            dist = SquareRoot0((pEm->Pos_x - pOther->Pos_x) * (pEm->Pos_x - pOther->Pos_x)
                               + (pEm->Pos_z - pOther->Pos_z) * (pEm->Pos_z - pOther->Pos_z));
            if (EF(pOther)->Place != 1)
                dist = 9999;
        }
        if (dist > 2500) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0xE, 0, 3);
        }
        Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        Add_speedXYZ(pEm, 0, EF(pEm)->Sc_dir);
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            EF(pEm)->Place = 1;
            EF(pEm)->Flip = 0x800;
            if (EF(pEm)->Wall_pos & 1)
                pEm->Cdir_y = pEm->Cdir_x + 0x800;
            else
                pEm->Cdir_y += 0x800;
            pEm->Cdir_z = 0x800;
            EF(pEm)->Nowall_cnt = 150;
            pEm->Cdir_x = 0;
            pEm->Spd_x = 1225;
            pEm->Pos_y -= 475;
            Add_speedXZ(pEm, EF(pEm)->Flip);
            pEm->Col.W0 = pEm->Col.D0 = 500;
            pEm->Col.Unk26 = -1910;
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            EF(pEm)->Cont_flg = 0;
            EF(pEm)->Kage_routine = 0;
        } else {
            if (pEm->Col.W0 > 500)
                pEm->Col.D0 = pEm->Col.W0 -= 100;
        }
    }
    Set_foot_se(pEm);
}

/* RE2 Em25_Mv_wall60 */
void func_801027C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() & 0xF) + 30;
        pEm->Spd_x = 300;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        EM_SET_MOVE(pEm, 4, 0, 3);
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 5, 0, 3);
        }
        break;
    case 2:
        Em25_add_speed(pEm, 0, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x400);
        if (Ck_wall_edge(pEm, EF(pEm)->Sc_dir) || pEm->Col.Unk3 != 0xFF || (pEm->Col.Hit_flg & 0x20)) {
            pEm->Routine_2 = 3;
            EM_SET_MOVE(pEm, 7, 0, 3);
        }
        if (Em25_atck_ck(pEm, 2000, 4300)) {
            pEm->Routine_2 = 3;
            EM_SET_MOVE(pEm, 7, 0, 3);
        }
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_1 = 4;
            EM_RTN23(pEm) = 0;
            EF(pEm)->Em_next = 0;
        }
    }
    Set_foot_se(pEm);
}

/* RE2 Em25_Mv_wall70 */
void func_8010295C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT type;
    SHORT Dir;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x10;
        EM_SET_MOVE(pEm, 1, 0, 3);
    case 1:
        Dir = Ck_wall_to_pl(pEm, 0x40);
        Em25_wall_rot(pEm, Dir);
        if (!Dir || !--pEm->Timer0) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0xB, 0, 3);
            type = ((EF(pEm)->Target_pl ? (pEm->Unk158 & 0x3FFFFFFF) : (LONG)pEm->L_pl) < 3000) ? 0x81 : 0x80;
            Set_poison(pEm, type, Get_wallshot(pEm));
        }
        Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_1 = 4;
            EM_RTN23(pEm) = 0;
            EF(pEm)->Em_next = 0;
        }
        break;
    case 3:
        if (Em25_mv08sp(pEm, pKan, pSeq)) {
            pEm->Routine_1 = 4;
            EM_RTN23(pEm) = 0;
            EF(pEm)->Em_next = 0;
        }
    }
    Em_seq_sound(pEm);
}

/* RE2 Get_wallshot */
SHORT Get_wallshot(ENEMY_WORK *pEm)
{
    SHORT dir;

    dir = EF(pEm)->Sc_dir;
    if (!EF(pEm)->Wall_pos)
        dir = -dir;
    return ((EF(pEm)->Wall_pos << 10) + (dir - (dir >> 2))) & 0xFFF;
}

/* RE2 Em25_Move_wallset */
void func_80102B3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    move_wallset_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em25_Mv_wall001 */
void func_80102B78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em25_wall_init(pEm, pKan, pSeq);
    pEm->Routine_1 = 2;
    EM_RTN23(pEm) = 0;
}

/* RE2 Em25_Mv_wall201 */
void func_80102BA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Joint_move(pEm, pKan, pSeq, 0x400);
    if (((EF(pEm)->Target_pl ? (pEm->Unk158 & 0x3FFFFFFF) : (LONG)pEm->L_pl) < 7200)
        || Ck_near_wall(pEm, EF(pEm)->pWall, EF(pEm)->Wall_pos)) {
        if (((EF(pEm)->Target_pl ? (pEm->Unk158 & 0x3FFFFFFF) : (LONG)pEm->L_pl) < 3000) && EF(pEm)->Unk2E)
            pEm->Routine_1 = 7;
        else
            pEm->Routine_1 = 3;
        EM_RTN23(pEm) = 0;
    }
}

/* RE3: hit area offset for the wall position */
void func_80102C7C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    SHORT dir;

    Sv.vx = 1400;
    Sv.vz = 0;
    dir = EF(pEm)->Sc_dir;
    if (EF(pEm)->Wall_pos)
        dir = -dir;
    Rot_vector(dir, &Sv, &Sv);
    pEm->Col.Unk24 = Sv.vx;
    pEm->Col.Unk28 = Sv.vz;
}

/* RE2 Em25_Damage */
void func_80102CE8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2)
        EF(pEm)->Dmg_type = pEm->Damage_no;
    if (!pEm->Water)
        pEm->Water = Water_ck(pEm->Pos_x, pEm->Pos_z);
    Em25_dm[pEm->Damage_no - 1](pEm, pKan, pSeq);
}

/* RE2 Em25_dm_nml */
void Em25_dm_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    dmg_nml_tbl[EF(pEm)->Place](pEm, pKan, pSeq);
}

/* RE2 Em25_dm_nml00 */
void Em25_dm_nml00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG next;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 1, 0, 3);
        if (EF(pEm)->Dmg_type != 1)
            Set_blood(pEm, 0, 0);
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        if (pEm->Damage_no == 1 || !(pEm->Damage_flg & 0xC))
            next = 0x901;
        else
            next = (next_rtn_tbl[Rnd_l() & 7] << 8) + 1;
        EM_RTN(pEm) = next;
    }
}

/* RE2 Em25_dm_nml10 */
void func_80102ED4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (pFp->Cont_flg) {
        EM_RTN(pEm) = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
        if (EF(pEm)->Dmg_type != 1)
            Set_blood(pEm, 0, 0);
        Em25_Move(pEm, pKan, pSeq);
    } else {
        Em25_dm_nml00(pEm, pKan, pSeq);
    }
}

/* RE2 Em25_dm_nml20 */
void func_80102F88(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (EF(pEm)->Dmg_type != 1)
        Set_blood(pEm, 1, 0);
    if (pFp->Cont_flg) {
        EM_RTN(pEm) = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
        Em25_Move(pEm, pKan, pSeq);
    } else {
        pFp->Em_next = 1;
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    }
}

/* RE2 Em25_dm_ftl */
void Em25_dm_ftl(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    dmg_ftl_tbl[EF(pEm)->Place](pEm, pKan, pSeq);
}

/* RE2 Em25_dm_ftl00 */
void Em25_dm_ftl00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 1, 0, 3);
        Foot_break_set(pEm);
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 9, 0, 0);
    }
}

/* RE2 Em25_dm_ftl10 */
void func_80103134(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (pFp->Cont_flg) {
        Foot_break_set(pEm);
        EM_RTN(pEm) = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
        Em25_Move(pEm, pKan, pSeq);
    } else if (!(pEm->Type & 1)) {
        dm_ftl11_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    } else {
        Em25_dm_ftl12(pEm, pKan, pSeq);
    }
}

/* RE2 Em25_dm_ftl110: fall off the ceiling */
void Em25_dm_ftl110(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pEm->Routine_2 = 1;
    pEm->Spd_y = 40;
    pEm->Timer0 = 0x800;
    pFp = EF(pEm);
    pEm->Spd_x = 0;
    pEm->Spd_z = 0;
    EM_SET_MOVE(pEm, 1, 0, 3);
    pFp->Jump_flg = 1;
    pFp->Flip = 0;
    pEm->Damage_cnt |= 0x80;
    Foot_break_set(pEm);
    Em25_Set_child(pEm, 0x2004, (Rnd_l() & 1) + 1);
}

/* RE2 Em25_dm_ftl111 */
void func_801032A4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Add_speedXYZ(pEm, 0, pEm->Timer0);
    pEm->Spd_y += 40;
    if (pEm->Timer0) {
        pEm->Col.Unk26 = 0;
        pEm->Cdir_z += 0x200;
        pEm->Timer0 -= 0x200;
    }
    if (pEm->Pos_y >= pEm->Ground) {
        pEm->Col.W0 = pEm->Col.D0 = 1000;
        pEm->Pos_y = pEm->Ground;
        if (!pEm->Timer0) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 9, 0, 3);
        }
        Snd_se_enem(EF(pEm)->Hitline < 0 ? 6 : 5, pEm);
    } else if (pEm->Col.W0 < 1000) {
        pEm->Col.W0 += 50;
        if (pEm->Col.W0 >= 1000)
            pEm->Col.W0 = 1000;
        pEm->Col.D0 = pEm->Col.W0;
    }
    Joint_move(pEm, pKan, pSeq, 0x400);
}

/* RE2 Em25_dm_ftl112 */
void func_801033CC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (Joint_move(pEm, pKan, pSeq, 0x400)) {
        pFp->Jump_flg = 0;
        pFp->Place = 0;
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        pEm->Damage_cnt &= 0x7F;
    }
}

/* RE2 Em25_dm_ftl12 */
void Em25_dm_ftl12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2)
        Em25_Set_child(pEm, 0x2004, (Rnd_l() & 1) + 1);
    Em25_dm_ftl00(pEm, pKan, pSeq);
}

/* RE2 Em25_dm_ftl20 */
void func_80103490(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (pFp->Cont_flg) {
        Foot_break_set(pEm);
        EM_RTN(pEm) = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
        Em25_Move(pEm, pKan, pSeq);
    } else if (!(pEm->Type & 1)) {
        dm_ftl21_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    } else {
        Foot_break_set(pEm);
        Em25_Set_child(pEm, 0x2004, (Rnd_l() & 1) + 1);
        pFp->Em_next = 1;
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    }
}

/* RE2 Em25_dm_ftl210: fall off the wall */
void func_801035A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em25_dm_ftl110(pEm, pKan, pSeq);
    Joint_move(pEm, pKan, pSeq, 0x400);
    pEm->Spd_x = 480;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Pos_x = pEm->Col.Pos_x;
    pEm->Pos_z = pEm->Col.Pos_z;
    if (EF(pEm)->Wall_pos & 1) {
        pEm->Timer0 = 0;
        pEm->UnkDA = (EF(pEm)->Wall_pos << 10) - pEm->Cdir_y;
    } else {
        pEm->UnkDA = 0;
        pEm->Timer0 = -pEm->Cdir_x;
    }
    pEm->Damage_cnt |= 0x80;
}

/* RE2 Em25_dm_ftl211 */
void func_80103658(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em25_add_speed(pEm, pEm->Timer0, pEm->UnkDA, 0);
    pEm->Spd_x += 40;
    if (pEm->Pos_y + 400 >= pEm->Ground) {
        pEm->Cdir_x = 0;
        pEm->Cdir_z = 0;
        pEm->Cdir_y = -(EF(pEm)->Sc_dir + (EF(pEm)->Wall_pos << 10) * (EF(pEm)->Wall_pos & 1));
        pEm->Pos_y = pEm->Ground;
        pEm->Routine_2 = 2;
        EM_SET_MOVE(pEm, 9, 0, 3);
        Joint_move(pEm, pKan, pSeq, 0x400);
        pEm->Col.W0 = pEm->Col.D0 = 1000;
        pEm->Col.Unk26 = 0;
        EF(pEm)->Kage_routine = 0;
        Snd_se_enem(EF(pEm)->Hitline < 0 ? 6 : 5, pEm);
    }
}

/* RE2 Em25_dm_combo */
void func_8010374C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG no;

    if (!pEm->Routine_2) {
        Set_blood(pEm, hitmark_tbl00[Rnd_l() & 3], 3);
        no = Rnd_l() & 7;
        if (!((EF(pEm)->Foot_flg >> no) & 1))
            Set_blood(pEm, no * 2 + 3, 3);
    }
    Em25_dm_nml(pEm, pKan, pSeq);
}

/* RE2 Em25_dm_gnml */
void func_801037FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    ULONG i, d0;

    pEm->Routine_1 = 7;
    Em25_dm_ftl(pEm, pKan, pSeq);
    for (i = 0; i < 2 - ((pEm->Damage_flg & 0xC) >> 2); i++) {
        d0 = (Rnd_l() & 0xF) + 3;
        pP_ptr = &pEm->pSin_parts_ptr[d0];
        if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20))
            Set_blood(pEm, d0, 5);
    }
}

/* RE2 Em25_dm_fire */
void func_801038C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    ULONG i, d0;

    if (!(pEm->Status_flg & 1)) {
        for (i = 0; i < 5; i++) {
            d0 = (Rnd_l() & 0xF) + 3;
            pP_ptr = &pEm->pSin_parts_ptr[d0];
            if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20))
                Set_blood(pEm, d0, 5);
        }
    }
    pEm->Status_flg |= 1;
    for (i = 0; i < 20; i++) {
        pP_ptr = &pEm->pSin_parts_ptr[i];
        pP_ptr->UnkA2 &= ~0x40;
    }
    pEm->Routine_1 = 2;
    Em25_dm_nml(pEm, pKan, pSeq);
}

/* RE2 Em25_dm_H2SO4 */
void func_801039E0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    EM25_FREE *pFp;
    ULONG base, no;

    pFp = EF(pEm);
    no = Rnd_l() & 7;
    base = no * 2 + 3;
    if (pFp->Foot_cnt >= 3 && !((pFp->Foot_flg >> no) & 1)) {
        pFp->Foot_flg |= 1 << no;
        pFp->Foot_cnt--;
        pP_ptr = &pEm->pSin_parts_ptr[base] + 1;
        pP_ptr->Poly_rgb = 0x101F3F;
        pP_ptr->Old_x = pEm->Cdir_y + 0x800;
        pP_ptr->Timer3 = 0;
        pP_ptr->Old_z = 100;
        pP_ptr->Old_y = 0;
        pP_ptr->Timer1 = 30;
    }
    pP_ptr = &pEm->pSin_parts_ptr[base];
    pP_ptr->Poly_rgb = 0x102F4F;
    if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20))
        Set_blood(pEm, base, 7);
    pEm->Routine_1 = 2;
    Em25_dm_nml(pEm, pKan, pSeq);
}

/* RE3 damage (Em25_dm[8]): legs frozen, model fades (Unk178) */
void func_80103B30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    ULONG base;

    if (!(pEm->Status_flg & 2)) {
        pEm->Add_col[2] = -0x1000;
        pEm->Add_col[0] = 0;
        pEm->Add_col[1] = 0;
        pEm->Col_flg = 0x40;
        pEm->Be_flg |= 8;
        pP_ptr = &pEm->pSin_parts_ptr[0];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[1];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[2];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[3];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[4];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[5];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[6];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[11];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[12];
        pP_ptr->UnkA2 |= 0x40;
        pP_ptr = &pEm->pSin_parts_ptr[13];
        pP_ptr->UnkA2 |= 0x40;
    }
    base = (Rnd_l() & 7) * 2 + 3;
    pP_ptr = &pEm->pSin_parts_ptr[base];
    if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20))
        Set_blood(pEm, base, 7);
    pEm->Routine_1 = 7;
    pEm->Status_flg |= 2;
    Em25_dm_ftl(pEm, pKan, pSeq);
}

/* RE2 Em25_dm_spark */
void func_80103D30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EF(pEm)->Spark_cnt = 20;
    Set_blood(pEm, 0, 10);
    pEm->Routine_1 = 7;
    Em25_dm_ftl(pEm, pKan, pSeq);
}

/* RE2 Em25_dm_fire2 (pFp is only set when the burn starts, as in the original) */
void func_80103D94(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    EM25_FREE *pFp;
    ULONG i, d0;

    if (!(pEm->Status_flg & 1)) {
        pFp = EF(pEm);
        if (!pFp->No_fire) {
            for (i = 0; i < 5; i++) {
                d0 = (Rnd_l() & 0xF) + 3;
                pP_ptr = &pEm->pSin_parts_ptr[d0];
                if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20))
                    Set_blood(pEm, d0, 5);
            }
            pFp->No_fire = 30;
        }
    }
    pEm->Status_flg |= 1;
    EM_RTN(pEm) = (pFp->Rno3_bak << 24) + ((pFp->Rno2_bak << 16) + (pFp->Rno1_bak << 8) + 1);
    Em25_Move(pEm, pKan, pSeq);
}

/* RE2 Em25_Die */
void func_80103EB8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2)
        EF(pEm)->Dmg_type = pEm->Damage_no;
    if (!pEm->Water)
        pEm->Water = Water_ck(pEm->Pos_x, pEm->Pos_z);
    em25_die_tbl[pEm->Damage_no - 1](pEm, pKan, pSeq);
    Em25_float_move(pEm);
    Em_dead_flg_on_cand(pEm);
}

/* RE2 Em25_Die_nml */
void Em25_Die_nml(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!EF(pEm)->Die_weak)
        die_nml_tbl[EF(pEm)->Place](pEm, pKan, pSeq);
    else
        Em25_Die_nml30(pEm, pKan, pSeq);
}

/* RE2 Em25_Die_nml00 */
void func_80103FC4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 0xC, 0, 7);
        if (EF(pEm)->Dmg_type != 1)
            Set_blood(pEm, 0, 0);
        pEm->Col.Id = 0;
    }
    Joint_move(pEm, pKan, pSeq, 0x200);
    if (*pEm->pNow_seq & 0x100) {
        if (pEm->Water) {
            switch (EF(pEm)->Dmg_type) {
            case 9:
            case 10:
            case 16:
                Set_blood(pEm, 0, 7);
            }
            EF(pEm)->Hitline = -1;
            if (!EF(pEm)->Water_rno) {
                EF(pEm)->Water_rno = 3;
                EF(pEm)->Unk27 = 1;
            }
        }
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        if (!pFp->No_redie) {
            pEm->Life = 1;
            pFp->Die_weak = 1;
            pEm->Damage_cnt &= 0x7F;
        }
    }
}

/* RE2 Em25_Die_nml10 */
void Em25_Die_nml10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    die_nml10_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em25_Die_nml100 */
void func_80104144(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    pEm->Routine_2 = 1;
    pEm->Spd_x = 0;
    pEm->Spd_y = 40;
    pEm->Spd_z = 0;
    pEm->Cdir_z = 0;
    pEm->Col.Unk24 = 0;
    pEm->Col.Unk26 = 0;
    pEm->Col.Unk28 = 0;
    pEm->Pos_y += 800;
    EM_SET_MOVE(pEm, 0xC, 9, 0);
    if (EF(pEm)->Dmg_type != 1)
        Set_blood(pEm, 0, 0);
}

/* RE2 Em25_Die_nml101 */
void func_801041B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    Joint_move(pEm, pKan, pSeq, 0x200);
    Add_speedXYZ(pEm, 0, 0);
    pEm->Spd_y += 40;
    if (Ck_land(pEm, 0, 0, 0)) {
        pEm->Routine_2 = 2;
        pFp->Place = 0;
        pEm->Col.Id = 0;
        Snd_se_enem(EF(pEm)->Hitline < 0 ? 6 : 5, pEm);
        Em25_Die_nml1010(pEm);
        if (pFp->Hitline < 0) {
            switch (pFp->Dmg_type) {
            case 9:
            case 10:
            case 16:
                Set_blood(pEm, 0, 7);
                break;
            }
        }
    } else if (pEm->Col.W0 < 1000) {
        pEm->Col.W0 += 50;
        if (pEm->Col.W0 >= 1000)
            pEm->Col.W0 = 1000;
        pEm->Col.D0 = pEm->Col.W0;
    }
}

/* RE2 Em25_Die_nml1010 (RE3 dropped the splash effects; the positions are still built) */
void Em25_Die_nml1010(ENEMY_WORK *pEm)
{
    PARTS_W *pP_ptr;
    SVECTOR Sv;
    ULONG i;

    if (EF(pEm)->Hitline <= 0) {
        pP_ptr = pEm->pSin_parts_ptr;
        for (i = 0; i < 8; i++) {
            Sv.vx = pP_ptr->Workm.t[0] + shibuki_tbl[i].vx;
            Sv.vy = pEm->Water;
            Sv.vz = pP_ptr->Workm.t[2] + shibuki_tbl[i].vz;
        }
        EF(pEm)->Water_rno = 1;
        EF(pEm)->Unk27 = 1;
    }
}

/* RE2 Em25_Die_nml102 */
void func_80104340(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Joint_move(pEm, pKan, pSeq, 0x400);
    if (*pEm->pNow_seq & 0x100) {
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        if (!EF(pEm)->No_redie) {
            pEm->Life = 1;
            EF(pEm)->Die_weak = 1;
            pEm->Damage_cnt &= 0x7F;
        }
    }
}

/* RE2 Em25_Die_nml20 */
void func_801043AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    die_nml20_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em25_Die_nml200 */
void func_801043E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    pEm->Routine_2 = 1;
    pEm->Timer0 = 0x800;
    EM_SET_MOVE(pEm, 1, 9, 0);
    Joint_move(pEm, pKan, pSeq, 0x400);
    EF(pEm)->Jump_flg = 1;
    EF(pEm)->Flip = 0;
    pEm->Spd_x = 480;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Pos_x = pEm->Col.Pos_x;
    pEm->Pos_z = pEm->Col.Pos_z;
    if (EF(pEm)->Wall_pos & 1) {
        pEm->Timer0 = 0;
        pEm->UnkDA = (EF(pEm)->Wall_pos << 10) - pEm->Cdir_y;
    } else {
        pEm->UnkDA = 0;
        pEm->Timer0 = -pEm->Cdir_x;
    }
    if (EF(pEm)->Dmg_type != 1)
        Set_blood(pEm, 0, 0);
}

/* RE2 Em25_Die_nml201 */
void func_801044AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    Em25_add_speed(pEm, pEm->Timer0, pEm->UnkDA, 0);
    pEm->Spd_x += 40;
    if (Ck_land(pEm, 400, 0, 0)) {
        pEm->Cdir_x = 0;
        pEm->Cdir_z = 0;
        pEm->Cdir_y = -(EF(pEm)->Sc_dir + (EF(pEm)->Wall_pos << 10) * (EF(pEm)->Wall_pos & 1));
        pEm->Routine_2 = 2;
        pFp->Place = 0;
        pEm->Col.Id = 0;
        EM_SET_MOVE(pEm, 0xC, 9, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Col.W0 = pEm->Col.D0 = 1000;
        pEm->Col.Unk26 = 0;
        pFp->Kage_routine = 0;
        Snd_se_enem(EF(pEm)->Hitline < 0 ? 6 : 5, pEm);
        Em25_Die_nml1010(pEm);
        if (pFp->Hitline < 0) {
            switch (pFp->Dmg_type) {
            case 9:
            case 10:
            case 16:
                Set_blood(pEm, 0, 7);
                break;
            }
        }
    }
}

/* RE2 Em25_Die_nml30 */
void Em25_Die_nml30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM_SET_MOVE(pEm, 0xD, 0, 7);
    Joint_move(pEm, pKan, pSeq, 0x200);
    Em25_Set_child(pEm, 0x2002, (Rnd_l() & 3) + 1);
    Set_blood(pEm, 0, 0);
    EM_SET_RTN(pEm, 7, 0, 0, 0);
    pEm->Col.Id = 0;
    if (EF(pEm)->Hitline < 0)
        pEm->Pos_y = pEm->Water + 800;
}

/* RE2 Em25_Die_ftl */
void Em25_Die_ftl(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!EF(pEm)->Die_weak)
        die_ftl_tbl[EF(pEm)->Place](pEm, pKan, pSeq);
    else
        Em25_Die_ftl30(pEm, pKan, pSeq);
}

/* RE2 Em25_Die_ftl00 */
void func_801046D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Col.Id = 0;
        EF(pEm)->Hitline = 1;
        EM_SET_MOVE(pEm, 0xC, 0, 7);
        Set_blood(pEm, 19, 1);
        Die_break_set(pEm);
        Em25_Set_child(pEm, 0x2002, (Rnd_l() & 3) + 6);
    }
    Joint_move(pEm, pKan, pSeq, 0x200);
    if (*pEm->pNow_seq & 0x100) {
        if (pEm->Water) {
            switch (EF(pEm)->Dmg_type) {
            case 9:
            case 10:
            case 16:
                Set_blood(pEm, 0, 7);
            }
            EF(pEm)->Hitline = -1;
            if (!EF(pEm)->Water_rno) {
                EF(pEm)->Water_rno = 3;
                EF(pEm)->Unk27 = 1;
            }
        }
        EM_SET_RTN(pEm, 7, 0, 0, 0);
    }
}

/* RE2 Em25_Die_ftl10 */
void func_80104804(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        Set_blood(pEm, 19, 1);
        Die_break_set(pEm);
        Em25_Set_child(pEm, 0x2004, (Rnd_l() & 3) + 6);
    }
    Em25_Die_nml10(pEm, pKan, pSeq);
}

/* RE2 Em25_Die_ftl20 */
void func_80104888(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    die_ftl20_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* RE2 Em25_Die_ftl200 */
void func_801048C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    pEm->Routine_2 = 1;
    pEm->Timer0 = 0x800;
    EM_SET_MOVE(pEm, 1, 9, 0);
    Joint_move(pEm, pKan, pSeq, 0x400);
    EF(pEm)->Jump_flg = 1;
    EF(pEm)->Flip = 0;
    pEm->Spd_x = 480;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Pos_x = pEm->Col.Pos_x;
    pEm->Pos_z = pEm->Col.Pos_z;
    if (EF(pEm)->Wall_pos & 1) {
        pEm->Timer0 = 0;
        pEm->UnkDA = (EF(pEm)->Wall_pos << 10) - pEm->Cdir_y;
    } else {
        pEm->UnkDA = 0;
        pEm->Timer0 = -pEm->Cdir_x;
    }
    Die_break_set(pEm);
    Set_blood(pEm, 19, 1);
    Em25_Set_child(pEm, 0x2004, (Rnd_l() & 3) + 6);
}

/* RE2 Em25_Die_ftl30 */
void Em25_Die_ftl30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM_SET_MOVE(pEm, 0xD, 0, 7);
    Joint_move(pEm, pKan, pSeq, 0x200);
    Em25_Set_child(pEm, 0x2002, (Rnd_l() & 3) + 6);
    Set_blood(pEm, 19, 1);
    Die_break_set(pEm);
    pEm->Col.Id = 0;
    EM_SET_RTN(pEm, 7, 0, 0, 0);
    if (EF(pEm)->Hitline < 0)
        pEm->Pos_y = pEm->Water + 800;
}

/* RE2 Die_break_set: the head (part 19) flies off */
void Die_break_set(ENEMY_WORK *pEm)
{
    ULONG i;
    PARTS_W *pP_ptr;

    pEm->Neck_flg |= 1;
    pP_ptr = &pEm->pSin_parts_ptr[19];
    pP_ptr->UnkA2 |= 0x20;
    pP_ptr->Be_flg |= 2;
    PW_FREE(pP_ptr)[0] = pEm->Cdir_y + 0x800;
    PW_FREE(pP_ptr)[4] = 0;
    PW_FREE(pP_ptr)[2] = 0;
    PW_FREE(pP_ptr)[1] = -240;
    PW_FREE(pP_ptr)[3] = 15;
    PW_FREE(pP_ptr)[6] = 0;
    pP_ptr->Poly_rgb = 0x2F2F2F;
    Set_blood(pEm, 19, 1);
    for (i = 0; i < 4; i++)
        Set_blood(pEm, 19, 2);
}

/* RE2 Em25_Die_gnml */
void func_80104AE4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    ULONG i, d0, d1;

    if (!pEm->Routine_2 && !EF(pEm)->Die_weak) {
        d1 = pEm->Water ? 5 : 4;
        Set_blood(pEm, 0, d1);
        for (i = 0; i < 3; i++) {
            d0 = (Rnd_l() & 0xF) + 3;
            pP_ptr = &pEm->pSin_parts_ptr[d0];
            if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20)) {
                Set_blood(pEm, d0, d1);
                Set_blood(pEm, d0, 5);
            }
        }
    }
    Em25_Die_ftl(pEm, pKan, pSeq);
}

/* RE2 Em25_Die_fire */
void func_80104C0C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    ULONG i, d0, d1;

    if (!pEm->Routine_2 && !EF(pEm)->Die_weak) {
        d1 = pEm->Water ? 5 : 4;
        Set_blood(pEm, 0, d1);
        for (i = 0; i < 4; i++) {
            d0 = (Rnd_l() & 0xF) + 3;
            pP_ptr = &pEm->pSin_parts_ptr[d0];
            if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20)) {
                Set_blood(pEm, d0, d1);
                Set_blood(pEm, d0, 5);
            }
        }
        Chg_modelcol(pEm, 0x202F2F);
    }
    for (i = 0; i < 20; i++) {
        pP_ptr = &pEm->pSin_parts_ptr[i];
        pP_ptr->UnkA2 &= ~0x40;
    }
    Em25_Die_nml(pEm, pKan, pSeq);
}

/* RE2 Em25_Die_H2SO4 */
void func_80104D74(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2 && !EF(pEm)->Die_weak) {
        Chg_modelcol(pEm, 0x101F3F);
        Set_blood(pEm, 19, 7);
        Set_blood(pEm, 0, 6);
        Set_blood(pEm, 1, 6);
        EF(pEm)->No_redie = 1;
    }
    Em25_Die_nml(pEm, pKan, pSeq);
}

/* RE3 death (em25_die_tbl[8]), counterpart of func_80103B30 */
void func_80104E18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    ULONG base;

    pEm->Col_flg = 0x40;
    pEm->Be_flg |= 8;
    pP_ptr = &pEm->pSin_parts_ptr[0];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[1];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[4];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[6];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[8];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[10];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[12];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[14];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[15];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[17];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[18];
    pP_ptr->UnkA2 |= 0x80;
    pP_ptr = &pEm->pSin_parts_ptr[19];
    pP_ptr->UnkA2 |= 0x80;
    base = (Rnd_l() & 7) * 2 + 3;
    pP_ptr = &pEm->pSin_parts_ptr[base];
    if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20))
        Set_blood(pEm, base, 7);
    Em25_Die_nml(pEm, pKan, pSeq);
}

/* RE2 Em25_Die_spark */
void func_80105010(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!EF(pEm)->Die_weak) {
        Chg_modelcol(pEm, 0x3F3F3F);
        EF(pEm)->Spark_cnt = 30;
        Set_blood(pEm, 19, 10);
        Set_blood(pEm, 0, 10);
    }
    pEm->Routine_1 = 2;
    Em25_Die_nml(pEm, pKan, pSeq);
}

/* RE2 Em25_Scenario */
void func_801050A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* RE2 Em25_Dead (RE3 sinks the body into the water) */
void func_801050A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_1) {
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_1 = 1;
            pEm->UnkE2 = (Rnd_l() & 0x1F) + 30;
        }
    } else if (!--pEm->UnkE2) {
        EM_SET_MOVE(pEm, 0xD, 0, 7);
        pEm->Routine_1 = 0;
    }
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        EF(pEm)->Kage_routine = 2;
        pEm->UnkDA = 0;
    }
    if (EF(pEm)->Hitline < 0) {
        Em25_float_move(pEm);
        switch (pEm->Routine_3) {
        case 0:
            pEm->Routine_3 = 1;
            pEm->Spd_y = 160;
        case 1:
            pEm->Pos_y += pEm->Spd_y;
            pEm->Spd_y -= 40;
            if (pEm->Spd_y < -160)
                pEm->Routine_3 = 2;
            break;
        case 2:
            pEm->Pos_y = pEm->Water + (SHORT)(float_fy[(pEm->UnkDA++ & 0x3F) >> 3] + 800);
            break;
        }
    }
}

/* RE2 Em25_kage_move */
void Em25_kage_move(ENEMY_WORK *pEm)
{
    KAGE_WORK *pKage;

    pKage = pEm->Kage.pKw;
    switch (EF(pEm)->Kage_routine) {
    case 0:
        if (!Water_ck(pEm->Pos_x, pEm->Pos_z)) {
            pEm->Be_flg &= ~0x20;
            pEm->Ground = Sca_get_ground(&pEm->Col.Pos_x, 800, 0x200, 8);
            break;
        }
    case 1:
        pEm->Be_flg |= 0x20;
        break;
    case 2:
        if (Water_ck(pEm->Pos_x, pEm->Pos_z))
            break;
        EF(pEm)->Kage_routine = 3;
        *(ULONG *)&pKage->Prim[0].r0 = (*(ULONG *)&pKage->Prim[0].r0 & 0xFF000000) | 0xBF407F;
        *(ULONG *)&pKage->Prim[1].r0 = (*(ULONG *)&pKage->Prim[1].r0 & 0xFF000000) | 0xBF407F;
        pEm->UnkE3 = 90;
        pEm->Be_flg &= ~0x20;
    case 3:
        pKage->Half_x += 8;
        pKage->Half_z += 8;
        if (!--pEm->UnkE3)
            EF(pEm)->Kage_routine = 4;
    case 4:
        break;
    }
}

/* RE2 Em25_float_move */
void Em25_float_move(ENEMY_WORK *pEm)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (!pFp->Water_rno)
        return;
    if (!pEm->Water) {
        pFp->Hitline = 1;
        pFp->Unk27 = 0;
        pEm->Pos_y = pEm->Ground;
        return;
    }
    switch (pFp->Water_rno) {
    case 1:
        pFp->Water_rno = 2;
        pEm->Spd_y = 140;
        pFp->Unk2C = 0;
    case 2:
        pFp->Unk2C += pEm->Spd_y;
        pEm->Pos_y = pEm->Water + 800 + pFp->Unk2C;
        pEm->Spd_y -= 15;
        if (pEm->Spd_y <= 0) {
            if (pEm->Spd_y < -70)
                pEm->Spd_y = -70;
            if (pEm->Pos_y <= pEm->Water + 800) {
                pEm->Pos_y = pEm->Water + 800;
                pFp->Water_rno = 3;
            }
        } else if (pEm->Pos_y >= pEm->Water) {
            pEm->Pos_y = pEm->Water;
            pEm->Spd_y = 0;
        }
        break;
    case 3:
        pEm->Pos_y = pEm->Water + (SHORT)(float_fy[(pEm->UnkDA++ & 0x3F) >> 3] + 800);
    }
}

/* RE2 Em25_foot_set */
void Em25_foot_set(ENEMY_WORK *pEm, UCHAR Flg)
{
    PARTS_W *pP_ptr;
    UCHAR no;
    MATRIX Tm;
    SHORT dx, dz;

    pP_ptr = pEm->pSin_parts_ptr;
    RotMatrix(CDIR_SV(pEm), (MATRIX *)pEm->Workm_m);
    CompM((MATRIX *)pEm->Workm_m, PW_M(pP_ptr), &Tm);
    pP_ptr += (Flg << 3) + 4;
    no = Flg << 2;
    if ((EF(pEm)->Foot_flg >> no) & 1) {
        pEm->Spd_x = 0;
    } else {
        CompM(&Tm, (MATRIX *)pP_ptr[-1].M, &Tm);
        CompM(&Tm, PW_M(pP_ptr), &Tm);
        dx = Tm.t[0] - pP_ptr->Workm.t[0];
        dz = Tm.t[2] - pP_ptr->Workm.t[2];
        pEm->Spd_x = SquareRoot0(dx * dx + dz * dz);
    }
}

/* RE2 Em25_goto00 */
SHORT Em25_goto00(ENEMY_WORK *pEm, SHORT rr)
{
    ENEMY_WORK *pTg;
    LONG x, z;

    pTg = EF(pEm)->Target_pl ? G.pEnemy[1] : &Pl_work;
    if (!EF(pEm)->Place || (EF(pEm)->Place == 1 && pTg->nFloor != pEm->nFloor)) {
        return Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, rr);
    } else {
        x = pTg->Pos_x - pEm->Pos_x;
        z = pTg->Pos_z - pEm->Pos_z;
        return Goto00_ck(pEm, pEm->Pos_x - x, pEm->Pos_z - z, rr);
    }
}

/* RE2 Ck_lock_wall (RE3: only the shape hit by Sca_ck_em) */
ULONG Ck_lock_wall(ENEMY_WORK *pEm)
{
    SCA_DATA *pSc;
    EM25_FREE *pFp;
    USHORT pos;
    USHORT dir;

    pSc = pEm->Col.pSca_hit_data;
    pFp = EF(pEm);
    if (pSc) {
        pos = func_800502B8(POS_V(pEm), pSc);
        pFp->pWall = pSc;
        pFp->Sc_dir = pos << 10;
        dir = (pEm->Cdir_y + 0x200) & 0xC00;
        if (pFp->Sc_dir == dir && SCA_FLOOR(pFp->pWall) >= -pEm->Pos_y / 1800)
            return pos + 1;
    }
    return 0;
}

/* RE2 Em25_wall_rot */
void Em25_wall_rot(ENEMY_WORK *pEm, SHORT rot)
{
    EM25_FREE *pFp;

    pFp = EF(pEm);
    if (pFp->Wall_pos & 1) {
        pEm->Cdir_y += rot;
    } else {
        if (!(pFp->Wall_pos & 2))
            rot = -rot;
        pEm->Cdir_x += rot;
    }
    pFp->Sc_dir += rot;
}

/* RE2 Em25_add_speed */
void Em25_add_speed(ENEMY_WORK *pEm, LONG rx, LONG ry, LONG rz)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = pEm->Cdir_x + rx;
    Sv.vy = pEm->Cdir_y + ry;
    Sv.vz = pEm->Cdir_z + rz;
    RotMatrix(&Sv, &M);
    ApplyMatrixSV(&M, EM_SPD_SV(pEm), &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_y += Sv.vy;
    pEm->Pos_z += Sv.vz;
}

/* RE2 Ck_wall_edge */
LONG Ck_wall_edge(ENEMY_WORK *pEm, SHORT Scdir)
{
    SCA_DATA *pSc;
    SHORT pos;
    LONG d;

    pSc = EF(pEm)->pWall;
    pos = EF(pEm)->Wall_pos;
    if (pos & 1) {
        Scdir += (pos << 10) - 0x400;
        if (Scdir == 0x400) {
            d = Sca_get_pos(2, pSc);
            return pEm->Pos_x < d + 2500;
        } else {
            d = Sca_get_pos(0, pSc);
            return d - 2500 < pEm->Pos_x;
        }
    } else {
        if (Scdir == 0x400) {
            d = Sca_get_pos(3, pSc);
            return d - 2500 < pEm->Pos_z;
        } else {
            d = Sca_get_pos(1, pSc);
            return pEm->Pos_z < d + 2500;
        }
    }
}

/* RE2 Ck_near_wall */
LONG Ck_near_wall(ENEMY_WORK *pEm, SCA_DATA *pSc, SHORT dir)
{
    LONG edge;
    ENEMY_WORK *pTg;

    pTg = EF(pEm)->Target_pl ? G.pEnemy[1] : &Pl_work;
    edge = Sca_get_pos(dir, pSc);
    if (dir & 1) {
        if (__builtin_abs(pTg->Pos_z - edge) < 3000)
            return 1;
    } else {
        if (__builtin_abs(pTg->Pos_x - edge) < 3000)
            return 1;
    }
    return 0;
}

/* RE2 Ck_wall_other (RE3 scans G.Sca_list only when Sca_ck_em hit nothing) */
LONG Ck_wall_other(ENEMY_WORK *pEm, SHORT r)
{
    SCA_DATA **ppScd;

    if (!pEm->Col.pSca_hit_data) {
        ppScd = G.Sca_list;
        while (*ppScd) {
            if (*ppScd != EF(pEm)->pWall) {
                if (SCA_FLOOR(*ppScd++) >= -pEm->Pos_y / 1800)
                    return 1;
            } else {
                ppScd++;
            }
        }
    }
    return 0;
}

/* RE2 Ck_wall_to_pl */
SHORT Ck_wall_to_pl(ENEMY_WORK *pEm, SHORT rot)
{
    SHORT xx, yy, zz;
    ENEMY_WORK *pTg;
    SHORT d;

    pTg = EF(pEm)->Target_pl ? G.pEnemy[1] : &Pl_work;
    xx = pEm->Cdir_x;
    yy = pEm->Cdir_y;
    zz = pEm->Cdir_z;
    pEm->Cdir_x = 0;
    pEm->Cdir_y = (EF(pEm)->Wall_pos << 10) & 0xFFF;
    pEm->Cdir_z = 0;
    d = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, rot);
    pEm->Cdir_x = xx;
    pEm->Cdir_y = yy;
    pEm->Cdir_z = zz;
    return d;
}

/* RE2 Set_blood */
void Set_blood(ENEMY_WORK *pEm, UCHAR parts, UCHAR type)
{
    PARTS_W *pP_ptr;
    SVECTOR Sv, Sv2;
    EM25_BLOOD_TBL *tbl;
    USHORT dir;
    LONG Id;
    USHORT Clut;
    MATRIX M;

    tbl = &blood_id_tbl[type & 0x7F];
    dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Pos_x, pEm->Pos_z);
    pP_ptr = pEm->pSin_parts_ptr;
    pP_ptr += parts;
    Clut = tbl->Clut;
    Id = tbl->Id & 0x7F;
    if (Clut == 0xFF)
        Clut = (pEm->Damage_flg & 0x200) ? 6 : 1;
    if (tbl->Id & 0x80) {
        Sv.vx = 0;
        Sv.vy = -100;
        Sv.vz = 0;
        Esp_call((((UCHAR)tbl->Clut & 0xF) << 24) | (tbl->Type << 8) | Id, (tbl->Zoom << 16) | dir,
                      &pP_ptr->Workm, &Sv, NULL);
    } else {
        Sv.vx = 0;
        Sv.vy = dir;
        Sv.vz = 0;
        RotMatrix(&Sv, &M);
        Sv2.vx = Sv2.vy = Sv2.vz = 0;
        switch (EF(pEm)->Place) {
        case 1:
            Sv2.vy = 800;
            break;
        case 2:
        case 3:
            Sv2.vx = 400;
            break;
        }
        ApplyMatrixSV(&M, &Sv2, &Sv);
        Sv.vx += pP_ptr->Workm.t[0];
        Sv.vy += pP_ptr->Workm.t[1];
        Sv.vz += pP_ptr->Workm.t[2];
        if (type & 0x80)
            Sv.vy = pEm->Water;
        Esp_call(((Clut & 0xF) << 24) | (tbl->Type << 8) | Id, (tbl->Zoom << 16) | dir,
                      &GsIDMATRIX, &Sv, NULL);
    }
}

/* RE2 Set_poison */
void Set_poison(ENEMY_WORK *pEm, ULONG Type, SHORT Dir)
{
    PARTS_W *pP_ptr;
    SVECTOR sv, sv2;
    MATRIX M;

    Type &= 0xF2;
    pP_ptr = pEm->pSin_parts_ptr;
    if (!(Type & 0x80)) {
        sv.vx = 200;
        sv.vy = 0;
        sv.vz = -100;
        Esp_call((Type << 8) | 0x1A, 0x10000000 | (USHORT)Dir, &pP_ptr[1].Workm, &sv, NULL);
    } else {
        sv.vx = pEm->Cdir_x;
        sv.vy = pEm->Cdir_y;
        sv.vz = pEm->Cdir_z;
        RotMatrix(&sv, &M);
        sv2.vx = 1000;
        sv2.vy = 0;
        sv2.vz = -100;
        ApplyMatrixSV(&M, &sv2, &sv);
        sv.vx += pP_ptr->Workm.t[0];
        sv.vy += pP_ptr->Workm.t[1];
        sv.vz += pP_ptr->Workm.t[2];
        Esp_call(((Type & 0x7F) << 8) | 0x1A, 0x10000000 | (USHORT)Dir, &GsIDMATRIX, &sv, NULL);
    }
}

/* RE2 atck_ck_main (dist is left unset for the sub player, as in the original) */
ULONG atck_ck_main(ENEMY_WORK *pEm, ULONG pl, LONG R)
{
    ENEMY_WORK *pTg;
    LONG dist;

    if (pl) {
        pTg = G.pEnemy[1];
    } else {
        pTg = &Pl_work;
        dist = pEm->L_pl;
    }
    if (dist < R) {
        if (!(pTg->Damage_cnt & 0x80)) {
            if (Pl_life_down(20, 0) != 2)
                Em25_Pl_to_dmg(pl, Cdir_ck(pEm, pTg));
            else
                Em25_Pl_to_die(pl);
            return 1;
        }
    }
    return 0;
}

/* RE2 Em25_atck_ck */
ULONG Em25_atck_ck(ENEMY_WORK *pEm, LONG R, LONG H)
{
    if (__builtin_abs(pEm->Pos_y - G.Player.Pos_y) < H) {
        if (atck_ck_main(pEm, 0, R)) {
            Snd_se_enem(1, pEm);
            Pad_vib_set0(2, 0);
            Pad_vib_set1(8, 200, 0);
            return 1;
        }
    }
    if ((LONG)G.Status_flg < 0) {
        if (__builtin_abs(pEm->Pos_y - G.pEnemy[1]->Pos_y) < H) {
            if (atck_ck_main(pEm, 1, R)) {
                Snd_se_enem(1, pEm);
                return 1;
            }
        }
    }
    return 0;
}

/* RE2 Foot_break_set: a leg (two parts) flies off */
LONG Foot_break_set(ENEMY_WORK *pEm)
{
    PARTS_W *pP_ptr;
    EM25_FREE *pFp;
    ULONG i, base, no;

    pFp = EF(pEm);
    if (pFp->Dmg_type == 14)
        return 1;
    no = Rnd_l() & 7;
    if (pFp->Foot_cnt >= 3 && !((pFp->Foot_flg >> no) & 1)) {
        pFp->Foot_flg |= 1 << no;
        pFp->Foot_cnt--;
        base = no * 2 + 3;
        for (i = base; i <= base + 1; i++) {
            pP_ptr = &pEm->pSin_parts_ptr[i];
            pP_ptr->UnkA2 |= 0x20;
            pP_ptr->Be_flg |= 2;
            PW_FREE(pP_ptr)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP_ptr)[4] = 0;
            PW_FREE(pP_ptr)[2] = 600;
            PW_FREE(pP_ptr)[1] = -120;
            PW_FREE(pP_ptr)[3] = 10;
            PW_FREE(pP_ptr)[6] = -100;
            pP_ptr->Poly_rgb = 0x2F2F2F;
        }
        Set_blood(pEm, base, 1);
        return 0;
    }
    Set_blood(pEm, 19, 1);
    return 1;
}

/* RE2 Em25_Set_child: RE3 wakes up placeholder baby spiders (id 0x26, Type 0x6000) */
void Em25_Set_child(ENEMY_WORK *pEm, USHORT type, UCHAR cnt)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pWk;

    ppEm = &G.pEnemy[0];
    while (cnt--) {
        if (!(ppEm = Get_child_work(ppEm)))
            return;
        pWk = *ppEm;
        pWk->Type = type | (pEm->Water != 0);
        pWk->Pos_x = pWk->Old_x = pEm->Col.Pos_x;
        pWk->Pos_y = pWk->Old_x = pEm->Col.Pos_y;
        pWk->Pos_z = pWk->Old_x = pEm->Col.Pos_z;
        pWk->nFloor = pEm->nFloor;
        pWk->Life = 1;
        if (EF(pEm)->Place < 2)
            pWk->Cdir_y = Rnd_l() << 4;
        else
            pWk->Cdir_y = (EF(pEm)->Wall_pos << 10) - (0x200 - (Rnd_l() << 2));
        EM_RTN(pWk) = (child_rtn_tbl[(UCHAR)pWk->Type] << 8) | 1;
        switch ((UCHAR)pWk->Type) {
        case 2:
        case 3:
            pWk->Be_flg = 0x405001;
            pWk->nFloor = pWk->Pos_y / -1800;
            break;
        case 4:
        case 5:
            pWk->Be_flg = 0x405001;
            pWk->Spd_x = 200;
            break;
        }
        if (Water_ck(pWk->Pos_x, pWk->Pos_z))
            pWk->Be_flg |= 0x20;
    }
}

/* RE3: next unused baby spider work (id 0x26, Type 0x6000) after ppEm */
ENEMY_WORK **Get_child_work(ENEMY_WORK **ppEm)
{
    do {
        ppEm++;
        if ((*ppEm)->Id == 0x26 && (*ppEm)->Type == 0x6000)
            return ppEm;
    } while (ppEm != G.ppEnemy_end);
    return NULL;
}

/* RE2 Em25_Pl_to_dmg */
void Em25_Pl_to_dmg(ULONG pl, UCHAR dir)
{
    ENEMY_WORK *pPl;

    pPl = pl ? G.pEnemy[1] : &Pl_work;
    pPl->Routine_0 = 2;
    pPl->Routine_1 = dir + 2;
    pPl->Routine_2 = 0;
    pPl->Routine_3 = 0;
}

/* RE2 Em25_Pl_to_die */
void Em25_Pl_to_die(ULONG pl)
{
    ENEMY_WORK *pPl;

    pPl = pl ? G.pEnemy[1] : &Pl_work;
    pPl->Routine_0 = 3;
    pPl->Routine_1 = 0;
    pPl->Routine_2 = 0;
    pPl->Routine_3 = 0;
}

/* RE2 Get_target_pl */
ENEMY_WORK *Get_target_pl(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pTg;

    pTg = G_PLAYER;
    EF(pEm)->Target_pl = 0;
    if ((LONG)G.Status_flg < 0) {
        pTg = G.pEnemy[1];
        EF(pEm)->Target_pl = 1;
    }
    return pTg;
}

/* RE2 Set_foot_se */
void Set_foot_se(ENEMY_WORK *pEm)
{
    if (*pEm->pNow_seq & 0x200) {
        if (!pEm->Water)
            Snd_se_enem(foot_se_tbl[EF(pEm)->Foot_se], pEm);
        else
            Snd_se_enem(10, pEm);
        EF(pEm)->Foot_se = (EF(pEm)->Foot_se + 1) % 3;
    }
}

/* RE2 Chg_modelcol */
void Chg_modelcol(ENEMY_WORK *pEm, ULONG Rgb)
{
    PARTS_W *pP_ptr;
    ULONG i;

    for (i = 0; i < 20; i++) {
        pP_ptr = &pEm->pSin_parts_ptr[i];
        pP_ptr->Poly_rgb = Rgb;
    }
}

/* RE2 Ck_land */
LONG Ck_land(ENEMY_WORK *pEm, LONG Fe, LONG Fg, LONG Fw)
{
    if (pEm->Water < pEm->Ground) {
        if (pEm->Pos_y + Fe < pEm->Water)
            return 0;
        pEm->Pos_y = pEm->Water + Fw;
        EF(pEm)->Hitline = -1;
        return -1;
    } else if (pEm->Pos_y + Fe >= pEm->Ground) {
        pEm->Pos_y = pEm->Ground + Fg;
        EF(pEm)->Hitline = 1;
        return 1;
    }
    return 0;
}

/* RE2 Em25_spark_move */
void func_80106578(ENEMY_WORK *pEm)
{
    PARTS_W *pP_ptr;
    ULONG i, d0;

    if (!EF(pEm)->Spark_cnt)
        return;
    if (--EF(pEm)->Spark_cnt & 1) {
        for (i = 0; i < 3; i++) {
            d0 = biripos_tbl[Rnd_l() & 7];
            pP_ptr = &pEm->pSin_parts_ptr[d0];
            if ((pP_ptr->Be_flg & 1) && !(pP_ptr->UnkA2 & 0x20))
                Set_blood(pEm, d0, 9);
        }
    }
}

/* RE2 Set_gunrange: RE3 removed the Be_flg updates and left the empty tests */
void func_80106640(ENEMY_WORK *pEm)
{
    LONG dh;

    switch (EF(pEm)->Place) {
    case 0:
        break;
    case 1:
        break;
    case 2:
    case 3:
        dh = Pl_work.Pos_y - pEm->Pos_y;
        if (dh > 4200) {
        } else {
            if (dh < 2100) {
            }
        }
    }
}

#include "enemy/common/em20/Ck_em_id.inc.c"

/* RE2 Sca_get_pos (SCA.C, module copy): coordinate of one side of the shape */
LONG Sca_get_pos(LONG no, SCA_DATA *pSc)
{
    switch (no) {
    case 0:
        return pSc->X1;
    case 1:
        return pSc->Z0;
    case 2:
        return pSc->X0;
    case 3:
        return pSc->Z1;
    }
}

/* RE3 (unused): ray test from the head part along Cdir against the room's box shapes;
   with Dist2 a second ray is cast 90 degrees aside. Leaves the last shape tested in pSca_hit_data. */
LONG func_80106744(ENEMY_WORK *pEm, SHORT Dist, LONG Dist2)
{
    VECTOR P0;
    VECTOR P1;
    SVECTOR Sv;
    SVECTOR Sv2;
    MATRIX M;
    MATRIX M2;
    SCA_DATA *pSc;
    SCA_DATA *pEnd;
    VECTOR *pPos;
    LONG Hit;
    ULONG Mask;

    pSc = (SCA_DATA *)G.pRoom->pSca;
    pPos = WORKM_POS_V(pEm->pSin_parts_ptr);
    Sv.vx = pEm->Cdir_x;
    Sv.vz = pEm->Cdir_z;
    Sv.vy = pEm->Cdir_y;
    RotMatrix_gte(&Sv, &M);
    Sv2.vx = Dist;
    Sv2.vz = 0;
    Sv2.vy = 0;
    ApplyMatrixSV(&M, &Sv2, &Sv);
    P0.vx = Sv.vx += pEm->pSin_parts_ptr->Workm.t[0];
    P0.vy = Sv.vy += pEm->pSin_parts_ptr->Workm.t[1];
    P0.vz = Sv.vz += pEm->pSin_parts_ptr->Workm.t[2];
    Hit = 0;
    Mask = 0xFFFFFFFF;
    pEnd = pSc + SCA_HEAD_NUM(pSc);
    for (pSc++; pSc != pEnd && !Hit; pSc++) {
        if ((pSc->Id & Mask) && (pSc->Id & 0xF) == 1)
            Hit = func_8004F258(((pPos->vx >> 1) << 16) | ((pPos->vz >> 1) & 0xFFFF),
                                ((P0.vx >> 1) << 16) | ((P0.vz >> 1) & 0xFFFF), pSc);
    }
    pEm->Col.pSca_hit_data = --pSc;
    if (Dist2 && !Hit) {
        Sv.vx = 0;
        Sv.vz = 800;
        Sv.vy = 0;
        P1.vx = P0.vx;
        P1.vy = P0.vy;
        P1.vz = P0.vz;
        RotMatrix_gte(&Sv, &M2);
        MulMatrix2(&M, &M2);
        Sv2.vx = Dist2;
        Sv2.vz = 0;
        Sv2.vy = 0;
        ApplyMatrixSV(&M2, &Sv2, &Sv);
        P1.vx += Sv.vx;
        P1.vy += Sv.vy;
        P1.vz += Sv.vz;
        pSc = (SCA_DATA *)G.pRoom->pSca;
        pEnd = pSc + SCA_HEAD_NUM(pSc);
        for (pSc++; pSc != pEnd && !Hit; pSc++) {
            if ((pSc->Id & Mask) && (pSc->Id & 0xF) == 1)
                Hit = func_8004F258(((P0.vx >> 1) << 16) | ((P0.vz >> 1) & 0xFFFF),
                                    ((P1.vx >> 1) << 16) | ((P1.vz >> 1) & 0xFFFF), pSc);
        }
        pEm->Col.pSca_hit_data = --pSc;
    }
    return Hit;
}
