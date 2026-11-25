/*
 * MONITOR.C (RE2 MONITOR.C, ~1:1): the three-level task scheduler on BIOS threads
 */
#include "monitor.h"
#include "mojidisp.h"
#include "cd.h"

/* system message No (Mess_sys_text + Mess_sys_ofs[No]); unused copy of mojidisp's Mess_sys_get */
static __inline__ UCHAR *Mess_sys_get(ULONG No)
{
    return Mess_sys_text + Mess_sys_ofs[No];
}

/* RE2 Init_scheduler: sets up the TCBs and stack tops of tasks 0..2 */
void Init_scheduler(void)
{
    LONG i;
    ULONG *pToT;
    struct TCB *pTcb;

    for (i = 0, pToT = (ULONG *)0x100; i < 3; i++) {
        Monitor.Task[i].Tcb_status = 0;
        pTcb = (struct TCB *)pToT[4] + (i + 1);
        Monitor.Task[i].pTcb = pTcb;
        pTcb->reg[35] = 0x40000404;
        Monitor.Task[i].Tcb_exec.gp = GetGp();
    }

    Monitor.Task[0].Tcb_init_sp = TASK0_STACK_TOP;
    Monitor.Task[1].Tcb_init_sp = TASK1_STACK_TOP;
    Monitor.Task[2].Tcb_init_sp = TASK2_STACK_TOP;
}

/* RE2 Scheduler: per frame runs each task that is new (opens its thread), woken or signalled */
void Scheduler(void)
{
    TASK *pCt;

    pCt = Monitor.Task;
    do {
        Monitor.pCtcb = pCt;
        switch (pCt->Tcb_status) {
        case 2:
            EnterCriticalSection();
            pCt->Tcb_th = OpenTh(pCt->pTcb_pc, pCt->Tcb_init_sp, pCt->Tcb_exec.gp);
            ExitCriticalSection();
            goto run;
        case 1:
            if (--pCt->Tcb_sleep_ctr > 0) {
                break;
            }
        case 4:
        run:
            pCt->Tcb_status = 0x7F;
            ChangeTh(pCt->Tcb_th);
            break;
        }
    } while (++pCt < &Monitor.Task[3]);
}

/* RE2 Task_lexecute: loads task file Task_no and starts it at Level; returns the size read */
ULONG Task_lexecute_(ULONG Level, ULONG Task_no)
{
    ULONG Size;

    Size = Cd_read(D_8009C944[Task_no].File_no, *D_8009C944[Task_no].Ld_adr, 0, "execute");
    Task_execute(Level, D_8009C944[Task_no].Func);
    return Size;
}

/* RE2 Task_lchain: loads task file Task_no and chains the current task to it */
void Task_lchain_(ULONG Task_no)
{
    Cd_read(D_8009C944[Task_no].File_no, *D_8009C944[Task_no].Ld_adr, 0, "chain");
    Task_chain(D_8009C944[Task_no].Func);
}

/* RE2 Task_execute: starts pFuncall as task Level */
void Task_execute(ULONG Level, void *pFuncall)
{
    Monitor.Task[Level].pTcb_pc = pFuncall;
    Monitor.Task[Level].Tcb_status = 2;
}

/* RE2 Task_sleep: suspends the current task for Sleep_counter frames */
void Task_sleep(ULONG Sleep_counter)
{
    TASK *pCt;

    pCt = Monitor.pCtcb;
    pCt->Tcb_sleep_ctr = Sleep_counter;
    pCt->Tcb_status = 1;
    ChangeTh(0xFF000000);
}

/* RE2 Task_exit: ends the current task */
void Task_exit(void)
{
    TASK *pCt;

    pCt = Monitor.pCtcb;
    pCt->Tcb_status = 0;
    EnterCriticalSection();
    CloseTh(pCt->Tcb_th);
    ExitCriticalSection();
    ChangeTh(0xFF000000);
}

/* RE2 Task_kill: ends task Level */
void Task_kill(ULONG Level)
{
    TASK *pT;

    pT = &Monitor.Task[Level];
    if (pT->Tcb_status != 0) {
        pT->Tcb_status = 0;
        EnterCriticalSection();
        CloseTh(pT->Tcb_th);
        ExitCriticalSection();
    }
}

/* RE2 Task_chain: restarts the current task at pFuncall */
void Task_chain(void *pFuncall)
{
    TASK *pCt;

    pCt = Monitor.pCtcb;
    pCt->Tcb_status = 2;
    pCt->pTcb_pc = pFuncall;
    EnterCriticalSection();
    CloseTh(pCt->Tcb_th);
    ExitCriticalSection();
    ChangeTh(0xFF000000);
}

/* RE2 Task_suspend: holds task Level (status bit 0x40) */
void Task_suspend(ULONG Level)
{
    Monitor.Task[Level].Tcb_status |= 0x40;
}

/* RE2 Task_signal: releases task Level */
void Task_signal(ULONG Level)
{
    Monitor.Task[Level].Tcb_status &= ~0x40;
}

/* RE2 Task_status: status of task Level */
ULONG Task_status(ULONG Level)
{
    return Monitor.Task[Level].Tcb_status;
}
