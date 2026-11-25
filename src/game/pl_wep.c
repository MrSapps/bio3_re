#include "pad.h"
#include "pl_wep.h"
#include "enemy/em_common.h"
#include "player.h"
#include "cd.h"
#include "sca.h"
#include "joint.h"
#include "sld.h"
#include "spl_auto.h"
#include "snd.h"
#include "status.h"
#include "sub.h"

/*
 * PL_WEP.C + PL_WEP2.C (RE2): the player's weapons: Pl_mv slot 5 (pl_mv05_*: aim, fire, reload by
 * weapon class), the per-weapon firing motions pl_wXX_t2_move, magazines (Mag_set), the weapon effect
 * parts and light polys (Pl_wep_eff_*, Pl_wep_poly_*), and the hit checks shared by everyone
 * (Weapon_at_ck, Pos_em_at_ck, Hit_area_em_ck, ...).
 */



void Pl_neck(ENEMY_WORK *pEm, ULONG LL, SHORT Dir)
{
    ULONG Lim;
    ENEMY_WORK *pNear;
    ULONG Min_dist;
    ENEMY_WORK *pSec;
    ENEMY_WORK **ppEm;
    ULONG Cand_dist;
    ENEMY_WORK *pCand;
    tagGLOBAL *pG = &G;

    pNear = 0;
    Min_dist = 0x7FFFFFFF;
    pSec = 0;
    if (pEm->Neck_flg & 1) {
        return;
    }
    if (Pl_neck_timer-- == 0) {
        Pl_neck_timer = 0x2D;
        if (pG->Em_num != 0) {
            Lim = LL;
            ppEm = &pG->pEnemy[1];
            do {
                pCand = *ppEm;
                if ((pCand->Be_flg & 1) && !(pCand->Type & 0xC000)) {
                    Cand_dist = pCand->L_pl;
                    if (Sca_ck_line(WORKM_POS_V(&pEm->pSin_parts_ptr[pEm->Neck_no]), WORKM_POS_V(&pCand->pSin_parts_ptr[pCand->Neck_no]), 0x40, 1) == 0
                        && Goto00_ck(pEm, pCand->Pos_x, pCand->Pos_z, Dir) == 0) {
                        if (!(pCand->Type & 0x2000)) {
                            if (Cand_dist < Lim) {
                                Lim = Cand_dist;
                                pNear = pCand;
                                ppEm++;
                                continue;
                            }
                        } else if (Cand_dist < Min_dist) {
                            Min_dist = Cand_dist;
                            pSec = pCand;
                        }
                    }
                }
                ppEm++;
            } while (ppEm != pG->ppEnemy_end);
            if (pNear != 0) {
                pEm->pUnk114 = pNear;
                return;
            }
            if (pSec != 0) {
                pEm->pUnk114 = pSec;
                return;
            }
        }
        pEm->pUnk114 = pEm;
    }
}

LONG Rnd_l(void) __asm__("Rnd"); /* returns long here (canonical unsigned long) */

void Pl_water(ENEMY_WORK *pEm)
{
    SVECTOR sv;
    SVECTOR sv2;
    PARTS_W *pParts;

    pParts = pEm->pSin_parts_ptr;
    CompM((MATRIX *)pEm->Workm_m, PW_M(pParts), &pParts->Workm);
    sv.vx = pParts->Workm.t[0];
    sv.vy = pEm->Water;
    sv.vz = pParts->Workm.t[2];
    Esp_call(0x61C, ((Rnd_l() * 4 + 0xC00) << 16) | (((USHORT)pEm->Cdir_y - Rnd_l() * 4) & 0xFFFF), &GsIDMATRIX, &sv2, 0);
    Esp_call(0x61C, ((Rnd_l() * 4 + 0xC00) << 16) | (((USHORT)pEm->Cdir_y + (0x800 - Rnd_l() * 4)) & 0xFFFF), &GsIDMATRIX, &sv, 0);
    Esp_call(0x161C, ((Rnd_l() * 4 + 0x800) << 16) | ((Rnd_l() * 16) & 0xFFFF), &GsIDMATRIX, &sv, 0);
    Esp_call(0x161C, ((Rnd_l() * 4 + 0xC00) << 16) | ((Rnd_l() * 16) & 0xFFFF), &GsIDMATRIX, &sv, 0);
    sv2.vx = 0x400;
    sv2.vy = 0;
    sv2.vz = 0;
    Esp_call(0x20E, (USHORT)pEm->Cdir_y | 0x20000000, &GsIDMATRIX, &sv, &sv2);
}

void Pl_weapon_vib(ENEMY_WORK *pEm)
{
    UCHAR (*pTbl)[6] = Pl_vib_tbl;
    UCHAR *pVib;

    pVib = pTbl[(UCHAR)(pEm->Type - 2)];
    Pad_vib_set0(pVib[0], 0);
    Pad_vib_set1(pVib[1], pVib[2], 0);
    Pad_vib_fade_set(pVib[5], pVib[3], pVib[4], pVib[1]);
}

void pl_mv05_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

void pl_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pl_aim_cnt++;
    if ((G.System_flg & 0x100) && pEm->Damage_cnt == 0 && pEm->Routine_2 < 6) {
        if ((LONG)pEm->Be_flg < 0 && (Pl_aim_cnt % 5) == 0 && ((Rnd_l() >> (Rnd_l() & 3)) & 1)) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
            Pl_mv[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
            return;
        }
        if (Rnd_l() & 1) {
            pEm->Be_flg |= 0x10000000;
        }
    }
    G.Status_flg |= 0x140;
    pl_mv05_w_tbl[(UCHAR)pEm->Type - 1](pEm, pKan, pSeq, G.Key);
    if (pEm->Spd_x == 0 && (pEm->Routine_2 != 0 || pEm->Hokan_flg == 0)) {
        Foot_set_pl(pEm, 0, Pl_wep_frame_tbl[(UCHAR)pEm->Type - 1][0] >> 7);
    }
}

void pl_mv05_w0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    pl_mv05_w0_tbl[pEm->Routine_2](pEm, pKan, pSeq, Key);
}

void pl_mv05_w0_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    PARTS_W *pP;
    PARTS_W *pP2;
    ENEMY_WORK *pHit;
    LONG Lo;
    LONG Hi;
    MATRIX Mtx;
    SVECTOR Sv;
    SVECTOR Spd;
    MATRIX M;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Sub_move_no = EM_MOVE(pEm) = Pl_aim_dir * 2 + 0x30001;
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 0;
    }
    if (!(Key & 0x500) && (Pl_wep_frame_tbl[(UCHAR)pEm->Type - 1][2] & 0x7F) < pEm->Move_cnt) {
        *(USHORT *)&pEm->Routine_2 = 1;
        return;
    }
    if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200)) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
    Leg_ik_set(pEm, 0x200);
    if (!(Key & 0x500) && pEm->Move_cnt > 20) {
        *(USHORT *)&pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        return;
    }
    pP = pEm->pSin_parts_ptr;
    RotMatrix(CDIR_SV(pEm), (MATRIX *)pEm->Workm_m);
    Spd.vx = 0;
    Spd.vz = 300;
    Spd.vy = 0;
    if (pEm->Id >= 8) {
        Spd.vx = 0;
        Spd.vz = 200;
        Spd.vy = 0;
    }
    ApplyMatrixSV((MATRIX *)pEm->Workm_m, &Spd, &Spd);
    CompM((MATRIX *)pEm->Workm_m, (MATRIX *)pP[0].M, &Mtx);
    CompM(&Mtx, (MATRIX *)pP[2].M, &Mtx);
    CompM(&Mtx, (MATRIX *)pP[3].M, &Mtx);
    CompM(&Mtx, (MATRIX *)pP[4].M, &Mtx);
    if (pEm->Water != 0) {
        if (pEm->Water < pEm->Ground - 500 && pEm->Move_cnt < 10 && (pEm->Move_cnt & 1)) {
            Pl_water(pEm);
        }
        if (pEm->Water < Mtx.t[1]) {
            Esp_call(0x161C, ((Rnd_l() * 4 + 0x60C) << 16) | (USHORT)pEm->Cdir_y, &Mtx, 0, 0);
            if ((signed char)pEm->UnkE2 == 0) {
                pEm->UnkE2 = 1;
                Snd_se_stad(0x10109, WORKM_POS_V(&pP[0]), 0, 0);
            }
        }
    }
    pHit = 0;
    Lo = 6;
    Hi = 13;
    if (pEm->Id < 8) {
        Lo = 5;
        Hi = 12;
    }
    if (Lo < pEm->Move_cnt && pEm->Move_cnt < Hi) {
        pP2 = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = pEm->Cdir_y;
        Sv.vz = -0x400;
        RotMatrix_gte(&Sv, &M);
        M.t[0] = Mtx.t[0];
        M.t[1] = Mtx.t[1];
        M.t[2] = Mtx.t[2];
        pHit = Weapon_at_ck(&M, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        if (Weapon_om_at_ck(&M, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type)) {
            Snd_se_stad(0x10108, WORKM_POS_V(&pP2[4]), 0, 0);
        }
        Mtx.t[0] += Spd.vx;
        Mtx.t[2] += Spd.vz;
        if ((SHORT)Sca_get_ground(Mtx.t, 10, 0x2040, 0x10) < Mtx.t[1]) {
            *(USHORT *)&pEm->Routine_2 = 7;
            EM_SET_MOVE(pEm, 7, 0, 3);
            *(ULONG *)&pEm->Sub_move_no = 0x30007;
            Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x400);
            pEm->Be_flg |= 0x2000000;
            Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x400);
            Leg_ik_set(pEm, 0x400);
            Snd_se_stad(0x10108, WORKM_POS_V(&pP2[4]), 0, 0);
            Pad_vib_set0(6, 0);
            Pad_vib_set1(3, 200, 0);
            return;
        }
    }
    if (pHit != 0) {
        Snd_se_stad(0x10106, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
        pEm->UnkE3 = 3;
        pEm->UnkDA = (pHit->Be_flg >> 22) & 3;
        if (pEm->UnkDA == 3) {
            pEm->UnkDA = 7;
        }
        Pad_vib_set1(8, 0xA0, 0);
    }
    if ((signed char)pEm->UnkE3 != 0) {
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 200;
        Sv.vy = 200;
        Sv.vz = 0;
        if (pEm->Id < 8) {
            Sv.vx = -200;
        }
        Esp_call(((UCHAR)pEm->UnkDA & 0xF) << 24, (USHORT)pEm->Cdir_y | 0xF380000, &pP[4].Workm, &Sv, 0);
        Esp_call((((UCHAR)pEm->UnkDA & 0xF) << 24) | 0x1700, (USHORT)pEm->Cdir_y | 0xF380000, &pP[4].Workm, &Sv, 0);
        pEm->UnkE3--;
    }
    ((void (*)(ENEMY_WORK *))pl_mv05_t2_spr[(UCHAR)pEm->Type - 1])(pEm);
}

void pl_mv05_w0_t6(ENEMY_WORK *pEm)
{
    SHORT a;

    a = pEm->Water;
    if (a != 0 && a < pEm->Ground - 500 && !(pEm->Move_cnt & 1) && pEm->Move_cnt < 9) {
        Pl_water(pEm);
    }
    if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x400)) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x400);
    Leg_ik_set(pEm, 0x400);
}

void pl_mv05_w1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    pl_mv05_w1_tbl[pEm->Routine_2](pEm, pKan, pSeq, Key);
}

void pl_mv05_w1_t0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    LONG Found;
    VECTOR *pPos;
    ENEMY_WORK *pT;
    ULONG Ret;

    switch (pEm->Routine_3) {
    case 0:
        Found = 0;
        pEm->Routine_3 = 1;
        pEm->Timer0 = 7;
        *(ULONG *)&pEm->Sub_move_no = EM_SET_MOVE(pEm, 0, 0, 7);
        pEm->UnkE3 = 0;
        pEm->UnkE2 = 0;
        pEm->Neck_flg |= 0x12;
        if (Key & 0x400) {
            pPos = Om_target_near(pEm, 0);
            if (pPos != 0) {
                pEm->Unk164 = pPos->vx;
                pEm->Unk168 = pPos->vz;
                pEm->UnkE3 = 1;
                Found = 1;
                pEm->UnkE2 = 2;
            }
        }
        if (!Found) {
            Ret = Em_ck(pEm, (UCHAR)pEm->Type == 1 ? 3000 : 0);
            if ((UCHAR)pEm->Type == 1 && (SHORT)Ret == 0) {
                Em_ck(pEm, 0);
            }
            if (pEm == pEm->pEnemy_ptr) {
                pEm->UnkE2 = 1;
            }
        }
        Pl_aim_dir = 0;
        if ((USHORT)(pEm->Spd_x + 10) > 20) {
            Snd_se_walk(0, 11, pEm);
        }
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        if (pEm->Water != 0 && pEm->Water < pEm->Ground - 500) {
            Pl_water(pEm);
        }
    case 1:
        pEm->Be_flg |= 0x10000000;
        if (pEm->Move_cnt < 5 && (UCHAR)pEm->Type >= 2 && pEm->Life >= 0x15 && (LONG)pEm->Be_flg < 0) {
            EM_SET_RTN(pEm, 1, 7, 2, 0);
            Pl_mv[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
            return;
        }
        if ((Key & 0x20) && (signed char)pEm->UnkE3 == 0) {
            Em_ck(pEm, 3000);
            pEm->UnkE3 = 1;
            if (pEm == pEm->pEnemy_ptr) {
                pEm->UnkE2 = 1;
            }
        }
        if (!(Main.Unk196 & 0x40)) {
            if ((signed char)pEm->UnkE2 == 0) {
                pT = pEm->pEnemy_ptr;
                Goto00(pEm, pT->Pos_x, pT->Pos_z, (UCHAR)pEm->Type == 1 ? 0x100 : 0xC4);
            } else if (pEm->UnkE2 == 2) {
                Goto00(pEm, pEm->Unk164, pEm->Unk168, 0xC4);
            }
        }
        if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set(pEm, 0x200);
        if ((Pl_wep_frame_tbl[(UCHAR)pEm->Type - 1][0] & 0x7F) < pEm->Move_cnt && (Key & 0x20)) {
            *(USHORT *)&pEm->Routine_2 = 1;
            Pl_aim_dir = 2;
            return;
        }
        if ((Pl_wep_frame_tbl[(UCHAR)pEm->Type - 1][1] & 0x7F) < pEm->Move_cnt && (Key & 0x10)) {
            *(USHORT *)&pEm->Routine_2 = 1;
            Pl_aim_dir = 1;
        }
        break;
    }
}

SHORT func_8006D030_(ULONG Arg0) __asm__("Tama_decrease"); /* returns short here (canonical long); arg 1 unsigned long (canonical UC) */
signed char func_8006CF0C_c(ULONG Arg0) __asm__("Tama_reload"); /* arg 1 unsigned long (canonical UC) */

void pl_mv05_w1_t1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Sub_move_no = EM_MOVE(pEm) = Pl_aim_dir * 2 + 0x70002;
        G.Room_player_flg |= 0x18;
    case 1:
        Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set(pEm, 0x200);
        break;
    }
    if ((G.Key_trg & 0x800) && !(Main.Unk196 & 0x40)) {
        *(USHORT *)&pEm->Routine_2 = 5;
        return;
    }
    if (!(Key & 0x500)) {
        *(USHORT *)&pEm->Routine_2 = 3;
        return;
    }
    if ((Key & 4) && Pl_aim_dir != 2) {
        Pl_aim_dir = 2;
        pEm->Routine_3 = 0;
    }
    if ((Key & 1) && Pl_aim_dir != 1) {
        Pl_aim_dir = 1;
        pEm->Routine_3 = 0;
    }
    if (!(Key & 5) && Pl_aim_dir != 0) {
        Pl_aim_dir = 0;
        pEm->Routine_3 = 0;
    }
    if (Key & 0x40) {
        if (func_8006D030_(0) != -1) {
            *(USHORT *)&pEm->Routine_2 = 2;
            pEm->Be_flg |= 0x10000000;
            if ((UCHAR)pEm->Type == 1 && (Key & 0x20)) {
                pEm->Be_flg &= 0x7FFFFFFF;
            }
            if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
                EM_SET_RTN(pEm, 1, 7, 2, 0);
                Pl_mv[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
            }
            return;
        }
        if (G.Key_trg & 0x40) {
            if (func_8006CF0C_c(0) == 0) {
                *(USHORT *)&pEm->Routine_2 = 4;
            } else {
                Snd_se_stad(0x10101, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
            }
        }
    }
    if (Key & 8) {
        pEm->Cdir_y -= Pl_wep_turn_tbl[(UCHAR)pEm->Type - 1] & 0x7F;
    }
    if (Key & 2) {
        pEm->Cdir_y += Pl_wep_turn_tbl[(UCHAR)pEm->Type - 1] & 0x7F;
    }
}

void pl_mv05_w1_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    SHORT a;
    SVECTOR Dummy;

    switch (pEm->Routine_3) {
    case 0:
        EM_MOVE(pEm) = Pl_aim_dir * 2 + 0x30001;
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Sub_move_no = Pl_aim_dir * 2 + 0x30001;
    case 1:
        a = pEm->Water;
        if (a != 0 && a < pEm->Ground - 500 && !(pEm->Move_cnt & 3) && pEm->Move_cnt < 10) {
            Pl_water(pEm);
        }
        ((void (*)(ENEMY_WORK *))pl_mv05_t2_spr[(UCHAR)pEm->Type - 1])(pEm);
        if (!(Pl_wep_turn_tbl[(UCHAR)pEm->Type - 1] & 0x80)) {
            Spd_get(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0);
        }
        if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x400)) {
            *(USHORT *)&pEm->Routine_2 = 1;
            pEm->Spd_x = 0;
        }
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x400);
        Leg_ik_set(pEm, 0x200);
        if (!(Pl_wep_turn_tbl[(UCHAR)pEm->Type - 1] & 0x80)) {
            Add_speedXZ(pEm, 0);
        }
        break;
    }
    if (!(Key & 0x500) && (Pl_wep_frame_tbl[(UCHAR)pEm->Type - 1][2] & 0x7F) < pEm->Move_cnt) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
}

void pl_mv05_w1_t3(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    LONG a;

    switch (pEm->Routine_3) {
    case 0:
        a = pEm->Water;
        pEm->Routine_3 = 1;
        EM_SET_MOVE(pEm, 0, 0, 7);
        *(ULONG *)&pEm->Sub_move_no = 0x70000;
        pEm->Spd_x = 0;
        if (a != 0 && a < pEm->Ground - 500) {
            Pl_water(pEm);
        }
    case 1:
        if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x10200)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Neck_flg &= 0xED;
            G.Status_flg &= ~0x40;
            G.Status_flg &= ~0x100;
        }
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x10200);
        Leg_ik_set(pEm, 0x200);
        break;
    }
}

void pl_mv05_w1_t4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    ULONG n;
    SVECTOR Sv;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        EM_SET_MOVE(pEm, 7, 0, 7);
        *(ULONG *)&pEm->Sub_move_no = 0x70007;
    }
    if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200)) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
    Leg_ik_set(pEm, 0x200);
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, &pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
    }
    if ((*pEm->pNow_seq & 0x400) && (pEm->Status_flg & 0x400)) {
        if ((UCHAR)pEm->Type == 5) {
            Esp_call(0x302, 0x08000000, &pEm->pSin_parts_ptr[4].Workm, 0, 0);
        } else {
            Mag_set(pEm);
        }
        pEm->Status_flg &= ~0x400;
    }
    if (*pEm->pNow_seq & 0x200) {
        Tama_reload(1);
        pEm->Status_flg |= 0x400;
    }
}

void pl_mv05_w1_t5(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        Pl_target_next(pEm);
        pEm->Be_flg |= 0x2000000;
        pT = pEm->pEnemy_ptr;
        if (pT == pEm) {
            *(USHORT *)&pEm->Routine_2 = 1;
            break;
        }
        pEm->UnkE3 = 12;
    case 1:
        pEm->Be_flg |= 0x2000000;
        pT = pEm->pEnemy_ptr;
        if (pT != pEm) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xD8);
        }
        if (!Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x10)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        if (--*(signed char *)&pEm->UnkE3 == -1) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        break;
    }
}

/* aim at the next live enemy in view after the current pEnemy_ptr (or the player itself) */
void Pl_target_next(ENEMY_WORK *pEm)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK **ppT;
    ENEMY_WORK *pT;
    ENEMY_WORK *pCur;
    LONG Idx;
    LONG Found;
    LONG Cnt;
    LONG Start;
    LONG Sel;
    LONG Prev;
    VECTOR Tgt;

    Idx = 2;
    Sel = 0;
    Prev = 0;
    if (pG->Em_num == 0) {
        pEm->pEnemy_ptr = pEm;
        return;
    }
    Found = 0;
    ppT = &pG->pEnemy[2];
    Cnt = 0;
    pCur = pEm->pEnemy_ptr;
    do {
        pT = *ppT;
        if (pCur == pT) {
            Found = Idx;
        }
        if (pT->Be_flg & 1) {
            Cnt++;
        }
    } while (Idx++, ++ppT != pG->ppEnemy_end);
    if (Cnt == 0) {
        pEm->pEnemy_ptr = pEm;
        return;
    }
    Idx = 2;
    if (Found != 0) {
        Idx = Found;
    }
    Start = Idx;
    Idx = Start + 1;
    if (Idx == 42) {
        Idx = 2;
    }
    do {
        pT = G.pEnemy[Idx];
        if (pT->Be_flg & 1) {
            Tgt.vx = pT->pSin_parts_ptr[pT->Neck_no].Workm.t[0];
            Tgt.vy = pT->pSin_parts_ptr[pT->Neck_no].Workm.t[1];
            Tgt.vz = pT->pSin_parts_ptr[pT->Neck_no].Workm.t[2];
            Cnt--;
            if (pT->Life >= 0 && Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400) == 0
                && Sca_ck_line(WORKM_POS_V(&pEm->pSin_parts_ptr[pEm->Neck_no]), &Tgt, 0x40, 1) == 0
                && !(pT->Type & 0xC000)) {
                Prev = Sel;
                Sel = Idx;
            }
        }
        if ((ENEMY_WORK **)G.pEnemy[Idx] == G.ppEnemy_end) {
            Idx = 2;
        } else {
            Idx++;
        }
    } while (Cnt != 0);
    if (Sel == 0) {
        pEm->pEnemy_ptr = pEm;
        return;
    }
    if (Start == Sel && Prev != 0) {
        Sel = Prev;
    }
    pEm->pEnemy_ptr = G.pEnemy[Sel];
}

/* pl_mv05_w0_tbl slot 6: turn to the point pUnk15C, upper body played separately */
void pl_mv05_w_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    LONG *pPos;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_SET_MOVE(pEm, 2, 0, 7);
        *(ULONG *)&pEm->Sub_move_no = 0x70002;
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        pEm->Neck_flg |= 0x12;
        Pl_aim_dir = 0;
    case 1:
        pPos = pEm->pUnk15C;
        Goto00(pEm, pPos[0], pPos[2], 0x100);
        Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200);
        if (pEm->Hokan_flg == 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set(pEm, 0x200);
        break;
    case 2:
        EM_SET_MOVE(pEm, 2, 0, 7);
        pEm->Routine_3 = 3;
    case 3:
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
}

signed char func_8006CF0C_c(ULONG Arg0) __asm__("Tama_reload"); /* arg 1 unsigned long (canonical UC) */

void pl_mv05_w0_t8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    pEm->Be_flg |= 0x20000000;
    switch (pEm->Routine_3) {
    case 0:
        EM_SET_MOVE(pEm, 13, 0, 7);
        pEm->Routine_3 = 1;
        pEm->Spd_x = 1;
        pEm->Neck_flg |= 0x12;
        Pl_aim_dir = 0;
        if (!(Pl_wep_flg[(UCHAR)pEm->Type] & 0x80)) {
            EM_SET_MOVE(pEm, 17, 0, 7);
            pEm->Spd_x = 0;
        }
    case 1:
        if (pEm->Spd_x != 0) {
            Goto00(pEm, pEm->pUnk15C[0], pEm->pUnk15C[2], 0x80);
        }
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            pEm->Routine_3 = 2;
            EM_SET_MOVE(pEm, 15, 0, 0);
            pEm->UnkE3 = 0xF;
            pEm->Spd_x = 0;
            if (!(Pl_wep_flg[(UCHAR)pEm->Type] & 0x80)) {
                *(USHORT *)&pEm->Routine_2 = 6;
            }
        }
        break;
    case 2:
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x400);
        if ((G.Key_trg & 0x800) || !(Key & 0x500) || (signed char)pEm->UnkE3 == 0 || (G.Key_trg & 5)) {
            *(USHORT *)&pEm->Routine_2 = 10;
            break;
        }
        if (Key & 0x40) {
            if (func_8006D030_(0) != -1) {
                *(USHORT *)&pEm->Routine_2 = 9;
                pEm->UnkE3--;
                break;
            }
            if (G.Key_trg & 0x40) {
                if (func_8006CF0C_c(0) == 0) {
                    *(USHORT *)&pEm->Routine_2 = 10;
                } else {
                    Snd_se_stad(0x10101, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
                }
            }
        }
        if (Key & 8) {
            pEm->Cdir_y -= 0x10;
        }
        if (Key & 2) {
            pEm->Cdir_y += 0x10;
        }
        break;
    }
}

void pl_mv05_w0_t9(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        EM_SET_MOVE(pEm, 14, 0, 0);
        pEm->Routine_3 = 1;
    case 1:
        pl_mv05_t2_spr[(UCHAR)pEm->Type - 1](pEm, pKan, pSeq, Key);
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 0x208;
            EM_SET_MOVE(pEm, 15, 0, 0);
        }
        break;
    }
}

void pl_mv05_w0_t10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        EM_SET_MOVE(pEm, 16, 0, 0);
        pEm->Routine_3 = 1;
        pEm->Spd_x = 0;
    case 1:
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        break;
    }
}

/* lower the weapon and go back to standing (no table refers to it) */
void pl_mv05_w_end(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        EM_SET_MOVE(pEm, 17, 0, 0);
        pEm->Routine_3 = 1;
        pEm->Spd_x = 0;
    case 1:
        if (Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            pEm->Neck_flg &= 0xED;
            G.Status_flg &= ~0x100;
            G.Status_flg &= ~0x40;
        }
        break;
    }
}

void pl_mv05_w2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    pl_mv05_w2_tbl[pEm->Routine_2](pEm, pKan, pSeq, Key);
}

void pl_mv05_w2_t1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    SHORT Mv[3] = { 3, 6, 9 };

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        EM_MOVE(pEm) = Mv[Pl_aim_dir] + 0x70000;
        *(ULONG *)&pEm->Sub_move_no = Mv[Pl_aim_dir] + 0x70000;
        G.Room_player_flg |= 0x18;
    }
    Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200);
    pEm->Be_flg |= 0x2000000;
    Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
    Leg_ik_set(pEm, 0x200);
    if (!(Key & 0x500)) {
        *(USHORT *)&pEm->Routine_2 = 3;
        return;
    }
    if ((UCHAR)pEm->Type != 11) {
        if ((Key & 0x10) && Pl_aim_dir != 1) {
            pEm->Routine_3 = 0;
            Pl_aim_dir = 1;
            return;
        }
        if ((Key & 0x20) && Pl_aim_dir != 2) {
            pEm->Routine_3 = 0;
            Pl_aim_dir = 2;
            return;
        }
        if (!(Key & 0x30) && Pl_aim_dir != 0) {
            pEm->Routine_3 = 0;
            Pl_aim_dir = 0;
            return;
        }
    }
    if ((G.Key_trg & 0x800) && !(Main.Unk196 & 0x40)) {
        *(USHORT *)&pEm->Routine_2 = 5;
    }
    if (Key & 0x40) {
        if (func_8006D030_(0) != -1) {
            *(USHORT *)&pEm->Routine_2 = 2;
            if (!(G.Key_trg & 0x40)) {
                if (!((Rnd_l() >> (Rnd_l() & 3)) & 1)) {
                    return;
                }
            }
            pEm->Be_flg |= 0x10000000;
            if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
                EM_SET_RTN(pEm, 1, 7, 2, 0);
                Pl_mv[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
            }
            return;
        }
        if (G.Key_trg & 0x40) {
            if (func_8006CF0C_c(0) == 0) {
                *(USHORT *)&pEm->Routine_2 = 4;
            } else {
                Snd_se_stad(0x10101, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
            }
        }
    }
    if (Key & 8) {
        pEm->Cdir_y -= 0x20;
    }
    if (Key & 2) {
        pEm->Cdir_y += 0x20;
    }
}

#define RND() ((signed char)Rnd())
LONG func_800455B4_() __asm__("Pl_wep_eff_parts_set"); /* unprototyped call */

void Leg_ik_set_ul(ENEMY_WORK *pEm, ULONG Flg) __asm__("Leg_ik_set"); /* arg 2 unsigned long (canonical long) */

void pl_mv05_w2_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    SHORT Mv0[3] = { 1, 4, 7 };
    SHORT Mv1[3] = { 2, 5, 8 };

    switch (pEm->Routine_3) {
    case 0:
        EM_MOVE(pEm) = Mv0[Pl_aim_dir] + 0x70000;
        *(ULONG *)&pEm->Sub_move_no = Mv0[Pl_aim_dir] + 0x70000;
        Pad_vib_fade_set(10, 0, Pl_vib_tbl[(UCHAR)pEm->Type - 2][2], 0);
    case 1:
        pEm->Move_no = Mv0[Pl_aim_dir];
        pEm->Hokan_flg = 7;
        pEm->Sub_move_no = Mv0[Pl_aim_dir];
        pEm->Sub_hokan_flg = 7;
        pEm->Routine_3 = 2;
        pEm->UnkE3 = 9;
    case 2:
        if ((pEm->Move_cnt % 3) == 0 && func_8006D030_(0) == -1) {
            pEm->Routine_3 = 3;
            pEm->Be_flg |= 0x2000000;
            return;
        }
        G.Room_player_flg |= 1;
        ((void (*)(ENEMY_WORK *))pl_mv05_t2_spr[(UCHAR)pEm->Type - 1])(pEm);
        Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set_ul(pEm, 0x200);
        if (!(Key & 0x500)) {
            *(USHORT *)&pEm->Routine_2 = 3;
            return;
        }
        if ((Key & 0x10) && Pl_aim_dir != 1) {
            pEm->Routine_3 = 1;
            Pl_aim_dir = 1;
            return;
        }
        if ((Key & 0x20) && Pl_aim_dir != 2) {
            pEm->Routine_3 = 1;
            Pl_aim_dir = 2;
            return;
        }
        if (!(Key & 0x30) && Pl_aim_dir != 0) {
            pEm->Routine_3 = 1;
            Pl_aim_dir = 0;
            return;
        }
        if (!(Key & 0x40)) {
            pEm->Routine_3 = 3;
            return;
        }
        if (pEm->Water != 0 && pEm->Water < pEm->Ground - 500 && !(pEm->Move_cnt & 3)) {
            Pl_water(pEm);
        }
        break;
    case 3:
        EM_MOVE(pEm) = Mv1[Pl_aim_dir] + 0x70000;
        *(ULONG *)&pEm->Sub_move_no = Mv1[Pl_aim_dir] + 0x70000;
        pEm->Routine_3 = 4;
    case 4:
        if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set_ul(pEm, 0x200);
        if (Key & 0x40) {
            if (func_8006D030_(0) != -1) {
                *(USHORT *)&pEm->Routine_2 = 2;
                return;
            }
            if (G.Key_trg & 0x40) {
                if (Tama_reload(0) == 0) {
                    *(USHORT *)&pEm->Routine_2 = 4;
                } else {
                    Snd_se_stad(0x10101, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
                }
            }
        }
        break;
    }
    if (Key & 8) {
        pEm->Cdir_y -= 8;
    }
    if (Key & 2) {
        pEm->Cdir_y += 8;
    }
}

void Leg_ik_set_ul(ENEMY_WORK *pEm, ULONG Flg) __asm__("Leg_ik_set"); /* arg 2 unsigned long (canonical long) */

void pl_mv05_w2_t4(ENEMY_WORK *pEm)
{
    ULONG n;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        EM_SET_MOVE(pEm, 10, 0, 7);
        *(ULONG *)&pEm->Sub_move_no = 0x7000A;
    }
    if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200) != 0) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
    Leg_ik_set_ul(pEm, 0x200);
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
    }
    if ((*pEm->pNow_seq & 0x400) && (pEm->Status_flg & 0x400)) {
        Mag_set(pEm);
        pEm->Status_flg &= ~0x400;
    }
    if (*pEm->pNow_seq & 0x200) {
        Tama_reload(1);
        pEm->Status_flg |= 0x400;
    }
}

void pl_mv05_w0b(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    pl_mv05_w0b_tbl[pEm->Routine_2](pEm, pKan, pSeq, Key);
}

void pl_mv05_w0b_t2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    PARTS_W *pP;
    SVECTOR Sv;

    switch (pEm->Routine_3) {
    case 0:
        EM_SET_MOVE(pEm, 1, 0, 7);
        *(ULONG *)&pEm->Sub_move_no = 0x70001;
        pEm->Routine_3 = 1;
        Snd_se_stad(0x10112, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
        Pad_vib_fade_set(15, 0, Pl_vib_tbl[(UCHAR)pEm->Type - 2][2], 0);
    case 1:
        Pl_wep_flag_set(pEm);
        if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200)) {
            EM_SET_MOVE(pEm, 2, 0, 7);
            *(ULONG *)&pEm->Sub_move_no = 0x70002;
            pEm->Routine_3 = 2;
        }
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set_ul(pEm, 0x200);
        pP = pEm->pSin_parts_ptr;
        if ((pEm->Move_cnt % 6) == 0) {
            Sv.vx = -0x6C;
            Sv.vy = 0x4B0;
            Sv.vz = 0;
            Esp_call(0x603, (USHORT)(signed char)Rnd() | 0x10000000, &pP[4].Workm, &Sv, 0);
        }
        if ((pEm->Move_cnt % 3) == 0) {
            Sv.vx = -0xE6;
            Sv.vy = 0x2BC;
            Sv.vz = 0;
            Esp_call(0x503, (USHORT)(signed char)Rnd() | 0x1E000000, &pP[4].Workm, &Sv, 0);
        }
        break;
    case 2:
        Pl_wep_flag_set(pEm);
        G.Room_player_flg |= 1;
        ((void (*)(ENEMY_WORK *))pl_mv05_t2_spr[(UCHAR)pEm->Type - 1])(pEm);
        Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set_ul(pEm, 0x200);
        if (!(Key & 0x500)) {
            *(USHORT *)&pEm->Routine_2 = 3;
            Snd_se_stad(0x10113, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
            return;
        }
        if (!(Key & 0x40)) {
            pEm->Routine_3 = 3;
            return;
        }
        if (pEm->Water != 0 && pEm->Water < pEm->Ground - 500 && !(pEm->Move_cnt & 3)) {
            Pl_water(pEm);
        }
        break;
    case 3:
        EM_SET_MOVE(pEm, 4, 0, 7);
        *(ULONG *)&pEm->Sub_move_no = 0x70004;
        pEm->Routine_3 = 4;
        pEm->Spd_x = 0;
        Snd_se_stad(0x10113, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
    case 4:
        if (Joint_move(pEm, G.Player.pUnk18C_kan_t_ptr, G.Player.pUnk188_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        Leg_ik_kan(G_PLAYER, G.Player.pUnk184_kan_t_ptr, G.Player.pUnk180_seq_t_ptr, 0x200);
        Leg_ik_set_ul(pEm, 0x200);
        if (Key & 0x40) {
            *(USHORT *)&pEm->Routine_2 = 0x102;
            return;
        }
        break;
    case 5:
        break;
    }
    if (Key & 8) {
        pEm->Cdir_y -= 8;
    }
    if (Key & 2) {
        pEm->Cdir_y += 8;
    }
}

void Pl_wep_ready_reset(ENEMY_WORK *pEm)
{
    PARTS_W *pParts = &pEm->pSin_parts_ptr[22];

    pParts->Timer0 = 0;
    pParts->Timer2 = 0;
    pParts->Timer3 = 0;
}

void Pl_wep_recoil_6(ENEMY_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)((UCHAR)pEm->Type - 6) < 4) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Timer3 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl0[pWep->Timer2];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Timer3 = 0xFF;
                return;
            }
            pWep->Timer2++;
            RotMatrix_gte(CDIR_SV(&pParts[22]), (MATRIX *)pParts[22].M);
        }
    }
}

void Pl_wep_recoil_10(ENEMY_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0x10) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Timer3 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl1[pWep->Timer2];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Timer3 = 0xFF;
                return;
            }
            pWep->Timer2++;
            RotMatrix_gte(CDIR_SV(&pParts[22]), (MATRIX *)pParts[22].M);
        }
    }
}

void Pl_wep_flag_set(ENEMY_WORK *pEm)
{
    pEm->pSin_parts_ptr[22].Timer2 = 1;
}

void Pl_wep_spin_0b(ENEMY_WORK *pEm)
{
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0xB) {
        pWep = &pEm->pSin_parts_ptr[22];
        if (pWep->Timer2 == 0) {
            pWep->Timer3 -= 8;
            if (pWep->Timer3 < 0) {
                pWep->Timer3 = 0;
            }
        } else {
            pWep->Timer3 += 0x20;
            if (pWep->Timer3 > 0x200) {
                pWep->Timer3 = 0x200;
            }
        }
        pWep->Cdir_y += pWep->Timer3;
        RotMatrix(CDIR_SV(pWep), PW_M(pWep));
        pWep->Timer2 = 0;
    }
}

void pl_w01_t2_move(ENEMY_WORK *pEm)
{
    ULONG n;
    PARTS_W *pP;

    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        pP = pEm->pSin_parts_ptr;
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
    }
}

void pl_w02_t2_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        Esp_call(2, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x1C2;
        Sv.vz = 0;
        Esp_call(1, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        Esp_call(3, 0x10000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

/* pl_mv05_t2_spr slot 2 (weapon 3): firing motion, RE2 naming */
void pl_w03_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ULONG n;
    ESP_WORK *pEsp;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x5DC;
        Sv.vz = 0;
        Esp_call(1, 0x28000000, pM, &Sv, 0);
        Sv.vy = 0x78A;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Esp_call(0x801, 0x24000000, pM, &Sv, &Dsv);
        Sv.vx = 0x82;
        Sv.vy = 0x3E8;
        Sv.vz = 0;
        Esp_call(3, 0x2D000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x640;
        Sv.vz = 0;
        Esp_call(0x703, 0x34000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x514;
        Sv.vz = 0;
        Esp_call(0x901, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x6A4;
        Sv.vz = 0;
        Esp_call(0x901, (USHORT)pEm->Cdir_y | 0x13000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = -0xC8;
        Dsv.vx = (RND() >> 1) - 0x8C;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        Esp_call(0xA01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = 0xC8;
        Dsv.vx = (RND() >> 1) + 0x8C;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        Esp_call(0xA01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x12C;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x48C;
        Esp_call(0xA01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = -0x64;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x374;
        Esp_call(0xA01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
    }
    if (pEm->Move_cnt == 1 || pEm->Move_cnt == 2) {
        Weapon_at_ck(&pP[4].Workm, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = 0x9D;
        Sv.vy = 0x1EA;
        Sv.vz = 0;
        Esp_call(0x202, (USHORT)(pEm->Cdir_y + 0x400) | 0xB000000, &pP[4].Workm, &Sv, 0);
        Sv.vx = 0xC8;
        Sv.vy = 0x1FE;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
}

/* firing motion of weapon 4 */
void pl_w04_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x21C;
        Sv.vz = 0;
        Esp_call(1, 0x18000000, pM, &Sv, 0);
        Sv.vy = 0x33E;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Esp_call(0x801, 0x10000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x12C;
        Sv.vz = 0;
        Esp_call(3, 0x1A000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        Esp_call(0x703, 0x1E000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x96;
        Sv.vy = 0x33E;
        Sv.vz = -0x64;
        Dsv.vx = (RND() >> 2) - 0x82;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 2) - 0x400;
        Esp_call(0xB01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x96;
        Sv.vy = 0x33E;
        Sv.vz = 0x64;
        Dsv.vx = (RND() >> 2) + 0x82;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 2) - 0x400;
        Esp_call(0xB01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0xFA;
        Sv.vy = 0x33E;
        Sv.vz = 0;
        Dsv.vx = RND() >> 2;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 2) - 0x482;
        Esp_call(0xB01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x32;
        Sv.vy = 0x33E;
        Sv.vz = 0;
        Dsv.vx = RND() >> 2;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 2) - 0x37E;
        Esp_call(0xB01, (RND() + 0x1000) << 16, &pP[4].Workm, &Sv, &Dsv);
    }
}

/* firing motion of weapon 5 */
void pl_w05_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        func_8006D030_(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        Esp_call(0x1000402, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        Esp_call(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        Esp_call(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
}

/* firing motion of weapon 6 */
void pl_w06_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        func_8006D030_(1);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        Esp_call(0x1000702, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        Esp_call(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        Esp_call(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
}

/* firing motion of weapon 7 */
void pl_w07_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        func_8006D030_(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        Esp_call(0x1000802, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        Esp_call(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        Esp_call(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
}

/* firing motion of weapon 8 */
void pl_w08_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        func_8006D030_(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        Esp_call(0x2000902, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        Esp_call(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        Esp_call(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
}

/* firing motion of weapon 9 */
void pl_w09_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = &pEm->pSin_parts_ptr[4];
        Snd_se_stad(0x10100, WORKM_POS_V(&pEm->pSin_parts_ptr[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        switch ((SHORT)(func_8006D030_(1) % 4)) {
        case 0:
            Sv.vx = 0x16A;
            Sv.vy = 0x44C;
            Sv.vz = -0x38;
            break;
        case 1:
            Sv.vx = 0x188;
            Sv.vy = 0x44C;
            Sv.vz = 0x5B;
            break;
        case 2:
            Sv.vx = 0xBD;
            Sv.vy = 0x44C;
            Sv.vz = -0x38;
            break;
        case 3:
            Sv.vx = 0xBD;
            Sv.vy = 0x44C;
            Sv.vz = 0x5B;
            break;
        }
        pM = &pP->Workm;
        Esp_call(0x1000502, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x17C;
        Sv.vy = 0x4B0;
        Sv.vz = 0;
        Esp_call(1, 0x38000000, pM, &Sv, 0);
        Sv.vx = 0x17C;
        Sv.vy = 0x320;
        Sv.vz = 0;
        Esp_call(3, 0x50000000, pM, &Sv, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x30000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 5;
            pEsp->Spd_y = -0x50;
        }
        Sv.vx = 0xDC;
        Sv.vy = -0x3E8;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x30000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 5;
            pEsp->Spd_y = -0x50;
        }
        Game.Plight_flg = 1;
    }
    G.Status_flg &= ~0x40;
}

/* firing motion of weapon 0xA */
void pl_w0a_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    UCHAR Cnt;

    pP = pEm->pSin_parts_ptr;
    Cnt = pEm->Move_cnt % 3;
    if (Cnt == 0) {
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = -0x6C;
        Sv.vy = 0x528;
        Sv.vz = 0;
        G.Room_player_flg |= 1;
        Esp_call(1, 0x18000000, pM, &Sv, 0);
        Sv.vy = 0x60E;
        Dsv.vz = -0x400;
        Sv.vx = -0x6C;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Esp_call(0x801, 0x18000000, pM, &Sv, &Dsv);
        Sv.vx = -0x190;
        Sv.vy = 0x96;
        Sv.vz = 0;
        Esp_call(0xC02, (USHORT)(pEm->Cdir_y - 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = -0xE6;
        Sv.vy = 0x2BC;
        Sv.vz = 0;
        Esp_call(0x503, (USHORT)(signed char)Rnd() | 0x1E000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
        Pl_weapon_vib(pEm);
    }
    if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
        Sv.vx = -0x6C;
        Sv.vy = 0x4B0;
        Sv.vz = 0;
        Esp_call(0x603, (USHORT)(signed char)Rnd() | 0x10000000, &pP[4].Workm, &Sv, 0);
    }
    if (Cnt == 1) {
        Weapon_at_ck(&pP[4].Workm, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
    }
}

/* firing motion of weapon 0xB */
void pl_w0b_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        func_800455B4_();
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x318;
        Sv.vz = 0;
        Esp_call(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x1EC;
        Sv.vz = 0;
        Esp_call(3, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x37C;
        Sv.vz = 0;
        Esp_call(0x703, 0x30000000, pM, &Sv, 0);
        Sv.vx = 0x97;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x8000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x14;
            pEsp->Spd_y = -0x14;
            pEsp->Spd_z = -0x14;
        }
        pEsp = Esp_call(3, 0x8000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x14;
            pEsp->Spd_y = -0x14;
            pEsp->Spd_z = 0x14;
        }
        pEsp = Esp_call(3, 0x10000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x1E;
            pEsp->Spd_y = -0x1E;
            pEsp->Spd_z = -0x1E;
        }
        pEsp = Esp_call(3, 0x10000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x1E;
            pEsp->Spd_y = -0x1E;
            pEsp->Spd_z = 0x1E;
        }
        Game.Plight_flg = 1;
    }
}

/* firing motion of weapon 0xC */
void pl_w0c_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        Esp_call(2, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x5F;
        Sv.vy = 0x258;
        Sv.vz = 0x20;
        Esp_call(1, (USHORT)pEm->Cdir_y | 0x13000000, pM, &Sv, 0);
        Sv.vx = 0x5F;
        Sv.vy = 0x1F4;
        Sv.vz = 0x20;
        Esp_call(3, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

/* firing motion of weapon 0xD */
void pl_w0d_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ESP_WORK *pEsp;
    UCHAR Cnt;

    Cnt = pEm->Move_cnt;
    pP = &pEm->pSin_parts_ptr[4];
    if (Cnt == 0 || Cnt == 4 || Cnt == 7) {
        Sv.vz = 0x14;
        pM = &pP->Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Esp_call(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        Esp_call(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x528;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        Esp_call(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        Esp_call(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        Esp_call(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        Esp_call(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        Esp_call(0x603, (USHORT)(signed char)Rnd() | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = Esp_call(0x603, (USHORT)(signed char)Rnd() | 0x10000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Snd_se_stad(0x10100, WORKM_POS_V(pP), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        func_8006D030_(1);
        Game.Plight_flg = 1;
    }
}

/* firing motion of weapon 0xE */
void pl_w0e_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    PARTS_W *pO;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ESP_WORK *pEsp;

    if ((UCHAR)(pEm->Move_cnt % 3) == 0) {
        pP = pEm->pSin_parts_ptr;
        pO = &pP[4];
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        func_8006D030_(1);
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        Esp_call(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        Esp_call(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x528;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        Esp_call(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        Esp_call(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        Esp_call(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        Esp_call(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Pl_weapon_vib(pEm);
        Game.Plight_flg = 1;
    }
    if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        Esp_call(0x603, (USHORT)(signed char)Rnd() | 0x10000000, &pO->Workm, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = Esp_call(0x603, (USHORT)(signed char)Rnd() | 0x10000000, &pO->Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
}

/* firing motion of weapon 0xF */
void pl_w0f_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ULONG n;
    ESP_WORK *pEsp;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x50;
        Sv.vy = 0x33F;
        Sv.vz = 0xD;
        Esp_call(1, 0x20000000, pM, &Sv, 0);
        Sv.vy = 0x407;
        Dsv.vz = -0x400;
        Sv.vx = 0x50;
        Sv.vz = 0xD;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Esp_call(0x801, 0x1F000000, pM, &Sv, &Dsv);
        Sv.vx = 0x50;
        Sv.vy = 0x1A4;
        Sv.vz = 0xD;
        Esp_call(3, 0x25000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x3A3;
        Sv.vz = 0xD;
        Esp_call(0x703, 0x2D000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x2DB;
        Sv.vz = 0xD;
        Esp_call(0x901, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x3D5;
        Sv.vz = 0xD;
        Esp_call(0x901, (USHORT)pEm->Cdir_y | 0x13000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x50;
        Sv.vy = 0x407;
        Sv.vz = -0xBB;
        Dsv.vx = (RND() >> 1) - 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        Esp_call(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x50;
        Sv.vy = 0x407;
        Sv.vz = 0xD5;
        Dsv.vx = (RND() >> 1) + 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        Esp_call(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x118;
        Sv.vy = 0x407;
        Sv.vz = 0xD;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x4A0;
        Esp_call(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = -0x78;
        Sv.vy = 0x407;
        Sv.vz = 0xD;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x360;
        Esp_call(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Weapon_at_ck(&pP[4].Workm, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_at_ck(&pP[4].Workm, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x104;
        Sv.vz = 0xD;
        Esp_call(0x202, (USHORT)(pEm->Cdir_y + 0x800) | 0xB000000, &pP[4].Workm, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x104;
        Sv.vz = 0xD;
        pEsp = Esp_call(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
    if (pEm->Move_cnt == 9) {
        Pl_wep_ready_reset(pEm);
        Pad_vib_set0(2, 0);
        Pad_vib_set1(6, 0x96, 0);
    }
}

/* firing motion of weapon 0x10 */
void pl_w10_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        Esp_call(0x4000002, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x1C2;
        Sv.vz = 0;
        Esp_call(1, (USHORT)pEm->Cdir_y | 0x16000000, pM, &Sv, 0);
        Sv.vy = 0x2DA;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Esp_call(0x801, 0x10000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        Esp_call(3, 0x14000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x28A;
        Sv.vz = 0;
        Esp_call(3, 0x18000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

/* firing motion of weapon 0x11 */
void pl_w11_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ULONG n;
    ESP_WORK *pEsp;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        Weapon_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        Weapon_om_at_ck(pM, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x5DC;
        Sv.vz = 0;
        Esp_call(1, 0x2E000000, pM, &Sv, 0);
        Sv.vy = 0x78A;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Esp_call(0x801, 0x26000000, pM, &Sv, &Dsv);
        Sv.vx = 0x82;
        Sv.vy = 0x3E8;
        Sv.vz = 0;
        Esp_call(0x2303, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x3E8;
        Sv.vz = 0;
        Esp_call(3, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x514;
        Sv.vz = 0;
        Esp_call(0x703, 0x2D000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x76C;
        Sv.vz = 0;
        Esp_call(0x703, 0x34000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x640;
        Sv.vz = 0;
        Esp_call(0x901, (USHORT)pEm->Cdir_y | 0x14000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x76C;
        Sv.vz = 0;
        Esp_call(0x901, (USHORT)pEm->Cdir_y | 0x1B000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = -0x12C;
        Dsv.vx = (RND() >> 1) - 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        Esp_call(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = 0x12C;
        Dsv.vx = (RND() >> 1) + 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        Esp_call(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x190;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x4A0;
        Esp_call(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = -0xC8;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x360;
        Esp_call(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
    }
    if (pEm->Move_cnt == 1 || pEm->Move_cnt == 2) {
        Weapon_at_ck(&pP[4].Workm, Pl_wep_data[pEm->Id], (UCHAR)pEm->Type);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        Snd_se_stad(((n >> 12) - 1) | 0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        Sv.vx = 0x9D;
        Sv.vy = 0x1EA;
        Sv.vz = 0;
        Esp_call(0x5000202, (USHORT)(pEm->Cdir_y + 0x400) | 0xB000000, &pP[4].Workm, &Sv, 0);
        Sv.vx = 0xC8;
        Sv.vy = 0x1FE;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = 0x14;
            pEsp->Spd_y = -0x28;
        }
    }
    G.Status_flg &= ~0x40;
}

/* firing motion of weapon 0x12 */
void pl_w12_t2_move(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        func_800455B4_();
        func_8006D030_(1);
        Snd_se_stad(0x10100, WORKM_POS_V(&pP[4]), 0, 0);
        G.Room_player_flg |= 1;
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x318;
        Sv.vz = 0;
        Esp_call(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x1EC;
        Sv.vz = 0;
        Esp_call(3, 0x10000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x4A8;
        Sv.vz = 0;
        Esp_call(3, 0x1D000000, pM, &Sv, 0);
        Sv.vx = 0x97;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        pEsp = Esp_call(3, 0x6000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x14;
            pEsp->Spd_y = -0x14;
            pEsp->Spd_z = -0x14;
        }
        pEsp = Esp_call(3, 0x6000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x14;
            pEsp->Spd_y = -0x14;
            pEsp->Spd_z = 0x14;
        }
        pEsp = Esp_call(3, 0xD000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x1E;
            pEsp->Spd_y = -0x1E;
            pEsp->Spd_z = -0x1E;
        }
        pEsp = Esp_call(3, 0xD000000, pM, &Sv, 0);
        if (pEsp != ESP_NONE) {
            pEsp->Spd_x = -0x1E;
            pEsp->Spd_y = -0x1E;
            pEsp->Spd_z = 0x1E;
        }
        Game.Plight_flg = 1;
    }
}

void Pl_weapon_ch(ENEMY_WORK *pEm)
{
    PARTS_W *pW;
    PARTS_W *pTop;
    UCHAR *pData;
    LONG *pOfs;
    ULONG Cnt;
    UCHAR No;
    MD_OBJ *pTmd;
    TIM_IMAGE *pTim;
    TIM_IMAGE Tim;
    MD_OBJ *p;

    pW = pPl_emd_top;
    pData = (UCHAR *)&pW[4];
    Cd_read(Plw_no[pEm->Id] + (UCHAR)pEm->Type, (ULONG *)pData, 0, "WEP DATA");
    pOfs = (LONG *)(pData + *(LONG *)&pW[4]);
    No = pEm->Parts_num;
    Cnt = *(ULONG *)(pData + pOfs[7] + 4);
    pEm->Parts_num = No + Cnt;
    pEm->pSub0_seq_t_ptr = (ULONG *)(pData + pOfs[0]);
    pEm->pSub0_kan_t_ptr = (ULONG *)(pData + pOfs[1]);
    OpenTIM((ULONG *)(pData + pOfs[8]));
    pTim = ReadTIM(&Tim);
    pTim->prect->x = pEm->Tpage * 0x40 - 0x39C;
    pTim->prect->y = 0x1E0;
    pTim->prect->w = 0x1C;
    pTim->prect->h = 0x20;
    LoadImage(pTim->prect, pTim->paddr);
    DrawSync(0);
    pTim->crect->x = 0xF0;
    pTim->crect->y = pEm->Clut + 0x1E1;
    pTim->crect->w = 0x10;
    pTop = pW;
    LoadImage(pTim->crect, pTim->caddr);
    DrawSync(0);
    Mapping_md((ULONG *)(pData + pOfs[7]), pEm->Tpage, pEm->Clut);
    G.Player.pUnk180_seq_t_ptr = (ULONG *)(pData + pOfs[2]);
    G.Player.pUnk184_kan_t_ptr = (ULONG *)(pData + pOfs[3]);
    G.Player.pUnk1B4 = (ULONG *)(pData + pOfs[4]);
    G.Player.pUnk188_seq_t_ptr = (ULONG *)(pData + pOfs[5]);
    G.Player.pUnk18C_kan_t_ptr = (ULONG *)(pData + pOfs[6]);
    pTmd = (MD_OBJ *)(pData + pOfs[7] + 8);
    do {
        pW->UnkA2 = 0;
        pW->pSuper = &GsIDMATRIX;
        pW->Old_x = 0;
        pW->Pos_x = 0;
        pW->Old_y = 0;
        pW->Pos_y = 0;
        pW->Old_z = 0;
        pW->Pos_z = 0;
        pW->pTmd = pTmd;
        *(ULONG *)&pW->Workm.m[0][0] = 0x1000;
        *(ULONG *)&pW->Workm.m[0][2] = 0;
        *(ULONG *)&pW->Workm.m[1][1] = 0x1000;
        *(ULONG *)&pW->Workm.m[2][0] = 0;
        *(ULONG *)&pW->Workm.m[2][2] = 0x1000;
        pW->Workm.t[0] = 0;
        pW->Workm.t[1] = 0;
        pW->Workm.t[2] = 0;
        *(ULONG *)&pW->M[0][0] = 0x1000;
        *(ULONG *)&pW->M[0][2] = 0;
        *(ULONG *)&pW->M[1][1] = 0x1000;
        *(ULONG *)&pW->M[2][0] = 0;
        *(ULONG *)&pW->M[2][2] = 0x1000;
        pW->Attribute = 0;
        pW->Poly_rgb = 0x808080;
        *(ULONG *)&pW->Cdir_x = 0;
        pW->Cdir_z = 0;
        pW->Be_flg = 0;
        pW->Parts_no = No;
        *(SHORT *)&pW->Workm.t[0] = 0;
        pW->Timer0 = 0;
        pW->Timer1 = 0;
        pW->Timer2 = 0;
        pW->Timer3 = 0;
        No++;
        pW++;
        pTmd++;
    } while (--Cnt != 0);
    pW = pTop;
    pTop = &pEm->pSin_parts_ptr[4];
    p = pTop->pTmd;
    pTop->pTmd = pW->pTmd;
    Pl_wep_tmd = pW->pTmd;
    pW->pTmd = p;
    Pl_weapon_init[(UCHAR)pEm->Type](pEm);
    Snd_vab_load(1, (UCHAR)pEm->Type, (ULONG *)0x801FBF00, (ULONG *)0x801C1814);
}

void Weapon_cls(ENEMY_WORK *pEm)
{
    ULONG *pTmd = (ULONG *)Pl_wep_tmd; /* the n_obj word before the objects */
    PARTS_W *pP = pEm->pSin_parts_ptr;
    ULONG n;

    pP[4].pTmd = pP[21].pTmd;
    n = pTmd[-1];
    pEm->Parts_num = pEm->Parts_num - n;
}

void Weapon_parts_clr(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pEm->pSin_parts_ptr[4].pTmd = pEm->pSin_parts_ptr[21].pTmd;
    pP = &pEm->pSin_parts_ptr[21];
    i = ((ULONG *)Pl_wep_tmd)[-1]; /* MD_HEAD.n_obj */
    do {
        pP->Be_flg = 0;
        pP->UnkA2 = 0;
        pP++;
    } while (--i != 0);
}

void Pl_weapon_ch_init(ENEMY_WORK *pEm)
{
    pEm->pSin_parts_ptr[4].pTmd = Pl_wep_tmd;
    Pl_weapon_init[(UCHAR)pEm->Type](pEm);
}

void Non_init(ENEMY_WORK *pEm)
{
}

void Plw02_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0;
    pO->Pos_y = 0x96;
    pO->Pos_z = 0;
    pO->UnkA2 |= 0xB;
}

void Plw06_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO;

    Pl_wep_oya = &pP[4];
    pO = pEm->pSin_parts_ptr;
    Pl_wep_parts = &pO[22];
    pO[22].Be_flg = 1;
    pO = &pO[22];
    pO->Pos_x = 0x51;
    pO->Pos_y = 0x284;
    pO->pSuper = &pP[4].Workm;
    pO->Pos_z = 0x37;
    pO->pOya_parts = &pP[4];
    pO->Timer3 = 0xFF;
    pO->UnkA2 |= 0xB;
}

void Plw0b_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO;

    Pl_wep_oya = &pP[4];
    pO = pEm->pSin_parts_ptr;
    Pl_wep_parts = &pO[22];
    pO[22].Be_flg = 1;
    pO = &pO[22];
    pO->pSuper = &pP[4].Workm;
    pO->Pos_x = -0xC4;
    pO->pOya_parts = &pP[4];
    pO->Pos_y = 0x18B;
    pO->Pos_z = 0;
    pO->UnkA2 |= 0xB;
}

void Plw0c_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 1;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0x43;
    pO->Pos_y = 0x1B9;
    pO->Pos_z = 0x44;
    pO->UnkA2 |= 0xB;
}

void Plw0d_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0;
    pO->Pos_y = 0x96;
    pO->Pos_z = 0;
    pO->UnkA2 |= 0xB;
}

void Plw0e_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = -0x32;
    pO->Pos_y = 0x1B8;
    pO->Pos_z = 0xA;
    pO->UnkA2 |= 0xB;
}

void Plw10_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 1;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = -0x1D;
    pO->Pos_y = 0x7E;
    pO->Pos_z = 0xD;
    pO->Timer3 = 0xFF;
    pO->UnkA2 |= 0xB;
}

ULONG Em_ck(ENEMY_WORK *pEm, ULONG LL)
{
    LONG dx;
    LONG dz;
    ULONG Dist;
    ENEMY_WORK *pTgt;
    ENEMY_WORK **ppTgt;
    ULONG Min_dist;
    ULONG Min_dist_sp;
    ENEMY_WORK *pNear_sp;
    ENEMY_WORK *pNear;
    VECTOR Tgt_pt;
    tagGLOBAL *pG = &G;

    Min_dist = 0x7FFFFFFF;
    pNear = 0;
    Min_dist_sp = 0x7FFFFFFF;
    pNear_sp = 0;
    if (pG->Em_num == 0) {
        pEm->pEnemy_ptr = pEm;
        return 0;
    }
    ppTgt = &pG->pEnemy[2];
    if (LL != 0) {
        Min_dist = LL;
    }
    do {
        pTgt = *ppTgt;
        if ((pTgt->Be_flg & 1) && !(pTgt->Type & 0xC000) && pTgt->Life >= 0) {
            dx = (pEm->Pos_x - pTgt->Pos_x) >> 1;
            dz = (pEm->Pos_z - pTgt->Pos_z) >> 1;
            Dist = SquareRoot0(dx * dx + dz * dz);
            Tgt_pt.vx = pTgt->pSin_parts_ptr[pTgt->Neck_no].Workm.t[0];
            Tgt_pt.vy = pTgt->pSin_parts_ptr[pTgt->Neck_no].Workm.t[1];
            Tgt_pt.vz = pTgt->pSin_parts_ptr[pTgt->Neck_no].Workm.t[2];
            if (Sca_ck_line(WORKM_POS_V(&pEm->pSin_parts_ptr[pEm->Neck_no]), &Tgt_pt, 0x40, 1) == 0) {
                if (!(pTgt->Type & 0x2000)) {
                    if (Dist < Min_dist) {
                        Min_dist = Dist;
                        pNear = pTgt;
                        ppTgt++;
                        continue;
                    }
                } else if (Dist < Min_dist_sp) {
                    Min_dist_sp = Dist;
                    pNear_sp = pTgt;
                }
            }
        }
        ppTgt++;
    } while (ppTgt != pG->ppEnemy_end);
    if (pNear != 0) {
        pEm->pEnemy_ptr = pNear;
    } else {
        if (pNear_sp == 0) {
            pEm->pEnemy_ptr = pEm;
            return 0;
        }
        pEm->pEnemy_ptr = pNear_sp;
    }
    return 1;
}

/* is the point pSv (in pM's space) inside pEm's Hit_area box? */
LONG Hit_area_ck(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm)
{
    EM_HIT_AREA *pH;
    MATRIX *pHm;
    LONG H;
    LONG Y;
    LONG Y1;
    SHORT Dx;
    SHORT Dz;
    SHORT Dist;
    ULONG Ret;
    VECTOR V;

    pH = &pEm->Hit_area;
    pHm = pH->pM;
    H = pH->Top - pH->Bottom;
    Y = pHm->t[1] + pH->Top;
    ApplyMatrix(pM, pSv, &V);
    V.vx += pM->t[0];
    V.vy += pM->t[1];
    V.vz += pM->t[2];
    if ((ULONG)H < (ULONG)(Y - V.vy)) {
        return 0;
    }
    Dx = pHm->t[0] - V.vx;
    Dz = pHm->t[2] - V.vz;
    Dist = SquareRoot0(Dx * Dx + Dz * Dz);
    if (pSv->pad + pH->R < Dist) {
        return 0;
    }
    Y1 = Y;
    Y += (H * pH->Rate0) >> 8;
    Y1 += (H * pH->Rate1) >> 8;
    if (Y < V.vy) {
        Ret = pEm->Damage_flg & 0x4000;
        return Ret == 0;
    }
    if (V.vy >= Y1) {
        return 2;
    }
    return ((pEm->Damage_flg & 0x8000) == 0) << 2;
}

/* is pEm inside the attack area pA (pPos0 -> pPos1)? used by every enemy module */
LONG Hit_area_em_ck(LONG *pPos0, LONG *pPos1, HIT_AREA *pA, ENEMY_WORK *pEm)
{
    SVECTOR Rot;
    MATRIX M;
    LONG Unused[8];
    VECTOR V;
    LONG Unused2[4];

    Rot.vx = 0;
    Rot.vy = -pA->Dir;
    Rot.vz = 0;
    RotMatrix_gte(&Rot, &M);
    V.vx = pEm->Pos_x - pA->pPos->vx;
    V.vy = 0;
    V.vz = pEm->Pos_z - pA->pPos->vz;
    ApplyMatrixLV(&M, &V, &V);
    V.vx -= pA->pBox[0];
    V.vz -= pA->pBox[1];
    if ((ULONG)(pA->pBox[2] - V.vx) > (ULONG)(pA->pBox[2] * 2)) {
        return 0;
    }
    if ((ULONG)(pA->pBox[3] - V.vz) > (ULONG)(pA->pBox[3] * 2)) {
        return 0;
    }
    pEm->pUnk16C = pPos0;
    pEm->pUnk170 = pPos1;
    pEm->Be_flg |= 0x80000000;
    pEm->UnkC7 = pA->Id;
    return 1;
}

/* pV inside the wedge given by the packed shorts A, B, widened by W (y/z) */
LONG Wedge_ck_w(ULONG A, ULONG B, VECTOR *pV, LONG W)
{
    SHORT ay, ax, by, bx;
    LONG t;

    ay = A >> 16;
    ax = A;
    by = B >> 16;
    bx = B;
    if (pV->vy < ax) {
        return 0;
    }
    t = -by * pV->vy / bx;
    if (pV->vz < (SHORT)(t - ay - W)) {
        return 0;
    }
    t = (SHORT)t;
    return (SHORT)(ay - t + W) >= pV->vz;
}

/* pV inside the wedge given by the packed shorts A, B (y/x) */
LONG Wedge_ck(ULONG A, ULONG B, VECTOR *pV)
{
    SHORT ay, by, bx;
    LONG t;

    ay = A >> 16;
    by = B >> 16;
    bx = B;
    t = -by * pV->vy / bx;
    if (pV->vx < (SHORT)(t - ay)) {
        return 0;
    }
    t = (SHORT)t;
    return (SHORT)(ay - t) >= pV->vx;
}


/* weapon hit check: the nearest enemy inside the weapon wedge (pW = Pl_wep_data entry, Type = weapon)
 * takes the damage of that weapon; returns it, or NULL */
ENEMY_WORK *Weapon_at_ck(MATRIX *pM, PL_WEP_DATA *pW, ULONG Type)
{
    ENEMY_WORK **ppEnd;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;
    ENEMY_WORK *pBest;
    EM_HIT_AREA *pH;
    MATRIX *pTm;
    VECTOR *pPos;
    PL_WEP_DMG *pD;
    LONG Dist;
    LONG dz;
    LONG Min;
    ULONG Side;
    EM_HIT_AREA *pBest_h;
    ULONG Dmg;
    ULONG Cnt;
    ULONG Kind;
    ULONG Dir;
    ULONG Flg;
    LONG X0;
    LONG X1;
    VECTOR V;
    VECTOR V2;
    MATRIX Unused; /* never used; keeps the frame layout */
    MATRIX Mt;
    VECTOR Best_v;
    VECTOR Best_v2;

    Min = 0x7FFFFFFF;
    pW += Type - 1;
    pPos = (VECTOR *)pM->t;
    Side = 0;
    TransposeMatrix(pM, &Mt);
    SetRotMatrix(&Mt);
    ppEnd = G.ppEnemy_end;
    for (ppEm = &G.pEnemy[2]; ppEm != ppEnd; ppEm++) {
        pEm = *ppEm;
        pH = &pEm->Hit_area;
        if (!(pEm->Be_flg & 1)) {
            continue;
        }
        if (pEm->Damage_cnt != 0) {
            continue;
        }
        if (pEm->Type & 0xC000) {
            continue;
        }
        pTm = pH->pM;
        if (Sca_ck_line(pPos, (VECTOR *)pTm->t, 0x40, 1)) {
            continue;
        }
        Dist = (pPos->vx - pTm->t[0]) >> 1;
        dz = (pPos->vz - pTm->t[2]) >> 1;
        Dist = SquareRoot0(Dist * Dist + dz * dz) * 2;
        if (Dist > Min) {
            continue;
        }
        if (pW->Range + pH->R < Dist) {
            continue;
        }
        V.vx = pTm->t[0] - pM->t[0];
        V.vy = 0;
        V.vz = pTm->t[2] - pM->t[2];
        ApplyRotMatrixLV(&V, &V);
        if (!Wedge_ck_w(pW->UnkE << 16 | pW->UnkC, pW->Unk4 << 16 | pW->Unk2, &V, pH->R)) {
            continue;
        }
        V.vx = pTm->t[0] - pM->t[0];
        V.vy = pTm->t[1] + pEm->Hit_area.Top - pM->t[1] + (SHORT)(pH->R >> 1);
        V.vz = pTm->t[2] - pM->t[2];
        ApplyRotMatrixLV(&V, &V);
        V2.vx = pTm->t[0] - pM->t[0];
        V2.vy = pTm->t[1] + pH->Bottom - pM->t[1] - (SHORT)(pH->R >> 1);
        V2.vz = pTm->t[2] - pM->t[2];
        ApplyRotMatrixLV(&V2, &V2);
        if (V2.vx < 0 || V.vx > 0) {
            if (!Wedge_ck(pW->Unk10 << 16 | pW->UnkC, pW->Unk0 << 16 | pW->Unk2, &V)
                && !Wedge_ck(pW->Unk10 << 16 | pW->UnkC, pW->Unk0 << 16 | pW->Unk2, &V2)) {
                continue;
            }
            if (V2.vx < 0 && (pEm->Damage_flg & 0x8000)) {
                continue;
            }
            if (pEm->Damage_flg & 0x4000) {
                continue;
            }
            Min = Dist;
            Side = 0;
            pBest_h = pH;
            Best_v = V;
            Best_v2 = V2;
            pBest = pEm;
        } else {
            Min = Dist;
            pBest = pEm;
            Side = 1;
            pBest_h = pH;
            Best_v = V;
            Best_v2 = V2;
        }
    }
    if (Min == 0x7FFFFFFF) {
        return 0;
    }
    pBest->Damage_no = Type;
    if (Type == 0xD && (Rnd() & 0xF) == 2) {
        pBest->Damage_no = 5;
        Type = 5;
        pW++;
    }
    pD = pW->pDmg[pBest->Id - 0x10];
    pBest->Old_routine = EM_RTN(pBest);
    pBest->Damage_flg &= 0xC000;
    pD += Type - 1;
    Dmg = pD->Damage & 0x3FF;
    Cnt = pD->Cnt & 0x1F;
    Kind = pD->Kind & 0x1F;
    if (pW->Dist1 < Min) {
        pBest->Damage_flg |= 4;
        Dmg = (pD->Damage >> 10) & 0x3FF;
        Cnt = (pD->Cnt >> 5) & 0x1F;
        Kind = (pD->Kind >> 5) & 0x1F;
    }
    if (pW->Dist2 < Min) {
        pBest->Damage_flg = (pBest->Damage_flg & 0xC000) | 8;
        Dmg = (pD->Damage >> 20) & 0x3FF;
        Cnt = (pD->Cnt >> 10) & 0x1F;
        Kind = (pD->Kind >> 10) & 0x1F;
    }
    Flg = pBest->Damage_flg | Kind << 8;
    pBest->Damage_flg = Flg;
    Dir = 0;
    if (Side == 0) {
        pBest->Damage_flg |= 1;
        if (Best_v2.vx < 0) {
            pBest->Damage_flg = (pBest->Damage_flg & 0xFFFC) | 3;
            Dir = 2;
        }
        pBest->Life -= Dmg;
        pBest->Damage_cnt = Cnt;
        EM_SET_RTN(pBest, 2, Dir, 0, 0);
        if (pBest->Life < 0) {
            EM_SET_RTN(pBest, 3, Dir, 0, 0);
            pBest->Damage_cnt |= 0x80;
        }
        pBest->Damage_flg |= Cdir_ck_pos(pPos->vx, pPos->vz, pBest) << 4;
        return pBest;
    } else {
        X1 = Best_v.vx;
        X0 = X1 + ((Best_v2.vx - X1) * pBest_h->Rate0 >> 8);
        X1 += (Best_v2.vx - X1) * pBest_h->Rate1 >> 8;
        Flg |= 1;
        pBest->Damage_flg = Flg;
        if (X0 < 0) {
            pBest->Damage_flg = (pBest->Damage_flg & 0xFFFC) | 2;
            Dir = 1;
        }
        if (X1 < 0) {
            pBest->Damage_flg |= 3;
            Dir = 2;
        }
        pBest->Life -= Dmg;
        pBest->Damage_cnt = Cnt;
        EM_SET_RTN(pBest, 2, Dir, 0, 0);
        if (pBest->Life < 0) {
            EM_SET_RTN(pBest, 3, Dir, 0, 0);
            pBest->Damage_cnt |= 0x80;
        }
        pBest->Damage_flg |= Cdir_ck_pos(pPos->vx, pPos->vz, pBest) << 4;
        return pBest;
    }
}

/* position of pM1's origin in pM0's space */
void Matrix_rel_pos(MATRIX *pM0, MATRIX *pM1, SVECTOR *pOut)
{
    MATRIX M;
    VECTOR V;

    TransposeMatrix(pM0, &M);
    V.vx = pM1->t[0] - pM0->t[0];
    V.vy = pM1->t[1] - pM0->t[1];
    V.vz = pM1->t[2] - pM0->t[2];
    ApplyMatrixLV(&M, &V, &V);
    pOut->vx = V.vx;
    pOut->vy = V.vy;
    pOut->vz = V.vz;
}

void Mag_set(ENEMY_WORK *pEm)
{
    LONG Type;
    PARTS_W *pP;
    PARTS_W *pP_ptr;
    signed char Spd_y;
    ULONG Flg;
    MATRIX Tm;
    SVECTOR Ts;
    SVECTOR Ts0;

    Type = (UCHAR)pEm->Type;
    if (Type != 0) {
        Type = Pl_wep_mag_type[Type - 1];
        if (Type != 0) {
            Type--;
            pP_ptr = pEm->pSin_parts_ptr;
            pP = &pP_ptr[Pl_mag[Type].Parts_no];
            pP->Be_flg |= 3;
            Spd_y = Pl_mag[Type].V_y;
            pP->Timer1 = 1;
            pP->Timer2 = 0;
            pP->Timer0 = Spd_y;
            Flg = Pl_mag[Type].Flg;
            pP->Cdir_x = 0;
            pP->Cdir_y = 0;
            pP->Cdir_z = 0;
            pP->Timer3 = Flg;
            RotMatrix(CDIR_SV(pP), PW_M(pP));
            CompM(&pP_ptr[4].Workm, PW_M(pP), &pP->Workm);
            pP->Old_x = pEm->Ground;
            Ts.vx = 0;
            Ts.vy = Pl_mag[Type].Dir + pEm->Cdir_y;
            Ts.vz = 0;
            RotMatrix(&Ts, &Tm);
            Ts.vx = Pl_mag[Type].V_xz;
            Ts.vy = 0;
            ApplyMatrixSV(&Tm, &Ts, &Ts0);
            pP->Old_y = Ts0.vx;
            pP->Old_z = Ts0.vz;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", Mag_down);

void Pl_wep_eff_init(ENEMY_WORK *pEm)
{
    Pl_wep_eff_parts_init(pEm);
    Pl_wep_poly_init();
}

/* empty, unreferenced */
void Pl_wep_dummy(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", Pl_wep_parts_drop);

/* empty, unreferenced */
void Pl_wep_dummy2(void) {
}

LONG Pl_wep_eff_parts_init(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pP = &pEm->pSin_parts_ptr[15];
    i = 6;
    do {
        pP->Unk74 = 0xFF;
        pP->Be_flg = 0;
        pP->Rot_spd_x = 0;
        Esp_kill(0x103, &pP->Workm);
        Esp_kill(0xC03, &pP->Workm);
        Esp_kill(0x105, &pP->Workm);
        Esp_kill(0x805, &pP->Workm);
        pP++;
    } while (--i != 0);
    return 0;
}

/* start the weapon effect parts (parts 15..20) of the player */
LONG Pl_wep_eff_parts_set(ENEMY_WORK *pEm)
{
    PARTS_W *pW;
    PARTS_W *pP;
    MATRIX *pM;
    LONG i;
    SVECTOR Dsv;
    SVECTOR V;

    pW = &pEm->pSin_parts_ptr[15];
    pW->Rot_spd_x = 12;
    if ((UCHAR)pEm->Type == 0x14) {
        pW->Rot_spd_x = 0x14;
    }
    i = 6;
    do {
        i--;
        if (pW->Unk74 == 0xFF) {
            pW->Unk74 = 0;
            pW->Be_flg = 0x23;
            pW->Pos_x = 0xAA;
            pW->Pos_y = 0x190;
            pW->Pos_z = 0x76;
            *(ULONG *)&pW->M[0][0] = 0x1000;
            *(ULONG *)&pW->M[0][2] = 0;
            *(ULONG *)&pW->M[1][1] = 0x1000;
            *(ULONG *)&pW->M[2][0] = 0;
            *(ULONG *)&pW->M[2][2] = 0x1000;
            pW->UnkA2 |= 0x200B;
            pW->Rot_y = pEm->Cdir_y;
            if ((UCHAR)pEm->Type == 0x14) {
                Dsv.vx = 0;
                Dsv.vy = 0;
                Dsv.vz = 0;
                V.vx = 0;
                V.vy = -150;
                V.vz = 0;
                Esp_call(0x805, 0x4000000, &pW->Workm, &V, &Dsv);
                Dsv.vx = 0;
                Dsv.vy = 0x400;
                Dsv.vz = 0;
                Esp_call(0x805, 0x4000000, &pW->Workm, &V, &Dsv);
            } else {
                Esp_call(0x103, 0x10000000, &pW->Workm, 0, 0);
            }
            pP = pEm->pSin_parts_ptr;
            pM = &pP[4].Workm;
            pW->pSuper = pM;
            pW->pOya_parts = &pP[4];
            CompM(pW->pSuper, PW_M(pW), &pW->Workm);
            pW->Unk7C = pEm->Ground;
            return 1;
        }
        pW++;
    } while (i != 0);
}

UCHAR Pl_wep_eff_parts_cnt(void)
{
    LONG i;
    UCHAR Cnt;
    PARTS_W *pP;
    UCHAR Unused[16];

    i = 6;
    Cnt = 0;
    pP = &G.Player.pSin_parts_ptr[15];
    do {
        if (pP->Unk74 != 0xFF) {
            Cnt++;
        }
        pP++;
    } while (--i != 0);
    return Cnt;
}

/* nearest live enemy within LL of pPos and Lim of the direction Dir */
ENEMY_WORK *Em_near_dir_ck(VECTOR *pPos, SHORT Dir, ULONG LL, SHORT Lim)
{
    tagGLOBAL *pG;
    ENEMY_WORK **ppT;
    ENEMY_WORK *pT;
    ENEMY_WORK *pNear;
    ULONG Dist;
    ULONG Min;
    LONG Wk;
    LONG dz;
    LONG d;

    pNear = 0;
    pG = &G;
    if (pG->Em_num == 0) {
        return pG->pEm;
    }
    Min = LL;
    ppT = &pG->pEnemy[2];
    do {
        pT = *ppT;
        if ((pT->Be_flg & 1) && pT->Damage_cnt == 0 && !(pT->Type & 0xC000)) {
            Wk = (pPos->vx - pT->Pos_x) >> 1;
            dz = (pPos->vz - pT->Pos_z) >> 1;
            Dist = SquareRoot0(Wk * Wk + dz * dz) * 2;
            d = Direction_ck(pPos->vx, pPos->vz, pT->Pos_x, pT->Pos_z);
            if (pT->Life >= 0 && Dir_limit((SHORT)d, Dir, Lim, (SHORT *)&Wk) == 0
                && Sca_ck_line(POS_V(pT), pPos, 0x40, 1) == 0 && Dist < Min) {
                Min = Dist;
                pNear = pT;
                ppT++;
                continue;
            }
        }
        ppT++;
    } while (ppT != pG->ppEnemy_end);
    if (pNear != 0) {
        return pNear;
    }
    return G.pEm;
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", Pl_wep_eff_move);

/* part of pEm (from the list pTbl) nearest to pM's origin */
ULONG Parts_near_get(ENEMY_WORK *pEm, MATRIX *pM, UCHAR *pTbl)
{
    ULONG Min;
    ULONG Ret;
    ULONG d;
    LONG dy;
    LONG i;
    ULONG j;
    UCHAR *p;
    PARTS_W *pP;
    LONG sq;

    Min = 0x7FFFFFFF;
    if (pEm->Id < 0x10) {
        return 0;
    }
    i = 4;
    j = 0;
    p = &pTbl[pEm->Id * 4 - 0x40];
    do {
        pP = &pEm->pSin_parts_ptr[*p];
        sq = (pP->Workm.t[0] - pM->t[0]) * (pP->Workm.t[0] - pM->t[0]) + (pP->Workm.t[2] - pM->t[2]) * (pP->Workm.t[2] - pM->t[2]);
        dy = pP->Workm.t[1] - pM->t[1];
        d = dy < 0 ? -dy : dy;
        d += SquareRoot0(sq);
        if (d < Min) {
            Min = d;
            Ret = *p | (j << 16);
        }
        p++;
        j++;
    } while (--i != 0);
    return Ret;
}

LONG Dir_limit(LONG Dir, LONG Base, LONG Lim, SHORT *pDir)
{
    LONG d;
    SHORT Out;

    d = (Lim + (Dir - Base)) & 0xFFF;
    Base -= Lim;
    if (d < (SHORT)Lim * 2) {
        *pDir = Dir;
        return 0;
    }
    Out = Base;
    if ((ULONG)d <= 0x800) {
        Out = Base + Lim * 2;
    }
    *pDir = Out;
    return 1;
}

/* y and z angles of the vector pV */
void Vec_dir_get(SVECTOR *pV, SVECTOR *pOut)
{
    SHORT Dir_y;
    SHORT Dir_z;

    Dir_y = Get_dir_xz(pV->vx, pV->vz);
    Dir_z = (Get_dir_xz(SquareRoot0(pV->vx * pV->vx + pV->vz * pV->vz), -pV->vy) - 0x400) & 0xFFF;
    pOut->vx = 0;
    pOut->vy = Dir_y;
    pOut->vz = Dir_z;
}

SHORT Get_dir_xz(LONG X, LONG Z)
{
    LONG Dir;
    LONG Base;

    Dir = 0x400;
    if (Z > 0) {
        Dir = 0xC00;
    }
    if (X != 0) {
        Dir = catan((Z << 12) / X);
        if (X < 0) {
            return (0x800 - Dir) & 0xFFF;
        }
        return (0x1000 - Dir) & 0xFFF;
    }
    return Dir;
}

/* set up the six weapon light polys */
void Pl_wep_poly_init(void)
{
    PL_WEP_POLY *pWp;
    LONG i;
    ULONG Tp;
    ULONG Cl;

    pWp = Pl_wep_poly;
    i = 6;
    Tp = G.Player.Tpage;
    Cl = G.Player.Clut;
    do {
        pWp->Be_flg = 0;
        pWp->Unk2 = 10;
        *(ULONG *)&pWp->Poly[0].r0 = 0x808080;
        SetPolyFT4(&pWp->Poly[0]);
        SetSemiTrans(&pWp->Poly[0], 1);
        pWp->Poly[0].tpage = GetTPage(1, 1, (Tp - 14) << 6, 0x100);
        pWp->Poly[0].clut = GetClut(0, Cl + 0x1E2);
        pWp->Poly[0].u0 = 0;
        pWp->Poly[0].v0 = 0xEF;
        pWp->Poly[0].u1 = 0x10;
        pWp->Poly[0].v1 = 0xEF;
        pWp->Poly[0].u2 = 0;
        pWp->Poly[0].v2 = 0xFE;
        pWp->Poly[0].u3 = 0x10;
        pWp->Poly[0].v3 = 0xFE;
        Memcpy(&pWp->Poly[1], &pWp->Poly[0], 0x28);
        pWp++;
    } while (--i != 0);
}

void Pl_wep_poly_set(PL_WEP_POLY *pWp, SHORT Unk2, LONG *pPos)
{
    pWp->Be_flg = 1;
    pWp->pPos = pPos;
    pWp->Unk2 = Unk2;
}

void Pl_wep_poly_rgb(PL_WEP_POLY *pWp, ULONG Rgb)
{
    *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[0].r0 |= Rgb;
    *(ULONG *)&pWp->Poly[1].r0 |= Rgb;
}

/* hide a weapon light poly */
void Pl_wep_poly_off(PL_WEP_POLY *pWp)
{
    pWp->Be_flg = 0;
}

/* show a weapon light poly at half brightness when its depth is in range */
void Pl_wep_poly_ck(PL_WEP_POLY *pWp, ENEMY_WORK *pEm)
{
    LONG d;
    ULONG Rgb;

    pWp->Be_flg = 0;
    d = *(USHORT *)&pWp->pPos[1] - (USHORT)pEm->Pos_y;
    if ((USHORT)(d + 0xC7F) < 0x125B) {
        pWp->Be_flg = 1;
        Rgb = *(ULONG *)&pWp->Poly[0].r0 & 0xFFFFFF;
        *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[0].r0 |= Rgb >> 1;
        *(ULONG *)&pWp->Poly[1].r0 |= Rgb >> 1;
    }
    if ((USHORT)(d + 0xA8B) < 0xE73) {
        pWp->Be_flg = 1;
        *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[0].r0 |= Rgb;
        *(ULONG *)&pWp->Poly[1].r0 |= Rgb;
    }
}

/* draw the visible weapon light polys */
void Pl_wep_poly_trans(void)
{
    tagGLOBAL *pG = &G;
    PL_WEP_POLY *pWp;
    POLY_FT4 *pPoly;
    ULONG *pOt;
    LONG Scale;
    LONG i;
    SHORT Z;
    SHORT R;
    SHORT X;
    SHORT Y;
    SVECTOR Sv;
    DVECTOR Sxy[2];

    if (pG->Stop_flg & 0x1000000) {
        return;
    }
    pWp = Pl_wep_poly;
    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    i = 6;
    Scale = pG->pRoom->Cut[pG->Cut_no].ViewR >> 7;
    do {
        if (!(pWp->Be_flg & 1)) {
            pWp++;
            continue;
        }
        if (!Hit_ck_point4(pWp->pPos, G.Cccut)) {
            pWp++;
            continue;
        }
        pOt = Ot_get_pos(pWp->pPos);
        Sv.vx = pWp->pPos[0];
        Sv.vy = pWp->pPos[1];
        Sv.vz = pWp->pPos[2];
        Z = RotTransPers(&Sv, (LONG *)&Sxy[0], (LONG *)&Sxy[1], (LONG *)&Sxy[1]);
        if (Z < 8) {
            pWp++;
            continue;
        }
        R = Scale * pWp->Unk2 / (Z * 4);
        pPoly = &pWp->Poly[pG->Cbuf_id];
        X = Sxy[0].vx;
        Y = Sxy[0].vy;
        setXY4(pPoly, X - R, Y - R, X + R, Y - R, X - R, R + Y, X + R, R + Y);
        if (Z >= 0x2000) {
            Z = 0x1FFF;
        }
        pWp++;
        addPrim(&pOt[Z >> 3], pPoly);
    } while (--i != 0);
}

/* flags of the enemies whose parts are within Ll0/Ll1 of pPos */
ULONG Em_near_pos_ck(VECTOR *pPos, LONG Ll0, LONG Ll1)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK **ppT;
    ENEMY_WORK **ppEnd;
    ENEMY_WORK *pT;
    PARTS_W *pP;
    ULONG Ret;
    ULONG i;
    LONG dx;
    LONG dy;
    LONG dz;
    LONG d;

    Ret = 0;
    ppT = &pG->pEnemy[0];
    i = 0;
    ppEnd = pG->ppEnemy_end;
    do {
        pT = *ppT;
        if ((pT->Be_flg & 1) && pT->Life >= 0 && pT->Damage_cnt == 0 && !(pT->Type & 0xC000)) {
            pP = &pT->pSin_parts_ptr[pT->Neck_no];
            dx = (pPos->vx - pP->Workm.t[0]) >> 1;
            dy = (pPos->vy - pP->Workm.t[1]) >> 1;
            dz = (pPos->vz - pP->Workm.t[2]) >> 1;
            d = SquareRoot0(dx * dx + dy * dy + dz * dz) * 2;
            if (d < Ll0) {
                Ret |= 1;
            }
            if (d < Ll1 && i != 0) {
                Ret |= 2;
            }
            dx = (pPos->vx - pT->Pos_x) >> 1;
            dy = (pPos->vy - pT->Pos_y) >> 1;
            dz = (pPos->vz - pT->Pos_z) >> 1;
            d = SquareRoot0(dx * dx + dy * dy + dz * dz) * 2;
            if (d < Ll0) {
                Ret |= 1;
            }
            if (d < Ll1 && i != 0) {
                Ret |= 2;
            }
        }
        ppT++;
        i++;
    } while (ppT != ppEnd);
    return Ret;
}

/* damage request against the enemies (Unk0 0x80: also the player and sub player, 0x40: up to
 * ppEnemy_end); the inner box (W2/D2) gives Damage2; returns the hit flags */
LONG Pos_em_at_ck(HIT_REQ *pReq, HIT_ATTR *pAttr)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK **ppEnd;
    ENEMY_WORK *pEm;
    EM_HIT_AREA *pH;
    MATRIX *pTm;
    VECTOR *pPos;
    LONG i;
    LONG Ret;
    LONG Hit;
    LONG Dmg;
    ULONG Cnt;
    ULONG Dmg_flg;
    LONG Dir;
    LONG Rt;
    LONG w;
    LONG d;
    LONG X0;
    LONG X1;
    SHORT Top;
    SHORT Bottom;
    UCHAR Attr;
    UCHAR Mode;
    SVECTOR Sv;
    MATRIX M;
    MATRIX Mt;
    VECTOR V;

    Sv.vx = 0;
    Sv.vy = pReq->R;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    Sv.vx = pReq->W;
    Sv.vy = 0;
    Sv.vz = pReq->D;
    ApplyMatrixSV(&M, &Sv, &Sv);
    M.t[0] = pReq->pPos->vx + Sv.vx;
    M.t[1] = pReq->pPos->vy;
    M.t[2] = pReq->pPos->vz + Sv.vz;
    i = 0;
    Bottom = pAttr->Bottom;
    Top = pAttr->Top;
    TransposeMatrix(&M, &Mt);
    SetRotMatrix(&Mt);
    Attr = pAttr->Unk0;
    pPos = (VECTOR *)M.t;
    if (!(Attr & 0x80)) {
        ppEm = &G.pEnemy[2];
        i = 2;
    } else {
        ppEm = &G.pEnemy[0];
    }
    if (Attr & 0x40) {
        ppEnd = G.ppEnemy_end;
    } else {
        ppEnd = &G.pEnemy[2];
    }
    if (ppEm == ppEnd) {
        return 0;
    }
    Ret = 0;
    do {
        pEm = *ppEm;
        i++;
        if (!(pEm->Be_flg & 1)) {
            goto next;
        }
        if (pEm->Damage_cnt != 0) {
            goto next;
        }
        if (pEm->Type & 0xC000) {
            goto next;
        }
        pH = &pEm->Hit_area;
        pTm = pH->pM;
        if (Sca_ck_line(pPos, (VECTOR *)pTm->t, 0x40, 1)) {
            goto next;
        }
        V.vx = pTm->t[0] - pPos->vx;
        V.vy = pTm->t[1] - pPos->vy;
        V.vz = pTm->t[2] - pPos->vz;
        ApplyRotMatrixLV(&V, &V);
        if (Bottom > pEm->Hit_area.Top + V.vy) {
            goto next;
        }
        if (Top < pH->Bottom + V.vy) {
            goto next;
        }
        w = pAttr->W + pEm->Hit_area.R;
        d = pAttr->D + pEm->Hit_area.R;
        Hit = 0;
        if ((ULONG)(V.vx + w) < (ULONG)(w * 2)) {
            Hit = (ULONG)(d * 2) > (ULONG)(V.vz + d);
        }
        w = pAttr->W2 + pEm->Hit_area.R;
        d = pAttr->D2 + pEm->Hit_area.R;
        if ((ULONG)(V.vx + w) < (ULONG)(w * 2) && (ULONG)(V.vz + d) < (ULONG)(d * 2)) {
            Hit = 2;
        }
        if (Hit == 0) {
            goto next;
        }
        pEm->Damage_flg &= 0xC000;
        pEm->Old_routine = EM_RTN(pEm);
        if (Hit == 2) {
            Dmg = pAttr->Damage2;
            Cnt = pAttr->Damage_cnt2;
        } else {
            pEm->Damage_flg |= 4;
            Dmg = pAttr->Damage;
            Cnt = pAttr->Damage_cnt;
        }
        Dmg_flg = pEm->Damage_flg | 1;
        pEm->Damage_flg = Dmg_flg;
        Dir = 0;
        X0 = pH->Top + V.vy + ((pH->Bottom - pH->Top) * pEm->Hit_area.Rate0 >> 8);
        X1 = pH->Top + V.vy + ((pH->Bottom - pH->Top) * pEm->Hit_area.Rate1 >> 8);
        if (X0 > 0) {
            pEm->Damage_flg = (Dmg_flg & 0xFFFC) | 2;
            Dir = 1;
        }
        if (X1 > 0) {
            pEm->Damage_flg |= 3;
            Dir = 2;
        }
        if (pH->Top + V.vy <= 0) {
            Dir = 0;
            pEm->Damage_flg = (pEm->Damage_flg & 0xFFFC) | 1;
        }
        if (pH->Bottom + V.vy >= 0) {
            Dir = 2;
            pEm->Damage_flg |= 3;
        }
        pEm->Damage_no = pAttr->Unk1;
        pEm->Damage_cnt = Cnt;
        Hit = Cdir_ck_pos(pPos->vx, pPos->vz, pEm);
        if (i < 3) {
            Rt = Hit + 8;
            EM_SET_RTN(pEm, 2, (pEm->Damage_no - 0x18) * 2 + Rt, 0, 0);
            Dmg >>= 1;
            if (i == 1) {
                Ret |= 0x80000001;
                Mode = Attr & 0x20;
                if (Pl_life_down(Dmg, Mode != 0) == 2) {
                    EM_SET_RTN(pEm, 3, 0, 0, 0);
                    pEm->Damage_cnt |= 0x80;
                }
            } else {
                Ret |= 0x40000001;
                Mode = Attr & 0x20;
                if (Spl_life_sub(Dmg, Mode != 0) == 2) {
                    EM_SET_RTN(pEm, 3, 0, 0, 0);
                    pEm->Damage_cnt |= 0x80;
                }
            }
        } else {
            EM_SET_RTN(pEm, 2, Dir, 0, 0);
            if (pEm->Status_flg & 0x100) {
                Dmg >>= 1;
            }
            Ret |= 0x20000001;
            if ((SHORT)(pEm->Life -= Dmg) < 0) {
                EM_SET_RTN(pEm, 3, Dir, 0, 0);
                pEm->Damage_cnt |= 0x80;
                if (G.System_flg & 0x80) {
                    Em_kill_score(pEm->Id, 1);
                }
            }
        }
        pEm->Damage_flg |= Hit << 4;
        pEm->Damage_flg |= (Attr & 0x1F) << 8;
next:
        ppEm++;
    } while (ppEm != ppEnd);
    return Ret;
}

/* weapon hit check against the object model targets */
LONG Weapon_om_at_ck(MATRIX *pM, PL_WEP_DATA *pW, ULONG Type)
{
    tagGLOBAL *pG = &G;
    OM_TARGET **ppT;
    OM_TARGET *pT;
    EM_HIT_AREA *pH;
    MATRIX *pTm;
    VECTOR *pPos;
    LONG i;
    LONG Dist;
    ULONG Min;
    LONG dz;
    VECTOR V;
    VECTOR V2;
    MATRIX Unused;
    MATRIX Mt;

    Min = 0x7FFFFFFF;
    if (pG->Om_target_num == 0) {
        return 0;
    }
    pW += Type - 1;
    pPos = (VECTOR *)pM->t;
    TransposeMatrix(pM, &Mt);
    SetRotMatrix(&Mt);
    i = pG->Om_target_num;
    ppT = pG->pOm_target;
    do {
        pT = *ppT;
        if (pT->Flg == 0xFF) {
            goto next;
        }
        if (Type == 1 && !(pT->pOm->Type & 0x80)) {
            goto next;
        }
        pH = &pT->Area;
        pTm = pH->pM;
        if (pT->pOm->Type == 0) {
            pTm->t[1] -= 0x708;
        }
        if (Sca_ck_line(pPos, (VECTOR *)pTm->t, 0x40, 1)) {
            do {
                do {
                    ppT++;
                } while (0);
            } while (0);
            if (pT->pOm->Type == 0) {
                pTm->t[1] += 0x708;
            }
            goto end;
        }
        if (pT->pOm->Type == 0) {
            pTm->t[1] += 0x708;
        }
        Dist = (pPos->vx - pTm->t[0]) >> 1;
        dz = (pPos->vz - pTm->t[2]) >> 1;
        Dist = SquareRoot0(Dist * Dist + dz * dz) * 2;
        if (pW->Range < Dist) {
            goto next;
        }
        V.vx = pTm->t[0] - pM->t[0];
        V.vy = 0;
        V.vz = pTm->t[2] - pM->t[2];
        ApplyRotMatrixLV(&V, &V);
        if (!Wedge_ck_w(0x1900000 | pW->UnkC, 0x1000, &V, pH->R)) {
            goto next;
        }
        V.vx = pTm->t[0] - pM->t[0];
        V.vy = pTm->t[1] + pT->Area.Top - pM->t[1];
        V.vz = pTm->t[2] - pM->t[2];
        ApplyRotMatrixLV(&V, &V);
        V2.vx = pTm->t[0] - pM->t[0];
        V2.vy = pTm->t[1] + pH->Bottom - pM->t[1];
        V2.vz = pTm->t[2] - pM->t[2];
        ApplyRotMatrixLV(&V2, &V2);
        if (V2.vx < 0 || V.vx > 0) {
            if (!Wedge_ck(0x3E80000 | pW->UnkC, 0x1001000, &V) && !Wedge_ck(0x3E80000 | pW->UnkC, 0x1001000, &V2)) {
                goto next;
            }
        }
        goto hit;
next:
        ppT++;
        goto end;
hit:
        do {
            do {
                ppT++;
            } while (0);
        } while (0);
        pT->Flg = 0xFF;
        Min = Dist;
        pT->Unk2 = Type;
end:
        ;
    } while (--i != 0);
    return Min != 0x7FFFFFFF;
}

/* damage request against the object model targets */
LONG Pos_om_at_ck(HIT_REQ *pReq, HIT_ATTR *pAttr)
{
    tagGLOBAL *pG = &G;
    OM_TARGET **ppT;
    OM_TARGET *pT;
    EM_HIT_AREA *pH;
    VECTOR *pPos;
    LONG i;
    LONG Ret;
    SHORT Top;
    SHORT Bottom;
    LONG w;
    LONG d;
    MATRIX *pM;
    SVECTOR Rot;
    MATRIX M;
    MATRIX Mt;
    VECTOR V;

    if (pG->Om_target_num == 0) {
        return 0;
    }
    Rot.vx = 0;
    Rot.vy = pReq->R;
    Rot.vz = 0;
    RotMatrix_gte(&Rot, &M);
    ppT = pG->pOm_target;
    *(VECTOR *)M.t = *pReq->pPos;
    pPos = pReq->pPos;
    Bottom = pAttr->Bottom;
    Top = pAttr->Top;
    TransposeMatrix(&M, &Mt);
    SetRotMatrix(&Mt);
    Ret = 0;
    i = pG->Om_target_num;
    do {
        pT = *ppT;
        pH = &pT->Area;
        if (pT->Flg != 0xFF) {
            pM = pH->pM;
            V.vx = pM->t[0] - pPos->vx;
            V.vy = pM->t[1] - pPos->vy;
            V.vz = pM->t[2] - pPos->vz;
            ApplyRotMatrixLV(&V, &V);
            if (pH->Top + V.vy >= Bottom && Top >= pH->Bottom + V.vy) {
                w = pAttr->W + pT->Area.R;
                d = pAttr->D + pT->Area.R;
                if ((ULONG)(V.vx + w) < (ULONG)(w * 2) && (ULONG)(V.vz + d) < (ULONG)(d * 2)) {
                    pT->Flg = 0xFF;
                    pT->Unk2 = pAttr->Unk1;
                    Ret = 1;
                }
            }
        }
        i--;
        do {
            ppT++;
        } while (0);
    } while (i != 0);
    return Ret;
}

/* position of the nearest object model target within LL */
VECTOR *Om_target_near(ENEMY_WORK *pEm, ULONG LL)
{
    tagGLOBAL *pG = &G;
    OM_TARGET **ppT;
    OM_TARGET *pT;
    OM_TARGET *pNear;
    MATRIX *pM;
    ULONG Min;
    ULONG Dist;
    LONG i;
    LONG dx;
    LONG dz;
    VECTOR Unused; /* never referenced; keeps the 0x48 frame */

    Min = 0x7FFFFFFF;
    i = pG->Om_target_num;
    if (i == 0) {
        return 0;
    }
    ppT = pG->pOm_target;
    if (LL != 0) {
        Min = LL;
    }
    pNear = 0;
    do {
        do {
            pT = *ppT;
        } while (0);
        if (pT->Flg != 0xFF) {
            pM = pT->Area.pM;
            dx = (pEm->Pos_x - pM->t[0]) >> 1;
            dz = (pEm->Pos_z - pM->t[2]) >> 1;
            Dist = SquareRoot0(dx * dx + dz * dz);
            if (pT->pOm->Type == 0) {
                pM->t[1] -= 0x708;
            }
            if (Sca_ck_line(WORKM_POS_V(&pEm->pSin_parts_ptr[pEm->Neck_no]), (VECTOR *)pM->t, 0x40, 1) == 0 && Dist < Min) {
                Min = Dist;
                pNear = pT;
            }
            if (pT->pOm->Type == 0) {
                pM->t[1] += 0x708;
            }
        }
        i--;
        do {
            ppT++;
        } while (0);
    } while (i != 0);
    if (pNear != 0) {
        return (VECTOR *)pNear->Area.pM->t;
    }
    return 0;
}

/* damage request against one enemy pEm */
ULONG Pos_em1_at_ck(ENEMY_WORK *pEm, HIT_REQ *pReq, HIT_ATTR *pAttr)
{
    ENEMY_WORK **ppT;
    ENEMY_WORK **ppEnd;
    ENEMY_WORK *pT;
    EM_HIT_AREA *pH;
    MATRIX *pM;
    VECTOR *pPos;
    ULONG Ret;
    LONG Part;
    LONG Hit;
    LONG Lo;
    LONG Hi;
    LONG Y;
    LONG H;
    LONG Dmg;
    SHORT Top;
    SHORT Bottom;
    UCHAR Kind;
    UCHAR Cnt;
    LONG w;
    LONG d;
    SVECTOR Rot;
    MATRIX M;
    MATRIX Mt;
    VECTOR V;

    Rot.vx = 0;
    Rot.vy = pReq->R;
    Rot.vz = 0;
    RotMatrix_gte(&Rot, &M);
    *(VECTOR *)M.t = *pReq->pPos;
    pPos = pReq->pPos;
    Bottom = pAttr->Bottom;
    Top = pAttr->Top;
    TransposeMatrix(&M, &Mt);
    SetRotMatrix(&Mt);
    ppT = &G.pEnemy[2];
    Kind = pAttr->Unk0;
    ppEnd = G.ppEnemy_end;
    if (ppEnd == ppT) {
        return 0;
    }
    Ret = 0;
    do {
        pT = *ppT;
        if ((pT->Be_flg & 1) && pT->Damage_cnt == 0 && !(pT->Type & 0xC000) && pT != pEm) {
            pH = &pT->Hit_area;
            pM = pH->pM;
            V.vx = pM->t[0] - pPos->vx;
            V.vy = pM->t[1] - pPos->vy;
            V.vz = pM->t[2] - pPos->vz;
            ApplyRotMatrixLV(&V, &V);
            if (Bottom <= pT->Hit_area.Top + V.vy && Top >= pH->Bottom + V.vy) {
                Hit = 0;
                w = pAttr->W + pT->Hit_area.R;
                d = pAttr->D + pT->Hit_area.R;
                if ((ULONG)(V.vx + w) < (ULONG)(w * 2)) {
                    Hit = (ULONG)(d * 2) > (ULONG)(V.vz + d);
                }
                if (Hit) {
                    Part = 0;
                    pT->Damage_flg &= 0xC000;
                    pT->Old_routine = *(ULONG *)&pT->Routine_0;
                    pT->Damage_flg |= 4;
                    Dmg = pAttr->Damage;
                    Cnt = pAttr->Damage_cnt;
                    pT->Damage_flg |= 1;
                    Y = pT->Hit_area.Top;
                    H = pH->Bottom - Y;
                    Lo = Y + V.vy + ((H * pT->Hit_area.Rate0) >> 8);
                    Hi = Y + V.vy + ((H * pT->Hit_area.Rate1) >> 8);
                    if (Lo > 0) {
                        pT->Damage_flg = (pT->Damage_flg & 0xFFFC) | 2;
                        Part = 1;
                    }
                    if (Hi > 0) {
                        Part = 2;
                        pT->Damage_flg |= 3;
                    }
                    if (pT->Hit_area.Top + V.vy <= 0) {
                        Part = 0;
                        pT->Damage_flg = (pT->Damage_flg & 0xFFFC) | 1;
                    }
                    if (pH->Bottom + V.vy >= 0) {
                        Part = 2;
                        pT->Damage_flg |= 3;
                    }
                    pT->Damage_no = pAttr->Unk1;
                    pT->Damage_cnt = Cnt;
                    d = Cdir_ck_pos(pPos->vx, pPos->vz, pT);
                    Ret |= 0x20000001;
                    EM_RTN(pT) = (Part << 8) | 2;
                    pT->Damage_flg = pT->Damage_flg | (d << 4) | ((Kind & 0x1F) << 8);
                    pT->Life -= Dmg;
                    if (pT->Life < 0) {
                        EM_RTN(pT) = (Part << 8) | 3;
                        pT->Damage_cnt |= 0x80;
                    }
                }
            }
        }
        ppT++;
    } while (ppT != ppEnd);
    return Ret;
}

void Quake_add(LONG X, LONG Y)
{
    G.Time += Y;
    G.Bonus_time += Y;
    G.Score += X;
    if (G.Bonus_timer == 0) {
        G.Bonus_timer += 0x5A;
    }
}

/* score / time bonus for killing enemy Id (Flg: no combo count) */
void Em_kill_score(ULONG Id, LONG Flg)
{
    UCHAR n;

    n = Id - 0x10;
    if (n == 0x26 && Flg != 0) {
        return;
    }
    if ((Pl_work.Type & 0x1F) == 10) {
        Quake_add((UCHAR)(Em_score_tbl[n][0] / 3), Em_score_tbl[n][1]);
    } else {
        Quake_add(Em_score_tbl[n][0], Em_score_tbl[n][1]);
    }
    if ((Pl_work.Type & 0x1F) == 1) {
        if (n == 0x26) {
            Quake_add(Chaser_kill_score, 0);
        } else {
            Quake_add(Em_score_tbl[n][0], Em_score_tbl[n][1] * 7);
        }
    }
    if (Em_kill_combo) {
        Quake_add(Em_score_tbl[n][0] >> 1, Em_score_tbl[n][1] >> 1);
    }
    if (Em_kill_combo >= 2) {
        Quake_add(Em_score_tbl[n][0], Em_score_tbl[n][1]);
    }
    if (Em_kill_combo >= 3) {
        Quake_add(Em_score_tbl[n][0], Em_score_tbl[n][1]);
    }
    if (!Flg) {
        Em_kill_combo++;
        if (G.Bonus_timer < 15) {
            G.Bonus_timer += 15;
        }
    }
}
