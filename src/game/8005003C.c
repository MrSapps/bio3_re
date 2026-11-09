#include "sce0.h"
#include "em_sub.h"
#include "main.h"
#include "sca.h"
#include "line_scr.h"
#include "sub.h"
#include "player.h"



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
void func_80059A20(void *p);
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
LONG func_80055E38(LONG A0, LONG A1);
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
/* ---- end externs ---- */

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005003C);

void func_80050208(ULONG Type, ULONG Id, ULONG Floor)
{
    SCA_DATA *pData;
    SCA_DATA *pEnd;
    ULONG Cnt;
    SCA_DATA **ppList;

    pData = (SCA_DATA *)G.pRoom->pSca;
    Cnt = 0;
    ppList = G.Sca_list;
    *ppList = NULL;
    pEnd = pData + pData->X0;
    if (pData->X0 == 0) {
        return;
    }
    pData++;
    goto test;
loop:
        if ((pData->Type & Type) && (pData->Id & Id) && Floor >= ((s8 *)&pData->Floor)[0] && ((s8 *)&pData->Floor)[1] >= Floor) {
            *ppList++ = pData;
            Cnt++;
            *ppList = NULL;
            if (Cnt == 32) {
                return;
            }
        }
        pData++;
test:
    if (pData != pEnd) goto loop;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800502B8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800503E8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050524);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800505AC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050B58);

/* RE2 Nothing */
ULONG func_80050D00(void *pAot)
{
    G.Sce_work[0] = Old_Fatari;        /* RE2 G.SaveData.F_atari */
    G.Sce_work[1] = Old_Uatari;        /* RE2 G.SaveData.U_atari */
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050D28);

void func_80050FE0(void)
{
    tagGLOBAL *pG = &G;
    UCHAR Rtn;

    Rtn = pG->Front_func_rtn;
    if (Rtn == 0) {
        if ((Moji.Mess_flg & 0x80) == 0) {
            if (pG->Unk7848 == 0) {
                func_8006CD68(D_800DEC98);
            }
            pG->Front_func = NULL;
        }
    } else {
        D_800DEC98 = Rtn - 1;
        if ((Moji.Mess_flg & 0x80) == 0) {
            if (func_8006CCF0(D_800DEC98, 1, 1) == 0) {
                Mess_set(0, 0, 0x10, 0xff000000);
                pG->Unk2468 |= 0xff000000;
            } else {
                pG->Front_func = NULL;
            }
            pG->Front_func_rtn = 0;
        }
    }
}

void func_800510B0(void)
{
    if (G.Front_func_rtn == 0 && (Moji.Mess_flg & 0x80) == 0) {
        if (G.Unk7848 == 0) {
            Game.Doordemo_flg = 1;
            G.pSce_aot_on = (void *)G.Unk21DC;
            G.Unk2468 |= 0xff000000;
        }
        G.Front_func = NULL;
        G.Front_func_rtn = 0;
    }
}

/* RE2 Sce_Item */
ULONG func_8005111C(void *pAot)
{
    G.Unk21DC = (LONG)pAot;
    G.pAot_data = pCAot;
    if ((((UCHAR *)pAot)[7] & 1) != 0) {
        G.Pl_routine_1 = 8;
        G.Pl_routine_2 = 0;
        return 1;
    }
    if (((UCHAR *)pAot)[4] >= 0xF8) {
        G.Front_func = func_800511B0;
        G.Front_func_rtn = 0;
        return 0;
    }
    D_800E01C4[0] = 1;
    Game.Unk1 = 1;
    Game.Stop_bak = G.Unk2468;
    G.Status_flg |= 0x200;
    return 0;
}

void func_800511B0(void)
{
    tagGLOBAL *pG = &G;
    UCHAR *pDat;

    pDat = (UCHAR *)pG->Unk21DC;
    switch (pG->Front_func_rtn) {
    case 0:
        Moji.Mess_item = pDat[0];
        Mess_set(0, 0, 6, 0xff000000);
        pG->Front_func_rtn = 1;
        pG->Unk2468 |= 0xff000000;
        break;
    case 1:
        if ((Moji.Mess_flg & 0x80) == 0) {
            pG->Front_func = NULL;
            pG->Front_func_rtn = 0;
            D_800E01C4[0] = 1;
            Game.Unk1 = 1;
            Game.Stop_bak = pG->Unk2468;
            pG->Unk2468 |= 0xff000000;
            pG->Status_flg |= 0x200;
        }
        break;
    }
}

LONG func_8005127C(void)
{
    return 0;
}

/* RE2 Sce_Message */
ULONG func_80051284(void *pAot)
{
    USHORT *pDat;

    pDat = (USHORT *)pAot;
    Mess_set(0, pDat[1] | 0x2000, pDat[0], (ULONG)pDat[2] << 16);
    return 0;
}

/* RE2 Sce_Event */
ULONG func_800512BC(void *pAot)
{
    if ((G.Unk2468 & 0x2000000) == 0) {
        Event_exec(((USHORT *)pAot)[0], ((UCHAR *)pAot)[3]);
    }
    return 0;
}

/* RE2 Sce_Flg_chg */
ULONG func_800512FC(void *pAot)
{
    USHORT *pDat;
    ULONG *pFlg;
    ULONG Bit;

    pDat = (USHORT *)pAot;
    Bit = pDat[1];
    pFlg = Flg_addr[pDat[0]] + ((UCHAR)Bit >> 5);
    if (pDat[2] != 0) {
        *pFlg |= 0x80000000U >> (Bit & 0x1f);
    } else {
        *pFlg &= ~(0x80000000U >> (Bit & 0x1f));
    }
    return 0;
}

/* RE2 Sce_Water */
ULONG func_8005136C(void *pAot)
{
    G.pEm->Water = ((SHORT *)pAot)[0];
    return 0;
}

/* RE2 Sce_Save */
ULONG func_80051388(void *pAot)
{
    G.Back_func = func_800513CC;
    G.Back_func_rtn = 0;
    G.Save_area = *(UCHAR *)pAot;
    Game.Stop_bak = G.Unk2468;
    G.Unk2468 |= 0xff000000;
    return 0;
}

/* RE2 Save_entrance */
void func_800513CC(void)
{
    tagGLOBAL *pG = &G;

    switch (pG->Back_func_rtn) {
    case 0:
        if (func_8006CC8C(0x81, 0) >= 0) {
            Mess_set(0, 0, 1, 0xff000000);
            pG->Back_func_rtn = 1;
        } else {
            pG->Back_func = NULL;
            pG->Back_func_rtn = 0;
            pG->Unk2468 = Game.Stop_bak;
            Mess_set(0, 0, 0, 0xff000000);
        }
        break;
    case 1:
        if ((Moji.Mess_flg & 0x80) == 0) {
            if (pG->Unk7848 == 0) {
                Game.Unk1 = 1;
                pG->System_flg |= 0x80000;
            } else {
                pG->Unk2468 = Game.Stop_bak;
            }
            pG->Back_func = NULL;
            pG->Back_func_rtn = 0;
        }
        break;
    }
}

ULONG func_800514C4(void *pAot)
{
    D_800E01C4[0] = 2;
    G.Back_func = func_800514F0;
    G.Back_func_rtn = 0;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800514F0);

ULONG func_80051684(void *pAot)
{
    G.Unk21DC = (LONG)pAot;
    G.System_flg |= 0x2000;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800516A4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005190C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051B40);

/* RE2 Sce_Door */
ULONG func_80051CB0(void *pAot)
{
    if ((G.Status_flg & 0x400000) == 0 && G.Pl_damage_cnt == 0 && (G.Status_flg & 0x100) == 0 &&
        (D_800DBB58 & 0xC0) == 0) {
        Game.Doordemo_flg = 1;
        G.pSce_aot_on = pAot;
        G.Unk2468 |= 0xff000000;
        return 0;
    }
}

ULONG func_80051D28(void *pAot)
{
    G.Back_func = func_80051D60;
    G.Back_func_rtn = 0;
    Game.Stop_bak = G.Unk2468;
    G.Unk2468 |= 0xff000000;
    return 0;
}

void func_80051D60(void)
{
    tagGLOBAL *pG = &G;

    switch (pG->Back_func_rtn) {
    case 0:
        Mess_set(0, 0, 7, 0xff000000);
        pG->Back_func_rtn = 1;
        break;
    case 1:
        if ((Moji.Mess_flg & 0x80) == 0) {
            if (pG->Unk7848 == 0) {
                *(ULONG *)&pG->Pl_routine_0 = 1;
                pG->Unk2468 = Game.Stop_bak;
                if (pG->Pl_status_flg & 0x200) {
                    pG->Pl_status_flg &= ~0x200;
                    Mess_set(0, 0, 8, 0xff000000);
                } else {
                    Mess_set(0, 0, 9, 0xff000000);
                }
                pG->Back_func = NULL;
                pG->Back_func_rtn = 0;
            } else {
                pG->Back_func = NULL;
                pG->Back_func_rtn = 0;
                pG->Unk2468 = Game.Stop_bak;
            }
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051E48);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051F78);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052194);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800522D8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800523C4);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052588);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052644);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052BA4);

/* RE2 Nop / Break_point */
ULONG Nop(SCE_TASK *pSce)
{
    pSce->Data++;
    return 1;
}

/* RE2 Evt_end */
ULONG Evt_end(SCE_TASK *pSce)
{
    LONG Sub_ctr;
    UCHAR *D0;
    ULONG Ret;
    LONG Ifel_ctr;

    Sub_ctr = pSce->Sub_ctr;
    if (Sub_ctr != 0) {
        Sub_ctr--;
        D0 = pSce->Ret_addr[Sub_ctr];
        Ret = 1;
        Ifel_ctr = pSce->Ifel_ctr[Sub_ctr];
        pSce->Sub_ctr = Sub_ctr;
        pSce->Data = D0;
        pSce->pS_SP = pSce->Stack[Sub_ctr] + (Ifel_ctr + 1);
        return Ret;
    }
    pSce->Status = 0;
    return 2;
}

/* RE2 Evt_next */
ULONG Evt_next(SCE_TASK *pSce)
{
    pSce->Data++;
    return 2;
}

/* RE2 Evt_chain */
ULONG Evt_chain(SCE_TASK *pSce)
{
    Event_init(pSce, pSce->Data[3]);
    return 1;
}

/* RE2 Evt_exec */
ULONG Evt_exec(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Evt_no;
    ULONG Task_level;

    pData = pSce->Data;
    Task_level = pData[1];
    Evt_no = pData[3];
    pSce->Data = pData + 4;
    Event_exec(Task_level, Evt_no);
    return 1;
}

ULONG Sce_unk_80052EDC(SCE_TASK *pSce)
{
    UCHAR Flg;

    Flg = pSce->Data[1];
    if (Flg == 1) {
        Sce.Unk149B = Flg;
        Sce.Unk149D = pSce->Data[3];
        Sce.Unk149C = pSce->Task_level;
        pSce->Data += 4;
    } else {
        Sce.Unk149B = 0;
        Sce.Unk149C = 0;
        pSce->Data += 2;
    }
    return 1;
}

/* RE2 Evt_kill */
ULONG Evt_kill(SCE_TASK *pSce)
{
    Sce.Sce_task[pSce->Data[1]].Status = 0;
    pSce->Data += 2;
    return 1;
}

/* RE2 Ifel_ck */
ULONG Ifel_ck(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;

    pData = pSce->Data + 4;
    pSce->Data = pData;
    D0 = (ULONG)pData + ((USHORT *)pData)[-1];
    pSce->Ifel_ctr[pSce->Sub_ctr]++;
    *pSce->pS_SP++ = D0;
    return 1;
}

/* RE2 Else_ck */
ULONG Else_ck(SCE_TASK *pSce)
{
    pSce->pS_SP--;
    pSce->Data += ((USHORT *)pSce->Data)[1];
    pSce->Ifel_ctr[pSce->Sub_ctr]--;
    return 1;
}

/* RE2 Endif */
ULONG Endif(SCE_TASK *pSce)
{
    pSce->pS_SP--;
    pSce->Data += 2;
    pSce->Ifel_ctr[pSce->Sub_ctr]--;
    return 1;
}

/* RE2 Sleep */
ULONG Sleep(SCE_TASK *pSce)
{
    LONG Sub_idx;
    UCHAR Loop_cnt;
    UCHAR *pData;

    Sub_idx = pSce->Sub_ctr;
    pData = pSce->Data++;
    Loop_cnt = pSce->Loop_ctr[Sub_idx] + 1;
    pSce->Loop_ctr[Sub_idx] = Loop_cnt;
    pSce->Lcnt[Sub_idx][(s8)Loop_cnt] = ((USHORT *)pData)[1];
    return 1;
}

/* RE2 Sleeping */
ULONG Sleeping(SCE_TASK *pSce)
{
    LONG Sub_idx;

    Sub_idx = pSce->Sub_ctr;
    if (--pSce->Lcnt[Sub_idx][pSce->Loop_ctr[Sub_idx]] == 0) {
        pSce->Data += 3;
        pSce->Loop_ctr[Sub_idx]--;
    }
    return 2;
}

/* RE2 Wsleep */
ULONG Wsleep(SCE_TASK *pSce)
{
    pSce->Loop_ctr[pSce->Sub_ctr]++;
    pSce->Data++;
    return 1;
}

/* RE2 Wsleeping */
ULONG Wsleeping(SCE_TASK *pSce)
{
    if (((G.Status_flg & STAT_XA_VOICE) == 0) && ((Moji.Mess_flg & 0x80) == 0)) {
        pSce->Data++;
        pSce->Loop_ctr[pSce->Sub_ctr]--;
    }
    return 2;
}

/* Loop-header record of For: SHORT break offset at +2, SHORT loop count at +4 */
typedef struct {
    UCHAR Dummy[2];
    SHORT Brk_ofs;
    SHORT Lcnt;
} SCE_FOR_REC;

/* Loop-header record of While / Do: SHORT break offset at +2 */
typedef struct {
    UCHAR Dummy[2];
    SHORT Brk_ofs;
} SCE_BRK_REC;

/* RE2 For */
ULONG For(SCE_TASK *pSce)
{
    LONG Sub_idx;
    LONG Brk_ofs;
    s8 Loop_idx;
    UCHAR *pCmd;

    pCmd = pSce->Data;
    Sub_idx = pSce->Sub_ctr;
    if (((SCE_FOR_REC *)pCmd)->Lcnt != 0) {
        Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
        pSce->Loop_ctr[Sub_idx] = Loop_idx;
        Brk_ofs = ((SCE_FOR_REC *)pCmd)->Brk_ofs;
        pSce->Lcnt[Sub_idx][Loop_idx] = ((SCE_FOR_REC *)pCmd)->Lcnt;
        pCmd += 6;
        pSce->Data = pCmd;
        pSce->Lbreak[Sub_idx][Loop_idx] = pCmd + Brk_ofs;
        pSce->Lstack[Sub_idx][Loop_idx] = pCmd;
        pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
    } else {
        Brk_ofs = ((SCE_FOR_REC *)pCmd)->Brk_ofs;
        pSce->Data = pCmd + Brk_ofs + 6;
    }
    return 1;
}

/* RE2 For2: loop count taken from a scenario work variable (RE3 resumes at +5 but skips to +6) */
ULONG For2(SCE_TASK *pSce)
{
    LONG Sub_idx;
    LONG Brk_ofs;
    s8 Loop_idx;
    UCHAR *pCmd;
    SHORT *pWork;

    pCmd = pSce->Data;
    pWork = G.Sce_work;
    Sub_idx = pSce->Sub_ctr;
    if (pWork[pCmd[5]] != 0) {
        Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
        pSce->Loop_ctr[Sub_idx] = Loop_idx;
        Brk_ofs = ((SCE_BRK_REC *)pCmd)->Brk_ofs;
        pSce->Lcnt[Sub_idx][Loop_idx] = pWork[pCmd[5]];
        pCmd += 5;
        pSce->Data = pCmd;
        pSce->Lbreak[Sub_idx][Loop_idx] = pCmd + Brk_ofs;
        pSce->Lstack[Sub_idx][Loop_idx] = pCmd;
        pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
    } else {
        Brk_ofs = ((SCE_BRK_REC *)pCmd)->Brk_ofs;
        pSce->Data = pCmd + Brk_ofs + 6;
    }
    return 1;
}

/* RE2 Next */
ULONG Next(SCE_TASK *pSce)
{
    LONG Sub_idx;
    LONG Loop_idx;

    Sub_idx = pSce->Sub_ctr;
    Loop_idx = pSce->Loop_ctr[Sub_idx];
    if (--pSce->Lcnt[Sub_idx][Loop_idx] != 0) {
        pSce->Data = pSce->Lstack[Sub_idx][Loop_idx];
    } else {
        pSce->Data += 2;
        pSce->Loop_ctr[Sub_idx]--;
    }
    return 1;
}

/* RE2 While */
ULONG While(SCE_TASK *pSce)
{
    LONG Sub_idx;
    LONG Brk_ofs;
    s8 Loop_idx;
    UCHAR *pCmd;
    UCHAR *pData;

    Sub_idx = pSce->Sub_ctr;
    pCmd = pSce->Data;
    Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
    pSce->Loop_ctr[Sub_idx] = Loop_idx;
    Brk_ofs = ((SCE_BRK_REC *)pCmd)->Brk_ofs;
    pSce->Lstack[Sub_idx][Loop_idx] = pCmd;
    pData = pCmd + 4;
    pSce->Data = pData;
    pSce->Lbreak[Sub_idx][Loop_idx] = pData + Brk_ofs;
    pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
    if (While_main(pSce, pData, pData[-3]) != 0) {
        return 1;
    }
    pSce->Data = pSce->Lbreak[Sub_idx][Loop_idx];
    pSce->Loop_ctr[Sub_idx]--;
    return 1;
}

/* RE2 Ewhile */
ULONG Ewhile(SCE_TASK *pSce)
{
    s8 Sub_idx;

    Sub_idx = pSce->Sub_ctr;
    pSce->Data = pSce->Lstack[Sub_idx][pSce->Loop_ctr[Sub_idx]];
    pSce->Loop_ctr[Sub_idx]--;
    return 1;
}

/* RE2 Do */
ULONG Do(SCE_TASK *pSce)
{
    s8 Sub_idx;
    LONG Brk_ofs;
    UCHAR *pBody;
    s8 Loop_idx;

    Sub_idx = pSce->Sub_ctr;
    pBody = pSce->Data;
    Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
    pSce->Loop_ctr[Sub_idx] = Loop_idx;
    Brk_ofs = ((SCE_BRK_REC *)pBody)->Brk_ofs;
    pBody += 4;
    pSce->Data = pBody;
    pSce->Lstack[Sub_idx][Loop_idx] = pBody;
    pSce->Lbreak[Sub_idx][Loop_idx] = pBody + Brk_ofs;
    pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
    return 1;
}

/* RE2 Edwhile */
ULONG Edwhile(SCE_TASK *pSce)
{
    UCHAR *pData;
    LONG Sub_ctr;

    pData = pSce->Data + 2;
    pSce->Data = pData;
    Sub_ctr = pSce->Sub_ctr;
    if (While_main(pSce, pData, pData[-1]) != 0) {
        pSce->Data = pSce->Lstack[Sub_ctr][pSce->Loop_ctr[Sub_ctr]];
        return 1;
    } else {
        pSce->Loop_ctr[Sub_ctr]--;
        return 1;
    }
}

/* RE2 While_main: evaluate a condition list (terms joined by SHORT logic operators, 2-byte aligned) */
ULONG While_main(SCE_TASK *pSce, UCHAR *pData, UCHAR Ofs)
{
    ULONG Logic_op;
    ULONG Ret;
    ULONG Cmd_ret;
    UCHAR *pEnd;

    pEnd = pData + Ofs;
    Ret = Sce_jmp_tbl[*pData](pSce);
    pData = pSce->Data;
    pData = (UCHAR *)(((ULONG)pData + 1) & ~1);
    while (pData != pEnd) {
        Logic_op = *(USHORT *)pData;
        pData += 2;
        pSce->Data = pData;
        Cmd_ret = Sce_jmp_tbl[*pData](pSce);
        pData = pSce->Data;
        pData = (UCHAR *)(((ULONG)pData + 1) & ~1);
        if (Logic_op != 0) {
            Ret |= Cmd_ret;
        } else {
            Ret &= Cmd_ret;
        }
    }
    return Ret;
}

/* Switch head record: USHORT break offset at +2 */
typedef struct {
    UCHAR Dummy[2];
    USHORT Brk_ofs;
} SCE_SWITCH_REC;

/* Case record: USHORT offset to the next case at +2, case value at +4 */
typedef struct {
    UCHAR Dummy[2];
    USHORT Ofs;
    USHORT Val;
} SCE_CASE_REC;

/* RE2 Switch */
ULONG Switch(SCE_TASK *pSce)
{
    LONG Sub_idx;
    ULONG Var_no;
    s8 Loop_idx;
    UCHAR *pCase;
    ULONG Ofs;
    SHORT Val;
    USHORT Case_val;
    SHORT *pWork;

    Sub_idx = pSce->Sub_ctr;
    pCase = pSce->Data;
    Ofs = ((SCE_SWITCH_REC *)pCase)->Brk_ofs;
    Var_no = pCase[1];
    pCase += 4;
    Loop_idx = pSce->Loop_ctr[Sub_idx] + 1;
    pSce->Loop_ctr[Sub_idx] = Loop_idx;
    pSce->Lbreak[Sub_idx][Loop_idx] = pCase + Ofs;
    pSce->Loop_if_class[Sub_idx][Loop_idx] = pSce->Ifel_ctr[Sub_idx];
    pWork = G.Sce_work;
    Val = pWork[Var_no];
    while (1) {
        if (*pCase == 0x16) {
            pCase += 2;
            goto end;
        }
        if (*pCase == 0x17) {
            break;
        }
        Ofs = ((SCE_CASE_REC *)pCase)->Ofs;
        Case_val = ((SCE_CASE_REC *)pCase)->Val;
        pCase += 6;
        if (Val == Case_val) {
            goto end;
        }
        pCase += Ofs;
    }
    pSce->Loop_ctr[Sub_idx]--;
    pCase += 2;
end:
    pSce->Data = pCase;
    return 1;
}

/* RE2 Case */
ULONG Case(SCE_TASK *pSce)
{
    pSce->Data += 6;
    return 1;
}

/* RE2 Default */
ULONG Default(SCE_TASK *pSce)
{
    pSce->Data += 2;
    return 1;
}

/* RE2 Eswitch */
ULONG Eswitch(SCE_TASK *pSce)
{
    pSce->Loop_ctr[pSce->Sub_ctr]--;
    pSce->Data += 2;
    return 1;
}

/* Goto record: SHORT jump offset at +4 */
typedef struct {
    UCHAR Dummy[4];
    SHORT Jmp_ofs;
} SCE_GOTO_REC;

/* RE2 Goto */
ULONG Goto(SCE_TASK *pSce)
{
    ULONG Sub_idx;
    ULONG Ifel_val;
    UCHAR Loop_val;
    LONG Jmp_ofs;
    UCHAR *pData;

    Sub_idx = pSce->Sub_ctr;
    pData = pSce->Data;
    Ifel_val = pData[1];
    Loop_val = pData[2];
    Jmp_ofs = ((SCE_GOTO_REC *)pData)->Jmp_ofs;
    pSce->Ifel_ctr[Sub_idx] = Ifel_val;
    pSce->pS_SP = pSce->Stack[Sub_idx] + (Ifel_val + 1);
    pSce->Loop_ctr[Sub_idx] = Loop_val;
    pSce->Data = pData + Jmp_ofs;
    return 1;
}

/* RE2 Gosub */
ULONG Gosub(SCE_TASK *pSce)
{
    LONG Sub_idx;
    ULONG Sub_no;
    ULONG Sub_ofs;
    UCHAR *pData;

    pData = pSce->Data;
    Sub_idx = pSce->Sub_ctr;
    Sub_no = pData[1];
    pData += 2;
    pSce->Ret_addr[Sub_idx++] = pData;
    pSce->Ifel_ctr[Sub_idx] = -1;
    pSce->Loop_ctr[Sub_idx] = -1;
    pSce->pS_SP = pSce->Stack[Sub_idx];
    pData = Sce.pCScd;
    Sub_ofs = ((USHORT *)pData)[Sub_no];
    pSce->Sub_ctr = Sub_idx;
    pSce->Data = pData + Sub_ofs;
    return 1;
}

/* RE2 Return */
ULONG Return(SCE_TASK *pSce)
{
    s8 Sub_idx;

    Sub_idx = pSce->Sub_ctr - 1;
    pSce->Sub_ctr = Sub_idx;
    pSce->Data = pSce->Ret_addr[Sub_idx];
    pSce->pS_SP = pSce->Stack[Sub_idx] + (pSce->Ifel_ctr[Sub_idx] + 1);
    return 1;
}

/* RE2 Break */
ULONG Break(SCE_TASK *pSce)
{
    LONG Sub_idx;
    LONG Loop_idx;
    s8 Ifel_val;

    Sub_idx = pSce->Sub_ctr;
    Loop_idx = pSce->Loop_ctr[Sub_idx];
    pSce->Data = pSce->Lbreak[Sub_idx][Loop_idx];
    Ifel_val = pSce->Loop_if_class[Sub_idx][Loop_idx];
    pSce->Loop_ctr[Sub_idx] = Loop_idx - 1;
    pSce->Ifel_ctr[Sub_idx] = Ifel_val;
    return 1;
}

/* SCE opcode that only steps over itself (RE2 Rbj_reset slot?) */
ULONG Sce_nop_800538C8(SCE_TASK *pSce)
{
    pSce->Data++;
    return 1;
}

/* RE2 Work_copy */
ULONG Work_copy(SCE_TASK *pSce)
{
    ULONG Dst_ofs;
    UCHAR *pData;
    ULONG D0;
    ULONG D2;

    pData = pSce->Data;
    D0 = pData[1];
    Dst_ofs = pData[2];
    D2 = pData[3];
    pData += 4;
    pSce->Data = pData;
    if (D2 != 0) {
        *(SHORT *)(pData + Dst_ofs) = *(G.Sce_work + D0);
    } else {
        pData[Dst_ofs] = *(G.Sce_work + D0);
    }
    return 1;
}

/* Save record: SHORT value at +2 */
typedef struct {
    UCHAR Dummy[2];
    SHORT Set_val;
} SCE_SAVE_REC;

/* RE2 Save */
ULONG Save(SCE_TASK *pSce)
{
    ULONG Var_no;
    LONG Set_val;
    UCHAR *pData;
    SHORT *pWork;

    pData = pSce->Data;
    Var_no = pData[1];
    Set_val = ((SCE_SAVE_REC *)pData)->Set_val;
    pSce->Data = pData + 4;
    pWork = G.Sce_work;
    pWork[Var_no] = Set_val;
    return 1;
}

/* RE2 Copy */
ULONG Copy(SCE_TASK *pSce)
{
    ULONG Dst_no;
    ULONG Src_no;
    UCHAR *pData;
    SHORT *pWork;

    pData = pSce->Data;
    Dst_no = pData[1];
    Src_no = pData[2];
    pSce->Data = pData + 3;
    pWork = G.Sce_work;
    pWork[Dst_no] = pWork[Src_no];
    return 1;
}

/* Calc record: USHORT command word at +2 (low byte operator, high byte variable), SHORT immediate at +4 */
typedef struct {
    UCHAR Dummy[2];
    USHORT Cmd_word;
    SHORT Imm;
} SCE_CALC_REC;

/* RE2 Calc */
ULONG Calc(SCE_TASK *pSce)
{
    LONG Imm;
    ULONG Cmd_word;
    UCHAR *pData;

    pData = pSce->Data;
    Cmd_word = ((SCE_CALC_REC *)pData)->Cmd_word;
    Imm = ((SCE_CALC_REC *)pData)->Imm;
    pSce->Data = pData + 6;
    Calc_branch(Cmd_word & 0xFF, G.Sce_work + (Cmd_word >> 8), Imm);
    return 1;
}

/* RE2 Calc2 */
ULONG Calc2(SCE_TASK *pSce)
{
    LONG Op_no;
    LONG Dst_no;
    LONG Src_val;
    UCHAR *pData;
    SHORT *pWork;

    pWork = G.Sce_work;
    pData = pSce->Data;
    Op_no = pData[1];
    Dst_no = pData[2];
    Src_val = pWork[pData[3]];
    pSce->Data = pData + 4;
    Calc_branch(Op_no, pWork + Dst_no, Src_val);
    return 1;
}

/* RE2 Calc_branch */
void Calc_branch(ULONG Exp, SHORT *Ans, ULONG Num)
{
    switch (Exp) {
    case 0:
        *Ans += Num;
        break;
    case 1:
        *Ans -= Num;
        break;
    case 2:
        *Ans *= Num;
        break;
    case 3:
        *Ans /= Num;
        break;
    case 4:
        *Ans = (USHORT)*Ans % Num;
        break;
    case 5:
        *Ans |= Num;
        break;
    case 6:
        *Ans &= Num;
        break;
    case 7:
        *Ans ^= Num;
        break;
    case 8:
        *Ans = ~*Ans;
        break;
    case 9:
        *Ans = (USHORT)*Ans << Num;
        break;
    case 10:
        *Ans = (LONG)(USHORT)*Ans >> Num;
        break;
    case 11:
        *Ans >>= Num;
        break;
    }
}

/* Member_set record: SHORT value at +2 */
typedef struct {
    UCHAR Dummy[2];
    SHORT D0;
} SCE_MEMBER_SET_REC;

/* RE2 Member_set */
ULONG Member_set(SCE_TASK *pSce)
{
    Member_set_branch(pSce->pWork, pSce->Data[1], ((SCE_MEMBER_SET_REC *)pSce->Data)->D0);
    pSce->Data += 4;
    return 1;
}

/* RE2 Member_set2 */
ULONG Member_set2(SCE_TASK *pSce)
{
    Member_set_branch(pSce->pWork, pSce->Data[1], *(G.Sce_work + pSce->Data[2]));
    pSce->Data += 3;
    return 1;
}

/* RE2 Member_copy */
ULONG Member_copy(SCE_TASK *pSce)
{
    ULONG Dst_no;
    ULONG Mem_no;
    s8 *pData;

    pData = (s8 *)pSce->Data;
    Dst_no = pData[1];
    Mem_no = pData[2];
    pSce->Data = (UCHAR *)(pData + 3);
    *(G.Sce_work + Dst_no) = Load_member_branch(pSce->pWork, Mem_no);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", Member_cmp);

/* RE2 Member_calc */
ULONG Member_calc(SCE_TASK *pSce)
{
    LONG Imm;
    USHORT Cmd_word;
    SHORT *Ans;

    Cmd_word = ((SCE_CALC_REC *)pSce->Data)->Cmd_word;
    Imm = ((SCE_CALC_REC *)pSce->Data)->Imm;
    Ans = Load_member_addr_branch(pSce->pWork, Cmd_word >> 6);
    Calc_branch(Cmd_word & 0xFF, Ans, Imm);
    pSce->Data += 6;
    return 1;
}

/* RE2 Member_calc2 */
ULONG Member_calc2(SCE_TASK *pSce)
{
    SHORT *Ans;

    Ans = Load_member_addr_branch(pSce->pWork, pSce->Data[2]);
    Calc_branch(pSce->Data[1], Ans, *(G.Sce_work + pSce->Data[3]));
    pSce->Data += 4;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", Member_set_branch);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", Load_member_branch);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80053FE0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80053FEC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80053FF8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054004);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054010);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005401C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054028);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054034);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054040);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005404C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054058);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054064);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054070);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005407C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054088);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054094);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540A0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540AC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540B8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540C4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540D0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540DC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540E8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800540F4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054100);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005410C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054118);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054124);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054130);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005413C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054148);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054154);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054164);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054174);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054184);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054194);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800541A4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800541B4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800541C0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800541CC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800541D8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800541E4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", Load_member_addr_branch);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054384);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", Work_set);

/* Speed record: USHORT value at +2 */
typedef struct {
    UCHAR Dummy[2];
    USHORT Spd_val;
} SCE_SPEED_REC;

/* RE2 Speed_set */
ULONG Speed_set(SCE_TASK *pSce)
{
    ULONG Spd_idx;
    USHORT Spd_val;
    UCHAR *pData;

    pData = pSce->Data;
    Spd_idx = pData[1];
    Spd_val = ((SCE_SPEED_REC *)pData)->Spd_val;
    pSce->Data = pData + 4;
    (&pSce->Spd_x)[Spd_idx] = Spd_val;
    return 1;
}

/* RE2 Add_speed */
ULONG Add_speed(SCE_TASK *pSce)
{
    ULONG S0;
    ULONG S1;
    ULONG S2;
    ENEMY_WORK *pEm;

    pEm = pSce->pWork;
    S0 = pSce->Spd_x;
    S1 = pSce->Spd_y;
    S2 = pSce->Spd_z;
    pEm->Pos_x += S0;
    pEm->Pos_y += S1;
    pEm->Pos_z += S2;
    S0 = pSce->Dspd_x;
    S1 = pSce->Dspd_y;
    S2 = pSce->Dspd_z;
    pEm->Cdir_x += S0;
    pEm->Cdir_y += S1;
    pEm->Cdir_z += S2;
    pSce->Data++;
    return 1;
}

/* RE2 Add_aspeed */
ULONG Add_aspeed(SCE_TASK *pSce)
{
    pSce->Spd_x += pSce->Aspd_x;
    pSce->Spd_y += pSce->Aspd_y;
    pSce->Spd_z += pSce->Aspd_z;
    pSce->Dspd_x += pSce->Adspd_x;
    pSce->Dspd_y += pSce->Adspd_y;
    pSce->Dspd_z += pSce->Adspd_z;
    pSce->Data++;
    return 1;
}

ULONG func_80054628(SCE_TASK *pSce)
{
    SCE_CAMERA *pCam = &D_800E0150;
    LONG x, y, z;

    x = pSce->Spd_x;
    y = pSce->Spd_y;
    z = pSce->Spd_z;
    pCam->Pos[0] += x;
    pCam->Pos[1] += y;
    pCam->Pos[2] += z;
    x = pSce->Dspd_x;
    y = pSce->Dspd_y;
    z = pSce->Dspd_z;
    pCam->Tgt[0] += x;
    pCam->Tgt[1] += y;
    pCam->Tgt[2] += z;
    Set_view(pCam->Pos, pCam->Tgt);
    pSce->Data++;
    return 1;
}

/* RE2 Ck */
ULONG func_800546CC(SCE_TASK *pSce)
{
    UCHAR *pData;
    LONG D0;
    LONG D1;
    ULONG *pFlg;

    pData = pSce->Data;
    D0 = pData[1];
    D1 = ((USHORT *)pData)[1];
    pSce->Data = pData + 4;
    pFlg = Flg_addr[D0] + ((D1 >> 5) & 7);
    return ((*pFlg & (0x80000000U >> (D1 & 0x1f))) != 0) ^ ((D1 >> 8) == 0);
}

/* RE2 Set */
ULONG func_8005472C(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    LONG D1;
    LONG D2;
    ULONG *pFlg;
    ULONG Top;
    LONG Bit;

    pData = pSce->Data;
    D0 = pData[1];
    D1 = pData[2];
    D2 = pData[3];
    pSce->Data = pData + 4;
    Top = 0x80000000U;
    pFlg = Flg_addr[D0] + (D1 >> 5);
    Bit = D1 & 0x1f;
    switch (D2) {
    case 0:
        *pFlg &= ~(Top >> Bit);
        return 1;
    case 1:
        *pFlg |= Top >> Bit;
        return 1;
    case 7:
        *pFlg ^= Top >> Bit;
        return 1;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800547F0);

ULONG func_8005489C(SCE_TASK *pSce)
{
    pSce->Data++;
    func_800523C4();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800548C8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054960);

void func_800549C4(ULONG Cut)
{
    UCHAR Old;

    Old = G.Cut_no;
    G.Cut_no = Cut;
    D_800D442E[0] = 1;
    D_800E0175[0] = Old;
    Cut_change(Cut);
}

ULONG func_80054A04(SCE_TASK *pSce)
{
    if (pSce->Data[1] == 1) {
        G.Status_flg &= ~0x80;
    } else {
        G.Status_flg |= 0x80;
    }
    pSce->Data += 2;
    return 1;
}

ULONG func_80054A68(SCE_TASK *pSce)
{
    ULONG Cut_a;
    ULONG Cut_b;
    VCUT *pVc;

    pVc = G.pRoom->pVcut;
    Cut_a = pSce->Data[1];
    Cut_b = pSce->Data[2];
    while (pVc->Fcut != 0xff) {
        if (pVc->Fcut == Cut_a) {
            pVc->Fcut = Cut_b;
        } else if (pVc->Fcut == Cut_b) {
            pVc->Fcut = Cut_a;
        }
        if (pVc->Tcut == Cut_a) {
            pVc->Tcut = Cut_b;
        } else if (pVc->Tcut == Cut_b) {
            pVc->Tcut = Cut_a;
        }
        pVc++;
    }
    if (G.Cccut_next->Fcut == Cut_a) {
        Cut_change(Cut_b);
    }
    pSce->Data += 3;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054B48);

ULONG func_80054BC0(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pW;
    LONG x, y, z;

    pData = pSce->Data;
    pW = pSce->pWork;
    x = ((SHORT *)pData)[1];
    y = ((SHORT *)pData)[2];
    z = ((SHORT *)pData)[3];
    pW->Unk40[0] = x;
    pW->Pos_x = x;
    pW->Unk40[1] = y;
    pW->Pos_y = y;
    pW->Unk40[2] = z;
    pW->Pos_z = z;
    func_80034124((UCHAR *)pW + 0x80);
    pSce->Data = pData + 8;
    return 1;
}

ULONG func_80054C28(SCE_TASK *pSce)
{
    SHORT *pData;
    ENEMY_WORK *pW;
    LONG x, y, z;

    pData = (SHORT *)pSce->Data;
    pW = pSce->pWork;
    x = pData[1];
    y = pData[2];
    z = pData[3];
    pW->Cdir_x = x;
    pW->Cdir_y = y;
    pW->Cdir_z = z;
    pSce->Data = (UCHAR *)(pData + 4);
    return 1;
}

/* RE2 Vib_set0 */
ULONG func_80054C58(SCE_TASK *pSce)
{
    Pad_vib_set0(((USHORT *)pSce->Data)[1], ((USHORT *)pSce->Data)[2]);
    pSce->Data += 6;
    return 1;
}

/* RE2 Vib_set1 */
ULONG func_80054CA4(SCE_TASK *pSce)
{
    Pad_vib_set1(((USHORT *)pSce->Data)[1], pSce->Data[1], ((USHORT *)pSce->Data)[2]);
    pSce->Data += 6;
    return 1;
}

/* RE2 Vib_fade_set */
ULONG func_80054CF4(SCE_TASK *pSce)
{
    Pad_vib_fade_set(((USHORT *)pSce->Data)[2], pSce->Data[2], pSce->Data[3], ((USHORT *)pSce->Data)[3]);
    pSce->Data += 8;
    return 1;
}

ULONG func_80054D48(SCE_TASK *pSce)
{
    D_800E017F[0] = pSce->Data[1];
    pSce->Data += 2;
    return 1;
}

/* RE2 Message_on */
ULONG func_80054D74(SCE_TASK *pSce)
{
    ULONG Flg;

    Flg = ((USHORT *)pSce->Data)[2] << 16;
    Mess_set(0, ((USHORT *)pSce->Data)[1] | 0x2000, pSce->Data[1], Flg);
    G.Unk2468 |= Flg;
    pSce->Data += 6;
    return 1;
}

ULONG func_80054DEC(SCE_TASK *pSce)
{
    Moji.Mess_flg = 0;
    G.Status_flg &= ~0x10000000;
    pSce->Data++;
    return 1;
}

ULONG func_80054E28(SCE_TASK *pSce)
{
    func_80048974(pSce->Data[1]);
    pSce->Data += 2;
    return 1;
}

/* RE2 Sce_shake_on */
ULONG func_80054E70(SCE_TASK *pSce)
{
    LONG Amp;
    LONG Dir;
    LONG Line_no;
    LONG Ofs;

    Amp = (s8)pSce->Data[1];
    Dir = (s8)pSce->Data[2];
    Ofs = Main.pDb->Draw.ofs[1];
    Line.Rect.x = 0;
    Line.Rect.y = 0;
    Line.Rect.w = 320;
    Line.Rect.h = 240;
    line_mode_set(0, 7);
    for (Line_no = 0; Line_no != 0xF0; Line_no++) {
        line_trans(Line_no, Amp, Dir, Ofs);
    }
    ((LONG (*)(void))line_scr_end)();
    pSce->Data += 3;
    return 1;
}

/* RE2 Sce_line_start; the line functions are called as int-returning (no prototype in the original) */
ULONG func_80054F40(SCE_TASK *pSce)
{
    Line.Rect.x = 0;
    Line.Rect.y = 0;
    Line.Rect.w = 320;
    Line.Rect.h = 240;
    ((LONG (*)(ULONG, ULONG))line_mode_set)(pSce->Data[1], ((USHORT *)pSce->Data)[1]);
    pSce->Data += 4;
    return 1;
}

static inline ULONG Sce_skip(SCE_TASK *pSce, LONG n)
{
    pSce->Data += n;
    return 1;
}
/* RE2 Sce_line_main */
ULONG func_80054FAC(SCE_TASK *pSce)
{
    ((LONG (*)(LONG, LONG, LONG, LONG))line_trans)(pSce->Data[1], ((SHORT *)pSce->Data)[1], ((SHORT *)pSce->Data)[2], Main.pDb->Draw.ofs[1]);
    pSce->Data += 6;
    return 1;
}

/* RE2 Sce_line_end */
ULONG func_80055004(SCE_TASK *pSce)
{
    ((LONG (*)(void))line_scr_end)();
    pSce->Data++;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055038);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005518C);

ULONG func_800554C8(SCE_TASK *pSce)
{
    LONG Dummy[4];
    ULONG No;
    ULONG Ofs;

    No = pSce->Data[1];
    Ofs = ((USHORT *)pSce->Data)[1];
    G.Status_flg |= STAT_XA_VOICE;
    Xa_play(No, Ofs);
    pSce->Data += 4;
    return 1;
}

/* RE2 Movie_on */
ULONG func_80055520(SCE_TASK *pSce)
{
    func_800321C4(pSce->Data[1]);
    pSce->Data += 2;
    return 1;
}

ULONG func_80055568(SCE_TASK *pSce)
{
    UCHAR *pData;

    pData = pSce->Data;
    func_80078498(pData[2] - 1, pData[3], ((USHORT *)pData)[2]);
    pSce->Data = pData + 6;
    return 1;
}

ULONG func_800555BC(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG a;
    ULONG b;
    ULONG c;

    pData = pSce->Data;
    a = pData[2];
    b = pData[3];
    c = ((USHORT *)pData)[2];
    if (a >= 6) {
        a -= 5;
    }
    pSce->Data = pData + 6;
    return G.Unk7C84[D_800A0FDC[a - 1] + b] == c;
}

/* RE2 Keep_Item_ck */
ULONG func_80055620(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Id;

    pData = pSce->Data;
    Id = pData[1];
    pSce->Data = pData + 2;
    return func_8006CC8C(Id, 0) >= 0;
}

/* RE2 Sce_key_ck */
ULONG func_80055660(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Res;
    ULONG Mask;

    pData = pSce->Data;
    Res = pData[1];
    Mask = ((USHORT *)pData)[1];
    pSce->Data = pData + 4;
    if ((G.Key & Mask) != 0) {
        return Res;
    }
    return Res ^ 1;
}

/* RE2 Sce_trg_ck */
ULONG func_800556A0(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Res;
    ULONG Mask;

    pData = pSce->Data;
    Res = pData[1];
    Mask = ((USHORT *)pData)[1];
    pSce->Data = pData + 4;
    if ((G.Key_trg & Mask) != 0) {
        return Res;
    }
    return Res ^ 1;
}

/* RE2 Sca_id_set */
ULONG func_800556E0(SCE_TASK *pSce)
{
    UCHAR *pData;
    SCA_DATA *pSca;
    ULONG Slot;

    pData = pSce->Data;
    Slot = pData[1] + 1;
    pSca = (SCA_DATA *)G.pRoom->pSca + Slot;
    pSca->Id = (pSca->Id & 0x3f) | (((USHORT *)pData)[1] & 0xffc0);
    pSce->Data = pData + 4;
    return 1;
}

ULONG func_80055728(SCE_TASK *pSce)
{
    UCHAR *pData;
    SCE_OM_WORK *pOm;

    pData = pSce->Data;
    pOm = (SCE_OM_WORK *)G.Ob_model[pData[1]];
    if (pOm->Attr & 4) {
        *G.pUnk75AC[pOm->UnkC1] = 0xFF;
    }
    pSce->Data = pData + 2;
    return 1;
}

ULONG func_800557A0(SCE_TASK *pSce)
{
    UCHAR *pData;
    SCE_OM_WORK *pOm;
    USHORT *pFlg;

    pData = pSce->Data;
    pOm = (SCE_OM_WORK *)G.Ob_model[pData[1]];
    if ((pOm->Attr & 4) && (((USHORT *)pOm)[0x23] & 0x80)) {
        pFlg = G.pUnk75AC[pOm->UnkC1];
        if (*pFlg == 0xFF) {
            *pFlg = 0;
            Flg_off(G.Unk799C, ((UCHAR *)pOm)[0xC2]);
        } else {
            *pFlg = 0xFF;
            Flg_on(G.Unk799C, ((UCHAR *)pOm)[0xC2]);
        }
    }
    pSce->Data = pData + 2;
    return 1;
}

/* RE2 Light_pos_set2 */
ULONG func_80055884(SCE_TASK *pSce)
{
    UCHAR *pData;
    SHORT (*pPos)[3];
    ULONG *pLight;

    pLight = G.pRoom->pLight;
    pData = pSce->Data;
    pPos = ((LIGHT_CUT_DATA *)(pLight + (pLight[0] >> 2)))[pData[1]].Pos + pData[2];
    switch (pData[3]) {
    case 9:
        (*pPos)[0] = ((SHORT *)pData)[2];
        break;
    case 10:
        (*pPos)[1] = ((SHORT *)pData)[2];
        break;
    case 11:
        (*pPos)[2] = ((SHORT *)pData)[2];
        break;
    }
    pSce->Data = pData + 6;
    return 1;
}

/* RE2 Light_kido_set2 */
ULONG func_80055944(SCE_TASK *pSce)
{
    UCHAR *pData;
    LIGHT_CUT_DATA *pLc;
    ULONG *pLight;

    pLight = G.pRoom->pLight;
    pData = pSce->Data;
    pLc = (LIGHT_CUT_DATA *)(pLight + (pLight[0] >> 2));
    pLc[pData[2]].L[pData[3]] = ((USHORT *)pData)[2];
    pSce->Data = pData + 6;
    return 1;
}

/* RE2 Light_color_set2 */
ULONG func_800559A0(SCE_TASK *pSce)
{
    UCHAR *pData;
    UCHAR (*pCol)[3];
    ULONG *pLight;

    pLight = G.pRoom->pLight;
    pData = pSce->Data;
    pCol = ((LIGHT_CUT_DATA *)(pLight + (pLight[0] >> 2)))[pData[1]].Col + pData[2];
    (*pCol)[0] = pData[3];
    (*pCol)[1] = pData[4];
    (*pCol)[2] = pData[5];
    pSce->Data = pData + 6;
    return 1;
}

ULONG func_80055A18(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Id;
    ULONG Num;
    LONG i;
    UCHAR *p;

    pData = pSce->Data;
    i = 0;
    Id = pData[1];
    Num = pData[2];
    pSce->Data = pData + 3;
    while (1) {
        i = func_8006CC8C(Id, i);
        if (i == -1) {
            break;
        }
        p = G.pUnk7C7C + i * 4;
        if (((ITEM_TBL4 *)D_800A0514)[p[0]].Max >= p[1] + Num) {
            return 1;
        }
        i++;
    }
    i = func_8006CC8C(0, 0);
    return i != -1;
}

/* RE2 Dir_ck */
ULONG func_80055AF0(SCE_TASK *pSce)
{
    SHORT *pData;

    pData = (SHORT *)(pSce->Data += 8);
    return Goto00_ck(pSce->pWork, pData[-3], pData[-2], pData[-1]) == 0;
}

ULONG func_80055B30(SCE_TASK *pSce)
{
    pSce->Data++;
    G.System_flg |= 0x20000;
    return 1;
}

/* RE2 Aot_set */
ULONG func_80055B5C(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data += 0x20;
    return 1;
}

ULONG func_80055BBC(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data[3] |= 0x80;
    pSce->Data += 0x28;
    return 1;
}

ULONG func_80055C34(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data += 0x14;
    return 1;
}

ULONG func_80055C94(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data[3] |= 0x80;
    pSce->Data += 0x1C;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055D0C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055D7C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055E38);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056004);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800560B8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056178);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005624C);

ULONG func_8005632C(SCE_TASK *pSce)
{
    LONG Ret;

    Ret = func_80055E38((s8)pSce->Data[3], (s8)pSce->Data[4]);
    func_8001B894((pSce->Data[2] << 8) | pSce->Data[1], Ret);
    pSce->Data += 5;
    return 1;
}

ULONG func_80056394(SCE_TASK *pSce)
{
    func_8001B894((pSce->Data[1] << 16) | 0x1000000, 0);
    pSce->Data += 2;
    return 1;
}

ULONG func_800563E4(SCE_TASK *pSce)
{
    LONG v;
    ULONG i;

    if (pSce->Data[1] != 6) {
        v = func_80055E38((s8)pSce->Data[1], (s8)pSce->Data[2]);
        for (i = 0; i < 0x60; i++) {
            if (D_800BA8A8[i].UnkC0 == v) {
                D_800BA8A8[i].Unk1 = 0;
                D_800BA8A8[i].Unk0 = 0;
                D_800BA8A8[i].Unk24 = 0;
            }
        }
    } else {
        func_8001B894(0x2000000, 0);
    }
    pSce->Data += 3;
    return 1;
}

ULONG func_8005648C(SCE_TASK *pSce)
{
    UCHAR *pData;

    pData = pSce->Data;
    func_8001BA48((pData[2] << 8) | pData[1], func_80055E38((s8)pData[3], (s8)pData[4]),
                  pData[5] | (pData[6] << 16), pData[7] | (pData[8] << 16));
    pSce->Data = pData + 10;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056510);

/* RE2 Mizu_div_set */
ULONG func_80056950(SCE_TASK *pSce)
{
    POLY_GT4 *pPrim;
    ULONG Div_num;
    LONG i;
    LONG j;

    Div_num = pSce->Data[1];
    Sce.Mizu_div_max = Div_num;
    if (Div_num == 0) {
        pSce->Data += 2;
        return 1;
    }
    Sce.pMizu_div = (UCHAR *)G.pEm_mem;
    pPrim = (POLY_GT4 *)G.pEm_mem;
    G.pEm_mem = (ENEMY_WORK *)((UCHAR *)G.pEm_mem + Div_num * 0x548);
    do {
        for (i = 0; i < 13; i++) {
            for (j = 2; j != 0; j--) {
                pPrim->tag = 0x0C000000;
                *(ULONG *)&pPrim->r0 = 0x3E808080;
                *(ULONG *)&pPrim->r1 = 0x808080;
                *(ULONG *)&pPrim->r2 = 0x808080;
                *(ULONG *)&pPrim->r3 = 0x808080;
                pPrim++;
            }
        }
        Div_num--;
    } while (Div_num != 0);
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056A2C);

ULONG func_80056DC0(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pW;
    ULONG Move_no;
    ULONG Attr;

    pData = pSce->Data;
    pW = pSce->pWork;
    Move_no = pData[2];
    Attr = pData[3];
    *(ULONG *)&pW->Routine_0 = (pData[1] << 8) | 4;
    pW->Move_cnt = 0;
    pW->Move_no = Move_no;
    if (pW->Id == 0) {
        pW->Unk1A0 = Move_no;
        pW->Unk1A1 = 0;
    }
    pW->Unk144 = Attr;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    pW->Type |= 0x100;
    pSce->Data += 4;
    return 1;
}

ULONG func_80056E28(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pW;
    SHORT a, b;

    pData = pSce->Data;
    pW = pSce->pWork;
    a = pData[1] | 0x80;
    b = ((USHORT *)pData)[1];
    pW->Unk150 = a;
    pW->Unk152 = b;
    pSce->Data += 4;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056E5C);

ULONG func_80057008(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    ULONG v;

    v = pSce->Data[1];
    pW = pSce->pWork;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    *(ULONG *)&pW->Routine_0 = (v << 8) | 4;
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057040);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005709C);

ULONG func_80057210(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 1;
    if (pW->Id < 0x10) {
        pW->Type &= 0xFFF;
    }
    pW->Unk120 = 0;
    pW->Damage_cnt = 0;
    pW->Type &= 0xAEFF;
    pSce->Data++;
    func_80034124((UCHAR *)pW + 0x80);
    return 1;
}

ULONG func_80057280(SCE_TASK *pSce)
{
    ULONG R_no;

    R_no = *(ULONG *)&pSce->pWork->Routine_0;
    if (R_no == 0x1404) {
        *(ULONG *)&pSce->pWork->Routine_0 = pSce->R_no_bak;
    } else {
        pSce->R_no_bak = R_no;
        *(ULONG *)&pSce->pWork->Routine_0 = 0x1404;
    }
    pSce->Data++;
    return 1;
}

ULONG func_800572C8(SCE_TASK *pSce)
{
    ULONG Mode;
    USHORT v;

    Mode = pSce->Data[1];
    v = ((USHORT *)pSce->Data)[1];
    switch (Mode) {
    case 0:
        pSce->pWork->Unk144 |= v;
        break;
    case 1:
        pSce->pWork->Unk144 = v;
        break;
    case 2:
        pSce->pWork->Unk144 ^= v;
        break;
    }
    pSce->Data += 4;
    return 1;
}

ULONG func_8005734C(SCE_TASK *pSce)
{
    switch (pSce->Data[1]) {
    case 0:
        pSce->pWork->Add_dir = ((USHORT *)pSce->Data)[1];
        break;
    case 1:
        pSce->pWork->Unk14A = ((USHORT *)pSce->Data)[1];
        break;
    }
    pSce->Data += 4;
    return 1;
}

ULONG func_800573B0(SCE_TASK *pSce)
{
    pSce->pWork->Move_cnt = pSce->Data[1];
    pSce->Data += 2;
    return 1;
}

ULONG func_800573DC(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 1;
    pW->pUnk114 = (ENEMY_WORK *)((UCHAR *)&G + 0x248C);
    pW->Unk120 = 0;
    pW->Type = (pW->Type & 0xAEFF) | 0x400;
    G.Status_flg |= 0x80000000;
    pW->Life = G.Unk7852;
    pW->Max_life = 200;
    pSce->Data++;
    func_80034124((UCHAR *)pW + 0x80);
    return 1;
}

ULONG func_8005745C(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 0x604;
    pW->pUnk114 = pW;
    pW->Unk120 = 0x92;
    pW->Type = (pW->Type | 0x5000) & ~0x400;
    G.Status_flg &= 0x7FFFFFFF;
    pSce->Data++;
    return 1;
}

ULONG func_800574B8(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 0x604;
    pW->pUnk114 = pW;
    pW->Unk120 = 0x92;
    pW->Type |= 0x5000;
    pSce->Data++;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800574F4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800576C4);

/* RE2 Kage_set */
ULONG func_800578A0(SCE_TASK *pSce)
{
    KAGE_WORK9 *pK;
    UCHAR *pData;
    ULONG r, g, b;

    pData = pSce->Data;
    switch (pData[1]) {
    case 1:
        pK = G.pUnk265C->pKage_work;
        break;
    case 2:
        pK = G.pUnk2660->pKage_work;
        break;
    case 3:
        pK = ((struct { UCHAR pad[0x265C]; ENEMY_WORK *pWork[42]; } *)&G)->pWork[(s8)pData[2] + 2]->pKage_work;
        break;
    }
    r = pSce->Data[3];
    g = pSce->Data[4];
    b = pSce->Data[5];
    if (pK->Be_flg & 2) {
        Kage_work9_color_set(pK, r | (g << 8) | (b << 16));
    } else {
        Kage_work_color_set((KAGE_WORK *)pK, r | (g << 8) | (b << 16));
    }
    pK->Half_x = ((USHORT *)pSce->Data)[3];
    pK->Half_z = ((USHORT *)pSce->Data)[4];
    pK->Off_x = ((USHORT *)pSce->Data)[5];
    pK->Off_z = ((USHORT *)pSce->Data)[6];
    pSce->Data += 14;
    return 1;
}

/* RE2 Super_set */
ULONG func_80057A00(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pEm;
    ULONG Sup_val;
    ULONG Sup_no;
    ENEMY_WORK *pT;
    UCHAR *pOm;
    LONG x, y, z;
    tagGLOBAL *pG = &G;

    pData = pSce->Data;
    pEm = pSce->pWork;
    Sup_val = pData[2];
    Sup_no = pData[3];
    switch (Sup_val & 0xC0) {
    case 0:
        pEm->pSuper = &GsIDMATRIX;
        break;
    case 0x40:
        if (Sup_no & 0x80) {
            pEm->pSuper = &pG->Pl_pSin_parts_ptr[Sup_no & 0x7F].Workm;
        } else {
            pEm->pSuper = (MATRIX *)((UCHAR *)pG + 0x24AC);
        }
        break;
    case 0x80:
        if (Sup_no & 0x80) {
            pEm->pSuper = &((struct { UCHAR pad[0x265C]; ENEMY_WORK *pWork[42]; } *)pG)->pWork[Sup_val - 0x80]->pSin_parts_ptr[Sup_no & 0x7F].Workm;
        } else {
            pEm->pSuper = (MATRIX *)((UCHAR *)((struct { UCHAR pad[0x265C]; ENEMY_WORK *pWork[42]; } *)pG)->pWork[Sup_val - 0x80] + 0x20);
        }
        break;
    case 0xC0:
        if (Sup_no & 0x80) {
            pOm = pG->Ob_model[Sup_val - 0xC0];
            pEm->pSuper = &((struct { UCHAR pad[0xC4]; PARTS_W *pParts; } *)pOm)->pParts[Sup_no & 0x7F].Workm;
        } else {
            pEm->pSuper = (MATRIX *)(pG->Ob_model[Sup_val - 0xC0] + 0x20);
        }
        break;
    }
    x = ((SHORT *)pData)[2];
    y = ((SHORT *)pData)[3];
    z = ((SHORT *)pData)[4];
    pEm->Pos_x = x;
    pEm->Pos_y = y;
    pEm->Pos_z = z;
    x = ((SHORT *)pData)[5];
    y = ((SHORT *)pData)[6];
    z = ((SHORT *)pData)[7];
    pEm->Cdir_x = x;
    pEm->Cdir_y = y;
    pEm->Cdir_z = z;
    pSce->Data = pData + 0x10;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057BD0);

ULONG func_80057CF8(SCE_TASK *pSce)
{
    ENEMY_WORK *pPl;
    ULONG Wep;

    pPl = (ENEMY_WORK *)((UCHAR *)&G + 0x248C);
    Wep = D_800E01E8;
    pPl->Type = (pPl->Type & 0xFF00) | Wep;
    if (Wep != 0) {
        G.pUnk7C7C[0x128] = func_8006CC8C(Wep, 0);
        G.pUnk7C7C[0x129] = Wep;
    } else {
        G.pUnk7C7C[0x128] = 0xFF;
        G.pUnk7C7C[0x129] = 0;
    }
    if (Sce.Unk1489 != 0) {
        Weapon_cls(pPl);
        func_80043BE4(pPl);
        Sce.Unk1489 = 0;
    } else {
        Pl_weapon_ch_init(pPl);
    }
    pSce->Data++;
    return 1;
}

ULONG func_80057DD8(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    PARTS_W *p0;
    PARTS_W *p1;
    ULONG *pTmd;

    pW = pSce->pWork;
    p0 = &pW->pSin_parts_ptr[4];
    p1 = &pW->pSin_parts_ptr[15];
    pTmd = p0->pTmd;
    p0->pTmd = p1->pTmd;
    p1->pTmd = pTmd;
    pW->Unk4B ^= 1;
    pSce->Data++;
    return 1;
}

ULONG func_80057E28(SCE_TASK *pSce)
{
    D_800E01C4[0] = 0;
    Game.Unk1 = 1;
    G.Status_flg |= 0x200;
    Game.Stop_bak = G.Unk2468;
    pSce->Data++;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057E74);

ULONG func_80057EF4(SCE_TASK *pSce)
{
    func_8006CD68(func_8006CC8C(pSce->Data[1], 0));
    pSce->Data += 2;
    return 1;
}

ULONG func_80057F44(SCE_TASK *pSce)
{
    UCHAR v;

    v = pSce->Data[1];
    G.Status_flg |= 0x10000000;
    D_800C703E[0] = v;
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057F84);

ULONG func_8005863C(SCE_TASK *pSce)
{
    UCHAR *pData;
    PARTS_W *pP;
    ULONG Rgb;

    pData = pSce->Data;
    pP = &pSce->pWork->pSin_parts_ptr[pData[2]];
    pP->UnkA2 |= 0x20;
    pP->Be_flg |= 2;
    Rgb = pData[3] | (pData[4] << 8) | (pData[5] << 16);
    ((SHORT *)&pP->Unk74)[4] = 0;
    pP->Poly_rgb = Rgb;
    ((SHORT *)&pP->Unk74)[0] = ((USHORT *)pData)[3];
    ((SHORT *)&pP->Unk74)[2] = ((USHORT *)pData)[4];
    ((SHORT *)&pP->Unk74)[6] = ((USHORT *)pData)[5];
    ((SHORT *)&pP->Unk74)[1] = ((USHORT *)pData)[6];
    ((SHORT *)&pP->Unk74)[3] = ((USHORT *)pData)[7];
    pSce->Data += 16;
    return 1;
}

ULONG func_800586F4(SCE_TASK *pSce)
{
    UCHAR *pData;
    PARTS_W *pP;

    pData = pSce->Data;
    pP = &pSce->pWork->pSin_parts_ptr[pData[1]];
    pP->Unk96 = 0;
    pP->Be_flg |= 0xA;
    pP->Old_x = ((USHORT *)pData)[1];
    pP->Old_z = ((USHORT *)pData)[2];
    pP->Old_y = ((USHORT *)pData)[3];
    pP->Unk85 = pData[8];
    pP->Unk90 = ((USHORT *)pData)[5];
    pP->Unk92 = ((USHORT *)pData)[6];
    pP->Unk8E = ((USHORT *)pData)[7];
    pSce->Data += 16;
    return 1;
}

ULONG func_8005879C(SCE_TASK *pSce)
{
    switch (pSce->Data[1]) {
    case 0x34:
        D_800D1F8E[0] = 0x320;
        break;
    case 0x35:
        D_800D1F8E[0] = 0x320;
        break;
    case 0x36:
        D_800D1F8E[0] = 0x320;
        break;
    }
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058800);

void func_800588F0(SCE_TASK *pSce)
{
    D_800D442C[0] = pSce->Data[1];
    pSce->Data += 2;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058918);

ULONG func_800589FC(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    ULONG i;
    LONG nFloor;

    i = pSce->Data[1] + 2;
    pW = ((struct { UCHAR pad[0x265C]; ENEMY_WORK *pWork[42]; } *)&G)->pWork[i];
    nFloor = pW->nFloor;
    pW->Be_flg = 1;
    *(ULONG *)&pW->Routine_0 = 0;
    pW->Status_flg = 0;
    pW->Damage_cnt = 0;
    pW->UnkC6 = 0;
    pW->Timer0 = 0;
    pW->UnkDA = 0;
    pW->UnkE2 = 0;
    pW->UnkE3 = 0;
    pW->Unk122 = -(nFloor * 1800);
    if (pW->pKage_work != NULL) {
        pW->pKage_work->Be_flg = 0;
    }
    pW->pKage_work = NULL;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    pW->Unk14C = 0;
    pW->Unk14E = 0;
    pW->Water = 0;
    pW->Unk1C2 = 0;
    pW->Unk1C1 = 0;
    pW->pad1C0 = 0;
    pW->Unk1BC[1] = 0;
    pW->Unk1BC[0] = 0;
    pW->Unk1BA = 0;
    pW->Unk1B8 = 0;
    pW->Unk12A = 0x1000;
    pW->Unk1CA = 0;
    pW->Unk1C8 = 0;
    pW->Unk132 = 0;
    pW->Unk158 = 0;
    func_800261AC(pW, 0, pW->pUnk10C);
    pSce->Data += 2;
    return 1;
}

ULONG func_80058B00(SCE_TASK *pSce)
{
    UCHAR i;
    tagGLOBAL *pG = &G;

    if ((pG->Status_flg & 0x40000000) == 0) {
        pSce->Data += 4;
        return 1;
    }
    for (i = 0; i < 8; i++) {
        if ((USHORT)pG->Unk7E24[i] == 0) {
            pG->Unk7E24[i] = ((USHORT *)pSce->Data)[1];
            break;
        }
    }
    pSce->Data += 4;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058B84);

ULONG func_80058C24(SCE_TASK *pSce)
{
    Quake_add(((USHORT *)pSce->Data)[1], ((USHORT *)pSce->Data)[2]);
    pSce->Data += 6;
    return 1;
}

ULONG func_80058C70(SCE_TASK *pSce)
{
    ULONG x;

    St.St_type = 4;
    Game.Stop_bak = G.Unk2468;
    x = pSce->Data[2];
    G.Status_flg |= 0x200;
    Game.Unk1 = 1;
    St.UnkB8 = x;
    pSce->Data += 4;
    return 1;
}

ULONG func_80058CD0(SCE_TASK *pSce)
{
    UCHAR i;

    switch (D_800D1F94) {
    case 3:
    case 4:
        for (i = 0; i < 10; i++) {
            Flg_off(D_800D258C, i + 0x50);
        }
        break;
    case 5:
        for (i = 0; i < 0x14; i++) {
            Flg_off(D_800D258C, i + 0x61);
        }
        break;
    case 6:
        for (i = 0; i < 7; i++) {
            Flg_off(D_800D258C, i + 0x5A);
        }
        break;
    }
    pSce->Data++;
    return 1;
}

void func_80058DD4(ENEMY_WORK *pEm)
{
    SVECTOR Sv0;
    SVECTOR Sv1;
    ULONG Id;
    PARTS_W *pP;

    if (G.Unk2468 & 0x40000000) {
        return;
    }
    if (pEm->Damage_cnt & 0x7F) {
        pEm->Damage_cnt--;
    }
    Id = pEm->Id;
    if (Id == 0x5B || Id == 0x5F || Id >= 0x62) {
        if (Pl_work.Id < 8) {
            func_80059310(pEm);
        }
    }
    func_80059824(pEm);
    D_8009E6EC[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 0x1F4) {
        pP = pEm->pSin_parts_ptr;
        Sv0.vx = pP->Workm.t[0];
        Sv0.vy = pEm->Water;
        Sv0.vz = pP->Workm.t[2];
        Sv1.vx = 0x400;
        Sv1.vy = 0;
        Sv1.vz = 0;
        if ((pEm->Move_cnt & 3) == 2 && (USHORT)(pEm->Spd_x + 0x14) >= 0x29) {
            func_8001B484(0x20E, (((((LONG (*)(void))func_800102E8)() << 3) + 0x2000) << 16) | (USHORT)pEm->Cdir_y,
                          (LONG)&GsIDMATRIX, &Sv0, (ULONG)&Sv1);
        }
    }
    func_8001A248(pEm, pEm->Cdir_y);
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058F68);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059310);

void func_80059460(void)
{
    G.Unk3EE0[0] = func_80058DD4;
    G.Unk3EE0[1] = func_80058DD4;
    G.Unk3EE0[2] = func_80058DD4;
    G.Unk3EE0[3] = func_80058DD4;
    G.Unk3EE0[4] = func_80058DD4;
    G.Unk3EE0[5] = func_80058DD4;
    G.Unk3EE0[6] = func_80058DD4;
    G.Unk3EE0[7] = func_80058DD4;
    G.Unk3EE0[8] = func_80058DD4;
    G.Unk3EE0[9] = func_80058DD4;
    G.Unk3EE0[10] = func_80058DD4;
    G.Unk3EE0[11] = func_80058DD4;
    G.Unk3EE0[12] = func_80058DD4;
    G.Unk3EE0[13] = func_80058DD4;
    G.Unk3EE0[14] = func_80058DD4;
    G.Unk3EE0[15] = func_80058DD4;
    G.Unk3EE0[16] = func_80058DD4;
    G.Unk3EE0[17] = func_80058DD4;
    G.Unk3EE0[18] = func_80058DD4;
    G.Unk3EE0[19] = func_80058DD4;
    G.Unk3EE0[20] = func_80058DD4;
    G.Unk3EE0[21] = func_80058DD4;
    G.Unk3EE0[22] = func_80058DD4;
    G.Unk3EE0[23] = func_80058DD4;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800594D4);

void func_800597F0(UCHAR *p)
{
    if (p[0x1DE] != 0) {
        p[0x1DE]--;
    }
    func_80059A20(p);
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059824);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059A20);

void func_80059BC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E7DC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059C44(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E87C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059CC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E8CC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059D4C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E91C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059DD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E96C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059E54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E9BC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059ED8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EA0C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059F5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EA5C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_80059FE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EAAC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8005A064(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EAFC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8005A0E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EB4C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8005A16C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EB9C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8005A1F0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EBEC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void func_8005A274(ENEMY_WORK *pEm, ULONG Type)
{
    ENEMY_WORK *pT;

    pT = D_800E01A8;
    if (((UCHAR *)pEm->Unk1CC)[0x12] != 0) {
        return;
    }
    if ((Type & 0xF) == 0xA) {
        pT = func_8005F7E8(pEm);
        if (pT == NULL) {
            return;
        }
        if (pT->Life < 0 || (pT->Type & 0x2000)) {
            pT = func_8005F8F0(pEm, pT);
        }
    }
    if (pT == NULL || pT->Life < 0) {
        return;
    }
    if ((pT->Type & 0x2000) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        pEm->pEnemy_ptr = pT;
        D_8009E70D[0] = 0;
        return;
    }
    if (pT->Type & 1) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        pEm->pEnemy_ptr = pT;
        D_8009E70D[0] = 2;
    }
}

void func_8005A35C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    USHORT Dist;

    pT = D_800E01A8;
    if (((UCHAR *)pEm->Unk1CC)[0x12] != 0) {
        return;
    }
    if (pT != NULL && pT->Life >= 0) {
        if ((pT->Type & 0x2000) == 0) {
            goto set;
        }
        if (pT->Type & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x501;
            pEm->pEnemy_ptr = pT;
            D_8009E70D[0] = 2;
        }
    }
    pT = D_800E01AC;
    Dist = D_800E01B6;
    if (pT == NULL || pT->Life < 0 || (pT->Type & 0x2000) || Dist >= 0xBB8) {
        return;
    }
set:
    *(ULONG *)&pEm->Routine_0 = 0x501;
    pEm->pEnemy_ptr = pT;
    D_8009E70D[0] = 0;
}

void func_8005A418(ENEMY_WORK *pEm, ULONG Type)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    SHORT Dy;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if ((Type & 0xF) == 0xA) {
        pT = func_8005F7E8();
        if (pT == NULL) {
            return;
        }
        if (pT->Life < 0) {
            pT = func_8005F8F0(pEm, pT);
        }
    }
    if (pT == NULL) {
        return;
    }
    Dy = pT->Pos_y - pEm->Pos_y;
    if (pT->Life >= 0 && Dist < 0x5DC) {
        if (Dy >= -0x1387) {
            if (Dy < 0x7D0) {
                *(ULONG *)&pEm->Routine_0 = 0x501;
                pEm->pEnemy_ptr = pT;
            }
        }
    }
}

void func_8005A4D8(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    SHORT Dy;

    pT = D_800E01A8;
    if (pT != NULL) {
        Dy = pT->Pos_y - pEm->Pos_y;
        if (pT->Life >= 0 && (pT->Type & 0x2000) == 0) {
            if (Dy >= -0x1387) {
                if (Dy < 0x7D0) {
                    goto set;
                }
            }
        }
    }
    pT = D_800E01AC;
    Dist = D_800E01B6;
    if (pT == NULL) {
        return;
    }
    Dy = pT->Pos_y - pEm->Pos_y;
    if (pT->Life >= 0 && (pT->Type & 0x2000) == 0 && Dist < 0x7D0) {
        if (Dy >= -0x1387) {
            if (Dy < 0x7D0) {
set:
                *(ULONG *)&pEm->Routine_0 = 0x501;
                pEm->pEnemy_ptr = pT;
            }
        }
    }
}

void func_8005A5AC(void) {
}

void func_8005A5B4(void) {
}

void func_8005A5BC(void) {
}

void func_8005A5C4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A5CC);

void func_8005A844(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    LONG Life;
    LONG r;
    ULONG d;

    pT = D_800E01A8;
    if (pEm->Routine_2 == 0) {
        Life = pEm->Life;
        D_8009E70C[0] = 0;
        if (Life < 0x65) {
            D_8009E70C[0] = 1;
        }
        if (pEm->Status_flg & 0x200) {
            D_8009E70C[0] = 1;
        }
        if (Life < 0x15) {
            D_8009E70C[0] = 2;
        }
    }
    if (pT != NULL) {
        D_8009ECAC[D_8009EC3C[pT->Id - 0x10]](pEm, pEm->Type);
    }
    r = Goto00_ck(pEm, ((SHORT *)pEm->Unk1CC)[0], ((SHORT *)pEm->Unk1CC)[1], 0x400);
    d = ((USHORT *)pEm->Unk1CC)[3];
    if (d > 0xBB8) {
        if (r == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0xC01;
        }
    } else if (d > 0x1770) {
        if (r == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0xD01;
        }
    }
}

void func_8005A948(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    LONG Life;
    LONG r;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if (pEm->Routine_2 == 0) {
        Life = pEm->Life;
        D_8009E70C[0] = 0;
        if (Life < 0x65) {
            D_8009E70C[0] = 1;
        }
        if (pEm->Status_flg & 0x200) {
            D_8009E70C[0] = 1;
        }
        if (Life < 0x15) {
            D_8009E70C[0] = 2;
        }
    }
    if (pT != NULL && (LONG)pEm->Be_flg < 0) {
        *(ULONG *)&pEm->Routine_0 = 0x1001;
        pEm->pEnemy_ptr = pT;
        return;
    }
    if (G.Unk75EA & 3) {
        *(ULONG *)&pEm->Routine_0 = 0xF01;
        ((USHORT *)pEm->Unk1CC)[10] &= ~2;
        if (G.Unk75EA & 1) {
            ((USHORT *)pEm->Unk1CC)[10] |= 2;
        }
        pEm->Timer0 = 0x14;
        pEm->UnkE3 = 0;
        return;
    }
    if (pT != NULL) {
        if (Dist < 0x9C4 && (pT->Type & 0x2000) == 0) {
            *(ULONG *)&pEm->Routine_0 = 0xF01;
            ((USHORT *)pEm->Unk1CC)[10] &= ~2;
            if (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 1) < 0) {
                ((USHORT *)pEm->Unk1CC)[10] |= 2;
            }
            pEm->Timer0 = 0x14;
            pEm->UnkE3 = 0;
            return;
        }
        if (pT->Life >= 0 && (pT->Type & 0x2000) == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x501;
            pEm->pEnemy_ptr = pT;
            D_8009E70D[0] = 0;
            return;
        }
    }
    r = Goto00_ck(pEm, ((SHORT *)pEm->Unk1CC)[0], ((SHORT *)pEm->Unk1CC)[1], 0x400);
    if (((USHORT *)pEm->Unk1CC)[3] >= 0x5DD) {
        if (r == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0xD01;
        }
    }
}

void func_8005AB14(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    LONG Life;

    pT = D_800E01A8;
    if (pEm->Routine_2 == 0) {
        Life = pEm->Life;
        D_8009E70C[0] = 0;
        if (Life < 0x65) {
            D_8009E70C[0] = 1;
        }
        if (pEm->Status_flg & 0x200) {
            D_8009E70C[0] = 1;
        }
        if (Life < 0x15) {
            D_8009E70C[0] = 2;
        }
    }
    if (pT != NULL) {
        D_8009ECAC[D_8009EC3C[pT->Id - 0x10]](pEm, pEm->Type);
    }
}

void func_8005ABC4(ENEMY_WORK *pEm)
{
    ULONG v;

    switch (pEm->Routine_2) {
    case 0:
        v = D_8009CDE0[D_8009E70C[0]];
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x3C;
        *(ULONG *)&pEm->Move_no = v + 0x70000;
    case 1:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (pEm->UnkBA & 0x8000) {
            if (--pEm->Timer0 == 0) {
                *(ULONG *)&pEm->Routine_0 = 0xE01;
            }
        } else {
            pEm->Timer0 = 0x3C;
        }
        break;
    }
}

void func_8005AC9C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    USHORT Dist;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if (((UCHAR *)pEm->Unk1CC)[0x12] != 0 || pT == NULL || pT->Life < 0) {
        return;
    }
    if ((pT->Type & 0x2000) == 0 && Dist < 0x9C4) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        pEm->pEnemy_ptr = pT;
        D_8009E70D[0] = 0;
        return;
    }
    if (pT->Life >= 0 && (pT->Type & 1) && (pT->Type & 0x2000)) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        pEm->pEnemy_ptr = pT;
        D_8009E70D[0] = 2;
    }
}

void func_8005AD48(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    SHORT Dy;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if (pT != NULL) {
        Dy = pT->Pos_y - pEm->Pos_y;
        if (pT->Life >= 0 && (pT->Type & 0x2000) == 0 && Dist < 0x9C4) {
            if (Dy >= -0x1387) {
                if (Dy < 0x7D0) {
                    *(ULONG *)&pEm->Routine_0 = 0x501;
                    pEm->pEnemy_ptr = pT;
                }
            }
        }
    }
}

void func_8005ADBC(void) {
}

void func_8005ADC4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005ADCC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B0D0);

void func_8005B2CC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    ULONG D;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if (pT != NULL) {
        if ((LONG)pEm->Be_flg < 0) {
            *(ULONG *)&pEm->Routine_0 = 0x1001;
            pEm->pEnemy_ptr = pT;
            return;
        }
        if (Dist < 0x9C4 && (pT->Type & 0x2000) == 0) {
            *(ULONG *)&pEm->Routine_0 = 0xF01;
            ((USHORT *)pEm->Unk1CC)[10] &= ~2;
            if (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 1) < 0) {
                ((USHORT *)pEm->Unk1CC)[10] |= 2;
            }
            pEm->Timer0 = 0x14;
            pEm->UnkE3 = 0;
            return;
        }
        if (pT->Life >= 0 && (pT->Type & 0x2000) == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x501;
            pEm->pEnemy_ptr = pT;
            D_8009E70D[0] = 0;
            return;
        }
    }
    if (pEm->UnkAE & 0x40) {
        if (func_800632B8(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0x1101;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x21101;
        }
    }
    D = ((USHORT *)pEm->Unk1CC)[3];
    if (D < 0x12C) {
        *(ULONG *)&pEm->Routine_0 = 0xB01;
        return;
    }
    if (D >= 0x5DD) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
        return;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x28);
}

void func_8005B438(ENEMY_WORK *pEm)
{
    ULONG v;
    ULONG b;

    switch (pEm->Routine_2) {
    case 0:
        v = D_8009CDE0[D_8009E70C[0] + 3];
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x3C;
        pEm->UnkDA = 0x3C;
        *(ULONG *)&pEm->Move_no = v + 0x70000;
    case 1:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            func_800776B0(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        if (((USHORT *)pEm->Unk1CC)[3] < 0x5DC && (pEm->UnkAE & 0x30)) {
            if (--pEm->Timer0 == 0) {
                *(ULONG *)&pEm->Routine_0 = 0x401;
            }
        } else {
            pEm->Timer0 = 0x3C;
        }
        if ((LONG)pEm->Unk154 < 0x5DC && (pEm->UnkAE & 0x30)) {
            if (--pEm->UnkDA == 0) {
                *(ULONG *)&pEm->Routine_0 = 0x401;
            }
        } else {
            pEm->UnkDA = 0x3C;
        }
        break;
    }
    if (pEm->Water < pEm->Unk122 - 0x1F4) {
        pEm->Spd_x -= pEm->Spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B614);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B824);

void func_8005B9D0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    USHORT v;

    pT = D_800E01A8;
    if (pT != NULL && (LONG)pEm->Be_flg < 0) {
        *(ULONG *)&pEm->Routine_0 = 0x1001;
        pEm->pEnemy_ptr = pT;
        return;
    }
    if (G.Unk75EA & 3) {
        *(ULONG *)&pEm->Routine_0 = 0xF01;
        v = ((USHORT *)pEm->Unk1CC)[10] & ~2;
        ((USHORT *)pEm->Unk1CC)[10] = v;
        if (G.Unk75EA & 1) {
            ((USHORT *)pEm->Unk1CC)[10] = v | 2;
        }
        pEm->Timer0 = 0x14;
        pEm->UnkE3 = 0;
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_800632B8(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0x1201;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x21201;
        }
    }
    if (((USHORT *)pEm->Unk1CC)[3] < 0x3E8) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        return;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x40);
}

void func_8005BAD4(ENEMY_WORK *pEm)
{
    ULONG v;
    ULONG b;

    switch (pEm->Routine_2) {
    case 0:
        v = D_8009CDE0[D_8009E70C[0] + 6];
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Move_no = v + 0x70000;
    case 1:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            func_800776B0(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        break;
    }
    if (pEm->UnkCA != 0) {
        pEm->Spd_x >>= 1;
    }
    if (pEm->Water < pEm->Unk122 - 0x1F4) {
        pEm->Spd_x -= pEm->Spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

void func_8005BC28(ENEMY_WORK *pEm)
{
    LONG r;

    if (pEm->Routine_2 == 0) {
        r = Goto00_ck(&Pl_work, pEm->Pos_x, pEm->Pos_z, 1);
        pEm->UnkDA = r * 0x60;
        if ((SHORT)(r * 0x60) == 0) {
            pEm->UnkDA = 0x60;
        }
        pEm->UnkE2 = 0;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_800632B8(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0x1301;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x21301;
        }
    }
    if (Goto01_ck(pEm, Pl_work.Cdir_y, 0x200) == 0 && pEm->UnkE2 >= 6) {
        *(ULONG *)&pEm->Routine_0 = 1;
    }
    if (pEm->UnkE2 >= 0x79) {
        *(ULONG *)&pEm->Routine_0 = 1;
    }
    pEm->Cdir_y += pEm->UnkDA;
}

void func_8005BD24(ENEMY_WORK *pEm)
{
    LONG r;
    ULONG b;

    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70009;
        if (D_8009E70C[0] != 0) {
            *(ULONG *)&pEm->Move_no = 0x7000A;
        }
        pEm->Routine_2 = 1;
        pEm->UnkE2 = 0;
        r = Goto00_ck(&Pl_work, pEm->Pos_x, pEm->Pos_z, 1);
        pEm->UnkDA = r << 7;
        if ((SHORT)(r << 7) == 0) {
            pEm->UnkDA = 0x80;
        }
        pEm->Timer0 = 0x3C;
    case 1:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            func_800776B0(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        pEm->UnkE2++;
        break;
    }
    if (pEm->UnkCA != 0) {
        pEm->Spd_x >>= 1;
    }
    Add_speedXZ(pEm, 0);
}

void func_8005BE84(ENEMY_WORK *pEm)
{
    if (Goto00_ck(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x50) == 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    } else {
        Goto00(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x50);
    }
}

void func_8005BEE8(ENEMY_WORK *pEm)
{
    ULONG v;
    ULONG b;

    switch (pEm->Routine_2) {
    case 0:
        v = D_8009CDE0[D_8009E70C[0] + 3];
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        *(ULONG *)&pEm->Move_no = v + 0x70000;
    case 1:
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            func_800776B0(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        break;
    }
}

void func_8005BFC0(ENEMY_WORK *pEm)
{
    D_8009ECDC[pEm->Routine_2](pEm);
}

void func_8005BFFC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    pT = D_800E01A8;
    func_8005C098(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    if (pT != NULL && (LONG)pEm->Be_flg < 0) {
        *(ULONG *)&pEm->Routine_0 = 0x1001;
        pEm->pEnemy_ptr = pT;
        ((USHORT *)pEm->Unk1CC)[10] &= ~2;
        return;
    }
    if (pEm->Routine_2 != 0 || pEm->UnkCA == 0) {
        func_80019318(pEm, 0, 1);
    }
}

void func_8005C098(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)((UCHAR *)D_8009ECF8 + (pEm->Routine_2 << 2)))[0](pEm);
}

void func_8005C0D4(void) {
}

void func_8005C0DC(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70000;
        *(ULONG *)&pEm->Unk1A0 = 0x70000;
        pEm->pEnemy_ptr = D_800E01A8;
        if (pEm->pEnemy_ptr == NULL) {
            *(ULONG *)&pEm->Routine_0 = 1;
            return;
        }
        if ((pEm->Type & 0xF) == 9) {
            pEm->pEnemy_ptr = func_8005F7E8(pEm);
        }
        pEm->Unk120 |= 0x12;
        ((USHORT *)pEm->Unk1CC)[2] |= 0x4000;
        ((UCHAR *)pEm->Unk1CC)[0x13] = (func_800102E8() & 3) + 2;
    case 1:
        pT = pEm->pEnemy_ptr;
        if (pT != pEm) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xC4);
        }
        if (func_80026BE8(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200) != 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
            pEm->UnkCA = 7;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->Unk180[1], pEm->Unk180[0], 0x200);
        func_80079868(pEm, 0x200);
        break;
    }
}

void func_8005C210(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    pT = pEm->pEnemy_ptr;
    if (pT->Damage_cnt == 0 && pT->Life >= 0) {
        if ((pT->Type & 0x2000) == 0) {
            if ((s8)--pEm->UnkE3 >= 0) {
                *(USHORT *)&pEm->Routine_2 = 2;
                return;
            }
            *(USHORT *)&pEm->Routine_2 = 2;
            pEm->UnkE3 = 0xD;
            return;
        }
        if ((pT->Type & 1) && D_8009E70D[0] == 2) {
            if ((s8)--pEm->UnkE3 >= 0) {
                *(USHORT *)&pEm->Routine_2 = 2;
                return;
            }
            *(USHORT *)&pEm->Routine_2 = 2;
            pEm->UnkE3 = 0xD;
            return;
        }
        if (pT->Life >= 0 && (pT->Type & 0x2000) == 0) {
            return;
        }
    }
    *(USHORT *)&pEm->Routine_2 = 3;
}

void func_8005C2F8(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    pT = pEm->pEnemy_ptr;
    if ((LONG)(pT->Unk158 & 0x3FFFFFFF) < 0x9C4 && (pEm->UnkAE & 0x30) == 0 && (LONG)pEm->Unk154 >= 0xBB9) {
        *(ULONG *)&pEm->Routine_0 = 0xF01;
        if (func_800102E8() & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        }
        pEm->Timer0 = 0x14;
        pEm->UnkE3 = 0;
        return;
    }
    if (pT->Life < 0 || (pT->Type & 0x2000)) {
        *(USHORT *)&pEm->Routine_2 = 3;
        return;
    }
    if ((LONG)(pT->Unk158 & 0x3FFFFFFF) < 0xFA0) {
        ((UCHAR *)pEm->Unk1CC)[0x13]--;
    }
    if (((UCHAR *)pEm->Unk1CC)[0x13] != 0) {
        *(USHORT *)&pEm->Routine_2 = 2;
    } else {
        *(USHORT *)&pEm->Routine_2 = 3;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C3F0);

void func_8005C638(ENEMY_WORK *pEm)
{
    ULONG v;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        v = D_8009E70D[0] * 2 + 0x70002;
        *(ULONG *)&pEm->Move_no = v;
        *(ULONG *)&pEm->Unk1A0 = v;
        G.Unk75E8 |= 0x18;
        pEm->Timer0 = 0x1E;
        pEm->UnkDA = 0;
        if ((pEm->Type & 0xF) == 0xF) {
            pEm->UnkDA = 0x14;
        }
    }
    func_80026BE8(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200);
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, pEm->Unk180[1], pEm->Unk180[0], 0x200);
    func_80079868(pEm, 0x200);
    if (--pEm->Timer0 == 0) {
        *(USHORT *)&pEm->Routine_2 = 3;
    }
}

void func_8005C730(void) {
}

void func_8005C738(ENEMY_WORK *pEm)
{
    LONG Dummy[4];
    ULONG v;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        v = D_8009E70D[0] * 2 + 0x30001;
        *(ULONG *)&pEm->Move_no = v;
        *(ULONG *)&pEm->Unk1A0 = v;
        ((USHORT *)pEm->Unk1CC)[2] &= 0xBFFF;
    }
    D_8009E720[D_8009E710[pEm->Id - 0x50]](pEm);
    if (func_80026BE8(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200) != 0) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, pEm->Unk180[1], pEm->Unk180[0], 0x200);
    func_80079868(pEm, 0x200);
}

void func_8005C81C(void) {
}

void func_8005C824(ENEMY_WORK *pEm)
{
    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70000;
        *(ULONG *)&pEm->Unk1A0 = 0x70000;
    }
    if (func_80026BE8(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x10200) != 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
        ((UCHAR *)pEm->Unk1CC)[0x12] = 0x1E;
        pEm->Unk120 &= 0xED;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, pEm->Unk180[1], pEm->Unk180[0], 0x10200);
    func_80079868(pEm, 0x200);
}

void func_8005C8D0(void) {
}

void func_8005C8D8(ENEMY_WORK *pEm)
{
    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70007;
        *(ULONG *)&pEm->Unk1A0 = 0x70007;
    }
    if (func_80026BE8(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200) != 0) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, pEm->Unk180[1], pEm->Unk180[0], 0x200);
    func_80079868(pEm, 0x200);
}

void func_8005C96C(void) {
}

void func_8005C974(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
    case 1:
        pEm->Be_flg |= 0x2000000;
        pT = pEm->pEnemy_ptr;
        Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xD8);
        if (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x10) == 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        break;
    }
}

void func_8005CA0C(void) {
}

void func_8005CA14(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70002;
        *(ULONG *)&pEm->Unk1A0 = 0x70002;
        *(ULONG *)&pEm->Move_no = 0x70000;
        *(ULONG *)&pEm->Unk1A0 = 0x70000;
        pEm->Unk120 |= 0x12;
        D_8009E70D[0] = 0;
        ((USHORT *)pEm->Unk1CC)[2] |= 0x4000;
        pEm->Unk120 |= 0x12;
        ((UCHAR *)pEm->Unk1CC)[0x13] = (func_800102E8() & 3) + 2;
    case 1:
        Goto00(pEm, pEm->pEnemy_ptr->Pos_x, pEm->pEnemy_ptr->Pos_z, 0xC4);
        func_80026BE8(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200);
        if (pEm->UnkCA == 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->Unk180[1], pEm->Unk180[0], 0x200);
        func_80079868(pEm, 0x200);
        break;
    }
}

void func_8005CB24(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005CB2C);

void func_8005CBF0(void) {
}

void func_8005CBF8(void) {
}

void func_8005CC00(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005CC08);

void func_8005CE7C(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005CE84);

void func_8005D138(void) {
}

void func_8005D140(void) {
}

void func_8005D148(ENEMY_WORK *pEm)
{
    if (Goto01_ck(pEm, G.Pl_cdir_y, 0x50) == 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    } else {
        Goto01(pEm, G.Pl_cdir_y, 0xA0);
    }
}

void func_8005D1A4(ENEMY_WORK *pEm)
{
    if (Goto01_ck(pEm, G.Pl_cdir_y + 0x800, 0x50) == 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    } else {
        Goto01(pEm, G.Pl_cdir_y + 0x800, 0xA0);
    }
}

void func_8005D208(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    pT = D_800E01A8;
    if (pT == NULL) {
        *(ULONG *)&pEm->Routine_0 = 1;
    } else if ((LONG)pEm->Be_flg < 0) {
        *(ULONG *)&pEm->Routine_0 = 0x1001;
        pEm->pEnemy_ptr = pT;
    } else if (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x50) == 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    } else {
        Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xA0);
    }
}

void func_8005D290(void) {
}

void func_8005D298(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    ULONG D;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if (pEm->Routine_2 == 0) {
        pEm->Timer0 = 0x3C;
    }
    if (pEm->Timer0 != 0) {
        pEm->Timer0--;
    }
    D = ((USHORT *)pEm->Unk1CC)[3];
    if (D < 0xC8) {
        *(ULONG *)&pEm->Routine_0 = 0xB01;
        return;
    }
    if (pEm->Timer0 < 0x1E && D >= 0x3E9) {
        *(ULONG *)&pEm->Routine_0 = 0xD01;
        return;
    }
    if (((USHORT *)pEm->Unk1CC)[3] >= 0x2BD && Pl_work.Spd_x >= 0x65) {
        *(ULONG *)&pEm->Routine_0 = 0xD01;
    }
    if (pEm->Timer0 == 0) {
        *(ULONG *)&pEm->Routine_0 = 0xD01;
    }
    if (Dist < 0x5DC && (pEm->UnkAE & 0x30) == 0 && pT != NULL) {
        Goto00(pEm, pT->Pos_x, pEm->Pos_z, 0x30);
    } else {
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x30);
    }
}

void func_8005D3B4(ENEMY_WORK *pEm)
{
    ULONG D;

    if (pEm->Routine_2 == 0) {
        pEm->Timer0 = 0x3C;
    }
    if (pEm->Timer0 != 0) {
        pEm->Timer0--;
    }
    D = ((USHORT *)pEm->Unk1CC)[3];
    if (D < 0xC8) {
        *(ULONG *)&pEm->Routine_0 = 0xB01;
        return;
    }
    if (pEm->Timer0 < 0x1E && D >= 0x3E9) {
        *(ULONG *)&pEm->Routine_0 = 0xD01;
        return;
    }
    if (((USHORT *)pEm->Unk1CC)[3] >= 0x2BD && Pl_work.Spd_x >= 0x65) {
        *(ULONG *)&pEm->Routine_0 = 0xD01;
    }
    if (pEm->Timer0 == 0) {
        *(ULONG *)&pEm->Routine_0 = 0xD01;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x30);
}

void func_8005D484(ENEMY_WORK *pEm)
{
    if (Goto00_ck(pEm, ((SHORT *)pEm->Unk1CC)[0], ((SHORT *)pEm->Unk1CC)[1], 0x50) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
    } else {
        Goto00(pEm, ((SHORT *)pEm->Unk1CC)[0], ((SHORT *)pEm->Unk1CC)[1], 0xA0);
    }
}

void func_8005D4D8(ENEMY_WORK *pEm)
{
    ULONG Dist;
    ENEMY_WORK *pPl = &Pl_work;

    Dist = pEm->Unk154;
    if (pEm->Routine_2 == 0) {
        pEm->Timer0 = 0x2D;
    }
    if (pEm->Timer0 == 0 || --pEm->Timer0 == 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    }
    if (Dist > 0x7D0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    }
    Goto00(pEm, pPl->Pos_x, pEm->Pos_z, 0x30);
}

void func_8005D554(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    SHORT d;
    UCHAR e;

    pT = D_800E01A8;
    if (pT == NULL) {
        *(ULONG *)&pEm->Routine_0 = 1;
        return;
    }
    if ((LONG)pEm->Be_flg < 0) {
        *(ULONG *)&pEm->Routine_0 = 0x1001;
        pEm->pEnemy_ptr = pT;
        return;
    }
    if (pEm->Routine_2 == 0) {
        pEm->Timer0 = 0x14;
        pEm->UnkE3 = 0;
        return;
    }
    if (--pEm->Timer0 == -1) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (pT->Life >= 0 && (pT->Type & 0x2000) == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x501;
        }
    }
    if (pEm->UnkAE & 0x30) {
        e = pEm->UnkE3;
        pEm->UnkE3 = e + 1;
        if (pEm->UnkAE & 0x10) {
            pEm->UnkE3 = e + 6;
        }
    } else {
        pEm->UnkE3 = 0;
    }
    if ((s8)pEm->UnkE3 >= 0xB) {
        if (pT->Type & 0x2000) {
            *(ULONG *)&pEm->Routine_0 = 1;
        } else if (pT->Life >= 0) {
            *(ULONG *)&pEm->Routine_0 = 0x501;
        } else {
            *(ULONG *)&pEm->Routine_0 = 1;
        }
    }
    d = -0x400;
    if (((USHORT *)pEm->Unk1CC)[10] & 2) {
        d = 0x400;
    }
    if ((LONG)(pT->Unk158 & 0x3FFFFFFF) < 0x7D0) {
        Goto01(pEm, pT->Unk130 & 0xFFF, 0xA0);
        pEm->Timer0 += 0x14;
    } else {
        Goto01(pEm, (pT->Unk130 + (SHORT)d) & 0xFFF, 0x50);
    }
}

void func_8005D704(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D70C);

void func_8005DA44(ENEMY_WORK *pEm)
{
    ULONG D;

    if ((pEm->UnkAE & 0x40) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
    }
    if ((func_800632B8(pEm, 0) & 0x1000) == 0) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    D = ((USHORT *)pEm->Unk1CC)[3];
    if (D < 0x12C) {
        *(ULONG *)&pEm->Routine_0 = 0xB01;
    } else if (D >= 0x1389) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    } else if (D < 0x7D0 && G.Pl_routine_1 == 2 && ((LONG (*)(ENEMY_WORK *, ENEMY_WORK *))Cdir_ck2)(pEm, (ENEMY_WORK *)((UCHAR *)&G + 0x248C)) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005DBCC);

void func_8005DE3C(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if ((pEm->UnkAE & 0x40) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if ((func_800634F4(pEm, 0) & 0x1000) == 0) {
        if (pEm->Routine_2 == 1) {
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if (((USHORT *)pEm->Unk1CC)[3] < 0xBB8) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        return;
    }
    if (Dist < 0x7D0 && Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400) == 0 && (pEm->UnkAE & 0x30) == 0) {
        Goto00(pEm, pT->Pos_x, pT->Pos_z, -0x38);
    } else {
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x38);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005DF4C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E1D0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E40C);

void func_8005E688(ENEMY_WORK *pEm)
{
    D_8009ED24[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

void func_8005E6CC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x30004;
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        func_800775A0(0, pEm);
        pEm->Be_flg &= ~0x8000;
    case 1:
        pEm->Routine_2 += func_80027940(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        *(ULONG *)&pEm->Routine_0 = 1;
        pEm->Damage_cnt = 0;
        pEm->Unk120 &= 0xED;
        break;
    }
}

void func_8005E7C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Spd_z = 0;
        *(ULONG *)&pEm->Move_no = (pEm->Routine_1 & 1) + 0x30002;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        pEm->Be_flg &= ~0x8000;
        *(LONG *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 400 - 200;
        func_800775A0(pEm->Routine_2 - 1, pEm);
    case 1:
        if (pEm->UnkCA != 0) {
            Add_speedXZ(pEm, 0);
        }
        pEm->Routine_2 += func_80027940(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        *(ULONG *)&pEm->Routine_0 = 1;
        pEm->Damage_cnt = 0;
        pEm->Unk120 &= 0xED;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E90C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005EBC0);

void func_8005EE14(ENEMY_WORK *pEm)
{
    G.Status_flg |= 0x100;
    D_8009ED84[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

void func_8005EE70(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP1;
    PARTS_W *pP2;
    PARTS_W *pP3;

    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70001;
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        pEm->Unk120 = 1;
        pEm->Damage_cnt |= 0x80;
        func_800775A0(3, pEm);
        G.System_flg |= 0x20000;
        if (pEm->Unk132 & 0x500) {
            pP1 = pEm->pSin_parts_ptr;
            func_8001B484(0x1001B03, ((((LONG (*)(void))func_800102E8)() << 1) + 0x800) << 16,
                          (LONG)&pP1[2].Workm, NULL, 0);
            pP2 = pEm->pSin_parts_ptr;
            func_8001B484(0x1001B03, ((((LONG (*)(void))func_800102E8)() << 1) + 0x1000) << 16,
                          (LONG)&pP2[0].Workm, NULL, 0);
            pP3 = pEm->pSin_parts_ptr;
            func_8001B484(0x1001B03, ((((LONG (*)(void))func_800102E8)() << 1) + 0x1000) << 16,
                          (LONG)&pP3[13].Workm, NULL, 0);
        }
    case 1:
        pEm->Routine_2 += func_80027940(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        *(ULONG *)&pEm->Routine_0 = 7;
        break;
    }
    if (pEm->UnkCA == 0) {
        func_80019318(pEm, 0, 1);
    }
}

void func_8005F01C(ENEMY_WORK *pEm)
{
    KAGE_WORK9 *pK;

    pK = pEm->pKage_work;
    switch (pEm->Routine_1) {
    case 0:
        pEm->Routine_1 = 1;
        Kage_work9_color_set(pK, 0xBFBF10);
        pEm->Timer0 = 0x3C;
    case 1:
        pK->Half_x += 8;
        pK->Half_z += 8;
        if (--pEm->Timer0 == -1) {
            pEm->Routine_1 = 2;
        }
        break;
    case 2:
        break;
    }
}

LONG func_8005F0BC(LONG Len, LONG Dir1, LONG Dir2, SVECTOR *pOut)
{
    MATRIX m;
    SVECTOR Rot;
    SVECTOR v;
    SVECTOR a;
    SVECTOR b;
    ULONG d1;
    ULONG d2;
    SHORT dx;
    SHORT dz;

    Rot.vz = 0;
    Rot.vx = 0;
    Rot.vy = G.Pl_cdir_y + Dir1;
    RotMatrix(&Rot, &m);
    v.vx = Len;
    v.vz = 0;
    v.vy = 0;
    ApplyMatrixSV(&m, &v, &a);
    dx = G.pEm->Pos_x - (G.Pl_pos_x + a.vx);
    dz = G.pEm->Pos_z - (G.Pl_pos_z + a.vz);
    d1 = SquareRoot0(dx * dx + dz * dz);
    Rot.vy = G.Pl_cdir_y + Dir2;
    RotMatrix(&Rot, &m);
    ApplyMatrixSV(&m, &v, &b);
    dx = G.pEm->Pos_x - (G.Pl_pos_x + b.vx);
    dz = G.pEm->Pos_z - (G.Pl_pos_z + b.vz);
    d2 = SquareRoot0(dx * dx + dz * dz);
    pOut->vy = G.Pl_pos_y;
    pOut->vx = G.Pl_pos_x + a.vx;
    pOut->vz = G.Pl_pos_z + a.vz;
    if (d2 < d1) {
        pOut->vx = G.Pl_pos_x + b.vx;
        pOut->vz = G.Pl_pos_z + b.vz;
        d1 = d2;
    }
    return d1;
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F298);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F660);

ENEMY_WORK *func_8005F7E8(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pA;
    ENEMY_WORK *pB;
    ENEMY_WORK *p;
    ENEMY_WORK **pp;
    USHORT *pd;
    ULONG MinA;
    ULONG MinB;
    LONG i;

    MinA = 0x7FFFFFFF;
    MinB = 0x7FFFFFFF;
    pA = NULL;
    pB = NULL;
    i = 2;
    pd = &D_800E01B4;
    pp = &D_800E01A8;
loop:
    p = *pp;
    if (p == NULL) {
        goto end;
    }
    if (Goto00_ck(pEm, p->Pos_x, p->Pos_z, 0x400) == 0) {
        if (*pd < MinA) {
            pA = p;
        }
    } else {
        if (*pd < MinB) {
            pB = p;
        }
    }
    pd++;
    pp++;
    if (i-- != 0) {
        goto loop;
    }
end:
    if (pA != NULL) {
        return pA;
    }
    return pB;
}

ENEMY_WORK *func_8005F8F0(ENEMY_WORK *pEm, ENEMY_WORK *pEx)
{
    ULONG Found;
    ENEMY_WORK *pRes;
    ULONG i;
    LONG n;
    ENEMY_WORK *p;

    Found = 0;
    pRes = NULL;
    i = 0;
    n = 2;
    do {
        p = (&D_800E01A8)[i];
        if (p != NULL) {
            if (G.Pl_pEnemy_ptr == p) {
                i++;
                Found = 1;
                continue;
            }
            if (pEx != p) {
                pRes = p;
            }
        }
        i++;
    } while (n-- != 0);
    if (pRes != NULL) {
        return pRes;
    }
    if (Found) {
        return G.Pl_pEnemy_ptr;
    }
    return NULL;
}

LONG func_8005F984(LONG Dmg, ULONG Flg)
{
    ENEMY_WORK *pW;

    pW = G.pUnk2660;
    if ((ULONG)(Dmg - 10) < 0x5B) {
        Dmg += 5;
    }
    pW->Life -= Dmg;
    G.Unk7852 -= Dmg;
    if (Flg & 1) {
        if (pW->Life < 0) {
            pW->Life = 0;
            G.Unk7852 = 0;
        }
        return 0;
    }
    if (Flg == 0) {
        return ((USHORT)pW->Life >> 14) & 2;
    }
}

LONG func_8005FA10(ENEMY_WORK *pEm, LONG Dmg, ULONG Flg)
{
    ENEMY_WORK *pW;

    pW = G.pUnk2660;
    pW->Life -= Dmg;
    G.Unk7852 -= Dmg;
    *(ULONG *)&pW->Routine_0 = 2;
    pW->Unk132 &= 0xE0FF;
    if (Flg & 2) {
        pW->Routine_1 = 2;
    }
    pW->Routine_1 += Cdir_ck(pEm, pW);
    if (Flg & 1) {
        if (pW->Life < 0) {
            pW->Life = 0;
            G.Unk7852 = 0;
        }
        return 0;
    }
    if (pW->Life >= 0) {
        return 0;
    }
    *(ULONG *)&pW->Routine_0 = 3;
    return 2;
}

void func_8005FAEC(ENEMY_WORK *pEm)
{
    UCHAR *pP;
    SVECTOR Sv;

    if (pEm->Move_cnt == 0) {
        pP = (UCHAR *)pEm->pSin_parts_ptr + 0x330;
        if (func_80044804(pP, D_8009DBB4, 3) != 0) {
            ((USHORT *)pEm->Unk1CC)[2] |= 0x4000;
        }
        func_800775A0(0xF, pEm);
        Sv.vx = 100;
        Sv.vy = 150;
        Sv.vz = 0;
        G.Unk75E8 |= 1;
        func_8001B484(2, ((USHORT)(pEm->Cdir_y + 0x400)) | 0x8000000, (LONG)pP, &Sv, 0);
        Sv.vx = 100;
        Sv.vy = 450;
        Sv.vz = 0;
        func_8001B484(1, (USHORT)pEm->Cdir_y | 0x10000000, (LONG)pP, &Sv, 0);
        Sv.vx = 100;
        Sv.vy = 350;
        Sv.vz = 0;
        func_8001B484(3, (USHORT)pEm->Cdir_y | 0x10000000, (LONG)pP, &Sv, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005FC18);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005FEE0);
