#include "mojidisp.h"
#include "pad.h"
#include "player.h"
#include "pl_wep.h"
#include "joint.h"
#include "sca.h"
#include "sld.h"
#include "snd.h"
#include "sub.h"
#include "status.h"
#include "enemy/em_common.h"

/*
 * PLAYER.C (RE2 PLAYER.C): the player work (an ENEMY_WORK, G.Player / Pl_work). Player_move runs
 * Pl_routine[Routine_0] (Pl_init, Pl_move, Pl_damage, Pl_die, ...); Pl_move runs the pad handler and
 * the motion of slot Routine_1 (Pl_mv_pad[] / Pl_mv[]). Slots 0-5 and 9 keep RE2's routines; RE3 adds
 * the quick turn (6), the slope/stairs moves (0xA, 0xE, 0xF) and others. The routine bytes and the
 * motion bytes are written as one word, like RE2 (EM_SET_RTN / EM_SET_MOVE).
 */

INCLUDE_ASM("asm/nonmatchings/game/player", Player_set);

/* per frame: damage blink, the Pl_routine step, magazine/recoil effects, water ripples, neck aim */
void Player_move(ENEMY_WORK *pEm)
{
    SVECTOR sv;
    SVECTOR sv2;
    PARTS_W *pParts;

    if ((LONG)G.Stop_flg < 0) {
        return;
    }
    G.Status_flg &= ~0x40;
    G.Room_player_flg &= 0xFFE0;
    if (pEm->Damage_cnt & 0x7F) {
        pEm->Damage_cnt--;
        pEm->Be_flg |= 0x10000000;
    }
    if (pEm->Damage_cnt & 0x80) {
        pEm->Damage_flg &= 0x7FFF;
    }
    Pl_routine[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Mag_down(pEm);
    Pl_wep_eff_move(pEm);
    Pl_wep_recoil_6(pEm);
    Pl_wep_recoil_10(pEm);
    Pl_wep_spin_0b(pEm);
    if (pEm->Water != 0 && pEm->Water < pEm->Ground - 500) {
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

/* first frame: hit area, neck limits, 9-quad shadow, stand motion (RE2 Pl_init) */
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
    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Hit_area.Top = 0x5DC;
    pEm->Hit_area.Bottom = -1000;
    pEm->Hit_area.Rate0 = 0x55;
    pEm->Damage_cnt = 0;
    pEm->pEnemy_ptr = pEm;
    pEm->pUnk114 = pEm;
    pEm->Hit_area.Rate1 = 0xAA;
    pEm->Hit_area.R = w;
    pEm->Unk140 = 0;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 1;
    pEm->Ground = -(pEm->nFloor * 1800);
    pEm->Status_flg |= 0x400;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pNeck = &pEm->pSin_parts_ptr[1];
    PW_NECK(pNeck)->Add_dir_y = 0x60;
    PW_NECK(pNeck)->Add_dir_z = 0x40;
    PW_NECK(pNeck)->L_rot_y = 0x200;
    PW_NECK(pNeck)->L_rot_z = 0x138;
    PW_NECK(pNeck)->N_rot_y = 0;
    PW_NECK(pNeck)->N_rot_z = 0;
    if (!(G.Kage9[0].Be_flg & 1)) {
        Kage_work9_set(&pEm->Kage.pKw9, 0, half_x | (half_z << 16), 0, &pEm->Pos_x);
    }
    EM_SET_MOVE(pEm, 2, 0, 0);
    Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
}

/* shadow back on; clear the water, scenario and tilt state of the player work */
void Pl_reset(ENEMY_WORK *pEm)
{
    if (!(G.Kage9[0].Be_flg & 1)) {
        Kage_work9_set(&pEm->Kage.pKw9, 0, 0x022601C2, 0, &pEm->Pos_x);
    }
    pEm->Water = 0;
    pEm->Damage_flg = 0;
    pEm->Unk144 = 0;
    pEm->Unk150 = 0;
    pEm->Unk152 = 0;
    pEm->Unk1B8 = 0;
    pEm->Tilt_max = 0;
    pEm->Tilt[0] = 0;
    pEm->Tilt[1] = 0;
    pEm->Unk1C0 = 0;
    pEm->Tilt_flg = 0;
    pEm->Tilt_spd = 0;
    pEm->Unk1CA = 0;
    pEm->Unk1C8 = 0;
}

/* pad handler and motion of the move slot Routine_1; poison drains 1 life every 30 frames */
void Pl_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pl_mv_pad[pEm->Routine_1](pEm, G.Key, G.Key_trg);
    Pl_mv[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if ((pEm->Status_flg & 0x200) && !(G.Stop_flg & 0x1000000)) {
        if (--G.Pl_poison_timer == -1) {
            G.Pl_poison_timer = 30;
            if (pEm->Life >= 2) {
                pEm->Life--;
            }
        }
    }
}

/* Pl_mv_pad 0: standing: pick walk/run/back/turn/aim from the pad */
void pl_mv00_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = Pl_life_u;
        Pl_life_u = 0;
        if (life < 0x65) {
            Pl_life_u = 1;
        }
        if (pEm->Status_flg & 0x300) {
            Pl_life_u = 1;
        }
        if (life < 0x15) {
            Pl_life_u = 2;
        }
        if (prev != Pl_life_u) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (Key & 0xA) {
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    }
    if (Key & 4) {
        EM_SET_RTN(pEm, 1, 3, 0, 0);
        if (Key_trg & 0x200) {
            EM_SET_RTN(pEm, 1, 6, 0, 0);
        }
    }
    if (Key & 1) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Key & 0x200) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
}

/* Pl_mv 0: stand (idle motions by life state) */
void pl_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        n = Pl_mot_tbl[Pl_life_u];
        pEm->Routine_2 = 1;
        n += 0x70000;
        EM_MOVE(pEm) = n;
        G.Status_flg &= ~0x100;
        pEm->Timer0 = 0xF0;
    case 1:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (Pl_life_u != 0) {
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
        EM_SET_MOVE(pEm, 11, 0, 7);
        pEm->Routine_2 = 3;
        pEm->Timer0 = ((Rnd() & 7) + 8) * 30;
        if ((Rnd() & 0x1F) == 2 && G.Em_num == 0) {
            pEm->Timer0 |= 0x8000;
        }
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            EM_SET_MOVE(pEm, 12, 0, 7);
            pEm->Routine_2 = 3;
        }
        if (Pl_life_u != 0) {
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
        EM_SET_MOVE(pEm, 11, 0, 7);
        pEm->Routine_2 = 6;
    case 6:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            pEm->Routine_2 = 0;
        }
        break;
    case 7:
        EM_SET_MOVE(pEm, 21, 0, 15);
        pEm->Routine_2 = 8;
        pEm->Neck_flg |= 0x12;
    case 8:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 0;
            pEm->Neck_flg &= 0xED;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* Pl_mv_pad 1: walking */
void pl_mv01_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = Pl_life_u;
        Pl_life_u = 0;
        if (life < 0x65) {
            Pl_life_u = 1;
        }
        if (pEm->Status_flg & 0x300) {
            Pl_life_u = 1;
        }
        if (life < 0x15) {
            Pl_life_u = 2;
        }
        if (prev != Pl_life_u) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 1)) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (Key & 0xA) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        if (Key & 4) {
            EM_SET_RTN(pEm, 1, 3, 0, 0);
            if (Key_trg & 0x200) {
                EM_SET_RTN(pEm, 1, 6, 0, 0);
            }
        }
        return;
    }
    if (pEm->Col.Hit_flg & 0x40) {
        if (Pl_slope_dir(pEm, 0) & 0x1000) {
            EM_SET_RTN(pEm, 1, 10, 0, 0);
        } else {
            EM_SET_RTN(pEm, 1, 10, 2, 0);
        }
    }
    if (Key & 2) {
        pEm->Cdir_y += Pl_rot_spd_walk[Pl_life_u];
    }
    if (Key & 8) {
        pEm->Cdir_y -= Pl_rot_spd_walk[Pl_life_u];
    }
    if (Key & 0x200) {
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if (D_800D1F2C & 0x10) {
        EM_SET_RTN(pEm, 1, 9, 0, 0);
    }
}

/* Pl_mv 1: walk */
void pl_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    SHORT spd_x;

    switch (pEm->Routine_2) {
    case 0:
        n = Pl_mot_tbl[Pl_life_u + 3];
        pEm->Routine_2 = 1;
        n += 0x70000;
        EM_MOVE(pEm) = n;
        G.Status_flg &= ~0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(0, m * 3 + 11, pEm);
            G.Room_player_flg |= 2;
        }
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Water < pEm->Ground - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

/* Pl_mv_pad 2: running */
void pl_mv02_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = Pl_life_u;
        Pl_life_u = 0;
        if (life < 0x65) {
            Pl_life_u = 1;
        }
        if (pEm->Status_flg & 0x300) {
            Pl_life_u = 1;
        }
        if (life < 0x15) {
            Pl_life_u = 2;
        }
        if (prev != Pl_life_u) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if ((Key & 0x201) != 0x201) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (Key & 0xA) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        if (Key & 4) {
            EM_SET_RTN(pEm, 1, 3, 0, 0);
        }
        if (Key & 1) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        return;
    }
    if (pEm->Col.Hit_flg & 0x40) {
        if (Pl_slope_dir(pEm, 0) & 0x1000) {
            EM_SET_RTN(pEm, 1, 15, 0, 0);
        } else {
            EM_SET_RTN(pEm, 1, 15, 2, 0);
        }
    }
    if (pEm->Spd_x >= 0x33) {
        if (Key & 2) {
            pEm->Cdir_y += Pl_rot_spd_run[Pl_life_u];
        }
        if (Key & 8) {
            pEm->Cdir_y -= Pl_rot_spd_run[Pl_life_u];
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 0, 0);
        }
    }
    if (D_800D1F2C & 0x10) {
        EM_SET_RTN(pEm, 1, 9, 0, 0);
    }
}

/* Pl_mv 2: run */
void pl_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    SHORT spd_x;

    switch (pEm->Routine_2) {
    case 0:
        n = Pl_mot_tbl[Pl_life_u + 6];
        pEm->Routine_2 = 1;
        n += 0x70000;
        EM_MOVE(pEm) = n;
        G.Status_flg &= ~0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Room_player_flg |= 4;
        }
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Water < pEm->Ground - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

/* Pl_mv_pad 3: stepping back */
void pl_mv03_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = Pl_life_u;
        Pl_life_u = 0;
        if (life < 0x65) {
            Pl_life_u = 1;
        }
        if (pEm->Status_flg & 0x300) {
            Pl_life_u = 1;
        }
        if (life < 0x15) {
            Pl_life_u = 2;
        }
        if (prev != Pl_life_u) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 4)) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (Key & 0xA) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        if (Key & 1) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        return;
    }
    if (pEm->Col.Hit_flg & 0x40) {
        if (Pl_slope_dir(pEm, 0) & 0x1000) {
            EM_SET_RTN(pEm, 1, 14, 0, 0);
        } else {
            EM_SET_RTN(pEm, 1, 14, 2, 0);
        }
    }
    if (Key_trg & 0x200) {
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    }
    if (Key & 2) {
        pEm->Cdir_y += Pl_rot_spd_back[Pl_life_u];
    }
    if (Key & 8) {
        pEm->Cdir_y -= Pl_rot_spd_back[Pl_life_u];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
}

/* Pl_mv 3: step back */
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
        EM_SET_MOVE(pEm, 9, 0, 7);
        if (Pl_life_u != 0) {
            EM_SET_MOVE(pEm, 10, 0, 7);
        }
        pEm->Routine_2 = 1;
        G.Status_flg &= ~0x100;
        pEm->UnkE2 = 9;
        pEm->UnkE3 = 0;
    case 1:
        if (*(signed char *)&pEm->UnkE3 == 0 && Pl_life_u == 0) {
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
            G.Room_player_flg |= 2;
        }
        break;
    }
    if (pEm->Water < pEm->Ground - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}


/* pEnemy_ptr = the nearest live enemy within Lim, in sight and within Dir of the facing (RE2 Lim_dir_ck) */
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
                    && Sca_ck_line(WORKM_POS_V(&pEm->pSin_parts_ptr[pEm->Neck_no]),
                                   WORKM_POS_V(&pT->pSin_parts_ptr[pT->Neck_no]), 0x2040, 1) == 0
                    && Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, Dir) == 0
                    && Cdir_ck(pEm, pT) == 0 && dist < min) {
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

/* Pl_mv_pad 4: turning on the spot */
void pl_mv04_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = Pl_life_u;
        Pl_life_u = 0;
        if (life < 0x65) {
            Pl_life_u = 1;
        }
        if (pEm->Status_flg & 0x300) {
            Pl_life_u = 1;
        }
        if (life < 0x15) {
            Pl_life_u = 2;
        }
        if (prev != Pl_life_u) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 0xA)) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
    }
    if (Key & 4) {
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    if (Key & 1) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
        if (Key & 0x200) {
            EM_SET_RTN(pEm, 1, 2, 0, 0);
        }
    }
    if (Key & 2) {
        pEm->Cdir_y += Pl_rot_spd[Pl_life_u];
    }
    if (Key & 8) {
        pEm->Cdir_y -= Pl_rot_spd[Pl_life_u];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
}

/* Pl_mv 4: turn on the spot */
void pl_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        n = Pl_mot_tbl[Pl_life_u + 3];
        pEm->Routine_2 = 1;
        n += 0x70000;
        EM_MOVE(pEm) = n;
        G.Status_flg &= ~0x100;
        pEm->Spd_x = 0x15;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(0, m * 3 + 11, pEm);
            G.Room_player_flg |= 2;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
}

/* Pl_mv_pad 6: quick turn (back to standing when the game is stopped) */
void pl_mv06_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (G.Stop_flg & 0x1000000) {
        EM_SET_RTN(pEm, 1, 0, 0, 0); /* Routine_0 = 1, Routine_1..3 = 0 */
    }
}

/* Pl_mv 6: quick turn, 8 frames of +0x100 while stepping */
void pl_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        n = Pl_mot_tbl[Pl_life_u + 6];
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 8;
        pEm->Spd_x = 0x32;
        pEm->Spd_z = 0;
        n += 0xF0000;
        EM_MOVE(pEm) = n;
        Get_speedXZ(pEm, 0x700, &pEm->Spd_x, &pEm->Spd_z);
        G.Status_flg &= ~0x100;
        G.Room_player_flg |= 4;
        Snd_se_walk(0, 11, pEm);
    case 1:
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x100);
        pEm->Cdir_y += 0x100;
        if (--*((signed char *)pEm + 0xE2) == 0) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            G.Room_player_flg |= 4;
            Snd_se_walk(0, 11, pEm);
        }
        pEm->Pos_x += pEm->Spd_x;
        pEm->Pos_z += pEm->Spd_z;
        break;
    }
}

/* Pl_mv_pad 7: no pad input */
void pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/player", pl_mv07);

/* Pl_mv_pad 8: no pad input */
void pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

/* Pl_mv 8: motion 5, then open the status screen and wait */
void pl_mv08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
        G.Status_flg |= 0x140;
    case 1:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        if (!(G.System_flg & 0x40)) {
            St.St_type = 1;
            Game.Unk1 = 1;
            G.Status_flg |= 0x200;
            Game.Stop_bak = G.Stop_flg;
        }
        pEm->Routine_2 = 3;
        break;
    case 4:
        EM_SET_MOVE(pEm, 5, 0, 7);
        pEm->Routine_2 = 5;
    case 5:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x10200);
        break;
    case 6:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        G.Status_flg &= ~0x40;
        G.Status_flg &= ~0x100;
        break;
    }
}

/* Pl_mv_pad 9: lets the push end (Routine_2 5 -> 6) unless Flg 0x10 is blocked */
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
        Snd_se_stad(0x10200, POS_V(pEm), 0, 0);
        Pad_vib_init();
    }
}

/* Pl_mv 9: push an object: square up, push with rumble, step back (RE2 pl_mv0a) */
void pl_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    USHORT dir;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = Pl_mot_tbl[Pl_life_u + 3] + 0x70000;
        G.Status_flg |= 0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Room_player_flg |= 4;
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
        EM_SET_MOVE(pEm, 6, 0, 7);
    case 3:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 4:
        pEm->Routine_2 = 5;
        EM_SET_MOVE(pEm, 7, 0, 7);
    case 5:
        if (pEm->Move_cnt == 1) {
            Snd_se_stad(0x1022C, POS_V(pEm), 0, 0);
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
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        break;
    }
}

/* Pl_mv_pad 0xA: walking on a slope or stairs */
void pl_mv_slope_walk_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (!(Key & 1)) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (Key & 0xA) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        if (Key & 4) {
            EM_SET_RTN(pEm, 1, 14, 0, 0);
            if (Key_trg & 0x200) {
                EM_SET_RTN(pEm, 1, 6, 0, 0);
            }
        }
        return;
    }
    if (!(pEm->Col.Hit_flg & 0x40)) {
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
    if (Key & 2) {
        pEm->Cdir_y += Pl_rot_spd_walk[Pl_life_u];
    }
    if (Key & 8) {
        pEm->Cdir_y -= Pl_rot_spd_walk[Pl_life_u];
    }
    if (!(Pl_slope_dir(pEm, 0) & 0x1000)) {
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
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
}

/* Pl_mv 0xA: walk along the slope, upper and lower body played separately */
void pl_mv_slope_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(Pl_slope_dir(pEm, 1) & 0x1000)) {
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
            G.Room_player_flg |= 4;
        }
        Joint_move(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk194_kan_t_ptr, G.Player.pUnk190_seq_t_ptr, 0x200);
        Leg_ik_set(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        EM_MOVE(pEm) = Pl_mot_tbl[Pl_life_u + 3] + 0x70000;
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
            G.Room_player_flg |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, Pl_slope_dir(pEm, 0), 0x10);
    } else {
        Goto01(pEm, Pl_slope_dir(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

/* Pl_mv_pad 0xE: stepping back on a slope */
void pl_mv_slope_back_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 4)) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (Key & 0xA) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        if (Key & 1) {
            EM_SET_RTN(pEm, 1, 10, 0, 0);
        }
        return;
    }
    if (!(Pl_slope_dir(pEm, 0) & 0x1000)) {
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
        EM_SET_RTN(pEm, 1, 3, 0, 0);
    }
    if (Key_trg & 0x200) {
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    }
    if (Key & 2) {
        pEm->Cdir_y += Pl_rot_spd_back[Pl_life_u];
    }
    if (Key & 8) {
        pEm->Cdir_y -= Pl_rot_spd_back[Pl_life_u];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
}

/* Pl_mv 0xE: step back on a slope */
void pl_mv_slope_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(Pl_slope_dir(pEm, 1) & 0x1000)) {
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
            G.Room_player_flg |= 4;
        }
        Joint_move(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0x10200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk194_kan_t_ptr, G.Player.pUnk190_seq_t_ptr, 0x10200);
        Leg_ik_set(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        EM_SET_MOVE(pEm, 9, 0, 7);
        if (Pl_life_u != 0) {
            EM_SET_MOVE(pEm, 10, 0, 7);
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
            G.Room_player_flg |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, Pl_slope_dir(pEm, 0), 0x10);
    } else {
        Goto01(pEm, Pl_slope_dir(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

/* Pl_mv_pad 0xF: running on a slope */
void pl_mv_slope_run_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if ((Key & 0x201) != 0x201) {
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        if (Key & 0xA) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
        if (Key & 4) {
            EM_SET_RTN(pEm, 1, 3, 0, 0);
        }
        if (Key & 1) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        return;
    }
    if (!(pEm->Col.Hit_flg & 0x40)) {
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    }
    if (pEm->Spd_x >= 0x33) {
        if (Key & 2) {
            pEm->Cdir_y += Pl_rot_spd_run[Pl_life_u];
        }
        if (Key & 8) {
            pEm->Cdir_y -= Pl_rot_spd_run[Pl_life_u];
        }
    }
    if (!(Pl_slope_dir_run(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        EM_SET_RTN(pEm, 1, 5, 0, 0);
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
        }
    }
}

/* Pl_mv 0xF: run on a slope */
void pl_mv_slope_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(Pl_slope_dir_run(pEm, 1) & 0x1000)) {
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
            G.Room_player_flg |= 4;
        }
        Joint_move(pEm, G.Player.pUnk19C_kan_t_ptr, G.Player.pUnk198_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk194_kan_t_ptr, G.Player.pUnk190_seq_t_ptr, 0x200);
        Leg_ik_set(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        EM_MOVE(pEm) = Pl_mot_tbl[Pl_life_u + 6] + 0x70000;
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
            G.Room_player_flg |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, Pl_slope_dir_run(pEm, 0), 0x10);
    } else {
        Goto01(pEm, Pl_slope_dir_run(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

/* direction up the slope under the player (Slope_dir_tbl), | 0x1000 when facing down it; Flg: switch to the matching slope walk motion */
ULONG Pl_slope_dir(ENEMY_WORK *pEm, ULONG Flg)
{
    USHORT dir;
    ULONG d;
    ULONG mot;
    USHORT r;

    dir = Slope_dir_tbl[(UCHAR)pEm->Col.Unk39 & 7];
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
                if (Pl_life_u == 0) {
                    mot = (Pl_wep_flg[(UCHAR)pEm->Type] & 3) << 2;
                }
                EM_MOVE(pEm) = (pEm->Move_cnt << 8) + (mot | 0x70000);
                *(ULONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70000;
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
            if (Pl_life_u == 0) {
                mot = ((Pl_wep_flg[(UCHAR)pEm->Type] & 3) << 2) | 1;
            }
            EM_MOVE(pEm) = (pEm->Move_cnt << 8) + (mot | 0x70000);
            *(ULONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70001;
        }
        return (((SHORT)r + 0x800) & 0xFFF) | 0x1000;
    }
    return ((SHORT)dir + 0x800) & 0xFFF;
}

/* Pl_slope_dir with the slope run motions */
LONG Pl_slope_dir_run(ENEMY_WORK *pEm, ULONG Flg)
{
    USHORT dir;
    ULONG d;
    ULONG mot;
    USHORT r;

    dir = Slope_dir_tbl[(UCHAR)pEm->Col.Unk39 & 7];
    d = (dir - (USHORT)pEm->Cdir_y) & 0xFFF;
    r = dir;
    if (d - 0x400 > 0x800) {
        if (d - 0x258 > 0xB50) {
            if (Flg) {
                mot = 0xE;
                if (Pl_life_u == 0) {
                    mot = ((Pl_wep_flg[(UCHAR)pEm->Type] & 3) << 2) | 2;
                }
                EM_MOVE(pEm) = (pEm->Move_cnt << 8) + (mot | 0x70000);
                *(ULONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70002;
            }
            return (SHORT)r + 0x1000;
        }
        return (SHORT)dir;
    }
    if (d - 0x5A9 < 0x4AF) {
        if (Flg) {
            mot = 0xF;
            if (Pl_life_u == 0) {
                mot = ((Pl_wep_flg[(UCHAR)pEm->Type] & 3) << 2) | 3;
            }
            EM_MOVE(pEm) = (pEm->Move_cnt << 8) + (mot | 0x70000);
            *(ULONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70003;
        }
        return (((SHORT)r + 0x800) & 0xFFF) | 0x1000;
    }
    return ((SHORT)dir + 0x800) & 0xFFF;
}

/* Pl_mv_pad 0xB: no pad input */
void pl_mv0b_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

/* Pl_mv 0xB */
void pl_mv0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        EM_SET_MOVE(pEm, 0, 0, 7);
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
            G.Room_player_flg |= 4;
        }
        Oba_hit_clr(&Pl_work.Col);
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
        EM_SET_MOVE(pEm, 0, 0, 3);
    case 3:
        if (pEm->Move_cnt == 0x11) {
            pEm->Damage_cnt |= 0x80;
            pEm->nFloor++;
            pEm->Unk1CA = pEm->Unk1C8;
            if (pEm->Water != 0 && pEm->Water < pEm->Ground - 500) {
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
            pEm->Ground -= 0x708;
            if (pEm->Water != 0 && pEm->Water < pEm->Ground - 500) {
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
            G.Room_player_flg |= 4;
        }
        Add_speedXZ(pEm, 0);
        Oba_hit_clr(&Pl_work.Col);
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

/* Pl_mv_pad 0xC: no pad input */
void pl_mv0c_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

/* Pl_mv 0xC */
void pl_mv0c(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_SET_MOVE(pEm, 1, 0, 7);
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
            G.Room_player_flg |= 4;
        }
        Add_speedXZ(pEm, 0);
        Oba_hit_clr(&Pl_work.Col);
        if (pEm->Move_cnt == 0xE) {
            pEm->Routine_3 = 2;
        }
        break;
    case 2:
        pEm->Routine_3 = 3;
        EM_SET_MOVE(pEm, 3, 0, 0);
        pEm->Spd_x = 0x96;
        pEm->Timer0 = 0;
        pEm->Pos_y += 0x226;
        pEm->Timer0 = 5;
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        pEm->Unk1CA = 0;
        pEm->nFloor--;
        Oba_hit_clr(&Pl_work.Col);
        pEm->UnkE3 = 0;
        pEm->Be_flg &= ~0x2000;
    case 3:
        pEm->Pos_y += pEm->Timer0++ * 30;
        pEm->Ground = Sca_get_ground(&pEm->Pos_x, 400, 0x8000, 0);
        pEm->nFloor = pEm->Pos_y / -1800;
        Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            Snd_se_walk(1, (m << 1) + m + 11, pEm);
            G.Room_player_flg |= 4;
        }
        if (pEm->Ground - 300 < pEm->Pos_y) {
            pEm->Routine_3 = 4;
            Pad_vib_set0(1, 0);
            Pad_vib_set1(6, 0x96, 0);
        }
        if (pEm->Water != 0 && pEm->Water < pEm->Ground - 500) {
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
        pEm->Pos_y = pEm->Ground;
        pEm->Damage_cnt &= 0x7F;
        pEm->Be_flg &= ~0x8000;
        Snd_se_walk(1, 11, pEm);
        G.Room_player_flg |= 4;
    case 5:
        if (pEm->Move_cnt == 6) {
            Snd_se_walk(1, 14, pEm);
        }
        pEm->nFloor = pEm->Pos_y / -1800;
        pEm->Routine_3 += Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        Oba_hit_clr(&Pl_work.Col);
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

/* Pl_mv_pad 0xD: no pad input */
void pl_mv0d_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

/* Pl_mv 0xD */
void pl_mv0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    HIT_REQ Req;
    ENEMY_WORK *pT;
    LONG hit;
    LONG *p;
    ULONG no;

    pT = pEm->pEnemy_ptr;
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 18, 0, 3);
        pEm->Routine_2 = 1;
        Snd_se_stad(0x1000B, POS_V(pEm), 0, 0);
        G.Status_flg |= 0x100;
        G.Room_player_flg |= 0x1C;
        G.Dodge_cnt++;
        Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x100);
        pEm->UnkE3 = 0;
        pEm->Neck_flg |= 0x12;
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
            EM_SET_RTN(pEm, 1, 5, 6, 0);
            p = &pT->Pos_x;
            pEm->pUnk16C = p;
            pEm->pUnk170 = p;
            pEm->pUnk15C = p;
            if ((signed char)pEm->UnkE3 != 0 && !(G.Key & 0x500)) {
                EM_SET_RTN(pEm, 1, 0, 0, 0);
            }
            if ((UCHAR)pEm->Type == 0) {
                EM_SET_RTN(pEm, 1, 0, 0, 0);
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
            Req.pPos = WORKM_POS_V(pEm->pSin_parts_ptr);
            Req.W = 1000;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEm->Cdir_y;
            hit = Pos_em_at_ck(&Req, &D_8009CE0C);
            pEm->Damage_cnt |= 0x80;
        }
        if (hit) {
            no = 3;
            if ((ULONG)(Snd_edt[3] - 1) < 30) {
                no = 0x13;
            }
            Snd_se_stad(no | 0x10300, POS_V(pEm), 0, 0);
        }
    default:
        Add_speedXZ(pEm, 0);
    }
}

/* damage routine of slot Routine_1 (Pl_dm[]) (RE2 Pl_damage) */
void Pl_damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_dm[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* light hit: short flinch motion with rumble */
void pl_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 3);
        pEm->Routine_2 = 1;
        Snd_se_stad(0x10000, POS_V(pEm), 0, 0);
        pEm->Damage_cnt = 0x80;
        Pad_vib_set0(5, 0);
        Pad_vib_set1(8, 0x96, 0);
        pEm->Be_flg &= ~0x8000;
        pEm->Neck_flg |= 0x12;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            G.Status_flg &= ~0x100;
            pEm->Neck_flg &= 0xED;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* hit from the side (Routine_1 & 1 picks the side): slides sideways */
void pl_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = (pEm->Routine_1 & 1) + 0x30002;
        Snd_se_stad(((pEm->Routine_1 & 1) + 1) | 0x10000, POS_V(pEm), 0, 0);
        pEm->Damage_cnt = 0x80;
        Pad_vib_set0(5, 0);
        Pad_vib_set1(8, 200, 0);
        pEm->Be_flg &= ~0x8000;
        *(LONG *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 400 - 200;
        if (pEm->Routine_1 >= 8) {
            *(LONG *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 600 - 300;
        }
        pEm->Spd_z = 0;
        pEm->Neck_flg |= 0x12;
    case 1:
        if (pEm->Hokan_flg != 0) {
            Add_speedXZ(pEm, 0);
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            G.Status_flg &= ~0x100;
            pEm->Neck_flg &= 0xED;
        }
        break;
    }
    Flg = *pEm->pNow_seq;
    if (Flg & 0x200) {
        n = Flg & 0x400;
        m = n != 0;
        Snd_se_walk(1, m * 14, pEm);
        G.Room_player_flg |= 4;
    }
}

/* heavy hit from the front: knocked back (Spd_x -800), blood when Unk132 0x500 */
void pl_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 10, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Spd_x = -800;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 0x12;
        Snd_se_stad(0x10001, POS_V(pEm), 0, 0);
        pEm->Be_flg &= ~0x8000;
        if (pEm->Damage_flg & 0x500) {
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
            EM_SET_MOVE(pEm, 12, 0, 3);
            pEm->Routine_2 = 2;
        }
        Add_speedXZ(pEm, 0);
        Joint_move2(pEm, pKan, pSeq, 0x400);
        pEm->Spd_x += 0x96;
        break;
    case 2:
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Neck_flg &= 0xED;
        }
        pEm->Spd_x += 10;
        if (pEm->Spd_x > 0) {
            pEm->Spd_x = 0;
        }
        break;
    case 3:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            EM_SET_MOVE(pEm, 9, 0, 3);
            pEm->Routine_2 = 4;
            pEm->Damage_cnt = 0;
        }
        break;
    case 4:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 5;
            EM_SET_MOVE(pEm, 5, 0, 3);
        }
        break;
    case 5:
        if (Joint_move2(pEm, pKan, pSeq, 0x10400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Neck_flg &= 0xED;
        }
        break;
    }
}

/* heavy hit from behind: knocked forward */
void pl_dm03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 11, 0, 3);
        pEm->Routine_2 = 1;
        pEm->Spd_x = 0x190;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Neck_flg |= 0x12;
        Snd_se_stad(0x10002, POS_V(pEm), 0, 0);
        pEm->Be_flg &= ~0x8000;
        if (pEm->Damage_flg & 0x500) {
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
            EM_SET_MOVE(pEm, 9, 0, 3);
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
            EM_SET_MOVE(pEm, 5, 0, 3);
        }
        break;
    case 3:
        if (Joint_move2(pEm, pKan, pSeq, 0x10400)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Damage_cnt = 0;
            pEm->Neck_flg &= 0xED;
        }
        break;
    }
}

/* death routine of slot Routine_1 (Pl_die_t[]) (RE2 Pl_die) */
void Pl_die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_die_t[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* Pl_die_t 0: death motion, then game over */
void pl_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 7);
        pEm->Routine_2 = 1;
        *(LONG *)&pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        Snd_se_stad(0x10003, POS_V(pEm), 0, 0);
        pEm->Neck_flg = 1;
        Pad_vib_set0(3, 0);
        Pad_vib_set1(8, 200, 0);
        Pad_vib_fade_set(0x5A, 200, 0, 8);
        if (pEm->Damage_flg & 0x500) {
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
        EM_SET_RTN(pEm, 7, 0, 0, 0);
        break;
    }
    if (pEm->Hokan_flg == 0) {
        Foot_set_pl(pEm, 0, 1);
    }
}

/* empty, unreferenced */
void Pl_null(void) {
}

/* damage motion played by the enemy that holds the player (by enemy kind) */
void Pl_Em_damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEm->Be_flg &= ~0x8000;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldamage_Em[pEnemy->Id - 0x10](pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
}

/* death motion played by the enemy that killed the player */
void Pl_Em_die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldie_Em[pEnemy->Id - 0x10](pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
}

/* after death: wait, then game over (RE2 Pl_dead) */
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

/* take Down_life off the player's life; Mode picks the hit reaction; 1 when the player dies (RE2 Pl_life_down) */
ULONG Pl_life_down(LONG Down_life, ULONG Mode)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Life;

    if ((ULONG)(Down_life - 10) < 0x5B) {
        Down_life += 10;
    }
    Life = (USHORT)pPl->Life - Down_life;
    pPl->Life = Life;
    pPl->Damage_flg &= 0xE0FF;
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

/* heal Up_life, cure poison (RE2 Pl_life_up) */
void Pl_life_up(LONG Up_life)
{
    Pl_work.Life += Up_life;
    Pl_work.Status_flg &= ~0x800;
    if (Pl_work.Max_life < Pl_work.Life) {
        Pl_work.Life = Pl_work.Max_life;
    }
}
