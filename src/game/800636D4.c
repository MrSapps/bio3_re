#include "sub.h"
#include "pcstr.h"
#include "snd.h"
#include "em_sub.h"
#include "cd.h"

/* status work (RE3, 0x800E01C0) */
typedef struct tagST_WORK {
    UCHAR pad0[0x10];
    UCHAR Rtn0;                        /* 0x10 */
    UCHAR Rtn1;                        /* 0x11 */
    UCHAR Rtn2;                        /* 0x12 */
    UCHAR pad13[0x14 - 0x13];
    ULONG Flg14;                       /* 0x14 */
    ULONG Flg;                         /* 0x18 */
    UCHAR Unk1C;                       /* 0x1C */
    UCHAR Unk1D;                       /* 0x1D */
    UCHAR Unk1E;                       /* 0x1E */
    UCHAR pad1F[0x22 - 0x1F];
    SHORT Unk22;                       /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    UCHAR pad26[0x58 - 0x26];
    LONG Unk58;                        /* 0x58 */
    UCHAR pad5C[0xAA - 0x5C];
    UCHAR UnkAA;                       /* 0xAA */
    UCHAR UnkAB;                       /* 0xAB */
    UCHAR UnkAC;                       /* 0xAC */
    UCHAR padAD[0xBA - 0xAD];
    UCHAR UnkBA;                       /* 0xBA */
    UCHAR UnkBB;                       /* 0xBB */
    UCHAR padBC[0xC8 - 0xBC];
    SHORT UnkC8;                       /* 0xC8 */
    UCHAR UnkCA;                       /* 0xCA */
    UCHAR UnkCB;                       /* 0xCB */
    UCHAR padCC[0x118 - 0xCC];
    SHORT Unk118;                      /* 0x118 */
    SHORT Unk11A;                      /* 0x11A */
} ST_WORK;

/* overlays for G areas past the named fields */
typedef struct { UCHAR pad[0x265C]; ENEMY_WORK *pEm[42]; } G265C;  /* 0x265C: pUnk265C, pUnk2660, pEnemy[] as one array */
typedef struct tagUNK7C7C {
    UCHAR Tbl[0x4A][4];                /* 0x000 */
    UCHAR Unk128;                      /* 0x128 */
    UCHAR Unk129;                      /* 0x129 */
    UCHAR Num;                         /* 0x12A */
    UCHAR pad12B[0x140 - 0x12B];
} UNK7C7C;
typedef struct tagGLOBAL_X {
    UCHAR pad0[0x2489];
    UCHAR Unk2489;                     /* 0x2489 */
    UCHAR pad248A[0x2558 - 0x248A];
    SHORT Unk2558;                     /* 0x2558 player life? */
    UCHAR pad255A[0x255E - 0x255A];
    USHORT Unk255E;                    /* 0x255E */
    UCHAR pad2560[0x77F8 - 0x2560];
    LONG Unk77F8;                      /* 0x77F8 */
    UCHAR pad77FC[0x7806 - 0x77FC];
    USHORT Unk7806;                    /* 0x7806 */
    UCHAR Unk7808;                     /* 0x7808 */
    UCHAR pad7809[0x783E - 0x7809];
    UCHAR Unk783E;                     /* 0x783E */
    UCHAR pad783F[0x7840 - 0x783F];
    UCHAR Unk7840;                     /* 0x7840 */
    UCHAR pad7841[0x7848 - 0x7841];
    SHORT Unk7848;                     /* 0x7848 (D_800D1F80) */
    UCHAR pad784A[0x79FC - 0x784A];
    UNK7C7C Unk79FC[2];                /* 0x79FC, 0x140 bytes each */
    UNK7C7C *pUnk7C7C;                 /* 0x7C7C */
    LONG Unk7C80;                      /* 0x7C80 */
    SHORT Unk7C84[(0x7E98 - 0x7C84) / 2]; /* 0x7C84 */
    LONG Unk7E98[3];                   /* 0x7E98 */
    UCHAR Unk7EA4[3];                  /* 0x7EA4 */
    UCHAR pad7EA7;
    LONG Unk7EA8[3];                   /* 0x7EA8 */
    LONG Unk7EB4;                      /* 0x7EB4 */
} GLOBAL_X;
#define GX (*(GLOBAL_X *)&G)

#define EM_P(pEm, ofs, type) (*(type *)((UCHAR *)(pEm) + (ofs)))  /* ENEMY_WORK fields past the named ones */
#define FLD(p, ofs, type) (*(type *)((UCHAR *)(p) + (ofs)))  /* unnamed struct fields */
#define ST_P(p, ofs, type) FLD(p, ofs, type)

/* ---- merged helpers (forks + parent) ---- */
typedef struct { UCHAR b[0xA0]; } FA_A0[2][2];
typedef struct { UCHAR b[0x140]; } FA_B140R;
typedef DR_TPAGE FA_TP2[2][2];
typedef UCHAR FA_B140[2][0x140];
extern UCHAR D_800CC848;
void func_800642D4(ST_WORK *pSt, void *p, UCHAR *pXy, ULONG f);
void func_8006E8BC(ST_WORK *pSt, SPRT *p, UCHAR *pXy, ULONG f);
void func_80060E90(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern SHORT D_8009EDA4[];
extern SHORT D_8009EDCC[];
extern SHORT D_8009EDF4[];
extern SHORT D_8009EE1C[];
void func_80060BF0(ENEMY_WORK *pEm);
LONG func_800634F4(ENEMY_WORK *pEm, LONG f);
void func_8001B484(LONG no, ULONG dir, void *p, SVECTOR *pSv, LONG a4);
LONG func_80044804(void *p, LONG a1, LONG a2);
extern LONG D_8009DBD4;
void FA_Add_speedXZ3(ENEMY_WORK *pEm, LONG a, LONG b) __asm__("Add_speedXZ");
void func_80018EC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG f);
LONG func_80063198(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d);
void func_800606A0(ENEMY_WORK *pEm);
LONG func_800632B8(ENEMY_WORK *pEm, LONG f);
void func_80061390(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_800641D0(ST_WORK *pSt, UCHAR a, UCHAR b);
extern UCHAR D_8009F2EC[];
extern UCHAR D_8009F2F8[];
extern UCHAR D_8009F304[];
extern UCHAR D_8009F310[];
extern UCHAR D_8009F31C[];
void func_8006E600(ST_WORK *pSt, void *p, UCHAR *pTbl, ULONG f);
void FA_func_8006EBEC(ULONG *p) __asm__("func_8006EBEC");
void func_80063E9C(ST_WORK *pSt);
LONG func_80062B68(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern USHORT D_8009F26C[];
extern ULONG D_8009EF90[];
extern UCHAR D_8009EED8[];
extern char D_80010DCC[];
extern SHORT D_800CCBBC;
void func_800784E0(ULONG *p);
void func_8006ED28(ST_WORK *pSt);
void FB_OpenTIM(void) __asm__("OpenTIM");
typedef struct { UCHAR pad[0x2484]; UCHAR Unk2484; UCHAR Unk2485; } FB_G2484;
UCHAR FB_func_8006E598(void) __asm__("func_8006E598");
extern UCHAR D_800A04F8[];
extern UCHAR D_800A07C4[];
extern UCHAR D_800A0514[][4];
/* ---- forkC: sound driver ---- */
typedef struct tagFC_SNDW {
    UCHAR pad0[0x7C0];
    UCHAR Rv_mode;                     /* 0x7C0 requested reverb mode */
    UCHAR Rv_cur;                      /* 0x7C1 current reverb mode */
    UCHAR Rv_step;                     /* 0x7C2 */
    signed char Rv_cnt;                /* 0x7C3 */
    SHORT Vol[8];                      /* 0x7C4 (Vol[2] = SND_WORK.Unk7C8) */
    UCHAR pad7D4[0xAD8 - 0x7D4];
    LONG Cam_x;                        /* 0xAD8 listener position */
    LONG Cam_y;                        /* 0xADC */
    LONG Cam_z;                        /* 0xAE0 */
    UCHAR padAE4[0xAE8 - 0xAE4];
    SpuCommonAttr Cattr;               /* 0xAE8 */
    UCHAR padB10[0xB3C - (0xAE8 + sizeof(SpuCommonAttr))];
    LONG Dist[8];                      /* 0xB3C */
    UCHAR Vprio[24];                   /* 0xB5C */
    UCHAR UnkB74;                      /* 0xB74 */
    UCHAR padB75[0xB77 - 0xB75];
    UCHAR UnkB77;                      /* 0xB77 */
    UCHAR padB78;
    UCHAR UnkB79;                      /* 0xB79 */
} FC_SNDW;
#define FC_SW (*(FC_SNDW *)&Snd_work)
typedef struct { UCHAR pad[6]; UCHAR Unk6; UCHAR pad7; LONG *pSnd; UCHAR pad0C[0x34 - 0xC]; SHORT *pSnd_area; } FC_ROOM;
typedef struct { UCHAR pad[0x213C]; UCHAR *Unk213C; } FC_G;
extern SpuReverbAttr D_800A1108[];
extern UCHAR D_800A10F4[][0x14];

typedef struct { UCHAR pad[0x2484]; UCHAR Tp_x; UCHAR Clut_y; } FC_G2;
typedef struct {
    USHORT Cdir_y;                     /* 0x0 */
    SHORT Pos_x;                       /* 0x2 */
    SHORT Pos_y;                       /* 0x4 */
    SHORT Pos_z;                       /* 0x6 */
    USHORT Life;                       /* 0x8 */
    UCHAR UnkA;                        /* 0xA */
    UCHAR UnkB;                        /* 0xB */
    UCHAR Save[0x908];                 /* 0xC */
} FC_SAVE;
extern FC_SAVE D_800C7048;
typedef struct { UCHAR pad[0x24D6]; UCHAR Unk24D6; UCHAR pad24D7[0x2558 - 0x24D7]; USHORT Life; UCHAR pad255A[0x255E - 0x255A]; USHORT Unk255E; } FC_G3;
#define FC_G3X (*(FC_G3 *)&G)
void func_80078FE8(void);
void func_8007906C(void);
void func_800792FC(void);
void func_800790F0(ULONG *pAdr);

typedef struct { ULONG Mask; UCHAR pad4[0xC]; } FC_TR;
typedef struct { UCHAR pad0[0x5C]; FC_TR Tr[16]; } FC_SEQX;
typedef struct { UCHAR pad0[0xAC]; FC_SEQX Seq[3]; } FC_SNDW2;

extern LONG D_800E0608;
void func_80075C30(void);

typedef struct {
    ULONG Mask;                        /* 0x0 */
    SHORT Pitch;                       /* 0x4 */
    UCHAR pad6[2];
    UCHAR Pan;                         /* 0x8 */
    UCHAR pad9;
    UCHAR Vol;                         /* 0xA */
    UCHAR padB[3];
    UCHAR UnkE;                        /* 0xE */
    UCHAR UnkF;                        /* 0xF */
} FC_TRK;
typedef struct {
    ULONG *pHead;                      /* 0x00 */
    UCHAR *pPtr;                       /* 0x04 */
    UCHAR *pLoop;                      /* 0x08 */
    LONG Delta;                        /* 0x0C */
    ULONG Tempo;                       /* 0x10 */
    ULONG Tempo2;                      /* 0x14 */
    LONG Unk18;                        /* 0x18 */
    UCHAR pad1C[4];
    LONG Unk20;                        /* 0x20 */
    SHORT Unk24;                       /* 0x24 */
    SHORT No;                          /* 0x26 */
    UCHAR pad28[4];
    UCHAR Play;                        /* 0x2C */
    UCHAR Stop;                        /* 0x2D */
    UCHAR pad2E[0x38 - 0x2E];
    UCHAR Fade;                        /* 0x38 */
    UCHAR pad39[3];
    ULONG Fade_vol;                    /* 0x3C */
    ULONG Fade_step;                   /* 0x40 */
    UCHAR pad44[0x56 - 0x44];
    USHORT Reso;                       /* 0x56 */
    UCHAR pad58[0x5C - 0x58];
    FC_TRK Tr[16];                     /* 0x5C */
} FC_SEQ2;
typedef struct {
    UCHAR pad0[0x14];
    SHORT Unk14;
    UCHAR pad16[0x1D - 0x16];
    UCHAR Unk1D;
    UCHAR pad1E[2];
} FC_VOICE;
typedef struct {
    UCHAR pad0[0xAC];
    FC_SEQ2 Seq[3];                    /* 0x0AC */
    FC_VOICE Voice[24];                /* 0x4C0 */
    UCHAR pad7C0[0xB74 - 0x7C0];
    UCHAR UnkB74;                      /* 0xB74 */
    UCHAR padB75;
    UCHAR UnkB76;                      /* 0xB76 */
} FC_SNDW4;

typedef struct { UCHAR pad[0x24]; LONG *pCam; } FC_ROOM2;
#define FC_SQ(x) ((x) * (x))

typedef struct { UCHAR A0; UCHAR B0; UCHAR A1; UCHAR B1; } FC_MAP;
extern FC_MAP D_800A0430[];
LONG func_8004D720(VECTOR *pV, LONG a1, LONG a2, LONG a3);
extern MATRIX D_800E1200[];
extern MATRIX D_800E1220;
extern MATRIX D_800E1260;
extern MATRIX D_800E1280;
typedef struct tagMY_JNT {
    SVECTOR a;
    SVECTOR b;
    SHORT c[2];
} MY_JNT;
void func_80026728(MATRIX *a, MATRIX *b, MATRIX *c, LONG r);
void func_8002AEE8(MATRIX *a, MATRIX *b, MATRIX *c);
void func_80079374(MATRIX *a, MATRIX *b, LONG *t, MY_JNT *j);
void func_80079CC0(ENEMY_WORK *pEm, LONG h);
void MY_func_800611E0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_800611E0");
LONG func_80062BFC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d);
LONG MY_func_80077B84(LONG x, LONG z, LONG a) __asm__("func_80077B84");
void Pad_vib_fade_set(LONG a0, LONG a1, LONG a2, LONG a3);
void MY_func_8001B484(LONG id, ULONG code, MATRIX *pM, SVECTOR *pOfs, SVECTOR *p5) __asm__("func_8001B484");

/* ---- merged helpers (forks + parent) ---- */


/* local prototypes (this module) */
LONG Snd_seq_get_vlq(UCHAR **pp);
UCHAR func_80077220_u(LONG a0, LONG a1, LONG a2, LONG a3) __asm__("func_80077220");
extern ULONG D_800D1FC0[];
extern LONG D_800A1140[];
extern LONG D_800A1170[];
extern ULONG D_800CC858;
void func_800749A0(SND_REQ *pReq, USHORT a1, ULONG *p, LONG bank);
UCHAR func_80062CD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern ULONG D_800A0FEC[];
extern ULONG D_800A1000;
void Pl_wep_eff_parts_init(void *p);
void func_80046C90(void);
extern ULONG D_8009F678[];
void func_80010000(ULONG a0, ULONG *pDst);
LONG func_80026BE8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
LONG func_80079690(ENEMY_WORK *pEm, UCHAR *pKan, UCHAR *pSeq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, LONG rate);
extern void (*D_8009EEA4[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern UCHAR D_8009F568[];
extern char D_80010F84[];
void func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
extern USHORT D_8009F4F8[][2];
extern LONG D_800D23B8;
extern ST_WORK D_800E01C0;
extern UCHAR *D_800BA6E8;
void func_80073CCC(UCHAR *a, UCHAR *b);
extern SpuCommonAttr D_800E10F8;
LONG func_80027940(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
extern UCHAR D_800DBB58;
extern SHORT D_800D1F80;
extern ULONG *D_800E01C8;
extern USHORT D_800A004C[][2];
void func_800746C0(ULONG code, VECTOR *pPos, SHORT a2, SHORT a3);
void func_80068024(ST_WORK *pSt);
void func_8006CD68(UCHAR no);
LONG func_800759D8(LONG x);
void func_8007836C(LONG no, ULONG *pAdr);
extern UCHAR D_800A0FE4[];
extern UCHAR D_800A0FDC[];
extern UCHAR D_800CCBBF;
extern SHORT D_800E067C;
extern UCHAR D_800A0BB4[];
extern UCHAR D_800A0BC4[];
void func_800623C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern void (*D_8009F4E4[])(ST_WORK *pSt);
extern void (*D_800A0100[])(ST_WORK *pSt);
extern void (*D_800A0138[])(ST_WORK *pSt);
extern void (*D_800A0500[])(ST_WORK *pSt);
extern ULONG D_800D20DC[];
UCHAR func_80069614(ST_WORK *pSt, LONG a1);
void func_80027338(ULONG *pKan, ULONG *pSeq, ULONG a2, ULONG Flg);
void func_80049BA8(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);
void func_80019318(ENEMY_WORK *pEm, LONG a1, LONG a2);
void func_80060464(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_8006037C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
extern void (*D_8009EE44[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_800776B0(LONG kind, ULONG parts, ENEMY_WORK *pEm);
void func_80062D3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8006055C(ENEMY_WORK *pEm);
void func_8002B2BC(ULONG *pKan, ULONG *pSeq, USHORT *pNow_seq, ULONG R_flg);

/* declarations from earlier parts of the original address-split file */
signed char func_800102E8(void);
LONG FF_func_8001B484(LONG no, ULONG dir, void *p, SVECTOR *pSv, LONG a4) __asm__("func_8001B484");

void func_800636D4(ST_WORK *pSt)
{
    ULONG i, sum, n;
    ULONG t;
    ULONG *p;

    i = 0;
    sum = 0;
    t = D_8009F26C[FLD(pSt, 0xBC, UCHAR)];
    FLD(pSt, 0xBD, UCHAR) = 0;
    CD.Fno = 0x1C;
    n = t - 1;
    CD.Fsize = D_8009EF90[n];
    CD.Fsector = (floc[0x1C].sec_high << 16) + floc[0x1C].sector;
    for (; i < n; i++) {
        sum += D_8009EF90[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = D_8009EED8[t - 1];
    func_80012818(0x1C, FLD(pSt, 8, ULONG *), 2, D_80010DCC);
    p = FLD(pSt, 8, ULONG *);
    D_800CCBBC = 0xA17;
    FA_func_8006EBEC(p);
    func_800641D0(pSt, FLD(pSt, 0xBC, UCHAR), FLD(pSt, 0xBD, UCHAR));
    pSt->Rtn1 = 0;
    FLD(pSt, 0xBE, UCHAR) = 0;
    FLD(pSt, 0xBF, UCHAR) = 0;
    pSt->Rtn0++;
    pSt->Flg &= ~0x40000000;
    pSt->Flg &= ~0x80000000;
    pSt->Flg |= 0x100000;
    pSt->Flg |= 0x80000;
    pSt->Flg &= ~0x40000;
    pSt->Flg &= ~0x20000;
    func_80063E9C(pSt);
}

INCLUDE_RODATA("asm/nonmatchings/game/800636D4", D_80010DCC);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80063850);

extern UCHAR D_800DBB5B;
extern UCHAR D_800CEA80[];
LONG func_8001B894();
void func_8002FD30(LONG a0, LONG a1, LONG a2, LONG a3);

void func_80063CAC(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    UCHAR *pA = pG->pAot_data;
    UCHAR *pD = (UCHAR *)pG->Unk21DC;
    UCHAR n;
    LONG ofs;
    ULONG f;
    ULONG *p;

    switch (pSt->Rtn1) {
    case 0:
        switch (FLD(pSt, 4, UCHAR)) {
        case 0:
            pSt->Rtn0 = 2;
            pSt->Rtn1 = 5;
            pSt->Rtn2 = 5;
            FLD(pSt, 0x13, UCHAR) = 0;
            f = pSt->Flg | 0x40000000;
            pSt->Flg = (f | 0x80000000) & ~0x100000;
            func_80012818(0x1D, FLD(pSt, 8, ULONG *), 0, "FILEI.TIM");
            p = FLD(pSt, 8, ULONG *);
            FLD(pG, 0x2484, SHORT) = 0x917;
            func_800784E0(p);
            break;
        case 3:
            pSt->Flg &= ~0x100000;
            *pA = 0;
            n = pD[6];
            if (pG->Unk783E < 2) {
                Flg_on((ULONG *)((UCHAR *)pG + 0x78D0), *(USHORT *)(pD + 4));
            } else {
                Flg_on((ULONG *)((UCHAR *)pG + 0x78F0), *(USHORT *)(pD + 4));
            }
            ofs = (n & 0xFF) * 0x194;
            func_8001B894(0x705, D_800CEA80 + ofs);
            func_8001B894(0x905, 0);
            if (!(n & 0x80)) {
                *(LONG *)(D_800CEA80 + ofs - 0x20) = 0;
            }
            D_800DBB5B = pD[0];
            func_8002FD30(0, 0x1000, 6, 0);
            pSt->Rtn1++;
            break;
        }
        break;
    case 1:
        if (!(D_800DBB58 & 0x80)) {
            pSt->Rtn0 = 0xD;
            pSt->Rtn1 = 0;
            func_800746C0(5, NULL, 0, 0);
        }
        break;
    }
}

void func_80063E9C(ST_WORK *pSt)
{
    ULONG f;
    UCHAR c;

    SetDrawTPage((DR_TPAGE *)((UCHAR *)pSt + 0x3E4), 0, 0, 0x97);
    SetDrawTPage((DR_TPAGE *)((UCHAR *)pSt + 0x3F4), 0, 0, 0x97);
    SetDrawTPage((DR_TPAGE *)((UCHAR *)pSt + 0x3EC), 0, 0, 0x18);
    SetDrawTPage((DR_TPAGE *)((UCHAR *)pSt + 0x3FC), 0, 0, 0x18);
    func_8006E600(pSt, (void *)0x801ADA90, D_8009F2EC, (FLD(pSt, 0xD7, UCHAR) << 8) | 1);
    c = FLD(pSt, 0xD7, UCHAR) + 1;
    func_8006E600(pSt, (void *)0x801ADAB8, D_8009F2F8, (c << 8) | 1);
    c = FLD(pSt, 0xD3, UCHAR) + 2;
    f = (c << 8) | 0x10001;
    func_8006E600(pSt, (void *)0x801ADAE0, D_8009F310, f);
    func_8006E600(pSt, (void *)0x801ADB08, D_8009F304, f);
    func_8006E600(pSt, (void *)0x801ADB80, D_8009F31C, (FLD(pSt, 0xD3, UCHAR) << 8) | 0x10001);
}

void func_80063FE4(ST_WORK *pSt)
{
    UCHAR *p;
    UCHAR c;

    AddPrim(G.pOt0 + 4, (UCHAR *)pSt + (G.Cbuf_id * 16 + 0x3E4));
    AddPrim(G.pOt0 + 3, &(*(FA_TP2 *)((UCHAR *)pSt + 0x3E4))[G.Cbuf_id][1]);
    AddPrim(G.pOt0 + 2, &(*(FA_A0 *)((UCHAR *)pSt + 0x144))[G.Cbuf_id][1]);
    func_800642D4(pSt, (void *)0x801ADA90, D_8009F2EC, 0x1101);
    if (FLD(pSt, 0xBC, UCHAR) == 0x1B) {
        p = (UCHAR *)0x801ADA90;
        if (G.Cbuf_id) {
            p = (UCHAR *)0x801ADAA4;
        }
        FLD(p, 0xA, SHORT) = FLD(pSt, 0x12A, USHORT) + 0x24;
    }
    func_800642D4(pSt, (void *)0x801ADAB8, D_8009F2F8, 0x11201);
    if (pSt->Flg & 0x80000) {
        func_8006E8BC(pSt, (SPRT *)0x801ADAE0, D_8009F310, 0x101301);
    }
    if (pSt->Flg & 0x40000) {
        func_8006E8BC(pSt, (SPRT *)0x801ADB08, D_8009F304, 0x101401);
    }
    if (pSt->Flg & 0x20000) {
        p = (UCHAR *)0x801ADB80;
        if (D_800CC848) {
            p = (UCHAR *)0x801ADB94;
        }
        if (FLD(pSt, 0xBF, UCHAR) == 0) {
            c = FLD(pSt, 0xD3, UCHAR) + 7;
        } else {
            c = FLD(pSt, 0xD3, UCHAR);
        }
        FLD(p, 0xE, SHORT) = ((c + 0x1E0) << 6) | 0x13;
        func_8006E8BC(pSt, (SPRT *)0x801ADB80, D_8009F31C, 0x101501);
    }
}

void func_800641D0(ST_WORK *pSt, UCHAR a, UCHAR b)
{
    ULONG i, sum, n;
    ULONG *p;

    i = 0;
    sum = 0;
    n = D_8009F26C[a] + b;
    CD.Fno = 0x1C;
    CD.Fsize = D_8009EF90[n];
    CD.Fsector = (floc[0x1C].sec_high << 16) + floc[0x1C].sector;
    for (; i < n; i++) {
        sum += D_8009EF90[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = D_8009EED8[n];
    func_80012818(0x1C, FLD(pSt, 8, ULONG *), 2, D_80010DCC);
    p = FLD(pSt, 8, ULONG *);
    D_800CCBBC = 0xB18;
    func_800784E0(p);
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800642D4);

void func_800643E4(ST_WORK *pSt)
{
    ULONG a[2];
    pSt->Rtn0++;
    pSt->Rtn1 = 0;
    pSt->Unk22 = 0;
    pSt->Unk24 = 0;
    pSt->UnkCB = 0;
    pSt->Flg |= 0x4000000;
    pSt->Flg |= 0x20000000;
}

void func_80064424(ST_WORK *pSt)
{
    D_8009F4E4[pSt->Rtn1](pSt);
}

void func_80064460(ST_WORK *pSt)
{
    pSt->Rtn0 = 0xD;
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006446C);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800646F0);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80064E80);

void func_80065054(ST_WORK *pSt)
{
    if (pSt->UnkCA == 0) {
        if (pSt->UnkC8 >= -4) {
            pSt->Rtn1 = 1;
        }
        pSt->UnkC8 += 4;
    } else {
        if (pSt->UnkC8 < 5) {
            pSt->Rtn1 = 1;
        }
        pSt->UnkC8 -= 4;
    }
}

void func_800650BC(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        pSt->Rtn1 = 0;
        pSt->Unk22 = 0;
        pSt->Unk24 = 0;
        func_800746C0(5, 0, 0, 0);
        pSt->Flg |= 0x20000000;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006511C);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800654A8);

void func_80065BA4(UCHAR a, UCHAR b)
{
    RECT r;
    ST_WORK *pSt = &D_800E01C0;
    USHORT (*tbl)[2] = D_800A004C;
    USHORT *pa = tbl[a];
    USHORT *pb;
    USHORT (*tbl2)[2];

    r.x = pa[0];
    r.y = pa[1];
    r.w = 0x14;
    r.h = 0x1E;
    StoreImage(&r, FLD(pSt, 8, ULONG *));
    DrawSync(0);
    tbl2 = D_8009F4F8;
    pb = tbl2[b];
    r.x = pb[0];
    r.y = pb[1];
    MoveImage(&r, pa[0], pa[1]);
    DrawSync(0);
    r.x = pb[0];
    r.y = pb[1];
    LoadImage(&r, FLD(pSt, 8, ULONG *));
    DrawSync(0);
}

void func_80065C8C(UCHAR dst, UCHAR src)
{
    RECT r;

    r.x = D_800A004C[src][0];
    r.y = D_800A004C[src][1];
    r.w = 0x14;
    r.h = 0x1E;
    MoveImage(&r, D_8009F4F8[dst][0], D_8009F4F8[dst][1]);
    DrawSync(0);
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80065D04);

void func_80065EE0(ST_WORK *pSt)
{
    ULONG i, j;
    TILE *pT;
    LINE_F2 *pL;
    SPRT *pS;

    for (j = 0, pT = (TILE *)0x8019E1B8; j < 2; j++, pT++) {
        setTile(pT);
        setRGB0(pT, 0x20, 0x20, 0x40);
        setSemiTrans(pT, 1);
        setXY0(pT, 0x10, 0x30);
        setWH(pT, 0xB8, 0x30);
    }
    for (j = 0, pT = (TILE *)0x8019E1D8; j < 2; j++, pT++) {
        setTile(pT);
        setRGB0(pT, 0x20, 0x20, 0x40);
        setSemiTrans(pT, 1);
        setXY0(pT, 0x10, 0x78);
        setWH(pT, 0xB8, 0x30);
    }
    for (j = 0, pL = (LINE_F2 *)0x8019E1F8; j < 2; pL++, j++) {
        setlen(pL, 3);
        setcode(pL, 0x42);
        setXY2(pL, 0x10, 0x5F, 0xC8, 0x5F);
    }
    for (j = 0, pL = (LINE_F2 *)0x8019E218; j < 2; pL++, j++) {
        setlen(pL, 3);
        setcode(pL, 0x42);
        setXY2(pL, 0x10, 0x78, 0xC8, 0x78);
    }
    for (j = 0, pL = (LINE_F2 *)0x8019E238; j < 2; pL++, j++) {
        setlen(pL, 3);
        setcode(pL, 0x42);
        setRGB0(pL, 0x80, 0x20, 0x20);
    }
    pS = (SPRT *)0x8019E168;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; pS++, j++) {
            setSprt(pS);
            setRGB0(pS, 0x80, 0x80, 0x80);
            pS->clut = (FLD(pSt, 0xD5, UCHAR) + 0x1E0) << 6;
            setSemiTrans(pS, 1);
            setXY0(pS, 0xC8, 1);
            setUV0(pS, 0x20, 0x88);
            setWH(pS, 0x10, 5);
        }
    }
    pS = (SPRT *)0x8019D768;
    for (i = 0; i < 0x40; i++) {
        for (j = 0; j < 2; pS++, j++) {
            setSprt(pS);
            setRGB0(pS, 0x80, 0x80, 0x80);
            setXY0(pS, 0xCC, i + 0x4C);
            setWH(pS, 8, 1);
            pS->clut = (FLD(pSt, 0xD5, UCHAR) + 0x1E0) << 6;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800661B4);

void func_80066530(ST_WORK *pSt)
{
switch (ST_P(pSt, 4, UCHAR)) { case 1: pSt->Rtn1 = 0xB; pSt->Rtn2 = 0; pSt->Rtn0++; break; case 0: ST_P(pSt, 0xF0, SHORT) = 0x48; ST_P(pSt, 0xF4, SHORT) = 0x48; ST_P(pSt, 0xF8, SHORT) = 0x48; ST_P(pSt, 0xFC, SHORT) = 0x48; ST_P(pSt, 0x100, SHORT) = 0x48; ST_P(pSt, 0x104, SHORT) = -0xD8; ST_P(pSt, 0x108, SHORT) = -0xD8; ST_P(pSt, 0x11C, SHORT) = -0xD8; ST_P(pSt, 0x144, SHORT) = -0xD8; ST_P(pSt, 0x10E, SHORT) = 0x30; pSt->Rtn1 = 0; pSt->Unk22 = 0; pSt->Unk24 = 0; pSt->Flg |= 0x20000000; pSt->Flg |= 0x40000000; pSt->Rtn0++; break; }
}

void func_800665C8(ST_WORK *pSt)
{
    D_800A0100[pSt->Rtn1](pSt);
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80066604);

void func_800668E4(ST_WORK *pSt)
{
    if (func_80069614(pSt, 0)) {
        pSt->Rtn1 = 2;
        pSt->Unk1E = 0;
    }
}

void func_80066920(ST_WORK *pSt)
{
    UCHAR *e = GX.pUnk7C7C->Tbl[(signed char)pSt->Unk1C];
    LONG n = 3;

    if (((USHORT *)e)[1] & 0x10) {
        n = 4;
    }
    if (G.Key_trg & 0x2000) {
        pSt->Rtn1 = 3;
        func_800746C0(5, 0, 0, 0);
    } else if (G.Key_trg & 0x1000) {
        if ((signed char)pSt->Unk1E == 0 && (UCHAR)(e[0] + 0x7D) < 2) {
            pSt->Rtn1 = 8;
        } else {
            pSt->Rtn1 = pSt->Unk1E + 6;
        }
        FLD(pSt, 0x12, UCHAR) = 0;
        FLD(pSt, 0x13, UCHAR) = 0;
        func_800746C0(6, 0, 0, 0);
    } else {
        if (FLD(&G, 0x2100, USHORT) & 0x1000) {
            pSt->Unk1E--;
        }
        if (FLD(&G, 0x2100, USHORT) & 0x4000) {
            pSt->Unk1E++;
        }
        if ((signed char)pSt->Unk1E < 0) {
            pSt->Unk1E = n - 1;
        }
        if ((signed char)pSt->Unk1E >= n) {
            pSt->Unk1E = 0;
        }
        if (FLD(&G, 0x2100, ULONG) & 0x5000) {
            func_800746C0(4, 0, 0, 0);
        }
    }
}

void func_80066A90(ST_WORK *pSt)
{
    if (func_80069614(pSt, 1)) {
        pSt->Rtn1 = 0;
        pSt->Unk22 = 0;
        pSt->Unk24 = 0;
        pSt->Flg |= 0x20000000;
    }
}

void func_80066ADC(ST_WORK *pSt)
{
    switch (pSt->Rtn2) {
    case 0:
        pSt->Flg &= ~0x20000000;
        if (FLD(pSt, 0x13, UCHAR) < 0xC) {
            FLD(pSt, 0x112, SHORT) += 6;
            FLD(pSt, 0xE4, SHORT) -= 0xE;
            FLD(pSt, 0xE8, SHORT) += 0xE;
            FLD(pSt, 0xEC, SHORT) += 0xE;
            FLD(pSt, 0xF0, SHORT) += 0xE;
            FLD(pSt, 0xF4, SHORT) += 0xE;
            FLD(pSt, 0xF8, SHORT) += 0xE;
            FLD(pSt, 0xFC, SHORT) += 0xE;
            FLD(pSt, 0x100, SHORT) += 0xE;
            FLD(pSt, 0x114, SHORT) += 0xE;
            FLD(pSt, 0x13, UCHAR)++;
        } else {
            pSt->Rtn0 = 4;
            pSt->Flg &= ~0x40000000;
            func_8006ED28(pSt);
        }
        break;
    case 1:
        if (FLD(pSt, 0x13, UCHAR) < 0xC) {
            FLD(pSt, 0x112, SHORT) -= 6;
            FLD(pSt, 0xE4, SHORT) += 0xE;
            FLD(pSt, 0xE8, SHORT) -= 0xE;
            FLD(pSt, 0xEC, SHORT) -= 0xE;
            FLD(pSt, 0xF0, SHORT) -= 0xE;
            FLD(pSt, 0xF4, SHORT) -= 0xE;
            FLD(pSt, 0xF8, SHORT) -= 0xE;
            FLD(pSt, 0xFC, SHORT) -= 0xE;
            FLD(pSt, 0x100, SHORT) -= 0xE;
            FLD(pSt, 0x114, SHORT) -= 0xE;
            FLD(pSt, 0x13, UCHAR)++;
        } else {
            pSt->Rtn1 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Flg |= 0x20000000;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80066CA0);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800676B8);

void func_80067B70(ST_WORK *pSt)
{
    D_800A0138[pSt->Rtn2](pSt);
}

void func_80067BAC(ST_WORK *pSt)
{
    UCHAR v = pSt->Unk1C;

    pSt->Unk22 = 0;
    pSt->Unk24 = 0;
    pSt->Unk1D = v;
    pSt->Unk118 = (v & 1) * 40;
    pSt->Flg |= 0x2000000;
    pSt->Flg |= 0x20000000;
    pSt->Rtn2++;
    pSt->Unk11A = ((signed char)pSt->Unk1D >> 1) * 30;
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80067C14);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80068024);

void func_80068A54(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        pSt->Rtn1 = 3;
        func_800746C0(5, 0, 0, 0);
        pSt->Flg &= ~0x2000000;
    }
}

void func_80068AB4(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        switch (GX.Unk7848) {
        case 0:
            Pl_wep_eff_parts_init(&GX.Unk2489 + 3);
            func_80046C90();
            func_80068024(pSt);
            break;
        case 1:
            pSt->Rtn1 = 3;
            pSt->Flg &= ~0x2000000;
            func_800746C0(5, 0, 0, 0);
            break;
        }
    }
}

void func_80068B5C(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        switch (D_800D1F80) {
        case 0:
            pSt->Unk58 += 8;
        case 1:
            func_80068024(pSt);
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80068BC0);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80069280);

void func_8006954C(ST_WORK *pSt)
{
    GLOBAL_X *pG = &GX;
    UCHAR *p = pG->pUnk7C7C->Tbl[(signed char)pSt->Unk1C];
    USHORT w = ((USHORT *)p)[1];
    UCHAR id = p[0];
    USHORT n;

    if (w & 0x100) {
        ((USHORT *)p)[1] = w & ~0x100;
        p[0] = 0xF;
    } else {
        ((USHORT *)p)[1] = w | 0x100;
        p[0] = 0xE;
    }
    n = D_800A0514[p[0]][3] | (((USHORT *)p)[1] & 0xFF00);
    ((USHORT *)p)[1] = n;
    if ((w & 3) == 3) {
        ((USHORT *)p)[1] = n | 3;
    }
    if (id == pG->pUnk7C7C->Unk129 && (signed char)pSt->Unk1C == pG->pUnk7C7C->Unk128) {
        pG->pUnk7C7C->Unk129 = p[0];
    }
    pSt->Rtn1 = 2;
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80069614);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80069A6C);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80069C3C);

extern ULONG D_800CC840;
void func_8006AC88(UCHAR no);

void func_8006A234(ST_WORK *pSt)
{
    GLOBAL_X *pG = &GX;
    UCHAR *p;
    UCHAR *pI;
    UCHAR n;
    LONG dx, dy;
    switch (pSt->Rtn2) {
    case 0:
        func_8006AC88(FLD(pSt, 0x76, UCHAR));
        func_8006A9AC(12, FLD(pSt, 0x76, UCHAR));
        FLD(pSt, 0x13, UCHAR) = 0;
        pSt->Rtn2++;
        func_800746C0(6, NULL, 0, 0);
        break;
    case 1:
        if (FLD(pSt, 0x13, UCHAR) < 12) {
            FLD(pSt, 0x104, SHORT) += 0x12;
        } else {
            pSt->Rtn2++;
            pSt->Flg |= 0x20000000;
            p = pG->pUnk7C7C->Tbl[FLD(pSt, 0x77, signed char)];
            p[0] = FLD(pSt, 0x76, UCHAR);
        }
        FLD(pSt, 0x13, UCHAR)++;
        break;
    case 2:
        if (D_800CC840 & 0x3000) {
            func_800746C0(6, NULL, 0, 0);
            n = FLD(pSt, 0x77, UCHAR);
            FLD(pSt, 0x6F, UCHAR) = 0x38;
            FLD(pSt, 0x70, UCHAR) = 0x24;
            FLD(pSt, 0x120, SHORT) = 0x70;
            FLD(pSt, 0x13, UCHAR) = 0;
            FLD(pSt, 0x122, SHORT) = 0x7C;
            dx = (n & 1) * 20 + 0xF4;
            dy = ((signed char)n / 2) * 30 + 0x51;
            FLD(pSt, 0x71, UCHAR) = (dx - FLD(pSt, 0x120, SHORT)) / 10;
            pSt->Rtn2++;
            pSt->Flg = (pSt->Flg | 0x400000) & ~0x20000000;
            FLD(pSt, 0x72, UCHAR) = (dy - FLD(pSt, 0x122, SHORT)) / 10;
        }
        break;
    case 3:
        if (FLD(pSt, 0x13, UCHAR) < 10) {
            FLD(pSt, 0x120, SHORT) += FLD(pSt, 0x71, signed char);
            FLD(pSt, 0x122, SHORT) += FLD(pSt, 0x72, signed char);
            FLD(pSt, 0x6F, UCHAR) -= 3;
            FLD(pSt, 0x70, UCHAR) -= 2;
        FLD(pSt, 0x13, UCHAR)++;
            return;
        }
        p = pG->pUnk7C7C->Tbl[FLD(pSt, 0x77, signed char)];
        pI = D_800A0514[FLD(pSt, 0x76, UCHAR)];
        func_8006AE20(FLD(pSt, 0x77, UCHAR), 12);
        p[0] = FLD(pSt, 0x76, UCHAR);
        p[1] = pI[1];
        ((USHORT *)p)[1] = pI[3];
        pSt->Rtn2++;
        pSt->Flg &= ~0x400000;
        break;
    case 4:
        if (D_800CC840 & 0x3000) {
            func_800746C0(5, NULL, 0, 0);
            FLD(pSt, 0x13, UCHAR) = 0;
            pSt->Rtn2++;
        }
        break;
    case 5:
        if (FLD(pSt, 0x13, UCHAR) < 12) {
            FLD(pSt, 0x104, SHORT) -= 0x12;
        } else {
            pSt->Rtn1 = 0;
            pSt->Rtn2 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Flg |= 0x20000000;
        }
        FLD(pSt, 0x13, UCHAR)++;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006A59C);

void func_8006A888(ST_WORK *pSt)
{
    pSt->Rtn0 = 0xD;
    pSt->Rtn1 = 0;
}

UCHAR *func_8006A898(LONG a0, UCHAR a, UCHAR b)
{
    UCHAR *p = D_800A07C4;
    UCHAR c = p[1];

    if (c != 0xFF) {
        do {
            if (c == a && p[2] == b) {
                return p;
            }
            if (p[1] == b && p[2] == a) {
                return p;
            }
            p += 8;
            c = p[1];
        } while (c != 0xFF);
    }
    return (UCHAR *)-1;
}

UCHAR *func_8006A918(LONG a0, UCHAR id)
{
    UCHAR *p = D_800A0BB4;

    while (p[1] != 0xFF) {
        if (p[1] == id) {
            return p;
        }
        p += 4;
    }
    return (UCHAR *)-1;
}

UCHAR *func_8006A95C(LONG a0, UCHAR id)
{
    UCHAR *p = D_800A0BC4;

    while (p[0] != 0xFF) {
        if (p[0] == id) {
            return p;
        }
        p += 4;
    }
    return (UCHAR *)-1;
}

void func_8006A9AC(UCHAR no, UCHAR idx)
{
    RECT r;
    ST_WORK *pSt = &D_800E01C0;

    CD.Fno = 0x34;
    CD.Fsize = 0x800;
    CD.Fsector = (floc[0x34].sec_high << 16) + floc[0x34].sector + idx;
    CD.Fsum = D_8009F568[idx];
    func_80012818(0x34, FLD(pSt, 8, ULONG *), 2, D_80010F84);
    r.x = D_800A004C[no][0];
    r.y = D_800A004C[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, FLD(pSt, 8, ULONG *));
}

void func_8006AA84(UCHAR no)
{
    LONG pad[2];

    CD.Fno = 0x34;
    CD.Fsize = 0x800;
    CD.Fsector = (floc[0x34].sec_high << 16) + floc[0x34].sector + no;
    CD.Fsum = D_8009F568[no];
    func_80012818(0x34, D_800E01C8, 2, D_80010F84);
}

void func_8006AB08(UCHAR no)
{
    RECT r;

    r.x = D_800A004C[no][0];
    r.y = D_800A004C[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, D_800E01C8);
}

void func_8006AB68(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf)
{
    RECT r;

    func_80010000(ofs + D_8009F678[tbl], pBuf);
    r.x = D_800A004C[no][0];
    r.y = D_800A004C[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pBuf);
}

void func_8006ABF8(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf)
{
    RECT r;

    func_80010000(ofs + D_8009F678[tbl], pBuf);
    r.x = D_8009F4F8[no % 8][0];
    r.y = D_8009F4F8[no % 8][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pBuf);
}

INCLUDE_RODATA("asm/nonmatchings/game/800636D4", D_80010F84);

extern UCHAR D_8009F5F0[];

void func_8006AC88(UCHAR no)
{
    ST_WORK *pSt = &D_800E01C0;
    LONG pad[2];

    CD.Fno = 0x33;
    CD.Fsize = 0x2800;
    CD.Fsector = (floc[0x33].sec_high << 16) + floc[0x33].sector + no * 5;
    CD.Fsum = D_8009F5F0[no];
    func_80012818(0x33, FLD(pSt, 8, ULONG *), 2, "ITEMG.PIX");
    D_800CCBBC = 0x817;
    func_800784E0(FLD(pSt, 8, ULONG *));
    FLD(pSt, 0xD4, UCHAR) = 8;
}

void func_8006AD38(UCHAR a, UCHAR b)
{
    RECT r;
    ST_WORK *pSt = &D_800E01C0;
    USHORT (*tbl)[2] = D_800A004C;
    USHORT *pa = tbl[a];
    USHORT *pb;

    r.x = pa[0];
    r.y = pa[1];
    r.w = 0x14;
    r.h = 0x1E;
    StoreImage(&r, FLD(pSt, 8, ULONG *));
    DrawSync(0);
    pb = tbl[b];
    r.x = pb[0];
    r.y = pb[1];
    MoveImage(&r, pa[0], pa[1]);
    DrawSync(0);
    r.x = pb[0];
    r.y = pb[1];
    LoadImage(&r, FLD(pSt, 8, ULONG *));
    DrawSync(0);
}

void func_8006AE20(UCHAR dst, UCHAR src)
{
    RECT r;

    r.x = D_800A004C[src][0];
    r.y = D_800A004C[src][1];
    r.w = 0x14;
    r.h = 0x1E;
    MoveImage(&r, D_800A004C[dst][0], D_800A004C[dst][1]);
    DrawSync(0);
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006AE90);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006B66C);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006C370);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006C484);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006C6D0);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006CA10);

signed char func_8006CC8C(UCHAR id, signed char i)
{
    for (; (UCHAR)i < GX.pUnk7C7C->Num; i++) {
        if (GX.pUnk7C7C->Tbl[(UCHAR)i][0] == id) {
            return i;
        }
    }
    return -1;
}

signed char func_8006CCF0(UCHAR no, UCHAR n, UCHAR f)
{
    UCHAR *p = GX.pUnk7C7C->Tbl[no];

    if ((((USHORT *)p)[1] & 3) != 3) {
        p[1] -= n;
    }
    if (p[1] == 0 && f == 0) {
        func_8006CD68(no);
    }
    return (signed char)p[1];
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006CD68);

void func_8006CEA8(void)
{
    UCHAR no;

    if ((no = GX.pUnk7C7C->Unk128) != 0xFF) {
        func_8006CD68(no);
        GX.pUnk7C7C->Unk129 = 0;
        GX.pUnk7C7C->Unk128 = 0xFF;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006CF0C);

LONG func_8006D030(UCHAR f)
{
    UCHAR *p;
    USHORT w;
    LONG t;

    if (GX.pUnk7C7C->Unk128 == 0xFF) {
        return -1;
    }
    p = GX.pUnk7C7C->Tbl[GX.pUnk7C7C->Unk128];
    w = ((USHORT *)p)[1];
    t = w & 3;
    if (t == 3) {
        return 0xFF;
    }
    if (p[1] == 0) {
        return -1;
    }
    if (f) {
        if (t == 2) {
            ((USHORT *)p)[1] = w - 0x4000;
            if (!(((USHORT *)p)[1] & 0xC000)) {
                p[1]--;
                ((USHORT *)p)[1] |= 0xC000;
            }
        } else {
            p[1]--;
        }
    }
    return p[1];
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006D0D8);

extern char D_80010FEC[];

void func_8006D720(void)
{
    ST_WORK *pSt = &D_800E01C0;
    UCHAR *p;
    ULONG i;
    UCHAR n;
    LONG pad[2];

    if (Flg_ck(&D_800CC858, 0x18)) {
        switch (FLD(&D_800CC858, 0x3B6, UCHAR)) {
        case 8:
            func_80012818(0x5A, (ULONG *)0x801B1500, 0, "STMAIN1.TIM");
            break;
        case 9:
            func_80012818(0x5C, (ULONG *)0x801B1500, 0, "STMAIN2.TIM");
            break;
        case 10:
            func_80012818(0x5E, (ULONG *)0x801B1500, 0, "STMAIN3.TIM");
            break;
        }
    } else {
        func_80012818(0x58, (ULONG *)0x801B1500, 0, "STMAIN0.TIM");
    }
    FLD(&G, 0x2484, SHORT) = 0x41A;
    func_800784E0((ULONG *)0x801B1500);
    func_80012818(0x60, (ULONG *)0x801B1500, 0, "STMOJI.TIM");
    FLD(&G, 0x2484, SHORT) = 0x1A;
    func_800784E0((ULONG *)0x801B1500);
    func_80012818(0x32, (ULONG *)0x801B1500, 0, D_80010FEC);
    p = ((GLOBAL_X *)&G)->pUnk7C7C->Tbl[0];
    for (i = 0; i < 10; i++, p += 4) {
        func_8006AB68(i, *p, 0x801B1500, (ULONG *)0x801B1000);
        DrawSync(0);
    }
    if (GX.pUnk7C7C->Unk128 != 0xFF) {
        func_8006AE20(10, GX.pUnk7C7C->Unk128);
    }
    n = GX.pUnk7C7C->Unk129;
    FLD(pSt, 0xD0, UCHAR) = 4;
    FLD(pSt, 0xD1, UCHAR) = 5;
    FLD(pSt, 0xD2, UCHAR) = 6;
    FLD(pSt, 0xD4, UCHAR) = 8;
    FLD(pSt, 0xD5, UCHAR) = 14;
    FLD(pSt, 0xD7, UCHAR) = 10;
    FLD(pSt, 0xD3, UCHAR) = 0;
    FLD(pSt, 0xD8, UCHAR) = 9;
    FLD(pSt, 0xD6, UCHAR) = 0;
    FLD(pSt, 4, UCHAR) = 0;
    FLD(pSt, 6, UCHAR) = 0;
    FLD(pSt, 0x2E, UCHAR) = 0;
    FLD(pSt, 0xB8, SHORT) = 0;
    FLD(pSt, 0x28, UCHAR) = n;
}

INCLUDE_RODATA("asm/nonmatchings/game/800636D4", D_80010FEC);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006D948);

extern void (*D_800A02F0[])(ST_WORK *pSt);
void func_8006D948(ST_WORK *pSt);
void func_8006E0A4(ST_WORK *pSt);
void func_8006E268(ST_WORK *pSt);
void func_8006E34C(ST_WORK *pSt);
void FC_func_80073E7C(ST_WORK *pSt) __asm__("func_80073E7C");

void func_8006DFDC(void)
{
    ST_WORK *pSt = &D_800E01C0;

    func_8006D948(pSt);
    while (FLD(pSt, 0x27, UCHAR)) {
        if (Main.Unk194 != 1) {
            D_800A02F0[pSt->Rtn0](pSt);
            func_8006E268(pSt);
        }
        func_8006E34C(pSt);
        FC_func_80073E7C(pSt);
        Task_sleep(1);
    }
    func_8006E0A4(pSt);
    Task_exit();
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006E0A4);

void func_8006E268(ST_WORK *pSt)
{
    FLD(pSt, 0x34, UCHAR) = FB_func_8006E598();
    if (*(USHORT *)&pSt->Rtn0 == 0x205) {
        return;
    }
    if (pSt->Unk24) {
        pSt->Unk22 -= 2;
    } else {
        pSt->Unk22 += 2;
    }
    if (pSt->Unk22 >= 0x3F) {
        pSt->Unk22 = 0x3F;
        pSt->Unk24 = 1;
    }
    if (pSt->Unk22 <= 0) {
        pSt->Unk22 = 0;
        pSt->Unk24 = 0;
    }
    if (pSt->Flg & 0x800000) {
        FLD(pSt, 0x32, SHORT) += 3;
    } else if (++FLD(pSt, 0x32, SHORT) > 0x50) {
        FLD(pSt, 0x32, SHORT) = -0x20;
    }
}

void func_800654A8(ST_WORK *pSt);
void func_8006B66C(ST_WORK *pSt);
void func_80070244(ST_WORK *pSt);

void func_8006E34C(ST_WORK *pSt)
{
    switch (FLD(pSt, 4, UCHAR)) {
    case 4:
    case 5:
        if (pSt->Flg & 0x200000) {
            func_80070244(pSt);
        }
        break;
    case 2:
        func_800654A8(pSt);
        break;
    case 0:
    case 1:
        if (pSt->Flg & 0x40000000) {
            func_8006B66C(pSt);
        }
        if (pSt->Flg & 0x200000) {
            func_80070244(pSt);
        }
    case 3:
        if (pSt->Flg & 0x100000) {
            func_80063FE4(pSt);
        }
        break;
    }
}

void func_8006E424(ST_WORK *pSt)
{
    SND_WORK *pW = &Snd_work;

    if (Fade_status(0) != 0) {
        return;
    }
    if (FLD(pW, 0xB1C, LONG) != 0) {
        return;
    }
    switch (FLD(pSt, 4, UCHAR)) {
    case 4:
        pSt->Rtn0 = 4;
        break;
    case 5:
        pSt->Rtn0 = 4;
        break;
    case 3:
        pSt->Rtn0 = 7;
        break;
    case 2:
        pSt->Rtn0 = 10;
        break;
    case 0:
    case 1:
        pSt->Rtn0 = 1;
        break;
    }
}

void func_8006E4CC(ST_WORK *pSt)
{
    SND_WORK *pW = &Snd_work;
    LONG pad[1];

    switch (pSt->Rtn1) {
    case 0:
        Fade_set(0, 0, 2, 2, 0, 0xFFFFFF, 6);
        FLD(pW, 0xB1C, LONG) = 0x80;
        FLD(pW, 0xB20, LONG) = 0;
        FLD(pW, 0xB24, LONG) = -0xB00;
        pSt->Rtn1++;
        break;
    case 1:
        if ((Fade_status(0) & 4) && FLD(pW, 0xB1C, LONG) == 0) {
            FLD(pSt, 0x27, UCHAR) = 0;
        }
        break;
    }
}

LONG func_8006E598(void)
{
    USHORT f = GX.Unk255E;
    LONG l = GX.Unk2558;

    if (f & 0x100) {
        return 5;
    }
    if (f & 0x200) {
        return 4;
    }
    if (l > 100) {
        return 0;
    }
    if (l > 40) {
        return 1;
    }
    if (l > 20) {
        return 2;
    }
    return 3;
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006E600);

void func_8006E6D8(ST_WORK *pSt, POLY_FT4 *p, USHORT *src, ULONG prm)
{
    ULONG i, j;
    UCHAR y = prm >> 8;
    UCHAR semi = prm >> 16;
    UCHAR w = prm >> 24;
    ULONG n = prm;
    LONG tp;
    ULONG u, v;

    for (i = 0; i < (UCHAR)n; i++) {
        u = src[0];
        v = src[1];
        for (j = 0; j < 2; j++) {
            setPolyFT4(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = u;
            p->v0 = v;
            p->u1 = w + u;
            p->v1 = v;
            p->u2 = u;
            p->v2 = w + v;
            p->u3 = w + u;
            p->v3 = w + v;
            tp = semi == 0 ? 0x1B : 6;
            p->tpage = getTPage(1, 0, (tp & 0xF) << 6, (tp & 0x10) << 4);
            p->clut = (y + 0x1E0) << 6;
            p++;
        }
        src += 6;
    }
}

void func_8006E7D4(LONG a0, TILE *p, SHORT *pWh, UCHAR n)
{
    ULONG i, j;

    for (i = 0; i < n; i++, pWh += 6) {
        for (j = 0; j < 2; j++) {
            setTile(p);
            setRGB0(p, 8, 8, 8);
            p->w = pWh[2];
            p->h = pWh[3];
            p++;
        }
    }
}

void func_8006E84C(void)
{
    LINE_F2 *p;
    ULONG i, j, n;

    n = 32;
    p = (LINE_F2 *)0x801AF0E8;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 2; j++) {
            setLineF2(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            setSemiTrans(p, 1);
            p++;
        }
    }
}

void func_8006E8BC(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm)
{
    ULONG i;
    LONG ox, oy;
    UCHAR ofs = prm >> 8;
    UCHAR ot = prm >> 16;
    ULONG n = prm;
    SHORT *pO;

    if (G.Cbuf_id) {
        p++;
    }
    pO = (SHORT *)((UCHAR *)pSt + ofs * 4);
    ox = pO[0x72];
    oy = pO[0x73];
    for (i = 0; i < (UCHAR)n; i++, p += 2, src += 12) {
        p->x0 = *(SHORT *)(src + 8) + ox;
        p->y0 = *(SHORT *)(src + 10) + oy;
        AddPrim((UCHAR *)pSt + 0x164 + G.Cbuf_id * 0x140 + ot * 8, p);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006E9D4);

void func_8006EAF0(ST_WORK *pSt, TILE *p, UCHAR *src, ULONG prm)
{
    ULONG i;
    LONG ox, oy;
    UCHAR ofs = prm >> 8;
    UCHAR ot = prm >> 16;
    ULONG n = prm;
    SHORT *pO;

    if (G.Cbuf_id) {
        p++;
    }
    pO = (SHORT *)((UCHAR *)pSt + ofs * 4);
    ox = pO[0x72];
    oy = pO[0x73];
    for (i = 0; i < (UCHAR)n; i++, p += 2, src += 12) {
        p->x0 = *(SHORT *)(src + 8) + ox;
        p->y0 = *(SHORT *)(src + 10) + oy;
        AddPrim(G.pOt0 + ot, p);
    }
}

void func_8006EBEC(void)
{
    TIM_IMAGE tim;
    TIM_IMAGE *p;
    LONG y;

    FB_OpenTIM();
    p = ReadTIM(&tim);
    p->prect->x = (((FB_G2484 *)&G)->Unk2484 << 6) - (((FB_G2484 *)&G)->Unk2484 >= 0x10 ? 0x400 : 0);
    y = p->prect->y;
    if (((FB_G2484 *)&G)->Unk2484 >= 0x10) {
        y += 0x100;
    }
    p->prect->y = y;
    LoadImage(p->prect, p->paddr);
    if (p->caddr) {
        p->crect->y = ((FB_G2484 *)&G)->Unk2485 + 0x1E0;
        LoadImage(p->crect, p->caddr);
    }
    DrawSync(0);
}

void func_8006ECB0(ULONG no)
{
    switch (no) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        D_800D23B8 = 0;
        break;
    case 8:
        D_800D23B8 = 1;
        break;
    }
    GX.pUnk7C7C = &GX.Unk79FC[GX.Unk7C80];
    D_800E01C0.Flg14 |= 0x40000000;
}

void func_8006ED28(ST_WORK *pSt)
{
    UCHAR c;

    pSt->UnkBB = 0;
    pSt->UnkBA = 0;
    pSt->UnkAB = GX.Unk783E;
    pSt->UnkAC = GX.Unk7840;
    func_80073CCC(&pSt->UnkAB, &pSt->UnkAC);
    pSt->UnkAA = GX.Unk783E;
    c = GX.Unk7840;
    func_80073CCC(&pSt->UnkAA, &c);
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006ED98);

void func_8006F6CC(ST_WORK *pSt)
{
    D_800A0500[pSt->Rtn1](pSt);
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006F708);

void func_8006FAE4(ST_WORK *pSt)
{
    if (FLD(pSt, 0xA8, UCHAR)) {
        FLD(pSt, 0x8C, LONG) += FLD(pSt, 0x84, LONG) - FLD(pSt, 0x94, LONG);
        FLD(pSt, 0x90, LONG) += FLD(pSt, 0x88, LONG) - FLD(pSt, 0x98, LONG);
        FLD(pSt, 0x7C, LONG) = FLD(pSt, 0x84, LONG) - FLD(pSt, 0x8C, LONG) / 10;
        FLD(pSt, 0x80, LONG) = FLD(pSt, 0x88, LONG) - FLD(pSt, 0x90, LONG) / 10;
        if ((FLD(pSt, 0xA4, LONG) -= 5) < 0x33) {
            pSt->Rtn1 = 0;
            FLD(pSt, 0xA4, LONG) = 0x32;
            FLD(pSt, 0x7C, LONG) = FLD(pSt, 0x94, LONG);
            FLD(pSt, 0x80, LONG) = FLD(pSt, 0x98, LONG);
        }
    } else {
        FLD(pSt, 0x8C, LONG) += FLD(pSt, 0x94, LONG) - FLD(pSt, 0x84, LONG);
        FLD(pSt, 0x90, LONG) += FLD(pSt, 0x98, LONG) - FLD(pSt, 0x88, LONG);
        FLD(pSt, 0x7C, LONG) = FLD(pSt, 0x94, LONG) - FLD(pSt, 0x8C, LONG) / 10;
        FLD(pSt, 0x80, LONG) = FLD(pSt, 0x98, LONG) - FLD(pSt, 0x90, LONG) / 10;
        if ((FLD(pSt, 0xA4, LONG) += 5) >= 100) {
            FLD(pSt, 0xA4, LONG) = 100;
            pSt->Rtn1 = 0;
            FLD(pSt, 0xA9, UCHAR) = 1;
            FLD(pSt, 0xBE, UCHAR) = 0;
            FLD(pSt, 0x7C, LONG) = FLD(pSt, 0x84, LONG);
            FLD(pSt, 0x80, LONG) = FLD(pSt, 0x88, LONG);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006FC68);

void func_8006FEB0(ST_WORK *pSt)
{
    switch (pSt->Rtn2) {
    case 0:
        Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 8);
        pSt->Rtn2++;
        break;
    case 1:
        if (Fade_status(0) == 0) {
            pSt->Rtn1 = 0;
            pSt->Rtn2 = 0;
        }
        break;
    }
}

void func_8006FF48(ST_WORK *pSt)
{
    switch (pSt->Rtn2) {
    case 0:
        Fade_set(0, 0, 2, 2, 0, 0xFFFFFF, 8);
        pSt->Rtn2++;
        break;
    case 1:
        if (Fade_status(0) & 4) {
            pSt->Rtn0 = 4;
            pSt->Flg &= ~0x200000;
            pSt->Rtn1 = 0;
            pSt->Rtn2 = 0;
            FLD(pSt, 0x13, UCHAR) = 0;
            pSt->UnkAB = D_800A04F8[(signed char)pSt->UnkBB];
            pSt->UnkBA = 1;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80070024);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80070244);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8007116C);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800713DC);

void func_80071510(UCHAR a, UCHAR b)
{
    ULONG *p;

    if (a == 0 && b == 16) {
        D_800D20DC[0] |= 0x80000000U >> (b & 0x1F);
    }
    if (a == 1 && b == 0) {
        D_800D20DC[2] |= 0x80000000U >> (b & 0x1F);
    }
    if (a == 1 && b == 1) {
        D_800D20DC[1] |= 0x10000;
    }
    if (a == 3 && b == 21) {
        D_800D20DC[6] |= 0x80000000U >> (b & 0x1F);
    }
    func_80073CCC(&a, &b);
    p = &D_800D20DC[a * 2 + (b >> 5)];
    *p |= 0x80000000U >> (b & 0x1F);
    Flg_on(&D_800D20DC[19], a);
}

ULONG func_80071668(UCHAR a, UCHAR b)
{
    ULONG *p = &D_800D20DC[a * 2 + (b >> 5)];

    return *p & (0x80000000U >> (b & 0x1F));
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800716A4);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80071878);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8007275C);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80072AEC);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80072D38);

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800738CC);

LONG func_80073BC8(LONG a0, UCHAR st, UCHAR rm)
{
    switch (st) {
    case 2:
        if (rm == 12) {
            return 2;
        }
        return rm >= 10;
    case 4:
        if (rm < 4 || (rm >= 8 && rm < 11)) {
            return 1;
        }
        if ((rm >= 11 && rm < 18) || rm == 6 || (rm >= 4 && rm < 8)) {
            return 0;
        }
        break;
    case 5:
        return rm >= 8;
    case 6:
        if (rm >= 5 && rm < 9) {
            return 2;
        }
        if (rm < 5) {
            return 1;
        }
        if (rm < 9) {
            break;
        }
        return 0;
    }
    return 0;
}

void func_80073CCC(UCHAR *pA, UCHAR *pB)
{
    LONG i;

    if (*pA == 1 && *pB == 15 && G.Pl_pos_x >= -0x2276) {
        *pB = 16;
        return;
    }
    if (*pA == 3 && *pB == 21 && G.Pl_pos_x >= 800) {
        *pB = 0x11;
        return;
    }
    if (*pA == 4 && *pB == 2 && Flg_ck(D_800D1FC0, 0x1F)) {
        *pB = 0x11;
        return;
    }
    if (*pA == 2 && *pB == 0x17 && G.Pl_pos_z >= -0x2C24 && !Flg_ck((ULONG *)((UCHAR *)&G + 0x7868), 200)) {
        *pB = 2;
    }
    if (*pA >= 5) {
        *pA -= 5;
    }
    for (i = 0; D_800A0430[i].A0 != 0xFF; i++) {
        if (D_800A0430[i].A0 == *pA && D_800A0430[i].B0 == *pB) {
            *pA = D_800A0430[i].A1;
            *pB = D_800A0430[i].B1;
        }
    }
}

void func_80073E7C(void) {
}
