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
#include "om.h"

/*
 * EM_SUB.C (RE2 EM_SUB.C): helpers shared by the player and the enemy modules: movement
 * along the facing direction, direction/range checks, motion root positions (null pos),
 * motion speeds (Spd_get), shadows (Kage_work*) and neck tracking (Rot_neck*).
 * From Esp_init_C on, the object holds RE2's ESP.C (effect pool, Esp_call, Esp_kill, ...)
 * and the first part of ESP_R.C (the Esp_tblj routines Esp_00..Esp_20).
 */

/* is pV inside the x/z box pPxzhw (x, z, w, d)? */
ULONG Hani_ck(VECTOR *pV, SHORT *pPxzhw)
{
    if ((ULONG)(pV->vx - pPxzhw[0]) > (ULONG)pPxzhw[2] || (ULONG)(pV->vz - pPxzhw[1]) > (ULONG)pPxzhw[3]) {
        return 0;
    }
    return 1;
}

/* move pEm by its motion speed (Spd_x/z) turned to Cdir_y + muki; slower in water */
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
    ApplyMatrixSV(&M, EM_SPD_SV(pEm), &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_z += Sv.vz;
}

/* move pEm by Spd_x/y/z turned by Cdir_y + muki_y and Cdir_z + muki_z */
void Add_speedXYZ(ENEMY_WORK *pEm, LONG muki_y, LONG muki_z)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki_y;
    Sv.vz = pEm->Cdir_z + (SHORT)muki_z;
    RotMatrix_gte(&Sv, &M);
    ApplyMatrixSV(&M, EM_SPD_SV(pEm), &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_y += Sv.vy;
    pEm->Pos_z += Sv.vz;
}

/* world x/z step of the motion speed turned to Cdir_y + muki, without moving */
void Get_speedXZ(ENEMY_WORK *pEm, LONG muki, SHORT *pX, SHORT *pZ)
{
    SVECTOR Sv;
    MATRIX M;

    Sv.vx = 0;
    Sv.vy = pEm->Cdir_y + (SHORT)muki;
    Sv.vz = 0;
    RotMatrix_gte(&Sv, &M);
    ApplyMatrixSV(&M, EM_SPD_SV(pEm), &Sv);
    *pX = Sv.vx;
    *pZ = Sv.vz;
}

/* Add_speedXYZ in the space of the parent matrix pSuper */
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
    ApplyMatrixSV(&Tm, EM_SPD_SV(pEm), &Sv);
    pEm->Pos_x += Sv.vx;
    pEm->Pos_y += Sv.vy;
    pEm->Pos_z += Sv.vz;
}

/* direction (0..0xFFF) from (Hontai_x, Hontai_z) to (Target_x, Target_z) */
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

/* turn needed to face (Vec_x, Vec_z): 0 when within Add_dir, else +-Add_dir */
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

/* turn needed to face direction Dir: 0 when within Add_dir, else +-Add_dir */
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

/* is pP outside the +-Hani_dir cone around Hed_dir seen from pV? */
LONG Dir_pos_ck(VECTOR *pV, VECTOR *pP, SHORT Hed_dir, SHORT Hani_dir)
{
    SHORT Dir;

    Dir = Direction_ck((SHORT)pV->vx, (SHORT)pV->vz, (SHORT)pP->vx, (SHORT)pP->vz);
    return ((((LONG)Dir - (LONG)Hed_dir) + (LONG)Hani_dir) & 0xFFFU) < (ULONG)((LONG)Hani_dir << 1) ^ 1;
}

/* x/z distance between pV and pP below Ll? */
LONG L_pos_ckXZ(VECTOR *pV, VECTOR *pP, ULONG Ll)
{
    LONG Dx;
    LONG Dz;

    Dx = (pV->vx - pP->vx) >> 1;
    Dz = (pV->vz - pP->vz) >> 1;
    return (ULONG)(SquareRoot0(Dx * Dx + Dz * Dz) * 2) < Ll;
}

/* L_pos_ckXZ from the point pSv (or the origin) transformed by pM */
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

/* does pT face the same way as pEm (within +-90 degrees)? */
LONG Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    return ((((LONG)pT->Cdir_y - (LONG)pEm->Cdir_y) + 0x400) & 0xFFF) < 0x800;
}

/* does pT face away from the point (Pos_x, Pos_z)? */
LONG Cdir_ck_pos(LONG Pos_x, LONG Pos_z, ENEMY_WORK *pT)
{
    return (ULONG)(((USHORT)pT->Cdir_y - Direction_ck((SHORT)Pos_x, (SHORT)Pos_z, (SHORT)pT->Pos_x, (SHORT)pT->Pos_z)) + 0x400 & 0xFFF) < 0x800;
}

/* does pT face away from pEm? */
LONG Cdir_ck2(ENEMY_WORK *pEm, ENEMY_WORK *pT)
{
    SHORT Dir;

    Dir = Direction_ck((SHORT)pEm->Pos_x, (SHORT)pEm->Pos_z, (SHORT)pT->Pos_x, (SHORT)pT->Pos_z);
    return ((((LONG)pT->Cdir_y - (LONG)Dir) + 0x400) & 0xFFF) < 0x800;
}

/* are the directions Dir0 and Dir1 within +-90 degrees of each other? */
LONG Dir_ck(LONG Dir0, LONG Dir1)
{
    return ((((SHORT)Dir1 - (SHORT)Dir0) + 0x400) & 0xFFF) < 0x800;
}

/* was a direction or action button pressed this frame (struggling out of a grab)? */
ULONG Gacha_ch(void)
{
    return (G.Key_trg & 0xF4F) != 0;
}

/* is pTa inside the view triangle from pEm towards pAi, Han wide at pAi? */
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

/* base position of the current enemy (G.pEm) from its motion null position; copied to pEm */
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

/* Pos = Base_pos + the null position of the current frame turned to Cdir_y */
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

/* null (root) position of the current frame of G.pEm; R_flg plays the motion backwards */
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

/* Null_pos_set on x/z, using the interpolated frames of get_null_pos2 */
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

/* get_null_pos with interpolated frames (seq flag 0x100) and y = 0 */
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
                Hokan_key_set(pKan, pSeq_t, pSeq + 1, R_flg);
            }
            rr = 0x100 - r;
        } else {
            if (!(pSeq[-1] & 0x100)) {
                Hokan_key_set(pKan, pSeq_t, pSeq - 1, 0);
            }
        }
        pP = pEm->pSin_parts_ptr;
        pSv->vx = (USHORT)pP->Pos_base_x + ((pP->Pos_step_x * rr) >> 8);
        pSv->vy = 0;
        pSv->vz = (USHORT)pP->Pos_base_z + ((pP->Pos_step_z * rr) >> 8);
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

/* Spd_get for sequences with in-between frames (seq word bit 8, rate in the low byte):
 * Spd_x/y/z = root offset of this frame minus the last frame's (Spd_base); R_flg plays it backwards */
void Spd_get2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq_t, ULONG R_flg)
{
    USHORT *pSeq;
    ULONG f_num;
    SHORT f_base;
    ULONG Rec_sz;
    UCHAR *pTop;
    UCHAR *Bone_adr;
    PARTS_W *pP;
    ULONG w;
    ULONG r;
    USHORT rr;
    SHORT x;
    SHORT y;
    SHORT z;

    pTop = (UCHAR *)pSeq_t;
    pSeq = (USHORT *)((UCHAR *)pSeq_t + pEm->Move_no * 8);
    f_base = pSeq[2];
    f_num = pSeq[0];
    pSeq++;
    if (R_flg != 0) {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + (f_num - pEm->Move_cnt - 1);
    } else {
        pSeq = (USHORT *)((UCHAR *)pSeq_t + *pSeq) + pEm->Move_cnt;
    }
    w = *pSeq;
    if (w & 0x100) {
        /* in-between frame: interpolate the stored root offset (as get_null_pos2) */
        r = w & 0xFF;
        rr = r;
        if (R_flg != 0) {
            if (!(pSeq[1] & 0x100)) {
                Hokan_key_set(pKan, pSeq_t, pSeq + 1, R_flg);
            }
            rr = 0x100 - r;
        } else {
            if (!(pSeq[-1] & 0x100)) {
                Hokan_key_set(pKan, pSeq_t, pSeq - 1, 0);
            }
        }
        pP = pEm->pSin_parts_ptr;
        x = (USHORT)pP->Pos_base_x + ((pP->Pos_step_x * rr) >> 8);
        y = (USHORT)pP->Pos_base_y + ((pP->Pos_step_y * rr) >> 8);
        z = (USHORT)pP->Pos_base_z + ((pP->Pos_step_z * rr) >> 8);
    } else {
        Rec_sz = ((KAN_HEAD *)pKan)->Rec_sz >> 16;
        Bone_adr = (UCHAR *)pKan + ((KAN_HEAD *)pKan)->Bone_ofs;
        Bone_adr += (Rec_sz >> 2) * (f_base + (w & 0xFF)) * 4;
        /* the loop is never repeated; the original has one (it changes register allocation) */
        do {
            x = ((SVECTOR *)Bone_adr)->vx;
            y = ((SVECTOR *)Bone_adr)->vy;
            z = ((SVECTOR *)Bone_adr)->vz;
        } while (0);
    }
    if (pEm->Move_cnt == 0) {
        if (R_flg != 0) {
            pEm->Spd_base_x = x;
            pEm->Spd_base_y = y;
            pEm->Spd_base_z = z;
            pSeq = (USHORT *)(pTop + pEm->Move_no * 8);
            pSeq = (USHORT *)((UCHAR *)pSeq_t + pSeq[1]);
            Bone_adr = (UCHAR *)pKan + ((KAN_HEAD *)pKan)->Bone_ofs;
            Bone_adr += (Rec_sz >> 2) * (f_base + *(UCHAR *)pSeq) * 4;
            pEm->Spd_base_x += ((SVECTOR *)Bone_adr)->vx;
            pEm->Spd_base_y += ((SVECTOR *)Bone_adr)->vy;
            pEm->Spd_base_z += ((SVECTOR *)Bone_adr)->vz;
        } else {
            *(ULONG *)&pEm->Spd_base_x = 0;
            pEm->Spd_base_z = 0;
        }
    }
    pEm->Spd_x = x - pEm->Spd_base_x;
    pEm->Spd_y = y - pEm->Spd_base_y;
    pEm->Spd_z = z - pEm->Spd_base_z;
    *(ULONG *)&pEm->Spd_base_x = x | (y << 16);
    pEm->Spd_base_z = z;
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Spd_get_sub);

/* play the enemy sound coded in the top 4 bits of the current seq frame */
void Em_seq_sound(ENEMY_WORK *pEm)
{
    if (*pEm->pNow_seq & 0xF000) {
        Snd_se_enem((*pEm->pNow_seq >> 12) - 1, pEm);
    }
}

/* keep the foot Foot_no planted: Mode 0 moves pEm, else Spd_x = the foot's x/z slide */
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
    pM = EM_WORKM(pEm);
    pP_ptr = pEm->pSin_parts_ptr;
    RotMatrix(CDIR_SV(pEm), pM);
    if (pEm->Be_flg & 0xC0000) {
        Rot.vy = 0;
        Rot.vx = pEm->Tilt[0];
        Rot.vz = pEm->Tilt[1];
        RotMatrix_gte(&Rot, &Mat);
        MulMatrix2(&Mat, pM);
    }
    CompM(pM, PW_M(pP_ptr), &Mat);
    CompM(&Mat, PW_M(&pP_ptr[8]), &Mat);
    pP_ptr += Foot_no * 3 + 11;
    CompM(&Mat, PW_M(&pP_ptr[-2]), &Mat);
    CompM(&Mat, PW_M(&pP_ptr[-1]), &Mat);
    CompM(&Mat, PW_M(pP_ptr), &Mat);
    if (Mode == 0) {
        pEm->Pos_x = pEm->Pos_x - (Mat.t[0] - pP_ptr->Workm.t[0]);
        pEm->Pos_z = pEm->Pos_z - (Mat.t[2] - pP_ptr->Workm.t[2]);
    } else {
        x = Mat.t[0] - pP_ptr->Workm.t[0];
        z = Mat.t[2] - pP_ptr->Workm.t[2];
        pEm->Spd_x = SquareRoot0(x * x + z * z);
    }
}

/* set up the 50 shadow quads (POLY_FT4 pairs, shadow texture) */
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

/* free the five 9-quad shadows */
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

/* take a free shadow for the position pPos (Off: x/z offset pair, Half: x/z half size pair) */
void Kage_work_set(KAGE_WORK **ppK, ULONG Off, ULONG Half, ULONG Color, LONG *pPos)
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
/* take a 9-quad shadow (nine Kage_work quads) for pPos */
void Kage_work9_set(KAGE_WORK9 **ppK, ULONG Off, ULONG Half, ULONG Color, LONG *pPos)
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

/* place the shadow at the position it follows, on the ground Grand, turned to Dir_y */
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
        pKw->Pos_x = Rot_sv.vx + (SHORT)pKw->pPos[0];
        pKw->Pos_y = Grand;
        pKw->Pos_z = Rot_sv.vz + (SHORT)pKw->pPos[2];
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

/* transform and add the visible shadows to the ordering table */
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
            Ot_base = Ot_get_pos(pKw->pPos);
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

/* set the colour of a shadow's quads */
void Kage_work_color_set(KAGE_WORK *pK, ULONG Color)
{
    *(ULONG *)&pK->Prim[0].r0 = (*(ULONG *)&pK->Prim[0].r0 & 0xFF000000) | Color;
    *(ULONG *)&pK->Prim[1].r0 = (*(ULONG *)&pK->Prim[1].r0 & 0xFF000000) | Color;
}

/* set the colour of the nine quads of a 9-quad shadow */
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

/* turn the neck part of pPm towards the neck of its target pEnemy_ptr, limited by the neck free work */
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

    if (!(pPm->Neck_flg & 1)) {
        Own_neck_no = pPm->Neck_no;
        pOwn_parts = pPm->pSin_parts_ptr;
        pOwn_p = &pOwn_parts[Own_neck_no];
        pNeck_enemy = pPm->pEnemy_ptr;
        Enemy_neck_no = pNeck_enemy->Neck_no;
        Body_dir_y = pOwn_parts->Cdir_y;
        pEn_parts = pNeck_enemy->pSin_parts_ptr;
        pEn_p = &pEn_parts[Enemy_neck_no];
        Neck_tgt = *WORKM_POS_V(pEn_p);
        Dir_sum = Cdir_y + Body_dir_y;
        Neck_Dir_ZY(WORKM_POS_V(pOwn_p), &Neck_tgt, &Neck_rot, Dir_sum);
        if (pPm->Neck_flg & 2) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
        }
        if (pPm == pNeck_enemy) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
            Neck_rot.vz = 0;
        }
        Spd_lim = (USHORT)PW_NECK(pOwn_p)->Add_dir_y;
        Vel = PW_NECK(pOwn_p)->L_rot_y;
        if (pPm->Neck_flg & 0x80) {
            Spd_lim = PW_NECK(pOwn_p)->Sce_add_dir_y;
        }
        Ang_dif = (Neck_rot.vy - Cdir_y) - (SHORT)Body_dir_y;
        if ((LONG)((Ang_dif + Vel) & 0xFFF) > Vel * 2) {
            if (!(Ang_dif & 0x800)) {
                Neck_rot.vy = Vel + Dir_sum;
            } else {
                Neck_rot.vy = Dir_sum - Vel;
            }
        }
        Free_dif = Spd_lim + ((USHORT)Neck_rot.vy - ((USHORT)PW_NECK(pOwn_p)->N_rot_y + (Cdir_y + (USHORT)pOwn_p->Cdir_y + Body_dir_y)));
        Vel = PW_NECK(pOwn_p)->N_rot_y - Spd_lim;
        Free_dif12 = Free_dif & 0xFFF;
        PW_NECK(pOwn_p)->N_rot_y = Vel;
        if (!(Free_dif & 0x800)) {
            PW_NECK(pOwn_p)->N_rot_y = Vel + Spd_lim * 2;
        }
        if ((LONG)Free_dif12 < (LONG)(Spd_lim << 16) >> 15) {
            PW_NECK(pOwn_p)->N_rot_y = Neck_rot.vy - (Cdir_y + pOwn_p->Cdir_y + Body_dir_y);
        }
        pOwn_p->Cdir_y += PW_NECK(pOwn_p)->N_rot_y;
        RotMatrix_gte(CDIR_SV(pOwn_p), PW_M(pOwn_p));
        pOwn_p->Cdir_y -= PW_NECK(pOwn_p)->N_rot_y;
    }
}

/* Rot_neck_em that only returns the new neck turn in *pDir */
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

    if (!(pPm->Neck_flg & 1)) {
        Own_neck_no = pPm->Neck_no;
        pOwn_parts = pPm->pSin_parts_ptr;
        pOwn_p = &pOwn_parts[Own_neck_no];
        pNeck_enemy = pPm->pEnemy_ptr;
        Enemy_neck_no = pNeck_enemy->Neck_no;
        Body_dir_y = pOwn_parts->Cdir_y;
        pEn_parts = pNeck_enemy->pSin_parts_ptr;
        pEn_p = &pEn_parts[Enemy_neck_no];
        Neck_tgt = *WORKM_POS_V(pEn_p);
        Dir_sum = Cdir_y + Body_dir_y;
        Neck_Dir_ZY(WORKM_POS_V(pOwn_p), &Neck_tgt, &Neck_rot, Dir_sum);
        if (pPm->Neck_flg & 2) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
        }
        if (pPm == pNeck_enemy) {
            Neck_rot.vy = Cdir_y + pOwn_p->Cdir_y + Body_dir_y;
            Neck_rot.vz = 0;
        }
        Spd_lim = (USHORT)PW_NECK(pOwn_p)->Add_dir_y;
        Vel = PW_NECK(pOwn_p)->L_rot_y;
        if (pPm->Neck_flg & 0x80) {
            Spd_lim = PW_NECK(pOwn_p)->Sce_add_dir_y;
        }
        Ang_dif = (Neck_rot.vy - Cdir_y) - (SHORT)Body_dir_y;
        if ((LONG)((Ang_dif + Vel) & 0xFFF) > Vel * 2) {
            if (!(Ang_dif & 0x800)) {
                Neck_rot.vy = Vel + Dir_sum;
            } else {
                Neck_rot.vy = Dir_sum - Vel;
            }
        }
        Free_dif = Spd_lim + ((USHORT)Neck_rot.vy - ((USHORT)PW_NECK(pOwn_p)->N_rot_y + (Cdir_y + (USHORT)pOwn_p->Cdir_y + Body_dir_y)));
        Vel = PW_NECK(pOwn_p)->N_rot_y - Spd_lim;
        Free_dif12 = Free_dif & 0xFFF;
        PW_NECK(pOwn_p)->N_rot_y = Vel;
        if (!(Free_dif & 0x800)) {
            PW_NECK(pOwn_p)->N_rot_y = Vel + Spd_lim * 2;
        }
        if ((LONG)Free_dif12 < (LONG)(Spd_lim << 16) >> 15) {
            PW_NECK(pOwn_p)->N_rot_y = Neck_rot.vy - (Cdir_y + pOwn_p->Cdir_y + Body_dir_y);
        }
        *pDir = PW_NECK(pOwn_p)->N_rot_y;
    }
}

/* y and z angles from pA to pB */
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

/* pNow_seq = the current seq frame, skipping interpolated (0x100) frames */
void Now_seq_set(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq_t, ULONG R_flg)
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

/* record the kill of pEm in the enemy flags of the current difficulty */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm)
{
    if (G.System_flg & 0x80) {
        if (Flg_ck(G.Unk7890, pEm->Em_set_flg) == 0) {
            Em_kill_score(pEm->Id, 0);
        }
        Flg_on(G.Unk7890, pEm->Em_set_flg);
    } else if (G.Unk783E >= 2) {
        Flg_on(G.Unk78B0, pEm->Em_set_flg);
    } else {
        Flg_on(G.Unk7890, pEm->Em_set_flg);
    }
}

/* Hit_area Bottom/Top from the heights of two parts (low half: part, high half: offset) */
void Hit_area_parts_set(ENEMY_WORK *pEm, ULONG Bottom_part, ULONG Top_part)
{
    PARTS_W *pParts = pEm->pSin_parts_ptr;
    USHORT y = (USHORT)pEm->Hit_area.pM->t[1];

    pEm->Hit_area.Bottom = (USHORT)pParts[Bottom_part & 0xFFFF].Workm.t[1] - y + (Bottom_part >> 16);
    pEm->Hit_area.Top = (USHORT)pParts[Top_part & 0xFFFF].Workm.t[1] - y + (Top_part >> 16);
}

/* append a copy of pSrc to the enemy list and set up its parts */
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
    pG->pEm_mem = (ENEMY_WORK *)PartsWork_link(pG->pEnemy[i], PartsWork_set(pG->pEnemy[i], (ULONG)pG->pEm_mem),
                                              pG->pEnemy[i]->pUnk10C);
    return pG->pEnemy[i];
}

/* the last enemy (pEnemy[2..]) of kind Id, or NULL */
ENEMY_WORK *Em_work_search(ULONG Id)
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

/* clear the effect pool and load the core effects (CORE00.ESP) */
void Esp_init_C(void)
{
    ULONG i;
    ULONG *pEnd;
    ESP_DATA *pT;

    for (i = 0x60; i != 0; ) {
        i--;
        Esp[i].Be_flg = 0;
    }
    pT = &Esp_data[32];
    do {
        pT--;
        pT->Id = 0xFF;
        pT->pData = NULL;
        pT->pMove = NULL;
    } while (pT > Esp_data);
    i = Cd_read(0x14, (ULONG *)0x801FC200, 0, "CORE00.ESP");
    pEnd = (ULONG *)0x801FC200;
    pEnd += i >> 2;
    if (i & 3) {
        pEnd++;
    }
    Esp_data_set0((UCHAR *)0x801FC200, pEnd - 1, 0);
}

/* clear the effect pool and register the room's effects and textures */
LONG Esp_init_R(void)
{
    ESP_DATA *pT;
    LONG i;
    LONG *pData;

    i = 0x60;
    do {
        i--;
        Esp[i].Be_flg = 0;
    } while (i != 0);
    pT = &Esp_data[32];
    do {
        pT--;
        pT->Id = 0xFF;
        pT->pData = NULL;
        pT->pMove = NULL;
    } while (pT > &Esp_data[ESP_DATA_ROOM]);
    pData = G.pRoom->pUnk4C;
    if (pData != NULL && *pData != -1) {
        Esp_data_set0((UCHAR *)pData, G.pRoom->pUnk50, 1);
        Esp_data_set1(G.pRoom->pUnk54);
    }
}

/* register up to 16 effect data blocks (id list pData, offsets read backwards from pOfs) */
ULONG Esp_data_set0(UCHAR *pData, ULONG *pOfs, ULONG Mode)
{
    ESP_DATA *pT;
    ULONG i;
    ULONG *p;
    ULONG c;
    ULONG n;

    i = 0;
    if ((UCHAR)Mode != 0) {
        pT = &Esp_data[ESP_DATA_ROOM];
    } else {
        pT = Esp_data;
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
        pT->pMove = (ULONG *)((UCHAR *)p + ((*p & 0xFFFF) * 2 + n + 2) * 4);
        pT++;
        i++;
    } while (i < 16);
    return (UCHAR)i;
}

/* upload the effect textures of the room to VRAM */
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

/* start an effect from the running effect pParent; the new one belongs to the same enemy */
ESP_WORK *Esp_call_child(ESP_WORK *pParent, ULONG Id, ULONG Dir_size, MATRIX *pM, LONG *pPos, SVECTOR *pSpd)
{
    SVECTOR sv;
    ESP_WORK *pEw;

    if (pPos != NULL) {
        if (pSpd != NULL) {
            sv.vx = pPos[0];
            sv.vy = pPos[1];
            sv.vz = pPos[2];
            pEw = Esp_call(Id, Dir_size, pM, &sv, pSpd);
        } else {
            sv.vx = pPos[0];
            sv.vy = pPos[1];
            sv.vz = pPos[2];
            pEw = Esp_call(Id, Dir_size, pM, &sv, 0);
        }
    } else {
        if (pSpd != NULL) {
            pEw = Esp_call(Id, Dir_size, pM, NULL, pSpd);
        } else {
            pEw = Esp_call(Id, Dir_size, pM, NULL, 0);
        }
    }
    if (pEw != ESP_NONE) {
        pEw->pEm = pParent->pEm;
        pEw->Mov_y = 0;
        Esp_pos_matrix(pEw);
        pEw->Old.vx = pEw->Pos.vx;
        pEw->Old.vy = pEw->Pos.vy;
        pEw->Old.vz = pEw->Pos.vz;
        pEw->Pos.pad = pParent->pEm->Pos_y;
        if (pEw->Attr & 0x10) {
            Esp_hit_ck(pEw);
        }
    }
    return pEw;
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Esp_call);

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Esp_kill);

/* set the four routine bytes (Rtn01 = Routine_0 | Routine_1 << 16, Rtn23 = Routine_2 | Unk03 << 16) of the live
 * effects of kind Id, only those attached to pM unless pM is NULL (RE2 Esp_ctrl sets Routine0 alone) */
void Esp_ctrl(ULONG Id, MATRIX *pM, ULONG Rtn01, ULONG Rtn23)
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

/* run every live effect: routine, movement, ground checks and sprite sequence (RE2 Esp_move) */
void Esp_task_main(void)
{
    ESP_WORK *pEw;

    for (pEw = pEsp_end - 1; pEw >= pEsp_top; pEw--) {
        if (!(G.Unk2468 & 0x10000000) || pEw->Id.B[0] == 0x15 || pEw->Id.B[0] == 0x1B) {
            if (pEw->Be_flg & 0x8000) {
                if (pEw->pLink != NULL && pEw->pLink->Be_flg == 0) {
                    pEw->Routine_1 = 0;
                    pEw->Routine_0 = 0;
                    pEw->Be_flg = 0;
                } else {
                    if (pEw->Routine_0 != 0) {
                        Esp_tblj[pEw->Routine_0](pEw);
                    }
                    pEw->Flg2 &= 0xFF00;
                    if (pEw->Be_flg & 2) {
                        Esp_speed_add(pEw);
                        Esp_pos_matrix(pEw);
                        Esp_hit_ck(pEw);
                    }
                    if (pEw->Be_flg & 1) {
                        Esp_anm_move(pEw);
                    }
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Esp_speed_add);

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Esp_pos_matrix);

/* water, floor and wall checks of an effect, result in Flg2 */
void Esp_hit_ck(ESP_WORK *pEw)
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
            y = Sca_get_ground(&pEw->Pos.vx, 2, 0x1000, 0);
            if (y < pEw->Pos.vy) {
                if (y < pEw->Pos.pad) {
                    pEw->Flg2 |= 0x80;
                } else {
                    pEw->Flg2 |= 0x40;
                }
            }
        }
        y = Sca_get_ground(&pEw->Pos.vx, 2, 0x2000, 0);
        if (y < pEw->Pos.vy) {
            if (y < pEw->Pos.pad) {
                pEw->Flg2 |= 0x20;
                if (pEw->Be_flg & 0x20) {
                    pEw->Spd_x -= pEw->Acc_x;
                    pEw->Pos.vx = pEw->Old.vx;
                    pEw->Mov_x -= pEw->Spd_x;
                }
            } else {
                pEw->Flg2 |= 0x10;
                pEw->Mov_y += y - (USHORT)pEw->Pos.vy;
                pEw->Pos.vy = y;
            }
        } else {
            pEw->Pos.pad = y;
        }
    }
}

/* step the sprite sequence; Time 0 ends the effect, 0xFE holds, 0xFF jumps */
void Esp_anm_move(ESP_WORK *pEw)
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

/* a free Esp[] slot (else one flagged 8, reusable), cleared; ESP_NONE if none */
ESP_WORK *Esp_work_get(void)
{
    ESP_WORK *pEw;

    pEw = &Esp[95];
    do {
        if (pEw->Be_flg == 0) {
            goto found;
        }
        pEw--;
    } while (pEw > &Esp[-1]); /* down to Esp[0] */
    pEw = &Esp[95];
    do {
        if (pEw->Be_flg & 8) {
            goto found;
        }
        pEw--;
    } while (pEw > &Esp[-1]); /* down to Esp[0] */
    return ESP_NONE;
found:
    Memclr(pEw, sizeof(ESP_WORK));
    return pEw;
}

/* the running effect with id Id (0xFFnn: any of kind nn) */
ESP_WORK *Esp_search(USHORT Id)
{
    ESP_WORK *pEw;

    pEw = &Esp[95];
    if ((Id & 0xFF00) == 0xFF00) {
        do {
            if (pEw->Be_flg != 0 && pEw->Id.B[0] == (UCHAR)Id) {
                return pEw;
            }
            pEw--;
        } while (pEw > &Esp[-1]); /* down to Esp[0] */
    } else {
        do {
            if (pEw->Be_flg != 0 && pEw->Id.W == (USHORT)Id) {
                return pEw;
            }
            pEw--;
        } while (pEw > &Esp[-1]); /* down to Esp[0] */
    }
}

/* Esp_tblj 0x00: free slot, no-op */
void Esp_00(void) {
}

/* Esp_tblj 0x01: on a floor/wall hit or in water play the Snd_no sound and end (Esp_end with Unk20) */
void Esp_01(ESP_WORK *pEw)
{
    if (pEw->Flg2 & 0x30) {
        if (pEw->Snd_no != 0) {
            Snd_se_stad(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), &pEw->Pos, 0, 0);
        }
        Esp_end(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
    if (pEw->Flg2 & 0x8) {
        if (pEw->Snd_no != 0) {
            Snd_se_stad(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), &pEw->Pos, 0, 0);
        }
        Esp_end(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
}

/* Esp_tblj 0x02: random sequence start, then no routine */
void Esp_02(ESP_WORK *pEw)
{
    Esp_seq_rnd_set(pEw, pEw->Unk18);
    pEw->Routine_0 = 0;
}

/* Esp_tblj 0x03: random start/speed, Size changed by Unk1C (Unk1C decays by Unk1E); ends on a hit, freed in water */
void Esp_03(ESP_WORK *pEw)
{
    switch (pEw->Routine_2) {
    case 0:
        Esp_seq_rnd_set(pEw, pEw->Unk18);
        Esp_spd_rnd_set(pEw, pEw->Unk1A, pEw->Unk1B);
        pEw->Routine_2++;
        break;
    case 1:
        pEw->Size += ((SHORT)pEw->Unk1C * (SHORT)pEw->Size) >> 12;
        pEw->Unk1C -= pEw->Unk1E;
        break;
    }
    if (pEw->Flg2 & 0x30) {
        if (pEw->Snd_no != 0) {
            Snd_se_stad(0x10000 | ((pEw->Snd_no >> 6) << 8) | (pEw->Snd_no & 0x3F), &pEw->Pos, 0, 0);
        }
        Esp_end(pEw, pEw->Unk20 >> 8, (UCHAR)pEw->Unk20);
    }
    if (pEw->Flg2 & 0x8) {
        pEw->Be_flg = 0;
    }
}

/* Esp_tblj 0x04: emitter, starts the child effect Unk18/Snd_no every Unk1A frames with a random spread (Routine_1 picks the variant) */
void Esp_04(ESP_WORK *pEw)
{
    VECTOR Unused;
    SVECTOR Rot;
    LONG d;
    ESP_WORK *p;
    ULONG r;

    d = 0;
    if (pEw->Unk03 == 0) {
        Esp_child_burst(pEw, (UCHAR)pEw->Unk1C);
        pEw->Unk03 = 1;
    }
    if (pEw->Routine_1 == 2) {
        if (pEw->Unk20 == 0) {
            pEw->UnkB6++;
            d = (pEw->UnkB6 * pEw->Unk22 * pEw->Size) >> 12;
            if ((ULONG)(pEw->Size - d) <= 0x300) {
                pEw->Be_flg = 0;
                return;
            }
        } else {
            pEw->Unk20--;
        }
    }
    if (pEw->UnkB4 == 0) {
        if (pEw->Unk1E == 0 || Rnd_tbl[Rnd()] % pEw->Unk1E == 0) {
            if (pEw->Routine_1 == 3) {
                p = Esp_call_child(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                  ((pEw->Size + Rnd_tbl[Rnd()] * 4 - d) << 16) |
                                      (USHORT)(pEw->Dir_y + ((signed char)Rnd() << pEw->Unk22)),
                                  pEw->pMat, pEw->Base, 0);
            } else {
                Rot.vx = pEw->Rot[0];
                Rot.vy = pEw->Rot[1];
                Rot.vz = pEw->Rot[2];
                r = Rnd();
                p = Esp_call_child(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                  ((pEw->Size + Rnd_tbl[r] * 4 - d) << 16) |
                                      (USHORT)(pEw->Dir_y + ((Rnd_tbl[Rnd()] - 0x80) >> pEw->Unk1B)),
                                  pEw->pMat, pEw->Base, &Rot);
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
                p->Spd_x += Rnd_tbl[Rnd()] % pEw->Unk22;
                p->Spd_y += Rnd_tbl[Rnd()] % pEw->Unk22;
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

/* Esp_tblj 0x05: Size shrinks to Unk20 (Routine_1 0) or grows back (1); splash in water */
void Esp_05(ESP_WORK *pEw)
{
    VECTOR Pos;

    if (pEw->Routine_1 == 1 && pEw->UnkB4 == 0) {
        pEw->UnkB4 = pEw->Size;
        pEw->Size = pEw->Unk20;
    }
    if (pEw->Unk1C != 0) {
        pEw->Unk1C--;
    } else {
        switch (pEw->Routine_1) {
        case 0:
            pEw->Size -= (pEw->Unk1E * pEw->Size) >> 12;
            if (pEw->Unk20 >= pEw->Size) {
                pEw->Be_flg = 0;
            }
            break;
        case 1:
            pEw->Size += (pEw->Unk1E * pEw->Size) >> 12;
            if (pEw->Size >= pEw->UnkB4) {
                pEw->Routine_0 = 0;
                pEw->Size = pEw->UnkB4;
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
        Esp_call_child(pEw, 0x1503, (pEw->Size << 16) | pEw->Dir_y, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
    }
}

/* Esp_tblj 0x06: damage area with Esp_at_tbl[Unk18] / [Snd_no]; a hit sets the flags Unk1A / Unk1B */
void Esp_06(ESP_WORK *pEw)
{
    HIT_REQ Req;

    if (pEw->Unk18 != 0xFF) {
        if (pEw->UnkB8 == 0) {
            Req.pPos = &pEw->Pos;
            Req.W = pEw->Unk1C;
            Req.H = pEw->Unk1E;
            Req.D = pEw->Unk20;
            Req.R = pEw->Dir_y;
            if (Pos_em_at_ck(&Req, Esp_at_tbl[pEw->Unk18]) < 0) {
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
        Req.R = pEw->Dir_y;
        if (Pos_em_at_ck(&Req, Esp_at_tbl[pEw->Snd_no]) & 0x20000000) {
            if (pEw->Unk1B != 0xFF) {
                Flg_on(D_800D1FC0, pEw->Unk1B);
            }
        }
    }
}

/* Esp_tblj 0x07: after Unk20 frames (or a hit) start the effect Unk18, then stop (Routine_1 0) or free (1) */
void Esp_07(ESP_WORK *pEw)
{
    ULONG Mode;
    ULONG Size;

    if (pEw->Flg2 & 0x30) {
        pEw->Unk20 = 0;
    }
    if (pEw->Unk20 == 0) {
        if (pEw->Unk18 != 0xFF) {
            if (pEw->Unk1A != 0xFF) {
                Mode = pEw->Unk1A;
            } else {
                Mode = pEw->UnkB3;
            }
            if (pEw->Unk1C != 0xFFFF) {
                Size = pEw->Unk1C;
            } else {
                Size = pEw->Size;
            }
            Esp_call_child(pEw, ((Mode & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                          (Size << 16) | (USHORT)(pEw->Dir_y + pEw->Unk1E), &GsIDMATRIX, &pEw->Pos.vx, 0);
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

/* Esp_tblj 0x08-0x0F: unused, no-op */
void Esp_08(void) {
}

void Esp_09(void) {
}

void Esp_0a(void) {
}

void Esp_0b(void) {
}

void Esp_0c(void) {
}

void Esp_0d(void) {
}

void Esp_0e(void) {
}

void Esp_0f(void) {
}

/* start the sprite sequence at a random one of n frames from Unk0F */
void Esp_seq_rnd_set(ESP_WORK *pEw, ULONG n)
{
    SSEQUENCE *pSeq;

    n &= 0xFF;
    if (n != 0) {
        pEw->Seq_no = pEw->Unk0F + Rnd_tbl[Rnd()] % n;
        pSeq = pEw->pSeq;
        pSeq += pEw->Seq_no;
        pEw->Timer = pSeq->Time;
    }
}

/* scale Acc/Spd by Size, Spd_x/Spd_y with a random spread of Spread_x/Spread_y */
void Esp_spd_rnd_set(ESP_WORK *pEw, ULONG Spread_x, ULONG Spread_y)
{
    pEw->Acc_x = (pEw->Acc_x * pEw->Size) >> 12;
    pEw->Acc_z = (pEw->Acc_z * pEw->Size) >> 12;
    if ((UCHAR)Spread_x != 0) {
        pEw->Spd_x = ((pEw->Spd_x + (signed char)Rnd() % (UCHAR)Spread_x) * pEw->Size) >> 12;
    }
    if ((UCHAR)Spread_y != 0) {
        pEw->Spd_y = ((pEw->Spd_y + (signed char)Rnd() % (UCHAR)Spread_y) * pEw->Size) >> 12;
    }
    pEw->Spd_z = (pEw->Spd_z * pEw->Size) >> 12;
}

/* end an effect by Routine_1: 2 starts the effect Kind/Type first, 0 frees it, 3 keeps only its sprite sequence */
void Esp_end(ESP_WORK *pEw, ULONG Kind, ULONG Type)
{
    switch (pEw->Routine_1) {
    case 2:
        Esp_call_child(pEw, ((pEw->UnkB3 & 0xF) << 24) | ((Type & 0xFF) << 8) | (Kind & 0xFF),
                      (pEw->Size << 16) | pEw->Dir_y, &GsIDMATRIX, &pEw->Pos.vx, 0);
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

/* start n child effects at once (Unk18/Snd_no) and run their first frames */
void Esp_child_burst(ESP_WORK *pEw, ULONG n)
{
    ESP_WORK *p;
    ULONG i;
    ULONG r;

    for (i = 0; i < n; i++) {
        if (pEw->UnkB4 == 0) {
            pEw->UnkB4 = pEw->Unk1A;
            r = Rnd();
            p = Esp_call_child(pEw, ((pEw->UnkB3 & 0xF) << 24) | (pEw->Snd_no << 8) | pEw->Unk18,
                                 ((pEw->Size + Rnd_tbl[r] * 4) << 16) |
                                     (USHORT)(pEw->Dir_y + ((Rnd_tbl[Rnd()] - 0x80) >> pEw->Unk1B)),
                                 pEw->pMat, pEw->Base, 0);
        } else {
            pEw->UnkB4--;
        }
        if (p != ESP_NONE) {
            p->UnkB6 = 1;
        }
        for (p = pEsp_end2 - 1; p >= pEsp_top2; p--) {
            if (p->UnkB6 != 0 && p->Id.W == *(USHORT *)&pEw->Unk18) {
                Esp_tblj[p->Routine_0](p);
                Esp_speed_add(p);
                Esp_pos_matrix(p);
                Esp_anm_move(p);
            }
        }
    }
    for (p = pEsp_end2 - 1; p >= pEsp_top2; p--) {
        if (p->Id.W == *(USHORT *)&pEw->Unk18) {
            p->UnkB6 = 0;
        }
    }
    pEw->UnkB4 = 0;
}

/* Esp_tblj 0x10: bouncing debris: slows on floor/wall hits with a sound (shell case sound from the player's weapon), splash in water */
void Esp_10(ESP_WORK *pEw)
{
    VECTOR Pos;
    UCHAR Hit;
    UCHAR n;

    switch (pEw->Routine_1) {
    case 0:
        Esp_seq_rnd_set(pEw, 3);
        pEw->Spd_x += Rnd_tbl[Rnd()] & 0xF;
        pEw->Spd_y += Rnd_tbl[Rnd()] & 0xF;
        pEw->Routine_1++;
        break;
    case 1:
        if (pEw->Flg2 & 8) {
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy;
            Pos.vz = pEw->Pos.vz;
            Esp_call_child(pEw, 0xF1C, pEw->Dir_y | 0x8000000, &GsIDMATRIX, &Pos.vx, 0);
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
            pEw->Spd_z = (Rnd_tbl[Rnd()] & 3) * 3;
            pEw->UnkB4++;
            Hit = 1;
        }
        if (pEw->Flg2 & 0x20) {
            if (pEw->UnkB6 >= 2) {
                pEw->Be_flg = 0;
                break;
            }
            pEw->Spd_x *= -1;
            pEw->Spd_z = (Rnd_tbl[Rnd()] & 3) * 3;
            pEw->UnkB6++;
            Hit = 1;
        }
        if (Hit) {
            if (pEw->pMat == &G.Player.pSin_parts_ptr[4].Workm) {
                n = 2;
                if (pEw->Id.B[1] == 2) {
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

/* Esp_tblj 0x11: random size on the first frame (Unk18/Snd_no spread), freed on a floor/wall hit */
void Esp_11(ESP_WORK *pEw)
{
    if (pEw->Routine_1 == 0) {
        pEw->Unk08 += (signed char)Rnd_tbl[Rnd()] * pEw->Unk18;
        pEw->Unk0A += (signed char)Rnd_tbl[Rnd()] * pEw->Snd_no;
        pEw->Routine_1++;
    }
    if (pEw->Flg2 & 0x30) {
        pEw->Be_flg = 0;
    }
}

/* Esp_tblj 0x12: burning projectile: damage (Esp_at_tbl[30]) every other frame, trail sparks, splash in water */
void Esp_12(ESP_WORK *pEw)
{
    VECTOR Pos;
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    if (pEw->Routine_1 == 0) {
        if (pEw->Unk1A != 0) {
            pEw->Spd_x = ((pEw->Spd_x + (signed char)Rnd() % pEw->Unk1A) * pEw->Size) >> 12;
        }
        if (pEw->Unk1B != 0) {
            pEw->Spd_y = ((pEw->Spd_y + (signed char)Rnd() % pEw->Unk1B) * pEw->Size) >> 12;
        }
        pEw->Routine_1++;
        pEw->Spd_z = (pEw->Spd_z * pEw->Size) >> 12;
    }
    if (!(pEw->UnkBA & 1)) {
        Req.pPos = &pEw->Pos;
        Req.W = 0;
        Req.H = 0;
        Req.D = 0;
        Req.R = pEw->Dir_y;
        Hit |= Pos_em_at_ck(&Req, Esp_at_tbl[30]);
        if (Hit) {
            pEw->Be_flg = 0;
        }
    }
    if (pEw->Flg2 & 8) {
        Pos.vx = pEw->Pos.vx;
        Pos.vy = pEw->Pos.vy;
        Pos.vz = pEw->Pos.vz;
        Esp_call_child(pEw, 0x1503, (pEw->Size << 16) | pEw->Dir_y, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
        return;
    }
    if (!(pEw->UnkBA & 1)) {
        Esp_call_child(pEw, 0x3003603,
                      (((pEw->Size >> 1) - (pEw->Size >> 2) + ((signed char)Rnd() << 3)) << 16) | pEw->Dir_y,
                      &GsIDMATRIX, &pEw->Pos.vx, 0);
    }
    if (pEw->Flg2 & 0x30) {
        if (pEw->Unk22 == 0) {
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy + 400;
            Pos.vz = pEw->Pos.vz;
            Esp_call_child(pEw, 0x205, (pEw->Size << 16) | pEw->Dir_y, &GsIDMATRIX, &Pos.vx, 0);
        }
        pEw->Be_flg = 0;
    }
    pEw->UnkBA++;
}

/* Esp_tblj 0x13, 0x14: unused, no-op */
void Esp_13(void) {
}

void Esp_14(void) {
}

/* Esp_tblj 0x15: grenade round (Routine_1 = round type): hits like the player's weapon, explodes with the round's effects on a hit */
void Esp_15(ESP_WORK *pEw)
{
    ESP_WORK *pNew;
    LONG Ret;
    VECTOR Pos;
    SVECTOR Rot;
    VECTOR Avg;
    HIT_REQ Req;

    if (pEw->Dir_y != 0) {
        pEw->UnkBA = pEw->Dir_y;
        pEw->Dir_y = 0;
    }
    Ret = pEw->Flg2 & 0x30;
    Ret |= (LONG)Weapon_at_ck(&pEw->Workm2, Pl_wep_data[0], pEw->Routine_1 + 6);
    Ret |= Weapon_om_at_ck(&pEw->Workm2, Pl_wep_data[0], pEw->Routine_1 + 6);
    if (++pEw->UnkB4 >= 15) {
        Ret |= 1;
    }
    Pos.vx = (signed char)Rnd_tbl[Rnd()] >> 1;
    Pos.vy = 0;
    Pos.vz = (signed char)Rnd_tbl[Rnd()] >> 1;
    Esp_call_child(pEw, 0xC03, 0x10000000, &pEw->Workm2, &Pos.vx, 0);
    if (pEw->Flg2 & 8) {
        Pos.vx = pEw->Pos.vx;
        Pos.vy = pEw->Pos.vy;
        Pos.vz = pEw->Pos.vz;
        Req.pPos = &Pos;
        Req.W = 0;
        Req.H = 0;
        Req.D = 0;
        Req.R = pEw->UnkBA;
        Pos_em_at_ck(&Req, Esp_at_tbl[15]);
        Pos_om_at_ck(&Req, Esp_at_tbl[15]);
        Ret |= 1;
        if (Water_ck(pEw->Pos.vx, pEw->Pos.vz) != 1) {
            Esp_call_child(pEw, 0x71C, (USHORT)pEw->UnkBA | 0x0C000000, &GsIDMATRIX, &Pos.vx, 0);
            Esp_call_child(pEw, 0x91C, (USHORT)pEw->UnkBA | 0x15000000, &GsIDMATRIX, &Pos.vx, 0);
            Rot.vx = 0x400;
            Rot.vy = 0;
            Rot.vz = 0;
            Esp_call_child(pEw, 0x20E, (USHORT)pEw->UnkBA | 0x20000000, &GsIDMATRIX, &Pos.vx, &Rot);
            Rot.vx = 0x400;
            Rot.vy = 0;
            Rot.vz = 0x400;
            Esp_call_child(pEw, 0x30E, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Pos.vx, &Rot);
            Avg.vx = (pEw->Pos.vx + pEw->Old.vx) / 2;
            Avg.vy = (pEw->Pos.vy + pEw->Old.vy) / 2;
            Avg.vz = (pEw->Pos.vz + pEw->Old.vz) / 2;
            switch (pEw->Routine_1) {
            case 0:
                Esp_call_child(pEw, 0x3000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            case 1:
                Esp_call_child(pEw, 0x3000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            case 2:
                Esp_call_child(pEw, 0x2000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            case 3:
                Esp_call_child(pEw, 0xA03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
                break;
            }
            Snd_se_stad(0x1022A, &pEw->Pos, 0, 0);
            pEw->Be_flg = 0;
            return;
        }
        Esp_call_child(pEw, 0x91C, (USHORT)pEw->UnkBA | 0x15000000, &GsIDMATRIX, &Pos.vx, 0);
        Rot.vx = 0x400;
        Rot.vy = 0;
        Rot.vz = 0;
        Esp_call_child(pEw, 0x20E, (USHORT)pEw->UnkBA | 0x20000000, &GsIDMATRIX, &Pos.vx, &Rot);
        Rot.vx = 0x400;
        Rot.vy = 0;
        Rot.vz = 0x400;
        Esp_call_child(pEw, 0x30E, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Pos.vx, &Rot);
    }
    Avg.vx = (pEw->Pos.vx + pEw->Old.vx) / 2;
    Avg.vy = (pEw->Pos.vy + pEw->Old.vy) / 2;
    Avg.vz = (pEw->Pos.vz + pEw->Old.vz) / 2;
    if (Ret) {
        switch (pEw->Routine_1) {
        case 0:
            Snd_se_stad(0x1010E, &pEw->Pos, 0, 0);
            Esp_call_child(pEw, 0x203, (USHORT)pEw->UnkBA | 0x09000000, &GsIDMATRIX, &Avg.vx, 0);
            Req.pPos = &pEw->Pos;
            Req.W = 0;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEw->UnkBA;
            Pos_em_at_ck(&Req, Esp_at_tbl[15]);
            Pos_om_at_ck(&Req, Esp_at_tbl[15]);
            break;
        case 1:
            Snd_se_stad(0x1010F, &pEw->Pos, 0, 0);
            Esp_call_child(pEw, 0x3000903, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Avg.vx, 0);
            Esp_call_child(pEw, 0x3000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy;
            Pos.vz = pEw->Pos.vz;
            Esp_call_child(pEw, 0x1000803, (USHORT)pEw->UnkBA | 0x30000000, &GsIDMATRIX, &Pos.vx, 0);
            if (pEw->Flg2 & 0x20) {
                Esp_call_child(pEw, 0x3000D03, (USHORT)(pEw->UnkBA - 0xFA) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
                Esp_call_child(pEw, 0x3000D03, (USHORT)(pEw->UnkBA + 0xFA) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            } else {
                Esp_call_child(pEw, 0x3000D03, (USHORT)pEw->UnkBA | 0x32000000, &GsIDMATRIX, &pEw->Old.vx, 0);
                Esp_call_child(pEw, 0x3000D03, (USHORT)(pEw->UnkBA - 0x15E) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
                Esp_call_child(pEw, 0x3000D03, (USHORT)(pEw->UnkBA + 0x15E) | 0x30000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            }
            Req.pPos = &pEw->Pos;
            Req.W = 0;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEw->UnkBA;
            Pos_em_at_ck(&Req, Esp_at_tbl[16]);
            Pos_om_at_ck(&Req, Esp_at_tbl[16]);
            break;
        case 2:
            Snd_se_stad(0x10110, &pEw->Pos, 0, 0);
            Esp_call_child(pEw, 0x2000903, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &Avg.vx, 0);
            Esp_call_child(pEw, 0x2000A03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy - 100;
            Pos.vz = pEw->Pos.vz;
            pNew = Esp_call_child(pEw, 0x3000E00, (USHORT)pEw->UnkBA | 0x24000000, &GsIDMATRIX, &Pos.vx, 0);
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
            Esp_call_child(pEw, 0x3001000, (USHORT)pEw->UnkBA | 0x24000000, &GsIDMATRIX, &Pos.vx, 0);
            break;
        case 3:
            Snd_se_stad(0x10111, &pEw->Pos, 0, 0);
            Esp_call_child(pEw, 0x2203, (USHORT)pEw->UnkBA | 0x34000000, &GsIDMATRIX, &Avg.vx, 0);
            Esp_call_child(pEw, 0xA03, (USHORT)pEw->UnkBA | 0x38000000, &GsIDMATRIX, &Avg.vx, 0);
            pNew = Esp_call_child(pEw, 0x2003, (USHORT)pEw->UnkBA | 0x10000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            if (pNew != ESP_NONE) {
                if (pEw->Flg2 & 0x20) {
                    pNew->UnkB6 = 1;
                } else {
                    pNew->UnkB6 = 0;
                }
            }
            Esp_call_child(pEw, 0xB03, (USHORT)pEw->UnkBA | 0x28000000, &GsIDMATRIX, &pEw->Old.vx, 0);
            break;
        }
        pEw->Be_flg = 0;
    }
}

/* Esp_tblj 0x16: rocket: flies with a smoke trail, explodes on a hit or at the room bounds */
void Esp_16(ESP_WORK *pEw)
{
    VECTOR Spd;
    SVECTOR Rot;
    VECTOR Pos;
    LONG Hit;
    ESP_WORK *p;

    Hit = 0;
    switch (pEw->Routine_2) {
    case 0:
        if (pEw->Dir_y != 0) {
            pEw->UnkBA = pEw->Dir_y;
            pEw->Dir_y = 0;
        }
        if (pEw->Routine_1 == 0) {
            Rot.vx = 0;
            Rot.vy = 0;
            Rot.vz = 0x400;
            Esp_call_child(pEw, 0x3000D02, 0x18000000, &pEw->Workm2, NULL, &Rot);
            Rot.vx = 0;
            Rot.vy = 0x400;
            Rot.vz = 0x400;
            Esp_call_child(pEw, 0x3000D02, 0x18000000, &pEw->Workm2, NULL, &Rot);
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
                Esp_kill(0xD02, &pEw->Workm2);
            }
            if (--pEw->UnkB8 == 1) {
                pEw->Be_flg = 0;
            }
            break;
        }
        if (pEw->Mov_y >= 30000) {
            p = Esp_call(((pEw->UnkB3 & 0xF) << 24) | (pEw->Id.B[1] << 8) | pEw->Id.B[0],
                              (pEw->Size << 16) | pEw->Dir_y, &pEw->Workm2, NULL, 0);
            Esp_speed_add(p);
            Esp_pos_matrix(p);
            pEw->UnkB8 = 10;
        }
        if (pEw->Flg2 & 0x30) {
            Hit |= 1;
        }
        Pos.vx = (pEw->Pos.vx + pEw->Old.vx) / 2;
        Pos.vy = (pEw->Pos.vy + pEw->Old.vy) / 2;
        Pos.vz = (pEw->Pos.vz + pEw->Old.vz) / 2;
        Hit |= (LONG)Weapon_at_ck(&pEw->Workm2, Pl_wep_data[0], 10);
        Hit |= Weapon_om_at_ck(&pEw->Workm2, Pl_wep_data[0], 10);
        Spd.vx = (signed char)Rnd_tbl[Rnd()] >> 1;
        Spd.vy = 500;
        Spd.vz = (signed char)Rnd_tbl[Rnd()] >> 1;
        Esp_call_child(pEw, 0xC03, 0x18000000, &pEw->Workm2, &Spd.vx, 0);
        if (Hit) {
            Esp_kill(0xD02, &pEw->Workm2);
            Snd_se_stad(0x1010A, &pEw->Pos, 0, 0);
            Esp_call_child(pEw, 0x203, (USHORT)pEw->UnkBA | 0xD000000, &GsIDMATRIX, &Pos.vx, 0);
            pEw->Be_flg = 0;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/em_sub", Esp_17);

/* Esp_tblj 0x18: two flames at Base, then smoke every frame */
void Esp_18(ESP_WORK *pEw)
{
    VECTOR Pos;
    SVECTOR Rot;

    switch (pEw->Routine_1) {
    case 0:
        pEw->Routine_1++;
    case 1:
        Pos.vx = pEw->Base[0];
        Pos.vy = pEw->Base[1] - 100;
        Pos.vz = pEw->Base[2];
        Rot.vx = 0;
        Rot.vy = 0;
        Rot.vz = 0;
        Esp_call_child(pEw, 0x805, ((pEw->Size >> 1) << 16) | pEw->Dir_y, pEw->pMat, &Pos.vx, &Rot);
        Rot.vx = 0;
        Rot.vy = 0x400;
        Rot.vz = 0;
        Esp_call_child(pEw, 0x805, ((pEw->Size >> 1) << 16) | pEw->Dir_y, pEw->pMat, &Pos.vx, &Rot);
        pEw->Routine_1++;
        break;
    default:
        Pos.vx = (signed char)Rnd_tbl[Rnd()] >> 1;
        Pos.vy = -150;
        Pos.vz = (signed char)Rnd_tbl[Rnd()] >> 1;
        Esp_call_child(pEw, 0xC03, ((pEw->Size >> 1) << 16) | pEw->Dir_y, pEw->pMat, &Pos.vx, 0);
        break;
    }
}

/* Esp_tblj 0x19: explosion: blasts on the first frame, a last one (0x1000803) on the third */
void Esp_19(ESP_WORK *pEw)
{
    VECTOR Pos;
    VECTOR Dummy;

    switch (pEw->Routine_1) {
    case 0:
        Esp_call_child(pEw, 0x3000903, (pEw->Size << 18) | pEw->Dir_y, &GsIDMATRIX, &pEw->Pos.vx, 0);
        Esp_call_child(pEw, 0x3000A03, ((pEw->Size * 5) << 16) | pEw->Dir_y, &GsIDMATRIX, &pEw->Pos.vx, 0);
        break;
    case 2:
        Pos.vx = pEw->Pos.vx;
        Pos.vy = pEw->Pos.vy;
        Pos.vz = pEw->Pos.vz;
        Esp_call_child(pEw, 0x1000803, ((((ULONG)pEw->Size * 17) >> 2) << 16) | pEw->Dir_y, &GsIDMATRIX, &Pos.vx, 0);
        pEw->Be_flg = 0;
        return;
    }
    pEw->Routine_1++;
}

/* Esp_tblj 0x1A: while Routine_1 is 1, grow the sprite by Routine_2 / Unk03 (signed) per frame */
void Esp_1a(ESP_WORK *pEw)
{
    if (pEw->Routine_1 == 1) {
        pEw->Unk08 += (signed char)pEw->Routine_2 << 4;
        pEw->Unk0A += (signed char)pEw->Unk03 << 4;
    }
}

/* Esp_tblj 0x1B: like Esp_03, plus damage (Esp_at_tbl[19]) on frames 16..20 */
void Esp_1b(ESP_WORK *pEw)
{
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    switch (pEw->Routine_2) {
    case 0:
        Esp_seq_rnd_set(pEw, pEw->Unk18);
        Esp_spd_rnd_set(pEw, pEw->Unk1A, pEw->Unk1B);
        pEw->Routine_2++;
        break;
    case 1:
        pEw->Size += ((SHORT)pEw->Unk1C * (SHORT)pEw->Size) >> 12;
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
            Req.R = pEw->Dir_y;
            Hit |= Pos_em_at_ck(&Req, Esp_at_tbl[19]);
            Hit |= Pos_om_at_ck(&Req, Esp_at_tbl[19]);
            if (Hit) {
                pEw->Be_flg = 0;
            }
        }
    }
}

/* Esp_tblj 0x1C: three 0x2103 children every other frame for 5 frames */
void Esp_1c(ESP_WORK *pEw)
{
    SHORT Ofs[3] = { -200, 0, 200 };
    SHORT Spd[3] = { -2, 0, -2 };
    UCHAR i;
    ESP_WORK *p;

    if (++pEw->UnkB4 & 1) {
        for (i = 0; i < 3; i++) {
            p = Esp_call_child(pEw, 0x2103,
                              ((0x2800 - pEw->UnkB4 * 1000) << 16) |
                                  (USHORT)(pEw->Dir_y + (signed char)Rnd_tbl[Rnd()] + Ofs[i]),
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

/* Esp_tblj 0x1D: like Esp_03 on x only; starts 0x505 on a wall hit, 0x405 on frames 1 and 7 */
void Esp_1d(ESP_WORK *pEw)
{
    VECTOR Unused;
    VECTOR Pos;
    ESP_WORK *p;

    switch (pEw->Routine_1) {
    case 0:
        Esp_seq_rnd_set(pEw, pEw->Unk18);
        Esp_spd_rnd_set(pEw, pEw->Unk1A, 0);
        pEw->Routine_1++;
        break;
    case 1:
        pEw->Size += ((SHORT)pEw->Unk1C * (SHORT)pEw->Size) >> 12;
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
        p = Esp_call_child(pEw, 0x505, ((pEw->Size - ((signed char)Rnd() << 4)) << 16) | pEw->Dir_y,
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
            p = Esp_call_child(pEw, 0x405, ((pEw->Size - ((signed char)Rnd() << 4)) << 16) | pEw->Dir_y,
                              &GsIDMATRIX, &Pos.vx, 0);
            if (p != ESP_NONE) {
                p->Spd_x = (Rnd_tbl[Rnd()] & 0x1F) + 0x50;
                if (pEw->UnkB4 == 7) {
                    p->UnkBA = 1;
                }
            }
        }
        pEw->UnkB4++;
    }
}

/* Esp_tblj 0x1E: shrinking flame with damage (Esp_at_tbl[17]); smoke when it lands or hits water */
void Esp_1e(ESP_WORK *pEw)
{
    VECTOR Pos;
    HIT_REQ Req;
    UCHAR Hit;

    Hit = 0;
    switch (pEw->Routine_1) {
    case 0:
        if (pEw->Unk18 == 0) {
            pEw->UnkB4 = (Rnd_tbl[Rnd()] & 0xF) | 0x10;
            pEw->UnkB6 = (((Rnd_tbl[Rnd()] & 0xF) + 0x10) * pEw->Size) >> 11;
        } else {
            pEw->UnkB4 = (Rnd_tbl[Rnd()] & 0xF) + 0x18;
            pEw->UnkB6 = (((Rnd_tbl[Rnd()] & 0xF) + 0x18) * pEw->Size) >> 11;
            if (!(pEw->UnkBA & 0x80)) {
                pEw->Be_flg &= 0xFFFD;
            }
        }
        pEw->UnkB8 = 0;
        pEw->Routine_1++;
    case 1:
        pEw->Unk0A -= pEw->UnkB4;
        pEw->Size -= pEw->UnkB6;
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
                    Req.R = pEw->Dir_y;
                    Hit |= Pos_em_at_ck(&Req, Esp_at_tbl[17]);
                    Hit |= Pos_om_at_ck(&Req, Esp_at_tbl[17]);
                    if (Hit) {
                        pEw->UnkBA = 0;
                        pEw->Unk0A += pEw->UnkB4 << 5;
                        pEw->Size += pEw->UnkB6 << 5;
                    }
                }
            }
        } else if (pEw->Size <= 0x2000) {
            pEw->Be_flg |= 2;
        }
        if ((USHORT)pEw->Unk0A <= 0x500 || pEw->Size <= 0x300) {
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
    Esp_call_child(pEw, 0x1503, ((pEw->Size >> 2) << 16) | pEw->Dir_y, pEw->pMat, &Pos.vx, 0);
    pEw->Be_flg = 0;
}

/* Esp_tblj 0x1F: two 0x3000F00 children and damage (Esp_at_tbl[18]) per frame for 3 frames */
void Esp_1f(ESP_WORK *pEw)
{
    HIT_REQ Req;
    VECTOR Pos;
    ESP_WORK *pNew;
    ULONG i;

    for (i = 0; i < 2; i++) {
        pNew = Esp_call_child(pEw, 0x3000F00, (pEw->Size << 16) | (USHORT)(pEw->Dir_y + (signed char)Rnd() * 4),
                             pEw->pMat, pEw->Base, 0);
        if (pNew != ESP_NONE) {
            pNew->Spd_x = (Rnd_tbl[Rnd()] & 0x3F) + 0x82;
            pNew->Spd_y = (Rnd_tbl[Rnd()] & 0x7F) - 100;
        }
    }
    Req.pPos = &pEw->Pos;
    Req.W = 0;
    Req.H = 0;
    Req.D = 0;
    Req.R = pEw->Dir_y;
    Pos_em_at_ck(&Req, Esp_at_tbl[18]);
    Pos_om_at_ck(&Req, Esp_at_tbl[18]);
    if (++pEw->UnkB4 >= 3) {
        if (pEw->UnkB6 != 0) {
            Pos.vx = pEw->Pos.vx;
            Pos.vy = pEw->Pos.vy;
            Pos.vz = pEw->Pos.vz;
            Esp_call_child(pEw, 0x2000E03, pEw->Dir_y | 0x20000000, &GsIDMATRIX, &Pos.vx, 0);
        }
        pEw->Be_flg = 0;
    }
}

/* Esp_tblj 0x20: flash: 0x108, a burst of 0x9 sparks and the 0x300 light, then 0x200 after 9 frames */
void Esp_20(ESP_WORK *pEw)
{
    VECTOR Pos;

    switch (pEw->Routine_1) {
    case 0:
        Esp_call_child(pEw, 0x108, pEw->Dir_y | 0x10000000, pEw->pMat, pEw->Base, 0);
        pEw->Routine_1++;
        break;
    case 1:
        Esp_call_child(pEw, 9, (USHORT)(pEw->Dir_y + (signed char)Rnd_tbl[Rnd()] * 7 + 0x800) | 0x9000000,
                      pEw->pMat, pEw->Base, 0);
        Esp_call_child(pEw, 9, (USHORT)(pEw->Dir_y + (signed char)Rnd_tbl[Rnd()] * 7 + 0x800) | 0xB000000,
                      pEw->pMat, pEw->Base, 0);
        Esp_call_child(pEw, 0x109, (USHORT)(pEw->Dir_y + (signed char)Rnd_tbl[Rnd()] * 4 + 0x800) | 0xA000000,
                      pEw->pMat, pEw->Base, 0);
        Esp_call_child(pEw, 0x209, (USHORT)(pEw->Dir_y + (signed char)Rnd_tbl[Rnd()] * 2 + 0x800) | 0x7000000,
                      pEw->pMat, pEw->Base, 0);
        Esp_call_child(pEw, 0x309, (USHORT)(pEw->Dir_y + (signed char)Rnd_tbl[Rnd()] * 6 + 0x800) | 0x8000000,
                      pEw->pMat, pEw->Base, 0);
        Esp_call_child(pEw, 9, (USHORT)(pEw->Dir_y + (signed char)Rnd_tbl[Rnd()] * 7) | 0xB000000,
                      pEw->pMat, pEw->Base, 0);
        Pos.vx = pEw->Base[0];
        Pos.vy = pEw->Base[1] + 200;
        Pos.vz = pEw->Base[2];
        Esp_call_child(pEw, ((pEw->UnkB3 & 0xF) << 24) | 0x300, 0x10000000, pEw->pMat, &Pos.vx, 0);
        pEw->Routine_1++;
        break;
    case 2:
        if (++pEw->UnkB4 >= 9) {
            pEw->Routine_1++;
        }
        break;
    case 3:
        Pos.vx = pEw->Base[0];
        Pos.vy = pEw->Base[1] + 200;
        Pos.vz = pEw->Base[2];
        Esp_call_child(pEw, ((pEw->UnkB3 & 0xF) << 24) | 0x200, 0x10000000, pEw->pMat, &Pos.vx, 0);
        pEw->Be_flg = 0;
        break;
    }
}
