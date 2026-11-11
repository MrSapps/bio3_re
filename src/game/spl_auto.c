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

/* declarations from earlier parts of the original address-split file */
extern SHORT D_800DEC9C;               /* RE2 Item_box_entrance static Dir_spd */
extern SHORT D_800DEC9E;               /* RE2 Item_box_entrance static Spd_add */
extern OBJ_MODEL_WORK *D_800DECA0;     /* RE2 Item_box_entrance static pOwork */

extern UCHAR D_800B9E21;
void Xa_fade_set(ULONG frames);
void Pad_vib_init(void);

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

/* Goto record: SHORT jump offset at +4 */
typedef struct {
    UCHAR Dummy[4];
    SHORT Jmp_ofs;
} SCE_GOTO_REC;

/* Save record: SHORT value at +2 */
typedef struct {
    UCHAR Dummy[2];
    SHORT Set_val;
} SCE_SAVE_REC;

/* Calc record: USHORT command word at +2 (low byte operator, high byte variable), SHORT immediate at +4 */
typedef struct {
    UCHAR Dummy[2];
    USHORT Cmd_word;
    SHORT Imm;
} SCE_CALC_REC;

/* Member_set record: SHORT value at +2 */
typedef struct {
    UCHAR Dummy[2];
    SHORT D0;
} SCE_MEMBER_SET_REC;

extern UCHAR *D_800BA700[10];          /* RE2 pDwork[] */
/* G+0x265C viewed as one work list (RE2 G.pEnemy: [0] player, [1] sub player, [2..] enemies) */
typedef struct {
    UCHAR pad[0x265C];
    ENEMY_WORK *pWork[42];
} G_WORK_LIST;
#define G_pWork (((G_WORK_LIST *)&G)->pWork)

/* Speed record: USHORT value at +2 */
typedef struct {
    UCHAR Dummy[2];
    USHORT Spd_val;
} SCE_SPEED_REC;

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

void Spl_Auto(UCHAR *p)
{
    if (p[0x1DE] != 0) {
        p[0x1DE]--;
    }
    Aida_move(p);
}

void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);
LONG Guide_check(LONG X, LONG Z, LONG Floor);
LONG Spl_soba_pos_set(LONG Len, LONG Dir1, LONG Dir2, SVECTOR *pOut);
extern SHORT D_8009E770[6];
void Spl_Root_ck_sys(ENEMY_WORK *pEm)
{
    VECTOR Vec;
    SVECTOR Sv;
    ENEMY_WORK *pPl;
    LONG Dx, Dz;

    pPl = (ENEMY_WORK *)((UCHAR *)&G + 0x248C);
    if (!(G.Status_flg & 0x80000000) || pEm != G.pUnk2660) {
        Vec.vx = pEm->Unk14C;
        Vec.vz = pEm->Unk14E;
        Vec.vy = 0;
        Root_ck(pEm, &Vec, 0, 0);
        return;
    }
    switch (((USHORT *)pEm->Unk1CC)[2] & 3) {
    case 0:
        Spl_soba_pos_set(D_8009E770[0], D_8009E770[1], D_8009E770[2], &Sv);
        ((SHORT *)pEm->Unk1CC)[0] = Vec.vx = Sv.vx;
        Vec.vy = Sv.vy;
        ((SHORT *)pEm->Unk1CC)[1] = Vec.vz = Sv.vz;
        break;
    case 1:
        Vec.vx = ((SHORT *)pEm->Unk1CC)[0];
        Vec.vy = 0;
        Vec.vz = ((SHORT *)pEm->Unk1CC)[1];
        break;
    case 2:
        Spl_soba_pos_set(D_8009E770[3], D_8009E770[4], D_8009E770[5], &Sv);
        ((SHORT *)pEm->Unk1CC)[0] = Vec.vx = Sv.vx;
        Vec.vy = Sv.vy;
        ((SHORT *)pEm->Unk1CC)[1] = Vec.vz = Sv.vz;
        break;
    }
    if (Guide_check(Vec.vx, Vec.vz, Vec.vy / -1800) == 0xFF) {
        ((SHORT *)pEm->Unk1CC)[0] = pPl->Pos_x;
        ((SHORT *)pEm->Unk1CC)[1] = pPl->Pos_z;
        Vec.vx = pPl->Pos_x;
        Vec.vy = pPl->Pos_y;
        Vec.vz = pPl->Pos_z;
    }
    Root_ck(pEm, &Vec, 0, 0);
    Dx = (pEm->Pos_x - ((SHORT *)pEm->Unk1CC)[0]) >> 1;
    Dz = (pEm->Pos_z - ((SHORT *)pEm->Unk1CC)[1]) >> 1;
    pEm->pUnk114 = pPl;
    ((SHORT *)pEm->Unk1CC)[3] = SquareRoot0(Dx * Dx + Dz * Dz) << 1;
}

extern ENEMY_WORK *Near_em[3] __asm__("D_800E01A8"); /* D_800E01A8 seen as an array (Spl_em_pos_ck) */
void Aida_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG Old;
    ULONG F;

    Spl_enemy_ck(pEm, pKan, pSeq);
    if (D_8009E7D8[0] == 0 && (pEm->Type & 0xF) < 0xB) {
        Old = (UCHAR)pEm->Type & 0xF;
        pEm->Type &= 0xFF00;
        pEm->Type |= Spl_em_pos_ck(pEm, 0xBB8);
        F = ((USHORT *)pEm->Unk1CC)[2];
        ((USHORT *)pEm->Unk1CC)[2] = F & 0x7FFF;
        if (Old != (pEm->Type & 0xF)) {
            F &= 0x7FFC;
            ((USHORT *)pEm->Unk1CC)[2] = F;
            F |= *(D_8009E7C8 + (pEm->Type & 0xF));
            F |= 0x8000;
            ((USHORT *)pEm->Unk1CC)[2] = F;
        }
        D_8009E7D8[0] = 0xF;
    } else {
        D_8009E7D8[0]--;
    }
    if (Near_em[0] != NULL && Near_em[0]->Id == 0x34) {
        pEm->Type = (pEm->Type & 0xFFF0) | 0xB;
        pEm->pUnk114 = Near_em[0];
    }
    if ((pEm->Type & 0xF) == 0xB && Near_em[0] == NULL) {
        pEm->Type &= 0xFFF0;
    }
    D_8009E788[pEm->Type & 0xF](pEm, pKan, pSeq);
}

void Aida_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E7DC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E87C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E8CC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E91C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E96C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009E9BC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EA0C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EA5C)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EAAC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)D_8009EAFC)[pEm->Routine_1](pEm);
    D_8009E82C[pEm->Routine_1](pEm, pKan, pSeq);
}

void Aida_Move_type0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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

void Aida_em10_at00_pl(ENEMY_WORK *pEm, ULONG Type)
{
    ENEMY_WORK *pT;

    pT = D_800E01A8;
    if (((UCHAR *)pEm->Unk1CC)[0x12] != 0) {
        return;
    }
    if ((Type & 0xF) == 0xA) {
        pT = Spl_em_cdir_ck(pEm);
        if (pT == NULL) {
            return;
        }
        if (pT->Life < 0 || (pT->Type & 0x2000)) {
            pT = Spl_pllock_get(pEm, pT);
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

void Aida_em10_at00_spl(ENEMY_WORK *pEm)
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

void Aida_em25_at00_pl(ENEMY_WORK *pEm, ULONG Type)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    SHORT Dy;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if ((Type & 0xF) == 0xA) {
        pT = Spl_em_cdir_ck();
        if (pT == NULL) {
            return;
        }
        if (pT->Life < 0) {
            pT = Spl_pllock_get(pEm, pT);
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

void Aida_em25_at00_spl(ENEMY_WORK *pEm)
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

extern void (*D_8009EC9C[])(ENEMY_WORK *pEm, ULONG Type);

void Aida_mv00_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    LONG Life;
    LONG r;
    ULONG d;

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
    pT = D_800E01A8;
    if (pT != NULL && (G.Unk75E8 & 1)) {
        D_8009EC9C[D_8009EC3C[pT->Id - 0x10]](pEm, pEm->Type);
    }
    r = Goto00_ck(pEm, ((SHORT *)pEm->Unk1CC)[0], ((SHORT *)pEm->Unk1CC)[1], 0x400);
    if (((USHORT *)pEm->Unk1CC)[2] & 0x8000) {
        if (r == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0xC01;
        }
        return;
    }
    if ((pEm->Type & 0xF) == 0xA && ((USHORT *)pEm->Unk1CC)[3] > 0x1F4) {
        if (r == 0) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0xC01;
        }
        return;
    }
    if (Pl_work.Damage_cnt != 0) {
        ((USHORT *)pEm->Unk1CC)[10] &= ~2;
        if (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 1) < 0) {
            ((USHORT *)pEm->Unk1CC)[10] |= 2;
        }
    }
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
    } else if ((LONG)pEm->Unk154 < 0xFA0 && G.Pl_routine_1 == 2
               && ((LONG (*)(ENEMY_WORK *, ENEMY_WORK *))Cdir_ck2)(pEm, (ENEMY_WORK *)((UCHAR *)&G + 0x248C)) == 0
               && Goto00_ck(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x200) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    } else if ((LONG)pEm->Unk154 < 0x3E8 && G.Pl_routine_1 == 1
               && ((LONG (*)(ENEMY_WORK *, ENEMY_WORK *))Cdir_ck2)(pEm, (ENEMY_WORK *)((UCHAR *)&G + 0x248C)) == 0
               && Goto00_ck(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x200) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    }
}

void Aida_mv00_br_1(ENEMY_WORK *pEm)
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

void Aida_mv00(ENEMY_WORK *pEm)
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
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
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

void Aida_em10_at01(ENEMY_WORK *pEm)
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

void Aida_em25_at01(ENEMY_WORK *pEm)
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

extern void (*D_8009ECCC[])(ENEMY_WORK *pEm, ULONG Type);

void Aida_mv01_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    LONG r;
    ULONG D;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if ((pEm->UnkAE & 0x30) && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0
        && ++((SHORT *)pEm->Unk1CC)[6] >= 0x3D) {
        ((USHORT *)pEm->Unk1CC)[2] |= 0x1000;
        r = (((((LONG (*)(void))func_800102E8)() >> (((LONG (*)(void))func_800102E8)() & 3)) & 1) << 8) - 0x880;
        ((SHORT *)pEm->Unk1CC)[6] = 0x1E;
        ((SHORT *)pEm->Unk1CC)[4] = pEm->Cdir_y - r;
        return;
    }
    if ((pEm->UnkAE & 0x30) == 0 && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0) {
        ((SHORT *)pEm->Unk1CC)[6] = 0;
    }
    if ((((USHORT *)pEm->Unk1CC)[2] & 0x1000) && ((SHORT *)pEm->Unk1CC)[6] != 0) {
        Goto01(pEm, ((SHORT *)pEm->Unk1CC)[4], 0x28);
        if (--((SHORT *)pEm->Unk1CC)[6] == 0) {
            ((USHORT *)pEm->Unk1CC)[2] &= ~0x1000;
        }
        return;
    }
    if (pT != NULL) {
        if (D_8009E70C[0] != 0) {
            D_8009ECCC[D_8009EC3C[pT->Id - 0x10]](pEm, pEm->Type);
        }
        if (Dist < 0x7D0 && Dist < pEm->Unk154) {
            D_8009ECCC[D_8009EC3C[pT->Id - 0x10]](pEm, pEm->Type);
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
    if (D >= 0x1389) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
        return;
    }
    if (D < 0x7D0 && G.Pl_routine_1 == 2 && ((LONG (*)(ENEMY_WORK *, ENEMY_WORK *))Cdir_ck2)(pEm, (ENEMY_WORK *)((UCHAR *)&G + 0x248C)) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
        return;
    }
    if (Dist < 0x5DC && pT != NULL && Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x4B0) == 0 && (pEm->UnkAE & 0x30) == 0) {
        Goto00(pEm, pT->Pos_x, pT->Pos_z, -0x28);
    } else {
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x28);
    }
}

void Aida_mv01_br_1(ENEMY_WORK *pEm)
{
    LONG r;
    ULONG D;

    if ((pEm->UnkAE & 0x30) && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0
        && ++((SHORT *)pEm->Unk1CC)[6] >= 0x3D) {
        ((USHORT *)pEm->Unk1CC)[2] |= 0x1000;
        r = (((((LONG (*)(void))func_800102E8)() >> (((LONG (*)(void))func_800102E8)() & 3)) & 1) << 8) - 0x880;
        ((SHORT *)pEm->Unk1CC)[6] = 0x1E;
        ((SHORT *)pEm->Unk1CC)[4] = pEm->Cdir_y - r;
        return;
    }
    if ((pEm->UnkAE & 0x30) == 0 && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0) {
        ((SHORT *)pEm->Unk1CC)[6] = 0;
    }
    if ((((USHORT *)pEm->Unk1CC)[2] & 0x1000) && ((SHORT *)pEm->Unk1CC)[6] != 0) {
        Goto01(pEm, ((SHORT *)pEm->Unk1CC)[4], 0x28);
        if (--((SHORT *)pEm->Unk1CC)[6] == 0) {
            ((USHORT *)pEm->Unk1CC)[2] &= ~0x1000;
        }
        return;
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
    if (D >= 0x1389) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
        return;
    }
    if (D < 0x7D0 && G.Pl_routine_1 == 2 && ((LONG (*)(ENEMY_WORK *, ENEMY_WORK *))Cdir_ck2)(pEm, (ENEMY_WORK *)((UCHAR *)&G + 0x248C)) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
        return;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x28);
}

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

void Aida_mv01(ENEMY_WORK *pEm)
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
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
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

void Aida_mv02_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    LONG r;

    pT = D_800E01A8;
    Dist = D_800E01B4;
    if ((pEm->UnkAE & 0x30) && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0
        && ++((SHORT *)pEm->Unk1CC)[6] >= 0x3D) {
        ((USHORT *)pEm->Unk1CC)[2] |= 0x1000;
        r = (((((LONG (*)(void))func_800102E8)() >> (((LONG (*)(void))func_800102E8)() & 3)) & 1) << 8) - 0x880;
        ((SHORT *)pEm->Unk1CC)[6] = 0xF;
        ((SHORT *)pEm->Unk1CC)[4] = pEm->Cdir_y - r;
        return;
    }
    if ((pEm->UnkAE & 0x30) == 0 && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0) {
        ((SHORT *)pEm->Unk1CC)[6] = 0;
    }
    if ((((USHORT *)pEm->Unk1CC)[2] & 0x1000) && ((SHORT *)pEm->Unk1CC)[6] != 0) {
        Goto01(pEm, ((SHORT *)pEm->Unk1CC)[4], 0x50);
        if (--((SHORT *)pEm->Unk1CC)[6] == 0) {
            ((USHORT *)pEm->Unk1CC)[2] &= ~0x1000;
        }
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_800632B8(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0x1201;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x21201;
        }
    }
    if (((USHORT *)pEm->Unk1CC)[3] < 0xBB8) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        return;
    }
    if (Dist < 0x7D0 && pT != NULL && Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x400) == 0 && (pEm->UnkAE & 0x30) == 0) {
        Goto00(pEm, pT->Pos_x, pT->Pos_z, -0x38);
    } else {
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x38);
    }
}

void Aida_mv02_br_1(ENEMY_WORK *pEm)
{
    LONG r;

    if ((pEm->UnkAE & 0x30) && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0
        && ++((SHORT *)pEm->Unk1CC)[6] >= 0x3D) {
        ((USHORT *)pEm->Unk1CC)[2] |= 0x1000;
        r = (((((LONG (*)(void))func_800102E8)() >> (((LONG (*)(void))func_800102E8)() & 3)) & 1) << 8) - 0x880;
        ((SHORT *)pEm->Unk1CC)[6] = 0xF;
        ((SHORT *)pEm->Unk1CC)[4] = pEm->Cdir_y - r;
        return;
    }
    if ((pEm->UnkAE & 0x30) == 0 && (((USHORT *)pEm->Unk1CC)[2] & 0x1000) == 0) {
        ((SHORT *)pEm->Unk1CC)[6] = 0;
    }
    if ((((USHORT *)pEm->Unk1CC)[2] & 0x1000) && ((SHORT *)pEm->Unk1CC)[6] != 0) {
        Goto01(pEm, ((SHORT *)pEm->Unk1CC)[4], 0x50);
        if (--((SHORT *)pEm->Unk1CC)[6] == 0) {
            ((USHORT *)pEm->Unk1CC)[2] &= ~0x1000;
        }
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_800632B8(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0x1201;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x21201;
        }
    }
    if (((USHORT *)pEm->Unk1CC)[3] < 0xBB8) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        return;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, 0x38);
}

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

void Aida_mv02(ENEMY_WORK *pEm)
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
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
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

void Aida_mv03_br(ENEMY_WORK *pEm)
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

void Aida_mv03(ENEMY_WORK *pEm)
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
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
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

void Aida_mv04_br(ENEMY_WORK *pEm)
{
    if (Goto00_ck(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x50) == 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    } else {
        Goto00(pEm, G.Pl_pos_x, G.Pl_pos_z, 0x50);
    }
}

void Aida_mv04(ENEMY_WORK *pEm)
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
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (*pEm->pNow_seq & 0x200) {
            b = 0;
            if (*pEm->pNow_seq & 0x400) {
                b = 1;
            }
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        break;
    }
}

void Aida_mv05_br(ENEMY_WORK *pEm)
{
    D_8009ECDC[pEm->Routine_2](pEm);
}

void Aida_mv05(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    pT = D_800E01A8;
    Aida_mv05_w1(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    if (pT != NULL && (LONG)pEm->Be_flg < 0) {
        *(ULONG *)&pEm->Routine_0 = 0x1001;
        pEm->pEnemy_ptr = pT;
        ((USHORT *)pEm->Unk1CC)[10] &= ~2;
        return;
    }
    if (pEm->Routine_2 != 0 || pEm->UnkCA == 0) {
        Foot_set_pl(pEm, 0, 1);
    }
}

void Aida_mv05_w1(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ((EM_FUNC *)((UCHAR *)D_8009ECF8 + (pEm->Routine_2 << 2)))[0](pEm);
}

void Aida_mv05_w1_t0_br(void) {
}

void Aida_mv05_w1_t0(ENEMY_WORK *pEm)
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
            pEm->pEnemy_ptr = Spl_em_cdir_ck(pEm);
        }
        pEm->Unk120 |= 0x12;
        ((USHORT *)pEm->Unk1CC)[2] |= 0x4000;
        ((UCHAR *)pEm->Unk1CC)[0x13] = (func_800102E8() & 3) + 2;
    case 1:
        pT = pEm->pEnemy_ptr;
        if (pT != pEm) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xC4);
        }
        if (Joint_move(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200) != 0) {
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

extern void (*D_8009ED14[])(ENEMY_WORK *pEm, ULONG Type);

void Aida_mv05_w1_t1_br(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ENEMY_WORK *pN;
    ULONG i;
    LONG n;

    if ((((USHORT *)pEm->Unk1CC)[2] & 0x4000) == 0) {
        *(USHORT *)&pEm->Routine_2 = 3;
        return;
    }
    if (pEm->Routine_3 == 0) {
        return;
    }
    if (pEm->UnkDA != 0) {
        pEm->UnkDA--;
        return;
    }
    if (D_8009E70D[0] != pEm->Move_no - 2 && pEm->UnkCA != 0) {
        return;
    }
    pT = pEm->pEnemy_ptr;
    if ((pEm->Type & 0xF) == 0xF && pT->Damage_cnt != 0) {
        return;
    }
    D_8009ED14[D_8009EC3C[pT->Id - 0x10]](pEm, pEm->Type);
    if (pT->Id == 0x34) {
        return;
    }
    if (pT->Life >= 0 && ((pT->Type & 0x2000) == 0 || D_8009E70D[0] == 2)) {
        return;
    }
    *(USHORT *)&pEm->Routine_2 = 5;
    n = 3;
    i = 0;
    while (pT != Near_em[i]) {
        i++;
        if (--n == 0) {
            break;
        }
    }
    i++;
    if (i >= 3) {
        i = 0;
    }
    pT = Near_em[i];
    n = 3;
    while (pT->Life < 0 || (pT->Type & 0x2000)) {
        i++;
        if (i >= 3) {
            i = 0;
        }
        if (--n == 0) {
            break;
        }
        pT = Near_em[i];
    }
    if (n == 0) {
        *(USHORT *)&pEm->Routine_2 = 3;
        return;
    }
    pN = Near_em[i];
    pEm->pEnemy_ptr = pN;
    n = pN->Type & 0x2000;
    D_8009E70D[0] = 0;
    if (n) {
        D_8009E70D[0] = 2;
    }
}

void Aida_mv05_w1_t1(ENEMY_WORK *pEm)
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
    Joint_move(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200);
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
    if (Joint_move(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200) != 0) {
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
    if (Joint_move(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x10200) != 0) {
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
    if (Joint_move(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200) != 0) {
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
        Joint_move(pEm, pEm->Unk180[3], pEm->Unk180[2], 0x200);
        if (pEm->UnkCA == 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->Unk180[1], pEm->Unk180[0], 0x200);
        func_80079868(pEm, 0x200);
        break;
    }
}

LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);

void Aida_mv06_br(void) {
}

void Aida_mv06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70005;
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
    case 1:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        pEm->Routine_2 = 4;
        break;
    case 4:
        *(ULONG *)&pEm->Move_no = 0x70005;
        pEm->Routine_2 = 5;
    case 5:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x10200);
        break;
    case 6:
        *(ULONG *)&pEm->Routine_0 = 1;
        break;
    }
}

void Aida_mv07_br(void) {
}

void Aida_mv07(void) {
}

void Aida_mv08_br(void) {
}

void Aida_mv08(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Move_no = 0;
        if (pEm->Routine_2 & 2) {
            pEm->Move_no = 1;
        }
        pEm->UnkCA = 7;
        pEm->Move_cnt = 0;
        *(ULONG *)&pEm->Move_no = 0x70000;
        pEm->Spd_x = 0x32;
        pEm->Be_flg |= 0x8000;
    case 1:
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        if (pEm->Cdir_y & 0x200) {
            pEm->Cdir_y += (UCHAR)(pEm->Cdir_y >> 2);
        } else {
            pEm->Cdir_y -= (UCHAR)(pEm->Cdir_y >> 2);
        }
        if ((pEm->Cdir_y & 0x3E0) == 0) {
            pEm->Cdir_y &= 0xFF00;
            pEm->Routine_3 = 2;
            if (pEm->Routine_2 & 2) {
                pEm->Routine_3 = 0;
                pEm->Routine_1++;
            }
        }
        break;
    case 2:
        pEm->Routine_3 = 3;
        pEm->Move_no = 0;
        pEm->UnkCA = 3;
    case 3:
        if (pEm->Move_cnt == 0x11) {
            pEm->Damage_cnt |= 0x80;
        }
        if (pEm->Id == 0x45 || pEm->Id == 0x4F) {
            if (pEm->Move_cnt == 0x63) {
                pEm->UnkCA = 0;
                Add_speedXZ(pEm, 0);
                pEm->Pos_y -= 0x708;
                pEm->Unk122 -= 0x708;
                pEm->nFloor++;
            }
            if (pEm->Move_cnt == 0x77) {
                pEm->Damage_cnt &= 0x7F;
                pEm->Be_flg &= ~0x8000;
            }
        } else {
            if (pEm->Move_cnt == 0x1D) {
                Add_speedXZ(pEm, 0);
                pEm->Pos_y -= 0x708;
                pEm->Unk122 -= 0x708;
                pEm->nFloor++;
            }
            if (pEm->Move_cnt == 0x31) {
                pEm->Damage_cnt &= 0x7F;
                pEm->Be_flg &= ~0x8000;
            }
        }
        pEm->Routine_3 += Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x400);
        break;
    case 4:
        if ((LONG)G.Status_flg < 0 && pEm == G.pUnk2660) {
            pEm->Routine_1 = 0;
        }
        pEm->Routine_2 = 0;
        pEm->Routine_3 = 0;
        break;
    }
}

void Aida_mv09_br(void) {
}

void Aida_mv09(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pEm->Move_no = 1;
        pEm->UnkCA = 7;
        pEm->Move_cnt = 0;
        pEm->Be_flg |= 0x8000;
    case 1:
        Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        if (pEm->Id == 0x45 || pEm->Id == 0x4F) {
            if (pEm->Move_cnt == 0x1D) {
                pEm->Routine_3 = 2;
            }
        } else {
            if (pEm->Move_cnt == 0xE) {
                pEm->Routine_3 = 2;
            }
        }
        break;
    case 2:
        pEm->Routine_3 = 3;
        pEm->Move_no = 3;
        pEm->Move_cnt = 0;
        pEm->UnkCA = 0;
        pEm->Timer0 = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        Add_speedXZ(pEm, 0);
        pEm->Timer0 = 5;
        pEm->Pos_y += 0x226;
        pEm->nFloor--;
    case 3:
        pEm->Pos_y += pEm->Timer0++ * 30;
        pEm->Unk122 = func_8004D720((VECTOR *)&pEm->Pos_x, 0x190, 0x8000, 1);
        pEm->nFloor = pEm->Pos_y / -1800;
        Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        if (pEm->Unk122 - 300 < pEm->Pos_y) {
            pEm->Routine_3 = 4;
        }
        break;
    case 4:
        pEm->Routine_3 = 5;
        pEm->Move_no = 2;
        if ((pEm->Routine_2 & 0xC) >= 5) {
            pEm->Move_no = 1;
        }
        pEm->Move_cnt = 4;
        pEm->UnkCA = 7;
        pEm->Pos_y = pEm->Unk122;
        pEm->Damage_cnt &= 0x7F;
        pEm->Be_flg &= ~0x8000;
        G.Unk75E8 |= 4;
    case 5:
        pEm->nFloor = pEm->Pos_y / -1800;
        pEm->Routine_3 += Joint_move2(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr, 0x200);
        break;
    case 6:
        if ((LONG)G.Status_flg < 0 && pEm == G.pUnk2660) {
            pEm->Routine_1 = 0;
            pEm->Routine_2 = 0;
            pEm->Routine_3 = 0;
        }
        break;
    }
}

void Aida_mv0a_br(void) {
}

void Aida_mv_nop_br(void) {
}

void Aida_mv0a(ENEMY_WORK *pEm)
{
    if (Goto01_ck(pEm, G.Pl_cdir_y, 0x50) == 0) {
        *(ULONG *)&pEm->Routine_0 = 1;
    } else {
        Goto01(pEm, G.Pl_cdir_y, 0xA0);
    }
}

void Aida_mv0b_br(ENEMY_WORK *pEm)
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

void Aida_mv0b_br_1(ENEMY_WORK *pEm)
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

void Aida_mv0b(ENEMY_WORK *pEm)
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

void Aida_mv0c_br(ENEMY_WORK *pEm)
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

extern UCHAR D_8009DD34[];

void func_8005D70C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    UCHAR Tbl[8] = {0x0F, 0x0F, 0x0E, 0x0E, 0x0E, 0x0E, 0x10, 0x10};
    UCHAR Rot[8];
    UCHAR A[3];
    UCHAR B[2];
    UCHAR C[2];
    UCHAR Out[8];
    SHORT n;
    SHORT q;
    LONG k;
    ULONG j;
    LONG d;
    LONG m;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        n = (((pEm->Cdir_y & 0xFFF) / 0x200) + 1) & 7;
        for (k = 8; k != 0; k--) {
            Rot[n] = Tbl[8 - k];
            n++;
            if (n >= 8) {
                n = 0;
            }
        }
        q = (Direction_ck(pEm->Pos_x, pEm->Pos_z, pEm->pEnemy_ptr->Pos_x, pEm->pEnemy_ptr->Pos_z) & 0xFFF) / 0x200;
        d = (q + 4) & 7;
        A[1] = (d - 1) & 7;
        A[2] = (d + 1) & 7;
        B[0] = (d - 2) & 7;
        B[1] = (d - 3) & 7;
        C[0] = (d + 2) & 7;
        A[0] = d;
        C[1] = (d + 3) & 7;
        m = pEm->UnkC7 & 0xF;
        for (j = 0; j < 4; j++) {
            switch (D_8009DD34[j + m * 4]) {
            case 0:
                break;
            case 1:
                Out[k] = A[0];
                Out[k + 1] = A[1];
                Out[k + 2] = A[2];
                k += 3;
                break;
            case 2:
                Out[k] = B[0];
                Out[k + 1] = B[1];
                k += 2;
                break;
            case 3:
                Out[k] = C[0];
                Out[k + 1] = C[1];
                k += 2;
                break;
            }
        }
        d = Out[0];
        *(ULONG *)&pEm->Move_no = Rot[d] & 0x7F;
        pEm->Cdir_y = (d << 9) + 0x800;
        pEm->Damage_cnt = 3;
        if (pEm->Move_no == 0xF) {
            pEm->Cdir_y += 0x400;
        }
        if (pEm->Move_no == 0x10) {
            pEm->Cdir_y -= 0x400;
        }
        Snd_se_enem(0xB, pEm);
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 0x60501;
            return;
        }
        if ((pEm->UnkAE & 0x30) && (pEm->Damage_cnt & 0x7F)) {
            pEm->Damage_cnt--;
        }
    }
    Add_speedXZ(pEm, 0);
}

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

void func_8005DBCC(ENEMY_WORK *pEm)
{
    USHORT Flg;
    ULONG n;
    USHORT b;

    switch (pEm->Routine_2) {
    case 0:
        if ((func_800632B8(pEm, 1) & 0x1000) == 0) {
            goto walk;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, (ULONG *)pEm->Unk180[7], (ULONG *)pEm->Unk180[6], 0);
    case 1:
        Spd_get(pEm, (ULONG *)pEm->Unk180[7], (ULONG *)pEm->Unk180[6], 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            b = n != 0;
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move(pEm, pEm->Unk180[7], pEm->Unk180[6], 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->Unk180[5], pEm->Unk180[4], 0x200);
        func_80079868(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    walk:
        *(ULONG *)&pEm->Move_no = D_8009CDE0[D_8009E70C[0] + 3] + 0x70000;
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            b = n != 0;
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_800632B8(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_800632B8(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

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

void func_8005DF4C(ENEMY_WORK *pEm)
{
    USHORT Flg;
    ULONG n;
    USHORT b;

    switch (pEm->Routine_2) {
    case 0:
        if ((func_800634F4(pEm, 1) & 0x1000) == 0) {
            goto walk;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, (ULONG *)pEm->Unk180[7], (ULONG *)pEm->Unk180[6], 0);
    case 1:
        Spd_get(pEm, (ULONG *)pEm->Unk180[7], (ULONG *)pEm->Unk180[6], 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x = 30;
        }
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            b = n != 0;
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move(pEm, pEm->Unk180[7], pEm->Unk180[6], 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->Unk180[5], pEm->Unk180[4], 0x200);
        func_80079868(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    walk:
        *(ULONG *)&pEm->Move_no = D_8009CDE0[D_8009E70C[0] + 6] + 0x70000;
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            b = n != 0;
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_800634F4(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_800634F4(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void func_8005E1D0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;
    ULONG Dist;
    ULONG D;

    pT = D_800E01A8;
    Dist = D_800E01B4;
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
    if ((pEm->UnkAE & 0x40) == 0) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    }
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
    if (Dist < 0x5DC && (pEm->UnkAE & 0x30) == 0) {
        Goto00(pEm, pT->Pos_x, pEm->Pos_z, 0x30);
    } else {
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, -0x30);
    }
}

void func_8005E40C(ENEMY_WORK *pEm)
{
    USHORT Flg;
    ULONG n;
    USHORT b;

    switch (pEm->Routine_2) {
    case 0:
        if ((func_800632B8(pEm, 1) & 0x1000) == 0) {
            goto walk;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, (ULONG *)pEm->Unk180[7], (ULONG *)pEm->Unk180[6], 1);
    case 1:
        Spd_get(pEm, (ULONG *)pEm->Unk180[7], (ULONG *)pEm->Unk180[6], 1);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            b = n == 0;
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move(pEm, pEm->Unk180[7], pEm->Unk180[6], 0x10200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->Unk180[5], pEm->Unk180[4], 0x10200);
        func_80079868(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    walk:
        *(ULONG *)&pEm->Move_no = 0x70009;
        if (D_8009E70C[0] != 0) {
            *(ULONG *)&pEm->Move_no = 0x7000A;
        }
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            b = n != 0;
            Snd_se_walk(1, (b << 1) + b + 0xB, pEm);
            G.Unk75E8 |= 4;
        }
        Joint_move2(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_800632B8(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_800632B8(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void Spl_Damage(ENEMY_WORK *pEm)
{
    D_8009ED24[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

void Spl_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x30004;
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_enem(0, pEm);
        pEm->Be_flg &= ~0x8000;
    case 1:
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        *(ULONG *)&pEm->Routine_0 = 1;
        pEm->Damage_cnt = 0;
        pEm->Unk120 &= 0xED;
        break;
    }
}

void Spl_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        Snd_se_enem(pEm->Routine_2 - 1, pEm);
    case 1:
        if (pEm->UnkCA != 0) {
            Add_speedXZ(pEm, 0);
        }
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x400);
        break;
    case 2:
        *(ULONG *)&pEm->Routine_0 = 1;
        pEm->Damage_cnt = 0;
        pEm->Unk120 &= 0xED;
        break;
    }
}

void func_8005E90C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP1;
    PARTS_W *pP2;
    PARTS_W *pP3;

    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x3000A;
        pEm->Routine_2 = 1;
        pEm->Spd_x = -800;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_enem(1, pEm);
        pEm->Be_flg &= ~0x8000;
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
        if (pEm->UnkCA == 0) {
            *(ULONG *)&pEm->Move_no = 0x3000C;
            pEm->Routine_2 = 2;
        }
        Add_speedXZ(pEm, 0);
        Joint_move2(pEm, pKan, pSeq, 0x400);
        pEm->Spd_x += 0x96;
        break;
    case 2:
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= 0xED;
        }
        pEm->Spd_x += 10;
        if (pEm->Spd_x > 0) {
            pEm->Spd_x = 0;
        }
        break;
    case 3:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Move_no = 0x30009;
            pEm->Routine_2 = 4;
            pEm->Damage_cnt = 0;
        }
        break;
    case 4:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 5;
            *(ULONG *)&pEm->Move_no = 0x30005;
        }
        break;
    case 5:
        if (Joint_move2(pEm, pKan, pSeq, 0x10400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= 0xED;
        }
        break;
    }
}

void func_8005EBC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    PARTS_W *pP1;
    PARTS_W *pP2;
    PARTS_W *pP3;

    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x3000B;
        pEm->Spd_x = 400;
        pEm->Routine_2 = 1;
        pEm->Spd_y = 0;
        pEm->Spd_z = 0;
        pEm->Damage_cnt |= 0x80;
        pEm->Unk120 |= 0x12;
        Snd_se_enem(2, pEm);
        pEm->Be_flg &= ~0x8000;
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
        Add_speedXZ(pEm, 0);
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            *(ULONG *)&pEm->Move_no = 0x30009;
            pEm->Routine_2 = 2;
            pEm->Damage_cnt = 0;
        }
        if (pEm->UnkCA != 0) {
            pEm->Spd_x -= 20;
        } else {
            pEm->Spd_x -= 50;
        }
        if (pEm->Spd_x < 0) {
            pEm->Spd_x = 0;
        }
        break;
    case 2:
        if (Joint_move2(pEm, pKan, pSeq, 0x400)) {
            pEm->Routine_2 = 3;
            *(ULONG *)&pEm->Move_no = 0x30005;
        }
        break;
    case 3:
        if (Joint_move2(pEm, pKan, pSeq, 0x10400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            pEm->Unk120 &= 0xED;
        }
        break;
    }
}

void Spl_Die(ENEMY_WORK *pEm)
{
    G.Status_flg |= 0x100;
    D_8009ED84[pEm->Routine_1](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

void Spl_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        Snd_se_enem(3, pEm);
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
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    case 2:
        *(ULONG *)&pEm->Routine_0 = 7;
        break;
    }
    if (pEm->UnkCA == 0) {
        Foot_set_pl(pEm, 0, 1);
    }
}

void Spl_Dead(ENEMY_WORK *pEm)
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

LONG Spl_soba_pos_set(LONG Len, LONG Dir1, LONG Dir2, SVECTOR *pOut)
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

extern USHORT Em_l[3] __asm__("D_800E01B4");    /* RE2 Em_l */
extern SHORT Em_dir[3] __asm__("D_800E01A0");   /* RE2 Em_dir */
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);

void Spl_enemy_ck(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pNear[3];
    ENEMY_WORK **ppEnemy;
    ENEMY_WORK *pCur;
    LONG dx;
    LONG dz;
    ULONG dist;
    ULONG d0;
    ULONG d1;
    ULONG d2;
    tagGLOBAL *pG;

    d0 = 0x7FFFFFFF;
    d1 = 0x7FFFFFFF;
    d2 = 0x7FFFFFFF;
    pG = &G;
    memset(pNear, 0, sizeof(pNear));
    Near_em[0] = NULL;
    Near_em[1] = NULL;
    Near_em[2] = NULL;
    if (pG->Em_num != 0) {
        ppEnemy = pG->pEnemy;
        do {
            pCur = *ppEnemy;
            if ((pCur->Be_flg & 1) && (pCur->Type & 0xC000) == 0) {
                dx = (pCur->Pos_x - pEm->Pos_x) >> 1;
                dz = (pCur->Pos_z - pEm->Pos_z) >> 1;
                dist = SquareRoot0(dx * dx + dz * dz) << 1;
                pCur->Unk158 = dist;
                if (Sca_ck_line((VECTOR *)pEm->pSin_parts_ptr[pEm->Neck_no].Workm.t,
                                (VECTOR *)pCur->pSin_parts_ptr[pCur->Neck_no].Workm.t, 0x2040, 1) == 0
                    && pCur->Life >= 0) {
                    if (dist < d0) {
                        d2 = d1;
                        d1 = d0;
                        ppEnemy++;
                        d0 = dist;
                        pNear[2] = pNear[1];
                        pNear[1] = pNear[0];
                        pNear[0] = pCur;
                        continue;
                    } else if (dist < d1) {
                        d2 = d1;
                        ppEnemy++;
                        d1 = dist;
                        pNear[2] = pNear[1];
                        pNear[1] = pCur;
                        continue;
                    } else if (dist < d2) {
                        pNear[2] = pCur;
                        ppEnemy++;
                        d2 = dist;
                        continue;
                    }
                }
            }
            ppEnemy++;
        } while (ppEnemy != pG->ppEnemy_end);
        Em_l[0] = d0;
        Em_l[1] = d1;
        Em_l[2] = d2;
        Near_em[0] = pNear[0];
        Near_em[1] = pNear[1];
        Near_em[2] = pNear[2];
        if (pNear[0] != NULL) {
            Em_dir[0] = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear[0]->Pos_x, pNear[0]->Pos_z);
            pNear[0]->Unk158 |= 0x40000000;
            pNear[0]->Unk130 = (Em_dir[0] + 0x800) & 0xFFF;
        }
        if (pNear[1] != NULL) {
            Em_dir[1] = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear[1]->Pos_x, pNear[1]->Pos_z);
            pNear[1]->Unk158 |= 0x80000000;
            pNear[1]->Unk130 = (Em_dir[1] + 0x800) & 0xFFF;
        }
        if (pNear[2] != NULL) {
            Em_dir[2] = Direction_ck(pEm->Pos_x, pEm->Pos_z, pNear[2]->Pos_x, pNear[2]->Pos_z);
            pNear[2]->Unk158 |= 0xC0000000;
            pNear[2]->Unk130 = (Em_dir[2] + 0x800) & 0xFFF;
        }
        Print_color(5, 0, 0);
        Print8(0x20, 0x28, "%x %d %d", Near_em[0], Em_l[0], Em_dir[0]);
        Print8(0x20, 0x30, "%x %d %d", Near_em[1], Em_l[1], Em_dir[1]);
        Print8(0x20, 0x38, "%x %d %d", Near_em[2], Em_l[2], Em_dir[2]);
    }
}

ULONG Spl_em_pos_ck(ENEMY_WORK *pEm, ULONG Lim)
{
    ULONG ret_bits;
    ENEMY_WORK *pNear;
    ENEMY_WORK *pPl;
    ULONG idx;
    ULONG near_cnt;
    ULONG lim_cnt;
    USHORT *pDist;
    ENEMY_WORK **ppNear;

    ret_bits = 0;
    pPl = &Pl_work;
    near_cnt = 0;
    idx = 3;
    do {
        if (Near_em[idx - 1] != NULL) {
            near_cnt++;
        }
    } while (--idx != 0);
    lim_cnt = 0;
    idx = near_cnt - 1;
    if (near_cnt != 0) {
        pDist = Em_l;
        ppNear = Near_em;
        do {
            pNear = *ppNear;
            if (*pDist < Lim) {
                ret_bits |= Goto00_ck(pEm, pNear->Pos_x, pNear->Pos_z, 0x400) == 0 ? 0x10000 : 0x20000;
                lim_cnt++;
            }
            if (Goto00_ck(pPl, pNear->Pos_x, pNear->Pos_z, 0x400) == 0) {
                ret_bits |= 1;
            } else {
                ret_bits |= 2;
            }
            pDist++;
            ppNear++;
        } while (idx-- != 0);
    }
    if (near_cnt == 0) {
        return 0;
    }
    if (ret_bits & 0xFFFF0000) {
        if (lim_cnt > 1) {
            return (ret_bits >> 15) + 3;
        }
        return (ret_bits >> 15) - 1;
    }
    if (near_cnt < 2) {
        return ret_bits * 2;
    }
    return ret_bits * 2 + 4;
}

ENEMY_WORK *Spl_em_cdir_ck(ENEMY_WORK *pEm)
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

ENEMY_WORK *Spl_pllock_get(ENEMY_WORK *pEm, ENEMY_WORK *pEx)
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

LONG Spl_life_down(ENEMY_WORK *pEm, LONG Dmg, ULONG Flg)
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
        Snd_se_enem(0xF, pEm);
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

SHORT *func_8001B484_spl(ULONG Id, ULONG Dir, MATRIX *pM, SVECTOR *pV, SVECTOR *pDsv) __asm__("func_8001B484");
ULONG func_800102E8_spl(void) __asm__("func_800102E8");

void func_8005FC18(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    SHORT *pEsp;
    UCHAR Cnt;

    Cnt = pEm->Move_cnt;
    pP = &pEm->pSin_parts_ptr[4];
    if (Cnt == 0 || Cnt == 4 || Cnt == 7) {
        pM = &pP->Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484_spl(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        func_8001B484_spl(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x58C;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        func_8001B484_spl(0x801, 0xA000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x58C;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        func_8001B484_spl(0x801, 0xA000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x58C;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        func_8001B484_spl(0x801, 0xA000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        func_8001B484_spl(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484_spl(0x603, (USHORT)(signed char)func_800102E8_spl() | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = func_8001B484_spl(0x603, (USHORT)(signed char)func_800102E8_spl() | 0x10000000, pM, &Sv, 0);
        if (pEsp != (SHORT *)-1) {
            pEsp[8] = 0x14;
            pEsp[9] = -0x28;
        }
        if (func_80044804(pM, D_8009DBB4, 0xE) != 0) {
            ((USHORT *)pEm->Unk1CC)[2] |= 0x4000;
        }
        Snd_se_enem(0xF, pEm);
        G.Unk75E8 |= 1;
    }
}

void func_8005FEE0(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    PARTS_W *pO;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    SHORT *pEsp;

    if ((UCHAR)(pEm->Move_cnt % 3) == 0) {
        pP = pEm->pSin_parts_ptr;
        pO = &pP[4];
        pM = &pP[4].Workm;
        if (func_80044804(pM, D_8009DBB4, 0xF) != 0) {
            ((USHORT *)pEm->Unk1CC)[2] |= 0x4000;
        }
        Snd_se_enem(0xF, pEm);
        G.Unk75E8 |= 1;
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484_spl(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        func_8001B484_spl(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x58C;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        func_8001B484_spl(0x801, 0xA000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x58C;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        func_8001B484_spl(0x801, 0xA000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x58C;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        func_8001B484_spl(0x801, 0xA000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        func_8001B484_spl(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
    }
    if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484_spl(0x603, (USHORT)(signed char)func_800102E8_spl() | 0x10000000, &pO->Workm, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = func_8001B484_spl(0x603, (USHORT)(signed char)func_800102E8_spl() | 0x10000000, &pO->Workm, &Sv, 0);
        if (pEsp != (SHORT *)-1) {
            pEsp[8] = 0x14;
            pEsp[9] = -0x28;
        }
    }
}
