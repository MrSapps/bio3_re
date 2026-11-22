#include "sub.h"
#include "pcstr.h"
#include "snd.h"
#include "em_sub.h"
#include "cd.h"
#include "status.h"

#define EM_P(pEm, ofs, type) (*(type *)((UCHAR *)(pEm) + (ofs)))  /* ENEMY_WORK fields past the named ones */
#define FLD(p, ofs, type) (*(type *)((UCHAR *)(p) + (ofs)))  /* unnamed struct fields */

/* ---- merged helpers (forks + parent) ---- */
typedef struct { UCHAR b[0xA0]; } FA_A0[2][2];
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
UCHAR FB_func_8006E598(void) __asm__("func_8006E598");
extern UCHAR D_800A04F8[];
extern UCHAR D_800A07C4[];
extern UCHAR D_800A0514[][4];
extern SpuReverbAttr D_800A1108[];
extern UCHAR D_800A10F4[][0x14];

void func_80078FE8(void);
void func_8007906C(void);
void func_800792FC(void);
void func_800790F0(ULONG *pAdr);

extern LONG D_800E0608;
void func_80075C30(void);

#define FC_SQ(x) ((x) * (x))

LONG func_8004D720(VECTOR *pV, LONG a1, LONG a2, LONG a3);
extern MATRIX D_800E1200[];
extern MATRIX D_800E1220;
extern MATRIX D_800E1260;
extern MATRIX D_800E1280;
void HMatrix(MATRIX *a, MATRIX *b, MATRIX *c, LONG r);
void MulLMatrix(MATRIX *a, MATRIX *b, MATRIX *c);
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
void func_800749A0(SND_REQ *pReq, USHORT Id, SND_SE *p, LONG bank);
LONG func_80075AE4(UCHAR *pVag, SND_VOICE *pV);
LONG func_80074F44(SND_SE *pSe, LONG v, SND_REQ *pReq);
void func_80074CD0(SND_VOICE *pV, signed char v);
void Snd_set_voice_vol(SHORT l, SHORT r, ULONG voice);
extern void (*D_800A11A0[])(SND_SEQ *pSeq, SND_TRACK *pTr, ULONG Status);
extern void (*D_800A1130[])(SND_REQ *pReq);   /* func_80074770 (SE), func_80074820 (sequence) */
extern void (*D_800A1138[])(SND_SEQ *pSeq);   /* Snd_seq_fade_in, Snd_seq_fade_out */
void func_800740AC(void);
void func_800772A0(LONG no, UCHAR *pData, LONG bank, LONG loop); /* MIDI event handlers, status 0x80..0xF0 */
void func_80075154(SND_REQ *pReq, USHORT vol, USHORT *pR, USHORT *pL, SHORT dist);
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);
LONG func_8007592C(LONG x0, LONG z0, LONG x1, LONG z1);
extern UCHAR D_800A1008[];              /* pan curve, 0x80 entries */
extern UCHAR D_800A1088[];              /* attenuation by distance / 250 */
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
    SND_WORK *pW = &Snd_work;

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
    pW->Vol.Master = 0x7F;
    pW->Vol.Unk6 = 0x7F;
    pW->Vol.Se = 0x7F;
    pW->Vol.Bgm = 0x7F;
    pW->Vol.Master_old = 0x7F;
    pW->Vol.UnkE = 0x7F;
    pW->Vol.Se_old = 0x7F;
    pW->Vol.Bgm_old = 0x7F;
    Snd_work_init();
    pW->Vol.Rv_mode = 0;
    pW->Vol.Rv_cur = 0;
    pW->Vol.Rv_step = 0;
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
        pW->Voice[i].Prio = 0;
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
    if (Snd_work.Vol.Rv_step == 0) {
        SpuReserveReverbWorkArea(1);
        SpuClearReverbWorkArea(D_800A1108[Snd_work.Vol.Rv_mode - 1].mode);
        Snd_work.Vol.Rv_cnt = 30;
        Snd_work.Vol.Rv_step++;
    } else if (--Snd_work.Vol.Rv_cnt == 0) {
        SpuSetReverbModeParam((SpuReverbAttr *)D_800A10F4[Snd_work.Vol.Rv_mode]);
        SpuSetReverbDepth((SpuReverbAttr *)D_800A10F4[Snd_work.Vol.Rv_mode]);
        SpuSetReverb(1);
        Snd_work.Vol.Rv_step = 0;
        Snd_work.Vol.Rv_cur = Snd_work.Vol.Rv_mode;
        Snd_work.Vol.Rv_mode = 0;
    }
}

/* sound driver frame: reverb change, master fade, volume changes, request queue,
   sequence start/fade requests, SE key on */
void func_800741A0(void)
{
    SND_WORK *pW = &Snd_work;
    SND_VOL *pVol;
    SND_VOICE *pV;
    SND_REQ *pReq;
    SND_SEQ *pSeq;
    ULONG Flg;
    LONG i;

    Flg = 0;
    pVol = &pW->Vol;
    if (!(pW->UnkB38 & 1)) {
        return;
    }
    if (pW->Vol.Rv_mode && pW->Vol.Rv_mode != pW->Vol.Rv_cur) {
        func_800740AC();
    }
    if (pW->UnkB1C) {
        if (pW->UnkB24 >= 0) {
            if (pW->UnkB24 >= pW->UnkB20) {
                pW->UnkB20 = 0;
                pW->UnkB24 = 0;
            } else {
                pW->UnkB20 -= pW->UnkB24;
            }
        } else {
            if (pW->UnkB20 >= pW->UnkB24 + 0x3FFF) {
                pW->UnkB20 = 0x3FFF;
                pW->UnkB24 = 0;
            } else {
                pW->UnkB20 -= pW->UnkB24;
            }
        }
        Flg = pW->UnkB1C;
    }
    if (pVol->Master != pVol->Master_old) {
        pVol->Master_old = pVol->Master;
        Flg = 0xFFFFFF;
    } else {
        if (pVol->Se != pVol->Se_old) {
            pVol->Se_old = pVol->Se;
            Flg = 0xFF;
        }
        if (pVol->Bgm != pVol->Bgm_old) {
            pVol->Bgm_old = pVol->Bgm;
            Flg = 0xFFFF00;
        }
    }
    i = 0;
    if (Flg) {
        pV = pW->Voice;
        do {
            if (Flg & 1) {
                pV->Unk8 = pV->UnkE * pVol->Master / 127;
                pV->UnkA = pV->UnkC * pVol->Master / 127;
                if (i < 8) {
                    if (pW->UnkB1C == 0) {
                        pV->Unk8 = pV->UnkE * pVol->Se / 127;
                        pV->UnkA = pV->UnkC * pVol->Se / 127;
                    } else {
                        pV->Unk8 = pV->UnkE * pW->UnkB20 / 0x3FFF;
                        pV->UnkA = pV->UnkC * pW->UnkB20 / 0x3FFF;
                    }
                } else {
                    pV->Unk8 = pV->UnkE * pVol->Bgm / 127;
                    pV->UnkA = pV->UnkC * pVol->Bgm / 127;
                }
                SpuSetVoiceVolume(i, pV->UnkA, pV->Unk8);
            }
            pV++;
            Flg >>= 1;
            i++;
        } while (Flg);
        if (pW->UnkB20 == 0 || pW->UnkB20 == 0x3FFF) {
            pW->UnkB1C = 0;
        }
    }
    pW->UnkB10 = pW->UnkB14 = pW->UnkB18 = 0;
    SpuRGetAllKeysStatus(0, 23, pW->Vprio);
    for (pReq = pW->Req; pReq != pW->pReq; pReq++) {
        D_800A1130[pReq->Bank >= 5](pReq);
    }
    pW->pReq = pW->Req;
    pSeq = pW->Seq;
    for (i = 0; i < 3; i++) {
        if (pSeq->Unk54 == 1) {
            func_800772A0(pSeq->Req_no, pSeq->pReq_data, pSeq->Req_bank, pSeq->Req_loop);
            pSeq->Unk54 = 0;
        }
        if (pSeq->Unk38) {
            D_800A1138[pSeq->Unk38 - 1](pSeq);
        }
        pSeq++;
    }
    SpuSetKey(0, pW->UnkB14);
    pV = pW->Voice;
    i = 0;
    Flg = pW->UnkB10;
    do {
        if (Flg & 1) {
            func_80074CD0(pV, i);
        }
        Flg >>= 1;
        i++;
        pV++;
    } while (i < 8);
    SpuSetKey(1, pW->UnkB10);
}

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
            func_800749A0(pReq, Snd_work.Bank_id[b].Id, (SND_SE *)p, b);
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
        pSeq->Bank = b;
        pSeq->No = no;
        if (pSeq->Unk2C) {
            Snd_seq_stop(no);
            pSeq->Unk54 = 2;
        } else {
            pSeq->Unk54 = 1;
        }
        pSeq->pVh = pW->Bank[no ? 6 : 5].pHead;
        pSeq->Req_no = pSeq->No;
        pSeq->pReq_data = (UCHAR *)pW->Seq_data[no].pData;
        pSeq->Req_bank = pW->Bank_id[no ? 6 : 5].Id;
        pSeq->Req_loop = 0;
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

/* keys the voices of one sound effect: picks free voices, sets their tone and volume */
void func_800749A0(SND_REQ *pReq, USHORT Id, SND_SE *p, LONG bank)
{
    SND_WORK *pW = &Snd_work;
    SND_VOICE *pV;
    VagAtr *pVag;
    ULONG v;
    ULONG mask;
    ULONG bit;
    LONG vol;
    signed char i;

    i = 0;
    if (!(p->Voice & 0x80)) {
        v = p->Voice & 0x1F;
    } else {
        v = p->Voice & 0x1F;
        mask = p->Mask;
        for (; i < 8; i++) {
            if (mask & 1) {
                if (pW->Vprio[v + i] == 0 || pW->Vprio[v + i] == 3) {
                    v += i;
                    break;
                }
            }
            mask >>= 1;
        }
        i = 0;
    }
    do {
        if (func_80074F44(p, v, pReq) == 0) {
            pV = &pW->Voice[v + i];
            pV->Unk17 = p->Flg & 1;
            pV->Unk18 = i + (p->Tone >> 4);
            pV->Prio = p->Tone & 7;
            pV->Unk1B = bank;
            pV->Unk12 = Id;
            pVag = (VagAtr *)pW->Bank[bank].Unk4 + (pV->Unk18 + pV->Unk17 * pW->Bank[pV->Unk1B].pHead[3]);
            pV->Unk19 = pVag->min;
            if (pReq->Unk18 != 0) {
                pV->Unk19 += pReq->Unk18;
                pReq->Unk18 = 0;
            }
            if (!(p->Flg & 0x20)) {
                vol = func_80075AE4((UCHAR *)pVag, pV);
                if (pReq->Unk1A != 0) {
                    vol += pReq->Unk1A;
                    pReq->Unk1A = 0;
                }
                func_80075154(pReq, vol, (USHORT *)&pV->UnkA, (USHORT *)&pV->Unk8, pW->Dist[v]);
            } else {
                vol = func_80075AE4((UCHAR *)pVag, pV);
                if (pReq->Unk1A != 0) {
                    vol += pReq->Unk1A;
                    pReq->Unk1A = 0;
                }
                Snd_pan_vol((SpuVoiceAttr *)pV, vol, pVag->pan);
                pV->UnkA = (pV->UnkA * pV->UnkA * 0x2080F) >> 17;
                pV->Unk8 = (pV->Unk8 * pV->Unk8 * 0x2080F) >> 17;
            }
            pV->Unk14 = 0;
            pV->UnkC = pV->UnkA;
            pV->UnkE = pV->Unk8;
            bit = 1 << (v + i);
            pW->UnkB10 |= bit;
            pW->UnkB14 |= bit;
            pW->Vprio[v + i] = 1;
        }
        i++;
    } while (i < (p->Flg >> 6) + 1);
}

/* keys voice v for SND_VOICE pV: reverb, pitch, ADSR and volume from its VagAtr */
void func_80074CD0(SND_VOICE *pV, signed char v)
{
    SpuVoiceAttr Attr;
    SND_WORK *pW = &Snd_work;
    VagAtr *pVag;
    ULONG Note;
    ULONG Pitch;

    pVag = (VagAtr *)pW->Bank[pV->Unk1B].Unk4 + (pV->Unk18 + pV->Unk17 * pW->Bank[pV->Unk1B].pHead[3]);
    pW->UnkB18 = 0;
    if (pV->Unk1B < 5) {
        if (pVag->mode & pW->UnkB78) {
            pW->UnkB18 = 1 << v;
            SpuSetReverbVoice(1, pW->UnkB18);
        } else {
            pW->UnkB18 = 1 << v;
            SpuSetReverbVoice(0, pW->UnkB18);
        }
    }
    Attr.voice = 1 << v;
    Attr.mask = 0x60093;
    Attr.addr = (ULONG)Snd_bank_get_tone(pV->Unk1B, pVag->vag);
    Note = pVag->shift;
    Pitch = (pV->Unk19 << 7) | Note;
    Note = Pitch;
    if (pV->Unk14 > 0x2000) {
        Note += ((pV->Unk14 - 0x2000) << 7) * pVag->pbmax / 0x1FFF;
    }
    if (pV->Unk14 < 0x2000) {
        Note -= ((0x2000 - pV->Unk14) << 7) * pVag->pbmin / 0x1FFF;
    }
    Attr.pitch = func_80075A80(((Note & 0x3F80) >> 7) - pVag->center, Note & 0x7F);
    Attr.adsr1 = pVag->adsr1;
    Attr.adsr2 = pVag->adsr2;
    Attr.ar = (Attr.adsr1 & 0x7F00) >> 8;
    Attr.dr = (Attr.adsr1 & 0xF0) >> 4;
    Attr.sl = Attr.adsr1 & 0xF;
    Attr.rr = Attr.adsr2 & 0x1F;
    Attr.sr = (Attr.adsr2 & 0x1FC0) >> 6;
    if (Attr.adsr1 & 0x8000) {
        Attr.a_mode = 5;
    } else {
        Attr.a_mode = 1;
    }
    if (Attr.adsr2 & 0x20) {
        Attr.r_mode = 7;
    } else {
        Attr.r_mode = 3;
    }
    Attr.s_mode = (Attr.adsr2 >> 14) + 1;
    Attr.volume.left = pV->UnkA;
    Attr.volume.right = pV->Unk8;
    SpuSetVoiceAttr(&Attr);
}

LONG func_80074F44(SND_SE *pSe, LONG v, SND_REQ *pReq)
{
    SND_WORK *pW = &Snd_work;
    SND_VOICE *pV;
    LONG dx, dy, dz, d;

    pV = &((SND_WORK *)pW)->Voice[v];
    if ((UCHAR)(pW->Vprio[v] - 1) < 2) {
        if (pV->Prio >= (pSe->Tone & 7)) {
            if (pV->Prio != (pSe->Tone & 7)) {
                return -1;
            }
            if (pSe->Tone & 8) {
                return -1;
            }
        }
    }
    if (pReq->Unk1E) {
        pW->Cam_x = G.pRoom->pRcut[G.Cut_no].View_p[0];
        pW->Cam_y = G.pRoom->pRcut[G.Cut_no].View_p[1];
        pW->Cam_z = G.pRoom->pRcut[G.Cut_no].View_p[2];
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

/* voice volumes of a sound request: 2D pan (Unk1E 0) or 3D from the camera (Unk1E 1) */
void func_80075154(SND_REQ *pReq, USHORT vol, USHORT *pR, USHORT *pL, SHORT dist)
{
    SND_WORK *pW = &Snd_work;
    VECTOR V;
    UCHAR Flg;                         /* never set: the original tests an uninitialised stack byte */
    RCUT *pCut;
    VECTOR *pPos;
    LONG Far;
    LONG a0;
    LONG a1;
    LONG d;
    UCHAR l;
    UCHAR r;
    UCHAR *pAtt;
    LONG Sum;
    LONG k;

    vol = (vol * vol * 0x2080F) >> 17;
    switch (pReq->Unk1E) {
    case 0:
        if (pReq->Pan > 0) {
            *pR = vol * (0x80 - (USHORT)pReq->Pan) / 128;
            *pL = vol;
        } else {
            *pR = vol;
            *pL = vol * (USHORT)(pReq->Pan + 0x80) / 128;
        }
        if (pW->UnkB75) {
            if (*pR > *pL) {
                *pL = *pR;
            } else {
                *pR = *pL;
            }
        }
        *pR = *pR * (0x80 - pReq->Atten) / 128;
        *pL = *pL * (0x80 - pReq->Atten) / 128;
        break;
    case 1:
        if (dist == 0) {
            *pL = vol;
            *pR = vol;
            return;
        }
        pCut = &G.pRoom->pRcut[G.Cut_no];
        pPos = &pReq->Pos;
        V.vx = __builtin_abs(pCut->View_p[0] - pReq->Pos.vx);
        V.vy = __builtin_abs(pCut->View_p[1] - pPos->vy);
        V.vz = __builtin_abs(pCut->View_p[2] - pPos->vz);
        Far = SquareRoot0(V.vy * V.vy + FC_SQ(SquareRoot0(V.vx * V.vx + V.vz * V.vz))) / 250;
        if (Far > 0x7F) {
            Far = 0x7F;
        }
        a0 = (SHORT)func_8007592C(pCut->View_p[0], pCut->View_p[2], pReq->Pos.vx, pPos->vz);
        a1 = (SHORT)func_8007592C(pCut->View_p[0], pCut->View_p[2], pCut->View_r[0], pCut->View_r[2]);
        a0 -= (SHORT)func_8007592C(0, 0, pCut->View_p[0], pCut->View_p[2]);
        a1 -= (SHORT)func_8007592C(0, 0, pCut->View_p[0], pCut->View_p[2]);
        if (a0 < a1) {
            d = a0 + (0x1000 - a1);
        } else {
            d = a0 - a1;
        }
        if (d == 0 || d == 0x1000 || d == 0x800) {
            *pL = 0x7F;
            *pR = 0x7F;
        } else {
            if (d & 0x800) {
                l = 1;
                r = 0;
            } else {
                l = 0;
                r = 1;
            }
            d &= 0x7FF;
            if (d & 0x400) {
                d = 0x800 - d;
            }
            d /= 8;
            a1 = D_800A1008[d];
            a0 = 0x7F;
            *pL = l * a1 + r * a0;
            *pR = l * a0 + r * a1;
        }
        pAtt = &D_800A1088[Far];
        if (*pAtt < *pR) {
            *pR -= *pAtt;
        }
        if (*pAtt < *pL) {
            *pL -= *pAtt;
        }
        V.vx = pPos->vx;
        V.vy = pPos->vy - 0x5DC;
        V.vz = pPos->vz;
        if (Flg != 2 && Sca_ck_line((VECTOR *)pCut->View_p, &V, 0x8400, 1)) {
            *pR = *pR * 65 / 100;
            *pL = *pL * 65 / 100;
        }
        if (*pR >= 0x80) {
            *pR = 0x7F;
        }
        if (*pL >= 0x80) {
            *pL = 0x7F;
        }
        if (pW->UnkB75) {
            if (*pR > *pL) {
                *pL = *pR;
            } else {
                *pR = *pL;
            }
        }
        *pR = vol * *pR / 127;
        *pL = vol * *pL / 127;
        if (dist < 0) {
            dist *= -1;
        }
        k = 60000 - dist;
        *pR = *pR * k / 60000;
        *pL = *pL * k / 60000;
        Sum = *pR + *pL;
        if (Sum < 0x400) {
            if (*pR == *pL) {
                *pL = 0x200;
                *pR = 0x200;
            } else if (*pR < *pL) {
                *pR = *pR + (0x400 - Sum) / Sum * *pR;
                *pL = 0x400 - *pR;
            } else {
                *pL = *pL + (0x400 - Sum) / Sum * *pL;
                *pR = 0x400 - *pL;
            }
        }
        break;
    }
    *pR = *pR * pW->Vol.Master / 127;
    *pL = *pL * pW->Vol.Master / 127;
    *pR = *pR * pW->Vol.Se / 127;
    *pL = *pL * pW->Vol.Se / 127;
}

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

/* 2^(x/12) scale: 0x1000 shifted by whole octaves times the 12-step table entry, >> 16.
   `v *= r` is repeated in both branches (cross-jumped into one tail): a mult in the same block
   as the /12 is what lets reload move the high-part pseudo out of HI into v1 (see NOTES.md). */
LONG func_800759D8(LONG x)
{
    LONG v = 0x1000;
    LONG *pTbl;
    LONG r;

    if (x >= 0) {
        pTbl = D_800A1140;
        r = pTbl[x % 12];
        v <<= x / 12;
        v *= r;
    } else {
        x = -x;
        pTbl = D_800A1170;
        r = pTbl[x % 12];
        v >>= x / 12;
        v *= r;
    }
    return v >> 16;
}

/* interpolates func_800759D8 between x and x + 1, frac in 1/128 */
LONG func_80075A80(LONG x, LONG frac)
{
    LONG a, b;

    a = func_800759D8(x);
    b = func_800759D8(x + 1);
    a += (b - a) * frac / 128;
    return a;
}

LONG func_80075AE4(UCHAR *pVag, SND_VOICE *pV)
{
    VabHdr *pVh = (VabHdr *)Snd_work.Bank[pV->Unk1B].pHead;
    ULONG i = pV->Unk17;

    return pVh->mvol * ((ProgAtr *)(pVh + 1))[i].mvol / 127 * pVag[2] / 127 * Snd_work.Vol.Se / 127;
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

/* sequencer tick (SPU root counter event): advances the sequences, keys the voices
   they requested and applies the fades */
void func_80075C30(void)
{
    SND_WORK *pW = &Snd_work;
    SND_SEQ *pSeq;
    SND_VOICE *pV;
    ULONG Flg;
    ULONG Mask;
    ULONG c;
    signed char i;
    signed char j;
    LONG Unused[22];
    LONG Frac;

    if (pW->UnkB74 == 0) {
        return;
    }
    i = 3;
    pSeq = pW->Seq;
    Flg = 0;
    pW->UnkB28 = pW->UnkB2C = pW->UnkB30 = pW->UnkB34 = 0;
    do {
        i--;
        if (pSeq->Unk2C && !pSeq->Unk2D) {
            Flg |= 1 << (2 - i);
            pSeq->Delta -= 300000;
            pSeq->Tempo_cnt -= 4800000;
            if (pSeq->Tempo_cnt <= 0) {
                Flg |= 1 << (6 - i);
                pSeq->Tempo_cnt = pSeq->Tempo;
            }
            while (pSeq->Delta <= 0) {
                c = *pSeq->pData;
                if (c >= 0x80) {
                    pSeq->pData++;
                    pSeq->Status = c & 0xF;
                    pSeq->Running = c;
                } else {
                    c = pSeq->Running;
                }
                D_800A11A0[(c >> 4) - 8](pSeq, &pSeq->Trk[pSeq->Status], c);
                if (pSeq->Loop_mode == 1) {
                    pSeq->Loop_mode = 0;
                    pSeq->pLoop = pSeq->pData;
                }
                Frac = pSeq->Delta * (pSeq->Tempo & 0xF);
                pSeq->Delta = pSeq->Delta * (pSeq->Tempo >> 4) + ((Frac + 0xF) >> 4);
            }
            if (pSeq->Loop_mode) {
                if (pSeq->Loop_mode == 1) {
                    pSeq->pLoop = pSeq->pData;
                } else if (pSeq->Loop_cnt == 0x7F || --pSeq->Loop_cnt != 0) {
                    pSeq->pData = pSeq->pLoop;
                }
                pSeq->Loop_mode = 0;
            }
        }
        pSeq++;
    } while (i != 0);
    Mask = pW->UnkB28 >> 8;
    if (Flg) {
        pV = &pW->Voice[8];
        SpuSetKey(0, pW->UnkB2C);
        j = 8;
        SpuSetReverbVoice(0, pW->UnkB34);
        do {
            if (Mask & 1) {
                func_80074CD0(pV, j);
            }
            Mask >>= 1;
            j++;
            pV++;
        } while (j < 24);
        SpuSetReverbVoice(1, pW->UnkB30);
        SpuSetKey(1, pW->UnkB28);
    }
    for (j = 8, pV = &pW->Voice[8]; j < 24; j++, pV++) {
        if (pV->Unk1D) {
            if (Flg & (0x10 << pV->Unk1C)) {
                if (pW->Seq[pV->Unk1C].Unk38) {
                    pV->UnkA = pV->UnkC * pW->Seq[pV->Unk1C].Unk3C / 0x3FFF;
                    pV->Unk8 = pV->UnkE * pW->Seq[pV->Unk1C].Unk3C / 0x3FFF;
                    Snd_set_voice_vol(pV->UnkA, pV->Unk8, 1 << j);
                }
            }
            pV->Unk4++;
        }
    }
    Mask = 0;
    i = 2;
    pSeq = pW->Seq;
    do {
        if (pSeq->Unk2C && pSeq->Unk2D) {
            pSeq->Unk2C = pSeq->Unk2D = 0;
            pW->UnkB74 &= ~(1 << pSeq->No);
            pW->UnkB76 &= ~(1 << pSeq->No);
            for (j = 8, pV = &pW->Voice[8]; j < 24; j++, pV++) {
                if (pV->Unk1C == pSeq->No) {
                    Mask |= 1 << j;
                    pV->Unk1D = 0;
                }
            }
        }
        if (pSeq->Unk54 == 2) {
            pSeq->Unk54 = 1;
        }
        pSeq++;
    } while (--i != -1);
    if (Mask) {
        SpuSetKey(0, Mask);
        Snd_set_voice_vol(0, 0, Mask);
    }
}

void func_80076114(UCHAR *pSeq, UCHAR *pTr)
{
    SND_WORK *pW;
    VabHdr *pVh;
    LONG vel;
    VagAtr *pVag;
    ProgAtr *pPa;

    pTr[0xC] = **(UCHAR **)(pSeq + 4);
    (*(UCHAR **)(pSeq + 4))++;
    vel = **(UCHAR **)(pSeq + 4);
    (*(UCHAR **)(pSeq + 4))++;
    pW = &Snd_work;
    pVh = (VabHdr *)((SND_WORK *)pW)->Bank[pTr[0xF]].pHead;
    pTr[0xB] = pW->Vol.Bgm * pVh->mvol / 127;
    pTr[0xB] = pTr[0xB] * pTr[0xA] / 127;
    pTr[0xB] = pTr[0xB] * pTr[7] / 127;
    pTr[0xB] = pTr[0xB] * pW->Vol.Master / 127;
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
    ProgAtr *pPa;
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
    pTr->UnkB = pW->Vol.Bgm * pVh->mvol / 127;
    pTr->UnkB = pTr->UnkB * pTr->Vol / 127;
    pTr->UnkB = pTr->UnkB * pW->Vol.Master / 127;
    pTr->UnkB = pTr->UnkB * Vel / 127;
    pTr->UnkB = pTr->UnkB * pTr->Vol2 / 127;
    pVag = (VagAtr *)pW->Bank[pTr->Bank].Unk4 + pTr->Prog * ((ULONG *)pVh)[3];
    pPa = (ProgAtr *)pVh + pTr->Prog + 2;
    Vol = pTr->Vol2;
    mask = Snd_tone_mask(pVag, pTr, pPa->tones);
    if (Vel != 0) {
        Pan = pTr->Pan2 + (pTr->Pan - 0x40);
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
                        pV->Unk8 = pV->UnkE = Attr.volume.left;
                        pV->pVag = (UCHAR *)pVag;
                        pV->Unk10 = Vol;
                        pV->Unk16 = Pan;
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

/* sequencer pitch bend event: re-pitches every voice keyed by the track */
void func_80076C64(SND_SEQ *pSeq, SND_TRACK *pTr)
{
    SND_VOICE *pV;
    VagAtr *pVag;
    ULONG *pHead;
    ULONG Note;
    ULONG Pitch;
    LONG i;

    pTr->Bend = *pSeq->pData;
    pTr->Bend |= *++pSeq->pData << 7;
    pSeq->pData++;
    if (pTr->Mask != 0) {
        for (i = 8; i < 24; i++) {
            if (!(pTr->Mask & (1 << i))) {
                continue;
            }
            pV = &Snd_work.Voice[i];
            if (pV->Unk1D == 0 || pV->Unk4 == 0) {
                continue;
            }
            pV->Unk14 = pTr->Bend;
            pHead = Snd_work.Bank[pV->Unk1B].pHead;
            pVag = (VagAtr *)Snd_work.Bank[pV->Unk1B].Unk4 + (pV->Unk18 + pV->Unk17 * pHead[3]);
            Note = pVag->shift;
            Pitch = (pV->Unk19 << 7) | Note; /* via a temp: or v0,a0 operand order */
            Note = Pitch;
            if (pV->Unk14 > 0x2000) {
                Note += ((pV->Unk14 - 0x2000) << 7) * pVag->pbmax / 0x1FFF;
            }
            if (pV->Unk14 < 0x2000) {
                Note -= ((0x2000 - pV->Unk14) << 7) * pVag->pbmin / 0x1FFF;
            }
            SpuSetVoicePitch(i, func_80075A80(((Note & 0x3F80) >> 7) - pVag->center, Note & 0x7F));
        }
    }
    pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
}

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

/* reads a MIDI variable-length quantity (at most 4 bytes) */
LONG Snd_seq_get_vlq(UCHAR **pp)
{
    LONG v;
    LONG i;
    UCHAR c;

    v = 0;
    i = 3;
loop:
    c = *(*pp)++;
    v <<= 7;
    v |= c & 0x7F;
    if (c & 0x80) {
        if (i-- != 0) {
            goto loop;
        }
    }
    return v;
}

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
    SND_WORK *pW = &Snd_work;
    SND_SEQ *p = &pW->Seq[no];
    SND_TRACK *pT;
    SND_VOICE *pV;
    ULONG i;
    ULONG t;

    p->pData = p->pTop = pData + 0xC;
    p->Unk18 = a3;
    p->Bank = a2;
    p->Unk24 = a2;
    t = *(ULONG *)(pData + 4);
    p->Resolution = *(USHORT *)(pData + 8);
    p->Delta = 0;
    p->Tempo_cnt = p->Tempo = t * 1000 / p->Resolution;
    pT = p->Trk;
    pV = &pW->Voice[8];
    for (i = 0; i < 16; i++) {
        pT->Vol2 = 0x7F;
        pT->Pan = 0x40;
        pT->Bend = 0x2000;
        pT->Mask = 0;
        pT->Bank = a2;
        pT->UnkE = a2;
        if (pV->Unk1D == 0) {
            pV->Unk14 = 0x2000;
        }
        pT++;
        pV++;
    }
    p->Unk38 = 0;
    p->Unk40 = 0;
    p->Unk3C = 0;
    pW->UnkB74 |= 1 << p->No;
    pW->UnkB76 &= ~(1 << no);
    p->Unk2C = 1;
    p->Unk2D = 0;
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
    SND_WORK *pW = &Snd_work;

    mask = 0;
    if (Snd_work.Seq[no].Unk2C != 0) {
        Snd_work.UnkB76 |= 1 << no;
        Snd_work.Seq[no].Unk2C = 0;
        for (i = 0; i < 16; i++) {
            if (pW->Seq[no].Trk[i].Mask != 0) {
                j = 8;
                v = pW->Seq[no].Trk[i].Mask;
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
    if (pEm == G.pEnemy[0]) {
        Snd_se_stad(kind | 0x10200, &v, 0, 0);
    } else {
        Snd_se_stad(kind | 0x10200, &v, -10, -2);
    }
}

UCHAR func_80077B84(LONG x, LONG z, LONG id)
{
    LONG pos[3]; UCHAR *pe; LONG n;
    LONG f;
    pe = (UCHAR *)G.pRoom->pUnk34; n = *(SHORT *)pe; pe += 2; pos[0] = x; pos[2] = z; pe += n * 0xC;
    while (n--) {
        pe -= 0xC;
        f = ((SHORT *)pe)[5];
        if (!(f & 0x100) && id == (f & 0x7F) && func_800101C8(pos, (SHORT *)pe)) {
            return pe[8];
        }
    }
    return 0x1D;
}

/* room BGM request: Unk8C2/Unk8C3 = main/sub sequence ids from the room table, fades out what changes */
void func_80077C4C(void)
{
    tagGLOBAL *pG = &G;
    SND_WORK *pW = &Snd_work;
    tagCD *pCd = &CD;
    SHORT Stage;
    USHORT Bgm;

    Stage = pG->Unk783E;
    if (Stage >= 5) {
        Stage -= 5;
    }
    Bgm = pG->Unk7C84[D_800A0FDC[Stage] + pG->Unk7840];
    pW->UnkB20 = 0x3C00;
    pW->UnkB1C = 0xFF;
    pW->UnkB24 = 0x400;
    if ((Bgm >> 8) != 0xFF) {
        pW->UnkB77 = Bgm >> 15;
    }
    if ((Bgm & 0xFF) != 0xFF) {
        pW->UnkB77 |= (Bgm >> 5) & 6;
    }
    pCd->Unk8C2 = (Bgm >> 8) & 0x3F;
    if ((!(pW->UnkB77 & 1) && pW->Seq_data[0].Id == pCd->Unk8C2) || pW->Seq_data[0].Id != pCd->Unk8C2) {
        if (pW->Seq[0].Unk2C && !pW->Seq[0].Unk2D) {
            if (pW->Seq[0].Unk38 != 2) {
                pW->Seq[0].Unk38 = 2;
                pW->Seq[0].Unk3C = 0x3F00;
            }
            pW->Seq[0].Unk40 = 0x100;
        }
    } else if (pW->Seq[0].Unk2C) {
        pW->UnkB77 &= 6;
    }
    if (pCd->Unk8C2 == 0x3F) {
        pCd->Unk8C2 = 0xFF;
        pW->Seq_data[0].Id = 0xFF;
    }
    pCd->Unk8C3 = Bgm & 0x3F;
    if ((!(pW->UnkB77 & 2) && pW->Seq_data[1].Id == pCd->Unk8C3) || pW->Seq_data[1].Id != pCd->Unk8C3) {
        if (pW->Seq[1].Unk2C && !pW->Seq[1].Unk2D) {
            if (pW->Seq[1].Unk38 != 2) {
                pW->Seq[1].Unk38 = 2;
                pW->Seq[1].Unk3C = 0x3F00;
            }
            pW->Seq[1].Unk40 = 0x100;
        }
    } else if (pW->Seq[1].Unk2C) {
        pW->UnkB77 &= 5;
    }
    if ((!(pW->UnkB77 & 4) && pW->Seq_data[1].Id == pCd->Unk8C3) || pW->Seq_data[1].Id != pCd->Unk8C3) {
        if (pW->Seq[2].Unk2C && !pW->Seq[2].Unk2D) {
            if (pW->Seq[2].Unk38 != 2) {
                pW->Seq[2].Unk38 = 2;
                pW->Seq[2].Unk3C = 0x3F00;
            }
            pW->Seq[2].Unk40 = 0x100;
        }
    } else if (pW->Seq[2].Unk2C) {
        pW->UnkB77 &= 3;
    }
    if (pCd->Unk8C3 == 0x3F) {
        pCd->Unk8C3 = 0xFF;
        pW->Seq_data[1].Id = 0xFF;
        pW->Seq_data[2].Id = 0xFF;
    }
}

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

    p = (LONG *)G.pRoom->pEdt0;
    pW = &Snd_work;
    if (p == NULL) {
        pW->Bank_id[2].Id = -1;
        return;
    }
    *(LONG **)&D_800BA6E8 = p;
    pW->UnkB78 = G.pRoom->Unk6 * 4;
    while (*p != -1) {
        p++;
    }
    p++;
    func_8007836C(2, (ULONG *)p);
    if (G.pRoom->pEdt0 != NULL) {
        G.pEm_mem = (ENEMY_WORK *)(((ULONG)pW->Bank[2].pHead + pW->Bank[2].pHead[1] + 3) & ~3);
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
    G.Unk7C84[D_800A0FDC[i] + b] = c;
}
