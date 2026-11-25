/*
 * ROOM.C (RE2 ROOM.C lineage): stage and room loading (Set_stage, Set_room) and room animation hand-out
 */
#include "pl_wep.h"
#include "monitor.h"
#include "sce0.h"
#include "cd.h"
#include "game.h"
#include "player.h"
#include "psp.h"
#include "room.h"
#include "sld.h"
#include "snd.h"
#include "status.h"

#define RND() ((signed char)Rnd())


void Map_visit_set_i(LONG Stage, LONG Room) __asm__("Map_visit_set"); /* Set_room passes int stage/room (implicit declaration, no UCHAR masks) */

/* load and set up the room (RE2 Set_room) */
void Set_room(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pPl = G_PLAYER;
    ROOM_WORK *pRoom;
    ENEMY_WORK **ppEm;
    ULONG **ppAdr;
    LONG Stage;
    LONG Room;
    LONG Size;
    LONG i;
    ENEMY_WORK *pEm_mem;

    pEm_mem = (ENEMY_WORK *)pG->pFree_work_head;
    pG->Ctp.Tpage_clut = 0xD05;
    Stage = pG->Stage_no;
    Room = pG->Room_no;
    pG->Room_enemy_flg = 0;
    pG->Room_player_flg = 0;
    pG->pEm_mem = pEm_mem;
    pG->pRoom = (ROOM_WORK *)pEm_mem;
    pG->Status_flg &= 0xFFFF0000;
    pRoom = pG->pRoom;
    Kage_work_init();
    Kage_work9_init();
    Memclr(&pG->Sub_work, sizeof(ENEMY_WORK));
    ppEm = &pG->pEnemy[1];
    pG->ppEnemy_end = &pG->pEnemy[2];
    pG->Em_num = 0;
    for (i = 41; i > 0; i--) {
        *ppEm++ = &pG->Sub_work;
    }
    pG->Em_bin_info[0].Id = 0xFF;
    pG->Em_bin_info[1].Id = 0xFF;
    pG->Em_bin_info[0].Use = 0;
    pG->Em_bin_info[1].Use = 0;
    pG->pObj_work_end = &pG->Ob_model[0];
    for (i = 0; i < 32; i++) {
        pG->Ob_model[0].Be_flg = 0;
    }
    if (pPl->Id != pG->Change_player) {
        if (pG->Change_player >= 8) {
            if (pPl->Id < 8) {
                pPl->Life = 200;
                pPl->Status_flg = 0;
                pG->Pl_id_bak = pPl->Id;
            }
            G.Status_flg |= 0x8000000;
            Snd_vab_load(0, 8, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
        } else {
            if (pPl->Id >= 8) {
                pPl->Life = 200;
                pPl->Status_flg = 0;
                pG->Change_player = pG->Pl_id_bak;
            } else {
                if (pG->Change_player == 1) {
                    Item_work0_num = 10;
                }
                if (pG->Pl_id_bak >= 2) {
                    if (pG->Change_player < 2) {
                        pG->Change_player = pG->Pl_id_bak;
                    }
                } else if (pG->Change_player >= 2) {
                    pG->Pl_id_bak = pG->Change_player;
                }
            }
            G.Status_flg &= ~0x8000000;
            Snd_vab_load(0, 2, (ULONG *)0x801FBC00, (ULONG *)0x801C1814);
        }
        pPl->Max_life = 200;
        pPl->Id = pG->Change_player;
        St_chenge_pl(pG->Change_player);
        Player_set(pPl);
        *(ULONG *)&pPl->Routine_0 = 0;
    }
    Snd_bgm_fade_wait();
    Size = Cd_read(Room_file_tbl[Stage][Room], NULL, 4, "ARD");
    ppAdr = &pRoom->pEdt0;
    do {
        if (*ppAdr) {
            *ppAdr = (ULONG *)((UCHAR *)*ppAdr + (ULONG)pRoom);
        }
        ppAdr++;
    } while (ppAdr < (ULONG **)pRoom->Cut);
    for (i = 0; i < pRoom->nCut; i++) {
        pRoom->pRcut[i].pSp = (ULONG *)((UCHAR *)pRoom->pRcut[i].pSp + (ULONG)pRoom);
    }
    pG->pEm_mem = (ENEMY_WORK *)((UCHAR *)pG->pEm_mem + Size);
    for (i = 0; i < pRoom->nOmodel; i++) {
        pRoom->pOmodel[i].Tim_adr = (ULONG *)((UCHAR *)pRoom->pOmodel[i].Tim_adr + (ULONG)pRoom);
        pRoom->pOmodel[i].Tmd_adr = (ULONG *)((UCHAR *)pRoom->pOmodel[i].Tmd_adr + (ULONG)pRoom);
    }
    Esp_init_R();
    Snd_sys_pset();
    Cut_change(pG->Cut_no);
    Oba_init();
    Pri_be_flg_init();
    Sce_model_init();
    Ot_sub_init();
    if (G.System_flg & 0x10000000) {
        G_random = Game.pSave->Unk714;
        G.Sce_random = Game.pSave->Unk718;
        Sce_rnd_idx[0] = 0;
    }
    Sce_scheduler_set();
    if (G.Em_num != 0 || Rbj_id != 0xFF || G.pEnemy[1]->Be_flg != 0) {
        Cd_read(Room_file_tbl[Stage][Room] + 1, NULL, 5, "BIN FILE");
    }
    Rbj_set();
    Psp_init();
    Snd_sys_enem_set();
    if (G.System_flg & 0x10000000) {
        G_random = Game.pSave->Unk714;
        G.Sce_random = Game.pSave->Unk718;
        Sce_rnd_idx[0] = 0;
    }
    Em_init_move();
    Map_visit_set_i(Stage, Room);
}

/* RE2 Set_stage: loads the stage overlay (task file Stage + 0x11) and sets the free memory after it */
void Set_stage(void)
{
    ULONG Size = Task_lexecute(2, G.Stage_no + 0x11);

    G.pFree_work_head = (UCHAR *)(((Size + 3) & ~3) + 0x8011A000);
    Task_sleep(1);
}

/* hand the room's sub animation sets (pRoom->pEdd) to the player and enemy works */
void Rbj_set(void)
{
    UCHAR *pEdd;
    ULONG *pOfs;
    ULONG *pKan;
    ULONG Bits;
    UCHAR No;
    UCHAR Cnt;
    ENEMY_WORK *pEm;

    No = 0;
    if (Rbj_id == 0xFF) {
        return;
    }
    pEdd = G.pRoom->pEdd;
    Cnt = pEdd[4];
    pOfs = (ULONG *)(pEdd + *(ULONG *)pEdd);
    do {
        pKan = (ULONG *)(pEdd + pOfs[0]);
        Bits = *pKan;
        if (Bits & 1) {
            G.Player.pUnk100_kan_t_ptr = pKan + 1;
            G.Player.pUnk104_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
        }
        for (Bits >>= 1; Bits != 0; Bits >>= 1, No++) {
            if (Bits & 1) {
                if (No < 15) {
                    pEm = G.pEnemy[No + 1];
                    if (pEm->Be_flg & 1) {
                        pEm->pUnk100_kan_t_ptr = pKan + 1;
                        G.pEnemy[No + 1]->pUnk104_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
                    }
                } else if (No < 23) {
                    G.pEnemy[No - 15]->pUnk19C_kan_t_ptr = pKan + 1;
                    G.pEnemy[No - 15]->pUnk198_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
                } else {
                    G.pEnemy[No - 23]->pUnk194_kan_t_ptr = pKan + 1;
                    G.pEnemy[No - 23]->pUnk190_seq_t_ptr = (ULONG *)(pEdd + pOfs[1]);
                }
            }
        }
        pOfs += 2;
        No = 0;
    } while (--Cnt);
}

/* run every enemy's routine once at room start (RE2 Em_init_move) */
void Em_init_move(void)
{
    LONG Stop_save;
    LONG Timer0_save;
    ULONG Type_hi;
    ENEMY_WORK *pEm;
    ENEMY_WORK **ppEm;

    Type_hi = 0;
    if (G.Stop_flg & 0x20000000) {
        G.Stop_flg &= ~0x20000000;
        Stop_save = 1;
    } else {
        Stop_save = 0;
    }
    Pl_reset(G_PLAYER);
    ppEm = &G.pEnemy[1];
    if (G.ppEnemy_end != ppEm) {
        do {
            pEm = *ppEm;
            G.pEm = pEm;
            if (pEm->Be_flg & 1) {
                Timer0_save = pEm->Timer0;
                G.Em_move_tbl[pEm->Id - 0x10](pEm);
                if (Timer0_save != 0) {
                    *(ULONG *)&pEm->Routine_0 = ((Timer0_save & 0xFF00) - 0x100) | 4;
                    pEm->Move_no = Timer0_save;
                    pEm->Move_cnt = 0;
                    pEm->Unk144 = pEm->UnkDA;
                    G.Em_move_tbl[pEm->Id - 0x10](pEm);
                }
                pEm->Type |= Type_hi; /* RE2 restored Type & 0x8000 here; RE3 keeps a dead 0 */
            }
            ppEm++;
        } while (ppEm != G.ppEnemy_end);
    }
    if (Stop_save) {
        G.Stop_flg |= 0x20000000;
    }
}
