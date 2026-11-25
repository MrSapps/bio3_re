/* ---- from 80020298.c ---- */
#include "mojidisp.h"
#include "monitor.h"
#include "sub.h"
#include "sce0.h"
#include "game.h"
#include "snd.h"
#include "status.h"

/* GTE inline macros (PsyQ inline_c.h with the DMPSX-converted opcodes) */
#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stsv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 2(%0); sh $14, 4(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_gpf12_b() __asm__ volatile(".word 0x4B98003D")
#define gte_gpl12() __asm__ volatile("nop; nop; .word 0x4BA8003E")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_ldsxy3(r0, r1, r2) __asm__ volatile("mtc2 %0, $12; mtc2 %2, $14; mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldsz3(r0, r1, r2) __asm__ volatile("mtc2 %0, $17; mtc2 %1, $18; mtc2 %2, $19" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile("swc2 $12, 8(%0); swc2 $13, 20(%0); swc2 $14, 32(%0)" : : "r"(r0) : "memory")
#define gte_nclip_b() __asm__ volatile(".word 0x4B400006")
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_avsz3() __asm__ volatile("nop; nop; .word 0x4B58002D")
#define gte_avsz4() __asm__ volatile("nop; nop; .word 0x4B68002E")
#define gte_rtpt() __asm__ volatile("nop; nop; .word 0x4A280030")
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetLightMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $8; ctc2 $13, $9; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $10; ctc2 $13, $11; ctc2 $14, $12" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stclmv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_rtir() __asm__ volatile("nop; nop; .word 0x4A49E012")
#define gte_MulMatrix0(r1, r2, r3) { gte_SetRotMatrix(r1); gte_ldclmv(r2); gte_rtir(); gte_stclmv(r3); gte_ldclmv((char *)(r2) + 2); gte_rtir(); gte_stclmv((char *)(r3) + 2); gte_ldclmv((char *)(r2) + 4); gte_rtir(); gte_stclmv((char *)(r3) + 4); }
#define gte_ldlvl(r0) __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0)" : : "r"(r0))
#define gte_stlvl(r0) __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(r0) : "memory")

void Print_color(UCHAR A0, UCHAR Col, USHORT Attr)
{
    Moji.Unk75C9 = A0;
    Moji.Mess_attr = Attr | (Col << 3);
}

void Print8(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), Moji.Mess_attr, &pFmt);
}

void Print14(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), Moji.Mess_attr | 0x8000, &pFmt);
}

void Print8_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), (Moji.Mess_attr & 0xFF07) | (Col << 3), &pFmt);
}

void Print14_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), ((SHORT)(Moji.Mess_attr & 0xFF07) | 0x8000) | (Col << 3), &pFmt);
}

INCLUDE_ASM("asm/nonmatchings/game/mojidisp", Print_main);

void func_8002F98C(MOJIDISP *pMoji, UCHAR *pStr, ULONG Xy, ULONG Attr)
{
    ULONG Col;
    ULONG Mode;
    ULONG Clut;
    ULONG Tag;
    ULONG c;
    SPRT_8 *pPrim;
    SPRT_8 *pEnd;
    ULONG *pPrev;

    if (*pStr == 0) {
        return;
    }
    Col = (Attr & 0xF8) >> 3;
    Mode = Attr & 7;
    pPrim = pMoji->pPrim8_cur;
    pEnd = &pMoji->Prim_sprt_8[G.Cbuf_id][128];
    Clut = GetClut(0x100, Col | 0x1E0) << 16;
    Col = (Col & 1) << 25;
    pPrev = (ULONG *)&pMoji->Moji_mode[G.Cbuf_id][Mode * 2];
    Tag = *pPrev;
    do {
        if (*pStr != ' ') {
            *(ULONG *)&pPrim->r0 = Col | 0x74808080;
            *(ULONG *)&pPrim->x0 = Xy;
            c = *pStr - 0x20;
            *(ULONG *)&pPrim->u0 = ((c & 0x1F) << 3) + ((c & 0xE0) << 6) + Clut;
            *pPrev = (*pPrev & 0xFF000000) | ((ULONG)pPrim & 0xFFFFFF);
            pPrev = (ULONG *)pPrim;
            pPrim++;
            if (pPrim == pEnd) {
                break;
            }
        }
        pStr++;
        Xy += 8;
    } while (*pStr);
    *pPrev = (*pPrev & 0xFF000000) | (Tag & 0xFFFFFF);
    pMoji->pPrim8_cur = pPrim;
}

void func_8002FB14(MOJIDISP *pMoji, UCHAR *pStr, ULONG Xy, ULONG Attr)
{
    ULONG Clut;
    ULONG Wh;
    ULONG Tag;
    ULONG c;
    LONG W;
    ULONG Mode;
    SPRT *pPrim;
    SPRT *pCur;
    SPRT *pEnd;
    ULONG *pPrev;

    if (*pStr == 0) {
        return;
    }
    pPrim = pMoji->pPrim14_cur;
    pEnd = Moji.Prim_sprt[G.Cbuf_id + 1];
    if (pPrim >= pEnd) {
        return;
    }
    Wh = 0xE000E;
    Clut = GetClut(0x100, ((Attr & 0xF0) >> 3) | 0x1E0) << 16;
    Mode = (USHORT)(Attr & 0x100);
    W = (Attr & 7) * 2;
    pPrev = (ULONG *)&pMoji->Moji_mode[G.Cbuf_id][W];
    Tag = *pPrev;
    do {
        pCur = pPrim;
        if (*pStr != 0x20) {
            W = func_80031970(Mode, *pStr, (LONG *)&Xy);
            *(ULONG *)&pCur->r0 = 0x64808080;
            *(ULONG *)&pCur->x0 = Xy;
            c = *pStr;
            *(ULONG *)&pCur->u0 = (UCHAR)(c % 18) * 14 + ((UCHAR)(c / 18) * 14 << 8) + Clut;
            *(ULONG *)&pCur->w = Wh;
            *pPrev = (*pPrev & 0xFF000000) | ((ULONG)pCur & 0xFFFFFF);
            pPrev = (ULONG *)pCur;
            pPrim = pCur + 1;
            if (pPrim >= pEnd) {
                break;
            }
        } else {
            W = func_80031970(Mode, *pStr, (LONG *)&Xy);
        }
        Xy += W;
        pStr++;
    } while (*pStr);
    *pPrev = (*pPrev & 0xFF000000) | (Tag & 0xFFFFFF);
    pMoji->pPrim14_cur = pPrim;
}

LONG Mess_set(ULONG Pos_xy, USHORT Attr, ULONG Mess_no, ULONG Stop_data)
{
    MOJIDISP *pMoji;
    tagGLOBAL *pG;
    UCHAR *pTop;

    pMoji = &Moji;
    pG = &G;
    if (pMoji->Mess_flg & 0x80) {
        return -1;
    }
    pMoji->Mess_flg = 0x80;
    pG->Unk7848 = 0;
    pMoji->Stop_data = Stop_data;
    pMoji->Mess_r_no0 = 0;
    pMoji->Mess_attr2 = Attr | 0x100;
    pG->Status_flg |= 0x100;
    pMoji->Stop_keep = pG->Unk2468;
    switch (Attr & 0x3000) {
    case 0:
        pMoji->Pos_x_def = 0x22;
        pMoji->Pos_y_def = 0xB9;
        pMoji->Mess_top = D_80098E88 + D_80099654[Mess_no];
        break;
    case 0x1000:
        pMoji->Pos_x_def = 0xE;
        pMoji->Pos_y_def = 0xAD;
        pMoji->Mess_top = D_800996E4 + D_8009BDB4[Mess_no];
        break;
    case 0x2000:
        pMoji->Pos_x_def = 0x22;
        pMoji->Pos_y_def = 0xB9;
        pTop = pG->pRoom->pMessage;
        pMoji->Mess_top = pTop;
        pMoji->Mess_top = pTop + *(USHORT *)(pTop + (Mess_no << 1));
        break;
    default:
        pMoji->Pos_x_def = Pos_xy;
        pMoji->Pos_y_def = Pos_xy >> 16;
        pMoji->Mess_top = (UCHAR *)Mess_no;
        break;
    }
    return 0;
}

void Moji_trans_main(void)
{
    MOJIDISP *pMoji = &Moji;

    if ((pMoji->Mess_flg & 0x40) && !(G.Key & 0x80)) {
        pMoji->Mess_flg &= ~0x40;
    }
    if (pMoji->Mess_flg & 0x80) {
        Mess_disp(pMoji);
        mess_trans(pMoji);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/mojidisp", Mess_disp);
/* ---- from 800303CC.c ---- */

ULONG func_800303CC(ULONG Val)
{
    if (Val == 0) {
        return 0;
    }
    return 0x10000 / Val + 1;
}

INCLUDE_ASM("asm/nonmatchings/game/mojidisp", Branch_disp);

void func_80030764(MOJIDISP *pMoji, UCHAR *pData)
{
    UCHAR *pStr;
    LONG Sel;
    LONG Se;
    ULONG i;
    ULONG Mode;
    ULONG X;
    ULONG Y;
    LINE_F3 *pL;
    UCHAR *pLine[2];
    ULONG Width[2];
    ULONG Pos;
    ULONG Col0;
    ULONG Col1;

    Mode = (USHORT)(pMoji->Mess_attr2 & 0x100);
    if (((*pData++ >> 4) & 7) == 0) {
        pStr = D_80098E80;
    } else {
        pStr = pData;
    }
    Sel = G.Unk7848;
    if (pMoji->Timer != 0) {
        pMoji->Timer--;
    }
    if (D_800C703C.Cnt != 0) {
        if (G.Key_trg & 0x1000) {
            if (pMoji->Timer == 0) {
                G.Key = 0xFFFF;
                G.Status_flg &= 0xEFFFFFFF;
                G.System_flg |= 0x400;
                pMoji->Mess_flg = (pMoji->Mess_flg & 0x7F) | 0x40;
                G.Unk2468 = pMoji->Stop_keep;
                if (pMoji->Mess_attr2 & 0x400) {
                    Se = 6;
                    if (Sel != 0) {
                        if (Sel == 1) {
                            Se = 5;
                        }
                    }
                    Snd_se_stad(Se, 0, 0, 0);
                }
                return;
            }
        } else if (*(ULONG *)&G.Unk20FC & 0x5000) {
            USHORT Key = G.Unk20FC;

            if (Key & 0x4000) {
                Sel++;
            }
            if (Key & 0x1000) {
                Sel--;
            }
            if (Sel >= 2) {
                Sel = 0;
            }
            if (Sel < 0) {
                Sel = 1;
            }
            Snd_se_stad(4, 0, 0, 0);
            pMoji->Timer = 0;
            pMoji->Mess_time = 0;
        }
        G.Unk7848 = Sel;
    }
    for (i = 0; i < 2; i++) {
        pLine[i] = pStr;
        Pos = 0;
        while (1) {
            switch (*pStr) {
            case 0xEA:
            case 0xEB:
            case 0xEC:
            case 0xED:
            case 0xEE:
            case 0xEF:
            case 0xF0:
                pStr += 2;
                Pos += 14;
                continue;
            case 0xF5:
            case 0xF6:
            case 0xF7:
            case 0xFC:
                pStr++;
                continue;
            case 0xF3:
            case 0xF4:
            case 0xF8:
            case 0xF9:
            case 0xFA:
            case 0xFB:
            case 0xFD:
                pStr += 2;
                continue;
            case 0xFE:
                pStr++;
                goto next;
            default:
                Pos += Moji_char_width(Mode, *pStr, (LONG *)&Pos);
                pStr++;
                continue;
            }
        }
    next:
        Width[i] = Pos;
    }
    if (Width[0] > Width[1]) {
        Pos = Width[0];
    } else {
        Pos = Width[1];
    }
    Y = D_800C703F;
    X = ((0xFC - Pos) >> 1) + 0x22;
    for (i = 0; i < 2; i++) {
        Mess_print(X, Y + i * 0x16, pLine[i], pMoji->Mess_attr2);
    }
    Y += Sel * 0x16;
    if (D_800C703C.Cnt == 0) {
        return;
    }
    switch (D_800C703C.Cnt) {
    case 1:
        Col0 = 0x48E02020;
        Col1 = 0x48600000;
        pMoji->Mess_time -= 1;
        break;
    case 2:
        Col0 = 0x4820E0E0;
        Col1 = 0x48006060;
        pMoji->Mess_time -= 2;
        break;
    case 3:
        Col0 = 0x482020E0;
        Col1 = 0x48000060;
        pMoji->Mess_time -= 4;
        break;
    }
    if (pMoji->Mess_time & ((Main.Vloop == 1) ? 0x30 : 0x18)) {
        pL = pMoji->Cursor_line[G.Cbuf_id];
        SetLineF3(pL);
        *(ULONG *)&pL->r0 = Col0;
        pL->x0 = X - 5;
        pL->y0 = Y - 3;
        pL->x1 = Pos + X + 3;
        pL->y1 = Y - 3;
        pL->x2 = Pos + X + 3;
        pL->y2 = Y + 15;
        AddPrim(G.pOt0, pL);
        pL++;
        SetLineF3(pL);
        *(ULONG *)&pL->r0 = Col1;
        pL->x0 = X - 4;
        pL->y0 = Y - 2;
        pL->x1 = Pos + X + 4;
        pL->y1 = Y - 2;
        pL->x2 = Pos + X + 4;
        pL->y2 = Y + 16;
        AddPrim(G.pOt0 + 1, pL);
        pL++;
        SetLineF3(pL);
        *(ULONG *)&pL->r0 = Col0;
        pL->x0 = Pos + X + 3;
        pL->y0 = Y + 15;
        pL->x1 = X - 5;
        pL->y1 = Y + 15;
        pL->x2 = X - 5;
        pL->y2 = Y - 3;
        AddPrim(G.pOt0, pL);
        pL++;
        SetLineF3(pL);
        *(ULONG *)&pL->r0 = Col1;
        pL->x0 = Pos + X + 4;
        pL->y0 = Y + 16;
        pL->x1 = X - 4;
        pL->y1 = Y + 16;
        pL->x2 = X - 4;
        pL->y2 = Y - 2;
        AddPrim(G.pOt0, pL);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/mojidisp", mess_trans);

void Name_disp(LONG x, LONG y, ULONG Attr, ULONG Item)
{
    UCHAR c;
    SPRT *pP;
    DR_TPAGE *pDr_alt;
    ULONG Cbuf;
    ULONG Mode;
    ULONG Clut;
    SPRT *pEnd;
    DR_TPAGE *pOt;
    UCHAR Col;
    DR_TPAGE *pDr_mode;
    LONG W;
    UCHAR *pStr;

    pP = Moji.pPrim14_cur;
    Cbuf = G.Cbuf_id;
    pEnd = Moji.Prim_sprt[Cbuf + 1];
    if (pP >= pEnd) {
        return;
    }
    Col = (Attr & 0xF0) >> 3;
    W = (Attr & 7) * 2;
    pDr_mode = &Moji.Moji_mode[Cbuf][W];
    GetClut(0x100, Col | 0x1E0);
    pStr = name_ptr_set(Item);
    pDr_alt = pDr_mode + 1;
    Attr |= 0x100;
    Mode = Attr & 0x100;
    c = *pStr;
    if (c != 0xF7) {
        do {
            W = 14;
            if (c != 0) {
                switch (*pStr) {
                case 0xEA:
                    pStr++;
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x2E;
                    break;
                case 0xEB:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14;
                    break;
                case 0xEC:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                    break;
                case 0xED:
                    pStr++;
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14;
                    break;
                case 0xEE:
                    pStr++;
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                    break;
                case 0xEF:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14;
                    break;
                case 0xF0:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                    break;
                default:
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 + 0x1C;
                    W = Moji_char_width(Mode, *pStr, &x);
                    break;
                }
                *(ULONG *)&pP->r0 = 0x64808080;
                *(ULONG *)&pP->x0 = (x & 0xFFFF) | (y << 16);
                *(ULONG *)&pP->w = 0xE000E;
                pP->u0 = (UCHAR)(*pStr % 18) * 14;
                pP->clut = Clut;
                AddPrim(pOt, pP);
                pP++;
                if (pP >= pEnd) {
                    break;
                }
            } else {
                W = Moji_char_width(Mode, *pStr, &x);
            }
            x += W;
            pStr++;
            c = *pStr;
        } while (c != 0xF7);
    }
    Moji.pPrim14_cur = pP;
}

void Mess_print(ULONG x, ULONG y, UCHAR *pStr, ULONG Attr)
{
    UCHAR c;
    SPRT *pP;
    DR_TPAGE *pDr_alt;
    ULONG Cbuf;
    ULONG Mode;
    ULONG Clut;
    SPRT *pEnd;
    DR_TPAGE *pOt;
    UCHAR Col;
    DR_TPAGE *pDr_mode;
    LONG W;

    pP = Moji.pPrim14_cur;
    Cbuf = G.Cbuf_id;
    pEnd = Moji.Prim_sprt[Cbuf + 1];
    if (pP >= pEnd) {
        return;
    }
    Col = (Attr & 0xF0) >> 3;
    W = (Attr & 7) * 2;
    Mode = (USHORT)(Attr & 0x100);
    pDr_mode = &Moji.Moji_mode[Cbuf][W];
    GetClut(0x100, Col | 0x1E0);
    pDr_alt = pDr_mode + 1;
    c = *pStr;
    if (c != 0xFE) {
        do {
            W = 14;
            if (c != 0) {
                switch (*pStr) {
                case 0xEA:
                    pStr++;
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x2E;
                    break;
                case 0xEB:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14;
                    break;
                case 0xEC:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                    break;
                case 0xED:
                    pStr++;
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14;
                    break;
                case 0xEE:
                    pStr++;
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                    break;
                case 0xEF:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14;
                    break;
                case 0xF0:
                    pStr++;
                    pOt = pDr_alt;
                    Clut = GetClut(0x100, Col + 0x1EA);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                    break;
                case 0xF5:
                    Moji_char_width(Mode, 0, (LONG *)&x);
                    goto next;
                case 0xF6:
                    x += 7;
                    goto next;
                case 0xF3:
                case 0xF4:
                case 0xF8:
                case 0xF9:
                case 0xFA:
                case 0xFD:
                    pStr++;
                case 0xFC:
                    goto next;
                case 0xF7:
                case 0xFE:
                    return;
                default:
                    pOt = pDr_mode;
                    Clut = GetClut(0x100, Col + 0x1E0);
                    pP->v0 = (UCHAR)(*pStr / 18) * 14 + 0x1C;
                    W = Moji_char_width(Mode, *pStr, (LONG *)&x);
                    break;
                }
                *(ULONG *)&pP->r0 = 0x64808080;
                *(ULONG *)&pP->w = 0xE000E;
                *(ULONG *)&pP->x0 = x | (y << 16);
                pP->u0 = (UCHAR)(*pStr % 18) * 14;
                pP->clut = Clut;
                AddPrim(pOt, pP);
                pP++;
                if (pP >= pEnd) {
                    break;
                }
            } else {
                W = Moji_char_width(Mode, *pStr, (LONG *)&x);
            }
            x += W;
        next:
            pStr++;
            c = *pStr;
        } while (c != 0xFE);
    }
    Moji.pPrim14_cur = pP;
}

UCHAR *name_ptr_set(UCHAR No)
{
    ULONG Flg;
    UCHAR *pTop;
    USHORT *pOfs;

    Flg = Item_data_tbl[No].Chk_flg;
    pTop = D_8009BEE4;
    pOfs = D_8009C7C0;
    if (Flg != 0 && Flg_ck(D_800D2048, Flg)) {
        No = Flg - 0x55;
    }
    return pTop + pOfs[No];
}

LONG func_80031970(ULONG Mode, ULONG Code, LONG *pPos)
{
    ULONG Idx;

    if (Mode != 0) {
        if (Code != 0x20) {
            Idx = Code - 0x24;
            if (Idx < 0x57) {
                Idx += Idx;
                *pPos -= msg_space[Idx];
                return msg_space[Idx + 1] + msg_space[Idx];
            }
        } else {
            *pPos -= msg_space[0];
            return msg_space[1] + msg_space[0];
        }
    }
    return 14;
}

LONG Moji_char_width(ULONG Mode, ULONG Code, LONG *pPos)
{
    if (Mode != 0 && Code < 0x57) {
        Code *= 2;
        *pPos -= msg_space[Code];
        return msg_space[Code + 1] + msg_space[Code];
    }
    return 14;
}

void Moji_init(void)
{
    LONG i;
    LONG j;
    LONG k;
    DR_TPAGE *pDr_mode;
    SPRT *pSpr;
    SPRT_8 *pSpr8;
    MOJIDISP *pMoji;

    pMoji = &Moji;
    pDr_mode = pMoji->Moji_mode[0];
    i = 0x10;
    do {
        SetDrawTPage(pDr_mode, 0, 0, GetTPage(0, 0, 0x300, 0x100));
        SetDrawTPage(pDr_mode + 1, 0, 0, GetTPage(0, 0, 0x340, 0x100));
        pDr_mode += 2;
        i--;
    } while (i != 0);
    pSpr = pMoji->Prim_sprt[0];
    pSpr8 = pMoji->Prim_sprt_8[0];
    j = 0x500;
    k = 0x100;
    do {
        pSpr->tag = 0x4000000;
        j--;
        pSpr++;
    } while (j != 0);
    do {
        pSpr8->tag = 0x3000000;
        k--;
        pSpr8++;
    } while (k != 0);
    Moji_mode_init();
}

void Moji_mode_init(void)
{
    ULONG i;
    DR_TPAGE *pDr_mode;
    ULONG *pOt;
    ULONG Cbuf;

    i = 8;
    Cbuf = G.Cbuf_id;
    pDr_mode = Moji.Moji_mode[Cbuf];
    pOt = G.Ot0[Cbuf];
    do {
        AddPrim(pOt, pDr_mode);
        AddPrim(pOt, pDr_mode + 1);
        pDr_mode += 2;
        i--;
        pOt++;
    } while (i != 0);
    Moji.pPrim14_cur = Moji.Prim_sprt[Cbuf];
    Moji.pPrim8_cur = Moji.Prim_sprt_8[Cbuf];
}

static __inline__ UCHAR *Msg_get_80031BDC(ULONG No)
{
    return D_80098E88 + D_80099654[No];
}

void func_80031BDC(void)
{
    ULONG idx;
    ULONG i;
    LONG t;
    UCHAR *p1;
    UCHAR *p2;

    D_800D4433 = 1;
    switch (D_800CC84D) {
    case 0:
        idx = 0x28;
        break;
    case 1:
        idx = 0x30;
        break;
    case 2:
        idx = 0x38;
        break;
    case 3:
        idx = 0x40;
        break;
    }
    i = 0;
    p1 = Msg_get_80031BDC(idx);
    p2 = Msg_get_80031BDC(idx + 1);
    t = VSync(-1);
    while (1) {
        if ((ULONG)(VSync(-1) - t) > 180) {
            i += 2;
            if (i == 6) {
                break;
            }
            p1 = Msg_get_80031BDC(idx + i);
            p2 = Msg_get_80031BDC(idx + i + 1);
            Mess_print(0x22, 0xB9, p1, 0x100);
            Mess_print(0x22, 0xC9, p2, 0x100);
            Task_sleep(1);
            t = VSync(-1);
        }
        Mess_print(0x22, 0xB9, p1, 0x100);
        Mess_print(0x22, 0xC9, p2, 0x100);
        Task_sleep(1);
    }
    D_800D4433 = 0;
    Task_exit();
}
