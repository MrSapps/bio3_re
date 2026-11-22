#ifndef MONITOR_H
#define MONITOR_H
#include "bio3.h"

/* Task scheduler (RE2 MONITOR.C). Task slot is 0x80 bytes, same layout as RE2. */
typedef struct tagTASK {
    USHORT Tcb_status;                 /* 0x00 0 free, 1 sleep, 2 start, 4 resume, 0x7F run, |0x40 suspended */
    SHORT Tcb_sleep_ctr;               /* 0x02 */
    long (*pTcb_pc)(); /* 0x04 */
    ULONG Tcb_th;                      /* 0x08 */
    struct TCB *pTcb;                  /* 0x0C */
    ULONG Tcb_init_sp;                 /* 0x10 */
    struct EXEC Tcb_exec;              /* 0x14 */
    ULONG *pPc;                        /* 0x50 */
    USHORT R_no[6];                    /* 0x54 */
    SHORT Ctr[8];                      /* 0x60 */
    UCHAR Free[16];                    /* 0x70 */
} TASK;

typedef struct tagMONITOR {
    ULONG Gp;                          /* 0x000 */
    TASK Task[3];                      /* 0x004 */
    TASK *pCtcb;                       /* 0x184 */
} MONITOR;
extern MONITOR Monitor;                /* 0x800DCB90 */

/* Overlay task table entry (RE2 tagLOADTASK) */
typedef struct tagLOADTASK {
    ULONG File_no;
    void (*Func)();
    ULONG **Ld_adr;
} LOADTASK;

#define TASK0_STACK_TOP 0x801FFB00
#define TASK1_STACK_TOP 0x801FF400
#define TASK2_STACK_TOP 0x801FED00

void Init_scheduler(void);
void Scheduler(void);
void Task_execute(ULONG Level, void *pFuncall);
void Task_sleep(ULONG Sleep_counter);
void Task_exit(void);
void Task_kill(ULONG Level);
void Task_chain(void *pFuncall);
void Task_suspend(ULONG Level);
void Task_signal(ULONG Level);
ULONG Task_status(ULONG Level);

extern LOADTASK D_8009C944[];

ULONG Task_lexecute(ULONG Level, ULONG Task_no); /* RE2 Task_lexecute? */
void Task_lchain(ULONG Task_no);

#endif /* MONITOR_H */
