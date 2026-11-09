#include "sce0.h"
#include "em_sub.h"

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005003C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050208);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800502B8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800503E8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050524);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800505AC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050B58);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050D00);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050D28);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80050FE0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800510B0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005111C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800511B0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005127C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051284);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800512BC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800512FC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005136C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051388);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800513CC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800514C4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800514F0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051684);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800516A4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005190C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051B40);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051CB0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051D28);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051D60);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051E48);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80051F78);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052094);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800520D8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005211C);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052294);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005283C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800528D4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005290C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052968);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052A94);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80052B18);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054628);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800546CC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005472C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800547F0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005489C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800548C8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054960);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800549C4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054A04);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054A68);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054B48);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054BC0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054C28);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054C58);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054CA4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054CF4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054D48);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054D74);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054DEC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054E28);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054E70);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054F40);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80054FAC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055004);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055038);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005518C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800554C8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055520);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055568);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800555BC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055620);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055660);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800556A0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800556E0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055728);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800557A0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055884);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055944);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800559A0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055A18);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055AF0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055B30);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055B5C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055BBC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055C34);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055C94);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055D0C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055D7C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80055E38);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056004);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800560B8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056178);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005624C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005632C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056394);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800563E4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005648C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056510);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056950);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056A2C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056DC0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056E28);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80056E5C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057008);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057040);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005709C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057210);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057280);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800572C8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005734C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800573B0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800573DC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005745C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800574B8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800574F4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800576C4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800578A0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057A00);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057BD0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057CF8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057DD8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057E28);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057E74);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057EF4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057F44);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80057F84);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005863C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800586F4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005879C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058800);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800588F0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058918);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800589FC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058B00);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058B84);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058C24);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058C70);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058CD0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058DD4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80058F68);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059310);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059460);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800594D4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_800597F0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059824);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059A20);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059BC0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059C44);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059CC8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059D4C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059DD0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059E54);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059ED8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059F5C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_80059FE0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A064);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A0E8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A16C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A1F0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A274);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A35C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A418);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A4D8);

void func_8005A5AC(void) {
}

void func_8005A5B4(void) {
}

void func_8005A5BC(void) {
}

void func_8005A5C4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A5CC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A844);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005A948);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005AB14);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005ABC4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005AC9C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005AD48);

void func_8005ADBC(void) {
}

void func_8005ADC4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005ADCC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B0D0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B2CC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B438);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B614);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B824);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005B9D0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005BAD4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005BC28);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005BD24);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005BE84);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005BEE8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005BFC0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005BFFC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C098);

void func_8005C0D4(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C0DC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C210);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C2F8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C3F0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C638);

void func_8005C730(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C738);

void func_8005C81C(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C824);

void func_8005C8D0(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C8D8);

void func_8005C96C(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005C974);

void func_8005CA0C(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005CA14);

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

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D148);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D1A4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D208);

void func_8005D290(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D298);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D3B4);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D484);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D4D8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D554);

void func_8005D704(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005D70C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005DA44);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005DBCC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005DE3C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005DF4C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E1D0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E40C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E688);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E6CC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E7C8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005E90C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005EBC0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005EE14);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005EE70);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F01C);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F0BC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F298);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F660);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F7E8);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F8F0);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005F984);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005FA10);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005FAEC);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005FC18);

INCLUDE_ASM("asm/nonmatchings/game/8005003C", func_8005FEE0);
