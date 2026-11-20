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
LONG func_80075A80(LONG x, LONG frac);
LONG func_800770DC_l(LONG lo, ULONG hi) __asm__("func_800770DC");
ULONG Snd_tone_mask(VagAtr *pVag, SND_TRACK *pTr, UCHAR tones);
void func_800762C8(UCHAR *pSeq, UCHAR *pTr, ULONG mask);
void Snd_pan_vol(SpuVoiceAttr *pAttr, SHORT vol, ULONG pan);
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

void Snd_work_init(void)
{
    SND_WORK *pW = &Snd_work;
    LONG i, j;

    for (i = 0; i < 8; i++) {
        pW->Bank_id[i].Id = -1;
    }
    for (i = 0; i < 3; i++) {
        pW->Seq_data[i].Id = 0xFF;
        pW->Seq[i].Unk54 = 0;
        pW->Seq[i].Unk38 = 0;
        for (j = 0; j < 16; j++) {
            pW->Seq[i].Trk[j].Mask = 0;
        }
    }
    for (i = 0; i < 24; i++) {
        pW->Voice[i].Unk1D = 0;
        pW->Voice[i].pad1A = 0;
        pW->Voice[i].Unk4 = 0;
        pW->Req[i].Unk18 = 0;
        pW->Req[i].Unk1A = 0;
    }
    pW->UnkB38 = 1;
    pW->UnkB1C = 0;
    pW->UnkB20 = 0;
    pW->UnkB24 = 0;
    pW->UnkB78 = 0;
    pW->UnkB76 = 0;
    pW->UnkB77 = 0;
    pW->pReq = pW->Req;
}

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

INCLUDE_ASM("asm/nonmatchings/game/snd", func_800741A0);

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

void Snd_se_stad(ULONG code, VECTOR *pPos, SHORT a2, SHORT a3)
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

INCLUDE_ASM("asm/nonmatchings/game/snd", func_800749A0);

INCLUDE_ASM("asm/nonmatchings/game/snd", func_80074CD0);

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

INCLUDE_ASM("asm/nonmatchings/game/snd", func_80075154);

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

INCLUDE_ASM("asm/nonmatchings/game/snd", func_800759D8);

INCLUDE_ASM("asm/nonmatchings/game/snd", func_80075A80);

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

INCLUDE_ASM("asm/nonmatchings/game/snd", func_80075C30);

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

/* sequencer note on: keys a voice for every tone of the program that covers the note */
void func_8007638C(SND_SEQ *pSeq, SND_TRACK *pTr)
{
    struct {
        ULONG voice;
        ULONG mask;
        SpuVolume volume;              /* 0x08 the only part Snd_pan_vol writes */
        UCHAR padC[0x20 - 0xC];
    } Attr;
    SND_WORK *pW;
    VabHdr *pVh;
    VagAtr *pVag;
    SND_VOICE *pV;
    ULONG mask;
    LONG Vel;
    LONG Pan;
    LONG v;
    USHORT Vol;

    pTr->Note = *pSeq->pData;
    Vel = *++pSeq->pData;
    pSeq->pData++;
    pW = &Snd_work;
    pVh = (VabHdr *)pW->Bank[pTr->Bank].pHead;
    pTr->UnkB = pW->Unk7CA * pVh->mvol / 127;
    pTr->UnkB = pTr->UnkB * pTr->Vol / 127;
    pTr->UnkB = pTr->UnkB * pW->Unk7C4 / 127;
    pTr->UnkB = pTr->UnkB * Vel / 127;
    pTr->UnkB = pTr->UnkB * pTr->Vol2 / 127;
    Vol = pTr->Vol2;
    mask = Snd_tone_mask((VagAtr *)pW->Bank[pTr->Bank].Unk4 + pTr->Prog * ((ULONG *)pVh)[3], pTr,
                         ((ProgAtr *)pVh + pTr->Prog + 2)->tones);
    if (Vel != 0) {
        Pan = pTr->Pan + pTr->Pan2 - 0x40;
        pTr->UnkD = 0;
        while (mask != 0) {
            if (mask & 1) {
                pVag = (VagAtr *)pW->Bank[pTr->Bank].Unk4 + pTr->Prog * ((ULONG *)pVh)[3] + pTr->UnkD;
                Snd_pan_vol((SpuVoiceAttr *)&Attr, pTr->UnkB * pVag->vol / 127, Pan + (pVag->pan - 0x40));
                Attr.volume.right = (Attr.volume.right * Attr.volume.right * 133135) >> 17;
                Attr.volume.left = (Attr.volume.left * Attr.volume.left * 133135) >> 17;
                if (pSeq->Unk38 != 0) {
                    Attr.volume.right = Attr.volume.right * pSeq->Unk3C / 0x3FFF;
                    Attr.volume.left = Attr.volume.left * pSeq->Unk3C / 0x3FFF;
                }
                if (Attr.volume.right | Attr.volume.left) {
                    v = func_800770DC_l(8, 24);
                    if (v != 0xFF) {
                        pV = &pW->Voice[v];
                        pV->Unk1D = 1;
                        pV->Unk17 = pTr->Prog;
                        pV->Unk18 = pTr->UnkD;
                        pV->Unk19 = pTr->Note;
                        pV->Unk14 = pTr->Bend;
                        pV->Unk1B = pTr->Bank;
                        pV->Unk12 = pTr->UnkE;
                        pV->Unk1C = pSeq->No;
                        pV->Unk4 = 0;
                        if (pW->UnkB75) {
                            if (Attr.volume.left > Attr.volume.right) {
                                Attr.volume.right = Attr.volume.left;
                            } else {
                                Attr.volume.left = Attr.volume.right;
                            }
                        }
                        pV->UnkA = pV->UnkC = Attr.volume.right;
                        pV->Unk16 = Pan;
                        pV->Unk8 = pV->UnkE = Attr.volume.left;
                        pV->pVag = (UCHAR *)pVag;
                        pV->Unk10 = Vol;
                        if (pVag->mode) {
                            pW->UnkB30 |= 1 << v;
                        } else {
                            pW->UnkB34 |= 1 << v;
                        }
                        pW->UnkB28 |= 1 << v;
                        pW->UnkB2C |= 1 << v;
                        pTr->Mask |= 1 << v;
                    }
                }
            }
            mask >>= 1;
            pTr->UnkD++;
        }
    } else {
        func_800762C8((UCHAR *)pSeq, (UCHAR *)pTr, mask);
    }
    pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
}

/* sequencer control change */
void func_8007688C(SND_SEQ *pSeq, SND_TRACK *pTr)
{
    UCHAR *p = pSeq->pData;
    SND_VOICE *pV;
    signed char i;

    switch (*p) {
    case 0:
        pSeq->pData = p + 1;
        pTr->Bank = p[1];
        pSeq->pData++;
        break;
    case 6:
        pSeq->pData = p + 1;
        if (pSeq->Nrpn == 0x14) {
            pSeq->Loop_mode = 1;
            pSeq->Loop_cnt = *pSeq->pData;
        }
        pSeq->pData++;
        break;
    case 7:
        pSeq->pData = p + 1;
        pTr->Vol2 = p[1];
        for (i = 8; i < 24 && pTr->Mask != 0; i++) {
            if (!(pTr->Mask & (1 << i))) {
                continue;
            }
            pV = &Snd_work.Voice[i];
            if (pV->Unk1D == 0 || pV->Unk4 == 0) {
                continue;
            }
            if (pV->Unk10 != 0) {
                pV->UnkA = pV->UnkA * pTr->Vol2 / pV->Unk10;
                pV->Unk8 = pV->Unk8 * pTr->Vol2 / pV->Unk10;
                if (Snd_work.UnkB75) {
                    if (pV->UnkA < pV->Unk8) {
                        pV->UnkA = pV->Unk8;
                    } else {
                        pV->Unk8 = pV->UnkA;
                    }
                }
                pV->UnkC = pV->UnkA;
                pV->UnkE = pV->Unk8;
                pV->Unk10 = pTr->Vol2;
                SpuSetVoiceVolume(i, pV->UnkA, pV->Unk8);
            } else {
                SpuSetVoiceVolume(i, 0, 0);
            }
        }
        pSeq->pData++;
        break;
    case 10:
        pSeq->pData = p + 1;
        pTr->Pan = p[1];
        pSeq->pData++;
        break;
    case 99:
        pSeq->pData = p + 1;
        pSeq->Nrpn = p[1];
        pSeq->pData++;
        if (pSeq->Nrpn == 0x1E) {
            pSeq->Loop_mode = 2;
        }
        break;
    case 11:
    case 64:
    case 91:
    case 98:
    case 100:
    case 101:
    case 121:
        pSeq->pData += 2;
        break;
    }
    pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
}

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

INCLUDE_ASM("asm/nonmatchings/game/snd", func_80076C64);

/* sequencer meta event: 0x51 set tempo, 0x2F end of track */
void func_80076E7C(SND_SEQ *pSeq)
{
    SND_WORK *pW = &Snd_work;
    ULONG Tempo;
    signed char i;
    LONG Unused[6];

    if (pSeq->Status != 0xF) {
        return;
    }
    if (*pSeq->pData == 0x51) {
        Tempo = 0;
        i = 1;
        do {
            Tempo |= *++pSeq->pData << ((3 - i) * 8);
        } while (i++ < 3);
        pSeq->Tempo_cnt = pSeq->Tempo = Tempo * 1000 / pSeq->Resolution;
        pSeq->pData++;
        pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
    } else if (*pSeq->pData == 0x2F && pSeq->pData[1] == 0) {
        if (pSeq->Unk18 == 0) {
            if (pSeq->Loop_mode == 2) {
                if (pSeq->Loop_cnt == 0x7F || --pSeq->Loop_cnt != 0) {
                    pSeq->pData = pSeq->pLoop;
                }
                pSeq->Loop_mode = 0;
            } else {
                pSeq->Unk2C = 0;
                pW->UnkB74 &= ~(1 << pSeq->No);
                pW->UnkB76 &= ~(1 << pSeq->No);
            }
        } else {
            pSeq->pData = pSeq->pTop;
        }
        if (pSeq->Unk2C == 0) {
            pSeq->Delta = 1;
        } else {
            pSeq->Delta = 0;
        }
    }
}

void func_80077030(LONG *p)
{
    p[3] = 1;
}

INCLUDE_ASM("asm/nonmatchings/game/snd", Snd_seq_get_vlq);

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

void Snd_se_enem(ULONG no, ENEMY_WORK *pEm)
{
    VECTOR v;

    v.vx = pEm->Pos_x;
    v.vy = pEm->Pos_y;
    v.vz = pEm->Pos_z;
    if (*((UCHAR *)pEm + 0x12F) == D_800BA6E8[3]) {
        no += 0x10;
    }
    Snd_se_stad(no | 0x10300, &v, 0, 0);
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
    Snd_se_stad(no | 0x10300, &v, 0, 0);
}

void Snd_se_walk(LONG kind, ULONG parts, ENEMY_WORK *pEm)
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
        Snd_se_stad(kind | 0x10200, &v, 0, 0);
    } else {
        Snd_se_stad(kind | 0x10200, &v, -10, -2);
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

INCLUDE_ASM("asm/nonmatchings/game/snd", func_80077C4C);

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
    Snd_se_stad(0xE, 0, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/game/snd", func_80077F60);

LONG Cd_read_s(LONG Fno, LONG a1, ULONG *pAdr, LONG a3, char *pName);
LONG FE_func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName) __asm__("Cd_read");
void FE_func_800783BC(ULONG no, ULONG size, ULONG *a2) __asm__("func_800783BC");

LONG func_8007809C(LONG no, LONG idx, ULONG *pAdr, ULONG *pBuf)
{
    USHORT tbl[8] = { 0x104, 0x103, 0xDA, 0xD9, 0, 0, 0, 0 };
    signed char n;
    ULONG *p;
    LONG r;

    if (no == 1 && idx == 0) {
        return 1;
    }
    n = (signed char)D_800A0FE4[no];
    p = pAdr;
    while (n > 0) {
        *p++ = -1;
        n--;
    }
    r = FE_func_80012818(idx * 2 + tbl[no * 2], pAdr, 0, "Vab Header");
    func_8007836C(no, pAdr);
    FE_func_800783BC(no, Cd_read_s(idx * 2 + tbl[no * 2 + 1], 2, pBuf, no, "Vab Body"), pBuf);
    if (r < 0) {
        return 0;
    }
    return r;
}

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
    pW->UnkB78 = ((FC_ROOM *)G.pRoom)->Unk6 * 4;
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

/* registers a sequence pack (and its VAB) loaded for enemy slot no */
void func_800782F4(ULONG no, ULONG id, ULONG *pAdr)
{
    SND_WORK *pW = &Snd_work;
    ULONG *p = pAdr;
    ULONG bank = no + 5;
    VabHdr *pVh;
    ProgAtr *pPa;

    pW->Seq_data[no].Id = id;
    pW->Seq_data[no].Unk1 = 0;
    pW->Seq_data[no].pData = pAdr;
    if (no != 0) {
        pAdr = (ULONG *)((UCHAR *)pAdr + *pAdr);
        pW->Seq_data[no + 1].Id = id;
        pW->Seq_data[no + 1].Unk1 = 0;
        p = pAdr;
        pW->Seq_data[no + 1].pData = pAdr;
    }
    pW->Bank[bank].pHead = (ULONG *)(pVh = (VabHdr *)((UCHAR *)pAdr + *p));
    pPa = (ProgAtr *)(pVh + 1);
    pW->Bank[bank].Unk4 = (ULONG *)(pPa + pVh->ps);
}

/* registers the VAB of bank no, D_800A0FE4[no] words into the file at pAdr */
void func_8007836C(LONG no, ULONG *pAdr)
{
    SND_WORK *pW = &Snd_work;
    VabHdr *pVh;
    ProgAtr *pPa;

    pW->pAdr[no] = pAdr;
    pW->Bank[no].pHead = (ULONG *)(pVh = (VabHdr *)(pAdr + D_800A0FE4[no]));
    pPa = (ProgAtr *)(pVh + 1);
    pW->Bank[no].Unk4 = (ULONG *)(pPa + pVh->ps);
}

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
