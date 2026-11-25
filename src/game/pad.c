/*
 * PAD.C (RE2 PAD.C lineage): pad reading, key repeat, and the DualShock vibration request queues
 */
#include "main.h"
#include "mojidisp.h"
#include "pad.h"
#include "sce_at.h"

/* RE2 Padd_trg / Padd_rep are ULONG; tagGLOBAL has USHORT views (lhu readers elsewhere) */
#define PADD_TRG(pG) (*(ULONG *)&(pG)->Unk20FC)
#define PADD_REP(pG) (*(ULONG *)&(pG)->Unk2100)

/* RE2 Pad_set: reads pad 0 (type, analog stick, actuators), builds held/trigger/repeat keys */
void Pad_set(void)
{
    tagGLOBAL *pG = &G;
    PAD_WORK *pW = &Pad_work;
    ULONG Work;                        /* PadGetState result, small motor power, Padd_rep bit */
    UCHAR Vib_mode = Vib_mode;
    UCHAR Stk_x = Stk_x;
    UCHAR Stk_y = Stk_y;
    UCHAR Stk_act = Stk_act;
    USHORT *pKey;
    ULONG Padd, Bit;
    ULONG Work2;                       /* large motor power, Padd trigger, repeat table count */
    UCHAR *pT;

    Work = PadGetState(0);
    if (Work >= 2) {
        pG->Pad_type = PadInfoMode(0, 1, 0);
        Vib_mode = PadInfoMode(0, 2, 0);
        if (Pad_type_old != pG->Pad_type) {
            Pad_act_set = 0;
        }
        switch (pG->Pad_type) {
        case 7:
            Stk_x = pG->Padd_data[0][6];
            Stk_y = pG->Padd_data[0][7];
        case 4:
            if (Pad_act_set == 0) {
                PadSetAct(0, PadActData, 2);
                if (Work == 2) {
                    Pad_act_set = 1;
                } else if (Work == 6 && PadSetActAlign(0, Pad_act_align) != 0) {
                    Pad_act_set = 2;
                }
            }
            goto accepted;
        }
    }
    pG->Padd_data[0][3] = 0xFF;
    pG->Padd_data[0][2] = 0xFF;
    pG->Padd_data[1][3] = 0xFF;
    pG->Padd_data[1][2] = 0xFF;
    Pad_act_set = 0;
    pG->Pad_type = 0;
accepted:
    PadActData[1] = 0;
    PadActData[0] = 0;
    Pad_type_old = pG->Pad_type;
    if (pG->Unk2488 == 0) {
        if ((!(Moji.Mess_flg & 0x80) || (G.Status_flg & STAT_CINESCO)) && !(G.System_flg & 0x10000000)) {
            Work = SetPadActData(Vib_req0);
            Work2 = SetPadActData(Vib_req1);
            if (Vib_mode) {
                PadActData[0] = Work;
                PadActData[1] = Work2;
            } else if (Work != 0 || Work2 != 0) {
                PadActData[0] = 0x40;
                PadActData[1] = 0xFF;
            }
        }
    }
    if (Main.Unk196 & 0x80) {
        PadActData[1] = 0;
        PadActData[0] = 0;
    }
    pKey = Key_type[Main.Unk195];
    PADD_TRG(pG) = pG->Unk20F8;
    if (!(G.System_flg & 0x10000000)) {
        Padd = ~(pG->Padd_data[0][2] << 8 | pG->Padd_data[0][3]);
        pG->Unk20F8 = (USHORT)Padd;
        if (pG->Pad_type == 7) {
            Stk_act = 0;
            if (!(Padd & 0xF000)) {
                if (Stk_y < 0x30) {
                    pG->Unk20F8 |= 0x1000;
                }
                if (Stk_y > 0xD0) {
                    pG->Unk20F8 |= 0x4000;
                }
                if (Stk_x < 0x50) {
                    if (Stk_x != 0) {
                        Stk_x = 0x50 - Stk_x;
                    }
                    Stk_act = 1;
                    pG->Unk20F8 |= 0x8000;
                }
                if (Stk_x > 0xB0) {
                    if (Stk_x < 0xFF) {
                        Stk_x += 0x50;
                    }
                    Stk_act |= 1;
                    pG->Unk20F8 |= 0x2000;
                }
            }
            if (pG->Unk20F8 & 0x400) {
                pG->Unk20F8 |= 0x800;
            }
        }
    } else if (G.System_flg & 0x8000000) {
        Padd = pG->Padd_data[0][2] << 8 | pG->Padd_data[0][3];
        pG->Unk20F8 = (USHORT)~Padd;
        Work2 = (PADD_TRG(pG) ^ pG->Unk20F8) & pG->Unk20F8;
        switch (Game.Unk4E) {
        case 1:
            if (Game.Pdemo_cnt < Game.pSave->Pdemo_cnt - 0x1F && !(Work2 & 0xC00)) {
                break;
            }
            Game.Unk4E = 2;
        case 2:
            Fade_set(1, 0, 2, 2, 0, 0xFFFFFF, 0x1E);
            Game.Unk4E = 3;
        case 3:
            if (Fade_status(1) & 4) {
                Game.Unk4E = 0;
                Main.Unk196 = Game.Unk4D;
                Main.Unk195 = Game.Unk4C;
                G.System_flg |= 0x200000;
            }
            break;
        }
        if (Game.Unk4E != 0) {
            pKey = Key_type_demo;
            *(USHORT *)&G.Unk20F8 = Game.pSave->Padd[Game.Pdemo_cnt++];
        }
    }
    Padd = pG->Unk20F8;
    pG->Key_old = pG->Key;
    pG->Key = 0;
    Bit = 1;
    do {
        if (Padd & *pKey) {
            pG->Key |= Bit;
        }
        Bit <<= 1;
        pKey++;
    } while (Bit & 0xFFFF);
    if (G.Unk2468 & 0x1000000) {
        pG->Key &= 0x3000;
        Pad_work.Stop_bak = pG->Unk2468;
    } else if (Pad_work.Stop_bak & 0x1000000) {
        Pad_work.Stop_bak = 0;
        pG->Key_old = pG->Key;
    }
    if (!(G.System_flg & 0x10000000) && pG->Pad_type == 7 && Stk_act) {
        if (Stk_x != 0xFF) {
            pG->Unk20F8 &= ~0x2000;
            PADD_TRG(pG) &= ~0x2000;
        }
        if (Stk_x != 0) {
            pG->Unk20F8 &= ~0x8000;
            PADD_TRG(pG) &= ~0x8000;
        }
    }
    PADD_REP(pG) = 0;
    PADD_TRG(pG) = (PADD_TRG(pG) ^ pG->Unk20F8) & pG->Unk20F8;
    pG->Key_trg = (pG->Key_old ^ pG->Key) & pG->Key;
    if (pW->Num != 0) {
        Work2 = pW->Num;
        pT = pW->Tbl[0];
        do {
            Work = 1 << pT[0];
            if (PADD_TRG(pG) & Work) {
                pT[1] = pW->Rep.Time[0];
                PADD_REP(pG) |= Work;
            }
            if (pG->Unk20F8 & Work) {
                if (pT[1] != 0) {
                    pT[1]--;
                } else {
                    pT[1] = pW->Rep.Time[1];
                    PADD_REP(pG) |= Work;
                }
            }
            pT += 2;
        } while (--Work2 != 0);
    }
}

/* RE2 Pad_rep_set: sets the repeat delay/rate pair and the list of keys (Flg bits) that repeat */
void Pad_rep_set(ULONG Flg, SHORT Val)
{
    PAD_WORK *pW;
    UCHAR *p;
    LONG i;
    LONG n;

    pW = &Pad_work;
    pW->Rep.Both = Val;
    p = pW->Tbl[0];
    i = 0;
    n = 0;
    do {
        if (Flg & 1) {
            *p = i;
            p += 2;
            n++;
        }
        Flg >>= 1;
        i++;
    } while (Flg != 0);
    pW->Num = n;
}

/* queues a small-motor (on/off) vibration of Time frames after Attr frames delay */
void Pad_vib_set0(LONG Time, USHORT Attr)
{
    VIB_REQ *pReq = Vib_req0;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur0 = Pad_vib_get_free(pReq);
        Vib_cur0->Flg = 1;
        Vib_cur0->Time = Time;
        Vib_cur0->Pow = (Time != 0);
        Vib_cur0->Delay = Attr;
        Vib_cur0->Step = 0;
    }
}

/* queues a large-motor vibration at Power for Time frames after Attr frames delay */
void Pad_vib_set1(LONG Time, ULONG Power, ULONG Attr)
{
    VIB_REQ *pReq = Vib_req1;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur1 = Pad_vib_get_free(pReq);
        Vib_cur1->Flg = 1;
        Vib_cur1->Time = Time;
        Vib_cur1->Pow = Power;
        Vib_cur1->Delay = Attr;
        Vib_cur1->Step = 0;
    }
}

/* queues a large-motor vibration fading from Pow_st to Pow_ed over Time frames */
void Pad_vib_fade_set(LONG Time, LONG Pow_st, LONG Pow_ed, ULONG Attr)
{
    VIB_REQ *pReq = Vib_req1;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur1 = Pad_vib_get_free(pReq);
        Vib_cur1->Flg = 1;
        Vib_cur1->Time = Time;
        Vib_cur1->Delay = Attr;
        Vib_cur1->Pow_cur = Pow_st << 7;
        Vib_cur1->Step = (Pow_ed - Pow_st) * 0x80 / Time;
    }
}

/* vibration script: USHORT records {1, Time, Attr} -> Pad_vib_set0, {2, Time, Power, Attr} -> Pad_vib_set1,
 * {3, Time, Pow_st | Pow_ed << 8, Attr} -> Pad_vib_fade_set; 0xFFFF ends. pTbl starts with byte offsets. */
void Pad_vib_data_set(USHORT *pTbl, ULONG No)
{
    USHORT *p = (USHORT *)((UCHAR *)pTbl + pTbl[No]);
    ULONG Time, Pow, Attr;

    while (*p != 0xFFFF) {
        switch (*p) {
        case 1:
            Pad_vib_set0(p[1], p[2]);
            p += 3;
            break;
        case 2:
            Pad_vib_set1(p[1], p[2], p[3]);
            p += 4;
            break;
        case 3:
            Time = p[1];
            Attr = p[3];
            Pow = p[2];
            Pad_vib_fade_set(Time, Pow & 0xFF, Pow >> 8, Attr);
            p += 4;
            break;
        }
    }
}

/* clears both vibration queues and stops the motors */
void Pad_vib_init(void)
{
    ULONG i;

    for (i = 0; i < 33; i++) {
        Vib_req0[i].Flg = 0;
        Vib_req1[i].Flg = 0;
    }
    Vib_cur0 = Vib_req0;
    Vib_cur1 = Vib_req1;
    PadActData[1] = 0;
    PadActData[0] = 0;
}

/* steps a vibration queue one frame; returns the strongest active power */
UCHAR SetPadActData(VIB_REQ *pReq)
{
    ULONG i;
    UCHAR Max = 0;
    UCHAR *pB;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg == 0) {
            continue;
        }
        pB = &pReq->Pow;
        if (*(USHORT *)(pB + 1) != 0) {
            (*(USHORT *)(pB + 1))--;
            continue;
        }
        if (--(*(USHORT *)(pB + 3)) == 0) {
            pReq->Flg = 0;
        }
        if (*(SHORT *)(pB + 5) != 0) {
            *(USHORT *)(pB + 7) += *(USHORT *)(pB + 5);
            *pB = *(USHORT *)(pB + 7) >> 7;
        }
        if (Max < *pB) {
            Max = *pB;
        }
    }
    return Max;
}

/* first free request of a vibration queue (32 entries) */
VIB_REQ *Pad_vib_get_free(VIB_REQ *pReq)
{
    ULONG i;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg == 0) {
            break;
        }
    }
    return pReq;
}

/* number of active requests of a vibration queue */
LONG Pad_vib_count(VIB_REQ *pReq)
{
    ULONG i;
    LONG Cnt = 0;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg != 0) {
            Cnt++;
        }
    }
    return Cnt;
}
