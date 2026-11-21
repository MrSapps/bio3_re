#include "sub.h"
#include "pcstr.h"
#include "snd.h"
#include "em_sub.h"
#include "cd.h"

#define EM_P(pEm, ofs, type) (*(type *)((UCHAR *)(pEm) + (ofs)))  /* ENEMY_WORK fields past the named ones */
#define FLD(p, ofs, type) (*(type *)((UCHAR *)(p) + (ofs)))  /* unnamed struct fields */

/* player state kept by func_800791F0 (0x800C7048) */
typedef struct tagPL_SAVE {
    USHORT Cdir_y;                     /* 0x0 */
    SHORT Pos_x;                       /* 0x2 */
    SHORT Pos_y;                       /* 0x4 */
    SHORT Pos_z;                       /* 0x6 */
    USHORT Life;                       /* 0x8 */
    UCHAR Pl_id;                       /* 0xA */
    UCHAR Poison;                      /* 0xB 0x80 | poison timer */
    UCHAR Save[0x908];                 /* 0xC copy of G.Card_head.. */
} PL_SAVE;
extern PL_SAVE D_800C7048;

/* two-bone joint lengths (func_80079374) */
typedef struct tagMY_JNT {
    SVECTOR a;
    SVECTOR b;
    SHORT c[2];
} MY_JNT;

extern MATRIX D_800E1200[];
extern MATRIX D_800E1220;
extern MATRIX D_800E1260;
extern MATRIX D_800E1280;

void func_80078FE8(void);
void func_8007906C(void);
void func_800792FC(void);
void func_800790F0(ULONG *pAdr);
LONG func_8004D720(VECTOR *pV, LONG a1, LONG a2, LONG a3);
void HMatrix(MATRIX *a, MATRIX *b, MATRIX *c, LONG r);
void MulLMatrix(MATRIX *a, MATRIX *b, MATRIX *c);
void func_80079374(MATRIX *a, MATRIX *b, LONG *t, MY_JNT *j);
void func_80079CC0(ENEMY_WORK *pEm, LONG h);
LONG func_80079690(ENEMY_WORK *pEm, UCHAR *pKan, UCHAR *pSeq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, LONG rate);
void Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
LONG Cd_read_s(LONG Fno, LONG a1, ULONG *pAdr, LONG a3, char *pName);
void func_8007836C(LONG no, ULONG *pAdr);
void func_800783BC(ULONG no, ULONG size, ULONG *a2);

/* G.Unk2484 as the RE2 Ctpage/Cclut bytes (next free texture page x / clut row); a byte
 * pointer into G.Unk2484 does not match, the original addresses them as G fields (as cd.c) */
typedef struct {
    UCHAR pad[0x2484];
    UCHAR Ctpage;                      /* 0x2484 */
    UCHAR Cclut;                       /* 0x2485 */
} G_CTPAGE;
#define G_TP_X (((G_CTPAGE *)&G)->Ctpage)
#define G_CLUT_Y (((G_CTPAGE *)&G)->Cclut)

void Mapping_tim(ULONG *pAdr)
{
    TIM_IMAGE tim;
    TIM_IMAGE *p;
    LONG x;
    RECT *r;

    OpenTIM(pAdr);
    p = (TIM_IMAGE *)ReadTIM((u_long *)&tim);
    r = p->prect;
    x = G_TP_X << 6;
    if (G_TP_X >= 16) {
        x -= 0x400;
    }
    r->x = x;
    p->prect->y = (G_TP_X >= 16) << 8;
    LoadImage(p->prect, p->paddr);
    G_TP_X += (p->prect->w + 0x3F) / 64;
    if (p->caddr) {
        p->crect->y = G_CLUT_Y + 0x1E0;
        LoadImage(p->crect, p->caddr);
        G_CLUT_Y += p->crect->h;
    }
    DrawSync(0);
}

void SetSpadStack(void)
{
    G.System_flg |= SYS_SPAD_STACK;
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\tmove $29, %1" : : "r"(&pStack_bak), "r"(0x1f8003fc) : "$8");
}

void ResetSpadStack(void)
{
    G.System_flg &= ~SYS_SPAD_STACK;
    __asm__ volatile("move $29, %0" : : "r"(pStack_bak));
}

void Vram_clr(USHORT x, USHORT y, USHORT w, USHORT h)
{
    RECT Rect;

    Rect.x = x;
    Rect.y = y;
    Rect.w = w;
    Rect.h = h;
    ClearImage(&Rect, 0, 0, 0);
}

void Rot_vector(SHORT dir, SVECTOR *p, SVECTOR *q)
{
    MATRIX *pMat;

    Rot_vector_Svec.vy = dir;
    pMat = &Tmp_matrix[0];
    RotMatrix(&Rot_vector_Svec, pMat);
    ApplyMatrixSV(pMat, p, q);
}

void Rot_vector_super(SHORT dir, MATRIX *pM, SVECTOR *p, SVECTOR *q)
{
    MATRIX *pMat;

    Rot_vector_super_Svec.vy = dir;
    pMat = &Tmp_matrix[1];
    RotMatrix(&Rot_vector_super_Svec, pMat);
    MulMatrix2(pM, pMat);
    ApplyMatrixSV(pMat, p, q);
}

void Rot_add_matrix(LONG *pPos, SHORT *pDir, MATRIX *pM, SVECTOR *Add_dir)
{
    MATRIX Mat;
    MATRIX Mat2;
    SVECTOR Svec;
    VECTOR Vec;
    LONG ang;
    int m02;
    int m12;

    int ySq;
    int x;
    int y;

    Mat.t[0] = pPos[0];
    Mat.t[1] = pPos[1];
    Mat.t[2] = pPos[2];
    RotMatrix(Add_dir, &Mat);
    MulMatrix2(&Mat, pM);
    MatrixNormal(pM, pM);

    m02 = pM->m[0][2];
    m12 = pM->m[1][2];
    x = pM->m[2][2];
    y = m02 * m02;
    y += m12 * m12;
    y += x * x;
    y = (m02 << 0xc) / (y / 4096);

    ang = ratan2(-m12, x);
    *pDir = ang;

    ySq = y * y;
    if (ySq < 0) {
        ySq += 0xfff;
    }
    ang = SquareRoot12(0x1000 - (ySq >> 0xc));
    ang = ratan2(y, ang);
    pDir[1] = ang;
    pDir[2] = 0;

    Mat.t[0] = 0;
    Mat.t[1] = 0;
    Mat.t[2] = 0;
    RotMatrix((SVECTOR *)pDir, &Mat);
    TransposeMatrix(&Mat, &Mat2);

    Svec.vx = pM->m[0][0];
    Svec.vy = pM->m[1][0];
    Svec.vz = pM->m[2][0];
    ApplyMatrix(&Mat2, &Svec, &Vec);
    ang = ratan2(Vec.vy, Vec.vx);
    pDir[2] = ang;
}

void Flg_on(ULONG *p, ULONG x)
{
    p[x >> 5] |= 0x80000000U >> (x & 0x1f);
}

void Flg_off(ULONG *p, ULONG x)
{
    p[x >> 5] &= ~(0x80000000U >> (x & 0x1f));
}

ULONG Flg_ck(ULONG *p, ULONG x)
{
    return p[x >> 5] & (0x80000000U >> (x & 0x1f));
}

void Set_view(LONG *pVp, LONG *pVr)
{
    MATRIX Mat;
    LONG F1;
    LONG F2;
    LONG Dx;
    LONG Dy;
    LONG Dz;
    VECTOR V;

    GsWSMATRIX = GsIDMATRIX;

    Dx = pVr[0] - pVp[0];
    Dy = pVr[1] - pVp[1];
    Dz = pVr[2] - pVp[2];

    F1 = SquareRoot0(Dx * Dx + Dy * Dy + Dz * Dz);
    Dy = (Dy << 12) / F1;
    F2 = SquareRoot0(Dx * Dx + Dz * Dz);

    Mat = GsIDMATRIX;
    Mat.m[2][2] = Mat.m[1][1] = (F2 << 12) / F1;
    Mat.m[1][2] = -Dy;
    Mat.m[2][1] = Dy;
    MulMatrix(&GsWSMATRIX, &Mat);

    if (F2 != 0) {
        Mat = GsIDMATRIX;
        Mat.m[2][2] = Mat.m[0][0] = (Dz << 12) / F2;
        Dx = (Dx << 12) / F2;
        Mat.m[0][2] = -Dx;
        Mat.m[2][0] = Dx;
        MulMatrix(&GsWSMATRIX, &Mat);
    }

    V.vx = -pVp[0];
    V.vy = -pVp[1];
    V.vz = -pVp[2];
    ApplyMatrixLV(&GsWSMATRIX, &V, (VECTOR *)&GsWSMATRIX.t);
}

void Set_light_pos(VECTOR *pV)
{
    Set_light(Get_light_cut((LONG *)pV), pV);
}

INCLUDE_ASM("asm/nonmatchings/game/sub", Set_light_default);

void Set_light(LIGHT_CUT_DATA *pLightCut, VECTOR *pV)
{
    VECTOR Vec;
    UCHAR Col[4];
    ULONG No;
    LONG modeOfs;
    LONG atten;
    LONG dist;
    ULONG lightRng;

    No = 0;
    modeOfs = 4;
    do {
        if (pLightCut->Mode[No] == 0) {
            Vec.vx = (pV->vx - pLightCut->Pos[No][0]) >> 4;
            Vec.vy = (pV->vy - pLightCut->Pos[No][1]) >> 4;
            Vec.vz = (pV->vz - pLightCut->Pos[No][2]) >> 4;
            lightRng = pLightCut->L[No] >> 4;
            dist = SquareRoot0(Vec.vx * Vec.vx + Vec.vz * Vec.vz);
            atten = lightRng - dist;
            if (atten < 0) {
                atten = 0;
            }
            if (lightRng != 0) {
                Col[0] = (LONG)(pLightCut->Col[No][0] * atten) / (LONG)lightRng;
                Col[1] = (LONG)(pLightCut->Col[No][1] * atten) / (LONG)lightRng;
                Col[2] = (LONG)(pLightCut->Col[No][2] * atten) / (LONG)lightRng;
            } else {
                *(ULONG *)Col = 0;
            }
            Set_Light_data(No, &Vec, Col, pLightCut);
        } else {
            Vec.vx = pLightCut->Pos[No][0];
            Vec.vy = pLightCut->Pos[No][1];
            Vec.vz = pLightCut->Pos[No][2];
            Set_Light_data(No, &Vec, (UCHAR *)pLightCut + modeOfs, pLightCut);
        }
        modeOfs += 3;
        No++;
    } while ((LONG)No < 3);
    SetBackColor(pLightCut->Ambient[0], pLightCut->Ambient[1], pLightCut->Ambient[2]);
}

void Set_Light_data(ULONG No, VECTOR *pLig, UCHAR *pRgb, LIGHT_CUT_DATA *pLight)
{
    VECTOR normVec;
    SHORT *pLm;
    SHORT *pCm;

    pLm = &LIGHTMATRIX.m[No][0];
    pCm = &COLORMATRIX.m[0][No];
    VectorNormal(pLig, &normVec);
    pLm[0] = normVec.vx * -pLight->Mag;
    pLm[1] = normVec.vy * -pLight->Mag;
    pLm[2] = normVec.vz * -pLight->Mag;
    pCm[0] = *pRgb << 4;
    pCm[3] = pRgb[1] << 4;
    pCm[6] = pRgb[2] << 4;
}

INCLUDE_ASM("asm/nonmatchings/game/sub", Get_light_cut);

typedef struct {
    UCHAR Unk0;
    UCHAR Unk1;
    USHORT Unk2;
    LONG Unk4;
    LONG Unk8[3];
    UCHAR Unk14[4];
    LONG Unk18[3];
} FI_SAVE;
extern FI_SAVE D_800E11D0;
extern LONG D_800E11F4;

void func_80078FE8(void)
{
    ULONG i;
    tagGLOBAL *pG = &G;
    FI_SAVE *p = &D_800E11D0;

    i = 0;
    p->Unk1 = pG->Save_slot;
    p->Unk4 = pG->Unk77F8[0];
    p->Unk2 = pG->Unk7806;
    D_800E11D0.Unk0 = pG->Save_cnt;
    for (; i < 3; i++) {
        p->Unk8[i] = pG->Unk7E98[i];
        D_800E11D0.Unk14[i] = pG->Unk7EA4[i];
        p->Unk18[i] = pG->Unk7EA8[i];
    }
    D_800E11F4 = pG->Unk7EB4;
}

void func_8007906C(void)
{
    ULONG i;
    FI_SAVE *p = &D_800E11D0;
    tagGLOBAL *pG = &G;

    i = 0;
    pG->Save_slot = p->Unk1;
    pG->Unk77F8[0] = p->Unk4;
    pG->Unk7806 = p->Unk2;
    pG->Save_cnt = D_800E11D0.Unk0;
    for (; i < 3; i++) {
        pG->Unk7E98[i] = p->Unk8[i];
        pG->Unk7EA4[i] = D_800E11D0.Unk14[i];
        pG->Unk7EA8[i] = p->Unk18[i];
    }
    pG->Unk7EB4 = D_800E11F4;
}

INCLUDE_ASM("asm/nonmatchings/game/sub", func_800790F0);

void func_800791F0(void)
{
    func_80078FE8();
    func_800100A4(G.Card_head, D_800C7048.Save, 0x908);
    G.Pl_id = D_800C7048.Pl_id;
    G.Pl_pos_x = D_800C7048.Pos_x;
    G.Pl_pos_y = D_800C7048.Pos_y;
    G.Pl_pos_z = D_800C7048.Pos_z;
    G.Pl_cdir_y = D_800C7048.Cdir_y;
    G.Pl_life = D_800C7048.Life;
    if (D_800C7048.Poison & 0x80) {
        G.Pl_poison_timer = D_800C7048.Poison;
        G.Pl_status_flg |= 0x200;
    } else {
        G.Pl_poison_timer = 0;
        G.Pl_status_flg &= ~0x200;
    }
    func_8007906C();
    G.Next_cut_no = G.Cut_no;
    func_800792FC();
    func_800790F0((ULONG *)0x80100000);
    G.System_flg |= 0x4000;
    Task_lexecute(1, (void *)2);
    Task_sleep(1);
    G.System_flg &= ~0x4000;
    G.System_flg &= ~0x40000;
}

void func_800792FC(void)
{
    func_800783BC(2, Cd_read_s(0x128, 0, (ULONG *)0x80101000, 2, "INIT_SUB"), 0);
    Cd_read(0x127, (ULONG *)0x80101000, 0, "INIT_SUB");
    func_8007836C(2, (ULONG *)0x80101000);
}


void func_80079374(MATRIX *pM0, MATRIX *pM1, LONG *pT, MY_JNT *pJ)
{
    MATRIX m0;
    MATRIX m1;
    LONG x;
    LONG y;
    LONG z;
    LONG xy2;
    LONG dxy;
    LONG d;
    LONG c;
    LONG s;
    LONG l1;
    LONG l2;
    LONG sum;
    LONG d2;
    LONG l1_2;
    LONG l2_2;

    x = pT[0];
    y = pT[1];
    z = -pT[2];
    xy2 = x * x + y * y;
    dxy = SquareRoot0(xy2);
    d = SquareRoot0(xy2 + z * z);
    c = (dxy << 12) / d;
    s = (z << 12) / d;
    ((LONG *)m0.m)[0] = 0x1000;
    ((LONG *)m0.m)[1] = 0;
    m0.m[1][1] = c;
    m0.m[1][2] = s;
    m0.m[2][0] = 0;
    m0.m[2][1] = -s;
    m0.m[2][2] = c;
    c = (y << 12) / dxy;
    s = (x << 12) / dxy;
    m1.m[0][0] = c;
    m1.m[0][1] = s;
    m1.m[0][2] = 0;
    m1.m[1][0] = -s;
    m1.m[1][1] = c;
    m1.m[1][2] = 0;
    ((LONG *)m1.m)[3] = 0;
    ((LONG *)m1.m)[4] = 0x1000;
    func_8002D4B0(&m1, &m0, &m1);
    l1 = pJ->a.pad;
    l2 = pJ->b.pad;
    sum = l1 + l2;
    if (d < sum) {
        d2 = d * d;
        l1_2 = l1 * l1;
        l2_2 = l2 * l2;
        c = ((d2 + l1_2 - l2_2) << 3) / l1;
        c <<= 8;
        c /= d;
        s = SquareRoot0(0x1000000 - c * c);
        m0.m[0][0] = c;
        m0.m[0][1] = s;
        m0.m[0][2] = 0;
        m0.m[1][0] = -s;
        m0.m[1][1] = c;
        m0.m[1][2] = 0;
        ((LONG *)m0.m)[3] = 0;
        ((LONG *)m0.m)[4] = 0x1000;
        MulMatrix(&m1, &m0);
        if (sum + 500 < d) {
            c = ((l2_2 + l1_2 - d2) << 3) / l1;
            c <<= 8;
            c /= l2;
        } else {
            c = ((l2_2 + l1_2 - d2) << 4) / l1;
            c <<= 7;
            c /= l2;
        }
        s = SquareRoot0(0x1000000 - c * c);
        pM1->m[0][0] = -c;
        pM1->m[0][1] = -s;
        pM1->m[0][2] = 0;
        pM1->m[1][0] = s;
        pM1->m[1][1] = -c;
        pM1->m[1][2] = 0;
        ((LONG *)pM1->m)[3] = 0;
        ((LONG *)pM1->m)[4] = 0x1000;
    }
    MulMatrix(pM0, &m1);
}


void Copy_rot_matrix(MATRIX *m0, MATRIX *m1)
{
    LONG r0, r1, r2, r3, r4;

    r0 = ((LONG *)m0)[0];
    r1 = ((LONG *)m0)[1];
    r2 = ((LONG *)m0)[2];
    r3 = ((LONG *)m0)[3];
    r4 = ((LONG *)m0)[4];
    ((LONG *)m1)[0] = r0;
    ((LONG *)m1)[1] = r1;
    ((LONG *)m1)[2] = r2;
    ((LONG *)m1)[3] = r3;
    ((LONG *)m1)[4] = r4;
}

void Copy_matrix(MATRIX *m0, MATRIX *m1)
{
    LONG r0, r1, r2, r3, r4, r5, r6, r7;

    r0 = ((LONG *)m0)[0];
    r1 = ((LONG *)m0)[1];
    r2 = ((LONG *)m0)[2];
    r3 = ((LONG *)m0)[3];
    r4 = ((LONG *)m0)[4];
    r5 = ((LONG *)m0)[5];
    r6 = ((LONG *)m0)[6];
    r7 = ((LONG *)m0)[7];
    ((LONG *)m1)[0] = r0;
    ((LONG *)m1)[1] = r1;
    ((LONG *)m1)[2] = r2;
    ((LONG *)m1)[3] = r3;
    ((LONG *)m1)[4] = r4;
    ((LONG *)m1)[5] = r5;
    ((LONG *)m1)[6] = r6;
    ((LONG *)m1)[7] = r7;
}

LONG func_80079690(ENEMY_WORK *pEm, UCHAR *pKan, UCHAR *pSeq, ULONG Flg)
{
    UCHAR pad[0x50];
    UCHAR *pS;
    ULONG w;
    USHORT base;
    ULONG n;
    UCHAR *pF;
    PARTS_W *pP;
    ULONG w2;
    MATRIX *pM;

    pS = (UCHAR *)(EM_P(pEm, 0x1A0, UCHAR) * 8 + (ULONG)pSeq);
    n = *(ULONG *)pS;
    base = *(USHORT *)(pS + 4);
    pSeq += (n >> 17) * 2;
    pM = D_800E1200;
    n &= 0xFFFF;
    if (Flg & 0xFFFF0000) {
        pSeq += (n - EM_P(pEm, 0x1A1, UCHAR)) * 2 - 2;
    } else {
        pSeq += EM_P(pEm, 0x1A1, UCHAR) * 2;
    }
    pKan += ((USHORT *)pKan)[1] & 0xFFFC;
    pKan += (*pSeq + (SHORT)base) * 32;
    pF = pKan;
    EM_P(pEm, 0x1A4, LONG) = FLD(pF, 0x14, LONG);
    EM_P(pEm, 0x1A8, USHORT) = FLD(pF, 0x18, USHORT);
    EM_P(pEm, 0x1AC, ULONG) = FLD(pF, 0x1A, USHORT) | (FLD(pF, 0x1C, ULONG) << 16);
    EM_P(pEm, 0x1B0, USHORT) = FLD(pF, 0x1E, USHORT);
    pEm->pSin_parts_ptr[9].Cdir_y = FLD(pF, 8, USHORT) & 0xFFF;
    pEm->pSin_parts_ptr[12].Cdir_y = FLD(pF, 0xE, USHORT) & 0xFFF;
    pP = &pEm->pSin_parts_ptr[11];
    w2 = FLD(pF, 8, ULONG);
    *(ULONG *)&pP->Cdir_x = ((w2 >> 12) & 0xFFF) | (((w2 >> 24) | ((FLD(pF, 0xC, USHORT) & 0xF) << 8)) << 16);
    pP->Cdir_z = (FLD(pF, 0xC, ULONG) & 0xFFF0) >> 4;
    RotMatrixZYX_gte((SVECTOR *)&pP->Cdir_x, &pM[2]);
    pP = &pEm->pSin_parts_ptr[14];
    *(ULONG *)&pP->Cdir_x = (FLD(pF, 0xC, ULONG) >> 28) | (FLD(pF, 0x10, UCHAR) << 4) | (((FLD(pF, 0x10, ULONG) >> 8) & 0xFFF) << 16);
    pP->Cdir_z = FLD(pF, 0x10, ULONG) >> 20;
    RotMatrixZYX_gte((SVECTOR *)&pP->Cdir_x, &pM[5]);
    EM_P(pEm, 0x1A1, UCHAR)++;
    if (EM_P(pEm, 0x1A1, UCHAR) >= n) {
        EM_P(pEm, 0x1A1, UCHAR) = 0;
        return 1;
    }
    return 0;
}

void func_80079868(ENEMY_WORK *pEm, LONG rate)
{
    MATRIX m0;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    MATRIX m4;
    MATRIX m5;
    MY_JNT j;
    VECTOR v;
    UCHAR cnt;
    MATRIX *pW;
    PARTS_W *pP;
    PARTS_W *pP0;
    MATRIX *pM;
    SHORT r;
    LONG w;
    ULONG n;

    pW = (MATRIX *)((UCHAR *)pEm + 0x20);
    pP0 = pEm->pSin_parts_ptr;
    cnt = EM_P(pEm, 0x1A2, UCHAR);
    pM = D_800E1200;
    RotMatrix((SVECTOR *)&pEm->Cdir_x, pW);
    func_8002D4B0(pW, (MATRIX *)pP0->M, &m4);
    func_8002D4B0(&m4, (MATRIX *)pP0[8].M, &pP0[8].Workm);
    pP0 = &pEm->pSin_parts_ptr[8];
    Copy_rot_matrix(&pP0->Workm, &m0);
    m0.t[0] = (pP0->Workm.t[0] - pEm->Pos_x) << 3;
    m0.t[1] = (pP0->Workm.t[1] - pEm->Pos_y) << 3;
    m0.t[2] = (pP0->Workm.t[2] - pEm->Pos_z) << 3;
    j = *EM_P(pEm, 0x1B4, MY_JNT *);
    pP = pEm->pSin_parts_ptr;
    RotMatrix_gte((SVECTOR *)&pP[9].Cdir_x, pM);
    RotMatrix_gte((SVECTOR *)&pP[12].Cdir_x, &D_800E1260);
    Copy_rot_matrix(pW, &m3);
    Copy_rot_matrix(pM, &m4);
    Copy_rot_matrix(&D_800E1260, &m5);
    Copy_rot_matrix(&GsIDMATRIX, &D_800E1220);
    Copy_rot_matrix(&GsIDMATRIX, &D_800E1280);
    m4.t[0] = j.a.vx;
    m4.t[1] = j.a.vy;
    m4.t[2] = j.a.vz;
    m5.t[0] = j.b.vx;
    m5.t[1] = j.b.vy;
    m5.t[2] = j.b.vz;
    func_8002D4B0(&m0, &m5, &m2);
    func_8002D4B0(&m0, &m4, &m1);
    if (pEm->UnkAE & 0x40) {
        func_80079CC0(pEm, pEm->pSin_parts_ptr->Pos_y);
    }
    ApplyMatrix(&m3, (SVECTOR *)((UCHAR *)pEm + 0x1A4), (VECTOR *)m3.t);
    ApplyRotMatrix((SVECTOR *)((UCHAR *)pEm + 0x1AC), &v);
    MulLMatrix(&m1, &m3, &m4);
    m3.t[0] = v.vx;
    m3.t[1] = v.vy;
    m3.t[2] = v.vz;
    MulLMatrix(&m2, &m3, &m5);
    func_80079374(pM, &D_800E1220, m4.t, &j);
    func_80079374(&D_800E1260, &D_800E1280, m5.t, &j);
    n = cnt;
    if (n == 0) {
        Copy_rot_matrix(pM, (MATRIX *)pP[9].M);
        Copy_rot_matrix(&D_800E1220, (MATRIX *)pP[10].M);
        Copy_rot_matrix(&pM[2], (MATRIX *)pP[11].M);
        Copy_rot_matrix(&D_800E1260, (MATRIX *)pP[12].M);
        Copy_rot_matrix(&D_800E1280, (MATRIX *)pP[13].M);
        Copy_rot_matrix(&pM[5], (MATRIX *)pP[14].M);
    } else {
        r = (rate & 0xFFFF) * n;
        w = 0x1000 - r;
        HMatrix((MATRIX *)pP[9].M, pM, (MATRIX *)pP[9].M, w);
        HMatrix((MATRIX *)pP[10].M, &D_800E1220, (MATRIX *)pP[10].M, w);
        HMatrix((MATRIX *)pP[11].M, &pM[2], (MATRIX *)pP[11].M, w);
        HMatrix((MATRIX *)pP[12].M, &D_800E1260, (MATRIX *)pP[12].M, w);
        HMatrix((MATRIX *)pP[13].M, &D_800E1280, (MATRIX *)pP[13].M, w);
        HMatrix((MATRIX *)pP[14].M, &pM[5], (MATRIX *)pP[14].M, 0xFFF - r);
        EM_P(pEm, 0x1A2, UCHAR)--;
    }
}

void func_80079C70(UCHAR *p)
{
    UCHAR buf[0xE8];
    UCHAR *q = *(UCHAR **)(p + 0x108);

    MulMatrix0((MATRIX *)(p + 0x20), (MATRIX *)(q + 0x828), (MATRIX *)(q + 0x854));
    MulMatrix0((MATRIX *)(p + 0x20), (MATRIX *)(q + 0xA5C), (MATRIX *)(q + 0xA88));
}

void func_80079CC0(ENEMY_WORK *pEm, LONG h)
{
    MATRIX m;
    SVECTOR s0;
    SVECTOR s1;
    VECTOR v;
    LONG d;
    LONG n;
    SHORT y;

    memset(&s1, 0, 8);
    EM_P(pEm, 0x1B8, SHORT) = 0;
    s1.vy = pEm->Cdir_y;
    RotMatrix(&s1, &m);
    ApplyMatrixSV(&m, (SVECTOR *)((UCHAR *)pEm + 0x1A4), &s0);
    ApplyMatrixSV(&m, (SVECTOR *)((UCHAR *)pEm + 0x1AC), &s1);
    v.vx = pEm->Pos_x + ((SHORT)s0.vx >> 3);
    v.vz = pEm->Pos_z + ((SHORT)s0.vz >> 3);
    v.vy = pEm->Pos_y;
    d = func_8004D720(&v, 0x320, 0x8000, 0x69) - EM_P(pEm, 0x122, SHORT);
    n = 0;
    if (h < d) {
        y = EM_P(pEm, 0x1A6, SHORT);
        d <<= 3;
        if (d < y) {
            EM_P(pEm, 0x1A6, SHORT) = d;
        } else {
            n = 1;
            EM_P(pEm, 0x1B8, SHORT) += (d - y) >> 6;
        }
    }
    v.vx = pEm->Pos_x + ((SHORT)s1.vx >> 3);
    v.vz = pEm->Pos_z + ((SHORT)s1.vz >> 3);
    v.vy = pEm->Pos_y;
    d = func_8004D720(&v, 0x320, 0x8000, 0x69) - EM_P(pEm, 0x122, SHORT);
    if (h < d) {
        y = EM_P(pEm, 0x1AE, SHORT);
        d <<= 3;
        if (d < y) {
            if (pEm != G.pUnk265C && pEm->Type != 1) {
                EM_P(pEm, 0x1AE, SHORT) = d;
            }
        } else {
            n++;
            EM_P(pEm, 0x1B8, SHORT) += (d - y) >> 6;
        }
    }
    if (n != 0) {
        EM_P(pEm, 0x1B8, SHORT) >>= n - 1;
    }
    if (*(SHORT *)&pEm->Routine_0 != 0x501) {
        EM_P(pEm, 0x1B8, SHORT) = 0;
    }
}

void func_80079EB8(void) {
}
