#include "cd.h"
#include "door.h"
#include "enemy/em_common.h"
#include "monitor.h"
#include "sub.h"
#include "joint.h"
#include "snd.h"
#include "pl_wep.h"
#include "root_ck.h"
#include "sld.h"
#include "spl_sce.h"

/*
 * DOOR.C (RE2 DOOR.C): the door-opening sequence (Door_main .. Door_snd_trans). The tail of the object
 * holds enemy setup from RE2 EM_SET.C (Enemy_work_set, Emd_load, Em_move_tbl_set) and the enemies'
 * scenario handler Em_sce_main with its Em_sce[] slots (the enemy copy of SPL_SCE.C).
 */

/* local declarations (80011CF8.c) */
/* Add_speedXZ is called with a third (unused) argument in RE3 */
#define ADD_SPEED_XZ3(pEm) ((void (*)(ENEMY_WORK *, LONG, LONG))Add_speedXZ)((pEm), 0, 0)

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
    if (!(pDwork[0]->Attribute2 & 0x4000)) {
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
        pDoor->Unk226++;
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
    if (pDoor->Sound_flg != 0) {
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
    pDoor_info = &Dinfo[Door_no];
    File_no = Door_file_tbl[G.Unk783E][Door_no];
    Sector = floc[File_no].sector + floc[File_no].sec_high * 0x10000 + pDoor_info->Sector_offset;
    pCd->Fsum = pDoor_info->Data_sum;
    pCd->Fsize = pDoor_info->Data_size;
    G.Status_flg |= 0x800;
    pCd->Fsector = Sector;
    Cd_read(File_no, (ULONG *)0x8019D000, 3, "DOOR TEXTURE");
}

void Door_model_init(DOOR_WORK *pDw)
{
    DOOR *pDoor_cur;
    ULONG *Tmd_adr;

    pDoor_cur = pDoor;
    Tmd_adr = pDoor->pTmd_adr;
    pDw->Poly_rgb = 0x808080;
    pDw->pTmd = Tmd_adr + pDw->Model_no * 6;
    pDoor->pPrepacket = PresetObjectGT3_neo((MD_WORK *)&pDw->Attribute, pDoor_cur->pPrepacket);
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
    pDw = pDwork[pData->Unk_01];
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
        pMat = &pDwork[Free_flg & 0xF]->Workm;
    } else {
        pMat = &GsWSMATRIX;
    }
    pDw->pSuper = pMat;
    if (Free_flg & 0x2000) {
        pDoor->Sound_flg = 1;
    }
    pSce->Data = (UCHAR *)(pData + 1);
    Door_model_init(pDw);
    return 1;
}

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
    pDoor_info = &Dinfo[((UCHAR *)G.pSce_aot_on)[0xC]];
    while (pDoor_tex_trans != 0) {
        Task_sleep(1);
    }
    Sound_size = pDoor_info->Sound_size;
    Sum = pDoor_info->Sound_sum;
    File_no = Door_file_tbl[G.Unk783E][Door_no];
    Address = (ULONG *)((UCHAR *)0x8019D000 - Sound_size);
    pCd->Fsector = floc[File_no].sector + floc[File_no].sec_high * 0x10000;
    pCd->Fsum = Sum;
    pCd->Fsize = Sound_size;
    Cd_read(File_no, Address, 3, "DOOR SOUND");
    Memcpy((void *)0x801FC100, Address, Address[5] + 0x10);
    SpuSetTransferMode(0);
    Snd_vab_head_set(4, (ULONG *)0x801FC100);
    Snd_work.Bank_id[4].Id = 4;
    Snd_work.Bank_id[4].pData = (UCHAR *)0x3DE10;
    SpuSetTransferStartAddr(0x3DE10);
    SpuRead((UCHAR *)Address + ((Address[5] + 0x10) >> 2 << 2), Address[4]);
    while (SpuIsTransferCompleted(0) == 0);
    G.Status_flg &= ~0x800;
}

/* scenario-driven moves of an enemy (Spl_Scenario counterpart): Em_sce[Routine_1], interpolation, feet, neck */
void Em_sce_main(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
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
        Em_sce[pEm->Routine_1](pEm, pKan, pSeq);
        pEm->Unk144 &= ~0x800;
    } else if (pEm->Routine_1 != 20) {
        if (!(Flg & 0x800)) {
            Em_sce[pEm->Routine_1](pEm, pKan, pSeq);
            Spl_sce_hokan_set(pEm, pKan, pSeq);
        } else {
            Spl_sce_hokan(pEm, pKan, pSeq, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200);
        }
    }
    if (pEm->Unk144 & 1) {
        Foot_set_pl(pEm, 0, 0);
    }
    if (pEm->Unk144 & 2) {
        Foot_set_pl(pEm, 0, 1);
    }
    if (pEm->Neck_flg & 0x80) {
        Rot_neck(pEm, pEm->Cdir_y);
    }
}

/* Em_sce slot 0: Em_sce_mv00 with the room kan/seq */
void Em_sce00(ENEMY_WORK *pEm)
{
    Em_sce_mv00(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr);
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 1: Em_sce_mv00 with Kan/Seq */
void Em_sce01(ENEMY_WORK *pEm)
{
    Em_sce_mv00(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 2: Em_sce_mv00 with Sub0 */
void Em_sce02(ENEMY_WORK *pEm)
{
    Em_sce_mv00(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 3: Em_sce_mv00 with Sub1 */
void Em_sce03(ENEMY_WORK *pEm)
{
    Em_sce_mv00(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 4 */
void Em_sce04(ENEMY_WORK *pEm)
{
    ULONG *pKan;
    ULONG *pSeq;
    LONG Move;

    pKan = pEm->pKan_t_ptr;
    pSeq = pEm->pSeq_t_ptr;
    Move = Em_sce04_move_tbl[pEm->Id - 0x10];
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
        EM_MOVE(pEm) = Move + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)Em_sce_mv01(pEm, pKan, pSeq)) {
            Spd_get2(pEm, pKan, pSeq, 0);
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Spd_get2(pEm, pKan, pSeq, 0);
        if ((UCHAR)Em_sce_mv02(pEm, pKan, pSeq, 0)) {
            Flg_on(Room_flg, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 4;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 5 */
void Em_sce05(ENEMY_WORK *pEm)
{
    ULONG *pKan;
    ULONG *pSeq;
    LONG Move;

    pKan = pEm->pKan_t_ptr;
    pSeq = pEm->pSeq_t_ptr;
    Move = Em_sce05_move_tbl[pEm->Id - 0x10];
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
        EM_MOVE(pEm) = Move + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)Em_sce_mv01(pEm, pKan, pSeq)) {
            Spd_get2(pEm, pKan, pSeq, 0);
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        Spd_get2(pEm, pKan, pSeq, 0);
        if ((UCHAR)Em_sce_mv02(pEm, pKan, pSeq, 0)) {
            Flg_on(Room_flg, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 5;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 6 */
void Em_sce06(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70000 + (Em_sce06_move_tbl[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        if (Em_sce06_move_tbl[pEm->Id - 0x10] & 0x80) {
            Joint_move(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        } else {
            Joint_move(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x200);
        }
        break;
    }
}

/* Em_sce slot 7 */
void Em_sce07(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70000 + (Em_sce07_move_tbl[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, Em_sce07_move_tbl[pEm->Id - 0x10] & 0x40);
        if ((UCHAR)Em_sce_mv02(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, Em_sce07_move_tbl[pEm->Id - 0x10] & 0x40)) {
            Flg_on(Room_flg, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 7;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 8 */
void Em_sce08(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        pEm->Spd_z = 0;
        EM_MOVE(pEm) = 0x70000 + (Em_sce07_move_tbl[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        Spd_get2(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
        if ((UCHAR)Em_sce_mv02(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0)) {
            Flg_on(Room_flg, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 8;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 9 */
void Em_sce09(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70000 + (Em_sce04_move_tbl[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)Em_sce_mv01(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr)) {
            Flg_on(Room_flg, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 9;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Em_sce_foot_se(pEm);
}

/* Em_sce slot 0x15 */
void Em_sce15(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = 0x70000 + (Em_sce04_move_tbl[pEm->Id - 0x10] & 0x3F);
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)Em_sce_mv03(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr)) {
            Flg_on(Room_flg, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0x15;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Em_sce_foot_se(pEm);
}

/* play the scenario motion in place (Spl_sce_mv04 counterpart) */
void Em_sce_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG Flg;
    ULONG R;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Hokan_flg = 7;
        if (pEm->Unk144 & 0x40) {
            pEm->Hokan_flg = 0;
        }
    case 1:
        if (Joint_move(pEm, pKan, pSeq, (((UCHAR)pEm->Unk144 >> 7) << 16) | 0x200)) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
            }
            pEm->Cdir_y += pEm->Unk14A;
        } else {
            Flg = pEm->Unk144;
            if (Flg & 8) {
                if (Joint_move(pEm, pKan, pSeq, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200)) {
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

/* turn towards (Dest_x, Dest_z) while playing the motion; 1 when facing it */
LONG Em_sce_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x15E) == 0) {
        Joint_move(pEm, pKan, pSeq, 0x200);
        return 1;
    }
    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Add_dir);
    Joint_move(pEm, pKan, pSeq, 0x200);
    return 0;
}

/* walk to (Dest_x, Dest_z); 1 when there */
LONG Em_sce_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG a3)
{
    LONG x, z;

    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * pEm->Add_dir);
    Joint_move(pEm, pKan, pSeq, (a3 << 16) | 0x200);
    Add_speedXZ(pEm, 0);
    x = pEm->Pos_x - pEm->Dest_x;
    z = pEm->Pos_z - pEm->Dest_z;
    return SquareRoot0(x * x + z * z) < 100;
}

/* turn to the direction Dest_x; 1 when reached */
LONG Em_sce_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Goto01(pEm, pEm->Dest_x, pEm->Dest_z);
    Joint_move(pEm, pKan, pSeq, 0x200);
    return pEm->Cdir_y == pEm->Dest_x;
}

/* play the scenario motion with its root speed (Spl_sce_mv00 counterpart) */
void Em_sce_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG Flg;
    ULONG R;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Hokan_flg = 7;
        if (pEm->Unk144 & 0x40) {
            pEm->Hokan_flg = 0;
        }
        Spd_get2(pEm, pKan, pSeq, (UCHAR)pEm->Unk144 >> 7);
    case 1:
        Spd_get2(pEm, pKan, pSeq, (UCHAR)pEm->Unk144 >> 7);
        if (pEm->Unk144 & 0x10) {
            pEm->Spd_x = pEm->Spd_x >> 1;
            pEm->Spd_y = pEm->Spd_y >> 1;
            pEm->Spd_z = pEm->Spd_z >> 1;
        }
        if (Joint_move(pEm, pKan, pSeq, (((UCHAR)pEm->Unk144 >> 7) << 16) | 0x200)) {
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
                if (Joint_move(pEm, pKan, pSeq, ((Flg = (R = Flg & 0x80) != 0) << 16) | 0x200)) {
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

/* footstep sound from the current seq frame */
void Em_sce_foot_se(ENEMY_WORK *pEm)
{
    if (!(pEm->Unk144 & 0x100)) {
        Em_seq_sound(pEm);
    }
}

void Enemy_work_set(void)
{
    tagGLOBAL *pG = &G;
    ENEMY_WORK **ppEm;
    ULONG Mem_top;
    ULONG Em_num;
    ULONG Em_size;
    LONG i;

    ppEm = &pG->pEnemy[1];
    i = 41;
    do {
        *ppEm = &pG->Sub_work;
        i--;
        ppEm++;
    } while (i != 0);
    Em_num = pG->Em_num;
    pG->pEnemy[0] = PLAYER_EM(pG);
    pG->pEnemy[0]->Be_flg = 1;
    pG->ppEnemy_end = &pG->pEnemy[2];
    if (Em_num != 0) {
        Em_size = 0x1FC;
        Mem_top = (ULONG)pG->pEm_mem;
        pG->pEm_mem = (ENEMY_WORK *)((ULONG)pG->pEm_mem + Em_num * Em_size);
        i = 0;
        do {
            pG->pEnemy[i + 2] = (ENEMY_WORK *)Mem_top;
            ((ENEMY_WORK *)Mem_top)->Be_flg = 0;
            Mem_top += Em_size;
            i++;
        } while (--Em_num != 0);
        pG->ppEnemy_end = &pG->pEnemy[i + 2];
    }
}

/* RE2 Emd_load (EM_SET.C), relocation part: Mem_top holds the loaded EMD, its first word the offset of
   the table of section offsets */
ULONG Emd_load(ULONG Id, ENEMY_WORK *pEm, ULONG Mem_top, ENEMY_WORK *pData)
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
    pEm->pUnk180_seq_t_ptr = (ULONG *)(Mem_top + pChg[9]);
    pEm->pUnk184_kan_t_ptr = (ULONG *)(Mem_top + pChg[10]);
    pEm->pUnk188_seq_t_ptr = (ULONG *)(Mem_top + pChg[11]);
    pEm->pUnk18C_kan_t_ptr = (ULONG *)(Mem_top + pChg[12]);
    pEm->pTmd = (ULONG *)(Mem_top + pChg[14]);
    pEm->Poly_rgb = 0x808080;
    pEm->pUnk174 = (void *)(Mem_top + pChg[13]);
    pG = &G;
    if (*(ULONG *)pEm->pUnk174 != 0) {
        Mapping_md((ULONG *)pEm->pUnk174, pEm->Tpage, pEm->Clut);
        pEm->pUnk174 = (void *)((ULONG *)pEm->pUnk174 + 2);
    }
    Tmd_size = Size - pChg[14];
    if (pEm->Unk4B & 0x80) {
        Memcpy((void *)(Mem_top + pChg[4]), (void *)(Mem_top + pChg[14]), Tmd_size);
        pEm->pTmd = (ULONG *)(Mem_top + pChg[4]);
        pEm->pSub0_kan_t_ptr = pData->pSub0_kan_t_ptr;
        pEm->pSub0_seq_t_ptr = pData->pSub0_seq_t_ptr;
        pEm->pSub1_kan_t_ptr = pData->pSub1_kan_t_ptr;
        pEm->pSub1_seq_t_ptr = pData->pSub1_seq_t_ptr;
    }
    if (pEm->Unk4B & 0x40) {
        Memcpy((void *)(Mem_top + pChg[2]), (void *)(Mem_top + pChg[4]), Size - pChg[4]);
        Work = pChg[4] - pChg[2];
        Work >>= 2;
        pEm->pTmd -= Work;
        pEm->pSub0_seq_t_ptr -= Work;
        pEm->pSub0_kan_t_ptr -= Work;
        pEm->pSub1_seq_t_ptr -= Work;
        pEm->pSub1_kan_t_ptr -= Work;
        pEm->pSeq_t_ptr = pG->Player.pSeq_t_ptr;
        pEm->pUnk10C = pEm->pKan_t_ptr = pG->Player.pKan_t_ptr;
    }
    if (pEm->Unk4B & 0x20) {
        Work = Mem_top + pChg[3];
        Ofs = ((SHORT *)Work)[1];
        Memcpy((void *)(Work + Ofs), (void *)(Mem_top + pChg[14]), Tmd_size);
        pEm->pTmd = (ULONG *)(Mem_top + pChg[3] + Ofs);
        pEm->pKan_t_ptr = pData->pKan_t_ptr;
        pEm->pSeq_t_ptr = pData->pSeq_t_ptr;
        pEm->pSub0_seq_t_ptr = pData->pSub0_seq_t_ptr;
        pEm->pSub0_kan_t_ptr = pData->pSub0_kan_t_ptr;
        pEm->pSub1_seq_t_ptr = pData->pSub1_seq_t_ptr;
        pEm->pSub1_kan_t_ptr = pData->pSub1_kan_t_ptr;
        pEm->pUnk1B4 = pData->pUnk1B4;
        pEm->pUnk180_seq_t_ptr = pData->pUnk180_seq_t_ptr;
        pEm->pUnk184_kan_t_ptr = pData->pUnk184_kan_t_ptr;
        pEm->pUnk188_seq_t_ptr = pData->pUnk188_seq_t_ptr;
        pEm->pUnk18C_kan_t_ptr = pData->pUnk18C_kan_t_ptr;
    }
    if (pEm->Id >= 0x50 && (pG->Status_flg & 0x20000000)) {
        pG->Status_flg &= ~0x20000000;
        Work = pChg[4] - pChg[2];
        Memcpy((void *)0x8010D000, (void *)(Mem_top + pChg[2]), Work);
        pEm->pSeq_t_ptr = (ULONG *)0x8010D000;
        pEm->pUnk10C = pEm->pKan_t_ptr = (ULONG *)(pChg[3] - pChg[2] + 0x8010D000);
        Memcpy((void *)(Mem_top + pChg[2]), (void *)(Mem_top + pChg[4]), Size - pChg[4]);
        Work >>= 2;
        pEm->pTmd -= Work;
        pEm->pSub0_seq_t_ptr -= Work;
        pEm->pSub0_kan_t_ptr -= Work;
        pEm->pSub1_seq_t_ptr -= Work;
        pEm->pSub1_kan_t_ptr -= Work;
        pEm->pUnk1B4 -= Work;
        pEm->pUnk180_seq_t_ptr -= Work;
        pEm->pUnk184_kan_t_ptr -= Work;
        pEm->pUnk188_seq_t_ptr -= Work;
        pEm->pUnk18C_kan_t_ptr -= Work;
    }
    Mapping_md(pEm->pTmd, pEm->Tpage, pEm->Clut);
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
    G.Em_bin_info[0].Id = 0xFF;
    G.Em_bin_info[1].Id = 0xFF;
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
