#include "sce0.h"
#include "sub.h"
#include "om.h"
#include "cd.h"
#include "pad.h"
#include "sld.h"

UCHAR Rnd_uc(void) __asm__("Rnd"); /* RE2 declared Rnd as UCHAR; the canonical ULONG keeps unsigned arithmetic for the other callers */

/* AOT entry (RE2 struct tagSCE_AOT, SCE1.H); RE3 entries made at run time carry a part pointer */
struct tagSCE_AOT {
    UCHAR Id;                          /* 0x00 */
    UCHAR Type;                        /* 0x01 */
    signed char nFloor;                /* 0x02 */
    UCHAR Super;                       /* 0x03 */
    SHORT X;                           /* 0x04 */
    SHORT Z;                           /* 0x06 */
    USHORT W;                          /* 0x08 */
    USHORT D;                          /* 0x0A */
    USHORT Data[4];                    /* 0x0C */
    PARTS_W *pParts;                   /* 0x14 */
};

/* AOT for a dropped part (func_800452E8: a part landing on the floor) */
void Sce_aot_parts_set(ULONG Mode, PARTS_W *pP, UCHAR *pArg)
{
    struct tagSCE_AOT *pAot;
    tagGLOBAL *pG = &G;

    if (!(Mode & 1)) {
        if (pG->Sce_aot[31] == NULL) {
            pAot = (struct tagSCE_AOT *)pG->pEm_mem;
            pG->pEm_mem = (ENEMY_WORK *)(pAot + 1);
            pG->Sce_aot[31] = pAot;
            pAot->Id = 0;
            pG->nSce_aot++;
        }
        return;
    }
    if (pG->Sce_aot[31] == NULL) {
        pAot = (struct tagSCE_AOT *)pG->pEm_mem;
        pG->pEm_mem = (ENEMY_WORK *)(pAot + 1);
        pG->Sce_aot[31] = pAot;
        pAot->Id = 0;
        pG->nSce_aot++;
    } else {
        pAot = pG->Sce_aot[31];
    }
    pAot->Id = 2;
    pAot->Type = 0x51;
    pAot->nFloor = pP->Workm.t[1] / -1800;
    pAot->Super = 0;
    pAot->X = pP->Workm.t[0] - 800;
    pAot->Z = pP->Workm.t[2] - 800;
    pAot->W = 1000;
    pAot->D = 1000;
    pAot->Data[0] = pArg[0];
    pAot->Data[1] = pArg[1];
    pAot->Data[2] = *(USHORT *)(pArg + 2);
    pAot->Data[3] = 0xFFFF;
    pAot->pParts = pP;
}

ULONG Em_kind_search(ULONG Id)
{
    EM_KIND *p;

    p = Em_kind_tbl;
    do {
        if (p->Id == Id) {
            return p->Kind;
        }
        p++;
    } while (p->Id != 0xFF);
    return 0x80;
}

UCHAR Em_kind_flg_search(ULONG Id)
{
    EM_KIND *p;

    p = Em_kind_tbl;
    do {
        if (p->Id == Id) {
            return p->Flg;
        }
        p++;
    } while (p->Id != 0xFF);
    return 0x80;
}

void *Em_dat_search(ULONG Id)
{
    EM_DAT *p;
    ULONG i;

    p = Em_dat;
    for (i = 0; i < Em_dat_num; i++, p++) {
        if (p->Kind == Id) {
            return p->pEm;
        }
    }
    return NULL;
}

/* RE2 Sce_aot_init */
void Sce_aot_init(void)
{
    ULONG i;
    struct tagSCE_AOT **pAot;

    pAot = G.Sce_aot;
    for (i = 32; i != 0; i--) {
        *pAot++ = NULL;
    }
    G.nSce_aot = 0;
}

/* RE2 Sce_model_init */
void Sce_model_init(void)
{
    ULONG i;
    ULONG *pTim;
    OBJ_MODEL_WORK *pOm;
    MODEL_LINK_INFO *pMi;
    UCHAR Tpage, Clut;
    tagGLOBAL *pG;

    pG = &G;
    pG->pOld_tim = NULL;
    pG->pOld_tmd = NULL;
    pOm = pG->Ob_model;
    for (i = 32; i != 0; i--) {
        pOm->Be_flg = 0;
        pOm->UnkC3 = 0;
        pOm++;
    }

    i = pG->pRoom->nOmodel;
    pMi = pG->pRoom->pOmodel;
    pOm = pG->Ob_model;
    for (; i != 0; i--) {
        Tpage = pG->Ctp.u.Ctpage;
        pTim = pMi->Tim_adr;
        Clut = pG->Ctp.u.Cclut;
        pOm->Tpage = Tpage;
        pOm->Unk49 = Clut;
        if (pG->pOld_tim != pTim) {
            pG->pOld_tim = pTim;
            if (pTim != (ULONG *)pG->pRoom) {
                Mapping_tim(pTim);
            } else {
                pOm->Be_flg = 0x80000000;
            }
        } else {
            /* same TIM as the previous model: share its tpage/clut */
            pOm->Tpage = pOm[-1].Tpage;
            pOm->Unk49 = pOm[-1].Unk49;
        }
        pOm++;
        pMi++;
    }
}

void Sce_work_clr(void)
{
    G.Sce_work[0] = -1;
    G.Sce_work[1] = -1;
    G.Sce_work[2] = -1;
    G.Sce_work[3] = -1;
    G.Unk788C = 0;
    G.Unk2474[0] = 0;
    G.Unk2474[1] = 0;
    G.Unk2474[2] = 0;
    G.Unk2474[3] = 0;
    D_800E017E[0] = 0;
}

void Sce_work_clr_at(void) {
}

/* RE2 Sce_work_clr_set */
void Sce_work_clr_set(void)
{
    ULONG i;
    SCE_TASK *pTask;
    LONG *p;
    tagGLOBAL *pG = &G;

    pTask = Sce.Sce_task;
    for (i = 0; i < 10; i++) {
        pTask->Task_level = i;
        pTask->Status = 0;
        pTask->Sub_ctr = 0;
        pTask->Ifel_ctr[0] = -1;
        pTask->Loop_ctr[0] = -1;
        pTask++;
    }
    Sce_aot_init();
    pG->Unk7888 = 0;
    Sce.Unk1497 = 0xFF;
    Sce.Mizu_div_max = 0;
    Sce.Mizu_div_ctr = 0;
    Cc_work.Ccol_no = 0;
    Cc_work.Ctex_old = 0xFF;
    Sce.Em_id = 0xFF;
    Sce.Em_kind_flg = 0xFF;
    Sce.Unk1494 = 0xFF;
    pG->Unk75D9 = -1;
    Sce.Em_set_kind0 = 0;
    Sce.Em_set_kind1 = 0;
    G.pUnk75AC[10] = 0;
    Sce.Em_dat_num = 0;
    Sce.Skip_state = 0;
    Sce.pEm_last = G.pEnemy[2];
    for (i = 0, p = (LONG *)Em_dat; i < 5; i++) {
        *p++ = -1;
    }
}

/* RE2 Sce_rnd_set (RE3: two Rnd_uc() bytes plus a table step) */
void Sce_rnd_set(void)
{
    LONG D0;

    D0 = (s8)Rnd_uc() << 8;
    D0 += Rnd_uc() + (s8)D_80098728[Sce.Rnd_idx++];
    D_800D1F82 = D0;
}

/* RE2 Event_init */
void Event_init(SCE_TASK *pSce, ULONG Evt_no)
{
    UCHAR *pScd;
    ULONG D0;

    pScd = Sce.pCScd;
    D0 = ((USHORT *)pScd)[Evt_no];
    pSce->Status = 1;
    pSce->Routine0 = 0;
    pSce->pS_SP = pSce->Stack[pSce->Sub_ctr];
    pSce->Ifel_ctr[0] = -1;
    pSce->Loop_ctr[0] = -1;
    pSce->Data = pScd + D0;
}

/* RE2 Event_exec */
void Event_exec(ULONG Task_level, ULONG Evt_no)
{
    ULONG i;
    ULONG *pSpd;
    SCE_TASK *pTask;
    SCENARIO *pScn;

    pScn = &Sce;
    if (pScn->Type == 0) {
        if (Task_level > 9) {
            Task_level = 2;
            pTask = &pScn->Sce_task[2];
            while (pTask->Status != 0) {
                if (Task_level == 9) {
                    break;
                }
                Task_level++;
                if (Task_level > 9) {
                    return;
                }
                pTask++;
                if (pTask->Status == 0) {
                    break;
                }
            }
        }
    } else if (Task_level > 13) {
        Task_level = 10;
        pTask = &pScn->Sce_task[10];
        while (pTask->Status != 0) {
            if (Task_level == 13) {
                break;
            }
            Task_level++;
            if (Task_level > 13) {
                return;
            }
            pTask++;
        }
    }

    pTask = &pScn->Sce_task[Task_level];
    pTask->Sub_ctr = 0;
    pTask->pWork = (struct tagENEMY_WORK *)1;
    pSpd = (ULONG *)&pTask->Spd_x;
    for (i = 6; i != 0; i--) {
        *pSpd++ = 0;
    }
    Event_init(pTask, Evt_no);
}

/* RE2 Sce_kirakira_set: sparkle effect on the object models flagged in UnkC3 */
void Sce_kirakira_set(void)
{
    ULONG Cnt;
    ULONG i;
    ULONG D0;
    SVECTOR Svec;
    OBJ_MODEL_WORK *pOm;

    Cnt = G.pRoom->nOmodel;
    for (i = 0; i < Cnt;) {
        pOm = &G.Ob_model[i++];
        if ((pOm->Be_flg & 1) == 0) {
            continue;
        }
        D0 = pOm->UnkC3;
        if ((D0 & 0x80) == 0) {
            continue;
        }
        Svec.vx = 0;
        Svec.vy = -90;
        Svec.vz = 0;
        Esp_call(((D0 & 0x60) << 19) | 0x705, 0x28000000, &pOm->M, &Svec, 0);
    }
}

void Sce_skip_ck(void)
{
    ULONG i;

    i = 2;
    switch (Sce.Skip_state) {
    case 1:
        if (G.Unk20FC & 0x100) {
            Sce.Skip_state = 2;
            Fade_set(0, 1, 0, 2, 0, 0xFFFFFF, 0x10);
            Moji.Mess_flg = 0;
            Xa_fade_set(0x10);
            Game.Stop_bak = G.Unk2468;
            G.Unk2468 |= 0xFD000000;
            do {
                Sce.Sce_task[i].Status = 0;
            } while (i++ < 10);
        } else if (Sce.Sce_task[Sce.Skip_task].Status == 0) {
            Sce.Skip_state = 0;
        }
        break;
    case 2:
        if (Fade_status(0) == 0) {
            Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0);
            Sce.Skip_state = 4;
        }
        break;
    case 4:
        if (D_800B9E21 == 0) {
            G.Unk2468 = Game.Stop_bak;
            Pad_vib_init();
            Fade_off(1);
            Fade_off(2);
            Fade_off(3);
            Moji.Mess_flg = 0;
            Flg_off(&G.Status_flg, 0x1D);
            Flg_off(&G.Status_flg, 0x1E);
            Event_exec(1, Sce.Skip_evt);
            Sce.Skip_state = 8;
            func_8001B894(0xFF02, 0);
        }
        break;
    }
}

void Sce_skip_end(void)
{
    SCENARIO *pScn = &Sce;

    switch (pScn->Skip_state) {
    case 1:
        if (pScn->Sce_task[pScn->Skip_task].Status == 0) {
            pScn->Skip_state = 0;
        }
        break;
    case 8:
        if (pScn->Sce_task[1].Status == 0) {
            Fade_off(0);
            pScn->Skip_state = 0;
        }
        break;
    }
}

void Chaser_room_clr(void)
{
    UCHAR i;
    tagGLOBAL *pG = &G;

    for (i = 0; i < 8; i++) {
        pG->Unk7E24[i] = 0;
    }
}

void Chaser_state_reset(void)
{
    switch (G.Unk7862) {
    case 3:
    case 4:
        G.Unk7862 = 0;
        break;
    case 5:
        G.Unk7862 = 0;
        Chaser_room_clr();
        break;
    }
}

void Chaser_room_ck(void)
{
    USHORT Id;
    tagGLOBAL *pG = &G;
    UCHAR i;

    if ((pG->Status_flg & 0x40000000) == 0) {
        return;
    }
    Chaser_state_reset();
    Id = (USHORT)pG->Unk7840 + (((USHORT)pG->Unk783E + 1) << 8);
    switch (pG->Unk7862) {
    case 6:
        Chaser_room_clr();
    case 0:
        if (pG->Unk784C != Id) {
            pG->Unk7862 = 3;
        } else {
            pG->Unk7862 = 4;
        }
        break;
    case 1:
        if (pG->Unk784C == Id) {
            pG->Unk7862 = 4;
        }
        break;
    case 2:
        i = 0;
        do {
            if (Id == (USHORT)pG->Unk7E24[i]) {
                pG->Unk784C = Id;
                pG->Unk7862 = 5;
                break;
            }
        } while (i++ < 8);
        break;
    }
}

void Sce_scheduler_set(void)
{
    Memcpy(ScratchPad, Sce_jmp_tbl, 0x400);
    Sce_rnd_set();
    Sce_work_clr();
    Sce_work_clr_set();
    Chaser_room_ck();
    Sce.Type = 0;
    Sce.pCScd = G.pRoom->pScd;
    Event_exec(1, 0);
    Sce_scheduler_main();
    Sce_kirakira_set();
}

void Sce_scheduler(void)
{
    if ((G.Unk2468 & 0x2000000) == 0) {
        Memcpy(ScratchPad, Sce_jmp_tbl, 0x400);
        Sce_rnd_set();
        Sce.Type = 0;
        Sce.pCScd = G.pRoom->pScd;
        Event_exec(0, 1);
        Sce_skip_ck();
        Sce_scheduler_main();
        Sce_skip_end();
    }
}

/* RE2 Sce_scheduler_main */
void Sce_scheduler_main(void)
{
    ULONG Level;
    ULONG D0;
    SCE_TASK *pSce;
    ENEMY_WORK *pSub;

    Sce.Pl_pos[0] = G.Player.Pos_x;
    Sce.Pl_pos[1] = G.Player.Pos_y;
    Sce.Pl_pos[2] = G.Player.Pos_z;
    G.Player.Status_flg &= ~0x80;
    pSub = G.pEnemy[1];
    if (pSub->Be_flg & 1) {
        Sce.Sub_pos[0] = pSub->Pos_x;
        Sce.Sub_pos[1] = pSub->Pos_y;
        Sce.Sub_pos[2] = pSub->Pos_z;
        pSub->Status_flg &= ~0x80;
    }
    for (Level = 0; Level < 10; Level++) {
        pSce = &Sce.Sce_task[Level];
        if (pSce->Status == 0) {
            continue;
        }
        while (1) {
            D0 = ((SCE_JMP_FUNC *)0x1F800000)[*pSce->Data](pSce);
            if (D0 == 1) {
                continue;
            }
            if (D0 == 2) {
                break;
            }
            if (pSce->Ifel_ctr[pSce->Sub_ctr] < 0) {
                break;
            }
            pSce->Data = (UCHAR *)*--pSce->pS_SP;
            pSce->Ifel_ctr[pSce->Sub_ctr]--;
        }
    }
    Sce_work_clr();
    Sce_work_clr_at();
    if (Sce.Pl_pos[0] != G.Player.Pos_x || Sce.Pl_pos[1] != G.Player.Pos_y || Sce.Pl_pos[2] != G.Player.Pos_z) {
        G.Player.Status_flg |= 0x80;
    }
    if (G.pEnemy[1]->Be_flg & 1) {
        if (Sce.Sub_pos[0] != G.pEnemy[1]->Pos_x || Sce.Sub_pos[1] != G.pEnemy[1]->Pos_y || Sce.Sub_pos[2] != G.pEnemy[1]->Pos_z) {
            G.pEnemy[1]->Status_flg |= 0x80;
        }
    }
}
