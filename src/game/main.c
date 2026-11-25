/*
 * MAIN.C (RE2 MAIN.C lineage): main loop, system/global init, buffer swap, background, screen fades,
 * cinema-scope curtain, camera cut zones, pause.
 */
#include "main.h"
#include "monitor.h"
#include "pad.h"
#include "line_scr.h"
#include "sub.h"
#include "cd.h"
#include "sce0.h"
#include "game.h"
#include "psp.h"
#include "room.h"
#include "sld.h"
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

/* RE2 main: system init, then the frame loop (pause, task scheduler, sound, CD, drawing, buffer swap) */
int main(void)
{
    tagMAIN *pMain;
    tagGLOBAL *pG;
    ULONG i;

    Init_system();
    pMain = &Main;
    pG = &G;
    while (1) {
        if (Main.Pollhost_flg != 0) {
            __asm__ volatile(".word 0x0001000D"); /* break 1 (RE2 trap(0x400)) */
        }
        pG->pOt0 = pG->Ot0[pG->Cbuf_id];
        pG->pOt1 = pG->Ot1[pG->Cbuf_id];
        pG->pOt2 = pG->Ot2[pG->Cbuf_id];
        pMain->pDb = &pMain->Db[pG->Cbuf_id];
        ClearOTagR(pG->pOt0, 8);
        ClearOTagR(pG->pOt1, 0x400);
        ClearOTagR(pG->pOt2, 8);
        if (pG->Unk2470) {
            i = 0;
            do {
                ClearOTagR(pG->pOt_sub[pG->Cbuf_id][i + 1], 0x400);
                i++;
            } while (i < pG->Unk2470);
        }
        Moji_mode_init();
        if (pG->Padd_data[0][0] != 0) {
            pG->Padd_data[0][3] = 0xFF;
            pG->Padd_data[0][2] = 0xFF;
            pG->Padd_data[1][3] = 0xFF;
            pG->Padd_data[1][2] = 0xFF;
        }
        if (pG->Padd_data[1][0] != 0) {
            pG->Padd_data[1][3] = 0xFF;
            pG->Padd_data[1][2] = 0xFF;
        }
        Pad_set();
        if (G.System_flg & 0x200000) {
            Soft_reset();
            continue;
        }
        if ((G.System_flg & 0x8000000) && Main.Unk18E == 0 && *(USHORT *)&Game.Doordemo_flg == 0
            && *(USHORT *)&CD.Cd_rtn_no == 0 && !(G.Unk2468 & 0x1000000)
            && !(G.System_flg & 0x10594240) && !(G.Status_flg & 0x400208) && Main.Unk194 == 0) {
            if ((G.Unk20FC & 0x800) && G.Unk246C >= 0) {
                G.Unk20FC ^= 0x800;
                Main.Unk194 = 2;
            } else if (pG->Padd_data[0][0] == 0xFF) {
                Main.Unk194 = 1;
            } else {
                goto no_pause;
            }
            Snd_seq_pause(0);
            Snd_seq_pause(1);
            Snd_seq_pause(2);
            G.Unk2488 = 1;
        }
    no_pause:
        if (Main.Unk194 != 0) {
            Pause_disp();
            if ((Main.Unk194 == 1 && pG->Padd_data[0][0] != 0xFF) || (Main.Unk194 == 2 && (G.Unk20FC & 0x800))) {
                Snd_seq_play_ck(0);
                Snd_seq_play_ck(1);
                Snd_seq_play_ck(2);
                G.Unk2488 = 0;
                Main.Unk194 = 0;
            }
        }
        Scheduler();
        pMain->Unk198 = pG->Status_flg;
        pG->Status_flg &= 0xFF3FFFFF;
        if (G.System_flg & 0x800000) {
            Fade_out_reset();
            continue;
        }
        Snd_se_call();
        Cd_system_control();
        System_trans_all();
        Swap_Cbuff();
        Rnd();
    }
}

/* RE2 Swap_Cbuff: waits for the frame, swaps the draw/display buffers and starts drawing */
INCLUDE_ASM("asm/nonmatchings/game/main", Swap_Cbuff);

/* inverts the colours of the displayed frame in VRAM (Swap_Cbuff, while D_800C703C is set) */
void Scrn_negate(void)
{
    RECT Rect;
    tagMAIN *pM;
    ULONG *p;
    ULONG i;
    ULONG j;
    LONG y;

    pM = &Main;
    pM->Vloop = 4;
    if (G.Unk20FC & 0x20) {
        D_800CA720 = 0;
    }
    if (!(pM->Unk198 & 0x400000)) {
        y = pM->pDb->Draw.ofs[1];
        if (y == 0) {
            y = 240;
        } else {
            y = 0;
        }
        Rect.y = y;
        Rect.x = 0;
        Rect.w = 320;
        Rect.h = 120;
        for (i = 0; i < 2; i++) {
            DrawSync(0);
            StoreImage(&Rect, (ULONG *)0x801C1814);
            p = (ULONG *)0x801C1814;
            for (j = 0; j < 0x4B00; j++) {
                *p = ~*p;
                p++;
            }
            LoadImage(&Rect, (ULONG *)0x801C1814);
            Rect.y += 120;
        }
    }
}

/* RE2 Init_system: libraries, pads, CD, sound, card events, the first task */
void Init_system(void)
{
    tagMAIN *pM;
    tagGLOBAL *pG;

    pM = &Main;
    pG = &G;
    ResetCallback();
    SetGraphDebug(0);
    InitGeom();
    Cd_init();
    Snd_sys_init();
    Init_scheduler();
    VSyncCallback(func_800101AC);
    pM->Vloop = 1;
    D_800D454C = 0;
    SetDefDrawEnv(&pM->Db[0].Draw, 0, 0, 320, 240);
    SetDefDrawEnv(&pM->Db[1].Draw, 0, 240, 320, 240);
    SetDefDispEnv(&pM->Db[0].Disp, 0, 240, 320, 240);
    SetDefDispEnv(&pM->Db[1].Disp, 0, 0, 320, 240);
    pM->Db[1].Draw.dfe = 1;
    pM->Db[0].Draw.dfe = 1;
    pM->Db[1].Draw.dtd = 0;
    pM->Db[0].Draw.dtd = 0;
    pM->Rect.x = 0;
    pM->Rect.y = 0;
    pM->Rect.w = 320;
    pM->Rect.h = 240;
    InitCARD(0);
    StartCARD();
    _bu_init();
    PadInitDirect(pG->Padd_data[0], pG->Padd_data[1]);
    PadStartCom();
    Game.Unk4F = 0;
    Init_global();
    Moji_init();
    Line_work_init();
    pG->Unk2114 = 0;
    Task_execute(0, Init_main);
    ResetGraph(0);
    Vram_clr(0, 0, 0x400, 0x200);
    D_800D454C = 0;
}

/* RE2 Init_global: clears G (keeping Keep_data across a retry) and sets up the curtain tiles */
void Init_global(void)
{
    tagGLOBAL *pG;
    TILE *pT;
    ULONG i;
    ULONG j;

    pG = &G;
    if (pG->System_flg & 0x80) {
        Keep_data_save();
    }
    Memclr(&pG->Unk2468, 0x5A90);
    if (pG->System_flg & 0x80) {
        Keep_data_load();
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            pT = &Main.Db[i].Curtain[j];
            SetTile(pT);
            pT->x0 = 0;
            pT->y0 = j * 216;
            pT->w = 320;
            pT->h = 24;
            setSemiTrans(pT, 1);
        }
        SetDrawTPage(&Main.Db[i].Curtain_Dm, 0, 0, GetTPage(0, 2, 0, 0));
    }
    G.Unk246C = 0;
    *(ULONG *)pG->Padd_data[0] = 0xFFFF0000;
    *(ULONG *)pG->Padd_data[1] = 0xFFFF0000;
    pG->Room_no_old = -1;
    pG->Cut_no_old = -1;
    Moji.Mess_flg = 0;
    Main.Unk194 = 0;
    G.System_flg &= 0x80;
    D_80098700 = 0xD2706CA4;
    for (i = 0; i < 4; i++) {
        Fade_off(i);
    }
    for (i = 0; i < 2; i++) {
        pT = &Main.Db[i].Pause;
        SetTile(pT);
        SetSemiTrans(pT, 1);
        pT->w = 320;
        pT->r0 = 0;
        pT->g0 = 0;
        pT->b0 = 0;
        pT->x0 = 0;
        pT->y0 = 0;
        pT->h = 240;
        SetDrawTPage(&Main.Db[i].Pause_Dm, 0, 0, 0);
    }
}

/* RE2 Init_main: double buffers, ordering tables and the main task */
void Init_main(void)
{
    tagMAIN *pMain;
    DBUF *pDb;
    LONG i;

    pMain = &Main;
    Card_event_set();
    Cd_read(0x63, (ULONG *)0x8011A000, 1, "TEX_TIM");
    Mapping_tpage_clut = 0x1C;
    Mapping_tim((ULONG *)0x8011A000);
    for (i = 0; i < 2; i++) {
        pDb = &pMain->Db[i];
        SetTile(&pDb->Bg_clr);
        *(ULONG *)&pDb->Bg_clr.r0 = 0x440000;
        *(ULONG *)&pDb->Bg_clr.x0 = 0;
        *(ULONG *)&pDb->Bg_clr.w = 0xF00140;
    }
    pMain->Bg_mode = 1;
    pMain->Unk196 = 0;
    if (PadGetState(0) != 0) {
        while (PadGetState(0) != 2 && PadSetMainMode(0, 1, 2) == 0) {
        }
    }
    DrawSync(0);
    SetDispMask(1);
    Bg_set_mode(1, 0);
    Main.Unk193 = 0;
    Task_lexecute(1, 1);
    while (Main.Unk193 == 0) {
        Task_sleep(1);
    }
    G.System_flg |= 0x20000000;
    Task_lchain(0);
}

/* empty */
void Main_dummy(void) {
}

/* RE2 System_trans role: fades, curtain, messages per frame */
void System_trans_all(void)
{
    Fade_trans();
    Curtain_trans();
    Moji_trans_main();
}

/* cinema-scope bars: fades them in or out with STAT_CINESCO and draws them */
void Curtain_trans(void)
{
    tagMAIN *pMain = &Main;
    DBUF *pDb = pMain->pDb;

    if (G.Status_flg & STAT_CINESCO) {
        if (pMain->Curtain_kido < 0xF0) {
            pMain->Curtain_kido += 0x10;
        } else {
            pMain->Curtain_kido = 0xFF;
        }
    } else if (pMain->Curtain_kido >= 0x10) {
        pMain->Curtain_kido -= 0x10;
    } else {
        pMain->Curtain_kido = 0;
    }
    pDb->Curtain[0].r0 = pDb->Curtain[0].g0 = pDb->Curtain[0].b0 =
        pDb->Curtain[1].r0 = pDb->Curtain[1].g0 = pDb->Curtain[1].b0 = pMain->Curtain_kido;
    if (!(G.System_flg & SYS_DOOR_INIT) && pMain->Curtain_kido != 0) {
        AddPrim(G.pOt0 + 4, &pDb->Curtain[0]);
        AddPrim(G.pOt0 + 4, &pDb->Curtain[1]);
        AddPrim(G.pOt0 + 4, &pDb->Curtain_Dm);
    }
}

/* RE2 Bg_draw: draws the room background (or the plain colour of Bg_mode) */
void Bg_draw(void)
{
    RECT Rect;
    volatile UCHAR pad_save0;
    volatile UCHAR pad_save1;
    tagGLOBAL *pG;
    USHORT *pBs_size;
    ULONG Size;
    USHORT Ctpage_save;
    LONG i;
    SHORT n;

    pad_save0 = PadActData[0];
    pad_save1 = PadActData[1];
    PadActData[1] = 0;
    PadActData[0] = 0;
    pG = &G;
    Ctpage_save = pG->Ctp.Tpage_clut;
    if (Main.Unk18E != 2) {
        n = pG->Next_cut_no;
        pG->Room_no_old = pG->Cut_no;
        pG->Cut_no = n;
        pG->Cccut = Ccut_search(n);
        if (!(pG->System_flg & 0x80000000)) {
            Psp_set();
        }
        pBs_size = (USHORT *)(pG->Unk7840 * 2 + (ULONG)pG->pBs_size);
        Size = pG->pUnk2128[*pBs_size + pG->Cut_no];
        CD.Check_sum_flg = 2;
        CD.Fsum = pG->pBs_check_sum[*pBs_size + pG->Cut_no];
        CD.Fsize = Size;
        CD.Fno = D_8009DFD0[pG->Unk783E][pG->Unk7840 + 1] - 1;
        CD.Fsector = (floc[CD.Fno].sec_high << 16) + floc[CD.Fno].sector + pG->Cut_no * 32;
        Cd_read(CD.Fno, (ULONG *)0x80194000, 2, "BS");
        CD.Check_sum_flg = 0;
        if (*(ULONG *)(0x80193FFC + Size) != 0) {
            func_80010000(*(ULONG *)(0x80193FF8 + Size) + 0x80194000, (ULONG *)0x801AC000);
            pG->Ctp.Tpage_clut = 0x15;
            Mapping_tim((ULONG *)0x801AC000);
            pG->Ctp.Tpage_clut = Ctpage_save;
        }
    }
    DecDCTReset(0);
    DecDCToutCallback(0);
    DecDCTvlc((ULONG *)0x80194000, (ULONG *)0x801AA000);
    DecDCTin((ULONG *)0x801AA000, 0);
    DecDCTout((ULONG *)0x8019C000, 0x780);
    Rect.x = 0;
    Rect.y = Main.Rect.y = Main.pDb->Draw.ofs[1];
    Rect.w = 16;
    Rect.h = 240;
    SetGeomScreen(pG->pRoom->pRcut[pG->Cut_no].ViewR >> 7);
    Set_view(pG->pRoom->pRcut[pG->Cut_no].View_p, pG->pRoom->pRcut[pG->Cut_no].View_r);
    Sce.Cut = pG->pRoom->pRcut[pG->Cut_no];
    if (G.Status_flg & 0x1000) {
        func_80048FF4();
    }
    Set_light_default();
    DecDCToutSync(0);
    for (i = 1; i < 20;) {
        DecDCTout((ULONG *)0x8019C000 + (i % 2) * 0x780, 0x780);
        LoadImage(&Rect, (ULONG *)0x8019C000 + ((i - 1) % 2) * 0x780);
        i++;
        Rect.x += 16;
        DecDCToutSync(0);
    }
    LoadImage(&Rect, (ULONG *)0x8019DE00);
    StoreImage(&Main.Rect, (ULONG *)0x8019C000);
    PadActData[0] = pad_save0;
    PadActData[1] = pad_save1;
}

/* soft reset: clears the screen, stops the tasks and sound, clears G and restarts task 0 */
void Soft_reset(void)
{
    RECT rect;

    Pad_vib_init();
    Sce_work_clr_set();
    rect.w = 0x140;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1E0;
    DrawSync(0);
    VSync(0);
    ResetCallback();
    ClearImage(&rect, 0, 0, 0);
    Bg_set_mode(1, 0);
    Task_kill(0);
    Task_kill(1);
    Task_kill(2);
    Snd_reset();
    Init_global();
    VSync(0);
    G.Unk20F8 = -1;
    Task_lexecute(0, 0);
}

/* draws "Paused" over the screen */
void Pause_disp(void)
{
    tagGLOBAL *pG;
    tagMAIN *pMain;

    Print_color(0xFF, 0, 0x100);
    Print14(0x87, 0x6B, "Paused");
    pG = &G;
    pMain = &Main;
    AddPrim(pG->pOt0 + 1, &pMain->pDb->Pause);
    AddPrim(pG->pOt0 + 1, &pMain->pDb->Pause_Dm);
}

/* RE2 Bg_set_mode: background mode and colour */
void Bg_set_mode(ULONG Mode, ULONG Rgb)
{
    Main.Bg_mode = Mode;
    if (Mode == 1) {
        *(ULONG *)&Main.Db[1].Bg_clr.r0 = Rgb;
        *(ULONG *)&Main.Db[0].Bg_clr.r0 = Rgb;
    }
}

/* RE2 Fade_set: starts screen fade No from Rgb0 to Rgb1 over Cnt_max frames */
void Fade_set(ULONG No, UCHAR Pri, ULONG Flg, ULONG Abr, ULONG Rgb0, ULONG Rgb1, USHORT Cnt_max)
{
    FADE *pF;
    USHORT tpage;

    pF = &Main.Fade[No];
    pF->Rgb0[0] = Rgb0;
    pF->Rgb0[1] = Rgb0 >> 8;
    pF->Rgb0[2] = Rgb0 >> 16;
    pF->Rgb1[0] = Rgb1;
    pF->Rgb1[1] = Rgb1 >> 8;
    pF->Rgb1[2] = Rgb1 >> 16;
    tpage = GetTPage(0, Abr, 0, 0);
    SetDrawTPage(&pF->Tpage[0], 1, 0, tpage);
    SetDrawTPage(&pF->Tpage[1], 1, 0, tpage);
    Flg |= FADE_ON;
    pF->Rect.w = 0x140;
    pF->Pri = Pri;
    pF->Flg = Flg;
    pF->Cnt_max = Cnt_max;
    pF->Cnt = 0;
    pF->Rect.x = 0;
    pF->Rect.y = 0;
    pF->Rect.h = 0xF0;
}

/* sets the rectangle of fade No */
void Fade_rect_set(ULONG No, RECT *Rect)
{
    FADE *pF = &Main.Fade[No];

    pF->Rect.x = Rect->x;
    pF->Rect.y = Rect->y;
    pF->Rect.w = Rect->w;
    pF->Rect.h = Rect->h;
}

/* per frame: advances and draws the four screen fades */
void Fade_trans(void)
{
    FADE *pF;
    TILE *pT;
    UCHAR r, g, b;
    ULONG *pOt;
    FADE *pEnd;

    pF = Main.Fade;
    pEnd = &Main.Fade[4];
    do {
        if (pF->Flg & FADE_ON) {
            pT = &pF->T[G.Cbuf_id];
            if (pF->Cnt_max == pF->Cnt) {
                if (!(pF->Flg & FADE_HOLD)) {
                    pF->Flg = 0;
                } else {
                    pF->Flg |= FADE_END;
                }
                r = pF->Rgb1[0];
                g = pF->Rgb1[1];
                b = pF->Rgb1[2];
            } else {
                pF->Cnt++;
                r = pF->Rgb0[0];
                r += (pF->Rgb1[0] - r) * pF->Cnt / pF->Cnt_max;
                g = pF->Rgb0[1];
                g += (pF->Rgb1[1] - g) * pF->Cnt / pF->Cnt_max;
                b = pF->Rgb0[2];
                b += (pF->Rgb1[2] - b) * pF->Cnt / pF->Cnt_max;
            }
            *(ULONG *)&pT->r0 = 0x62000000 | (b << 16) | (g << 8) | r; /* semi-transparent tile */
            *(ULONG *)pT = 0x03000000;
            pT->x0 = pF->Rect.x;
            pT->y0 = pF->Rect.y;
            pT->w = pF->Rect.w;
            pT->h = pF->Rect.h;
            if (pF == &Main.Fade[3]) {
                pOt = G.pOt2 + pF->Pri;
            } else {
                pOt = G.pOt0 + pF->Pri;
            }
            AddPrim(pOt, pT);
            AddPrim(pOt, &pF->Tpage[G.Cbuf_id]);
        }
        pF++;
    } while (pF < pEnd);
}

/* RE2 Fade_off */
void Fade_off(ULONG No)
{
    Main.Fade[No].Flg = 0;
}

/* RE2 Fade_status: FADE_* flags of fade No */
ULONG Fade_status(ULONG No)
{
    return Main.Fade[No].Flg;
}

/* fade to grey before a reset (System_flg SYS_FADE_OUT) */
void Fade_out_reset(void)
{
    tagGLOBAL *pG = &G;
    FADE *pF = &Main.Fade[0];
    TILE *pT = &pF->T[pG->Cbuf_id];

    switch (pF->Flg) {
    case 0:
        DrawSync(0);
        PutDispEnv(&Main.pDb->Disp);
        *(ULONG *)&pT->tag = 0x3000000;
        *(ULONG *)&pT->r0 = 0x62202020;
        *(ULONG *)&pT->x0 = 0;
        *(ULONG *)&pT->w = 0xF00140;
        SetDrawTPage(&pF->Tpage[pG->Cbuf_id], 1, 0, GetTPage(0, 2, 0, 0));
        pF->Cnt_max = 8;
        pF->Flg = 1;
    case 1:
        AddPrim(pG->pOt0, pT);
        AddPrim(pG->pOt0, &pF->Tpage[pG->Cbuf_id]);
        DrawSync(0);
        VSync(0);
        DrawOTag(pG->pOt0 + 7);
        DrawSync(0);
        if (--pF->Cnt_max == 0) {
            pF->Flg = 0;
            G.System_flg &= ~SYS_FADE_OUT;
        }
        break;
    }
}

/* RE2 Cut_check: camera switch zones the player is in */
ULONG Cut_check(ULONG Flg)
{
    tagGLOBAL *pG = &G;
    VCUT *cp;
    ULONG next_cut;

    for (cp = pG->Cccut_next + 1; cp->Fcut == pG->Next_cut_no; cp++) {
        if (cp->Be_flg != 0 && (cp->nFloor == -0x80 || cp->nFloor == pG->Player.nFloor) &&
            Hit_ck_point4(&pG->Player.Pos_x, cp) != 0) {
            next_cut = cp->Tcut;
            goto apply_cut;
        }
    }
    if (Flg == 0) {
        return 0;
    }
    next_cut = pG->Next_cut_no;
apply_cut:
    Cut_change(next_cut);
    return 1;
}

/* RE2 Cut_change: requests camera cut Fc */
void Cut_change(ULONG Fc)
{
    G.Next_cut_no = Fc;
    G.Cccut_next = Ccut_search(Fc);
}

/* RE2 Ccut_search: first camera switch zone of cut Fc */
VCUT *Ccut_search(ULONG Fc)
{
    VCUT *pCcut;

    pCcut = G.pRoom->pVcut;
    while (pCcut->Fcut != Fc) {
        pCcut++;
    }
    return pCcut;
}

/* RE2 Card_event_set: opens the memory card events */
void Card_event_set(void)
{
    EnterCriticalSection();
    Ev0 = OpenEvent(0xF4000001, 4, 0x2000, NULL);
    Ev1 = OpenEvent(0xF4000001, 0x100, 0x2000, NULL);
    Ev2 = OpenEvent(0xF4000001, 0x2000, 0x2000, NULL);
    Ev3 = OpenEvent(0xF4000001, 0x8000, 0x2000, NULL);
    Ev10 = OpenEvent(0xF0000011, 4, 0x2000, NULL);
    Ev11 = OpenEvent(0xF0000011, 0x100, 0x2000, NULL);
    Ev13 = OpenEvent(0xF0000011, 0x8000, 0x2000, NULL);
    ExitCriticalSection();
    EnableEvent(Ev0);
    EnableEvent(Ev1);
    EnableEvent(Ev2);
    EnableEvent(Ev3);
    EnableEvent(Ev10);
    EnableEvent(Ev11);
    EnableEvent(Ev13);
}
