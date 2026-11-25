#include "esp.h"
#include "em_sub.h"
#include "cd.h"
#include "sub.h"
#include "joint.h"
#include "snd.h"
#include "pl_wep.h"
#include "sca.h"
#include "sce_at.h"
#include "sld.h"
#include "spl_sce.h"

/* local declarations (80011CF8.c) */
/* Add_speedXZ is called with a third (unused) argument in RE3 */
#define ADD_SPEED_XZ3(pEm) ((void (*)(ENEMY_WORK *, LONG, LONG))Add_speedXZ)((pEm), 0, 0)
/* PARTS_W Free[] work (RE2 pOwn_p->Free), 0x74.. in RE3 */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

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
    if ((pEm->Col.Hit_flg & 0x40) && !(pEm->Be_flg & BE_FLG_400)) {
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
    CompM(pEm->pSuper, &M, &Tm);
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

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Goto00);

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

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Goto01);

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

ULONG Sikai_ck(ENEMY_WORK *pEm, VECTOR *pTa, LONG Han, VECTOR *pAi)
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

void Set_base_pos(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq_t, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;
    ENEMY_WORK *pPm;

    pPm = G.pEm;
    get_null_pos(&sv, R_flg, pKan, pSeq_t);
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

void Null_pos_set(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq_t, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;

    get_null_pos(&sv, R_flg, pKan, pSeq_t);
    m = GsIDMATRIX;
    RotMatrixY(pEm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pEm->Pos_x = pEm->Base_pos_x + sv.vx;
    pEm->Pos_y = pEm->Base_pos_y + sv.vy;
    pEm->Pos_z = pEm->Base_pos_z + sv.vz;
}

void get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG *pKan, ULONG *pSeq_t)
{
    ENEMY_WORK *pEm;
    USHORT *pSeq;
    ULONG f_num;
    ULONG f_base;
    ULONG Rec_sz;
    UCHAR *Bone_adr;

    pEm = G.pEm;
    pSeq = (USHORT *)((UCHAR *)pSeq_t + pEm->Move_no * 8);
    f_num = pSeq[0];
    f_base = pSeq[2];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + pEm->Move_cnt;
    }
    Bone_adr = (UCHAR *)pKan + ((KAN_HEAD *)pKan)->Bone_ofs;
    Rec_sz = ((KAN_HEAD *)pKan)->Rec_sz >> 18;
    while (*pSeq & 0x100) {
        pSeq++;
    }
    Bone_adr += Rec_sz * (f_base + *(UCHAR *)pSeq) * 4;
    pSv->vx = ((SVECTOR *)Bone_adr)->vx;
    pSv->vy = ((SVECTOR *)Bone_adr)->vy;
    pSv->vz = ((SVECTOR *)Bone_adr)->vz;
}

void Null_pos_set2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq_t, ULONG R_flg)
{
    MATRIX m;
    SVECTOR sv;

    get_null_pos2(&sv, R_flg, pKan, pSeq_t);
    m = GsIDMATRIX;
    RotMatrixY(pEm->Cdir_y, &m);
    ApplyMatrixSV(&m, &sv, &sv);
    pEm->Pos_x = pEm->Base_pos_x + sv.vx;
    pEm->Pos_y = pEm->Base_pos_y + sv.vy;
    pEm->Pos_z = pEm->Base_pos_z + sv.vz;
}

void get_null_pos2(SVECTOR *pSv, ULONG R_flg, ULONG *pKan, ULONG *pSeq_t)
{
    ENEMY_WORK *pEm;
    USHORT *pSeq;
    ULONG f_num;
    ULONG f_base;
    ULONG w;
    ULONG r;
    USHORT rr;
    PARTS_W *pP;
    UCHAR *Bone_adr;

    pEm = G.pEm;
    pSeq = (USHORT *)((UCHAR *)pSeq_t + pEm->Move_no * 8);
    f_num = pSeq[0];
    f_base = pSeq[2];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + pEm->Move_cnt;
    }
    w = *pSeq;
    if (w & 0x100) {
        r = w & 0xFF;
        rr = r;
        if (R_flg != 0) {
            if (!(pSeq[1] & 0x100)) {
                func_8002B2BC(pKan, pSeq_t, pSeq + 1, R_flg);
            }
            rr = 0x100 - r;
        } else {
            if (!(pSeq[-1] & 0x100)) {
                func_8002B2BC(pKan, pSeq_t, pSeq - 1, 0);
            }
        }
        pP = pEm->pSin_parts_ptr;
        pSv->vx = (USHORT)pP->UnkAC + ((pP->UnkB4 * rr) >> 8);
        pSv->vy = 0;
        pSv->vz = (USHORT)pP->UnkB0 + ((pP->UnkB2 * rr) >> 8);
    } else {
        Bone_adr = (UCHAR *)pKan + ((KAN_HEAD *)pKan)->Bone_ofs + (((KAN_HEAD *)pKan)->Rec_sz >> 18) * (f_base + (w & 0xFF)) * 4;
        pSv->vx = ((SVECTOR *)Bone_adr)->vx;
        pSv->vy = 0;
        pSv->vz = ((SVECTOR *)Bone_adr)->vz;
    }
}

/* RE2 Spd_get: Spd_x/y/z = root (bone 0) offset of this animation frame minus the last frame's (Spd_base);
 * Flg & 1 plays the sequence backwards, Flg < 0 just reads the offset */
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq_t, LONG Flg)
{
    USHORT *pSeq;
    ULONG f_num;
    SHORT f_base;
    ULONG Rec_sz;
    UCHAR *pTop;
    UCHAR *Bone_adr;
    SHORT x;
    SHORT y;
    SHORT z;
    ULONG Raw_y;

    pSeq = (USHORT *)((UCHAR *)pSeq_t + pEm->Move_no * 8);
    pTop = (UCHAR *)pSeq_t;
    f_base = pSeq[2];
    f_num = pSeq[0];
    pSeq++;
    if (Flg & 1) {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + pEm->Move_cnt;
    }
    Rec_sz = ((KAN_HEAD *)pKan)->Rec_sz >> 18;
    Bone_adr = (UCHAR *)pKan + ((KAN_HEAD *)pKan)->Bone_ofs;
    Bone_adr += Rec_sz * (f_base + *(UCHAR *)pSeq) * 4;
    x = ((SVECTOR *)Bone_adr)->vx;
    z = ((SVECTOR *)Bone_adr)->vz;
    Raw_y = (USHORT)((SVECTOR *)Bone_adr)->vy;
    y = Raw_y;
    if (pEm->Move_cnt == 0 && Flg >= 0) {
        /* first frame: the base is 0, or (backwards) the offsets of the last and the first frame */
        *(ULONG *)&pEm->Spd_base_x = 0;
        pEm->Spd_base_z = 0;
        if (Flg & 1) {
            pEm->Spd_base_x = x;
            pEm->Spd_base_y = y;
            pEm->Spd_base_z = z;
            pSeq = (USHORT *)(pTop + pEm->Move_no * 8);
            pSeq = (USHORT *)((UCHAR *)pSeq_t + pSeq[1]);
            Bone_adr = (UCHAR *)pKan + ((KAN_HEAD *)pKan)->Bone_ofs;
            Bone_adr += Rec_sz * (f_base + *(UCHAR *)pSeq) * 4;
            pEm->Spd_base_x += ((SVECTOR *)Bone_adr)->vx;
            pEm->Spd_base_y += ((SVECTOR *)Bone_adr)->vy;
            pEm->Spd_base_z += ((SVECTOR *)Bone_adr)->vz;
        }
    }
    pEm->Spd_x = x;
    pEm->Spd_y = y;
    pEm->Spd_z = z;
    if (Flg >= 0) {
        pEm->Spd_x -= pEm->Spd_base_x;
        pEm->Spd_y -= pEm->Spd_base_y;
        pEm->Spd_z -= pEm->Spd_base_z;
        /* Spd_base_x/y in one word store (x is not masked) */
        *(ULONG *)&pEm->Spd_base_x = x | (y << 16);
        pEm->Spd_base_z = z;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", func_80018EC8);

INCLUDE_ASM("asm/nonmatchings/game/em_sub", func_80019178);

void Em_seq_sound(ENEMY_WORK *pEm)
{
    if (*pEm->pNow_seq & 0xF000) {
        Snd_se_enem((*pEm->pNow_seq >> 12) - 1, pEm);
    }
}

void Foot_set_pl(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no)
{
    PARTS_W *pP_ptr;
    MATRIX *pM;
    MATRIX Mat;
    SVECTOR Rot;
    LONG x, z;

    if (pEm->Status_flg & 0x80) {
        return;
    }
    pM = (MATRIX *)pEm->Workm_m;
    pP_ptr = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, pM);
    if (pEm->Be_flg & 0xC0000) {
        Rot.vy = 0;
        Rot.vx = pEm->Unk1BC[0];
        Rot.vz = pEm->Unk1BC[1];
        RotMatrix_gte(&Rot, &Mat);
        MulMatrix2(&Mat, pM);
    }
    CompM(pM, (MATRIX *)pP_ptr->M, &Mat);
    CompM(&Mat, (MATRIX *)pP_ptr[8].M, &Mat);
    pP_ptr += Foot_no * 3 + 11;
    CompM(&Mat, (MATRIX *)pP_ptr[-2].M, &Mat);
    CompM(&Mat, (MATRIX *)pP_ptr[-1].M, &Mat);
    CompM(&Mat, (MATRIX *)pP_ptr->M, &Mat);
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
        Memcpy(&pK->Prim[1], p, 0x28);
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

void Kage_work_set(KAGE_WORK **ppK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos)
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
        *ppK = NULL;
    } else {
        *ppK = pKw;
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
void Kage_work9_set(KAGE_WORK9 **ppK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos)
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
        *ppK = NULL;
        return;
    }
    *ppK = pK9;
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

void MulKage(KAGE_WORK *pKw, SHORT Grand, SHORT Dir_y, ULONG Flg)
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

/* GTE inline macros (PsyQ inline_c.h with the opcodes written out) */
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_rtpt_b() __asm__ volatile(".word 0x4A280030")
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_avsz4() __asm__ volatile("nop; nop; .word 0x4B68002E")
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile("swc2 $12, 0(%0); swc2 $13, 0(%1); swc2 $14, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")

void Kage_work_sort(void)
{
    KAGE_WORK *pKw;
    POLY_FT4 *pTag;
    ULONG *pOt;
    ULONG *Ot_base;
    LONG Unit;
    LONG i;
    LONG Hx;
    VCUT *pCut;
    MATRIX Mx;
    MATRIX M2;
    SVECTOR Sv[4];
    VECTOR Pos;
    ULONG Otz;
    SHORT *pXy0;
    SHORT *pXy1;

    i = 0x32;
    pKw = G.Kage;
    pCut = G.Cccut;
    Unit = 0x1000;
    do {
        if (pKw->Be_flg & 4) if (!(pKw->Be_flg & 8)) {
            Ot_base = func_80037D50(pKw->pPos);
            *(LONG *)&Mx.m[0][0] = Unit;
            *(LONG *)&Mx.m[0][2] = 0;
            *(LONG *)&Mx.m[1][1] = Unit;
            *(LONG *)&Mx.m[2][0] = 0;
            *(LONG *)&Mx.m[2][2] = Unit;
            RotMatrixY(pKw->Dir_y, &Mx);
            Mx.t[0] = pKw->Pos_x;
            Mx.t[1] = pKw->Pos_y;
            Mx.t[2] = pKw->Pos_z;
            CompM(&GsWSMATRIX, &Mx, &M2);
            SetRotMatrix(&M2);
            SetTransMatrix(&M2);
            Hx = pKw->Half_x;
            Sv[0].vz = pKw->Half_z;
            Sv[1].vz = pKw->Half_z;
            *(LONG *)&Sv[0] = ~Hx - 1;
            *(LONG *)&Sv[1] = Hx;
            *(LONG *)&Sv[2] = ~Hx - 1;
            *(LONG *)&Sv[3] = Hx;
            Sv[3].vz = Sv[2].vz = ~pKw->Half_z - 1;
            Pos = *(VECTOR *)Mx.t;
            pTag = &pKw->Prim[G.Cbuf_id];
            if (Hit_ck_point4((LONG *)&Pos, pCut) != 0) {
                gte_ldv3(&Sv[0], &Sv[1], &Sv[2]);
                pXy0 = &pTag->x0;
                pXy1 = &pTag->x1;
                gte_rtpt_b();
                gte_nclip();
                gte_stopz(&Otz);
                gte_stsxy3(pXy0, pXy1, &pTag->x2);
                gte_ldv0(&Sv[3]);
                gte_rtps();
                pXy0 = &pTag->x3;
                gte_stsxy(pXy0);
                gte_avsz4();
                gte_stotz(&Otz);
                Otz += 0x20;
                if (Otz > 0x1FFF) {
                    Otz = 0x1FFF;
                }
                if (pKw->Be_flg & 0x40) {
                    Otz = 0x1FFF;
                    Ot_base = G.pOt_sub[G.Cbuf_id][2];
                }
                pOt = (ULONG *)((Otz >> 3) * 4 + (ULONG)Ot_base); /* integer add: operand order */
                addPrim(pOt, pTag);
            }
        }
        /* the nested do/while (0) raises the giv's allocation weight (s0 for &pKw->Be_flg) */
        do { do { do { pKw++; } while (0); } while (0); } while (0);
        i--;
    } while (i != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Kage_work9_sort);

void Kage_work_color_set(KAGE_WORK *pK, ULONG Color)
{
    *(ULONG *)&pK->Prim[0].r0 = (*(ULONG *)&pK->Prim[0].r0 & 0xFF000000) | Color;
    *(ULONG *)&pK->Prim[1].r0 = (*(ULONG *)&pK->Prim[1].r0 & 0xFF000000) | Color;
}

void Kage_work9_color_set(KAGE_WORK9 *pK, ULONG Color)
{
    KAGE_WORK *pPrim;
    UCHAR cnt;

    cnt = 9;
    pPrim = pK->pKw_top;
    do {
        cnt--;
        *(ULONG *)&pPrim->Prim[0].r0 = (*(ULONG *)&pPrim->Prim[0].r0 & 0xFF000000) | Color;
        *(ULONG *)&pPrim->Prim[1].r0 = (*(ULONG *)&pPrim->Prim[1].r0 & 0xFF000000) | Color;
        pPrim++;
    } while (cnt != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Rot_neck);

void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y)
{
    ULONG Own_neck_no;
    ULONG Enemy_neck_no;
    USHORT Body_dir_y;
    PARTS_W *pEn_parts;
    PARTS_W *pOwn_parts;
    PARTS_W *pEn_p;
    PARTS_W *pOwn_p;
    SHORT Vel;
    ULONG Ang_dif;
    SHORT Free_dif;
    ULONG Free_dif12;
    ULONG Spd_lim;
    ENEMY_WORK *pNeck_enemy;
    SHORT Dir_sum;
    SVECTOR Neck_rot;
    VECTOR Neck_tgt;

    if (!(pPm->Unk120 & 1)) {
        Own_neck_no = pPm->Neck_no;
        pOwn_parts = pPm->pSin_parts_ptr;
        pOwn_p = &pOwn_parts[Own_neck_no];
        pNeck_enemy = pPm->pEnemy_ptr;
        Enemy_neck_no = pNeck_enemy->Neck_no;
        Body_dir_y = pOwn_parts->Cdir_y;
        pEn_parts = pNeck_enemy->pSin_parts_ptr;
        pEn_p = &pEn_parts[Enemy_neck_no];
        Neck_tgt = *(VECTOR *)pEn_p->Workm.t;
        Dir_sum = Cdir_y + Body_dir_y;
        Neck_Dir_ZY((VECTOR *)pOwn_p->Workm.t, &Neck_tgt, &Neck_rot, Dir_sum);
        if (pPm->Unk120 & 2) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
        }
        if (pPm == pNeck_enemy) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
            Neck_rot.vz = 0;
        }
        Spd_lim = (USHORT)PW_FREE(pOwn_p)[2];
        Vel = PW_FREE(pOwn_p)[4];
        if (pPm->Unk120 & 0x80) {
            Spd_lim = (UCHAR)PW_FREE(pOwn_p)[6];
        }
        Ang_dif = (Neck_rot.vy - Cdir_y) - (SHORT)Body_dir_y;
        if ((LONG)((Ang_dif + Vel) & 0xFFF) > Vel * 2) {
            if (!(Ang_dif & 0x800)) {
                Neck_rot.vy = Vel + Dir_sum;
            } else {
                Neck_rot.vy = Dir_sum - Vel;
            }
        }
        Free_dif = Spd_lim + ((USHORT)Neck_rot.vy - ((USHORT)PW_FREE(pOwn_p)[0] + (Cdir_y + (USHORT)pOwn_p->Cdir_y + Body_dir_y)));
        Vel = PW_FREE(pOwn_p)[0] - Spd_lim;
        Free_dif12 = Free_dif & 0xFFF;
        PW_FREE(pOwn_p)[0] = Vel;
        if (!(Free_dif & 0x800)) {
            PW_FREE(pOwn_p)[0] = Vel + Spd_lim * 2;
        }
        if ((LONG)Free_dif12 < (LONG)(Spd_lim << 16) >> 15) {
            PW_FREE(pOwn_p)[0] = Neck_rot.vy - (Cdir_y + pOwn_p->Cdir_y + Body_dir_y);
        }
        pOwn_p->Cdir_y += PW_FREE(pOwn_p)[0];
        RotMatrix_gte((SVECTOR *)&pOwn_p->Cdir_x, (MATRIX *)pOwn_p->M);
        pOwn_p->Cdir_y -= PW_FREE(pOwn_p)[0];
    }
}

void Rot_neck_em_n(ENEMY_WORK *pPm, SHORT Cdir_y, SHORT *pDir)
{
    ULONG Own_neck_no;
    ULONG Enemy_neck_no;
    USHORT Body_dir_y;
    PARTS_W *pEn_parts;
    PARTS_W *pOwn_parts;
    PARTS_W *pEn_p;
    PARTS_W *pOwn_p;
    SHORT Vel;
    ULONG Ang_dif;
    SHORT Free_dif;
    ULONG Free_dif12;
    ULONG Spd_lim;
    ENEMY_WORK *pNeck_enemy;
    SHORT Dir_sum;
    SVECTOR Neck_rot;
    VECTOR Neck_tgt;

    if (!(pPm->Unk120 & 1)) {
        Own_neck_no = pPm->Neck_no;
        pOwn_parts = pPm->pSin_parts_ptr;
        pOwn_p = &pOwn_parts[Own_neck_no];
        pNeck_enemy = pPm->pEnemy_ptr;
        Enemy_neck_no = pNeck_enemy->Neck_no;
        Body_dir_y = pOwn_parts->Cdir_y;
        pEn_parts = pNeck_enemy->pSin_parts_ptr;
        pEn_p = &pEn_parts[Enemy_neck_no];
        Neck_tgt = *(VECTOR *)pEn_p->Workm.t;
        Dir_sum = Cdir_y + Body_dir_y;
        Neck_Dir_ZY((VECTOR *)pOwn_p->Workm.t, &Neck_tgt, &Neck_rot, Dir_sum);
        if (pPm->Unk120 & 2) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
        }
        if (pPm == pNeck_enemy) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
            Neck_rot.vz = 0;
        }
        Spd_lim = (USHORT)PW_FREE(pOwn_p)[2];
        Vel = PW_FREE(pOwn_p)[4];
        if (pPm->Unk120 & 0x80) {
            Spd_lim = (UCHAR)PW_FREE(pOwn_p)[6];
        }
        Ang_dif = (Neck_rot.vy - Cdir_y) - (SHORT)Body_dir_y;
        if ((LONG)((Ang_dif + Vel) & 0xFFF) > Vel * 2) {
            if (!(Ang_dif & 0x800)) {
                Neck_rot.vy = Vel + Dir_sum;
            } else {
                Neck_rot.vy = Dir_sum - Vel;
            }
        }
        Free_dif = Spd_lim + ((USHORT)Neck_rot.vy - ((USHORT)PW_FREE(pOwn_p)[0] + (Cdir_y + (USHORT)pOwn_p->Cdir_y + Body_dir_y)));
        Vel = PW_FREE(pOwn_p)[0] - Spd_lim;
        Free_dif12 = Free_dif & 0xFFF;
        PW_FREE(pOwn_p)[0] = Vel;
        if (!(Free_dif & 0x800)) {
            PW_FREE(pOwn_p)[0] = Vel + Spd_lim * 2;
        }
        if ((LONG)Free_dif12 < (LONG)(Spd_lim << 16) >> 15) {
            PW_FREE(pOwn_p)[0] = Neck_rot.vy - (Cdir_y + pOwn_p->Cdir_y + Body_dir_y);
        }
        *pDir = PW_FREE(pOwn_p)[0];
    }
}

void Neck_Dir_ZY(VECTOR *pA, VECTOR *pB, SVECTOR *pRot, SHORT Front_dir_y)
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

void func_8001AD78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq_t, ULONG R_flg)
{
    USHORT *pSeq;
    ULONG f_num;

    pSeq = (USHORT *)((UCHAR *)pSeq_t + pEm->Move_no * 8);
    f_num = pSeq[0];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + pEm->Move_cnt;
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

void Em_dead_flg_on_cand(ENEMY_WORK *pEm)
{
    if (G.System_flg & 0x80) {
        if (Flg_ck(G.Unk7890, pEm->Unk147) == 0) {
            func_8004834C(pEm->Id, 0);
        }
        Flg_on(G.Unk7890, pEm->Unk147);
    } else if (G.Unk783E >= 2) {
        Flg_on(G.Unk78B0, pEm->Unk147);
    } else {
        Flg_on(G.Unk7890, pEm->Unk147);
    }
}

void func_8001AECC(ENEMY_WORK *pEm, ULONG a1, ULONG a2)
{
    PARTS_W *pParts = pEm->pSin_parts_ptr;
    USHORT y = *(USHORT *)(pEm->pUnk13C + 0x18);

    pEm->Unk136 = (USHORT)pParts[a1 & 0xFFFF].Workm.t[1] - y + (a1 >> 16);
    pEm->Unk134 = (USHORT)pParts[a2 & 0xFFFF].Workm.t[1] - y + (a2 >> 16);
}

ENEMY_WORK *Em_work_add(ENEMY_WORK *pSrc)
{
    tagGLOBAL *pG = &G;
    ULONG i = 0;

    while (&pG->pEnemy[i] != pG->ppEnemy_end) {
        i++;
    }
    pG->Em_num++;
    pG->pEnemy[i] = pG->pEm_mem;
    pG->ppEnemy_end = &pG->pEnemy[i + 1];
    pG->pEm_mem++;
    *pG->pEnemy[i] = *pSrc;
    pG->pEm_mem = (ENEMY_WORK *)func_800261AC(pG->pEnemy[i], PartsWork_set(pG->pEnemy[i], (ULONG)pG->pEm_mem),
                                              pG->pEnemy[i]->pUnk10C);
    return pG->pEnemy[i];
}

ENEMY_WORK *func_8001B048(ULONG Id)
{
    ENEMY_WORK **ppEm;
    ENEMY_WORK *pRet = NULL;
    tagGLOBAL *pG = &G;

    ppEm = &pG->pEnemy[2];
    do {
        if ((*ppEm)->Id == Id) {
            pRet = *ppEm;
        }
        ppEm++;
    } while (ppEm != pG->ppEnemy_end);
    return pRet;
}

void Esp_init_C(void)
{
    ULONG i;
    ULONG *pEnd;
    TBL12 *pT;

    for (i = 0x60; i != 0; ) {
        i--;
        Esp[i].Be_flg = 0;
    }
    pT = &D_800BA728[32];
    do {
        pT--;
        pT->Id = 0xFF;
        pT->pData = NULL;
        pT->pData2 = NULL;
    } while (pT > D_800BA728);
    i = Cd_read(0x14, (ULONG *)0x801FC200, 0, "CORE00.ESP");
    pEnd = (ULONG *)0x801FC200;
    pEnd += i >> 2;
    if (i & 3) {
        pEnd++;
    }
    Esp_data_set0((UCHAR *)0x801FC200, pEnd - 1, 0);
}

LONG Esp_init_R(void)
{
    TBL12 *pT;
    LONG i;
    LONG *pData;

    i = 0x60;
    do {
        i--;
        Esp[i].Be_flg = 0;
    } while (i != 0);
    pT = (TBL12 *)Esp;
    do {
        pT--;
        pT->Id = 0xFF;
        pT->pData = NULL;
        pT->pData2 = NULL;
    } while (pT > (TBL12 *)Esp - 16);
    pData = G.pRoom->pUnk4C;
    if (pData != NULL && *pData != -1) {
        Esp_data_set0((UCHAR *)pData, G.pRoom->pUnk50, 1);
        Esp_data_set1(G.pRoom->pUnk54);
    }
}

ULONG Esp_data_set0(UCHAR *pData, ULONG *pOfs, ULONG Mode)
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

void Esp_data_set1(ULONG *pTim)
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

ESP_WORK *func_8001B35C(ESP_WORK *pParent, ULONG a1, ULONG a2, MATRIX *pM, LONG *pPos, SVECTOR *pSpd)
{
    SVECTOR sv;
    ESP_WORK *pEw;

    if (pPos != NULL) {
        if (pSpd != NULL) {
            sv.vx = pPos[0];
            sv.vy = pPos[1];
            sv.vz = pPos[2];
            pEw = Esp_call(a1, a2, pM, &sv, pSpd);
        } else {
            sv.vx = pPos[0];
            sv.vy = pPos[1];
            sv.vz = pPos[2];
            pEw = Esp_call(a1, a2, pM, &sv, 0);
        }
    } else {
        if (pSpd != NULL) {
            pEw = Esp_call(a1, a2, pM, NULL, pSpd);
        } else {
            pEw = Esp_call(a1, a2, pM, NULL, 0);
        }
    }
    if (pEw != ESP_NONE) {
        pEw->pEm = pParent->pEm;
        pEw->Unk32 = 0;
        func_8001BCFC(pEw);
        pEw->Old.vx = pEw->Pos.vx;
        pEw->Old.vy = pEw->Pos.vy;
        pEw->Old.vz = pEw->Pos.vz;
        pEw->Pos.pad = pParent->pEm->Pos_y;
        if (pEw->Attr & 0x10) {
            func_8001BFCC(pEw);
        }
    }
    return pEw;
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", func_8001B484);

INCLUDE_ASM("asm/nonmatchings/game/em_sub", func_8001B894);

/* set the four routine bytes (Rtn01 = Routine_0 | Routine_1 << 16, Rtn23 = Routine_2 | Unk03 << 16) of the live
 * effects of kind Id, only those attached to pM unless pM is NULL (RE2 Esp_ctrl sets Routine0 alone) */
void func_8001BA48(ULONG Id, MATRIX *pM, ULONG Rtn01, ULONG Rtn23)
{
    ULONG i;
    ESP_WORK *pEw;
    UCHAR Rtn1, Rtn3;

    Rtn1 = Rtn01 >> 16;
    Rtn3 = Rtn23 >> 16;
    pEw = Esp;
    /* the loop is never repeated; the original has one (it changes register allocation) */
    do {
        if (pM != NULL) {
            for (i = 0; i < 96; i++, pEw++) {
                if ((pEw->Be_flg & 0x8000) && pEw->Id.W == (USHORT)Id && pEw->pMat == pM) {
                    pEw->Routine_0 = Rtn01;
                    pEw->Routine_1 = Rtn1;
                    pEw->Routine_2 = Rtn23;
                    pEw->Unk03 = Rtn3;
                }
            }
        } else {
            for (i = 0; i < 96; i++, pEw++) {
                if ((pEw->Be_flg & 0x8000) && pEw->Id.W == (USHORT)Id) {
                    pEw->Routine_0 = Rtn01;
                    pEw->Routine_1 = Rtn1;
                    pEw->Routine_2 = Rtn23;
                    pEw->Unk03 = Rtn3;
                }
            }
        }
    } while (0);
}

void Esp_task_main(void)
{
    ESP_WORK *pEw;

    for (pEw = D_80098088 - 1; pEw >= D_80098084; pEw--) {
        if (!(G.Unk2468 & 0x10000000) || pEw->Id.B[0] == 0x15 || pEw->Id.B[0] == 0x1B) {
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

INCLUDE_ASM("asm/nonmatchings/game/em_sub", func_8001BC80);

INCLUDE_ASM("asm/nonmatchings/game/em_sub", func_8001BCFC);

void func_8001BFCC(ESP_WORK *pEw)
{
    LONG y;
    LONG y0;

    if (pEw->Be_flg & 0x80) {
        y0 = Water_ck(pEw->Pos.vx, pEw->Pos.vz);
        if (y0 < pEw->Pos.vy && y0 != 0) {
            pEw->Flg2 |= 8;
        }
    }
    if (*(ULONG *)&pEw->Be_flg & 0x50) {
        if (pEw->Be_flg & 0x40) {
            y = func_8004D720(&pEw->Pos.vx, 2, 0x1000, 0);
            if (y < pEw->Pos.vy) {
                if (y < pEw->Pos.pad) {
                    pEw->Flg2 |= 0x80;
                } else {
                    pEw->Flg2 |= 0x40;
                }
            }
        }
        y = func_8004D720(&pEw->Pos.vx, 2, 0x2000, 0);
        if (y < pEw->Pos.vy) {
            if (y < pEw->Pos.pad) {
                pEw->Flg2 |= 0x20;
                if (pEw->Be_flg & 0x20) {
                    pEw->Spd_x -= pEw->Acc_x;
                    pEw->Pos.vx = pEw->Old.vx;
                    pEw->Unk30 -= pEw->Spd_x;
                }
            } else {
                pEw->Flg2 |= 0x10;
                pEw->Unk32 += y - (USHORT)pEw->Pos.vy;
                pEw->Pos.vy = y;
            }
        } else {
            pEw->Pos.pad = y;
        }
    }
}

void func_8001C168(ESP_WORK *pEw)
{
    SSEQUENCE *pSeq;

    if (pEw->Timer == 0) {
        pEw->Seq_no++;
        pSeq = &pEw->pSeq[pEw->Seq_no];
        switch (pSeq->Time) {
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
        pEw->Timer = pSeq->Time;
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
    Memclr(p, 0xD4);
    return p;
}

ESP_WORK *func_8001C2DC(USHORT Id)
{
    ESP_WORK *pEw;

    pEw = (ESP_WORK *)D_800BF754;
    if ((Id & 0xFF00) == 0xFF00) {
        do {
            if (pEw->Be_flg != 0 && pEw->Id.B[0] == (UCHAR)Id) {
                return pEw;
            }
            pEw--;
        } while (pEw > (ESP_WORK *)(D_800BF754 - 0x4F80));
    } else {
        do {
            if (pEw->Be_flg != 0 && pEw->Id.W == (USHORT)Id) {
                return pEw;
            }
            pEw--;
        } while (pEw > (ESP_WORK *)(D_800BF754 - 0x4F80));
    }
}

void func_8001C384(void) {
}

void func_8001C38C(ESP_WORK *pEw)
{
    if (pEw->Flg2 & 0x30) {
        if (pEw->Snd_no != 0) {
            Snd_se_stad(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), &pEw->Pos, 0, 0);
        }
        func_8001CF24(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
    if (pEw->Flg2 & 0x8) {
        if (pEw->Snd_no != 0) {
            Snd_se_stad(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), &pEw->Pos, 0, 0);
        }
        func_8001CF24(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
}

void func_8001C46C(ESP_WORK *pEw)
{
    func_8001CDA4(pEw, pEw->Unk18);
    pEw->Routine_0 = 0;
}

void func_8001C49C(ESP_WORK *pEw)
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
            Snd_se_stad(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), &pEw->Pos, 0, 0);
        }
        func_8001CF24(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
    if (pEw->Flg2 & 0x8) {
        pEw->Be_flg = 0;
    }
}

void func_8001C5BC(ESP_WORK *pEw)
{
    VECTOR Unused;
    SVECTOR Rot;
    LONG d;
    ESP_WORK *p;
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
        if (pEw->Unk1E == 0 || D_80098728[Rnd()] % pEw->Unk1E == 0) {
            if (pEw->Routine_1 == 3) {
                p = func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                  ((pEw->Unk2E + D_80098728[Rnd()] * 4 - d) << 16) |
                                      (USHORT)(pEw->Unk2C + ((signed char)Rnd() << pEw->Unk22)),
                                  pEw->pMat, pEw->Unk38, 0);
            } else {
                Rot.vx = pEw->Rot[0];
                Rot.vy = pEw->Rot[1];
                Rot.vz = pEw->Rot[2];
                r = Rnd();
                p = func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                  ((pEw->Unk2E + D_80098728[r] * 4 - d) << 16) |
                                      (USHORT)(pEw->Unk2C + ((D_80098728[Rnd()] - 0x80) >> pEw->Unk1B)),
                                  pEw->pMat, pEw->Unk38, &Rot);
            }
            if (pEw->Routine_1 == 5) {
                Snd_se_stad((pEw->Unk22 + (Rnd() & 1)) | 0x10200, &pEw->Pos, 0, 0);
            }
            if (pEw->Routine_1 == 6) {
                Snd_se_stad(pEw->Unk22 | 0x10200, &pEw->Pos, 0, 0);
            }
            if (pEw->Routine_1 == 1) {
                if (--pEw->Unk22 == 0) {
                    pEw->Be_flg = 0;
                    return;
                }
            }
            if (pEw->Routine_1 == 4 && p != ESP_NONE) {
                p->Spd_x += D_80098728[Rnd()] % pEw->Unk22;
                p->Spd_y += D_80098728[Rnd()] % pEw->Unk22;
            }
            if (p != ESP_NONE) {
                p->pEm = pEw->pEm;
            }
            if (!(pEw->Flg2 & 0x100) && p != ESP_NONE) {
                p->Flg2 |= 0x100;
                pEw->Flg2 |= 0x100;
            }
            pEw->UnkB4 = pEw->Unk1A;
        }
    } else {
        pEw->UnkB4--;
    }
}

void func_8001C98C(ESP_WORK *pEw)
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
        Pos.vx = pEw->Pos.vx;
        Pos.vy = pEw->Pos.vy;
        Pos.vz = pEw->Pos.vz;
        func_8001B35C(pEw, 0x1503, (pEw->Unk2E << 16) | pEw->Unk2C, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
    }
}

void func_8001CB04(ESP_WORK *pEw)
{
    HIT_REQ Req;

    if (pEw->Unk18 != 0xFF) {
        if (pEw->UnkB8 == 0) {
            Req.pPos = &pEw->Pos;
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
        Req.pPos = &pEw->Pos;
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

void func_8001CC60(ESP_WORK *pEw)
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
                          (a3 << 16) | (USHORT)(pEw->Unk2C + pEw->Unk1E), &GsIDMATRIX, &pEw->Pos.vx, 0);
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

void func_8001CDA4(ESP_WORK *pEw, ULONG n)
{
    SSEQUENCE *pSeq;

    n &= 0xFF;
    if (n != 0) {
        pEw->Seq_no = pEw->Unk0F + D_80098728[Rnd()] % n;
        pSeq = pEw->pSeq;
        pSeq += pEw->Seq_no;
        pEw->Timer = pSeq->Time;
    }
}

void func_8001CE24(ESP_WORK *pEw, ULONG a1, ULONG a2)
{
    pEw->Acc_x = (pEw->Acc_x * pEw->Unk2E) >> 12;
    pEw->Acc_z = (pEw->Acc_z * pEw->Unk2E) >> 12;
    if ((UCHAR)a1 != 0) {
        pEw->Spd_x = ((pEw->Spd_x + (signed char)Rnd() % (UCHAR)a1) * pEw->Unk2E) >> 12;
    }
    if ((UCHAR)a2 != 0) {
        pEw->Spd_y = ((pEw->Spd_y + (signed char)Rnd() % (UCHAR)a2) * pEw->Unk2E) >> 12;
    }
    pEw->Spd_z = (pEw->Spd_z * pEw->Unk2E) >> 12;
}

void func_8001CF24(ESP_WORK *pEw, ULONG a1, ULONG a2)
{
    switch (pEw->Routine_1) {
    case 2:
        func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | ((a2 & 0xFF) << 8) | (a1 & 0xFF),
                      (pEw->Unk2E << 16) | pEw->Unk2C, &GsIDMATRIX, &pEw->Pos.vx, 0);
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

void func_8001CFF4(ESP_WORK *pEw, ULONG n)
{
    ESP_WORK *p;
    ULONG i;
    ULONG r;

    for (i = 0; i < n; i++) {
        if (pEw->UnkB4 == 0) {
            pEw->UnkB4 = pEw->Unk1A;
            r = Rnd();
            p = func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                 ((pEw->Unk2E + D_80098728[r] * 4) << 16) |
                                     (USHORT)(pEw->Unk2C + ((D_80098728[Rnd()] - 0x80) >> pEw->Unk1B)),
                                 pEw->pMat, pEw->Unk38, 0);
        } else {
            pEw->UnkB4--;
        }
        if (p != ESP_NONE) {
            p->UnkB6 = 1;
        }
        for (p = D_80098090 - 1; p >= D_8009808C; p--) {
            if (p->UnkB6 != 0 && p->Id.W == *(USHORT *)&pEw->Unk18) {
                D_80097BD4[p->Routine_0](p);
                func_8001BC80(p);
                func_8001BCFC(p);
                func_8001C168(p);
            }
        }
    }
    for (p = D_80098090 - 1; p >= D_8009808C; p--) {
        if (p->Id.W == *(USHORT *)&pEw->Unk18) {
            p->UnkB6 = 0;
        }
    }
    pEw->UnkB4 = 0;
}

void func_8001D210(ESP_WORK *pEw)
{
    VECTOR Pos;
    UCHAR Hit;
    UCHAR n;

    switch (pEw->Routine_1) {
    case 0:
        func_8001CDA4(pEw, 3);
        pEw->Spd_x += D_80098728[Rnd()] & 0xF;
        pEw->Spd_y += D_80098728[Rnd()] & 0xF;
        pEw->Routine_1++;
        break;
    case 1:
        if (pEw->Flg2 & 8) {
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy;
            Pos.vz = pEw->Pos.vz;
            func_8001B35C(pEw, 0xF1C, pEw->Unk2C | 0x8000000, &GsIDMATRIX, &Pos.vx, 0);
            pEw->Be_flg = 0;
            break;
        }
        Hit = 0;
        if (pEw->Flg2 & 0x10) {
            if (pEw->UnkB4 > 0) {
                pEw->Be_flg = 0;
                break;
            }
            pEw->Spd_x = pEw->Spd_x * 60 / 100;
            pEw->Spd_y = -(pEw->Spd_y * 40) / 100;
            pEw->Spd_z = (D_80098728[Rnd()] & 3) * 3;
            pEw->UnkB4++;
            Hit = 1;
        }
        if (pEw->Flg2 & 0x20) {
            if (pEw->UnkB6 >= 2) {
                pEw->Be_flg = 0;
                break;
            }
            pEw->Spd_x *= -1;
            pEw->Spd_z = (D_80098728[Rnd()] & 3) * 3;
            pEw->UnkB6++;
            Hit = 1;
        }
        if (Hit) {
            if (pEw->pMat == &G.Player.pSin_parts_ptr[4].Workm) {
                n = 2;
                if (((UCHAR *)&pEw->Id.W)[1] == 2) {
                    n = 0xD;
                }
                Snd_se_stad(n | 0x10100, &pEw->Pos, 0, 0);
            } else {
                Snd_se_stad(0x10314, &pEw->Pos, 0, 0);
            }
        }
        break;
    }
}

void func_8001D480(UCHAR *p)
{
    if (p[1] == 0) {
        *(SHORT *)(p + 8) += (signed char)D_80098728[Rnd()] * p[0x18];
        *(SHORT *)(p + 0xA) += (signed char)D_80098728[Rnd()] * p[0x19];
        p[1]++;
    }
    if (*(USHORT *)(p + 0x26) & 0x30) {
        *(USHORT *)(p + 0x24) = 0;
    }
}

void func_8001D534(ESP_WORK *pEw)
{
    VECTOR Pos;
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    if (pEw->Routine_1 == 0) {
        if (pEw->Unk1A != 0) {
            pEw->Spd_x = ((pEw->Spd_x + (signed char)Rnd() % pEw->Unk1A) * pEw->Unk2E) >> 12;
        }
        if (pEw->Unk1B != 0) {
            pEw->Spd_y = ((pEw->Spd_y + (signed char)Rnd() % pEw->Unk1B) * pEw->Unk2E) >> 12;
        }
        pEw->Routine_1++;
        pEw->Spd_z = (pEw->Spd_z * pEw->Unk2E) >> 12;
    }
    if (!(pEw->UnkBA & 1)) {
        Req.pPos = &pEw->Pos;
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
        Pos.vx = pEw->Pos.vx;
        Pos.vy = pEw->Pos.vy;
        Pos.vz = pEw->Pos.vz;
        func_8001B35C(pEw, 0x1503, (pEw->Unk2E << 16) | pEw->Unk2C, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
        return;
    }
    if (!(pEw->UnkBA & 1)) {
        func_8001B35C(pEw, 0x3003603,
                      (((pEw->Unk2E >> 1) - (pEw->Unk2E >> 2) + ((signed char)Rnd() << 3)) << 16) | pEw->Unk2C,
                      &GsIDMATRIX, &pEw->Pos.vx, 0);
    }
    if (pEw->Flg2 & 0x30) {
        if (pEw->Unk22 == 0) {
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy + 400;
            Pos.vz = pEw->Pos.vz;
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

LONG func_80044804_(MATRIX *pM, PL_WEP_DATA *pW, ULONG Type) __asm__("func_80044804"); /* result or-ed into a hit mask: long here (canonical ENEMY_WORK *) */

void func_8001D7D0(ESP_WORK *pEw)
{
    ESP_WORK *pNew;
    LONG Ret;
    VECTOR Pos;
    SVECTOR Rot;
    VECTOR Avg;
    HIT_REQ Req;

    if (pEw->Unk2C != 0) {
        pEw->UnkBA = pEw->Unk2C;
        pEw->Unk2C = 0;
    }
    Ret = pEw->Flg2 & 0x30;
    Ret |= func_80044804_(&pEw->Workm2, D_8009DBB4[0], pEw->Routine_1 + 6);
    Ret |= func_80047860(&pEw->Workm2, D_8009DBB4[0], pEw->Routine_1 + 6);
    if (++pEw->UnkB4 >= 15) {
        Ret |= 1;
    }
    Pos.vx = (signed char)D_80098728[Rnd()] >> 1;
    Pos.vy = 0;
    Pos.vz = (signed char)D_80098728[Rnd()] >> 1;
    func_8001B35C(pEw, 0xC03, 0x10000000, &pEw->Workm2, &Pos.vx, 0);
    if (pEw->Flg2 & 8) {
        Pos.vx = pEw->Pos.vx;
        Pos.vy = pEw->Pos.vy;
        Pos.vz = pEw->Pos.vz;
        Req.pPos = &Pos;
        Req.W = 0;
        Req.H = 0;
        Req.D = 0;
        Req.R = pEw->UnkBA;
        func_800472EC(&Req, D_80097FCC[15]);
        func_80047BD0(&Req, D_80097FCC[15]);
        Ret |= 1;
        if (Water_ck(pEw->Pos.vx, pEw->Pos.vz) != 1) {
            func_8001B35C(pEw, 0x71C, (USHORT)pEw->UnkBA | 0x0C000000, &GsIDMATRIX, &Pos.vx, 0);
            func_8001B35C(pEw, 0x91C, (USHORT)pEw->UnkBA | 0x15000000, &GsIDMATRIX, &Pos.vx, 0);
            Rot.vx = 0x400;
            Rot.vy = 0;
            Rot.vz = 0;
            func_8001B35C(pEw, 0x20E, (USHORT)pEw->UnkBA | 0x20000000, &GsIDMATRIX, &Pos.vx, &Rot);
            Rot.vx = 0x400;
            Rot.vy = 0;
            Rot.vz = 0x400;
            func_8001B35C(pEw, 0x30E, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Pos.vx, &Rot);
            Avg.vx = (pEw->Pos.vx + pEw->Old.vx) / 2;
            Avg.vy = (pEw->Pos.vy + pEw->Old.vy) / 2;
            Avg.vz = (pEw->Pos.vz + pEw->Old.vz) / 2;
            switch (pEw->Routine_1) {
            case 0:
                func_8001B35C(pEw, 0x3000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            case 1:
                func_8001B35C(pEw, 0x3000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            case 2:
                func_8001B35C(pEw, 0x2000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            case 3:
                func_8001B35C(pEw, 0xA03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            }
            Snd_se_stad(0x1022A, &pEw->Pos, 0, 0);
            pEw->Be_flg = 0;
            return;
        }
        func_8001B35C(pEw, 0x91C, (USHORT)pEw->UnkBA | 0x15000000, &GsIDMATRIX, &Pos.vx, 0);
        Rot.vx = 0x400;
        Rot.vy = 0;
        Rot.vz = 0;
        func_8001B35C(pEw, 0x20E, (USHORT)pEw->UnkBA | 0x20000000, &GsIDMATRIX, &Pos.vx, &Rot);
        Rot.vx = 0x400;
        Rot.vy = 0;
        Rot.vz = 0x400;
        func_8001B35C(pEw, 0x30E, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Pos.vx, &Rot);
    }
    Avg.vx = (pEw->Pos.vx + pEw->Old.vx) / 2;
    Avg.vy = (pEw->Pos.vy + pEw->Old.vy) / 2;
    Avg.vz = (pEw->Pos.vz + pEw->Old.vz) / 2;
    if (Ret) {
        switch (pEw->Routine_1) {
        case 0:
            Snd_se_stad(0x1010E, &pEw->Pos, 0, 0);
            func_8001B35C(pEw, 0x203, (USHORT)pEw->UnkBA | 0x09000000, &GsIDMATRIX, &Avg.vx, 0);
            Req.pPos = &pEw->Pos;
            Req.W = 0;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEw->UnkBA;
            func_800472EC(&Req, D_80097FCC[15]);
            func_80047BD0(&Req, D_80097FCC[15]);
            break;
        case 1:
            Snd_se_stad(0x1010F, &pEw->Pos, 0, 0);
            func_8001B35C(pEw, 0x3000903, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Avg.vx, 0);
            func_8001B35C(pEw, 0x3000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy;
            Pos.vz = pEw->Pos.vz;
            func_8001B35C(pEw, 0x1000803, (USHORT)pEw->UnkBA | 0x30000000, &GsIDMATRIX, &Pos.vx, 0);
            if (pEw->Flg2 & 0x20) {
                func_8001B35C(pEw, 0x3000D03, (USHORT)(pEw->UnkBA - 0xFA) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
                func_8001B35C(pEw, 0x3000D03, (USHORT)(pEw->UnkBA + 0xFA) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            } else {
                func_8001B35C(pEw, 0x3000D03, (USHORT)pEw->UnkBA | 0x32000000, &GsIDMATRIX, &pEw->Old.vx, 0);
                func_8001B35C(pEw, 0x3000D03, (USHORT)(pEw->UnkBA - 0x15E) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
                func_8001B35C(pEw, 0x3000D03, (USHORT)(pEw->UnkBA + 0x15E) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            }
            Req.pPos = &pEw->Pos;
            Req.W = 0;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEw->UnkBA;
            func_800472EC(&Req, D_80097FCC[16]);
            func_80047BD0(&Req, D_80097FCC[16]);
            break;
        case 2:
            Snd_se_stad(0x10110, &pEw->Pos, 0, 0);
            func_8001B35C(pEw, 0x2000903, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Avg.vx, 0);
            func_8001B35C(pEw, 0x2000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy - 100;
            Pos.vz = pEw->Pos.vz;
            pNew = func_8001B35C(pEw, 0x3000E00, (USHORT)pEw->UnkBA | 0x24000000, &GsIDMATRIX, &Pos.vx, 0);
            if (pNew != ESP_NONE) {
                if (pEw->Flg2 & 0x30) {
                    pNew->UnkB6 = 1;
                } else {
                    pNew->UnkB6 = 0;
                }
            }
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy + 300;
            Pos.vz = pEw->Pos.vz;
            func_8001B35C(pEw, 0x3001000, (USHORT)pEw->UnkBA | 0x24000000, &GsIDMATRIX, &Pos.vx, 0);
            break;
        case 3:
            Snd_se_stad(0x10111, &pEw->Pos, 0, 0);
            func_8001B35C(pEw, 0x2203, (USHORT)pEw->UnkBA | 0x34000000, &GsIDMATRIX, &Avg.vx, 0);
            func_8001B35C(pEw, 0xA03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
            pNew = func_8001B35C(pEw, 0x2003, (USHORT)pEw->UnkBA | 0x10000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            if (pNew != ESP_NONE) {
                if (pEw->Flg2 & 0x20) {
                    pNew->UnkB6 = 1;
                } else {
                    pNew->UnkB6 = 0;
                }
            }
            func_8001B35C(pEw, 0xB03, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            break;
        }
        pEw->Be_flg = 0;
    }
}

void func_8001E0F4(ESP_WORK *pEw)
{
    VECTOR Spd;
    SVECTOR Rot;
    VECTOR Pos;
    LONG Hit;
    ESP_WORK *p;

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
            func_8001B35C(pEw, 0x3000D02, 0x18000000, &pEw->Workm2, NULL, &Rot);
            Rot.vx = 0;
            Rot.vy = 0x400;
            Rot.vz = 0x400;
            func_8001B35C(pEw, 0x3000D02, 0x18000000, &pEw->Workm2, NULL, &Rot);
        }
        pEw->Routine_2++;
    case 1:
        if (pEw->Pos.vx >= 0x7918) {
            Hit |= 1;
        }
        if (pEw->Pos.vx < -0x7917) {
            Hit |= 1;
        }
        if (pEw->Pos.vz >= 0x7918) {
            Hit |= 1;
        }
        if (pEw->Pos.vz < -0x7917) {
            Hit |= 1;
        }
        if (pEw->UnkB8 != 0) {
            if (pEw->UnkB8 == 10) {
                pEw->Be_flg = 0x8000;
                func_8001B894(0xD02, &pEw->Workm2);
            }
            if (--pEw->UnkB8 == 1) {
                pEw->Be_flg = 0;
            }
            break;
        }
        if (pEw->Unk32 >= 30000) {
            p = Esp_call(((pEw->UnkB3 & 0xF) << 24) | (((UCHAR *)&pEw->Id.W)[1] << 8) | ((UCHAR *)&pEw->Id.W)[0],
                              (pEw->Unk2E << 16) | pEw->Unk2C, &pEw->Workm2, NULL, 0);
            func_8001BC80(p);
            func_8001BCFC(p);
            pEw->UnkB8 = 10;
        }
        if (pEw->Flg2 & 0x30) {
            Hit |= 1;
        }
        Pos.vx = (pEw->Pos.vx + pEw->Old.vx) / 2;
        Pos.vy = (pEw->Pos.vy + pEw->Old.vy) / 2;
        Pos.vz = (pEw->Pos.vz + pEw->Old.vz) / 2;
        Hit |= func_80044804_(&pEw->Workm2, D_8009DBB4[0], 10);
        Hit |= func_80047860(&pEw->Workm2, D_8009DBB4[0], 10);
        Spd.vx = (signed char)D_80098728[Rnd()] >> 1;
        Spd.vy = 500;
        Spd.vz = (signed char)D_80098728[Rnd()] >> 1;
        func_8001B35C(pEw, 0xC03, 0x18000000, &pEw->Workm2, &Spd.vx, 0);
        if (Hit) {
            func_8001B894(0xD02, &pEw->Workm2);
            Snd_se_stad(0x1010A, &pEw->Pos, 0, 0);
            func_8001B35C(pEw, 0x203, (USHORT)pEw->UnkBA | 0xD000000, &GsIDMATRIX, &Pos.vx, 0);
            pEw->Be_flg = 0;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", func_8001E444);

void func_8001F208(ESP_WORK *pEw)
{
    VECTOR Pos;
    SVECTOR Rot;

    switch (pEw->Routine_1) {
    case 0:
        pEw->Routine_1++;
    case 1:
        Pos.vx = pEw->Unk38[0];
        Pos.vy = pEw->Unk38[1] - 100;
        Pos.vz = pEw->Unk38[2];
        Rot.vx = 0;
        Rot.vy = 0;
        Rot.vz = 0;
        func_8001B35C(pEw, 0x805, ((pEw->Unk2E >> 1) << 16) | pEw->Unk2C, pEw->pMat, &Pos.vx, &Rot);
        Rot.vx = 0;
        Rot.vy = 0x400;
        Rot.vz = 0;
        func_8001B35C(pEw, 0x805, ((pEw->Unk2E >> 1) << 16) | pEw->Unk2C, pEw->pMat, &Pos.vx, &Rot);
        pEw->Routine_1++;
        break;
    default:
        Pos.vx = (signed char)D_80098728[Rnd()] >> 1;
        Pos.vy = -150;
        Pos.vz = (signed char)D_80098728[Rnd()] >> 1;
        func_8001B35C(pEw, 0xC03, ((pEw->Unk2E >> 1) << 16) | pEw->Unk2C, pEw->pMat, &Pos.vx, 0);
        break;
    }
}

void func_8001F378(ESP_WORK *pEw)
{
    VECTOR Pos;
    VECTOR Dummy;

    switch (pEw->Routine_1) {
    case 0:
        func_8001B35C(pEw, 0x3000903, (pEw->Unk2E << 18) | pEw->Unk2C, &GsIDMATRIX, &pEw->Pos.vx, 0);
        func_8001B35C(pEw, 0x3000A03, ((pEw->Unk2E * 5) << 16) | pEw->Unk2C, &GsIDMATRIX, &pEw->Pos.vx, 0);
        break;
    case 2:
        Pos.vx = pEw->Pos.vx;
        Pos.vy = pEw->Pos.vy;
        Pos.vz = pEw->Pos.vz;
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

void func_8001F4F4(ESP_WORK *pEw)
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
            Req.pPos = &pEw->Pos;
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

void func_8001F674(ESP_WORK *pEw)
{
    SHORT Ofs[3] = { -200, 0, 200 };
    SHORT Spd[3] = { -2, 0, -2 };
    UCHAR i;
    ESP_WORK *p;

    if (++pEw->UnkB4 & 1) {
        for (i = 0; i < 3; i++) {
            p = func_8001B35C(pEw, 0x2103,
                              ((0x2800 - pEw->UnkB4 * 1000) << 16) |
                                  (USHORT)(pEw->Unk2C + (signed char)D_80098728[Rnd()] + Ofs[i]),
                              &GsIDMATRIX, &pEw->Old.vx, 0);
            if (p != ESP_NONE) {
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

void func_8001F824(ESP_WORK *pEw)
{
    VECTOR Unused;
    VECTOR Pos;
    ESP_WORK *p;

    switch (pEw->Routine_1) {
    case 0:
        func_8001CDA4(pEw, pEw->Unk18);
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
        Pos.vx = pEw->Old.vx;
        Pos.vy = pEw->Old.vy;
        Pos.vz = pEw->Old.vz;
        p = func_8001B35C(pEw, 0x505, ((pEw->Unk2E - ((signed char)Rnd() << 4)) << 16) | pEw->Unk2C,
                          &GsIDMATRIX, &Pos.vx, 0);
        if (p != ESP_NONE) {
            p->UnkBA = 1;
            if (!(pEw->Flg2 & 0x80)) {
                p->UnkBA = 0x81;
            }
        }
        pEw->UnkB4 = 8;
    } else {
        if (pEw->UnkB4 == 1 || pEw->UnkB4 == 7) {
            Pos.vx = pEw->Old.vx;
            Pos.vy = pEw->Old.vy;
            Pos.vz = pEw->Old.vz;
            p = func_8001B35C(pEw, 0x405, ((pEw->Unk2E - ((signed char)Rnd() << 4)) << 16) | pEw->Unk2C,
                              &GsIDMATRIX, &Pos.vx, 0);
            if (p != ESP_NONE) {
                p->Spd_x = (D_80098728[Rnd()] & 0x1F) + 0x50;
                if (pEw->UnkB4 == 7) {
                    p->UnkBA = 1;
                }
            }
        }
        pEw->UnkB4++;
    }
}

void func_8001FA90(ESP_WORK *pEw)
{
    VECTOR Pos;
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    switch (pEw->Routine_1) {
    case 0:
        if (pEw->Unk18 == 0) {
            pEw->UnkB4 = (D_80098728[Rnd()] & 0xF) | 0x10;
            pEw->UnkB6 = (((D_80098728[Rnd()] & 0xF) + 0x10) * pEw->Unk2E) >> 11;
        } else {
            pEw->UnkB4 = (D_80098728[Rnd()] & 0xF) + 0x18;
            pEw->UnkB6 = (((D_80098728[Rnd()] & 0xF) + 0x18) * pEw->Unk2E) >> 11;
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
                    Req.pPos = &pEw->Pos;
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
    Pos.vx = pEw->Pos.vx;
    Pos.vy = pEw->Pos.vy;
    Pos.vz = pEw->Pos.vz;
    func_8001B35C(pEw, 0x1503, ((pEw->Unk2E >> 2) << 16) | pEw->Unk2C, pEw->pMat, &Pos.vx, 0);
    pEw->Be_flg = 0;
}

void func_8001FDD0(ESP_WORK *pEw)
{
    HIT_REQ Req;
    VECTOR Pos;
    ESP_WORK *pNew;
    ULONG i;

    for (i = 0; i < 2; i++) {
        pNew = func_8001B35C(pEw, 0x3000F00, (pEw->Unk2E << 16) | (USHORT)(pEw->Unk2C + (signed char)Rnd() * 4),
                             pEw->pMat, pEw->Unk38, 0);
        if (pNew != ESP_NONE) {
            pNew->Spd_x = (D_80098728[Rnd()] & 0x3F) + 0x82;
            pNew->Spd_y = (D_80098728[Rnd()] & 0x7F) - 100;
        }
    }
    Req.pPos = &pEw->Pos;
    Req.W = 0;
    Req.H = 0;
    Req.D = 0;
    Req.R = pEw->Unk2C;
    func_800472EC(&Req, D_80097FCC[18]);
    func_80047BD0(&Req, D_80097FCC[18]);
    if (++pEw->UnkB4 >= 3) {
        if (pEw->UnkB6 != 0) {
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy;
            Pos.vz = pEw->Pos.vz;
            func_8001B35C(pEw, 0x2000E03, pEw->Unk2C | 0x20000000, &GsIDMATRIX, &Pos.vx, 0);
        }
        pEw->Be_flg = 0;
    }
}

void func_8001FF6C(ESP_WORK *pEw)
{
    VECTOR Pos;

    switch (pEw->Routine_1) {
    case 0:
        func_8001B35C(pEw, 0x108, pEw->Unk2C | 0x10000000, pEw->pMat, pEw->Unk38, 0);
        pEw->Routine_1++;
        break;
    case 1:
        func_8001B35C(pEw, 9, (USHORT)(pEw->Unk2C + (signed char)D_80098728[Rnd()] * 7 + 0x800) | 0x9000000,
                      pEw->pMat, pEw->Unk38, 0);
        func_8001B35C(pEw, 9, (USHORT)(pEw->Unk2C + (signed char)D_80098728[Rnd()] * 7 + 0x800) | 0xB000000,
                      pEw->pMat, pEw->Unk38, 0);
        func_8001B35C(pEw, 0x109, (USHORT)(pEw->Unk2C + (signed char)D_80098728[Rnd()] * 4 + 0x800) | 0xA000000,
                      pEw->pMat, pEw->Unk38, 0);
        func_8001B35C(pEw, 0x209, (USHORT)(pEw->Unk2C + (signed char)D_80098728[Rnd()] * 2 + 0x800) | 0x7000000,
                      pEw->pMat, pEw->Unk38, 0);
        func_8001B35C(pEw, 0x309, (USHORT)(pEw->Unk2C + (signed char)D_80098728[Rnd()] * 6 + 0x800) | 0x8000000,
                      pEw->pMat, pEw->Unk38, 0);
        func_8001B35C(pEw, 9, (USHORT)(pEw->Unk2C + (signed char)D_80098728[Rnd()] * 7) | 0xB000000,
                      pEw->pMat, pEw->Unk38, 0);
        Pos.vx = pEw->Unk38[0];
        Pos.vy = pEw->Unk38[1] + 200;
        Pos.vz = pEw->Unk38[2];
        func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | 0x300, 0x10000000, pEw->pMat, &Pos.vx, 0);
        pEw->Routine_1++;
        break;
    case 2:
        if (++pEw->UnkB4 >= 9) {
            pEw->Routine_1++;
        }
        break;
    case 3:
        Pos.vx = pEw->Unk38[0];
        Pos.vy = pEw->Unk38[1] + 200;
        Pos.vz = pEw->Unk38[2];
        func_8001B35C(pEw, ((pEw->UnkB3 & 0xF) << 24) | 0x200, 0x10000000, pEw->pMat, &Pos.vx, 0);
        pEw->Be_flg = 0;
        break;
    }
}
