#include "cd.h"
#include "door.h"
#include "em_sub.h"
#include "main.h"
#include "monitor.h"
#include "sce0.h"
#include "sub.h"
#include "joint.h"
#include "snd.h"

/* room sound directory at 0x801C181C (CD_ENTRY list, Id bit 7 = more entries follow) */
#define CD_DIR ((CD_ENTRY *)0x801C181C)
#define CD_DIR_HEAD ((ULONG *)0x801C1814)

void Sa_dat_set(ENEMY_WORK *pEm, ULONG *pData);
void func_8007A2D4(ENEMY_WORK *pEm, ULONG *pData);
extern UCHAR D_800E017F;               /* room sound pack id */
extern UCHAR D_800E06AC;
void func_800139D8(void);
ULONG func_800261AC(ENEMY_WORK *pEm, ULONG work_top, ULONG *pKan);

void Cd_init(void)
{
    ULONG i;
    tagCD *pCd = &CD;

    for (i = CD_RING_BUFFERS; i != 0;) {
        i--;
        pCd->Ring_status[i] = 0;
        pCd->Ring_size[i] = 0;
    }

    while (DsInit() == 0);

    G.Status_flg &= ~STAT_XA_VOICE;
    DsSetDebug(0);
    DsReadyCallback(NULL);
    DsSyncCallback(NULL);

    while (DsCommand(DslSetmode, &Cd_mode, NULL, -1) == 0);

    pCd->Vol = 0x46;
    pCd->Cd_rtn_no = 0;
    pCd->Xa_rtn_no = 0;
    pCd->Use_mode = 0;
    pCd->Unk8C2 = 0xFF;
    pCd->Unk8C3 = 0xFF;
    Set_volume(pCd->Vol);
}

void Cd_system_control(void)
{
    if (CD.Use_mode == 1) {
        Xa_control();
    }
}

INCLUDE_RODATA("asm/nonmatchings/game/cd", D_8001031C);

extern USHORT D_800970DC[];
extern USHORT D_800970E8[];
extern SHORT D_800E0DD8;

void Xa_play(ULONG mode, ULONG no)
{
    USHORT Xa_no[12] = { 0x538, 0x539, 0x1E5, 0x539, 0x285, 0x539, 0x319, 0x539, 0x3A8, 0x539, 0x428, 0x539 };
    ULONG sub = mode & 1;
    tagCD *pCd = &CD;
    ULONG file_no;
    USHORT *pwd;
    ULONG mul;

    if (mode >= 2) {
        pwd = sub ? D_800970E8 : D_800970DC;
        file_no = Xa_no[sub];
    } else {
        pwd = G.pXa_sector;
        file_no = Xa_no[sub + (G.Unk783E + 1) * 2];
    }
    pwd += no * 3;
    pCd->Filter.file = 1;
    pCd->Filter.chan = pwd[0];
    sub = sub ? 8 : 16;
    pCd->Start_sector = (floc[file_no].sec_high << 16) + floc[file_no].sector;
    pCd->End_sector = pCd->Start_sector;
    pCd->Start_sector += pwd[1] * sub + pwd[0];
    pCd->End_sector += (pwd[1] + pwd[2]) * sub + pwd[0] - 15;
    pCd->Current_sector = pCd->Start_sector;
    DsIntToPos(pCd->Start_sector, &pCd->Fpos);
    func_80074658(1);
    pCd->Vol = pCd->Vol * D_800E0DD8 / 127;
    pCd->Xa_rtn_no = 1;
    pCd->Fade_flg = 0;
    pCd->Fade_step = 0;
    pCd->Limit_break = 0;
    pCd->Type = 0;
    pCd->Status = 0;
    pCd->Use_mode = 1;
    Change_volume(1);
    DsFlush();
    DsSyncCallback((DslCB)Xa_seek);

    while (DsCommand(DslPause, NULL, NULL, -1) == 0);
    while (DsCommand(DslSetfilter, &pCd->Filter.file, NULL, -1) == 0);
    while (DsCommand(DslSetmode, &Xa_cdmode, NULL, -1) == 0);
    while (DsCommand(DslSeekL, &pCd->Fpos.minute, NULL, -1) == 0);
    while (DsCommand(DslReadS, &pCd->Fpos.minute, NULL, -1) == 0);
    G.Status_flg |= 0x20;
}

void Xa_error(void)
{
    func_80074658(0);
    Change_volume(0);
    CD.Xa_rtn_no = 3;
}

void Xa_control(void)
{
    Xa_control_tbl[CD.Xa_rtn_no]();
}

void Xa_control_stop(void)
{
    CD.Use_mode = 0;
    CD.Xa_rtn_no = 0;
    CD.Limit_break = 0;
    G.Status_flg &= ~STAT_XA_VOICE;
}

void Xa_control_init(void)
{
    tagCD *pCd = &CD;

    pCd->Time_ctr = 0;
    do {
        VSync(0);
        if (++pCd->Time_ctr >= 0x3C) {
            CD.Xa_rtn_no = 3;
            return;
        }
    } while (DsQueueLen() != 0);
    CD.Xa_rtn_no = 2;
}

void Xa_seek(UCHAR intr, UCHAR *result)
{
    tagCD *pCd = &CD;
    ULONG i;

    i = 7;
    do {
        pCd->Result[i] = result[i];
    } while (i-- != 0);

    if (intr == DslComplete) {
        pCd->Status++;
        pCd->Limit_break = 0;
        if (DsQueueLen() == 0) {
            DsSyncCallback((DslCB)Xa_ready_cb);
        }
    } else {
        pCd->Status = 0;
    }
}

void Xa_ready_cb(UCHAR intr, UCHAR *result)
{
    tagCD *pCd = &CD;
    ULONG i;

    i = 7;
    do {
        pCd->Result[i] = result[i];
    } while (i-- != 0);

    if (intr == DslComplete) {
        pCd->Status = 1;
        pCd->Limit_break = 0;
        pCd->Result[8] = 0;
        pCd->Current_sector = DsPosToInt((DslLOC *)&pCd->Result[5]);
        if (pCd->End_sector <= pCd->Current_sector) {
            pCd->Xa_rtn_no = 3;
        }
    } else {
        pCd->Status = 0;
    }
}

void Xa_control_play(void)
{
    tagCD *pCd = &CD;

    if (DsSystemStatus() == DslReady) {
        Xa_control_cdmode ^= 1;
        if (Xa_control_cdmode != 0) {
            while (DsCommand(DslGetlocP, NULL, NULL, -1) == 0);
        }
        if (pCd->Status == 0) {
            pCd->Xa_rtn_no = 3;
        }
    }
    if (pCd->Fade_flg != 0) {
        if (pCd->Fade_step < pCd->Vol) {
            pCd->Vol -= pCd->Fade_step;
            Set_volume(pCd->Vol);
        } else {
            pCd->Xa_rtn_no = 3;
        }
    }
    if (pCd->Current_sector < pCd->Start_sector) {
        pCd->Xa_rtn_no = 3;
    }
    pCd->Limit_break++;
    if (pCd->Limit_break > 0x3C) {
        pCd->Xa_rtn_no = 3;
    }
}

void Xa_control_end(void)
{
    CD.Fade_flg = 0;
    CD.Vol = 0x46;
    DsSyncCallback(NULL);

    while (DsCommand(DslPause, NULL, NULL, -1) == 0);

    CD.Xa_rtn_no = 0;
    func_80074658(0);
    Change_volume(0);
}

extern UCHAR Cd_read_s_cdmode;          /* 0x800971E1 RE2 static cdread_s_cdmode */
extern UCHAR Cd_read_cdmode;            /* 0x800971E2 RE2 static cdread_cdmode */
extern LONG D_800A0FEC[];              /* SPU transfer address per bank type */
void func_80013C1C(UCHAR intr, UCHAR *result);

/* RE2 Cd_read_s: streams a file through the ring buffer to SPU memory (RE3: libspu, no libsnd VAB) */
ULONG Cd_read_s(ULONG File_no, ULONG Mode, ULONG *Address, ULONG Type, char *Mess)
{
    tagCD *pCd = &CD;
    ULONG Mode_org = Mode;
    ULONG i;

retry:
    Mode = Mode_org;
    if (!(Mode_org & 1)) {
        pCd->Fsum = floc[File_no].sum;
        pCd->Fsize = floc[File_no].size;
        pCd->Fsector = (ULONG)floc[File_no].sec_high * 0x10000 + floc[File_no].sector;
    }
    for (i = 0; i < CD_RING_BUFFERS; i++) {
        pCd->Ring_status[i] = 0;
        pCd->Ring_size[i] = 0;
    }
    pCd->Fno = File_no;
    pCd->Buff_adr = Address;
    pCd->Cd_rtn_no = 1;
    pCd->Type = 2;
    pCd->Fsector_read = 0;
    pCd->Fsize_read = 0;
    pCd->Limit_break = 0;
    pCd->Sum_cnt = 0;
    pCd->Sum = 0;
    pCd->Write_ctr = 0;
    pCd->Read_ctr = 0;
    pCd->First_flg = 0;
    pCd->Unk85F = 0;
    pCd->Fsector_num = (pCd->Fsize + 0x7FF) >> 11;
    pCd->Current_sector = pCd->Fsector;
    pCd->Fsize_now = pCd->Fsize;

    DsFlush();
    DsIntToPos(pCd->Current_sector, &CD.Fpos);

    while (DsCommand(DslPause, NULL, NULL, -1) == 0);
    while (DsCommand(DslSetmode, &Cd_read_s_cdmode, NULL, -1) == 0);
    while (DsCommand(DslSeekL, &CD.Fpos.minute, NULL, -1) == 0);

    DsReadyCallback((DslRCB)func_80013C1C);

    while (DsCommand(DslReadN, &CD.Fpos.minute, NULL, -1) == 0);

    while (1) {
        if (DsSystemStatus() == DslReady) {
            if ((DsStatus() & DslStatRead) == 0) {
                pCd->Cd_rtn_no = 6;
            } else if (pCd->Fsize_now == 0 && pCd->Fsector_num == 0) {
                if (CD.Sum == CD.Fsum) {
                    pCd->Cd_rtn_no = 0;
                } else {
                    pCd->Cd_rtn_no = 4;
                }
                pCd->Limit_break = 0;
                while (DsCommand(DslPause, NULL, NULL, -1) == 0);
            }
        }

        while (pCd->Ring_status[pCd->Write_ctr] != 0) {
            if (pCd->First_flg != 0) {
                while (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 0);
            } else {
                if (Type == 6) {
                    SpuSetTransferStartAddr(D_800A0FEC[6] - pCd->Fsize);
                } else {
                    SpuSetTransferStartAddr(D_800A0FEC[Type]);
                }
                pCd->First_flg = 1;
            }
            SpuWritePartly((UCHAR *)pCd->Buff_adr + (pCd->Write_ctr << 11), pCd->Ring_size[pCd->Write_ctr]);
            while (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 0);
            pCd->Ring_status[pCd->Write_ctr] = 0;
            if (++pCd->Write_ctr == CD_RING_BUFFERS) {
                pCd->Write_ctr = 0;
            }
        }

        if (Mode < 2) {
            Task_sleep(1);
        } else {
            VSync(0);
        }

        if (pCd->Cd_rtn_no > 1) {
            goto retry;
        }
        if (pCd->Cd_rtn_no == 0) {
            return pCd->Fsize_read;
        }
    }
}

void func_80012ECC(tagCD *pCd);
void func_800130A0(tagCD *pCd);
void func_80013EF8(UCHAR intr, UCHAR *result);
void func_80014300(UCHAR intr, UCHAR *result);
void func_800150E4(void);
void func_80014D10(void);

/* RE2 Cd_read. Mode 0/1: floc[File_no], 2/3: CD.Fsize etc. preset (0/2 wait with VSync, 1/3 with Task_sleep);
 * 4/5: room sound pack (Type 3/4) streamed to the SPU through the ring */
ULONG Cd_read(ULONG File_no, ULONG *Address, ULONG Mode, char *Mess)
{
    tagCD *pCd = &CD;
    ULONG Mode_org = Mode;
    ULONG i;
    LONG Ret;
    LONG *pFlg;

retry:
    Mode = Mode_org;
retry2:
    pCd->Type = 1;
    switch (Mode) {
    case 4:
    case 5:
        for (i = 0; i < CD_RING_BUFFERS; i++) {
            pCd->Ring_status[i] = 0;
            pCd->Ring_size[i] = 0;
        }
        if (Mode == 4) {
            pCd->Type = 3;
        } else {
            pCd->Type = 4;
        }
    case 0:
    case 1:
        pCd->Fsum = floc[File_no].sum;
        pCd->Fsize = floc[File_no].size;
        pCd->Fsector = (ULONG)floc[File_no].sec_high * 0x10000 + floc[File_no].sector;
        break;
    default:
        Mode -= 2;
        break;
    }
    pCd->pEntry = pCd->pEntry2 = CD_DIR;
    pCd->Buff_adr = (ULONG *)0x801C2014;
    pCd->Cd_rtn_no = 1;
    pCd->Fno = File_no;
    pCd->Read_adr = Address;
    pCd->Unk8A8 = 0;
    pCd->Fsector_read = 0;
    pCd->Fsize_read = 0;
    pCd->Limit_break = 0;
    pCd->Sum_cnt = 0;
    pCd->Sum = 0;
    pCd->Write_ctr = 0;
    pCd->Read_ctr = 0;
    pCd->First_flg = 0;
    pCd->Unk85F = 0;
    pCd->Unk8AC = 0;
    pCd->Unk8B0 = 0;
    pCd->Unk8B4 = 0;
    pCd->Entry_no = 0;
    pCd->Unk8C8 = 0;
    pCd->Unk8C0 = 0;
    pCd->Unk8B8 = 0;
    pCd->Unk8BC = 0;
    pCd->Time_out = 0;
    pCd->Fsector_num = (pCd->Fsize + 0x7FF) >> 11;
    pCd->Current_sector = pCd->Fsector;
    pCd->Fsize_now = pCd->Fsize;
    pCd->Unk8D0 = G.Ctp.u.Ctpage;
    pCd->Unk8D1 = G.Ctp.u.Cclut;
    pCd->Unk8CC = (ULONG)G.pEm_mem;
    if (pCd->Type == 4) {
        i = 0;
        pFlg = (LONG *)D_800DECC0;
        Sce.Unk148E = 0xFF;
        Sce.Unk148F = 0xFF;
        Sce.Unk1494 = 0xFF;
        Sce.Unk1488 = 0;
        Sce.pUnk1460 = G.pEnemy[2];
        for (; i < 5; i++) {
            *pFlg++ = -1;
        }
    }

    DsFlush();
    DsIntToPos(pCd->Current_sector, &CD.Fpos);

    while (DsCommand(DslPause, NULL, NULL, -1) == 0);
    while (DsCommand(DslSetmode, &Cd_read_cdmode, NULL, -1) == 0);
    while (DsCommand(DslSeekL, &CD.Fpos.minute, NULL, -1) == 0);

    switch (pCd->Type) {
    case 3:
        DsReadyCallback((DslRCB)func_80013EF8);
        break;
    case 4:
        func_800150E4();
        DsReadyCallback((DslRCB)func_80014300);
        break;
    default:
        DsReadyCallback((DslRCB)func_80013C1C);
        break;
    }

    while (DsCommand(DslReadN, &pCd->Fpos.minute, NULL, -1) == 0);

    while (1) {
        Ret = DsSystemStatus();
        if (Ret == DslReady) {
            if ((DsStatus() & DslStatRead) == 0) {
                pCd->Cd_rtn_no = 6;
            } else if (pCd->Fsize_now == 0 && pCd->Fsector_num == 0) {
                if (pCd->Type >= 3 || pCd->Sum == pCd->Fsum || pCd->Check_sum_flg == Ret) {
                    pCd->Cd_rtn_no = 0;
                } else {
                    pCd->Cd_rtn_no = 4;
                }
                pCd->Limit_break = 0;
                while (DsCommand(DslPause, NULL, NULL, -1) == 0);
            }
        }
        if (pCd->Type >= 3) {
            while (pCd->Ring_status[pCd->Write_ctr] != 0) {
                switch (pCd->pEntry[pCd->Ring_status[pCd->Write_ctr] & 0x7F].Unk5) {
                case 0:
                case 1:
                    func_800130A0(&CD);
                    break;
                case 2:
                    func_80012ECC(&CD);
                    break;
                }
                pCd->Ring_status[pCd->Write_ctr] = 0;
                if (++pCd->Write_ctr == CD_RING_BUFFERS) {
                    pCd->Write_ctr = 0;
                }
            }
            switch (Ret = pCd->Unk8C0) {
            case 1:
                if (pCd->First_flg == 2) {
                    break;
                }
                pCd->Current_sector = pCd->Fsector + pCd->Unk8B8;
                DsFlush();
                DsIntToPos(pCd->Current_sector, &pCd->Fpos);
                while (DsCommand(DslPause, NULL, NULL, -1) == 0);
                while (DsCommand(DslSetmode, &Cd_read_cdmode, NULL, -1) == 0);
                while (DsCommand(DslSeekL, &pCd->Fpos.minute, NULL, -1) == 0);
                pCd->Unk8C0 = 0;
                while (DsCommand(DslReadN, &pCd->Fpos.minute, NULL, -1) == 0);
                pCd->pEntry2 = &pCd->pEntry[pCd->Entry_no];
                pCd->Unk8B0 = pCd->pEntry2->Size;
                break;
            case 2:
                DsReadyCallback(NULL);
                if (pCd->Cd_rtn_no != Ret) {
                    Mode = Mode_org;
                    goto retry2;
                }
                while (DsCommand(DslPause, NULL, NULL, -1) == 0);
                DsReset();
                Mode = Mode_org;
                VSync(3);
                goto retry2;
            case 0xFF:
                DsReadyCallback(NULL);
                pCd->Unk8C0 = 0;
                break;
            }
        }
        if (Mode != 0) {
            Task_sleep(1);
        } else {
            VSync(0);
        }
        if (pCd->Time_out++ > 600) {
            goto retry;
        }
        if (pCd->Cd_rtn_no > 1) {
            goto retry;
        }
        if (pCd->Cd_rtn_no == 0 && pCd->First_flg == 0) {
            break;
        }
    }
    switch (pCd->Type) {
    case 3:
        while (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 0);
        return pCd->pEntry[8].Size;
    case 4:
        if (pCd->Em_id[0] < 0x50) {
            func_80014D10();
        }
        G.pEm_mem = (ENEMY_WORK *)pCd->Unk8CC;
        break;
    default:
        return pCd->Fsize_read;
    }
}

void func_800783BC(ULONG no, ULONG size, ULONG a2);
void func_800782F4(ULONG no, ULONG id, ULONG *pAdr);

void func_80012ECC(tagCD *pCd)
{
    ULONG no;
    LONG adr;

    no = pCd->Ring_status[pCd->Write_ctr] & 0x7F;
    while (SpuIsTransferCompleted(0) == 0);
    if (pCd->First_flg == 0) {
        switch (pCd->pEntry[no].Type) {
        case 3:
            adr = D_800A0FEC[4] - ((pCd->pEntry[no].Size + 0x3F) & ~0x3F);
            break;
        case 6:
            adr = D_800A0FEC[6] - ((pCd->pEntry[no].Size + 0x3F) & ~0x3F);
            break;
        default:
            adr = D_800A0FEC[pCd->pEntry[no].Type];
            break;
        }
        pCd->Unk8AC = pCd->pEntry[no].Size;
        SpuSetTransferStartAddr(adr);
        pCd->First_flg = 1;
    }
    if (pCd->Unk8AC > 0x800) {
        pCd->Unk8AC -= 0x800;
        SpuWritePartly((UCHAR *)pCd->Buff_adr + (pCd->Write_ctr << 11), 0x800);
    } else {
        adr = (pCd->Unk8AC + 0x3F) & ~0x3F;
        pCd->Unk8AC = 0;
        pCd->First_flg = 0;
        SpuWritePartly((UCHAR *)pCd->Buff_adr + (pCd->Write_ctr << 11), adr);
        while (SpuIsTransferCompleted(0) == 0);
        func_800783BC(pCd->pEntry[no].Type, pCd->pEntry[no].Size, 0);
        if (pCd->pEntry[no].Type >= 5) {
            func_800782F4(pCd->pEntry[no].Type - 5, pCd->pEntry[no].Id,
                          pCd->pEntry[no].Type < 6 ? (ULONG *)0x801F7E00 : (ULONG *)0x801F2600);
        }
    }
}

ULONG Em_kind_search(ULONG Id);
UCHAR func_800520D8(ULONG Id);
void *func_8005211C(ULONG Id);
ULONG func_8001760C(ULONG Id, ENEMY_WORK *pEm, ULONG Work, void *pData);

/* copies one ring sector of the room pack entry being streamed to its load address; finished enemy
 * texture (TIM) and model entries are uploaded / linked to the enemy works of that kind */
void func_800130A0(tagCD *pCd)
{
    ULONG Adr[7] = { 0, 0x80100000, 0x8010D000, 0x801FB600, 0, 0x801F7E00, 0x801F2600 };
    TIM_IMAGE Tim;
    TIM_IMAGE *pTim;
    tagGLOBAL *pG;
    EM_DAT *pDat;
    ENEMY_WORK *pEm;
    void *pData;
    ULONG Work;
    ULONG Size;
    ULONG Type;
    ULONG No;
    ULONG Kind;
    ULONG k;

    No = pCd->Ring_status[pCd->Write_ctr] & 0x7F;
    pG = &G;
    pDat = &D_800DECC0[D_800E0170];
    Type = pCd->pEntry[No].Type & 0x7F;
    if (Type == 5 && pCd->pEntry[No].Id != pCd->Unk8C2) {
        return;
    }
    if (Type == 6 && pCd->pEntry[No].Id != pCd->Unk8C3) {
        return;
    }
    pCd->Unk8D4 = 0;
    if (pCd->First_flg == 0) {
        if ((pCd->Unk8A8 = Adr[pCd->pEntry[No].Type & 0x7F]) == 0) {
            pCd->Unk8A8 = pCd->Unk8CC;
        }
        pCd->Unk8AC = (pCd->pEntry[No].Size + 3) & ~3;
        pCd->First_flg = 1;
        if (pCd->pEntry[No].Unk5 == 1) {
            pCd->First_flg = 2;
        }
    }
    if (pCd->Unk8AC > 0x800) {
        Size = 0x800;
        pCd->Unk8AC -= 0x800;
    } else {
        if (pCd->Type == 4) {
            if (pCd->Unk8BC != 0) {
                pCd->Unk8CC += (pCd->Unk8BC + 3) & ~3;
                pCd->Unk8BC = 0;
            }
            if (pCd->pEntry[No].Unk5 == 1) {
                pCd->Unk8D4 = 1;
            }
            if (pCd->pEntry[No - 1].Unk5 == 1) {
                pCd->Unk8D4 = 2;
            }
        } else if ((pCd->pEntry[No].Type & 0x7F) == 0) {
            pCd->Unk8CC += (pCd->pEntry[8].Size + 3) & ~3;
        }
        Size = pCd->Unk8AC;
        pCd->Unk8AC = 0;
        pCd->First_flg = 0;
    }
    func_800100A4((void *)pCd->Unk8A8, (UCHAR *)pCd->Buff_adr + (pCd->Write_ctr << 11), Size);
    pCd->Unk8A8 += 0x800;
    if (pCd->Unk8D4 == 1) {
        for (; pG->ppEnemy_end != &pG->pEnemy[pCd->Unk8D4]; pCd->Unk8D4++) {
            if (pCd->pEntry[No].Id == pG->pEnemy[pCd->Unk8D4]->Id) {
                pG->pEnemy[pCd->Unk8D4]->Clut = pCd->Unk8D1;
                pG->pEnemy[pCd->Unk8D4]->Tpage = pCd->Unk8D0;
                if (pCd->Unk8D4 == 1) {
                    break;
                }
            }
        }
        OpenTIM((u_long *)pCd->Unk8CC);
        pTim = ReadTIM(&Tim);
        /* texture pages 16.. are the second row: x wraps at 1024, y = 256 */
        pTim->prect->x = (pCd->Unk8D0 << 6) - (pCd->Unk8D0 >= 0x10 ? 0x400 : 0);
        pTim->prect->y = (pCd->Unk8D0 >= 0x10) << 8;
        LoadImage(pTim->prect, pTim->paddr);
        pCd->Unk8D0 += (pTim->prect->w + 0x3F) / 64;
        if (pTim->caddr != NULL) {
            pTim->crect->y = pCd->Unk8D1 + 0x1E0;
            LoadImage(pTim->crect, pTim->caddr);
            pCd->Unk8D1 += pTim->crect->h;
        }
        DrawSync(0);
    }
    if (pCd->pEntry[No - 1].Unk5 == 1 && pCd->Unk8D4 == 2) {
        pCd->Unk8D4 = 1;
        for (k = 1; pG->ppEnemy_end != &pG->pEnemy[k]; pCd->Unk8D4 = k + 1, k = pCd->Unk8D4) {
            if (pCd->pEntry[No].Id == pG->pEnemy[k]->Id) {
                pEm = pG->pEnemy[k];
                break;
            }
        }
        Work = pCd->Unk8CC;
        Sce.Unk148E = pEm->Id;
        pEm->Unk4B &= 0xF;
        Sce.Unk148F = pEm->Unk4B;
        Sce.pUnk1460 = pEm;
        Kind = Em_kind_search(pEm->Id);
        pData = func_8005211C(Kind);
        if (pData == NULL) {
            pDat->pEm = pEm;
            pDat->Id = pEm->Id;
            pDat->Kind = Kind;
            Sce.Unk1488++;
        } else {
            pEm->Unk4B |= func_800520D8(pEm->Id);
        }
        Work = func_8001760C(pCd->pEntry[No].Id, pEm, Work, pData);
        Sce.pUnk1490 = pEm->pUnk11C;
        Sce.Type = 0;
        Sce.pCScd = pG->pRoom->pScd;
        Work = PartsWork_set(pEm, Work);
        Work = func_800261AC(pEm, Work, pEm->pUnk10C);
        Sa_dat_set(pEm, pEm->pUnk118);
        pCd->Unk8CC = Work;
        if (pEm->Id < 0x50) {
            if (pEm->Id == pCd->Em_id[0]) {
                pCd->Unk8D4 = 0;
            } else {
                pCd->Unk8D4 = 1;
            }
            pG->Em_bin_info[pCd->Unk8D4].Use = 1;
            pG->Em_bin_info[pCd->Unk8D4].Id = pCd->pEntry[No].Id;
            pG->Em_routine[pCd->pEntry[No].Id - 0x10] = pG->Em_ov_mtbl[pCd->Unk8D4][pCd->pEntry[No].Id - 0x10];
            return;
        }
        if (*Sce.pUnk1490 != 0) {
            func_8007A2D4(pEm, Sce.pUnk1490);
        }
    }
}

void func_80013754(void)
{
    tagCD *pCd = &CD;
    UCHAR i;
    UCHAR j;
    ULONG Size;

    pCd->Unk8C0 = 1;
    pCd->Unk8C8 |= 1;
    if (pCd->pEntry[0].Id == pCd->Unk8C2 && Snd_work.Seq_data[0].Id != pCd->pEntry[0].Id) {
        pCd->Unk8C0 = 0;
        return;
    }
    if (pCd->pEntry[2].Id == pCd->Unk8C2 && Snd_work.Seq_data[0].Id != pCd->pEntry[2].Id) {
        i = 2;
    } else {
        for (i = 4; i < 8; i += 2) {
            if (pCd->pEntry[i].Id == pCd->Unk8C3 && Snd_work.Seq_data[1].Id != pCd->pEntry[i].Id) {
                break;
            }
        }
        if (i >= 6) {
            pCd->Unk8C8 |= 2;
        }
    }
    j = 0;
    Size = 0x800;
    do {
        Size += (pCd->pEntry[j].Size + 0x7FF) & ~0x7FF;
    } while (++j < i);
    Cd_set_read_pos(Size);
    pCd->Entry_no = i;
}

void func_800138B0(void)
{
    tagGLOBAL *pG;
    tagCD *pCd;
    CD_ENTRY *pEnt;
    ULONG Size;
    ULONG i;
    ULONG Key;

    Size = 0;
    i = 0;
    pEnt = CD_DIR;
    pG = &G;
    pCd = &CD;
    Key = D_800E017F | 0x80;
    pCd->Unk8C8 |= 1;
    do {
        if (pEnt->Id == Key) {
            pG->pRoom->pEdd = (UCHAR *)pCd->Unk8CC;
            if (pEnt == CD_DIR) {
                pCd->Unk8BC = pCd->pEntry[0].Size;
            } else {
                for (; &pCd->pEntry[i] != pEnt; i++) {
                    Size += (pCd->pEntry[i].Size + 0x7FF) & ~0x7FF;
                }
                Size += 0x800;
                Cd_set_read_pos(Size);
                pCd->Entry_no = i;
                pCd->Unk8C0 = 1;
                pCd->Unk8BC = pCd->pEntry[i].Size;
            }
            return;
        }
        pEnt++;
    } while (pEnt->Id & 0x80);
    func_800139D8();
}

INCLUDE_ASM("asm/nonmatchings/game/cd", func_800139D8);

/* RE2 Cd_ready_cb: DsReadyCallback during Cd_read / Cd_read_s */
void func_80013C1C(UCHAR intr, UCHAR *result)
{
    tagCD *pCd = &CD;
    ULONG read_num;
    ULONG Size;
    ULONG i;
    UCHAR *sum_ck;

    read_num = pCd->Read_ctr;
    i = 4;
    if (intr == DslDataReady) {
        DsGetSector(pCd->Work_buf, 3);
        if (pCd->Current_sector != DsPosToInt((DslLOC *)pCd->Work_buf)) {
            pCd->Cd_rtn_no = 3;
            return;
        }
        pCd->Current_sector++;
        pCd->Limit_break = 0;
        pCd->Fsector_num--;
        pCd->Fsector_read++;
        if (pCd->Type == 1) {
            if (pCd->Fsize_now > 0x800) {
                DsGetSector(pCd->Read_adr, 0x200);
                sum_ck = (UCHAR *)pCd->Read_adr;
                pCd->Fsize_now -= 0x800;
                pCd->Fsize_read += 0x800;
                do {
                    pCd->Sum ^= *sum_ck;
                    sum_ck += 0x200;
                    pCd->Sum_cnt += 0x200;
                } while (--i != 0);
                pCd->Read_adr += 0x200;
            } else {
                DsGetSector(pCd->Work_buf, 0x200);
                func_800100A4(pCd->Read_adr, pCd->Work_buf, pCd->Fsize_now);
                sum_ck = pCd->Work_buf;
                pCd->Fsize_read += pCd->Fsize_now;
                pCd->Fsize_now = 0;
                do {
                    pCd->Sum ^= *sum_ck;
                    sum_ck += 0x200;
                    pCd->Sum_cnt += 0x200;
                } while (pCd->Sum_cnt < pCd->Fsize_read);
                DsReadyCallback(NULL);
            }
        } else if (pCd->Type == 2) {
            if (pCd->Ring_status[read_num] != 0) {
                pCd->Cd_rtn_no = 5;
                return;
            }
            DsGetSector((UCHAR *)pCd->Buff_adr + (read_num << 11), 0x200);
            pCd->Ring_status[read_num] = 1;
            if (pCd->Fsize_now > 0x800) {
                pCd->Fsize_now -= 0x800;
                pCd->Ring_size[read_num] = 0x800;
                sum_ck = (UCHAR *)pCd->Buff_adr + (read_num << 11);
                pCd->Fsize_read += 0x800;
                do {
                    pCd->Sum ^= *sum_ck;
                    sum_ck += 0x200;
                    pCd->Sum_cnt += 0x200;
                } while (--i != 0);
            } else {
                pCd->Ring_size[read_num] = pCd->Fsize_now;
                Size = pCd->Fsize_now;
                pCd->Fsize_now = 0;
                sum_ck = (UCHAR *)pCd->Buff_adr + (read_num << 11);
                pCd->Fsize_read += Size;
                do {
                    pCd->Sum ^= *sum_ck;
                    sum_ck += 0x200;
                    pCd->Sum_cnt += 0x200;
                } while (pCd->Sum_cnt < pCd->Fsize_read);
                DsReadyCallback(NULL);
            }
            if (++pCd->Read_ctr == CD_RING_BUFFERS) {
                pCd->Read_ctr = 0;
            }
        }
    }
    if (intr == DslDiskError) {
        DsSyncCallback(NULL);
        DsReadyCallback(NULL);
        pCd->Cd_rtn_no = 2;
    }
}

/* DsReadyCallback for a room sound pack (Cd_read Type 3): directory sector, then the entries into the ring */
void func_80013EF8(UCHAR intr, UCHAR *result)
{
    tagCD *pCd = &CD;
    ULONG read_num;
    ULONG Size;
    ULONG i;
    LONG Len;
    UCHAR *pBuf;
    UCHAR *sum_ck;

    read_num = pCd->Read_ctr;
    if (intr == DslDataReady) {
        if (pCd->Unk8C0 != 0) {
            return;
        }
        DsGetSector(pCd->Work_buf, 3);
        if (pCd->Current_sector != DsPosToInt((DslLOC *)pCd->Work_buf)) {
            if (pCd->Fsector_read != 0) {
                goto retry;
            }
            pCd->Unk8C0 = 2;
            pCd->Cd_rtn_no = 4;
            return;
        }
        pCd->Current_sector++;
        pCd->Fsector_num--;
        pCd->Fsize_now -= 0x800;
        pCd->Fsize_read += 0x800;
        if (++pCd->Fsector_read == 1) {
            DsGetSector(CD_DIR_HEAD, 0x200);
            pCd->Unk8B4 = *CD_DIR_HEAD;
            if (CD_ENTRY_KIND(&pCd->pEntry[0]) != 5 || CD_ENTRY_KIND(&pCd->pEntry[1]) != 0x205) {
                pCd->Unk8C0 = 2;
                pCd->Cd_rtn_no = 4;
                return;
            }
            func_80013754();
            pCd->pEntry2 = &pCd->pEntry[pCd->Entry_no];
            if ((pCd->Unk8B0 = pCd->pEntry2->Size) < 0) {
                pCd->Unk8C0 = 2;
                pCd->Cd_rtn_no = 4;
                pCd->Unk8B0 = 0x800;
            }
            return;
        }
        if (pCd->Ring_status[read_num] != 0) {
            pCd->Limit_break = 3;
            goto retry;
        }
        pBuf = (UCHAR *)pCd->Buff_adr + (read_num << 11);
        DsGetSector(pBuf, 0x200);
        pCd->Ring_status[read_num] = pCd->Entry_no | 0x80;
        pCd->Ring_size[read_num] = 0x800;
        Len = 0x800;
        if (pCd->Unk8B0 < 0x800) {
            Len = pCd->Unk8B0;
        }
        sum_ck = pBuf;
        pCd->Unk8B0 -= 0x800;
        pCd->Sum_cnt = 0;
        do {
            pCd->Sum ^= *sum_ck;
            sum_ck += 0x200;
            pCd->Sum_cnt += 0x200;
        } while ((LONG)pCd->Sum_cnt < Len);
        if (++pCd->Read_ctr == CD_RING_BUFFERS) {
            pCd->Read_ctr = 0;
        }
        if (pCd->Unk8B0 > 0) {
            goto end;
        }
        if (pCd->Sum == pCd->pEntry2->Sum) {
            goto match;
        }
    retry:
        Size = 0x800;
        for (i = 0; i < pCd->Entry_no; i++) {
            Size += (pCd->pEntry[i].Size + 0x7FF) & ~0x7FF;
        }
        for (i = 0; i < CD_RING_BUFFERS; i++) {
            if (pCd->Entry_no == (pCd->Ring_status[i] & 0x7F)) {
                pCd->Ring_status[i] = 0;
                pCd->Ring_size[i] = 0;
            }
        }
        pCd->Read_ctr = pCd->Write_ctr;
        if (pCd->Ring_status[pCd->Read_ctr] != 0) {
            do {
                if (++pCd->Read_ctr >= CD_RING_BUFFERS) {
                    pCd->Read_ctr = 0;
                }
                if (pCd->Ring_status[pCd->Read_ctr] == 0) {
                    goto set;
                }
            } while (pCd->Read_ctr != pCd->Write_ctr);
            pCd->Unk8C0 = 2;
            return;
        }
        pCd->First_flg = 0;
    set:
        pCd->Unk8C0 = 1;
        Cd_set_read_pos(Size);
        if (pCd->Limit_break++ >= 4) {
            pCd->Unk8C0++;
        }
        goto next;
    match:
        if (pCd->Fsize_now == 0) {
            pCd->Unk8C0 = 0xFF;
            return;
        }
        pCd->pEntry2++;
        pCd->Limit_break = 0;
        pCd->Entry_no++;
        if ((pCd->Unk8C8 & 3) != 3) {
            func_8001480C(pCd);
        }
    next:
        pCd->Unk8B0 = pCd->pEntry2->Size;
        pCd->Sum = 0;
    }
end:
    if (intr == DslDiskError) {
        pCd->Unk8C0 = 2;
        pCd->Cd_rtn_no = 2;
    }
}

void func_80014300(UCHAR intr, UCHAR *result)
{
    tagCD *pCd = &CD;
    ULONG read_num;
    ULONG Size;
    ULONG i;
    LONG Len;
    UCHAR *pBuf;
    ULONG *p;
    UCHAR *sum_ck;

    read_num = pCd->Read_ctr;
    if (intr == DslDataReady) {
        if (pCd->Unk8C0 != 0) {
            return;
        }
        DsGetSector(pCd->Work_buf, 3);
        if (pCd->Current_sector != DsPosToInt((DslLOC *)pCd->Work_buf)) {
            if (pCd->Fsector_read != 0) {
                goto retry;
            }
            pCd->Unk8C0 = 2;
            pCd->Cd_rtn_no = 4;
            return;
        }
        pCd->Current_sector++;
        pCd->Fsector_num--;
        pCd->Fsize_now -= 0x800;
        pCd->Fsize_read += 0x800;
        if (++pCd->Fsector_read == 1) {
            DsGetSector(CD_DIR_HEAD, 0x200);
            pCd->Unk8B4 = *CD_DIR_HEAD;
            p = CD_DIR_HEAD + 0x100;
            do {
                if (*p++ != 0) {
                    goto err;
                }
            } while (p != CD_DIR_HEAD + 0x200);
            if (pCd->pEntry[0].Id & 0x80) {
                func_800138B0();
            } else {
                pCd->Unk8C8 |= 1;
                func_800139D8();
            }
            pCd->pEntry2 = &pCd->pEntry[pCd->Entry_no];
            if ((pCd->Unk8B0 = pCd->pEntry2->Size) < 0) {
                pCd->Unk8C0 = 2;
                pCd->Cd_rtn_no = 4;
                pCd->Unk8B0 = 0x800;
            }
            return;
        }
        if (pCd->Ring_status[read_num] != 0) {
            pCd->Limit_break = 3;
            goto retry;
        }
        pBuf = (UCHAR *)pCd->Buff_adr + (read_num << 11);
        DsGetSector(pBuf, 0x200);
        pCd->Ring_status[read_num] = pCd->Entry_no | 0x80;
        pCd->Ring_size[read_num] = 0x800;
        Len = 0x800;
        if (pCd->Unk8B0 < 0x800) {
            Len = pCd->Unk8B0;
        }
        sum_ck = pBuf;
        pCd->Unk8B0 -= 0x800;
        pCd->Sum_cnt = 0;
        do {
            pCd->Sum ^= *sum_ck;
            sum_ck += 0x200;
            pCd->Sum_cnt += 0x200;
        } while ((LONG)pCd->Sum_cnt < Len);
        if (++pCd->Read_ctr == CD_RING_BUFFERS) {
            pCd->Read_ctr = 0;
        }
        if (pCd->Unk8B0 > 0) {
            goto end;
        }
        if (pCd->Sum == pCd->pEntry2->Sum) {
            goto match;
        }
    retry:
        Size = 0x800;
        for (i = 0; i < pCd->Entry_no; i++) {
            Size += (pCd->pEntry[i].Size + 0x7FF) & ~0x7FF;
        }
        for (i = 0; i < CD_RING_BUFFERS; i++) {
            if (pCd->Entry_no == (pCd->Ring_status[i] & 0x7F)) {
                pCd->Ring_status[i] = 0;
                pCd->Ring_size[i] = 0;
            }
        }
        pCd->Read_ctr = pCd->Write_ctr;
        if (pCd->Ring_status[pCd->Read_ctr] != 0) {
            do {
                if (++pCd->Read_ctr >= CD_RING_BUFFERS) {
                    pCd->Read_ctr = 0;
                }
                if (pCd->Ring_status[pCd->Read_ctr] == 0) {
                    goto set;
                }
            } while (pCd->Read_ctr != pCd->Write_ctr);
            pCd->Unk8C0 = 2;
            return;
        }
        pCd->First_flg = 0;
    set:
        pCd->Unk8C0 = 1;
        Cd_set_read_pos(Size);
        if (pCd->Unk85F != 0) {
            pCd->Fsize_now = (pCd->pEntry2->Size + 0x7FF) & ~0x7FF;
            pCd->Fsize_read = (pCd->Fsize - (pCd->pEntry2->Size + 0x7FF)) & ~0x7FF;
            pCd->Fsector_num = pCd->Fsize_now >> 11;
            pCd->Fsector_read = pCd->Fsize_read >> 11;
        }
        if (pCd->Limit_break++ >= 4) {
            pCd->Unk8C0++;
        }
        goto next;
    match:
        if (pCd->Fsize_now == 0) {
            pCd->Unk8C0 = 0xFF;
            return;
        }
        pCd->pEntry2++;
        pCd->Limit_break = 0;
        pCd->Entry_no++;
        if ((pCd->Unk8C8 & 3) != 3) {
            if (G.Em_num == 0 && !(G.pEnemy[1]->Be_flg & 1)) {
                pCd->Fsize_now = 0;
                pCd->Fsector_num = 0;
                pCd->Unk8C0 = 0xFF;
                return;
            }
            func_800139D8();
            goto next;
        err:
            pCd->Unk8C0 = 2;
            pCd->Cd_rtn_no = 4;
            return;
        }
        if (pCd->pEntry2->Unk5 != 2) {
            func_80014968(pCd);
        }
    next:
        pCd->Unk8B0 = pCd->pEntry2->Size;
        pCd->Sum = 0;
    }
end:
    if (intr == DslDiskError) {
        pCd->Unk8C0 = 2;
        pCd->Cd_rtn_no = 2;
    }
}

void func_8001480C(tagCD *pCd)
{
    CD_ENTRY *pEnt;
    ULONG Size;
    ULONG i;

    i = 0;
    switch (pCd->Entry_no) {
    case 2:
        pCd->Unk8C0 = 1;
        i = 2;
    case 4:
        if (pCd->pEntry2[i].Id == pCd->Unk8C3 && D_800E06AC != pCd->pEntry2[i].Id) {
            if (i == 0) {
                return;
            }
            pCd->Unk8C0 = 1;
            pCd->Entry_no = 4;
        } else {
            i += 2;
            pCd->Unk8C0 = 1;
            if (pCd->pEntry2[i].Id == pCd->Unk8C3 && D_800E06AC != pCd->pEntry2[i].Id) {
                pCd->Entry_no = 6;
            } else {
                pCd->Entry_no = 8;
            }
        }
        break;
    case 6:
        pCd->Unk8C0 = 1;
        pCd->Entry_no = 8;
        break;
    default:
        return;
    }
    i = 0;
    if (pCd->Entry_no == 8) {
        pCd->Unk8C8 |= 2;
    }
    Size = 0x800;
    pCd->pEntry2 = pCd->pEntry;
    pEnt = pCd->pEntry2;
    do {
        Size += (pEnt[i].Size + 0x7FF) & ~0x7FF;
        i++;
    } while (i < pCd->Entry_no);
    Cd_set_read_pos(Size);
}

/* picks the next enemy sound bank to stream (Unk8C8 holds 3 state bits per CD.Em_id slot) */
void func_80014968(tagCD *pCd)
{
    CD_ENTRY *pEnt;
    ULONG Size;
    ULONG n;

    pEnt = CD_DIR;
    n = 0;
    pCd->Unk8D5 = 0;
    while (((pCd->Unk8C8 >> (pCd->Unk8D5 * 3)) & 0x1C) == 0x1C) {
        pCd->Unk8D5++;
    }
    if (pCd->Em_id[pCd->Unk8D5] >= 0x50) {
        pCd->Unk8C8 |= 4 << (pCd->Unk8D5 * 3);
    }
    switch ((pCd->Unk8C8 >> (pCd->Unk8D5 * 3)) & 0x1C) {
    case 0:
        Size = 0x800;
        for (; pEnt[1].Size != 0; pEnt++, n++) {
            if (!(pEnt->Id & 0x80) && pEnt->Type != 3 && pEnt->Id == pCd->Em_id[pCd->Unk8D5]) {
                break;
            }
            Size += (pEnt->Size + 0x7FF) & ~0x7FF;
        }
        if (pCd->Unk8D5 != 0) {
            n++;
            Size += (pEnt->Size + 0x7FF) & ~0x7FF;
            G.Em_bin_info[1].Use = 1;
        } else {
            G.Em_bin_info[0].Use = 1;
        }
        pCd->Unk8C8 |= 4 << (pCd->Unk8D5 * 3);
        Cd_set_read_pos(Size);
        pCd->Entry_no = n;
        pCd->Unk8C0 = 1;
        break;
    case 4:
        pCd->Unk8C8 |= 8 << (pCd->Unk8D5 * 3);
        if (G.Em_bin_info[0].Use == 0 && pCd->Em_id[pCd->Unk8D5] < 0x50) {
            return;
        }
        Size = 0x800;
        for (; pEnt[1].Size != 0; pEnt++, n++) {
            if (!(pEnt->Id & 0x80) && pEnt->Type != 3 && pEnt->Id == pCd->Em_id[pCd->Unk8D5]) {
                break;
            }
            Size += (pEnt->Size + 0x7FF) & ~0x7FF;
        }
        if (pCd->Em_id[pCd->Unk8D5] < 0x50) {
            n += 2;
            Size += (pEnt[0].Size + 0x7FF) & ~0x7FF;
            Size += (pEnt[1].Size + 0x7FF) & ~0x7FF;
        }
        Cd_set_read_pos(Size);
        pCd->Entry_no = n;
        pCd->Unk8C0 = 1;
        break;
    case 0xC:
        pCd->Unk8C8 |= 0x10 << (pCd->Unk8D5 * 3);
        if (pCd->Em_id[pCd->Unk8D5 + 1] == 0xFF || pCd->Unk8D5 == 3) {
            pCd->Fsize_now = (pCd->pEntry2->Size + 0x7FF) & ~0x7FF;
            pCd->Fsize_read = (pCd->Fsize - (pCd->pEntry2->Size + 0x7FF)) & ~0x7FF;
            pCd->Unk85F = 1;
            pCd->Fsector_num = pCd->Fsize_now >> 11;
            pCd->Fsector_read = pCd->Fsize_read >> 11;
            pCd->Unk8B8 += ((pCd->pEntry2->Size + 0x7FF) & ~0x7FF) >> 11;
        }
        break;
    }
}

/* enemies of the same kind share the model data of the first one (CD.Em_id[0], then Em_id[1]) */
void func_80014D10(void)
{
    tagGLOBAL *pG = &G;
    tagCD *pCd = &CD;
    ENEMY_WORK *pSrc;
    ENEMY_WORK *pEm;
    ULONG Work;
    UCHAR Second;
    UCHAR i;

    i = 2;
    if (pCd->Em_id[0] >= 0x50) {
        return;
    }
    for (; &pG->pEnemy[i] != pG->ppEnemy_end; i++) {
        if (pG->pEnemy[i]->Id == pCd->Em_id[0]) {
            pSrc = pG->pEnemy[i];
            i++;
            break;
        }
    }
    Second = pCd->Em_id[1] >= 0x50;
    if (&pG->pEnemy[i] == pG->ppEnemy_end && Second) {
        if (*pSrc->pUnk11C != 0) {
            func_8007A2D4(pSrc, pSrc->pUnk11C);
        }
        return;
    }
    Work = pCd->Unk8CC;
copy:
    for (; &pG->pEnemy[i] != pG->ppEnemy_end; i++) {
        pEm = pG->pEnemy[i];
        if (pEm->Id == pSrc->Id) {
            pEm->pUnk10C = pEm->pKan_t_ptr = pSrc->pKan_t_ptr;
            pEm->pSeq_t_ptr = pSrc->pSeq_t_ptr;
            pEm->pTmd = pSrc->pTmd;
            pEm->pUnk174 = pSrc->pUnk174;
            pEm->pSub0_kan_t_ptr = pSrc->pSub0_kan_t_ptr;
            pEm->pSub0_seq_t_ptr = pSrc->pSub0_seq_t_ptr;
            pEm->pSub1_kan_t_ptr = pSrc->pSub1_kan_t_ptr;
            pEm->pSub1_seq_t_ptr = pSrc->pSub1_seq_t_ptr;
            pEm->pUnk118 = pSrc->pUnk118;
            pEm->pUnk11C = pSrc->pUnk11C;
            pEm->Tpage = pSrc->Tpage;
            pEm->Clut = pSrc->Clut;
            pEm->pUnk1B4 = Sce.pUnk1460->pUnk1B4;
            pEm->Unk180[0] = Sce.pUnk1460->Unk180[0];
            pEm->Unk180[1] = Sce.pUnk1460->Unk180[1];
            pEm->Unk180[2] = Sce.pUnk1460->Unk180[2];
            pEm->Unk180[3] = Sce.pUnk1460->Unk180[3];
            Work = PartsWork_set(pEm, Work);
            Work = func_800261AC(pEm, Work, pEm->pUnk10C);
            Sa_dat_set(pEm, pEm->pUnk118);
        }
    }
    if (!Second) {
        for (i = 2; &pG->pEnemy[i] != pG->ppEnemy_end; i++) {
            if (pG->pEnemy[i]->Id == pCd->Em_id[1]) {
                pSrc = pG->pEnemy[i];
                break;
            }
        }
        i++;
        Second = 1;
        goto copy;
    }
    for (i = 2; &pG->pEnemy[i] != pG->ppEnemy_end; i++) {
        if ((pG->pEnemy[i]->Be_flg & 1) && *pG->pEnemy[i]->pUnk11C != 0) {
            func_8007A2D4(pG->pEnemy[i], pG->pEnemy[i]->pUnk11C);
        }
    }
    pCd->Unk8CC = Work;
}

void Cd_set_read_pos(ULONG size)
{
    tagCD *pCd = &CD;

    pCd->Fsize_read = size;
    pCd->Unk8B8 = size >> 11;
    pCd->Fsector_read = size >> 11;
    pCd->Fsize_now = pCd->Fsize - size;
    pCd->Fsector_num = (pCd->Fsize_now + 0x7FF) >> 11;
}

/* collects the distinct enemy kinds of the room in CD.Em_id[], sorted ascending, 0xFF = unused */
void func_800150E4(void)
{
    tagGLOBAL *pG = &G;
    tagCD *pCd = &CD;
    UCHAR i, j, n;

    for (j = 0; j < 4; j++) {
        pCd->Em_id[j] = 0xFF;
    }
    i = 1;
    n = 0;
    for (; pG->ppEnemy_end != &pG->pEnemy[i]; i++) {
        if (pG->pEnemy[i]->Id != 0 && pCd->Em_id[n] == 0xFF) {
            for (j = 0; j < n; j++) {
                if (pCd->Em_id[j] == pG->pEnemy[i]->Id) {
                    break;
                }
            }
            if (n == j) {
                pCd->Em_id[n] = pG->pEnemy[i]->Id;
                n++;
            }
        }
    }
    for (j = 0; j < 3; j++) {
        for (n = j + 1; n < 4; n++) {
            if (pCd->Em_id[j] > pCd->Em_id[n]) {
                i = pCd->Em_id[j];
                pCd->Em_id[j] = pCd->Em_id[n];
                pCd->Em_id[n] = i;
            }
        }
    }
}

void Set_volume(ULONG vol)
{
    tagCD *pCd = &CD;

    if (Stereo_flg == 0) {
        pCd->Cdvol.val0 = vol;
        pCd->Cdvol.val1 = 0;
        pCd->Cdvol.val2 = vol;
        pCd->Cdvol.val3 = 0;
    } else {
        pCd->Cdvol.val0 = vol >> 1;
        pCd->Cdvol.val1 = vol >> 1;
        pCd->Cdvol.val2 = vol >> 1;
        pCd->Cdvol.val3 = vol >> 1;
    }
    DsMix(&pCd->Cdvol);
}

void Change_volume(ULONG mode)
{
    LONG i;

    if (mode == 0) {
        for (i = (LONG)CD.Vol; i >= 0; i--) {
            Set_volume(i);
        }
    } else {
        i = 0;
        do {
            Set_volume(i);
            i++;
        } while ((ULONG)i <= CD.Vol);
    }
}

void Xa_fade_set(ULONG frames)
{
    tagCD *pCd = &CD;

    pCd->Fade_flg = 1;
    pCd->Fade_step = pCd->Vol / ((UCHAR)frames - 1);
}

void Cd_seek(ULONG File_no, ULONG Mode)
{
    tagCD *pCd = &CD;

    if (Mode < 2) {
        pCd->Fsum = floc[File_no].sum;
        pCd->Fsize = floc[File_no].size;
        pCd->Fsector = (ULONG)floc[File_no].sec_high * 0x10000 + floc[File_no].sector;
    }
    pCd->Fno = File_no;
    pCd->Current_sector = pCd->Fsector;
    DsFlush();
    DsIntToPos(pCd->Current_sector, &pCd->Fpos);
    while (DsCommand(DslPause, NULL, NULL, -1) == 0);
    while (DsCommand(DslSeekL, (u_char *)&pCd->Fpos, NULL, -1) == 0);
}
