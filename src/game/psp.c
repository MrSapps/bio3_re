#include "sub.h"
#include "psp.h"
#include "sld.h"
#include "sca.h"
#include "esp.h"

/*
 * PSP.C (RE2 PSP.C): the room sprites of the current cut (Psp_init, Psp_set, Psp_trans), plus RE3's
 * rain effect (Rain_init .. Rain_wind_move).
 */

#define RND() ((signed char)Rnd())
/* Rnd (SLD.S) with the int result this file used (no zero-extension of the canonical ULONG/byte) */
LONG Rnd_l(void) __asm__("Rnd");

extern UCHAR Rain_esp_no; /* Esp_call bank (id bits 24-27) of the rain splash */

typedef struct {
    USHORT Uv;
    UCHAR X;
    UCHAR Y;
    USHORT Otz;
    USHORT Wh;
    ULONG Size;
} PSP_REC;

/* room sprite work: OT z and group per sprite, two prims per sprite (RE2 Psp_init0 + Psp_init1) */
void Psp_init(void)
{
    tagGLOBAL *pG;
    UCHAR *p;
    ULONG n;
    PSP_PRIM *pPrim;
    PSP_PRIM *pP;

    pG = &G;
    n = pG->pRoom->nSprite_max;
    p = (UCHAR *)pG->pEm_mem;
    Psp_otz = (USHORT *)p;
    p += n * 2;
    Psp_id = p;
    pPsp_prim = pPrim = (PSP_PRIM *)(((ULONG)(p + n) + 3) & ~3);
    n *= 2;
    pG->pEm_mem = (struct tagENEMY_WORK *)(pPrim + n);
    pP = pPrim;
    while (n--) {
        pP->Tpage = 0xE1000095;
        pP->Code = 0x64808080;
        pP++;
    }
}

/* sets the 32 words of G+0x7914 to -1 (RE2 does this loop inline in Sce_work_clr_set on G.SaveData.Pri_be_flg) */
void Pri_be_flg_init(void)
{
    LONG *p = Pri_be_flg;
    LONG i = 32;

    do {
        *p++ = -1;
    } while (--i != 0);
}

/* unpack the sprite table of the current cut into the prims (RE2 Psp_set) */
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
    pOtz = Psp_otz;
    pId = Psp_id;
    pPrim = pPsp_prim;
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

/* add the sprites whose group flag is on to the OT (RE2 Psp_trans) */
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

    pOtz = Psp_otz;
    pG = &G;
    n = pG->pRoom->nSprite;
    pId = Psp_id;
    pPrim = &pPsp_prim[pG->Cbuf_id];
    pFlg = &pG->Pri_be_flg[pG->Cut_no];
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


/* rain: drop work and prims for Arg drops, first scatter; Status_flg 0x1000 */
void Rain_init(SHORT Arg)
{
    UCHAR *p;
    RAIN_PRIM *pPr;
    LONG i;
    RAIN_WORK *pW = &Rain_work;

    p = (UCHAR *)G.pEm_mem;
    Rain_work.p0 = p;
    p += 0x64;
    pW->pPos = (PSP_POS *)p;
    p += 0x4B0;
    pW->pPrim = (RAIN_PRIM *)p;
    p += 0x1900;
    Memclr(Rain_work.p0, p - Rain_work.p0);
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
    pW->Level = Arg;
    Rain_drop_set();
    G.Status_flg |= 0x1000;
}

/* moves the falling drops (splash Esp at the ground) and respawns up to Level free drops around the camera */
void Rain_move(void)
{
    RAIN_WORK *pW;
    tagGLOBAL *pG;
    UCHAR *pFlg;
    PSP_POS *pV;
    ULONG Cnt; /* declared before pTop: both are spilled, Cnt gets the lower stack slot */
    PSP_POS *pTop;
    ULONG i;
    LONG x;
    LONG y;
    LONG z;
    SVECTOR Sv;
    LONG X1; /* range-test temps; also hold the ground height and the random terms (shared registers) */
    LONG Z1;

    pW = &Rain_work;
    Rain_wind_move(pW);
    pG = &G;
    pW->Spd_y = pW->Level * 40 + 500;
    pW->Num = pW->Level * 7;
    pW->Spd_x = (ccos((USHORT)pG->Wind_dir) * (USHORT)pG->Wind_pow) >> 12;
    pW->Spd_z = (csin((USHORT)pG->Wind_dir) * (USHORT)pG->Wind_pow) >> 12;
    pW->Dx = pW->Spd_x * 900 / pW->Spd_y;
    pW->Dz = pW->Spd_z * 900 / pW->Spd_y;
    pTop = pW->pPos;
    pFlg = pW->p0;
    for (i = 0; i < 100; pFlg++, i++) {
        if (*pFlg & 1) {
            pV = &pTop[i];
            x = pV->x + pW->Spd_x;
            y = pV->y + pW->Spd_y;
            z = pV->z + pW->Spd_z;
            X1 = x + pW->Dx;
            Z1 = z + pW->Dz;
            X1 += 0x8000;
            Z1 += 0x8000;
            if ((X1 & 0xFFFF0000) || (Z1 & 0xFFFF0000)) {
                *pFlg = 0;
            } else {
                pV->x = x;
                pV->y = y;
                pV->z = z;
                X1 = Sca_get_ground(&pV->x, 10, 0x400, 0);
                if (X1 < y) {
                    *pFlg = 0;
                    Sv.vx = x;
                    Sv.vy = X1;
                    Sv.vz = z;
                    if (Rnd() & 1) {
                        Esp_call(((Rain_esp_no & 0xF) << 24) | ((Rnd_l() % 3) << 8) | 0xA, 0x10000000, &GsIDMATRIX, &Sv, NULL);
                    }
                }
            }
        }
    }
    Cnt = 0;
    x = pW->Cx;
    y = G.Player.Pos_y;
    z = pW->Cz;
    pFlg = pW->p0;
    for (i = 0; i < 100; pFlg++, i++) {
        if (!(*pFlg & 1)) {
            if (Cnt == pW->Level) {
                return;
            }
            pV = &pTop[i];
            pV->y = RND() * 10 + y - 9000;
            X1 = RND();
            Z1 = (signed char)Rnd_tbl[pW->Unk20++];
            X1 = ((X1 + Z1) * pW->R >> 8) + x;
            Z1 = X1 + pW->Dx;
            pV->x = X1;
            X1 += 0x8000;
            Z1 += 0x8000;
            if ((X1 & 0xFFFF0000) || (Z1 & 0xFFFF0000)) {
                continue;
            }
            X1 = RND();
            Z1 = (signed char)Rnd_tbl[pW->Unk20++];
            X1 = ((X1 + Z1) * pW->R >> 8) + z;
            Z1 = X1 + pW->Dz;
            pV->z = X1;
            X1 += 0x8000;
            Z1 += 0x8000;
            if ((X1 & 0xFFFF0000) || (Z1 & 0xFFFF0000)) {
                continue;
            }
            *pFlg = 1;
            Cnt++;
        }
    }
}

#define gte_ldv3(r0, r1, r2) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile("nop; nop; .word 0x4A280030")
#define gte_stsz(r0) __asm__ volatile("swc2 $19, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy1(r0) __asm__ volatile("swc2 $13, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy2(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")

/* rain: each live drop is a line from its position to position + (Dx, 900, Dz), shaded by depth */
void Rain_trans(void)
{
    SVECTOR Sv0;
    SVECTOR Sv1;
    SVECTOR Sv2;
    ULONG Otz;
    PSP_POS *pV;
    UCHAR *pFlg;
    RAIN_PRIM *pPr;
    ULONG *pOt;
    ULONG i;
    LONG D0;
    LONG y, z;
    ULONG *pT;

    pV = Rain_work.pPos;
    pFlg = Rain_work.p0;
    pPr = Rain_work.pPrim;
    Sv2.vx = 0;
    Sv2.vy = 0;
    Sv2.vz = 0;
    pOt = G.pOt1;
    pPr += G.Cbuf_id;
    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    i = 100;
    do {
        D0 = *pFlg;
        i--;
        pFlg++;
        if (D0 & 1) {
            D0 = pV->x;
            y = pV->y;
            z = pV->z;
            Sv0.vx = D0;
            Sv0.vy = y;
            Sv0.vz = z;
            D0 += Rain_work.Dx;
            y += 900;
            z += Rain_work.Dz;
            Sv1.vx = D0;
            Sv1.vy = y;
            Sv1.vz = z;
            gte_ldv3(&Sv2, &Sv1, &Sv0);
            gte_rtpt();
            gte_stsz(&Otz);
            if (Otz >= 0x20) {
                Otz = (Otz & 0x7FFF) >> 5;
                D0 = Otz >> 5;
                if (D0 >= 0x10) {
                    D0 = 0x10;
                }
                pPr->Rgb1 = 0x181818 - D0 * 0x10101;
                pT = &pOt[Otz];
                D0 = *pT;
                *pT = (ULONG)pPr & 0xFFFFFF;
                pPr->Tag = D0 | 0x06000000;
                gte_stsxy1(&pPr->Xy1);
                gte_stsxy2(&pPr->Xy0);
                pPr += 2; /* double buffered pairs */
            }
        }
        pV++;
    } while (i != 0);
}

/* scatter the drops around the centre of the camera view (radius 9000..19800) */
void Rain_drop_set(void)
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
    Rain_work.Cx = (pP[0] + pR[0]) >> 1;
    Rain_work.Cz = (pP[2] + pR[2]) >> 1;
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
    pFlg = Rain_work.p0;
    cx = Rain_work.Cx;
    Rain_work.R = R;
    y = G.Player.Pos_y;
    cz = Rain_work.Cz;
    Memclr(pFlg, 100);
    pV = Rain_work.pPos;
    if (Rain_work.Num != 0) {
        do {
            *pFlg = 1;
            r = RND();
            pV->x = ((r + (signed char)Rnd_tbl[Rain_work.Unk20++]) * Rain_work.R >> 8) + cx;
            pV->y = y - (UCHAR)Rnd_l() * 40;
            r = RND();
            pV->z = ((r + (signed char)Rnd_tbl[Rain_work.Unk20++]) * Rain_work.R >> 8) + cz;
            pFlg++;
            i++;
            pV++;
        } while (i != 100 && i < Rain_work.Num);
    }
}

/* wind: G.Wind_dir/Wind_pow drift to random targets over random times */
void Rain_wind_move(RAIN_WORK *pW)
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
        pW->Unk24 = pG->Wind_dir;
        pW->Unk26 = pG->Wind_pow;
        pW->Unk28 = (Rnd_l() << 4) - pG->Wind_dir;
        pW->Unk2A = Rnd_l() * 250 / 256 - pG->Wind_pow;
        pW->Unk22 = (Rnd_l() >> 4) + 10;
        pW->Unk23 = 0;
        pW->Unk21 = 2;
    case 2:
        pW->Unk23++;
        G.Wind_dir = pW->Unk24 + pW->Unk28 * pW->Unk23 / pW->Unk22;
        G.Wind_pow = pW->Unk26 + pW->Unk2A * pW->Unk23 / pW->Unk22;
        if (pW->Unk23 == pW->Unk22) {
            pW->Unk21 = 3;
        }
        break;
    case 3:
        G.Wind_dir = pW->Unk24 + pW->Unk28;
        G.Wind_pow = pW->Unk26 + pW->Unk2A;
        pW->Unk22 = Rnd_l();
        pW->Unk21 = 0;
        break;
    }
}
