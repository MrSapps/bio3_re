/*
 * SPL.C (RE2 SPL.C lineage): the sub player (partner character) routine entry, init and routine table
 */
#include "sce0.h"
#include "player.h"
#include "joint.h"
#include "md_sub.h"
#include "pad.h"
#include "sld.h"
#include "spl.h"
#include "spl_auto.h"


static inline ULONG Sce_skip(SCE_TASK *pSce, LONG n)
{
    pSce->Data += n;
    return 1;
}

/* RE2 Spl: sub player per frame: damage timer, weapon sync, root check, routine, water splash, neck */
void Spl(ENEMY_WORK *pEm)
{
    SVECTOR Sv0;
    SVECTOR Sv1;
    ULONG Id;
    PARTS_W *pP;

    if (G.Stop_flg & 0x40000000) {
        return;
    }
    if (pEm->Damage_cnt & 0x7F) {
        pEm->Damage_cnt--;
    }
    Id = pEm->Id;
    if (Id == 0x5B || Id == 0x5F || Id >= 0x62) {
        if (Pl_work.Id < 8) {
            Spl_wep_set(pEm);
        }
    }
    Spl_Root_ck_sys(pEm);
    Spl_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if (pEm->Water != 0 && pEm->Water < pEm->Ground - 0x1F4) {
        pP = pEm->pSin_parts_ptr;
        Sv0.vx = pP->Workm.t[0];
        Sv0.vy = pEm->Water;
        Sv0.vz = pP->Workm.t[2];
        Sv1.vx = 0x400;
        Sv1.vy = 0;
        Sv1.vz = 0;
        if ((pEm->Move_cnt & 3) == 2 && (USHORT)(pEm->Spd_x + 0x14) >= 0x29) {
            Esp_call(0x20E, (((((LONG (*)(void))Rnd)() << 3) + 0x2000) << 16) | (USHORT)pEm->Cdir_y,
                          &GsIDMATRIX, &Sv0, &Sv1);
        }
    }
    Rot_neck(pEm, pEm->Cdir_y);
}

/* RE2 Spl_Init: sets up the sub player (collision, neck limits, part swaps) */
void Spl_Init(ENEMY_WORK *pEm)
{
    USHORT Neck_dir[26] = {
        0x1000, 0x1000, 0x1052, 0x1000, 0x1000, 0x1141, 0x1052, 0x1117, 0x1000, 0x1000, 0x107B, 0x1000, 0x1000,
        0x1000, 0x1052, 0x1000, 0x1052, 0x107B, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
    };
    COL_OBJ *pCol;
    MD_OBJ *pTmd;
    PARTS_W *pP;                       /* clear loop, neck free work, then the weapon part */
    PARTS_W *pHand;
    LONG i;
    USHORT *pSize;

    *(ULONG *)&pEm->Routine_0 = 1;
    pEm->Mot_scale_y = Neck_dir[pEm->Id - 0x50];
    pEm->pEnemy_ptr = pEm;
    pEm->pUnk114 = pEm;
    *(LONG *)&pEm->Move_no = 2;
    Joint_move(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x100);
    Kage_work9_set(&pEm->Kage.pKw9, 0, 0x22601C2, 0, pEm->pSin_parts_ptr->Workm.t);
    pCol = &pEm->Col;
    pCol->pEm = pEm;
    pSize = Spl_size_tbl[pEm->Id];
    pCol->W0 = pCol->D0 = pSize[1];
    pCol->H = 3000;
    pCol->Unk28 = 0;
    pCol->Unk24 = 0;
    pCol->Unk26 = pSize[0];
    pCol->Flg = 0;
    if (pEm == G.pEnemy[1]) {
        pCol->Type = 1;
    } else {
        pCol->Type = 2;
    }
    Oba_set(&pEm->Col, 0);
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[1];
    pEm->Hit_area.Top = 1500;
    pEm->Hit_area.Bottom = -1000;
    pEm->Hit_area.Rate0 = 0x54;
    pEm->Hit_area.Rate1 = 0xA0;
    pEm->Hit_area.R = 450;
    pEm->Unk140 = 0;
    pEm->Neck_flg = 0;
    pEm->Neck_no = 1;
    PW_NECK(pP)->Add_dir_y = 0x40;
    PW_NECK(pP)->Add_dir_z = 0x40;
    PW_NECK(pP)->L_rot_y = 0x200;
    PW_NECK(pP)->L_rot_z = 0x138;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    if (G.Status_flg & 0x80000000) {
        pEm->Life = G.Spl_life + 50;
        if (pEm->Life > 200) {
            pEm->Life = 200;
        }
        pEm->Status_flg = G.Spl_status_flg;
    } else {
        pEm->Life = G.Spl_life = 200;
        pEm->Status_flg = G.Spl_status_flg = 0;
    }
    if (pEm->Type & 0x400) {
        pEm->Type &= ~0x1000;
        G.Status_flg |= 0x80000000;
        pEm->Max_life = 200;
        pEm->pUnk114 = G_PLAYER;
        pEm->Neck_flg = 0;
        pEm->Routine_0 = 1;
        pEm->Routine_1 = 0;
    } else {
        pEm->Type |= 0x5000;
        pEm->Life = pEm->Max_life = -1;
        pEm->Routine_0 = 4;
        pEm->Routine_1 = 6;
    }
    SPL_W(pEm)->Unk1D7 = 0;
    SPL_W(pEm)->Unk1DE = 0;
    Spl_life_u[0] = 0;
    SPL_W(pEm)->Unk1D0 = 0;
    SPL_W(pEm)->Unk1D8 = 0;
    SPL_W(pEm)->Unk1D4 = 0;
    i = pEm->Parts_num;
    while (i >= 16) {
        i--;
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 = 0;
    }
    if ((pEm->Unk4B & 1) && pEm->Id != 0x5B && pEm->Id != 0x5F && pEm->Id < 0x62) {
        pP = &pEm->pSin_parts_ptr[15];
        pHand = &pEm->pSin_parts_ptr[4];
        pTmd = pHand->pTmd;
        pHand->pTmd = pP->pTmd;
        pP->pTmd = pTmd;
    }
}

/* gives the sub player the player's weapon type and its part/offset setup (D_8009E6BC..E4 tables) */
INCLUDE_ASM("asm/nonmatchings/game/spl", Spl_wep_set);

/* RE2 Spl_set: installs Spl as the routine of the sub player work slots */
void Spl_set(void)
{
    G.Em_move_tbl[0x40] = Spl;
    G.Em_move_tbl[0x41] = Spl;
    G.Em_move_tbl[0x42] = Spl;
    G.Em_move_tbl[0x43] = Spl;
    G.Em_move_tbl[0x44] = Spl;
    G.Em_move_tbl[0x45] = Spl;
    G.Em_move_tbl[0x46] = Spl;
    G.Em_move_tbl[0x47] = Spl;
    G.Em_move_tbl[0x48] = Spl;
    G.Em_move_tbl[0x49] = Spl;
    G.Em_move_tbl[0x4A] = Spl;
    G.Em_move_tbl[0x4B] = Spl;
    G.Em_move_tbl[0x4C] = Spl;
    G.Em_move_tbl[0x4D] = Spl;
    G.Em_move_tbl[0x4E] = Spl;
    G.Em_move_tbl[0x4F] = Spl;
    G.Em_move_tbl[0x50] = Spl;
    G.Em_move_tbl[0x51] = Spl;
    G.Em_move_tbl[0x52] = Spl;
    G.Em_move_tbl[0x53] = Spl;
    G.Em_move_tbl[0x54] = Spl;
    G.Em_move_tbl[0x55] = Spl;
    G.Em_move_tbl[0x56] = Spl;
    G.Em_move_tbl[0x57] = Spl;
}

/* thrown back by the work in Add_dir's slot: turn away, play the knock-back moves */
void Spl_Knockback(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pUnk100_kan_t_ptr;
    ULONG *pSeq = pEm->pUnk104_seq_t_ptr;
    ENEMY_WORK *pTg;
    SHORT Dir;
    SHORT Base_x;
    SHORT Base_z;

    G.Status_flg |= 0x100;
    pTg = G.pEnemy[pEm->Unk14A];
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Damage_no = 0;
        *(LONG *)&pEm->Move_no = pEm->Add_dir + 0x70000;
        pEm->Neck_flg |= 0x12;
        pEm->Be_flg |= 0x2000;
    case 1:
        Goto01(pEm, pTg->Cdir_y + 0x800, 0x800);
        Dir = pEm->Cdir_y;
        pEm->Cdir_y = Dir + 0x800;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y = Dir;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            pEm->UnkE3 = 4;
            pEm->Move_no++;
            pEm->Be_flg |= 0x2000;
        }
        break;
    case 2:
        pEm->Cdir_y += 0x800;
        Spd_get(pEm, pKan, pSeq, 0);
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (Joint_move2(pEm, pKan, pSeq, 0x200) && --pEm->UnkE3 == 0) {
            pEm->Routine_2 = 3;
            pEm->Move_no++;
            Pad_vib_set0(2, 0);
            Pad_vib_fade_set(0x2D, 0x96, 0, 0);
        }
        if (*pEm->pNow_seq & 0x200) {
            Pad_vib_set0(4, 0);
            Pad_vib_set1(6, 0xFA, 0);
            Pad_vib_fade_set(0x19, 0x96, 0x28, 6);
        }
        break;
    case 3:
        if (pEm->Move_cnt == 0) {
            Base_x = pEm->Spd_base_x;
            Base_z = pEm->Spd_base_z;
        }
        pEm->Cdir_y += 0x800;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x -= Base_x;
            pEm->Spd_z -= Base_z;
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 4;
        }
        Add_speedXZ(pEm, 0x800);
        if (*pEm->pNow_seq & 0x200) {
            Pad_vib_set0(3, 0);
            Pad_vib_set1(6, 0xC8, 0);
            Pad_vib_set0(2, 8);
            Pad_vib_set1(4, 0xC8, 8);
        }
        break;
    case 4:
        break;
    }
}
