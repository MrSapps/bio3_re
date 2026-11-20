#include "common.h"
#include "ovl/em39.h"

SHORT func_801012A4(ENEMY_WORK *pEm, PARTS_W *pP);
void func_8010133C(ENEMY_WORK *pEm, PARTS_W *pP, LONG Size);

INCLUDE_ASM("asm/nonmatchings/ovl/em39/em39", func_80100004);

INCLUDE_ASM("asm/nonmatchings/ovl/em39/em39", func_801003B0);

/* RE2 Em39_move */
void func_80100588(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Em39_move_type[(UCHAR)pEm->Type](pEm, Kan, Seq);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em39/em39", func_801005C4);

INCLUDE_ASM("asm/nonmatchings/ovl/em39/em39", func_80100648);

/* wait for the player in reach: start the attack (Routine_1 = 1) */
void func_801006CC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist;
    SHORT Turn;

    Dist = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Turn = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x400, 0x264);
    if (Dist < 7000 && !Turn && !EM39_W(pEm)->Wait && !pEm->Spd_z)
        EM_RTN(pEm) = 0x101;
}

/* idle sway */
void func_8010079C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x3F0000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd() & 0x3F) + 60;
        pEm->Spd_x = 0x400;
        pEm->Spd_y = 0;
    case 1:
        Joint_move(pEm, Kan, Seq, 0x40);
        if (--pEm->Timer0 == -1) {
            EM_MOVE(pEm) = 0x3F0000 + ((Rnd() & 0xF) << 8) + Em39_move_rnd[(Rnd() >> (Rnd() & 3)) & 7];
            pEm->Timer0 = (Rnd() & 0x3F) + 60;
        }
        break;
    }
    if (pEm->Cdir_z >= -0x7F) {
        pEm->Cdir_z -= 8;
        if (pEm->Cdir_z < -0x80)
            pEm->Cdir_z = -0x80;
    }
}

void func_801008E0(void) {
}

/* attack: lash at the player */
void func_801008E8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG Hit;
    LONG Dir_old;
    LONG Dir;
    LONG d;
    SVECTOR Sv;
    HIT_AREA Area;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 1);
        EM_MOVE(pEm) = 0x1F0002;
        if (pEm->UnkE2 == 1)
            pEm->Move_no++;
        pEm->Spd_x = 0x1000;
        pEm->Routine_2 = 1;
        pEm->Spd_y = 0x50;
        pEm->UnkE3 = 0;
    case 1:
        if (Joint_move(pEm, Kan, Seq, 0x80)) {
            EM_RTN(pEm) = 1;
            EM39_W(pEm)->Wait = (Rnd() & 0x3F) + 90;
            if (pPl->Life < 100)
                EM39_W(pEm)->Wait = (Rnd() & 0x3F) + 120;
            if (pPl->Life < 20)
                EM39_W(pEm)->Wait = (Rnd() & 0x7F) + 150;
        }
        if (pPl->Damage_cnt)
            pEm->UnkE3 = -0x80;
        if ((*pEm->pNow_seq & 0x200) && !pPl->Damage_cnt) {
            pP = pEm->pSin_parts_ptr;
            pEm->UnkE3 = -0x80;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 1200;
            Hit = func_80044420(&pP[2].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Sv, pPl);
            Hit |= func_80044420(&pP[5].Workm, &Sv, pPl);
            if (Hit) {
                Dir_old = pEm->Cdir_y;
                d = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
                pEm->Cdir_y = d - 0x400;
                if (pEm->Move_no == 3)
                    pEm->Cdir_y = d + 0x400;
                Dir = pEm->Cdir_y;
                Hit = Cdir_ck_l(pEm, pPl);
                pEm->Cdir_y = Dir_old;
                EM_RTN(pPl) = ((Hit + 4) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                pPl->pEnemy_ptr = pEm;
                Dir -= 0x800;
                if (Hit)
                    Dir += 0x800;
                Goto01(pPl, Dir, 0x200);
                Snd_se_enem(1, pEm);
                Pl_life_down(10, 1);
                Pad_vib_set0(3, 0);
                Pad_vib_set1(10, 250, 0);
                pEm->Spd_x = 0;
                pEm->Spd_y = 0;
            }
        }
        break;
    }
    if (!EM39_W(pEm)->pParent->Pos_y) {
        if (pEm->UnkE2 == 1) {
            if (pEm->Move_cnt >= 12 && pEm->Move_cnt < 18) {
                Area.pPos = (VECTOR *)&pEm->Pos_x;
                Area.pBox = Em39_box1;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x43;
                func_800445C8(pEm->pSin_parts_ptr->Workm.t, &pEm->Pos_x, &Area, &Pl_work);
            }
            if (pEm->Move_cnt >= 17 && pEm->Move_cnt < 21) {
                Area.pPos = (VECTOR *)&pEm->Pos_x;
                Area.pBox = Em39_box2;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x43;
                func_800445C8(pEm->pSin_parts_ptr->Workm.t, &pEm->Pos_x, &Area, &Pl_work);
            }
            if (pEm->Move_cnt >= 20 && pEm->Move_cnt < 24) {
                Area.pPos = (VECTOR *)&pEm->Pos_x;
                Area.pBox = Em39_box0;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x43;
                func_800445C8(pEm->pSin_parts_ptr->Workm.t, &pEm->Pos_x, &Area, &Pl_work);
            }
        } else {
            if (pEm->Move_cnt >= 12 && pEm->Move_cnt < 18) {
                Area.pPos = (VECTOR *)&pEm->Pos_x;
                Area.pBox = Em39_box0;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x43;
                func_800445C8(pEm->pSin_parts_ptr->Workm.t, &pEm->Pos_x, &Area, &Pl_work);
            }
            if (pEm->Move_cnt >= 17 && pEm->Move_cnt < 21) {
                Area.pPos = (VECTOR *)&pEm->Pos_x;
                Area.pBox = Em39_box2;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x43;
                func_800445C8(pEm->pSin_parts_ptr->Workm.t, &pEm->Pos_x, &Area, &Pl_work);
            }
            if (pEm->Move_cnt >= 20 && pEm->Move_cnt < 24) {
                Area.pPos = (VECTOR *)&pEm->Pos_x;
                Area.pBox = Em39_box1;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x43;
                func_800445C8(pEm->pSin_parts_ptr->Workm.t, &pEm->Pos_x, &Area, &Pl_work);
            }
        }
        if (pEm->Cdir_z < 0) {
            pEm->Cdir_z += 16;
            if (pEm->Cdir_z > 0)
                pEm->Cdir_z = 0;
        }
    }
}

void func_80100DB8(void) {
}

void func_80100DC0(void) {
}

void func_80100DC8(void) {
}

/* grow back out of the parent */
void func_80100DD0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    PARTS_W *pP;
    LONG Size;
    SVECTOR Sv;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x3F0001;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->UnkDA = 0;
        EM39_W(pEm)->Flg |= 1;
    case 1:
        Joint_move(pEm, Kan, Seq, 0x40);
        pP = pEm->pSin_parts_ptr;
        Size = pP->Size_y;
        Size += 0x80;
        if (Size > 0x1000) {
            Size = 0x1000;
            pEm->UnkDA = 1;
        }
        pP[0].Size_y = Size;
        func_8010133C(pEm, &pP[0], Size);
        pP[1].Size_y = Size;
        func_8010133C(pEm, &pP[1], Size);
        pP[2].Size_y = Size;
        func_8010133C(pEm, &pP[2], Size);
        pP[3].Size_y = Size;
        func_8010133C(pEm, &pP[3], Size);
        pP[4].Size_y = Size;
        func_8010133C(pEm, &pP[4], Size);
        pP[5].Size_y = Size;
        func_8010133C(pEm, &pP[5], Size);
        if (pEm->UnkDA) {
            EM_RTN(pEm) = 1;
            EM39_W(pEm)->Wait = (Rnd() & 0x7F) + 30;
            pP[4].Be_flg &= ~4;
            pP[5].Be_flg &= ~4;
            EM39_W(pEm)->Flg &= ~1;
        }
        pEm->Timer0++;
        if (!(pEm->Move_cnt & 3)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0x100 - Rnd() * 2;
            Sv.vy = 0x100 - Rnd() * 2;
            Sv.vz = 0x100 - Rnd() * 2;
            func_8001B484(0x1900, ((Rnd() * 2 + 0x1800) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
            func_8001B484(0x1800, 0x10000000, &pP->Workm, &Sv, NULL);
            Sv.vx = 0x100 - Rnd() * 2;
            Sv.vy = 0x100 - Rnd() * 2;
            Sv.vz = 0x100 - Rnd() * 2;
            func_8001B484((((Rnd() & 1) + 1) << 24) | 0x1900, ((Rnd() * 2 + 0x1800) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
            func_8001B484((((Rnd() & 1) + 1) << 24) | 0x1800, 0x10000000, &pP->Workm, &Sv, NULL);
        }
        break;
    }
}

void func_80101094(void) {
}

void func_8010109C(void) {
}

void func_801010A4(void) {
}

void func_801010AC(void) {
}

void func_801010B4(void) {
}

void func_801010BC(void) {
}

void func_801010C4(void) {
}

void func_801010CC(void) {
}

void func_801010D4(void) {
}

void func_801010DC(void) {
}

void func_801010E4(void) {
}

void func_801010EC(void) {
}

void func_801010F4(void) {
}

void func_801010FC(void) {
}

void func_80101104(void)
{
    SVECTOR Unused;
}

void func_80101114(void) {
}

void func_8010111C(void) {
}

void func_80101124(void) {
}

void func_8010112C(void) {
}

void func_80101134(void) {
}

/* splash effects while moving (declared with a result it never returns) */
LONG func_8010113C(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR Sv;

    if ((pEm->Move_cnt & 7) == 1) {
        Sv.vx = 11;
        Sv.vy = (Rnd() & 0xF) + 0x10C;
        Sv.vz = 50;
        pP = pEm->pSin_parts_ptr;
        func_8001B484(0x4001700, ((Rnd() * 4 + 0x1400) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
        func_8001B484(0x4000500, 0xA000000, &pP->Workm, &Sv, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && (Rnd() & 1)) {
        Sv.vx = 11;
        Sv.vy = 300;
        Sv.vz = 50;
        pP = pEm->pSin_parts_ptr;
        func_8001B484(0x2001700, ((Rnd() * 4 + 0x1400) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
        func_8001B484(0x2000500, 0xA000000, &pP->Workm, &Sv, NULL);
    }
}

/* direction of part pP's x axis */
SHORT func_801012A4(ENEMY_WORK *pEm, PARTS_W *pP)
{
    VECTOR Unused;
    VECTOR V;
    LONG x;
    LONG z;
    SHORT Dir;

    V.vx = -4000;
    V.vy = 4000;
    V.vz = 0;
    ApplyMatrixLV(&pP->Workm, &V, &V);
    x = V.vx;
    z = V.vz;
    if (x) {
        Dir = catan((z << 12) / x);
        if (x < 0)
            Dir = 0x800 - Dir;
        else
            Dir = 0x1000 - Dir;
    } else {
        Dir = 0x400;
        if (z > 0)
            Dir = 0xC00;
    }
    return Dir;
}

/* part position from the kan data, scaled by Size (0x1000 = 1) */
void func_8010133C(ENEMY_WORK *pEm, PARTS_W *pP, LONG Size)
{
    SHORT *pPos;

    pPos = (SHORT *)(pEm->pKan_t_ptr + 2);
    pPos += pP->Unk84 * 3;
    pP->Pos_x = pPos[0] * Size >> 12;
    pP->Pos_y = pPos[1] * Size >> 12;
    pP->Pos_z = pPos[2] * Size >> 12;
}
