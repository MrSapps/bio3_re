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
#include "cd.h"
#include "sce_at.h"
#include "joint.h"
#include "pl_wep.h"
#include "om.h"
#include "psp.h"
#include "line_scr.h"
#include "80048520.h"


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

/* RE2 Game_loop: game task: per frame player, enemies, scenario, drawing, room changes */
/* Game_loop passes the position as a 4th argument (joint.c defines it with 3) */
void Joint_trans_4(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg, VECTOR *pPos) __asm__("Joint_trans");
void func_8011A03C(void);
extern UCHAR D_8011B4D8;
extern ULONG D_800C7038;

/* Mercenaries timer display */
typedef struct tagGAME_TIMER {
    UCHAR Col;                         /* 0x0 */
    UCHAR pad1;
    SHORT Cnt;                         /* 0x2 */
    SHORT Sec;                         /* 0x4 */
    UCHAR pad6[2];
    ULONG Status_bak;                  /* 0x8 */
} GAME_TIMER;
extern GAME_TIMER D_800C79B8;

void Game_loop(void)
{
    tagGLOBAL *pG;
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;
    VECTOR *pPos;
    LONG Px, Pz, x, z;
    SHORT Min;
    ULONG i;
    ULONG Clr0, Clr1, Clr2;

    pG = &G;
    Game_init();
    goto first;
    do {
        if (Main.Unk194 != 0) goto render;
        if (G.System_flg & 0x200) {
            if (D_800C79B8.Cnt == 0x10) {
                if (G.Status_flg & 0x20) goto render;
                Task_lexecute(1, 3);
                Moji.Mess_flg = 0;
                D_800C79B8.Cnt = -30000;
                G.System_flg |= 0x40;
            } else {
                D_800C79B8.Cnt++;
            }
            goto render;
        }
        if (Game.Doordemo_flg == 1 && !(Moji.Mess_flg & 0x80) && Fade_status(0) == 0
                && !(G.Status_flg & 0x10000000) && !(G.System_flg & 0x400)) {
            G.Stop_flg |= 0xFF000000;
            if (G.pSce_aot_on != NULL) {
                G.System_flg |= 0x800000;
                Fade_off(0);
            }
            G.System_flg |= 0x1000000;
            Bg_set_mode(1, 0);
            pG->Stop_flg = 0;
            D_800C7038 = 0;
            Set_door();
first:
            if (G.System_flg & 0x10000000) {
                G_random = Game.pSave->Unk714;
                G.Sce_random = Game.pSave->Unk718;
                Sce_rnd_idx[0] = 0;
            }
            pG->pEm = PLAYER_EM(pG);
            *(ULONG *)&pG->Player.Routine_0 = 0;
            Pl_wep_eff_init(pG->pEm);
            Player_move(PLAYER_EM(pG));
            G.System_flg &= ~0x1000000;
            Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 6);
            Game.Doordemo_flg = 0;
        }
        switch (Game.Unk1) {
        case 1:
            if ((Moji.Mess_flg & 0x80) || Fade_status(0) != 0
                    || (G.Status_flg & 0x10000000) || (G.System_flg & 0x400)) {
                break;
            }
            G.Stop_flg |= 0xFF000000;
            Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 6);
            Game.Unk1 = 2;
        case 2:
            if (!(Fade_status(0) & FADE_END)) break;
            Task_sleep(2);
            Main.Fade[0].Flg = 0;
            pG->Stop_flg = 0;
            D_800C7038 = 0;
            G.System_flg |= 0x1000000;
            if (G.Status_flg & 0x200) {
                Task_execute(1, Status);
                Task_sleep(1);
                G.Status_flg &= ~0x200;
                D_800C7038 = Game.Stop_bak;
                Game.Stop_bak = 0;
            }
            if (G.System_flg & 0x100000) {
                G.Ovl_mode = 0;
                Task_lexecute(1, 4);
                Task_sleep(1);
                G.System_flg &= ~0x100000;
            }
            if (G.System_flg & 0x80000) {
                Task_lexecute(1, 2);
                D_800C7038 = Game.Stop_bak;
                Game.Stop_bak = 0;
                Task_sleep(1);
                G.System_flg &= ~0x80000;
                G.System_flg &= ~0x40000;
            }
            pG->pEm = PLAYER_EM(pG);
            *(ULONG *)&pG->Player.Routine_0 = 0;
            Player_move(PLAYER_EM(pG));
            Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 6);
            Game.Unk1 = 3;
            pG->Stop_flg = D_800C7038;
            G.System_flg &= ~0x1000000;
            if (G.Stop_flg & 0x1000000) {
                pG->Key &= 0x3000;
                pG->Key_trg &= 0x3000;
            }
            break;
        case 3:
            if (Fade_status(0) == 0) {
                Game.Unk1 = 0;
            }
            break;
        }
        if (G.Unk246C & 0x40000000) goto render;
        if (G.Status_flg & 0x2000000) {
            if (!(D_800C79B8.Status_bak & 0x2000000)) {
                D_8011B4D8 = 0;
            }
            func_8011A03C();
        } else if (G.Status_flg & 0x4000000) {
            if (G.System_flg & 0x80) {
                Bonus_time_disp();
            }
            if (D_800C79B8.Cnt >= 30 || !(D_800C79B8.Status_bak & 0x4000000)) {
                D_800C79B8.Cnt = 0;
                D_800C79B8.Sec = 8;
                pG->Time--;
                if (G.System_flg & 0x80) {
                    G.Unk785A++;
                }
            }
            if ((SHORT)pG->Time < 60) {
                D_800C79B8.Col = 4;
            } else {
                D_800C79B8.Col = 0;
            }
            if ((SHORT)pG->Time >= 0) {
                Print_color(0xFF, 0, 0);
                Min = (SHORT)pG->Time / 60;
                Print14_col(0x6F, 0x20, D_800C79B8.Col, "%2d^", Min);
                Print14_col(0x92, 0x20, D_800C79B8.Col, "%d%d.", (SHORT)((SHORT)((SHORT)pG->Time % 60) / 10), (SHORT)((SHORT)((SHORT)pG->Time % 60) % 10));
                Print14_col(0xB5, 0x20, D_800C79B8.Col, "%d%d", 9 - (SHORT)(D_800C79B8.Cnt / 3), D_800C79B8.Sec);
            } else {
                Print_color(0xFF, 4, 0);
                Print14(0x6F, 0x20, " 0^");
                Print14(0x92, 0x20, "00.");
                Print14(0xB5, 0x20, "00");
            }
            if ((SHORT)pG->Time < 0) {
                if (*(USHORT *)&Game.Doordemo_flg != 0) goto render;
                if (Fade_status(0) != 0) goto render;
                if (Task_status(1) != 0) goto render;
                if (CD.Xa_rtn_no != 0) {
                    CD.Xa_rtn_no = 3;
                }
                D_800C79B8.Cnt = 0;
                D_800C79B8.Sec = 0;
                G.Status_flg &= ~0x4000000;
                G.System_flg |= 0x200;
                goto render;
            }
            if (*(USHORT *)&Game.Doordemo_flg == 0) {
                D_800C79B8.Cnt++;
                D_800C79B8.Sec -= 3;
                if (D_800C79B8.Sec <= 0) {
                    D_800C79B8.Sec = 8;
                }
            }
        }
        D_800C79B8.Status_bak = pG->Status_flg;
        if (G.System_flg & 0x400) goto render;
        if (G.Status_flg & 0x10000000) {
            Ctrl_mode = 2;
            if (Live_sel.Wait != 0 && G.Cbuf_id != 0) goto render;
        }
        if (pG->Front_func != NULL) {
            pG->Front_func();
        }
        if (!(G.Status_flg & 0x80)) {
            Cut_check(0);
        }
        pG->pEm = PLAYER_EM(pG);
        Sce_scheduler();
        if ((G.Status_flg & 0x10000000) && Task_status(1) == 0) {
            Task_execute(1, Live_select);
        }
        if (Task_status(1) == 0 && !(G.System_flg & 0x10000000) && pG->Cut_no == pG->Next_cut_no
                && Main.Unk18E == 0 && pG->Player.Damage_cnt == 0 && *(USHORT *)&Game.Doordemo_flg == 0
                && !(Sce.Skip_state & 0xE) && !(Moji.Mess_flg & 0x80) && !(G.Stop_flg & 0x1000000)
                && !(G.System_flg & 0x30000) && !(G.Status_flg & 0x108)) {
            if (G.Key_trg & 0x4000) {
                G.Status_flg |= 0x200;
                St.St_type = 0;
            }
            if (G.Padd_trg & 1) {
                St.St_type = 4;
                G.Status_flg |= 0x200;
            }
            if ((G.Padd_trg & 0x100) && !(G.Status_flg & 0x200)) {
                Snd_se_stad(6, 0, 0, 0);
                Game.Unk1 = 1;
                G.System_flg |= 0x100000;
                Game.Stop_bak = pG->Stop_flg;
                G.Stop_flg |= 0xFF000000;
                goto render;
            }
            if (G.Status_flg & 0x200) {
                Game.Unk1 = 1;
                G.Status_flg |= 0x100;
                Game.Stop_bak = pG->Stop_flg;
                G.Stop_flg |= 0xFF000000;
                if (St.St_type == 4) {
                    Snd_se_stad(9, 0, 0, 0);
                } else {
                    Snd_se_stad(6, 0, 0, 0);
                }
                goto render;
            }
        }
        SetSpadStack();
        pG->Room_enemy_flg &= 0xFF;
        if (!(G.Stop_flg & 0x20000000)) {
            Clr0 = 0xCFFFFFFF;
            Clr1 = 0x3FFFFFFF;
            Px = pG->Player.Pos_x;
            Pz = pG->Player.Pos_z;
            ppEm = &pG->pEnemy[1];
            if (ppEm != pG->ppEnemy_end) do {
                pEm = *ppEm;
                pG->pEm = pEm;
                pEm->Be_flg &= Clr0;
                if ((pEm->Be_flg & 1) && !(pEm->Type & 0x8000)) {
                    x = (pEm->Pos_x - Px) >> 1;
                    z = (pEm->Pos_z - Pz) >> 1;
                    pEm->L_pl = SquareRoot0(x * x + z * z) << 1;
                    pG->Em_move_tbl[pEm->Id - 0x10](pEm);
                }
                pEm->Be_flg &= Clr1;
                ppEm++;
            } while (ppEm != pG->ppEnemy_end);
        }
        pG->pEm = PLAYER_EM(pG);
        pEm = pG->pEm;
        if (!(G.Stop_flg & 0x80000000) && !(pEm->Type & 0x8000)) {
            pG->Player.Be_flg &= 0xCFFFFFFF;
            Player_move(pEm);
            pG->Player.Be_flg &= 0x3FFFFFFF;
        }
        Oba_main();
        Om_move();
        Sce_at();
        if (pG->Back_func != NULL) {
            pG->Back_func();
        }
        Esp_task_main();
        if ((G.Status_flg & 0x1000) && !(G.Stop_flg & 0x10000000)) {
            Rain_move();
        }
        ResetSpadStack();
        if (G.System_flg & 0x2000) {
            Task_lexecute(1, *(USHORT *)G.pAot_dat + 0xC);
            G.System_flg &= ~0x2000;
            goto post_render;
        }
        if ((G.System_flg & 0x20000) && Task_status(1) == 0 && Main.Unk18E == 0 && G.Cut_no == G.Next_cut_no) {
            Task_lexecute(1, 3);
            G.System_flg &= ~0x20000;
            G.System_flg |= 0x40;
        }
render:
        SetSpadStack();
        Ot_set_all();
        for (ppEm = pG->pEnemy; ppEm != pG->ppEnemy_end; ppEm++) {
            pEm = *ppEm;
            pG->pEm = pEm;
            if (pEm->Be_flg & 1) {
                Joint_trans_4(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, (VECTOR *)&pEm->Pos_x);
                MulKage(pEm->Kage.pKw, pEm->Ground, pEm->Cdir_y, pEm->Be_flg & 0x30);
                if (pEm->Be_flg & 0x2000000) {
                    Sub_matrix_mul((UCHAR *)pEm);
                }
            }
        }
        if (Game.Plight_flg != 0) {
            Gun_light_set();
        }
        Clr2 = 0xF0FFFFFF;
        ppEm = pG->pEnemy;
        pG->pPacket = (POLY_GT4 *)pG->pEm_mem + pG->Cbuf_id;
        for (; ppEm != pG->ppEnemy_end; ppEm++) {
            pEm = *ppEm;
            if (pEm->Be_flg & 1) {
                pPos = (VECTOR *)&pEm->Pos_x;
                Set_light_pos(pPos);
                SetColorMatrix(&COLORMATRIX);
                pG->pEm = pEm;
                Joint_trans2(pEm, pEm->pSin_parts_ptr, pEm->Be_flg, pPos);
                pEm->Be_flg &= Clr2;
            }
        }
        if (Game.Plight_flg != 0) {
            Gun_light_reset();
        }
        Om_trans();
        Eff_sprt_trans();
        if ((LONG)G.System_flg >= 0) {
            Psp_trans();
        }
        Kage_work_sort();
        Kage_work9_sort();
        Pl_wep_poly_trans();
        if (Main.Unk194 == 0 && !(G.Stop_flg & 0x2000000)) {
            Scr_effect();
        }
        if (G_status_flg & 0x1000) {
            Rain_trans();
        }
        ResetSpadStack();
post_render:
        if (G.System_flg & 0x4000) {
            Bg_set_mode(1, 0);
            Task_lchain(9);
        }
        Task_sleep(1);
    } while (G.System_flg & 0x8000000);
    if ((G.System_flg & 0xC0) == 0xC0) {
        G.System_flg &= ~0x40;
        for (i = 0; i < 4; i++) {
            Fade_off(i);
        }
        Fade_set(0, 0, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
        D_800CC84D = 2;
        Task_lexecute(1, 8);
        Task_sleep(1);
        Fade_set(0, 0, 2, 2, 0xFFFFFF, 0xFFFFFF, 1);
        Retry_load();
    }
    G.System_flg |= 0x200000;
    Task_exit();
}

/* RE2 Gun_light_set: muzzle flash light */
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

/* RE2 Gun_light_reset */
void Gun_light_reset(void)
{
    Memcpy(Get_light_cut(&G.Player.Pos_x), Game.Plight_bak, 0x28);
    if (Game.Plight_cnt >= 2) {
        Game.Plight_flg = 0;
        Game.Plight_cnt = 0;
    }
}

/* game start: places the player (new game, Mercenaries, or a loaded save), sets the stage and room */
/* game start: place the player (new game, demo or loaded save), load the stage and room */
void Game_init(void)
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
            pPl->Ground = 0;
            pPl->nFloor = 0;
            pG->Stage_no = 6;
            pG->Room_no = 12;
            pG->Cut_no = 2;
        } else {
            pPl->Max_life = pPl->Life = 200;
            pPl->Pos_x = pPl->Old_x = 13262;
            pPl->Pos_z = pPl->Old_z = -12537;
            pPl->Cdir_y = 0x720;
            pPl->Pos_y = pPl->Old_y = 0;
            pPl->Ground = 0;
            pPl->nFloor = 0;
            pPl->Status_flg = 0;
            pG->Room_no = 13;
            pG->Stage_no = 0;
            pG->Cut_no = 11;
            pG->Pl_id_bak = 0;
            pG->Change_player = 0;
        }
    } else if (pG->System_flg & 0x10000000) {
        Game_save_load();
        pG->pItem_set = &G.Item_work[G.Item_work_no];
        goto set;
    } else {
        pPl->Max_life = 200;
        pPl->Old_x = pPl->Pos_x;
        pPl->Old_y = pPl->Pos_y;
        pPl->Old_z = pPl->Pos_z;
        pPl->Ground = pPl->Pos_y;
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
        G_random = Game.pSave->Unk714;
        G.Sce_random = Game.pSave->Unk718;
        Sce_rnd_idx[0] = 0;
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
        G_random = Game.pSave->Unk714;
        G.Sce_random = Game.pSave->Unk718;
        Sce_rnd_idx[0] = 0;
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

/* RE2 Set_door: next position, stage, room and cut from the door AOT */
void Set_door(void)
{
    tagGLOBAL *pG;
    DOOR_NEXT *pAot;
    UCHAR n;

    pG = &G;
    pG->Ot_sub_num = 0;
    if (pG->Status_flg & 0x40000000) {
        Chaser_data_save();
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
    pG->Player.Ground = pG->Player.Pos_y = pG->Player.Old_y = pAot->Next_pos_y;
    pG->Player.Pos_z = pG->Player.Old_z = pAot->Next_pos_z;
    pG->Player.Cdir_y = pAot->Next_cdir_y;
    pG->Player.nFloor = pAot->Next_nfloor;
    n = pAot->Next_cut;
    pG->Next_cut_no = n;
    pG->Cut_no_old = pG->Room_no + (pG->Stage_no + 1) * 0x100;
    pG->Cut_no = n;
    pG->Room_no = pAot->Next_room;
    if (pG->Stage_no != pAot->Next_stage % 9) {
        pG->Stage_no = pAot->Next_stage % 9;
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
        G_random = Game.pSave->Unk714;
        G.Sce_random = Game.pSave->Unk718;
        Sce_rnd_idx[0] = 0;
        Game.Unk4E = 1;
    }
    Task_sleep(1);
}

/* restores G and the player from a loaded save (System_flg 0x10000000) */
void Game_save_load(void)
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
    pPl->Pos_z = pPl->Old_z = pPl->Ground = pSave->Pos_z;
    pPl->nFloor = pSave->Pos_y / -1800;
    pPl->Cdir_y = pSave->Cdir_y;
    pPl->Life = pSave->Life;
    pPl->Id = pSave->Id;
    pGame->Unk4D = pMain->Unk196;
    pMain->Unk196 = 0;
    pGame->Unk4C = pMain->Unk195;
    pMain->Unk195 = pSave->Unk713;
}

/* empty */
void Game_dummy(void) {
}

/* keeps Nemesis' life, type and free work in G (enemy ids 0x34..0x36) */
void Chaser_data_save(void)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pEm;
    LONG i;

    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        pEm = *ppEm;
        if ((ULONG)(pEm->Id - 0x34) < 3) {
            G.Chaser_life = pEm->Life;
            G.Chaser_type = pEm->Type & 1;
            for (i = 0; (ULONG)i < 12; i++) {
                G.Chaser_free[i] = pEm->Unk1CC[i];
            }
        }
    }
}

/* live selection task: flashes, shows the choice and waits (two Live_select_flash rounds) */
void Live_select(void)
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
    Live_sel.Negate = 1;
    Flash_fade_out();
    Flash_fade_in();
    Mess_set(0, 0x2001, Live_sel.Mess_no, 0);
    Live_sel.Mess_y = 0x62;
    Live_sel.Step = 0;
    Flash_fade_out();
    Flash_fade_in();
    Fade_set(2, 0, 0, 1, 0xFFFFFF, 0xFFFFFF, 1);
    Live_sel.Mess_y = 0xB3;
    Task_sleep(1);
    Live_sel.Negate = 0;
    Fade_set(2, 0, 0, 1, 0xFFFFFF, 0, 4);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
    Live_sel.Wait = 1;
    Ctrl_mode = 2;
    G.System_flg &= ~0x400;
    if (!Live_select_flash() && !Live_select_flash()) {
        Live_sel.Step++;
        while (G.Status_flg & 0x10000000) {
            Task_sleep(1);
        }
    }
    G.System_flg |= 0x400;
    Live_sel.Negate = 1;
    Pad_vib_data_set(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    i = 7;
    do {
        Task_sleep(1);
    } while (i-- != 0);
    Live_sel.Negate = 0;
    Ctrl_mode = 2;
    Live_sel.Wait = 0;
    G.System_flg &= ~0x400;
    Task_exit();
}

/* one flash of the live selection; returns 1 once the choice was made */
ULONG Live_select_flash(void)
{
    ULONG i;

    Live_sel.Step++;
    if (Main.Unk18E == 0) {
        do {
            Task_sleep(1);
            if (!(G.Status_flg & 0x10000000)) {
                return 1;
            }
        } while (Main.Unk18E == 0);
    }
    Main.Unk18E = 0;
    Live_sel.Negate = 1;
    G.System_flg |= 0x400;
    Pad_vib_data_set(Vib_data_tbl, 0);
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
    Live_sel.Negate = 0;
    return 0;
}

/* white flash in, with vibration and sound */
void Flash_fade_in(void)
{
    Pad_vib_data_set(Vib_data_tbl, 0);
    Snd_se_stad(0x22E, NULL, 0, 0);
    Fade_set(2, 0, 0, 1, 0, 0xFFFFFF, 4);
    while (Fade_status(2) != 0) {
        Task_sleep(1);
    }
}

/* white flash out */
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
