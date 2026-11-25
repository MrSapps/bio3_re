#include "sub.h"
#include "player.h"
#include "status.h"
#include "joint.h"
#include "md_sub.h"
#include "pl_wep.h"
#include "sce2.h"
#include "sce_at.h"
#include "sld.h"

int Kage_work_color_set_() __asm__("Kage_work_color_set"); /* Capcom called it undeclared here (implicit int); Kage_set differs with the void prototype */

extern UCHAR Ctrl_mode_a[] __asm__("Ctrl_mode"); /* stored through an array: pSce->Data is reloaded after it */
s8 Search_item_id_(UCHAR Id, UCHAR A1) __asm__("Search_item_id"); /* Item_get_ck passes the start slot unsigned (andi, not sll/sra) */
LONG Quake_add_() __asm__("Quake_add"); /* Capcom called it undeclared here (implicit int); Sce_quake_add differs with the void prototype */
LONG Flg_off_(ULONG *pFlg, ULONG Bit) __asm__("Flg_off"); /* int-returning here (implicit declaration in Capcom's file); Chaser_evt_clr differs with void */

/* opcode records read by field (layouts as in the SCD data) */
typedef struct { UCHAR Op; UCHAR pad1; UCHAR Sup_val; UCHAR Sup_no; SHORT Pos[3]; SHORT Dir[3]; } SCE_SUPER_REC;
typedef struct { UCHAR Op; UCHAR Parts; USHORT Old_x; USHORT Old_z; USHORT Old_y; UCHAR Timer1; UCHAR pad9;
                 USHORT Rot_spd_x; USHORT Rot_spd_y; USHORT Rot_spd_z; } SCE_PARTS_DOWN_REC; /* PARTS_W fields it sets */
typedef struct { UCHAR Op; UCHAR pad1; UCHAR Parts; UCHAR R; UCHAR G; UCHAR B; USHORT W[5]; } SCE_PARTS_BOMB_REC;
typedef struct { UCHAR Op; s8 Aot; UCHAR Id; UCHAR Type; SHORT X; SHORT Y; SHORT Z; } SCE_AOT_RESET_REC;

static inline ULONG Sce_skip(SCE_TASK *pSce, LONG n)
{
    pSce->Data += n;
    return 1;
}

/* true when Data[2] of item Data[1] fit an existing stack or a free slot */
ULONG Item_get_ck(SCE_TASK *pSce)
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
        p = &G.pItem_set->Item[i];
        if (Item_data_tbl[p->Id].Max >= p->Num + Num) {
            return 1;
        }
        i++;
    }
    i = Search_item_id_(0, 0);
    return i != -1;
}

/* RE2 Dir_ck */
ULONG Sce_dir_ck(SCE_TASK *pSce)
{
    SHORT *pData;

    pData = (SHORT *)(pSce->Data += 8);
    return Goto00_ck(pSce->pWork, pData[-3], pData[-2], pData[-1]) == 0;
}

/* sets System_flg 0x20000 (die demo on) */
ULONG Diedemo_on(SCE_TASK *pSce)
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

/* RE2 Door_aot_set_4p: registers the 4-point door AOT that follows as G.Sce_aot[Data[1]] */
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

/* RE2 Aot_set: registers the AOT that follows as G.Sce_aot[Data[1]] */
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

/* RE2 Aot_set_4p: registers the 4-point AOT that follows as G.Sce_aot[Data[1]] */
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
    SCE_AOT_RESET_REC *pData;
    UCHAR *pAot;
    ULONG N;
    LONG Y, Z;

    pData = (SCE_AOT_RESET_REC *)pSce->Data;
    pAot = (UCHAR *)G.Sce_aot[pData->Aot];
    N = pAot[1];
    pAot[0] = pData->Id;
    pAot[1] = pData->Type | (N & 0x80);
    if (N & 0x80) {
        pAot += 0x14;
    } else {
        pAot += 0xC;
    }
    Y = pData->Y;
    N = pData->X;
    Z = pData->Z;
    ((SHORT *)pAot)[1] = Y;
    ((SHORT *)pAot)[0] = N;
    ((SHORT *)pAot)[2] = Z;
    pSce->Data = (UCHAR *)(pData + 1);
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

/* RE2 Get_matrix: matrix of a work (player, enemy, object model or part) by kind and number */
MATRIX *Get_matrix(s8 Work_kind, s8 Work_no)
{
    LONG Kind;
    ENEMY_WORK *pEm;
    PARTS_W *pP;

    Kind = (s8)Work_kind;
    if (!(Kind & 0x80)) {
        switch (Kind) {
        case 0:
            return &GsIDMATRIX;
        case 1:
            return EM_WORKM(G.pEnemy[0]);
        case 2:
            return EM_WORKM(G.pEnemy[1]);
        case 3:
            return EM_WORKM(G.pEnemy[2 + Work_no]);
        case 4:
            return &G.Ob_model[Work_no].M;
        case 6:
            return NULL;
        }
    } else {
        switch ((Work_kind & 0x60) >> 5) {
        case 0:
            pEm = G.pEnemy[0];
            break;
        case 1:
            pEm = G.pEnemy[1];
            break;
        case 2:
            pEm = G.pEnemy[2 + Work_no];
            break;
        case 3:
            pP = &G.Ob_model[Work_no].pParts[Work_kind & 0x1F];
            return &pP->Workm;
        }
        pP = &pEm->pSin_parts_ptr[Work_kind & 0x1F];
        return &pP->Workm;
    }
    /* kind 5 and >= 7 fall off the end (no return value), like RE2 */
}

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

/* RE2 Sce_espr_kill: kills effect (Data[2] << 8 | Data[1]) attached to the matrix of (Data[3], Data[4]) */
ULONG Sce_espr_kill(SCE_TASK *pSce)
{
    MATRIX *pM;

    pM = Get_matrix((s8)pSce->Data[3], (s8)pSce->Data[4]);
    Esp_kill((pSce->Data[2] << 8) | pSce->Data[1], pM);
    pSce->Data += 5;
    return 1;
}

/* int-returning view of the void Esp_kill: the caller's register allocation needs v0 clobbered */
LONG func_8001B894_l(ULONG Id, MATRIX *pM) __asm__("Esp_kill"); /* returns long here (canonical void) */

/* RE2 Sce_espr_kill2: kills the effects of group Data[1] */
ULONG Sce_espr_kill2(SCE_TASK *pSce)
{
    func_8001B894_l((pSce->Data[1] << 16) | 0x1000000, 0);
    pSce->Data += 2;
    return 1;
}

/* RE2 Sce_espr_task: stops every effect attached to the matrix of (Data[1], Data[2]); 6 = all */
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
        Esp_kill(0x2000000, 0);
    }
    pSce->Data += 3;
    return 1;
}

/* RE2 Sce_espr_control: sends control values to effect (Data[2] << 8 | Data[1]) of a work matrix */
ULONG Sce_espr_control(SCE_TASK *pSce)
{
    UCHAR *pData;

    pData = pSce->Data;
    Esp_ctrl((pData[2] << 8) | pData[1], Get_matrix((s8)pData[3], (s8)pData[4]),
                  pData[5] | (pData[6] << 16), pData[7] | (pData[8] << 16));
    pSce->Data = pData + 10;
    return 1;
}

/* RE2 Obj_model_set: sets up a room object model (OBJ_MODEL_WORK) from the opcode record */
/* Obj_model_set opcode record (0x28 bytes; RE2 is 0x26, no Flg_no / Cc fields) */
typedef struct tagSCE_OM_SET {
    UCHAR Opcode;                      /* 0x00 */
    UCHAR Om_no;                       /* 0x01 */
    UCHAR Id;                          /* 0x02 */
    UCHAR Cc_ctr;                      /* 0x03 0x40 texture change, 0x20 uv change, low 5 bits Cc_ctr */
    USHORT Cc_wait;                    /* 0x04 high byte: Cc_wait */
    USHORT Cc_num;                     /* 0x06 high byte: Cc_num (uv change: low nibble count, top nibble start) */
    UCHAR nFloor;                      /* 0x08 */
    UCHAR Super;                       /* 0x09 0xC0: parent kind, 0x3F: parent number */
    UCHAR Type;                        /* 0x0A */
    UCHAR Flg_no;                      /* 0x0B G.Om_flg bit (0xFF none) */
    USHORT Be_flg;                     /* 0x0C */
    SHORT Attribute;                   /* 0x0E */
    SHORT Pos_x;                       /* 0x10 */
    SHORT Pos_y;                       /* 0x12 */
    SHORT Pos_z;                       /* 0x14 */
    SHORT Cdir_x;                      /* 0x16 */
    SHORT Cdir_y;                      /* 0x18 */
    SHORT Cdir_z;                      /* 0x1A */
    SHORT Ofs_x;                       /* 0x1C */
    SHORT Ofs_y;                       /* 0x1E */
    SHORT Ofs_z;                       /* 0x20 */
    SHORT W;                           /* 0x22 */
    SHORT H;                           /* 0x24 */
    SHORT D;                           /* 0x26 */
} SCE_OM_SET;

ULONG Obj_model_set(SCE_TASK *pSce)
{
    tagGLOBAL *pG;
    SCE_OM_SET *pData;
    OBJ_MODEL_WORK *pOm;
    CC_PARTS_WORK *pCc;
    OM_TARGET *pT;
    ULONG *pTmd;
    ULONG Om_no;
    ULONG Sup_flg;
    LONG Work_a;
    LONG Work_b;
    LONG Work_c;
    LONG Work_d;
    ULONG *pFlg;
    ULONG Old_be_flg;

    pG = &G;
    pData = (SCE_OM_SET *)pSce->Data;
    pFlg = pG->Om_flg;
    Om_no = pData->Flg_no;
    if (Om_no == 0xFF || !Flg_ck(pFlg, Om_no)) {
        Om_no = pData->Om_no;
        Work_a = pData->Id;
        Work_b = pData->nFloor;
        Work_c = pData->Type;
        Work_d = pData->Flg_no;
        pOm = pG->Ob_model + Om_no;
        Old_be_flg = pOm->Be_flg;
        pOm->Unk4A = Work_a;
        pOm->nFloor = Work_b;
        pOm->Type = Work_c;
        pOm->UnkC2 = Work_d;
        Sup_flg = pData->Super;
        if (Old_be_flg) {
            pOm->Be_flg = 0;
        } else {
            pOm->Be_flg = pData->Be_flg | 1;
        }
        if (G.pObj_work_end < pOm + 1) {
            G.pObj_work_end = pOm + 1;
        }
        Work_a = pData->Attribute;
        Work_b = pData->Pos_x;
        Work_c = pData->Pos_y;
        Work_d = pData->Pos_z;
        pOm->Attribute = Work_a;
        pOm->M.t[0] = Work_b;
        pOm->M.t[1] = Work_c;
        pOm->M.t[2] = Work_d;
        if (Work_a & 0x20) {
            pOm->Be_flg &= ~1;
        }
        Work_a = pData->Cdir_x;
        Work_b = pData->Cdir_y;
        Work_c = pData->Cdir_z;
        pOm->Cdir.vx = Work_a;
        pOm->Cdir.vy = Work_b;
        pOm->Cdir.vz = Work_c;
        Work_a = pData->Ofs_x;
        Work_b = pData->Ofs_y;
        Work_c = pData->Ofs_z;
        pOm->Col.Unk24 = Work_a;
        pOm->Col.Unk26 = Work_b;
        pOm->Col.Unk28 = Work_c;
        Work_a = pData->W;
        Work_b = pData->H;
        Work_c = pData->D;
        pOm->Parts.Be_flg = 1;
        pOm->UnkD = 0;
        pOm->Col.W0 = Work_a;
        pOm->Col.H = Work_b;
        pOm->Col.D0 = Work_c;
        switch (Sup_flg & 0xC0) {
        case 0:
            pOm->pSuper = &GsIDMATRIX;
            break;
        case 0x40:
            pOm->pSuper = EM_WORKM(&pG->Player);
            break;
        case 0x80:
            pOm->pSuper = EM_WORKM(pG->pEnemy[Sup_flg & 0x3F]);
            break;
        case 0xC0:
            pOm->pSuper = &pG->Ob_model[Sup_flg & 0x3F].M;
            break;
        }
        pTmd = pG->pRoom->pOmodel[Om_no].Tmd_adr;
        if (pTmd != (ULONG *)pG->pRoom) {
            Om_init((ENEMY_WORK *)pOm, pTmd); /* the model work shares the ENEMY_WORK head */
        } else {
            pOm->Be_flg = 0;
        }
        RotMatrix(&pOm->Cdir, &pOm->M);
        CompM(pOm->pSuper, &pOm->M, &pOm->Workm);
        Work_a = pData->Cc_ctr;
        Work_b = pData->Cc_wait;
        Work_c = pData->Cc_num;
        pCc = &Cc_work.Cc_parts[Cc_work.Ccol_no];
        pCc->Cc_flg = 0;
        if (Work_a & 0x40) {
            pCc->Cc_flg = 0x40;
            pCc->Cc_ctr = Work_a & 0x1F;
            pCc->Cc_wait = Work_b >> 8;
            pCc->Cc_num = Work_c >> 8;
            pCc->Cc_cnt = 0;
            pCc->pOm = pOm;
            Tex_chg_init(Cc_work.Ccol_no, pCc);
        }
        if (Work_a & 0x20) {
            pCc->Cc_wait = Work_b >> 8;
            pCc->Cc_num = (Work_c >> 8) & 0xF;
            pCc->Cc_cnt = Work_c >> 12;
            pCc->pOm = pOm;
            pCc->Cc_flg |= 0x20;
            Uv_chg_init(Cc_work.Ccol_no, pCc);
        }
        if (Work_a & 0xE0) {
            Cc_work.Ccol_no++;
        }
        if (pOm->Unk4A != 1) {
            pOm->Col.pEm = (ENEMY_WORK *)pOm;
            if (pOm->Attribute & 0x4000) {
                pOm->Col.Flg = 1;
            } else {
                pOm->Col.Flg = 0;
            }
            if (pOm->Type & 0x40) {
                pOm->Col.Type = 4;
            } else {
                pOm->Col.Type = 3;
            }
            Oba_set(&pOm->Col, 0);
        }
        if (pOm->Unk4A & 4) {
            pT = (OM_TARGET *)G.pEm_mem;
            G.pOm_target[G.Om_target_num] = pT;
            G.pEm_mem = (ENEMY_WORK *)((UCHAR *)G.pEm_mem + sizeof(OM_TARGET));
            pT->Area.Bottom = -1800;
            pT->Area.Rate1 = pT->Area.Rate0 = 0x40;
            pT->Area.R = 400;
            pT->Flg = 0;
            pT->pOm = pOm;
            pT->Area.Top = 0;
            pT->Area.pM = &pOm->Workm;
            pT->Unk14 = 0;
            pOm->UnkC1 = G.Om_target_num++;
        }
    }
    pSce->Data = (UCHAR *)(pData + 1);
    return 1;
}

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
extern SCE_EM_SET *pSce_em_set;          /* operand of the em_set being run */

/* RE2 Sce_em_set: creates enemy work Data Work_no + 2 from the Em_set record unless its flag is set */
ULONG Sce_em_set(SCE_TASK *pSce)
{
    tagGLOBAL *pG = &G;
    ULONG *pFlg;
    ENEMY_WORK *pEm;
    LONG No;
    ENEMY_WORK *pNext;

    pSce_em_set = (SCE_EM_SET *)pSce->Data;
    if (pG->Stage_no >= 2 && !(pG->System_flg & 0x80))
        pFlg = pG->Enemy_flg2;
    else
        pFlg = pG->Enemy_flg;
    if (pSce_em_set->Flg_no == 0xFF || !Flg_ck(pFlg, pSce_em_set->Flg_no)) {
        if (G.Player.Id < 8) {
            if ((pSce_em_set->Id == 0x5B || pSce_em_set->Id == 0x5F) && G.Player.Id >= 2)
                pSce_em_set->Id = G.Player.Id + 0x60;
        } else {
            if ((pSce_em_set->Id == 0x5B || pSce_em_set->Id == 0x5F) && G.Pl_id_bak >= 2)
                pSce_em_set->Id = G.Pl_id_bak + 0x60;
        }
        No = pSce_em_set->Work_no + 2;
        if (No != 1)
            pG->Em_num++;
        pEm = pG->pEnemy[No] = pG->pEm_mem;
        pG->pEm_mem = (ENEMY_WORK *)((UCHAR *)pG->pEm_mem + 0x1FC);
        if (pG->ppEnemy_end < &pG->pEnemy[No + 1])
            pG->ppEnemy_end = &pG->pEnemy[No + 1];
        pNext = pG->pEm_mem;
        pEm->Be_flg = 1;
        pEm->Unk12F = pSce_em_set->Unk9;
        if (Sce.Em_set_kind0 != pSce_em_set->Unk9) {
            if (Sce.Em_set_kind0)
                Sce.Em_set_kind1 = pSce_em_set->Unk9;
            else
                Sce.Em_set_kind0 = pSce_em_set->Unk9;
        }
        *(ULONG *)&pEm->Routine_0 = 0;
        pEm->Pos_x = pEm->Old_x = pSce_em_set->Pos_x;
        pEm->Pos_y = pEm->Old_y = pSce_em_set->Pos_y;
        pEm->Pos_z = pEm->Old_z = pSce_em_set->Pos_z;
        pEm->Cdir_x = 0;
        pEm->Cdir_y = pSce_em_set->Cdir_y;
        pEm->Cdir_z = 0;
        pEm->Dest_x = 0;
        pEm->Dest_z = 0;
        pEm->Id = pSce_em_set->Id;
        pEm->Type = pSce_em_set->Type;
        pEm->Status_flg = pSce_em_set->Status_flg;
        pEm->Em_set_flg = pSce_em_set->Flg_no;
        pEm->Unk4B = pSce_em_set->UnkA;
        pEm->nFloor = pSce_em_set->nFloor;
        pEm->Ground = -(pSce_em_set->nFloor * 1800);
        pEm->Damage_cnt = 0;
        pEm->UnkC6 = 0;
        pEm->Timer0 = pSce_em_set->Timer0;
        pEm->UnkDA = pSce_em_set->UnkDA;
        pEm->Mot_scale_y = 0x1000;
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
        pEm->Tilt_max = 0;
        pEm->Tilt[0] = 0;
        pEm->Tilt[1] = 0;
        pEm->Unk1C0 = 0;
        pEm->Tilt_flg = 0;
        pEm->Tilt_spd = 0;
        pEm->Unk1CA = 0;
        pEm->Unk1C8 = 0;
        pEm->Damage_flg = 0;
        pEm->Unk158 = 0;
        pEm->Col.Id = 0;
        pEm->UnkD = 0;
        pEm->Root_ck_cnt = No & 7;
        pG->pEm_mem = pNext;
    }
    pSce->Data += 0x18;
    return 1;
}

/* RE2 Plc_motion: plays motion Data[2] of type Data[1] on the work, attribute Data[3] */
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
        pW->Sub_move_no = Move_no;
        pW->Sub_move_cnt = 0;
    }
    pW->Unk144 = Attr;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    pW->Type |= 0x100;
    pSce->Data += 4;
    return 1;
}

/* sets the work's destination mode (Data[1] | 0x80) and target (USHORT +2) in Unk150/Unk152 */
ULONG Plc_dest2(SCE_TASK *pSce)
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
    pEm->Set_flg_no = Set_no;
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

/* RE2 Plc_gun: puts the work in its aiming routine Data[1] */
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

/* RE2 Plc_gun_eff: muzzle flash positions (computed, unused) */
ULONG Plc_gun_eff(SCE_TASK *pSce)
{
    VECTOR Pos; /* the effect positions are set and never used (dead code left in the original) */

    Pos.vx = 140;
    Pos.vy = 605;
    Pos.vz = 0;
    Pos.vx = 145;
    Pos.vy = 500;
    Pos.vz = -25;
    Pos.vx = 145;
    Pos.vy = 265;
    Pos.vz = -50;
    pSce->Data++;
    return 1;
}

/* RE2 Plc_neck: neck/head look mode Data[1] with four SHORT parameters */
ULONG Plc_neck(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;
    ULONG Kind;
    PARTS_W *pParts;
    LONG A, B, C, D;

    pW = pSce->pWork;
    Kind = pSce->Data[1];
    pParts = pW->pSin_parts_ptr + 1;
    pW->Neck_flg = 0x80;
    A = ((SHORT *)pSce->Data)[1];
    B = ((SHORT *)pSce->Data)[2];
    C = ((SHORT *)pSce->Data)[3];
    D = ((SHORT *)pSce->Data)[4];
    switch (Kind) {
    case 0:
        pW->Neck_flg |= 0x12;
        break;
    case 2:
        pW->Neck_flg |= 0x8;
        break;
    case 3:
        pW->Neck_flg |= 0x2A;
        break;
    case 4:
        pW->Neck_flg |= 0x58;
        break;
    case 1:
    case 5:
        pW->Neck_flg |= 0x4;
        break;
    case 6:
        pW->Neck_flg = 0;
        break;
    case 7:
        pW->Neck_flg |= 0x1;
        break;
    case 8:
        pW->Neck_flg |= 0x12;
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

/* RE2 Plc_ret: returns the work to its normal routine and clears the scripted state */
ULONG Plc_ret(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 1;
    if (pW->Id < 0x10) {
        pW->Type &= 0xFFF;
    }
    pW->Neck_flg = 0;
    pW->Damage_cnt = 0;
    pW->Type &= 0xAEFF;
    pSce->Data++;
    Oba_hit_clr(&pW->Col);
    return 1;
}

/* RE2 Plc_stop: toggles the work between stopped (0x1404) and its saved routine */
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

/* RE2 Plc_flg: Unk144 |= / = / ^= (USHORT +2) for Data[1] 0/1/2 */
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

/* RE2 Plc_rot: sets Add_dir (Data[1] 0) or Unk14A (1) to (USHORT +2) */
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

/* RE2 Plc_cnt: sets the work's Move_cnt to Data[1] */
ULONG Plc_cnt(SCE_TASK *pSce)
{
    pSce->pWork->Move_cnt = pSce->Data[1];
    pSce->Data += 2;
    return 1;
}

/* RE2 Splc_ret: returns the sub player to its partner AI and restores its life */
ULONG Splc_ret(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 1;
    pW->pUnk114 = G_PLAYER;
    pW->Neck_flg = 0;
    pW->Type = (pW->Type & 0xAEFF) | 0x400;
    G.Status_flg |= 0x80000000;
    pW->Life = G.Spl_life;
    pW->Max_life = 200;
    pSce->Data++;
    Oba_hit_clr(&pW->Col);
    return 1;
}

/* RE2 Splc_sce: puts the sub player under script control (routine 0x604) */
ULONG Splc_sce(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 0x604;
    pW->pUnk114 = pW;
    pW->Neck_flg = 0x92;
    pW->Type = (pW->Type | 0x5000) & ~0x400;
    G.Status_flg &= 0x7FFFFFFF;
    pSce->Data++;
    return 1;
}

/* Splc_sce without changing Status_flg bit 31 */
ULONG Splc_sce2(SCE_TASK *pSce)
{
    ENEMY_WORK *pW;

    pW = pSce->pWork;
    *(ULONG *)&pW->Routine_0 = 0x604;
    pW->pUnk114 = pW;
    pW->Neck_flg = 0x92;
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
    if (pG->Stage_no >= 2 && !(pG->System_flg & 0x80)) {
        pFlg = Flg_addr_item2;
    } else {
        pFlg = Flg_addr_item;
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
    if (pG->Stage_no >= 2 && !(pG->System_flg & 0x80)) {
        pFlg = Flg_addr_item2;
    } else {
        pFlg = Flg_addr_item;
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
    SCE_SUPER_REC *pData;
    ENEMY_WORK *pEm;
    ULONG Sup_val;
    ULONG Sup_no;
    ENEMY_WORK *pT;
    OBJ_MODEL_WORK *pOm;
    LONG x, y, z;
    tagGLOBAL *pG = &G;

    pData = (SCE_SUPER_REC *)pSce->Data;
    pEm = pSce->pWork;
    Sup_val = pData->Sup_val;
    Sup_no = pData->Sup_no;
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
    x = pData->Pos[0];
    y = pData->Pos[1];
    z = pData->Pos[2];
    pEm->Pos_x = x;
    pEm->Pos_y = y;
    pEm->Pos_z = z;
    x = pData->Dir[0];
    y = pData->Dir[1];
    z = pData->Dir[2];
    pEm->Cdir_x = x;
    pEm->Cdir_y = y;
    pEm->Cdir_z = z;
    pSce->Data = (UCHAR *)(pData + 1);
    return 1;
}

/* RE2 Weapon_chg: equips weapon Data[1] (0xFF none, bit 7 keeps the model change) */
ULONG Weapon_chg(SCE_TASK *pSce)
{
    ENEMY_WORK *pPl;
    ULONG Wep;
    ULONG Flg;

    Wep = pSce->Data[1];
    pPl = G_PLAYER;
    Flg = Wep & 0x80;
    if (Wep == 0xFF) {
        G.pItem_set->Equip_no = 0xFF;
        G.pItem_set->Equip_id = 0;
        pSce->Data += 2;
        return 1;
    } else {
        Wep &= 0x7F;
        pPl->Type = (pPl->Type & 0xFF00) | Wep;
        if (Wep != 0) {
            G.pItem_set->Equip_no = Search_item_id_(Wep, 0);
            G.pItem_set->Equip_id = Wep;
            Weapon_cls(pPl);
            Pl_weapon_ch(pPl);
            Sce_weapon_chg_flg[0] = 1;
        } else {
            G.pItem_set->Equip_no = 0xFF;
            G.pItem_set->Equip_id = 0;
            if (Flg) {
                Weapon_cls(pPl);
                Pl_weapon_ch(pPl);
                Flg_on(St_flg14_a, 1);
                Sce_weapon_chg_flg[0] = 1;
            } else {
                Weapon_parts_clr(pPl);
                Sce_weapon_chg_flg[0] = 0;
            }
        }
    }
    pSce->Data += 2;
    return 1;
}

/* re-equips the weapon kept in St.Equip_id_old */
ULONG Weapon_chg_old(SCE_TASK *pSce)
{
    ENEMY_WORK *pPl;
    ULONG Wep;

    pPl = G_PLAYER;
    Wep = St_equip_id_old;
    pPl->Type = (pPl->Type & 0xFF00) | Wep;
    if (Wep != 0) {
        G.pItem_set->Equip_no = Search_item_id_(Wep, 0);
        G.pItem_set->Equip_id = Wep;
    } else {
        G.pItem_set->Equip_no = 0xFF;
        G.pItem_set->Equip_id = 0;
    }
    if (Sce.Weapon_chg_flg != 0) {
        Weapon_cls(pPl);
        Pl_weapon_ch(pPl);
        Sce.Weapon_chg_flg = 0;
    } else {
        Pl_weapon_ch_init(pPl);
    }
    pSce->Data++;
    return 1;
}

/* swaps the models of parts 4 and 15 of the work and toggles Unk4B bit 0 */
ULONG Parts_tmd_swap(SCE_TASK *pSce)
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

/* opens the status screen (St_type 0) and stops the game */
ULONG Status_on(SCE_TASK *pSce)
{
    St_type_a[0] = 0;
    Game.Unk1 = 1;
    G.Status_flg |= 0x200;
    Game.Stop_bak = G.Stop_flg;
    pSce->Data++;
    return 1;
}

/* switches floor area Data[1] of the room floor table on (Data[2] 1) or off */
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

/* RE2 Sce_Item_lost: removes item Data[1] from the inventory */
ULONG Sce_Item_lost(SCE_TASK *pSce)
{
    Item_delete(Search_item_id_(pSce->Data[1], 0));
    pSce->Data += 2;
    return 1;
}

/* starts a live selection with message Data[1] */
ULONG Sel_evt_on(SCE_TASK *pSce)
{
    UCHAR v;

    v = pSce->Data[1];
    G.Status_flg |= 0x10000000;
    Live_sel_mess_no[0] = v;
    pSce->Data += 2;
    return 1;
}

/* item AOT that Chaser_item_set builds in the free work (SCE_AOT head, then the item data) */
typedef struct {
    UCHAR Id;                          /* 0x00 */
    UCHAR Type;                        /* 0x01 */
    signed char nFloor;                /* 0x02 */
    UCHAR Super;                       /* 0x03 */
    SHORT X;                           /* 0x04 */
    SHORT Z;                           /* 0x06 */
    USHORT W;                          /* 0x08 */
    USHORT D;                          /* 0x0A */
    USHORT iItem;                      /* 0x0C */
    USHORT nItem;                      /* 0x0E */
    USHORT Item_flg;                   /* 0x10 */
    USHORT Om_no;                      /* 0x12 */
    UCHAR pad14[0x18 - 0x14];
} CHASER_ITEM_AOT;

/* places Nemesis' item drops by room (G+0x7DC0 records, Chaser_item_room_tbl room table): the drop of
 * this room (or of its paired room) as an item model + item AOT in Sce_aot[31], else a new drop when
 * Nemesis (Id 0x34..0x36) is in the room */
ULONG Chaser_item_set(SCE_TASK *pSce)
{
    SVECTOR Spd;
    tagGLOBAL *pG;
    ULONG Found;
    USHORT Room;
    USHORT Room2;
    USHORT Room_a;
    ULONG i;
    ULONG j;
    ULONG Om_no;
    ULONG Em_no;
    OBJ_MODEL_WORK *pOm;
    CHASER_ITEM_AOT *pAot;
    ENEMY_WORK **ppEm;

    pG = &G;
    Found = 0;
    if (pG->System_flg & 0x100) {
        pSce->Data += 3;
        return 1;
    }
    Room = G.Room_no | ((G.Stage_no + 1) << 8);
    for (i = 0; i < 8; i++) {
        if (G.Chaser_drop[i].Be_flg) {
            for (j = 0; j < 13; j++) {
                Room_a = Chaser_item_room_tbl[j][0];
                if (Room == Room_a) {
                    Room2 = Chaser_item_room_tbl[j][1];
                    Found = 1;
                    goto found;
                }
                if (Room == Chaser_item_room_tbl[j][1]) {
                    Room2 = Room_a;
                    Found = 1;
                    goto found;
                }
            }
        }
    }
found:
    for (i = 0; i < G.Chaser_drop_num; i++) {
        if (G.Chaser_drop[i].Be_flg && Room == G.Chaser_drop[i].Room_id) {
            goto drop;
        }
    }
    if ((UCHAR)Found) {
        for (i = 0; i < G.Chaser_drop_num; i++) {
            if (G.Chaser_drop[i].Be_flg && Room2 == G.Chaser_drop[i].Room_id) {
                goto drop;
            }
        }
    }
    for (ppEm = &G.pEnemy[2]; ppEm != G.ppEnemy_end; ppEm++) {
        if ((ULONG)((*ppEm)->Id - 0x34) < 3) {
            goto chaser;
        }
    }
    goto end;
chaser:
    Om_no = pSce->Data[2];
    Em_no = pSce->Data[1];
    G.Status_flg |= 0x2000;
    pOm = &pG->Ob_model[Om_no];
    pOm->Unk4A = 8;
    pOm->UnkC2 = 0xFF;
    pOm->nFloor = 0;
    pOm->Type = 0;
    pOm->Be_flg = 0x6011;
    if (G.pObj_work_end < pOm + 1) {
        G.pObj_work_end = pOm + 1;
    }
    pOm->Attribute = 0x10;
    pOm->M.t[0] = 600;
    pOm->M.t[1] = -1800;
    pOm->M.t[2] = -500;
    pOm->Parts.Be_flg = 1;
    pOm->Cdir.vx = 0;
    pOm->Cdir.vy = 0;
    pOm->Cdir.vz = 0;
    pOm->Col.Unk24 = 0;
    pOm->Col.Unk26 = 0;
    pOm->Col.Unk28 = 0;
    pOm->Col.W0 = 0;
    pOm->Col.H = 0;
    pOm->Col.D0 = 0;
    pOm->pSuper = EM_WORKM(pG->pEnemy[Em_no + 2]);
    Om_init((ENEMY_WORK *)pOm, G.pRoom->pOmodel[Om_no].Tmd_adr);
    RotMatrix(&pOm->Cdir, &pOm->M);
    CompM(pOm->pSuper, &pOm->M, &pOm->Workm);
    pAot = (CHASER_ITEM_AOT *)G.pEm_mem;
    G.pEm_mem = (ENEMY_WORK *)(pAot + 1);
    pG->Sce_aot[31] = (struct tagSCE_AOT *)pAot;
    pG->nSce_aot++;
    pAot->Id = 0;
    pAot->Type = 0x31;
    pAot->nFloor = 0;
    pAot->Super = 0;
    pAot->X = -500;
    pAot->Z = -500;
    pAot->W = 1000;
    pAot->D = 1000;
    if (G.Clear_cnt == 0 && G.Chaser_drop_num == 6) {
        pAot->iItem = 0xF;
        pAot->nItem = 100;
    } else {
        pAot->iItem = Chaser_item_id_tbl[G.Chaser_drop_num];
        pAot->nItem = Chaser_item_num_tbl[G.Chaser_drop_num];
    }
    pAot->Om_no = Om_no | 0x100;
    pAot->Item_flg = D_800D2558 + 0xF0;
    goto end;
drop:
    Om_no = pSce->Data[2];
    pOm = &pG->Ob_model[Om_no];
    pOm->Unk4A = 8;
    pOm->nFloor = (SHORT)G.Chaser_drop[i].Pos_y / -1800;
    pOm->UnkC2 = 0xFF;
    pOm->Type = 0;
    pOm->Be_flg = 0x6001;
    if (G.pObj_work_end < pOm + 1) {
        G.pObj_work_end = pOm + 1;
    }
    pOm->Attribute = 0x10;
    pOm->M.t[0] = (SHORT)G.Chaser_drop[i].Pos_x;
    pOm->M.t[1] = (SHORT)G.Chaser_drop[i].Pos_y;
    pOm->M.t[2] = (SHORT)G.Chaser_drop[i].Pos_z;
    pOm->Cdir.vx = 0;
    pOm->Cdir.vy = 0;
    pOm->Cdir.vz = 0;
    if (Room == 0x30E) {
        if (Hit_ck_point4(&pOm->M.t[0], &Chaser_zone_30E_0)) {
            pOm->M.t[0] = 0x144B;
            pOm->M.t[1] = 0;
            pOm->M.t[2] = -0x1FA8;
        } else if (Hit_ck_point4(&pOm->M.t[0], &Chaser_zone_30E_1)) {
            pOm->M.t[0] = -0x103B;
            pOm->M.t[1] = 0;
            pOm->M.t[2] = -0x3B64;
        }
    }
    pOm->Parts.Be_flg = 1;
    pOm->Col.Unk24 = 0;
    pOm->Col.Unk26 = 0;
    pOm->Col.Unk28 = 0;
    pOm->Col.W0 = 0;
    pOm->Col.H = 0;
    pOm->Col.D0 = 0;
    pOm->pSuper = &GsIDMATRIX;
    Om_init((ENEMY_WORK *)pOm, G.pRoom->pOmodel[Om_no].Tmd_adr);
    RotMatrix(&pOm->Cdir, &pOm->M);
    CompM(pOm->pSuper, &pOm->M, &pOm->Workm);
    pAot = (CHASER_ITEM_AOT *)G.pEm_mem;
    G.pEm_mem = (ENEMY_WORK *)(pAot + 1);
    pG->Sce_aot[31] = (struct tagSCE_AOT *)pAot;
    pG->nSce_aot++;
    pAot->Id = 2;
    pAot->Type = 0x31;
    pAot->nFloor = pOm->M.t[1] / -1800;
    pAot->Super = 0;
    pAot->X = pOm->M.t[0] - 500;
    pAot->Z = pOm->M.t[2] - 500;
    pAot->W = 1000;
    pAot->D = 1000;
    pAot->iItem = G.Chaser_drop[i].Item_id;
    if (G.Clear_cnt == 0 && i == 6) {
        pAot->nItem = 100;
    } else {
        pAot->nItem = Chaser_item_num_tbl[i];
    }
    pAot->Item_flg = i + 0xF0;
    pAot->Om_no = Om_no | 0x100;
    Spd.vx = 0;
    Spd.vy = -400;
    Spd.vz = 0;
    Esp_call(0x705, 0x28000000, &pOm->M, &Spd, NULL);
end:
    pSce->Data += 3;
    return 1;
}

/* RE2 Sce_parts_bomb: makes part Parts fly off (colour R, G, B and speeds W[]) */
ULONG Sce_parts_bomb(SCE_TASK *pSce)
{
    SCE_PARTS_BOMB_REC *pData;
    PARTS_W *pP;
    ULONG Rgb;

    pData = (SCE_PARTS_BOMB_REC *)pSce->Data;
    pP = &pSce->pWork->pSin_parts_ptr[pData->Parts];
    pP->UnkA2 |= 0x20;
    pP->Be_flg |= 2;
    Rgb = pData->R | (pData->G << 8) | (pData->B << 16);
    ((SHORT *)&pP->Unk74)[4] = 0;
    pP->Poly_rgb = Rgb;
    ((SHORT *)&pP->Unk74)[0] = pData->W[0];
    ((SHORT *)&pP->Unk74)[2] = pData->W[1];
    ((SHORT *)&pP->Unk74)[6] = pData->W[2];
    ((SHORT *)&pP->Unk74)[1] = pData->W[3];
    ((SHORT *)&pP->Unk74)[3] = pData->W[4];
    pSce->Data += 16;
    return 1;
}

/* RE2 Sce_parts_down: makes part Parts fall from Old_x/y/z */
ULONG Sce_parts_down(SCE_TASK *pSce)
{
    SCE_PARTS_DOWN_REC *pData;
    PARTS_W *pP;

    pData = (SCE_PARTS_DOWN_REC *)pSce->Data;
    pP = &pSce->pWork->pSin_parts_ptr[pData->Parts];
    pP->Timer3 = 0;
    pP->Be_flg |= 0xA;
    pP->Old_x = pData->Old_x;
    pP->Old_z = pData->Old_z;
    pP->Old_y = pData->Old_y;
    pP->Timer1 = pData->Timer1;
    pP->Rot_spd_x = pData->Rot_spd_x;
    pP->Rot_spd_y = pData->Rot_spd_y;
    pP->Rot_spd_z = pData->Rot_spd_z;
    pSce->Data += 16;
    return 1;
}

/* sets Nemesis' life to 800 for enemy ids 0x34..0x36 */
ULONG Chaser_life_init(SCE_TASK *pSce)
{
    switch (pSce->Data[1]) {
    case 0x34:
        Chaser_life[0] = 0x320;
        break;
    case 0x35:
        Chaser_life[0] = 0x320;
        break;
    case 0x36:
        Chaser_life[0] = 0x320;
        break;
    }
    pSce->Data += 2;
    return 1;
}

/* switches entry Data[2] of camera-cut table Data[1] (G.pRoom->pUnk40) on (Data[3]) or off */
ULONG Cut_flg_set(SCE_TASK *pSce)
{
    UCHAR *pData;
    UCHAR *p;
    UCHAR *q;
    UCHAR *pHdr;
    ULONG n;
    ULONG k;

    pData = pSce->Data;
    pSce->Data = pData + 4;
    if (G.Ot_sub_num != 0) {
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

/* sets Ctrl_mode to Data[1] */
void Sce_ctrl_mode_set(SCE_TASK *pSce)
{
    Ctrl_mode_a[0] = pSce->Data[1];
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
        pParts->Rot_spd_x = Val;
        break;
    case 8:
        pParts->Rot_spd_y = Val;
        break;
    case 9:
        pParts->Rot_spd_z = Val;
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

/* re-initialises enemy work Data[1] + 2 to its start state */
ULONG Sce_em_reset(SCE_TASK *pSce)
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
    pW->Ground = -(nFloor * 1800);
    if (pW->Kage.pKw != NULL) {
        pW->Kage.pKw->Be_flg = 0;
    }
    pW->Kage.pKw = NULL;
    pW->Add_dir = 0;
    pW->Unk14A = 0;
    pW->Unk14C = 0;
    pW->Unk14E = 0;
    pW->Water = 0;
    pW->Tilt_spd = 0;
    pW->Tilt_flg = 0;
    pW->Unk1C0 = 0;
    pW->Tilt[1] = 0;
    pW->Tilt[0] = 0;
    pW->Tilt_max = 0;
    pW->Unk1B8 = 0;
    pW->Mot_scale_y = 0x1000;
    pW->Unk1CA = 0;
    pW->Unk1C8 = 0;
    pW->Damage_flg = 0;
    pW->Unk158 = 0;
    PartsWork_link(pW, 0, pW->pUnk10C);
    pSce->Data += 2;
    return 1;
}

/* adds room id (USHORT +2) to the Nemesis room list G.Chaser_room_list (when chasing) */
ULONG Ahead_room_set(SCE_TASK *pSce)
{
    UCHAR i;
    tagGLOBAL *pG = &G;

    if ((pG->Status_flg & 0x40000000) == 0) {
        pSce->Data += 4;
        return 1;
    }
    for (i = 0; i < 8; i++) {
        if ((USHORT)pG->Chaser_room_list[i] == 0) {
            pG->Chaser_room_list[i] = ((USHORT *)pSce->Data)[1];
            break;
        }
    }
    pSce->Data += 4;
    return 1;
}

/* true while the action button is pressed and the player may open a door */
ULONG Sce_door_ck(SCE_TASK *pSce)
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
                    if (!(Moji.Mess_flg & 0xC0) && Game_st_flg == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/* shakes the screen: Quake_add(USHORT +2, USHORT +4) */
ULONG Sce_quake_add(SCE_TASK *pSce)
{
    Quake_add_(((USHORT *)pSce->Data)[1], ((USHORT *)pSce->Data)[2]);
    pSce->Data += 6;
    return 1;
}

/* opens the map (St_type 4) with event map Data[2] */
ULONG Sce_map_open(SCE_TASK *pSce)
{
    ULONG x;

    St.St_type = 4;
    Game.Stop_bak = G.Stop_flg;
    x = pSce->Data[2];
    G.Status_flg |= 0x200;
    Game.Unk1 = 1;
    St.Map_event = x;
    pSce->Data += 4;
    return 1;
}

/* clears the Nemesis event flags of the current chase state */
ULONG Chaser_evt_clr(SCE_TASK *pSce)
{
    UCHAR i;

    switch (Chaser_evt_state) {
    case 3:
    case 4:
        for (i = 0; i < 10; i++) {
            Flg_off_(G.Chaser_flg, i + 0x50);
        }
        break;
    case 5:
        for (i = 0; i < 0x14; i++) {
            Flg_off_(G.Chaser_flg, i + 0x61);
        }
        break;
    case 6:
        for (i = 0; i < 7; i++) {
            Flg_off_(G.Chaser_flg, i + 0x5A);
        }
        break;
    }
    pSce->Data++;
    return 1;
}
