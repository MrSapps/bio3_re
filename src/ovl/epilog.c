#include "ovl/epilog.h"
#include "pad.h"
#include "sld.h"

/* task entry */
void Epilog_main(void)
{
    EPI_WORK *pWk = &Epi_work;
    ULONG i, j;
    SPRT *pSp;
    DR_TPAGE *pTp;
    USHORT *pPos;

    Task_suspend(0);
    Pad_rep_set(0xF000, 0x60A);
    Main.Vloop = 1;
    Cd_read(0x11F, (ULONG *)0x801F7E00, 0, "EPILOG BGM");
    Snd_seq_data_set(0, 0x33, (ULONG *)0x801F7E00);
    Snd_bank_set(5, Cd_read_s(0x120, 2, (ULONG *)0x80100000, 5, "EPILOG VB"), 0);
    Snd_se_stad(0x500, NULL, 0, 0);
    Cd_read(0x1A, (ULONG *)0x80100000, 0, "EPIS.TIM");
    DrawSync(0);
    Memcpy((void *)0x8019C000, (void *)0x80100000, 0x26000);
    G.Ctp.Tpage_clut = 0xD05;
    Mapping_tim((ULONG *)0x80126000);
    Bg_set_mode(2, 0);
    Main.Unk191 = 1;
    pTp = pWk->Tpage;
    for (i = 0; i < 2; i++) {
        SetDrawTPage(pTp, 0, 0, 0x85);
        pTp++;
    }
    pSp = pWk->Page[0];
    pPos = D_801C2BA8[0];
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 2; i++, pSp++) {
            SetSprt(pSp);
            setRGB0(pSp, 0x80, 0x80, 0x80);
            setXY0(pSp, pPos[0], pPos[1]);
            setUV0(pSp, 0, 0);
            setWH(pSp, 0x46, 0x36);
            pSp->clut = 0x7B40;
        }
        pPos += 2;
    }
    pSp = pWk->Cursor;
    for (i = 0; i < 2; i++, pSp++) {
        SetSprt(pSp);
        pSp->r0 = 0x60;
        pSp->g0 = 0x60;
        pSp->b0 = 0x60;
        pSp->u0 = 0;
        pSp->v0 = 0x40;
        pSp->w = 0x46;
        pSp->h = 0x36;
        pSp->clut = 0x7B40;
    }
    pWk->Rno0 = 0;
    pWk->Rno1 = 0;
    pWk->Rno2 = 0;
    pWk->Rno3 = 0;
    pWk->u.s.Kido_dir = 0;
    pWk->Kido = 0;
    Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 0x1E);
    Flg_on(pWk->u.Flg, 0);
    Flg_on(pWk->u.Flg, 1);
    while (Flg_ck(pWk->u.Flg, 0)) {
        Epilog_routine[pWk->Rno0]();
        Epilog_draw();
        Task_sleep(1);
    }
    Main.Vloop = 2;
    Task_signal(0);
    Task_exit();
}

/* routine 1: menu */
void Epilog_menu(void)
{
    EPI_WORK *pWk = &Epi_work;
    USHORT Max;
    UCHAR Se;

    Max = G.Unk7806;
    if (Max >= 8) {
        Max = 8;
    }
    if (G.Key_trg & 0x2000) {
        Epi_work.Rno0 = 3;
        pWk->Rno1 = 0;
        pWk->Rno2 = 0;
        pWk->Rno3 = 0;
        Snd_se_stad(5, NULL, 0, 0);
    } else if (G.Key_trg & 0x1000) {
        if (pWk->u.s.Cursor == 8) {
            Epi_work.Rno0 = 3;
            pWk->Rno1 = 0;
            pWk->Rno2 = 0;
            pWk->Rno3 = 0;
            Snd_se_stad(5, NULL, 0, 0);
        } else if (pWk->u.s.Cursor < Max) {
            Epi_work.Rno0 = 2;
            pWk->Rno1 = 0;
            pWk->Rno2 = 0;
            pWk->Rno3 = 0;
            Snd_se_stad(6, NULL, 0, 0);
        } else {
            Snd_se_stad(7, NULL, 0, 0);
        }
    } else {
        Se = 0;
        if ((G.Unk2100 & 0x1000) && (signed char)(pWk->u.s.Cursor / 3) > 0) {
            pWk->u.s.Cursor -= 3;
            Se = 1;
        }
        if ((G.Unk2100 & 0x4000) && (signed char)(pWk->u.s.Cursor / 3) < 2) {
            pWk->u.s.Cursor += 3;
            Se = 1;
        }
        if ((G.Unk2100 & 0x8000) && (signed char)(pWk->u.s.Cursor % 3) > 0) {
            pWk->u.s.Cursor--;
            Se = 1;
        }
        if ((G.Unk2100 & 0x2000) && (signed char)(pWk->u.s.Cursor % 3) < 2) {
            pWk->u.s.Cursor++;
            Se = 1;
        }
        if (Se) {
            Snd_se_stad(4, NULL, 0, 0);
        }
        if (pWk->u.s.Kido_dir) {
            pWk->Kido -= 2;
        } else {
            pWk->Kido += 2;
        }
        if (pWk->Kido >= 0x3F) {
            pWk->Kido = 0x3F;
            pWk->u.s.Kido_dir = 1;
        }
        if (pWk->Kido <= 0) {
            pWk->Kido = 0;
            pWk->u.s.Kido_dir = 0;
        }
    }
}

/* routine 2: show the selected epilogue */
void Epilog_show(void)
{
    EPI_WORK *pWk = &Epi_work;

    switch (pWk->Rno1) {
    case 1:
        if (Fade_status(0) & FADE_END) {
            DrawSync(0);
            Epilog_pic_read((ULONG *)0x8019C000, pWk->u.s.Cursor);
            Flg_off(pWk->u.Flg, 1);
            Flg_off(pWk->u.Flg, 2);
            Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 0xF);
            pWk->Rno1++;
        }
        break;
    case 2:
        if (Fade_status(0) == 0) {
            pWk->Rno1++;
        }
        break;
    case 3:
        if (!(G.Key_trg & 0x3000)) {
            break;
        }
        Snd_se_stad(5, NULL, 0, 0);
    case 0:
        Fade_set(0, 0, 2, 2, 0, 0xFFFFFF, 0xF);
        pWk->Rno1++;
        break;
    case 4:
        if (Fade_status(0) & FADE_END) {
            DrawSync(0);
            Memcpy((void *)0x8019C000, (void *)0x80100000, 0x26000);
            Flg_on(pWk->u.Flg, 1);
            Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 0xF);
            pWk->Rno0 = 0;
            pWk->Rno1 = 0;
            pWk->Rno2 = 0;
            pWk->Rno3 = 0;
        }
        break;
    }
}

/* routine 0: wait for the fade in, then start the menu */
void Epilog_start(void)
{
    EPI_WORK *pWk = &Epi_work;

    if (Fade_status(0) == 0) {
        Epi_work.Rno0 = 1;
        pWk->Rno1 = 0;
        pWk->Rno2 = 0;
        pWk->Rno3 = 0;
        pWk->u.s.Kido_dir = 0;
        pWk->Kido = 0;
        Flg_on(pWk->u.Flg, 2);
        pWk->Cursor[0].r0 = 0x60;
        pWk->Cursor[0].g0 = 0x60;
        pWk->Cursor[0].b0 = 0x60;
        pWk->Cursor[1].r0 = 0x60;
        pWk->Cursor[1].g0 = 0x60;
        pWk->Cursor[1].b0 = 0x60;
    }
}

/* routine 3: fade out the BGM and the screen, then end the task */
void Epilog_exit(void)
{
    EPI_WORK *pWk = &Epi_work;

    switch (pWk->Rno1) {
    case 0:
        Snd_work.Seq[0].Fade_mode = 2;
        Snd_work.Seq[0].Fade_vol = 0x3DAF;
        Snd_work.Seq[0].Fade_step = 0x250;
        Fade_set(0, 0, 2, 2, 0, 0xFFFFFF, 0x1E);
        pWk->Rno1++;
        break;
    case 1:
        if ((Fade_status(0) & FADE_END) && Snd_work.Seq[0].Fade_mode == 0) {
            Flg_off(pWk->u.Flg, 0);
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/epilog", Epilog_draw);

/* load epilogue picture No of OMBG.TIM */
void Epilog_pic_read(ULONG *pAdr, UCHAR No)
{
    CD.Fno = 0x3C;
    CD.Fsize = D_801C2B4C[No].Size;
    CD.Fsector = (floc[0x3C].sec_high << 16) + floc[0x3C].sector + D_801C2B4C[No].Ofs;
    CD.Fsum = D_801C2B9C[No];
    Cd_read(0x3C, pAdr, 2, "OMBG.TIM");
}
/* the original .rodata ends with three junk bytes after "OMBG.TIM" */
__asm__(".section .rodata\n\t.byte 0x1F, 0, 0x78\n\t.section .text");
