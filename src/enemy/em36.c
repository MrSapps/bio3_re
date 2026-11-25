#include "enemy/em36.h"
#include "door.h"
#include "root_ck.h"
#include "sabun.h"
#include "sca.h"
#include "snd.h"

/* Capcom's declarations in this module differ from the exe's: the sabun call Sa_morph_step is
   passed a third argument, Cdir_ck returns int (no andi after the call), and Dir_pos_ck takes int
   directions (no narrowing at the call) */
void func_8004A934_(ENEMY_WORK *pEm, PARTS_W *pP, LONG a2) __asm__("Sa_morph_step");
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");
LONG Dir_pos_ck_l(LONG *pPos, LONG *pTg, LONG Hed_dir, LONG Hani_dir) __asm__("Dir_pos_ck");

/* per-frame entry (RE2 Em12a / EM_MAIN) */
void Em36_main(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR *pTv;
    LONG Dist;
    PARTS_W *pP;
    LONG Spd;
    ULONG Se;
    USHORT Seq;
    USHORT Rno;
    LONG Route;
    VECTOR *pV;
    LONG Unused[2];

    pTv = POS_V(pPl);
    if (EM36_W(pEm)->Lost_time)
        pTv = &Em36_lost_pos;
    if ((UCHAR)pEm->Type == 8 || (UCHAR)pEm->Type == 12)
        pTv = &Em36_route_pos[EM36_W(pEm)->Route_no];
    if (EM36_W(pEm)->Flg & 0x1000 && *(SHORT *)&pEm->Routine_0 != 0x601 && EM36_W(pEm)->Dm_stage < 3) {
        Route = pEm->Unk14E & 0xF;
        pTv = &Em36_route[Route][EM36_W(pEm)->Route_pt];
    }
    Root_ck(pEm, pTv, 0, 0);
    EM36_W(pEm)->Flg &= ~0x800;
    pEm->Pos_y -= 400;
    pPl->Pos_y -= 400;
    if (!Sca_ck_line(POS_V(pEm), POS_V(pPl), 0x40, 1))
        EM36_W(pEm)->Flg |= 0x800;
    pEm->Pos_y += 400;
    pPl->Pos_y += 400;
    if (*(SHORT *)&pPl->Routine_0 == 0x701)
        EM36_W(pEm)->Flg |= 0x800;
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (!(pEm->Type & 1))
        Em36_dodge(pEm);
    Em36_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if (G.Unk783E == 4 && G.Unk7840 == 9) {
        Rno = pEm->Unk14E;
        if (Rno & 0x80) {
            if (!(EM36_W(pEm)->Flg & 0x1000)) {
                Route = Rno & 0xF;
                pV = &Em36_route[Route][0];
                EM36_W(pEm)->Route_dist2 = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
                if (EM36_W(pEm)->Route_dist2 < 3000) {
                    EM36_W(pEm)->Route_pt = 1;
                    EM36_W(pEm)->Flg |= 0x1000;
                    pV = &Em36_route[Route][1];
                    Dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
                    pV = &Em36_route[Route][2];
                    if (SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z)) < Dist)
                        EM36_W(pEm)->Route_pt = 2;
                }
            }
        } else {
            EM36_W(pEm)->Flg &= ~0x1000;
        }
    }
    Em36_body_morph(pEm);
    Em36_mouth_morph(pEm);
    Em36_head_shake(pEm);
    if (pEm->Type & 1) {
        Em36_wound_fx(pEm);
        if (EM36_W(pEm)->Dm_stage >= 2)
            Em36_wound_fx2(pEm);
        if (EM36_W(pEm)->Dm_stage >= 3)
            Em36_wound_fx3(pEm);
    }
    if ((LONG)pEm->L_pl < 3000) {
        if (EM36_W(pEm)->Sweep_timer)
            EM36_W(pEm)->Sweep_timer--;
    } else {
        EM36_W(pEm)->Sweep_timer = 120;
    }
    if (pEm->Be_flg & 8) {
        if (pEm->Add_col[2] > 0) {
            pEm->Add_col[2] -= 24;
            if (pEm->Add_col[2] < 0)
                pEm->Add_col[2] = 0;
        }
        if (pEm->Add_col[0] > 0) {
            pEm->Add_col[0] -= 24;
            if (pEm->Add_col[0] < 0)
                pEm->Add_col[0] = 0;
        }
        if (pEm->Add_col[2] < 0) {
            pEm->Add_col[2] += 24;
            if (pEm->Add_col[2] > 0)
                pEm->Add_col[2] = 0;
        }
        if (pEm->Add_col[0] < 0) {
            pEm->Add_col[0] += 24;
            if (pEm->Add_col[0] > 0)
                pEm->Add_col[0] = 0;
        }
        if (pEm->Add_col[0] == 0 && pEm->Add_col[2] == 0) {
            pEm->Be_flg &= ~8;
            pP = &pEm->pSin_parts_ptr[0];
            pP->UnkA2 &= ~0x40;
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 &= ~0x40;
        }
    }
    if (EM36_W(pEm)->Punch_timer)
        EM36_W(pEm)->Punch_timer--;
    if (EM36_W(pEm)->Mv12_timer)
        EM36_W(pEm)->Mv12_timer--;
    if (EM36_W(pEm)->Grab_timer)
        EM36_W(pEm)->Grab_timer--;
    if (EM36_W(pEm)->Lunge_timer)
        EM36_W(pEm)->Lunge_timer--;
    if (EM36_W(pEm)->Run_timer)
        EM36_W(pEm)->Run_timer--;
    Hit_area_parts_set(pEm, 0xFE700002, 0x190000C);
    pEm->pEnemy_ptr = pPl;
    Rot_neck_em(pEm, pEm->Cdir_y);
    Spd = 0x200;
    if (pEm->Routine_0 == 7 || EM_RTN(pEm) == 0x30801)
        Spd = 0;
    pP = &pEm->pSin_parts_ptr[1];
    EM36_SABUN_P(pP)->Key[0].Rate += EM36_W(pEm)->Morph_spd;
    if (EM36_SABUN_P(pP)->Key[0].Rate >= 0x1000)
        EM36_W(pEm)->Morph_spd = -Spd;
    if (EM36_SABUN_P(pP)->Key[0].Rate <= 0)
        EM36_W(pEm)->Morph_spd = Spd;
    func_8004A934_(pEm, pP, Spd);
    Seq = *pEm->pNow_seq;
    if (Seq & 0xF000) {
        Se = (Seq >> 12) - 1;
        if ((UCHAR)pEm->Type == 3 && Se - 11 < 4)
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        else
            Snd_se_enem((USHORT)Se, pEm);
    }
}

INCLUDE_ASM("asm/nonmatchings/enemy/em36", Em36_Init);

/* Routine_0 1: move routine set by Type */
void Em36_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_Move_type[(UCHAR)pEm->Type](pEm, pKan, pSeq);
}

/* brain and motion routines by Routine_1 (Type 0) */
void Em36_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_mv_br00[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* brain and motion routines by Routine_1 (Type 1) */
void Em36_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_mv_br01[pEm->Routine_1](pEm);
    Em36_mv01[pEm->Routine_1](pEm, pKan, pSeq);
    if ((EM36_FLGW(pEm) & 0x2001) == 0x2000 && !(pEm->Damage_cnt & 0x80)) {
        pEm->Damage_no = 30;
        EM_SET_RTN(pEm, 2, 0, 0, 0);
        pEm->Damage_cnt |= 0x80;
        EM36_W(pEm)->Flg &= ~3;
    }
}

/* brain and motion routines by Routine_1 (Type 2) */
void Em36_Move_type02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    pEm->Damage_cnt |= 0x80;
    pEm->Status_flg &= ~0x2000;
    Em36_mv_br02[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* brain and motion routines by Routine_1 (Type 3) */
void Em36_Move_type03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = G_PLAYER;

    Em36_mv_br03[pEm->Routine_1](pEm);
    Em36_mv03[pEm->Routine_1](pEm, pKan, pSeq);
    if (G.Unk75E8 & 5) {
        if (!EM36_W(pEm)->Lost_time)
            EM36_W(pEm)->Lost_time = 240;
        Em36_lost_pos.vx = pPl->Pos_x;
        Em36_lost_pos.vy = pPl->Pos_y;
        Em36_lost_pos.vz = pPl->Pos_z;
    }
    pEm->Type &= ~0x800;
    if (EM36_W(pEm)->Lost_time) {
        pEm->Type |= 0x800;
        EM36_W(pEm)->Lost_dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);
    }
    if ((EM36_FLGW(pEm) & 0x2001) == 0x2000 && !(pEm->Damage_cnt & 0x80)) {
        pEm->Damage_no = 30;
        EM_SET_RTN(pEm, 4, 0xA, 0, 0);
        pEm->Life = -1;
        pEm->Damage_cnt |= 0x80;
        EM36_W(pEm)->Flg &= ~3;
        Em_dead_flg_on_cand(pEm);
    }
}

/* brain and motion routines by Routine_1 (Type 6) */
void Em36_Move_type06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];

    EM36_W(pEm)->Route_dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
    Em36_mv_br00[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
    if (EM36_W(pEm)->Type_timer) {
        if (--EM36_W(pEm)->Type_timer == 0) {
            EM36_W(pEm)->Type_timer = 450;
            pEm->Type += 2;
        }
        if ((LONG)pEm->L_pl < 4000)
            EM36_W(pEm)->Type_timer++;
    }
}

/* brain and motion routines by Routine_1 (Type 8) */
void Em36_Move_type08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];

    EM36_W(pEm)->Route_dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
    Em36_mv_br04[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
    if (EM36_W(pEm)->Type_timer) {
        EM36_W(pEm)->Type_timer--;
        if ((LONG)pEm->L_pl < 3000)
            EM36_W(pEm)->Type_timer -= 3;
        if (pEm->Damage_cnt == 1)
            EM36_W(pEm)->Type_timer -= 10;
        if ((LONG)pEm->L_pl > 5000)
            EM36_W(pEm)->Type_timer++;
        if (EM36_W(pEm)->Type_timer <= 0) {
            EM36_W(pEm)->Type_timer = 300;
            pEm->Type -= 2;
        }
    }
}

/* brain and motion routines by Routine_1 (Type 10) */
void Em36_Move_type10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];

    EM36_W(pEm)->Route_dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
    Em36_mv_br00[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* brain and motion routines by Routine_1 (Type 12) */
void Em36_Move_type12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];

    EM36_W(pEm)->Route_dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
    Em36_mv_br05[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* brain: stand */
void Em36_stand_br(ENEMY_WORK *pEm)
{
    ULONG Dist = pEm->L_pl;
    SHORT Dir = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x400);

    if (Dist < 3000 && Dir) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
    if (!Dir && pEm->Status_flg & 0x2000) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
}

/* motion: stand */
void Em36_stand(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
    }
}

/* brain: walk (RE2 Em2a_brain) */
void Em36_walk_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;
    SHORT Dir, Dir_far, Dir_eye;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Dir_far = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Dir_eye = Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 8000 && !Dir && pEm->Status_flg & 0x2000 && !EM36_W(pEm)->Run_timer)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far && EM36_W(pEm)->Flg & 0x800) {
        EM_SET_RTN(pEm, 1, 4, 0, 0);
        if (Dist < 3500 && !Dir_eye && (Rnd() & 3) == 1 && !Flg_ck(&G.Unk7888, 0x1F))
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (Dist < 1300 && !Dir)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (!EM36_W(pEm)->Punch_timer && Dist < 3000 && !Dir && !pPl->Damage_cnt) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (pPl->Life < 40 && pPl->Damage_no != 0x80 && EM36_W(pEm)->Flg & 0x800 && !Flg_ck(&G.Unk7888, 0x1E)) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM36_W(pEm)->Flg &= ~0x200;
        }
    }
    if (EM36_W(pEm)->Punch_timer < 5 && Dist < 3000 && !Dir_eye && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    if (EM36_W(pEm)->Flg & 0x800) {
        if (!EM36_W(pEm)->Lunge_timer && Dist < 4500 && Dist > 2000 && !Dir && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1E)) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM36_W(pEm)->Flg &= ~0x200;
        }
        if (!EM36_W(pEm)->Mv12_timer && Dist > 4500 && Dist < 5500 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
    if (!EM36_W(pEm)->Sweep_timer && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    if (EM36_W(pEm)->Flg & 0x10)
        EM_SET_RTN(pEm, 1, 9, 0, 0);
}

/* motion: walk */
void Em36_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd_old;
    LONG Side;

    EM36_W(pEm)->Flg &= ~3;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x1F);
        pEm->Routine_2 = 1;
        if (pEm->Spd_x > 100)
            pEm->Spd_x = 100;
        pEm->Timer0 = 300;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 5;
        if (!pEm->UnkE2) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 40);
            if ((LONG)pEm->L_pl < 3000)
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
        } else {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -40);
            pEm->UnkE2--;
        }
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (pEm->Col.Hit_flg & 0x30) {
            if (++pEm->UnkDA > 100) {
                pEm->UnkE2 = 30;
                Side = Rnd() & 1;
                pEm->Timer0 = 300;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x40 - Side * 120;
            }
        } else {
            pEm->UnkDA = 0;
        }
        if (--pEm->Timer0 == -1)
            EM_SET_RTN(pEm, 1, 2, 0, 0);
    }
}

/* brain: walk (type 1) */
void Em36_walk_br01(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;
    SHORT Dir, Dir_far, Dir_near, Dir_eye;
    VECTOR *pV;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Dir_far = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Dir_near = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Dir_eye = Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist < 1300 && !Dir)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (Dist < 6000 && Dir_far && !pEm->Hokan_flg)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (!EM36_W(pEm)->Punch_timer) {
        if (Dist < 3000 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (!EM36_W(pEm)->Punch_timer && Dist < 3500 && !Dir_eye && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (EM36_W(pEm)->Flg & 0x800) {
        if (!EM36_W(pEm)->Lunge_timer && Dist < 3500 && !Dir && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1E)) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM36_W(pEm)->Flg &= ~0x200;
        }
        if (!EM36_W(pEm)->Mv12_timer && Dist > 3500 && Dist < 4500 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
    if (!EM36_W(pEm)->Sweep_timer && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    if (!EM36_W(pEm)->Punch_timer && Dist < 9000 && Dist > 6000 && !Dir_near && !pPl->Damage_cnt && EM36_W(pEm)->Dm_stage >= 2)
        EM_SET_RTN(pEm, 1, 0x12, 0, 0);
    if (EM36_W(pEm)->Flg & 0x1000) {
        EM_SET_RTN(pEm, 1, 0x14, 0, 0);
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Route_pt];
        if (Goto00_ck(pEm, pV->vx, pV->vz, 0x400))
            pEm->Routine_2 = 2;
    }
    if (EM36_W(pEm)->Flg & 0x40)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
}

/* motion: run */
void Em36_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd_old;
    SHORT Add;
    LONG Side;

    EM36_W(pEm)->Flg &= ~3;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x21, 0, 0x3F);
        pEm->Routine_2 = 1;
        if (pEm->Spd_x > 100)
            pEm->Spd_x = 100;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        pEm->Be_flg &= ~0x8000;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 6;
        if (pEm->Spd_x > 50) {
            Add = -32;
            if (!pEm->UnkE2) {
                Add = 24;
                if (Rnd() & 1)
                    Add = 40;
                if ((LONG)pEm->L_pl < 3000) {
                    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
                    Add += 16;
                }
            } else {
                pEm->UnkE2--;
            }
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
        }
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        if (pEm->Col.Hit_flg & 0x30) {
            if (++pEm->UnkDA > 150) {
                pEm->UnkE2 = 60;
                Side = Rnd() & 1;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x40 - Side * 120;
            }
        } else {
            pEm->UnkDA = 0;
        }
    }
}

/* brain: run */
void Em36_run_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;
    SHORT Dir, Dir_far;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Dir_far = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x400, 0x264);
    if (Dist > 11000 && !Dir && pEm->Status_flg & 0x2000)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
}

/* brain: combo (next routine from Em36_combo_tbl) */
void Em36_combo_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    UCHAR Rtn;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (EM36_W(pEm)->Flg & 0x100) {
        Rtn = Em36_combo_tbl[EM36_W(pEm)->Combo_set][EM36_W(pEm)->Combo_no];
        EM36_W(pEm)->Combo_no++;
        if (!(Rtn & 0x80)) {
            EM_RTN(pEm) = (Rtn << 8) + 1;
            EM36_W(pEm)->Flg &= ~0x200;
            return;
        }
        EM36_W(pEm)->Flg &= ~0x100;
    }
}

/* motion: run to the last seen player position */
void Em36_run_lost(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd_old;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x21, 0, 0x3F);
        pEm->Routine_2 = 1;
        if (pEm->Spd_x > 100)
            pEm->Spd_x = 100;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        if (!EM36_W(pEm)->Lost_time)
            EM36_W(pEm)->Lost_time = 1;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 6;
        if (pEm->Spd_x > 50)
            Goto01(pEm, EM36_W(pEm)->Lost_dir, 24);
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        if (pEm->Col.Hit_flg & 0x20 && !EM36_W(pEm)->Lost_time) {
            if (++pEm->UnkDA > 150) {
                pEm->UnkDA = 0;
                EM36_W(pEm)->Lost_dir -= (SHORT)(Rnd() * 4 - 0xA00);
                EM36_W(pEm)->Combo_no = 0;
                EM36_W(pEm)->Flg |= 0x100;
                EM36_W(pEm)->Combo_set = Rnd() & 7;
            }
        }
        if (pEm->Col.Hit_flg & 0x10 && !EM36_W(pEm)->Lost_time) {
            if ((pEm->UnkDA += 20) > 150) {
                pEm->UnkDA = 0;
                EM36_W(pEm)->Lost_dir -= (SHORT)(Rnd() * 4 - 0xA00);
                EM36_W(pEm)->Combo_no = 0;
                EM36_W(pEm)->Flg |= 0x100;
                EM36_W(pEm)->Combo_set = Rnd() & 7;
            }
        }
        if (EM36_W(pEm)->Lost_time) {
            EM36_W(pEm)->Lost_time--;
            if (pEm->Col.Hit_flg & 0x10 || !EM36_W(pEm)->Lost_time) {
                EM36_W(pEm)->Combo_no = 0;
                EM36_W(pEm)->Flg |= 0x100;
                EM36_W(pEm)->Combo_set = Rnd() & 7;
            }
        }
    }
}

/* brain: walk along the route (type 8) */
void Em36_route_br04(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;
    ULONG Dist_pl; /* never set in the original */
    SHORT Dir_wide, Dir, Dir_far;

    Dir_wide = Goto00_ck(pEm, pV->vx, pV->vz, 0x400);
    Dir = Goto00_ck(pEm, pV->vx, pV->vz, 0x100);
    Dir_far = Goto00_ck(pEm, pV->vx, pV->vz, 0x600);
    Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 9000 && !Dir)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (!EM36_W(pEm)->Punch_timer && Dist < 2500 && !Dir)
        EM_SET_RTN(pEm, 1, 0x13, 0, 0);
    if (Dist_pl < 2000 && !Dir_wide)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
}

/* brain: walk along the route (type 12) */
void Em36_route_br05(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;
    ULONG Dist_pl; /* never set in the original */
    SHORT Dir_wide, Dir, Dir_far;

    Dir_wide = Goto00_ck(pEm, pV->vx, pV->vz, 0x400);
    Dir = Goto00_ck(pEm, pV->vx, pV->vz, 0x100);
    Dir_far = Goto00_ck(pEm, pV->vx, pV->vz, 0x600);
    Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 9000 && !Dir)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist_pl < 2000 && !Dir_wide)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
}

/* brain: attack wait */
void Em36_turn_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;
    SHORT Dir;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    if (Dist < 3000 && pEm->Status_flg & 0x2000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 5000 && EM36_W(pEm)->Flg & 0x800 && !Dir && !Flg_ck(&G.Unk7888, 0x1E)) {
        EM_SET_RTN(pEm, 1, 0xA, 0, 0);
        EM36_W(pEm)->Flg |= 0x200;
    }
}

/* mv_br[2] (Type 2): walk again when the player is within 3000 */
void Em36_turn_br02(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    if (Dist < 3000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* mv_br[2] (route types): walk again when the waypoint is within 6000 */
void Em36_turn_br04(ENEMY_WORK *pEm)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;

    Goto00_ck(pEm, pV->vx, pV->vz, 800);
    Goto00_ck(pEm, pV->vx, pV->vz, 0x200);
    if (Dist < 6000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* motion: turn */
void Em36_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd_old;
    SHORT Add;
    LONG Side;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 0xF);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 130;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 4;
        Add = 96;
        if (pEm->UnkE2) {
            Add = -96;
            pEm->UnkE2--;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
        if (pEm->Col.Hit_flg & 0x30) {
            if (++pEm->UnkDA > 90) {
                pEm->UnkE2 = 10;
                Side = Rnd() & 1;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x60 - Side * 192;
            }
        } else {
            pEm->UnkDA = 0;
        }
        Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        if (--pEm->Timer0 == -1)
            EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
}

/* brain: turn */
void Em36_back_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;
    SHORT Dir, Dir_atk, Dir_eye;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir_atk = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Dir_eye = Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 3000 && !Dir)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist > 5000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 4000 && Dir) {
        EM_SET_RTN(pEm, 1, 4, 0, 0);
        if (Dist < 3000 && !Dir_eye && (Rnd() & 3) == 1 && !Flg_ck(&G.Unk7888, 0x1F))
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (!EM36_W(pEm)->Punch_timer && Dist < 3000 && !Dir_atk && !pPl->Damage_cnt)
        EM_SET_RTN(pEm, 1, 5, 0, 0);
    if (!EM36_W(pEm)->Lunge_timer && Dist < 4500 && !Dir_atk && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1E) && EM36_W(pEm)->Flg & 0x800) {
        EM_SET_RTN(pEm, 1, 0xA, 0, 0);
        EM36_W(pEm)->Flg &= ~0x200;
    }
}

/* mv_br[3] (route types): walk again when the waypoint is over 3000 away */
void Em36_back_br04(ENEMY_WORK *pEm)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;

    Goto00_ck(pEm, pV->vx, pV->vz, 800);
    Goto00_ck(pEm, pV->vx, pV->vz, 0x200);
    if (Dist > 3000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* motion: walk back */
void Em36_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd_old;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 90;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 5;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 32);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (--pEm->Timer0 == -1)
            EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
}

/* no brain step for this move */
void Em36_turn_around_br(ENEMY_WORK *pEm)
{
}

/* motion: turn around */
void Em36_turn_around(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 7);
        if (pEm->Type & 1)
            EM_SET_MOVE(pEm, 0x24, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        EM36_W(pEm)->Flg |= 3;
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, -32);
        Spd_get(pEm, pKan, pSeq, 0);
        Add_speedXZ(pEm, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Cdir_y += 0x800;
            EM36_W(pEm)->Flg &= ~3;
            pEm->Neck_flg &= ~0x12;
        }
        if (pEm->Move_cnt == 10)
            pEm->Neck_flg |= 0x12;
    }
}

/* no brain step for this move */
void Em36_punch_br(ENEMY_WORK *pEm)
{
}

/* motion: punch */
void Em36_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SHORT Spd_x, Spd_z;
    SVECTOR Ofs;
    PARTS_W *pP;
    LONG Hit;
    LONG *pPos;
    ULONG Dist;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 6, 0, 0x1F);
        if (pEm->Type & 1)
            pEm->Move_no++;
        pEm->Routine_2 = 1;
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 1;
        EM36_W(pEm)->pSub->Routine_2 = 0;
        EM36_W(pEm)->pSub->Move_cnt = 0;
        EM36_W(pEm)->pSub->Timer0 = 0x1000;
        EM36_W(pEm)->Flg |= 1;
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        if (Rnd() & 1) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(14, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
    case 1:
        if (pEm->Move_cnt < 15) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 24);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 24);
            }
        }
        if (pEm->Move_cnt >= 21 && pEm->Move_cnt < 30) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 32);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 32);
            }
        }
        Spd_x = pEm->Spd_x;
        Spd_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_x) * pEm->Hokan_flg) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Spd_z) * pEm->Hokan_flg) >> 5;
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM36_W(pEm)->Punch_timer = 30;
            EM36_W(pEm)->Lunge_timer += 5;
            EM36_W(pEm)->Mv12_timer += 10;
            if (pEm->Type & 1) {
                EM36_W(pEm)->Punch_timer += 10;
                EM36_W(pEm)->Mv12_timer += 10;
            }
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            if (pEm->UnkE2 && !(pEm->Type & 1))
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            if (!Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->L_pl < 3000 && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            if (!(pEm->UnkE3 & 0x80))
                EM36_W(pEm)->Hit_cnt++;
        }
        Add_speedXZ(pEm, 0);
        if (!(pEm->UnkE3 & 0x80)) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 16);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 16);
            }
            Dist = pEm->L_pl;
            if (pEm->Move_cnt >= 11 && pEm->Move_cnt < 20 && (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) || Dist > 4500) && !(EM36_W(pEm)->Flg & 0x80)) {
                EM_SET_RTN(pEm, 1, 1, 0, 0);
                EM36_W(pEm)->Punch_timer = 30;
                EM36_W(pEm)->Flg &= ~1;
                EM36_W(pEm)->pSub->Routine_0 = 1;
                EM36_W(pEm)->pSub->Routine_1 = 0;
                EM36_W(pEm)->pSub->Routine_2 = 0;
                pEm->Neck_flg &= ~3;
                if (!Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->L_pl < 3000 && !(Rnd() & 3) && !(EM36_W(pEm)->Flg & 0x80) && !Flg_ck(&G.Unk7888, 0x1F))
                    EM_SET_RTN(pEm, 1, 0xF, 0, 0);
                EM36_W(pEm)->Hit_cnt++;
                return;
            }
        }
        if (*pEm->pNow_seq & 0x200 && !pPl->Damage_cnt) {
            pEm->UnkE3 = -0x80;
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Ofs.pad = 400;
            pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
            Hit = Hit_area_ck(&pP[1].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[2].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[3].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[4].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
            if (Hit) {
                EM_RTN(pPl) = ((Cdir_ck_l(pEm, pPl) + 2) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                if (pPl->Damage_no == 0x80) {
                    pPl->pEnemy_ptr = pEm;
                    pPl->Neck_flg |= 0x12;
                    EM_SET_RTN(pPl, 5, 0, 8, 0);
                    pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                    pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                    pPl->Damage_cnt |= 0x80;
                    pEm->UnkE2 = 1;
                }
                Snd_se_enem(4, pEm);
                Pl_life_down(15, 1);
                Pad_vib_set0(8, 0);
                Pad_vib_set1(4, 200, 2);
            }
        }
    }
    if (*pEm->pNow_seq & 0x400) {
        pPos = &pEm->Pos_x;
        Em36_hit_area.pPos = (VECTOR *)pPos;
        Em36_hit_area.pBox = Em36_punch_box0;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 1;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_punch_box1;
        Em36_hit_area.Id = 2;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
    }
}

/* brain: attack */
void Em36_mv06_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;
    SHORT Dir, Dir_near, Dir_eye;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Dir_near = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Dir_eye = Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (!EM36_W(pEm)->Punch_timer) {
        if (Dist < 3000 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (!EM36_W(pEm)->Punch_timer && Dist < 3000 && !Dir_eye && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (EM36_W(pEm)->Punch_timer < 10 && Dist < 9000 && !Dir_near && !pPl->Damage_cnt && EM36_W(pEm)->Dm_stage >= 2 && Dist > 3500)
        EM_SET_RTN(pEm, 1, 0x12, 0, 0);
}

/* mv[6]: lumber forwards growling (motion 0x1C) */
void Em36_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Old;
    SHORT Spd;
    LONG Rot;

    EM36_W(pEm)->Flg &= ~3;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1C, 0, 0x3F);
        pEm->Routine_2 = 1;
        if (pEm->Spd_x > 100)
            pEm->Spd_x = 100;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Neck_flg |= 3;
        *(SHORT *)&pEm->pSin_parts_ptr[2].Unk74 = 0;
        pEm->UnkE3 = (Rnd() & 0x1F) + 45;
        EM36_W(pEm)->Flg |= 0x40;
        pEm->Be_flg |= 0x8000;
    case 1:
        if (--pEm->UnkE3 == -1) {
            pEm->UnkE3 = (Rnd() & 0x3F) + 90;
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(14, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
        Old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old) * pEm->Hokan_flg) >> 6;
        if (pEm->Spd_x > 50) {
            Spd = 0x20;
            if (pEm->UnkE2) {
                Spd = -0x20;
                pEm->UnkE2--;
            }
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Spd);
        }
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        if (pEm->Col.Hit_flg & 0x30) {
            if (++pEm->UnkDA > 150) {
                pEm->UnkE2 = 60;
                Rot = Rnd() & 1;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x40 - Rot * 120;
            }
        } else {
            pEm->UnkDA = 0;
        }
        if (!EM36_W(pEm)->Mv06_timer)
            pEm->Routine_2 = 2;
        break;
    case 2:
        EM_SET_MOVE(pEm, 0x1E, 0x1E, 0x1F);
        pEm->Routine_2 = 3;
        pEm->Hit_area.R = 600;
        EM36_W(pEm)->Flg |= 9;
        pEm->Type |= 0x2000;
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        EM36_W(pEm)->Flg &= ~0x40;
        pEm->Life = EM36_W(pEm)->Stage_life;
        EM36_W(pEm)->Stage_life -= 250;
        pEm->Max_life = EM36_W(pEm)->Stage_life + 200;
        pEm->Damage_cnt |= 0x80;
    case 3:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 30) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 8, 0, 0);
            pEm->Timer0 = (Rnd() & 0x7F) + 150;
            EM_SET_MOVE(pEm, 0x20, 0, 0x1F);
            pEm->Damage_cnt &= 0x7F;
            EM36_W(pEm)->Flg |= 3;
            pEm->Status_flg |= 0x800;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
    if (EM36_W(pEm)->Mv06_timer)
        EM36_W(pEm)->Mv06_timer--;
    Em36_punch_effect(pEm);
}

/* effects of the punch motion (part 5) */
void Em36_punch_effect(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 1) {
        Ofs.vx = 11;
        Ofs.vy = (Rnd() & 0xF) + 0x10C;
        Ofs.vz = 50;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[5].Workm;
        Esp_call(0x4001700, ((Rnd() * 4 + 0x1000) << 16) | (USHORT)(Rnd() * 16), pM, &Ofs, NULL);
        Esp_call(0x4000500, 0xA000000, pM, &Ofs, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && Rnd() & 1) {
        Ofs.vx = 11;
        Ofs.vy = 300;
        Ofs.vz = 50;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[5].Workm;
        Esp_call(0x2001700, ((Rnd() * 4 + 0x1000) << 16) | (USHORT)(Rnd() * 16), pM, &Ofs, NULL);
        Esp_call(0x2000500, 0xA000000, pM, &Ofs, NULL);
    }
}

/* no brain step for this move */
void Em36_roar_br(ENEMY_WORK *pEm)
{
}

/* motion: roar */
void Em36_roar(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Unused[2];

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 0x10, 0, 0xF);
        Snd_se_enem((Rnd() & 1) ? 15 : 14, pEm);
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* no brain step for this move */
void Em36_stand_up_br(ENEMY_WORK *pEm)
{
}

/* motion: stand up */
void Em36_stand_up(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Unused[2];

    switch (pEm->Routine_2) {
    case 0:
        if (--pEm->Timer0 == -1)
            pEm->Routine_2 = 1;
        if (pEm->Damage_cnt) {
            pEm->Timer0 -= 30;
            if (pEm->Timer0 < 0)
                pEm->Routine_2 = 1;
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        EM36_W(pEm)->Flg &= ~0x40;
        break;
    case 1:
        EM_SET_MOVE(pEm, 0x23, 0, 0x1F);
        pEm->Routine_2 = 2;
        pEm->Be_flg &= ~0x8000;
    case 2:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM36_W(pEm)->Flg &= ~0xB;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* no brain step for this move */
void Em36_side_step_br(ENEMY_WORK *pEm)
{
}

/* motion: step aside */
void Em36_side_step(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 9, 0, 3);
        pEm->Routine_2 = 1;
        if (Goto00_ck(pPl, pEm->Pos_x, pEm->Pos_z, 1) > 0)
            EM_SET_MOVE(pEm, 0xA, 0, 3);
        pEm->Hit_area.R = 100;
        pEm->Damage_cnt = 2;
        EM36_W(pEm)->Flg |= 1;
    case 1:
        if (pEm->Col.Hit_flg & 0x30)
            pEm->Damage_cnt = 0;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x60);
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x += pEm->Spd_x;
            pEm->Spd_z += pEm->Spd_z;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Hit_area.R = 600;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* no brain step for this move */
void Em36_lunge_br(ENEMY_WORK *pEm)
{
}

/* motion: lunge (tentacle stab) */
void Em36_lunge(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Ofs;
    PARTS_W *pP;
    LONG Hit;
    LONG *pPos;
    SHORT Spd_x, Spd_z;
    UCHAR Rate;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x11, 0, 0xF);
        if (pEm->Type & 1)
            pEm->Move_no++;
        EM36_W(pEm)->Flg |= 1;
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 3;
        EM36_W(pEm)->pSub->Routine_2 = 0;
        EM36_W(pEm)->pSub->Move_cnt = 0;
        pEm->Move_cnt = 0;
        EM36_W(pEm)->pSub->Move_cnt = 0;
        pEm->Timer0 = 0;
        if (EM36_W(pEm)->Flg & 0x200) {
            pEm->Move_cnt = 13;
            EM36_W(pEm)->pSub->Move_cnt = 13;
            pEm->UnkE2 = 0x80;
            pEm->Timer0 = pEm->Spd_x >> 1;
        } else {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            if ((Rnd() & 3) == 2) {
                pEm->Spd_x = 5;
                pEm->Spd_z = 5;
            }
        }
        if (EM36_W(pEm)->Flg & 0x20) {
            pEm->Move_cnt = 10;
            EM36_W(pEm)->pSub->Move_cnt = 10;
        }
        if (EM36_W(pEm)->Hit_cnt > 10) {
            pEm->Move_cnt = 15;
            EM36_W(pEm)->pSub->Move_cnt = 15;
            pEm->UnkE2 = 0x80;
        }
        if (EM36_W(pEm)->Flg & 0x400) {
            pEm->Move_cnt = 0;
            EM36_W(pEm)->pSub->Move_cnt = 0;
            pEm->UnkE2 = 0x80;
            EM36_W(pEm)->Flg &= ~0x400;
        }
        if ((UCHAR)pEm->Type != 3) {
            if ((Rnd() & 3) == 1)
                Snd_se_enem(15, pEm);
            else if ((Rnd() & 3) == 2)
                Snd_se_enem(11, pEm);
        } else {
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
    case 1:
        if (pEm->Move_cnt >= 9 && pEm->Move_cnt < 30) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 24);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 24);
            }
            EM36_W(pEm)->pSub->Timer0 = pEm->L_pl;
        }
        Spd_x = pEm->Spd_x;
        Spd_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        Rate = pEm->Hokan_flg;
        pEm->Spd_x -= ((pEm->Spd_x - Spd_x) * Rate) >> 4;
        pEm->Spd_z -= ((pEm->Spd_z - Spd_z) * Rate) >> 4;
        if (pEm->Timer0 > 0) {
            if ((pEm->Timer0 -= 10) < 0)
                pEm->Timer0 = 0;
            pEm->Spd_x += pEm->Timer0;
            if ((EM36_FLGW(pEm) & 0x880) == 0x800)
                Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 40);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (Rnd() & 3 == 1) /* sic: always false */
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            pEm->Neck_flg &= ~3;
            EM36_W(pEm)->Flg &= ~1;
            EM36_W(pEm)->Lunge_timer = (Rnd() & 0x1F) - 0x6A;
            EM36_W(pEm)->Punch_timer += 10;
            EM36_W(pEm)->Mv12_timer += 10;
            if (pEm->Type & 1) {
                EM36_W(pEm)->Punch_timer += 10;
                EM36_W(pEm)->Mv12_timer += 10;
                EM36_W(pEm)->Lunge_timer = (Rnd() & 0xF) + 90;
            }
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            if (!Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->L_pl < 3000
                && Rnd() & 1 && !(EM36_W(pEm)->Flg & 0x80) && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            if (!(pEm->UnkE3 & 0x80))
                EM36_W(pEm)->Hit_cnt += 2;
        }
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x200 && !pPl->Damage_cnt && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40) && EM36_W(pEm)->Flg & 0x800) {
            pEm->UnkE3 = 0x81;
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Ofs.pad = 500;
            pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
            Hit = Hit_area_ck(&pP[1].Workm, &Ofs, pPl) | Hit_area_ck(&pP[2].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[3].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[4].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
            if (Hit) {
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
                pEm->Damage_cnt |= 0x80;
                pPl->Damage_cnt |= 0x80;
                EM36_W(pEm)->Flg &= ~1;
                Snd_se_enem(0, pEm);
                Pad_vib_set0(2, 0);
                Pad_vib_set1(6, 150, 0);
            }
        }
        if (pEm->Move_cnt >= 11 && pEm->Move_cnt < 25 && !(EM36_W(pEm)->Flg & 0x200)
            && (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) || (LONG)pEm->L_pl > 5500) && !(EM36_W(pEm)->Flg & 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM36_W(pEm)->Lunge_timer = 10;
            EM36_W(pEm)->Flg &= ~1;
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            pEm->Neck_flg &= ~3;
            if (!Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->L_pl < 3000
                && Rnd() & 1 && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            EM36_W(pEm)->Hit_cnt += 3;
            return;
        }
    }
    if (*pEm->pNow_seq & 0x400) {
        Em36_hit_area.pPos = POS_V(pEm);
        Em36_hit_area.pBox = Em36_lunge_box0;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 1;
        if (!(Rnd() & 1))
            Em36_hit_area.Id = 6;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_lunge_box1;
        Em36_hit_area.Id = 2;
        if (!(Rnd() & 1))
            Em36_hit_area.Id = 7;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
}

/* no brain step for this move */
void Em36_squeeze_br(ENEMY_WORK *pEm)
{
}

/* motion: grab and squeeze the player */
void Em36_squeeze(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = G_PLAYER;
    SHORT Base_x, Base_z;
    SHORT Ret;
    UCHAR Rate;
    LONG Unused[6];

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x13, 0, 0xF);
        pEm->Routine_2 = 1;
        pEm->Timer0 = pEm->Cdir_y;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pEm->Cdir_y = pEm->Timer0;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
        pPl->Neck_flg |= 0x12;
        pPl->UnkDA = pEm->L_pl;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x800;
        pPl->Be_flg |= 0x800;
        EM_SET_RTN(pPl, 5, 0, 0, 0);
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 4;
        EM36_W(pEm)->pSub->Routine_2 = 0;
        EM36_W(pEm)->Hit_cnt = 0;
    case 1:
        if (pEm->Move_cnt < 8)
            Goto01(pEm, pEm->Timer0, 0x100);
        Rate = pEm->Hokan_flg;
        pEm->Base_pos_x = pPl->Base_pos_x - (((pPl->Base_pos_x - pEm->Base_pos_x) * Rate) >> 3);
        pEm->Base_pos_z = pPl->Base_pos_z - (((pPl->Base_pos_z - pEm->Base_pos_z) * Rate) >> 3);
        pEm->UnkDA = pEm->Cdir_y;
        pEm->Cdir_y = pEm->Timer0;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y = pEm->UnkDA;
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0x14, 0, 0);
            pEm->UnkE3 = 1;
            if (pPl->Life > 130 && !(pEm->UnkE2 & 0x80))
                pEm->UnkE3 = 2;
            pPl->UnkE3 = pEm->UnkE3;
            Ret = Pl_life_down(51, 0);
            if (pPl->UnkE3 == 2)
                Ret = Pl_life_down(51, 0);
            if (Ret == 2) {
                pEm->UnkE3 = 4;
                pPl->UnkE3 = 4;
                pPl->Routine_0 = 6;
                G.System_flg |= 0x800;
            }
        }
        break;
    case 2:
        pEm->Base_pos_x = pPl->Base_pos_x;
        pEm->Base_pos_z = pPl->Base_pos_z;
        Spd_get(pEm, pKan, pSeq, 0);
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (!--pEm->UnkE3) {
                pEm->Routine_2 = 3;
                EM_SET_MOVE(pEm, 0x15, 0, 0);
                EM36_W(pEm)->pSub->Routine_0 = 1;
                EM36_W(pEm)->pSub->Routine_1 = 4;
                EM36_W(pEm)->pSub->Routine_2 = 3;
            }
        }
        break;
    case 3:
        if (!pEm->Move_cnt) {
            Base_x = pEm->Spd_base_x;
            Base_z = pEm->Spd_base_z;
        }
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x -= Base_x;
            pEm->Spd_z -= Base_z;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM36_W(pEm)->Punch_timer = 5;
            EM36_W(pEm)->Mv12_timer = 45;
            EM36_W(pEm)->Grab_timer = 45;
            EM36_W(pEm)->Lunge_timer = 250;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            pEm->Damage_cnt &= 0x7F;
            if (pPl->Life < 50)
                EM36_W(pEm)->Lunge_timer = 60;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* no brain step for this move */
void Em36_mv12_br(ENEMY_WORK *pEm)
{
}

INCLUDE_ASM("asm/nonmatchings/enemy/em36", Em36_mv12);

/* no brain step for this move */
void Em36_grab_br(ENEMY_WORK *pEm)
{
}

/* motion: grab the player */
void Em36_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = G_PLAYER;
    VECTOR *pV;
    LONG Unused[6];

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x18, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = pEm->Cdir_y;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pEm->Cdir_y = pEm->Timer0;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
        pPl->Neck_flg |= 0x12;
        pPl->Timer0 = pEm->L_pl;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        EM_SET_RTN(pPl, 5, 2, 0, 0);
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 6;
        EM36_W(pEm)->pSub->Routine_2 = 0;
    case 1:
        if (pEm->Move_cnt < 8)
            Goto01(pEm, pEm->Timer0, 0x100);
        pEm->Base_pos_x = pPl->Base_pos_x;
        pEm->Base_pos_z = pPl->Base_pos_z;
        pEm->UnkDA = pEm->Cdir_y;
        pEm->Cdir_y = pEm->Timer0;
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        pEm->Cdir_y = pEm->UnkDA;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Neck_flg &= ~3;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            EM_SET_MOVE(pEm, 4, 0, 0x1F);
            pEm->Routine_2 = 2;
            pEm->Timer0 = 8;
            pEm->Damage_cnt &= 0x7F;
            pEm->Cdir_y -= 0x400;
            if (pPl->Life > 180 && !Flg_ck(&G.Unk7888, 0x1F)) {
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
                EM36_W(pEm)->Punch_timer = 5;
                EM36_W(pEm)->Mv12_timer = 45;
                EM36_W(pEm)->Grab_timer = 45;
            }
            if (((Rnd() & 7) == 1 | pPl->Life < 0) && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 700)
                && !Flg_ck(&G.Unk7888, 0x1E) && EM36_W(pEm)->Flg & 0x800) {
                EM_SET_RTN(pEm, 1, 0xA, 0, 0);
                EM36_W(pEm)->Flg = (EM36_W(pEm)->Flg & ~0x200) | 0x400;
            }
            if (EM36_W(pEm)->Flg & 0x1000) {
                EM_SET_RTN(pEm, 1, 0x14, 0, 0);
                pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Route_pt];
                if (Goto00_ck(pEm, pV->vx, pV->vz, 0x400))
                    pEm->Routine_2 = 2;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (--pEm->Timer0 == -1) {
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            if (Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 5, 0, 0);
            EM36_W(pEm)->Punch_timer = 5;
            EM36_W(pEm)->Mv12_timer = 45;
            EM36_W(pEm)->Grab_timer = 45;
        }
        break;
    }
}

/* no brain step for this move */
void Em36_mv14_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em36_mv14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em36_sweep_br(ENEMY_WORK *pEm)
{
}

/* motion: sweep the arm (tentacle) */
void Em36_sweep(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Ofs;
    PARTS_W *pP;
    LONG *pPos;
    LONG Cdir;
    LONG Dir;
    SHORT Gy;
    LONG Hit;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1A, 0, 0xF);
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 9;
        EM36_W(pEm)->pSub->Routine_2 = 0;
        EM36_W(pEm)->pSub->Routine_3 = 0;
        EM36_W(pEm)->Flg |= 3;
        if ((UCHAR)pEm->Type != 3) {
            if ((Rnd() & 3) == 1)
                Snd_se_enem(15, pEm);
            else if ((Rnd() & 3) == 2)
                Snd_se_enem(14, pEm);
        } else {
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
        if ((Rnd() >> (Rnd() & 3)) & 1) {
            pEm->Move_cnt = 10;
            pEm->Timer0 = 1;
            EM36_W(pEm)->pSub->Routine_3 = 1;
        }
        pEm->Routine_3 = 0;
        if (!Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x600, 0x4C8))
            pEm->Routine_3 = 0x60;
    case 1:
        if (pEm->Hokan_flg)
            pEm->Cdir_y -= 16;
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Timer0) {
            pEm->Timer0 = 0;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (!(Rnd() & 3) && !(pEm->Type & 1))
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            EM36_W(pEm)->Punch_timer = 30;
            EM36_W(pEm)->Sweep_timer = 180;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            pEm->Cdir_y += 0x400;
            EM36_W(pEm)->Flg &= ~3;
            EM36_W(pEm)->Mv12_timer += 10;
            EM36_W(pEm)->Lunge_timer += 5;
            if (!(pEm->UnkE2 & 0x80)) {
                EM36_W(pEm)->Hit_cnt += 2;
                if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) && (LONG)pEm->L_pl < 4000) {
                    EM_SET_RTN(pEm, 1, 5, 0, 0);
                    if (Rnd() & 1) {
                        if (!Flg_ck(&G.Unk7888, 0x1E) && EM36_W(pEm)->Flg & 0x800)
                            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
                    }
                }
            }
        }
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x200 && !pPl->Damage_cnt) {
            pEm->UnkE3 = -0x80;
            pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
            Gy = Sca_get_ground(pP[1].Workm.t, 200, 0x2000, 0);
            if (pEm->Move_cnt < 30 && Gy < pP[1].Workm.t[1]) {
                EM_SET_RTN(pEm, 1, 0x11, 0, 0);
                return;
            }
            pEm->Cdir_y += pEm->Routine_3;
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Ofs.pad = 900;
            Hit = Hit_area_ck(&pP[1].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[2].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[3].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[4].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
            if (Hit) {
                Cdir = pEm->Cdir_y;
                Dir = pEm->Cdir_y = Direction_ck(pPl->Pos_x, pPl->Pos_z, pEm->Pos_x, pEm->Pos_z) - 0x400;
                Hit = Cdir_ck_l(pEm, pPl);
                Dir -= 0x800;
                pEm->Cdir_y = Cdir;
                EM_RTN(pPl) = ((Hit + 4) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                if (Hit)
                    Dir += 0x800;
                Goto01(pPl, Dir, 0x100);
                Snd_se_enem(4, pEm);
                Pl_life_down(30, 1);
                Pad_vib_set0(10, 0);
                Pad_vib_set1(5, 250, 0);
                pEm->UnkE2 = 0x85;
            }
        }
        break;
    }
    if (pEm->Move_cnt >= 22 && pEm->Move_cnt < 28) {
        Em36_hit_area.pPos = POS_V(pEm);
        Em36_hit_area.pBox = Em36_sweep_box0;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 3;
        if (!(Rnd() & 3))
            Em36_hit_area.Id = 6;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
    if (pEm->Move_cnt >= 24 && pEm->Move_cnt < 29) {
        Em36_hit_area.pPos = POS_V(pEm);
        Em36_hit_area.pBox = Em36_sweep_box1;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 3;
        if (!(Rnd() & 3))
            Em36_hit_area.Id = 6;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
    if (pEm->Move_cnt >= 26 && pEm->Move_cnt < 32) {
        Em36_hit_area.pPos = POS_V(pEm);
        Em36_hit_area.pBox = Em36_sweep_box2;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 3;
        if (Rnd() & 1)
            Em36_hit_area.Id = 6;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_sweep_box3;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
}

/* no brain step for this move */
void Em36_mv16_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em36_mv16(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em36_swing_br(ENEMY_WORK *pEm)
{
}

/* motion: swing */
void Em36_swing(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Unused[2];

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1B, 0, 7);
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 10;
        EM36_W(pEm)->pSub->Routine_2 = 0;
        Snd_se_enem(4, pEm);
        if (Rnd() & 1) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(12, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400))
                EM_SET_RTN(pEm, 1, 4, 0, 0);
            if (pEm->Type & 1 && Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300))
                EM_SET_RTN(pEm, 1, 4, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            EM36_W(pEm)->Punch_timer = 20;
            pEm->Neck_flg &= ~3;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* no brain step for this move */
void Em36_swing_arm_br(ENEMY_WORK *pEm)
{
}

/* motion: swing the arm (tentacle) */
void Em36_swing_arm(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Ofs;
    PARTS_W *pP;
    LONG Hit;
    LONG *pPos;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1D, 0, 0xF);
        pEm->Routine_2 = 1;
        EM36_W(pEm)->Flg |= 1;
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 8;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1800;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
            if (pEm->Type & 1) {
                EM36_W(pEm)->pSub->Spd_y = 0x2400;
                EM36_W(pEm)->pSub->Spd_x = 0x400;
            }
        }
        pEm->UnkE3 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        pEm->UnkE2 = 0;
        if ((UCHAR)pEm->Type != 3)
            Snd_se_enem((Rnd() & 1) ? 15 : 14, pEm);
        else
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
    case 1:
        if (pEm->Move_cnt >= 9 && pEm->Move_cnt < 40) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 48);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 48);
            }
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_y = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM36_W(pEm)->Punch_timer = 15;
            EM36_W(pEm)->Flg &= ~1;
            pEm->Neck_flg &= ~3;
            if (!Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->L_pl < 3000
                && !(Rnd() & 1) && !(EM36_W(pEm)->Flg & 0x80) && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            EM36_W(pEm)->Hit_cnt += 3;
        }
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x200 && !pPl->Damage_cnt) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 8);
            }
            Ofs.vy = 200;
            Ofs.vx = 0;
            Ofs.vz = 0;
            Ofs.pad = 600;
            pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
            Hit = Hit_area_ck(&pP[1].Workm, &Ofs, pPl) | Hit_area_ck(&pP[2].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[3].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[4].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[5].Workm, &Ofs, pPl);
            if (Hit) {
                EM_RTN(pPl) = ((Cdir_ck_l(pEm, pPl) + 2) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                Snd_se_enem(4, pEm);
                if (Pl_life_down(40, 0) == 2)
                    EM_SET_RTN(pPl, 3, 0, 0, 0);
                Pad_vib_set0(10, 0);
                Pad_vib_set1(5, 250, 0);
                pEm->UnkE2 = 0x8F;
            }
        }
        if (pEm->UnkE2 & 0x7F) {
            pEm->UnkE2--;
            pP = &EM36_W(pEm)->pSub->pSin_parts_ptr[5];
            Ofs.vx = pP->Workm.t[0];
            Ofs.vy = pP->Workm.t[1];
            Ofs.vz = pP->Workm.t[2];
            Esp_call(0x1700, ((Rnd() * 2 + 0x1800) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Ofs, NULL);
        }
        break;
    }
    if (*pEm->pNow_seq & 0x400) {
        Em36_hit_area.pPos = (VECTOR *)EM36_W(pEm)->pSub->pSin_parts_ptr[3].Workm.t;
        Em36_hit_area.pBox = Em36_swing_box0;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 1;
        if (Rnd() & 1)
            Em36_hit_area.Id = 3;
        pPos = &pEm->Pos_x;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_swing_box1;
        Em36_hit_area.Id = 2;
        if (Rnd() & 1)
            Em36_hit_area.Id = 3;
        Hit_area_em_ck(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
    }
}

/* no brain step for this move */
void Em36_run_punch_br(ENEMY_WORK *pEm)
{
}

/* motion: punch on the run (route type) */
void Em36_run_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Ofs;
    PARTS_W *pP;
    LONG Hit;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 6, 0, 0x1F);
        pEm->Routine_2 = 1;
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 1;
        EM36_W(pEm)->pSub->Routine_2 = 0;
        EM36_W(pEm)->pSub->Move_cnt = 0;
        EM36_W(pEm)->pSub->Timer0 = 0x400;
        EM36_W(pEm)->Flg |= 1;
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        EM36_W(pEm)->Type_timer -= 50;
        if ((UCHAR)pEm->Type != 3)
            Snd_se_enem((Rnd() & 1) ? 15 : 11, pEm);
        else
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
    case 1:
        if (pEm->Move_cnt < 15) {
            Goto00(pEm, Em36_route_pos[EM36_W(pEm)->Route_no].vx, Em36_route_pos[EM36_W(pEm)->Route_no].vz, 24);
        }
        if (pEm->Move_cnt == 25) {
            Flg_on(&G.Unk7888, 0x20);
            if ((LONG)pEm->L_pl > 6000)
                Flg_on(&G.Unk7888, 0x21);
            if ((LONG)pEm->L_pl > 10000)
                Flg_on(&G.Unk7888, 0x22);
            Snd_se_enem(4, pEm);
        }
        if (pEm->Move_cnt == 28)
            Snd_se_enem(4, pEm);
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_y = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM36_W(pEm)->Punch_timer = 30;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            if (!Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->L_pl < 3000)
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
        }
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x200 && !pPl->Damage_cnt) {
            pEm->UnkE3 = -0x80;
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Ofs.pad = 400;
            pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
            Hit = Hit_area_ck(&pP[1].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[2].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[3].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pP[4].Workm, &Ofs, pPl);
            Hit |= Hit_area_ck(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
            if (Hit) {
                EM_RTN(pPl) = ((Cdir_ck_l(pEm, pPl) + 2) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                if (pPl->Damage_no == 0x80) {
                    pPl->pEnemy_ptr = pEm;
                    pPl->Neck_flg |= 0x12;
                    EM_SET_RTN(pPl, 5, 0, 8, 0);
                    pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                    pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                    pPl->Damage_cnt |= 0x80;
                    pEm->UnkE2 = 1;
                }
                Snd_se_enem(4, pEm);
                Pl_life_down(15, 1);
                Pad_vib_set0(8, 0);
                Pad_vib_set1(4, 200, 2);
            }
        }
    }
}

/* brain: on a route (type 1) */
void Em36_route_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->L_pl;
    SHORT Dir_wide, Dir, Dir_near;

    Dir_wide = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Dir_near = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (!EM36_W(pEm)->Punch_timer) {
        if (Dist < 3000 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (!EM36_W(pEm)->Punch_timer) {
            if (Dist < 3000 && Dir_wide && !pPl->Damage_cnt)
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            if (!EM36_W(pEm)->Punch_timer && Dist < 9000 && Dist > 6000 && !Dir_near && !pPl->Damage_cnt && EM36_W(pEm)->Dm_stage >= 2)
                EM_SET_RTN(pEm, 1, 0x12, 0, 0);
        }
    }
    if (!(EM36_W(pEm)->Flg & 0x1000))
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* motion: follow the route */
void Em36_route_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SHORT Spd_old;
    SHORT Dist;
    VECTOR *pV;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x21, 0, 0x3F);
        pEm->Routine_2 = 1;
        if (pEm->Spd_x > 100)
            pEm->Spd_x = 100;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Neck_flg |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        EM36_W(pEm)->Flg &= ~3;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 6;
        if (pEm->Spd_x > 50) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 32);
            if (Rnd() & 1)
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
        }
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Route_pt];
        Dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
        if (Dist < 500
            && Goto00_ck(pEm, pV->vx, pV->vz, 0x200))
            pEm->Routine_2 = 4;
        break;
    case 2:
        EM_SET_MOVE(pEm, 4, 0, 0x3F);
        pEm->Routine_2 = 3;
        EM36_W(pEm)->Flg &= ~3;
    case 3:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 6;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -32);
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Route_pt];
        Dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
        if (Dist < 500
            && !Goto00_ck(pEm, pV->vx, pV->vz, 0x200))
            pEm->Routine_2 = 4;
        break;
    case 4:
        pEm->Routine_2 = 5;
        EM36_W(pEm)->Flg &= ~3;
    case 5:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Route_pt];
        Dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
        if (Dist > 1500
            && (LONG)pEm->L_pl > 7000)
            pEm->Routine_2 = 6;
        break;
    case 6:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 16);
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* Routine_0 2: damage (RE2 Em2a_damage) */
void Em36_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Lim;
    LONG Unused[2];

    if (!pEm->Routine_2) {
        if (pEm->Damage_flg & 0x100)
            Em34_blood_spray(pEm);
        if (pEm->Damage_flg & 0x400)
            Em34_blood_spray(pEm);
        if (pEm->Damage_flg & 0x200)
            Em36_knock_back2(pEm);
        if (pEm->Damage_flg & 0x800)
            Em36_knock_back(pEm);
    }
    Lim = 400;
    if (G.System_flg & 0x100)
        Lim = 300;
    if (!(pEm->Type & 1)) {
        if (pEm->Life < Lim && !(EM36_W(pEm)->Flg & 0x20) && !pEm->Routine_2) {
            pEm->Damage_no = 32;
            pEm->Life = Lim;
            EM36_W(pEm)->Flg |= 0x20;
            if (!(EM36_W(pEm)->Flg & 2))
                EM36_W(pEm)->Flg &= ~3;
            else
                EM36_W(pEm)->Flg &= ~0x20;
        }
        if (EM36_W(pEm)->Flg & 1) {
            EM36_W(pEm)->Shake_no = 1;
            EM_RTN(pEm) = pEm->Old_routine;
            Em34_blood_set(pEm);
            if (pEm->Routine_0 == 4)
                Em36_Scenario(pEm, pKan, pSeq);
            else
                Em36_Move(pEm, pKan, pSeq);
            return;
        }
        Em36_dm[pEm->Damage_no - 1][pEm->Routine_1](pEm, pKan, pSeq);
        return;
    }
    if (EM36_W(pEm)->Flg & 0x40 && !pEm->Routine_2 && pEm->Damage_no != 30) {
        pEm->Damage_no = 32;
        pEm->Life = EM36_W(pEm)->Stage_life;
        if (!(EM36_W(pEm)->Flg & 2))
            EM36_W(pEm)->Flg &= ~3;
        pEm->Be_flg |= 0x8000;
    }
    if (EM36_W(pEm)->Stage_life > pEm->Life && !pEm->Routine_2) {
        pEm->Life = EM36_W(pEm)->Stage_life;
        EM36_W(pEm)->Flg |= 0x40;
        EM36_W(pEm)->Mv06_timer = 450;
        if (!(EM36_W(pEm)->Flg & 2))
            EM36_W(pEm)->Flg &= ~3;
        pEm->Damage_no = 33;
    }
    if (!(EM36_W(pEm)->Flg & 2) && pEm->Damage_no == 30)
        EM36_W(pEm)->Flg &= ~3;
    if (EM36_W(pEm)->Flg & 8 && pEm->Damage_no == 30)
        EM36_W(pEm)->Flg &= ~3;
    if (EM36_W(pEm)->Flg & 1) {
        if (pEm->Damage_no == 30)
            EM36_W(pEm)->Flg |= 0x2000;
        if (EM36_W(pEm)->Flg & 1) {
            EM36_W(pEm)->Shake_no = 1;
            EM_RTN(pEm) = pEm->Old_routine;
            Em34_blood_set(pEm);
            if (pEm->Routine_0 == 4)
                Em36_Scenario(pEm, pKan, pSeq);
            else
                Em36_Move(pEm, pKan, pSeq);
            return;
        }
    }
    Em36_dm2[pEm->Damage_no - 1](pEm, pKan, pSeq);
}

/* damage: flinch */
void Em36_dm_flinch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Unused[2];

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE_V(pEm, (Rnd() & 1) + 0xB, 0, 7);
        pEm->Routine_2 = 1;
        Em34_blood_set(pEm);
        if (pEm->Damage_flg & 0x10)
            EM_SET_MOVE(pEm, 0xF, 0, 7);
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        if ((UCHAR)pEm->Type == 8)
            EM36_W(pEm)->Type_timer -= 60;
        pEm->Hit_area.R = 600;
        if (pEm->Type & 1) {
            pEm->Damage_cnt |= 0x80;
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(15, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        } else {
            Snd_se_enem(13, pEm);
        }
        pEm->Be_flg &= ~0x8000;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (((LONG)pEm->L_pl > 7000 || (Rnd() & 3) == 1) && !(pEm->Type & 1))
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            if ((UCHAR)pEm->Type == 8 && !(Rnd() & 3)) {
                if (EM36_W(pEm)->Type_timer < 150) {
                    EM36_W(pEm)->Type_timer = 300;
                    pEm->Type -= 2;
                }
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            }
            if (pEm->Type & 1) {
                EM_SET_RTN(pEm, 1, 6, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
        }
        Add_speedXZ(pEm, 0);
    }
}

/* damage: down; Last (player Id >= 8, System_flg 0x80 clear) also clears Status_flg 0x40000000 */
void Em36_dm_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    tagGLOBAL *pG = &G;
    LONG Last;
    ULONG Sys;

    Last = 0;
    if (pG->Player.Id >= 8) {
        Sys = pG->System_flg & 0x80;
        Last = Sys == 0;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1E, 0, 0x1F);
        pEm->Routine_2 = 1;
        if (Last) {
            EM_SET_MOVE(pEm, 0xD, 0, 0x1F);
            Em_dead_flg_on_cand(pEm);
            pG->Unk785C = 0;
            pG->Status_flg &= ~0x40000000;
            pEm->Type |= 0x4000;
        }
        pEm->Damage_cnt |= 0x80;
        Em34_blood_set(pEm);
        EM36_W(pEm)->Flg |= 8;
        if ((UCHAR)pEm->Type != 3)
            Snd_se_enem(12, pEm);
        else
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        pEm->Type |= 0x2000;
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        if (pEm->Type & 1) {
            EM36_W(pEm)->Flg &= ~0x40;
            pEm->Life = EM36_W(pEm)->Stage_life;
            EM36_W(pEm)->Stage_life -= 250;
            pEm->Max_life = EM36_W(pEm)->Stage_life + 200;
        }
        EM36_W(pEm)->Mv06_timer = 0;
        pEm->Hit_area.R = 600;
    case 1:
        if (pEm->Move_cnt == 40) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(13, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 8, 0, 0);
            pEm->Timer0 = (Rnd() & 0x7F) + 150;
            EM_SET_MOVE(pEm, 0x20, 0, 0x1F);
            pEm->Damage_cnt &= 0x7F;
            EM36_W(pEm)->Flg |= 3;
            pEm->Status_flg |= 0x800;
            if (Last) {
                EM_SET_RTN(pEm, 1, 8, 3, 0);
                pEm->Damage_cnt |= 0x80;
                pEm->Col.Id = 0;
                pEm->Neck_flg |= 3;
                *(SHORT *)&pEm->pSin_parts_ptr[2].Unk74 = 0;
            }
        }
        Add_speedXZ(pEm, 0);
    }
}

/* damage: light hit, keep moving */
void Em36_dm_light(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM36_W(pEm)->Shake_no = 1;
    if ((UCHAR)pEm->Type != 3) {
        if (Rnd() & 1)
            Snd_se_enem(12, pEm);
    } else {
        Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
    }
    EM_RTN(pEm) = pEm->Old_routine;
    Em34_blood_set(pEm);
    if (pEm->Routine_0 == 2) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (!(EM36_W(pEm)->Flg & 0x800) && Rnd() & 1)
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        if (EM36_W(pEm)->Flg & 0x40)
            EM_SET_RTN(pEm, 1, 6, 0, 0);
    }
    Em36_Move(pEm, pKan, pSeq);
}

/* damage: lose a part (Unk15 = damage stage) */
void Em36_dm_part(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x22, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        EM36_W(pEm)->Flg |= 8;
        if ((UCHAR)pEm->Type != 3)
            Snd_se_enem(13, pEm);
        else
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        EM36_W(pEm)->Mv06_timer = 0;
        pEm->Hit_area.R = 600;
        Flg_on(&G.Unk7888, 0x1D);
        EM36_W(pEm)->Flg &= ~0x40;
        EM36_W(pEm)->Stage_life = pEm->Life - 250;
        if (EM36_W(pEm)->Stage_life < 0)
            EM36_W(pEm)->Stage_life = 0;
        pEm->Max_life = EM36_W(pEm)->Stage_life + 200;
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[1].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[3].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[14].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x8000000, &pEm->pSin_parts_ptr[12].Workm, NULL, NULL);
        EM36_W(pEm)->Flg &= ~0x2000;
        Snd_se_stad(0x10204, POS_V(pEm), 0, 0);
    case 1:
        switch (EM36_W(pEm)->Dm_stage) {
        case 0:
            Em36_parts_effect(pEm);
            EM36_W(pEm)->Dm_stage = 1;
            break;
        case 1:
            if (pEm->Move_cnt == 15) {
                Em36_blow_off6(pEm);
                EM36_W(pEm)->Dm_stage = 2;
            }
            break;
        case 2:
            if (pEm->Move_cnt == 15) {
                Em36_blow_off22(pEm);
                EM36_W(pEm)->Dm_stage = 3;
                EM36_W(pEm)->Lost_time = 60;
                pEm->Type = (pEm->Type & 0xFFF0) | 3;
                EM36_W(pEm)->Flg |= 0x80;
            }
            break;
        case 3:
            if (!pEm->Move_cnt) {
                Em_dead_flg_on_cand(pEm);
                EM_SET_RTN(pEm, 4, 0xA, 0, 0);
                pEm->Life = -1;
                return;
            }
            break;
        }
        if (pEm->Move_cnt == 15 || pEm->Move_cnt == 65) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(12, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
        if (pEm->Move_cnt == 45) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(13, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 8, 1, 0);
            pEm->Damage_cnt &= 0x7F;
            EM36_W(pEm)->Flg |= 0xB;
            pEm->Be_flg &= ~0x8000;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* damage: knocked down */
void Em36_dm_knock(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1F, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        Em34_blood_set(pEm);
        EM36_W(pEm)->Flg |= 8;
        Snd_se_enem(12, pEm);
        pEm->Type |= 0x2000;
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        EM36_W(pEm)->Mv06_timer = 0;
        pEm->Hit_area.R = 600;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 8, 0, 0);
            pEm->Timer0 = (Rnd() & 0x7F) + 150;
            EM_SET_MOVE(pEm, 0x20, 0, 0x1F);
            pEm->Damage_cnt &= 0x7F;
            EM36_W(pEm)->Flg |= 3;
            pEm->Status_flg |= 0x800;
            pEm->Be_flg &= ~0x8000;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* effects on parts 16..21 */
void Em36_parts_effect(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG i;
    SVECTOR Pos;

    pP = &pEm->pSin_parts_ptr[16];
    for (i = 6; i != 0; i--) {
        Pos.vx = pP->Pos_x;
        Pos.vy = pP->Pos_y;
        Pos.vz = pP->Pos_z;
        Esp_call(0x1C03, 0x10000000, pP->pSuper, &Pos, NULL);
        pP++;
    }
}

/* blow off part 6 (and part 8) */
void Em36_blow_off6(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    MATRIX *pM1;
    MATRIX *pM8;
    SVECTOR Pos;

    pP = &pEm->pSin_parts_ptr[6];
    pP->Be_flg |= 0xA;
    pP->Old_y = 40;
    pP->Old_x = 0;
    pP->Timer3 = 0;
    pP->Old_z = 0;
    pP->Timer1 = 9;
    pP->Rot_spd_x = 0;
    pP->UnkA2 |= 0x400;
    pP->Rot_spd_y = pEm->Cdir_y;
    pP->Rot_spd_z = 0;
    Pos.vx = pP->Pos_x;
    Pos.vy = pP->Pos_y;
    Pos.vz = pP->Pos_z;
    pM = &pP->Workm;
    Esp_call(0x1C03, 0xA000000, pM, NULL, NULL);
    Esp_call(0, 0x10000000, pM, NULL, NULL);
    pM1 = &pEm->pSin_parts_ptr[1].Workm;
    Esp_call(0x1C03, 0x10000000, pM1, &Pos, NULL);
    Esp_call(0x200, 0x10000000, pM1, &Pos, NULL);
    Esp_call(0x1403, 0x6000000, pM1, &Pos, NULL);
    Pos.vx -= 20;
    Pos.vy += 50;
    Pos.vz -= 20;
    Esp_call(0x1403, 0x6000000, pM1, &Pos, NULL);
    pP = &pEm->pSin_parts_ptr[8];
    pP->Be_flg |= 0xA;
    pP->Old_x = 0;
    pP->Timer3 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Timer1 = 9;
    pP->Rot_spd_x = 0;
    pP->UnkA2 |= 0x400;
    pP->Rot_spd_y = pEm->Cdir_y;
    pP->Rot_spd_z = 0;
    pM8 = &pP->Workm;
    Esp_call(0x1C03, 0x10000000, pM8, NULL, NULL);
    Esp_call(0, 0x10000000, pM8, NULL, NULL);
}

/* blow off part 22 (copy of part 2) */
void Em36_blow_off22(ENEMY_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pP;
    PARTS_W *pSrc;
    MATRIX *pM;
    MATRIX *pM1;
    SVECTOR Pos;

    pParts = pEm->pSin_parts_ptr;
    pP = &pParts[22];
    pSrc = &pParts[2];
    pP->UnkA2 |= 1;
    Pos.vx = pSrc->Pos_x;
    Pos.vy = pSrc->Pos_y;
    Pos.vz = pSrc->Pos_z;
    pP->pSuper = pSrc->pSuper;
    pM = &pParts[22].Workm;
    pP->pOya_parts = pSrc->pOya_parts;
    *(MATRIX *)pParts[22].M = *(MATRIX *)pParts[2].M;
    CompM(pP->pSuper, (MATRIX *)pParts[22].M, pM);
    pSrc->UnkA2 &= ~1;
    pParts[22].Be_flg |= 0xA;
    pP->Old_y = -20;
    pP->Old_x = 0;
    pP->Timer3 = 0;
    pP->Old_z = 0;
    pP->Timer1 = 9;
    pP->Rot_spd_x = 0;
    pP->UnkA2 |= 0x400;
    pP->Rot_spd_y = pEm->Cdir_y;
    pP->Rot_spd_z = 0x338;
    Esp_call(0x1C03, 0xA000000, pM, NULL, NULL);
    Esp_call(0, 0x10000000, pM, NULL, NULL);
    pM1 = &pEm->pSin_parts_ptr[1].Workm;
    Esp_call(0x200, 0x18000000, pM1, &Pos, NULL);
    Esp_call(0x1403, 0x6000000, pM1, &Pos, NULL);
    Pos.vx += 10;
    Pos.vy += 50;
    Pos.vz -= 20;
    Esp_call(0x1403, 0x6000000, pM1, &Pos, NULL);
    pP = pEm->pSin_parts_ptr;
    Esp_call(0x1403, 0x10000000, &pP->Workm, NULL, NULL);
}

/* blow off part 23 (copy of part 4), part 5 and the sub work's parts 0 and 3 */
void Em36_blow_off23(ENEMY_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pP;
    PARTS_W *pSrc;
    MATRIX *pM;
    MATRIX *pM2;
    MATRIX *pM3;
    MATRIX *pM4;

    pParts = pEm->pSin_parts_ptr;
    pP = &pParts[23];
    pSrc = &pParts[4];
    pP->UnkA2 |= 1;
    pP->pSuper = pSrc->pSuper;
    pM = &pParts[23].Workm;
    pP->pOya_parts = pSrc->pOya_parts;
    *(MATRIX *)pParts[23].M = *(MATRIX *)pParts[4].M;
    CompM(pP->pSuper, (MATRIX *)pParts[23].M, pM);
    pSrc->UnkA2 &= ~1;
    pParts[23].Be_flg |= 0xA;
    pP->Old_x = 0;
    pP->Timer3 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Timer1 = 15;
    pP->Rot_spd_x = 0;
    pP->Rot_spd_y = 0;
    pP->Rot_spd_z = 0;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM, NULL, NULL);
    Esp_call(0, 0x10000000, pM, NULL, NULL);

    pM2 = (MATRIX *)pEm->pSin_parts_ptr;
    pP = &((PARTS_W *)pM2)[5];
    ((PARTS_W *)pM2)[5].Be_flg |= 0xA;
    pM2 = &((PARTS_W *)pM2)[5].Workm;
    pP->Old_x = 0;
    pP->Timer3 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Timer1 = 15;
    pP->Rot_spd_x = 0;
    pP->Rot_spd_y = 0;
    pP->Rot_spd_z = 0;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM2, NULL, NULL);
    Esp_call(0, 0x10000000, pM2, NULL, NULL);

    pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
    pM3 = &pP->Workm;
    pP->Old_x = 0;
    pP->Timer3 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Timer1 = 15;
    pP->Rot_spd_x = 0;
    pP->Rot_spd_y = 0;
    pP->Rot_spd_z = 0;
    pP->Be_flg |= 0xA;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM3, NULL, NULL);
    Esp_call(0, 0x10000000, pM3, NULL, NULL);

    pM4 = (MATRIX *)EM36_W(pEm)->pSub->pSin_parts_ptr;
    pP = &((PARTS_W *)pM4)[3];
    ((PARTS_W *)pM4)[3].Be_flg |= 0xA;
    pM4 = &((PARTS_W *)pM4)[3].Workm;
    pP->Old_x = 0;
    pP->Timer3 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Timer1 = 15;
    pP->Rot_spd_x = 0;
    pP->Rot_spd_y = 0;
    pP->Rot_spd_z = 0;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM4, NULL, NULL);
    Esp_call(0, 0x10000000, pM4, NULL, NULL);
}

/* Routine_0 3: die */
void Em36_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        if (pEm->Damage_flg & 0x100)
            Em34_blood_spray(pEm);
        if (pEm->Damage_flg & 0x400)
            Em34_blood_spray(pEm);
        if (pEm->Damage_flg & 0x200)
            Em36_knock_back2(pEm);
        if (pEm->Damage_flg & 0x800)
            Em36_knock_back(pEm);
    }
    if (EM36_W(pEm)->Flg & 2) {
        if (pEm->Damage_no == 30) {
            Em_dead_flg_on_cand(pEm);
            EM_SET_RTN(pEm, 4, 0xA, 0, 0);
            pEm->Life = -1;
        } else {
            pEm->Life = 0;
            pEm->Damage_cnt &= 0x7F;
            EM_RTN(pEm) = pEm->Old_routine;
            Em34_blood_set(pEm);
            Em36_Move(pEm, pKan, pSeq);
        }
    } else {
        Em36_die_burn(pEm, pKan, pSeq);
    }
}

/* die: fall down and burn */
void Em36_die_burn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX *pM;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xD, 0, 0x1F);
        pEm->Routine_2 = 1;
        if ((UCHAR)pEm->Type != 3)
            Snd_se_enem(13, pEm);
        else
            Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        G.Unk785C = 0;
        G.Status_flg &= ~0x40000000;
        if (G.Status_flg & 0x2000)
            Om_drop_set(pEm, 10);
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        Em_dead_flg_on_cand(pEm);
        if (pEm->Type & 1) {
            EM_SET_RTN(pEm, 4, 0xA, 0, 0);
            pEm->Life = -1;
            return;
        }
        pEm->Be_flg |= 0x8000;
    case 1:
        if (pEm->Move_cnt == 45) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(11, pEm);
            else
                Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
        }
        if (pEm->Move_cnt < 104 && pEm->Move_cnt % 7 == 1) {
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            Ofs.vx = 100 - (Rnd() & 1) * 200;
            Ofs.vy = 0;
            Ofs.vz = 100 - (Rnd() & 1) * 200;
            pM = &((PARTS_W *)pM)[3].Workm;
            Esp_call(0x4000000, ((Rnd() * 8 + 0x1000) << 16) | (USHORT)(Rnd() * 16), pM, &Ofs, NULL);
            Esp_call(0x4001700, ((Rnd() * 8 + 0x1000) << 16) | (USHORT)(Rnd() * 16), pM, &Ofs, NULL);
            Esp_call(0x4000500, 0x10000000, pM, &Ofs, NULL);
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            Esp_call(0x4001700, ((Rnd() * 8 + 0xF00) << 16) | (USHORT)(Rnd() * 16), &((PARTS_W *)pM)[2].Workm, NULL, NULL);
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            pM = &((PARTS_W *)pM)[5].Workm;
            Esp_call(0x4001700, ((Rnd() * 8 + 0x800) << 16) | (USHORT)(Rnd() * 16), pM, NULL, NULL);
            Esp_call(0x4000500, 0x10000000, pM, NULL, NULL);
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 7, 0, 0, 0);
            pEm->Be_flg &= ~0x8000;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* Routine_0 4: scenario routine by Routine_1 (RE2 Em2a_sce) */
void Em36_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Type & 0x100) {
        Em_sce_main(pEm, pKan, pSeq);
        return;
    }
    Em36_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

/* sce: stand */
void Em36_sce_stand(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
    }
}

/* sce: walk to Dest */
void Em36_sce_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
    }
}

/* sce: play motion Add_dir from the sub animation */
void Em36_sce_motion(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE_V(pEm, pEm->Add_dir, 0, 0);
        EM36_W(pEm)->Flg |= 1;
    case 1:
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->Unk14A & 8 && G.Status_flg & 0x2000)
            Om_drop_set(pEm, 30);
        break;
    case 2:
        if (pEm->Unk14A) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            EM36_W(pEm)->Flg &= ~1;
        }
    }
}

/* sce: turn around */
void Em36_sce_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 4, 0, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Cdir_y += 0x800;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* sce: play motion Add_dir from the sub animation, stop at frame Unk14C */
void Em36_sce_motion_stop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE_V(pEm, pEm->Add_dir, 0, 0);
        EM36_W(pEm)->Flg |= 1;
    case 1:
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == pEm->Unk14C)
            pEm->Routine_2 = 2;
        break;
    case 2:
        break;
    case 3:
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        if (pEm->Unk14A) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            EM36_W(pEm)->Flg &= ~1;
        }
    }
}

#include "enemy/common/em34/Em34_sce05.inc.c"

/* sce: jump forward, then slide to a stop */
void Em36_sce_jump(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE_V(pEm, pEm->Add_dir, 7, 0);
        Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        pEm->Spd_x = 600;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Move_cnt--;
        if (pEm->Unk14A & 1)
            pEm->Routine_2 = 1;
        EM36_W(pEm)->Flg |= 1;
        pEm->Damage_cnt |= 0x80;
        break;
    case 1:
        Add_speedXZ(pEm, 0);
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += 30;
        if (pEm->Pos_y > -100) {
            pEm->Pos_y = 0;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        pEm->Move_cnt = 0;
        pEm->Move_no++;
        pEm->Hokan_flg = 3;
        pEm->Routine_2 = 3;
        EM36_W(pEm)->Flg &= ~1;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk14A |= 0x40;
        pEm->Timer0 = pEm->Spd_x;
    case 3:
        if ((pEm->Timer0 -= 50) < 0)
            pEm->Timer0 = 0;
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Spd_x += pEm->Timer0;
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Routine_2 = 4;
            pEm->Unk14A |= 0x80;
            pEm->Cdir_y += 0x800;
            if (pEm->Unk14A & 2) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        break;
    case 5:
        pEm->Routine_2 = 6;
        pEm->Move_cnt = 0;
        pEm->Hokan_flg = 0;
        pEm->Timer0 = 600;
    case 6:
        if ((pEm->Timer0 -= 50) < 0)
            pEm->Timer0 = 0;
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Spd_x += pEm->Timer0;
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Routine_2 = 7;
            pEm->Unk14A |= 0x80;
            pEm->Cdir_y += 0x800;
            if (pEm->Unk14A & 2) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 7:
        break;
    }
}

/* sce: jump with speed Unk14C / gravity Unk14E, land at Ground */
void Em36_sce_leap(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE_V(pEm, pEm->Add_dir, 0, 0);
        Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        pEm->Move_cnt--;
        pEm->Spd_y = pEm->Unk14C;
        if (pEm->Unk14A & 1)
            pEm->Routine_2 = 1;
        EM36_W(pEm)->Flg |= 1;
        break;
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x10400);
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += pEm->Unk14E;
        if (pEm->Spd_y > 0)
            pEm->Unk14A |= 0x20;
        if (pEm->Pos_y > pEm->Ground - 100 && pEm->Spd_y > 0) {
            pEm->Routine_2 = 3;
            pEm->Pos_y = pEm->Ground;
        }
        break;
    case 2:
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += pEm->Unk14E;
        if (pEm->Spd_y > 0)
            pEm->Unk14A |= 0x20;
        if (pEm->Pos_y > pEm->Ground - 100 && pEm->Spd_y > 0) {
            pEm->Routine_2 = 3;
            pEm->Pos_y = pEm->Ground;
        }
        break;
    case 3:
        pEm->Move_cnt = 10;
        pEm->Hokan_flg = 3;
        pEm->Routine_2 = 4;
        pEm->Move_no++;
        Snd_se_enem(10, pEm);
        EM36_W(pEm)->Flg &= ~1;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk14A |= 0x40;
    case 4:
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Routine_2 = 5;
            if ((pEm->Unk14A |= 0x80) & 2) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
            if (pEm->Unk14A & 4) {
                EM_SET_RTN(pEm, 1, 2, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 5:
        break;
    }
}

/* sce: jump down (Add_dir motion, then fall) */
void Em36_sce_jump_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE_V(pEm, pEm->Add_dir, 0, 0xF);
        if (pEm->Unk14A & 1)
            pEm->Routine_2 = 1;
        EM36_W(pEm)->Flg |= 1;
        break;
    case 1:
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x100)) {
            pEm->Hokan_flg = 3;
            pEm->Routine_2 = 2;
            pEm->Spd_y = 0;
            pEm->Move_no++;
            pEm->Spd_x += 120;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Hokan_flg = 7;
            pEm->Routine_2 = 3;
            pEm->Move_cnt++;
        }
        Add_speedXZ(pEm, 0);
    case 3:
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += 30;
        if (pEm->Pos_y > -100) {
            pEm->Pos_y = 0;
            pEm->Routine_2 = 4;
        }
        break;
    case 4:
        pEm->Move_cnt = 10;
        pEm->Hokan_flg = 3;
        pEm->Routine_2 = 5;
        pEm->Move_no++;
        Snd_se_enem(10, pEm);
        EM36_W(pEm)->Flg &= ~1;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk14A |= 0x40;
    case 5:
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (pEm->Move_cnt == 10) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Routine_2 = 6;
            if ((pEm->Unk14A |= 0x80) & 2) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 6:
        break;
    }
}

/* sce: die (parts fall off) */
void Em36_sce_die_fall(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pParts;
    PARTS_W *pP;
    MATRIX *pM;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE_V(pEm, pEm->Add_dir, 0, 7);
        pEm->Damage_cnt |= 0x80;
        Snd_se_enem(13, pEm);
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        EM36_W(pEm)->Dm_stage = 1;
        Esp_call(0x1C03, 0xA000000, &pEm->pSin_parts_ptr[2].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[9].Workm, NULL, NULL);
        Snd_se_stad(0x10204, POS_V(pEm), 0, 0);
    case 1:
        if (!(pEm->Move_cnt % 60))
            Em36_parts_effect(pEm);
        if (pEm->Move_cnt == 30) {
        pParts = pEm->pSin_parts_ptr;
        pP = &pParts[21];
        pM = &pParts[21].Workm;
        pParts[21].Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Timer3 = 0;
        pP->Old_z = 0;
        pP->Old_y = 0;
        pP->Timer1 = 5;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = pEm->Cdir_y + 0x800;
        pP->Rot_spd_z = -900;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        }
        if (pEm->Move_cnt == 60) {
        pParts = pEm->pSin_parts_ptr;
        pP = &pParts[20];
        pM = &pParts[20].Workm;
        pParts[20].Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Timer3 = 0;
        pP->Old_z = 0;
        pP->Old_y = 0;
        pP->Timer1 = 5;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = pEm->Cdir_y + 0x600;
        pP->Rot_spd_z = -724;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        pParts = pEm->pSin_parts_ptr;
        pP = &pParts[19];
        pM = &pParts[19].Workm;
        pParts[19].Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Timer3 = 0;
        pP->Old_z = 0;
        pP->Old_y = 0;
        pP->Timer1 = 5;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = pEm->Cdir_y + 0x600;
        pP->Rot_spd_z = -824;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        }
        if (pEm->Move_cnt == 80) {
        pP = &pEm->pSin_parts_ptr[16];
        pM = &pP->Workm;
        pP->Be_flg |= 0xA;
        pP->Old_y = 20;
        pP->Old_x = 0;
        pP->Timer3 = 0;
        pP->Old_z = 0;
        pP->Timer1 = 9;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = pEm->Cdir_y - 0x32C;
        pP->Rot_spd_z = -624;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        Esp_call(0x1700, 0x10000000, pM, NULL, NULL);
        }
        if (pEm->Move_cnt == 85) {
        pP = &pEm->pSin_parts_ptr[17];
        pM = &pP->Workm;
        pP->Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Timer3 = 0;
        pP->Old_z = 0;
        pP->Old_y = 30;
        pP->Timer1 = 9;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = pEm->Cdir_y - 0x200;
        pP->Rot_spd_z = -524;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        Esp_call(0x1C03, 0x8000800, pM, NULL, NULL);
        Esp_call(0x1700, 0x10000000, pM, NULL, NULL);
        }
        if (pEm->Move_cnt == 99) {
        pP = &pEm->pSin_parts_ptr[18];
        pM = &pP->Workm;
        pP->Be_flg |= 0xA;
        pP->Old_y = 40;
        pP->Old_x = 0;
        pP->Timer3 = 0;
        pP->Old_z = 0;
        pP->Timer1 = 9;
        pP->Rot_spd_x = 0;
        pP->Rot_spd_y = pEm->Cdir_y;
        pP->Rot_spd_z = -424;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        Esp_call(0x1C03, 0x8000800, pM, NULL, NULL);
        Esp_call(0x1700, 0x10000000, pM, NULL, NULL);
        }
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200)) {
            pEm->Routine_2 = 2;
            pEm->Damage_cnt &= 0x7F;
            pEm->Unk14A |= 0x80;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        break;
    }
}

/* sce: die (parts blown off) */
void Em36_sce_die_blow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Pos;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Routine_2 = 1;
        Em_dead_flg_on_cand(pEm);
        pEm->Damage_cnt |= 0x80;
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[9].Workm, NULL, NULL);
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pP = &((PARTS_W *)pM)[2];
        Pos.vx = pP->Pos_x;
        Pos.vy = pP->Pos_y;
        Pos.vz = pP->Pos_z;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1C03, 0xA000000, pM, &Pos, NULL);
        Esp_call(0x1C03, 0x10000800, pM, &Pos, NULL);
        pP = &pEm->pSin_parts_ptr[6];
        Pos.vx = pP->Pos_x;
        Pos.vy = pP->Pos_y;
        Pos.vz = pP->Pos_z;
        Esp_call(0x1C03, 0x10000000, pM, &Pos, NULL);
        Esp_call(0x1C03, 0x10000800, pM, &Pos, NULL);
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pP = &((PARTS_W *)pM)[4];
        Pos.vx = pP->Pos_x;
        Pos.vy = pP->Pos_y;
        Pos.vz = pP->Pos_z;
        pM = &((PARTS_W *)pM)[4].Workm;
        Esp_call(0x1C03, 0x10000000, pM, &Pos, NULL);
        Esp_call(0x1C03, 0xA000000, pM, &Pos, NULL);
    case 1:
        if (pEm->Move_cnt == 18) {
            if (EM36_W(pEm)->Dm_stage < 2)
                Em36_blow_off6(pEm);
            if (EM36_W(pEm)->Dm_stage < 3) {
                Em36_blow_off22(pEm);
                pEm->Type = (pEm->Type & 0xFFF0) | 3;
            }
        }
        if (pEm->Move_cnt == 90)
            Em36_blow_off23(pEm);
        if (pEm->Move_cnt == 25)
            Snd_se_stad(0x1020D, POS_V(pEm), 0, 0);
        Spd_get2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200)) {
            pEm->Routine_2 = 2;
            pEm->Unk14A |= 0x80;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}

/* sce: grab the work Add_dir (G.pEnemy[0][]) and throw it */
void Em36_sce_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pT;
    SHORT Base_x, Base_z;
    LONG Unused[6];

    pT = G.pEnemy[pEm->Add_dir];
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x13, 0, 0xF);
        pEm->Routine_2 = 1;
        Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x800);
        pT->pEnemy_ptr = pEm;
        Set_base_pos(pT, pKan, pSeq, 0);
        pT->Neck_flg |= 0x12;
        pT->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x800;
        pT->Be_flg |= 0x800;
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 4;
        EM36_W(pEm)->pSub->Routine_2 = 0;
    case 1:
        pEm->Base_pos_x = pT->Base_pos_x;
        pEm->Base_pos_z = pT->Base_pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0x14, 0, 0);
            pEm->UnkE3 = 4;
        }
        break;
    case 2:
        pEm->Base_pos_x = pT->Base_pos_x;
        pEm->Base_pos_z = pT->Base_pos_z;
        Spd_get(pEm, pKan, pSeq, 0);
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (!--pEm->UnkE3) {
                pEm->Routine_2 = 3;
                EM_SET_MOVE(pEm, 0x15, 0, 0);
                EM36_W(pEm)->pSub->Routine_0 = 1;
                EM36_W(pEm)->pSub->Routine_1 = 4;
                EM36_W(pEm)->pSub->Routine_2 = 3;
            }
        }
        break;
    case 3:
        if (!pEm->Move_cnt) {
            Base_x = pEm->Spd_base_x;
            Base_z = pEm->Spd_base_z;
        }
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x -= Base_x;
            pEm->Spd_z -= Base_z;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 4;
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        break;
    }
}

/* Routine_0 7: fade out the shadow */
void Em36_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pK = pEm->Kage.pKw;

    switch (pEm->Routine_1) {
    case 0:
        pEm->Col.Id = 0;
        pEm->Life = -1;
        pEm->Routine_1 = 1;
        pEm->Neck_flg |= 1;
        Em_dead_flg_on_cand(pEm);
        pEm->UnkE2 = 120;
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 7;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0;
            EM36_W(pEm)->pSub->Spd_x = 0;
        }
    case 1:
        if ((KAGE_RGB(pK, 0) & 0xFF) != 0x10) {
            KAGE_RGB(pK, 0) -= 0x10001;
            KAGE_RGB(pK, 1) -= 0x10001;
        }
        if ((KAGE_RGB(pK, 0) & 0xFF00) != 0xAF00) {
            KAGE_RGB(pK, 0) += 0x100;
            KAGE_RGB(pK, 1) += 0x100;
        }
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pK->Half_x += 5;
            pK->Half_z += 5;
            if (!pEm->UnkE2)
                pEm->Routine_1 = 2;
        } else {
            pEm->Routine_1 = 2;
        }
        break;
    case 2:
        break;
    }
}

/* G.pPldamage_Em hook (RE2 PLDAMEGE_EM): the player's damage routine by Routine_1 */
void Pldamege_Em36(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldamage_Em36_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* player damage: grabbed and thrown */
void Em36_pldm_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pE;
    SHORT Old_x, Old_z, Cdir;
    UCHAR Cnt;

    pE = pEm->pEnemy_ptr;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Neck_flg |= 0x12;
        Snd_se_stad(0x10002, POS_V(pEm), 0, 0);
        pEm->Damage_no = 24;
        pEm->Timer0 = pEm->Col.D0;
        pEm->Col.W0 = 0;
        pEm->Col.D0 = 0;
        pEm->UnkE2 = 8;
        if (pEm->UnkDA > 2000)
            pEm->UnkE2 = 5;
        if (pEm->UnkDA > 3000)
            pEm->UnkE2 = 3;
        if (pEm->UnkDA > 3500)
            pEm->UnkE2 = 0;
        pEm->Damage_no = 0;
        pEm->Be_flg |= 0x2000;
    case 1:
        if (pEm->Move_cnt < 10)
            Goto01(pEm, pE->Cdir_y + 0x800, 0x100);
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        Cdir = pEm->Cdir_y;
        Cnt = pEm->Move_cnt;
        pEm->Cdir_y = pE->Cdir_y + 0x1000;
        if (pEm->Move_cnt < pEm->UnkE2)
            pEm->Move_cnt = pEm->UnkE2;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y = Cdir;
        pEm->Move_cnt = Cnt;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 3;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 1, 0, 0);
            pEm->Be_flg |= 0x2000;
            Goto01(pEm, pE->Cdir_y + 0x800, 0x800);
            pEm->Base_pos_x = pE->Base_pos_x;
            pEm->Base_pos_z = pE->Base_pos_z;
        }
        break;
    case 2:
        G.Status_flg |= 0x40;
        pEm->Cdir_y += 0x800;
        Spd_get(pEm, pKan, pSeq, 0);
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            if (!--pEm->UnkE3) {
                pEm->Routine_2 = 3;
                EM_SET_MOVE(pEm, 2, 0, 0);
                pEm->Be_flg &= ~0x2000;
                pEm->Col.W0 = pEm->Timer0;
                pEm->Col.D0 = pEm->Timer0;
                Pad_vib_set0(2, 0);
                Pad_vib_fade_set(45, 150, 0, 0);
                G.Status_flg &= ~0x40;
            }
        }
        if (*pEm->pNow_seq & 0x200) {
            Snd_se_stad(0x10000, POS_V(pEm), 0, 0);
            Pad_vib_set0(4, 0);
            Pad_vib_set1(6, 250, 0);
            Pad_vib_fade_set(25, 150, 40, 6);
        }
        break;
    case 3:
        if (!pEm->Move_cnt) {
            Old_x = pEm->Spd_base_x;
            Old_z = pEm->Spd_base_z;
        }
        pEm->Cdir_y += 0x800;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (!pEm->Move_cnt) {
            pEm->Spd_x -= Old_x;
            pEm->Spd_z -= Old_z;
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            if (pEm->Routine_0 != 6) {
                pEm->Routine_2 = 4;
                pEm->Be_flg &= ~0x800;
                pE->Be_flg &= ~0x800;
                EM_SET_MOVE(pEm, 6, 0, 7);
                pEm->UnkE3 = 0;
                pEm->Damage_no = 0x80;
                pEm->Damage_cnt &= 0x7F;
            } else {
                EM_SET_RTN(pEm, 7, 0, 0, 0);
            }
        }
        Add_speedXZ(pEm, 0x800);
        if (*pEm->pNow_seq & 0x200) {
            Snd_se_stad(0x10000, POS_V(pEm), 0, 0);
            Snd_se_stad(0x10229, POS_V(pEm), 0, 0);
            Pad_vib_set0(3, 0);
            Pad_vib_set1(6, 200, 0);
            Pad_vib_set0(2, 8);
            Pad_vib_set1(4, 200, 8);
        }
        break;
    case 4:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Gacha_ch() && pEm->Life > 50)
            pEm->UnkE3 = 1;
        if (!(pEm->Move_cnt & 1) && pEm->UnkE3 && pEm->Move_cnt < 55) {
            Joint_move2(pEm, pKan, pSeq, 0x200);
            pEm->UnkE3 = 0;
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 5;
            EM_SET_MOVE(pEm, 5, 0, 0xF);
            pEm->UnkE3 = 45;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 5:
        Joint_move2(pEm, pKan, pSeq, 0x100);
        pEm->UnkE3 = pEm->UnkE3 - 1 - Gacha_ch() * 15;
        if (pEm->UnkE3 < 0) {
            pEm->Routine_2 = 6;
            EM_SET_MOVE(pEm, 9, 0, 7);
        }
        break;
    case 6:
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200)) {
            pEm->Routine_2 = 7;
            EM_SET_MOVE(pEm, 5, 0, 3);
            pEm->UnkE3 = 0;
        }
        break;
    case 7:
        if (Gacha_ch() && pEm->Life > 20)
            pEm->UnkE3 = 1;
        if (!(pEm->Move_cnt & 1) && pEm->UnkE3 && pEm->Move_cnt < 15) {
            Joint_move2(pEm, pKan, pSeq, 0x10400);
            pEm->UnkE3 = 0;
        }
        if (Joint_move2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x10400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Neck_flg &= ~0x12;
            pEm->Damage_cnt &= 0x7F;
        }
        if (pEm->Move_cnt == 15 || pEm->Move_cnt == 16)
            pEm->Damage_no = 0;
        break;
    case 8:
        EM_SET_MOVE(pEm, 7, 0, 3);
        pEm->Routine_2 = 9;
        Snd_se_stad(0x10000, POS_V(pEm), 0, 0);
    case 9:
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->UnkE3 = 5;
            pEm->Routine_2 = 6;
            EM_SET_MOVE(pEm, 9, 7, 7);
        }
        break;
    }
}

/* player damage: thrown by the punch */
void Em36_pldm_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pE;
    SHORT Cdir;

    pE = pEm->pEnemy_ptr;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Neck_flg |= 0x12;
        Snd_se_stad(0x10001, POS_V(pEm), 0, 0);
        pEm->Damage_no = 24;
    case 1:
        if (pEm->Move_cnt < 10)
            Goto01(pEm, pE->Cdir_y + 0x800, 0x100);
        Cdir = pEm->Cdir_y;
        pEm->Cdir_y = pE->Cdir_y + 0x1000;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Cdir_y = Cdir;
        if (pEm->Move_cnt >= 13)
            pEm->Spd_x = (pEm->Timer0 * pEm->Spd_x) / 4413;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 4, 0, 7);
            pEm->UnkE3 = 0;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        Add_speedXZ(pEm, 0x800);
        if (*pEm->pNow_seq & 0x200) {
            Snd_se_stad(0x10000, POS_V(pEm), 0, 0);
            Pad_vib_set0(3, 0);
            Pad_vib_set1(15, 150, 0);
            Pad_vib_fade_set(30, 150, 0, 15);
        }
        if (pEm->Move_cnt >= 16 && pEm->Move_cnt < 35 && pEm->Move_cnt & 3)
            Pad_vib_set0((Rnd() + 1) & 1, 0);
        break;
    case 2:
        if (Gacha_ch() && pEm->Life > 70)
            pEm->UnkE3 = 1;
        if (pEm->Id < 8) {
            if (!(pEm->Move_cnt & 1) && pEm->UnkE3 && pEm->Move_cnt < 45) {
                Joint_move2(pEm, pKan, pSeq, 0x200);
                pEm->UnkE3 = 0;
            }
            if (pEm->Move_cnt == 50) {
                pEm->Neck_flg &= ~0x12;
                pEm->Damage_cnt &= 0x7F;
            }
        } else {
            Spd_get(pEm, pKan, pSeq, 0);
            if (pEm->Move_cnt == 35) {
                pEm->Neck_flg &= ~0x12;
                pEm->Damage_cnt &= 0x7F;
            }
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* unused */
void Em36_dummy0(void)
{
}

/* G.pPldie_Em hook (RE2 PLDIE_EM): the player's death routine by Routine_1 */
void Pldie_Em36(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldie_Em36_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* unused */
void Em36_dummy1(void)
{
}

/* dodge timer: step aside when the player aims and fires */
void Em36_dodge(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = G_PLAYER;
    LONG Div;
    LONG Time;

    EM36_W(pEm)->Flg &= ~0x10;
    if (EM36_W(pEm)->Dodge_timer & 0x7F)
        EM36_W(pEm)->Dodge_timer--;
    if ((EM36_FLGW(pEm) & 0xFF000000) == 0x80000000) {
        EM36_W(pEm)->Flg |= 0x10;
        EM36_W(pEm)->Dodge_timer &= 0x7F;
    }
    if (!(EM36_W(pEm)->Dodge_timer & 0x80) && *(USHORT *)&pPl->Routine_0 == 0x501 && !Cdir_ck_l(pEm, pPl)
        && !Goto00_ck(pPl, pEm->Pos_x, pEm->Pos_z, 0x80) && G.Unk75E8 & 1) {
        Div = 0;
        if ((UCHAR)pPl->Type == 10) {
            Div = 7000;
            if (!(Rnd() & 1))
                Div = 760;
        }
        if ((ULONG)((UCHAR)pPl->Type - 6) < 4) {
            Div = 7000;
            if (!(Rnd() & 1))
                Div = 640;
        }
        if (Div) {
            Time = (LONG)pEm->L_pl / Div - 5;
            if (Time >= 0)
                EM36_W(pEm)->Dodge_timer = Time | 0x80;
        }
    }
}

/* head shake when hit (part 1) */
void Em36_head_shake(ENEMY_WORK *pEm)
{
    SVECTOR Rot;
    MATRIX M;
    MATRIX Unused;
    LONG Unused2[4];
    signed char Swing[8] = { 8, 12, -8, -12, -4, 0, 4, 0 };

    switch (EM36_W(pEm)->Shake_no) {
    case 0:
        break;
    case 1:
        EM36_W(pEm)->Shake_amp = Swing[Rnd() & 7];
        EM36_W(pEm)->Shake_no = 2;
        EM36_W(pEm)->Shake_cnt = 0;
        EM36_W(pEm)->Shake_side = 1;
        if (pEm->Damage_flg & 0x10)
            EM36_W(pEm)->Shake_side = -1;
    case 2: {
        PARTS_W *pP = pEm->pSin_parts_ptr;

        Rot.vx = 0;
        Rot.vy = -(EM36_W(pEm)->Shake_cnt * EM36_W(pEm)->Shake_amp * 8) * EM36_W(pEm)->Shake_side;
        Rot.vz = -(EM36_W(pEm)->Shake_cnt * 64) * EM36_W(pEm)->Shake_side;
        RotMatrix(&Rot, &M);
        MulMatrix((MATRIX *)pP[1].M, &M);
        if (EM36_W(pEm)->Shake_cnt++ >= 3) {
            EM36_W(pEm)->Shake_no = 3;
            EM36_W(pEm)->Shake_cnt = 16;
        }
        break;
    }
    case 3: {
        PARTS_W *pP = pEm->pSin_parts_ptr;

        Rot.vx = 0;
        Rot.vy = -(EM36_W(pEm)->Shake_cnt * EM36_W(pEm)->Shake_amp) * EM36_W(pEm)->Shake_side;
        Rot.vz = -(EM36_W(pEm)->Shake_cnt * 8) * EM36_W(pEm)->Shake_side;
        RotMatrix(&Rot, &M);
        MulMatrix((MATRIX *)pP[1].M, &M);
        if (--EM36_W(pEm)->Shake_cnt == -1)
            EM36_W(pEm)->Shake_no = 0;
        break;
    }
    }
}

#include "enemy/common/em34/Em34_foot_set.inc.c"

#include "enemy/common/em34/Em34_blood_set.inc.c"

/* unused */
void Em36_dummy2(void)
{
}

/* knocked back by a strong hit: lean the body away from the player */
void Em36_knock_back(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    PARTS_W *pP2;
    PARTS_W *pP3;

    if (pEm->Be_flg & 8)
        return;
    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    Sv.vx = 0;
    Sv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - pEm->Cdir_y + 0x100;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    Sv.vx = 0x7FA;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Add_col);
    pEm->Col_flg = 0x230;
    if ((pEm->Damage_flg & 3) == 1) {
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, (Rnd() * 8 + 0x800) << 16, &pP[10].Workm, NULL, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, (Rnd() * 8 + 0x800) << 16, &pP[14].Workm, NULL, NULL);
    } else {
        pP2 = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd();
        Sv.vy = -0xAC - Rnd();
        Sv.vz = 0x80 - Rnd();
        Esp_call(0x1C03, (Rnd() * 8 + 0x800) << 16, &pP2[9].Workm, &Sv, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, (Rnd() * 8 + 0x800) << 16, &pP[2].Workm, NULL, NULL);
    }
    pP3 = &pEm->pSin_parts_ptr[0];
    pP3->UnkA2 |= 0x40;
    pP3 = &pEm->pSin_parts_ptr[9];
    pP3->UnkA2 |= 0x40;
}

#include "enemy/common/em34/Em34_blood_spray.inc.c"

/* knocked back (weaker variant of Em36_knock_back) */
void Em36_knock_back2(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    PARTS_W *pP2;
    PARTS_W *pP3;

    if (pEm->Be_flg & 8)
        return;
    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    Sv.vx = 0;
    Sv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Player.Pos_x, G.Player.Pos_z) - pEm->Cdir_y + 0x100;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    Sv.vx = 0xC00;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Add_col);
    pEm->Col_flg = 0x40;
    if ((pEm->Damage_flg & 3) == 1) {
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1803, (Rnd() * 8 + 0x800) << 16, &pP[10].Workm, NULL, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1803, (Rnd() * 8 + 0x800) << 16, &pP[14].Workm, NULL, NULL);
    } else {
        pP2 = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd();
        Sv.vy = -0xAC - Rnd();
        Sv.vz = 0x80 - Rnd();
        Esp_call(0x1803, (Rnd() * 8 + 0x800) << 16, &pP2[9].Workm, &Sv, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1803, (Rnd() * 8 + 0x800) << 16, &pP[2].Workm, NULL, NULL);
    }
    pP3 = &pEm->pSin_parts_ptr[0];
    pP3->UnkA2 |= 0x40;
    pP3 = &pEm->pSin_parts_ptr[9];
    pP3->UnkA2 |= 0x40;
}

/* morph of part 2 (mouth) driven by seq flag 0x800 */
void Em36_mouth_morph(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG Mode;

    EM36_W(pEm)->Mouth = 0;
    pP = &pEm->pSin_parts_ptr[2];
    if (*pEm->pNow_seq & 0x800)
        EM36_W(pEm)->Mouth = 1;
    switch (Mode = EM36_W(pEm)->Mouth) {
    case 0:
        EM36_SABUN_P(pP)->Key[0].Rate -= 0x100;
        if (EM36_SABUN_P(pP)->Key[0].Rate <= 0)
            EM36_SABUN_P(pP)->Key[0].Rate = 0;
        break;
    case 1:
        EM36_SABUN_P(pP)->Key[0].Rate += 0x100;
        if (EM36_SABUN_P(pP)->Key[0].Rate >= 0x1000) {
            EM36_SABUN_P(pP)->Key[0].Rate = 0x1000;
            EM36_W(pEm)->Mouth = Mode;
        }
        break;
    }
    func_8004A934_(pEm, pP, Mode);
}

/* morph of parts 16..21 (cycle through the 7 keys) */
void Em36_body_morph(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG Add;
    LONG i;

    i = 6;
    pP = &pEm->pSin_parts_ptr[16];
    Add = 0x400;
    if (pEm->Routine_0 == 2 || pEm->Routine_0 == 3)
        Add = 0x800;
    if (pEm->Routine_0 == 7 || *(SHORT *)&pEm->Routine_0 == 0x801)
        Add = 0x80;
    do {
        if (pEm->Routine_0 == 7) {
            pP->Size_y -= 8;
            if (pP->Size_y < 0x600)
                pP->Size_y = 0x600;
        }
        switch (EM36_PF(pP)->Key_no) {
        case 0:
            EM36_SABUN_P(pP)->Key[4].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[4].Rate <= 0)
                EM36_SABUN_P(pP)->Key[4].Rate = 0;
            EM36_SABUN_P(pP)->Key[6].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[6].Rate <= 0)
                EM36_SABUN_P(pP)->Key[6].Rate = 0;
            EM36_SABUN_P(pP)->Key[0].Rate += Add;
            if (EM36_SABUN_P(pP)->Key[0].Rate >= 0x1000) {
                EM36_SABUN_P(pP)->Key[0].Rate = 0x1000;
                EM36_PF(pP)->Key_no = 1;
                if (Rnd() & 1)
                    EM36_PF(pP)->Key_no = 3;
            }
            break;
        case 1:
            EM36_SABUN_P(pP)->Key[5].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[5].Rate <= 0)
                EM36_SABUN_P(pP)->Key[5].Rate = 0;
            EM36_SABUN_P(pP)->Key[0].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[0].Rate <= 0)
                EM36_SABUN_P(pP)->Key[0].Rate = 0;
            EM36_SABUN_P(pP)->Key[1].Rate += Add;
            if (EM36_SABUN_P(pP)->Key[1].Rate >= 0x1000) {
                EM36_SABUN_P(pP)->Key[1].Rate = 0x1000;
                EM36_PF(pP)->Key_no = 2;
                if (Rnd() & 1)
                    EM36_PF(pP)->Key_no = 4;
            }
            break;
        case 2:
            EM36_SABUN_P(pP)->Key[6].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[6].Rate <= 0)
                EM36_SABUN_P(pP)->Key[6].Rate = 0;
            EM36_SABUN_P(pP)->Key[1].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[1].Rate <= 0)
                EM36_SABUN_P(pP)->Key[1].Rate = 0;
            EM36_SABUN_P(pP)->Key[2].Rate += Add;
            if (EM36_SABUN_P(pP)->Key[2].Rate >= 0x1000) {
                EM36_SABUN_P(pP)->Key[2].Rate = 0x1000;
                EM36_PF(pP)->Key_no = 3;
                if (Rnd() & 1)
                    EM36_PF(pP)->Key_no = 5;
            }
            break;
        case 3:
            EM36_SABUN_P(pP)->Key[0].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[0].Rate <= 0)
                EM36_SABUN_P(pP)->Key[0].Rate = 0;
            EM36_SABUN_P(pP)->Key[2].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[2].Rate <= 0)
                EM36_SABUN_P(pP)->Key[2].Rate = 0;
            EM36_SABUN_P(pP)->Key[3].Rate += Add;
            if (EM36_SABUN_P(pP)->Key[3].Rate >= 0x1000) {
                EM36_SABUN_P(pP)->Key[3].Rate = 0x1000;
                EM36_PF(pP)->Key_no = 4;
                if (Rnd() & 1)
                    EM36_PF(pP)->Key_no = 6;
            }
            break;
        case 4:
            EM36_SABUN_P(pP)->Key[1].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[1].Rate <= 0)
                EM36_SABUN_P(pP)->Key[1].Rate = 0;
            EM36_SABUN_P(pP)->Key[3].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[3].Rate <= 0)
                EM36_SABUN_P(pP)->Key[3].Rate = 0;
            EM36_SABUN_P(pP)->Key[4].Rate += Add;
            if (EM36_SABUN_P(pP)->Key[4].Rate >= 0x1000) {
                EM36_SABUN_P(pP)->Key[4].Rate = 0x1000;
                EM36_PF(pP)->Key_no = 5;
                if (Rnd() & 1)
                    EM36_PF(pP)->Key_no = 0;
            }
            break;
        case 5:
            EM36_SABUN_P(pP)->Key[2].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[2].Rate <= 0)
                EM36_SABUN_P(pP)->Key[2].Rate = 0;
            EM36_SABUN_P(pP)->Key[4].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[4].Rate <= 0)
                EM36_SABUN_P(pP)->Key[4].Rate = 0;
            EM36_SABUN_P(pP)->Key[5].Rate += Add;
            if (EM36_SABUN_P(pP)->Key[5].Rate >= 0x1000) {
                EM36_SABUN_P(pP)->Key[5].Rate = 0x1000;
                EM36_PF(pP)->Key_no = 6;
                if (Rnd() & 1)
                    EM36_PF(pP)->Key_no = 1;
            }
            break;
        case 6:
            EM36_SABUN_P(pP)->Key[3].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[3].Rate <= 0)
                EM36_SABUN_P(pP)->Key[3].Rate = 0;
            EM36_SABUN_P(pP)->Key[5].Rate -= Add;
            if (EM36_SABUN_P(pP)->Key[5].Rate <= 0)
                EM36_SABUN_P(pP)->Key[5].Rate = 0;
            EM36_SABUN_P(pP)->Key[6].Rate += Add;
            if (EM36_SABUN_P(pP)->Key[6].Rate >= 0x1000) {
                EM36_SABUN_P(pP)->Key[6].Rate = 0x1000;
                EM36_PF(pP)->Key_no = 0;
                if (Rnd() & 1)
                    EM36_PF(pP)->Key_no = 2;
            }
            break;
        }
        Sa_morph_step(pEm, pP);
        pP++;
    } while (--i);
}

/* effects of parts 1 and 4 */
void Em36_wound_fx(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 1 && Rnd() & 1) {
        Ofs.vx = -295;
        Ofs.vy = -65;
        Ofs.vz = 240;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
        Ofs.vx = 7;
        Ofs.vy = 380;
        Ofs.vz = 118;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[4].Workm;
        Esp_call(0x1700, (Rnd() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && Rnd() & 1) {
        Ofs.vx = -143;
        Ofs.vy = 340;
        Ofs.vz = -2;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[4].Workm;
        Esp_call(0x1700, (Rnd() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
        Ofs.vx = -268;
        Ofs.vy = -510;
        Ofs.vz = -300;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
}

/* effects of part 1 */
void Em36_wound_fx2(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 2 && Rnd() & 1) {
        Ofs.vy = -450;
        Ofs.vx = 0;
        Ofs.vz = 400;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd() + 0x1000) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
}

/* effects of part 1 */
void Em36_wound_fx3(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 5 && Rnd() & 1) {
        Ofs.vx = 20;
        Ofs.vy = -700;
        Ofs.vz = 0;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd() + 0x1400) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
}
