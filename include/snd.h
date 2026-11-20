#ifndef SND_H
#define SND_H
#include "bio3.h"

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
    ULONG Tempo;                       /* 0x010 */
    ULONG Tempo_cnt;                   /* 0x014 */
    LONG Unk18;                        /* 0x018 non-zero: restart at pTop on end of track */
    UCHAR pad1C[0x20 - 0x1C];
    LONG Bank;                         /* 0x020 */
    UCHAR pad24[0x26 - 0x24];
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
    UCHAR pad44[0x54 - 0x44];
    UCHAR Unk54;                       /* 0x054 */
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
    UCHAR pad1A;
    UCHAR Unk1B;                       /* 0x1B */
    UCHAR Unk1C;                       /* 0x1C */
    UCHAR Unk1D;                       /* 0x1D in use */
    UCHAR pad1E[0x20 - 0x1E];
} SND_VOICE;

/* sound request (0x20 bytes, queued by Snd_se_stad) */
typedef struct tagSND_REQ {
    UCHAR pad0[8];
    VECTOR Pos;                        /* 0x08 */
    SHORT Unk18;                       /* 0x18 */
    SHORT Unk1A;                       /* 0x1A */
    SHORT No;                          /* 0x1C */
    UCHAR Unk1E;                       /* 0x1E */
    UCHAR Bank;                        /* 0x1F */
} SND_REQ;

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
    UCHAR pad7C0[0x7C4 - 0x7C0];
    SHORT Unk7C4;                      /* 0x7C4 */
    UCHAR pad7C6[0x7C8 - 0x7C6];
    SHORT Unk7C8;                      /* 0x7C8 master volume (0..127) */
    SHORT Unk7CA;                      /* 0x7CA */
    UCHAR pad7CC[0x7D4 - 0x7CC];
    SND_REQ Req[24];                   /* 0x7D4 request queue */
    struct tagSND_REQ *pReq;          /* 0xAD4 request queue write pointer; the queue ends here */
    UCHAR padAD8[0xB1C - 0xAD8];
    LONG UnkB1C;                       /* 0xB1C volume fade frames (0x80 on option screen enter/exit) */
    LONG UnkB20;                       /* 0xB20 volume fade level */
    LONG UnkB24;                       /* 0xB24 volume fade step */
    ULONG UnkB28;                      /* 0xB28 voice mask (key on) */
    ULONG UnkB2C;                      /* 0xB2C voice mask */
    ULONG UnkB30;                      /* 0xB30 voice mask */
    ULONG UnkB34;                      /* 0xB34 voice mask */
    LONG UnkB38;                       /* 0xB38 */
    UCHAR padB3C[0xB74 - 0xB3C];
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

#endif /* SND_H */
