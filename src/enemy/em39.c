#include "enemy/em39.h"
#include "joint.h"
#include "sca.h"
#include "snd.h"

/* Capcom's declarations in this module differ from the exe's: Dir_pos_ck takes int directions
   (no narrowing at the call) and Cdir_ck returns int (no andi after the call) */
LONG Dir_pos_ck_l(LONG *pPos, LONG *pTg, LONG Hed_dir, LONG Hani_dir) __asm__("Dir_pos_ck");
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");

/* per-frame entry (RE2 EM_MAIN): follow the parent part, then grow/shrink with the ground under parts 3..5 */
void Em39_main(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    USHORT n;
    LONG Under;
    SVECTOR Sv;
    MATRIX Unused_m;
    VECTOR Unused_v;

    if (pEm->Routine_0) {
        pP = EM39_W(pEm)->pParent->pSin_parts_ptr;
        if (pEm->Type & 1) {
            Sv.vx = -0x1B8;
            Sv.vy = -0x96;
            Sv.vz = 0x96;
            pEm->Cdir_y = Em39_part_dir(EM39_W(pEm)->pParent, &pP[12]) + (SHORT)(EM39_W(pEm)->Dir + 0x200);
        } else {
            Sv.vx = -0x1B8;
            Sv.vy = -0x96;
            Sv.vz = -0x96;
            pEm->Cdir_y = Em39_part_dir(EM39_W(pEm)->pParent, &pP[12]) + (SHORT)(EM39_W(pEm)->Dir - 0x100);
        }
        EM39_W(pEm)->Dir += Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 8);
        if (EM39_W(pEm)->Dir > 0x100)
            EM39_W(pEm)->Dir = 0x100;
        if (EM39_W(pEm)->Dir < -0x100)
            EM39_W(pEm)->Dir = -0x100;
        ApplyMatrixSV(&pP->Workm, &Sv, &Sv);
        pEm->Pos_x = pP->Workm.t[0] + Sv.vx;
        pEm->Pos_y = pP->Workm.t[1] + Sv.vy;
        pEm->Pos_z = pP->Workm.t[2] + Sv.vz;
    }
    Em39_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if (EM39_W(pEm)->Wait)
        EM39_W(pEm)->Wait--;
    if (pEm->Spd_z)
        pEm->Spd_z--;
    if (!(EM39_W(pEm)->Flg & 1)) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Spd_x != pP->Size_y) {
            if (pEm->Spd_x < pP->Size_y) {
                pP->Size_y -= pEm->Spd_y + 0x40;
                if (pEm->Spd_x >= pP->Size_y)
                    pP->Size_y = pEm->Spd_x;
            } else {
                pP->Size_y += pEm->Spd_y + 0x40;
                if (pP->Size_y >= pEm->Spd_x)
                    pP->Size_y = pEm->Spd_x;
            }
        }
        Under = (SHORT)func_8004D720(pP[3].Workm.t, 500, 0x2000, 0) < pP[3].Workm.t[1];
        n = Under;
        if ((SHORT)func_8004D720(pP[4].Workm.t, 500, 0x2000, 0) < pP[4].Workm.t[1])
            n = Under + 1;
        if ((SHORT)func_8004D720(pP[5].Workm.t, 500, 0x2000, 0) < pP[5].Workm.t[1])
            n++;
        pP->Size_y -= (SHORT)n * 0x80;
        if (pP->Size_y > 0x1000)
            pP->Size_y = 0x1000;
        if (pP->Size_y < 0x200)
            pP->Size_y = 0x200;
        n = pP->Size_y;
        pP[1].Size_y = n;
        pP[1].Pos_y = (SHORT)n * 626 / 4096;
        pP[2].Size_y = n;
        pP[2].Pos_y = (SHORT)n * 1235 / 4096;
        pP[3].Size_y = n;
        pP[3].Pos_y = (SHORT)n * 1460 / 4096;
    }
    if (pEm->Routine_0 != 3)
        Em_seq_sound(pEm);
}

/* Routine_0 0: find the parent (id 0x38) and set up the parts */
void Em39_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    tagGLOBAL *pG;
    ENEMY_WORK **ppEm;
    PARTS_W *pP;
    LONG n;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Damage_cnt = 0x80;
    pEm->Spd_x = 0x800;
    pEm->Life = -1;
    pEm->Max_life = -1;
    pG = &G;
    EM39_W(pEm)->Wait = 0;
    EM39_W(pEm)->Flg = 0;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Unk134 = 0;
    pEm->Unk136 = 0;
    pEm->Unk138 = 0;
    pEm->Unk139 = 0;
    pEm->Unk13A = 0;
    pEm->Unk140 = 0;
    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Unk120 = 0;
    pEm->Neck_no = 0;
    EM_SET_MOVE(pEm, 0, 0, 0);
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    Joint_move(pEm, pKan, pSeq, 0x400);
    EM39_W(pEm)->pParent = NULL;
    ppEm = &pG->pEnemy[2];
    do {
        if ((*ppEm)->Id == 0x38)
            EM39_W(pEm)->pParent = *ppEm;
    } while (++ppEm != pG->ppEnemy_end);
    pP = pEm->pSin_parts_ptr;
    pP[0].Be_flg |= 0x20;
    pP[3].Be_flg |= 0x20;
    pEm->Be_flg |= 0x200000;
    pP = pEm->pSin_parts_ptr;
    pP[0].Size_z = pP[0].Size_x = 0x1000;
    pP[0].Size_y = 0x800;
    pP[0].Be_flg |= 4;
    pP[1].Size_z = pP[1].Size_x = 0x1000;
    pP[1].Size_y = 0x800;
    pP[1].Be_flg |= 4;
    pP[2].Size_z = pP[2].Size_x = 0x1000;
    pP[2].Size_y = 0x800;
    pP[2].Be_flg |= 4;
    pP[3].Size_z = pP[3].Size_x = 0x1000;
    pP[3].Size_y = 0x800;
    pP[3].Be_flg |= 4;
    pP = pEm->pSin_parts_ptr;
    EM_SET_RTN(pEm, 1, 2, 0, 0);
    for (n = 6; n; n--) {
        pP->Be_flg |= 4;
        pP->Size_x = 0x1000;
        pP->Size_y = 0;
        pP->Size_z = 0x1000;
        Em39_part_scale_pos(pEm, pP, 0);
        pP++;
    }
}

/* Routine_0 1: move routine set by Type */
void Em39_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em39_move_type[(UCHAR)pEm->Type](pEm, pKan, pSeq);
}

/* move type 0: pick the next move (mv_br), then run it (mv) */
void Em39_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em39_mv_br[pEm->Routine_1](pEm);
    Em39_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* unused: as Em39_Move_type00 with brain table Em39_mv_br_unk */
void Em39_Move_type_unk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em39_mv_br_unk[pEm->Routine_1](pEm);
    Em39_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* wait for the player in reach: start the attack (Routine_1 = 1) */
void Em39_wait_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist;
    SHORT Turn;

    Dist = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Turn = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Dir_pos_ck_l(&pEm->Pos_x, &pPl->Pos_x, pEm->Cdir_y + 0x400, 0x264);
    if (Dist < 7000 && !Turn && !EM39_W(pEm)->Wait && !pEm->Spd_z)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* idle sway */
void Em39_wait(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x3F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd() & 0x3F) + 60;
        pEm->Spd_x = 0x400;
        pEm->Spd_y = 0;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x40);
        if (--pEm->Timer0 == -1) {
            EM_MOVE(pEm) = 0x3F0000 + ((Rnd() & 0xF) << 8) + Em39_move_rnd[(Rnd() >> (Rnd() & 3)) & 7]; /* random idle motion and frame, Hokan_flg 0x3F */
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

/* no brain step for this move */
void Em39_lash_br(ENEMY_WORK *pEm)
{
}

/* attack: lash at the player */
void Em39_lash(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        EM_SET_MOVE(pEm, 2, 0, 0x1F);
        if (pEm->UnkE2 == 1)
            pEm->Move_no++;
        pEm->Spd_x = 0x1000;
        pEm->Routine_2 = 1;
        pEm->Spd_y = 0x50;
        pEm->UnkE3 = 0;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
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

/* no brain step for this move */
void Em39_mv02_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em39_grow_br(ENEMY_WORK *pEm)
{
}

/* grow back out of the parent */
void Em39_grow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG Size;
    SVECTOR Sv;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x3F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->UnkDA = 0;
        EM39_W(pEm)->Flg |= 1;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x40);
        pP = pEm->pSin_parts_ptr;
        Size = pP->Size_y;
        Size += 0x80;
        if (Size > 0x1000) {
            Size = 0x1000;
            pEm->UnkDA = 1;
        }
        pP[0].Size_y = Size;
        Em39_part_scale_pos(pEm, &pP[0], Size);
        pP[1].Size_y = Size;
        Em39_part_scale_pos(pEm, &pP[1], Size);
        pP[2].Size_y = Size;
        Em39_part_scale_pos(pEm, &pP[2], Size);
        pP[3].Size_y = Size;
        Em39_part_scale_pos(pEm, &pP[3], Size);
        pP[4].Size_y = Size;
        Em39_part_scale_pos(pEm, &pP[4], Size);
        pP[5].Size_y = Size;
        Em39_part_scale_pos(pEm, &pP[5], Size);
        if (pEm->UnkDA) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
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
            Esp_call(0x1900, ((Rnd() * 2 + 0x1800) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
            Esp_call(0x1800, 0x10000000, &pP->Workm, &Sv, NULL);
            Sv.vx = 0x100 - Rnd() * 2;
            Sv.vy = 0x100 - Rnd() * 2;
            Sv.vz = 0x100 - Rnd() * 2;
            Esp_call((((Rnd() & 1) + 1) << 24) | 0x1900, ((Rnd() * 2 + 0x1800) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
            Esp_call((((Rnd() & 1) + 1) << 24) | 0x1800, 0x10000000, &pP->Workm, &Sv, NULL);
        }
        break;
    }
}

/* no brain step for this move */
void Em39_mv04_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em39_mv05_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em39_mv06_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em39_mv07_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em39_mv08_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em39_mv09_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* no brain step for this move */
void Em39_mv10_br(ENEMY_WORK *pEm)
{
}

/* empty move slot */
void Em39_mv10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* Routine_0 2: no damage */
void Em39_Damage(void)
{
    SVECTOR Unused;
}

/* Routine_0 3: nothing */
void Em39_Die(void)
{
}

/* Routine_0 4: nothing */
void Em39_Scenario(void)
{
}

/* Routine_0 7: nothing */
void Em39_Dead(void)
{
}

/* unused */
void Em39_dummy0(void)
{
}

/* unused */
void Em39_dummy1(void)
{
}

/* splash effects while moving (declared with a result it never returns) */
LONG Em39_splash(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR Sv;

    if ((pEm->Move_cnt & 7) == 1) {
        Sv.vx = 11;
        Sv.vy = (Rnd() & 0xF) + 0x10C;
        Sv.vz = 50;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x4001700, ((Rnd() * 4 + 0x1400) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
        Esp_call(0x4000500, 0xA000000, &pP->Workm, &Sv, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && (Rnd() & 1)) {
        Sv.vx = 11;
        Sv.vy = 300;
        Sv.vz = 50;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x2001700, ((Rnd() * 4 + 0x1400) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, NULL);
        Esp_call(0x2000500, 0xA000000, &pP->Workm, &Sv, NULL);
    }
}

/* direction of part pP's x axis */
SHORT Em39_part_dir(ENEMY_WORK *pEm, PARTS_W *pP)
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
void Em39_part_scale_pos(ENEMY_WORK *pEm, PARTS_W *pP, LONG Size)
{
    SHORT *pPos;

    pPos = (SHORT *)(pEm->pKan_t_ptr + 2);
    pPos += pP->Unk84 * 3;
    pP->Pos_x = pPos[0] * Size >> 12;
    pP->Pos_y = pPos[1] * Size >> 12;
    pP->Pos_z = pPos[2] * Size >> 12;
}
