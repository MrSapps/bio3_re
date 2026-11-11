/* ---- from 800402F4.c ---- */
#include "pl_wep.h"
#include "sca.h"
#include "monitor.h"
#include "pl_wep2.h"
#include "pad.h"

/* RE2 Esp_call-like effect spawn; returns the effect work or -1 */
typedef struct {
    UCHAR pad0[0x10];
    SHORT Unk10;                       /* 0x10 */
    SHORT Unk12;                       /* 0x12 */
    SHORT Unk14;                       /* 0x14 */
} ESP_WORK;
ESP_WORK *func_8001B484(ULONG Id, ULONG Dir, MATRIX *pM, SVECTOR *pV, SVECTOR *pDsv);
SHORT func_8006D030(ULONG Arg0);
void func_80044804(MATRIX *pM, ULONG Arg1, ULONG Type);
void func_80047860(MATRIX *pM, ULONG Arg1, ULONG Type);
extern ULONG D_8009DBB4[];
ULONG func_800102E8(void);                 /* rand */
SHORT Get_dir_xz(LONG X, LONG Z);
LONG func_800455B4();
LONG func_8004EF74(ULONG P0, ULONG P1, ULONG P2, ULONG P3);

extern UCHAR D_8009CD3D;
LONG func_80026BE8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
signed char func_8006CF0C_r(ULONG Arg0) __asm__("func_8006CF0C");
void func_8003E088(ENEMY_WORK *pEm);

/* declarations from earlier parts of the original address-split file */
LONG func_80026BE8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
void func_8006CF0C(ULONG Arg0);
void func_80044F34(PLAYER_WORK *pEm);

void Pl_wep_flag_set(PLAYER_WORK *pEm);

#define RND() ((signed char)func_800102E8())

extern PARTS_W *D_8009CD30;
extern USHORT D_8009DCB4[];
void func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
void func_8007809C(LONG a0, LONG a1, ULONG a2, ULONG a3);

typedef struct {
    VECTOR *pPos;                      /* 0x00 */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} HIT_AREA;

extern ENEMY_WORK Pl_work;
extern UCHAR D_8009DDC4[][2];
extern UCHAR D_8009DE10;
extern UCHAR D_8009DE24;

extern UCHAR D_8009DE24;
extern USHORT D_8009DE26;
extern signed char D_8009DE28;

extern LONG D_800D204C[];

typedef struct {
    ULONG Tag;                         /* 0x00 */
    ULONG Tpage;                       /* 0x04 */
    ULONG pad8;
    ULONG Code;                        /* 0x0C */
    UCHAR pad10[0x1C - 0x10];
} PRIM_1C;
typedef struct {
    UCHAR *p0;                         /* 0x00 */
    UCHAR *p1;                         /* 0x04 */
    PRIM_1C *pPrim;                    /* 0x08 */
    UCHAR padC[0x16 - 0xC];
    SHORT Unk16;                       /* 0x16 */
    UCHAR pad18[0x20 - 0x18];
    UCHAR Unk20;                       /* 0x20 */
    UCHAR Unk21;                       /* 0x21 */
} W_DEB18;
extern W_DEB18 D_800DEB18;
void func_80010150(void *p, ULONG Size);
void func_80048FF4(void);

ULONG func_800102E8(void);
extern USHORT **D_800CC86C;

extern SHORT D_800DEB60;

void func_8004B7E8(ENEMY_WORK *pEm, LONG Arg1, LONG Y)
{
    pEm->Unk122 = Y;
    pEm->nFloor = Y / -1800;
    if (!(pEm->Be_flg & 0x400)) {
        pEm->Base_pos_y = pEm->Pos_y = pEm->Unk122;
    }
    if (D_800DEB60 != 0) {
        pEm->Pos_y += pEm->Unk1B8;
        if (pEm->Unk122 < pEm->Pos_y) {
            pEm->Pos_y = pEm->Unk122;
        }
    } else {
        pEm->Unk1B8 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004B874);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004BB4C);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C408);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C57C);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C6EC);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C85C);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004C960);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004CC54);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004CE2C);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004D194);

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

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004D590);

LONG func_8004D6B0(void)
{
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004D6B8);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004D720);

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

LONG func_8004DEA8(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);

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




LONG Sca_high_naname_a(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_b(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_c(SCA_DATA *pT, VECTOR *pPos, LONG High);
LONG Sca_high_naname_d(SCA_DATA *pT, VECTOR *pPos, LONG High);

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



LONG func_8004E10C(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X;
    LONG Z;
    LONG Lo;
    LONG Hi;
    LONG Range;

    X = pPos->vx;
    Z = pPos->vz;
    Lo = -(*(signed char *)&pT->Floor * 1800);
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

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004E230);

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
    Sl = h + *(signed char *)&pT->Floor * 1800;
    d = SquareRoot0(dx * dx + dz * dz);
    if (R - d > 0) {
        return (SHORT)h - Sl * d / R;
    }
    return High;
}

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004E830);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004ED4C);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004EF74);

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

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004F5B4);

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

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004F928);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8004FBD4);

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
    lo = -(*(signed char *)&pT->Floor * 1800) >> 1;
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
/* ---- from 8005003C.c ---- */
#include "sce0.h"
#include "em_sub.h"
#include "main.h"
#include "sca.h"
#include "line_scr.h"
#include "sub.h"
#include "player.h"
#include "om.h"



extern UCHAR D_800D442C[];
extern UCHAR D_800E017F[];
extern ULONG Old_Fatari;               /* 0x800DECB4 RE2 Old_Fatari */
extern ULONG Old_Uatari;               /* 0x800DECB8 RE2 Old_Uatari */
extern ULONG *Flg_addr[];              /* 0x8009E3F8 RE2 Flg_addr */
extern UCHAR D_800DBB58;
LONG Mess_set(ULONG A0, ULONG Attr, ULONG No, ULONG Flg); /* 0x8002FD30 RE2 Mess_set */
void func_800513CC();
void func_80051D60();
extern UCHAR D_800E01C4[];
void func_800514F0();
extern UCHAR *pCAot;                    /* 0x800DECB0 RE2 pCAot */
void func_800511B0();
extern LONG D_800DEC98;
LONG func_8006CD68(UCHAR No);
s8 func_8006CCF0(UCHAR No, ULONG A1, ULONG A2);
extern UCHAR D_800E017E[];
void func_80059A20();
s8 func_8006CC8C(UCHAR Id, UCHAR A1);  /* RE2 Search_item_id */
extern UCHAR D_800C703E[];
extern UCHAR D_800D442E[];
extern UCHAR D_800E0175[];
LONG func_80048974();
LONG func_800321C4();
LONG Quake_add();
LONG Pad_vib_set0_8005003C() __asm__("Pad_vib_set0");
LONG Pad_vib_set1_8005003C() __asm__("Pad_vib_set1");
LONG Pad_vib_fade_set_8005003C() __asm__("Pad_vib_fade_set");
LONG func_8001B894_8005003C() __asm__("func_8001B894");
void func_80078498(UCHAR A0, UCHAR A1, USHORT A2);
extern UCHAR D_8009E340[];
extern UCHAR D_800E0170;
typedef struct {
    void *pData;                       /* 0x0 */
    UCHAR pad4;
    UCHAR Id;                          /* 0x5 */
    UCHAR pad6[2];
} SCE_UNK_DECC0;
extern SCE_UNK_DECC0 D_800DECC0[];
LONG Xa_play();
typedef void (*EM_FUNC)(ENEMY_WORK *pEm);
typedef void (*EM_FUNC3)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern EM_FUNC D_8009ECDC[];
extern EM_FUNC D_8009ECF8[];
extern EM_FUNC3 D_8009ED24[];
extern EM_FUNC3 D_8009ED84[];
extern EM_FUNC3 D_8009E7DC[];
extern EM_FUNC3 D_8009E82C[];
extern EM_FUNC3 D_8009E87C[];
extern EM_FUNC3 D_8009E8CC[];
extern EM_FUNC3 D_8009E91C[];
extern EM_FUNC3 D_8009E96C[];
extern EM_FUNC3 D_8009E9BC[];
extern EM_FUNC3 D_8009EA0C[];
extern EM_FUNC3 D_8009EA5C[];
extern EM_FUNC3 D_8009EAAC[];
extern EM_FUNC3 D_8009EAFC[];
extern EM_FUNC3 D_8009EB4C[];
extern EM_FUNC3 D_8009EB9C[];
extern EM_FUNC3 D_8009EBEC[];
typedef struct {
    UCHAR pad0[4];
    UCHAR St_type;                     /* 0x04 */
    UCHAR pad5[0xB8 - 0x5];
    SHORT UnkB8;                       /* 0xB8 */
} ST_WORK;
extern ST_WORK St;                     /* 0x800E01C0 RE2 St */
extern UCHAR D_800A0FDC[];
extern SHORT D_800D1F8E[];
extern SHORT D_800D1F82;
extern s8 D_80098728[];
UCHAR func_800102E8_8005003C(void) __asm__("func_800102E8");             /* sld.s Rnd? */
void func_80034124(void *p);
LONG func_80055E38(s8 Work_kind, s8 Work_no);
void func_80058DD4();
extern ENEMY_WORK *D_800E01A8;
extern USHORT D_800E01B4;
void func_80052968(void);
void func_80052BA4(void);
void func_80052588(void);
void func_8001BA48();
void func_80052644(void);
void func_8005283C(void);
LONG func_80026BE8(ENEMY_WORK *pEm, ULONG A1, ULONG A2, ULONG A3);
void func_80079690(ENEMY_WORK *pEm, ULONG A1, ULONG A2, ULONG A3);
void func_80079868(ENEMY_WORK *pEm, ULONG A1);
extern UCHAR D_800E01C6[];
void func_80019318(ENEMY_WORK *pEm, ULONG A1, ULONG A2);
typedef struct {
    LONG Unk0;                         /* 0x00 */
    LONG Pos[3];                       /* 0x04 */
    LONG Tgt[3];                       /* 0x10 */
} SCE_CAMERA;
extern SCE_CAMERA D_800E0150;
LONG Set_view(LONG *pPos, LONG *pTgt);
typedef struct {
    UCHAR Unk0;                        /* 0x00 */
    UCHAR Unk1;                        /* 0x01 */
    UCHAR pad2[0x24 - 0x2];
    SHORT Unk24;                       /* 0x24 */
    UCHAR pad26[0xC0 - 0x26];
    LONG UnkC0;                        /* 0xC0 */
    UCHAR padC4[0xD4 - 0xC4];
} SCE_UNK_D4;
extern SCE_UNK_D4 D_800BA8A8[];
extern UCHAR D_8009E70C[];
extern UCHAR D_8009E70D[];
extern UCHAR D_8009EC3C[];
extern void (*D_8009ECAC[])(ENEMY_WORK *pEm, ULONG Type);
void func_8001B484_8005003C(ULONG A0, ULONG A1, LONG A2, SVECTOR *pSv, ULONG A4) __asm__("func_8001B484");
typedef struct {
    ULONG Be_flg;                      /* 0x000 */
    UCHAR pad4[0x20 - 0x4];
    LONG Pos[3];                       /* 0x020 */
    UCHAR pad2C[0x4A - 0x2C];
    UCHAR Attr;                        /* 0x04A */
    UCHAR pad4B[0xC1 - 0x4B];
    UCHAR UnkC1;                       /* 0x0C1 */
    UCHAR padC2;
    UCHAR UnkC3;                       /* 0x0C3 */
    UCHAR padC4[0x194 - 0xC4];
} SCE_OM_WORK;
extern ULONG (*Sce_at_jump_tbl[])(void *pAot); /* 0x8009E0BC RE2 Sce_at_jump_tbl */
extern ENEMY_WORK *D_800E01AC;
extern USHORT D_800E01B6;
ENEMY_WORK *func_8005F7E8();
ENEMY_WORK *func_8005F8F0();
extern UCHAR D_800A0514[];
typedef struct {
    UCHAR Unk0;
    UCHAR Max;                         /* 0x1 */
    UCHAR pad2[2];
} ITEM_TBL4;
extern UCHAR D_8009CDE0[];
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG A3);
ULONG func_80027940(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void func_800776B0(ULONG A0, ULONG No, ENEMY_WORK *pEm);
void Flg_on(ULONG *pFlg, ULONG Bit);
LONG Flg_off(ULONG *pFlg, ULONG Bit);
LONG Weapon_cls(ENEMY_WORK *pEm);
LONG func_80043BE4(ENEMY_WORK *pEm);
LONG Pl_weapon_ch_init(ENEMY_WORK *pEm);
extern UCHAR D_800E01E8;
extern UCHAR D_8009E710[];
extern void (*D_8009E720[])(ENEMY_WORK *pEm);
ULONG func_800632B8(ENEMY_WORK *pEm, ULONG A1);
LONG func_800261AC(ENEMY_WORK *pEm, ULONG A1, void *A2);
extern USHORT D_800D1F94;
extern ULONG D_800D258C[];
ULONG func_800634F4(ENEMY_WORK *pEm, ULONG A1);
LONG Weapon_parts_clr(ENEMY_WORK *pEm);
extern UCHAR D_800E0171[];
extern ULONG D_800E01D4[];
extern ULONG D_8009DBB4_8005003C __asm__("D_8009DBB4");
LONG func_80044804_8005003C(void *p, ULONG A1, ULONG A2) __asm__("func_80044804");
extern ULONG D_800DECAC;
ULONG func_800505AC(void *pEm, ULONG Attribute, ULONG Auto_flg); /* RE2 Sce_at_check */
extern UCHAR D_8009E6BC[];
extern SHORT D_8009E6D4[];
extern SHORT D_8009E6DC[];
extern SHORT D_8009E6E4[];
extern void (*D_8009E6EC[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80059824(ENEMY_WORK *pEm);
void func_8001A248(ENEMY_WORK *pEm, LONG Dir);
void func_8005F298();
ULONG func_8005F660(ENEMY_WORK *pEm, ULONG A1);
extern UCHAR D_8009E7D8[];
extern UCHAR D_8009E7C8[];
extern void (*D_8009E788[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
ULONG Flg_ck(ULONG *pFlg, ULONG Bit);
extern ULONG *D_8009E414;              /* Flg_addr[7] */
extern ULONG *D_8009E418;              /* Flg_addr[8] */
void func_8005C098(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_800549C4(ULONG Cut);

/* ---- end externs ---- */

INCLUDE_ASM("asm/nonmatchings/game/sca", func_8005003C);

void func_80050208(ULONG Type, ULONG Id, ULONG Floor)
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
        if ((pData->Type & Type) && (pData->Id & Id) && Floor >= ((s8 *)&pData->Floor)[0] && ((s8 *)&pData->Floor)[1] >= Floor) {
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

INCLUDE_ASM("asm/nonmatchings/game/sca", func_800502B8);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_800503E8);

INCLUDE_ASM("asm/nonmatchings/game/sca", func_80050524);
