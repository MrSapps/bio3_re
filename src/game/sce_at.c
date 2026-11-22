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
extern SHORT D_800D1F82;
extern s8 D_80098728[];
UCHAR func_800102E8(void);             /* sld.s Rnd? */
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
void func_8001B484(ULONG A0, ULONG A1, LONG A2, SVECTOR *pSv, ULONG A4);
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

/* AOT entry (RE2 struct tagSCE_AOT, SCE1.H) */
struct tagSCE_AOT {
    UCHAR Id;                          /* 0x00 */
    UCHAR Type;                        /* 0x01 */
    UCHAR nFloor;                      /* 0x02 */
    UCHAR Super;                       /* 0x03 */
    SHORT X;                           /* 0x04 */
    SHORT Z;                           /* 0x06 */
    USHORT W;                          /* 0x08 */
    USHORT D;                          /* 0x0A */
    USHORT Data;                       /* 0x0C */
    USHORT Data1;                      /* 0x0E water AOT: Ob_model index (0xFFFF: none) */
};

/* 4-point AOT (Type & 0x80): the area is given by four points, the data follows them */
typedef struct tagSCE_AOT4 {
    UCHAR Id;                          /* 0x00 */
    UCHAR Type;                        /* 0x01 */
    UCHAR nFloor;                      /* 0x02 */
    UCHAR Super;                       /* 0x03 */
    USHORT Xz[4][2];                   /* 0x04 */
    USHORT Data;                       /* 0x14 */
} SCE_AOT4;

/* item AOT data (RE2 struct tagSCE_ITEM_DATA, SCE_AT.H) */
typedef struct tagSCE_ITEM_DATA {
    SHORT Data0;                       /* 0x0 */
    SHORT Data1;                       /* 0x2 */
    USHORT Data2;                      /* 0x4 item flag bit */
    UCHAR Om_no;                       /* 0x6 */
    UCHAR Free;                        /* 0x7 */
} SCE_ITEM_DATA;

void Hikidashi_entrance(void);

/* door AOT (RE2 IN_DOOR_WORK; RE3 layout) */
typedef struct tagSCE_AOT_DOOR {
    UCHAR pad0[0xD];
    UCHAR UnkD;                        /* 0x0D 4: "locked from the other side" message */
    UCHAR UnkE;                        /* 0x0E selects the door sound */
    UCHAR Key_id;                      /* 0x0F 0x80: locked, low 6 bits key flag */
    UCHAR Key_type;                    /* 0x10 item id of the key, 0xFE/0xFF special */
    UCHAR Mess;                        /* 0x11 0x80: message in low 4 bits, 0x40: ask first */
} SCE_AOT_DOOR;
void Key_lost(void);
void func_800510B0(void);
extern void *Hiki_pAot;                /* 0x800DECA4 RE2 Hikidashi_entrance static pAot */
extern SCE_ITEM_DATA *Hiki_pItem_dat;  /* 0x800DECA8 RE2 Hikidashi_entrance static pItem_dat */

INCLUDE_ASM("asm/nonmatchings/game/sce_at", Sce_at_check);

void Sce_at(void)
{
    ENEMY_WORK *pEw;
    ENEMY_WORK **ppEw;
    OBJ_MODEL_WORK *pOm_end;

    if (G.Pl_life < 0) {
        return;
    }
    G.Pl_water = 0;
    D_800DECAC = 0;
    if (G.Pl_damage_cnt != 0 || Game.Unk1 != 0) {
        G.Status_flg &= ~0x800000;
    }
    if (G.Status_flg & 0x800000) {
        Sce_at_check(&G.Pl_be_flg, 1, 0x10);
    }
    Sce_at_check(&G.Pl_be_flg, 1, 0);
    for (ppEw = &G.pEnemy[2]; ppEw != G.ppEnemy_end; ppEw++) {
        pEw = *ppEw;
        if (pEw->Be_flg & 1) {
            pEw->Water = 0;
            Sce_at_check(pEw, 2, 0);
        }
    }
    pEw = (ENEMY_WORK *)G.Ob_model;
    pOm_end = G.Ob_model + G.pRoom->nOmodel;
    for (; pEw < (ENEMY_WORK *)pOm_end; pEw = (ENEMY_WORK *)((OBJ_MODEL_WORK *)pEw + 1)) {
        if (pEw->Be_flg & 1) {
            pEw->Water = 0;
            Sce_at_check(pEw, 4, 0);
        }
    }
    if (G.pEnemy[1]->Be_flg & 1) {
        G.pEnemy[1]->Water = 0;
        Sce_at_check(G.pEnemy[1], 8, 0);
    }
}

/* RE2 Nothing */
ULONG Nothing(void *pAot)
{
    G.Sce_work[0] = Old_Fatari;        /* RE2 G.SaveData.F_atari */
    G.Sce_work[1] = Old_Uatari;        /* RE2 G.SaveData.U_atari */
    return 0;
}

ULONG Sce_Door(void *pAot)
{
    tagGLOBAL *pG;
    SCE_AOT_DOOR *pDoor;
    ULONG Key_type;
    LONG Key_work;

    pG = &G;
    pDoor = (SCE_AOT_DOOR *)pAot;
    pG->pAot_data = pCAot;
    pG->pUnk21DC = pAot;
    if (!(pG->Status_flg & 0x400000)) {
        if (pDoor->Key_id & 0x80) {
            if (pG->System_flg & 0x80) {
                Mess_set(0, 0, 0xC, 0xFF000000);
                return 0;
            }
            if (!Flg_ck(pG->Unk7994, pDoor->Key_id & 0x3F)) {
                Key_type = pDoor->Key_type;
                if (Key_type == 0xFE) {
                    Snd_se_stad(0x226, 0, 0, 0);
                    Mess_set(0, 0, 0x11, 0xFF000000);
                    goto door_in;
                }
                if (Key_type == 0xFF) {
                    Snd_se_stad(0x216, 0, 0, 0);
                    Mess_set(0, 0, 0x12, 0xFF000000);
                    return 0;
                }
                Key_work = Search_item_id(Key_type, 0);
                if (Key_work >= 0) {
                    Moji.Mess_item = Key_type;
                    Mess_set(0, 0, 5, 0xFF000000);
                    if (pDoor->UnkE == 0) {
                        Snd_se_stad(0x225, 0, 0, 0);
                    } else {
                        Snd_se_stad(0x204, 0, 0, 0);
                    }
                    pG->Front_func = Key_lost;
                    pG->Front_func_rtn = Key_work + 1;
                door_in:
                    Flg_on(pG->Unk7994, pDoor->Key_id & 0x3F);
                    return 0;
                }
                if (pDoor->Mess & 0x80) {
                    if (pDoor->UnkE == 0) {
                        Snd_se_stad(0x216, 0, 0, 0);
                    } else {
                        Snd_se_stad(0x205, 0, 0, 0);
                    }
                    Mess_set(0, 0x2000, pDoor->Mess & 0xF, 0xFF000000);
                    return 0;
                }
                if (pDoor->UnkE == 0) {
                    Snd_se_stad(0x216, 0, 0, 0);
                } else {
                    Snd_se_stad(0x205, 0, 0, 0);
                }
                Mess_set(0, 0, Key_type - 0x5F, 0xFF000000);
                return 0;
            }
        }
        if (pDoor->Mess & 0x40) {
            if (pDoor->UnkD == 4) {
                Mess_set(0, 0, 0xA, 0xFF000000);
            } else {
                Mess_set(0, 0, 0xB, 0xFF000000);
            }
            pG->Front_func = func_800510B0;
            pG->Front_func_rtn = 0;
            return 0;
        }
        Game.Doordemo_flg = 1;
        G.pSce_aot_on = pAot;
        G.Unk2468 |= 0xFF000000;
        return 0;
    }
}

void Key_lost(void)
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
            G.pSce_aot_on = G.pUnk21DC;
            G.Unk2468 |= 0xff000000;
        }
        G.Front_func = NULL;
        G.Front_func_rtn = 0;
    }
}

/* RE2 Sce_Item */
ULONG Sce_Item(void *pAot)
{
    G.pUnk21DC = pAot;
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

    pDat = pG->pUnk21DC;
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

LONG Sce_Normal(void)
{
    return 0;
}

/* RE2 Sce_Message */
ULONG Sce_Message(void *pAot)
{
    USHORT *pDat;

    pDat = (USHORT *)pAot;
    Mess_set(0, pDat[1] | 0x2000, pDat[0], (ULONG)pDat[2] << 16);
    return 0;
}

/* RE2 Sce_Event */
ULONG Sce_Event(void *pAot)
{
    if ((G.Unk2468 & 0x2000000) == 0) {
        Event_exec(((USHORT *)pAot)[0], ((UCHAR *)pAot)[3]);
    }
    return 0;
}

/* RE2 Sce_Flg_chg */
ULONG Sce_Flg_chg(void *pAot)
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
ULONG Sce_Water(void *pAot)
{
    G.pEm->Water = ((SHORT *)pAot)[0];
    return 0;
}

/* RE2 Sce_Save */
ULONG Sce_Save(void *pAot)
{
    G.Back_func = Save_entrance;
    G.Back_func_rtn = 0;
    G.Save_area = *(UCHAR *)pAot;
    Game.Stop_bak = G.Unk2468;
    G.Unk2468 |= 0xff000000;
    return 0;
}

/* RE2 Save_entrance */
void Save_entrance(void)
{
    tagGLOBAL *pG = &G;

    switch (pG->Back_func_rtn) {
    case 0:
        if (Search_item_id(0x81, 0) >= 0) {
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

ULONG Sce_Itembox(void *pAot)
{
    D_800E01C4[0] = 2;
    G.Back_func = Item_box_entrance;
    G.Back_func_rtn = 0;
    return 0;
}

extern SHORT D_800DEC9C;               /* RE2 Item_box_entrance static Dir_spd */
extern SHORT D_800DEC9E;               /* RE2 Item_box_entrance static Spd_add */
extern OBJ_MODEL_WORK *D_800DECA0;     /* RE2 Item_box_entrance static pOwork */
/* RE2 Item_box_entrance */
void Item_box_entrance(void)
{
    tagGLOBAL *pG = &G;

    switch (pG->Back_func_rtn) {
    case 0:
        D_800DEC9C = 1;
        D_800DEC9E = 3;
        Game.Stop_bak = pG->Unk2468;
        pG->Unk2468 |= 0x7F000000;
        Snd_se_stad(0x214, 0, 0, 0);
        D_800DECA0 = &pG->Ob_model[0];
        pG->Back_func_rtn = 1;
    case 1:
        D_800DECA0->Cdir.vz -= D_800DEC9C;
        D_800DEC9C += D_800DEC9E;
        if (D_800DECA0->Cdir.vz < -399) {
            pG->Back_func_rtn = 2;
            D_800DEC9E *= -1;
        }
        break;
    case 2:
        D_800DECA0->Cdir.vz -= D_800DEC9C;
        D_800DEC9C += D_800DEC9E;
        if (D_800DEC9C > 0) {
            break;
        }
        Game.Unk1 = 1;
        D_800DEC9C = 0;
        pG->Back_func_rtn = 3;
        pG->Status_flg |= 0x200;
    case 3:
        D_800DEC9C++;
        if (D_800DEC9C < 7) {
            break;
        }
        D_800DECA0->Cdir.vz = 0;
        pG->Back_func = NULL;
        pG->Back_func_rtn = 0;
        break;
    }
}

ULONG func_80051684(void *pAot)
{
    G.pUnk21DC = pAot;
    G.System_flg |= 0x2000;
    return 0;
}

ULONG Sce_Hikidashi(void *pAot)
{
    tagGLOBAL *pG;
    USHORT *pDat;
    ULONG *pFlg;

    pG = &G;
    pDat = (USHORT *)pAot;
    if (pG->Front_func == NULL && pG->Pl_damage_cnt == 0 && !(pG->Status_flg & 0x300)
        && pG->Next_cut_no == pG->Cut_no) {
        if (pG->Unk783E < 2) {
            pFlg = pG->Unk78D0;
        } else {
            pFlg = pG->Unk78F0;
        }
        if (!Flg_ck(pFlg, ((SCE_ITEM_DATA *)&pG->Sce_aot[pDat[1]]->Data)->Data2)) {
            if (!Flg_ck(pG->Unk7994, pDat[0])) {
                if (Search_item_id(0x72, 0) >= 0) {
                    pG->Front_func_rtn = 1;
                    pG->pUnk21DC = pAot;
                    pG->pAot_data = pCAot;
                    pG->Front_func = Hikidashi_entrance;
                } else {
                    Mess_set(0, 0x2000, pDat[2] >> 8, 0xFF000000);
                }
            } else {
                pG->pUnk21DC = pAot;
                pG->pAot_data = pCAot;
                Snd_se_stad(0x228, 0, 0, 0);
                pG->Ob_model[((SCE_ITEM_DATA *)&pG->Sce_aot[pDat[1]]->Data)->Om_no].Be_flg |= 0x41;
                Cut_change(((UCHAR *)pDat)[4]);
                Game.Stop_bak = pG->Unk2468;
                G.Unk2468 |= 0xFF000000;
                G.Status_flg |= 0x100;
                pG->Cccut_next->Xz[0][0] = -0x7FBC;
                pG->Cccut_next->Xz[1][0] = -0x7FBC;
                pG->Cccut_next->Xz[2][0] = -0x7F58;
                pG->Cccut_next->Xz[3][0] = -0x7F58;
                pG->Cccut_next->Xz[0][1] = -0x7FBC;
                pG->Cccut_next->Xz[1][1] = -0x7F58;
                pG->Cccut_next->Xz[2][1] = -0x7F58;
                pG->Cccut_next->Xz[3][1] = -0x7FBC;
                pG->Front_func_rtn = 0x23;
                pG->Front_func = Hikidashi_entrance;
            }
        } else {
            Mess_set(0, 0, 4, 0xFF000000);
        }
    }
    return 0;
}

void Hikidashi_entrance(void)
{
    tagGLOBAL *pG;
    USHORT *pDat;

    pG = &G;
    pDat = pG->pUnk21DC;
    switch (pG->Front_func_rtn) {
    case 0:
        break;
    case 1:
        Moji.Mess_item = 0x72;
        Mess_set(0, 0x2000, (pDat[2] >> 8) + 1, 0xFF000000);
        pG->Front_func_rtn = 2;
        break;
    case 2:
        if (Moji.Mess_flg & 0x80) {
            break;
        }
        if (!(pG->Unk7848 & 1)) {
            Flg_on(pG->Unk7994, pDat[0]);
            Moji.Mess_item = 0x72;
            Mess_set(0, 0, 5, 0xFF000000);
        }
        pG->Front_func = NULL;
        pG->Front_func_rtn = 0;
        break;
    case 4:
        if (G.Status_flg & 0x200) {
            break;
        }
        G.Status_flg &= ~0x80;
        pG->Ob_model[Hiki_pItem_dat->Om_no].Be_flg &= ~0x41;
        pG->Front_func = NULL;
        pG->Front_func_rtn = 0;
        break;
    case 5:
        Hiki_pAot = pG->pAot_data;
        Hiki_pItem_dat = (SCE_ITEM_DATA *)&pG->Sce_aot[pDat[1]]->Data;
        pG->pAot_data = pG->Sce_aot[pDat[1]];
        pG->pUnk21DC = &pG->Sce_aot[pDat[1]]->Data;
        St.St_type = 1;
        G.Status_flg = (G.Status_flg & ~0x100) | 0x200;
        Game.Unk1 = 1;
        Cut_change(pG->Room_no_old);
        pG->Cut_no = pG->Room_no_old;
        St.Unk6 = 1;
        G.Status_flg |= 0x80;
        pG->Front_func_rtn--;
        break;
    default:
        pG->Front_func_rtn--;
        break;
    }
}

/* RE2 Sce_Damage */
ULONG Sce_Damage(void *pAot)
{
    tagGLOBAL *pG = &G;
    USHORT *pDat;
    LONG Dir;

    pDat = (USHORT *)pAot;
    if (!(pG->pEm->Damage_cnt & 0x80)) {
        if (pG->pEm->Status_flg & 2) {
            pG->pEm->Status_flg &= ~2;
        } else {
            pG->pEm->Damage_cnt |= 0x80;
            pG->pEm->Life -= pDat[1];
            if (pG->pEm->Life < 0) {
                if (pG->pEm->Routine_0 != 3) {
                    pG->pEm->Routine_0 = 3;
                    pG->pEm->Routine_1 = 0;
                    pG->pEm->Routine_2 = 0;
                    pG->pEm->Routine_3 = 0;
                }
            } else {
                pG->pEm->Routine_0 = 2;
                pG->pEm->Routine_1 = pDat[0];
                pG->pEm->Routine_2 = 0;
                pG->pEm->Routine_3 = 0;
                Dir = Direction_ck(((SHORT *)pCAot)[2] + (((USHORT *)pCAot)[4] >> 1), ((SHORT *)pCAot)[3] + (((USHORT *)pCAot)[5] >> 1),
                                   pG->pEm->Pos_x, pG->pEm->Pos_z);
                Dir -= pG->pEm->Cdir_y;
                pG->pEm->Spd_x = 200;
                Add_speedXZ(pG->pEm, Dir);
                Pad_vib_set0(4, 0);
                Pad_vib_set1(10, 0x96, 0);
            }
        }
    }
    return 1;
}

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

/* RE2 Water_ck */
LONG Water_ck(LONG Pos_x, LONG Pos_z)
{
    VECTOR Vec;
    SHORT Work[10];
    ULONG i;
    ULONG j;
    ULONG D0;
    UCHAR *pAot;

    D0 = G.nSce_aot;
    Vec.vx = Pos_x;
    Vec.vy = 0;
    Vec.vz = Pos_z;
    for (i = 0, j = 0; i < D0;) {
        pAot = (UCHAR *)G.Sce_aot[j++];
        if (pAot != NULL) {
            i++;
            if (pAot[0] == 7) {
                if (pAot[1] & 0x80) {
                    Work[2] = ((SHORT *)pAot)[2];
                    Work[3] = ((SHORT *)pAot)[3];
                    Work[4] = ((SHORT *)pAot)[4];
                    Work[5] = ((SHORT *)pAot)[5];
                    Work[6] = ((SHORT *)pAot)[6];
                    Work[7] = ((SHORT *)pAot)[7];
                    Work[8] = ((SHORT *)pAot)[8];
                    Work[9] = ((SHORT *)pAot)[9];
                    if (func_8001020C((LONG *)&Vec, (VCUT *)Work)) {
                        return ((SHORT *)pAot)[10];
                    }
                } else if (func_800101C8((LONG *)&Vec, (SHORT *)(pAot + 4))) {
                    return ((SHORT *)pAot)[6];
                }
            }
        }
    }
    return 0;
}
