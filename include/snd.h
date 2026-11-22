#ifndef SND_H
#define SND_H
#include "bio3.h"
#include "em_sub.h"

/* RE3 sound driver (custom sequencer on libspu; RE2 SND.C used libsnd, little carries over) */

/* sequencer track (0x10 bytes, 16 per SND_SEQ) */
typedef struct tagSND_TRACK {
    ULONG Mask;                        /* 0x00 voices keyed by this track */
    USHORT Bend;                       /* 0x04 pitch bend, 0x2000 = centre */
    UCHAR Prog;                        /* 0x06 */
    UCHAR Vol;                         /* 0x07 */
    UCHAR Pan;                         /* 0x08 controller 10 */
    UCHAR Pan2;                        /* 0x09 */
    UCHAR Vol2;                        /* 0x0A controller 7 */
    UCHAR UnkB;                        /* 0x0B */
    UCHAR Note;                        /* 0x0C */
    UCHAR UnkD;                        /* 0x0D */
    UCHAR UnkE;                        /* 0x0E */
    UCHAR Bank;                        /* 0x0F */
} SND_TRACK;

/* sequence slot (0x15C bytes, 3 of them in SND_WORK) */
typedef struct tagSND_SEQ {
    ULONG *pVh;                        /* 0x000 bank header */
    UCHAR *pData;                      /* 0x004 read pointer */
    UCHAR *pTop;                       /* 0x008 restart point */
    LONG Delta;                        /* 0x00C ticks to the next event */
    LONG Tempo;                        /* 0x010 */
    LONG Tempo_cnt;                    /* 0x014 */
    LONG Unk18;                        /* 0x018 non-zero: restart at pTop on end of track */
    LONG Running;                      /* 0x01C running status byte */
    LONG Bank;                         /* 0x020 */
    SHORT Unk24;                       /* 0x024 bank (func_800772A0) */
    SHORT No;                          /* 0x026 sequence slot */
    UCHAR pad28[0x2B - 0x28];
    UCHAR Status;                      /* 0x02B running status >> 4 (0xF: meta event) */
    UCHAR Unk2C;                       /* 0x02C playing */
    UCHAR Unk2D;                       /* 0x02D stop request */
    UCHAR Nrpn;                        /* 0x02E NRPN (controller 99) */
    UCHAR Loop_mode;                   /* 0x02F 2: loop back to pLoop */
    UCHAR Loop_cnt;                    /* 0x030 0x7F: forever */
    UCHAR pad31[0x34 - 0x31];
    UCHAR *pLoop;                      /* 0x034 */
    UCHAR Unk38;                       /* 0x038 fade mode: 1 in, 2 out (fn table 0x800A1138) */
    UCHAR pad39[0x3C - 0x39];
    ULONG Unk3C;                       /* 0x03C fade volume, 0..0x3FFF */
    ULONG Unk40;                       /* 0x040 fade step */
    UCHAR *pReq_data;                  /* 0x044 start request (func_80074820): sequence data */
    LONG Req_no;                       /* 0x048 slot */
    LONG Req_bank;                     /* 0x04C bank id */
    LONG Req_loop;                     /* 0x050 */
    UCHAR Unk54;                       /* 0x054 1: start request, 2: start after the stop */
    UCHAR pad55;
    USHORT Resolution;                 /* 0x056 ticks per quarter note */
    UCHAR pad58[0x5C - 0x58];
    SND_TRACK Trk[16];                 /* 0x05C */
} SND_SEQ;

/* voice slot (0x20 bytes, 24 of them in SND_WORK) */
typedef struct tagSND_VOICE {
    UCHAR *pVag;                       /* 0x00 VagAtr */
    LONG Unk4;                         /* 0x04 */
    SHORT Unk8;                        /* 0x08 */
    SHORT UnkA;                        /* 0x0A */
    SHORT UnkC;                        /* 0x0C */
    SHORT UnkE;                        /* 0x0E */
    SHORT Unk10;                       /* 0x10 track volume the voice volumes were scaled with */
    SHORT Unk12;                       /* 0x12 */
    USHORT Unk14;                      /* 0x14 */
    UCHAR Unk16;                       /* 0x16 */
    UCHAR Unk17;                       /* 0x17 */
    UCHAR Unk18;                       /* 0x18 */
    UCHAR Unk19;                       /* 0x19 */
    UCHAR Prio;                        /* 0x1A priority (func_80074F44) */
    UCHAR Unk1B;                       /* 0x1B */
    UCHAR Unk1C;                       /* 0x1C */
    UCHAR Unk1D;                       /* 0x1D in use */
    UCHAR pad1E[0x20 - 0x1E];
} SND_VOICE;

/* sound request (0x20 bytes, queued by Snd_se_stad) */
typedef struct tagSND_REQ {
    LONG Pan;                          /* 0x00 2D: < 0 left, > 0 right (func_80075154) */
    LONG Atten;                        /* 0x04 2D: attenuation, 0..0x80 */
    VECTOR Pos;                        /* 0x08 */
    SHORT Unk18;                       /* 0x18 */
    SHORT Unk1A;                       /* 0x1A */
    SHORT No;                          /* 0x1C */
    UCHAR Unk1E;                       /* 0x1E */
    UCHAR Bank;                        /* 0x1F */
} SND_REQ;

/* SND_WORK 0x7C0-0x7D3: reverb and volumes (func_800741A0 addresses it through a pointer);
   the *_old copies let it rescale the voices when a volume changes */
typedef struct tagSND_VOL {
    UCHAR Rv_mode;                     /* 0x00 requested reverb mode */
    UCHAR Rv_cur;                      /* 0x01 current reverb mode */
    UCHAR Rv_step;                     /* 0x02 */
    signed char Rv_cnt;                /* 0x03 */
    SHORT Master;                      /* 0x04 master volume (0..127), SE and BGM */
    SHORT Unk6;                        /* 0x06 */
    SHORT Se;                          /* 0x08 SE volume (0..127) */
    SHORT Bgm;                         /* 0x0A BGM volume (0..127) */
    SHORT Master_old;                  /* 0x0C */
    SHORT UnkE;                        /* 0x0E */
    SHORT Se_old;                      /* 0x10 */
    SHORT Bgm_old;                     /* 0x12 */
} SND_VOL;

/* sound effect record (4 bytes, pointed to by the bank's SE table) */
typedef struct tagSND_SE {
    UCHAR Flg;                         /* 0 bit 0: program, 0x20: no 3D volume, >> 6: voices - 1 */
    UCHAR Tone;                        /* 1 >> 4: first tone, & 7: priority, & 8: don't replace equal priority */
    UCHAR Voice;                       /* 2 & 0x1F: first voice, 0x80: search a free voice with Mask */
    UCHAR Mask;                        /* 3 candidate voices */
} SND_SE;

/* sound driver work */
typedef struct tagSND_BANK {
    ULONG *pHead;                      /* +0 */
    ULONG *Unk4;                       /* +4 */
} SND_BANK;

typedef struct tagSND_BANK_ID {
    SHORT Id;                          /* +0 */
    SHORT pad2;
    UCHAR *pData;                      /* +4 8 byte records */
} SND_BANK_ID;

typedef struct tagSND_SEQ_DATA {
    UCHAR Id;                          /* +0 */
    UCHAR Unk1;                        /* +1 */
    UCHAR pad2[2];
    ULONG *pData;                      /* +4 */
} SND_SEQ_DATA;

typedef struct tagSND_WORK {
    ULONG *pAdr[5];                    /* 0x000 bank file addresses */
    SND_BANK Bank[8];                  /* 0x014 */
    SND_BANK_ID Bank_id[8];            /* 0x054 */
    SND_SEQ_DATA Seq_data[3];          /* 0x094 */
    SND_SEQ Seq[3];                    /* 0x0AC */
    SND_VOICE Voice[24];               /* 0x4C0 */
    SND_VOL Vol;                       /* 0x7C0 reverb and volumes */
    SND_REQ Req[24];                   /* 0x7D4 request queue */
    struct tagSND_REQ *pReq;          /* 0xAD4 request queue write pointer; the queue ends here */
    LONG Cam_x;                        /* 0xAD8 listener (camera) position (func_80074F44) */
    LONG Cam_y;                        /* 0xADC */
    LONG Cam_z;                        /* 0xAE0 */
    UCHAR padAE4[0xAE8 - 0xAE4];
    SpuCommonAttr Cattr;               /* 0xAE8 */
    ULONG UnkB10;                      /* 0xB10 voice mask (set on key on, func_800749A0) */
    ULONG UnkB14;                      /* 0xB14 voice mask (set on key on, func_800749A0) */
    ULONG UnkB18;                      /* 0xB18 voice bit of the last reverb on/off (func_80074CD0) */
    LONG UnkB1C;                       /* 0xB1C volume fade frames (0x80 on option screen enter/exit) */
    ULONG UnkB20;                      /* 0xB20 volume fade level (0..0x3FFF) */
    LONG UnkB24;                       /* 0xB24 volume fade step */
    ULONG UnkB28;                      /* 0xB28 voice mask (key on) */
    ULONG UnkB2C;                      /* 0xB2C voice mask */
    ULONG UnkB30;                      /* 0xB30 voice mask */
    ULONG UnkB34;                      /* 0xB34 voice mask */
    LONG UnkB38;                       /* 0xB38 */
    LONG Dist[8];                      /* 0xB3C listener distance per voice group (func_80074F44) */
    UCHAR Vprio[24];                   /* 0xB5C voice state: 0/3 may be taken, 1 on key on (func_800749A0) */
    UCHAR UnkB74;                      /* 0xB74 per-seq bits: playing */
    UCHAR UnkB75;                      /* 0xB75 mono */
    UCHAR UnkB76;                      /* 0xB76 per-seq bit mask */
    UCHAR UnkB77;                      /* 0xB77 per-seq bits: play the stop jingle (func_80077F60) */
    UCHAR UnkB78;                      /* 0xB78 */
    UCHAR UnkB79;                      /* 0xB79 per-seq bits: cancel UnkB77 */
} SND_WORK;
extern SND_WORK Snd_work;              /* 0x800E0610 */

LONG func_800759D8(LONG x);
void Snd_seq_stop(SHORT no);
void Snd_work_init(void);
void Snd_seq_stop_all(void);

extern UCHAR D_800A0FDC[];
extern UCHAR D_800A0FE4[];
extern ULONG D_800A1000;
extern UCHAR D_800A1008[]; /* pan curve, 0x80 entries */
extern UCHAR D_800A1088[]; /* attenuation by distance / 250 */
extern UCHAR D_800A10F4[][0x14];
extern SpuReverbAttr D_800A1108[];
extern void (*D_800A1130[])(SND_REQ *pReq); /* func_80074770 (SE), func_80074820 (sequence) */
extern void (*D_800A1138[])(SND_SEQ *pSeq); /* Snd_seq_fade_in, Snd_seq_fade_out */
extern LONG D_800A1140[];
extern LONG D_800A1170[];
extern void (*D_800A11A0[])(SND_SEQ *pSeq, SND_TRACK *pTr, ULONG Status);
extern UCHAR *D_800BA6E8; /* [3]: sound bank id of the room */
extern ULONG D_800CC858;
extern LONG D_800E0608;
extern SHORT D_800E067C;
extern SpuCommonAttr D_800E10F8;

void func_80073E84(void);
void Snd_reset(void);
void func_800740AC(void);
void func_800741A0(void);
void func_80074658(LONG on); /* RE2 Close_movie: SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SOFF) */
void Snd_se_stad(ULONG code, VECTOR *pPos, SHORT a2, SHORT a3);
void func_80074770(SND_REQ *pReq);
void func_80074820(SND_REQ *pReq);
void Snd_seq_fade_in(SND_SEQ *pSeq);
void Snd_seq_fade_out(SND_SEQ *pSeq);
void func_800749A0(SND_REQ *pReq, USHORT Id, SND_SE *p, LONG bank);
void func_80074CD0(SND_VOICE *pV, signed char v);
LONG func_80074F44(SND_SE *pSe, LONG v, SND_REQ *pReq);
LONG Snd_bank_search(LONG id);
void Snd_set_voice_vol(SHORT l, SHORT r, ULONG voice);
void func_80075154(SND_REQ *pReq, USHORT vol, USHORT *pR, USHORT *pL, SHORT dist);
LONG func_8007592C(LONG x0, LONG z0, LONG x1, LONG z1);
UCHAR *Snd_bank_get_tone(LONG no, SHORT idx);
LONG func_80075A80(LONG x, LONG frac);
LONG func_80075AE4(UCHAR *pVag, SND_VOICE *pV);
void Snd_pan_vol(SpuVoiceAttr *pAttr, SHORT vol, ULONG pan);
void func_80075C30(void);
void func_80076114(UCHAR *pSeq, UCHAR *pTr);
void func_800762C8(UCHAR *pSeq, UCHAR *pTr, ULONG mask);
void func_8007638C(SND_SEQ *pSeq, SND_TRACK *pTr);
void func_8007688C(SND_SEQ *pSeq, SND_TRACK *pTr);
void func_80076BAC(UCHAR *pSeq, UCHAR *pTr);
void func_80076C64(SND_SEQ *pSeq, SND_TRACK *pTr);
void func_80076E7C(SND_SEQ *pSeq);
void func_80077030(LONG *p);
LONG Snd_seq_get_vlq(UCHAR **pp);
ULONG Snd_tone_mask(VagAtr *pVag, SND_TRACK *pTr, UCHAR tones);
signed char func_800770DC(LONG lo, ULONG hi);
LONG func_80077220(LONG a0, LONG a1, LONG a2, LONG a3);
void func_800772A0(LONG no, UCHAR *pData, LONG a2, LONG a3); /* MIDI event handlers, status 0x80..0xF0 */
void func_8007745C(LONG no);
void Snd_seq_play_ck(LONG no);
void Snd_se_enem(ULONG no, ENEMY_WORK *pEm); /* RE2 Snd_se_enem */
void func_80077618(LONG no, UCHAR parts, ENEMY_WORK *pEm);
void Snd_se_walk(LONG kind, ULONG parts, ENEMY_WORK *pEm);
UCHAR func_80077B84(LONG x, LONG z, LONG id);
void func_80077C4C(void);
void func_80077ED4(void);
void func_80077F60(void);
LONG func_8007809C(LONG no, LONG idx, ULONG *pAdr, ULONG *pBuf);
void func_80078204(void);
void func_800782F4(ULONG no, ULONG id, ULONG *pAdr);
void func_8007836C(LONG no, ULONG *pAdr);
void func_800783BC(ULONG no, ULONG size);
void func_80078450(void);
void func_80078498(UCHAR a, UCHAR b, SHORT c);

#endif /* SND_H */
