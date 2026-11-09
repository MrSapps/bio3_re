#include "common.h"
#include "monitor.h"
#include "mojidisp.h"
#include "pad.h"
#include "em_sub.h"
#include "om.h"
#include "pl_wep2.h"
#include "movie.h"
#include "player.h"
#include "cd.h"

/* RE3-only collision object list (80033508..80035790). Nodes are linked through pNext;
 * the player's node lives at Pl_work + 0x80. */
typedef struct tagCOL_OBJ {
    UCHAR Id;                          /* 0x00 */
    UCHAR Flg;                         /* 0x01 */
    UCHAR Type;                        /* 0x02 0 = player, < 3 = character */
    UCHAR Unk3;                        /* 0x03 */
    ENEMY_WORK *pEm;                   /* 0x04 */
    LONG Pos_x;                        /* 0x08 */
    LONG Pos_y;                        /* 0x0C */
    LONG Pos_z;                        /* 0x10 */
    SHORT Old_x;                       /* 0x14 */
    SHORT Old_y;                       /* 0x16 */
    SHORT Old_z;                       /* 0x18 */
    USHORT W0;                         /* 0x1A */
    USHORT D0;                         /* 0x1C */
    USHORT H;                          /* 0x1E */
    USHORT W1;                         /* 0x20 */
    USHORT D1;                         /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    SHORT Unk26;                       /* 0x26 */
    SHORT Unk28;                       /* 0x28 */
    SHORT Unk2A;                       /* 0x2A */
    SHORT Unk2C;                       /* 0x2C */
    USHORT Hit_flg;                    /* 0x2E */
    MATRIX *pM;                        /* 0x30 */
    struct tagCOL_OBJ *pNext;          /* 0x34 */
    UCHAR Em_idx;                      /* 0x38 */
    UCHAR Unk39;                       /* 0x39 */
    SHORT Unk3A;                       /* 0x3A */
    LONG Unk3C;                        /* 0x3C */
} COL_OBJ;

typedef struct tagCOL_ENT {            /* 0x18 bytes */
    SHORT X0;                          /* 0x00 */
    SHORT Z0;                          /* 0x02 */
    SHORT X1;                          /* 0x04 */
    SHORT Z1;                          /* 0x06 */
    USHORT Unk8;                       /* 0x08 */
    USHORT UnkA;                       /* 0x0A */
    LONG UnkC;                         /* 0x0C -1 terminates */
    UCHAR Type;                        /* 0x10 */
    UCHAR Em_idx;                      /* 0x11 */
    UCHAR pad12[2];
    COL_OBJ *pObj;                     /* 0x14 */
} COL_ENT;

typedef struct tagCOL_WORK {
    COL_OBJ *pTop;                     /* 0x00 */
    COL_OBJ *pTail;                    /* 0x04 */
    COL_ENT Ent[1];                    /* 0x08 */
} COL_WORK;
extern COL_WORK D_800DCED0;
extern COL_OBJ *D_800DCDD0[];

/* ENEMY_WORK fields not in em_sub.h yet */
#define EM_SHORT(p, ofs) (*(SHORT *)((UCHAR *)(p) + (ofs)))
#define EM_UCHAR(p, ofs) (*(UCHAR *)((UCHAR *)(p) + (ofs)))

extern ULONG D_8009CBC8[];
extern SHORT D_800DEB58[];
extern COL_ENT D_800DEB48;
LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);
void func_8004B7E8(ENEMY_WORK *pEm, COL_OBJ *pObj, LONG Arg2);

ULONG Flg_ck(ULONG *pFlg, ULONG Id);

/* array inside G at an offset not yet in tagGLOBAL (keeps the &G + index*size + ofs addressing) */
#define G_ARR(type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)&G)->a)
#define P_ARR(p, type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)(p))->a)

typedef ULONG OT8[8];

extern ULONG D_800CC87C;
extern ULONG D_800D1F2C;
extern UCHAR D_8009CD3C;
ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Arg1);
LONG Hit_ck_point4_l(ULONG Arg0, ULONG Arg1) __asm__("func_8001020C");
void func_800288C8(ENEMY_WORK *pEm, PARTS_W *pParts, LONG Arg2, MATRIX *pM);

/* G.pUnk265C, G.pUnk2660 and G.pEnemy[40] seen as one array */
typedef struct {
    UCHAR pad0[0x265C];
    ENEMY_WORK *pWork[42];             /* 0x265C */
} G_WORKS;

UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);
LONG func_800336B4(ENEMY_WORK *pEm);
void func_80034124(COL_OBJ *pObj);
void func_800336F0(COL_OBJ *pTop, COL_ENT *pEnt);
void func_800337B8(COL_OBJ *pTop, COL_ENT *pEnt);
void func_8003389C(COL_OBJ *pTop, COL_ENT *pEnt);
void func_80033FBC(COL_OBJ *pTop);
void func_80033B88(COL_OBJ *pTop);
COL_ENT *func_80033CF4(COL_OBJ *pObj, COL_ENT *pEnt);
void func_80034284(COL_OBJ *pObj, COL_ENT *pEnt);

ULONG func_800303CC(ULONG Val)
{
    if (Val == 0) {
        return 0;
    }
    return 0x10000 / Val + 1;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800303EC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80030764);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80030CB8);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003114C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80031504);

extern UCHAR D_800A0514[][4];
extern ULONG D_800D2048[];
extern UCHAR D_8009BEE4[];
extern USHORT D_8009C7C0[];

UCHAR *func_800318DC(UCHAR No)
{
    ULONG Flg;
    UCHAR *pTop;
    USHORT *pOfs;

    Flg = D_800A0514[No][2];
    pTop = D_8009BEE4;
    pOfs = D_8009C7C0;
    if (Flg != 0 && Flg_ck(D_800D2048, Flg)) {
        No = Flg - 0x55;
    }
    return pTop + pOfs[No];
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80031970);

LONG Moji_char_width(ULONG Mode, ULONG Code, LONG *pPos)
{
    if (Mode != 0 && Code < 0x57) {
        Code *= 2;
        *pPos -= msg_space[Code];
        return msg_space[Code + 1] + msg_space[Code];
    }
    return 14;
}

void Moji_init(void)
{
    LONG i;
    LONG j;
    LONG k;
    DR_TPAGE *pDr_mode;
    SPRT *pSpr;
    SPRT_8 *pSpr8;
    MOJIDISP *pMoji;

    pMoji = &Moji;
    pDr_mode = pMoji->Moji_mode[0];
    i = 0x10;
    do {
        SetDrawTPage(pDr_mode, 0, 0, GetTPage(0, 0, 0x300, 0x100));
        SetDrawTPage(pDr_mode + 1, 0, 0, GetTPage(0, 0, 0x340, 0x100));
        pDr_mode += 2;
        i--;
    } while (i != 0);
    pSpr = pMoji->Prim_sprt[0];
    pSpr8 = pMoji->Prim_sprt_8[0];
    j = 0x500;
    k = 0x100;
    do {
        pSpr->tag = 0x4000000;
        j--;
        pSpr++;
    } while (j != 0);
    do {
        pSpr8->tag = 0x3000000;
        k--;
        pSpr8++;
    } while (k != 0);
    Moji_mode_init();
}

void Moji_mode_init(void)
{
    ULONG i;
    DR_TPAGE *pDr_mode;
    ULONG *pOt;
    ULONG Cbuf;

    i = 8;
    Cbuf = G.Cbuf_id;
    pDr_mode = Moji.Moji_mode[Cbuf];
    pOt = G.Ot0[Cbuf];
    do {
        AddPrim(pOt, pDr_mode);
        AddPrim(pOt, pDr_mode + 1);
        pDr_mode += 2;
        i--;
        pOt++;
    } while (i != 0);
    Moji.pPrim14_cur = Moji.Prim_sprt[Cbuf];
    Moji.pPrim8_cur = Moji.Prim_sprt_8[Cbuf];
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80031BDC);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", Task_lexecute);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", Task_lchain);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800321C4);

extern void (*D_8009CBB4[])(void);

void func_800324A0(void)
{
    VM_WORK *pWork;

    pWork = &Vm_work;
    do {
        D_8009CBB4[pWork->Rno]();
        if (G.System_flg & 0x8000) {
            Main.Db[G.Cbuf_id ^ 1].Disp.isrgb24 = *((UCHAR *)pWork + 0x2E);
            Main.Db[G.Cbuf_id ^ 1].Disp.disp.w = pWork->Movie_w;
        }
        if (*(USHORT *)((UCHAR *)pWork + 0x1C) != 0) {
            G.Cbuf_id ^= 1;
        }
    } while (pWork->Rno < 2);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800325A4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800327A4);

INCLUDE_RODATA("asm/nonmatchings/game/800303CC", D_800106EC);

INCLUDE_RODATA("asm/nonmatchings/game/800303CC", D_800106F4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800329E4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80032AD4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80032D18);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80032F58);

void Close_movie(void)
{
    Movie_xa_stop();
    func_8008FB34(NULL);
    DsDataCallback(NULL);
    DsReadyCallback(NULL);
    DsCommand(DslPause, NULL, NULL, -1);
    StUnSetRing();
    func_80074658(0);
}

void RuntoImage(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    pDisp->Cur_pos_x = pDisp->Pos_x;
    if (Main.Db[0].Draw.ofs[1] == 0) {
        pDisp->Rect.y = (G.Cbuf_id != 0) ? 0xF0 : 0;
    } else {
        pDisp->Rect.y = (G.Cbuf_id == 0) ? 0xF0 : 0;
    }
    pDisp->Count = 0;
    pDisp->Rect.y += pDisp->Pos_y;
    func_8008F9FC(pWork->Rl_buff[pWork->Cbuf_id], 1);
    func_8008FA78(pWork->pImage_buff, pDisp->Size);
    func_8008FA98(0);
}

void Mdec_callback(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    if (StCdIntrFlag) {
        StCdInterrupt();
        StCdIntrFlag = 0;
    }
    pDisp->Rect.x = pDisp->Cur_pos_x;
    LoadImage(&pDisp->Rect, pWork->pImage_buff);
    if (++pDisp->Count < pDisp->Loop) {
        pDisp->Cur_pos_x += pDisp->Width;
        func_8008FA78(pWork->pImage_buff, pDisp->Size);
    } else {
        pWork->Movie_sync = 1;
    }
}

void Movie_xa_start(void)
{
    LONG Ctr;

    for (Ctr = 0; Ctr <= (LONG)Vm_work.Movie_volume; Ctr++) {
        Set_movie_volume2(Ctr);
    }
}

void Movie_xa_stop(void)
{
    LONG Ctr;

    for (Ctr = Vm_work.Movie_volume; Ctr >= 2; Ctr -= 2) {
        Set_movie_volume2(Ctr);
    }
    Set_movie_volume2(0);
}

void Set_movie_volume2(ULONG Volume)
{
    if (Stereo_flg == 0) {
        pAtv->val0 = pAtv->val2 = Volume;
        pAtv->val1 = pAtv->val3 = 0;
    } else {
        pAtv->val0 = pAtv->val1 = pAtv->val2 = pAtv->val3 = Volume >> 1;
    }
    DsMix(pAtv);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033394);

void func_80033508(void)
{
    ENEMY_WORK *pEm;
    COL_OBJ *pObj;

    D_800DCED0.pTail = NULL;
    D_800DCED0.pTop = NULL;
    D_800DCED0.Ent[0].UnkC = -1;
    pEm = &Pl_work;
    pObj = (COL_OBJ *)((UCHAR *)pEm + 0x80);
    pObj->pEm = pEm;
    pObj->W0 = pObj->D0 = 450;
    pObj->W1 = pObj->D1 = 450;
    pObj->H = 0xAF0;
    pObj->Unk24 = 0;
    pObj->Unk26 = 0;
    pObj->Unk28 = 0;
    pObj->Flg = 0;
    pObj->Type = 0;
    func_80033580(pObj, 0);
}

UCHAR func_80033580(COL_OBJ *pObj, ULONG Id)
{
    COL_OBJ *p;
    ULONG No;
    ULONG Old;

    No = 1;
    for (p = D_800DCED0.pTop; p != NULL; p = p->pNext) {
        if (p == pObj) {
            return;
        }
    }
    pObj->pNext = NULL;
    pObj->Unk3C = 0;
    pObj->Unk2A = 0;
    pObj->Unk2C = 0;
    if (Id != 0) {
        pObj->Id = Id;
    } else {
        do {
            Old = No;
            for (p = D_800DCED0.pTop; p != NULL; p = p->pNext) {
                if (p->Id == No) {
                    No++;
                    break;
                }
            }
        } while (No != Old);
        pObj->Id = No;
    }
    pObj->Hit_flg = 0;
    if ((p = D_800DCED0.pTop) != NULL) {
        D_800DCED0.pTop = pObj;
        pObj->pNext = p;
    } else {
        D_800DCED0.pTail = pObj;
        D_800DCED0.pTop = pObj;
    }
    func_80034124(pObj);
    pObj->pM = NULL;
    pObj->Unk39 = 8;
    pObj->Old_x = pObj->Pos_x;
    pObj->Old_y = pObj->Pos_y;
    pObj->Old_z = pObj->Pos_z;
    pObj->W1 = pObj->W0;
    pObj->D1 = pObj->D0;
    if (pObj->Type < 3) {
        pObj->Em_idx = func_800336B4(pObj->pEm);
    }
    return pObj->Id;
}

LONG func_800336B4(ENEMY_WORK *pEm)
{
    LONG i;

    for (i = 0; i < 42; i++) {
        if (((G_WORKS *)&G)->pWork[i] == pEm) {
            return i;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800336F0);

void func_800337B8(COL_OBJ *pTop, COL_ENT *pEnt)
{
    COL_OBJ **pp;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    USHORT Flg;
    UCHAR R;
    UCHAR Type;
    UCHAR Idx;
    LONG i;

    for (i = 0; (p = D_800DCDD0[i]) != NULL; i++) {
        Flg = p->Hit_flg;
        p->Hit_flg = Flg & 0x6C0;
        pEm = p->pEm;
        if (p->Type < 2) {
            R = pEm->Routine_0;
            if (R == 2 || R == 5) {
                p->Hit_flg = Flg & 0xC0;
            }
        }
        Type = p->Type;
        Idx = p->Em_idx;
        pEnt->pObj = p;
        pEnt->Type = Type;
        pEnt->Em_idx = Idx;
        if (p->Flg & 7) {
            pEnt = func_80033CF4(p, pEnt);
        } else {
            pEnt->Unk8 = 0xFE40;
            pEnt->UnkA = 0xF00;
            pEnt->UnkC = 0;
            func_80034284(p, pEnt);
        }
        pEnt++;
    }
    pEnt->UnkC = -1;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003389C);

void func_80033B20(void)
{
    COL_OBJ *pTop;

    pTop = D_800DCED0.pTop;
    func_800336F0(pTop, D_800DCED0.Ent);
    func_80033FBC(pTop);
    func_800337B8(pTop, D_800DCED0.Ent);
    func_8003389C(pTop, D_800DCED0.Ent);
    func_80033B88(pTop);
}

void func_80033B88(COL_OBJ *pTop)
{
    LONG i;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    VECTOR V;
    VECTOR *pV;
    LONG y;
    ULONG W;
    ULONG Tbl;

    pV = &V;
    for (i = 0; (p = D_800DCDD0[i]) != NULL; i++) {
        pEm = p->pEm;
        if (pEm->Be_flg & 0x2000) {
            continue;
        }
        if (p->Type >= 3) {
            continue;
        }
        Tbl = D_8009CBC8[p->Type];
        W = p->W0;
        pV->vx = p->Pos_x;
        pV->vz = p->Pos_z;
        pV->vy = y = p->Pos_y;
        if (EM_SHORT(pEm, 0x1B8) != 0) {
            pV->vy = y - EM_SHORT(pEm, 0x1B8);
        }
        D_800DEB58[4] = 0;
        p->Unk39 &= ~8;
        if (p->Hit_flg & 0x80) {
            func_8004B7E8(pEm, p, func_8004D720(pV, W, Tbl, 9));
        }
        if (D_800DEB58[4] == 0) {
            p->Unk39 |= 8;
            p->Hit_flg &= ~0x40;
            if (p->Type < 3) {
                EM_UCHAR(pEm, 0x1C1) = p->Unk39;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033CF4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033F38);

void func_80033FBC(COL_OBJ *pTop)
{
    LONG i;
    COL_OBJ *p;
    ENEMY_WORK *pEm;
    SVECTOR Sv;
    VECTOR V;
    MATRIX M;

    for (i = 0; (p = D_800DCDD0[i]) != NULL; i++) {
        p->Unk3 = 0xFF;
        pEm = p->pEm;
        p->Unk2A = 0;
        p->Unk2C = 0;
        p->Unk3A = 0;
        if (p->pM != NULL) {
            M = GsIDMATRIX;
            MulMatrix2(p->pM, &M);
        } else {
            RotMatrix_gte((SVECTOR *)&pEm->Cdir_x, &M);
        }
        ApplyMatrix(&M, (SVECTOR *)&p->Unk24, &V);
        if (p->pM != NULL) {
            p->Pos_x = p->pM->t[0] + V.vx;
            p->Pos_y = p->pM->t[1] + V.vy;
            p->Pos_z = p->pM->t[2] + V.vz;
        } else {
            p->Pos_x = pEm->Pos_x + V.vx;
            p->Pos_y = pEm->Pos_y + V.vy;
            p->Pos_z = pEm->Pos_z + V.vz;
        }
    }
}

void func_80034124(COL_OBJ *pObj)
{
    ENEMY_WORK *pEm;
    SVECTOR Sv;
    VECTOR V;
    MATRIX M;
    SVECTOR *pSv;

    pSv = &Sv;
    if (pObj->Id == 0) {
        return;
    }
    pEm = pObj->pEm;
    pObj->Unk2A = 0;
    pObj->Unk2C = 0;
    pObj->Hit_flg = 0;
    pObj->Unk3 = 0xFF;
    pObj->Unk3A = 0;
    pSv->vx = pEm->Cdir_x;
    pSv->vy = pEm->Cdir_y;
    pSv->vz = pEm->Cdir_z;
    RotMatrix_gte(pSv, &M);
    pSv->vx = pObj->Unk24;
    pSv->vy = pObj->Unk26;
    pSv->vz = pObj->Unk28;
    ApplyMatrix(&M, pSv, &V);
    V.vx = pObj->Old_x = pObj->Pos_x = pEm->Pos_x + V.vx;
    V.vy = pObj->Old_y = pObj->Pos_y = pEm->Pos_y + V.vy;
    V.vz = pObj->Old_z = pObj->Pos_z = pEm->Pos_z + V.vz;
    if (pObj->Type < 3) {
        EM_SHORT(pEm, 0x122) = func_8004D720(&V, pObj->W0, D_8009CBC8[pObj->Type], 9);
    }
}

void func_80034284(COL_OBJ *pObj, COL_ENT *pEnt)
{
    SHORT *pCam;
    LONG x0;
    LONG x1;
    LONG z0;
    LONG z1;
    LONG px;
    LONG pz;

    pCam = *(SHORT **)((UCHAR *)G.pRoom + 0x20);
    x0 = pObj->Old_x - pObj->W0 - 900;
    x1 = pObj->Old_x + pObj->W0 + 900;
    z0 = pObj->Old_z - pObj->W0 - 900;
    z1 = pObj->Old_z + pObj->W0 + 900;
    px = pCam[2];
    pz = pCam[3];
    if (px <= x0 || px <= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 1;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 4;
        }
    }
    if (px >= x0 || px >= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 2;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 8;
        }
    }
    px = pCam[4];
    pz = pCam[5];
    if (px <= x0 || px <= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 0x10;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 0x40;
        }
    }
    if (px >= x0 || px >= x1) {
        if (pz <= z0 || pz <= z1) {
            pEnt->UnkA |= 0x20;
        }
        if (pz >= z0 || pz >= z1) {
            pEnt->UnkA |= 0x80;
        }
    }
}

#define EM_COL(p) ((COL_OBJ *)((UCHAR *)(p) + 0x80))
LONG func_80034448(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SVECTOR Sv;
    LONG w;
    LONG d;
    LONG dx;
    LONG dz;
    LONG ax;
    LONG az;

    Sv.vx = EM_COL(pEm)->W0 * 120 / 100 + 0x110;
    Sv.vz = 0;
    Rot_vector(pEm->Cdir_y, &Sv, &Sv);
    Sv.vx += pEm->Pos_x;
    Sv.vz += pEm->Pos_z;
    w = EM_COL(pT)->W0 - 0x110;
    d = EM_COL(pT)->D0 - 0x110;
    dx = pT->Pos_x - pEm->Pos_x;
    dz = pT->Pos_z - pEm->Pos_z;
    if ((ULONG)(dx + w) > (ULONG)(w * 2) && (ULONG)(dz + d) > (ULONG)(d * 2)) {
        return 0;
    }
    ax = __builtin_abs(dx);
    az = __builtin_abs(dz);
    if (az < ax) {
        dx = Sv.vx - EM_COL(pT)->Pos_x;
        dz = pEm->Pos_z - EM_COL(pT)->Pos_z;
        d += EM_COL(pEm)->W0;
    } else {
        dx = pEm->Pos_x - EM_COL(pT)->Pos_x;
        dz = Sv.vz - EM_COL(pT)->Pos_z;
        w += EM_COL(pEm)->W0;
    }
    if ((ULONG)(dx + w) > (ULONG)(w * 2)) {
        return 0;
    }
    return !((ULONG)(dz + d) > (ULONG)(d * 2));
}

LONG func_800346AC(COL_OBJ *pObj, ENEMY_WORK *pEm, ENEMY_WORK *pT);

LONG func_800345C0(COL_OBJ *pObj, ENEMY_WORK *pEm)
{
    ENEMY_WORK **pp;
    ENEMY_WORK *pT;
    ULONG Be;

    if (pEm->Be_flg & 0x4000) {
        return 0;
    }
    for (pp = &G.pUnk265C; pp != G.ppEnemy_end; pp++) {
        pT = *pp;
        if (pEm == pT) {
            continue;
        }
        Be = pT->Be_flg;
        if ((Be & 0x11) != 1) {
            continue;
        }
        if ((Be & 0x6000) == 0x6000) {
            continue;
        }
        if (EM_UCHAR(pT, 0x80) == EM_UCHAR(pEm, 0x80)) {
            continue;
        }
        if (func_800346AC(pObj, pEm, pT)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800346AC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034814);

LONG func_80034448(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG func_8004B374(COL_ENT *pEnt, ULONG Type, ULONG Arg2);

void func_80034D74(COL_OBJ *pObj, COL_ENT *pEnt, ULONG Arg2, ULONG Type)
{
    ENEMY_WORK *pPl;

    pPl = &Pl_work;
    if (func_80034448(pPl, pEnt->pObj->pEm)) {
        if (Type == 7 && !(pObj->Hit_flg & 0x600)) {
            if (func_8004B374(pEnt, 7, 0) == 0) {
                pEnt->pObj->Hit_flg |= 0x10;
                pEnt->pObj->Unk3A |= 0x8000;
            } else {
                EM_SHORT(pPl, 0x1C8) = 1;
            }
        }
    } else if (Type == 8 && !(pObj->Hit_flg & 0x600)) {
        if (func_8004B374(pEnt, 8, 0)) {
            EM_SHORT(pPl, 0x1C8) = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034E78);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034F08);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035184);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800353CC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003553C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035678);

void func_8002AB34(ULONG *pTmd, ULONG Tpage, ULONG Clut);
PARTS_W *func_8002E61C(ENEMY_WORK *pEm, PARTS_W *pParts);
PARTS_W *func_80035868(ENEMY_WORK *pEm, PARTS_W *pParts);
void func_8003589C(ENEMY_WORK *pEm, ULONG *pData);

void func_80035790(ENEMY_WORK *pEm, ULONG *pData)
{
    if (pEm->Id != 1) {
        *(ULONG **)((UCHAR *)pEm + 0x184) = pData + 1;
        pData = (ULONG *)((UCHAR *)pData + *pData);
    }
    if (G_ARR(ULONG *, 0x2150)[0] != pData) {
        G_ARR(ULONG *, 0x2150)[0] = pData;
        func_8002AB34(pData, EM_UCHAR(pEm, 0x48), EM_UCHAR(pEm, 0x49));
    }
    *(ULONG *)&pEm->Routine_0 = 0;
    pEm->pTmd = pData + 2;
    pEm->Poly_rgb = 0x808080;
    if (pEm->Id == 1) {
        G_ARR(PARTS_W *, 0x213C)[0] = func_8002E61C(pEm, G_ARR(PARTS_W *, 0x213C)[0]);
    } else {
        G_ARR(PARTS_W *, 0x213C)[0] = func_80035868(pEm, G_ARR(PARTS_W *, 0x213C)[0]);
        func_8003589C(pEm, *(ULONG **)((UCHAR *)pEm + 0x184));
    }
}

PARTS_W *func_80035868(ENEMY_WORK *pEm, PARTS_W *pParts)
{
    pEm->Parts_num = ((UCHAR *)pEm->pTmd)[-4];
    *(PARTS_W **)&pEm->Spd_z = pParts;
    return pParts + pEm->Parts_num;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003589C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035B84);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035C18);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035D9C);

void Tex_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
    ULONG i;
    OBJ_MODEL_WORK *pMw;
    CC_PARTS_WORK *pPart;
    RECT *pRect;

    pMw = pCcp->pOm;
    if (Cc_work.Ctex_old == pMw->Tpage) {
        return;
    }
    pPart = &Cc_work.Cc_parts[Col_wk_no];
    Cc_work.Ctex_old = pMw->Tpage;
    pPart->Cc_num++;
    pPart->Cc_cnt = pPart->Cc_wait;
    pPart->Cc_pos_x = (USHORT)((pMw->Tpage & 0xf) << 6);
    pPart->Cc_pos_y = (USHORT)((pMw->Tpage >> 4) << 8);
    for (i = 0; i < 2; i++) {
        pRect = pPart->Cc_dr_rect + i;
        pRect->w = 0x20;
        pRect->h = 0x40;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", Tex_chg);

void Om_chg_init_80036234(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
    pCcp->Cc_ctr = 1;
    pCcp->Cc_pos_x = 0;
    pCcp->Cc_pos_y = 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036248);

void Tex_ctr(void)
{
    CC_PARTS_WORK *pCcp;
    DR_MOVE *pMove;
    RECT *pRect;
    ULONG i;
    ULONG D0;
    ULONG D1;
    ULONG *pOt;

    D1 = Cc_work.Ccol_no;
    if (D1 == 0) {
        return;
    }
    D0 = G.Cbuf_id;
    pCcp = Cc_work.Cc_parts;
    pOt = G_ARR(OT8, 0x2048)[D0];
    i = 0;
    do {
        if (pCcp->Cc_flg & 0x40) {
            pMove = &pCcp->Cc_dr_move[D0];
            pRect = &pCcp->Cc_dr_rect[D0];
            Tex_chg(pCcp, pRect, pCcp->Cc_ctr);
            SetDrawMove(pMove, pRect, pCcp->Cc_pos_x, pCcp->Cc_pos_y);
            AddPrim(pOt, pMove);
        }
        if (pCcp->Cc_flg & 0x20) {
            func_80036248(pCcp);
        }
        pCcp++;
        i++;
    } while (i < D1);
}

typedef struct tagOM_WORK {
    ULONG Be_flg;                      /* 0x000 */
    UCHAR Routine_0;                   /* 0x004 */
    UCHAR pad5[0x194 - 0x5];
} OM_WORK;
extern void (*D_8009CC64[])(OM_WORK *pOm);
extern LONG D_800DD4AC;
extern ENEMY_WORK *D_800DD4B0;
void func_80036570(void);

void func_800364A4(void)
{
    OM_WORK *pOm;

    if (EM_UCHAR(G.pRoom, 2) == 0) {
        return;
    }
    if (G.Unk2468 & 0x8000000) {
        return;
    }
    D_800DD4AC = 0;
    D_800DD4B0 = NULL;
    G.Status_flg &= ~0x10;
    Tex_ctr();
    for (pOm = G_ARR(OM_WORK, 0x4328); pOm != G_ARR(OM_WORK *, 0x75A8)[0]; pOm++) {
        D_8009CC64[pOm->Routine_0](pOm);
    }
    func_80036570();
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036570);

void Em_routine0_set1_80036640(ENEMY_WORK *pEm)
{
    pEm->Routine_0 = 1;
}

extern SHORT D_800DE614;
extern SHORT D_800DE618;
extern SHORT D_800DE61A;
extern SHORT D_800DE61E;

void func_8003664C(ENEMY_WORK *pEm)
{
    SHORT dx;
    SHORT dz;

    dx = pEm->Pos_x - G.Pl_pos_x;
    dz = pEm->Pos_z - G.Pl_pos_z;
    if (__builtin_abs(dx) < 900) {
        D_800DE61A = 0;
        D_800DE61E = (dz >> 31) | 1;
    } else {
        D_800DE61E = 0;
        D_800DE61A = (dz >> 31) | 1;
    }
    D_800DE614 = pEm->Cdir_x;
    D_800DE618 = pEm->Cdir_z;
    pEm->Routine_0 = 2;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800366EC);

extern ULONG D_800D20D4[];
void Flg_on(ULONG *pFlg, ULONG Id);
void func_800366EC(ENEMY_WORK *pEm, ULONG Arg1, ULONG Arg2);
void Em_routine0_set1_80036640(ENEMY_WORK *pEm);

void func_80036C60(ENEMY_WORK *pEm)
{
    tagGLOBAL *pG;

    pG = &G;
    if (!(pEm->Be_flg & 1)) {
        return;
    }
    if (P_ARR(pG, UCHAR, 0x2491)[0] != 1 && P_ARR(pG, UCHAR, 0x2491)[0] != 9) {
        EM_UCHAR(pEm, 0xC0) = 0;
    }
    if ((pEm->UnkAE & 0x10) && (*(USHORT *)((UCHAR *)pEm + 0xBA) & 0x8000)) {
        D_800DD4AC++;
        if (pG->Pl_nFloor == pEm->nFloor) {
            D_800DD4B0 = pEm;
        }
    }
    if ((pEm->Id & 4) && *G_ARR(USHORT *, 0x75AC)[EM_UCHAR(pEm, 0xC1)] == 0xFF) {
        switch (pEm->Type & 0xC0) {
        case 0:
            func_800366EC(pEm, 1, 1);
            break;
        case 0x40:
            Em_routine0_set1_80036640(pEm);
            break;
        case 0x80:
            break;
        }
        Flg_on(D_800D20D4, EM_UCHAR(pEm, 0xC2));
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036D94);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036F78);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003749C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037680);

void func_8003783C(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_1) {
    case 0:
        *G_ARR(SHORT *, 0x75AC)[EM_UCHAR(pEm, 0xC1)] = 0xFF;
        pEm->Routine_1 = 1;
        break;
    case 1:
        break;
    case 2:
        *G_ARR(SHORT *, 0x75AC)[EM_UCHAR(pEm, 0xC1)] = 0;
        pEm->Routine_0 = 0;
        pEm->Routine_1 = 0;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800378C8);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037A90);

typedef ULONG ULONG4[4];
ULONG func_80037D50(VECTOR *pPos);

void func_80037C38(void)
{
    tagGLOBAL *pG;
    ENEMY_WORK **pp;
    ENEMY_WORK **ppEnd;
    ENEMY_WORK *pEm;
    OM_WORK *pOm;

    pG = &G;
    ppEnd = pG->ppEnemy_end;
    for (pp = &pG->pUnk265C; pp != ppEnd; pp++) {
        pEm = *pp;
        if (pEm->Be_flg & 1) {
            if (pEm->Be_flg & 0x80) {
                *(ULONG *)((UCHAR *)pEm + 0x7C) = P_ARR(pG, ULONG4, 0x2090)[pG->Cbuf_id][EM_UCHAR(pEm, 0xD)];
            } else {
                *(ULONG *)((UCHAR *)pEm + 0x7C) = func_80037D50((VECTOR *)&pEm->Pos_x);
            }
        }
    }
    for (pOm = P_ARR(pG, OM_WORK, 0x4328); pOm != P_ARR(pG, OM_WORK *, 0x75A8)[0]; pOm++) {
        if (pOm->Be_flg & 1) {
            if (pOm->Be_flg & 0x80) {
                *(ULONG *)((UCHAR *)pOm + 0x7C) = P_ARR(pG, ULONG4, 0x2090)[pG->Cbuf_id][EM_UCHAR(pOm, 0xD)];
            } else {
                *(ULONG *)((UCHAR *)pOm + 0x7C) = func_80037D50((VECTOR *)((UCHAR *)pOm + 0x34));
            }
        }
    }
}

LONG func_800101C8(VECTOR *pPos, UCHAR *pArea);

ULONG func_80037D50(VECTOR *pPos)
{
    tagGLOBAL *pG;
    USHORT *pHead;
    UCHAR *p;
    ULONG n;
    USHORT *pBlk;

    pG = &G;
    if (P_ARR(pG, LONG, 0x2470)[0] == 0) {
        goto end;
    }
    pHead = *(USHORT **)((UCHAR *)pG->pRoom + 0x40);
    if (pG->Cut_no < pHead[1]) {
        goto start;
    }
    goto end;
found:
    return P_ARR(pG, ULONG4, 0x2090)[pG->Cbuf_id][((USHORT *)p)[1]];
start:
    pBlk = (USHORT *)((UCHAR *)pHead + ((ULONG *)pHead)[pG->Cut_no + 1]);
    n = *pBlk;
    p = (UCHAR *)(pBlk + 2);
    for (; n != 0; n--) {
        if (p[1] == 0) {
            if (!(p[0] & 2) && func_800101C8(pPos, p + 4)) {
                goto found;
            }
            p += 12;
        } else {
            if (!(p[0] & 2) && Hit_ck_point4_l((ULONG)pPos, (ULONG)p)) {
                goto found;
            }
            p += 20;
        }
    }
end:
    return (ULONG)pG->pOt1;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037E80);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037FA0);

typedef struct {
    UCHAR pad0[4];
    SHORT Unk4;                        /* 0x04 */
    UCHAR Num;                         /* 0x06 */
    UCHAR pad7;
    UCHAR Tbl[16][2];                  /* 0x08 */
} PAD_WORK_800DE628;
extern PAD_WORK_800DE628 D_800DE628;

void func_80038634(ULONG Flg, SHORT Val)
{
    PAD_WORK_800DE628 *pW;
    UCHAR *p;
    LONG i;
    LONG n;

    pW = &D_800DE628;
    pW->Unk4 = Val;
    p = pW->Tbl[0];
    i = 0;
    n = 0;
    do {
        if (Flg & 1) {
            *p = i;
            p += 2;
            n++;
        }
        Flg >>= 1;
        i++;
    } while (Flg != 0);
    pW->Num = n;
}

void Pad_vib_set0(LONG Time, USHORT Attr)
{
    VIB_REQ *pReq = Vib_req0;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur0 = Pad_vib_get_free(pReq);
        Vib_cur0->Flg = 1;
        Vib_cur0->Time = Time;
        Vib_cur0->Pow = (Time != 0);
        Vib_cur0->Delay = Attr;
        Vib_cur0->Step = 0;
    }
}

void Pad_vib_set1(LONG Time, ULONG Power, ULONG Attr)
{
    VIB_REQ *pReq = Vib_req1;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur1 = Pad_vib_get_free(pReq);
        Vib_cur1->Flg = 1;
        Vib_cur1->Time = Time;
        Vib_cur1->Pow = Power;
        Vib_cur1->Delay = Attr;
        Vib_cur1->Step = 0;
    }
}

void Pad_vib_fade_set(LONG Time, LONG Pow_st, LONG Pow_ed, ULONG Attr)
{
    VIB_REQ *pReq = Vib_req1;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur1 = Pad_vib_get_free(pReq);
        Vib_cur1->Flg = 1;
        Vib_cur1->Time = Time;
        Vib_cur1->Delay = Attr;
        Vib_cur1->Pow_cur = Pow_st << 7;
        Vib_cur1->Step = (Pow_ed - Pow_st) * 0x80 / Time;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003884C);

void Pad_vib_init(void)
{
    ULONG i;

    for (i = 0; i < 33; i++) {
        Vib_req0[i].Flg = 0;
        Vib_req1[i].Flg = 0;
    }
    Vib_cur0 = Vib_req0;
    Vib_cur1 = Vib_req1;
    PadActData[1] = 0;
    PadActData[0] = 0;
}

UCHAR SetPadActData(VIB_REQ *pReq)
{
    ULONG i;
    UCHAR Max = 0;
    UCHAR *pB;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg == 0) {
            continue;
        }
        pB = &pReq->Pow;
        if (*(USHORT *)(pB + 1) != 0) {
            (*(USHORT *)(pB + 1))--;
            continue;
        }
        if (--(*(USHORT *)(pB + 3)) == 0) {
            pReq->Flg = 0;
        }
        if (*(SHORT *)(pB + 5) != 0) {
            *(USHORT *)(pB + 7) += *(USHORT *)(pB + 5);
            *pB = *(USHORT *)(pB + 7) >> 7;
        }
        if (Max < *pB) {
            Max = *pB;
        }
    }
    return Max;
}

VIB_REQ *Pad_vib_get_free(VIB_REQ *pReq)
{
    ULONG i;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg == 0) {
            break;
        }
    }
    return pReq;
}

LONG Pad_vib_count(VIB_REQ *pReq)
{
    ULONG i;
    LONG Cnt = 0;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg != 0) {
            Cnt++;
        }
    }
    return Cnt;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80038AAC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80038C7C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80038E50);

void Kage_work9_set(KAGE_WORK9 **ppKage, ULONG Arg1, ULONG Arg2, ULONG Arg3, VECTOR *pPos);

void func_80038F98(ENEMY_WORK *pEm)
{
    if (!(*((UCHAR *)&G + 0x2912) & 1)) {
        Kage_work9_set(&pEm->pKage_work, 0, 0x022601C2, 0, (VECTOR *)&pEm->Pos_x);
    }
    EM_SHORT(pEm, 0xA) = 0;
    pEm->Unk132 = 0;
    EM_SHORT(pEm, 0x144) = 0;
    EM_SHORT(pEm, 0x150) = 0;
    EM_SHORT(pEm, 0x152) = 0;
    EM_SHORT(pEm, 0x1B8) = 0;
    EM_SHORT(pEm, 0x1BA) = 0;
    EM_SHORT(pEm, 0x1BC) = 0;
    EM_SHORT(pEm, 0x1BE) = 0;
    EM_UCHAR(pEm, 0x1C0) = 0;
    EM_UCHAR(pEm, 0x1C1) = 0;
    EM_SHORT(pEm, 0x1C2) = 0;
    EM_SHORT(pEm, 0x1CA) = 0;
    EM_SHORT(pEm, 0x1C8) = 0;
}

void Pl_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Pl_mv_pad[pEm->Routine_1](pEm, G.Key, G.Key_trg);
    Pl_mv[pEm->Routine_1](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
    if ((pEm->Status_flg & 0x200) && !(G.Unk2468 & 0x1000000)) {
        if (--G.Pl_poison_timer == -1) {
            G.Pl_poison_timer = 30;
            if (pEm->Life >= 2) {
                pEm->Life--;
            }
        }
    }
}

void func_8003910C(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (Key & 0xA) {
        *(ULONG *)&pEm->Routine_0 = 0x401;
    }
    if (Key & 4) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
        if (Key_trg & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x601;
        }
    }
    if (Key & 1) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        if (Key & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80039294);

extern UCHAR D_8009CDEC[];

void func_8003957C(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 1)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
            if (Key_trg & 0x200) {
                *(ULONG *)&pEm->Routine_0 = 0x601;
            }
        }
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xA01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20A01;
        }
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDEC[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDEC[D_8009CD3C];
    }
    if (Key & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if (D_800D1F2C & 0x10) {
        *(ULONG *)&pEm->Routine_0 = 0x901;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800397DC);

extern UCHAR D_8009CDF0[];

void func_80039924(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if ((Key & 0x201) != 0x201) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        }
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xF01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20F01;
        }
    }
    if (pEm->Spd_x >= 0x33) {
        if (Key & 2) {
            pEm->Cdir_y += D_8009CDF0[D_8009CD3C];
        }
        if (Key & 8) {
            pEm->Cdir_y -= D_8009CDF0[D_8009CD3C];
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x701;
        }
    }
    if (D_800D1F2C & 0x10) {
        *(ULONG *)&pEm->Routine_0 = 0x901;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80039B84);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80039CCC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80039F08);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003A1E0);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003A398);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003A574);

void pl_mv06_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (G.Unk2468 & 0x1000000) {
        *(ULONG *)&pEm->Routine_0 = 1; /* Routine_0 = 1, Routine_1..3 = 0 */
    }
}

extern UCHAR D_8009CDE0[];
void func_800776B0(ULONG Arg0, ULONG No, ENEMY_WORK *pEm);

void func_8003A688(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 6];
        pEm->Routine_2 = 1;
        EM_UCHAR(pEm, 0xE2) = 8;
        pEm->Spd_x = 0x32;
        pEm->Spd_z = 0;
        n += 0xF0000;
        *(ULONG *)&pEm->Move_no = n;
        Get_speedXZ(pEm, 0x700, &pEm->Spd_x, &pEm->Spd_z);
        G.Status_flg &= ~0x100;
        G_ARR(USHORT, 0x75E8)[0] |= 4;
        func_800776B0(0, 11, pEm);
    case 1:
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x100);
        pEm->Cdir_y += 0x100;
        if (--*((signed char *)pEm + 0xE2) == 0) {
            *(ULONG *)&pEm->Routine_0 = 1;
            G_ARR(USHORT, 0x75E8)[0] |= 4;
            func_800776B0(0, 11, pEm);
        }
        pEm->Pos_x += pEm->Spd_x;
        pEm->Pos_z += pEm->Spd_z;
        break;
    }
}

void pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003A7D8);

void pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003B080);

void func_8003B1C4(ENEMY_WORK *pEm, ULONG Flg)
{
    if ((Flg & 0x10) && (D_800D1F2C & 0x10)) {
        return;
    }
    if (pEm->Routine_2 == 5) {
        pEm->Routine_2 = 6;
        pEm->Move_no = 6;
        pEm->Move_cnt = 0;
        *((UCHAR *)pEm + 0xCA) = 7;
        func_800746C0(0x10200, (VECTOR *)&pEm->Pos_x, 0, 0);
        Pad_vib_init();
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003B244);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003B4FC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003B784);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003B9FC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003BCA4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003BF28);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003C104);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003C390);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003C5B0);

void pl_mv0b_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003C740);

void pl_mv0c_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003CA88);

void pl_mv0d_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003CEA0);

void Pl_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_dm[pEm->Routine_1](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
}

void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);

void func_8003D1A8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x30004;
        pEm->Routine_2 = 1;
        func_800746C0(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Damage_cnt = 0x80;
        Pad_vib_set0(5, 0);
        Pad_vib_set1(8, 0x96, 0);
        pEm->Be_flg &= ~0x8000;
        EM_UCHAR(pEm, 0x120) |= 0x12;
    case 1:
        Spd_get(pEm, Kan, Seq, 0);
        if (func_80027940(pEm, (ULONG *)Kan, (ULONG *)Seq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            G.Status_flg &= ~0x100;
            EM_UCHAR(pEm, 0x120) &= 0xED;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003D2D8);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003D4C0);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003D780);

void Pl_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_die_t[pEm->Routine_1](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003DA3C);

void func_8003DC0C(void) {
}

void Pl_Em_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEm->Be_flg &= ~0x8000;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldamage_Em[pEnemy->Id - 0x10](pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

void Pl_Em_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldie_Em[pEnemy->Id - 0x10](pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

void Pl_dead(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    LONG routine1;
    KAGE_WORK9 *pKage;

    routine1 = pEm->Routine_1;
    pKage = pEm->pKage_work;
    if (routine1 != 1) {
        if (1 < routine1) {
            return;
        }
        if (routine1 != 0) {
            return;
        }
        pEm->Routine_1 = 1;
        Kage_work9_color_set(pKage, 0xBFBF10);
        pEm->Timer0 = 0x3C;
    }
    pKage->Half_x += 8;
    pKage->Half_z += 8;
    if (--pEm->Timer0 == -1) {
        pEm->Routine_1 = 2;
    }
}

ULONG Pl_life_down(LONG Down_life, ULONG Mode)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Life;

    if ((ULONG)(Down_life - 10) < 0x5B) {
        Down_life += 10;
    }
    Life = (USHORT)pPl->Life - Down_life;
    pPl->Life = Life;
    pPl->Unk132 &= 0xE0FF;
    if (Mode == 1) {
        if ((SHORT)Life < 0) {
            pPl->Life = 0;
            pPl->Status_flg |= 0x800;
            return 1;
        }
        return 0;
    }
    if (Mode == 0) {
        if ((SHORT)Life < 0) {
            if ((SHORT)Life >= -10 && !(pPl->Status_flg & 0x800)) {
                pPl->Life = 0;
                pPl->Status_flg |= 0x800;
                return 1;
            }
            pPl->Damage_cnt |= 0x80;
            G.System_flg |= SYS_GAME_OVER;
            return 2;
        }
        return 0;
    }
    /* Mode >= 2: no return value (v0 still holds 1 from the Mode compare) */
}

void Pl_life_up(LONG Up_life)
{
    Pl_work.Life += Up_life;
    Pl_work.Status_flg &= ~0x800;
    if (Pl_work.Max_life < Pl_work.Life) {
        Pl_work.Life = Pl_work.Max_life;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003DEA4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003E088);

void Pl_weapon_vib(ENEMY_WORK *pEm)
{
    UCHAR (*pTbl)[6] = Pl_vib_tbl;
    UCHAR *pVib;

    pVib = pTbl[(UCHAR)(pEm->Type - 2)];
    Pad_vib_set0(pVib[0], 0);
    Pad_vib_set1(pVib[1], pVib[2], 0);
    Pad_vib_fade_set(pVib[5], pVib[3], pVib[4], pVib[1]);
}

void pl_mv05_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", pl_mv05);

void pl_mv05_w0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w0_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003E4D0);

void func_8003E088(ENEMY_WORK *pEm);
LONG func_80026BE8(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079690(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079868(ENEMY_WORK *pEm, LONG a1);

void func_8003EA5C(ENEMY_WORK *pEm)
{
    SHORT a;

    a = EM_SHORT(pEm, 0xA);
    if (a != 0 && a < EM_SHORT(pEm, 0x122) - 500 && !(pEm->Move_cnt & 1) && pEm->Move_cnt < 9) {
        func_8003E088(pEm);
    }
    if (func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x400)) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x400);
    func_80079868(pEm, 0x400);
}

void pl_mv05_w1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w1_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003EB64);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003EF08);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003F208);

void func_8003F414(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    LONG a;

    switch (pEm->Routine_3) {
    case 0:
        a = EM_SHORT(pEm, 0xA);
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70000;
        *(ULONG *)((UCHAR *)pEm + 0x1A0) = 0x70000;
        pEm->Spd_x = 0;
        if (a != 0 && a < EM_SHORT(pEm, 0x122) - 500) {
            func_8003E088(pEm);
        }
    case 1:
        if (func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x10200)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            EM_UCHAR(pEm, 0x120) &= 0xED;
            G.Status_flg &= ~0x40;
            G.Status_flg &= ~0x100;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x10200);
        func_80079868(pEm, 0x200);
        break;
    }
}

void func_8001B484(ULONG Arg0, ULONG Arg1, MATRIX *pM, ULONG Arg3, ULONG Arg4);
void func_80044F34(ENEMY_WORK *pEm);
void func_8006CF0C(ULONG Arg0);

void func_8003F520(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    ULONG n;
    SVECTOR Sv;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70007;
        *(ULONG *)((UCHAR *)pEm + 0x1A0) = 0x70007;
    }
    if (func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x200)) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x200);
    func_80079868(pEm, 0x200);
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)&pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
    }
    if ((*pEm->pNow_seq & 0x400) && (pEm->Status_flg & 0x400)) {
        if ((UCHAR)pEm->Type == 5) {
            func_8001B484(0x302, 0x08000000, &pEm->pSin_parts_ptr[4].Workm, 0, 0);
        } else {
            func_80044F34(pEm);
        }
        pEm->Status_flg &= ~0x400;
    }
    if (*pEm->pNow_seq & 0x200) {
        func_8006CF0C(1);
        pEm->Status_flg |= 0x400;
    }
}

void func_8003F79C(ENEMY_WORK *pEm);

void func_8003F6B0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        func_8003F79C(pEm);
        pEm->Be_flg |= 0x2000000;
        pT = pEm->pEnemy_ptr;
        if (pT == pEm) {
            *(USHORT *)&pEm->Routine_2 = 1;
            break;
        }
        EM_UCHAR(pEm, 0xE3) = 12;
    case 1:
        pEm->Be_flg |= 0x2000000;
        pT = pEm->pEnemy_ptr;
        if (pT != pEm) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xD8);
        }
        if (!Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x10)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        if (--*((signed char *)pEm + 0xE3) == -1) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003F79C);

extern UCHAR D_8009CD3D;

void func_8003FA38(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    LONG *pPos;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70002;
        *(ULONG *)((UCHAR *)pEm + 0x1A0) = 0x70002;
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        EM_UCHAR(pEm, 0x120) |= 0x12;
        D_8009CD3D = 0;
    case 1:
        pPos = *(LONG **)((UCHAR *)pEm + 0x15C);
        Goto00(pEm, pPos[0], pPos[2], 0x100);
        func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x200);
        if (*((UCHAR *)pEm + 0xCA) == 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x200);
        func_80079868(pEm, 0x200);
        break;
    case 2:
        *(ULONG *)&pEm->Move_no = 0x70002;
        pEm->Routine_3 = 3;
    case 3:
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003FB78);

void pl_mv05_w0_t9(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0xE;
        pEm->Routine_3 = 1;
    case 1:
        pl_mv05_t2_spr[(UCHAR)pEm->Type - 1](pEm, Kan, Seq, Key);
        if (func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 0x208;
            *(ULONG *)&pEm->Move_no = 0xF;
        }
        break;
    }
}

void pl_mv05_w0_t10(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x10;
        pEm->Routine_3 = 1;
        pEm->Spd_x = 0;
    case 1:
        if (func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        break;
    }
}

void func_8003FEFC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x11;
        pEm->Routine_3 = 1;
        pEm->Spd_x = 0;
    case 1:
        if (func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            EM_UCHAR(pEm, 0x120) &= 0xED;
            G.Status_flg &= ~0x100;
            G.Status_flg &= ~0x40;
        }
        break;
    }
}

void pl_mv05_w2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w2_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003FFD8);
