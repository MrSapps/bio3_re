#include "enemy/em32.h"
#include "joint.h"
#include "root_ck.h"
#include "sabun.h"
#include "sce_at.h"
#include "snd.h"
#include "sub.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
LONG Guide_check_(LONG X, LONG Z, LONG Floor) __asm__("Guide_check"); /* arg 1 long (canonical short); arg 2 long (canonical short); arg 3 long (canonical UC) */
void func_8002646C_(ENEMY_WORK *pEm, LONG A, LONG B, LONG C) __asm__("func_8002646C"); /* arg 2 long (canonical short); arg 3 long (canonical short); arg 4 long (canonical UC) */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector"); /* sub.c Rot_vector, unnarrowed dir */

/* RE2 EM_MAIN */
void func_801000C4(ENEMY_WORK *pEm)
{
    LONG Dummy[2];

    Em32_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if (!Water_ck(pEm->Pos_x, pEm->Pos_z))
        pEm->Be_flg &= ~0x20;
    else
        pEm->Be_flg |= 0x20;
    if (pEm->Routine_0 != 5)
        Em32_red(pEm);
}

/* hit flash: the parts lose red/green by Red and fade back */
void Em32_red(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG i;
    LONG Rg, B;
    ULONG Rgb;

    Rg = B = 0x80;
    i = 5;
    pP = pEm->pSin_parts_ptr;
    B -= EM32_W(pEm)->Red >> 2;
    Rg -= EM32_W(pEm)->Red;
    Rgb = (Rg << 16) | (Rg << 8) | B;
    do {
        pP->Poly_rgb = Rgb;
        i--;
        pP++;
    } while (i >= 0);
    if (pEm->Routine_1 != 3) {
        if (++EM32_W(pEm)->Red_cnt & 4) {
            EM32_W(pEm)->Red_cnt = 0;
            if (EM32_W(pEm)->Red)
                EM32_W(pEm)->Red--;
        }
    }
}

/* RE2 Em23_Init */
void func_801001F4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    COL_OBJ *pCol;
    PARTS_W *pP;

    EM_RTN(pEm) = 1;
    EM_MOVE(pEm) = 1;
    Joint_move(pEm, pKan, pSeq, 0x100);
    Kage_work_set(&pEm->Kage.pKw, 0, 0x1900320, 0, (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t);
    pEm->Unk134 = 1400;
    pEm->Unk136 = -250;
    pEm->Unk138 = 0x78;
    pEm->Unk139 = 0xBC;
    pEm->Unk13A = 400;
    pEm->Unk140 = 0;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr[3].Workm;
    pCol = &pEm->Col;
    pCol->W0 = pCol->D0 = 750;
    pCol->H = 600;
    pCol->Type = 2;
    pCol->pEm = pEm;
    pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    func_80033580(pCol, Pl_work.Col.Id);
    pP = pEm->pSin_parts_ptr;
    pEm->Unk120 = 0;
    pEm->Neck_no = 0;
    PW_FREE(pP)[2] = 0x40;
    PW_FREE(pP)[3] = 0x40;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[5] = 0x138;
    pEm->Life = 1;
    EM32_W(pEm)->Unk28 = 1;
    pEm->Max_life = 1;
    pEm->Timer0 = 0;
    pEm->UnkDA = 0;
    EM32_W(pEm)->Unk22 = 1;
    pEm->Be_flg |= 0xC00000;
    EM_RTN(pEm) = ((pEm->Type & 0xF) << 8) + 5;
    switch ((UCHAR)pEm->Type) {
    case 1:
        EM_MOVE(pEm) = 0;
        pEm->Timer0 = 2;
        pEm->Damage_cnt = 0x80;
        pEm->Be_flg |= 0x2000;
        break;
    case 2:
        EM_MOVE(pEm) = 6;
        pEm->Damage_cnt = 0x80;
        pEm->Be_flg |= 0x2400;
        pEm->Unk122 = pEm->Add_dir;
        break;
    case 3:
        EM32_W(pEm)->Unk22 = 0;
        EM_RTN(pEm) = 1;
        break;
    case 4:
        EM32_W(pEm)->Unk28 = 0;
        EM_RTN(pEm) = 1;
        break;
    default:
        EM_RTN(pEm) = 1;
        break;
    }
    EM32_W(pEm)->Type_bak = pEm->Type & 0xF;
    if (pEm->Type & 0x100)
        EM32_W(pEm)->Type_bak = 0;
    EM32_W(pEm)->nFloor_bak = pEm->nFloor;
    EM32_W(pEm)->Red = 0;
    EM32_W(pEm)->Red_cnt = 0;
    EM32_W(pEm)->Unk1B = 0;
    EM32_W(pEm)->Unk1C = 0;
    EM32_W(pEm)->Atck_cnt = 0;
    EM32_W(pEm)->Unk20 = 0;
    EM32_W(pEm)->Unk24 = 0;
    EM32_W(pEm)->Rev = 0;
    EM32_W(pEm)->Pos_x_bak = pEm->Pos_x;
    EM32_W(pEm)->Pos_y_bak = pEm->Pos_y;
    EM32_W(pEm)->Pos_z_bak = pEm->Pos_z;
    EM32_W(pEm)->Ground_bak = pEm->Unk122;
    EM32_W(pEm)->Cdir_y_bak = pEm->Cdir_y;
    EM32_W(pEm)->Timer0_bak = pEm->Timer0;
    EM32_W(pEm)->Be_flg_bak = pEm->Be_flg & ~0x10;
    G.pPldamage_Em[pEm->Id - 0x10] = Em32_Pldm;
}

/* RE2 Em23_Move */
void func_80100490(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    if (Guide_check_(pEm->Pos_x, pEm->Pos_z, pEm->nFloor) != 0xFF) {
        Root_ck(pEm, (VECTOR *)&pPl->Pos_x, 0, 0);
    } else {
        pEm->Dest_x = pPl->Pos_x;
        pEm->Dest_z = pPl->Pos_z;
    }
    Em32_mv_br[pEm->Routine_1](pEm);
    Em32_mv[pEm->Routine_1](pEm, pKan, pSeq);
    if (pEm->Col.Hit_flg & 0x40)
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
    else
        pEm->Be_flg |= 0x80000;
    if (pPl->Damage_cnt)
        pPl->Be_flg &= 0x7FFFFFFF;
}

/* mv_br[0]: wait, decide the next move */
void func_801005D0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;

    if (pEm->Col.W0 < 750)
        pEm->Col.D0 = pEm->Col.W0 += 50;
    if (EM32_W(pEm)->Unk22) {
        if (Em32_fall_ck(pEm))
            EM_RTN(pEm) = 0x101;
        if (func_80100F84(pEm))
            EM_RTN(pEm) = 0x201;
        if (pEm->Col.pSca_hit_data && (pEm->Col.pSca_hit_data->Id & 0xF) == 8 && (Rnd_l() & 1)) {
            EM32_W(pEm)->Atck_cnt = 50;
            EM32_W(pEm)->Rev = 1;
        }
        if (!EM32_W(pEm)->Red && !(pPl->Col.Hit_flg & 0x600)) {
            if ((LONG)pEm->Unk154 < 1000)
                Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
            if ((LONG)pEm->Unk154 > 1500 && (LONG)pEm->Unk154 < 2500 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100) &&
                pPl->Routine_0 == 1)
                EM_RTN(pEm) = 0x501;
        }
        if (EM32_W(pEm)->Unk20)
            EM32_W(pEm)->Unk20--;
        else if (EM32_W(pEm)->Rev)
            EM32_W(pEm)->Rev = 0;
        if (!EM32_W(pEm)->Unk20 && Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300)) {
            pEm->Move_cnt = 0;
            if (Rnd_l() & 1)
                EM_RTN(pEm) = 0x401;
            else
                EM_RTN(pEm) = 0x701;
        }
        if ((LONG)pEm->Unk154 < 2000)
            EM32_W(pEm)->Atck_cnt++;
        if (EM32_W(pEm)->Atck_cnt >= 31) {
            EM32_W(pEm)->Atck_cnt = 0;
            pEm->Move_cnt = 0;
            if (!(Rnd_l() & 3))
                EM32_W(pEm)->Rev = 1;
            if (Rnd_l() & 1)
                EM_RTN(pEm) = 0x401;
            else
                EM_RTN(pEm) = 0x701;
        }
    } else if ((LONG)pEm->Unk154 < 4500 && Em32_alone_ck(pEm, 0x501) && (pEm->Col.Hit_flg & 0x20)) {
        if (++EM32_W(pEm)->Unk24 >= 16 && pEm->nFloor == pPl->nFloor && pEm->Pos_y == pPl->Pos_y)
            EM_RTN(pEm) = 0x501;
    }
}

/* mv[0]: hop around the player */
void func_80100888(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        if (pEm->Water && pEm->Water < pEm->Pos_y) {
            EM_MOVE(pEm) = 0xF0001;
        } else if (Rnd_l() & 1) {
            EM_MOVE(pEm) = 0xF0003;
            if ((Rnd_l() & 1) && EM32_W(pEm)->Unk28)
                EM_MOVE(pEm) = 0xF0004;
        } else {
            EM_MOVE(pEm) = 0xF0002;
        }
        pEm->Move_cnt = Rnd_l() & 7;
        pEm->UnkE2 = 1;
        Spd_get(pEm, pKan, pSeq, 0);
    case 1:
        if (!EM32_W(pEm)->Rev)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x10);
        else
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, -0x10);
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->UnkE2) {
            pEm->UnkE2 = 0;
            pEm->Spd_x = 0;
        }
        if ((pEm->Move_cnt & 0xF) == 12 && pEm->Move_no != 1)
            Snd_se_enem(8, pEm);
        Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        func_80102800(pEm);
    }
}

/* at a ledge (shape 7): is there floor ahead and to both sides? 1: jump down */
LONG Em32_fall_ck(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    VECTOR Pos;

    if (pEm->Col.pSca_hit_data && (pEm->Col.pSca_hit_data->Id & 0xF) == 7) {
        if (func_800345C0(&Pos, pEm))
            return 0;
        Sv.vx = 1600;
        Sv.vz = 0;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Pos.vx = Sv.vx + pEm->Pos_x;
        Pos.vy = pEm->Pos_y - 1800;
        Pos.vz = Sv.vz + pEm->Pos_z;
        if (func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 0) != Pos.vy)
            return 0;
        Sv.vx = pEm->Col.W0 + 1600;
        Sv.vz = 0;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Pos.vx = Sv.vx + pEm->Pos_x;
        Pos.vy = pEm->Pos_y - 1800;
        Pos.vz = Sv.vz + pEm->Pos_z;
        if (func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 0) != Pos.vy)
            return 0;
        Sv.vx = 1000;
        Sv.vz = 0;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Pos.vx = Sv.vx + pEm->Pos_x;
        Pos.vy = pEm->Pos_y - 1800;
        Pos.vz = Sv.vz + pEm->Pos_z;
        if (func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 0) != Pos.vy)
            return 0;
        Sv.vx = 1767;
        Sv.vz = 0;
        Rot_vector_l(pEm->Cdir_y + 0x11D, &Sv, &Sv);
        Pos.vx = Sv.vx + pEm->Pos_x;
        Pos.vy = pEm->Pos_y - 1800;
        Pos.vz = Sv.vz + pEm->Pos_z;
        if (func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 0) == Pos.vy) {
            Rot_vector_l(pEm->Cdir_y - 0x11D, &Sv, &Sv);
            Pos.vx = Sv.vx + pEm->Pos_x;
            Pos.vy = pEm->Pos_y - 1800;
            Pos.vz = Sv.vz + pEm->Pos_z;
            return func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 0) == Pos.vy;
        }
    }
    return 0;
}

/* jump-down speed */
void func_80100CE8(ENEMY_WORK *pEm)
{
    SVECTOR Sv;

    Sv.vx = 1600;
    Sv.vz = 0;
    pEm->Spd_x = 0x40;
    pEm->Spd_z = Sv.vz / 25;
    pEm->Spd_y = 0;
    pEm->pSin_parts_ptr[3].Be_flg &= ~2;
}

void func_80100D4C(void) {
}

/* mv[1]: jump down one floor */
void func_80100D54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        func_80100CE8(pEm);
        EM_MOVE(pEm) = 0xF0005;
        pEm->Routine_2++;
        pEm->Be_flg |= 0x400;
        EM32_W(pEm)->Unk14 = pEm->nFloor;
        pEm->nFloor = -1;
        if (pEm->Water && pEm->Water < pEm->Pos_y)
            Snd_se_enem(6, pEm);
    case 1:
        if (!Joint_move(pEm, pKan, pSeq, 0x100))
            Add_speedXYZ(pEm, 0, 0);
        else
            pEm->Routine_2++;
        break;
    case 2:
        pEm->UnkE3 = 0;
        if (pEm->Water && pEm->Water < pEm->Pos_y) {
            EM_MOVE(pEm) = 0xF0001;
        } else if (Rnd_l() & 1) {
            EM_MOVE(pEm) = 0xF0003;
            if ((Rnd_l() & 1) && EM32_W(pEm)->Unk28)
                EM_MOVE(pEm) = 0xF0004;
        } else {
            EM_MOVE(pEm) = 0xF0002;
        }
        pEm->Be_flg &= ~0x400;
        pEm->Move_cnt = Rnd_l() & 7;
        pEm->UnkE2 = 1;
        Spd_get(pEm, pKan, pSeq, 0);
        func_80034124(&pEm->Col);
        EM_RTN(pEm) = 0x10001;
        pEm->nFloor = EM32_W(pEm)->Unk14 + 1;
        pEm->pSin_parts_ptr->Old_y += 1800;
        pEm->Unk122 = pEm->Pos_y = -(pEm->nFloor * 1800);
        Joint_move(pEm, pKan, pSeq, 0x100);
    }
}

/* under a ceiling edge (shape 8): is there a floor one storey up ahead? 1: jump up */
LONG func_80100F84(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    VECTOR Pos;

    if (pEm->Col.pSca_hit_data && (pEm->Col.pSca_hit_data->Id & 0xF) == 8) {
        Sv.vx = 1500;
        Sv.vz = 0;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Pos.vx = Sv.vx + pEm->Pos_x;
        Pos.vy = pEm->Pos_y + 1800;
        Pos.vz = Sv.vz + pEm->Pos_z;
        if (func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 4) < Pos.vy)
            return 0;
        Sv.vx = pEm->Col.W0 + 1500;
        Sv.vz = 0;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Pos.vx = Sv.vx + pEm->Pos_x;
        Pos.vy = pEm->Pos_y + 1800;
        Pos.vz = Sv.vz + pEm->Pos_z;
        if (func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 4) < Pos.vy)
            return 0;
        Sv.vx = 1000;
        Sv.vz = 0;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        Pos.vx = Sv.vx + pEm->Pos_x;
        Pos.vy = pEm->Pos_y + 1800;
        Pos.vz = Sv.vz + pEm->Pos_z;
        if (func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 4) >= Pos.vy)
            return !func_800345C0(&Pos, pEm);
    }
    return 0;
}

/* jump-up speed */
void func_80101144(ENEMY_WORK *pEm)
{
    SVECTOR Sv;

    Sv.vx = 1500;
    Sv.vz = 0;
    pEm->Spd_y = 90;
    pEm->pSin_parts_ptr[3].Be_flg &= ~2;
}

void func_80101178(void) {
}

/* mv[2]: jump up one floor */
void func_80101180(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    SHORT Spd_y;

    switch (pEm->Routine_2) {
    case 0:
        func_80101144(pEm);
        pEm->UnkE2 = 16;
        EM_MOVE(pEm) = 0xF0006;
        pEm->UnkE3 = 0;
        pEm->Routine_2++;
        pEm->Be_flg |= 0x400;
        EM32_W(pEm)->Unk14 = pEm->nFloor;
        pEm->nFloor = -1;
    case 1:
        Spd_y = pEm->Spd_y;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_y = Spd_y;
        if (!Joint_move(pEm, pKan, pSeq, 0x100)) {
            Add_speedXZ(pEm, 0);
            if (pEm->Water && Water_ck(pEm->Pos_x, pEm->Pos_z) < pEm->Pos_y)
                Snd_se_enem(0, pEm);
            if (pEm->Move_cnt == 5)
                pEm->Unk122 = pEm->Pos_y + 1800;
            if (pEm->Move_cnt >= 5 && pEm->Move_cnt < 25)
                pEm->Pos_y += pEm->Spd_y;
            pEm->UnkE2--;
        } else {
            EM_RTN(pEm) = 1;
            pEm->Be_flg &= ~0x400;
            func_80034124(&pEm->Col);
            pEm->nFloor = EM32_W(pEm)->Unk14 - 1;
            pEm->Pos_y = -(pEm->nFloor * 1800);
        }
        if (pEm->Water && pEm->Water < pEm->pSin_parts_ptr->Workm.t[1] && !pEm->UnkE3) {
            pEm->UnkE3 = 1;
            Sv.vx = pEm->Pos_x;
            Sv.vy = pEm->Water;
            Sv.vz = pEm->Pos_z;
            Esp_call(0x61C, (USHORT)(pEm->Cdir_y - 0x800) | 0x1C000000, &GsIDMATRIX, &Sv, NULL);
            Esp_call(0x61C, (USHORT)(pEm->Cdir_y - 0x92C) | 0x18000000, &GsIDMATRIX, &Sv, NULL);
            Esp_call(0x61C, (USHORT)(pEm->Cdir_y - 0x6D4) | 0x18000000, &GsIDMATRIX, &Sv, NULL);
            Snd_se_enem(5, pEm);
        }
    }
}

void func_80101438(void) {
}

/* mv[3]: grab the player */
void func_80101440(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Old_x, Old_z;
    SVECTOR Sv;
    VECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = 120;
        EM_MOVE(pEm) = 0x30009;
        pEm->Routine_2++;
        func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
        EM_RTN(pPl) = (Cdir_ck(pEm, pPl) << 24) | 5;
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pEm->Be_flg |= 0x8800;
        pPl->Be_flg |= 0x8800;
    case 1:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x400))
            pEm->Routine_2++;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->UnkCA) >> 2;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->UnkCA) >> 2;
        break;
    case 2:
        EM_MOVE(pEm) = 0xF000A;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pEm->Routine_2++;
    case 3:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            pEm->Routine_2++;
        if (pEm->Move_cnt == 22 || pEm->Move_cnt == 30 || pEm->Move_cnt == 60) {
            Sv.vx = 100;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x500, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[1].Workm, &Sv, NULL);
            Sv.vx = 0;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[1].Workm, &Sv, NULL);
        }
        if (Gacha_ch()) {
            if (Joint_move(pEm, pKan, pSeq, 0x100))
                pEm->Routine_2++;
            if (pEm->Move_cnt)
                Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, NULL);
        }
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->UnkCA) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->UnkCA) >> 3;
        break;
    case 4:
        EM_MOVE(pEm) = 0xF000B;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pEm->Routine_2++;
    case 5:
        if (pEm->Col.W0 < 750)
            pEm->Col.D0 = pEm->Col.W0 += 50;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_RTN(pEm) = 1;
            pEm->Be_flg |= 0x80000;
        }
    }
}

/* mv_br[4]: leap, turn by the motion's table and home in on the player */
void func_80101818(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl;
    SHORT Dx, Dz;

    pEm->Cdir_y += Em32_leap_dir[pEm->Move_cnt];
    pPl = &Pl_work;
    pEm->Cdir_y += (Rnd_l() & 0xF) - 7;
    if (pEm->Routine_2 == 1 && (ULONG)(pEm->Move_cnt - 8) < 12) {
        if (!EM32_W(pEm)->Rev) {
            if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x1A4)) {
                pEm->Move_cnt = 20;
                pEm->UnkCA = 7;
            }
        } else {
            Dx = pPl->Pos_x - pEm->Pos_x;
            Dz = pPl->Pos_z - pEm->Pos_z;
            if (!Goto00_ck(pEm, pEm->Pos_x - Dx, pEm->Pos_z - Dz, 0x1A4)) {
                pEm->Move_cnt = 20;
                pEm->UnkCA = 7;
            }
        }
    }
}

/* mv[4]: leap at the player */
void func_80101928(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX Unused;
    SVECTOR Unused2;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70007;
        pEm->Routine_2++;
        pEm->pSin_parts_ptr->UnkAE = 0;
    case 1:
        if (pEm->Col.W0 < 750)
            pEm->Col.D0 = pEm->Col.W0 += 50;
        pEm->Spd_x = Em32_leap_spd[pEm->Move_cnt];
        pEm->Spd_x += (Rnd_l() & 0x1F) - 15;
        pEm->Spd_z = 0;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM32_W(pEm)->Unk20 = Rnd_l() & 0x58;
            EM_RTN(pEm) = 1;
        }
        Add_speedXZ(pEm, 0);
        func_80102800(pEm);
        if ((!pEm->Water || pEm->Pos_y < pEm->Water) && (pEm->Move_cnt & 0xF) == 12)
            Snd_se_enem(8, pEm);
    }
}

/* mv_br[7]: turning leap */
void func_80101A7C(ENEMY_WORK *pEm)
{
    if ((ULONG)(pEm->Move_cnt - 21) < 32)
        pEm->Cdir_y -= 0x40;
}

/* mv[7]: turning leap */
void func_80101AAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX Unused;
    SVECTOR Unused2;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70008;
        pEm->Routine_2++;
        pEm->pSin_parts_ptr->UnkAE = 0;
    case 1:
        if (pEm->Col.W0 < 750)
            pEm->Col.D0 = pEm->Col.W0 += 50;
        pEm->Spd_x = Em32_leap2_spd[pEm->Move_cnt];
        pEm->Spd_x += (Rnd_l() & 0x1F) - 15;
        pEm->Spd_z = 0;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM32_W(pEm)->Unk20 = Rnd_l() & 0x58;
            EM_RTN(pEm) = 1;
        }
        Add_speedXZ(pEm, 0);
        func_80102800(pEm);
        if ((!pEm->Water || pEm->Pos_y < pEm->Water) && (pEm->Move_cnt & 0xF) == 12)
            Snd_se_enem(8, pEm);
    }
}

/* mv_br[5]: leap onto the player's back */
void func_80101C00(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Dir;

    if (pEm->Routine_2 == 1 && pEm->Move_cnt >= 6 && pEm->Move_cnt < 20 && (LONG)pEm->Unk154 < 1000 &&
        !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100) && pPl->Routine_0 == 1 && pPl->Routine_1 < 7) {
        if (EM32_W(pEm)->Unk1C < 3 && pEm->pSin_parts_ptr->Workm.t[1] < pPl->Pos_y - 1200) {
            func_8010413C(++EM32_W(pEm)->Unk1C);
            if (EM32_W(pEm)->Unk1C < 3) {
                pPl->pEnemy_ptr = pEm;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                EM_RTN(pPl) = 0x105;
            } else {
                pPl->pEnemy_ptr = pEm;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                EM_RTN(pPl) = 0x205;
            }
            pEm->pSin_parts_ptr->Old_y = 0;
            pEm->Pos_y = pEm->pSin_parts_ptr->Workm.t[1];
            pEm->Routine_2++;
            Snd_se_enem(1, pEm);
            if (!EM32_W(pEm)->Unk22) {
                EM32_W(pEm)->Unk22 = 1;
                pEm->Be_flg &= ~0x2000;
            }
        } else {
            Dir = pEm->Cdir_y;
            if (Rnd_l() & 1)
                pEm->Cdir_y = Dir + 400;
            else
                pEm->Cdir_y = Dir - 400;
        }
    }
    if ((pPl->Routine_0 == 3 || pPl->Routine_0 == 7) && EM32_W(pEm)->Unk1B == 1) {
        EM32_W(pEm)->Unk1B = 0;
        func_8010413C(--EM32_W(pEm)->Unk1C);
        if (!EM32_W(pEm)->Unk1C)
            pPl->Damage_cnt &= 0x7F;
        EM_RTN(pEm) = 0x601;
        pEm->Type &= ~0x2000;
    }
}

/* mv[5]: leap onto the player's back, bite, drop off */
void func_80101E30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl;
    VECTOR Pos;
    HIT_AREA Ha0;
    HIT_AREA Ha1;
    SVECTOR Box[2];
    ULONG Ret;
    SHORT Ground;

    Box[0].vx = 1200;
    Box[0].vy = 400;
    Box[0].vz = 800;
    Box[0].pad = 400;
    Ha0.pPos = (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t;
    Ha0.pBox = &Box[0].vx;
    Ha0.Dir = pEm->Cdir_y;
    Ha0.Id = 0x81;
    Box[1].vx = 1200;
    Box[1].vy = -400;
    Box[1].vz = 800;
    Box[1].pad = 400;
    Ha1.pPos = (VECTOR *)pEm->pSin_parts_ptr[3].Workm.t;
    Ha1.pBox = &Box[1].vx;
    Ha1.Dir = pEm->Cdir_y;
    Ha1.Id = 0x82;
    pPl = &Pl_work;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x7000C;
        pEm->Routine_2++;
        pEm->UnkE2 = 0;
        pEm->Type |= 0x2000;
        if (!EM32_W(pEm)->Unk22)
            pEm->Be_flg |= 0x2000;
    case 1:
        if (pEm->Move_cnt < 10)
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Ha0, pPl);
        func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Ha1, pPl);
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_RTN(pEm) = 1;
            pEm->Type &= ~0x2000;
        }
        if (!EM32_W(pEm)->Unk22 && pEm->Move_cnt >= 15 && func_8004D720(&pEm->Pos_x, pEm->Col.W0, 0x200, 0) <= 0) {
            EM32_W(pEm)->Unk22 = 1;
            pEm->Be_flg &= ~0x2000;
        }
        if (pEm->Move_cnt == 23)
            Snd_se_enem(0, pEm);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        EM_MOVE(pEm) = 0x7000D;
        EM32_W(pEm)->Unk1B = 1;
        pEm->Be_flg |= 0x400;
        pPl->Damage_cnt |= 0x80;
        EM32_W(pEm)->Unk16 = Direction_ck(pPl->Pos_x, pPl->Pos_z, pEm->Pos_x, pEm->Pos_z);
        EM32_W(pEm)->Unk16 = (EM32_W(pEm)->Unk16 - pPl->Cdir_y) & 0xFFF;
        Box[0].vx = (Rnd_l() & 8) - 4;
        Box[0].vy = 0;
        Box[0].vz = (Rnd_l() & 8) - 4;
        Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x8000000, &pEm->pSin_parts_ptr[1].Workm, &Box[0], NULL);
        Pad_vib_set0(2, 0);
        pEm->Col.D0 = pEm->Col.W0 = 100;
        pEm->Routine_2++;
        EM32_W(pEm)->Unk26 = pPl->Unk122;
    case 3:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
        Pos.vx = pPl->pSin_parts_ptr->Workm.t[0];
        Pos.vz = pPl->pSin_parts_ptr->Workm.t[2];
        Em32_on_back(pEm, 350, EM32_W(pEm)->Unk16 + pPl->Cdir_y, &Pos);
        break;
    case 4:
        EM_MOVE(pEm) = 0x7000E;
        pEm->UnkE2 = 0;
        EM32_W(pEm)->Red = 0;
        pEm->Routine_2++;
    case 5:
        if (Gacha_ch() && pEm->UnkE2 < 15)
            pEm->UnkE2++;
        if (pEm->Move_cnt == 0)
            Pad_vib_fade_set(7, 90, 110, 0);
        if (pEm->Move_cnt == 15)
            Pad_vib_fade_set(10, 110, 130, 0);
        if (pEm->Move_cnt == 30) {
            Pad_vib_fade_set(10, 130, 110, 0);
            Box[0].vx = (Rnd_l() & 7) - 4;
            Box[0].vy = 0;
            Box[0].vz = (Rnd_l() & 7) - 4;
            Esp_call(0, (USHORT)pEm->Cdir_y | 0x8000000, &pEm->pSin_parts_ptr[1].Workm, &Box[0], NULL);
        }
        if (pEm->Move_cnt == 45)
            Pad_vib_fade_set(7, 110, 90, 0);
        if (pEm->Move_cnt & 1) {
            EM32_W(pEm)->Red += 2;
            if (EM32_W(pEm)->Red > 60)
                EM32_W(pEm)->Red = 60;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (pEm->UnkE2 >= 15 && pPl->Routine_0 == 1 && pPl->Routine_1 < 7) {
                pPl->pEnemy_ptr = pEm;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                EM_RTN(pPl) = (Cdir_ck(pEm, pPl) << 24) | 0x40205;
            }
            if (EM32_W(pEm)->Red == 60 && pPl->Routine_0 == 1 && pPl->Routine_1 < 7) {
                pPl->pEnemy_ptr = pEm;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                EM_RTN(pPl) = (Cdir_ck(pEm, pPl) << 24) | 0x205;
            }
            Ret = Pl_life_down(1, 0);
            Pad_vib_set0(2, 0);
            Box[0].vx = (Rnd_l() & 7) - 4;
            Box[0].vy = 0;
            Box[0].vz = (Rnd_l() & 7) - 4;
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0xA000000, &pEm->pSin_parts_ptr->Workm, &Box[0], NULL);
            if (Ret & 2)
                EM_RTN(pPl) = 3;
        }
        pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
        Pos.vx = pPl->pSin_parts_ptr->Workm.t[0];
        Pos.vz = pPl->pSin_parts_ptr->Workm.t[2];
        Em32_on_back(pEm, 350, EM32_W(pEm)->Unk16 + pPl->Cdir_y, &Pos);
        pEm->Pos_y -= 5;
        if (pEm->Pos_y < pPl->Pos_y - 2000)
            pEm->Pos_y = pPl->Pos_y - 2000;
        break;
    case 6:
        pPl->Damage_cnt &= 0x7F;
        EM_MOVE(pEm) = 0x7000F;
        pEm->Routine_2++;
    case 7:
        if (pEm->Col.W0 < 750)
            pEm->Col.D0 = pEm->Col.W0 += 50;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_RTN(pEm) = 1;
            pEm->Type &= ~0x2000;
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Ground = func_8004D720(&Pos.vx, pEm->Col.W0, 0x8000, 4);
            pEm->Unk122 = Ground;
            pEm->Pos_y = Ground;
            pEm->nFloor = Ground / -1800;
        }
        break;
    }
}

/* stay on the player's back: L from the target point at angle Dir, follow the player's height */
void Em32_on_back(ENEMY_WORK *pEm, SHORT L, SHORT Dir, VECTOR *pPos)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = Dir;
    Sv.vz = 0;
    RotMatrix(&Sv, &M);
    Sv.vx = L;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, &Sv);
    pEm->Pos_x = Sv.vx + pPos->vx;
    pEm->Pos_z = Sv.vz + pPos->vz;
    pEm->Pos_y += pPl->Unk122 - EM32_W(pEm)->Unk26;
    EM32_W(pEm)->Unk26 = pPl->Unk122;
}

void func_8010268C(void) {
}

/* mv[6]: drop from the ceiling (Type 2) */
void func_80102694(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR Unused;
    SVECTOR Unused2;
    LONG Spd_y;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_enem(2, pEm);
        EM_MOVE(pEm) = 0xF000B;
        pEm->Spd_y = (pEm->Unk122 - pEm->Pos_y) / 8;
        pEm->Move_cnt = 33;
        Spd_y = pEm->Spd_y;
        pEm->Routine_2++;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_y = Spd_y;
    case 1:
        Spd_y = pEm->Spd_y;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Pos_y += Spd_y;
        pEm->Spd_y = Spd_y;
        if (pEm->Col.W0 < 750)
            pEm->Col.D0 = pEm->Col.W0 += 50;
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_RTN(pEm) = 1;
            pEm->Be_flg &= ~0x400;
            pEm->Pos_y = pEm->Unk122;
            pEm->Be_flg |= 0x80000;
        }
        pEm->Spd_x -= Rnd_l() & 7;
        Add_speedXZ(pEm, 0);
    }
}

/* body morph cycle (breathing), faster when near the player */
void func_80102800(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG Spd;

    pP = pEm->pSin_parts_ptr;
    Spd = 0x100;
    if (pEm->UnkDA < 3)
        Spd = 0x200;
    switch (pEm->UnkDA) {
    case 0:
        EM32_SABUN_P(pP)->Key[1].Rate = 0x1000;
        if ((LONG)pEm->Unk154 < 3000)
            pEm->UnkDA = 1;
        break;
    case 1:
        EM32_SABUN_P(pP)->Key[0].Rate += Spd;
        if (EM32_SABUN_P(pP)->Key[0].Rate > 0x1000)
            EM32_SABUN_P(pP)->Key[0].Rate = 0x1000;
        EM32_SABUN_P(pP)->Key[1].Rate -= Spd;
        if (EM32_SABUN_P(pP)->Key[1].Rate < 0) {
            EM32_SABUN_P(pP)->Key[1].Rate = 0;
            pEm->UnkDA = 2;
        }
        break;
    case 2:
        EM32_SABUN_P(pP)->Key[0].Rate -= Spd;
        if (EM32_SABUN_P(pP)->Key[0].Rate < 0) {
            EM32_SABUN_P(pP)->Key[0].Rate = 0;
            if ((LONG)pEm->Unk154 > 4000)
                pEm->UnkDA = 3;
        }
        break;
    case 3:
        EM32_SABUN_P(pP)->Key[0].Rate += Spd;
        if (EM32_SABUN_P(pP)->Key[0].Rate > 0x1000) {
            EM32_SABUN_P(pP)->Key[0].Rate = 0x1000;
            pEm->UnkDA = 4;
        }
        break;
    case 4:
        EM32_SABUN_P(pP)->Key[1].Rate += Spd;
        if (EM32_SABUN_P(pP)->Key[1].Rate > 0x1000)
            EM32_SABUN_P(pP)->Key[1].Rate = 0x1000;
        EM32_SABUN_P(pP)->Key[0].Rate -= Spd;
        if (EM32_SABUN_P(pP)->Key[0].Rate < 0) {
            EM32_SABUN_P(pP)->Key[0].Rate = 0;
            pEm->UnkDA = 0;
        }
        break;
    }
    func_8004AC54(pEm, pP);
}

/* where part Parts would be after this frame's motion: move the work by the
 * difference (Spd_flg 0) or take its length as the speed */
void func_80102A00(ENEMY_WORK *pEm, LONG Spd_flg, LONG Parts)
{
    MATRIX M;
    SVECTOR Unused;
    PARTS_W *pP;
    LONG i;
    LONG Dx, Dz;

    pP = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
    CompM((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &M);
    for (i = 1; Parts != 0; Parts--, i++) {
        pP = &pEm->pSin_parts_ptr[i];
        CompM(&M, (MATRIX *)pP->M, &M);
    }
    if (!Spd_flg) {
        pEm->Pos_x -= M.t[0] - pP->Workm.t[0];
        pEm->Pos_z -= M.t[2] - pP->Workm.t[2];
    } else {
        Dx = M.t[0] - pP->Workm.t[0];
        Dz = M.t[2] - pP->Workm.t[2];
        pEm->Spd_x = SquareRoot0(Dx * Dx + Dz * Dz);
        pEm->Spd_z = 0;
    }
}

/* Routine_0 5 */
void func_80102B14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em32_dm_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* hidden until there is room, then come out and fade in */
void func_80102B50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR Pos;
    PARTS_W *pP;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        pP = pEm->pSin_parts_ptr;
        i = 5;
        do {
            pP->Poly_rgb = 0;
            i--;
            pP++;
        } while (i >= 0);
        Pos.vx = pEm->Pos_x;
        Pos.vz = pEm->Pos_z;
        if (func_800345C0(&Pos, pEm))
            break;
        pEm->Be_flg &= ~0x10;
        pEm->Routine_2++;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (pEm->Timer0) {
                pEm->Timer0--;
            } else if (pEm->Type & 0xF) {
                pEm->Be_flg &= ~0x2000;
                func_80034124(&pEm->Col);
                EM_RTN(pEm) = 1;
                pEm->Damage_cnt = 0;
                pEm->Type &= 0xF0;
            }
        }
        Add_speedXZ(pEm, 0);
        for (pP = pEm->pSin_parts_ptr, i = 0; i < 6; i++, pP++) {
            if (pP->Poly_rgb != 0x808080)
                pP->Poly_rgb += 0x80808;
        }
    }
    pEm->UnkDA = 0;
    func_80102800(pEm);
}

/* hidden until there is room, then jump up out of the water */
void func_80102CE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR Pos;
    SVECTOR Sv;
    SHORT Spd_y;

    switch (pEm->Routine_2) {
    case 0:
        Pos.vx = pEm->Pos_x;
        Pos.vz = pEm->Pos_z;
        if (func_800345C0(&Pos, pEm))
            break;
        pEm->Be_flg &= ~0x10;
        pEm->Routine_2++;
    case 1:
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 16;
        pEm->Routine_2++;
        pEm->Be_flg |= 0x400;
        pEm->Spd_y = (pEm->Unk122 - pEm->Pos_y) / 20;
    case 2:
        Spd_y = pEm->Spd_y;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_y = Spd_y;
        if (!Joint_move(pEm, pKan, pSeq, 0x200)) {
            Add_speedXZ(pEm, 0);
            if (pEm->Move_cnt >= 5 && pEm->Move_cnt < 25)
                pEm->Pos_y += pEm->Spd_y;
            if (pEm->Move_cnt == 25 && !pEm->Water)
                Snd_se_enem(0, pEm);
            pEm->UnkE2--;
        } else if (pEm->Type & 0xF) {
            pEm->Be_flg &= ~0x400;
            pEm->Be_flg &= ~0x2000;
            func_80034124(&pEm->Col);
            EM_RTN(pEm) = 1;
            pEm->Damage_cnt = 0;
            pEm->Type &= 0xF0;
            pEm->Pos_y = pEm->Unk122;
        }
        if (pEm->Water && pEm->Water < pEm->pSin_parts_ptr->Workm.t[1] && !pEm->UnkE3) {
            pEm->UnkE3 = 1;
            Sv.vx = pEm->Pos_x;
            Sv.vy = pEm->Water;
            Sv.vz = pEm->Pos_z;
            Esp_call(0x61C, (USHORT)(pEm->Cdir_y - 0x800) | 0x1C000000, &GsIDMATRIX, &Sv, NULL);
            Esp_call(0x61C, (USHORT)(pEm->Cdir_y - 0x92C) | 0x18000000, &GsIDMATRIX, &Sv, NULL);
            Esp_call(0x61C, (USHORT)(pEm->Cdir_y - 0x6D4) | 0x18000000, &GsIDMATRIX, &Sv, NULL);
            Snd_se_enem(5, pEm);
        }
    }
    pEm->UnkDA = 0;
    func_80102800(pEm);
}

/* RE2 Em23_Damage: every hit kills; knocked back, or burst and shrink */
void func_80102FD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl;
    PARTS_W *pP;
    SVECTOR Sv;
    VECTOR Unused;
    LONG i;
    LONG Spd_y;
    LONG Wep;

    pEm->Type &= ~0x2000;
    pPl = &Pl_work;
    if (pEm->Routine_2 == 0) {
        if (pEm->Pos_y != pEm->Unk122) {
            pEm->Life = -1;
            pEm->Routine_2 = 7;
            pEm->Damage_cnt |= 0x80;
        }
        if (pEm->Routine_2 == 0) {
            pEm->Life = -1;
            pEm->Damage_cnt |= 0x80;
            Wep = pEm->Unk12C;
            if ((UCHAR)Wep < 4 || (ULONG)(Wep - 12) < 4 || (UCHAR)Wep >= 17)
                pEm->Routine_2 = 2;
            if (Rnd_l() & 1)
                pEm->Routine_2 = 2;
        }
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = (Rnd_l() & 1) + 0xF0012;
        pEm->Timer0 = (pPl->Cdir_y + 0x800) & 0xFFF;
        pEm->Routine_2++;
    case 1:
        Goto01(pEm, pEm->Timer0, 0x100);
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        pEm->Timer0 = 0x1000;
        pEm->UnkE3 = 0;
        pEm->Be_flg |= 0x100000;
        if (EM32_W(pEm)->Unk1B == 1) {
            EM32_W(pEm)->Unk1B = 0;
            func_8010413C(--EM32_W(pEm)->Unk1C);
            if (!EM32_W(pEm)->Unk1C)
                pPl->Damage_cnt &= 0x7F;
        }
        EM_MOVE(pEm) = (Rnd_l() & 1) + 0x70014;
        Snd_se_enem(3, pEm);
        pEm->Routine_2++;
        for (i = 0, pP = pEm->pSin_parts_ptr; i < 6; i++, pP++)
            pP->UnkA2 |= 0x400;
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(0x7000700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr->Workm, &Sv, NULL);
        Sv.vx = (Rnd_l() & 0xF) - 8;
        Sv.vy = 0;
        Sv.vz = (Rnd_l() & 0xF) - 8;
        Esp_call(0x7000000, (USHORT)pEm->Cdir_y | 0xC000000, &pEm->pSin_parts_ptr->Workm, &Sv, NULL);
    case 3:
        if (pEm->Timer0)
            pEm->Size = pEm->Timer0 -= 0x100;
        else
            pEm->Routine_2++;
        if (pEm->Timer0 > 0x800 && (pEm->Timer0 & 0x100)) {
            Sv.vx = (Rnd_l() & 7) - 4;
            Sv.vy = 0;
            Sv.vz = (Rnd_l() & 7) - 4;
            Esp_call(0x7000000, (USHORT)pEm->Cdir_y | 0xA000000, &pEm->pSin_parts_ptr->Workm, &Sv, NULL);
        }
        if (!pEm->UnkE3)
            pEm->UnkE3 = Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 4:
        pEm->Timer0 = 0x1000;
        pEm->Be_flg |= 0x200000;
        pEm->Routine_2++;
    case 5:
        if (pEm->Timer0) {
            for (i = 0, pP = pEm->pSin_parts_ptr; i < 6; i++, pP++) {
                pP->Be_flg |= 4;
                pP->Size_x = pEm->Timer0;
                pP->Size_y = 0;
                pP->Size_z = pEm->Timer0;
            }
            pEm->Timer0 -= 0x100;
        } else {
            for (i = 0, pP = pEm->pSin_parts_ptr; i < 6; i++, pP++)
                pP->UnkA2 |= 0x400;
            pEm->Routine_2++;
        }
        if (!pEm->UnkE3)
            pEm->UnkE3 = Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 6:
        EM_RTN(pEm) = 7;
        break;
    case 7:
        EM_MOVE(pEm) = 0xF000B;
        pEm->Spd_y = (pEm->Unk122 - pEm->Pos_y) / 8;
        pEm->Move_cnt = 33;
        Spd_y = pEm->Spd_y;
        pEm->Routine_2++;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_y = Spd_y;
    case 8:
        Spd_y = pEm->Spd_y;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Pos_y += Spd_y;
        pEm->Spd_y = Spd_y;
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            for (i = 0, pP = pEm->pSin_parts_ptr; i < 6; i++, pP++)
                pP->UnkA2 |= 0x400;
            pEm->Pos_y = pEm->Unk122;
            pEm->Routine_2 = 0;
            pEm->Be_flg = (pEm->Be_flg & ~0x400) | 0x80000;
            break;
        }
        pEm->Spd_x -= Rnd_l() & 7;
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em23_Dead: respawn at the start position, or fade the shadow out */
void func_8010356C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    KAGE_WORK *pK;
    SVECTOR Unused;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        if (!(EM32_W(pEm)->Type_bak & 0xF)) {
            pEm->Routine_2 = 3;
            Em_dead_flg_on_cand(pEm);
            pEm->Col.Id = 0;
            pEm->Type |= 0x4000;
            break;
        }
        pEm->Pos_x = pEm->Old_x = EM32_W(pEm)->Pos_x_bak;
        pEm->Pos_y = pEm->Old_y = EM32_W(pEm)->Pos_y_bak;
        pEm->Pos_z = pEm->Old_z = EM32_W(pEm)->Pos_z_bak;
        pEm->Routine_2++;
    case 1:
        pEm->Life = 1;
        pEm->Type = EM32_W(pEm)->Type_bak;
        pEm->Unk122 = EM32_W(pEm)->Ground_bak;
        pEm->Cdir_y = EM32_W(pEm)->Cdir_y_bak;
        pEm->Timer0 = EM32_W(pEm)->Timer0_bak;
        pEm->nFloor = EM32_W(pEm)->nFloor_bak;
        pEm->Be_flg = EM32_W(pEm)->Be_flg_bak | 0x10;
        func_80034124(&pEm->Col);
        pEm->Size = 0x1000;
        pEm->Be_flg &= 0xFFCFFFFF;
        for (i = 0, pP = pEm->pSin_parts_ptr; i < 6; i++, pP++) {
            pP->UnkA2 &= ~0x400;
            pP->Poly_rgb = 0x808080;
        }
        EM32_W(pEm)->Unk22 = 1;
        pEm->UnkDA = 0;
        pEm->Damage_cnt = 0;
        EM_RTN(pEm) = ((pEm->Type & 0xF) << 8) + 5;
        switch (pEm->Type & 0xF) {
        case 1:
            EM_MOVE(pEm) = 0;
            pEm->Damage_cnt = 0x80;
            pEm->Be_flg |= 0x2000;
            break;
        case 2:
            EM_MOVE(pEm) = 6;
            pEm->Damage_cnt = 0x80;
            pEm->Be_flg |= 0x2400;
            pEm->Unk122 = pEm->Add_dir;
            break;
        case 0x103:
            EM32_W(pEm)->Unk22 = 0;
            EM_MOVE(pEm) = 1;
            EM_RTN(pEm) = 1;
            break;
        default:
            EM_MOVE(pEm) = 1;
            EM_RTN(pEm) = 1;
            break;
        }
        break;
    case 2:
    case 5:
        break;
    case 3:
        pK = pEm->Kage.pKw;
        PRIM_RGBC(pK->Prim[0]) = (PRIM_RGBC(pK->Prim[0]) & 0xFF000000) | 0xBF3030;
        PRIM_RGBC(pK->Prim[1]) = (PRIM_RGBC(pK->Prim[1]) & 0xFF000000) | 0xBF3030;
        pEm->Timer0 = 60;
        pEm->Routine_2++;
    case 4:
        pK = pEm->Kage.pKw;
        if (pEm->Timer0) {
            pEm->Timer0--;
            pK->Half_x += 8;
            pK->Half_z += 8;
        }
        break;
    }
    EM32_W(pEm)->Red = 0;
    EM32_W(pEm)->Red_cnt = 0;
    EM32_W(pEm)->Unk1B = 0;
    EM32_W(pEm)->Atck_cnt = 0;
    EM32_W(pEm)->Unk20 = 0;
}

/* 1 if no other em32 is in the move Rtn (Routine_0/1) */
LONG Em32_alone_ck(ENEMY_WORK *pEm, USHORT Rtn)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm2;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm2 = *ppEm;
        if (pEm2 != pEm && pEm2->Id == 0x32 && (pEm2->Be_flg & 1) && EM_RTN01(pEm2) == Rtn)
            return 0;
    }
    return 1;
}

/* RE2 Em123_Pldm: the player while held (G.pPldamage_Em) */
void Em32_Pldm(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEm = pPl->pEnemy_ptr;

    if (pEm->Routine_0 != 1) {
        EM_RTN(pPl) = 1;
        pPl->Be_flg = (pPl->Be_flg | 0x80000) & 0xFFFF77FF;
        pEm->Be_flg &= 0xFFFF77FF;
        pPl->Unk120 &= 0xED;
        pPl->Damage_cnt &= 0x7F;
        Player_move(pPl);
    } else {
        Em32_pldm_tbl[pPl->Routine_1](pPl, pKan, pSeq);
    }
}

/* pldm[0]: the player shakes off the em32 on the back (or it bites).
   The motion is picked per side (Routine_3) with the same values for both sides; the
   cross-jumped if/else is what keeps those stores ahead of the following code. */
void func_80103930(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEm = pPl->pEnemy_ptr;
    SHORT Dir = (pPl->Routine_3 == 0) << 11;
    SHORT Old_x;
    SHORT Old_z;

    switch (pPl->Routine_2) {
    case 0:
        if (pPl->Routine_3)
            EM_MOVE(pPl) = 0x30000;
        else
            EM_MOVE(pPl) = 0x30000;
        func_8002646C_(pPl, 300, 60, pPl->Col.Unk39);
        pPl->Routine_2++;
    case 1:
        Old_x = pPl->Pos_x;
        Old_z = pPl->Pos_z;
        Goto01(pPl, pEm->Cdir_y + Dir, 300);
        pPl->Cdir_y += Dir;
        Null_pos_set(pPl, pKan, pSeq, 0);
        pPl->Cdir_y -= Dir;
        if (Joint_move2(pPl, pKan, pSeq, 0x400))
            pPl->Routine_2++;
        pPl->Pos_x -= ((pPl->Pos_x - Old_x) * pPl->UnkCA) >> 2;
        pPl->Pos_z -= ((pPl->Pos_z - Old_z) * pPl->UnkCA) >> 2;
        break;
    case 2:
        if (pPl->Routine_3)
            EM_MOVE(pPl) = 0x70001;
        else
            EM_MOVE(pPl) = 0x70001;
        Pad_vib_set0(3, 10);
        Pad_vib_set1(3, 200, 14);
        Pad_vib_set0(7, 15);
        Pad_vib_set1(3, 250, 19);
        Pad_vib_set0(3, 20);
        Pad_vib_set1(3, 210, 24);
        Pad_vib_set0(3, 25);
        Pad_vib_set1(3, 150, 29);
        pPl->Routine_2++;
    case 3:
        Old_x = pPl->Pos_x;
        Old_z = pPl->Pos_z;
        pPl->Cdir_y += Dir;
        Null_pos_set(pPl, pKan, pSeq, 0);
        pPl->Cdir_y -= Dir;
        if (Joint_move2(pPl, pKan, pSeq, 0x200)) {
            pPl->Routine_2++;
            Pad_vib_set0(3, 10);
            Pad_vib_set1(3, 200, 14);
            Pad_vib_set0(7, 15);
            Pad_vib_set1(3, 150, 19);
            Pad_vib_set0(3, 20);
        }
        if (Gacha_ch() && Joint_move2(pPl, pKan, pSeq, 0x200)) {
            pPl->Routine_2++;
            Pad_vib_set0(3, 10);
            Pad_vib_set1(3, 200, 14);
            Pad_vib_set0(7, 15);
            Pad_vib_set1(3, 150, 19);
            Pad_vib_set0(3, 20);
        }
        pPl->Pos_x -= ((pPl->Pos_x - Old_x) * pPl->UnkCA) >> 3;
        pPl->Pos_z -= ((pPl->Pos_z - Old_z) * pPl->UnkCA) >> 3;
        break;
    case 4:
        if (pPl->Routine_3) {
            EM_MOVE(pPl) = 0x70002;
            pPl->Be_flg |= 0x80000;
        } else {
            EM_MOVE(pPl) = 0x70002;
            pPl->Be_flg |= 0x80000;
        }
        pPl->Routine_2++;
    case 5:
        pPl->Cdir_y += Dir;
        Null_pos_set(pPl, pKan, pSeq, 0);
        pPl->Cdir_y -= Dir;
        if (Joint_move2(pPl, pKan, pSeq, 0x200)) {
            EM_RTN(pPl) = 1;
            pPl->Unk120 &= 0xED;
            pPl->Be_flg &= 0xFFFF77FF;
            pEm->Be_flg &= 0xFFFF77FF;
            pPl->Damage_cnt &= 0x7F;
        }
    }
}

/* pldm[1]: the player knocks the em32 off */
void func_80103D60(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    switch (pPl->Routine_2) {
    case 0:
        EM_MOVE(pPl) = 0x70006;
        Snd_se_stad(0x10000, (VECTOR *)&pPl->Pos_x, 0, 0);
        pPl->Routine_2++;
        pPl->Unk120 |= 0x12;
    case 1:
        Spd_get(pPl, pKan, pSeq, 0);
        if (Joint_move2(pPl, pKan, pSeq, 0x200)) {
            EM_RTN(pPl) = 1;
            pPl->Unk120 &= 0xED;
        }
        Add_speedXZ(pPl, 0);
    }
}

/* pldm[2]: three em32 on the back, the player goes down */
void func_80103E3C(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    switch (pPl->Routine_2) {
    case 0:
        EM_MOVE(pPl) = 0x70006;
        pPl->Routine_2++;
        pPl->Unk120 |= 0x12;
    case 1:
        Spd_get(pPl, pKan, pSeq, 0);
        if (Joint_move2(pPl, pKan, pSeq, 0x200))
            pPl->Routine_2++;
        Add_speedXZ(pPl, 0);
        break;
    case 2:
        EM_MOVE(pPl) = 0x70007;
        pPl->Routine_3 = 0;
        pPl->Routine_2++;
        pPl->Unk120 |= 0x12;
    case 3:
        Spd_get(pPl, pKan, pSeq, 0);
        if (pPl->Move_cnt == 0) {
            pPl->Spd_x = 0;
            pPl->Spd_z = 9;
        }
        if (Joint_move2(pPl, pKan, pSeq, 0x200) || ((G.Unk20FC & 0xF00F) && Joint_move2(pPl, pKan, pSeq, 0x200))) {
            if (++pPl->Routine_3 >= 3)
                pPl->Routine_2++;
            if (Pl_life_down(4, 0) & 2)
                EM_RTN(pPl) = 3;
        }
        Add_speedXZ(pPl, 0);
        break;
    case 4:
        EM_MOVE(pPl) = 0x70008;
        pPl->Routine_2++;
    case 5:
        Spd_get(pPl, pKan, pSeq, 0);
        if (pPl->Move_cnt == 0) {
            pPl->Spd_x = -5;
            pPl->Spd_z = -6;
        }
        if (Joint_move2(pPl, pKan, pSeq, 0x200)) {
            EM_RTN(pPl) = 1;
            pPl->Unk120 &= 0xED;
            pPl->Damage_cnt &= 0x7F;
        }
        Add_speedXZ(pPl, 0);
        if (pPl->Move_cnt == 12) {
            func_801040B0();
            func_8010413C(0);
        }
    }
}

/* knock every em32 off the player's back */
void func_801040B0(void)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        if (pEm->Id == 0x32 && (pEm->Be_flg & 1) && EM32_W(pEm)->Unk1B == 1) {
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Routine_3 = 0;
            EM32_W(pEm)->Unk1B = 0;
        }
    }
}

/* tell every em32 how many are on the player's back */
void func_8010413C(SHORT Num)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        if (pEm->Id == 0x32 && (pEm->Be_flg & 1))
            EM32_W(pEm)->Unk1C = Num;
    }
}
