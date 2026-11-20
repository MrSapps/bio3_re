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
MATRIX *Get_matrix(s8 Work_kind, s8 Work_no);
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
void func_8001B484(ULONG A0, ULONG A1, MATRIX *pM, SVECTOR *pSv, ULONG A4);
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
        i = Search_item_id(Id, i);
        if (i == -1) {
            break;
        }
        p = G.pUnk7C7C + i * 4;
        if (((ITEM_TBL4 *)D_800A0514)[p[0]].Max >= p[1] + Num) {
            return 1;
        }
        i++;
    }
    i = Search_item_id(0, 0);
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
ULONG Door_aot_set(SCE_TASK *pSce)
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

ULONG Door_aot_set_4p(SCE_TASK *pSce)
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

ULONG Aot_set(SCE_TASK *pSce)
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

ULONG Aot_set_4p(SCE_TASK *pSce)
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

/* RE2 Aot_reset */
ULONG Aot_reset(SCE_TASK *pSce)
{
    UCHAR *pData;
    UCHAR *pAot;
    ULONG N;
    LONG Y, Z;

    pData = pSce->Data;
    pAot = (UCHAR *)G.Sce_aot[(s8)pData[1]];
    N = pAot[1];
    pAot[0] = pData[2];
    pAot[1] = pData[3] | (N & 0x80);
    if (N & 0x80) {
        pAot += 0x14;
    } else {
        pAot += 0xC;
    }
    Y = ((SHORT *)pData)[3];
    N = ((SHORT *)pData)[2];
    Z = ((SHORT *)pData)[4];
    ((SHORT *)pAot)[1] = Y;
    ((SHORT *)pAot)[0] = N;
    ((SHORT *)pAot)[2] = Z;
    pSce->Data = pData + 0xA;
    return 1;
}

/* RE2 Aot_on; RE3 also points G.pEm at the task's work (1 = player) */
ULONG Aot_on(SCE_TASK *pSce)
{
    UCHAR *pAot;
    UCHAR *pArg;

    pCAot = pAot = (UCHAR *)G.Sce_aot[pSce->Data[1]];
    if (pAot[1] & 0x80) {
        pArg = pAot + 0x14;
    } else {
        pArg = pAot + 0xC;
    }
    if (pSce->pWork == (ENEMY_WORK *)1) {
        G.pEm = (ENEMY_WORK *)&G.Pl_be_flg;
    } else {
        G.pEm = pSce->pWork;
    }
    Sce_at_jump_tbl[pAot[0]](pArg);
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sce2", Get_matrix);

/* RE2 Sce_espr_on */
ULONG Sce_espr_on(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[1];
    Dir = ((USHORT *)pData)[2];
    Mat = Get_matrix((s8)pData[4], (SHORT)Dir >> 8);
    Dir = ((USHORT *)pData)[3];
    Svec.vx = ((SHORT *)pData)[4];
    Svec.vy = ((SHORT *)pData)[5];
    Svec.vz = ((SHORT *)pData)[6];
    func_8001B484((((D0 >> 8) & 0xF) << 24) | ((D0 & 0xFF) << 8) | pData[1],
                  (Dir << 16) | ((USHORT *)pData)[7], Mat, &Svec, 0);
    pSce->Data = pData + 0x10;
    return 1;
}

/* RE2 Sce_espr_on2 */
ULONG Sce_espr_on2(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[2];
    Dir = ((USHORT *)pData)[3];
    Mat = Get_matrix((s8)pData[6], (SHORT)Dir >> 8);
    Dir = ((USHORT *)pData)[4];
    Svec.vx = ((SHORT *)pData)[5];
    Svec.vy = ((SHORT *)pData)[6];
    Svec.vz = ((SHORT *)pData)[7];
    func_8001B484((((D0 >> 8) & 0xF) << 24) | (pData[2] << 16) | ((D0 & 0xFF) << 8) | pData[3],
                  (Dir << 16) | ((USHORT *)pData)[8], Mat, &Svec, 0);
    pSce->Data = pData + 0x12;
    return 1;
}

/* RE2 Sce_espr3d_on */
ULONG Sce_espr3d_on(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;
    SVECTOR Svec2;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[1];
    Dir = (SHORT)((USHORT *)pData)[2];
    Mat = Get_matrix((s8)Dir, (LONG)Dir >> 8);
    Dir = ((USHORT *)pData)[3];
    Svec.vx = ((SHORT *)pData)[4];
    Svec.vy = ((SHORT *)pData)[5];
    Svec.vz = ((SHORT *)pData)[6];
    Svec2.vx = ((SHORT *)pData)[7];
    Svec2.vy = ((SHORT *)pData)[8];
    Svec2.vz = ((SHORT *)pData)[9];
    func_8001B484((((D0 >> 8) & 0xF) << 24) | ((D0 & 0xFF) << 8) | pData[1],
                  (Dir << 16) | ((USHORT *)pData)[10], Mat, &Svec, (ULONG)&Svec2);
    pSce->Data = pData + 0x16;
    return 1;
}

/* RE2 Sce_espr3d_on2 */
ULONG Sce_espr3d_on2(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;
    SVECTOR Svec2;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[2];
    Dir = (SHORT)((USHORT *)pData)[3];
    Mat = Get_matrix((s8)Dir, (LONG)Dir >> 8);
    Dir = ((USHORT *)pData)[4];
    Svec.vx = ((SHORT *)pData)[5];
    Svec.vy = ((SHORT *)pData)[6];
    Svec.vz = ((SHORT *)pData)[7];
    Svec2.vx = ((SHORT *)pData)[8];
    Svec2.vy = ((SHORT *)pData)[9];
    Svec2.vz = ((SHORT *)pData)[10];
    func_8001B484((((D0 >> 8) & 0xF) << 24) | (pData[2] << 16) | ((D0 & 0xFF) << 8) | pData[3],
                  (Dir << 16) | ((USHORT *)pData)[11], Mat, &Svec, (ULONG)&Svec2);
    pSce->Data = pData + 0x18;
    return 1;
}

ULONG Sce_espr_kill(SCE_TASK *pSce)
{
    MATRIX *pM;

    pM = Get_matrix((s8)pSce->Data[3], (s8)pSce->Data[4]);
    func_8001B894((pSce->Data[2] << 8) | pSce->Data[1], pM);
    pSce->Data += 5;
    return 1;
}

ULONG Sce_espr_kill2(SCE_TASK *pSce)
{
    func_8001B894((pSce->Data[1] << 16) | 0x1000000, 0);
    pSce->Data += 2;
    return 1;
}

ULONG Sce_espr_task(SCE_TASK *pSce)
{
    LONG v;
    ULONG i;

    if (pSce->Data[1] != 6) {
        v = Get_matrix((s8)pSce->Data[1], (s8)pSce->Data[2]);
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

ULONG Sce_espr_control(SCE_TASK *pSce)
{
    UCHAR *pData;

    pData = pSce->Data;
    func_8001BA48((pData[2] << 8) | pData[1], Get_matrix((s8)pData[3], (s8)pData[4]),
                  pData[5] | (pData[6] << 16), pData[7] | (pData[8] << 16));
    pSce->Data = pData + 10;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sce2", Obj_model_set);

/* RE2 Mizu_div_set */
ULONG Mizu_div_set(SCE_TASK *pSce)
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

INCLUDE_ASM("asm/nonmatchings/game/sce2", Sce_em_set);

ULONG Plc_motion(SCE_TASK *pSce)
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

extern UCHAR D_8009E52C[][2];          /* RE2 Subpl_walk */
extern UCHAR D_8009E5CC[][2];          /* RE2 Subpl_run */
extern UCHAR D_8009E554[][2];          /* RE2 Subpl_back */
extern UCHAR D_8009E5A4[][2];          /* RE2 Subpl_ato */
extern UCHAR D_8009E57C[][2];          /* RE2 Subpl_dir */
/* RE2 Plc_dest */
ULONG Plc_dest(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pEm;
    ULONG Id;
    ULONG Mode;
    UCHAR Set_no;

    pEm = pSce->pWork;
    pData = pSce->Data;
    Id = pEm->Id;
    Mode = pData[2];
    if (!(pEm->Unk144 & 4) || pEm->Routine_1 != Mode) {
        *(ULONG *)&pEm->Routine_0 = (Mode << 8) | 4;
    }
    if (Id < 0x10) {
        switch (Mode) {
        case 4:
        case 0x12:
            pEm->Add_dir = D_8009E52C[Id][1];
            break;
        case 5:
            pEm->Add_dir = D_8009E5CC[Id][1];
            break;
        case 7:
            pEm->Add_dir = D_8009E554[Id][1];
            break;
        case 8:
            pEm->Add_dir = D_8009E5A4[Id][1];
            break;
        case 9:
            pEm->Add_dir = D_8009E57C[Id][1];
            break;
        }
    } else {
        switch (Mode) {
        case 4:
        case 0x12:
            pEm->Add_dir = 0x30;
            break;
        case 5:
            pEm->Add_dir = 0x48;
            break;
        case 7:
            pEm->Add_dir = 0x30;
            break;
        case 8:
            pEm->Add_dir = 0x30;
            break;
        case 9:
            pEm->Add_dir = 0x60;
            break;
        }
    }
    Set_no = pData[3];
    pEm->Unk144 = 0;
    pEm->Unk14A = 0;
    pEm->Unk146 = Set_no;
    pEm->Type |= 0x100;
    pEm->Dest_x = ((SHORT *)pData)[2];
    pEm->Dest_z = ((SHORT *)pData)[3];
    pEm->Unk152 = 0;
    pEm->Unk150 = 0;
    pEm->Unk14C = pEm->Dest_x;
    pEm->Unk14E = pEm->Dest_z;
    pSce->Data = pData + 8;
    return 1;
}

ULONG Plc_gun(SCE_TASK *pSce)
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

INCLUDE_ASM("asm/nonmatchings/game/sce2", Plc_gun_eff);

ULONG Plc_neck(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    ULONG Kind;
    PARTS_W *pParts;
    LONG A, B, C, D;

    pW = pSce->pWork;
    Kind = pSce->Data[1];
    pParts = pW->pSin_parts_ptr + 1;
    pW->Unk120 = 0x80;
    A = ((SHORT *)pSce->Data)[1];
    B = ((SHORT *)pSce->Data)[2];
    C = ((SHORT *)pSce->Data)[3];
    D = ((SHORT *)pSce->Data)[4];
    switch (Kind) {
    case 0:
        pW->Unk120 |= 0x12;
        break;
    case 2:
        pW->Unk120 |= 0x8;
        break;
    case 3:
        pW->Unk120 |= 0x2A;
        break;
    case 4:
        pW->Unk120 |= 0x58;
        break;
    case 1:
    case 5:
        pW->Unk120 |= 0x4;
        break;
    case 6:
        pW->Unk120 = 0;
        break;
    case 7:
        pW->Unk120 |= 0x1;
        break;
    case 8:
        pW->Unk120 |= 0x12;
        ((SHORT *)&pParts->Unk74)[0] = 0;
        ((SHORT *)&pParts->Unk74)[1] = 0;
        break;
    }
    pW->Unk164 = A;
    pW->Unk166 = B;
    pW->Unk168 = C;
    if (Kind == 5) {
        switch (pW->Unk164) {
        case 1:
            pW->Unk164 = 0;
            break;
        case 2:
            pW->Unk164 = 1;
            break;
        default:
            pW->Unk164 = pW->Unk166 + 2;
            break;
        }
        pW->Unk166 = -0x8000;
    }
    ((UCHAR *)pParts)[0x80] = D;
    ((UCHAR *)pParts)[0x81] = (ULONG)D >> 8;
    pSce->Data += 10;
    return 1;
}

ULONG Plc_ret(SCE_TASK *pSce)
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

ULONG Plc_stop(SCE_TASK *pSce)
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

ULONG Plc_flg(SCE_TASK *pSce)
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

ULONG Plc_rot(SCE_TASK *pSce)
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

ULONG Plc_cnt(SCE_TASK *pSce)
{
    pSce->pWork->Move_cnt = pSce->Data[1];
    pSce->Data += 2;
    return 1;
}

ULONG Splc_ret(SCE_TASK *pSce)
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

ULONG Splc_sce(SCE_TASK *pSce)
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

INCLUDE_ASM("asm/nonmatchings/game/sce2", Item_aot_set);

INCLUDE_ASM("asm/nonmatchings/game/sce2", Item_aot_set_4p);

/* RE2 Kage_set */
ULONG Kage_set(SCE_TASK *pSce)
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
ULONG Super_set(SCE_TASK *pSce)
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

ULONG Weapon_chg(SCE_TASK *pSce)
{
    ENEMY_WORK *pPl;
    ULONG Wep;
    ULONG Flg;

    Wep = pSce->Data[1];
    pPl = (ENEMY_WORK *)((UCHAR *)&G + 0x248C);
    Flg = Wep & 0x80;
    if (Wep == 0xFF) {
        G.pUnk7C7C[0x128] = 0xFF;
        G.pUnk7C7C[0x129] = 0;
        pSce->Data += 2;
        return 1;
    } else {
        Wep &= 0x7F;
        pPl->Type = (pPl->Type & 0xFF00) | Wep;
        if (Wep != 0) {
            G.pUnk7C7C[0x128] = Search_item_id(Wep, 0);
            G.pUnk7C7C[0x129] = Wep;
            Weapon_cls(pPl);
            Pl_weapon_ch(pPl);
            D_800E0171[0] = 1;
        } else {
            G.pUnk7C7C[0x128] = 0xFF;
            G.pUnk7C7C[0x129] = 0;
            if (Flg) {
                Weapon_cls(pPl);
                Pl_weapon_ch(pPl);
                Flg_on(D_800E01D4, 1);
                D_800E0171[0] = 1;
            } else {
                Weapon_parts_clr(pPl);
                D_800E0171[0] = 0;
            }
        }
    }
    pSce->Data += 2;
    return 1;
}

ULONG func_80057CF8(SCE_TASK *pSce)
{
    ENEMY_WORK *pPl;
    ULONG Wep;

    pPl = (ENEMY_WORK *)((UCHAR *)&G + 0x248C);
    Wep = D_800E01E8;
    pPl->Type = (pPl->Type & 0xFF00) | Wep;
    if (Wep != 0) {
        G.pUnk7C7C[0x128] = Search_item_id(Wep, 0);
        G.pUnk7C7C[0x129] = Wep;
    } else {
        G.pUnk7C7C[0x128] = 0xFF;
        G.pUnk7C7C[0x129] = 0;
    }
    if (Sce.Unk1489 != 0) {
        Weapon_cls(pPl);
        Pl_weapon_ch(pPl);
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

INCLUDE_ASM("asm/nonmatchings/game/sce2", Flr_set);

ULONG Sce_Item_lost(SCE_TASK *pSce)
{
    func_8006CD68(Search_item_id(pSce->Data[1], 0));
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

INCLUDE_ASM("asm/nonmatchings/game/sce2", func_80057F84);

ULONG Sce_parts_bomb(SCE_TASK *pSce)
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

ULONG Sce_parts_down(SCE_TASK *pSce)
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

ULONG func_80058800(SCE_TASK *pSce)
{
    UCHAR *pData;
    UCHAR *p;
    UCHAR *q;
    UCHAR *pHdr;
    ULONG n;
    ULONG k;

    pData = pSce->Data;
    pSce->Data = pData + 4;
    if (G.Unk2470 != 0) {
        p = G.pRoom->pUnk40;
        if (((USHORT *)p)[1] <= G.Cut_no) {
            return 1;
        }
        pHdr = p + ((ULONG *)p)[pData[1] + 1];
        k = pData[2];
        n = *(USHORT *)pHdr;
        q = pHdr + 4;
        for (; n != 0; n--, k--) {
            if (k == 0) {
                if (pData[3] != 0) {
                    q[0] &= 0xFD;
                } else {
                    q[0] |= 2;
                }
                break;
            }
            if (q[1] == 0) {
                q += 0xC;
            } else {
                q += 0x14;
            }
        }
    }
    return 1;
}

void func_800588F0(SCE_TASK *pSce)
{
    D_800D442C[0] = pSce->Data[1];
    pSce->Data += 2;
}

/* RE2 Parts_set */
ULONG Parts_set(SCE_TASK *pSce)
{
    UCHAR *pData;
    LONG Kind;
    LONG No;
    USHORT Val;
    PARTS_W *pParts;

    pData = pSce->Data;
    Kind = (s8)pData[3];
    No = (s8)pData[2];
    Val = ((USHORT *)pData)[2];
    if ((ULONG)Kind >= 10) {
        pParts = (*(PARTS_W **)((UCHAR *)pSce->pWork + 0xC4)) + No;
    } else {
        pParts = pSce->pWork->pSin_parts_ptr + No;
    }
    switch (Kind) {
    case 2:
        *(USHORT *)&pParts->Be_flg = Val;
        break;
    case 3:
        *((USHORT *)&pParts->Be_flg + 1) = Val;
        break;
    case 4:
        pParts->Cdir_x = Val;
        break;
    case 5:
        pParts->Cdir_y = Val;
        break;
    case 6:
        pParts->Cdir_z = Val;
        break;
    case 7:
        pParts->Unk90 = Val;
        break;
    case 8:
        pParts->Unk92 = Val;
        break;
    case 9:
        pParts->Unk8E = Val;
        break;
    case 0:
    case 10:
        *(USHORT *)&pParts->Poly_rgb = Val;
        break;
    case 1:
    case 11:
        ((UCHAR *)&pParts->Poly_rgb)[2] = Val;
        break;
    }
    pSce->Data += 6;
    return 1;
}

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

extern UCHAR D_800C7961;
ULONG func_80058B84(SCE_TASK *pSce)
{
    tagGLOBAL *pG;
    ULONG Key;

    pSce->Data++;
    pG = &G;
    if (pG->Pl_damage_cnt == 0) {
        Key = pG->Key;
        if (!(Key & 0x100) || pG->Unk24D2 == 0) {
            if (Key & 0x80) {
                if (!(pG->Status_flg & 0x100)) {
                    if (!(D_800DBB58 & 0xC0) && D_800C7961 == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

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
