#include "ovl/title.h"
#include "cd.h"
#include "game.h"
#include "mojidisp.h"
#include "movie.h"
#include "sld.h"
#include "status.h"

/* main loop */
void Title(void)
{
    Pad_rep_set(0x5000, 0x20A);
    Title_wk.Timer = 300;
    TITLE_RTN_SET(&Title_wk, 0);
    Main.Vloop = 1;
    do {
        Title_main_tbl[Title_wk.Main_rtn](&Title_wk);
        Task_sleep(1);
    } while (1);
}

/* Capcom logo (after a reset only), then the opening movie; START skips.
 * splat did not split it from Title (it is only referenced from Title_main_tbl) */
void Capcom_logo(TITLE_WORK *pTi)
{
    ULONG Step;
    LONG Timer;

    Main.Unk193 = 2;
    Task_sleep(1);
    Step = 0;
    if (G.System_flg & 0x20000000) {
        while (Main.Unk193 != 0) {
            Task_sleep(1);
        }
        while (1) {
            switch (Step) {
            case 0:
                Snd_work.Vol.Rv_mode = 2;
                Capcom_logo_load();
                Bg_set_mode(1, 0);
                Fade_set(0, 0, FADE_HOLD, 1, 0, 0xFFFFFF, 30);
                Step++;
                break;
            case 1:
                if (Fade_status(0) & FADE_END) {
                    Step++;
                }
                break;
            case 2:
                Bg_set_mode(2, 0);
                Fade_set(0, 0, 0, 1, 0xFFFFFF, 0, 30);
                Step++;
            case 3:
                if (Fade_status(0) == 0) {
                    Step++;
                }
                break;
            case 4:
                Title_bg_load();
                Timer = 120;
                Step++;
            case 5:
                if (Timer-- == 0) {
                    Step++;
                }
                break;
            case 6:
                Fade_set(0, 0, FADE_HOLD, 1, 0, 0xFFFFFF, 30);
                Step++;
            case 7:
                if (Fade_status(0) & FADE_END) {
                    Step++;
                }
                break;
            case 8:
                Bg_set_mode(1, 0);
                Fade_set(0, 0, 0, 1, 0xFFFFFF, 0, 30);
                Step++;
            case 9:
                if (Fade_status(0) == 0) {
                    Step++;
                }
                break;
            }
            if (Step == 10) {
                break;
            }
            if (G.Unk20FC & 0x800) {
                Bg_set_mode(1, 0);
                Fade_off(0);
                Task_sleep(2);
                if (Step < 5) {
                    Title_bg_load();
                }
                break;
            }
            Task_sleep(1);
        }
    } else {
        Bg_set_mode(1, 0);
        Title_bg_load();
    }
    if (!(G.System_flg & 0x80)) {
        Init_movie_work(12);
        while (G.System_flg & 0x10000) {
            Task_sleep(1);
        }
    }
    Main.Unk191 = 1;
    Fade_set(1, 1, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
    pTi->Main_rtn = 1;
}

/* title init: sound banks, menu sprites, title picture */
void Title_init(TITLE_WORK *pTi)
{
    Main.Vloop = 1;
    Fade_set(1, 1, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
    if (!(G.System_flg & 0x80)) {
        Snd_vab_load(0, 1, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
    } else {
        Snd_vab_load(0, 11, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
    }
    Cd_read(0x121, (ULONG *)0x801F7E00, 1, "OPTION BGM");
    Snd_seq_data_set(0, 0x38, (ULONG *)0x801F7E00);
    Snd_bank_set(5, Cd_read_s(0x122, 0, (ULONG *)0x801C1814, 5, "OPTIOIN VB"), 0);
    Main.Unk191 = 1;
    Bg_set_mode(1, 0);
    pTi->Timer = 900;
    TITLE_RTN_SET(pTi, 2);
    Title_sprt_init(pTi);
    pTi->Kido_cnt = 0;
    pTi->Kido = Rsin_tbl[0] / 3 - 0x80;
    if (G.System_flg & 0x80) {
        pTi->Cur = 0;
        pTi->Cur_old = 0;
    } else {
        pTi->Cur = 1;
        pTi->Cur_old = 1;
    }
    Title_bg_reload();
    Bg_set_mode(2, 0);
}

/* menu sprites of both draw buffers */
void Title_sprt_init(TITLE_WORK *pTi)
{
    TITLE_BUF *pBuf;
    SPRT *pSp;
    ULONG i;

    pBuf = pTi->Buf;
    i = 0;
    do {
        pSp = &pBuf->Menu.Logo;
        SetSprt(pSp);
        pSp->y0 = 0xD5;
        pSp->v0 = 0xA0;
        pSp->w = 0xD0;
        pSp->h = 0x10;
        pSp->r0 = 0x80;
        pSp->g0 = 0x80;
        pSp->b0 = 0x80;
        pSp->clut = 0x7FC0;
        pSp->x0 = 0x3C;
        pSp->u0 = 0;
        pSp = &pBuf->Menu.Item[0];
        if (G.System_flg & 0x80) {
            SetSprt(pSp);
            pSp->x0 = 0x85;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0x8C;
            pSp->v0 = 0x90;
            pSp->u0 = 0;
            pSp->w = 0x36;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[1];
            SetSprt(pSp);
            pSp->x0 = 0x8F;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0x9E;
            pSp->u0 = 0x40;
            pSp->v0 = 0x90;
            pSp->w = 0x22;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[2];
            SetSprt(pSp);
            pSp->x0 = 0x82;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0xB0;
            pSp->u0 = 0;
            pSp->v0 = 0x80;
            pSp->w = 0x3C;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[3];
            SetSprt(pSp);
            pSp->x0 = 0x95;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0xC2;
            pSp->u0 = 0x68;
            pSp->v0 = 0x90;
            pSp->w = 0x16;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[4];
            SetSprt(pSp);
            pSp->x0 = 0x86;
            pSp->r0 = 0;
            pSp->g0 = 0;
            pSp->b0 = 0;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0x8D;
            pSp->v0 = 0x90;
            pSp->u0 = 0;
            pSp->w = 0x36;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[5];
            SetSprt(pSp);
            pSp->r0 = 0;
            pSp->g0 = 0;
            pSp->b0 = 0;
            pSp->clut = 0x7FC0;
            pSp->x0 = 0x90;
            pSp->y0 = 0x9F;
            pSp->u0 = 0x40;
            pSp->v0 = 0x90;
            pSp->w = 0x22;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[6];
            SetSprt(pSp);
            pSp->x0 = 0x83;
            pSp->r0 = 0;
            pSp->g0 = 0;
            pSp->b0 = 0;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0xB1;
            pSp->u0 = 0;
            pSp->v0 = 0x80;
            pSp->w = 0x3C;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[7];
            SetSprt(pSp);
            pSp->x0 = 0x96;
            pSp->r0 = 0;
            pSp->g0 = 0;
            pSp->b0 = 0;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0xC3;
            pSp->u0 = 0x68;
            pSp->v0 = 0x90;
            pSp->w = 0x16;
            pSp->h = 0xC;
        } else {
            SetSprt(pSp);
            pSp->x0 = 0x44;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0xC1;
            pSp->u0 = 0;
            pSp->v0 = 0x68;
            pSp->w = 0x30;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[1];
            SetSprt(pSp);
            pSp->x0 = 0x84;
            pSp->u0 = 0x40;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0xC1;
            pSp->v0 = 0x68;
            pSp->w = 0x32;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[2];
            SetSprt(pSp);
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->x0 = 0xC8;
            pSp->y0 = 0xC1;
            pSp->u0 = 0;
            pSp->v0 = 0x80;
            pSp->w = 0x3C;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[3];
            SetSprt(pSp);
            pSp->x0 = 0x4C;
            pSp->y0 = 0x9C;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->u0 = 0;
            pSp->v0 = 0;
            pSp->w = 0xA8;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[4];
            SetSprt(pSp);
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->x0 = 0x50;
            pSp->y0 = 0xC1;
            pSp->u0 = 0;
            pSp->v0 = 0x80;
            pSp->w = 0x3C;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[5];
            SetSprt(pSp);
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->x0 = 0xB4;
            pSp->y0 = 0xC1;
            pSp->u0 = 0x40;
            pSp->v0 = 0x80;
            pSp->w = 0x3E;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[6];
            SetSprt(pSp);
            pSp->x0 = 0x50;
            pSp->u0 = 0x38;
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->y0 = 0xC1;
            pSp->v0 = 0xB0;
            pSp->w = 0x38;
            pSp->h = 0xC;
            pSp = &pBuf->Menu.Item[7];
            SetSprt(pSp);
            pSp->r0 = 0x80;
            pSp->g0 = 0x80;
            pSp->b0 = 0x80;
            pSp->clut = 0x7FC0;
            pSp->x0 = 0xB4;
            pSp->y0 = 0xC1;
            pSp->u0 = 0;
            pSp->v0 = 0xB0;
            pSp->w = 0x36;
            pSp->h = 0xC;
        }
        SetDrawTPage(&pBuf->Menu.Tpage, 0, 0, 5);
        pBuf++;
    } while (++i < 2);
}

/* "press start" wait */
void Title_main_wait(TITLE_WORK *pTi)
{
    Title_logo_slide(pTi);
    Title_menu_disp(&Title_wk);
}

/* "press start": the logo slides in, then the menu fades in */
void Title_logo_slide(TITLE_WORK *pTi)
{
    switch (pTi->Sub_rtn) {
    case 0:
        pTi->Moji_y = 0xA0;
        pTi->Mode = 0;
        pTi->Sub_rtn++;
    case 1:
        pTi->Moji_y -= 30;
        if (pTi->Moji_y < 0) {
            Fade_set(0, 0, 0, 1, 0, 0xFFFFFF, 5);
            pTi->Moji_y = 0;
            pTi->Sub_rtn++;
        }
        break;
    case 2:
        if (Fade_status(0) != 0) {
            break;
        }
        Fade_set(1, 1, 0, 2, 0xFFFFFF, 0, 60);
        pTi->Sub_rtn++;
    case 3:
        if (Fade_status(1) == 0) {
            pTi->Mode = 1;
            pTi->Disp_add = 1;
            pTi->Disp_cnt = 0;
            pTi->Sub_rtn++;
            TITLE_RTN_SET(pTi, 3);
        }
        break;
    }
}

/* menu draw: extra menu layout once the game is cleared */
void Title_menu_disp(TITLE_WORK *pTi)
{
    if (G.System_flg & 0x80) {
        Title_menu_disp_ex(pTi);
    } else {
        Title_menu_disp_nml(pTi);
    }
}

/* menu draw: new game / load / option and the game mode page, fading between them (Page) */
void Title_menu_disp_nml(TITLE_WORK *pTi)
{
    TITLE_BUF *pBuf;
    ULONG No0;
    ULONG No1;
    LONG Col;

    pBuf = &pTi->Buf[G.Cbuf_id];
    if (pTi->Mode == 0) {
        return;
    }
    Col = pTi->Kido;
    switch (pTi->Mode) {
    case 2:
        No0 = 6;
        No1 = 7;
        break;
    case 3:
        No0 = 4;
        No1 = 5;
        break;
    }
    switch (pTi->Page) {
    case 0:
        SetSemiTrans(&pBuf->Menu.Item[0], pTi->Cur != 0);
        SetSemiTrans(&pBuf->Menu.Item[1], pTi->Cur != 1);
        SetSemiTrans(&pBuf->Menu.Item[2], pTi->Cur != 2);
        setRGB0(&pBuf->Menu.Item[0], 0x80, 0x80, 0x80);
        setRGB0(&pBuf->Menu.Item[1], 0x80, 0x80, 0x80);
        setRGB0(&pBuf->Menu.Item[2], 0x80, 0x80, 0x80);
        if (pTi->Cur == 0) {
            setRGB0(&pBuf->Menu.Item[0], Col, Col, Col);
        }
        if (pTi->Cur == 1) {
            setRGB0(&pBuf->Menu.Item[1], Col, Col, Col);
        }
        if (pTi->Cur == 2) {
            setRGB0(&pBuf->Menu.Item[2], Col, Col, Col);
        }
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[0]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[1]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[2]);
        break;
    case 1:
        pTi->Fade_cnt = 5;
        pTi->Page++;
    case 2:
        SetSemiTrans(&pBuf->Menu.Item[0], 1);
        SetSemiTrans(&pBuf->Menu.Item[1], 1);
        SetSemiTrans(&pBuf->Menu.Item[2], 1);
        Col = (pTi->Fade_cnt << 7) / 5;
        setRGB0(&pBuf->Menu.Item[0], Col, Col, Col);
        setRGB0(&pBuf->Menu.Item[1], Col, Col, Col);
        setRGB0(&pBuf->Menu.Item[2], Col, Col, Col);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[0]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[1]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[2]);
        if (--pTi->Fade_cnt == 0) {
            pTi->Page++;
        }
        break;
    case 3:
        pTi->Fade_cnt = 0;
        pTi->Page++;
    case 4:
        if (++pTi->Fade_cnt == 5) {
            pTi->Page++;
        }
        SetSemiTrans(&pBuf->Menu.Item[No0], 1);
        SetSemiTrans(&pBuf->Menu.Item[No1], 1);
        Col = (pTi->Fade_cnt << 7) / 5;
        setRGB0(&pBuf->Menu.Item[No0], Col, Col, Col);
        setRGB0(&pBuf->Menu.Item[No1], Col, Col, Col);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[No0]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[No1]);
        break;
    case 5:
        SetSemiTrans(&pBuf->Menu.Item[No0], pTi->Cur != 0);
        SetSemiTrans(&pBuf->Menu.Item[No1], pTi->Cur != 1);
        setRGB0(&pBuf->Menu.Item[No0], 0x80, 0x80, 0x80);
        setRGB0(&pBuf->Menu.Item[No1], 0x80, 0x80, 0x80);
        if (pTi->Cur == 0) {
            setRGB0(&pBuf->Menu.Item[No0], Col, Col, Col);
        }
        if (pTi->Cur == 1) {
            setRGB0(&pBuf->Menu.Item[No1], Col, Col, Col);
        }
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[No0]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[No1]);
        break;
    case 6:
        pTi->Fade_cnt = 5;
        pTi->Page++;
    case 7:
        SetSemiTrans(&pBuf->Menu.Item[No0], 1);
        SetSemiTrans(&pBuf->Menu.Item[No1], 1);
        Col = (pTi->Fade_cnt << 7) / 5;
        setRGB0(&pBuf->Menu.Item[No0], Col, Col, Col);
        setRGB0(&pBuf->Menu.Item[No1], Col, Col, Col);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[No0]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[No1]);
        if (--pTi->Fade_cnt == 0) {
            pTi->Page++;
        }
        break;
    case 8:
        pTi->Fade_cnt = 0;
        pTi->Page++;
    case 9:
        if (++pTi->Fade_cnt == 5) {
            pTi->Page = 0;
        }
        SetSemiTrans(&pBuf->Menu.Item[0], 1);
        SetSemiTrans(&pBuf->Menu.Item[1], 1);
        SetSemiTrans(&pBuf->Menu.Item[2], 1);
        Col = (pTi->Fade_cnt << 7) / 5;
        setRGB0(&pBuf->Menu.Item[0], Col, Col, Col);
        setRGB0(&pBuf->Menu.Item[1], Col, Col, Col);
        setRGB0(&pBuf->Menu.Item[2], Col, Col, Col);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[0]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[1]);
        AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[2]);
        break;
    }
    AddPrim(G.pOt0 + 3, &pBuf->Menu.Logo);
    AddPrim(G.pOt0 + 3, &pBuf->Menu.Tpage);
}

/* menu draw after clearing the game: the item under the cursor pulses */
void Title_menu_disp_ex(TITLE_WORK *pTi)
{
    TITLE_BUF *pBuf;
    SPRT *pSp0;
    SPRT *pSp1;
    SPRT *pSp2;
    SPRT *pSp3;
    UCHAR Kido;

    Kido = pTi->Kido;
    pBuf = &pTi->Buf[G.Cbuf_id];
    pSp0 = &pBuf->Menu.Item[0];
    SetSemiTrans(pSp0, 0);
    pSp1 = &pBuf->Menu.Item[1];
    SetSemiTrans(pSp1, 0);
    pSp2 = &pBuf->Menu.Item[2];
    SetSemiTrans(pSp2, 0);
    pSp3 = &pBuf->Menu.Item[3];
    SetSemiTrans(pSp3, 0);
    setRGB0(&pBuf->Menu.Item[0], 0x40, 0x40, 0x40);
    setRGB0(&pBuf->Menu.Item[1], 0x40, 0x40, 0x40);
    setRGB0(&pBuf->Menu.Item[2], 0x40, 0x40, 0x40);
    setRGB0(&pBuf->Menu.Item[3], 0x40, 0x40, 0x40);
    if (pTi->Cur == 0) {
        setRGB0(&pBuf->Menu.Item[0], Kido, Kido, Kido);
    }
    if (pTi->Cur == 1) {
        setRGB0(&pBuf->Menu.Item[1], Kido, Kido, Kido);
    }
    if (pTi->Cur == 2) {
        setRGB0(&pBuf->Menu.Item[2], Kido, Kido, Kido);
    }
    if (pTi->Cur == 3) {
        setRGB0(&pBuf->Menu.Item[3], Kido, Kido, Kido);
    }
    AddPrim(G.pOt0 + 3, pSp0);
    AddPrim(G.pOt0 + 3, pSp1);
    AddPrim(G.pOt0 + 3, pSp2);
    AddPrim(G.pOt0 + 3, pSp3);
    AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[4]);
    AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[5]);
    AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[6]);
    AddPrim(G.pOt0 + 3, &pBuf->Menu.Item[7]);
    AddPrim(G.pOt0 + 3, &pBuf->Menu.Tpage);
}

/* start the game with the sound bank of the player */
void Title_main_game(TITLE_WORK *pTi)
{
    switch (G.Player.Id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        Snd_vab_load(0, 2, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
        break;
    case 8:
        Snd_vab_load(0, 8, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
        break;
    case 9:
        Snd_vab_load(0, 9, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
        break;
    case 10:
        Snd_vab_load(0, 10, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
        break;
    }
    Task_chain(Game_loop);
}

/* menu: blinking cursor, then the sub routine */
void Title_main(TITLE_WORK *pTi)
{
    pTi->Kido_cnt += 4;
    pTi->Kido = Rsin_tbl[pTi->Kido_cnt] / 3 - 0x80;
    Title_sub_tbl[pTi->Sub_rtn](pTi);
    Title_menu_disp(&Title_wk);
}

/* Title_sub_tbl[0]: RE2 Title_main_select */
void Title_main_select(TITLE_WORK *pTi)
{
    if (G.System_flg & 0x80) {
        Title_sel_menu_ex(pTi);
    } else {
        Title_sel_menu(pTi);
    }
}

/* title menu: new game / load / option; back to the demo when nobody presses anything */
void Title_sel_menu(TITLE_WORK *pTi)
{
    if (--pTi->Timer == 0xFFFF) {
        pTi->Sub_rtn = 10;
        return;
    }
    if (G.Unk20FC & 0x2000) {
        pTi->Cur++;
    }
    if (G.Unk20FC & 0x8000) {
        pTi->Cur--;
    }
    if (pTi->Cur >= 3) {
        pTi->Cur = 0;
    }
    if (pTi->Cur < 0) {
        pTi->Cur = 2;
    }
    if (pTi->Cur != pTi->Cur_old) {
        pTi->Timer = 900;
        pTi->Kido_cnt = 0;
        Snd_se_stad(4, NULL, 0, 0);
    }
    pTi->Cur_old = pTi->Cur;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        pTi->Kido_cnt = 0;
        switch (pTi->Cur) {
        case 0:
            pTi->Sub_rtn = 1;
            pTi->Page = 1;
            pTi->Mode = 2;
            pTi->Cur = 0;
            pTi->Cur_old = 0;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        case 1:
            pTi->Sub_rtn = 2;
            break;
        case 2:
            pTi->Sub_rtn = 5;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        }
    }
}

/* title menu after clearing the game: new game / extra / load / option */
void Title_sel_menu_ex(TITLE_WORK *pTi)
{
    if (G.Unk20FC & 0x4000) {
        pTi->Cur++;
    }
    if (G.Unk20FC & 0x1000) {
        pTi->Cur--;
    }
    if (pTi->Cur >= 4) {
        pTi->Cur = 0;
    }
    if (pTi->Cur < 0) {
        pTi->Cur = 3;
    }
    if (pTi->Cur != pTi->Cur_old) {
        pTi->Timer = 900;
        pTi->Kido_cnt = 0;
        Snd_se_stad(4, NULL, 0, 0);
    }
    pTi->Cur_old = pTi->Cur;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        pTi->Kido_cnt = 0;
        switch (pTi->Cur) {
        case 0:
            pTi->Sub_rtn = 1;
            break;
        case 1:
            pTi->Sub_rtn = 7;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        case 2:
            pTi->Sub_rtn = 5;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        case 3:
            Snd_se_stad(6, NULL, 0, 0);
            pTi->Sub_rtn = 9;
            break;
        }
    }
}

/* new game: flash, then the game task (or the extra game) */
void Title_sel_start(TITLE_WORK *pTi)
{
    Pad_vib_set0(7, 0);
    Pad_vib_set1(9, 0xFF, 0);
    Pad_vib_fade_set(4, 0xFF, 0x3C, 10);
    Fade_set(0, 1, 0, 1, 0, 0xFFFFFF, 4);
    Snd_se_stad(0, NULL, 0, 0);
    while (Fade_status(0) != 0) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Fade_set(0, 1, 0, 1, 0xFFFFFF, 0, 12);
    while (Fade_status(0) != 0) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Fade_set(0, 0, 0, 2, 0, 0xFFFFFF, 180);
    while (Fade_status(0) != 0) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    G.Unk2115 = 0;
    G.System_flg |= 0x40000;
    Task_lexecute(1, 2);
    Task_sleep(1);
    if (G.Unk2115 != 0) {
        Task_lexecute(1, 10);
        Task_sleep(1);
        pTi->Main_rtn = 1;
        Fade_set(1, 7, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
        return;
    }
    if (G.System_flg & 0x40000) {
        G.System_flg &= ~0x40000;
        pTi->Main_rtn = 1;
        Fade_set(1, 7, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
        return;
    }
    if (G.System_flg & 0x80) {
        G.System_flg &= ~0x40000;
        pTi->Main_rtn = 1;
        return;
    }
    if (Flg_ck(G.Unk77F8, 0x1F) != 0) {
        G.System_flg |= 0x100;
    }
    if (G.System_flg & 0x20) {
        G.System_flg |= 0x1000;
        pTi->Main_rtn = 3;
        pTi->Sub_rtn = 1;
        return;
    }
    G.Unk2115 = 1;
    Task_execute(1, Mess_hint_task);
    G.System_flg &= ~0x1000;
    pTi->Main_rtn = 4;
}

/* file names of the save image and extra game sound banks (also used by Title_ex_init) */
const char Title_str_init_sub[] = "INIT_SUB";
const char Title_str_omake_bgm[] = "OMAKE BGM";
const char Title_str_omake_vb[] = "OMAKE VB";
const char Title_str_init_tbl[] = "INIT_TBL";

/* game mode (after "new game" with the extra game unlocked), then start */
void Title_sel_mode(TITLE_WORK *pTi)
{
    ULONG i;

    if (!(G.System_flg & 0x80)) {
        if (G.System_flg & 0x20) {
            Keep_data_save();
            goto init_tbl;
        }
        if (G.Unk20FC & 0x2000) {
            pTi->Cur++;
        }
        if (G.Unk20FC & 0x8000) {
            pTi->Cur--;
        }
        if (pTi->Cur >= 2) {
            pTi->Cur = 0;
        }
        if (pTi->Cur < 0) {
            pTi->Cur = 1;
        }
        if (pTi->Cur != pTi->Cur_old) {
            pTi->Timer = 900;
            pTi->Kido_cnt = 0;
            Snd_se_stad(4, NULL, 0, 0);
        }
        pTi->Cur_old = pTi->Cur;
        if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
            pTi->Kido_cnt = 0;
            switch (pTi->Cur) {
            case 0:
                G.System_flg &= ~0x100;
                break;
            case 1:
                G.System_flg |= 0x100;
                break;
            }
        } else {
            if ((G.Key_trg & 0x2000) && pTi->Page == 5) {
                Snd_se_stad(5, NULL, 0, 0);
                pTi->Sub_rtn = 0;
                pTi->Cur = 0;
                pTi->Cur_old = 0;
                pTi->Page++;
            }
            return;
        }
    }
    if (!(G.System_flg & 0x10000000)) {
        Pad_vib_set0(7, 0);
        Pad_vib_set1(9, 0xFF, 0);
        Pad_vib_fade_set(4, 0xFF, 0x3C, 10);
        Snd_se_stad(0, NULL, 0, 0);
    }
    Fade_set(0, 1, 0, 1, 0, 0xFFFFFF, 4);
    while (Fade_status(0) != 0) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Fade_set(0, 1, 0, 1, 0xFFFFFF, 0, 12);
    while (Fade_status(0) != 0) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Fade_set(0, 0, FADE_HOLD, 2, 0, 0xFFFFFF, 180);
    while (!(Fade_status(0) & FADE_END)) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Main.Unk18E = 0;
    G.System_flg |= 0x1000;
    if (G.System_flg & 0x80) {
        Keep_data_save();
        Cd_read(0x2F, (ULONG *)G.Card_head, 0, (char *)Title_str_init_sub);
        G.Next_cut_no = G.Cut_no;
        Cd_read(0x123, (ULONG *)0x801F7E00, 0, (char *)Title_str_omake_bgm);
        Snd_seq_data_set(0, 0x39, (ULONG *)0x801F7E00);
        Snd_bank_set(5, Cd_read_s(0x124, 2, (ULONG *)0x801C1814, 5, (char *)Title_str_omake_vb), 0);
        Keep_data_load();
        G.Pl_poison_timer = 0;
        G.Player.Status_flg &= ~0x200;
        Task_lexecute(1, 6);
        Task_sleep(1);
    } else {
    init_tbl:
        Cd_read(0x30, (ULONG *)G.Card_head, 0, (char *)Title_str_init_tbl);
        if (G.System_flg & 0x20) {
            Keep_data_load();
            G.Save_cnt = 0;
        } else {
            for (i = 0; i < 3; i++) {
                G.Merc_time[i] = -1;
                G.Merc_grade[i] = 0xFF;
                G.Merc_money[i] = 0;
            }
        }
        G.Next_cut_no = G.Cut_no;
        Task_lexecute(1, 5);
        Task_sleep(1);
        Init_movie_work(0);
        while (G.System_flg & 0x10000) {
            Task_sleep(1);
        }
        Fade_set(0, 0, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
        if (G.System_flg & 0x100) {
            Flg_on(G.Unk77F8, 0x1F);
        }
    }
    Status_init();
    if (G.System_flg & 0x80) {
        G.Unk2115 = 2;
    } else {
        G.Unk2115 = 0;
    }
    Task_execute(1, Mess_hint_task);
    pTi->Main_rtn = 4;
}

/* Title_sub_tbl[3]: empty */
void Title_sub_nop3(TITLE_WORK *pTi)
{
}

/* game mode select */
void Title_game_mode_sel(TITLE_WORK *pTi)
{
    if (G.Unk20FC & 0x2000) {
        pTi->Cur++;
    }
    if (G.Unk20FC & 0x8000) {
        pTi->Cur--;
    }
    if (pTi->Cur >= 2) {
        pTi->Cur = 0;
    }
    if (pTi->Cur < 0) {
        pTi->Cur = 1;
    }
    if (pTi->Cur != pTi->Cur_old) {
        pTi->Timer = 900;
        pTi->Kido_cnt = 0;
        Snd_se_stad(4, NULL, 0, 0);
    }
    pTi->Cur_old = pTi->Cur;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        Snd_se_stad(6, NULL, 0, 0);
        pTi->Kido_cnt = 0;
        switch (pTi->Cur) {
        case 0:
            pTi->Sub_rtn = 5;
            break;
        case 1:
            pTi->Sub_rtn = 6;
            break;
        }
    }
    if ((G.Key_trg & 0x2000) && pTi->Page == 5) {
        Snd_se_stad(5, NULL, 0, 0);
        pTi->Sub_rtn = 0;
        pTi->Cur = 2;
        pTi->Cur_old = 2;
        pTi->Page++;
    }
}

/* load game */
void Title_main_load(TITLE_WORK *pTi)
{
    Fade_set(0, 1, FADE_HOLD, 2, 0, 0xFFFFFF, 12);
    while (!(Fade_status(0) & FADE_END)) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    G.Unk2115 = 1;
    Task_lexecute(1, 4);
    Task_sleep(1);
    Fade_set(0, 0, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Main.Unk191 = 1;
    pTi->Main_rtn = 1;
}

/* option: movie of the opening */
void Title_main_movie(TITLE_WORK *pTi)
{
    Fade_set(0, 1, FADE_HOLD, 2, 0, 0xFFFFFF, 12);
    while (!(Fade_status(0) & FADE_END)) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Init_movie_work(13);
    while (G.System_flg & 0x10000) {
        Task_sleep(1);
    }
    Title_bg_load();
    pTi->Main_rtn = 1;
}

/* option screen */
void Title_main_option(TITLE_WORK *pTi)
{
    ULONG Flg;

    Fade_set(0, 1, FADE_HOLD, 2, 0, 0xFFFFFF, 12);
    while (!(Fade_status(0) & FADE_END)) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Flg = G.Unk77F8[0];
    G.Unk2115 = 0;
    Task_lexecute(1, 8);
    Task_sleep(1);
    if (Flg != G.Unk77F8[0]) {
        Retry_load();
    }
    Main.Vloop = 1;
    Fade_set(0, 0, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Main.Unk191 = 1;
    pTi->Main_rtn = 1;
}

/* Title_sub_tbl[8]: empty */
void Title_sub_nop8(TITLE_WORK *pTi)
{
}

/* Title_sub_tbl[9]: leaves the extra menu (clears System_flg 0x80) and reloads TITLE_DAT */
void Title_ex_off(TITLE_WORK *pTi)
{
    Fade_set(0, 1, FADE_HOLD, 2, 0, 0xFFFFFF, 30);
    while (!(Fade_status(0) & FADE_END)) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    G.System_flg &= ~0x80;
    Title_bg_load();
    pTi->Main_rtn = 1;
}

/* demo play */
void Title_main_demo(TITLE_WORK *pTi)
{
    Fade_set(0, 0, FADE_HOLD, 2, 0, 0xFFFFFF, 60);
    while (!(Fade_status(0) & FADE_END)) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Cd_read(Title_pdemo_cd[Game.Unk4F], (ULONG *)0x80192000, 0, "Pdemo");
    Game.pSave = (SAVE_POS *)0x80192000;
    G.Player.Id = Game.pSave->Id;
    if (++Game.Unk4F >= 3) {
        Game.Unk4F = 0;
    }
    Game.Unk4E = 0;
    Game.Pdemo_cnt = 0;
    G.Unk2115 = 3;
    Task_execute(1, Mess_hint_task);
    G.System_flg = (G.System_flg | 0x10000000) & ~0x1000;
    pTi->Main_rtn = 4;
}

/* extra menu: character select of "The Mercenaries" (4 pictures, 2 x 2) */
void Title_ex_init(TITLE_WORK *pTi)
{
    SPRT *pSp;
    TILE *pTl;
    ENEMY_WORK *pPl;
    ULONG i;
    ULONG j;
    tagGLOBAL *pG;                     /* save data fields go through this (own &G base) */

    pG = &G;
    Fade_set(0, 0, FADE_HOLD, 2, 0, 0xFFFFFF, 15);
    while (!(Fade_status(0) & FADE_END)) {
        Title_menu_disp(pTi);
        Task_sleep(1);
    }
    Memcpy((void *)0x8019C000, (void *)0x80155814, 0x25814);
    G.Ctp.Tpage_clut = 0x1E06;
    Mapping_tim((ULONG *)0x80183068);
    for (i = 0; i < 2; i++) {
        pSp = &pTi->Buf[i].Ex.Pic;
        SetSprt(pSp);
        pSp->clut = 0x7F80;
        pSp->x0 = 0x1C;
        pSp->r0 = 0x80;
        pSp->g0 = 0x80;
        pSp->b0 = 0x80;
        pSp->y0 = 0x26;
        pSp->u0 = 0;
        pSp->v0 = 0;
        pSp->w = 0x84;
        pSp->h = 0x5A;
        for (j = 0; j < 3; j++) {
            pTl = &pTi->Buf[i].Ex.Shade[j];
            SetTile(pTl);
            setRGB0(pTl, 0x20, 0x20, 0x20);
            SetSemiTrans(pTl, 1);
            pTl->w = 0x84;
            pTl->h = 0x5A;
        }
        SetDrawTPage(&pTi->Buf[i].Ex.Tpage, 0, 0, 0x46);
    }
    Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 15);
    pTi->Kido = Rsin_tbl[0] / 2 - 0x80;
    while (Fade_status(0) != 0) {
        Title_ex_disp(pTi);
        Task_sleep(1);
    }
    pTi->Kido_cnt = 0;
    pTi->Cur_old = 0;
    pTi->Cur = 0;
    while (1) {
        pTi->Kido_cnt += 4;
        pTi->Kido = Rsin_tbl[pTi->Kido_cnt] / 2 - 0x80;
        if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
            goto start;
        }
        if (G.Key_trg & 0x2000) {
            break;
        }
        if ((G.Unk20FC & 0x2000) && pTi->Cur != 1) {
            pTi->Cur++;
        }
        if ((G.Unk20FC & 0x8000) && pTi->Cur != 2) {
            pTi->Cur--;
        }
        if (G.Unk20FC & 0x4000) {
            pTi->Cur += 2;
        }
        if (G.Unk20FC & 0x1000) {
            pTi->Cur -= 2;
        }
        if ((UCHAR)pTi->Cur >= 4) {
            pTi->Cur = pTi->Cur_old;
        }
        if (pTi->Cur != pTi->Cur_old) {
            pTi->Kido_cnt = 0;
            Snd_se_stad(4, NULL, 0, 0);
        }
        pTi->Cur_old = pTi->Cur;
        Title_ex_disp(pTi);
        Task_sleep(1);
    }
    /* cancel: back to the title menu */
    Snd_se_stad(5, NULL, 0, 0);
    Fade_set(0, 0, FADE_HOLD, 2, 0, 0xFFFFFF, 15);
    while (!(Fade_status(0) & FADE_END)) {
        Title_ex_disp(pTi);
        Task_sleep(1);
    }
    pTi->Main_rtn = 1;
    pTi->Sub_rtn = 0;
    return;
start:
    Pad_vib_set0(7, 0);
    Pad_vib_set1(9, 0xFF, 0);
    Pad_vib_fade_set(4, 0xFF, 0x3C, 10);
    Snd_se_stad(0, NULL, 0, 0);
    Fade_set(0, 1, 0, 1, 0, 0xFFFFFF, 4);
    while (Fade_status(0) != 0) {
        Title_ex_disp(pTi);
        Task_sleep(1);
    }
    Fade_set(0, 1, 0, 1, 0xFFFFFF, 0, 12);
    while (Fade_status(0) != 0) {
        Title_ex_disp(pTi);
        Task_sleep(1);
    }
    Fade_set(0, 0, FADE_HOLD, 2, 0, 0xFFFFFF, 180);
    while (!(Fade_status(0) & FADE_END)) {
        Title_ex_disp(pTi);
        Task_sleep(1);
    }
    Main.Unk18E = 0;
    G.System_flg |= 0x1010;
    if (pTi->Cur == 3) {
        Cd_read(0x2F, (ULONG *)G.Card_head, 0, (char *)Title_str_init_sub);
        G.Next_cut_no = G.Cut_no;
        Cd_read(0x123, (ULONG *)0x801F7E00, 0, (char *)Title_str_omake_bgm);
        Snd_seq_data_set(0, 0x39, (ULONG *)0x801F7E00);
        Snd_bank_set(5, Cd_read_s(0x124, 2, (ULONG *)0x801C1814, 5, (char *)Title_str_omake_vb), 0);
        G.System_flg |= 0x80;
    } else {
        Cd_read(0x30, (ULONG *)G.Card_head, 0, (char *)Title_str_init_tbl);
        pG->Time = 0x10E;
        G.Status_flg |= 0x4000000;
    }
    D_800C79C0 = 0;
    /* i is still 2 from the sprite loop */
    G.Merc_time[i] = -1;
    G.Merc_grade[i] = 0xFF;
    G.Merc_money[i] = 0;
    G.Pl_poison_timer = 0;
    G.pEnemy[0] = G_PLAYER;
    G.Player.Status_flg &= ~0x200;
    G.Next_cut_no = G.Cut_no;
    pPl = G_PLAYER;
    switch (pTi->Cur) {
    case 0:
        Task_lexecute(1, 5);
        Task_sleep(1);
        Init_movie_work(0);
        while (G.System_flg & 0x10000) {
            Task_sleep(1);
        }
        Fade_set(0, 0, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
        pPl->Life = 200;
        pPl->Max_life = 200;
        pPl->Old_x = 0x33CE;
        pPl->Pos_x = 0x33CE;
        pPl->Old_z = -0x30F9;
        pPl->Pos_z = -0x30F9;
        pPl->Ground = 0;
        pPl->Cdir_y = 0x720;
        pPl->Old_y = 0;
        pPl->Pos_y = 0;
        pPl->Status_flg = 0;
        pG->Unk783E = 0;
        pG->Unk7840 = 13;
        pG->Cut_no = 11;
        pG->Unk7864 = 0;
        pG->Unk784E = 0;

        pPl->Id = 0;
        G.Unk7E20 = 0;
        G.Unk2115 = 0;
        pPl->nFloor = pPl->Ground / -1800;
        break;
    case 1:
        pPl->Life = 200;
        pPl->Max_life = 200;
        pPl->Old_x = -0x36A4;
        pPl->Pos_x = -0x36A4;
        pPl->Old_z = -0x481B;
        pPl->Pos_z = -0x481B;
        pPl->Cdir_y = 0xB58;
        pPl->Old_y = 0;
        pPl->Pos_y = 0;
        pPl->Ground = 0;
        pPl->nFloor = 0;
        pPl->Status_flg = 0;
        pG->Unk783E = 1;
        pG->Unk7840 = 0x11;
        pG->Cut_no = 5;
        pG->Unk7864 = 0;
        pG->Unk784E = 0;

        pPl->Id = 0;
        G.Unk7E20 = 2;
        G.Unk2115 = 1;
        break;
    case 2:
        pPl->Life = 200;
        pPl->Max_life = 200;
        pPl->Old_x = -0x286;
        pPl->Pos_x = -0x286;
        pPl->Old_z = -0x4065;
        pPl->Pos_z = -0x4065;
        pPl->Cdir_y = 0xB4B;
        pPl->Old_y = 0;
        pPl->Pos_y = 0;
        pPl->Ground = 0;
        pPl->nFloor = 0;
        pPl->Status_flg = 0;
        pG->Unk783E = 2;
        pG->Unk7840 = 3;
        pG->Cut_no = 1;
        pG->Unk7864 = 1;
        pG->Unk784E = 1;

        pPl->Id = 1;
        G.Unk7E20 = 2;
        G.Unk2115 = 1;
        break;
    case 3:
        pPl->Life = 200;
        pPl->Max_life = 200;
        pPl->Old_x = -0x47EF;
        pPl->Pos_x = -0x47EF;
        pPl->Old_z = -0x58D0;
        pPl->Pos_z = -0x58D0;
        pPl->Cdir_y = 0x4A8;
        pPl->Old_y = 0;
        pPl->Pos_y = 0;
        pPl->Ground = 0;
        pPl->nFloor = 0;
        pG->Unk783E = 6;
        pG->Unk7840 = 12;
        pG->Cut_no = 2;
        pG->Unk7864 = 8;
        pG->Unk784E = 8;

        pPl->Id = 8;
        G.Unk7E20 = 0;
        G.Unk2115 = 2;
        break;
    }
    Extra_pl = pTi->Cur;
    G.Unk2118 = 0;
    G.Score = 0;
    G.Unk2116 = 0;
    G.Next_cut_no = G.Cut_no;
    Status_init();
    Task_execute(1, Mess_hint_task);
    pTi->Main_rtn = 4;
}

/* extra menu draw: the selected character lit, dark tiles over the others */
void Title_ex_disp(TITLE_WORK *pTi)
{
    TILE *pTl;
    SPRT *pSp;
    ULONG i;

    pTl = pTi->Buf[G.Cbuf_id].Ex.Shade;
    for (i = 0; i < 4; i++) {
        if (i == pTi->Cur) {
            pSp = &pTi->Buf[G.Cbuf_id].Ex.Pic;
            pSp->x0 = (i & 1) * 135 + 0x1C;
            pSp->y0 = (i >> 1) * 93 + 0x26;
            pSp->r0 = pTi->Kido;
            pSp->g0 = pTi->Kido;
            pSp->b0 = pTi->Kido;
        } else {
            pTl->x0 = (i & 1) * 135 + 0x1C;
            pTl->y0 = (i >> 1) * 93 + 0x26;
            pTl++;
        }
    }
    AddPrim(G.pOt0 + 3, &pTi->Buf[G.Cbuf_id].Ex.Shade[0]);
    AddPrim(G.pOt0 + 3, &pTi->Buf[G.Cbuf_id].Ex.Shade[1]);
    AddPrim(G.pOt0 + 3, &pTi->Buf[G.Cbuf_id].Ex.Shade[2]);
    AddPrim(G.pOt0 + 3, &pTi->Buf[G.Cbuf_id].Ex.Pic);
    AddPrim(G.pOt0 + 3, &pTi->Buf[G.Cbuf_id].Ex.Tpage);
}

/* RE2 Title_bg_load */
void Title_bg_load(void)
{
    Cd_read(0x67, (ULONG *)0x80130000, 0, "TITLE_DAT");
}

/* loads the Capcom logo */
void Capcom_logo_load(void)
{
    Cd_read(0x11, (ULONG *)0x8019C000, 0, "Capcom.tim");
}

/* title picture: the cleared-game version is at 0x80155814 */
void Title_bg_reload(void)
{
    SVECTOR Unused;

    if (G.System_flg & 0x80) {
        Memcpy((void *)0x8019C000, (void *)0x80155814, 0x25814);
    } else {
        Memcpy((void *)0x8019C000, (void *)0x80130000, 0x25814);
    }
    G.Ctp.Tpage_clut = 0x1F05;
    Mapping_tim((ULONG *)0x8017B028);
}
