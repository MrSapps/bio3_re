#include "common.h"
#include "ovl/ltsout.h"

/* main task */
void func_801C2018(void)
{
    tagGLOBAL *pG = &G;
    LTS_EVT *pEvt = pG->pUnk21DC;
    LTS_WORK *pLw;
    ULONG i;
    TILE *pT;
    SVECTOR Pos;
    SVECTOR Spd;

    Task_suspend(0);
    pLw = &Lts_wk;
    pG->Status_flg |= 0x80;
    pLw->Cut_old = pG->Cut_no;
    pG->Cut_no = pEvt->Cut_no;
    Main.Unk18E = 1;
    Cut_change(pEvt->Cut_no);
    pLw->Run = 1;
    pLw->Blink = 1;
    pLw->Routine = 0;
    pLw->Cnt = 0;
    pLw->Cnt2 = 0;
    pLw->Esp_on = 0;
    pLw->Cursor = 1;
    pLw->Mess = 0;
    pG->Status_flg |= 0x400000;
    pLw->Flg_no = pEvt->Flg_no;
    pLw->Stage = 0;
    if (Flg_ck(&pG->Unk7888, 8))
        pLw->Stage = 1;
    if (Flg_ck(&pG->Unk7888, 9))
        pLw->Stage = 2;
    Pos.vy = -0x7B9;
    Pos.vz = -0x3C8C;
    Spd.vx = -0x280;
    Spd.vy = 0;
    Spd.vz = 0;
    switch (pLw->Stage) {
    case 1:
        Pos.vx = -0x65D6;
        func_8001B484(0x215, 0x1C00000, &GsIDMATRIX, &Pos, &Spd);
        pLw->Esp_on = 1;
        break;
    case 2:
        Pos.vx = -0x65D6;
        func_8001B484(0x215, 0x1C00000, &GsIDMATRIX, &Pos, &Spd);
        Pos.vx = -0x65AB;
        func_8001B484(0x215, 0x1C00000, &GsIDMATRIX, &Pos, &Spd);
        pLw->Esp_on = 1;
        break;
    }
    for (i = 0; i < 4; i++)
        pLw->Pattern[i] = 0;
    SetDrawMode(&pLw->Dr_mode[0], 0, 0, 0, NULL);
    SetDrawMode(&pLw->Dr_mode[1], 0, 0, 0, NULL);
    pT = &pLw->Tile[0];
    setTile(pT);
    setRGB0(pT, 0x10, 0x10, 0x10);
    setWH(pT, 0xA8, 0x1C);
    setXY0(pT, 0x4C, 0xA6);
    setSemiTrans(pT, 1);
    pT = &pLw->Tile[1];
    setTile(pT);
    setRGB0(pT, 0x10, 0x10, 0x10);
    setWH(pT, 0xA8, 0x1C);
    setXY0(pT, 0x4C, 0xA6);
    setSemiTrans(pT, 1);
    func_80038634(0xF00C, 0x60A);
    while (pLw->Run) {
        func_801C2370();
        Esp_task_main();
        func_801C28BC();
        Eff_sprt_trans();
        if (!(G.System_flg & 0x80000000))
            Psp_trans();
        Task_sleep(1);
    }
    Main.Unk18E = 1;
    G.Cut_no = pLw->Cut_old;
    Cut_change(pLw->Cut_old);
    func_8001B894(0x215, NULL);
    G.Status_flg |= 0x400000;
    G.Status_flg &= ~0x80;
    Task_signal(0);
    Task_exit();
}

/* puzzle state machine */
void func_801C2370(void)
{
    LTS_WORK *pLw = &Lts_wk;
    ULONG i;
    LONG r;
    UCHAR Flg;
    SVECTOR Pos;
    SVECTOR Spd;

    switch (pLw->Routine) {
    case 0:
        if (++pLw->Cnt >= 11) {
            pLw->Cnt = 0;
            pLw->Routine++;
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            r = Rnd() % 10;
            if (r != pLw->Pattern[pLw->Stage])
                break;
        }
        pLw->Pattern[pLw->Stage + 1] = r;
        pLw->Lamp[1] = Lts_tbl[r][0];
        pLw->Lamp[2] = Lts_tbl[r][1];
        pLw->Lamp[3] = Lts_tbl[r][2];
        pLw->Lamp[4] = Lts_tbl[r][3];
        pLw->Answer = Lts_tbl[r][4];
        pLw->Routine++;
        pLw->Cnt = 0;
        pLw->Cnt2 = 0;
        Snd_se_stad(0x20A, NULL, 0, 0);
        if (!pLw->Esp_on) {
            Pos.vy = -0x7B9;
            Pos.vz = -0x3C8C;
            Spd.vx = -0x280;
            Spd.vy = 0;
            Spd.vz = 0;
            switch (pLw->Stage) {
            case 1:
                Pos.vx = -0x65D6;
                func_8001B484(0x215, 0x1C00000, &GsIDMATRIX, &Pos, &Spd);
                break;
            case 2:
                Pos.vx = -0x65AB;
                func_8001B484(0x215, 0x1C00000, &GsIDMATRIX, &Pos, &Spd);
                break;
            }
        }
        pLw->Esp_on = 0;
        break;
    case 2:
        if (G.Key_trg & 0x1000) {
            if (pLw->Cursor < LTS_SW_NUM) {
                Snd_se_stad(0x202, NULL, 0, 0);
                pLw->Lamp[pLw->Cursor - 1] ^= 1;
                pLw->Lamp[pLw->Cursor] ^= 1;
                pLw->Lamp[pLw->Cursor + 1] ^= 1;
            } else {
                Snd_se_stad(5, NULL, 0, 0);
                pLw->Run = 0;
            }
            break;
        }
        if (G.Unk2100 & 0x8000) {
            if (pLw->Cursor >= 2) {
                pLw->Cursor--;
                Flg = 1;
            }
        }
        if (G.Unk2100 & 0x2000) {
            if (pLw->Cursor < LTS_SW_NUM) {
                pLw->Cursor++;
                Flg = 1;
            }
        }
        if (Flg == 1)
            Snd_se_stad(4, NULL, 0, 0);
        Flg = 1;
        for (i = 1; i < LTS_SW_NUM; i++) {
            if (i == pLw->Answer) {
                if (pLw->Lamp[i] != 1)
                    Flg = 0;
            } else if (pLw->Lamp[i]) {
                Flg = 0;
            }
        }
        if (Flg) {
            pLw->Cnt = 0;
            pLw->Cnt2 = 0;
            pLw->Esp_on = 0;
            pLw->Routine++;
        }
        break;
    case 3:
        if (++pLw->Cnt2 >= 7) {
            pLw->Cnt2 = 0;
            pLw->Cnt++;
            if (pLw->Blink ^= 1)
                Snd_se_stad(0x209, NULL, 0, 0);
        }
        if (pLw->Cnt >= 7) {
            Flg_on(&G.Unk7888, pLw->Stage + 8);
            if (++pLw->Stage == 3) {
                pLw->Cnt = 0;
                pLw->Cnt2 = 0;
                pLw->Esp_on = 0;
                pLw->Blink = 0;
                pLw->Lamp[1] = 1;
                pLw->Lamp[2] = 1;
                pLw->Lamp[3] = 1;
                pLw->Lamp[4] = 1;
                pLw->Answer = 0;
                pLw->Routine++;
            } else {
                pLw->Routine = 0;
                pLw->Blink = 1;
            }
        }
        break;
    case 4:
        if (++pLw->Cnt2 >= 7) {
            pLw->Cnt2 = 0;
            pLw->Cnt++;
            if (pLw->Blink ^= 2) {
                Snd_se_stad(0x20A, NULL, 0, 0);
                if (pLw->Cnt == 1) {
                    Pos.vy = -0x7B9;
                    Pos.vz = -0x3C8C;
                    Spd.vx = -0x280;
                    Pos.vx = -0x6582;
                    Spd.vy = 0;
                    Spd.vz = 0;
                    func_8001B484(0x215, 0x1C00000, &GsIDMATRIX, &Pos, &Spd);
                }
            }
        }
        if (pLw->Cnt >= 9) {
            pLw->Run = 0;
            Flg_on(G.Unk7868, pLw->Flg_no);
        }
        break;
    }
}

/* draw: dark background, switch labels, lamps through the cut flags */
void func_801C28BC(void)
{
    LTS_WORK *pLw = &Lts_wk;
    DR_MODE *pDm;
    TILE *pT;
    ULONG i;
    ULONG Bit;
    ULONG Mess;
    ULONG Attr;
    UCHAR *pStr;

    pDm = &pLw->Dr_mode[0];
    if (G.Cbuf_id)
        pDm = &pLw->Dr_mode[1];
    AddPrim(G.pOt0 + 6, pDm);
    pT = &pLw->Tile[0];
    if (G.Cbuf_id)
        pT = &pLw->Tile[1];
    AddPrim(pDm, pT);
    Mess = pLw->Mess;
    Bit = 0;
    for (i = 1; i < LTS_SW_NUM + 1; i++) {
        pStr = &Lts_mess[Lts_mess_ofs[Mess]];
        Attr = pLw->Cursor == i;
        Mess_print(i * 0x1C + 0x37, 0xAD, pStr, (Attr << 4) | 0x100);
        if (i < LTS_SW_NUM) {
            switch (pLw->Blink) {
            case 0:
                Flg_off(&G.Cut_flg[G.Cut_no], Bit);
                Flg_off(&G.Cut_flg[G.Cut_no], Bit + 4);
                break;
            case 1:
                if (pLw->Lamp[i])
                    Flg_on(&G.Cut_flg[G.Cut_no], Bit);
                else
                    Flg_off(&G.Cut_flg[G.Cut_no], Bit);
                if (i == pLw->Answer)
                    Flg_on(&G.Cut_flg[G.Cut_no], Bit + 4);
                else
                    Flg_off(&G.Cut_flg[G.Cut_no], Bit + 4);
                break;
            case 2:
                Flg_on(&G.Cut_flg[G.Cut_no], Bit);
                Flg_on(&G.Cut_flg[G.Cut_no], Bit + 4);
                break;
            }
        }
        Mess++;
        Bit++;
    }
}
