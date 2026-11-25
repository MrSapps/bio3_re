#include "enemy/em3e.h"
#include "md_sub.h"
#include "joint.h"
#include "root_ck.h"
#include "sld.h"
#include "snd.h"
#include "player.h"

/* per-frame entry (RE2 EM_MAIN) */
void Em3e_main(ENEMY_WORK *pEm)
{
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em3e_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Em3e_rotor(pEm);
    Em3e_hover(pEm);
    EM3E_W(pEm)->Old_life = pEm->Life;
}

/* Routine_0 0: set up the parts, collision and flight zone */
void Em3e_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    COL_OBJ *pCol;
    PARTS_W *pP;

    EM_SET_MOVE(pEm, 0, 0, 7);
    EM_SET_RTN(pEm, 4, 0, 0, 0);
    Joint_move(pEm, pKan, pSeq, 0x100);
    pEm->Hit_area.Top = 1800;
    pEm->Hit_area.Bottom = -2700;
    pEm->Hit_area.Rate0 = 0x78;
    pEm->Hit_area.Rate1 = 0xBC;
    pEm->Hit_area.R = 2700;
    pEm->Unk140 = 0;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr[4].Workm;
    pCol = &pEm->Col;
    pCol->H = 4500;
    pCol->pEm = pEm;
    pCol->D0 = 2700;
    pCol->W0 = 2700;
    pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 1800;
    pCol->Flg = 0;
    pCol->Type = 2;
    Oba_set(pCol, 0);
    EM3E_W(pEm)->Sway_rtn = 0;
    EM3E_W(pEm)->Bob_rtn = 0;
    EM3E_W(pEm)->Time = 0;
    EM3E_W(pEm)->Shot_wait = 20;
    EM3E_W(pEm)->Tilt = 0;
    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Spd_base_x = 0;
    pEm->Spd_base_y = 0;
    pEm->Spd_base_z = 0;
    pP = pEm->pSin_parts_ptr;
    pP->Cdir_x = 0;
    pP = &pEm->pSin_parts_ptr[1];
    pEm->Neck_flg = 0;
    pEm->Neck_no = 4;
    PW_NECK(pP)->Add_dir_y = 0x40;
    PW_NECK(pP)->Add_dir_z = 0x40;
    PW_NECK(pP)->L_rot_y = 0x200;
    PW_NECK(pP)->L_rot_z = 0x138;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = 250;
    else
        pEm->Max_life = pEm->Life = 350;
    if (pEm->Type & 1) {
        pEm->Routine_1 = 1;
        EM3E_W(pEm)->Parts_num = 5;
        EM3E_W(pEm)->Abr0 = 2;
        EM3E_W(pEm)->Rotor0 = 1;
        EM3E_W(pEm)->Rotor1 = 4;
    } else {
        EM3E_W(pEm)->Parts_num = 8;
        EM3E_W(pEm)->Rotor0 = 2;
        EM3E_W(pEm)->Abr0 = 3;
        pEm->Routine_1 = 0;
        EM3E_W(pEm)->Rotor1 = 7;
        Em3e_parts_trans(pEm);
    }
    pEm->Unk14E = 0x110;
    Em3e_parts_abr(pEm, EM3E_W(pEm)->Abr0);
    Em3e_parts_abr(pEm, EM3E_W(pEm)->Rotor1);
    EM_SET_MOVE(pEm, 0, 0, 0);
    Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
}

/* Routine_0 1: move and fly routines by Routine_1 */
void Em3e_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (G.System_flg & 0x100) {
        if (EM3E_W(pEm)->Time++ > 1800)
            Em_dead_flg_on_cand(pEm);
    } else if (EM3E_W(pEm)->Time++ > 3600)
        Em_dead_flg_on_cand(pEm);
    Em3e_move_tbl[pEm->Routine_1](pEm);
    Em3e_fly_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* move 0: pick a destination at the other end of the zone */
void Em3e_dest_set(ENEMY_WORK *pEm)
{
    if (pEm->Pos_x > 20000) {
        pEm->Dest_x = 10000;
        pEm->Dest_z = Rnd() * -25;
    } else {
        pEm->Dest_x = 27000;
        pEm->Dest_z = Rnd() * -25;
    }
    Em3e_rtn1_set(pEm, 1);
}

/* fly 0: nothing */
void Em3e_fly_nop(ENEMY_WORK *pEm)
{
}

/* move 1: turn back at the destination box or the zone edge (timer 30); shoot when the player is level (Shot_wait) */
void Em3e_fly(ENEMY_WORK *pEm)
{
    LONG Hi;
    LONG Lo;
    SHORT Area[2][4];

    if (pEm->Timer0 == 0) {
        Area[0][0] = pEm->Dest_x - 450;
        Area[0][1] = pEm->Dest_z - 450;
        Area[0][2] = 900;
        Area[0][3] = 900;
        Area[1][0] = 10000;
        Area[1][1] = -25000;
        Area[1][2] = 17000;
        Area[1][3] = 25000;
        if (Hit_ck_box(&pEm->Pos_x, Area[0]) || !Hit_ck_box(&pEm->Pos_x, Area[1])) {
            pEm->Timer0 = 30;
            Em3e_rtn1_set(pEm, 0);
        }
    } else {
        pEm->Timer0--;
    }
    if (EM3E_W(pEm)->Shot_wait != 0) {
        EM3E_W(pEm)->Shot_wait--;
    } else {
        Hi = pEm->Pos_x + 1800;
        Lo = pEm->Pos_x - 1800;
        if (Pl_work.Pos_x < Hi && Lo < Pl_work.Pos_x) {
            EM3E_W(pEm)->Shot_wait = (Rnd() & 0x7F) + 10;
            Em3e_shot_check(pEm, 2);
        }
    }
}

/* fly 1: head for the destination */
void Em3e_fly_dest(ENEMY_WORK *pEm)
{

    switch (pEm->Routine_2) {
    case 0:
        EM3E_W(pEm)->Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);
        pEm->Spd_x = Rsin_tbl[(EM3E_W(pEm)->Dir / 16 + 0x40) & 0xFF] * 240 / 256;
        pEm->Spd_z = -(Rsin_tbl[(EM3E_W(pEm)->Dir / 16) & 0xFF] * 240 / 256);
        pEm->Routine_2++;
    case 1:
        Em3e_add_speed(pEm);
    }
}

/* Routine_0 2, 3: damage by the hit kind (Em3e_damage_tbl[Damage_no - 1]) */
void Em3e_Damage(ENEMY_WORK *pEm)
{
    Em3e_damage_tbl[pEm->Damage_no - 1](pEm);
    Em3e_add_speed(pEm);
    Em3e_retarget(pEm);
    if (pEm->Life < 0) {
        Em_dead_flg_on_cand(pEm);
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Spd_base_x = 0;
        pEm->Spd_base_y = 0;
        pEm->Spd_base_z = 0;
    }
}

/* damage without effect on life: sparks */
void Em3e_dm_spark0(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP = pEm->pSin_parts_ptr;

    pEm->Damage_cnt = 0;
    EM3E_W(pEm)->Time += 60;
    pEm->Life = EM3E_W(pEm)->Old_life;
    Ofs.vx = 0x141E;
    Ofs.vy = (signed char)Rnd() << 2;
    Ofs.vz = (signed char)Rnd() << 2;
    Esp_call(1, 0x14000000, &pP->Workm, &Ofs, NULL);
    EM_RTN(pEm) = pEm->Old_routine;
}

/* damage: sparks */
void Em3e_dm_spark(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP = pEm->pSin_parts_ptr;

    EM3E_W(pEm)->Time += 60;
    Ofs.vx = 0x141E;
    Ofs.vy = (signed char)Rnd() << 2;
    Ofs.vz = (signed char)Rnd() << 2;
    Esp_call(1, 0x1C000000, &pP->Workm, &Ofs, NULL);
    EM_RTN(pEm) = pEm->Old_routine;
}

/* damage: spin around and back */
void Em3e_dm_spin(ENEMY_WORK *pEm)
{

    switch (pEm->Routine_3) {
    case 0:
        EM3E_W(pEm)->Turn = 20;
        EM3E_W(pEm)->Time += 60;
        pEm->Damage_cnt |= 0x80;
        pEm->Routine_3++;
        break;
    case 1:
        pEm->Cdir_y += EM3E_W(pEm)->Turn;
        if (--EM3E_W(pEm)->Turn > 0)
            break;
        pEm->Routine_3++;
        break;
    case 2:
        pEm->Cdir_y -= 3;
        if (pEm->Cdir_y >= 0xC00)
            break;
        pEm->Routine_3++;
        break;
    case 3:
        pEm->Damage_cnt = 0;
        pEm->Cdir_y = 0xC00;
        EM_RTN(pEm) = pEm->Old_routine;
        break;
    }
}

/* damage: no reaction */
void Em3e_dm_none(ENEMY_WORK *pEm)
{
    EM3E_W(pEm)->Time += 60;
    EM_RTN(pEm) = pEm->Old_routine;
}

/* Routine_0 4: scenario routine by Routine_1 */
void Em3e_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Root_ck(pEm, POS_V(&G.Player), 0, 0);
    Em3e_sce_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* sce 0: hover, explode Add_dir times on request, fire on request */
void Em3e_sce_hover(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x40;
        pEm->Routine_2++;
    case 1:
        if (pEm->Add_dir == 0)
            break;
        EM3E_W(pEm)->Timer = 0;
        pEm->Routine_2++;
        break;
    case 2:
        if (EM3E_W(pEm)->Timer-- != 0)
            break;
        EM3E_W(pEm)->Timer = 2;
        Em3e_parts_trans_off(pEm);
        Em3e_explode(pEm);
        if (--pEm->Add_dir != 0)
            break;
        pEm->Routine_2++;
        break;
    case 3:
        break;
    case 4:
        Em3e_shot_l(pEm);
        pEm->Routine_2 = 3;
        break;
    case 6:
        Em3e_shot_l(pEm);
    case 5:
        Em3e_shot_r(pEm);
        pEm->Routine_2 = 3;
        break;
    }
}

/* sce 1: crash */
void Em3e_sce_crash(ENEMY_WORK *pEm)
{
    LONG Range;
    LONG Fall;
    LONG Size;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x2050;
        pEm->Type |= 0x4000;
        Em3e_parts_hide(pEm);
        pEm->Routine_2++;
    case 1:
        if (pEm->Add_dir != 1)
            break;
        pEm->Add_dir = 0;
        EM3E_W(pEm)->Timer = 0;
        pEm->Be_flg ^= 0x10;
        pEm->Routine_2++;
        EM3E_W(pEm)->Turn = pEm->Pos_y;
        EM3E_W(pEm)->Start_x = pEm->Pos_x;
        EM3E_W(pEm)->Fall_x = pEm->Pos_x - 20000;
        break;
    case 2:
        pEm->Pos_y -= EM3E_W(pEm)->Timer >> 4;
        EM3E_W(pEm)->Timer += 10;
        Range = EM3E_W(pEm)->Turn + 26000;
        Fall = EM3E_W(pEm)->Turn - pEm->Pos_y;
        Size = (Fall << 12) / Range;
        Em3e_parts_size(pEm, 0, Size);
        Em3e_parts_size(pEm, 1, Size);
        Em3e_parts_size(pEm, 2, Size);
        pEm->Pos_x = EM3E_W(pEm)->Start_x - EM3E_W(pEm)->Fall_x * Fall / Range;
        if (pEm->Pos_y < -25999) {
            pEm->Add_dir = 1;
            Em3e_parts_show(pEm);
            pEm->Routine_2++;
        }
        break;
    }
}

/* Routine_0 7 */
void Em3e_Dead(ENEMY_WORK *pEm)
{
    if (!pEm->Routine_2) {
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Spd_base_x = 0;
        pEm->Spd_base_y = 0;
        pEm->Spd_base_z = 0;
        pEm->Routine_2++;
        Em3e_parts_trans(pEm);
    }
}

/* shrink parts 0..2 to nothing and toggle parts 3, 4 */
void Em3e_parts_hide(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    for (i = 0; i < 3; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->Size_x = 0;
        pP->Size_y = 0;
        pP->Size_z = 0;
        pP->Be_flg |= 4;
    }
    pP = &pEm->pSin_parts_ptr[3];
    pP->UnkA2 ^= 1;
    pP = &pEm->pSin_parts_ptr[4];
    pP->UnkA2 ^= 1;
}

/* scale part No by Size (0x1000 = 1) */
void Em3e_parts_size(ENEMY_WORK *pEm, ULONG No, LONG Size)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[No];

    pP->Size_x = Size;
    pP->Size_y = Size;
    pP->Size_z = Size;
}

/* parts 0..2 at full size, parts 3 and 4 visible */
void Em3e_parts_show(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    for (i = 0; i < 3; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->Size_x = 0x1000;
        pP->Size_y = 0x1000;
        pP->Size_z = 0x1000;
        pP->Be_flg |= 4;
    }
    pP = &pEm->pSin_parts_ptr[3];
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[4];
    pP->UnkA2 |= 1;
}

/* set part flag 0x400 on the first Parts_num parts */
void Em3e_parts_trans(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    for (i = 0; i < EM3E_W(pEm)->Parts_num; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 |= 0x400;
    }
}

/* clear part flag 0x400 on the first Parts_num parts */
void Em3e_parts_trans_off(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    for (i = 0; i < EM3E_W(pEm)->Parts_num; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x400;
    }
}

/* draw part No semi-transparent */
void Em3e_parts_abr(ENEMY_WORK *pEm, ULONG No)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[No];

    pP->UnkA2 |= 0x100;
    Md_set_abr(pP->pTmd, 1);
}

/* turn both rotors by Unk14E */
void Em3e_rotor(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = &pEm->pSin_parts_ptr[EM3E_W(pEm)->Rotor0];
    pP->Cdir_y += pEm->Unk14E;
    RotMatrix(CDIR_SV(pP), PW_M(pP));
    pP = &pEm->pSin_parts_ptr[EM3E_W(pEm)->Rotor1];
    pP->Cdir_y += pEm->Unk14E;
    RotMatrix(CDIR_SV(pP), PW_M(pP));
}

/* fire when inside the firing zone (Mode 0: never, 2: both guns in the middle) */
void Em3e_shot_check(ENEMY_WORK *pEm, LONG Mode)
{
    LONG X = pEm->Pos_x;

    if (X > 13000 && X < 26000 && pEm->Pos_y > -5200 && pEm->Pos_y < -3600 && Mode) {
        if (X > 13000 && X < 17900)
            Em3e_shot_l(pEm);
        if (X > 21400 && X < 26000)
            Em3e_shot_r(pEm);
        if (X > 17900 && X < 21400) {
            if (Mode == 2) {
                Em3e_shot_l(pEm);
                Em3e_shot_r(pEm);
            } else if (Rnd() & 0x80)
                Em3e_shot_l(pEm);
            else
                Em3e_shot_r(pEm);
        }
    }
}

/* muzzle flash and shot sound of the right gun */
void Em3e_shot_r(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP;

    Ofs.vx = 30;
    Ofs.vy = 200;
    Ofs.vz = 0x80C;
    pP = &pEm->pSin_parts_ptr[1];
    Esp_call(0xE02, (USHORT)pEm->Cdir_y | 0x8000000, &pP->Workm, &Ofs, NULL);
    Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
}

/* muzzle flash and shot sound of the left gun */
void Em3e_shot_l(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP;

    Ofs.vx = 30;
    Ofs.vy = 200;
    Ofs.vz = -0x80C;
    pP = &pEm->pSin_parts_ptr[1];
    Esp_call(0xE02, (USHORT)pEm->Cdir_y | 0x8000000, &pP->Workm, &Ofs, NULL);
    Snd_se_stad(0x1020F, POS_V(pEm), 0, 0);
}

/* explosion effects and sound on the body */
void Em3e_explode(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    SVECTOR Spd;
    VECTOR Pos;
    PARTS_W *pP = pEm->pSin_parts_ptr;

    Pos.vx = -700;
    Pos.vy = 2523;
    Pos.vz = 0;
    Snd_se_stad(0x1022F, &Pos, 0, 0);
    Ofs.vx = -700;
    Ofs.vy = 2523;
    Ofs.vz = 0;
    Esp_call(1, 0x16000000, &pP[5].Workm, &Ofs, NULL);
    Ofs.vx = -700;
    Ofs.vy = 2884;
    Ofs.vz = 0;
    Spd.vx = 0;
    Spd.vy = 0;
    Spd.vz = -0x400;
    Esp_call(0x801, 0x16000000, &pP[5].Workm, &Ofs, &Spd);
    Ofs.vx = -450;
    Ofs.vy = 2523;
    Ofs.vz = 20;
    Spd.vx = -0x400;
    Spd.vy = 0x600;
    Spd.vz = 0;
    Esp_call(0x801, 0x10000000, &pP[5].Workm, &Ofs, &Spd);
    Ofs.vx = -850;
    Ofs.vy = 2523;
    Ofs.vz = -100;
    Spd.vx = -0x200;
    Spd.vy = 0x200;
    Spd.vz = -0x300;
    Esp_call(0x801, 0x10000000, &pP[5].Workm, &Ofs, &Spd);
    Ofs.vx = -850;
    Ofs.vy = 2523;
    Ofs.vz = 140;
    Spd.vx = 0x200;
    Spd.vy = -0x200;
    Spd.vz = -0x300;
    Esp_call(0x801, 0x10000000, &pP[5].Workm, &Ofs, &Spd);
}

/* pitch sway, height bob, speed easing and body tilt */
void Em3e_hover(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    SVECTOR Rot;

    switch (EM3E_W(pEm)->Sway_rtn) {
    case 0:
        EM3E_W(pEm)->Sway_cnt = EM3E_W(pEm)->Sway_time = (Rnd() & 0x1F) + 15;
        EM3E_W(pEm)->Sway_rtn++;
    case 1:
        pP->Cdir_x += 2;
        if (--EM3E_W(pEm)->Sway_cnt == 0) {
            EM3E_W(pEm)->Sway_cnt = EM3E_W(pEm)->Sway_time * 2;
            EM3E_W(pEm)->Sway_rtn++;
        }
        break;
    case 2:
        pP->Cdir_x -= 2;
        if (--EM3E_W(pEm)->Sway_cnt == 0) {
            EM3E_W(pEm)->Sway_cnt = EM3E_W(pEm)->Sway_time;
            EM3E_W(pEm)->Sway_rtn++;
        }
        break;
    case 3:
        pP->Cdir_x += 2;
        if (--EM3E_W(pEm)->Sway_cnt == 0)
            EM3E_W(pEm)->Sway_rtn = 0;
        break;
    }
    switch (EM3E_W(pEm)->Bob_rtn) {
    case 0:
        EM3E_W(pEm)->Bob_cnt = EM3E_W(pEm)->Bob_time = (Rnd() & 0x1F) + 25;
        EM3E_W(pEm)->Bob_rtn++;
    case 1:
        pEm->Pos_y += 4;
        if (--EM3E_W(pEm)->Bob_cnt == 0) {
            EM3E_W(pEm)->Bob_cnt = EM3E_W(pEm)->Bob_time * 2;
            EM3E_W(pEm)->Bob_rtn++;
        }
        break;
    case 2:
        pEm->Pos_y -= 4;
        if (--EM3E_W(pEm)->Bob_cnt == 0) {
            EM3E_W(pEm)->Bob_cnt = EM3E_W(pEm)->Bob_time;
            EM3E_W(pEm)->Bob_rtn++;
        }
        break;
    case 3:
        pEm->Pos_y += 4;
        if (--EM3E_W(pEm)->Bob_cnt == 0)
            EM3E_W(pEm)->Bob_rtn = 0;
        break;
    }
    if (pEm->Spd_x != pEm->Spd_base_x) {
        if (pEm->Spd_base_x < pEm->Spd_x) {
            pEm->Spd_base_x += 4;
            if (pEm->Spd_base_x >= pEm->Spd_x)
                pEm->Spd_base_x = pEm->Spd_x;
        } else {
            pEm->Spd_base_x -= 4;
            if (pEm->Spd_base_x <= pEm->Spd_x)
                pEm->Spd_base_x = pEm->Spd_x;
        }
    }
    if (pEm->Spd_x > 0) {
        if (EM3E_W(pEm)->Tilt < (pEm->Spd_x << 7) / 240)
            EM3E_W(pEm)->Tilt += 4;
    } else {
        if (EM3E_W(pEm)->Tilt >= (pEm->Spd_x << 7) / 240)
            EM3E_W(pEm)->Tilt -= 4;
    }
    Rot.vx = pP->Cdir_x + EM3E_W(pEm)->Tilt;
    Rot.vy = 0;
    Rot.vz = 0;
    RotMatrix(&Rot, PW_M(pP));
}

/* set Routine_0 / Routine_1, clear Routine_2 / Routine_3 */
void Em3e_rtn_set(ENEMY_WORK *pEm, LONG Rtn0, LONG Rtn1)
{
    pEm->Routine_0 = Rtn0;
    pEm->Routine_1 = Rtn1;
    EM_RTN23(pEm) = 0;
}

/* set Routine_1, clear Routine_2 / Routine_3 */
void Em3e_rtn1_set(ENEMY_WORK *pEm, LONG Rtn1)
{
    pEm->Routine_1 = Rtn1;
    EM_RTN23(pEm) = 0;
}

/* move by the speed */
void Em3e_add_speed(ENEMY_WORK *pEm)
{
    pEm->Pos_x += pEm->Spd_base_x;
    pEm->Pos_y += pEm->Spd_y;
    pEm->Pos_z += pEm->Spd_z;
}

/* re-aim at a new destination when reached or out of the zone (while damaged) */
void Em3e_retarget(ENEMY_WORK *pEm)
{
    SHORT Zone[4];
    SHORT Area[4];

    if (pEm->Timer0 == 0) {
        Zone[0] = pEm->Dest_x - 450;
        Zone[1] = pEm->Dest_z - 450;
        Zone[2] = 900;
        Zone[3] = 900;
        Area[0] = 10000;
        Area[1] = -25000;
        Area[2] = 17000;
        Area[3] = 25000;
        if (Hit_ck_box(&pEm->Pos_x, Zone) || !Hit_ck_box(&pEm->Pos_x, Area)) {
            pEm->Timer0 = 30;
            if (pEm->Pos_x > 20000) {
                pEm->Dest_x = 10000;
                pEm->Dest_z = Rnd() * -25;
            } else {
                pEm->Dest_x = 27000;
                pEm->Dest_z = Rnd() * -25;
            }
            EM3E_W(pEm)->Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);
            pEm->Spd_x = Rsin_tbl[(EM3E_W(pEm)->Dir / 16 + 0x40) & 0xFF] * 240 / 256;
            pEm->Spd_z = -(Rsin_tbl[(EM3E_W(pEm)->Dir / 16) & 0xFF] * 240 / 256);
        }
    } else
        pEm->Timer0--;
}
