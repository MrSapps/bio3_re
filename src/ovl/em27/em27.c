#include "ovl/em27.h"
#include "sca.h"
#include "snd.h"

LONG Rnd(void) __asm__("func_800102E8");                                 /* RE2 Rnd (sld.s) */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector");  /* main.c, called with an int angle */

/* RE2 EM_MAIN */
void func_8010001C(ENEMY_WORK *pEm)
{
    Em27_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* stay on the floor: undo the move when the ground height changes */
void func_80100060(ENEMY_WORK *pEm)
{
    VECTOR V;

    V.vx = pEm->Pos_x;
    V.vz = pEm->Pos_z;
    V.vy = pEm->Unk122;
    if (func_8004D720(&V.vx, 350, 0x8000, 0) != pEm->Unk122) {
        pEm->Pos_x = pEm->Old_x;
        pEm->Pos_z = pEm->Old_z;
    } else {
        pEm->Old_x = pEm->Pos_x;
        pEm->Old_z = pEm->Pos_z;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801000E0);

/* reset the parts: hide 0 and 1, part 2 at the body, parts 3 and 4 beside it */
void func_80100378(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR Sv;

    pP = pEm->pSin_parts_ptr;
    pP->Be_flg |= 4;
    pP->Size_z = 0;
    pP->Size_y = 0;
    pP->Size_x = 0;
    pP = &pEm->pSin_parts_ptr[1];
    pEm->pSin_parts_ptr[1].Be_flg |= 4;
    pP->Size_z = 0;
    pP->Size_y = 0;
    pP->Size_x = 0;
    pP = &pEm->pSin_parts_ptr[2];
    pP->Pos_x = pEm->Pos_x;
    pP->Pos_y = pEm->Pos_y;
    pP->Pos_z = pEm->Pos_z;
    pP = &pEm->pSin_parts_ptr[3];
    pEm->pSin_parts_ptr[3].Be_flg |= 0x20;
    pP->UnkA2 |= 0x1000;
    Sv.vx = -250;
    Sv.vz = 0;
    Rot_vector_l(pEm->Cdir_y - 0x180, &Sv, &Sv);
    pP->Pos_x = pEm->Pos_x + Sv.vx;
    pP->Pos_y = pEm->Pos_y;
    pP->Pos_z = pEm->Pos_z + Sv.vz;
    pP = &pEm->pSin_parts_ptr[4];
    pEm->pSin_parts_ptr[4].Be_flg |= 0x20;
    pP->UnkA2 |= 0x1000;
    Sv.vx = -250;
    Sv.vz = 0;
    Rot_vector_l(pEm->Cdir_y + 0x180, &Sv, &Sv);
    pP->Pos_x = pEm->Pos_x + Sv.vx;
    pP->Pos_y = pEm->Pos_y;
    pP->Pos_z = pEm->Pos_z + Sv.vz;
    EM27_W(pEm)->Wait = (Rnd() & 0x7F) + 0x40;
    EM27_W(pEm)->Pop = 0;
    pEm->Timer0 = 30;
}

/* hide parts 0 and 1, put part 2 at the body, show parts 3 and 4 */
void func_80100518(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    pP->Be_flg |= 4;
    pP->Size_z = 0;
    pP->Size_y = 0;
    pP->Size_x = 0;
    pP = &pEm->pSin_parts_ptr[1];
    pEm->pSin_parts_ptr[1].Be_flg |= 4;
    pP->Size_z = 0;
    pP->Size_y = 0;
    pP->Size_x = 0;
    pP = &pEm->pSin_parts_ptr[2];
    pP->Pos_x = pEm->Pos_x;
    pP->Pos_y = pEm->Pos_y;
    pP->Pos_z = pEm->Pos_z;
    pP = &pEm->pSin_parts_ptr[3];
    pP->UnkA2 &= ~1;
    pP = &pEm->pSin_parts_ptr[4];
    pP->UnkA2 &= ~1;
    EM27_W(pEm)->Wait = (Rnd() & 0x7F) + 0x40;
    EM27_W(pEm)->Pop = 0;
    pEm->Timer0 = 120;
}

/* RE2 Em27_Move */
void func_801005F8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em27_mv_br[pEm->Routine_1](pEm);
    Em27_mv[pEm->Routine_1](pEm, pKan, pSeq);
    pEm->Unk1C4 = EM_RTN(pEm);
}

/* wait, then pick the next move (Routine_1 = 1) */
void func_80100688(ENEMY_WORK *pEm)
{
    if (EM27_W(pEm)->Wait) {
        EM27_W(pEm)->Wait--;
        return;
    }
    EM27_W(pEm)->Wait = (Rnd() & 0x2F) + 0x40;
    EM_RTN(pEm) = 0x101;
    pEm->Damage_cnt &= 0x7F;
}

/* mv[0]: crawl in short bursts towards the player or a random point */
void func_801006E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70000;
        pEm->Spd_x = 300;
        pEm->Timer0 = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Routine_2++;
        if (Rnd() & 3) {
            pEm->Dest_x = pPl->Pos_x;
            pEm->Dest_z = pPl->Pos_z;
        } else {
            pEm->Dest_x = pEm->Pos_x + (Rnd() - 0x7F) * 2;
            pEm->Dest_z = pEm->Pos_z + (Rnd() - 0x7F) * 2;
        }
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_z = 0;
        pEm->Spd_x -= 4;
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        pEm->Damage_cnt &= 0x7F;
        if (++pEm->Timer0 >= 9) {
            pEm->Timer0 = 0;
            pEm->Spd_x = 300;
            pEm->Routine_2--;
            if (Rnd() & 3) {
                pEm->Dest_x = pPl->Pos_x;
                pEm->Dest_z = pPl->Pos_z;
            } else {
                pEm->Dest_x = pEm->Pos_x + (Rnd() - 0x7F) * 2;
                pEm->Dest_z = pEm->Pos_z + (Rnd() - 0x7F) * 2;
            }
        }
        break;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (Rnd() & 0xF) | 0x20);
}

/* mv_br[1]: wander; leap at a near player (Routine_1 2); shaken off by a step move */
void func_801008EC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;

    if (EM27_W(pEm)->Wait)
        EM27_W(pEm)->Wait--;
    else {
        EM27_W(pEm)->Wait = (Rnd() & 0x1F) + 0x10;
        EM_RTN(pEm) = 1;
    }
    if ((LONG)pEm->Unk154 < 3000 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200) && pPl->Routine_0 == 1
        && pPl->Routine_1 < 6)
        EM_RTN(pEm) = 0x201;
    if ((LONG)pEm->Unk154 < 750 && (G.Unk75E8 & 6) && pEm->nFloor == pPl->nFloor && pEm->Pos_y == pEm->Unk122
        && (EM_RTN01(pPl) == 0x101 || EM_RTN01(pPl) == 0x201 || EM_RTN01(pPl) == 0x301 || EM_RTN01(pPl) == 0x401)) {
        EM_RTN(pEm) = 0x1000003;
        Snd_se_enem(15, pEm);
        Pad_vib_set0(1, 0);
        Pad_vib_set1(3, 0x50, 0);
    }
}

/* mv[1]: crawl towards the player or a random point nearby */
void func_80100A50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70001;
        if (Rnd() & 3) {
            pEm->Dest_x = pPl->Pos_x;
            pEm->Dest_z = pPl->Pos_z;
        } else {
            pEm->Dest_x = pEm->Pos_x + (Rnd() - 0x7F) * 2;
            pEm->Dest_z = pEm->Pos_z + (Rnd() - 0x7F) * 2;
        }
        pEm->Routine_2++;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (Rnd() & 0xF) | 0x40);
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Cdir_y += (Rnd() & 3) * 32 - (40 - (Rnd() & 0xF));
        pEm->Spd_x = 150;
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
    }
}

/* mv_br[2]: close to the player: leap onto it (Routine_1 3) or turn aside */
void func_80100BAC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG Dir;

    if ((LONG)pEm->Unk154 < 1000) {
        if (pEm->Move_cnt >= 12 && pEm->Move_cnt < 15 && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100)
            && pPl->Routine_0 == 1 && pPl->Routine_1 < 7 && !pPl->Damage_cnt) {
            pP = pEm->pSin_parts_ptr;
            if (pPl->pSin_parts_ptr[1].Workm.t[1] - 200 < pP->Workm.t[1]) {
                pP->Old_y = 0;
                pEm->Pos_y = pEm->pSin_parts_ptr->Workm.t[1];
                pPl->Damage_cnt |= 0x80;
                EM_RTN(pEm) = 0x301;
                return;
            }
        }
        if ((LONG)pEm->Unk154 < 600) {
            Dir = pEm->Cdir_y;
            if (Rnd() & 1)
                pEm->Cdir_y = Dir + 500;
            else
                pEm->Cdir_y = Dir - 500;
        }
    }
}

/* mv[2]: hop towards the player */
void func_80100CD8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70002;
        pEm->Damage_cnt |= 0x80;
        pEm->Routine_2++;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            EM_RTN(pEm) = ((Rnd() & 1) << 8) + 1;
        if (pEm->Move_cnt < 15)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
        Add_speedXZ(pEm, 0);
    }
}

void func_80100DDC(void) {
}

/* mv[3]: latched onto the player: bite, ride along until shaken off (Gacha_ch) */
void func_80100DE4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Pos;
    SVECTOR Sv;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->UnkE2 = 1;
        EM_MOVE(pEm) = 0x70003;
        EM27_W(pEm)->Dir = Direction_ck(pPl->Pos_x, pPl->Pos_z, pEm->Pos_x, pEm->Pos_z);
        EM27_W(pEm)->Dir = (EM27_W(pEm)->Dir - pPl->Cdir_y) & 0xFFF;
        pPl->pEnemy_ptr = pEm;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Routine_0 = 5;
        pPl->Routine_1 = 0;
        pPl->Routine_2 = 0;
        pPl->Routine_3 = 0;
        Pl_life_down(1, 1);
        Pad_vib_set0(2, 0);
        Pad_vib_set0(1, 4);
        Sv.vx = (Rnd() & 7) - 4;
        Sv.vy = 0;
        Sv.vz = (Rnd() & 7) - 4;
        func_8001B484(0x700, (USHORT)pEm->Cdir_y | 0xA000000, &pEm->pSin_parts_ptr->Workm, &Sv, NULL);
        EM27_W(pEm)->Floor = pPl->Unk122;
        pEm->Be_flg |= 0x400;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200) && pEm->UnkE2)
            EM_RTN(pEm) = 0x401;
        if (Gacha_ch())
            pEm->UnkE2 = 1;
        pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
        Pos.vx = pPl->pSin_parts_ptr->Workm.t[0];
        Pos.vz = pPl->pSin_parts_ptr->Workm.t[2];
        func_801014B0(pEm, 150, EM27_W(pEm)->Dir + pPl->Cdir_y, &Pos);
        Add_speedXZ(pEm, 0);
    }
}

/* mv_br[4]: latched on: the player shakes it off with a step move */
void func_80100FFC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = G_PLAYER;

    if (pEm->Routine_2 >= 2 && (LONG)pEm->Unk154 < 750 && (G.Unk75E8 & 6) && pEm->nFloor == pPl->nFloor
        && pEm->Pos_y == pEm->Unk122
        && (EM_RTN01(pPl) == 0x101 || EM_RTN01(pPl) == 0x201 || EM_RTN01(pPl) == 0x301 || EM_RTN01(pPl) == 0x401)) {
        EM_RTN(pEm) = 0x1000003;
        Snd_se_enem(15, pEm);
        Pad_vib_set0(1, 0);
        Pad_vib_set1(3, 0x50, 0);
    }
}

/* mv[4]: thrown off: fall to the floor, roll a few times, then back to mv 1 */
void func_801010D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 5;
        pEm->Routine_2++;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x = -100;
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        pEm->Unk122 = func_8004D720(&pEm->Pos_x, pEm->Col.W0, 0x200, 9);
        pEm->Pos_y += pEm->Timer0;
        if (pEm->Timer0 < 800)
            pEm->Timer0 <<= 1;
        if (pEm->Pos_y >= pEm->Unk122) {
            pEm->Pos_y = pEm->Unk122;
            pEm->Routine_2++;
        }
        break;
    case 2:
        pEm->Be_flg &= ~0x400;
        pEm->pSin_parts_ptr->Old_y = -300;
        EM_MOVE(pEm) = 0x70007;
        pEm->Pos_y = pEm->Unk122;
        pEm->Damage_cnt &= 0x7F;
        pEm->UnkE3 = Rnd() & 3;
        pEm->Routine_2++;
    case 3:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_z = 0;
            pEm->Spd_x = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (pEm->UnkE3 == 0)
                pEm->Routine_2++;
            else
                pEm->UnkE3--;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        EM_MOVE(pEm) = 0x70008;
        pEm->Routine_2++;
    case 5:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_z = 0;
            pEm->Spd_x = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            EM_RTN(pEm) = 0x101;
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_8010131C(void) {
}

void func_80101324(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Joint_move(pEm, pKan, pSeq, 0x400);
}

/* hidden: wait for the player, then pop up (parts 0 and 1 grow) and come alive */
void func_80101344(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        func_80100518(pEm);
        pEm->Routine_2++;
    case 1:
        if (pEm->Timer0 == 0) {
            if ((LONG)pEm->Unk154 < 3000) {
                pEm->Timer0 = 0;
                EM27_W(pEm)->Pop = 1;
                pEm->Routine_2++;
            }
        } else
            pEm->Timer0--;
        break;
    case 2:
        if (pEm->Timer0 != 0x1000) {
            pP = pEm->pSin_parts_ptr;
            pP->Be_flg |= 4;
            pP->Size_x = pP->Size_y = pP->Size_z = pEm->Timer0;
            pP = &pEm->pSin_parts_ptr[1];
            pEm->pSin_parts_ptr[1].Be_flg |= 4;
            pP->Size_x = pP->Size_y = pP->Size_z = pEm->Timer0;
            pEm->Timer0 += 0x200;
        } else if (EM27_W(pEm)->Pop == 3) {
            pEm->Life = 1;
            EM_RTN(pEm) = 1;
            pEm->Be_flg &= ~0x100000;
            pEm->Type &= ~0x4000;
        }
        break;
    }
    func_8010158C(pEm);
}

/* place the body Dist away from pPos at angle Dir, following the player's floor */
void func_801014B0(ENEMY_WORK *pEm, SHORT Dist, SHORT Dir, VECTOR *pPos)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = Dir;
    Sv.vz = 0;
    RotMatrix(&Sv, &M);
    Sv.vx = Dist;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, &Sv);
    pEm->Pos_x = Sv.vx + pPos->vx;
    pEm->Pos_z = Sv.vz + pPos->vz;
    pEm->Pos_y += pPl->Unk122 - EM27_W(pEm)->Floor;
    EM27_W(pEm)->Floor = pPl->Unk122;
    pEm->nFloor = pPl->nFloor;
}

/* pop part 2 up to full size, then shrink it back */
void func_8010158C(ENEMY_WORK *pEm)
{
    PARTS_W *pP0;
    PARTS_W *pP;

    pP0 = pEm->pSin_parts_ptr;
    pP = &pP0[2];
    switch (EM27_W(pEm)->Pop) {
    case 0:
        break;
    case 1:
        Snd_se_enem(14, pEm);
        pP0[2].Be_flg |= 4;
        pP->Size_z = 0x1000;
        pP->Size_x = 0x1000;
        pP->Size_y = 0x1000;
        EM27_W(pEm)->Pop = 2;
        break;
    case 2:
        pP->Size_y -= 0x100;
        if (pP->Size_y < 0x400) {
            pP->UnkA2 &= ~1;
            EM27_W(pEm)->Pop = 3;
        }
        break;
    }
}

void func_80101660(void) {
}

/* RE2 Em27_Die: fall, or (Routine_3 1) burst into blood */
void func_80101668(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pKage;
    SVECTOR Sv;

    pKage = pEm->Kage.pKw;
    if (pEm->Routine_3 == 1) {
        pEm->Routine_2 = 2;
        pEm->Routine_3 = 0;
    }
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        Em_dead_flg_on_cand(pEm);
        EM_MOVE(pEm) = 0x70005;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            EM_RTN(pEm) = 7;
        break;
    case 2:
        Em_dead_flg_on_cand(pEm);
        *(ULONG *)&pKage->Prim[0].r0 = (*(ULONG *)&pKage->Prim[0].r0 & 0xFF000000) | 0xEF409F;
        *(ULONG *)&pKage->Prim[1].r0 = (*(ULONG *)&pKage->Prim[1].r0 & 0xFF000000) | 0xEF409F;
        pEm->Col.Id = 0;
        Sv.vx = (Rnd() & 7) - 4;
        Sv.vy = 0;
        Sv.vz = (Rnd() & 7) - 4;
        func_8001B484(0x1000700, (USHORT)pEm->Cdir_y | 0x16000000, &pEm->pSin_parts_ptr->Workm, &Sv, NULL);
        pEm->pSin_parts_ptr->Be_flg = 0;
        pEm->Routine_2++;
        break;
    case 3:
        pEm->Be_flg = 0;
        G.Em_num--;
        break;
    }
}

/* RE2 Em27_Dead: the shadow turns red and spreads */
void func_801017FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pKage;

    pKage = pEm->Kage.pKw;
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Timer0 = 30;
        EM_MOVE(pEm) = 0x70006;
        *(ULONG *)&pKage->Prim[0].r0 = (*(ULONG *)&pKage->Prim[0].r0 & 0xFF000000) | 0xEF409F;
        *(ULONG *)&pKage->Prim[1].r0 = (*(ULONG *)&pKage->Prim[1].r0 & 0xFF000000) | 0xEF409F;
    case 1:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        break;
    }
    if (pEm->Timer0) {
        pEm->Timer0--;
        pKage->Half_x += 4;
        pKage->Half_z += 4;
    }
}

/* move: hop (seq sound, collision off while airborne) */
void func_801018D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70000;
        Snd_se_stad(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Routine_2++;
        pEm->Unk120 |= 0x12;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            EM_RTN(pEm) = 1;
            pEm->Unk120 &= ~0x12;
            pEm->Damage_cnt &= 0x7F;
        }
        Add_speedXZ(pEm, 0);
    }
}

/* number of other live enemies of id 0x23 / 0x28 */
LONG func_801019BC(ENEMY_WORK *pEm)
{
    ENEMY_WORK **ppEm;
    LONG n = 0;

    for (ppEm = &G.pEnemy[0]; ppEm != G.ppEnemy_end; ppEm++) {
        if (((*ppEm)->Be_flg & 1) && ((*ppEm)->Id == 0x23 || (*ppEm)->Id == 0x28) && *ppEm != pEm)
            n++;
    }
    return n;
}
