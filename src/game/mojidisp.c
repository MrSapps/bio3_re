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

/* RE2 Print_color: colour and attributes of the next debug prints */
void Print_color(UCHAR A0, UCHAR Col, USHORT Attr)
{
    Moji.Unk75C9 = A0;
    Moji.Mess_attr = Attr | (Col << 3);
}

/* RE2 Print8: printf-style 8-pixel text */
void Print8(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), Moji.Mess_attr, &pFmt);
}

/* RE2 Print14: printf-style 14-pixel text */
void Print14(USHORT X, USHORT Y, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), Moji.Mess_attr | 0x8000, &pFmt);
}

/* Print8 in colour Col */
void Print8_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), (Moji.Mess_attr & 0xFF07) | (Col << 3), &pFmt);
}

/* Print14 in colour Col */
void Print14_col(USHORT X, USHORT Y, UCHAR Col, UCHAR *pFmt, ...)
{
    Print_main(X | (Y << 16), ((SHORT)(Moji.Mess_attr & 0xFF07) | 0x8000) | (Col << 3), &pFmt);
}

/* formats a Print* string and queues its glyphs */
INCLUDE_ASM("asm/nonmatchings/game/mojidisp", Print_main);

/* queues the 8-pixel glyph sprites of a string at Xy */
void Print8_put(MOJIDISP *pMoji, UCHAR *pStr, ULONG Xy, ULONG Attr)
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

/* queues the 14-pixel glyph sprites of a string at Xy */
void Print14_put(MOJIDISP *pMoji, UCHAR *pStr, ULONG Xy, ULONG Attr)
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
            W = Mess_char_width(Mode, *pStr, (LONG *)&Xy);
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
            W = Mess_char_width(Mode, *pStr, (LONG *)&Xy);
        }
        Xy += W;
        pStr++;
    } while (*pStr);
    *pPrev = (*pPrev & 0xFF000000) | (Tag & 0xFFFFFF);
    pMoji->pPrim14_cur = pPrim;
}

/* RE2 Mess_set: opens message Mess_no (Attr 0 system, 0x1000 status, 0x2000 room messages) */
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
    pG->Mess_sel = 0;
    pMoji->Stop_data = Stop_data;
    pMoji->Mess_r_no0 = 0;
    pMoji->Mess_attr2 = Attr | 0x100;
    pG->Status_flg |= 0x100;
    pMoji->Stop_keep = pG->Stop_flg;
    switch (Attr & 0x3000) {
    case 0:
        pMoji->Pos_x_def = 0x22;
        pMoji->Pos_y_def = 0xB9;
        pMoji->Mess_top = Mess_sys_text + Mess_sys_ofs[Mess_no];
        break;
    case 0x1000:
        pMoji->Pos_x_def = 0xE;
        pMoji->Pos_y_def = 0xAD;
        pMoji->Mess_top = Mess_st_text + Mess_st_ofs[Mess_no];
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

/* RE2 Moji_trans_main: per frame: message window and the queued text */
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

/* RE2 Mess_disp: message state machine (control codes 0xEA..0xFE) */
void Mess_disp(MOJIDISP *pMoji)
{
    UCHAR *pMess_end;
    ULONG Vloop_ck;
    USHORT Cut;
    ULONG No;
    USHORT Cut_back;

    pMess_end = pMoji->Mess_end;
    Vloop_ck = (Ctrl_mode == 1);
    switch (pMoji->Mess_r_no0) {
    case 0:
        pMoji->Mess_r_no0 = 1;
        pMoji->Unk75E8 = Moji_recip(2 << Vloop_ck);
        pMoji->Timer = 0x10000;
        pMoji->Mess_type = 0;
        pMoji->Mess_wait = 0;
        pMoji->Unk75CF = G.Cut_no;
        G.Stop_flg |= pMoji->Stop_data;
        pMess_end = pMoji->Mess_top;
        G.Status_flg &= ~0x100;
        /* fall through */
    case 1:
        if (pMoji->Timer & 0xFFFF0000) {
            do {
                while (*pMess_end >= 0xEA) {
                    switch (*pMess_end) {
                    case 0xEA:
                    case 0xEB:
                    case 0xEC:
                    case 0xED:
                    case 0xEE:
                    case 0xEF:
                    case 0xF0:
                        pMess_end++;
                        goto text;
                    case 0xF3:
                        Snd_se_stad(*++pMess_end | 0x200, 0, 0, 0);
                        pMess_end++;
                        break;
                    case 0xF4:
                        pMess_end++;
                        pMoji->Unk75CF = G.Cut_no;
                        Cut = *pMess_end++;
                        Main_bg_chg_flg = 1;
                        G.Cut_no = Cut;
                        Cut_change(Cut);
                        break;
                    case 0xF5:
                        goto text;
                    case 0xF7:
                        pMess_end = pMoji->Mess_svp + 2;
                        break;
                    case 0xF8:
                        pMoji->Mess_svp = pMess_end;
                        pMess_end++;
                        if (*pMess_end) {
                            No = *pMess_end;
                        } else {
                            No = pMoji->Mess_item;
                        }
                        pMess_end = name_ptr_set(No);
                        break;
                    case 0xF9:
                        pMess_end++;
                        pMoji->Mess_wait = *pMess_end++;
                        break;
                    case 0xFA:
                        pMess_end++;
                        pMoji->Unk75E8 = Moji_recip(*pMess_end++ << Vloop_ck);
                        break;
                    case 0xFB:
                        pMoji->Mess_r_no0 = 4;
                        pMoji->Timer = 8 << Vloop_ck;
                        G.Mess_sel = pMess_end[1] & 0xF;
                        goto end_set;
                    case 0xF6:
                    case 0xFC:
                        pMess_end++;
                        break;
                    case 0xFD:
                        pMess_end++;
                        if (*pMess_end) {
                            pMoji->Mess_r_no0 = 3;
                            pMoji->Timer = *pMess_end << Vloop_ck;
                        } else {
                            pMoji->Mess_r_no0 = 2;
                        }
                        pMess_end++;
                        goto end_set;
                    case 0xFE:
                        pMess_end++;
                        if (*pMess_end) {
                            pMoji->Mess_r_no0 = 6;
                            pMoji->Timer = *pMess_end << Vloop_ck;
                        } else {
                            pMoji->Mess_r_no0 = 5;
                        }
                        goto end_set;
                    }
                }
            text:
                if (*pMess_end != 0 && pMoji->Unk75E8 != 0) {
                    pMoji->Timer -= 0x10000;
                }
                pMess_end++;
            } while (pMoji->Timer & 0xFFFF0000);
        }
        if (!(pMoji->Mess_attr2 & 0x200) && (G.Key & 0x3000)) {
            pMoji->Timer += pMoji->Unk75E8 << 2;
        } else {
            pMoji->Timer += pMoji->Unk75E8;
        }
        break;
    case 2:
        if (G.Key_trg & 0x1000) {
            G.Key = 0xFFFF;
            goto next;
        }
        if (--pMoji->Timer & (0x18 << Vloop_ck)) {
            Print_color(0xFF, 0, 3);
            if ((pMoji->Mess_attr2 & 0x3000) == 0x1000) {
                Print14(0x69, pMoji->Pos_y_def + 0x2A, "/");
            } else {
                Print14(0x99, pMoji->Pos_y_def + 0x1C, "/");
            }
        }
        break;
    case 3:
        if (--pMoji->Timer == 0) {
        next:
            pMoji->Mess_r_no0 = 1;
            pMoji->Mess_top = pMess_end;
            pMoji->Timer = 0x10000;
            pMoji->Mess_type = pMoji->Mess_wait;
        }
        break;
    case 4:
        if (pMess_end[1] & 0x80) {
            Live_sel_disp(pMoji, pMess_end + 1);
        } else {
            Branch_disp(pMoji, pMess_end + 1);
        }
        break;
    case 5:
        if (G.Key_trg & 0x3000) {
            G.Key = 0xFFFF;
            pMoji->Mess_flg = (pMoji->Mess_flg & 0x7F) | 0x40;
            G.Stop_flg = pMoji->Stop_keep;
            if (pMoji->Unk75CF != G.Cut_no) {
                Cut_back = pMoji->Unk75CF;
                Main_bg_chg_flg = 1;
                G.Cut_no = Cut_back;
                Cut_change(Cut_back);
            }
            return;
        }
        break;
    case 6:
        if (--pMoji->Timer == 0) {
            pMoji->Mess_flg &= 0x7F;
            G.Stop_flg = pMoji->Stop_keep;
            return;
        }
        break;
    }
end_set:
    pMoji->Mess_end = pMess_end;
}
/* ---- from 800303CC.c ---- */

/* 0x10000 / Val + 1 (0 for 0) */
ULONG Moji_recip(ULONG Val)
{
    if (Val == 0) {
        return 0;
    }
    return 0x10000 / Val + 1;
}

/* RE2 Branch_disp: yes/no choice of a message */
void Branch_disp(MOJIDISP *pMoji, UCHAR *pData)
{
    UCHAR *pStr;
    LONG Num;
    LONG Sel;
    ULONG i;
    USHORT Mode;
    ULONG X;
    ULONG Y;
    SPRT *pP;
    LONG Pos;
    LONG Cur;
    LONG W;
    ULONG n;

    n = (*pData++ >> 4) & 7;
    if (n == 0) {
        Num = 2;
        pStr = Mess_sel_text;
    } else {
        Num = n;
        pStr = pData;
    }
    Sel = G.Mess_sel;
    if (pMoji->Timer != 0) {
        pMoji->Timer--;
    }
    if (G.Key_trg & 0x1000) {
        if (pMoji->Timer == 0) {
            LONG Se;

            G.Key = 0xFFFF;
            pMoji->Mess_flg = (pMoji->Mess_flg & 0x7F) | 0x40;
            G.Stop_flg = pMoji->Stop_keep;
            if (!(pMoji->Mess_attr2 & 0x400)) {
                Se = 6;
                if (Sel != 0) {
                    if (Sel == 1) {
                        Se = 5;
                    }
                }
                Snd_se_stad(Se, 0, 0, 0);
            }
        }
    } else if (*(ULONG *)&G.Padd_trg & 0xA000) {
        USHORT Key = G.Padd_trg;

        if (Key & 0x2000) {
            Sel++;
        }
        if (Key & 0x8000) {
            Sel--;
        }
        if (Sel >= Num) {
            Sel = 0;
        }
        if (Sel < 0) {
            Sel = Num - 1;
        }
        Snd_se_stad(4, 0, 0, 0);
        pMoji->Timer = 0;
        pMoji->Mess_time = 0;
    }
    G.Mess_sel = Sel;
    if ((pMoji->Mess_attr2 & 0x3000) == 0x1000) {
        X = 0xD2;
        Y = 0xCD;
    } else {
        X = 0x11E;
        Y = 0xC9;
    }
    pP = Moji.pPrim14_cur;
    Mode = pMoji->Mess_attr2 & 0x100;
    Pos = 0;
    for (i = 0; i < Num; i++) {
        if (i == Sel) {
            Cur = X + Pos - 0xB;
        }
        Mess_print(X + Pos, Y, pStr, pMoji->Mess_attr2);
        while (1) {
            W = 14;
            switch (*pStr) {
            case 0xEA:
            case 0xEB:
            case 0xEC:
            case 0xED:
            case 0xEE:
            case 0xEF:
            case 0xF0:
                pStr++;
                break;
            case 0xF5:
                break;
            case 0xF6:
                W = 7;
                break;
            case 0xF7:
            case 0xFC:
                pStr++;
                continue;
            case 0xF3:
            case 0xF4:
            case 0xF8:
            case 0xF9:
            case 0xFA:
            case 0xFD:
                pStr += 2;
                continue;
            case 0xFE:
                pStr++;
                goto next;
            default:
                W = Moji_char_width(Mode, *pStr, &Pos);
                break;
            }
            Pos += W;
            pStr++;
        }
    next:
        Pos += 0x13;
    }
    Pos -= 0x13;
    Cur -= Pos;
    do {
        while (pP < Moji.pPrim14_cur) {
            pP->x0 -= Pos;
            pP++;
        }
        pMoji->Mess_time--;
        if (pMoji->Mess_time & ((Main.Vloop == 1) ? 0x30 : 0x18)) {
            Print_color(0xFF, 0, 0);
            Print14(Cur, Y, "&");
        }
    } while (0);
}

/* live selection choice: two lines and the blinking cursor (colour by Live_sel.Step) */
void Live_sel_disp(MOJIDISP *pMoji, UCHAR *pData)
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
        pStr = Mess_sel_text;
    } else {
        pStr = pData;
    }
    Sel = G.Mess_sel;
    if (pMoji->Timer != 0) {
        pMoji->Timer--;
    }
    if (Live_sel.Step != 0) {
        if (G.Key_trg & 0x1000) {
            if (pMoji->Timer == 0) {
                G.Key = 0xFFFF;
                G.Status_flg &= 0xEFFFFFFF;
                G.System_flg |= 0x400;
                pMoji->Mess_flg = (pMoji->Mess_flg & 0x7F) | 0x40;
                G.Stop_flg = pMoji->Stop_keep;
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
        } else if (*(ULONG *)&G.Padd_trg & 0x5000) {
            USHORT Key = G.Padd_trg;

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
        G.Mess_sel = Sel;
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
    Y = Live_sel_mess_y;
    X = ((0xFC - Pos) >> 1) + 0x22;
    for (i = 0; i < 2; i++) {
        Mess_print(X, Y + i * 0x16, pLine[i], pMoji->Mess_attr2);
    }
    Y += Sel * 0x16;
    if (Live_sel.Step == 0) {
        return;
    }
    switch (Live_sel.Step) {
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

/* RE2 mess_trans: interprets the message control codes */
void mess_trans(MOJIDISP *pMoji)
{
    SPRT *pP;
    UCHAR *pStr;
    UCHAR *pKeep;
    DR_TPAGE *pOt;
    DR_TPAGE *pDr_mode;
    ULONG Col;
    ULONG Clut;
    ULONG Mode;
    SPRT *pEnd;
    LONG W;
    LONG Xy;
    ULONG Glyph_v;

    pP = Moji.pPrim14_cur;
    pEnd = Moji.Prim_sprt[G.Cbuf_id + 1];
    if (pP >= pEnd) {
        return;
    }
    Col = 0;
    pStr = pMoji->Mess_top;
    pMoji->Mess_pos_y = pMoji->Pos_y_def;
    Xy = pMoji->Pos_x_def | (pMoji->Mess_pos_y << 16);
    W = (pMoji->Mess_attr2 & 7) * 2;
    pDr_mode = &pMoji->Moji_mode[G.Cbuf_id][W];
    Mode = (USHORT)(pMoji->Mess_attr2 & 0x100);
    if (pStr != pMoji->Mess_end) {
        do {
            W = 14;
            if (*pStr == 0) {
                goto zero;
            }
            switch (*pStr) {
            case 0xEA:
                pStr++;
                Clut = GetClut(0x100, Col * 2 | 0x1E0);
                Glyph_v = (UCHAR)(*pStr / 18) * 14 - 0x2E;
                goto mode;
            case 0xEB:
                pStr++;
                pOt = pDr_mode + 1;
                Clut = GetClut(0x100, Col * 2 | 0x1E0);
                pP->v0 = (UCHAR)(*pStr / 18) * 14;
                goto glyph;
            case 0xEC:
                pStr++;
                pOt = pDr_mode + 1;
                Clut = GetClut(0x100, Col * 2 | 0x1E0);
                pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                goto glyph;
            case 0xED:
                pStr++;
                Clut = GetClut(0x100, Col * 2 + 0x1EA);
                Glyph_v = (UCHAR)(*pStr / 18) * 14;
                goto mode;
            case 0xEE:
                pStr++;
                Clut = GetClut(0x100, Col * 2 + 0x1EA);
                Glyph_v = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                goto mode;
            case 0xEF:
                pStr++;
                pOt = pDr_mode + 1;
                Clut = GetClut(0x100, Col * 2 + 0x1EA);
                pP->v0 = (UCHAR)(*pStr / 18) * 14;
                goto glyph;
            case 0xF0:
                pStr++;
                pOt = pDr_mode + 1;
                Clut = GetClut(0x100, Col * 2 + 0x1EA);
                pP->v0 = (UCHAR)(*pStr / 18) * 14 - 0x3C;
                goto glyph;
            case 0xF5:
                W = Moji_char_width(Mode, 0, &Xy);
                goto adv;
            case 0xF6:
                W = 7;
                goto adv;
            case 0xF7:
                pStr = pKeep + 2;
                continue;
            case 0xF8:
                pKeep = pStr;
                pStr++;
                pStr = name_ptr_set(*pStr == 0 ? pMoji->Mess_item : *pStr);
                continue;
            case 0xF9:
                pStr++;
                Col = *pStr++ & 0xF;
                continue;
            case 0xF3:
            case 0xF4:
            case 0xFA:
                pStr += 2;
                continue;
            case 0xFC:
                pStr++;
                pMoji->Mess_pos_y += 0x10;
                Xy = pMoji->Pos_x_def | (pMoji->Mess_pos_y << 16);
                continue;
            case 0xFD:
                pMoji->Mess_pos_y = pMoji->Pos_y_def;
                pStr += 2;
                continue;
            case 0xFE:
                pMoji->Mess_pos_y = pMoji->Pos_y_def;
                pStr++;
                continue;
            default:
                W = Moji_char_width(Mode, *pStr, &Xy);
                Clut = GetClut(0x100, Col * 2 | 0x1E0);
                Glyph_v = (UCHAR)(*pStr / 18) * 14 + 0x1C;
            mode:
                pP->v0 = Glyph_v;
                pOt = pDr_mode;
            glyph:
            *(ULONG *)&pP->x0 = Xy;
            *(ULONG *)&pP->r0 = 0x64808080;
            pP->u0 = (UCHAR)(*pStr % 18) * 14;
            pP->clut = Clut;
            *(ULONG *)&pP->w = 0xE000E;
            AddPrim(pOt, pP);
                pP++;
                if (pP >= pEnd) {
                    goto exit;
                }
                goto adv;
            zero:
                W = Moji_char_width(Mode, *pStr, &Xy);
            adv:
                Xy += W;
                pStr++;
                break;
            }
        } while (pStr != pMoji->Mess_end);
    }
exit:
    Moji.pPrim14_cur = pP;
    pMoji->Mess_pos_x = Xy;
}

/* RE2 Name_disp: draws an item name */
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

/* draws a message string directly */
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

/* RE2 name_ptr_set: name text of item No (the examined name once its Chk_flg is set) */
UCHAR *name_ptr_set(UCHAR No)
{
    ULONG Flg;
    UCHAR *pTop;
    USHORT *pOfs;

    Flg = Item_data_tbl[No].Chk_flg;
    pTop = Item_name_text;
    pOfs = Item_name_ofs;
    if (Flg != 0 && Flg_ck(G.Item_chk_flg, Flg)) {
        No = Flg - 0x55;
    }
    return pTop + pOfs[No];
}

/* width of a proportional glyph (control codes 0x24..), moves *pPos by its left space */
LONG Mess_char_width(ULONG Mode, ULONG Code, LONG *pPos)
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

/* width of glyph Code, moves *pPos by its left space */
LONG Moji_char_width(ULONG Mode, ULONG Code, LONG *pPos)
{
    if (Mode != 0 && Code < 0x57) {
        Code *= 2;
        *pPos -= msg_space[Code];
        return msg_space[Code + 1] + msg_space[Code];
    }
    return 14;
}

/* RE2 Moji_init: message work and glyph sprites */
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

/* per frame: links the text tpage nodes into the 8 OT levels and resets the glyph sprite cursors */
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

static __inline__ UCHAR *Mess_sys_get(ULONG No)
{
    return Mess_sys_text + Mess_sys_ofs[No];
}

/* task: shows three two-line hint pages (by G.Ovl_mode) for 3 seconds each */
void Mess_hint_task(void)
{
    ULONG idx;
    ULONG i;
    LONG t;
    UCHAR *p1;
    UCHAR *p2;

    Main_task_flg = 1;
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
    p1 = Mess_sys_get(idx);
    p2 = Mess_sys_get(idx + 1);
    t = VSync(-1);
    while (1) {
        if ((ULONG)(VSync(-1) - t) > 180) {
            i += 2;
            if (i == 6) {
                break;
            }
            p1 = Mess_sys_get(idx + i);
            p2 = Mess_sys_get(idx + i + 1);
            Mess_print(0x22, 0xB9, p1, 0x100);
            Mess_print(0x22, 0xC9, p2, 0x100);
            Task_sleep(1);
            t = VSync(-1);
        }
        Mess_print(0x22, 0xB9, p1, 0x100);
        Mess_print(0x22, 0xC9, p2, 0x100);
        Task_sleep(1);
    }
    Main_task_flg = 0;
    Task_exit();
}
