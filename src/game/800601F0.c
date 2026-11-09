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

void func_800601F0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR V;
    USHORT f;
    ULONG n;

    if (!(pEm->Type & 0x4000)) {
        pEm->Type |= 0x5000;
    }
    if (!(pEm->Type & 0x1000)) {
        V.vx = pEm->Unk14C;
        V.vz = pEm->Unk14E;
        V.vy = 0;
        func_80049BA8(pEm, &V, 0, 0);
    }
    f = pEm->Unk144;
    if (!(f & 0x10)) {
        D_8009EE44[pEm->Routine_1](pEm, pKan, pSeq);
        pEm->Unk144 &= ~0x800;
    } else if (pEm->Routine_1 != 0x14) {
        if (!(f & 0x800)) {
            D_8009EE44[pEm->Routine_1](pEm, pKan, pSeq);
            func_80060464(pEm, pKan, pSeq);
        } else {
            n = f & 0x80;
            n = n != 0;
            func_8006037C(pEm, pKan, pSeq, n << 16 | 0x200);
        }
    }
    if (pEm->Unk144 & 1) {
        func_80019318(pEm, 0, 0);
    }
    if (pEm->Unk144 & 2) {
        func_80019318(pEm, 0, 1);
    }
}

LONG func_8006037C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg)
{
    UCHAR r;
    if (pEm->Routine_2 != 2) {
        r = pEm->Routine_1;
        switch (r) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            pKan = pEm->pSub0_kan_t_ptr;
            pSeq = pEm->pSub0_seq_t_ptr;
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        func_80027338(pKan, pSeq, 0x80, Flg);
        if (!(pEm->Unk144 & 0x20)) {
            ((void (*)(ENEMY_WORK *, LONG, LONG))Add_speedXZ)(pEm, 0, 0);
        }
        pEm->Unk144 &= ~0x800;
    }
}

void func_80060464(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT n;

    switch (pEm->Routine_1) {
    case 0:
        pKan = pEm->pUnk100_kan_t_ptr;
        pSeq = pEm->pUnk104_seq_t_ptr;
        break;
    case 1:
        pKan = pEm->pKan_t_ptr;
        pSeq = pEm->pSeq_t_ptr;
        break;
    case 2:
        pKan = pEm->pSub0_kan_t_ptr;
        pSeq = pEm->pSub0_seq_t_ptr;
        break;
    case 3:
        pKan = pEm->pSub1_kan_t_ptr;
        pSeq = pEm->pSub1_seq_t_ptr;
        break;
    }
    n = *(USHORT *)&pSeq[pEm->Move_no * 2];
    if (!(pEm->Unk144 & 0x800)) {
        if (pEm->Move_cnt == 0) {
            pEm->Move_cnt = n - 1;
        } else {
            pEm->Move_cnt--;
        }
        func_8002B2BC(pKan, pSeq, pEm->pNow_seq, *(UCHAR *)&pEm->Unk144 >> 7);
    }
    pEm->Unk144 ^= 0x800;
}

void func_8006055C(ENEMY_WORK *pEm)
{
    USHORT v;
    LONG n;

    if (!(pEm->Unk144 & 0x100)) {
        v = *pEm->pNow_seq;
        if (v & 0x200) {
            n = v & 0x400;
            n = n != 0;
            func_800776B0(1, n * 3 + 0xB, pEm);
        }
    }
}

void func_800605C0(ENEMY_WORK *pEm)
{
    func_80062D3C(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr);
    func_8006055C(pEm);
}

void func_800605F8(ENEMY_WORK *pEm)
{
    func_80062D3C(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    func_8006055C(pEm);
}

void func_80060630(ENEMY_WORK *pEm)
{
    func_80062D3C(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    func_8006055C(pEm);
}

void func_80060668(ENEMY_WORK *pEm)
{
    func_80062D3C(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
    func_8006055C(pEm);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800606A0);

void func_800608C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->UnkAE & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x20404;
        *(LONG *)&pEm->Move_no = 0x70000;
        func_800606A0(pEm);
        return;
    }
    if (!(func_800632B8(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else if (pEm->Routine_2 == 3) {
        *(USHORT *)&pEm->Routine_2 = 0;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_800632B8(pEm, 1);
        pEm->Routine_2 = 1;
        Spd_get(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), 0);
    case 1:
        Spd_get(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), 0);
        if ((UCHAR)func_80063198(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xC;
                pEm->Routine_2 = 1;
            }
        }
        break;
    case 2:
        *(LONG *)&pEm->Move_no = 0x70000;
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if ((UCHAR)func_80062BFC(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xC;
                pEm->Routine_2 = 3;
            }
        }
        break;
    }
    func_8006055C(pEm);
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_800632B8(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_800632B8(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void func_80060BF0(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 1;
    ULONG r;
    SHORT *pT;

    if (EM_P(pEm, 0x150, USHORT) & 0x80) {
        switch (EM_P(pEm, 0x150, USHORT) & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = EM_P(pEm, 0x152, SHORT);
    }
    switch (pEm->Routine_2) {
    case 0:
        *(LONG *)&pEm->Move_no = n + 0x70000;
        pEm->Routine_2 = 1;
        pEm->Routine_3 = 0;
    case 1:
        if ((UCHAR)func_80062B68(pEm, pKan, pSeq)) {
            Spd_get(pEm, pKan, pSeq, 0);
            pEm->Routine_2 = 2;
            pEm->Routine_3 = 1;
        }
        break;
    case 2:
        if (pEm->UnkAE & 0x40) {
            r = func_800632B8(pEm, 0) & 0x1000;
            if (r) {
                r = 0xD04;
            } else {
                r = 0x20D04;
            }
            *(ULONG *)&pEm->Routine_0 = r;
            func_80060E90(pEm, pKan, pSeq);
            return;
        }
        if (pEm->Id < 0x10) {
            if (pEm->Id < 8) {
                pEm->Spd_x = D_8009EDA4[pEm->Move_cnt];
                pEm->Spd_z = D_8009EDCC[pEm->Move_cnt];
            } else {
                pEm->Spd_x = D_8009EDF4[pEm->Move_cnt];
                pEm->Spd_z = D_8009EE1C[pEm->Move_cnt];
            }
            if (pEm->Routine_3) {
                pEm->Routine_3 = 0;
                pEm->Spd_z = 0;
                pEm->Spd_x = 0;
            }
        } else {
            Spd_get(pEm, pKan, pSeq, 0);
        }
        if ((UCHAR)func_80062BFC(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 5;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_8006055C(pEm);
}

void func_80060E90(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->UnkAE & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x20504;
        *(LONG *)&pEm->Move_no = 0x70001;
        func_80060BF0(pEm);
        return;
    }
    if (!(func_800634F4(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else if (pEm->Routine_2 == 3) {
        *(USHORT *)&pEm->Routine_2 = 0;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_800634F4(pEm, 1);
        pEm->Routine_2 = 1;
        Spd_get(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), 0);
    case 1:
        Spd_get(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), 0);
        if ((UCHAR)func_80063198(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xD;
                pEm->Routine_2 = 1;
            }
        }
        break;
    case 2:
        *(LONG *)&pEm->Move_no = 0x70001;
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if ((UCHAR)func_80062BFC(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xD;
                pEm->Routine_2 = 3;
            }
        }
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_800634F4(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_800634F4(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void func_80061108(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 2;

    if (EM_P(pEm, 0x150, USHORT) & 0x80) {
        switch (EM_P(pEm, 0x150, USHORT) & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = EM_P(pEm, 0x152, SHORT);
    }
    switch (pEm->Routine_2) {
    case 0:
        *(LONG *)&pEm->Move_no = n + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        func_80027940(pEm, pKan, pSeq, 0x200);
        break;
    }
}

void func_800611E0(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 9;
    ULONG r;

    if (EM_P(pEm, 0x150, USHORT) & 0x80) {
        switch (EM_P(pEm, 0x150, USHORT) & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = EM_P(pEm, 0x152, SHORT);
    }
    if (pEm->UnkAE & 0x40) {
        r = func_800632B8(pEm, 0) & 0x1000;
        if (r) {
            r = 0xE04;
        } else {
            r = 0x20E04;
        }
        *(ULONG *)&pEm->Routine_0 = r;
        func_80061390(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        *(LONG *)&pEm->Move_no = n + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if ((UCHAR)func_80062BFC(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 7;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_8006055C(pEm);
}

void func_80061390(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(func_800632B8(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else if (pEm->Routine_2 == 3) {
        *(USHORT *)&pEm->Routine_2 = 0;
    }
    if (!(pEm->UnkAE & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x704;
        MY_func_800611E0(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        func_800632B8(pEm, 1);
        pEm->Routine_2 = 1;
        Spd_get(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), 1);
    case 1:
        Spd_get(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), 1);
        if ((UCHAR)func_80063198(pEm, pKan, pSeq, 1)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xE;
                pEm->Routine_2 = 1;
            }
        }
        break;
    case 2:
        *(LONG *)&pEm->Move_no = 0x70009;
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if ((UCHAR)func_80062BFC(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xE;
                pEm->Routine_2 = 3;
            }
        }
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_800632B8(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_800632B8(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void func_800616B4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG n = 0;

    if (EM_P(pEm, 0x150, USHORT) & 0x80) {
        switch (EM_P(pEm, 0x150, USHORT) & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            pKan = pEm->pSub0_kan_t_ptr;
            pSeq = pEm->pSub0_seq_t_ptr;
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = EM_P(pEm, 0x152, SHORT);
    }
    switch (pEm->Routine_2) {
    case 0:
        *(LONG *)&pEm->Move_no = n + 0x70000;
        pEm->Spd_z = 0;
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if ((UCHAR)func_80062BFC(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 8;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_8006055C(pEm);
}

void func_80061830(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 0;

    if (EM_P(pEm, 0x150, USHORT) & 0x80) {
        switch (EM_P(pEm, 0x150, USHORT) & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = EM_P(pEm, 0x152, SHORT);
    }
    switch (pEm->Routine_2) {
    case 0:
        *(LONG *)&pEm->Move_no = n + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)func_80062B68(pEm, pKan, pSeq)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, USHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 9;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_8006055C(pEm);
}

void func_80061964(ENEMY_WORK *pEm)
{
    pEm->Routine_2 = *((UCHAR *)pEm + 0xD4);
}

void func_80061970(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if ((pEm->Routine_2 | pEm->Routine_3) == 0) {
        func_80061964(pEm);
    }
    D_8009EEA4[pEm->Routine_2](pEm, pKan, pSeq);
}

void func_800619F0(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(LONG *)&pEm->Move_no = pEm->Routine_2 ? 0x70007 : 0x70000;
        EM_P(pEm, 0x1A0, LONG) = pEm->Routine_2 ? 0x70007 : 0x70000;
        EM_P(pEm, 0x120, UCHAR) |= 0x12;
    case 1:
        if (func_80026BE8(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200) != 0) {
            *(USHORT *)&pEm->Routine_2 = pEm->Routine_2 ? 6 : 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x200);
        func_80079868(pEm, 0x200);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80061AD0);

void func_80061BC4(ENEMY_WORK *pEm)
{
    USHORT f = EM_P(pEm, 0xD6, USHORT);
    LONG n = f & 0xF;
    ENEMY_WORK *pT;
    UCHAR *pP;
    SVECTOR sv;
    LONG r;

    if (f & 0xF0) {
        pT = ((G265C *)&G)->pEm[(f >> 4) & 0xF];
        if (pT != pEm) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xD8);
        }
        if (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x50)) {
            return;
        }
        EM_P(pEm, 0xD6, USHORT) &= 0xF;
    }
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pP = (UCHAR *)pEm->pSin_parts_ptr;
        EM_P(pEm, 0x1A0, LONG) = *(LONG *)&pEm->Move_no = n * 2 + 0x30001;
        func_800775A0(0xF, pEm);
        sv.vx = 100;
        sv.vy = 150;
        sv.vz = 0;
        func_8001B484(2, ((USHORT)(pEm->Cdir_y + 0x400)) | 0x8000000, pP + 0x330, &sv, 0);
        sv.vx = 100;
        sv.vy = 450;
        sv.vz = 0;
        func_8001B484(1, (USHORT)pEm->Cdir_y | 0x10000000, pP + 0x330, &sv, 0);
        sv.vx = 100;
        sv.vy = 350;
        sv.vz = 0;
        func_8001B484(3, (USHORT)pEm->Cdir_y | 0x10000000, pP + 0x330, &sv, 0);
        EM_P(pEm, 0x1D0, USHORT) &= ~0x4000;
        r = func_80044804((UCHAR *)pEm->pSin_parts_ptr + 0x330, D_8009DBD4, 3);
        if (r != 0 && r != 0xFF) {
            EM_P(pEm, 0x1D0, USHORT) |= 0x4000;
        }
    case 1:
        if (func_80026BE8(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200) != 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x400);
        func_80079868(pEm, 0x200);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80061DF4);

void func_800621F0(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(LONG *)&pEm->Move_no = pEm->Routine_2 != 3 ? 0x70007 : 0x70000;
        EM_P(pEm, 0x1A0, LONG) = pEm->Routine_2 != 3 ? 0x70007 : 0x70000;
    case 1:
        if (func_80026BE8(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x10200) != 0) {
            *(ULONG *)&pEm->Routine_0 = 0x604;
            EM_P(pEm, 0x120, UCHAR) &= 0xED;
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x10200);
        func_80079868(pEm, 0x200);
        break;
    }
}

void func_800622D8(ENEMY_WORK *pEm)
{
    LONG pad[2];

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(LONG *)&pEm->Move_no = 0x70007;
        EM_P(pEm, 0x1A0, LONG) = 0x70007;
    }
    if (func_80026BE8(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200) != 0) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x200);
    func_80079868(pEm, 0x200);
}

void func_8006236C(ENEMY_WORK *pEm)
{
    pEm->pEnemy_ptr = ((G265C *)&G)->pEm[*(SHORT *)((UCHAR *)pEm + 0xD6) + 2];
    pEm->pSub1_kan_t_ptr = pEm->pEnemy_ptr->pSub1_kan_t_ptr;
    pEm->pSub1_seq_t_ptr = pEm->pEnemy_ptr->pSub1_seq_t_ptr;
    func_800623C4(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800623C4);

void func_80062970(void) {
}

void func_80062978(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        *(LONG *)&pEm->Move_no = 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if (func_80062CD0(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr)) {
            Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            EM_P(pEm, 0x152, SHORT) = 0;
            EM_P(pEm, 0x150, SHORT) = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0x15;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_8006055C(pEm);
}

void func_80062A28(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT f;
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        *((UCHAR *)pEm + 0xCA) = 7;
        if (pEm->Unk144 & 0x40) {
            *((UCHAR *)pEm + 0xCA) = 0;
        }
    case 1:
        if (func_80027940(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
            }
            pEm->Cdir_y += EM_P(pEm, 0x14A, SHORT);
        } else {
            f = pEm->Unk144;
            if (f & 8) {
                n = f & 0x80;
                n = n != 0;
                if (func_80027940(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += EM_P(pEm, 0x14A, SHORT);
                }
            }
            pEm->Cdir_y += EM_P(pEm, 0x148, SHORT);
        }
        break;
    case 2:
        break;
    }
}

LONG func_80062B68(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG ret;

    if (Goto00_ck(pEm, EM_P(pEm, 0xD4, SHORT), EM_P(pEm, 0xD6, SHORT), 0x15E) != 0) {
        Goto00(pEm, EM_P(pEm, 0xD4, SHORT), EM_P(pEm, 0xD6, SHORT), EM_P(pEm, 0x148, SHORT));
        func_80027940(pEm, pKan, pSeq, 0x200);
        ret = 0;
    } else {
        func_80027940(pEm, pKan, pSeq, 0x200);
        ret = 1;
    }
    return ret;
}

LONG func_80062BFC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d)
{
    LONG x, z;

    Goto00(pEm, EM_P(pEm, 0xD4, SHORT), EM_P(pEm, 0xD6, SHORT), (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * EM_P(pEm, 0x148, SHORT));
    func_80027940(pEm, pKan, pSeq, (d << 16) | 0x200);
    Add_speedXZ(pEm, d);
    x = pEm->Pos_x - EM_P(pEm, 0xD4, SHORT);
    z = pEm->Pos_z - EM_P(pEm, 0xD6, SHORT);
    return SquareRoot0(x * x + z * z) < 0x69;
}

UCHAR func_80062CD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Goto01(pEm, *(SHORT *)((UCHAR *)pEm + 0xD4), *(SHORT *)((UCHAR *)pEm + 0xD6));
    func_80027940(pEm, pKan, pSeq, 0x200);
    return pEm->Cdir_y == *(SHORT *)((UCHAR *)pEm + 0xD4);
}

void func_80062D3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT f;
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        *((UCHAR *)pEm + 0xCA) = 7;
        if (pEm->Unk144 & 0x40) {
            *((UCHAR *)pEm + 0xCA) = 0;
        }
        func_80018EC8(pEm, pKan, pSeq, *(UCHAR *)&pEm->Unk144 >> 7);
    case 1:
        func_80018EC8(pEm, pKan, pSeq, *(UCHAR *)&pEm->Unk144 >> 7);
        if (pEm->Unk144 & 0x10) {
            pEm->Spd_x >>= 1;
            pEm->Spd_y >>= 1;
            pEm->Spd_z >>= 1;
        }
        if (func_80027940(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
                if (pEm->Unk144 & 0x10) {
                    pEm->Unk144 |= 0x800;
                }
            }
            pEm->Cdir_y += EM_P(pEm, 0x14A, SHORT);
            if (!(pEm->Unk144 & 0x20)) {
                FA_Add_speedXZ3(pEm, 0, 0);
            }
        } else {
            if (!(pEm->Unk144 & 0x20)) {
                FA_Add_speedXZ3(pEm, 0, 0);
            }
            f = pEm->Unk144;
            if (f & 8) {
                n = f & 0x80;
                n = n != 0;
                if (func_80027940(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += EM_P(pEm, 0x14A, SHORT);
                }
                if (!(pEm->Unk144 & 0x20)) {
                    FA_Add_speedXZ3(pEm, 0, 0);
                }
            }
            pEm->Cdir_y += EM_P(pEm, 0x148, SHORT);
        }
        break;
    case 2:
        break;
    }
}

void func_80062F64(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT f;
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        *((UCHAR *)pEm + 0xCA) = 7;
        if (pEm->Unk144 & 0x40) {
            *((UCHAR *)pEm + 0xCA) = 0;
        }
        Spd_get(pEm, pKan, pSeq, *(UCHAR *)&pEm->Unk144 >> 7);
    case 1:
        if (func_80026BE8(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
            }
            pEm->Cdir_y += EM_P(pEm, 0x14A, SHORT);
            if (!(pEm->Unk144 & 0x20)) {
                FA_Add_speedXZ3(pEm, 0, 0);
            }
        } else {
            if (pEm->Id == 0) {
                pEm->Be_flg |= 0x2000000;
                func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x200);
                func_80079868(pEm, 0x200);
            }
            if (!(pEm->Unk144 & 0x20)) {
                FA_Add_speedXZ3(pEm, 0, 0);
            }
            f = pEm->Unk144;
            if (f & 8) {
                n = f & 0x80;
                n = n != 0;
                if (func_80026BE8(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += EM_P(pEm, 0x14A, SHORT);
                }
                if (pEm->Id == 0) {
                    pEm->Be_flg |= 0x2000000;
                    func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x200);
                    func_80079868(pEm, 0x200);
                }
                if (!(pEm->Unk144 & 0x20)) {
                    FA_Add_speedXZ3(pEm, 0, 0);
                }
            }
            pEm->Cdir_y += EM_P(pEm, 0x148, SHORT);
        }
        break;
    case 2:
        break;
    }
}

LONG func_80063198(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d)
{
    LONG x, z;

    Goto00(pEm, EM_P(pEm, 0xD4, SHORT), EM_P(pEm, 0xD6, SHORT), (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * EM_P(pEm, 0x148, SHORT));
    func_80026BE8(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), (d << 16) | 0x200);
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, EM_P(pEm, 0x194, ULONG *), EM_P(pEm, 0x190, ULONG *), d ? 0x10200 : 0x200);
    func_80079868(pEm, 0x200);
    pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x << 2) / 3;
    Add_speedXZ(pEm, 0);
    x = pEm->Pos_x - EM_P(pEm, 0xD4, SHORT);
    z = pEm->Pos_z - EM_P(pEm, 0xD6, SHORT);
    return SquareRoot0(x * x + z * z) < 0x64;
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800632B8);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800634F4);

LONG func_80063690(ENEMY_WORK *pEm)
{
    LONG id;

    if (*((UCHAR *)pEm + 0x4B) == 0 || (id = pEm->Id) < 8) {
        return 0;
    }
    if (id < 0x10) {
        return 1;
    }
    return id - 0x4E;
}

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

INCLUDE_RODATA("asm/nonmatchings/game/800601F0", D_80010DCC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80063850);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80063CAC);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800642D4);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006446C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800646F0);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80064E80);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006511C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800654A8);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80065BA4);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80065D04);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800661B4);

void func_80066530(ST_WORK *pSt)
{
switch (ST_P(pSt, 4, UCHAR)) { case 1: pSt->Rtn1 = 0xB; pSt->Rtn2 = 0; pSt->Rtn0++; break; case 0: ST_P(pSt, 0xF0, SHORT) = 0x48; ST_P(pSt, 0xF4, SHORT) = 0x48; ST_P(pSt, 0xF8, SHORT) = 0x48; ST_P(pSt, 0xFC, SHORT) = 0x48; ST_P(pSt, 0x100, SHORT) = 0x48; ST_P(pSt, 0x104, SHORT) = -0xD8; ST_P(pSt, 0x108, SHORT) = -0xD8; ST_P(pSt, 0x11C, SHORT) = -0xD8; ST_P(pSt, 0x144, SHORT) = -0xD8; ST_P(pSt, 0x10E, SHORT) = 0x30; pSt->Rtn1 = 0; pSt->Unk22 = 0; pSt->Unk24 = 0; pSt->Flg |= 0x20000000; pSt->Flg |= 0x40000000; pSt->Rtn0++; break; }
}

void func_800665C8(ST_WORK *pSt)
{
    D_800A0100[pSt->Rtn1](pSt);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80066604);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80066CA0);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800676B8);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80067C14);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80068024);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80068BC0);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80069280);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80069614);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80069A6C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80069C3C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006A234);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006A59C);

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

INCLUDE_RODATA("asm/nonmatchings/game/800601F0", D_80010F84);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006AC88);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006AE90);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006B66C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006C370);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006C484);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006C6D0);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006CA10);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006CD68);

void func_8006CEA8(void)
{
    UCHAR no;

    if ((no = GX.pUnk7C7C->Unk128) != 0xFF) {
        func_8006CD68(no);
        GX.pUnk7C7C->Unk129 = 0;
        GX.pUnk7C7C->Unk128 = 0xFF;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006CF0C);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006D0D8);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006D720);

INCLUDE_RODATA("asm/nonmatchings/game/800601F0", D_80010FEC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006D948);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006DFDC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006E0A4);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006E34C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006E424);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006E600);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006E9D4);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006ED98);

void func_8006F6CC(ST_WORK *pSt)
{
    D_800A0500[pSt->Rtn1](pSt);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006F708);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8006FC68);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80070024);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80070244);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007116C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800713DC);

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

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800716A4);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80071878);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007275C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80072AEC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80072D38);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800738CC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80073BC8);

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

void func_80073E84(void)
{
    SpuReverbAttr r;
    SpuCommonAttr *pA;
    FC_SNDW *pW = &FC_SW;

    SsUtReverbOff();
    pA = &pW->Cattr;
    pW->Cattr.mask = 0x2CF;
    pA->mvol.right = 0x3FFF;
    pA->mvol.left = 0x3FFF;
    pA->mvolmode.right = 0;
    pA->mvolmode.left = 0;
    pA->cd.volume.right = 0x1FFF;
    pA->cd.volume.left = 0x1FFF;
    pA->cd.mix = 1;
    SpuSetCommonAttr(pA);
    SpuSetTransferMode(0);
    pW->Vol[0] = 0x7F;
    pW->Vol[1] = 0x7F;
    pW->Vol[2] = 0x7F;
    pW->Vol[3] = 0x7F;
    pW->Vol[4] = 0x7F;
    pW->Vol[5] = 0x7F;
    pW->Vol[6] = 0x7F;
    pW->Vol[7] = 0x7F;
    Snd_work_init();
    pW->Rv_mode = 0;
    pW->Rv_cur = 0;
    pW->Rv_step = 0;
    r.mask = 1;
    r.mode = 0;
    SpuSetReverbModeParam(&r);
    SpuSetReverb(0);
    VSync(30);
    StopRCnt(0xF2000002);
    EnterCriticalSection();
    D_800E0608 = OpenEvent(0xF2000002, 2, 0x1000, func_80075C30);
    ExitCriticalSection();
    SetRCnt(0xF2000002, 20000, 0x1000);
    EnableEvent(D_800E0608);
    StartRCnt(0xF2000002);
    pW->UnkB74 = 0;
}

void Snd_reset(void)
{
    Snd_seq_stop_all();
    Snd_work_init();
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", Snd_work_init);

void func_800740AC(void)
{
    if (FC_SW.Rv_step == 0) {
        SpuReserveReverbWorkArea(1);
        SpuClearReverbWorkArea(D_800A1108[FC_SW.Rv_mode - 1].mode);
        FC_SW.Rv_cnt = 30;
        FC_SW.Rv_step++;
    } else if (--FC_SW.Rv_cnt == 0) {
        SpuSetReverbModeParam((SpuReverbAttr *)D_800A10F4[FC_SW.Rv_mode]);
        SpuSetReverbDepth((SpuReverbAttr *)D_800A10F4[FC_SW.Rv_mode]);
        SpuSetReverb(1);
        FC_SW.Rv_step = 0;
        FC_SW.Rv_cur = FC_SW.Rv_mode;
        FC_SW.Rv_mode = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800741A0);

void func_80074658(LONG on)
{
    SpuCommonAttr *p = &D_800E10F8;

    if (on) {
        p->cd.volume.right = 0x7FFF;
        p->cd.volume.left = 0x7FFF;
        p->cd.mix = 1;
        D_800E10F8.mask |= 0x2C0;
    } else {
        p->cd.volume.left = 0;
        p->cd.volume.right = 0;
        p->cd.mix = 0;
        D_800E10F8.mask &= ~0x2C0;
    }
    SpuSetCommonAttr(p);
}

void func_800746C0(ULONG code, VECTOR *pPos, SHORT a2, SHORT a3)
{
    if (Snd_work.pReq != (SND_REQ *)&Snd_work.pReq && (!(D_800CC858 & 0x10000000) || ((code >> 8) & 0xFF) < 5)) {
        Snd_work.pReq->Unk1E = code >> 16;
        Snd_work.pReq->Bank = code >> 8;
        Snd_work.pReq->No = code & 0xFF;
        Snd_work.pReq->Pos = *pPos;
        Snd_work.pReq->Unk1A = a2;
        Snd_work.pReq->Unk18 = a3;
        Snd_work.pReq++;
    }
}

void func_80074770(SND_REQ *pReq)
{
    ULONG *p;
    LONG b;

    p = Snd_work.pAdr[pReq->Bank] + (USHORT)pReq->No;
    if (*p != -1 && Snd_bank_search(pReq->Bank) != -1) {
        b = Snd_bank_search(((UCHAR)*p & 0xE) >> 1);
        if (b != -1) {
            func_800749A0(pReq, Snd_work.Bank_id[b].Id, p, b);
        }
    }
}

void func_80074820(SND_REQ *pReq)
{
    LONG no;
    LONG b;
    LONG k;
    SND_SEQ *pSeq;
    SND_WORK *pW;

    k = 5;
    pW = &Snd_work;
    no = pReq->Bank - 5;
    if (no) {
        k = 6;
    }
    b = Snd_bank_search(k);
    if (b != -1) {
        pSeq = &pW->Seq[no];
        FLD(pSeq, 0x20, LONG) = b;
        FLD(pSeq, 0x26, SHORT) = no;
        if (pSeq->Unk2C) {
            Snd_seq_stop(no);
            FLD(pSeq, 0x54, UCHAR) = 2;
        } else {
            FLD(pSeq, 0x54, UCHAR) = 1;
        }
        FLD(pSeq, 0, ULONG *) = pW->Bank[no ? 6 : 5].pHead;
        FLD(pSeq, 0x48, LONG) = FLD(pSeq, 0x26, SHORT);
        FLD(pSeq, 0x44, ULONG *) = pW->Seq_data[no].pData;
        FLD(pSeq, 0x4C, LONG) = pW->Bank_id[no ? 6 : 5].Id;
        FLD(pSeq, 0x50, LONG) = 0;
    }
}

void Snd_seq_fade_in(SND_SEQ *pSeq)
{
    if (pSeq->Unk2C != 0) {
        if (pSeq->Unk3C >= 0x3FFF - pSeq->Unk40) {
            pSeq->Unk3C = 0x3FFF;
            pSeq->Unk38 = 0;
        } else {
            pSeq->Unk3C += pSeq->Unk40;
        }
    }
}

void Snd_seq_fade_out(SND_SEQ *pSeq)
{
    if (pSeq->Unk2C != 0) {
        if (pSeq->Unk3C <= pSeq->Unk40) {
            pSeq->Unk3C = 0;
            pSeq->Unk38 = 0;
        pSeq->Unk2D = 1;
            pSeq->Unk2D = 1;
        } else {
            pSeq->Unk3C -= pSeq->Unk40;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800749A0);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80074CD0);

LONG func_80074F44(UCHAR *pD, LONG v, SND_REQ *pReq)
{
    FC_SNDW *pW = &FC_SW;
    UCHAR *pV;
    LONG *pC;
    LONG dx, dy, dz, d;

    pV = (UCHAR *)&((SND_WORK *)pW)->Voice[v];
    if ((UCHAR)(pW->Vprio[v] - 1) < 2) {
        if (pV[0x1A] >= (pD[1] & 7)) {
            if (pV[0x1A] != (pD[1] & 7)) {
                return -1;
            }
            if (pD[1] & 8) {
                return -1;
            }
        }
    }
    if (pReq->Unk1E) {
        pW->Cam_x = ((FC_ROOM2 *)G.pRoom)->pCam[G.Cut_no * 8 + 1];
        pW->Cam_y = ((FC_ROOM2 *)G.pRoom)->pCam[G.Cut_no * 8 + 2];
        pW->Cam_z = ((FC_ROOM2 *)G.pRoom)->pCam[G.Cut_no * 8 + 3];
        dx = (pW->Cam_x - pReq->Pos.vx) >> 1;
        dz = (pW->Cam_z - pReq->Pos.vz) >> 1;
        dy = (pW->Cam_y - pReq->Pos.vy) >> 1;
        d = SquareRoot0(FC_SQ(SquareRoot0(dx * dx + dz * dz)) + dy * dy) * 2;
        if (d > 0xEA5F) {
            d = 0xEA5F;
        }
        pW->Dist[v] = d;
    }
    return 0;
}

LONG Snd_bank_search(LONG id)
{
    LONG i;

    for (i = 0; i < 8; i++) {
        if (Snd_work.Bank_id[i].Id == id) {
            return i;
        }
    }
    return -1;
}

void Snd_set_voice_vol(SHORT l, SHORT r, ULONG voice)
{
    SpuVoiceAttr attr;

    attr.volume.left = l;
    attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR;
    attr.voice = voice;
    attr.volume.right = r;
    SpuSetVoiceAttr(&attr);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80075154);

LONG func_8007592C(LONG x0, LONG z0, LONG x1, LONG z1)
{
    LONG dx = x1 - x0;
    LONG dz = z1 - z0;
    LONG a;

    if (dx != 0) {
        a = catan((dz << 12) / dx);
        if (dx < 0) {
            return (0x800 - a) & 0xFFF;
        }
        return (0x1000 - a) & 0xFFF;
    }
    if (dz <= 0) {
        return 0x400;
    }
    return 0xC00;
}

UCHAR *Snd_bank_get_tone(LONG no, SHORT idx)
{
    ULONG *pHead;
    USHORT *pTbl;
    ULONG n;

    pHead = Snd_work.Bank[no].pHead;
    n = (pHead[2] >> 1) - 1;
    pTbl = (USHORT *)pHead + (n + idx);
    return Snd_work.Bank_id[no].pData + *pTbl * 8;
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800759D8);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80075A80);

LONG func_80075AE4(UCHAR *p, SND_REQ *pT)
{
    VabHdr *pVh = (VabHdr *)Snd_work.Bank[*((UCHAR *)pT + 0x1B)].pHead;
    ULONG i = *((UCHAR *)pT + 0x17);

    return pVh->mvol * ((ProgAtr *)(pVh + 1))[i].mvol / 127 * p[2] / 127 * Snd_work.Unk7C8 / 127;
}

void Snd_pan_vol(SpuVoiceAttr *pAttr, SHORT vol, ULONG pan)
{
    pAttr->volume.right = vol;
    pAttr->volume.left = vol;
    if (pan < 0x40) {
        pAttr->volume.left = vol * (SHORT)pan / 63;
    }
    if (pan > 0x40) {
        pAttr->volume.right = vol * (SHORT)(0x7F - pan) / 63;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80075C30);

void func_80076114(UCHAR *pSeq, UCHAR *pTr)
{
    FC_SNDW *pW;
    VabHdr *pVh;
    LONG vel;
    VagAtr *pVag;
    ProgAtr *pPa;

    pTr[0xC] = **(UCHAR **)(pSeq + 4);
    (*(UCHAR **)(pSeq + 4))++;
    vel = **(UCHAR **)(pSeq + 4);
    (*(UCHAR **)(pSeq + 4))++;
    pW = &FC_SW;
    pVh = (VabHdr *)((SND_WORK *)pW)->Bank[pTr[0xF]].pHead;
    pTr[0xB] = pW->Vol[3] * pVh->mvol / 127;
    pTr[0xB] = pTr[0xB] * pTr[0xA] / 127;
    pTr[0xB] = pTr[0xB] * pTr[7] / 127;
    pTr[0xB] = pTr[0xB] * pW->Vol[0] / 127;
    pTr[0xB] = pTr[0xB] * vel / 127;
    pVag = (VagAtr *)((SND_WORK *)pW)->Bank[pTr[0xF]].Unk4 + pTr[6] * *(LONG *)((UCHAR *)pVh + 0xC);
    pPa = (ProgAtr *)pVh + pTr[6] + 2;
    func_800762C8(pSeq, pTr, Snd_tone_mask(pVag, (SND_TRACK *)pTr, pPa->tones));
    *(LONG *)(pSeq + 0xC) = Snd_seq_get_vlq((UCHAR **)(pSeq + 4));
}

void func_800762C8(UCHAR *pSeq, UCHAR *pTr, ULONG mask)
{
    UCHAR v;
    ULONG b;

    pTr[0xD] = 0;
    while (mask != 0) {
        if (mask & 1) {
            v = func_80077220_u(FLD(pSeq, 0x26, SHORT), pTr[6], pTr[0xD], pTr[0xC]);
            if (v != 0xFF) {
                b = 1 << v;
                Snd_work.UnkB2C |= b;
                Snd_work.UnkB34 |= b;
                FLD(pTr, 0, ULONG) &= ~b;
            }
        }
        mask >>= 1;
        pTr[0xD]++;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007638C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007688C);

void func_80076BAC(UCHAR *pSeq, UCHAR *pTr)
{
    VabHdr *pVh;
    ProgAtr *pPa;
    LONG pan;

    pVh = (VabHdr *)Snd_work.Bank[pTr[0xF]].pHead;
    pTr[6] = **(UCHAR **)(pSeq + 4);
    pPa = (ProgAtr *)pVh + pTr[6] + 2;
    pTr[7] = pVh->mvol * pPa->mvol / 127;
    pan = pVh->pan + 0xC0;
    pPa = (ProgAtr *)pVh + pTr[6] + 2;
    pTr[9] = pPa->mpan + pan;
    (*(UCHAR **)(pSeq + 4))++;
    *(LONG *)(pSeq + 0xC) = Snd_seq_get_vlq((UCHAR **)(pSeq + 4));
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80076C64);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80076E7C);

void func_80077030(LONG *p)
{
    p[3] = 1;
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", Snd_seq_get_vlq);

ULONG Snd_tone_mask(VagAtr *pVag, SND_TRACK *pTr, UCHAR tones)
{
    ULONG i;
    ULONG mask;

    mask = 0;
    for (i = 0; i < tones; i++, pVag++) {
        if (pTr->Note >= pVag->min && pVag->max >= pTr->Note) {
            mask |= 1 << i;
        }
    }
    return mask;
}

signed char func_800770DC(LONG lo, ULONG hi)
{
    UCHAR pad[0x40];
    char st[24];
    signed char i;
    SND_VOICE *pV;
    ULONG best;
    signed char bi;

    SpuRGetAllKeysStatus(lo, hi - 1, st);
    pV = &Snd_work.Voice[lo];
    for (i = lo; i < hi; i++, pV++) {
        if (st[i] == 0 && pV->Unk1D == 0) {
            return i;
        }
    }
    pV = &Snd_work.Voice[lo];
    best = 1;
    bi = -1;
    for (i = lo; i < hi; i++, pV++) {
        if (st[i] == 3) {
            return i;
        }
        if ((ULONG)pV->Unk4 >= best) {
            best = pV->Unk4;
            bi = i;
        }
    }
    return bi;
}

LONG func_80077220(LONG a0, LONG a1, LONG a2, LONG a3)
{
    ULONG i;
    SND_VOICE *p;

    for (i = 8, p = &Snd_work.Voice[8]; i < 24; i++, p++) {
        if (p->Unk17 == a1 && p->Unk18 == a2 && p->Unk19 == a3 && p->Unk1D != 0 && a0 == p->Unk1C) {
            p->Unk1D = 0;
            return i;
        }
    }
    return 0xFF;
}

void func_800772A0(LONG no, UCHAR *pData, LONG a2, LONG a3)
{
    FC_SNDW4 *pW = (FC_SNDW4 *)&Snd_work;
    FC_SEQ2 *p = &pW->Seq[no];
    FC_TRK *pT;
    FC_VOICE *pV;
    ULONG i;
    ULONG t;

    p->pPtr = p->pLoop = pData + 0xC;
    p->Unk18 = a3;
    p->Unk20 = a2;
    p->Unk24 = a2;
    t = *(ULONG *)(pData + 4);
    p->Reso = *(USHORT *)(pData + 8);
    p->Delta = 0;
    p->Tempo2 = p->Tempo = t * 1000 / p->Reso;
    pT = p->Tr;
    pV = &pW->Voice[8];
    for (i = 0; i < 16; i++) {
        pT->Vol = 0x7F;
        pT->Pan = 0x40;
        pT->Pitch = 0x2000;
        pT->Mask = 0;
        pT->UnkF = a2;
        pT->UnkE = a2;
        if (pV->Unk1D == 0) {
            pV->Unk14 = 0x2000;
        }
        pT++;
        pV++;
    }
    p->Fade = 0;
    p->Fade_step = 0;
    p->Fade_vol = 0;
    pW->UnkB74 |= 1 << p->No;
    pW->UnkB76 &= ~(1 << no);
    p->Play = 1;
    p->Stop = 0;
}

void Snd_seq_stop(SHORT no)
{
    if (Snd_work.Seq[no].Unk2C != 0) {
        Snd_work.Seq[no].Unk2D = 1;
        Snd_work.Seq[no].Unk38 = 0;
    }
}

void Snd_seq_stop_all(void)
{
    SHORT i;

    for (i = 0; i < 3; i++) {
        Snd_seq_stop(i);
    }
}

void func_8007745C(LONG no)
{
    ULONG mask;
    UCHAR i, j;
    ULONG v;
    FC_SNDW2 *pW = (FC_SNDW2 *)&Snd_work;

    mask = 0;
    if (Snd_work.Seq[no].Unk2C != 0) {
        Snd_work.UnkB76 |= 1 << no;
        Snd_work.Seq[no].Unk2C = 0;
        for (i = 0; i < 16; i++) {
            if (pW->Seq[no].Tr[i].Mask != 0) {
                j = 8;
                v = pW->Seq[no].Tr[i].Mask;
                for (; j < 24; j++) {
                    if (v & (1 << j)) {
                        mask |= 1 << j;
                    }
                }
            }
        }
        Snd_set_voice_vol(0, 0, mask);
        SpuSetKey(0, mask);
    }
}

void Snd_seq_play_ck(LONG no)
{
    if ((Snd_work.UnkB76 >> no) & 1) {
        Snd_work.Seq[no].Unk2C = 1;
    }
}

void func_800775A0(ULONG no, ENEMY_WORK *pEm)
{
    VECTOR v;

    v.vx = pEm->Pos_x;
    v.vy = pEm->Pos_y;
    v.vz = pEm->Pos_z;
    if (*((UCHAR *)pEm + 0x12F) == D_800BA6E8[3]) {
        no += 0x10;
    }
    func_800746C0(no | 0x10300, &v, 0, 0);
}

void func_80077618(LONG no, UCHAR parts, ENEMY_WORK *pEm)
{
    VECTOR v;
    PARTS_W *p = &pEm->pSin_parts_ptr[parts];

    v.vx = p->Workm.t[0];
    v.vy = p->Workm.t[1];
    v.vz = p->Workm.t[2];
    if (*((UCHAR *)pEm + 0x12F) == D_800BA6E8[3]) {
        no += 0x10;
    }
    func_800746C0(no | 0x10300, &v, 0, 0);
}

void func_800776B0(LONG kind, ULONG parts, ENEMY_WORK *pEm)
{
    LONG one;
    VECTOR v;
    SVECTOR s;
    SVECTOR t;
    PARTS_W *pP;
    LONG se;


    pP = &pEm->pSin_parts_ptr[parts];
    se = MY_func_80077B84(pP->Workm.t[0], pP->Workm.t[2], FLD(pEm, 9, signed char)) & 0x7F;
    if (FLD(pEm, 0xA, SHORT) != 0) {
        se = 0x17;
        if (FLD(pEm, 0xA, SHORT) == 1) {
            t.vx = 0x400;
            s.vx = 0;
            s.vy = 0;
            s.vz = 0;
            t.vy = 0;
            t.vz = 0;
            MY_func_8001B484(0xE, (USHORT)G.Pl_cdir_y | 0x10000000, &pP->Workm, &s, &t);
        } else {
            pP--;
            s.vy = FLD(pEm, 0xA, SHORT) - pP->Workm.t[1];
            se = 0x1A;
            if (parts < 12) {
                s.vz = -0xA0;
            } else {
                s.vz = 0xA0;
            }
            if (pEm->Id == 0xF) {
                if (s.vy < 500) {
                    one = 1;
                    if (kind == 0) {
                        Pad_vib_fade_set(one, 0x82, 0x64, 0);
                        Pad_vib_fade_set(9, 0x55, 0xA, one);
                    }
                    if (kind == one) {
                        Pad_vib_fade_set(3, 0x32, 0x64, 0);
                        Pad_vib_fade_set(5, 0x55, 0, 3);
                    }
                }
                s.vy += 100;
                if (kind == 0) {
                    s.vx = -100;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x800) & 0xFFFF) | 0xE000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y + 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                }
                if (kind == 1) {
                    s.vx = -300;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x800) & 0xFFFF) | 0x15000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x400) & 0xFFFF) | 0x10000000, &pP->Workm, &s, 0);
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y + 0x400) & 0xFFFF) | 0x10000000, &pP->Workm, &s, 0);
                }
            } else {
                if (s.vy < 1200 && pEm->Id < 0x10) {
                    if (kind == 0) {
                        Pad_vib_fade_set(1, 0x82, 0x64, 0);
                        Pad_vib_fade_set(9, 0x55, 0xA, 1);
                    }
                    if (kind == 1) {
                        Pad_vib_fade_set(3, 0x32, 0x64, 0);
                        Pad_vib_fade_set(5, 0x55, 0xA, 3);
                    }
                }
                if (kind == 0) {
                    s.vx = -200;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x800) & 0xFFFF) | 0xE000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y + 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                }
                if (kind == 1) {
                    s.vx = -600;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x800) & 0xFFFF) | 0x17000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y - 0x400) & 0xFFFF) | 0x14000000, &pP->Workm, &s, 0);
                    MY_func_8001B484(0x61C, (((USHORT)G.Pl_cdir_y + 0x400) & 0xFFFF) | 0x14000000, &pP->Workm, &s, 0);
                }
            }
        }
    }
    kind = se - -kind;
    if (EM_P(pEm, 0x1CA, USHORT)) {
        kind = 0x2D;
    }
    v.vx = pP->Workm.t[0];
    v.vy = pP->Workm.t[1];
    v.vz = pP->Workm.t[2];
    if (pEm == G.pUnk265C) {
        func_800746C0(kind | 0x10200, &v, 0, 0);
    } else {
        func_800746C0(kind | 0x10200, &v, -10, -2);
    }
}

UCHAR func_80077B84(LONG x, LONG z, LONG id)
{
    LONG pos[3]; UCHAR *pe; LONG n;
    LONG f;
    pe = (UCHAR *)((FC_ROOM *)G.pRoom)->pSnd_area; n = *(SHORT *)pe; pe += 2; pos[0] = x; pos[2] = z; pe += n * 0xC;
    while (n--) {
        pe -= 0xC;
        f = ((SHORT *)pe)[5];
        if (!(f & 0x100) && id == (f & 0x7F) && func_800101C8(pos, (SHORT *)pe)) {
            return pe[8];
        }
    }
    return 0x1D;
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80077C4C);

void func_80077ED4(void)
{
    SND_WORK *p = &Snd_work;
    UCHAR i = 0;
    UCHAR n;

    do {
        n = p->Seq[0].Unk38 + p->Seq[1].Unk38 + p->Seq[2].Unk38 + p->UnkB1C;
        if ((signed char)i++ < 0) {
            break;
        }
        Task_sleep(1);
    } while (n);
    func_800746C0(0xE, 0, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80077F60);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007809C);

void func_80078204(void)
{
    LONG *p;
    SND_WORK *pW;

    p = ((FC_ROOM *)G.pRoom)->pSnd;
    pW = &Snd_work;
    if (p == NULL) {
        pW->Bank_id[2].Id = -1;
        return;
    }
    *(LONG **)&D_800BA6E8 = p;
    FLD(pW, 0xB78, UCHAR) = ((FC_ROOM *)G.pRoom)->Unk6 * 4;
    while (*p != -1) {
        p++;
    }
    p++;
    func_8007836C(2, (ULONG *)p);
    if (((FC_ROOM *)G.pRoom)->pSnd != NULL) {
        ((FC_G *)&G)->Unk213C = (UCHAR *)(((ULONG)pW->Bank[2].pHead + pW->Bank[2].pHead[1] + 3) & ~3);
    }
    func_800783BC(2, p[0x30], 0);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800782F4);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007836C);

void func_800783BC(ULONG no, ULONG size)
{
    SND_WORK *pW = &Snd_work;
    SND_WORK *p = (SND_WORK *)((UCHAR *)pW + no * 8);

    size = (size + 0x3F) & ~0x3F;
    p->Bank_id[0].pad2 = size >> 6;
    p->Bank_id[0].Id = no;
    if (no < 5) {
        if (no != 3) {
            p->Bank_id[0].pData = (UCHAR *)D_800A0FEC[no];
        } else {
            pW->Bank_id[3].pData = (UCHAR *)(0x3DE10 - size);
        }
    } else if (no == 5) {
        pW->Bank_id[5].pData = (UCHAR *)D_800A1000;
    } else {
        p->Bank_id[0].pData = (UCHAR *)(0x7B7C0 - size);
    }
}

void func_80078450(void)
{
    if (D_800CCBBF) {
        func_8007836C(3, (ULONG *)0x801FB600);
    } else {
        D_800E067C = -1;
    }
}

void func_80078498(UCHAR a, UCHAR b, SHORT c)
{
    SHORT i = a;

    if (i >= 5) {
        i -= 5;
    }
    GX.Unk7C84[D_800A0FDC[i] + b] = c;
}

void func_800784E0(ULONG *pAdr)
{
    TIM_IMAGE tim;
    TIM_IMAGE *p;
    LONG x;
    RECT *r;

    OpenTIM(pAdr);
    p = (TIM_IMAGE *)ReadTIM((u_long *)&tim);
    r = p->prect;
    x = ((FC_G2 *)&G)->Tp_x << 6;
    if (((FC_G2 *)&G)->Tp_x >= 16) {
        x -= 0x400;
    }
    r->x = x;
    p->prect->y = (((FC_G2 *)&G)->Tp_x >= 16) << 8;
    LoadImage(p->prect, p->paddr);
    ((FC_G2 *)&G)->Tp_x += (p->prect->w + 0x3F) / 64;
    if (p->caddr) {
        p->crect->y = ((FC_G2 *)&G)->Clut_y + 0x1E0;
        LoadImage(p->crect, p->caddr);
        ((FC_G2 *)&G)->Clut_y += p->crect->h;
    }
    DrawSync(0);
}

void SetSpadStack(void)
{
    G.System_flg |= SYS_SPAD_STACK;
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\tmove $29, %1" : : "r"(&pStack_bak), "r"(0x1f8003fc) : "$8");
}

void ResetSpadStack(void)
{
    G.System_flg &= ~SYS_SPAD_STACK;
    __asm__ volatile("move $29, %0" : : "r"(pStack_bak));
}

void Vram_clr(USHORT x, USHORT y, USHORT w, USHORT h)
{
    RECT Rect;

    Rect.x = x;
    Rect.y = y;
    Rect.w = w;
    Rect.h = h;
    ClearImage(&Rect, 0, 0, 0);
}

void Rot_vector(SHORT dir, SVECTOR *p, SVECTOR *q)
{
    MATRIX *pMat;

    Rot_vector_Svec.vy = dir;
    pMat = &Tmp_matrix[0];
    RotMatrix(&Rot_vector_Svec, pMat);
    ApplyMatrixSV(pMat, p, q);
}

void Rot_vector_super(SHORT dir, MATRIX *pM, SVECTOR *p, SVECTOR *q)
{
    MATRIX *pMat;

    Rot_vector_super_Svec.vy = dir;
    pMat = &Tmp_matrix[1];
    RotMatrix(&Rot_vector_super_Svec, pMat);
    MulMatrix2(pM, pMat);
    ApplyMatrixSV(pMat, p, q);
}

void Rot_add_matrix(LONG *pPos, SHORT *pDir, MATRIX *pM, SVECTOR *Add_dir)
{
    MATRIX Mat;
    MATRIX Mat2;
    SVECTOR Svec;
    VECTOR Vec;
    LONG ang;
    int m02;
    int m12;

    int ySq;
    int x;
    int y;

    Mat.t[0] = pPos[0];
    Mat.t[1] = pPos[1];
    Mat.t[2] = pPos[2];
    RotMatrix(Add_dir, &Mat);
    MulMatrix2(&Mat, pM);
    MatrixNormal(pM, pM);

    m02 = pM->m[0][2];
    m12 = pM->m[1][2];
    x = pM->m[2][2];
    y = m02 * m02;
    y += m12 * m12;
    y += x * x;
    y = (m02 << 0xc) / (y / 4096);

    ang = ratan2(-m12, x);
    *pDir = ang;

    ySq = y * y;
    if (ySq < 0) {
        ySq += 0xfff;
    }
    ang = SquareRoot12(0x1000 - (ySq >> 0xc));
    ang = ratan2(y, ang);
    pDir[1] = ang;
    pDir[2] = 0;

    Mat.t[0] = 0;
    Mat.t[1] = 0;
    Mat.t[2] = 0;
    RotMatrix((SVECTOR *)pDir, &Mat);
    TransposeMatrix(&Mat, &Mat2);

    Svec.vx = pM->m[0][0];
    Svec.vy = pM->m[1][0];
    Svec.vz = pM->m[2][0];
    ApplyMatrix(&Mat2, &Svec, &Vec);
    ang = ratan2(Vec.vy, Vec.vx);
    pDir[2] = ang;
}

void Flg_on(ULONG *p, ULONG x)
{
    p[x >> 5] |= 0x80000000U >> (x & 0x1f);
}

void Flg_off(ULONG *p, ULONG x)
{
    p[x >> 5] &= ~(0x80000000U >> (x & 0x1f));
}

ULONG Flg_ck(ULONG *p, ULONG x)
{
    return p[x >> 5] & (0x80000000U >> (x & 0x1f));
}

void Set_view(LONG *pVp, LONG *pVr)
{
    MATRIX Mat;
    LONG F1;
    LONG F2;
    LONG Dx;
    LONG Dy;
    LONG Dz;
    VECTOR V;

    GsWSMATRIX = GsIDMATRIX;

    Dx = pVr[0] - pVp[0];
    Dy = pVr[1] - pVp[1];
    Dz = pVr[2] - pVp[2];

    F1 = SquareRoot0(Dx * Dx + Dy * Dy + Dz * Dz);
    Dy = (Dy << 12) / F1;
    F2 = SquareRoot0(Dx * Dx + Dz * Dz);

    Mat = GsIDMATRIX;
    Mat.m[2][2] = Mat.m[1][1] = (F2 << 12) / F1;
    Mat.m[1][2] = -Dy;
    Mat.m[2][1] = Dy;
    MulMatrix(&GsWSMATRIX, &Mat);

    if (F2 != 0) {
        Mat = GsIDMATRIX;
        Mat.m[2][2] = Mat.m[0][0] = (Dz << 12) / F2;
        Dx = (Dx << 12) / F2;
        Mat.m[0][2] = -Dx;
        Mat.m[2][0] = Dx;
        MulMatrix(&GsWSMATRIX, &Mat);
    }

    V.vx = -pVp[0];
    V.vy = -pVp[1];
    V.vz = -pVp[2];
    ApplyMatrixLV(&GsWSMATRIX, &V, (VECTOR *)&GsWSMATRIX.t);
}

void Set_light_pos(VECTOR *pV)
{
    Set_light(Get_light_cut((LONG *)pV), pV);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", Set_light_default);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", Set_light);

void Set_Light_data(ULONG No, VECTOR *pLig, UCHAR *pRgb, LIGHT_CUT_DATA *pLight)
{
    VECTOR normVec;
    SHORT *pLm;
    SHORT *pCm;

    pLm = &LIGHTMATRIX.m[No][0];
    pCm = &COLORMATRIX.m[0][No];
    VectorNormal(pLig, &normVec);
    pLm[0] = normVec.vx * -pLight->Mag;
    pLm[1] = normVec.vy * -pLight->Mag;
    pLm[2] = normVec.vz * -pLight->Mag;
    pCm[0] = *pRgb << 4;
    pCm[3] = pRgb[1] << 4;
    pCm[6] = pRgb[2] << 4;
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", Get_light_cut);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80078FE8);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007906C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800790F0);

void func_800791F0(void)
{
    func_80078FE8();
    func_800100A4((UCHAR *)&G + 0x75F0, D_800C7048.Save, 0x908);
    FC_G3X.Unk24D6 = D_800C7048.UnkA;
    G.Pl_pos_x = D_800C7048.Pos_x;
    G.Pl_pos_y = D_800C7048.Pos_y;
    G.Pl_pos_z = D_800C7048.Pos_z;
    G.Pl_cdir_y = D_800C7048.Cdir_y;
    FC_G3X.Life = D_800C7048.Life;
    if (D_800C7048.UnkB & 0x80) {
        G.Pl_poison_timer = D_800C7048.UnkB;
        FC_G3X.Unk255E |= 0x200;
    } else {
        G.Pl_poison_timer = 0;
        FC_G3X.Unk255E &= ~0x200;
    }
    func_8007906C();
    G.Next_cut_no = G.Cut_no;
    func_800792FC();
    func_800790F0((ULONG *)0x80100000);
    G.System_flg |= 0x4000;
    Task_lexecute(1, (void *)2);
    Task_sleep(1);
    G.System_flg &= ~0x4000;
    G.System_flg &= ~0x40000;
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_800792FC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_80079374);

void Copy_rot_matrix(MATRIX *m0, MATRIX *m1)
{
    LONG r0, r1, r2, r3, r4;

    r0 = ((LONG *)m0)[0];
    r1 = ((LONG *)m0)[1];
    r2 = ((LONG *)m0)[2];
    r3 = ((LONG *)m0)[3];
    r4 = ((LONG *)m0)[4];
    ((LONG *)m1)[0] = r0;
    ((LONG *)m1)[1] = r1;
    ((LONG *)m1)[2] = r2;
    ((LONG *)m1)[3] = r3;
    ((LONG *)m1)[4] = r4;
}

void Copy_matrix(MATRIX *m0, MATRIX *m1)
{
    LONG r0, r1, r2, r3, r4, r5, r6, r7;

    r0 = ((LONG *)m0)[0];
    r1 = ((LONG *)m0)[1];
    r2 = ((LONG *)m0)[2];
    r3 = ((LONG *)m0)[3];
    r4 = ((LONG *)m0)[4];
    r5 = ((LONG *)m0)[5];
    r6 = ((LONG *)m0)[6];
    r7 = ((LONG *)m0)[7];
    ((LONG *)m1)[0] = r0;
    ((LONG *)m1)[1] = r1;
    ((LONG *)m1)[2] = r2;
    ((LONG *)m1)[3] = r3;
    ((LONG *)m1)[4] = r4;
    ((LONG *)m1)[5] = r5;
    ((LONG *)m1)[6] = r6;
    ((LONG *)m1)[7] = r7;
}

LONG func_80079690(ENEMY_WORK *pEm, UCHAR *pKan, UCHAR *pSeq, ULONG Flg)
{
    UCHAR pad[0x50];
    UCHAR *pS;
    ULONG w;
    USHORT base;
    ULONG n;
    UCHAR *pF;
    PARTS_W *pP;
    ULONG w2;
    MATRIX *pM;

    pS = (UCHAR *)(EM_P(pEm, 0x1A0, UCHAR) * 8 + (ULONG)pSeq);
    n = *(ULONG *)pS;
    base = *(USHORT *)(pS + 4);
    pSeq += (n >> 17) * 2;
    pM = D_800E1200;
    n &= 0xFFFF;
    if (Flg & 0xFFFF0000) {
        pSeq += (n - EM_P(pEm, 0x1A1, UCHAR)) * 2 - 2;
    } else {
        pSeq += EM_P(pEm, 0x1A1, UCHAR) * 2;
    }
    pKan += ((USHORT *)pKan)[1] & 0xFFFC;
    pKan += (*pSeq + (SHORT)base) * 32;
    pF = pKan;
    EM_P(pEm, 0x1A4, LONG) = FLD(pF, 0x14, LONG);
    EM_P(pEm, 0x1A8, USHORT) = FLD(pF, 0x18, USHORT);
    EM_P(pEm, 0x1AC, ULONG) = FLD(pF, 0x1A, USHORT) | (FLD(pF, 0x1C, ULONG) << 16);
    EM_P(pEm, 0x1B0, USHORT) = FLD(pF, 0x1E, USHORT);
    pEm->pSin_parts_ptr[9].Cdir_y = FLD(pF, 8, USHORT) & 0xFFF;
    pEm->pSin_parts_ptr[12].Cdir_y = FLD(pF, 0xE, USHORT) & 0xFFF;
    pP = &pEm->pSin_parts_ptr[11];
    w2 = FLD(pF, 8, ULONG);
    *(ULONG *)&pP->Cdir_x = ((w2 >> 12) & 0xFFF) | (((w2 >> 24) | ((FLD(pF, 0xC, USHORT) & 0xF) << 8)) << 16);
    pP->Cdir_z = (FLD(pF, 0xC, ULONG) & 0xFFF0) >> 4;
    RotMatrixZYX_gte((SVECTOR *)&pP->Cdir_x, &pM[2]);
    pP = &pEm->pSin_parts_ptr[14];
    *(ULONG *)&pP->Cdir_x = (FLD(pF, 0xC, ULONG) >> 28) | (FLD(pF, 0x10, UCHAR) << 4) | (((FLD(pF, 0x10, ULONG) >> 8) & 0xFFF) << 16);
    pP->Cdir_z = FLD(pF, 0x10, ULONG) >> 20;
    RotMatrixZYX_gte((SVECTOR *)&pP->Cdir_x, &pM[5]);
    EM_P(pEm, 0x1A1, UCHAR)++;
    if (EM_P(pEm, 0x1A1, UCHAR) >= n) {
        EM_P(pEm, 0x1A1, UCHAR) = 0;
        return 1;
    }
    return 0;
}

void func_80079868(ENEMY_WORK *pEm, LONG rate)
{
    MATRIX m0;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    MATRIX m4;
    MATRIX m5;
    MY_JNT j;
    VECTOR v;
    UCHAR cnt;
    MATRIX *pW;
    PARTS_W *pP;
    PARTS_W *pP0;
    MATRIX *pM;
    SHORT r;
    LONG w;
    ULONG n;

    pW = (MATRIX *)((UCHAR *)pEm + 0x20);
    pP0 = pEm->pSin_parts_ptr;
    cnt = EM_P(pEm, 0x1A2, UCHAR);
    pM = D_800E1200;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, pW);
    func_8002D4B0(pW, (MATRIX *)pP0->M, &m4);
    func_8002D4B0(&m4, (MATRIX *)pP0[8].M, &pP0[8].Workm);
    pP0 = &pEm->pSin_parts_ptr[8];
    Copy_rot_matrix(&pP0->Workm, &m0);
    m0.t[0] = (pP0->Workm.t[0] - pEm->Pos_x) << 3;
    m0.t[1] = (pP0->Workm.t[1] - pEm->Pos_y) << 3;
    m0.t[2] = (pP0->Workm.t[2] - pEm->Pos_z) << 3;
    j = *EM_P(pEm, 0x1B4, MY_JNT *);
    pP = pEm->pSin_parts_ptr;
    RotMatrix_gte((SVECTOR *)&pP[9].Cdir_x, pM);
    RotMatrix_gte((SVECTOR *)&pP[12].Cdir_x, &D_800E1260);
    Copy_rot_matrix(pW, &m3);
    Copy_rot_matrix(pM, &m4);
    Copy_rot_matrix(&D_800E1260, &m5);
    Copy_rot_matrix(&GsIDMATRIX, &D_800E1220);
    Copy_rot_matrix(&GsIDMATRIX, &D_800E1280);
    m4.t[0] = j.a.vx;
    m4.t[1] = j.a.vy;
    m4.t[2] = j.a.vz;
    m5.t[0] = j.b.vx;
    m5.t[1] = j.b.vy;
    m5.t[2] = j.b.vz;
    func_8002D4B0(&m0, &m5, &m2);
    func_8002D4B0(&m0, &m4, &m1);
    if (pEm->UnkAE & 0x40) {
        func_80079CC0(pEm, pEm->pSin_parts_ptr->Pos_y);
    }
    ApplyMatrix(&m3, (SVECTOR *)((UCHAR *)pEm + 0x1A4), (VECTOR *)m3.t);
    ApplyRotMatrix((SVECTOR *)((UCHAR *)pEm + 0x1AC), &v);
    func_8002AEE8(&m1, &m3, &m4);
    m3.t[0] = v.vx;
    m3.t[1] = v.vy;
    m3.t[2] = v.vz;
    func_8002AEE8(&m2, &m3, &m5);
    func_80079374(pM, &D_800E1220, m4.t, &j);
    func_80079374(&D_800E1260, &D_800E1280, m5.t, &j);
    n = cnt;
    if (n == 0) {
        Copy_rot_matrix(pM, (MATRIX *)pP[9].M);
        Copy_rot_matrix(&D_800E1220, (MATRIX *)pP[10].M);
        Copy_rot_matrix(&pM[2], (MATRIX *)pP[11].M);
        Copy_rot_matrix(&D_800E1260, (MATRIX *)pP[12].M);
        Copy_rot_matrix(&D_800E1280, (MATRIX *)pP[13].M);
        Copy_rot_matrix(&pM[5], (MATRIX *)pP[14].M);
    } else {
        r = (rate & 0xFFFF) * n;
        w = 0x1000 - r;
        func_80026728((MATRIX *)pP[9].M, pM, (MATRIX *)pP[9].M, w);
        func_80026728((MATRIX *)pP[10].M, &D_800E1220, (MATRIX *)pP[10].M, w);
        func_80026728((MATRIX *)pP[11].M, &pM[2], (MATRIX *)pP[11].M, w);
        func_80026728((MATRIX *)pP[12].M, &D_800E1260, (MATRIX *)pP[12].M, w);
        func_80026728((MATRIX *)pP[13].M, &D_800E1280, (MATRIX *)pP[13].M, w);
        func_80026728((MATRIX *)pP[14].M, &pM[5], (MATRIX *)pP[14].M, 0xFFF - r);
        EM_P(pEm, 0x1A2, UCHAR)--;
    }
}

void func_80079C70(UCHAR *p)
{
    UCHAR buf[0xE8];
    UCHAR *q = *(UCHAR **)(p + 0x108);

    MulMatrix0((MATRIX *)(p + 0x20), (MATRIX *)(q + 0x828), (MATRIX *)(q + 0x854));
    MulMatrix0((MATRIX *)(p + 0x20), (MATRIX *)(q + 0xA5C), (MATRIX *)(q + 0xA88));
}

void func_80079CC0(ENEMY_WORK *pEm, LONG h)
{
    MATRIX m;
    SVECTOR s0;
    SVECTOR s1;
    VECTOR v;
    LONG d;
    LONG n;
    SHORT y;

    memset(&s1, 0, 8);
    EM_P(pEm, 0x1B8, SHORT) = 0;
    s1.vy = pEm->Cdir_y;
    RotMatrix(&s1, &m);
    ApplyMatrixSV(&m, (SVECTOR *)((UCHAR *)pEm + 0x1A4), &s0);
    ApplyMatrixSV(&m, (SVECTOR *)((UCHAR *)pEm + 0x1AC), &s1);
    v.vx = pEm->Pos_x + ((SHORT)s0.vx >> 3);
    v.vz = pEm->Pos_z + ((SHORT)s0.vz >> 3);
    v.vy = pEm->Pos_y;
    d = func_8004D720(&v, 0x320, 0x8000, 0x69) - EM_P(pEm, 0x122, SHORT);
    n = 0;
    if (h < d) {
        y = EM_P(pEm, 0x1A6, SHORT);
        d <<= 3;
        if (d < y) {
            EM_P(pEm, 0x1A6, SHORT) = d;
        } else {
            n = 1;
            EM_P(pEm, 0x1B8, SHORT) += (d - y) >> 6;
        }
    }
    v.vx = pEm->Pos_x + ((SHORT)s1.vx >> 3);
    v.vz = pEm->Pos_z + ((SHORT)s1.vz >> 3);
    v.vy = pEm->Pos_y;
    d = func_8004D720(&v, 0x320, 0x8000, 0x69) - EM_P(pEm, 0x122, SHORT);
    if (h < d) {
        y = EM_P(pEm, 0x1AE, SHORT);
        d <<= 3;
        if (d < y) {
            if (pEm != G.pUnk265C && pEm->Type != 1) {
                EM_P(pEm, 0x1AE, SHORT) = d;
            }
        } else {
            n++;
            EM_P(pEm, 0x1B8, SHORT) += (d - y) >> 6;
        }
    }
    if (n != 0) {
        EM_P(pEm, 0x1B8, SHORT) >>= n - 1;
    }
    if (*(SHORT *)&pEm->Routine_0 != 0x501) {
        EM_P(pEm, 0x1B8, SHORT) = 0;
    }
}

void func_80079EB8(void) {
}

void Pcstr_set_buf(ULONG *pBuf, LONG size)
{
    Pcstr.pBuf[0] = pBuf;
    Pcstr.pBuf[1] = pBuf + (size << 8);
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", Pcstr_open);

void Pcstr_set(LONG unused, ULONG start, ULONG end, void (*start_func)(void), void (*end_func)(void))
{
    Pcstr.End_flg = 0;
    Pcstr.Buf_no = 0;
    Pcstr.End_frame = end;
    Pcstr.Start_func = start_func;
    Pcstr.Head[1].Frame_count = 0;
    Pcstr.Head[0].Frame_count = 0;
    Pcstr.Start_frame = start;
    Pcstr.End_func = end_func;
}

void Pcstr_close(void)
{
    PCclose(Pcstr.Fd);
}

LONG Pcstr_next(void)
{
    PCSTR_WORK *p = &Pcstr;

    p->Buf_no ^= 1;
    Pcstr_read_frame(p->pBuf[p->Buf_no]);
    if (p->Start_func != NULL) {
        p->Start_func();
    }
    if ((p->Head[p->Buf_no].Frame_count >= p->End_frame || p->End_flg != 0) && p->End_func != NULL) {
        p->End_func();
    }
    return 0;
}

LONG Pcstr_get(ULONG **ppBuf, PCSTR_HEADER **ppHead)
{
    *ppBuf = Pcstr.pBuf[Pcstr.Buf_no];
    *ppHead = &Pcstr.Head[Pcstr.Buf_no];
    return 0;
}

void Pcstr_read_frame(ULONG *pBuf)
{


    if (Pcstr.End_flg != 0) {
        return;
    }
    do {
        if (Pcstr.Interleave != 0) {
            while (1) {
                if (PCread(Pcstr.Fd, (char *)Pcstr.Sub_head, 8) == 0) {
                    Pcstr.End_flg = 1;
                    return;
                }
                Pcstr.Sub_head[0] &= 0xFF0000;
                Pcstr.Sub_head[1] &= 0xFF0000;
                if (Pcstr.Sub_head[0] == 0x420000 || Pcstr.Sub_head[0] == 0x480000) {
                    break;
                }
                PClseek(Pcstr.Fd, 0x918, 1);
            }
        }
        if (PCread(Pcstr.Fd, (char *)(Pcstr.Head + Pcstr.Buf_no), 0x14) == 0) {
            Pcstr.End_flg = 1;
            return;
        }
        PClseek(Pcstr.Fd, 0xC, 1);
        PCread(Pcstr.Fd, (char *)pBuf, 0x7E0);
        pBuf += 0x7E0 / 4;
        if (Pcstr.Interleave != 0) {
            PClseek(Pcstr.Fd, 0x118, 1);
        }
    } while (Pcstr.Head[Pcstr.Buf_no].Sec_count != Pcstr.Head[Pcstr.Buf_no].Sec_num - 1);
    if (Pcstr.Head[Pcstr.Buf_no].Frame_count >= Pcstr.End_frame) {
        Pcstr.End_flg = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007A2D4);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007A350);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007A490);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007A600);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007A918);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007AC8C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007B2FC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007B518);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007B824);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007BC7C);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007BDF8);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007BFBC);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007C434);

INCLUDE_ASM("asm/nonmatchings/game/800601F0", func_8007C91C);
