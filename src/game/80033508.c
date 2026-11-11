#include "common.h"
#include "monitor.h"
#include "mojidisp.h"
#include "pad.h"
#include "em_sub.h"
#include "om.h"
#include "pl_wep2.h"
#include "movie.h"
#include "player.h"
#include "cd.h"

/* RE3-only collision object list (80033508..80035790). Nodes are linked through pNext;
 * the player's node lives at Pl_work + 0x80. */
typedef struct tagCOL_OBJ {
    UCHAR Id;                          /* 0x00 */
    UCHAR Flg;                         /* 0x01 */
    UCHAR Type;                        /* 0x02 0 = player, < 3 = character */
    UCHAR Unk3;                        /* 0x03 */
    ENEMY_WORK *pEm;                   /* 0x04 */
    LONG Pos_x;                        /* 0x08 */
    LONG Pos_y;                        /* 0x0C */
    LONG Pos_z;                        /* 0x10 */
    SHORT Old_x;                       /* 0x14 */
    SHORT Old_y;                       /* 0x16 */
    SHORT Old_z;                       /* 0x18 */
    USHORT W0;                         /* 0x1A */
    USHORT D0;                         /* 0x1C */
    USHORT H;                          /* 0x1E */
    USHORT W1;                         /* 0x20 */
    USHORT D1;                         /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    SHORT Unk26;                       /* 0x26 */
    SHORT Unk28;                       /* 0x28 */
    SHORT Unk2A;                       /* 0x2A */
    SHORT Unk2C;                       /* 0x2C */
    USHORT Hit_flg;                    /* 0x2E */
    MATRIX *pM;                        /* 0x30 */
    struct tagCOL_OBJ *pNext;          /* 0x34 */
    UCHAR Em_idx;                      /* 0x38 */
    UCHAR Unk39;                       /* 0x39 */
    SHORT Unk3A;                       /* 0x3A */
    LONG Unk3C;                        /* 0x3C */
} COL_OBJ;

typedef struct tagCOL_ENT {            /* 0x18 bytes */
    SHORT X0;                          /* 0x00 */
    SHORT Z0;                          /* 0x02 */
    SHORT X1;                          /* 0x04 */
    SHORT Z1;                          /* 0x06 */
    USHORT Unk8;                       /* 0x08 */
    USHORT UnkA;                       /* 0x0A */
    LONG UnkC;                         /* 0x0C -1 terminates */
    UCHAR Type;                        /* 0x10 */
    UCHAR Em_idx;                      /* 0x11 */
    UCHAR pad12[2];
    COL_OBJ *pObj;                     /* 0x14 */
} COL_ENT;

typedef struct tagCOL_WORK {
    COL_OBJ *pTop;                     /* 0x00 */
    COL_OBJ *pTail;                    /* 0x04 */
    COL_ENT Ent[1];                    /* 0x08 */
} COL_WORK;
extern COL_WORK D_800DCED0;
extern COL_OBJ *D_800DCDD0[];

/* ENEMY_WORK fields not in em_sub.h yet */
#define EM_SHORT(p, ofs) (*(SHORT *)((UCHAR *)(p) + (ofs)))
#define EM_UCHAR(p, ofs) (*(UCHAR *)((UCHAR *)(p) + (ofs)))

extern ULONG D_8009CBC8[];
extern SHORT D_800DEB58[];
extern COL_ENT D_800DEB48;
LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);
void func_8004B7E8(ENEMY_WORK *pEm, COL_OBJ *pObj, LONG Arg2);

ULONG Flg_ck(ULONG *pFlg, ULONG Id);

/* array inside G at an offset not yet in tagGLOBAL (keeps the &G + index*size + ofs addressing) */
#define G_ARR(type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)&G)->a)
#define P_ARR(p, type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)(p))->a)

typedef ULONG OT8[8];

extern ULONG D_800CC87C;
extern ULONG D_800D1F2C;
extern UCHAR D_8009CD3C;
ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Arg1);
LONG Hit_ck_point4_l(ULONG Arg0, ULONG Arg1) __asm__("func_8001020C");
void func_800288C8(ENEMY_WORK *pEm, PARTS_W *pParts, LONG Arg2, MATRIX *pM);

/* G.pUnk265C, G.pUnk2660 and G.pEnemy[40] seen as one array */
typedef struct {
    UCHAR pad0[0x265C];
    ENEMY_WORK *pWork[42];             /* 0x265C */
} G_WORKS;

UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);
LONG func_800336B4(ENEMY_WORK *pEm);
void func_80034124(COL_OBJ *pObj);
void func_800336F0(COL_OBJ *pTop, COL_ENT *pEnt);
void func_800337B8(COL_OBJ *pTop, COL_ENT *pEnt);
void func_8003389C(COL_OBJ *pTop, COL_ENT *pEnt);
void func_80033FBC(COL_OBJ *pTop);
void func_80033B88(COL_OBJ *pTop);
COL_ENT *func_80033CF4(COL_OBJ *pObj, COL_ENT *pEnt);
void func_80034284(COL_OBJ *pObj, COL_ENT *pEnt);

/* declarations from earlier parts of the original address-split file */
extern UCHAR D_800A0514[][4];
extern ULONG D_800D2048[];
extern UCHAR D_8009BEE4[];
extern USHORT D_8009C7C0[];

extern USHORT D_80099654[];
extern UCHAR D_80098E88[];
extern UCHAR D_800CC84D;
extern UCHAR D_800D4433;
void func_80031504(ULONG x, ULONG y, UCHAR *pStr, ULONG Attr);

static __inline__ UCHAR *Msg_get_80031BDC(ULONG No)
{
    return D_80098E88 + D_80099654[No];
}

ULONG func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
extern LOADTASK D_8009C944[];
ULONG Task_lexecute_(ULONG Level, ULONG Task_no) __asm__("Task_lexecute");

void Task_lchain_(ULONG Task_no) __asm__("Task_lchain");

extern void (*D_8009CBB4[])(void);

void func_80033508(void)
{
    ENEMY_WORK *pEm;
    COL_OBJ *pObj;

    D_800DCED0.pTail = NULL;
    D_800DCED0.pTop = NULL;
    D_800DCED0.Ent[0].UnkC = -1;
    pEm = &Pl_work;
    pObj = (COL_OBJ *)((UCHAR *)pEm + 0x80);
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
        if (((G_WORKS *)&G)->pWork[i] == pEm) {
            return i;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_800336F0);

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
            pEnt->Unk8 = 0xFE40;
            pEnt->UnkA = 0xF00;
            pEnt->UnkC = 0;
            func_80034284(p, pEnt);
        }
        pEnt++;
    }
    pEnt->UnkC = -1;
}

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_8003389C);

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
        if (EM_SHORT(pEm, 0x1B8) != 0) {
            pV->vy = y - EM_SHORT(pEm, 0x1B8);
        }
        D_800DEB58[4] = 0;
        p->Unk39 &= ~8;
        if (p->Hit_flg & 0x80) {
            func_8004B7E8(pEm, p, func_8004D720(pV, W, Tbl, 9));
        }
        if (D_800DEB58[4] == 0) {
            p->Unk39 |= 8;
            p->Hit_flg &= ~0x40;
            if (p->Type < 3) {
                EM_UCHAR(pEm, 0x1C1) = p->Unk39;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_80033CF4);

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_80033F38);

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
        EM_SHORT(pEm, 0x122) = func_8004D720(&V, pObj->W0, D_8009CBC8[pObj->Type], 9);
    }
}

void func_80034284(COL_OBJ *pObj, COL_ENT *pEnt)
{
    SHORT *pCam;
    LONG x0;
    LONG x1;
    LONG z0;
    LONG z1;
    LONG px;
    LONG pz;

    pCam = *(SHORT **)((UCHAR *)G.pRoom + 0x20);
    x0 = pObj->Old_x - pObj->W0 - 900;
    x1 = pObj->Old_x + pObj->W0 + 900;
    z0 = pObj->Old_z - pObj->W0 - 900;
    z1 = pObj->Old_z + pObj->W0 + 900;
    px = pCam[2];
    pz = pCam[3];
    if (px <= x0 || px <= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 1;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 4;
        }
    }
    if (px >= x0 || px >= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 2;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 8;
        }
    }
    px = pCam[4];
    pz = pCam[5];
    if (px <= x0 || px <= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 0x10;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 0x40;
        }
    }
    if (px >= x0 || px >= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 0x20;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 0x80;
        }
    }
}

#define EM_COL(p) ((COL_OBJ *)((UCHAR *)(p) + 0x80))
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

LONG func_800346AC(COL_OBJ *pObj, ENEMY_WORK *pEm, ENEMY_WORK *pT);

LONG func_800345C0(COL_OBJ *pObj, ENEMY_WORK *pEm)
{
    ENEMY_WORK **pp;
    ENEMY_WORK *pT;
    ULONG Be;

    if (pEm->Be_flg & 0x4000) {
        return 0;
    }
    for (pp = &G.pUnk265C; pp != G.ppEnemy_end; pp++) {
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
        if (EM_UCHAR(pT, 0x80) == EM_UCHAR(pEm, 0x80)) {
            continue;
        }
        if (func_800346AC(pObj, pEm, pT)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_800346AC);

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_80034814);

LONG func_80034448(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG func_8004B374(COL_ENT *pEnt, ULONG Type, ULONG Arg2);

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
                EM_SHORT(pPl, 0x1C8) = 1;
            }
        }
    } else if (Type == 8 && !(pObj->Hit_flg & 0x600)) {
        if (func_8004B374(pEnt, 8, 0)) {
            EM_SHORT(pPl, 0x1C8) = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_80034E78);

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_80034F08);

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_80035184);

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_800353CC);

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_8003553C);

INCLUDE_ASM("asm/nonmatchings/game/80033508", func_80035678);
