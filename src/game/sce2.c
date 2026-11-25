#include "sub.h"
#include "player.h"
#include "status.h"
#include "joint.h"
#include "md_sub.h"
#include "pl_wep.h"
#include "sce2.h"
#include "sce_at.h"

int Kage_work_color_set_() __asm__("Kage_work_color_set"); /* returns int here (canonical void); unprototyped call */

extern UCHAR D_800D442C_[] __asm__("D_800D442C"); /* type differs from the canonical extern */
s8 Search_item_id_(UCHAR Id, UCHAR A1) __asm__("Search_item_id"); /* arg 2 UC (canonical signed char) */
LONG Quake_add_() __asm__("Quake_add"); /* returns long here (canonical void); unprototyped call */
typedef struct {
    UCHAR Unk0;
    UCHAR Max;                         /* 0x1 */
    UCHAR pad2[2];
} ITEM_TBL4;
LONG Flg_off_(ULONG *pFlg, ULONG Bit) __asm__("Flg_off"); /* returns long here (canonical void) */

static inline ULONG Sce_skip(SCE_TASK *pSce, LONG n)
{
    pSce->Data += n;
    return 1;
}

ULONG func_80055A18(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG Id;
    ULONG Num;
    LONG i;
    ITEM_WORK *p;

    pData = pSce->Data;
    i = 0;
    Id = pData[1];
    Num = pData[2];
    pSce->Data = pData + 3;
    while (1) {
        i = Search_item_id_(Id, i);
        if (i == -1) {
            break;
        }
        p = &G.pUnk7C7C->Item[i];
        if (((ITEM_TBL4 *)D_800A0514)[p->Id].Max >= p->Num + Num) {
            return 1;
        }
        i++;
    }
    i = Search_item_id_(0, 0);
    return i != -1;
}

/* RE2 Dir_ck */
ULONG func_80055AF0(SCE_TASK *pSce)
{
    SHORT *pData;

    pData = (SHORT *)(pSce->Data += 8);
    return Goto00_ck(pSce->pWork, pData[-3], pData[-2], pData[-1]) == 0;
}

ULONG func_80055B30(SCE_TASK *pSce)
{
    pSce->Data++;
    G.System_flg |= 0x20000;
    return 1;
}

/* RE2 Aot_set */
ULONG Door_aot_set(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data += 0x20;
    return 1;
}

ULONG Door_aot_set_4p(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data[3] |= 0x80;
    pSce->Data += 0x28;
    return 1;
}

ULONG Aot_set(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data += 0x14;
    return 1;
}

ULONG Aot_set_4p(SCE_TASK *pSce)
{
    ULONG i;

    i = pSce->Data[1];
    if (G.Sce_aot[i] == NULL) {
        G.nSce_aot++;
    }
    G.Sce_aot[i] = (struct tagSCE_AOT *)(pSce->Data + 2);
    pSce->Data[3] |= 0x80;
    pSce->Data += 0x1C;
    return 1;
}

/* RE2 Aot_reset */
ULONG Aot_reset(SCE_TASK *pSce)
{
    UCHAR *pData;
    UCHAR *pAot;
    ULONG N;
    LONG Y, Z;

    pData = pSce->Data;
    pAot = (UCHAR *)G.Sce_aot[(s8)pData[1]];
    N = pAot[1];
    pAot[0] = pData[2];
    pAot[1] = pData[3] | (N & 0x80);
    if (N & 0x80) {
        pAot += 0x14;
    } else {
        pAot += 0xC;
    }
    Y = ((SHORT *)pData)[3];
    N = ((SHORT *)pData)[2];
    Z = ((SHORT *)pData)[4];
    ((SHORT *)pAot)[1] = Y;
    ((SHORT *)pAot)[0] = N;
    ((SHORT *)pAot)[2] = Z;
    pSce->Data = pData + 0xA;
    return 1;
}

/* RE2 Aot_on; RE3 also points G.pEm at the task's work (1 = player) */
ULONG Aot_on(SCE_TASK *pSce)
{
    UCHAR *pAot;
    UCHAR *pArg;

    pCAot = pAot = (UCHAR *)G.Sce_aot[pSce->Data[1]];
    if (pAot[1] & 0x80) {
        pArg = pAot + 0x14;
    } else {
        pArg = pAot + 0xC;
    }
    if (pSce->pWork == (ENEMY_WORK *)1) {
        G.pEm = G_PLAYER;
    } else {
        G.pEm = pSce->pWork;
    }
    Sce_at_jump_tbl[pAot[0]](pArg);
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sce2", Get_matrix);

/* RE2 Sce_espr_on */
ULONG Sce_espr_on(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[1];
    Dir = ((USHORT *)pData)[2];
    Mat = Get_matrix((s8)pData[4], (SHORT)Dir >> 8);
    Dir = ((USHORT *)pData)[3];
    Svec.vx = ((SHORT *)pData)[4];
    Svec.vy = ((SHORT *)pData)[5];
    Svec.vz = ((SHORT *)pData)[6];
    Esp_call((((D0 >> 8) & 0xF) << 24) | ((D0 & 0xFF) << 8) | pData[1],
                  (Dir << 16) | ((USHORT *)pData)[7], Mat, &Svec, 0);
    pSce->Data = pData + 0x10;
    return 1;
}

/* RE2 Sce_espr_on2 */
ULONG Sce_espr_on2(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[2];
    Dir = ((USHORT *)pData)[3];
    Mat = Get_matrix((s8)pData[6], (SHORT)Dir >> 8);
    Dir = ((USHORT *)pData)[4];
    Svec.vx = ((SHORT *)pData)[5];
    Svec.vy = ((SHORT *)pData)[6];
    Svec.vz = ((SHORT *)pData)[7];
    Esp_call((((D0 >> 8) & 0xF) << 24) | (pData[2] << 16) | ((D0 & 0xFF) << 8) | pData[3],
                  (Dir << 16) | ((USHORT *)pData)[8], Mat, &Svec, 0);
    pSce->Data = pData + 0x12;
    return 1;
}

/* RE2 Sce_espr3d_on */
ULONG Sce_espr3d_on(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;
    SVECTOR Svec2;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[1];
    Dir = (SHORT)((USHORT *)pData)[2];
    Mat = Get_matrix((s8)Dir, (LONG)Dir >> 8);
    Dir = ((USHORT *)pData)[3];
    Svec.vx = ((SHORT *)pData)[4];
    Svec.vy = ((SHORT *)pData)[5];
    Svec.vz = ((SHORT *)pData)[6];
    Svec2.vx = ((SHORT *)pData)[7];
    Svec2.vy = ((SHORT *)pData)[8];
    Svec2.vz = ((SHORT *)pData)[9];
    Esp_call((((D0 >> 8) & 0xF) << 24) | ((D0 & 0xFF) << 8) | pData[1],
                  (Dir << 16) | ((USHORT *)pData)[10], Mat, &Svec, &Svec2);
    pSce->Data = pData + 0x16;
    return 1;
}

/* RE2 Sce_espr3d_on2 */
ULONG Sce_espr3d_on2(SCE_TASK *pSce)
{
    UCHAR *pData;
    ULONG D0;
    MATRIX *Mat;
    ULONG Dir;
    SVECTOR Svec;
    SVECTOR Svec2;

    pData = pSce->Data;
    D0 = ((USHORT *)pData)[2];
    Dir = (SHORT)((USHORT *)pData)[3];
    Mat = Get_matrix((s8)Dir, (LONG)Dir >> 8);
    Dir = ((USHORT *)pData)[4];
    Svec.vx = ((SHORT *)pData)[5];
    Svec.vy = ((SHORT *)pData)[6];
    Svec.vz = ((SHORT *)pData)[7];
    Svec2.vx = ((SHORT *)pData)[8];
    Svec2.vy = ((SHORT *)pData)[9];
    Svec2.vz = ((SHORT *)pData)[10];
    Esp_call((((D0 >> 8) & 0xF) << 24) | (pData[2] << 16) | ((D0 & 0xFF) << 8) | pData[3],
                  (Dir << 16) | ((USHORT *)pData)[11], Mat, &Svec, &Svec2);
    pSce->Data = pData + 0x18;
    return 1;
}

ULONG Sce_espr_kill(SCE_TASK *pSce)
{
    MATRIX *pM;

    pM = Get_matrix((s8)pSce->Data[3], (s8)pSce->Data[4]);
    func_8001B894((pSce->Data[2] << 8) | pSce->Data[1], pM);
    pSce->Data += 5;
    return 1;
}

/* int-returning view of the void func_8001B894: the caller's register allocation needs v0 clobbered */
LONG func_8001B894_l(ULONG Id, MATRIX *pM) __asm__("func_8001B894"); /* returns long here (canonical void) */

ULONG Sce_espr_kill2(SCE_TASK *pSce)
{
    func_8001B894_l((pSce->Data[1] << 16) | 0x1000000, 0);
    pSce->Data += 2;
    return 1;
}

ULONG Sce_espr_task(SCE_TASK *pSce)
{
    MATRIX *v;
    ULONG i;

    if (pSce->Data[1] != 6) {
        v = Get_matrix((s8)pSce->Data[1], (s8)pSce->Data[2]);
        for (i = 0; i < 0x60; i++) {
            if (Esp[i].pMat == v) {
                Esp[i].Routine_1 = 0;
                Esp[i].Routine_0 = 0;
                Esp[i].Be_flg = 0;
            }
        }
    } else {
        func_8001B894(0x2000000, 0);
    }
    pSce->Data += 3;
    return 1;
}

ULONG Sce_espr_control(SCE_TASK *pSce)
{
    UCHAR *pData;

    pData = pSce->Data;
    func_8001BA48((pData[2] << 8) | pData[1], Get_matrix((s8)pData[3], (s8)pData[4]),
                  pData[5] | (pData[6] << 16), pData[7] | (pData[8] << 16));
    pSce->Data = pData + 10;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sce2", Obj_model_set);

/* RE2 Mizu_div_set */
ULONG Mizu_div_set(SCE_TASK *pSce)
{
    POLY_GT4 *pPrim;
    ULONG Div_num;
    LONG i;
    LONG j;

    Div_num = pSce->Data[1];
    Sce.Mizu_div_max = Div_num;
    if (Div_num == 0) {
        pSce->Data += 2;
        return 1;
    }
    Sce.pMizu_div = (UCHAR *)G.pEm_mem;
    pPrim = (POLY_GT4 *)G.pEm_mem;
    G.pEm_mem = (ENEMY_WORK *)((UCHAR *)G.pEm_mem + Div_num * 0x548);
    do {
        for (i = 0; i < 13; i++) {
            for (j = 2; j != 0; j--) {
                pPrim->tag = 0x0C000000;
                *(ULONG *)&pPrim->r0 = 0x3E808080;
                *(ULONG *)&pPrim->r1 = 0x808080;
                *(ULONG *)&pPrim->r2 = 0x808080;
                *(ULONG *)&pPrim->r3 = 0x808080;
                pPrim++;
            }
        }
        Div_num--;
    } while (Div_num != 0);
    pSce->Data += 2;
    return 1;
}

/* SCE em_set operand (0x18 bytes) */
typedef struct {
    UCHAR Opcode;                      /* 0x00 */
    UCHAR Unk1;                        /* 0x01 */
    signed char Work_no;                     /* 0x02 G.pEnemy[Work_no + 2] */
    UCHAR Id;                          /* 0x03 */
    USHORT Type;                       /* 0x04 */
    USHORT Status_flg;                 /* 0x06 */
    UCHAR nFloor;                      /* 0x08 */
    UCHAR Unk9;                        /* 0x09 */
    UCHAR UnkA;                        /* 0x0A */
    UCHAR Flg_no;                      /* 0x0B 0xFF: none */
    SHORT Pos_x;                       /* 0x0C */
    SHORT Pos_y;                       /* 0x0E */
    SHORT Pos_z;                       /* 0x10 */
    SHORT Cdir_y;                      /* 0x12 */
    SHORT Timer0;                      /* 0x14 */
    SHORT UnkDA;                       /* 0x16 */
} SCE_EM_SET;
extern SCE_EM_SET *D_800E0198;          /* operand of the em_set being run */

ULONG Sce_em_set(SCE_TASK *pSce)
{
    tagGLOBAL *pG = &G;
    ULONG *pFlg;
    ENEMY_WORK *pEm;
    LONG No;
    ENEMY_WORK *pNext;

    D_800E0198 = (SCE_EM_SET *)pSce->Data;
    if (pG->Unk783E >= 2 && !(pG->System_flg & 0x80))
        pFlg = pG->Unk78B0;
    else
        pFlg = pG->Unk7890;
    if (D_800E0198->Flg_no == 0xFF || !Flg_ck(pFlg, D_800E0198->Flg_no)) {
        if (G.Player.Id < 8) {
            if ((D_800E0198->Id == 0x5B || D_800E0198->Id == 0x5F) && G.Player.Id >= 2)
                D_800E0198->Id = G.Player.Id + 0x60;
        } else {
            if ((D_800E0198->Id == 0x5B || D_800E0198->Id == 0x5F) && G.Unk7864 >= 2)
                D_800E0198->Id = G.Unk7864 + 0x60;
        }
        No = D_800E0198->Work_no + 2;
        if (No != 1)
            pG->Em_num++;
        pEm = pG->pEnemy[No] = pG->pEm_mem;
        pG->pEm_mem = (ENEMY_WORK *)((UCHAR *)pG->pEm_mem + 0x1FC);
        if (pG->ppEnemy_end < &pG->pEnemy[No + 1])
            pG->ppEnemy_end = &pG->pEnemy[No + 1];
        pNext = pG->pEm_mem;
        pEm->Be_flg = 1;
        pEm->Unk12F = D_800E0198->Unk9;
        if (Sce.Unk1498 != D_800E0198->Unk9) {
            if (Sce.Unk1498)
                Sce.Unk1499 = D_800E0198->Unk9;
            else
                Sce.Unk1498 = D_800E0198->Unk9;
        }
        *(ULONG *)&pEm->Routine_0 = 0;
        pEm->Pos_x = pEm->Old_x = D_800E0198->Pos_x;
        pEm->Pos_y = pEm->Old_y = D_800E0198->Pos_y;
        pEm->Pos_z = pEm->Old_z = D_800E0198->Pos_z;
        pEm->Cdir_x = 0;
        pEm->Cdir_y = D_800E0198->Cdir_y;
        pEm->Cdir_z = 0;
        pEm->Dest_x = 0;
        pEm->Dest_z = 0;
        pEm->Id = D_800E0198->Id;
        pEm->Type = D_800E0198->Type;
        pEm->Status_flg = D_800E0198->Status_flg;
        pEm->Unk147 = D_800E0198->Flg_no;
        pEm->Unk4B = D_800E0198->UnkA;
        pEm->nFloor = D_800E0198->nFloor;
        pEm->Unk122 = -(D_800E0198->nFloor * 1800);
        pEm->Damage_cnt = 0;
        pEm->UnkC6 = 0;
        pEm->Timer0 = D_800E0198->Timer0;
        pEm->UnkDA = D_800E0198->UnkDA;
        pEm->Unk12A = 0x1000;
        pEm->pUnk114 = &Pl_work;
        pEm->UnkE2 = 0;
        pEm->UnkE3 = 0;
        pEm->Kage.pKw = 0;
        pEm->Unk144 = 0;
        pEm->Add_dir = 0;
        pEm->Unk14A = 0;
        pEm->Unk14C = 0;
        pEm->Unk14E = 0;
        pEm->Unk150 = 0;
        pEm->Unk152 = 0;
        pEm->Water = 0;
        pEm->Unk1B8 = 0;
        pEm->Unk1BA = 0;
        pEm->Unk1BC[0] = 0;
        pEm->Unk1BC[1] = 0;
        pEm->Unk1C0 = 0;
        pEm->Unk1C1 = 0;
        pEm->Unk1C2 = 0;
        pEm->Unk1CA = 0;
        pEm->Unk1C8 = 0;
        pEm->Unk132 = 0;
        pEm->Unk158 = 0;
        pEm->Col.Id = 0;
        pEm->UnkD = 0;
        pEm->Root_ck_cnt = No & 7;
        pG->pEm_mem = pNext;
    }
    pSce->Data += 0x18;
    return 1;
}

ULONG Plc_motion(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pW;
    ULONG Move_no;
    ULONG Attr;

    pData = pSce->Data;
    pW = pSce->pWork;
    Move_no = pData[2];
    Attr = pData[3];
    *(ULONG *)&pW->Routine_0 = (pData[1] << 8) | 4;
    pW->Move_cnt = 0;
    pW->Move_no = Move_no;
    if (pW->Id == 0) {
        pW->Unk1A0 = Move_no;
        pW->Unk1A1 = 0;
    }
    pW->Unk144 = Attr;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    pW->Type |= 0x100;
    pSce->Data += 4;
    return 1;
}

ULONG func_80056E28(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pW;
    SHORT a, b;

    pData = pSce->Data;
    pW = pSce->pWork;
    a = pData[1] | 0x80;
    b = ((USHORT *)pData)[1];
    pW->Unk150 = a;
    pW->Unk152 = b;
    pSce->Data += 4;
    return 1;
}

/* RE2 Plc_dest */
ULONG Plc_dest(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pEm;
    ULONG Id;
    ULONG Mode;
    UCHAR Set_no;

    pEm = pSce->pWork;
    pData = pSce->Data;
    Id = pEm->Id;
    Mode = pData[2];
    if (!(pEm->Unk144 & 4) || pEm->Routine_1 != Mode) {
        *(ULONG *)&pEm->Routine_0 = (Mode << 8) | 4;
    }
    if (Id < 0x10) {
        switch (Mode) {
        case 4:
        case 0x12:
            pEm->Add_dir = Subpl_walk[Id][1];
            break;
        case 5:
            pEm->Add_dir = Subpl_run[Id][1];
            break;
        case 7:
            pEm->Add_dir = Subpl_back[Id][1];
            break;
        case 8:
            pEm->Add_dir = Subpl_ato[Id][1];
            break;
        case 9:
            pEm->Add_dir = Subpl_dir[Id][1];
            break;
        }
    } else {
        switch (Mode) {
        case 4:
        case 0x12:
            pEm->Add_dir = 0x30;
            break;
        case 5:
            pEm->Add_dir = 0x48;
            break;
        case 7:
            pEm->Add_dir = 0x30;
            break;
        case 8:
            pEm->Add_dir = 0x30;
            break;
        case 9:
            pEm->Add_dir = 0x60;
            break;
        }
    }
    Set_no = pData[3];
    pEm->Unk144 = 0;
    pEm->Unk14A = 0;
    pEm->Unk146 = Set_no;
    pEm->Type |= 0x100;
    pEm->Dest_x = ((SHORT *)pData)[2];
    pEm->Dest_z = ((SHORT *)pData)[3];
    pEm->Unk152 = 0;
    pEm->Unk150 = 0;
    pEm->Unk14C = pEm->Dest_x;
    pEm->Unk14E = pEm->Dest_z;
    pSce->Data = pData + 8;
    return 1;
}

ULONG Plc_gun(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    ULONG v;

    v = pSce->Data[1];
    pW = pSce->pWork;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    *(ULONG *)&pW->Routine_0 = (v << 8) | 4;
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sce2", Plc_gun_eff);

ULONG Plc_neck(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    ULONG Kind;
    PARTS_W *pParts;
    LONG A, B, C, D;

    pW = pSce->pWork;
    Kind = pSce->Data[1];
    pParts = pW->pSin_parts_ptr + 1;
    pW->Unk120 = 0x80;
    A = ((SHORT *)pSce->Data)[1];
    B = ((SHORT *)pSce->Data)[2];
    C = ((SHORT *)pSce->Data)[3];
    D = ((SHORT *)pSce->Data)[4];
    switch (Kind) {
    case 0:
        pW->Unk120 |= 0x12;
        break;
    case 2:
        pW->Unk120 |= 0x8;
        break;
    case 3:
        pW->Unk120 |= 0x2A;
        break;
    case 4:
        pW->Unk120 |= 0x58;
        break;
    case 1:
    case 5:
        pW->Unk120 |= 0x4;
        break;
    case 6:
        pW->Unk120 = 0;
        break;
    case 7:
        pW->Unk120 |= 0x1;
        break;
    case 8:
        pW->Unk120 |= 0x12;
        ((SHORT *)&pParts->Unk74)[0] = 0;
        ((SHORT *)&pParts->Unk74)[1] = 0;
        break;
    }
    pW->Unk164 = A;
    pW->Unk166 = B;
    pW->Unk168 = C;
    if (Kind == 5) {
        switch (pW->Unk164) {
        case 1:
            pW->Unk164 = 0;
            break;
        case 2:
            pW->Unk164 = 1;
            break;
        default:
            pW->Unk164 = pW->Unk166 + 2;
            break;
        }
        pW->Unk166 = -0x8000;
    }
    ((UCHAR *)pParts)[0x80] = D;
    ((UCHAR *)pParts)[0x81] = (ULONG)D >> 8;
    pSce->Data += 10;
    return 1;
}

ULONG Plc_ret(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 1;
    if (pW->Id < 0x10) {
        pW->Type &= 0xFFF;
    }
    pW->Unk120 = 0;
    pW->Damage_cnt = 0;
    pW->Type &= 0xAEFF;
    pSce->Data++;
    func_80034124(&pW->Col);
    return 1;
}

ULONG Plc_stop(SCE_TASK *pSce)
{
    ULONG R_no;

    R_no = *(ULONG *)&pSce->pWork->Routine_0;
    if (R_no == 0x1404) {
        *(ULONG *)&pSce->pWork->Routine_0 = pSce->R_no_bak;
    } else {
        pSce->R_no_bak = R_no;
        *(ULONG *)&pSce->pWork->Routine_0 = 0x1404;
    }
    pSce->Data++;
    return 1;
}

ULONG Plc_flg(SCE_TASK *pSce)
{
    ULONG Mode;
    USHORT v;

    Mode = pSce->Data[1];
    v = ((USHORT *)pSce->Data)[1];
    switch (Mode) {
    case 0:
        pSce->pWork->Unk144 |= v;
        break;
    case 1:
        pSce->pWork->Unk144 = v;
        break;
    case 2:
        pSce->pWork->Unk144 ^= v;
        break;
    }
    pSce->Data += 4;
    return 1;
}

ULONG Plc_rot(SCE_TASK *pSce)
{
    switch (pSce->Data[1]) {
    case 0:
        pSce->pWork->Add_dir = ((USHORT *)pSce->Data)[1];
        break;
    case 1:
        pSce->pWork->Unk14A = ((USHORT *)pSce->Data)[1];
        break;
    }
    pSce->Data += 4;
    return 1;
}

ULONG Plc_cnt(SCE_TASK *pSce)
{
    pSce->pWork->Move_cnt = pSce->Data[1];
    pSce->Data += 2;
    return 1;
}

ULONG Splc_ret(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 1;
    pW->pUnk114 = G_PLAYER;
    pW->Unk120 = 0;
    pW->Type = (pW->Type & 0xAEFF) | 0x400;
    G.Status_flg |= 0x80000000;
    pW->Life = G.Unk7852;
    pW->Max_life = 200;
    pSce->Data++;
    func_80034124(&pW->Col);
    return 1;
}

ULONG Splc_sce(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 0x604;
    pW->pUnk114 = pW;
    pW->Unk120 = 0x92;
    pW->Type = (pW->Type | 0x5000) & ~0x400;
    G.Status_flg &= 0x7FFFFFFF;
    pSce->Data++;
    return 1;
}

ULONG func_800574B8(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 0x604;
    pW->pUnk114 = pW;
    pW->Unk120 = 0x92;
    pW->Type |= 0x5000;
    pSce->Data++;
    return 1;
}

/* SCE opcode Item_aot_set (RE2 struct Item_aot_set; iItem is a byte here) */
typedef struct {
    UCHAR Opcode;                      /* 0x00 */
    UCHAR Aot;                         /* 0x01 G.Sce_aot index */
    UCHAR Id;                          /* 0x02 SCE_AOT starts here */
    UCHAR Type;                        /* 0x03 */
    UCHAR nFloor;                      /* 0x04 */
    UCHAR Super;                       /* 0x05 */
    SHORT X;                           /* 0x06 */
    SHORT Z;                           /* 0x08 */
    USHORT W;                          /* 0x0A */
    USHORT D;                          /* 0x0C */
    UCHAR iItem;                       /* 0x0E */
    UCHAR padF;
    USHORT nItem;                      /* 0x10 */
    USHORT Item_flg;                   /* 0x12 */
    UCHAR Om_no;                       /* 0x14 */
    UCHAR Action;                      /* 0x15 */
} SCE_ITEM_AOT_SET;

/* The two do/while(0) wrappers are needed for register allocation. */
ULONG Item_aot_set(SCE_TASK *pSce)
{
    LONG Unused[2];
    SCE_ITEM_AOT_SET *pData;
    ULONG Flg_bit;
    ULONG Om_no;
    UCHAR Aot;
    ULONG *pFlg;
    UCHAR Item;
    tagGLOBAL *pG;

    pData = (SCE_ITEM_AOT_SET *)pSce->Data;
    Flg_bit = pData->Item_flg;
    do {
        Om_no = pData->Om_no;
    } while (0);
    pSce->Data = (UCHAR *)(pData + 1);
    pG = &G;
    Aot = pData->Aot;
    if (pG->Sce_aot[Aot] == NULL) {
        pG->nSce_aot++;
    }
    pG->Sce_aot[Aot] = (struct tagSCE_AOT *)&pData->Id;
    if (pG->Unk783E >= 2 && !(pG->System_flg & 0x80)) {
        pFlg = D_8009E418;
    } else {
        pFlg = D_8009E414;
    }
    if (G.System_flg & 0x100) {
        Item = pData->iItem;
        if (Item == 0x81) {
            do {
                Flg_on(pFlg, Flg_bit);
            } while (0);
        }
        if ((ULONG)(Item - 0x15) < 0xB) {
            pData->nItem <<= 1;
        }
    }
    if (Flg_ck(pFlg, Flg_bit)) {
        *(UCHAR *)pG->Sce_aot[Aot] = 0;
        if (Om_no < 0x20) {
            pG->Ob_model[Om_no].Be_flg = 0x80000000;
            pG->Ob_model[Om_no].UnkC3 = 0;
        }
    } else if (Om_no < 0x20) {
        Aot = pData->Action;
        pG->Ob_model[Om_no].UnkC3 = Aot;
        if (Aot & 2) {
            pG->Ob_model[Om_no].Be_flg = 0x80000000;
        }
    }
    return 1;
}

/* SCE opcode Item_aot_set_4p (RE2 struct Item_aot_set_4p; iItem/nItem are bytes here) */
typedef struct {
    UCHAR Opcode;                      /* 0x00 */
    UCHAR Aot;                         /* 0x01 G.Sce_aot index */
    UCHAR Id;                          /* 0x02 SCE_AOT starts here */
    UCHAR Type;                        /* 0x03 */
    UCHAR nFloor;                      /* 0x04 */
    UCHAR Super;                       /* 0x05 */
    SHORT Xz[4][2];                    /* 0x06 */
    UCHAR iItem;                       /* 0x16 */
    UCHAR pad17;
    UCHAR nItem;                       /* 0x18 */
    UCHAR pad19;
    USHORT Item_flg;                   /* 0x1A */
    UCHAR Om_no;                       /* 0x1C */
    UCHAR Action;                      /* 0x1D */
} SCE_ITEM_AOT_SET_4P;

/* The do/while(0) wrappers are needed for register allocation (loop-depth weighting). */
ULONG Item_aot_set_4p(SCE_TASK *pSce)
{
    LONG Unused[2];
    SCE_ITEM_AOT_SET_4P *pData;
    ULONG Flg_bit;
    ULONG Om_no;
    UCHAR Aot;
    ULONG *pFlg;
    UCHAR Item;
    tagGLOBAL *pG;

    pData = (SCE_ITEM_AOT_SET_4P *)pSce->Data;
    do {
        pData->Type |= 0x80;
        Flg_bit = pData->Item_flg;
        Om_no = pData->Om_no;
    } while (0);
    pSce->Data = (UCHAR *)(pData + 1);
    pG = &G;
    Aot = pData->Aot;
    if (pG->Sce_aot[Aot] == NULL) {
        pG->nSce_aot++;
    }
    pG->Sce_aot[Aot] = (struct tagSCE_AOT *)&pData->Id;
    if (pG->Unk783E >= 2 && !(pG->System_flg & 0x80)) {
        pFlg = D_8009E418;
    } else {
        pFlg = D_8009E414;
    }
    if (G.System_flg & 0x100) {
        Item = pData->iItem;
        if (Item == 0x81) {
            do {
                Flg_on(pFlg, Flg_bit);
            } while (0);
        }
        if ((ULONG)(Item - 0x15) < 0xB) {
            pData->nItem <<= 1;
        }
    }
    if (Flg_ck(pFlg, Flg_bit)) {
        do {
        *(UCHAR *)pG->Sce_aot[Aot] = 0;
        } while (0);
        if (Om_no < 0x20) {
            pG->Ob_model[Om_no].Be_flg = 0x80000000;
            pG->Ob_model[Om_no].UnkC3 = 0;
        }
    } else if (Om_no < 0x20) {
        Aot = pData->Action;
        pG->Ob_model[Om_no].UnkC3 = Aot;
        if (Aot & 2) {
            pG->Ob_model[Om_no].Be_flg = 0x80000000;
        }
    }
    return 1;
}

/* RE2 Kage_set */
ULONG Kage_set(SCE_TASK *pSce)
{
    KAGE_WORK9 *pK;
    UCHAR *pData;
    ULONG r, g, b;

    pData = pSce->Data;
    switch (pData[1]) {
    case 1:
        pK = G.pEnemy[0]->Kage.pKw9;
        break;
    case 2:
        pK = G.pEnemy[1]->Kage.pKw9;
        break;
    case 3:
        pK = G.pEnemy[(s8)pData[2] + 2]->Kage.pKw9;
        break;
    }
    r = pSce->Data[3];
    g = pSce->Data[4];
    b = pSce->Data[5];
    if (pK->Be_flg & 2) {
        Kage_work9_color_set(pK, r | (g << 8) | (b << 16));
    } else {
        Kage_work_color_set_((KAGE_WORK *)pK, r | (g << 8) | (b << 16));
    }
    pK->Half_x = ((USHORT *)pSce->Data)[3];
    pK->Half_z = ((USHORT *)pSce->Data)[4];
    pK->Off_x = ((USHORT *)pSce->Data)[5];
    pK->Off_z = ((USHORT *)pSce->Data)[6];
    pSce->Data += 14;
    return 1;
}

/* RE2 Super_set */
ULONG Super_set(SCE_TASK *pSce)
{
    UCHAR *pData;
    ENEMY_WORK *pEm;
    ULONG Sup_val;
    ULONG Sup_no;
    ENEMY_WORK *pT;
    OBJ_MODEL_WORK *pOm;
    LONG x, y, z;
    tagGLOBAL *pG = &G;

    pData = pSce->Data;
    pEm = pSce->pWork;
    Sup_val = pData[2];
    Sup_no = pData[3];
    switch (Sup_val & 0xC0) {
    case 0:
        pEm->pSuper = &GsIDMATRIX;
        break;
    case 0x40:
        if (Sup_no & 0x80) {
            pEm->pSuper = &pG->Player.pSin_parts_ptr[Sup_no & 0x7F].Workm;
        } else {
            pEm->pSuper = (MATRIX *)pG->Player.Workm_m;
        }
        break;
    case 0x80:
        if (Sup_no & 0x80) {
            pEm->pSuper = &pG->pEnemy[Sup_val - 0x80]->pSin_parts_ptr[Sup_no & 0x7F].Workm;
        } else {
            pEm->pSuper = (MATRIX *)pG->pEnemy[Sup_val - 0x80]->Workm_m;
        }
        break;
    case 0xC0:
        if (Sup_no & 0x80) {
            pOm = &pG->Ob_model[Sup_val - 0xC0];
            pEm->pSuper = &pOm->pParts[Sup_no & 0x7F].Workm;
        } else {
            pEm->pSuper = &pG->Ob_model[Sup_val - 0xC0].M;
        }
        break;
    }
    x = ((SHORT *)pData)[2];
    y = ((SHORT *)pData)[3];
    z = ((SHORT *)pData)[4];
    pEm->Pos_x = x;
    pEm->Pos_y = y;
    pEm->Pos_z = z;
    x = ((SHORT *)pData)[5];
    y = ((SHORT *)pData)[6];
    z = ((SHORT *)pData)[7];
    pEm->Cdir_x = x;
    pEm->Cdir_y = y;
    pEm->Cdir_z = z;
    pSce->Data = pData + 0x10;
    return 1;
}

ULONG Weapon_chg(SCE_TASK *pSce)
{
    ENEMY_WORK *pPl;
    ULONG Wep;
    ULONG Flg;

    Wep = pSce->Data[1];
    pPl = G_PLAYER;
    Flg = Wep & 0x80;
    if (Wep == 0xFF) {
        G.pUnk7C7C->Unk128 = 0xFF;
        G.pUnk7C7C->Unk129 = 0;
        pSce->Data += 2;
        return 1;
    } else {
        Wep &= 0x7F;
        pPl->Type = (pPl->Type & 0xFF00) | Wep;
        if (Wep != 0) {
            G.pUnk7C7C->Unk128 = Search_item_id_(Wep, 0);
            G.pUnk7C7C->Unk129 = Wep;
            Weapon_cls(pPl);
            Pl_weapon_ch(pPl);
            D_800E0171[0] = 1;
        } else {
            G.pUnk7C7C->Unk128 = 0xFF;
            G.pUnk7C7C->Unk129 = 0;
            if (Flg) {
                Weapon_cls(pPl);
                Pl_weapon_ch(pPl);
                Flg_on(D_800E01D4, 1);
                D_800E0171[0] = 1;
            } else {
                Weapon_parts_clr(pPl);
                D_800E0171[0] = 0;
            }
        }
    }
    pSce->Data += 2;
    return 1;
}

ULONG func_80057CF8(SCE_TASK *pSce)
{
    ENEMY_WORK *pPl;
    ULONG Wep;

    pPl = G_PLAYER;
    Wep = D_800E01E8;
    pPl->Type = (pPl->Type & 0xFF00) | Wep;
    if (Wep != 0) {
        G.pUnk7C7C->Unk128 = Search_item_id_(Wep, 0);
        G.pUnk7C7C->Unk129 = Wep;
    } else {
        G.pUnk7C7C->Unk128 = 0xFF;
        G.pUnk7C7C->Unk129 = 0;
    }
    if (Sce.Unk1489 != 0) {
        Weapon_cls(pPl);
        Pl_weapon_ch(pPl);
        Sce.Unk1489 = 0;
    } else {
        Pl_weapon_ch_init(pPl);
    }
    pSce->Data++;
    return 1;
}

ULONG func_80057DD8(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    PARTS_W *p0;
    PARTS_W *p1;
    MD_OBJ *pTmd;

    pW = pSce->pWork;
    p0 = &pW->pSin_parts_ptr[4];
    p1 = &pW->pSin_parts_ptr[15];
    pTmd = p0->pTmd;
    p0->pTmd = p1->pTmd;
    p1->pTmd = pTmd;
    pW->Unk4B ^= 1;
    pSce->Data++;
    return 1;
}

ULONG func_80057E28(SCE_TASK *pSce)
{
    D_800E01C4[0] = 0;
    Game.Unk1 = 1;
    G.Status_flg |= 0x200;
    Game.Stop_bak = G.Unk2468;
    pSce->Data++;
    return 1;
}

ULONG Flr_set(SCE_TASK *pSce)
{
    UCHAR *pData;
    USHORT *pFlr;
    USHORT *pAttr;
    UCHAR *p;
    ULONG n;

    pData = pSce->Data;
    pFlr = G.pRoom->pUnk34;
    n = pData[1];
    if (n < pFlr[0]) {
        p = (UCHAR *)pFlr;
        p += n * 12;
        pAttr = (USHORT *)(p + 12);
        p = pData;
        if (p[2] == 1) {
            *pAttr &= ~0x100;
        } else {
            *pAttr |= 0x100;
        }
    }
    pSce->Data += 3;
    return 1;
}

ULONG Sce_Item_lost(SCE_TASK *pSce)
{
    func_8006CD68(Search_item_id_(pSce->Data[1], 0));
    pSce->Data += 2;
    return 1;
}

ULONG func_80057F44(SCE_TASK *pSce)
{
    UCHAR v;

    v = pSce->Data[1];
    G.Status_flg |= 0x10000000;
    D_800C703E[0] = v;
    pSce->Data += 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/sce2", func_80057F84);

ULONG Sce_parts_bomb(SCE_TASK *pSce)
{
    UCHAR *pData;
    PARTS_W *pP;
    ULONG Rgb;

    pData = pSce->Data;
    pP = &pSce->pWork->pSin_parts_ptr[pData[2]];
    pP->UnkA2 |= 0x20;
    pP->Be_flg |= 2;
    Rgb = pData[3] | (pData[4] << 8) | (pData[5] << 16);
    ((SHORT *)&pP->Unk74)[4] = 0;
    pP->Poly_rgb = Rgb;
    ((SHORT *)&pP->Unk74)[0] = ((USHORT *)pData)[3];
    ((SHORT *)&pP->Unk74)[2] = ((USHORT *)pData)[4];
    ((SHORT *)&pP->Unk74)[6] = ((USHORT *)pData)[5];
    ((SHORT *)&pP->Unk74)[1] = ((USHORT *)pData)[6];
    ((SHORT *)&pP->Unk74)[3] = ((USHORT *)pData)[7];
    pSce->Data += 16;
    return 1;
}

ULONG Sce_parts_down(SCE_TASK *pSce)
{
    UCHAR *pData;
    PARTS_W *pP;

    pData = pSce->Data;
    pP = &pSce->pWork->pSin_parts_ptr[pData[1]];
    pP->Unk96 = 0;
    pP->Be_flg |= 0xA;
    pP->Old_x = ((USHORT *)pData)[1];
    pP->Old_z = ((USHORT *)pData)[2];
    pP->Old_y = ((USHORT *)pData)[3];
    pP->Unk85 = pData[8];
    pP->Unk90 = ((USHORT *)pData)[5];
    pP->Unk92 = ((USHORT *)pData)[6];
    pP->Unk8E = ((USHORT *)pData)[7];
    pSce->Data += 16;
    return 1;
}

ULONG func_8005879C(SCE_TASK *pSce)
{
    switch (pSce->Data[1]) {
    case 0x34:
        D_800D1F8E[0] = 0x320;
        break;
    case 0x35:
        D_800D1F8E[0] = 0x320;
        break;
    case 0x36:
        D_800D1F8E[0] = 0x320;
        break;
    }
    pSce->Data += 2;
    return 1;
}

ULONG func_80058800(SCE_TASK *pSce)
{
    UCHAR *pData;
    UCHAR *p;
    UCHAR *q;
    UCHAR *pHdr;
    ULONG n;
    ULONG k;

    pData = pSce->Data;
    pSce->Data = pData + 4;
    if (G.Unk2470 != 0) {
        p = G.pRoom->pUnk40;
        if (((USHORT *)p)[1] <= G.Cut_no) {
            return 1;
        }
        pHdr = p + ((ULONG *)p)[pData[1] + 1];
        k = pData[2];
        n = *(USHORT *)pHdr;
        q = pHdr + 4;
        for (; n != 0; n--, k--) {
            if (k == 0) {
                if (pData[3] != 0) {
                    q[0] &= 0xFD;
                } else {
                    q[0] |= 2;
                }
                break;
            }
            if (q[1] == 0) {
                q += 0xC;
            } else {
                q += 0x14;
            }
        }
    }
    return 1;
}

void func_800588F0(SCE_TASK *pSce)
{
    D_800D442C_[0] = pSce->Data[1];
    pSce->Data += 2;
}

/* RE2 Parts_set */
ULONG Parts_set(SCE_TASK *pSce)
{
    UCHAR *pData;
    LONG Kind;
    LONG No;
    USHORT Val;
    PARTS_W *pParts;

    pData = pSce->Data;
    Kind = (s8)pData[3];
    No = (s8)pData[2];
    Val = ((USHORT *)pData)[2];
    if ((ULONG)Kind >= 10) {
        pParts = ((OBJ_MODEL_WORK *)pSce->pWork)->pParts + No;
    } else {
        pParts = pSce->pWork->pSin_parts_ptr + No;
    }
    switch (Kind) {
    case 2:
        *(USHORT *)&pParts->Be_flg = Val;
        break;
    case 3:
        *((USHORT *)&pParts->Be_flg + 1) = Val;
        break;
    case 4:
        pParts->Cdir_x = Val;
        break;
    case 5:
        pParts->Cdir_y = Val;
        break;
    case 6:
        pParts->Cdir_z = Val;
        break;
    case 7:
        pParts->Unk90 = Val;
        break;
    case 8:
        pParts->Unk92 = Val;
        break;
    case 9:
        pParts->Unk8E = Val;
        break;
    case 0:
    case 10:
        *(USHORT *)&pParts->Poly_rgb = Val;
        break;
    case 1:
    case 11:
        ((UCHAR *)&pParts->Poly_rgb)[2] = Val;
        break;
    }
    pSce->Data += 6;
    return 1;
}

ULONG func_800589FC(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    ULONG i;
    LONG nFloor;

    i = pSce->Data[1] + 2;
    pW = G.pEnemy[i];
    nFloor = pW->nFloor;
    pW->Be_flg = 1;
    *(ULONG *)&pW->Routine_0 = 0;
    pW->Status_flg = 0;
    pW->Damage_cnt = 0;
    pW->UnkC6 = 0;
    pW->Timer0 = 0;
    pW->UnkDA = 0;
    pW->UnkE2 = 0;
    pW->UnkE3 = 0;
    pW->Unk122 = -(nFloor * 1800);
    if (pW->Kage.pKw != NULL) {
        pW->Kage.pKw->Be_flg = 0;
    }
    pW->Kage.pKw = NULL;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    pW->Unk14C = 0;
    pW->Unk14E = 0;
    pW->Water = 0;
    pW->Unk1C2 = 0;
    pW->Unk1C1 = 0;
    pW->Unk1C0 = 0;
    pW->Unk1BC[1] = 0;
    pW->Unk1BC[0] = 0;
    pW->Unk1BA = 0;
    pW->Unk1B8 = 0;
    pW->Unk12A = 0x1000;
    pW->Unk1CA = 0;
    pW->Unk1C8 = 0;
    pW->Unk132 = 0;
    pW->Unk158 = 0;
    func_800261AC(pW, 0, pW->pUnk10C);
    pSce->Data += 2;
    return 1;
}

ULONG func_80058B00(SCE_TASK *pSce)
{
    UCHAR i;
    tagGLOBAL *pG = &G;

    if ((pG->Status_flg & 0x40000000) == 0) {
        pSce->Data += 4;
        return 1;
    }
    for (i = 0; i < 8; i++) {
        if ((USHORT)pG->Unk7E24[i] == 0) {
            pG->Unk7E24[i] = ((USHORT *)pSce->Data)[1];
            break;
        }
    }
    pSce->Data += 4;
    return 1;
}

ULONG func_80058B84(SCE_TASK *pSce)
{
    tagGLOBAL *pG;
    ULONG Key;

    pSce->Data++;
    pG = &G;
    if (pG->Player.Damage_cnt == 0) {
        Key = pG->Key;
        if (!(Key & 0x100) || (UCHAR)pG->Player.Type == 0) {
            if (Key & 0x80) {
                if (!(pG->Status_flg & 0x100)) {
                    if (!(D_800DBB58 & 0xC0) && D_800C7961 == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

ULONG func_80058C24(SCE_TASK *pSce)
{
    Quake_add_(((USHORT *)pSce->Data)[1], ((USHORT *)pSce->Data)[2]);
    pSce->Data += 6;
    return 1;
}

ULONG func_80058C70(SCE_TASK *pSce)
{
    ULONG x;

    St.St_type = 4;
    Game.Stop_bak = G.Unk2468;
    x = pSce->Data[2];
    G.Status_flg |= 0x200;
    Game.Unk1 = 1;
    St.UnkB8 = x;
    pSce->Data += 4;
    return 1;
}

ULONG func_80058CD0(SCE_TASK *pSce)
{
    UCHAR i;

    switch (D_800D1F94) {
    case 3:
    case 4:
        for (i = 0; i < 10; i++) {
            Flg_off_(D_800D258C, i + 0x50);
        }
        break;
    case 5:
        for (i = 0; i < 0x14; i++) {
            Flg_off_(D_800D258C, i + 0x61);
        }
        break;
    case 6:
        for (i = 0; i < 7; i++) {
            Flg_off_(D_800D258C, i + 0x5A);
        }
        break;
    }
    pSce->Data++;
    return 1;
}
