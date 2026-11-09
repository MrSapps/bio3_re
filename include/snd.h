#ifndef SND_H
#define SND_H
#include "bio3.h"

/* RE3 sound driver (custom sequencer on libspu; RE2 SND.C used libsnd, little carries over) */

/* sequencer track (only the fields matched code uses) */
typedef struct tagSND_TRACK {
    UCHAR pad0[0xC];
    UCHAR Note;                        /* 0x0C */
} SND_TRACK;

/* sequence slot (0x15C bytes, 3 of them in SND_WORK) */
typedef struct tagSND_SEQ {
    UCHAR pad0[0x2C];
    UCHAR Unk2C;                       /* 0x02C playing */
    UCHAR Unk2D;                       /* 0x02D stop request */
    UCHAR pad2E[0x38 - 0x2E];
    UCHAR Unk38;                       /* 0x038 fade mode: 1 in, 2 out (fn table 0x800A1138) */
    UCHAR pad39[0x3C - 0x39];
    ULONG Unk3C;                       /* 0x03C fade volume, 0..0x3FFF */
    ULONG Unk40;                       /* 0x040 fade step */
    UCHAR pad44[0x15C - 0x44];
} SND_SEQ;

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

typedef struct tagSND_WORK {
    UCHAR pad0[0x14];
    SND_BANK Bank[8];                  /* 0x014 */
    SND_BANK_ID Bank_id[8];            /* 0x054 */
    UCHAR pad94[0xAC - 0x94];
    SND_SEQ Seq[3];                    /* 0x0AC */
    UCHAR pad4C0[0xB76 - 0x4C0];
    UCHAR UnkB76;                      /* 0xB76 per-seq bit mask */
} SND_WORK;
extern SND_WORK Snd_work;              /* 0x800E0610 */

LONG func_800759D8(LONG x);
void Snd_seq_stop(SHORT no);
void Snd_work_init(void);
void Snd_seq_stop_all(void);

#endif /* SND_H */
