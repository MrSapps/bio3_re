#include "common.h"
#include "ovl/em38.h"

/* module .data; routine tables are unprototyped like RE2's, the routines take (pEm, pKan, pSeq) */
extern void (*Em38_routine_0[])() __asm__("D_8010CCA0");
extern void (*Em38_Move_type[])() __asm__("D_8010CCC4");
extern void (*Em38_mv_br_type00[])() __asm__("D_8010CCD4");
extern void (*Em38_mv_type00[])() __asm__("D_8010CD14");
extern void (*Em38_dm[])() __asm__("D_8010CD54");
extern void (*Em38_sce[])() __asm__("D_8010CDD4");
extern void (*Pldamage_Em38_tbl[])() __asm__("D_8010CE3C");
extern void (*Pldie_Em38_tbl[])() __asm__("D_8010CE54");

extern VECTOR Em38_goal_pos __asm__("D_8010CC60");

void func_801011D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B550(ENEMY_WORK *pEm);
void func_8010B6C4(ENEMY_WORK *pEm);
void func_8010B848(ENEMY_WORK *pEm);
void func_8010B94C(ENEMY_WORK *pEm);
void func_8010A924(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80100154);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80100714);

/* RE2 Em_Move */
void func_801011D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em38_Move_type[pEm->Type & 3](pEm, pKan, pSeq);
}

/* RE2 Em_Move_type00 */
void func_80101214(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em38_mv_br_type00[pEm->Routine_1](pEm);
    Em38_mv_type00[pEm->Routine_1](pEm, pKan, pSeq);
}

/* mv00_br */
void func_80101298(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pPl = &Pl_work;
    ULONG L_pl;

    if (pEm->Routine_2 == 0) {
        pEm->Col_w0 = 1800;
        pEm->Col_d0 = 1800;
        pEm->Unk134 = 1800;
        pEm->Be_flg &= ~0x2000;
        EM38_W(pEm)->Flg &= ~0xC;
        pEm->Be_flg &= ~0x20;
    }
    L_pl = pEm->Unk154;
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x400);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x200);
    Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x600);
    if (L_pl < 5000)
        EM_RTN(pEm) = 0x101;
}

/* mv00 */
void func_80101364(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x1F0000;
        pEm->Routine_2 = 1;
        EM38_W(pEm)->Flg &= ~2;
    case 1:
        Joint_move(pEm, pKan, pSeq, 0x80);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801013C0);

/* mv01 */
void func_80101714(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x7F0001;
        pEm->Routine_2 = 1;
        EM38_W(pEm)->Flg &= ~2;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->UnkCA) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->UnkCA) >> 7;
        Joint_move(pEm, pKan, pSeq, 0x20);
        break;
    }
    Add = 2;
    if (pEm->Spd_x > 30)
        Add = 6;
    if (pEm->Spd_x > 20)
        Add += 4;
    if (!Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x10))
        Add = 0x10;
    if (EM38_W(pEm)->Flg & 0x80) {
        Add += 0x40;
        pEm->Spd_x += 30;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

/* mv03_br */
void func_801018A8(ENEMY_WORK *pEm)
{
    ULONG L_pl;
    SHORT Ck;

    L_pl = pEm->Unk154;
    Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x400);
    Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x200);
    Ck = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x600);
    if (L_pl > 4000)
        EM_RTN(pEm) = 0x101;
    if (L_pl < 3000 && Ck && !EM38_W(pEm)->Unk0A)
        EM_RTN(pEm) = 0x601;
}

/* mv03 */
void func_8010195C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x7F0002;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 90;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->UnkCA) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->UnkCA) >> 7;
        Joint_move(pEm, pKan, pSeq, 0x20);
        if (--pEm->Timer0 == -1)
            EM_RTN(pEm) = 0x101;
        break;
    }
    Add = (pEm->Spd_x < -10) * 2;
    if (pEm->Spd_x < -20)
        Add += 2;
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

void func_80101AC4(void) {
}

/* mv04 */
void func_80101ACC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SHORT Old_x, Old_z;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        EM_MOVE(pEm) = pEm->UnkE3 + 0x1F0003;
        EM38_W(pEm)->Flg |= 8;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->UnkCA) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->UnkCA) >> 5;
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            pEm->Cdir_y -= 0x400;
            if (pEm->UnkE3)
                pEm->Cdir_y += 0x800;
            EM_RTN(pEm) = 0x101;
            EM38_W(pEm)->Unk0B = 60;
            pEm->Spd_x = 0;
            pEm->Spd_z = 0;
            Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 4);
            EM38_W(pEm)->Flg &= ~0xC;
        }
        break;
    }
    Add_speedXZ(pEm, 0);
}

void func_80101C4C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80101C54);

void func_80102068(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80102070);

void func_80102940(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80102948);

void func_80102E94(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80102E9C);

void func_80103468(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80103470);

/* mv0a_br */
void func_801037E8(ENEMY_WORK *pEm)
{
    ULONG L_pl;
    SHORT Ck0, Ck1;

    L_pl = pEm->Unk154;
    Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x80);
    Ck0 = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x200);
    Ck1 = Goto00_ck(pEm, Pl_work.Pos_x, Pl_work.Pos_z, 0x600);
    if (pEm->Routine_2 < 2) {
        if (L_pl < 4000 && !Ck0 && !EM38_W(pEm)->Unk0A) {
            EM_RTN(pEm) = 0x501;
            if (!(Rnd() & 3))
                EM_RTN(pEm) = 0x901;
        }
        if (L_pl < 5000 && Ck1 && !EM38_W(pEm)->Unk1C)
            EM_RTN(pEm) = 0x601;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801038E8);

void func_80103FF0(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80103FF8);

/* mv0c_br */
void func_80104718(ENEMY_WORK *pEm)
{
    VECTOR *pV = &Em38_goal_pos;
    LONG Dx, Dz;

    pEm->Life = 10000;
    if (pEm->Routine_2 == 1) {
        Dx = (pV->vx - pEm->Pos_x) >> 1;
        Dz = (pV->vz - pEm->Pos_z) >> 1;
        if (SquareRoot0(Dx * Dx + Dz * Dz) * 2 < 901) {
            Flg_on(&G.Unk7888, 0x1F);
            pEm->Routine_2 = 2;
        }
    }
    if (pEm->Routine_2 == 4) {
        Dx = (pV->vx - pEm->Pos_x) >> 1;
        Dz = (pV->vz - pEm->Pos_z) >> 1;
        if (SquareRoot0(Dx * Dx + Dz * Dz) * 2 > 2499) {
            Flg_off(&G.Unk7888, 0x1F);
            pEm->Routine_2 = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80104828);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80104D84);

/* mv0d */
void func_80104F34(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x7F0001;
        EM38_W(pEm)->Flg |= 2;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 150;
        EM38_W(pEm)->Flg |= 0xC;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x += 40;
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->UnkCA) >> 7;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->UnkCA) >> 7;
        Joint_move(pEm, pKan, pSeq, 0x20);
        Add = 8;
        if (pEm->Spd_x > 30)
            Add = 0x10;
        if (pEm->Spd_x > 20)
            Add += 8;
        Goto00(pEm, pPl->Pos_x, pPl->Pos_z, Add);
        Add_speedXZ(pEm, 0);
        pEm->Timer0--;
        break;
    }
}

void func_801050C4(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801050CC);

/* RE2 Em_Damage */
void func_80105424(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ENEMY_WORK *pPl = &Pl_work;
    PARTS_W *pP;
    EM38_SABUN *pSa;
    SVECTOR Unused;                    /* frame size */

    if (pEm->Routine_2 == 0) {
        if (!pEm->Pos_y) {
            pP = &pEm->pSin_parts_ptr[20];
            pSa = (EM38_SABUN *)pP->pUnk98;
            if (pSa->Unk0C) {
                pSa->Unk0C = 0;
                func_8004A934(pEm, pP);
            }
            if ((pEm->Type & 1) && pEm->Unk12C == 24)
                pEm->Unk12C = 3;
            if (pEm->Unk132 & 0x500)
                func_8010B848(pEm);
            if (pEm->Unk132 & 0x200)
                func_8010B94C(pEm);
            if (pEm->Unk132 & 0x800)
                func_8010B6C4(pEm);
            if (pEm->Unk12C == 5)
                EM38_W(pEm)->Unk18 = 1;
            if (!(EM38_W(pEm)->Flg & 8) && !(pEm->Type & 1) && Goto00_ck(pEm, pPl->Pos_x, pPl->Pos_z, 0x72C))
                pEm->Unk12C = 32;
        }
        if (pEm->Routine_2 == 0 && pEm->Unk12C == 24 && (pEm->nFloor || (pEm->Be_flg & 0x2000)))
            pEm->Unk12C = 3;
    }
    Em38_dm[pEm->Unk12C - 1](pEm, pKan, pSeq);
}

/* dm00: hit, keep moving */
void func_80105600(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP;

    EM38_W(pEm)->Unk10 = 1;
    if (pEm->Type & 1) {
        EM_RTN(pEm) = pEm->Unk1C4;
        func_8010B550(pEm);
        if (pEm->Routine_0 == 2)
            EM_RTN(pEm) = 0xC01;
        func_801011D4(pEm, pKan, pSeq);
        if (!pEm->Pos_y) {
            pP = &pEm->pSin_parts_ptr[27 + (Rnd() & 3)];
            if (!pP->UnkB4) {
                pP->UnkB4 = 1;
                Snd_se_enem(0, pEm);
            }
        }
    } else {
        EM_RTN(pEm) = pEm->Unk1C4;
        func_8010B550(pEm);
        if (pEm->Routine_0 == 2)
            EM_RTN(pEm) = 0x101;
        func_801011D4(pEm, pKan, pSeq);
        if (!pEm->Pos_y) {
            pP = &pEm->pSin_parts_ptr[27 + (Rnd() & 3)];
            if (!pP->UnkB4) {
                pP->UnkB4 = 1;
                Snd_se_enem(0, pEm);
            }
            if ((Rnd() & 3) == 1)
                Snd_se_enem(8, pEm);
        } else {
            Snd_se_enem(8, pEm);
        }
    }
}

/* dm32 */
void func_80105784(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR Ofs;
    PARTS_W *pP;
    SHORT Old_x, Old_z;
    SHORT Add;

    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x1F030A;
        pEm->Routine_2 = 1;
        pEm->Damage_cnt |= 0x80;
        EM38_W(pEm)->Flg &= 0xD;
        pP = EM38_W(pEm)->pUnk00->pSin_parts_ptr;
        Ofs.vx = 0;
        Ofs.vy = -800;
        Ofs.vz = 0;
        func_8001B484(0x200, 0x28000000, &pP->Workm, &Ofs, 0);
        func_8001B484(0x2000200, 0x20000800, &pP->Workm, &Ofs, 0);
        EM38_W(pEm)->Flg &= ~2;
        Snd_se_enem(8, pEm);
        if (pEm->Life > 50)
            pEm->Life -= 40;
    case 1:
        Old_x = pEm->Spd_x;
        Old_z = pEm->Spd_z;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Spd_x -= ((pEm->Spd_x - Old_x) * pEm->UnkCA) >> 5;
        pEm->Spd_z -= ((pEm->Spd_z - Old_z) * pEm->UnkCA) >> 5;
        if (pEm->Move_cnt == 20) {
            pP = EM38_W(pEm)->pUnk00->pSin_parts_ptr;
            Ofs.vx = 0;
            Ofs.vy = -800;
            Ofs.vz = 0;
            func_8001B484(0x200, 0x28000000, &pP->Workm, &Ofs, 0);
            func_8001B484(0x2000200, 0x20000800, &pP->Workm, &Ofs, 0);
        }
        if (Joint_move(pEm, pKan, pSeq, 0x80)) {
            EM38_W(pEm)->Unk0A = 30;
            EM_RTN(pEm) = 0x101;
            pEm->Damage_cnt &= 0x7F;
        }
        break;
    }
    Add = 2;
    if (pEm->Spd_x > 30)
        Add = 6;
    if (pEm->Spd_x > 20)
        Add += 4;
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, Add);
    Add_speedXZ(pEm, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801059EC);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80105D8C);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80105F44);

/* RE2 Em_Scenario */
void func_801060EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Em38_sce[pEm->Routine_1](pEm, pKan, pSeq);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80106128);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801064C8);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801075D8);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801081C8);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80108FD8);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80109394);

void func_8010955C(void) {
}

/* RE2 PLDAMEGE_EM */
void func_80109564(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldamage_Em38_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_801095A0);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80109948);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80109BD8);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80109DF0);

/* RE2 PLDIE_EM */
void func_80109F14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Pldie_Em38_tbl[pEm->Routine_1](pEm, pKan, pSeq);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_80109F50);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010A1C8);

void func_8010A480(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    PARTS_W *pP1;

    RotMatrix((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
    pP = pEm->pSin_parts_ptr;
    func_8002D4B0(pP[0].pSuper, (MATRIX *)pP[0].M, &pP[0].Workm);
    func_8002D4B0(pP[12].pSuper, (MATRIX *)pP[12].M, &pP[12].Workm);
    func_8002D4B0(pP[16].pSuper, (MATRIX *)pP[16].M, &pP[16].Workm);
    func_8002D4B0(pP[17].pSuper, (MATRIX *)pP[17].M, &pP[17].Workm);
    func_8002D4B0(pP[18].pSuper, (MATRIX *)pP[18].M, &pP[18].Workm);
    func_8002D4B0(pP[19].pSuper, (MATRIX *)pP[19].M, &pP[19].Workm);
    pP1 = pEm->pSin_parts_ptr;
    pP = &pP1[16];
    func_8010A924(pEm, &pP1[12], pP);
    func_8010A924(pEm, pP, &pP1[17]);
    func_8010A924(pEm, &pP1[17], &pP1[18]);
    func_8010A924(pEm, &pP1[18], &pP1[19]);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010A574);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010A7D4);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010A924);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010AA3C);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010B02C);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010B1E8);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010B550);

void func_8010B6BC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010B6C4);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010B848);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010B94C);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010BAD0);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010C03C);

INCLUDE_ASM("asm/nonmatchings/ovl/em38/em38", func_8010C3E8);

/* part position from the animation's bone offsets scaled by Rate */
void func_8010C9A4(ENEMY_WORK *pEm, PARTS_W *pP, LONG Rate)
{
    SHORT *pOfs = (SHORT *)(pEm->pKan_t_ptr + 2);

    pOfs += pP->Unk84 * 3;
    pP->Pos_x = (pOfs[0] * Rate) >> 12;
    pP->Pos_y = (pOfs[1] * Rate) >> 12;
    pP->Pos_z = (pOfs[2] * Rate) >> 12;
}
