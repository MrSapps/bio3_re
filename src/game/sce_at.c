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
ENEMY_WORK *Spl_em_cdir_ck();
ENEMY_WORK *Spl_pllock_get();
extern UCHAR D_800A0514[];
typedef struct {
    UCHAR Unk0;
    UCHAR Max;                         /* 0x1 */
    UCHAR pad2[2];
} ITEM_TBL4;
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

INCLUDE_ASM("asm/nonmatchings/game/sce_at", Sce_at_check);

INCLUDE_ASM("asm/nonmatchings/game/sce_at", Sce_at);

/* RE2 Nothing */
ULONG Nothing(void *pAot)
{
    G.Sce_work[0] = Old_Fatari;        /* RE2 G.SaveData.F_atari */
    G.Sce_work[1] = Old_Uatari;        /* RE2 G.SaveData.U_atari */
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/sce_at", Sce_Door);

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
            G.pSce_aot_on = (void *)G.Unk21DC;
            G.Unk2468 |= 0xff000000;
        }
        G.Front_func = NULL;
        G.Front_func_rtn = 0;
    }
}

/* RE2 Sce_Item */
ULONG Sce_Item(void *pAot)
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
        D_800DECA0 = (OBJ_MODEL_WORK *)pG->Ob_model[0];
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
    G.Unk21DC = (LONG)pAot;
    G.System_flg |= 0x2000;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/sce_at", Sce_Hikidashi);

INCLUDE_ASM("asm/nonmatchings/game/sce_at", Hikidashi_entrance);

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
