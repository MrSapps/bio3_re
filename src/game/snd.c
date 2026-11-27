/*
 * SND.C: RE3's own sound driver on libspu (RE2 used libsnd; only the role of the entry points carries over).
 * Sound effects are queued by Snd_se_stad and keyed by Snd_se_call once per frame; BGM sequences (3 slots,
 * SEQ data) are played by a root-counter driven sequencer (Snd_seq_tick, Snd_midi_*). VAB banks live in
 * SPU memory, registered by Snd_vab_head_set/Snd_bank_set; the room's BGM and banks come in with the room pack (cd.c).
 */
#include "snd.h"
#include "cd.h"
#include "pad.h"
#include "player.h"
#include "sca.h"
#include "sld.h"

#define SQ(x) ((x) * (x))

/* RE2 Snd_sys_init role: SPU common attributes, volumes, work, reverb off, and the sequencer root counter event */
void Snd_sys_init(void)
{
    SpuReverbAttr Rv_attr;
    SpuCommonAttr *pCattr;
    SND_WORK *pW = &Snd_work;

    SsUtReverbOff();
    pCattr = &pW->Cattr;
    pW->Cattr.mask = 0x2CF;
    pCattr->mvol.right = 0x3FFF;
    pCattr->mvol.left = 0x3FFF;
    pCattr->mvolmode.right = 0;
    pCattr->mvolmode.left = 0;
    pCattr->cd.volume.right = 0x1FFF;
    pCattr->cd.volume.left = 0x1FFF;
    pCattr->cd.mix = 1;
    SpuSetCommonAttr(pCattr);
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
    Rv_attr.mask = 1;
    Rv_attr.mode = 0;
    SpuSetReverbModeParam(&Rv_attr);
    SpuSetReverb(0);
    VSync(30);
    StopRCnt(0xF2000002);
    EnterCriticalSection();
    Snd_event = OpenEvent(0xF2000002, 2, 0x1000, Snd_seq_tick);
    ExitCriticalSection();
    SetRCnt(0xF2000002, 20000, 0x1000);
    EnableEvent(Snd_event);
    StartRCnt(0xF2000002);
    pW->Seq_play = 0;
}

/* stops every sequence and voice and clears the work */
void Snd_reset(void)
{
    Snd_seq_stop_all();
    Snd_work_init();
}

/* clears the sound work (sequences, voices, request queue, master fade) */
void Snd_work_init(void)
{
    SND_WORK *pW = &Snd_work;
    LONG i, j;

    for (i = 0; i < 8; i++) {
        pW->Bank_id[i].Id = -1;
    }
    for (i = 0; i < 3; i++) {
        pW->Seq_data[i].Id = 0xFF;
        pW->Seq[i].Req_flg = 0;
        pW->Seq[i].Fade_mode = 0;
        for (j = 0; j < 16; j++) {
            pW->Seq[i].Trk[j].Mask = 0;
        }
    }
    for (i = 0; i < 24; i++) {
        pW->Voice[i].Use = 0;
        pW->Voice[i].Prio = 0;
        pW->Voice[i].Time = 0;
        pW->Req[i].Note_add = 0;
        pW->Req[i].Vol_add = 0;
    }
    pW->Active = 1;
    pW->Fade_mask = 0;
    pW->Fade_vol = 0;
    pW->Fade_step = 0;
    pW->Rv_mask = 0;
    pW->Seq_pause = 0;
    pW->Bgm_req = 0;
    pW->pReq = pW->Req;
}

/* changes the reverb mode in two steps (clear the work area, then the parameters 30 frames later) */
void Snd_reverb_set(void)
{
    if (Snd_work.Vol.Rv_step == 0) {
        SpuReserveReverbWorkArea(1);
        SpuClearReverbWorkArea(Snd_rv_attr[Snd_work.Vol.Rv_mode - 1].mode);
        Snd_work.Vol.Rv_cnt = 30;
        Snd_work.Vol.Rv_step++;
    } else if (--Snd_work.Vol.Rv_cnt == 0) {
        SpuSetReverbModeParam((SpuReverbAttr *)Snd_rv_attr_m1[Snd_work.Vol.Rv_mode]);
        SpuSetReverbDepth((SpuReverbAttr *)Snd_rv_attr_m1[Snd_work.Vol.Rv_mode]);
        SpuSetReverb(1);
        Snd_work.Vol.Rv_step = 0;
        Snd_work.Vol.Rv_cur = Snd_work.Vol.Rv_mode;
        Snd_work.Vol.Rv_mode = 0;
    }
}

/* per frame: reverb, master fade, volume changes, the request queue, sequence requests and fades, SE key on/off */
void Snd_se_call(void)
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
    if (!(pW->Active & 1)) {
        return;
    }
    if (pW->Vol.Rv_mode && pW->Vol.Rv_mode != pW->Vol.Rv_cur) {
        Snd_reverb_set();
    }
    if (pW->Fade_mask) {
        if (pW->Fade_step >= 0) {
            if (pW->Fade_step >= pW->Fade_vol) {
                pW->Fade_vol = 0;
                pW->Fade_step = 0;
            } else {
                pW->Fade_vol -= pW->Fade_step;
            }
        } else {
            if (pW->Fade_vol >= pW->Fade_step + 0x3FFF) {
                pW->Fade_vol = 0x3FFF;
                pW->Fade_step = 0;
            } else {
                pW->Fade_vol -= pW->Fade_step;
            }
        }
        Flg = pW->Fade_mask;
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
                pV->Vol_r = pV->Vol_r_base * pVol->Master / 127;
                pV->Vol_l = pV->Vol_l_base * pVol->Master / 127;
                if (i < 8) {
                    if (pW->Fade_mask == 0) {
                        pV->Vol_r = pV->Vol_r_base * pVol->Se / 127;
                        pV->Vol_l = pV->Vol_l_base * pVol->Se / 127;
                    } else {
                        pV->Vol_r = pV->Vol_r_base * pW->Fade_vol / 0x3FFF;
                        pV->Vol_l = pV->Vol_l_base * pW->Fade_vol / 0x3FFF;
                    }
                } else {
                    pV->Vol_r = pV->Vol_r_base * pVol->Bgm / 127;
                    pV->Vol_l = pV->Vol_l_base * pVol->Bgm / 127;
                }
                SpuSetVoiceVolume(i, pV->Vol_l, pV->Vol_r);
            }
            pV++;
            Flg >>= 1;
            i++;
        } while (Flg);
        if (pW->Fade_vol == 0 || pW->Fade_vol == 0x3FFF) {
            pW->Fade_mask = 0;
        }
    }
    pW->Se_key_on = pW->Se_key_off = pW->Rv_voice = 0;
    SpuRGetAllKeysStatus(0, 23, pW->Vprio);
    for (pReq = pW->Req; pReq != pW->pReq; pReq++) {
        Snd_req_tbl[pReq->Bank >= 5](pReq);
    }
    pW->pReq = pW->Req;
    pSeq = pW->Seq;
    for (i = 0; i < 3; i++) {
        if (pSeq->Req_flg == 1) {
            Snd_seq_start(pSeq->Req_no, pSeq->pReq_data, pSeq->Req_bank, pSeq->Req_loop);
            pSeq->Req_flg = 0;
        }
        if (pSeq->Fade_mode) {
            Snd_seq_fade_tbl[pSeq->Fade_mode - 1](pSeq);
        }
        pSeq++;
    }
    SpuSetKey(0, pW->Se_key_off);
    pV = pW->Voice;
    i = 0;
    Flg = pW->Se_key_on;
    do {
        if (Flg & 1) {
            Snd_voice_key_on(pV, i);
        }
        Flg >>= 1;
        i++;
        pV++;
    } while (i < 8);
    SpuSetKey(1, pW->Se_key_on);
}

/* turns CD audio mixing into the SPU output on or off */
void Snd_cd_mix_set(LONG on)
{
    SpuCommonAttr *p = &Snd_common_attr;

    if (on) {
        p->cd.volume.right = 0x7FFF;
        p->cd.volume.left = 0x7FFF;
        p->cd.mix = 1;
        Snd_common_attr.mask |= 0x2C0;
    } else {
        p->cd.volume.left = 0;
        p->cd.volume.right = 0;
        p->cd.mix = 0;
        Snd_common_attr.mask &= ~0x2C0;
    }
    SpuSetCommonAttr(p);
}

/* queues a sound request: code = Mode << 16 | Bank << 8 | No, at pPos (RE2 Snd_se_stad) */
void Snd_se_stad(ULONG code, void *pPos, SHORT Vol_add, SHORT Note_add)
{
    if (Snd_work.pReq != (SND_REQ *)&Snd_work.pReq && (!(G.System_flg & 0x10000000) || ((code >> 8) & 0xFF) < 5)) {
        Snd_work.pReq->Mode = code >> 16;
        Snd_work.pReq->Bank = code >> 8;
        Snd_work.pReq->No = code & 0xFF;
        Snd_work.pReq->Pos = *(VECTOR *)pPos;
        Snd_work.pReq->Vol_add = Vol_add;
        Snd_work.pReq->Note_add = Note_add;
        Snd_work.pReq++;
    }
}

/* request handler for banks 0..4: plays the SE record of the bank */
void Snd_se_req(SND_REQ *pReq)
{
    ULONG *pSe;
    LONG Bank;

    pSe = Snd_work.pAdr[pReq->Bank] + (USHORT)pReq->No;
    if (*pSe != -1 && Snd_bank_search(pReq->Bank) != -1) {
        Bank = Snd_bank_search(((UCHAR)*pSe & 0xE) >> 1);
        if (Bank != -1) {
            Snd_se_voice_set(pReq, Snd_work.Bank_id[Bank].Id, (SND_SE *)pSe, Bank);
        }
    }
}

/* request handler for banks 5..: queues a sequence start */
void Snd_seq_req(SND_REQ *pReq)
{
    LONG Seq_no;
    LONG Bank;
    LONG Kind;
    SND_SEQ *pSeq;
    SND_WORK *pW;

    Kind = 5;
    pW = &Snd_work;
    Seq_no = pReq->Bank - 5;
    if (Seq_no) {
        Kind = 6;
    }
    Bank = Snd_bank_search(Kind);
    if (Bank != -1) {
        pSeq = &pW->Seq[Seq_no];
        pSeq->Bank = Bank;
        pSeq->No = Seq_no;
        if (pSeq->Play_flg) {
            Snd_seq_stop(Seq_no);
            pSeq->Req_flg = 2;
        } else {
            pSeq->Req_flg = 1;
        }
        pSeq->pVh = pW->Bank[Seq_no ? 6 : 5].pHead;
        pSeq->Req_no = pSeq->No;
        pSeq->pReq_data = (UCHAR *)pW->Seq_data[Seq_no].pData;
        pSeq->Req_bank = pW->Bank_id[Seq_no ? 6 : 5].Id;
        pSeq->Req_loop = 0;
    }
}

/* sequence fade in step */
void Snd_seq_fade_in(SND_SEQ *pSeq)
{
    if (pSeq->Play_flg != 0) {
        if (pSeq->Fade_vol >= 0x3FFF - pSeq->Fade_step) {
            pSeq->Fade_vol = 0x3FFF;
            pSeq->Fade_mode = 0;
        } else {
            pSeq->Fade_vol += pSeq->Fade_step;
        }
    }
}

/* sequence fade out step; stops the sequence at 0 */
void Snd_seq_fade_out(SND_SEQ *pSeq)
{
    if (pSeq->Play_flg != 0) {
        if (pSeq->Fade_vol <= pSeq->Fade_step) {
            pSeq->Fade_vol = 0;
            pSeq->Fade_mode = 0;
        pSeq->Stop_req = 1;
            pSeq->Stop_req = 1;
        } else {
            pSeq->Fade_vol -= pSeq->Fade_step;
        }
    }
}

/* picks the voices of one SE and sets tone, priority, volume and the key-on bits */
void Snd_se_voice_set(SND_REQ *pReq, USHORT Id, SND_SE *p, LONG bank)
{
    SND_WORK *pW = &Snd_work;
    SND_VOICE *pV;
    VagAtr *pVag;
    ULONG Voice;
    ULONG mask;
    ULONG bit;
    LONG vol;
    signed char Tone;

    Tone = 0;
    if (!(p->Voice & 0x80)) {
        Voice = p->Voice & 0x1F;
    } else {
        Voice = p->Voice & 0x1F;
        mask = p->Mask;
        for (; Tone < 8; Tone++) {
            if (mask & 1) {
                if (pW->Vprio[Voice + Tone] == 0 || pW->Vprio[Voice + Tone] == 3) {
                    Voice += Tone;
                    break;
                }
            }
            mask >>= 1;
        }
        Tone = 0;
    }
    do {
        if (Snd_se_pri_ck(p, Voice, pReq) == 0) {
            pV = &pW->Voice[Voice + Tone];
            pV->Prog = p->Flg & 1;
            pV->Tone = Tone + (p->Tone >> 4);
            pV->Prio = p->Tone & 7;
            pV->Bank = bank;
            pV->Bank_id = Id;
            pVag = (VagAtr *)pW->Bank[bank].pVag + (pV->Tone + pV->Prog * pW->Bank[pV->Bank].pHead[3]);
            pV->Note = pVag->min;
            if (pReq->Note_add != 0) {
                pV->Note += pReq->Note_add;
                pReq->Note_add = 0;
            }
            if (!(p->Flg & 0x20)) {
                vol = Snd_se_vol((UCHAR *)pVag, pV);
                if (pReq->Vol_add != 0) {
                    vol += pReq->Vol_add;
                    pReq->Vol_add = 0;
                }
                Snd_se_3D(pReq, vol, (USHORT *)&pV->Vol_l, (USHORT *)&pV->Vol_r, pW->Dist[Voice]);
            } else {
                vol = Snd_se_vol((UCHAR *)pVag, pV);
                if (pReq->Vol_add != 0) {
                    vol += pReq->Vol_add;
                    pReq->Vol_add = 0;
                }
                Snd_pan_vol((SpuVoiceAttr *)pV, vol, pVag->pan);
                pV->Vol_l = (pV->Vol_l * pV->Vol_l * 0x2080F) >> 17;
                pV->Vol_r = (pV->Vol_r * pV->Vol_r * 0x2080F) >> 17;
            }
            pV->Bend = 0;
            pV->Vol_l_base = pV->Vol_l;
            pV->Vol_r_base = pV->Vol_r;
            bit = 1 << (Voice + Tone);
            pW->Se_key_on |= bit;
            pW->Se_key_off |= bit;
            pW->Vprio[Voice + Tone] = 1;
        }
        Tone++;
    } while (Tone < (p->Flg >> 6) + 1);
}

/* fills a SpuVoiceAttr (reverb, pitch with bend, ADSR, volume) for voice v */
void Snd_voice_key_on(SND_VOICE *pV, signed char v)
{
    SpuVoiceAttr Attr;
    SND_WORK *pW = &Snd_work;
    VagAtr *pVag;
    ULONG Note;
    ULONG Pitch;

    pVag = (VagAtr *)pW->Bank[pV->Bank].pVag + (pV->Tone + pV->Prog * pW->Bank[pV->Bank].pHead[3]);
    pW->Rv_voice = 0;
    if (pV->Bank < 5) {
        if (pVag->mode & pW->Rv_mask) {
            pW->Rv_voice = 1 << v;
            SpuSetReverbVoice(1, pW->Rv_voice);
        } else {
            pW->Rv_voice = 1 << v;
            SpuSetReverbVoice(0, pW->Rv_voice);
        }
    }
    Attr.voice = 1 << v;
    Attr.mask = 0x60093;
    Attr.addr = (ULONG)Snd_bank_get_tone(pV->Bank, pVag->vag);
    Note = pVag->shift;
    Pitch = (pV->Note << 7) | Note;
    Note = Pitch;
    if (pV->Bend > 0x2000) {
        Note += ((pV->Bend - 0x2000) << 7) * pVag->pbmax / 0x1FFF;
    }
    if (pV->Bend < 0x2000) {
        Note -= ((0x2000 - pV->Bend) << 7) * pVag->pbmin / 0x1FFF;
    }
    Attr.pitch = Snd_pitch_calc(((Note & 0x3F80) >> 7) - pVag->center, Note & 0x7F);
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
    Attr.volume.left = pV->Vol_l;
    Attr.volume.right = pV->Vol_r;
    SpuSetVoiceAttr(&Attr);
}

/* may the SE take voice v (priority check, RE2 Snd_se_pri_ck role); 3D requests store the distance */
LONG Snd_se_pri_ck(SND_SE *pSe, LONG v, SND_REQ *pReq)
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
    if (pReq->Mode) {
        pW->Cam_x = G.pRoom->pRcut[G.Cut_no].View_p[0];
        pW->Cam_y = G.pRoom->pRcut[G.Cut_no].View_p[1];
        pW->Cam_z = G.pRoom->pRcut[G.Cut_no].View_p[2];
        dx = (pW->Cam_x - pReq->Pos.vx) >> 1;
        dz = (pW->Cam_z - pReq->Pos.vz) >> 1;
        dy = (pW->Cam_y - pReq->Pos.vy) >> 1;
        d = SquareRoot0(SQ(SquareRoot0(dx * dx + dz * dz)) + dy * dy) * 2;
        if (d > 0xEA5F) {
            d = 0xEA5F;
        }
        pW->Dist[v] = d;
    }
    return 0;
}

/* slot of the bank with id, or -1 */
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

/* sets the volume of a voice */
void Snd_set_voice_vol(SHORT l, SHORT r, ULONG voice)
{
    SpuVoiceAttr attr;

    attr.volume.left = l;
    attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR;
    attr.voice = voice;
    attr.volume.right = r;
    SpuSetVoiceAttr(&attr);
}

/* volumes of a request: 2D pan, or 3D from the camera direction and distance (RE2 Snd_se_3D) */
void Snd_se_3D(SND_REQ *pReq, USHORT vol, USHORT *pR, USHORT *pL, SHORT dist)
{
    SND_WORK *pW = &Snd_work;
    VECTOR V;
    UCHAR Flg;                         /* never set: the original tests an uninitialised stack byte */
    RCUT *pCut;
    VECTOR *pPos;
    LONG Dist_idx;
    LONG Dir_src;
    LONG Dir_view;
    LONG d;
    UCHAR l;
    UCHAR r;
    UCHAR *pAtt;
    LONG Sum;
    LONG k;

    vol = (vol * vol * 0x2080F) >> 17;
    switch (pReq->Mode) {
    case 0:
        if (pReq->Pan > 0) {
            *pR = vol * (0x80 - (USHORT)pReq->Pan) / 128;
            *pL = vol;
        } else {
            *pR = vol;
            *pL = vol * (USHORT)(pReq->Pan + 0x80) / 128;
        }
        if (pW->Mono) {
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
        Dist_idx = SquareRoot0(V.vy * V.vy + SQ(SquareRoot0(V.vx * V.vx + V.vz * V.vz))) / 250;
        if (Dist_idx > 0x7F) {
            Dist_idx = 0x7F;
        }
        Dir_src = (SHORT)Snd_se_dir_ck(pCut->View_p[0], pCut->View_p[2], pReq->Pos.vx, pPos->vz);
        Dir_view = (SHORT)Snd_se_dir_ck(pCut->View_p[0], pCut->View_p[2], pCut->View_r[0], pCut->View_r[2]);
        Dir_src -= (SHORT)Snd_se_dir_ck(0, 0, pCut->View_p[0], pCut->View_p[2]);
        Dir_view -= (SHORT)Snd_se_dir_ck(0, 0, pCut->View_p[0], pCut->View_p[2]);
        if (Dir_src < Dir_view) {
            d = Dir_src + (0x1000 - Dir_view);
        } else {
            d = Dir_src - Dir_view;
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
            Dir_view = Snd_pan_tbl[d];
            Dir_src = 0x7F;
            *pL = l * Dir_view + r * Dir_src;
            *pR = l * Dir_src + r * Dir_view;
        }
        pAtt = &Snd_dist_atten[Dist_idx];
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
        if (pW->Mono) {
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

/* RE2 Snd_se_dir_ck: direction from (x0, z0) to (x1, z1) via catan */
LONG Snd_se_dir_ck(LONG x0, LONG z0, LONG x1, LONG z1)
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

/* SPU address of tone idx of bank no */
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

/* 0x1000 * 2^(x / 12) */
LONG Snd_pitch_scale(LONG x)
{
    LONG v = 0x1000;
    LONG *pTbl;
    LONG r;

    if (x >= 0) {
        pTbl = Snd_pitch_up_tbl;
        r = pTbl[x % 12];
        v <<= x / 12;
        v *= r;
    } else {
        x = -x;
        pTbl = Snd_pitch_down_tbl;
        r = pTbl[x % 12];
        v >>= x / 12;
        v *= r;
    }
    return v >> 16;
}

/* Snd_pitch_scale with 1/128 semitone steps */
LONG Snd_pitch_calc(LONG x, LONG frac)
{
    LONG a, b;

    a = Snd_pitch_scale(x);
    b = Snd_pitch_scale(x + 1);
    a += (b - a) * frac / 128;
    return a;
}

/* VAB master * program * tone * SE volume of a voice */
LONG Snd_se_vol(UCHAR *pVag, SND_VOICE *pV)
{
    VabHdr *pVh = (VabHdr *)Snd_work.Bank[pV->Bank].pHead;
    ULONG Prog = pV->Prog;

    return pVh->mvol * ((ProgAtr *)(pVh + 1))[Prog].mvol / 127 * pVag[2] / 127 * Snd_work.Vol.Se / 127;
}

/* left/right volumes for a pan position */
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

/* root counter handler: advances the three sequences, fades and stops */
LONG Snd_seq_tick(void) /* event handler (OpenEvent); returns nothing */
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

    if (pW->Seq_play == 0) {
        return;
    }
    i = 3;
    pSeq = pW->Seq;
    Flg = 0;
    pW->Seq_key_on = pW->Seq_key_off = pW->Seq_rv_on = pW->Seq_rv_off = 0;
    do {
        i--;
        if (pSeq->Play_flg && !pSeq->Stop_req) {
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
                Snd_midi_tbl[(c >> 4) - 8](pSeq, &pSeq->Trk[pSeq->Status], c);
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
    Mask = pW->Seq_key_on >> 8;
    if (Flg) {
        pV = &pW->Voice[8];
        SpuSetKey(0, pW->Seq_key_off);
        j = 8;
        SpuSetReverbVoice(0, pW->Seq_rv_off);
        do {
            if (Mask & 1) {
                Snd_voice_key_on(pV, j);
            }
            Mask >>= 1;
            j++;
            pV++;
        } while (j < 24);
        SpuSetReverbVoice(1, pW->Seq_rv_on);
        SpuSetKey(1, pW->Seq_key_on);
    }
    for (j = 8, pV = &pW->Voice[8]; j < 24; j++, pV++) {
        if (pV->Use) {
            if (Flg & (0x10 << pV->Seq_no)) {
                if (pW->Seq[pV->Seq_no].Fade_mode) {
                    pV->Vol_l = pV->Vol_l_base * pW->Seq[pV->Seq_no].Fade_vol / 0x3FFF;
                    pV->Vol_r = pV->Vol_r_base * pW->Seq[pV->Seq_no].Fade_vol / 0x3FFF;
                    Snd_set_voice_vol(pV->Vol_l, pV->Vol_r, 1 << j);
                }
            }
            pV->Time++;
        }
    }
    Mask = 0;
    i = 2;
    pSeq = pW->Seq;
    do {
        if (pSeq->Play_flg && pSeq->Stop_req) {
            pSeq->Play_flg = pSeq->Stop_req = 0;
            pW->Seq_play &= ~(1 << pSeq->No);
            pW->Seq_pause &= ~(1 << pSeq->No);
            for (j = 8, pV = &pW->Voice[8]; j < 24; j++, pV++) {
                if (pV->Seq_no == pSeq->No) {
                    Mask |= 1 << j;
                    pV->Use = 0;
                }
            }
        }
        if (pSeq->Req_flg == 2) {
            pSeq->Req_flg = 1;
        }
        pSeq++;
    } while (--i != -1);
    if (Mask) {
        SpuSetKey(0, Mask);
        Snd_set_voice_vol(0, 0, Mask);
    }
}

/* sequencer note off (0x8n) */
void Snd_midi_note_off(SND_SEQ *pSeq, SND_TRACK *pTr)
{
    SND_WORK *pW;
    VabHdr *pVh;
    LONG vel;
    VagAtr *pVag;
    ProgAtr *pPa;

    pTr->Note = *pSeq->pData;
    pSeq->pData++;
    vel = *pSeq->pData;
    pSeq->pData++;
    pW = &Snd_work;
    pVh = (VabHdr *)pW->Bank[pTr->Bank].pHead;
    pTr->Note_vol = pW->Vol.Bgm * pVh->mvol / 127;
    pTr->Note_vol = pTr->Note_vol * pTr->Vol2 / 127;
    pTr->Note_vol = pTr->Note_vol * pTr->Vol / 127;
    pTr->Note_vol = pTr->Note_vol * pW->Vol.Master / 127;
    pTr->Note_vol = pTr->Note_vol * vel / 127;
    pVag = (VagAtr *)pW->Bank[pTr->Bank].pVag + pTr->Prog * *(LONG *)&pVh->fsize;
    pPa = (ProgAtr *)pVh + pTr->Prog + 2;
    Snd_seq_key_off(pSeq, pTr, Snd_tone_mask(pVag, pTr, pPa->tones));
    pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
}

/* keys off the voices of a track's tones in mask */
void Snd_seq_key_off(SND_SEQ *pSeq, SND_TRACK *pTr, ULONG mask)
{
    UCHAR v;
    ULONG b;

    pTr->Tone = 0;
    while (mask != 0) {
        if (mask & 1) {
            v = Snd_seq_voice_search(pSeq->No, pTr->Prog, pTr->Tone, pTr->Note);
            if (v != 0xFF) {
                b = 1 << v;
                Snd_work.Seq_key_off |= b;
                Snd_work.Seq_rv_off |= b;
                pTr->Mask &= ~b;
            }
        }
        mask >>= 1;
        pTr->Tone++;
    }
}

/* sequencer note on (0x9n) */
void Snd_midi_note_on(SND_SEQ *pSeq, SND_TRACK *pTr)
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
    pTr->Note_vol = pW->Vol.Bgm * pVh->mvol / 127;
    pTr->Note_vol = pTr->Note_vol * pTr->Vol / 127;
    pTr->Note_vol = pTr->Note_vol * pW->Vol.Master / 127;
    pTr->Note_vol = pTr->Note_vol * Vel / 127;
    pTr->Note_vol = pTr->Note_vol * pTr->Vol2 / 127;
    pVag = (VagAtr *)pW->Bank[pTr->Bank].pVag + pTr->Prog * ((ULONG *)pVh)[3];
    pPa = (ProgAtr *)pVh + pTr->Prog + 2;
    Vol = pTr->Vol2;
    mask = Snd_tone_mask(pVag, pTr, pPa->tones);
    if (Vel != 0) {
        Pan = pTr->Pan2 + (pTr->Pan - 0x40);
        pTr->Tone = 0;
        while (mask != 0) {
            if (mask & 1) {
                pVag = (VagAtr *)pW->Bank[pTr->Bank].pVag + pTr->Prog * ((ULONG *)pVh)[3] + pTr->Tone;
                Snd_pan_vol((SpuVoiceAttr *)&Attr, pTr->Note_vol * pVag->vol / 127, Pan + (pVag->pan - 0x40));
                Attr.volume.right = (Attr.volume.right * Attr.volume.right * 133135) >> 17;
                Attr.volume.left = (Attr.volume.left * Attr.volume.left * 133135) >> 17;
                if (pSeq->Fade_mode != 0) {
                    Attr.volume.right = Attr.volume.right * pSeq->Fade_vol / 0x3FFF;
                    Attr.volume.left = Attr.volume.left * pSeq->Fade_vol / 0x3FFF;
                }
                if (Attr.volume.right | Attr.volume.left) {
                    v = Snd_voice_get(8, 24);
                    if (v != 0xFF) {
                        pV = &pW->Voice[v];
                        pV->Use = 1;
                        pV->Prog = pTr->Prog;
                        pV->Tone = pTr->Tone;
                        pV->Note = pTr->Note;
                        pV->Bend = pTr->Bend;
                        pV->Bank = pTr->Bank;
                        pV->Bank_id = pTr->Bank_org;
                        pV->Seq_no = pSeq->No;
                        pV->Time = 0;
                        if (pW->Mono) {
                            if (Attr.volume.left > Attr.volume.right) {
                                Attr.volume.right = Attr.volume.left;
                            } else {
                                Attr.volume.left = Attr.volume.right;
                            }
                        }
                        pV->Vol_l = pV->Vol_l_base = Attr.volume.right;
                        pV->Vol_r = pV->Vol_r_base = Attr.volume.left;
                        pV->pVag = (UCHAR *)pVag;
                        pV->Ch_vol = Vol;
                        pV->Pan = Pan;
                        if (pVag->mode) {
                            pW->Seq_rv_on |= 1 << v;
                        } else {
                            pW->Seq_rv_off |= 1 << v;
                        }
                        pW->Seq_key_on |= 1 << v;
                        pW->Seq_key_off |= 1 << v;
                        pTr->Mask |= 1 << v;
                    }
                }
            }
            mask >>= 1;
            pTr->Tone++;
        }
    } else {
        Snd_seq_key_off(pSeq, pTr, mask);
    }
    pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
}

/* sequencer control change (0xBn: 0, 6, 7, 10, 99) */
void Snd_midi_ctrl(SND_SEQ *pSeq, SND_TRACK *pTr)
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
            if (pV->Use == 0 || pV->Time == 0) {
                continue;
            }
            if (pV->Ch_vol != 0) {
                pV->Vol_l = pV->Vol_l * pTr->Vol2 / pV->Ch_vol;
                pV->Vol_r = pV->Vol_r * pTr->Vol2 / pV->Ch_vol;
                if (Snd_work.Mono) {
                    if (pV->Vol_l < pV->Vol_r) {
                        pV->Vol_l = pV->Vol_r;
                    } else {
                        pV->Vol_r = pV->Vol_l;
                    }
                }
                pV->Vol_l_base = pV->Vol_l;
                pV->Vol_r_base = pV->Vol_r;
                pV->Ch_vol = pTr->Vol2;
                SpuSetVoiceVolume(i, pV->Vol_l, pV->Vol_r);
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

/* sequencer program change (0xCn) */
void Snd_midi_prog(SND_SEQ *pSeq, SND_TRACK *pTr)
{
    VabHdr *pVh;
    ProgAtr *pPa;
    LONG pan;

    pVh = (VabHdr *)Snd_work.Bank[pTr->Bank].pHead;
    pTr->Prog = *pSeq->pData;
    pPa = (ProgAtr *)pVh + pTr->Prog + 2;
    pTr->Vol = pVh->mvol * pPa->mvol / 127;
    pan = pVh->pan + 0xC0;
    pPa = (ProgAtr *)pVh + pTr->Prog + 2;
    pTr->Pan2 = pPa->mpan + pan;
    pSeq->pData++;
    pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
}

/* sequencer pitch bend (0xEn): re-pitches every voice keyed by the track */
void Snd_midi_bend(SND_SEQ *pSeq, SND_TRACK *pTr)
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
            if (pV->Use == 0 || pV->Time == 0) {
                continue;
            }
            pV->Bend = pTr->Bend;
            pHead = Snd_work.Bank[pV->Bank].pHead;
            pVag = (VagAtr *)Snd_work.Bank[pV->Bank].pVag + (pV->Tone + pV->Prog * pHead[3]);
            Note = pVag->shift;
            Pitch = (pV->Note << 7) | Note; /* via a temp: or v0,a0 operand order */
            Note = Pitch;
            if (pV->Bend > 0x2000) {
                Note += ((pV->Bend - 0x2000) << 7) * pVag->pbmax / 0x1FFF;
            }
            if (pV->Bend < 0x2000) {
                Note -= ((0x2000 - pV->Bend) << 7) * pVag->pbmin / 0x1FFF;
            }
            SpuSetVoicePitch(i, Snd_pitch_calc(((Note & 0x3F80) >> 7) - pVag->center, Note & 0x7F));
        }
    }
    pSeq->Delta = Snd_seq_get_vlq(&pSeq->pData);
}

/* sequencer meta event: 0x51 set tempo, 0x2F end of track */
void Snd_midi_meta(SND_SEQ *pSeq)
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
        if (pSeq->Repeat == 0) {
            if (pSeq->Loop_mode == 2) {
                if (pSeq->Loop_cnt == 0x7F || --pSeq->Loop_cnt != 0) {
                    pSeq->pData = pSeq->pLoop;
                }
                pSeq->Loop_mode = 0;
            } else {
                pSeq->Play_flg = 0;
                pW->Seq_play &= ~(1 << pSeq->No);
                pW->Seq_pause &= ~(1 << pSeq->No);
            }
        } else {
            pSeq->pData = pSeq->pTop;
        }
        if (pSeq->Play_flg == 0) {
            pSeq->Delta = 1;
        } else {
            pSeq->Delta = 0;
        }
    }
}

/* ignored events (0xAn, 0xDn) */
void Snd_midi_skip(SND_SEQ *pSeq)
{
    pSeq->Delta = 1;
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

/* voices a program's tones may use */
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

/* a free sequence voice in lo..hi, else a released one, else the oldest */
LONG Snd_voice_get(LONG lo, ULONG hi)
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
        if (st[i] == 0 && pV->Use == 0) {
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
        if ((ULONG)pV->Time >= best) {
            best = pV->Time;
            bi = i;
        }
    }
    return bi;
}

/* frees the voice playing Note of (Seq_no, Prog, Tone); returns it or 0xFF */
LONG Snd_seq_voice_search(LONG Seq_no, LONG Prog, LONG Tone, LONG Note)
{
    ULONG i;
    SND_VOICE *p;

    for (i = 8, p = &Snd_work.Voice[8]; i < 24; i++, p++) {
        if (p->Prog == Prog && p->Tone == Tone && p->Note == Note && p->Use != 0 && Seq_no == p->Seq_no) {
            p->Use = 0;
            return i;
        }
    }
    return 0xFF;
}

/* starts sequence slot no on SEQ data (tempo at +4, resolution at +8, events from +0xC) */
void Snd_seq_start(LONG no, UCHAR *pData, LONG Bank, LONG Loop)
{
    SND_WORK *pW = &Snd_work;
    SND_SEQ *pSeq = &pW->Seq[no];
    SND_TRACK *pTr;
    SND_VOICE *pV;
    ULONG i;
    ULONG Tempo;

    pSeq->pData = pSeq->pTop = pData + 0xC;
    pSeq->Repeat = Loop;
    pSeq->Bank = Bank;
    pSeq->Bank_org = Bank;
    Tempo = *(ULONG *)(pData + 4);
    pSeq->Resolution = *(USHORT *)(pData + 8);
    pSeq->Delta = 0;
    pSeq->Tempo_cnt = pSeq->Tempo = Tempo * 1000 / pSeq->Resolution;
    pTr = pSeq->Trk;
    pV = &pW->Voice[8];
    for (i = 0; i < 16; i++) {
        pTr->Vol2 = 0x7F;
        pTr->Pan = 0x40;
        pTr->Bend = 0x2000;
        pTr->Mask = 0;
        pTr->Bank = Bank;
        pTr->Bank_org = Bank;
        if (pV->Use == 0) {
            pV->Bend = 0x2000;
        }
        pTr++;
        pV++;
    }
    pSeq->Fade_mode = 0;
    pSeq->Fade_step = 0;
    pSeq->Fade_vol = 0;
    pW->Seq_play |= 1 << pSeq->No;
    pW->Seq_pause &= ~(1 << no);
    pSeq->Play_flg = 1;
    pSeq->Stop_req = 0;
}

/* stops sequence slot no */
void Snd_seq_stop(SHORT no)
{
    if (Snd_work.Seq[no].Play_flg != 0) {
        Snd_work.Seq[no].Stop_req = 1;
        Snd_work.Seq[no].Fade_mode = 0;
    }
}

/* stops all sequences */
void Snd_seq_stop_all(void)
{
    SHORT i;

    for (i = 0; i < 3; i++) {
        Snd_seq_stop(i);
    }
}

/* keys off a sequence's voices and pauses it (RE2 Snd_bgm_pause role) */
void Snd_seq_pause(LONG no)
{
    ULONG mask;
    UCHAR i, j;
    ULONG v;
    SND_WORK *pW = &Snd_work;

    mask = 0;
    if (Snd_work.Seq[no].Play_flg != 0) {
        Snd_work.Seq_pause |= 1 << no;
        Snd_work.Seq[no].Play_flg = 0;
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

/* resumes a paused sequence */
void Snd_seq_play_ck(LONG no)
{
    if ((Snd_work.Seq_pause >> no) & 1) {
        Snd_work.Seq[no].Play_flg = 1;
    }
}

/* RE2 Snd_se_enem: SE no of the enemy's bank at its position */
void Snd_se_enem(ULONG no, ENEMY_WORK *pEm)
{
    VECTOR v;

    v.vx = pEm->Pos_x;
    v.vy = pEm->Pos_y;
    v.vz = pEm->Pos_z;
    if (pEm->Unk12F == Snd_edt[3]) {
        no += 0x10;
    }
    Snd_se_stad(no | 0x10300, &v, 0, 0);
}

/* Snd_se_enem at a part's position */
void Snd_se_enem_parts(LONG no, UCHAR parts, ENEMY_WORK *pEm)
{
    VECTOR v;
    PARTS_W *p = &pEm->pSin_parts_ptr[parts];

    v.vx = p->Workm.t[0];
    v.vy = p->Workm.t[1];
    v.vz = p->Workm.t[2];
    if (pEm->Unk12F == Snd_edt[3]) {
        no += 0x10;
    }
    Snd_se_stad(no | 0x10300, &v, 0, 0);
}

/* RE2 Snd_se_walk: footstep SE for the floor under a part */
void Snd_se_walk(LONG kind, ULONG parts, ENEMY_WORK *pEm)
{
    LONG one;
    VECTOR v;
    SVECTOR s;
    SVECTOR t;
    PARTS_W *pP;
    LONG se;

    pP = &pEm->pSin_parts_ptr[parts];
    se = Snd_floor_kind(pP->Workm.t[0], pP->Workm.t[2], pEm->nFloor) & 0x7F;
    if (pEm->Water != 0) {
        se = 0x17;
        if (pEm->Water == 1) {
            t.vx = 0x400;
            s.vx = 0;
            s.vy = 0;
            s.vz = 0;
            t.vy = 0;
            t.vz = 0;
            Esp_call(0xE, (USHORT)G.Player.Cdir_y | 0x10000000, &pP->Workm, &s, &t);
        } else {
            pP--;
            s.vy = pEm->Water - pP->Workm.t[1];
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
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x800) & 0xFFFF) | 0xE000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y + 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                }
                if (kind == 1) {
                    s.vx = -300;
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x800) & 0xFFFF) | 0x15000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x400) & 0xFFFF) | 0x10000000, &pP->Workm, &s, 0);
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y + 0x400) & 0xFFFF) | 0x10000000, &pP->Workm, &s, 0);
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
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x800) & 0xFFFF) | 0xE000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y + 0x400) & 0xFFFF) | 0xC000000, &pP->Workm, &s, 0);
                }
                if (kind == 1) {
                    s.vx = -600;
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x800) & 0xFFFF) | 0x17000000, &pP->Workm, &s, 0);
                    s.vx = 0;
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y - 0x400) & 0xFFFF) | 0x14000000, &pP->Workm, &s, 0);
                    Esp_call(0x61C, (((USHORT)G.Player.Cdir_y + 0x400) & 0xFFFF) | 0x14000000, &pP->Workm, &s, 0);
                }
            }
        }
    }
    kind = se - -kind;
    if (pEm->Unk1CA) {
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

/* footstep sound kind of the floor at (x, z) */
UCHAR Snd_floor_kind(LONG x, LONG z, LONG id)
{
    LONG pos[3]; UCHAR *pFlr; LONG n;
    LONG Attr;
    pFlr = (UCHAR *)G.pRoom->pUnk34; n = *(SHORT *)pFlr; pFlr += 2; pos[0] = x; pos[2] = z; pFlr += n * 0xC;
    while (n--) {
        pFlr -= 0xC;
        Attr = ((SHORT *)pFlr)[5];
        if (!(Attr & 0x100) && id == (Attr & 0x7F) && Hit_ck_box(pos, (SHORT *)pFlr)) {
            return pFlr[8];
        }
    }
    return 0x1D;
}

/* RE2 Snd_bgm_ck role: reads the room's BGM entry, sets CD.Bgm_main_id/Bgm_sub_id and fades out what changes */
void Snd_bgm_ck(void)
{
    tagGLOBAL *pG = &G;
    SND_WORK *pW = &Snd_work;
    tagCD *pCd = &CD;
    SHORT Stage;
    USHORT Bgm;

    Stage = pG->Stage_no;
    if (Stage >= 5) {
        Stage -= 5;
    }
    Bgm = pG->Bgm_tbl[Bgm_room_ofs[Stage] + pG->Room_no];
    pW->Fade_vol = 0x3C00;
    pW->Fade_mask = 0xFF;
    pW->Fade_step = 0x400;
    if ((Bgm >> 8) != 0xFF) {
        pW->Bgm_req = Bgm >> 15;
    }
    if ((Bgm & 0xFF) != 0xFF) {
        pW->Bgm_req |= (Bgm >> 5) & 6;
    }
    pCd->Bgm_main_id = (Bgm >> 8) & 0x3F;
    if ((!(pW->Bgm_req & 1) && pW->Seq_data[0].Id == pCd->Bgm_main_id) || pW->Seq_data[0].Id != pCd->Bgm_main_id) {
        if (pW->Seq[0].Play_flg && !pW->Seq[0].Stop_req) {
            if (pW->Seq[0].Fade_mode != 2) {
                pW->Seq[0].Fade_mode = 2;
                pW->Seq[0].Fade_vol = 0x3F00;
            }
            pW->Seq[0].Fade_step = 0x100;
        }
    } else if (pW->Seq[0].Play_flg) {
        pW->Bgm_req &= 6;
    }
    if (pCd->Bgm_main_id == 0x3F) {
        pCd->Bgm_main_id = 0xFF;
        pW->Seq_data[0].Id = 0xFF;
    }
    pCd->Bgm_sub_id = Bgm & 0x3F;
    if ((!(pW->Bgm_req & 2) && pW->Seq_data[1].Id == pCd->Bgm_sub_id) || pW->Seq_data[1].Id != pCd->Bgm_sub_id) {
        if (pW->Seq[1].Play_flg && !pW->Seq[1].Stop_req) {
            if (pW->Seq[1].Fade_mode != 2) {
                pW->Seq[1].Fade_mode = 2;
                pW->Seq[1].Fade_vol = 0x3F00;
            }
            pW->Seq[1].Fade_step = 0x100;
        }
    } else if (pW->Seq[1].Play_flg) {
        pW->Bgm_req &= 5;
    }
    if ((!(pW->Bgm_req & 4) && pW->Seq_data[1].Id == pCd->Bgm_sub_id) || pW->Seq_data[1].Id != pCd->Bgm_sub_id) {
        if (pW->Seq[2].Play_flg && !pW->Seq[2].Stop_req) {
            if (pW->Seq[2].Fade_mode != 2) {
                pW->Seq[2].Fade_mode = 2;
                pW->Seq[2].Fade_vol = 0x3F00;
            }
            pW->Seq[2].Fade_step = 0x100;
        }
    } else if (pW->Seq[2].Play_flg) {
        pW->Bgm_req &= 3;
    }
    if (pCd->Bgm_sub_id == 0x3F) {
        pCd->Bgm_sub_id = 0xFF;
        pW->Seq_data[1].Id = 0xFF;
        pW->Seq_data[2].Id = 0xFF;
    }
}

/* waits (bounded) for the sequence and master fades */
void Snd_bgm_fade_wait(void)
{
    SND_WORK *pW = &Snd_work;
    UCHAR i = 0;
    UCHAR Busy;

    do {
        Busy = pW->Seq[0].Fade_mode + pW->Seq[1].Fade_mode + pW->Seq[2].Fade_mode + pW->Fade_mask;
        if ((signed char)i++ < 0) {
            break;
        }
        Task_sleep(1); /* undeclared on purpose (monitor.h not included): any prototype, even int Task_sleep(), changes this loop */
    } while (Busy);
    Snd_se_stad(0xE, 0, 0, 0);
}

/* RE2 Snd_bgm_play_ck role: requests the room BGM sequences that are not playing */
void Snd_bgm_play_ck(void)
{
    SND_WORK *p = &Snd_work;
    tagCD *pCd = &CD;
    UCHAR i;

    /* Never-false test that re-sets both pointers. It only steers the allocator: cse reuses the two %hi
       pseudos for the re-sets (the NULLs hide the old values), so both live into this block and become
       global allocnos (the do/while(0) gives the Snd_work one the higher priority: v0, CD v1).
       reload_cse then deletes the re-sets (s0/s1 already hold them) and jump2 the empty branch. */
    if ((tagCD *)p != pCd) {
        p = NULL;
        pCd = NULL;
        do {
            p = &Snd_work;
        } while (0);
        pCd = &CD;
    }
    for (i = 0; i < 3; i++) {
        if ((p->Bgm_cancel >> i) & 1) {
            p->Bgm_req &= (1 << i) ^ 7;
        }
        p->Bgm_cancel &= ~(1 << i);
    }
    if (p->Bgm_req) {
        if ((p->Bgm_req & 1) && pCd->Bgm_main_id != 0xFF) {
            Snd_se_stad(0x500, 0, 0, 0);
        }
        if (pCd->Bgm_sub_id != 0xFF) {
            if ((p->Bgm_req & 2) && !p->Seq[1].Play_flg) {
                Snd_se_stad(0x600, 0, 0, 0);
            }
            if ((p->Bgm_req & 4) && !p->Seq[2].Play_flg) {
                Snd_se_stad(0x700, 0, 0, 0);
            }
        }
        p->Bgm_req = 0;
    }
}


/* loads bank no's VAB header and body (bank 0 player, 1 weapon; RE2 Snd_sys_core_set/arms_set) */
LONG Snd_vab_load(LONG no, LONG idx, ULONG *pAdr, ULONG *pBuf)
{
    USHORT File_no[8] = { 0x104, 0x103, 0xDA, 0xD9, 0, 0, 0, 0 };
    signed char Cnt;
    ULONG *p;
    LONG Size;

    if (no == 1 && idx == 0) {
        return 1;
    }
    Cnt = (signed char)Vab_head_ofs[no];
    p = pAdr;
    while (Cnt > 0) {
        *p++ = -1;
        Cnt--;
    }
    Size = Cd_read(idx * 2 + File_no[no * 2], pAdr, 0, "Vab Header");
    Snd_vab_head_set(no, pAdr);
    Snd_bank_set(no, Cd_read_s(idx * 2 + File_no[no * 2 + 1], 2, pBuf, no, "Vab Body"), pBuf);
    if (Size < 0) {
        return 0;
    }
    return Size;
}

/* RE2 Snd_sys_pset: registers the room EDT as bank 2 */
void Snd_sys_pset(void)
{
    LONG *pEdt;
    SND_WORK *pW;

    pEdt = (LONG *)G.pRoom->pEdt0;
    pW = &Snd_work;
    if (pEdt == NULL) {
        pW->Bank_id[2].Id = -1;
        return;
    }
    *(LONG **)&Snd_edt = pEdt;
    pW->Rv_mask = G.pRoom->Unk6 * 4;
    while (*pEdt != -1) {
        pEdt++;
    }
    pEdt++;
    Snd_vab_head_set(2, (ULONG *)pEdt);
    if (G.pRoom->pEdt0 != NULL) {
        G.pEm_mem = (ENEMY_WORK *)(((ULONG)pW->Bank[2].pHead + pW->Bank[2].pHead[1] + 3) & ~3);
    }
    Snd_bank_set(2, pEdt[0x30], 0);
}

/* registers a sequence pack and its VAB for BGM slot no */
void Snd_seq_data_set(ULONG no, ULONG id, ULONG *pAdr)
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
    pW->Bank[bank].pVag = (ULONG *)(pPa + pVh->ps);
}

/* registers bank no's VAB header */
void Snd_vab_head_set(LONG no, ULONG *pAdr)
{
    SND_WORK *pW = &Snd_work;
    VabHdr *pVh;
    ProgAtr *pPa;

    pW->pAdr[no] = pAdr;
    pW->Bank[no].pHead = (ULONG *)(pVh = (VabHdr *)(pAdr + Vab_head_ofs[no]));
    pPa = (ProgAtr *)(pVh + 1);
    pW->Bank[no].pVag = (ULONG *)(pPa + pVh->ps);
}

/* stores bank no's SPU address and size after its body was sent */
void Snd_bank_set(ULONG no, ULONG size, ULONG *pBuf)
{
    SND_WORK *pW = &Snd_work;
    SND_WORK *p = (SND_WORK *)((UCHAR *)pW + no * 8);

    size = (size + 0x3F) & ~0x3F;
    p->Bank_id[0].Size = size >> 6;
    p->Bank_id[0].Id = no;
    if (no < 5) {
        if (no != 3) {
            p->Bank_id[0].pData = (UCHAR *)Snd_spu_adr[no];
        } else {
            pW->Bank_id[3].pData = (UCHAR *)(0x3DE10 - size);
        }
    } else if (no == 5) {
        pW->Bank_id[5].pData = (UCHAR *)Snd_spu_adr[5];
    } else {
        p->Bank_id[0].pData = (UCHAR *)(0x7B7C0 - size);
    }
}

/* RE2 Snd_sys_enem_set role: registers the enemy bank (3) when the room has enemies */
void Snd_sys_enem_set(void)
{
    if (G.Em_num) {
        Snd_vab_head_set(3, (ULONG *)0x801FB600);
    } else {
        Snd_work.Bank_id[3].Id = -1;
    }
}

/* RE2 Snd_bgm_tbl_set: sets the BGM of a room */
void Snd_bgm_tbl_set(UCHAR Stage, UCHAR Room, USHORT Bgm)
{
    SHORT Stage_idx = Stage;

    if (Stage_idx >= 5) {
        Stage_idx -= 5;
    }
    G.Bgm_tbl[Bgm_room_ofs[Stage_idx] + Room] = Bgm;
}
