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
ULONG func_8002AC98(void *pMw, ULONG pPacket);
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
void func_800746C0(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);
ULONG func_8001B204(UCHAR *pData, ULONG *pOfs, ULONG Mode);
void func_8001B2A4(ULONG *pTim);
/* 0xC-byte table entry, 16 entries at D_800BA728 / D_800BA7E8 (filled by func_8001B204) */
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
void func_80015964(void);              /* RE2 Door_Trans */
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
LONG func_80051E48(LONG x, LONG z);
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);
typedef void (*EM_PROC)(ENEMY_WORK *pEm, LONG a1, LONG a2);
extern EM_PROC D_80097AA0[];
void func_80049BA8(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);
void func_80060464(ENEMY_WORK *pEm, LONG a1, LONG a2);
void func_8006037C(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80019318(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no);
void func_8001A248(ENEMY_WORK *pEm, LONG Dir);
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
void func_80016208(void);              /* RE2 Door_texture_load */
void func_800784E0(ULONG *pTim);       /* RE2 Mapping_tim */
void Set_view(VECTOR *pView_p, VECTOR *pView_r);
extern EFF_WORK *D_8009808C;           /* pool 2 start */
extern EFF_WORK *D_80098090;           /* pool 2 end */
LONG func_80026BE8(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
ULONG func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName); /* RE2 Cd_read */
void func_8001B894(ULONG Id, ULONG Key);
void func_8001CFF4(EFF_WORK *pEw, ULONG n);
void func_8001AC58(VECTOR *pA, VECTOR *pB, SVECTOR *pRot, SHORT Front_dir_y); /* RE2 Neck_Dir_ZY */
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

INCLUDE_RODATA("asm/nonmatchings/game/80011CF8", D_8001031C);

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

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Cd_read_s);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80012818);

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

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800130A0);

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

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800138B0);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800139D8);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80013C1C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80013EF8);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80014300);

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

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80014968);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80014D10);

void Cd_set_read_pos(ULONG size)
{
    tagCD *pCd = &CD;

    pCd->Fsize_read = size;
    pCd->Unk8B8 = size >> 11;
    pCd->Fsector_read = size >> 11;
    pCd->Fsize_now = pCd->Fsize - size;
    pCd->Fsector_num = (pCd->Fsize_now + 0x7FF) >> 11;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_800150E4);

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

void Door_main(void)
{
    UCHAR i;

    for (i = 0; Door_proc_tbl[i] != NULL; i++) {
        Door_proc_tbl[i]();
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Door_init);

void Door_move(void)
{
    tagGLOBAL *pG;
    SCENARIO *pSce;

    while (G.System_flg & 0x800000) {
        Task_sleep(1);
    }
    if (!(*(USHORT *)(D_800BA700[0] + 0xC0) & 0x4000)) {
        Main.Vloop = 1;
    }
    if (Sce.Sce_task[10].Status != 0) {
        pG = &G;
        pSce = &Sce;
loop:
        if (!(pG->Status_flg & 0x800)) {
            *(SHORT *)((UCHAR *)pG + 0x7828) = 0;
        }
        Door_scheduler_main();
        func_80015964();
        (*(USHORT *)((UCHAR *)D_800BA6F8 + 0x226))++;
        Task_sleep(1);
        if (pSce->Sce_task[10].Status != 0) {
            goto loop;
        }
    }
}

void Door_scheduler_main(void)
{
    LONG Ifel_idx;
    ULONG Op_result;
    SCE_TASK *pSce;
    ULONG Task_idx;

    Sce.Type = 1;
    Sce.pCScd = (UCHAR *)0x8019D008;
    for (Task_idx = 10; Task_idx < 0xE; Task_idx++) {
        pSce = &Sce.Sce_task[Task_idx];
        if (pSce->Status == 0) {
            continue;
        }
        while (1) {
            Op_result = Sce_jmp_tbl[*pSce->Data](pSce);
            if (Op_result == 1) {
                continue;
            }
            if (Op_result == 2) {
                break;
            }
            if (pSce->Ifel_ctr[pSce->Sub_ctr] < 0) {
                break;
            }
            Op_result = (ULONG)pSce->pS_SP;
            pSce->pS_SP = (ULONG *)(Op_result - 4);
            Ifel_idx = pSce->Sub_ctr;
            pSce->Data = (UCHAR *)*pSce->pS_SP;
            pSce->Ifel_ctr[Ifel_idx]--;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80015964);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80015D98);

void Door_exit(void)
{
Door_exit_wait:
    if (Fade_status(0) != 0) {
        Task_sleep(1);
        goto Door_exit_wait;
    }
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x10);
    if (*(USHORT *)((UCHAR *)D_800BA6F8 + 0x240) != 0) {
        func_800746C0(0x401, NULL, 0, 0);
    }
    Main.Vloop = 2;
    G.System_flg &= ~0x400000;
    Task_exit();
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80016208);

void Door_model_init(UCHAR *pDw)
{
    ULONG *pDoor_cur;
    ULONG *Tmd_adr;

    pDoor_cur = D_800BA6F8;
    Tmd_adr = (ULONG *)D_800BA6F8[1];
    *(ULONG *)(pDw + 0x74) = 0x808080;
    *(ULONG **)(pDw + 0x14) = Tmd_adr + pDw[0xC2] * 6;
    *D_800BA6F8 = func_8002AC98(pDw + 0x10, pDoor_cur[0]);
}

/* RE3 door model work (pDwork[] slot), only the fields used here */
typedef struct {
    ULONG Be_flg;                      /* 0x00 */
    UCHAR pad04[0x10 - 0x4];
    LONG Attribute;                    /* 0x10 */
    UCHAR pad14[0x34 - 0x14];
    LONG Pos_x;                        /* 0x34 */
    LONG Pos_y;                        /* 0x38 */
    LONG Pos_z;                        /* 0x3C */
    UCHAR pad40[0x4A - 0x40];
    UCHAR Id;                          /* 0x4A */
    UCHAR pad4B;
    MATRIX Workm;                      /* 0x4C */
    SHORT Cdir_x;                      /* 0x6C */
    SHORT Cdir_y;                      /* 0x6E */
    SHORT Cdir_z;                      /* 0x70 */
    UCHAR pad72[0x78 - 0x72];
    MATRIX *pSuper;                    /* 0x78 */
    UCHAR pad7C[0xC0 - 0x7C];
    USHORT Attribute2;                 /* 0xC0 */
    UCHAR Model_no;                    /* 0xC2 */
    UCHAR Type;                        /* 0xC3 */
} DOOR_WORK;
/* Door_model_set script record (22 bytes) */
typedef struct {
    UCHAR Unk_00;
    UCHAR Unk_01;
    UCHAR Id;
    UCHAR Type;
    UCHAR Be_flg;
    UCHAR Model_no;
    USHORT Attribute2;
    SHORT Attribute;
    SHORT Pos_x;
    SHORT Pos_y;
    SHORT Pos_z;
    USHORT Cdir_x;
    USHORT Cdir_y;
    USHORT Cdir_z;
} DOOR_MODEL_SET_REC;

ULONG Door_model_set(SCE_TASK *pSce)
{
    ULONG Free_flg;
    MATRIX *pMat;
    DOOR_WORK *pDw;
    DOOR_MODEL_SET_REC *pData;

    pData = (DOOR_MODEL_SET_REC *)pSce->Data;
    pDw = (DOOR_WORK *)D_800BA700[pData->Unk_01];
    pDw->Id = pData->Id;
    pDw->Type = pData->Type;
    pDw->Be_flg = pData->Be_flg;
    pDw->Model_no = pData->Model_no;
    Free_flg = pDw->Attribute2 = pData->Attribute2;
    pDw->Attribute = pData->Attribute;
    pDw->Pos_x = pData->Pos_x;
    pDw->Pos_y = pData->Pos_y;
    pDw->Pos_z = pData->Pos_z;
    pDw->Cdir_x = pData->Cdir_x;
    pDw->Cdir_y = pData->Cdir_y;
    pDw->Cdir_z = pData->Cdir_z;
    if (Free_flg & 0x100) {
        pMat = &((DOOR_WORK *)D_800BA700[Free_flg & 0xF])->Workm;
    } else {
        pMat = &GsWSMATRIX;
    }
    pDw->pSuper = pMat;
    if (Free_flg & 0x2000) {
        *(USHORT *)((UCHAR *)D_800BA6F8 + 0x240) = 1;
    }
    pSce->Data = (UCHAR *)(pData + 1);
    Door_model_init((UCHAR *)pDw);
    return 1;
}

/* door texture/sound info (12 bytes, RE2 DOOR_INFO) */
typedef struct {
    USHORT Sound_size;                 /* 0x0 */
    USHORT Data_size;                  /* 0x2 */
    ULONG Sector_offset;               /* 0x4 */
    UCHAR Sound_sum;                   /* 0x8 */
    UCHAR Data_sum;                    /* 0x9 */
    UCHAR padA[2];
} DOOR_INFO;
extern DOOR_INFO D_800971E4[];
extern USHORT *D_800979D4[];
extern ULONG D_800BA6F0;
void func_8007836C(LONG no, ULONG *pAdr);

void Door_snd_trans(void)
{
    ULONG Sound_size;
    DOOR_INFO *pDoor_info;
    ULONG Door_no;
    UCHAR Sum;
    tagCD *pCd;
    ULONG File_no;
    ULONG *Address;

    Door_no = ((UCHAR *)G.pSce_aot_on)[0xC];
    G.Status_flg |= 0x800;
    pCd = &CD;
    pDoor_info = &D_800971E4[((UCHAR *)G.pSce_aot_on)[0xC]];
    while (D_800BA6F0 != 0) {
        Task_sleep(1);
    }
    Sound_size = pDoor_info->Sound_size;
    Sum = pDoor_info->Sound_sum;
    File_no = D_800979D4[G.Unk783E][Door_no];
    Address = (ULONG *)((UCHAR *)0x8019D000 - Sound_size);
    pCd->Fsector = floc[File_no].sector + floc[File_no].sec_high * 0x10000;
    pCd->Fsum = Sum;
    pCd->Fsize = Sound_size;
    func_80012818(File_no, Address, 3, "DOOR SOUND");
    func_800100A4((void *)0x801FC100, Address, Address[5] + 0x10);
    SpuSetTransferMode(0);
    func_8007836C(4, (ULONG *)0x801FC100);
    Snd_work.Bank_id[4].Id = 4;
    Snd_work.Bank_id[4].pData = (UCHAR *)0x3DE10;
    SpuSetTransferStartAddr(0x3DE10);
    SpuRead((UCHAR *)Address + ((Address[5] + 0x10) >> 2 << 2), Address[4]);
    while (SpuIsTransferCompleted(0) == 0);
    G.Status_flg &= ~0x800;
}

void func_80016600(ENEMY_WORK *pEm, LONG a1, LONG a2)
{
    VECTOR v;
    ULONG Flg;
    ULONG R;

    if (!(pEm->Type & 0x4000)) {
        pEm->Type |= 0x5000;
    }
    if (!(pEm->Type & 0x1000)) {
        v.vx = pEm->Unk14C;
        v.vz = pEm->Unk14E;
        v.vy = 0;
        func_80049BA8(pEm, &v, 0, 0);
    }
    Flg = pEm->Unk144;
    if (!(Flg & 0x10)) {
        D_80097AA0[pEm->Routine_1](pEm, a1, a2);
        pEm->Unk144 &= ~0x800;
    } else if (pEm->Routine_1 != 20) {
        if (!(Flg & 0x800)) {
            D_80097AA0[pEm->Routine_1](pEm, a1, a2);
            func_80060464(pEm, a1, a2);
        } else {
            func_8006037C(pEm, a1, a2, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200);
        }
    }
    if (pEm->Unk144 & 1) {
        func_80019318(pEm, 0, 0);
    }
    if (pEm->Unk144 & 2) {
        func_80019318(pEm, 0, 1);
    }
    if (pEm->Unk120 & 0x80) {
        func_8001A248(pEm, pEm->Cdir_y);
    }
}

void func_800167AC(ENEMY_WORK *pEm)
{
    func_80017318(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr);
    func_80017540(pEm);
}

void func_800167E4(ENEMY_WORK *pEm)
{
    func_80017318(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    func_80017540(pEm);
}

void func_8001681C(ENEMY_WORK *pEm)
{
    func_80017318(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    func_80017540(pEm);
}

void func_80016854(ENEMY_WORK *pEm)
{
    func_80017318(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
    func_80017540(pEm);
}

void func_8001688C(ENEMY_WORK *pEm)
{
    ULONG *pKan;
    ULONG *pSeq;
    LONG Move;

    pKan = pEm->pKan_t_ptr;
    pSeq = pEm->pSeq_t_ptr;
    Move = D_80097A28[pEm->Id - 0x10];
    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            break;
        case 2:
            pKan = pEm->pSub0_kan_t_ptr;
            pSeq = pEm->pSub0_seq_t_ptr;
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        Move = pEm->Unk152;
    }
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = Move + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)func_80017148(pEm, (LONG)pKan, (LONG)pSeq)) {
            func_80018EC8(pEm, pKan, pSeq, 0);
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if ((UCHAR)func_800171DC(pEm, (LONG)pKan, (LONG)pSeq, 0)) {
            Flg_on(D_800D1FC0, *((UCHAR *)pEm + 0x146));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 4;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_80017540(pEm);
}

void func_80016A40(ENEMY_WORK *pEm)
{
    ULONG *pKan;
    ULONG *pSeq;
    LONG Move;

    pKan = pEm->pKan_t_ptr;
    pSeq = pEm->pSeq_t_ptr;
    Move = D_80097A50[pEm->Id - 0x10];
    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            break;
        case 2:
            pKan = pEm->pSub0_kan_t_ptr;
            pSeq = pEm->pSub0_seq_t_ptr;
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        Move = pEm->Unk152;
    }
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = Move + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)func_80017148(pEm, (LONG)pKan, (LONG)pSeq)) {
            func_80018EC8(pEm, pKan, pSeq, 0);
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        func_80018EC8(pEm, pKan, pSeq, 0);
        if ((UCHAR)func_800171DC(pEm, (LONG)pKan, (LONG)pSeq, 0)) {
            Flg_on(D_800D1FC0, *((UCHAR *)pEm + 0x146));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 5;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_80017540(pEm);
}

void func_80016BF4(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70000 + (D_80097A00[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        if (D_80097A00[pEm->Id - 0x10] & 0x80) {
            func_80026BE8(pEm, (LONG)pEm->pSub0_kan_t_ptr, (LONG)pEm->pSub0_seq_t_ptr, 0x200);
        } else {
            func_80026BE8(pEm, (LONG)pEm->pKan_t_ptr, (LONG)pEm->pSeq_t_ptr, 0x200);
        }
        break;
    }
}

void func_80016C9C(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70000 + (D_80097A78[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, D_80097A78[pEm->Id - 0x10] & 0x40);
        if ((UCHAR)func_800171DC(pEm, (LONG)pEm->pKan_t_ptr, (LONG)pEm->pSeq_t_ptr, D_80097A78[pEm->Id - 0x10] & 0x40)) {
            Flg_on(D_800D1FC0, *((UCHAR *)pEm + 0x146));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 7;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_80017540(pEm);
}

void func_80016DA0(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Spd_z = 0;
        *(ULONG *)&pEm->Move_no = 0x70000 + (D_80097A78[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        func_80018EC8(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        if ((UCHAR)func_800171DC(pEm, (LONG)pEm->pKan_t_ptr, (LONG)pEm->pSeq_t_ptr, 0)) {
            Flg_on(D_800D1FC0, *((UCHAR *)pEm + 0x146));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 8;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_80017540(pEm);
}

void func_80016E80(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70000 + (D_80097A28[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)func_80017148(pEm, (LONG)pEm->pKan_t_ptr, (LONG)pEm->pSeq_t_ptr)) {
            Flg_on(D_800D1FC0, *((UCHAR *)pEm + 0x146));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 9;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_80017540(pEm);
}

void func_80016F44(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70000 + (D_80097A28[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)func_800172AC(pEm, (LONG)pEm->pKan_t_ptr, (LONG)pEm->pSeq_t_ptr)) {
            Flg_on(D_800D1FC0, *((UCHAR *)pEm + 0x146));
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0x15;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    func_80017540(pEm);
}

void func_80017008(ENEMY_WORK *pEm, LONG a1, LONG a2)
{
    ULONG Flg;
    ULONG R;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->UnkCA = 7;
        if (pEm->Unk144 & 0x40) {
            pEm->UnkCA = 0;
        }
    case 1:
        if (func_80026BE8(pEm, a1, a2, (((UCHAR)pEm->Unk144 >> 7) << 16) | 0x200)) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
            }
            pEm->Cdir_y += pEm->Unk14A;
        } else {
            Flg = pEm->Unk144;
            if (Flg & 8) {
                if (func_80026BE8(pEm, a1, a2, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200)) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += pEm->Unk14A;
                }
            }
            pEm->Cdir_y += pEm->Add_dir;
        }
        break;
    case 2:
        break;
    }
}

LONG func_80017148(ENEMY_WORK *pEm, LONG a1, LONG a2)
{
    if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x15E) == 0) {
        func_80026BE8(pEm, a1, a2, 0x200);
        return 1;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Add_dir);
    func_80026BE8(pEm, a1, a2, 0x200);
    return 0;
}

LONG func_800171DC(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3)
{
    LONG x, z;

    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * pEm->Add_dir);
    func_80026BE8(pEm, a1, a2, (a3 << 16) | 0x200);
    Add_speedXZ(pEm, 0);
    x = pEm->Pos_x - pEm->Dest_x;
    z = pEm->Pos_z - pEm->Dest_z;
    return SquareRoot0(x * x + z * z) < 100;
}

LONG func_800172AC(ENEMY_WORK *pEm, LONG a1, LONG a2)
{
    Goto01(pEm, pEm->Dest_x, pEm->Dest_z);
    func_80026BE8(pEm, a1, a2, 0x200);
    return pEm->Cdir_y == pEm->Dest_x;
}

void func_80017318(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG Flg;
    ULONG R;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->UnkCA = 7;
        if (pEm->Unk144 & 0x40) {
            pEm->UnkCA = 0;
        }
        func_80018EC8(pEm, pKan, pSeq, (UCHAR)pEm->Unk144 >> 7);
    case 1:
        func_80018EC8(pEm, pKan, pSeq, (UCHAR)pEm->Unk144 >> 7);
        if (pEm->Unk144 & 0x10) {
            pEm->Spd_x = pEm->Spd_x >> 1;
            pEm->Spd_y = pEm->Spd_y >> 1;
            pEm->Spd_z = pEm->Spd_z >> 1;
        }
        if (func_80026BE8(pEm, (LONG)pKan, (LONG)pSeq, (((UCHAR)pEm->Unk144 >> 7) << 16) | 0x200)) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
                if (pEm->Unk144 & 0x10) {
                    pEm->Unk144 |= 0x800;
                }
            }
            pEm->Cdir_y += pEm->Unk14A;
            if (!(pEm->Unk144 & 0x20)) {
                ADD_SPEED_XZ3(pEm);
            }
        } else {
            if (!(pEm->Unk144 & 0x20)) {
                ADD_SPEED_XZ3(pEm);
            }
            Flg = pEm->Unk144;
            if (Flg & 8) {
                if (func_80026BE8(pEm, (LONG)pKan, (LONG)pSeq, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200)) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += pEm->Unk14A;
                }
                if (!(pEm->Unk144 & 0x20)) {
                    ADD_SPEED_XZ3(pEm);
                }
            }
            pEm->Cdir_y += pEm->Add_dir;
        }
        break;
    case 2:
        break;
    }
}

void func_80017540(ENEMY_WORK *pEm)
{
    if (!(pEm->Unk144 & 0x100)) {
        Em_seq_sound(pEm);
    }
}

void func_80017574(void)
{
    G_EMTBL *pG = (G_EMTBL *)&G;
    ENEMY_WORK **ppEm;
    ULONG Mem_top;
    ULONG Em_num;
    ULONG Em_size;
    LONG i;

    ppEm = &pG->pEm_tbl[1];
    i = 41;
    do {
        *ppEm = (ENEMY_WORK *)((UCHAR *)pG + 0x2708);
        i--;
        ppEm++;
    } while (i != 0);
    Em_num = pG->Em_num;
    pG->pEm_tbl[0] = (ENEMY_WORK *)((UCHAR *)pG + 0x248C);
    pG->pEm_tbl[0]->Be_flg = 1;
    pG->ppEnemy_end = &pG->pEm_tbl[2];
    if (Em_num != 0) {
        Em_size = 0x1FC;
        Mem_top = (ULONG)pG->pEm_mem;
        pG->pEm_mem = (ENEMY_WORK *)((ULONG)pG->pEm_mem + Em_num * Em_size);
        i = 0;
        do {
            pG->pEm_tbl[i + 2] = (ENEMY_WORK *)Mem_top;
            ((ENEMY_WORK *)Mem_top)->Be_flg = 0;
            Mem_top += Em_size;
            i++;
        } while (--Em_num != 0);
        pG->ppEnemy_end = &pG->pEm_tbl[i + 2];
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001760C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80017ADC);

void func_80017D9C(void) {
}

ULONG Hani_ck(VECTOR *pV, SHORT *pPxzhw)
{
    if ((ULONG)(pV->vx - pPxzhw[0]) > (ULONG)pPxzhw[2] || (ULONG)(pV->vz - pPxzhw[1]) > (ULONG)pPxzhw[3]) {
        return 0;
    }
    return 1;
}

void Add_speedXZ(ENEMY_WORK *pEm, LONG muki)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    if ((pEm->UnkAE & 0x40) && !(pEm->Be_flg & BE_FLG_400)) {
        pEm->Spd_x = (SHORT)(pEm->Spd_x * 3) >> 2;
    }
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_z += Sv.vz;
}

void Add_speedXYZ(ENEMY_WORK *pEm, LONG muki_y, LONG muki_z)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki_y;
    Sv.vz = pEm->Cdir_z + (SHORT)muki_z;
    RotMatrix_gte(&Sv, &M);
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_y += Sv.vy;
    pEm->Pos_z += Sv.vz;
}

void Get_speedXZ(ENEMY_WORK *pEm, LONG muki, SHORT *pX, SHORT *pZ)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    ApplyMatrixSV(&M, (SVECTOR *)&pEm->Spd_x, &Sv);
    *pX = Sv.vx;
    *pZ = Sv.vz;
}

void Add_speedXYZsuper(ENEMY_WORK *pEm, LONG muki_y, LONG muki_z)
{
    SVECTOR Sv;
    MATRIX M;
    MATRIX Tm;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki_y;
    Sv.vz = pEm->Cdir_z + (SHORT)muki_z;
    RotMatrix_gte(&Sv, &M);
    func_8002D4B0(pEm->pSuper, &M, &Tm);
    ApplyMatrixSV(&Tm, (SVECTOR *)&pEm->Spd_x, &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_y += Sv.vy;
    pEm->Pos_z += Sv.vz;
}

LONG Direction_ck(LONG Hontai_x, LONG Hontai_z, LONG Target_x, LONG Target_z)
{
    USHORT Dir;
    LONG ZZ;
    LONG XX;
    LONG Dz;

    Dir = 0x400;
    XX = (SHORT)Target_x - (SHORT)Hontai_x;
    Dz = (SHORT)Target_z - (SHORT)Hontai_z;
    if (Dz > 0) {
        Dir = 0xC00;
    }
    if (XX != 0) {
        ZZ = (SHORT)catan(Dz * 0x1000 / XX);
        Dir = ((XX < 0 ? 0x800 : 0x1000) - ZZ) & 0xFFF;
    }
    return Dir;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Goto00);

LONG Goto00_ck(ENEMY_WORK *pEm, LONG Vec_x, LONG Vec_z, LONG Add_dir)
{
    ULONG Ang_err;

    Ang_err = (Add_dir + (Direction_ck((SHORT)pEm->Pos_x, (SHORT)pEm->Pos_z, (SHORT)Vec_x, (SHORT)Vec_z) - (USHORT)pEm->Cdir_y)) & 0xFFF;
    if ((LONG)Ang_err < (SHORT)Add_dir * 2) {
        return 0;
    }
    if (Ang_err < 0x801) {
        return (SHORT)Add_dir;
    }
    return -(SHORT)Add_dir;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Goto01);

LONG Goto01_ck(ENEMY_WORK *pEm, LONG Dir, LONG Add_dir)
{
    ULONG Rot;

    Rot = (Add_dir + (Dir - (USHORT)pEm->Cdir_y)) & 0xFFF;
    if ((LONG)Rot < (SHORT)Add_dir * 2) {
        return 0;
    }
    if (Rot < 0x801) {
        return (SHORT)Add_dir;
    }
    return -(SHORT)Add_dir;
}

LONG Dir_pos_ck(VECTOR *pV, VECTOR *pP, SHORT Hed_dir, SHORT Hani_dir)
{
    SHORT Dir;

    Dir = Direction_ck((SHORT)pV->vx, (SHORT)pV->vz, (SHORT)pP->vx, (SHORT)pP->vz);
    return ((((LONG)Dir - (LONG)Hed_dir) + (LONG)Hani_dir) & 0xFFFU) < (ULONG)((LONG)Hani_dir << 1) ^ 1;
}

LONG L_pos_ckXZ(VECTOR *pV, VECTOR *pP, ULONG Ll)
{
    LONG Dx;
    LONG Dz;

    Dx = (pV->vx - pP->vx) >> 1;
    Dz = (pV->vz - pP->vz) >> 1;
    return (ULONG)(SquareRoot0(Dx * Dx + Dz * Dz) * 2) < Ll;
}

LONG L_pos_ckXZM(MATRIX *pM, VECTOR *pP, ULONG Ll, SVECTOR *pSv)
{
    LONG Dx;
    LONG Dz;
    SVECTOR Sv;
    VECTOR V;

    if (pSv == NULL) {
        *(ULONG *)&Sv.vx = 0;
        Sv.vz = 0;
    } else {
        *(ULONG *)&Sv.vx = *(ULONG *)&pSv->vx;
        Sv.vz = pSv->vz;
    }
    ApplyMatrix(pM, &Sv, &V);
    V.vx += pM->t[0];
    V.vy += pM->t[1];
    V.vz += pM->t[2];
    Dx = (V.vx - pP->vx) >> 1;
    Dz = (V.vz - pP->vz) >> 1;
    return (ULONG)(SquareRoot0(Dx * Dx + Dz * Dz) * 2) < Ll;
}

UCHAR Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    return ((((LONG)pT->Cdir_y - (LONG)pEm->Cdir_y) + 0x400) & 0xFFF) < 0x800;
}

UCHAR Cdir_ck_pos(LONG Pos_x, LONG Pos_z, ENEMY_WORK *pT)
{
    return (ULONG)(((USHORT)pT->Cdir_y - Direction_ck((SHORT)Pos_x, (SHORT)Pos_z, (SHORT)pT->Pos_x, (SHORT)pT->Pos_z)) + 0x400 & 0xFFF) < 0x800;
}

UCHAR Cdir_ck2(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SHORT Dir;

    Dir = Direction_ck((SHORT)pEm->Pos_x, (SHORT)pEm->Pos_z, (SHORT)pT->Pos_x, (SHORT)pT->Pos_z);
    return ((((LONG)pT->Cdir_y - (LONG)Dir) + 0x400) & 0xFFF) < 0x800;
}

UCHAR Dir_ck(LONG Dir0, LONG Dir1)
{
    return ((((SHORT)Dir1 - (SHORT)Dir0) + 0x400) & 0xFFF) < 0x800;
}

ULONG Gacha_ch(void)
{
    return (G.Key_trg & 0xF4F) != 0;
}

ULONG func_800185D0(ENEMY_WORK *pEm, VECTOR *pTa, LONG Han, VECTOR *pAi)
{
    LONG Dir;
    LONG x;
    LONG z;
    ULONG L;
    SVECTOR Sv;
    MATRIX M;
    MATRIX M1;
    SVECTOR Sck0;
    SVECTOR Sck1;
    SVECTOR Tp0;
    SVECTOR Tp1;
    SVECTOR Tp2;

    Dir = (SHORT)Direction_ck((SHORT)pEm->Pos_x, (SHORT)pEm->Pos_z, (SHORT)pAi->vx, (SHORT)pAi->vz);
    x = (pEm->Pos_x - pAi->vx) >> 1;
    z = (pEm->Pos_z - pAi->vz) >> 1;
    L = SquareRoot0(x * x + z * z) * 2;


    *(LONG *)&Sv = Dir << 16;
    Sv.vz = 0;
    RotMatrix(&Sv, &M);
    *(LONG *)&Sv = L;
    ApplyMatrixSV(&M, &Sv, &Sck0);
    *(LONG *)&Sv = 0;
    Sv.vz = Han;
    ApplyMatrixSV(&M, &Sv, &Sck1);

    *(LONG *)&Tp0 = pTa->vx - (pEm->Pos_x + Sck1.vx);
    Tp0.vz = pTa->vz - (pEm->Pos_z + Sck1.vz);
    *(LONG *)&Tp1 = pTa->vx - (pEm->Pos_x - Sck1.vx);
    Tp1.vz = pTa->vz - (pEm->Pos_z - Sck1.vz);
    *(LONG *)&Tp2 = pTa->vx - (Tp0.vx + Sck0.vx);
    Tp2.vz = pTa->vz - (Tp0.vz + Sck0.vz);

    *(LONG *)&M.m[0][0] = Sck0.vx;
    M.m[0][2] = Sck0.vz;
    M.m[1][0] = Sck1.vx;
    *(LONG *)&M.m[1][1] = Sck1.vz << 16;

    *(LONG *)&M1.m[1][1] = 0;
    M1.m[0][0] = Tp0.vx;
    M1.m[0][1] = Tp2.vx;
    *(LONG *)&M1.m[0][2] = Tp1.vx;
    M1.m[2][0] = Tp0.vz;
    M1.m[2][1] = Tp2.vz;
    M1.m[2][2] = Tp1.vz;

    MulMatrix0(&M, &M1, &M);
    if ((M.m[0][0] & 0x8000) != (M.m[0][1] & 0x8000) && (M.m[1][0] & 0x8000) != (M.m[1][2] & 0x8000)) {
        return 1;
    }
    return 0;
}

void Set_base_pos(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;
    ENEMY_WORK *pPm;

    pPm = G.pEm;
    get_null_pos(&sv, R_flg, Kan, Seq);
    m = GsIDMATRIX;
    RotMatrixY(pPm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pPm->Base_pos_x = (SHORT)pPm->Pos_x - sv.vx;
    pPm->Base_pos_y = (SHORT)pPm->Pos_y - sv.vy;
    pPm->Base_pos_z = (SHORT)pPm->Pos_z - sv.vz;
    pEm->Base_pos_x = pPm->Base_pos_x;
    pEm->Base_pos_y = pPm->Base_pos_y;
    pEm->Base_pos_z = pPm->Base_pos_z;
}

void Null_pos_set(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;

    get_null_pos(&sv, R_flg, Kan, Seq);
    m = GsIDMATRIX;
    RotMatrixY(pEm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pEm->Pos_x = pEm->Base_pos_x + sv.vx;
    pEm->Pos_y = pEm->Base_pos_y + sv.vy;
    pEm->Pos_z = pEm->Base_pos_z + sv.vz;
}

void get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pEm;
    USHORT *pSeq;
    ULONG f_num;
    ULONG f_base;
    ULONG Rec_sz;
    UCHAR *Bone_adr;

    pEm = G.pEm;
    pSeq = (USHORT *)(Seq + pEm->Move_no * 8);
    f_num = pSeq[0];
    f_base = pSeq[2];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)(Seq + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)(Seq + *pSeq) + pEm->Move_cnt;
    }
    Bone_adr = (UCHAR *)Kan + ((KAN_HEAD *)Kan)->Bone_ofs;
    Rec_sz = ((KAN_HEAD *)Kan)->Rec_sz >> 18;
    while (*pSeq & 0x100) {
        pSeq++;
    }
    Bone_adr += Rec_sz * (f_base + *(UCHAR *)pSeq) * 4;
    pSv->vx = ((SVECTOR *)Bone_adr)->vx;
    pSv->vy = ((SVECTOR *)Bone_adr)->vy;
    pSv->vz = ((SVECTOR *)Bone_adr)->vz;
}

void Null_pos_set2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;

    get_null_pos2(&sv, R_flg, Kan, Seq);
    m = GsIDMATRIX;
    RotMatrixY(pEm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pEm->Pos_x = pEm->Base_pos_x + sv.vx;
    pEm->Pos_y = pEm->Base_pos_y + sv.vy;
    pEm->Pos_z = pEm->Base_pos_z + sv.vz;
}

void get_null_pos2(SVECTOR *pSv, ULONG R_flg, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pEm;
    USHORT *pSeq;
    ULONG f_num;
    ULONG f_base;
    ULONG w;
    ULONG r;
    USHORT rr;
    UCHAR *pP;
    UCHAR *Bone_adr;

    pEm = G.pEm;
    pSeq = (USHORT *)(Seq + pEm->Move_no * 8);
    f_num = pSeq[0];
    f_base = pSeq[2];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)(Seq + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)(Seq + *pSeq) + pEm->Move_cnt;
    }
    w = *pSeq;
    if (w & 0x100) {
        r = w & 0xFF;
        rr = r;
        if (R_flg != 0) {
            if (!(pSeq[1] & 0x100)) {
                func_8002B2BC(Kan, Seq, pSeq + 1, R_flg);
            }
            rr = 0x100 - r;
        } else {
            if (!(pSeq[-1] & 0x100)) {
                func_8002B2BC(Kan, Seq, pSeq - 1, 0);
            }
        }
        pP = (UCHAR *)pEm->pSin_parts_ptr;
        pSv->vx = *(USHORT *)(pP + 0xAC) + ((*(SHORT *)(pP + 0xB4) * rr) >> 8);
        pSv->vy = 0;
        pSv->vz = *(USHORT *)(pP + 0xB0) + ((*(SHORT *)(pP + 0xB2) * rr) >> 8);
    } else {
        Bone_adr = (UCHAR *)Kan + ((KAN_HEAD *)Kan)->Bone_ofs + (((KAN_HEAD *)Kan)->Rec_sz >> 18) * (f_base + (w & 0xFF)) * 4;
        pSv->vx = ((SVECTOR *)Bone_adr)->vx;
        pSv->vy = 0;
        pSv->vz = ((SVECTOR *)Bone_adr)->vz;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", Spd_get);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80018EC8);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019178);

void Em_seq_sound(ENEMY_WORK *pEm)
{
    if (*pEm->pNow_seq & 0xF000) {
        func_800775A0((*pEm->pNow_seq >> 12) - 1, pEm);
    }
}

void func_80019318(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no)
{
    PARTS_W *pP_ptr;
    MATRIX *pM;
    MATRIX Mat;
    SVECTOR Rot;
    LONG x, z;

    if (pEm->Status_flg & 0x80) {
        return;
    }
    pM = (MATRIX *)((UCHAR *)pEm + 0x20);
    pP_ptr = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, pM);
    if (pEm->Be_flg & 0xC0000) {
        Rot.vy = 0;
        Rot.vx = *(SHORT *)((UCHAR *)pEm + 0x1BC);
        Rot.vz = *(SHORT *)((UCHAR *)pEm + 0x1BE);
        RotMatrix_gte(&Rot, &Mat);
        MulMatrix2(&Mat, pM);
    }
    func_8002D4B0(pM, (MATRIX *)pP_ptr->M, &Mat);
    func_8002D4B0(&Mat, (MATRIX *)pP_ptr[8].M, &Mat);
    pP_ptr += Foot_no * 3 + 11;
    func_8002D4B0(&Mat, (MATRIX *)pP_ptr[-2].M, &Mat);
    func_8002D4B0(&Mat, (MATRIX *)pP_ptr[-1].M, &Mat);
    func_8002D4B0(&Mat, (MATRIX *)pP_ptr->M, &Mat);
    if (Mode == 0) {
        pEm->Pos_x = pEm->Pos_x - (Mat.t[0] - pP_ptr->Workm.t[0]);
        pEm->Pos_z = pEm->Pos_z - (Mat.t[2] - pP_ptr->Workm.t[2]);
    } else {
        x = Mat.t[0] - pP_ptr->Workm.t[0];
        z = Mat.t[2] - pP_ptr->Workm.t[2];
        pEm->Spd_x = SquareRoot0(x * x + z * z);
    }
}

void Kage_work_init(void)
{
    KAGE_WORK *pK;
    ULONG i;
    POLY_FT4 *p;

    pK = G.Kage;
    i = 0x32;
    do {
        p = &pK->Prim[0];
        pK->Be_flg = 0;
        *(ULONG *)&p->r0 = 0x808080;
        SetPolyFT4(p);
        SetSemiTrans(p, 1);
        p->tpage = GetTPage(1, 2, 0x240, 0x100);
        p->clut = GetClut(0, 0x1E3);
        p->u0 = 0x62;
        p->v0 = 0xE1;
        p->u1 = 0x7E;
        p->v1 = 0xE1;
        p->u2 = 0x62;
        p->v2 = 0xFF;
        p->u3 = 0x7E;
        p->v3 = 0xFF;
        func_800100A4(&pK->Prim[1], p, 0x28);
        pK++;
        i--;
    } while (i != 0);
}

void Kage_work9_init(void)
{
    KAGE_WORK9 *pK;
    ULONG i;

    pK = G.Kage9;
    i = 5;
    do {
        pK->Be_flg = 0;
        i--;
        pK++;
    } while (i != 0);
}

void Kage_work_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos)
{
    KAGE_WORK *pKw;
    LONG Left_cnt;

    pKw = G.Kage;
    Left_cnt = 0x32;
    while (pKw->Be_flg != 0) {
        Left_cnt--;
        if (Left_cnt == 0) {
            break;
        }
        pKw++;
    }
    if (Left_cnt == 0) {
        *pK = 0;
    } else {
        *pK = (ULONG)pKw;
        pKw->Be_flg = 5;
        pKw->pPos = pPos;
        if (Color != 0) {
            *(ULONG *)&pKw->Prim[0].r0 = (*(ULONG *)&pKw->Prim[0].r0 & 0xFF000000) | Color;
            *(ULONG *)&pKw->Prim[1].r0 = (*(ULONG *)&pKw->Prim[1].r0 & 0xFF000000) | Color;
        } else {
            *(ULONG *)&pKw->Prim[0].r0 = (*(ULONG *)&pKw->Prim[0].r0 & 0xFF000000) | 0x808080;
            *(ULONG *)&pKw->Prim[1].r0 = (*(ULONG *)&pKw->Prim[1].r0 & 0xFF000000) | 0x808080;
        }
        *(ULONG *)&pKw->Off_x = Off;
        *(ULONG *)&pKw->Half_x = Half;
    }
}

#define KAGE9_UV(p, A, B, C, D, E, F, G_, H) \
    (p)->u0 = (A); (p)->v0 = (B); (p)->u1 = (C); (p)->v1 = (D); (p)->u2 = (E); (p)->v2 = (F); (p)->u3 = (G_); (p)->v3 = (H)
#define KAGE9_UV2(w, A, B, C, D, E, F, G_, H) \
    p = &(w)->Prim[0]; KAGE9_UV(p, A, B, C, D, E, F, G_, H); \
    p = &(w)->Prim[1]; KAGE9_UV(p, A, B, C, D, E, F, G_, H); (w)++
#define KAGE9_UV(p, A, B, C, D, E, F, G_, H) \
    (p)->u0 = (A); (p)->v0 = (B); (p)->u1 = (C); (p)->v1 = (D); (p)->u2 = (E); (p)->v2 = (F); (p)->u3 = (G_); (p)->v3 = (H)
#define KAGE9_UV2(w, A, B, C, D, E, F, G_, H) \
    p = &(w)->Prim[0]; KAGE9_UV(p, A, B, C, D, E, F, G_, H); \
    p = &(w)->Prim[1]; KAGE9_UV(p, A, B, C, D, E, F, G_, H); (w)++
#define KAGE9_UV(p, A, B, C, D, E, F, G_, H) \
    (p)->u0 = (A); (p)->v0 = (B); (p)->u1 = (C); (p)->v1 = (D); (p)->u2 = (E); (p)->v2 = (F); (p)->u3 = (G_); (p)->v3 = (H)
#define KAGE9_UV2(w, A, B, C, D, E, F, G_, H) \
    p = &(w)->Prim[0]; KAGE9_UV(p, A, B, C, D, E, F, G_, H); \
    p = &(w)->Prim[1]; KAGE9_UV(p, A, B, C, D, E, F, G_, H); (w)++
void Kage_work9_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos)
{
    KAGE_WORK9 *pK9;
    KAGE_WORK *pKw;
    POLY_FT4 *p;
    LONG i;

    pK9 = G.Kage9;
    i = 5;
    while (pK9->Be_flg != 0) {
        i--;
        if (i == 0) {
            break;
        }
        pK9++;
    }
    if (i == 0) {
        return;
    }
    pK9->Be_flg = 7;
    pKw = G.Kage;
    i = 0x29;
    while (pKw->Be_flg != 0) {
        i--;
        if (i == 0) {
            break;
        }
        pKw++;
    }
    if (i == 0) {
        pK9->Be_flg = 0;
        *pK = 0;
        return;
    }
    *pK = (ULONG)pK9;
    pK9->pKw_top = pKw;
    *(ULONG *)&pK9->Off_x = Off;
    *(ULONG *)&pK9->Half_x = Half;
    pK9->pPos = pPos;
    i = 9;
    do {
        pKw->Be_flg = 2;
        if (Color != 0) {
            *(ULONG *)&pKw->Prim[0].r0 = (*(ULONG *)&pKw->Prim[0].r0 & 0xFF000000) | Color;
            *(ULONG *)&pKw->Prim[1].r0 = (*(ULONG *)&pKw->Prim[1].r0 & 0xFF000000) | Color;
        }
        i--;
        pKw++;
    } while (i != 0);
    pKw -= 9;
    KAGE9_UV2(pKw, 0x62, 0xE1, 0x6B, 0xE1, 0x62, 0xEB, 0x6B, 0xEB);
    KAGE9_UV2(pKw, 0x6B, 0xE1, 0x74, 0xE1, 0x6B, 0xEB, 0x74, 0xEB);
    KAGE9_UV2(pKw, 0x74, 0xE1, 0x7E, 0xE1, 0x74, 0xEB, 0x7E, 0xEB);
    KAGE9_UV2(pKw, 0x62, 0xEB, 0x6B, 0xEB, 0x62, 0xF5, 0x6B, 0xF5);
    KAGE9_UV2(pKw, 0x6B, 0xEB, 0x74, 0xEB, 0x6B, 0xF5, 0x74, 0xF5);
    KAGE9_UV2(pKw, 0x74, 0xEB, 0x7E, 0xEB, 0x74, 0xF5, 0x7E, 0xF5);
    KAGE9_UV2(pKw, 0x62, 0xF5, 0x6B, 0xF5, 0x62, 0xFF, 0x6B, 0xFF);
    KAGE9_UV2(pKw, 0x6B, 0xF5, 0x74, 0xF5, 0x6B, 0xFF, 0x74, 0xFF);
    KAGE9_UV2(pKw, 0x74, 0xF5, 0x7E, 0xF5, 0x74, 0xFF, 0x7E, 0xFF);
}

void func_80019A50(KAGE_WORK *pKw, SHORT Grand, SHORT Dir_y, ULONG Flg)
{
    MATRIX Rot_m;
    SVECTOR Off_sv;
    SVECTOR Rot_sv;

    if (pKw != NULL && pKw->Be_flg != 0) {
        *(int *)&Rot_m.m[0][0] = 0x1000;
        *(int *)&Rot_m.m[0][2] = 0;
        *(int *)&Rot_m.m[1][1] = 0x1000;
        *(int *)&Rot_m.m[2][0] = 0;
        *(int *)&Rot_m.m[2][2] = 0x1000;
        RotMatrixY(Dir_y, &Rot_m);
        *(int *)&Off_sv = pKw->Off_x;
        Off_sv.vz = pKw->Off_z;
        ApplyMatrixSV(&Rot_m, &Off_sv, &Rot_sv);
        pKw->Dir_y = Dir_y;
        pKw->Pos_x = Rot_sv.vx + (SHORT)pKw->pPos->vx;
        pKw->Pos_y = Grand;
        pKw->Pos_z = Rot_sv.vz + (SHORT)pKw->pPos->vz;
        pKw->Be_flg |= 4;
        if (Flg != 0 && !(pKw->Be_flg & 0x80)) {
            pKw->Be_flg &= 0xFB;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019B3C);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_80019DF4);

void Kage_work_color_set(ULONG *pK, ULONG Color)
{
    *(ULONG *)&((KAGE_WORK *)pK)->Prim[0].r0 = (*(ULONG *)&((KAGE_WORK *)pK)->Prim[0].r0 & 0xFF000000) | Color;
    *(ULONG *)&((KAGE_WORK *)pK)->Prim[1].r0 = (*(ULONG *)&((KAGE_WORK *)pK)->Prim[1].r0 & 0xFF000000) | Color;
}

void Kage_work9_color_set(ULONG *pK, ULONG Color)
{
    KAGE_WORK *pPrim;
    UCHAR cnt;

    cnt = 9;
    pPrim = ((KAGE_WORK9 *)pK)->pKw_top;
    do {
        cnt--;
        *(ULONG *)&pPrim->Prim[0].r0 = (*(ULONG *)&pPrim->Prim[0].r0 & 0xFF000000) | Color;
        *(ULONG *)&pPrim->Prim[1].r0 = (*(ULONG *)&pPrim->Prim[1].r0 & 0xFF000000) | Color;
        pPrim++;
    } while (cnt != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001A248);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001A808);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AA3C);

void func_8001AC58(VECTOR *pA, VECTOR *pB, SVECTOR *pRot, SHORT Front_dir_y)
{
    LONG dx, dz, dy, dxz;
    ULONG t;
    LONG Dir;

    dx = pB->vx - pA->vx;
    dz = pB->vz - pA->vz;
    dy = pA->vy - pB->vy;
    dxz = SquareRoot0(dx * dx + dz * dz);
    if (dz != 0) {
        t = dz & 0x80000000;
        t = t == 0;
        pRot->vy = (t << 11) + 0x400;
    }
    if (dx != 0) {
        Dir = 0x1000 - catan((dz << 12) / dx);
        pRot->vy = (Dir - (((ULONG)dx >> 20) & 0x800)) & 0xFFF;
    }
    if (dy != 0) {
        t = dy & 0x80000000;
        t = t == 0;
        pRot->vz = (t << 11) + 0xC00;
    }
    if (dxz != 0) {
        pRot->vz = (0x1000 - catan((dy << 12) / dxz)) & 0xFFF;
    }
}

void func_8001AD78(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg)
{
    USHORT *pSeq;
    ULONG f_num;

    pSeq = (USHORT *)(Seq + pEm->Move_no * 8);
    f_num = pSeq[0];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)(Seq + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)(Seq + *pSeq) + pEm->Move_cnt;
    }
    if (*pSeq & 0x100) {
        if (R_flg != 0) {
            do {
                pSeq++;
            } while (*pSeq & 0x100);
        } else {
            do {
                pSeq--;
            } while (*pSeq & 0x100);
        }
    }
    pEm->pNow_seq = pSeq;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001AE2C);

void func_8001AECC(ENEMY_WORK *pEm, ULONG a1, ULONG a2)
{
    PARTS_W *pParts = pEm->pSin_parts_ptr;
    USHORT y = *(USHORT *)(pEm->pUnk13C + 0x18);

    pEm->Unk136 = (USHORT)pParts[a1 & 0xFFFF].Workm.t[1] - y + (a1 >> 16);
    pEm->Unk134 = (USHORT)pParts[a2 & 0xFFFF].Workm.t[1] - y + (a2 >> 16);
}

ENEMY_WORK *func_8001AF3C(ENEMY_WORK *pSrc)
{
    G_EMTBL *pG = (G_EMTBL *)&G;
    ULONG i = 0;

    while (&pG->pEm_tbl[i] != pG->ppEnemy_end) {
        i++;
    }
    pG->Em_num++;
    pG->pEm_tbl[i] = pG->pEm_mem;
    pG->ppEnemy_end = &pG->pEm_tbl[i + 1];
    pG->pEm_mem++;
    *pG->pEm_tbl[i] = *pSrc;
    pG->pEm_mem = (ENEMY_WORK *)func_800261AC(pG->pEm_tbl[i], PartsWork_set(pG->pEm_tbl[i], (ULONG)pG->pEm_mem),
                                              *(ULONG *)((UCHAR *)pG->pEm_tbl[i] + 0x10C));
    return pG->pEm_tbl[i];
}

ENEMY_WORK *func_8001B048(ULONG Id)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pRet = NULL;
    tagGLOBAL *pG = &G;

    ppEm = pG->pEnemy;
    do {
        if ((*ppEm)->Id == Id) {
            pRet = *ppEm;
        }
        ppEm++;
    } while (ppEm != pG->ppEnemy_end);
    return pRet;
}


void func_8001B08C(void)
{
    ULONG i;
    ULONG *pEnd;
    TBL12 *pT;

    for (i = 0x60; i != 0; ) {
        i--;
        D_800BA8A8[i].Be_flg = 0;
    }
    pT = &D_800BA728[32];
    do {
        pT--;
        pT->Id = 0xFF;
        pT->pData = NULL;
        pT->pData2 = NULL;
    } while (pT > D_800BA728);
    i = func_80012818(0x14, (ULONG *)0x801FC200, 0, "CORE00.ESP");
    pEnd = (ULONG *)0x801FC200;
    pEnd += i >> 2;
    if (i & 3) {
        pEnd++;
    }
    func_8001B204((UCHAR *)0x801FC200, pEnd - 1, 0);
}

LONG func_8001B148(void)
{
    TBL12 *pT;
    LONG i;
    LONG *pData;

    i = 0x60;
    do {
        i--;
        D_800BA8A8[i].Be_flg = 0;
    } while (i != 0);
    pT = (TBL12 *)D_800BA8A8;
    do {
        pT--;
        pT->Id = 0xFF;
        pT->pData = NULL;
        pT->pData2 = NULL;
    } while (pT > (TBL12 *)D_800BA8A8 - 16);
    pData = *(LONG **)((UCHAR *)G.pRoom + 0x4C);
    if (pData != NULL && *pData != -1) {
        func_8001B204((UCHAR *)pData, *(ULONG **)((UCHAR *)G.pRoom + 0x50), 1);
        func_8001B2A4(*(ULONG **)((UCHAR *)G.pRoom + 0x54));
    }
}

ULONG func_8001B204(UCHAR *pData, ULONG *pOfs, ULONG Mode)
{
    TBL12 *pT;
    ULONG i;
    ULONG *p;
    ULONG c;
    ULONG n;

    i = 0;
    if ((UCHAR)Mode != 0) {
        pT = D_800BA7E8;
    } else {
        pT = D_800BA728;
    }
    do {
        c = pData[i];
        pT->Id = c;
        if (c == 0xFF) {
            return (UCHAR)i;
        }
        p = (ULONG *)(pOfs[-i] + (ULONG)pData);
        pT->pData = p;
        n = *p >> 16;
        pT->pData2 = (ULONG *)((UCHAR *)p + ((*p & 0xFFFF) * 2 + n + 2) * 4);
        pT++;
        i++;
    } while (i < 16);
    return (UCHAR)i;
}

void func_8001B2A4(ULONG *pTim)
{
    UCHAR *pTop;
    ULONG n;
    ULONG i;
    ULONG *p;

    pTop = (UCHAR *)pTim;
    n = *pTim++;
    G.Unk2113 = 0;
    for (i = 0; i < n; i++, pTim++) {
        p = (ULONG *)(pTop + *pTim);
        LoadImage((RECT *)p, p + 2);
        if (G.Unk2113 == 0) {
            G.Unk2113 = ((UCHAR *)p)[6];
        }
    }
    DrawSync(0);
}

EFF_WORK *func_8001B35C(EFF_WORK *pParent, ULONG a1, ULONG a2, MATRIX *pM, LONG *pPos, ULONG a5)
{
    SVECTOR sv;
    EFF_WORK *pEw;

    if (pPos != NULL) {
        if (a5 != 0) {
            sv.vx = pPos[0];
            sv.vy = pPos[1];
            sv.vz = pPos[2];
            pEw = func_8001B484(a1, a2, pM, &sv, a5);
        } else {
            sv.vx = pPos[0];
            sv.vy = pPos[1];
            sv.vz = pPos[2];
            pEw = func_8001B484(a1, a2, pM, &sv, 0);
        }
    } else {
        if (a5 != 0) {
            pEw = func_8001B484(a1, a2, pM, NULL, a5);
        } else {
            pEw = func_8001B484(a1, a2, pM, NULL, 0);
        }
    }
    if (pEw != (EFF_WORK *)-1) {
        pEw->pUnkBC = pParent->pUnkBC;
        pEw->Rot_y = 0;
        func_8001BCFC(pEw);
        pEw->Old_x = pEw->Pos_x;
        pEw->Old_y = pEw->Pos_y;
        pEw->Old_z = pEw->Pos_z;
        pEw->Unk54 = pParent->pUnkBC[0x38 / 4];
        if (pEw->Attr & 0x10) {
            func_8001BFCC(pEw);
        }
    }
    return pEw;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B484);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001B894);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BA48);

void func_8001BB24(void)
{
    EFF_WORK *pEw;

    for (pEw = D_80098088 - 1; pEw >= D_80098084; pEw--) {
        if (!(G.Unk2468 & 0x10000000) || *(UCHAR *)&pEw->Id == 0x15 || *(UCHAR *)&pEw->Id == 0x1B) {
            if (pEw->Be_flg & 0x8000) {
                if (pEw->pLink != NULL && pEw->pLink->Be_flg == 0) {
                    pEw->Routine_1 = 0;
                    pEw->Routine_0 = 0;
                    pEw->Be_flg = 0;
                } else {
                    if (pEw->Routine_0 != 0) {
                        D_80097BD4[pEw->Routine_0](pEw);
                    }
                    pEw->Flg2 &= 0xFF00;
                    if (pEw->Be_flg & 2) {
                        func_8001BC80(pEw);
                        func_8001BCFC(pEw);
                        func_8001BFCC(pEw);
                    }
                    if (pEw->Be_flg & 1) {
                        func_8001C168(pEw);
                    }
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BC80);

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001BCFC);

void func_8001BFCC(EFF_WORK *pEw)
{
    LONG y;
    LONG y0;

    if (pEw->Be_flg & 0x80) {
        y0 = func_80051E48(pEw->Pos_x, pEw->Pos_z);
        if (y0 < pEw->Pos_y && y0 != 0) {
            pEw->Flg2 |= 8;
        }
    }
    if (*(ULONG *)&pEw->Be_flg & 0x50) {
        if (pEw->Be_flg & 0x40) {
            y = func_8004D720(&pEw->Pos_x, 2, 0x1000, 0);
            if (y < pEw->Pos_y) {
                if (y < pEw->Unk54) {
                    pEw->Flg2 |= 0x80;
                } else {
                    pEw->Flg2 |= 0x40;
                }
            }
        }
        y = func_8004D720(&pEw->Pos_x, 2, 0x2000, 0);
        if (y < pEw->Pos_y) {
            if (y < pEw->Unk54) {
                pEw->Flg2 |= 0x20;
                if (pEw->Be_flg & 0x20) {
                    pEw->Spd_x -= pEw->Acc_x;
                    pEw->Pos_x = pEw->Old_x;
                    pEw->Rot_x -= pEw->Spd_x;
                }
            } else {
                pEw->Flg2 |= 0x10;
                pEw->Rot_y += y - (USHORT)pEw->Pos_y;
                pEw->Pos_y = y;
            }
        } else {
            pEw->Unk54 = y;
        }
    }
}

void func_8001C168(EFF_WORK *pEw)
{
    SEQ8 *pSeq;

    if (pEw->Timer == 0) {
        pEw->Seq_no++;
        pSeq = &pEw->pSeq[pEw->Seq_no];
        switch (pSeq->Unk2) {
        case 0:
            pEw->Routine_1 = 0;
            pEw->Routine_0 = 0;
            pEw->Be_flg = 0;
            pEw->Flg2 |= 2;
            break;
        case 0xFE:
            pEw->Be_flg &= ~1;
            pEw->Flg2 |= 4;
            pEw->Seq_no--;
            goto end;
        case 0xFF:
            pEw->Seq_no = pSeq->Unk0;
            pSeq = pEw->pSeq;
            pEw->Flg2 |= 1;
            pSeq += pEw->Seq_no;
            break;
        }
        pEw->Timer = pSeq->Unk2;
    }
end:
    pEw->Timer--;
}

UCHAR *func_8001C254(void)
{
    UCHAR *p;

    p = D_800BF754;
    do {
        if (*(USHORT *)(p + 0x24) == 0) {
            goto found;
        }
        p -= 0xD4;
    } while (p > D_800BF754 - 0x4F80);
    p = D_800BF754;
    do {
        if (*(USHORT *)(p + 0x24) & 8) {
            goto found;
        }
        p -= 0xD4;
    } while (p > D_800BF754 - 0x4F80);
    return (UCHAR *)-1;
found:
    func_80010150(p, 0xD4);
    return p;
}

EFF_WORK *func_8001C2DC(USHORT Id)
{
    EFF_WORK *pEw;

    pEw = (EFF_WORK *)D_800BF754;
    if ((Id & 0xFF00) == 0xFF00) {
        do {
            if (pEw->Be_flg != 0 && *(UCHAR *)&pEw->Id == (UCHAR)Id) {
                return pEw;
            }
            pEw--;
        } while (pEw > (EFF_WORK *)(D_800BF754 - 0x4F80));
    } else {
        do {
            if (pEw->Be_flg != 0 && pEw->Id == Id) {
                return pEw;
            }
            pEw--;
        } while (pEw > (EFF_WORK *)(D_800BF754 - 0x4F80));
    }
}

void func_8001C384(void) {
}

void func_8001C38C(EFF_WORK *pEw)
{
    if (pEw->Flg2 & 0x30) {
        if (pEw->Snd_no != 0) {
            func_800746C0(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), (VECTOR *)&pEw->Pos_x, 0, 0);
        }
        func_8001CF24(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
    if (pEw->Flg2 & 0x8) {
        if (pEw->Snd_no != 0) {
            func_800746C0(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), (VECTOR *)&pEw->Pos_x, 0, 0);
        }
        func_8001CF24(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
}

void func_8001C46C(UCHAR *p)
{
    func_8001CDA4(p, p[0x18]);
    p[0] = 0;
}

void func_8001C49C(EFF_WORK *pEw)
{
    switch (pEw->Routine_2) {
    case 0:
        func_8001CDA4(pEw, pEw->Unk18);
        func_8001CE24(pEw, pEw->Unk1A, pEw->Unk1B);
        pEw->Routine_2++;
        break;
    case 1:
        pEw->Unk2E += ((SHORT)pEw->Unk1C * (SHORT)pEw->Unk2E) >> 12;
        pEw->Unk1C -= pEw->Unk1E;
        break;
    }
    if (pEw->Flg2 & 0x30) {
        if (pEw->Snd_no != 0) {
            func_800746C0(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), (VECTOR *)&pEw->Pos_x, 0, 0);
        }
        func_8001CF24(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
    if (pEw->Flg2 & 0x8) {
        pEw->Be_flg = 0;
    }
}

void func_8001C5BC(EFF_WORK *pEw)
{
    VECTOR Unused;
    SVECTOR Rot;
    LONG d;
    EFF_WORK *p;
    ULONG r;

    d = 0;
    if (pEw->Unk03 == 0) {
        func_8001CFF4(pEw, (UCHAR)pEw->Unk1C);
        pEw->Unk03 = 1;
    }
    if (pEw->Routine_1 == 2) {
        if (pEw->Unk20 == 0) {
            pEw->UnkB6++;
            d = (pEw->UnkB6 * pEw->Unk22 * pEw->Unk2E) >> 12;
            if ((ULONG)(pEw->Unk2E - d) <= 0x300) {
                pEw->Be_flg = 0;
                return;
            }
        } else {
            pEw->Unk20--;
        }
    }
    if (pEw->UnkB4 == 0) {
        if (pEw->Unk1E == 0 || D_80098728[func_800102E8()] % pEw->Unk1E == 0) {
            if (pEw->Routine_1 == 3) {
                p = func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                  ((pEw->Unk2E + D_80098728[func_800102E8()] * 4 - d) << 16) |
                                      (USHORT)(pEw->Unk2C + ((signed char)func_800102E8() << pEw->Unk22)),
                                  (MATRIX *)pEw->Key, &pEw->Unk38, 0);
            } else {
                Rot.vx = pEw->Unk68;
                Rot.vy = pEw->Unk6A;
                Rot.vz = pEw->Unk6C;
                r = func_800102E8();
                p = func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                  ((pEw->Unk2E + D_80098728[r] * 4 - d) << 16) |
                                      (USHORT)(pEw->Unk2C + ((D_80098728[func_800102E8()] - 0x80) >> pEw->Unk1B)),
                                  (MATRIX *)pEw->Key, &pEw->Unk38, (ULONG)&Rot);
            }
            if (pEw->Routine_1 == 5) {
                func_800746C0((pEw->Unk22 + (func_800102E8() & 1)) | 0x10200, (VECTOR *)&pEw->Pos_x, 0, 0);
            }
            if (pEw->Routine_1 == 6) {
                func_800746C0(pEw->Unk22 | 0x10200, (VECTOR *)&pEw->Pos_x, 0, 0);
            }
            if (pEw->Routine_1 == 1) {
                if (--pEw->Unk22 == 0) {
                    pEw->Be_flg = 0;
                    return;
                }
            }
            if (pEw->Routine_1 == 4 && p != (EFF_WORK *)-1) {
                p->Spd_x += D_80098728[func_800102E8()] % pEw->Unk22;
                p->Spd_y += D_80098728[func_800102E8()] % pEw->Unk22;
            }
            if (p != (EFF_WORK *)-1) {
                p->pUnkBC = pEw->pUnkBC;
            }
            if (!(pEw->Flg2 & 0x100) && p != (EFF_WORK *)-1) {
                p->Flg2 |= 0x100;
                pEw->Flg2 |= 0x100;
            }
            pEw->UnkB4 = pEw->Unk1A;
        }
    } else {
        pEw->UnkB4--;
    }
}

void func_8001C98C(EFF_WORK *pEw)
{
    VECTOR Pos;

    if (pEw->Routine_1 == 1 && pEw->UnkB4 == 0) {
        pEw->UnkB4 = pEw->Unk2E;
        pEw->Unk2E = pEw->Unk20;
    }
    if (pEw->Unk1C != 0) {
        pEw->Unk1C--;
    } else {
        switch (pEw->Routine_1) {
        case 0:
            pEw->Unk2E -= (pEw->Unk1E * pEw->Unk2E) >> 12;
            if (pEw->Unk20 >= pEw->Unk2E) {
                pEw->Be_flg = 0;
            }
            break;
        case 1:
            pEw->Unk2E += (pEw->Unk1E * pEw->Unk2E) >> 12;
            if (pEw->Unk2E >= pEw->UnkB4) {
                pEw->Routine_0 = 0;
                pEw->Unk2E = pEw->UnkB4;
            }
            break;
        case 2:
            break;
        }
    }
    if (pEw->Flg2 & 0x30) {
        pEw->Be_flg = 0;
    }
    if (pEw->Flg2 & 8) {
        Pos.vx = pEw->Pos_x;
        Pos.vy = pEw->Pos_y;
        Pos.vz = pEw->Pos_z;
        func_8001B35C(pEw, 0x1503, (pEw->Unk2E << 16) | pEw->Unk2C, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
    }
}

void func_8001CB04(EFF_WORK *pEw)
{
    HIT_REQ Req;

    if (pEw->Unk18 != 0xFF) {
        if (pEw->UnkB8 == 0) {
            Req.pPos = &pEw->Pos_x;
            Req.W = pEw->Unk1C;
            Req.H = pEw->Unk1E;
            Req.D = pEw->Unk20;
            Req.R = pEw->Unk2C;
            if (func_800472EC(&Req, D_80097FCC[pEw->Unk18]) < 0) {
                pEw->UnkB8 = 30;
                if (pEw->Unk1A != 0xFF) {
                    Flg_on(D_800D1FC0, pEw->Unk1A);
                }
            }
        } else {
            pEw->UnkB8--;
        }
    }
    if (pEw->Snd_no != 0xFF) {
        Req.pPos = &pEw->Pos_x;
        Req.W = pEw->Unk1C;
        Req.H = pEw->Unk1E;
        Req.D = pEw->Unk20;
        Req.R = pEw->Unk2C;
        if (func_800472EC(&Req, D_80097FCC[pEw->Snd_no]) & 0x20000000) {
            if (pEw->Unk1B != 0xFF) {
                Flg_on(D_800D1FC0, pEw->Unk1B);
            }
        }
    }
}

void func_8001CC60(EFF_WORK *pEw)
{
    ULONG a1;
    ULONG a3;

    if (pEw->Flg2 & 0x30) {
        pEw->Unk20 = 0;
    }
    if (pEw->Unk20 == 0) {
        if (pEw->Unk18 != 0xFF) {
            if (pEw->Unk1A != 0xFF) {
                a1 = pEw->Unk1A;
            } else {
                a1 = pEw->UnkB3;
            }
            if (pEw->Unk1C != 0xFFFF) {
                a3 = pEw->Unk1C;
            } else {
                a3 = pEw->Unk2E;
            }
            func_8001B35C(pEw, ((a1 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                          (a3 << 16) | (USHORT)(pEw->Unk2C + pEw->Unk1E), &GsIDMATRIX, &pEw->Pos_x, 0);
        }
        switch (pEw->Routine_1) {
        case 0:
            pEw->Routine_0 = 0;
            break;
        case 1:
            pEw->Be_flg = 0;
            break;
        }
    } else {
        pEw->Unk20--;
    }
}

void func_8001CD64(void) {
}

void func_8001CD6C(void) {
}

void func_8001CD74(void) {
}

void func_8001CD7C(void) {
}

void func_8001CD84(void) {
}

void func_8001CD8C(void) {
}

void func_8001CD94(void) {
}

void func_8001CD9C(void) {
}

void func_8001CDA4(UCHAR *p, ULONG n)
{
    SEQ8 *pSeq;

    n &= 0xFF;
    if (n != 0) {
        p[0x6F] = p[0xF] + D_80098728[func_800102E8()] % n;
        pSeq = *(SEQ8 **)(p + 0xC4);
        pSeq += p[0x6F];
        p[0x2B] = pSeq->Unk2;
    }
}

void func_8001CE24(EFF_WORK *pEw, ULONG a1, ULONG a2)
{
    pEw->Acc_x = (pEw->Acc_x * pEw->Unk2E) >> 12;
    pEw->Acc_z = (pEw->Acc_z * pEw->Unk2E) >> 12;
    if ((UCHAR)a1 != 0) {
        pEw->Spd_x = ((pEw->Spd_x + (signed char)func_800102E8() % (UCHAR)a1) * pEw->Unk2E) >> 12;
    }
    if ((UCHAR)a2 != 0) {
        pEw->Spd_y = ((pEw->Spd_y + (signed char)func_800102E8() % (UCHAR)a2) * pEw->Unk2E) >> 12;
    }
    pEw->Spd_z = (pEw->Spd_z * pEw->Unk2E) >> 12;
}

void func_8001CF24(EFF_WORK *pEw, ULONG a1, ULONG a2)
{
    switch (pEw->Routine_1) {
    case 2:
        func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | ((a2 & 0xFF) << 8) | (a1 & 0xFF),
                      (pEw->Unk2E << 16) | pEw->Unk2C, &GsIDMATRIX, &pEw->Pos_x, 0);
    case 0:
        pEw->Be_flg = 0;
        pEw->Routine_0 = 0;
        break;
    case 3:
        pEw->Routine_0 = 0;
        pEw->Be_flg = (pEw->Be_flg & 0xFFED) | 1;
        break;
    }
}

void func_8001CFF4(EFF_WORK *pEw, ULONG n)
{
    EFF_WORK *p;
    ULONG i;
    ULONG r;

    for (i = 0; i < n; i++) {
        if (pEw->UnkB4 == 0) {
            pEw->UnkB4 = pEw->Unk1A;
            r = func_800102E8();
            p = func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                 ((pEw->Unk2E + D_80098728[r] * 4) << 16) |
                                     (USHORT)(pEw->Unk2C + ((D_80098728[func_800102E8()] - 0x80) >> pEw->Unk1B)),
                                 (MATRIX *)pEw->Key, (LONG *)((UCHAR *)pEw + 0x38), 0);
        } else {
            pEw->UnkB4--;
        }
        if (p != (EFF_WORK *)-1) {
            p->UnkB6 = 1;
        }
        for (p = D_80098090 - 1; p >= D_8009808C; p--) {
            if (p->UnkB6 != 0 && p->Id == *(USHORT *)&pEw->Unk18) {
                D_80097BD4[p->Routine_0](p);
                func_8001BC80(p);
                func_8001BCFC(p);
                func_8001C168(p);
            }
        }
    }
    for (p = D_80098090 - 1; p >= D_8009808C; p--) {
        if (p->Id == *(USHORT *)&pEw->Unk18) {
            p->UnkB6 = 0;
        }
    }
    pEw->UnkB4 = 0;
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001D210);

void func_8001D480(UCHAR *p)
{
    if (p[1] == 0) {
        *(SHORT *)(p + 8) += (signed char)D_80098728[func_800102E8()] * p[0x18];
        *(SHORT *)(p + 0xA) += (signed char)D_80098728[func_800102E8()] * p[0x19];
        p[1]++;
    }
    if (*(USHORT *)(p + 0x26) & 0x30) {
        *(USHORT *)(p + 0x24) = 0;
    }
}

void func_8001D534(EFF_WORK *pEw)
{
    VECTOR Pos;
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    if (pEw->Routine_1 == 0) {
        if (pEw->Unk1A != 0) {
            pEw->Spd_x = ((pEw->Spd_x + (signed char)func_800102E8() % pEw->Unk1A) * pEw->Unk2E) >> 12;
        }
        if (pEw->Unk1B != 0) {
            pEw->Spd_y = ((pEw->Spd_y + (signed char)func_800102E8() % pEw->Unk1B) * pEw->Unk2E) >> 12;
        }
        pEw->Routine_1++;
        pEw->Spd_z = (pEw->Spd_z * pEw->Unk2E) >> 12;
    }
    if (!(pEw->UnkBA & 1)) {
        Req.pPos = &pEw->Pos_x;
        Req.W = 0;
        Req.H = 0;
        Req.D = 0;
        Req.R = pEw->Unk2C;
        Hit |= func_800472EC(&Req, D_80097FCC[30]);
        if (Hit) {
            pEw->Be_flg = 0;
        }
    }
    if (pEw->Flg2 & 8) {
        Pos.vx = pEw->Pos_x;
        Pos.vy = pEw->Pos_y;
        Pos.vz = pEw->Pos_z;
        func_8001B35C(pEw, 0x1503, (pEw->Unk2E << 16) | pEw->Unk2C, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
        return;
    }
    if (!(pEw->UnkBA & 1)) {
        func_8001B35C(pEw, 0x3003603,
                      (((pEw->Unk2E >> 1) - (pEw->Unk2E >> 2) + ((signed char)func_800102E8() << 3)) << 16) | pEw->Unk2C,
                      &GsIDMATRIX, &pEw->Pos_x, 0);
    }
    if (pEw->Flg2 & 0x30) {
        if (pEw->Unk22 == 0) {
            Pos.vx = pEw->Pos_x;
            Pos.vy = pEw->Pos_y + 400;
            Pos.vz = pEw->Pos_z;
            func_8001B35C(pEw, 0x205, (pEw->Unk2E << 16) | pEw->Unk2C, &GsIDMATRIX, &Pos.vx, 0);
        }
        pEw->Be_flg = 0;
    }
    pEw->UnkBA++;
}

void func_8001D7C0(void) {
}

void func_8001D7C8(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001D7D0);

LONG func_80044804(MATRIX *pM, ULONG a1, ULONG a2);
LONG func_80047860(MATRIX *pM, ULONG a1, ULONG a2);
extern ULONG D_8009DBB4;

void func_8001E0F4(EFF_WORK *pEw)
{
    VECTOR Spd;
    SVECTOR Rot;
    VECTOR Pos;
    LONG Hit;
    EFF_WORK *p;

    Hit = 0;
    switch (pEw->Routine_2) {
    case 0:
        if (pEw->Unk2C != 0) {
            pEw->UnkBA = pEw->Unk2C;
            pEw->Unk2C = 0;
        }
        if (pEw->Routine_1 == 0) {
            Rot.vx = 0;
            Rot.vy = 0;
            Rot.vz = 0x400;
            func_8001B35C(pEw, 0x3000D02, 0x18000000, &pEw->Workm2, NULL, (ULONG)&Rot);
            Rot.vx = 0;
            Rot.vy = 0x400;
            Rot.vz = 0x400;
            func_8001B35C(pEw, 0x3000D02, 0x18000000, &pEw->Workm2, NULL, (ULONG)&Rot);
        }
        pEw->Routine_2++;
    case 1:
        if (pEw->Pos_x >= 0x7918) {
            Hit |= 1;
        }
        if (pEw->Pos_x < -0x7917) {
            Hit |= 1;
        }
        if (pEw->Pos_z >= 0x7918) {
            Hit |= 1;
        }
        if (pEw->Pos_z < -0x7917) {
            Hit |= 1;
        }
        if (pEw->UnkB8 != 0) {
            if (pEw->UnkB8 == 10) {
                pEw->Be_flg = 0x8000;
                func_8001B894(0xD02, (ULONG)&pEw->Workm2);
            }
            if (--pEw->UnkB8 == 1) {
                pEw->Be_flg = 0;
            }
            break;
        }
        if (pEw->Rot_y >= 30000) {
            p = func_8001B484(((pEw->UnkB3 & 0xF) << 24) | (((UCHAR *)&pEw->Id)[1] << 8) | ((UCHAR *)&pEw->Id)[0],
                              (pEw->Unk2E << 16) | pEw->Unk2C, &pEw->Workm2, NULL, 0);
            func_8001BC80(p);
            func_8001BCFC(p);
            pEw->UnkB8 = 10;
        }
        if (pEw->Flg2 & 0x30) {
            Hit |= 1;
        }
        Pos.vx = (pEw->Pos_x + pEw->Old_x) / 2;
        Pos.vy = (pEw->Pos_y + pEw->Old_y) / 2;
        Pos.vz = (pEw->Pos_z + pEw->Old_z) / 2;
        Hit |= func_80044804(&pEw->Workm2, D_8009DBB4, 10);
        Hit |= func_80047860(&pEw->Workm2, D_8009DBB4, 10);
        Spd.vx = (signed char)D_80098728[func_800102E8()] >> 1;
        Spd.vy = 500;
        Spd.vz = (signed char)D_80098728[func_800102E8()] >> 1;
        func_8001B35C(pEw, 0xC03, 0x18000000, &pEw->Workm2, &Spd.vx, 0);
        if (Hit) {
            func_8001B894(0xD02, (ULONG)&pEw->Workm2);
            func_800746C0(0x1010A, (VECTOR *)&pEw->Pos_x, 0, 0);
            func_8001B35C(pEw, 0x203, (USHORT)pEw->UnkBA | 0xD000000, &GsIDMATRIX, &Pos.vx, 0);
            pEw->Be_flg = 0;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/80011CF8", func_8001E444);

void func_8001F208(EFF_WORK *pEw)
{
    VECTOR Pos;
    SVECTOR Rot;

    switch (pEw->Routine_1) {
    case 0:
        pEw->Routine_1++;
    case 1:
        Pos.vx = *(LONG *)((UCHAR *)pEw + 0x38);
        Pos.vy = *(LONG *)((UCHAR *)pEw + 0x3C) - 100;
        Pos.vz = *(LONG *)((UCHAR *)pEw + 0x40);
        Rot.vx = 0;
        Rot.vy = 0;
        Rot.vz = 0;
        func_8001B35C(pEw, 0x805, ((pEw->Unk2E >> 1) << 16) | pEw->Unk2C, (MATRIX *)pEw->Key, &Pos.vx, (ULONG)&Rot);
        Rot.vx = 0;
        Rot.vy = 0x400;
        Rot.vz = 0;
        func_8001B35C(pEw, 0x805, ((pEw->Unk2E >> 1) << 16) | pEw->Unk2C, (MATRIX *)pEw->Key, &Pos.vx, (ULONG)&Rot);
        pEw->Routine_1++;
        break;
    default:
        Pos.vx = (signed char)D_80098728[func_800102E8()] >> 1;
        Pos.vy = -150;
        Pos.vz = (signed char)D_80098728[func_800102E8()] >> 1;
        func_8001B35C(pEw, 0xC03, ((pEw->Unk2E >> 1) << 16) | pEw->Unk2C, (MATRIX *)pEw->Key, &Pos.vx, 0);
        break;
    }
}

void func_8001F378(EFF_WORK *pEw)
{
    VECTOR Pos;
    VECTOR Dummy;

    switch (pEw->Routine_1) {
    case 0:
        func_8001B35C(pEw, 0x3000903, (pEw->Unk2E << 18) | pEw->Unk2C, &GsIDMATRIX, &pEw->Pos_x, 0);
        func_8001B35C(pEw, 0x3000A03, ((pEw->Unk2E * 5) << 16) | pEw->Unk2C, &GsIDMATRIX, &pEw->Pos_x, 0);
        break;
    case 2:
        Pos.vx = pEw->Pos_x;
        Pos.vy = pEw->Pos_y;
        Pos.vz = pEw->Pos_z;
        func_8001B35C(pEw, 0x1000803, ((((ULONG)pEw->Unk2E * 17) >> 2) << 16) | pEw->Unk2C, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
        return;
    }
    pEw->Routine_1++;
}

void func_8001F4A8(UCHAR *p)
{
    if (p[1] == 1) {
        *(SHORT *)(p + 8) += (signed char)p[2] << 4;
        *(SHORT *)(p + 0xA) += (signed char)p[3] << 4;
    }
}

void func_8001F4F4(EFF_WORK *pEw)
{
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    switch (pEw->Routine_2) {
    case 0:
        func_8001CDA4(pEw, pEw->Unk18);
        func_8001CE24(pEw, pEw->Unk1A, pEw->Unk1B);
        pEw->Routine_2++;
        break;
    case 1:
        pEw->Unk2E += ((SHORT)pEw->Unk1C * (SHORT)pEw->Unk2E) >> 12;
        pEw->Unk1C -= pEw->Unk1E;
        break;
    }
    if (pEw->Flg2 & 0x30) {
        pEw->Routine_0 = 0;
        pEw->Be_flg = (pEw->Be_flg & 0xFFED) | 1;
    }
    if (pEw->UnkBA != 0) {
        if (++pEw->UnkB8 >= 21) {
            pEw->UnkBA = 0;
        }
        if (pEw->UnkB8 >= 16) {
            Req.pPos = &pEw->Pos_x;
            Req.W = 0;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEw->Unk2C;
            Hit |= func_800472EC(&Req, D_80097FCC[19]);
            Hit |= func_80047BD0(&Req, D_80097FCC[19]);
            if (Hit) {
                pEw->Be_flg = 0;
            }
        }
    }
}

void func_8001F674(EFF_WORK *pEw)
{
    SHORT Ofs[3] = { -200, 0, 200 };
    SHORT Spd[3] = { -2, 0, -2 };
    UCHAR i;
    EFF_WORK *p;

    if (++pEw->UnkB4 & 1) {
        for (i = 0; i < 3; i++) {
            p = func_8001B35C(pEw, 0x2103,
                              ((0x2800 - pEw->UnkB4 * 1000) << 16) |
                                  (USHORT)(pEw->Unk2C + (signed char)D_80098728[func_800102E8()] + Ofs[i]),
                              &GsIDMATRIX, &pEw->Old_x, 0);
            if (p != (EFF_WORK *)-1) {
                p->Spd_x -= pEw->UnkB4 * 10;
                p->Spd_y = Spd[i];
                if (pEw->UnkBA == 0) {
                    p->UnkBA = 1;
                }
            }
        }
        if (pEw->UnkB6 != 0) {
            pEw->Be_flg = 0;
        }
    }
    if (pEw->UnkB4 >= 5) {
        pEw->Be_flg = 0;
    }
}

/* Spd[] above is the last .rodata of this file: aspsx padded the section to a word */
__asm__(".section .rodata\n.align 2\n.section .text");

void func_8001F824(EFF_WORK *pEw)
{
    VECTOR Unused;
    VECTOR Pos;
    EFF_WORK *p;

    switch (pEw->Routine_1) {
    case 0:
        func_8001CDA4((UCHAR *)pEw, pEw->Unk18);
        func_8001CE24(pEw, pEw->Unk1A, 0);
        pEw->Routine_1++;
        break;
    case 1:
        pEw->Unk2E += ((SHORT)pEw->Unk1C * (SHORT)pEw->Unk2E) >> 12;
        pEw->Unk1C -= pEw->Unk1E;
        break;
    }
    if (pEw->Flg2 & 0x30) {
        pEw->Be_flg &= 0xFFED;
    }
    if ((pEw->Flg2 & 0x20) && pEw->UnkB4 < 8) {
        Pos.vx = pEw->Old_x;
        Pos.vy = pEw->Old_y;
        Pos.vz = pEw->Old_z;
        p = func_8001B35C(pEw, 0x505, ((pEw->Unk2E - ((signed char)func_800102E8() << 4)) << 16) | pEw->Unk2C,
                          &GsIDMATRIX, &Pos.vx, 0);
        if (p != (EFF_WORK *)-1) {
            p->UnkBA = 1;
            if (!(pEw->Flg2 & 0x80)) {
                p->UnkBA = 0x81;
            }
        }
        pEw->UnkB4 = 8;
    } else {
        if (pEw->UnkB4 == 1 || pEw->UnkB4 == 7) {
            Pos.vx = pEw->Old_x;
            Pos.vy = pEw->Old_y;
            Pos.vz = pEw->Old_z;
            p = func_8001B35C(pEw, 0x405, ((pEw->Unk2E - ((signed char)func_800102E8() << 4)) << 16) | pEw->Unk2C,
                              &GsIDMATRIX, &Pos.vx, 0);
            if (p != (EFF_WORK *)-1) {
                p->Spd_x = (D_80098728[func_800102E8()] & 0x1F) + 0x50;
                if (pEw->UnkB4 == 7) {
                    p->UnkBA = 1;
                }
            }
        }
        pEw->UnkB4++;
    }
}

void func_8001FA90(EFF_WORK *pEw)
{
    VECTOR Pos;
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    switch (pEw->Routine_1) {
    case 0:
        if (pEw->Unk18 == 0) {
            pEw->UnkB4 = (D_80098728[func_800102E8()] & 0xF) | 0x10;
            pEw->UnkB6 = (((D_80098728[func_800102E8()] & 0xF) + 0x10) * pEw->Unk2E) >> 11;
        } else {
            pEw->UnkB4 = (D_80098728[func_800102E8()] & 0xF) + 0x18;
            pEw->UnkB6 = (((D_80098728[func_800102E8()] & 0xF) + 0x18) * pEw->Unk2E) >> 11;
            if (!(pEw->UnkBA & 0x80)) {
                pEw->Be_flg &= 0xFFFD;
            }
        }
        pEw->UnkB8 = 0;
        pEw->Routine_1++;
    case 1:
        pEw->Unk0A -= pEw->UnkB4;
        pEw->Unk2E -= pEw->UnkB6;
        if (pEw->Unk18 == 0) {
            if (pEw->UnkBA != 0) {
                if (++pEw->UnkB8 >= 21) {
                    pEw->UnkBA = 0;
                }
                if (pEw->UnkB8 >= 16) {
                    Req.pPos = &pEw->Pos_x;
                    Req.W = 0;
                    Req.H = 0;
                    Req.D = 0;
                    Req.R = pEw->Unk2C;
                    Hit |= func_800472EC(&Req, D_80097FCC[17]);
                    Hit |= func_80047BD0(&Req, D_80097FCC[17]);
                    if (Hit) {
                        pEw->UnkBA = 0;
                        pEw->Unk0A += pEw->UnkB4 << 5;
                        pEw->Unk2E += pEw->UnkB6 << 5;
                    }
                }
            }
        } else if (pEw->Unk2E <= 0x2000) {
            pEw->Be_flg |= 2;
        }
        if ((USHORT)pEw->Unk0A <= 0x500 || pEw->Unk2E <= 0x300) {
            pEw->Be_flg = 0;
        }
        break;
    }
    if (!(pEw->Flg2 & 8)) {
        if (pEw->Spd_x < 0 || (pEw->Flg2 & 0x20)) {
            if (pEw->Acc_x == 0) {
                pEw->Spd_y = 0;
                pEw->Acc_y = 0;
            }
            pEw->Spd_x = 0;
            pEw->Acc_x = 0;
        }
        if (!(pEw->Flg2 & 0x10)) {
            return;
        }
        pEw->Spd_y = 0;
        if (pEw->Flg2 & 0x40) {
            return;
        }
    }
    Pos.vx = pEw->Pos_x;
    Pos.vy = pEw->Pos_y;
    Pos.vz = pEw->Pos_z;
    func_8001B35C(pEw, 0x1503, ((pEw->Unk2E >> 2) << 16) | pEw->Unk2C, (MATRIX *)pEw->Key, &Pos.vx, 0);
    pEw->Be_flg = 0;
}

void func_8001FDD0(EFF_WORK *pEw)
{
    HIT_REQ Req;
    VECTOR Pos;
    EFF_WORK *pNew;
    ULONG i;

    for (i = 0; i < 2; i++) {
        pNew = func_8001B35C(pEw, 0x3000F00, (pEw->Unk2E << 16) | (USHORT)(pEw->Unk2C + (signed char)func_800102E8() * 4),
                             (MATRIX *)pEw->Key, (LONG *)((UCHAR *)pEw + 0x38), 0);
        if (pNew != (EFF_WORK *)-1) {
            pNew->Spd_x = (D_80098728[func_800102E8()] & 0x3F) + 0x82;
            pNew->Spd_y = (D_80098728[func_800102E8()] & 0x7F) - 100;
        }
    }
    Req.pPos = &pEw->Pos_x;
    Req.W = 0;
    Req.H = 0;
    Req.D = 0;
    Req.R = pEw->Unk2C;
    func_800472EC(&Req, D_80097FCC[18]);
    func_80047BD0(&Req, D_80097FCC[18]);
    if (++pEw->UnkB4 >= 3) {
        if (pEw->UnkB6 != 0) {
            Pos.vx = pEw->Pos_x;
            Pos.vy = pEw->Pos_y;
            Pos.vz = pEw->Pos_z;
            func_8001B35C(pEw, 0x2000E03, pEw->Unk2C | 0x20000000, &GsIDMATRIX, &Pos.vx, 0);
        }
        pEw->Be_flg = 0;
    }
}

void func_8001FF6C(EFF_WORK *pEw)
{
    VECTOR Pos;

    switch (pEw->Routine_1) {
    case 0:
        func_8001B35C(pEw, 0x108, pEw->Unk2C | 0x10000000, (MATRIX *)pEw->Key, &pEw->Unk38, 0);
        pEw->Routine_1++;
        break;
    case 1:
        func_8001B35C(pEw, 9, (USHORT)(pEw->Unk2C + (signed char)D_80098728[func_800102E8()] * 7 + 0x800) | 0x9000000,
                      (MATRIX *)pEw->Key, &pEw->Unk38, 0);
        func_8001B35C(pEw, 9, (USHORT)(pEw->Unk2C + (signed char)D_80098728[func_800102E8()] * 7 + 0x800) | 0xB000000,
                      (MATRIX *)pEw->Key, &pEw->Unk38, 0);
        func_8001B35C(pEw, 0x109, (USHORT)(pEw->Unk2C + (signed char)D_80098728[func_800102E8()] * 4 + 0x800) | 0xA000000,
                      (MATRIX *)pEw->Key, &pEw->Unk38, 0);
        func_8001B35C(pEw, 0x209, (USHORT)(pEw->Unk2C + (signed char)D_80098728[func_800102E8()] * 2 + 0x800) | 0x7000000,
                      (MATRIX *)pEw->Key, &pEw->Unk38, 0);
        func_8001B35C(pEw, 0x309, (USHORT)(pEw->Unk2C + (signed char)D_80098728[func_800102E8()] * 6 + 0x800) | 0x8000000,
                      (MATRIX *)pEw->Key, &pEw->Unk38, 0);
        func_8001B35C(pEw, 9, (USHORT)(pEw->Unk2C + (signed char)D_80098728[func_800102E8()] * 7) | 0xB000000,
                      (MATRIX *)pEw->Key, &pEw->Unk38, 0);
        Pos.vx = pEw->Unk38;
        Pos.vy = pEw->Unk3C + 200;
        Pos.vz = pEw->Unk40;
        func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | 0x300, 0x10000000, (MATRIX *)pEw->Key, &Pos.vx, 0);
        pEw->Routine_1++;
        break;
    case 2:
        if (++pEw->UnkB4 >= 9) {
            pEw->Routine_1++;
        }
        break;
    case 3:
        Pos.vx = pEw->Unk38;
        Pos.vy = pEw->Unk3C + 200;
        Pos.vz = pEw->Unk40;
        func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | 0x200, 0x10000000, (MATRIX *)pEw->Key, &Pos.vx, 0);
        pEw->Be_flg = 0;
        break;
    }
}
