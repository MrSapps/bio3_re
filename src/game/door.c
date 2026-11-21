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

/* declarations from earlier parts of the original address-split file */
extern USHORT D_800970DC[];
extern USHORT D_800970E8[];
extern SHORT D_800E0DD8;

extern LONG D_800A0FEC[];
void func_800783BC(ULONG no, ULONG size, ULONG a2);
void func_800782F4(ULONG no, ULONG id, ULONG *pAdr);

/* RE3 door model work (pDwork[] slot, 0xC8 bytes); 0x10.. is laid out like MODEL_WORK */
typedef struct {
    ULONG Be_flg;                      /* 0x00 */
    UCHAR pad04[0x10 - 0x4];
    LONG Attribute;                    /* 0x10 */
    ULONG *pTmd;                       /* 0x14 */
    POLY_GT3 *pPacket;                 /* 0x18 two buffers per door, index G.Cbuf_id */
    UCHAR pad1C[0x20 - 0x1C];
    SHORT M[3][3];                     /* 0x20 rotation part of a MATRIX whose t[] is Pos_x..Pos_z */
    UCHAR pad32[0x34 - 0x32];
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
    UCHAR pad72[0x74 - 0x72];
    ULONG Poly_rgb;                    /* 0x74 */
    MATRIX *pSuper;                    /* 0x78 */
    UCHAR pad7C[0xC0 - 0x7C];
    USHORT Attribute2;                 /* 0xC0 */
    UCHAR Model_no;                    /* 0xC2 */
    UCHAR Type;                        /* 0xC3 */
    USHORT Free;                       /* 0xC4 brightness ramp counter (Attribute2 & 0x1000) */
    USHORT Free2;                      /* 0xC6 current brightness */
} DOOR_WORK;

void Door_main(void)
{
    UCHAR i;

    for (i = 0; Door_proc_tbl[i] != NULL; i++) {
        Door_proc_tbl[i]();
    }
}

INCLUDE_ASM("asm/nonmatchings/game/door", Door_init);

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
            pG->DSce_work[1] = 0;
        }
        Door_scheduler_main();
        Door_Trans();
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
    Sce.pCScd = Door_data->Scd;
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

INCLUDE_ASM("asm/nonmatchings/game/door", Door_Trans);

INCLUDE_ASM("asm/nonmatchings/game/door", DoorSort);

void Door_exit(void)
{
Door_exit_wait:
    if (Fade_status(0) != 0) {
        Task_sleep(1);
        goto Door_exit_wait;
    }
    Fade_set(0, 1, 2, 2, 0, 0xFFFFFF, 0x10);
    if (*(USHORT *)((UCHAR *)D_800BA6F8 + 0x240) != 0) {
        Snd_se_stad(0x401, NULL, 0, 0);
    }
    Main.Vloop = 2;
    G.System_flg &= ~0x400000;
    Task_exit();
}

void Door_texture_load(void)
{
    ULONG Sector;
    ULONG File_no;
    DOOR_INFO *pDoor_info;
    UCHAR Door_no;
    tagCD *pCd;

    pCd = &CD;
    Door_no = ((UCHAR *)G.pSce_aot_on)[0xC];
    pDoor_info = &D_800971E4[Door_no];
    File_no = D_800979D4[G.Unk783E][Door_no];
    Sector = floc[File_no].sector + floc[File_no].sec_high * 0x10000 + pDoor_info->Sector_offset;
    pCd->Fsum = pDoor_info->Data_sum;
    pCd->Fsize = pDoor_info->Data_size;
    G.Status_flg |= 0x800;
    pCd->Fsector = Sector;
    Cd_read(File_no, (ULONG *)0x8019D000, 3, "DOOR TEXTURE");
}

void Door_model_init(UCHAR *pDw)
{
    ULONG *pDoor_cur;
    ULONG *Tmd_adr;

    pDoor_cur = D_800BA6F8;
    Tmd_adr = (ULONG *)D_800BA6F8[1];
    *(ULONG *)(pDw + 0x74) = 0x808080;
    *(ULONG **)(pDw + 0x14) = Tmd_adr + pDw[0xC2] * 6;
    *D_800BA6F8 = PresetObjectGT3_neo(pDw + 0x10, pDoor_cur[0]);
}

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
    Cd_read(File_no, Address, 3, "DOOR SOUND");
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
        Root_ck(pEm, &v, 0, 0);
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
        Foot_set_pl(pEm, 0, 0);
    }
    if (pEm->Unk144 & 2) {
        Foot_set_pl(pEm, 0, 1);
    }
    if (pEm->Unk120 & 0x80) {
        Rot_neck(pEm, pEm->Cdir_y);
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
            Flg_on(D_800D1FC0, pEm->Unk146);
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
            Flg_on(D_800D1FC0, pEm->Unk146);
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
            Joint_move(pEm, (LONG)pEm->pSub0_kan_t_ptr, (LONG)pEm->pSub0_seq_t_ptr, 0x200);
        } else {
            Joint_move(pEm, (LONG)pEm->pKan_t_ptr, (LONG)pEm->pSeq_t_ptr, 0x200);
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
            Flg_on(D_800D1FC0, pEm->Unk146);
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
            Flg_on(D_800D1FC0, pEm->Unk146);
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
            Flg_on(D_800D1FC0, pEm->Unk146);
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
            Flg_on(D_800D1FC0, pEm->Unk146);
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
        if (Joint_move(pEm, a1, a2, (((UCHAR)pEm->Unk144 >> 7) << 16) | 0x200)) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
            }
            pEm->Cdir_y += pEm->Unk14A;
        } else {
            Flg = pEm->Unk144;
            if (Flg & 8) {
                if (Joint_move(pEm, a1, a2, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200)) {
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
        Joint_move(pEm, a1, a2, 0x200);
        return 1;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Add_dir);
    Joint_move(pEm, a1, a2, 0x200);
    return 0;
}

LONG func_800171DC(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3)
{
    LONG x, z;

    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * pEm->Add_dir);
    Joint_move(pEm, a1, a2, (a3 << 16) | 0x200);
    Add_speedXZ(pEm, 0);
    x = pEm->Pos_x - pEm->Dest_x;
    z = pEm->Pos_z - pEm->Dest_z;
    return SquareRoot0(x * x + z * z) < 100;
}

LONG func_800172AC(ENEMY_WORK *pEm, LONG a1, LONG a2)
{
    Goto01(pEm, pEm->Dest_x, pEm->Dest_z);
    Joint_move(pEm, a1, a2, 0x200);
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
        if (Joint_move(pEm, (LONG)pKan, (LONG)pSeq, (((UCHAR)pEm->Unk144 >> 7) << 16) | 0x200)) {
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
                if (Joint_move(pEm, (LONG)pKan, (LONG)pSeq, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200)) {
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

void Enemy_work_set(void)
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

/* RE2 Emd_load (EM_SET.C), relocation part: Mem_top holds the loaded EMD, its first word the offset of
   the table of section offsets */
ULONG func_8001760C(ULONG Id, ENEMY_WORK *pEm, ULONG Mem_top, ENEMY_WORK *pData)
{
    ULONG *pChg;
    ULONG Size;
    ULONG Tmd_size;
    LONG Work;
    SHORT Ofs;
    tagGLOBAL *pG;

    pChg = (ULONG *)(Mem_top + *(ULONG *)Mem_top);
    Size = CD.pEntry[CD.Ring_status[CD.Write_ctr] & 0x7F].Size;
    pEm->pUnk11C = (ULONG *)(Mem_top + pChg[0]);
    pEm->pUnk118 = (ULONG *)(Mem_top + pChg[1]);
    pEm->pSeq_t_ptr = (ULONG *)(Mem_top + pChg[2]);
    pEm->pUnk10C = pEm->pKan_t_ptr = (ULONG *)(Mem_top + pChg[3]);
    pEm->pSub0_seq_t_ptr = (ULONG *)(Mem_top + pChg[4]);
    pEm->pSub0_kan_t_ptr = (ULONG *)(Mem_top + pChg[5]);
    pEm->pSub1_seq_t_ptr = (ULONG *)(Mem_top + pChg[6]);
    pEm->pSub1_kan_t_ptr = (ULONG *)(Mem_top + pChg[7]);
    pEm->pUnk1B4 = (ULONG *)(Mem_top + pChg[8]);
    pEm->Unk180[0] = Mem_top + pChg[9];
    pEm->Unk180[1] = Mem_top + pChg[10];
    pEm->Unk180[2] = Mem_top + pChg[11];
    pEm->Unk180[3] = Mem_top + pChg[12];
    pEm->pTmd = (ULONG *)(Mem_top + pChg[14]);
    pEm->Poly_rgb = 0x808080;
    pEm->pUnk174 = (void *)(Mem_top + pChg[13]);
    pG = &G;
    if (*(ULONG *)pEm->pUnk174 != 0) {
        func_8002AB34((ULONG *)pEm->pUnk174, pEm->Tpage, pEm->Clut);
        pEm->pUnk174 = (void *)((ULONG *)pEm->pUnk174 + 2);
    }
    Tmd_size = Size - pChg[14];
    if (pEm->Unk4B & 0x80) {
        func_800100A4((void *)(Mem_top + pChg[4]), (void *)(Mem_top + pChg[14]), Tmd_size);
        pEm->pTmd = (ULONG *)(Mem_top + pChg[4]);
        pEm->pSub0_kan_t_ptr = pData->pSub0_kan_t_ptr;
        pEm->pSub0_seq_t_ptr = pData->pSub0_seq_t_ptr;
        pEm->pSub1_kan_t_ptr = pData->pSub1_kan_t_ptr;
        pEm->pSub1_seq_t_ptr = pData->pSub1_seq_t_ptr;
    }
    if (pEm->Unk4B & 0x40) {
        func_800100A4((void *)(Mem_top + pChg[2]), (void *)(Mem_top + pChg[4]), Size - pChg[4]);
        Work = pChg[4] - pChg[2];
        Work >>= 2;
        pEm->pTmd -= Work;
        pEm->pSub0_seq_t_ptr -= Work;
        pEm->pSub0_kan_t_ptr -= Work;
        pEm->pSub1_seq_t_ptr -= Work;
        pEm->pSub1_kan_t_ptr -= Work;
        pEm->pSeq_t_ptr = pG->Pl_pSeq_t_ptr;
        pEm->pUnk10C = pEm->pKan_t_ptr = pG->Pl_pKan_t_ptr;
    }
    if (pEm->Unk4B & 0x20) {
        Work = Mem_top + pChg[3];
        Ofs = ((SHORT *)Work)[1];
        func_800100A4((void *)(Work + Ofs), (void *)(Mem_top + pChg[14]), Tmd_size);
        pEm->pTmd = (ULONG *)(Mem_top + pChg[3] + Ofs);
        pEm->pKan_t_ptr = pData->pKan_t_ptr;
        pEm->pSeq_t_ptr = pData->pSeq_t_ptr;
        pEm->pSub0_seq_t_ptr = pData->pSub0_seq_t_ptr;
        pEm->pSub0_kan_t_ptr = pData->pSub0_kan_t_ptr;
        pEm->pSub1_seq_t_ptr = pData->pSub1_seq_t_ptr;
        pEm->pSub1_kan_t_ptr = pData->pSub1_kan_t_ptr;
        pEm->pUnk1B4 = pData->pUnk1B4;
        pEm->Unk180[0] = pData->Unk180[0];
        pEm->Unk180[1] = pData->Unk180[1];
        pEm->Unk180[2] = pData->Unk180[2];
        pEm->Unk180[3] = pData->Unk180[3];
    }
    if (pEm->Id >= 0x50 && (pG->Status_flg & 0x20000000)) {
        pG->Status_flg &= ~0x20000000;
        Work = pChg[4] - pChg[2];
        func_800100A4((void *)0x8010D000, (void *)(Mem_top + pChg[2]), Work);
        pEm->pSeq_t_ptr = (ULONG *)0x8010D000;
        pEm->pUnk10C = pEm->pKan_t_ptr = (ULONG *)(pChg[3] - pChg[2] + 0x8010D000);
        func_800100A4((void *)(Mem_top + pChg[2]), (void *)(Mem_top + pChg[4]), Size - pChg[4]);
        Work >>= 2;
        pEm->pTmd -= Work;
        pEm->pSub0_seq_t_ptr -= Work;
        pEm->pSub0_kan_t_ptr -= Work;
        pEm->pSub1_seq_t_ptr -= Work;
        pEm->pSub1_kan_t_ptr -= Work;
        pEm->pUnk1B4 -= Work;
        pEm->Unk180[0] -= Work * 4;
        pEm->Unk180[1] -= Work * 4;
        pEm->Unk180[2] -= Work * 4;
        pEm->Unk180[3] -= Work * 4;
    }
    func_8002AB34(pEm->pTmd, pEm->Tpage, pEm->Clut);
    pEm->pTmd += 2;
    return (ULONG)(pEm->pTmd + ((Tmd_size >> 2) - 2));
}

/* move routines of the enemy overlays as linked into bank 0 (0x80100000) and bank 1 (0x8010D000) */
void Em40(ENEMY_WORK *pEm);
void Em39(ENEMY_WORK *pEm);
void Em27(ENEMY_WORK *pEm);
void Em26(ENEMY_WORK *pEm);
void Em2c(ENEMY_WORK *pEm);
void Em2d(ENEMY_WORK *pEm);
void Em3e(ENEMY_WORK *pEm);
void Em20(ENEMY_WORK *pEm);
void Em37(ENEMY_WORK *pEm);
void Em30(ENEMY_WORK *pEm);
void Em22(ENEMY_WORK *pEm);
void Em32(ENEMY_WORK *pEm);
void Em25(ENEMY_WORK *pEm);
void Em33(ENEMY_WORK *pEm);
void Em36(ENEMY_WORK *pEm);
void Em23(ENEMY_WORK *pEm);
void Em21(ENEMY_WORK *pEm);
void Em38(ENEMY_WORK *pEm);
void Em34(ENEMY_WORK *pEm);
void Em16(ENEMY_WORK *pEm);
void Em10(ENEMY_WORK *pEm);
void Em40_b1(ENEMY_WORK *pEm);
void Em39_b1(ENEMY_WORK *pEm);
void Em2c_b1(ENEMY_WORK *pEm);
void Em26_b1(ENEMY_WORK *pEm);
void Em27_b1(ENEMY_WORK *pEm);
void Em3e_b1(ENEMY_WORK *pEm);
void Em2d_b1(ENEMY_WORK *pEm);
void Em20_b1(ENEMY_WORK *pEm);
void Em37_b1(ENEMY_WORK *pEm);
void Em30_b1(ENEMY_WORK *pEm);
void Em22_b1(ENEMY_WORK *pEm);
void Em32_b1(ENEMY_WORK *pEm);
void Em25_b1(ENEMY_WORK *pEm);
void Em33_b1(ENEMY_WORK *pEm);
void Em36_b1(ENEMY_WORK *pEm);
void Em23_b1(ENEMY_WORK *pEm);
void Em21_b1(ENEMY_WORK *pEm);
void Em38_b1(ENEMY_WORK *pEm);
void Em34_b1(ENEMY_WORK *pEm);
void Em16_b1(ENEMY_WORK *pEm);
void Em10_b1(ENEMY_WORK *pEm);

void Em_move_tbl_set(void)
{
    G.Unk4320 = 0xFF;
    G.Unk4324 = 0xFF;
    G.Em_ov_mtbl[0][0x00] = Em10;
    G.Em_ov_mtbl[0][0x01] = Em10;
    G.Em_ov_mtbl[0][0x02] = Em10;
    G.Em_ov_mtbl[0][0x03] = Em10;
    G.Em_ov_mtbl[0][0x04] = Em10;
    G.Em_ov_mtbl[0][0x05] = Em10;
    G.Em_ov_mtbl[0][0x06] = Em16;
    G.Em_ov_mtbl[0][0x07] = Em10;
    G.Em_ov_mtbl[0][0x08] = Em16;
    G.Em_ov_mtbl[0][0x09] = Em16;
    G.Em_ov_mtbl[0][0x0A] = Em16;
    G.Em_ov_mtbl[0][0x0B] = Em16;
    G.Em_ov_mtbl[0][0x0C] = Em16;
    G.Em_ov_mtbl[0][0x0D] = Em16;
    G.Em_ov_mtbl[0][0x0E] = Em16;
    G.Em_ov_mtbl[0][0x0F] = Em16;
    G.Em_ov_mtbl[0][0x10] = Em20;
    G.Em_ov_mtbl[0][0x11] = Em21;
    G.Em_ov_mtbl[0][0x12] = Em22;
    G.Em_ov_mtbl[0][0x13] = Em23;
    G.Em_ov_mtbl[0][0x14] = Em22;
    G.Em_ov_mtbl[0][0x15] = Em25;
    G.Em_ov_mtbl[0][0x16] = Em26;
    G.Em_ov_mtbl[0][0x17] = Em27;
    G.Em_ov_mtbl[0][0x18] = Em23;
    G.Em_ov_mtbl[0][0x1C] = Em2c;
    G.Em_ov_mtbl[0][0x1D] = Em2d;
    G.Em_ov_mtbl[0][0x1E] = Em10;
    G.Em_ov_mtbl[0][0x1F] = Em10;
    G.Em_ov_mtbl[0][0x20] = Em30;
    G.Em_ov_mtbl[0][0x22] = Em32;
    G.Em_ov_mtbl[0][0x23] = Em33;
    G.Em_ov_mtbl[0][0x24] = Em34;
    G.Em_ov_mtbl[0][0x25] = Em34;
    G.Em_ov_mtbl[0][0x26] = Em36;
    G.Em_ov_mtbl[0][0x27] = Em37;
    G.Em_ov_mtbl[0][0x28] = Em38;
    G.Em_ov_mtbl[0][0x29] = Em39;
    G.Em_ov_mtbl[0][0x2A] = Em36;
    G.Em_ov_mtbl[0][0x2B] = Em37;
    G.Em_ov_mtbl[0][0x2E] = Em3e;
    G.Em_ov_mtbl[0][0x2F] = Em3e;
    G.Em_ov_mtbl[0][0x30] = Em40;
    G.Em_ov_mtbl[1][0x00] = Em10_b1;
    G.Em_ov_mtbl[1][0x01] = Em10_b1;
    G.Em_ov_mtbl[1][0x02] = Em10_b1;
    G.Em_ov_mtbl[1][0x03] = Em10_b1;
    G.Em_ov_mtbl[1][0x04] = Em10_b1;
    G.Em_ov_mtbl[1][0x05] = Em10_b1;
    G.Em_ov_mtbl[1][0x06] = Em16_b1;
    G.Em_ov_mtbl[1][0x07] = Em10_b1;
    G.Em_ov_mtbl[1][0x08] = Em16_b1;
    G.Em_ov_mtbl[1][0x09] = Em16_b1;
    G.Em_ov_mtbl[1][0x0A] = Em16_b1;
    G.Em_ov_mtbl[1][0x0B] = Em16_b1;
    G.Em_ov_mtbl[1][0x0C] = Em16_b1;
    G.Em_ov_mtbl[1][0x0D] = Em16_b1;
    G.Em_ov_mtbl[1][0x0E] = Em16_b1;
    G.Em_ov_mtbl[1][0x0F] = Em16_b1;
    G.Em_ov_mtbl[1][0x10] = Em20_b1;
    G.Em_ov_mtbl[1][0x11] = Em21_b1;
    G.Em_ov_mtbl[1][0x12] = Em22_b1;
    G.Em_ov_mtbl[1][0x13] = Em23_b1;
    G.Em_ov_mtbl[1][0x14] = Em22_b1;
    G.Em_ov_mtbl[1][0x15] = Em25_b1;
    G.Em_ov_mtbl[1][0x16] = Em26_b1;
    G.Em_ov_mtbl[1][0x17] = Em27_b1;
    G.Em_ov_mtbl[1][0x18] = Em23_b1;
    G.Em_ov_mtbl[1][0x1C] = Em2c_b1;
    G.Em_ov_mtbl[1][0x1D] = Em2d_b1;
    G.Em_ov_mtbl[1][0x1E] = Em10_b1;
    G.Em_ov_mtbl[1][0x1F] = Em10_b1;
    G.Em_ov_mtbl[1][0x20] = Em30_b1;
    G.Em_ov_mtbl[1][0x22] = Em32_b1;
    G.Em_ov_mtbl[1][0x23] = Em33_b1;
    G.Em_ov_mtbl[1][0x24] = Em34_b1;
    G.Em_ov_mtbl[1][0x25] = Em34_b1;
    G.Em_ov_mtbl[1][0x26] = Em36_b1;
    G.Em_ov_mtbl[1][0x27] = Em37_b1;
    G.Em_ov_mtbl[1][0x28] = Em38_b1;
    G.Em_ov_mtbl[1][0x29] = Em39_b1;
    G.Em_ov_mtbl[1][0x2A] = Em36_b1;
    G.Em_ov_mtbl[1][0x2B] = Em37_b1;
    G.Em_ov_mtbl[1][0x2E] = Em3e_b1;
    G.Em_ov_mtbl[1][0x2F] = Em3e_b1;
    G.Em_ov_mtbl[1][0x30] = Em40_b1;
}

void func_80017D9C(void) {
}
