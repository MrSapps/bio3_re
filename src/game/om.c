#include "om.h"
#include "mojidisp.h"
#include "pl_wep.h"
#include "joint.h"
#include "sld.h"
#include "sub.h"

/*
 * OM.C (RE2 OM.C, reworked): object models (G.Ob_model[]): setup (Om_init), the per-kind
 * routines Om_routine[Routine_0] (wait/trigger, drop, tipping box, explosion, fall, target off),
 * the colour/texture change works (Col_chg*, Tex_chg*, Uv_chg*), drawing (Om_trans), and the
 * ordering-table helpers (Ot_get_pos: the OT of the room block a position is in).
 */

/* object model setup: map its model once (G.pOld_tmd), then its parts (or packet) work */
void Om_init(ENEMY_WORK *pEm, ULONG *pData)
{
    if (pEm->Id != 1) {
        pEm->pUnk184_kan_t_ptr = pData + 1;
        pData = (ULONG *)((UCHAR *)pData + *pData);
    }
    if (G.pOld_tmd != pData) {
        G.pOld_tmd = pData;
        Mapping_md(pData, pEm->Tpage, pEm->Clut);
    }
    *(ULONG *)&pEm->Routine_0 = 0;
    pEm->pTmd = pData + 2;
    pEm->Poly_rgb = 0x808080;
    if (pEm->Id == 1) {
        G.pEm_mem = (ENEMY_WORK *)PresetObjectGT4_mizu((MD_WORK2 *)pEm, (POLY_GT4 *)G.pEm_mem); /* MD_WORK2: the model view of the work */
    } else {
        G.pEm_mem = (ENEMY_WORK *)Om_parts_work_set(pEm, (PARTS_W *)G.pEm_mem);
        Om_parts_link((OBJ_MODEL_WORK *)pEm, pEm->pUnk184_kan_t_ptr);
    }
}

/* take Parts_num PARTS_W for an object model; returns the next free address */
PARTS_W *Om_parts_work_set(ENEMY_WORK *pEm, PARTS_W *pParts)
{
    pEm->Parts_num = ((UCHAR *)pEm->pTmd)[-4];
    *(PARTS_W **)&pEm->Spd_z = pParts;
    return pParts + pEm->Parts_num;
}

INCLUDE_ASM("asm/nonmatchings/game/om", Om_parts_link);

/* draw the parts of an object model unless pPos is outside the cut (Be_flg 0x40: always) */
void Om_joint_trans2(OBJ_MODEL_WORK *pOm, PARTS_W *pParts, ULONG Flg, LONG *pPos)
{
    ULONG n;

    if ((pOm->Be_flg & 0x40) || Hit_ck_point4(pPos, G.Cccut)) {
        n = pOm->Parts_num;
    loop:
        if (pParts->Be_flg & 1) {
            Om_joint_trans((ENEMY_WORK *)pOm, pParts, (SHORT)pParts->UnkA2, &pParts->Workm);
        }
        pParts++;
        if (--n != 0) {
            goto loop;
        }
    }
}

/* colour change: point every polygon at the colour-change CLUT row (RE2 Col_chg_init) */
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
    Clut = GetClut(0x120, G_esp_tex_h + 0x1E0 + Col_wk_no) & 0xFFFF;
    if (pMw->Unk4A != 1) {
        Poly_num = (USHORT)pMw->pTmd->n_prim;
        p = pMw->pTmd->pPrim3;
        if (Poly_num != 0)
            D0 = *p >> 22;
        for (i = 0; i < Poly_num; i++) {
            *p = (*p & 0xffff) | (Clut << 16);
            p += 3;
        }
        Poly_num = pMw->pTmd->n_prim >> 16;
        p = pMw->pTmd->pPrim4;
        if (Poly_num != 0)
            D0 = *p >> 22;
        for (i = 0; i < Poly_num; i++) {
            *p = (*p & 0xffff) | (Clut << 16);
            p += 4;
        }
    } else {
        Poly_num = pMw->pTmd->n_prim >> 16;
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

/* colour change step: CLUT source rect for Ctr (RE2 Col_chg) */
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

/* texture change init (RE2 Tex_chg_init) */
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

/* texture change step: source rect for Ctr (RE2 Tex_chg) */
ULONG Tex_chg(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr)
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

/* init of the texture scroll + bob change */
void Uv_chg_init(ULONG Col_wk_no, CC_PARTS_WORK *pCcp)
{
    pCcp->Cc_ctr = 1;
    pCcp->Cc_pos_x = 0;
    pCcp->Cc_pos_y = 0;
}

/* RE3 colour change: scroll the texture v of every quad by Cc_num (every Cc_cnt + 1 frames)
 * and bob the model up and down by Cc_wait, reversing every 0x33 frames */
void Uv_chg_ctr(CC_PARTS_WORK *pCcp)
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
    n = pOm->pTmd->n_prim >> 16;
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

/* run the colour/texture change works: DR_MOVE copies and Uv_chg_ctr (RE2 Tex_ctr) */
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
            Uv_chg_ctr(pCcp);
        }
        pCcp++;
        i++;
    } while (i < D1);
}

/* run Om_routine[Routine_0] of every object model, then the push check (RE2 Om_move) */
void Om_move(void)
{
    OBJ_MODEL_WORK *pOm;

    if (G.pRoom->nOmodel == 0) {
        return;
    }
    if (G.Stop_flg & 0x8000000) {
        return;
    }
    Om_push_num = 0;
    pOm_push = NULL;
    G.Status_flg &= ~0x10;
    Tex_ctr();
    for (pOm = G.Ob_model; pOm != G.pObj_work_end; pOm++) {
        Om_routine[pOm->Routine_0](pOm);
    }
    Om_push_ck();
}

/* a pushable object model stays in place while the player pushes it; after 6 frames the push starts */
void Om_push_ck(void)
{
    OBJ_MODEL_WORK *pOm;
    SHORT Pos_x;
    SHORT Pos_z;

    if (Om_push_num == 1 && pOm_push != NULL) {
        pOm = pOm_push;
        if (!(pOm->Be_flg & 0x4000)) {
            if (pOm->Be_flg & 0x100) {
                Pos_x = pOm->M.t[0];
                Pos_z = pOm->M.t[2];
                if (++pOm->Push_cnt >= 6) {
                    if (pOm->UnkAE & 0x20) {
                        pOm->Push_cnt = 0;
                    } else {
                        Om_push_num = 0;
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

/* Om_routine 1 from now on */
void Om_move_start(OBJ_MODEL_WORK *pOm)
{
    pOm->Routine_0 = 1;
}

/* direction a pushed box tips over: away from the player along the nearer axis (RE2 Om_set_fall_dir) */
void Om_set_fall_dir(ENEMY_WORK *pEm)
{
    SHORT dx;
    SHORT dz;

    dx = pEm->Pos_x - G.Player.Pos_x;
    dz = pEm->Pos_z - G.Player.Pos_z;
    if (__builtin_abs(dx) < 900) {
        Om_fall_dir_x = 0;
        Om_fall_dir_z = (dz >> 31) | 1;
    } else {
        Om_fall_dir_z = 0;
        Om_fall_dir_x = (dz >> 31) | 1;
    }
    Om_fall_cdir_x = pEm->Cdir_x;
    Om_fall_cdir_z = pEm->Cdir_z;
    pEm->Routine_0 = 2;
}

INCLUDE_ASM("asm/nonmatchings/game/om", Om_explosion);

/* Om_routine 0: push bookkeeping; when its target flag is set, explode or start moving by Type */
void Om_rtn_wait(OBJ_MODEL_WORK *pOm)
{
    tagGLOBAL *pG;

    pG = &G;
    if (!(pOm->Be_flg & 1)) {
        return;
    }
    if (pG->Player.Routine_1 != 1 && pG->Player.Routine_1 != 9) {
        pOm->Push_cnt = 0;
    }
    if ((pOm->UnkAE & 0x10) && (pOm->UnkBA & 0x8000)) {
        Om_push_num++;
        if (pG->Player.nFloor == pOm->nFloor) {
            pOm_push = pOm;
        }
    }
    if ((pOm->Unk4A & 4) && *G.pOm_target[pOm->UnkC1] == 0xFF) {
        switch (pOm->Type & 0xC0) {
        case 0:
            Om_explosion(pOm, 1, 1);
            break;
        case 0x40:
            Om_move_start(pOm);
            break;
        case 0x80:
            break;
        }
        Flg_on(Om_save_flg, pOm->UnkC2);
    }
}

/* OM.C called the ground check with three arguments (its own declaration); the canonical one has four */
LONG Sca_get_ground3(LONG *pPos, ULONG W, ULONG Arg2) __asm__("Sca_get_ground");

/* Om_routine 1: drop to the ground and land */
void Om_rtn_drop(OBJ_MODEL_WORK *pOm)
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
                pOm->Unk192 = Sca_get_ground3(pOm->M.t, 0x1C2, 0);
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
            Om_explosion(pOm, 0, 0);
        }
        break;
    case 2:
        pOm->M.t[1] -= 30;
        pOm->Routine_1++;
        Om_explosion(pOm, 0, 0);
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

INCLUDE_ASM("asm/nonmatchings/game/om", Om_rtn_fall);

/* Om_routine 2: a pushed box tips over the edge (RE2 Om_move_box) */
void Om_move_box(OBJ_MODEL_WORK *pOm)
{
    SHORT n;
    LONG Dummy[4];

    switch (pOm->Routine_1) {
    case 0:
        Om_dir_sum = 0;
        Om_pos_spd = Om_pos_spd_add = 13;
        Om_dir_spd = 0;
        Om_dir_spd_add = 16;
        Om_add_dir.vx = 0;
        Om_add_dir.vy = 0;
        Om_add_dir.vz = 0;
        Om_fall_spd = 0;
        Om_in_water_flg = 0;
        pOm->Routine_1 = 1;
        pOm->Routine_2 = 0;
    case 1:
        pOm->M.t[0] += Om_pos_spd * Om_fall_dir_x;
        pOm->M.t[2] += Om_pos_spd * Om_fall_dir_z;
        if (Om_dir_sum < 0x400) {
            n = Om_dir_sum + Om_dir_spd;
            Om_dir_sum = n;
            if (n > 0x400) {
                Om_dir_spd -= n - 0x400;
            }
            Om_add_dir.vx = Om_dir_spd * -Om_fall_dir_z;
            Om_add_dir.vz = Om_dir_spd * Om_fall_dir_x;
        } else {
            Om_add_dir.vz = 0;
            Om_add_dir.vx = 0;
        }
        Om_dir_spd += Om_dir_spd_add;
        Om_pos_spd += Om_pos_spd_add;
        if (++pOm->Routine_2 >= 16) {
            Om_pos_spd_add = 0;
            pOm->Routine_0 = 0;
            pOm->Routine_1 = 0;
        }
        break;
    }
    Rot_add_matrix(pOm->M.t, &pOm->Cdir.vx, &pOm->M, &Om_add_dir);
}

typedef struct {                       /* object model part, 0xBC bytes (Om_explosion_set) */
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

/* Om_routine 3: the parts fly apart (PW_BOMB) and hurt around for 10 frames (RE2 Om_explosion_set) */
void Om_explosion_set(OBJ_MODEL_WORK *pOm)
{
    HIT_REQ Req;
    PARTS_W *pP;
    UCHAR Num;
    UCHAR Idx;
    ULONG k;
    ULONG n;

    pP = pOm->pParts;
    Num = Om_bomb_parts[pOm->Type & 0x1F].Num;
    Idx = Om_bomb_parts[pOm->Type & 0x1F].Idx;
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
            PW_BOMB(pP)->V0_x = pOm->Cdir.vy + 0x800;
            PW_BOMB(pP)->Cnt0 = 0x8000;
            PW_BOMB(pP)->V0_z = 0;
            PW_BOMB(pP)->V0_y = 0;
            PW_BOMB(pP)->G = 0;
            PW_BOMB(pP)->Gxz = 0;
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
            Pos_em_at_ck(&Req, Om_at_tbl[pOm->Type & 0xF]);
            Pos_om_at_ck(&Req, Om_at_tbl[pOm->Type & 0xF]);
        } else {
            pOm->Routine_2 = 2;
        }
        break;
    case 2:
        break;
    }
}

/* Om_routine 5: aim target off while Routine_1 is 1 */
void Om_rtn_target_off(OBJ_MODEL_WORK *pOm)
{
    switch (pOm->Routine_1) {
    case 0:
        *G.pOm_target[pOm->UnkC1] = 0xFF;
        pOm->Routine_1 = 1;
        break;
    case 1:
        break;
    case 2:
        *G.pOm_target[pOm->UnkC1] = 0;
        pOm->Routine_0 = 0;
        pOm->Routine_1 = 0;
        break;
    }
}

/* OM.C passed Joint_trans an object model and a fourth argument (pPos), which the 3-argument canonical
 * prototype cannot express without changing every other caller */
void Joint_trans_om(OBJ_MODEL_WORK *pOm, struct tagPARTS_W *pParts, ULONG Be_flg, LONG *pPos) __asm__("Joint_trans");

/* matrices and drawing of every visible object model (RE2 Om_trans) */
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
        CompM(pOm->pSuper, &pOm->M, &pOm->Workm);
        Set_light_pos(WORKM_POS_V(pOm));
        SetColorMatrix(&COLORMATRIX);
        if (pOm->Unk4A == 2)
            pOm->Workm.t[1] -= 900;
        if (pOm->Unk4A == 1) {
            MulMatrix0(&LIGHTMATRIX, &pOm->Workm, &M);
            SetLightMatrix(&M);
            CompM(&GsWSMATRIX, &pOm->Workm, &M);
            SetRotMatrix(&M);
            SetTransMatrix(&M);
            Mizu_trans((ENEMY_WORK *)pOm, 1); /* the model work shares the ENEMY_WORK head */
        } else {
            Joint_trans_om(pOm, pOm->pParts, pOm->Be_flg, pOm->Workm.t);
            Om_joint_trans2(pOm, pOm->pParts, pOm->Be_flg, pOm->Workm.t);
            pOm->Be_flg &= 0xF0FFFFFF;
        }
        pOm->Old_x = pOm->M.t[0];
        pOm->Old_y = pOm->M.t[1];
        pOm->Old_z = pOm->M.t[2];
    }
}

/* the object model of kind 8 falls where pEm is (Om_routine 4); logged in G.Chaser_drop */
void Om_drop_set(ENEMY_WORK *pEm, ULONG Arg1)
{
    ENEMY_WORK **pp;
    OBJ_MODEL_WORK *p;
    UCHAR *pAot;
    ULONG i;
    tagGLOBAL *pG = &G;

    pG->Status_flg &= ~0x2000;
    pAot = (UCHAR *)G.Sce_aot[31];
    for (pp = &pG->pEnemy[2]; pp != pG->ppEnemy_end; pp++) {
        if ((*pp)->Id == pEm->Id)
            break;
    }
    i = 0;
    for (p = G.Ob_model; p != G.pObj_work_end; p++, i++) {
        if ((p->Be_flg & 1) && p->Unk4A == 8)
            break;
    }
    pAot[3] = i | 0x80;
    G.Chaser_drop[G.Chaser_drop_num].Be_flg = 1;
    G.Chaser_drop[G.Chaser_drop_num].Item_id = pAot[0xC];
    G.Chaser_drop[G.Chaser_drop_num].Room_id = G.Room_no | ((G.Stage_no + 1) << 8);
    G.Chaser_drop[G.Chaser_drop_num].Pos_x = pEm->Pos_x;
    G.Chaser_drop[G.Chaser_drop_num].Pos_y = pEm->Pos_y;
    G.Chaser_drop[G.Chaser_drop_num].Pos_z = pEm->Pos_z;
    p->Routine_0 = 4;
    p->Routine_1 = 0;
    p->Routine_2 = Arg1;
    G.Chaser_drop_num++;
}

/* ordering table of every live work and object model: by position, or the fixed one (Be_flg 0x80) */
void Ot_set_all(void)
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
                pEm->pUnk7C = pG->pOt_sub[pG->Cbuf_id][pEm->UnkD];
            } else {
                pEm->pUnk7C = Ot_get_pos(&pEm->Pos_x);
            }
        }
    }
    for (pOm = pG->Ob_model; pOm != pG->pObj_work_end; pOm++) {
        if (pOm->Be_flg & 1) {
            if (pOm->Be_flg & 0x80) {
                pOm->pUnk7C = pG->pOt_sub[pG->Cbuf_id][pOm->UnkD];
            } else {
                pOm->pUnk7C = Ot_get_pos(pOm->M.t);
            }
        }
    }
}

/* ordering table of the room block that holds pPos (G.pOt1 if none) */
ULONG *Ot_get_pos(LONG *pPos)
{
    tagGLOBAL *pG;
    USHORT *pHead;
    UCHAR *p;
    ULONG n;
    USHORT *pBlk;

    pG = &G;
    if (pG->Ot_sub_num == 0) {
        goto end;
    }
    pHead = (USHORT *)pG->pRoom->pUnk40;
    if (pG->Cut_no < pHead[1]) {
        goto start;
    }
    goto end;
found:
    return pG->pOt_sub[pG->Cbuf_id][((USHORT *)p)[1]];
start:
    pBlk = (USHORT *)((UCHAR *)pHead + ((ULONG *)pHead)[pG->Cut_no + 1]);
    n = *pBlk;
    p = (UCHAR *)(pBlk + 2);
    for (; n != 0; n--) {
        if (p[1] == 0) {
            if (!(p[0] & 2) && Hit_ck_box(pPos, (SHORT *)(p + 4))) {
                goto found;
            }
            p += 12;
        } else {
            if (!(p[0] & 2) && Hit_ck_point4(pPos, (VCUT *)p)) {
                goto found;
            }
            p += 20;
        }
    }
end:
    return pG->pOt1;
}

/* room OT blocks: Unk2470 extra 0x400-entry OTs per buffer (pOt_sub[][1..]) from the enemy work memory */
void Ot_sub_init(void)
{
    tagGLOBAL *pG;
    ROOM_WORK *pRoom;
    USHORT *pData;
    ULONG n;
    ULONG i, j;

    pG = &G;
    pRoom = pG->pRoom;
    pG->Ot_sub_num = 0;
    pData = (USHORT *)pRoom->pUnk40;
    if (pData == NULL) {
        return;
    }
    n = *pData;
    if (n == 0) {
        pRoom->pUnk40 = NULL;
        return;
    }
    j = 0;
    do {
        pG->pOt_sub[j][0] = pG->Ot1[j];
        i = 1;
        do {
            pG->pOt_sub[j][i] = (ULONG *)pG->pEm_mem;
            pG->pEm_mem = (ENEMY_WORK *)((UCHAR *)pG->pEm_mem + 0x1000);
            i++;
        } while (i < n + 1);
        j++;
    } while (j < 2);
    pG->Ot_sub_num = n;
    i = 0;
    do {
        j = 0;
        do {
            ClearOTagR(pG->pOt_sub[i][j + 1], 0x400);
            j++;
        } while (j < n);
        i++;
    } while (i < 2);
}
