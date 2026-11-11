#include "sce0.h"
#include "em_sub.h"
#include "main.h"
#include "sca.h"
#include "line_scr.h"
#include "sub.h"
#include "player.h"
#include "om.h"



extern UCHAR D_800D442C[];
extern UCHAR D_800E017F[];
extern ULONG Old_Fatari;               /* 0x800DECB4 RE2 Old_Fatari */
extern ULONG Old_Uatari;               /* 0x800DECB8 RE2 Old_Uatari */
extern ULONG *Flg_addr[];              /* 0x8009E3F8 RE2 Flg_addr */
extern UCHAR D_800DBB58;
LONG Mess_set(ULONG A0, ULONG Attr, ULONG No, ULONG Flg); /* 0x8002FD30 RE2 Mess_set */
void func_800513CC();
void func_80051D60();
extern UCHAR D_800E01C4[];
void func_800514F0();
extern UCHAR *pCAot;                    /* 0x800DECB0 RE2 pCAot */
void func_800511B0();
extern LONG D_800DEC98;
LONG func_8006CD68(UCHAR No);
s8 func_8006CCF0(UCHAR No, ULONG A1, ULONG A2);
extern UCHAR D_800E017E[];
void func_80059A20();
s8 func_8006CC8C(UCHAR Id, UCHAR A1);  /* RE2 Search_item_id */
extern UCHAR D_800C703E[];
extern UCHAR D_800D442E[];
extern UCHAR D_800E0175[];
LONG func_80048974();
LONG func_800321C4();
LONG Quake_add();
LONG Pad_vib_set0();
LONG Pad_vib_set1();
LONG Pad_vib_fade_set();
LONG func_8001B894();
void func_80078498(UCHAR A0, UCHAR A1, USHORT A2);
extern UCHAR D_8009E340[];
extern UCHAR D_800E0170;
typedef struct {
    void *pData;                       /* 0x0 */
    UCHAR pad4;
    UCHAR Id;                          /* 0x5 */
    UCHAR pad6[2];
} SCE_UNK_DECC0;
extern SCE_UNK_DECC0 D_800DECC0[];
LONG Xa_play();
typedef void (*EM_FUNC)(ENEMY_WORK *pEm);
typedef void (*EM_FUNC3)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
extern EM_FUNC D_8009ECDC[];
extern EM_FUNC D_8009ECF8[];
extern EM_FUNC3 D_8009ED24[];
extern EM_FUNC3 D_8009ED84[];
extern EM_FUNC3 D_8009E7DC[];
extern EM_FUNC3 D_8009E82C[];
extern EM_FUNC3 D_8009E87C[];
extern EM_FUNC3 D_8009E8CC[];
extern EM_FUNC3 D_8009E91C[];
extern EM_FUNC3 D_8009E96C[];
extern EM_FUNC3 D_8009E9BC[];
extern EM_FUNC3 D_8009EA0C[];
extern EM_FUNC3 D_8009EA5C[];
extern EM_FUNC3 D_8009EAAC[];
extern EM_FUNC3 D_8009EAFC[];
extern EM_FUNC3 D_8009EB4C[];
extern EM_FUNC3 D_8009EB9C[];
extern EM_FUNC3 D_8009EBEC[];
typedef struct {
    UCHAR pad0[4];
    UCHAR St_type;                     /* 0x04 */
    UCHAR pad5[0xB8 - 0x5];
    SHORT UnkB8;                       /* 0xB8 */
} ST_WORK;
extern ST_WORK St;                     /* 0x800E01C0 RE2 St */
extern UCHAR D_800A0FDC[];
extern SHORT D_800D1F8E[];
extern SHORT D_800D1F82;
extern s8 D_80098728[];
UCHAR func_800102E8(void);             /* sld.s Rnd? */
void func_80034124(void *p);
LONG func_80055E38(s8 Work_kind, s8 Work_no);
void func_80058DD4();
extern ENEMY_WORK *D_800E01A8;
extern USHORT D_800E01B4;
void func_80052968(void);
void func_80052BA4(void);
void func_80052588(void);
void func_8001BA48();
void func_80052644(void);
void func_8005283C(void);
LONG func_80026BE8(ENEMY_WORK *pEm, ULONG A1, ULONG A2, ULONG A3);
void func_80079690(ENEMY_WORK *pEm, ULONG A1, ULONG A2, ULONG A3);
void func_80079868(ENEMY_WORK *pEm, ULONG A1);
extern UCHAR D_800E01C6[];
void func_80019318(ENEMY_WORK *pEm, ULONG A1, ULONG A2);
typedef struct {
    LONG Unk0;                         /* 0x00 */
    LONG Pos[3];                       /* 0x04 */
    LONG Tgt[3];                       /* 0x10 */
} SCE_CAMERA;
extern SCE_CAMERA D_800E0150;
LONG Set_view(LONG *pPos, LONG *pTgt);
typedef struct {
    UCHAR Unk0;                        /* 0x00 */
    UCHAR Unk1;                        /* 0x01 */
    UCHAR pad2[0x24 - 0x2];
    SHORT Unk24;                       /* 0x24 */
    UCHAR pad26[0xC0 - 0x26];
    LONG UnkC0;                        /* 0xC0 */
    UCHAR padC4[0xD4 - 0xC4];
} SCE_UNK_D4;
extern SCE_UNK_D4 D_800BA8A8[];
extern UCHAR D_8009E70C[];
extern UCHAR D_8009E70D[];
extern UCHAR D_8009EC3C[];
extern void (*D_8009ECAC[])(ENEMY_WORK *pEm, ULONG Type);
void func_8001B484(ULONG A0, ULONG A1, LONG A2, SVECTOR *pSv, ULONG A4);
typedef struct {
    ULONG Be_flg;                      /* 0x000 */
    UCHAR pad4[0x20 - 0x4];
    LONG Pos[3];                       /* 0x020 */
    UCHAR pad2C[0x4A - 0x2C];
    UCHAR Attr;                        /* 0x04A */
    UCHAR pad4B[0xC1 - 0x4B];
    UCHAR UnkC1;                       /* 0x0C1 */
    UCHAR padC2;
    UCHAR UnkC3;                       /* 0x0C3 */
    UCHAR padC4[0x194 - 0xC4];
} SCE_OM_WORK;
extern ULONG (*Sce_at_jump_tbl[])(void *pAot); /* 0x8009E0BC RE2 Sce_at_jump_tbl */
extern ENEMY_WORK *D_800E01AC;
extern USHORT D_800E01B6;
ENEMY_WORK *func_8005F7E8();
ENEMY_WORK *func_8005F8F0();
extern UCHAR D_800A0514[];
typedef struct {
    UCHAR Unk0;
    UCHAR Max;                         /* 0x1 */
    UCHAR pad2[2];
} ITEM_TBL4;
extern UCHAR D_8009CDE0[];
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG A3);
ULONG func_80027940(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void func_800776B0(ULONG A0, ULONG No, ENEMY_WORK *pEm);
void Flg_on(ULONG *pFlg, ULONG Bit);
LONG Flg_off(ULONG *pFlg, ULONG Bit);
LONG Weapon_cls(ENEMY_WORK *pEm);
LONG func_80043BE4(ENEMY_WORK *pEm);
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
ULONG func_800505AC(void *pEm, ULONG Attribute, ULONG Auto_flg); /* RE2 Sce_at_check */
extern UCHAR D_8009E6BC[];
extern SHORT D_8009E6D4[];
extern SHORT D_8009E6DC[];
extern SHORT D_8009E6E4[];
extern void (*D_8009E6EC[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80059824(ENEMY_WORK *pEm);
void func_8001A248(ENEMY_WORK *pEm, LONG Dir);
void func_8005F298();
ULONG func_8005F660(ENEMY_WORK *pEm, ULONG A1);
extern UCHAR D_8009E7D8[];
extern UCHAR D_8009E7C8[];
extern void (*D_8009E788[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
ULONG Flg_ck(ULONG *pFlg, ULONG Bit);
extern ULONG *D_8009E414;              /* Flg_addr[7] */
extern ULONG *D_8009E418;              /* Flg_addr[8] */
void func_8005C098(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_800549C4(ULONG Cut);

/* declarations from earlier parts of the original address-split file */
extern SHORT D_800DEC9C;               /* RE2 Item_box_entrance static Dir_spd */
extern SHORT D_800DEC9E;               /* RE2 Item_box_entrance static Spd_add */
extern OBJ_MODEL_WORK *D_800DECA0;     /* RE2 Item_box_entrance static pOwork */

INCLUDE_ASM("asm/nonmatchings/game/sce0", func_80051F78);

UCHAR func_80052094(ULONG Id)
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
    SCE_UNK_DECC0 *p;
    ULONG i;

    p = D_800DECC0;
    for (i = 0; i < D_800E0170; i++, p++) {
        if (p->Id == Id) {
            return p->pData;
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

INCLUDE_ASM("asm/nonmatchings/game/sce0", func_80052194);

void func_80052294(void)
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

void func_800522D0(void) {
}

/* RE2 Sce_work_clr_set */
void func_800522D8(void)
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
    Sce.pUnk1460 = G.pEnemy[0];
    for (i = 0, p = (LONG *)D_800DECC0; i < 5; i++) {
        *p++ = -1;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/sce0", func_800523C4);

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

INCLUDE_ASM("asm/nonmatchings/game/sce0", func_80052588);

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

void func_80052A94(void)
{
    func_800100A4(ScratchPad, Sce_jmp_tbl, 0x400);
    func_800523C4();
    func_80052294();
    func_800522D8();
    func_80052968();
    Sce.Type = 0;
    Sce.pCScd = G.pRoom->pScd;
    Event_exec(1, 0);
    func_80052BA4();
    func_80052588();
}

void func_80052B18(void)
{
    if ((G.Unk2468 & 0x2000000) == 0) {
        func_800100A4(ScratchPad, Sce_jmp_tbl, 0x400);
        func_800523C4();
        Sce.Type = 0;
        Sce.pCScd = G.pRoom->pScd;
        Event_exec(0, 1);
        func_80052644();
        func_80052BA4();
        func_8005283C();
    }
}

/* RE2 Sce_scheduler_main */
void func_80052BA4(void)
{
    ULONG Level;
    ULONG D0;
    SCE_TASK *pSce;
    ENEMY_WORK *pSub;

    Sce.Pl_pos[0] = G.Pl_pos_x;
    Sce.Pl_pos[1] = G.Pl_pos_y;
    Sce.Pl_pos[2] = G.Pl_pos_z;
    G.Pl_status_flg &= ~0x80;
    pSub = G.pUnk2660;
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
    func_80052294();
    func_800522D0();
    if (Sce.Pl_pos[0] != G.Pl_pos_x || Sce.Pl_pos[1] != G.Pl_pos_y || Sce.Pl_pos[2] != G.Pl_pos_z) {
        G.Pl_status_flg |= 0x80;
    }
    if (G.pUnk2660->Be_flg & 1) {
        if (Sce.Sub_pos[0] != G.pUnk2660->Pos_x || Sce.Sub_pos[1] != G.pUnk2660->Pos_y || Sce.Sub_pos[2] != G.pUnk2660->Pos_z) {
            G.pUnk2660->Status_flg |= 0x80;
        }
    }
}
