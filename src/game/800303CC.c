#include "common.h"
#include "monitor.h"
#include "mojidisp.h"
#include "pad.h"
#include "em_sub.h"
#include "om.h"
#include "pl_wep2.h"
#include "movie.h"
#include "player.h"

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800318DC);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800324A0);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033508);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033580);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800336B4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800336F0);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800337B8);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003389C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033B20);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033B88);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033CF4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033F38);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80033FBC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034124);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034284);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034448);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800345C0);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800346AC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034814);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034D74);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034E78);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80034F08);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035184);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800353CC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003553C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035678);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035790);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035868);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003589C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035B84);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035C18);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80035D9C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", Tex_chg_init);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", Tex_chg);

void Om_chg_init_80036234(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
    pCcp->Cc_ctr = 1;
    pCcp->Cc_pos_x = 0;
    pCcp->Cc_pos_y = 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036248);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", Tex_ctr);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800364A4);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036570);

void Em_routine0_set1_80036640(ENEMY_WORK *pEm)
{
    pEm->Routine_0 = 1;
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003664C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800366EC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036C60);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036D94);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80036F78);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003749C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037680);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003783C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800378C8);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037A90);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037C38);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037D50);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037E80);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80037FA0);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80038634);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80038F98);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003910C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80039294);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003957C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_800397DC);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_80039924);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003A688);

void pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003A7D8);

void pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003B080);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003B1C4);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003D1A8);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003EA5C);

void pl_mv05_w1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w1_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003EB64);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003EF08);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003F208);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003F414);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003F520);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003F6B0);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003F79C);

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003FA38);

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

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003FEFC);

void pl_mv05_w2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w2_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/800303CC", func_8003FFD8);
