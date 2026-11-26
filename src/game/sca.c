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

/* store a ground height in the character (Ground, nFloor, Pos_y unless Be_flg 0x400; keeps the stairs offset Unk1B8 while Sca_work.Unk8 is set); oba.c passes the Sca_get_ground result */
void Sca_set_ground(ENEMY_WORK *pEm, struct tagCOL_OBJ *pObj, LONG Y)
{
    pEm->Ground = Y;
    pEm->nFloor = Y / -1800;
    if (!(pEm->Be_flg & 0x400)) {
        pEm->Base_pos_y = pEm->Pos_y = pEm->Ground;
    }
    if (Sca_work.Unk8 != 0) {
        pEm->Pos_y += pEm->Unk1B8;
        if (pEm->Ground < pEm->Pos_y) {
            pEm->Pos_y = pEm->Ground;
        }
    } else {
        pEm->Unk1B8 = 0;
    }
}

/* push a COL_OBJ off the wall segment Sca_work.Pt0-Pt1 (GTE nclip side test, Sca_ck_pass_line first), then circle-push off both end points via Tmp_Sca/Sca_hit_circle; building block of the edge-based hit routines (no RE2 equivalent) */
ULONG Sca_hit_line(COL_OBJ *pObj, LONG R, ULONG P0)
{
    SCA_WORK *pW;
    LONG P1;
    LONG P2;
    LONG P3;
    LONG P;
    LONG Nx;
    LONG V0;
    LONG V1;
    LONG Nz;
    LONG Len;
    LONG D;
    ULONG Ret;
    ULONG Rot;
    LONG Opz[4];

    Rot = 0;
    pW = &Sca_work;
    P2 = pW->Pt0;
    P3 = pW->Pt1;
    Ret = Sca_ck_pass_line(pObj, R, P0);
    V1 = (SHORT)P3;
    V0 = P2 << 16; /* (SHORT)P2 in two statements: V0 is then set three times and global, which gives a0 */
    V0 >>= 16;
    Nx = (V1 - V0) >> 1;
    V1 = P3 >> 16;
    V0 = P2 >> 16;
    Nz = -((V1 - V0) >> 1);
    P1 = (pObj->Pos_x << 16) | (USHORT)pObj->Pos_z;
    Len = SquareRoot0(Nx * Nx + Nz * Nz);
    if (Len != 0) {
        if (R < 0) {
            R = -R;
            Rot = 1;
        }
        gte_ldsxy3(P1, P2, P3);
        gte_nclip();
        gte_stopz(&Opz[0]);
        Nx *= R;
        Nx /= Len;
        Nz *= R;
        Nz /= Len;
        if (Opz[0] > 0) {
            Nx = -Nx;
            Nz = -Nz;
        }
        do { P = P1 + ((Nx << 16) | (Nz & 0xFFFF)); } while (0); /* extra P1 ref: P1 above P2 in global-alloc */
        gte_ldsxy3(P, P2, P3);
        gte_nclip();
        gte_stopz(&Opz[1]);
        if (((Opz[0] ^ Opz[1]) & 0x80000000) != 0) {
            gte_ldsxy3(P2, P1, P);
            gte_nclip();
            gte_stopz(&Opz[2]);
            gte_ldsxy3(P3, P1, P);
            gte_nclip();
            gte_stopz(&Opz[3]);
            if (((Opz[2] ^ Opz[3]) & 0x80000000) != 0) {
                D = Opz[1] >> 8;
                Nx *= D;
                Nx /= (Opz[0] >> 8) + (-Opz[1] >> 8);
                Nz *= D;
                Nz /= (Opz[0] >> 8) + (-Opz[1] >> 8);
                Ret |= 1;
                pObj->Pos_x += Nx;
                pObj->Pos_z += Nz;
            }
        }
        if (!Rot) {
            Tmp_Sca.X0 = P2 >> 16;
            Tmp_Sca.Z0 = P2;
            Tmp_Sca.X1 = P2 >> 16;
            Tmp_Sca.Z1 = P2;
            Ret |= Sca_hit_circle(pObj, R, &Tmp_Sca, P1);
            Tmp_Sca.X0 = P3 >> 16;
            Tmp_Sca.Z0 = P3;
            Tmp_Sca.X1 = P3 >> 16;
            Tmp_Sca.Z1 = P3;
            Ret |= Sca_hit_circle(pObj, R, &Tmp_Sca, P1);
        }
        return Ret;
    }
}

/* hit routine for shape 4 (diamond): Sca_rtn_tbl[4]; RE2 Sca_hit_hishi (same shape slot as Sca_high_hishi / Sca_line_hishi) */
INCLUDE_ASM("asm/nonmatchings/game/sca", Sca_hit_hishi);

/* hit routine for shape 0: push the object out of the circle (R < 0 = use the 45-degree rotated position in Sca_work and rotate the push back); RE2 Sca_hit_circle */
ULONG Sca_hit_circle(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3)
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
    pW = &Sca_work;
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

/* hit routine for shape 2: capsule along X, middle part as Sca_hit_box, rounded ends as Sca_hit_circle on Tmp_Sca; RE2 Sca_hit_koban_x */
ULONG Sca_hit_koban_x(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3)
{
    SCA_WORK *pW;
    SCA_DATA *pC;
    LONG X0;
    LONG Z0;
    SHORT D;
    LONG Half;
    SHORT Cx1;  /* X1 - Half for the zone test, then a temp of the end-circle stores */
    SHORT Cx0;
    LONG X;
    LONG Z;
    ULONG Zone;

    Z0 = pT->Z0;
    X0 = pT->X0;
    pW = &Sca_work;
    D = pT->Z1 - Z0;
    Half = D >> 1;
    Cx1 = pT->X1 - Half;
    Cx0 = X0 + Half;
    if (pT->Type & 0x1000) {
        X = pW->X;
        Z = pW->Z;
    } else {
        X = pObj->Pos_x;
        Z = pObj->Pos_z;
    }
    Zone = ((ULONG)(X - Cx1) >> 30) & 2;
    Zone |= (ULONG)(X - Cx0) >> 31;
    switch (Zone) {
    case 0:
        pC = &Tmp_Sca;
        pC->X1 = pT->X1;
        Cx1 = pT->X1 - D;
        pC->Z1 = D + Z0;
        pC->Z0 = Z0;
        Sca_work.X = X;
        Sca_work.Z = Z;
        pC->X0 = Cx1;
        goto Circle;
    case 2:
        do { return Sca_hit_box(pObj, R, pT, Arg3); } while (0); /* extra pT ref (pT above D in global-alloc) */
    case 3:
        pC = &Tmp_Sca;
        pC->X1 = D + X0;
        Cx1 = D + Z0;
        do { pC->X0 = X0; } while (0); /* extra X0 ref (X0 above X in global-alloc) */
        pC->Z1 = Cx1;
        pC->Z0 = Z0;
        Sca_work.X = X;
        Sca_work.Z = Z;
    Circle:
        if (pT->Type & 0x1000) {
            R = -R;
        }
        return Sca_hit_circle(pObj, R, pC, Arg3);
    }
}

/* hit routine for shape 3: same along Z; RE2 Sca_hit_koban_z */
ULONG Sca_hit_koban_z(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3)
{
    SCA_WORK *pW;
    SCA_DATA *pC;
    SCA_WORK *pS;   /* Sca_work for the case-3 stores (own pointer: its la is issued between the X1 and Z0 stores) */
    LONG Z0;
    LONG X0;
    SHORT D;
    LONG Half;
    SHORT Cz1;  /* Z1 - Half for the zone test, then a temp of the end-circle stores */
    SHORT Cz0;
    LONG Z;
    LONG X;
    ULONG Zone;

    X0 = pT->X0;
    Z0 = pT->Z0;
    pW = &Sca_work;
    D = pT->X1 - X0;
    Half = D >> 1;
    Cz1 = pT->Z1 - Half;
    Cz0 = Z0 + Half;
    if (pT->Type & 0x1000) {
        X = pW->X;
        Z = pW->Z;
    } else {
        X = pObj->Pos_x;
        Z = pObj->Pos_z;
    }
    Zone = ((ULONG)(Z - Cz1) >> 30) & 2;
    Zone |= (ULONG)(Z - Cz0) >> 31;
    switch (Zone) {
    case 0:
        pC = &Tmp_Sca;
        pC->Z1 = pT->Z1;
        Cz1 = pT->Z1 - D;
        pC->X1 = D + X0;
        pC->X0 = X0;
        Sca_work.X = X;
        Sca_work.Z = Z;
        pC->Z0 = Cz1;
        goto Circle;
    case 2:
        do { return Sca_hit_box(pObj, R, pT, Arg3); } while (0); /* extra pT ref (pT above D in global-alloc) */
    case 3:
        pC = &Tmp_Sca;
        pC->Z1 = D + Z0;
        pC->X1 = D + X0;
        pS = &Sca_work;
        do { do { pC->Z0 = Z0; } while (0); } while (0); /* loop depth 3: Z0 above Z in global-alloc */
        pC->X0 = X0;
        pS->X = X;
        pS->Z = Z;
    Circle:
        if (pT->Type & 0x1000) {
            R = -R;
        }
        return Sca_hit_circle(pObj, R, pC, Arg3);
    }
}

/* box hit when the old position is already inside: push to the nearer edge on each axis (direction from the move), then Sca_hosei_box; RE2 Sca_hit_box2 */
ULONG Sca_hit_box2(COL_OBJ *pObj, LONG R, SCA_DATA *pT)
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

    pW = &Sca_work;
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
    return Sca_hosei_box(pObj, Hx, Hz, pT);
}

/* hit routine for shapes 1/5/7/8: box push (Type & 0xF00 == 0 only sets Hit_flg 0x80), inside case goes to Sca_hit_box2; RE2 Sca_hit_box */
INCLUDE_ASM("asm/nonmatchings/game/sca", Sca_hit_box);

/* apply the box push Dx/Dz: pick the axis from the move direction with a slide limit (W0 capped at 400, else Hit_flg 0x100), rotate back for 45-degree shapes; RE2 Sca_hosei_box */
ULONG Sca_hosei_box(COL_OBJ *pObj, LONG Dx, LONG Dz, SCA_DATA *pT)
{
    SCA_WORK *pW;
    LONG Lim;
    LONG Mx;
    LONG Mz;
    LONG X;
    LONG Z;

    Lim = 400;
    pW = &Sca_work;
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

/* hit routine for shapes 9/10 (slope, stairs): Sca_hit_line on each edge flagged in Type 0x100..0x800 (stairs side walls dropped when the object is within the width), RE2 Sca_hit_slope role */
INCLUDE_ASM("asm/nonmatchings/game/sca", Sca_hit_slope);

/* hit routine for shape 6 (right triangle, corner = (Id >> 4) & 3): Sca_hit_line on the flagged legs / hypotenuse; one routine for RE2 Sca_hit_naname_a..d */
ULONG Sca_hit_naname(COL_OBJ *pObj, LONG R, SCA_DATA *pT, ULONG Arg3)
{
    SCA_WORK *pW;
    ULONG Ret;
    ULONG Dir;
    ULONG Type;

    Ret = 0;
    pW = &Sca_work;
    Dir = (pT->Id >> 4) & 3;
    Type = pT->Type;
    if (Type & 0x100) {
        if (Dir & 2) {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z0;
            Ret = Sca_hit_line(pObj, R, Arg3);
        } else {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z1;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z1;
            Ret = Sca_hit_line(pObj, R, Arg3);
        }
    }
    if (Type & 0x200) {
        if (Dir & 1) {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X0 << 16) | (USHORT)pT->Z1;
            Ret |= Sca_hit_line(pObj, R, Arg3);
        } else {
            pW->Pt0 = (pT->X1 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z1;
            Ret |= Sca_hit_line(pObj, R, Arg3);
        }
    }
    if (Type & 0x400) {
        if (Dir == 0 || Dir == 3) {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z1;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z0;
            Ret |= Sca_hit_line(pObj, R, Arg3);
        } else {
            pW->Pt0 = (pT->X0 << 16) | (USHORT)pT->Z0;
            pW->Pt1 = (pT->X1 << 16) | (USHORT)pT->Z1;
            Ret |= Sca_hit_line(pObj, R, Arg3);
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

/* push pObj out of the circle of radius R around another object at (X, Z) (oba.c): first the
   object's move is tested against the circle's diameter across the move direction (Sca_ck_pass_line),
   then the circle push of Sca_hit_circle */
LONG Sca_hit_obj_circle(COL_OBJ *pObj, LONG W, SHORT X, SHORT Z, SHORT R, ULONG Old)
{
    SCA_WORK *pW;
    LONG Dz;
    LONG Dx;
    LONG Len;
    LONG Ox;
    LONG Oz;
    LONG Ret;

    pW = &Sca_work;
    Dz = pObj->Pos_z - pObj->Old_z;
    Dx = pObj->Old_x - pObj->Pos_x;
    Ret = 0;
    Len = SquareRoot0(Dz * Dz + Dx * Dx);
    if (Len != 0) {
        Dz *= R;
        Ox = Dz / Len;
        Dx *= R;
        Oz = Dx / Len;
        pW->Pt0 = ((X + Ox) << 16) | ((Z + Oz) & 0xFFFF);
        pW->Pt1 = ((X - Ox) << 16) | ((Z - Oz) & 0xFFFF);
        Ret = Sca_ck_pass_line(pObj, W, Old);
    }
    Tmp_Sca.X0 = X - R;
    Tmp_Sca.Z0 = Z - R;
    Tmp_Sca.X1 = X + R;
    Tmp_Sca.Z1 = Z + R;
    Ret |= Sca_hit_circle(pObj, W, &Tmp_Sca, Old);
    return Ret;
}

/* did the move Old->Pos cross the segment Sca_work.Pt0-Pt1 (GTE nclip); if so add (Old - Pos) back and return 1 */
ULONG Sca_ck_pass_line(COL_OBJ *pObj, LONG R, ULONG P0)
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

    pW = &Sca_work;
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

/* hit routine for shapes 11/12: returns 0 */
LONG Sca_hit_dummy(void)
{
    return 0;
}

/* area bits of (X, Z) against the room's two split points (RE2 Sca_get_area twice) */
ULONG Sca_get_area(LONG X, LONG Z)
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

/* ground height under pPos: the lowest top (Sca_high_tbl[Flg & 8][shape]) of the room shapes in pPos's grid block
 * (Sca_get_block) whose Id matches Att, then the collision objects (Sca_get_ground_obj); Sca_work.Gnd keeps the
 * Id/Type of the shape found (stairs, shape 10, win over a higher floor box only when Gnd says so) */
LONG Sca_get_ground(LONG *pPos, ULONG R, ULONG Att, ULONG Flg)
{
    SCA_DATA *pT;
    SCA_DATA *pData;
    UCHAR *pList;
    SCA_WORK *pW;
    VECTOR Pos;
    VECTOR *pV;
    LONG Y;
    LONG Ceil;
    ULONG Level;
    ULONG Shape;
    LONG Dx;
    LONG Dz;
    LONG W;
    LONG High;
    LONG Num;
    UCHAR *pTbl;

    pList = Sca_block_list;
    pT = (SCA_DATA *)G.pRoom->pSca;
    Num = ((SCA_HEAD *)pT)->Num;
    Y = ((SCA_HEAD *)pT)->Ceiling;
    pTbl = (UCHAR *)(pT + Num);
    pData = pT + 1;
    pV = &Pos;
    do { pW = &Sca_work; } while (0);
    Ceil = Y;
    if (Num == 0) {
        return 0;
    }
    pW->Gnd.s.Id = 0x1000;
    pW->Gnd.s.Type = 0;
    Level = (Flg & 8) >> 3;
    if ((ULONG)(pPos[0] + 0x8000) > 0xFFFF) {
        return Y;
    }
    if ((ULONG)(pPos[2] + 0x8000) > 0xFFFF) {
        return Y;
    }
    if (Sca_get_block((VECTOR *)pPos, (LONG)pT, pTbl, pList) && *pList != 0xFF) {
        do {
            pT = &pData[*pList++];
            if (pT->Id & Att) {
                if (pT->Floor_lo * -1800 < pPos[1]) {
                    continue;
                }
                if (pT->High >= Y && !(Flg & 1)) {
                    continue;
                }
                if (pT->Type & 0x1000) {
                    Dx = pPos[0] - pT->X0;
                    Dz = pPos[2] - pT->Z0;
                    pV->vx = ((Dx - Dz) * 181) >> 8;
                    pV->vz = ((Dx + Dz) * 181) >> 8;
                    pV->vx += pT->X0;
                    pV->vz += pT->Z0;
                } else {
                    pV->vx = pPos[0];
                    pV->vz = pPos[2];
                }
                if (Flg & 0x20) {
                    pV->vx += R;
                    pV->vz += R;
                    W = R * 2;
                } else {
                    W = 0;
                }
                if ((ULONG)(pV->vx - pT->X0) > (ULONG)(pT->X1 - pT->X0 + W)) {
                    continue;
                }
                if ((ULONG)(pV->vz - pT->Z0) > (ULONG)(pT->Z1 - pT->Z0 + W)) {
                    continue;
                }
                if (Flg & 0x20) {
                    pV->vx -= R;
                    pV->vz -= R;
                }
                Shape = pT->Id & 0xF;
                if ((Flg & 0x40) && Shape != 10) {
                    continue;
                }
                High = Sca_high_tbl[Level][Shape](pT, pV, Flg, Ceil);
                if (High < Y) {
                    if ((pW->Gnd.s.Id & 0xF) == 10 && Shape == 1 && !(pT->Type & 0xF00)) {
                        continue;
                    }
                } else {
                    if (Shape != 10) {
                        continue;
                    }
                    if ((Flg & 3) == 2) {
                        continue;
                    }
                    if ((pW->Gnd.All & 0x0F00000F) != 1) {
                        continue;
                    }
                }
                do {
                    pW->Gnd.s.Id = pT->Id;
                    Y = High;
                    pW->Gnd.s.Type = pT->Type;
                } while (0);
            }
        } while (*pList != 0xFF);
    }
    return Sca_get_ground_obj((VECTOR *)pPos, R, Att, Flg, Y);
}

/* Sca_get_ground's pass over the collision object list: the highest top (lowest y) below Ground of the
   objects under pPos; a character found there goes to pEm_hit/pEm_hit2 */
LONG Sca_get_ground_obj(VECTOR *pPos, LONG R, ULONG Att, ULONG Flg, LONG Ground)
{
    SCA_WORK *pW;
    COL_ENT *pEnt;
    SCA_DATA *pT;
    ULONG Area;
    LONG High;

    pW = &Sca_work;
    pW->pEm_hit = NULL;
    pW->pEm_hit2 = NULL;
    High = Ground;
    Area = Sca_get_area((SHORT)pPos->vx, (SHORT)pPos->vz);
    pT = (SCA_DATA *)Oba_work.Ent;
    for (pEnt = (COL_ENT *)pT; ((COL_ENT *)pT)->C.All != -1; pEnt = (COL_ENT *)pT) {
        if ((pT->Type & Area) && (pT->Id & Att)) {
            switch (pT->Id & 0xF) {
            case 1:
            case 7:
                if (pPos->vy > pT->Floor_lo * -1800) {
                    goto next;
                }
                if (pT->High >= Ground) {
                    goto next;
                }
                if ((ULONG)(pPos->vx - pT->X0) < (ULONG)(pT->X1 - pT->X0) && (ULONG)(pPos->vz - pT->Z0) < (ULONG)(pT->Z1 - pT->Z0)) {
                    High = pT->High;
                }
                break;
            case 0:
                if (Flg & 0x14) {
                    if ((Flg & 0x10) && pEnt->Type != 3) {
                        break;
                    }
                    pT = &Tmp_Sca;
                    Oba_tmp_sca_set(pEnt->pObj, pT);
                    if (pPos->vy > pT->Floor_lo * -1800) {
                        goto next;
                    }
                    if (pT->High >= Ground) {
                        goto next;
                    }
                    if (Sca_ck_circle(pPos, 0, pT)) {
                        High = pT->High;
                    }
                }
                break;
            }
            if (High < Ground) {
                Ground = High;
                if (High < pPos->vy) {
                    if (pEnt->Type < 3) {
                        pW->pEm_hit = pEnt->pObj->pEm;
                        pW->pEm_hit2 = NULL;
                        pW->Gnd.s.Id = 0x1000;
                    } else {
                        pW->pEm_hit2 = pEnt->pObj->pEm;
                        pW->pEm_hit = NULL;
                        pW->Gnd.s.Id = 0x1000;
                    }
                }
            }
        }
    next:
        pT = (SCA_DATA *)(pEnt + 1);
    }
    return Ground;
}

/* copy the 0xFF-terminated shape index list of the 16x16 room grid block containing pPos (table of USHORT offsets + byte lists); used by Sca_get_ground */
ULONG Sca_get_block(VECTOR *pPos, LONG Arg1, UCHAR *pTbl, UCHAR *pOut)
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

/* height routine for shapes without a top (5, 7, 8, 11, ...): returns High unchanged */
LONG Sca_high_dummy(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
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

/* height routine for shape 6: dispatch to Sca_high_naname_a..d by (Id >> 4) & 3 */
LONG Sca_high_naname(SCA_DATA *pT, VECTOR *pPos, LONG Unused, LONG High)
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

/* height on a slope: linear ramp from the Floor_lo level to High + 1800 along the direction (Id >> 4) & 3, clamped; RE2 Sca_get_slope_high */
LONG Sca_high_slope(SCA_DATA *pT, VECTOR *pPos, LONG High, LONG Unused)
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

/* height routine for shape 10 (stairs): sets Sca_work.Unk8, Flg & 1 -> Sca_high_slope, Flg & 2 -> High, else 450 per 540 step, clamped; RE2 Sca_get_stairs_high (same 0x21C/0x1C2 constants) */
LONG Sca_high_stairs(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    LONG Dir;
    LONG Lo;
    LONG Hi;
    LONG H;

    Dir = (pT->Id >> 4) & 3;
    Sca_work.Unk8 = 1;
    if (Flg & 1) {
        return Sca_high_slope(pT, pPos, Flg, High);
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

/* height of a koban (capsule) along X, RE2 Sca_ck_koban_x as a height routine: in the rounded ends the
 * point must lie in the end circle */
LONG Sca_high_koban_x(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    SHORT D;
    SHORT Half;
    ULONG Zone;

    D = pT->Z1 - pT->Z0;
    Half = D >> 1;
    Zone = ((ULONG)(pPos->vx - (SHORT)(pT->X1 - Half)) >> 30) & 2;
    Zone |= (ULONG)(pPos->vx - (SHORT)(pT->X0 + Half)) >> 31;
    /* the loop is never repeated; the original has one (it changes register allocation) */
    do {
        switch (Zone) {
        case 0:
            Tmp_Sca.X1 = pT->X1;
            Tmp_Sca.X0 = pT->X1 - D;
            Tmp_Sca.Z1 = pT->Z1;
            Tmp_Sca.Z0 = pT->Z0;
            if (Sca_ck_circle2(pPos, 0, &Tmp_Sca)) {
                return pT->High;
            }
            return High;
        case 2:
            return pT->High;
        case 3:
            Tmp_Sca.X1 = pT->X0 + D;
            Tmp_Sca.X0 = pT->X0;
            Tmp_Sca.Z1 = pT->Z1;
            Tmp_Sca.Z0 = pT->Z0;
            if (Sca_ck_circle2(pPos, 0, &Tmp_Sca)) {
                return pT->High;
            }
            return High;
        }
    } while (0);
}

/* height of a koban (capsule) along Z, RE2 Sca_ck_koban_z as a height routine */
LONG Sca_high_koban_z(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    SHORT W;
    SHORT Half;
    ULONG Zone;

    W = pT->X1 - pT->X0;
    Half = W >> 1;
    Zone = ((ULONG)(pPos->vz - (SHORT)(pT->Z1 - Half)) >> 30) & 2;
    Zone |= (ULONG)(pPos->vz - (SHORT)(pT->Z0 + Half)) >> 31;
    /* the loop is never repeated; the original has one (it changes register allocation) */
    do {
        switch (Zone) {
        case 0:
            Tmp_Sca.Z1 = pT->Z1;
            Tmp_Sca.Z0 = pT->Z1 - W;
            Tmp_Sca.X1 = pT->X1;
            Tmp_Sca.X0 = pT->X0;
            if (Sca_ck_circle2(pPos, 0, &Tmp_Sca)) {
                return pT->High;
            }
            return High;
        case 2:
            return pT->High;
        case 3:
            Tmp_Sca.Z1 = pT->Z0 + W;
            Tmp_Sca.Z0 = pT->Z0;
            Tmp_Sca.X1 = pT->X1;
            Tmp_Sca.X0 = pT->X0;
            if (Sca_ck_circle2(pPos, 0, &Tmp_Sca)) {
                return pT->High;
            }
            return High;
        }
    } while (0);
}

/* height routine for shape 4 (diamond): per quadrant from the centre, inside the edge line -> pT->High; RE2 Sca_ck_hishi as a height test */
INCLUDE_ASM("asm/nonmatchings/game/sca", Sca_high_hishi);

/* height routine for shape 12: top falls linearly from High at the centre to the Floor_lo level at the circle's rim (cone / mound); no RE2 equivalent (RE2 Sca_get_curve_high is a half cylinder) */
LONG Sca_high_cone(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
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

/* segment pV0-pV1 against the room's SCA shapes (Attr bits of Id, same area at both ends, bounding box, Flg & 1: height
 * range and the 3D confirmation of Sca_line_xy/zy); Flg & 2 also scans the collision objects (Sca_ck_line_oba),
 * Flg & 4 only those; RE2 Sca_ck_line */
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg)
{
    SCA_DATA *pT;
    SCA_DATA *pEnd;
    VECTOR *pA;
    VECTOR *pB;
    VECTOR A;
    VECTOR B;
    ULONG Area;
    LONG Ret;
    LONG Dx;
    LONG Dz;
    LONG Max;
    LONG Min;
    ULONG P0;
    ULONG P1;

    Ret = 0;
    pT = (SCA_DATA *)G.pRoom->pSca;
    pEnd = pT + ((SCA_HEAD *)pT)->Num;
    Sca_work.pEm_hit = NULL;
    Sca_work.pEm_hit2 = NULL;
    pA = &A;
    pB = &B;
    if (G.pRoom->pSca->Num == 0) {
        return Ret;
    }
    if ((pV0->vx >> 1) == (pV1->vx >> 1) && (pV0->vz >> 1) == (pV1->vz >> 1)) {
        return 0;
    }
    Area = Sca_get_area((SHORT)pV0->vx, (SHORT)pV0->vz);
    if (Area != Sca_get_area((SHORT)pV1->vx, (SHORT)pV1->vz)) {
        Area = 0xFF;
    }
    if (!(Flg & 4)) {
        for (pT++; pT != pEnd && Ret == 0; pT++) {
            if (!(pT->Id & Attr) || (pT->Id & 0xF) == 11 || !(pT->Type & Area)) {
                continue;
            }
            if (pT->Type & 0x1000) {
                Dx = pV0->vx - pT->X0;
                Dz = pV0->vz - pT->Z0;
                pA->vx = ((Dx - Dz) * 181) >> 8;
                pA->vz = ((Dx + Dz) * 181) >> 8;
                pA->vx += pT->X0;
                pA->vz += pT->Z0;
                Dx = pV1->vx - pT->X0;
                Dz = pV1->vz - pT->Z0;
                pB->vx = ((Dx - Dz) * 181) >> 8;
                pB->vz = ((Dx + Dz) * 181) >> 8;
                pB->vx += pT->X0;
                pB->vz += pT->Z0;
            } else {
                pA->vx = pV0->vx;
                pA->vz = pV0->vz;
                pB->vx = pV1->vx;
                pB->vz = pV1->vz;
            }
            pA->vy = pV0->vy;
            pB->vy = pV1->vy;
            Max = pB->vx;
            if (Max < pA->vx) {
                Max = pA->vx;
            }
            Min = pB->vx;
            if (pA->vx < Min) {
                Min = pA->vx;
            }
            if (pT->X1 < Min || Max < pT->X0) {
                continue;
            }
            Max = pB->vz;
            if (Max < pA->vz) {
                Max = pA->vz;
            }
            Min = pB->vz;
            if (pA->vz < Min) {
                Min = pA->vz;
            }
            if (pT->Z1 < Min || Max < pT->Z0) {
                continue;
            }
            if (Flg & 1) {
                Max = pB->vy;
                if (Max < pA->vy) {
                    Max = pA->vy;
                }
                Min = pB->vy;
                if (pA->vy < Min) {
                    Min = pA->vy;
                }
                if (Max < pT->High || pT->Floor_lo * -1800 < Min) {
                    continue;
                }
            }
            P0 = ((pA->vx >> 1) << 16) | ((pA->vz >> 1) & 0xFFFF);
            P1 = ((pB->vx >> 1) << 16) | ((pB->vz >> 1) & 0xFFFF);
            Ret = Sca_line_tbl[pT->Id & 0xF](P0, P1, pT);
            if (Flg & 1) {
                if ((pT->Id & 0xF) == 6) {
                    if (Ret == 2) {
                        Ret = 0;
                    } else if (Ret != 0) {
                        Ret = Sca_line_xy_box(pA, pB, pT);
                        if (Ret != 0) {
                            Ret = Sca_line_zy_box(pA, pB, pT);
                        }
                    }
                } else if (Ret != 0) {
                    Ret = Sca_line_xy(pA, pB, pT);
                    if (Ret != 0) {
                        Ret = Sca_line_zy(pA, pB, pT);
                    }
                }
            }
        }
        if (!(Flg & 2)) {
            return Ret;
        }
    }
    return Sca_ck_line_oba(pV0, pV1, Attr, Flg);
}

/* Sca_ck_line-like scan of the collision object list: segment pA-pB against every
 * object (shape test Sca_line_tbl[Id & 0xF]); with Flg & 1 the hit is confirmed in 3D */
LONG Sca_ck_line_oba(VECTOR *pA, VECTOR *pB, ULONG Att, ULONG Flg)
{
    SCA_WORK *pW;
    COL_ENT *pEnt;
    SCA_DATA *pT;
    ULONG P0;
    ULONG P1;
    LONG Ret;

    Ret = 0;
    pT = (SCA_DATA *)G.pRoom->pSca;
    pW = &Sca_work;
    if (pT->X0 == 0) {
        return Ret;
    }
    pT = (SCA_DATA *)Oba_work.Ent;
    P0 = ((pA->vx >> 1) << 16) | ((pA->vz >> 1) & 0xFFFF);
    P1 = ((pB->vx >> 1) << 16) | ((pB->vz >> 1) & 0xFFFF);
    for (pEnt = (COL_ENT *)pT; ((COL_ENT *)pT)->C.All != -1 && Ret == 0; pEnt = (COL_ENT *)pT) {
        if (pT->Id & Att) {
            if (!(pEnt->pObj->Flg & 7)) {
                pT = &Tmp_Sca;
                Oba_tmp_sca_set(pEnt->pObj, pT);
            }
            Ret = Sca_line_tbl[pT->Id & 0xF](P0, P1, pT);
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
                Ret = Sca_line_xy(pA, pB, pT);
                if (Ret != 0) {
                    Ret = Sca_line_zy(pA, pB, pT);
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

/* do segments P0-P1 and P2-P3 cross (two pairs of GTE nclip sign tests); points packed (x << 16) | z */
LONG Sca_ck_cross(ULONG P0, ULONG P1, ULONG P2, ULONG P3)
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

/* line test for shapes 8/11/12: returns 0 */
LONG Sca_line_dummy(void)
{
    return 0;
}

/* line test for shape 5: the single diagonal (X0,Z0)-(X1,Z1) */
LONG Sca_line_diag(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    UCHAR Unused[0x20];

    return Sca_ck_cross(P0, P1, ((pT->X0 >> 1) << 16) | ((pT->Z0 >> 1) & 0xFFFF), ((pT->X1 >> 1) << 16) | ((pT->Z1 >> 1) & 0xFFFF));
}

/* line test for shape 0: line against the circle's diameter perpendicular to it, or P1 inside the circle */
LONG Sca_line_circle(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[4];
    SHORT X1;
    LONG Z1;
    LONG Dx;
    LONG Dz;
    LONG Len;
    LONG X0;
    LONG Z0;
    LONG Half;
    LONG Dx2;
    LONG Dz2;
    LONG Half2;
    ULONG A, B;

    X1 = (LONG)P1 >> 16;
    Dx = ((LONG)P0 >> 16) - X1;
    Z1 = (SHORT)P1;
    Dz = Z1 - (SHORT)P0;
    X0 = pT->X0 >> 1;
    Half = pT->X1 >> 1;
    Half = (Half - X0) >> 1;
    Z0 = pT->Z0 >> 1;
    Len = SquareRoot0(Dz * Dz + Dx * Dx);
    Dz *= Half;
    Dz /= Len;
    Dx *= Half;
    Dx /= Len;
    X0 += Half;
    Z0 += Half;
    A = (X0 + Dz) << 16;
    B = (X0 - Dz) << 16;
    /* Half reused for the result (its extra refs put it above X0 in global-alloc) */
    Half = Sca_ck_cross(P0, P1, A | ((Z0 + Dx) & 0xFFFF), B | ((Z0 - Dx) & 0xFFFF));
    if (Half) {
        return 1;
    }
    Half2 = (pT->X1 - pT->X0) >> 2;
    Dx2 = X1 - ((pT->X0 >> 1) + Half2);
    Dz2 = Z1 - ((pT->Z0 >> 1) + Half2);
    if (Dx2 != 0 || Dz2 != 0) {
        /* X0 reused for the sum: combine folds it into the argument, but X0 stays a global (s6) */
        X0 = Dx2 * Dx2 + Dz2 * Dz2;
        if (SquareRoot0(X0) < Half2) {
            return 1;
        }
    }
    return 0;
}

/* line test for shapes 9/10 (and boxes in em23/em25): each box edge whose wall flag is set in Type 0x100/0x200/0x400/0x800 */
LONG Sca_line_wall(ULONG P0, ULONG P1, SCA_DATA *pT)
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
    if ((Flg & 0x100) && Sca_ck_cross(P0, P1, A, B)) {
        return 1;
    }
    A = (x1 << 16) | (z1 & 0xFFFF);
    if ((Flg & 0x200) && Sca_ck_cross(P0, P1, A, B)) {
        return 1;
    }
    B = (x1 << 16) | (z0 & 0xFFFF);
    if ((Flg & 0x400) && Sca_ck_cross(P0, P1, A, B)) {
        return 1;
    }
    A = (x0 << 16) | (z0 & 0xFFFF);
    if ((Flg & 0x800) && Sca_ck_cross(P0, P1, A, B)) {
        return 1;
    }
    return 0;
}

/* line test for shapes 1/7: both diagonals of the box */
LONG Sca_line_box(ULONG P0, ULONG P1, SCA_DATA *pT)
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
    if (Sca_ck_cross(P0, P1, A, B)) {
        return 1;
    }
    A &= 0xFFFF0000;
    B &= 0xFFFF0000;
    return Sca_ck_cross(P0, P1, A | Z0, B | Z1) != 0;
}

/* line test for shape 2: capsule along X (centre line + two inner diagonals) */
LONG Sca_line_koban_x(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    LONG x0, x1, z0, z1, d;
    ULONG A, B, Z0, Z1;

    x0 = pT->X0 >> 1;
    z0 = pT->Z0 >> 1;
    d = (pT->Z1 - pT->Z0) >> 2;
    z1 = pT->Z1 >> 1;
    x1 = pT->X1 >> 1;
    if (Sca_ck_cross(P0, P1, (x0 << 16) | ((z0 + d) & 0xFFFF), (x1 << 16) | ((z0 + d) & 0xFFFF))) {
        return 1;
    }
    A = ((x0 + d) << 16) | (Z0 = z0 & 0xFFFF);
    B = ((x1 - d) << 16) | (Z1 = z1 & 0xFFFF);
    if (Sca_ck_cross(P0, P1, A, B)) {
        return 1;
    }
    A &= 0xFFFF0000;
    B &= 0xFFFF0000;
    return Sca_ck_cross(P0, P1, A | Z1, B | Z0) != 0;
}

/* line test for shape 3: capsule along Z */
LONG Sca_line_koban_z(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    LONG Unused[8];
    LONG x0, x1, z0, z1, d;
    ULONG A, B;

    x0 = pT->X0 >> 1;
    x1 = pT->X1 >> 1;
    z0 = pT->Z0 >> 1;
    d = (pT->X1 - pT->X0) >> 2;
    z1 = pT->Z1 >> 1;
    if (Sca_ck_cross(P0, P1, ((x0 + d) << 16) | (z0 & 0xFFFF), ((x0 + d) << 16) | (z1 & 0xFFFF))) {
        return 1;
    }
    A = (x0 << 16) | ((z0 + d) & 0xFFFF);
    B = (x1 << 16) | ((z1 - d) & 0xFFFF);
    if (Sca_ck_cross(P0, P1, A, B)) {
        return 1;
    }
    A &= 0xFFFF;
    B &= 0xFFFF;
    return Sca_ck_cross(P0, P1, A | (x1 << 16), B | (x0 << 16)) != 0;
}

/* line test for shape 4: the two centre lines (diamond's diagonals) */
LONG Sca_line_hishi(ULONG P0, ULONG P1, SCA_DATA *pT)
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
    if (Sca_ck_cross(P0, P1, (xm << 16) | (z0 & 0xFFFF), (xm << 16) | (z1 & 0xFFFF))) {
        return 1;
    }
    return Sca_ck_cross(P0, P1, (x0 << 16) | ((z0 + dz) & 0xFFFF), (x1 << 16) | ((z0 + dz) & 0xFFFF)) != 0;
}

/* line test for shape 6: the two legs (return 1) and the hypotenuse (return 2) of the triangle, corner from Id & 0x30 */
LONG Sca_line_naname(ULONG P0, ULONG P1, SCA_DATA *pT)
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
        Ret = Sca_ck_cross(P0, P1, (x0 << 16) | (z0 & 0xFFFF), (x1 << 16) | (z0 & 0xFFFF));
    } else {
        Ret = Sca_ck_cross(P0, P1, (x0 << 16) | (z1 & 0xFFFF), (x1 << 16) | (z1 & 0xFFFF));
    }
    if (Ret) {
        return 1;
    }
    if (Flg & 0x10) {
        Ret = Sca_ck_cross(P0, P1, (x0 << 16) | (z0 & 0xFFFF), (x0 << 16) | (z1 & 0xFFFF));
    } else {
        Ret = Sca_ck_cross(P0, P1, (x1 << 16) | (z0 & 0xFFFF), (x1 << 16) | (z1 & 0xFFFF));
    }
    if (Ret) {
        return 1;
    }
    if (Flg == 0 || Flg == 0x30) {
        if (Sca_ck_cross(P0, P1, (x0 << 16) | (z1 & 0xFFFF), (x1 << 16) | (z0 & 0xFFFF))) {
            return 2;
        }
    } else {
        if (Sca_ck_cross(P0, P1, (x0 << 16) | (z0 & 0xFFFF), (x1 << 16) | (z1 & 0xFFFF))) {
            return 2;
        }
    }
    return 0;
}

/* Sca_ck_line height confirm, X-Y projection: line against the shape's side rectangle (Floor_lo..High), slope / stairs profile for shapes >= 9 */
LONG Sca_line_xy(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
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
                if (Sca_ck_cross(P0, P1, A, B)) {
                    return 1;
                }
                A &= 0xFFFF;
                if (Sca_ck_cross(P0, P1, A | (x1 << 16), B)) {
                    return 1;
                }
            } else {
                A = (x1 << 16) | (lo & 0xFFFF);
                B = (x1 << 16) | (hi & 0xFFFF);
                if (Sca_ck_cross(P0, P1, A, B)) {
                    return 1;
                }
                A &= 0xFFFF;
                if (Sca_ck_cross(P0, P1, A | (x0 << 16), B)) {
                    return 1;
                }
            }
            if (Sca_ck_cross(P0, P1, (x0 << 16) | (lo & 0xFFFF), (x1 << 16) | (lo & 0xFFFF))) {
                return 1;
            }
        }
        return 0;
    }
    if (ax < x0 || bx < x0 || x1 < ax || x1 < bx || ay < hi || by < hi || lo < ay || lo < by) {
        A = (x0 << 16) | (lo & 0xFFFF);
        B = (x0 << 16) | (hi & 0xFFFF);
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= x1 << 16;
        A |= x1 << 16;
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A = (x0 << 16) | (hi & 0xFFFF);
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return Sca_ck_cross(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
    }
    return 1;
}

/* same in the Z-Y projection */
LONG Sca_line_zy(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
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
                if (Sca_ck_cross(P0, P1, (z0 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
                if (Sca_ck_cross(P0, P1, (z1 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
            } else {
                B = (z1 << 16) | (hi & 0xFFFF);
                if (Sca_ck_cross(P0, P1, (z1 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
                if (Sca_ck_cross(P0, P1, (z0 << 16) | (lo & 0xFFFF), B)) {
                    return 1;
                }
            }
            if (Sca_ck_cross(P0, P1, (z0 << 16) | (lo & 0xFFFF), (z1 << 16) | (lo & 0xFFFF))) {
                return 1;
            }
        }
        return 0;
    }
    if (az < z0 || bz < z0 || z1 < az || z1 < bz || ay < hi || by < hi || lo < ay || lo < by) {
        A = (z0 << 16) | (lo & 0xFFFF);
        B = (z0 << 16) | (hi & 0xFFFF);
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= z1 << 16;
        A |= z1 << 16;
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A = (z0 << 16) | (hi & 0xFFFF);
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return Sca_ck_cross(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
    }
    return 1;
}

/* X-Y projection test against the plain box side (no slope case); Sca_ck_line uses it for a triangle leg hit */
LONG Sca_line_xy_box(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
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
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= x1 << 16;
        A |= x1 << 16;
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A = (x0 << 16) | (hi & 0xFFFF);
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return Sca_ck_cross(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
    }
    return 1;
}

/* same in the Z-Y projection */
LONG Sca_line_zy_box(VECTOR *pA, VECTOR *pB, SCA_DATA *pT)
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
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF;
        B &= 0xFFFF;
        B |= z1 << 16;
        A |= z1 << 16;
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A = (z0 << 16) | (hi & 0xFFFF);
        if (Sca_ck_cross(P0, P1, A, B)) {
            return 1;
        }
        A &= 0xFFFF0000;
        B &= 0xFFFF0000;
        return Sca_ck_cross(P0, P1, A | (lo & 0xFFFF), B | (lo & 0xFFFF)) != 0;
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
USHORT Sca_get_dir(VECTOR *pPos, SCA_DATA *pT)
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

/* quadrant of the shape the point is in (45 degree shapes are rotated first): bit 0 left of the centre,
 * bit 2 right, bit 3 in front, bit 1 behind (on a centre line both bits of that axis) */
USHORT Sca_get_quadrant(VECTOR *pPos, SCA_DATA *pT)
{
    VECTOR Pos;
    VECTOR *pP;
    ULONG Dir;
    LONG X; /* offset from the corner, then the centre */
    LONG Z;

    pP = &Pos;
    if (pT->Type & 0x1000) {
        X = pPos->vx - pT->X0;
        Z = pPos->vz - pT->Z0;
        Pos.vx = (X - Z) * 181 >> 8;
        Pos.vz = (X + Z) * 181 >> 8;
        Pos.vx += pT->X0;
        Pos.vz += pT->Z0;
    } else {
        Pos.vx = pPos->vx;
        Pos.vz = pPos->vz;
    }
    X = pT->X0 + ((pT->X1 - pT->X0) >> 1);
    Z = pT->Z0 + ((pT->Z1 - pT->Z0) >> 1);
    if (pP->vx >= X) {
        if (pP->vx == X) {
            Dir = 5;
        } else {
            Dir = 4;
        }
    } else {
        Dir = 1;
    }
    if (pP->vz >= Z) {
        if (pP->vz == Z) {
            Dir |= 0xA;
        } else {
            Dir |= 2;
        }
    } else {
        Dir |= 8;
    }
    return Dir;
}

/* is the object inside the extent of the wall it hit, along the wall's axis (Sca_get_dir) */
LONG Sca_ck_wall_range(COL_OBJ *pObj, SCA_DATA *pT)
{
    LONG Pos;
    LONG Min;
    LONG Max;

    if (Sca_get_dir(POS_V(pObj), pT) & 1) {
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
