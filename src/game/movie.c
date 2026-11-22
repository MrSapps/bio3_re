#include "movie.h"
#include "pad.h"
#include "cd.h"
#include "snd.h"

/* MOVIE.C: STR movie playback (DS streaming + MDEC), RE2 MOVIE.C reworked for RE3 */

LONG Pad_vib_init_l(void) __asm__("Pad_vib_init"); /* int-returning alias */

/* Returns Pad_vib_init's v0 (the sce*.c callers declare it LONG): that explicit v0 use is what makes
   reload put the /127 mfhi in t0 (see NOTES.md). */
LONG Init_movie_work(ULONG Id)
{
    MOVIE_TBL *pTbl;
    SND_WORK *pSnd = &Snd_work;
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    pTbl = &Movie_tbl[Id];
    if (pTbl->Attribute & 0x200)
        pWork->pFree_work = (UCHAR *)0x80100000;
    else
        pWork->pFree_work = (UCHAR *)0x80194000;
    if (pTbl->Attribute & 8) {
        pSnd->UnkB1C = 0x80;
        pSnd->UnkB24 = pSnd->UnkB20 = 0x3FFF;
        pWork->Save_flg = G.Unk2468;
        G.Unk2468 |= 0xFF000000;
    }
    pMovie_result = Ret_Value;
    pVm_frm = (VM_FRAME *)pWork->pFree_work;
    pWork->Movie_Isrgb24 = 1;
    pWork->pFree_work += sizeof(VM_FRAME);
    pFile = (DslFILE *)pWork->pFree_work;
    pWork->pFree_work += sizeof(DslFILE);
    pAtv = (DslATV *)pWork->pFree_work;
    pWork->pFree_work += sizeof(DslATV);
    pWork->Movie_id = Id;
    pWork->Rl_buff[0] = (ULONG *)pWork->pFree_work;
    pWork->pFree_work += Movie_tbl[pWork->Movie_id].Rl_size;
    if (pDisp->Attribute & 0x100) {
        pWork->Rl_buff[1] = (ULONG *)((UCHAR *)pWork->Rl_buff[0] + Movie_tbl[pWork->Movie_id].Rl_size);
        pWork->pFree_work += Movie_tbl[pWork->Movie_id].Rl_size;
    } else {
        pWork->Rl_buff[1] = pWork->Rl_buff[0];
    }
    pWork->pImage_buff = (ULONG *)((UCHAR *)pWork->Rl_buff[1] + Movie_tbl[pWork->Movie_id].Rl_size);
    pWork->pFree_work += 0x1E00;
    pWork->Movie_w = pTbl->Disp_w;
    pWork->Rno = 0;
    pWork->Movie_sync = 0;
    pDisp->Rect.w = pDisp->Width = 24;
    pDisp->Pos_x = pTbl->Pos_x;
    pDisp->Pos_y = pTbl->Pos_y;
    pDisp->Attribute = pTbl->Attribute;
    if (pDisp->Attribute & 2) {
        if (pWork->Eom == 2) {
            pWork->Eom = 1;
            pWork->Padoff_stop = 2;
            pDisp->Attribute = (pDisp->Attribute & ~0x80) | 0x40;
        } else {
            pWork->Eom = 0;
            pWork->Padoff_stop = 0;
        }
    } else {
        pWork->Eom = 0;
    }
    pWork->Movie_volume = pTbl->Volume * Snd_work.Vol.Bgm / 127;
    pWork->Frame = 0;
    pWork->Free0 = 0;
    pWork->Movie_cancel = pTbl->Cancel;
    G.System_flg |= 0x18000;
    return Pad_vib_init_l();
}

void Movie(void)
{
    VM_WORK *pWork;

    pWork = &Vm_work;
    do {
        pMovie_proc_tbl[pWork->Rno]();
        if (G.System_flg & 0x8000) {
            Main.Db[G.Cbuf_id ^ 1].Disp.isrgb24 = pWork->Movie_Isrgb24;
            Main.Db[G.Cbuf_id ^ 1].Disp.disp.w = pWork->Movie_w;
        }
        if (pWork->Frame != 0) {
            G.Cbuf_id ^= 1;
        }
    } while (pWork->Rno < 2);
}

/* VRAM saved under an overlay movie (Movie_init) and restored by Movie_exit */
static const RECT Movie_save_rect __asm__("D_800106EC") = {0x140, 0, 0xA0, 0x1E0};
static const RECT Movie_save_rect2 __asm__("D_800106F4") = {0x1C0, 0xF0, 0x20, 0xF0};

void Movie_init(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;
    RECT Rect = Movie_save_rect;
    RECT Rect2 = Movie_save_rect2;

    if (pDisp->Attribute & 8) {
        Set_movie_volume2(pWork->Movie_volume);
        func_80074658(1);
        *(ULONG *)&Rect.w = 0x10000A0;
        StoreImage(&Rect, (ULONG *)(((ULONG)Vm_work.pFree_work + 15) & ~15));
        Vm_work.pFree_work += 0x14010;
        StoreImage(&Rect2, (ULONG *)(((ULONG)Vm_work.pFree_work + 15) & ~15));
        Vm_work.pFree_work += 0x3C10;
        DrawSync(0);
        if (Open_movie(pWork->Movie_id)) {
            Movie_end();
            pWork->Status = 1;
            pWork->Rno += 2;
        } else {
            pWork->Status = 0;
            pWork->Frame = 0;
            pWork->Rno++;
        }
    } else if (pWork->Eom) {
        Movie_end();
        pWork->Rno += 2;
    } else {
        Set_movie_volume2(pWork->Movie_volume);
        func_80074658(1);
        if (Open_movie(pWork->Movie_id)) {
            Movie_end();
            pWork->Status = 1;
            pWork->Rno += 2;
        } else {
            pWork->Status = 0;
            if (pDisp->Attribute & 1) {
                Fade_off(0);
                Bg_set_mode(0, 0);
            }
            pWork->Frame = 0;
            pWork->Rno++;
        }
    }
}

void Movie_main(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;
    SHORT Eom;
    ULONG Rect[2];                     /* RECT written as two words */

    DrawSync(0);
    VSync(2);
    PutDrawEnv(&Main.Db[G.Cbuf_id].Draw);
    PutDispEnv(&Main.Db[G.Cbuf_id].Disp);
    if (pWork->Frame < 2) {
        if (Main.Db[0].Draw.ofs[1] == 0)
            Rect[0] = (G.Cbuf_id != 0) ? 0xF0 << 16 : 0;
        else
            Rect[0] = (G.Cbuf_id == 0) ? 0xF0 << 16 : 0;
        Rect[1] = (0xF0 << 16) | 0x1E0;
        ClearImage((RECT *)Rect, 0, 0, 0);
        if (pWork->Frame == 0)
            Movie_xa_start();
    }
    pWork->Frame++;
    Eom = Play_movie();
    if (pWork->Status == 0) {
        while (pWork->Movie_sync == 0)
            ;
    }
    switch (Eom) {
    case 1:
        pWork->Eom = 1;
        pWork->Padoff_stop = 0;
        break;
    case 0:
        if (!(pDisp->Attribute & 2) && pWork->Frame <= 60)
            break;
        if (pWork->Movie_cancel == 0)
            break;
        Pad_set();
        if (!((USHORT)G.Unk20F8 & pWork->Movie_cancel))
            break;
    default:
        /* error, or skipped with a cancel button */
        pWork->Eom = 2;
        pWork->Padoff_stop = 2;
        pDisp->Attribute = (pDisp->Attribute & ~0x80) | 0x40;
        break;
    }
    if (pWork->Eom && (pWork->Frame & 1)) {
        pWork->Frame = 0;
        pWork->Rno++;
        Movie_end();
    }
}

void Movie_end(void)
{
    RECT unused_rect = {0x140, 0, 0xa0, 0x200};
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    Close_movie();
    if (pDisp->Attribute & 1)
        Bg_set_mode(2, 0);
    if (pDisp->Attribute & 0x40)
        Bg_set_mode(1, 0);
    if (pDisp->Attribute & 0x80)
        Bg_set_mode(1, 0);
    pWork->Movie_Isrgb24 = 0;
    pWork->Movie_w = 0x140;
    if (pDisp->Attribute & 8)
        G.System_flg &= ~0x8000;
    SetDispMask(0);
}

void Movie_exit(void)
{
    RECT Rect = Movie_save_rect;
    RECT Rect2 = Movie_save_rect2;
    SND_WORK *pSnd = &Snd_work;
    tagMAIN *pMain = &Main;            /* unused; its address is shared by the Main accesses below */
    VM_WORK *pWork = &Vm_work;

    if (Vm_disp.Attribute & 8) {
        switch (pWork->Free0) {
        case 0:
            /* restore the VRAM saved by Movie_init */
            Vm_work.pFree_work -= 0x3C10;
            LoadImage(&Rect2, (ULONG *)(((ULONG)Vm_work.pFree_work + 15) & ~15));
            Vm_work.pFree_work -= 0x14010;
            *(ULONG *)&Rect.w = 0x10000A0;
            LoadImage(&Rect, (ULONG *)(((ULONG)Vm_work.pFree_work + 15) & ~15));
            DrawSync(0);
            Main.Db[0].Disp.isrgb24 = pWork->Movie_Isrgb24;
            Main.Db[0].Disp.disp.w = pWork->Movie_w;
            Main.Db[1].Disp.isrgb24 = pWork->Movie_Isrgb24;
            Main.Db[1].Disp.disp.w = pWork->Movie_w;
            G.Cbuf_id ^= 1;
            Bg_set_mode(2, 0);
            Main.Unk18E = 1;
            Bg_draw();
            pSnd->UnkB1C = 0x80;
            pSnd->UnkB20 = 0;
            pSnd->UnkB24 = -0xB00;
            break;
        case 1:
            if (pSnd->UnkB1C != 0)
                return;
            Main.Unk18E = 0;
            break;
        case 2:
            G.Unk2468 = pWork->Save_flg;
            G.System_flg &= ~0x10000;
            SetDispMask(1);
            break;
        }
    } else {
        switch (pWork->Free0) {
        case 0:
            break;
        case 1:
            G.System_flg &= ~0x8000;
            G.System_flg &= ~0x10000;
            SetDispMask(1);
            break;
        }
    }
    pWork->Free0++;
}

ULONG Open_movie(ULONG Movie_id)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp;
    StHEADER *pRead_header;
    ULONG *pBs_buff;
    ULONG Time_out;
    ULONG Start;
    FLOC *pFpos;
    UCHAR Param[4];

    pWork->End_frame = Movie_tbl[Movie_id].N_frame;
    pWork->Cbuf_id = 0;
    pVm_frm->No = pVm_frm->Old_no = 0;
    pDisp = &Vm_disp;
    Movie_xa_stop();
    StSetRing((ULONG *)Vm_work.pFree_work, 25);
    StSetStream(1, 0, 0xFFFFFFF, NULL, NULL);
    pFpos = &floc[Movie_tbl[Movie_id].Filecode];
    Start = (pFpos->sec_high << 16) + pFpos->sector;
    if (Movie_tbl[Movie_id].Free)
        Start += Movie_tbl[Movie_id].Free * 5;
    DsIntToPos(pWork->Start_sector = Start, &Loc);
    Channel.file = Channel.chan = 1;
    Param[0] = DslModeSpeed | DslModeSize1 | DslModeSF;
    DsCommand(DslPause, NULL, NULL, -1);
    DsCommand(DslSetmode, Param, NULL, -1);
    DsCommand(DslSetfilter, (UCHAR *)&Channel, NULL, -1);
    DsRead2(&Loc, DslModeStream | DslModeSpeed | DslModeRT | DslModeSF);
    Time_out = 0;
    while (StGetNext(&pBs_buff, (ULONG **)&pRead_header) != 0) {
        VSync(0);
        if (++Time_out >= 0xB5)
            return 1;
    }
    DecDCTReset(0);
    DecDCToutCallback(Mdec_callback);
    DecDCTvlc(pBs_buff, pWork->Rl_buff[pWork->Cbuf_id]);
    pDisp->Rect.h = pVm_frm->Pic_h = ((ULONG)pRead_header->height + 8) & 0xFFF0;
    pDisp->Size = pVm_frm->Pic_h * 12;
    pDisp->Pos_x = (pRead_header->width != 320) ? 0x30 : 0;
    pDisp->Loop = ((ULONG)pRead_header->width + 8) >> 4;
    pVm_frm->No = pVm_frm->Old_no = pRead_header->frameCount;
    StFreeRing(pBs_buff);
    return 0;
}

ULONG Play_movie(void)
{
    ULONG Time_out = 0;
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;
    ULONG *pBs_buff;
    StHEADER *pRead_header;
    ULONG Ret;

    while (StGetNext(&pBs_buff, (ULONG **)&pRead_header) != 0) {
        VSync(0);
        if (++Time_out >= 0xB4) {
            pWork->Status = 1;
            return 2;
        }
    }
    pVm_frm->No = pRead_header->frameCount;
    if (pVm_frm->No >= pWork->End_frame || pVm_frm->No < pVm_frm->Old_no)
        Ret = 1;
    else
        Ret = 0;
    pVm_frm->Old_no = pVm_frm->No;
    RuntoImage();
    pWork->Cbuf_id ^= 1;
    pDisp->Pos_x = (pRead_header->width != 320) ? 0x30 : 0;
    pDisp->Loop = ((ULONG)pRead_header->width + 8) >> 4;
    DecDCTvlc(pBs_buff, pWork->Rl_buff[pWork->Cbuf_id]);
    StFreeRing(pBs_buff);
    return Ret;
}

void Close_movie(void)
{
    Movie_xa_stop();
    DecDCToutCallback(NULL);
    DsDataCallback(NULL);
    DsReadyCallback(NULL);
    DsCommand(DslPause, NULL, NULL, -1);
    StUnSetRing();
    func_80074658(0);
}

void RuntoImage(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    pDisp->Cur_pos_x = pDisp->Pos_x;
    if (Main.Db[0].Draw.ofs[1] == 0) {
        pDisp->Rect.y = (G.Cbuf_id != 0) ? 0xF0 : 0;
    } else {
        pDisp->Rect.y = (G.Cbuf_id == 0) ? 0xF0 : 0;
    }
    pDisp->Count = 0;
    pDisp->Rect.y += pDisp->Pos_y;
    DecDCTin(pWork->Rl_buff[pWork->Cbuf_id], 1);
    DecDCTout(pWork->pImage_buff, pDisp->Size);
    DecDCTinSync(0);
}

void Mdec_callback(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    if (StCdIntrFlag) {
        StCdInterrupt();
        StCdIntrFlag = 0;
    }
    pDisp->Rect.x = pDisp->Cur_pos_x;
    LoadImage(&pDisp->Rect, pWork->pImage_buff);
    if (++pDisp->Count < pDisp->Loop) {
        pDisp->Cur_pos_x += pDisp->Width;
        DecDCTout(pWork->pImage_buff, pDisp->Size);
    } else {
        pWork->Movie_sync = 1;
    }
}

void Movie_xa_start(void)
{
    LONG Ctr;

    for (Ctr = 0; Ctr <= (LONG)Vm_work.Movie_volume; Ctr++) {
        Set_movie_volume2(Ctr);
    }
}

void Movie_xa_stop(void)
{
    LONG Ctr;

    for (Ctr = Vm_work.Movie_volume; Ctr >= 2; Ctr -= 2) {
        Set_movie_volume2(Ctr);
    }
    Set_movie_volume2(0);
}

void Set_movie_volume2(ULONG Volume)
{
    if (Stereo_flg == 0) {
        pAtv->val0 = pAtv->val2 = Volume;
        pAtv->val1 = pAtv->val3 = 0;
    } else {
        pAtv->val0 = pAtv->val1 = pAtv->val2 = pAtv->val3 = Volume >> 1;
    }
    DsMix(pAtv);
}

/* set the CD mode and seek back to the first file: 1 = ok, 0 = failed (no callers) */
LONG CdReset2(LONG Mode)
{
    UCHAR Param;
    UCHAR Status;
    ULONG Time_out;

    Time_out = 0;
    if (Mode == 0)
        Param = DslModeSpeed | DslModeSize0;
    else
        Param = DslModeSpeed | DslModeSize1 | DslModeSF;
    DsControlB(DslSetmode, &Param, pMovie_result);
    Status = *pMovie_result;
    while (Status & (DslStatShellOpen | DslStatSeekError | DslStatError)) {
        /* wait for the shell to close */
        while (Status & DslStatShellOpen) {
            VSync(5);
            DsControlB(DslNop, NULL, pMovie_result);
            Time_out++;
            Status = *pMovie_result;
            if (Time_out >= 0x25)
                return 0;
        }
        Time_out = 0;
        if (!(Status & DslStatStandby)) {
            /* wait for the spindle motor */
            do {
                VSync(5);
                DsControlB(DslNop, NULL, pMovie_result);
                Time_out++;
                Status = *pMovie_result;
                if (Time_out >= 0x25)
                    return 0;
            } while ((Status & DslStatStandby) == 0);
        }
        DsControlB(DslSetmode, &Param, pMovie_result);
        Status = *pMovie_result;
    }
    VSync(3);
    DsIntToPos((floc[0].sec_high << 16) | floc[0].sector, &Loc);
    do {
        while (DsSync(1, NULL) != DslComplete)
            ;
    } while (DsControlB(DslSeekL, (UCHAR *)&Loc, NULL) == 0 || DsControlB(DslPause, NULL, NULL) == 0);
    return 1;
}
