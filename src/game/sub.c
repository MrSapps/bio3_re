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

/* declarations from earlier parts of the original address-split file */
signed char func_800102E8(void);
LONG FF_func_8001B484(LONG no, ULONG dir, void *p, SVECTOR *pSv, LONG a4) __asm__("func_8001B484");

extern UCHAR D_800DBB5B;
extern UCHAR D_800CEA80[];
LONG func_8001B894();
void Mess_set(LONG a0, LONG a1, LONG a2, LONG a3);

extern ULONG D_800CC840;
void func_8006AC88(UCHAR no);

extern UCHAR D_8009F5F0[];

extern char D_80010FEC[];

extern void (*D_800A02F0[])(ST_WORK *pSt);
void func_8006D948(ST_WORK *pSt);
void func_8006E0A4(ST_WORK *pSt);
void func_8006E268(ST_WORK *pSt);
void func_8006E34C(ST_WORK *pSt);
void FC_func_80073E7C(ST_WORK *pSt) __asm__("func_80073E7C");

void func_800654A8(ST_WORK *pSt);
void func_8006B66C(ST_WORK *pSt);
void func_80070244(ST_WORK *pSt);

LONG Cd_read_s(LONG Fno, LONG a1, ULONG *pAdr, LONG a3, char *pName);
LONG FE_func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName) __asm__("Cd_read");
void FE_func_800783BC(ULONG no, ULONG size, ULONG *a2) __asm__("func_800783BC");

void Mapping_tim(ULONG *pAdr)
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

INCLUDE_ASM("asm/nonmatchings/game/sub", Set_light_default);

INCLUDE_ASM("asm/nonmatchings/game/sub", Set_light);

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

INCLUDE_ASM("asm/nonmatchings/game/sub", Get_light_cut);

typedef struct {
    UCHAR Unk0;
    UCHAR Unk1;
    USHORT Unk2;
    LONG Unk4;
    LONG Unk8[3];
    UCHAR Unk14[4];
    LONG Unk18[3];
} FI_SAVE;
extern FI_SAVE D_800E11D0;
extern LONG D_800E11F4;

void func_80078FE8(void)
{
    ULONG i;
    GLOBAL_X *pG = &GX;
    FI_SAVE *p = &D_800E11D0;

    i = 0;
    p->Unk1 = pG->Unk2489;
    p->Unk4 = pG->Unk77F8;
    p->Unk2 = pG->Unk7806;
    D_800E11D0.Unk0 = pG->Unk7808;
    for (; i < 3; i++) {
        p->Unk8[i] = pG->Unk7E98[i];
        D_800E11D0.Unk14[i] = pG->Unk7EA4[i];
        p->Unk18[i] = pG->Unk7EA8[i];
    }
    D_800E11F4 = pG->Unk7EB4;
}

void func_8007906C(void)
{
    ULONG i;
    FI_SAVE *p = &D_800E11D0;
    GLOBAL_X *pG = &GX;

    i = 0;
    pG->Unk2489 = p->Unk1;
    pG->Unk77F8 = p->Unk4;
    pG->Unk7806 = p->Unk2;
    pG->Unk7808 = D_800E11D0.Unk0;
    for (; i < 3; i++) {
        pG->Unk7E98[i] = p->Unk8[i];
        pG->Unk7EA4[i] = D_800E11D0.Unk14[i];
        pG->Unk7EA8[i] = p->Unk18[i];
    }
    pG->Unk7EB4 = D_800E11F4;
}

INCLUDE_ASM("asm/nonmatchings/game/sub", func_800790F0);

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

void func_800792FC(void)
{
    FE_func_800783BC(2, Cd_read_s(0x128, 0, (ULONG *)0x80101000, 2, "INIT_SUB"), 0);
    Cd_read(0x127, (ULONG *)0x80101000, 0, "INIT_SUB");
    func_8007836C(2, (ULONG *)0x80101000);
}


INCLUDE_ASM("asm/nonmatchings/game/sub", func_80079374);

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
    MulLMatrix(&m1, &m3, &m4);
    m3.t[0] = v.vx;
    m3.t[1] = v.vy;
    m3.t[2] = v.vz;
    MulLMatrix(&m2, &m3, &m5);
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
        HMatrix((MATRIX *)pP[9].M, pM, (MATRIX *)pP[9].M, w);
        HMatrix((MATRIX *)pP[10].M, &D_800E1220, (MATRIX *)pP[10].M, w);
        HMatrix((MATRIX *)pP[11].M, &pM[2], (MATRIX *)pP[11].M, w);
        HMatrix((MATRIX *)pP[12].M, &D_800E1260, (MATRIX *)pP[12].M, w);
        HMatrix((MATRIX *)pP[13].M, &D_800E1280, (MATRIX *)pP[13].M, w);
        HMatrix((MATRIX *)pP[14].M, &pM[5], (MATRIX *)pP[14].M, 0xFFF - r);
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
