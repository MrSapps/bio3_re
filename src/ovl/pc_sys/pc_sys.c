#include "common.h"
#include "ovl/pc_sys.h"

/* main task: run the terminal picked by the AOT */
void func_801C2354(void)
{
    Task_suspend(0);
    DrawSync(0);
    for (;;) {
        switch (((USHORT *)G.pUnk21DC)[1]) {
        case 0:
            func_801C347C();
            break;
        case 1:
            func_801C3A0C();
            break;
        }
        Task_sleep(1);
    }
}

/* window open/close requests; returns 1 while the window zooms */
ULONG func_801C23D0(void)
{
    switch (G.Unk75EE) {
    case 0:
        break;
    case 1:
        Snd_se_stad(0x206, NULL, 0, 0);
    case 2:
        G.Unk75EF = 0;
        G.Unk75EE = 2;
        if (func_801C24E0(0x30, 0x8E, 0xF7, 0x57))
            return 1;
        func_801C2998(1, 0xF7, 0x57, 0x80);
        break;
    case 3:
        Snd_se_stad(0x207, NULL, 0, 0);
    case 4:
        func_801C2998(0, 0, 0, 0x80);
        G.Unk75EF = 1;
        G.Unk75EE = 4;
        if (func_801C24E0(0x30, 0x8E, 0xF7, 0x57))
            return 1;
        break;
    }
    G.Unk75EE = 0;
    return 0;
}

/* window zoom frame step: grows (G.Unk75EF == 0) or shrinks in 12 steps */
ULONG func_801C24E0(ULONG X, ULONG Y, ULONG W, ULONG H)
{
    PC_WORK *pPc = Pc_wk;
    tagGLOBAL *pG = &G;

    if (pG->Unk75EF) {
        if (pPc->Zoom < 13) {
            func_801C2608(X, Y, X + W - pPc->Zoom * W / 12, Y + H - pPc->Zoom * H / 12);
            pPc->Zoom++;
            return 1;
        }
    } else {
        if (pPc->Zoom < 13) {
            func_801C2608(X, Y, X + pPc->Zoom * (W / 12), Y + pPc->Zoom * (H / 12));
            pPc->Zoom++;
            return 1;
        }
    }
    pPc->Zoom = 0;
    pG->Unk75EF = 0;
    return 0;
}

/* window zoom frame: four white LINE_F2 */
void func_801C2608(LONG X0, LONG Y0, LONG X1, LONG Y1)
{
    PC_WORK *pPc = Pc_wk;
    PC_LINE *pLine = pPc->Line[G.Cbuf_id];
    LONG i;
    ULONG Rgb = 0xFFFFFF;

    for (i = 3; i >= 0; i--)
        pLine[i].Rgbc = Rgb;
    pLine[0].X0 = X0;
    pLine[0].X1 = X1;
    pLine[1].X0 = X0;
    pLine[1].X1 = X1;
    pLine[2].X0 = X0;
    pLine[2].X1 = X0;
    pLine[3].X0 = X1;
    pLine[3].X1 = X1;
    pLine[0].Y0 = Y0;
    pLine[0].Y1 = Y0;
    pLine[1].Y0 = Y1;
    pLine[1].Y1 = Y1;
    pLine[2].Y0 = Y0;
    pLine[2].Y1 = Y1;
    pLine[3].Y0 = Y0;
    pLine[3].Y1 = Y1;
    for (i = 3; i >= 0; i--) {
        SetLineF2((LINE_F2 *)&pPc->Line[G.Cbuf_id][i]);
        AddPrim(G.Ot0[G.Cbuf_id], &pPc->Line[G.Cbuf_id][i]);
    }
}

/* keyboard key number -> character */
ULONG func_801C270C(UCHAR Key)
{
    return Key + 0x3F;
}

/*
 * keyboard input. Max == 0: yes/no or one of four choices (Mode 0/1); else a
 * password of up to Max keys, echoed as '*' when Mode is set. Returns 1 on RETURN
 * (Input holds the typed text, 0 terminated) or cancel.
 */
ULONG func_801C2718(ULONG Max, ULONG Mode)
{
    PC_WORK *pPc = Pc_wk;
    LONG Done = 0;
    ULONG i;
    ULONG Ch;

    if (func_801C29F0()) {
        pPc->Push = 1;
        if (pPc->Key == 1) {
            pPc->Input_num = 0;
            goto Enter;
        }
        if (Max == 0) {
            switch (Mode) {
            case 0:
                pPc->Input[pPc->Input_num] = pPc->Key;
                if (pPc->Key == 0x1A) {
                    pPc->Input_num = 1;
                    func_801C2F80('Y');
                    goto Enter;
                }
                if (pPc->Key == 0xF) {
                    pPc->Input_num = 1;
                    func_801C2F80('N');
                    goto Enter;
                }
                return 0;
            case 1:
                pPc->Input[pPc->Input_num] = pPc->Key;
                if (pPc->Key == 2) {
                    pPc->Input_num = 1;
                    func_801C2F80('A');
                    goto Enter;
                }
                if (pPc->Key == 3) {
                    pPc->Input_num = 1;
                    func_801C2F80('B');
                    goto Enter;
                }
                if (pPc->Key == 4) {
                    pPc->Input_num = 1;
                    func_801C2F80('C');
                    goto Enter;
                }
                if (pPc->Key == 5) {
                    pPc->Input_num = 1;
                    func_801C2F80('D');
                    goto Enter;
                }
                return 0;
            case 2:
                pPc->Input_num = 1;
                goto Enter;
            }
            return 0;
        } else {
            switch (pPc->Key) {
            case 0x1C:
            Enter:
                func_801C31A4(0, "]", &Done);
                for (i = 0; i < pPc->Input_num; i++)
                    pPc->Input[i] = func_801C270C(pPc->Input[i]);
                pPc->Input[i] = 0;
                pPc->Input_num = 0;
                return 1;
            case 0x1D:
                if (pPc->Input_num) {
                    pPc->Col--;
                    pPc->Input_num--;
                    func_801C2F80(' ');
                    return 0;
                }
                break;
            default:
                if (pPc->Input_num >= Max)
                    return 0;
                Ch = '*';
                if (Mode == 0)
                    Ch = (UCHAR)func_801C270C(pPc->Key);
                func_801C2F80(Ch);
                pPc->Input[pPc->Input_num] = pPc->Key;
                pPc->Col++;
                pPc->Input_num++;
                break;
            }
        }
    }
    return 0;
}
/* the original .rodata has a stray " " string after "]" */
__asm__(".section .rodata\n\t.byte 0x20, 0\n\t.section .text");

/* window box sprite; draft in scratchpad/drafts: register allocation differs */
INCLUDE_ASM("asm/nonmatchings/ovl/pc_sys/pc_sys", func_801C2998);

/* on-screen keyboard: cursor moves, returns the key pressed (0 none, 1 cancel) */
ULONG func_801C29F0(void)
{
    PC_WORK *pPc = Pc_wk;
    LONG Move;

    Move = 0;
    func_80038634(0xF000, 0x20A);
    if (G.Unk2100 & 0x1000) {
        pPc->Key_y--;
        Move = 1;
        /* x 9, y 1 is the lower half of the two-row RETURN key */
        if ((*(ULONG *)pPc & 0xFFFF0000) == 0x01090000)
            pPc->Key_y = 0;
        if (pPc->Key_y < 0)
            pPc->Key_y = 2;
    }
    if (G.Unk2100 & 0x4000) {
        Move = 1;
        if (++pPc->Key_y >= 3)
            pPc->Key_y = 0;
    }
    if (G.Unk2100 & 0x8000) {
        Move = 1;
        if (--pPc->Key_x < 0)
            pPc->Key_x = 9;
    }
    if (G.Unk2100 & 0x2000) {
        Move = 1;
        if (++pPc->Key_x >= 10)
            pPc->Key_x = 0;
    }
    if (Move)
        Snd_se_stad(0x210, NULL, 0, 0);
    pPc->Kido += pPc->Kido_add;
    if (pPc->Kido == 0x60 || pPc->Kido == 0x40)
        pPc->Kido_add *= -1;
    pPc->Spr[PC_SPR_KEY].s.Col = pPc->Kido;
    pPc->Spr[PC_SPR_KEY].s.W = 0x18;
    pPc->Spr[PC_SPR_KEY].s.X = pPc->Key_x * 0x18 + 0x34;
    pPc->Spr[PC_SPR_KEY].s.Y = pPc->Key_y * 0x18 + 0x9A;
    pPc->Spr[PC_SPR_KEY].s.U = pPc->Key_x * 0x18 + 4;
    pPc->Spr[PC_SPR_KEY].s.V = pPc->Key_y * 0x18 + 0x2A - pPc->Push;
    pPc->Spr[PC_SPR_KEY].s.H = 0x17 - pPc->Push;
    if (pPc->Key_x == 9 && pPc->Key_y > 0) {
        pPc->Spr[PC_SPR_KEY].s.Y = 0xB2;
        pPc->Spr[PC_SPR_KEY].s.V = 0x42 - pPc->Push;
        pPc->Spr[PC_SPR_KEY].s.H = 0x2F - pPc->Push;
    }
    pPc->Push = 0;
    if (G.Key_trg & 0x1000) {
        Snd_se_stad(0x209, NULL, 0, 0);
        pPc->Key = pPc->Key_x + pPc->Key_y * 10;
        if (pPc->Key == 9)
            pPc->Key = 0x1E;
        if (pPc->Key < 9)
            pPc->Key++;
        if (pPc->Key == 0x13)
            pPc->Key = 0x1D;
        if (pPc->Key >= 0x14)
            pPc->Key--;
        pPc->Blink = 0x10;
    } else {
        pPc->Key = 0;
    }
    if (G.Key_trg & 0x2000)
        pPc->Key = 1;
    return pPc->Key;
}

void func_801C2D34(PC_SPR *pDst, PC_SPR *pSrc)
{
    pDst->w.Xy = pSrc->w.Xy;
    pDst->w.Wh = pSrc->w.Wh;
    pDst->w.Uv_clut = pSrc->w.Uv_clut + ((pSrc->s.Clut + 0x7B40) << 16);
    pDst->w.Attr = pSrc->s.Ot + (pSrc->s.Code << 8) + (pSrc->s.Col << 16) + (pSrc->s.On << 24);
}

/* sprites 0..Num-1 -> SPRT packets (semi-transparent code + grey level) */
void func_801C2D94(ULONG Num)
{
    PC_SPRT *pPkt = Pc_wk->Sprt[G.Cbuf_id];
    PC_SPR *pSpr = Pc_wk->Spr;
    PC_SPR *pEnd = &pSpr[Num];

    do {
        if (pSpr->s.On) {
            pPkt->Tag = 0x4000000;
            pPkt->Rgbc = (pSpr->s.Code << 24) + 0x64000000 + pSpr->s.Col + (pSpr->s.Col << 8) + (pSpr->s.Col << 16);
            pPkt->Xy = pSpr->w.Xy;
            pPkt->Wh = pSpr->w.Wh;
            pPkt->Uv_clut = pSpr->w.Uv_clut;
            AddPrim(&G.Ot0[G.Cbuf_id][pSpr->s.Ot], pPkt);
            pPkt++;
        }
    } while (++pSpr < pEnd);
    func_801C2EBC();
}

/* one DR_MODE per OT slot */
void func_801C2EBC(void)
{
    PC_WORK *pPc = Pc_wk;
    LONG i;

    i = 8;
    do {
        i--;
        SetDrawMode(&pPc->Dm[G.Cbuf_id][i], 0, 0, 0x85, NULL);
        AddPrim(&G.Ot0[G.Cbuf_id][i], &pPc->Dm[G.Cbuf_id][i]);
    } while (i != 0);
}

/* put a character at the text cursor */
void func_801C2F80(UCHAR Code)
{
    Pc_wk->Text[Pc_wk->Row][Pc_wk->Col] = Code;
}

/* scroll the text screen up one row */
void func_801C2FB4(void)
{
    PC_WORK *pPc = Pc_wk;
    ULONG i;

    for (i = 0; i < PC_ROW_NUM - 1; i++) {
        func_8001012C(pPc->Text[i], pPc->Text[i + 1], PC_COL_NUM);
        func_8001012C(pPc->Attr[i], pPc->Attr[i + 1], PC_COL_NUM);
    }
    for (i = 0; i < PC_COL_NUM; i++) {
        pPc->Text[PC_ROW_NUM - 1][i] = ' ';
        pPc->Attr[PC_ROW_NUM - 1][i] = 0;
    }
}

/* clear the text screen */
void func_801C3074(void)
{
    PC_WORK *pPc = Pc_wk;
    UCHAR *pText = pPc->Text[0];
    UCHAR *pAttr = pPc->Attr[0];
    ULONG i;

    for (i = 0; i < PC_ROW_NUM * PC_COL_NUM; i++) {
        *pText++ = ' ';
        *pAttr++ = 0;
    }
    pPc->Color = 0;
    pPc->Row = 0;
    pPc->Col = 0;
}

/* character sprites + text cursor; draft in scratchpad/drafts: 7 scheduling diffs */
INCLUDE_ASM("asm/nonmatchings/ovl/pc_sys/pc_sys", func_801C30B8);

/*
 * type out pStr, one character per Wait frames (all at once when Wait is 0).
 * Codes: ']' new line, '[nnn' pause nnn frames, '^nn' column back nn, '|n' colour n.
 * Sets *pDone and returns 1 when the string is finished.
 */
ULONG func_801C31A4(LONG Wait, UCHAR *pStr, LONG *pDone)
{
    PC_WORK *pPc = Pc_wk;
    UCHAR Put;
    UCHAR Ofs;
    UCHAR *pChr;

    if (*pDone)
        return 0;
    if (pPc->pStr != pStr) {
        pPc->pStr = pStr;
        pPc->pCur = pStr;
        pPc->Wait = 1;
    }
    if (--pPc->Wait)
        return 0;
    pPc->Wait = Wait;
    Put = 0;
    while (*pPc->pCur) {
        switch (*pPc->pCur) {
        case ']':
            pPc->Col = 0;
            pPc->Row++;
            break;
        case '[':
            pPc->Wait = (*++pPc->pCur - '0') * 100;
            pPc->Wait += (*++pPc->pCur - '0') * 10;
            pPc->Wait += *++pPc->pCur - '0';
            pPc->pCur++;
            return 0;
        case '^':
            Ofs = (*++pPc->pCur - '0') * 10;
            Ofs += *++pPc->pCur - '0';
            pPc->Col -= Ofs;
            break;
        case '|':
            pPc->pCur++;
            pPc->Color = *pPc->pCur - '0';
            break;
        default:
            Put = 1;
            pChr = &pPc->Text[pPc->Row][pPc->Col];
            *pChr = *pPc->pCur;
            pChr = &pPc->Attr[pPc->Row][pPc->Col];
            *pChr = pPc->Color;
            pPc->Blink = 0x10;
            pPc->Col++;
            break;
        }
        if (pPc->Col >= PC_COL_NUM) {
            pPc->Col = 0;
            pPc->Row++;
        }
        if (pPc->Row >= PC_ROW_NUM) {
            pPc->Row--;
            func_801C2FB4();
        }
        Snd_se_stad(0x210, NULL, 0, 0);
        pPc->pCur++;
        if (Wait && Put)
            return 0;
    }
    pPc->pStr = NULL;
    return *pDone = 1;
}

/* terminal 0: R.P.D. notice with today's safe password */
void func_801C347C(void)
{
    tagGLOBAL *pG;
    ULONG Rno;

    if (func_801C23D0() == 0) {
        switch (Pc0_mode) {
        case 0:
            if (Pc0_done && (G.Key & 0x1000))
                Pc0_skip = 1;
            break;
        case 1:
            if (Pc0_done) {
                if (G.Unk20FC & 0xF0)
                    Pc0_skip = Pc0_mode;
                if (G.Unk20FC & 0xF)
                    Pc0_skip = Pc0_mode;
            }
            break;
        }
        if (Pc0_skip) {
            Pc0_voice = NULL;
            Pc0_voice2 = NULL;
        }
        switch (Rno = G.Unk75ED) {
        case 0:
            pG = &G;
            Pc_wk = (PC_WORK *)pG->pEm_mem;
            pG->pEm_mem = (struct tagENEMY_WORK *)((PC_WORK *)pG->pEm_mem + 1);
            func_801C44F4(((USHORT *)pG->pUnk21DC)[2], 1);
            Pc0_pass_no = 0;
            if (Flg_ck(pG->Unk7868, 8)) {
                if (Flg_ck(pG->Unk7868, 0x3B)) {
                    if (Flg_ck(pG->Unk7868, 0x3C))
                        Pc0_pass_no = 3;
                    else
                        Pc0_pass_no = 2;
                } else if (Flg_ck(pG->Unk7868, 0x3C)) {
                    Pc0_pass_no = 1;
                } else {
                    Pc0_pass_no = 0;
                }
            } else {
                if (pG->Unk784A & 0x200) {
                    Flg_on(pG->Unk7868, 0x3B);
                    if (pG->Unk784A & 0x40) {
                        Pc0_pass_no = 3;
                        Flg_on(pG->Unk7868, 0x3C);
                    } else {
                        Pc0_pass_no = 2;
                    }
                } else if (pG->Unk784A & 0x40) {
                    Pc0_pass_no = 1;
                    Flg_on(pG->Unk7868, 0x3C);
                } else {
                    Pc0_pass_no = 0;
                }
                Flg_on(G.Unk7868, 8);
            }
            Pc0_done = 0;
            Pc0_skip = 0;
            G.Unk75ED = 1;
            break;
        case 1:
            Pc0_voice = &Pc_voice_data[Pc_voice_ofs[0]];
            func_801C31A4(1, "NOTICE TO STARS PERSONNEL]", &Pc0_done);
            if (Pc0_done & Pc0_skip)
                G.Unk75ED = 2;
            break;
        case 2:
            func_801C31A4(1, ".[025.[025.]", &Pc0_done);
            if (Pc0_done)
                G.Unk75ED = 3;
            break;
        case 3:
            Pc0_voice2 = &Pc_voice_data[Pc_voice_ofs[1]];
            Pc0_voice = &Pc_voice_data[Pc_voice_ofs[2]];
            func_801C31A4(1, "Due to the emergency, the]", &Pc0_done);
            if (Pc0_done)
                G.Unk75ED = 4;
            break;
        case 4:
            Pc0_voice2 = &Pc_voice_data[Pc_voice_ofs[1]];
            Pc0_voice = &Pc_voice_data[Pc_voice_ofs[2]];
            func_801C31A4(1, "key to the STARS Office has]been moved to the evidence]room.]]", &Pc0_done);
            if (Pc0_done & Pc0_skip)
                G.Unk75ED = 5;
            break;
        case 5:
            Pc0_voice2 = &Pc_voice_data[Pc_voice_ofs[3]];
            Pc0_voice = &Pc_voice_data[Pc_voice_ofs[Pc0_pass_no + 4]];
            func_801C31A4(1, "Today's password for the]safe is ", &Pc0_done);
            if (Pc0_done)
                G.Unk75ED = 6;
            break;
        case 6:
            Pc0_voice2 = &Pc_voice_data[Pc_voice_ofs[3]];
            Pc0_voice = &Pc_voice_data[Pc_voice_ofs[Pc0_pass_no + 4]];
            switch (Pc0_pass_no) {
            case 0:
                func_801C31A4(1, "|20513|0.]", &Pc0_done);
                break;
            case 1:
                func_801C31A4(1, "|20131|0.]", &Pc0_done);
                break;
            case 2:
                func_801C31A4(1, "|24011|0.]", &Pc0_done);
                break;
            case 3:
                func_801C31A4(1, "|24312|0.]", &Pc0_done);
                break;
            default:
                func_801C31A4(1, "****.]", &Pc0_done);
                break;
            }
            if (Pc0_done & Pc0_skip)
                func_801C4624(1);
        case 7:
            if (G.Unk20FC & 0x20)
                func_801C4624(1);
            break;
        }
    }
    if ((Pc0_skip & Pc0_done) || Rno != G.Unk75ED) {
        Pc0_voice = NULL;
        Pc0_done = 0;
        Pc0_skip = 0;
        Pc0_voice2 = NULL;
    }
    func_801C30B8(1);
    func_801C2D94(PC_SPR_NUM);
}

/* terminal 1: hospital security system, password for the medical storage room */
void func_801C3A0C(void)
{
    PC_WORK *pPc = Pc_wk;
    SVECTOR Pos = { -0x4D44, -0x9A2, -0x34BC };
    SVECTOR Rot;
    tagGLOBAL *pG;
    ULONG Rno;
    LONG i;

    memset(&Rot, 0, sizeof(Rot));
    Rot.vy = -0x280;
    if (func_801C23D0() == 0) {
        switch (Pc1_mode) {
        case 0:
            if (Pc1_done && (G.Key & 0x1000))
                Pc1_skip = 1;
            break;
        case 1:
            if (Pc1_done) {
                if (G.Unk20FC & 0xF0)
                    Pc1_skip = Pc1_mode;
                if (G.Unk20FC & 0xF)
                    Pc1_skip = Pc1_mode;
            }
            break;
        }
        if (Pc1_skip) {
            Pc1_voice = NULL;
            Pc1_voice2 = NULL;
        }
        switch (Rno = G.Unk75ED) {
        case 0:
            pG = &G;
            pPc = Pc_wk = (PC_WORK *)pG->pEm_mem;
            pG->pEm_mem = (struct tagENEMY_WORK *)((PC_WORK *)pG->pEm_mem + 1);
            func_801C44F4(((USHORT *)pG->pUnk21DC)[2], 1);
            Pc1_done = 0;
            Pc1_skip = 0;
            pG->Unk75ED = 1;
            if (Flg_ck(pG->Unk7868, 0xA3)) {
                Pc1_pass_no = 0;
                if (Flg_ck(pG->Unk7868, 0xA8))
                    Pc1_pass_no = 1;
                if (Flg_ck(pG->Unk7868, 0xA9))
                    Pc1_pass_no = 2;
                Flg_on(pG->Unk7868, 0xAA);
            } else {
                switch (Pc1_pass_no = (SHORT)(pG->Unk784A % 3)) {
                case 0:
                    Flg_off(pG->Unk7868, 0xA8);
                    Flg_off(pG->Unk7868, 0xA9);
                    break;
                case 1:
                    Flg_on(pG->Unk7868, 0xA8);
                    Flg_off(pG->Unk7868, 0xA9);
                    break;
                case 2:
                    Flg_off(pG->Unk7868, 0xA8);
                    Flg_on(pG->Unk7868, 0xA9);
                    break;
                default:
                    Flg_on(G.Unk7868, 0xA8);
                    Flg_off(G.Unk7868, 0xA9);
                    Pc1_pass_no = 1;
                    break;
                }
                Flg_on(G.Unk7868, 0xA3);
            }
            break;
        case 1:
            G.Unk75ED = 2;
            break;
        case 2:
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[8]];
            func_801C31A4(1, "Umbrella Security System]", &Pc1_done);
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 3;
            break;
        case 3:
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[9]];
            func_801C31A4(1, "First Class ]Medical Storage Room:] Security Authorization]", &Pc1_done);
            if (Pc1_done & Pc1_skip) {
                if (Flg_ck(G.Unk7994, 0x1B))
                    G.Unk75ED = 15;
                else
                    G.Unk75ED = 4;
            }
            break;
        case 4:
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[10]];
            func_801C31A4(1, "Current Status: |1Locked|0]", &Pc1_done);
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 5;
            break;
        case 5:
            func_801C31A4(1, "Please enter password and ]then press the RETURN key.]", &Pc1_done);
            if (Pc1_done)
                G.Unk75ED = 6;
            break;
        case 6:
            func_801C31A4(1, "Enter Password:", &Pc1_done);
            if (Pc1_done & Pc1_skip) {
                G.Unk75ED = 7;
                G.Unk75EE = 1;
            }
            break;
        case 7:
            if (func_801C2718(8, 0)) {
                G.Unk75EE = 3;
                switch (Pc1_pass_no) {
                case 0:
                    if (strcmp(pPc->Input, "ADRAVIL") == 0) {
                        G.Unk75ED = 10;
                        func_801C3074();
                    } else {
                        G.Unk75ED = 8;
                    }
                    break;
                case 1:
                    if (strcmp(pPc->Input, "SAFSPRIN") == 0) {
                        G.Unk75ED = 10;
                        func_801C3074();
                    } else {
                        G.Unk75ED = 8;
                    }
                    break;
                case 2:
                    if (strcmp(pPc->Input, "AQUACURE") == 0) {
                        G.Unk75ED = 10;
                        func_801C3074();
                    } else {
                        G.Unk75ED = 8;
                    }
                    break;
                }
            }
            break;
        case 8:
            func_801C31A4(1, "|1ERROR...[020]ERROR...[020]", &Pc1_done);
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[15]];
            if (Pc1_done)
                G.Unk75ED = 9;
            break;
        case 9:
            func_801C31A4(1, "|1Invalid password.]Please re-enter password and ]then press the RETURN key.|0]", &Pc1_done);
            Pc1_voice2 = &Pc_voice_data[Pc_voice_ofs[16]];
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[17]];
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 6;
            break;
        case 10:
            func_801C31A4(1, "Password: Confirmed]", &Pc1_done);
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[11]];
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 11;
            break;
        case 11:
            func_801C31A4(1, "Room Access: Confirmed]", &Pc1_done);
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[12]];
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 12;
            break;
        case 12:
            func_801C31A4(1, "Deactivating lock. ]Please wait[010.[010.[010.]", &Pc1_done);
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[13]];
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 13;
            break;
        case 13:
            func_801C31A4(1, "Current Status: Unlocked]", &Pc1_done);
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[14]];
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 14;
            break;
        case 14:
            /* the lock opens: three effects with sounds */
            Main.Vloop = 2;
            Flg_on(G.Unk7994, 0x1B);
            G.Next_cut_no = 4;
            Main.Unk18E = 1;
            for (i = 0; i < 30; i++) {
                G.Unk208C = (ULONG)((ULONG *)G.pEm_mem + G.Cbuf_id * 13);
                func_80037C38();
                Om_move();
                Esp_task_main();
                Om_trans();
                Eff_sprt_trans();
                Task_sleep(1);
            }
            func_8001B894(0x1010000, NULL);
            func_8001B484(0x1010015, 0x5000000, &GsIDMATRIX, &Pos, &Rot);
            Snd_se_stad(0x204, NULL, 0, 0);
            for (i = 0; i < 40; i++) {
                G.Unk208C = (ULONG)((ULONG *)G.pEm_mem + G.Cbuf_id * 13);
                func_80037C38();
                Om_move();
                Esp_task_main();
                Om_trans();
                Eff_sprt_trans();
                Task_sleep(1);
            }
            func_8001B894(0x1010000, NULL);
            func_8001B484(0x2010015, 0x5000000, &GsIDMATRIX, &Pos, &Rot);
            Snd_se_stad(0x225, NULL, 0, 0);
            for (i = 0; i < 60; i++) {
                G.Unk208C = (ULONG)((ULONG *)G.pEm_mem + G.Cbuf_id * 13);
                func_80037C38();
                Om_move();
                Esp_task_main();
                Om_trans();
                Eff_sprt_trans();
                Task_sleep(1);
            }
            G.Next_cut_no = 9;
            Main.Unk18E = 1;
            G.Unk75ED = 0x11;
            Main.Vloop = 1;
            break;
        case 15:
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[14]];
            func_801C31A4(1, "Current Status: Unlocked]", &Pc1_done);
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 16;
            break;
        case 16:
            Pc1_voice = &Pc_voice_data[Pc_voice_ofs[18]];
            func_801C31A4(1, "System Status: Normal]", &Pc1_done);
            if (Pc1_done & Pc1_skip)
                G.Unk75ED = 17;
            break;
        case 17:
            func_801C4624(0);
            return;
        }
        if (pPc->Key == 1) {
            pPc->Key = 0;
            G.Unk75ED = 0x11;
        }
    }
    if ((Pc1_skip & Pc1_done) || Rno != G.Unk75ED) {
        Pc1_voice = NULL;
        Pc1_done = 0;
        Pc1_skip = 0;
        Pc1_voice2 = NULL;
    }
    func_801C30B8(1);
    func_801C2D94(PC_SPR_NUM);
}

/* open a terminal: work init, sprites, zoom the window open */
void func_801C44F4(ULONG Cut, LONG Zoom)
{
    tagGLOBAL *pG;
    PC_WORK *pPc;
    ULONG i;

    Main.Vloop = 1;
    pPc = Pc_wk;
    func_801C3074();
    for (i = 0; i < PC_SPR_NUM; i++)
        func_801C2D34(&pPc->Spr[i], &Pc_spr_def[i]);
    pPc->Kido = 0x40;
    pPc->Kido_add = 4;
    pG = &G;
    pPc->Key_y = 0;
    pPc->Key_x = 0;
    pPc->Blink = 0;
    pPc->Input_num = 0;
    pPc->Zoom = 0;
    pPc->Cut_bak = pG->Next_cut_no;
    pG->Next_cut_no = Cut;
    Main.Unk18E = 1;
    Task_sleep(1);
    if (Zoom) {
        Snd_se_stad(0x206, NULL, 0, 0);
        pG->Unk75EF = 0;
        while (func_801C24E0(0xC, 0xA, 0xFC, 0x8B)) {
            func_801C2D94(PC_SPR_NUM);
            Task_sleep(1);
        }
    }
}

/* close the terminal: zoom the window shut, free the work, end the task */
void func_801C4624(LONG Zoom)
{
    PC_WORK *pPc = Pc_wk;

    if (Zoom) {
        Snd_se_stad(0x207, NULL, 0, 0);
        G.Unk75EF = 1;
        while (func_801C24E0(0xC, 0xA, 0xFC, 0x8B))
            Task_sleep(1);
    }
    G.Unk75EC = 0;
    G.Unk75ED = 0;
    G.pEm_mem = (struct tagENEMY_WORK *)((PC_WORK *)G.pEm_mem - 1);
    G.Next_cut_no = pPc->Cut_bak;
    Main.Unk18E = 1;
    Main.Vloop = 2;
    Task_signal(0);
    Task_exit();
}
