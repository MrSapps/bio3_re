/* not Capcom code: built with a gcc 2.7.2 and plain GNU as (see Makefile WARNING_CC1) */
#include "common.h"
#include "ovl/warning.h"

/* pick the check and the message language from the BIOS region */
void func_801840F8(void)
{
    switch (Bios_region) {
    case 'A':
        Warn_state = 1;
        Warn_lang_no = 1;
        break;
    case 'E':
        Warn_state = 0;
        Warn_lang_no = 2;
        break;
    case 'I':
    default:
        Warn_state = 1;
        Warn_lang_no = 0;
        break;
    }
}

/*
 * Mod-chip check state machine (Warn_state), one step per call, all steps while
 * Once == 0. Commands are Cd_cmd_tbl indices: GetTN, Init, GetTD of the last
 * track, ReadTOC, GetID, Setloc/Setmode/SeekP to half of that position, Mute,
 * Play, Test 0x19 sub 4 and 5; a non-zero Test answer or a failing GetID ends in
 * func_80184D60. Returns the state (busy), -1 (error, retry) or 0 (done).
 */
LONG func_80184164(LONG Once)
{
    LONG Ret;
    LONG Min;
    LONG Sec;
    LONG Pos;

    Ret = 0;
loop:
    switch (Warn_state) {
    case 1:
        func_80184C70(1);
        Warn_state = 2;
        Ret = 1;
        break;
    case 2:
        switch (Ret = func_80184A9C(1)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = 2;
            break;
        case 1:
            Warn_last_track = Cd_result[2];
            func_80184C70(6);
            Warn_state = 3;
            Ret = 2;
            break;
        case -1:
            func_80184C70(1);
            Ret = 2;
            break;
        case 0:
        default:
            Ret = 2;
            break;
        }
        break;
    case 3:
        switch (Ret = func_80184A9C(6)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Cd_param[0] = Warn_last_track >= 2 ? 2 : 0;
            func_80184C70(2);
            Warn_state = 4;
        case 0:
        default:
            Ret = 3;
            break;
        }
        break;
    case 4:
        switch (Ret = func_80184A9C(2)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Min = (Cd_result[1] >> 4) * 10 + (Cd_result[1] & 0xF);
            Sec = (Cd_result[2] >> 4) * 10 + (Cd_result[2] & 0xF);
            Pos = (Min * 60 + Sec) >> 1;
            Warn_min = Pos / 60;
            Warn_sec = Pos % 60;
            Warn_min = ((Warn_min / 10) << 4) | (Warn_min % 10);
            Warn_sec = ((Warn_sec / 10) << 4) | (Warn_min % 10); /* sic */
            func_80184C70(12);
            Warn_state = 5;
        case 0:
        default:
            Ret = 7; /* sic, not 4 */
            break;
        }
        break;
    case 5:
        switch (Ret = func_80184A9C(12)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            if (!(Cd_result[0] & 1))
                Warn_state = 1;
            else if (Cd_result[1] & 0x40) {
                Cd_param[0] = Warn_min;
                Cd_param[1] = Warn_sec;
                Cd_param[2] = 0;
                func_80184C70(3);
                Warn_state = 7;
                Ret = 5;
            }
            break;
        case 1:
            func_80184C70(13);
            Warn_state = 6;
        case 0:
        default:
            Ret = 5;
            break;
        }
        break;
    case 6:
        switch (Ret = func_80184A9C(13)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            func_80184D60();
            break;
        case 1:
            Cd_param[0] = Warn_min;
            Cd_param[1] = Warn_sec;
            Cd_param[2] = 0;
            func_80184C70(3);
            Warn_state = 7;
        case 0:
        default:
            Ret = 6;
            break;
        }
        break;
    case 7:
        switch (Ret = func_80184A9C(3)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Cd_param[0] = 1;
            func_80184C70(5);
            Warn_state = 8;
        case 0:
        default:
            Ret = 7;
            break;
        }
        break;
    case 8:
        switch (Ret = func_80184A9C(5)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Warn_vsync = VSync(-1);
            Warn_state = 9;
        case 0:
        default:
            Ret = 8;
            break;
        }
        break;
    case 9:
        if (VSync(-1) > Warn_vsync + 3) {
            func_80184C70(4);
            Warn_state = 10;
        }
        Ret = 9;
        break;
    case 10:
        switch (Ret = func_80184A9C(4)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            func_80184C70(7);
            Warn_state = 11;
        case 0:
        default:
            Ret = 10;
            break;
        }
        break;
    case 11:
        switch (Ret = func_80184A9C(7)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            func_80184C70(8);
            Warn_state = 12;
        case 0:
        default:
            Ret = 11;
            break;
        }
        break;
    case 12:
        switch (Ret = func_80184A9C(8)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Cd_param[0] = 4;
            func_80184C70(9);
            Warn_state = 13;
        case 0:
        default:
            Ret = 12;
            break;
        }
        break;
    case 13:
        switch (Ret = func_80184A9C(9)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Warn_vsync = VSync(-1);
            Warn_state = 14;
        case 0:
        default:
            Ret = 13;
            break;
        }
        break;
    case 14:
        if (VSync(-1) > Warn_vsync + 200) {
            Cd_param[0] = 5;
            func_80184C70(10);
            Warn_state = 15;
        }
        Ret = 14;
        break;
    case 15:
        switch (Ret = func_80184A9C(10)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            if (Cd_result[1] != 0) {
                func_80184C70(0);
                Warn_state = 16;
            } else {
                Warn_vsync = VSync(-1);
                Warn_state = 19;
            }
        case 0:
        default:
            Ret = 15;
            break;
        }
        break;
    case 16:
        switch (Ret = func_80184A9C(0)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            func_80184D60();
        case 0:
        default:
            Ret = 16;
            break;
        }
        break;
    case 17:
        switch (Ret = func_80184A9C(0)) {
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Warn_state = 1;
            Ret = 16;
            break;
        case 0:
        default:
            Ret = 16;
            break;
        case -2:
            func_80184C70(0);
            Ret = 16;
            break;
        }
        break;
    case 18:
        switch (Ret = func_80184A9C(11)) {
        case -2:
            func_80184C70(0);
            Warn_state = 17;
            Ret = -1;
            break;
        case -1:
            Warn_state = 1;
            break;
        case 1:
            Warn_state = 0;
        case 0:
        default:
            Ret = 18;
            break;
        }
        break;
    case 19:
        if (VSync(-1) > Warn_vsync + 10) {
            func_80184C70(11);
            Warn_state = 18;
        }
        Ret = 19;
        break;
    case 0:
        Ret = 0;
        break;
    }
    if (!Once && Ret != 0)
        goto loop;
    return Ret;
}

/* poll the CD interrupt for Cd_cmd_tbl[Cmd]: 0 busy, 1 done (Cd_result read), -1 error (INT5), -2 lid open (stat 0x10) */
LONG func_80184A9C(LONG Cmd)
{
    LONG Nint;
    signed char Irq;
    LONG i;
    LONG j;
    UCHAR Int;

    Nint = Cd_cmd_tbl[Cmd].Nint;
    *pCd_reg0 = 1;
    Irq = *pCd_reg3 & 7;
    if (Irq == (*pCd_reg3 & 7)) {
        Int = Irq;
        if (Int != 0) {
            Cd_irq_sum += Int;
            *pCd_reg0 = 1;
            *pCd_reg3 = 7;
            for (j = 0; j < 4; j++)
                *(volatile LONG *)0 = j;
            if (Cd_irq_sum >= Nint) {
                Cd_irq_sum = 0;
                if (Irq == 5) {
                    Cd_result[0] = *pCd_reg1;
                    Cd_result[1] = *pCd_reg1;
                    *pCd_reg0 = 1;
                    *pCd_reg2 = 0x1F;
                    if (Cd_result[0] & 0x10)
                        return -2;
                    return -1;
                }
                for (i = 0; i < Cd_cmd_tbl[Cmd].Nresult; i++)
                    Cd_result[i] = *pCd_reg1;
                *pCd_reg0 = 1;
                *pCd_reg2 = 0x1F;
                if (Cmd == 10)
                    return 1;
                if (Cd_result[0] & 0x10)
                    return -2;
                return 1;
            }
        }
    }
    return 0;
}

/* send a command to the CD controller */
void func_80184C70(LONG Cmd)
{
    LONG i;

    *pCd_reg0 = 1;
    *pCd_reg3 = 7;
    for (i = 0; i < 4; i++)
        *(volatile LONG *)0 = i;
    *pCd_reg0 = 1;
    *pCd_reg2 = 0x18;
    *pCd_reg0 = 0;
    for (i = 0; i < Cd_cmd_tbl[Cmd].Nparam; i++)
        *pCd_reg2 = Cd_param[i];
    *pCd_reg0 = 0;
    *pCd_reg1 = Cd_cmd_tbl[Cmd].Cmd;
}

/* the check failed: draw the message and the red frame with the BIOS font and stop */
void func_80184D60(void)
{
    DRAWENV Draw;
    DISPENV Disp;
    DR_ENV Env;
    ULONG Mask[3];
    WARN_POS Pos;
    LONG i;
    ULONG Col;

    ResetGraph(1);
    StopCallback();
    ResetGraph(5);
    *(USHORT *)0x1F801DAA = 0;
    SetDefDrawEnv(&Draw, 0, 0, 320, 240);
    SetDefDispEnv(&Disp, 0, 0, 320, 240);
    Draw.isbg = 1;
    SetDrawEnv(&Env, &Draw);
    DrawPrim(&Env);
    PutDispEnv(&Disp);
    Mask[0] = 0x02000000;
    Mask[1] = 0xE6000002;
    Mask[2] = 0;
    DrawPrim(Mask);
    Col = 0xFFFF;
    Pos.H = 1;
    Pos.W = Warn_lang[Warn_lang_no].Char_w;
    for (i = 0; i < 2; i++) {
        Pos.X = Warn_lang[Warn_lang_no].X + i;
        Pos.Y = Warn_lang[Warn_lang_no].Y + i;
        func_80184ECC(Warn_lang[Warn_lang_no].pMess, &Pos, Col);
        Col = 0x8000;
    }
    func_80185288();
    SetDispMask(1);
    exit();
}

/* print a message: Shift-JIS through Krom2RawAdd (Japanese) or the BIOS 8x15 ASCII font */
void func_80184ECC(UCHAR *pStr, WARN_POS *pPos, ULONG Col)
{
    UCHAR *pTop;
    UCHAR *pFont;
    LONG Len;
    LONG X;
    ULONG Bits;
    LONG Code;

    Len = strlen(pStr);
    pTop = pStr;
    X = pPos->X;
    for (; pStr < pTop + Len; pStr++) {
        if (*pStr == '\n') {
            pPos->X = X;
            pPos->Y += 18;
        } else if (Warn_lang_no == 0) {
            Code = *pStr++ << 8;
            pFont = (UCHAR *)Krom2RawAdd(*pStr | Code);
            func_80185040(pPos, pFont, Col, 7);
            pPos->X += pPos->W + 1;
        } else if (*pStr == ' ') {
            pPos->X += 8;
        } else {
            pFont = (UCHAR *)(0xBFC7F8DE + (*pStr - 0x21) * 15);
            Bits = func_801851F4(pFont);
            pPos->W = Bits - (Bits >> 16) + 1;
            func_80185040(pPos, pFont, Col, Bits);
            pPos->X += pPos->W + 2;
        }
    }
}

/* draw one glyph (Font_w bytes x 15 rows, bits Left..Right of each byte) twice, 1 pixel apart */
void func_80185040(WARN_POS *pPos, UCHAR *pFont, ULONG Col, ULONG Bits)
{
    WARN_IMG Img;
    USHORT *pPix;
    LONG X;
    LONG Y;
    LONG Row;
    LONG i;
    LONG Bit;
    USHORT Right;
    USHORT Left;

    Left = Bits;
    Right = Bits >> 16;
    X = pPos->X;
    Y = pPos->Y;
    Img.Tag = (((pPos->W + 1) >> 1) + 3) << 24;
    Img.Code = 0xA0000000;
    Img.Wh = *(ULONG *)&pPos->W;
    for (Row = 0; Row < 15; Row++) {
        pPix = Img.Pix;
        for (i = 0; i < Warn_lang[Warn_lang_no].Font_w; i++, pFont++) {
            for (Bit = Left; Bit >= Right; Bit--)
                *pPix++ = ((*pFont >> Bit) & 1) ? Col : 0;
        }
        for (i = 0; i < 2; i++) {
            Img.Xy = *(ULONG *)&pPos->X;
            DrawPrim(&Img);
            pPos->X++;
        }
        pPos->X = X;
        pPos->Y++;
    }
    pPos->Y = Y;
}

/* horizontal extent of a 8x15 BIOS font glyph: (rightmost bit << 16) | leftmost bit (bit 7 = left) */
ULONG func_801851F4(UCHAR *pFont)
{
    USHORT Left;
    USHORT Right;
    LONG First;
    LONG Bit;
    LONG Row;

    Left = 0;
    Right = 7;
    for (Row = 14; Row >= 0; Row--) {
        First = 1;
        for (Bit = 7; Bit >= 0; Bit--) {
            if ((*pFont >> Bit) & 1) {
                if (First) {
                    if (Left < Bit)
                        Left = Bit;
                    if (Bit < Right)
                        Right = Bit;
                    First = 0;
                } else if (Bit < Right)
                    Right = Bit;
            }
        }
        pFont++;
    }
    return (Right << 16) | Left;
}

INCLUDE_RODATA("asm/nonmatchings/ovl/warning/warning", D_80184050);
