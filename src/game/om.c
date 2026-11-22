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
#include "pl_wep.h"
#include "oba.h"

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

extern ULONG D_800CC87C;
extern ULONG D_800D1F2C;
extern UCHAR D_8009CD3C;
ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Arg1);
LONG Hit_ck_point4_l(ULONG Arg0, ULONG Arg1) __asm__("func_8001020C");
void func_800288C8(ENEMY_WORK *pEm, PARTS_W *pParts, LONG Arg2, MATRIX *pM);

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

/* declarations from earlier parts of the original address-split file */
extern UCHAR D_800A0514[][4];
extern ULONG D_800D2048[];
extern UCHAR D_8009BEE4[];
extern USHORT D_8009C7C0[];

extern USHORT D_80099654[];
extern UCHAR D_80098E88[];
extern UCHAR D_800CC84D;
extern UCHAR D_800D4433;
void Mess_print(ULONG x, ULONG y, UCHAR *pStr, ULONG Attr);

static __inline__ UCHAR *Msg_get_80031BDC(ULONG No)
{
    return D_80098E88 + D_80099654[No];
}

ULONG Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
extern LOADTASK D_8009C944[];
ULONG Task_lexecute_(ULONG Level, ULONG Task_no) __asm__("Task_lexecute");

void Task_lchain_(ULONG Task_no) __asm__("Task_lchain");

extern void (*D_8009CBB4[])(void);

LONG func_800346AC(COL_OBJ *pObj, ENEMY_WORK *pEm, ENEMY_WORK *pT);

LONG func_80034448(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG func_8004B374(COL_ENT *pEnt, ULONG Type, ULONG Arg2);

PARTS_W *func_8002E61C(ENEMY_WORK *pEm, PARTS_W *pParts);
PARTS_W *func_80035868(ENEMY_WORK *pEm, PARTS_W *pParts);
void func_8003589C(OBJ_MODEL_WORK *pOm, ULONG *pKan);
#define PW_FREE(p) ((USHORT *)&(p)->Unk74)  /* PARTS_W 0x74..0x7F */
/* per object-model type (ENEMY_WORK.Type & 0x1F) */
typedef struct {
    UCHAR Unk0;                        /* 0x0 non-zero: part Unk1 starts hidden (func_8003589C) */
    UCHAR Unk1;                        /* 0x1 */
    UCHAR Num;                         /* 0x2 */
    UCHAR Idx;                         /* 0x3 */
} OM_BREAK_TBL;
extern OM_BREAK_TBL D_8009CBEC[];

void func_80035790(ENEMY_WORK *pEm, ULONG *pData)
{
    if (pEm->Id != 1) {
        *(ULONG **)((UCHAR *)pEm + 0x184) = pData + 1;
        pData = (ULONG *)((UCHAR *)pData + *pData);
    }
    if (G.pOld_tmd != pData) {
        G.pOld_tmd = pData;
        func_8002AB34(pData, EM_UCHAR(pEm, 0x48), EM_UCHAR(pEm, 0x49));
    }
    *(ULONG *)&pEm->Routine_0 = 0;
    pEm->pTmd = pData + 2;
    pEm->Poly_rgb = 0x808080;
    if (pEm->Id == 1) {
        G.pEm_mem = (ENEMY_WORK *)func_8002E61C(pEm, (PARTS_W *)G.pEm_mem);
    } else {
        G.pEm_mem = (ENEMY_WORK *)func_80035868(pEm, (PARTS_W *)G.pEm_mem);
        func_8003589C((OBJ_MODEL_WORK *)pEm, *(ULONG **)((UCHAR *)pEm + 0x184));
    }
}

PARTS_W *func_80035868(ENEMY_WORK *pEm, PARTS_W *pParts)
{
    pEm->Parts_num = ((UCHAR *)pEm->pTmd)[-4];
    *(PARTS_W **)&pEm->Spd_z = pParts;
    return pParts + pEm->Parts_num;
}

INCLUDE_ASM("asm/nonmatchings/game/om", func_8003589C);

void func_80035B84(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Arg2, ULONG Arg3)
{
    ULONG n;

    if ((pEm->Be_flg & 0x40) || Hit_ck_point4_l(Arg3, D_800CC87C)) {
        n = pEm->Parts_num;
    loop:
        if (pParts->Be_flg & 1) {
            func_800288C8(pEm, pParts, (SHORT)pParts->UnkA2, &pParts->Workm);
        }
        pParts++;
        if (--n != 0) {
            goto loop;
        }
    }
}

extern UCHAR D_800CC84B;

void Col_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
    ULONG Poly_num;
    ULONG Clut;
    ULONG i;
    ULONG D0;
    ULONG *p;
    OBJ_MODEL_WORK *pMw;
    RECT *pRect;

    pMw = pCcp->pOm;
    Clut = GetClut(0x120, D_800CC84B + 0x1E0 + Col_wk_no) & 0xFFFF;
    if (pMw->Unk4A != 1) {
        Poly_num = pMw->pTmd->n_prim3;
        p = pMw->pTmd->pPrim3;
        if (Poly_num != 0)
            D0 = *p >> 22;
        for (i = 0; i < Poly_num; i++) {
            *p = (*p & 0xffff) | (Clut << 16);
            p += 3;
        }
        Poly_num = pMw->pTmd->n_prim4;
        p = pMw->pTmd->pPrim4;
        if (Poly_num != 0)
            D0 = *p >> 22;
        for (i = 0; i < Poly_num; i++) {
            *p = (*p & 0xffff) | (Clut << 16);
            p += 4;
        }
    } else {
        Poly_num = pMw->pTmd->n_prim4;
        p = pMw->pPacket188;
        if (Poly_num != 0)
            D0 = p[3] >> 22;
        Poly_num <<= 1;
        for (i = 0; i < Poly_num; i++) {
            p[3] = (p[3] & 0xffff) | (Clut << 16);
            p += 13;
        }
    }
    for (i = 0; i < 2; i++) {
        pRect = pCcp->Col_dr_rect + i;
        pRect->y = (SHORT)D0;
        pRect->w = 0x10;
        pRect->h = 1;
    }
}

void Col_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr)
{
    switch (Ctr)
    {
    case 0:
        pRect->x = (SHORT)(pCcp->Col_num << 4);
        break;
    case 1:
        pRect->x = (SHORT)((pCcp->Col_cnt / pCcp->Col_wait) << 4);
        if ((ULONG)pCcp->Col_cnt != (ULONG)pCcp->Col_num * pCcp->Col_wait - 1)
            pCcp->Col_cnt++;
        break;
    case 2:
        pRect->x = (SHORT)((pCcp->Col_cnt / pCcp->Col_wait) << 4);
        pCcp->Col_cnt++;
        if ((ULONG)pCcp->Col_cnt == (ULONG)pCcp->Col_num * pCcp->Col_wait)
            pCcp->Col_cnt = 0;
        break;
    case 3:
        pRect->x = (SHORT)((pCcp->Col_cnt / pCcp->Col_wait) << 4);
        pCcp->Col_cnt++;
        if ((ULONG)pCcp->Col_cnt == (ULONG)pCcp->Col_num * pCcp->Col_wait)
        {
            pCcp->Col_cnt -= pCcp->Col_wait;
            pCcp->Col_ctr  = 4;
        }
        break;
    case 4:
        pCcp->Col_cnt--;
        pRect->x = (SHORT)((pCcp->Col_cnt / pCcp->Col_wait) << 4);
        if (pCcp->Col_cnt == 0)
        {
            pCcp->Col_cnt += pCcp->Col_wait;
            pCcp->Col_ctr = 3;
        }
        break;
    }
}

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

ULONG Tex_chg_(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr) __asm__("Tex_chg");
ULONG Tex_chg_(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr)
{
    switch (Ctr)
    {
    case 0:
    	pRect->x = (SHORT)((pCcp->Cc_num >> 2) << 5);
    	pRect->y = (SHORT)((pCcp->Cc_num & 3) << 6);
    	break;
    case 1:
    	pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
    	pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
    	if ((ULONG)pCcp->Cc_cnt != (ULONG)pCcp->Cc_num * pCcp->Cc_wait - 1)
    		pCcp->Cc_cnt++;
    	break;
    case 2:
    	pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
    	pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
    	pCcp->Cc_cnt++;
    	if ((ULONG)pCcp->Cc_cnt == pCcp->Cc_num * (ULONG)pCcp->Cc_wait)
    		pCcp->Cc_cnt = pCcp->Cc_wait;
    	break;
    case 3:
    	pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
    	pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
    	pCcp->Cc_cnt++;
    	if ((ULONG)pCcp->Cc_cnt == pCcp->Cc_num * (ULONG)pCcp->Cc_wait)
    	{
    		pCcp->Cc_cnt -= pCcp->Cc_wait;
    		pCcp->Cc_ctr  = 4;
    	}
    	break;
    case 4:
    	pCcp->Cc_cnt--;
    	pRect->x = pCcp->Cc_pos_x + (SHORT)((pCcp->Cc_cnt / pCcp->Cc_wait) >> 2) * 0x20;
    	pRect->y = pCcp->Cc_pos_y + ((pCcp->Cc_cnt / pCcp->Cc_wait) & 3) * 0x40;
    	if (pCcp->Cc_cnt == pCcp->Cc_wait)
    	{
    		pCcp->Cc_cnt += pCcp->Cc_wait;
    		pCcp->Cc_ctr  = 3;
    	}
    	break;
    }
}

void Om_chg_init_80036234(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
    pCcp->Cc_ctr = 1;
    pCcp->Cc_pos_x = 0;
    pCcp->Cc_pos_y = 0;
}

/* RE3 colour change: scroll the texture v of every quad by Cc_num (every Cc_cnt + 1 frames)
 * and bob the model up and down by Cc_wait, reversing every 0x33 frames */
void Col_chg_ctr_80036248(CC_PARTS_WORK *pCcp)
{
    OBJ_MODEL_WORK *pOm = pCcp->pOm;
    POLY_GT4 *pPrim0;
    POLY_GT4 *pPrim1;
    ULONG n;
    signed char d;

    if (pCcp->Cc_cnt != 0) {
        if (pCcp->Cc_cnt != pCcp->Cc_pos_y) {
            pCcp->Cc_pos_y++;
            goto move;
        }
        pCcp->Cc_pos_y = 0;
    }
    pPrim0 = (POLY_GT4 *)pOm->pPacket188;
    pPrim1 = pPrim0 + 1;
    n = pOm->pTmd->n_prim4;
    do {
        if (pPrim0->v0 < pCcp->Cc_num) {
            d = pCcp->Cc_num - pPrim0->v0;
            pPrim1->v0 = pPrim1->v2 = pPrim0->v0 = pPrim0->v2 = -0x40 - d;
            pPrim1->v1 = pPrim1->v3 = pPrim0->v1 = pPrim0->v3 = ~d;
        } else {
            pPrim1->v0 = pPrim1->v2 = pPrim0->v0 = pPrim0->v2 -= pCcp->Cc_num;
            pPrim1->v1 = pPrim1->v3 = pPrim0->v1 = pPrim0->v3 -= pCcp->Cc_num;
        }
        pPrim0 += 2;
        pPrim1 += 2;
    } while (--n != 0);
move:
    if (++pCcp->Cc_pos_x == 0x33) {
        pCcp->Cc_pos_x = 0;
        pCcp->Cc_ctr = 1 - pCcp->Cc_ctr;
        return;
    }
    if (pCcp->Cc_ctr)
        pOm->M.t[1] += pCcp->Cc_wait;
    else
        pOm->M.t[1] -= pCcp->Cc_wait;
}

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
    pOt = G.Ot2[D0];
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
            Col_chg_ctr_80036248(pCcp);
        }
        pCcp++;
        i++;
    } while (i < D1);
}

extern void (*D_8009CC64[])(OBJ_MODEL_WORK *pOm);
extern LONG D_800DD4AC;                /* number of pushable object models touched this frame */
extern OBJ_MODEL_WORK *D_800DD4B0;     /* the one on the player's floor */
void func_80036570(void);

void Om_move(void)
{
    OBJ_MODEL_WORK *pOm;

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
    for (pOm = G.Ob_model; pOm != G.pObj_work_end; pOm++) {
        D_8009CC64[pOm->Routine_0](pOm);
    }
    func_80036570();
}

/* a pushable object model stays in place while the player pushes it; after 6 frames the push starts */
void func_80036570(void)
{
    OBJ_MODEL_WORK *pOm;
    SHORT Pos_x;
    SHORT Pos_z;

    if (D_800DD4AC == 1 && D_800DD4B0 != NULL) {
        pOm = D_800DD4B0;
        if (!(pOm->Be_flg & 0x4000)) {
            if (pOm->Be_flg & 0x100) {
                Pos_x = pOm->M.t[0];
                Pos_z = pOm->M.t[2];
                if (++pOm->Push_cnt >= 6) {
                    if (pOm->UnkAE & 0x20) {
                        pOm->Push_cnt = 0;
                    } else {
                        D_800DD4AC = 0;
                        G.Status_flg |= 0x10;
                        if (pOm->Push_cnt != 6) {
                            pOm->Push_cnt = 7;
                            return;
                        }
                    }
                }
                pOm->M.t[0] = Pos_x;
                pOm->M.t[2] = Pos_z;
            }
        }
    }
}

void Em_routine0_set1_80036640(OBJ_MODEL_WORK *pOm)
{
    pOm->Routine_0 = 1;
}

extern SHORT D_800DE614;
extern SHORT D_800DE618;
extern SHORT D_800DE61A;
extern SHORT D_800DE61E;

void Om_set_fall_dir(ENEMY_WORK *pEm)
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

INCLUDE_ASM("asm/nonmatchings/game/om", func_800366EC);

extern ULONG D_800D20D4[];
void Flg_on(ULONG *pFlg, ULONG Id);
void func_800366EC(OBJ_MODEL_WORK *pOm, ULONG Arg1, ULONG Arg2);

void func_80036C60(OBJ_MODEL_WORK *pOm)
{
    tagGLOBAL *pG;

    pG = &G;
    if (!(pOm->Be_flg & 1)) {
        return;
    }
    if (pG->Pl_routine_1 != 1 && pG->Pl_routine_1 != 9) {
        pOm->Push_cnt = 0;
    }
    if ((pOm->UnkAE & 0x10) && (pOm->UnkBA & 0x8000)) {
        D_800DD4AC++;
        if (pG->Pl_nFloor == pOm->nFloor) {
            D_800DD4B0 = pOm;
        }
    }
    if ((pOm->Unk4A & 4) && *G.pUnk75AC[pOm->UnkC1] == 0xFF) {
        switch (pOm->Type & 0xC0) {
        case 0:
            func_800366EC(pOm, 1, 1);
            break;
        case 0x40:
            Em_routine0_set1_80036640(pOm);
            break;
        case 0x80:
            break;
        }
        Flg_on(D_800D20D4, pOm->UnkC2);
    }
}

LONG func_8004D720_3(VECTOR *pV, ULONG W, ULONG Arg2) __asm__("func_8004D720");

void func_80036D94(OBJ_MODEL_WORK *pOm)
{
    switch (pOm->Routine_1) {
    case 0:
        pOm->Unk18E = 13;
        pOm->Unk18C = 13;
        pOm->Unk190 = 0;
        pOm->Routine_1 = 1;
        pOm->Routine_2 = 0;
    case 1:
        pOm->Unk18C += pOm->Unk18E;
        if (++pOm->Routine_2 >= 2) {
            pOm->Unk18E = 0;
            pOm->M.t[1] += pOm->Unk190;
            pOm->Unk190 += 0x2D;
            if (pOm->Routine_2 == 13) {
                pOm->Unk192 = func_8004D720_3((VECTOR *)pOm->M.t, 0x1C2, 0);
                pOm->Old_x = pOm->M.t[0];
                pOm->Old_z = pOm->M.t[2];
                pOm->nFloor = pOm->Unk192 / -1800;
            }
            if (pOm->Unk192 < pOm->M.t[1]) {
                pOm->M.t[1] = pOm->Unk192;
                pOm->Routine_1 = 2;
            }
        } else {
            pOm->M.t[1] += (SHORT)(pOm->Unk18C / 3);
        }
        if (pOm->UnkBA != 0) {
            func_800366EC(pOm, 0, 0);
        }
        break;
    case 2:
        pOm->M.t[1] -= 30;
        pOm->Routine_1++;
        func_800366EC(pOm, 0, 0);
        pOm->Routine_1 = 0;
        break;
    case 3:
        pOm->Unk4A = 0;
        pOm->Routine_0 = 0;
        pOm->Routine_1 = 0;
        pOm->M.t[1] += 30;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/om", func_80036F78);

extern SHORT D_800DD492;
extern SHORT D_800DD494;
extern SHORT D_800DD496;
extern SHORT D_800DD498;
extern SHORT D_800DD49A;
extern SHORT D_800DD49C;
extern SVECTOR D_800DD4A0;
extern UCHAR D_800DD4A8;
void Rot_add_matrix(LONG *pPos, SHORT *pDir, MATRIX *pM, SVECTOR *Add_dir);

void func_8003749C(OBJ_MODEL_WORK *pOm)
{
    SHORT n;
    LONG Dummy[4];

    switch (pOm->Routine_1) {
    case 0:
        D_800DD49C = 0;
        D_800DD492 = D_800DD494 = 13;
        D_800DD496 = 0;
        D_800DD498 = 16;
        D_800DD4A0.vx = 0;
        D_800DD4A0.vy = 0;
        D_800DD4A0.vz = 0;
        D_800DD49A = 0;
        D_800DD4A8 = 0;
        pOm->Routine_1 = 1;
        pOm->Routine_2 = 0;
    case 1:
        pOm->M.t[0] += D_800DD492 * D_800DE61A;
        pOm->M.t[2] += D_800DD492 * D_800DE61E;
        if (D_800DD49C < 0x400) {
            n = D_800DD49C + D_800DD496;
            D_800DD49C = n;
            if (n > 0x400) {
                D_800DD496 -= n - 0x400;
            }
            D_800DD4A0.vx = D_800DD496 * -D_800DE61E;
            D_800DD4A0.vz = D_800DD496 * D_800DE61A;
        } else {
            D_800DD4A0.vz = 0;
            D_800DD4A0.vx = 0;
        }
        D_800DD496 += D_800DD498;
        D_800DD492 += D_800DD494;
        if (++pOm->Routine_2 >= 16) {
            D_800DD494 = 0;
            pOm->Routine_0 = 0;
            pOm->Routine_1 = 0;
        }
        break;
    }
    Rot_add_matrix(pOm->M.t, &pOm->Cdir.vx, &pOm->M, &D_800DD4A0);
}

LONG func_800472EC(HIT_REQ *pReq, ULONG a1);
LONG func_80047BD0(HIT_REQ *pReq, ULONG a1);

typedef struct {                       /* object model part, 0xBC bytes (func_80037680) */
    ULONG Be_flg;                      /* 0x00 */
    UCHAR pad4[0x74 - 0x4];
    SHORT Cdir_y;                      /* 0x74 */
    SHORT Unk76;                       /* 0x76 */
    SHORT Unk78;                       /* 0x78 */
    SHORT Unk7A;                       /* 0x7A */
    USHORT Unk7C;                      /* 0x7C */
    SHORT pad7E;
    SHORT Unk80;                       /* 0x80 */
    UCHAR pad82[0xA2 - 0x82];
    USHORT UnkA2;                      /* 0xA2 */
    UCHAR padA4[0xBC - 0xA4];
} OM_PARTS;

extern ULONG D_8009CC54[];

void func_80037680(OBJ_MODEL_WORK *pOm)
{
    HIT_REQ Req;
    OM_PARTS *pP;
    UCHAR Num;
    UCHAR Idx;
    ULONG k;
    ULONG n;

    pP = (OM_PARTS *)pOm->pParts;
    Num = D_8009CBEC[pOm->Type & 0x1F].Num;
    Idx = D_8009CBEC[pOm->Type & 0x1F].Idx;
    k = 0;
    if (pOm->Routine_1 != 0) {
        pOm->Routine_1--;
        return;
    }
    switch (pOm->Routine_2) {
    case 0:
        n = pOm->Parts_num;
        do {
            if (Num) {
                if (k++ == Idx) {
                    pP->Be_flg |= 2;
                    Num--;
                    Idx++;
                    pP->UnkA2 |= 0x23;
                } else {
                    pP->UnkA2 = 0;
                }
            } else {
                pP->Be_flg |= 2;
                pP->UnkA2 |= 0x23;
            }
            pP->Cdir_y = pOm->Cdir.vy + 0x800;
            pP->Unk7C = 0x8000;
            pP->Unk78 = 0;
            pP->Unk76 = 0;
            pP->Unk7A = 0;
            pP->Unk80 = 0;
            pP++;
        } while (--n != 0);
        pOm->Unk130 = 0x101020;
        pOm->Routine_2 = 1;
        pOm->Routine_3 = 10;
        break;
    case 1:
        if (--pOm->Routine_3 != 0) {
            Req.pPos = (VECTOR *)pOm->M.t;
            Req.W = 0;
            Req.H = 0;
            Req.D = 0;
            Req.R = 0;
            func_800472EC(&Req, D_8009CC54[pOm->Type & 0xF]);
            func_80047BD0(&Req, D_8009CC54[pOm->Type & 0xF]);
        } else {
            pOm->Routine_2 = 2;
        }
        break;
    case 2:
        break;
    }
}

void func_8003783C(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_1) {
    case 0:
        *G.pUnk75AC[EM_UCHAR(pEm, 0xC1)] = 0xFF;
        pEm->Routine_1 = 1;
        break;
    case 1:
        break;
    case 2:
        *G.pUnk75AC[EM_UCHAR(pEm, 0xC1)] = 0;
        pEm->Routine_0 = 0;
        pEm->Routine_1 = 0;
        break;
    }
}

void Set_light_pos(VECTOR *pPos);
void Joint_trans_om(OBJ_MODEL_WORK *pOm, struct tagPARTS_W *pParts, ULONG Be_flg, LONG *pPos) __asm__("func_800251FC");
void Parts_trans_om(OBJ_MODEL_WORK *pOm, struct tagPARTS_W *pParts, ULONG Be_flg, LONG *pPos) __asm__("func_80035B84");
void Om_sort_om(OBJ_MODEL_WORK *pOm, ULONG Abr) __asm__("func_8002E74C");
extern MATRIX GsWSMATRIX;              /* 0x80098990 */
extern MATRIX LIGHTMATRIX;             /* 0x80098950 */
extern MATRIX COLORMATRIX;             /* 0x80098930 */

void Om_trans(void)
{
    MATRIX M;
    OBJ_MODEL_WORK *pOm;
    ULONG Cnt;
    ULONG nOmodel;
    tagGLOBAL *pG;

    nOmodel = G.pRoom->nOmodel;
    if (nOmodel == 0)
        return;
    for (Cnt = 0; Cnt < nOmodel;) {
        pG = &G;
        pOm = G.Ob_model + Cnt;
        pG->pEm = (struct tagENEMY_WORK *)pOm;
        Cnt++;
        if (!(pOm->Be_flg & 1))
            continue;
        if (pOm->Be_flg & 0x10)
            continue;
        RotMatrix(&pOm->Cdir, &pOm->M);
        func_8002D4B0(pOm->pSuper, &pOm->M, &pOm->Workm);
        Set_light_pos((VECTOR *)pOm->Workm.t);
        SetColorMatrix(&COLORMATRIX);
        if (pOm->Unk4A == 2)
            pOm->Workm.t[1] -= 900;
        if (pOm->Unk4A == 1) {
            MulMatrix0(&LIGHTMATRIX, &pOm->Workm, &M);
            SetLightMatrix(&M);
            func_8002D4B0(&GsWSMATRIX, &pOm->Workm, &M);
            SetRotMatrix(&M);
            SetTransMatrix(&M);
            Om_sort_om(pOm, 1);
        } else {
            Joint_trans_om(pOm, pOm->pParts, pOm->Be_flg, pOm->Workm.t);
            Parts_trans_om(pOm, pOm->pParts, pOm->Be_flg, pOm->Workm.t);
            pOm->Be_flg &= 0xF0FFFFFF;
        }
        pOm->Old_x = pOm->M.t[0];
        pOm->Old_y = pOm->M.t[1];
        pOm->Old_z = pOm->M.t[2];
    }
}

void func_80037A90(OBJ_MODEL_WORK *pOm, ULONG Arg1)
{
    ENEMY_WORK **pp;
    OBJ_MODEL_WORK *p;
    UCHAR *pAot;
    ULONG i;
    tagGLOBAL *pG = &G;

    pG->Status_flg &= ~0x2000;
    pAot = (UCHAR *)G.Sce_aot[31];
    for (pp = &pG->pEnemy[2]; pp != pG->ppEnemy_end; pp++) {
        if ((*pp)->Id == pOm->Unk4A)
            break;
    }
    i = 0;
    for (p = G.Ob_model; p != G.pObj_work_end; p++, i++) {
        if ((p->Be_flg & 1) && p->Unk4A == 8)
            break;
    }
    pAot[3] = i | 0x80;
    G.Unk7DC0[G.Unk7E20].Unk0 = 1;
    G.Unk7DC0[G.Unk7E20].Unk1 = pAot[0xC];
    G.Unk7DC0[G.Unk7E20].Unk4 = G.Unk7840 | ((G.Unk783E + 1) << 8);
    G.Unk7DC0[G.Unk7E20].Pos_x = pOm->M.t[0];
    G.Unk7DC0[G.Unk7E20].Pos_y = pOm->M.t[1];
    G.Unk7DC0[G.Unk7E20].Pos_z = pOm->M.t[2];
    p->Routine_0 = 4;
    p->Routine_1 = 0;
    p->Routine_2 = Arg1;
    G.Unk7E20++;
}

ULONG func_80037D50(VECTOR *pPos);

void func_80037C38(void)
{
    tagGLOBAL *pG;
    ENEMY_WORK **pp;
    ENEMY_WORK **ppEnd;
    ENEMY_WORK *pEm;
    OBJ_MODEL_WORK *pOm;

    pG = &G;
    ppEnd = pG->ppEnemy_end;
    for (pp = &pG->pEnemy[0]; pp != ppEnd; pp++) {
        pEm = *pp;
        if (pEm->Be_flg & 1) {
            if (pEm->Be_flg & 0x80) {
                *(ULONG *)((UCHAR *)pEm + 0x7C) = (ULONG)pG->pOt_sub[pG->Cbuf_id][EM_UCHAR(pEm, 0xD)];
            } else {
                *(ULONG *)((UCHAR *)pEm + 0x7C) = func_80037D50((VECTOR *)&pEm->Pos_x);
            }
        }
    }
    for (pOm = pG->Ob_model; pOm != pG->pObj_work_end; pOm++) {
        if (pOm->Be_flg & 1) {
            if (pOm->Be_flg & 0x80) {
                *(ULONG *)((UCHAR *)pOm + 0x7C) = (ULONG)pG->pOt_sub[pG->Cbuf_id][EM_UCHAR(pOm, 0xD)];
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
    if (pG->Unk2470 == 0) {
        goto end;
    }
    pHead = *(USHORT **)((UCHAR *)pG->pRoom + 0x40);
    if (pG->Cut_no < pHead[1]) {
        goto start;
    }
    goto end;
found:
    return (ULONG)pG->pOt_sub[pG->Cbuf_id][((USHORT *)p)[1]];
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

INCLUDE_ASM("asm/nonmatchings/game/om", func_80037E80);
