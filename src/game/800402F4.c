#include "pl_wep.h"
#include "sca.h"

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800402F4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80040764);

void pl_mv05_w0b(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w0b_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", pl_mv05_w0b_t2);

void Pl_wep_ready_reset(PLAYER_WORK *pEm)
{
    PARTS_W *pParts = &pEm->pSin_parts_ptr[22];

    pParts->Unk94 = 0;
    pParts->Unk86 = 0;
    pParts->Unk96 = 0;
}

void Pl_wep_recoil_6(PLAYER_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)((UCHAR)pEm->Type - 6) < 4) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Unk96 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl0[pWep->Unk86];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Unk96 = 0xFF;
                return;
            }
            pWep->Unk86++;
            RotMatrix_gte((SVECTOR *)&pParts[22].Cdir_x, (MATRIX *)pParts[22].M);
        }
    }
}

void Pl_wep_recoil_10(PLAYER_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0x10) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Unk96 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl1[pWep->Unk86];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Unk96 = 0xFF;
                return;
            }
            pWep->Unk86++;
            RotMatrix_gte((SVECTOR *)&pParts[22].Cdir_x, (MATRIX *)pParts[22].M);
        }
    }
}


void Pl_wep_flag_set(PLAYER_WORK *pEm)
{
    pEm->pSin_parts_ptr[22].Unk86 = 1;
}

void Pl_wep_spin_0b(PLAYER_WORK *pEm)
{
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0xB) {
        pWep = &pEm->pSin_parts_ptr[22];
        if (pWep->Unk86 == 0) {
            pWep->Unk96 -= 8;
            if (pWep->Unk96 < 0) {
                pWep->Unk96 = 0;
            }
        } else {
            pWep->Unk96 += 0x20;
            if (pWep->Unk96 > 0x200) {
                pWep->Unk96 = 0x200;
            }
        }
        pWep->Cdir_y += pWep->Unk96;
        RotMatrix((SVECTOR *)&pWep->Cdir_x, (MATRIX *)pWep->M);
        pWep->Unk86 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", pl_w01_t2_move);


INCLUDE_ASM("asm/nonmatchings/game/800402F4", pl_w02_t2_move);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800410F8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80041594);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800418CC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80041A80);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80041C34);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80041DE8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80041F9C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800421F8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80042474);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80042660);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800427D8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80042AE4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80042E34);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80043328);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800434F8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800439F8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80043BE4);

void Weapon_cls(PLAYER_WORK *pEm)
{
    ULONG *pTmd = Pl_wep_tmd;
    PARTS_W *pP = pEm->pSin_parts_ptr;
    ULONG n;

    pP[4].pTmd = pP[21].pTmd;
    n = pTmd[-1];
    pEm->Parts_num = pEm->Parts_num - n;
}

void Weapon_parts_clr(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pEm->pSin_parts_ptr[4].pTmd = pEm->pSin_parts_ptr[21].pTmd;
    pP = &pEm->pSin_parts_ptr[21];
    i = Pl_wep_tmd[-1];
    do {
        pP->Be_flg = 0;
        pP->UnkA2 = 0;
        pP++;
    } while (--i != 0);
}

void Pl_weapon_ch_init(PLAYER_WORK *pEm)
{
    pEm->pSin_parts_ptr[4].pTmd = Pl_wep_tmd;
    Pl_weapon_init[(UCHAR)pEm->Type](pEm);
}



void Non_init(PLAYER_WORK *pEm)
{
}

void Plw02_init(PLAYER_WORK *pEm)
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

INCLUDE_ASM("asm/nonmatchings/game/800402F4", Plw_init_43FF0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", Plw_init_44054);

void Plw_init_440AC(PLAYER_WORK *pEm)
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

void Plw_init_440F4(PLAYER_WORK *pEm)
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

void Plw_init_44134(PLAYER_WORK *pEm)
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

void Plw_init_4417C(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 1;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = -0x1D;
    pO->Pos_y = 0x7E;
    pO->Pos_z = 0xD;
    pO->Unk96 = 0xFF;
    pO->UnkA2 |= 0xB;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800441CC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80044420);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800445C8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004470C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004479C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80044804);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80044E90);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80044F34);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80045094);

void Pl_wep_eff_init(PLAYER_WORK *pEm)
{
    Pl_wep_eff_parts_init(pEm);
    func_80046C90();
}

void func_800452E0(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800452E8);

void func_80045514(void) {
}

LONG Pl_wep_eff_parts_init(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pP = &pEm->pSin_parts_ptr[15];
    i = 6;
    do {
        pP->Unk74 = 0xFF;
        pP->Be_flg = 0;
        pP->Unk90 = 0;
        func_8001B894(0x103, &pP->Workm);
        func_8001B894(0xC03, &pP->Workm);
        func_8001B894(0x105, &pP->Workm);
        func_8001B894(0x805, &pP->Workm);
        pP++;
    } while (--i != 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800455B4);

UCHAR Pl_wep_eff_parts_cnt(void)
{
    LONG i;
    UCHAR Cnt;
    PARTS_W *pP;
    UCHAR Unused[16];

    i = 6;
    Cnt = 0;
    pP = &G.Pl_pSin_parts_ptr[15];
    do {
        if (pP->Unk74 != 0xFF) {
            Cnt++;
        }
        pP++;
    } while (--i != 0);
    return Cnt;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800457A4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80045950);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046A0C);

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

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046B94);

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

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046C90);

void Pl_wep_poly_set(PL_WEP_POLY *pWp, SHORT Unk2, LONG Unk4)
{
    pWp->Be_flg = 1;
    pWp->Unk4 = Unk4;
    pWp->Unk2 = Unk2;
}

void Pl_wep_poly_rgb(PL_WEP_POLY *pWp, ULONG Rgb)
{
    *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[0].r0 |= Rgb;
    *(ULONG *)&pWp->Poly[1].r0 |= Rgb;
}


INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046DF8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046E00);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046EC4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800470F4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800472EC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047860);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047BD0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047DD0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047F70);

void Quake_add(LONG X, LONG Y)
{
    G.Unk7860 += Y;
    G.Unk2118 += Y;
    G.Unk211A += X;
    if (G.Unk2116 == 0) {
        G.Unk2116 += 0x5A;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004834C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048520);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048630);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800486C8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800486F0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048844);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048974);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048A3C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048E34);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048FF4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049218);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800493EC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049898);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800498F0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049A50);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049BA8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049E80);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049ED8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049F80);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004A080);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004A7F4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004A8BC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004A934);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004AC54);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004AD94);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004AF04);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004B374);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004B7E8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004B874);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004BB4C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C408);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C57C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C6EC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C85C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C960);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004CC54);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004CE2C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D194);

ULONG Sca_ck_circle(VECTOR *pPos, LONG R, SCA_DATA *pT)
{
    LONG Half;
    LONG Cx;
    LONG Cz;
    LONG Dx;
    LONG Dz;

    Half = (pT->X1 - pT->X0) >> 1;
    Cx = pT->X0 + Half;
    Cz = pT->Z0 + Half;
    Half += R;
    Dx = pPos->vx - Cx;
    Dz = pPos->vz - Cz;
    if ((Dx != 0 || Dz != 0) && SquareRoot0(Dx * Dx + Dz * Dz) < Half) {
        return 2;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D404);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D590);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D6B0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D6B8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D720);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004DB10);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004DDC4);

ULONG Sca_ck_circle2(VECTOR *pPos, LONG R, SCA_DATA *pT)
{
    LONG Half;
    LONG Dx;
    LONG Dz;
    LONG Lim;

    Half = (pT->X1 - pT->X0) >> 1;
    Dx = pPos->vx - (pT->X0 + Half);
    Dz = pPos->vz - (pT->Z0 + Half);
    Lim = Half + R;
    return SquareRoot0(Dx * Dx + Dz * Dz) < Lim;
}

LONG func_8004DEA8(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004DEA8);

LONG Sca_high_box(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    ULONG Type = pT->Type;

    if (!(Flg & 8) || !(Type & 0xF00)) {
        High = pT->High;
    }
    return High;
}

LONG Sca_high_circle(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    if (Sca_ck_circle(pPos, 0, pT)) {
        High = pT->High;
    }
    return High;
}




INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004DF24);

LONG Sca_high_naname_a(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X0;
    LONG Z = pT->Z1;

    if ((pPos->vx - X) * (pT->Z0 - Z) / (pT->X1 - X) < pPos->vz - Z) {
        return pT->High;
    }
    return High;
}

LONG Sca_high_naname_b(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X1;
    LONG Z = pT->Z1;

    if ((pPos->vx - X) * (pT->Z0 - Z) / (pT->X0 - X) < pPos->vz - Z) {
        return pT->High;
    }
    return High;
}

LONG Sca_high_naname_c(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X0;
    LONG Z;

    Z = pT->Z1 - pT->Z0;
    if (pPos->vz - pT->Z0 < (pPos->vx - X) * Z / (pT->X1 - X)) {
        return pT->High;
    }
    return High;
}

LONG Sca_high_naname_d(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X1;
    LONG Z1 = pT->Z1;
    LONG Z = pT->Z0;

    if (pPos->vz - Z < (pPos->vx - X) * (Z - Z1) / (X - pT->X0)) {
        return pT->High;
    }
    return High;
}



INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E10C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E230);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E38C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E4C4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E5FC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E750);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E830);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004ED4C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004EF74);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F02C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F034);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F098);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F258);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F3C8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F498);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F5B4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F6CC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F7A8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F928);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004FBD4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004FE70);
