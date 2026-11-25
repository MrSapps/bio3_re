#include "enemy/em30.h"
#include "joint.h"
#include "root_ck.h"
#include "sabun.h"
#include "sce_at.h"
#include "snd.h"

/* Capcom declared these differently in this module: Cdir_ck returning int (the UCHAR result is
   tested without andi), and the sabun call returning a value (Em30_morph_body returns it) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");
LONG func_8004AC54_l(ENEMY_WORK *pEm, PARTS_W *pP) __asm__("Sa_morph_blend");

/* per-frame entry (RE2 EM_MAIN): the head steers and runs its routine; a segment follows its part */
void Em30_main(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl;
    ENEMY_WORK *pOya;
    VECTOR *pTg;
    tagGLOBAL *pG;
    PARTS_W *pP;
    SVECTOR Unused0;
    VECTOR Tg;
    MATRIX M;
    MATRIX Unused1;
    VECTOR Out;
    SVECTOR Unused2;
    LONG y;

    pPl = G_PLAYER;
    if (!(pEm->Type & 1)) {
        Em30_hole_set(pEm);
        if ((ULONG)(pEm->Routine_1 - 7) < 2 || (EM_RTN(pEm) & 0xFFFF00) == 0x20300) {
            if (EM30_W(pEm)->Hole == 1) {
                Tg.vx = -0x4BD4;
                Tg.vz = -0x2C88;
                if (SquareRoot0((pPl->Pos_x + 0x4BD4) * (pPl->Pos_x + 0x4BD4) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                    Tg.vz = -0x1F40;
                pTg = &Tg;
            } else if (EM30_W(pEm)->Hole == 9) {
                Tg.vx = -0x86B;
                Tg.vz = -0x2C88;
                if (SquareRoot0((pPl->Pos_x + 0x86B) * (pPl->Pos_x + 0x86B) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                    Tg.vz = -0x1F40;
                pTg = &Tg;
            } else {
                pTg = POS_V(pPl);
            }
            if ((EM_RTN(pEm) & 0xFFFF00) == 0x20300 && pEm->Spd_x < 0 &&
                (EM30_W(pEm)->Hole == 1 || EM30_W(pEm)->Hole == 9))
                pEm->Spd_x = -pEm->Spd_x;
        } else {
            pTg = POS_V(pPl);
        }
        pEm->Type &= ~0x4000;
        Root_ck(pEm, pTg, 0, 0);
        Em30_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        pEm->pEnemy_ptr = pPl;
        if (pEm->Id != 0x30)
            Rot_neck_em(pEm, pEm->Cdir_y);
        Em_seq_sound(pEm);
        if (EM30_W(pEm)->Morph_spd)
            Em30_morph_body(pEm);
        if (pEm->Routine_0 != 3 && !(G.System_flg & 0x800)) {
            pP = pEm->pSin_parts_ptr;
            if (pP->UnkA2 & 1) {
                pEm->Damage_cnt &= 0x7F;
                pEm->Be_flg &= ~0x20;
                pEm->Type &= ~0x4000;
            } else {
                pEm->Damage_cnt |= 0x80;
                pEm->Be_flg |= 0x20;
                pEm->Type |= 0x4000;
            }
        }
        y = pEm->Pos_y;
        if (y < 0) {
            pEm->Col.Unk26 = -y;
            pEm->Col.H = 800 - y;
        } else {
            pEm->Col.Unk26 = 0;
            pEm->Col.H = 800;
        }
    } else {
        pG = &G;
        pOya = (&pG->pEnemy[0])[EM30_SUB(pEm)->Oya_no];
        if (pOya->Routine_0 != 3 && !(pG->System_flg & 0x800)) {
            pP = &pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no];
            if (pP->UnkA2 & 1) {
                pEm->Damage_cnt &= 0x7F;
                pEm->Kage.pKw->Be_flg |= 0x80;
                pEm->Type &= ~0x4000;
            } else {
                pEm->Damage_cnt |= 0x80;
                pEm->Kage.pKw->Be_flg &= 0x7F;
                pEm->Type |= 0x4000;
            }
        }
        if (pEm->Routine_0 == 2)
            Em30_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
        else if (pEm->Damage_cnt & 0x7F)
            pEm->Damage_cnt--;
        M = GsIDMATRIX;
        MulMatrix2(pEm->Col.pM, &M);
        pEm->Col.Unk26 = 0;
        ApplyMatrix(&M, (SVECTOR *)&pEm->Col.Unk24, &Out);
        y = pEm->Col.pM->t[1] + Out.vy;
        if (y < 0) {
            pEm->Col.Unk26 = -y;
            pEm->Col.H = 800 - y;
        } else {
            pEm->Col.H = 800;
        }
        pEm->Pos_x = pEm->Col.Pos_x;
        pEm->Pos_y = pEm->Col.Pos_y;
        pEm->Pos_z = pEm->Col.Pos_z;
    }
    Em30_dirt_steps(pEm);
    Em30_dirt_line(pEm);
}

/* Routine_0 0: set up the head, its seven segment works and their shadows */
void Em30_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    COL_OBJ *pCol;
    PARTS_W *pP;
    ENEMY_WORK *pSub;
    PARTS_W *pBody;
    UCHAR i;

    EM_SET_RTN(pEm, 1, 0, 0, 0);
    EM30_W(pEm)->u13.Blood_cnt = 0;
    pBody = pEm->pSin_parts_ptr;
    EM30_W(pEm)->Die_flg = 10;
    EM30_W(pEm)->u13.Blood_cnt = 1;
    pEm->Unk16B = 0;
    pEm->Unk16A = 0;
    pEm->Damage_cnt = 0;
    EM30_W(pEm)->No = 0;
    EM30_W(pEm)->Morph_phase = 0;
    EM30_W(pEm)->Head_up = 0;
    EM30_W(pEm)->Morph_dir = 0;
    EM30_W(pEm)->Heavy_hit = 0;
    EM30_W(pEm)->Under = 0;
    EM30_W(pEm)->u10.Dirt = 0;
    EM30_W(pEm)->Hole = 0;
    EM30_W(pEm)->Hit_cnt = 0;
    EM30_W(pEm)->Near_cnt = 0;
    EM30_W(pEm)->Pl_type = 0;
    EM30_W(pEm)->Timer = 0;
    EM30_W(pEm)->Die_flg = 0;
    EM30_W(pEm)->Morph_spd = 0x210;
    EM30_W(pEm)->Unk16 = 0;
    EM30_W(pEm)->Fade_cnt = 0;
    Kage_work_set(&pEm->Kage.pKw, 0, 0x5140514, 0, pBody->Workm.t);
    pEm->Be_flg |= 0x9000;
    pEm->Status_flg |= 0x100;
    pCol = &pEm->Col;
    pCol->W0 = pCol->D0 = 1100;
    pCol->Unk24 = 400;
    pCol->pEm = pEm;
    pCol->H = 800;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    Oba_set(pCol, 0);
    pEm->Hit_area.Top = 1800;
    pEm->Hit_area.Bottom = -1200;
    pEm->Hit_area.Rate0 = 0x78;
    pEm->Hit_area.Rate1 = 0xB4;
    pEm->Hit_area.R = 1000;
    pEm->Neck_no = 8;
    pEm->Unk140 = 0;
    pEm->Neck_flg = 0;
    EM30_W(pEm)->Col_id = pEm->Col.Id |= 0x80;
    pEm->Hit_area.pM = &pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[8];
    PW_NECK(pP)->Add_dir_y = 0x40;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    PW_NECK(pP)->Add_dir_z = 0;
    PW_NECK(pP)->L_rot_y = 0x300;
    PW_NECK(pP)->L_rot_z = 0;
    pEm->Be_flg |= 0xC00000;
    if (G.System_flg & 0x100) {
        pEm->Life = 600;
        pEm->Max_life = 600;
        EM30_W(pEm)->Old_life = 600;
    } else {
        pEm->Life = 800;
        pEm->Max_life = 800;
        EM30_W(pEm)->Old_life = 800;
    }
    pEm->Max_life += 100;
    pEm->Life += 100;
    if (Flg_ck(G.Unk7890, 0xDC))
    {
        pEm->Life *= 3;
        pEm->Life /= 4;
    }
    EM_SET_MOVE(pEm, 0, 0, 0);
    Joint_move(pEm, pKan, pSeq, 0x400);
    pSub = Em30_work_set(pEm);
    pSub->Be_flg |= 0xC00010;
    Em30_sub_init(pSub, 1, pEm, 8);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x5140514, 0, pEm->pSin_parts_ptr[8].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = Em30_work_set(pEm);
    pSub->Be_flg |= 0xC00010;
    Em30_sub_init(pSub, 2, pEm, 1);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x5140514, 0, pEm->pSin_parts_ptr[1].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = Em30_work_set(pEm);
    pSub->Be_flg |= 0xC00010;
    Em30_sub_init(pSub, 3, pEm, 2);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x5140514, 0, pEm->pSin_parts_ptr[2].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = Em30_work_set(pEm);
    pSub->Be_flg |= 0xC00010;
    Em30_sub_init(pSub, 4, pEm, 3);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x44C044C, 0, pEm->pSin_parts_ptr[3].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = Em30_work_set(pEm);
    pSub->Be_flg |= 0xC00010;
    Em30_sub_init(pSub, 5, pEm, 4);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x44C044C, 0, pEm->pSin_parts_ptr[4].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = Em30_work_set(pEm);
    pSub->Be_flg |= 0xC00010;
    Em30_sub_init(pSub, 6, pEm, 5);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x44C044C, 0, pEm->pSin_parts_ptr[5].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    pSub = Em30_work_set(pEm);
    pSub->Be_flg |= 0xC00010;
    Em30_sub_init(pSub, 7, pEm, 6);
    Kage_work_set(&pSub->Kage.pKw, 0, 0x3E803E8, 0, pEm->pSin_parts_ptr[6].Workm.t);
    pSub->Kage.pKw->Be_flg |= 0x80;
    G.pPldie_Em[pEm->Id - 0x10] = Em30_Pldie;
    i = 0;
    do {
        pP = pEm->pSin_parts_ptr + i;
        pP->Be_flg |= 0x20;
    } while (++i < 17);
    EM30_W(pEm)->Se_timer = 0;
    EM30_W(pEm)->Splash_timer = 0;
    pEm->Add_dir = 0;
    i = 0;
    do {
        pP = pEm->pSin_parts_ptr + i;
        pP->UnkA2 &= ~1;
    } while (++i < 18);
    EM_SET_MOVE(pEm, 5, 0, 0);
    pEm->Pos_y += 0x1518;
    G.Unk75EA = 0;
}

/* set up segment work No following part Parts_no of the head pEm */
void Em30_sub_init(ENEMY_WORK *pSub, LONG No, ENEMY_WORK *pEm, LONG Parts_no)
{
    COL_OBJ *pCol;

    pCol = &pSub->Col;
    pSub->Type = 1;
    pSub->Unk16B = 0;
    pSub->Unk16A = 0;
    EM30_W(pSub)->Heavy_hit = 0;
    EM30_W(pSub)->u13.Blood_cnt = 0;
    EM30_W(pSub)->No = No;
    pCol->pEm = pSub;
    pCol->W0 = pCol->D0 = Em30_col_w[Parts_no];
    pCol->H = 800;
    if (Parts_no == 8)
        pCol->Unk24 = 600;
    else
        pCol->Unk24 = 0;
    pCol->Unk28 = 0;
    pCol->Unk26 = 0;
    pCol->Flg = 0;
    pCol->Type = 2;
    Oba_set(pCol, EM30_W(pEm)->Col_id);
    pCol->pM = &pEm->pSin_parts_ptr[Parts_no].Workm;
    pSub->Hit_area.pM = &pEm->pSin_parts_ptr[Parts_no].Workm;
    pSub->Hit_area.Top = 1800;
    pSub->Hit_area.Bottom = -1200;
    pSub->Hit_area.Rate0 = 0x78;
    pSub->Hit_area.Rate1 = 0xB4;
    if (Parts_no == 8)
        pSub->Hit_area.R = 1400;
    else
        pSub->Hit_area.R = 800;
    pSub->Unk140 = 0;
    pSub->Life = 30000;
    pSub->Max_life = 30000;
    EM30_W(pSub)->Old_life = 30000;
    EM30_SUB(pSub)->Parts_no = Parts_no;
    EM30_SUB(pSub)->Oya_no = Em_work_no_get(pEm);
    pEm->pSin_parts_ptr[Parts_no].Unk82 = Em_work_no_get(pSub);
    pSub->Status_flg |= 0x100;
}

/* Routine_0 1 */
void Em30_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    Em30_Move_type(pEm, pKan, pSeq);
}

/* brain routine, then motion routine, by Routine_1 */
void Em30_Move_type(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em30_mv_br[pEm->Routine_1](pEm);
    Em30_mv[pEm->Routine_1](pEm, pKan, pSeq);
}

/* mv_br[0]: start as surfacing from the ground (dive routine, step 3) */
void Em30_appear_br(ENEMY_WORK *pEm)
{
    EM30_W(pEm)->Near_timer = 0;
    pEm->Timer0 = 60;
    Em30_parts_mask(pEm);
    EM_SET_RTN(pEm, 1, 3, 3, 0);
}

/* mv[0], unused slots */
void Em30_nop(ENEMY_WORK *pEm)
{
}

/* mv_br[1]: pick the next move by the distance to the player */
void Em30_crawl_br(ENEMY_WORK *pEm)
{
    UCHAR Se[2] = {9, 6};

    if ((LONG)pEm->L_pl < 4500 && !EM30_W(pEm)->Morph_dir)
        EM_SET_RTN(pEm, 1, 6, 0, 0);
    if ((LONG)pEm->L_pl < 4500 && !EM30_W(pEm)->Head_up)
        EM_SET_RTN(pEm, 1, 2, 0, 0);
    if ((LONG)pEm->L_pl > 7000 && (LONG)pEm->L_pl < 8000 && (Pl_work.Life > 80 || !EM30_W(pEm)->Head_up))
        EM_SET_RTN(pEm, 1, 4, 0, 0);
    if ((LONG)pEm->L_pl < 5000) {
        if (Pl_work.Life < 80 && !((Rnd() >> (Rnd() & 3)) & 3)) {
            EM_SET_RTN(pEm, 1, 5, 0, 0);
            if (!EM30_W(pEm)->Head_up)
                EM_SET_RTN(pEm, 1, 2, 0, 0);
        } else if (EM30_W(pEm)->Head_up) {
            EM_SET_RTN(pEm, 1, 4, 0, 0);
        }
    }
    if ((Pl_work.Col.Unk3A & 0x3FFF) || (LONG)pEm->L_pl < 1200) {
        EM30_W(pEm)->Near_timer++;
        if (EM30_W(pEm)->Spd >= 2)
            EM30_W(pEm)->Spd -= 2;
        if (EM30_W(pEm)->Near_timer > 60) {
            EM30_W(pEm)->Near_timer = 0;
            EM_SET_RTN(pEm, 1, 3, 0, 0);
        }
    } else {
        EM30_W(pEm)->Near_timer = 0;
    }
    if (!EM30_W(pEm)->Se_timer) {
        Snd_se_enem(Se[EM30_W(pEm)->Under], pEm);
        EM30_W(pEm)->Se_timer = 25;
    } else {
        EM30_W(pEm)->Se_timer--;
    }
}

/* mv[1]: crawl towards the target */
void Em30_crawl(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[4] = {0, 1, 2, 3};
    UCHAR Se[2] = {9, 6};

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE_V(pEm, Mot[EM30_W(pEm)->Morph_dir * 2 + EM30_W(pEm)->Head_up], 0, 0x1F);
        pEm->Routine_2 = 1;
        EM30_W(pEm)->Spd = 40;
        EM30_W(pEm)->Spd_lim = 80;
        EM30_W(pEm)->Spd_add = 2;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Joint_move(pEm, pKan, pSeq, 0x80);
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
        if (EM30_W(pEm)->Spd_add > 0 && EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim)
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
        Em30_body_chain(pEm);
    }
}

/* no brain step for this move */
void Em30_rear_br(ENEMY_WORK *pEm)
{
}

/* mv[2]: rear up or lower the head (Head_up toggles at the end) */
void Em30_rear(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = {0xB, 0xA};
    SVECTOR Sv;
    UCHAR Se[2] = {9, 6};
    ENEMY_WORK *pPl = &Pl_work;

    switch (pEm->Routine_2) {
    case 0:
        EM30_W(pEm)->Morph_dir = Rnd() & 1;
        if (EM30_W(pEm)->Head_up && EM30_W(pEm)->Morph_dir)
            EM30_W(pEm)->Morph_dir = 0;
        EM_SET_MOVE_V(pEm, Mot[EM30_W(pEm)->Morph_dir], 0, 0x1F);
        pEm->Routine_2 = 1;
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 8);
        if (Joint_move(pEm, pKan, pSeq, (EM30_W(pEm)->Head_up << 16) | 0x80)) {
            EM30_W(pEm)->Head_up ^= 1;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            if (EM30_W(pEm)->Head_up) {
                Snd_se_enem(4, pEm);
                Sv.vx = 850;
                Sv.vy = 660;
                Sv.vz = -420;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x14000000, &GsIDMATRIX, &Sv, NULL);
                Sv.vx = 850;
                Sv.vy = 670;
                Sv.vz = -440;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x12000000, &GsIDMATRIX, &Sv, NULL);
            }
        }
        pEm->Spd_x = 5;
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        Em30_body_chain(pEm);
    }
    if (!EM30_W(pEm)->Se_timer) {
        Snd_se_enem(Se[EM30_W(pEm)->Under], pEm);
        EM30_W(pEm)->Se_timer = 25;
    } else {
        EM30_W(pEm)->Se_timer--;
    }
}

/* no brain step for this move */
void Em30_dive_br(ENEMY_WORK *pEm)
{
}

/* mv[3]: dive into the ground and come up again somewhere else */
void Em30_dive(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    VECTOR Pos;
    UCHAR Se[2] = {9, 6};
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG i;
    LONG Flg;
    LONG Zone;
    LONG Dist;
    LONG n;

    switch (pEm->Routine_2) {
    case 0:
        EM30_W(pEm)->Under = 1;
        pEm->Routine_2 = 1;
        EM_SET_MOVE(pEm, 4, 0, 0x1F);
        pEm->Spd_x = 10;
        pEm->Spd_z = 0;
        pP = pEm->pSin_parts_ptr;
        for (i = 17; i != 0; i--) {
            pP->UnkA2 |= 0x400;
            pP++;
        }
        EM30_W(pEm)->Dirt_part = 8;
        if (G.System_flg & 0x100)
            EM30_W(pEm)->Dive_cnt = (Rnd() & 1) + 2;
        else
            EM30_W(pEm)->Dive_cnt = (Rnd() & 1) + (Rnd() & 1) + 3;
        pEm->UnkE2 = 0;
    case 1:
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pP = &pEm->pSin_parts_ptr[5];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pP = &pEm->pSin_parts_ptr[6];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pEm->Routine_2 = 2;
            pEm->Timer0 = 50;
            EM_SET_MOVE(pEm, 0, 0, 0);
            pEm->Spd_x = 150;
            pEm->Spd_z = 0;
            pEm->Pos_y += 0x1518;
            G.Status_flg &= ~4;
            if (++pEm->Add_dir > 100)
                pEm->Add_dir = 100;
        }
        Add_speedXZ(pEm, 0);
        Em30_body_chain(pEm);
        if (pEm->Move_cnt == 60) {
            Pad_vib_data_set(Em30_vib_tbl, 4);
            G.Status_flg |= 4;
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 &= ~1;
        }
        if (pEm->Move_cnt == 70) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 &= ~1;
            EM30_W(pEm)->Dirt_part = 0;
            pEm->Be_flg |= 0x20;
        }
        if (pEm->Move_cnt == 77) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Dirt_part = 1;
        }
        if (pEm->Move_cnt == 86) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Dirt_part = 2;
            Snd_se_enem(10, pEm);
        }
        if (pEm->Move_cnt == 96) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Dirt_part = 3;
        }
        if (pEm->Move_cnt == 40) {
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 100);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 150);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 200);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 250);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 300);
            Snd_se_enem(7, pEm);
            Snd_se_enem(10, pEm);
        }
        if (pEm->Move_cnt >= 41)
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 100);
        break;
    case 2:
        EM30_W(pEm)->Near_cnt = 0;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (!pEm->Timer0) {
            Flg = 0;
            if (Flg_ck(&G.Unk7888, 4))
                Flg = pEm->Pos_z < -15000;
            if (EM30_W(pEm)->Dive_cnt) {
                EM30_W(pEm)->Hole = 0;
                EM30_W(pEm)->Timer = 0;
                EM_SET_RTN(pEm, 1, 7, 4, 0);
            } else {
                if ((LONG)pEm->L_pl > 2500 && !Flg) {
                    pEm->Routine_2 = 3;
                    EM_SET_MOVE(pEm, 5, 0, 0);
                    EM30_W(pEm)->Dirt_part = 8;
                    pEm->Timer0 = 45;
                } else {
                    if (EM30_W(pEm)->Hole != 1 && EM30_W(pEm)->Hole != 9) {
                        if (pEm->Spd_x < 0) {
                            if (pEm->Spd_x > -100)
                                pEm->Spd_x = -300;
                        } else if (pEm->Spd_x < 100) {
                            pEm->Spd_x = 300;
                        }
                        Dist = pEm->L_pl;
                        if (Dist < 2500)
                            Zone = 1;
                        else
                            Zone = (Dist > 6500) * 2;
                        if ((ULONG)(Dist - 2500) > 4000 && Zone == EM30_W(pEm)->Zone) {
                            if (Dist > 6500) {
                                pEm->Spd_x += (Rnd() & 0x1F) - 15;
                                if ((LONG)pEm->L_pl >= EM30_W(pEm)->Old_dist)
                                    pEm->Spd_x = -pEm->Spd_x;
                            }
                            if ((LONG)pEm->L_pl < 2500) {
                                pEm->Spd_x += (Rnd() & 0x1F) - 15;
                                if (EM30_W(pEm)->Old_dist >= (LONG)pEm->L_pl)
                                    pEm->Spd_x = -pEm->Spd_x;
                            }
                        }
                        EM30_W(pEm)->Zone = Zone;
                        EM30_W(pEm)->Old_dist = pEm->L_pl;
                    }
                    pEm->Timer0 = (Rnd() + 30) & 0x1F;
                }
            }
        } else {
            pEm->Timer0--;
        }
        Em30_body_chain(pEm);
        if (!EM30_W(pEm)->Se_timer) {
            Snd_se_enem(Se[EM30_W(pEm)->Under], pEm);
            EM30_W(pEm)->Se_timer = 25;
        } else {
            EM30_W(pEm)->Se_timer--;
        }
        break;
    case 3:
        if (pEm->Timer0 == 45) {
            Pos.vx = pEm->Pos_x;
            Pos.vy = 0;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x10210, &Pos, 0, 0);
            G.Status_flg |= 2;
        }
        if (pEm->Timer0 == 42)
            G.Status_flg &= ~2;
        if (pEm->Timer0) {
            if (pEm->Timer0 >= 39 && pEm->Timer0 < 45)
                Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 100);
            else if (pEm->Timer0 < 10)
                Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 100);
            pEm->Timer0--;
        } else {
            pEm->Pos_y -= 0x1518;
            Joint_move(pEm, pKan, pSeq, 0x80);
            Snd_se_enem(8, pEm);
            Snd_se_enem(10, pEm);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 100);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 150);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 200);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 250);
            Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 300);
            pEm->Spd_x = 40;
            pEm->Timer0 = 0;
            pEm->UnkE2 = 0;
            pEm->Routine_2++;
            Pad_vib_data_set(Em30_vib_tbl, 3);
            G.Status_flg |= 4;
        }
        break;
    case 4:
        if (pEm->Move_cnt < 10)
            Em30_blood_part(pEm, 14);
        Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 100);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM30_W(pEm)->Under = 0;
            EM30_W(pEm)->Morph_dir = 0;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
            pP = pEm->pSin_parts_ptr;
            for (i = 17; i != 0; i--) {
                pP->UnkA2 &= ~0x400;
                pP++;
            }
            G.Status_flg &= ~4;
        }
        Add_speedXZ(pEm, 0);
        Em30_body_chain(pEm);
        if (pEm->Move_cnt == 2) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 6) {
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
        }
        if (pEm->Move_cnt == 7) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 1;
            EM30_W(pEm)->Dirt_part = 0;
            pEm->Be_flg &= ~0x20;
        }
        if (pEm->Move_cnt == 15) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 1;
        }
        if (pEm->Move_cnt == 34) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 2;
        }
        if (pEm->Move_cnt == 35)
            Snd_se_enem(4, pEm);
        if (pEm->Move_cnt == 50) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 3;
        }
        if (pEm->Move_cnt == 71) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 5;
        }
        if (pEm->Move_cnt == 40 || pEm->Move_cnt == 80)
            Snd_se_enem(10, pEm);
        if (pEm->Move_cnt == 99) {
            pP = &pEm->pSin_parts_ptr[6];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 6;
        }
        if (pEm->Move_cnt >= 31) {
            Sv.vx = 600;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 1400;
            if (!pPl->Damage_cnt && Hit_area_ck(&pEm->pSin_parts_ptr[9].Workm, &Sv, pPl) && !pEm->Timer0) {
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl);
                pPl->Routine_2 = 0;
                pEm->Timer0 = 1;
                pEm->UnkE2 = 5;
            }
        }
        if (pEm->UnkE2) {
            n = pEm->UnkE2;
            pPl->Spd_x = Cdir_ck_l(pEm, pPl) ? n * 20 : -(n * 20);
            pPl->Spd_z = 0;
            Add_speedXZ(pPl, 0);
            pEm->UnkE2--;
        }
        break;
    }
}

/* dirt (or a splash in water) around part No */
void Em30_dirt_spray(ENEMY_WORK *pEm, LONG No, LONG Big)
{
    SVECTOR Sv;
    LONG Col;
    LONG Kind;
    LONG Size;

    Sv.vx = (Rnd() & 0xFF) - 0x7F;
    Sv.vy = -((Rnd() & 0xFF) + Big);
    Sv.vz = (Rnd() & 0xFF) - 0x7F;
    Sv.vx <<= 3;
    Sv.vz <<= 3;
    Col = (Rnd() & 0xFF) << 4;
    if (Big)
        Kind = Rnd() & 0xF;
    else
        Kind = (Rnd() & 3) + (Rnd() & 1);
    Size = ((Rnd() & 3) + 2) * 5;
    Sv.vx += pEm->pSin_parts_ptr[No].Workm.t[0];
    Sv.vz += pEm->pSin_parts_ptr[No].Workm.t[2];
    if (Water_ck(Sv.vx, Sv.vz)) {
        Esp_call(0xE1C, (Size << 24) | Col, &GsIDMATRIX, &Sv, NULL);
    } else {
        Esp_call((Kind << 8) | 0x36, (Size << 24) | Col, &GsIDMATRIX, &Sv, NULL);
        Esp_call(0x4003103, (Size << 24) | Col, &GsIDMATRIX, &Sv, NULL);
    }
}

/* dirt effect around part No; Big: larger and higher */
void Em30_dirt_puff(ENEMY_WORK *pEm, LONG No, LONG Big)
{
    SVECTOR Sv;
    LONG Size;
    LONG Col;

    Sv.vx = (Rnd() & 0xFF) - 0x7F;
    Sv.vy = -((Rnd() & 0xFF) + Big);
    Sv.vz = (Rnd() & 0xFF) - 0x7F;
    if (Big) {
        Sv.vz <<= 3;
        Sv.vx <<= 3;
        Size = ((Rnd() & 3) + 2) * 5 << 8;
    } else {
        Sv.vz <<= 2;
        Sv.vx <<= 2;
        Size = ((Rnd() & 3) + 2) * 3 << 8;
    }
    Col = (Rnd() & 0xFF) << 4;
    Sv.vx += pEm->pSin_parts_ptr[No].Workm.t[0];
    Sv.vz += pEm->pSin_parts_ptr[No].Workm.t[2];
    Esp_call(0x4003103, (Size << 16) | Col, &GsIDMATRIX, &Sv, NULL);
}

/* blood effect on part No */
void Em30_blood_part(ENEMY_WORK *pEm, LONG No)
{
    SVECTOR Sv;

    Sv.vx = 0;
    Sv.vy = 0;
    Sv.vz = 0;
    Esp_call(0x36, (USHORT)pEm->Cdir_y | 0x14000000, &pEm->pSin_parts_ptr[No].Workm, &Sv, NULL);
}

/* flag 0x400 on every part but the odd ones from 9 on */
void Em30_parts_mask(ENEMY_WORK *pEm)
{
    LONG i;
    PARTS_W *pP;

    for (i = 0; i < 17; i++) {
        if (i >= 9 && (i & 1))
            i++;
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 |= 0x400;
    }
}

/* the body comes out: parts 0..8 shown (dirt where a part breaks the surface) and their shadows on */
void Em30_surface(ENEMY_WORK *pEm)
{
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[1];
    pEm->Be_flg &= ~0x20;
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 1, 100);
        Em30_dirt_puff(pEm, 1, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[2];
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 2, 100);
        Em30_dirt_puff(pEm, 2, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[3];
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 3, 100);
        Em30_dirt_puff(pEm, 3, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[4];
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 4, 100);
        Em30_dirt_puff(pEm, 4, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[5];
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 5, 100);
        Em30_dirt_puff(pEm, 5, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[6];
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 6, 100);
        Em30_dirt_puff(pEm, 6, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[7];
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 7, 100);
        Em30_dirt_puff(pEm, 7, 150);
    }
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[8];
    if (!(pP->UnkA2 & 1)) {
        Em30_dirt_puff(pEm, 8, 100);
        Em30_dirt_puff(pEm, 8, 150);
    }
    pP->UnkA2 |= 1;
    G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
    pP = &pEm->pSin_parts_ptr[10];
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[12];
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[14];
    pP->UnkA2 |= 1;
    pP = &pEm->pSin_parts_ptr[16];
    pP->UnkA2 |= 1;
}

/* no brain step for this move */
void Em30_bite_br(ENEMY_WORK *pEm)
{
}

/* mv[4]: bite at the player (Head_up: bite from above) */
void Em30_bite(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    UCHAR Mot[2] = {0xC, 0xD};
    UCHAR Se[2] = {9, 6};
    HIT_AREA Area;
    ENEMY_WORK *pPl = &Pl_work;
    LONG n;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE_V(pEm, Mot[EM30_W(pEm)->Head_up], 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Spd_x = 100;
        pEm->Spd_z = 0;
        if (!EM30_W(pEm)->Head_up) {
            EM30_W(pEm)->Spd_lim = 200;
            EM30_W(pEm)->Spd = 100;
            EM30_W(pEm)->Spd_add = 10;
        }
    case 1:
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            if (EM30_W(pEm)->Head_up == 1) {
                EM30_W(pEm)->Head_up = 0;
                EM_SET_RTN(pEm, 1, 3, 0, 0);
            } else {
                EM_SET_RTN(pEm, 1, 1, 0, 0);
            }
        }
        if ((pEm->Move_cnt == 24 || pEm->Move_cnt == 42) && !pEm->UnkDA)
            Snd_se_enem(0, pEm);
        if ((*pEm->pNow_seq >> 10) & 1) {
            if (EM30_W(pEm)->Head_up == 1) {
                Sv.vx = 2000;
                Sv.vy = 0;
                Sv.vz = 1400;
                Sv.pad = 1400;
            } else {
                Sv.vx = 1800;
                Sv.vy = 0;
                Sv.vz = 1400;
                Sv.pad = 1400;
            }
            Area.pPos = WORKM_POS_V(&pEm->pSin_parts_ptr[8]);
            Area.pBox = &Sv.vx;
            Area.Dir = pEm->Cdir_y;
            Area.Id = 0x85;
            Hit_area_em_ck(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
        }
        if ((*pEm->pNow_seq >> 9) & 1) {
            Sv.vx = 400;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 800;
            if (!pPl->Damage_cnt && Hit_area_ck(&pEm->pSin_parts_ptr[9].Workm, &Sv, pPl) && !pEm->UnkDA) {
                Snd_se_enem(1, pEm);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl) + 2;
                pPl->Routine_2 = 0;
                pEm->UnkDA = 1;
                if (Pl_life_down(35, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
                pEm->UnkE2 = 5;
            }
        }
        if (!EM30_W(pEm)->Head_up) {
            if (pEm->Move_cnt == 12) {
                EM30_W(pEm)->Spd_add = -20;
                EM30_W(pEm)->Spd_lim = 100;
            }
            if (pEm->Move_cnt == 28) {
                if (pEm->UnkDA) {
                    EM30_W(pEm)->Head_up = 0;
                    EM_SET_RTN(pEm, 1, 3, 0, 0);
                }
                EM30_W(pEm)->Spd_add = 40;
                EM30_W(pEm)->Spd_lim = 350;
            }
            if (pEm->Move_cnt >= 29 && pEm->UnkDA) {
                EM30_W(pEm)->Spd_add = -50;
                EM30_W(pEm)->Spd_lim = 50;
            }
            if (pEm->Move_cnt == 38 && !pEm->UnkDA) {
                EM30_W(pEm)->Spd_add = -50;
                EM30_W(pEm)->Spd_lim = 100;
            }
            pEm->Spd_x = EM30_W(pEm)->Spd;
            EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
            if (EM30_W(pEm)->Spd_add > 0) {
                if (EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim)
                    EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
            } else if (EM30_W(pEm)->Spd < EM30_W(pEm)->Spd_lim) {
                EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
            }
        }
        Add_speedXZ(pEm, 0);
        Em30_body_chain(pEm);
        if (pEm->UnkE2) {
            EM30_W(pEm)->Spd = 10;
            EM30_W(pEm)->Spd_add = 0;
            Sv.vx = Rnd() & 0x3F;
            Sv.vy = 150;
            Sv.vz = 0;
            Esp_call(0, (USHORT)pPl->Cdir_y | 0x14000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, NULL);
            n = pEm->UnkE2;
            pPl->Spd_x = Cdir_ck_l(pEm, pPl) ? n * 30 : -(n * 30);
            pPl->Spd_z = 0;
            Add_speedXZ(pPl, 0);
            pEm->UnkE2--;
        }
    }
    if (!EM30_W(pEm)->Se_timer) {
        Snd_se_enem(Se[EM30_W(pEm)->Under], pEm);
        EM30_W(pEm)->Se_timer = 25;
    } else {
        EM30_W(pEm)->Se_timer--;
    }
}

/* no brain step for this move */
void Em30_swallow_br(ENEMY_WORK *pEm)
{
}

/* mv[5]: snap up the player in front (or miss) */
void Em30_swallow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SVECTOR Sv;

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0xE, 0, 0x1F);
        pEm->Routine_2 = 1;
        pEm->UnkDA = 0;
        pEm->Spd_x = 100;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80))
            pEm->Routine_2 = 4;
        Sv.vx = 800;
        Sv.vy = 0;
        Sv.vz = 0;
        Sv.pad = 1000;
        if (Hit_area_ck(&pEm->pSin_parts_ptr[8].Workm, &Sv, pPl) && !pEm->UnkDA)
            pEm->UnkDA = 1;
        if (pEm->Move_cnt == 42) {
            if (pEm->UnkDA) {
                Snd_se_enem(1, pEm);
                pPl->Be_flg |= 0x800;
                pPl->Damage_cnt |= 0x80;
                pEm->Be_flg |= 0x800;
                pEm->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[8].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[1].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[2].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[3].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[4].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Damage_cnt |= 0x80;
                G.pEnemy[pEm->pSin_parts_ptr[6].Unk82]->Damage_cnt |= 0x80;
                pPl->Routine_0 = 6;
                pPl->Routine_1 = 0;
                pPl->Routine_2 = 0;
                pPl->pEnemy_ptr = pEm;
                pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
                pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
                pPl->Neck_flg |= 1;
                G.System_flg |= 0x20800;
                Pad_vib_set0(4, 0);
                Pad_vib_set0(4, 0x1B);
                Pad_vib_fade_set(0x1A, 0xE4, 0x3C, 0);
                Pad_vib_fade_set(0x1C, 0xDC, 0xC, 0x1B);
                Pad_vib_fade_set(0xC, 0xB8, 0xD8, 0x3D);
                Pad_vib_fade_set(0x2A, 0xD8, 0x10, 0x49);
            } else {
                pEm->Routine_2++;
            }
        }
        if (pEm->UnkDA && ++pEm->UnkDA < 25) {
            Sv.vx = 700;
            Sv.vy = Rnd() - 0x80;
            Sv.vz = Rnd() - 0x80;
            Esp_call(0, ((((Rnd() & 7) << 7) + 0x1400) << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[8].Workm, &Sv, NULL);
        }
        Add_speedXZ(pEm, 0);
    case 4:
        Em30_body_chain(pEm);
        break;
    case 2:
        EM_SET_MOVE(pEm, 0xF, 0, 0x1F);
        pEm->Routine_2 = 3;
        pEm->UnkDA = 0;
        pEm->Spd_x = 100;
        pEm->Spd_z = 0;
    case 3:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM30_W(pEm)->Head_up = 0;
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        }
        Add_speedXZ(pEm, 0);
        Em30_body_chain(pEm);
    }
}

/* no brain step for this move */
void Em30_lunge_br(ENEMY_WORK *pEm)
{
}

/* mv[6]: lunge, snapping the jaws (with dirt) at frame 55 */
void Em30_lunge(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = {9, 10};
    SVECTOR Sv;
    UCHAR Se[2] = {9, 6};

    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE_V(pEm, Mot[EM30_W(pEm)->Head_up], 0, 0x1F);
        pEm->Routine_2 = 1;
        EM30_W(pEm)->Spd = 40;
        EM30_W(pEm)->Spd_add = -5;
        pEm->Timer0 = EM30_W(pEm)->Morph_dir;
        EM30_W(pEm)->Spd_lim = 0;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        if (pEm->Move_cnt == 55) {
            if (EM30_W(pEm)->Morph_dir ^= 1) {
                Snd_se_enem(4, pEm);
                Sv.vx = 850;
                Sv.vy = 660;
                Sv.vz = -420;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x14000000, &GsIDMATRIX, &Sv, NULL);
                Sv.vy = 670;
                Sv.vx = 850;
                Sv.vz = -440;
                Rot_vector(pEm->Cdir_y, &Sv, &Sv);
                {
                    PARTS_W *pP = &pEm->pSin_parts_ptr[8];

                    Sv.vx += pP->Workm.t[0];
                    Sv.vy += pP->Workm.t[1];
                    Sv.vz += pP->Workm.t[2];
                }
                Esp_call(0x39, 0x12000000, &GsIDMATRIX, &Sv, NULL);
            }
        }
        if (pEm->Move_cnt >= 56)
            Em30_morph_step(pEm, 0x80, EM30_W(pEm)->Morph_dir);
        if (Joint_move(pEm, pKan, pSeq, (pEm->Timer0 << 16) | 0x80))
            EM_SET_RTN(pEm, 1, 1, 0, 0);
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
        if (EM30_W(pEm)->Spd_add > 0) {
            if (EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim)
                EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
            if (!EM30_W(pEm)->Se_timer) {
                Snd_se_enem(Se[EM30_W(pEm)->Under], pEm);
                EM30_W(pEm)->Se_timer = 25;
            } else {
                EM30_W(pEm)->Se_timer--;
            }
        } else if (EM30_W(pEm)->Spd < EM30_W(pEm)->Spd_lim) {
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
        }
        Em30_body_chain(pEm);
    }
}

/* mv_br[7]: growl; after 500 frames go back to the hole (mv 8) */
void Em30_ambush_br(ENEMY_WORK *pEm)
{
    UCHAR Se[2] = {9, 6};

    if (pEm->Routine_2 == 4) {
        if (!EM30_W(pEm)->Se_timer) {
            Snd_se_enem(Se[EM30_W(pEm)->Under], pEm);
            EM30_W(pEm)->Se_timer = 25;
        } else {
            EM30_W(pEm)->Se_timer--;
        }
    }
    if (EM30_W(pEm)->Timer < 500)
        EM30_W(pEm)->Timer++;
    else if (pEm->Routine_2 == 4)
        EM_SET_RTN(pEm, 1, 8, 0, 0);
}

/* mv[7]: wait underground, then burst out under the player */
void Em30_ambush(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    VECTOR Unused;
    HIT_AREA Area;
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    LONG i;
    LONG Dist;
    LONG Zone;
    LONG Flg;
    LONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = (Rnd() + 15) & 0xF;
        EM30_W(pEm)->Old_dist = pEm->L_pl;
        {
            LONG Area_no;

            if ((LONG)pEm->L_pl >= 4500)
                Area_no = ((LONG)pEm->L_pl > 10000) * 2;
            else
                Area_no = 1;
            EM30_W(pEm)->Zone = Area_no;
        }
        EM30_W(pEm)->Near_cnt = 0;
        pEm->Routine_2++;
    case 1:
        if (pEm->Timer0 == 15)
            G.Status_flg |= 2;
        if (pEm->Timer0 == 5)
            G.Status_flg &= ~2;
        if (pEm->Timer0 >= 9)
            Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 100);
        if (!pEm->Timer0 || !--pEm->Timer0) {
            pEm->UnkE3 = 0;
            pEm->Routine_2++;
        }
        break;
    case 2:
        EM_SET_MOVE(pEm, 0x11, 0, 0x1F);
        pEm->UnkDA = 0;
        pEm->UnkE2 = 0;
        pEm->Routine_2++;
        pEm->Pos_y -= 0x1518;
        Snd_se_enem(8, pEm);
        Snd_se_enem(10, pEm);
        Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 100);
        Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 150);
        Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 200);
        Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 250);
        Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 300);
        for (i = 0; i < 17; i++) {
            pP = &pEm->pSin_parts_ptr[i];
            pP->Be_flg &= ~2;
        }
        EM30_W(pEm)->Dirt_part = 8;
        Pad_vib_data_set(Em30_vib_tbl, 2);
    case 3:
        if (pEm->Move_cnt < 10)
            Goto00(pEm, pPl->Pos_x, pPl->Pos_z, 0x20);
        Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 100);
        if (pEm->Move_cnt == 0) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 4) {
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
        }
        if (pEm->Move_cnt == 7) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 1;
            EM30_W(pEm)->Dirt_part = 0;
            pEm->Be_flg &= ~0x20;
        }
        if (pEm->Move_cnt == 8)
            Snd_se_enem(4, pEm);
        if (pEm->Move_cnt == 11) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 1;
        }
        if (pEm->Move_cnt == 17) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 2;
        }
        if (pEm->Move_cnt == 30) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 |= 1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg |= 0x80;
            EM30_W(pEm)->Dirt_part = 3;
        }
        if (pEm->Move_cnt == 49) {
            pP = &pEm->pSin_parts_ptr[3];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Dirt_part = 2;
        }
        if (pEm->Move_cnt == 51) {
            pP = &pEm->pSin_parts_ptr[2];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Dirt_part = 1;
            Snd_se_enem(10, pEm);
        }
        if (pEm->Move_cnt == 54) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Dirt_part = 0;
        }
        if (pEm->Move_cnt == 61) {
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 &= ~1;
            G.pEnemy[pP->Unk82]->Kage.pKw->Be_flg &= 0x7F;
            EM30_W(pEm)->Dirt_part = 8;
        }
        if (pEm->Move_cnt == 63) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 &= ~1;
            pEm->Be_flg |= 0x20;
        }
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 &= ~1;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 &= ~1;
            pEm->Pos_y += 0x1518;
            if (--EM30_W(pEm)->Dive_cnt) {
                EM_SET_MOVE(pEm, 0, 0, 0);
                EM30_W(pEm)->u10.Dirt = Rnd() & 3;
                pEm->Routine_2++;
            } else {
                pEm->Timer0 = 90;
                EM_SET_RTN(pEm, 1, 3, 2, 0);
                EM30_W(pEm)->Hole = 0;
                EM_SET_MOVE(pEm, 0, 0, 0);
                pEm->Spd_x = 150;
                pEm->Spd_z = 0;
                G.Status_flg &= ~4;
            }
        }
        if (pEm->Move_cnt && !pEm->UnkDA) {
            if (pEm->Move_cnt < 7) {
                Sv.vx = 3000;
                Sv.vz = 1700;
                Sv.vy = 0;
                Sv.pad = 1500;
                Area.pPos = WORKM_POS_V(&pEm->pSin_parts_ptr[8]);
                Area.pBox = &Sv.vx;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x85;
                Hit_area_em_ck(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
            } else if (pEm->Move_cnt < 18) {
                Sv.vx = 2000;
                Sv.vz = 1200;
                Sv.vy = 0;
                Sv.pad = 1400;
                Area.pPos = WORKM_POS_V(&pEm->pSin_parts_ptr[8]);
                Area.pBox = &Sv.vx;
                Area.Dir = pEm->Cdir_y;
                Area.Id = 0x85;
                Hit_area_em_ck(&pEm->Pos_x, &pEm->Pos_x, &Area, pPl);
            }
        }
        if (pEm->Move_cnt >= 8 && pEm->Move_cnt < 22) {
            Sv.vx = 1000;
            Sv.vy = 0;
            Sv.vz = 0;
            Sv.pad = 950;
            if (Hit_area_ck(&pEm->pSin_parts_ptr[8].Workm, &Sv, pPl) && !pPl->Damage_cnt && !pEm->UnkDA) {
                Snd_se_enem(1, pEm);
                pPl->Routine_0 = 2;
                pPl->Routine_1 = Cdir_ck(pEm, pPl) + 2;
                pPl->Routine_2 = 0;
                pEm->UnkDA = 1;
                if (Pl_life_down(50, 0) & 2)
                    EM_SET_RTN(&Pl_work, 3, 0, 0, 0);
                pEm->UnkE2 = 5;
            }
        }
        Add_speedXZ(pEm, 0);
        if (pEm->UnkE2) {
            Sv.vx = Rnd() & 0x3F;
            Sv.vy = 150;
            Sv.vz = 0;
            if (pEm->UnkE2 == 5)
                Esp_call(0x700, (USHORT)pPl->Cdir_y | 0x14000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, NULL);
            Esp_call(0, (USHORT)pPl->Cdir_y | 0x18000000, &pPl->pSin_parts_ptr[8].Workm, &Sv, NULL);
            n = pEm->UnkE2;
            pPl->Spd_x = Cdir_ck_l(pEm, pPl) ? n * 100 : -(n * 100);
            pPl->Spd_z = 0;
            Add_speedXZ(pPl, 0);
            pEm->UnkE2--;
        }
        break;
    case 4:
        Dist = pEm->L_pl;
        if (Dist > 8000 || (!pEm->Timer0 && ((EM30_W(pEm)->Die_flg == 1 && Dist < 2000) || (EM30_W(pEm)->Die_flg == 9 && (LONG)pEm->L_pl < 2000)))) {
            if (pEm->Spd_x < 0)
                pEm->Spd_x = -pEm->Spd_x;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (!pEm->Timer0) {
            Flg = Flg_ck(&G.Unk7888, 4) != 0;
            if ((LONG)pEm->L_pl > 3450 && (LONG)pEm->L_pl < 5000 && !Flg && !pPl->Damage_cnt) {
                pEm->Cdir_y = Direction_ck(pEm->Pos_x, pEm->Pos_z, pPl->Pos_x, pPl->Pos_z);
                pEm->Routine_2 = 0;
            } else if (Flg_ck(&G.Unk7888, 4)) {
                EM_SET_RTN(pEm, 1, 8, 0, 0);
                break;
            } else {
                if ((LONG)pEm->L_pl < 3450) {
                    EM30_W(pEm)->Near_cnt++;
                } else {
                    EM30_W(pEm)->Near_cnt = 0;
                    EM30_W(pEm)->Hole = 0;
                }
                if (EM30_W(pEm)->Hole == 1 || EM30_W(pEm)->Hole == 9) {
                    if (pEm->Spd_x < 0)
                        pEm->Spd_x = -pEm->Spd_x;
                } else {
                    if (pEm->Spd_x < 0) {
                        if (pEm->Spd_x > -100)
                            pEm->Spd_x = -300;
                    } else if (pEm->Spd_x < 100) {
                        pEm->Spd_x = 300;
                    }
                    Dist = pEm->L_pl;
                    if (Dist < 3450)
                        Zone = 1;
                    else
                        Zone = (Dist > 5000) * 2;
                }
                if (!EM30_W(pEm)->Hole) {
                    Dist = pEm->L_pl;
                    if ((ULONG)(Dist - 3450) > 1550 && Zone == EM30_W(pEm)->Zone) {
                        if (Dist > 5000) {
                            pEm->Spd_x += (Rnd() & 0x1F) - 15;
                            if ((LONG)pEm->L_pl >= EM30_W(pEm)->Old_dist)
                                pEm->Spd_x = -pEm->Spd_x;
                        }
                        if ((LONG)pEm->L_pl < 3450) {
                            pEm->Spd_x += (Rnd() & 0x1F) - 15;
                            if (EM30_W(pEm)->Old_dist >= (LONG)pEm->L_pl)
                                pEm->Spd_x = -pEm->Spd_x;
                        }
                    }
                    EM30_W(pEm)->Zone = Zone;
                    EM30_W(pEm)->Old_dist = pEm->L_pl;
                }
                if (Rnd() & 3)
                    pEm->Timer0 = (Rnd() + 30) & 0x1F;
                else
                    pEm->Timer0 = (Rnd() + 10) & 0xF;
                pEm->UnkE3 = 30;
                EM30_W(pEm)->u10.Dirt = Rnd() & 3;
            }
        } else {
            pEm->Timer0--;
            if (pEm->Timer0 >= 36 && pEm->Timer0 < 40) {
                if (EM30_W(pEm)->u10.Dirt)
                    Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 0);
                Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 0);
            if (pEm->Timer0 == 39)
                G.Status_flg |= 4;
            if (pEm->Timer0 == 36)
                G.Status_flg &= ~4;
            }
            if (pEm->Timer0 >= 16 && pEm->Timer0 < 20) {
                if (EM30_W(pEm)->u10.Dirt) {
                    Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 0);
                    Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 0);
                }
            if (pEm->Timer0 == 19)
                G.Status_flg |= 4;
            if (pEm->Timer0 == 16)
                G.Status_flg &= ~4;
            }
            if (pEm->Timer0 < 10) {
                if (EM30_W(pEm)->u10.Dirt) {
                    Em30_dirt_spray(pEm, EM30_W(pEm)->Dirt_part, 0);
                    Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 0);
                }
            if (pEm->Timer0 == 9)
                G.Status_flg |= 4;
            if (pEm->Timer0 == 6)
                G.Status_flg &= ~4;
            }
        }
        Em30_body_chain(pEm);
        break;
    case 5:
        break;
    }
}

/* mv_br[8]: back at the hole: dive into it (mv 3, step 2) */
void Em30_hole_br(ENEMY_WORK *pEm)
{
    UCHAR Se[2] = {9, 6};
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Tg;

    if (!EM30_W(pEm)->Se_timer) {
        Snd_se_enem(Se[EM30_W(pEm)->Under], pEm);
        EM30_W(pEm)->Se_timer = 25;
    } else {
        EM30_W(pEm)->Se_timer--;
    }
    if (pEm->Routine_2 == 1) {
        if (EM30_W(pEm)->Hole == 1) {
            Tg.vx = -0x4BD4;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x4BD4) * (pPl->Pos_x + 0x4BD4) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        } else {
            Tg.vx = -0x86B;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x86B) * (pPl->Pos_x + 0x86B) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        }
        if (SquareRoot0((Tg.vx - pEm->Pos_x) * (Tg.vx - pEm->Pos_x) + (Tg.vz - pEm->Pos_z) * (Tg.vz - pEm->Pos_z)) < 200) {
            EM_SET_RTN(pEm, 1, 3, 2, 0);
            EM30_W(pEm)->Dive_cnt = 0;
            pEm->Timer0 = 0;
            EM_SET_MOVE(pEm, 0, 0, 0);
            pEm->Be_flg &= ~0x2000;
            G.Status_flg &= ~4;
        }
    }
}

/* mv[8]: crawl back to the hole */
void Em30_hole(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    VECTOR Tg;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Timer0 = 0;
        pEm->Be_flg |= 0x2000;
        pEm->Routine_2++;
    case 1:
        if (EM30_W(pEm)->Hole == 1) {
            Tg.vx = -0x4BD4;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x4BD4) * (pPl->Pos_x + 0x4BD4) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        } else {
            Tg.vx = -0x86B;
            Tg.vz = -0x2C88;
            if (SquareRoot0((pPl->Pos_x + 0x86B) * (pPl->Pos_x + 0x86B) + (pPl->Pos_z + 0x2C88) * (pPl->Pos_z + 0x2C88)) < 0xA28)
                Tg.vz = -0x1F40;
        }
        Goto00(pEm, Tg.vx, Tg.vz, 0x180);
        if (SquareRoot0((Tg.vx - pEm->Pos_x) * (Tg.vx - pEm->Pos_x) + (Tg.vz - pEm->Pos_z) * (Tg.vz - pEm->Pos_z)) < 900) {
            pEm->Spd_x = 50;
            pEm->Spd_z = 0;
        } else {
            pEm->Spd_x = 150;
            pEm->Spd_z = 0;
        }
        Joint_move(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        if (++pEm->Timer0 == 3)
            G.Status_flg |= 4;
        if (pEm->Timer0 == 6)
            G.Status_flg &= ~4;
        if (pEm->Timer0 == 20)
            pEm->Timer0 = 0;
    }
}

#include "enemy/common/em30/Em30_body_chain.inc.c"

#include "enemy/common/em30/Em30_part_hang.inc.c"

#include "enemy/common/em30/Em30_part_hang_sink.inc.c"

#include "enemy/common/em30/Em30_work_set.inc.c"

/* morph the body (keys 0..2 of parts 1..3) in a cycle, faster with the speed.
   Returns the last sabun call's value (only one path returns): that explicit v0 use is what
   makes reload put the /50 mfhi in a3. */
LONG Em30_morph_body(ENEMY_WORK *pEm)
{
    PARTS_W *pP = &pEm->pSin_parts_ptr[1];
    LONG Step;

    if (EM_RTN01(pEm) != 0x301 || pEm->Routine_2 == 2) {
        Step = EM30_W(pEm)->Morph_spd * pEm->Spd_x / 50;
        if (Step < 0)
            Step = 0;
        switch (EM30_W(pEm)->Morph_phase) {
        case 0:
            EM30_SABUN_P(&pP[0])->Key[0].Rate += Step;
            EM30_SABUN_P(&pP[1])->Key[0].Rate += Step;
            EM30_SABUN_P(&pP[2])->Key[0].Rate += Step;
            if (EM30_SABUN_P(&pP[0])->Key[0].Rate > 0xC00) {
                EM30_SABUN_P(&pP[0])->Key[0].Rate = 0xC00;
                EM30_SABUN_P(&pP[1])->Key[0].Rate = 0xC00;
                EM30_SABUN_P(&pP[2])->Key[0].Rate = 0xC00;
                EM30_W(pEm)->Morph_phase = 1;
            }
            break;
        case 1:
            EM30_SABUN_P(&pP[0])->Key[0].Rate -= Step;
            EM30_SABUN_P(&pP[1])->Key[0].Rate -= Step;
            EM30_SABUN_P(&pP[2])->Key[0].Rate -= Step;
            if (EM30_SABUN_P(&pP[0])->Key[0].Rate < 0) {
                EM30_SABUN_P(&pP[0])->Key[0].Rate = 0;
                EM30_SABUN_P(&pP[1])->Key[0].Rate = 0;
                EM30_SABUN_P(&pP[2])->Key[0].Rate = 0;
            }
            EM30_SABUN_P(&pP[0])->Key[1].Rate += Step;
            EM30_SABUN_P(&pP[1])->Key[1].Rate += Step;
            EM30_SABUN_P(&pP[2])->Key[1].Rate += Step;
            if (EM30_SABUN_P(&pP[0])->Key[1].Rate > 0xC00) {
                EM30_SABUN_P(&pP[0])->Key[1].Rate = 0xC00;
                EM30_SABUN_P(&pP[1])->Key[1].Rate = 0xC00;
                EM30_SABUN_P(&pP[2])->Key[1].Rate = 0xC00;
                EM30_W(pEm)->Morph_phase = 2;
            }
            break;
        case 2:
            EM30_SABUN_P(&pP[0])->Key[1].Rate -= Step;
            EM30_SABUN_P(&pP[1])->Key[1].Rate -= Step;
            EM30_SABUN_P(&pP[2])->Key[1].Rate -= Step;
            if (EM30_SABUN_P(&pP[0])->Key[1].Rate < 0) {
                EM30_SABUN_P(&pP[0])->Key[1].Rate = 0;
                EM30_SABUN_P(&pP[1])->Key[1].Rate = 0;
                EM30_SABUN_P(&pP[2])->Key[1].Rate = 0;
            }
            EM30_SABUN_P(&pP[0])->Key[2].Rate += Step;
            EM30_SABUN_P(&pP[1])->Key[2].Rate += Step;
            EM30_SABUN_P(&pP[2])->Key[2].Rate += Step;
            if (EM30_SABUN_P(&pP[0])->Key[2].Rate > 0xC00) {
                EM30_SABUN_P(&pP[0])->Key[2].Rate = 0xC00;
                EM30_SABUN_P(&pP[1])->Key[2].Rate = 0xC00;
                EM30_SABUN_P(&pP[2])->Key[2].Rate = 0xC00;
                EM30_W(pEm)->Morph_phase = 3;
            }
            break;
        case 3:
            EM30_SABUN_P(&pP[0])->Key[2].Rate -= Step;
            EM30_SABUN_P(&pP[1])->Key[2].Rate -= Step;
            EM30_SABUN_P(&pP[2])->Key[2].Rate -= Step;
            if (EM30_SABUN_P(&pP[0])->Key[2].Rate < 0) {
                EM30_SABUN_P(&pP[0])->Key[2].Rate = 0;
                EM30_SABUN_P(&pP[1])->Key[2].Rate = 0;
                EM30_SABUN_P(&pP[2])->Key[2].Rate = 0;
                EM30_W(pEm)->Morph_phase = 0;
            }
            break;
        }
        Sa_morph_blend(pEm, &pP[0]);
        Sa_morph_blend(pEm, &pP[1]);
        return func_8004AC54_l(pEm, &pP[2]);
    }
}

#include "enemy/common/em30/Em30_morph_step.inc.c"

/* pick the hole to go back to (Hole 1 or 9) by x, after flag 4 or ten close dives */
void Em30_hole_set(ENEMY_WORK *pEm)
{
    LONG Flg = 0;

    if ((ULONG)(pEm->Routine_1 - 7) < 2 || (EM_RTN(pEm) & 0xFFFF00) == 0x20300) {
        if (Flg_ck(&G.Unk7888, 4))
            Flg = 1;
    }
    if (Flg || EM30_W(pEm)->Near_cnt > 10) {
        if (EM30_W(pEm)->Hole != 1 && EM30_W(pEm)->Hole != 9) {
            if (pEm->Pos_x < -10000)
                EM30_W(pEm)->Hole = 1;
            else
                EM30_W(pEm)->Hole = 9;
        }
    }
}

#include "enemy/common/em30/Em30_Damage.inc.c"

/* dm[0..28]: ordinary hit; a segment passes the damage on to the head */
void Em30_dm_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOya;
    LONG Dm;

    if (!(pEm->Type & 1)) {
        if (++EM30_W(pEm)->Hit_cnt >= 16 && EM30_W(pEm)->Under != 1) {
            EM30_W(pEm)->Hit_cnt = 0;
            pEm->Damage_no = 0x20;
            Em30_Damage(pEm, pKan, pSeq);
            EM30_W(pEm)->Morph_dir = 0;
        } else {
            if (pEm->Life < 0) {
                pEm->Life = 0;
                EM30_W(pEm)->Old_life = 0;
            }
            EM_RTN(pEm) = pEm->Old_routine;
            Em30_Move(pEm, pKan, pSeq);
            Snd_se_enem(2, pEm);
        }
    } else {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (EM30_SUB(pEm)->Parts_no == 8) {
            if (EM30_W(pOya)->Morph_dir) {
                EM30_W(pOya)->Hit_cnt += 5;
                if (!pOya->Damage_cnt) {
                    Dm = EM30_W(pEm)->Old_life - pEm->Life;
                    pOya->Damage_cnt = pEm->Damage_cnt;
                    pOya->Life -= Dm;
                }
            } else if (!pOya->Damage_cnt) {
                Dm = EM30_W(pEm)->Old_life - pEm->Life;
                pOya->Damage_cnt = pEm->Damage_cnt;
                pOya->Life -= Dm;
            }
        } else if (!pOya->Damage_cnt) {
            Dm = EM30_W(pEm)->Old_life - pEm->Life;
            pOya->Damage_cnt = pEm->Damage_cnt;
            pOya->Life -= Dm * 2 / 3;
        }
        if (pOya->Life < 0) {
            if (pOya->Routine_0 == 1 && EM30_W(pOya)->Under != 1) {
                pOya->Damage_no = pEm->Damage_no;
                EM_SET_RTN(pOya, 3, 0, 0, 0);
            } else {
                pOya->Life = 0;
                EM30_W(pOya)->Old_life = 0;
            }
        }
        EM30_W(pOya)->Hit_cnt++;
        Snd_se_enem(2, pOya);
        EM_RTN(pEm) = pEm->Old_routine;
    }
    if (pEm->Damage_no == 5) {
        pEm->Unk16A = 1;
        EM30_W(pEm)->Old_life = pEm->Life;
    } else {
        Em30_blood_set(pEm);
    }
}

/* dm[31]: flinch back after 16 hits (a segment hands the routine to the head) */
void Em30_dm_flinch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[4] = {0x12, 0x13, 0x14, 0x15};
    UCHAR Se[2] = {7, 8};
    ENEMY_WORK *pOya;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        pOya->Damage_no = pEm->Damage_no;
        if (pOya->Routine_0 == 1)
            pOya->Old_routine = EM_RTN(pOya);
        EM_RTN(pOya) = EM_RTN(pEm);
        EM_RTN(pEm) = pEm->Old_routine;
        Em30_blood_set(pEm);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        pEm->Damage_cnt |= 0x80;
        EM_SET_MOVE_V(pEm, Mot[EM30_W(pEm)->Head_up * 2 + EM30_W(pEm)->Morph_dir], 0, 0x1F);
        Em30_blood_set(pEm);
        EM30_W(pEm)->Spd = 40;
        EM30_W(pEm)->Spd_lim = 80;
        EM30_W(pEm)->Spd_add = 5;
        pEm->Spd_z = 0;
        Snd_se_enem(3, pEm);
        Em30_surface(pEm);
        pEm->Routine_2++;
    case 1:
        if (pEm->Move_cnt == 3 || pEm->Move_cnt == 9)
            Em30_blood_set(pEm);
        Em30_surface(pEm);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x20);
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM_SET_RTN(pEm, 1, 3, 0, 0);
            pEm->Damage_cnt &= 0x7F;
        }
        pEm->Spd_x = EM30_W(pEm)->Spd;
        Add_speedXZ(pEm, 0);
        EM30_W(pEm)->Spd += EM30_W(pEm)->Spd_add;
        if (EM30_W(pEm)->Spd_add > 0) {
            if (EM30_W(pEm)->Spd > EM30_W(pEm)->Spd_lim) {
                EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
                EM30_W(pEm)->Spd_add = -8;
                EM30_W(pEm)->Spd_lim = 40;
            }
        } else if (EM30_W(pEm)->Spd < EM30_W(pEm)->Spd_lim) {
            EM30_W(pEm)->Spd = EM30_W(pEm)->Spd_lim;
            EM30_W(pEm)->Spd_add = 5;
            EM30_W(pEm)->Spd_lim = 80;
        }
        Em30_body_chain(pEm);
    }
}

/* dm[29][0] (Damage_no 30 with Damage_flg bit 2): the first such hit takes Life / 3, the second kills */
void Em30_dm_heavy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pOya;

    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        if (!Flg_ck(&G.Unk7888, 0) && EM30_W(pOya)->Under != 1) {
            pOya->Damage_no = pEm->Damage_no;
            pOya->Damage_flg = pEm->Damage_flg;
            pOya->Damage_cnt = 10;
            pOya->Old_routine = EM_RTN(pOya);
            EM_RTN(pOya) = EM_RTN(pEm);
            Em30_body_number(pEm);
            Flg_on(&G.Unk7888, 0);
            G.Unk75EA = 1;
        }
        pEm->Damage_cnt = 10;
        EM_RTN(pEm) = pEm->Old_routine;
    } else {
        if (EM30_W(pEm)->Under != 1) {
            if (EM30_W(pEm)->Heavy_hit == 1) {
                Em30_dm_fire(pEm, pKan, pSeq);
                return;
            }
            EM30_W(pEm)->Heavy_hit = 1;
            pEm->Damage_cnt = 10;
            pEm->Life /= 3;
            Em30_body_number(pEm);
            if (G.Unk75EA)
                G.Unk75EA = 0;
            else
                Flg_on(&G.Unk7888, 0);
        }
        EM_RTN(pEm) = pEm->Old_routine;
        Em30_Move(pEm, pKan, pSeq);
    }
}

#include "enemy/common/em30/Em30_dm_fire.inc.c"

/* blood from the hit part, more for a bigger hit */
void Em30_blood_set(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pOya;
    SVECTOR Sv;
    LONG Dm;
    LONG n;
    ULONG Dir;

    Dm = EM30_W(pEm)->Old_life - pEm->Life;
    EM30_W(pEm)->Old_life = pEm->Life;
    n = 2;
    if (Dm > 20)
        n = 4;
    if (Dm > 50)
        n += 2;
    if (Dm > 100)
        n += 2;
    if (pEm->Type & 1) {
        pOya = G.pEnemy[EM30_SUB(pEm)->Oya_no];
        while (n--) {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = (Rnd() & 0xFF) - 0x7F;
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd() & 3) * 0x300 + 0x1200;
            Esp_call(0x7000700, (Dir << 16) | (USHORT)pOya->Cdir_y, &pOya->pSin_parts_ptr[EM30_SUB(pEm)->Parts_no].Workm, &Sv, NULL);
        }
    } else {
        while (n--) {
            Sv.vx = (Rnd() & 0xFF) - 0x7F;
            Sv.vy = (Rnd() & 0xFF) - 0x7F;
            Sv.vz = (Rnd() & 0xFF) - 0x7F;
            Sv.vx <<= 2;
            Sv.vz <<= 2;
            Sv.vy <<= 2;
            Dir = (Rnd() & 3) * 0x300 + 0x1200;
            Esp_call(0x7000700, (Dir << 16) | (USHORT)pEm->Cdir_y, &pEm->pSin_parts_ptr[8].Workm, &Sv, NULL);
        }
    }
}

#include "enemy/common/em30/Em30_dirt_hit.inc.c"

#include "enemy/common/em30/Em30_dirt_line.inc.c"

#include "enemy/common/em30/Em30_body_number.inc.c"

#include "enemy/common/em30/Em30_dirt_steps.inc.c"

#include "enemy/common/em30/Em30_dirt_set.inc.c"

/* Routine_0 3: die routine by the hit kind (none while underground) */
void Em30_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (EM30_W(pEm)->Under == 1) {
        pEm->Life = 1;
        EM_RTN(pEm) = pEm->Old_routine;
        Em30_Move(pEm, pKan, pSeq);
    } else {
        Em30_die[pEm->Damage_no - 1](pEm, pKan, pSeq);
    }
}

/* die[0..29]: shot to pieces (by the weapon in Blow[]) and sinking */
void Em30_die_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    signed char Blow[32] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0};
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    PARTS_W *pS;
    KAGE_WORK *pK;
    LONG Dir;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2++;
        pEm->Damage_cnt |= 0x80;
        Snd_se_enem(5, pEm);
        EM_SET_MOVE(pEm, 0x17, 0, 0x1F);
        Em30_surface(pEm);
        pP = pEm->pSin_parts_ptr;
        for (i = 17; i != 0; i--) {
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP++;
        }
        pEm->Life = -1;
        pEm->Neck_flg |= 1;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[8];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[2];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[3];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[4];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[5];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[6];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        EM30_W(pEm)->Pl_type = pPl->Type;
        if (Blow[EM30_W(pEm)->Pl_type]) {
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[14];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[16];
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP = &pEm->pSin_parts_ptr[17];
            PW_RGB(pP)->Rgb0 = 0x20;
            PW_RGB(pP)->Rgb1 = 0x101030;
            pP->UnkA2 |= 0x405;
        }
        G.Status_flg &= ~4;
        G.Status_flg &= ~2;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 2500;
        Sv.vy = 0;
        Sv.vz = -700;
        Esp_call(0x7001500, 0x2E000938, &pP->Workm, &Sv, NULL);
        Sv.vz = 700;
        Esp_call(0x7001500, 0x32000550, &pP->Workm, &Sv, NULL);
        Sv.vy = 700;
        Sv.vz = 0;
        Esp_call(0x7001500, 0x20000800, &pP->Workm, &Sv, NULL);
        Sv.vy = -700;
        Esp_call(0x7001500, 0x18000800, &pP->Workm, &Sv, NULL);
        Sv.vy = 0;
        Esp_call(0x7001500, 0x22000800, &pP->Workm, &Sv, NULL);
        Sv.vy = 0;
        Sv.vz = -600;
        Esp_call(0x7000000, 0x34000938, &pP->Workm, &Sv, NULL);
        Sv.vz = 550;
        Esp_call(0x7000000, 0x25000550, &pP->Workm, &Sv, NULL);
        Sv.vy = 750;
        Sv.vz = 0;
        Esp_call(0x7000000, 0x18000800, &pP->Workm, &Sv, NULL);
        Sv.vy = -700;
        Esp_call(0x7000000, 0x42000800, &pP->Workm, &Sv, NULL);
        pEm->Timer0 = 0;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        Pad_vib_data_set(Em30_vib_tbl, 9);
    case 1:
        Dir = pEm->Cdir_y + (((UCHAR)(pEm->Move_cnt % 7) - 3) << 8);
        if (pEm->Move_cnt < 50 && Blow[EM30_W(pEm)->Pl_type]) {
            if (pEm->Move_cnt < 32) {
                pP = &pEm->pSin_parts_ptr[8];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[10];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[12];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[14];
                pP->Poly_rgb -= 0x40202;
                pP = &pEm->pSin_parts_ptr[16];
                pP->Poly_rgb -= 0x40202;
            }
            if (pEm->Move_cnt == 33) {
                pP = &pEm->pSin_parts_ptr[8];
                Sv.vx = 0;
                Sv.vy = 0;
                Sv.vz = 0;
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[10];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[12];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[14];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
                pP = &pEm->pSin_parts_ptr[16];
                Esp_call(0x7001500, (USHORT)Dir | 0x2E000000, &pP->Workm, &Sv, NULL);
                pP->UnkA2 &= ~1;
            }
            if (!(pEm->Move_cnt & 3)) {
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 2500;
                Sv.vy = 0;
                Sv.vz = -700;
                Esp_call(0x7001500, (USHORT)(Dir + 0x938) | 0x2E000000, &pP->Workm, &Sv, NULL);
                Sv.vz = 700;
                Esp_call(0x7001500, (USHORT)(Dir + 0x550) | 0x32000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 700;
                Sv.vz = 0;
                Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x20000000, &pP->Workm, &Sv, NULL);
                Sv.vy = -700;
                Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x18000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 0;
                Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x22000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 0;
                Sv.vz = -600;
                Esp_call(0x7000000, (USHORT)(Dir + 0x938) | 0x14000000, &pP->Workm, &Sv, NULL);
                Sv.vz = 550;
                Esp_call(0x7000000, (USHORT)(Dir + 0x550) | 0x25000000, &pP->Workm, &Sv, NULL);
                Sv.vy = 750;
                Sv.vz = 0;
                Esp_call(0x7000000, (USHORT)(Dir + 0x800) | 0x18000000, &pP->Workm, &Sv, NULL);
                Sv.vy = -700;
                Esp_call(0x7000000, (USHORT)(Dir + 0x800) | 0x32000000, &pP->Workm, &Sv, NULL);
            }
        } else if (!(pEm->Move_cnt & 7)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 2500;
            Sv.vy = 0;
            Sv.vz = -700;
            Esp_call(0x7001500, (USHORT)(Dir + 0x938) | 0x24000000, &pP->Workm, &Sv, NULL);
            Sv.vz = 700;
            Esp_call(0x7001500, (USHORT)(Dir + 0x550) | 0x22000000, &pP->Workm, &Sv, NULL);
            Sv.vy = 700;
            Sv.vz = 0;
            Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x18000000, &pP->Workm, &Sv, NULL);
            Sv.vy = -700;
            Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x12000000, &pP->Workm, &Sv, NULL);
            Sv.vy = 0;
            Esp_call(0x7001500, (USHORT)(Dir + 0x800) | 0x1E000000, &pP->Workm, &Sv, NULL);
        }
        Sv.vx = 500;
        if (pEm->Move_cnt == 30) {
            pP = pEm->pSin_parts_ptr;
            Sv.vy = 300;
            Sv.vz = -700;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x20000000, &pP->Workm, &Sv, NULL);
        }
        if (pEm->Move_cnt == 45) {
            pS = pEm->pSin_parts_ptr;
            Sv.vy = 200;
            Sv.vz = -700;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x1E000000, &pS[3].Workm, &Sv, NULL);
        }
        if (pEm->Move_cnt == 60) {
            pS = pEm->pSin_parts_ptr;
            Sv.vy = 0;
            Sv.vz = -100;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x20000000, &pS[1].Workm, &Sv, NULL);
        }
        if (pEm->Move_cnt == 75) {
            pS = pEm->pSin_parts_ptr;
            Sv.vy = 0;
            Sv.vz = -400;
            Esp_call(0x1403, (USHORT)(Dir + 0x938) | 0x18000000, &pS[4].Workm, &Sv, NULL);
        }
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x80);
        Em30_body_chain(pEm);
        break;
    case 2:
        Em_dead_flg_on_cand(pEm);
        for (i = 0; i < 9; i++) {
            if (i == 7)
                i = 8;
            if (!i) {
                pK = pEm->Kage.pKw;
            } else {
                pP = &pEm->pSin_parts_ptr[i];
                pK = G.pEnemy[pP->Unk82]->Kage.pKw;
            }
            *(ULONG *)&pK->Prim[0].r0 = (*(ULONG *)&pK->Prim[0].r0 & 0xFF000000) | 0xBF3030;
            *(ULONG *)&pK->Prim[1].r0 = (*(ULONG *)&pK->Prim[1].r0 & 0xFF000000) | 0xBF3030;
        }
        pEm->Timer0 = 20;
        pEm->Routine_2++;
        break;
    case 3:
        if (--pEm->Timer0) {
            for (i = 0; i < 9; i++) {
                if (i == 7)
                    i = 8;
                if (!i) {
                    pK = pEm->Kage.pKw;
                } else {
                    pP = &pEm->pSin_parts_ptr[i];
                    pK = G.pEnemy[pP->Unk82]->Kage.pKw;
                }
                    pK->Half_x++;
                pK->Half_z++;
            }
        } else {
            pEm->Routine_2++;
        }
        break;
    case 4:
        break;
    }
    if (pEm->Pos_y < 1200) {
        if (pEm->Pos_y > 1100) {
            for (i = 0; i < 18; i++) {
                pP = &pEm->pSin_parts_ptr[i];
                pP->Poly_rgb -= 0x201;
            }
            if (pEm->Pos_y & 2) {
                for (i = 0; i < 18; i++) {
                    pP = &pEm->pSin_parts_ptr[i];
                    pP->Poly_rgb -= 0x10000;
                }
            }
        }
    }
    if (pEm->Pos_y > 1100 && pEm->Col.Id) {
        pEm->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[8].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[1].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[2].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[3].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[4].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Col.Id = 0;
        G.pEnemy[pEm->pSin_parts_ptr[6].Unk82]->Col.Id = 0;
    }
    if (pEm->Pos_y < 1400) {
        pEm->Pos_y = pEm->Pos_y < 1000 ? pEm->Pos_y + 6 : pEm->Pos_y + 2;
        pP = &pEm->pSin_parts_ptr[8];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[1];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[2];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[3];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[4];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[5];
        pP->Workm.t[1] += 2;
        pP = &pEm->pSin_parts_ptr[6];
        pP->Workm.t[1] += 2;
        {
            ENEMY_WORK *pB = G.pEnemy[(Rnd() & 7) + 2];

            if (++pEm->UnkE2 == 8) {
                pEm->UnkE2 = 0;
                if ((pB->Be_flg & 1) && pB->Id == 0x30)
                    Em30_dirt_hit(pB);
            }
        }
    } else if (!pEm->UnkE3) {
        pEm->UnkE3 = 1;
        Esp_kill(0x1403, NULL);
        Dir = pEm->Cdir_y + (((UCHAR)(pEm->Move_cnt % 7) - 3) << 8);
        Sv.vx = 500;
        pP = pEm->pSin_parts_ptr;
        Sv.vy = 300;
        Sv.vz = -700;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x20000000, &pP->Workm, &Sv, NULL);
        pS = pEm->pSin_parts_ptr;
        Sv.vy = 200;
        Sv.vz = -700;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x1E000000, &pS[3].Workm, &Sv, NULL);
        pS = pEm->pSin_parts_ptr;
        Sv.vy = 0;
        Sv.vz = -100;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x20000000, &pS[1].Workm, &Sv, NULL);
        pS = pEm->pSin_parts_ptr;
        Sv.vy = 0;
        Sv.vz = -400;
        Esp_call(0x1C03, (USHORT)(Dir + 0x938) | 0x18000000, &pS[4].Workm, &Sv, NULL);
    }
    if (EM30_W(pEm)->Morph_spd)
        EM30_W(pEm)->Morph_spd -= 4;
}

/* die[29]: killed by Damage_no 30: moved to a fixed spot, thrashes about and sinks */
void Em30_die_heavy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR Pos;
    PARTS_W *pP;
    LONG i;

    switch (pEm->Routine_2) {
    case 0:
        Flg_on(&G.Unk7888, 0);
        Flg_on(&G.Unk7888, 1);
        Em30_surface(pEm);
        pEm->Life = -1;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[8];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[2];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[3];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[4];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[5];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[6];
        G.pEnemy[pP->Unk82]->Life = -1;
        G.pEnemy[pP->Unk82]->Damage_cnt |= 0x80;
        pEm->Routine_2++;
        Pad_vib_data_set(Em30_vib_tbl, 8);
        pEm->Timer0 = 0;
        pEm->UnkE3 = 0;
        break;
    case 1:
        pEm->UnkDA = 0;
        Snd_se_enem(5, pEm);
        EM_SET_MOVE(pEm, 0x18, 0, 0x1F);
        pP = pEm->pSin_parts_ptr;
        for (i = 17; i != 0; i--) {
            pP->UnkA2 |= 0x400;
            pP->Be_flg &= ~2;
            pP++;
        }
        pEm->Neck_flg |= 1;
        G.Status_flg &= ~4;
        G.Status_flg &= ~2;
        pEm->Pos_x = -0x1D00;
        pEm->Pos_z = -0x3E6E;
        pEm->Cdir_y = 0x98B;
        pEm->UnkE2 = 0;
        pEm->Routine_2++;
        Oba_hit_clr(&pEm->Col);
    case 2:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x80);
        if (!EM30_W(pEm)->u13.Blood_cnt)
            Em30_body_number(pEm);
        if (pEm->Move_cnt == 37) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            Em30_dirt_set(pEm, 0, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 100);
            Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 100);
            Em30_dirt_puff(pEm, EM30_W(pEm)->Dirt_part, 100);
        }
        if (pEm->Move_cnt == 49) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            Em30_dirt_puff(pEm, 0, 100);
            Em30_dirt_puff(pEm, 2, 100);
            Em30_dirt_puff(pEm, 4, 100);
        }
        if (pEm->Move_cnt == 56) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            Em30_dirt_set(pEm, 3, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            Em30_dirt_puff(pEm, 1, 100);
            Em30_dirt_puff(pEm, 2, 100);
            Em30_dirt_puff(pEm, 5, 100);
        }
        if (pEm->Move_cnt == 74) {
            G.Status_flg |= 2;
            pEm->UnkE2 = 10;
            Em30_dirt_set(pEm, 1, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x10210, &Pos, 0, 0);
            Em30_dirt_puff(pEm, 0, 100);
            Em30_dirt_puff(pEm, 1, 100);
            Em30_dirt_puff(pEm, 2, 100);
            Em30_dirt_puff(pEm, 3, 100);
            Em30_dirt_puff(pEm, 4, 100);
        }
        if (pEm->Move_cnt == 97) {
            G.Status_flg |= 4;
            pEm->UnkE2 = 3;
            Em30_dirt_set(pEm, 4, 0x12);
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x1020E, &Pos, 0, 0);
            Em30_dirt_puff(pEm, 0, 100);
            Em30_dirt_puff(pEm, 2, 100);
            Em30_dirt_puff(pEm, 4, 100);
        }
        if (pEm->Move_cnt == 115) {
            G.Status_flg |= 2;
            pEm->UnkE2 = 10;
            Pos.vx = pEm->Pos_x;
            Pos.vy = pEm->Pos_y;
            Pos.vz = pEm->Pos_z;
            Snd_se_stad(0x10210, &Pos, 0, 0);
            Em30_dirt_puff(pEm, 0, 100);
            Em30_dirt_puff(pEm, 1, 100);
            Em30_dirt_puff(pEm, 2, 100);
            Em30_dirt_puff(pEm, 3, 100);
            Em30_dirt_puff(pEm, 4, 100);
        }
        if (pEm->UnkE2 && !--pEm->UnkE2) {
            G.Status_flg &= ~4;
            G.Status_flg &= ~2;
        }
        if (pEm->Move_cnt & 1) {
            pP = pEm->pSin_parts_ptr;
            i = 0;
            do {
                pP->Poly_rgb -= 0x10101;
                pP++;
            } while (++i < pEm->Parts_num);
        }
        break;
    case 3:
        EM_SET_MOVE(pEm, 0x19, 0, 7);
        pEm->Routine_2++;
        break;
    case 4:
        pEm->Routine_2 += Joint_move(pEm, pKan, pSeq, 0x200);
        break;
    case 5:
        if (!(Rnd() & 0xF))
            pEm->Routine_2 -= 2;
        break;
    }
    if (pEm->Routine_2 >= 4) {
        if (pEm->Pos_y < 600)
            Em_dead_flg_on_cand(pEm);
        if (pEm->Pos_y < 900 && pEm->Col.Id) {
            pEm->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[8].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[1].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[2].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[3].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[4].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[5].Unk82]->Col.Id = 0;
            G.pEnemy[pEm->pSin_parts_ptr[6].Unk82]->Col.Id = 0;
        }
        if (pEm->Pos_y < 1400) {
            pEm->Pos_y += 2;
            if (++pEm->Timer0 == 8) {
                ENEMY_WORK *pB;

                pEm->Timer0 = 0;
                pB = G.pEnemy[(Rnd() & 7) + 2];
                if ((pB->Be_flg & 1) && pB->Id == 0x30)
                    Em30_dirt_hit(pB);
            }
        } else if (!pEm->UnkE3) {
            pEm->UnkE3 = 1;
            Esp_kill(0x1203, NULL);
            Em30_dirt_set(pEm, 0, 0x1B);
            Em30_dirt_set(pEm, 1, 0x1B);
            Em30_dirt_set(pEm, 3, 0x1B);
            Em30_dirt_set(pEm, 4, 0x1B);
        }
    }
    if (EM30_W(pEm)->Morph_spd)
        EM30_W(pEm)->Morph_spd -= 4;
}

#include "enemy/common/em30/Em30_se_set.inc.c"

/* G.pPldie_Em hook: the player was killed by the Grave Digger */
void Em30_Pldie(ENEMY_WORK *pPl)
{
    pPl->Be_flg |= 0x10;
}
