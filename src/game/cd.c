#include "cd.h"
#include "door.h"
#include "em_sub.h"
#include "main.h"
#include "monitor.h"
#include "sce0.h"
#include "sub.h"
#include "joint.h"
#include "snd.h"

/* local declarations (80011CF8.c) */
void func_80017318(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80017540(ENEMY_WORK *pEm);
void func_8001CDA4(UCHAR *p, ULONG n);
extern ULONG *D_800BA6F8;
ULONG PresetObjectGT3_neo(void *pMw, ULONG pPacket);
ULONG func_800102E8(void);
void func_80010150(void *p, ULONG size);
extern UCHAR D_80098728[];
extern UCHAR D_800BF754[];
/* 8-byte animation/sequence record (byte 2 = frame) */
typedef struct {
    UCHAR Unk0;
    UCHAR Unk1;
    UCHAR Unk2;
    UCHAR Unk3;
    ULONG Unk4;
} SEQ8;
/* effect/object pool entry (0xD4 bytes, pool D_800BA8A8[0x60], searched downwards from D_800BF754) */
typedef struct tagEFF_WORK {
    UCHAR Routine_0;                   /* 0x00 */
    UCHAR Routine_1;                   /* 0x01 */
    UCHAR Routine_2;                   /* 0x02 */
    UCHAR Unk03;                       /* 0x03 */
    USHORT Attr;                       /* 0x04 */
    UCHAR pad06[0x8 - 0x6];
    SHORT Unk08;                       /* 0x08 */
    SHORT Unk0A;                       /* 0x0A */
    signed char Acc_x;                 /* 0x0C */
    signed char Acc_y;                 /* 0x0D */
    signed char Acc_z;                 /* 0x0E */
    UCHAR Unk0F;                       /* 0x0F */
    SHORT Spd_x;                       /* 0x10 */
    SHORT Spd_y;                       /* 0x12 */
    SHORT Spd_z;                       /* 0x14 */
    UCHAR pad16[0x18 - 0x16];
    UCHAR Unk18;                       /* 0x18 */
    UCHAR Snd_no;                      /* 0x19 */
    UCHAR Unk1A;                       /* 0x1A */
    UCHAR Unk1B;                       /* 0x1B */
    USHORT Unk1C;                      /* 0x1C */
    USHORT Unk1E;                      /* 0x1E */
    USHORT Unk20;                      /* 0x20 */
    USHORT Unk22;                      /* 0x22 */
    USHORT Be_flg;                     /* 0x24 */
    USHORT Flg2;                       /* 0x26 */
    USHORT Id;                         /* 0x28 */
    UCHAR Unk2A;                       /* 0x2A */
    UCHAR Timer;                       /* 0x2B */
    USHORT Unk2C;                      /* 0x2C */
    USHORT Unk2E;                      /* 0x2E */
    SHORT Rot_x;                       /* 0x30 */
    SHORT Rot_y;                       /* 0x32 */
    SHORT Rot_z;                       /* 0x34 */
    UCHAR pad36[0x38 - 0x36];
    LONG Unk38;                        /* 0x38 */
    LONG Unk3C;                        /* 0x3C */
    LONG Unk40;                        /* 0x40 */
    UCHAR pad44[0x48 - 0x44];
    LONG Pos_x;                        /* 0x48 */
    LONG Pos_y;                        /* 0x4C */
    LONG Pos_z;                        /* 0x50 */
    LONG Unk54;                        /* 0x54 */
    LONG Old_x;                        /* 0x58 */
    LONG Old_y;                        /* 0x5C */
    LONG Old_z;                        /* 0x60 */
    UCHAR pad64[0x68 - 0x64];
    SHORT Unk68;                       /* 0x68 */
    SHORT Unk6A;                       /* 0x6A */
    SHORT Unk6C;                       /* 0x6C */
    UCHAR pad6E;
    UCHAR Seq_no;                      /* 0x6F */
    MATRIX Workm;                      /* 0x70 */
    MATRIX Workm2;                     /* 0x90 copy of Workm (func_8001BCFC) */
    UCHAR padB0[0xB3 - 0xB0];
    UCHAR UnkB3;                       /* 0xB3 */
    SHORT UnkB4;                       /* 0xB4 */
    SHORT UnkB6;                       /* 0xB6 */
    SHORT UnkB8;                       /* 0xB8 */
    SHORT UnkBA;                       /* 0xBA */
    ULONG *pUnkBC;                     /* 0xBC */
    ULONG Key;                         /* 0xC0 */
    SEQ8 *pSeq;                        /* 0xC4 */
    UCHAR padC8[0xD0 - 0xC8];
    struct tagEFF_WORK *pLink;         /* 0xD0 killed when the linked work dies */
} EFF_WORK;
void Flg_on(ULONG *pFlg, ULONG Id);
ULONG Flg_ck(ULONG *pFlg, ULONG Id);
void func_8004834C(ULONG Id, ULONG a1);
extern ULONG D_800D1FC8[];
#define G_FLG(ofs) ((ULONG *)((UCHAR *)&G + (ofs)))
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);
ULONG Esp_data_set0(UCHAR *pData, ULONG *pOfs, ULONG Mode);
void Esp_data_set1(ULONG *pTim);
/* 0xC-byte table entry, 16 entries at D_800BA728 / D_800BA7E8 (filled by Esp_data_set0) */
typedef struct {
    ULONG *pData;                      /* 0x0 */
    ULONG *pData2;                     /* 0x4 */
    UCHAR Id;                          /* 0x8 */
    UCHAR pad9[3];
} TBL12;
extern TBL12 D_800BA728[16];
extern TBL12 D_800BA7E8[16];
extern EFF_WORK D_800BA8A8[0x60];
LONG func_80017148(ENEMY_WORK *pEm, LONG a1, LONG a2);
LONG func_800172AC(ENEMY_WORK *pEm, LONG a1, LONG a2);
extern UCHAR D_80097A00[];
extern UCHAR D_80097A28[];
extern ULONG D_800D1FC0[];
LONG func_800171DC(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80018EC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
extern UCHAR D_80097A78[];
extern UCHAR D_80097A50[];
EFF_WORK *func_8001B35C(EFF_WORK *pParent, ULONG a1, ULONG a2, MATRIX *pM, LONG *pPos, ULONG a5);
void func_8001CF24(EFF_WORK *pEw, ULONG a1, ULONG a2);
extern UCHAR *D_800BA700[10];          /* RE2 pDwork[] (door work slots) */
void Door_scheduler_main(void);
void Door_Trans(void);              /* RE2 Door_Trans */
void Door_model_init(UCHAR *pDw);
void func_8001CE24(EFF_WORK *pEw, ULONG a1, ULONG a2);
ULONG func_800261AC(ENEMY_WORK *pEm, ULONG work_top, ULONG a2);
EFF_WORK *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, ULONG a4);
void func_8001BCFC(EFF_WORK *pEw);
void func_8001BFCC(EFF_WORK *pEw);
extern UCHAR D_800E017F;
void func_800139D8(void);
/* hit-check request passed to func_800472EC */
typedef struct {
    LONG *pPos;                        /* 0x0 */
    SHORT W;                           /* 0x4 */
    SHORT H;                           /* 0x6 */
    SHORT D;                           /* 0x8 */
    SHORT padA;
    SHORT R;                           /* 0xC */
    SHORT padE;
} HIT_REQ;
LONG func_800472EC(HIT_REQ *pReq, ULONG a1);
extern ULONG D_80097FCC[];
LONG func_80047BD0(HIT_REQ *pReq, ULONG a1);
LONG Water_ck(LONG x, LONG z);
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);
typedef void (*EM_PROC)(ENEMY_WORK *pEm, LONG a1, LONG a2);
extern EM_PROC D_80097AA0[];
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);
void func_80060464(ENEMY_WORK *pEm, LONG a1, LONG a2);
void func_8006037C(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void Foot_set_pl(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no);
void Rot_neck(ENEMY_WORK *pEm, LONG Dir);
/* G viewed with the work pointer table as one array (RE2 pEnemy[]: [0] player, [1] sub player, [2..] enemies) */
typedef struct {
    UCHAR pad0[0x213C];
    ENEMY_WORK *pEm_mem;               /* 0x213C */
    UCHAR pad2140[0x2487 - 0x2140];
    UCHAR Em_num;                      /* 0x2487 */
    UCHAR pad2488[0x265C - 0x2488];
    ENEMY_WORK *pEm_tbl[42];           /* 0x265C */
    ENEMY_WORK **ppEnemy_end;          /* 0x2704 */
} G_EMTBL;
extern EFF_WORK *D_80098084;           /* pool start */
extern EFF_WORK *D_80098088;           /* pool end */
typedef void (*EFF_PROC)(EFF_WORK *pEw);
extern EFF_PROC D_80097BD4[];
void func_8001BC80(EFF_WORK *pEw);
void func_8001C168(EFF_WORK *pEw);
void func_8002B2BC(ULONG Kan, ULONG Seq, USHORT *pSeq, ULONG R_flg);
extern UCHAR D_800E06AC;
/* Add_speedXZ is called with a third (unused) argument in RE3 */
#define ADD_SPEED_XZ3(pEm) ((void (*)(ENEMY_WORK *, LONG, LONG))Add_speedXZ)((pEm), 0, 0)
extern ULONG D_800BA6F0;               /* RE2 pDoor[1] */
extern ULONG D_800BA6F4;               /* door work slot size */
void Door_texture_load(void);              /* RE2 Door_texture_load */
void Mapping_tim(ULONG *pTim);       /* RE2 Mapping_tim */
void Set_view(VECTOR *pView_p, VECTOR *pView_r);
extern EFF_WORK *D_8009808C;           /* pool 2 start */
extern EFF_WORK *D_80098090;           /* pool 2 end */
LONG Joint_move(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
ULONG Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName); /* RE2 Cd_read */
void func_8001B894(ULONG Id, ULONG Key);
void func_8001CFF4(EFF_WORK *pEw, ULONG n);
void Neck_Dir_ZY(VECTOR *pA, VECTOR *pB, SVECTOR *pRot, SHORT Front_dir_y); /* RE2 Neck_Dir_ZY */
/* PARTS_W Free[] work (RE2 pOwn_p->Free), 0x74.. in RE3 */
#define PW_FREE(p) ((SHORT *)((UCHAR *)(p) + 0x74))

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

INCLUDE_ASM("asm/nonmatchings/game/cd", Cd_read_s);

INCLUDE_ASM("asm/nonmatchings/game/cd", Cd_read);

extern LONG D_800A0FEC[];
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

INCLUDE_ASM("asm/nonmatchings/game/cd", func_800130A0);

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

INCLUDE_ASM("asm/nonmatchings/game/cd", func_800138B0);

INCLUDE_ASM("asm/nonmatchings/game/cd", func_800139D8);

INCLUDE_ASM("asm/nonmatchings/game/cd", func_80013C1C);

INCLUDE_ASM("asm/nonmatchings/game/cd", func_80013EF8);

INCLUDE_ASM("asm/nonmatchings/game/cd", func_80014300);

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

INCLUDE_ASM("asm/nonmatchings/game/cd", func_80014968);

INCLUDE_ASM("asm/nonmatchings/game/cd", func_80014D10);

void Cd_set_read_pos(ULONG size)
{
    tagCD *pCd = &CD;

    pCd->Fsize_read = size;
    pCd->Unk8B8 = size >> 11;
    pCd->Fsector_read = size >> 11;
    pCd->Fsize_now = pCd->Fsize - size;
    pCd->Fsector_num = (pCd->Fsize_now + 0x7FF) >> 11;
}

INCLUDE_ASM("asm/nonmatchings/game/cd", func_800150E4);

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
