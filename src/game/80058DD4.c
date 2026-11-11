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

INCLUDE_ASM("asm/nonmatchings/game/80058DD4", func_80058F68);

INCLUDE_ASM("asm/nonmatchings/game/80058DD4", func_80059310);

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

INCLUDE_ASM("asm/nonmatchings/game/80058DD4", func_800594D4);
