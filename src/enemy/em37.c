#include "enemy/em37.h"
#include "door.h"
#include "sca.h"
#include "sld.h"


const EM37_MOTION Em37_motion  = { { 2, 4, 4, 2, 4, 2, 4, 2 } };

/* RE2 EM_MAIN: follow the parent, grow/shrink part 0 towards Size, sink into the ground under parts 3 and 5 */
void func_8010003C(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    USHORT n;
    LONG Under;
    SVECTOR Unused_sv;
    MATRIX Unused_m;
    VECTOR Unused_v;

    Em37_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if ((UCHAR)pEm->Type == 2)
        return;
    pP = pEm->pSin_parts_ptr;
    pEm->Pos_x = pP->Workm.t[0];
    pEm->Pos_y = pP->Workm.t[1];
    pEm->Pos_z = pP->Workm.t[2];
    if (pEm->Routine_1 != 11) {
        pP->Pos_x = -0x13;
        pP->Pos_y = 0xD2;
        pP->Pos_z = 0x78;
        pP->Old_x = -0x13;
        pP->Old_y = 0xD2;
        pP->Old_z = 0x78;
    }
    pP = &pEm->pSin_parts_ptr[1];
    if (EM37_W(pEm)->Size != pP[-1].Size_y) {
        if (EM37_W(pEm)->Size < pP[-1].Size_y) {
            pP[-1].Size_y -= pEm->Spd_x + 0xC0;
            if (EM37_W(pEm)->Size >= pP[-1].Size_y)
                pP[-1].Size_y = EM37_W(pEm)->Size;
        } else {
            pP[-1].Size_y += pEm->Spd_x + 0x10E;
            if (pP[-1].Size_y >= EM37_W(pEm)->Size)
                pP[-1].Size_y = EM37_W(pEm)->Size;
        }
        func_80101854(pEm);
    }
    Under = (SHORT)Sca_get_ground(pP[2].Workm.t, 500, 0x2000, 0) < pP[2].Workm.t[1];
    n = Under;
    if ((SHORT)Sca_get_ground(pP[4].Workm.t, 500, 0x2000, 0) < pP[4].Workm.t[1])
        n = Under + 1;
    if (EM37_W(pEm)->pParent->Damage_cnt & 0x80)
        n = 0;
    pP[-1].Size_y -= n * 0xC0;
    if (pP[-1].Size_y < 0)
        pP[-1].Size_y = 0;
    pP[0].Size_y = pP[-1].Size_y;
    pP[1].Size_y = pP[-1].Size_y;
    pP[0].Pos_y = pP[-1].Size_y * 313 / 4096;
    pP[1].Pos_y = pP[0].Size_y * 617 / 4096;
    pP[2].Pos_y = pP[1].Size_y * 730 / 4096;
    if (EM37_W(pEm)->pParent->Id == 0x35) {
        pP = &pEm->pSin_parts_ptr[3];
        pP[0].Size_y = pP[-1].Size_y;
        pP[1].Size_y = pP[-1].Size_y;
        pP[2].Size_y = pP[-1].Size_y;
        pP[1].Pos_y = pP[0].Size_y * 678 / 4096;
        pP[2].Pos_y = pP[1].Size_y * 599 / 4096;
    }
}

/* RE2 Em37_Init: find the parent (id 0x35 or 0x36) and hang part 0 on its part 5 */
void func_80100370(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    tagGLOBAL *pG;
    ENEMY_WORK **ppEm;
    PARTS_W *pP;
    ENEMY_WORK *pE;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Damage_cnt = 0x80;
    pEm->Life = -1;
    pEm->Max_life = -1;
    pG = &G;
    EM37_W(pEm)->Size = 0;
    pEm->Hit_area.Top = 0;
    pEm->Hit_area.Bottom = 0;
    pEm->Hit_area.Rate0 = 0;
    pEm->Hit_area.Rate1 = 0;
    pEm->Hit_area.R = 0;
    pEm->Unk140 = 0;
    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 0;
    EM_SET_MOVE(pEm, 0, 0, 0);
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    Joint_move(pEm, pKan, pSeq, 0x400);
    EM37_W(pEm)->pParent = NULL;
    ppEm = &pG->pEnemy[2];
    do {
        pE = *ppEm;
        if (pE->Id == 0x36)
            EM37_W(pEm)->pParent = pE;
        if (pE->Id == 0x35)
            EM37_W(pEm)->pParent = pE;
    } while (++ppEm != pG->ppEnemy_end);
    if (EM37_W(pEm)->pParent && (UCHAR)pEm->Type != 2) {
        pP = pEm->pSin_parts_ptr;
        pP->pSuper = &EM37_W(pEm)->pParent->pSin_parts_ptr[5].Workm;
    }
    pP = pEm->pSin_parts_ptr;
    pP[0].Be_flg |= 0x20;
    pP[3].Be_flg |= 0x20;
    if ((UCHAR)pEm->Type != 2) {
        pP->Pos_x = -0x13;
        pP->Pos_y = 0xD2;
        pP->Pos_z = 0x78;
        pP->Old_x = -0x13;
        pP->Old_y = 0xD2;
        pP->Old_z = 0x78;
        pEm->Be_flg |= 0x200000;
        pP = pEm->pSin_parts_ptr;
        pP->Size_x = 0x1000;
        pP->Size_y = 0;
        pP->Size_z = 0x1000;
        pP->Be_flg |= 4;
        pP++;
        pP->Size_x = 0x1000;
        pP->Size_y = 0;
        pP->Size_z = 0x1000;
        pP->Be_flg |= 4;
        pP++;
        pP->Size_x = 0x1000;
        pP->Size_y = 0;
        pP->Size_z = 0x1000;
        pP->Be_flg |= 4;
        pP++;
        if (EM37_W(pEm)->pParent->Id == 0x35) {
            pP->Size_x = 0x1000;
            pP->Size_y = 0;
            pP->Size_z = 0x1000;
            pP->Be_flg |= 4;
            pP++;
            pP->Size_x = 0x1000;
            pP->Size_y = 0;
            pP->Size_z = 0x1000;
            pP->Be_flg |= 4;
            pP++;
            pP->Size_x = 0x1000;
            pP->Size_y = 0;
            pP->Size_z = 0x1000;
            pP->Be_flg |= 4;
        }
    }
}

/* RE2 Em37_move */
void func_801005A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em37_move_type[(UCHAR)pEm->Type](pEm, pKan, pSeq);
}

void func_801005DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em37_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80100618(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em37_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80100654(void) {
}

/* mv[0]: idle, random sways; follows the parent's state */
void func_8010065C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM37_MOTION Tbl = Em37_motion;
    UCHAR *pMotion = Tbl.No;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 0x3F);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0;
        pEm->UnkE2 = (Rnd() & 0x1F) + 30;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x40);
        if (EM37_W(pEm)->pParent->Routine_0 == 2 || EM37_W(pEm)->pParent->Routine_0 == 3)
            Joint_move(pEm, pKan, pSeq, 0x40);
        if (EM37_W(pEm)->pParent->Routine_0 == 7) {
            pEm->Routine_2 = 2;
            pEm->Hokan_flg = 15;
            pEm->Move_cnt = 0;
        }
        if (pEm->pSin_parts_ptr->Size_y == 0)
            pEm->Spd_x = 0;
        if (--pEm->UnkE2 == -1) {
            pEm->Move_cnt >>= 1;
            pEm->Move_no = Tbl.No[Rnd() & 7];
            pEm->Hokan_flg = 0x3F;
            pEm->UnkE2 = (Rnd() & 0x1F) + 30;
        }
        break;
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Move_cnt--;
        if (pEm->Hokan_flg == 0) {
            pEm->Hokan_flg = 15;
            pEm->Move_cnt++;
            if (--pEm->UnkE2 == -1) {
                pEm->Move_cnt >>= 1;
                pEm->Move_no = pMotion[Rnd() & 7];
                pEm->UnkE2 = (Rnd() & 3) + 6;
            }
        }
        break;
    }
}

void func_80100894(void) {
}

/* mv[1] */
void func_8010089C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Move_no = 8;
        pEm->Hokan_flg = 15;
        if (pEm->Type & 1)
            pEm->Move_no++;
        pEm->Routine_2 = 1;
        pEm->Spd_x = 0x80;
        EM37_W(pEm)->Size = pEm->Timer0;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (pEm->Move_cnt == 30)
            EM37_W(pEm)->Size = 0;
    }
}

void func_8010094C(void) {
}

/* mv[2] */
void func_80100954(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xA, 0, 0xF);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0xC00;
        pEm->Spd_x = 0x80;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (pEm->Move_cnt == 35)
            EM37_W(pEm)->Size = 0;
    }
}

void func_801009E4(void) {
}

/* mv[3] */
void func_801009EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Move_no = 11;
        pEm->Hokan_flg = 15;
        if (pEm->Type & 1)
            pEm->Move_no++;
        pEm->Routine_2 = 1;
        pEm->Spd_x = 0x100;
    case 1:
        EM37_W(pEm)->Size = 0x1400;
        if (pEm->Timer0 < 3500)
            EM37_W(pEm)->Size = 0x1000;
        if (pEm->Timer0 < 2000)
            EM37_W(pEm)->Size = 0x800;
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
    }
}

void func_80100AB4(void) {
}

/* mv[4] */
void func_80100ABC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xD, 0, 7);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0x1000;
        pEm->Spd_x = 0;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            EM_SET_MOVE(pEm, 0xE, 0, 0);
        }
        break;
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
}

void func_80100B8C(void) {
}

/* mv[5] */
void func_80100B94(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x10, 0, 0xF);
        if (pEm->Type & 1)
            pEm->Move_no++;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Move_cnt < 46) {
            if (pEm->Timer0 < 0x400)
                pEm->Timer0 = 0x400;
            EM37_W(pEm)->Size = (pEm->Timer0 << 12) / 1660;
            pEm->Spd_x = 0x200;
            if (EM37_W(pEm)->Size > 0xC00)
                EM37_W(pEm)->Size += 0x400;
            if (EM37_W(pEm)->Size > 0x1600)
                EM37_W(pEm)->Size = 0x1A00;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (pEm->Move_cnt == 50)
            EM37_W(pEm)->Size = 0;
    }
}

void func_80100CBC(void) {
}

/* mv[6] */
void func_80100CC4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x12, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Spd_x = 0;
    case 1:
        if (pEm->Move_cnt == 8)
            EM37_W(pEm)->Size = 0x1000;
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (pEm->Move_cnt == 20)
            EM37_W(pEm)->Size = 0;
    }
}

void func_80100D5C(void) {
}

/* mv[7] */
void func_80100D64(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0xF);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0x1000;
        pEm->Spd_x = 0x100;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (pEm->Move_cnt == 30)
            EM37_W(pEm)->Size = 0;
    }
}

void func_80100DF0(void) {
}

/* mv[8] */
void func_80100DF8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 0xF);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0;
    case 1:
        if (pEm->Move_cnt == 20)
            EM37_W(pEm)->Size = 0;
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (pEm->Move_cnt == 40) {
            pEm->Spd_x = 0x200;
            EM37_W(pEm)->Size = pEm->Spd_y;
        }
        if (pEm->Move_cnt == 60) {
            EM37_W(pEm)->Size = 0;
            pEm->Spd_x = 0x80;
        }
    }
}

void func_80100EB0(void) {
}

/* mv[9] */
void func_80100EB8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x13, 0, 0xF);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0x1800;
        if (pEm->Routine_3 == 1)
            pEm->Move_cnt = 10;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            EM37_W(pEm)->Size = 0;
        }
        if (pEm->Move_cnt == 40)
            EM37_W(pEm)->Size = 0;
    }
}

void func_80100F58(void) {
}

/* mv[10] */
void func_80100F60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x14, 0, 0xF);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0;
        pEm->Spd_x = 0x400;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0, 0, 0);
    }
}

void func_80100FD8(void) {
}

/* mv[11]: grab (UnkE3 picks one of six holds on the parent), then struggle with splash effects */
void func_80100FE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM37_MOTION Tbl = Em37_motion;
    PARTS_W *pP;
    PARTS_W *pP0;
    MATRIX M;
    SVECTOR Sv;

    switch (pEm->Routine_2) {
    case 0:
        pP = pEm->pSin_parts_ptr;
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        PW_RGB(pP)->Rgb0 = 0x40;
        PW_RGB(pP)->Rgb1 = 0x202030;
        pP->UnkA2 |= 4;
        pP = &pEm->pSin_parts_ptr[3];
        PW_RGB(pP)->Rgb0 = 0x40;
        PW_RGB(pP)->Rgb1 = 0x202030;
        pP->UnkA2 |= 4;
    case 1:
        pEm->UnkE2 = (Rnd() & 0x1F) + 30;
        EM_SET_MOVE(pEm, 4, 0, 0x3F);
        EM37_W(pEm)->Size = 0x800;
        pEm->Spd_x = 0x200;
        pEm->Routine_2 = 2;
        pEm->Timer0 = 13;
        pP = pEm->pSin_parts_ptr;
        pP->pSuper = &EM37_W(pEm)->pParent->pSin_parts_ptr[1].Workm;
        switch (pEm->UnkE3) {
        case 0:
            pEm->Cdir_x = 500;
            pEm->Cdir_y = 100;
            pEm->Cdir_z = 200;
            pP->Pos_x = -0x13B;
            pP->Pos_y = -0x41;
            pP->Pos_z = 0x104;
            break;
        case 1:
            pEm->Cdir_x = -200;
            pEm->Cdir_y = 160;
            pEm->Cdir_z = 400;
            pP->Pos_x = -0x120;
            pP->Pos_y = -0x1FE;
            pP->Pos_z = -0x140;
            break;
        case 2:
            pEm->Cdir_x = -80;
            pEm->Cdir_y = -60;
            pEm->Cdir_z = 400;
            pP->Pos_x = -0x10D;
            pP->Pos_y = 0x40;
            pP->Pos_z = -0x5A;
            break;
        case 3:
            pEm->Cdir_x = -500;
            pEm->Cdir_y = 100;
            pEm->Cdir_z = -200;
            pP->Pos_x = 0x13B;
            pP->Pos_y = -0x41;
            pP->Pos_z = -0x104;
            break;
        case 4:
            pEm->Cdir_x = 200;
            pEm->Cdir_y = 160;
            pEm->Cdir_z = -400;
            pP->Pos_x = 0x120;
            pP->Pos_y = -0x1FE;
            pP->Pos_z = 0x140;
            break;
        case 5:
            pEm->Cdir_x = 80;
            pEm->Cdir_y = -60;
            pEm->Cdir_z = -400;
            pP->Pos_x = 0x10D;
            pP->Pos_y = 0x40;
            pP->Pos_z = 0x5A;
            break;
        }
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x40);
        Joint_move(pEm, pKan, pSeq, 0x40);
        pP = pEm->pSin_parts_ptr;
        RotMatrix((SVECTOR *)&pEm->Cdir_x, &M);
        MulMatrix((MATRIX *)pP->M, &M);
        pP = pEm->pSin_parts_ptr;
        if (pP->Size_y == 0)
            pEm->Spd_x = 0;
        if (--pEm->UnkE2 == -1) {
            pEm->Move_cnt >>= 1;
            pEm->Move_no = Tbl.No[Rnd() & 7];
            pEm->Hokan_flg = 0x3F;
            pEm->UnkE2 = (Rnd() & 0x1F) + 30;
        }
        if (--pEm->Timer0 == 0) {
            pEm->Routine_2 = 3;
            pEm->UnkE3++;
        }
        if (!(pEm->Move_cnt & 7)) {
            pP0 = pEm->pSin_parts_ptr;
            Sv.vx = 0x80 - Rnd();
            Sv.vy = 0x80 - Rnd();
            Sv.vz = 0x80 - Rnd();
            Esp_call(0x1700, (Rnd() * 2 + 0x1000) << 16, &pP0[1].Workm, &Sv, NULL);
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0x80 - Rnd();
            Sv.vy = 0x80 - Rnd();
            Sv.vz = 0x80 - Rnd();
            Esp_call((Rnd() & 1) << 8, (Rnd() * 2 + 0xA00) << 16, &pP[3].Workm, &Sv, NULL);
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0x80 - Rnd();
            Sv.vy = 0x80 - Rnd();
            Sv.vz = 0x80 - Rnd();
            Esp_call((Rnd() & 1) << 8, (Rnd() + 0x800) << 16, &pP[4].Workm, &Sv, NULL);
            if (!(pEm->Move_cnt & 7)) {
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 0x80 - Rnd();
                Sv.vy = 0;
                Sv.vz = 0x80 - Rnd();
                Esp_call(0x1700, (Rnd() * 2 + 0x1000) << 16, &pP->Workm, &Sv, NULL);
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 0x80 - Rnd();
                Sv.vy = 0x80 - Rnd();
                Sv.vz = 0x80 - Rnd();
                Esp_call((Rnd() & 1) << 8, (Rnd() * 2 + 0xA00) << 16, &pP[2].Workm, &Sv, NULL);
            }
        }
        break;
    case 3:
        pEm->Spd_x = 0x300;
        EM37_W(pEm)->Size = 0;
        pEm->Routine_2 = 4;
    case 4:
        Joint_move(pEm, pKan, pSeq, 0x40);
        Joint_move(pEm, pKan, pSeq, 0x40);
        pP = pEm->pSin_parts_ptr;
        RotMatrix((SVECTOR *)&pEm->Cdir_x, &M);
        MulMatrix((MATRIX *)pP->M, &M);
        break;
    }
}

void func_80101598(void)
{
    SVECTOR Unused;
}

void func_801015A8(void) {
}

/* RE2 Em37_Scenario */
void func_801015B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Type & 0x100)
        func_80016600(pEm, pKan, pSeq);
    else
        Em37_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

/* Routine_0 7: shrink back, idle sways (parts 3..5 scaled 1:1) */
void func_8010160C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EM37_MOTION Tbl = Em37_motion;
    PARTS_W *pP;
    PARTS_W *pPart;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 0xF);
        pEm->Routine_2 = 1;
        EM37_W(pEm)->Size = 0;
        pEm->UnkE2 = (Rnd() & 0x1F) + 30;
        pP = pEm->pSin_parts_ptr;
        pEm->Spd_x = 0;
        pP[3].Be_flg |= 4;
        pPart = &pP[3];
        pPart->Size_x = 0x1000;
        pPart->Size_y = 0x1000;
        pPart->Size_z = 0x1000;
        pP[4].Be_flg |= 4;
        pPart = &pP[4];
        pPart->Size_x = 0x1000;
        pPart->Size_y = 0x1000;
        pPart->Size_z = 0x1000;
        pP[5].Be_flg |= 4;
        pPart = &pP[5];
        pPart->Size_x = 0x1000;
        pPart->Size_y = 0x1000;
        pPart->Size_z = 0x1000;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Move_cnt--;
        if (pEm->Hokan_flg == 0) {
            pEm->Hokan_flg = 15;
            pEm->Move_cnt++;
            if (--pEm->UnkE2 == -1) {
                pEm->Move_cnt >>= 1;
                pEm->Move_no = Tbl.No[Rnd() & 7];
                pEm->UnkE2 = (Rnd() & 3) + 6;
            }
        }
        pPart = &pEm->pSin_parts_ptr[3];
        if (pPart->Size_y > 0x400) {
            pPart->Size_y -= 16;
            pPart[1].Size_y -= 16;
            pPart[2].Size_y -= 16;
            pPart[1].Pos_y = pPart->Size_y * 678 / 4096;
            pPart[2].Pos_y = pPart[1].Size_y * 599 / 4096;
        }
    }
}

void func_80101844(void) {
}

void func_8010184C(void) {
}

/* splash effects while the part moves (non-void without a return value: v0 stays out of the last delay slot) */
LONG func_80101854(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR Pos;

    if ((pEm->Move_cnt & 7) == 1) {
        Pos.vx = 11;
        Pos.vy = (Rnd() & 0xF) + 0x10C;
        Pos.vz = 50;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x4001700, ((Rnd() * 4 + 0x1400) << 16) | ((Rnd() << 4) & 0xFFFF), &pP->Workm, &Pos, NULL);
        Esp_call(0x4001800, 0xA000000, &pP->Workm, &Pos, NULL);
    }
    if ((pEm->Move_cnt & 7) == 4 && (Rnd() & 1)) {
        Pos.vx = 11;
        Pos.vy = 300;
        Pos.vz = 50;
        pP = pEm->pSin_parts_ptr;
        Esp_call(0x2001700, ((Rnd() * 4 + 0x1400) << 16) | ((Rnd() << 4) & 0xFFFF), &pP->Workm, &Pos, NULL);
        Esp_call(0x2001800, 0xA000000, &pP->Workm, &Pos, NULL);
    }
}
