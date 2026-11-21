#include "pl_wep.h"
#include "sca.h"
#include "monitor.h"
#include "pl_wep2.h"
#include "pad.h"
#include "sub.h"

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
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
signed char func_8006CF0C_r(ULONG Arg0) __asm__("func_8006CF0C");
void Pl_water(ENEMY_WORK *pEm);

/* declarations from earlier parts of the original address-split file */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
void func_8006CF0C(ULONG Arg0);
void Mag_set(PLAYER_WORK *pEm);

void Pl_wep_flag_set(PLAYER_WORK *pEm);

#define RND() ((signed char)func_800102E8())
LONG Rnd_l(void) __asm__("func_800102E8");

extern PARTS_W *D_8009CD30;
extern USHORT D_8009DCB4[];
void Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
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

typedef struct {
    ULONG Tag;
    ULONG Tpage;
    ULONG Code;
    ULONG Xy;
    ULONG Uv;
    ULONG Wh;
} PSP_PRIM;
typedef struct {
    USHORT Uv;
    UCHAR X;
    UCHAR Y;
    USHORT Otz;
    USHORT Wh;
    ULONG Size;
} PSP_REC;
extern SHORT *D_800DEB0C;               /* per-sprite OT z */
extern UCHAR *D_800DEB08;               /* per-sprite group id (Cut_flg bit) */
extern PSP_PRIM *D_800DEB10;            /* two prims per sprite (double buffer) */

void func_80048630(void)
{
    tagGLOBAL *pG;
    UCHAR *p;
    ULONG n;
    PSP_PRIM *pPrim;
    PSP_PRIM *pP;

    pG = &G;
    n = pG->pRoom->nSprite_max;
    p = (UCHAR *)pG->pEm_mem;
    D_800DEB0C = (USHORT *)p;
    p += n * 2;
    D_800DEB08 = p;
    D_800DEB10 = pPrim = (PSP_PRIM *)(((ULONG)(p + n) + 3) & ~3);
    n *= 2;
    pG->pEm_mem = (struct tagENEMY_WORK *)(pPrim + n);
    pP = pPrim;
    while (n--) {
        pP->Tpage = 0xE1000095;
        pP->Code = 0x64808080;
        pP++;
    }
}

extern LONG D_800D204C[];

void func_800486C8(void)
{
    LONG *p = D_800D204C;
    LONG i = 32;

    do {
        *p++ = -1;
    } while (--i != 0);
}

void Psp_set(void)
{
    ROOM_WORK *pRoom;
    USHORT *pSp;
    PSP_REC *pRec;
    SHORT *pOtz;
    UCHAR *pId;
    PSP_PRIM *pPrim;
    ULONG nGrp;
    ULONG Grp;
    ULONG n;
    LONG ox;
    LONG oy;
    ULONG Xy;
    ULONG Uv;
    ULONG Wh;
    LONG j;

    pRoom = G.pRoom;
    pSp = (USHORT *)pRoom->pRcut[G.Cut_no].pSp;
    if (*(LONG *)pSp == -1) {
        pRoom->nSprite = 0;
        return;
    }
    pOtz = D_800DEB0C;
    pId = D_800DEB08;
    pPrim = D_800DEB10;
    nGrp = pSp[0];
    pRoom->nSprite = ((UCHAR *)pSp)[2];
    pSp += 2;
    pRec = (PSP_REC *)(pSp + nGrp * 4);
    for (Grp = 0; Grp < nGrp; Grp++) {
        n = pSp[0];
        ox = (SHORT)pSp[2];
        oy = (SHORT)pSp[3];
        while (n--) {
            *pOtz = pRec->Otz;
            *pId = Grp;
            Wh = pRec->Wh;
            Xy = (ox + pRec->X) | ((oy + pRec->Y) << 16);
            Uv = pRec->Uv + 0x78000000;
            if (Wh != 0) {
                Wh |= Wh << 16;
                pRec = (PSP_REC *)&pRec->Size;
            } else {
                Wh = pRec->Size;
                pRec++;
            }
            for (j = 2; j != 0; j--) {
                pPrim->Xy = Xy;
                pPrim->Uv = Uv;
                pPrim->Wh = Wh;
                pPrim++;
            }
            pOtz++;
            pId++;
        }
        pSp += 4;
    }
}

void Psp_trans(void)
{
    tagGLOBAL *pG;
    SHORT *pOtz;
    UCHAR *pId;
    PSP_PRIM *pPrim;
    ULONG *pFlg;
    ULONG *pOt;
    ULONG n;
    ULONG Tag;

    pOtz = D_800DEB0C;
    pG = &G;
    n = pG->pRoom->nSprite;
    pId = D_800DEB08;
    pPrim = &D_800DEB10[pG->Cbuf_id];
    pFlg = &pG->Cut_flg[pG->Cut_no];
    while (n--) {
        if (Flg_ck(pFlg, *pId)) {
            if ((USHORT)*pOtz < 0x400) {
                pOt = pG->pOt1 + (USHORT)*pOtz;
            } else {
                pOt = pG->pOt_sub[pG->Cbuf_id][(*pOtz & 0xFFFF) >> 10] + (*pOtz & 0x3FF);
            }
            Tag = *pOt;
            *pOt = (ULONG)pPrim & 0xFFFFFF;
            pPrim->Tag = Tag | 0x05000000;
        }
        pPrim += 2;
        pOtz++;
        pId++;
    }
}

/* rain drop primitive: draw mode word + LINE_G2 */
typedef struct {
    ULONG Tag;                         /* 0x00 */
    ULONG Tpage;                       /* 0x04 */
    ULONG pad8;
    ULONG Code;                        /* 0x0C rgb0 + code */
    ULONG Xy0;                         /* 0x10 */
    ULONG Rgb1;                        /* 0x14 */
    ULONG Xy1;                         /* 0x18 */
} PRIM_1C;
typedef struct tagPSP_POS {
    LONG x;
    LONG y;
    LONG z;
} PSP_POS;
typedef struct {
    UCHAR *p0;                         /* 0x00 */
    struct tagPSP_POS *pPos;           /* 0x04 [100] drop positions (func_80048FF4) */
    PRIM_1C *pPrim;                    /* 0x08 */
    SHORT Cx;                          /* 0x0C centre of the camera view (func_80048FF4) */
    SHORT Cz;                          /* 0x0E */
    USHORT R;                          /* 0x10 radius */
    USHORT Unk12;                      /* 0x12 */
    USHORT Num;                        /* 0x14 drops in use */
    SHORT Unk16;                       /* 0x16 */
    SHORT Unk18;                       /* 0x18 */
    SHORT Unk1A;                       /* 0x1A */
    SHORT Dx;                          /* 0x1C drop direction x */
    SHORT Dz;                          /* 0x1E drop direction z */
    UCHAR Unk20;                       /* 0x20 */
    UCHAR Unk21;                       /* 0x21 shake routine (func_80049218) */
    UCHAR Unk22;                       /* 0x22 frames of the current step */
    UCHAR Unk23;                       /* 0x23 frame counter */
    SHORT Unk24;                       /* 0x24 start G.Unk75E4 */
    SHORT Unk26;                       /* 0x26 start G.Unk75E6 */
    SHORT Unk28;                       /* 0x28 delta x */
    SHORT Unk2A;                       /* 0x2A delta y */
} W_DEB18;
extern W_DEB18 D_800DEB18;
void func_80010150(void *p, ULONG Size);
void func_80048FF4(void);

extern s8 D_80098728[];                /* random offsets (Sce_rnd_set) */

void func_80048974(SHORT Arg)
{
    UCHAR *p;
    PRIM_1C *pPr;
    LONG i;
    W_DEB18 *pW = &D_800DEB18;

    p = (UCHAR *)G.pEm_mem;
    D_800DEB18.p0 = p;
    p += 0x64;
    pW->pPos = (PSP_POS *)p;
    p += 0x4B0;
    pW->pPrim = (PRIM_1C *)p;
    p += 0x1900;
    func_80010150(D_800DEB18.p0, p - D_800DEB18.p0);
    i = 200;
    G.pEm_mem = (struct tagENEMY_WORK *)p;
    pPr = pW->pPrim;
    do {
        pPr->Tpage = 0xE1000420;
        pPr->Code = 0x52000000;
        pPr++;
    } while (--i != 0);
    pW->Unk20 = 0;
    pW->Unk21 = 0;
    pW->Unk16 = Arg;
    func_80048FF4();
    G.Status_flg |= 0x1000;
}

INCLUDE_ASM("asm/nonmatchings/game/psp", func_80048A3C);

INCLUDE_ASM("asm/nonmatchings/game/psp", func_80048E34);

void func_80048FF4(void)
{
    LONG *pP;
    LONG *pR;
    ULONG d;
    LONG R;
    LONG dx;
    LONG dz;
    LONG cx;
    LONG cz;
    LONG y;
    ULONG i;
    LONG r;
    UCHAR *pFlg;
    PSP_POS *pV;

    pP = G.pRoom->pRcut[G.Cut_no].View_p;
    pR = G.pRoom->pRcut[G.Cut_no].View_r;
    dx = pR[0] - pP[0];
    dz = pR[2] - pP[2];
    D_800DEB18.Cx = (pP[0] + pR[0]) >> 1;
    D_800DEB18.Cz = (pP[2] + pR[2]) >> 1;
    d = SquareRoot0(dx * dx + dz * dz);
    if (d >= 9000) {
        if (d < 19801) {
            R = d;
        } else {
            R = 19800;
        }
    } else {
        R = 9000;
    }
    do {
        i = 0;
    } while (0); /* needed for register allocation */
    pFlg = D_800DEB18.p0;
    cx = D_800DEB18.Cx;
    D_800DEB18.R = R;
    y = G.Pl_pos_y;
    cz = D_800DEB18.Cz;
    func_80010150(pFlg, 100);
    pV = D_800DEB18.pPos;
    if (D_800DEB18.Num != 0) {
        do {
            *pFlg = 1;
            r = RND();
            pV->x = ((r + D_80098728[D_800DEB18.Unk20++]) * D_800DEB18.R >> 8) + cx;
            pV->y = y - (UCHAR)Rnd_l() * 40;
            r = RND();
            pV->z = ((r + D_80098728[D_800DEB18.Unk20++]) * D_800DEB18.R >> 8) + cz;
            pFlg++;
            i++;
            pV++;
        } while (i != 100 && i < D_800DEB18.Num);
    }
}

void func_80049218(W_DEB18 *pW)
{
    tagGLOBAL *pG;

    switch (pW->Unk21) {
    case 0:
        if (--pW->Unk22 == 0) {
            pW->Unk21 = 1;
        }
        break;
    case 1:
        pG = &G;
        pW->Unk24 = pG->Unk75E4;
        pW->Unk26 = pG->Unk75E6;
        pW->Unk28 = (Rnd_l() << 4) - pG->Unk75E4;
        pW->Unk2A = Rnd_l() * 250 / 256 - pG->Unk75E6;
        pW->Unk22 = (Rnd_l() >> 4) + 10;
        pW->Unk23 = 0;
        pW->Unk21 = 2;
    case 2:
        pW->Unk23++;
        G.Unk75E4 = pW->Unk24 + pW->Unk28 * pW->Unk23 / pW->Unk22;
        G.Unk75E6 = pW->Unk26 + pW->Unk2A * pW->Unk23 / pW->Unk22;
        if (pW->Unk23 == pW->Unk22) {
            pW->Unk21 = 3;
        }
        break;
    case 3:
        G.Unk75E4 = pW->Unk24 + pW->Unk28;
        G.Unk75E6 = pW->Unk26 + pW->Unk2A;
        pW->Unk22 = Rnd_l();
        pW->Unk21 = 0;
        break;
    }
}
