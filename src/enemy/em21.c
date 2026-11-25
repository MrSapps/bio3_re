#include "enemy/em21.h"
#include "joint.h"
#include "root_ck.h"
#include "sca.h"
#include "snd.h"

/* Capcom's EM21.C called these without a prototype in scope: int results, and Add_speedXZ with a
 * third argument. */
LONG Rnd_l(void) __asm__("Rnd");
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");
void Add_speedXZ_n(ENEMY_WORK *pEm, LONG muki, LONG Dummy) __asm__("Add_speedXZ");
LONG Sca_get_dir_l(SVECTOR *pPos, struct tagSCA_DATA *pT) __asm__("func_800502B8"); /* RE2 Sca_get_dir */

/* RE2 EM_MAIN (Em121/Em221): per-frame entry of the crow: routine dispatch, collision, shadow, sounds */
void Em21_main(ENEMY_WORK *pEm)
{
    VECTOR Vec0, Vec1;

    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    pEm->nFloor = pEm->Pos_y / -1800;
    if (!(EF(pEm)->Flg & 0x100))
        pEm->Ground = Sca_get_ground(&pEm->Pos_x, 350, 0x200, 1);
    Vec0.vx = pEm->Pos_x;
    Vec0.vy = pEm->Pos_y;
    Vec0.vz = pEm->Pos_z;
    Vec1.vx = G.Player.Pos_x;
    Vec1.vy = G.Player.Pos_y;
    Vec1.vz = G.Player.Pos_z;
    if (pEm->Status_flg & 0x2000)
        EF(pEm)->Flg |= 2;
    else
        EF(pEm)->Flg &= ~2;
    if ((LONG)pEm->L_pl < 1100)
        pEm->Col.Unk26 = 180;
    else
        pEm->Col.Unk26 = 530;
    Em21_attacker_off();
    Em21_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Em21_kage_set(pEm);
    Em21_lock_off_ck(pEm);
    EF(pEm)->Old_x = pEm->Pos_x;
    EF(pEm)->Old_y = pEm->Pos_y;
    EF(pEm)->Old_z = pEm->Pos_z;
    if (G.pEnemy[3] == pEm) {
        Print_color(3, 1, 0);
        Print8(50, 50, "%04x, %d", pEm->Col.Hit_flg, EF(pEm)->Sca_cnt);
    }
}

/* RE2 Em21_kage_set: shadow size and colour by the height above the floor */
void Em21_kage_set(ENEMY_WORK *pEm)
{
    KAGE_WORK *pK = pEm->Kage.pKw;
    POLY_FT4 *pPrim = &pK->Prim[G.Cbuf_id];
    LONG Dy;
    LONG Half;

    if (pEm->Routine_0 != 7) {
        Dy = pEm->Ground - pEm->Pos_y - 250;
        if (Dy > 0) {
            Half = 0x57E40 / ((Dy + 0x708) >> 1);
            *(ULONG *)&pK->Half_x = Half | (Half << 16);
            if (Dy > 0x1900)
                *(ULONG *)&pPrim->r0 = 0x2E000000;
            else
                *(ULONG *)&pPrim->r0 = 0x2E808080 - (Dy / 100) * 0x20202;
        } else {
            *(ULONG *)&pK->Half_x = 0x1900190;
        }
    }
}

/* RE2 Em21_Init: life, collision, flags and the start routine of the Type */
void Em21_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Unk144 = 0;
    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Life = 5;
    pEm->Unk1CC[0] = 0;
    pEm->Unk1CC[1] = 0;
    pEm->Unk1CC[2] = 0;
    pEm->Unk1CC[3] = 0;
    pEm->Unk1CC[4] = 0;
    pEm->Unk1CC[5] = 0;
    pEm->Unk1CC[6] = 0;
    pEm->Unk1CC[7] = 0;
    pEm->Add_dir = 0;
    pEm->Unk14A = 0;
    pEm->Unk14C = 0;
    pEm->Unk14E = 0;
    EM_SET_MOVE(pEm, 0, 0, 0);
    pEm->Be_flg |= 0x400;
    Joint_move(pEm, pKan, pSeq, 0x200);
    pP = &pEm->pSin_parts_ptr[2];
    pEm->Neck_flg = 0;
    pEm->Neck_no = 8;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    PW_NECK(pP)->Add_dir_y = 0x60;
    PW_NECK(pP)->Add_dir_z = 0x60;
    PW_NECK(pP)->L_rot_y = 0x200;
    PW_NECK(pP)->L_rot_z = 0x138;
    pEm->Hit_area.Top = 400;
    pEm->Hit_area.Bottom = -400;
    pEm->Hit_area.R = 400;
    pEm->Hit_area.Rate0 = 0x54;
    pEm->Hit_area.Rate1 = 0xA8;
    pEm->Col.Unk26 = 0x212;
    pEm->Col.H = 0x424;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pEm->Unk140 = 0;
    pEm->Col.pEm = pEm;
    pEm->Col.Unk24 = 0;
    pEm->Col.Unk28 = 0;
    pEm->Col.Flg = 0;
    pEm->Col.Type = 2;
    Em21_atd_set(pEm, 350);
    Oba_set(&pEm->Col, 0);
    Kage_work_set(&pEm->Kage.pKw, 0, 0xC800C8, 0, &pEm->Pos_x);
    if (pEm->Type & 0x4000) {
        pEm->Routine_0 = 4;
        pEm->Type &= ~0x4000;
        EF(pEm)->Flg |= 1;
        pEm->Routine_1 = (UCHAR)pEm->Type & 0xF;
        Em21_routine_0[pEm->Routine_0](pEm, pKan, pSeq);
    }
    if (pEm->Type & 0x40) {
        pEm->Damage_cnt = 0x80;
        pEm->Be_flg |= 0x2000;
    }
    G.pPldamage_Em[0x21 - 0x10] = (void (*)())Pldamege_Em21;
}

/* RE2 Em21_Move: br then mv routine by Routine_1 (indoor or outdoor tables) */
void Em21_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Flg;

    Flg = 0;
    if (EF(pEm)->Idly_cnt == 0) {
        if (!(Rnd_l() & 0x1F)) {
            EF(pEm)->Idly_cnt = Rnd_l() & 0x7F;
            EF(pEm)->Idly_flg = Rnd_area();
        }
    }
    if (G.Player.Life < 0) {
        if (EF(pEm)->Pl_dead_cnt) {
            if (EF(pEm)->Idly_cnt)
                Flg = 1;
            EF(pEm)->Pl_dead_cnt--;
        }
    } else if (!(EF(pEm)->Flg & 0x40)) {
        if (EF(pEm)->Idly_cnt)
            Flg = 1;
    }
    if (Flg) {
        Root_ck(pEm, POS_V(&G.Player), EF(pEm)->Idly_flg, 1);
        if (EF(pEm)->Idly_cnt-- == 0)
            EF(pEm)->Idly_flg = 0;
    } else {
        Root_ck(pEm, POS_V(&G.Player), 0, 0);
    }
    if (pEm->Type & 0x80) {
        Em21_outdoor_br[pEm->Routine_1](pEm);
        Em21_outdoor_mv[pEm->Routine_1](pEm, pKan, pSeq);
    } else {
        Em21_indoor_br[pEm->Routine_1](pEm);
        Em21_indoor_mv[pEm->Routine_1](pEm, pKan, pSeq);
    }
    if (EF(pEm)->Atck) {
        Em21_hit_area.pPos = POS_V(pEm);
        Em21_hit_area.pBox = Em21_peck_box0;
        Em21_hit_area.Dir = pEm->Cdir_y;
        Em21_hit_area.Id = 1;
        Hit_area_em_ck(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em21_hit_area, &Pl_work);
        Em21_hit_area.pBox = Em21_peck_box1;
        Em21_hit_area.Id = 2;
        Hit_area_em_ck(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em21_hit_area, &Pl_work);
    }
}

/* RE2 Em21_br_wait_i: perched: take off when the player comes near or the alarm is up */
void Em21_br_wait_i(ENEMY_WORK *pEm)
{
    LONG D0;

    D0 = 0;
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0 && (LONG)pEm->L_pl < 1800)
        return;
    if ((LONG)pEm->L_pl <= 1800)
        D0 = 1;
    if ((G.Unk75E8 & 4) && (LONG)pEm->L_pl <= 5400)
        D0 = 1;
    if (G.Unk75E8 & 1)
        D0 = 1;
    if (EF(pEm)->Flg & 1) {
        if (pEm->Timer0-- == 0)
            D0 = 1;
    }
    if (D0)
        Em21_r1_set(pEm, 1);
}

/* RE2 Em21_mv_wait_i: perched / walking on the ground, pecking about */
void Em21_mv_wait_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EF(pEm)->Cnt = Rnd_l() % 100;
        pEm->Routine_2 = 7;
        break;
    case 1:
        EM_SET_MOVE(pEm, 0, Rnd_l() % 5, 7);
        EF(pEm)->Cnt = 5;
        pEm->Routine_2 = 10;
        break;
    case 2:
        EM_SET_MOVE(pEm, 1, 0, 7);
        pEm->Routine_2 = 8;
        break;
    case 3:
        EM_SET_MOVE(pEm, 1, 0, 7);
        pEm->Routine_2 = 9;
        break;
    case 4:
        EM_SET_MOVE(pEm, 1, 0, 7);
        pEm->Routine_2 = 11;
        break;
    case 5:
        EM_SET_MOVE(pEm, 2, Rnd_l() % 7, 7);
        pEm->Routine_2 = 8;
        break;
    case 6:
        EM_SET_MOVE(pEm, 0xA, 0, 7);
        EF(pEm)->Cnt = Rnd_l() % 5 + 15;
        EF(pEm)->Tmp_0 = pEm->Pos_x;
        EF(pEm)->Tmp_1 = pEm->Pos_z;
        pEm->Routine_2 = 13;
        break;
    case 7:
        if (EF(pEm)->Cnt == 0) {
            EF(pEm)->Tmp = Rnd_l() % 7;
            if (EF(pEm)->Tmp == 6) {
                if (!(Rnd_l() & 0xF))
                    pEm->Routine_2 = 6;
                else
                    pEm->Routine_2 = 0;
            } else {
                pEm->Routine_2 = EF(pEm)->Tmp;
            }
        }
        EF(pEm)->Cnt--;
        break;
    case 9:
        if (Joint_move(pEm, pKan, pSeq, 0x10200))
            pEm->Routine_2 = 0;
        break;
    case 10:
        if (EF(pEm)->Cnt-- == 0) {
            if (!(EF(pEm)->Flg & 0x20) && (Rnd_l() & 1))
                Snd_se_enem(6, pEm);
        }
    case 8:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 0;
        break;
    case 11:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 12;
            EF(pEm)->Cnt = Rnd_l() % 100;
        }
        break;
    case 12:
        if (EF(pEm)->Cnt == 0)
            pEm->Routine_2 = 3;
        EF(pEm)->Cnt--;
        break;
    case 13:
        Joint_move(pEm, pKan, pSeq, 0x10200);
        Em21_foot_set(pEm);
        if (pEm->Move_cnt == EF(pEm)->Cnt) {
            pEm->Routine_2 = 14;
            EM_SET_MOVE(pEm, 0xA, 0x27 - EF(pEm)->Cnt, 7);
        }
        break;
    case 14:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 0;
            pEm->Pos_x = EF(pEm)->Tmp_0;
            pEm->Pos_z = EF(pEm)->Tmp_1;
        } else {
            Em21_foot_set(pEm);
        }
    }
}

/* RE2 Em21_br_go_wait_i: empty */
void Em21_br_go_wait_i(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_go_wait_i: a short random wait, then take off (or glide off, Type 0x40) */
void Em21_mv_go_wait_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EF(pEm)->Cnt = Rnd_l() & 7;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            if (pEm->Type & 0x40)
                Em21_r1_set(pEm, 3);
            else
                Em21_r1_set(pEm, 2);
        }
    }
}

/* RE2 Em21_br_take_off_i: empty */
void Em21_br_take_off_i(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_take_off_i: taking off */
void Em21_mv_take_off_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        D0 = Rnd_l() & 7;
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 7))
            Em_21_feather_call(pEm, 1);
        EM_SET_MOVE(pEm, 7, D0, 7);
        EF(pEm)->Sca_cnt = 0;
        pEm->Spd_x = (Rnd_l() & 0x1F) + 100;
        pEm->Spd_y = -10;
        EF(pEm)->Flg |= 1;
        EF(pEm)->Cnt = 9 - D0;
        EF(pEm)->Tmp = 1;
        pEm->Routine_2 = 5;
        goto move;
    case 1:
        pEm->Spd_y = -80;
        EF(pEm)->Cnt = 6;
        EF(pEm)->Tmp = 2;
        pEm->Routine_2 = 5;
        goto move;
    case 2:
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        pEm->Spd_y = -60;
        EF(pEm)->Cnt = 4;
        EF(pEm)->Tmp = 3;
        pEm->Routine_2 = 5;
        goto move;
    case 3:
        pEm->Spd_y = -100;
        EF(pEm)->Cnt = 9;
        EF(pEm)->Tmp = 4;
        pEm->Routine_2 = 5;
        goto move;
    case 4:
        if (!(Rnd_l() & 7))
            Em_21_feather_call(pEm, 1);
        pEm->Spd_y = -80;
        EF(pEm)->Cnt = 0xFF;
        pEm->Routine_2 = 5;
    case 5:
    move:
        EF(pEm)->Cnt--;
        if (EF(pEm)->Cnt == 0)
            pEm->Routine_2 = EF(pEm)->Tmp;
        if (Rnd_l() & 1)
            Goto00(pEm, -pEm->Pos_x + G.Player.Pos_x, -pEm->Pos_z + G.Player.Pos_z, 0x20);
        else
            Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x++;
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            Em21_r1_set(pEm, 4);
    }
}

/* RE2 Em21_br_glide_i: empty */
void Em21_br_glide_i(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_glide_i: gliding */
void Em21_mv_glide_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Type &= ~0x40;
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        D0 = Rnd_l() & 7;
        EM_SET_MOVE(pEm, 6, D0, 7);
        EF(pEm)->Sca_cnt = 0;
        pEm->Spd_x = (Rnd_l() & 7) + 60;
        pEm->Spd_y = 0;
        EF(pEm)->Flg |= 1;
        EF(pEm)->Cnt = 12;
        EF(pEm)->Cnt2 = 0;
        EF(pEm)->Tmp = 1;
        pEm->Routine_2 = 2;
        goto move;
    case 1:
        pEm->Be_flg &= ~0x2000;
        pEm->Damage_cnt &= 0x7F;
        pEm->Spd_x = (Rnd_l() & 7) + 80;
        pEm->Spd_y = 80;
        EF(pEm)->Cnt = 0xFF;
        EF(pEm)->Cnt2 = 0x20;
        pEm->Routine_2 = 2;
    case 2:
    move:
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, EF(pEm)->Cnt2);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x++;
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 3;
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = EF(pEm)->Tmp;
        break;
    case 3:
        Em21_r1_set(pEm, 4);
    }
}

/* RE2 Em21_br_flight_i: flying: climb or descend towards the player's height, attack checks */
void Em21_br_flight_i(ENEMY_WORK *pEm)
{
    if (pEm->Type & 0x80) {
        if ((LONG)pEm->L_pl > 10800) {
            if (Rnd_l() & 0xF)
                Em21_r1_set(pEm, 8);
            else
                Em21_r1_set(pEm, 7);
        }
    } else if ((LONG)pEm->L_pl > 7200) {
        if (Rnd_l() & 3)
            Em21_r1_set(pEm, 8);
        else
            Em21_r1_set(pEm, 7);
    }
    if (G.Player.Pos_y < pEm->Pos_y) {
        pEm->Timer0 = Rnd_l() & 0x3F;
        EF(pEm)->Tmp_0 = -120;
        Em21_r1_set(pEm, 5);
    } else if (pEm->Pos_y - pEm->Ground > -1800) {
        pEm->Timer0 = Rnd_l() & 0x3F;
        EF(pEm)->Tmp_0 = -120;
        Em21_r1_set(pEm, 5);
    } else if (!(Rnd_l() & 0xF)) {
        pEm->Timer0 = Rnd_l() & 0x3F;
        if (pEm->Ground - 3600 < pEm->Pos_y) {
            EF(pEm)->Tmp_0 = -40;
            Em21_r1_set(pEm, 5);
        } else {
            Em21_r1_set(pEm, 6);
        }
    }
    Em21_sca_avoid_ck(pEm);
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
        Em21_pl_dead_ck(pEm);
    } else {
        Em21_run_away_ck(pEm);
        Em21_attack_ck(pEm);
    }
}

/* RE2 Em21_mv_flight_i: flapping flight towards the target */
void Em21_mv_flight_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        if (Rnd_l() & 1) {
            EM_SET_MOVE(pEm, 3, 0, 7);
            EF(pEm)->Tmp = 2;
        } else {
            EM_SET_MOVE(pEm, 4, 0, 7);
            if (!(Rnd_l() & 7))
                Em_21_feather_call(pEm, 1);
            EF(pEm)->Tmp = 3;
        }
        EF(pEm)->Cnt = Rnd_l() & 0x1F;
        pEm->Spd_y = 0;
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x += EF(pEm)->Tmp;
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 0;
    }
}

/* RE2 Em21_br_up_i: climbing until high enough, then fly on; obstacle and attack checks */
void Em21_br_up_i(ENEMY_WORK *pEm)
{
    if (pEm->Type & 0x80) {
        if (pEm->Pos_y < pEm->Ground - 6000)
            Em21_r1_set(pEm, 4);
    } else if (pEm->Pos_y < pEm->Ground - 4800) {
        Em21_r1_set(pEm, 4);
    }
    if (pEm->Timer0-- == 0)
        Em21_r1_set(pEm, 4);
    Em21_sca_avoid_ck(pEm);
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
        Em21_pl_dead_ck(pEm);
    } else {
        Em21_run_away_ck(pEm);
        Em21_attack_ck(pEm);
    }
}

/* RE2 Em21_mv_up_i: climbing flight */
void Em21_mv_up_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
        pEm->Spd_y = EF(pEm)->Tmp_0;
    case 1:
        if (EF(pEm)->Cnt-- == 0)
            EF(pEm)->Cnt = 9;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x += 2;
        Joint_move(pEm, pKan, pSeq, 0x200);
    }
}

/* RE2 Em21_br_down_i: descending until low enough, then fly on; obstacle and attack checks */
void Em21_br_down_i(ENEMY_WORK *pEm)
{
    if (pEm->Ground - 1800 < pEm->Pos_y)
        Em21_r1_set(pEm, 4);
    if (pEm->Timer0-- == 0)
        Em21_r1_set(pEm, 4);
    Em21_sca_avoid_ck(pEm);
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
        Em21_pl_dead_ck(pEm);
    } else {
        Em21_run_away_ck(pEm);
        Em21_attack_ck(pEm);
    }
}

/* RE2 Em21_mv_down_i: descending flight */
void Em21_mv_down_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 7);
        pEm->Spd_y = 40;
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0)
            EF(pEm)->Cnt = 15;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x++;
        Joint_move(pEm, pKan, pSeq, 0x200);
    }
}

/* RE2 Em21_br_landing_i: empty */
void Em21_br_landing_i(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_landing_i: landing on the floor */
void Em21_mv_landing_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;
    SVECTOR Svec;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 7);
        EF(pEm)->Sca_cnt = 0;
        EF(pEm)->Cnt = 0;
        pEm->Spd_x = 50;
        pEm->Spd_y = 120;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            EF(pEm)->Cnt = 15;
        }
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        Add_speedXYZ(pEm, 0, 0);
        D0 = pEm->Ground - 250;
        if (pEm->Pos_y - D0 > -300)
            pEm->Routine_2 = 2;
        Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_SET_MOVE(pEm, 0xA, 0, 7);
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        pEm->Spd_x = 50;
        pEm->Spd_y = 15;
        EF(pEm)->Cnt = 15;
        EF(pEm)->Tmp = 3;
        pEm->Routine_2 = 5;
        goto move;
    case 3:
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        EF(pEm)->Cnt = 5;
        EF(pEm)->Tmp = 4;
        pEm->Routine_2 = 5;
        EF(pEm)->Flg |= 0x100;
        goto move;
    case 4:
        pEm->Spd_x = 10;
        pEm->Spd_y = 0;
        pEm->Pos_y = pEm->Ground - 250;
        EF(pEm)->Cnt = 0xFF;
        pEm->Routine_2 = 5;
    case 5:
    move:
        EF(pEm)->Cnt--;
        if (EF(pEm)->Cnt == 0)
            pEm->Routine_2 = EF(pEm)->Tmp;
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        Add_speedXYZ(pEm, 0, 0);
        D0 = pEm->Ground - 250;
        if (D0 < pEm->Pos_y)
            pEm->Pos_y = D0;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Spd_x = 10;
            pEm->Routine_2 = 6;
            EM_SET_MOVE(pEm, 8, 0, 7);
        }
        break;
    case 6:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_y = pEm->Ground - 250;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EF(pEm)->Flg &= ~0x100;
            if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
                Em21_r1_set(pEm, 0);
            } else {
                pEm->Timer0 = Rnd_l() << 1;
                Em21_r1_set(pEm, 2);
            }
        }
    }
}

/* RE2 Em21_br_quick_turn_i: turning: obstacle check */
void Em21_br_quick_turn_i(ENEMY_WORK *pEm)
{
    Em21_run_away_ck(pEm);
    Em21_sca_avoid_ck(pEm);
}

/* RE2 Em21_mv_quick_turn_i: quick turn in the air */
void Em21_mv_quick_turn_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        if (!(Rnd_l() & 7))
            Em_21_feather_call(pEm, 1);
        EM_SET_MOVE(pEm, 4, 0, 7);
        EF(pEm)->Cnt = Rnd_l() & 0x3F;
        EF(pEm)->Cnt2 = 0;
        EF(pEm)->Sca_cnt = 0;
        pEm->Spd_y = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt2-- == 0)
            EF(pEm)->Cnt2 = 14;
        if ((LONG)pEm->L_pl < 900 && G.Player.Pos_y - 3700 < pEm->Pos_y)
            pEm->Pos_y -= 50;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);
        pEm->Cdir_y += (signed char)EF(pEm)->Tmp;
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x80) || EF(pEm)->Cnt-- == 0)
            Em21_r1_set(pEm, 4);
    }
}

/* RE2 Em21_br_jump_i: empty */
void Em21_br_jump_i(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_jump_i: hopping on the ground */
void Em21_mv_jump_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 8, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Timer0 = Rnd_l() & 0xF;
            Em21_r1_set(pEm, 0);
        }
    }
}

/* RE2 Em21_br_fall_i: empty */
void Em21_br_fall_i(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_fall_i: falling to the floor */
void Em21_mv_fall_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_enem(3, pEm);
        EM_SET_MOVE(pEm, 9, 0, 7);
        pEm->Spd_y = 0;
        pEm->Spd_x = 0;
        EF(pEm)->Tmp_0 = 0;
        EF(pEm)->Sca_cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        EF(pEm)->Tmp_0 += 20;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        D0 = pEm->Ground - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            EM_SET_MOVE(pEm, 0xB, 0, 7);
            EF(pEm)->Cnt = Rnd_l() & 0xF;
            pEm->Cdir_z = 0;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 4;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt--;
        break;
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            Em21_r0_set(pEm, 1, 9);
    }
}

/* RE2 Em21_br_crash_f_i: diving at the player: break off when he turns away */
void Em21_br_crash_f_i(ENEMY_WORK *pEm)
{
    if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80) || !(EF(pEm)->Flg & 2))
        Em21_r1_set(pEm, 4);
    Em21_sca_avoid_ck(pEm);
    Em21_run_away_ck(pEm);
}

/* RE2 Em21_mv_crash_f_i: diving at the player's height */
void Em21_mv_crash_f_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        EF(pEm)->Tmp_0 = G.Player.Ground - 2800;
        D0 = EF(pEm)->Tmp_0 - pEm->Pos_y;
        if (D0 < 0)
            EF(pEm)->Tmp = 0;
        else
            EF(pEm)->Tmp = 1;
        pEm->Spd_y = 0;
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            Snd_se_enem(1, pEm);
            EF(pEm)->Cnt2 = 14;
        }
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
        if (pEm->Spd_x < 300)
            pEm->Spd_x += 5;
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = G.Player.Ground - 2800;
        if (EF(pEm)->Tmp == 0) {
            if (D0 < pEm->Pos_y)
                pEm->Pos_y -= 40;
        } else if (pEm->Pos_y < D0) {
            pEm->Pos_y += 40;
        }
        if ((LONG)pEm->L_pl < 900) {
            D0 = D0 - pEm->Pos_y;
            if (D0 > -360 && D0 < 360 && pEm->Spd_x > 230) {
                Em21_r1_set(pEm, 12);
            } else {
                EF(pEm)->Idly_cnt = Rnd_l() & 0x7F;
                EF(pEm)->Idly_flg = Rnd_area();
                Em21_r1_set(pEm, 8);
            }
        } else {
            Add_speedXYZ(pEm, 0, 0);
        }
    }
}

/* RE2 Em21_br_crash_i: run-away check only */
void Em21_br_crash_i(ENEMY_WORK *pEm)
{
    Em21_run_away_ck(pEm);
}

/* RE2 Em21_mv_crash_i: lying stunned on the floor after a crash, then hopping up */
void Em21_mv_crash_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        if (Cdir_ck_l(pEm, &Pl_work))
            Em21_r0_set(&Pl_work, 2, 1);
        else
            Em21_r0_set(&Pl_work, 2, 0);
        Pad_vib_set0(4, 0);
        Pad_vib_set1(10, 150, 0);
        Snd_se_enem(4, pEm);
        EM_SET_MOVE(pEm, 9, 0, 7);
        pEm->Spd_y = 0;
        pEm->Spd_x = 0;
        EF(pEm)->Obj_cnt = 0;
        EF(pEm)->Tmp_0 = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        EF(pEm)->Tmp_0 += 20;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        D0 = pEm->Ground - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            EM_SET_MOVE(pEm, 0xB, 0, 7);
            EF(pEm)->Cnt = Rnd_l() & 0xF;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 4;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt--;
        break;
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400)) {
                Em21_r1_set(pEm, 9);
            } else {
                pEm->Timer0 = Rnd_l() & 0xF;
                Em21_r1_set(pEm, 0);
            }
        }
    }
}

/* RE2 Em21_br_peck_f_i: hovering at the player: break off when he turns away */
void Em21_br_peck_f_i(ENEMY_WORK *pEm)
{
    if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100) || !(EF(pEm)->Flg & 2)) {
        Em21_atd_set(pEm, 350);
        Em21_r1_set(pEm, 4);
    }
    Em21_sca_avoid_ck(pEm);
    Em21_run_away_ck(pEm);
}

/* RE2 Em21_mv_peck_f_i: hovering and pecking at the player's head */
void Em21_mv_peck_f_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D1;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Cdir_x = 0;
        pEm->Cdir_z = 0;
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            Snd_se_enem(1, pEm);
            EF(pEm)->Cnt2 = 14;
        }
        if (EF(pEm)->Flg & 4) {
            Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
            if (pEm->Spd_x < 300)
                pEm->Spd_x += 4;
            if ((LONG)pEm->L_pl < 2300) {
                if (EF(pEm)->Atck == 0) {
                    if (EF(pEm)->Atck_se == 0)
                        Snd_se_enem(6, pEm);
                    EF(pEm)->Atck_se = 1;
                    EF(pEm)->Atck = 1;
                }
            }
            if ((LONG)pEm->L_pl >= 2300) {
                if (EF(pEm)->Atck == 1)
                    EF(pEm)->Atck = 0;
            }
        } else {
            Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200);
            if (pEm->Spd_x < 300)
                pEm->Spd_x += 12;
        }
        Joint_move(pEm, pKan, pSeq, 0x200);
        EF(pEm)->Tmp_0 = G.Player.Ground - 2800;
        D1 = EF(pEm)->Tmp_0 - pEm->Pos_y;
        if (D1 < -60 || D1 > 60) {
            if (D1 < -60)
                pEm->Pos_y -= 60;
            else if (D1 > 60)
                pEm->Pos_y += 60;
        }
        if ((LONG)pEm->L_pl < 900)
            Em21_atd_set(pEm, 100);
        if ((LONG)pEm->L_pl < 650) {
            if (D1 > -100 && D1 < 100)
                Em21_r1_set(pEm, 14);
        } else {
            Add_speedXYZ(pEm, 0, 0);
        }
    }
}

/* RE2 Em21_br_peck_i: run-away check only */
void Em21_br_peck_i(ENEMY_WORK *pEm)
{
    Em21_run_away_ck(pEm);
}

/* RE2 Em21_mv_peck_i: pecking at the player on the ground (or the dead player) */
void Em21_mv_peck_i(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;
    SVECTOR Svec;

    if (G.Player.Life <= 0)
        EF(pEm)->Tmp = 0;
    else
        EF(pEm)->Tmp = 1;
    switch (pEm->Routine_2) {
    case 0:
        if (G.Player.Life < 0)
            goto end;
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Cdir_x = 0;
        pEm->Cdir_z = 0;
        if (EF(pEm)->Flg & 4) {
            EF(pEm)->Atck = 0;
            EF(pEm)->Atck_se = 0;
            Em21_set_flg_all(8);
            Em21_set_flg_all(0x40);
            Em21_r0_set(G_PLAYER, 5, 0);
            G.Player.pEnemy_ptr = pEm;
            G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
            G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
            G.Player.Be_flg |= 0x8800;
        }
        pEm->Pos_y = EF(pEm)->Tmp_0;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        EM_SET_MOVE(pEm, 5, 0, 7);
        EF(pEm)->Cnt = 100;
        EF(pEm)->Cnt2 = 0;
        pEm->Spd_x = 300;
        Em21_atd_set(pEm, 350);
        pEm->Routine_2 = 1;
        pEm->Be_flg |= 0x800;
    case 1:
        if (EF(pEm)->Flg & 4) {
            Pad_vib_set0(6, 0);
            Pad_vib_set1(4, 150, 1);
            Svec.vx = pEm->Pos_x;
            Svec.vy = pEm->Pos_y + 500;
            Svec.vz = pEm->Pos_z;
            Esp_call(0x100, (USHORT)pEm->Cdir_y | 0x10000000, &GsIDMATRIX, &Svec, NULL);
            if (!(Rnd_l() & 3))
                Em_21_feather_call(pEm, 1);
            Snd_se_enem(2, pEm);
            D0 = Pl_life_down(5, EF(pEm)->Tmp);
            if (D0 == 1)
                goto end;
            if (D0 == 2) {
                Em21_set_flg_all(0x80);
                goto end;
            }
        } else {
            if (!(Rnd_l() & 7))
                Em_21_feather_call(pEm, 1);
        }
        EF(pEm)->Cnt3 = 4;
        pEm->Routine_2 = 2;
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = Gacha_ch() * 3 + 1;
        EF(pEm)->Cnt -= D0;
        if ((signed char)EF(pEm)->Cnt < 0)
            goto end;
        if (EF(pEm)->Cnt3-- == 0) {
            EF(pEm)->Tmp_0 = 150;
            EF(pEm)->Cnt3 = EF(pEm)->Cnt2 = (Rnd_l() & 7) + 2;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = Gacha_ch() * 3 + 1;
        EF(pEm)->Cnt -= D0;
        if ((signed char)EF(pEm)->Cnt < 0)
            goto end;
        pEm->Pos_y -= EF(pEm)->Tmp_0;
        EF(pEm)->Tmp_0 -= 15;
        if (EF(pEm)->Cnt2-- == 0)
            pEm->Routine_2 = 4;
        break;
    case 4:
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = Gacha_ch() * 3 + 1;
        EF(pEm)->Cnt -= D0;
        if ((signed char)EF(pEm)->Cnt < 0)
            goto end;
        EF(pEm)->Tmp_0 += 15;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        if (EF(pEm)->Cnt3-- == 0) {
            EM_SET_MOVE(pEm, 5, 0, 7);
            pEm->Routine_2 = 1;
            EF(pEm)->Cnt2 = 7;
        }
        break;
    case 5:
    end:
        EF(pEm)->Idly_cnt = (Rnd_l() & 0x7F) + 30;
        EF(pEm)->Idly_flg = Rnd_area();
        EF(pEm)->Na_cnt = 10;
        pEm->Be_flg &= ~0x800;
        Em21_r1_set(pEm, 4);
        if (EF(pEm)->Flg & 4) {
            Em21_set_flg_all(0x10);
            G.Player.Routine_2 = 3;
            G.Player.Be_flg &= ~0x8800;
        }
    }
}

/* RE2 Em21_Damage: hit (Em21_dm by damage kind); also used as the Die routine */
void Em21_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Root_ck(pEm, POS_V(&G.Player), 0, 0);
    Em21_dm[pEm->Damage_no - 1](pEm, pKan, pSeq);
}

/* RE2 Em21_dm00: hit: knocked away from the player, falls and dies */
void Em21_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_enem(3, pEm);
        EM_SET_MOVE(pEm, 9, 0, 7);
        pEm->Spd_y = 0;
        pEm->Spd_x = 0;
        EF(pEm)->Sca_cnt = 0;
        EF(pEm)->Tmp_0 = 0;
        EF(pEm)->Flg |= 0x100;
        D0 = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);
        EF(pEm)->Tmp_1 = D0 - pEm->Cdir_y;
        G.pEm->Spd_x = 100;
        if (Rnd_l() & 1)
            EF(pEm)->Tmp_2 = 200;
        else
            EF(pEm)->Tmp_2 = -200;
        Em_21_blood_call(pEm);
        Em_21_feather_call(pEm, (Rnd_l() & 1) | 2);
        EF(pEm)->Cnt = 3;
        EF(pEm)->Cnt2 = 3;
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        if (EF(pEm)->Cnt-- == 0)
            Em_21_feather_call(pEm, 1);
        if (EF(pEm)->Cnt2-- == 0)
            Em_21_feather_call(pEm, 1);
        EF(pEm)->Tmp_0 += 20;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        Add_speedXZ(G.pEm, EF(pEm)->Tmp_1);
        if (EF(pEm)->Tmp_2 < 0) {
            if (pEm->Cdir_z > -2048)
                pEm->Cdir_z += EF(pEm)->Tmp_2;
        } else {
            if (pEm->Cdir_z < 2048)
                pEm->Cdir_z += EF(pEm)->Tmp_2;
        }
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        D0 = pEm->Ground - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            EM_SET_MOVE(pEm, 0xB, 0, 7);
            EF(pEm)->Cnt = Rnd_l() & 0xF;
            pEm->Cdir_z = 0;
            if (pEm->Life < 0) {
                EF(pEm)->Cnt = 11;
                Em_dead_flg_on_cand(pEm);
                pEm->Routine_2 = 5;
            } else {
                pEm->Routine_2 = 3;
            }
        }
        break;
    case 3:
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 4;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt--;
        break;
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400)) {
                Em21_r0_set(pEm, 1, 9);
            } else {
                pEm->Timer0 = Rnd_l() & 0xF;
                Em21_r0_set(pEm, 1, 0);
            }
        }
        break;
    case 5:
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0)
            Em21_r0_set(pEm, 7, 0);
    }
}

/* RE2 Em21_dm01: blown apart: the parts fly off or drop */
void Em21_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */
    LONG D0;
    ULONG Rnd0;

    pEm->Life = -1;
    pEm->Spd_y = 0;
    D0 = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);
    Rnd0 = Rnd_l();
    Em21_parts_bobm_set(pEm->pSin_parts_ptr, D0);
    if (Rnd0 & 1) {
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 1, D0);
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 2, D0);
    } else {
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 1, D0);
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 2, D0);
    }
    if (Rnd0 & 2) {
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 3, D0);
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 4, D0);
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 5, D0);
    } else {
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 3, D0);
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 4, D0);
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 5, D0);
    }
    if (Rnd0 & 4) {
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 6, D0);
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 7, D0);
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 8, D0);
    } else {
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 6, D0);
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 7, D0);
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 8, D0);
    }
    if (Rnd0 & 8)
        Em21_parts_bobm_set(pEm->pSin_parts_ptr + 9, D0);
    else
        Em21_parts_fall_set(pEm->pSin_parts_ptr + 9, D0);
    Em_21_blood_call(pEm);
    Em_21_blood_call(pEm);
    Em_21_blood_call(pEm);
    Em_21_feather_call(pEm, (Rnd_l() & 3) | 8);
    Em_dead_flg_on_cand(pEm);
    Em21_r0_set(pEm, 7, 1);
}

/* RE2 Em21_parts_bobm_set: a part flies off (PARTS_W bomb work), direction B_y */
void Em21_parts_bobm_set(PARTS_W *pP, LONG B_y)
{
    pP->UnkA2 |= 0x20;
    pP->Be_flg |= 2;
    PW_BOMB(pP)->V0_x = B_y;
    PW_BOMB(pP)->Cnt0 = 0;
    PW_BOMB(pP)->V0_z = 800;
    PW_BOMB(pP)->V0_y = 0;
    PW_BOMB(pP)->G = 10;
    PW_BOMB(pP)->Gxz = -100;
    pP->Poly_rgb = 0x101040;
}

/* RE2 Em21_parts_fall_set: a part drops to the floor, direction D_y */
void Em21_parts_fall_set(PARTS_W *pP, LONG D_y)
{
    pP->Be_flg |= 0xA;
    pP->Old_x = D_y + (Rnd_l() << 2);
    pP->Timer3 = 0;
    pP->Old_z = (Rnd_l() & 0x7F) + 100;
    pP->Old_y = 0;
    pP->Timer1 = 20;
    pP->Rot_spd_x = 0;
    pP->Rot_spd_y = Rnd_l() << 4;
    pP->Rot_spd_z = 0;
}

/* RE2 Em21_dm03 (RE3 slot dm02): knocked away along the player's facing */
void Em21_dm03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        EF(pEm)->Flg &= ~0x100;
        Em_dead_flg_on_cand(pEm);
        Snd_se_enem(3, pEm);
        Em21_atd_set(pEm, 10);
        pEm->Life = -1;
        Em_21_blood_call(pEm);
        Em_21_feather_call(pEm, (Rnd_l() & 3) + 5);
        pEm->Cdir_y = G.Player.Cdir_y;
        pEm->Spd_x = 600;
        EM_SET_MOVE(pEm, 9, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        Add_speedXZ_n(pEm, 0, 0);
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += 20;
        D0 = pEm->Ground - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            pEm->Spd_y = 0;
            EF(pEm)->Cnt = 16;
            pEm->Routine_2 = 3;
        }
        if (pEm->Col.Hit_flg & 0x20) {
            pEm->Pos_x = EF(pEm)->Old_x;
            pEm->Pos_y = EF(pEm)->Old_y;
            pEm->Pos_z = EF(pEm)->Old_z;
            pEm->Ground = Sca_get_ground(&pEm->Pos_x, 350, 0x200, 1);
            Snd_se_enem(3, pEm);
            Em_21_blood_call(pEm);
            pEm->Spd_y = 0;
            pEm->Spd_x = 20;
            pEm->Cdir_y += 0x800;
            pEm->Routine_2 = 4;
        }
        break;
    case 3:
        Add_speedXZ_n(pEm, 0, 0);
        pEm->Spd_x >>= 1;
        if (EF(pEm)->Cnt-- == 0)
            Em21_r0_set(pEm, 7, 2);
        break;
    case 4:
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += 20;
        Add_speedXZ_n(pEm, 0, 0);
        D0 = pEm->Ground - 250;
        if (D0 < pEm->Pos_y) {
            Em21_atd_set(pEm, 350);
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            pEm->Spd_y = 0;
            Em21_r0_set(pEm, 7, 2);
        }
    }
}

/* RE2 Em21_Die: empty (the Die routine is Em21_Damage) */
void Em21_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* RE2 Em21_Scenario: scenario routines (Em21_sce_mv by Routine_1) */
void Em21_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Root_ck(pEm, POS_V(&G.Player), 0, 0);
    Em21_sce_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em21_sce_wait: perched, as Em21_mv_wait_i */
void Em21_sce_wait(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em21_mv_wait_i(pEm, pKan, pSeq);
    switch (pEm->Routine_3) {
    case 0:
        EF(pEm)->Flg |= 0x20;
        pEm->Routine_3++;
    case 1:
        if (pEm->Add_dir == 1) {
            pEm->Add_dir = 0;
            Em21_r0_set(pEm, 1, 1);
        }
        break;
    case 2:
        EF(pEm)->Flg &= ~0x20;
        pEm->Routine_3++;
    case 3:
        if (pEm->Add_dir == 1 || (G.Unk75E8 & 1)) {
            pEm->Add_dir = 0;
            Em21_r0_set(pEm, 1, 1);
        }
    }
}

/* RE2 Em21_sce_crash (reworked): flies into a window or wall and falls */
void Em21_sce_crash(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x2010;
        pEm->Type |= 0x4000;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Add_dir == 2) {
            pEm->Add_dir = 0;
            EM_SET_MOVE(pEm, 4, 0, 7);
            pEm->Spd_x = 300;
            pEm->Spd_y = 0;
            pEm->Be_flg &= ~0x10;
            pEm->Type &= ~0x4000;
            EF(pEm)->Cnt = 6;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x -= 10;
        if (EF(pEm)->Cnt-- == 0) {
            pEm->Be_flg &= ~0x2000;
            Em21_r0_set(pEm, 1, 4);
        }
    }
}

/* scenario: flies away along a fixed path */
void Em21_sce_fly_away(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x2000;
        pEm->Type |= 0x4000;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Add_dir == 3) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            pEm->Add_dir = 0;
            EM_SET_MOVE(pEm, 4, 0, 7);
            pEm->Spd_x = 150;
            pEm->Spd_y = -150;
            pEm->Type &= ~0x4000;
            EF(pEm)->Cnt = 50;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Goto00(pEm, -22375, 761, 0x18);
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 0x3F))
            Em_21_feather_call(pEm, 1);
        if (EF(pEm)->Cnt-- == 0) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            pEm->Spd_y = 0;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 0x3F))
            Em_21_feather_call(pEm, 1);
        Goto00(pEm, 11600, -10000, 0x20);
        Joint_move(pEm, pKan, pSeq, 0x200);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Unk14A == 1) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            pEm->Unk14A = 0;
            EM_SET_MOVE(pEm, 4, 0, 7);
            pEm->Spd_y = 200;
            pEm->Type &= ~0x4000;
            pEm->Be_flg &= ~0x2000;
            pEm->Damage_cnt &= 0x7F;
            pEm->Routine_2 = 4;
        }
        break;
    case 4:
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (pEm->Pos_y > -6000)
            Em21_r0_set(pEm, 1, 4);
    }
}

/* scenario: circles around its start position */
void Em21_sce_circle(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        pEm->Spd_x = 100;
        pEm->Be_flg |= 0x2000;
        pEm->Spd_y = 0;
        pEm->Routine_2 = 1;
        pEm->Type |= 0x5000;
        pEm->Dest_x = pEm->Pos_x;
        pEm->Dest_z = pEm->Pos_z;
    case 1:
        EF(pEm)->Tmp = (Rnd_l() & 0x1F) + 10;
        pEm->Spd_x = (Rnd_l() & 0x3F) + 80;
        if (Rnd_l() & 3) {
            EM_SET_MOVE(pEm, 3, 0, 7);
            pEm->Routine_2 = 3;
            EF(pEm)->Cnt = 20;
            goto c3;
        }
        if (Rnd_l() & 3)
            EM_SET_MOVE(pEm, 3, 0, 7);
        else
            EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Routine_2 = 2;
        EF(pEm)->Cnt = Rnd_l() & 0x3F;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, EF(pEm)->Tmp);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_x = EM21_LIMIT(pEm->Pos_x, 32000);
        pEm->Pos_z = EM21_LIMIT(pEm->Pos_z, 32000);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 1;
        break;
    case 3:
    c3:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, EF(pEm)->Tmp);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_x = EM21_LIMIT(pEm->Pos_x, 32000);
        pEm->Pos_z = EM21_LIMIT(pEm->Pos_z, 32000);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0) {
            pEm->Routine_2 = 4;
            EF(pEm)->Cnt = Rnd_l() & 0x3F;
        }
        break;
    case 4:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, EF(pEm)->Tmp);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_x = EM21_LIMIT(pEm->Pos_x, 32000);
        pEm->Pos_z = EM21_LIMIT(pEm->Pos_z, 32000);
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 1;
    }
}

/* scenario: hops about on the ground */
void Em21_sce_hop(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 4, 0);
        pEm->Be_flg |= 0x4000;
        pEm->Type |= 0x5000;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Routine_2 = 1;
    case 1:
        pEm->Timer0 = Rnd_l() & 0x7F;
        EF(pEm)->Tmp_0 = (signed char)Rnd_l() % 2;
        EF(pEm)->Tmp_1 = (signed char)Rnd_l() % 2;
        EF(pEm)->Tmp_2 = (signed char)Rnd_l() % 2;
        pEm->Routine_2 = 2;
    case 2:
        pEm->Pos_x += EF(pEm)->Tmp_0;
        pEm->Pos_z += EF(pEm)->Tmp_1;
        pEm->Cdir_y += EF(pEm)->Tmp_2;
        if (pEm->Timer0-- == 0)
            pEm->Routine_2 = 1;
    }
}

/* scenario: flies up out of the room */
void Em21_sce_fly_up(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Type |= 0x4000;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Add_dir & 1) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            EM_SET_MOVE(pEm, 4, 0, 7);
            pEm->Spd_x = 150;
            pEm->Spd_y = -150;
            pEm->Add_dir = 0;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 0x3F))
            Em_21_feather_call(pEm, 1);
        if (pEm->Pos_y < -32000) {
            pEm->Routine_2 = 3;
            pEm->Spd_y = 0;
            pEm->Be_flg |= 0x10;
            pEm->Type |= 0x8000;
        }
    }
}

/* RE2 Em21_Dead: the corpse (Em21_dead_mv by Routine_1) */
void Em21_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em21_dead_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em21_dead_normal: falling to the floor dead, then the corpse */
void Em21_dead_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG D0;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x4000;
        pEm->Damage_cnt = 0x80;
        EM_SET_MOVE(pEm, 0xB, 0, 7);
        EF(pEm)->Cnt = (Rnd_l() & 0x1F) + 0x19;
        pEm->Timer0 = EF(pEm)->Cnt;
        Kage_work_color_set(pEm->Kage.pKw, 0xBFBF10);
        (pEm->Kage.pKw)->Half_x = 400;
        (pEm->Kage.pKw)->Half_z = 400;
        pEm->UnkDA = 60;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            if (pEm->Timer0 > 0x5A) {
                pEm->Move_cnt = 11;
                EF(pEm)->Cnt = 6;
            } else if (Rnd_l() & 1) {
                pEm->Move_cnt = 0;
                EF(pEm)->Cnt = 12;
            } else {
                pEm->Move_cnt = 11;
                EF(pEm)->Cnt = 7;
            }
            pEm->Routine_2 = 2;
        }
        if (pEm->UnkDA) {
            pEm->UnkDA--;
            (pEm->Kage.pKw)->Half_x += 10;
            (pEm->Kage.pKw)->Half_z += 10;
        }
        break;
    case 2:
        D0 = 1;
        if (pEm->Timer0 > 0x5A) {
            EF(pEm)->Cnt2++;
            if (EF(pEm)->Cnt2 & 1)
                D0 = 0;
        }
        if (D0) {
            if (!pEm->Move_cnt && (Rnd_l() & 1) && pEm->Timer0 < 0x5A)
                Snd_se_enem(3, pEm);
            Joint_move(pEm, pKan, pSeq, 0x200);
            EF(pEm)->Cnt--;
        }
        if (!EF(pEm)->Cnt) {
            pEm->Timer0 += (Rnd_l() & 0xF) + 10;
            if (pEm->Timer0 > 0x96) {
                pEm->Routine_2 = 3;
            } else {
                EF(pEm)->Cnt = pEm->Timer0;
                pEm->Routine_2 = 1;
            }
        }
    }
}

/* RE2 Em21_dead_bomb: blown apart: no corpse, shadow off */
void Em21_dead_bomb(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Be_flg |= 0x4000;
        pEm->Damage_cnt = 0x80;
        (pEm->Kage.pKw)->Be_flg = 0;
        pEm->Routine_2 = 1;
    }
}

/* RE2 Em21_dead_instant: dead on the floor at once, the shadow spreads */
void Em21_dead_instant(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x4000;
        pEm->Damage_cnt = 0x80;
        EM_SET_MOVE(pEm, 0xB, 0, 7);
        Kage_work_color_set(pEm->Kage.pKw, 0xBFBF10);
        (pEm->Kage.pKw)->Half_x = 400;
        (pEm->Kage.pKw)->Half_z = 400;
        pEm->UnkDA = 60;
        EF(pEm)->Cnt = 11;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt) {
            EF(pEm)->Cnt--;
            Joint_move(pEm, pKan, pSeq, 0x200);
        }
        if (pEm->UnkDA) {
            pEm->UnkDA--;
            (pEm->Kage.pKw)->Half_x += 10;
            (pEm->Kage.pKw)->Half_z += 10;
        } else {
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        break;
    }
}

/* RE2 Em21_foot_set: keep the feet (part 9) on the spot by moving the body */
void Em21_foot_set(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX M;

    pP = pEm->pSin_parts_ptr;
    RotMatrix(CDIR_SV(pEm), (MATRIX *)pEm->Workm_m);
    CompM((MATRIX *)pEm->Workm_m, PW_M(pP), &M);
    CompM(&M, (MATRIX *)pP[9].M, &M);
    pEm->Pos_x -= M.t[0] - pP[9].Workm.t[0];
}

/* RE2 Em21_r0_set: set Routine_0 / Routine_1, clear Routine_2 / Routine_3 */
void Em21_r0_set(ENEMY_WORK *pEm, ULONG Rno0, ULONG Rno1)
{
    pEm->Routine_0 = Rno0;
    pEm->Routine_1 = Rno1;
    EM_RTN23(pEm) = 0;
}

/* RE2 Em21_r1_set: set Routine_1, clear Routine_2 / Routine_3 */
void Em21_r1_set(ENEMY_WORK *pEm, ULONG Rno1)
{
    pEm->Routine_1 = Rno1;
    EM_RTN23(pEm) = 0;
}

/* RE2 Em21_sca_avoid_ck: turn away from walls and objects in the flight path */
void Em21_sca_avoid_ck(ENEMY_WORK *pEm)
{
    ULONG D0;
    SVECTOR Svec;
    ULONG Cdir_y;

    Cdir_y = pEm->Cdir_y & 0xFFF;
    if (pEm->Col.Hit_flg & 0x20) {
        if (!EF(pEm)->Sca_cnt) {
            if (pEm->Spd_x > 230) {
                Svec.vx = pEm->Pos_x;
                Svec.vy = pEm->Pos_y;
                Svec.vz = pEm->Pos_z;
                D0 = Sca_get_dir_l(&Svec, pEm->Col.pSca_hit_data);
                if (D0 == 2) {
                    if (Cdir_y < 0x80 || Cdir_y > 0xF80)
                        Em21_r1_set(pEm, 10);
                } else {
                    D0 <<= 10;
                    if (((D0 + 0x780) & 0xFFF) < Cdir_y && Cdir_y < ((D0 + 0x880) & 0xFFF))
                        Em21_r1_set(pEm, 10);
                }
            }
        }
        if (EF(pEm)->Sca_cnt > 10) {
            if (!(Rnd_l() & 0x7F))
                Em21_r1_set(pEm, 7);
            else
                Em21_r1_set(pEm, 8);
        }
        if (pEm->Spd_x > 100)
            pEm->Spd_x -= 4;
        EF(pEm)->Sca_cnt++;
    } else {
        EF(pEm)->Sca_cnt = 0;
    }
}

/* RE2 Em21_attack_ck: start an attack on the player (peck in flight or on the ground) */
void Em21_attack_ck(ENEMY_WORK *pEm)
{
    LONG Dy;

    if (G.Player.Life < 0 || G.Player.Damage_cnt)
        return;
    if ((pEm->Unk1CC[4] & 0xA0000) == 0xA0000 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100)) {
        Em21_r1_set(pEm, 13);
        EF(pEm)->Flg &= ~0x40;
        EF(pEm)->Flg &= ~0x08;
        EF(pEm)->Na_cnt = 0;
        return;
    }
    if (EF(pEm)->Na_cnt) {
        EF(pEm)->Na_cnt--;
        return;
    }
    Dy = -(pEm->Pos_y + 2800) + G.Player.Pos_y;
    if ((LONG)pEm->L_pl > 900 && (LONG)pEm->L_pl < 7200 && !(G.Unk75EA & 1) && (EF(pEm)->Flg & 2)
        && Dy < 1800 && Dy > -1800 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100)) {
        G.Unk75EA |= 1;
        EF(pEm)->Flg |= 4;
        if (!(Rnd_l() & 0xF))
            Em21_r1_set(pEm, 11);
        else
            Em21_r1_set(pEm, 13);
        return;
    }
    if ((LONG)pEm->L_pl < 900 && Dy < 900 && Dy > -1800) {
        if (!EF(pEm)->Obj_cnt && pEm->Spd_x > 230 && !(G.Unk75EA & 1)) {
            if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100))
                Em21_r1_set(pEm, 12);
        }
        if (EF(pEm)->Obj_cnt > 10) {
            Em21_r1_set(pEm, 8);
            EF(pEm)->Idly_cnt = Rnd_l() & 0x7F;
            EF(pEm)->Idly_flg = Rnd_area();
        }
        if (pEm->Spd_x > 100)
            pEm->Spd_x -= 4;
        EF(pEm)->Obj_cnt++;
    } else {
        EF(pEm)->Obj_cnt = 0;
    }
}

/* RE2 Em21_run_away_ck: fly off after an attack (Flg 0x10) */
void Em21_run_away_ck(ENEMY_WORK *pEm)
{
    if (EF(pEm)->Flg & 0x10) {
        Em21_atd_set(pEm, 350);
        pEm->Be_flg &= ~0x800;
        EF(pEm)->Idly_cnt = (Rnd_l() & 0x7F) + 30;
        EF(pEm)->Idly_flg = Rnd_area();
        EF(pEm)->Na_cnt = 60;
        Em21_r1_set(pEm, 4);
        EF(pEm)->Flg &= ~0x58;
    }
}

/* RE2 Em21_pl_dead_ck: player close: land next to him (Routine_1 7) */
void Em21_pl_dead_ck(ENEMY_WORK *pEm)
{
    if ((LONG)pEm->L_pl < 1800)
        Em21_r1_set(pEm, 7);
}

/* RE2 Em21_lock_off_ck: give up the attack when the player is far away or gone */
void Em21_lock_off_ck(ENEMY_WORK *pEm)
{
    ULONG D0;

    if (EF(pEm)->Flg & 4) {
        D0 = pEm->Routine_1;
        if (D0 < 11 || D0 > 14) {
            EF(pEm)->Atck = 0;
            EF(pEm)->Atck_se = 0;
            G.Unk75EA &= ~1;
            EF(pEm)->Flg &= ~4;
        }
        if ((LONG)pEm->L_pl > 14400) {
            EF(pEm)->Atck = 0;
            EF(pEm)->Atck_se = 0;
            G.Unk75EA &= ~1;
            EF(pEm)->Flg &= ~4;
        }
    }
    if (EF(pEm)->Flg & 0x80) {
        EF(pEm)->Atck = 0;
        EF(pEm)->Atck_se = 0;
        EF(pEm)->Pl_dead_cnt = 120;
        EF(pEm)->Flg &= ~0x80;
    }
}

/* RE2 Em21_set_flg_all: set Mask in the Flg of every crow that is not attacking */
void Em21_set_flg_all(ULONG Mask)
{
    ENEMY_WORK *pEm;
    ENEMY_WORK **ppEm;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        if (pEm->Id == 0x21 && !(EF(pEm)->Flg & 4)) {
            EF(pEm)->Flg |= Mask;
            EF(pEm)->Idly_cnt = 0;
            EF(pEm)->Idly_flg = 0;
        }
    }
}

/* RE2 Em_21_blood_call: a blood effect near the body */
void Em_21_blood_call(ENEMY_WORK *pEm)
{
    SVECTOR Svec;

    Svec.vx = pEm->Pos_x + (signed char)Rnd_l();
    Svec.vy = pEm->Pos_y + (signed char)Rnd_l();
    Svec.vz = pEm->Pos_z + (signed char)Rnd_l();
    Esp_call(0x100, (USHORT)(Rnd_l() << 4) | 0x28000000, &GsIDMATRIX, &Svec, NULL);
}

/* RE2 Em_21_feather_call: Num feather effects around the body */
void Em_21_feather_call(ENEMY_WORK *pEm, LONG Num)
{
    SVECTOR Svec;

    while (Num-- != 0) {
        Svec.vx = pEm->Pos_x + ((signed char)Rnd_l() << 2);
        Svec.vy = pEm->Pos_y + ((signed char)Rnd_l() << 2);
        Svec.vz = pEm->Pos_z + ((signed char)Rnd_l() << 2);
        Esp_call(0xF, (USHORT)(Rnd_l() << 4) | 0x8000000, &GsIDMATRIX, &Svec, NULL);
    }
}

/* RE2 Em21_atd_set: collision box width and depth */
void Em21_atd_set(ENEMY_WORK *pEm, LONG Size)
{
    pEm->Col.W0 = Size;
    pEm->Col.D0 = Size;
}

/* RE3: player hit while crows were pecking it, scatter the attackers */
void Em21_attacker_off(void)
{
    ENEMY_WORK *pEm;
    ENEMY_WORK **ppEm;
    ULONG D0;

    if (!G.Player.Damage_cnt)
        return;
    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        D0 = pEm->Routine_1;
        if (pEm->Id == 0x21 && (pEm->Be_flg & 1) && (D0 < 4 || D0 > 6)) {
            if (EF(pEm)->Flg & 4) {
                EF(pEm)->Idly_cnt = (Rnd_l() & 0x7F) + 30;
                EF(pEm)->Idly_flg = Rnd_area();
                EF(pEm)->Na_cnt = 10;
                pEm->Be_flg &= ~0x800;
                Em21_r1_set(pEm, 4);
            } else {
                EF(pEm)->Flg |= 0x10;
                EF(pEm)->Idly_cnt = 0;
                EF(pEm)->Idly_flg = 0;
            }
        }
    }
}

/* RE2 PLDAMEGE_EM (Pldamege_Em121/221), runs on the player work */
void Pldamege_Em21(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pT = pEm->pEnemy_ptr;

    if (pT->Routine_0 == 2 || pT->Routine_0 == 3 || pT->Routine_0 == 7) {
        Em21_attacker_off();
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        G.Player.Damage_cnt &= 0x7F;
        pEm->Neck_flg &= 0xED;
        pT->Be_flg &= ~0x800;
        Em21_set_flg_all(0x10);
        pEm->Be_flg &= ~0x8800;
        pT->Be_flg &= ~0x8800;
        return;
    }
    Pldamage_Em21_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em21_pldm00: the player's reaction to the pecking (pEm is the player) */
void Em21_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Damage_no = 0;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_MOVE(pEm, 1, 0, 7);
            pEm->Routine_2 = 2;
        }
        Add_speedXZ_n(pEm, 0, 0);
        break;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Hokan_flg = 7;
        Add_speedXZ_n(pEm, 0, 0);
        break;
    case 3:
        if (G.Player.Life < 0) {
            G.Player.Routine_0 = 3;
            G.Player.Routine_1 = 0;
            G.Player.Routine_2 = 0;
            G.Player.Routine_3 = 0;
            break;
        }
        EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Routine_2 = 4;
    case 4:
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_MOVE(pEm, 1, 0, 7);
            Em21_r0_set(pEm, 1, 0);
        }
        Add_speedXZ_n(pEm, 0, 0);
        break;
    case 5:
        break;
    }
}

/* RE2 Em21_dummy_br: empty */
void Em21_dummy_br(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_dummy: empty */
void Em21_dummy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}
