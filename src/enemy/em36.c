#include "enemy/em36.h"
#include "door.h"
#include "root_ck.h"
#include "sabun.h"
#include "sca.h"
#include "snd.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
void func_8004A934_(ENEMY_WORK *pEm, PARTS_W *pP, LONG a2) __asm__("func_8004A934"); /* 3 args here (canonical 2) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck"); /* returns long here (canonical UC) */
LONG Dir_pos_ck_l(VECTOR *pV, VECTOR *pP, LONG Hed_dir, LONG Hani_dir) __asm__("Dir_pos_ck"); /* arg 3 long (canonical short); arg 4 long (canonical short) */

/* RE2 Em12a: enemy main routine */
void func_801000F8(ENEMY_WORK *pEm)
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

    pTv = (VECTOR *)&pPl->Pos_x;
    if (EM36_W(pEm)->Lost_time)
        pTv = &Em36_lost_pos;
    if ((UCHAR)pEm->Type == 8 || (UCHAR)pEm->Type == 12)
        pTv = &Em36_route_pos[EM36_W(pEm)->Route_no];
    if (EM36_W(pEm)->Flg & 0x1000 && *(SHORT *)&pEm->Routine_0 != 0x601 && EM36_W(pEm)->Unk15 < 3) {
        Route = pEm->Unk14E & 0xF;
        pTv = &Em36_route[Route][EM36_W(pEm)->Unk28];
    }
    Root_ck(pEm, pTv, 0, 0);
    EM36_W(pEm)->Flg &= ~0x800;
    pEm->Pos_y -= 400;
    pPl->Pos_y -= 400;
    if (!Sca_ck_line((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, 0x40, 1))
        EM36_W(pEm)->Flg |= 0x800;
    pEm->Pos_y += 400;
    pPl->Pos_y += 400;
    if (*(SHORT *)&pPl->Routine_0 == 0x701)
        EM36_W(pEm)->Flg |= 0x800;
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (!(pEm->Type & 1))
        func_8010B19C(pEm);
    Em36_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if (G.Unk783E == 4 && G.Unk7840 == 9) {
        Rno = pEm->Unk14E;
        if (Rno & 0x80) {
            if (!(EM36_W(pEm)->Flg & 0x1000)) {
                Route = Rno & 0xF;
                pV = &Em36_route[Route][0];
                EM36_W(pEm)->Unk26 = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
                if (EM36_W(pEm)->Unk26 < 3000) {
                    EM36_W(pEm)->Unk28 = 1;
                    EM36_W(pEm)->Flg |= 0x1000;
                    pV = &Em36_route[Route][1];
                    Dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
                    pV = &Em36_route[Route][2];
                    if (SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z)) < Dist)
                        EM36_W(pEm)->Unk28 = 2;
                }
            }
        } else {
            EM36_W(pEm)->Flg &= ~0x1000;
        }
    }
    func_8010BEAC(pEm);
    func_8010BDD0(pEm);
    func_8010B308(pEm);
    if (pEm->Type & 1) {
        func_8010C4FC(pEm);
        if (EM36_W(pEm)->Unk15 >= 2)
            func_8010C6CC(pEm);
        if (EM36_W(pEm)->Unk15 >= 3)
            func_8010C764(pEm);
    }
    if ((LONG)pEm->Unk154 < 3000) {
        if (EM36_W(pEm)->Unk29)
            EM36_W(pEm)->Unk29--;
    } else {
        EM36_W(pEm)->Unk29 = 120;
    }
    if (pEm->Be_flg & 8) {
        if (pEm->Unk178[2] > 0) {
            pEm->Unk178[2] -= 24;
            if (pEm->Unk178[2] < 0)
                pEm->Unk178[2] = 0;
        }
        if (pEm->Unk178[0] > 0) {
            pEm->Unk178[0] -= 24;
            if (pEm->Unk178[0] < 0)
                pEm->Unk178[0] = 0;
        }
        if (pEm->Unk178[2] < 0) {
            pEm->Unk178[2] += 24;
            if (pEm->Unk178[2] > 0)
                pEm->Unk178[2] = 0;
        }
        if (pEm->Unk178[0] < 0) {
            pEm->Unk178[0] += 24;
            if (pEm->Unk178[0] > 0)
                pEm->Unk178[0] = 0;
        }
        if (pEm->Unk178[0] == 0 && pEm->Unk178[2] == 0) {
            pEm->Be_flg &= ~8;
            pP = &pEm->pSin_parts_ptr[0];
            pP->UnkA2 &= ~0x40;
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 &= ~0x40;
        }
    }
    if (EM36_W(pEm)->Unk08)
        EM36_W(pEm)->Unk08--;
    if (EM36_W(pEm)->Unk06)
        EM36_W(pEm)->Unk06--;
    if (EM36_W(pEm)->Unk09)
        EM36_W(pEm)->Unk09--;
    if (EM36_W(pEm)->Unk14)
        EM36_W(pEm)->Unk14--;
    if (EM36_W(pEm)->Unk2A)
        EM36_W(pEm)->Unk2A--;
    Hit_area_parts_set(pEm, 0xFE700002, 0x190000C);
    pEm->pEnemy_ptr = pPl;
    Rot_neck_em(pEm, pEm->Cdir_y);
    Spd = 0x200;
    if (pEm->Routine_0 == 7 || EM_RTN(pEm) == 0x30801)
        Spd = 0;
    pP = &pEm->pSin_parts_ptr[1];
    EM36_SABUN_P(pP)->Key[0].Rate += EM36_W(pEm)->Unk16;
    if (EM36_SABUN_P(pP)->Key[0].Rate >= 0x1000)
        EM36_W(pEm)->Unk16 = -Spd;
    if (EM36_SABUN_P(pP)->Key[0].Rate <= 0)
        EM36_W(pEm)->Unk16 = Spd;
    func_8004A934_(pEm, pP, Spd);
    Seq = *pEm->pNow_seq;
    if (Seq & 0xF000) {
        Se = (Seq >> 12) - 1;
        if ((UCHAR)pEm->Type == 3 && Se - 11 < 4)
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        else
            Snd_se_enem((USHORT)Se, pEm);
    }
}

INCLUDE_ASM("asm/nonmatchings/enemy/em36", func_8010076C);

/* RE2 Em2a_Move */
void func_8010105C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_Move_type[(UCHAR)pEm->Type](pEm, pKan, pSeq);
}

void func_80101098(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_mv_br00[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8010111C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_mv_br01[pEm->Routine_1](pEm);
    Em36_mv01[pEm->Routine_1](pEm, pKan, pSeq);
    if ((EM36_FLGW(pEm) & 0x2001) == 0x2000 && !(pEm->Damage_cnt & 0x80)) {
        pEm->Unk12C = 30;
        EM_SET_RTN(pEm, 2, 0, 0, 0);
        pEm->Damage_cnt |= 0x80;
        EM36_W(pEm)->Flg &= ~3;
    }
}

void func_801011EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    pEm->Damage_cnt |= 0x80;
    pEm->Status_flg &= ~0x2000;
    Em36_mv_br02[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80101288(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        pEm->Unk12C = 30;
        EM_SET_RTN(pEm, 4, 0xA, 0, 0);
        pEm->Life = -1;
        pEm->Damage_cnt |= 0x80;
        EM36_W(pEm)->Flg &= ~3;
        Em_dead_flg_on_cand(pEm);
    }
}

void func_801013F4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        if ((LONG)pEm->Unk154 < 4000)
            EM36_W(pEm)->Type_timer++;
    }
}

void func_80101520(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];

    EM36_W(pEm)->Route_dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
    Em36_mv_br04[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
    if (EM36_W(pEm)->Type_timer) {
        EM36_W(pEm)->Type_timer--;
        if ((LONG)pEm->Unk154 < 3000)
            EM36_W(pEm)->Type_timer -= 3;
        if (pEm->Damage_cnt == 1)
            EM36_W(pEm)->Type_timer -= 10;
        if ((LONG)pEm->Unk154 > 5000)
            EM36_W(pEm)->Type_timer++;
        if (EM36_W(pEm)->Type_timer <= 0) {
            EM36_W(pEm)->Type_timer = 300;
            pEm->Type -= 2;
        }
    }
}

void func_80101688(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];

    EM36_W(pEm)->Route_dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
    Em36_mv_br00[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80101760(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];

    EM36_W(pEm)->Route_dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
    Em36_mv_br05[pEm->Routine_1](pEm);
    Em36_mv00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* brain: stand */
void func_80101838(ENEMY_WORK *pEm)
{
    ULONG Dist = pEm->Unk154;
    SHORT Dir = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x400);

    if (Dist < 3000 && Dir) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
    if (!Dir && pEm->Status_flg & 0x2000) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
}

/* motion: stand */
void func_801018BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
void func_80101958(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir, Dir_far, Dir_eye;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Dir_far = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Dir_eye = Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 8000 && !Dir && pEm->Status_flg & 0x2000 && !EM36_W(pEm)->Unk2A)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far && EM36_W(pEm)->Flg & 0x800) {
        EM_SET_RTN(pEm, 1, 4, 0, 0);
        if (Dist < 3500 && !Dir_eye && (Rnd_l() & 3) == 1 && !Flg_ck(&G.Unk7888, 0x1F))
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (Dist < 1300 && !Dir)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (!EM36_W(pEm)->Unk08 && Dist < 3000 && !Dir && !pPl->Damage_cnt) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (pPl->Life < 40 && pPl->Unk12C != 0x80 && EM36_W(pEm)->Flg & 0x800 && !Flg_ck(&G.Unk7888, 0x1E)) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM36_W(pEm)->Flg &= ~0x200;
        }
    }
    if (EM36_W(pEm)->Unk08 < 5 && Dist < 3000 && !Dir_eye && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    if (EM36_W(pEm)->Flg & 0x800) {
        if (!EM36_W(pEm)->Unk14 && Dist < 4500 && Dist > 2000 && !Dir && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1E)) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM36_W(pEm)->Flg &= ~0x200;
        }
        if (!EM36_W(pEm)->Unk06 && Dist > 4500 && Dist < 5500 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
    if (!EM36_W(pEm)->Unk29 && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    if (EM36_W(pEm)->Flg & 0x10)
        EM_SET_RTN(pEm, 1, 9, 0, 0);
}

/* motion: walk */
void func_80101CC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            if ((LONG)pEm->Unk154 < 3000)
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
                Side = Rnd_l() & 1;
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
void func_80101EC8(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir, Dir_far, Dir_near, Dir_eye;
    VECTOR *pV;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Dir_far = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Dir_near = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Dir_eye = Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist < 1300 && !Dir)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (Dist < 6000 && Dir_far && !pEm->Hokan_flg)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (!EM36_W(pEm)->Unk08) {
        if (Dist < 3000 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (!EM36_W(pEm)->Unk08 && Dist < 3500 && !Dir_eye && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (EM36_W(pEm)->Flg & 0x800) {
        if (!EM36_W(pEm)->Unk14 && Dist < 3500 && !Dir && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1E)) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM36_W(pEm)->Flg &= ~0x200;
        }
        if (!EM36_W(pEm)->Unk06 && Dist > 3500 && Dist < 4500 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
    if (!EM36_W(pEm)->Unk29 && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1F))
        EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    if (!EM36_W(pEm)->Unk08 && Dist < 9000 && Dist > 6000 && !Dir_near && !pPl->Damage_cnt && EM36_W(pEm)->Unk15 >= 2)
        EM_SET_RTN(pEm, 1, 0x12, 0, 0);
    if (EM36_W(pEm)->Flg & 0x1000) {
        EM_SET_RTN(pEm, 1, 0x14, 0, 0);
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Unk28];
        if (Goto00_ck(pEm, pV->vx, pV->vz, 0x400))
            pEm->Routine_2 = 2;
    }
    if (EM36_W(pEm)->Flg & 0x40)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
}

/* motion: run */
void func_80102204(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
                if (Rnd_l() & 1)
                    Add = 40;
                if ((LONG)pEm->Unk154 < 3000) {
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
                Side = Rnd_l() & 1;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x40 - Side * 120;
            }
        } else {
            pEm->UnkDA = 0;
        }
    }
}

/* brain: run */
void func_80102420(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir, Dir_far;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Dir_far = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x400, 0x264);
    if (Dist > 11000 && !Dir && pEm->Status_flg & 0x2000)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
}

/* brain: combo (next routine from Em36_combo_tbl) */
void func_80102504(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    UCHAR Rtn;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (EM36_W(pEm)->Flg & 0x100) {
        Rtn = Em36_combo_tbl[EM36_W(pEm)->Unk20][EM36_W(pEm)->Unk1D];
        EM36_W(pEm)->Unk1D++;
        if (!(Rtn & 0x80)) {
            EM_RTN(pEm) = (Rtn << 8) + 1;
            EM36_W(pEm)->Flg &= ~0x200;
            return;
        }
        EM36_W(pEm)->Flg &= ~0x100;
    }
}

/* motion: run to the last seen player position */
void func_80102608(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
                EM36_W(pEm)->Lost_dir -= (SHORT)(Rnd_l() * 4 - 0xA00);
                EM36_W(pEm)->Unk1D = 0;
                EM36_W(pEm)->Flg |= 0x100;
                EM36_W(pEm)->Unk20 = Rnd_l() & 7;
            }
        }
        if (pEm->Col.Hit_flg & 0x10 && !EM36_W(pEm)->Lost_time) {
            if ((pEm->UnkDA += 20) > 150) {
                pEm->UnkDA = 0;
                EM36_W(pEm)->Lost_dir -= (SHORT)(Rnd_l() * 4 - 0xA00);
                EM36_W(pEm)->Unk1D = 0;
                EM36_W(pEm)->Flg |= 0x100;
                EM36_W(pEm)->Unk20 = Rnd_l() & 7;
            }
        }
        if (EM36_W(pEm)->Lost_time) {
            EM36_W(pEm)->Lost_time--;
            if (pEm->Col.Hit_flg & 0x10 || !EM36_W(pEm)->Lost_time) {
                EM36_W(pEm)->Unk1D = 0;
                EM36_W(pEm)->Flg |= 0x100;
                EM36_W(pEm)->Unk20 = Rnd_l() & 7;
            }
        }
    }
}

/* brain: walk along the route (type 8) */
void func_80102894(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;
    ULONG Dist_pl; /* never set in the original */
    SHORT Dir_wide, Dir, Dir_far;

    Dir_wide = Goto00_ck(pEm, pV->vx, pV->vz, 0x400);
    Dir = Goto00_ck(pEm, pV->vx, pV->vz, 0x100);
    Dir_far = Goto00_ck(pEm, pV->vx, pV->vz, 0x600);
    Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 9000 && !Dir)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (!EM36_W(pEm)->Unk08 && Dist < 2500 && !Dir)
        EM_SET_RTN(pEm, 1, 0x13, 0, 0);
    if (Dist_pl < 2000 && !Dir_wide)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
}

/* brain: walk along the route (type 12) */
void func_801029E0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;
    ULONG Dist_pl; /* never set in the original */
    SHORT Dir_wide, Dir, Dir_far;

    Dir_wide = Goto00_ck(pEm, pV->vx, pV->vz, 0x400);
    Dir = Goto00_ck(pEm, pV->vx, pV->vz, 0x100);
    Dir_far = Goto00_ck(pEm, pV->vx, pV->vz, 0x600);
    Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 9000 && !Dir)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir_far)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist_pl < 2000 && !Dir_wide)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
}

/* brain: attack wait */
void func_80102B00(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
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

void func_80102BD4(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    if (Dist < 3000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

void func_80102C44(ENEMY_WORK *pEm)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;

    Goto00_ck(pEm, pV->vx, pV->vz, 800);
    Goto00_ck(pEm, pV->vx, pV->vz, 0x200);
    if (Dist < 6000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* motion: turn */
void func_80102CC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
                Side = Rnd_l() & 1;
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
void func_80102E70(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir, Dir_atk, Dir_eye;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir_atk = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Dir_eye = Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (Dist > 3000 && !Dir)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist > 5000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 4000 && Dir) {
        EM_SET_RTN(pEm, 1, 4, 0, 0);
        if (Dist < 3000 && !Dir_eye && (Rnd_l() & 3) == 1 && !Flg_ck(&G.Unk7888, 0x1F))
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (!EM36_W(pEm)->Unk08 && Dist < 3000 && !Dir_atk && !pPl->Damage_cnt)
        EM_SET_RTN(pEm, 1, 5, 0, 0);
    if (!EM36_W(pEm)->Unk14 && Dist < 4500 && !Dir_atk && !pPl->Damage_cnt && !Flg_ck(&G.Unk7888, 0x1E) && EM36_W(pEm)->Flg & 0x800) {
        EM_SET_RTN(pEm, 1, 0xA, 0, 0);
        EM36_W(pEm)->Flg &= ~0x200;
    }
}

void func_8010304C(ENEMY_WORK *pEm)
{
    VECTOR *pV = &Em36_route_pos[EM36_W(pEm)->Route_no];
    ULONG Dist = EM36_W(pEm)->Route_dist;

    Goto00_ck(pEm, pV->vx, pV->vz, 800);
    Goto00_ck(pEm, pV->vx, pV->vz, 0x200);
    if (Dist > 3000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* motion: walk back */
void func_801030C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

void func_801031E0(void) {
}

/* motion: turn around */
void func_801031E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

void func_8010332C(void) {
}

/* motion: punch */
void func_80103334(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        if (Rnd_l() & 1) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(14, pEm);
            else
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
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
            EM36_W(pEm)->Unk08 = 30;
            EM36_W(pEm)->Unk14 += 5;
            EM36_W(pEm)->Unk06 += 10;
            if (pEm->Type & 1) {
                EM36_W(pEm)->Unk08 += 10;
                EM36_W(pEm)->Unk06 += 10;
            }
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            if (pEm->UnkE2 && !(pEm->Type & 1))
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            if (!Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->Unk154 < 3000 && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            if (!(pEm->UnkE3 & 0x80))
                EM36_W(pEm)->Unk22++;
        }
        Add_speedXZ(pEm, 0);
        if (!(pEm->UnkE3 & 0x80)) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 16);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 16);
            }
            Dist = pEm->Unk154;
            if (pEm->Move_cnt >= 11 && pEm->Move_cnt < 20 && (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) || Dist > 4500) && !(EM36_W(pEm)->Flg & 0x80)) {
                EM_SET_RTN(pEm, 1, 1, 0, 0);
                EM36_W(pEm)->Unk08 = 30;
                EM36_W(pEm)->Flg &= ~1;
                EM36_W(pEm)->pSub->Routine_0 = 1;
                EM36_W(pEm)->pSub->Routine_1 = 0;
                EM36_W(pEm)->pSub->Routine_2 = 0;
                pEm->Neck_flg &= ~3;
                if (!Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->Unk154 < 3000 && !(Rnd_l() & 3) && !(EM36_W(pEm)->Flg & 0x80) && !Flg_ck(&G.Unk7888, 0x1F))
                    EM_SET_RTN(pEm, 1, 0xF, 0, 0);
                EM36_W(pEm)->Unk22++;
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
            Hit = func_80044420(&pP[1].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[2].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
            if (Hit) {
                EM_RTN(pPl) = ((Cdir_ck_l(pEm, pPl) + 2) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                if (pPl->Unk12C == 0x80) {
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
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_punch_box1;
        Em36_hit_area.Id = 2;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
    }
}

/* brain: attack */
void func_80103A20(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir, Dir_near, Dir_eye;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    Dir_near = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Dir_eye = Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (!EM36_W(pEm)->Unk08) {
        if (Dist < 3000 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (!EM36_W(pEm)->Unk08 && Dist < 3000 && !Dir_eye && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
    }
    if (EM36_W(pEm)->Unk08 < 10 && Dist < 9000 && !Dir_near && !pPl->Damage_cnt && EM36_W(pEm)->Unk15 >= 2 && Dist > 3500)
        EM_SET_RTN(pEm, 1, 0x12, 0, 0);
}

void func_80103B9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        pEm->UnkE3 = (Rnd_l() & 0x1F) + 45;
        EM36_W(pEm)->Flg |= 0x40;
        pEm->Be_flg |= 0x8000;
    case 1:
        if (--pEm->UnkE3 == -1) {
            pEm->UnkE3 = (Rnd_l() & 0x3F) + 90;
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(14, pEm);
            else
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
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
                Rot = Rnd_l() & 1;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x40 - Rot * 120;
            }
        } else {
            pEm->UnkDA = 0;
        }
        if (!EM36_W(pEm)->Unk1A)
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
        pEm->Life = EM36_W(pEm)->Unk18;
        EM36_W(pEm)->Unk18 -= 250;
        pEm->Max_life = EM36_W(pEm)->Unk18 + 200;
        pEm->Damage_cnt |= 0x80;
    case 3:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 30) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 8, 0, 0);
            pEm->Timer0 = (Rnd_l() & 0x7F) + 150;
            EM_SET_MOVE(pEm, 0x20, 0, 0x1F);
            pEm->Damage_cnt &= 0x7F;
            EM36_W(pEm)->Flg |= 3;
            pEm->Status_flg |= 0x800;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
    if (EM36_W(pEm)->Unk1A)
        EM36_W(pEm)->Unk1A--;
    func_80103F88(pEm);
}

/* effects of the punch motion (part 5) */
void func_80103F88(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 1) {
        Ofs.vx = 11;
        Ofs.vy = (Rnd_l() & 0xF) + 0x10C;
        Ofs.vz = 50;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[5].Workm;
        Esp_call(0x4001700, ((Rnd_l() * 4 + 0x1000) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
        Esp_call(0x4000500, 0xA000000, pM, &Ofs, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && Rnd_l() & 1) {
        Ofs.vx = 11;
        Ofs.vy = 300;
        Ofs.vz = 50;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[5].Workm;
        Esp_call(0x2001700, ((Rnd_l() * 4 + 0x1000) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
        Esp_call(0x2000500, 0xA000000, pM, &Ofs, NULL);
    }
}

void func_801040E4(void) {
}

/* motion: roar */
void func_801040EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Unused[2];

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 0x10, 0, 0xF);
        Snd_se_enem((Rnd_l() & 1) ? 15 : 14, pEm);
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

void func_801041BC(void) {
}

/* motion: stand up */
void func_801041C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

void func_8010435C(void) {
}

/* motion: step aside */
void func_80104364(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

void func_801044BC(void) {
}

/* motion: lunge (tentacle stab) */
void func_801044C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            if ((Rnd_l() & 3) == 2) {
                pEm->Spd_x = 5;
                pEm->Spd_z = 5;
            }
        }
        if (EM36_W(pEm)->Flg & 0x20) {
            pEm->Move_cnt = 10;
            EM36_W(pEm)->pSub->Move_cnt = 10;
        }
        if (EM36_W(pEm)->Unk22 > 10) {
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
            if ((Rnd_l() & 3) == 1)
                Snd_se_enem(15, pEm);
            else if ((Rnd_l() & 3) == 2)
                Snd_se_enem(11, pEm);
        } else {
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        }
    case 1:
        if (pEm->Move_cnt >= 9 && pEm->Move_cnt < 30) {
            if (!(EM36_W(pEm)->Flg & 0x80)) {
                if (EM36_W(pEm)->Flg & 0x800)
                    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 24);
            } else {
                Goto01(pEm, EM36_W(pEm)->Lost_dir, 24);
            }
            EM36_W(pEm)->pSub->Timer0 = pEm->Unk154;
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
            if (Rnd_l() & 3 == 1) /* sic: always false */
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            pEm->Neck_flg &= ~3;
            EM36_W(pEm)->Flg &= ~1;
            EM36_W(pEm)->Unk14 = (Rnd_l() & 0x1F) - 0x6A;
            EM36_W(pEm)->Unk08 += 10;
            EM36_W(pEm)->Unk06 += 10;
            if (pEm->Type & 1) {
                EM36_W(pEm)->Unk08 += 10;
                EM36_W(pEm)->Unk06 += 10;
                EM36_W(pEm)->Unk14 = (Rnd_l() & 0xF) + 90;
            }
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            if (!Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->Unk154 < 3000
                && Rnd_l() & 1 && !(EM36_W(pEm)->Flg & 0x80) && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            if (!(pEm->UnkE3 & 0x80))
                EM36_W(pEm)->Unk22 += 2;
        }
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x200 && !pPl->Damage_cnt && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40) && EM36_W(pEm)->Flg & 0x800) {
            pEm->UnkE3 = 0x81;
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Ofs.pad = 500;
            pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
            Hit = func_80044420(&pP[1].Workm, &Ofs, pPl) | func_80044420(&pP[2].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
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
            && (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) || (LONG)pEm->Unk154 > 5500) && !(EM36_W(pEm)->Flg & 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM36_W(pEm)->Unk14 = 10;
            EM36_W(pEm)->Flg &= ~1;
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            pEm->Neck_flg &= ~3;
            if (!Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->Unk154 < 3000
                && Rnd_l() & 1 && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            EM36_W(pEm)->Unk22 += 3;
            return;
        }
    }
    if (*pEm->pNow_seq & 0x400) {
        Em36_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
        Em36_hit_area.pBox = Em36_lunge_box0;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 1;
        if (!(Rnd_l() & 1))
            Em36_hit_area.Id = 6;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_lunge_box1;
        Em36_hit_area.Id = 2;
        if (!(Rnd_l() & 1))
            Em36_hit_area.Id = 7;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
}

void func_80104CA4(void) {
}

/* motion: grab and squeeze the player */
void func_80104CAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        pPl->UnkDA = pEm->Unk154;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x800;
        pPl->Be_flg |= 0x800;
        EM_SET_RTN(pPl, 5, 0, 0, 0);
        EM36_W(pEm)->pSub->Routine_0 = 1;
        EM36_W(pEm)->pSub->Routine_1 = 4;
        EM36_W(pEm)->pSub->Routine_2 = 0;
        EM36_W(pEm)->Unk22 = 0;
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
            EM36_W(pEm)->Unk08 = 5;
            EM36_W(pEm)->Unk06 = 45;
            EM36_W(pEm)->Unk09 = 45;
            EM36_W(pEm)->Unk14 = 250;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            pEm->Damage_cnt &= 0x7F;
            if (pPl->Life < 50)
                EM36_W(pEm)->Unk14 = 60;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_80105114(void) {
}

INCLUDE_ASM("asm/nonmatchings/enemy/em36", func_8010511C);

void func_8010580C(void) {
}

/* motion: grab the player */
void func_80105814(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        pPl->Timer0 = pEm->Unk154;
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
                EM36_W(pEm)->Unk08 = 5;
                EM36_W(pEm)->Unk06 = 45;
                EM36_W(pEm)->Unk09 = 45;
            }
            if (((Rnd_l() & 7) == 1 | pPl->Life < 0) && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 700)
                && !Flg_ck(&G.Unk7888, 0x1E) && EM36_W(pEm)->Flg & 0x800) {
                EM_SET_RTN(pEm, 1, 0xA, 0, 0);
                EM36_W(pEm)->Flg = (EM36_W(pEm)->Flg & ~0x200) | 0x400;
            }
            if (EM36_W(pEm)->Flg & 0x1000) {
                EM_SET_RTN(pEm, 1, 0x14, 0, 0);
                pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Unk28];
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
            EM36_W(pEm)->Unk08 = 5;
            EM36_W(pEm)->Unk06 = 45;
            EM36_W(pEm)->Unk09 = 45;
        }
        break;
    }
}

void func_80105BF0(void) {
}

void func_80105BF8(void) {
}

void func_80105C00(void) {
}

/* motion: sweep the arm (tentacle) */
void func_80105C08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            if ((Rnd_l() & 3) == 1)
                Snd_se_enem(15, pEm);
            else if ((Rnd_l() & 3) == 2)
                Snd_se_enem(14, pEm);
        } else {
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        }
        if ((Rnd_l() >> (Rnd_l() & 3)) & 1) {
            pEm->Move_cnt = 10;
            pEm->Timer0 = 1;
            EM36_W(pEm)->pSub->Routine_3 = 1;
        }
        pEm->Routine_3 = 0;
        if (!Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x600, 0x4C8))
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
            if (!(Rnd_l() & 3) && !(pEm->Type & 1))
                EM_SET_RTN(pEm, 1, 7, 0, 0);
            EM36_W(pEm)->Unk08 = 30;
            EM36_W(pEm)->Unk29 = 180;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            pEm->Cdir_y += 0x400;
            EM36_W(pEm)->Flg &= ~3;
            EM36_W(pEm)->Unk06 += 10;
            EM36_W(pEm)->Unk14 += 5;
            if (!(pEm->UnkE2 & 0x80)) {
                EM36_W(pEm)->Unk22 += 2;
                if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) && (LONG)pEm->Unk154 < 4000) {
                    EM_SET_RTN(pEm, 1, 5, 0, 0);
                    if (Rnd_l() & 1) {
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
            Gy = func_8004D720(pP[1].Workm.t, 200, 0x2000, 0);
            if (pEm->Move_cnt < 30 && Gy < pP[1].Workm.t[1]) {
                EM_SET_RTN(pEm, 1, 0x11, 0, 0);
                return;
            }
            pEm->Cdir_y += pEm->Routine_3;
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Ofs.pad = 900;
            Hit = func_80044420(&pP[1].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[2].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
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
        Em36_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
        Em36_hit_area.pBox = Em36_sweep_box0;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 3;
        if (!(Rnd_l() & 3))
            Em36_hit_area.Id = 6;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
    if (pEm->Move_cnt >= 24 && pEm->Move_cnt < 29) {
        Em36_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
        Em36_hit_area.pBox = Em36_sweep_box1;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 3;
        if (!(Rnd_l() & 3))
            Em36_hit_area.Id = 6;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
    if (pEm->Move_cnt >= 26 && pEm->Move_cnt < 32) {
        Em36_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
        Em36_hit_area.pBox = Em36_sweep_box2;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 3;
        if (Rnd_l() & 1)
            Em36_hit_area.Id = 6;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_sweep_box3;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em36_hit_area, &Pl_work);
    }
}

void func_80106260(void) {
}

void func_80106268(void) {
}

void func_80106270(void) {
}

/* motion: swing */
void func_80106278(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        if (Rnd_l() & 1) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(12, pEm);
            else
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
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
            EM36_W(pEm)->Unk08 = 20;
            pEm->Neck_flg &= ~3;
        }
        Add_speedXZ(pEm, 0);
    }
}

void func_80106430(void) {
}

/* motion: swing the arm (tentacle) */
void func_80106438(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            Snd_se_enem((Rnd_l() & 1) ? 15 : 14, pEm);
        else
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
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
            EM36_W(pEm)->Unk08 = 15;
            EM36_W(pEm)->Flg &= ~1;
            pEm->Neck_flg &= ~3;
            if (!Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->Unk154 < 3000
                && !(Rnd_l() & 1) && !(EM36_W(pEm)->Flg & 0x80) && !Flg_ck(&G.Unk7888, 0x1F))
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            EM36_W(pEm)->Unk22 += 3;
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
            Hit = func_80044420(&pP[1].Workm, &Ofs, pPl) | func_80044420(&pP[2].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[5].Workm, &Ofs, pPl);
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
            Esp_call(0x1700, ((Rnd_l() * 2 + 0x1800) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Ofs, NULL);
        }
        break;
    }
    if (*pEm->pNow_seq & 0x400) {
        Em36_hit_area.pPos = (VECTOR *)EM36_W(pEm)->pSub->pSin_parts_ptr[3].Workm.t;
        Em36_hit_area.pBox = Em36_swing_box0;
        Em36_hit_area.Dir = pEm->Cdir_y;
        Em36_hit_area.Id = 1;
        if (Rnd_l() & 1)
            Em36_hit_area.Id = 3;
        pPos = &pEm->Pos_x;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
        Em36_hit_area.pBox = Em36_swing_box1;
        Em36_hit_area.Id = 2;
        if (Rnd_l() & 1)
            Em36_hit_area.Id = 3;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em36_hit_area, &Pl_work);
    }
}

void func_8010698C(void) {
}

/* motion: punch on the run (route type) */
void func_80106994(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            Snd_se_enem((Rnd_l() & 1) ? 15 : 11, pEm);
        else
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
    case 1:
        if (pEm->Move_cnt < 15) {
            Goto00(pEm, Em36_route_pos[EM36_W(pEm)->Route_no].vx, Em36_route_pos[EM36_W(pEm)->Route_no].vz, 24);
        }
        if (pEm->Move_cnt == 25) {
            Flg_on(&G.Unk7888, 0x20);
            if ((LONG)pEm->Unk154 > 6000)
                Flg_on(&G.Unk7888, 0x21);
            if ((LONG)pEm->Unk154 > 10000)
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
            EM36_W(pEm)->Unk08 = 30;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Neck_flg &= ~3;
            if (!Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4) && (LONG)pEm->Unk154 < 3000)
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
            Hit = func_80044420(&pP[1].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[2].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pEm->pSin_parts_ptr[4].Workm, &Ofs, pPl);
            if (Hit) {
                EM_RTN(pPl) = ((Cdir_ck_l(pEm, pPl) + 2) << 8) | 2;
                pPl->Damage_cnt |= 0x80;
                if (pPl->Unk12C == 0x80) {
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
void func_80106D98(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir_wide, Dir, Dir_near;

    Dir_wide = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x300);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x500);
    Dir_near = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Dir_pos_ck_l((VECTOR *)&pEm->Pos_x, (VECTOR *)&pPl->Pos_x, pEm->Cdir_y + 0x500, 0x3E4);
    if (!EM36_W(pEm)->Unk08) {
        if (Dist < 3000 && !Dir && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (!EM36_W(pEm)->Unk08) {
            if (Dist < 3000 && Dir_wide && !pPl->Damage_cnt)
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            if (!EM36_W(pEm)->Unk08 && Dist < 9000 && Dist > 6000 && !Dir_near && !pPl->Damage_cnt && EM36_W(pEm)->Unk15 >= 2)
                EM_SET_RTN(pEm, 1, 0x12, 0, 0);
        }
    }
    if (!(EM36_W(pEm)->Flg & 0x1000))
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* motion: follow the route */
void func_80106F2C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            if (Rnd_l() & 1)
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
        }
        Joint_move(pEm, pKan, pSeq, 0x40);
        Add_speedXZ(pEm, 0);
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Unk28];
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
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Unk28];
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
        pV = &Em36_route[pEm->Unk14E & 0xF][EM36_W(pEm)->Unk28];
        Dist = SquareRoot0((pV->vx - pEm->Pos_x) * (pV->vx - pEm->Pos_x) + (pV->vz - pEm->Pos_z) * (pV->vz - pEm->Pos_z));
        if (Dist > 1500
            && (LONG)pEm->Unk154 > 7000)
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

/* routine 2: damage (RE2 Em2a_damage) */
void func_80107390(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Lim;
    LONG Unused[2];

    if (!pEm->Routine_2) {
        if (pEm->Unk132 & 0x100)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x400)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x200)
            func_8010BBE8(pEm);
        if (pEm->Unk132 & 0x800)
            func_8010B804(pEm);
    }
    Lim = 400;
    if (G.System_flg & 0x100)
        Lim = 300;
    if (!(pEm->Type & 1)) {
        if (pEm->Life < Lim && !(EM36_W(pEm)->Flg & 0x20) && !pEm->Routine_2) {
            pEm->Unk12C = 32;
            pEm->Life = Lim;
            EM36_W(pEm)->Flg |= 0x20;
            if (!(EM36_W(pEm)->Flg & 2))
                EM36_W(pEm)->Flg &= ~3;
            else
                EM36_W(pEm)->Flg &= ~0x20;
        }
        if (EM36_W(pEm)->Flg & 1) {
            EM36_W(pEm)->Unk0C = 1;
            EM_RTN(pEm) = pEm->Unk1C4;
            Em34_blood_set(pEm);
            if (pEm->Routine_0 == 4)
                func_80108EA4(pEm, pKan, pSeq);
            else
                func_8010105C(pEm, pKan, pSeq);
            return;
        }
        Em36_dm[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
        return;
    }
    if (EM36_W(pEm)->Flg & 0x40 && !pEm->Routine_2 && pEm->Unk12C != 30) {
        pEm->Unk12C = 32;
        pEm->Life = EM36_W(pEm)->Unk18;
        if (!(EM36_W(pEm)->Flg & 2))
            EM36_W(pEm)->Flg &= ~3;
        pEm->Be_flg |= 0x8000;
    }
    if (EM36_W(pEm)->Unk18 > pEm->Life && !pEm->Routine_2) {
        pEm->Life = EM36_W(pEm)->Unk18;
        EM36_W(pEm)->Flg |= 0x40;
        EM36_W(pEm)->Unk1A = 450;
        if (!(EM36_W(pEm)->Flg & 2))
            EM36_W(pEm)->Flg &= ~3;
        pEm->Unk12C = 33;
    }
    if (!(EM36_W(pEm)->Flg & 2) && pEm->Unk12C == 30)
        EM36_W(pEm)->Flg &= ~3;
    if (EM36_W(pEm)->Flg & 8 && pEm->Unk12C == 30)
        EM36_W(pEm)->Flg &= ~3;
    if (EM36_W(pEm)->Flg & 1) {
        if (pEm->Unk12C == 30)
            EM36_W(pEm)->Flg |= 0x2000;
        if (EM36_W(pEm)->Flg & 1) {
            EM36_W(pEm)->Unk0C = 1;
            EM_RTN(pEm) = pEm->Unk1C4;
            Em34_blood_set(pEm);
            if (pEm->Routine_0 == 4)
                func_80108EA4(pEm, pKan, pSeq);
            else
                func_8010105C(pEm, pKan, pSeq);
            return;
        }
    }
    Em36_dm2[pEm->Unk12C - 1](pEm, pKan, pSeq);
}

/* damage: flinch */
void func_80107724(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Unused[2];

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = (Rnd_l() & 1) + 0x7000B;
        pEm->Routine_2 = 1;
        Em34_blood_set(pEm);
        if (pEm->Unk132 & 0x10)
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
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        } else {
            Snd_se_enem(13, pEm);
        }
        pEm->Be_flg &= ~0x8000;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (((LONG)pEm->Unk154 > 7000 || (Rnd_l() & 3) == 1) && !(pEm->Type & 1))
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            if ((UCHAR)pEm->Type == 8 && !(Rnd_l() & 3)) {
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
void func_80107990(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
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
            pEm->Life = EM36_W(pEm)->Unk18;
            EM36_W(pEm)->Unk18 -= 250;
            pEm->Max_life = EM36_W(pEm)->Unk18 + 200;
        }
        EM36_W(pEm)->Unk1A = 0;
        pEm->Hit_area.R = 600;
    case 1:
        if (pEm->Move_cnt == 40) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(13, pEm);
            else
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        }
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 8, 0, 0);
            pEm->Timer0 = (Rnd_l() & 0x7F) + 150;
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
void func_80107C60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM36_W(pEm)->Unk0C = 1;
    if ((UCHAR)pEm->Type != 3) {
        if (Rnd_l() & 1)
            Snd_se_enem(12, pEm);
    } else {
        Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
    }
    EM_RTN(pEm) = pEm->Unk1C4;
    Em34_blood_set(pEm);
    if (pEm->Routine_0 == 2) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (!(EM36_W(pEm)->Flg & 0x800) && Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        if (EM36_W(pEm)->Flg & 0x40)
            EM_SET_RTN(pEm, 1, 6, 0, 0);
    }
    func_8010105C(pEm, pKan, pSeq);
}

/* damage: lose a part (Unk15 = damage stage) */
void func_80107D5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        EM36_W(pEm)->Unk1A = 0;
        pEm->Hit_area.R = 600;
        Flg_on(&G.Unk7888, 0x1D);
        EM36_W(pEm)->Flg &= ~0x40;
        EM36_W(pEm)->Unk18 = pEm->Life - 250;
        if (EM36_W(pEm)->Unk18 < 0)
            EM36_W(pEm)->Unk18 = 0;
        pEm->Max_life = EM36_W(pEm)->Unk18 + 200;
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[1].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[3].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[14].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x8000000, &pEm->pSin_parts_ptr[12].Workm, NULL, NULL);
        EM36_W(pEm)->Flg &= ~0x2000;
        Snd_se_stad(0x10204, (VECTOR *)&pEm->Pos_x, 0, 0);
    case 1:
        switch (EM36_W(pEm)->Unk15) {
        case 0:
            func_801082BC(pEm);
            EM36_W(pEm)->Unk15 = 1;
            break;
        case 1:
            if (pEm->Move_cnt == 15) {
                func_8010832C(pEm);
                EM36_W(pEm)->Unk15 = 2;
            }
            break;
        case 2:
            if (pEm->Move_cnt == 15) {
                func_801084F8(pEm);
                EM36_W(pEm)->Unk15 = 3;
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
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        }
        if (pEm->Move_cnt == 45) {
            if ((UCHAR)pEm->Type != 3)
                Snd_se_enem(13, pEm);
            else
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
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
void func_80108134(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        EM36_W(pEm)->Unk1A = 0;
        pEm->Hit_area.R = 600;
    case 1:
        Spd_get2(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 8, 0, 0);
            pEm->Timer0 = (Rnd_l() & 0x7F) + 150;
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
void func_801082BC(ENEMY_WORK *pEm)
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
void func_8010832C(ENEMY_WORK *pEm)
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
    pP->Unk96 = 0;
    pP->Old_z = 0;
    pP->Unk85 = 9;
    pP->Unk90 = 0;
    pP->UnkA2 |= 0x400;
    pP->Unk92 = pEm->Cdir_y;
    pP->Unk8E = 0;
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
    pP->Unk96 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Unk85 = 9;
    pP->Unk90 = 0;
    pP->UnkA2 |= 0x400;
    pP->Unk92 = pEm->Cdir_y;
    pP->Unk8E = 0;
    pM8 = &pP->Workm;
    Esp_call(0x1C03, 0x10000000, pM8, NULL, NULL);
    Esp_call(0, 0x10000000, pM8, NULL, NULL);
}

/* blow off part 22 (copy of part 2) */
void func_801084F8(ENEMY_WORK *pEm)
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
    pP->Unk96 = 0;
    pP->Old_z = 0;
    pP->Unk85 = 9;
    pP->Unk90 = 0;
    pP->UnkA2 |= 0x400;
    pP->Unk92 = pEm->Cdir_y;
    pP->Unk8E = 0x338;
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
void func_801086F0(ENEMY_WORK *pEm)
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
    pP->Unk96 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Unk85 = 15;
    pP->Unk90 = 0;
    pP->Unk92 = 0;
    pP->Unk8E = 0;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM, NULL, NULL);
    Esp_call(0, 0x10000000, pM, NULL, NULL);

    pM2 = (MATRIX *)pEm->pSin_parts_ptr;
    pP = &((PARTS_W *)pM2)[5];
    ((PARTS_W *)pM2)[5].Be_flg |= 0xA;
    pM2 = &((PARTS_W *)pM2)[5].Workm;
    pP->Old_x = 0;
    pP->Unk96 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Unk85 = 15;
    pP->Unk90 = 0;
    pP->Unk92 = 0;
    pP->Unk8E = 0;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM2, NULL, NULL);
    Esp_call(0, 0x10000000, pM2, NULL, NULL);

    pP = EM36_W(pEm)->pSub->pSin_parts_ptr;
    pM3 = &pP->Workm;
    pP->Old_x = 0;
    pP->Unk96 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Unk85 = 15;
    pP->Unk90 = 0;
    pP->Unk92 = 0;
    pP->Unk8E = 0;
    pP->Be_flg |= 0xA;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM3, NULL, NULL);
    Esp_call(0, 0x10000000, pM3, NULL, NULL);

    pM4 = (MATRIX *)EM36_W(pEm)->pSub->pSin_parts_ptr;
    pP = &((PARTS_W *)pM4)[3];
    ((PARTS_W *)pM4)[3].Be_flg |= 0xA;
    pM4 = &((PARTS_W *)pM4)[3].Workm;
    pP->Old_x = 0;
    pP->Unk96 = 0;
    pP->Old_z = 0;
    pP->Old_y = 0;
    pP->Unk85 = 15;
    pP->Unk90 = 0;
    pP->Unk92 = 0;
    pP->Unk8E = 0;
    pP->UnkA2 |= 0x400;
    Esp_call(0x1C03, 0xA000000, pM4, NULL, NULL);
    Esp_call(0, 0x10000000, pM4, NULL, NULL);
}

/* routine 3: die */
void func_80108988(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        if (pEm->Unk132 & 0x100)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x400)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x200)
            func_8010BBE8(pEm);
        if (pEm->Unk132 & 0x800)
            func_8010B804(pEm);
    }
    if (EM36_W(pEm)->Flg & 2) {
        if (pEm->Unk12C == 30) {
            Em_dead_flg_on_cand(pEm);
            EM_SET_RTN(pEm, 4, 0xA, 0, 0);
            pEm->Life = -1;
        } else {
            pEm->Life = 0;
            pEm->Damage_cnt &= 0x7F;
            EM_RTN(pEm) = pEm->Unk1C4;
            Em34_blood_set(pEm);
            func_8010105C(pEm, pKan, pSeq);
        }
    } else {
        func_80108AC0(pEm, pKan, pSeq);
    }
}

/* die: fall down and burn */
void func_80108AC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        G.Unk785C = 0;
        G.Status_flg &= ~0x40000000;
        if (G.Status_flg & 0x2000)
            func_80037A90(pEm, 10);
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
                Snd_se_stad(0x1020F, (VECTOR *)&pEm->Pos_x, 0, 0);
        }
        if (pEm->Move_cnt < 104 && pEm->Move_cnt % 7 == 1) {
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            Ofs.vx = 100 - (Rnd_l() & 1) * 200;
            Ofs.vy = 0;
            Ofs.vz = 100 - (Rnd_l() & 1) * 200;
            pM = &((PARTS_W *)pM)[3].Workm;
            Esp_call(0x4000000, ((Rnd_l() * 8 + 0x1000) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
            Esp_call(0x4001700, ((Rnd_l() * 8 + 0x1000) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
            Esp_call(0x4000500, 0x10000000, pM, &Ofs, NULL);
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            Esp_call(0x4001700, ((Rnd_l() * 8 + 0xF00) << 16) | (USHORT)(Rnd_l() * 16), &((PARTS_W *)pM)[2].Workm, NULL, NULL);
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            pM = &((PARTS_W *)pM)[5].Workm;
            Esp_call(0x4001700, ((Rnd_l() * 8 + 0x800) << 16) | (USHORT)(Rnd_l() * 16), pM, NULL, NULL);
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

/* scenario move (RE2 Em2a_sce) */
void func_80108EA4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Type & 0x100) {
        func_80016600(pEm, pKan, pSeq);
        return;
    }
    Em36_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

/* sce: stand */
void func_80108F00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
void func_80108F9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
void func_80109050(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->Add_dir;
        EM36_W(pEm)->Flg |= 1;
    case 1:
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->Unk14A & 8 && G.Status_flg & 0x2000)
            func_80037A90(pEm, 30);
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
void func_80109164(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
void func_80109224(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->Add_dir;
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
void func_80109440(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = pEm->Add_dir + 0x700;
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

/* sce: jump with speed Unk14C / gravity Unk14E, land at Unk122 */
void func_801096B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = pEm->Add_dir;
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
        if (pEm->Pos_y > pEm->Unk122 - 100 && pEm->Spd_y > 0) {
            pEm->Routine_2 = 3;
            pEm->Pos_y = pEm->Unk122;
        }
        break;
    case 2:
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += pEm->Unk14E;
        if (pEm->Spd_y > 0)
            pEm->Unk14A |= 0x20;
        if (pEm->Pos_y > pEm->Unk122 - 100 && pEm->Spd_y > 0) {
            pEm->Routine_2 = 3;
            pEm->Pos_y = pEm->Unk122;
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
void func_80109900(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = pEm->Add_dir + 0xF0000;
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
void func_80109B30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pParts;
    PARTS_W *pP;
    MATRIX *pM;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->Add_dir + 0x70000;
        pEm->Damage_cnt |= 0x80;
        Snd_se_enem(13, pEm);
        if (EM36_W(pEm)->pSub) {
            EM36_W(pEm)->pSub->Routine_0 = 1;
            EM36_W(pEm)->pSub->Routine_1 = 0;
            EM36_W(pEm)->pSub->Routine_2 = 0;
            EM36_W(pEm)->pSub->Spd_y = 0x1000;
            EM36_W(pEm)->pSub->Spd_x = 0x200;
        }
        EM36_W(pEm)->Unk15 = 1;
        Esp_call(0x1C03, 0xA000000, &pEm->pSin_parts_ptr[2].Workm, NULL, NULL);
        Esp_call(0x1C03, 0x10000000, &pEm->pSin_parts_ptr[9].Workm, NULL, NULL);
        Snd_se_stad(0x10204, (VECTOR *)&pEm->Pos_x, 0, 0);
    case 1:
        if (!(pEm->Move_cnt % 60))
            func_801082BC(pEm);
        if (pEm->Move_cnt == 30) {
        pParts = pEm->pSin_parts_ptr;
        pP = &pParts[21];
        pM = &pParts[21].Workm;
        pParts[21].Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Unk96 = 0;
        pP->Old_z = 0;
        pP->Old_y = 0;
        pP->Unk85 = 5;
        pP->Unk90 = 0;
        pP->Unk92 = pEm->Cdir_y + 0x800;
        pP->Unk8E = -900;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        }
        if (pEm->Move_cnt == 60) {
        pParts = pEm->pSin_parts_ptr;
        pP = &pParts[20];
        pM = &pParts[20].Workm;
        pParts[20].Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Unk96 = 0;
        pP->Old_z = 0;
        pP->Old_y = 0;
        pP->Unk85 = 5;
        pP->Unk90 = 0;
        pP->Unk92 = pEm->Cdir_y + 0x600;
        pP->Unk8E = -724;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        pParts = pEm->pSin_parts_ptr;
        pP = &pParts[19];
        pM = &pParts[19].Workm;
        pParts[19].Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Unk96 = 0;
        pP->Old_z = 0;
        pP->Old_y = 0;
        pP->Unk85 = 5;
        pP->Unk90 = 0;
        pP->Unk92 = pEm->Cdir_y + 0x600;
        pP->Unk8E = -824;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        }
        if (pEm->Move_cnt == 80) {
        pP = &pEm->pSin_parts_ptr[16];
        pM = &pP->Workm;
        pP->Be_flg |= 0xA;
        pP->Old_y = 20;
        pP->Old_x = 0;
        pP->Unk96 = 0;
        pP->Old_z = 0;
        pP->Unk85 = 9;
        pP->Unk90 = 0;
        pP->Unk92 = pEm->Cdir_y - 0x32C;
        pP->Unk8E = -624;
        Esp_call(0x1C03, 0x8000000, pM, NULL, NULL);
        Esp_call(0x1700, 0x10000000, pM, NULL, NULL);
        }
        if (pEm->Move_cnt == 85) {
        pP = &pEm->pSin_parts_ptr[17];
        pM = &pP->Workm;
        pP->Be_flg |= 0xA;
        pP->Old_x = 0;
        pP->Unk96 = 0;
        pP->Old_z = 0;
        pP->Old_y = 30;
        pP->Unk85 = 9;
        pP->Unk90 = 0;
        pP->Unk92 = pEm->Cdir_y - 0x200;
        pP->Unk8E = -524;
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
        pP->Unk96 = 0;
        pP->Old_z = 0;
        pP->Unk85 = 9;
        pP->Unk90 = 0;
        pP->Unk92 = pEm->Cdir_y;
        pP->Unk8E = -424;
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
void func_80109FCC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
            if (EM36_W(pEm)->Unk15 < 2)
                func_8010832C(pEm);
            if (EM36_W(pEm)->Unk15 < 3) {
                func_801084F8(pEm);
                pEm->Type = (pEm->Type & 0xFFF0) | 3;
            }
        }
        if (pEm->Move_cnt == 90)
            func_801086F0(pEm);
        if (pEm->Move_cnt == 25)
            Snd_se_stad(0x1020D, (VECTOR *)&pEm->Pos_x, 0, 0);
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
void func_8010A29C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

/* routine 7: fade out the shadow */
void func_8010A53C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

/* die: dispatch */
void func_8010A684(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_die[pEm->Routine_1](pEm, pKan, pSeq);
}

/* player damage: grabbed and thrown */
void func_8010A6C0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        Snd_se_stad(0x10002, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk12C = 24;
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
        pEm->Unk12C = 0;
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
            Snd_se_stad(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
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
                pEm->Unk12C = 0x80;
                pEm->Damage_cnt &= 0x7F;
            } else {
                EM_SET_RTN(pEm, 7, 0, 0, 0);
            }
        }
        Add_speedXZ(pEm, 0x800);
        if (*pEm->pNow_seq & 0x200) {
            Snd_se_stad(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
            Snd_se_stad(0x10229, (VECTOR *)&pEm->Pos_x, 0, 0);
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
            pEm->Unk12C = 0;
        break;
    case 8:
        EM_SET_MOVE(pEm, 7, 0, 3);
        pEm->Routine_2 = 9;
        Snd_se_stad(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
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
void func_8010AE3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pE;
    SHORT Cdir;

    pE = pEm->pEnemy_ptr;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Neck_flg |= 0x12;
        Snd_se_stad(0x10001, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk12C = 24;
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
            Snd_se_stad(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
            Pad_vib_set0(3, 0);
            Pad_vib_set1(15, 150, 0);
            Pad_vib_fade_set(30, 150, 0, 15);
        }
        if (pEm->Move_cnt >= 16 && pEm->Move_cnt < 35 && pEm->Move_cnt & 3)
            Pad_vib_set0((Rnd_l() + 1) & 1, 0);
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

void func_8010B150(void) {
}

void func_8010B158(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em36_die2[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8010B194(void) {
}

/* dodge timer: step aside when the player aims and fires */
void func_8010B19C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = G_PLAYER;
    LONG Div;
    LONG Time;

    EM36_W(pEm)->Flg &= ~0x10;
    if (EM36_W(pEm)->Unk07 & 0x7F)
        EM36_W(pEm)->Unk07--;
    if ((EM36_FLGW(pEm) & 0xFF000000) == 0x80000000) {
        EM36_W(pEm)->Flg |= 0x10;
        EM36_W(pEm)->Unk07 &= 0x7F;
    }
    if (!(EM36_W(pEm)->Unk07 & 0x80) && *(USHORT *)&pPl->Routine_0 == 0x501 && !Cdir_ck_l(pEm, pPl)
        && !Goto00_ck(pPl, pEm->Pos_x, pEm->Pos_z, 0x80) && G.Unk75E8 & 1) {
        Div = 0;
        if ((UCHAR)pPl->Type == 10) {
            Div = 7000;
            if (!(Rnd_l() & 1))
                Div = 760;
        }
        if ((ULONG)((UCHAR)pPl->Type - 6) < 4) {
            Div = 7000;
            if (!(Rnd_l() & 1))
                Div = 640;
        }
        if (Div) {
            Time = (LONG)pEm->Unk154 / Div - 5;
            if (Time >= 0)
                EM36_W(pEm)->Unk07 = Time | 0x80;
        }
    }
}

/* head shake when hit (part 1) */
void func_8010B308(ENEMY_WORK *pEm)
{
    SVECTOR Rot;
    MATRIX M;
    MATRIX Unused;
    LONG Unused2[4];
    signed char Swing[8] = { 8, 12, -8, -12, -4, 0, 4, 0 };

    switch (EM36_W(pEm)->Unk0C) {
    case 0:
        break;
    case 1:
        EM36_W(pEm)->Unk0F = Swing[Rnd_l() & 7];
        EM36_W(pEm)->Unk0C = 2;
        EM36_W(pEm)->Unk0D = 0;
        EM36_W(pEm)->Unk0E = 1;
        if (pEm->Unk132 & 0x10)
            EM36_W(pEm)->Unk0E = -1;
    case 2: {
        PARTS_W *pP = pEm->pSin_parts_ptr;

        Rot.vx = 0;
        Rot.vy = -(EM36_W(pEm)->Unk0D * EM36_W(pEm)->Unk0F * 8) * EM36_W(pEm)->Unk0E;
        Rot.vz = -(EM36_W(pEm)->Unk0D * 64) * EM36_W(pEm)->Unk0E;
        RotMatrix(&Rot, &M);
        MulMatrix((MATRIX *)pP[1].M, &M);
        if (EM36_W(pEm)->Unk0D++ >= 3) {
            EM36_W(pEm)->Unk0C = 3;
            EM36_W(pEm)->Unk0D = 16;
        }
        break;
    }
    case 3: {
        PARTS_W *pP = pEm->pSin_parts_ptr;

        Rot.vx = 0;
        Rot.vy = -(EM36_W(pEm)->Unk0D * EM36_W(pEm)->Unk0F) * EM36_W(pEm)->Unk0E;
        Rot.vz = -(EM36_W(pEm)->Unk0D * 8) * EM36_W(pEm)->Unk0E;
        RotMatrix(&Rot, &M);
        MulMatrix((MATRIX *)pP[1].M, &M);
        if (--EM36_W(pEm)->Unk0D == -1)
            EM36_W(pEm)->Unk0C = 0;
        break;
    }
    }
}

#include "enemy/common/em34/Em34_foot_set.inc.c"

#include "enemy/common/em34/Em34_blood_set.inc.c"

void func_8010B7FC(void) {
}

/* knocked back by a strong hit: lean the body away from the player */
void func_8010B804(ENEMY_WORK *pEm)
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
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Unk178);
    pEm->Unk17E = 0x230;
    if ((pEm->Unk132 & 3) == 1) {
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, (Rnd_l() * 8 + 0x800) << 16, &pP[10].Workm, NULL, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, (Rnd_l() * 8 + 0x800) << 16, &pP[14].Workm, NULL, NULL);
    } else {
        pP2 = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd_l();
        Sv.vy = -0xAC - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(0x1C03, (Rnd_l() * 8 + 0x800) << 16, &pP2[9].Workm, &Sv, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1C03, (Rnd_l() * 8 + 0x800) << 16, &pP[2].Workm, NULL, NULL);
    }
    pP3 = &pEm->pSin_parts_ptr[0];
    pP3->UnkA2 |= 0x40;
    pP3 = &pEm->pSin_parts_ptr[9];
    pP3->UnkA2 |= 0x40;
}

#include "enemy/common/em34/Em34_blood_spray.inc.c"

/* knocked back (weaker variant of func_8010B804) */
void func_8010BBE8(ENEMY_WORK *pEm)
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
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Unk178);
    pEm->Unk17E = 0x40;
    if ((pEm->Unk132 & 3) == 1) {
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1803, (Rnd_l() * 8 + 0x800) << 16, &pP[10].Workm, NULL, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1803, (Rnd_l() * 8 + 0x800) << 16, &pP[14].Workm, NULL, NULL);
    } else {
        pP2 = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd_l();
        Sv.vy = -0xAC - Rnd_l();
        Sv.vz = 0x80 - Rnd_l();
        Esp_call(0x1803, (Rnd_l() * 8 + 0x800) << 16, &pP2[9].Workm, &Sv, NULL);
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x1803, (Rnd_l() * 8 + 0x800) << 16, &pP[2].Workm, NULL, NULL);
    }
    pP3 = &pEm->pSin_parts_ptr[0];
    pP3->UnkA2 |= 0x40;
    pP3 = &pEm->pSin_parts_ptr[9];
    pP3->UnkA2 |= 0x40;
}

/* morph of part 2 (mouth) driven by seq flag 0x800 */
void func_8010BDD0(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG Mode;

    EM36_W(pEm)->Unk10 = 0;
    pP = &pEm->pSin_parts_ptr[2];
    if (*pEm->pNow_seq & 0x800)
        EM36_W(pEm)->Unk10 = 1;
    switch (Mode = EM36_W(pEm)->Unk10) {
    case 0:
        EM36_SABUN_P(pP)->Key[0].Rate -= 0x100;
        if (EM36_SABUN_P(pP)->Key[0].Rate <= 0)
            EM36_SABUN_P(pP)->Key[0].Rate = 0;
        break;
    case 1:
        EM36_SABUN_P(pP)->Key[0].Rate += 0x100;
        if (EM36_SABUN_P(pP)->Key[0].Rate >= 0x1000) {
            EM36_SABUN_P(pP)->Key[0].Rate = 0x1000;
            EM36_W(pEm)->Unk10 = Mode;
        }
        break;
    }
    func_8004A934_(pEm, pP, Mode);
}

/* morph of parts 16..21 (cycle through the 7 keys) */
void func_8010BEAC(ENEMY_WORK *pEm)
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
        switch (pP->Unk74) {
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
                pP->Unk74 = 1;
                if (Rnd_l() & 1)
                    pP->Unk74 = 3;
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
                pP->Unk74 = 2;
                if (Rnd_l() & 1)
                    pP->Unk74 = 4;
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
                pP->Unk74 = 3;
                if (Rnd_l() & 1)
                    pP->Unk74 = 5;
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
                pP->Unk74 = 4;
                if (Rnd_l() & 1)
                    pP->Unk74 = 6;
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
                pP->Unk74 = 5;
                if (Rnd_l() & 1)
                    pP->Unk74 = 0;
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
                pP->Unk74 = 6;
                if (Rnd_l() & 1)
                    pP->Unk74 = 1;
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
                pP->Unk74 = 0;
                if (Rnd_l() & 1)
                    pP->Unk74 = 2;
            }
            break;
        }
        func_8004A934(pEm, pP);
        pP++;
    } while (--i);
}

/* effects of parts 1 and 4 */
void func_8010C4FC(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 1 && Rnd_l() & 1) {
        Ofs.vx = -295;
        Ofs.vy = -65;
        Ofs.vz = 240;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd_l() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
        Ofs.vx = 7;
        Ofs.vy = 380;
        Ofs.vz = 118;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[4].Workm;
        Esp_call(0x1700, (Rnd_l() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && Rnd_l() & 1) {
        Ofs.vx = -143;
        Ofs.vy = 340;
        Ofs.vz = -2;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[4].Workm;
        Esp_call(0x1700, (Rnd_l() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
        Ofs.vx = -268;
        Ofs.vy = -510;
        Ofs.vz = -300;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd_l() + 0x800) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
}

/* effects of part 1 */
void func_8010C6CC(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 2 && Rnd_l() & 1) {
        Ofs.vy = -450;
        Ofs.vx = 0;
        Ofs.vz = 400;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd_l() + 0x1000) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
}

/* effects of part 1 */
void func_8010C764(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if ((pEm->Move_cnt & 7) == 5 && Rnd_l() & 1) {
        Ofs.vx = 20;
        Ofs.vy = -700;
        Ofs.vz = 0;
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[1].Workm;
        Esp_call(0x1700, (Rnd_l() + 0x1400) << 16, pM, &Ofs, NULL);
        Esp_call(0x1800, 0xA000000, pM, &Ofs, NULL);
    }
}
