#include "sce0.h"
#include "em_sub.h"
#include "main.h"
#include "sca.h"
#include "line_scr.h"
#include "sub.h"
#include "player.h"
#include "om.h"
#include "oba.h"
#include "esp.h"
#include "cd.h"
#include "status.h"
#include "joint.h"

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

/* declarations from earlier parts of the original address-split file */
extern SHORT D_800DEC9C;               /* RE2 Item_box_entrance static Dir_spd */
extern SHORT D_800DEC9E;               /* RE2 Item_box_entrance static Spd_add */
extern OBJ_MODEL_WORK *D_800DECA0;     /* RE2 Item_box_entrance static pOwork */

extern UCHAR D_800B9E21;
void Xa_fade_set(ULONG frames);
void Pad_vib_init(void);

extern UCHAR *D_800BA700[10];          /* RE2 pDwork[] */

static inline ULONG Sce_skip(SCE_TASK *pSce, LONG n)
{
    pSce->Data += n;
    return 1;
}

extern UCHAR D_8009E52C[][2];          /* RE2 Subpl_walk */
extern UCHAR D_8009E5CC[][2];          /* RE2 Subpl_run */
extern UCHAR D_8009E554[][2];          /* RE2 Subpl_back */
extern UCHAR D_8009E5A4[][2];          /* RE2 Subpl_ato */
extern UCHAR D_8009E57C[][2];          /* RE2 Subpl_dir */

extern UCHAR D_800C7961;

void Spl(ENEMY_WORK *pEm)
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
    Spl_Root_ck_sys(pEm);
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
    Rot_neck(pEm, pEm->Cdir_y);
}

extern USHORT D_8009E374[][4];         /* collision size by Id: [0] Unk26, [1] W0/D0 */
void Kage_work9_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos);
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);

/* sub player work tail (ENEMY_WORK.Unk1CC[] is player-only data) */
typedef struct {
    UCHAR pad[0x1D0];
    SHORT Unk1D0;                      /* 0x1D0 */
    UCHAR pad1D2[2];
    SHORT Unk1D4;                      /* 0x1D4 */
    UCHAR pad1D6;
    UCHAR Unk1D7;                      /* 0x1D7 */
    SHORT Unk1D8;                      /* 0x1D8 */
    UCHAR pad1DA[4];
    UCHAR Unk1DE;                      /* 0x1DE */
} SPL_WORK;
#define SPL(pEm) ((SPL_WORK *)(pEm))
#define PLAYER ((ENEMY_WORK *)((UCHAR *)&G + 0x248C))

void Spl_Init(ENEMY_WORK *pEm)
{
    USHORT Neck_dir[26] = {
        0x1000, 0x1000, 0x1052, 0x1000, 0x1000, 0x1141, 0x1052, 0x1117, 0x1000, 0x1000, 0x107B, 0x1000, 0x1000,
        0x1000, 0x1052, 0x1000, 0x1052, 0x107B, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
    };
    COL_OBJ *pCol;
    ULONG *pTmd;
    PARTS_W *pP;                       /* clear loop, neck free work, then the weapon part */
    PARTS_W *pHand;
    LONG i;
    USHORT *pSize;

    *(ULONG *)&pEm->Routine_0 = 1;
    pEm->Unk12A = Neck_dir[pEm->Id - 0x50];
    pEm->pEnemy_ptr = pEm;
    pEm->pUnk114 = pEm;
    *(LONG *)&pEm->Move_no = 2;
    Joint_move(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0x100);
    Kage_work9_set((ULONG *)&pEm->pKage_work, 0, 0x22601C2, 0, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
    pCol = (COL_OBJ *)&pEm->Col_id;
    pCol->pEm = pEm;
    pSize = D_8009E374[pEm->Id];
    pCol->W0 = pCol->D0 = pSize[1];
    pCol->H = 3000;
    pCol->Unk28 = 0;
    pCol->Unk24 = 0;
    pCol->Unk26 = pSize[0];
    pCol->Flg = 0;
    if (pEm == G.pEnemy[1]) {
        pCol->Type = 1;
    } else {
        pCol->Type = 2;
    }
    func_80033580((COL_OBJ *)&pEm->Col_id, 0);
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[1];
    pEm->Unk134 = 1500;
    pEm->Unk136 = -1000;
    pEm->Unk138 = 0x54;
    pEm->Unk139 = 0xA0;
    pEm->Unk13A = 450;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    pEm->Neck_no = 1;
    PW_NECK(pP)->Add_dir_y = 0x40;
    PW_NECK(pP)->Add_dir_z = 0x40;
    PW_NECK(pP)->L_rot_y = 0x200;
    PW_NECK(pP)->L_rot_z = 0x138;
    PW_NECK(pP)->N_rot_y = 0;
    PW_NECK(pP)->N_rot_z = 0;
    if (G.Status_flg & 0x80000000) {
        pEm->Life = G.Unk7852 + 50;
        if (pEm->Life > 200) {
            pEm->Life = 200;
        }
        pEm->Status_flg = G.Unk7850;
    } else {
        pEm->Life = G.Unk7852 = 200;
        pEm->Status_flg = G.Unk7850 = 0;
    }
    if (pEm->Type & 0x400) {
        pEm->Type &= ~0x1000;
        G.Status_flg |= 0x80000000;
        pEm->Max_life = 200;
        pEm->pUnk114 = PLAYER;
        pEm->Unk120 = 0;
        pEm->Routine_0 = 1;
        pEm->Routine_1 = 0;
    } else {
        pEm->Type |= 0x5000;
        pEm->Life = pEm->Max_life = -1;
        pEm->Routine_0 = 4;
        pEm->Routine_1 = 6;
    }
    SPL(pEm)->Unk1D7 = 0;
    SPL(pEm)->Unk1DE = 0;
    D_8009E70C[0] = 0;
    SPL(pEm)->Unk1D0 = 0;
    SPL(pEm)->Unk1D8 = 0;
    SPL(pEm)->Unk1D4 = 0;
    i = pEm->Parts_num;
    while (i >= 16) {
        i--;
        pP = &pEm->pSin_parts_ptr[i];
        pP->UnkA2 = 0;
    }
    if ((pEm->Unk4B & 1) && pEm->Id != 0x5B && pEm->Id != 0x5F && pEm->Id < 0x62) {
        pP = &pEm->pSin_parts_ptr[15];
        pHand = &pEm->pSin_parts_ptr[4];
        pTmd = pHand->pTmd;
        pHand->pTmd = pP->pTmd;
        pP->pTmd = pTmd;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/spl", func_80059310);

void Spl_set(void)
{
    G.Unk3EE0[0] = Spl;
    G.Unk3EE0[1] = Spl;
    G.Unk3EE0[2] = Spl;
    G.Unk3EE0[3] = Spl;
    G.Unk3EE0[4] = Spl;
    G.Unk3EE0[5] = Spl;
    G.Unk3EE0[6] = Spl;
    G.Unk3EE0[7] = Spl;
    G.Unk3EE0[8] = Spl;
    G.Unk3EE0[9] = Spl;
    G.Unk3EE0[10] = Spl;
    G.Unk3EE0[11] = Spl;
    G.Unk3EE0[12] = Spl;
    G.Unk3EE0[13] = Spl;
    G.Unk3EE0[14] = Spl;
    G.Unk3EE0[15] = Spl;
    G.Unk3EE0[16] = Spl;
    G.Unk3EE0[17] = Spl;
    G.Unk3EE0[18] = Spl;
    G.Unk3EE0[19] = Spl;
    G.Unk3EE0[20] = Spl;
    G.Unk3EE0[21] = Spl;
    G.Unk3EE0[22] = Spl;
    G.Unk3EE0[23] = Spl;
}

/* thrown back by the work in Add_dir's slot: turn away, play the knock-back moves */
void func_800594D4(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pUnk100_kan_t_ptr;
    ULONG *pSeq = pEm->pUnk104_seq_t_ptr;
    ENEMY_WORK *pTg;
    SHORT Dir;
    SHORT Base_x;
    SHORT Base_z;

    G.Status_flg |= 0x100;
    pTg = G.pEnemy[pEm->Unk14A];
    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Unk12C = 0;
        *(LONG *)&pEm->Move_no = pEm->Add_dir + 0x70000;
        pEm->Unk120 |= 0x12;
        pEm->Be_flg |= 0x2000;
    case 1:
        Goto01(pEm, pTg->Cdir_y + 0x800, 0x800);
        Dir = pEm->Cdir_y;
        pEm->Cdir_y = Dir + 0x800;
        Null_pos_set(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y = Dir;
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 2;
            pEm->UnkE3 = 4;
            pEm->Move_no++;
            pEm->Be_flg |= 0x2000;
        }
        break;
    case 2:
        pEm->Cdir_y += 0x800;
        Spd_get(pEm, pKan, pSeq, 0);
        Null_pos_set(pEm, (ULONG)pKan, (ULONG)pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (Joint_move2(pEm, pKan, pSeq, 0x200) && --pEm->UnkE3 == 0) {
            pEm->Routine_2 = 3;
            pEm->Move_no++;
            Pad_vib_set0(2, 0);
            Pad_vib_fade_set(0x2D, 0x96, 0, 0);
        }
        if (*pEm->pNow_seq & 0x200) {
            Pad_vib_set0(4, 0);
            Pad_vib_set1(6, 0xFA, 0);
            Pad_vib_fade_set(0x19, 0x96, 0x28, 6);
        }
        break;
    case 3:
        if (pEm->Move_cnt == 0) {
            Base_x = pEm->Spd_base_x;
            Base_z = pEm->Spd_base_z;
        }
        pEm->Cdir_y += 0x800;
        Spd_get(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= 0x800;
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x -= Base_x;
            pEm->Spd_z -= Base_z;
        }
        if (Joint_move2(pEm, pKan, pSeq, 0x200)) {
            pEm->Routine_2 = 4;
        }
        Add_speedXZ(pEm, 0x800);
        if (*pEm->pNow_seq & 0x200) {
            Pad_vib_set0(3, 0);
            Pad_vib_set1(6, 0xC8, 0);
            Pad_vib_set0(2, 8);
            Pad_vib_set1(4, 0xC8, 8);
        }
        break;
    case 4:
        break;
    }
}
