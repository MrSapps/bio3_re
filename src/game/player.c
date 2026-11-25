#include "mojidisp.h"
#include "pad.h"
#include "player.h"
#include "pl_wep.h"
#include "joint.h"
#include "sca.h"
#include "sld.h"
#include "snd.h"
#include "sub.h"

static __inline__ UCHAR *Msg_get_80031BDC(ULONG No)
{
    return Mess_sys_text + Mess_sys_ofs[No];
}

INCLUDE_ASM("asm/nonmatchings/game/player", Player_set);

void Player_move(ENEMY_WORK *pEm)
{
    SVECTOR sv;
    SVECTOR sv2;
    PARTS_W *pParts;

    if ((LONG)G.Unk2468 < 0) {
        return;
    }
    G.Status_flg &= ~0x40;
    G.Unk75E8 &= 0xFFE0;
    if (pEm->Damage_cnt & 0x7F) {
        pEm->Damage_cnt--;
        pEm->Be_flg |= 0x10000000;
    }
    if (pEm->Damage_cnt & 0x80) {
        pEm->Unk132 &= 0x7FFF;
    }
    Pl_routine[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Mag_down(pEm);
    func_80045950(pEm);
    Pl_wep_recoil_6(pEm);
    Pl_wep_recoil_10(pEm);
    Pl_wep_spin_0b(pEm);
    if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 500) {
        pParts = pEm->pSin_parts_ptr;
        sv.vx = pParts->Workm.t[0];
        sv.vy = pEm->Water;
        sv.vz = pParts->Workm.t[2];
        sv2.vx = 0x400;
        sv2.vy = 0;
        sv2.vz = 0;
        if ((pEm->Move_cnt & 3) == 2 && (USHORT)(pEm->Spd_x + 20) > 40) {
            Esp_call(0x20E, ((Rnd() * 8 + 0x2000) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, &sv, &sv2);
        }
    }
    Pl_neck(pEm, 7000, 1500);
    Rot_neck(pEm, pEm->Cdir_y);
}

void Pl_init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pNeck;
    LONG half_x;
    LONG half_z;
    LONG w;

    half_x = 0x1C2;
    half_z = 0x226;
    w = 0x190;
    if (pEm->Id >= 8) {
        half_x = 0x1F4;
        half_z = 0x258;
        w = 0x1C2;
    }
    *(ULONG *)&pEm->Routine_0 = 1;
    pEm->Unk134 = 0x5DC;
    pEm->Unk136 = -1000;
    pEm->Unk138 = 0x55;
    pEm->Damage_cnt = 0;
    pEm->pEnemy_ptr = pEm;
    pEm->pUnk114 = pEm;
    pEm->Unk139 = 0xAA;
    pEm->Unk13A = w;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    pEm->Neck_no = 1;
    pEm->Unk122 = -(pEm->nFloor * 1800);
    pEm->Status_flg |= 0x400;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    pNeck = &pEm->pSin_parts_ptr[1];
    PW_NECK(pNeck)->Add_dir_y = 0x60;
    PW_NECK(pNeck)->Add_dir_z = 0x40;
    PW_NECK(pNeck)->L_rot_y = 0x200;
    PW_NECK(pNeck)->L_rot_z = 0x138;
    PW_NECK(pNeck)->N_rot_y = 0;
    PW_NECK(pNeck)->N_rot_z = 0;
    if (!(G.Kage9[0].Be_flg & 1)) {
        Kage_work9_set(&pEm->Kage.pKw9, 0, half_x | (half_z << 16), 0, (VECTOR *)&pEm->Pos_x);
    }
    *(ULONG *)&pEm->Move_no = 2;
    Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
}

void func_80038F98(ENEMY_WORK *pEm)
{
    if (!(G.Kage9[0].Be_flg & 1)) {
        Kage_work9_set(&pEm->Kage.pKw9, 0, 0x022601C2, 0, (VECTOR *)&pEm->Pos_x);
    }
    pEm->Water = 0;
    pEm->Unk132 = 0;
    pEm->Unk144 = 0;
    pEm->Unk150 = 0;
    pEm->Unk152 = 0;
    pEm->Unk1B8 = 0;
    pEm->Unk1BA = 0;
    pEm->Unk1BC[0] = 0;
    pEm->Unk1BC[1] = 0;
    pEm->Unk1C0 = 0;
    pEm->Unk1C1 = 0;
    pEm->Unk1C2 = 0;
    pEm->Unk1CA = 0;
    pEm->Unk1C8 = 0;
}

void Pl_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pl_mv_pad[pEm->Routine_1](pEm, G.Key, G.Key_trg);
    Pl_mv[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if ((pEm->Status_flg & 0x200) && !(G.Unk2468 & 0x1000000)) {
        if (--G.Pl_poison_timer == -1) {
            G.Pl_poison_timer = 30;
            if (pEm->Life >= 2) {
                pEm->Life--;
            }
        }
    }
}

void pl_mv00_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (Key & 0xA) {
        *(ULONG *)&pEm->Routine_0 = 0x401;
    }
    if (Key & 4) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
        if (Key_trg & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x601;
        }
    }
    if (Key & 1) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        if (Key & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void pl_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C];
        pEm->Routine_2 = 1;
        n += 0x70000;
        *(ULONG *)&pEm->Move_no = n;
        G.Status_flg &= ~0x100;
        pEm->Timer0 = 0xF0;
    case 1:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (D_8009CD3C != 0) {
            break;
        }
        if ((UCHAR)pEm->Type == 10) {
            break;
        }
        if ((UCHAR)pEm->Type == 11) {
            break;
        }
        if (--pEm->Timer0 == -1) {
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        *(ULONG *)&pEm->Move_no = 0x7000B;
        pEm->Routine_2 = 3;
        pEm->Timer0 = ((Rnd() & 7) + 8) * 30;
        if ((Rnd() & 0x1F) == 2 && D_800CCBBF == 0) {
            pEm->Timer0 |= 0x8000;
        }
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(ULONG *)&pEm->Move_no = 0x7000C;
            pEm->Routine_2 = 3;
        }
        if (D_8009CD3C != 0) {
            break;
        }
        if (pEm->Timer0 & 0x8000) {
            if (!(pEm->Timer0-- & 0x7FFF)) {
                pEm->Routine_2 = 7;
            }
        }
        break;
    case 4:
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    case 5:
        *(ULONG *)&pEm->Move_no = 0x7000B;
        pEm->Routine_2 = 6;
    case 6:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            pEm->Routine_2 = 0;
        }
        break;
    case 7:
        *(ULONG *)&pEm->Move_no = 0xF0015;
        pEm->Routine_2 = 8;
        pEm->Unk120 |= 0x12;
    case 8:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 0;
            pEm->Unk120 &= 0xED;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void pl_mv01_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 1)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
            if (Key_trg & 0x200) {
                *(ULONG *)&pEm->Routine_0 = 0x601;
            }
        }
        return;
    }
    if (pEm->Col.Hit_flg & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xA01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20A01;
        }
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDEC[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDEC[D_8009CD3C];
    }
    if (Key & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if (D_800D1F2C & 0x10) {
        *(ULONG *)&pEm->Routine_0 = 0x901;
    }
}

void pl_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    SHORT spd_x;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 3];
        pEm->Routine_2 = 1;
        n += 0x70000;
        *(ULONG *)&pEm->Move_no = n;
        G.Status_flg &= ~0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(0, m * 3 + 11, pEm);
            G.Unk75E8 |= 2;
        }
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Water < pEm->Unk122 - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

void pl_mv02_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if ((Key & 0x201) != 0x201) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        }
        return;
    }
    if (pEm->Col.Hit_flg & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xF01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20F01;
        }
    }
    if (pEm->Spd_x >= 0x33) {
        if (Key & 2) {
            pEm->Cdir_y += D_8009CDF0[D_8009CD3C];
        }
        if (Key & 8) {
            pEm->Cdir_y -= D_8009CDF0[D_8009CD3C];
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x701;
        }
    }
    if (D_800D1F2C & 0x10) {
        *(ULONG *)&pEm->Routine_0 = 0x901;
    }
}

void pl_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    SHORT spd_x;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 6];
        pEm->Routine_2 = 1;
        n += 0x70000;
        *(ULONG *)&pEm->Move_no = n;
        G.Status_flg &= ~0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Water < pEm->Unk122 - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

void pl_mv03_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 4)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        }
        return;
    }
    if (pEm->Col.Hit_flg & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xE01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20E01;
        }
    }
    if (Key_trg & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x601;
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDF4[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDF4[D_8009CD3C];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void pl_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[3] = {9, 10, 10};
    LONG Dummy[2];
    USHORT Flg;
    ULONG n;
    ULONG m;
    UCHAR u;
    SHORT spd_x;

    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70009;
        if (D_8009CD3C != 0) {
            *(ULONG *)&pEm->Move_no = 0x7000A;
        }
        pEm->Routine_2 = 1;
        G.Status_flg &= ~0x100;
        pEm->UnkE2 = 9;
        pEm->UnkE3 = 0;
    case 1:
        if (*(signed char *)&pEm->UnkE3 == 0 && D_8009CD3C == 0) {
            if (Lim_dir_ck(pEm, 5000, 800)) {
                if (pEm->UnkE2 == 9) {
                    u = pEm->Move_cnt;
                    pEm->Move_no = 0;
                    pEm->UnkE2 = 0;
                    pEm->Hokan_flg = 7;
                    pEm->Move_cnt = 0;
                    Spd_get(pEm, pKan, pSeq, 0);
                    pEm->Move_cnt = u;
                    Spd_get(pEm, pKan, pSeq, 0);
                }
            } else if (pEm->UnkE2 != 9) {
                pEm->Move_no = 9;
                pEm->UnkE2 = 9;
                pEm->Move_cnt = pEm->Move_cnt / 7;
                u = pEm->Move_cnt * 7;
                pEm->Hokan_flg = 7;
                pEm->Move_cnt = 0;
                Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
                pEm->Move_cnt = u;
                Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
            }
            pEm->UnkE3 = 15;
        }
        pEm->UnkE3--;
        if (pEm->Move_no != 0) {
            Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
            Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        } else {
            Spd_get(pEm, pKan, pSeq, 0);
            Joint_move2(pEm, pKan, pSeq, 0x200);
        }
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(0, m * 3 + 11, pEm);
            G.Unk75E8 |= 2;
        }
        break;
    }
    if (pEm->Water < pEm->Unk122 - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck"); /* returns long here (canonical UC) */

ULONG Lim_dir_ck(ENEMY_WORK *pEm, ULONG Lim, SHORT Dir)
{
    ENEMY_WORK *pT;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pNear;
    tagGLOBAL *pG;
    LONG dx;
    LONG dz;
    ULONG dist;
    ULONG min;

    pNear = NULL;
    pG = &G;
    if (pG->Em_num != 0) {
        min = Lim;
        ppEm = &pG->pEnemy[2];
        do {
            pT = *ppEm;
            if ((pT->Be_flg & 1) && (pT->Type & 0xE000) == 0) {
                dx = pEm->Pos_x - pT->Pos_x;
                dz = pEm->Pos_z - pT->Pos_z;
                dist = SquareRoot0(dx * dx + dz * dz);
                if (pT->Life >= 0
                    && Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t,
                                   (VECTOR *)pT->pSin_parts_ptr[pT->Neck_no].Workm.t, 0x2040, 1) == 0
                    && Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, Dir) == 0
                    && Cdir_ck_l(pEm, pT) == 0 && dist < min) {
                    min = dist;
                    pNear = pT;
                    ppEm++;
                    continue;
                }
            }
            ppEm++;
        } while (ppEm != pG->ppEnemy_end);
        if (pNear != NULL) {
            goto found;
        }
    }
    pEm->pEnemy_ptr = pEm;
    return 0;
found:
    pEm->pEnemy_ptr = pNear;
    return 1;
}

void pl_mv04_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 0xA)) {
        *(ULONG *)&pEm->Routine_0 = 1;
    }
    if (Key & 4) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    }
    if (Key & 1) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        if (Key & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        }
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDF8[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDF8[D_8009CD3C];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void pl_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 3];
        pEm->Routine_2 = 1;
        n += 0x70000;
        *(ULONG *)&pEm->Move_no = n;
        G.Status_flg &= ~0x100;
        pEm->Spd_x = 0x15;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(0, m * 3 + 11, pEm);
            G.Unk75E8 |= 2;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
}

void pl_mv06_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (G.Unk2468 & 0x1000000) {
        *(ULONG *)&pEm->Routine_0 = 1; /* Routine_0 = 1, Routine_1..3 = 0 */
    }
}

void func_8003A688(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 6];
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 8;
        pEm->Spd_x = 0x32;
        pEm->Spd_z = 0;
        n += 0xF0000;
        *(ULONG *)&pEm->Move_no = n;
        Get_speedXZ(pEm, 0x700, &pEm->Spd_x, &pEm->Spd_z);
        G.Status_flg &= ~0x100;
        G.Unk75E8 |= 4;
        Snd_se_walk(0, 11, pEm);
    case 1:
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x100);
        pEm->Cdir_y += 0x100;
        if (--*((signed char *)pEm + 0xE2) == 0) {
            *(ULONG *)&pEm->Routine_0 = 1;
            G.Unk75E8 |= 4;
            Snd_se_walk(0, 11, pEm);
        }
        pEm->Pos_x += pEm->Spd_x;
        pEm->Pos_z += pEm->Spd_z;
        break;
    }
}

void pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003A7D8);

void pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

extern UCHAR D_800E01C4_ __asm__("D_800E01C4"); /* type differs from the canonical extern */

void func_8003B080(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70005;
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
        G.Status_flg |= 0x140;
    case 1:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        if (!(G.System_flg & 0x40)) {
            D_800E01C4_ = 1;
            Game.Unk1 = 1;
            G.Status_flg |= 0x200;
            Game.Stop_bak = G.Unk2468;
        }
        pEm->Routine_2 = 3;
        break;
    case 4:
        *(ULONG *)&pEm->Move_no = 0x70005;
        pEm->Routine_2 = 5;
    case 5:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x10200);
        break;
    case 6:
        *(ULONG *)&pEm->Routine_0 = 1;
        G.Status_flg &= ~0x40;
        G.Status_flg &= ~0x100;
        break;
    }
}

void pl_mv0a_pad(ENEMY_WORK *pEm, ULONG Flg)
{
    if ((Flg & 0x10) && (D_800D1F2C & 0x10)) {
        return;
    }
    if (pEm->Routine_2 == 5) {
        pEm->Routine_2 = 6;
        pEm->Move_no = 6;
        pEm->Move_cnt = 0;
        pEm->Hokan_flg = 7;
        Snd_se_stad(0x10200, &pEm->Pos_x, 0, 0);
        Pad_vib_init();
    }
}

void pl_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    USHORT dir;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Move_no = D_8009CDE0[D_8009CD3C + 3] + 0x70000;
        G.Status_flg |= 0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        dir = pEm->Cdir_y;
        if (dir & 0x200) {
            pEm->Cdir_y = dir + (UCHAR)(dir >> 2);
        } else {
            pEm->Cdir_y = dir - (UCHAR)(dir >> 2);
        }
        if (!(pEm->Cdir_y & 0x3E0)) {
            pEm->Cdir_y &= 0xFF00;
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        pEm->Routine_2 = 3;
        *(ULONG *)&pEm->Move_no = 0x70006;
    case 3:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 4:
        pEm->Routine_2 = 5;
        *(ULONG *)&pEm->Move_no = 0x70007;
    case 5:
        if (pEm->Move_cnt == 1) {
            Snd_se_stad(0x1022C, &pEm->Pos_x, 0, 0);
            Pad_vib_set0(2, 0);
            Pad_vib_set1(3, 0x96, 0);
            Pad_vib_fade_set(0x14, 0x96, 0, 3);
        }
        Joint_move2(pEm, pKan, pSeq, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            if (pEm->Id != 15) {
                Foot_set_pl(pEm, 1, 0);
            } else {
                Foot_set_pl(pEm, 1, 1);
            }
        }
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        break;
    case 6:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x10200);
        break;
    case 7:
        G.Status_flg &= ~0x100;
        *(ULONG *)&pEm->Routine_0 = 1;
        break;
    }
}

void func_8003B4FC(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (!(Key & 1)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0xE01;
            if (Key_trg & 0x200) {
                *(ULONG *)&pEm->Routine_0 = 0x601;
            }
        }
        return;
    }
    if (!(pEm->Col.Hit_flg & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDEC[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDEC[D_8009CD3C];
    }
    if (!(func_8003C390(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if (Key & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void func_8003B784(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(func_8003C390(pEm, 1) & 0x1000)) {
            goto case2;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0);
    case 1:
        Spd_get(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk194_kan_t_ptr, G.Player.pUnk190_seq_t_ptr, 0x200);
        Leg_ik_set(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        *(ULONG *)&pEm->Move_no = D_8009CDE0[D_8009CD3C + 3] + 0x70000;
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, func_8003C390(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_8003C390(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void func_8003B9FC(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 4)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0xA01;
        }
        return;
    }
    if (!(func_8003C390(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if (!(pEm->Col.Hit_flg & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    }
    if (Key_trg & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x601;
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDF4[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDF4[D_8009CD3C];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void func_8003BCA4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(func_8003C390(pEm, 1) & 0x1000)) {
            goto case2;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 1);
    case 1:
        Spd_get(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 1);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n == 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0x10200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk194_kan_t_ptr, G.Player.pUnk190_seq_t_ptr, 0x10200);
        Leg_ik_set(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        *(ULONG *)&pEm->Move_no = 0x70009;
        if (D_8009CD3C != 0) {
            *(ULONG *)&pEm->Move_no = 0x7000A;
        }
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, func_8003C390(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_8003C390(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void func_8003BF28(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if ((Key & 0x201) != 0x201) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        }
        return;
    }
    if (!(pEm->Col.Hit_flg & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if (pEm->Spd_x >= 0x33) {
        if (Key & 2) {
            pEm->Cdir_y += D_8009CDF0[D_8009CD3C];
        }
        if (Key & 8) {
            pEm->Cdir_y -= D_8009CDF0[D_8009CD3C];
        }
    }
    if (!(func_8003C5B0(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void func_8003C104(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(func_8003C5B0(pEm, 1) & 0x1000)) {
            goto case2;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0);
    case 1:
        Spd_get(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x = 30;
        }
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk194_kan_t_ptr, G.Player.pUnk190_seq_t_ptr, 0x200);
        Leg_ik_set(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        *(ULONG *)&pEm->Move_no = D_8009CDE0[D_8009CD3C + 6] + 0x70000;
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, func_8003C5B0(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_8003C5B0(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Flg)
{
    USHORT dir;
    ULONG d;
    ULONG mot;
    USHORT r;

    dir = D_8009CDFC[(UCHAR)pEm->Col.Unk39 & 7];
    d = (dir - (USHORT)pEm->Cdir_y) & 0xFFF;
    r = dir;
    if (d - 0x400 > 0x800) {
        if (d - 0x258 > 0xB50) {
            if (Flg) {
                pEm->Move_cnt = pEm->Move_cnt * 34;
                pEm->Move_cnt = pEm->Move_cnt / 30;
                if (pEm->Move_cnt >= 3) {
                    pEm->Move_cnt -= 3;
                } else {
                    pEm->Move_cnt += 31;
                }
                mot = 0xC;
                if (D_8009CD3C == 0) {
                    mot = (D_8009CF78[(UCHAR)pEm->Type] & 3) << 2;
                }
                *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
                *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70000;
            }
            return (SHORT)r + 0x1000;
        }
        return (SHORT)dir;
    }
    if (d - 0x5A9 < 0x4AF) {
        if (Flg) {
            pEm->Move_cnt = pEm->Move_cnt * 34;
            pEm->Move_cnt = pEm->Move_cnt / 30;
            if (pEm->Move_cnt >= 4) {
                pEm->Move_cnt -= 2;
            } else {
                pEm->Move_cnt += 30;
            }
            mot = 0xD;
            if (D_8009CD3C == 0) {
                mot = ((D_8009CF78[(UCHAR)pEm->Type] & 3) << 2) | 1;
            }
            *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
            *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70001;
        }
        return (((SHORT)r + 0x800) & 0xFFF) | 0x1000;
    }
    return ((SHORT)dir + 0x800) & 0xFFF;
}

LONG func_8003C5B0(ENEMY_WORK *pEm, ULONG Flg)
{
    USHORT dir;
    ULONG d;
    ULONG mot;
    USHORT r;

    dir = D_8009CDFC[(UCHAR)pEm->Col.Unk39 & 7];
    d = (dir - (USHORT)pEm->Cdir_y) & 0xFFF;
    r = dir;
    if (d - 0x400 > 0x800) {
        if (d - 0x258 > 0xB50) {
            if (Flg) {
                mot = 0xE;
                if (D_8009CD3C == 0) {
                    mot = ((D_8009CF78[(UCHAR)pEm->Type] & 3) << 2) | 2;
                }
                *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
                *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70002;
            }
            return (SHORT)r + 0x1000;
        }
        return (SHORT)dir;
    }
    if (d - 0x5A9 < 0x4AF) {
        if (Flg) {
            mot = 0xF;
            if (D_8009CD3C == 0) {
                mot = ((D_8009CF78[(UCHAR)pEm->Type] & 3) << 2) | 3;
            }
            *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
            *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70003;
        }
        return (((SHORT)r + 0x800) & 0xFFF) | 0x1000;
    }
    return ((SHORT)dir + 0x800) & 0xFFF;
}

void pl_mv0b_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

void func_8003C740(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    USHORT dir;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Move_no = 0;
        if (pEm->Routine_2 & 2) {
            pEm->Move_no = 0;
        }
        pEm->Hokan_flg = 7;
        pEm->Move_cnt = 0;
        *(ULONG *)&pEm->Move_no = 0x70000;
        G.Status_flg |= 0x100;
        pEm->Be_flg |= 0x8000;
    case 1:
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        func_80034124(&D_800CCC44);
        dir = pEm->Cdir_y;
        if (dir & 0x200) {
            pEm->Cdir_y = dir + (UCHAR)(dir >> 2);
        } else {
            pEm->Cdir_y = dir - (UCHAR)(dir >> 2);
        }
        if (!(pEm->Cdir_y & 0x3E0)) {
            pEm->Cdir_y &= 0xFF00;
            pEm->Routine_3 = 2;
            if (pEm->Routine_2 & 2) {
                pEm->Routine_1 = 0xC;
                pEm->Routine_3 = 0;
            }
        }
        break;
    case 2:
        pEm->Routine_3 = 3;
        *(ULONG *)&pEm->Move_no = 0x30000;
    case 3:
        if (pEm->Move_cnt == 0x11) {
            pEm->Damage_cnt |= 0x80;
            pEm->nFloor++;
            pEm->Unk1CA = pEm->Unk1C8;
            if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 500) {
                Pl_water(pEm);
            }
        }
        if (pEm->Move_cnt == 0x31) {
            pEm->Damage_cnt &= 0x7F;
            pEm->Be_flg &= ~0x8000;
        }
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (pEm->Move_cnt == 0x1D) {
            pEm->Pos_y -= 0x708;
            pEm->Unk122 -= 0x708;
            if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 500) {
                Pl_water(pEm);
            }
        }
        pEm->Routine_3 += Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Add_speedXZ(pEm, 0);
        func_80034124(&D_800CCC44);
        break;
    case 4:
        pEm->Routine_1 = 0;
        pEm->Routine_2 = 0;
        pEm->Routine_3 = 0;
        G.Status_flg &= ~0x100;
        pEm->Col.Hit_flg &= 0xFBFF;
        break;
    }
}

void pl_mv0c_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

void func_8003CA88(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70001;
        pEm->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0xA000;
    case 1:
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        pEm->Routine_3 += Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        Add_speedXZ(pEm, 0);
        func_80034124(&D_800CCC44);
        if (pEm->Move_cnt == 0xE) {
            pEm->Routine_3 = 2;
        }
        break;
    case 2:
        pEm->Routine_3 = 3;
        *(ULONG *)&pEm->Move_no = 3;
        pEm->Spd_x = 0x96;
        pEm->Timer0 = 0;
        pEm->Pos_y += 0x226;
        pEm->Timer0 = 5;
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        pEm->Unk1CA = 0;
        pEm->nFloor--;
        func_80034124(&D_800CCC44);
        pEm->UnkE3 = 0;
        pEm->Be_flg &= ~0x2000;
    case 3:
        pEm->Pos_y += pEm->Timer0++ * 30;
        pEm->Unk122 = func_8004D720(&pEm->Pos_x, 400, 0x8000, 0);
        pEm->nFloor = pEm->Pos_y / -1800;
        Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Unk75E8 |= 4;
        }
        if (pEm->Unk122 - 300 < pEm->Pos_y) {
            pEm->Routine_3 = 4;
            Pad_vib_set0(1, 0);
            Pad_vib_set1(6, 0x96, 0);
        }
        if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 500) {
            Pl_water(pEm);
        }
        break;
    case 4:
        pEm->Routine_3 = 5;
        pEm->Move_no = 2;
        if ((pEm->Routine_2 & 0xC) >= 5) {
            pEm->Move_no = 1;
        }
        pEm->Move_cnt = 4;
        pEm->Hokan_flg = 7;
        pEm->Pos_y = pEm->Unk122;
        pEm->Damage_cnt &= 0x7F;
        pEm->Be_flg &= ~0x8000;
        Snd_se_walk(1, 11, pEm);
        G.Unk75E8 |= 4;
    case 5:
        if (pEm->Move_cnt == 6) {
            Snd_se_walk(1, 14, pEm);
        }
        pEm->nFloor = pEm->Pos_y / -1800;
        pEm->Routine_3 += Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        func_80034124(&D_800CCC44);
        break;
    case 6:
        G.Status_flg &= ~0x100;
        pEm->Routine_1 = 0;
        pEm->Routine_2 = 0;
        pEm->Routine_3 = 0;
        pEm->Col.Hit_flg &= 0xFBFF;
        break;
    }
}

void pl_mv0d_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

void func_8003CEA0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    HIT_REQ Req;
    ENEMY_WORK *pT;
    LONG hit;
    LONG *p;
    ULONG no;

    pT = pEm->pEnemy_ptr;
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x30012;
        pEm->Routine_2 = 1;
        Snd_se_stad(0x1000B, &pEm->Pos_x, 0, 0);
        G.Status_flg |= 0x100;
        G.Unk75E8 |= 0x1C;
        G.Unk785E++;
        Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x100);
        pEm->UnkE3 = 0;
        pEm->Unk120 |= 0x12;
        if (!(G.Key & 0x500)) {
            pEm->UnkE3 = 1;
        }
        if (G.System_flg & 0x80) {
            Quake_add(2, 1);
        }
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            if (G.System_flg & 0x80) {
                Quake_add(2, 1);
            }
            *(ULONG *)&pEm->Routine_0 = 0x60501;
            p = &pT->Pos_x;
            pEm->pUnk16C = p;
            pEm->pUnk170 = p;
            pEm->pUnk15C = p;
            if ((signed char)pEm->UnkE3 != 0 && !(G.Key & 0x500)) {
                *(ULONG *)&pEm->Routine_0 = 1;
            }
            if ((UCHAR)pEm->Type == 0) {
                *(ULONG *)&pEm->Routine_0 = 1;
            }
            return;
        }
        pEm->Damage_cnt = 0;
        if (*pEm->pNow_seq & 0x400) {
            pEm->Damage_cnt = 0x80;
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x100);
        }
        hit = 0;
        if (*pEm->pNow_seq & 0x200) {
            Req.pPos = (VECTOR *)pEm->pSin_parts_ptr->Workm.t;
            Req.W = 1000;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEm->Cdir_y;
            hit = func_800472EC(&Req, &D_8009CE0C);
            pEm->Damage_cnt |= 0x80;
        }
        if (hit) {
            no = 3;
            if ((ULONG)(Snd_edt[3] - 1) < 30) {
                no = 0x13;
            }
            Snd_se_stad(no | 0x10300, &pEm->Pos_x, 0, 0);
        }
    default:
        Add_speedXZ(pEm, 0);
    }
}

void Pl_damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_dm[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

void pl_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x30004;
        pEm->Routine_2 = 1;
        Snd_se_stad(0x10000, &pEm->Pos_x, 0, 0);
        pEm->Damage_cnt = 0x80;
        Pad_vib_set0(5, 0);
        Pad_vib_set1(8, 0x96, 0);
        pEm->Be_flg &= ~0x8000;
        pEm->Unk120 |= 0x12;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            G.Status_flg &= ~0x100;
            pEm->Unk120 &= 0xED;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void pl_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Move_no = (pEm->Routine_1 & 1) + 0x30002;
        Snd_se_stad(((pEm->Routine_1 & 1) + 1) | 0x10000, &pEm->Pos_x, 0, 0);
        pEm->Damage_cnt = 0x80;
        Pad_vib_set0(5, 0);
        Pad_vib_set1(8, 200, 0);
        pEm->Be_flg &= ~0x8000;
        *(LONG *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 400 - 200;
        if (pEm->Routine_1 >= 8) {
            *(LONG *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 600 - 300;
        }
        pEm->Spd_z = 0;
        pEm->Unk120 |= 0x12;
    case 1:
        if (pEm->Hokan_flg != 0) {
            Add_speedXZ(pEm, 0);
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            G.Status_flg &= ~0x100;
            pEm->Unk120 &= 0xED;
        }
        break;
    }
    Flg = *pEm->pNow_seq;
    if (Flg & 0x200) {
        n = Flg & 0x400;
        m = n != 0;
        Snd_se_walk(1, m * 14, pEm);
        G.Unk75E8 |= 4;
    }
}

void pl_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x3000A;
        pEm->Routine_2 = 1;
        pEm->Spd_x = -800;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10001, &pEm->Pos_x, 0, 0);
        pEm->Be_flg &= ~0x8000;
        if (pEm->Unk132 & 0x500) {
            PARTS_W *pParts;
            PARTS_W *pParts1;
            PARTS_W *pParts2;
            pParts = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x800) << 16, &pParts[2].Workm, 0, 0);
            pParts1 = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x1000) << 16, &pParts1[0].Workm, 0, 0);
            pParts2 = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x1000) << 16, &pParts2[13].Workm, 0, 0);
        }
    case 1:
        if (pEm->Hokan_flg == 0) {
            *(ULONG *)&pEm->Move_no = 0x3000C;
            pEm->Routine_2 = 2;
        }
        Add_speedXZ(pEm, 0);
        Joint_move2(pEm, pKan, pSeq, 0x400);
        pEm->Spd_x += 0x96;
        break;
    case 2:
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= 0xED;
        }
        pEm->Spd_x += 10;
        if (pEm->Spd_x > 0) {
            pEm->Spd_x = 0;
        }
        break;
    case 3:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Move_no = 0x30009;
            pEm->Routine_2 = 4;
            pEm->Damage_cnt = 0;
        }
        break;
    case 4:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 5;
            *(ULONG *)&pEm->Move_no = 0x30005;
        }
        break;
    case 5:
        if (Joint_move2(pEm, pKan, pSeq, 0x10400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= 0xED;
        }
        break;
    }
}

void pl_dm03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x3000B;
        pEm->Routine_2 = 1;
        pEm->Spd_x = 0x190;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_stad(0x10002, &pEm->Pos_x, 0, 0);
        pEm->Be_flg &= ~0x8000;
        if (pEm->Unk132 & 0x500) {
            PARTS_W *pParts;
            PARTS_W *pParts1;
            PARTS_W *pParts2;
            pParts = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x800) << 16, &pParts[2].Workm, 0, 0);
            pParts1 = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x1000) << 16, &pParts1[0].Workm, 0, 0);
            pParts2 = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x1000) << 16, &pParts2[13].Workm, 0, 0);
        }
    case 1:
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Move_no = 0x30009;
            pEm->Routine_2 = 2;
            pEm->Damage_cnt = 0;
        }
        if (pEm->Hokan_flg != 0) {
            pEm->Spd_x -= 0x14;
        } else {
            pEm->Spd_x -= 0x32;
        }
        if (pEm->Spd_x < 0) {
            pEm->Spd_x = 0;
        }
        break;
    case 2:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 3;
            *(ULONG *)&pEm->Move_no = 0x30005;
        }
        break;
    case 3:
        if (Joint_move2(pEm, pKan, pSeq, 0x10400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= 0xED;
        }
        break;
    }
}

void Pl_die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_die_t[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

void pl_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70001;
        pEm->Routine_2 = 1;
        *(LONG *)&pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        Snd_se_stad(0x10003, &pEm->Pos_x, 0, 0);
        pEm->Unk120 = 1;
        Pad_vib_set0(3, 0);
        Pad_vib_set1(8, 200, 0);
        Pad_vib_fade_set(0x5A, 200, 0, 8);
        if (pEm->Unk132 & 0x500) {
            PARTS_W *pParts;
            PARTS_W *pParts1;
            PARTS_W *pParts2;
            pParts = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x800) << 16, &pParts[2].Workm, 0, 0);
            pParts1 = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x1000) << 16, &pParts1[0].Workm, 0, 0);
            pParts2 = pEm->pSin_parts_ptr;
            Esp_call(0x1001B03, (Rnd() * 2 + 0x1000) << 16, &pParts2[13].Workm, 0, 0);
        }
    case 1:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        *(ULONG *)&pEm->Routine_0 = 7;
        break;
    }
    if (pEm->Hokan_flg == 0) {
        Foot_set_pl(pEm, 0, 1);
    }
}

void func_8003DC0C(void) {
}

void Pl_Em_damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEm->Be_flg &= ~0x8000;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldamage_Em[pEnemy->Id - 0x10](pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
}

void Pl_Em_die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldie_Em[pEnemy->Id - 0x10](pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
}

void Pl_dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG routine1;
    KAGE_WORK9 *pKage;

    routine1 = pEm->Routine_1;
    pKage = pEm->Kage.pKw9;
    if (routine1 != 1) {
        if (1 < routine1) {
            return;
        }
        if (routine1 != 0) {
            return;
        }
        pEm->Routine_1 = 1;
        Kage_work9_color_set(pKage, 0xBFBF10);
        pEm->Timer0 = 0x3C;
    }
    pKage->Half_x += 8;
    pKage->Half_z += 8;
    if (--pEm->Timer0 == -1) {
        pEm->Routine_1 = 2;
    }
}

ULONG Pl_life_down(LONG Down_life, ULONG Mode)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Life;

    if ((ULONG)(Down_life - 10) < 0x5B) {
        Down_life += 10;
    }
    Life = (USHORT)pPl->Life - Down_life;
    pPl->Life = Life;
    pPl->Unk132 &= 0xE0FF;
    if (Mode == 1) {
        if ((SHORT)Life < 0) {
            pPl->Life = 0;
            pPl->Status_flg |= 0x800;
            return 1;
        }
        return 0;
    }
    if (Mode == 0) {
        if ((SHORT)Life < 0) {
            if ((SHORT)Life >= -10 && !(pPl->Status_flg & 0x800)) {
                pPl->Life = 0;
                pPl->Status_flg |= 0x800;
                return 1;
            }
            pPl->Damage_cnt |= 0x80;
            G.System_flg |= SYS_GAME_OVER;
            return 2;
        }
        return 0;
    }
    /* Mode >= 2: no return value (v0 still holds 1 from the Mode compare) */
}

void Pl_life_up(LONG Up_life)
{
    Pl_work.Life += Up_life;
    Pl_work.Status_flg &= ~0x800;
    if (Pl_work.Max_life < Pl_work.Life) {
        Pl_work.Life = Pl_work.Max_life;
    }
}
