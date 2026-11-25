#include "enemy/em34.h"
#include "door.h"
#include "root_ck.h"
#include "sabun.h"
#include "sca.h"
#include "snd.h"
#include "spl_auto.h"

extern LONG D_800D1F2C_ __asm__("D_800D1F2C"); /* type differs from the canonical extern */
LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
void func_8002646C_(ENEMY_WORK *pEm, LONG A, LONG B, LONG C) __asm__("func_8002646C"); /* arg 2 long (canonical short); arg 3 long (canonical short); arg 4 long (canonical UC) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");      /* em_sub.c, result used as int */

/* module .data (names after RE2 EM2A.C) */
/* arm models: the claw arm from pUnk174 and the original arm, parts 5 and 8 */
      /* .rodata, shared by two motions */    /* .rodata, shared by two player routines */

/* @aliases-end */

/* RE2 Em12a: enemy main routine */
void Em134(ENEMY_WORK *pEm)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pPl = PLAYER_EM(pG);
    VECTOR *pTv = (VECTOR *)&pG->Player.Pos_x;
    PARTS_W *pP;
    SVECTOR Pos;
    SVECTOR Spd;

    pG->Unk75EA &= 0xFF00;
    if ((LONG)pG->Status_flg < 0) {
        if ((pEm->Unk158 & 0x3FFFFFFF) < (LONG)pEm->Unk154 >> 1) {
            EM34_W(pEm)->Target_time = 60;
            EM34_W(pEm)->Flg |= 0x100;
            EM34_W(pEm)->pTarget = pG->pEnemy[1];
        }
        if ((pEm->Unk158 & 0x3FFFFFFF) < (LONG)pEm->Unk154) {
            if (++EM34_W(pEm)->Near_cnt > 150) {
                EM34_W(pEm)->Target_time = 60;
                EM34_W(pEm)->Flg |= 0x100;
                EM34_W(pEm)->pTarget = pG->pEnemy[1];
            }
            if (EM34_W(pEm)->Near_cnt > 60 && (LONG)pEm->Unk154 < 3500) {
                EM34_W(pEm)->Target_time = 90;
                EM34_W(pEm)->Flg |= 0x100;
                EM34_W(pEm)->pTarget = pG->pEnemy[1];
            }
        } else {
            EM34_W(pEm)->Near_cnt = 0;
        }
    } else {
        EM34_W(pEm)->pTarget = pPl;
    }
    if (EM34_W(pEm)->Flg & 0x100) {
        G.Unk75EA |= 4;
        pTv = (VECTOR *)&pG->pEnemy[1]->Pos_x;
    }
    Root_ck(pEm, pTv, 0, 0);
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em34_before(pEm);
    Em34_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Em34_after0(pEm);
    Em34_after1(pEm);
    Em34_after2(pEm);
    Em34_after3(pEm);
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
            pP = &pEm->pSin_parts_ptr[1];
            pEm->Be_flg &= ~8;
            pP->UnkA2 &= ~0x40;
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 &= ~0x40;
        }
    }
    if (EM34_W(pEm)->Unk00)
        EM34_W(pEm)->Unk00--;
    if (EM34_W(pEm)->Unk06)
        EM34_W(pEm)->Unk06--;
    if (EM34_W(pEm)->Unk10)
        EM34_W(pEm)->Unk10--;
    if (EM34_W(pEm)->Unk12)
        EM34_W(pEm)->Unk12--;
    if (EM34_W(pEm)->Unk22)
        EM34_W(pEm)->Unk22--;
    if (D_800D1F2C_ < 0) {
        if (EM34_W(pEm)->Unk00)
            EM34_W(pEm)->Unk00--;
        if (EM34_W(pEm)->Unk10)
            EM34_W(pEm)->Unk10--;
        if (EM34_W(pEm)->Unk12)
            EM34_W(pEm)->Unk12--;
    }
    if (EM34_W(pEm)->Target_time) {
        if (--EM34_W(pEm)->Target_time == 0) {
            EM34_W(pEm)->pTarget = &Pl_work;
            EM34_W(pEm)->Flg &= ~0x100;
        }
    }
    if (pEm->Col.Unk3 != 0xFF) {
        EM34_W(pEm)->Hit_cnt++;
        if (pEm->Col.Unk3 >= 2)
            EM34_W(pEm)->Hit_cnt++;
    } else {
        EM34_W(pEm)->Hit_cnt = 0;
    }
    if (pEm->Water && (*pEm->pNow_seq & 0xF000) == 0x2000) {
        pP = pEm->pSin_parts_ptr;
        Pos.vx = pP->Workm.t[0];
        Pos.vy = pEm->Water;
        Pos.vz = pP->Workm.t[2];
        Spd.vx = 0x400;
        Spd.vy = 0;
        Spd.vz = 0;
        Esp_call(0xE, (USHORT)pEm->Cdir_y | 0x20000000, &GsIDMATRIX, &Pos, &Spd);
    }
    func_8001AECC(pEm, 0xFE700002, 0x190000C);
    pEm->pEnemy_ptr = pPl;
    Rot_neck_em(pEm, pEm->Cdir_y);
    Em_seq_sound(pEm);
}

INCLUDE_ASM("asm/nonmatchings/enemy/em34", Em34_Init);

/* RE2 Em2a_Move: the motion routines are chosen by the low byte of Type */
void Em34_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em34_Move_type[(UCHAR)pEm->Type](pEm, pKan, pSeq);
}

void Em34_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (D_800D1F2C_ >= 0)
        Em34_mv_br_type00[pEm->Routine_1](pEm);
    else
        Em34_mv_br_type00h[pEm->Routine_1](pEm);
    Em34_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

void Em34_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (D_800D1F2C_ >= 0)
        Em34_mv_br_type01[pEm->Routine_1](pEm);
    else
        Em34_mv_br_type01h[pEm->Routine_1](pEm);
    Em34_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

void Em34_Move_type02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em34_mv_br_type02[pEm->Routine_1](pEm);
    Em34_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* debug: shows the explosion timer and blows up the arm/body parts when it runs out */
void Em34_Move_type03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX *pM;
    MATRIX *pM2;
    SVECTOR Ofs;

    Print_color(1, 0, 0);
    Print8(0x40, 0x80, "F = %d", EM34_W(pEm)->Unk14);
    if (EM34_W(pEm)->Unk14) {
        if (--EM34_W(pEm)->Unk14 == 0) {
            pM = &pEm->pSin_parts_ptr[6].Workm;
            func_8001B894(5, pM);
            func_8001B894(0x1203, pM);
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Esp_call(0x205, 0x10000000, pM, &Ofs, NULL);
            Esp_call(0x1001B03, 0x10000000, pM, &Ofs, NULL);
            Ofs.vy = 300;
            Esp_call(0x205, 0xA000000, pM, &Ofs, NULL);
            pM2 = &pEm->pSin_parts_ptr[1].Workm;
            func_8001B894(5, pM2);
            Ofs.vx = 100;
            Ofs.vy = -400;
            Ofs.vz = 300;
            Esp_call(0x205, 0x12000000, pM2, &Ofs, NULL);
            Ofs.vx = -100;
            Ofs.vy = -300;
            Ofs.vz = 300;
            Esp_call(0x205, 0x12000000, pM2, &Ofs, NULL);
        }
    }
    Em34_mv_br_type03[pEm->Routine_1](pEm);
    Em34_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* brain: idle */
void Em34_taiki_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Dir = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400);
    if (Dist < 3000 && Dir)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (pEm->Status_flg & 0x2000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (pEm->Damage_cnt)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

void Em34_taiki_br_h(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Dir = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400);
    if (Dist < 3000 && Dir)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (!Dir && (pEm->Status_flg & 0x2000))
        EM_SET_RTN(pEm, 1, 1, 0, 0);
}

/* motion: idle */
void Em34_taiki(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* brain: walk (Type 0) */
void Em34_walk_br0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir4 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    SHORT Dir2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    SHORT Dir6 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);

    if (Dist > 8000 && !Dir2 && (pEm->Status_flg & 0x2000) && !EM34_W(pEm)->Unk22) {
        EM_SET_RTN(pEm, 1, 2, 0, 0);
        if ((Rnd_l() & 1) && !EM34_W(pEm)->Unk10) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            return;
        }
    }
    if (!EM34_W(pEm)->Unk10 && Dist > 5000 && !Dir4 && (pEm->Status_flg & 0x2000))
        EM_SET_RTN(pEm, 1, 0xA, 0, 0);
    if (EM34_W(pEm)->Hit_cnt > 60) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg |= 0x800;
    }
    if (Dist < 5000 && Dir6)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist < 1300 && !Dir2)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
        if (EM34_W(pEm)->Unk13 < 0) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM34_W(pEm)->Unk13 = (Rnd_l() & 1) + 2;
        }
    }
    if (EM34_W(pEm)->Flg & 0x40)
        EM_SET_RTN(pEm, 1, 0x10, 0, 0);
    if (!EM34_W(pEm)->Unk00 && (pPl->Be_flg & 0x20000000)) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
    if (pEm->nFloor + 1 == pPl->nFloor && EM34_W(pEm)->Unk00 < 5 && !Dir4) {
        EM_SET_RTN(pEm, 1, 0xA, 0, 0);
        EM34_W(pEm)->Unk13 = (Rnd_l() & 1) + 2;
    }
}

/* brain: walk (Type 1) */
void Em34_walk_br1(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir4 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    SHORT Dir2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    SHORT Dir6 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);

    if (Dist > 8000 && !Dir2 && (pEm->Status_flg & 0x2000) && !EM34_W(pEm)->Unk22)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist > 2500 && !(pEm->Status_flg & 0x2000) && !EM34_W(pEm)->Unk12 && !EM34_W(pEm)->Unk22)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (EM34_W(pEm)->Hit_cnt > 60) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg |= 0x800;
    }
    if (Dist < 5000 && Dir6)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist < 1300 && !Dir2)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (pEm->Pos_y == pPl->Pos_y && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
        if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            EM34_W(pEm)->Flg &= ~0x800;
        }
        if (!EM34_W(pEm)->Unk06 && Dist < 1500 && !Dir2 && !pPl->Damage_cnt) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
            EM34_W(pEm)->Flg &= ~0x400;
        }
        if (Dist < 2000 && !Dir2 && pPl->Routine_0 == 5 && pPl->Unk12C)
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    } else if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
    if (EM34_W(pEm)->Flg & 0x40)
        EM_SET_RTN(pEm, 1, 0x10, 0, 0);
    if ((!EM34_W(pEm)->Unk00 || !EM34_W(pEm)->Unk06) && (pPl->Be_flg & 0x20000000)) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
    if (pEm->nFloor + 1 == pPl->nFloor && EM34_W(pEm)->Unk00 < 5 && !Dir4 && Dist < 2000) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* brain: walk (Type 3) */
void Em34_walk_br3(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir4 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    SHORT Dir2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    SHORT Dir6 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    SHORT Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0xD4);

    if (Dist > 8000 && !Dir2 && (pEm->Status_flg & 0x2000))
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist > 2500 && !(pEm->Status_flg & 0x2000) && !EM34_W(pEm)->Unk12)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir6)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist < 1300 && !Dir2)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (pEm->Pos_y == pPl->Pos_y && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
        if (!EM34_W(pEm)->Unk00 && Dist < 3000 && !Dir2 && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0x11, 0, 0);
        if ((!EM34_W(pEm)->Unk00 || pPl->Routine_1 == 5) && Dist > 3000 && !Dir1 && Dist < 5500 && !pPl->Damage_cnt)
            EM_SET_RTN(pEm, 1, 0x12, 0, 0);
        if (!EM34_W(pEm)->Unk06 && Dist < 1500 && !Dir2 && !pPl->Damage_cnt) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
            EM34_W(pEm)->Flg &= ~0x400;
        }
        if (Dist < 2000 && !Dir2 && pPl->Routine_0 == 5)
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    } else if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
    if (EM34_W(pEm)->Flg & 0x40)
        EM_SET_RTN(pEm, 1, 0x10, 0, 0);
    if ((!EM34_W(pEm)->Unk00 || !EM34_W(pEm)->Unk06) && (pPl->Be_flg & 0x20000000)) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* brain: walk (Type 0, D_800D1F2C < 0), chases the current target */
void Em34_walk_br0h(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir4;
    SHORT Dir2;
    SHORT Dir6;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Dir4 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400);
    Dir2 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x200);
    Dir6 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x600);
    if (Dist > 8000 && !Dir2 && (pEm->Status_flg & 0x2000) && !EM34_W(pEm)->Unk22) {
        EM_SET_RTN(pEm, 1, 2, 0, 0);
        if ((Rnd_l() & 1) && !EM34_W(pEm)->Unk10) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            return;
        }
    }
    if (!EM34_W(pEm)->Unk10 && Dist > 5000 && !Dir4 && (pEm->Status_flg & 0x2000))
        EM_SET_RTN(pEm, 1, 0xA, 0, 0);
    if (Dist < 5000 && Dir6)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist < 1300 && !Dir2)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
    if (EM34_W(pEm)->Flg & 0x40)
        EM_SET_RTN(pEm, 1, 0x10, 0, 0);
    if (!EM34_W(pEm)->Unk00 && (pT->Be_flg & 0x20000000)) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* brain: walk (Type 1, D_800D1F2C < 0) */
void Em34_walk_br1h(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir4;
    SHORT Dir2;
    SHORT Dir6;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Dir4 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400);
    Dir2 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x200);
    Dir6 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x600);
    if (Dist > 4000 && !Dir2 && (pEm->Status_flg & 0x2000) && !EM34_W(pEm)->Unk22)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (Dist < 5000 && Dir6)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (Dist < 1300 && !Dir2)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    if (pEm->Pos_y == pT->Pos_y && !(pT->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
        if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            EM34_W(pEm)->Flg &= ~0x800;
        }
        if (pT == &Pl_work) {
            if (!EM34_W(pEm)->Unk06 && Dist < 1500 && !Dir2 && !pT->Damage_cnt) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
                EM34_W(pEm)->Flg &= ~0x400;
            }
            if (Dist < 2000 && !Dir2 && pT->Routine_0 == 5 && pT->Unk12C)
                EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
    } else if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
    if (EM34_W(pEm)->Flg & 0x40)
        EM_SET_RTN(pEm, 1, 0x10, 0, 0);
    if ((!EM34_W(pEm)->Unk00 || !EM34_W(pEm)->Unk06) && (pT->Be_flg & 0x20000000)) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* motion: walk */
void Em34_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd_old;
    LONG Side;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x1F);
        pEm->Routine_2 = 1;
        if (pEm->Spd_x > 100)
            pEm->Spd_x = 100;
        pEm->Timer0 = 150;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 5;
        if (!pEm->UnkE2) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
            if ((LONG)pEm->Unk154 < 3000)
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        } else {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x20);
            pEm->UnkE2--;
        }
        if (pEm->Col.Hit_flg & 0x30) {
            if (++pEm->UnkDA > 100) {
                pEm->UnkE2 = 30;
                Side = Rnd_l() & 1;
                pEm->Timer0 = 150;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x40 - Side * 120;
            }
        } else {
            pEm->UnkDA = 0;
        }
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (--pEm->Timer0 == -1)
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        break;
    case 2:
        EM_SET_MOVE(pEm, 2, 0, 0x1F);
        pEm->Routine_2 = 3;
    case 3:
        func_80018EC8(pEm, pKan, pSeq, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        if ((LONG)pEm->Unk154 < 3000)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* brain: run (Type 0) */
void Em34_run_br0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir2;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Dir2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    if (Dist < 2000 && (pEm->Status_flg & 0x2000))
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3500 && !Dir2 && !EM34_W(pEm)->Unk00 && pEm->UnkE3)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    if (EM34_W(pEm)->Hit_cnt > 60) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg |= 0x800;
    }
}

/* brain: run (Type 1) */
void Em34_run_br1(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir8 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    SHORT Dir2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);

    if (Dist < 1300 && (pEm->Status_flg & 0x2000))
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3500 && !Dir2 && !EM34_W(pEm)->Unk00 && pEm->UnkE3)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    if (pEm->Pos_y == pPl->Pos_y && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
        if (!EM34_W(pEm)->Unk06 && Dist < 1500 && !Dir2 && !pPl->Damage_cnt) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
            EM34_W(pEm)->Flg &= ~0x400;
            if (pPl->Be_flg & 0x20000000) {
                EM_SET_RTN(pEm, 1, 5, 0, 0);
                EM34_W(pEm)->Flg &= ~0x800;
            }
        }
        if (Dist < 2000 && !Dir2 && pPl->Routine_0 == 5 && pPl->Unk12C)
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    } else if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir8) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
    if (EM34_W(pEm)->Hit_cnt > 60) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg |= 0x800;
    }
}

/* brain: run (Type 0, D_800D1F2C < 0) */
void Em34_run_br0h(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir2;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 800);
    Dir2 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x200);
    if (Dist < 2000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3500 && !Dir2 && !EM34_W(pEm)->Unk00 && pEm->UnkE3)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
}

/* brain: run (Type 1, D_800D1F2C < 0) */
void Em34_run_br1h(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir8;
    SHORT Dir2;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Dir8 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 800);
    Dir2 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x200);
    if (Dist < 1300)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3500 && !Dir2 && !EM34_W(pEm)->Unk00 && pEm->UnkE3)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    if (pEm->Pos_y == pT->Pos_y && !(pT->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
        if (pT == &Pl_work) {
            if (!EM34_W(pEm)->Unk06 && Dist < 1500 && !Dir2 && !pT->Damage_cnt) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
                EM34_W(pEm)->Flg &= ~0x400;
            }
            if (Dist < 2000 && !Dir2 && pT->Routine_0 == 5 && pT->Unk12C)
                EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
    } else if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir8) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* motion: run */
void Em34_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Cnt_tbl0[8] = { 0, 1, 1, 0, 1, 0, 1, 1 };
    UCHAR Cnt_tbl1[8] = { 0, 1, 0, 1, 1, 0, 1, 1 };
    SHORT Spd_old;
    LONG Side;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 6, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 150;
        pEm->UnkE3 = Cnt_tbl0[Rnd_l() & 7];
        if (pEm->Type & 1)
            pEm->UnkE3 = Cnt_tbl1[Rnd_l() & 7];
        EM34_W(pEm)->Unk00 = 15;
        EM34_W(pEm)->Unk06 = 15;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 3;
        if (!pEm->UnkE2) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x60);
        } else {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x60);
            pEm->UnkE2--;
        }
        if (pEm->Col.Hit_flg & 0x30) {
            if (++pEm->UnkDA > 100) {
                pEm->UnkE2 = 20;
                Side = Rnd_l() & 1;
                pEm->UnkDA = 0;
                pEm->Cdir_y += 0x60 - Side * 192;
            }
        } else {
            pEm->UnkDA = 0;
        }
        Joint_move(pEm, pKan, pSeq, 0x200);
        Add_speedXZ(pEm, 0);
        if (--pEm->Timer0 == -1)
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        break;
    }
}

/* brain: motion 3 (Type 0) */
void Em34_back_br0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir4;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    Dir4 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    if (Dist > 3000 && !Dir4) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Rnd_l() & 1) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            return;
        }
    }
    if (Dist > 5000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3000 && Dir4)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
        if (EM34_W(pEm)->Unk13 < 0) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM34_W(pEm)->Unk13 = (Rnd_l() & 1) + 2;
        }
    }
}

/* brain: motion 3 (Type 1) */
void Em34_back_br1(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG Dist = pEm->Unk154;
    SHORT Dir8 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800);
    SHORT Dir4 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);

    if (Dist > 3000 && !Dir4)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist > 5000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3000 && Dir4)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (pEm->Pos_y == pPl->Pos_y && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
        if (!EM34_W(pEm)->Unk06 && Dist < 1500 && !Dir4 && !pPl->Damage_cnt) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
            EM34_W(pEm)->Flg &= ~0x400;
        }
        if (Dist < 2000 && !Dir4 && pPl->Routine_0 == 5 && pPl->Unk12C)
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    } else if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir8) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* brain: motion 3 (Type 0, D_800D1F2C < 0) */
void Em34_back_br0h(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir4;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 800);
    Dir4 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400);
    if (Dist > 3000 && !Dir4) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Rnd_l() & 1) {
            EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            return;
        }
    }
    if (Dist > 5000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3000 && Dir4)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir4) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* brain: motion 3 (Type 1, D_800D1F2C < 0) */
void Em34_back_br1h(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    ULONG Dist = pEm->Unk154;
    SHORT Dir8;
    SHORT Dir4;

    if (pT == G.pEnemy[1])
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    Dir8 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 800);
    Dir4 = Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400);
    if (Dist > 3000 && !Dir4)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist > 5000)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    if (Dist < 3000 && Dir4)
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if (pEm->Pos_y == pT->Pos_y && !(pT->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40) && pT == &Pl_work) {
        if (!EM34_W(pEm)->Unk06 && Dist < 1500 && !Dir4 && !pT->Damage_cnt) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
            EM34_W(pEm)->Flg &= ~0x400;
        }
        if (Dist < 2000 && !Dir4 && pT->Routine_0 == 5 && pT->Unk12C)
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    }
    if (!EM34_W(pEm)->Unk00 && Dist < 1500 && !Dir8) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        EM34_W(pEm)->Flg &= ~0x800;
    }
}

/* motion 3: walks for 120 frames */
void Em34_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd_old;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 120;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 5;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (--pEm->Timer0 == -1) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if ((Rnd_l() & 3) == 1)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
        break;
    }
}

void func_80103048(void) {
}

/* motion 4: turns around away from the target */
void Em34_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x11, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        EM34_W(pEm)->Flg |= 3;
        Snd_se_enem(1, pEm);
    case 1:
        Goto00(pEm, pT->Pos_x, pT->Pos_z, -0x20);
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Cdir_y += 0x800;
            EM34_W(pEm)->Flg &= ~3;
            pEm->Unk120 &= ~0x12;
        }
        if (pEm->Move_cnt == 10)
            pEm->Unk120 |= 0x12;
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_80103184(void) {
}

/* motion 5: punch / claw swing */
void Em34_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pT = EM34_W(pEm)->pTarget;
    SVECTOR Ofs;
    MATRIX Mat;
    HIT_REQ At; /* attack box handed to func_80047F70: hits other enemies */
    UCHAR Id_tbl0[8] = { 0, 1, 2, 3, 2, 0, 1, 3 };
    UCHAR Id_tbl1[8] = { 0, 1, 2, 1, 2, 0, 1, 2 };
    tagGLOBAL *pG;
    ENEMY_WORK *pSpl;
    ENEMY_WORK *pPl;
    ULONG Dist;
    PARTS_W *pP;
    LONG Side;
    LONG Dir;

    pG = &G;
    pSpl = pG->pEnemy[1];
    Dist = pEm->Unk154;
    pPl = PLAYER_EM(pG);
    if (pT == pSpl)
        Dist = pEm->Unk158 & 0x3FFFFFFF;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 7, 0, 7);
        (pP = &pEm->pSin_parts_ptr[5])->pTmd = Em34_tmd_old5;
        pEm->UnkE3 = 0;
        if (pPl->Pos_y > pEm->Pos_y || (Rnd_l() & 1) || (pPl->Be_flg & 0x20000000)) {
            EM_SET_MOVE(pEm, 8, 0, 7);
            pEm->UnkE3 = 3;
            (pP = &pEm->pSin_parts_ptr[5])->pTmd = Em34_tmd_new5;
            if (pEm->Type & 1)
                (pP = &pEm->pSin_parts_ptr[8])->pTmd = Em34_tmd_old8;
        }
        if (pEm->nFloor + 1 == pPl->nFloor) {
            EM_SET_MOVE(pEm, 7, 0, 7);
            (pP = &pEm->pSin_parts_ptr[5])->pTmd = Em34_tmd_old5;
            pEm->UnkE3 = 0;
            if (pEm->Type & 1)
                (pP = &pEm->pSin_parts_ptr[8])->pTmd = Em34_tmd_new8;
        }
        pEm->Routine_2 = 1;
        EM34_W(pEm)->Flg |= 1;
        pEm->Unk120 |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        if (EM34_W(pEm)->Flg & 0x80) {
            pEm->Move_cnt = 3;
            func_80018EC8(pEm, pKan, pSeq, 0);
            Joint_move(pEm, pKan, pSeq, 0x200);
            pEm->Move_cnt = 4;
            func_80018EC8(pEm, pKan, pSeq, 0);
            pEm->Move_cnt = 5;
            pEm->Hokan_flg = 7;
        }
        EM34_W(pEm)->Unk13--;
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM34_W(pEm)->Unk00 = 20;
            if (pPl->Pos_y != pEm->Pos_y)
                EM34_W(pEm)->Unk00 = 10;
            (pP = &pEm->pSin_parts_ptr[5])->pTmd = Em34_tmd_new5;
            if (pEm->Type & 1)
                pEm->pSin_parts_ptr[8].pTmd = Em34_tmd_new8;
            if (!(pEm->Type & 1)) {
                if ((LONG)pEm->Unk154 > 3000 && !(Rnd_l() & 1))
                    EM_SET_RTN(pEm, 1, 0xA, 0, 0);
                if (!(pEm->Type & 1) && pPl->Life < 50)
                    EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            }
            pEm->Unk120 &= ~3;
            EM34_W(pEm)->Flg &= ~1;
        }
        Add_speedXZ(pEm, 0);
        if (!(pEm->UnkE3 & 0x80)) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x20);
            if (pEm->Move_cnt >= 11 && !(EM34_W(pEm)->Flg & 0x800)
                && (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 800) || (LONG)Dist > 2500)) {
                EM_SET_RTN(pEm, 1, 1, 0, 0);
                EM34_W(pEm)->Unk00 = 30;
                EM34_W(pEm)->Unk06 = 15;
                if ((Rnd_l() & 3) == 1)
                    EM_SET_RTN(pEm, 1, 2, 0, 0);
                if (pPl->Pos_y != pEm->Pos_y) {
                    EM34_W(pEm)->Unk00 = 15;
                    if (Rnd_l() & 1)
                        EM_SET_RTN(pEm, 1, 2, 0, 0);
                }
                pEm->Unk120 &= ~3;
                EM34_W(pEm)->Flg &= ~1;
                pEm->pSin_parts_ptr[5].pTmd = Em34_tmd_new5;
                if (pEm->Type & 1)
                    pEm->pSin_parts_ptr[8].pTmd = Em34_tmd_new8;
                break;
            }
        }
        if (*pEm->pNow_seq & 0x800) {
            pEm->UnkE3 |= 0x80;
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x40);
            pP = &pEm->pSin_parts_ptr[5 + (pEm->UnkE3 & 0x7F)];
            if (!pPl->Damage_cnt) {
                Mat = pP->Workm;
                Ofs.vy = 100;
                Ofs.vx = 0;
                Ofs.vz = 0;
                Ofs.pad = 600;
                Mat.t[1] += 400;
                if (pEm->nFloor + 1 == pPl->nFloor)
                    Ofs.vy = -400;
                if (func_80044420(&Mat, &Ofs, pPl)) {
                    LONG Ang;

                    Side = 0x400;
                    if (!(pEm->UnkE3 & 1))
                        Side = -0x400;
                    Ang = (pPl->Cdir_y - (SHORT)(Side + Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z)) + 0x400) & 0xFFF;
                    Dir = Ang < 0x800;
                    EM_RTN(pPl) = (Dir + 2) << 8 | 2;
                    pPl->Damage_cnt |= 0x80;
                    Pl_life_down(20, 1);
                    Snd_se_enem(4, pEm);
                    if (pPl->Pos_y != pEm->Pos_y) {
                        if (Pl_life_down(1, 0) == 2)
                            EM_SET_RTN(pPl, 3, 0, 0, 0);
                    }
                    Pad_vib_set0(2, 0);
                    Pad_vib_set1(6, 250, 0);
                }
            }
            if (pG->Em_num >= 2) {
                At.pPos = (VECTOR *)pP->Workm.t;
                At.W = 0;
                At.H = 100;
                At.D = 0;
                At.R = pEm->Cdir_y;
                if (func_80047F70(pEm, &At, &Em34_at_data))
                    Snd_se_enem(4, pEm);
            }
            if (D_800D1F2C_ < 0 && !pSpl->Damage_cnt) {
                Mat = pP->Workm;
                Ofs.vy = 100;
                Ofs.vx = 0;
                Ofs.vz = 0;
                Ofs.pad = 600;
                Mat.t[1] += 400;
                if (func_80044420(&Mat, &Ofs, pSpl)) {
                    LONG Ang;

                    Side = 0x400;
                    if (!(pEm->UnkE3 & 1))
                        Side = -0x400;
                    Ang = (pSpl->Cdir_y - (SHORT)(Side + Direction_ck(pEm->Pos_x, pEm->Pos_z, pSpl->Pos_x, pSpl->Pos_z)) + 0x400) & 0xFFF;
                    Dir = Ang < 0x800;
                    EM_RTN(pSpl) = (Dir + 2) << 8 | 2;
                    pSpl->Damage_cnt |= 0x80;
                    if (func_8005F984(15, 0) == 2)
                        EM_SET_RTN(pSpl, 3, 0, 0, 0);
                    Snd_se_enem(4, pEm);
                }
            }
        }
    default:
        if (*pEm->pNow_seq & 0x400) {
            Em34_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
            Em34_hit_area.pBox = Em34_at_box;
            Em34_hit_area.Dir = pEm->Cdir_y;
            Em34_hit_area.Id = Id_tbl0[(Rnd_l() >> (Rnd_l() & 3)) & 7];
            if (pPl->Pos_y < pEm->Pos_y)
                Em34_hit_area.Id = Id_tbl1[(Rnd_l() >> (Rnd_l() & 3)) & 7];
            func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em34_hit_area, G_PLAYER);
            if ((LONG)G.Status_flg < 0) {
                Em34_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
                Em34_hit_area.pBox = Em34_at_box;
                Em34_hit_area.Dir = pEm->Cdir_y;
                Em34_hit_area.Id = 0;
                func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em34_hit_area, G.pEnemy[1]);
            }
        }
        break;
    }
}

void func_80103AD0(void) {
}

/* motion 6: running shoulder attack */
void Em34_rush(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pSpl = pG->pEnemy[1];
    ENEMY_WORK *pPl = PLAYER_EM(pG);
    PARTS_W *pP;
    SVECTOR Ofs;
    HIT_REQ At; /* attack box handed to func_80047F70: hits other enemies */

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 9, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = pEm->Spd_x;
        EM34_W(pEm)->Flg |= 1;
        pP = &pEm->pSin_parts_ptr[8];
        pP->pTmd = Em34_tmd_old8;
        pEm->Unk120 |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        if (Rnd_l() & 1)
            Snd_se_enem(14, pEm);
        if (EM34_W(pEm)->Flg & 0x80) {
            pEm->Move_cnt = 8;
            Spd_get(pEm, pKan, pSeq, 0);
        }
        EM34_W(pEm)->Unk13--;
    case 1:
        if (pEm->Move_cnt < 30)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x30);
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (pEm->Timer0 > 0)
            pEm->Timer0 -= 20;
        pEm->Spd_x += pEm->Timer0;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pP = &pEm->pSin_parts_ptr[8];
            pP->pTmd = Em34_tmd_new8;
            if (!(pEm->Type & 1) && (LONG)pEm->Unk154 > 3000 && !(Rnd_l() & 1))
                EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            EM34_W(pEm)->Unk00 = 20;
            pEm->Unk120 &= ~3;
            EM34_W(pEm)->Flg &= ~1;
        }
        Add_speedXZ(pEm, 0);
        if (*pEm->pNow_seq & 0x800) {
            pP = &pEm->pSin_parts_ptr[8];
            if (!pPl->Damage_cnt) {
                Ofs.vy = 300;
                Ofs.vx = 0;
                Ofs.vz = 0;
                Ofs.pad = 700;
                if (func_80044420(&pP->Workm, &Ofs, pPl)) {
                    EM_RTN(pPl) = (Cdir_ck_l(pEm, pPl) + 2) << 8 | 2;
                    pPl->Damage_cnt |= 0x80;
                    Snd_se_enem(4, pEm);
                    Pl_life_down(25, 1);
                    Pad_vib_set0(3, 0);
                    Pad_vib_set1(8, 250, 0);
                }
            }
            if (pG->Em_num >= 2) {
                At.pPos = (VECTOR *)pP->Workm.t;
                At.W = 0;
                At.H = 300;
                At.D = 0;
                At.R = pEm->Cdir_y;
                if (func_80047F70(pEm, &At, &Em34_at_data))
                    Snd_se_enem(4, pEm);
            }
            if (D_800D1F2C_ < 0 && !pSpl->Damage_cnt) {
                Ofs.vy = 300;
                Ofs.vx = 0;
                Ofs.vz = 0;
                Ofs.pad = 700;
                if (func_80044420(&pP->Workm, &Ofs, pSpl)) {
                    EM_RTN(pSpl) = (Cdir_ck_l(pEm, pSpl) + 2) << 8 | 2;
                    pSpl->Damage_cnt |= 0x80;
                    if (func_8005F984(15, 0) == 2)
                        EM_SET_RTN(pSpl, 3, 0, 0, 0);
                    Snd_se_enem(4, pEm);
                }
            }
        }
        break;
    }
    if (*pEm->pNow_seq & 0x400) {
        Em34_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
        Em34_hit_area.pBox = Em34_at_box2;
        Em34_hit_area.Dir = pEm->Cdir_y;
        Em34_hit_area.Id = 1;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em34_hit_area, &Pl_work);
        Em34_hit_area.pBox = Em34_at_box3;
        Em34_hit_area.Id = 2;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em34_hit_area, &Pl_work);
    }
}

void func_80103F30(void) {
}

/* motion 7: grabs the player (Routine 8 on a hit) */
void Em34_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Ofs;
    MATRIX *pM;
    UCHAR Id_tbl0[8] = { 0, 3, 2, 3, 2, 0, 2, 3 };
    UCHAR Id_tbl1[8] = { 0, 0, 2, 0, 2, 0, 2, 2 };
    ULONG Dist = pEm->Unk154;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xA, 0, 7);
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        EM34_W(pEm)->Flg |= 1;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        if (EM34_FLG_L(pEm) & 0x480) {
            pEm->Move_cnt = 3;
            pEm->Timer0 = pEm->Spd_x >> 1;
            Spd_get(pEm, pKan, pSeq, 0);
        }
        pEm->Unk120 |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        pEm->pSin_parts_ptr[8].pTmd = Em34_tmd_new8;
    case 1:
        if (pEm->Move_cnt < 21)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x30);
        Spd_get(pEm, pKan, pSeq, 0);
        if (EM34_W(pEm)->Flg & 0x400) {
            if (pEm->Timer0) {
                pEm->Spd_x += pEm->Timer0;
                pEm->Timer0 -= 20;
                if (pEm->Timer0 < 0)
                    pEm->Timer0 = 0;
            }
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if ((Rnd_l() & 3) == 1)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            EM34_W(pEm)->Unk06 = 45;
            pEm->Unk120 &= ~3;
            EM34_W(pEm)->Flg &= ~1;
        }
        Add_speedXZ(pEm, 0);
        if (!pEm->UnkE3 && pEm->Move_cnt >= 11
            && (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) || (LONG)Dist > 2500 || (pPl->Col.Hit_flg & 0x40) || (pEm->Col.Hit_flg & 0x40))) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if ((Rnd_l() & 3) == 1)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            EM34_W(pEm)->Unk06 = 15;
            EM34_W(pEm)->Unk00 = 10;
            pEm->Unk120 &= ~3;
            EM34_W(pEm)->Flg &= ~1;
            break;
        }
        if ((*pEm->pNow_seq & 0x800) && !pPl->Damage_cnt && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
            pM = &pEm->pSin_parts_ptr[8].Workm;
            pEm->UnkE3 = 1;
            Ofs.vy = -100;
            Ofs.vz = 200;
            Ofs.vx = 0;
            Ofs.pad = 400;
            if (func_80044420(pM, &Ofs, pPl)) {
                EM_SET_RTN(pEm, 1, 8, 0, 0);
                pEm->Damage_cnt |= 0x80;
                pPl->Damage_cnt |= 0x80;
                EM34_W(pEm)->Flg &= ~1;
                Snd_se_enem(0, pEm);
                Pad_vib_set0(2, 0);
                Pad_vib_set1(6, 150, 0);
            }
        }
    default:
        if (*pEm->pNow_seq & 0x400) {
            Em34_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
            Em34_hit_area.pBox = Em34_at_box;
            Em34_hit_area.Dir = pEm->Cdir_y;
            Em34_hit_area.Id = Id_tbl0[(Rnd_l() >> (Rnd_l() & 3)) & 7] + 0x40;
            if (pPl->Pos_y < pEm->Pos_y)
                Em34_hit_area.Id = Id_tbl1[(Rnd_l() >> (Rnd_l() & 3)) & 7] + 0x40;
            if ((Rnd_l() & 3) == 1)
                Em34_hit_area.Id = 0x62;
            func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em34_hit_area, &Pl_work);
        }
        break;
    }
}

void func_801043EC(void) {
}

/* motion 8: grabs the player from the front and throws him (left, right or down) */
void Em34_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = G_PLAYER;
    EM34_MOVE3 Move_tbl = Em34_catch_move;
    EM34_MOVE3 Stop_tbl = Em34_pl_stop;
    SVECTOR Ofs;
    VECTOR Pos;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xB, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = pEm->Cdir_y;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pEm->Cdir_y = pEm->Timer0;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
        pPl->Unk120 |= 0x12;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x800;
        EM_SET_RTN(pPl, 5, 0, 0, 0);
        G_PLAYER->Be_flg |= 0x800;
    case 1:
        if (pEm->Move_cnt < 8)
            Goto01(pEm, pEm->Timer0, 0x100);
        pEm->Base_pos_x = pPl->Base_pos_x;
        pEm->Base_pos_z = pPl->Base_pos_z;
        pEm->UnkDA = pEm->Cdir_y;
        pEm->Cdir_y = pEm->Timer0;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y = pEm->UnkDA;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            pPl->UnkE3 = Rnd_l() & 1;
            Ofs.vx = 2000 - pPl->UnkE3 * 4000;
            Ofs.vy = 0;
            Ofs.vz = 0;
            ApplyMatrix((MATRIX *)pEm->Workm_m, &Ofs, &Pos);
            Pos.vx += pEm->Pos_x;
            Pos.vy += pEm->Pos_y;
            Pos.vz += pEm->Pos_z;
            Em34_floor = func_8004D720(&Pos.vx, 600, 0x2000, 0);
            if (pEm->Unk122 != Em34_floor)
                pPl->UnkE3 = (pPl->UnkE3 - 1) & 1;
            if (--EM34_W(pEm)->Unk02 == 0xFF) {
                EM34_W(pEm)->Unk02 = 3;
                pPl->UnkE3 = 2;
            }
            if (pPl->UnkE3 != 2) {
                Pl_life_down(25, 1);
                if (pPl->Life < 2)
                    pPl->UnkE3 = 2;
            }
            pEm->UnkE3 = pPl->UnkE3;
            EM_MOVE(pEm) = Move_tbl.Move[pPl->UnkE3] + 0x70000;
            if (pEm->UnkE3 == 2)
                EM_SET_RTN(pEm, 1, 0xD, 0, 0);
            Snd_se_enem((Rnd_l() & 1) ? 14 : 15, pEm);
        }
        break;
    case 2:
        if (pEm->Move_cnt < (UCHAR)Stop_tbl.Move[pEm->UnkE3]) {
            pEm->Base_pos_x = pPl->Base_pos_x;
            pEm->Base_pos_z = pPl->Base_pos_z;
        }
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if ((Rnd_l() & 7) == 2)
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            EM34_W(pEm)->Unk00 = 5;
            EM34_W(pEm)->Unk06 = 45;
            EM34_W(pEm)->Unk12 = 45;
            pEm->Unk120 &= ~3;
            pEm->Damage_cnt &= 0x7F;
        }
        break;
    }
}

void func_80104828(void) {
}

/* motion 9: grabs the player and lifts him (RE2 Em2a_pl_dm_c start) */
void Em34_catch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM34_MOVE3 Move_tbl = Em34_catch_move;
    ENEMY_WORK *pPl = G_PLAYER;
    LONG Unused[6];

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x12, 0, 7);
        pEm->Routine_2 = 1;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        pEm->Cdir_y += 0x400;
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->Unk120 |= 0x12;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x800;
        EM_SET_RTN(pPl, 5, 1, 0, 0);
        G_PLAYER->Be_flg |= 0x800;
        Snd_se_enem(0, pEm);
        Pad_vib_set0(2, 0);
        Pad_vib_set1(10, 150, 0);
        EM34_W(pEm)->Flg |= 1;
    case 1:
        pEm->Base_pos_x = pPl->Base_pos_x;
        pEm->Base_pos_z = pPl->Base_pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 8, 2, 0);
            pPl->UnkE3 = 2;
            if (--EM34_W(pEm)->Unk02 == 0xFF)
                EM34_W(pEm)->Unk02 = 3;
            pEm->UnkE3 = pPl->UnkE3;
            EM_MOVE(pEm) = Move_tbl.Move[pPl->UnkE3] + 0x70000;
            if (pEm->UnkE3 == 2)
                EM_SET_RTN(pEm, 1, 0xD, 0, 0);
            Snd_se_enem(11, pEm);
        }
        break;
    }
}

void func_80104A4C(void) {
}

INCLUDE_RODATA("asm/nonmatchings/enemy/em34", Em34_catch_move);

INCLUDE_RODATA("asm/nonmatchings/enemy/em34", Em34_pl_stop);

INCLUDE_RODATA("asm/nonmatchings/enemy/em34", Em34_shot_ofs);

INCLUDE_ASM("asm/nonmatchings/enemy/em34", Em34_shoot);

void func_80105590(void) {
}

/* motion 11: roar (Type 0 spits sparks and morphs the head, part 16) */
void Em34_roar(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    MATRIX *pM;
    MATRIX *pM2;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1D, 0, 0xF);
        pEm->Routine_2 = 1;
        if (pEm->Type & 1)
            EM_SET_MOVE(pEm, 0x1F, 0, 0xF);
        Snd_se_enem((Rnd_l() & 1) ? 15 : 14, pEm);
        pP = &pEm->pSin_parts_ptr[16];
        pEm->pSin_parts_ptr[16].Be_flg |= 1;
        pP->Size_z = 0;
        pP->Cdir_z = 0;
        pP->Cdir_x = 0;
        EM34_SA(pP)->Key[0].Rate = 0;
        pEm->UnkE3 = 0;
    case 1:
        if (!(pEm->Type & 1)) {
            if (!(pEm->Move_cnt & 3) && pEm->Move_cnt < 53) {
                Ofs.vx = 11;
                Ofs.vy = (Rnd_l() & 0xF) + 0x10C;
                Ofs.vz = 50;
                pM = (MATRIX *)pEm->pSin_parts_ptr;
                pM = &((PARTS_W *)pM)[5].Workm;
                Esp_call(0x4001700, ((Rnd_l() * 4 + 0xC00) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
                Esp_call(0x4000500, 0xA000000, pM, &Ofs, NULL);
                Ofs.vx = (0xF - Rnd_l()) & 0x1F;
                Ofs.vy = (Rnd_l() & 0xF) + 0x10C;
                Ofs.vz = (0xF - Rnd_l()) & 0x1F;
                Esp_call(0x2001700, ((Rnd_l() * 4 + 0xA00) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
                pP = pEm->pSin_parts_ptr;
                Ofs.vx = 0;
                Ofs.vy = 0;
                Ofs.vz = 100;
                pM2 = &pP[16].Workm;
                Esp_call(0x4001700, ((Rnd_l() * 2 + 0xF00) << 16) | (USHORT)pEm->Cdir_y, pM2, &Ofs, NULL);
                Ofs.vz = 200;
                Esp_call(0x4001700, ((Rnd_l() * 2 + 0xA00) << 16) | (USHORT)pEm->Cdir_y, pM2, &Ofs, NULL);
            }
            pP = &pEm->pSin_parts_ptr[16];
            if ((ULONG)(pEm->Move_cnt - 11) < 39) {
                pP->Size_z += 0x80;
                if (pP->Size_z > 0xC00)
                    pP->Size_z = 0xC00;
            }
            if (pEm->Move_cnt >= 51) {
                pP->Size_z -= 0x100;
                if (pP->Size_z < 0)
                    pP->Size_z = 0;
            }
            EM34_SA(pP)->Key[1].Rate = 0x1000;
            if (pP->Size_z > 0x800) {
                EM34_SA(pP)->Key[0].Rate = Em34_morph_tbl[pEm->UnkE3] * 16;
                EM34_SA(pP)->Key[1].Rate = 0;
                if (++pEm->UnkE3 >= 8)
                    pEm->UnkE3 = 0;
                pP->Cdir_z += 0x80;
                RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
            }
            Sa_morph_step(pEm, pP);
        }
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_80105964(void) {
}

/* motion 12: grows the claw arm (part 17 glows, then the arm model is swapped) */
void Em34_arm_grow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX *pM;
    PARTS_W *pP;
    ULONG Rgb;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1C, 0, 7);
        pEm->Routine_2 = 1;
        pM = &pEm->pSin_parts_ptr[17].Workm;
        EM34_W(pEm)->Flg |= 0x1001;
        Ofs.vx = 300;
        Ofs.vy = 500;
        Ofs.vz = 0;
        Esp_call(0x1001203, 0xA000000, pM, &Ofs, NULL);
        Ofs.vx = 300;
        Ofs.vy = -1500;
        Ofs.vz = 0;
        Esp_call(0x1001203, 0xA000000, pM, &Ofs, NULL);
        pEm->Type = (pEm->Type & ~3) | 1;
    case 1:
        if (pEm->Move_cnt < 20) {
            pP = &pEm->pSin_parts_ptr[17];
            Rgb = pP->Poly_rgb;
            pP->Poly_rgb = Rgb + 3;
            if (((Rgb + 3) & 0xFF) > 0xF0)
                pP->Poly_rgb = Rgb;
        }
        if (pEm->Move_cnt == 20) {
            pEm->pSin_parts_ptr[17].Unk74 = 1;
            EM34_W(pEm)->Flg |= 4;
            pEm->Type = (pEm->Type & ~3) | 1;
            pEm->pSin_parts_ptr[8].pTmd = Em34_tmd_new8;
        }
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            EM34_W(pEm)->Flg &= ~0x1001;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_80105B44(void) {
}

/* motion 13: holds the player and bites (part 16 morphs), then lets go */
void Em34_bite(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pParts = pEm->pSin_parts_ptr;
    PARTS_W *pP = &pParts[16];
    SVECTOR Ofs;
    UCHAR Cnt;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pParts[16].Be_flg |= 1;
        pP->Size_z = 0;
        pP->Cdir_z = 0;
        pP->Cdir_x = 0;
        EM34_SA(pP)->Key[0].Rate = 0;
        pEm->UnkE3 = 0;
    case 1:
        if (pEm->Move_cnt == 0 || pEm->Move_cnt == 20 || pEm->Move_cnt == 30 || pEm->Move_cnt == 35
            || pEm->Move_cnt == 37 || pEm->Move_cnt == 39) {
            Pad_vib_set0(2, 0);
            Pad_vib_set1(3, 150, 0);
        }
        if (pEm->Move_cnt < 53) {
            pP->Size_z += 0x90;
            if (pP->Size_z > 0x1800)
                pP->Size_z = 0x1800;
        }
        if ((ULONG)(pEm->Move_cnt - 54) < 4) {
            pP->Size_z -= 0x300;
            if (pP->Size_z < 0)
                pP->Size_z = 0;
            if (pEm->Move_cnt == 54) {
                Pad_vib_set0(5, 0);
                Pad_vib_set1(10, 200, 0);
            }
            if (!(pEm->Move_cnt & 3))
                Pad_vib_set0((Rnd_l() & 1) + 1, 0);
        }
        Cnt = pEm->Move_cnt;
        if ((ULONG)(Cnt - 61) < 29) {
            if (!(Cnt & 3)) {
                Pad_vib_set0((Rnd_l() & 1) + 1, 0);
                Pad_vib_set1(3, 200, 0);
            }
            if (pEm->Move_cnt == 89) {
                Pad_vib_set0(5, 0);
                Pad_vib_fade_set(90, 200, 0, 0);
            }
        }
        if ((ULONG)(pEm->Move_cnt - 91) < 8)
            pP->Size_z += 0xC0;
        if ((ULONG)(pEm->Move_cnt - 100) < 4)
            pP->Size_z -= 0x180;
        EM34_SA(pP)->Key[1].Rate = 0x1000;
        if (pP->Size_z > 0x800 && pEm->Move_cnt < 58) {
            EM34_SA(pP)->Key[0].Rate = Em34_morph_tbl[pEm->UnkE3] * 16;
            EM34_SA(pP)->Key[1].Rate = 0;
            if (++pEm->UnkE3 >= 8)
                pEm->UnkE3 = 0;
            pP->Cdir_x = 0x100;
            pP->Cdir_z += 0x80;
            RotMatrix_gte((SVECTOR *)&pP->Cdir_x, (MATRIX *)pP->M);
        }
        Sa_morph_step(pEm, pP);
        if (pEm->Move_cnt < 100 && pEm->Move_cnt % 7 == 1) {
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 100;
            Esp_call(0x4001700, ((Rnd_l() * 8 + 0x1800) << 16) | (USHORT)(Rnd_l() * 16), &pP->Workm, &Ofs, NULL);
            Esp_call(0x4000500, (USHORT)pEm->Cdir_y | 0x10000000, &pP->Workm, &Ofs, NULL);
            Esp_call(0x2000000, ((Rnd_l() * 8 + 0x1000) << 16) | (USHORT)(Rnd_l() * 16), &pP->Workm, &Ofs, NULL);
            Esp_call(0x2000500, (USHORT)pEm->Cdir_y | 0x8000000, &pP->Workm, &Ofs, NULL);
            if (pP->Size_z > 0x800) {
                Ofs.vz = 150;
                Esp_call(0x4001700, ((Rnd_l() * 8 + 0x1400) << 16) | (USHORT)(Rnd_l() * 16), &pP->Workm, &Ofs, NULL);
            }
        }
        pEm->Base_pos_x = pPl->Base_pos_x;
        pEm->Base_pos_z = pPl->Base_pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            EM34_W(pEm)->Unk00 = 60;
            pEm->Unk120 &= ~3;
            pP->Be_flg &= ~1;
        }
        break;
    case 2:
        EM_SET_MOVE(pEm, 0xF, 0, 7);
        pEm->Routine_2 = 3;
        pParts[16].Be_flg &= ~1;
        Ofs.vx = 0;
        Ofs.vy = 0;
        Ofs.vz = 50;
        Esp_call(0x4001700, ((Rnd_l() * 2 + 0xF00) << 16) | (USHORT)pEm->Cdir_y, &pParts[16].Workm, &Ofs, NULL);
        Ofs.vz = 100;
        Esp_call(0x4001700, ((Rnd_l() * 2 + 0xA00) << 16) | (USHORT)pEm->Cdir_y, &pParts[16].Workm, &Ofs, NULL);
        Ofs.vz = 150;
        Esp_call(0x4001700, (USHORT)pEm->Cdir_y | 0x8000000, &pParts[16].Workm, &Ofs, NULL);
        pEm->Col.W0 = 200;
        pEm->Col.D0 = 200;
        pEm->Be_flg &= ~0x800;
        Pl_work.Be_flg &= ~0x800;
        pEm->Damage_cnt &= 0x7F;
        Snd_se_enem(2, pEm);
        pEm->Unk120 &= ~3;
        EM34_W(pEm)->Flg &= ~1;
    case 3:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        pEm->Col.D0 += 25;
        pEm->Col.W0 += 25;
        if (pEm->Col.D0 > 800) {
            pEm->Col.D0 = 800;
            pEm->Col.W0 = 800;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            EM34_W(pEm)->Unk00 = 60;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* brain of the scripted motions: back to walking unless the player is held */
void func_8010625C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;

    if (pEm->Routine_2 < 2 && pPl->Routine_0 != 5) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        EM34_W(pEm)->Unk06 = (Rnd_l() & 0xF) + 10;
        EM34_W(pEm)->Unk00 = 15;
    }
}

/* motion 14: walks behind the player and grabs him from behind */
void Em34_back_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs = { 500, 0, 1000 };
    SVECTOR Rot;
    SVECTOR Pos;
    MATRIX M;
    ENEMY_WORK *pPl = &Pl_work;
    SHORT Spd_old;
    LONG Dx;
    LONG Dz;
    LONG Dy;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x1F);
        pEm->Routine_2 = 1;
        if (pEm->Spd_x > 100)
            pEm->Spd_x = 100;
    case 1:
        Spd_old = pEm->Spd_x;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd_old) * pEm->Hokan_flg) >> 5;
        Rot.vx = 0;
        Rot.vy = pPl->Cdir_y;
        Rot.vz = 0;
        RotMatrix_gte(&Rot, &M);
        ApplyMatrixSV(&M, &Ofs, &Pos);
        Pos.vx += pPl->Pos_x;
        Pos.vz += pPl->Pos_z;
        Goto00(pEm, Pos.vx, Pos.vz, 0x40);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        Dx = (Pos.vx - pEm->Pos_x) >> 1;
        Dz = (Pos.vz - pEm->Pos_z) >> 1;
        if (SquareRoot0(Dx * Dx + Dz * Dz) * 2 < 500 && !Goto01_ck(pEm, pPl->Cdir_y + 0x800, 0x400)) {
            EM_SET_MOVE(pEm, 0x10, 0, 7);
            pEm->Routine_2 = 2;
            pEm->Unk120 |= 3;
            PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
            if (!pPl->Unk12C) {
                EM_SET_RTN(pEm, 1, 3, 0, 0);
                pEm->Spd_x = 0;
                pEm->Spd_z = 0;
                pEm->Unk120 &= ~3;
            }
        }
        break;
    case 2:
        Goto01(pEm, pPl->Cdir_y + 0x800, 0x80);
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Unk120 &= ~3;
        }
        Add_speedXZ(pEm, 0);
        if ((*pEm->pNow_seq & 0x800) && pPl->Unk12C && !(pPl->Col.Hit_flg & 0x40) && !(pEm->Col.Hit_flg & 0x40)) {
            Dy = pPl->pSin_parts_ptr[1].Workm.t[1] - pEm->pSin_parts_ptr[8].Workm.t[1];
            if (Dy < 0)
                Dy = -Dy;
            if (Dy < 300) {
                EM_SET_RTN(pEm, 1, 9, 0, 0);
                Snd_se_enem(0, pEm);
                pPl->Damage_cnt |= 0x80;
                pEm->Damage_cnt |= 0x80;
            }
        }
        break;
    }
}

void func_80106630(void) {
}

/* motion 15: lies on the floor after a knock-down, gets up (or plays dead again) */
void Em34_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pParts;
    PARTS_W *pP;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        if (--pEm->Timer0 == -1)
            pEm->Routine_2 = 1;
        if (pEm->Damage_cnt)
            pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 0x16, 0, 0);
        EM34_W(pEm)->Flg |= 0x23;
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    case 1:
        EM_SET_MOVE(pEm, 0x16, 0, 0x1F);
        pEm->Routine_2 = 2;
        if (Rnd_l() & 1)
            Snd_se_enem(11, pEm);
    case 2:
        if (pEm->Move_cnt == 30)
            pEm->Be_flg |= 0x80000;
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM34_W(pEm)->Flg &= ~0x23;
            if (EM34_W(pEm)->Flg & 0x80) {
                EM_SET_RTN(pEm, 1, 0xF, 3, 0);
                EM_SET_MOVE(pEm, 0x1F, 0, 0xF);
                Snd_se_enem(15, pEm);
                EM34_W(pEm)->Flg |= 1;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 3:
        if (pEm->Move_cnt == 20)
            Snd_se_enem(11, pEm);
        if (pEm->Move_cnt == 50)
            Snd_se_enem(11, pEm);
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            EM34_W(pEm)->Flg &= ~1;
            pEm->Type &= ~0x2000;
        }
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt & 3) {
            Ofs.vx = 0x80 - Rnd_l();
            Ofs.vy = 0x80 - Rnd_l();
            Ofs.vz = 0x80 - Rnd_l();
            pParts = pEm->pSin_parts_ptr;
            pP = &pParts[3];
            Esp_call(0x4001700, ((Rnd_l() * 4 + 0xA00) << 16) | (USHORT)pEm->Cdir_y, &pParts[3].Workm, &Ofs, NULL);
        }
        if ((pEm->Move_cnt & 7) == 3) {
            Ofs.vx = 0x80 - Rnd_l();
            Ofs.vy = 0x80 - Rnd_l();
            Ofs.vz = 0x80 - Rnd_l();
            Esp_call(0x2000000, ((Rnd_l() + 0x800) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, &Ofs, NULL);
        }
        break;
    }
}

void func_8010696C(void) {
}

/* motion 16: jumps sideways around the player */
void Em34_side_step(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 3);
        pEm->Routine_2 = 1;
        if (Goto00_ck(pPl, pEm->Pos_x, pEm->Pos_z, 1) > 0)
            EM_SET_MOVE(pEm, 4, 0, 3);
        pEm->Unk13A = 100;
        pEm->Damage_cnt = 2;
        EM34_W(pEm)->Flg |= 1;
    case 1:
        if (pEm->Col.Hit_flg & 0x30)
            pEm->Damage_cnt = 0;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x60);
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x <<= 1;
            pEm->Spd_z <<= 1;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Unk13A = 600;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_80106AC4(void) {
}

/* motion 16: hits the player with the partner held like a club, then may throw it */
void Em34_club(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs;
    UCHAR Id_tbl[8] = { 3, 6, 3, 3, 3, 3, 7, 3 };
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG *pPos;
    LONG Hit;
    LONG Dir;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x21, 0, 0x1F);
        pEm->Routine_2 = 1;
        EM34_W(pEm)->Flg |= 1;
        if (EM34_W(pEm)->pPartner) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 7;
            EM34_W(pEm)->pPartner->Routine_2 = 0;
        }
        pEm->Unk120 |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        pEm->UnkE3 = 0;
        if (Rnd_l() & 1) {
            Snd_se_enem(14, pEm);
        } else {
            Snd_se_enem(11, pEm);
            pEm->UnkE3 = 1;
        }
    case 1:
        if (pEm->Move_cnt < 20)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            EM34_W(pEm)->Unk00 = 15;
            pEm->Unk120 &= ~3;
            EM34_W(pEm)->Flg &= ~1;
        }
        if ((*pEm->pNow_seq & 0x800) && !pPl->Damage_cnt) {
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
            Ofs.vx = 0;
            Ofs.vy = 0;
            Ofs.vz = 0;
            Ofs.pad = 800;
            pP = EM34_W(pEm)->pPartner->pSin_parts_ptr;
            Hit = 0;
            Hit |= func_80044420(&pP[1].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[2].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Ofs, pPl);
            if (Hit) {
                pEm->Cdir_y += 0x400;
                Hit = Cdir_ck_l(pEm, pPl);
                pEm->Cdir_y -= 0x400;
                pPl->Damage_cnt |= 0x80;
                EM_RTN(pPl) = Hit << 8 | 2;
                Dir = Direction_ck(pPl->Pos_x, pPl->Pos_z, pEm->Pos_x, pEm->Pos_z);
                Dir += 0x400;
                if (Hit)
                    Dir -= 0x800;
                Goto01(pPl, Dir, 0x200);
                Snd_se_stad(0x1020D, &pEm->Pos_x, 0, 0);
                Pl_life_down(20, 1);
                Pad_vib_set0(8, 0);
                Pad_vib_set1(8, 200, 0);
                pEm->UnkE3 = 1;
            }
        }
        if (pEm->Move_cnt == 30 && pEm->UnkE3 && Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200)
            && (LONG)pEm->Unk154 > 5000)
            pEm->UnkE3 = 0;
        if (pEm->Move_cnt == 30 && pEm->UnkE3) {
            EM_SET_RTN(pEm, 1, 0x12, 1, 0);
            EM_SET_MOVE(pEm, 0x22, 0x14, 0xF);
            if (EM34_W(pEm)->pPartner) {
                EM34_W(pEm)->pPartner->Routine_0 = 1;
                EM34_W(pEm)->pPartner->Routine_1 = 8;
                EM34_W(pEm)->pPartner->Routine_2 = 1;
                EM34_W(pEm)->pPartner->Move_no++;
                EM34_W(pEm)->pPartner->Move_cnt = 20;
                EM34_W(pEm)->pPartner->Hokan_flg = 15;
                EM34_W(pEm)->pPartner->Spd_x = 0x100;
                EM34_W(pEm)->pPartner->Spd_y = 0x1000;
            }
        }
        if (pEm->Move_cnt == 24)
            Snd_se_stad(0x1020F, &pEm->Pos_x, 0, 0);
        Add_speedXZ(pEm, 0);
        break;
    }
    if (*pEm->pNow_seq & 0x400) {
        Em34_hit_area.pPos = (VECTOR *)(pPos = &pEm->Pos_x);
        Em34_hit_area.pBox = Em34_at_box4;
        Em34_hit_area.Dir = pEm->Cdir_y;
        Em34_hit_area.Id = Id_tbl[(Rnd_l() >> (Rnd_l() & 3)) & 7];
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em34_hit_area, &Pl_work);
        Em34_hit_area.pBox = Em34_at_box5;
        func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, pPos, &Em34_hit_area, &Pl_work);
    }
}

void func_80106F94(void) {
}

/* motion 17: swings the partner (enemy 0x37) like a club */
void Em34_swing(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG Hit;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x22, 0, 0xF);
        pEm->Routine_2 = 1;
        EM34_W(pEm)->Flg |= 1;
        if (EM34_W(pEm)->pPartner) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 8;
            EM34_W(pEm)->pPartner->Routine_2 = 0;
            EM34_W(pEm)->pPartner->Spd_y = 0x1400;
            EM34_W(pEm)->pPartner->Spd_x = 0x180;
        }
        pEm->UnkE3 = 0;
        pEm->Unk120 |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        pEm->UnkE2 = 0;
        if (Rnd_l() & 1)
            Snd_se_enem(15, pEm);
    case 1:
        if (pEm->Move_cnt < 33)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x30);
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (pEm->UnkE3) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->UnkE3 = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (!(Rnd_l() & 1))
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (pEm->UnkE2 & 0x80) {
                EM_SET_RTN(pEm, 1, 2, 0, 0);
                if (!(Rnd_l() & 3))
                    EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            }
            EM34_W(pEm)->Unk00 = 15;
            EM34_W(pEm)->Flg &= ~1;
            pEm->Unk120 &= ~3;
        }
        Add_speedXZ(pEm, 0);
        if ((*pEm->pNow_seq & 0x800) && !pPl->Damage_cnt) {
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x10);
            Ofs.vy = 200;
            Ofs.vx = 0;
            Ofs.vz = 0;
            Ofs.pad = 400;
            pP = EM34_W(pEm)->pPartner->pSin_parts_ptr;
            Hit = 0;
            Hit |= func_80044420(&pP[1].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[2].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[3].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[4].Workm, &Ofs, pPl);
            Hit |= func_80044420(&pP[5].Workm, &Ofs, pPl);
            if (Hit) {
                EM_RTN(pPl) = (Cdir_ck_l(pEm, pPl) + 2) << 8 | 2;
                pPl->Damage_cnt |= 0x80;
                Snd_se_stad(0x1020D, &pEm->Pos_x, 0, 0);
                if (Pl_life_down(25, 0) == 2)
                    EM_SET_RTN(pPl, 3, 0, 0, 0);
                Pad_vib_set0(10, 0);
                Pad_vib_set1(5, 250, 0);
                pEm->UnkE2 = -123;
            }
        }
        if (pEm->UnkE2 & 0x7F) {
            pEm->UnkE2--;
            pP = &EM34_W(pEm)->pPartner->pSin_parts_ptr[5];
            Ofs.vx = pP->Workm.t[0];
            Ofs.vy = pP->Workm.t[1];
            Ofs.vz = pP->Workm.t[2];
            Esp_call(0x1700, ((Rnd_l() * 2 + 0x1800) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Ofs, NULL);
        }
        if (pEm->Move_cnt == 39)
            Snd_se_stad(0x1020E, &pEm->Pos_x, 0, 0);
        break;
    }
    if (*pEm->pNow_seq & 0x400) {
        pP = &EM34_W(pEm)->pPartner->pSin_parts_ptr[3];
        Em34_hit_area.pBox = Em34_at_box6;
        /* pPos is declared after the first Em34_hit_area access, so it loses the
           global-alloc priority tie against the %hi(Em34_hit_area) pseudo */
        {
            LONG *pPos = pP->Workm.t;

            Em34_hit_area.pPos = (VECTOR *)pPos;
            Em34_hit_area.Dir = pEm->Cdir_y;
            Em34_hit_area.Id = 1;
            if (!(Rnd_l() & 3))
                Em34_hit_area.Id = 3;
            func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em34_hit_area, &Pl_work);
            Em34_hit_area.pPos = (VECTOR *)pPos;
            Em34_hit_area.pBox = Em34_at_box7;
            Em34_hit_area.Dir = pEm->Cdir_y;
            Em34_hit_area.Id = 2;
            if (!(Rnd_l() & 3))
                Em34_hit_area.Id = 3;
            func_800445C8(pEm->pSin_parts_ptr[5].Workm.t, &pEm->Pos_x, &Em34_hit_area, &Pl_work);
        }
    }
}

void func_80107434(void) {
}

/* motion 18: fires the rocket launcher (Type 2) */
void Em34_rocket(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs = Em34_shot_ofs;
    MATRIX Unused;
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    MATRIX *pM;
    PARTS_W *pP1;
    LONG Dir;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 100;
        EM34_W(pEm)->Flg |= 1;
        EM34_W(pEm)->Unk03 = 2;
        EM34_W(pEm)->Unk08 = 0;
        EM34_W(pEm)->Unk0A = 0;
        EM34_W(pEm)->Flg &= ~0x18;
        Snd_se_enem(11, pEm);
        G.Unk75EA |= 1;
        pEm->Spd_x = 0;
        pEm->Damage_cnt |= 0x80;
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x40);
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 7;
        break;
    case 2:
        Goto01(pEm, (SHORT)Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z) + 0xC0, 0x100);
        Joint_move(pEm, pKan, pSeq, 0x400);
        pEm->Routine_3++;
        if (!(pEm->Status_flg & 0x2000))
            pEm->Routine_3 += 2;
        if (pEm->Unk14A & 1) {
            pEm->Routine_2 = 3;
            pEm->Unk14A = 0;
        }
        break;
    case 3:
        EM_SET_MOVE(pEm, 0x19, 0, 0);
        pEm->Routine_2 = 4;
        Esp_call(0x1000602, (USHORT)(pEm->Cdir_y + EM34_W(pEm)->Unk08) | 0x10000000,
                      &pEm->pSin_parts_ptr[17].Workm, &Ofs, NULL);
        Snd_se_enem(7, pEm);
        G.Unk75EA |= 2;
        pEm->Spd_x = 300;
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (!pEm->UnkE2) {
                pEm->Routine_2 = 5;
            } else {
                pEm->UnkE2--;
                pEm->Routine_2 = 7;
            }
        }
        if (pPl->Damage_cnt && !(EM34_W(pEm)->Flg & 0x80) && pPl->Unk12C)
            pEm->UnkE3 = 1;
        break;
    case 5:
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        pEm->Routine_2 = 6;
    case 6:
        if (Joint_move(pEm, pKan, pSeq, 0x10200)) {
            pEm->Spd_x = 0;
            EM34_W(pEm)->Unk10 = 90;
        }
        break;
    case 7:
        EM_SET_MOVE(pEm, 0x1B, 0, 0);
        pEm->Routine_2 = 8;
        EM34_W(pEm)->Unk0A = 0;
        EM34_W(pEm)->Flg &= ~8;
        G.Unk75EA |= 1;
    case 8:
        if (pEm->Move_cnt == 12) {
            Ofs.vx = 300;
            Ofs.vy = 500;
            Ofs.vz = 0;
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            pM = &((PARTS_W *)pM)[17].Workm;
            Esp_call(0x1001303, 0xA000000, pM, &Ofs, NULL);
            Esp_call(0x1001303, 0x8000000, pM, &Ofs, NULL);
            Ofs.vy = -1500;
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            pM = &((PARTS_W *)pM)[17].Workm;
            Esp_call(0x1001303, 0xA000000, pM, &Ofs, NULL);
            Esp_call(0x1001303, 0x8000000, pM, &Ofs, NULL);
            if (!EM34_W(pEm)->Unk03) {
                pM = (MATRIX *)pEm->pSin_parts_ptr;
                Ofs.vx = 300;
                Ofs.vy = 0;
                Ofs.vz = 0;
                pM = &((PARTS_W *)pM)[17].Workm;
                Esp_call(0x1001203, 0xA000000, pM, &Ofs, NULL);
                Ofs.vx = 300;
                Ofs.vy = -700;
                Ofs.vz = 0;
                Esp_call(0x1001203, 0xA000000, pM, &Ofs, NULL);
            }
        }
        if (Joint_move(pEm, pKan, pSeq, 0x10200)) {
            pEm->Routine_2 = 2;
            pEm->UnkDA = 20;
            EM_SET_MOVE(pEm, 0x1A, 0, 0);
            pEm->Timer0 = 8;
            pEm->Routine_3 = 0;
        }
        break;
    }
    pP = pEm->pSin_parts_ptr;
    pP1 = &pP[1];
    pP1->Cdir_z += Get_dir_xz(pEm->Unk154, pEm->Pos_y - (pPl->Pos_y + 2800));
    RotMatrix_gte((SVECTOR *)&pP[1].Cdir_x, (MATRIX *)pP[1].M);
    if (pEm->Routine_2 >= 2)
        Em34_foot_set(pEm, 0, 1);
}

/* Routine_0 2: damage (RE2 Em2a_Damage); the table is chosen by the hit weapon (Unk12C) */
void Em34_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Unused[2];
    SHORT Life;

    if (pEm->Routine_2 == 0) {
        if (pEm->Unk132 & 0x100)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x400)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x200)
            Em34_knock_back2(pEm);
        if (pEm->Unk132 & 0x800)
            Em34_knock_back(pEm);
        pEm->pSin_parts_ptr[16].Be_flg &= ~1;
    }
    Life = 400;
    if (G.System_flg & 0x100)
        Life = 300;
    if (pEm->Life < Life && !(EM34_W(pEm)->Unk01 & 2) && pEm->Routine_2 == 0) {
        pEm->Unk12C = 32;
        pEm->Life = Life;
        EM34_W(pEm)->Unk01 |= 2;
        EM34_W(pEm)->Flg |= 0x80;
        if (!(EM34_W(pEm)->Flg & 0x1002)) {
            EM34_W(pEm)->Flg &= ~0x1003;
        } else {
            EM34_W(pEm)->Unk01 &= ~2;
            EM34_W(pEm)->Flg &= ~0x80;
        }
    }
    if (EM34_W(pEm)->Flg & 1) {
        EM34_W(pEm)->Flinch = 1;
        EM_RTN(pEm) = pEm->Unk1C4;
        Em34_blood_set(pEm);
        if (pEm->Routine_0 == 4)
            Em34_Sce(pEm, pKan, pSeq);
        else
            Em34_Move(pEm, pKan, pSeq);
        return;
    }
    Em34_damage[(pEm->Unk12C - 1) * 3 + pEm->Routine_1](pEm, pKan, pSeq);
}

/* damage: flinch */
void Em34_dm_flinch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Unused[2];

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = (Rnd_l() & 1) + 0x70013;
        pEm->Routine_2 = 1;
        Em34_blood_set(pEm);
        Snd_se_enem(13, pEm);
        if (pEm->Unk132 & 0x10)
            EM_SET_MOVE(pEm, 0x1E, 0, 7);
        pEm->Unk13A = 600;
        if (EM34_W(pEm)->pPartner) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 0;
            EM34_W(pEm)->pPartner->Routine_2 = 0;
            EM34_W(pEm)->pPartner->Routine_3 = 0;
        }
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (Rnd_l() & 1)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
            if (!(pEm->Type & 1) && (Rnd_l() & 3) == 1)
                EM_SET_RTN(pEm, 1, 0xA, 0, 0);
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* damage: knocked down */
void Em34_dm_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x15, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        Em34_blood_set(pEm);
        EM34_W(pEm)->Flg |= 0x20;
        Snd_se_enem(12, pEm);
        pEm->Unk13A = 600;
        pEm->Type |= 0x2000;
        if (EM34_W(pEm)->pPartner) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 0;
            EM34_W(pEm)->pPartner->Routine_2 = 0;
            EM34_W(pEm)->pPartner->Routine_3 = 0;
        }
        func_8002646C_(pEm, 200, 20, pEm->Col.Unk39);
        if (G.System_flg & 0x80)
            Quake_add(10, 10);
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            pEm->Timer0 = (Rnd_l() & 0x7F) + 60;
            pEm->Damage_cnt &= 0x7F;
            EM34_W(pEm)->Flg |= 3;
            pEm->Status_flg |= 0x800;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* damage without a reaction: back to the motion that was running */
void Em34_dm_no(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM34_W(pEm)->Flinch = 1;
    if (Rnd_l() & 1)
        Snd_se_enem(12, pEm);
    EM_RTN(pEm) = pEm->Unk1C4;
    Em34_blood_set(pEm);
    if (pEm->Routine_0 == 2)
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    Em34_Move(pEm, pKan, pSeq);
}

/* Routine_0 3: dying (RE2 Em2a_Die); the table is chosen by the hit weapon (Unk12C) */
void Em34_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Routine_2 == 0) {
        if (pEm->Unk132 & 0x100)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x400)
            Em34_blood_spray(pEm);
        if (pEm->Unk132 & 0x200)
            Em34_knock_back2(pEm);
        if (pEm->Unk132 & 0x800)
            Em34_knock_back(pEm);
        pEm->pSin_parts_ptr[16].Be_flg &= ~1;
    }
    if (EM34_W(pEm)->Flg & 2) {
        pEm->Life = 0;
        pEm->Damage_cnt &= 0x7F;
        EM_RTN(pEm) = pEm->Unk1C4;
        Em34_blood_set(pEm);
        Em34_Move(pEm, pKan, pSeq);
        return;
    }
    Em34_die[(pEm->Unk12C - 1) * 3 + pEm->Routine_1](pEm, pKan, pSeq);
}

/* die: fall down and burn */
void Em34_die_burn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX *pM;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x17, 0, 0x1F);
        pEm->Routine_2 = 1;
        Snd_se_enem(13, pEm);
        G.Status_flg &= ~0x40000000;
        Em_dead_flg_on_cand(pEm);
        if (G.Status_flg & 0x2000)
            func_80037A90(pEm, 30);
        G.Unk785C = 0;
        if (EM34_W(pEm)->pPartner) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 0;
            EM34_W(pEm)->pPartner->Routine_2 = 0;
            EM34_W(pEm)->pPartner->Routine_3 = 0;
        }
        func_8002646C_(pEm, 200, 20, pEm->Col.Unk39);
    case 1:
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
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            EM_SET_RTN(pEm, 7, 0, 0, 0);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* Routine_0 4: scripted motions (RE2 Em2a_Sce) */
void Em34_Sce(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Type & 0x100)
        func_80016600(pEm, pKan, pSeq);
    else
        Em34_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

/* scripted motion 0: stand */
void Em34_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* scripted motion 1: walk to Dest */
void Em34_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* scripted motion 2: plays the script's motion (Add_dir) from the second animation set */
void Em34_sce02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->Add_dir;
        EM34_W(pEm)->Flg |= 1;
        if (pEm->Unk14A & 0x20)
            pEm->Hokan_flg = 15;
    case 1:
        func_80018EC8(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Routine_2 += Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x100);
        if (!(pEm->Unk14A & 0x10))
            Add_speedXZ(pEm, 0);
        if ((pEm->Unk14A & 8) && (D_800D1F2C_ & 0x2000))
            func_80037A90(pEm, 30);
        break;
    case 2:
        if (pEm->Unk14A & 1) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            EM34_W(pEm)->Flg &= ~1;
        }
        break;
    }
}

/* scripted motion 3: turn around */
void Em34_sce03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x11, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 4, 1, 0, 0);
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            pEm->Cdir_y += 0x800;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* scripted motion 4: plays the script's motion from the second animation set, pauses at frame Unk14C */
void Em34_sce04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->Add_dir;
        EM34_W(pEm)->Flg |= 1;
        pEm->Unk120 |= 3;
        PW_NECK(&pEm->pSin_parts_ptr[2])->N_rot_y = 0;
        if ((pEm->Unk14A & 8) && (D_800D1F2C_ & 0x2000))
            func_80037A90(pEm, 0);
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
            EM34_W(pEm)->Flg &= ~1;
            pEm->Unk120 &= ~3;
            if (pEm->Unk14A & 2) {
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
        }
        break;
    }
}

#include "enemy/common/em34/Em34_sce05.inc.c"

/* scripted motion 6: jump down (Add_dir motion, fall to Unk122, land) */
void Em34_sce06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = pEm->Add_dir + 0xF0000;
        if (pEm->Unk14A & 1)
            pEm->Routine_2 = 1;
        if (pEm->Unk14A & 0x8000) {
            pEm->Routine_2 = 2;
            pEm->Spd_y = 0;
            pEm->Spd_x = 0;
            EM_MOVE(pEm) = pEm->Add_dir + 0x30001;
        }
        EM34_W(pEm)->Flg |= 1;
        pEm->Damage_cnt |= 0x80;
        break;
    case 1:
        func_80018EC8(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x100)) {
            pEm->Hokan_flg = 3;
            pEm->Routine_2 = 2;
            pEm->Spd_y = 0;
            pEm->Move_no++;
            pEm->Spd_x >>= 1;
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
        if (pEm->Pos_y > pEm->Unk122 - 100) {
            pEm->Pos_y = pEm->Unk122;
            pEm->Routine_2 = 4;
        }
        break;
    case 4:
        pEm->Move_cnt = 10;
        pEm->Hokan_flg = 3;
        pEm->Routine_2 = 5;
        pEm->Move_no++;
        Snd_se_enem(10, pEm);
        EM34_W(pEm)->Flg &= ~1;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk14A |= 0x40;
    case 5:
        func_80018EC8(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (pEm->Move_cnt == 10) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Routine_2 = 6;
            if ((pEm->Unk14A |= 0x80) & 2) {
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
                pEm->Damage_cnt &= 0x7F;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 6:
        break;
    }
}

/* scripted motion 7: throws the partner (enemy 0x37/0x3B) */
void Em34_sce07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    SVECTOR Pos;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x22, 0x14, 0xF);
        pEm->Routine_2 = 1;
        if (EM34_W(pEm)->pPartner) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 8;
            EM34_W(pEm)->pPartner->Routine_2 = 1;
            EM34_W(pEm)->pPartner->Move_no = 1;
            EM34_W(pEm)->pPartner->Move_cnt = 20;
            EM34_W(pEm)->pPartner->Hokan_flg = 15;
            EM34_W(pEm)->pPartner->Spd_x = 0x100;
            EM34_W(pEm)->pPartner->Spd_y = 0x1000;
        }
        Snd_se_enem(11, pEm);
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x10);
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 20) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Unk14A = 0x80;
            pEm->Routine_2 = 2;
        }
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 39)
            Snd_se_stad(0x1020E, &pEm->Pos_x, 0, 0);
        if (pEm->Add_dir) {
            pEm->Add_dir--;
            pP = &EM34_W(pEm)->pPartner->pSin_parts_ptr[5];
            Pos.vx = pP->Workm.t[0];
            Pos.vy = pP->Workm.t[1];
            Pos.vz = pP->Workm.t[2];
            Esp_call(0x1700, ((Rnd_l() * 2 + 0x1800) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &Pos, NULL);
        }
        break;
    case 2:
        break;
    }
}

/* scripted motion 9 */
void Em34_sce09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x20, 0, 0xF);
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Move_cnt == 30)
            Snd_se_enem(15, pEm);
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Unk14A = 0x80;
            pEm->Routine_2 = 2;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        break;
    }
}

/* scripted motion 8: the explosion (Type 3): the claw arm is blown off */
void Em34_sce08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pParts;
    PARTS_W *pP;
    MATRIX *pM;
    MATRIX *pM2;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->Add_dir;
        pEm->Type = (pEm->Type & ~0xF) | 3;
        func_8002C680(pEm->pSin_parts_ptr[1].pTmd, 1, 1, 0, 0);
        func_8002C680(pEm->pSin_parts_ptr[2].pTmd, 4, 4, 0, 0);
        func_8002C680(pEm->pSin_parts_ptr[6].pTmd, 1, 1, 0, 0);
        pParts = pEm->pSin_parts_ptr;
        pP = &pParts[17];
        if (G.System_flg & 0x100) {
            pEm->Life = 600;
            pEm->Max_life = 600;
        }
        if (!(pEm->Unk14A & 8)) {
            Esp_call(0x1000203, 0x10000000, &pParts[17].Workm, NULL, NULL);
            EM34_W(pEm)->Flg |= 0x200;
            pP->pSuper = &GsIDMATRIX;
            pP->UnkA2 |= 0x20;
            pParts[17].Be_flg |= 2;
            PW_FREE(pP)->Free[0] = 0;
            PW_FREE(pP)->Free[4] = 0;
            PW_FREE(pP)->Free[2] = 0;
            PW_FREE(pP)->Free[1] = -10;
            PW_FREE(pP)->Free[3] = 0;
            PW_FREE(pP)->Free[6] = 0;
            pP->Poly_rgb = 0x202020;
            Snd_se_stad(0x1000D, &pEm->Pos_x, 0, 0);
        } else {
            pParts[17].Be_flg = 0;
            RotMatrix((SVECTOR *)&pEm->pSin_parts_ptr[1].Cdir_x, (MATRIX *)pEm->pSin_parts_ptr[1].M);
            EM34_W(pEm)->Flinch = 0;
        }
        pM = (MATRIX *)pEm->pSin_parts_ptr;
        pM = &((PARTS_W *)pM)[6].Workm;
        Ofs.vx = 0;
        Ofs.vy = 0;
        Ofs.vz = 0;
        Esp_call(5, 0x10000000, pM, &Ofs, NULL);
        Esp_call(0x1001203, 0x10000000, pM, &Ofs, NULL);
        Ofs.vy = 300;
        Esp_call(5, 0xA000000, pM, &Ofs, NULL);
        pM2 = (MATRIX *)pEm->pSin_parts_ptr;
        pM2 = &((PARTS_W *)pM2)[1].Workm;
        Ofs.vx = 100;
        Ofs.vy = -400;
        Ofs.vz = 300;
        Esp_call(5, 0x12000000, pM2, &Ofs, NULL);
        Ofs.vx = -100;
        Ofs.vy = -300;
        Ofs.vz = 300;
        Esp_call(5, 0x12000000, pM2, &Ofs, NULL);
        EM34_W(pEm)->Unk14 = 300;
        pEm->pSin_parts_ptr[5].pTmd = Em34_tmd_new5;
        pEm->pSin_parts_ptr[8].pTmd = Em34_tmd_new8;
    case 1:
        func_80018EC8(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Routine_2 = 2;
            pEm->Unk14A = 0x80;
            pEm->Move_no++;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
    case 4:
        break;
    case 3:
        func_80018EC8(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400)) {
            pEm->Routine_2 = 4;
            pEm->Unk14A = 0x80;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* scripted motion 10: shooting sequence of the cutscenes */
void Em34_sce10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs = Em34_shot_ofs;
    MATRIX Unused;
    MATRIX *pM;
    LONG Id;

    if (pEm->Unk14A & 0x4000) {
        EM34_W(pEm)->Flinch = 1;
        pEm->Unk14A &= ~0x4000;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x18, 0, 7);
        pEm->Routine_2 = 1;
        Snd_se_enem(11, pEm);
        pEm->Spd_x = 0;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0x1A, 0, 0);
            pEm->Unk14A |= 0x80;
        }
        break;
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x400);
        break;
    case 3:
        EM_SET_MOVE(pEm, 0x19, 0, 0);
        pEm->Routine_2 = 4;
        Id = 6;
        if ((UCHAR)pEm->Type == 2)
            Id = 11;
        Esp_call((Id << 8) | 0x1000002, (USHORT)(pEm->Cdir_y + EM34_W(pEm)->Unk08) | 0x10000000,
                      &pEm->pSin_parts_ptr[17].Workm, &Ofs, NULL);
        Snd_se_enem(7, pEm);
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0x1A, 0, 0);
            pEm->Unk14A |= 0x80;
        }
        break;
    case 5:
        EM_SET_MOVE(pEm, 0x18, 0, 0);
        pEm->Routine_2 = 6;
    case 6:
        if (Joint_move(pEm, pKan, pSeq, 0x10200)) {
            pEm->Routine_2 = 9;
            pEm->Unk14A |= 0x80;
        }
        break;
    case 7:
        EM_SET_MOVE(pEm, 0x1B, 0, 0);
        pEm->Routine_2 = 8;
    case 8:
        if (pEm->Move_cnt == 12) {
            Ofs.vx = 300;
            Ofs.vy = 500;
            Ofs.vz = 0;
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            pM = &((PARTS_W *)pM)[17].Workm;
            Esp_call(0x1001303, 0xA000000, pM, &Ofs, NULL);
            Esp_call(0x1001303, 0x8000000, pM, &Ofs, NULL);
            Ofs.vy = -1500;
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            pM = &((PARTS_W *)pM)[17].Workm;
            Esp_call(0x1001303, 0xA000000, pM, &Ofs, NULL);
            Esp_call(0x1001303, 0x8000000, pM, &Ofs, NULL);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x10200)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0x1A, 0, 0);
            pEm->Unk14A |= 0x80;
        }
        break;
    case 9:
        break;
    }
    if (!(pEm->Unk14A & 0x8000))
        Em34_foot_set(pEm, 0, 1);
}

void func_801094B4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    case 2:
        EM_SET_MOVE(pEm, 6, 0, 0x1F);
        pEm->Routine_2 = 3;
    case 3:
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    }
}

/* scripted motion 12: burning, staggers to Dest with the partner on fire */
void Em34_sce12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX *pM;
    PARTS_W *pP;
    LONG Dx;
    LONG Dz;
    SVECTOR Ofs;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x17, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->Type |= 0x4000;
        EM34_W(pEm)->pPartner->Routine_0 = 1;
        EM34_W(pEm)->pPartner->Routine_1 = 11;
        EM34_W(pEm)->pPartner->Routine_2 = 0;
        EM34_W(pEm)->pPartner->Routine_3 = 0;
        Snd_se_enem(2, pEm);
    case 1:
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
        func_80018EC8(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 88) {
            pEm->Routine_2 = 2;
            pEm->Unk14A |= 1;
        }
        if (pEm->Move_cnt == 52) {
            Snd_se_enem(15, pEm);
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (Rnd_l() * 2 + 0x1000) << 16, &pP->Workm, NULL, NULL);
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (Rnd_l() * 2 + 0x1000) << 16, &pP[2].Workm, NULL, NULL);
        }
        if (pEm->Move_cnt == 60) {
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (Rnd_l() * 2 + 0x1800) << 16, &pP->Workm, NULL, NULL);
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (Rnd_l() * 2 + 0x1800) << 16, &pP[2].Workm, NULL, NULL);
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (Rnd_l() * 2 + 0x1800) << 16, &pP[3].Workm, NULL, NULL);
            pP = pEm->pSin_parts_ptr;
            Esp_call(0, (Rnd_l() * 2 + 0x1800) << 16, &pP[6].Workm, NULL, NULL);
            pP = &pEm->pSin_parts_ptr[1];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[3];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[6];
            PW_RGB(pP)->Rgb0 = 0x40;
            PW_RGB(pP)->Rgb1 = 0x202030;
            pP->UnkA2 |= 4;
        }
        if (pEm->Move_cnt == 18 || pEm->Move_cnt == 38 || pEm->Move_cnt == 56 || pEm->Move_cnt == 82) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 11;
            EM34_W(pEm)->pPartner->Routine_2 = 1;
            EM34_W(pEm)->pPartner->Routine_3 = 0;
            Snd_se_enem(2, pEm);
        }
        break;
    case 2:
        pEm->Routine_2 = 3;
        EM_MOVE(pEm) = pEm->Add_dir + 0x1F0000;
        pEm->UnkDA = (Rnd_l() & 0x1F) + 30;
        pEm->Timer0 = (Rnd_l() & 0xF) + 15;
    case 3:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        func_80018EC8(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        Joint_move(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x80);
        Add_speedXZ(pEm, 0);
        Dx = (pEm->Dest_x - pEm->Pos_x) >> 1;
        Dz = (pEm->Dest_z - pEm->Pos_z) >> 1;
        if (SquareRoot0(Dx * Dx + Dz * Dz) * 2 < 300)
            pEm->Unk14A |= 2;
        if (--pEm->UnkDA == -1) {
            pEm->UnkDA = (Rnd_l() & 0x1F) + 60;
            Snd_se_enem(Rnd_l() % 3 + 13, pEm);
        }
        if (pEm->Move_cnt % 7 == 1) {
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            Ofs.vx = 100 - (Rnd_l() & 1) * 200;
            Ofs.vy = 0;
            Ofs.vz = 100 - (Rnd_l() & 1) * 200;
            pM = &((PARTS_W *)pM)[3].Workm;
            Esp_call(0, ((Rnd_l() * 2 + 0xA00) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
            Esp_call(0x1700, ((Rnd_l() * 2 + 0xA00) << 16) | (USHORT)(Rnd_l() * 16), pM, &Ofs, NULL);
            Esp_call(0x500, 0x10000000, pM, &Ofs, NULL);
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            Esp_call(0x1700, ((Rnd_l() * 2 + 0xA00) << 16) | (USHORT)(Rnd_l() * 16), &((PARTS_W *)pM)[2].Workm, NULL, NULL);
            pM = (MATRIX *)pEm->pSin_parts_ptr;
            pM = &((PARTS_W *)pM)[5].Workm;
            Esp_call(0x1700, ((Rnd_l() * 2 + 0x800) << 16) | (USHORT)(Rnd_l() * 16), pM, NULL, NULL);
            Esp_call(0x500, 0x10000000, pM, NULL, NULL);
        }
        break;
    case 4:
        EM_SET_MOVE(pEm, 0x17, 0, 0x1F);
        pEm->Routine_2 = 5;
        Snd_se_enem(11, pEm);
        EM34_W(pEm)->pPartner->Routine_0 = 1;
        EM34_W(pEm)->pPartner->Routine_1 = 11;
        EM34_W(pEm)->pPartner->Routine_2 = 0;
        EM34_W(pEm)->pPartner->Routine_3 = 0;
    case 5:
        if (pEm->Move_cnt == 17 || pEm->Move_cnt == 35) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 11;
            EM34_W(pEm)->pPartner->Routine_2 = 1;
            EM34_W(pEm)->pPartner->Routine_3 = 0;
        }
        if (pEm->Move_cnt == 50) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 11;
            EM34_W(pEm)->pPartner->Routine_2 = 0;
            EM34_W(pEm)->pPartner->Routine_3 = 0;
        }
        if (pEm->Move_cnt == 67 || pEm->Move_cnt == 90) {
            EM34_W(pEm)->pPartner->Routine_0 = 1;
            EM34_W(pEm)->pPartner->Routine_1 = 11;
            EM34_W(pEm)->pPartner->Routine_2 = 1;
            EM34_W(pEm)->pPartner->Routine_3 = 0;
            Snd_se_enem(2, pEm);
        }
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            pEm->Routine_2 = 6;
            pEm->Unk14A |= 4;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 6:
        break;
    }
}

/* Routine_0 7: dead, the shadow fades and grows */
void Em34_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pKage = pEm->Kage.pKw;

    switch (pEm->Routine_1) {
    case 0:
        pEm->Col.Id = 0;
        pEm->Life = -1;
        pEm->Routine_1 = 1;
        pEm->Unk120 |= 1;
        Em_dead_flg_on_cand(pEm);
        pEm->UnkE2 = 120;
    case 1:
        if ((*(ULONG *)&pKage->Prim[0].r0 & 0xFF) != 0x10) {
            *(ULONG *)&pKage->Prim[0].r0 -= 0x10001;
            *(ULONG *)&pKage->Prim[1].r0 -= 0x10001;
        }
        if ((*(ULONG *)&pKage->Prim[0].r0 & 0xFF00) != 0xAF00) {
            *(ULONG *)&pKage->Prim[0].r0 += 0x100;
            *(ULONG *)&pKage->Prim[1].r0 += 0x100;
        }
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pKage->Half_x += 5;
            pKage->Half_z += 5;
        }
        if (!pEm->UnkE2)
            pEm->Routine_1 = 2;
        break;
    case 2:
        break;
    }
}

/* RE2 PLDAMEGE_EM: the player's damage routine while held by this enemy (G.pPldamage_Em) */
void Em134_Pldamage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em34_pldamage[pEm->Routine_1](pEm, pKan, pSeq);
}

INCLUDE_RODATA("asm/nonmatchings/enemy/em34", Em34_pl_move);

/* player routine while held, 0: thrown to the floor, struggles free with the pad (Gacha_ch) */
void Em134_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM34_MOVE3 Move_tbl = Em34_pl_move;
    EM34_MOVE3 Stop_tbl = Em34_pl_stop;
    ENEMY_WORK *pE = pEm->pEnemy_ptr;
    SHORT Cdir;
    SHORT Old_x;
    SHORT Old_z;
    SHORT Gacha;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10000, &pEm->Pos_x, 0, 0);
        pEm->Unk12C = 24;
    case 1:
        if (pEm->Move_cnt < 8)
            Goto01(pEm, pE->Cdir_y + 0x800, 0x100);
        if (pEm->Move_cnt == 0 || pEm->Move_cnt == 20 || pEm->Move_cnt == 30 || pEm->Move_cnt == 35
            || pEm->Move_cnt == 37 || pEm->Move_cnt == 39) {
            Pad_vib_set0(2, 0);
            Pad_vib_set1(3, 150, 0);
        }
        Cdir = pEm->Cdir_y;
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y = pE->Cdir_y + 0x1000;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y = Cdir;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 3;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            EM_MOVE(pEm) = Move_tbl.Move[pEm->UnkE3];
            Pad_vib_fade_set(60, 150, 0, 0);
            if (pEm->UnkE3 == 2)
                EM_SET_RTN(pEm, 5, 2, 0, 0);
        }
        break;
    case 2:
        pEm->Cdir_y += 0x800;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x += pEm->Spd_x * 4 / 10;
        if (pEm->Move_cnt < (UCHAR)Stop_tbl.Move[pEm->UnkE3])
            Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (pEm->Move_cnt == (UCHAR)Stop_tbl.Move[pEm->UnkE3])
            Snd_se_stad(((Rnd_l() & 1) + 1) | 0x10000, &pEm->Pos_x, 0, 0);
        if (*pEm->pNow_seq & 0x800) {
            Snd_se_stad(0x10229, &pEm->Pos_x, 0, 0);
            Pad_vib_set0(3, 0);
            Pad_vib_set1(4, 250, 0);
            Pad_vib_set0(2, 10);
            Pad_vib_set1(6, 120, 10);
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 3;
            pEm->UnkE2 = 50;
            if (pEm->Life < 100)
                pEm->UnkE2 = 80;
            if (pEm->Life < 40)
                pEm->UnkE2 = 120;
            EM_SET_MOVE(pEm, 6, 0, 7);
            pEm->Be_flg &= ~0x800;
            pE->Be_flg &= ~0x800;
            if (pEm->UnkE3)
                pEm->Cdir_y += 0x800;
            pEm->Timer0 = pEm->Col.D0;
            pEm->Col.D0 = 50;
            pEm->Col.W0 = 50;
        }
        if (pEm->Move_cnt >= (UCHAR)Stop_tbl.Move[pEm->UnkE3] + 1)
            Add_speedXZ(pEm, 0x800);
        break;
    case 3:
        Gacha = Gacha_ch();
        pEm->UnkE2 -= Gacha * 2 + 1;
        if (pEm->UnkE2 < 0)
            pEm->Routine_2 = 5;
        Joint_move2(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt = 0;
        if (Gacha) {
            pEm->Routine_2 = 4;
            pEm->UnkE3 = (Rnd_l() & 3) + 1;
        }
        break;
    case 4:
        Gacha = Gacha_ch();
        pEm->UnkE2 -= Gacha * 5 + 1;
        Joint_move2(pEm, pKan, pSeq, 0x200);
        if (pEm->Move_cnt > pEm->UnkE3) {
            pEm->Routine_2 = 3;
            pEm->Move_cnt = 0;
            pEm->Hokan_flg = 7;
        }
        if (pEm->UnkE2 < 0)
            pEm->Routine_2 = 5;
        break;
    case 5:
        pEm->Routine_2 = 6;
        pEm->UnkE3 = 0;
    case 6:
        pEm->Col.D0 += 10;
        pEm->Col.W0 += 10;
        if (pEm->Timer0 < pEm->Col.D0) {
            pEm->Col.D0 = pEm->Timer0;
            pEm->Col.W0 = pEm->Timer0;
        }
        if (Gacha_ch() && pEm->Life > 20)
            pEm->UnkE3 = 1;
        if (!(pEm->Move_cnt & 1) && pEm->UnkE3 && pEm->Move_cnt < 48) {
            Joint_move2(pEm, pKan, pSeq, 0x200);
            pEm->UnkE3 = 0;
            pEm->Col.D0 += 10;
            pEm->Col.W0 += 10;
        }
        if (pEm->Col.D0 == pEm->Timer0)
            pEm->Damage_cnt &= 0x7F;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Unk120 &= ~0x12;
            pEm->Col.D0 = pEm->Timer0;
            pEm->Col.W0 = pEm->Timer0;
            pEm->Damage_cnt &= 0x7F;
        }
        break;
    }
}

/* player routine while held, 1: thrown away (pEm is the player, pEnemy_ptr the holder) */
void Em134_pldm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM34_MOVE3 Move_tbl = Em34_pl_move;
    ENEMY_WORK *pE = pEm->pEnemy_ptr;
    SHORT Old_x;
    SHORT Old_z;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Col.D0 = pEm->Timer0;
        pEm->Col.W0 = pEm->Timer0;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10002, &pEm->Pos_x, 0, 0);
        pEm->Unk12C = 24;
    case 1:
        if (pEm->Move_cnt < 8)
            Goto01(pEm, pE->Cdir_y + 0x800, 0x40);
        if (pEm->Move_cnt == 0 || pEm->Move_cnt == 20 || pEm->Move_cnt == 30 || pEm->Move_cnt == 35
            || pEm->Move_cnt == 37 || pEm->Move_cnt == 39) {
            Pad_vib_set0(2, 0);
            Pad_vib_set1(3, 150, 0);
        }
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += 0x800;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->Hokan_flg) >> 3;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 5, 0, 2, 0);
            EM_MOVE(pEm) = Move_tbl.Move[pEm->UnkE3];
            if (pEm->UnkE3 == 2)
                EM_SET_RTN(pEm, 5, 2, 0, 0);
            Pad_vib_fade_set(60, 150, 0, 0);
        }
        break;
    }
}

/* player routine while held, 2: impaled by the claw, struggles free with the pad (Gacha_ch) */
void Em134_pldm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pE = pEm->pEnemy_ptr;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        if (pEm->Life > 0)
            Pl_life_down(5, 1);
        pEm->UnkDA = 39;
        if (pEm->Life < 100)
            pEm->UnkDA = 70;
        if (pEm->Life < 40)
            pEm->UnkDA = 110;
        if (pEm->Life < 20)
            pEm->UnkDA = 150;
        if (pEm->Life < 0)
            pEm->UnkDA = 10000;
        Snd_se_stad(0x10001, &pEm->Pos_x, 0, 0);
    case 1:
        pEm->Cdir_y += 0x800;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            pEm->Be_flg &= ~0x800;
            pE->Be_flg &= ~0x800;
        }
        if (pEm->Life >= 0) {
            pEm->UnkDA = pEm->UnkDA - 1 - Gacha_ch() * 3;
            if (pEm->Move_cnt < 41 && pEm->UnkDA < 0) {
                EM_SET_MOVE(pEm, 4, 0, 7);
                pEm->Routine_2 = 3;
                pE->Routine_2 = 2;
                Snd_se_stad(0x1000B, &pEm->Pos_x, 0, 0);
                break;
            }
        }
        if (pEm->Move_cnt == 40) {
            Pl_life_down(500, 0);
            G.System_flg |= 0x800;
        }
        if (pEm->Move_cnt == 60) {
            Snd_se_stad(0x10003, &pEm->Pos_x, 0, 0);
            pEm->Unk120 |= 1;
            pP = &pEm->pSin_parts_ptr[1];
            PW_RGB(pP)->Rgb0 = 0x302020;
            PW_RGB(pP)->Rgb1 = 0x403040;
            pP->UnkA2 |= 4;
            pP = pEm->pSin_parts_ptr;
            PW_RGB(pP)->Rgb0 = 0x302020;
            PW_RGB(pP)->Rgb1 = 0x403040;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[2];
            PW_RGB(pP)->Rgb0 = 0x302020;
            PW_RGB(pP)->Rgb1 = 0x403040;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[5];
            PW_RGB(pP)->Rgb0 = 0x302020;
            PW_RGB(pP)->Rgb1 = 0x403040;
            pP->UnkA2 |= 4;
        }
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    case 3:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Unk120 &= ~0x12;
            pEm->Damage_cnt &= 0x7F;
        }
        Add_speedXZ(pEm, 0x800);
        break;
    }
}

/* RE2 PLDIE_EM: the player's death routine while held by this enemy (G.pPldie_Em) */
void Em134_Pldie(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em34_pldie[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8010AC4C(void) {
}

#include "enemy/common/em34/Em34_foot_set.inc.c"

#include "enemy/common/em34/Em34_blood_set.inc.c"

/* after-routine 0: flinch of the upper body (part 1) after a light hit */
void Em34_after0(ENEMY_WORK *pEm)
{
    PARTS_W *pP2;
    PARTS_W *pP3;
    LONG Spd;
    SVECTOR Rot;
    MATRIX M;
    MATRIX Unused;
    LONG Unused2[4];
    char Spd_tbl[8] = { 8, 12, -8, -12, -4, 0, 4, 0 };

    if (EM34_W(pEm)->Unk10)
        EM34_W(pEm)->Unk10--;
    switch (EM34_W(pEm)->Flinch) {
    case 1:
        Spd = Spd_tbl[Rnd_l() & 7];
        EM34_W(pEm)->Flinch = 2;
        EM34_W(pEm)->Flinch_cnt = 0;
        EM34_W(pEm)->Flinch_dir = 1;
        EM34_W(pEm)->Flinch_spd = Spd;
        if (pEm->Unk132 & 0x10)
            EM34_W(pEm)->Flinch_dir = -1;
    case 2:
        pP2 = pEm->pSin_parts_ptr;
        Rot.vx = 0;
        Rot.vy = -(EM34_W(pEm)->Flinch_cnt * EM34_W(pEm)->Flinch_spd * 8) * EM34_W(pEm)->Flinch_dir;
        Rot.vz = -(EM34_W(pEm)->Flinch_cnt * 64) * EM34_W(pEm)->Flinch_dir;
        RotMatrix(&Rot, &M);
        MulMatrix((MATRIX *)pP2[1].M, &M);
        if (EM34_W(pEm)->Flinch_cnt++ >= 3) {
            EM34_W(pEm)->Flinch = 3;
            EM34_W(pEm)->Flinch_cnt = 16;
        }
        break;
    case 3:
        pP3 = pEm->pSin_parts_ptr;
        Rot.vx = 0;
        Rot.vy = -(EM34_W(pEm)->Flinch_cnt * EM34_W(pEm)->Flinch_spd) * EM34_W(pEm)->Flinch_dir;
        Rot.vz = -(EM34_W(pEm)->Flinch_cnt * 8) * EM34_W(pEm)->Flinch_dir;
        RotMatrix(&Rot, &M);
        MulMatrix((MATRIX *)pP3[1].M, &M);
        if (--EM34_W(pEm)->Flinch_cnt == -1)
            EM34_W(pEm)->Flinch = 0;
        break;
    case 0:
        break;
    }
}

/* after-routine 1: the shed arm (part 17) falls, bounces and burns */
void Em34_after1(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    MATRIX *pM2;
    MATRIX M;
    SVECTOR Sv;
    SHORT Floor;

    if (EM34_W(pEm)->Flg & 4) {
        if (EM34_W(pEm)->Flg & 0x200)
            return;
        pP = &pEm->pSin_parts_ptr[17];
        switch (pP->Unk74) {
        case 0:
            break;
        case 1:
            pM = &pP->Workm;
            pP->Unk74 = 2;
            PW_FREE(pP)->Free[1] = 0;
            PW_FREE(pP)->Free[2] = 2;
            pP->pSuper = &GsIDMATRIX;
            MulLMatrix(&GsIDMATRIX, pM, (MATRIX *)pP->M);
            PW_FREE(pP)->Free[3] = 100;
            PW_FREE(pP)->Free[1] = pEm->Cdir_y + 0x900;
            Sv.vx = 300;
            Sv.vy = 500;
            Sv.vz = 0;
            Esp_call(0x1001203, 0xA000000, pM, &Sv, NULL);
            Sv.vx = 300;
            Sv.vy = -1500;
            Sv.vz = 0;
            Esp_call(0x1001203, 0xA000000, pM, &Sv, NULL);
        case 2:
            Floor = func_8004D720(pP->Workm.t, 400, 0x2000, 0);
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0x44;
            RotMatrix(&Sv, &M);
            MulMatrix((MATRIX *)pP->M, &M);
            PW_FREE(pP)->Free[3] += 25;
            pP->Pos_y += PW_FREE(pP)->Free[3];
            if (pP->Pos_y > pEm->Unk122 - 400) {
                pP->Pos_y = pEm->Unk122 - 400;
                PW_FREE(pP)->Free[3] = -(PW_FREE(pP)->Free[3] >> 2);
                if (--PW_FREE(pP)->Free[2] == 0) {
                    pP->Unk74 = 3;
                    PW_FREE(pP)->Free[2] = 0;
                    PW_FREE(pP)->Free[3] = 150;
                }
                Snd_se_enem(6, pEm);
            }
            if (Floor == pEm->Unk122) {
                Sv.vx = 0;
                Sv.vy = PW_FREE(pP)->Free[1];
                Sv.vz = 0;
                RotMatrix(&Sv, &M);
                Sv.vx = 150;
                Sv.vy = 0;
                Sv.vz = 0;
                ApplyMatrixSV(&M, &Sv, &Sv);
                pP->Pos_x += Sv.vx;
                pP->Pos_z += Sv.vz;
            }
            if ((pP->Poly_rgb & 0xFF) != 0x30)
                pP->Poly_rgb -= 0x10101;
            break;
        case 3:
            Sv.vx = 0;
            Sv.vy = 0x20;
            Sv.vz = 0;
            RotMatrix(&Sv, &M);
            MulMatrix((MATRIX *)pP->M, &M);
            if ((SHORT)func_8004D720(pP->Workm.t, 400, 0x2000, 0) == pEm->Unk122) {
                Sv.vx = 0;
                Sv.vy = PW_FREE(pP)->Free[1];
                Sv.vz = 0;
                RotMatrix(&Sv, &M);
                Sv.vx = PW_FREE(pP)->Free[3];
                Sv.vy = 0;
                Sv.vz = 0;
                ApplyMatrixSV(&M, &Sv, &Sv);
                pP->Pos_x += Sv.vx;
                pP->Pos_z += Sv.vz;
                PW_FREE(pP)->Free[3] -= PW_FREE(pP)->Free[2] * 20;
                PW_FREE(pP)->Free[2]++;
                if (PW_FREE(pP)->Free[3] < 0)
                    pP->Unk74 = 4;
            } else {
                pP->Unk74 = 4;
            }
            break;
        case 4:
            pM2 = &pP->Workm;
            Esp_call(0x1000203, 0x10000000, pM2, NULL, NULL);
            EM34_W(pEm)->Flg |= 0x200;
            PW_FREE(pP)->Free[1] = -10;
            pP->Poly_rgb = 0x202020;
            PW_FREE(pP)->Free[0] = 0;
            PW_FREE(pP)->Free[4] = 0;
            PW_FREE(pP)->Free[2] = 0;
            PW_FREE(pP)->Free[3] = 0;
            PW_FREE(pP)->Free[6] = 0;
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            func_8001B894(0x1203, pM2);
            Snd_se_stad(0x1000D, &pEm->Pos_x, 0, 0);
            break;
        }
    }
}

void func_8010B574(void) {
}

/* knocked back by a hit (Unk132 & 0x800): lean the body away from the player */
void Em34_knock_back(ENEMY_WORK *pEm)
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
    pP3 = &pEm->pSin_parts_ptr[1];
    pP3->UnkA2 |= 0x40;
    pP3 = &pEm->pSin_parts_ptr[9];
    pP3->UnkA2 |= 0x40;
}

#include "enemy/common/em34/Em34_blood_spray.inc.c"

/* knocked back by a strong hit (Unk132 & 0x200) */
void Em34_knock_back2(ENEMY_WORK *pEm)
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
    pP3 = &pEm->pSin_parts_ptr[1];
    pP3->UnkA2 |= 0x40;
    pP3 = &pEm->pSin_parts_ptr[9];
    pP3->UnkA2 |= 0x40;
}

/* after-routine 2: sparks from the claw arm (part 5) while Flg & 0x80 */
void Em34_after2(ENEMY_WORK *pEm)
{
    MATRIX *pM;
    SVECTOR Ofs;

    if (EM34_W(pEm)->Flg & 0x80) {
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
}

/* after-routine 3: Type 3 smokes from part 6 */
void Em34_after3(ENEMY_WORK *pEm)
{
    SVECTOR Spd;

    if ((UCHAR)pEm->Type == 3) {
        if ((pEm->Move_cnt & 0xF) == 1) {
            Spd.vx = 0x40 - (Rnd_l() & 0x7F);
            Spd.vy = 0;
            Spd.vz = 0x40 - (Rnd_l() & 0x7F);
            Esp_call(0x1800, 0xC000000, &pEm->pSin_parts_ptr[6].Workm, &Spd, NULL);
        }
        if ((pEm->Move_cnt & 0xF) == 4 && (Rnd_l() & 1)) {
            Spd.vx = 0x40 - (Rnd_l() & 0x7F);
            Spd.vy = 0;
            Spd.vz = 0x40 - (Rnd_l() & 0x7F);
            Esp_call(0x1800, 0xC000000, &pEm->pSin_parts_ptr[6].Workm, &Spd, NULL);
        }
    }
}

/* before-routine: dodge timer, steps aside when the player aims and fires */
void Em34_before(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = G_PLAYER;
    LONG Div;
    LONG Time;

    EM34_W(pEm)->Flg &= ~0x40;
    if (EM34_W(pEm)->Unk07 & 0x7F)
        EM34_W(pEm)->Unk07--;
    if ((EM34_FLG_L(pEm) & 0xFF000000) == 0x80000000) {
        EM34_W(pEm)->Flg |= 0x40;
        EM34_W(pEm)->Unk07 &= 0x7F;
    }
    if (!(EM34_W(pEm)->Unk07 & 0x80) && *(USHORT *)&pPl->Routine_0 == 0x501 && !Cdir_ck_l(pEm, pPl)
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
                EM34_W(pEm)->Unk07 = Time | 0x80;
        }
    }
}
