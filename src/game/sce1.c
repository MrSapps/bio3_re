#include "sce0.h"
#include "em_sub.h"
#include "main.h"
#include "sca.h"
#include "line_scr.h"
#include "sub.h"
#include "player.h"
#include "om.h"
#include "snd.h"



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

/* RE2 Member_cmp */
ULONG Member_cmp(SCE_TASK *pSce)
{
    LONG Cmp_word;
    LONG Cmp_val;
    LONG Mem_val;
    UCHAR *pData;

    pData = pSce->Data;
    Cmp_word = *(USHORT *)(pData + 2);
    Cmp_val = *(SHORT *)(pData + 4);
    pSce->Data = pData + 6;
    Mem_val = Load_member_branch(pSce->pWork, Cmp_word & 0xFF);
    switch (Cmp_word >> 8) {
    case 0:
        return Mem_val == Cmp_val;
    case 1:
        return Cmp_val < Mem_val;
    case 2:
        return Mem_val >= Cmp_val;
    case 3:
        return Mem_val < Cmp_val;
    case 4:
        return Cmp_val >= Mem_val;
    case 5:
        return Mem_val != Cmp_val;
    case 6:
        return (Mem_val & Cmp_val) != 0;
    }
}

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

/* RE2 Member_set_branch */
void Member_set_branch(ENEMY_WORK *pW, ULONG Member, LONG D0)
{
    switch (Member) {
    case 0:
        *(USHORT *)&pW->Be_flg = D0;
        break;
    case 1:
        *((USHORT *)&pW->Be_flg + 1) = D0;
        break;
    case 2:
        pW->Routine_0 = D0;
        break;
    case 3:
        pW->Routine_1 = D0;
        break;
    case 4:
        pW->Routine_2 = D0;
        break;
    case 5:
        pW->Routine_3 = D0;
        break;
    case 6:
        pW->Id = D0;
        break;
    case 7:
        pW->Type = D0;
        break;
    case 8:
        pW->Attribute = D0;
        break;
    case 9:
        pW->Pos_x = D0;
        break;
    case 10:
        pW->Pos_y = D0;
        break;
    case 11:
        pW->Pos_z = D0;
        break;
    case 12:
        pW->Cdir_x = D0;
        break;
    case 13:
        pW->Cdir_y = D0;
        break;
    case 14:
        pW->Cdir_z = D0;
        break;
    case 15:
        pW->nFloor = D0;
        break;
    case 16:
        pW->Status_flg = D0;
        break;
    case 17:
        pW->Unk122 = D0;
        break;
    case 18:
        pW->Dest_x = D0;
        break;
    case 19:
        pW->Dest_z = D0;
        break;
    case 20:
        pW->Unk144 = D0;
        break;
    case 21:
        pW->Add_dir = D0;
        break;
    case 22:
        pW->Unk14A = D0;
        break;
    case 23:
        pW->Unk14C = D0;
        break;
    case 24:
        pW->Unk14E = D0;
        break;
    case 25:
        *(UCHAR *)&pW->Spd_x = D0;
        break;
    case 26:
        pW->Spd_x = D0;
        break;
    case 27:
        pW->Spd_y = D0;
        break;
    case 28:
        pW->Spd_z = D0;
        break;
    case 29:
        *(USHORT *)&pW->Unk1CC[0] = D0;
        break;
    case 30:
        *((USHORT *)&pW->Unk1CC[0] + 1) = D0;
        break;
    case 31:
        pW->Damage_cnt = D0;
        break;
    case 32:
        D_800E0150.Pos[0] = D0;
        break;
    case 33:
        D_800E0150.Pos[1] = D0;
        break;
    case 34:
        D_800E0150.Pos[2] = D0;
        break;
    case 35:
        D_800E0150.Tgt[0] = D0;
        break;
    case 36:
        D_800E0150.Tgt[1] = D0;
        break;
    case 37:
        D_800E0150.Tgt[2] = D0;
        break;
    case 38:
        pW->Life = D0;
        break;
    case 39:
        pW->UnkC = D0;
        break;
    case 40:
        pW->Unk12A = D0;
        break;
    case 41:
        pW->Unk12C = D0;
        break;
    case 42:
        pW->UnkD = D0;
        break;
    }
}

/* RE2 Load_member_branch */
LONG Load_member_branch(ENEMY_WORK *pW, ULONG Member)
{
    switch (Member) {
    case 0:
        return *(USHORT *)&pW->Be_flg;
    case 1:
        return *((USHORT *)&pW->Be_flg + 1);
    case 2:
        return pW->Routine_0;
    case 3:
        return pW->Routine_1;
    case 4:
        return pW->Routine_2;
    case 5:
        return pW->Routine_3;
    case 6:
        return pW->Id;
    case 7:
        return pW->Type;
    case 8:
        return pW->Attribute;
    case 9:
        return pW->Pos_x;
    case 10:
        return pW->Pos_y;
    case 11:
        return pW->Pos_z;
    case 12:
        return pW->Cdir_x;
    case 13:
        return pW->Cdir_y;
    case 14:
        return pW->Cdir_z;
    case 15:
        return pW->nFloor;
    case 16:
        return pW->Status_flg;
    case 17:
        return pW->Unk122;
    case 18:
        return pW->Dest_x;
    case 19:
        return pW->Dest_z;
    case 20:
        return pW->Unk144;
    case 21:
        return pW->Add_dir;
    case 22:
        return pW->Unk14A;
    case 23:
        return pW->Unk14C;
    case 24:
        return pW->Unk14E;
    case 25:
        return *(UCHAR *)&pW->Spd_x;
    case 26:
        return pW->Spd_x;
    case 27:
        return pW->Spd_y;
    case 28:
        return pW->Spd_z;
    case 29:
        return *(USHORT *)&pW->Unk1CC[0];
    case 30:
        return *((USHORT *)&pW->Unk1CC[0] + 1);
    case 31:
        return pW->Damage_cnt;
    case 32:
        return D_800E0150.Pos[0];
    case 33:
        return D_800E0150.Pos[1];
    case 34:
        return D_800E0150.Pos[2];
    case 35:
        return D_800E0150.Tgt[0];
    case 36:
        return D_800E0150.Tgt[1];
    case 37:
        return D_800E0150.Tgt[2];
    case 38:
        return pW->Life;
    case 39:
        return pW->UnkC;
    case 40:
        return pW->Unk12A;
    case 41:
        return pW->Unk12C;
    case 42:
        return pW->UnkD;
    }
}

/* RE2 Load_member_addr_branch */
SHORT *Load_member_addr_branch(ENEMY_WORK *pW, ULONG Member)
{
    switch (Member) {
    case 0:
        return (SHORT *)pW;
    case 1:
        return (SHORT *)&*((USHORT *)&pW->Be_flg + 1);
    case 2:
        return (SHORT *)&pW->Routine_0;
    case 3:
        return (SHORT *)&pW->Routine_1;
    case 4:
        return (SHORT *)&pW->Routine_2;
    case 5:
        return (SHORT *)&pW->Routine_3;
    case 6:
        return (SHORT *)&pW->Id;
    case 7:
        return (SHORT *)&pW->Type;
    case 8:
        return (SHORT *)&pW->Attribute;
    case 9:
        return (SHORT *)&pW->Pos_x;
    case 10:
        return (SHORT *)&pW->Pos_y;
    case 11:
        return (SHORT *)&pW->Pos_z;
    case 12:
        return (SHORT *)&pW->Cdir_x;
    case 13:
        return (SHORT *)&pW->Cdir_y;
    case 14:
        return (SHORT *)&pW->Cdir_z;
    case 15:
        return (SHORT *)&pW->nFloor;
    case 16:
        return (SHORT *)&pW->Status_flg;
    case 17:
        return (SHORT *)&pW->Unk122;
    case 18:
        return (SHORT *)&pW->Dest_x;
    case 19:
        return (SHORT *)&pW->Dest_z;
    case 20:
        return (SHORT *)&pW->Unk144;
    case 21:
        return (SHORT *)&pW->Add_dir;
    case 22:
        return (SHORT *)&pW->Unk14A;
    case 23:
        return (SHORT *)&pW->Unk14C;
    case 24:
        return (SHORT *)&pW->Unk14E;
    case 25:
        return (SHORT *)&*(UCHAR *)&pW->Spd_x;
    case 26:
        return (SHORT *)&pW->Spd_x;
    case 27:
        return (SHORT *)&pW->Spd_y;
    case 28:
        return (SHORT *)&pW->Spd_z;
    case 29:
        return (SHORT *)&*(USHORT *)&pW->Unk1CC[0];
    case 30:
        return (SHORT *)&*((USHORT *)&pW->Unk1CC[0] + 1);
    case 31:
        return (SHORT *)&pW->Damage_cnt;
    case 32:
        return (SHORT *)&D_800E0150.Pos[0];
    case 33:
        return (SHORT *)&D_800E0150.Pos[1];
    case 34:
        return (SHORT *)&D_800E0150.Pos[2];
    case 35:
        return (SHORT *)&D_800E0150.Tgt[0];
    case 36:
        return (SHORT *)&D_800E0150.Tgt[1];
    case 37:
        return (SHORT *)&D_800E0150.Tgt[2];
    case 38:
        return (SHORT *)&pW->Life;
    case 39:
        return (SHORT *)&pW->UnkC;
    case 40:
        return (SHORT *)&pW->Unk12A;
    case 41:
        return (SHORT *)&pW->Unk12C;
    case 42:
        return (SHORT *)&pW->UnkD;
    }
}

/* RE2 Fade_set (SCE) */
ULONG func_80054384(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Rgb0;
    ULONG Rgb1;

    pData = pSce->Data;
    Rgb0 = pData[6];
    Rgb0 |= pData[5] << 8;
    Rgb0 |= pData[4] << 16;
    Rgb1 = pData[9];
    Rgb1 |= pData[8] << 8;
    Rgb1 |= pData[7] << 16;
    Fade_set(pData[1], 7, pData[2], pData[3], Rgb0, Rgb1, pData[10]);
    pSce->Data = pData + 0xB;
    return 1;
}

extern UCHAR *D_800BA700[10];          /* RE2 pDwork[] */
/* G+0x265C viewed as one work list (RE2 G.pEnemy: [0] player, [1] sub player, [2..] enemies) */
typedef struct {
    UCHAR pad[0x265C];
    ENEMY_WORK *pWork[42];
} G_WORK_LIST;
#define G_pWork (((G_WORK_LIST *)&G)->pWork)
/* RE2 Work_set */
ULONG Work_set(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Kind;
    LONG Idx;
    ULONG *p;

    p = (ULONG *)&pSce->Spd_x;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    pData = pSce->Data;
    Kind = pData[1];
    Idx = (s8)pData[2];
    pSce->Data = pData + 3;
    switch (Kind) {
    case 1:
        pSce->pWork = G_pWork[0];
        break;
    case 2:
        pSce->pWork = G_pWork[1];
        break;
    case 3:
        pSce->pWork = G_pWork[Idx + 2];
        break;
    case 4:
        pSce->pWork = (ENEMY_WORK *)G.Ob_model[Idx];
        break;
    case 5:
        pSce->pWork = (ENEMY_WORK *)D_800BA700[Idx];
        break;
    }
    return 1;
}

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
ULONG Ck(SCE_TASK *pSce)
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
ULONG Set(SCE_TASK *pSce)
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

/* RE2 Cmp */
ULONG Cmp(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Cmp_word;
    LONG Cmp_val;
    LONG Val;

    pData = pSce->Data;
    Cmp_word = *(USHORT *)(pData + 2);
    Cmp_val = *(SHORT *)(pData + 4);
    pSce->Data = pData + 6;
    Val = *(G.Sce_work + (Cmp_word & 0xFF));
    switch (Cmp_word >> 8) {
    case 0:
        return Val == Cmp_val;
    case 1:
        return Cmp_val < Val;
    case 2:
        return Val >= Cmp_val;
    case 3:
        return Val < Cmp_val;
    case 4:
        return Cmp_val >= Val;
    case 5:
        return Val != Cmp_val;
    case 6:
        return (Val & Cmp_val) != 0;
    }
}

ULONG Sce_rnd(SCE_TASK *pSce)
{
    pSce->Data++;
    Sce_rnd_set();
    return 1;
}

ULONG Cut_chg(SCE_TASK *pSce)
{
    tagGLOBAL *pG = &G;
    UCHAR Cut;

    pG->Status_flg |= 0x80;
    Cut = pSce->Data[1];
    Cut_chg_main(Cut & 0x7F);
    pG->Status_flg |= 0x400000;
    if (Cut & 0x80) {
        D_800D442E[0] = 0;
        D_800E01C6[0] = 1;
    }
    pSce->Data += 2;
    return 1;
}

ULONG Cut_old(SCE_TASK *pSce)
{
    Cut_chg_main(D_800E0175[0]);
    G.Status_flg = (G.Status_flg | 0x400000) & ~0x80;
    pSce->Data++;
    return 1;
}

void Cut_chg_main(ULONG Cut)
{
    UCHAR Old;

    Old = G.Cut_no;
    G.Cut_no = Cut;
    D_800D442E[0] = 1;
    D_800E0175[0] = Old;
    Cut_change(Cut);
}

ULONG Cut_auto(SCE_TASK *pSce)
{
    if (pSce->Data[1] == 1) {
        G.Status_flg &= ~0x80;
    } else {
        G.Status_flg |= 0x80;
    }
    pSce->Data += 2;
    return 1;
}

ULONG Cut_replace(SCE_TASK *pSce)
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

INCLUDE_ASM("asm/nonmatchings/game/sce1", Cut_be_set);

ULONG Pos_set(SCE_TASK *pSce)
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

ULONG Dir_set(SCE_TASK *pSce)
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
ULONG Vib_set0(SCE_TASK *pSce)
{
    Pad_vib_set0(((USHORT *)pSce->Data)[1], ((USHORT *)pSce->Data)[2]);
    pSce->Data += 6;
    return 1;
}

/* RE2 Vib_set1 */
ULONG Vib_set1(SCE_TASK *pSce)
{
    Pad_vib_set1(((USHORT *)pSce->Data)[1], pSce->Data[1], ((USHORT *)pSce->Data)[2]);
    pSce->Data += 6;
    return 1;
}

/* RE2 Vib_fade_set */
ULONG Vib_fade_set(SCE_TASK *pSce)
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
ULONG Message_on(SCE_TASK *pSce)
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
ULONG Sce_shake_on(SCE_TASK *pSce)
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
ULONG Sce_line_start(SCE_TASK *pSce)
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
ULONG Sce_line_main(SCE_TASK *pSce)
{
    ((LONG (*)(LONG, LONG, LONG, LONG))line_trans)(pSce->Data[1], ((SHORT *)pSce->Data)[1], ((SHORT *)pSce->Data)[2], Main.pDb->Draw.ofs[1]);
    pSce->Data += 6;
    return 1;
}

/* RE2 Sce_line_end */
ULONG Sce_line_end(SCE_TASK *pSce)
{
    ((LONG (*)(void))line_scr_end)();
    pSce->Data++;
    return 1;
}

void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);

/* RE2 view of G.pEnemy: its list starts with the player and sub player at 0x265C */
typedef struct {
    UCHAR pad[0x265C];
    ENEMY_WORK *pEnemy[42];
} G_ENEMY_LIST;

/* RE2 Se_on (SCE2.C); RE3 sound code is D0 low/D0 high/Type packed differently */
ULONG Se_on(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    ULONG D1;
    ULONG Type;
    ENEMY_WORK *pW;
    LONG ox;
    LONG oy;
    LONG oz;
    VECTOR Vec;

    pData = pSce->Data;
    D1 = ((SHORT *)pData)[2];
    Type = pData[1];
    D0 = ((SHORT *)pData)[1];

    switch (D1 & 0xFF) {
    case 0:
        ox = 0;
        oy = 0;
        oz = 0;
        goto offset_done;
    case 1:
        pW = G.pUnk265C;
        break;
    case 2:
        pW = G.pUnk2660;
        break;
    case 3:
        pW = ((G_ENEMY_LIST *)&G)->pEnemy[(D1 >> 8) + 2];
        break;
    case 4:
        pW = (ENEMY_WORK *)G.Ob_model[D1 >> 8];
        break;
    }
    ox = pW->Pos_x;
    oy = pW->Pos_y;
    oz = pW->Pos_z;
offset_done:
    Vec.vx = ((SHORT *)pData)[3] + ox;
    Vec.vy = ((SHORT *)pData)[4] + oy;
    Vec.vz = ((SHORT *)pData)[5] + oz;
    Snd_se_stad((((D0 >> 8) & 0xFF) << 16) | (Type << 8) | (D0 & 0xFF), &Vec, 0, 0);
    pSce->Data = pData + 12;
    return 1;
}

void func_8007745C(LONG no);
void Snd_seq_play_ck(LONG no);

/* VAB head: header and program attributes */
typedef struct {
    VabHdr Hdr;                        /* 0x000 */
    ProgAtr Prog[128];                 /* 0x020 */
} VAB_HEAD;

/* VAB head of the bgm bank of sequence No */
#define BGM_VH(pW, No) ((VAB_HEAD *)(pW)->Bank[(No) ? 6 : 5].pHead)

/* bgm control: 0 volume/pan, 1 stop jingle, 2 stop, 3 play, 4 ?, 5 fade out */
ULONG Sce_bgm_control(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG No;
    ULONG Type;
    ULONG Prog;
    ULONG Vol;
    ULONG Pan;
    LONG D0;
    UCHAR Mvol;
    UCHAR Mpan;
    signed char i;
    SND_TRACK *pTrk;
    SND_WORK *pW;

    pW = &Snd_work;
    pData = pSce->Data;
    Type = pData[2];
    No = pData[1];
    Prog = pData[3];
    Vol = pData[4];
    Pan = pData[5];
    switch (Type) {
    case 0:
        if (Prog == 0) {
            if (Vol < 0x80) {
                BGM_VH(pW, No)->Hdr.mvol = Vol;
            }
            if (Pan < 0x80) {
                BGM_VH(pW, No)->Hdr.pan = Pan;
            }
            break;
        }
        if (Vol < 0x80) {
            BGM_VH(pW, No)->Prog[Prog - 1].mvol = Vol;
        }
        if (Pan < 0x80) {
            BGM_VH(pW, No)->Prog[Prog - 1].mpan = Pan;
        }
        D0 = BGM_VH(pW, No)->Hdr.mvol;
        Mvol = D0 * BGM_VH(pW, No)->Prog[Prog - 1].mvol / 127;
        Mpan = BGM_VH(pW, No)->Hdr.pan + 0xC0;
        Mpan = BGM_VH(pW, No)->Prog[Prog - 1].mpan + Mpan;
        pTrk = pW->Seq[No].Trk;
        for (i = 0; i < 16; i++, pTrk++) {
            if (pTrk->Prog == Prog - 1) {
                pTrk->Vol = Mvol;
                pTrk->Pan2 = Mpan;
            }
        }
        break;
    case 1:
        if (G.System_flg & 0x400000) {
            pW->UnkB77 = 1 << No;
        } else {
            Snd_se_stad((No + 5) << 8, 0, 0, 0);
        }
        break;
    case 2:
        if (G.System_flg & 0x400000) {
            pW->UnkB79 = 1 << No;
        } else {
            pW->UnkB79 = 0;
        }
        Snd_seq_stop(No);
        break;
    case 4:
        func_8007745C(No);
        break;
    case 3:
        Snd_seq_play_ck(No);
        break;
    case 5:
        if (pW->Seq[No].Unk2C != 0 && pW->Seq[No].Unk2D == 0) {
            if (pW->Seq[No].Unk38 != 2) {
                pW->Seq[No].Unk38 = 2;
                pW->Seq[No].Unk3C = 0x3FFF;
            }
            pW->Seq[No].Unk40 = 0x3FFF / Vol + 1;
        }
        break;
    }
    pSce->Data = pData + 6;
    return 1;
}

ULONG Xa_on(SCE_TASK *pSce)
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
ULONG Movie_on(SCE_TASK *pSce)
{
    Init_movie_work(pSce->Data[1]);
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
ULONG Keep_Item_ck(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Id;

    pData = pSce->Data;
    Id = pData[1];
    pSce->Data = pData + 2;
    return Search_item_id(Id, 0) >= 0;
}

/* RE2 Sce_key_ck */
ULONG Sce_key_ck(SCE_TASK *pSce)
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
ULONG Sce_trg_ck(SCE_TASK *pSce)
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
ULONG Sca_id_set(SCE_TASK *pSce)
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
ULONG Light_pos_set2(SCE_TASK *pSce)
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
ULONG Light_kido_set2(SCE_TASK *pSce)
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
ULONG Light_color_set2(SCE_TASK *pSce)
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
