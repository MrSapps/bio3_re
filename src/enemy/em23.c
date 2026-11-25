#include "enemy/em23.h"
#include "door.h"
#include "joint.h"
#include "root_ck.h"
#include "snd.h"
#include "sub.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");     /* em_sub.c Cdir_ck, int result */
void Em_dead_flg_on_cand_() __asm__("Em_dead_flg_on_cand"); /* unprototyped call */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector"); /* main.h Rot_vector, int Dir */
LONG Guide_check_(LONG X, LONG Z, LONG Floor) __asm__("Guide_check"); /* arg 1 long (canonical short); arg 2 long (canonical short); arg 3 long (canonical UC) */
void Root_ck_(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3) __asm__("Root_ck"); /* returns void here (canonical long); arg 3 long (canonical UC); arg 4 long (canonical UC) */
void func_8002646C_(ENEMY_WORK *pEm, LONG A, LONG B, LONG C) __asm__("func_8002646C"); /* arg 2 long (canonical short); arg 3 long (canonical short); arg 4 long (canonical UC) */

void func_80109174_() __asm__("func_80109174"); /* unprototyped call */

/* RE2 EM_MAIN */
void func_80100144(ENEMY_WORK *pEm)
{
    SVECTOR Sv; /* unused */
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR *pPos;

    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    pPos = (VECTOR *)&pPl->Pos_x;
    if (EF(pEm)->Flg & 8) {
        if (!EF(pEm)->Timer && Guide_check_(pEm->Pos_x, pEm->Pos_z, pEm->nFloor) != 0xFF)
            Root_ck_(pEm, pPos, Rnd_area(), 1);
    } else {
        Root_ck_(pEm, pPos, 0, 0);
    }
    if ((pEm->Col.Hit_flg & 0x80) && EF(pEm)->Mode != 4)
        EF(pEm)->Ground = pEm->Unk122;
    Em23_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    pEm->pEnemy_ptr = pPl;
    pEm->Unk120 &= ~2;
    if (pEm->Type & 0x4000)
        pEm->Unk120 |= 2;
    if (EF(pEm)->Mode != 0 && EF(pEm)->Mode != 4)
        pEm->Be_flg |= 0x8000;
    else
        pEm->Be_flg &= ~0x8000;
    if (EF_W0(pEm) & 0xFFFF00)
        pEm->Be_flg |= 0x400;
    else
        pEm->Be_flg &= ~0x400;
    if (pEm->Water && pEm->Water < pEm->pSin_parts_ptr->Workm.t[1] && (pEm->Move_cnt & 0xF) == 1 && pEm->Spd_x >= 11)
        func_80109234(pEm);
}

/* RE2 Em23_Init */
void func_80100350(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG Dir;
    UCHAR i;
    EM23_FREE *pF;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Damage_cnt = 0;
    EF(pEm)->Unk2C = 0;
    EF(pEm)->Side = 0;
    EF(pEm)->Unk01 = 0;
    EF(pEm)->Mode = 0;
    EF(pEm)->Unk03 = 0;
    pEm->Unk1A4[1] = 0;
    pEm->Unk1AC[1] = 0;
    EF(pEm)->Floor_y = SCA_FLOOR_Y(G.pRoom->pSca);
    EF(pEm)->Timer = 0;
    EF(pEm)->Unk21 = 0;
    EF(pEm)->Flg = 8;
    pF = EF(pEm);
    pF->Unk24 = 0;
    EF(pEm)->Unk22 = 0;
    pF->Unk28 = 0;
    EF(pEm)->Unk26 = 0;
    pEm->Cdir_x = 0;
    pEm->Cdir_z = 0;
    pEm->Col.pSca_hit_data = 0;
    EF(pEm)->Unk11 = 0;
    EF(pEm)->Unk2E = 0;
    EF(pEm)->Unk04 = 0;
    EF(pEm)->Unk1A = 0;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x4B004B0, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
    pEm->Col.pEm = pEm;
    pEm->Col.D0 = pEm->Col.W0 = 800;
    pEm->Col.H = 1800;
    pEm->Col.Type = 2;
    pEm->Col.Unk24 = 0;
    pEm->Col.Unk28 = 0;
    pEm->Col.Unk26 = 0;
    pEm->Col.Flg = 0;
    pEm->Be_flg |= 0x400000;
    func_80033580(&pEm->Col, 0);
    pEm->Unk136 = -1000;
    pEm->Unk134 = 600;
    pEm->Unk138 = 0x50;
    pEm->Unk139 = 0xA0;
    pEm->Unk13A = 800;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    pEm->Neck_no = 14;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[14];
    PW_FREE(pP)[2] = 0x60;
    PW_FREE(pP)[3] = 0x60;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[5] = 0x138;
    EF(pEm)->Ground = func_8004D720(&pEm->Pos_x, 0, -1, 8);
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = Em23_life_easy[(Rnd_l() >> (Rnd_l() & 3)) & 0xF];
    else
        pEm->Max_life = pEm->Life = Em23_life[(Rnd_l() >> (Rnd_l() & 3)) & 0xF];
    EM_SET_MOVE(pEm, 3, 0, 0);
    pEm->Max_life += 10;
    pEm->Life += 10;
    Joint_move(pEm, pKan, pSeq, 0x100);
    if (pEm->Type == 0x4001) {
        pEm->Pos_x = -2622;
        pEm->Unk122 = -6500;
        pEm->Pos_y = -6500;
        pEm->Pos_z = 6000;
        pEm->Cdir_y = 0x400;
        pEm->Unk144 = 4;
        EF(pEm)->Unk01 = 1;
        pEm->Be_flg |= 0x2420;
        pEm->Type |= 0x8100;
        func_80034124(&pEm->Col);
        EM_SET_MOVE(pEm, 0, 0, 7);
        Joint_move(pEm, pKan, pSeq, 0x100);
        EM_SET_RTN(pEm, 4, 0x14, 0, 0);
        EF(pEm)->Flg = 1;
    }
    if (pEm->Type == 0x8002) {
        pEm->Unk122 = 0;
        EF(pEm)->Ground = 0;
        pEm->Cdir_y = 0x400;
        EM_SET_MOVE(pEm, 5, 0, 1);
        Joint_move(pEm, pKan, pSeq, 0x10000);
        pEm->Col.D0 = 400;
        pEm->Col.W0 = 400;
        EF(pEm)->Side = 1;
        EF(pEm)->Unk2A = -8000;
        EF(pEm)->Mode = 4;
        func_80103518(pEm);
        func_801035A8(pEm);
        pEm->Pos_x = 3454;
        pEm->Pos_y = -7500;
        pEm->Pos_z = -6255;
        func_80034124(&pEm->Col);
        EM_SET_MOVE(pEm, 1, 0, 7);
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
        EM_SET_RTN(pEm, 1, 3, 3, 0);
        EF(pEm)->Flg = 2;
    }
    if (pEm->Type == 0x4003) {
        EM_SET_MOVE(pEm, 6, 0, 7);
        pEm->Pos_x = 13100;
        pEm->Pos_y = -4000;
        pEm->Pos_z = 2500;
        pEm->Cdir_y = 0x800;
        pEm->Cdir_z = 0x800;
        EM_SET_RTN(pEm, 4, 1, 0, 0);
        EF(pEm)->Flg = 1;
        pEm->Cdir_x = 0;
        pEm->Be_flg |= 0x2000;
        pEm->Type |= 0x8000;
        for (i = 0; i < 21; i++) {
            pP = &pEm->pSin_parts_ptr[i];
            pP->Poly_rgb = 0;
            pP->UnkA2 &= ~1;
        }
    }
    if (pEm->Type == 0xB) {
        EF(pEm)->Side = 0;
        func_80104D88(pEm);
        pEm->Col.Unk26 = -2000;
        EM_SET_RTN(pEm, 1, 4, 0, 0);
        EM_SET_MOVE(pEm, 2, 0, 0);
        pEm->Pos_y = EF(pEm)->Floor_y;
    }
    if ((pEm->Type & 0xF) >= 12) {
        Dir = pEm->Cdir_y;
        pEm->Cdir_y = 0;
        func_80102C08(pEm, 3000, (pEm->Type << 10) - 0x2800);
        EF(pEm)->Mode = 0;
        pEm->Timer0 = 0;
        func_80102180(pEm, pEm->Col.pSca_hit_data);
        pEm->Col.D0 = 400;
        pEm->Col.W0 = 400;
        func_801037E4(pEm);
        pEm->Dest_x = EF(pEm)->Unk12;
        pEm->Dest_z = EF(pEm)->Unk14;
        func_80103518(pEm);
        func_801036E8(pEm, Dir & 0xFF0);
        EM_SET_MOVE(pEm, 2, 0, 0);
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt = 0;
        EM_SET_RTN(pEm, 1, 3, 3, 0);
    }
    if (pEm->Unk4B & 1)
        pEm->pSin_parts_ptr[14].pTmd = pEm->pUnk174;
    G.pPldamage_Em[pEm->Id - 0x10] = func_8010BE00;
    G.pPldie_Em[pEm->Id - 0x10] = func_8010C478;
    pEm->pSin_parts_ptr[1].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[2].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[5].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[8].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[11].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[14].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[15].Be_flg |= 0x20;
    pEm->pSin_parts_ptr[18].Be_flg |= 0x20;
    pEm->Unk16A = 0;
}

/* RE2 Em23_Move */
void func_801009E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (EF(pEm)->Unk2E)
        EF(pEm)->Unk2E--;
    func_80100C8C(pEm);
    Em23_mv_br[pEm->Routine_1](pEm);
    if ((pEm->Col.Hit_flg & 0x40) && !EF(pEm)->Unk01 && !(pEm->Be_flg & 0x400) && EF(pEm)->Unk03 != 1
        && pEm->Routine_1 != 9 && pEm->Routine_1 != 10)
        func_8002646C_(pEm, 400, 80, pEm->Col.Unk39);
    else
        pEm->Be_flg |= 0x80000;
    Em23_mv[pEm->Routine_1](pEm, pKan, pSeq);
    func_80100B24(pEm);
    func_80100B94(pEm);
}

/* bubbles from part 1 */
void func_80100B24(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX *pM;

    if (!(pEm->Move_cnt & 7) && (Rnd_l() & 1)) {
        pM = &pEm->pSin_parts_ptr[1].Workm;
        Sv.vx = Sv.vy = Sv.vz = 0;
        Esp_call(0x2000500, 0x8000000, pM, &Sv, 0);
    }
}

/* seq sound */
void func_80100B94(ENEMY_WORK *pEm)
{
    VECTOR Pos;
    USHORT pSeq;
    USHORT No;

    pSeq = *pEm->pNow_seq;
    if (!(pSeq & 0xF000))
        return;
    No = (pSeq >> 12) - 1;
    if (No == 10 && (Rnd_l() & 3))
        return;
    if (No == 11) {
        Pos.vx = pEm->Pos_x;
        Pos.vy = pEm->Pos_y;
        Pos.vz = pEm->Pos_z;
        if (G.Unk783E == 4 && G.Unk7840 == 7 && pEm->Water)
            Snd_se_stad(0x10208, &Pos, 0, 0);
    }
    Snd_se_enem(No, pEm);
}

/* shadow size and colour from the height above the ground */
void func_80100C8C(ENEMY_WORK *pEm)
{
    KAGE_WORK *pK = pEm->Kage.pKw;
    LONG H;
    LONG C;

    H = pEm->Unk122 - pEm->Pos_y;
    H >>= 4;
    if (H > 800)
        H = 800;
    if (H < 0)
        H = 0;
    pK->Half_x = 1200 - H;
    pK->Half_z = 1200 - H;
    H >>= 2;
    C = 0x80 - H;
    if (C < 0x10)
        C = 0x10;
    C = C | ((C << 16) | (C << 8));
    *(ULONG *)&pK->Prim[0].r0 &= 0xFF000000;
    *(ULONG *)&pK->Prim[1].r0 &= 0xFF000000;
    *(ULONG *)&pK->Prim[0].r0 |= C;
    *(ULONG *)&pK->Prim[1].r0 |= C;
}
void func_80100D24(ENEMY_WORK *pEm)
{
    LONG T;

    func_80109174_();
    if (G.Unk75E8 & 7) {
        EF(pEm)->Timer = 90;
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
    if (pEm->Routine_2) {
        T = pEm->Timer0;
        pEm->Timer0 = ((LONG)pEm->Unk154 < 2000) ? T + 10 : T + 1;
        if (pEm->Timer0 >= 31) {
            pEm->Timer0 = 0;
            if (++pEm->UnkDA > pEm->UnkE2) {
                EF(pEm)->Timer = 90;
                EM_SET_RTN(pEm, 1, 1, 0, 0);
            }
        }
    }
    if (!(EF(pEm)->Flg & 8))
        EM_SET_RTN(pEm, 1, 2, 0, 0);
}

void func_80100DF8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        if (Rnd_l() & 1)
            EM_SET_MOVE(pEm, 3, 0, 7);
        else
            EM_SET_MOVE(pEm, 4, 0, 7);
        pEm->Timer0 = 0;
        pEm->UnkDA = 0;
        pEm->UnkE2 = (Rnd_l() & 1) ? 15 : 20;
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (pEm->Move_no == 4)
            Add_speedXZ(pEm, 0);
    }
}

void func_80100EDC(ENEMY_WORK *pEm)
{
    if (EF(pEm)->Flg & 8) {
        func_80109174_();
        func_80100F54(pEm);
        return;
    }
    Em23_root_mv[((signed char)EF(pEm)->Flg - 1) & 1](pEm);
}

void func_80100F54(ENEMY_WORK *pEm)
{
    if (G.Unk75E8 & 7) {
        EF(pEm)->Timer = 90;
    } else if (--EF(pEm)->Timer == 0) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
    }
    if (func_801032D0(pEm, 1600) && !(pEm->Col.Hit_flg & 0x40) && func_80102180(pEm, pEm->Col.pSca_hit_data) && !pEm->UnkDA) {
        pEm->Timer0 = 0;
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
}

/* mv00_br: on the floor, player not seen yet */
void func_80101008(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Pos;
    LONG Hit;
    SHORT Ck;
    SHORT Ck2;
    LONG Free;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x320);
    Ck2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Free = func_80108EDC(pEm, 0x901, 0x801);
    Free &= func_80108EDC(pEm, 0xB01, 0xC01);
    pEm->Col.Unk26 = 0;
    if (pEm->UnkDA)
        pEm->UnkDA--;
    if ((LONG)pEm->Unk154 > 4000)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (func_801032D0(pEm, 1600) && !(pEm->Col.Hit_flg & 0x40) && func_80102180(pEm, pEm->Col.pSca_hit_data) && !pEm->UnkDA) {
        pEm->Timer0 = 0;
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    if (pEm->Col.W0 < 800) {
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Col.W0 += 100;
        pEm->Col.D0 = pEm->Col.W0;
    }
    Ck = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x320);
    if (pEm->Id == 0x23) {
        if ((LONG)pEm->Unk154 < 3000 && !Ck && pPl->Routine_0 == 1) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
            if (!(Rnd_l() & 0xF) && Free)
                EM_SET_RTN(pEm, 1, 8, 0, 0);
        }
    } else if ((LONG)pEm->Unk154 < 6000 && pPl->Routine_0 == 1 && Free) {
        EM_SET_RTN(pEm, 1, 8, 2, 0);
    }
    if ((pEm->Col.Unk3A & 0x3FFF) && func_801028B0(pEm, 3000, 0x200, -0x200) && func_80102180(pEm, pEm->Col.pSca_hit_data)) {
        func_801037E4(pEm);
        EF(pEm)->Unk22 = EF(pEm)->Unk12;
        EF(pEm)->Unk24 = EF(pEm)->Unk14;
        Pos.vx = EF(pEm)->Unk12;
        Pos.vz = EF(pEm)->Unk14;
        Pos.vy = pEm->Pos_y;
        Hit = func_800345C0(&Pos, pEm);
        if (!(pEm->Status_flg & 2) && !Hit)
            EM_SET_RTN(pEm, 1, 0x12, 0, 0);
    }
    if ((Rnd_l() & 0x1F) == 1 && func_8010C76C(&G.pEnemy[1]) && (LONG)pEm->Unk154 > 8000 && !(pEm->Col.Hit_flg & 0x20))
        EM_SET_RTN(pEm, 1, 0x15, 0, 0);
    if (EF(pEm)->Unk2E && pEm->Id == 0x28 && (LONG)pEm->Unk154 < 6000 && !Ck2 && (LONG)pEm->Unk154 > 1000) {
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 0x14, 0, 0);
    }
    if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400))
        EM_SET_RTN(pEm, 1, 6, 0, 0);
}

/* mv01_br */
void func_80101364(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Pos;
    LONG Hit;
    SHORT Ck2;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x320);
    Ck2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    func_80108EDC(pEm, 0x901, 0x801);
    func_80108EDC(pEm, 0xB01, 0xC01);
    pEm->Col.Unk26 = 0;
    if (pEm->UnkDA)
        pEm->UnkDA--;
    if (pEm->Unk154 - 2000 > 4000)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (func_801032D0(pEm, 1600) && !(pEm->Col.Hit_flg & 0x40) && func_80102180(pEm, pEm->Col.pSca_hit_data) && !pEm->UnkDA) {
        pEm->Timer0 = 0;
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    if (pEm->Col.W0 < 800) {
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Col.W0 += 100;
        pEm->Col.D0 = pEm->Col.W0;
    }
    if ((pEm->Col.Unk3A & 0x3FFF) && func_801028B0(pEm, 3000, 0x200, -0x200) && func_80102180(pEm, pEm->Col.pSca_hit_data)) {
        func_801037E4(pEm);
        EF(pEm)->Unk22 = EF(pEm)->Unk12;
        EF(pEm)->Unk24 = EF(pEm)->Unk14;
        Pos.vx = EF(pEm)->Unk12;
        Pos.vz = EF(pEm)->Unk14;
        Pos.vy = pEm->Pos_y;
        Hit = func_800345C0(&Pos, pEm);
        if (!(pEm->Status_flg & 2) && !Hit)
            EM_SET_RTN(pEm, 1, 0x12, 0, 0);
    }
    if ((Rnd_l() & 0xF) == 1 && func_8010C76C(&G.pEnemy[1]) && (LONG)pEm->Unk154 > 8000 && !(pEm->Col.Hit_flg & 0x20))
        EM_SET_RTN(pEm, 1, 0x15, 0, 0);
    if (EF(pEm)->Unk2E && pEm->Id == 0x28 && (LONG)pEm->Unk154 < 6000 && !Ck2 && (LONG)pEm->Unk154 > 1000) {
        if (Rnd_l() & 1)
            EM_SET_RTN(pEm, 1, 0x14, 0, 0);
    }
    if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_x, 0xC00))
        EM_SET_RTN(pEm, 1, 6, 0, 0);
}

/* mv01: walking */
void func_80101604(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        if (!(Rnd_l() & 3) && !EF(pEm)->Unk2E)
            EM_SET_MOVE(pEm, 0, 0, 7);
        else
            EM_SET_MOVE(pEm, 1, 0, 7);
        EF(pEm)->Unk20 = 0;
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (Rnd_l() & 1)
                EM_SET_MOVE(pEm, 0, 0, 7);
            else
                EM_SET_MOVE(pEm, 1, 0, 7);
        }
        Add_speedXZ(pEm, 0);
    }
    if (pEm->Spd_x > 12)
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Spd_x - 12);
}

void func_80101718(ENEMY_WORK *pEm)
{
    Em23_tbl_C8E4[((signed char)EF(pEm)->Flg - 1) & 1](pEm);
}

/* mv02_br */
void func_8010175C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    SHORT Ck;
    LONG Free;

    pEm->Col.Unk26 = 0;
    Ck = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x320);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x258);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x80);
    Free = func_80108EDC(pEm, 0x901, 0x801);
    Free &= func_80108EDC(pEm, 0xB01, 0xC01);
    if (pEm->UnkDA)
        pEm->UnkDA--;
    if (pEm->Unk154 - 6001 < 999 && !Ck && !(pEm->Col.Hit_flg & 0x40) && Free && !(Rnd_l() & 7)
        && (EM_D0W(pEm) & 0x20020000) == 0x20000000) {
        pEm->UnkE2 = 0;
        EM_SET_RTN(pEm, 1, 0xB, 0, 0);
    }
    if (pEm->Id == 0x23) {
        if ((LONG)pEm->Unk154 < 2000 && !(Rnd_l() & 7) && Free)
            EM_SET_RTN(pEm, 1, 8, 0, 0);
    } else if ((LONG)pEm->Unk154 < 5000 && Free) {
        EM_SET_RTN(pEm, 1, 8, 2, 0);
    }
    if (func_801032D0(pEm, 1600) && !(pEm->Col.Hit_flg & 0x40) && func_80102180(pEm, pEm->Col.pSca_hit_data) && !pEm->UnkDA) {
        pEm->Timer0 = 0;
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    if (pEm->Col.W0 < 800) {
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Col.W0 += 100;
        pEm->Col.D0 = pEm->Col.W0;
    }
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x258);
    if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400))
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    EF(pEm)->Unk22 = pEm->Dest_x;
    EF(pEm)->Unk24 = pEm->Dest_z;
    if ((pEm->Col.Unk3A || (LONG)pEm->Unk154 > 8000) && func_801028B0(pEm, 2000, 0x200, -0x200)
        && func_80102180(pEm, pEm->Col.pSca_hit_data)) {
        EF(pEm)->pSca = pEm->Col.pSca_hit_data;
        EF(pEm)->Unk22 = (EF(pEm)->pSca->X0 + EF(pEm)->pSca->X1) >> 2;
        EF(pEm)->Unk24 = (EF(pEm)->pSca->Z0 + EF(pEm)->pSca->Z1) >> 2;
    }
    if ((Rnd_l() & 0x3F) == 1 && func_8010C76C(&G.pEnemy[1]) && (LONG)pEm->Unk154 > 8000 && !(pEm->Col.Hit_flg & 0x20))
        EM_SET_RTN(pEm, 1, 0x15, 0, 0);
    if (EF(pEm)->Unk2E && pEm->Id == 0x28 && pEm->Unk154 - 1001 < 4999 && (Rnd_l() & 1))
        EM_SET_RTN(pEm, 1, 0x14, 0, 0);
    if (pEm->Col.Unk3A & 0x3FFF) {
        if (++EF(pEm)->Unk21 >= 11) {
            EF(pEm)->Unk21 = 0;
            if ((LONG)pEm->Unk154 < 5000 && !Ck && !(pEm->Col.Hit_flg & 0x40) && Free
                && (EM_D0W(pEm) & 0x20020000) == 0x20000000) {
                EF(pEm)->Unk2E = 0;
                pEm->UnkE2 = 0;
                EM_SET_RTN(pEm, 1, 0xB, 0, 0);
            } else {
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            }
        }
    }
}

/* mv03_br */
void func_80101B94(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    SHORT Ck;
    SHORT Ck2;
    LONG Free;

    pEm->Col.Unk26 = 0;
    Ck = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x3E8);
    Ck2 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x258);
    Free = func_80108EDC(pEm, 0x901, 0x801);
    Free &= func_80108EDC(pEm, 0xB01, 0xC01);
    if (EF(pEm)->Timer)
        EF(pEm)->Timer--;
    if (pEm->UnkDA)
        pEm->UnkDA--;
    if (!EF(pEm)->Timer) {
        if ((LONG)pEm->Unk154 < 4000 && !Ck && !(pEm->Col.Hit_flg & 0x40) && Free
            && (EM_D0W(pEm) & 0x20020000) == 0x20000000) {
            EF(pEm)->Unk2E = 0;
            pEm->UnkE2 = 0;
            EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
    } else if ((LONG)pEm->Unk154 < 2000 && !Ck2 && pPl->Routine_0 == 1) {
        EM_SET_RTN(pEm, 1, 7, 0, 0);
    }
    if (func_801032D0(pEm, 1600) && !(pEm->Col.Hit_flg & 0x40) && func_80102180(pEm, pEm->Col.pSca_hit_data) && !pEm->UnkDA) {
        pEm->Timer0 = 0;
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    if (pEm->Col.W0 < 800) {
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Col.W0 += 100;
        pEm->Col.D0 = pEm->Col.W0;
    }
    if ((LONG)pEm->Unk154 < 5000 && (pEm->Status_flg & 0x2000)) {
        if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0xC00))
            EM_SET_RTN(pEm, 1, 6, 0, 0);
    } else if ((Rnd_l() & 0x17) == 1 && func_8010C76C(&G.pEnemy[1]) && !(pEm->Col.Hit_flg & 0x20)) {
        EM_SET_RTN(pEm, 1, 0x15, 0, 0);
    }
    EF(pEm)->Unk22 = pEm->Dest_x;
    EF(pEm)->Unk24 = pEm->Dest_z;
    if ((pEm->Col.Unk3A || (LONG)pEm->Unk154 > 10000) && func_801028B0(pEm, 2000, 0x200, -0x200)
        && func_80102180(pEm, pEm->Col.pSca_hit_data)) {
        EF(pEm)->pSca = pEm->Col.pSca_hit_data;
        EF(pEm)->Unk22 = (EF(pEm)->pSca->X0 + EF(pEm)->pSca->X1) >> 2;
        EF(pEm)->Unk24 = (EF(pEm)->pSca->Z0 + EF(pEm)->pSca->Z1) >> 2;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        if (++EF(pEm)->Unk21 >= 11) {
            EF(pEm)->Unk21 = 0;
            EF(pEm)->Timer = 0;
        }
    }
    if ((pEm->Col.Unk3A & 0x8000) && !pPl->Damage_cnt && !Ck2 && pPl->Routine_0 == 1)
        EM_SET_RTN(pEm, 1, 7, 0, 0);
}

/* mv02/mv03: running */
void func_80101F24(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_3 = Rnd_l() & 7;
        if (EF(pEm)->Flg != 3)
            EM_SET_MOVE(pEm, 2, 0, 7);
        else if (Rnd_l() & 1)
            EM_SET_MOVE(pEm, 2, 0, 7);
        else
            EM_SET_MOVE(pEm, 1, 0, 7);
        if (pEm->Status_flg & 2)
            EM_SET_MOVE(pEm, 1, 0, 7);
        EF(pEm)->Unk20 = 0;
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (EF(pEm)->Flg == 1)
            pEm->Spd_x = (SHORT)(pEm->Spd_x * 5) >> 2;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (EF(pEm)->Flg == 3) {
                if (Rnd_l() & 1)
                    EM_SET_MOVE(pEm, 2, 0, 7);
                else
                    EM_SET_MOVE(pEm, 1, 0, 7);
            }
            if (pEm->Routine_3 >= 4)
                pEm->Routine_3 -= 2;
            else
                pEm->Routine_3 = Rnd_l() & 7;
            if (pEm->Status_flg & 2)
                EM_SET_MOVE(pEm, 1, 0, 7);
        }
        Add_speedXZ(pEm, 0);
    }
    if (EF(pEm)->Flg == 1) {
        if (pEm->Move_no == 2 && pEm->Spd_x > 120)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Spd_x - 120);
    } else if (pEm->Move_no == 2 && pEm->Spd_x > 100) {
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Spd_x - 100);
    }
    if (pEm->Move_no == 1 && pEm->Spd_x > 30)
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Spd_x - 30);
}

/* can the wall pS be climbed from here (side of the wall in Unk00) */
LONG func_80102180(ENEMY_WORK *pEm, SCA_DATA *pS)
{
    ULONG Side;

    Side = func_800502B8((VECTOR *)&pEm->Pos_x, pS);
    EF(pEm)->Side = Side;
    if (Side & 1) {
        if (pS->X0 + 800 < pEm->Pos_x && pEm->Pos_x < pS->X1 - 800 && (pS->Type & 0x500))
            goto ok;
    } else if (pS->Z0 + 800 < pEm->Pos_z && pEm->Pos_z < pS->Z1 - 800 && (pS->Type & 0xA00)) {
    ok:
        EF(pEm)->pSca = pEm->Col.pSca_hit_data;
        return 1;
    }
    return 0;
}

/* wall probe from the body ahead (Dist), then up (Up) and sideways (Side) */
LONG func_80102264(ENEMY_WORK *pEm, LONG Dist, LONG Up, LONG Side)
{
    VECTOR P0;
    VECTOR P1;
    SVECTOR Rot;
    SVECTOR Ofs;
    MATRIX M;
    MATRIX M2;
    SCA_DATA *pS;
    SCA_DATA *pEnd;
    LONG *pPos;
    LONG Hit;

    pS = (SCA_DATA *)G.pRoom->pSca;
    pPos = pEm->pSin_parts_ptr->Workm.t;
    Rot.vx = pEm->Cdir_x;
    Rot.vz = pEm->Cdir_z;
    Rot.vy = pEm->Cdir_y;
    RotMatrix_gte(&Rot, &M);
    Ofs.vx = Dist;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    P0.vx = Rot.vx += pEm->pSin_parts_ptr->Workm.t[0];
    P0.vy = Rot.vy += pEm->pSin_parts_ptr->Workm.t[1];
    P0.vz = Rot.vz += pEm->pSin_parts_ptr->Workm.t[2];
    Hit = 0;
    pEnd = pS + G.pRoom->pSca->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x100) {
            if ((pS->Id & 0xF) == 1)
                Hit = func_8004F258(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
        }
    }
    pEm->Col.pSca_hit_data = pS;
    if (!Up || Hit)
        return Hit;
    Rot.vx = 0;
    Rot.vz = 800;
    Rot.vy = 0;
    P1.vx = P0.vx;
    P1.vy = P0.vy;
    P1.vz = P0.vz;
    RotMatrix_gte(&Rot, &M2);
    MulMatrix2(&M, &M2);
    Ofs.vx = Up;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M2, &Ofs, &Rot);
    P1.vx += Rot.vx;
    P1.vy += Rot.vy;
    P1.vz += Rot.vz;
    pS = (SCA_DATA *)G.pRoom->pSca;
    pEnd = pS + ((SCA_HEAD *)pS)->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x100) {
            if ((pS->Id & 0xF) == 1)
                Hit = func_8004F258(SCA_XZ(P0.vx, P0.vz), SCA_XZ(P1.vx, P1.vz), pS);
        }
    }
    pEm->Col.pSca_hit_data = pS;
    if (!Side || Hit)
        return Hit;
    Ofs.vx = -Side;
    Ofs.vz = 0;
    Ofs.vy = 0;
    P0.vx = P1.vx;
    P0.vy = P1.vy;
    P0.vz = P1.vz;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    P1.vx += Rot.vx;
    P1.vy += Rot.vy;
    P1.vz += Rot.vz;
    pS = (SCA_DATA *)G.pRoom->pSca;
    pEnd = pS + ((SCA_HEAD *)pS)->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x100) {
            if ((pS->Id & 0xF) == 1)
                Hit = func_8004F258(SCA_XZ(P0.vx, P0.vz), SCA_XZ(P1.vx, P1.vz), pS);
        }
    }
    pEm->Col.pSca_hit_data = pS;
    return 1 - Hit;
}

/* probe from the body ahead (Dist) and then towards the floor (Down): floor edge test */
LONG func_8010268C(ENEMY_WORK *pEm, LONG Dist, LONG Down)
{
    VECTOR P0;
    VECTOR P1;
    SVECTOR Rot;
    SVECTOR Ofs;
    MATRIX M;
    MATRIX M2;
    SCA_DATA *pS;
    SCA_DATA *pEnd;
    LONG Hit;
    tagGLOBAL *pG = &G;

    Hit = 0;
    Rot.vx = pEm->Cdir_x;
    Rot.vz = pEm->Cdir_z;
    Rot.vy = pEm->Cdir_y;
    RotMatrix_gte(&Rot, &M);
    Ofs.vx = Dist;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    P0.vx = Rot.vx += pEm->pSin_parts_ptr->Workm.t[0];
    P0.vy = Rot.vy += pEm->pSin_parts_ptr->Workm.t[1];
    P0.vz = Rot.vz += pEm->pSin_parts_ptr->Workm.t[2];
    P1.vx = P0.vx;
    P1.vy = P0.vy;
    P1.vz = P0.vz;
    Rot.vx = 0;
    Rot.vy = 0;
    Rot.vz = 800;
    RotMatrix_gte(&Rot, &M2);
    MulMatrix2(&M, &M2);
    Ofs.vx = Down;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M2, &Ofs, &Rot);
    P1.vx += Rot.vx;
    P1.vy += Rot.vy;
    P1.vz += Rot.vz;
    pS = (SCA_DATA *)pG->pRoom->pSca;
    pEnd = pS + ((SCA_HEAD *)pS)->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x200) {
            if ((pS->Id & 0xF) == 1)
                Hit = func_8004F258(SCA_XZ(P0.vx, P0.vz), SCA_XZ(P1.vx, P1.vz), pS);
        }
    }
    return Hit;
}

/* wall probe from the position ahead (Dist, turned by Dir); Flg: probe again, bit 0 = hit there */
LONG func_801028B0(ENEMY_WORK *pEm, LONG Dist, LONG Dir, LONG Flg)
{
    VECTOR P0;
    VECTOR P1; /* unused */
    SVECTOR Rot;
    SVECTOR Ofs;
    MATRIX M;
    MATRIX M2; /* unused */
    SCA_DATA *pS;
    SCA_DATA *pEnd;
    LONG *pPos;
    LONG Hit;

    pS = (SCA_DATA *)G.pRoom->pSca;
    pPos = pEm->pSin_parts_ptr->Workm.t;
    Rot.vx = pEm->Cdir_x;
    Rot.vz = pEm->Cdir_z;
    Rot.vy = pEm->Cdir_y + Dir;
    RotMatrix_gte(&Rot, &M);
    Ofs.vx = Dist;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    P0.vx = Rot.vx += pEm->Pos_x;
    P0.vy = Rot.vy += pEm->Pos_y;
    P0.vz = Rot.vz += pEm->Pos_z;
    Hit = 0;
    pEnd = pS + G.pRoom->pSca->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x100) {
            if ((pS->Id & 0xF) == 1)
                Hit = func_8004F258(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
        }
    }
    pEm->Col.pSca_hit_data = pS;
    Hit <<= 1;
    if (Flg) {
        Rot.vx = pEm->Cdir_x;
        Rot.vz = pEm->Cdir_z;
        Rot.vy = pEm->Cdir_y + Dir;
        RotMatrix_gte(&Rot, &M);
        Ofs.vx = Dist;
        Ofs.vz = 0;
        Ofs.vy = 0;
        ApplyMatrixSV(&M, &Ofs, &Rot);
        P0.vx = Rot.vx += pEm->Pos_x;
        P0.vy = Rot.vy += pEm->Pos_y;
        P0.vz = Rot.vz += pEm->Pos_z;
        pEnd = pS + G.pRoom->pSca->Num;
        {
            while (!(Hit & 1) && ++pS != pEnd) {
                if (pS->Id & 0x100) {
                    if ((pS->Id & 0xF) == 1)
                        Hit |= func_8004F258(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
                }
            }
        }
        if ((Hit & 1) && (!(Hit & 2) || (Rnd_l() & 1)))
            pEm->Col.pSca_hit_data = pS;
    }
    return Hit;
}

/* wall probe from the position to Dist ahead (turned by Dir); result * 2 */
LONG func_80102C08(ENEMY_WORK *pEm, LONG Dist, LONG Dir)
{
    VECTOR P0;
    VECTOR P1; /* unused */
    SVECTOR Rot;
    SVECTOR Ofs;
    MATRIX M;
    MATRIX M2; /* unused */
    SCA_DATA *pS;
    SCA_DATA *pEnd;
    LONG *pPos;
    LONG Hit;

    Hit = 0;
    pS = (SCA_DATA *)G.pRoom->pSca;
    Rot.vx = pEm->Cdir_x;
    Rot.vz = pEm->Cdir_z;
    pPos = &pEm->Pos_x;
    Rot.vy = pEm->Cdir_y + Dir;
    RotMatrix_gte(&Rot, &M);
    Ofs.vx = Dist;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    P0.vx = Rot.vx += pEm->Pos_x;
    P0.vy = Rot.vy += pEm->Pos_y;
    P0.vz = Rot.vz += pEm->Pos_z;
    pEnd = pS + G.pRoom->pSca->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x100) {
            if ((pS->Id & 0xF) == 1)
                Hit = func_8004F258(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
        }
    }
    pEm->Col.pSca_hit_data = pS;
    return Hit << 1;
}

/* floor shape probe from the body to Dist ahead (all shape kinds) */
LONG func_80102DB8(ENEMY_WORK *pEm, LONG Dist)
{
    VECTOR P0;
    VECTOR P1; /* unused */
    SVECTOR Rot;
    SVECTOR Ofs;
    MATRIX M;
    MATRIX M2; /* unused */
    SCA_DATA *pS;
    SCA_DATA *pEnd;
    LONG *pPos;
    LONG Hit;

    Hit = 0;
    pS = (SCA_DATA *)G.pRoom->pSca;
    pPos = pEm->pSin_parts_ptr->Workm.t;
    Rot.vx = pEm->Cdir_x;
    Rot.vz = pEm->Cdir_z;
    Rot.vy = pEm->Cdir_y;
    RotMatrix_gte(&Rot, &M);
    Ofs.vx = Dist;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    P0.vx = Rot.vx += pEm->pSin_parts_ptr->Workm.t[0];
    P0.vy = Rot.vy += pEm->pSin_parts_ptr->Workm.t[1];
    P0.vz = Rot.vz += pEm->pSin_parts_ptr->Workm.t[2];
    pEnd = pS + G.pRoom->pSca->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x200) {
            if ((pS->Id & 0xF) == 1)
                Hit = func_8004F258(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
            if (!(pS->Id & 0xF))
                Hit = func_8004F098(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
            if ((pS->Id & 0xF) == 2)
                Hit = func_8004F498(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
            if ((pS->Id & 0xF) == 3)
                Hit = func_8004F5B4(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
        }
    }
    pEm->Col.pSca_hit_data = pS;
    return Hit;
}

/* floor shape probe from the body to Dist ahead (box/slope shapes only) */
LONG func_80103070(ENEMY_WORK *pEm, LONG Dist)
{
    VECTOR P0;
    VECTOR P1; /* unused */
    SVECTOR Rot;
    SVECTOR Ofs;
    MATRIX M;
    MATRIX M2; /* unused */
    SCA_DATA *pS;
    SCA_DATA *pEnd;
    LONG *pPos;
    LONG Hit;

    Hit = 0;
    pS = (SCA_DATA *)G.pRoom->pSca;
    pPos = pEm->pSin_parts_ptr->Workm.t;
    Rot.vx = pEm->Cdir_x;
    Rot.vz = pEm->Cdir_z;
    Rot.vy = pEm->Cdir_y;
    RotMatrix_gte(&Rot, &M);
    Ofs.vx = Dist;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    P0.vx = Rot.vx += pEm->pSin_parts_ptr->Workm.t[0];
    P0.vy = Rot.vy += pEm->pSin_parts_ptr->Workm.t[1];
    P0.vz = Rot.vz += pEm->pSin_parts_ptr->Workm.t[2];
    pEnd = pS + G.pRoom->pSca->Num;
    while (!Hit && ++pS != pEnd) {
        if (pS->Id & 0x200) {
            if (!(pS->Id & 0xF))
                Hit = func_8004F098(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
            if ((pS->Id & 0xF) == 2)
                Hit = func_8004F498(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
            if ((pS->Id & 0xF) == 3)
                Hit = func_8004F5B4(SCA_XZ(pPos[0], pPos[2]), SCA_XZ(P0.vx, P0.vz), pS);
        }
    }
    pEm->Col.pSca_hit_data = pS;
    return Hit;
}

/* probe from the body to Dist ahead against walls (bit 0) and floor shapes (bit 1) */
LONG func_801032D0(ENEMY_WORK *pEm, LONG Dist)
{
    VECTOR P0;
    VECTOR P1; /* unused */
    SVECTOR Rot;
    SVECTOR Ofs;
    MATRIX M;
    MATRIX M2; /* unused */
    SCA_DATA *pS;
    SCA_DATA *pEnd;
    SCA_DATA *pHit;
    LONG *pT;
    ULONG A;
    ULONG B;
    LONG Hit;
    LONG Res;

    pHit = 0;
    pT = pEm->pSin_parts_ptr->Workm.t;
    pS = (SCA_DATA *)G.pRoom->pSca;
    Rot.vx = pEm->Cdir_x;
    Rot.vz = pEm->Cdir_z;
    Rot.vy = pEm->Cdir_y;
    RotMatrix_gte(&Rot, &M);
    Ofs.vx = Dist;
    Ofs.vz = 0;
    Ofs.vy = 0;
    ApplyMatrixSV(&M, &Ofs, &Rot);
    Res = 0;
    P0.vx = Rot.vx += pEm->pSin_parts_ptr->Workm.t[0];
    P0.vy = Rot.vy += pEm->pSin_parts_ptr->Workm.t[1];
    P0.vz = Rot.vz += pEm->pSin_parts_ptr->Workm.t[2];
    B = SCA_XZ(Rot.vx, Rot.vz);
    A = SCA_XZ(pT[0], pT[2]);
    pEnd = pS + G.pRoom->pSca->Num;
    while (Res != 2 && ++pS != pEnd) {
        if (pS->Id & 0x300) {
            Hit = 0;
            if (pS->Id & 0x100) {
                if ((pS->Id & 0xF) == 1)
                    Hit = func_8004F258(A, B, pS);
            } else {
                if (!(pS->Id & 0xF))
                    Hit = func_8004F098(A, B, pS) << 1;
                if ((pS->Id & 0xF) == 2)
                    Hit = func_8004F498(A, B, pS) << 1;
                if ((pS->Id & 0xF) == 3)
                    Hit = func_8004F5B4(A, B, pS) << 1;
            }
            if (Hit) {
                Res = Hit;
                pHit = pS;
            }
        }
    }
    pEm->Col.pSca_hit_data = pHit;
    return Res & 1;
}

/* base direction for climbing the wall (side in Unk00) */
void func_80103518(ENEMY_WORK *pEm)
{
    MATRIX M; /* unused */
    SVECTOR Sv; /* unused */
    LONG Side;

    Side = EF(pEm)->Side;
    if (Side & 1) {
        pEm->Cdir_z = 0;
        if (Side & 2) {
            pEm->Cdir_y = 0x400;
            pEm->Cdir_x = 0xC00;
        } else {
            pEm->Cdir_y = 0xC00;
            pEm->Cdir_x = 0x400;
        }
    } else {
        pEm->Cdir_z = 0xC00;
        pEm->Cdir_x = 0;
        if (Side & 2)
            pEm->Cdir_y = 0;
        else
            pEm->Cdir_y = 0x800;
    }
    EF(pEm)->Unk06 = pEm->Cdir_x;
    EF(pEm)->Unk08 = pEm->Cdir_y;
    EF(pEm)->Unk0A = pEm->Cdir_z;
}

/* stick to the wall: position from the body, start climbing (up from the floor, down from the ceiling) */
void func_801035A8(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = 758;
    Sv.vz = 0;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, &M);
    ApplyMatrixSV(&M, &Sv, &Sv);
    pEm->Pos_x = pEm->pSin_parts_ptr->Workm.t[0] + Sv.vx;
    pEm->Pos_z = pEm->pSin_parts_ptr->Workm.t[2] + Sv.vz;
    pEm->Pos_y = pEm->pSin_parts_ptr->Workm.t[1] + Sv.vy;
    pEm->Col.Unk26 = -900;
    func_80034124(&pEm->Col);
    if (EF(pEm)->Mode == 4) {
        EF(pEm)->Mode = 2;
        EF(pEm)->Unk04 = 0x800;
        EF(pEm)->Unk1A = 0x800;
    } else {
        EF(pEm)->Mode = 1;
        EF(pEm)->Unk04 = 0;
        EF(pEm)->Unk1A = 0;
    }
    EM23_WALL_DIR_T(pEm);
}

/* stick to the wall at the wall point (Unk12/Unk14), heading Dir */
void func_801036E8(ENEMY_WORK *pEm, LONG Dir)
{
    MATRIX M; /* unused */
    SVECTOR Sv; /* unused */

    pEm->Col.Unk26 = -900;
    pEm->Pos_x = pEm->Old_x = EF(pEm)->Unk12;
    pEm->Pos_z = pEm->Old_z = EF(pEm)->Unk14;
    func_80034124(&pEm->Col);
    EF(pEm)->Unk04 = Dir & 0xFFF;
    if (EF(pEm)->Unk04 > 0x400 && EF(pEm)->Unk04 < 0xC00) {
        EF(pEm)->Mode = 2;
        EF(pEm)->Unk1A = 0x800;
    } else {
        EF(pEm)->Mode = 1;
        EF(pEm)->Unk1A = 0;
    }
    EM23_WALL_DIR_T(pEm);
}

/* wall point in front of the wall shape EF->pSca */
void func_801037E4(ENEMY_WORK *pEm)
{
    SCA_DATA *pS = EF(pEm)->pSca;

    EF(pEm)->Unk2A = pS->High;
    if (EF(pEm)->Side & 1) {
        if (EF(pEm)->Side & 2) {
            EF(pEm)->Unk14 = pS->Z1 + 10;
            EF(pEm)->Unk12 = pEm->Pos_x;
        } else {
            EF(pEm)->Unk14 = pS->Z0 - 10;
            EF(pEm)->Unk12 = pEm->Pos_x;
        }
    } else if (EF(pEm)->Side & 2) {
        EF(pEm)->Unk14 = pEm->Pos_z;
        EF(pEm)->Unk12 = pS->X0 - 10;
    } else {
        EF(pEm)->Unk14 = pEm->Pos_z;
        EF(pEm)->Unk12 = pS->X1 + 10;
    }
}

/* move along the wall by Spd (in the wall plane, direction Unk04 + Add) */
void func_8010387C(ENEMY_WORK *pEm, LONG Add)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vz = 0;
    Sv.vx = 0;
    Sv.vy = EF(pEm)->Unk04 + Add;
    RotMatrix(&Sv, &M);
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
    pEm->Pos_y -= Sv.vx;
    if (EF(pEm)->Side & 1)
        pEm->Pos_x += Sv.vz;
    else
        pEm->Pos_z += Sv.vz;
}

/* turn the wall direction towards (X, Z) by Add (negative: away) */
void func_80103920(ENEMY_WORK *pEm, LONG X, LONG Z, LONG Add)
{
    SHORT A;
    SHORT Dir;
    LONG D;
    LONG R;

    A = Add;
    R = Direction_ck(pEm->Pos_x, pEm->Pos_z, X, Z);
    Dir = R;
    Add <<= 16;
    Add >>= 16;
    if (Add & 0x8000) {
        A = -Add;
        Dir = (R + 0x800) & 0xFFF;
    }
    D = (USHORT)(A + (Dir - EF(pEm)->Unk04)) & 0xFFF;
    if (D < A * 2) {
        EF(pEm)->Unk04 = Dir;
        return;
    }
    EF(pEm)->Unk04 -= A;
    if ((ULONG)D <= 0x800)
        EF(pEm)->Unk04 += A << 1;
    EM23_WALL_DIR(pEm);
}

void func_80103A3C(ENEMY_WORK *pEm)
{
    if (EF(pEm)->Flg & 8) {
        func_80109174_();
        func_80103A90(pEm);
        return;
    }
    func_80103D58(pEm);
}

/* on the wall, root search (Flg & 8) */
void func_80103A90(ENEMY_WORK *pEm)
{
    LONG Old;
    LONG Floor;
    LONG High;

    if (EF(pEm)->Timer)
        EF(pEm)->Timer--;
    Old = EF(pEm)->Ground;
    EF(pEm)->Unk04 &= 0xFFF;
    EF(pEm)->Ground = func_8004D720(&pEm->Col.Pos_x, 0, -1, 8);
    if (EF(pEm)->Ground < pEm->Col.Pos_y)
        EF(pEm)->Ground = Old;
    if (pEm->Routine_2 == 3 || pEm->Routine_2 == 5) {
        if (!EF(pEm)->Unk1A) {
            if (EF(pEm)->Unk04)
                EF(pEm)->Unk04 = (EF(pEm)->Unk04 < 0x800) ? EF(pEm)->Unk04 - 16 : EF(pEm)->Unk04 + 16;
        } else if (EF(pEm)->Unk04 != 0x800) {
            EF(pEm)->Unk04 = (EF(pEm)->Unk04 < 0x800) ? EF(pEm)->Unk04 + 16 : EF(pEm)->Unk04 - 16;
        }
        EM23_WALL_DIR(pEm);
    }
    if (pEm->Routine_2 == 3) {
        pEm->Timer0++;
        Floor = EF(pEm)->Floor_y;
        High = EF(pEm)->Unk2A;
        if (High < Floor) {
            if (pEm->Pos_y - 1600 < Floor && EF(pEm)->Mode == 1) {
                pEm->Timer0 = 0;
                EM_RTN23(pEm) = 11;
            }
        } else if (pEm->Pos_y - 1600 < High && EF(pEm)->Mode == 1) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
        if (EF(pEm)->Ground - 1600 < pEm->Pos_y && EF(pEm)->Mode == 2)
            EM_SET_RTN(pEm, 1, 3, 4, 0);
        if ((EF(pEm)->Mode == 1 || EF(pEm)->Mode == 2)
            && ((EF(pEm)->Unk04 > 0xA80 && EF(pEm)->Unk04 < 0xD80) || (EF(pEm)->Unk04 > 0x280 && EF(pEm)->Unk04 < 0x580))
            && func_801032D0(pEm, 1600)) {
            EM_SET_RTN(pEm, 1, 3, 9, 0);
            pEm->Timer0 = 0;
        }
        if (Old != EF(pEm)->Ground && EF(pEm)->Ground - 600 < pEm->Pos_y && EF(pEm)->Mode != 2 && pEm->Routine_1 != 14) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
    }
}

/* on the wall */
void func_80103D58(ENEMY_WORK *pEm)
{
    LONG Old;
    LONG Floor;
    LONG High;
    LONG Ck;

    Old = EF(pEm)->Ground;
    EF(pEm)->Unk04 &= 0xFFF;
    EF(pEm)->Ground = func_8004D720(&pEm->Col.Pos_x, 0, -1, 8);
    if (EF(pEm)->Ground < pEm->Col.Pos_y)
        EF(pEm)->Ground = Old;
    if (pEm->Routine_2 == 3 || pEm->Routine_2 == 5) {
        if (!EF(pEm)->Unk1A) {
            if (EF(pEm)->Unk04)
                EF(pEm)->Unk04 = (EF(pEm)->Unk04 < 0x800) ? EF(pEm)->Unk04 - 16 : EF(pEm)->Unk04 + 16;
        } else if (EF(pEm)->Unk04 != 0x800) {
            EF(pEm)->Unk04 = (EF(pEm)->Unk04 < 0x800) ? EF(pEm)->Unk04 + 16 : EF(pEm)->Unk04 - 16;
        }
        EM23_WALL_DIR(pEm);
    }
    if (pEm->Routine_2 == 3) {
        pEm->Timer0++;
        if (EF(pEm)->Floor_y > EF(pEm)->Unk2A) {
            if (pEm->Pos_y - 1600 < EF(pEm)->Floor_y && EF(pEm)->Mode == 1) {
                pEm->Timer0 = 0;
                EM_RTN23(pEm) = 11;
            }
        } else if (pEm->Pos_y - 1600 < EF(pEm)->Unk2A && EF(pEm)->Mode == 1) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
        if (EF(pEm)->Ground - 1600 < pEm->Pos_y && EF(pEm)->Mode == 2)
            EM_SET_RTN(pEm, 1, 3, 4, 0);
        if ((EF(pEm)->Mode == 1 || EF(pEm)->Mode == 2)
            && ((EF(pEm)->Unk04 > 0xA80 && EF(pEm)->Unk04 < 0xD80) || (EF(pEm)->Unk04 > 0x280 && EF(pEm)->Unk04 < 0x580))) {
            if (func_801032D0(pEm, 1600)) {
                EM_SET_RTN(pEm, 1, 3, 9, 0);
                pEm->Timer0 = 0;
            }
            Floor = EF(pEm)->Floor_y;
            High = EF(pEm)->Unk2A;
            if (High < Floor)
                Ck = Floor < pEm->Pos_y - 1600;
            else
                Ck = High < pEm->Pos_y - 1600;
            if (Ck && EF(pEm)->Mode == 1) {
                EF(pEm)->Mode = 3;
                EM_SET_RTN(pEm, 1, 5, 0, 0);
            }
            if (pEm->Pos_y < EF(pEm)->Ground - 1600 && EF(pEm)->Mode == 2) {
                EF(pEm)->Mode = 3;
                EM_SET_RTN(pEm, 1, 5, 0, 0);
            }
        }
        if (Old != EF(pEm)->Ground && EF(pEm)->Ground - 600 < pEm->Pos_y && EF(pEm)->Mode != 2 && pEm->Routine_1 != 14) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
    }
}

/* part 0 offset on the wall */
void func_801040A0(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;

    pP->Old_y = -758;
    pP->Old_z = 33;
    pP->Cdir_x = 0x53;
    pP->Cdir_y = 0x40;
    pP->Old_x = 0;
    pP->Cdir_z = 0x2AA;
}

INCLUDE_ASM("asm/nonmatchings/enemy/em23", func_801040D4);

/* on the ceiling: base direction from the side */
void func_80104D88(ENEMY_WORK *pEm)
{
    pEm->Cdir_z = 0x800;
    pEm->Cdir_x = 0;
    switch (EF(pEm)->Side) {
    case 0:
        pEm->Cdir_y = 0x800;
        break;
    case 1:
        pEm->Cdir_y = 0xC00;
        break;
    case 2:
        pEm->Cdir_y = 0;
        break;
    case 3:
        pEm->Cdir_y = 0x400;
        break;
    }
    EF(pEm)->Unk04 = 0;
    EF(pEm)->Mode = 4;
    pEm->Timer0 = 0;
    EF(pEm)->Unk06 = pEm->Cdir_x;
    EF(pEm)->Unk08 = pEm->Cdir_y;
    EF(pEm)->Unk0A = pEm->Cdir_z;
}

void func_80104E20(ENEMY_WORK *pEm)
{
    if (EF(pEm)->Flg & 8)
        func_80109174_();
    if (!pEm->Routine_2) {
        EM_SET_RTN(pEm, 1, 0x11, 0, 0);
        return;
    }
    if (pEm->Timer0 < 20)
        pEm->Timer0++;
    if (pEm->Routine_2 < 3) {
        if (func_801032D0(pEm, 1600) && pEm->Timer0 >= 11) {
            if (func_80102180(pEm, pEm->Col.pSca_hit_data)) {
                pEm->Timer0 = 0;
                EM_RTN23(pEm) = 3;
            } else {
                EM_SET_RTN(pEm, 1, 0x10, 0, 0);
            }
        }
        if ((pEm->Col.Unk3A & 0x3FFF) && !(Rnd_l() & 3))
            EM_SET_RTN(pEm, 1, 0x10, 0, 0);
        if (func_80103070(pEm, 1600))
            EM_SET_RTN(pEm, 1, 0x10, 0, 0);
        if ((LONG)pEm->Unk154 < 3500 && pEm->Timer0 >= 11)
            EM_SET_RTN(pEm, 1, 0x10, 0, 0);
    }
    if (pEm->Col.W0 < 800 && pEm->Routine_2 < 3) {
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Col.Unk26 = -1910;
        pEm->Col.W0 += 100;
        pEm->Col.D0 = pEm->Col.W0;
    }
}

/* mv0D: jump off the wall back to the floor */
void func_80104FA0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd;

    switch (pEm->Routine_2) {
    case 0:
        break;
    case 1:
        pEm->Routine_2 = 2;
        EF(pEm)->Unk20 = 4;
        pEm->Timer0 = 0;
        pEm->Pos_y -= 1358;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0);
        pEm->Spd_x = -pEm->Spd_x;
        pEm->Spd_z = -pEm->Spd_z;
        Spd = pEm->Spd_x;
        pEm->Spd_x = (SHORT)(Spd * 14) / 5;
        pEm->Spd_z = (SHORT)(pEm->Spd_z * 14) / 5;
        Add_speedXZ(pEm, 0);
        if (Spd > 60)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -Spd + 60);
        break;
    case 3:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Routine_2 = 4;
        pEm->Col.D0 = 400;
        pEm->Col.W0 = 400;
        func_801037E4(pEm);
    case 4:
        pEm->Dest_x = EF(pEm)->Unk12;
        pEm->Dest_z = EF(pEm)->Unk14;
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Move_cnt == 45) {
            func_80103518(pEm);
            func_801035A8(pEm);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            func_80109428(pEm);
            EM_SET_RTN(pEm, 1, 3, 2, 0);
        } else if (pEm->Move_cnt >= 10 && pEm->Move_cnt < 21) {
            pEm->Pos_x += (EF(pEm)->Unk12 - pEm->Pos_x) / 10;
            pEm->Pos_z += (EF(pEm)->Unk14 - pEm->Pos_z) / 10;
        }
        if (!pEm->Move_cnt) {
            pEm->Spd_y = 0;
            pEm->Spd_z = 0;
            pEm->Spd_x = 0;
        }
        pEm->Spd_x = -pEm->Spd_x;
        pEm->Spd_z = -pEm->Spd_z;
        Add_speedXZ(pEm, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -32);
        break;
    }
}

/* move on the wall: follow the player, leave the wall at its edges */
void func_80105260(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = G_PLAYER;
    LONG Old;
    LONG Dy;
    LONG Dir;
    LONG X;
    LONG Z;

    Old = EF(pEm)->Ground;
    EF(pEm)->Ground = func_8004D720(&pEm->Col.Pos_x, 0, -1, 8);
    if (EF(pEm)->Ground < pEm->Col.Pos_y)
        EF(pEm)->Ground = Old;
    if (EF(pEm)->Unk11)
        EF(pEm)->Unk11--;
    Dy = __builtin_abs(pEm->Pos_y - pPl->Pos_y);
    Dir = EF(pEm)->Unk04 & 0xFFF;
    X = pEm->Dest_x = G.Player.Pos_x;
    Z = pEm->Dest_z = G.Player.Pos_z;
    EF(pEm)->Unk04 = Dir;
    if (EF(pEm)->Side & 1) {
        if (pEm->Pos_x < X) {
            if (((Dir + 0x400) & 0xFFF) < 0x800)
                EF(pEm)->Unk04 = Dir - 0x20;
            else
                EF(pEm)->Unk04 = Dir + 0x20;
            if (__builtin_abs(EF(pEm)->Unk04 - 0xC00) < 0x30)
                EF(pEm)->Unk04 = 0xC00;
        } else {
            if (Dir < 0x400)
                EF(pEm)->Unk04 = Dir + 0x20;
            else
                EF(pEm)->Unk04 = Dir - 0x20;
            if (__builtin_abs(EF(pEm)->Unk04 - 0x400) < 0x30)
                EF(pEm)->Unk04 = 0x400;
        }
        if (!(EF(pEm)->Flg & 8) && (LONG)pEm->Unk154 < 2000 && Dy < 3000 && pPl->Routine_0 == 1 && !EF(pEm)->Unk11)
            EM_SET_RTN(pEm, 1, 0xD, 0, 0);
        if (((EF(pEm)->Unk04 + 0x600) & 0xFFF) < 0x400 && pEm->Pos_x > X) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
        if (((EF(pEm)->Unk04 - 0x200) & 0xFFF) < 0x400 && pEm->Pos_x < X) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
        if (!func_80102264(pEm, 1500, 1500, 0)) {
            if (!func_80102264(pEm, 1500, 1500, 1200) && !func_8010268C(pEm, 1500, 1500)) {
                if (EF(pEm)->Unk04 > 0xAF0 && EF(pEm)->Unk04 < 0xCE4) {
                    EM_SET_RTN(pEm, 1, 3, 7, 0);
                    pEm->Timer0 = 0;
                    return;
                }
                EF(pEm)->Unk1A = 0x800;
                EF(pEm)->Mode = 2;
                EM_SET_RTN(pEm, 1, 0xE, 0, 0);
                return;
            }
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
    } else {
        if (pEm->Pos_z < Z) {
            if (((Dir + 0x400) & 0xFFF) < 0x800)
                EF(pEm)->Unk04 = Dir - 0x20;
            else
                EF(pEm)->Unk04 = Dir + 0x20;
            if (__builtin_abs(EF(pEm)->Unk04 - 0xC00) < 0x30)
                EF(pEm)->Unk04 = 0xC00;
        } else {
            if (Dir < 0x400)
                EF(pEm)->Unk04 = Dir + 0x20;
            else
                EF(pEm)->Unk04 = Dir - 0x20;
            if (__builtin_abs(EF(pEm)->Unk04 - 0x400) < 0x30)
                EF(pEm)->Unk04 = 0x400;
        }
        if (!(EF(pEm)->Flg & 8) && (LONG)pEm->Unk154 < 2000 && Dy < 3000 && pPl->Routine_0 == 1 && !EF(pEm)->Unk11)
            EM_SET_RTN(pEm, 1, 0xD, 0, 0);
        if (((EF(pEm)->Unk04 + 0x600) & 0xFFF) < 0x400 && pEm->Pos_z > Z) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
        if (((EF(pEm)->Unk04 - 0x200) & 0xFFF) < 0x400 && pEm->Pos_z < Z) {
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
        if (!func_80102264(pEm, 1500, 1500, 0)) {
            if (!func_80102264(pEm, 1500, 1500, 1200) && !func_8010268C(pEm, 1500, 1500)) {
                if (EF(pEm)->Unk04 > 0xA00 && EF(pEm)->Unk04 < 0xE00) {
                    EM_SET_RTN(pEm, 1, 3, 7, 0);
                    pEm->Timer0 = 0;
                    return;
                }
                EF(pEm)->Unk1A = 0x800;
                EF(pEm)->Mode = 2;
                EM_SET_RTN(pEm, 1, 0xE, 0, 0);
                return;
            }
            EF(pEm)->Unk1A = 0x800;
            EF(pEm)->Mode = 2;
            EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        }
    }
    if (pEm->Pos_y - 1600 < EF(pEm)->Unk2A) {
        EF(pEm)->Unk1A = 0x800;
        EF(pEm)->Mode = 2;
        EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    }
    if (pEm->Pos_y - 1600 < EF(pEm)->Floor_y) {
        pEm->Timer0 = 0;
        EF(pEm)->Mode = 1;
        EM_SET_RTN(pEm, 1, 3, 0xB, 0);
    }
    if (pEm->Pos_y < EF(pEm)->Ground - 1100 && ++pEm->Timer0 >= 11 && pEm->Routine_2) {
        EF(pEm)->Unk1A = 0x800;
        EF(pEm)->Mode = 2;
        EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    }
    if (func_80102DB8(pEm, 1600)) {
        EF(pEm)->Unk1A = 0x800;
        EF(pEm)->Mode = 2;
        EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        pEm->Timer0 = 0;
    }
    EM23_WALL_DIR(pEm);
}

void func_8010586C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Spd_get(pEm, pKan, pSeq, 0);
    Joint_move(pEm, pKan, pSeq, 0);
    func_8010387C(pEm, 0);
    func_80109428(pEm);
}

void func_801058D4(ENEMY_WORK *pEm)
{
    if (pEm->Col.W0 < 800) {
        pEm->Col.W0 += 100;
        pEm->Col.D0 = pEm->Col.W0;
    }
}

/* mv06: landing after a jump */
void func_801058F8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Pos;
    LONG Hit;
    LONG Free;

    Free = func_80108EDC(pEm, 0x901, 0x801);
    Free &= func_80108EDC(pEm, 0xB01, 0xC01);
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x258)) {
                EM_SET_RTN(pEm, 1, 2, 0, 0);
                if (!(EF(pEm)->Flg & 1)) {
                    if (Rnd_l() & 1)
                        EF(pEm)->Timer = 250;
                    else
                        EF(pEm)->Timer = 25;
                } else if (!Rnd_l()) {
                    EM_SET_RTN(pEm, 1, 2, 0, 0);
                }
            } else {
                if ((pEm->Status_flg & 0x2000) && Free)
                    EM_SET_RTN(pEm, 1, 0xB, 0, 0);
                else
                    EM_SET_RTN(pEm, 1, 6, 1, 0);
                if (!Rnd_l())
                    EM_SET_RTN(pEm, 1, 0xF, 0, 0);
                if (func_801028B0(pEm, 3000, 0x200, -0x200) && func_80102180(pEm, pEm->Col.pSca_hit_data)) {
                    func_801037E4(pEm);
                    EF(pEm)->Unk22 = EF(pEm)->Unk12;
                    EF(pEm)->Unk24 = EF(pEm)->Unk14;
                    Pos.vx = EF(pEm)->Unk12;
                    Pos.vz = EF(pEm)->Unk14;
                    Pos.vy = pEm->Pos_y;
                    Hit = func_800345C0(&Pos, pEm);
                    if (!(pEm->Status_flg & 2) && !Hit)
                        EM_SET_RTN(pEm, 1, 0x12, 0, 0);
                }
                if (Rnd_l() & 1)
                    EM_SET_RTN(pEm, 1, 6, 1, 0);
            }
        }
    }
    if (pEm->Spd_x > 60) {
        pEm->Spd_x -= 60;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, pEm->Spd_x);
        Add_speedXZ(pEm, 0);
    } else if (pEm->Spd_x > 30) {
        pEm->Spd_x -= 20;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, pEm->Spd_x);
        Add_speedXZ(pEm, 0);
    }
}

void func_80105B70(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Ck;
    ULONG Rtn;

    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Ck = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x190);
    if (pEm->Move_cnt < 30)
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
    if (Ck)
        Rtn = 0x1301;
    else if ((LONG)pEm->Unk154 >= 2501)
        Rtn = 0x601;
    else
        return;
    EM_RTN(pEm) = Rtn;
}

/* mv07: attack with both claws */
void func_80105C14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    HIT_AREA Area;
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xB, 0, 7);
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 0;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if ((*pEm->pNow_seq >> 9) & 1) {
            Sv.vx = 1500;
            Sv.vy = 0;
            Sv.vz = 1200;
            Sv.pad = 1200;
            Area.pPos = (VECTOR *)pEm->pSin_parts_ptr->Workm.t;
            Area.pBox = (SHORT *)&Sv;
            Area.Dir = pEm->Cdir_y;
            Area.Id = 0x80;
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        }
        Sv.vx = 400;
        Sv.vz = 0;
        Sv.vy = 0;
        Sv.pad = 600;
        if (((*pEm->pNow_seq >> 11) & 1) && func_80044420(&pEm->pSin_parts_ptr[10].Workm, &Sv, pPl)
            && !pPl->Damage_cnt && !pEm->UnkE2) {
            pEm->UnkE2++;
            Pad_vib_set0(7, 0);
            Pad_vib_set1(3, 0xFA, 4);
            if (!(pPl->Damage_cnt & 0x80)) {
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                if (Pl_life_down(10, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
            }
            Snd_se_enem(4, pEm);
        }
        if (((*pEm->pNow_seq >> 10) & 1) && func_80044420(&pEm->pSin_parts_ptr[13].Workm, &Sv, pPl)
            && !pPl->Damage_cnt && !pEm->UnkE3) {
            pEm->UnkE3++;
            Pad_vib_set0(7, 0);
            Pad_vib_set1(3, 0xFA, 4);
            if (!(pPl->Damage_cnt & 0x80)) {
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                if (Pl_life_down(15, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
            }
            Snd_se_enem(4, pEm);
        }
        Sv.vx = 400;
        Sv.vz = 0;
        Sv.vy = 0;
        if (pEm->UnkE2 && ++pEm->UnkE2 < 10)
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, 0);
        if (pEm->UnkE3 && ++pEm->UnkE3 < 10)
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[13].Workm, &Sv, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            if ((LONG)pEm->Unk154 < 1800)
                EM_SET_RTN(pEm, 1, 0xF, 0, 0);
            else
                EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        Add_speedXZ(pEm, 0);
    }
}

/* grab the player (Col.H 3000 while holding) */
void func_80105FC0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    UCHAR Rno;
    UCHAR Dmg;

    if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 500) && (pEm->Col.Unk3A & 0x8000) && (Rno = pPl->Routine_0) == 1
        && !((pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40) && !(Dmg = pPl->Damage_cnt)) {
        if (pEm->Id == 0x28) {
            if (pEm->Routine_2 == 5) {
                pPl->Damage_cnt = Dmg | 0x80;
                EM_SET_RTN(pEm, 1, 9, 0, 0);
                EF(pEm)->Unk03 = Rno;
                pEm->Col.H = 3000;
            } else {
                EF(pEm)->Unk03 = 0;
                pEm->Col.H = 1800;
            }
        } else if (pEm->Move_cnt >= 15 && pEm->Move_cnt < 27) {
            pPl->Damage_cnt = Dmg | 0x80;
            EM_SET_RTN(pEm, 1, 9, 0, 0);
            EF(pEm)->Unk03 = Rno;
            pEm->Col.H = 3000;
        } else {
            EF(pEm)->Unk03 = 0;
            pEm->Col.H = 1800;
        }
    }
}

/* mv08: jump at the player */
void func_801060D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR Unused;
    SVECTOR Sv;
    HIT_AREA Area;
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xC, 0, 7);
        Snd_se_enem(13, pEm);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Col.H = 1800;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        pEm->Spd_x = (SHORT)(pEm->Spd_x * 5) >> 2;
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        EM_SET_MOVE(pEm, 0x21, 0, 7);
        Snd_se_enem(13, pEm);
        pEm->Timer0 = Rnd_l() & 7;
        pEm->Routine_2++;
    case 3:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        EM_SET_MOVE(pEm, 0x22, 0, 7);
        EF(pEm)->Unk03 = 1;
        pEm->Routine_2++;
    case 5:
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 177;
            pEm->Spd_z = 6;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            if (!pEm->Timer0)
                pEm->Routine_2++;
            else
                pEm->Timer0--;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 6:
        EM_SET_MOVE(pEm, 0x23, 0, 7);
        EF(pEm)->Unk03 = 0;
        pEm->Col.H = 1800;
        pEm->Routine_2++;
    case 7:
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 175;
            pEm->Spd_z = -6;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        Add_speedXZ(pEm, 0);
        break;
    }
    if (EM_RTN01(pPl) != 0x701)
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 40);
    else
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 10);
    Sv.vx = 1400;
    Sv.vy = 200;
    Sv.vz = 1200;
    Sv.pad = 1200;
    Area.pPos = (VECTOR *)pEm->pSin_parts_ptr[14].Workm.t;
    Area.pBox = (SHORT *)&Sv;
    Area.Dir = pEm->Cdir_y;
    Area.Id = 0x81;
    func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
}

void func_801063DC(void) {
}

/* pl01: bite the held player */
void func_801063E4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    ENEMY_WORK *pPl = &Pl_work;
    LONG X;
    LONG Z;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xE, 0, 3);
        pEm->Col.H = 4000;
        pEm->Routine_2++;
        EM_RTN(pPl) = (Cdir_ck(pEm, pPl) << 24) | 5;
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pEm->Be_flg |= 0x8800;
        pPl->Be_flg |= 0x8800;
        Snd_se_enem(5, pEm);
    case 1:
        func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
        X = pEm->Pos_x;
        Z = pEm->Pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            pEm->Routine_2++;
        pEm->Pos_x -= ((pEm->Pos_x - X) * pEm->Hokan_flg) >> 2;
        pEm->Pos_z -= ((pEm->Pos_z - Z) * pEm->Hokan_flg) >> 2;
        break;
    case 2:
        EM_SET_MOVE(pEm, 0xF, 0, 7);
        Set_base_pos(pPl, pKan, pSeq, 0);
        pEm->Routine_2++;
    case 3:
        func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
        X = pEm->Pos_x;
        Z = pEm->Pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2++;
            if (Pl_life_down(30, 0) & 2)
                EM_SET_RTN(pEm, 1, 0xA, 0, 0);
        }
        if (pEm->Move_cnt == 22 || pEm->Move_cnt == 30 || pEm->Move_cnt == 60) {
            Sv.vx = 100;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x500, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[1].Workm, &Sv, 0);
            Sv.vx = 0;
            Sv.vz = 0;
            Sv.vy = 0;
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[1].Workm, &Sv, 0);
        }
        if (Gacha_ch()) {
            func_80100B94(pEm);
            if (Joint_move(pEm, pKan, pSeq, 0x100)) {
                pEm->Routine_2++;
                if (Pl_life_down(30, 0) & 2)
                    EM_SET_RTN(pEm, 1, 0xA, 0, 0);
            }
            if (pEm->Move_cnt == 22 || pEm->Move_cnt == 30 || pEm->Move_cnt == 60) {
                Sv.vx = 100;
                Sv.vz = 0;
                Sv.vy = 0;
                Esp_call(0x500, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[1].Workm, &Sv, 0);
                Sv.vx = 0;
                Sv.vz = 0;
                Sv.vy = 0;
                Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[1].Workm, &Sv, 0);
            }
        }
        pEm->Pos_x -= ((pEm->Pos_x - X) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Z) * pEm->Hokan_flg) >> 3;
        break;
    case 4:
        EM_SET_MOVE(pEm, 0x10, 0, 7);
        pEm->Routine_2++;
    case 5:
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 33;
            pEm->Spd_z = -7;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
            EF(pEm)->Unk03 = 0;
            pEm->Col.H = 1800;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_80106848(void) {
}

/* pl00: hold the player */
void func_80106850(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x11, 0, 7);
        pEm->Routine_2++;
        EM_SET_RTN(pPl, 6, 0, 0, 0);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pEm->Be_flg |= 0x8800;
        Pl_work.Be_flg |= 0x8800;
    case 1:
        pEm->Base_pos_x = pPl->Base_pos_x;
        pEm->Base_pos_z = pPl->Base_pos_z;
        Null_pos_set(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            pEm->Routine_2++;
    }
}

void func_80106990(ENEMY_WORK *pEm)
{
    LONG Free;
    ENEMY_WORK *pPl = &Pl_work;

    Free = func_80108EDC(pEm, 0x901, 0x801);
    Free &= func_80108EDC(pEm, 0xB01, 0xC01);
    if (pEm->Routine_2 == 1 && (!Free || (pEm->Col.Hit_flg & 0x100)) && !(pEm->Type & 0x4000))
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if (pEm->Routine_2 == 6 && pEm->UnkE2 == 2 && (LONG)pEm->Unk154 < 1600 && pPl->Routine_0 == 1
        && !Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 800) && !pPl->Damage_cnt) {
        pPl->Damage_cnt |= 0x80;
        EM_SET_RTN(pEm, 1, 0xC, 0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/enemy/em23", func_80106AA8);

void func_801071F0(void) {
}

void func_801071F8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    if (!pEm->Routine_2) {
        EF(pEm)->Unk01 = 0;
        EM_SET_MOVE(pEm, 0x16, 0, 7);
        func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
        pEm->Col.H = 4000;
        pEm->Routine_2++;
        EM_RTN(pPl) = (Cdir_ck(pEm, pPl) << 24) | 0x105;
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, pKan, pSeq, 0);
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pEm->Be_flg |= 0x800;
        Pl_work.Be_flg |= 0x800;
        EM_SET_RTN(pEm, 1, 9, 1, 0);
        Snd_se_enem(5, pEm);
    }
}

/* pick the jump target next to the player */
void func_801072F8(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    ENEMY_WORK *pPl = &Pl_work;
    LONG Near;
    LONG Dir;

    Near = pEm->Unk154;
    Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
    Near = Near < 4001;
    Sv.vx = 2000;
    Sv.vz = Sv.vy;
    Rot_vector_l(Dir, &Sv, &Sv);
    if (!Near) {
        if (!(Rnd_l() & 3)) {
            EF(pEm)->Unk22 = pPl->Pos_x - Sv.vx;
            EF(pEm)->Unk24 = pPl->Pos_z - Sv.vz;
        } else {
            EF(pEm)->Unk22 = pPl->Pos_x + Sv.vx;
            EF(pEm)->Unk24 = pPl->Pos_z + Sv.vz;
        }
    } else if (EF(pEm)->Flg & 1) {
        EF(pEm)->Unk22 = pPl->Pos_x;
        EF(pEm)->Unk24 = pPl->Pos_z;
    } else {
        EF(pEm)->Unk22 = pPl->Pos_x + Sv.vx;
        EF(pEm)->Unk24 = pPl->Pos_z + Sv.vz;
    }
    EF(pEm)->Unk26 = (EF(pEm)->Unk22 - pEm->Pos_x) / 12;
    EF(pEm)->Unk28 = (EF(pEm)->Unk24 - pEm->Pos_z) / 12;
}

void func_80107454(ENEMY_WORK *pEm)
{
    LONG Old;

    Old = EF(pEm)->Ground;
    EF(pEm)->Ground = func_8004D720(&pEm->Col.Pos_x, 0, -1, 8);
    if (EF(pEm)->Ground < pEm->Col.Pos_y)
        EF(pEm)->Ground = Old;
    EM23_WALL_DIR(pEm);
    if (pEm->Pos_y < EF(pEm)->Ground - 1100 && ++pEm->Timer0 >= 11 && pEm->Routine_2) {
        EF(pEm)->Unk1A = 0x800;
        EF(pEm)->Mode = 2;
        EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    }
    if (func_80102DB8(pEm, 1600)) {
        EF(pEm)->Unk1A = 0x800;
        EF(pEm)->Mode = 2;
        EM_SET_RTN(pEm, 1, 0xE, 0, 0);
        pEm->Timer0 = 0;
    }
}

/* mv11: attack on the wall */
void func_801075A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    HIT_AREA Area;
    ENEMY_WORK *pPl = &Pl_work;
    LONG Move;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xB, 0, 7);
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 0;
    case 1:
        if ((*pEm->pNow_seq >> 9) & 1) {
            Sv.vx = 1500;
            Sv.vy = 0;
            Sv.vz = 1200;
            Sv.pad = 1200;
            Area.pPos = (VECTOR *)pEm->pSin_parts_ptr->Workm.t;
            Area.pBox = (SHORT *)&Sv;
            Area.Dir = pEm->Cdir_y;
            Area.Id = (Rnd_l() & 1) - 0x7F;
            func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        }
        Sv.vx = 400;
        Sv.vz = 0;
        Sv.vy = 0;
        Sv.pad = 600;
        if (((*pEm->pNow_seq >> 11) & 1) && func_80044420(&pEm->pSin_parts_ptr[10].Workm, &Sv, pPl)
            && !pPl->Damage_cnt && !pEm->UnkE2) {
            pEm->UnkE2++;
            Pad_vib_set0(7, 0);
            Pad_vib_set1(3, 0xFA, 4);
            pPl->Routine_0 = 2;
            pPl->Routine_1 = Cdir_ck(pEm, pPl);
            pPl->Routine_2 = 0;
            if (Pl_life_down(10, 0) & 2)
                EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
        }
        if (((*pEm->pNow_seq >> 10) & 1) && func_80044420(&pEm->pSin_parts_ptr[13].Workm, &Sv, pPl)
            && !pPl->Damage_cnt && !pEm->UnkE3) {
            pEm->UnkE3++;
            Pad_vib_set0(7, 0);
            Pad_vib_set1(3, 0xFA, 4);
            pPl->Routine_0 = 2;
            pPl->Routine_1 = Cdir_ck(pEm, pPl);
            pPl->Routine_2 = 0;
            if (Pl_life_down(15, 0) & 2)
                EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
        }
        Sv.vx = 400;
        Sv.vz = 0;
        Sv.vy = 0;
        if (pEm->UnkE2 && ++pEm->UnkE2 < 10)
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[10].Workm, &Sv, 0);
        if (pEm->UnkE3 && ++pEm->UnkE3 < 10)
            Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[13].Workm, &Sv, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EF(pEm)->Unk11 = (Rnd_l() + 30) & 0x3F;
            Move = 2;
            if (EF(pEm)->Flg & 1)
                Move = 1;
            EM_MOVE(pEm) = Move;
            EM_SET_RTN(pEm, 1, 5, 0, 7);
        }
        func_8010387C(pEm, 0);
        break;
    }
    if (EF(pEm)->Floor_y > EF(pEm)->Unk2A) {
        if (pEm->Pos_y - 1600 < EF(pEm)->Floor_y) {
            pEm->Timer0 = 0;
            EM_SET_RTN(pEm, 1, 3, 0xB, 0);
        }
    } else if (pEm->Pos_y - 1600 < EF(pEm)->Unk2A && EF(pEm)->Mode == 1) {
        EF(pEm)->Unk1A = 0x800;
        EF(pEm)->Mode = 2;
        EM_SET_RTN(pEm, 1, 0xE, 0, 0);
    }
    func_80109428(pEm);
}

/* back on the floor once the wall direction is near 0x800 */
void func_8010799C(ENEMY_WORK *pEm)
{
    LONG Dir;

    Dir = EF(pEm)->Unk04 & 0xFFF;
    EF(pEm)->Unk04 = Dir;
    if (__builtin_abs(Dir - 0x800) < 0x40) {
        EF(pEm)->Unk04 = 0x800;
        EF(pEm)->Unk1A = 0x800;
        EF(pEm)->Mode = 2;
        EM_SET_RTN(pEm, 1, 3, 3, 0);
        pEm->Be_flg &= ~0x2000;
    }
}

void func_801079FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (EF(pEm)->Flg & 8)
        func_80107A50(pEm, pKan, pSeq);
    else
        func_80107B84(pEm, pKan, pSeq);
    func_80109428(pEm);
}

void func_80107A50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
    }
    if (pEm->Spd_x > 10 && EF(pEm)->Unk04 != 0x800) {
        if (EF(pEm)->Unk04 < 0x800)
            EF(pEm)->Unk04 += pEm->Spd_x - 10;
        else
            EF(pEm)->Unk04 -= pEm->Spd_x - 10;
    }
    EM23_WALL_DIR(pEm);
}

void func_80107B84(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move(pEm, pKan, pSeq, 0x100);
    }
    if (pEm->Spd_x > 60 && EF(pEm)->Unk04 != 0x800) {
        if (EF(pEm)->Unk04 < 0x800)
            EF(pEm)->Unk04 += pEm->Spd_x - 60;
        else
            EF(pEm)->Unk04 -= pEm->Spd_x - 60;
    }
    EM23_WALL_DIR(pEm);
}

void func_80107CB8(void) {
}

void func_80107CC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 1);
        if (Joint_move(pEm, pKan, pSeq, 0x10100))
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        Add_speedXZ(pEm, 0);
    }
    Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
}

void func_80107D94(void) {
}

/* mv10: drop from the ceiling onto the player, both claws */
void func_80107D9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    VECTOR Pos;
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xB, 0, 7);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        EF(pEm)->Unk16 = 2;
    case 1:
        pEm->Pos_y += EF(pEm)->Unk16;
        if (pEm->Move_cnt & 1) {
            EF(pEm)->Unk16 += EF(pEm)->Unk16;
            if (EF(pEm)->Unk16 > 800)
                EF(pEm)->Unk16 = 800;
        }
        if (EF(pEm)->Unk16 > 128 && pEm->Timer0) {
            pEm->Cdir_z += 0x100;
            pEm->Timer0 -= 0x100;
            pEm->Col.Unk26 += 250;
        }
        if (!pEm->UnkDA)
            pEm->UnkDA = Joint_move(pEm, pKan, pSeq, 0x100);
        Sv.vx = 400;
        Sv.vz = 0;
        Sv.vy = 0;
        Sv.pad = 600;
        if (((*pEm->pNow_seq >> 11) & 1) && func_80044420(&pEm->pSin_parts_ptr[10].Workm, &Sv, pPl)
            && !pPl->Damage_cnt && !pEm->UnkE2) {
            pEm->UnkE2++;
            Pad_vib_set0(7, 0);
            Pad_vib_set1(3, 0xFA, 4);
            if (!(pPl->Damage_cnt & 0x80)) {
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                if (Pl_life_down(10, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
            }
            Snd_se_enem(4, pEm);
        }
        if (((*pEm->pNow_seq >> 10) & 1) && func_80044420(&pEm->pSin_parts_ptr[13].Workm, &Sv, pPl)
            && !pPl->Damage_cnt && !pEm->UnkE3) {
            pEm->UnkE3++;
            Pad_vib_set0(7, 0);
            Pad_vib_set1(3, 0xFA, 4);
            if (!(pPl->Damage_cnt & 0x80)) {
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                if (Pl_life_down(15, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
            }
            Snd_se_enem(4, pEm);
        }
        if (pEm->Pos_y > pEm->Unk122) {
            EF(pEm)->Unk16 = 0;
            EF(pEm)->Mode = 0;
            pEm->Pos_y = pEm->Unk122;
            if (pEm->Water && G.Unk783E == 4 && G.Unk7840 == 7) {
                Pos.vx = pEm->Pos_x;
                Pos.vy = pEm->Pos_y;
                Pos.vz = pEm->Pos_z;
                Snd_se_stad(0x10209, &Pos, 0, 0);
            } else {
                Snd_se_enem(1, pEm);
            }
            EM_SET_RTN(pEm, 1, 6, 0, 0);
        }
        if (pEm->Col.W0 < 800) {
            pEm->Col.W0 += 100;
            pEm->Col.D0 = pEm->Col.W0;
        }
    }
}

void func_80108148(ENEMY_WORK *pEm)
{
    if (EF(pEm)->Flg & 8)
        func_80109174_();
    if (pEm->Col.W0 < 800) {
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Col.Unk26 = -2000;
        pEm->Col.W0 += 100;
        pEm->Col.D0 = pEm->Col.W0;
    }
    if ((LONG)pEm->Unk154 < 3500)
        EM_SET_RTN(pEm, 1, 0x10, 0, 0);
}

/* walk backwards from the target */
void func_801081CC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            if (EF(pEm)->Flg & 8) {
                if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 600))
                    EM_SET_RTN(pEm, 1, 4, 2, 0);
            } else {
                if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 600))
                    EM_SET_RTN(pEm, 1, 4, 2, 0);
            }
        }
    }
    if (pEm->Spd_x > 60) {
        if (EF(pEm)->Flg & 8)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -pEm->Spd_x + 60);
        else
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, -pEm->Spd_x + 60);
        pEm->Spd_x = -pEm->Spd_x;
        Add_speedXZ(pEm, 0);
    }
}

void func_80108320(void) {
}

/* mv1A: jump from the floor onto the wall at (Unk22, Unk24) */
void func_80108328(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR Pos;
    VECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 7, 0, 7);
        EF(pEm)->Unk16 = -800;
        pEm->Routine_2++;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 16);
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2++;
        else
            pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        Snd_se_enem(0, pEm);
        EM_SET_MOVE(pEm, 8, 0, 7);
        pEm->Routine_2++;
    case 3:
        Goto00(pEm, EF(pEm)->Unk22, EF(pEm)->Unk24, 16);
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EF(pEm)->Unk26 = (EF(pEm)->Unk12 - pEm->Pos_x) / 9;
            EF(pEm)->Unk28 = (EF(pEm)->Unk14 - pEm->Pos_z) / 9;
            if (EF(pEm)->Side != 2) {
                EF(pEm)->Unk08 = pEm->Cdir_y & 0xFFF;
                if (EF(pEm)->Unk08 > 0x800)
                    EF(pEm)->Unk08 = 0x1000 - EF(pEm)->Unk08;
                else
                    EF(pEm)->Unk08 = -EF(pEm)->Unk08;
            } else {
                EF(pEm)->Unk08 = 0x800 - (pEm->Cdir_y & 0xFFF);
            }
            pEm->Routine_2++;
            EF(pEm)->Unk08 /= 9;
        }
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        EM_SET_MOVE(pEm, 9, 0, 0xF);
        pEm->UnkE2 = 0;
        pEm->UnkDA = 0;
        pEm->Timer0 = 0;
        EF(pEm)->Unk01 = 1;
        pEm->Routine_2++;
    case 5:
        Goto00(pEm, EF(pEm)->Unk22, EF(pEm)->Unk24, 16);
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Timer0 = pEm->UnkDA >= 10;
        if (pEm->Move_cnt >= 10)
            pEm->Move_cnt = 9;
        pEm->UnkDA++;
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Timer0 = pEm->UnkDA >= 10;
        if (pEm->Move_cnt >= 10)
            pEm->Move_cnt = 9;
        pEm->UnkDA++;
        pEm->Pos_y += EF(pEm)->Unk16;
        if (pEm->Timer0) {
            EF(pEm)->Unk16 = 0;
            pEm->Routine_2++;
        }
        pEm->Pos_x += EF(pEm)->Unk26;
        pEm->Pos_z += EF(pEm)->Unk28;
        pEm->Cdir_y += EF(pEm)->Unk08;
        if (pEm->Col.W0 > 100) {
            pEm->Col.W0 -= 100;
            pEm->Col.D0 = pEm->Col.W0;
        }
        EF(pEm)->Unk16 += 100;
        break;
    case 6:
        EM_SET_MOVE(pEm, 0xA, 0, 7);
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Water && G.Unk783E == 4 && G.Unk7840 == 7) {
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x10209, &Pos, 0, 0);
        } else {
            Snd_se_enem(1, pEm);
        }
        EF(pEm)->Unk01 = 0;
        EF(pEm)->Mode = 1;
        pEm->Type = 0;
        pEm->Timer0 = 0;
        pEm->Routine_2++;
    case 7:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            func_80103518(pEm);
            func_801035A8(pEm);
            pEm->Pos_x = pEm->Old_x = EF(pEm)->Unk12;
            EM_SET_MOVE(pEm, 2, 0, 7);
            EM_SET_RTN(pEm, 1, 3, 3, 0);
            pEm->Pos_z = pEm->Old_z = EF(pEm)->Unk14;
            func_80109428(pEm);
            break;
        }
        if (pEm->Move_cnt < 8) {
            if (EF(pEm)->Side & 1) {
                pEm->Cdir_z = 0;
                if (EF(pEm)->Side & 2) {
                    pEm->Cdir_x = 0xC00;
                    pEm->Cdir_y += 0x80;
                } else {
                    pEm->Cdir_y = 0xC00;
                    pEm->Cdir_x += 0x80;
                }
            } else {
                pEm->Cdir_x = 0;
                pEm->Cdir_z -= 0x80;
                if (EF(pEm)->Side & 2)
                    pEm->Cdir_y = 0;
                else
                    pEm->Cdir_y = 0x800;
            }
        } else {
            pEm->Col.D0 = 400;
            pEm->Col.W0 = 400;
            pEm->Col.Unk26 = -900;
            func_80109428(pEm);
        }
        pEm->Pos_x += (EF(pEm)->Unk12 - pEm->Pos_x) / (21 - pEm->Move_cnt);
        pEm->Pos_z += (EF(pEm)->Unk14 - pEm->Pos_z) / (21 - pEm->Move_cnt);
        break;
    }
}

void func_801088FC(void) {
}

/* mv1C/1D: slash with the claw on the player's side */
void func_80108904(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    HIT_AREA Unused;
    ENEMY_WORK *pPl = &Pl_work;
    LONG Right;

    switch (pEm->Routine_2) {
    case 0:
        Right = (Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z) - pEm->Cdir_y) & 0x800;
        Right = Right == 0;
        EM_MOVE(pEm) = Right + 0x7001C;
        if (Right)
            pEm->Timer0 = 10;
        else
            pEm->Timer0 = 13;
        pEm->UnkDA = 0;
        pEm->Routine_2++;
    case 1:
        Sv.vx = 400;
        Sv.vz = 0;
        Sv.vy = 0;
        Sv.pad = 600;
        if (((*pEm->pNow_seq >> 10) & 1) && func_80044420(&pEm->pSin_parts_ptr[pEm->Timer0].Workm, &Sv, pPl)
            && !pPl->Damage_cnt && !pEm->UnkDA) {
            pEm->UnkDA++;
            Pad_vib_set0(7, 0);
            Pad_vib_set1(3, 0xFA, 4);
            if (!(pPl->Damage_cnt & 0x80)) {
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                if (Pl_life_down(10, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
            }
            Snd_se_enem(4, pEm);
        }
        if (pEm->UnkDA) {
            if (pEm->UnkDA < 4) {
                pPl->Spd_x = (pEm->UnkDA - 1) * 100 - 400;
                Add_speedXZ(pPl, 0);
            }
            if (++pEm->UnkDA < 10)
                Esp_call(0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pEm->pSin_parts_ptr[pEm->Timer0].Workm, &Sv, 0);
        }
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            EM_SET_RTN(pEm, 1, 0xF, 0, 0);
        Add_speedXZ(pEm, 0);
    }
}

void func_80108BBC(void) {
}

/* mv1F: bite */
void func_80108BC4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;
    HIT_AREA Area;
    SVECTOR Sv;
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x20, 0, 7);
        pEm->Routine_2 = 1;
        func_80108E04(pEm, pEm->Cdir_y);
        Snd_se_enem(7, pEm);
    case 1:
        Sv.vx = 3500;
        Sv.vz = 1800;
        Sv.vy = 0;
        Sv.pad = 1200;
        Area.pPos = (VECTOR *)pEm->pSin_parts_ptr->Workm.t;
        Area.pBox = (SHORT *)&Sv;
        Area.Dir = pEm->Cdir_y;
        Area.Id = (Rnd_l() & 1) - 0x7F;
        func_800445C8(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 2, 0, 0);
        EF(pEm)->Unk2E = 0;
        break;
    }
}

void func_80108D18(void) {
}

void func_80108D20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0x1F, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 12)
            func_8010C694(pEm);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 2, 0, 0);
        break;
    }
}

/* effect 0x1A in front of the body */
void func_80108E04(ENEMY_WORK *pEm, SHORT Dir)
{
    SVECTOR Pos;
    SVECTOR Ofs;
    MATRIX M;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    Pos.vx = pEm->Cdir_x;
    Pos.vy = pEm->Cdir_y;
    Pos.vz = pEm->Cdir_z;
    RotMatrix(&Pos, &M);
    Ofs.vx = 1000;
    Ofs.vy = 0;
    Ofs.vz = -100;
    ApplyMatrixSV(&M, &Ofs, &Pos);
    Pos.vx += pP->Workm.t[0];
    Pos.vy += pP->Workm.t[1];
    Pos.vz += pP->Workm.t[2];
    Esp_call(0x1A, (USHORT)Dir | 0x10000000, &GsIDMATRIX, &Pos, 0);
}

/* no other enemy is in routine Rtn0/Rtn1 and the player is on the same side */
LONG func_80108EDC(ENEMY_WORK *pEm, USHORT Rtn0, USHORT Rtn1)
{
    ENEMY_WORK *pPl = G_PLAYER;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pE;
    LONG Ck;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pE = *ppEm;
        if (pE != pEm && (pE->Be_flg & 1)) {
            if ((USHORT)EM_RTN01(pE) == Rtn0 || (USHORT)EM_RTN01(pE) == Rtn1)
                return 0;
        }
    }
    Ck = (pEm->Col.Hit_flg ^ pPl->Col.Hit_flg) & 0x40;
    return Ck == 0;
}

/* root search: the nearest em23 leads (Flg 1), the next one follows (2), the others wait (3) */
void func_80108F68(ENEMY_WORK *pEm)
{
    ENEMY_WORK **ppE;
    ENEMY_WORK *pE;
    ENEMY_WORK *pNear;
    LONG Min;
    ULONG Dist;

    pNear = pEm;
    Min = 0x7FFFFFFF;
    EF(pEm)->Flg = 0;
    Dist = 0;
    for (ppE = &G.pEnemy[2]; ppE != G.ppEnemy_end; ppE++) {
        pE = *ppE;
        if ((pE->Be_flg & 1) && (pE->Id == 0x23 || pE->Id == 0x28) && pE->Routine_0 != 7) {
            if ((LONG)pE->Unk154 < Min) {
                Min = pE->Unk154;
                pNear = pE;
            }
            EF(pE)->Timer = 0;
            EF(pE)->Flg = (EF(pE)->Flg & 8) | 1;
        }
    }
    if (pE->Routine_0 != 7 && pE->Routine_0 != 3 && !(EF(pNear)->Flg & 8))
        Snd_se_enem(13, pNear);
    for (ppE = &G.pEnemy[2]; ppE != G.ppEnemy_end; ppE++) {
        pE = *ppE;
        if ((pE->Be_flg & 1) && pE != pNear && (pE->Id == 0x23 || pE->Id == 0x28) && pE->Routine_0 != 7) {
            if (!Dist) {
                pEm = pE;
                Dist = pEm->Unk154;
                EF(pE)->Flg = (EF(pE)->Flg & 8) | 2;
            } else if ((LONG)pE->Unk154 < (LONG)Dist) {
                EF(pE)->Flg = (EF(pE)->Flg & 8) | 2;
                Dist = pE->Unk154;
                EF(pEm)->Flg = (EF(pEm)->Flg & 8) | 3;
            }
        }
    }
}

/* stop the root search when the player is close or firing */
void func_80109174(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = G_PLAYER;

    if ((((LONG)pEm->Unk154 < 3000 && ((G.Unk75E8 & 6) || Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100)))
            || ((LONG)pEm->Unk154 < 5000 && (G.Unk75E8 & 4)))
        && (EF(pEm)->Flg & 8)) {
        if (EF(pEm)->Flg & 7)
            EF(pEm)->Flg &= 7;
        else
            func_80108F68(pEm);
    }
}

/* water splash under the body */
void func_80109234(ENEMY_WORK *pEm)
{
    SVECTOR Pos;
    SVECTOR Ofs;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    CompM((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &pP->Workm);
    Pos.vx = pP->Workm.t[0];
    Pos.vy = pEm->Water;
    Pos.vz = pP->Workm.t[2];
    Ofs.vx = 500;
    Ofs.vy = 0;
    Ofs.vz = 0;
    ApplyMatrixSV((MATRIX *)pEm->Workm_m, &Ofs, &Ofs);
    Ofs.vx += Pos.vx;
    Ofs.vy += Pos.vy;
    Ofs.vz += Pos.vz;
    Esp_call(0x61C, ((Rnd_l() * 4 + 0x1000) << 16) | (USHORT)(pEm->Cdir_y - Rnd_l() * 4), &GsIDMATRIX, &Ofs, 0);
    Esp_call(0x61C, ((Rnd_l() * 4 + 0x1000) << 16) | (USHORT)(pEm->Cdir_y + 0x800 - Rnd_l() * 4), &GsIDMATRIX, &Pos, 0);
    Esp_call(0x161C, ((Rnd_l() * 4 + 0xC00) << 16) | (USHORT)(Rnd_l() * 16), &GsIDMATRIX, &Pos, 0);
    Esp_call(0x161C, ((Rnd_l() * 4 + 0x1000) << 16) | (USHORT)(Rnd_l() * 16), &GsIDMATRIX, &Pos, 0);
    Ofs.vx = 0x400;
    Ofs.vy = 0;
    Ofs.vz = 0;
    Esp_call(0x20E, (USHORT)pEm->Cdir_y | 0x28000000, &GsIDMATRIX, &Pos, &Ofs);
}

/* collision offset 800 behind along the wall direction */
void func_80109428(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    USHORT Dir;

    Sv.vx = -800;
    Sv.vz = 0;
    Dir = EF(pEm)->Unk04;
    if (EF(pEm)->Side & 1)
        Dir = -Dir;
    Rot_vector(Dir, &Sv, &Sv);
    pEm->Col.Unk24 = Sv.vx;
    pEm->Col.Unk28 = Sv.vz;
}

/* RE2 Em23_Damage */
void func_80109498(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;

    if (EF(pEm)->Flg & 8) {
        if (EF(pEm)->Flg & 7)
            EF(pEm)->Flg &= 7;
        else
            func_80108F68(pEm);
    }
    if (!pEm->Routine_2)
        func_8010962C(pEm);
    func_8010A728(pEm);
    if (!pEm->Unk1A4[1] && !pEm->Unk1AC[1] && pEm->Unk12C != 0x18 && (pEm->Unk1C4 & 0xF) == 1 && EF(pEm)->Unk2E) {
        EM_RTN(pEm) = pEm->Unk1C4;
        func_801009E8(pEm, pKan, pSeq);
        return;
    }
    pEm->Be_flg &= ~0x2000;
    Em23_dm[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
    if (pEm->Col.Unk26)
        pEm->Col.Unk26 = 0;
    if (pEm->Col.W0 < 800) {
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Col.W0 += 20;
        pEm->Col.D0 = pEm->Col.W0;
    }
    Em_seq_sound(pEm);
    pEm->Unk1C4 = EM_RTN(pEm);
}

/* RE2 Em23_Damage_ck? (status effects of the hit) */
void func_8010962C(ENEMY_WORK *pEm)
{
    if (!(pEm->Status_flg & 2) && (pEm->Unk132 & 0x200))
        func_80109F7C(pEm);
    if (!(pEm->Status_flg & 1) && (pEm->Unk132 & 0x100))
        func_8010A100(pEm);
    if (!(pEm->Status_flg & 4) && (pEm->Unk132 & 0x800))
        func_8010A27C(pEm);
    if (pEm->Unk132 & 0x400)
        func_8010A3AC(pEm);
    pEm->Unk16A = 4;
    if (pEm->Unk12C == 11 || pEm->Unk12C == 14 || pEm->Unk12C == 15)
        pEm->Unk16A = 1;
    Snd_se_enem(8, pEm);
}

/* dm00: knocked down (falls off the wall / ceiling first) */
void func_80109734(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Free;

    Free = func_80108EDC(pEm, 0x901, 0x801);
    Free |= func_80108EDC(pEm, 0xB01, 0xC01);
    switch (pEm->Routine_2) {
    case 0:
        if (!(EF_W0(pEm) & 0xFFFF00)) {
            pEm->Routine_2 = 2;
            if (EF(pEm)->Unk03 == 1) {
                pEm->Col.H = 1500;
                EF(pEm)->Unk03 = 0;
                EM_SET_MOVE(pEm, 0x17, 0, 0);
                pEm->Unk1A4[2] = 1;
            } else {
                EM_SET_MOVE(pEm, 0xD, 0, 0);
                pEm->Unk1A4[2] = 0;
            }
            goto next;
        }
        if (!pEm->Unk1A4[1]) {
            EF(pEm)->Unk16 = 2;
            pEm->Col.Unk28 = 0;
            pEm->Col.Unk26 = 0;
            pEm->Col.Unk24 = 0;
            pEm->Be_flg &= ~0x400;
            pEm->Pos_x = pEm->pSin_parts_ptr->Workm.t[0];
            pEm->Pos_z = pEm->pSin_parts_ptr->Workm.t[2];
            EM_SET_MOVE(pEm, 0x18, 0, 7);
            pEm->Cdir_z = 0;
            pEm->Cdir_x = 0;
            pEm->Cdir_y = (EF(pEm)->Mode == 4) ? pEm->Cdir_y - 0x800 : EF(pEm)->Side * 0x400;
            pEm->Unk1A4[1] = 1;
            pEm->Unk1A4[0] = 0;
            pEm->Unk1A4[2] = 0;
        }
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Col.W0 < 800) {
            pEm->Col.W0++;
            pEm->Col.D0 = pEm->Col.W0;
        }
        if (!pEm->Unk1A4[0])
            pEm->Unk1A4[0] = Joint_move(pEm, pKan, pSeq, 0x200);
        if (!pEm->Unk1A4[2]) {
            pEm->Pos_y += EF(pEm)->Unk16;
            EF(pEm)->Unk16 += EF(pEm)->Unk16;
            if (pEm->Pos_y > pEm->Unk122) {
                EF(pEm)->Unk16 = 0;
                pEm->Col.D0 = 800;
                pEm->Col.W0 = 800;
                pEm->Pos_y = pEm->Unk122;
                Snd_se_enem(1, pEm);
                pEm->Unk1A4[2] = 1;
            }
        }
        if (pEm->Unk1A4[0] && pEm->Unk1A4[2]) {
            EM_SET_MOVE(pEm, 0x15, 0, 0);
            EF(pEm)->Unk01 = 0;
            EF(pEm)->Mode = 0;
            pEm->Unk1A4[1] = 0;
            pEm->Unk1A4[2] = 0;
            pEm->Damage_cnt |= 0x80;
            pEm->Routine_2++;
        }
        break;
    case 2:
    next:
        pEm->Routine_2++;
    case 3:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0)) {
            if (!pEm->Unk1A4[2]) {
                pEm->Routine_2++;
            } else {
                EM_SET_MOVE(pEm, 0x15, 0, 0);
                pEm->Unk1A4[2] = 0;
                pEm->Damage_cnt |= 0x80;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        EF(pEm)->Mode = 0;
        EF(pEm)->Unk01 = 0;
        pEm->UnkDA = (Rnd_l() & 0x1F) + 30;
        pEm->Damage_cnt &= 0x7F;
        if (!EF(pEm)->Unk2E) {
            EF(pEm)->Unk2E = (Rnd_l() & 0x1F) + 15;
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (!(Rnd_l() & 3) && Free) {
            pEm->UnkE2 = 0;
            EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        break;
    }
}

/* dm01: blown away (falls off the wall / ceiling first) */
void func_80109AB4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    LONG Free;
    LONG Spd;
    LONG SpdZ;
    LONG N;

    Free = func_80108EDC(pEm, 0x901, 0x801);
    Free |= func_80108EDC(pEm, 0xB01, 0xC01);
    switch (pEm->Routine_2) {
    case 0:
        if (!(EF_W0(pEm) & 0xFFFF00)) {
            pEm->Routine_2 = 2;
            if (EF(pEm)->Unk03 == 1) {
                EM_SET_MOVE(pEm, 0x17, 0, 7);
                pEm->Col.H = 1500;
            } else {
                if (pEm->Unk132 & 0x10) {
                    EM_SET_MOVE(pEm, 0x1B, 0, 0);
                    Sv.vx = 200;
                } else {
                    EM_SET_MOVE(pEm, 0x1A, 0, 0);
                    Sv.vx = -200;
                }
                Sv.vz = 0;
                EF(pEm)->Unk26 = Sv.vx;
                EF(pEm)->Unk28 = Sv.vz;
            }
            goto next;
        }
        if (!pEm->Unk1AC[1]) {
            EF(pEm)->Unk16 = 2;
            pEm->Col.Unk28 = 0;
            pEm->Col.Unk26 = 0;
            pEm->Col.Unk24 = 0;
            pEm->Be_flg &= ~0x400;
            pEm->Pos_x = pEm->pSin_parts_ptr->Workm.t[0];
            pEm->Pos_z = pEm->pSin_parts_ptr->Workm.t[2];
            pEm->Cdir_z = 0;
            pEm->Cdir_x = 0;
            if (EF(pEm)->Mode == 4)
                pEm->Cdir_y -= 0x800;
            else
                pEm->Cdir_y = EF(pEm)->Side * 0x400;
            EM_SET_MOVE(pEm, 0x18, 0, 7);
            pEm->Unk1AC[0] = 0;
            pEm->Unk1AC[2] = 0;
            pEm->Unk1AC[1] = 1;
        }
        pEm->Routine_2 = 1;
    case 1:
        pEm->Pos_y += EF(pEm)->Unk16;
        EF(pEm)->Unk16 += EF(pEm)->Unk16;
        if (pEm->Col.W0 < 800) {
            pEm->Col.W0++;
            pEm->Col.D0 = pEm->Col.W0;
        }
        if (!pEm->Unk1AC[0])
            pEm->Unk1AC[0] = Joint_move(pEm, pKan, pSeq, 0x200);
        if (!pEm->Unk1AC[2] && pEm->Pos_y >= pEm->Unk122) {
            EF(pEm)->Unk16 = 0;
            pEm->Cdir_z = 0;
            pEm->Cdir_x = 0;
            pEm->Pos_y = pEm->Unk122;
            pEm->Cdir_y = (EF(pEm)->Mode == 4) ? pEm->Cdir_y - 0x800 : EF(pEm)->Side * 0x400;
            pEm->Col.D0 = 800;
            pEm->Col.W0 = 800;
            pEm->Unk1AC[2] = 1;
        }
        if (pEm->Unk1AC[0] && pEm->Unk1AC[2]) {
            EM_SET_MOVE(pEm, 0x15, 0, 7);
            EF(pEm)->Mode = 0;
            EF(pEm)->Unk01 = 0;
            pEm->Unk1AC[1] = 0;
            pEm->Routine_2 = 4;
            pEm->Damage_cnt |= 0x80;
        }
        break;
    case 2:
    next:
        pEm->Routine_2++;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_SET_MOVE(pEm, 0x15, 0, 7);
            EF(pEm)->Mode = 0;
            EF(pEm)->Unk01 = 0;
            pEm->Damage_cnt |= 0x80;
            pEm->Routine_2++;
        }
        pEm->Spd_x = EF(pEm)->Unk26;
        pEm->Spd_z = EF(pEm)->Unk28;
        Spd = EF(pEm)->Unk26;
        if (Spd) {
            if (__builtin_abs(Spd) > 10) {
                if (Spd <= 0)
                    N = Spd + 10;
                else
                    N = Spd - 10;
                EF(pEm)->Unk26 = N;
            } else {
                EF(pEm)->Unk26 = 0;
            }
        }
        SpdZ = EF(pEm)->Unk28;
        if (SpdZ) {
            if (__builtin_abs(SpdZ) > 10) {
                if (SpdZ <= 0)
                    N = SpdZ + 10;
                else
                    N = SpdZ - 10;
                EF(pEm)->Unk28 = N;
            } else {
                EF(pEm)->Unk28 = 0;
            }
        }
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    case 5:
        pEm->Damage_cnt &= 0x7F;
        pEm->UnkDA = (Rnd_l() & 0x1F) + 30;
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (!(Rnd_l() & 3) && Free) {
            pEm->UnkE2 = 0;
            EM_SET_RTN(pEm, 1, 0xB, 0, 0);
        }
        break;
    }
}

void func_80109F0C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Routine_2 == 0 && !(pEm->Unk132 & 0xC))
        func_8010A5F4(pEm);
    func_80109AB4(pEm, pKan, pSeq);
}

/* hit flash on the parts of the hit side */
void func_80109F7C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;

    pEm->Unk178[2] = 0x1000;
    pEm->Unk178[0] = 0;
    pEm->Unk178[1] = 0;
    pEm->Unk17E = 0x40;
    pEm->Be_flg |= 8;
    if (EF(pEm)->Unk03 == 1) {
        switch (pEm->Unk132 & 3) {
        case 1:
            pP = &pEm->pSin_parts_ptr[15];
            pP->UnkA2 |= 0x40;
            pP = &pEm->pSin_parts_ptr[18];
            pP->UnkA2 |= 0x40;
            break;
        case 2:
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 0x40;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 0x40;
            break;
        case 3:
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 0x40;
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 |= 0x40;
            break;
        }
    } else {
        pP = &pEm->pSin_parts_ptr[8];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[11];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[14];
        pP->UnkA2 |= 0x40;
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1C03, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
}

/* three blood splashes, then clear the hit flash */
void func_8010A100(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    LONG i;

    for (i = 0; i < 3; i++) {
        if (EF(pEm)->Unk03 == 1) {
            switch (pEm->Unk132 & 3) {
            case 1:
                pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 15];
                break;
            case 2:
                pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
                break;
            case 3:
                pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
                break;
            }
        } else {
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
        }
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(0x205, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
    }
    for (i = 0; i < 21; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
    }
}

/* blood from a random part of the hit side */
void func_8010A27C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;

    if (EF(pEm)->Unk03 == 1) {
        switch (pEm->Unk132 & 3) {
        case 1:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 15];
            break;
        case 2:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
            break;
        case 3:
            pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
            break;
        }
    } else {
        pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1503, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
}

/* tear off a whole leg (parts 2-4 / 5-7) once */
void func_8010A3AC(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG i;

    EF(pEm)->Unk2C = 0;
    if (!(pEm->Unk132 & 0xC)) {
        if (Rnd_l() & 1) {
            if (!(EF(pEm)->Unk2C & 1)) {
                i = 2;
                pP = &pEm->pSin_parts_ptr[2];
                pP->UnkA2 |= 0x20;
                pP->Be_flg = (pP->Be_flg | 2) & ~0x20;
                PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
                PW_FREE(pP)[4] = 0;
                PW_FREE(pP)[1] = -240;
                PW_FREE(pP)[2] = 0;
                PW_FREE(pP)[3] = 15;
                PW_FREE(pP)[6] = 0;
                for (; i < 5; i++) {
                    pP = &pEm->pSin_parts_ptr[i];
                    pP->Poly_rgb = 0x2F2F2F;
                    pP->Be_flg |= 0xA;
                    pP->Old_x = pEm->Cdir_y + 0x800;
                    pP->Unk96 = 0;
                    pP->Old_z = 100;
                    pP->Old_y = 0;
                    pP->Unk85 = 30;
                    pP->Unk90 = 0;
                    pP->Unk92 = pEm->Cdir_y;
                    pP->Unk8E = 0x400;
                }
                EF(pEm)->Unk2C |= 1;
            }
        } else if (!(EF(pEm)->Unk2C & 2)) {
            i = 5;
            pP = &pEm->pSin_parts_ptr[5];
            pP->UnkA2 |= 0x20;
            pP->Be_flg = (pP->Be_flg | 2) & ~0x20;
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[1] = -240;
            PW_FREE(pP)[2] = 0;
            PW_FREE(pP)[3] = 15;
            PW_FREE(pP)[6] = 0;
            for (; i < 8; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb = 0x2F2F2F;
                pP->Be_flg |= 0xA;
                pP->Old_x = pEm->Cdir_y + 0x800;
                pP->Unk96 = 0;
                pP->Old_z = 100;
                pP->Old_y = 0;
                pP->Unk85 = 30;
                pP->Unk90 = 0;
                pP->Unk92 = pEm->Cdir_y;
                pP->Unk8E = 0x400;
            }
            EF(pEm)->Unk2C |= 2;
        }
    }
    for (i = 0; i < 21; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
    }
}

/* knock off a claw (parts 2 / 5) once */
void func_8010A5F4(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    if (!(pEm->Unk132 & 0xC)) {
        if (Rnd_l() & 1) {
            if (!(EF(pEm)->Unk2C & 1)) {
                pP = &pEm->pSin_parts_ptr[2];
                pEm->pSin_parts_ptr[2].Be_flg |= 0xA;
                pP->Old_x = pEm->Cdir_y + 0x800;
                pP->Unk96 = 0;
                pP->Old_z = 100;
                pP->Old_y = -300;
                pP->Unk85 = 30;
                pP->Unk90 = 0;
                pP->Unk92 = pEm->Cdir_y;
                pP->Unk8E = 0x400;
                EF(pEm)->Unk2C |= 1;
            }
        } else if (!(EF(pEm)->Unk2C & 2)) {
            pP = &pEm->pSin_parts_ptr[5];
            pEm->pSin_parts_ptr[5].Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + 0x800;
            pP->Unk96 = 0;
            pP->Old_z = 100;
            pP->Old_y = -300;
            pP->Unk85 = 30;
            pP->Unk90 = 0;
            pP->Unk92 = pEm->Cdir_y;
            pP->Unk8E = 0x400;
            EF(pEm)->Unk2C |= 2;
        }
    }
}

/* blood while dying (Unk16A splashes) */
void func_8010A728(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    LONG Size;
    LONG Kind;

    Sv.vx = (Rnd_l() & 0x3F) - 0x20;
    Sv.vy = (Rnd_l() & 0x3F) - 0x20;
    Sv.vz = (Rnd_l() & 0x3F) - 0x20;
    Size = ((Rnd_l() & 3) << 9) + 0xE00;
    Kind = 1;
    if (pEm->Unk132 & 0x200)
        Kind = 6;
    if (pEm->Unk16A) {
        switch (pEm->Routine_1) {
        case 0:
            Esp_call((Kind << 24) | 0x700, (Size << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[18].Workm, &Sv, 0);
            break;
        case 1:
        case 3:
            Esp_call((Kind << 24) | 0x700, (Size << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[8].Workm, &Sv, 0);
            break;
        case 2:
            Esp_call((Kind << 24) | 0x700, (Size << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[1].Workm, &Sv, 0);
            break;
        }
        Sv.vx = (Rnd_l() & 0x3F) - 0x20;
        Sv.vy = (Rnd_l() & 0x3F) - 0x20;
        Sv.vz = (Rnd_l() & 0x3F) - 0x20;
        Size = ((Rnd_l() & 3) << 9) + 0xE00;
        Esp_call(Kind << 24, (Size << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[14].Workm, &Sv, 0);
        pEm->Unk16A--;
    }
}

/* RE2 Em23_Die */
void func_8010A8E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em23_die[pEm->Unk12C - 1][(pEm->Unk132 & 0xC) >> 2](pEm, pKan, pSeq);
    func_8010A728(pEm);
    Em_seq_sound(pEm);
}

/* die02: killed (falls off the wall / ceiling first) */
void func_8010A954(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    LONG Spd;
    LONG SpdZ;
    LONG N;

    switch (pEm->Routine_2) {
    case 0:
        Em_dead_flg_on_cand_(pEm);
        if (!(pEm->Status_flg & 2) && (pEm->Unk132 & 0x200))
            func_8010AD38(pEm);
        if (!(pEm->Status_flg & 1) && (pEm->Unk132 & 0x100))
            func_8010AEA0(pEm);
        if (!(pEm->Status_flg & 4) && (pEm->Unk132 & 0x800))
            func_8010B024(pEm);
        if (pEm->Unk132 & 0x400)
            func_8010B154(pEm);
        pEm->Unk16A = 7;
        if (pEm->Unk12C == 11 || pEm->Unk12C == 14 || pEm->Unk12C == 15)
            pEm->Unk16A = 3;
        if (!(EF_W0(pEm) & 0xFFFF00)) {
            pEm->Routine_2 = 2;
            if (pEm->Unk132 & 0x10) {
                EM_SET_MOVE(pEm, 0x1B, 0, 0);
                Sv.vx = 200;
            } else {
                EM_SET_MOVE(pEm, 0x1A, 0, 0);
                Sv.vx = -200;
            }
            Sv.vz = 0;
            EF(pEm)->Unk26 = Sv.vx;
            EF(pEm)->Unk28 = Sv.vz;
            pEm->Routine_3 = 1;
            goto next;
        }
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk26 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Pos_x = pEm->pSin_parts_ptr->Workm.t[0];
        pEm->Pos_z = pEm->pSin_parts_ptr->Workm.t[2];
        pEm->Routine_2 = 1;
        EF(pEm)->Unk16 = 2;
        EM_SET_MOVE(pEm, 0x18, 0, 7);
        pEm->Cdir_z = 0;
        pEm->Cdir_x = 0;
        pEm->Cdir_y = (EF(pEm)->Mode == 4) ? pEm->Cdir_y - 0x800 : EF(pEm)->Side * 0x400;
        pEm->Timer0 = 0;
        EF(pEm)->Unk26 = 0;
        EF(pEm)->Unk28 = 0;
        pEm->Routine_3 = 0;
    case 1:
        if (!pEm->Timer0)
            pEm->Timer0 = Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Pos_y += EF(pEm)->Unk16;
        EF(pEm)->Unk16 += EF(pEm)->Unk16;
        if (pEm->Pos_y > pEm->Unk122) {
            EF(pEm)->Unk16 = 0;
            EF(pEm)->Mode = 0;
            pEm->Col.D0 = 800;
            pEm->Col.W0 = 800;
            pEm->Pos_y = pEm->Unk122;
            Snd_se_enem(1, pEm);
        }
        if (pEm->Timer0 && pEm->Pos_y == pEm->Unk122) {
            Snd_se_enem(9, pEm);
            EM_SET_RTN(pEm, 7, 0, 0, 0);
        }
        break;
    case 2:
    next:
        Snd_se_enem(9, pEm);
        pEm->Routine_2++;
        break;
    case 3:
        if (!pEm->Routine_3) {
            Spd_get(pEm, pKan, pSeq, 0);
        } else {
            pEm->Spd_x = EF(pEm)->Unk26;
            pEm->Spd_z = EF(pEm)->Unk28;
            Spd = EF(pEm)->Unk26;
            if (Spd) {
                if (__builtin_abs(Spd) > 10) {
                    if (Spd <= 0)
                        N = Spd + 10;
                    else
                        N = Spd - 10;
                    EF(pEm)->Unk26 = N;
                } else {
                    EF(pEm)->Unk26 = 0;
                }
            }
            SpdZ = EF(pEm)->Unk28;
            if (SpdZ) {
                if (__builtin_abs(SpdZ) > 10) {
                    if (SpdZ <= 0)
                        N = SpdZ + 10;
                    else
                        N = SpdZ - 10;
                    EF(pEm)->Unk28 = N;
                } else {
                    EF(pEm)->Unk28 = 0;
                }
            }
        }
        if (Joint_move(pEm, pKan, pSeq, 0))
            EM_SET_RTN(pEm, 7, 0, 0, 0);
        Add_speedXZ(pEm, 0);
        break;
    case 4:
        break;
    }
}

/* hit flash (die variant of func_80109F7C) */
void func_8010AD38(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;

    pEm->Unk178[2] = 0x1000;
    pEm->Unk178[0] = 0;
    pEm->Unk178[1] = 0;
    pEm->Unk17E = 0x40;
    pEm->Be_flg |= 8;
    if (EF(pEm)->Unk03 == 1) {
        switch (pEm->Unk132 & 3) {
        case 1:
            pP = &pEm->pSin_parts_ptr[15];
            pP->UnkA2 |= 0x40;
            pP = &pEm->pSin_parts_ptr[18];
            pP->UnkA2 |= 0x40;
            break;
        case 2:
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 0x40;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 0x40;
            break;
        case 3:
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 0x40;
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 |= 0x40;
            break;
        }
    } else {
        pP = &pEm->pSin_parts_ptr[11];
        pP->UnkA2 |= 0x40;
        pP = &pEm->pSin_parts_ptr[14];
        pP->UnkA2 |= 0x40;
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1503, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
}

/* three blood splashes, then darken the parts */
void func_8010AEA0(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    LONG i;

    for (i = 0; i < 3; i++) {
        if (EF(pEm)->Unk03 == 1) {
            switch (pEm->Unk132 & 3) {
            case 1:
                pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 15];
                break;
            case 2:
                pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
                break;
            case 3:
                pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
                break;
            }
        } else {
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
        }
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        Esp_call(0x205, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
    }
    for (i = 0; i < 21; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
        pP->Poly_rgb = 0x202F2F;
    }
}

/* same as func_8010A27C */
void func_8010B024(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;

    if (EF(pEm)->Unk03 == 1) {
        switch (pEm->Unk132 & 3) {
        case 1:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 15];
            break;
        case 2:
            pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
            break;
        case 3:
            pP = &pEm->pSin_parts_ptr[Rnd_l() & 3];
            break;
        }
    } else {
        pP = &pEm->pSin_parts_ptr[(Rnd_l() & 3) + 10];
    }
    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x1503, ((pEm->Cdir_y + Rnd_l()) & 0xFFFF) | 0x14000000, &pP->Workm, &Sv, 0);
}

/* tear off the legs when killed (die damage) */
void func_8010B154(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    LONG i;

    switch ((pEm->Unk132 & 0xC) >> 2) {
    case 0:
        if (!(EF(pEm)->Unk2C & 1)) {
            i = 2;
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 |= 0x20;
            pP->Be_flg = (pP->Be_flg | 2) & ~0x20;
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[1] = -240;
            PW_FREE(pP)[2] = 0;
            PW_FREE(pP)[3] = 15;
            PW_FREE(pP)[6] = 0;
            for (; i < 5; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb = 0x2F2F2F;
                pP->Be_flg |= 0xA;
                pP->Old_x = pEm->Cdir_y + 0x800;
                pP->Unk96 = 0;
                pP->Old_z = 100;
                pP->Old_y = 0;
                pP->Unk85 = 30;
                pP->Unk90 = 0;
                pP->Unk92 = pEm->Cdir_y;
                pP->Unk8E = 0x400;
            }
            EF(pEm)->Unk2C |= 1;
        }
    case 1:
        if (!(EF(pEm)->Unk2C & 4)) {
            i = 11;
            pP = &pEm->pSin_parts_ptr[11];
            pP->UnkA2 |= 0x20;
            pP->Be_flg = (pP->Be_flg | 2) & ~0x20;
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[1] = -240;
            PW_FREE(pP)[2] = 0;
            PW_FREE(pP)[3] = 15;
            PW_FREE(pP)[6] = 0;
            for (; i < 14; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb = 0x2F2F2F;
                pP->Be_flg |= 0xA;
                pP->Old_x = pEm->Cdir_y + 0x800;
                pP->Unk96 = 0;
                pP->Old_z = 100;
                pP->Old_y = 0;
                pP->Unk85 = 30;
                pP->Unk90 = 0;
                pP->Unk92 = pEm->Cdir_y;
                pP->Unk8E = 0x400;
            }
            EF(pEm)->Unk2C |= 4;
        }
    case 2:
        if (!(EF(pEm)->Unk2C & 8)) {
            pP = &pEm->pSin_parts_ptr[15];
            pP->UnkA2 |= 0x20;
            pP->Be_flg = (pP->Be_flg | 2) & ~0x20;
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[1] = -240;
            PW_FREE(pP)[2] = 0;
            PW_FREE(pP)[3] = 15;
            PW_FREE(pP)[6] = 0;
            for (i = 15; i < 17; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb = 0x2F2F2F;
                pP->Be_flg |= 0xA;
                pP->Old_x = pEm->Cdir_y + 0x800;
                pP->Unk96 = 0;
                pP->Old_z = 100;
                pP->Old_y = 0;
                pP->Unk85 = 30;
                pP->Unk90 = 0;
                pP->Unk92 = pEm->Cdir_y;
                pP->Unk8E = 0x400;
            }
            EF(pEm)->Unk2C |= 8;
        }
    }
    for (i = 0; i < 21; i++) {
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 &= ~0x40;
        pP->Poly_rgb = 0x202F2F;
    }
}

/* die00: dies on the floor, or falls off the wall / ceiling first */
void func_8010B494(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Unused;
    KAGE_WORK *pK;

    if (!pEm->Routine_2) {
        Em_dead_flg_on_cand_();
        if (EF_W0(pEm) & 0xFFFF00)
            pEm->Routine_2 = 2;
    }
    switch (pEm->Routine_2) {
    case 0:
        pEm->Unk16A = 7;
        if (pEm->Unk12C == 11 || pEm->Unk12C == 14 || pEm->Unk12C == 15)
            pEm->Unk16A = 3;
        EM_SET_MOVE(pEm, 0x1E, 0, 0);
        Snd_se_enem(9, pEm);
        pEm->Routine_2++;
        break;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0)) {
            pK = pEm->Kage.pKw;
            *(ULONG *)&pK->Prim[0].r0 = (*(ULONG *)&pK->Prim[0].r0 & 0xFF000000) | 0xEF409F;
            *(ULONG *)&pK->Prim[1].r0 = (*(ULONG *)&pK->Prim[1].r0 & 0xFF000000) | 0xEF409F;
            pEm->Timer0 = 90;
            pEm->UnkDA = 1;
            pEm->Unk16A = 10;
            EM_SET_RTN(pEm, 7, 0, 3, 0);
        }
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        pEm->Col.Unk28 = 0;
        pEm->Col.Unk26 = 0;
        pEm->Col.Unk24 = 0;
        pEm->Pos_x = pEm->pSin_parts_ptr->Workm.t[0];
        pEm->Pos_z = pEm->pSin_parts_ptr->Workm.t[2];
        pEm->Routine_2 = 3;
        EF(pEm)->Unk16 = 2;
        EM_SET_MOVE(pEm, 0x18, 0, 7);
        pEm->Cdir_z = 0;
        pEm->Cdir_x = 0;
        pEm->Cdir_y = (EF(pEm)->Mode == 4) ? pEm->Cdir_y - 0x800 : EF(pEm)->Side * 0x400;
        pEm->Timer0 = 0;
        EF(pEm)->Unk26 = 0;
        EF(pEm)->Unk28 = 0;
        pEm->Routine_3 = 0;
    case 3:
        if (!pEm->Timer0)
            pEm->Timer0 = Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Pos_y += EF(pEm)->Unk16;
        EF(pEm)->Unk16 += EF(pEm)->Unk16;
        if (pEm->Pos_y > pEm->Unk122) {
            EF(pEm)->Unk16 = 0;
            EF(pEm)->Mode = 0;
            pEm->Col.D0 = 800;
            pEm->Col.W0 = 800;
            pEm->Pos_y = pEm->Unk122;
            Snd_se_enem(1, pEm);
        }
        if (pEm->Timer0 && pEm->Pos_y == pEm->Unk122) {
            Snd_se_enem(9, pEm);
            EM_SET_RTN(pEm, 7, 0, 0, 0);
        }
        break;
    }
}

/* RE2 Em23_Scenario */
void func_8010B754(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Type & 0x100)
        func_80016600(pEm, pKan, pSeq);
    else
        Em23_sce[pEm->Routine_1](pEm, pKan, pSeq);
    Em_seq_sound(pEm);
}

void func_8010B7C0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    EF(pEm)->Unk22 = -2400;
    EF(pEm)->Unk24 = -1300;
    EF(pEm)->Unk26 = (EF(pEm)->Unk22 - pEm->Pos_x) / 16;
    EF(pEm)->Unk28 = (EF(pEm)->Unk24 - pEm->Pos_z) / 16;
    EF(pEm)->Unk16 = -200;
    EM_SET_RTN(pEm, 1, 0xB, 1, 0);
    pEm->Type = 0x4000;
    EM_SET_MOVE(pEm, 7, 0, 7);
    pEm->Be_flg &= ~0x2020;
    func_80106AA8(pEm, pKan, pSeq);
}

/* die03: sink and fade out */
void func_8010B868(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    MATRIX Unused;
    SVECTOR Unused2;
    KAGE_WORK *pK = pEm->Kage.pKw;
    PARTS_W *pP;
    LONG i;
    LONG T;
    LONG C;

    switch (pEm->Routine_2) {
    case 0:
        if (!(pEm->Type & 0x8000))
            pEm->Routine_2++;
        break;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        if (pEm->Routine_2 != 2)
            pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Spd_x = -pEm->Spd_x;
        pEm->Spd_z = -pEm->Spd_z;
        Add_speedXZ(pEm, 0);
        if (pEm->Move_cnt == 10) {
            pP = &pEm->pSin_parts_ptr[14];
            pP->Poly_rgb = 0;
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 14) {
            for (i = 8; i < 14; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb = 0;
                pP->UnkA2 |= 1;
            }
        }
        if (pEm->Move_cnt == 28) {
            for (i = 15; i < 21; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb = 0;
                pP->UnkA2 |= 1;
            }
        }
        for (i = 0; i < 21; i++) {
            pP = &pEm->pSin_parts_ptr[i];
            if (pP->Poly_rgb != 0x808080)
                pP->Poly_rgb += 0x101010;
        }
        break;
    case 2:
        pEm->Timer0 = 10;
        pEm->Routine_2++;
    case 3:
        if (--pEm->Timer0)
            pEm->Pos_y -= 100;
        else
            pEm->Routine_2++;
        T = pEm->Timer0;
        if (T < 8) {
            *(ULONG *)&pK->Prim[0].r0 &= 0xFF000000;
            *(ULONG *)&pK->Prim[1].r0 &= 0xFF000000;
            C = (T << 4) | ((T << 20) | (T << 12));
            *(ULONG *)&pK->Prim[0].r0 |= C;
            *(ULONG *)&pK->Prim[1].r0 |= C;
        }
        break;
    case 4:
        pEm->Be_flg |= 0x30;
        break;
    }
}

/* die01: lies dead, twitching */
void func_8010BB3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pK = pEm->Kage.pKw;

    switch (pEm->Routine_2) {
    case 0:
        func_80108F68(pEm);
        if (Rnd_l() & 1) {
            EM_SET_MOVE(pEm, 0x13, 0, 7);
            pEm->UnkDA = 0;
        } else {
            EM_SET_MOVE(pEm, 0x19, 0, 7);
            pEm->UnkDA = 2;
        }
        pEm->Routine_2++;
        *(ULONG *)&pK->Prim[0].r0 = (*(ULONG *)&pK->Prim[0].r0 & 0xFF000000) | 0xEF409F;
        *(ULONG *)&pK->Prim[1].r0 = (*(ULONG *)&pK->Prim[1].r0 & 0xFF000000) | 0xEF409F;
        pEm->Timer0 = 90;
    case 1:
        if (!(pEm->UnkDA & 1))
            pEm->UnkDA |= Joint_move(pEm, pKan, pSeq, 0x200);
        if (pEm->Col.W0)
            pEm->Col.W0 -= 20;
        if (pEm->Col.D0)
            pEm->Col.D0 -= 20;
        if (!(pEm->Col.W0 | pEm->Col.D0))
            pEm->Col.Id = 0;
        if (!pEm->Col.Id && (pEm->UnkDA & 1) && !(pEm->UnkDA & 2)) {
            EM_SET_MOVE(pEm, 0x14, 0, 7);
            pEm->UnkE2 = 1;
            pEm->Routine_2++;
            pEm->UnkDA = Rnd_l();
        }
        break;
    case 2:
        if (pEm->UnkE2) {
            if (pEm->UnkDA)
                pEm->UnkDA--;
            else {
                pEm->UnkDA = Rnd_l() & 0x7F;
                pEm->UnkE2 = (Rnd_l() & 3) != 0;
            }
        } else {
            pEm->UnkE2 = Joint_move(pEm, pKan, pSeq, 0x100);
        }
        break;
    case 3:
        if (!pEm->UnkDA)
            pEm->UnkDA = Joint_move(pEm, pKan, pSeq, 0x200);
        if (pEm->Col.W0)
            pEm->Col.W0 -= 20;
        if (pEm->Col.D0)
            pEm->Col.D0 -= 20;
        if (!(pEm->Col.W0 | pEm->Col.D0))
            pEm->Col.Id = 0;
        break;
    }
    if (pEm->Timer0) {
        pEm->Timer0--;
        pK->Half_x += 8;
        pK->Half_z += 8;
    }
}

/* RE2 Pldamage_Em23: the player while held (G.pPldamage_Em) */
void func_8010BE00(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEm = pPl->pEnemy_ptr;

    if (pEm->Routine_0 != 1) {
        EM_SET_RTN(pPl, 1, 0, 0, 0);
        pPl->Be_flg = (pPl->Be_flg | 0x80000) & 0xFFFF77FF;
        pEm->Be_flg &= 0xFFFF77FF;
        pPl->Unk120 &= 0xED;
        pPl->Damage_cnt &= 0x7F;
        Player_move(pPl);
    } else {
        Pldamage_Em23_tbl[pPl->Routine_1](pPl, pKan, pSeq);
    }
}

/* the held player throws it off (Routine_3: front / back) */
void func_8010BEA4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = pEm->pEnemy_ptr;
    SHORT Add = !pEm->Routine_3 << 11;
    SHORT X;
    SHORT Z;

    switch (pEm->Routine_2) {
    case 0:
        if (pEm->Routine_3) {
            EM_SET_MOVE(pEm, 4, 0, 3);
            Snd_se_stad(0x10002, &pEm->Pos_x, 0, 0);
        } else {
            EM_SET_MOVE(pEm, 0, 0, 3);
            Snd_se_stad(0x10001, &pEm->Pos_x, 0, 0);
        }
        func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
        pEm->Routine_2++;
        pEm->Unk120 |= 0x12;
    case 1:
        X = pEm->Pos_x;
        Z = pEm->Pos_z;
        Goto01(pEm, pPl->Cdir_y + Add, 300);
        pEm->Cdir_y += Add;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= Add;
        if (Joint_move2(pEm, pKan, pSeq, 0x100))
            pEm->Routine_2++;
        pEm->Pos_x -= ((pEm->Pos_x - X) * pEm->Hokan_flg) >> 2;
        pEm->Pos_z -= ((pEm->Pos_z - Z) * pEm->Hokan_flg) >> 2;
        break;
    case 2:
        if (pEm->Routine_3)
            EM_SET_MOVE(pEm, 5, 0, 7);
        else
            EM_SET_MOVE(pEm, 1, 0, 7);
        Pad_vib_set0(3, 10);
        Pad_vib_set1(3, 0xC8, 14);
        Pad_vib_set0(7, 15);
        Pad_vib_set1(3, 0xFA, 19);
        Pad_vib_set0(3, 20);
        Pad_vib_set1(3, 0xD2, 24);
        Pad_vib_set0(3, 25);
        Pad_vib_set1(3, 0x96, 29);
        pEm->Routine_2++;
    case 3:
        X = pEm->Pos_x;
        Z = pEm->Pos_z;
        pEm->Cdir_y += Add;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= Add;
        if (Joint_move2(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2++;
            Pad_vib_set0(3, 10);
            Pad_vib_set1(3, 0xC8, 14);
            Pad_vib_set0(7, 15);
            Pad_vib_set1(3, 0x96, 19);
            Pad_vib_set0(3, 20);
        }
        if (Gacha_ch() && Joint_move2(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2++;
            Pad_vib_set0(3, 10);
            Pad_vib_set1(3, 0xC8, 14);
            Pad_vib_set0(7, 15);
            Pad_vib_set1(3, 0x96, 19);
            Pad_vib_set0(3, 20);
        }
        pEm->Pos_x -= ((pEm->Pos_x - X) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Z) * pEm->Hokan_flg) >> 3;
        break;
    case 4:
        if (pEm->Routine_3)
            EM_SET_MOVE(pEm, 6, 0, 7);
        else
            EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Be_flg |= 0x80000;
        pEm->Unk120 &= 0xED;
        pEm->Routine_2++;
    case 5:
        if (pEm->Move_cnt == 24) {
            pEm->Be_flg &= 0xFFFF77FF;
            pPl->Be_flg &= 0xFFFF77FF;
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt &= 0x7F;
        }
    }
}

/* the player shakes it off (Routine_3: front / back) */
void func_8010C314(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = pEm->pEnemy_ptr;
    SHORT Add = !pEm->Routine_3 << 11;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C_(pEm, 300, 60, pEm->Col.Unk39);
        if (pEm->Routine_3) {
            EM_SET_MOVE(pEm, 9, 0, 7);
            Snd_se_stad(0x10002, &pEm->Pos_x, 0, 0);
        } else {
            EM_SET_MOVE(pEm, 8, 0, 7);
            Snd_se_stad(0x10001, &pEm->Pos_x, 0, 0);
        }
        pEm->Unk120 |= 0x12;
        pEm->Routine_2++;
    case 1:
        Goto01(pEm, pPl->Cdir_y + Add, 300);
        pEm->Cdir_y += Add;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= Add;
        if (Joint_move2(pEm, pKan, pSeq, 0x100))
            EM_RTN(pEm) = (pEm->Routine_3 << 24) + 0x20005;
    }
}

void func_8010C470(void) {
}

/* RE2 Pldie_Em23 */
void func_8010C478(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldie_Em23_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* the player is killed: Routine_3 front / back */
void func_8010C4B4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = pEm->pEnemy_ptr;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        G.System_flg |= 0x20800;
        if (Cdir_ck_l(pEm, pPl)) {
            EM_SET_MOVE(pEm, 7, 0, 0xF);
            pEm->Routine_3 = 1;
        } else {
            EM_SET_MOVE(pEm, 3, 0, 0xF);
            pEm->Routine_3 = 0;
        }
        Pad_vib_set0(5, 12);
        Pad_vib_set1(3, 0xFA, 14);
        Pad_vib_set0(7, 25);
        Pad_vib_set1(3, 0xFA, 29);
        Pad_vib_set0(3, 30);
        Pad_vib_set1(3, 0xD2, 34);
        Pad_vib_set0(3, 35);
        Pad_vib_set1(3, 0x96, 39);
        pEm->Unk120 |= 0x12;
        pEm->Routine_2++;
    case 1:
        Add = !pEm->Routine_3 << 11;
        pEm->Cdir_y += Add;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= Add;
        if (Joint_move2(pEm, pKan, pSeq, 0x100))
            pEm->Routine_2++;
        if (pEm->Move_cnt == 55)
            Snd_se_stad(0x10003, &pEm->Pos_x, 0, 0);
        break;
    case 2:
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
}

/* put the 0x27/0x6000 enemy 250 in front of this one */
void func_8010C694(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    ENEMY_WORK **ppE;
    ENEMY_WORK *pE;

    ppE = func_8010C76C(&G.pEnemy[0]);
    if (ppE) {
        pE = *ppE;
        Sv.vx = -250;
        Sv.vz = 0;
        Rot_vector(pEm->Cdir_y, &Sv, &Sv);
        pE->Pos_x = pE->Old_x = pEm->Pos_x + Sv.vx;
        pE->Pos_y = pE->Old_x = pEm->Pos_y;
        pE->Pos_z = pE->Old_x = pEm->Pos_z + Sv.vz;
        pE->Unk122 = pEm->Unk122;
        pE->nFloor = pEm->nFloor;
        pE->Type = 0x6001;
        EM_SET_RTN(pE, 5, 0, 0, 0);
        pE->Be_flg = 0x405001;
    }
}

/* find the enemy with id 0x27 and type 0x6000 */
ENEMY_WORK **func_8010C76C(ENEMY_WORK **ppWork)
{
    do {
        ppWork++;
        if ((*ppWork)->Id == 0x27 && (*ppWork)->Type == 0x6000)
            return ppWork;
    } while (ppWork != G.ppEnemy_end);
    return NULL;
}
