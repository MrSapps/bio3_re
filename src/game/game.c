#include "monitor.h"
#include "pad.h"
#include "trans.h"
#include "sub.h"
#include "game.h"
#include "door.h"
#include "player.h"
#include "room.h"
#include "sld.h"
#include "snd.h"
#include "spl.h"
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

INCLUDE_ASM("asm/nonmatchings/game/game", Game_loop);

void Gun_light_set(void)
{
    LIGHT_CUT_DATA *pLight;
    SVECTOR v;

    pLight = Get_light_cut(&G.Player.Pos_x);
    Memcpy(Game.Plight_bak, pLight, 0x28);
    pLight->Mode[2] = 0;
    pLight->L[2] = 1;
    v.vx = 0x4B0;
    v.vz = 0;
    Rot_vector(G.Player.Cdir_y, &v, &v);
    Game.Plight_col[0] = 0xFF;
    Game.Plight_col[1] = 0xE6;
    Game.Plight_col[2] = 0xA0;
    Game.Plight_pos.vx = G.Player.Pos_x + v.vx;
    Game.Plight_pos.vy = G.Player.Pos_y - 0x960;
    Game.Plight_pos.vz = G.Player.Pos_z + v.vz;
    switch (Game.Plight_cnt) {
    case 0:
        {
            UCHAR Col_lvl = 0xFF;

            if (pLight->Col[2][0] < Col_lvl) {
                pLight->Col[2][0] = Col_lvl;
            }
        }
        if (pLight->Col[2][1] < Game.Plight_col[1]) {
            pLight->Col[2][1] = Game.Plight_col[1];
        }
        if (pLight->Col[2][2] < Game.Plight_col[2]) {
            pLight->Col[2][2] = Game.Plight_col[2];
        }
        Game.Plight_cnt++;
        break;
    case 1:
        {
            UCHAR Col_lvl = 0x7F;

            if (pLight->Col[2][0] < Col_lvl) {
                pLight->Col[2][0] = Col_lvl;
            }
        }
        if (pLight->Col[2][1] < Game.Plight_col[1] >> 1) {
            pLight->Col[2][1] = Game.Plight_col[1] >> 1;
        }
        if (pLight->Col[2][2] < Game.Plight_col[2] >> 1) {
            pLight->Col[2][2] = Game.Plight_col[2] >> 1;
        }
        Game.Plight_cnt++;
        break;
    }
    pLight->Pos[2][0] = Game.Plight_pos.vx;
    pLight->Pos[2][1] = Game.Plight_pos.vy;
    pLight->Pos[2][2] = Game.Plight_pos.vz;
    pLight->L[2] = 6000;
}

void Gun_light_reset(void)
{
    Memcpy(Get_light_cut(&G.Player.Pos_x), Game.Plight_bak, 0x28);
    if (Game.Plight_cnt >= 2) {
        Game.Plight_flg = 0;
        Game.Plight_cnt = 0;
    }
}

/* game start: place the player (new game, demo or loaded save), load the stage and room */
void func_800245A0(void)
{
    tagGLOBAL *pG = &G;
    tagMAIN *pMain = &Main;
    ENEMY_WORK *pPl;
    ULONG Be;

    Bg_set_mode(1, 0);
    Pad_rep_set(0, 0);
    pG->pEnemy[0] = G_PLAYER;
    pPl = G_PLAYER;
    if (pG->System_flg & 0x1000) {
        if (pG->System_flg & 0x80) {
            pPl->Max_life = pPl->Life = 200;
            pPl->Pos_x = pPl->Old_x = -18415;
            pPl->Pos_z = pPl->Old_z = -22736;
            pPl->Cdir_y = 0x4A8;
            pPl->Pos_y = pPl->Old_y = 0;
            pPl->Unk122 = 0;
            pPl->nFloor = 0;
            pG->Unk783E = 6;
            pG->Unk7840 = 12;
            pG->Cut_no = 2;
        } else {
            pPl->Max_life = pPl->Life = 200;
            pPl->Pos_x = pPl->Old_x = 13262;
            pPl->Pos_z = pPl->Old_z = -12537;
            pPl->Cdir_y = 0x720;
            pPl->Pos_y = pPl->Old_y = 0;
            pPl->Unk122 = 0;
            pPl->nFloor = 0;
            pPl->Status_flg = 0;
            pG->Unk7840 = 13;
            pG->Unk783E = 0;
            pG->Cut_no = 11;
            pG->Unk7864 = 0;
            pG->Unk784E = 0;
        }
    } else if (pG->System_flg & 0x10000000) {
        func_80024B34();
        pG->pItem_set = &G.Item_work[G.Item_work_no];
        goto set;
    } else {
        pPl->Max_life = 200;
        pPl->Old_x = pPl->Pos_x;
        pPl->Old_y = pPl->Pos_y;
        pPl->Old_z = pPl->Pos_z;
        pPl->Unk122 = pPl->Pos_y;
        pPl->nFloor = (SHORT)pPl->Pos_y / -1800;
    }
set:
    pPl->Be_flg = 1;
    pG->Sub_work.Col.Id = 0;
    Esp_init_C();
    Trans_work_init();
    St_tex_load();
    Em_move_tbl_set();
    Spl_set();
    Set_stage();
    Snd_bgm_ck();
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
    }
    Set_room();
    Snd_bgm_play_ck();
    pG->pEm = PLAYER_EM(pG);
    Player_set(pG->pEm);
    SetBackColor(0x80, 0x80, 0x80);
    pMain->Unk191 = 0;
    Bg_set_mode(2, 0);
    pMain->Unk18E = 1;
    SetGeomOffset(160, 120);
    while (Main.Unk193) {
        Task_sleep(1);
    }
    pG->Game_time_offset = VSync(-1);
    Ctrl_mode = 2;
    Game.Unk1 = 0;
    G.System_flg |= 0x8000000;
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
        Game.Unk4E = 1;
    }
    Task_sleep(1);
}

/* door AOT (RE2 SCE_AOT_DOOR next position part) */
typedef struct tagDOOR_NEXT {
    SHORT Next_pos_x;                  /* 0x0 */
    SHORT Next_pos_y;                  /* 0x2 */
    SHORT Next_pos_z;                  /* 0x4 */
    SHORT Next_cdir_y;                 /* 0x6 */
    UCHAR Next_stage;                  /* 0x8 */
    UCHAR Next_room;                   /* 0x9 */
    UCHAR Next_cut;                    /* 0xA */
    UCHAR Next_nfloor;                 /* 0xB */
} DOOR_NEXT;

void Set_door(void)
{
    tagGLOBAL *pG;
    DOOR_NEXT *pAot;
    UCHAR n;

    pG = &G;
    pG->Unk2470 = 0;
    if (pG->Status_flg & 0x40000000) {
        func_80024C58();
    }
    pG->Player.Damage_cnt = 0;
    pG->Player.Be_flg &= 0x9F4F;
    pG->Player.Type &= 0xBEFF;
    pG->System_flg |= 0x400000;
    Bg_set_mode(1, 0);
    Task_execute(1, Door_main);
    Task_sleep(1);
    Door_snd_trans();
    pAot = (DOOR_NEXT *)pG->pSce_aot_on;
    pG->Player.Pos_x = pG->Player.Old_x = pAot->Next_pos_x;
    pG->Player.Unk122 = pG->Player.Pos_y = pG->Player.Old_y = pAot->Next_pos_y;
    pG->Player.Pos_z = pG->Player.Old_z = pAot->Next_pos_z;
    pG->Player.Cdir_y = pAot->Next_cdir_y;
    pG->Player.nFloor = pAot->Next_nfloor;
    n = pAot->Next_cut;
    pG->Next_cut_no = n;
    pG->Cut_no_old = pG->Unk7840 + (pG->Unk783E + 1) * 0x100;
    pG->Cut_no = n;
    pG->Unk7840 = pAot->Next_room;
    if (pG->Unk783E != pAot->Next_stage % 9) {
        pG->Unk783E = pAot->Next_stage % 9;
        Set_stage();
    }
    Snd_bgm_ck();
    while (pG->Status_flg & 0x800) {
        Task_sleep(1);
    }
    Set_room();
    while (G.System_flg & 0x400000) {
        Task_sleep(1);
    }
    Snd_bgm_play_ck();
    Bg_set_mode(2, 0);
    Cut_check(1);
    Main.Unk18E = 1;
    if (G.System_flg & 0x10000000) {
        D_80098700 = Game.pSave->Unk714;
        G.Unk784A = Game.pSave->Unk718;
        D_800E0186[0] = 0;
        Game.Unk4E = 1;
    }
    Task_sleep(1);
}

void func_80024B34(void)
{
    ENEMY_WORK *pPl;
    SAVE_POS *pSave;
    tagMAIN *pMain;
    tagGAME *pGame;
    tagGLOBAL *pG;

    pG = &G;
    pMain = &Main;
    pGame = &Game;
    pPl = PLAYER_EM(pG);
    pSave = pGame->pSave;
    Memcpy(&pG->Game_time, pSave, 0x708);
    pPl->Pos_x = pPl->Old_x = pSave->Pos_x;
    pPl->Pos_y = pPl->Old_y = pSave->Pos_y;
    pPl->Pos_z = pPl->Old_z = pPl->Unk122 = pSave->Pos_z;
    pPl->nFloor = pSave->Pos_y / -1800;
    pPl->Cdir_y = pSave->Cdir_y;
    pPl->Life = pSave->Life;
    pPl->Id = pSave->Id;
    pGame->Unk4D = pMain->Unk196;
    pMain->Unk196 = 0;
    pGame->Unk4C = pMain->Unk195;
    pMain->Unk195 = pSave->Unk713;
}

void func_80024C50(void) {
}

void func_80024C58(void)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;
    LONG i;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        if ((ULONG)(pEm->Id - 0x34) < 3) {
            G.Unk7856 = pEm->Life;
            G.Unk7854 = pEm->Type & 1;
            for (i = 0; (ULONG)i < 12; i++) {
                G.Unk7D90[i] = pEm->Unk1CC[i];
            }
        }
    }
}

void func_80024CE8(void)
{
    ULONG i;

    G.System_flg |= 0x400;
    Ctrl_mode = 3;
    G.Status_flg &= ~4;
    G.Status_flg &= ~2;
    G.Status_flg &= ~1;
    Pad_vib_init();
    Flash_fade_in();
    Fade_set(2, 0, 0, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Task_sleep(1);
    D_800C703C.Flg = 1;
    Flash_fade_out();
    Flash_fade_in();
    Mess_set(0, 0x2001, D_800C703C.Mess_no, 0);
    D_800C703C.Unk3 = 0x62;
    D_800C703C.Cnt = 0;
    Flash_fade_out();
    Flash_fade_in();
    Fade_set(2, 0, 0, 1, 0xFFFFFF, 0xFFFFFF, 1);
    D_800C703C.Unk3 = 0xB3;
    Task_sleep(1);
    D_800C703C.Flg = 0;
    Fade_set(2, 0, 0, 1, 0xFFFFFF, 0, 4);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
    D_800C703C.Unk1 = 1;
    Ctrl_mode = 2;
    G.System_flg &= ~0x400;
    if (!func_80024F74() && !func_80024F74()) {
        D_800C703C.Cnt++;
        while (G.Status_flg & 0x10000000) {
            Task_sleep(1);
        }
    }
    G.System_flg |= 0x400;
    D_800C703C.Flg = 1;
    func_8003884C(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    i = 7;
    do {
        Task_sleep(1);
    } while (i-- != 0);
    D_800C703C.Flg = 0;
    Ctrl_mode = 2;
    D_800C703C.Unk1 = 0;
    G.System_flg &= ~0x400;
    Task_exit();
}

ULONG func_80024F74(void)
{
    ULONG i;

    D_800C703C.Cnt++;
    if (Main.Unk18E == 0) {
        do {
            Task_sleep(1);
            if (!(G.Status_flg & 0x10000000)) {
                return 1;
            }
        } while (Main.Unk18E == 0);
    }
    Main.Unk18E = 0;
    D_800C703C.Flg = 1;
    G.System_flg |= 0x400;
    func_8003884C(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    i = 7;
    do {
        Task_sleep(1);
        if (!(G.Status_flg & 0x10000000)) {
            return 1;
        }
    } while (i-- != 0);
    Main.Unk18E = 1;
    G.System_flg &= ~0x400;
    Task_sleep(1);
    if (!(G.Status_flg & 0x10000000)) {
        return 1;
    }
    D_800C703C.Flg = 0;
    return 0;
}

void Flash_fade_in(void)
{
    func_8003884C(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    Fade_set(2, 0, 0, 1, 0, 0xFFFFFF, 4);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
}

void Flash_fade_out(void)
{
    ULONG i;

    Fade_set(2, 0, 0, 2, 0xFFFFFF, 0, 3);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
    Fade_set(2, 0, FADE_HOLD, 1, 0, 0x606060, 1);
    i = 0x22;
    do {
        Task_sleep(1);
    } while (i-- != 0);
}
