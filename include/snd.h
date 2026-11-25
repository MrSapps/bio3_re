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
    UCHAR Note_vol;                    /* 0x0B velocity of the last note on */
    UCHAR Note;                        /* 0x0C */
    UCHAR Tone;                        /* 0x0D tone being keyed (loop index) */
    UCHAR Bank_org;                    /* 0x0E bank given at the sequence start */
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
    LONG Repeat;                       /* 0x018 non-zero: restart at pTop on end of track */
    LONG Running;                      /* 0x01C running status byte */
    LONG Bank;                         /* 0x020 */
    SHORT Bank_org;                    /* 0x024 bank given at the start (Snd_seq_start) */
    SHORT No;                          /* 0x026 sequence slot */
    UCHAR pad28[0x2B - 0x28];
    UCHAR Status;                      /* 0x02B running status >> 4 (0xF: meta event) */
    UCHAR Play_flg;                    /* 0x02C playing */
    UCHAR Stop_req;                    /* 0x02D stop request */
    UCHAR Nrpn;                        /* 0x02E NRPN (controller 99) */
    UCHAR Loop_mode;                   /* 0x02F 2: loop back to pLoop */
    UCHAR Loop_cnt;                    /* 0x030 0x7F: forever */
    UCHAR pad31[0x34 - 0x31];
    UCHAR *pLoop;                      /* 0x034 */
    UCHAR Fade_mode;                   /* 0x038 1 fade in, 2 fade out (Snd_seq_fade_tbl) */
    UCHAR pad39[0x3C - 0x39];
    ULONG Fade_vol;                    /* 0x03C 0..0x3FFF */
    ULONG Fade_step;                   /* 0x040 */
    UCHAR *pReq_data;                  /* 0x044 start request (Snd_seq_req): sequence data */
    LONG Req_no;                       /* 0x048 slot */
    LONG Req_bank;                     /* 0x04C bank id */
    LONG Req_loop;                     /* 0x050 */
    UCHAR Req_flg;                     /* 0x054 1: start request, 2: start after the stop */
    UCHAR pad55;
    USHORT Resolution;                 /* 0x056 ticks per quarter note */
    UCHAR pad58[0x5C - 0x58];
    SND_TRACK Trk[16];                 /* 0x05C */
} SND_SEQ;

/* voice slot (0x20 bytes, 24 of them in SND_WORK) */
typedef struct tagSND_VOICE {
    UCHAR *pVag;                       /* 0x00 VagAtr */
    LONG Time;                         /* 0x04 age in ticks (the oldest voice is stolen) */
    SHORT Vol_r;                       /* 0x08 volumes set on the SPU (Vol_*_base scaled) */
    SHORT Vol_l;                       /* 0x0A */
    SHORT Vol_l_base;                  /* 0x0C unscaled volumes */
    SHORT Vol_r_base;                  /* 0x0E */
    SHORT Ch_vol;                      /* 0x10 track volume the voice volumes were scaled with */
    SHORT Bank_id;                     /* 0x12 */
    USHORT Bend;                       /* 0x14 */
    UCHAR Pan;                         /* 0x16 */
    UCHAR Prog;                        /* 0x17 */
    UCHAR Tone;                        /* 0x18 */
    UCHAR Note;                        /* 0x19 */
    UCHAR Prio;                        /* 0x1A priority (Snd_se_pri_ck) */
    UCHAR Bank;                        /* 0x1B */
    UCHAR Seq_no;                      /* 0x1C sequence that keyed it */
    UCHAR Use;                         /* 0x1D in use */
    UCHAR pad1E[0x20 - 0x1E];
} SND_VOICE;

/* sound request (0x20 bytes, queued by Snd_se_stad) */
typedef struct tagSND_REQ {
    LONG Pan;                          /* 0x00 2D: < 0 left, > 0 right (Snd_se_3D) */
    LONG Atten;                        /* 0x04 2D: attenuation, 0..0x80 */
    VECTOR Pos;                        /* 0x08 */
    SHORT Note_add;                    /* 0x18 */
    SHORT Vol_add;                     /* 0x1A */
    SHORT No;                          /* 0x1C */
    UCHAR Mode;                        /* 0x1E 0 2D pan, 1 3D from the camera */
    UCHAR Bank;                        /* 0x1F */
} SND_REQ;

/* SND_WORK 0x7C0-0x7D3: reverb and volumes (Snd_se_call addresses it through a pointer);
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
    ULONG *pVag;                       /* +4 VagAtr table */
} SND_BANK;

typedef struct tagSND_BANK_ID {
    SHORT Id;                          /* +0 */
    SHORT Size;                        /* +2 in 64-byte units */
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
    LONG Cam_x;                        /* 0xAD8 listener (camera) position (Snd_se_pri_ck) */
    LONG Cam_y;                        /* 0xADC */
    LONG Cam_z;                        /* 0xAE0 */
    UCHAR padAE4[0xAE8 - 0xAE4];
    SpuCommonAttr Cattr;               /* 0xAE8 */
    ULONG Se_key_on;                   /* 0xB10 SE voices to key on this frame */
    ULONG Se_key_off;                  /* 0xB14 SE voices to key off */
    ULONG Rv_voice;                    /* 0xB18 voice bit of the last reverb on/off (Snd_voice_key_on) */
    LONG Fade_mask;                    /* 0xB1C voices under the master fade (0x80 on status/option enter and exit) */
    ULONG Fade_vol;                    /* 0xB20 master fade level (0..0x3FFF) */
    LONG Fade_step;                    /* 0xB24 master fade step */
    ULONG Seq_key_on;                  /* 0xB28 sequence voices to key on */
    ULONG Seq_key_off;                 /* 0xB2C sequence voices to key off */
    ULONG Seq_rv_on;                   /* 0xB30 sequence voices with reverb */
    ULONG Seq_rv_off;                  /* 0xB34 */
    LONG Active;                       /* 0xB38 bit 0: Snd_se_call runs */
    LONG Dist[8];                      /* 0xB3C listener distance per voice group (Snd_se_pri_ck) */
    UCHAR Vprio[24];                   /* 0xB5C voice state: 0/3 may be taken, 1 on key on (Snd_se_voice_set) */
    UCHAR Seq_play;                    /* 0xB74 per-seq bits: playing */
    UCHAR Mono;                        /* 0xB75 mono output */
    UCHAR Seq_pause;                   /* 0xB76 per-seq bits: paused (Snd_seq_pause) */
    UCHAR Bgm_req;                     /* 0xB77 per-seq bits: start the room BGM (Snd_bgm_play_ck) */
    UCHAR Rv_mask;                     /* 0xB78 room reverb (ROOM Unk6 * 4) */
    UCHAR Bgm_cancel;                  /* 0xB79 per-seq bits: cancel Bgm_req */
} SND_WORK;
extern SND_WORK Snd_work;              /* 0x800E0610 */

LONG Snd_pitch_scale(LONG x);
void Snd_seq_stop(SHORT no);
void Snd_work_init(void);
void Snd_seq_stop_all(void);

extern UCHAR Bgm_room_ofs[];
extern UCHAR Vab_head_ofs[];
extern UCHAR Snd_pan_tbl[]; /* pan curve, 0x80 entries */
extern UCHAR Snd_dist_atten[]; /* attenuation by distance / 250 */
extern UCHAR Snd_rv_attr_m1[][0x14];
extern SpuReverbAttr Snd_rv_attr[];
extern void (*Snd_req_tbl[])(SND_REQ *pReq); /* Snd_se_req (SE), Snd_seq_req (sequence) */
extern void (*Snd_seq_fade_tbl[])(SND_SEQ *pSeq); /* Snd_seq_fade_in, Snd_seq_fade_out */
extern LONG Snd_pitch_up_tbl[];
extern LONG Snd_pitch_down_tbl[];
extern void (*Snd_midi_tbl[])(SND_SEQ *pSeq, SND_TRACK *pTr, ULONG Status);
extern UCHAR *Snd_edt; /* [3]: sound bank id of the room */
extern LONG Snd_event;
extern SpuCommonAttr Snd_common_attr;

void Snd_sys_init(void);
void Snd_reset(void);
void Snd_reverb_set(void);
void Snd_se_call(void);
void Snd_cd_mix_set(LONG on); /* RE2 Close_movie: SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SOFF) */
void Snd_se_stad(ULONG code, void *pPos, SHORT Vol_add, SHORT Note_add); /* pPos: x, y, z LONGs (a VECTOR or &Pos_x), may be 0 */
void Snd_se_req(SND_REQ *pReq);
void Snd_seq_req(SND_REQ *pReq);
void Snd_seq_fade_in(SND_SEQ *pSeq);
void Snd_seq_fade_out(SND_SEQ *pSeq);
void Snd_se_voice_set(SND_REQ *pReq, USHORT Id, SND_SE *p, LONG bank);
void Snd_voice_key_on(SND_VOICE *pV, signed char v);
LONG Snd_se_pri_ck(SND_SE *pSe, LONG v, SND_REQ *pReq);
LONG Snd_bank_search(LONG id);
void Snd_set_voice_vol(SHORT l, SHORT r, ULONG voice);
void Snd_se_3D(SND_REQ *pReq, USHORT vol, USHORT *pR, USHORT *pL, SHORT dist);
LONG Snd_se_dir_ck(LONG x0, LONG z0, LONG x1, LONG z1);
UCHAR *Snd_bank_get_tone(LONG no, SHORT idx);
LONG Snd_pitch_calc(LONG x, LONG frac);
LONG Snd_se_vol(UCHAR *pVag, SND_VOICE *pV);
void Snd_pan_vol(SpuVoiceAttr *pAttr, SHORT vol, ULONG pan);
LONG Snd_seq_tick(void);
void Snd_midi_note_off(SND_SEQ *pSeq, SND_TRACK *pTr);
void Snd_seq_key_off(SND_SEQ *pSeq, SND_TRACK *pTr, ULONG mask);
void Snd_midi_note_on(SND_SEQ *pSeq, SND_TRACK *pTr);
void Snd_midi_ctrl(SND_SEQ *pSeq, SND_TRACK *pTr);
void Snd_midi_prog(SND_SEQ *pSeq, SND_TRACK *pTr);
void Snd_midi_bend(SND_SEQ *pSeq, SND_TRACK *pTr);
void Snd_midi_meta(SND_SEQ *pSeq);
void Snd_midi_skip(SND_SEQ *pSeq);
LONG Snd_seq_get_vlq(UCHAR **pp);
ULONG Snd_tone_mask(VagAtr *pVag, SND_TRACK *pTr, UCHAR tones);
LONG Snd_voice_get(LONG lo, ULONG hi);
LONG Snd_seq_voice_search(LONG Seq_no, LONG Prog, LONG Tone, LONG Note);
void Snd_seq_start(LONG no, UCHAR *pData, LONG Bank, LONG Loop);
void Snd_seq_pause(LONG no);
void Snd_seq_play_ck(LONG no);
void Snd_se_enem(ULONG no, ENEMY_WORK *pEm); /* RE2 Snd_se_enem */
void Snd_se_enem_parts(LONG no, UCHAR parts, ENEMY_WORK *pEm);
void Snd_se_walk(LONG kind, ULONG parts, ENEMY_WORK *pEm);
UCHAR Snd_floor_kind(LONG x, LONG z, LONG id);
void Snd_bgm_ck(void);
void Snd_bgm_fade_wait(void);
void Snd_bgm_play_ck(void);
LONG Snd_vab_load(LONG no, LONG idx, ULONG *pAdr, ULONG *pBuf);
void Snd_sys_pset(void);
void Snd_seq_data_set(ULONG no, ULONG id, ULONG *pAdr);
void Snd_vab_head_set(LONG no, ULONG *pAdr);
void Snd_bank_set(ULONG no, ULONG size, ULONG *pBuf); /* pBuf (where the body was read) is unused */
void Snd_sys_enem_set(void);
void Snd_bgm_tbl_set(UCHAR Stage, UCHAR Room, USHORT Bgm);

#endif /* SND_H */
