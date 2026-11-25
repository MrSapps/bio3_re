#include "oba.h"
#include "player.h"
#include "sca.h"
#include "sabun.h"
#include "sub.h"

/* empty the collision list and set up the player's COL_OBJ */
void Oba_init(void)
{
    ENEMY_WORK *pEm;
    COL_OBJ *pObj;

    Oba_work.pTail = NULL;
    Oba_work.pTop = NULL;
    Oba_work.Ent[0].C.All = -1;
    pEm = &Pl_work;
    pObj = &pEm->Col;
    pObj->pEm = pEm;
    pObj->W0 = pObj->D0 = 450;
    pObj->W1 = pObj->D1 = 450;
    pObj->H = 0xAF0;
    pObj->Unk24 = 0;
    pObj->Unk26 = 0;
    pObj->Unk28 = 0;
    pObj->Flg = 0;
    pObj->Type = 0;
    Oba_set(pObj, 0);
}

/* add pObj to the collision list (once) with shape Id */
UCHAR Oba_set(COL_OBJ *pObj, ULONG Id)
{
    COL_OBJ *p;
    ULONG No;
    ULONG Old;

    No = 1;
    for (p = Oba_work.pTop; p != NULL; p = p->pNext) {
        if (p == pObj) {
            return;
        }
    }
    pObj->pNext = NULL;
    pObj->pSca_hit_data = 0;
    pObj->Unk2A = 0;
    pObj->Unk2C = 0;
    if (Id != 0) {
        pObj->Id = Id;
    } else {
        do {
            Old = No;
            for (p = Oba_work.pTop; p != NULL; p = p->pNext) {
                if (p->Id == No) {
                    No++;
                    break;
                }
            }
        } while (No != Old);
        pObj->Id = No;
    }
    pObj->Hit_flg = 0;
    if ((p = Oba_work.pTop) != NULL) {
        Oba_work.pTop = pObj;
        pObj->pNext = p;
    } else {
        Oba_work.pTail = pObj;
        Oba_work.pTop = pObj;
    }
    Oba_hit_clr(pObj);
    pObj->pM = NULL;
    pObj->Unk39 = 8;
    pObj->Old_x = pObj->Pos_x;
    pObj->Old_y = pObj->Pos_y;
    pObj->Old_z = pObj->Pos_z;
    pObj->W1 = pObj->W0;
    pObj->D1 = pObj->D0;
    if (pObj->Type < 3) {
        pObj->Em_idx = Em_work_no_get(pObj->pEm);
    }
    return pObj->Id;
}

/* index of pEm in G.pEnemy[] (0 if absent) */
LONG Em_work_no_get(ENEMY_WORK *pEm)
{
    LONG i;

    for (i = 0; i < 42; i++) {
        if (G.pEnemy[i] == pEm) {
            return i;
        }
    }
    return 0;
}

/* collect the live collision objects into COL_ENT records (two passes) */
void Oba_ent_collect(COL_OBJ *pTop, COL_ENT *pEnt)
{
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    ULONG Be;
    LONG n;
    LONG Pass;

    n = 0;
    for (Pass = -1; Pass != 1; Pass++) {
        for (p = Oba_work.pTop; p != NULL; p = p->pNext) {
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
                pOba_list[n++] = p;
            } while (0); /* needed for register allocation */
        }
    }
    pOba_list[n] = NULL;
}

/* per object: keep the sticky Hit_flg bits, test it against the records */
void Oba_hit_ck_all(COL_OBJ *pTop, COL_ENT *pEnt)
{
    COL_OBJ **pp;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    USHORT Flg;
    UCHAR R;
    UCHAR Type;
    UCHAR Idx;
    LONG i;

    for (i = 0; (p = pOba_list[i]) != NULL; i++) {
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
            pEnt = Oba_ent_box_set(p, pEnt);
        } else {
            pEnt->Id = 0xFE40;
            pEnt->Area = 0xF00;
            pEnt->C.All = 0;
            Oba_ent_floor_set(p, pEnt);
        }
        pEnt++;
    }
    pEnt->C.All = -1;
}

/* no return value: v0 stays out of the last delay slot */
LONG Oba_push_all(COL_OBJ *pTop, COL_ENT *pEnt)
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
    if ((p = pOba_list[0]) != NULL) {
        do {
            pEnt = Oba_work.Ent;
            pEm = p->pEm;
            while (pEnt->pObj != p) {
                pEnt++;
            }
            Tbl = -1;
            if (p->Type < 5) {
                Tbl = Oba_att_tbl[p->Type];
                if (p->Type < 3 && (pEm->Damage_cnt & 0x80)) {
                    p->Hit_flg |= 0x8000;
                }
            }
            if (p->Type < 3) {
                Oba_em_hit_ck(pEm, p->W0, Tbl, p);
            } else {
                Oba_area_ck(pEm, p->W0, Tbl, p);
            }
            if (!(p->Flg & 0x80)) {
                Sca_hit_all(pEm, p->W0, Tbl, p);
            }
            if (p->Type < 3) {
                Oba_em_overlap_ck(pEm, p->W0, Tbl, p);
            }
            p->Hit_flg |= 0x4000;
            if (p->Hit_flg & 0x100) {
                w0 = p->Old_x;
                w1 = p->Old_y;
                w2 = p->Old_z;
                p->Pos_x = w0;
                p->Pos_y = w1;
                p->Pos_z = w2;
                w0 = p->pEm->Old_x;
                w1 = p->pEm->Old_z;
                p->pEm->Pos_x = w0;
                p->pEm->Pos_z = w1;
                if (p->Type < 3) {
                    pEm->Base_pos_x = w0;
                    pEm->Base_pos_z = w1;
                }
                dz = dx = 0;
            } else {
                w2 = pEm->Pos_y;
                dx = -pEm->Old_x + (ex = pEm->Pos_x);
                dz = -pEm->Old_z + (ez = pEm->Pos_z);
                pEm->Old_x = ex;
                pEm->Old_z = ez;
                pEm->Old_y = w2;
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
        } while ((p = pOba_list[++i]) != NULL);
    }
}

/* per frame: collect, update positions, test, push, write back */
void Oba_main(void)
{
    COL_OBJ *pTop;

    pTop = Oba_work.pTop;
    Oba_ent_collect(pTop, Oba_work.Ent);
    Oba_pos_update(pTop);
    Oba_hit_ck_all(pTop, Oba_work.Ent);
    Oba_push_all(pTop, Oba_work.Ent);
    Oba_pos_back(pTop);
}

/* write the resolved positions back to the works */
void Oba_pos_back(COL_OBJ *pTop)
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
    for (i = 0; (p = pOba_list[i]) != NULL; i++) {
        pEm = p->pEm;
        if (pEm->Be_flg & 0x2000) {
            continue;
        }
        if (p->Type >= 3) {
            continue;
        }
        Tbl = Oba_att_tbl[p->Type];
        W = p->W0;
        pV->vx = p->Pos_x;
        pV->vz = p->Pos_z;
        pV->vy = y = p->Pos_y;
        if (pEm->Unk1B8 != 0) {
            pV->vy = y - pEm->Unk1B8;
        }
        Sca_work.Unk8 = 0;
        p->Unk39 &= ~8;
        if (p->Hit_flg & 0x80) {
            Sca_set_ground(pEm, p, Sca_get_ground(&pV->vx, W, Tbl, 9));
        }
        if (Sca_work.Unk8 == 0) {
            p->Unk39 |= 8;
            p->Hit_flg &= ~0x40;
            if (p->Type < 3) {
                pEm->Tilt_flg = p->Unk39;
            }
        }
    }
}

/* writes the COL_ENT records of a box-shaped object (whole box + four quarter boxes via Oba_ent_set); returns the last record */
INCLUDE_ASM("asm/nonmatchings/game/oba", Oba_ent_box_set);

/* fill a COL_ENT for pObj (shape 8, all areas) with its floor range */
void Oba_ent_set(COL_OBJ *pObj, COL_ENT *pEnt, LONG n)
{
    SVECTOR Unused[3];
    LONG y;

    pEnt->Type = pObj->Type;
    pEnt->Id = 0x8008;
    pEnt->pObj = pObj;
    pEnt->Area = 0xF00;
    Oba_ent_floor_set(pObj, pEnt);
    COL_ENT_SET_HIGH(pEnt, pObj, y, n);
}

/* reset the per-frame hit state and take the positions from the matrices */
void Oba_pos_update(COL_OBJ *pTop)
{
    LONG i;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    SVECTOR Sv;
    VECTOR V;
    MATRIX M;

    for (i = 0; (p = pOba_list[i]) != NULL; i++) {
        p->Unk3 = 0xFF;
        pEm = p->pEm;
        p->Unk2A = 0;
        p->Unk2C = 0;
        p->Unk3A = 0;
        if (p->pM != NULL) {
            M = GsIDMATRIX;
            MulMatrix2(p->pM, &M);
        } else {
            RotMatrix_gte(CDIR_SV(pEm), &M);
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

/* clear the hit state of one object and re-read its position */
void Oba_hit_clr(COL_OBJ *pObj)
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
        pEm->Ground = Sca_get_ground(&V.vx, pObj->W0, Oba_att_tbl[pObj->Type], 9);
    }
}

/* floor range of a record from the room shapes around the object */
void Oba_ent_floor_set(COL_OBJ *pObj, COL_ENT *pEnt)
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

/* is pT within reach in front of pEm? */
LONG Oba_front_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SVECTOR Sv;
    LONG w;
    LONG d;
    LONG dx;
    LONG dz;
    LONG ax;
    LONG az;

    Sv.vx = pEm->Col.W0 * 120 / 100 + 0x110;
    Sv.vz = 0;
    Rot_vector(pEm->Cdir_y, &Sv, &Sv);
    Sv.vx += pEm->Pos_x;
    Sv.vz += pEm->Pos_z;
    w = pT->Col.W0 - 0x110;
    d = pT->Col.D0 - 0x110;
    dx = pT->Pos_x - pEm->Pos_x;
    dz = pT->Pos_z - pEm->Pos_z;
    if ((ULONG)(dx + w) > (ULONG)(w * 2) && (ULONG)(dz + d) > (ULONG)(d * 2)) {
        return 0;
    }
    ax = __builtin_abs(dx);
    az = __builtin_abs(dz);
    if (az < ax) {
        dx = Sv.vx - pT->Col.Pos_x;
        dz = pEm->Pos_z - pT->Col.Pos_z;
        d += pEm->Col.W0;
    } else {
        dx = pEm->Pos_x - pT->Col.Pos_x;
        dz = Sv.vz - pT->Col.Pos_z;
        w += pEm->Col.W0;
    }
    if ((ULONG)(dx + w) > (ULONG)(w * 2)) {
        return 0;
    }
    return !((ULONG)(dz + d) > (ULONG)(d * 2));
}

/* does pPos lie inside another live enemy than pEm? */
LONG Oba_pos_em_ck(VECTOR *pPos, ENEMY_WORK *pEm)
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
        if (pT->Col.Id == pEm->Col.Id) {
            continue;
        }
        if (Oba_pos_em2_ck(pPos, pEm, pT)) {
            return 1;
        }
    }
    return 0;
}

/* does pPos lie inside pT (as seen by pEm)? */
LONG Oba_pos_em2_ck(VECTOR *pPos, ENEMY_WORK *pEm, ENEMY_WORK *pT)
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
    pA = &pEm->Col;
    pB = &pT->Col;
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

/* character (Type<3) object against the collected COL_ENT records: overlap tests, Oba_box_push, push flags, Oba_push_obj_ck */
INCLUDE_ASM("asm/nonmatchings/game/oba", Oba_em_hit_ck);

/* player against a pushable object record (Type 7 / 8) */
void Oba_push_obj_ck(COL_OBJ *pObj, COL_ENT *pEnt, ULONG Arg2, ULONG Type)
{
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    if (Oba_front_ck(pPl, pEnt->pObj->pEm)) {
        if (Type == 7 && !(pObj->Hit_flg & 0x600)) {
            if (Pl_floor_ck(pEnt, 7, 0) == 0) {
                pEnt->pObj->Hit_flg |= 0x10;
                pEnt->pObj->Unk3A |= 0x8000;
            } else {
                pPl->Unk1C8 = 1;
            }
        }
    } else if (Type == 8 && !(pObj->Hit_flg & 0x600)) {
        if (Pl_floor_ck(pEnt, 8, 0)) {
            pPl->Unk1C8 = 0;
        }
    }
}

/* fills Tmp_Sca (SCA_DATA) with the box of a COL_OBJ (Id 0xFE40, all areas) for Sca_hit_all */
INCLUDE_ASM("asm/nonmatchings/game/oba", Oba_tmp_sca_set);

/* after Sca_hit_all: still overlapping another character record -> Hit_flg |= 0x100 (move reverted in Oba_push_all) */
INCLUDE_ASM("asm/nonmatchings/game/oba", Oba_em_overlap_ck);

/* no return value; pE and pEnt walk the same list (two pointers in the original) */
LONG Oba_area_ck(ENEMY_WORK *pEm, ULONG W, ULONG Tbl, COL_OBJ *pObj)
{
    COL_ENT *pEnt;
    COL_ENT *pE;
    COL_OBJ *pT;
    ULONG Area;
    ULONG Be;
    ULONG Tbe;
    LONG Shape;

    pE = pEnt = Oba_work.Ent;
    Area = Sca_get_area((SHORT)pObj->Pos_x, (SHORT)pObj->Pos_z);
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
            if (pT->Type == 0 && (ULONG)(G.Player.Routine_1 - 11) < 2) {
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
        if (pT->Type == 0 && (pObj->pEm->Be_flg & 0x100) && pT->pEm->Damage_cnt == 0 && Oba_cross_ck(pObj, pT) == 0) {
            pObj->Hit_flg |= 0x2000;
        }
    }
}

/* does the move of pObj cross pT? */
LONG Oba_cross_ck(COL_OBJ *pObj, COL_OBJ *pT)
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
    Oba_push_out(pObj, pT);
    Ret = Sca_hit_all(pObj->pEm, pObj->W0, 0x4000, pObj);
    if (Ret != 0 || !(G_status_flg & 0x10) || pT->pEm->Damage_cnt != 0) {
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

/* push pObj (and its work) out of pT along x or z */
LONG Oba_push_out(COL_OBJ *pObj, COL_OBJ *pT)
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

/* pushes pObj out of one box record (fills Sca_work pos/old, picks the side, calls Sca_hosei_box) */
ULONG Oba_box_push(COL_OBJ *pObj, LONG W, SCA_DATA *pT, ULONG Old)
{
    LONG X;
    LONG Z;
    LONG Dx;
    LONG Dz;
    ULONG Side;
    LONG Hx;
    LONG Near;
    LONG Hz;
    ULONG Zero;
    ULONG Still; /* (matching: a copy of Zero) */

    Sca_work.X = X = pObj->Pos_x;
    Sca_work.Z = Z = pObj->Pos_z;
    Sca_work.Old_x = Dx = pObj->Old_x;
    Dx = X - Dx;
    Sca_work.Old_z = Dz = pObj->Old_z;
    Dz = Z - Dz;
    Side = (Dx < 0) | ((ULONG)Dz >> 30 & 2);
    Hx = pT->X1 + pObj->W0 - X + 1;
    Near = pT->X0 - pObj->W0 - X - 1;
    Zero = (Dx | Dz) == 0;
    Still = Zero;
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
