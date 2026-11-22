#include "oba.h"
#include "player.h"
#include "sca.h"
#include "sabun.h"
#include "sub.h"

void func_80033508(void)
{
    ENEMY_WORK *pEm;
    COL_OBJ *pObj;

    D_800DCED0.pTail = NULL;
    D_800DCED0.pTop = NULL;
    D_800DCED0.Ent[0].C.All = -1;
    pEm = &Pl_work;
    pObj = EM_COL(pEm);
    pObj->pEm = pEm;
    pObj->W0 = pObj->D0 = 450;
    pObj->W1 = pObj->D1 = 450;
    pObj->H = 0xAF0;
    pObj->Unk24 = 0;
    pObj->Unk26 = 0;
    pObj->Unk28 = 0;
    pObj->Flg = 0;
    pObj->Type = 0;
    func_80033580(pObj, 0);
}

UCHAR func_80033580(COL_OBJ *pObj, ULONG Id)
{
    COL_OBJ *p;
    ULONG No;
    ULONG Old;

    No = 1;
    for (p = D_800DCED0.pTop; p != NULL; p = p->pNext) {
        if (p == pObj) {
            return;
        }
    }
    pObj->pNext = NULL;
    pObj->Unk3C = 0;
    pObj->Unk2A = 0;
    pObj->Unk2C = 0;
    if (Id != 0) {
        pObj->Id = Id;
    } else {
        do {
            Old = No;
            for (p = D_800DCED0.pTop; p != NULL; p = p->pNext) {
                if (p->Id == No) {
                    No++;
                    break;
                }
            }
        } while (No != Old);
        pObj->Id = No;
    }
    pObj->Hit_flg = 0;
    if ((p = D_800DCED0.pTop) != NULL) {
        D_800DCED0.pTop = pObj;
        pObj->pNext = p;
    } else {
        D_800DCED0.pTail = pObj;
        D_800DCED0.pTop = pObj;
    }
    func_80034124(pObj);
    pObj->pM = NULL;
    pObj->Unk39 = 8;
    pObj->Old_x = pObj->Pos_x;
    pObj->Old_y = pObj->Pos_y;
    pObj->Old_z = pObj->Pos_z;
    pObj->W1 = pObj->W0;
    pObj->D1 = pObj->D0;
    if (pObj->Type < 3) {
        pObj->Em_idx = func_800336B4(pObj->pEm);
    }
    return pObj->Id;
}

LONG func_800336B4(ENEMY_WORK *pEm)
{
    LONG i;

    for (i = 0; i < 42; i++) {
        if (G.pEnemy[i] == pEm) {
            return i;
        }
    }
    return 0;
}

void func_800336F0(COL_OBJ *pTop, COL_ENT *pEnt)
{
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    ULONG Be;
    LONG n;
    LONG Pass;

    n = 0;
    for (Pass = -1; Pass != 1; Pass++) {
        for (p = D_800DCED0.pTop; p != NULL; p = p->pNext) {
            pEm = p->pEm;
            if (p->Id != 0) {
                Be = pEm->Be_flg;
                if ((Be & 0x11) == 1 && (Be & 0x6000) != 0x6000) {
                    goto add;
                }
            }
            if (!(p->Id & 0x80)) {
                continue;
            }
        add:
            if (Pass < 0 && p->Type < 3) {
                continue;
            }
            do {
                D_800DCDD0[n++] = p;
            } while (0); /* needed for register allocation */
        }
    }
    D_800DCDD0[n] = NULL;
}

void func_800337B8(COL_OBJ *pTop, COL_ENT *pEnt)
{
    COL_OBJ **pp;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    USHORT Flg;
    UCHAR R;
    UCHAR Type;
    UCHAR Idx;
    LONG i;

    for (i = 0; (p = D_800DCDD0[i]) != NULL; i++) {
        Flg = p->Hit_flg;
        p->Hit_flg = Flg & 0x6C0;
        pEm = p->pEm;
        if (p->Type < 2) {
            R = pEm->Routine_0;
            if (R == 2 || R == 5) {
                p->Hit_flg = Flg & 0xC0;
            }
        }
        Type = p->Type;
        Idx = p->Em_idx;
        pEnt->pObj = p;
        pEnt->Type = Type;
        pEnt->Em_idx = Idx;
        if (p->Flg & 7) {
            pEnt = func_80033CF4(p, pEnt);
        } else {
            pEnt->Id = 0xFE40;
            pEnt->Area = 0xF00;
            pEnt->C.All = 0;
            func_80034284(p, pEnt);
        }
        pEnt++;
    }
    pEnt->C.All = -1;
}

/* no return value: v0 stays out of the last delay slot */
LONG func_8003389C(COL_OBJ *pTop, COL_ENT *pEnt)
{
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    ULONG Tbl;
    LONG dx;
    LONG dz;
    LONG w0;
    LONG w1;
    LONG w2;
    LONG ex;
    LONG ez;
    LONG i;

    i = 0;
    if ((p = D_800DCDD0[0]) != NULL) {
        do {
            pEnt = D_800DCED0.Ent;
            pEm = p->pEm;
            while (pEnt->pObj != p) {
                pEnt++;
            }
            Tbl = -1;
            if (p->Type < 5) {
                Tbl = D_8009CBC8[p->Type];
                if (p->Type < 3 && (pEm->Damage_cnt & 0x80)) {
                    p->Hit_flg |= 0x8000;
                }
            }
            if (p->Type < 3) {
                func_80034814(pEm, p->W0, Tbl, p);
            } else {
                func_80035184(pEm, p->W0, Tbl, p);
            }
            if (!(p->Flg & 0x80)) {
                func_8004AF04(pEm, p->W0, Tbl, p);
            }
            if (p->Type < 3) {
                func_80034F08(pEm, p->W0, Tbl, p);
            }
            p->Hit_flg |= 0x4000;
            if (p->Hit_flg & 0x100) {
                w0 = p->Old_x;
                w1 = p->Old_y;
                w2 = p->Old_z;
                p->Pos_x = w0;
                p->Pos_y = w1;
                p->Pos_z = w2;
                w0 = p->pEm->Unk40[0];
                w1 = p->pEm->Unk40[2];
                p->pEm->Pos_x = w0;
                p->pEm->Pos_z = w1;
                if (p->Type < 3) {
                    pEm->Base_pos_x = w0;
                    pEm->Base_pos_z = w1;
                }
                dz = dx = 0;
            } else {
                w2 = pEm->Pos_y;
                dx = -pEm->Unk40[0] + (ex = pEm->Pos_x);
                dz = -pEm->Unk40[2] + (ez = pEm->Pos_z);
                pEm->Unk40[0] = ex;
                pEm->Unk40[2] = ez;
                pEm->Unk40[1] = w2;
                w0 = p->Pos_x;
                w1 = p->Pos_z;
                w2 = p->Pos_y;
                p->Old_x = w0;
                p->Old_z = w1;
                p->Old_y = w2;
                p->W1 = p->W0;
                p->D1 = p->D0;
            }
            pEnt->X0 += dx;
            pEnt->X1 += dx;
            pEnt->Z0 += dz;
            pEnt->Z1 += dz;
        } while ((p = D_800DCDD0[++i]) != NULL);
    }
}

void func_80033B20(void)
{
    COL_OBJ *pTop;

    pTop = D_800DCED0.pTop;
    func_800336F0(pTop, D_800DCED0.Ent);
    func_80033FBC(pTop);
    func_800337B8(pTop, D_800DCED0.Ent);
    func_8003389C(pTop, D_800DCED0.Ent);
    func_80033B88(pTop);
}

void func_80033B88(COL_OBJ *pTop)
{
    LONG i;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    VECTOR V;
    VECTOR *pV;
    LONG y;
    ULONG W;
    ULONG Tbl;

    pV = &V;
    for (i = 0; (p = D_800DCDD0[i]) != NULL; i++) {
        pEm = p->pEm;
        if (pEm->Be_flg & 0x2000) {
            continue;
        }
        if (p->Type >= 3) {
            continue;
        }
        Tbl = D_8009CBC8[p->Type];
        W = p->W0;
        pV->vx = p->Pos_x;
        pV->vz = p->Pos_z;
        pV->vy = y = p->Pos_y;
        if (pEm->Unk1B8 != 0) {
            pV->vy = y - pEm->Unk1B8;
        }
        D_800DEB58.Unk8 = 0;
        p->Unk39 &= ~8;
        if (p->Hit_flg & 0x80) {
            func_8004B7E8(pEm, p, func_8004D720(&pV->vx, W, Tbl, 9));
        }
        if (D_800DEB58.Unk8 == 0) {
            p->Unk39 |= 8;
            p->Hit_flg &= ~0x40;
            if (p->Type < 3) {
                pEm->Unk1C1 = p->Unk39;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/oba", func_80033CF4);

void func_80033F38(COL_OBJ *pObj, COL_ENT *pEnt, LONG n)
{
    SVECTOR Unused[3];
    LONG y;

    pEnt->Type = pObj->Type;
    pEnt->Id = 0x8008;
    pEnt->pObj = pObj;
    pEnt->Area = 0xF00;
    func_80034284(pObj, pEnt);
    COL_ENT_SET_HIGH(pEnt, pObj, y, n);
}

void func_80033FBC(COL_OBJ *pTop)
{
    LONG i;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    SVECTOR Sv;
    VECTOR V;
    MATRIX M;

    for (i = 0; (p = D_800DCDD0[i]) != NULL; i++) {
        p->Unk3 = 0xFF;
        pEm = p->pEm;
        p->Unk2A = 0;
        p->Unk2C = 0;
        p->Unk3A = 0;
        if (p->pM != NULL) {
            M = GsIDMATRIX;
            MulMatrix2(p->pM, &M);
        } else {
            RotMatrix_gte((SVECTOR *)&pEm->Cdir_x, &M);
        }
        ApplyMatrix(&M, (SVECTOR *)&p->Unk24, &V);
        if (p->pM != NULL) {
            p->Pos_x = p->pM->t[0] + V.vx;
            p->Pos_y = p->pM->t[1] + V.vy;
            p->Pos_z = p->pM->t[2] + V.vz;
        } else {
            p->Pos_x = pEm->Pos_x + V.vx;
            p->Pos_y = pEm->Pos_y + V.vy;
            p->Pos_z = pEm->Pos_z + V.vz;
        }
    }
}

void func_80034124(COL_OBJ *pObj)
{
    ENEMY_WORK *pEm;
    SVECTOR Sv;
    VECTOR V;
    MATRIX M;
    SVECTOR *pSv;

    pSv = &Sv;
    if (pObj->Id == 0) {
        return;
    }
    pEm = pObj->pEm;
    pObj->Unk2A = 0;
    pObj->Unk2C = 0;
    pObj->Hit_flg = 0;
    pObj->Unk3 = 0xFF;
    pObj->Unk3A = 0;
    pSv->vx = pEm->Cdir_x;
    pSv->vy = pEm->Cdir_y;
    pSv->vz = pEm->Cdir_z;
    RotMatrix_gte(pSv, &M);
    pSv->vx = pObj->Unk24;
    pSv->vy = pObj->Unk26;
    pSv->vz = pObj->Unk28;
    ApplyMatrix(&M, pSv, &V);
    V.vx = pObj->Old_x = pObj->Pos_x = pEm->Pos_x + V.vx;
    V.vy = pObj->Old_y = pObj->Pos_y = pEm->Pos_y + V.vy;
    V.vz = pObj->Old_z = pObj->Pos_z = pEm->Pos_z + V.vz;
    if (pObj->Type < 3) {
        pEm->Unk122 = func_8004D720(&V.vx, pObj->W0, D_8009CBC8[pObj->Type], 9);
    }
}

void func_80034284(COL_OBJ *pObj, COL_ENT *pEnt)
{
    SCA_HEAD *pSca;
    LONG x0;
    LONG x1;
    LONG z0;
    LONG z1;
    LONG px;
    LONG pz;

    pSca = G.pRoom->pSca;
    x0 = pObj->Old_x - pObj->W0 - 900;
    x1 = pObj->Old_x + pObj->W0 + 900;
    z0 = pObj->Old_z - pObj->W0 - 900;
    z1 = pObj->Old_z + pObj->W0 + 900;
    px = pSca->Cx0;
    pz = pSca->Cz0;
    if (px <= x0 || px <= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->Area |= 1;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->Area |= 4;
        }
    }
    if (px >= x0 || px >= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->Area |= 2;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->Area |= 8;
        }
    }
    px = pSca->Cx1;
    pz = pSca->Cz1;
    if (px <= x0 || px <= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->Area |= 0x10;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->Area |= 0x40;
        }
    }
    if (px >= x0 || px >= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->Area |= 0x20;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->Area |= 0x80;
        }
    }
}

LONG func_80034448(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SVECTOR Sv;
    LONG w;
    LONG d;
    LONG dx;
    LONG dz;
    LONG ax;
    LONG az;

    Sv.vx = EM_COL(pEm)->W0 * 120 / 100 + 0x110;
    Sv.vz = 0;
    Rot_vector(pEm->Cdir_y, &Sv, &Sv);
    Sv.vx += pEm->Pos_x;
    Sv.vz += pEm->Pos_z;
    w = EM_COL(pT)->W0 - 0x110;
    d = EM_COL(pT)->D0 - 0x110;
    dx = pT->Pos_x - pEm->Pos_x;
    dz = pT->Pos_z - pEm->Pos_z;
    if ((ULONG)(dx + w) > (ULONG)(w * 2) && (ULONG)(dz + d) > (ULONG)(d * 2)) {
        return 0;
    }
    ax = __builtin_abs(dx);
    az = __builtin_abs(dz);
    if (az < ax) {
        dx = Sv.vx - EM_COL(pT)->Pos_x;
        dz = pEm->Pos_z - EM_COL(pT)->Pos_z;
        d += EM_COL(pEm)->W0;
    } else {
        dx = pEm->Pos_x - EM_COL(pT)->Pos_x;
        dz = Sv.vz - EM_COL(pT)->Pos_z;
        w += EM_COL(pEm)->W0;
    }
    if ((ULONG)(dx + w) > (ULONG)(w * 2)) {
        return 0;
    }
    return !((ULONG)(dz + d) > (ULONG)(d * 2));
}

LONG func_800345C0(VECTOR *pPos, ENEMY_WORK *pEm)
{
    ENEMY_WORK **pp;
    ENEMY_WORK *pT;
    ULONG Be;

    if (pEm->Be_flg & 0x4000) {
        return 0;
    }
    for (pp = &G.pEnemy[0]; pp != G.ppEnemy_end; pp++) {
        pT = *pp;
        if (pEm == pT) {
            continue;
        }
        Be = pT->Be_flg;
        if ((Be & 0x11) != 1) {
            continue;
        }
        if ((Be & 0x6000) == 0x6000) {
            continue;
        }
        if (EM_COL(pT)->Id == EM_COL(pEm)->Id) {
            continue;
        }
        if (func_800346AC(pPos, pEm, pT)) {
            return 1;
        }
    }
    return 0;
}

LONG func_800346AC(VECTOR *pPos, ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    COL_OBJ *pA;
    COL_OBJ *pB;
    LONG w;
    LONG r;
    LONG dx;
    LONG dz;
    LONG dy;
    LONG h;

    if ((pEm->Be_flg | pT->Be_flg) & 0x4000) {
        return 0;
    }
    if ((pEm->Be_flg & pT->Be_flg) & 0x800) {
        return 0;
    }
    pA = EM_COL(pEm);
    pB = EM_COL(pT);
    if (!(pA->Id & pB->Id)) {
        return 0;
    }
    w = pA->W0;
    r = w + pB->W0;
    if (pB->Hit_flg & 0x4000) {
        dx = pB->Pos_x - (pPos->vx + w);
        dz = pB->Pos_z - (pPos->vz + w);
    } else {
        dx = pB->Old_x - (pPos->vx + w);
        dz = pB->Old_z - (pPos->vz + w);
    }
    if ((ULONG)(dx + r) > (ULONG)(r * 2)) {
        return 0;
    }
    if ((ULONG)(dz + r) > (ULONG)(r * 2)) {
        return 0;
    }
    if (r - SquareRoot0(dx * dx + dz * dz) <= 0) {
        return 0;
    }
    h = pB->H + pA->H;
    dy = pB->Pos_y - (pPos->vy + pA->Unk26);
    if (-h >= dy || dy >= h) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/oba", func_80034814);

void func_80034D74(COL_OBJ *pObj, COL_ENT *pEnt, ULONG Arg2, ULONG Type)
{
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    if (func_80034448(pPl, pEnt->pObj->pEm)) {
        if (Type == 7 && !(pObj->Hit_flg & 0x600)) {
            if (func_8004B374(pEnt, 7, 0) == 0) {
                pEnt->pObj->Hit_flg |= 0x10;
                pEnt->pObj->Unk3A |= 0x8000;
            } else {
                pPl->Unk1C8 = 1;
            }
        }
    } else if (Type == 8 && !(pObj->Hit_flg & 0x600)) {
        if (func_8004B374(pEnt, 8, 0)) {
            pPl->Unk1C8 = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/oba", func_80034E78);

INCLUDE_ASM("asm/nonmatchings/game/oba", func_80034F08);

/* no return value; pE and pEnt walk the same list (two pointers in the original) */
LONG func_80035184(ENEMY_WORK *pEm, ULONG W, ULONG Tbl, COL_OBJ *pObj)
{
    COL_ENT *pEnt;
    COL_ENT *pE;
    COL_OBJ *pT;
    ULONG Area;
    ULONG Be;
    ULONG Tbe;
    LONG Shape;

    pE = pEnt = D_800DCED0.Ent;
    Area = func_8004D6B8((SHORT)pObj->Pos_x, (SHORT)pObj->Pos_z);
    for (; pEnt->C.All != -1; pEnt = ++pE) {
        if ((pEnt->Area & Area) != Area) {
            continue;
        }
        if (!(pEnt->Id & Tbl)) {
            continue;
        }
        pT = pE->pObj;
        if (pObj == pT) {
            continue;
        }
        if (pE->Type == 3) {
            Be = pEm->Be_flg;
            if (Be & 0x8000) {
                continue;
            }
            if ((Be | pT->pEm->Be_flg) & 0x1000) {
                continue;
            }
        } else {
            if (pEm->Be_flg & 0x8000) {
                continue;
            }
            Tbe = pT->pEm->Be_flg;
            if ((pEm->Be_flg | Tbe) & 0x4000) {
                continue;
            }
            if (Tbe & 0x1000) {
                continue;
            }
            if (pT->Type == 0 && (ULONG)(G.Pl_routine_1 - 11) < 2) {
                continue;
            }
        }
        if (pObj->pEm->Be_flg & pT->pEm->Be_flg & 0x800) {
            continue;
        }
        Shape = pEnt->Id & 0xF;
        if (Shape < 2) {
            if (pObj->Pos_y - pObj->H >= pT->Pos_y) {
                continue;
            }
            if (pT->Pos_y - pT->H >= pObj->Pos_y) {
                continue;
            }
        }
        if (Shape != 0) {
            continue;
        }
        if (pT->Type == 0 && (pObj->pEm->Be_flg & 0x100) && pT->pEm->Damage_cnt == 0 && func_800353CC(pObj, pT) == 0) {
            pObj->Hit_flg |= 0x2000;
        }
    }
}

LONG func_800353CC(COL_OBJ *pObj, COL_OBJ *pT)
{
    LONG x;
    LONG z;
    LONG ex;
    LONG ez;
    LONG tx;
    LONG tz;
    LONG tex;
    LONG tez;
    LONG Ret;

    x = pObj->Pos_x;
    z = pObj->Pos_z;
    ex = pObj->pEm->Pos_x;
    ez = pObj->pEm->Pos_z;
    tx = pT->Pos_x;
    tz = pT->Pos_z;
    tex = pT->pEm->Pos_x;
    tez = pT->pEm->Pos_z;
    func_8003553C(pObj, pT);
    Ret = func_8004AF04(pObj->pEm, pObj->W0, 0x4000, pObj);
    if (Ret != 0 || !(D_800D1F2C & 0x10) || pT->pEm->Damage_cnt != 0) {
        pObj->Pos_x = x;
        pObj->Pos_z = z;
        pObj->pEm->Pos_x = ex;
        pObj->pEm->Pos_z = ez;
        pT->Pos_x = tx;
        pT->Pos_z = tz;
        pT->pEm->Pos_x = tex;
        pT->pEm->Pos_z = tez;
    } else {
        pObj->Unk3A |= 0x8000 >> pT->Em_idx;
        pObj->Unk3 = pT->Em_idx;
        pObj->Hit_flg |= 0x10;
        pT->Unk3A |= 0x8000 >> pObj->Em_idx;
        pT->Unk3 = pObj->Em_idx;
        pT->Hit_flg |= 0x10;
    }
    return Ret;
}

LONG func_8003553C(COL_OBJ *pObj, COL_OBJ *pT)
{
    LONG r;
    LONG dx;
    LONG dz;
    LONG a;

    r = pObj->W0 + pT->W0;
    if (__builtin_abs(pObj->Pos_x - pT->Old_x) >= r) {
        dx = pObj->Pos_x - pT->Pos_x;
        a = __builtin_abs(dx);
        if (a < r) {
            a = r - a;
            pObj->Pos_x += dx > 0 ? a : -a;
            pObj->pEm->Pos_x += dx > 0 ? a : -a;
            return 1;
        }
    }
    r = pObj->D0 + pT->D0;
    if (__builtin_abs(pObj->Pos_z - pT->Old_z) >= r) {
        dz = pObj->Pos_z - pT->Pos_z;
        a = __builtin_abs(dz);
        if (a < r) {
            a = r - a;
            pObj->Pos_z += dz > 0 ? a : -a;
            pObj->pEm->Pos_z += dz > 0 ? a : -a;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/oba", func_80035678);
