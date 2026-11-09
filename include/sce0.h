#ifndef SCE0_H
#define SCE0_H
/* Scenario (SCE event script) work: RE2 SCE0.H / SCE1.C / SCE2.C */
#include "bio3.h"
#include "mojidisp.h"

/* One event script task (RE2 struct tagSCE_TASK, same layout in RE3, size 0x174) */
typedef struct tagSCE_TASK {
    UCHAR Routine0;                    /* 0x000 */
    UCHAR Status;                      /* 0x001 */
    s8 Sub_ctr;                        /* 0x002 */
    UCHAR Task_level;                  /* 0x003 */
    s8 Ifel_ctr[4];                    /* 0x004 */
    s8 Loop_ctr[4];                    /* 0x008 */
    s8 Loop_if_class[4][4];            /* 0x00C */
    UCHAR *Data;                       /* 0x01C */
    UCHAR *Lstack[4][4];               /* 0x020 */
    UCHAR *Lbreak[4][4];               /* 0x060 */
    SHORT Lcnt[4][4];                  /* 0x0A0 */
    ULONG Stack[4][8];                 /* 0x0C0 */
    ULONG *pS_SP;                      /* 0x140 */
    UCHAR *Ret_addr[4];                /* 0x144 */
    struct tagENEMY_WORK *pWork;       /* 0x154 */
    SHORT Spd_x;                       /* 0x158 */
    SHORT Spd_y;                       /* 0x15A */
    SHORT Spd_z;                       /* 0x15C */
    SHORT Dspd_x;                      /* 0x15E */
    SHORT Dspd_y;                      /* 0x160 */
    SHORT Dspd_z;                      /* 0x162 */
    SHORT Aspd_x;                      /* 0x164 */
    SHORT Aspd_y;                      /* 0x166 */
    SHORT Aspd_z;                      /* 0x168 */
    SHORT Adspd_x;                     /* 0x16A */
    SHORT Adspd_y;                     /* 0x16C */
    SHORT Adspd_z;                     /* 0x16E */
    ULONG R_no_bak;                    /* 0x170 */
} SCE_TASK;

#define SCE_TASK_MAX 14

/* Scenario state (RE2 struct tagSCENARIO; RE3 tail layout differs) */
typedef struct tagSCENARIO {
    SCE_TASK Sce_task[SCE_TASK_MAX];   /* 0x0000 */
    UCHAR pad1458[0x145C - 0x1458];    /* RE2 Random_base? */
    UCHAR *pCScd;                      /* 0x145C */
    UCHAR pad1460[0x1464 - 0x1460];
    UCHAR *pMizu_div;                  /* 0x1464 RE2 pMizu_div */
    UCHAR pad1468[0x1489 - 0x1468];
    UCHAR Unk1489;                     /* 0x1489 */
    UCHAR pad148A[0x148C - 0x148A];
    UCHAR Type;                        /* 0x148C 0: room script tasks (2..9), else door tasks (10..13) */
    UCHAR pad148D[0x1495 - 0x148D];
    UCHAR Mizu_div_max;                /* 0x1495 RE2 Mizu_div_max */
    UCHAR pad1496[0x149B - 0x1496];
    UCHAR Unk149B;                     /* 0x149B */
    UCHAR Unk149C;                     /* 0x149C */
    UCHAR Unk149D;                     /* 0x149D */
    UCHAR Unk149E;                     /* 0x149E index into D_80098728 (func_800523C4) */
} SCENARIO;
extern SCENARIO Sce;                   /* 0x800DECE8 */

typedef ULONG (*SCE_JMP_FUNC)(SCE_TASK *);
extern SCE_JMP_FUNC Sce_jmp_tbl[];      /* 0x8009E0F8, SCE opcode handlers */

void Event_init(SCE_TASK *pSce, ULONG Evt_no);  /* RE2 Event_init */
void Event_exec(ULONG Task_level, ULONG Evt_no); /* RE2 Event_exec */

ULONG While_main(SCE_TASK *pSce, UCHAR *pData, UCHAR Ofs); /* RE2 While_main */

void Calc_branch(ULONG Exp, SHORT *Ans, ULONG Num); /* RE2 Calc_branch */

void Member_set_branch(struct tagENEMY_WORK *pW, ULONG Member, LONG D0); /* RE2 Member_set_branch */
LONG Load_member_branch(struct tagENEMY_WORK *pW, ULONG Member);         /* RE2 Load_member_branch */
SHORT *Load_member_addr_branch(struct tagENEMY_WORK *pW, ULONG Member);       /* RE2 Load_member_addr_branch */

#endif /* SCE0_H */
