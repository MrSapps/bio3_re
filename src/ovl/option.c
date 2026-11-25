#include "ovl/option.h"
#include "sld.h"
#include "sub.h"

/* text drawn with Config_moji_disp is fullwidth Shift-JIS; identical literals are shared by cc1 */
#define S_TYPE            "\x82\x73\x82\x78\x82\x6F\x82\x64" /* ＴＹＰＥ */
#define S_A               "\x82\x60" /* Ａ */
#define S_B               "\x82\x61" /* Ｂ */
#define S_C               "\x82\x62" /* Ｃ */
#define S_VIBRATION       "\x82\x75\x82\x68\x82\x61\x82\x71\x82\x60\x82\x73\x82\x68\x82\x6E\x82\x6D" /* ＶＩＢＲＡＴＩＯＮ */
#define S_OFF             "\x82\x6E\x82\x65\x82\x65" /* ＯＦＦ */
#define S_ON              "\x82\x6E\x82\x6D" /* ＯＮ */
#define S_SOUND           "\x82\x72\x82\x6E\x82\x74\x82\x6D\x82\x63" /* ＳＯＵＮＤ */
#define S_MONAURAL        "\x82\x6C\x82\x6E\x82\x6D\x82\x60\x82\x74\x82\x71\x82\x60\x82\x6B" /* ＭＯＮＡＵＲＡＬ */
#define S_STEREO          "\x82\x72\x82\x73\x82\x64\x82\x71\x82\x64\x82\x6E" /* ＳＴＥＲＥＯ */
#define S_MONITOR_ADJUST  "\x82\x6C\x82\x6E\x82\x6D\x82\x68\x82\x73\x82\x6E\x82\x71\x81\x40\x82\x60\x82\x63\x82\x69\x82\x74\x82\x72\x82\x73" /* ＭＯＮＩＴＯＲ　ＡＤＪＵＳＴ */
#define S_RESET           "\x82\x71\x82\x64\x82\x72\x82\x64\x82\x73" /* ＲＥＳＥＴ */
#define S_EXIT            "\x82\x64\x82\x77\x82\x68\x82\x73" /* ＥＸＩＴ */
#define S_SLASH           "\x81\x5E" /* ／ */

/* cursor glow and per-frame primitive buffers (the task does this twice) */
#define OPT_FRAME_SET()                                                   \
    {                                                                     \
        Opt.Timer += 4;                                                   \
        Opt.Kido = Rsin_tbl[Opt.Timer] / 3 - 0x80;                      \
        Opt.pMoji = (SPRT *)Opt.Moji_prim[G.Cbuf_id];                     \
        Opt.pIcon = Opt.Icon[G.Cbuf_id];                                  \
    }

/* option screen task: fades between pages, runs the page routine of Opt.Routine every frame */
void Config(void)
{
    LONG Fade;

    Task_suspend(0);
    if (G.Ovl_mode == 1) {
        Snd_se_stad(0x500, 0, 0, 0);
    }
    Config_init();
    OPT_FRAME_SET();
    goto init;
    for (;;) {
        Fade = Opt.Fade_mode;
        OPT_FRAME_SET();
        switch (Fade) {
        case 1:
            G.Stop_flg |= 0xFF000000;
            Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0xF);
            Opt.Fade_mode = 2;
            if (G.Ovl_mode == 1 && Opt.Routine_next == 6) {
                Snd_work.Seq[0].Fade_vol = 0x3B5F;
                Snd_work.Seq[0].Fade_mode = 2;
                Snd_work.Seq[0].Fade_step = 0x4A0;
            }
        case 2:
            if (!(Fade_status(0) & 4)) {
                break;
            }
            Task_sleep(2);
            switch (Opt.Routine_next) {
            case 1:
            case 4:
                Opt.Shade_on = 0;
                break;
            case 0:
            case 2:
            case 3:
            case 5:
                Opt.Shade_on = 1;
                break;
            }
            Opt.Routine = Opt.Routine_next;
            if (Opt.Routine == 6) {
                goto exit;
            }
            if (Opt.Reset == 1) {
                G.System_flg |= 0x200000;
                if (G.Ovl_mode == 0) {
                    Snd_se_stad(0xE, 0, 0, 0);
                }
                goto exit;
            }
        init:
            Config_padd[Opt.Routine]();
            Fade_set(0, 1, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
            Task_sleep(2);
            Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 0xF);
            Opt.Fade_mode = 3;
        case 3:
            if (Fade_status(0) == 0) {
                G.Stop_flg = 0;
                Opt.Fade_mode = 0;
            }
            break;
        }
        if (Opt.Fade_mode == 0) {
            Config_padd[Opt.Routine]();
        }
        Config_padd_disp[Opt.Routine]();
        AddPrim(G.pOt0 + 3, &Opt.Moji_tpage[G.Cbuf_id]);
        if (Opt.Shade_on) {
            AddPrim(G.pOt0 + 5, &Opt.Line[G.Cbuf_id][0]);
            AddPrim(G.pOt0 + 5, &Opt.Line[G.Cbuf_id][1]);
            AddPrim(G.pOt0 + 5, &Opt.Shade[G.Cbuf_id]);
            AddPrim(G.pOt0 + 5, &Opt.Shade_tpage[G.Cbuf_id]);
        }
        Task_sleep(1);
    }
exit:
    G.Stop_flg = 0;
    Config_exit();
    Task_signal(0);
    Task_exit();
}

/* top menu: cursor over the six rows */
void Config_padd_opt(void)
{
    OPT_WORK *pW;

    pW = &Opt;
    if ((G.Padd_trg & 0x1000) && pW->Cur == 0) {
        pW->Cur = 5;
    } else if ((G.Padd_trg & 0x4000) && pW->Cur == 5) {
        pW->Cur = 0;
    } else {
        if (G.Padd_rep & 0x1000) {
            pW->Cur--;
        }
        if (G.Padd_rep & 0x4000) {
            pW->Cur++;
        }
        pW->Cur = pW->Cur < 0 ? 0 : pW->Cur > 5 ? 5 : pW->Cur;
    }
    if (pW->Cur_old != pW->Cur) {
        pW->Timer = 0;
        Snd_se_stad(4, 0, 0, 0);
    }
    pW->Cur_old = pW->Cur;
    if (Opt.Fade_mode == 0) {
        if (G.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            pW->Fade_mode = 1;
            pW->Routine_next = pW->Cur + 1;
        }
        if ((G.Key_trg & 0x2000) || (G.Padd_trg & 0x100)) {
            Snd_se_stad(5, 0, 0, 0);
            pW->Routine_next = 6;
            pW->Fade_mode = 1;
        }
    }
}

/* top menu: draw */
void Config_opt_disp(void)
{
    LONG Cur;
    LONG Dark;
    LONG Y;

    Cur = Opt.Cur;
    Config_moji_disp(0x5E, 0x28, "\x82\x66\x82\x60\x82\x6C\x82\x64\x81\x40\x82\x62\x82\x6E\x82\x6D\x82\x65\x82\x68\x82\x66", 0, 0); /* ＧＡＭＥ　ＣＯＮＦＩＧ */
    Dark = Cur != 0;
    Config_moji_disp(0x2A, 0x46, "\x82\x6A\x82\x64\x82\x78\x81\x40\x82\x62\x82\x6E\x82\x6D\x82\x65\x82\x68\x82\x66", Dark, 1); /* ＫＥＹ　ＣＯＮＦＩＧ */
    Config_moji_disp(0xCC, 0x46, S_TYPE, Dark, 0);
    Y = 0x46;
    switch (Main.Unk195) {
    case 0:
        Config_moji_disp(0x103, 0x46, S_A, Dark, 0);
        break;
    case 1:
        Config_moji_disp(0x103, 0x46, S_B, Dark, 0);
        break;
    case 2:
        Config_moji_disp(0x103, 0x46, S_C, Dark, 0);
        break;
    }
    Y += 0x14;
    Config_moji_disp(0x2A, Y, S_VIBRATION, Cur != 1, 1);
    if (Main.Unk196 & 0x80) {
        Config_moji_disp(0xED, Y, S_OFF, Cur != 1, 0);
    } else {
        Config_moji_disp(0xF7, Y, S_ON, Cur != 1, 0);
    }
    Y += 0x14;
    Config_moji_disp(0x2A, Y, S_SOUND, Cur != 2, 1);
    if (Stereo_flg) {
        Config_moji_disp(0xA8, Y, S_MONAURAL, Cur != 2, 0);
    } else {
        Config_moji_disp(0xCD, Y, S_STEREO, Cur != 2, 0);
    }
    Y += 0x14;
    Config_moji_disp(0x2A, Y, S_MONITOR_ADJUST, Cur != 3, 1);
    Y += 0x14;
    Config_moji_disp(0x2A, Y, S_RESET, Cur != 4, 1);
    Config_moji_disp(0x2A, Y + 0x1E, S_EXIT, Cur != 5, 1);
    Config_bg_disp();
}

/* screen init: load data, prims, background pieces; saves the main bg settings */
void Config_init(void)
{
    Config_bg_load();
    Config_prim_init();
    Opt.Routine = 0;
    Opt.Step = 0;
    Opt.Cur = 0;
    Opt.Cur_old = 0;
    Opt.Kido = 0x80;
    Opt.Timer = 0;
    Opt.Shade_on = 1;
    Opt.Reset = 0;
    Opt.Bg_mode_bak = Main.Bg_mode;
    Opt.Unk191_bak = Main.Unk191;
    Opt.Vloop_bak = Main.Vloop;
    Config_piece_set(0, 2, 0x63, 0x8A, 0);
    Config_piece_set(1, 0, 0x60, 0xC0, 0);
    Config_piece_set(2, 0, 0xBA, 0xC6, 0);
    Config_piece_set(3, 1, 0xEC, 0x92, 0);
    Config_piece_set(4, 0, 0x112, 0x4B, 0);
    Config_piece_set(5, 0, 0x94, 0x61, 0);
    Config_piece_set(6, 2, 0xBB, 0x85, 0);
    Config_piece_set(7, 1, 0x3D, 0x9F, 0);
    Config_piece_set(8, 0, 0x37, 0x5F, 0);
    Main.Unk191 = 1;
    Bg_set_mode(2, 0);
    Main.Vloop = 1;
    Pad_rep_set(0xF000, 0x314);
    Snd_work.Fade_mask = 0x80;
    Snd_work.Fade_vol = 0x3FFF;
    Snd_work.Fade_step = 0xB00;
}

/* screen init: set up the primitives of both buffers (background, text tpage, icons, pad picture, pieces, shade panel and its lines) */
void Config_prim_init(void)
{
    SPRT *pBg;
    SPRT *pIcon;
    DR_TPAGE *pTpage;
    SPRT *p;
    POLY_FT4 *pPoly;
    TILE *pTile;
    ULONG i;
    ULONG Clut0;
    ULONG Clut1;
    ULONG Tpage0;
    ULONG Tpage1;
    ULONG j;

    Clut0 = GetClut(0x100, 0x1FE);
    Clut1 = GetClut(0x100, 0x1FF);
    Tpage0 = GetTPage(0, 0, 0x380, 0x100);
    Tpage1 = GetTPage(0, 0, 0x3C0, 0x100);
    pBg = Opt.Bg[0];
    pTpage = Opt.Bg_tpage[0];
    pIcon = Opt.Icon[0];
    for (i = 0; i < 2; i++) {
        SetSprt(pBg);
        pBg->r0 = 0x80;
        pBg->g0 = 0x80;
        pBg->b0 = 0x80;
        pBg->x0 = 0;
        pBg->y0 = 0;
        pBg->u0 = 0;
        pBg->v0 = 0;
        pBg->clut = Clut0;
        pBg->w = 0x100;
        pBg->h = 0xF0;
        pBg++;
        SetSprt(pBg);
        pBg->r0 = 0x80;
        pBg->g0 = 0x80;
        pBg->b0 = 0x80;
        pBg->x0 = 0x100;
        pBg->y0 = 0;
        pBg->u0 = 0;
        pBg->v0 = 0x80;
        pBg->clut = Clut0;
        pBg->w = 0x40;
        pBg->h = 0x80;
        pBg++;
        SetSprt(pBg);
        pBg->r0 = 0x80;
        pBg->g0 = 0x80;
        pBg->b0 = 0x80;
        pBg->x0 = 0x100;
        pBg->y0 = 0x80;
        pBg->u0 = 0x40;
        pBg->v0 = 0x80;
        pBg->clut = Clut0;
        pBg->w = 0x40;
        pBg->h = 0x70;
        pBg++;
        SetDrawTPage(pTpage, 0, 0, Tpage1);
        pTpage++;
        SetDrawTPage(pTpage, 0, 0, Tpage0);
        pTpage++;
        SetDrawTPage(&Opt.Moji_tpage[i], 0, 0, Tpage0);
        for (j = 0; j < 4; j++) {
            SetSprt(pIcon);
            pIcon->r0 = 0x80;
            pIcon->g0 = 0x80;
            pIcon->b0 = 0x80;
            pIcon->clut = Clut1;
            pIcon->w = 0xE;
            pIcon->h = 0xE;
            pIcon++;
        }
        p = Opt.Button[i];
        SetSprt(p);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->clut = Clut1;
        p->w = 0x48;
        p->h = 0x10;
        p->x0 = Config_pad_icon_pos[0];
        p->y0 = Config_pad_icon_pos[1];
        for (j = 0; j < 20; j++) {
            pPoly = &Opt.Piece[j].Poly[i];
            SetPolyFT4(pPoly);
            SetSemiTrans(pPoly, 1);
            pPoly->r0 = 0x80;
            pPoly->g0 = 0x80;
            pPoly->b0 = 0x80;
            pPoly->clut = Clut0;
            pPoly->tpage = Tpage0;
        }
        pTile = &Opt.Shade[i];
        SetTile(pTile);
        setRGB0(pTile, 0x10, 0x10, 0x10);
        Opt.Shade[i].x0 = 0;
        Opt.Shade[i].y0 = 0x25;
        Opt.Shade[i].w = 0x140;
        Opt.Shade[i].h = 0xA0;
        SetSemiTrans(pTile, 1);
        SetDrawTPage(&Opt.Shade_tpage[i], 0, 0, GetTPage(0, 2, 0, 0));
        for (j = 0; j < 2; j++) {
            SetLineF2(&Opt.Line[i][j]);
            SetSemiTrans(&Opt.Line[i][j], 1);
            setRGB0(&Opt.Line[i][j], 0x18, 0x18, 0x18);
            Opt.Line[i][j].x0 = 0;
            Opt.Line[i][j].y0 = j * 0xA1 + 0x24;
            Opt.Line[i][j].x1 = 0x140;
            Opt.Line[i][j].y1 = j * 0xA1 + 0x24;
        }
    }
}

/* screen exit: reload the game texture page and restore the main bg settings */
void Config_exit(void)
{
    tagGLOBAL *pG;

    pG = &G;
    if (pG->System_flg & 0x100000) {
        Main.Unk18E = 2;
    }
    Cd_read(0x15, (ULONG *)0x8019C000, 0, "CORE00_TIM");
    pG->Ctp.Tpage_clut = 0x1E;
    Mapping_tim((ULONG *)0x8019C000);
    Main.Bg_mode = Opt.Bg_mode_bak;
    Main.Unk191 = Opt.Unk191_bak;
    Main.Vloop = Opt.Vloop_bak;
    Fade_set(0, 1, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Snd_work.Fade_mask = 0x80;
    Snd_work.Fade_vol = 0;
    Snd_work.Fade_step = -0xB00;
}

/* draw the background and the pieces */
void Config_bg_disp(void)
{
    OPT_WORK *pW;
    ULONG *pOt;
    SPRT *pBg;
    DR_TPAGE *pTpage;

    pW = &Opt;
    pOt = G.pOt0 + 7;
    pBg = pW->Bg[G.Cbuf_id];
    pTpage = pW->Bg_tpage[G.Cbuf_id];
    AddPrim(pOt, &pBg[0]);
    AddPrim(pOt, &pTpage[0]);
    AddPrim(pOt, &pBg[1]);
    AddPrim(pOt, &pBg[2]);
    AddPrim(pOt, &pTpage[1]);
    Config_piece_move();
    Config_piece_disp();
}

/* load Option.dat (background) and its cluts */
void Config_bg_load(void)
{
    RECT Rect;

    CD.Fno = 0x40;
    CD.Fsize = Config_file_tbl[0].Size;
    CD.Fsector = (floc[0x40].sec_high << 16) + floc[0x40].sector;
    CD.Fsum = Config_file_sum[0];
    Cd_read(0x40, (ULONG *)0x8019C000, 2, "Option.dat");
    Rect.x = 0x100;
    Rect.y = 0x1FE;
    Rect.w = 0x10;
    Rect.h = 2;
    LoadImage(&Rect, (ULONG *)0x8019C014);
    Rect.x = 0x380;
    Rect.y = 0x100;
    Rect.w = 0x40;
    Rect.h = 0x100;
    LoadImage(&Rect, (ULONG *)0x8019C060);
    Rect.x += 0x40;
    LoadImage(&Rect, (ULONG *)0x801A40C0);
}

/* load the pad picture for the pad type (dual shock or normal) */
void Config_pad_tim_load(void)
{
    if (G.Pad_type == 7) {
        CD.Fno = 0x40;
        CD.Fsize = Config_file_tbl[1].Size;
        CD.Fsector = (floc[0x40].sec_high << 16) + floc[0x40].sector + Config_file_tbl[1].Ofs;
        CD.Fsum = Config_file_sum[1];
        Cd_read(0x40, (ULONG *)0x8019C000, 2, "PaddD.tim");
    } else {
        CD.Fno = 0x40;
        CD.Fsize = Config_file_tbl[2].Size;
        CD.Fsector = (floc[0x40].sec_high << 16) + floc[0x40].sector + Config_file_tbl[2].Ofs;
        CD.Fsum = Config_file_sum[2];
        Cd_read(0x40, (ULONG *)0x8019C000, 2, "PaddN.tim");
    }
}

/* key config page */
void Config_padd_con(void)
{
    OPT_WORK *pW;

    pW = &Opt;
    if (pW->Step == 0) {
        Config_pad_tim_load();
        pW->Timer = 0;
        pW->Sub_cur = 0;
        pW->Pad_type = G.Pad_type;
        pW->Pad_cfg = Main.Unk195;
        pW->Flg = Main.Unk196;
        pW->Step++;
    }
    if (pW->Pad_type != G.Pad_type) {
        Config_pad_tim_load();
    }
    pW->Pad_type = G.Pad_type;
    if ((G.Padd_trg & 0x1000) && pW->Sub_cur == 1) {
        Snd_se_stad(4, 0, 0, 0);
        pW->Timer = 0;
        pW->Sub_cur = 0;
    }
    if ((G.Padd_trg & 0x4000) && Opt.Sub_cur == 0) {
        Snd_se_stad(4, 0, 0, 0);
        Opt.Timer = 0;
        Opt.Sub_cur = 1;
    }
    switch (Opt.Sub_cur) {
    case 0:
        if (G.Padd_rep & 0xA000) {
            Snd_se_stad(4, 0, 0, 0);
            if (G.Padd_rep & 0x2000) {
                if (++Opt.Pad_cfg == 3) {
                    Opt.Pad_cfg = 0;
                }
                Config_pad_arrow_r = 1;
            }
            if (G.Padd_rep & 0x8000) {
                if (--Opt.Pad_cfg == -1) {
                    Opt.Pad_cfg = 2;
                }
                Config_pad_arrow_l = 1;
            }
            Opt.Arrow = 2;
        }
        if (Opt.Pad_cfg == 2) {
            Opt.Flg |= 0x40;
        } else {
            Opt.Flg &= ~0x40;
        }
        if (!((USHORT)G.Padd & 0xA000)) {
            Opt.Arrow = 0;
            Config_pad_arrow_l = 0;
            Config_pad_arrow_r = 0;
        }
        break;
    case 1:
        if (Opt.Fade_mode != 0) {
            return;
        }
        if (G.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            G.Pad_type = Opt.Pad_type;
            Main.Unk195 = Opt.Pad_cfg;
            Main.Unk196 = Opt.Flg;
            Config_page_exit();
        }
        break;
    }
    if (Opt.Fade_mode == 0 && (G.Key_trg & 0x2000)) {
        Snd_se_stad(5, 0, 0, 0);
        Config_page_exit();
    }
}

/* key config page: draw */
void Config_con_disp(void)
{
    OPT_WORK *pW;
    LONG On;

    pW = &Opt;
    Config_moji_disp(0xC9, 0x16, S_TYPE, pW->Sub_cur != 0, 1);
    switch (pW->Pad_cfg) {
    case 0:
        Config_moji_disp(0x100, 0x16, S_A, pW->Sub_cur != 0, 1);
        break;
    case 1:
        Config_moji_disp(0x100, 0x16, S_B, pW->Sub_cur != 0, 1);
        break;
    case 2:
        Config_moji_disp(0x100, 0x16, S_C, pW->Sub_cur != 0, 1);
        break;
    }
    if (Opt.Arrow != 0) {
        Opt.Arrow--;
        On = 1;
    } else {
        On = 0;
    }
    Config_arrow_disp(0xB9, 0x16, 0, Config_pad_arrow_l && On);
    Config_arrow_disp(0x110, 0x16, 1, Config_pad_arrow_r && On);
    Config_moji_disp(0xD8, 0x25, S_EXIT, Opt.Sub_cur != 1, 1);
    Config_pad_disp();
}

/* key config page: pad button picture and the control mode text */
void Config_pad_disp(void)
{
    SPRT *p;
    UCHAR *pIcon;
    ULONG i;

    i = 0;
    p = Opt.Button[G.Cbuf_id];
    pIcon = &Config_pad_icon[Opt.Pad_cfg];
    for (; i < 1; i++) {
        p->u0 = (UCHAR)(pIcon[i] / 5) * 72;
        p->v0 = (UCHAR)(pIcon[i] % 5) * 16 + 0x30;
        AddPrim(G.pOt0 + 3, p);
        p++;
    }
    if (Opt.Flg & 0x40) {
        Config_moji_disp(0x7A, 0x5A, "\x82\x6C\x82\x60\x82\x6D\x82\x74\x82\x60\x82\x6B", 0, 0);
    } else {
        Config_moji_disp(0x87, 0x5A, "\x82\x60\x82\x74\x82\x73\x82\x6E", 0, 0);
    }
}

/* vibration page */
void Config_padd_vib(void)
{
    OPT_WORK *pW;

    pW = &Opt;
    if (pW->Step == 0) {
        pW->Timer = 0;
        pW->Step++;
        pW->Flg = Main.Unk196;
    }
    if (!(pW->Flg & 0x80) && (G.Padd_trg & 0x2000)) {
        Snd_se_stad(4, 0, 0, 0);
        pW->Timer = 0;
        pW->Flg |= 0x80;
    }
    if ((pW->Flg & 0x80) && (G.Padd_trg & 0x8000)) {
        Snd_se_stad(4, 0, 0, 0);
        Opt.Timer = 0;
        Opt.Flg &= ~0x80;
    }
    if (Opt.Fade_mode == 0) {
        if (G.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            Main.Unk196 = Opt.Flg;
            if (!(Opt.Flg & 0x80)) {
                Pad_vib_set0(0xF, 0);
                Pad_vib_set1(0xF, 0xFF, 0);
            }
            Config_page_exit();
        }
        if (G.Key_trg & 0x2000) {
            Snd_se_stad(5, 0, 0, 0);
            Config_page_exit();
        }
    }
}

/* vibration page: draw */
void Config_vib_disp(void)
{
    Config_moji_disp(0x6D, 0x36, S_VIBRATION, 0, 0);
    Config_moji_disp(0x70, 0x6A, S_ON, Opt.Flg >> 7, 1);
    Config_moji_disp(0x98, 0x6A, S_SLASH, 0, 0);
    Config_moji_disp(0xB4, 0x6A, S_OFF, (Opt.Flg >> 7) ^ 1, 1);
    Config_bg_disp();
}

/* sound page: stereo/mono and the BGM / SE volumes (0..100, Snd_work 0..127) */
void Config_padd_snd(void)
{
    OPT_WORK *pW;

    pW = &Opt;
    if (pW->Step == 0) {
        pW->Sub_cur = 0;
        pW->Sub_cur_old = 0;
        pW->Timer = 0;
        pW->Step++;
        pW->Stereo = Snd_work.Mono;
        pW->Vol_bak[0] = Snd_work.Vol.Bgm;
        pW->Vol_bak[1] = Snd_work.Vol.Se;
        pW->Vol[0] = Snd_work.Vol.Bgm * 100 / 127;
        pW->Vol[1] = Snd_work.Vol.Se * 100 / 127;
        pW->Vol_old[0] = pW->Vol[0];
        pW->Vol_old[1] = pW->Vol[1];
    }
    if ((G.Padd_trg & 0x1000) && pW->Sub_cur == 0) {
        pW->Sub_cur = 3;
    } else if ((G.Padd_trg & 0x4000) && Opt.Sub_cur == 3) {
        Opt.Sub_cur = 0;
    } else {
        if (G.Padd_rep & 0x1000) {
            Opt.Sub_cur--;
        }
        if (G.Padd_rep & 0x4000) {
            Opt.Sub_cur++;
        }
        Opt.Sub_cur = Opt.Sub_cur < 0 ? 0 : Opt.Sub_cur > 3 ? 3 : Opt.Sub_cur;
    }
    if (G.Padd_rep & 0x5000) {
        Opt.Hold = 0;
        Opt.Arrow = 0;
        Config_pad_arrow_l = 0;
        Config_pad_arrow_r = 0;
        Config_se_arrow_l = 0;
        Config_se_arrow_r = 0;
    }
    if (Opt.Sub_cur_old != Opt.Sub_cur) {
        Opt.Timer = 0;
        Snd_se_stad(4, 0, 0, 0);
    }
    Opt.Sub_cur_old = Opt.Sub_cur;
    switch (Opt.Sub_cur) {
    case 0:
        if (Opt.Stereo == 0 && (G.Padd_trg & 0x2000)) {
            Snd_se_stad(4, 0, 0, 0);
            Opt.Timer = 0;
            Opt.Stereo = 1;
        }
        if (Opt.Stereo == 1 && (G.Padd_trg & 0x8000)) {
            Snd_se_stad(4, 0, 0, 0);
            Opt.Timer = 0;
            Opt.Stereo = 0;
        }
        break;
    case 1:
        if (G.Padd_rep & 0xA000) {
            Opt.Hold++;
            if (G.Padd_rep & 0x2000) {
                Opt.Vol[0] += Opt.Hold / 6 + 1;
            }
            if (G.Padd_rep & 0x8000) {
                Opt.Vol[0] -= Opt.Hold / 6 + 1;
            }
            Opt.Vol[0] = Opt.Vol[0] < 0 ? 0 : Opt.Vol[0] > 100 ? 100 : Opt.Vol[0];
            Snd_work.Vol.Bgm = (Opt.Vol[0] * 127 + 99) / 100;
            if (Opt.Vol_old[0] != Opt.Vol[0]) {
                Snd_se_stad(4, 0, 0, 0);
                if (Opt.Vol_old[0] > Opt.Vol[0]) {
                    Config_pad_arrow_l = 1;
                } else {
                    Config_pad_arrow_r = 1;
                }
                Opt.Arrow = 2;
            }
            Opt.Vol_old[0] = Opt.Vol[0];
        }
        if (!((USHORT)G.Padd & 0xA000)) {
            Opt.Hold = 0;
            Opt.Arrow = 0;
            Config_pad_arrow_l = 0;
            Config_pad_arrow_r = 0;
            Config_se_arrow_l = 0;
            Config_se_arrow_r = 0;
        }
        break;
    case 2:
        if (G.Padd_rep & 0xA000) {
            Opt.Hold++;
            if (G.Padd_rep & 0x2000) {
                Opt.Vol[1] += Opt.Hold / 6 + 1;
            }
            if (G.Padd_rep & 0x8000) {
                Opt.Vol[1] -= Opt.Hold / 6 + 1;
            }
            Opt.Vol[1] = Opt.Vol[1] < 0 ? 0 : Opt.Vol[1] > 100 ? 100 : Opt.Vol[1];
            Snd_work.Vol.Se = (Opt.Vol[1] * 127 + 99) / 100;
            if (Opt.Vol_old[1] != Opt.Vol[1]) {
                Snd_se_stad(4, 0, 0, 0);
                if (Opt.Vol_old[1] > Opt.Vol[1]) {
                    Config_se_arrow_l = 1;
                } else {
                    Config_se_arrow_r = 1;
                }
                Opt.Arrow = 2;
            }
            Opt.Vol_old[1] = Opt.Vol[1];
        }
        if (!((USHORT)G.Padd & 0xA000)) {
            Opt.Hold = 0;
            Opt.Arrow = 0;
            Config_pad_arrow_l = 0;
            Config_pad_arrow_r = 0;
            Config_se_arrow_l = 0;
            Config_se_arrow_r = 0;
        }
        break;
    case 3:
        if (Opt.Fade_mode != 0) {
            return;
        }
        if (G.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            Stereo_flg = Opt.Stereo;
            Config_page_exit();
        }
        break;
    }
    if (Opt.Fade_mode == 0 && (G.Key_trg & 0x2000)) {
        Snd_se_stad(5, 0, 0, 0);
        Snd_work.Vol.Bgm = Opt.Vol_bak[0];
        Snd_work.Vol.Se = Opt.Vol_bak[1];
        Config_page_exit();
    }
}

/* sound page: draw */
void Config_snd_disp(void)
{
    OPT_WORK *pW;
    LONG On;

    Config_moji_disp(0x82, 0x32, S_SOUND, 0, 0);
    pW = &Opt;
    Config_moji_disp(0x2C, 0x50, S_STEREO, pW->Stereo != 0, pW->Sub_cur == 0);
    Config_moji_disp(0xB4, 0x50, S_MONAURAL, pW->Stereo == 0, pW->Sub_cur == 0);
    if (pW->Arrow != 0) {
        pW->Arrow--;
        On = 1;
    } else {
        On = 0;
    }
    Config_moji_disp(0x2C, 0x6C, "\x82\x61\x82\x66\x82\x6C\x81\x44\x82\x75\x82\x6E\x82\x6B", Opt.Sub_cur != 1, 1);
    Config_suuji(0xDC, 0x6C, Opt.Vol[0]);
    Config_arrow_disp(0xCE, 0x6C, 0, Config_pad_arrow_l && On);
    Config_arrow_disp(0x106, 0x6C, 1, Config_pad_arrow_r && On);
    Config_moji_disp(0xBF, 0x6C, "\x81\x7C", 0, 0);
    Config_moji_disp(0x115, 0x6C, "\x81\x7B", 0, 0);
    Config_moji_disp(0x2C, 0x88, "\x82\x72\x82\x64\x81\x44\x82\x75\x82\x6E\x82\x6B", Opt.Sub_cur != 2, 1);
    Config_suuji(0xDC, 0x88, Opt.Vol[1]);
    Config_arrow_disp(0xCE, 0x88, 0, Config_se_arrow_l && On);
    Config_arrow_disp(0x106, 0x88, 1, Config_se_arrow_r && On);
    Config_moji_disp(0xBF, 0x88, "\x81\x7C", 0, 0);
    Config_moji_disp(0x115, 0x88, "\x81\x7B", 0, 0);
    Config_moji_disp(0x2C, 0xB0, S_EXIT, Opt.Sub_cur != 3, 1);
    Config_bg_disp();
}

/* monitor adjust page (Color.tim test picture) */
void Config_padd_clr(void)
{
    OPT_WORK *pW;

    pW = &Opt;
    if (pW->Step == 0) {
        CD.Fno = 0x40;
        CD.Fsize = Config_file_tbl[3].Size;
        CD.Fsector = (floc[0x40].sec_high << 16) + floc[0x40].sector + Config_file_tbl[3].Ofs;
        CD.Fsum = Config_file_sum[3];
        Cd_read(0x40, (ULONG *)0x8019C000, 2, "Color.tim");
        pW->Timer = 0;
        pW->Step++;
    }
    if (pW->Fade_mode == 0 && (G.Key_trg & 0x3000)) {
        if (G.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
        }
        if (G.Key_trg & 0x2000) {
            Snd_se_stad(5, 0, 0, 0);
        }
        Config_page_exit();
    }
}

/* monitor adjust page: draw */
void Config_clr_disp(void)
{
    Config_moji_disp(0x4E, 0x28, S_MONITOR_ADJUST, 0, 0);
    Config_moji_disp(0xFB, 0xCA, S_EXIT, 0, 1);
}

/* reset page */
void Config_padd_res(void)
{
    OPT_WORK *pW;

    pW = &Opt;
    if (pW->Step == 0) {
        pW->No = 1;
        pW->No_old = 1;
        pW->Sub_cur = 0;
        pW->Sub_cur_old = 0;
        pW->Timer = 0;
        pW->Step++;
    }
    if (G.Padd_trg & 0x1000) {
        pW->Sub_cur = 0;
    }
    if (G.Padd_trg & 0x4000) {
        pW->Sub_cur = 1;
    }
    if (pW->Sub_cur_old != pW->Sub_cur) {
        Snd_se_stad(4, 0, 0, 0);
        pW->Timer = 0;
    }
    pW->Sub_cur_old = pW->Sub_cur;
    switch (pW->Sub_cur) {
    case 0:
        if (G.Padd_trg & 0x2000) {
            pW->No = 1;
        }
        if (G.Padd_trg & 0x8000) {
            pW->No = 0;
        }
        if (pW->No_old != pW->No) {
            Snd_se_stad(4, 0, 0, 0);
            pW->Timer = 0;
        }
        pW->No_old = pW->No;
        if (pW->Fade_mode != 0) {
            return;
        }
        if (G.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            if (pW->No == 0) {
                pW->Reset = 1;
            }
            Config_page_exit();
        }
        break;
    case 1:
        if (pW->Fade_mode != 0) {
            return;
        }
        if (G.Key_trg & 0x1000) {
            Snd_se_stad(6, 0, 0, 0);
            Config_page_exit();
        }
        break;
    }
    if (Opt.Fade_mode == 0 && (G.Key_trg & 0x2000)) {
        Snd_se_stad(5, 0, 0, 0);
        Config_page_exit();
    }
}

/* reset page: draw */
void Config_res_disp(void)
{
    Config_moji_disp(0x84, 0x36, S_RESET, 0, 0);
    Config_moji_disp(0x4E, 0x64, "\x82\x71\x82\x64\x82\x72\x82\x64\x82\x73\x81\x40\x82\x73\x82\x67\x82\x64\x81\x40\x82\x66\x82\x60\x82\x6C\x82\x64\x81\x48", 0, 0);
    Config_moji_disp(0x6A, 0x82, "\x82\x78\x82\x64\x82\x72", Opt.Sub_cur != 0 || Opt.No != 0, 1);
    Config_moji_disp(0x98, 0x82, S_SLASH, 0, 0);
    Config_moji_disp(0xB3, 0x82, "\x82\x6D\x82\x6E", Opt.Sub_cur != 0 || Opt.No != 1, 1);
    Config_moji_disp(0x8B, 0xAA, S_EXIT, Opt.Sub_cur != 1, 1);
    Config_bg_disp();
}

/* draws a fullwidth Shift-JIS string */
INCLUDE_ASM("asm/nonmatchings/ovl/option", Config_moji_disp);

__asm__(".section .rodata\n.align 2\n.section .text"); /* literals are word aligned */
INCLUDE_RODATA("asm/nonmatchings/ovl/option", Config_space_str);

/* draws a number (RE2 Config_suuji role) */
INCLUDE_ASM("asm/nonmatchings/ovl/option", Config_suuji);

/* compares one fullwidth (2 byte) character */
LONG Config_moji_cmp(char *pA, char *pB)
{
    if (pA[1] == pB[1] && pA[0] == pB[0]) {
        return 1;
    }
    return 0;
}

/* left (Pad 0) / right (Pad 1) arrow, lit when On */
void Config_arrow_disp(LONG X, LONG Y, ULONG Pad, ULONG On)
{
    SPRT *p;

    p = Opt.pIcon;
    p->x0 = X;
    p->y0 = Y;
    switch (Pad * 2 | On) {
    case 0:
        p->u0 = 0xD8;
        p->v0 = 0x20;
        break;
    case 1:
        p->u0 = 0xE6;
        p->v0 = 0x20;
        break;
    case 2:
        p->u0 = 0xD8;
        p->v0 = 0x30;
        break;
    case 3:
        p->u0 = 0xE6;
        p->v0 = 0x30;
        break;
    }
    AddPrim(G.pOt0 + 3, p);
    Opt.pIcon++;
}

/* leave the page: back to the top menu */
void Config_page_exit(void)
{
    Opt.Routine_next = 0;
    Opt.Step = 0;
    Opt.Fade_mode = 1;
}

/* animate the background pieces (frame scripts, random jitter) */
void Config_piece_move(void)
{
    OPT_PIECE *p;
    ULONG i;

    p = Opt.Piece;
    for (i = 0; i < 20; p++, i++) {
        if (p->Be_flg == 0) {
            continue;
        }
        if (--p->Timer == -1) {
            if (*(p->pAnim += 2) == 0xFF) {
                p->pAnim = Config_piece_anim[p->Type];
            }
            p->Anim_no = p->pAnim[0];
            p->Timer = p->pAnim[1] + (Rnd() & 3);
        }
        switch (Rnd() & 3) {
        case 3:
            p->Pos_x += Rnd() & 3;
            break;
        case 0:
            p->Pos_x -= Rnd() & 3;
            break;
        }
        switch (Rnd() & 3) {
        case 2:
            p->Pos_y += Rnd() & 3;
            break;
        case 3:
            p->Pos_y -= Rnd() & 3;
            break;
        }
        switch (Rnd() & 3) {
        case 0:
            p->Rot_y += Rnd() & 3;
            break;
        case 3:
            p->Rot_y -= Rnd() & 3;
            break;
        }
    }
}

/* draw the background pieces rotated about y */
void Config_piece_disp(void)
{
    MATRIX m;
    SVECTOR v[4];
    OPT_PIECE *p;
    POLY_FT4 *pPoly;
    ULONG *pOt;
    ULONG Ofs;
    ULONG i;
    SHORT x;
    SHORT y;
    ULONG Size;
    ULONG Half;
    UCHAR u;
    UCHAR vv;

    p = Opt.Piece;
    v[0].vy = 0;
    v[1].vy = 0;
    v[2].vy = 0;
    v[3].vy = 0;
    pOt = G.pOt0 + 6;
    Ofs = G.Cbuf_id * sizeof(POLY_FT4) + 0x10;
    for (i = 0; i < 20; p++, i++) {
        if (p->Be_flg == 0) {
            continue;
        }
        x = p->Pos_x >> 6;
        y = p->Pos_y >> 6;
        m = GsIDMATRIX;
        RotMatrixY(p->Rot_y, &m);
        Size = Config_piece_shape[p->Id].Size;
        Half = Size >> 1;
        v[0].vx = -Half + 1;
        v[0].vz = -Half + 1;
        v[1].vx = Half;
        v[1].vz = -Half + 1;
        v[2].vx = -Half + 1;
        v[2].vz = Half;
        v[3].vx = Half;
        v[3].vz = Half;
        ApplyMatrixSV(&m, &v[0], &v[0]);
        ApplyMatrixSV(&m, &v[1], &v[1]);
        ApplyMatrixSV(&m, &v[2], &v[2]);
        ApplyMatrixSV(&m, &v[3], &v[3]);
        pPoly = (POLY_FT4 *)((UCHAR *)p + Ofs);
        pPoly->x0 = v[0].vx + x;
        pPoly->y0 = v[0].vz + y;
        pPoly->x1 = v[1].vx + x;
        pPoly->y1 = v[1].vz + y;
        pPoly->x2 = v[2].vx + x;
        pPoly->y2 = v[2].vz + y;
        pPoly->x3 = v[3].vx + x;
        pPoly->y3 = v[3].vz + y;
        u = Config_piece_shape[p->Id].U + p->Anim_no * Size;
        vv = Config_piece_shape[p->Id].V;
        pPoly->u0 = u;
        pPoly->v0 = vv;
        pPoly->u1 = u + Size - 1;
        pPoly->v1 = vv;
        pPoly->u2 = u;
        pPoly->v2 = vv + Size - 1;
        pPoly->u3 = u + Size - 1;
        pPoly->v3 = vv + Size - 1;
        AddPrim(pOt, pPoly);
    }
}

/* start a background piece in the first free slot */
void Config_piece_set(LONG Id, LONG Type, LONG X, LONG Y, SHORT Rot_y)
{
    OPT_PIECE *p;
    UCHAR *pAnim;
    ULONG i;

    p = Opt.Piece;
    pAnim = Config_piece_anim[Type];
    for (i = 0; i < 20; p++, i++) {
        if (p->Be_flg == 0) {
            p->Be_flg = 1;
            p->Id = Id;
            p->Type = Type;
            p->Pos_x = X << 6;
            p->Pos_y = Y << 6;
            p->Rot_y = Rot_y;
            p->pAnim = pAnim;
            p->Anim_no = pAnim[0];
            p->Timer = pAnim[1] + (Rnd() & 3);
            return;
        }
    }
}
