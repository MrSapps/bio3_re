/*
 * SCA.C (RE2 name): room collision against the SCA shapes of the room and
 * the collision object list of oba.c. RE3 shapes are two corners (see sca.h);
 * shapes with Type & 0x1000 are rotated by 45 degrees (181 / 256 = cos 45).
 */
#include "sca.h"

/* GTE inline macros (PsyQ inline_c.h with the DMPSX-converted opcodes) */
#define gte_ldsxy3(r0, r1, r2) __asm__ volatile("mtc2 %0, $12; mtc2 %2, $14; mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")

void func_8004B7E8(ENEMY_WORK *pEm, struct tagCOL_OBJ *pObj, LONG Y)
{
    pEm->Ground = Y;
    pEm->nFloor = Y / -1800;
    if (!(pEm->Be_flg & 0x400)) {
        pEm->Base_pos_y = pEm->Pos_y = pEm->Ground;
    }
    if (D_800DEB58.Unk8 != 0) {
        pEm->Pos_y += pEm->Unk1B8;
        if (pEm->Ground < pEm->Pos_y) {
            pEm->Pos_y = pEm->Ground;
        }
    } else {
        pEm->Unk1B8 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004B874);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004BB4C);

ULONG func_8004C408(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3)
{
    SCA_WORK *pW;
    ULONG Rot;
    LONG X;
    LONG Z;
    LONG Half;
    LONG Dx;
    LONG Dz;
    LONG D;
    LONG Len;
    LONG Mx;
    LONG Mz;

    Rot = 0;
    pW = &D_800DEB58;
    if (R < 0) {
        do { /* (matching: raises R's register priority) */
            R = -R;
        } while (0);
        X = pW->X;
        Z = pW->Z;
        Rot = 1;
    } else {
        X = pObj->Pos_x;
        Z = pObj->Pos_z;
    }
    Half = (pT->X1 - pT->X0) >> 1;
    Dx = X - (pT->X0 + Half);
    Dz = Z - (pT->Z0 + Half);
    D = Half + R;
    Len = SquareRoot0(Dx * Dx + Dz * Dz);
    D -= Len;
    if (D <= 0) {
        return 0;
    }
    if (Dx > 0) {
        Dx = (Dx + 8) * D / Len;
    } else {
        Dx = (Dx - 8) * D / Len;
    }
    if (Dz > 0) {
        Dz = (Dz + 8) * D / Len;
    } else {
        Dz = (Dz - 8) * D / Len;
    }
    if (Rot) {
        Mx = (Dx + Dz) * 181 >> 8;
        Mz = (Dz - Dx) * 181 >> 8;
    } else {
        Mx = Dx;
        Mz = Dz;
    }
    pObj->Pos_x += Mx;
    pObj->Pos_z += Mz;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C57C);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C6EC);

ULONG func_8004C85C(COL_OBJ *pObj, LONG R, SCA_DATA *pT)
{
    SCA_WORK *pW;
    LONG X;
    LONG Z;
    LONG Dx;
    LONG Dz;
    ULONG Side;
    ULONG Still;
    LONG Hx;
    LONG Hz;
    LONG Near;

    pW = &D_800DEB58;
    X = pW->X;
    Dx = X - pW->Old_x;
    Side = (ULONG)Dx >> 31;
    Z = pW->Z;
    Dz = Z - pW->Old_z;
    Side |= (ULONG)Dz >> 30 & 2;
    Hx = pT->X1 + pObj->W0 - X + 1;
    Near = pT->X0 - pObj->W0 - X - 1;
    Dx |= Dz;
    Still = Dx == 0;
    if (!Still) {
        if (!(Side & 1)) {
            Hx = Near;
        }
    } else if (-Near < Hx) {
        Hx = Near;
    }
    Hz = pT->Z1 + pObj->D0 - Z + 1;
    Near = pT->Z0 - pObj->D0 - Z - 1;
    if (!Still) {
        if (!(Side & 2)) {
            Hz = Near;
        }
    } else if (-Near < Hz) {
        Hz = Near;
    }
    return func_8004CC54(pObj, Hx, Hz, pT);
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C960);

ULONG func_8004CC54(COL_OBJ *pObj, LONG Dx, LONG Dz, SCA_DATA *pT)
{
    SCA_WORK *pW;
    LONG Lim;
    LONG Mx;
    LONG Mz;
    LONG X;
    LONG Z;

    Lim = 400;
    pW = &D_800DEB58;
    if (pObj->W0 < 401) {
        Lim = pObj->W0;
    }
    Mz = 0;
    Mx = Mz;
    switch (((ULONG)(Dx ^ (pW->X - pW->Old_x)) >> 31) | ((ULONG)(Dz ^ (pW->Z - pW->Old_z)) >> 30 & 2)) {
    case 1:
        if (Lim < __builtin_abs(Dx)) {
            goto both;
        }
        Mx = Dx;
        break;
    case 2:
        if (__builtin_abs(Dz) <= Lim) {
            Mz = Dz;
            break;
        }
    case 3:
    both:
        if (__builtin_abs(Dx) < __builtin_abs(Dz)) {
            if (Lim * 2 < __builtin_abs(Dx)) {
                goto hit;
            }
            Mx = Dx;
        } else {
            if (Lim * 2 < __builtin_abs(Dz)) {
                goto hit;
            }
            Mz = Dz;
        }
        break;
    default:
        if (__builtin_abs(Dx) < __builtin_abs(Dz)) {
            Mx = Dx;
        } else {
            Mz = Dz;
        }
        break;
    hit:
        pObj->Hit_flg |= 0x100;
        return 1;
    }
    if (pT->Type & 0x1000) {
        X = (Mx + Mz) * 181 >> 8;
        Z = (Mz - Mx) * 181 >> 8;
    } else {
        X = Mx;
        Z = Mz;
    }
    pObj->Pos_x += X;
    pObj->Pos_z += Z;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004CE2C);

ULONG func_8004D194(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3)
{
    SCA_WORK *pW;
    ULONG Ret;
    ULONG Dir;
    ULONG Type;

    Ret = 0;
    pW = &D_800DEB58;
    Dir = (pT->Id >> 4) & 3;
    Type = pT->Type;
    if (Type & 0x100) {
        if (Dir & 2) {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z0;
            Ret = func_8004B874(pObj, R, Arg3);
        } else {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z1;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z1;
            Ret = func_8004B874(pObj, R, Arg3);
        }
    }
    if (Type & 0x200) {
        if (Dir & 1) {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X0 << 16) | (USHORT)pT->Z1;
            Ret |= func_8004B874(pObj, R, Arg3);
        } else {
            pW->Pt0 = (pT->X1 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z1;
            Ret |= func_8004B874(pObj, R, Arg3);
        }
    }
    if (Type & 0x400) {
        if (Dir == 0 || Dir == 3) {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z1;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z0;
            Ret |= func_8004B874(pObj, R, Arg3);
        } else {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z1;
            Ret |= func_8004B874(pObj, R, Arg3);
        }
    }
    return Ret;
}

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

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004D404);

ULONG func_8004D590(COL_OBJ *pObj, LONG R, ULONG P0)
{
    SCA_WORK *pW;
    LONG X;
    LONG Z;
    LONG Dx;
    LONG Dz;
    ULONG P1;
    ULONG P2;
    ULONG P3;
    ULONG Ret;
    ULONG Sign;
    LONG Opz[4];

    pW = &D_800DEB58;
    P2 = pW->Pt0;
    P3 = pW->Pt1;
    X = pObj->Pos_x;
    Z = pObj->Pos_z;
    Dx = pObj->Old_x - X;
    Dz = pObj->Old_z - Z;
    Ret = 0;
    if ((Dx | Dz) != 0) {
        P1 = (X << 16) | (Z & 0xFFFF);
        gte_ldsxy3(P0, P2, P3);
        gte_nclip();
        gte_stopz(&Opz[0]);
        gte_ldsxy3(P1, P2, P3);
        gte_nclip();
        gte_stopz(&Opz[1]);
        Sign = 0x80000000;
        if (((Opz[0] ^ Opz[1]) & Sign) != 0) {
            gte_ldsxy3(P2, P0, P1);
            gte_nclip();
            gte_stopz(&Opz[2]);
            gte_ldsxy3(P3, P0, P1);
            gte_nclip();
            gte_stopz(&Opz[3]);
            if (((Opz[2] ^ Opz[3]) & Sign) != 0) {
                Ret = 1;
                pObj->Pos_x += Dx;
                pObj->Pos_z += Dz;
            }
        }
    }
    return Ret;
}

LONG func_8004D6B0(void)
{
    return 0;
}

/* area bits of (X, Z) against the room's two split points (RE2 Sca_get_area twice) */
ULONG func_8004D6B8(LONG X, LONG Z)
{
    SCA_HEAD *pH = G.pRoom->pSca;
    LONG Sx;
    LONG Sz;
    ULONG Area;

    Sx = pH->Cx0;
    Sz = pH->Cz0;
    Area = (1 << ((ULONG)(X - Sx) >> 31)) << (((Z - Sz) & 0x80000000) >> 30);
    do { /* (matching: keeps the second pair of loads after the first area) */
        Sx = pH->Cx1;
        Sz = pH->Cz1;
    } while (0);
    return Area | (((1 << ((ULONG)(X - Sx) >> 31)) << (((Z - Sz) & 0x80000000) >> 30)) << 4);
}

INCLUDE_ASM("asm/nonmatchings/game/sca", Sca_get_ground);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004DB10);

ULONG func_8004DDC4(VECTOR *pPos, LONG Arg1, UCHAR *pTbl, UCHAR *pOut)
{
    USHORT *p;
    LONG x;
    LONG z;

    x = pPos->vx + 0x8000;
    z = pPos->vz + 0x8000;
    x >>= 12;
    z >>= 8;
    p = (USHORT *)pTbl + ((z & 0xFFF0) + x);
    pTbl += 0x200;
    if (*p == 0xFFFF) {
        return 0;
    }
    pTbl += *p;
    do {
        *pOut++ = *pTbl++;
    } while (*pTbl != 0xFF);
    *pOut = 0xFF;
    return 1;
}

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

LONG func_8004DEA8(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    return High;
}

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

LONG func_8004DF24(SCA_DATA *pT, VECTOR *pPos, LONG Unused, LONG High)
{
    LONG (*Tbl[4])(SCA_DATA *, VECTOR *, LONG) = {
        Sca_high_naname_a, Sca_high_naname_b, Sca_high_naname_c, Sca_high_naname_d
    };

    return Tbl[(pT->Id >> 4) & 3](pT, pPos, High);
}

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

LONG func_8004E10C(SCA_DATA *pT, VECTOR *pPos, LONG High, LONG Unused)
{
    LONG X;
    LONG Z;
    LONG Lo;
    LONG Hi;
    LONG Range;

    X = pPos->vx;
    Z = pPos->vz;
    Lo = -(pT->Floor_lo * 1800);
    Hi = pT->High + 1800;
    Range = Hi - Lo;
    switch ((pT->Id >> 4) & 3) {
    case 0:
        High = (X - pT->X0) * Range;
        High /= (pT->X1 - pT->X0);
        break;
    case 1:
        High = (pT->X1 - X) * Range;
        High /= (pT->X1 - pT->X0);
        break;
    case 2:
        High = (Z - pT->Z0) * Range;
        High /= (pT->Z1 - pT->Z0);
        break;
    case 3:
        High = (pT->Z1 - Z) * Range;
        High /= (pT->Z1 - pT->Z0);
        break;
    }
    High += Lo;
    if (High < Hi) {
        High = Hi;
    }
    if (Lo < High) {
        High = Lo;
    }
    return High;
}

LONG func_8004E230(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    LONG Dir;
    LONG Lo;
    LONG Hi;
    LONG H;

    Dir = (pT->Id >> 4) & 3;
    D_800DEB58.Unk8 = 1;
    if (Flg & 1) {
        return func_8004E10C(pT, pPos, Flg, High);
    }
    if (Flg & 2) {
        return pT->High;
    }
    Hi = pT->High + 1800;
    Lo = -(pT->Floor_lo * 1800);
    switch (Dir) {
    case 0:
        H = (pT->X0 - pPos->vx) / 540 * 450;
        break;
    case 1:
        H = (pPos->vx - pT->X1) / 540 * 450;
        break;
    case 2:
        H = (pT->Z0 - pPos->vz) / 540 * 450;
        break;
    case 3:
        H = (pPos->vz - pT->Z1) / 540 * 450;
        break;
    }
    H += Lo;
    if (H < Hi) {
        H = Hi;
    }
    if (Lo < H) {
        H = Lo;
    }
    return H;
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004E38C);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004E4C4);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004E5FC);

LONG func_8004E750(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    LONG R;
    LONG dx;
    LONG dz;
    LONG d;
    USHORT h;
    SHORT Sl;

    R = (pT->X1 - pT->X0) >> 1;
    dx = pPos->vx - (pT->X0 + R);
    dz = pPos->vz - (pT->Z0 + R);
    h = pT->High;
    Sl = h + pT->Floor_lo * 1800;
    d = SquareRoot0(dx * dx + dz * dz);
    if (R - d > 0) {
        return (SHORT)h - Sl * d / R;
    }
    return High;
}

INCLUDE_ASM("asm/nonmatchings/game/sca", Sca_ck_line);

/* Sca_ck_line-like scan of the collision object list: segment pA-pB against every
 * object (shape test D_8009E088[Id & 0xF]); with Flg & 1 the hit is confirmed in 3D */
LONG func_8004ED4C(VECTOR *pA, VECTOR *pB, ULONG Att, ULONG Flg)
{
    SCA_WORK *pW;
    COL_ENT *pEnt;
    SCA_DATA *pT;
    ULONG P0;
    ULONG P1;
    LONG Ret;

    Ret = 0;
    pT = (SCA_DATA *)G.pRoom->pSca;
    pW = &D_800DEB58;
    if (pT->X0 == 0) {
        return Ret;
    }
    pT = (SCA_DATA *)D_800DCED0.Ent;
    P0 = ((pA->vx >> 1) << 16) | ((pA->vz >> 1) & 0xFFFF);
    P1 = ((pB->vx >> 1) << 16) | ((pB->vz >> 1) & 0xFFFF);
    for (pEnt = (COL_ENT *)pT; ((COL_ENT *)pT)->C.All != -1 && Ret == 0; pEnt = (COL_ENT *)pT) {
        if (pT->Id & Att) {
            if (!(pEnt->pObj->Flg & 7)) {
                pT = &Tmp_Sca;
                func_80034E78(pEnt->pObj, pT);
            }
            Ret = D_8009E088[pT->Id & 0xF](P0, P1, pT);
            if (!(Flg & 1)) {
                if (Ret != 0) {
                    if (pEnt->Type < 3) {
                        pW->pEm_hit = pEnt->pObj->pEm;
                        pW->pEm_hit2 = NULL;
                    } else {
                        pW->pEm_hit2 = pEnt->pObj->pEm;
                        pW->pEm_hit = NULL;
                    }
                }
            } else if (Ret != 0) {
                Ret = func_8004F928(pA, pB, pT);
                if (Ret != 0) {
                    Ret = func_8004FBD4(pA, pB, pT);
                    if (Ret != 0) {
                        if (pEnt->Type < 3) {
                            pW->pEm_hit = pEnt->pObj->pEm;
                            pW->pEm_hit2 = NULL;
                        } else {
                            pW->pEm_hit2 = pEnt->pObj->pEm;
                            pW->pEm_hit = NULL;
                        }
                    }
                }
            }
        }
        pT = (SCA_DATA *)(pEnt + 1);
    }
    return Ret;
}

LONG func_8004EF74(ULONG P0, ULONG P1, ULONG P2, ULONG P3)
{
    LONG Opz[2];
    ULONG Ret;

    gte_ldsxy3(P0, P2, P3);
    gte_nclip();
    gte_stopz(&Opz[0]);
    gte_ldsxy3(P1, P2, P3);
    gte_nclip();
    gte_stopz(&Opz[1]);
    Ret = 0x80000000;
    if (((Opz[0] ^ Opz[1]) & Ret) == 0) {
        return 0;
    }
    gte_ldsxy3(P2, P0, P1);
    gte_nclip();
    gte_stopz(&Opz[0]);
    gte_ldsxy3(P3, P0, P1);
    gte_nclip();
    gte_stopz(&Opz[1]);
    return ((Opz[0] ^ Opz[1]) & Ret) != 0;
}

LONG func_8004F02C(void)
{
    return 0;
}

LONG func_8004F034(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    UCHAR Unused[0x20];

    return func_8004EF74(P0, P1, ((pT->X0 >> 1) << 16) | ((pT->Z0 >> 1) & 0xFFFF), ((pT->X1 >> 1) << 16) | ((pT->Z1 >> 1) & 0xFFFF));
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004F098);

LONG func_8004F258(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    LONG x0, x1, z0, z1;
    ULONG Flg;
    ULONG A, B;

    x0 = pT->X0 >> 1;
    z0 = pT->Z0 >> 1;
    Flg = pT->Type;
    z1 = pT->Z1 >> 1;
    x1 = pT->X1 >> 1;
    A = (x0 << 16) | (z0 & 0xFFFF);
    B = (x0 << 16) | (z1 & 0xFFFF);
    if ((Flg & 0x100) && func_8004EF74(P0, P1, A, B)) {
        return 1;
    }
    A = (x1 << 16) | (z1 & 0xFFFF);
    if ((Flg & 0x200) && func_8004EF74(P0, P1, A, B)) {
        return 1;
    }
    B = (x1 << 16) | (z0 & 0xFFFF);
    if ((Flg & 0x400) && func_8004EF74(P0, P1, A, B)) {
        return 1;
    }
    A = (x0 << 16) | (z0 & 0xFFFF);
    if ((Flg & 0x800) && func_8004EF74(P0, P1, A, B)) {
        return 1;
    }
    return 0;
}

LONG func_8004F3C8(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    ULONG Z0, Z1;
    ULONG A, B;
    LONG x0, x1, z0, z1;

    x0 = pT->X0 >> 1;
    z1 = pT->Z1 >> 1;
    x1 = pT->X1 >> 1;
    z0 = pT->Z0 >> 1;
    A = (x0 << 16) | (Z1 = z1 & 0xFFFF);
    B = (x1 << 16) | (Z0 = z0 & 0xFFFF);
    if (func_8004EF74(P0, P1, A, B)) {
        return 1;
    }
    A &= 0xFFFF0000;
    B &= 0xFFFF0000;
    return func_8004EF74(P0, P1, A | Z0, B | Z1) != 0;
}

LONG func_8004F498(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    LONG x0, x1, z0, z1, d;
    ULONG A, B, Z0, Z1;

    x0 = pT->X0 >> 1;
    z0 = pT->Z0 >> 1;
    d = (pT->Z1 - pT->Z0) >> 2;
    z1 = pT->Z1 >> 1;
    x1 = pT->X1 >> 1;
    if (func_8004EF74(P0, P1, (x0 << 16) | ((z0 + d) & 0xFFFF), (x1 << 16) | ((z0 + d) & 0xFFFF))) {
        return 1;
    }
    A = ((x0 + d) << 16) | (Z0 = z0 & 0xFFFF);
    B = ((x1 - d) << 16) | (Z1 = z1 & 0xFFFF);
    if (func_8004EF74(P0, P1, A, B)) {
        return 1;
    }
    A &= 0xFFFF0000;
    B &= 0xFFFF0000;
    return func_8004EF74(P0, P1, A | Z1, B | Z0) != 0;
}

LONG func_8004F5B4(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    LONG x0, x1, z0, z1, d;
    ULONG A, B;

    x0 = pT->X0 >> 1;
    x1 = pT->X1 >> 1;
    z0 = pT->Z0 >> 1;
    d = (pT->X1 - pT->X0) >> 2;
    z1 = pT->Z1 >> 1;
    if (func_8004EF74(P0, P1, ((x0 + d) << 16) | (z0 & 0xFFFF), ((x0 + d) << 16) | (z1 & 0xFFFF))) {
        return 1;
    }
    A = (x0 << 16) | ((z0 + d) & 0xFFFF);
    B = (x1 << 16) | ((z1 - d) & 0xFFFF);
    if (func_8004EF74(P0, P1, A, B)) {
        return 1;
    }
    A &= 0xFFFF;
    B &= 0xFFFF;
    return func_8004EF74(P0, P1, A | (x1 << 16), B | (x0 << 16)) != 0;
}

LONG func_8004F6CC(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    LONG x0, x1, z0, z1;
    LONG xm, dz;

    x0 = pT->X0 >> 1;
    x1 = pT->X1 >> 1;
    z0 = pT->Z0 >> 1;
    z1 = pT->Z1 >> 1;
    xm = x0 + ((x1 - x0) >> 1);
    dz = (z1 - z0) >> 1;
    if (func_8004EF74(P0, P1, (xm << 16) | (z0 & 0xFFFF), (xm << 16) | (z1 & 0xFFFF))) {
        return 1;
    }
    return func_8004EF74(P0, P1, (x0 << 16) | ((z0 + dz) & 0xFFFF), (x1 << 16) | ((z0 + dz) & 0xFFFF)) != 0;
}

LONG func_8004F7A8(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    LONG x0, x1, z0, z1;
    ULONG Flg;
    LONG Ret;

    Flg = pT->Id & 0x30;
    x0 = pT->X0 >> 1;
    x1 = pT->X1 >> 1;
    z0 = pT->Z0 >> 1;
    z1 = pT->Z1 >> 1;
    if (Flg & 0x20) {
        Ret = func_8004EF74(P0, P1, (x0 << 16) | (z0 & 0xFFFF), (x1 << 16) | (z0 & 0xFFFF));
    } else {
        Ret = func_8004EF74(P0, P1, (x0 << 16) | (z1 & 0xFFFF), (x1 << 16) | (z1 & 0xFFFF));
    }
    if (Ret) {
        return 1;
    }
    if (Flg & 0x10) {
        Ret = func_8004EF74(P0, P1, (x0 << 16) | (z0 & 0xFFFF), (x0 << 16) | (z1 & 0xFFFF));
    } else {
        Ret = func_8004EF74(P0, P1, (x1 << 16) | (z0 & 0xFFFF), (x1 << 16) | (z1 & 0xFFFF));
    }
    if (Ret) {
        return 1;
    }
    if (Flg == 0 || Flg == 0x30) {
        if (func_8004EF74(P0, P1, (x0 << 16) | (z1 & 0xFFFF), (x1 << 16) | (z0 & 0xFFFF))) {
            return 2;
        }
    } else {
        if (func_8004EF74(P0, P1, (x0 << 16) | (z0 & 0xFFFF), (x1 << 16) | (z1 & 0xFFFF))) {
            return 2;
        }
    }
    return 0;
}

LONG func_8004F928(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
{
    LONG ax, ay, bx, by;
    LONG hi, lo, x0, x1;
    ULONG P0, P1;
    ULONG A, B;
    LONG Id;
    LONG Shape;

    ax = pA->vx;
    ay = pA->vy;
    bx = pB->vx;
    by = pB->vy;
    hi = pT->High;
    Id = pT->Id;
    ax >>= 1;
    ay >>= 1;
    bx >>= 1;
    by >>= 1;
    hi >>= 1;
    Shape = Id & 0xF;
    P0 = (ax << 16) | (ay & 0xFFFF);
    P1 = (bx << 16) | (by & 0xFFFF);
    x0 = pT->X0 >> 1;
    x1 = pT->X1 >> 1;
    lo = -(pT->Floor_lo * 1800) >> 1;
    if (Shape >= 9) {
        hi += 900;
        if (!(Id & 0x20)) {
            if (Id & 0x10) {
                A = (x0 << 16) | (lo & 0xFFFF);
                B = (x0 << 16) | (hi & 0xFFFF);
                if (func_8004EF74(P0, P1, A, B)) {
                    return 1;
                }
                A &= 0xFFFF;
                if (func_8004EF74(P0, P1, A | (x1 << 16), B)) {
                    return 1;
                }
            } else {
                A = (x1 << 16) | (lo & 0xFFFF);
                B = (x1 << 16) | (hi & 0xFFFF);
                if (func_8004EF74(P0, P1, A, B)) {
                    return 1;
                }
                A &= 0xFFFF;
                if (func_8004EF74(P0, P1, A | (x0 << 16), B)) {
                    return 1;
                }
            }
            if (func_8004EF74(P0, P1, (x0 << 16) | (lo & 0xFFFF), (x1 << 16) | (lo & 0xFFFF))) {
                return 1;
            }
        }
        return 0;
    }
    if (ax < x0 || bx < x0 || x1 < ax || x1 < bx || ay < hi || by < hi || lo < ay || lo < by) {
        A = (x0 << 16) | (lo & 0xFFFF);
        B = (x0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= x1 << 16;
        A |= x1 << 16;
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A = (x0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return func_8004EF74(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
    }
    return 1;
}

LONG func_8004FBD4(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
{
    LONG az, ay, bz, by;
    LONG hi, lo, z0, z1;
    ULONG P0, P1;
    ULONG A, B;
    LONG Id;
    LONG Shape;

    az = pA->vz;
    ay = pA->vy;
    bz = pB->vz;
    by = pB->vy;
    hi = pT->High;
    Id = pT->Id;
    az >>= 1;
    ay >>= 1;
    bz >>= 1;
    by >>= 1;
    hi >>= 1;
    Shape = Id & 0xF;
    P0 = (az << 16) | (ay & 0xFFFF);
    P1 = (bz << 16) | (by & 0xFFFF);
    z0 = pT->Z0 >> 1;
    z1 = pT->Z1 >> 1;
    lo = -(pT->Floor_lo * 1800) >> 1;
    if (Shape >= 9) {
        hi += 900;
        if (Id & 0x20) {
            if (Id & 0x10) {
                B = (z0 << 16) | (hi & 0xFFFF);
                if (func_8004EF74(P0, P1, (z0 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
                if (func_8004EF74(P0, P1, (z1 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
            } else {
                B = (z1 << 16) | (hi & 0xFFFF);
                if (func_8004EF74(P0, P1, (z1 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
                if (func_8004EF74(P0, P1, (z0 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
            }
            if (func_8004EF74(P0, P1, (z0 << 16) | (lo & 0xFFFF), (z1 << 16) | (lo & 0xFFFF))) {
                return 1;
            }
        }
        return 0;
    }
    if (az < z0 || bz < z0 || z1 < az || z1 < bz || ay < hi || by < hi || lo < ay || lo < by) {
        A = (z0 << 16) | (lo & 0xFFFF);
        B = (z0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= z1 << 16;
        A |= z1 << 16;
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A = (z0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return func_8004EF74(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
    }
    return 1;
}

LONG func_8004FE70(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
{
    LONG ax, ay, bx, by;
    LONG hi, lo, x0, x1;
    ULONG P0, P1;
    ULONG A, B;

    ax = pA->vx;
    ay = pA->vy;
    bx = pB->vx;
    by = pB->vy;
    ax >>= 1;
    ay >>= 1;
    bx >>= 1;
    by >>= 1;
    P0 = (ax << 16) | (ay & 0xFFFF);
    P1 = (bx << 16) | (by & 0xFFFF);
    x0 = pT->X0 >> 1;
    x1 = pT->X1 >> 1;
    lo = -(pT->Floor_lo * 1800) >> 1;
    hi = pT->High >> 1;
    if (ax < x0 || bx < x0 || x1 < ax || x1 < bx || ay < hi || by < hi || lo < ay || lo < by) {
        A = (x0 << 16) | (lo & 0xFFFF);
        B = (x0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= x1 << 16;
        A |= x1 << 16;
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A = (x0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return func_8004EF74(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
    }
    return 1;
}

LONG func_8005003C(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
{
    LONG az, ay, bz, by;
    LONG hi, lo, z0, z1;
    ULONG P0, P1;
    ULONG A, B;

    az = pA->vz;
    ay = pA->vy;
    bz = pB->vz;
    by = pB->vy;
    az >>= 1;
    ay >>= 1;
    bz >>= 1;
    by >>= 1;
    P0 = (az << 16) | (ay & 0xFFFF);
    P1 = (bz << 16) | (by & 0xFFFF);
    z0 = pT->Z0 >> 1;
    z1 = pT->Z1 >> 1;
    lo = -(pT->Floor_lo * 1800) >> 1;
    hi = pT->High >> 1;
    if (az < z0 || bz < z0 || z1 < az || z1 < bz || ay < hi || by < hi || lo < ay || lo < by) {
        A = (z0 << 16) | (lo & 0xFFFF);
        B = (z0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= z1 << 16;
        A |= z1 << 16;
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A = (z0 << 16) | (hi & 0xFFFF);
        if (func_8004EF74(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return func_8004EF74(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
    }
    return 1;
}

void Sca_search(ULONG Type, ULONG Id, ULONG Floor)
{
    SCA_DATA *pData;
    SCA_DATA *pEnd;
    ULONG Cnt;
    SCA_DATA **ppList;

    pData = (SCA_DATA *)G.pRoom->pSca;
    Cnt = 0;
    ppList = G.Sca_list;
    *ppList = NULL;
    pEnd = pData + pData->X0;
    if (pData->X0 == 0) {
        return;
    }
    pData++;
    goto test;
loop:
        if ((pData->Type & Type) && (pData->Id & Id) && Floor >= pData->Floor_lo && pData->Floor_hi >= Floor) {
            *ppList++ = pData;
            Cnt++;
            *ppList = NULL;
            if (Cnt == 32) {
                return;
            }
        }
        pData++;
test:
    if (pData != pEnd) goto loop;
}

/* RE2 Sca_get_dir: side of the shape the point is on (45 degree shapes are rotated first) */
USHORT func_800502B8(VECTOR *pPos, SCA_DATA *pT)
{
    VECTOR Pos;
    VECTOR *pP;
    ULONG Dir;
    LONG Dx;
    LONG Dz;
    LONG Slope;

    Dir = 0;
    pP = &Pos;
    if (pT->Type & 0x1000) {
        Dx = pPos->vx - pT->X0;
        Dz = pPos->vz - pT->Z0;
        Pos.vx = (Dx - Dz) * 181 >> 8;
        Pos.vz = (Dx + Dz) * 181 >> 8;
        Pos.vx += pT->X0;
        Pos.vz += pT->Z0;
    } else {
        Pos.vx = pPos->vx;
        Pos.vz = pPos->vz;
    }
    Slope = (pT->Z1 - pT->Z0) * (pP->vx - pT->X0) / (pT->X1 - pT->X0);
    if (Slope < pP->vz - pT->Z0) {
        Dir |= 2;
        if (-Slope < pP->vz - pT->Z1) {
            Dir |= 1;
        }
    } else if (-Slope > pP->vz - pT->Z1) {
        Dir |= 1;
    }
    return Dir;
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_800503E8);

/* is the object inside the extent of the wall it hit, along the wall's axis (func_800502B8 = RE2 Sca_get_dir) */
LONG func_80050524(COL_OBJ *pObj, SCA_DATA *pT)
{
    LONG Pos;
    LONG Min;
    LONG Max;

    if (func_800502B8(POS_V(pObj), pT) & 1) {
        Min = pT->X0;
        Max = pT->X1;
        Pos = pObj->Pos_x;
        Min += pObj->W0;
        Max -= pObj->W0;
    } else {
        Min = pT->Z0;
        Max = pT->Z1;
        Pos = pObj->Pos_z;
        Min += pObj->W0;
        Max -= pObj->W0;
    }
    if (Pos < Min) {
        return 0;
    }
    return Pos <= Max;
}
