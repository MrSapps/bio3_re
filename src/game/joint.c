#include "pl_wep.h"
#include "sub.h"
#include "joint.h"
#include "sca.h"
#include "pcstr.h"
#include "sld.h"

/*
 * JOINT.C (RE2 JOINT.C): the model part tree of a work. Joint_trans builds the part matrices,
 * Joint_trans2 / ko_joint_trans2 light and sort them, PartsWork_set/PartsWork_link set the
 * tree up, Joint_move / Joint_move2 / Joint_move_p play kan/seq animations (J_seq*, with
 * interpolated frames in J_seq*_hokan), Parts_down / Parts_bomb move detached parts.
 * The sort helpers take pObj = &pP->Attribute: pObj[1] is the part's pTmd, pObj[25] its Poly_rgb.
 */

/* the PARTS_W free work as RE2's raw Free[] (PartsWork_link clears it; typed views in joint.h) */
#define PW_FREE(p) ((USHORT *)&(p)->Unk74)

/* GTE inline macros (PsyQ inline_c.h with the DMPSX-converted opcodes) */
#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stsv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 2(%0); sh $14, 4(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_gpf12_b() __asm__ volatile(".word 0x4B98003D")
#define gte_gpl12() __asm__ volatile("nop; nop; .word 0x4BA8003E")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_ldsxy3(r0, r1, r2) __asm__ volatile("mtc2 %0, $12; mtc2 %2, $14; mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldsz3(r0, r1, r2) __asm__ volatile("mtc2 %0, $17; mtc2 %1, $18; mtc2 %2, $19" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile("swc2 $12, 8(%0); swc2 $13, 20(%0); swc2 $14, 32(%0)" : : "r"(r0) : "memory")
#define gte_nclip_b() __asm__ volatile(".word 0x4B400006")
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_avsz3() __asm__ volatile("nop; nop; .word 0x4B58002D")
#define gte_avsz4() __asm__ volatile("nop; nop; .word 0x4B68002E")
#define gte_rtpt() __asm__ volatile("nop; nop; .word 0x4A280030")
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetLightMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $8; ctc2 $13, $9; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $10; ctc2 $13, $11; ctc2 $14, $12" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stclmv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_rtir() __asm__ volatile("nop; nop; .word 0x4A49E012")
#define gte_MulMatrix0(r1, r2, r3) { gte_SetRotMatrix(r1); gte_ldclmv(r2); gte_rtir(); gte_stclmv(r3); gte_ldclmv((char *)(r2) + 2); gte_rtir(); gte_stclmv((char *)(r3) + 2); gte_ldclmv((char *)(r2) + 4); gte_rtir(); gte_stclmv((char *)(r3) + 4); }
#define gte_ldlvl(r0) __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0)" : : "r"(r0))
#define gte_stlvl(r0) __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(r0) : "memory")

/* body matrix (Cdir, Size scale, tilt), then the world matrix of every live part and the part scales */
void Joint_trans(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg)
{
    VECTOR Dummy;
    SVECTOR Sv;
    MATRIX M;
    VECTOR v;
    ULONG i;

    if (Be_flg & 0x10) {
        return;
    }
    RotMatrix_gte(CDIR_SV(pEm), EM_WORKM(pEm));
    if (pEm->Be_flg & 0x100000) {
        v.vx = 0x1000;
        v.vy = pEm->Size;
        v.vz = 0x1000;
        ScaleMatrix(EM_WORKM(pEm), &v);
    }
    if (pEm->Be_flg & 0x80000) {
        if (!(G.Status_flg & 0x200) && !(G.Unk246C & 0x40000000)) {
            Em_tilt_return(pEm);
        }
    } else if (pEm->Be_flg & 0x40000) {
        if (!(G.Status_flg & 0x200) && !(G.Unk246C & 0x40000000)) {
            Em_tilt_move(pEm);
        }
    } else {
        goto parts;
    }
    Sv.vy = 0;
    Sv.vx = pEm->Tilt[0];
    Sv.vz = pEm->Tilt[1];
    RotMatrix_gte(&Sv, &M);
    MulMatrix2(&M, EM_WORKM(pEm));
parts:
    i = pEm->Parts_num;
    do {
        if (pP->Be_flg & 1) {
            ko_joint_trans(pEm, pP, pP->Be_flg, &pP->Workm);
        }
        pP++;
    } while (--i != 0);
    if (pEm->Be_flg & 0x200000) {
        i = pEm->Parts_num;
        pP = pEm->pSin_parts_ptr;
loop:
        if ((pP->Be_flg & 4) && ((pP->UnkA2 & 0x10) || pP->UnkA2 == 0)) {
            v.vx = pP->Size_x;
            v.vy = pP->Size_y;
            v.vz = pP->Size_z;
            ScaleMatrix(&pP->Workm, &v);
        }
        pP++;
        if (--i != 0) {
            goto loop;
        }
    }
}

/* world matrix of one part (pSuper * M); falling (Be_flg 8) and flying (UnkA2 0x20) parts move here */
void ko_joint_trans(ENEMY_WORK *pEm, PARTS_W *pP_ptr, ULONG Be_flg, MATRIX *wm)
{
    MATRIX *pSuper;
    MATRIX *pM;
    MATRIX lm;
    VECTOR sv;
    USHORT Flg;

    Flg = pP_ptr->UnkA2;
    pSuper = pP_ptr->pSuper;
    pM = PW_M(pP_ptr);
    if ((pP_ptr->pOya_parts->Be_flg & 9) == 8) {
        pP_ptr->Be_flg = (pP_ptr->Be_flg & ~1) | 8;
        return;
    }
    if (!(Be_flg & 2)) {
        CompM(pSuper, pM, wm);
    }
    if (Be_flg & 8) {
        Parts_down(pP_ptr, wm);
        return;
    }
    if (Flg & 0x20) {
        Parts_bomb(pP_ptr);
    }
}

/* draw the parts of pEm: off screen (pPos outside the cut) only the always-drawn ones; Be_flg 8 sets Model_col */
void Joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Be_flg, VECTOR *pPos)
{
    ULONG i;

    if (Be_flg & 0x10) {
        return;
    }
    if (!(Be_flg & 0x40) && Hit_ck_point4((LONG *)pPos, G.Cccut) == 0) {
        i = pEm->Parts_num;
loop0:
        if ((pP->Be_flg & 0x21) == 0x21 && (pP->UnkA2 & 1)) {
            ko_joint_trans2(pEm, pP, pP->UnkA2, &pP->Workm);
        }
        pP++;
        if (--i != 0) {
            goto loop0;
        }
        return;
    }
    if (Be_flg & 8) {
        Model_col.Unk0[0] = pEm->Add_col[0];
        Model_col.Unk0[1] = pEm->Add_col[1];
        Model_col.Unk0[2] = pEm->Add_col[2];
        Model_col.Unk6 = (pEm->Col_flg & 0xF00) >> 8;
        i = (UCHAR)pEm->Col_flg;
        Model_col.Rgb = i | (i << 8) | (i << 16);
    }
    i = pEm->Parts_num;
loop1:
    if ((pP->Be_flg & 1) && (pP->UnkA2 & 1)) {
        ko_joint_trans2(pEm, pP, pP->UnkA2, &pP->Workm);
    }
    pP++;
    if (--i != 0) {
        goto loop1;
    }
}

/* light matrix (optionally lit from the part itself), screen matrix and sort of one part by its UnkA2 flags */
void ko_joint_trans2(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM)
{
    MATRIX M;
    VECTOR Scale;
    tagGLOBAL *pG;
    ULONG Be;

    pG = &G;
    Be = pP->Be_flg;
    if (Be & 0x20) {
        if (Hit_ck_point4(pM->t, pG->Cccut) == 0) {
            return;
        }
    }
    if (!(Flg & 0x210)) {
        if (!(Flg & 0x1000)) {
            gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
            gte_SetLightMatrix(&M);
        } else {
            Set_light_pos((VECTOR *)pM->t);
            gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
            gte_SetLightMatrix(&M);
            SetColorMatrix(&COLORMATRIX);
            Set_light_pos(POS_V(pEm));
        }
    }
    if (!(Flg & 0x10)) {
        CompM(&GsWSMATRIX, pM, &M);
        if (Be & 4) {
            Scale.vx = pP->Size_x;
            Scale.vy = pP->Size_y;
            Scale.vz = pP->Size_z;
            ScaleMatrix(&M, &Scale);
        }
        gte_SetRotMatrix(&M);
        gte_SetTransMatrix(&M);
    }
    if (Flg & 0x20) {
        pG->pPacket = Bomb_Parts_SortGT3(pP->pTmd, pG->pPacket, pP);
        pG->pPacket = Bomb_Parts_SortGT4(pP->pTmd, pG->pPacket, pP);
        Parts_bomb(pP);
    } else if (Flg & 0x10) {
        pSkin_m = &M;
        pSkin_v = &Scale;
        Parts_sort_sabun(&pP->Attribute, Flg, pP);
    } else if (Flg & 2) {
        Parts_sort(&pP->Attribute, Flg, pP);
    }
}

/* light and screen matrices of a part, then Parts_sort_ex */
void ko_joint_trans3(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM)
{
    MATRIX M;
    VECTOR M2;

    pSkin_m = &M;
    pSkin_v = &M2;
    gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
    gte_SetLightMatrix(&M);
    CompM(&GsWSMATRIX, pM, &M);
    gte_SetRotMatrix(&M);
    gte_SetTransMatrix(&M);
    if (Flg & 1) {
        if (Flg & 2) {
            Parts_sort_ex(&pP->Attribute, (SHORT)pP->UnkA2, pP);
        }
    }
}

/* sort a part: optional colour passes (Flg 0x40/0x80), the textured polygons, colour blend (Flg 4, PW_RGB) */
void Parts_sort(ULONG *pObj, ULONG Flg, PARTS_W *pP)
{
    ULONG Work0[0x70];
    USHORT Work1[0x70];
    SORT_PARAM Prm;
    tagGLOBAL *pG;
    ULONG Abr;
    POLY_GT4 *Top;

    pG = &G;
    Prm.pObj = (MD_OBJ *)pObj[1];
    Prm.pOt = pG->pEm->pUnk7C;
    Abr = (Flg & 0x100) >> 7;
    if (Flg & 0x2000) {
        Prm.pOt = Ot_get_pos(pP->Workm.t);
    }
    if (Flg & 0x400) {
        Md_rtp_floor(Work0, Work1, (MD_OBJ *)pObj[1], &pP->Workm);
    } else {
        Md_rtp(Work0, Work1, (MD_OBJ *)pObj[1]);
    }
    if (Flg & 0x40) {
        Prm.u.Rgb = Model_col.Unk6 << 24;
        pG->pPacket = SortGT3_plus_light(&Prm, pG->pPacket, Work0, Work1);
        pG->pPacket = SortGT4_plus_light(&Prm, pG->pPacket, Work0, (SHORT *)Work1);
    }
    if (Flg & 0x80) {
        Prm.u.Rgb = *(ULONG *)((UCHAR *)&Model_col + 8) | (*(SHORT *)((UCHAR *)&Model_col + 6) << 24); /* Rgb | Unk6 << 24, addressed from &Rgb */
        pG->pPacket = SortGT3_plus_col(&Prm, pG->pPacket, Work0, Work1);
        pG->pPacket = SortGT4_plus_col(&Prm, pG->pPacket, Work0, Work1);
    }
    Prm.u.Rgb = pObj[25];
    Top = pG->pPacket;
    Prm.u.c.Code = Abr | 0x34;
    pG->pPacket = SortGT3_neo_uv(&Prm, pG->pPacket, Work0, Work1);
    Prm.u.c.Code = Abr | 0x3C;
    pG->pPacket = SortGT4_neo_uv(&Prm, pG->pPacket, Work0, Work1);
    if (Flg & 4) {
        Rgb_blend((RGB_BLEND_WORK *)Top, (RGB_BLEND_WORK *)pG->pPacket, PW_RGB(pP)->Rgb0, PW_RGB(pP)->Rgb1);
    }
}

/* Parts_sort for a vertex-animated (sabun) part */
void Parts_sort_sabun(ULONG *pObj, ULONG Flg, PARTS_W *pP)
{
    ULONG Work0[0x70];
    USHORT Work1[0x70];
    SORT_PARAM Prm;
    tagGLOBAL *pG;
    POLY_GT4 *Top;

    pG = &G;
    Prm.pObj = (MD_OBJ *)pObj[1];
    Prm.pOt = pG->pEm->pUnk7C;
    if (Flg & 0x400) {
        Skin_sort_alt(Work0, Work1, pObj[1], pP);
    } else {
        Skin_sort(Work0, Work1, pObj[1], pP);
    }
    Prm.UnkC = (ULONG)&Skin_rgb;
    if (Flg & 0x40) {
        Prm.u.Rgb = Model_col.Unk6 << 24;
        pG->pPacket = SortGT3_plus_light(&Prm, pG->pPacket, Work0, Work1);
        pG->pPacket = SortGT4_plus_light(&Prm, pG->pPacket, Work0, (SHORT *)Work1);
    }
    if (Flg & 0x80) {
        Prm.u.Rgb = *(ULONG *)((UCHAR *)&Model_col + 8) | (*(SHORT *)((UCHAR *)&Model_col + 6) << 24); /* Rgb | Unk6 << 24, addressed from &Rgb */
        pG->pPacket = SortGT3_plus_col(&Prm, pG->pPacket, Work0, Work1);
        pG->pPacket = SortGT4_plus_col(&Prm, pG->pPacket, Work0, Work1);
    }
    Top = pG->pPacket;
    Prm.u.Rgb = pObj[25];
    pG->pPacket = SortGT3_sabun(&Prm, Top, Work0, Work1);
    pG->pPacket = SortGT4_sabun(&Prm, pG->pPacket, Work0, Work1);
    if (Flg & 4) {
        Rgb_blend((RGB_BLEND_WORK *)Top, (RGB_BLEND_WORK *)pG->pPacket, PW_RGB(pP)->Rgb0, PW_RGB(pP)->Rgb1);
    }
}

/* sort a part with its extra sort parameter pP->UnkB8 */
void Parts_sort_ex(ULONG *pObj, ULONG Flg, PARTS_W *pP)
{
    ULONG Work0[0x72];
    USHORT Work1[0x74];
    SORT_PARAM Prm;
    MD_OBJ *D0;

    Prm.pObj = D0 = (MD_OBJ *)pObj[1];
    Prm.u.Rgb = pObj[25];
    Prm.pOt = G.pEm->pUnk7C;
    Md_rtp(Work0, Work1, D0);
    Prm.u.c.Code = 0x34;
    Prm.UnkC = pP->UnkB8;
    G.pPacket = SortGT3_col(&Prm, G.pPacket, Work0, Work1);
    Prm.u.c.Code = 0x3C;
    G.pPacket = SortGT4_col(&Prm, G.pPacket, Work0, Work1);
}

/* sort a part without colour passes (Flg 8 picks the second sorter pair); pP unused */
void Parts_sort_simple(ULONG *pObj, ULONG Flg, PARTS_W *pP)
{
    ULONG Work0[0x70];
    USHORT Work1[0x70];
    SORT_PARAM Prm;
    ULONG Abr;

    Prm.pObj = (MD_OBJ *)pObj[1];
    Prm.pOt = G.pEm->pUnk7C;
    Prm.u.Rgb = pObj[25];
    Abr = (Flg & 0x100) >> 7;
    Md_rtp(Work0, Work1, (MD_OBJ *)pObj[1]);
    if (Flg & 8) {
        Prm.u.c.Code = Abr | 0x34;
        G.pPacket = SortGT3_neo_uv(&Prm, G.pPacket, Work0, Work1);
        Prm.u.c.Code = Abr | 0x3C;
        G.pPacket = SortGT4_neo_uv(&Prm, G.pPacket, Work0, Work1);
    } else {
        Prm.u.c.Code = Abr | 0x34;
        G.pPacket = SortGT3_lno_uv(&Prm, G.pPacket, Work0, Work1);
        Prm.u.c.Code = Abr | 0x3C;
        G.pPacket = SortGT4_lno_uv(&Prm, G.pPacket, Work0, Work1);
    }
}

/* take Parts_num PARTS_W from work_top for pEm; returns the next free address */
ULONG PartsWork_set(ENEMY_WORK *pEm, ULONG work_top)
{
    pEm->Parts_num = ((UCHAR *)pEm->pTmd)[-4];
    pEm->pSin_parts_ptr = (PARTS_W *)work_top;
    return work_top + pEm->Parts_num * sizeof(PARTS_W);
}

/* set up the part tree from the kan: models, rest positions, parents (pSuper / pOya_parts) */
ULONG PartsWork_link(ENEMY_WORK *pEm, ULONG work_top, ULONG *pKan)
{
    LONG Unused[28];
    ULONG Link_cnt;
    ULONG Part_no;
    PARTS_W *pTop;
    PARTS_W *pCur;
    UCHAR *pLink_top;
    UCHAR *pChild;
    MD_OBJ *pTmd_cur;
    ULONG *pLink;
    ULONG n;
    ULONG Cnt;
    SHORT *pPos;

    Part_no = 0;
    pPos = (SHORT *)pKan + 4;
    n = pEm->Parts_num;
    pCur = pEm->pSin_parts_ptr;
    pTmd_cur = (MD_OBJ *)pEm->pTmd;
    pLink_top = (UCHAR *)pKan + (*(USHORT *)pKan & 0xFFFC);
    pLink = (ULONG *)pLink_top;
    Link_cnt = ((USHORT *)pKan)[2];
    pTop = pCur;
    pCur->pSuper = (MATRIX *)pEm->Workm_m;
    pCur->pOya_parts = (PARTS_W *)pEm;
    while (n-- != 0) {
        Copy_matrix(&GsIDMATRIX, &pCur->Workm);
        Copy_rot_matrix(&GsIDMATRIX, PW_M(pCur));
        pCur->Poly_rgb = 0x808080;
        pCur->Attribute = 0;
        *(ULONG *)&pCur->Cdir_x = 0;
        pCur->Cdir_z = 0;
        PW_FREE(pCur)[0] = 0; /* RE2 PartsWork_link clears Free[0..5] */
        PW_FREE(pCur)[1] = 0;
        PW_FREE(pCur)[2] = 0;
        PW_FREE(pCur)[3] = 0;
        PW_FREE(pCur)[4] = 0;
        PW_FREE(pCur)[5] = 0;
        pCur->Be_flg = 1;
        *(SHORT *)&pCur->Workm.t[0] = 0;
        pCur->Parts_no = Part_no;
        pCur->UnkA2 = 0xB;
        pCur->pTmd = pTmd_cur;
        if ((USHORT)pTmd_cur->n_vert < 4) {
            pCur->UnkA2 = 0;
            pTmd_cur->n_vert = 0;
        }
        pCur->Old_x = pCur->Pos_x = pPos[0];
        pCur->Old_y = pCur->Pos_y = pPos[1];
        pCur->Old_z = pCur->Pos_z = pPos[2];
        pPos += 3;
        if (Link_cnt != 0) {
            pChild = pLink_top + (*pLink >> 16);
            Cnt = *pLink & 0xFFFF;
            while (Cnt-- != 0) {
                pTop[*pChild].pSuper = &pCur->Workm;
                pTop[*pChild].pOya_parts = pCur;
                pChild++;
            }
            pLink++;
            Link_cnt--;
        } else {
            pCur->pSuper = &GsIDMATRIX;
            pCur->pOya_parts = NULL;
        }
        pCur++;
        pTmd_cur++;
        Part_no++;
    }
    return work_top;
}

/* reset every part to its rest position from the kan */
void Parts_work_pos_init(ENEMY_WORK *pEm, SHORT *pKan)
{
    SHORT *pPos;
    ULONG n;
    PARTS_W *pCur;

    pPos = &pKan[4];
    n = pEm->Parts_num;
    pCur = pEm->pSin_parts_ptr;
    while (n-- != 0) {
        pCur->Old_x = pCur->Pos_x = pPos[0];
        pCur->Old_y = pCur->Pos_y = pPos[1];
        pCur->Old_z = pCur->Pos_z = pPos[2];
        pPos += 3;
        pCur++;
    }
}

/* start tilting the body (Be_flg 0x40000) up to Max by Spd per frame, direction in Flg */
void Em_tilt_set(ENEMY_WORK *pEm, SHORT Max, SHORT Spd, UCHAR Flg)
{
    pEm->Tilt_max = Max;
    pEm->Tilt_flg = Flg;
    pEm->Tilt_spd = Spd;
    pEm->Be_flg = (pEm->Be_flg | 0x40000) & ~0x80000;
}

/* one tilt step in the Tilt_flg direction, clamped to Tilt_max; flag 8 switches to Em_tilt_return */
void Em_tilt_move(ENEMY_WORK *pEm)
{
    UCHAR Flg;
    LONG d;
    SHORT a;

    Flg = pEm->Tilt_flg;
    if (Flg & 8) {
        pEm->Be_flg = (pEm->Be_flg & ~0x40000) | 0x80000;
        return;
    }
    if (Flg & 2) {
        if (Flg & 4) {
            if (Flg & 1) {
                pEm->Tilt[1] += pEm->Tilt_spd;
            } else {
                pEm->Tilt[1] -= pEm->Tilt_spd;
            }
        } else {
            pEm->Tilt[1] = 0;
        }
        if (pEm->Tilt_flg & 1) {
            pEm->Tilt[0] -= pEm->Tilt_spd;
        } else {
            pEm->Tilt[0] += pEm->Tilt_spd;
        }
    } else {
        if (Flg & 4) {
            if (Flg & 1) {
                pEm->Tilt[0] += pEm->Tilt_spd;
            } else {
                pEm->Tilt[0] -= pEm->Tilt_spd;
            }
        } else {
            pEm->Tilt[0] = 0;
        }
        if (pEm->Tilt_flg & 1) {
            pEm->Tilt[1] += pEm->Tilt_spd;
        } else {
            pEm->Tilt[1] -= pEm->Tilt_spd;
        }
    }
    d = pEm->Tilt[0];
    if (pEm->Tilt_max < __builtin_abs(d)) {
        a = pEm->Tilt_max;
        if (d <= 0) {
            a = -a;
        }
        pEm->Tilt[0] = a;
    }
    d = pEm->Tilt[1];
    if (pEm->Tilt_max < __builtin_abs(d)) {
        a = pEm->Tilt_max;
        if (d <= 0) {
            a = -a;
        }
        pEm->Tilt[1] = a;
    }
}

/* bring the tilt back to 0 by Tilt_spd; clears Be_flg 0x80000 when level */
void Em_tilt_return(ENEMY_WORK *pEm)
{
    pEm->Be_flg &= ~0x40000;
    if (*(ULONG *)pEm->Tilt != 0 /* either tilt left */) {
        if (pEm->Tilt[0] > 0) {
            if (pEm->Tilt_spd < pEm->Tilt[0]) {
                pEm->Tilt[0] -= pEm->Tilt_spd;
            } else {
                pEm->Tilt[0] = 0;
            }
        } else {
            if (pEm->Tilt_spd < -pEm->Tilt[0]) {
                pEm->Tilt[0] += pEm->Tilt_spd;
            } else {
                pEm->Tilt[0] = 0;
            }
        }
        if (pEm->Tilt[1] > 0) {
            if (pEm->Tilt_spd < pEm->Tilt[1]) {
                pEm->Tilt[1] -= pEm->Tilt_spd;
            } else {
                pEm->Tilt[1] = 0;
            }
        } else {
            if (pEm->Tilt_spd < -pEm->Tilt[1]) {
                pEm->Tilt[1] += pEm->Tilt_spd;
            } else {
                pEm->Tilt[1] = 0;
            }
        }
    } else {
        pEm->Be_flg &= ~0x80000;
    }
}

/* pM2 = rotation of pM0 blended towards pM1 by Rate (0..0x1000), renormalised (RE2 HMatrix) */
void HMatrix(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate)
{
    SVECTOR Sv;
    LONG Rate0;

    Rate0 = 0x1000 - Rate;
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[0][0]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[0][0]);
    gte_gpl12();
    gte_stsv(&pM2->m[0][0]);
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[1][1]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[1][1]);
    gte_gpl12();
    gte_stsv(&pM2->m[1][1]);
    Sv.vx = pM0->m[1][0];
    Sv.vy = pM0->m[2][1];
    Sv.vz = pM0->m[2][2];
    gte_lddp(Rate0);
    gte_ldsv(&Sv);
    gte_gpf12();
    Sv.vx = pM1->m[1][0];
    Sv.vy = pM1->m[2][1];
    Sv.vz = pM1->m[2][2];
    gte_lddp(Rate);
    gte_ldsv(&Sv);
    gte_gpl12();
    gte_stsv(&Sv);
    pM2->m[1][0] = Sv.vx;
    pM2->m[2][1] = Sv.vy;
    pM2->m[2][2] = Sv.vz;
    MatrixNormal_0(pM2, pM2);
}

/* HMatrix that also blends the translation (RE2 HMatrix1) */
void HMatrix1(MATRIX *pM0, MATRIX *pM1, MATRIX *pM2, LONG Rate)
{
    SVECTOR Sv;
    LONG Rate0;

    Rate0 = 0x1000 - Rate;
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[0][0]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[0][0]);
    gte_gpl12();
    gte_stsv(&pM2->m[0][0]);
    gte_lddp(Rate0);
    gte_ldsv(&pM0->m[1][1]);
    gte_gpf12();
    gte_lddp(Rate);
    gte_ldsv(&pM1->m[1][1]);
    gte_gpl12();
    gte_stsv(&pM2->m[1][1]);
    Sv.vx = pM0->m[1][0];
    Sv.vy = pM0->m[2][1];
    Sv.vz = pM0->m[2][2];
    gte_lddp(Rate0);
    gte_ldsv(&Sv);
    gte_gpf12();
    Sv.vx = pM1->m[1][0];
    Sv.vy = pM1->m[2][1];
    Sv.vz = pM1->m[2][2];
    gte_lddp(Rate);
    gte_ldsv(&Sv);
    gte_gpl12();
    gte_stsv(&Sv);
    pM2->m[1][0] = Sv.vx;
    pM2->m[2][1] = Sv.vy;
    pM2->m[2][2] = Sv.vz;
    MatrixNormal_2(pM2, pM2);
    Sv.vx = pM0->t[0];
    Sv.vy = pM0->t[1];
    Sv.vz = pM0->t[2];
    gte_lddp(Rate0);
    gte_ldsv(&Sv);
    gte_gpf12();
    Sv.vx = pM1->t[0];
    Sv.vy = pM1->t[1];
    Sv.vz = pM1->t[2];
    gte_lddp(Rate);
    gte_ldsv(&Sv);
    gte_gpl12();
    gte_stsv(&Sv);
    pM2->t[0] = Sv.vx;
    pM2->t[1] = Sv.vy;
    pM2->t[2] = Sv.vz;
}

/* play one frame of pEm's motion (RE2 Joint_move); 1 at the end of the motion */
ULONG Joint_move(ENEMY_WORK *pEm, ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, ULONG Late_flg)
{
    USHORT *p;
    ULONG f_num;

    p = (USHORT *)((UCHAR *)Seq_t_ptr + pEm->Move_no * 8);
    f_num = *p++;
    if (Late_flg & 0xFFFF0000) {
        p = (USHORT *)((UCHAR *)Seq_t_ptr + *p) + (f_num - pEm->Move_cnt - 1);
    } else {
        p = (USHORT *)((UCHAR *)Seq_t_ptr + *p) + pEm->Move_cnt;
    }
    if (*p & 0x100) {
        if (!(*pEm->pNow_seq & 0x100)) {
            Hokan_key_set(Kan_t_ptr, Seq_t_ptr, pEm->pNow_seq, Late_flg >> 16);
        }
        pEm->pNow_seq = p;
        return J_seq_hokan(Kan_t_ptr, Seq_t_ptr, *(UCHAR *)p, Late_flg);
    } else {
        pEm->pNow_seq = p;
        return J_seq(Kan_t_ptr, Seq_t_ptr, p, Late_flg);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/joint", J_seq);

/* pOut = rotation pCur blended towards pDst by Rate, the short way round each axis (RE2 hokan_svec) */
void hokan_svec(SVECTOR *pCur, SVECTOR *pDst, SVECTOR *pOut, LONG Rate)
{
    LONG dx;
    LONG dy;
    LONG dz;

    *(ULONG *)&pCur->vx &= 0xFFF0FFF;
    pCur->vz &= 0xFFF;
    gte_lddp(0x1000 - Rate);
    gte_ldsv(pCur);
    dx = pDst->vx - pCur->vx + 0x800;
    dy = pDst->vy - pCur->vy + 0x800;
    dz = pDst->vz - pCur->vz + 0x800;
    if ((ULONG)dx > 0x1000) {
        pDst->vx += (dx < 0) ? 0x1000 : -0x1000;
    }
    if ((ULONG)dy > 0x1000) {
        pDst->vy += (dy >= 0) ? -0x1000 : 0x1000;
    }
    if ((ULONG)dz > 0x1000) {
        pDst->vz += (dz >= 0) ? -0x1000 : 0x1000;
    }
    gte_gpf12_b();
    gte_lddp(Rate);
    gte_ldsv(pDst);
    gte_gpl12();
    gte_stsv(pOut);
}



/* interpolated frame of Joint_move: root and part rotations = key + step * No / 256 */
ULONG J_seq_hokan(ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, LONG No, ULONG Late_flg)
{
    ENEMY_WORK *pEm;
    PARTS_W *pP;
    PARTS_W *pP_walk;
    ULONG Max;
    ULONG n;
    LONG w;
    VECTOR V;

    pEm = G.pEm;
    Max = *(USHORT *)((UCHAR *)Seq_t_ptr + pEm->Move_no * 8);
    if (Late_flg & 0xFFFF0000) {
        No = ~No;
        No &= 0xFF;
    }
    pP = pEm->pSin_parts_ptr;
    pP->Pos_y = pP->Pos_base_y + ((pP->Pos_step_y * No) >> 8);
    n = *(USHORT *)((UCHAR *)Kan_t_ptr + 4);
    pP_walk = pP;
    if (pEm->Hokan_flg == 0) {
        do {
            pP_walk->Cdir_x = pP_walk->Rot_x + ((pP_walk->Rot_spd_x * No) >> 8);
            pP_walk->Cdir_y = pP_walk->Rot_y + ((pP_walk->Rot_spd_y * No) >> 8);
            pP_walk->Cdir_z = pP_walk->Rot_z + ((pP_walk->Rot_spd_z * No) >> 8);
            if (!(pP_walk->Be_flg & 0x10)) {
                RotMatrix_gte(CDIR_SV(pP_walk), PW_M(pP_walk));
            }
            pP_walk++;
        } while (--n);
    } else {
        Late_flg &= 0xFFFF;
        w = Late_flg * pEm->Hokan_flg;
        V.vx = pP->Old_x;
        V.vy = pP->Old_y;
        V.vz = pP->Old_z;
        gte_lddp(0x1000 - w);
        gte_ldlvl(&pP->Pos_x);
        gte_gpf12();
        gte_lddp(w);
        gte_ldlvl(&V);
        gte_gpl12();
        gte_stlvl(&pP->Pos_x);
        pEm->Hokan_flg--;
        Parts_hokan(n, pP, No, Late_flg);
    }
    pP->Old_x = pP->Pos_x;
    pP->Old_y = pP->Pos_y;
    pP->Old_z = pP->Pos_z;
    if (++pEm->Move_cnt < Max) {
        return 0;
    }
    pEm->Move_cnt = 0;
    return 1;
}

/* falling part: bounces on the floor (Timer3 bounces) and blends into its fall rotation (RE2 Parts_down) */
void Parts_down(PARTS_W *pP_ptr, MATRIX *wm)
{
    MATRIX M;
    SVECTOR Sv;
    VECTOR Pos;
    LONG Floor_y;

    if (pP_ptr->Timer3 == 0) {
        Sv.vy = pP_ptr->Old_x;
        Sv.vz = 0;
        Sv.vx = 0;
        RotMatrix(&Sv, &M);
        Sv.vx = pP_ptr->Old_z;
        Sv.vy = pP_ptr->Old_y;
        ApplyMatrixSV(&M, &Sv, (SVECTOR *)&pP_ptr->Old_x);
        pP_ptr->Timer3 = 3;
        pP_ptr->Timer2 = 0xF;
    }
    if (pP_ptr->Timer3 > 1) {
        Pos = *WORKM_POS_V(pP_ptr);
        Floor_y = Sca_get_ground(pP_ptr->Workm.t, 100, 0x2000, 1);
        wm->t[0] += pP_ptr->Old_x;
        pP_ptr->Old_y += (signed char)pP_ptr->Timer1;
        wm->t[1] += pP_ptr->Old_y;
        wm->t[2] += pP_ptr->Old_z;
        if (Sca_get_ground(pP_ptr->Workm.t, 100, 0x2000, 1) < Floor_y) {
            *WORKM_POS_V(pP_ptr) = Pos;
            pP_ptr->Old_x >>= 2;
            pP_ptr->Old_z >>= 2;
            pP_ptr->Old_x *= -1;
            pP_ptr->Old_z *= -1;
        }
        if (Floor_y < wm->t[1]) {
            pP_ptr->Timer3--;
            wm->t[1] -= pP_ptr->Old_y;
            pP_ptr->Old_y = (SHORT)(pP_ptr->Old_y * -1) >> 2;
            pP_ptr->Old_x >>= 2;
            pP_ptr->Old_z >>= 2;
            Esp_call(0x1900, 0x7D00000 | (USHORT)(Rnd() << 4), &pP_ptr->Workm, NULL, 0);
            if (pP_ptr->Timer3 == 1) {
                pP_ptr->Be_flg &= ~1;
                Esp_kill(0xFF05, &pP_ptr->Workm);
            }
        }
    }
    if (pP_ptr->Rot_spd_x != 0xFF && pP_ptr->Timer2 >= 0) {
        Sv.vx = pP_ptr->Rot_spd_x;
        Sv.vy = pP_ptr->Rot_spd_y;
        Sv.vz = pP_ptr->Rot_spd_z;
        RotMatrix(&Sv, &M);
        HMatrix(&pP_ptr->Workm, &M, &pP_ptr->Workm, 0x1000 - pP_ptr->Timer2 * 0x100);
        pP_ptr->Timer2--;
    }
}

/* part flying off in an explosion: moves along Dir_y, falls, disappears after 0x1D frames (RE2 Parts_bomb) */
void Parts_bomb(PARTS_W *pP_ptr)
{
    MATRIX M;
    SVECTOR Sv;
    SVECTOR Sv1;
    SHORT Spd_y;
    USHORT Spd;

    if ((PW_BOMB(pP_ptr)->Cnt0 & 0x7FFF) >= 0x1D) {
        pP_ptr->Be_flg = 0;
        Esp_kill(0xFF05, &pP_ptr->Workm);
    } else {
        PW_BOMB(pP_ptr)->Cnt0++;
    }
    Spd_y = PW_BOMB(pP_ptr)->V0_y + PW_BOMB(pP_ptr)->G;
    PW_BOMB(pP_ptr)->V0_y = Spd_y;
    pP_ptr->Workm.t[1] += Spd_y;
    Sv.vy = PW_BOMB(pP_ptr)->V0_x;
    Sv.vz = 0;
    Sv.vx = 0;
    RotMatrix(&Sv, &M);
    Spd = PW_BOMB(pP_ptr)->V0_z + PW_BOMB(pP_ptr)->Gxz;
    PW_BOMB(pP_ptr)->V0_z = Spd;
    if ((LONG)(Spd << 16) < 0) {
        PW_BOMB(pP_ptr)->V0_z = 0;
    }
    Sv.vx = PW_BOMB(pP_ptr)->V0_z;
    Sv.vz = 0;
    ApplyMatrixSV(&M, &Sv, &Sv1);
    pP_ptr->Workm.t[0] += Sv1.vx;
    pP_ptr->Workm.t[2] += Sv1.vz;
}

/* Joint_move for the 15-part packed kan format (J_seq2) */
ULONG Joint_move2(ENEMY_WORK *pEm, ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, ULONG Late_flg)
{
    USHORT *p;
    ULONG f_num;

    p = (USHORT *)((UCHAR *)Seq_t_ptr + pEm->Move_no * 8);
    f_num = *p++;
    if (Late_flg & 0xFFFF0000) {
        p = (USHORT *)((UCHAR *)Seq_t_ptr + *p) + (f_num - pEm->Move_cnt - 1);
    } else {
        p = (USHORT *)((UCHAR *)Seq_t_ptr + *p) + pEm->Move_cnt;
    }
    if (*p & 0x100) {
        if (!(*pEm->pNow_seq & 0x100)) {
            Hokan_key_set(Kan_t_ptr, Seq_t_ptr, pEm->pNow_seq, Late_flg >> 16);
        }
        pEm->pNow_seq = p;
        return J_seq2_hokan(Kan_t_ptr, Seq_t_ptr, *(UCHAR *)p, Late_flg);
    } else {
        pEm->pNow_seq = p;
        return J_seq2(Kan_t_ptr, Seq_t_ptr, p, (Late_flg & 0xFFFF) | (f_num << 16));
    }
}

/* decode the packed 12-bit rotations of 15 parts for the current frame and blend them in (RE2 J_seq2) */
ULONG J_seq2(ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, USHORT *pSeq, ULONG Late_flg)
{
    ENEMY_WORK *pEm;
    ULONG *pRot;
    PARTS_W *pP;
    LONG w;
    ULONG Hokan;
    ULONG n;
    ULONG Max;
    VECTOR V;
    MATRIX M;

    pEm = G.pEm;
    pRot = (ULONG *)((UCHAR *)Kan_t_ptr + ((KAN_HEAD *)Kan_t_ptr)->Bone_ofs);
    pRot = (ULONG *)((UCHAR *)pRot + (((KAN_HEAD *)Kan_t_ptr)->Rec_sz >> 18) * (((USHORT *)((UCHAR *)Seq_t_ptr + pEm->Move_no * 8))[2] + *(UCHAR *)pSeq) * 4);
    pP = pEm->pSin_parts_ptr;
    w = ((SHORT *)pRot)[3];
    w *= pEm->Mot_scale_y;
    pP->Pos_y = w >> 12;
    Hokan = pEm->Hokan_flg;
    n = *(USHORT *)((UCHAR *)Kan_t_ptr + 4);
    Max = Late_flg >> 16;
    if (Hokan != 0) {
        V.vx = pP->Old_x;
        V.vy = pP->Old_y;
        V.vz = pP->Old_z;
        w = (Late_flg & 0xFFFF) * Hokan;
        LoadAverage12(POS_V(pP), &V, 0x1000 - w, w, POS_V(pP));
    }
    pRot += 2;
    pP->Old_x = pP->Pos_x;
    pP->Old_y = pP->Pos_y;
    pP->Old_z = pP->Pos_z;
    *(ULONG *)&pP[0].Rot_x = (((USHORT *)pRot)[0] & 0xFFF) | ((pRot[0] & 0xFFF000) << 4);
    pP[0].Rot_z = ((UCHAR *)pRot)[3] | ((((USHORT *)pRot)[2] & 0xF) << 8);
    *(ULONG *)&pP[1].Rot_x = ((pRot[1] & 0xFFF0) >> 4) | (pRot[1] & 0xFFF0000);
    pP[1].Rot_z = (pRot[1] >> 28) | (((UCHAR *)pRot)[8] << 4);
    *(ULONG *)&pP[2].Rot_x = ((pRot[2] & 0xFFF00) >> 8) | ((pRot[2] & 0xFFF00000) >> 4);
    pP[2].Rot_z = ((USHORT *)pRot)[6] & 0xFFF;
    *(ULONG *)&pP[3].Rot_x = ((pRot[3] & 0xFFF000) >> 12) | (((pRot[3] & 0xFF000000) >> 8) | ((pRot[4] & 0xF) << 24));
    pP[3].Rot_z = (pRot[4] & 0xFFF0) >> 4;
    *(ULONG *)&pP[4].Rot_x = ((LONG)(pRot[4] & 0xFFF0000) >> 16) | (((pRot[4] & 0xF0000000) >> 12) | (((UCHAR *)pRot)[20] << 20));
    pP[4].Rot_z = (pRot[5] & 0xFFF00) >> 8;
    *(ULONG *)&pP[5].Rot_x = (pRot[5] >> 20) | ((pRot[6] & 0xFFF) << 16);
    pP[5].Rot_z = (pRot[6] & 0xFFF000) >> 12;
    *(ULONG *)&pP[6].Rot_x = ((UCHAR *)pRot)[27] | ((((USHORT *)pRot)[14] & 0xF) << 8) | ((pRot[7] & 0xFFF0) << 12);
    pP[6].Rot_z = (pRot[7] & 0xFFF0000) >> 16;
    *(ULONG *)&pP[7].Rot_x = (pRot[7] >> 28) | (((UCHAR *)pRot)[32] << 4) | ((pRot[8] & 0xFFF00) << 8);
    pP[7].Rot_z = pRot[8] >> 20;
    *(ULONG *)&pP[8].Rot_x = (((USHORT *)pRot)[18] & 0xFFF) | ((pRot[9] & 0xFFF000) << 4);
    pP[8].Rot_z = ((UCHAR *)pRot)[39] | ((((USHORT *)pRot)[20] & 0xF) << 8);
    *(ULONG *)&pP[9].Rot_x = ((pRot[10] & 0xFFF0) >> 4) | (pRot[10] & 0xFFF0000);
    pP[9].Rot_z = (pRot[10] >> 28) | (((UCHAR *)pRot)[44] << 4);
    *(ULONG *)&pP[10].Rot_x = ((pRot[11] & 0xFFF00) >> 8) | ((pRot[11] & 0xFFF00000) >> 4);
    pP[10].Rot_z = ((USHORT *)pRot)[24] & 0xFFF;
    *(ULONG *)&pP[11].Rot_x = ((pRot[12] & 0xFFF000) >> 12) | (((pRot[12] & 0xFF000000) >> 8) | ((pRot[13] & 0xF) << 24));
    pP[11].Rot_z = (pRot[13] & 0xFFF0) >> 4;
    *(ULONG *)&pP[12].Rot_x = ((LONG)(pRot[13] & 0xFFF0000) >> 16) | (((pRot[13] & 0xF0000000) >> 12) | (((UCHAR *)pRot)[56] << 20));
    pP[12].Rot_z = (pRot[14] & 0xFFF00) >> 8;
    *(ULONG *)&pP[13].Rot_x = (pRot[14] >> 20) | ((pRot[15] & 0xFFF) << 16);
    pP[13].Rot_z = (pRot[15] & 0xFFF000) >> 12;
    *(ULONG *)&pP[14].Rot_x = ((UCHAR *)pRot)[63] | ((((USHORT *)pRot)[32] & 0xF) << 8) | ((pRot[16] & 0xFFF0) << 12);
    pP[14].Rot_z = (pRot[16] & 0xFFF0000) >> 16;
    if (Hokan == 0) {
    copy:
        *(ULONG *)&pP->Cdir_x = *(ULONG *)&pP->Rot_x;
        pP->Cdir_z = pP->Rot_z;
        RotMatrix_gte(CDIR_SV(pP), PW_M(pP));
        pP++;
        if (--n != 0) {
            goto copy;
        }
    } else {
        pEm->Hokan_flg--;
    hokan:
        if (n >= 7) {
            hokan_svec(PW_ROT_SV(pP), CDIR_SV(pP), CDIR_SV(pP), w);
            RotMatrix_gte(CDIR_SV(pP), PW_M(pP));
        } else {
            RotMatrix_gte(PW_ROT_SV(pP), &M);
            HMatrix(PW_M(pP), &M, PW_M(pP), 0xFFF - w);
        }
        pP++;
        n--;
        if (n != 0) {
            goto hokan;
        }
    }
    if (++pEm->Move_cnt >= Max) {
        pEm->Move_cnt = 0;
        return 1;
    }
    return 0;
}

/* interpolated frame of Joint_move2 */
ULONG J_seq2_hokan(ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, LONG No, ULONG Late_flg)
{
    ENEMY_WORK *pEm;
    PARTS_W *pP;
    PARTS_W *pP_walk;
    ULONG Max;
    ULONG n;
    LONG w;
    VECTOR V;

    pEm = G.pEm;
    Max = *(USHORT *)((UCHAR *)Seq_t_ptr + pEm->Move_no * 8);
    if (Late_flg & 0xFFFF0000) {
        No = ~No;
        No &= 0xFF;
    }
    pP = pEm->pSin_parts_ptr;
    pP->Pos_y = pP->Pos_base_y + ((pP->Pos_step_y * No) >> 8);
    n = 15;
    pP_walk = pP;
    if (pEm->Hokan_flg == 0) {
        do {
            pP_walk->Cdir_x = pP_walk->Rot_x + ((pP_walk->Rot_spd_x * No) >> 8);
            pP_walk->Cdir_y = pP_walk->Rot_y + ((pP_walk->Rot_spd_y * No) >> 8);
            pP_walk->Cdir_z = pP_walk->Rot_z + ((pP_walk->Rot_spd_z * No) >> 8);
            RotMatrix_gte(CDIR_SV(pP_walk), PW_M(pP_walk));
            pP_walk++;
        } while (--n);
    } else {
        Late_flg &= 0xFFFF;
        w = Late_flg * pEm->Hokan_flg;
        V.vx = pP->Old_x;
        V.vy = pP->Old_y;
        V.vz = pP->Old_z;
        gte_lddp(0x1000 - w);
        gte_ldlvl(&pP->Pos_x);
        gte_gpf12();
        gte_lddp(w);
        gte_ldlvl(&V);
        gte_gpl12();
        gte_stlvl(&pP->Pos_x);
        pEm->Hokan_flg--;
        Parts_hokan2(15, pP, No, Late_flg);
    }
    pP->Old_x = pP->Pos_x;
    pP->Old_y = pP->Pos_y;
    pP->Old_z = pP->Pos_z;
    if (++pEm->Move_cnt < Max) {
        return 0;
    }
    pEm->Move_cnt = 0;
    return 1;
}

/* blend 15 parts from their rotation to the interpolated key rotation */
void Parts_hokan2(ULONG n, PARTS_W *pP, ULONG Late, SHORT Rate)
{
    SVECTOR Sv;
    MATRIX M;
    SVECTOR *pSv;
    SHORT Hokan;

    pSv = &Sv;
    Hokan = 0x1000 - G.pEm->Hokan_flg * Rate;
    do {
        pSv->vx = pP->Rot_x + ((ULONG)(pP->Rot_spd_x * Late) >> 8);
        pSv->vy = pP->Rot_y + ((ULONG)(pP->Rot_spd_y * Late) >> 8);
        pSv->vz = pP->Rot_z + ((ULONG)(pP->Rot_spd_z * Late) >> 8);
        if (n >= 7) {
            hokan_svec(CDIR_SV(pP), pSv, CDIR_SV(pP), Hokan);
            RotMatrix_gte(CDIR_SV(pP), PW_M(pP));
        } else {
            RotMatrix_gte(pSv, &M);
            HMatrix(PW_M(pP), &M, PW_M(pP), Hokan);
        }
    } while (--n != 0);
}

/* Joint_move for a JOINT_P piece: only the parts in On_parts_flg (RE2 Joint_move_p) */
ULONG Joint_move_p(JOINT_P *pJp, ULONG *Kan_t_ptr, ULONG *Seq_t_ptr, ULONG Late_flg)
{
    ULONG *pRot;
    USHORT *pSeq;
    ULONG f_base;
    PARTS_W *pP;
    PARTS_W *pP_walk;
    ULONG Hokan;
    ULONG P0_no;
    ULONG On_flg;
    ULONG Max;
    ULONG n;
    ULONG Cnt;
    LONG w;
    LONG dx;
    LONG dy;
    LONG dz;
    SVECTOR Unused[2];                 /* frame space only */

    pSeq = (USHORT *)((UCHAR *)Seq_t_ptr + pJp->Move_no * 8);
    pSeq = (USHORT *)((UCHAR *)Seq_t_ptr + pSeq[1]) + pJp->Move_cnt;
    f_base = ((USHORT *)((UCHAR *)Seq_t_ptr + pJp->Move_no * 8))[2];
    pRot = (ULONG *)((UCHAR *)Kan_t_ptr + ((KAN_HEAD *)Kan_t_ptr)->Bone_ofs);
    pRot += (((KAN_HEAD *)Kan_t_ptr)->Rec_sz >> 18) * (f_base + *(UCHAR *)pSeq);
    w = ((SHORT *)pRot)[3];
    w *= G.pEm->Mot_scale_y;
    Hokan = pJp->Hokan_flg;
    pRot += 2;
    P0_no = pJp->P0_parts_no;
    pP = G.pEm->pSin_parts_ptr;
    On_flg = pJp->On_parts_flg;
    Max = *(USHORT *)((UCHAR *)Seq_t_ptr + pJp->Move_no * 8);
    pP_walk = pP;
    pJp->P0_m.t[1] = w >> 12;
    Cnt = *(USHORT *)((UCHAR *)Kan_t_ptr + 4);
    w = Late_flg & 0xFFFF;
    n = Cnt;
unpack:
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[0].Rot_x = (((USHORT *)pRot)[0] & 0xFFF) | ((pRot[0] & 0xFFF000) << 4);
        pP_walk[0].Rot_z = ((UCHAR *)pRot)[3] | ((((USHORT *)pRot)[2] & 0xF) << 8);
    }
    if (P0_no == 0) {
        P0_no = -1;
        *(ULONG *)&pJp->P0_sv.vx = (((USHORT *)pRot)[0] & 0xFFF) | ((pRot[0] & 0xFFF000) << 4);
        pJp->P0_sv.vz = ((UCHAR *)pRot)[3] | ((((USHORT *)pRot)[2] & 0xF) << 8);
        RotMatrix(&pJp->P0_sv, &pJp->P0_m);
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[1].Rot_x = ((pRot[1] & 0xFFF0) >> 4) | (pRot[1] & 0xFFF0000);
        pP_walk[1].Rot_z = (pRot[1] >> 28) | (((UCHAR *)pRot)[8] << 4);
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[2].Rot_x = ((pRot[2] & 0xFFF00) >> 8) | ((pRot[2] & 0xFFF00000) >> 4);
        pP_walk[2].Rot_z = ((USHORT *)pRot)[6] & 0xFFF;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[3].Rot_x = ((pRot[3] & 0xFFF000) >> 12) | (((pRot[3] & 0xFF000000) >> 8) | ((pRot[4] & 0xF) << 24));
        pP_walk[3].Rot_z = (pRot[4] & 0xFFF0) >> 4;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[4].Rot_x = ((LONG)(pRot[4] & 0xFFF0000) >> 16) | (((pRot[4] & 0xF0000000) >> 12) | (((UCHAR *)pRot)[20] << 20));
        pP_walk[4].Rot_z = (pRot[5] & 0xFFF00) >> 8;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[5].Rot_x = (pRot[5] >> 20) | ((pRot[6] & 0xFFF) << 16);
        pP_walk[5].Rot_z = (pRot[6] & 0xFFF000) >> 12;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[6].Rot_x = ((UCHAR *)pRot)[27] | ((((USHORT *)pRot)[14] & 0xF) << 8) | ((pRot[7] & 0xFFF0) << 12);
        pP_walk[6].Rot_z = (pRot[7] & 0xFFF0000) >> 16;
    }
    On_flg >>= 1;
    if (--n == 0) goto unpacked;
    if (On_flg & 1) {
        *(ULONG *)&pP_walk[7].Rot_x = (pRot[7] >> 28) | (((UCHAR *)pRot)[32] << 4) | ((pRot[8] & 0xFFF00) << 8);
        pP_walk[7].Rot_z = pRot[8] >> 20;
    }
    On_flg >>= 1;
    pRot += 9;
    pP_walk += 8;
    if (--n != 0) {
        goto unpack;
    }
unpacked:
    On_flg = pJp->On_parts_flg;
    if (Hokan == 0) {
        do {
            if (On_flg & 1) {
                *(ULONG *)&pP->Cdir_x = *(ULONG *)&pP->Rot_x;
                pP->Cdir_z = pP->Rot_z;
                RotMatrix_gte(PW_ROT_SV(pP), PW_M(pP));
            }
            On_flg >>= 1;
            pP++;
        } while (--Cnt != 0);
    } else {
        do {
            if (On_flg & 1) {
                *(ULONG *)&pP->Cdir_x &= 0xFFF0FFF;
                pP->Cdir_z &= 0xFFF;
                gte_lddp(w);
                gte_ldsv(&pP->Cdir_x);
                dx = pP->Rot_x - pP->Cdir_x + 0x800;
                dy = pP->Rot_y - pP->Cdir_y + 0x800;
                dz = pP->Rot_z - pP->Cdir_z + 0x800;
                if ((ULONG)dx > 0x1000) {
                    pP->Rot_x += (dx & 0x80000000) ? 0x1000 : -0x1000;
                }
                if ((ULONG)dy > 0x1000) {
                    pP->Rot_y += (dy & 0x80000000) ? 0x1000 : -0x1000;
                }
                if ((ULONG)dz > 0x1000) {
                    pP->Rot_z += (dz & 0x80000000) ? 0x1000 : -0x1000;
                }
                gte_gpf12_b();
                gte_lddp(0x1000 - w);
                gte_ldsv(&pP->Rot_x);
                gte_gpl12();
                gte_stsv(&pP->Cdir_x);
                RotMatrix_gte(CDIR_SV(pP), PW_M(pP));
            }
            On_flg >>= 1;
            pP++;
            pJp->Hokan_flg--;
        } while (--Cnt != 0);
    }
    if (++pJp->Move_cnt >= Max) {
        pJp->Move_cnt = 0;
        return 1;
    }
    return 0;
}

/* light, screen matrix and sort of an object model part (om.c) */
void Om_joint_trans(ENEMY_WORK *pEm, PARTS_W *pP, ULONG Flg, MATRIX *pM)
{
    MATRIX M;
    VECTOR Scale;
    tagGLOBAL *pG;
    ULONG Be;

    pG = &G;
    Be = pP->Be_flg;
    if (!(Flg & 0x200)) {
        gte_MulMatrix0(&LIGHTMATRIX, pM, &M);
        gte_SetLightMatrix(&M);
    }
    CompM(&GsWSMATRIX, pM, &M);
    if (Be & 4) {
        Scale.vx = pP->Size_x;
        Scale.vy = pP->Size_y;
        Scale.vz = pP->Size_z;
        ScaleMatrix(&M, &Scale);
    }
    gte_SetRotMatrix(&M);
    gte_SetTransMatrix(&M);
    if (Flg & 0x20) {
        pG->pPacket = Bomb_Parts_SortGT3(pP->pTmd, pG->pPacket, pP);
        pG->pPacket = Bomb_Parts_SortGT4(pP->pTmd, pG->pPacket, pP);
        Parts_bomb(pP);
    } else if (Flg & 2) {
        Parts_sort_simple(&pP->Attribute, Flg, pP);
    }
}
