#include "ovl/em39.h"
#include "joint.h"
#include "sca.h"
#include "snd.h"

LONG Dir_pos_ck_l(VECTOR *pV, VECTOR *pP, LONG Hed_dir, LONG Hani_dir) __asm__("Dir_pos_ck"); /* em_sub.c, called with int args */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");      /* em_sub.c, result used as int */
LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */

/* RE2 EM_MAIN: follow the parent part, then grow/shrink with the ground under parts 3..5 */
void func_80100004(ENEMY_WORK *pEm)
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
            pEm->Cdir_y = func_801012A4(EM39_W(pEm)->pParent, &pP[12]) + (SHORT)(EM39_W(pEm)->Dir + 0x200);
        } else {
            Sv.vx = -0x1B8;
            Sv.vy = -0x96;
            Sv.vz = -0x96;
            pEm->Cdir_y = func_801012A4(EM39_W(pEm)->pParent, &pP[12]) + (SHORT)(EM39_W(pEm)->Dir - 0x100);
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

/* RE2 Em39_init */
void func_801003B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    tagGLOBAL *pG;
    ENEMY_WORK **ppEm;
    PARTS_W *pP;
    LONG n;

    EM_RTN(pEm) = 1;
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
    EM_MOVE(pEm) = 0;
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
    EM_RTN(pEm) = 0x201;
    for (n = 6; n; n--) {
        pP->Be_flg |= 4;
        pP->Size_x = 0x1000;
        pP->Size_y = 0;
        pP->Size_z = 0x1000;
        func_8010133C(pEm, pP, 0);
        pP++;
    }
}

/* RE2 Em39_move */
void func_80100588(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em39_move_type[(UCHAR)pEm->Type](pEm, pKan, pSeq);
}

/* move type 0: pick the next move (mv_br), then run it (mv) */
void func_801005C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em39_mv_br[pEm->Routine_1](pEm);
    Em39_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* unused: same as func_801005C4 with a table that does not exist */
void func_80100648(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em39_mv_br_unk[pEm->Routine_1](pEm);
    Em39_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

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
void func_8010079C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x3F0000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd_l() & 0x3F) + 60;
        pEm->Spd_x = 0x400;
        pEm->Spd_y = 0;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x40);
        if (--pEm->Timer0 == -1) {
            EM_MOVE(pEm) = 0x3F0000 + ((Rnd_l() & 0xF) << 8) + Em39_move_rnd[(Rnd_l() >> (Rnd_l() & 3)) & 7];
            pEm->Timer0 = (Rnd_l() & 0x3F) + 60;
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
void func_801008E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_RTN(pEm) = 1;
            EM39_W(pEm)->Wait = (Rnd_l() & 0x3F) + 90;
            if (pPl->Life < 100)
                EM39_W(pEm)->Wait = (Rnd_l() & 0x3F) + 120;
            if (pPl->Life < 20)
                EM39_W(pEm)->Wait = (Rnd_l() & 0x7F) + 150;
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
void func_80100DD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        Joint_move(pEm, pKan, pSeq, 0x40);
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
            EM39_W(pEm)->Wait = (Rnd_l() & 0x7F) + 30;
            pP[4].Be_flg &= ~4;
            pP[5].Be_flg &= ~4;
            EM39_W(pEm)->Flg &= ~1;
        }
        pEm->Timer0++;
        if (!(pEm->Move_cnt & 3)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0x100 - Rnd_l() * 2;
            Sv.vy = 0x100 - Rnd_l() * 2;
            Sv.vz = 0x100 - Rnd_l() * 2;
            Esp_call(0x1900, ((Rnd_l() * 2 + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, NULL);
            Esp_call(0x1800, 0x10000000, &pP->Workm, &Sv, NULL);
            Sv.vx = 0x100 - Rnd_l() * 2;
            Sv.vy = 0x100 - Rnd_l() * 2;
            Sv.vz = 0x100 - Rnd_l() * 2;
            Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1900, ((Rnd_l() * 2 + 0x1800) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, NULL);
            Esp_call((((Rnd_l() & 1) + 1) << 24) | 0x1800, 0x10000000, &pP->Workm, &Sv, NULL);
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
        Sv.vy = (Rnd_l() & 0xF) + 0x10C;
        Sv.vz = 50;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x4001700, ((Rnd_l() * 4 + 0x1400) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, NULL);
        Esp_call(0x4000500, 0xA000000, &pP->Workm, &Sv, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && (Rnd_l() & 1)) {
        Sv.vx = 11;
        Sv.vy = 300;
        Sv.vz = 50;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x2001700, ((Rnd_l() * 4 + 0x1400) << 16) | (USHORT)(Rnd_l() << 4), &pP->Workm, &Sv, NULL);
        Esp_call(0x2000500, 0xA000000, &pP->Workm, &Sv, NULL);
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
