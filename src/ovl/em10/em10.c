#include "common.h"
#include "ovl/em10.h"

/* routine tables (module .data); like RE2 they are unprototyped, the routines take (pEm, pKan, pSeq) */
extern void (*Em10_routine_0[])() __asm__("D_8010C8E8");
extern void (*Em10_Move_type[])() __asm__("D_8010C90C");
extern void (*Em10_mv_br_type00[])() __asm__("D_8010C94C");
extern void (*Em10_mv_type00[])() __asm__("D_8010C98C");
extern void (*Em10_mv_br_type01[])() __asm__("D_8010C9CC");
extern void (*Em10_mv_type01[])() __asm__("D_8010C9DC");
extern void (*Em10_dm[][3])() __asm__("D_8010C9EC");
extern void (*Em10_haidm[])() __asm__("D_8010CB60");
extern void (*Em10_die[][3])() __asm__("D_8010CBDC");
extern void (*Em10_sce[])() __asm__("D_8010CD50");
extern void (*Pldamage_Em10_tbl[])() __asm__("D_8010CD64");
extern void (*Pldie_Em10_tbl[])() __asm__("D_8010CD74");
extern USHORT Em10_walk_rnd[32] __asm__("D_8010CD7C");  /* walk time, 0x8000: turn slower */
extern USHORT Em10_mv0d_rnd[16] __asm__("D_8010CDD0");  /* time, 0x8000: turn the other way */
extern EM10_ESP Em10_mv11_esp[] __asm__("D_8010CDF0");
extern UCHAR Em10_dm00_usonum[] __asm__("D_8010CF60");  /* Unk0B decrement per damage type */

extern USHORT Em10_life[16] __asm__("D_8010C8A8");
extern USHORT Em10_life_easy[16] __asm__("D_8010C8C8");

void func_80102340(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801030A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010879C(ENEMY_WORK *pEm);
void func_801089A8(ENEMY_WORK *pEm);
void func_80108B00(ENEMY_WORK *pEm);
void func_80108CBC(ENEMY_WORK *pEm);
void func_80109E54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010C57C(ENEMY_WORK *pEm);
void func_80107C10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801082B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108E6C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B278(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010AA5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010BB08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103510(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103B48(ENEMY_WORK *pEm);
void func_80103EF0(ENEMY_WORK *pEm);
void func_8010C698(ENEMY_WORK *pEm);
void func_8010C3F0(ENEMY_WORK *pEm, LONG a1, LONG a2);
void func_8010ABAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B6D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

/* RE2 EM_MAIN */
void func_80100254(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl;
    VECTOR *pTv;
    USHORT *pFlg;
    SVECTOR Sv;                        /* unused (frame size) */
    tagGLOBAL *pG = &G;

    pTv = (VECTOR *)&pG->Pl_pos_x;
    pPl = (ENEMY_WORK *)&pG->Pl_be_flg;
    if ((LONG)pG->Status_flg < 0) {
        if ((pEm->Unk158 & 0xC0000000) == 0x40000000)
            pTv = (VECTOR *)&pG->pEnemy[1]->Pos_x;
        if (pEm->Type & 0x200)
            pTv = (VECTOR *)&pG->pEnemy[1]->Pos_x;
    }
    pFlg = &EM10_W(pEm)->Flg;
    Root_ck(pEm, pTv, EM10_W(pEm)->Root_no, EM10_W(pEm)->Flg & 0x400);
    if (EM10_W(pEm)->Unk21)
        EM10_W(pEm)->Unk21--;
    if (EM10_W(pEm)->Unk26)
        EM10_W(pEm)->Unk26--;
    if (EM10_W(pEm)->u2C.w.Unk2F)
        EM10_W(pEm)->u2C.w.Unk2F--;
    if (pEm->Damage_cnt & 0x7F)
        pEm->Damage_cnt--;
    if (EM10_W(pEm)->Unk18) {
        if (--EM10_W(pEm)->Unk18 == 0)
            G.Unk75EA &= ~1;
    }
    Em10_routine_0[pEm->Routine_0](pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    if (pEm->Water) {
        if (pEm->pSin_parts_ptr->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 0xF) == 1 && pEm->Spd_x >= 11)
            func_8010C698(pEm);
        if ((*pEm->pNow_seq & 0xF000) == 0x1000)
            func_8010C698(pEm);
        if ((*pEm->pNow_seq & 0xF000) == 0x7000)
            func_8010C698(pEm);
    }
    func_8001AECC(pEm, 0xFDA80001, 0x190000B);
    if (EM10_W(pEm)->Ude_down)
        func_80103EF0(pEm);
    EM10_W(pEm)->Cnt++;
    pEm->pEnemy_ptr = pPl;
    pEm->Unk120 &= 0xFD;
    if ((pEm->Type & 0x4000) || (*pFlg & 0x200) || !(pEm->Status_flg & 0x2000))
        pEm->Unk120 |= 2;
    Rot_neck_em(pEm, pEm->Cdir_y);
    if (*pFlg & 1)
        func_80103510(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    if (*pFlg & 0x180)
        func_80103B48(pEm);
    EM10_W(pEm)->Routine = *(ULONG *)&pEm->Routine_0;
    Em_seq_sound(pEm);
}

/* RE2 Em10_Init */
void func_80100548(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG Tex;
    LONG i;

    EM_MOVE(pEm) = 0;
    EM_RTN(pEm) = 1;
    pEm->Damage_cnt = 0;
    pEm->Spd_x = 0;
    EM10_W(pEm)->Walk_no = Rnd() % 3;
    EM10_W(pEm)->Unk27 = Rnd() % 3 + 3;
    EM10_W(pEm)->u2C.w.Unk2E = (Rnd() & 1) + 6;
    EM10_W(pEm)->Flg = 0;
    EM10_W(pEm)->Unk01 = 0;
    EM10_W(pEm)->Unk0A = 0;
    EM10_W(pEm)->Unk0B = (Rnd() & 0xF) + 0x10;
    EM10_W(pEm)->Unk18 = 0;
    EM10_W(pEm)->Unk19 = 0;
    EM10_W(pEm)->Cnt = 0;
    EM10_W(pEm)->Unk21 = 0;
    EM10_W(pEm)->Unk22 = 0;
    EM10_W(pEm)->Unk23 = 0;
    EM10_W(pEm)->Ude_down = 0;
    EM10_W(pEm)->u2C.w.Unk2F = 0;
    EM10_W(pEm)->Unk26 = 0;
    EM10_W(pEm)->Unk18 = 0;
    if (!(Rnd() & 7) && !(pEm->Status_flg & 0x400))
        EM10_W(pEm)->Flg |= 0x4000;
    Kage_work_set((ULONG *)&pEm->pKage_work, 0, 0x2580190, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
    pEm->Col_d0 = pEm->Col_w0 = 500;
    pEm->Col_h = 3000;
    pEm->pCol_em = pEm;
    pEm->Col_unk24 = 0;
    pEm->Col_unk28 = 0;
    pEm->Col_unk26 = 0;
    pEm->Col_flg = 0;
    pEm->Col_type = 2;
    func_80033580((COL_OBJ *)&pEm->Col_id, 0);
    pEm->Unk134 = 0x708;
    pEm->Unk136 = -0x4B0;
    pEm->Unk138 = 0x78;
    pEm->Unk139 = 0xC8;
    pEm->Unk13A = 0x190;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    pEm->Neck_no = 1;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[1];
    PW_FREE(pP)[2] = 0x60;
    PW_FREE(pP)[3] = 0x60;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[5] = 0x138;
    i = (Rnd() >> (Rnd() & 3)) & 0xF;
    pEm->Max_life = pEm->Life = Em10_life[i];
    if (G.System_flg & 0x100)
        pEm->Max_life = pEm->Life = Em10_life_easy[i];
    pEm->Max_life += 10;
    pEm->Life += 10;
    if (pEm->Id == 0x11 || pEm->Id == 0x12)
        pEm->Unk12A = 0xED4;
    if (pEm->Id == 0x2F)
        pEm->Unk12A = 0xE86;
    if (pEm->Type & 0x80) {
        pEm->Status_flg |= 1;
        pEm->Type &= 0xFF7F;
        func_8010879C(pEm);
    }
    EM_MOVE(pEm) = 0;
    if ((pEm->Type & 0xF) == 6) {
        EM_RTN(pEm) = 0x101;
        pEm->Type &= 0xFFF0;
    }
    if ((pEm->Type & 0xF) == 1) {
        EM_MOVE(pEm) = 0x18;
        EM_RTN(pEm) = 0x201;
        pEm->Unk134 = 0xC8;
        pEm->Unk136 = -0x12C;
    }
    if ((pEm->Type & 0xF) == 3) {
        EM_MOVE(pEm) = 0x18;
        pEm->Unk134 = 0xC8;
        EM_RTN(pEm) = 1;
        pEm->Unk136 = -0x12C;
        pEm->Type = (pEm->Type & 0xFFF0) | 1;
    }
    if ((pEm->Type & 0xF) == 2) {
        EM_MOVE(pEm) = 0x17;
        EM_RTN(pEm) = 0x701;
        EM10_W(pEm)->Flg |= 4;
        if (pEm->Add_dir)
            EM_MOVE(pEm) = (pEm->Add_dir << 8) + 0xA;
    }
    if ((pEm->Type & 0xF) == 4) {
        EM_MOVE(pEm) = 0x18;
        EM_RTN(pEm) = 0x701;
        if (pEm->Add_dir)
            EM_MOVE(pEm) = (pEm->Add_dir << 8) + 9;
    }
    if ((pEm->Type & 0xF) == 5) {
        EM_MOVE(pEm) = 0x17;
        EM_RTN(pEm) = 0x701;
        pEm->Type = 0x4002;
        pEm->Life = -1;
        EM10_W(pEm)->Flg |= 4;
    }
    if ((pEm->Type & 0xF) == 7) {
        EM_MOVE(pEm) = 0x18;
        EM_RTN(pEm) = 0x701;
        pEm->Type = 0x4004;
        pEm->Life = -1;
    }
    if ((pEm->Type & 0xF) == 8) {
        EM_MOVE(pEm) = 0x13;
        EM_RTN(pEm) = 0x801;
    }
    if ((pEm->Type & 0xF) == 9) {
        pEm->Cdir_z = -0x400;
        EM_MOVE(pEm) = 0x18;
        EM_RTN(pEm) = 0x301;
    }
    if ((pEm->Type & 0xF) == 0xA) {
        EM_MOVE(pEm) = 0;
        EM_RTN(pEm) = 0xF01;
        pP = pEm->pSin_parts_ptr;
        for (i = 15; i != 0; i--, pP++) {
            pP->UnkA2 = (pP->UnkA2 | 0x400) & ~1;
        }
    }
    Joint_move_r(pEm, pKan, pSeq, 0x100);
    if (pEm->Type & 1)
        pEm->Type |= 0x2000;
    if (pEm->Id == 0x10) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Unk4B & 8)
            Tex = Rnd() % 3;
        Tex = pEm->Unk4B & 7;
        if (Tex == 1 || Tex == 3) {
            pP[0].pTmd = pEm->pUnk174[0];
            pP[2].pTmd = pEm->pUnk174[1];
            pP[5].pTmd = pEm->pUnk174[2];
        }
        if (Tex == 2 || Tex == 3) {
            pP[8].pTmd = pEm->pUnk174[3];
            pP[9].pTmd = pEm->pUnk174[4];
            pP[10].pTmd = pEm->pUnk174[5];
            pP[12].pTmd = pEm->pUnk174[6];
            pP[13].pTmd = pEm->pUnk174[7];
        }
    }
    if (pEm->Id == 0x17) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Unk4B & 8)
            Tex = Rnd() % 3;
        Tex = pEm->Unk4B & 7;
        if (Tex == 1 || Tex == 3) {
            pP[0].pTmd = pEm->pUnk174[0];
            pP[2].pTmd = pEm->pUnk174[1];
            pP[3].pTmd = pEm->pUnk174[2];
            pP[5].pTmd = pEm->pUnk174[3];
            pP[6].pTmd = pEm->pUnk174[4];
        }
        if (Tex == 2 || Tex == 3) {
            pP[8].pTmd = pEm->pUnk174[5];
            pP[9].pTmd = pEm->pUnk174[6];
            pP[10].pTmd = pEm->pUnk174[7];
            pP[12].pTmd = pEm->pUnk174[8];
            pP[13].pTmd = pEm->pUnk174[9];
        }
    }
    if (pEm->Id == 0x13) {
        pP = pEm->pSin_parts_ptr;
        Tex = pEm->Unk4B & 7;
        if (pEm->Unk4B & 8)
            Tex = Rnd() & 1;
        if (Tex == 1) {
            pP[0].pTmd = pEm->pUnk174[0];
            pP[1].pTmd = pEm->pUnk174[1];
            pP[2].pTmd = pEm->pUnk174[2];
            pP[3].pTmd = pEm->pUnk174[3];
            pP[4].pTmd = pEm->pUnk174[4];
            pP[5].pTmd = pEm->pUnk174[5];
            pP[6].pTmd = pEm->pUnk174[6];
            pP[7].pTmd = pEm->pUnk174[7];
            pP[8].pTmd = pEm->pUnk174[8];
            pP[9].pTmd = pEm->pUnk174[9];
            pP[10].pTmd = pEm->pUnk174[10];
            pP[11].pTmd = pEm->pUnk174[11];
            pP[12].pTmd = pEm->pUnk174[12];
            pP[13].pTmd = pEm->pUnk174[13];
            pP[14].pTmd = pEm->pUnk174[14];
        }
    }
    G.pPldamage_Em[pEm->Id - 0x10] = func_80102340;
    G.pPldie_Em[pEm->Id - 0x10] = func_801030A8;
    pP = &pEm->pSin_parts_ptr[3];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[4];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[6];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[7];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[9];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[10];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[11];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[12];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[13];
    pP->UnkA2 |= 0x200;
    pP = &pEm->pSin_parts_ptr[14];
    pP->UnkA2 |= 0x200;
}

/* RE2 Em10_Move */
void func_80100EA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em10_Move_type[pEm->Type & 0xF](pEm, pKan, pSeq);
}

/* RE2 Em10_Move_type00 */
void func_80100EE8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em10_mv_br_type00[pEm->Routine_1](pEm);
    Em10_mv_type00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em10_Move_type01 */
void func_80100F6C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em10_mv_br_type01[pEm->Routine_1](pEm);
    Em10_mv_type01[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em10_Damage */
void func_80100FF0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;                        /* unused (frame size) */
    LONG Lim;

    if (!pEm->Routine_2) {
        if ((pEm->Unk132 & 0x100) && !(pEm->Status_flg & 1))
            func_8010879C(pEm);
        if ((pEm->Unk132 & 0x400) && !(pEm->Status_flg & 1))
            func_80108B00(pEm);
        if ((pEm->Unk132 & 0x200) && !(pEm->Status_flg & 2))
            func_80108CBC(pEm);
        if ((pEm->Unk132 & 0x800) && !(pEm->Status_flg & 2))
            func_801089A8(pEm);
    }
    if (pEm->Type & 1) {
        Em10_haidm[pEm->Unk12C - 1](pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Flg & 0x10) {
        func_80109E54(pEm, pKan, pSeq);
        return;
    }
    if (pEm->Status_flg & 0x400) {
        EM10_W(pEm)->Unk0B = 0xF;
        EM10_W(pEm)->Unk0A = 0;
    }
    Lim = 0;
    if (!pEm->Routine_2 && EM10_W(pEm)->Unk0B <= Lim) {
        if (EM10_W(pEm)->Unk0A == 1 || (pEm->Unk12C == 1 && (LONG)G.Status_flg >= 0)) {
            func_8010C57C(pEm);
            EM10_W(pEm)->Unk0A = 1;
            EM_RTN(pEm) = 0x501;
            EM10_W(pEm)->Unk0B = (Rnd() & 0xF) + 0x10;
            return;
        }
        EM10_W(pEm)->Unk0A = 1;
    }
    if (EM10_W(pEm)->Flg & 2) {
        pEm->UnkE3 = 1;
        EM_RTN(pEm) = 0x60501;
        return;
    }
    Em10_dm[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80101260(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_3 = 0;
        if (pEm->Unk132 & 8)
            pEm->Routine_3 = 1;
        if (!(pEm->Unk132 & 0xC) && !(Rnd() & 3) && !(pEm->UnkAE & 0x40) && !(pEm->Type & 0x20)
            && !(pEm->Status_flg & 0x400))
            pEm->Routine_3 = 2;
    }
    switch (pEm->Routine_3) {
    case 0:
        func_801082B0(pEm, pKan, pSeq);
        break;
    case 1:
        func_80107C10(pEm, pKan, pSeq);
        break;
    case 2:
        func_80108E6C(pEm, pKan, pSeq);
        break;
    }
}

void func_8010139C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        pEm->Routine_3 = 0;
        if (!(pEm->Unk132 & 0xC))
            pEm->Routine_3 = 1;
    }
    switch (pEm->Routine_3) {
    case 0:
        func_801082B0(pEm, pKan, pSeq);
        break;
    case 1:
        func_8010B278(pEm, pKan, pSeq);
        break;
    }
}

/* RE2 Em10_Die */
void func_80101418(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!pEm->Routine_2) {
        if ((pEm->Unk132 & 0x100) && !(pEm->Status_flg & 1))
            func_8010879C(pEm);
        if ((pEm->Unk132 & 0x400) && !(pEm->Status_flg & 1))
            func_80108B00(pEm);
        if ((pEm->Unk132 & 0x200) && !(pEm->Status_flg & 2))
            func_80108CBC(pEm);
        if ((pEm->Unk132 & 0x800) && !(pEm->Status_flg & 2))
            func_801089A8(pEm);
    }
    EM10_W(pEm)->Flg &= 0xFDFF;
    if (pEm->Type & 1) {
        func_8010AA5C(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Flg & 0x10) {
        func_8010BB08(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Flg & 2) {
        func_8001B484(0, 0x1F400000, &pEm->pSin_parts_ptr->Workm, 0, 0);
        if (Rnd() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        EM_RTN(pEm) = 0x907;
        return;
    }
    Em10_die[pEm->Unk12C - 1][pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em10_Scenario */
void func_801015FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    LONG i;

    if (pEm->Status_flg & 1) {
        pP = pEm->pSin_parts_ptr;
        i = 15;
        if ((UCHAR)pP->Poly_rgb != 0x20 && !(pEm->UnkDA & 3)) {
            for (; i != 0; i--, pP++)
                pP->Poly_rgb -= 0x10101;
        }
    }
    if (pEm->Type & 0x100)
        func_80016600(pEm, pKan, pSeq);
    else
        Em10_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

/* RE2 Em10_sce00 */
void func_801016B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x1F0000;
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        Joint_move_r(pEm, pKan, pSeq, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_sce01 */
void func_80101754(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0x1F0000;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_sce02 */
void func_801017F4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Type_mot[4][2] = { { 12, 12 }, { 15, 15 }, { 12, 12 }, { 15, 15 } }; /* as Em10_mv03 */
    SVECTOR Sv;
    MATRIX M;                          /* unused (frame size) */
    ENEMY_WORK *pAt = G.pEnemy[pEm->Unk14A];
    UCHAR Atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } };
    VECTOR V;                          /* unused (frame size) */
    PARTS_W *pP;
    ULONG Hai;

    Hai = pEm->Type & 1;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Type_mot[Hai][0] + 0xF0000;
        pEm->Routine_2 = 1;
        Goto00(pEm, pAt->Pos_x, pAt->Pos_z, 0x800);
        pAt->pEnemy_ptr = pEm;
        Set_base_pos(pAt, (ULONG)pKan, (ULONG)pSeq, 0);
        if (Hai & 1)
            pAt->Routine_1 += 2;
        pAt->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pAt->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pEm->Be_flg |= 0x800;
        pAt->Be_flg |= 0x800;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
    case 1:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_MOVE(pEm) = Type_mot[Hai][0] + 1;
        pEm->Routine_2 = 3;
    case 3:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        Joint_move_r(pEm, pKan, pSeq, 0x100);
        if (pEm->Add_dir)
            pEm->Routine_2 = 4;
        if (pEm->Move_cnt == Atck[Hai][0])
            Snd_se_enem((Rnd() & 1) | 4, pEm);
        break;
    case 4:
        EM_MOVE(pEm) = Type_mot[Hai][0] + 0x70002;
        pEm->Routine_2 = 5;
    case 5:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        if (Hai & 1) {
            if (pEm->Move_cnt == 0x1C) {
                EM10_W(pEm)->Flg |= 0x80;
                Snd_se_enem(10, pEm);
                Pad_vib_set1(4, 0xFA, 0);
            }
            if (pEm->Move_cnt >= 0x17 && pEm->Move_cnt < 0x2A && (pEm->Move_cnt & 1)) {
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 0;
                Sv.vy = 100;
                Sv.vz = 0;
                func_8001B484(0, (USHORT)(pEm->Cdir_y + (Rnd() << 2) - 0x200) | 0x10000000, &pP[1].Workm, &Sv, 0);
            }
        }
        if (!(Hai & 1) && pEm->Move_cnt == 7)
            pEm->Routine_2 = 7;
        break;
    case 6:
        EM_RTN(pEm) = 7;
        pEm->Life = -1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Damage_cnt |= 0x80;
        break;
    case 7:
        func_80018EC8(pEm, pKan, pSeq, 0);
        func_80018EC8(pEm, pKan, pSeq, 0);
        pEm->Routine_2 = 8;
    case 8:
        func_80018EC8(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x -= 10;
        Add_speedXZ(pEm, 0);
        break;
    case 9:
        EM_RTN(pEm) = 0x101;
        break;
    }
}

/* RE2 Em10_sce03 */
void func_80101C68(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        EM_MOVE(pEm) = (pEm->Unk14C << 8) + 0x70000 + pEm->Add_dir;
    case 1:
        Spd_get(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (pEm->Unk14C && !pEm->UnkE3) {
            pEm->UnkE3 = 1;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
        }
        pEm->Routine_2 += Joint_move_r(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        if (pEm->Unk14A) {
            EM_RTN(pEm) = 0x101;
            if (pEm->Unk14A & 2)
                EM_RTN(pEm) = 0x201;
            if (pEm->Unk14A & 4)
                EM_RTN(pEm) = 0xB01;
            if (pEm->Unk14A & 8) {
                EM_RTN(pEm) = 0x20701;
                pEm->Add_dir = 0;
            }
            if (pEm->Type & 1)
                EM_RTN(pEm) = 1;
        }
        break;
    }
}

void func_80101DD8(void) {
}

/* RE2 Em10_Dead */
void func_80101DE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    KAGE_WORK *pKage;
    PARTS_W *pP;
    LONG i;

    pKage = (KAGE_WORK *)pEm->pKage_work;
    if (pEm->Be_flg & 8) {
        if (pEm->Unk178[2] > 0) {
            if ((pEm->Unk178[2] -= 16) < 0)
                pEm->Unk178[2] = 0;
        }
        if (pEm->Unk178[0] > 0) {
            if ((pEm->Unk178[0] -= 16) < 0)
                pEm->Unk178[0] = 0;
        }
        if (pEm->Unk178[2] < 0) {
            if ((pEm->Unk178[2] += 16) > 0)
                pEm->Unk178[2] = 0;
        }
        if (pEm->Unk178[0] < 0) {
            if ((pEm->Unk178[0] += 16) > 0)
                pEm->Unk178[0] = 0;
        }
        if (!pEm->Unk178[0] && !pEm->Unk178[2]) {
            pEm->Be_flg &= ~8;
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 &= ~0x40;
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 &= ~0x40;
        }
    }
    switch (pEm->Routine_1) {
    case 0:
        EM_MOVE(pEm) = 0x70018;
        pEm->Unk120 |= 1;
        pEm->Neck_no = 0;
        if (EM10_W(pEm)->Flg & 4)
            EM_MOVE(pEm) = 0x70017;
        if (pEm->Life == -30000) {
            pEm->UnkCA = 0;
            Joint_move_r(pEm, pKan, pSeq, 0x200);
        }
        pEm->Life = -1;
        pEm->Col_id = 0;
        pEm->Routine_1 = 1;
        *(ULONG *)&pKage->Prim[0].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[0].r0 |= 0xBFBF10;
        *(ULONG *)&pKage->Prim[1].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[1].r0 |= 0xBFBF10;
        pEm->UnkE2 = 120;
        pEm->Timer0 = 40;
        EM10_W(pEm)->Unk20 = 0;
        pEm->Spd_x = 0;
        Em_dead_flg_on_cand(pEm);
        if (!(pEm->Type & 0x4000) && Rnd() % 3) {
            pEm->Routine_1 = 8;
            return;
        }
        pEm->UnkDA = 0;
    case 1:
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pKage->Half_x += 5;
            pKage->Half_z += 5;
        }
        pEm->Routine_1 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        pEm->UnkE3 = (Rnd() & 0xF) + 1;
        pEm->Routine_1 = 3;
        if (pEm->Timer0-- == 0) {
            pEm->Routine_1 = 4;
            pEm->Timer0 = 30;
            break;
        }
    case 3:
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pKage->Half_x += 5;
            pKage->Half_z += 5;
        }
        if (pEm->UnkE3-- == 0)
            pEm->Routine_1 = 1;
        break;
    case 4:
        if (pEm->UnkE3-- == 0)
            pEm->Routine_1 = 5;
        break;
    case 6:
        pEm->UnkE3 = (Rnd() & 0x3F) + 60;
        pEm->Routine_1 = 4;
        if (pEm->Timer0-- == 0)
            pEm->Routine_1 = 7;
        break;
    case 8:
        if (pEm->UnkE2) {
            pEm->UnkE2--;
            pKage->Half_x += 5;
            pKage->Half_z += 5;
        }
        if (!pEm->UnkE2)
            pEm->Routine_1 = 7;
        break;
    case 9:
        EM_MOVE(pEm) = 0x70008;
        if (EM10_W(pEm)->Flg & 4)
            EM_MOVE(pEm) = 0x70017;
        pEm->Routine_1 = 10;
        Em_dead_flg_on_cand(pEm);
    case 5:
    case 10:
        pEm->Routine_1 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        break;
    case 11:
        pEm->Routine_1 = 0;
    }
    pEm->UnkDA++;
    if (pEm->Status_flg & 1) {
        pP = pEm->pSin_parts_ptr;
        i = 15;
        if ((UCHAR)pP->Poly_rgb != 0x20 && !(pEm->UnkDA & 3)) {
            do {
                pP->Poly_rgb -= 0x10101;
                pP++;
            } while (--i);
        }
    }
    if ((USHORT)pEm->Water > 1) {
        if (!(EM10_W(pEm)->Unk20 & 0x80)) {
            pEm->Pos_y -= 5;
            if (pEm->Pos_y < pEm->Water + 300) {
                pEm->Pos_y = pEm->Water + 300;
                EM10_W(pEm)->Unk20 = 0x80;
            }
        } else {
            if (EM10_W(pEm)->Unk20 < 0xA0)
                pEm->Pos_y -= 2;
            else {
                pEm->Pos_y += 2;
                if (EM10_W(pEm)->Unk20 == 0xBF)
                    EM10_W(pEm)->Unk20 = 0x80;
            }
            EM10_W(pEm)->Unk20++;
        }
    }
}

/* RE2 PLDAMEGE_EM (Pldamege_Em110), called for the player held by this enemy */
void func_80102340(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    Pldamage_Em10_tbl[pEm->Routine_1](pEm, pKan, pSeq);
    if (pEm->Water) {
        pP = pEm->pSin_parts_ptr;
        if (pP->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 7) == 1)
            func_8010C698(pEm);
    }
}

/* RE2 Em10_pldm00: the player (pEm) is bitten from the front or back (Routine_1 & 1) */
void func_801023D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    UCHAR Mot[2] = { 0, 3 };
    SHORT Blo[2][3] = { { 0, 200, 100 }, { -100, 100, -200 } };
    ENEMY_WORK *pAi;
    ENEMY_WORK *pT;
    PARTS_W *pP;
    SHORT Old_x;
    SHORT Old_z;
    SHORT Ck_flg;
    LONG i;

    pAi = pEm->pEnemy_ptr;
    if (pAi->Routine_0 == 2 || pAi->Routine_0 == 3) {
        EM_RTN(pEm) = 1;
        G.Pl_damage_cnt &= 0x7F;
        pEm->Unk120 &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        pEm->Be_flg |= 0x80000;
        return;
    }
    pEm->Be_flg |= 0x8000;
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C(pEm, 200, 20, pEm->Col_unk39);
        EM_MOVE(pEm) = Mot[pEm->Routine_1] + 0x30000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        pEm->UnkE2 = 0;
        if (pEm->Routine_1 & 1)
            pEm->Timer0 = 0;
        Snd_se_stad(((Rnd() & 1) << 1) | 0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->UnkDA = 0;
        pEm->Unk12C = 0;
        pEm->Unk120 |= 0x12;
    case 1:
        Goto01(pEm, pAi->Cdir_y + pEm->Timer0, 0xD0);
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->UnkCA) >> 2;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->UnkCA) >> 2;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        EM_MOVE(pEm) = Mot[pEm->Routine_1] + 1;
        pEm->Routine_2 = 3;
    case 3:
        if (pEm->Move_cnt == 15) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            func_8001B484(0, (USHORT)(pEm->Cdir_y + (Rnd() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0x17D00000,
                          &pP[1].Workm, &Sv, 0);
            pEm->UnkE2 = 0;
        }
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        Joint_move2(pEm, pKan, pSeq, 0x800);
        break;
    case 4:
        EM_MOVE(pEm) = Mot[pEm->Routine_1] + 0x70002;
        pEm->Routine_2 = 5;
        if (pEm->UnkE2) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            func_8001B484(0, (USHORT)(pEm->Cdir_y + (Rnd() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0x17D00000,
                          &pP[1].Workm, &Sv, 0);
        }
    case 5:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->UnkCA) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->UnkCA) >> 3;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        if ((UCHAR)(pEm->Move_cnt - 9) < 5) {
            Ck_flg = pEm->UnkBA;
            Ck_flg <<= 2;
            i = 2;
            while (Ck_flg) {
                if ((Ck_flg & 0x8000) && !((pEm->UnkDA >> i) & 1)) {
                    pT = G.pEnemy[i];
                    if ((pT->Id & 0x70) == 0x10 && pT->Life >= 0 && !(pT->Type & 1) && !pT->Damage_cnt
                        && !(EM10_W(pT)->Flg & 8) && !(pT->Type & 0x2000)) {
                        EM_RTN(pT) = 0x901;
                        pT->UnkE3 = ~Cdir_ck2(pEm, pT) & 1;
                    }
                }
                i++;
                Ck_flg <<= 1;
            }
            pEm->UnkDA |= pEm->UnkBA;
        }
        break;
    case 6:
        EM_RTN(pEm) = 1;
        pEm->Be_flg |= 0x80000;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk120 &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        pEm->Damage_cnt = 3;
        pEm->Be_flg |= 0x10000000;
        break;
    }
}

/* RE2 Em10_pldm02 */
void func_80102AA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    UCHAR Mot[2] = { 6, 10 };
    SHORT Blo[2][3] = { { -400, -100, 0 }, { 200, -100, 0 } };
    ENEMY_WORK *pAi;
    PARTS_W *pP;
    SHORT Old_x;
    SHORT Old_z;

    pAi = pEm->pEnemy_ptr;
    if (pAi->Routine_0 == 2 || pAi->Routine_0 == 3) {
        EM_RTN(pEm) = 1;
        G.Pl_damage_cnt &= 0x7F;
        pEm->Unk120 &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        return;
    }
    pEm->Be_flg |= 0x8000;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Mot[pEm->Routine_1 - 2] + 0x70000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        if (pEm->Routine_1 & 1)
            pEm->Timer0 = 0;
        Snd_se_stad(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk12C = 0;
        pEm->Unk120 |= 0x12;
    case 1:
        if (pEm->Move_cnt < 8)
            Goto01(pEm, pAi->Cdir_y + pEm->Timer0, 0x80);
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->UnkCA) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->UnkCA) >> 3;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_MOVE(pEm) = Mot[pEm->Routine_1 - 2] + 1;
        pEm->Routine_2 = 3;
    case 3:
        if (!pEm->Move_cnt) {
            pP = &pEm->pSin_parts_ptr[10];
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            func_8001B484(0, (USHORT)(pEm->Cdir_y + (Rnd() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0xFD00000,
                          &pP->Workm, &Sv, 0);
        }
        if (pEm->Move_cnt == 3) {
            pP = &pEm->pSin_parts_ptr[10];
            Sv.vx = Blo[pEm->Routine_1 & 1][0];
            Sv.vy = Blo[pEm->Routine_1 & 1][1];
            Sv.vz = Blo[pEm->Routine_1 & 1][2];
            func_8001B484(0, (USHORT)(pEm->Cdir_y + (Rnd() << 1) - 0x100 + (pEm->Routine_1 & 1) * 0x800) | 0x17D00000,
                          &pP->Workm, &Sv, 0);
        }
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        Joint_move2(pEm, pKan, pSeq, 0x800);
        break;
    case 4:
        EM_MOVE(pEm) = Mot[pEm->Routine_1 - 2] + 0x70002;
        pEm->Routine_2 = 5;
    case 5:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->UnkCA) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->UnkCA) >> 3;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 6:
        EM_MOVE(pEm) = 9;
        pEm->Routine_2 = 7;
        if (pEm->Routine_1 & 1) {
            pEm->Timer0 = 0x800;
            pEm->Cdir_y += 0x800;
        }
    case 7:
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 8:
        EM_RTN(pEm) = 1;
        pEm->Damage_cnt &= 0x7F;
        pEm->Unk120 &= 0xED;
        pEm->Be_flg &= ~0x8800;
        pAi->Be_flg &= ~0x8800;
        pEm->Damage_cnt = 3;
        pEm->Be_flg |= 0x10000000;
        break;
    }
}

/* RE2 PLDIE_EM (Pldie_Em110), called for the player held by this enemy */
void func_801030A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    Pldie_Em10_tbl[pEm->Routine_1](pEm, pKan, pSeq);
    if (pEm->Water) {
        pP = pEm->pSin_parts_ptr;
        if (pP->Workm.t[1] - 500 < pEm->Water && (pEm->Move_cnt & 7) == 1)
            func_8010C698(pEm);
    }
}

/* RE2 Em10_pldie00: the player (pEm) is eaten */
void func_80103138(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    SVECTOR Sv;
    UCHAR Mot[2] = { 13, 15 };

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Mot[pEm->Routine_1] + 0xF0000;
        func_8002646C(pEm, 200, 20, pEm->Col_unk39);
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        if (pEm->Routine_1 & 1)
            pEm->Timer0 = 0;
        pEm->Unk120 |= 1;
        Snd_se_stad(0x10003, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Unk120 |= 0x12;
        Pad_vib_fade_set(0x21, 0x32, 0x96, 0);
        Pad_vib_set1(8, 0xFA, 0x12);
        Pad_vib_set0(0x32, 0x32);
        Pad_vib_set1(6, 0xFA, 0x3C);
        Pad_vib_set1(6, 0xFA, 0x47);
        Pad_vib_set1(6, 0xFA, 0x64);
        Pad_vib_fade_set(0x5A, 0xFA, 0, 0x6A);
    case 1:
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x100);
        if (pEm->Move_cnt >= 41 && pEm->Move_cnt < 99 && !(pEm->Move_cnt & 1)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            func_8001B484(0, ((Rnd() << 3) + 0xBD0) << 16 | (USHORT)(Rnd() << 4), &pP[1].Workm, &Sv, 0);
        }
        if (pEm->Move_cnt == 50) {
            PARTS_W *pW;

            pW = &pEm->pSin_parts_ptr[1];
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
            pW = pEm->pSin_parts_ptr;
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
            pW = &pEm->pSin_parts_ptr[2];
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
            pW = &pEm->pSin_parts_ptr[5];
            pW->UnkA2 |= 4;
            PW_FREEL(pW)[0] = 0x20;
            PW_FREEL(pW)[1] = 0x101030;
        }
        if (pEm->Move_cnt >= 100 && pEm->Move_cnt < 105) {
            PARTS_W *pS;

            pS = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            func_8001B484(0, (USHORT)(Rnd() << 4) | 0x17D00000, &pS[1].Workm, &Sv, 0);
            pS = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            func_8001B484(0, (USHORT)(Rnd() << 4) | 0x1BD00000, &pS[1].Workm, &Sv, 0);
        }
        break;
    case 2:
        pEm->Routine_2 = 3;
        pEm->Move_no++;
        pEm->Move_cnt = 0;
        pEm->Life = -0x8000;
    case 3:
        pEm->UnkCA = 10;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x100);
        break;
    case 4:
        pEm->Routine_2 = 5;
        pEm->UnkE3 = (Rnd() & 0x1F) + 3;
    case 5:
        if (pEm->UnkE3-- == 0)
            pEm->Routine_2 = 3;
        break;
    case 6:
        EM_RTN(pEm) = 7;
        break;
    }
}

/* RE2 Em10_Kahannsin_walk: the lower half crawling on its own */
void func_80103510(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    JOINT_P Jp = { 0, 0, 0, 0, 0x7F00 };
    SHORT Tmp;
    SVECTOR Spd;
    SVECTOR Sv;
    MATRIX M;
    SHORT Ground;
    KAGE_WORK *pKage;
    PARTS_W *pP;

    switch (EM10_W(pEm)->Unk01) {
    case 0:
        EM10_W(pEm)->Unk06 = 7;
        pP = &pEm->pSin_parts_ptr[8];
        EM10_W(pEm)->Unk05 = 0;
        EM10_W(pEm)->Unk04 = EM10_W(pEm)->Walk_no;
        pEm->Super_matrix.t[0] = pP->Workm.t[0];
        pEm->Super_matrix.t[2] = pP->Workm.t[2];
        EM10_W(pEm)->Unk01 = 1;
        EM10_W(pEm)->Unk08 = pEm->Cdir_y;
        EM10_W(pEm)->Unk07 = (Rnd() & 0x1F) + 20;
        Kage_work_set(&EM10_W(pEm)->Unk0C, 0, 0x2BC01F4, 0, (VECTOR *)pEm->Super_matrix.t);
    case 1:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = EM10_W(pEm)->Unk05;
        Jp.Hokan_flg = EM10_W(pEm)->Unk06;
        func_80028308(&Jp, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
        EM10_W(pEm)->Unk04 = Jp.Move_no;
        EM10_W(pEm)->Unk05 = Jp.Move_cnt;
        EM10_W(pEm)->Unk06 = Jp.Hokan_flg;
        Spd.vx = 0;
        Spd.vy = EM10_W(pEm)->Unk08;
        Spd.vz = 0;
        RotMatrix(&Spd, &M);
        Spd.vx = (Rnd() & 7) + 10;
        Spd.vy = 0;
        ApplyMatrixSV(&M, &Spd, &Sv);
        Ground = func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1);
        pEm->Super_matrix.t[0] += Sv.vx;
        pEm->Super_matrix.t[2] += Sv.vz;
        if (Ground != (SHORT)func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1)) {
            pEm->Super_matrix.t[0] -= Sv.vx;
            pEm->Super_matrix.t[2] -= Sv.vz;
        }
        MulKage((KAGE_WORK *)EM10_W(pEm)->Unk0C, Ground, EM10_W(pEm)->Unk08, 0);
        if (EM10_W(pEm)->Unk07-- == 0)
            EM10_W(pEm)->Unk01 = 2;
        break;
    case 2:
        EM10_W(pEm)->Unk04 = 2;
        EM10_W(pEm)->Unk05 = 0;
        EM10_W(pEm)->Unk06 = 7;
        EM10_W(pEm)->Unk01 = 3;
    case 3:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = EM10_W(pEm)->Unk05;
        Jp.Hokan_flg = EM10_W(pEm)->Unk06;
        EM10_W(pEm)->Unk01 += func_80028308(&Jp, pKan, pSeq, 0x200) & 1;
        EM10_W(pEm)->Unk04 = Jp.Move_no;
        EM10_W(pEm)->Unk05 = Jp.Move_cnt;
        EM10_W(pEm)->Unk06 = Jp.Hokan_flg;
        Spd.vx = 0;
        Spd.vy = EM10_W(pEm)->Unk08;
        Spd.vz = 0;
        RotMatrix(&Spd, &M);
        Spd.vx = 30;
        Spd.vy = 0;
        ApplyMatrixSV(&M, &Spd, &Sv);
        Ground = func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1);
        pEm->Super_matrix.t[0] += Sv.vx;
        pEm->Super_matrix.t[2] += Sv.vz;
        if (Ground != (SHORT)func_8004D720(pEm->Super_matrix.t, 500, 0x200, 1)) {
            pEm->Super_matrix.t[0] -= Sv.vx;
            pEm->Super_matrix.t[2] -= Sv.vz;
        }
        MulKage((KAGE_WORK *)EM10_W(pEm)->Unk0C, Ground, EM10_W(pEm)->Unk08, 0);
        break;
    case 4:
        pEm->pSin_parts_ptr[8].Be_flg |= 2;
        EM10_W(pEm)->Unk01 = 5;
        EM10_W(pEm)->Unk04 = 0x17;
        EM10_W(pEm)->Unk05 = 0;
        EM10_W(pEm)->Unk06 = 7;
        EM10_W(pEm)->Unk07 = (Rnd() & 0x1F) + 10;
        pKage = (KAGE_WORK *)EM10_W(pEm)->Unk0C;
        *(ULONG *)&pKage->Prim[0].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[0].r0 |= 0xBFBF10;
        *(ULONG *)&pKage->Prim[1].r0 &= 0xFF000000;
        *(ULONG *)&pKage->Prim[1].r0 |= 0xBFBF10;
        if (Rnd() & 1) {
            EM10_W(pEm)->Unk01 = 8;
            return;
        }
    case 5:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = EM10_W(pEm)->Unk05;
        Jp.Hokan_flg = EM10_W(pEm)->Unk06;
        EM10_W(pEm)->Unk01 += func_80028308(&Jp, pKan, pSeq, 0x200);
        EM10_W(pEm)->Unk04 = Jp.Move_no;
        EM10_W(pEm)->Unk05 = Jp.Move_cnt;
        EM10_W(pEm)->Unk06 = Jp.Hokan_flg;
        break;
    case 6:
        EM10_W(pEm)->Unk07 = (Rnd() & 0x1F) + 60;
        EM10_W(pEm)->Unk01 = 7;
        if (!(Rnd() & 0xF)) {
            EM10_W(pEm)->Unk01 = 8;
            return;
        }
    case 7:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = 0;
        Jp.Hokan_flg = 0;
        func_80028308(&Jp, pKan, pSeq, 0x200);
        if (EM10_W(pEm)->Unk07-- == 0)
            EM10_W(pEm)->Unk01 = 5;
        break;
    case 8:
        Jp.Move_no = EM10_W(pEm)->Unk04;
        Jp.Move_cnt = 0;
        Jp.Hokan_flg = 0;
        func_80028308(&Jp, pKan, pSeq, 0x200);
        break;
    }
    if (EM10_W(pEm)->Unk01 < 4) {
        Tmp = pEm->Cdir_y;
        pEm->Cdir_y = EM10_W(pEm)->Unk08;
        RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
        pEm->Cdir_y = Tmp;
        MulMatrix0((MATRIX *)pEm->Workm_m, &Jp.P0_m, &pEm->Super_matrix);
        pEm->Super_matrix.t[1] = Jp.P0_m.t[1] + EM10_W(pEm)->Unk1A;
    }
    pP = &pEm->pSin_parts_ptr[8];
    pP[0].UnkA2 |= 1;
    pP[1].UnkA2 |= 1;
    pP[2].UnkA2 |= 1;
    pP[3].UnkA2 |= 1;
    pP[4].UnkA2 |= 1;
    pP[5].UnkA2 |= 1;
    pP[6].UnkA2 |= 1;
    if (!func_8001020C(pEm->Super_matrix.t, G.Cccut)) {
        pP[0].UnkA2 &= ~1;
        pP[1].UnkA2 &= ~1;
        pP[2].UnkA2 &= ~1;
        pP[3].UnkA2 &= ~1;
        pP[4].UnkA2 &= ~1;
        pP[5].UnkA2 &= ~1;
        pP[6].UnkA2 &= ~1;
    }
}

/* RE2 Em10_Kubigoro: the head rolling after it was shot off */
void func_80103B48(ENEMY_WORK *pEm)
{
    MATRIX M;
    SVECTOR Tmp_sv = { 300, -300, 0 };
    VECTOR Tv0;
    SHORT Ground;
    SHORT Ground2;
    PARTS_W *pP;

    pP = &pEm->pSin_parts_ptr[1];
    if (EM10_W(pEm)->Flg & 0x80) {
        pEm->Unk120 = 1;
        pP->Be_flg |= 0x22;
        EM10_W(pEm)->Flg &= 0xFF7F;
        EM10_W(pEm)->Flg |= 0x100;
        M = GsIDMATRIX;
        RotMatrixY(pEm->Cdir_y + 0x800, &M);
        ApplyMatrixSV(&M, &Tmp_sv, (SVECTOR *)&pP->Old_x);
        pP->Unk85 = 2;
    }
    M = GsIDMATRIX;
    RotMatrixZ(0x80, &M);
    RotMatrixX(0x40, &M);
    func_8002D4B0(&pP->Workm, &M, &pP->Workm);
    Tv0 = *(VECTOR *)pP->Workm.t;
    Ground = func_8004D720(pP->Workm.t, 100, 0x2000, 1);
    pP->Workm.t[0] += pP->Old_x;
    pP->Workm.t[1] += pP->Old_y;
    pP->Workm.t[2] += pP->Old_z;
    Ground2 = func_8004D720(pP->Workm.t, 100, 0x2000, 1);
    if (Ground > Ground2) {
        *(VECTOR *)pP->Workm.t = Tv0;
        Ground2 = Ground;
        pP->Old_x *= -1;
        pP->Old_z *= -1;
        func_8001B484(0, (USHORT)(Rnd() << 4) | 0x3E80000, &pP->Workm, 0, 0);
        if (pP->Unk85-- == 0) {
            EM10_W(pEm)->Flg &= 0xFEFF;
            pP->Be_flg = 0;
            func_8001B894(0x305, &pP->Workm);
            return;
        }
    }
    pP->Old_y += 50;
    if (Ground2 - 200 < pP->Workm.t[1]) {
        pP->Workm.t[1] = Ground2 - 200;
        pP->Old_x >>= 1;
        pP->Old_z >>= 1;
        pP->Old_y = -(pP->Old_y >> 2);
        func_8001B484(0, (USHORT)(Rnd() << 4) | 0x3E80000, &pP->Workm, 0, 0);
        Snd_se_enem(9, pEm);
        if (pP->Unk85-- == 0) {
            EM10_W(pEm)->Flg &= 0xFEFF;
            pP->Be_flg = 0;
            func_8001B894(0x305, &pP->Workm);
        }
    }
}

/* RE2 Em10_ude_down: an arm (parts 2..4 or 5..7) torn off by the player */
void func_80103EF0(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;

    pP = &pEm->pSin_parts_ptr[(EM10_W(pEm)->Unk25 >> 1) * 3 + 2];
    switch (EM10_W(pEm)->Ude_down) {
    case 1:
        pP->Be_flg |= 0x10;
        pP[2].Be_flg |= 0x10;
        pP->pSuper = &G.Pl_pSin_parts_ptr->Workm;
        pP->pOya_parts = G.Pl_pSin_parts_ptr;
        MulLMatrix(&G.Pl_pSin_parts_ptr->Workm, &pP->Workm, (MATRIX *)pP->M);
        MulLMatrix(&G.Pl_pSin_parts_ptr->Workm, &pP[2].Workm, &M);
        EM10_W(pEm)->Unk28 = M.t[0] >> 1;
        EM10_W(pEm)->Unk2A = M.t[1] + 200;
        EM10_W(pEm)->u2C.w.Unk2C = M.t[2] >> 1;
        EM10_W(pEm)->Ude_down = 2;
        EM10_W(pEm)->Unk23 = 90;
        Snd_se_enem(11, pEm);
        if (SquareRoot0(EM10_W(pEm)->Unk28 * EM10_W(pEm)->Unk28 + EM10_W(pEm)->u2C.w.Unk2C * EM10_W(pEm)->u2C.w.Unk2C) > 800) {
            EM10_W(pEm)->Ude_down = 3;
            break;
        }
    case 2:
        if (EM10_W(pEm)->Unk23-- == 0)
            EM10_W(pEm)->Ude_down = 3;
        MulLMatrix(&G.Pl_pSin_parts_ptr->Workm, &pP[2].Workm, &M);
        pP->Pos_x += EM10_W(pEm)->Unk28 - M.t[0];
        pP->Pos_y += EM10_W(pEm)->Unk2A - M.t[1];
        pP->Pos_z += EM10_W(pEm)->u2C.w.Unk2C - M.t[2];
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = (Rnd() & 7) * (1 - ((EM10_W(pEm)->Unk23 & 1) << 1)) - 12;
        RotMatrix(&Sv, &M);
        MulMatrix((MATRIX *)pP->M, &M);
        if (!(EM10_W(pEm)->Unk23 & 3)) {
            func_8001B484((Rnd() & 1) << 8, ((Rnd() + 0xC18) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, 0, 0);
            Sv.vx = 0;
            Sv.vy = -500;
            Sv.vz = (EM10_W(pEm)->Unk25 >> 1) * 500 - 250;
            pP = pEm->pSin_parts_ptr;
            func_8001B484(0, (((Rnd() & 1) + 0xC18) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, &Sv, 0);
        }
        break;
    case 3:
        pP->Be_flg |= 0xA;
        pP->Old_x = pEm->Cdir_y + 0x800;
        pP->Unk96 = 0;
        pP->Old_z = 0;
        pP->Old_y = -20;
        pP->Unk85 = 30;
        pP->Unk90 = 0;
        pP->Unk92 = G.Pl_cdir_y + ((~EM10_W(pEm)->Unk25 & 1) << 11);
        pP->Unk8E = -0x400;
        EM10_W(pEm)->Ude_down = 4;
        break;
    case 0:
    case 4:
        break;
    }
}

/* RE2 Em10_mv00_br */
void func_80104244(ENEMY_WORK *pEm)
{
    SHORT Dir0;
    SHORT Dir1;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);
    if (pEm->Type & 0x10)
        return;
    if (LL < 5000 && !Dir0 && (pEm->Status_flg & 0x2000))
        EM_RTN(pEm) = 0x101;
    if (Dir1 && LL < 2000 && !pPl->Damage_cnt && !EM10_W(pEm)->Unk26)
        EM_RTN(pEm) = 0xC01;
}

/* RE2 Em10_mv00: standing / swaying in place */
void func_80104328(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x1F0000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = Rnd() + 60;
        pEm->Move_cnt = (Rnd() & 0xF) << 1;
        Spd_get(pEm, pKan, pSeq, 0);
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move_r(pEm, pKan, pSeq, 0x80)) {
            if (!pEm->Move_no) {
                if (!(Rnd() & 0xF)) {
                    EM_MOVE(pEm) = 0x1F0012;
                    if (!EM10_W(pEm)->Unk21 && (Rnd() & 1)) {
                        Snd_se_enem(14, pEm);
                        EM10_W(pEm)->Unk21 = 150;
                    }
                }
            } else
                EM_MOVE(pEm) = 0x1F0000;
        }
        Add_speedXZ(pEm, 0);
        if (!(pEm->Type & 0x10)) {
            if (!pEm->Timer0 && (LONG)pEm->Unk154 < 5000 && (pEm->Status_flg & 0x2000))
                EM_RTN(pEm) = 0x101;
            if ((pEm->Type & 0x40) && !pEm->Timer0)
                EM_RTN(pEm) = 0x101;
        }
        if (pEm->Timer0)
            pEm->Timer0--;
        break;
    }
}

/* RE2 Em10_mv01_br */
void func_801044F8(ENEMY_WORK *pEm)
{
    SHORT Dir0;
    SHORT Dir1;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);
    if (!EM10_W(pEm)->u2C.w.Unk2F) {
        if ((pEm->Unk158 & 0xC0000000) && (pEm->Unk158 & 0x3FFFFFFF) < 1500 && !Goto01_ck(pEm, pEm->Unk130, 0x100)) {
            EM_RTN(pEm) = 0xE01;
            return;
        }
        if (!EM10_W(pEm)->u2C.w.Unk2F && !Dir0 && LL < 1500 && pEm->Pos_y != pPl->Pos_y && !pPl->Damage_cnt
            && !(pEm->Status_flg & 0x800))
            EM_RTN(pEm) = 0xE01;
    }
    if (!Dir0 && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->UnkAE ^ pPl->UnkAE) & 0x40)) {
        EM_RTN(pEm) = 0x301;
        if (pPl->Be_flg & 0x10000000)
            EM_RTN(pEm) = 0x401;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
    if (Dir1 && LL < 2000 && !pPl->Damage_cnt && !EM10_W(pEm)->Unk26)
        EM_RTN(pEm) = 0xC01;
}

/* RE2 Em10_mv01: walking */
void func_801046EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Dir;
    SHORT Spd;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0x1F0000;
        pEm->UnkDA = (Rnd() >> 3) + 100;
        pEm->UnkE2 = (Rnd() & 0xF) + 30;
        pEm->UnkE3 = (Rnd() & 0xF) << 1;
        pEm->Timer0 = (Em10_walk_rnd[pEm->UnkE3] & 0x7FFF) + (Rnd() & 0x3F);
        pEm->Move_cnt = Rnd() & 0x1F;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x = 0;
    case 1:
        if (pEm->Timer0-- == 0) {
            if (++pEm->UnkE3 >= 32)
                pEm->UnkE3 = 0;
            pEm->Timer0 = (Em10_walk_rnd[pEm->UnkE3] & 0x7FFF) + (Rnd() & 0x1F);
            pEm->Cdir_y -= 0x10 - ((Rnd() & 1) << 5);
        }
        if ((LONG)pEm->Unk154 > 5000)
            Dir = 8 - ((Em10_walk_rnd[pEm->UnkE3] >> 15) << 4);
        else
            Dir = 8;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Dir);
        if (pEm->UnkE2-- == 0) {
            pEm->UnkE2 = (Rnd() & 0xF) + 30;
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
            if (!EM10_W(pEm)->Unk21) {
                if (!(Rnd() & 0x1F)) {
                    Snd_se_enem(13, pEm);
                    EM10_W(pEm)->Unk21 = 150;
                } else if (!(Rnd() & 0x1F)) {
                    Snd_se_enem(14, pEm);
                    EM10_W(pEm)->Unk21 = 150;
                }
            }
        }
        if ((LONG)pEm->Unk154 < 3000)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Dir * 2);
        Spd = pEm->Spd_x;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pEm->Spd_x += pEm->Spd_x * 3 / 10 + 3;
        pEm->Spd_x -= ((pEm->Spd_x - Spd) * pEm->UnkCA) >> 5;
        Add_speedXZ(pEm, 0);
        if (--pEm->UnkDA == 0) {
            pEm->UnkDA = (Rnd() >> 3) + 100;
            if (!(Rnd() & 3) && (LONG)pEm->Unk154 < 5000)
                EM_RTN(pEm) = 0xB01;
        }
        if ((LONG)pEm->Unk154 < 4000 && !Goto00_ck(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x400) && !(G.Pl_damage_cnt & 0x80)) {
            EM_RTN(pEm) = 0x201;
            if (EM10_W(pEm)->Flg & 0x4000)
                EM_RTN(pEm) = 0xB01;
        }
        break;
    }
}

/* RE2 Em10_mv02_br */
void func_80104AC8(ENEMY_WORK *pEm)
{
    SHORT Dir0;
    SHORT Dir1;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    Dir1 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);
    if (!EM10_W(pEm)->u2C.w.Unk2F) {
        if ((pEm->Unk158 & 0xC0000000) && (pEm->Unk158 & 0x3FFFFFFF) < 2000 && !Goto01_ck(pEm, pEm->Unk130, 0x100)) {
            EM_RTN(pEm) = 0xE01;
            return;
        }
        if (!EM10_W(pEm)->u2C.w.Unk2F && !Dir0 && LL < 2000 && pEm->Pos_y != pPl->Pos_y && !pPl->Damage_cnt
            && !(pEm->Status_flg & 0x800))
            EM_RTN(pEm) = 0xE01;
    }
    if (!Dir0 && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->UnkAE ^ pPl->UnkAE) & 0x40)) {
        EM_RTN(pEm) = 0x301;
        if (pPl->Be_flg & 0x10000000)
            EM_RTN(pEm) = 0x401;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
    if (Dir1 && LL < 2000 && !pPl->Damage_cnt && !EM10_W(pEm)->Unk26)
        EM_RTN(pEm) = 0xC01;
}

/* RE2 Em10_mv02: walking towards the player */
void func_80104CBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = EM10_W(pEm)->Unk27 + 0x1F0000;
        pEm->Timer0 = (Rnd() & 0x7F) + 300;
        if (!EM10_W(pEm)->Unk21) {
            if (!(Rnd() & 1))
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Move_cnt = Rnd() & 0x1F;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x = 0;
        pEm->UnkDA = (Rnd() & 0x1F) + 45;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = (Rnd() & 0xF) + 8;
    case 1:
        if (!pEm->UnkE2)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->UnkE3);
        else {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x10);
            pEm->UnkE2--;
        }
        if (pEm->UnkAE & 0x30) {
            if (pEm->UnkDA-- == 0) {
                pEm->UnkDA = (Rnd() & 0x1F) + 45;
                pEm->UnkE2 = (Rnd() & 0x1F) + 45;
            }
        }
        if (!pEm->Move_cnt && (Rnd() & 7) == 1)
            pEm->UnkE2 = (Rnd() & 0x1F) + 15;
        Spd = pEm->Spd_x;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pEm->Spd_x += pEm->Spd_x * 3 / 10 + 3;
        pEm->Spd_x -= ((pEm->Spd_x - Spd) * pEm->UnkCA) >> 5;
        Add_speedXZ(pEm, 0);
        if (pEm->Timer0-- == 0 || (pPl->Damage_cnt && (LONG)pEm->Unk154 < 3000)) {
            EM_RTN(pEm) = 0x101;
            if (!pPl->Damage_cnt && (LONG)pEm->Unk154 > 4000 && (Rnd() & 7) == 2)
                EM_RTN(pEm) = 0xB01;
        }
        break;
    }
}

void func_80104FC0(void) {
}

/* RE2 Em10_mv03: grabbing and biting the player */
void func_80104FC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Type_mot[4][2] = { { 12, 12 }, { 15, 15 }, { 12, 12 }, { 15, 15 } };
    SVECTOR Sv;
    MATRIX M;                          /* unused (frame size) */
    UCHAR Atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } };
    VECTOR V;                          /* unused (frame size) */
    ENEMY_WORK *pT;
    PARTS_W *pP;
    PARTS_W *pW;
    ENEMY_WORK *pPl;
    ULONG Flg;
    ULONG Life_ck;
    ULONG Up;
    SHORT Old_x;
    SHORT Old_z;

    pPl = &Pl_work;
    Flg = pEm->Type & 1;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 0xF0000;
        pEm->Routine_2 = 1;
        Goto00(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, (ULONG)pKan, (ULONG)pSeq, 0);
        func_8002646C(pEm, 200, 20, pEm->Col_unk39);
        EM_RTN(pPl) = (Cdir_ck_l(pEm, pPl) << 8) | 5;
        pPl->pSub1_kan_t_ptr = pEm->pSub1_kan_t_ptr;
        pPl->pSub1_seq_t_ptr = pEm->pSub1_seq_t_ptr;
        pPl->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x800;
        pPl->Be_flg |= 0x800;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        Pad_vib_fade_set(20, 0, 0xFA, 0);
        pEm->Damage_cnt &= 0x7F;
        if (Flg & 1) {
            pPl->Routine_1 += 2;
            pEm->Damage_cnt |= 0x80;
        }
    case 1:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 1;
        pEm->Routine_2 = 3;
        pEm->Timer0 = 148;
        if (G.Unk75EA & 1)
            pEm->Timer0 = 74;
    case 3:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        Joint_move_r(pEm, pKan, pSeq, 0x100);
        pEm->Timer0 = pEm->Timer0 - 2 - Gacha_ch() * 5;
        if (pEm->Timer0 < 0) {
            pEm->Routine_2 = 4;
            pPl->Routine_2 = 4;
        }
        if (pEm->Move_cnt == Atck[Flg][0]) {
            Pad_vib_set1(6, 0xFA, 0);
            Pad_vib_set0(10, 0);
            Pad_vib_set1(4, 0xFA, 0xF);
            Pad_vib_set0(4, 0xD);
            Snd_se_enem((Rnd() & 1) | 4, pEm);
            Life_ck = Pl_life_down(Atck[Flg][1], Flg);
            if (Life_ck & 1) {
                pEm->Routine_2 = 4;
                pPl->Routine_2 = 4;
                pPl->UnkE2 = 1;
            }
            if (Life_ck & 2) {
                EM_RTN(pEm) = 0x601;
                EM_RTN(&Pl_work) = (Cdir_ck_l(pEm, pPl) << 8) | 6;
            }
        }
        break;
    case 4:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 0x70002;
        pEm->Routine_2 = 5;
        pEm->Timer0 = 0;
        pEm->UnkE2 = 0;
        Pad_vib_set1(4, 0xFA, 10);
    case 5:
        Old_x = pEm->Pos_x;
        Old_z = pEm->Pos_z;
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Pos_x -= ((pEm->Pos_x - Old_x) * pEm->UnkCA) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - Old_z) * pEm->UnkCA) >> 3;
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        if (Flg & 1) {
            if (pPl->Id >= 8) {
                if (pEm->Move_cnt == 0x16) {
                    pW = &pEm->pSin_parts_ptr[1];
                    pEm->Unk120 |= 1;
                    pW->UnkA2 |= 4;
                    PW_FREEL(pW)[0] = 0x40;
                    PW_FREEL(pW)[1] = 0x202030;
                    pW = pEm->pSin_parts_ptr;
                    Sv.vx = 0;
                    Sv.vy = -600;
                    Sv.vz = 0;
                    func_8001B484(8, (USHORT)pEm->Cdir_y | 0x10000000, &pW->Workm, &Sv, 0);
                    Snd_se_enem(2, pEm);
                    Pad_vib_set1(4, 0xFA, 0);
                }
            } else {
                if (pEm->Move_cnt == 0x1C) {
                    pW = &pEm->pSin_parts_ptr[1];
                    pEm->Unk120 |= 1;
                    pW->UnkA2 |= 4;
                    PW_FREEL(pW)[0] = 0x40;
                    PW_FREEL(pW)[1] = 0x202030;
                    Snd_se_enem(10, pEm);
                    Pad_vib_set1(4, 0xFA, 0);
                }
            }
            if (pEm->Move_cnt >= 0x17 && pEm->Move_cnt < 0x2A && (pEm->Move_cnt & 1)) {
                pP = pEm->pSin_parts_ptr;
                Sv.vx = 0;
                Sv.vy = 100;
                Sv.vz = 0;
                func_8001B484(0, (USHORT)(pEm->Cdir_y + (Rnd() << 2) - 0x200) | 0x10000000, &pP[1].Workm, &Sv, 0);
            }
        }
        if (!(Flg & 1) && pEm->Move_cnt == 7)
            pEm->Routine_2 = 7;
        break;
    case 6:
        EM_RTN(pEm) = 7;
        pEm->Life = -1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Damage_cnt |= 0x80;
        break;
    case 7:
        func_80018EC8(pEm, pKan, pSeq, 0);
        func_80018EC8(pEm, pKan, pSeq, 0);
        pEm->Routine_2 = 8;
    case 8:
        func_80018EC8(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        pEm->Spd_x -= 10;
        Add_speedXZ(pEm, 0);
        Up = 0;
        if (pEm->Move_cnt == 0x16 && !(pEm->Status_flg & 0x400))
            Up = (Rnd() % 4) != 0;
        if (Up) {
            EM_RTN(pEm) = 0x501;
            EM10_W(pEm)->Unk0B = (Rnd() & 0xF) + 0x10;
            EM10_W(pEm)->Flg &= 0xFFF7;
            pEm->UnkE2 = 1;
            EM10_W(pEm)->Flg &= 0xFFFF;
            EM10_W(pEm)->Flg |= 4;
            EM_MOVE(pEm) = pEm->UnkE2 * 0xA00 + 0xF0A03;
            pEm->Routine_2 = 1;
            EM10_W(pEm)->Flg &= 0xFFFF;
            EM10_W(pEm)->Flg |= 0x202;
            pEm->Damage_cnt |= 0x80;
            EM10_W(pEm)->Unk0A = 0;
            pEm->UnkE3 = 0;
            pEm->Type |= 0x2000;
            if (Rnd() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            pEm->Timer0 = 1;
            if (!(G.Unk75EA & 1)) {
                EM10_W(pEm)->Unk18 = 150;
                G.Unk75EA |= 1;
            }
            break;
        }
        if (pEm->Move_cnt >= 5 && pEm->Move_cnt < 25 && pEm->Col_unk3 != 0xFF) {
            pT = G.pEnemy[pEm->Col_unk3];
            if ((pT->Id & 0x70) == 0x10 && pT->Life >= 0 && !(pT->Type & 1) && !pT->Damage_cnt
                && !(EM10_W(pT)->Flg & 8) && !(pT->Type & 0x2000)) {
                EM_RTN(pT) = 0x901;
                pT->UnkE3 = Cdir_ck(pEm, pT);
                Snd_se_enem(6, pEm);
            }
        }
        break;
    case 9:
        EM_RTN(pEm) = 0x101;
        pEm->Be_flg |= 0x80000;
        if (!(G.Unk75EA & 1)) {
            EM10_W(pEm)->Unk18 = 150;
            G.Unk75EA |= 1;
        }
        break;
    }
}

/* RE2 Em10_mv04_br (crawling) */
void func_80105884(ENEMY_WORK *pEm)
{
    ULONG LL;
    ENEMY_WORK *pPl;
    SHORT Dir;
    LONG Y;
    LONG Hit;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x100);
    Y = pPl->Pos_y;
    Hit = Y - 400 < pEm->pSin_parts_ptr[1].Workm.t[1] && pEm->pSin_parts_ptr[1].Workm.t[1] < Y + 200;
    if (!Dir && LL < 1300 && !pPl->Damage_cnt && (pEm->nFloor == pPl->nFloor || Hit)
        && !((pEm->UnkAE ^ pPl->UnkAE) & 0x40)) {
        EM_RTN(pEm) = 0x101;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
    if (pEm->UnkAE & 0x40)
        func_8002646C(pEm, 400, 40, pEm->Col_unk39);
    else
        pEm->Be_flg |= 0x80000;
}

/* RE2 Em10_mv04: crawling */
void func_801059B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF0006;
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd() & 7) + 7;
        pEm->Move_cnt = Rnd() & 0xF;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->UnkDA = 0;
    case 1:
        if (pEm->Spd_x > 20) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
            if (pEm->Timer0-- == 0) {
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x18);
                pEm->Timer0 = (Rnd() & 7) + 7;
            }
        }
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move_r(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkAE & 0x30) {
            if (++pEm->UnkDA > 150) {
                pEm->Routine_2 = 2;
                pEm->Timer0 = (Rnd() & 7) + 7;
            }
        }
        break;
    case 2:
        if (pEm->Spd_x > 20) {
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x18);
            if (pEm->Timer0-- == 0) {
                Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x18);
                pEm->Timer0 = (Rnd() & 7) + 7;
            }
        }
        Spd_get(pEm, pKan, pSeq, 0);
        Joint_move_r(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkDA-- == 0) {
            pEm->Routine_2 = 1;
            pEm->Timer0 = (Rnd() & 7) + 7;
        }
        break;
    }
}

void func_80105C10(void) {
}

/* RE2 Em10_mv05 slot: grabbing the player and letting go */
void func_80105C18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Type_mot[4][2] = { { 12, 12 }, { 15, 15 }, { 12, 12 }, { 15, 15 } }; /* as Em10_mv03 */
    SVECTOR Sv;                        /* unused */
    MATRIX M;                          /* unused */
    UCHAR Atck[4][2] = { { 16, 20 }, { 1, 5 }, { 16, 30 }, { 1, 10 } }; /* unused */
    ENEMY_WORK *pPl;
    ULONG Flg;

    pPl = (ENEMY_WORK *)&G.Pl_be_flg;
    Flg = pEm->Type & 1;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 0xF0000;
        pEm->Routine_2 = 1;
        Goto00(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x800);
        pPl->pEnemy_ptr = pEm;
        Set_base_pos(pPl, (ULONG)pKan, (ULONG)pSeq, 0);
        pPl->Routine_0 = 1;
        pPl->Routine_1 = 13;
        pPl->Routine_2 = 0;
        pPl->Damage_cnt |= 0x80;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Damage_cnt &= 0x7F;
    case 1:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_MOVE(pEm) = Type_mot[Flg][0] + 1;
        pEm->Routine_2 = 3;
        pEm->Timer0 = 13;
    case 3:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        Joint_move_r(pEm, pKan, pSeq, 0x100);
        if (pEm->Timer0-- == 0)
            EM_RTN(pEm) = 0x101;
        break;
    }
}

void func_80105E64(void) {
}

/* RE2 Em10_mv06: falling down and getting up again */
void func_80105E6C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[3][2] = { { 1, 3 }, { 24, 23 }, { 9, 10 } };
    SVECTOR Sv;
    USHORT Timer[16] = { 100, 30, 200, 160, 10, 40, 150, 50, 150, 60, 35, 110, 30, 180, 50, 300 };
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd() & 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[0][pEm->UnkE2];
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Flg |= 0x200;
        EM10_W(pEm)->Flg |= 2;
        pEm->Damage_cnt |= 0x80;
        EM10_W(pEm)->Unk0A = 0;
        pEm->UnkE3 = 0;
        pEm->Type |= 0x2000;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Timer0 = 0;
    case 1:
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        if (!pEm->UnkCA || pEm->Timer0) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        pEm->Routine_2 = 3;
        EM_MOVE(pEm) = Mot[1][pEm->UnkE2] + 0xF0000;
        pEm->UnkDA = Timer[(Rnd() >> (Rnd() & 3)) & 0xF] + (Rnd() & 0xF);
        pEm->Damage_cnt &= 0x7F;
        pEm->Spd_x = Rnd() & 3;
        pEm->Col_w0 = pEm->Col_d0 = 100;
    case 3:
        if (!pEm->Spd_x)
            pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        if (pEm->UnkDA-- == 0)
            pEm->Routine_2 = 6;
        break;
    case 4:
        pEm->Timer0 = (Rnd() & 0xF) + 3;
        pEm->Routine_2 = 5;
    case 5:
        if (pEm->Timer0-- == 0)
            pEm->Routine_2 = 3;
        if (pEm->UnkDA-- == 0)
            pEm->Routine_2 = 6;
        break;
    case 6:
        EM_MOVE(pEm) = Mot[2][pEm->UnkE2] + 0xF0000;
        pEm->Routine_2 = 7;
        EM10_W(pEm)->Flg |= 0x10;
        EM10_W(pEm)->Unk1C = 1;
        if (EM10_W(pEm)->Flg & 4)
            EM10_W(pEm)->Unk1C = 0;
        if (pEm->UnkE3 == 1 && pEm->Unk12C != 0x1A) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            func_8001B484(0, (USHORT)pEm->Cdir_y | 0x1F400000, &pP->Workm, &Sv, 0);
        }
        if (!EM10_W(pEm)->Unk21 && !(Rnd() & 3)) {
            Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        EM10_W(pEm)->Flg &= 0xFDFF;
        pEm->Unk132 |= 0x8000;
    case 7:
        if (pEm->Move_cnt < 0x37)
            func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        if (Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            EM10_W(pEm)->Flg &= 0xFFEF;
            pEm->Routine_2 = 8;
            EM10_W(pEm)->Flg &= 0xFFFD;
        }
        pEm->Col_d0 += 10;
        pEm->Col_w0 += 10;
        if (pEm->Col_d0 > 500)
            pEm->Col_w0 = pEm->Col_d0 = 500;
        if (pEm->Move_cnt == 0x37) {
            pEm->Unk1C2 = 30;
            pEm->Unk132 &= 0x7FFF;
            pEm->Be_flg |= 0x80000;
        }
        break;
    case 8:
        EM_RTN(pEm) = 0x101;
        pEm->Col_w0 = pEm->Col_d0 = 500;
        pEm->Type &= 0xDFFF;
        EM10_W(pEm)->Flg &= 0xFFED;
        break;
    }
}

void func_801063CC(void) {
}

/* RE2 Em10_mv07 slot: lying down, getting up when the player comes near */
void func_801063D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Col_w0 = pEm->Col_d0 = 100;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 1;
    case 1:
        if (!(pEm->Type & 0x4000))
            pEm->Routine_2 = 2;
        break;
    case 2:
        if (!pEm->Add_dir) {
            EM_MOVE(pEm) = ((Rnd() & 0xF) << 8) + 0xF0009;
            if (EM10_W(pEm)->Flg & 4)
                EM_MOVE(pEm) = ((Rnd() & 0xF) << 8) + 0xF000A;
        } else {
            EM_MOVE(pEm) = (pEm->Add_dir << 8) + 0xF0009;
            if (EM10_W(pEm)->Flg & 4)
                EM_MOVE(pEm) = (pEm->Add_dir << 8) + 0xF000A;
        }
        pEm->Routine_2 = 3;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(12, pEm);
            else if (Rnd() & 1)
                Snd_se_enem(14, pEm);
            else
                Snd_se_enem(13, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        EM10_W(pEm)->Unk1C = 1;
        EM10_W(pEm)->Flg |= 0x10;
        pEm->Damage_cnt &= 0x7F;
        if (EM10_W(pEm)->Flg & 4)
            EM10_W(pEm)->Unk1C = 0;
        pEm->Unk132 |= 0x8000;
    case 3:
        if (Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 4;
            EM10_W(pEm)->Flg &= 0xFFEF;
        }
        pEm->Col_d0 += 10;
        pEm->Col_w0 += 10;
        if (pEm->Col_d0 > 500)
            pEm->Col_w0 = pEm->Col_d0 = 500;
        if (pEm->Move_cnt == 0x37)
            pEm->Unk132 &= 0x7FFF;
        break;
    case 4:
        EM_RTN(pEm) = 0x101;
        pEm->Col_w0 = pEm->Col_d0 = 500;
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Flg &= 0xFFEF;
        pEm->Unk120 &= 0xFE;
        break;
    }
}

void func_80106654(void) {
}

/* RE2 Em10_mv08 slot: eating the dead player */
void func_8010665C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF0019;
        pEm->Routine_2 = 1;
        Set_base_pos(&Pl_work, (ULONG)pKan, (ULONG)pSeq, 0);
        if (Rnd() & 1)
            Snd_se_enem(13, pEm);
        else
            Snd_se_enem(14, pEm);
        pEm->Damage_cnt |= 0x80;
    case 1:
        Null_pos_set2(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_RTN(pEm) = 0x801;
        pEm->Type |= 0x4000;
        break;
    }
}

/* RE2 Em10_mv09_br */
void func_80106770(ENEMY_WORK *pEm)
{
    SHORT Dir;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    if (!Dir && LL < 1300 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->UnkAE ^ pPl->UnkAE) & 0x40)) {
        EM_RTN(pEm) = 0x101;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
}

void func_80106844(void) {
}

void func_8010684C(void) {
}

/* RE2 Em10_mv0a: eating a corpse */
void func_80106854(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[8] = { 19, 20, 21, 19, 20, 21, 19, 20 };
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = Mot[Rnd() & 7] + 0x70000;
        pEm->Damage_cnt |= 0x80;
    case 1:
        if (Joint_move_r(pEm, pKan, pSeq, 0x200))
            EM_MOVE(pEm) = Mot[Rnd() & 7] + 0x300;
        if (!pEm->UnkCA) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        if (*pEm->pNow_seq & 0x200) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 200;
            Sv.vy = 0;
            Sv.vz = 0;
            func_8001B484(0, (USHORT)(Rnd() << 4) | 0x13D00000, &pP[1].Workm, &Sv, 0);
            Snd_se_enem((Rnd() & 1) | 4, pEm);
        }
        if (!(pEm->Type & 0x4000))
            pEm->Routine_2 = 3;
        break;
    case 2:
        EM_MOVE(pEm) = Mot[Rnd() & 7] + 0x300;
        pEm->Routine_2 = 1;
        break;
    case 3:
        pEm->Routine_2 = 4;
        EM_MOVE(pEm) = 0xF0016;
        EM10_W(pEm)->Unk1C = 2;
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Flg |= 0x10;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(14, pEm);
            else if (Rnd() & 1)
                Snd_se_enem(13, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
    case 4:
        if (Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            pEm->Routine_2 = 5;
            EM10_W(pEm)->Flg &= 0xFFEF;
        }
        break;
    case 5:
        EM_RTN(pEm) = 0x101;
        pEm->Damage_cnt &= 0x7F;
        EM10_W(pEm)->Flg &= 0xFFEF;
        break;
    case 6:
        pEm->Routine_2 = 7;
        EM_MOVE(pEm) = ((Rnd() & 7) << 8) + 0xF0016;
    case 7:
        Goto00(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x10);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x10100);
        break;
    case 8:
        pEm->Routine_2 = 0;
        break;
    }
}

void func_80106B7C(void) {
}

/* RE2 Em10_mv0b: knocked down by a push, then getting up (or falling as in Em10_mv06) */
void func_80106B84(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Down_mot[2][2] = { { 4, 4 }, { 5, 14 } };
    UCHAR Mot[3][2] = { { 1, 3 }, { 24, 23 }, { 9, 10 } };  /* as Em10_mv06 */

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd() & 1;
        EM_MOVE(pEm) = ((Rnd() & 3) << 8) + Down_mot[pEm->UnkE3][pEm->UnkE2];
        if (pEm->UnkE3 && pEm->UnkE2)
            pEm->Move_cnt = (Rnd() & 3) + 20;
        pEm->Routine_2 = 1;
        pEm->Spd_x = 400;
        pEm->Spd_z = 0;
        pEm->UnkDA = 0;
        pEm->Timer0 = pEm->UnkE3 * 2048;
        EM10_W(pEm)->Flg &= 0xFFEF;
        EM10_W(pEm)->Flg |= 8;
        if (!EM10_W(pEm)->Unk21 && (Rnd() & 1)) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
    case 1:
        Add_speedXZ(pEm, pEm->Timer0);
        if ((pEm->Spd_x -= 30) < 0) {
            pEm->Spd_x = 0;
            pEm->UnkDA++;
        }
        if (pEm->UnkDA == 1 && (Rnd() >> (Rnd() & 3)) & 7) {
            EM_RTN(pEm) = 0x501;
            EM10_W(pEm)->Unk0B = (Rnd() & 0xF) + 0x10;
            EM10_W(pEm)->Flg &= 0xFFF7;
            pEm->UnkE2 = pEm->UnkE3;
            EM10_W(pEm)->Flg &= 0xFFFB;
            if (pEm->UnkE2)
                EM10_W(pEm)->Flg |= 4;
            EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[0][pEm->UnkE2];
            pEm->Routine_2 = 1;
            EM10_W(pEm)->Flg |= 0x200;
            EM10_W(pEm)->Flg |= 2;
            pEm->Damage_cnt |= 0x80;
            EM10_W(pEm)->Unk0A = 0;
            pEm->UnkE3 = 0;
            pEm->Type |= 0x2000;
            if (!EM10_W(pEm)->Unk21) {
                if (Rnd() & 1)
                    Snd_se_enem(12, pEm);
                else
                    Snd_se_enem(15, pEm);
                EM10_W(pEm)->Unk21 = 150;
            }
            pEm->Timer0 = 0;
            break;
        }
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_RTN(pEm) = 0x101;
        EM10_W(pEm)->Flg &= 0xFFF7;
        break;
    }
}

void func_80106E90(void) {
}

void func_80106E98(void) {
}

/* RE2 Em10_mv0d_br */
void func_80106EA0(ENEMY_WORK *pEm)
{
    SHORT Dir0;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir0 = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x514);
    if (!EM10_W(pEm)->u2C.w.Unk2F) {
        if ((pEm->Unk158 & 0xC0000000) && (pEm->Unk158 & 0x3FFFFFFF) < 1500 && !Goto01_ck(pEm, pEm->Unk130, 0x100)) {
            EM_RTN(pEm) = 0xE01;
            return;
        }
        if (!EM10_W(pEm)->u2C.w.Unk2F && !Dir0 && LL < 1500 && pEm->Pos_y != pPl->Pos_y && !pPl->Damage_cnt
            && !(pEm->Status_flg & 0x800))
            EM_RTN(pEm) = 0xE01;
    }
    if (!Dir0 && LL < 1100 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->UnkAE ^ pPl->UnkAE) & 0x40)) {
        EM_RTN(pEm) = 0x301;
        if (pPl->Be_flg & 0x10000000)
            EM_RTN(pEm) = 0x401;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
}

/* RE2 Em10_mv0d: staggering about */
void func_80107050(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG LL;
    SHORT Dir;
    SHORT Spd;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = (Rnd() & 1) + 0xF0006;
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd() & 0x3F) + 300;
        if (!EM10_W(pEm)->Unk21) {
            if (!(Rnd() & 1))
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Move_cnt = Rnd() & 0xF;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x = 0;
        pEm->UnkE3 = Rnd() & 0xF;
        pEm->UnkDA = Em10_mv0d_rnd[pEm->UnkE3] & 0x7FFF;
    case 1:
        if (pEm->UnkDA-- == 0) {
            if (++pEm->UnkE3 >= 16)
                pEm->UnkE3 = 0;
            pEm->UnkDA = Em10_mv0d_rnd[pEm->UnkE3] & 0x7FFF;
            pEm->Cdir_y -= 0x10 - ((Rnd() & 1) << 5);
        }
        if (Em10_mv0d_rnd[pEm->UnkE3] & 0x8000)
            Dir = -0x20;
        else
            Dir = 0x20;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Dir);
        if (LL < 3000)
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x30);
        Spd = pEm->Spd_x;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Spd) * pEm->UnkCA) >> 4;
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
        if (pEm->Timer0-- == 0 || (pPl->Damage_cnt && (LONG)pEm->Unk154 < 3500))
            EM_RTN(pEm) = 0x101;
        break;
    }
}

/* RE2 Em10_mv0e_br */
void func_801072F0(ENEMY_WORK *pEm)
{
    SHORT Dir;
    ULONG LL;
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    Dir = Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x280);
    if (!Dir && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor && !((pEm->UnkAE ^ pPl->UnkAE) & 0x40)) {
        EM_RTN(pEm) = 0x301;
        if (Pl_work.Be_flg & 0x10000000)
            EM_RTN(pEm) = 0x401;
        pEm->Damage_cnt |= 0x80;
        pPl->Unk12C = 0;
        pPl->Damage_cnt |= 0x80;
    }
}

/* RE2 Em10_mv0e: lunging forward */
void func_801073D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF001A;
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Unk26 = (Rnd() & 0xF) + 70;
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(13, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Spd_x = 30;
        pEm->Spd_z = 0;
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0xD4);
        if (Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            EM_RTN(pEm) = 0x201;
            if (EM10_W(pEm)->Flg & 0x4000)
                EM_RTN(pEm) = 0xB01;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_801074E4(void) {
}

/* RE2 Em10_mv0f slot: standing still while the scenario holds it (Type 0x4000) */
void func_801074EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF0006;
        pEm->Routine_2 = 1;
        pEm->Timer0 = (Rnd() & 7) + 7;
    case 1:
        Joint_move_r(pEm, pKan, pSeq, 0x100);
        if (!(pEm->Type & 0x4000))
            EM_RTN(pEm) = 1;
        break;
    }
}

void func_80107594(void) {
}

/* RE2 Em10_mv10: vomiting */
void func_8010759C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF0012;
        pEm->Routine_2 = 1;
    case 1:
        if (pEm->Move_cnt == 20) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 300;
            Sv.vy = 0;
            Sv.vz = 0;
            func_8001B484(0x18, (USHORT)pEm->Cdir_y | 0x10000000, &pP[1].Workm, &Sv, 0);
            Snd_se_enem(7, pEm);
        }
        func_80018EC8(pEm, pKan, pSeq, 0);
        if (Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            EM_RTN(pEm) = 0x201;
            EM10_W(pEm)->u2C.w.Unk2F = 30;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_801076A0(void) {
}

/* RE2 Em10_mv11: a scripted motion with effects (Routine_3 counts the frames) */
void func_801076A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    LONG Parts_num;
    LONG *pIdx;
    EM10_ESP *pE;
    MATRIX *pM;
    PARTS_W *pP;

    Parts_num = 15;
    if (pEm->Id >= 0x18)
        Parts_num = 11;
    pIdx = &EM10_W(pEm)->u2C.Esp_idx;
    if (!pEm->Routine_3)
        EM10_W(pEm)->u2C.Esp_idx = 0;
    if (Em10_mv11_esp[EM10_W(pEm)->u2C.Esp_idx].Cnt != 0xFF) {
        while (pEm->Routine_3 == Em10_mv11_esp[*pIdx].Cnt) {
            pE = &Em10_mv11_esp[*pIdx];
            if (pE->Parts_no == 0xEE) {
                pEm->Routine_2++;
                (*pIdx)++;
                continue;
            }
            if (pE->Parts_no == 0xFF) {
                pM = &GsIDMATRIX;
                Sv.vx = pE->Ofs_x + pEm->Pos_x + ((signed char)Rnd() >> 2);
                Sv.vy = pE->Ofs_y + pEm->Pos_y;
                Sv.vz = pE->Ofs_z + pEm->Pos_z + ((signed char)Rnd() >> 2);
            } else {
                pP = &pEm->pSin_parts_ptr[pE->Parts_no];
                pM = &pP->Workm;
                Sv.vx = pE->Ofs_x;
                Sv.vy = pE->Ofs_y;
                Sv.vz = pE->Ofs_z;
            }
            func_8001B484((((UCHAR)pEm->Unk14C & 0xF) << 24) | (pE->Id_hi << 8) | pE->Id_lo,
                          ((pE->Esp_no + (Rnd() >> 1)) << 16) | (USHORT)pEm->Cdir_y, pM, &Sv, 0);
            (*pIdx)++;
        }
        switch (++pEm->Routine_3) {
        case 0:
            pEm->Be_flg |= 0x20;
            break;
        case 0x4B:
            pEm->Be_flg &= ~0x20;
            break;
        }
        if (Em10_mv11_esp[*pIdx].Cnt == 0xFF)
            *pIdx = 0;
    }
    switch (pEm->Routine_2) {
    case 1:
        EM_MOVE(pEm) = pEm->Add_dir;
        pEm->Routine_2++;
        EM10_W(pEm)->Unk1C = 0;
        EM10_W(pEm)->Flg |= 0x10;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[0];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[2];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[3];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 1;
        pP = &pEm->pSin_parts_ptr[6];
        pP->UnkA2 |= 1;
        if (Parts_num == 15) {
            pP = &pEm->pSin_parts_ptr[4];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[7];
            pP->UnkA2 |= 1;
        }
    case 2:
        if (pEm->Move_cnt == 0x46) {
            pP = &pEm->pSin_parts_ptr[8];
            pP->UnkA2 |= 1;
        }
        if (pEm->Move_cnt == 0x4B) {
            pP = &pEm->pSin_parts_ptr[9];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[12];
            pP->UnkA2 |= 1;
            pP = &pEm->pSin_parts_ptr[13];
            pP->UnkA2 |= 1;
            if (Parts_num == 15) {
                pP = &pEm->pSin_parts_ptr[11];
                pP->UnkA2 |= 1;
                pP = &pEm->pSin_parts_ptr[14];
                pP->UnkA2 |= 1;
            }
        }
        func_80018EC8(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0);
        if (Joint_move_r(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x80)) {
            EM_RTN(pEm) = 0x101;
            if (pEm->Unk14A & 2)
                EM_RTN(pEm) = 0x201;
            if (pEm->Unk14A & 4)
                EM_RTN(pEm) = 0xB01;
            if (pEm->Unk14A & 8) {
                EM_RTN(pEm) = 0x20701;
                pEm->Add_dir = 0;
            }
            if (pEm->Type & 1)
                EM_RTN(pEm) = 1;
            EM10_W(pEm)->Flg &= 0xFFEF;
            pEm->Damage_cnt &= 0x7F;
            pP = pEm->pSin_parts_ptr;
            do {
                pP->UnkA2 &= ~0x400;
                pP++;
            } while (--Parts_num);
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_dm00 (shared with em16 func_801076B8) */
void func_80107C10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    MATRIX M, M1;
    VECTOR V;                          /* unused (frame size) */
    signed char Tbl[8] = { 24, 0, -24, 0, -12, 0, 12, 0 };
    ENEMY_WORK *pPl;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pT;
    ULONG LL;
    UCHAR Cnt;

    pPl = &Pl_work;
    LL = pEm->Unk154;
    if (EM10_W(pEm)->Unk0A == 1) {
        func_801082B0(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        if (pEm->Move_no >= 8)
            EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0x1F0000;
        if (!EM10_W(pEm)->Unk21) {
            if (!(Rnd() & 1))
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(14, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->UnkE3 = 12;
        pEm->UnkE2 = Tbl[Rnd() & 7];
        Cnt = pEm->Move_cnt;
        pEm->Move_cnt = 0;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Move_cnt = Cnt;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        pEm->Routine_2 = 1;
        pEm->UnkDA = 1;
        pEm->Timer0 = 0;
        EM10_W(pEm)->Unk0B -= Em10_dm00_usonum[pEm->Unk12C - 1];
        if (pEm->Unk132 & 0x10)
            pEm->UnkDA = -1;
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        func_8010C57C(pEm);
    case 1:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -(pEm->Timer0 * pEm->UnkE2 * 8) * pEm->UnkDA;
        Sv.vz = -(pEm->Timer0 * pEm->UnkE3 * 8) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix((MATRIX *)pP->M, &M);
        pS = pEm->pSin_parts_ptr;
        pT = &pS[8];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pT->M);
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = -(pEm->Timer0 << 6) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix((MATRIX *)pT->M, &M);
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pS[9].M);
        MulMatrix2(&M1, (MATRIX *)pEm->pSin_parts_ptr[12].M);
        if (pEm->Timer0++ >= 3) {
            pEm->Routine_2 = 2;
            pEm->Timer0 = 16;
        }
        break;
    case 2:
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x80);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -(pEm->Timer0 * pEm->UnkE2) * pEm->UnkDA;
        Sv.vz = -(pEm->Timer0 * pEm->UnkE3) * pEm->UnkDA;
        RotMatrix(&Sv, &M);
        MulMatrix((MATRIX *)pP->M, &M);
        pP = &pEm->pSin_parts_ptr[8];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pP->M);
        if (pEm->Timer0-- == 0) {
            if (pEm->Move_no < 8) {
                if (pEm->Move_no < 3)
                    EM_RTN(pEm) = 0x10101;
                else if (pEm->Move_no < 6) {
                    EM_RTN(pEm) = 0x10201;
                    if (EM10_W(pEm)->Flg & 0x4000)
                        EM_RTN(pEm) = 0xB01;
                } else
                    EM_RTN(pEm) = 0x10B01;
                pEm->UnkDA = (Rnd() >> 3) + 100;
                pEm->UnkE2 = (Rnd() & 0x1F) + 30;
                pEm->UnkE3 = (Rnd() & 0xF) * 2;
                pEm->Timer0 = Rnd();
                if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x44C) && EM10_W(pEm)->Unk26 < 60)
                    EM_RTN(pEm) = 0xC01;
                if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400) && EM10_W(pEm)->Unk26 < 60 && LL < 3000 && (Rnd() & 1))
                    EM_RTN(pEm) = 0xC01;
                break;
            }
            EM_RTN(pEm) = 0x101;
            if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x44C) && EM10_W(pEm)->Unk26 < 30)
                EM_RTN(pEm) = 0xC01;
            if (Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400) && EM10_W(pEm)->Unk26 < 60 && LL < 3000 && (Rnd() & 1))
                EM_RTN(pEm) = 0xC01;
        }
        if (!Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x320) && LL < 1200 && !pPl->Damage_cnt && pEm->nFloor == pPl->nFloor
            && (LONG)G.Status_flg >= 0 && !((pEm->UnkAE ^ pPl->UnkAE) & 0x40)) {
            EM_RTN(pEm) = 0x301;
            pEm->Damage_cnt |= 0x80;
            pPl->Damage_cnt |= 0x80;
            pPl->Unk12C = 0;
            if (pPl->Be_flg & 0x10000000)
                EM_RTN(pEm) = 0x401;
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

/* RE2 Em10_dm01 slot: hit, staggering back (may fall over) */
void func_801082B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 0 };
    PARTS_W *pP;
    SVECTOR Sv;                        /* unused (frame size) */
    UCHAR Mot[2] = { 1, 3 };
    SHORT Dir;
    LONG R;

    switch (pEm->Routine_2) {
    case 0:
        Dir = pEm->Unk132 & 0x10;
        EM_MOVE(pEm) = 0x30005 - (Dir = Dir != 0);
        pEm->Routine_2 = 1;
        pEm->UnkDA = 10;
        pEm->Spd_x = Spd[(pEm->Unk132 >> 4) & 1];
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x10);
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->UnkE2 = 1;
        if (pEm->Unk132 & 0x10)
            pEm->UnkE2 = -1;
        if (pEm->Unk12C == 0x1A)
            pEm->UnkE2 = 0;
        else {
            func_8010C57C(pEm);
            if ((pEm->Unk132 & 3) == 1)
                pEm->UnkE2 = 0;
        }
        pEm->UnkE3 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE3 = 5;
        if (pEm->Unk12C == 0x1A && !(Rnd() & 1)) {
            EM10_W(pEm)->Unk0B = 0;
            EM10_W(pEm)->Unk0A = 1;
        }
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (pEm->Unk12C == 0x1A && pEm->Move_cnt < 10)
            pEm->Spd_x <<= 1;
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkDA && pEm->UnkE2) {
            if (!(pEm->UnkDA & 1)) {
                pP = pEm->pSin_parts_ptr;
                if (!EM10_W(pEm)->Unk0A) {
                    func_8001B484((pEm->UnkE3 & 0xF) << 24,
                                  (USHORT)(pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4)) | 0x1F400000, &pP->Workm,
                                  (SVECTOR *)&pEm->Spd_x, 0);
                } else {
                    pEm->Spd_y -= 500;
                    func_8001B484((pEm->UnkE3 & 0xF) << 24,
                                  (USHORT)(pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4)) | 0x13E80000, &pP->Workm,
                                  (SVECTOR *)&pEm->Spd_x, 0);
                }
            }
            pEm->UnkDA--;
        }
        if (pEm->Unk12C == 0x1A && pEm->Move_cnt == 13) {
            R = Rnd() & 3;
            if (R == 1 && !(pEm->Status_flg & 0x400)) {
                EM_RTN(pEm) = 0x501;
                EM10_W(pEm)->Unk0B = (Rnd() & 0xF) + 0x10;
                EM10_W(pEm)->Flg &= 0xFFF7;
                pEm->UnkE2 = R;
                EM10_W(pEm)->Flg |= 4;
                if (pEm->Unk132 & 0x10) {
                    pEm->UnkE2 = 0;
                    EM10_W(pEm)->Flg &= 0xFFFB;
                }
                EM_MOVE(pEm) = ((pEm->UnkE2 * 5 + 10) << 8) + 0xF0000 + Mot[pEm->UnkE2];
                pEm->Routine_2 = R;
                EM10_W(pEm)->Unk0A = 0;
                EM10_W(pEm)->Flg |= 0x202;
                pEm->UnkE3 = 0;
                pEm->Damage_cnt |= 0x80;
                pEm->Type |= 0x2000;
                if (!EM10_W(pEm)->Unk21) {
                    if (Rnd() & 1)
                        Snd_se_enem(12, pEm);
                    else
                        Snd_se_enem(15, pEm);
                    EM10_W(pEm)->Unk21 = 150;
                }
                pEm->Timer0 = 0;
            }
        }
        break;
    case 2:
        EM_RTN(pEm) = 0x101;
        if (Rnd() & 1) {
            EM_RTN(pEm) = 0x201;
            if (EM10_W(pEm)->Flg & 0x4000)
                EM_RTN(pEm) = 0xB01;
        }
        if (EM10_W(pEm)->Unk0B <= 0) {
            EM10_W(pEm)->Unk0B = (Rnd() & 0x10) + 0xF;
            EM10_W(pEm)->Unk0A = 0;
        }
        if ((LONG)pEm->Unk154 < 3000 && Goto00_ck(pEm, G.Pl_pos_x, G.Pl_pos_z, 900) && (Rnd() & 1))
            EM_RTN(pEm) = 0xC01;
    }
}

/* RE2 Em10_Firedm: set on fire */
void func_8010879C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pQ;

    pEm->Status_flg |= 1;
    if ((pEm->Unk132 & 3) == 1) {
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd();
        Sv.vy = 0x1AC - Rnd();
        Sv.vz = 0x80 - Rnd();
        func_8001B484(0x305, ((Rnd() << 4) + 0x1A00) << 16, &pP[8].Workm, &Sv, 0);
        pS = pEm->pSin_parts_ptr;
        func_8001B484(5, ((Rnd() << 4) + 0x800) << 16, &pS[13].Workm, 0, 0);
        pS = pEm->pSin_parts_ptr;
        func_8001B484(5, ((Rnd() << 4) + 0x800) << 16, &pS[9].Workm, 0, 0);
    } else {
        pQ = pEm->pSin_parts_ptr;
        Sv.vx = 0x80 - Rnd();
        Sv.vy = 0x1AC - Rnd();
        Sv.vz = 0x80 - Rnd();
        func_8001B484(5, ((Rnd() << 4) + 0x1A00) << 16, &pQ[8].Workm, &Sv, 0);
        pS = pEm->pSin_parts_ptr;
        func_8001B484(5, ((Rnd() << 4) + 0x800) << 16, &pS[1].Workm, 0, 0);
        pS = pEm->pSin_parts_ptr;
        func_8001B484(5, ((Rnd() << 4) + 0x800) << 16, &pS[5].Workm, 0, 0);
        pEm->pSin_parts_ptr[1].Poly_rgb = 0x404040;
    }
    pEm->pSin_parts_ptr[8].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[0].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[5].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[2].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[12].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[9].Poly_rgb = 0x606060;
}

/* RE2 Em10_Ryudm slot: acid hit, the head melts */
void func_801089A8(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;

    pEm->Unk178[2] = 0xFFF;
    pEm->Unk17E = 0x230;
    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    pP = pEm->pSin_parts_ptr;
    pEm->Unk178[0] = 0;
    pEm->Unk178[1] = 0;
    Sv.vx = 0x80 - Rnd();
    Sv.vy = -0xAC - Rnd();
    Sv.vz = 0x80 - Rnd();
    func_8001B484(0x1C03, ((Rnd() << 3) + 0x800) << 16, &pP[8].Workm, &Sv, 0);
    if (Rnd() & 1) {
        pS = pEm->pSin_parts_ptr;
        func_8001B484(0x1C03, ((Rnd() << 3) + 0x800) << 16, &pS[9].Workm, 0, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        func_8001B484(0x1C03, ((Rnd() << 3) + 0x800) << 16, &pS[1].Workm, 0, 0);
    }
    pW = &pEm->pSin_parts_ptr[8];
    pW->UnkA2 |= 0x40;
    pW = pEm->pSin_parts_ptr;
    pW->UnkA2 |= 0x40;
}

/* RE2 Em10_Tudm slot: hit by the grenade blast */
void func_80108B00(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pS;

    pEm->Status_flg |= 1;
    if (pEm->Type & 1) {
        func_8010879C(pEm);
        return;
    }
    pP = pEm->pSin_parts_ptr;
    Sv.vx = 0x80 - Rnd();
    Sv.vy = -300;
    Sv.vz = 0x80 - Rnd();
    func_8001B484(0x205, ((Rnd() << 4) + 0x1A00) << 16, &pP[8].Workm, &Sv, 0);
    pS = pEm->pSin_parts_ptr;
    func_8001B484(0x205, ((Rnd() << 4) + 0xA00) << 16, &pS[1].Workm, 0, 0);
    Sv.vx = 0x40 - (Rnd() & 0x7F);
    Sv.vy = 0;
    Sv.vz = 0x40 - (Rnd() & 0x7F);
    if (Rnd() & 1) {
        pS = pEm->pSin_parts_ptr;
        func_8001B484(0x205, ((Rnd() << 4) + 0x1400) << 16, &pS[12].Workm, &Sv, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        func_8001B484(0x205, ((Rnd() << 4) + 0x1000) << 16, &pS[5].Workm, &Sv, 0);
    }
    pEm->pSin_parts_ptr[8].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[0].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[1].Poly_rgb = 0x404040;
    pEm->pSin_parts_ptr[5].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[2].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[12].Poly_rgb = 0x606060;
    pEm->pSin_parts_ptr[9].Poly_rgb = 0x606060;
}

/* RE2 Em10_Spdm slot: acid hit from the front, the head is pushed back */
void func_80108CBC(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    PARTS_W *pS;
    PARTS_W *pW;

    pEm->Be_flg |= 8;
    pEm->Status_flg |= 2;
    Sv.vx = 0;
    Sv.vy = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Pl_pos_x, G.Pl_pos_z) - pEm->Cdir_y + 0x100;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    Sv.vx = 4000;
    Sv.vy = 0;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, (SVECTOR *)pEm->Unk178);
    pP = pEm->pSin_parts_ptr;
    pEm->Unk17E = 0x40;
    Sv.vx = 0x80 - Rnd();
    Sv.vy = -0xAC - Rnd();
    Sv.vz = 0x80 - Rnd();
    func_8001B484(0x1C03, ((Rnd() << 3) + 0x800) << 16, &pP[8].Workm, &Sv, 0);
    if (Rnd() & 1) {
        pS = pEm->pSin_parts_ptr;
        func_8001B484(0x1803, ((Rnd() << 3) + 0x800) << 16, &pS[9].Workm, 0, 0);
    } else {
        pS = pEm->pSin_parts_ptr;
        func_8001B484(0x1803, ((Rnd() << 3) + 0x800) << 16, &pS[1].Workm, 0, 0);
    }
    pW = &pEm->pSin_parts_ptr[8];
    pW->UnkA2 |= 0x40;
    pW = pEm->pSin_parts_ptr;
    pW->UnkA2 |= 0x40;
}

/* RE2 Em10_dm02: knocked down (falls on the back, may get up again) */
void func_80108E6C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -125, 50 };
    SVECTOR Sv;                        /* unused (frame size) */
    MATRIX M, M1;
    PARTS_W *pP;
    PARTS_W *pS;
    ULONG i;
    SHORT Dir;
    LONG Tmp;

    if (EM10_W(pEm)->Unk19 == 1) {
        func_8010B6D4(pEm, pKan, pSeq);
        return;
    }
    if (EM10_W(pEm)->Unk19 == 2) {
        func_8010B278(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        if ((Rnd() & 3) && pEm->Routine_0 == 3) {
            EM10_W(pEm)->Unk19 = 1;
            func_8010B6D4(pEm, pKan, pSeq);
            return;
        }
        Dir = pEm->Unk132 & 0x10;
        EM_MOVE(pEm) = 0x30005 - (Dir = Dir != 0);
        pEm->Routine_2 = 1;
        pEm->Add_dir = 0;
        pEm->Unk14A = 0;
        pEm->Unk14C = 0;
        pEm->Timer0 = 0;
        pEm->UnkE3 = 0;
        pEm->Spd_x = Spd[(pEm->Unk132 >> 4) & 1];
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        pP = pEm->pSin_parts_ptr;
        func_8001B484(0, (USHORT)(Rnd() << 4) | 0x17700000, &pP->Workm, 0, 0);
        func_8001B484(8, ((Rnd() + 0x7D0) << 17) | (USHORT)(Rnd() << 4), &pP->Workm, 0, 0);
        i = (Rnd() & 3) + 2;
        do {
            i--;
            func_8001B484(0x209, ((Rnd() + 0xC00) << 16) | (USHORT)(Rnd() << 4), &pP->Workm, 0, 0);
        } while (i);
        if (!EM10_W(pEm)->Unk21) {
            if (Rnd() & 1)
                Snd_se_enem(12, pEm);
            else
                Snd_se_enem(15, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        Snd_se_enem(11, pEm);
        EM10_W(pEm)->Unk1A = pEm->Unk122;
        Em_dead_flg_on_cand(pEm);
        pEm->Damage_cnt |= 0x80;
        pEm->Be_flg |= 0x400;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->Routine_2 == 2) {
            pP = pEm->pSin_parts_ptr;
            RotMatrix(SUPER_VECTOR(pEm), &M);
            MulMatrix((MATRIX *)pP->M, &M);
            pS = &pEm->pSin_parts_ptr[8];
            TransposeMatrix(&M, &M1);
            MulMatrix2(&M1, (MATRIX *)pS->M);
        }
        if (pEm->Move_cnt) {
            if (!(pEm->Move_cnt & 3)) {
                pP = pEm->pSin_parts_ptr;
                func_8001B484(0, (((Rnd() << 3) + 0x1770) << 16) | (USHORT)(Rnd() << 4), &pP->Workm,
                              (SVECTOR *)&pEm->Spd_x, 0);
            }
        }
        break;
    case 2:
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Damage_cnt &= 0x7F;
        pP = pEm->pSin_parts_ptr;
        pP->Old_y = -200;
        pP->Cdir_x = 0;
        pP->Cdir_y = 0;
        pP->Cdir_z = 0x400;
        pEm->Pos_y = EM10_W(pEm)->Unk1A;
        EM10_W(pEm)->Flg |= 1;
        pEm->Life >>= 1;
        if (pEm->Routine_0 == 3)
            pEm->Life = 10;
        pEm->Type = 0x2001;
        pEm->Be_flg &= ~0x400;
        if ((pEm->Type & 0x20) || (pEm->Status_flg & 0x400)) {
            pEm->Life = -1;
            EM_RTN(pEm) = 7;
            EM10_W(pEm)->Flg &= 0xFFFB;
            return;
        }
        EM_RTN(pEm) = 1;
        EM_MOVE(pEm) = 0xF0006;
        Joint_move_r(pEm, pKan, pSeq, 0x100);
        return;
    }
    if (pEm->Routine_2 < 2) {
        pP = pEm->pSin_parts_ptr;
        if (pEm->Move_cnt < 10) {
            pEm->Add_dir -= pEm->UnkE3 * 16;
            pEm->Unk14A = 0;
            pEm->Unk14C += pEm->UnkE3 * 24;
            pEm->UnkE3++;
        }
        if ((ULONG)(pEm->Move_cnt - 11) < 19) {
            pEm->Add_dir += pEm->UnkE3 * 8;
            pEm->Unk14A -= pEm->Timer0 * 2;
            pEm->Unk14C -= pEm->UnkE3 * 2;
            pEm->UnkE3--;
            pEm->Timer0++;
        }
        if ((ULONG)(pEm->Move_cnt - 31) < 19) {
            pEm->Add_dir -= pEm->UnkE3 * 4;
            pEm->Unk14A += pEm->Timer0 * 2;
            pEm->Unk14C -= pEm->UnkE3 * 2;
            pEm->UnkE3++;
            pEm->Timer0--;
        }
        RotMatrix(SUPER_VECTOR(pEm), &M);
        MulMatrix((MATRIX *)pP->M, &M);
        pS = &pEm->pSin_parts_ptr[8];
        TransposeMatrix(&M, &M1);
        MulMatrix2(&M1, (MATRIX *)pS->M);
    }
    if (pEm->Move_cnt == 20 && pEm->Routine_2 == 1) {
        pEm->pSin_parts_ptr[8].pSuper = &pEm->Super_matrix;
        pEm->Super_matrix = pEm->pSin_parts_ptr->Workm;
        pEm->Super_matrix.t[0] = pEm->Pos_x;
        pEm->Super_matrix.t[2] = pEm->Pos_z;
        pEm->UnkDA = 10;
        pEm->UnkE2 = 2;
        pP = &pEm->pSin_parts_ptr[8];
        func_8001B484(0x300, (USHORT)(Rnd() << 4) | 0x10000000, &pP->Workm, 0, 0);
        Snd_se_enem(11, pEm);
        pP = &pEm->pSin_parts_ptr[8];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[9];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[10];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[12];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        pP = &pEm->pSin_parts_ptr[13];
        pP->Be_flg |= 0x20;
        pP->UnkA2 |= 0x1000;
        if (pEm->Parts_num == 15) {
            pP = &pEm->pSin_parts_ptr[11];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[14];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
        }
    }
    if (pEm->Move_cnt >= 20 && pEm->Routine_2 == 1) {
        pP = pEm->pSin_parts_ptr;
        RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
        MulMatrix0((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &pEm->Super_matrix);
        pEm->Super_matrix.t[1] = pP->Pos_y + EM10_W(pEm)->Unk1A;
        if (pEm->UnkE2) {
            pEm->Pos_y += pEm->UnkDA;
            pEm->UnkDA += 5;
            if (pEm->Move_cnt >= 35)
                pEm->UnkDA += 50;
        }
        if (EM10_W(pEm)->Unk1A - (200 + pP->Pos_y) < pEm->Pos_y && pEm->UnkE2) {
            Tmp = pEm->UnkDA;
            pEm->UnkDA = -(Tmp >> 3);
            pEm->Pos_y = EM10_W(pEm)->Unk1A - (300 + pP->Pos_y);
            pEm->UnkE2--;
            Snd_se_enem(6, pEm);
        }
    }
}

void func_80109758(void) {
}

void func_80109760(void) {
}

/* RE2 Em10_die slot: a leg is blown off (the zombie may go on crawling) */
void func_80109768(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 125 };      /* unused */
    SVECTOR Sv;
    PARTS_W *pP;
    ULONG Mode;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF0001;
        func_80018EC8(pEm, pKan, pSeq, 0);
        pEm->Move_cnt = Rnd() & 7;
        func_80018EC8(pEm, pKan, pSeq, 0);
        Mode = 0;
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pEm->UnkE3 = Rnd() & 1;
        if (pEm->UnkE3 && !(EM10_W(pEm)->Flg & 0x8000)) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[2] = 0;
            PW_FREE(pP)[1] = -10;
            PW_FREE(pP)[3] = 0;
            PW_FREE(pP)[6] = 0;
            pP->Poly_rgb = 0x10104F;
            pEm->pSin_parts_ptr[11].Be_flg = 0;
            pP = &pEm->pSin_parts_ptr[9];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            Snd_se_enem(2, pEm);
            if (Rnd() & 1) {
                if (pEm->Id != 0x11 && pEm->Id != 0x13) {
                    pP = &pEm->pSin_parts_ptr[12];
                    pP->UnkA2 |= 0x20;
                    pP->Be_flg |= 2;
                    PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
                    PW_FREE(pP)[4] = 0;
                    PW_FREE(pP)[2] = 0;
                    PW_FREE(pP)[1] = -10;
                    PW_FREE(pP)[3] = 0;
                    PW_FREE(pP)[6] = 0;
                    pP->Poly_rgb = 0x10104F;
                    func_8001B484(Mode << 24, 0x17700000, &pP->Workm, 0, 0);
                    func_8001B484(8, (USHORT)(Rnd() << 4) | 0x10000000, &pP->Workm, 0, 0);
                }
                pP = &pEm->pSin_parts_ptr[13];
                pP->UnkA2 |= 0x20;
                pP->Be_flg |= 2;
                PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
                PW_FREE(pP)[4] = 0;
                PW_FREE(pP)[2] = 0;
                PW_FREE(pP)[1] = -10;
                PW_FREE(pP)[3] = 0;
                PW_FREE(pP)[6] = 0;
                pP->Poly_rgb = 0x10104F;
                func_8001B484(Mode << 24, 0x17700000, &pP->Workm, 0, 0);
                pEm->pSin_parts_ptr[14].Be_flg = 0;
            } else {
                pP = &pEm->pSin_parts_ptr[12];
                PW_FREEL(pP)[0] = 0x40;
                PW_FREEL(pP)[1] = 0x202030;
                pP->UnkA2 |= 4;
                pP = &pEm->pSin_parts_ptr[13];
                PW_FREEL(pP)[0] = 0x40;
                PW_FREEL(pP)[1] = 0x202030;
                pP->UnkA2 |= 4;
            }
        } else {
            pP = &pEm->pSin_parts_ptr[9];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[10];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[12];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
            pP = &pEm->pSin_parts_ptr[13];
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 4;
        }
        pP = &pEm->pSin_parts_ptr[10];
        Sv.vx = 0;
        Sv.vy = 0;
        Sv.vz = 0;
        func_8001B484(Mode << 24, 0x13880000, &pP->Workm, &Sv, 0);
        Sv.vy = 400;
        func_8001B484(Mode << 24, (USHORT)(Rnd() << 2) | 0x17700000, &pP->Workm, &Sv, 0);
        Sv.vy = 800;
        func_8001B484(Mode << 24, (USHORT)(Rnd() << 4) | 0xFA00000, &pP->Workm, &Sv, 0);
        func_8001B484((Mode << 24) | 8, (USHORT)(Rnd() << 4) | 0x10000000, &pP->Workm, 0, 0);
        if (!EM10_W(pEm)->Unk21) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        Em_dead_flg_on_cand(pEm);
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
    case 1:
        func_80018EC8(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    case 2:
        if (pEm->Routine_0 == 3) {
            if ((Rnd() & 3) || !pEm->UnkE3)
                pEm->Life = 10;
            else {
                EM10_W(pEm)->Flg &= 0xFFFB;
                EM_RTN(pEm) = 7;
                break;
            }
        }
        if ((pEm->Type & 0x20) || (pEm->Status_flg & 0x400)) {
            pEm->Life = -1;
            EM10_W(pEm)->Flg &= 0xFFFB;
            EM_RTN(pEm) = 7;
            break;
        }
        pEm->Type = 0x2001;
        if (Rnd() & 1)
            EM_RTN(pEm) = 1;
        else
            EM_RTN(pEm) = 0x201;
        pEm->Damage_cnt &= 0x7F;
        break;
    }
}

/* RE2 Em10_haidm: damage while crawling (Type & 1) */
void func_80109D08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Ofs[2] = { -450, 250 };      /* unused */
    SVECTOR Sv;
    LONG Mode;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = ((Rnd() & 7) << 8) + 0x30007;
        pEm->Routine_2 = 1;
        Mode = 0;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        if (pEm->Unk12C != 0x1A) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = 0;
            Sv.vz = 0;
            func_8001B484(Mode << 24, 0x17700000, &pP->Workm, &Sv, 0);
        }
        if (!EM10_W(pEm)->Unk21) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
    case 1:
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        pEm->Type = 1;
        EM_RTN(pEm) = 1;
        break;
    }
}

/* RE2 Em10_dm07 slot: the head is hit (shakes the head part and goes back to the saved routine) */
void func_80109E54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;
    SVECTOR Sv;
    MATRIX M, Mt;
    VECTOR V;                          /* unused (frame size) */
    signed char Shake[8] = { 24, -8, -24, 0, -12, 0, 12, 8 };

    switch (pEm->Routine_2) {
    case 0:
        pEm->Unk1C2 = 30;
        pEm->Be_flg |= 0x80000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0;
        pEm->UnkDA = 1;
        pEm->UnkE2 = 0;
        if (pEm->Unk132 & 0x10)
            pEm->UnkDA = -1;
        func_8010C57C(pEm);
        pEm->UnkCA = 15;
        if ((EM10_W(pEm)->Routine & 0xFF) == 1)
            EM10_W(pEm)->Unk14 = EM10_W(pEm)->Routine;
        if (pEm->Life < 0) {
            pEm->Life = 0;
            pEm->UnkE2 = 1;
            pP = &pEm->pSin_parts_ptr[1];
            func_8001B484(0x300, (USHORT)(Rnd() << 4) | 0x1F400000, &pP->Workm, &Sv, 0);
            pEm->UnkCA = 15;
        }
        pEm->UnkE3 = 12;
        pEm->UnkE2 = Shake[Rnd() & 7];
    case 1:
        if (!Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = -(pEm->Timer0 * pEm->UnkE2 * 8) * pEm->UnkDA;
            Sv.vz = -(pEm->Timer0 * pEm->UnkE3 * 8) * pEm->UnkDA;
            RotMatrix(&Sv, &M);
            MulMatrix((MATRIX *)pP->M, &M);
            pP = &pEm->pSin_parts_ptr[8];
            TransposeMatrix(&M, &Mt);
            MulMatrix2(&Mt, (MATRIX *)pP->M);
            if (pEm->Timer0++ >= 3) {
                pEm->Routine_2 = 2;
                pEm->Timer0 = 16;
            }
            break;
        } else {
            EM_RTN(pEm) = EM10_W(pEm)->Unk14;
            EM10_W(pEm)->Flg &= 0xFFED;
            pEm->Routine_2++;
        }
        break;
    case 2:
        if (!Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            pP = pEm->pSin_parts_ptr;
            Sv.vx = 0;
            Sv.vy = -(pEm->Timer0 * pEm->UnkE2) * pEm->UnkDA;
            Sv.vz = -(pEm->Timer0 * pEm->UnkE3) * pEm->UnkDA;
            RotMatrix(&Sv, &M);
            MulMatrix((MATRIX *)pP->M, &M);
            pP = &pEm->pSin_parts_ptr[8];
            TransposeMatrix(&M, &Mt);
            MulMatrix2(&Mt, (MATRIX *)pP->M);
            if (pEm->Timer0-- == 0)
                pEm->Routine_2 = 3;
            break;
        } else {
            EM_RTN(pEm) = EM10_W(pEm)->Unk14;
            EM10_W(pEm)->Flg &= 0xFFED;
            pEm->Routine_2++;
        }
        break;
    case 3:
        if (Joint_move_r(pEm, pKan, pSeq, 0x100)) {
            EM_RTN(pEm) = EM10_W(pEm)->Unk14;
            EM10_W(pEm)->Flg &= 0xFFED;
            pEm->Routine_2++;
        }
    }
}

/* RE2 Em10_dm slot: knocked back by a blast */
void func_8010A1D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;                        /* unused */
    UCHAR Mot[8] = { 27, 28, 29, 28, 27, 29, 27, 28 };
    LONG Idx;
    LONG No;
    LONG Mv;

    switch (pEm->Routine_2) {
    case 0:
        Idx = Rnd() & 7;
        Mv = Mot[Idx];
        No = pEm->Move_no;
        if (No != Mv)
            EM_MOVE(pEm) = Mv + 0x60000;
        else
            EM_MOVE(pEm) = (ULONG)(No - 26 + (Rnd() & 1)) % 3 + 0x6001B;
        pEm->Routine_2 = 1;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Pl_pos_x, G.Pl_pos_z);
        func_8010C57C(pEm);
        if (!EM10_W(pEm)->Unk21) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Pl_pos_x, G.Pl_pos_z);
        pEm->Spd_x = (Rnd() & 0x1F) + 60;
        pEm->Spd_z = 0x40 - (Rnd() & 0x7F);
        Add_speedXZ(pEm, pEm->Timer0 - pEm->Cdir_y + 0x800);
        Goto00(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x10);
    case 1:
        if (Joint_move_r(pEm, pKan, pSeq, 0x200)) {
            EM_RTN(pEm) = 0x101;
            if (Rnd() & 1)
                EM_RTN(pEm) = 0x201;
        }
        break;
    case 2:
        EM_RTN(pEm) = 0x101;
        if (Rnd() & 1)
            EM_RTN(pEm) = 0x201;
        break;
    }
}

/* RE2 Em10_haidm slot: hit while crawling, the upper body may burst */
void func_8010A3DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Ofs[2] = { -450, 250 };      /* unused */
    SVECTOR Sv;                        /* unused */
    PARTS_W *pP;
    LONG Mode;
    PARTS_W *pS;

    switch (pEm->Routine_2) {
    case 0:
        Mode = 0;
        EM_MOVE(pEm) = ((Rnd() & 7) << 8) + 0x30007;
        pEm->Routine_2 = 1;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pP = pEm->pSin_parts_ptr;
        func_8001B484(Mode << 24, 0x17700000, &pP->Workm, 0, 0);
        pS = &pEm->pSin_parts_ptr[1];
        func_8001B484((Mode << 24) | 0x700, (USHORT)pEm->Cdir_y | 0x10000000, &pS->Workm, 0, 0);
        if (!EM10_W(pEm)->Unk21) {
            Snd_se_enem(12, pEm);
            EM10_W(pEm)->Unk21 = 150;
        }
        pEm->UnkE3 = 0;
        if ((!(pEm->Unk132 & 0xC) && (Rnd() & 3) == 1) || (pEm->Unk132 & 0x400))
            pEm->UnkE3 = 1;
        if (pEm->UnkE3) {
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 4;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP = &pEm->pSin_parts_ptr[8];
            pP->Be_flg |= 0x22;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
            pP->UnkA2 |= 0x1004;
            pP = &pEm->pSin_parts_ptr[9];
            pP->Be_flg |= 0x22;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[12];
            pP->Be_flg |= 0x22;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[10];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            pP = &pEm->pSin_parts_ptr[13];
            pP->Be_flg |= 0x20;
            pP->UnkA2 |= 0x1000;
            if (pEm->Parts_num == 15) {
                pP = &pEm->pSin_parts_ptr[11];
                pP->Be_flg |= 0x20;
                pP->UnkA2 |= 0x1000;
                pP = &pEm->pSin_parts_ptr[14];
                pP->Be_flg |= 0x20;
                pP->UnkA2 |= 0x1000;
            }
        }
    case 1:
        if (pEm->UnkE3 && (pEm->Move_cnt & 1)) {
            pP = pEm->pSin_parts_ptr;
            func_8001B484(0, (USHORT)(Rnd() << 4) | 0x10000000, &pP->Workm, 0, 0);
        }
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        pEm->Type = 1;
        EM_RTN(pEm) = 1;
        break;
    }
}

/* RE2 Em10_die slot: falling over dead (sometimes gets up again as a crawler) */
void func_8010A730(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG T;

    switch (pEm->Routine_2) {
    case 0:
        pEm->UnkE2 = Rnd() & 1;
        if (EM10_W(pEm)->Flg & 0x2000)
            pEm->UnkE2 = 0;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        EM_MOVE(pEm) = pEm->UnkE2 * 0xA00 + 0xF0000 + Mot[pEm->UnkE2];
        pEm->Unk120 |= 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Routine_2 = 1;
        pEm->UnkE3 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE3 = 5;
        func_8010C57C(pEm);
        if (Rnd() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        if ((pEm->Unk132 & 3) != 1) {
            T = pEm->UnkE2 * 10;
            if (pEm->Move_cnt >= T + 4 && pEm->Move_cnt <= T + 16 && (pEm->Move_cnt & 1)) {
                pP = &pEm->pSin_parts_ptr[1];
                Sv.vx = pP->Pos_x;
                Sv.vy = pP->Pos_y + 500;
                Sv.vz = pP->Pos_z;
                pP = pEm->pSin_parts_ptr;
                func_8001B484((pEm->UnkE3 & 0xF) << 24, (((Rnd() << 3) + 0xFD0) << 16) | (USHORT)(Rnd() << 4),
                              &pP->Workm, &Sv, 0);
            }
        }
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        if (!pEm->UnkCA) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_RTN(pEm) = 7;
        if (!(Rnd() & 3) && !(EM10_W(pEm)->Flg & 4) && !(pEm->Type & 0x20) && !(pEm->Status_flg & 0x400)
            && !pEm->Water && (LONG)G.Status_flg >= 0) {
            pEm->Life = 1;
            pEm->Type = 0x2001;
            Rnd();
            EM_RTN(pEm) = 1;
            do {                       /* (a macro in the original? the loop notes keep the load below the store) */
                pEm->Damage_cnt &= 0x7F;
            } while (0);
        }
        break;
    }
}

/* RE2 Em10_haidie: death while crawling (Type & 1) */
void func_8010AA5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };           /* unused */
    SVECTOR Sv;
    LONG Mode;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0xF0008;
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        pEm->Unk120 |= 1;
        pEm->Damage_cnt |= 0x80;
        Mode = 0;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = 300;
        Sv.vz = 0;
        func_8001B484(Mode << 24, (USHORT)pEm->Cdir_y | 0x1FA00000, &pP->Workm, &Sv, 0);
        if (Rnd() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        break;
    case 2:
        EM_RTN(pEm) = 7;
        break;
    }
}

/* RE2 Em10_die02 */
void func_8010ABAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Spd[2] = { -225, 125 };
    PARTS_W *pP;
    SVECTOR Sv;
    UCHAR Em10_die02_mot[2] = { 3, 1 };

    if (EM10_W(pEm)->Unk19 == 1) {
        func_8010B6D4(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        pEm->Life = -1;
        pEm->UnkE3 = (pEm->Unk132 & 0x10) >> 4;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (!pEm->UnkE3)
            EM10_W(pEm)->Flg |= 4;
        EM_MOVE(pEm) = 0x30005 - pEm->UnkE3;
        pEm->Routine_2 = 1;
        pEm->UnkDA = 10;
        pEm->Spd_x = Spd[pEm->UnkE3];
        pEm->Spd_z = 0;
        Add_speedXZ(pEm, 0);
        pEm->Unk120 = 1;
        pEm->Damage_cnt |= 0x80;
        pP = &pEm->pSin_parts_ptr[1];
        pP->Poly_rgb = 0x303030;
        if (Rnd() & 1) {
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_FREE(pP)[0] = pEm->Cdir_y + 0x800;
            PW_FREE(pP)[2] = 400;
            PW_FREE(pP)[1] = -100;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[3] = 10;
            PW_FREE(pP)[6] = -50;
            pP->Poly_rgb = 0x101030;
        }
        pEm->UnkE2 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE2 = 5;
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        func_8001B484((pEm->UnkE2 & 0xF) << 24, (USHORT)(Rnd() << 4) | 0xBB80000, &pP->Workm, &Sv, 0);
        func_8001B484(8, ((Rnd() + 0xFA0) << 18) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, 0);
        pP = &pEm->pSin_parts_ptr[2];
        if (!(EM10_W(pEm)->Flg & 0x20)) {
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + 0x800;
            pP->Unk96 = 0;
            pP->Old_z = 100;
            pP->Old_y = 0;
            pP->Unk85 = 30;
            pP->Unk90 = 0;
            pP->Unk92 = pEm->Cdir_y;
            pP->Unk8E = 0x400;
        } else {
            pP[0].Be_flg = 0;
            pP[1].Be_flg = 0;
        }
        pP = &pEm->pSin_parts_ptr[5];
        if (!(EM10_W(pEm)->Flg & 0x40) && !(Rnd() & 1)) {
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
        pP = pEm->pSin_parts_ptr;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        pP->UnkA2 |= 4;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -500;
        Sv.vz = 0;
        func_8001B484((pEm->UnkE2 & 0xF) << 24, (USHORT)(pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4)) | 0x1F400000,
                      &pP->Workm, &Sv, 0);
        func_8001B484(8, ((Rnd() + 0xFA0) << 18) | (USHORT)(Rnd() << 4), &pP->Workm, &Sv, 0);
        Snd_se_enem(2, pEm);
        Em_dead_flg_on_cand(pEm);
        pEm->Spd_x = 11;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x400);
        Add_speedXZ(pEm, 0);
        if (pEm->UnkDA) {
            if (!(pEm->UnkDA & 1)) {
                pP = pEm->pSin_parts_ptr;
                func_8001B484((pEm->UnkE2 & 0xF) << 24,
                              (USHORT)(pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4)) | 0x1F400000, &pP->Workm,
                              (SVECTOR *)&pEm->Spd_x, 0);
                func_8001B484((pEm->UnkE2 & 0xF) << 24,
                              (USHORT)(pEm->Cdir_y + (Rnd() << 4) - (Rnd() << 4)) | 0x1F400000, &pP->Workm,
                              (SVECTOR *)&pEm->Spd_x, 0);
            }
            pEm->UnkDA--;
        }
        break;
    case 2:
        if (Rnd() & 1) {
            pEm->Routine_2 = 5;
            break;
        }
        EM_MOVE(pEm) = ((pEm->UnkE3 * 15 + 10) << 8) + 0x70000 + Em10_die02_mot[pEm->UnkE3];
        pEm->Routine_2 = 3;
    case 3:
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        if (!pEm->UnkCA) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 4:
        EM_RTN(pEm) = 7;
        break;
    case 5:
        EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0xF0000;
        pEm->Routine_2 = 6;
        pEm->Timer0 = (Rnd() & 0x1F) + 15;
    case 6:
        if (pEm->Timer0-- == 0) {
            pEm->Routine_2 = 3;
            EM_MOVE(pEm) = 0x70001;
            EM10_W(pEm)->Flg &= 0xFFFB;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
    }
}

/* RE2 Em10_die slot: the head is blown off */
void func_8010B278(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG Mode;
    LONG Dir;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Life = -1;
        pEm->UnkE2 = Rnd() & 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        Mode = 0;
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        if (pEm->Unk132 & 0x200)
            Mode = 5;
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        func_8001B484((Mode << 24) | 0x200, (USHORT)pEm->Cdir_y | 0x10000000, &pP->Workm, &Sv, 0);
        Dir = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Pl_pos_x, G.Pl_pos_z);
        Sv.vy -= 400;
        func_8001B484(8, (USHORT)Dir | 0x10000000, &pP->Workm, &Sv, 0);
        pEm->Unk120 = 1;
        if (G.Status_flg & 0x8000) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 4;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
        } else {
            pP = &pEm->pSin_parts_ptr[1];
            pP->UnkA2 |= 0x20;
            pP->Be_flg |= 2;
            PW_FREE(pP)[0] = Dir + 0x800;
            PW_FREE(pP)[2] = 400;
            PW_FREE(pP)[1] = -100;
            PW_FREE(pP)[3] = 10;
            PW_FREE(pP)[4] = 0;
            PW_FREE(pP)[6] = -50;
            pP->Poly_rgb = 0x101040;
        }
        pP = pEm->pSin_parts_ptr;
        pP->UnkA2 |= 4;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        pP = &pEm->pSin_parts_ptr[2];
        pP->UnkA2 |= 4;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        pP = &pEm->pSin_parts_ptr[5];
        pP->UnkA2 |= 4;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        Snd_se_enem(2, pEm);
        Em_dead_flg_on_cand(pEm);
        if (Rnd() & 1) {
            pEm->Routine_2 = 3;
            break;
        }
        EM_MOVE(pEm) = pEm->UnkE2 * 0xA00 + 0xF0000 + Mot[pEm->UnkE2];
    case 1:
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        if (!pEm->UnkCA) {
            func_8002646C(pEm, 400, 40, pEm->Col_unk39);
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_RTN(pEm) = 7;
        break;
    case 3:
        if (pEm->Move_no < 8)
            pEm->UnkCA = 15;
        else
            EM_MOVE(pEm) = EM10_W(pEm)->Walk_no + 0xF0000;
        pEm->Routine_2 = 4;
        pEm->Timer0 = (Rnd() & 0x3F) + 30;
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
    case 4:
        if (pEm->Timer0-- == 0) {
            pEm->Routine_2 = 1;
            EM_MOVE(pEm) = 0xF0001;
            EM10_W(pEm)->Flg &= 0xFFFB;
        }
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 8);
        Spd_get(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        Joint_move_r(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x100);
        Add_speedXZ(pEm, 0);
        break;
    }
}

/* RE2 Em10_die slot: blown away by a close shot */
void func_8010B6D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Mot[2] = { 1, 3 };
    SVECTOR Sv;
    PARTS_W *pP;
    LONG n;
    LONG T;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        pEm->Life = -1;
        pEm->Unk120 |= 1;
        pEm->Damage_cnt |= 0x80;
        pEm->Timer0 = Direction_ck(pEm->Pos_x, pEm->Pos_z, G.Pl_pos_x, G.Pl_pos_z);
        n = (Rnd() & 3) + 1;
        pEm->UnkE3 = 0;
        if (pEm->Unk132 & 0x200)
            pEm->UnkE3 = 5;
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -300;
        Sv.vz = 0;
        func_8001B484((pEm->UnkE3 & 0xF) << 24, (USHORT)pEm->Timer0 | 0x1FA00000, &pP->Workm, &Sv, 0);
        Sv.vx = 0;
        Sv.vy = -100;
        Sv.vz = 0;
        func_8001B484((pEm->UnkE3 & 0xF) << 24, (USHORT)pEm->Timer0 | 0x17D00000, &pP->Workm, &Sv, 0);
        if (!(pEm->Unk132 & 0xA00)) {
            do {
                n--;
                Sv.vx = 0x100 - (Rnd() << 1);
                Sv.vy = 0x38 - (Rnd() << 1);
                Sv.vz = 0x100 - (Rnd() << 1);
                func_8001B484(0x209, (((Rnd() << 2) + 0x600) << 16) | (USHORT)(pEm->Timer0 + 0x200 - (Rnd() << 2)),
                              &pP->Workm, &Sv, 0);
            } while (n);
            pP = pEm->pSin_parts_ptr;
            pP->UnkA2 |= 4;
            PW_FREEL(pP)[0] = 0x40;
            PW_FREEL(pP)[1] = 0x202030;
        }
        pEm->UnkE2 = ~Cdir_ck(pEm, &Pl_work) & 1;
        EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->UnkE2)
            EM10_W(pEm)->Flg |= 4;
        EM_MOVE(pEm) = Mot[pEm->UnkE2] + 0xF0F00;
        pEm->Routine_2 = 1;
        if (Rnd() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
        pEm->Spd_x = 200;
        pEm->Spd_z = 0;
    case 1:
        T = pEm->UnkE2 * 10;
        if (pEm->Move_cnt >= T + 4 && pEm->Move_cnt <= T + 16 && (pEm->Move_cnt & 1)) {
            pP = &pEm->pSin_parts_ptr[1];
            Sv.vx = pP->Pos_x;
            Sv.vy = pP->Pos_y + 500;
            Sv.vz = pP->Pos_z;
            pP = pEm->pSin_parts_ptr;
            func_8001B484((pEm->UnkE3 & 0xF) << 24, (((Rnd() << 3) + 0xFD0) << 16) | (USHORT)(Rnd() << 4),
                          &pP->Workm, &Sv, 0);
        }
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x100);
        Add_speedXZ(pEm, pEm->UnkE2 << 11);
        if ((pEm->Spd_x -= 50) < 0)
            pEm->Spd_x = 0;
        if (!pEm->Spd_x) {
            if (EM10_W(pEm)->Flg & 0x8000)
                func_8010C3F0(pEm, 0, 1);
            else
                Foot_set_pl(pEm, 0, 1);
        }
        break;
    case 2:
        EM_RTN(pEm) = 7;
        break;
    }
}

/* RE2 Em10_die slot: falling down dead (Unk1C picks the motion) */
void func_8010BB08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Tbl[3][3] = { { 0x37, 0x17, 0x01 }, { 0x28, 0x18, 0x01 }, { 0x00, 0x01, 0x01 } };
    SVECTOR Sv;
    PARTS_W *pP;

    switch (pEm->Routine_2) {
    case 0:
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        EM10_W(pEm)->Flg |= 4;
        if (EM10_W(pEm)->Unk1C == 1)
            EM10_W(pEm)->Flg &= 0xFFFB;
        if (pEm->Move_cnt < Tbl[EM10_W(pEm)->Unk1C][0]) {
            EM_MOVE(pEm) = Tbl[EM10_W(pEm)->Unk1C][1] + 0x1F0000;
            pEm->UnkE3 = 0;
        } else {
            EM10_W(pEm)->Flg &= 0xFFFB;
            EM_MOVE(pEm) = Tbl[EM10_W(pEm)->Unk1C][2] + 0x1F1E00;
            pEm->UnkE3 = 1;
        }
        pP = &pEm->pSin_parts_ptr[1];
        pEm->Routine_2 = 1;
        pEm->Unk120 |= 1;
        pEm->Damage_cnt |= 0x80;
        Sv.vx = pP->Pos_x;
        Sv.vy = pP->Pos_y + 300;
        Sv.vz = pP->Pos_z;
        pP = pEm->pSin_parts_ptr;
        func_8001B484(0, (USHORT)pEm->Cdir_y | 0x1FA00000, &pP->Workm, &Sv, 0);
        if (Rnd() & 1)
            Snd_se_enem(15, pEm);
        else
            Snd_se_enem(14, pEm);
        Em_dead_flg_on_cand(pEm);
    case 1:
        if (Joint_move_r(pEm, pKan, pSeq, 0x80)) {
            if (!pEm->UnkE3)
                pEm->Routine_2 = 2;
            else
                pEm->Routine_2 = 3;
        }
        break;
    case 2:
        Joint_move_r(pEm, pKan, pSeq, 0x80);
        if (!pEm->UnkCA)
            pEm->Routine_2 = 3;
        break;
    case 3:
        EM_RTN(pEm) = 7;
        break;
    }
}

/* death by explosion: head, arms and legs fly off (die table, damage types 0x14-0x16) */
void func_8010BD60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Sv;
    MATRIX M;
    PARTS_W *pP;
    UCHAR R;

    if (EM10_W(pEm)->Unk19 == 1) {
        func_8010ABAC(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_8002646C(pEm, 400, 80, pEm->Col_unk39);
        if (!(Rnd() & 1)) {
            EM10_W(pEm)->Unk19 = 1;
            func_8010ABAC(pEm, pKan, pSeq);
            return;
        }
        pEm->Life = -1;
        pEm->Unk120 |= 1;
        pEm->Damage_cnt |= 0x80;
        Em_dead_flg_on_cand(pEm);
        EM_MOVE(pEm) = 0x70003;
        R = Rnd();
        if (R & 1) {
            pP = &pEm->pSin_parts_ptr[1];
            pP->Be_flg |= 0xA;
            pP->Old_x = R << 4;
            pP->Old_z = 100;
            pP->Old_y = -300 - (R & 0x1F);
            pP->Unk85 = 25;
            pP->Unk96 = 0;
            pP->Unk90 = 0xFF;
            pP->Unk92 = 0;
            pP->Unk8E = 0;
            func_8001B484(0, 0x20000000, &pP->Workm, 0, 0);
            func_8001B484(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        R = Rnd();
        if (!(R & 1)) {
            pP = &pEm->pSin_parts_ptr[5];
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + (SHORT)(R - 0x480);
            pP->Unk96 = 0;
            pP->Old_z = 130;
            pP->Old_y = -200 - (R & 0x1F);
            pP->Unk85 = 25;
            pP->Unk90 = 0xFF;
            pP->Unk92 = 0;
            pP->Unk8E = 0;
            func_8001B484(0, 0x20000000, &pP->Workm, 0, 0);
            func_8001B484(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        pP = &pEm->pSin_parts_ptr[3];
        pP->Be_flg |= 0xA;
        pP->Old_x = pEm->Cdir_y + (R + 0x380);
        pP->Unk96 = 0;
        pP->Old_z = 150;
        pP->Old_y = -350 - (R & 0x1F);
        pP->Unk85 = 30;
        pP->Unk90 = 0xFF;
        pP->Unk92 = 0;
        pP->Unk8E = 0;
        func_8001B484(0, 0x20000000, &pP->Workm, 0, 0);
        func_8001B484(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        R = Rnd();
        if (!(R & 1)) {
            pP = &pEm->pSin_parts_ptr[10];
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + (R + 0x380);
            pP->Unk96 = 0;
            pP->Old_z = 150;
            pP->Old_y = -400 - (R & 0x1F);
            pP->Unk85 = 30;
            pP->Unk90 = 0xFF;
            pP->Unk92 = 0;
            pP->Unk8E = 0;
            func_8001B484(0, 0x20000000, &pP->Workm, 0, 0);
            func_8001B484(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        pP = &pEm->pSin_parts_ptr[13];
        pP->Be_flg |= 0xA;
        pP->Old_x = pEm->Cdir_y + (R + 0x380);
        pP->Unk96 = 0;
        pP->Old_z = 150;
        pP->Old_y = -400 - (R & 0x1F);
        pP->Unk85 = 30;
        pP->Unk90 = 0xFF;
        pP->Unk92 = 0;
        pP->Unk8E = 0;
        func_8001B484(0, 0x20000000, &pP->Workm, 0, 0);
        func_8001B484(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        R = Rnd();
        if (R & 3) {
            pP = &pEm->pSin_parts_ptr[8];
            pP->Be_flg |= 0xA;
            pP->Old_x = pEm->Cdir_y + (SHORT)((R << 2) + 0x600);
            pP->Unk96 = 0;
            pP->Old_z = 100;
            pP->Old_y = -400;
            pP->Unk85 = 30;
            pP->Unk90 = 0xFF;
            pP->Unk92 = 0;
            pP->Unk8E = 0;
            func_8001B484(0, 0x20000000, &pP->Workm, 0, 0);
            func_8001B484(0x1B03, 0x8000000, &pP->Workm, 0, 0);
        }
        func_8001B484(8, 0x10000000, &pP->Workm, 0, 0);
        pEm->Routine_2 = 1;
        EM10_W(pEm)->Flg |= 4;
        func_80108B00(pEm);
        pP = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = -200;
        Sv.vz = 0;
        func_8001B484(5, 0x1A000000, &pP->Workm, &Sv, 0);
        func_8001B484(0x1B03, 0x10000000, &pP->Workm, &Sv, 0);
        pP = pEm->pSin_parts_ptr;
        PW_FREEL(pP)[0] = 0x40;
        PW_FREEL(pP)[1] = 0x202030;
        pP->UnkA2 |= 4;
    case 1:
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = 0x80;
        Sv.vy = 0;
        Sv.vz = 0;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[5];
        Sv.vx = 0x80;
        Sv.vy = 0x40;
        Sv.vz = 0;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[3];
        Sv.vx = 0x80;
        Sv.vy = 0;
        Sv.vz = 0x80;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[10];
        Sv.vx = 0x40;
        Sv.vy = 0;
        Sv.vz = 0x40;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[13];
        Sv.vx = -0x40;
        Sv.vy = 0;
        Sv.vz = 0x40;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[8];
        Sv.vx = 0;
        Sv.vy = 0x80;
        Sv.vz = 0;
        RotMatrix_gte(&Sv, &M);
        MulMatrix2(&M, &pP->Workm);
        pEm->Routine_2 += Joint_move_r(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        EM_RTN(pEm) = 7;
        break;
    }
}

void func_8010C3E8(void) {
}

/* Foot_set_pl for the zombie: keeps foot Foot (0/1, parts 8.. / 10..) on the ground */
void func_8010C3F0(ENEMY_WORK *pEm, LONG Mode, LONG Foot)
{
    MATRIX M;
    SVECTOR Sv = { 0, 500, 0 };
    SVECTOR Sv0;
    SVECTOR Sv1;
    SVECTOR Rot;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
    if (pEm->Be_flg & 0xC0000) {
        Rot.vy = 0;
        Rot.vx = pEm->Unk1BC[0];
        Rot.vz = pEm->Unk1BC[1];
        RotMatrix_gte(&Rot, &M);
        MulMatrix2(&M, (MATRIX *)pEm->Workm_m);
    }
    func_8002D4B0((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &M);
    func_8002D4B0(&M, (MATRIX *)pP[6].M, &M);
    pP = &pP[Foot * 2 + 8];
    func_8002D4B0(&M, (MATRIX *)pP[-1].M, &M);
    func_8002D4B0(&M, (MATRIX *)pP->M, &M);
    ApplyMatrixSV(&M, &Sv, &Sv0);
    ApplyMatrixSV(&pP->Workm, &Sv, &Sv1);
    pEm->Pos_x -= (M.t[0] + Sv0.vx) - (pP->Workm.t[0] + Sv1.vx);
    pEm->Pos_z -= (M.t[2] + Sv0.vz) - (pP->Workm.t[2] + Sv1.vz);
}

/* blood when hit */
void func_8010C57C(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    PARTS_W *pP;
    PARTS_W *pW;
    LONG Mode;

    Mode = 0;
    if (pEm->Unk12C == 0x1A)
        return;
    if (pEm->Unk132 & 0x200)
        Mode = 5;
    if ((pEm->Unk132 & 3) == 1) {
        pW = pEm->pSin_parts_ptr;
        Sv.vx = 0;
        Sv.vy = 800;
        Sv.vz = 0;
        func_8001B484(Mode << 24, (USHORT)pEm->Cdir_y | 0x17D00000, &pW[8].Workm, &Sv, 0);
    } else {
        pP = &pEm->pSin_parts_ptr[1];
        Sv.vx = pP->Pos_x - ((Rnd() & 0x7F) - 0x40);
        Sv.vy = pP->Pos_y + 0xE4 - Rnd();
        Sv.vz = pP->Pos_z - ((Rnd() & 0x7F) - 0x40);
        pP = pEm->pSin_parts_ptr;
        func_8001B484(Mode << 24, (((Rnd() << 4) + 0x800) << 16) | (USHORT)pEm->Cdir_y, &pP->Workm, &Sv, 0);
    }
}

/* RE2 Em10_water: splashes */
void func_8010C698(ENEMY_WORK *pEm)
{
    SVECTOR Sv;
    SVECTOR Sv1;
    PARTS_W *pP;

    pP = pEm->pSin_parts_ptr;
    func_8002D4B0((MATRIX *)pEm->Workm_m, (MATRIX *)pP->M, &pP->Workm);
    Sv.vx = pP->Workm.t[0];
    Sv.vy = pEm->Water;
    Sv.vz = pP->Workm.t[2];
    Sv1.vx = 300;
    Sv1.vy = 0;
    Sv1.vz = 0;
    ApplyMatrixSV((MATRIX *)pEm->Workm_m, &Sv1, &Sv1);
    Sv1.vx += Sv.vx;
    Sv1.vy += Sv.vy;
    Sv1.vz += Sv.vz;
    if (pEm->Water == 1) {
        Sv1.vx = 0x400;
        Sv1.vy = 0;
        Sv1.vz = 0;
        func_8001B484(0x20E, (USHORT)pEm->Cdir_y | 0x20000000, &GsIDMATRIX, &Sv, &Sv1);
    } else {
        func_8001B484(0x61C, (((Rnd() << 2) + 0xC00) << 16) | (USHORT)(pEm->Cdir_y - (Rnd() << 2)), &GsIDMATRIX, &Sv1, 0);
        func_8001B484(0x61C, (((Rnd() << 2) + 0xC00) << 16) | (USHORT)(0x800 + pEm->Cdir_y - (Rnd() << 2)), &GsIDMATRIX, &Sv, 0);
        func_8001B484(0x161C, (((Rnd() << 2) + 0x800) << 16) | (USHORT)(Rnd() << 4), &GsIDMATRIX, &Sv, 0);
        func_8001B484(0x161C, (((Rnd() << 2) + 0xC00) << 16) | (USHORT)(Rnd() << 4), &GsIDMATRIX, &Sv, 0);
        Sv1.vx = 0x400;
        Sv1.vy = 0;
        Sv1.vz = 0;
        func_8001B484(0x20E, (USHORT)pEm->Cdir_y | 0x20000000, &GsIDMATRIX, &Sv, &Sv1);
    }
}
