#include "enemy/em26.h"
#include "joint.h"
#include "sce_at.h"
#include "snd.h"
#include "sld.h"

LONG func_8004D720_(LONG *pPos, ULONG W, ULONG a2, ULONG a3, LONG Pos_y) __asm__("Sca_get_ground"); /* Capcom passes a fifth argument (unprototyped call) */

/* RE2 EM_MAIN: per-frame entry of the young spider: routine dispatch, collision */
void Em26_main(ENEMY_WORK *pEm)
{
    if (!(G.Stop_flg & 0x20000000)) {
        if (pEm->Damage_cnt & 0x7F)
            pEm->Damage_cnt--;
        Em26_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    }
}

/* RE2 Em26_Init; Type 0 spawns a swarm of 16 copies */
void Em26_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG i;
    ENEMY_WORK *pCp;
    COL_OBJ *pCol;

    pEm->Life = 1;
    pEm->Be_flg |= 0x400000;
    pEm->Unk144 = 0;
    for (i = 0; i < 12; i++)
        pEm->Unk1CC[i] = 0;
    EM_SET_MOVE(pEm, 0, 0, 0xF);
    Joint_move(pEm, pKan, pSeq, 0x400);
    pEm->Neck_flg = 0;
    pEm->Neck_no = 0;
    EM26_W(pEm)->Kasakasarot = 0x78;
    EM26_W(pEm)->No_catch_cnt = Rnd() & 0x1F;
    pCol = &pEm->Col;
    EM26_W(pEm)->pTarget_pl = &Pl_work;
    pEm->Hit_area.Top = 300;
    pEm->Hit_area.Bottom = -300;
    pEm->Hit_area.R = 300;
    pEm->Hit_area.Rate0 = 0x50;
    pEm->Hit_area.Rate1 = 0xA0;
    pEm->Damage_cnt = 6;
    EM26_W(pEm)->Catch_wait = 0;
    pEm->Unk140 = 0;
    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Cdir_x = 0;
    pEm->Cdir_z = 0;
    pEm->Timer0 = 0;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pCol->pEm = pEm;
    pCol->W0 = pEm->Col.D0 = 30;
    pCol->H = 100;
    pCol->Type = 2;
    pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    EM_RTN(pEm) = (Em26_routine_tbl[(UCHAR)pEm->Type] << 8) + 1;
    if (Water_ck(pEm->Pos_x, pEm->Pos_z))
        pEm->Be_flg |= 0x20;
    switch ((UCHAR)pEm->Type) {
    case 0:
        pEm->Life = -1;
        pEm->Be_flg |= 0x7030;
        pEm->Type |= 0x4000;
        for (i = 0; i < 16; i++) {
            pCp = Em_work_add(pEm);
            EM_SET_MOVE(pEm, 0, 0, 0xF);
            Joint_move(pCp, pKan, pSeq, 0x400);
            pCol = &pCp->Col;
            pCp->Hit_area.Bottom = -300;
            pCp->Hit_area.Rate0 = 0x50;
            pCp->Hit_area.Rate1 = 0xA0;
            pCp->Hit_area.Top = 300;
            pCp->Hit_area.R = 300;
            pCp->Unk140 = 0;
            pCp->Hit_area.pM = &pCp->pSin_parts_ptr->Workm;
            pCol->pEm = pCp;
            pCol->W0 = pCp->Col.D0 = 30;
            pCol->H = 100;
            pCol->Unk24 = 0;
            pCol->Unk28 = 0;
            pCol->Unk26 = 0;
            pCol->Flg = 0;
            pCol->Type = 2;
            Oba_set(pCol, 0);
            Kage_work_set(&pCp->Kage.pKw, 0, 0xC800C8, 0, &pCp->Pos_x);
        }
        break;
    case 1:
        break;
    case 2:
    case 3:
        pEm->Be_flg |= 0x404000;
        break;
    case 4:
    case 5:
        pEm->Spd_x = 200;
        pEm->Be_flg |= 0x404000;
        break;
    }
    Oba_set(&pEm->Col, 0);
    Kage_work_set(&pEm->Kage.pKw, 0, 0xC800C8, 0, &pEm->Pos_x);
}

/* RE2 Em26_Move: move routines by Routine_1 (Em26_move_tbl) */
void Em26_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em26_move_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em26_Mv_dummy: standing still */
void Em26_Mv_dummy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Joint_move(pEm, pKan, pSeq, 0x400);
}

/* RE2 Em26_Mv_walk: on the floor: wait / run (Em26_mv_walk_tbl), catch the player when close */
void Em26_Mv_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pTg;
    LONG Dir0;
    LONG Dist;

    Em26_mv_walk_tbl[pEm->Routine_2](pEm, pKan, pSeq);
    pTg = EM26_W(pEm)->pTarget_pl;
    Dir0 = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);
    if (EM26_W(pEm)->No_catch_cnt)
        EM26_W(pEm)->No_catch_cnt--;
    if (EM26_W(pEm)->Catch_wait)
        EM26_W(pEm)->Catch_wait--;
    Dist = (pTg == G_PLAYER) ? pEm->L_pl : pEm->Unk158;
    if (Dist < 2000 && !Dir0)
        if (!EM26_W(pEm)->Catch_wait && !EM26_W(pEm)->No_catch_cnt && !(pTg->Damage_cnt & 0x7F)) {
            pEm->Routine_1 = 3;
            EM_RTN23(pEm) = 0;
        }
    pTg = G_PLAYER;
    if ((LONG)pEm->L_pl < 550 && (G.Room_player_flg & 6) && pEm->nFloor == pTg->nFloor) {
        if (EM_RTN01(pTg) == 0x101 || EM_RTN01(pTg) == 0x201 || EM_RTN01(pTg) == 0x301 || EM_RTN01(pTg) == 0x401) {
            EM_SET_RTN(pEm, 3, 0, 0, 0);
            Pad_vib_set0(1, 0);
            Pad_vib_set1(3, 80, 0);
        }
    }
    if ((LONG)G.Status_flg < 0) {
        pTg = G.pEnemy[1];
        if (pEm->nFloor == pTg->nFloor)
            if ((pEm->Unk158 & 0x3FFFFFFF) < 550)
                if (EM_RTN01(pTg) == 0x101 || EM_RTN01(pTg) == 0x201 || EM_RTN01(pTg) == 0x301 || EM_RTN01(pTg) == 0x401)
                    EM_SET_RTN(pEm, 3, 0, 0, 0);
    }
}

/* RE2 Em26_Mv_walk00: wait */
void Em26_Mv_walk00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Timer0 = (Rnd() + 0x1E) & (EM26_W(pEm)->Atck_flg * 30 + 0x3F);
        EM26_W(pEm)->Atck_flg = 0;
        EM26_W(pEm)->Chase_rot = ((0x1111 >> (Rnd() & 0xF)) & 1) ? -0x40 : 0x40;
        if ((LONG)G.Status_flg < 0) {
            if ((LONG)pEm->L_pl < (pEm->Unk158 & 0x3FFFFFFF))
                EM26_W(pEm)->pTarget_pl = G_PLAYER;
            else
                EM26_W(pEm)->pTarget_pl = G.pEnemy[1];
        }
    case 1:
        if (--pEm->Timer0 == -1)
            EM_RTN23(pEm) = 1;
    }
    if (!Get_sc_hit(pEm)) {
        pEm->Routine_1 = 2;
        EM_RTN23(pEm) = 0;
    }
}

/* RE2 Em26_Mv_walk10: run at the target */
void Em26_Mv_walk10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pTg;

    pTg = EM26_W(pEm)->pTarget_pl;
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Spd_x = (Rnd() & 0x3F) + 0x5A;
        pEm->Spd_z = 0;
        pEm->Timer0 = Rnd() & 0x2E;
    case 1:
        Goto00(pEm, pTg->Pos_x, pTg->Pos_z, EM26_W(pEm)->Chase_rot);
        pEm->Cdir_y += EM26_W(pEm)->Kasakasarot;
        EM26_W(pEm)->Kasakasarot *= -1;
        if (--pEm->Timer0 == -1)
            EM_RTN23(pEm) = 0;
    }
    Add_speedXZ(pEm, EM26_W(pEm)->Kasakasarot);
    if (Water_ck(pEm->Pos_x, pEm->Pos_z)) {
        pEm->Pos_x = pEm->Old_x;
        pEm->Pos_y = pEm->Old_y;
        pEm->Pos_z = pEm->Old_z;
    } else if (!Get_sc_hit(pEm)) {
        pEm->Routine_1 = 2;
        EM_RTN23(pEm) = 0;
    }
}

/* RE2 Em26_Mv_fall: falling to the floor below */
void Em26_Mv_fall(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Hit;
    SHORT Ground;

    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Spd_y = 40;
        pEm->Be_flg |= 0x4000;
        Ground = func_8004D720_(&pEm->Pos_x, pEm->Col.W0, 0x200, 8, pEm->Pos_y);
        pEm->Ground = Ground;
        EM26_W(pEm)->Jump_flg = 1;
        pEm->nFloor = Ground / -1800;
    }
    Add_speedXYZ(pEm, 0, 0);
    if (pEm->Spd_y < 800)
        pEm->Spd_y += 40;
    else
        pEm->Spd_y = 800;
    pEm->Damage_cnt = 0xFF;
    Hit = Get_sc_hit(pEm);
    if (Hit) {
        pEm->Damage_cnt = 0;
        EM26_W(pEm)->Jump_flg = 0;
        if (Hit < 0) {
            pEm->Pos_y = Hit + 10;
            pEm->Routine_1 = 4;
            Set_blood(pEm, 0, 0x82);
            pEm->Be_flg |= 0x20;
        } else {
            pEm->Routine_1 = 1;
            pEm->Pos_y = pEm->Ground;
            pEm->Be_flg &= ~0x20;
        }
        EM_RTN23(pEm) = 0;
    }
}

/* RE2 Em26_Mv_jump: jumping at the player */
void Em26_Mv_jump(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Hit;
    EM26_FREE *pFp;

    pFp = EM26_W(pEm);
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Spd_x = (Rnd() & 0x3F) + 0x5A;
        pEm->Spd_y = -300;
        pFp->Atck_flg = 0;
        pFp->Jump_flg = 1;
    }
    Add_speedXYZ(pEm, 0, 0);
    pEm->Spd_y += 40;
    if (!Em26_atck_ck(pEm, 0))
        if ((LONG)G.Status_flg < 0)
            Em26_atck_ck(pEm, 1);
    Hit = Get_sc_hit(pEm);
    if (Hit) {
        pFp->Jump_flg = 0;
        EM26_W(pEm)->Catch_wait = (Rnd() & 0x3F) + 0x5A;
        if (Hit < 0) {
            Set_blood(pEm, 0, 0x82);
            pEm->Life = -1;
            pEm->Pos_y = Hit + 10;
            EM_SET_RTN(pEm, 3, 0, 0, 0);
            pEm->Be_flg |= 0x20;
        } else {
            pEm->Routine_1 = 1;
            EM_RTN23(pEm) = 0;
            pFp->No_catch_cnt = (Rnd() & 0x1F) + 0x1E;
            pEm->Be_flg &= ~0x20;
            pEm->Pos_y = pEm->Ground;
        }
    }
}

/* RE2 Em26_Mv_swim: swimming on the water */
void Em26_Mv_swim(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pTg;
    LONG Dir0;
    LONG Dist;

    pTg = EM26_W(pEm)->pTarget_pl;
    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pEm->Timer0 = 450;
        pEm->UnkDA = (Rnd() & 1) ? 0x20 : -0x20;
        pEm->UnkE2 = Rnd() & 0x1F;
    }
    Dir0 = Goto00_ck(pEm, pTg->Pos_x, pTg->Pos_z, 0x100);
    pEm->Cdir_y += EM26_W(pEm)->Kasakasarot + pEm->UnkDA;
    EM26_W(pEm)->Kasakasarot *= -1;
    Joint_move(pEm, pKan, pSeq, 0x400);
    if (!(++pEm->UnkE2 & 0xF)) {
        Set_blood(pEm, 0, 0x82);
        pEm->UnkE2 += Rnd() & 7;
    }
    Dist = (EM26_W(pEm)->pTarget_pl == G_PLAYER) ? pEm->L_pl : pEm->Unk158;
    if (Dist < 2000 && !Dir0)
        if (!(pTg->Damage_cnt & 0x7F)) {
            pEm->Routine_1 = 3;
            EM_RTN23(pEm) = 0;
        }
    pEm->Pos_y = pEm->Water + 10;
    if (Get_sc_hit(pEm) >= 0) {
        pEm->Routine_1 = 1;
        EM_RTN23(pEm) = 0;
    } else if (!--pEm->Timer0) {
        pEm->Life = -1;
        EM_SET_RTN(pEm, 3, 0, 0, 0);
    }
}

/* RE2 Em26_Damage: empty */
void Em26_Damage(void) {
}

/* RE2 Em26_dm00: no reaction (sets Routine_2 only) */
void Em26_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
    case 1:
        break;
    case 2:
        return;
    }
}

/* RE2 Em26_dm01: empty */
void Em26_dm01(void) {
}

/* RE2 Em26_dm02: empty */
void Em26_dm02(void) {
}

/* RE2 Em26_Die: death on the floor (Em26_Die00) or sinking in the water (Em26_Die10) */
void Em26_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (Water_ck(pEm->Pos_x, pEm->Pos_z) >= pEm->Ground || EM26_W(pEm)->Jump_flg)
        Em26_Die00(pEm, pKan, pSeq);
    else
        Em26_Die10(pEm, pKan, pSeq);
}

/* RE2 Em26_Die00: squashed, blood by the damage kind */
void Em26_Die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pKage;

    if (!pEm->Routine_2) {
        pEm->Routine_2 = 1;
        pKage = pEm->Kage.pKw;
        if (!EM26_W(pEm)->Jump_flg) {
            *(ULONG *)&pKage->Prim[0].r0 &= 0xFF000000;
            *(ULONG *)&pKage->Prim[0].r0 |= 0xBF10BF;
            *(ULONG *)&pKage->Prim[1].r0 &= 0xFF000000;
            *(ULONG *)&pKage->Prim[1].r0 |= 0xBF10BF;
        } else {
            pEm->Be_flg |= 0x20;
        }
        Snd_se_enem(7, pEm);
        Set_blood(pEm, 0, Em26_dieblood_type_tbl[pEm->Routine_1 - 1]);
        pEm->Col.Id = 0;
        pEm->pSin_parts_ptr->Be_flg = 0;
    } else {
        KILL_ENEMY_WORK(pEm);
    }
}

/* RE2 Em26_Die10: sink */
void Em26_Die10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    pEm->Be_flg |= 0x4000;
    pEm->Pos_y += 30;
    pEm->Cdir_y += 0x20;
    pEm->Cdir_z += 0x40;
    Joint_move(pEm, pKan, pSeq, 0x400);
    if (pEm->Pos_y >= pEm->Ground || pEm->Pos_y < pEm->Water)
        KILL_ENEMY_WORK(pEm);
}

/* RE2 Em26_Scenario: empty */
void Em26_Scenario(void) {
}

/* RE2 Em26_Dead: empty */
void Em26_Dead(void) {
}

/* RE2 PLDAMAGE_EM: empty */
void Pldamege_Em26(void) {
}

/* RE2 Em26_atck_ck: bite the player (Pl 0) or the partner (Pl 1) */
ULONG Em26_atck_ck(ENEMY_WORK *pEm, ULONG Pl)
{
    ULONG Dist;
    ENEMY_WORK *pPl;
    EM26_FREE *pFp;

    pFp = EM26_W(pEm);
    if (!Pl) {
        pPl = &Pl_work;
        Dist = pEm->L_pl;
    } else {
        pPl = G.pEnemy[1];
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    }
    if (Dist < pPl->Col.W0 + 40 && (ULONG)(pPl->Pos_y - pEm->Pos_y) < 3000 && !pFp->Atck_flg) {
        if (!pPl->Damage_cnt) {
            if (!Pl) {
                Em26_Pl_to_dmg(pPl, Cdir_ck(pEm, pPl));
                Pl_life_down(1, 1);
                Pad_vib_set0(3, 0);
            }
            Set_blood(pEm, 0, 1);
            pFp->Atck_flg = 1;
            pEm->Spd_x = 0x5A;
            pEm->Cdir_y += 0x800;
            return 1;
        }
    }
    return 0;
}

/* RE2 Set_blood: blood effect of kind type at part parts of pTg (Em26_blood_id_tbl) */
void Set_blood(ENEMY_WORK *pTg, UCHAR Parts, UCHAR Type)
{
    PARTS_W *pP;
    SVECTOR Sv;
    LONG Dir;
    EM26_BLOOD_TBL *pTbl;

    pTbl = &Em26_blood_id_tbl[Type & 0x7F];
    Dir = pTg->Cdir_y + pTbl->Dir;
    pP = pTg->pSin_parts_ptr;
    pP += Parts;
    Sv.vx = pP->Workm.t[0];
    Sv.vy = pP->Workm.t[1];
    if ((Type & 0x80) && pTg->Water < pTg->Ground)
        Sv.vy = pTg->Water + 10;
    Sv.vz = pP->Workm.t[2];
    Esp_call(((pTbl->Flg & 0xF) << 24) | (pTbl->Type << 8) | pTbl->Id, (pTbl->Zoom << 16) | (USHORT)Dir, &GsIDMATRIX, &Sv, NULL);
}

/* RE2 Em26_Pl_to_dmg (without the Damage_cnt flag) */
void Em26_Pl_to_dmg(ENEMY_WORK *pPl, UCHAR Dir)
{
    pPl->Routine_0 = 2;
    pPl->Routine_1 = Dir;
    pPl->Routine_2 = 0;
    pPl->Routine_3 = 0;
}

/* RE2 Get_sc_hit: wall collision of the young spider */
LONG Get_sc_hit(ENEMY_WORK *pEm)
{
    LONG Wat;

    Wat = Water_ck(pEm->Pos_x, pEm->Pos_z);
    if (pEm->Pos_y >= pEm->Ground)
        return 1;
    if (pEm->Pos_y - 10 >= Wat)
        return Wat;
    return 0;
}
