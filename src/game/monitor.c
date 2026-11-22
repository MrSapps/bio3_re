#include "monitor.h"
#include "mojidisp.h"
#include "cd.h"

static __inline__ UCHAR *Msg_get_80031BDC(ULONG No)
{
    return D_80098E88 + D_80099654[No];
}

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

ULONG Task_lexecute_(ULONG Level, ULONG Task_no)
{
    ULONG Size;

    Size = Cd_read(D_8009C944[Task_no].File_no, *D_8009C944[Task_no].Ld_adr, 0, "execute");
    Task_execute(Level, D_8009C944[Task_no].Func);
    return Size;
}

void Task_lchain_(ULONG Task_no)
{
    Cd_read(D_8009C944[Task_no].File_no, *D_8009C944[Task_no].Ld_adr, 0, "chain");
    Task_chain(D_8009C944[Task_no].Func);
}

void Task_execute(ULONG Level, void *pFuncall)
{
    Monitor.Task[Level].pTcb_pc = pFuncall;
    Monitor.Task[Level].Tcb_status = 2;
}

void Task_sleep(ULONG Sleep_counter)
{
    TASK *pCt;

    pCt = Monitor.pCtcb;
    pCt->Tcb_sleep_ctr = Sleep_counter;
    pCt->Tcb_status = 1;
    ChangeTh(0xFF000000);
}

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

void Task_suspend(ULONG Level)
{
    Monitor.Task[Level].Tcb_status |= 0x40;
}

void Task_signal(ULONG Level)
{
    Monitor.Task[Level].Tcb_status &= ~0x40;
}

ULONG Task_status(ULONG Level)
{
    return Monitor.Task[Level].Tcb_status;
}
