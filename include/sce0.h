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

/* room object model link (RE2 struct tagMODEL_LINK_INFO, ROOM_WORK.pOmodel) */
typedef struct tagMODEL_LINK_INFO {
    ULONG *Tim_adr;                    /* 0x0 */
    ULONG *Tmd_adr;                    /* 0x4 */
} MODEL_LINK_INFO;

/* Scenario state (RE2 struct tagSCENARIO; RE3 tail layout differs) */
typedef struct tagSCENARIO {
    SCE_TASK Sce_task[SCE_TASK_MAX];   /* 0x0000 */
    UCHAR pad1458[0x145C - 0x1458];    /* RE2 Random_base? */
    UCHAR *pCScd;                      /* 0x145C */
    struct tagENEMY_WORK *pUnk1460;    /* 0x1460 = G.pEnemy[2] on room init (Sce_work_clr_set) */
    UCHAR *pMizu_div;                  /* 0x1464 RE2 pMizu_div */
    RCUT Cut;                          /* 0x1468 copy of the current camera cut (Bg_draw) */
    UCHAR Unk1488;                     /* 0x1488 */
    UCHAR Unk1489;                     /* 0x1489 */
    UCHAR pad148A[0x148C - 0x148A];
    UCHAR Type;                        /* 0x148C 0: room script tasks (2..9), else door tasks (10..13) */
    UCHAR pad148D;
    UCHAR Unk148E;                     /* 0x148E RE2 C_id? */
    UCHAR Unk148F;                     /* 0x148F */
    ULONG *pUnk1490;                   /* 0x1490 first EMD section of the enemy whose script runs (cd.c Cd_data_trans) */
    UCHAR Unk1494;                     /* 0x1494 */
    UCHAR Mizu_div_max;                /* 0x1495 RE2 Mizu_div_max */
    UCHAR Unk1496;                     /* 0x1496 RE2 Mizu_div_ctr? */
    UCHAR Unk1497;                     /* 0x1497 */
    UCHAR Unk1498;                     /* 0x1498 */
    UCHAR Unk1499;                     /* 0x1499 */
    UCHAR pad149A;
    UCHAR Unk149B;                     /* 0x149B */
    UCHAR Unk149C;                     /* 0x149C */
    UCHAR Unk149D;                     /* 0x149D */
    UCHAR Unk149E;                     /* 0x149E index into D_80098728 (Sce_rnd_set) */
    UCHAR pad149F;
    SHORT Pl_pos[3];                   /* 0x14A0 player position before the tasks ran (Sce_scheduler_main) */
    SHORT Sub_pos[3];                  /* 0x14A6 sub player position before the tasks ran */
} SCENARIO;
extern SCENARIO Sce;                   /* 0x800DECE8 */

typedef ULONG (*SCE_JMP_FUNC)(SCE_TASK *);
extern SCE_JMP_FUNC Sce_jmp_tbl[];      /* 0x8009E0F8, SCE opcode handlers */

void Event_init(SCE_TASK *pSce, ULONG Evt_no); /* RE2 Event_init */
void Event_exec(ULONG Task_level, ULONG Evt_no); /* RE2 Event_exec */

extern UCHAR D_8009E340[];
extern UCHAR D_8009E6BC[];
extern SHORT D_8009E6D4[];
extern SHORT D_8009E6DC[];
extern SHORT D_8009E6E4[];
extern UCHAR D_800B9E21;
extern SHORT D_800D1F82; /* RE2 G.SaveData.Sce_random */
extern UCHAR D_800E017E[];

void func_80051F78(ULONG Mode, PARTS_W *pP, UCHAR *pArg);
ULONG Em_kind_search(ULONG Id);
UCHAR func_800520D8(ULONG Id);
void *func_8005211C(ULONG Id);
void Sce_aot_init(void);
void Sce_model_init(void);
void Sce_work_clr(void);
void Sce_work_clr_at(void);
void Sce_work_clr_set(void);
void Sce_rnd_set(void);
void Sce_kirakira_set(void);
void func_80052644(void);
void func_8005283C(void);
void func_800528D4(void);
void func_8005290C(void);
void func_80052968(void);
void Sce_scheduler_set(void);
void Sce_scheduler(void);
void Sce_scheduler_main(void);

#endif /* SCE0_H */
