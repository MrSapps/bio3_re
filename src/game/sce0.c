#include "sce0.h"
#include "em_sub.h"
#include "main.h"
#include "sca.h"
#include "line_scr.h"
#include "sub.h"
#include "player.h"
#include "om.h"
#include "esp.h"
#include "cd.h"
#include "status.h"

extern UCHAR D_800D442C[];
extern UCHAR D_800E017F[];
extern ULONG Old_Fatari;               /* 0x800DECB4 RE2 Old_Fatari */
extern ULONG Old_Uatari;               /* 0x800DECB8 RE2 Old_Uatari */
extern ULONG *Flg_addr[];              /* 0x8009E3F8 RE2 Flg_addr */
extern UCHAR D_800DBB58;
LONG Mess_set(ULONG A0, ULONG Attr, ULONG No, ULONG Flg); /* 0x8002FD30 RE2 Mess_set */
void Save_entrance();
void func_80051D60();
extern UCHAR D_800E01C4[];
void Item_box_entrance();
extern UCHAR *pCAot;                    /* 0x800DECB0 RE2 pCAot */
void func_800511B0();
extern LONG D_800DEC98;
LONG func_8006CD68(UCHAR No);
s8 func_8006CCF0(UCHAR No, ULONG A1, ULONG A2);
extern UCHAR D_800E017E[];
void Aida_move();
s8 Search_item_id(UCHAR Id, UCHAR A1);  /* RE2 Search_item_id */
extern UCHAR D_800C703E[];
extern UCHAR D_800D442E[];
extern UCHAR D_800E0175[];
LONG func_80048974();
LONG Init_movie_work();
LONG Quake_add();
LONG Pad_vib_set0();
LONG Pad_vib_set1();
LONG Pad_vib_fade_set();
void func_80078498(UCHAR A0, UCHAR A1, USHORT A2);
extern UCHAR D_8009E340[];
LONG Xa_play();
typedef void (*EM_FUNC)(ENEMY_WORK *pEm);
typedef void (*EM_FUNC3)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern UCHAR D_800A0FDC[];
extern SHORT D_800D1F8E[];
extern SHORT D_800D1F82;              /* RE2 G.SaveData.Sce_random */
extern s8 D_80098728[];
UCHAR func_800102E8(void);             /* sld.s Rnd? */
UCHAR Rnd(void) __asm__("func_800102E8"); /* RE2 Rnd (sld.s) */
void func_80034124(void *p);
LONG Get_matrix(s8 Work_kind, s8 Work_no);
void Spl();
extern ENEMY_WORK *D_800E01A8;
extern USHORT D_800E01B4;
void func_80052968(void);
void Sce_scheduler_main(void);
void Sce_kirakira_set(void);
void func_8001BA48();
void func_80052644(void);
void func_8005283C(void);
LONG Joint_move(ENEMY_WORK *pEm, ULONG A1, ULONG A2, ULONG A3);
void func_80079690(ENEMY_WORK *pEm, ULONG A1, ULONG A2, ULONG A3);
void func_80079868(ENEMY_WORK *pEm, ULONG A1);
extern UCHAR D_800E01C6[];
void Foot_set_pl(ENEMY_WORK *pEm, ULONG A1, ULONG A2);
LONG Set_view(LONG *pPos, LONG *pTgt);
extern UCHAR D_8009E70C[];
extern UCHAR D_8009E70D[];
extern UCHAR D_8009EC3C[];
extern void (*D_8009ECAC[])(ENEMY_WORK *pEm, ULONG Type);
void func_8001B484(ULONG A0, ULONG A1, MATRIX *pM, SVECTOR *pSv, ULONG A4);
extern ULONG (*Sce_at_jump_tbl[])(void *pAot); /* 0x8009E0BC RE2 Sce_at_jump_tbl */
extern ENEMY_WORK *D_800E01AC;
extern USHORT D_800E01B6;
ENEMY_WORK *Spl_em_cdir_ck();
ENEMY_WORK *Spl_pllock_get();
extern UCHAR D_800A0514[];
extern UCHAR D_8009CDE0[];
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG A3);
ULONG Joint_move2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void Snd_se_walk(ULONG A0, ULONG No, ENEMY_WORK *pEm);
void Flg_on(ULONG *pFlg, ULONG Bit);
LONG Flg_off(ULONG *pFlg, ULONG Bit);
LONG Weapon_cls(ENEMY_WORK *pEm);
LONG Pl_weapon_ch(ENEMY_WORK *pEm);
LONG Pl_weapon_ch_init(ENEMY_WORK *pEm);
extern UCHAR D_800E01E8;
extern UCHAR D_8009E710[];
extern void (*D_8009E720[])(ENEMY_WORK *pEm);
ULONG func_800632B8(ENEMY_WORK *pEm, ULONG A1);
LONG func_800261AC(ENEMY_WORK *pEm, ULONG A1, void *A2);
extern USHORT D_800D1F94;
extern ULONG D_800D258C[];
ULONG func_800634F4(ENEMY_WORK *pEm, ULONG A1);
LONG Weapon_parts_clr(ENEMY_WORK *pEm);
extern UCHAR D_800E0171[];
extern ULONG D_800E01D4[];
extern ULONG D_8009DBB4;
LONG func_80044804(void *p, ULONG A1, ULONG A2);
extern ULONG D_800DECAC;
ULONG Sce_at_check(void *pEm, ULONG Attribute, ULONG Auto_flg); /* RE2 Sce_at_check */
extern UCHAR D_8009E6BC[];
extern SHORT D_8009E6D4[];
extern SHORT D_8009E6DC[];
extern SHORT D_8009E6E4[];
extern void (*D_8009E6EC[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Spl_Root_ck_sys(ENEMY_WORK *pEm);
void Rot_neck(ENEMY_WORK *pEm, LONG Dir);
void Spl_enemy_ck();
ULONG Spl_em_pos_ck(ENEMY_WORK *pEm, ULONG A1);
extern UCHAR D_8009E7D8[];
extern UCHAR D_8009E7C8[];
extern void (*D_8009E788[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
ULONG Flg_ck(ULONG *pFlg, ULONG Bit);
extern ULONG *D_8009E414;              /* Flg_addr[7] */
extern ULONG *D_8009E418;              /* Flg_addr[8] */
void Aida_mv05_w1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Cut_chg_main(ULONG Cut);

/* declarations from earlier parts of the original address-split file */
extern SHORT D_800DEC9C;               /* RE2 Item_box_entrance static Dir_spd */
extern SHORT D_800DEC9E;               /* RE2 Item_box_entrance static Spd_add */
extern OBJ_MODEL_WORK *D_800DECA0;     /* RE2 Item_box_entrance static pOwork */

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
void func_80051F78(ULONG Mode, PARTS_W *pP, UCHAR *pArg)
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

UCHAR Em_kind_search(ULONG Id)
{
    UCHAR *p;

    p = D_8009E340;
    do {
        if (p[0] == Id) {
            return p[1];
        }
        p += 3;
    } while (p[0] != 0xFF);
    return 0x80;
}

UCHAR func_800520D8(ULONG Id)
{
    UCHAR *p;

    p = D_8009E340;
    do {
        if (p[0] == Id) {
            return p[2];
        }
        p += 3;
    } while (p[0] != 0xFF);
    return 0x80;
}

void *func_8005211C(ULONG Id)
{
    EM_DAT *p;
    ULONG i;

    p = D_800DECC0;
    for (i = 0; i < D_800E0170; i++, p++) {
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
    Sce.Unk1496 = 0;
    Cc_work.Ccol_no = 0;
    Cc_work.Ctex_old = 0xFF;
    Sce.Unk148E = 0xFF;
    Sce.Unk148F = 0xFF;
    Sce.Unk1494 = 0xFF;
    pG->Unk75D9 = -1;
    Sce.Unk1498 = 0;
    Sce.Unk1499 = 0;
    G.pUnk75AC[10] = 0;
    Sce.Unk1488 = 0;
    Sce.Unk149B = 0;
    Sce.pUnk1460 = G.pEnemy[2];
    for (i = 0, p = (LONG *)D_800DECC0; i < 5; i++) {
        *p++ = -1;
    }
}

/* RE2 Sce_rnd_set (RE3: two Rnd() bytes plus a table step) */
void Sce_rnd_set(void)
{
    LONG D0;

    D0 = (s8)Rnd() << 8;
    D0 += Rnd() + D_80098728[Sce.Unk149E++];
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
        func_8001B484(((D0 & 0x60) << 19) | 0x705, 0x28000000, &pOm->M, &Svec, 0);
    }
}

extern UCHAR D_800B9E21;
void Xa_fade_set(ULONG frames);
void Pad_vib_init(void);
void func_80052644(void)
{
    ULONG i;

    i = 2;
    switch (Sce.Unk149B) {
    case 1:
        if (G.Unk20FC & 0x100) {
            Sce.Unk149B = 2;
            Fade_set(0, 1, 0, 2, 0, 0xFFFFFF, 0x10);
            Moji.Mess_flg = 0;
            Xa_fade_set(0x10);
            Game.Stop_bak = G.Unk2468;
            G.Unk2468 |= 0xFD000000;
            do {
                Sce.Sce_task[i].Status = 0;
            } while (i++ < 10);
        } else if (Sce.Sce_task[Sce.Unk149C].Status == 0) {
            Sce.Unk149B = 0;
        }
        break;
    case 2:
        if (Fade_status(0) == 0) {
            Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0);
            Sce.Unk149B = 4;
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
            Event_exec(1, Sce.Unk149D);
            Sce.Unk149B = 8;
            func_8001B894(0xFF02, 0);
        }
        break;
    }
}

void func_8005283C(void)
{
    SCENARIO *pScn = &Sce;

    switch (pScn->Unk149B) {
    case 1:
        if (pScn->Sce_task[pScn->Unk149C].Status == 0) {
            pScn->Unk149B = 0;
        }
        break;
    case 8:
        if (pScn->Sce_task[1].Status == 0) {
            Fade_off(0);
            pScn->Unk149B = 0;
        }
        break;
    }
}

void func_800528D4(void)
{
    UCHAR i;
    tagGLOBAL *pG = &G;

    for (i = 0; i < 8; i++) {
        pG->Unk7E24[i] = 0;
    }
}

void func_8005290C(void)
{
    switch (G.Unk7862) {
    case 3:
    case 4:
        G.Unk7862 = 0;
        break;
    case 5:
        G.Unk7862 = 0;
        func_800528D4();
        break;
    }
}

void func_80052968(void)
{
    USHORT Id;
    tagGLOBAL *pG = &G;
    UCHAR i;

    if ((pG->Status_flg & 0x40000000) == 0) {
        return;
    }
    func_8005290C();
    Id = (USHORT)pG->Unk7840 + (((USHORT)pG->Unk783E + 1) << 8);
    switch (pG->Unk7862) {
    case 6:
        func_800528D4();
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
    func_800100A4(ScratchPad, Sce_jmp_tbl, 0x400);
    Sce_rnd_set();
    Sce_work_clr();
    Sce_work_clr_set();
    func_80052968();
    Sce.Type = 0;
    Sce.pCScd = G.pRoom->pScd;
    Event_exec(1, 0);
    Sce_scheduler_main();
    Sce_kirakira_set();
}

void Sce_scheduler(void)
{
    if ((G.Unk2468 & 0x2000000) == 0) {
        func_800100A4(ScratchPad, Sce_jmp_tbl, 0x400);
        Sce_rnd_set();
        Sce.Type = 0;
        Sce.pCScd = G.pRoom->pScd;
        Event_exec(0, 1);
        func_80052644();
        Sce_scheduler_main();
        func_8005283C();
    }
}

/* RE2 Sce_scheduler_main */
void Sce_scheduler_main(void)
{
    ULONG Level;
    ULONG D0;
    SCE_TASK *pSce;
    ENEMY_WORK *pSub;

    Sce.Pl_pos[0] = G.Pl_pos_x;
    Sce.Pl_pos[1] = G.Pl_pos_y;
    Sce.Pl_pos[2] = G.Pl_pos_z;
    G.Pl_status_flg &= ~0x80;
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
    if (Sce.Pl_pos[0] != G.Pl_pos_x || Sce.Pl_pos[1] != G.Pl_pos_y || Sce.Pl_pos[2] != G.Pl_pos_z) {
        G.Pl_status_flg |= 0x80;
    }
    if (G.pEnemy[1]->Be_flg & 1) {
        if (Sce.Sub_pos[0] != G.pEnemy[1]->Pos_x || Sce.Sub_pos[1] != G.pEnemy[1]->Pos_y || Sce.Sub_pos[2] != G.pEnemy[1]->Pos_z) {
            G.pEnemy[1]->Status_flg |= 0x80;
        }
    }
}
