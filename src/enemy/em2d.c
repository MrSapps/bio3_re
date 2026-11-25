#include "enemy/em2d.h"
#include "joint.h"
#include "snd.h"
#include "sld.h"


/* per-frame entry (RE2 EM_MAIN) */
void Em2d_main(ENEMY_WORK *pEm)
{
    SVECTOR Sv; /* unused, as in RE2 */

    if ((G.Unk75EA & 0x7F) && (pEm->Type & 0x10))
        G.Unk75EA--;
    if (Se_call && (pEm->Type & 0x10))
        Se_call--;
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em2d_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* RE2 Em2d_Init */
void Em2d_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP_ptr;
    SVECTOR Sv; /* unused, as in RE2 */
    UCHAR i;
    tagGLOBAL *pG = &G;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    pEm->Unk144 = 0;
    pEm->Spd_x = 0;
    pEm->Spd_z = 0;
    pEm->Spd_y = 0;
    if (pEm->Type & 0x20)
        pEm->Cdir_z = -0x400;
    pG->pPldamage_Em[0x1D] = PLDAMEGE_EM;
    pEm->Timer0 = Rnd() & 0xF;
    EF(pEm)->Dir_y = pEm->Cdir_y;
    EF(pEm)->Pos_x = pEm->Pos_x;
    EF(pEm)->Pos_z = pEm->Pos_z;
    EF(pEm)->Pos_y = pEm->Pos_y;
    EF(pEm)->Se_first = 0;
    pEm->Life = 250;
    if (pEm->Type & 1) {
        pP_ptr = pEm->pSin_parts_ptr + P_hand[1][0];
        pEm->Unk120 = 0;
        pEm->Neck_no = P_hand[1][0];
        PW_NECK(pP_ptr)->Add_dir_y = 0x60;
        PW_NECK(pP_ptr)->Add_dir_z = 0x60;
        PW_NECK(pP_ptr)->L_rot_y = 0x200;
        PW_NECK(pP_ptr)->N_rot_y = 0;
        PW_NECK(pP_ptr)->N_rot_z = 0;
        PW_NECK(pP_ptr)->L_rot_z = 0x138;
        i = 7;
        do {
            pP_ptr = pEm->pSin_parts_ptr + P_hand[0][i - 1];
            pP_ptr->Be_flg = 0;
        } while (--i);
        Hand_display(pEm, 1, 0);
    } else {
        pP_ptr = pEm->pSin_parts_ptr + P_hand[0][0];
        pEm->Unk120 = 0;
        pEm->Neck_no = P_hand[0][0];
        PW_NECK(pP_ptr)->Add_dir_y = 0x60;
        PW_NECK(pP_ptr)->Add_dir_z = 0x60;
        PW_NECK(pP_ptr)->L_rot_y = 0x200;
        PW_NECK(pP_ptr)->N_rot_y = 0;
        PW_NECK(pP_ptr)->N_rot_z = 0;
        PW_NECK(pP_ptr)->L_rot_z = 0x138;
        i = 7;
        do {
            pP_ptr = pEm->pSin_parts_ptr + P_hand[1][i - 1];
            pP_ptr->Be_flg = 0;
        } while (--i);
        Hand_display(pEm, 0, 0);
    }
    pEm->Unk134 = 0;
    pEm->Unk136 = 0;
    pEm->Unk138 = 0;
    pEm->Unk139 = 0;
    pEm->Unk13A = 0;
    pEm->Unk140 = 0;
    Se_call = 0;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    G.Unk75EA = 0;
    EM_SET_MOVE(pEm, 0, 0, 0);
    Joint_move(pEm, pKan, pSeq, 0x100);
}

/* RE2 Em2d_Move */
void Em2d_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em2d_Move_type[pEm->Type & 1](pEm, pKan, pSeq);
}

/* RE2 Em2d_Move_type00 */
void Em2d_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em2d_mv_br_type00[pEm->Routine_1](pEm);
    Em2d_mv_type00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em2d_Move_type01 */
void Em2d_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em2d_mv_br_type01[pEm->Routine_1](pEm);
    Em2d_mv_type01[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em2d_mv00_br (RE3 dropped the grab branch but kept its Goto00_ck call) */
void Em2d_mv00_br(ENEMY_WORK *pEm)
{
    ULONG LL;
    SHORT Dir1;

    LL = pEm->Unk154;
    Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x400);
    Dir1 = Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x2F8);
    if (LL < 3000 && !Dir1 && pEm->Routine_2 == 2) {
        pEm->Routine_2 = 3;
        Hand_display(pEm, pEm->Type & 1, 1);
        pEm->Damage_cnt = 0;
        if (EF(pEm)->Se_first == 1) {
            if (!Se_call) {
                Snd_se_enem(4, pEm);
                Se_call = 60;
            }
        } else {
            EF(pEm)->Se_first = 1;
        }
    }
}

/* RE2 Em2d_mv00 */
void Em2d_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0xF);
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->Routine_2 = 1;
        pEm->Spd_x = 1000;
        pEm->Spd_z = 0;
        pEm->Spd_y = 0;
        pEm->Cdir_y = EF(pEm)->Dir_y;
        pEm->Pos_x = EF(pEm)->Pos_x;
        pEm->Pos_y = EF(pEm)->Pos_y;
        pEm->Pos_z = EF(pEm)->Pos_z;
        pEm->Damage_cnt |= 0x80;
        if (!(pEm->Type & 0x20)) {
            Add_speedXZ(pEm, 0x800);
        } else {
            Em2d_spd_rot(pEm, 0, 0);
            pEm->Pos_x -= pEm->Spd_x;
            pEm->Pos_y -= pEm->Spd_y;
            pEm->Pos_z -= pEm->Spd_z;
        }
        pEm->Timer0 = Rnd() & 0xF;
        EF(pEm)->Rgb = 0;
        Em2d_hand_rgb(pEm, pEm->Type & 1, 0);
    case 1:
        if (--pEm->Timer0 == -1)
            pEm->Routine_2 = 2;
        break;
    case 2:
        break;
    case 3:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            break;
        }
        if (pEm->Move_cnt < 11) {
            pEm->Spd_x = 100;
            pEm->Spd_z = 0;
            pEm->Spd_y = 0;
            if (!(pEm->Type & 0x20)) {
                Add_speedXZ(pEm, 0);
            } else {
                Em2d_spd_rot(pEm, 0, 0);
                pEm->Pos_x += pEm->Spd_x;
                pEm->Pos_y += pEm->Spd_y;
                pEm->Pos_z += pEm->Spd_z;
            }
            Em2d_hand_rgb(pEm, pEm->Type & 1, EF(pEm)->Rgb);
            EF(pEm)->Rgb += 0xC0C0C;
        }
    }
}

/* RE2 Em2d_mv01_br */
void Em2d_mv01_br(ENEMY_WORK *pEm)
{
    PARTS_W *pPt;
    SVECTOR Sv; /* unused, as in RE2 */
    SHORT Ck;

    pPt = pEm->pSin_parts_ptr + P_hand[pEm->Type & 1][2];
    Ck = L_pos_ckXZ((VECTOR *)&G.Player.Pos_x, (VECTOR *)pPt->Workm.t, 1000);
    if (Ck)
        EM_SET_RTN(pEm, 1, 3, 0, 0);
}

/* RE2 Em2d_mv01 */
void Em2d_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Move_tbl[4] = { 1, 2, 3, 5 };

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Move_tbl[Rnd() & 3] + 0xF0000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd() & 0x1F) + 60;
        if (Rnd() & 1) {
            if (Rnd() & 1) {
                Snd_se_enem(0, pEm);
            } else {
                Snd_se_enem(1, pEm);
            }
        }
        pEm->UnkDA = (Rnd() & 0x1F) + 10;
    case 1:
        pEm->Cdir_y += Goto00_ck(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x60);
        if (pEm->Cdir_y > EF(pEm)->Dir_y + 0x180)
            pEm->Cdir_y = EF(pEm)->Dir_y + 0x180;
        if (pEm->Cdir_y < EF(pEm)->Dir_y - 0x180)
            pEm->Cdir_y = EF(pEm)->Dir_y - 0x180;
        if (Joint_move(pEm, pKan, pSeq, 0x100))
            pEm->Hokan_flg = 15;
        if (--pEm->Timer0 == -1)
            EM_SET_RTN(pEm, 1, 5, 0, 0);
        if (--pEm->UnkDA == -1) {
            EM_MOVE(pEm) = Move_tbl[Rnd() & 3] + 0xF0000;
            pEm->UnkDA = (Rnd() & 0x1F) + 10;
        }
    }
}

/* RE2 Em2d_mv02_br */
void Em2d_mv02_br(ENEMY_WORK *pEm)
{
}

/* RE2 Em2d_mv02 */
void Em2d_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* RE2 Em2d_mv03_br */
void Em2d_mv03_br(ENEMY_WORK *pEm)
{
    PARTS_W *pPt;
    SVECTOR Sv; /* unused, as in RE2 */
    SHORT Ck;

    pPt = pEm->pSin_parts_ptr + P_hand[pEm->Type & 1][2];
    Ck = L_pos_ckXZ((VECTOR *)&G.Player.Pos_x, (VECTOR *)pPt->Workm.t, 900);
    if (Ck && pEm->Move_cnt >= 5 && !G.Unk75EA) {
        if (!G.Player.Damage_cnt) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
            G.Unk75EA |= 0x80;
        } else {
            EM_SET_RTN(pEm, 1, 6, 0, 0);
        }
    }
}

/* RE2 Em2d_mv03 */
void Em2d_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 3, 0, 0xF);
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, G.Player.Pos_x, G.Player.Pos_z, 0x30);
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 5, 0, 0);
    }
}

/* RE2 Em2d_mv04_br */
void Em2d_mv04_br(ENEMY_WORK *pEm)
{
}

/* RE2 Em2d_mv04: grab the player */
void Em2d_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pPt;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 0xF);
        pEm->Routine_2 = 1;
        pPt = pEm->pSin_parts_ptr + P_hand[pEm->Type & 1][2];
        pEm->Spd_x = (pPt->Workm.t[0] - G.Player.Pos_x) >> 2;
        pEm->Spd_z = (pPt->Workm.t[2] - G.Player.Pos_z) >> 2;
        pEm->UnkE2 = 4;
        G.Player.pEnemy_ptr = pEm;
        G.Player.pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        *(ULONG *)&G.Player.Routine_0 = 5;
        G.Player.pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        Pad_vib_set0(4, 0);
        Pad_vib_set1(4, 0xFA, 0);
        pEm->UnkDA = 150;
        if (G.Player.Life > 100) /* dead test kept from RE2 */
            pEm->UnkDA = 150;
        pEm->UnkE3 = 0;
        break;
    case 1:
        if (G.Player.Damage_cnt) {
            pEm->UnkE3 = 1;
            G.Unk75EA |= 0xFF7F;
        }
        if (!pEm->Move_cnt && !pEm->UnkE3) {
            Pad_vib_set0(2, 0);
            Pad_vib_set1(1, 0xB4, 0);
            Pad_vib_set1(2, 0xFA, 1);
            if (Rnd() & 1) {
                Snd_se_enem(5, pEm);
            } else {
                Snd_se_enem(7, pEm);
            }
        }
        Joint_move(pEm, pKan, pSeq, 0x100);
        pEm->UnkDA -= (int)Gacha_ch() * 2 + 1;
        if (pEm->UnkDA < 0) {
            pEm->Routine_2 = 2;
            if (!pEm->UnkE3) {
                G.Player.Routine_2 = 2;
                Pad_vib_set0(2, 0);
                Pad_vib_set1(4, 100, 0);
            }
        }
        if (pEm->UnkE2 && !pEm->UnkE3) {
            G.Player.Pos_x += pEm->Spd_x;
            G.Player.Pos_z += pEm->Spd_z;
            pEm->UnkE2--;
        }
        break;
    case 2:
        G.Unk75EA = 60;
        EM_SET_RTN(pEm, 1, 5, 0, 0);
    }
}

/* RE2 Em2d_mv05_br */
void Em2d_mv05_br(ENEMY_WORK *pEm)
{
}

/* RE2 Em2d_mv05: pull back */
void Em2d_mv05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 4, 0, 0xF);
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        Snd_se_enem(6, pEm);
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x100)) {
            pEm->Timer0 = (Rnd() & 0x1F) + 30;
            Hand_display(pEm, pEm->Type & 1, 0);
            EM_SET_RTN(pEm, 1, 0, 0, 0);
            if (pEm->Type & 0x40) {
                EM_SET_RTN(pEm, 1, 7, 0, 0);
                pEm->Life = -1;
                pEm->Damage_cnt |= 0x80;
            }
        }
        pEm->Spd_x = 100;
        pEm->Spd_z = 0;
        pEm->Spd_y = 0;
        if (!(pEm->Type & 0x20)) {
            Add_speedXZ(pEm, 0x800);
        } else {
            Em2d_spd_rot(pEm, 0, 0);
            pEm->Pos_x -= pEm->Spd_x;
            pEm->Pos_y -= pEm->Spd_y;
            pEm->Pos_z -= pEm->Spd_z;
        }
        Em2d_hand_rgb(pEm, pEm->Type & 1, EF(pEm)->Rgb);
        EF(pEm)->Rgb -= 0xC0C0C;
    }
}

/* RE2 Em2d_mv06_br */
void Em2d_mv06_br(ENEMY_WORK *pEm)
{
}

/* RE2 Em2d_mv06 */
void Em2d_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 5, 0, 0xF);
        pEm->Routine_2 = 1;
        break;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x100);
        if (!(G.Player.Damage_cnt & 0x80))
            pEm->Routine_2 = 2;
        break;
    case 2:
        EM_SET_RTN(pEm, 1, 1, 0, 0);
    }
}

/* RE2 Em2d_mv07_br */
void Em2d_mv07_br(ENEMY_WORK *pEm)
{
}

/* RE2 Em2d_mv07 */
void Em2d_mv07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* RE2 Em2d_Damage (RE3 calls Em2d_dm00 directly) */
void Em2d_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv; /* unused, as in RE2 */

    Em2d_dm00(pEm, pKan, pSeq);
}

/* RE2 Em2d_dm00 */
void Em2d_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv; /* unused, as in RE2 */
    MATRIX M;
    VECTOR V;

    EM_SET_RTN(pEm, 1, 5, 0, 0);
    pEm->Damage_cnt |= 0x80;
    if (Rnd() & 1) {
        Snd_se_enem(2, pEm);
    } else {
        Snd_se_enem(3, pEm);
    }
}

/* RE2 Em2d_Die (RE3 calls Em2d_die00 directly) */
void Em2d_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em2d_die00(pEm, pKan, pSeq);
}

/* RE2 Em2d_die00 (RE3 dropped the Esp_call) */
void Em2d_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;

    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    pEm->Life = 250;
    EM_SET_RTN(pEm, 1, 5, 0, 0);
    pEm->Damage_cnt |= 0x80;
    if (Rnd() & 1) {
        Snd_se_enem(2, pEm);
    } else {
        Snd_se_enem(3, pEm);
    }
}

/* RE2 Em2d_Scenario */
void Em2d_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em2d_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em2d_sce00 */
void Em2d_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* RE2 Em2d_Dead */
void Em2d_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
}

/* RE2 PLDIE_EM */
void PLDIE_EM(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldie_Em2d_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Hand_display (RE3 uses the part flags UnkA2 and Be_flg 0x4000) */
void Hand_display(ENEMY_WORK *pEm, UCHAR L_r, UCHAR On_off)
{
    PARTS_W *pP_ptr;
    UCHAR i;

    i = 7;
    do {
        pP_ptr = pEm->pSin_parts_ptr + P_hand[L_r][i - 1];
        if (On_off) {
            pP_ptr->UnkA2 |= 1;
        } else {
            pP_ptr->UnkA2 &= ~1;
        }
    } while (--i);
    if (On_off) {
        pEm->Be_flg &= ~0x4000;
    } else {
        pEm->Be_flg |= 0x4000;
    }
}

/* RE3: colour of the 7 hand parts (consecutive works from P_hand[L_r][0]) */
void Em2d_hand_rgb(ENEMY_WORK *pEm, UCHAR L_r, LONG Rgb)
{
    PARTS_W *pP_ptr;
    UCHAR i;

    i = 7;
    pP_ptr = pEm->pSin_parts_ptr + P_hand[L_r][0];
    do {
        pP_ptr->Poly_rgb = Rgb;
        pP_ptr++;
    } while (--i);
}

/* RE2 PLDAMEGE_EM */
void PLDAMEGE_EM(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldamage_Em2d_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em2d_pldm00 */
void Em2d_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 0xF);
        pEm->Routine_2 = 1;
        Snd_se_stad(((Rnd() & 1) + 1) | 0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk12C = 0;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_SET_MOVE(pEm, 1, 0, 7);
        pEm->Routine_2 = 3;
    case 3:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x100);
        break;
    case 4:
        EM_SET_RTN(pEm, 1, 0, 0, 0);
        G.Player.Damage_cnt &= 0x7F;
        G.Unk75EA &= ~0x80;
    }
}

/* RE3: rotate Spd_x/y/z by (0, Cdir_y + Add_y, Cdir_z + Add_z) */
void Em2d_spd_rot(ENEMY_WORK *pEm, LONG Add_y, LONG Add_z)
{
    SVECTOR Rot;
    MATRIX M;

    Rot.vx = 0;
    Rot.vy = pEm->Cdir_y + Add_y;
    Rot.vz = pEm->Cdir_z + Add_z;
    RotMatrix_gte(&Rot, &M);
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, (SVECTOR *)&pEm->Spd_x);
}
