#include "common.h"
#include "ovl/em3e.h"
#include "md_sub.h"

/* routine tables (module .data), unprototyped: routines take (pEm, Kan, Seq) */
extern void (*Em3e_routine_0[8])() __asm__("D_80101754");
extern void (*Em3e_move_tbl[2])() __asm__("D_80101774");
extern void (*Em3e_fly_tbl[2])() __asm__("D_8010177C");
extern void (*Em3e_damage_tbl[31])() __asm__("D_80101784"); /* index weapon Unk12C - 1 */
extern void (*Em3e_sce_tbl[2])() __asm__("D_80101800");

void func_80100E0C(ENEMY_WORK *pEm); /* Em3e_rotor */
void func_80101210(ENEMY_WORK *pEm); /* Em3e_hover */
void func_80100D24(ENEMY_WORK *pEm); /* Em3e_parts_trans */
void func_80100D70(ENEMY_WORK *pEm); /* Em3e_parts_trans_off */
void func_80100DC0(ENEMY_WORK *pEm, ULONG No); /* Em3e_parts_abr */
void func_80100EAC(ENEMY_WORK *pEm, LONG Mode); /* Em3e_shot_check */
void func_80100F94(ENEMY_WORK *pEm); /* Em3e_shot_r */
void func_80101008(ENEMY_WORK *pEm); /* Em3e_shot_l */
void func_8010107C(ENEMY_WORK *pEm); /* Em3e_explode */
void func_80100C0C(ENEMY_WORK *pEm); /* Em3e_parts_hide */
void func_80100C80(ENEMY_WORK *pEm, ULONG No, LONG Size); /* Em3e_parts_size */
void func_80100CAC(ENEMY_WORK *pEm); /* Em3e_parts_show */
void func_80101580(ENEMY_WORK *pEm, LONG Rtn1); /* Em3e_rtn1_set */
void func_8010158C(ENEMY_WORK *pEm); /* Em3e_add_speed */
void func_801015C0(ENEMY_WORK *pEm); /* Em3e_retarget */

/* EM_MAIN */
void func_80100020(ENEMY_WORK *pEm)
{
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em3e_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    func_80100E0C(pEm);
    func_80101210(pEm);
    EM3E_W(pEm)->Old_life = pEm->Life;
}

/* Em3e_init */
void func_801000A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    COL_OBJ *pCol;
    PARTS_W *pP;

    EM_MOVE(pEm) = 0x70000;
    EM_RTN(pEm) = 4;
    Joint_move(pEm, pKan, pSeq, 0x100);
    pEm->Unk134 = 1800;
    pEm->Unk136 = -2700;
    pEm->Unk138 = 0x78;
    pEm->Unk139 = 0xBC;
    pEm->Unk13A = 2700;
    pEm->Unk140 = 0;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr[4].Workm;
    pCol = (COL_OBJ *)&pEm->Col_id;
    pCol->H = 4500;
    pCol->pEm = pEm;
    pCol->D0 = 2700;
    pCol->W0 = 2700;
    pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 1800;
    pCol->Flg = 0;
    pCol->Type = 2;
    func_80033580(pCol, 0);
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
    pEm->Unk120 = 0;
    pEm->Neck_no = 4;
    PW_FREE(pP)[2] = 0x40;
    PW_FREE(pP)[3] = 0x40;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[5] = 0x138;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
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
        func_80100D24(pEm);
    }
    pEm->Unk14E = 0x110;
    func_80100DC0(pEm, EM3E_W(pEm)->Abr0);
    func_80100DC0(pEm, EM3E_W(pEm)->Rotor1);
    EM_MOVE(pEm) = 0;
    Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
}

/* Em3e_move: Routine_0 1 */
void func_8010027C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
void func_80100360(ENEMY_WORK *pEm)
{
    if (pEm->Pos_x > 20000) {
        pEm->Dest_x = 10000;
        pEm->Dest_z = Rnd() * -25;
    } else {
        pEm->Dest_x = 27000;
        pEm->Dest_z = Rnd() * -25;
    }
    func_80101580(pEm, 1);
}

void func_801003C4(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em3e/em3e", func_801003CC);

/* fly 1: head for the destination */
void func_801004F4(ENEMY_WORK *pEm)
{

    switch (pEm->Routine_2) {
    case 0:
        EM3E_W(pEm)->Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->Dest_x, pEm->Dest_z);
        pEm->Spd_x = Rsin_tbl[(EM3E_W(pEm)->Dir / 16 + 0x40) & 0xFF] * 240 / 256;
        pEm->Spd_z = -(Rsin_tbl[(EM3E_W(pEm)->Dir / 16) & 0xFF] * 240 / 256);
        pEm->Routine_2++;
    case 1:
        func_8010158C(pEm);
    }
}

/* Em3e_damage: Routine_0 2, 3 */
void func_801005E4(ENEMY_WORK *pEm)
{
    Em3e_damage_tbl[pEm->Unk12C - 1](pEm);
    func_8010158C(pEm);
    func_801015C0(pEm);
    if (pEm->Life < 0) {
        Em_dead_flg_on_cand(pEm);
        EM_RTN(pEm) = 7;
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Spd_base_x = 0;
        pEm->Spd_base_y = 0;
        pEm->Spd_base_z = 0;
    }
}

/* damage without effect on life: sparks */
void func_80100674(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP = pEm->pSin_parts_ptr;

    pEm->Damage_cnt = 0;
    EM3E_W(pEm)->Time += 60;
    pEm->Life = EM3E_W(pEm)->Old_life;
    Ofs.vx = 0x141E;
    Ofs.vy = (signed char)Rnd() << 2;
    Ofs.vz = (signed char)Rnd() << 2;
    func_8001B484(1, 0x14000000, &pP->Workm, &Ofs, NULL);
    EM_RTN(pEm) = pEm->Unk1C4;
}

/* damage: sparks */
void func_80100704(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP = pEm->pSin_parts_ptr;

    EM3E_W(pEm)->Time += 60;
    Ofs.vx = 0x141E;
    Ofs.vy = (signed char)Rnd() << 2;
    Ofs.vz = (signed char)Rnd() << 2;
    func_8001B484(1, 0x1C000000, &pP->Workm, &Ofs, NULL);
    EM_RTN(pEm) = pEm->Unk1C4;
}

/* damage: spin around and back */
void func_80100788(ENEMY_WORK *pEm)
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
        EM_RTN(pEm) = pEm->Unk1C4;
        break;
    }
}

/* damage: no reaction */
void func_80100878(ENEMY_WORK *pEm)
{
    EM3E_W(pEm)->Time += 60;
    EM_RTN(pEm) = pEm->Unk1C4;
}

/* Em3e_sce: Routine_0 4 */
void func_80100890(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Root_ck(pEm, (VECTOR *)&G.Pl_pos_x, 0, 0);
    Em3e_sce_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* sce 0: hover, explode Add_dir times on request, fire on request */
void func_80100908(ENEMY_WORK *pEm)
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
        func_80100D70(pEm);
        func_8010107C(pEm);
        if (--pEm->Add_dir != 0)
            break;
        pEm->Routine_2++;
        break;
    case 3:
        break;
    case 4:
        func_80101008(pEm);
        pEm->Routine_2 = 3;
        break;
    case 6:
        func_80101008(pEm);
    case 5:
        func_80100F94(pEm);
        pEm->Routine_2 = 3;
        break;
    }
}

/* sce 1: crash */
void func_80100A10(ENEMY_WORK *pEm)
{
    LONG Range;
    LONG Fall;
    LONG Size;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x2050;
        pEm->Type |= 0x4000;
        func_80100C0C(pEm);
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
        func_80100C80(pEm, 0, Size);
        func_80100C80(pEm, 1, Size);
        func_80100C80(pEm, 2, Size);
        pEm->Pos_x = EM3E_W(pEm)->Start_x - EM3E_W(pEm)->Fall_x * Fall / Range;
        if (pEm->Pos_y < -25999) {
            pEm->Add_dir = 1;
            func_80100CAC(pEm);
            pEm->Routine_2++;
        }
        break;
    }
}

/* Em3e_dead: Routine_0 7 */
void func_80100BBC(ENEMY_WORK *pEm)
{
    if (!pEm->Routine_2) {
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Spd_base_x = 0;
        pEm->Spd_base_y = 0;
        pEm->Spd_base_z = 0;
        pEm->Routine_2++;
        func_80100D24(pEm);
    }
}

/* Em3e_parts_hide */
void func_80100C0C(ENEMY_WORK *pEm)
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

/* Em3e_parts_size */
void func_80100C80(ENEMY_WORK *pEm, ULONG No, LONG Size)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[No];

    pP->Size_x = Size;
    pP->Size_y = Size;
    pP->Size_z = Size;
}

/* Em3e_parts_show */
void func_80100CAC(ENEMY_WORK *pEm)
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

/* Em3e_parts_trans */
void func_80100D24(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    for (i = 0; i < EM3E_W(pEm)->Parts_num; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 |= 0x400;
    }
}

/* Em3e_parts_trans_off */
void func_80100D70(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    for (i = 0; i < EM3E_W(pEm)->Parts_num; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x400;
    }
}

/* Em3e_parts_abr */
void func_80100DC0(ENEMY_WORK *pEm, ULONG No)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[No];

    pP->UnkA2 |= 0x100;
    Md_set_abr((MD_OBJ *)pP->pTmd, 1);
}

/* Em3e_rotor */
void func_80100E0C(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = &pEm->pSin_parts_ptr[EM3E_W(pEm)->Rotor0];
    pP->Cdir_y += pEm->Unk14E;
    RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
    pP = &pEm->pSin_parts_ptr[EM3E_W(pEm)->Rotor1];
    pP->Cdir_y += pEm->Unk14E;
    RotMatrix((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
}

/* fire when inside the firing zone (Mode 0: never, 2: both guns in the middle) */
/* Em3e_shot_check */
void func_80100EAC(ENEMY_WORK *pEm, LONG Mode)
{
    LONG X = pEm->Pos_x;

    if (X > 13000 && X < 26000 && pEm->Pos_y > -5200 && pEm->Pos_y < -3600 && Mode) {
        if (X > 13000 && X < 17900)
            func_80101008(pEm);
        if (X > 21400 && X < 26000)
            func_80100F94(pEm);
        if (X > 17900 && X < 21400) {
            if (Mode == 2) {
                func_80101008(pEm);
                func_80100F94(pEm);
            } else if (Rnd() & 0x80)
                func_80101008(pEm);
            else
                func_80100F94(pEm);
        }
    }
}

/* Em3e_shot_r */
void func_80100F94(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP;

    Ofs.vx = 30;
    Ofs.vy = 200;
    Ofs.vz = 0x80C;
    pP = &pEm->pSin_parts_ptr[1];
    func_8001B484(0xE02, (USHORT)pEm->Cdir_y | 0x8000000, &pP->Workm, &Ofs, NULL);
    Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
}

/* Em3e_shot_l */
void func_80101008(ENEMY_WORK *pEm)
{
    SVECTOR Ofs;
    PARTS_W *pP;

    Ofs.vx = 30;
    Ofs.vy = 200;
    Ofs.vz = -0x80C;
    pP = &pEm->pSin_parts_ptr[1];
    func_8001B484(0xE02, (USHORT)pEm->Cdir_y | 0x8000000, &pP->Workm, &Ofs, NULL);
    Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
}

/* Em3e_explode */
void func_8010107C(ENEMY_WORK *pEm)
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
    func_8001B484(1, 0x16000000, &pP[5].Workm, &Ofs, NULL);
    Ofs.vx = -700;
    Ofs.vy = 2884;
    Ofs.vz = 0;
    Spd.vx = 0;
    Spd.vy = 0;
    Spd.vz = -0x400;
    func_8001B484(0x801, 0x16000000, &pP[5].Workm, &Ofs, &Spd);
    Ofs.vx = -450;
    Ofs.vy = 2523;
    Ofs.vz = 20;
    Spd.vx = -0x400;
    Spd.vy = 0x600;
    Spd.vz = 0;
    func_8001B484(0x801, 0x10000000, &pP[5].Workm, &Ofs, &Spd);
    Ofs.vx = -850;
    Ofs.vy = 2523;
    Ofs.vz = -100;
    Spd.vx = -0x200;
    Spd.vy = 0x200;
    Spd.vz = -0x300;
    func_8001B484(0x801, 0x10000000, &pP[5].Workm, &Ofs, &Spd);
    Ofs.vx = -850;
    Ofs.vy = 2523;
    Ofs.vz = 140;
    Spd.vx = 0x200;
    Spd.vy = -0x200;
    Spd.vz = -0x300;
    func_8001B484(0x801, 0x10000000, &pP[5].Workm, &Ofs, &Spd);
}

/* pitch sway, height bob, speed easing and body tilt */
/* Em3e_hover */
void func_80101210(ENEMY_WORK *pEm)
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
    RotMatrix(&Rot, (MATRIX *)pP->M);
}

void func_80101570(ENEMY_WORK *pEm, LONG Rtn0, LONG Rtn1)
{
    pEm->Routine_0 = Rtn0;
    pEm->Routine_1 = Rtn1;
    EM_RTN23(pEm) = 0;
}

/* Em3e_rtn1_set */
void func_80101580(ENEMY_WORK *pEm, LONG Rtn1)
{
    pEm->Routine_1 = Rtn1;
    EM_RTN23(pEm) = 0;
}

/* Em3e_add_speed */
void func_8010158C(ENEMY_WORK *pEm)
{
    pEm->Pos_x += pEm->Spd_base_x;
    pEm->Pos_y += pEm->Spd_y;
    pEm->Pos_z += pEm->Spd_z;
}

/* re-aim at a new destination when reached or out of the zone (while damaged) */
/* Em3e_retarget */
void func_801015C0(ENEMY_WORK *pEm)
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
        if (func_800101C8(&pEm->Pos_x, Zone) || !func_800101C8(&pEm->Pos_x, Area)) {
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
