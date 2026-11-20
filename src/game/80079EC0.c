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

void Pcstr_set_buf(ULONG *pBuf, LONG size)
{
    Pcstr.pBuf[0] = pBuf;
    Pcstr.pBuf[1] = pBuf + (size << 8);
}

LONG Pcstr_open(char *pName)
{
    ULONG Sub0;
    ULONG Sub1;

    if ((Pcstr.Fd = PCopen(pName, 0, 0)) < 0)
        return -1;
    Pcstr.Interleave = 0;
    PCread(Pcstr.Fd, (char *)Pcstr.Sub_head, 8);
    Pcstr.Sub_head[0] = Sub0 = Pcstr.Sub_head[0] & 0xFF0000;
    Pcstr.Sub_head[1] = Sub1 = Pcstr.Sub_head[1] & 0xFF0000;
    /* sector subheaders of a raw (interleaved) STR: video or data submode in both copies */
    if ((Sub0 == 0x420000 && Sub1 == 0x420000) || (Sub0 == 0x480000 && Sub1 == 0x480000)
        || (Sub0 == 0x640000 && Sub1 == 0x640000))
        Pcstr.Interleave = 1;
    PClseek(Pcstr.Fd, 0, 0);
    Pcstr.Head[Pcstr.Buf_no].Frame_count = 0;
    do {
        Pcstr_read_frame(Pcstr.pBuf[Pcstr.Buf_no]);
    } while (Pcstr.Head[Pcstr.Buf_no].Frame_count < Pcstr.Start_frame);
    if (Pcstr.Start_func != NULL)
        Pcstr.Start_func();
    return 0;
}

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

typedef struct {
    UCHAR pad0[0xA2];
    USHORT UnkA2;
    UCHAR padA4[0xA8 - 0xA4];
    ULONG *pUnkA8;
    UCHAR padAC[0xBC - 0xAC];
} FJ_PARTS;

void func_8007A2D4(UCHAR *pW, UCHAR *pD)
{
    ULONG *pA = (ULONG *)(pD + 8);
    LONG i = 0;
    ULONG f;
    ULONG n;
    FJ_PARTS *pP;

    pP = FLD(pW, 0x108, FJ_PARTS *);
    FLD(pW, 0x11C, UCHAR *) = pD + *(USHORT *)(pD + 4);
    f = *(ULONG *)pD;
    n = *(USHORT *)(pD + 6);
loop:
    do {
        if (f & 1) {
            pP[i].pUnkA8 = pA;
            pA += 2;
            pP[i].UnkA2 |= 0x10;
        }
        f >>= 1;
        i++;
    } while (f != 0);
    if (--n != 0) {
        goto loop;
    }
}

/* model sort environment shared by the sort routines below */
typedef struct tagSORT_ENV {
    ENEMY_WORK *pEm;                   /* 0x00 G.pEm */
    ULONG Unk4;                        /* 0x04 */
    MATRIX *pWs;                       /* 0x08 &GsWSMATRIX */
    MATRIX *pM;                        /* 0x0C D_800C79F0 */
    VECTOR *pV;                        /* 0x10 D_800C79F4 */
} SORT_ENV;
extern SORT_ENV D_800E15A0;
extern UCHAR D_800E1388[];
extern MATRIX *D_800C79F0;
extern VECTOR *D_800C79F4;
extern MATRIX GsWSMATRIX;
void func_8007A600(void *pWork0, void *pWork1, SORT_ENV *pEnv, UCHAR *pData);
void func_8007A918(void *pWork0, void *pWork1, SORT_ENV *pEnv, UCHAR *pData);
void func_8007AC8C(void *pWork0, void *pWork1, SORT_ENV *pEnv, UCHAR *pData);
void func_8007B2FC(UCHAR *pDst, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData);
void func_8007B518(UCHAR *pDst, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData);
void func_8007B824(UCHAR *pDst, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData);

void func_8007A350(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP)
{
    UCHAR *pHead;
    ULONG Ofs;
    UCHAR *pData;
    SORT_ENV *pEnv = &D_800E15A0;

    pHead = (UCHAR *)pP->pUnkA8;
    Ofs = ((ULONG *)pHead)[0];
    pEnv->pEm = G.pEm;
    pEnv->Unk4 = Arg2;
    pEnv->pWs = &GsWSMATRIX;
    pEnv->pM = D_800C79F0;
    pEnv->pV = D_800C79F4;
    pData = (UCHAR *)((ULONG)pHead + (Ofs & 0xFFFF));
    func_8007A600(pWork0, pWork1, pEnv, pData);
    func_8007B2FC(D_800E1388, pP, pEnv, pData);
    if ((Ofs >>= 16) != 0) {
        func_8007A918(pWork0, pWork1, pEnv, pHead + Ofs);
        func_8007B518(D_800E1388, pP, pEnv, pHead + Ofs);
    }
    if ((Ofs = ((ULONG *)pHead)[1]) != 0) {
        func_8007AC8C(pWork0, pWork1, pEnv, pHead + Ofs);
        func_8007B824(D_800E1388, pP, pEnv, pHead + Ofs);
    }
}

extern UCHAR *D_800E15C8;              /* sort data of the part being drawn (func_8007A490) */
extern ULONG D_800E1598;               /* its current offset word */
void func_8007BFBC(void *pWork0, void *pWork1, SORT_ENV *pEnv, UCHAR *pData);
void func_8007C434(void *pWork0, void *pWork1, SORT_ENV *pEnv, UCHAR *pData);
void func_8007C91C(void *pWork0, void *pWork1, SORT_ENV *pEnv, UCHAR *pData);

/* func_8007A350 for parts drawn with the alternate (Flg 0x400) routines */
void func_8007A490(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP)
{
    SORT_ENV *pEnv = &D_800E15A0;
    UCHAR *pHead;
    ULONG Ofs;

    D_800E15C8 = pHead = (UCHAR *)pP->pUnkA8;
    Ofs = ((ULONG *)pHead)[0];
    pEnv->pEm = G.pEm;
    pEnv->Unk4 = Arg2;
    pEnv->pWs = &GsWSMATRIX;
    pEnv->pM = D_800C79F0;
    pEnv->pV = D_800C79F4;
    D_800E1598 = Ofs;
    func_8007BFBC(pWork0, pWork1, pEnv, (UCHAR *)((ULONG)pHead + (Ofs & 0xFFFF)));
    func_8007B2FC(D_800E1388, pP, pEnv, D_800E15C8 + (USHORT)D_800E1598);
    if ((D_800E1598 >>= 16) != 0) {
        func_8007C434(pWork0, pWork1, pEnv, D_800E15C8 + D_800E1598);
        func_8007B518(D_800E1388, pP, pEnv, D_800E15C8 + D_800E1598);
    }
    if ((D_800E1598 = ((ULONG *)D_800E15C8)[1]) != 0) {
        func_8007C91C(pWork0, pWork1, pEnv, D_800E15C8 + D_800E1598);
        func_8007B824(D_800E1388, pP, pEnv, D_800E15C8 + D_800E1598);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007A600);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007A918);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007AC8C);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007B2FC);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007B518);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007B824);

#define gte_ldsxy3(r0, r1, r2) __asm__ volatile("mtc2 %0, $12; mtc2 %2, $14; mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldsz3(r0, r1, r2) __asm__ volatile("mtc2 %0, $17; mtc2 %1, $18; mtc2 %2, $19" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_nclip_b() __asm__ volatile(".word 0x4B400006")
#define gte_avsz3() __asm__ volatile("nop; nop; .word 0x4B58002D")
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile("swc2 $12, 8(%0); swc2 $13, 20(%0); swc2 $14, 32(%0)" : : "r"(r0) : "memory")

/* textured gouraud triangle of a sort list: uv words with the vertex numbers in the top bytes */
typedef union tagSORT_GT3 {
    ULONG W[3];
    struct {
        UCHAR pad0[7];
        signed char V0;                /* 0x07 */
        UCHAR pad8[2];
        UCHAR V1;                      /* 0x0A */
        signed char V2;                /* 0x0B */
    } I;
} SORT_GT3;

typedef struct tagSORT_HDR {
    UCHAR pad0[0xC];
    SORT_GT3 *pGt3;                    /* 0x0C */
    struct tagSORT_GT4 *pGt4;           /* 0x10 */
    USHORT N_gt3;                      /* 0x14 */
    USHORT N_gt4;                      /* 0x16 */
} SORT_HDR;

typedef struct tagSORT_PRM {
    SORT_HDR *pHdr;                    /* 0x00 */
    ULONG Unk4;                        /* 0x04 */
    ULONG *pOt;                        /* 0x08 */
    ULONG *pRgb;                       /* 0x0C per-vertex colours */
} SORT_PRM;

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007BC7C);

/* textured gouraud quad of a sort list: the vertex numbers are the top two bytes of W[2] and W[3] */
typedef struct tagSORT_GT4 {
    LONG W[4];
} SORT_GT4;

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007BDF8);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007BFBC);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007C434);

INCLUDE_ASM("asm/nonmatchings/game/80079EC0", func_8007C91C);
