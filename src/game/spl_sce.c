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
void Spl_sce05(ENEMY_WORK *pEm);
LONG func_800634F4(ENEMY_WORK *pEm, LONG f);
void func_8001B484(LONG no, ULONG dir, void *p, SVECTOR *pSv, LONG a4);
LONG func_80044804(void *p, LONG a1, LONG a2);
extern LONG D_8009DBD4;
void FA_Add_speedXZ3(ENEMY_WORK *pEm, LONG a, LONG b) __asm__("Add_speedXZ");
void func_80018EC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG f);
LONG func_80063198(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d);
void Spl_sce04(ENEMY_WORK *pEm);
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
void Mapping_tim(ULONG *p);
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
void HMatrix(MATRIX *a, MATRIX *b, MATRIX *c, LONG r);
void MulLMatrix(MATRIX *a, MATRIX *b, MATRIX *c);
void func_80079374(MATRIX *a, MATRIX *b, LONG *t, MY_JNT *j);
void func_80079CC0(ENEMY_WORK *pEm, LONG h);
void MY_func_800611E0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("Spl_sce07");
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
LONG Joint_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
LONG func_80079690(ENEMY_WORK *pEm, UCHAR *pKan, UCHAR *pSeq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, LONG rate);
extern void (*D_8009EEA4[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern UCHAR D_8009F568[];
extern char D_80010F84[];
void Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
extern USHORT D_8009F4F8[][2];
extern LONG D_800D23B8;
extern ST_WORK St;
extern UCHAR *D_800BA6E8;
void func_80073CCC(UCHAR *a, UCHAR *b);
extern SpuCommonAttr D_800E10F8;
LONG Joint_move2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
extern UCHAR D_800DBB58;
extern SHORT D_800D1F80;
extern ULONG *D_800E01C8;
extern USHORT D_800A004C[][2];
void Snd_se_stad(ULONG code, VECTOR *pPos, SHORT a2, SHORT a3);
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
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);
void Foot_set_pl(ENEMY_WORK *pEm, LONG a1, LONG a2);
void func_80060464(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_8006037C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
extern void (*D_8009EE44[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Snd_se_walk(LONG kind, ULONG parts, ENEMY_WORK *pEm);
void Spl_sce_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8006055C(ENEMY_WORK *pEm);
void func_8002B2BC(ULONG *pKan, ULONG *pSeq, USHORT *pNow_seq, ULONG R_flg);

void Spl_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        Root_ck(pEm, &V, 0, 0);
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
        Foot_set_pl(pEm, 0, 0);
    }
    if (pEm->Unk144 & 2) {
        Foot_set_pl(pEm, 0, 1);
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
            Snd_se_walk(1, n * 3 + 0xB, pEm);
        }
    }
}

void Spl_sce00(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr);
    func_8006055C(pEm);
}

void Spl_sce01(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    func_8006055C(pEm);
}

void Spl_sce02(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    func_8006055C(pEm);
}

void Spl_sce03(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
    func_8006055C(pEm);
}

INCLUDE_ASM("asm/nonmatchings/game/spl_sce", Spl_sce04);

void func_800608C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->UnkAE & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x20404;
        *(LONG *)&pEm->Move_no = 0x70000;
        Spl_sce04(pEm);
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

void Spl_sce05(ENEMY_WORK *pEm)
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
        Spl_sce05(pEm);
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

void Spl_sce06(ENEMY_WORK *pEm)
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
        Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    }
}

void Spl_sce07(ENEMY_WORK *pEm)
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

void Spl_sce08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

void Spl_sce09(ENEMY_WORK *pEm)
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
        if (Joint_move(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200) != 0) {
            *(USHORT *)&pEm->Routine_2 = pEm->Routine_2 ? 6 : 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x200);
        func_80079868(pEm, 0x200);
        break;
    }
}

void func_80061AD0(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(LONG *)&pEm->Move_no = pEm->Dest_z * 2 + (pEm->Routine_2 != 1 ? 0x7000A : 0x70002);
        EM_P(pEm, 0x1A0, LONG) = (pEm->Dest_z << 1) + (pEm->Routine_2 != 1 ? 0x7000A : 0x70002);
    case 1:
        Joint_move(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x200);
        func_80079868(pEm, 0x200);
        Flg_on(D_800D1FC0, EM_P(pEm, 0x146, UCHAR));
        break;
    }
}

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
        Snd_se_enem(0xF, pEm);
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
        if (Joint_move(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200) != 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x400);
        func_80079868(pEm, 0x200);
        break;
    }
}

signed char func_800102E8(void);
LONG FF_func_8001B484(LONG no, ULONG dir, void *p, SVECTOR *pSv, LONG a4) __asm__("func_8001B484");

void func_80061DF4(ENEMY_WORK *pEm)
{
    UCHAR *pP;
    SVECTOR sv;
    SVECTOR sv2;
    LONG *pR;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_P(pEm, 0x1A0, LONG) = *(LONG *)&pEm->Move_no = 0x30008;
    case 1:
        pP = (UCHAR *)pEm->pSin_parts_ptr;
        if ((UCHAR)(pEm->Move_cnt % 3) == 0) {
            Snd_se_enem(0xF, pEm);
            sv.vx = 0xBE;
            sv.vy = 0x514;
            sv.vz = 0x14;
            func_8001B484(1, 0x12000000, pP + 0x330, &sv, 0);
            sv.vx = 0xBE;
            sv.vy = 0x640;
            sv.vz = 0x14;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = -0x400;
            func_8001B484(0x801, 0x12000000, pP + 0x330, &sv, (LONG)&sv2);
            sv.vx = 0x186;
            sv.vy = 0x58C;
            sv.vz = 0x14;
            sv2.vx = -0x400;
            sv2.vy = 0x600;
            sv2.vz = 0;
            func_8001B484(0x801, 0x0A000000, pP + 0x330, &sv, (LONG)&sv2);
            sv.vx = 0x64;
            sv.vy = 0x58C;
            sv.vz = -0x64;
            sv2.vx = -0x200;
            sv2.vy = 0x200;
            sv2.vz = -0x300;
            func_8001B484(0x801, 0x0A000000, pP + 0x330, &sv, (LONG)&sv2);
            sv.vx = 0x64;
            sv.vy = 0x58C;
            sv.vz = 0x8C;
            sv2.vx = 0x200;
            sv2.vy = -0x200;
            sv2.vz = -0x300;
            func_8001B484(0x801, 0x0A000000, pP + 0x330, &sv, (LONG)&sv2);
            sv.vx = 0xDA;
            sv.vy = 0x185;
            sv.vz = -0x26;
            func_8001B484(0x102, ((USHORT)(pEm->Cdir_y + 0x400)) | 0x8000000, pP + 0x330, &sv, 0);
        }
        if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
            sv.vx = 0xBE;
            sv.vy = 0x514;
            sv.vz = 0x14;
            func_8001B484(0x603, (USHORT)func_800102E8() | 0x10000000, pP + 0x330, &sv, 0);
            sv.vx = 0xDA;
            sv.vy = 0x185;
            sv.vz = -0x26;
            pR = (LONG *)FF_func_8001B484(0x603, (USHORT)func_800102E8() | 0x10000000, pP + 0x330, &sv, 0);
            if (pR != (LONG *)-1) {
                ((SHORT *)pR)[8] = 0x14;
                ((SHORT *)pR)[9] = -0x28;
            }
        }
        if (Joint_move(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200) != 0) {
            if (--pEm->Dest_z == 0) {
                pEm->Routine_3++;
            }
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x400);
        func_80079868(pEm, 0x200);
        break;
    case 2:
        EM_P(pEm, 0x1A0, LONG) = *(LONG *)&pEm->Move_no = 0x30009;
        pEm->Routine_3++;
    case 3:
        pEm->Routine_3 += Joint_move(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, EM_P(pEm, 0x184, ULONG *), EM_P(pEm, 0x180, ULONG *), 0x400);
        func_80079868(pEm, 0x200);
        break;
    case 4:
        *(USHORT *)&pEm->Routine_2 = 6;
        break;
    }
}

void func_800621F0(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(LONG *)&pEm->Move_no = pEm->Routine_2 != 3 ? 0x70007 : 0x70000;
        EM_P(pEm, 0x1A0, LONG) = pEm->Routine_2 != 3 ? 0x70007 : 0x70000;
    case 1:
        if (Joint_move(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x10200) != 0) {
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
    if (Joint_move(pEm, EM_P(pEm, 0x18C, ULONG *), EM_P(pEm, 0x188, ULONG *), 0x200) != 0) {
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

INCLUDE_ASM("asm/nonmatchings/game/spl_sce", func_800623C4);

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
        if (Joint_move2(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
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
                if (Joint_move2(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
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
        Joint_move2(pEm, pKan, pSeq, 0x200);
        ret = 0;
    } else {
        Joint_move2(pEm, pKan, pSeq, 0x200);
        ret = 1;
    }
    return ret;
}

LONG func_80062BFC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d)
{
    LONG x, z;

    Goto00(pEm, EM_P(pEm, 0xD4, SHORT), EM_P(pEm, 0xD6, SHORT), (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * EM_P(pEm, 0x148, SHORT));
    Joint_move2(pEm, pKan, pSeq, (d << 16) | 0x200);
    Add_speedXZ(pEm, d);
    x = pEm->Pos_x - EM_P(pEm, 0xD4, SHORT);
    z = pEm->Pos_z - EM_P(pEm, 0xD6, SHORT);
    return SquareRoot0(x * x + z * z) < 0x69;
}

UCHAR func_80062CD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Goto01(pEm, *(SHORT *)((UCHAR *)pEm + 0xD4), *(SHORT *)((UCHAR *)pEm + 0xD6));
    Joint_move2(pEm, pKan, pSeq, 0x200);
    return pEm->Cdir_y == *(SHORT *)((UCHAR *)pEm + 0xD4);
}

void Spl_sce_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        if (Joint_move2(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
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
                if (Joint_move2(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
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
        if (Joint_move(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
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
                if (Joint_move(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
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
    Joint_move(pEm, EM_P(pEm, 0x19C, ULONG *), EM_P(pEm, 0x198, ULONG *), (d << 16) | 0x200);
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, EM_P(pEm, 0x194, ULONG *), EM_P(pEm, 0x190, ULONG *), d ? 0x10200 : 0x200);
    func_80079868(pEm, 0x200);
    pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x << 2) / 3;
    Add_speedXZ(pEm, 0);
    x = pEm->Pos_x - EM_P(pEm, 0xD4, SHORT);
    z = pEm->Pos_z - EM_P(pEm, 0xD6, SHORT);
    return SquareRoot0(x * x + z * z) < 0x64;
}

INCLUDE_ASM("asm/nonmatchings/game/spl_sce", func_800632B8);

INCLUDE_ASM("asm/nonmatchings/game/spl_sce", func_800634F4);

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
