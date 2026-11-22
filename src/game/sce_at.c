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
#include "pad.h"
#include "pl_wep.h"
#include "sce1.h"
#include "sce_at.h"
#include "sld.h"
#include "snd.h"

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

/* door AOT (RE2 IN_DOOR_WORK; RE3 layout) */
typedef struct tagSCE_AOT_DOOR {
    UCHAR pad0[0xD];
    UCHAR UnkD;                        /* 0x0D 4: "locked from the other side" message */
    UCHAR UnkE;                        /* 0x0E selects the door sound */
    UCHAR Key_id;                      /* 0x0F 0x80: locked, low 6 bits key flag */
    UCHAR Key_type;                    /* 0x10 item id of the key, 0xFE/0xFF special */
    UCHAR Mess;                        /* 0x11 0x80: message in low 4 bits, 0x40: ask first */
} SCE_AOT_DOOR;

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
