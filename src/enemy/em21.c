#include "enemy/em21.h"
#include "joint.h"
#include "root_ck.h"
#include "sca.h"
#include "snd.h"

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");     /* em_sub.c Cdir_ck, int result as in RE2 EM21.H */
void Add_speedXZ_n(ENEMY_WORK *pEm, LONG muki, LONG Dummy) __asm__("Add_speedXZ"); /* RE2 EM21 passes a 3rd arg */
LONG func_800502B8_(SVECTOR *pSv, struct tagSCA_DATA *pT) __asm__("func_800502B8"); /* returns long here (canonical unsigned short); arg 1 SVECTOR* (canonical VECTOR*); arg 2 struct tagSCA_DATA* (canonical SCA_DATA*) */

/* RE2 EM_MAIN (Em121/Em221) */
void func_8010014C(ENEMY_WORK *pEm)
{
    VECTOR Vec0, Vec1;

    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    pEm->nFloor = pEm->Pos_y / -1800;
    if (!(EF(pEm)->Flg & 0x100))
        pEm->Unk122 = func_8004D720(&pEm->Pos_x, 350, 0x200, 1);
    Vec0.vx = pEm->Pos_x;
    Vec0.vy = pEm->Pos_y;
    Vec0.vz = pEm->Pos_z;
    Vec1.vx = G.Player.Pos_x;
    Vec1.vy = G.Player.Pos_y;
    Vec1.vz = G.Player.Pos_z;
    if (pEm->Status_flg & 0x2000)
        EF(pEm)->Flg |= 2;
    else
        EF(pEm)->Flg &= ~2;
    if ((LONG)pEm->Unk154 < 1100)
        pEm->Col.Unk26 = 180;
    else
        pEm->Col.Unk26 = 530;
    func_80104D5C();
    Em21_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    func_801002F8(pEm);
    func_80104AC4(pEm);
    EF(pEm)->Old_x = pEm->Pos_x;
    EF(pEm)->Old_y = pEm->Pos_y;
    EF(pEm)->Old_z = pEm->Pos_z;
    if (G.pEnemy[3] == pEm) {
        Print_color(3, 1, 0);
        Print8(50, 50, "%04x, %d", pEm->Col.Hit_flg, EF(pEm)->Sca_cnt);
    }
}

/* RE2 Em21_kage_set */
void func_801002F8(ENEMY_WORK *pEm)
{
    KAGE_WORK *pK = pEm->Kage.pKw;
    POLY_FT4 *pPrim = &pK->Prim[G.Cbuf_id];
    LONG Dy;
    LONG Half;

    if (pEm->Routine_0 != 7) {
        Dy = pEm->Unk122 - pEm->Pos_y - 250;
        if (Dy > 0) {
            Half = 0x57E40 / ((Dy + 0x708) >> 1);
            *(ULONG *)&pK->Half_x = Half | (Half << 16);
            if (Dy > 0x1900)
                *(ULONG *)&pPrim->r0 = 0x2E000000;
            else
                *(ULONG *)&pPrim->r0 = 0x2E808080 - (Dy / 100) * 0x20202;
        } else {
            *(ULONG *)&pK->Half_x = 0x1900190;
        }
    }
}

/* RE2 Em21_Init */
void func_801003D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    EM_RTN(pEm) = 1;
    pEm->Unk144 = 0;
    pEm->Spd_x = 0;
    pEm->Spd_y = 0;
    pEm->Spd_z = 0;
    pEm->Life = 5;
    pEm->Unk1CC[0] = 0;
    pEm->Unk1CC[1] = 0;
    pEm->Unk1CC[2] = 0;
    pEm->Unk1CC[3] = 0;
    pEm->Unk1CC[4] = 0;
    pEm->Unk1CC[5] = 0;
    pEm->Unk1CC[6] = 0;
    pEm->Unk1CC[7] = 0;
    pEm->Add_dir = 0;
    pEm->Unk14A = 0;
    pEm->Unk14C = 0;
    pEm->Unk14E = 0;
    EM_MOVE(pEm) = 0;
    pEm->Be_flg |= 0x400;
    Joint_move(pEm, pKan, pSeq, 0x200);
    pP = &pEm->pSin_parts_ptr[2];
    pEm->Unk120 = 0;
    pEm->Neck_no = 8;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[2] = 0x60;
    PW_FREE(pP)[3] = 0x60;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[5] = 0x138;
    pEm->Unk134 = 400;
    pEm->Unk136 = -400;
    pEm->Unk13A = 400;
    pEm->Unk138 = 0x54;
    pEm->Unk139 = 0xA8;
    pEm->Col.Unk26 = 0x212;
    pEm->Col.H = 0x424;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    pEm->Unk140 = 0;
    pEm->Col.pEm = pEm;
    pEm->Col.Unk24 = 0;
    pEm->Col.Unk28 = 0;
    pEm->Col.Flg = 0;
    pEm->Col.Type = 2;
    func_80104D50(pEm, 350);
    func_80033580(&pEm->Col, 0);
    Kage_work_set(&pEm->Kage.pKw, 0, 0xC800C8, 0, (VECTOR *)&pEm->Pos_x);
    if (pEm->Type & 0x4000) {
        pEm->Routine_0 = 4;
        pEm->Type &= ~0x4000;
        EF(pEm)->Flg |= 1;
        pEm->Routine_1 = (UCHAR)pEm->Type & 0xF;
        Em21_routine_0[pEm->Routine_0](pEm, pKan, pSeq);
    }
    if (pEm->Type & 0x40) {
        pEm->Damage_cnt = 0x80;
        pEm->Be_flg |= 0x2000;
    }
    G.pPldamage_Em[0x21 - 0x10] = (void (*)())func_80104E60;
}

/* RE2 Em21_Move */
void func_801005DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG Flg;

    Flg = 0;
    if (EF(pEm)->Idly_cnt == 0) {
        if (!(Rnd_l() & 0x1F)) {
            EF(pEm)->Idly_cnt = Rnd_l() & 0x7F;
            EF(pEm)->Idly_flg = Rnd_area();
        }
    }
    if (G.Player.Life < 0) {
        if (EF(pEm)->Pl_dead_cnt) {
            if (EF(pEm)->Idly_cnt)
                Flg = 1;
            EF(pEm)->Pl_dead_cnt--;
        }
    } else if (!(EF(pEm)->Flg & 0x40)) {
        if (EF(pEm)->Idly_cnt)
            Flg = 1;
    }
    if (Flg) {
        Root_ck(pEm, (VECTOR *)&G.Player.Pos_x, EF(pEm)->Idly_flg, 1);
        if (EF(pEm)->Idly_cnt-- == 0)
            EF(pEm)->Idly_flg = 0;
    } else {
        Root_ck(pEm, (VECTOR *)&G.Player.Pos_x, 0, 0);
    }
    if (pEm->Type & 0x80) {
        Em21_outdoor_br[pEm->Routine_1](pEm);
        Em21_outdoor_mv[pEm->Routine_1](pEm, pKan, pSeq);
    } else {
        Em21_indoor_br[pEm->Routine_1](pEm);
        Em21_indoor_mv[pEm->Routine_1](pEm, pKan, pSeq);
    }
    if (EF(pEm)->Atck) {
        Em21_hit_area.pPos = (VECTOR *)&pEm->Pos_x;
        Em21_hit_area.pBox = Em21_peck_box0;
        Em21_hit_area.Dir = pEm->Cdir_y;
        Em21_hit_area.Id = 1;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em21_hit_area, &Pl_work);
        Em21_hit_area.pBox = Em21_peck_box1;
        Em21_hit_area.Id = 2;
        func_800445C8(pEm->pSin_parts_ptr[8].Workm.t, &pEm->Pos_x, &Em21_hit_area, &Pl_work);
    }
}

/* RE2 Em21_br_wait_i */
void func_80100834(ENEMY_WORK *pEm)
{
    LONG D0;

    D0 = 0;
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0 && (LONG)pEm->Unk154 < 1800)
        return;
    if ((LONG)pEm->Unk154 <= 1800)
        D0 = 1;
    if ((G.Unk75E8 & 4) && (LONG)pEm->Unk154 <= 5400)
        D0 = 1;
    if (G.Unk75E8 & 1)
        D0 = 1;
    if (EF(pEm)->Flg & 1) {
        if (pEm->Timer0-- == 0)
            D0 = 1;
    }
    if (D0)
        func_80104648(pEm, 1);
}

/* RE2 Em21_mv_wait_i */
void func_8010091C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EF(pEm)->Cnt = Rnd_l() % 100;
        pEm->Routine_2 = 7;
        break;
    case 1:
        EM_MOVE(pEm) = 0x70000 + ((Rnd_l() % 5) << 8);
        EF(pEm)->Cnt = 5;
        pEm->Routine_2 = 10;
        break;
    case 2:
        EM_MOVE(pEm) = 0x70001;
        pEm->Routine_2 = 8;
        break;
    case 3:
        EM_MOVE(pEm) = 0x70001;
        pEm->Routine_2 = 9;
        break;
    case 4:
        EM_MOVE(pEm) = 0x70001;
        pEm->Routine_2 = 11;
        break;
    case 5:
        EM_MOVE(pEm) = 0x70002 + ((Rnd_l() % 7) << 8);
        pEm->Routine_2 = 8;
        break;
    case 6:
        EM_MOVE(pEm) = 0x7000A;
        EF(pEm)->Cnt = Rnd_l() % 5 + 15;
        EF(pEm)->Tmp_0 = pEm->Pos_x;
        EF(pEm)->Tmp_1 = pEm->Pos_z;
        pEm->Routine_2 = 13;
        break;
    case 7:
        if (EF(pEm)->Cnt == 0) {
            EF(pEm)->Tmp = Rnd_l() % 7;
            if (EF(pEm)->Tmp == 6) {
                if (!(Rnd_l() & 0xF))
                    pEm->Routine_2 = 6;
                else
                    pEm->Routine_2 = 0;
            } else {
                pEm->Routine_2 = EF(pEm)->Tmp;
            }
        }
        EF(pEm)->Cnt--;
        break;
    case 9:
        if (Joint_move(pEm, pKan, pSeq, 0x10200))
            pEm->Routine_2 = 0;
        break;
    case 10:
        if (EF(pEm)->Cnt-- == 0) {
            if (!(EF(pEm)->Flg & 0x20) && (Rnd_l() & 1))
                Snd_se_enem(6, pEm);
        }
    case 8:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 0;
        break;
    case 11:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 12;
            EF(pEm)->Cnt = Rnd_l() % 100;
        }
        break;
    case 12:
        if (EF(pEm)->Cnt == 0)
            pEm->Routine_2 = 3;
        EF(pEm)->Cnt--;
        break;
    case 13:
        Joint_move(pEm, pKan, pSeq, 0x10200);
        func_801045BC(pEm);
        if (pEm->Move_cnt == EF(pEm)->Cnt) {
            pEm->Routine_2 = 14;
            EM_MOVE(pEm) = 0x7000A + ((0x27 - EF(pEm)->Cnt) << 8);
        }
        break;
    case 14:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 0;
            pEm->Pos_x = EF(pEm)->Tmp_0;
            pEm->Pos_z = EF(pEm)->Tmp_1;
        } else {
            func_801045BC(pEm);
        }
    }
}

/* RE2 Em21_br_go_wait_i */
void func_80100D1C(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_go_wait_i */
void func_80100D24(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EF(pEm)->Cnt = Rnd_l() & 7;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            if (pEm->Type & 0x40)
                func_80104648(pEm, 3);
            else
                func_80104648(pEm, 2);
        }
    }
}

/* RE2 Em21_br_take_off_i */
void func_80100DB8(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_take_off_i */
void func_80100DC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        D0 = Rnd_l() & 7;
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 7))
            func_80104C94(pEm, 1);
        EM_MOVE(pEm) = 0x70007 + (D0 << 8);
        EF(pEm)->Sca_cnt = 0;
        pEm->Spd_x = (Rnd_l() & 0x1F) + 100;
        pEm->Spd_y = -10;
        EF(pEm)->Flg |= 1;
        EF(pEm)->Cnt = 9 - D0;
        EF(pEm)->Tmp = 1;
        pEm->Routine_2 = 5;
        goto move;
    case 1:
        pEm->Spd_y = -80;
        EF(pEm)->Cnt = 6;
        EF(pEm)->Tmp = 2;
        pEm->Routine_2 = 5;
        goto move;
    case 2:
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        pEm->Spd_y = -60;
        EF(pEm)->Cnt = 4;
        EF(pEm)->Tmp = 3;
        pEm->Routine_2 = 5;
        goto move;
    case 3:
        pEm->Spd_y = -100;
        EF(pEm)->Cnt = 9;
        EF(pEm)->Tmp = 4;
        pEm->Routine_2 = 5;
        goto move;
    case 4:
        if (!(Rnd_l() & 7))
            func_80104C94(pEm, 1);
        pEm->Spd_y = -80;
        EF(pEm)->Cnt = 0xFF;
        pEm->Routine_2 = 5;
    case 5:
    move:
        EF(pEm)->Cnt--;
        if (EF(pEm)->Cnt == 0)
            pEm->Routine_2 = EF(pEm)->Tmp;
        if (Rnd_l() & 1)
            Goto00(pEm, -pEm->Pos_x + G.Player.Pos_x, -pEm->Pos_z + G.Player.Pos_z, 0x20);
        else
            Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x++;
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            func_80104648(pEm, 4);
    }
}

/* RE2 Em21_br_glide_i */
void func_80101044(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_glide_i */
void func_8010104C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Type &= ~0x40;
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        D0 = Rnd_l() & 7;
        EM_MOVE(pEm) = 0x70006 + (D0 << 8);
        EF(pEm)->Sca_cnt = 0;
        pEm->Spd_x = (Rnd_l() & 7) + 60;
        pEm->Spd_y = 0;
        EF(pEm)->Flg |= 1;
        EF(pEm)->Cnt = 12;
        EF(pEm)->Cnt2 = 0;
        EF(pEm)->Tmp = 1;
        pEm->Routine_2 = 2;
        goto move;
    case 1:
        pEm->Be_flg &= ~0x2000;
        pEm->Damage_cnt &= 0x7F;
        pEm->Spd_x = (Rnd_l() & 7) + 80;
        pEm->Spd_y = 80;
        EF(pEm)->Cnt = 0xFF;
        EF(pEm)->Cnt2 = 0x20;
        pEm->Routine_2 = 2;
    case 2:
    move:
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, EF(pEm)->Cnt2);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x++;
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 3;
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = EF(pEm)->Tmp;
        break;
    case 3:
        func_80104648(pEm, 4);
    }
}

/* RE2 Em21_br_flight_i */
void func_80101238(ENEMY_WORK *pEm)
{
    if (pEm->Type & 0x80) {
        if ((LONG)pEm->Unk154 > 10800) {
            if (Rnd_l() & 0xF)
                func_80104648(pEm, 8);
            else
                func_80104648(pEm, 7);
        }
    } else if ((LONG)pEm->Unk154 > 7200) {
        if (Rnd_l() & 3)
            func_80104648(pEm, 8);
        else
            func_80104648(pEm, 7);
    }
    if (G.Player.Pos_y < pEm->Pos_y) {
        pEm->Timer0 = Rnd_l() & 0x3F;
        EF(pEm)->Tmp_0 = -120;
        func_80104648(pEm, 5);
    } else if (pEm->Pos_y - pEm->Unk122 > -1800) {
        pEm->Timer0 = Rnd_l() & 0x3F;
        EF(pEm)->Tmp_0 = -120;
        func_80104648(pEm, 5);
    } else if (!(Rnd_l() & 0xF)) {
        pEm->Timer0 = Rnd_l() & 0x3F;
        if (pEm->Unk122 - 3600 < pEm->Pos_y) {
            EF(pEm)->Tmp_0 = -40;
            func_80104648(pEm, 5);
        } else {
            func_80104648(pEm, 6);
        }
    }
    func_80104654(pEm);
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
        func_80104A90(pEm);
    } else {
        func_80104A08(pEm);
        func_801047A8(pEm);
    }
}

/* RE2 Em21_mv_flight_i */
void func_80101400(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        if (Rnd_l() & 1) {
            EM_MOVE(pEm) = 0x70003;
            EF(pEm)->Tmp = 2;
        } else {
            EM_MOVE(pEm) = 0x70004;
            if (!(Rnd_l() & 7))
                func_80104C94(pEm, 1);
            EF(pEm)->Tmp = 3;
        }
        EF(pEm)->Cnt = Rnd_l() & 0x1F;
        pEm->Spd_y = 0;
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x += EF(pEm)->Tmp;
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 0;
    }
}

/* RE2 Em21_br_up_i */
void func_80101538(ENEMY_WORK *pEm)
{
    if (pEm->Type & 0x80) {
        if (pEm->Pos_y < pEm->Unk122 - 6000)
            func_80104648(pEm, 4);
    } else if (pEm->Pos_y < pEm->Unk122 - 4800) {
        func_80104648(pEm, 4);
    }
    if (pEm->Timer0-- == 0)
        func_80104648(pEm, 4);
    func_80104654(pEm);
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
        func_80104A90(pEm);
    } else {
        func_80104A08(pEm);
        func_801047A8(pEm);
    }
}

/* RE2 Em21_mv_up_i */
void func_80101620(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70004;
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
        pEm->Spd_y = EF(pEm)->Tmp_0;
    case 1:
        if (EF(pEm)->Cnt-- == 0)
            EF(pEm)->Cnt = 9;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x += 2;
        Joint_move(pEm, pKan, pSeq, 0x200);
    }
}

/* RE2 Em21_br_down_i */
void func_80101708(ENEMY_WORK *pEm)
{
    if (pEm->Unk122 - 1800 < pEm->Pos_y)
        func_80104648(pEm, 4);
    if (pEm->Timer0-- == 0)
        func_80104648(pEm, 4);
    func_80104654(pEm);
    if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
        func_80104A90(pEm);
    } else {
        func_80104A08(pEm);
        func_801047A8(pEm);
    }
}

/* RE2 Em21_mv_down_i */
void func_801017BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70003;
        pEm->Spd_y = 40;
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0)
            EF(pEm)->Cnt = 15;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Spd_x < 300)
            pEm->Spd_x++;
        Joint_move(pEm, pKan, pSeq, 0x200);
    }
}

/* RE2 Em21_br_landing_i */
void func_801018A4(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_landing_i */
void func_801018AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;
    SVECTOR Svec;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70003;
        EF(pEm)->Sca_cnt = 0;
        EF(pEm)->Cnt = 0;
        pEm->Spd_x = 50;
        pEm->Spd_y = 120;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            EF(pEm)->Cnt = 15;
        }
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        Add_speedXYZ(pEm, 0, 0);
        D0 = pEm->Unk122 - 250;
        if (pEm->Pos_y - D0 > -300)
            pEm->Routine_2 = 2;
        Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_MOVE(pEm) = 0x7000A;
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        pEm->Spd_x = 50;
        pEm->Spd_y = 15;
        EF(pEm)->Cnt = 15;
        EF(pEm)->Tmp = 3;
        pEm->Routine_2 = 5;
        goto move;
    case 3:
        if (Rnd_l() & 1)
            Snd_se_enem(1, pEm);
        EF(pEm)->Cnt = 5;
        EF(pEm)->Tmp = 4;
        pEm->Routine_2 = 5;
        EF(pEm)->Flg |= 0x100;
        goto move;
    case 4:
        pEm->Spd_x = 10;
        pEm->Spd_y = 0;
        pEm->Pos_y = pEm->Unk122 - 250;
        EF(pEm)->Cnt = 0xFF;
        pEm->Routine_2 = 5;
    case 5:
    move:
        EF(pEm)->Cnt--;
        if (EF(pEm)->Cnt == 0)
            pEm->Routine_2 = EF(pEm)->Tmp;
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        Add_speedXYZ(pEm, 0, 0);
        D0 = pEm->Unk122 - 250;
        if (D0 < pEm->Pos_y)
            pEm->Pos_y = D0;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Spd_x = 10;
            pEm->Routine_2 = 6;
            EM_MOVE(pEm) = 0x70008;
        }
        break;
    case 6:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_y = pEm->Unk122 - 250;
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EF(pEm)->Flg &= ~0x100;
            if (G.Player.Life < 0 && EF(pEm)->Pl_dead_cnt <= 0) {
                func_80104648(pEm, 0);
            } else {
                pEm->Timer0 = Rnd_l() << 1;
                func_80104648(pEm, 2);
            }
        }
    }
}

/* RE2 Em21_br_quick_turn_i */
void func_80101BE0(ENEMY_WORK *pEm)
{
    func_80104A08(pEm);
    func_80104654(pEm);
}

/* RE2 Em21_mv_quick_turn_i */
void func_80101C0C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        if (!(Rnd_l() & 7))
            func_80104C94(pEm, 1);
        EM_MOVE(pEm) = 0x70004;
        EF(pEm)->Cnt = Rnd_l() & 0x3F;
        EF(pEm)->Cnt2 = 0;
        EF(pEm)->Sca_cnt = 0;
        pEm->Spd_y = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt2-- == 0)
            EF(pEm)->Cnt2 = 14;
        if ((LONG)pEm->Unk154 < 900 && G.Player.Pos_y - 3700 < pEm->Pos_y)
            pEm->Pos_y -= 50;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);
        pEm->Cdir_y += (signed char)EF(pEm)->Tmp;
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x80) || EF(pEm)->Cnt-- == 0)
            func_80104648(pEm, 4);
    }
}

/* RE2 Em21_br_jump_i */
void func_80101D94(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_jump_i */
void func_80101D9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70008;
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x80);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            pEm->Timer0 = Rnd_l() & 0xF;
            func_80104648(pEm, 0);
        }
    }
}

/* RE2 Em21_br_fall_i */
void func_80101E54(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_mv_fall_i */
void func_80101E5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_enem(3, pEm);
        EM_MOVE(pEm) = 0x70009;
        pEm->Spd_y = 0;
        pEm->Spd_x = 0;
        EF(pEm)->Tmp_0 = 0;
        EF(pEm)->Sca_cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        EF(pEm)->Tmp_0 += 20;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        D0 = pEm->Unk122 - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            EM_MOVE(pEm) = 0x7000B;
            EF(pEm)->Cnt = Rnd_l() & 0xF;
            pEm->Cdir_z = 0;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 4;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt--;
        break;
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            func_80104638(pEm, 1, 9);
    }
}

/* RE2 Em21_br_crash_f_i */
void func_80102010(ENEMY_WORK *pEm)
{
    if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80) || !(EF(pEm)->Flg & 2))
        func_80104648(pEm, 4);
    func_80104654(pEm);
    func_80104A08(pEm);
}

/* RE2 Em21_mv_crash_f_i */
void func_8010207C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70004;
        EF(pEm)->Tmp_0 = G.Player.Unk122 - 2800;
        D0 = EF(pEm)->Tmp_0 - pEm->Pos_y;
        if (D0 < 0)
            EF(pEm)->Tmp = 0;
        else
            EF(pEm)->Tmp = 1;
        pEm->Spd_y = 0;
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            Snd_se_enem(1, pEm);
            EF(pEm)->Cnt2 = 14;
        }
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
        if (pEm->Spd_x < 300)
            pEm->Spd_x += 5;
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = G.Player.Unk122 - 2800;
        if (EF(pEm)->Tmp == 0) {
            if (D0 < pEm->Pos_y)
                pEm->Pos_y -= 40;
        } else if (pEm->Pos_y < D0) {
            pEm->Pos_y += 40;
        }
        if ((LONG)pEm->Unk154 < 900) {
            D0 = D0 - pEm->Pos_y;
            if (D0 > -360 && D0 < 360 && pEm->Spd_x > 230) {
                func_80104648(pEm, 12);
            } else {
                EF(pEm)->Idly_cnt = Rnd_l() & 0x7F;
                EF(pEm)->Idly_flg = Rnd_area();
                func_80104648(pEm, 8);
            }
        } else {
            Add_speedXYZ(pEm, 0, 0);
        }
    }
}

/* RE2 Em21_br_crash_i */
void func_80102278(ENEMY_WORK *pEm)
{
    func_80104A08(pEm);
}

/* RE2 Em21_mv_crash_i */
void func_80102298(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        if (Cdir_ck_l(pEm, &Pl_work))
            func_80104638(&Pl_work, 2, 1);
        else
            func_80104638(&Pl_work, 2, 0);
        Pad_vib_set0(4, 0);
        Pad_vib_set1(10, 150, 0);
        Snd_se_enem(4, pEm);
        EM_MOVE(pEm) = 0x70009;
        pEm->Spd_y = 0;
        pEm->Spd_x = 0;
        EF(pEm)->Obj_cnt = 0;
        EF(pEm)->Tmp_0 = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        EF(pEm)->Tmp_0 += 20;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        D0 = pEm->Unk122 - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            EM_MOVE(pEm) = 0x7000B;
            EF(pEm)->Cnt = Rnd_l() & 0xF;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 4;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt--;
        break;
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400)) {
                func_80104648(pEm, 9);
            } else {
                pEm->Timer0 = Rnd_l() & 0xF;
                func_80104648(pEm, 0);
            }
        }
    }
}

/* RE2 Em21_br_peck_f_i */
void func_801024D8(ENEMY_WORK *pEm)
{
    if (Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100) || !(EF(pEm)->Flg & 2)) {
        func_80104D50(pEm, 350);
        func_80104648(pEm, 4);
    }
    func_80104654(pEm);
    func_80104A08(pEm);
}

/* RE2 Em21_mv_peck_f_i */
void func_80102550(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D1;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70004;
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Cdir_x = 0;
        pEm->Cdir_z = 0;
        EF(pEm)->Cnt = 0;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            Snd_se_enem(1, pEm);
            EF(pEm)->Cnt2 = 14;
        }
        if (EF(pEm)->Flg & 4) {
            Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x80);
            if (pEm->Spd_x < 300)
                pEm->Spd_x += 4;
            if ((LONG)pEm->Unk154 < 2300) {
                if (EF(pEm)->Atck == 0) {
                    if (EF(pEm)->Unk0B == 0)
                        Snd_se_enem(6, pEm);
                    EF(pEm)->Unk0B = 1;
                    EF(pEm)->Atck = 1;
                }
            }
            if ((LONG)pEm->Unk154 >= 2300) {
                if (EF(pEm)->Atck == 1)
                    EF(pEm)->Atck = 0;
            }
        } else {
            Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x200);
            if (pEm->Spd_x < 300)
                pEm->Spd_x += 12;
        }
        Joint_move(pEm, pKan, pSeq, 0x200);
        EF(pEm)->Tmp_0 = G.Player.Unk122 - 2800;
        D1 = EF(pEm)->Tmp_0 - pEm->Pos_y;
        if (D1 < -60 || D1 > 60) {
            if (D1 < -60)
                pEm->Pos_y -= 60;
            else if (D1 > 60)
                pEm->Pos_y += 60;
        }
        if ((LONG)pEm->Unk154 < 900)
            func_80104D50(pEm, 100);
        if ((LONG)pEm->Unk154 < 650) {
            if (D1 > -100 && D1 < 100)
                func_80104648(pEm, 14);
        } else {
            Add_speedXYZ(pEm, 0, 0);
        }
    }
}

/* RE2 Em21_br_peck_i */
void func_801027A4(ENEMY_WORK *pEm)
{
    func_80104A08(pEm);
}

/* RE2 Em21_mv_peck_i */
void func_801027C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG D0;
    SVECTOR Svec;

    if (G.Player.Life <= 0)
        EF(pEm)->Tmp = 0;
    else
        EF(pEm)->Tmp = 1;
    switch (pEm->Routine_2) {
    case 0:
        if (G.Player.Life < 0)
            goto end;
        pEm->Spd_x = 0;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Cdir_x = 0;
        pEm->Cdir_z = 0;
        if (EF(pEm)->Flg & 4) {
            EF(pEm)->Atck = 0;
            EF(pEm)->Unk0B = 0;
            func_80104B90(8);
            func_80104B90(0x40);
            func_80104638(G_PLAYER, 5, 0);
            G.Player.pEnemy_ptr = pEm;
            G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
            G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
            G.Player.Be_flg |= 0x8800;
        }
        pEm->Pos_y = EF(pEm)->Tmp_0;
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x800);
        EM_MOVE(pEm) = 0x70005;
        EF(pEm)->Cnt = 100;
        EF(pEm)->Cnt2 = 0;
        pEm->Spd_x = 300;
        func_80104D50(pEm, 350);
        pEm->Routine_2 = 1;
        pEm->Be_flg |= 0x800;
    case 1:
        if (EF(pEm)->Flg & 4) {
            Pad_vib_set0(6, 0);
            Pad_vib_set1(4, 150, 1);
            Svec.vx = pEm->Pos_x;
            Svec.vy = pEm->Pos_y + 500;
            Svec.vz = pEm->Pos_z;
            Esp_call(0x100, (USHORT)pEm->Cdir_y | 0x10000000, &GsIDMATRIX, &Svec, NULL);
            if (!(Rnd_l() & 3))
                func_80104C94(pEm, 1);
            Snd_se_enem(2, pEm);
            D0 = Pl_life_down(5, EF(pEm)->Tmp);
            if (D0 == 1)
                goto end;
            if (D0 == 2) {
                func_80104B90(0x80);
                goto end;
            }
        } else {
            if (!(Rnd_l() & 7))
                func_80104C94(pEm, 1);
        }
        EF(pEm)->Cnt3 = 4;
        pEm->Routine_2 = 2;
    case 2:
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = Gacha_ch() * 3 + 1;
        EF(pEm)->Cnt -= D0;
        if ((signed char)EF(pEm)->Cnt < 0)
            goto end;
        if (EF(pEm)->Cnt3-- == 0) {
            EF(pEm)->Tmp_0 = 150;
            EF(pEm)->Cnt3 = EF(pEm)->Cnt2 = (Rnd_l() & 7) + 2;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = Gacha_ch() * 3 + 1;
        EF(pEm)->Cnt -= D0;
        if ((signed char)EF(pEm)->Cnt < 0)
            goto end;
        pEm->Pos_y -= EF(pEm)->Tmp_0;
        EF(pEm)->Tmp_0 -= 15;
        if (EF(pEm)->Cnt2-- == 0)
            pEm->Routine_2 = 4;
        break;
    case 4:
        Joint_move(pEm, pKan, pSeq, 0x200);
        D0 = Gacha_ch() * 3 + 1;
        EF(pEm)->Cnt -= D0;
        if ((signed char)EF(pEm)->Cnt < 0)
            goto end;
        EF(pEm)->Tmp_0 += 15;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        if (EF(pEm)->Cnt3-- == 0) {
            EM_MOVE(pEm) = 0x70005;
            pEm->Routine_2 = 1;
            EF(pEm)->Cnt2 = 7;
        }
        break;
    case 5:
    end:
        EF(pEm)->Idly_cnt = (Rnd_l() & 0x7F) + 30;
        EF(pEm)->Idly_flg = Rnd_area();
        EF(pEm)->Na_cnt = 10;
        pEm->Be_flg &= ~0x800;
        func_80104648(pEm, 4);
        if (EF(pEm)->Flg & 4) {
            func_80104B90(0x10);
            G.Player.Routine_2 = 3;
            G.Player.Be_flg &= ~0x8800;
        }
    }
}

/* RE2 Em21_Damage */
void func_80102C48(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Root_ck(pEm, (VECTOR *)&G.Player.Pos_x, 0, 0);
    Em21_dm[pEm->Unk12C - 1](pEm, pKan, pSeq);
}

/* RE2 Em21_dm00 */
void func_80102CC4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        Snd_se_enem(3, pEm);
        EM_MOVE(pEm) = 0x70009;
        pEm->Spd_y = 0;
        pEm->Spd_x = 0;
        EF(pEm)->Sca_cnt = 0;
        EF(pEm)->Tmp_0 = 0;
        EF(pEm)->Flg |= 0x100;
        D0 = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);
        EF(pEm)->Tmp_1 = D0 - pEm->Cdir_y;
        G.pEm->Spd_x = 100;
        if (Rnd_l() & 1)
            EF(pEm)->Tmp_2 = 200;
        else
            EF(pEm)->Tmp_2 = -200;
        func_80104C00(pEm);
        func_80104C94(pEm, (Rnd_l() & 1) | 2);
        EF(pEm)->Cnt = 3;
        EF(pEm)->Cnt2 = 3;
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        if (EF(pEm)->Cnt-- == 0)
            func_80104C94(pEm, 1);
        if (EF(pEm)->Cnt2-- == 0)
            func_80104C94(pEm, 1);
        EF(pEm)->Tmp_0 += 20;
        pEm->Pos_y += EF(pEm)->Tmp_0;
        Add_speedXZ(G.pEm, EF(pEm)->Tmp_1);
        if (EF(pEm)->Tmp_2 < 0) {
            if (pEm->Cdir_z > -2048)
                pEm->Cdir_z += EF(pEm)->Tmp_2;
        } else {
            if (pEm->Cdir_z < 2048)
                pEm->Cdir_z += EF(pEm)->Tmp_2;
        }
        Svec.vx = pEm->Pos_x;
        Svec.vy = pEm->Pos_y;
        Svec.vz = pEm->Pos_z;
        D0 = pEm->Unk122 - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            EM_MOVE(pEm) = 0x7000B;
            EF(pEm)->Cnt = Rnd_l() & 0xF;
            pEm->Cdir_z = 0;
            if (pEm->Life < 0) {
                EF(pEm)->Cnt = 11;
                Em_dead_flg_on_cand(pEm);
                pEm->Routine_2 = 5;
            } else {
                pEm->Routine_2 = 3;
            }
        }
        break;
    case 3:
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 4;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Move_cnt--;
        break;
    case 4:
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x400)) {
                func_80104638(pEm, 1, 9);
            } else {
                pEm->Timer0 = Rnd_l() & 0xF;
                func_80104638(pEm, 1, 0);
            }
        }
        break;
    case 5:
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0)
            func_80104638(pEm, 7, 0);
    }
}

/* RE2 Em21_dm01 */
void func_8010304C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */
    LONG D0;
    ULONG Rnd0;

    pEm->Life = -1;
    pEm->Spd_y = 0;
    D0 = Direction_ck(G.Player.Pos_x, G.Player.Pos_z, pEm->Pos_x, pEm->Pos_z);
    Rnd0 = Rnd_l();
    func_80103258(pEm->pSin_parts_ptr, D0);
    if (Rnd0 & 1) {
        func_80103258(pEm->pSin_parts_ptr + 1, D0);
        func_80103258(pEm->pSin_parts_ptr + 2, D0);
    } else {
        func_801032A4(pEm->pSin_parts_ptr + 1, D0);
        func_801032A4(pEm->pSin_parts_ptr + 2, D0);
    }
    if (Rnd0 & 2) {
        func_80103258(pEm->pSin_parts_ptr + 3, D0);
        func_80103258(pEm->pSin_parts_ptr + 4, D0);
        func_80103258(pEm->pSin_parts_ptr + 5, D0);
    } else {
        func_801032A4(pEm->pSin_parts_ptr + 3, D0);
        func_801032A4(pEm->pSin_parts_ptr + 4, D0);
        func_801032A4(pEm->pSin_parts_ptr + 5, D0);
    }
    if (Rnd0 & 4) {
        func_80103258(pEm->pSin_parts_ptr + 6, D0);
        func_80103258(pEm->pSin_parts_ptr + 7, D0);
        func_80103258(pEm->pSin_parts_ptr + 8, D0);
    } else {
        func_801032A4(pEm->pSin_parts_ptr + 6, D0);
        func_801032A4(pEm->pSin_parts_ptr + 7, D0);
        func_801032A4(pEm->pSin_parts_ptr + 8, D0);
    }
    if (Rnd0 & 8)
        func_80103258(pEm->pSin_parts_ptr + 9, D0);
    else
        func_801032A4(pEm->pSin_parts_ptr + 9, D0);
    func_80104C00(pEm);
    func_80104C00(pEm);
    func_80104C00(pEm);
    func_80104C94(pEm, (Rnd_l() & 3) | 8);
    Em_dead_flg_on_cand(pEm);
    func_80104638(pEm, 7, 1);
}

/* RE2 Em21_parts_bobm_set */
void func_80103258(PARTS_W *pP, LONG B_y)
{
    pP->UnkA2 |= 0x20;
    pP->Be_flg |= 2;
    PW_FREE(pP)[0] = B_y;
    PW_FREE(pP)[4] = 0;
    PW_FREE(pP)[2] = 800;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[3] = 10;
    PW_FREE(pP)[6] = -100;
    pP->Poly_rgb = 0x101040;
}

/* RE2 Em21_parts_fall_set */
void func_801032A4(PARTS_W *pP, LONG D_y)
{
    pP->Be_flg |= 0xA;
    pP->Old_x = D_y + (Rnd_l() << 2);
    pP->Unk96 = 0;
    pP->Old_z = (Rnd_l() & 0x7F) + 100;
    pP->Old_y = 0;
    pP->Unk85 = 20;
    pP->Unk90 = 0;
    pP->Unk92 = Rnd_l() << 4;
    pP->Unk8E = 0;
}

/* RE2 Em21_dm03 (RE3 dm02: knocked away along the player's facing) */
void func_80103320(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */
    LONG D0;

    switch (pEm->Routine_2) {
    case 0:
        EF(pEm)->Flg &= ~0x100;
        Em_dead_flg_on_cand(pEm);
        Snd_se_enem(3, pEm);
        func_80104D50(pEm, 10);
        pEm->Life = -1;
        func_80104C00(pEm);
        func_80104C94(pEm, (Rnd_l() & 3) + 5);
        pEm->Cdir_y = G.Player.Cdir_y;
        pEm->Spd_x = 600;
        EM_MOVE(pEm) = 0x70009;
        pEm->Routine_2 = 1;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->Routine_2 = 2;
    case 2:
        Add_speedXZ_n(pEm, 0, 0);
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += 20;
        D0 = pEm->Unk122 - 250;
        if (D0 < pEm->Pos_y) {
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            pEm->Spd_y = 0;
            EF(pEm)->Cnt = 16;
            pEm->Routine_2 = 3;
        }
        if (pEm->Col.Hit_flg & 0x20) {
            pEm->Pos_x = EF(pEm)->Old_x;
            pEm->Pos_y = EF(pEm)->Old_y;
            pEm->Pos_z = EF(pEm)->Old_z;
            pEm->Unk122 = func_8004D720(&pEm->Pos_x, 350, 0x200, 1);
            Snd_se_enem(3, pEm);
            func_80104C00(pEm);
            pEm->Spd_y = 0;
            pEm->Spd_x = 20;
            pEm->Cdir_y += 0x800;
            pEm->Routine_2 = 4;
        }
        break;
    case 3:
        Add_speedXZ_n(pEm, 0, 0);
        pEm->Spd_x >>= 1;
        if (EF(pEm)->Cnt-- == 0)
            func_80104638(pEm, 7, 2);
        break;
    case 4:
        pEm->Pos_y += pEm->Spd_y;
        pEm->Spd_y += 20;
        Add_speedXZ_n(pEm, 0, 0);
        D0 = pEm->Unk122 - 250;
        if (D0 < pEm->Pos_y) {
            func_80104D50(pEm, 350);
            Snd_se_enem(5, pEm);
            pEm->Pos_y = D0;
            pEm->Spd_y = 0;
            func_80104638(pEm, 7, 2);
        }
    }
}

/* RE2 Em21_Die */
void func_801035BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* RE2 Em21_Scenario */
void func_801035C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Root_ck(pEm, (VECTOR *)&G.Player.Pos_x, 0, 0);
    Em21_sce_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em21_sce_wait */
void func_8010363C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    func_8010091C(pEm, pKan, pSeq);
    switch (pEm->Routine_3) {
    case 0:
        EF(pEm)->Flg |= 0x20;
        pEm->Routine_3++;
    case 1:
        if (pEm->Add_dir == 1) {
            pEm->Add_dir = 0;
            func_80104638(pEm, 1, 1);
        }
        break;
    case 2:
        EF(pEm)->Flg &= ~0x20;
        pEm->Routine_3++;
    case 3:
        if (pEm->Add_dir == 1 || (G.Unk75E8 & 1)) {
            pEm->Add_dir = 0;
            func_80104638(pEm, 1, 1);
        }
    }
}

/* RE2 Em21_sce_crash (reworked) */
void func_80103724(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x2010;
        pEm->Type |= 0x4000;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Add_dir == 2) {
            pEm->Add_dir = 0;
            EM_MOVE(pEm) = 0x70004;
            pEm->Spd_x = 300;
            pEm->Spd_y = 0;
            pEm->Be_flg &= ~0x10;
            pEm->Type &= ~0x4000;
            EF(pEm)->Cnt = 6;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x -= 10;
        if (EF(pEm)->Cnt-- == 0) {
            pEm->Be_flg &= ~0x2000;
            func_80104638(pEm, 1, 4);
        }
    }
}

/* scenario: flies away along a fixed path */
void func_8010386C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x2000;
        pEm->Type |= 0x4000;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Add_dir == 3) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            pEm->Add_dir = 0;
            EM_MOVE(pEm) = 0x70004;
            pEm->Spd_x = 150;
            pEm->Spd_y = -150;
            pEm->Type &= ~0x4000;
            EF(pEm)->Cnt = 50;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Goto00(pEm, -22375, 761, 0x18);
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 0x3F))
            func_80104C94(pEm, 1);
        if (EF(pEm)->Cnt-- == 0) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            pEm->Spd_y = 0;
            pEm->Routine_2 = 3;
        }
        break;
    case 3:
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 0x3F))
            func_80104C94(pEm, 1);
        Goto00(pEm, 11600, -10000, 0x20);
        Joint_move(pEm, pKan, pSeq, 0x200);
        Add_speedXYZ(pEm, 0, 0);
        if (pEm->Unk14A == 1) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            pEm->Unk14A = 0;
            EM_MOVE(pEm) = 0x70004;
            pEm->Spd_y = 200;
            pEm->Type &= ~0x4000;
            pEm->Be_flg &= ~0x2000;
            pEm->Damage_cnt &= 0x7F;
            pEm->Routine_2 = 4;
        }
        break;
    case 4:
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x20);
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (pEm->Pos_y > -6000)
            func_80104638(pEm, 1, 4);
    }
}

/* scenario: circles around its start position */
void func_80103B80(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        pEm->Spd_x = 100;
        pEm->Be_flg |= 0x2000;
        pEm->Spd_y = 0;
        pEm->Routine_2 = 1;
        pEm->Type |= 0x5000;
        pEm->Dest_x = pEm->Pos_x;
        pEm->Dest_z = pEm->Pos_z;
    case 1:
        EF(pEm)->Tmp = (Rnd_l() & 0x1F) + 10;
        pEm->Spd_x = (Rnd_l() & 0x3F) + 80;
        if (Rnd_l() & 3) {
            EM_MOVE(pEm) = 0x70003;
            pEm->Routine_2 = 3;
            EF(pEm)->Cnt = 20;
            goto c3;
        }
        if (Rnd_l() & 3)
            EM_MOVE(pEm) = 0x70003;
        else
            EM_MOVE(pEm) = 0x70004;
        pEm->Routine_2 = 2;
        EF(pEm)->Cnt = Rnd_l() & 0x3F;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, EF(pEm)->Tmp);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_x = EM21_LIMIT(pEm->Pos_x, 32000);
        pEm->Pos_z = EM21_LIMIT(pEm->Pos_z, 32000);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 1;
        break;
    case 3:
    c3:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, EF(pEm)->Tmp);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_x = EM21_LIMIT(pEm->Pos_x, 32000);
        pEm->Pos_z = EM21_LIMIT(pEm->Pos_z, 32000);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (EF(pEm)->Cnt-- == 0) {
            pEm->Routine_2 = 4;
            EF(pEm)->Cnt = Rnd_l() & 0x3F;
        }
        break;
    case 4:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, EF(pEm)->Tmp);
        Add_speedXYZ(pEm, 0, 0);
        pEm->Pos_x = EM21_LIMIT(pEm->Pos_x, 32000);
        pEm->Pos_z = EM21_LIMIT(pEm->Pos_z, 32000);
        if (EF(pEm)->Cnt-- == 0)
            pEm->Routine_2 = 1;
    }
}

/* scenario: hops about on the ground */
void func_80103E98(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Svec;                      /* unused (frame size) */

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x403;
        pEm->Be_flg |= 0x4000;
        pEm->Type |= 0x5000;
        Joint_move(pEm, pKan, pSeq, 0x200);
        pEm->Routine_2 = 1;
    case 1:
        pEm->Timer0 = Rnd_l() & 0x7F;
        EF(pEm)->Tmp_0 = (signed char)Rnd_l() % 2;
        EF(pEm)->Tmp_1 = (signed char)Rnd_l() % 2;
        EF(pEm)->Tmp_2 = (signed char)Rnd_l() % 2;
        pEm->Routine_2 = 2;
    case 2:
        pEm->Pos_x += EF(pEm)->Tmp_0;
        pEm->Pos_z += EF(pEm)->Tmp_1;
        pEm->Cdir_y += EF(pEm)->Tmp_2;
        if (pEm->Timer0-- == 0)
            pEm->Routine_2 = 1;
    }
}

/* scenario: flies up out of the room */
void func_80104014(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Type |= 0x4000;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Add_dir & 1) {
            if (Rnd_l() & 1)
                Snd_se_enem(1, pEm);
            EM_MOVE(pEm) = 0x70004;
            pEm->Spd_x = 150;
            pEm->Spd_y = -150;
            pEm->Add_dir = 0;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
        Add_speedXYZ(pEm, 0, 0);
        Joint_move(pEm, pKan, pSeq, 0x200);
        if (!(Rnd_l() & 0x7F))
            Snd_se_enem(1, pEm);
        if (!(Rnd_l() & 0x3F))
            func_80104C94(pEm, 1);
        if (pEm->Pos_y < -32000) {
            pEm->Routine_2 = 3;
            pEm->Spd_y = 0;
            pEm->Be_flg |= 0x10;
            pEm->Type |= 0x8000;
        }
    }
}

/* RE2 Em21_Dead */
void func_80104194(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em21_dead_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em21_dead_normal */
void func_801041D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG D0;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x4000;
        pEm->Damage_cnt = 0x80;
        EM_MOVE(pEm) = 0x7000B;
        EF(pEm)->Cnt = (Rnd_l() & 0x1F) + 0x19;
        pEm->Timer0 = EF(pEm)->Cnt;
        Kage_work_color_set(pEm->Kage.pKw, 0xBFBF10);
        (pEm->Kage.pKw)->Half_x = 400;
        (pEm->Kage.pKw)->Half_z = 400;
        pEm->UnkDA = 60;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt-- == 0) {
            if (pEm->Timer0 > 0x5A) {
                pEm->Move_cnt = 11;
                EF(pEm)->Cnt = 6;
            } else if (Rnd_l() & 1) {
                pEm->Move_cnt = 0;
                EF(pEm)->Cnt = 12;
            } else {
                pEm->Move_cnt = 11;
                EF(pEm)->Cnt = 7;
            }
            pEm->Routine_2 = 2;
        }
        if (pEm->UnkDA) {
            pEm->UnkDA--;
            (pEm->Kage.pKw)->Half_x += 10;
            (pEm->Kage.pKw)->Half_z += 10;
        }
        break;
    case 2:
        D0 = 1;
        if (pEm->Timer0 > 0x5A) {
            EF(pEm)->Cnt2++;
            if (EF(pEm)->Cnt2 & 1)
                D0 = 0;
        }
        if (D0) {
            if (!pEm->Move_cnt && (Rnd_l() & 1) && pEm->Timer0 < 0x5A)
                Snd_se_enem(3, pEm);
            Joint_move(pEm, pKan, pSeq, 0x200);
            EF(pEm)->Cnt--;
        }
        if (!EF(pEm)->Cnt) {
            pEm->Timer0 += (Rnd_l() & 0xF) + 10;
            if (pEm->Timer0 > 0x96) {
                pEm->Routine_2 = 3;
            } else {
                EF(pEm)->Cnt = pEm->Timer0;
                pEm->Routine_2 = 1;
            }
        }
    }
}

/* RE2 Em21_dead_bomb */
void func_80104458(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Be_flg |= 0x4000;
        pEm->Damage_cnt = 0x80;
        (pEm->Kage.pKw)->Be_flg = 0;
        pEm->Routine_2 = 1;
    }
}

/* RE2 Em21_dead_instant */
void func_80104490(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Be_flg |= 0x4000;
        pEm->Damage_cnt = 0x80;
        EM_MOVE(pEm) = 0x7000B;
        Kage_work_color_set(pEm->Kage.pKw, 0xBFBF10);
        (pEm->Kage.pKw)->Half_x = 400;
        (pEm->Kage.pKw)->Half_z = 400;
        pEm->UnkDA = 60;
        EF(pEm)->Cnt = 11;
        pEm->Routine_2 = 1;
    case 1:
        if (EF(pEm)->Cnt) {
            EF(pEm)->Cnt--;
            Joint_move(pEm, pKan, pSeq, 0x200);
        }
        if (pEm->UnkDA) {
            pEm->UnkDA--;
            (pEm->Kage.pKw)->Half_x += 10;
            (pEm->Kage.pKw)->Half_z += 10;
        } else {
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        break;
    }
}

/* RE2 Em21_foot_set */
void func_801045BC(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX M;

    pP = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
    CompM((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &M);
    CompM(&M, (MATRIX *)pP[9].M, &M);
    pEm->Pos_x -= M.t[0] - pP[9].Workm.t[0];
}

/* RE2 Em21_r0_set */
void func_80104638(ENEMY_WORK *pEm, ULONG Rno0, ULONG Rno1)
{
    pEm->Routine_0 = Rno0;
    pEm->Routine_1 = Rno1;
    *(USHORT *)&pEm->Routine_2 = 0;
}

/* RE2 Em21_r1_set */
void func_80104648(ENEMY_WORK *pEm, ULONG Rno1)
{
    pEm->Routine_1 = Rno1;
    *(USHORT *)&pEm->Routine_2 = 0;
}

/* RE2 Em21_sca_avoid_ck */
void func_80104654(ENEMY_WORK *pEm)
{
    ULONG D0;
    SVECTOR Svec;
    ULONG Cdir_y;

    Cdir_y = pEm->Cdir_y & 0xFFF;
    if (pEm->Col.Hit_flg & 0x20) {
        if (!EF(pEm)->Sca_cnt) {
            if (pEm->Spd_x > 230) {
                Svec.vx = pEm->Pos_x;
                Svec.vy = pEm->Pos_y;
                Svec.vz = pEm->Pos_z;
                D0 = func_800502B8_(&Svec, pEm->Col.pSca_hit_data);
                if (D0 == 2) {
                    if (Cdir_y < 0x80 || Cdir_y > 0xF80)
                        func_80104648(pEm, 10);
                } else {
                    D0 <<= 10;
                    if (((D0 + 0x780) & 0xFFF) < Cdir_y && Cdir_y < ((D0 + 0x880) & 0xFFF))
                        func_80104648(pEm, 10);
                }
            }
        }
        if (EF(pEm)->Sca_cnt > 10) {
            if (!(Rnd_l() & 0x7F))
                func_80104648(pEm, 7);
            else
                func_80104648(pEm, 8);
        }
        if (pEm->Spd_x > 100)
            pEm->Spd_x -= 4;
        EF(pEm)->Sca_cnt++;
    } else {
        EF(pEm)->Sca_cnt = 0;
    }
}

/* RE2 Em21_attack_ck */
void func_801047A8(ENEMY_WORK *pEm)
{
    LONG Dy;

    if (G.Player.Life < 0 || G.Player.Damage_cnt)
        return;
    if ((pEm->Unk1CC[4] & 0xA0000) == 0xA0000 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100)) {
        func_80104648(pEm, 13);
        EF(pEm)->Flg &= ~0x40;
        EF(pEm)->Flg &= ~0x08;
        EF(pEm)->Na_cnt = 0;
        return;
    }
    if (EF(pEm)->Na_cnt) {
        EF(pEm)->Na_cnt--;
        return;
    }
    Dy = -(pEm->Pos_y + 2800) + G.Player.Pos_y;
    if ((LONG)pEm->Unk154 > 900 && (LONG)pEm->Unk154 < 7200 && !(G.Unk75EA & 1) && (EF(pEm)->Flg & 2)
        && Dy < 1800 && Dy > -1800 && !Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100)) {
        G.Unk75EA |= 1;
        EF(pEm)->Flg |= 4;
        if (!(Rnd_l() & 0xF))
            func_80104648(pEm, 11);
        else
            func_80104648(pEm, 13);
        return;
    }
    if ((LONG)pEm->Unk154 < 900 && Dy < 900 && Dy > -1800) {
        if (!EF(pEm)->Obj_cnt && pEm->Spd_x > 230 && !(G.Unk75EA & 1)) {
            if (!Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x100))
                func_80104648(pEm, 12);
        }
        if (EF(pEm)->Obj_cnt > 10) {
            func_80104648(pEm, 8);
            EF(pEm)->Idly_cnt = Rnd_l() & 0x7F;
            EF(pEm)->Idly_flg = Rnd_area();
        }
        if (pEm->Spd_x > 100)
            pEm->Spd_x -= 4;
        EF(pEm)->Obj_cnt++;
    } else {
        EF(pEm)->Obj_cnt = 0;
    }
}

/* RE2 Em21_run_away_ck */
void func_80104A08(ENEMY_WORK *pEm)
{
    if (EF(pEm)->Flg & 0x10) {
        func_80104D50(pEm, 350);
        pEm->Be_flg &= ~0x800;
        EF(pEm)->Idly_cnt = (Rnd_l() & 0x7F) + 30;
        EF(pEm)->Idly_flg = Rnd_area();
        EF(pEm)->Na_cnt = 60;
        func_80104648(pEm, 4);
        EF(pEm)->Flg &= ~0x58;
    }
}

/* RE2 Em21_pl_dead_ck */
void func_80104A90(ENEMY_WORK *pEm)
{
    if ((LONG)pEm->Unk154 < 1800)
        func_80104648(pEm, 7);
}

/* RE2 Em21_lock_off_ck */
void func_80104AC4(ENEMY_WORK *pEm)
{
    ULONG D0;

    if (EF(pEm)->Flg & 4) {
        D0 = pEm->Routine_1;
        if (D0 < 11 || D0 > 14) {
            EF(pEm)->Atck = 0;
            EF(pEm)->Unk0B = 0;
            G.Unk75EA &= ~1;
            EF(pEm)->Flg &= ~4;
        }
        if ((LONG)pEm->Unk154 > 14400) {
            EF(pEm)->Atck = 0;
            EF(pEm)->Unk0B = 0;
            G.Unk75EA &= ~1;
            EF(pEm)->Flg &= ~4;
        }
    }
    if (EF(pEm)->Flg & 0x80) {
        EF(pEm)->Atck = 0;
        EF(pEm)->Unk0B = 0;
        EF(pEm)->Pl_dead_cnt = 120;
        EF(pEm)->Flg &= ~0x80;
    }
}

/* RE2 Em21_set_flg_all */
void func_80104B90(ULONG Mask)
{
    ENEMY_WORK *pEm;
    ENEMY_WORK **ppEm;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        if (pEm->Id == 0x21 && !(EF(pEm)->Flg & 4)) {
            EF(pEm)->Flg |= Mask;
            EF(pEm)->Idly_cnt = 0;
            EF(pEm)->Idly_flg = 0;
        }
    }
}

/* RE2 Em_21_blood_call */
void func_80104C00(ENEMY_WORK *pEm)
{
    SVECTOR Svec;

    Svec.vx = pEm->Pos_x + (signed char)Rnd_l();
    Svec.vy = pEm->Pos_y + (signed char)Rnd_l();
    Svec.vz = pEm->Pos_z + (signed char)Rnd_l();
    Esp_call(0x100, (USHORT)(Rnd_l() << 4) | 0x28000000, &GsIDMATRIX, &Svec, NULL);
}

/* RE2 Em_21_feather_call */
void func_80104C94(ENEMY_WORK *pEm, LONG Num)
{
    SVECTOR Svec;

    while (Num-- != 0) {
        Svec.vx = pEm->Pos_x + ((signed char)Rnd_l() << 2);
        Svec.vy = pEm->Pos_y + ((signed char)Rnd_l() << 2);
        Svec.vz = pEm->Pos_z + ((signed char)Rnd_l() << 2);
        Esp_call(0xF, (USHORT)(Rnd_l() << 4) | 0x8000000, &GsIDMATRIX, &Svec, NULL);
    }
}

/* RE2 Em21_atd_set */
void func_80104D50(ENEMY_WORK *pEm, LONG Size)
{
    pEm->Col.W0 = Size;
    pEm->Col.D0 = Size;
}

/* RE3: player hit while crows were pecking it, scatter the attackers */
void func_80104D5C(void)
{
    ENEMY_WORK *pEm;
    ENEMY_WORK **ppEm;
    ULONG D0;

    if (!G.Player.Damage_cnt)
        return;
    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        D0 = pEm->Routine_1;
        if (pEm->Id == 0x21 && (pEm->Be_flg & 1) && (D0 < 4 || D0 > 6)) {
            if (EF(pEm)->Flg & 4) {
                EF(pEm)->Idly_cnt = (Rnd_l() & 0x7F) + 30;
                EF(pEm)->Idly_flg = Rnd_area();
                EF(pEm)->Na_cnt = 10;
                pEm->Be_flg &= ~0x800;
                func_80104648(pEm, 4);
            } else {
                EF(pEm)->Flg |= 0x10;
                EF(pEm)->Idly_cnt = 0;
                EF(pEm)->Idly_flg = 0;
            }
        }
    }
}

/* RE2 PLDAMEGE_EM (Pldamege_Em121/221), runs on the player work */
void func_80104E60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pT = pEm->pEnemy_ptr;

    if (pT->Routine_0 == 2 || pT->Routine_0 == 3 || pT->Routine_0 == 7) {
        func_80104D5C();
        EM_RTN(pEm) = 1;
        G.Player.Damage_cnt &= 0x7F;
        pEm->Unk120 &= 0xED;
        pT->Be_flg &= ~0x800;
        func_80104B90(0x10);
        pEm->Be_flg &= ~0x8800;
        pT->Be_flg &= ~0x8800;
        return;
    }
    Pldamage_Em21_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em21_pldm00 */
void func_80104F48(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70000;
        pEm->Routine_2 = 1;
        pEm->Unk12C = 0;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_MOVE(pEm) = 0x70001;
            pEm->Routine_2 = 2;
        }
        Add_speedXZ_n(pEm, 0, 0);
        break;
    case 2:
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200))
            pEm->UnkCA = 7;
        Add_speedXZ_n(pEm, 0, 0);
        break;
    case 3:
        if (G.Player.Life < 0) {
            G.Player.Routine_0 = 3;
            G.Player.Routine_1 = 0;
            G.Player.Routine_2 = 0;
            G.Player.Routine_3 = 0;
            break;
        }
        EM_MOVE(pEm) = 0x70002;
        pEm->Routine_2 = 4;
    case 4:
        Spd_get(pEm, pKan, pSeq, 0);
        if (!pEm->Move_cnt) {
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        if (Joint_move(pEm, pKan, pSeq, 0x200)) {
            EM_MOVE(pEm) = 0x70001;
            func_80104638(pEm, 1, 0);
        }
        Add_speedXZ_n(pEm, 0, 0);
        break;
    case 5:
        break;
    }
}

/* RE2 Em21_dummy_br */
void func_80105100(ENEMY_WORK *pEm)
{
}

/* RE2 Em21_dummy */
void func_80105108(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}
