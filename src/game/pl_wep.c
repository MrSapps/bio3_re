/* ---- from 800303CC.c ---- */
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

/* RE3-only collision object list (80033508..80035790). Nodes are linked through pNext;
 * the player's node lives at Pl_work + 0x80. */
typedef struct tagCOL_OBJ {
    UCHAR Id;                          /* 0x00 */
    UCHAR Flg;                         /* 0x01 */
    UCHAR Type;                        /* 0x02 0 = player, < 3 = character */
    UCHAR Unk3;                        /* 0x03 */
    ENEMY_WORK *pEm;                   /* 0x04 */
    LONG Pos_x;                        /* 0x08 */
    LONG Pos_y;                        /* 0x0C */
    LONG Pos_z;                        /* 0x10 */
    SHORT Old_x;                       /* 0x14 */
    SHORT Old_y;                       /* 0x16 */
    SHORT Old_z;                       /* 0x18 */
    USHORT W0;                         /* 0x1A */
    USHORT D0;                         /* 0x1C */
    USHORT H;                          /* 0x1E */
    USHORT W1;                         /* 0x20 */
    USHORT D1;                         /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    SHORT Unk26;                       /* 0x26 */
    SHORT Unk28;                       /* 0x28 */
    SHORT Unk2A;                       /* 0x2A */
    SHORT Unk2C;                       /* 0x2C */
    USHORT Hit_flg;                    /* 0x2E */
    MATRIX *pM;                        /* 0x30 */
    struct tagCOL_OBJ *pNext;          /* 0x34 */
    UCHAR Em_idx;                      /* 0x38 */
    UCHAR Unk39;                       /* 0x39 */
    SHORT Unk3A;                       /* 0x3A */
    LONG Unk3C;                        /* 0x3C */
} COL_OBJ;

typedef struct tagCOL_ENT {            /* 0x18 bytes */
    SHORT X0;                          /* 0x00 */
    SHORT Z0;                          /* 0x02 */
    SHORT X1;                          /* 0x04 */
    SHORT Z1;                          /* 0x06 */
    USHORT Unk8;                       /* 0x08 */
    USHORT UnkA;                       /* 0x0A */
    LONG UnkC;                         /* 0x0C -1 terminates */
    UCHAR Type;                        /* 0x10 */
    UCHAR Em_idx;                      /* 0x11 */
    UCHAR pad12[2];
    COL_OBJ *pObj;                     /* 0x14 */
} COL_ENT;

typedef struct tagCOL_WORK {
    COL_OBJ *pTop;                     /* 0x00 */
    COL_OBJ *pTail;                    /* 0x04 */
    COL_ENT Ent[1];                    /* 0x08 */
} COL_WORK;
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

/* array inside G at an offset not yet in tagGLOBAL (keeps the &G + index*size + ofs addressing) */
#define G_ARR(type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)&G)->a)
#define P_ARR(p, type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)(p))->a)

typedef ULONG OT8[8];

extern ULONG D_800CC87C;
extern ULONG D_800D1F2C;
extern UCHAR D_8009CD3C;
ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Arg1);
LONG Hit_ck_point4_l(ULONG Arg0, ULONG Arg1) __asm__("func_8001020C");
void func_800288C8(ENEMY_WORK *pEm, PARTS_W *pParts, LONG Arg2, MATRIX *pM);

/* G.pUnk265C, G.pUnk2660 and G.pEnemy[40] seen as one array */
typedef struct {
    UCHAR pad0[0x265C];
    ENEMY_WORK *pWork[42];             /* 0x265C */
} G_WORKS;

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
void func_80031504(ULONG x, ULONG y, UCHAR *pStr, ULONG Attr);

static __inline__ UCHAR *Msg_get_80031BDC(ULONG No)
{
    return D_80098E88 + D_80099654[No];
}

ULONG func_80012818(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
extern LOADTASK D_8009C944[];
ULONG Task_lexecute_(ULONG Level, ULONG Task_no) __asm__("Task_lexecute");

void Task_lchain_(ULONG Task_no) __asm__("Task_lchain");

extern void (*D_8009CBB4[])(void);

#define EM_COL(p) ((COL_OBJ *)((UCHAR *)(p) + 0x80))

LONG func_800346AC(COL_OBJ *pObj, ENEMY_WORK *pEm, ENEMY_WORK *pT);

LONG func_80034448(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG func_8004B374(COL_ENT *pEnt, ULONG Type, ULONG Arg2);

void func_8002AB34(ULONG *pTmd, ULONG Tpage, ULONG Clut);
PARTS_W *func_8002E61C(ENEMY_WORK *pEm, PARTS_W *pParts);
PARTS_W *func_80035868(ENEMY_WORK *pEm, PARTS_W *pParts);
void func_8003589C(ENEMY_WORK *pEm, ULONG *pData);

extern UCHAR D_800CC84B;

ULONG Tex_chg_(CC_PARTS_WORK *pCcp, RECT *pRect, ULONG Ctr) __asm__("Tex_chg");

typedef struct tagOM_WORK {
    ULONG Be_flg;                      /* 0x000 */
    UCHAR Routine_0;                   /* 0x004 */
    UCHAR pad5[0x194 - 0x5];
} OM_WORK;
extern void (*D_8009CC64[])(OM_WORK *pOm);
extern LONG D_800DD4AC;
extern ENEMY_WORK *D_800DD4B0;
void func_80036570(void);

extern SHORT D_800DE614;
extern SHORT D_800DE618;
extern SHORT D_800DE61A;
extern SHORT D_800DE61E;

extern ULONG D_800D20D4[];
void Flg_on(ULONG *pFlg, ULONG Id);
void func_800366EC(ENEMY_WORK *pEm, ULONG Arg1, ULONG Arg2);
void Em_routine0_set1_80036640(ENEMY_WORK *pEm);

LONG func_8004D720_3(VECTOR *pV, ULONG W, ULONG Arg2) __asm__("func_8004D720");

extern SHORT D_800DD492;
extern SHORT D_800DD494;
extern SHORT D_800DD496;
extern SHORT D_800DD498;
extern SHORT D_800DD49A;
extern SHORT D_800DD49C;
extern SVECTOR D_800DD4A0;
extern UCHAR D_800DD4A8;
void Rot_add_matrix(LONG *pPos, SHORT *pDir, MATRIX *pM, SVECTOR *Add_dir);

typedef struct {
    LONG *pPos;
    SHORT W;
    SHORT H;
    SHORT D;
    SHORT padA;
    SHORT R;
    SHORT padE;
} HIT_REQ;
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

typedef struct {
    UCHAR pad0[2];
    UCHAR Num;
    UCHAR Idx;
} OM_BREAK_TBL;
extern OM_BREAK_TBL D_8009CBEC[];
extern ULONG D_8009CC54[];

typedef struct {
    OBJ_MODEL_WORK Om[32];
    OBJ_MODEL_WORK *pEnd;              /* G + 0x75A8 */
} OB_MODEL_TBL;
extern OB_MODEL_TBL D_800CEA60;        /* G.Ob_model */

typedef ULONG ULONG4[4];
ULONG func_80037D50(VECTOR *pPos);

LONG func_800101C8(VECTOR *pPos, UCHAR *pArea);

typedef struct {
    UCHAR pad0[4];
    SHORT Unk4;                        /* 0x04 */
    UCHAR Num;                         /* 0x06 */
    UCHAR pad7;
    UCHAR Tbl[16][2];                  /* 0x08 */
} PAD_WORK_800DE628;
extern PAD_WORK_800DE628 D_800DE628;

void func_80045094(ENEMY_WORK *pEm);
void func_80045950(ENEMY_WORK *pEm);
void Pl_wep_recoil_6(ENEMY_WORK *pEm);
void Pl_wep_recoil_10(ENEMY_WORK *pEm);
void Pl_wep_spin_0b(ENEMY_WORK *pEm);
void func_8003DEA4(ENEMY_WORK *pEm, ULONG Lim, SHORT Dir);
void func_8001A248(ENEMY_WORK *pEm, SHORT Dir);
LONG func_800102E8(void);
void func_8001B484(ULONG Arg0, ULONG Arg1, MATRIX *pM, ULONG Arg3, ULONG Arg4);

void Kage_work9_set(KAGE_WORK9 **ppKage, ULONG Arg1, ULONG Arg2, ULONG Arg3, VECTOR *pPos);

extern UCHAR D_8009CDEC[];

void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
extern UCHAR D_8009CDE0[];
void func_800776B0(ULONG Arg0, ULONG No, ENEMY_WORK *pEm);

extern UCHAR D_8009CDF0[];

extern UCHAR D_8009CDF4[];

extern UCHAR D_8009CDF8[];

extern UCHAR D_8009CDE0[];
void func_800776B0(ULONG Arg0, ULONG No, ENEMY_WORK *pEm);

extern UCHAR D_8009CDE0[];
void func_800776B0(ULONG Arg0, ULONG No, ENEMY_WORK *pEm);

LONG func_80026BE8(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079690(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079868(ENEMY_WORK *pEm, LONG a1);

LONG func_8003C5B0(ENEMY_WORK *pEm, ULONG Flg);

extern USHORT D_8009CDFC[];
extern UCHAR D_8009CF78[];

extern USHORT D_8009CDFC[];
extern UCHAR D_8009CF78[];

typedef struct {
    LONG *pPos;
    SHORT W;
    SHORT H;
    SHORT D;
    SHORT padA;
    SHORT R;
    SHORT padE;
} HIT_REQ2;
LONG func_800472EC_h(HIT_REQ2 *pReq, ULONG *a1) __asm__("func_800472EC");
void Quake_add(LONG X, LONG Y);
extern ULONG D_8009CE0C;
extern UCHAR *D_800BA6E8;

void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_8003DEA4);

LONG func_800102E8(void);
void func_8001B484(ULONG Arg0, ULONG Arg1, MATRIX *pM, ULONG Arg3, ULONG Arg4);

void func_8003E088(ENEMY_WORK *pEm)
{
    SVECTOR sv;
    SVECTOR sv2;
    PARTS_W *pParts;

    pParts = pEm->pSin_parts_ptr;
    func_8002D4B0((MATRIX *)((UCHAR *)pEm + 0x20), (MATRIX *)pParts->M, &pParts->Workm);
    sv.vx = pParts->Workm.t[0];
    sv.vy = pEm->Water;
    sv.vz = pParts->Workm.t[2];
    func_8001B484(0x61C, ((func_800102E8() * 4 + 0xC00) << 16) | (((USHORT)pEm->Cdir_y - func_800102E8() * 4) & 0xFFFF), &GsIDMATRIX, (ULONG)&sv2, 0);
    func_8001B484(0x61C, ((func_800102E8() * 4 + 0xC00) << 16) | (((USHORT)pEm->Cdir_y + (0x800 - func_800102E8() * 4)) & 0xFFFF), &GsIDMATRIX, (ULONG)&sv, 0);
    func_8001B484(0x161C, ((func_800102E8() * 4 + 0x800) << 16) | ((func_800102E8() * 16) & 0xFFFF), &GsIDMATRIX, (ULONG)&sv, 0);
    func_8001B484(0x161C, ((func_800102E8() * 4 + 0xC00) << 16) | ((func_800102E8() * 16) & 0xFFFF), &GsIDMATRIX, (ULONG)&sv, 0);
    sv2.vx = 0x400;
    sv2.vy = 0;
    sv2.vz = 0;
    func_8001B484(0x20E, (USHORT)pEm->Cdir_y | 0x20000000, &GsIDMATRIX, (ULONG)&sv, (ULONG)&sv2);
}

void Pl_weapon_vib(ENEMY_WORK *pEm)
{
    UCHAR (*pTbl)[6] = Pl_vib_tbl;
    UCHAR *pVib;

    pVib = pTbl[(UCHAR)(pEm->Type - 2)];
    Pad_vib_set0(pVib[0], 0);
    Pad_vib_set1(pVib[1], pVib[2], 0);
    Pad_vib_fade_set(pVib[5], pVib[3], pVib[4], pVib[1]);
}

void pl_mv05_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", pl_mv05);

void pl_mv05_w0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w0_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_8003E4D0);

void func_8003E088(ENEMY_WORK *pEm);
LONG func_80026BE8(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079690(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079868(ENEMY_WORK *pEm, LONG a1);

void func_8003EA5C(ENEMY_WORK *pEm)
{
    SHORT a;

    a = EM_SHORT(pEm, 0xA);
    if (a != 0 && a < EM_SHORT(pEm, 0x122) - 500 && !(pEm->Move_cnt & 1) && pEm->Move_cnt < 9) {
        func_8003E088(pEm);
    }
    if (func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x400)) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x400);
    func_80079868(pEm, 0x400);
}

void pl_mv05_w1(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w1_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_8003EB64);

SHORT func_8006D030(ULONG Arg0);
signed char func_8006CF0C_c(ULONG Arg0) __asm__("func_8006CF0C");
extern UCHAR D_8009CD3D_b __asm__("D_8009CD3D");
extern UCHAR D_8009CF64[];

void func_8003EF08(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Unk1A0 = *(ULONG *)&pEm->Move_no = D_8009CD3D_b * 2 + 0x70002;
        G.Unk75E8 |= 0x18;
    case 1:
        func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x200);
        func_80079868(pEm, 0x200);
        break;
    }
    if ((G.Key_trg & 0x800) && !(Main.Unk196 & 0x40)) {
        *(USHORT *)&pEm->Routine_2 = 5;
        return;
    }
    if (!(Key & 0x500)) {
        *(USHORT *)&pEm->Routine_2 = 3;
        return;
    }
    if ((Key & 4) && D_8009CD3D_b != 2) {
        D_8009CD3D_b = 2;
        pEm->Routine_3 = 0;
    }
    if ((Key & 1) && D_8009CD3D_b != 1) {
        D_8009CD3D_b = 1;
        pEm->Routine_3 = 0;
    }
    if (!(Key & 5) && D_8009CD3D_b != 0) {
        D_8009CD3D_b = 0;
        pEm->Routine_3 = 0;
    }
    if (Key & 0x40) {
        if (func_8006D030(0) != -1) {
            *(USHORT *)&pEm->Routine_2 = 2;
            pEm->Be_flg |= 0x10000000;
            if ((UCHAR)pEm->Type == 1 && (Key & 0x20)) {
                pEm->Be_flg &= 0x7FFFFFFF;
            }
            if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
                *(ULONG *)&pEm->Routine_0 = 0x20701;
                Pl_mv[pEm->Routine_1](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
            }
            return;
        }
        if (G.Key_trg & 0x40) {
            if (func_8006CF0C_c(0) == 0) {
                *(USHORT *)&pEm->Routine_2 = 4;
            } else {
                func_800746C0(0x10101, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
            }
        }
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CF64[(UCHAR)pEm->Type - 1] & 0x7F;
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CF64[(UCHAR)pEm->Type - 1] & 0x7F;
    }
}

extern UCHAR D_8009CF28[][3];
extern UCHAR D_8009CD3D_a[] __asm__("D_8009CD3D");
extern UCHAR D_8009CF64[];

void func_8003F208(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    SHORT a;
    SVECTOR Dummy;

    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = D_8009CD3D_a[0] * 2 + 0x30001;
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Unk1A0 = D_8009CD3D_a[0] * 2 + 0x30001;
    case 1:
        a = pEm->Water;
        if (a != 0 && a < pEm->Unk122 - 500 && !(pEm->Move_cnt & 3) && pEm->Move_cnt < 10) {
            func_8003E088(pEm);
        }
        ((void (*)(ENEMY_WORK *))pl_mv05_t2_spr[(UCHAR)pEm->Type - 1])(pEm);
        if (!(D_8009CF64[(UCHAR)pEm->Type - 1] & 0x80)) {
            Spd_get(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0);
        }
        if (func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x400)) {
            *(USHORT *)&pEm->Routine_2 = 1;
            pEm->Spd_x = 0;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x400);
        func_80079868(pEm, 0x200);
        if (!(D_8009CF64[(UCHAR)pEm->Type - 1] & 0x80)) {
            Add_speedXZ(pEm, 0);
        }
        break;
    }
    if (!(Key & 0x500) && (D_8009CF28[(UCHAR)pEm->Type - 1][2] & 0x7F) < pEm->Move_cnt) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
}

void func_8003F414(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    LONG a;

    switch (pEm->Routine_3) {
    case 0:
        a = EM_SHORT(pEm, 0xA);
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70000;
        *(ULONG *)((UCHAR *)pEm + 0x1A0) = 0x70000;
        pEm->Spd_x = 0;
        if (a != 0 && a < EM_SHORT(pEm, 0x122) - 500) {
            func_8003E088(pEm);
        }
    case 1:
        if (func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x10200)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            EM_UCHAR(pEm, 0x120) &= 0xED;
            G.Status_flg &= ~0x40;
            G.Status_flg &= ~0x100;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x10200);
        func_80079868(pEm, 0x200);
        break;
    }
}

void func_8001B484(ULONG Arg0, ULONG Arg1, MATRIX *pM, ULONG Arg3, ULONG Arg4);
void func_80044F34(ENEMY_WORK *pEm);
void func_8006CF0C(ULONG Arg0);

void func_8003F520(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    ULONG n;
    SVECTOR Sv;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70007;
        *(ULONG *)((UCHAR *)pEm + 0x1A0) = 0x70007;
    }
    if (func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x200)) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x200);
    func_80079868(pEm, 0x200);
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)&pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
    }
    if ((*pEm->pNow_seq & 0x400) && (pEm->Status_flg & 0x400)) {
        if ((UCHAR)pEm->Type == 5) {
            func_8001B484(0x302, 0x08000000, &pEm->pSin_parts_ptr[4].Workm, 0, 0);
        } else {
            func_80044F34(pEm);
        }
        pEm->Status_flg &= ~0x400;
    }
    if (*pEm->pNow_seq & 0x200) {
        func_8006CF0C(1);
        pEm->Status_flg |= 0x400;
    }
}

void func_8003F79C(ENEMY_WORK *pEm);

void func_8003F6B0(ENEMY_WORK *pEm)
{
    ENEMY_WORK *pT;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        func_8003F79C(pEm);
        pEm->Be_flg |= 0x2000000;
        pT = pEm->pEnemy_ptr;
        if (pT == pEm) {
            *(USHORT *)&pEm->Routine_2 = 1;
            break;
        }
        EM_UCHAR(pEm, 0xE3) = 12;
    case 1:
        pEm->Be_flg |= 0x2000000;
        pT = pEm->pEnemy_ptr;
        if (pT != pEm) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xD8);
        }
        if (!Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x10)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        if (--*((signed char *)pEm + 0xE3) == -1) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_8003F79C);

extern UCHAR D_8009CD3D;

void func_8003FA38(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    LONG *pPos;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x70002;
        *(ULONG *)((UCHAR *)pEm + 0x1A0) = 0x70002;
        pEm->Spd_x = 0;
        pEm->Spd_z = 0;
        EM_UCHAR(pEm, 0x120) |= 0x12;
        D_8009CD3D = 0;
    case 1:
        pPos = *(LONG **)((UCHAR *)pEm + 0x15C);
        Goto00(pEm, pPos[0], pPos[2], 0x100);
        func_80026BE8(pEm, G_ARR(LONG, 0x2618)[0], G_ARR(LONG, 0x2614)[0], 0x200);
        if (*((UCHAR *)pEm + 0xCA) == 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2610)[0], G_ARR(LONG, 0x260C)[0], 0x200);
        func_80079868(pEm, 0x200);
        break;
    case 2:
        *(ULONG *)&pEm->Move_no = 0x70002;
        pEm->Routine_3 = 3;
    case 3:
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
}

SHORT func_8006D030(ULONG Arg0);
signed char func_8006CF0C_c(ULONG Arg0) __asm__("func_8006CF0C");
extern UCHAR D_8009CD3D_b __asm__("D_8009CD3D");

void func_8003FB78(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pEm->Be_flg |= 0x20000000;
    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x7000D;
        pEm->Routine_3 = 1;
        pEm->Spd_x = 1;
        pEm->Unk120 |= 0x12;
        D_8009CD3D_b = 0;
        if (!(D_8009CF78[(UCHAR)pEm->Type] & 0x80)) {
            *(ULONG *)&pEm->Move_no = 0x70011;
            pEm->Spd_x = 0;
        }
    case 1:
        if (pEm->Spd_x != 0) {
            Goto00(pEm, pEm->pUnk15C[0], pEm->pUnk15C[2], 0x80);
        }
        if (func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            pEm->Routine_3 = 2;
            *(ULONG *)&pEm->Move_no = 0xF;
            pEm->UnkE3 = 0xF;
            pEm->Spd_x = 0;
            if (!(D_8009CF78[(UCHAR)pEm->Type] & 0x80)) {
                *(USHORT *)&pEm->Routine_2 = 6;
            }
        }
        break;
    case 2:
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x400);
        if ((G.Key_trg & 0x800) || !(Key & 0x500) || (signed char)pEm->UnkE3 == 0 || (G.Key_trg & 5)) {
            *(USHORT *)&pEm->Routine_2 = 10;
            break;
        }
        if (Key & 0x40) {
            if (func_8006D030(0) != -1) {
                *(USHORT *)&pEm->Routine_2 = 9;
                pEm->UnkE3--;
                break;
            }
            if (G.Key_trg & 0x40) {
                if (func_8006CF0C_c(0) == 0) {
                    *(USHORT *)&pEm->Routine_2 = 10;
                } else {
                    func_800746C0(0x10101, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
                }
            }
        }
        if (Key & 8) {
            pEm->Cdir_y -= 0x10;
        }
        if (Key & 2) {
            pEm->Cdir_y += 0x10;
        }
        break;
    }
}

void pl_mv05_w0_t9(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0xE;
        pEm->Routine_3 = 1;
    case 1:
        pl_mv05_t2_spr[(UCHAR)pEm->Type - 1](pEm, Kan, Seq, Key);
        if (func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 0x208;
            *(ULONG *)&pEm->Move_no = 0xF;
        }
        break;
    }
}

void pl_mv05_w0_t10(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x10;
        pEm->Routine_3 = 1;
        pEm->Spd_x = 0;
    case 1:
        if (func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        break;
    }
}

void func_8003FEFC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x11;
        pEm->Routine_3 = 1;
        pEm->Spd_x = 0;
    case 1:
        if (func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            EM_UCHAR(pEm, 0x120) &= 0xED;
            G.Status_flg &= ~0x100;
            G.Status_flg &= ~0x40;
        }
        break;
    }
}

void pl_mv05_w2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w2_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_8003FFD8);
/* ---- from 800402F4.c ---- */
#include "pl_wep.h"
#include "sca.h"
#include "monitor.h"
#include "pl_wep2.h"
#include "pad.h"

/* RE2 Esp_call-like effect spawn; returns the effect work or -1 */
typedef struct {
    UCHAR pad0[0x10];
    SHORT Unk10;                       /* 0x10 */
    SHORT Unk12;                       /* 0x12 */
    SHORT Unk14;                       /* 0x14 */
} ESP_WORK;
ESP_WORK *func_8001B484_800402F4(ULONG Id, ULONG Dir, MATRIX *pM, SVECTOR *pV, SVECTOR *pDsv) __asm__("func_8001B484");
SHORT func_8006D030(ULONG Arg0);
void func_80044804(MATRIX *pM, ULONG Arg1, ULONG Type);
void func_80047860(MATRIX *pM, ULONG Arg1, ULONG Type);
extern ULONG D_8009DBB4[];
ULONG func_800102E8_800402F4(void) __asm__("func_800102E8");                 /* rand */
SHORT Get_dir_xz(LONG X, LONG Z);
LONG func_800455B4();
LONG func_8004EF74(ULONG P0, ULONG P1, ULONG P2, ULONG P3);

extern UCHAR D_8009CD3D;
LONG func_80026BE8_800402F4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg) __asm__("func_80026BE8");
void func_80079690_800402F4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg) __asm__("func_80079690");
void func_80079868_800402F4(ENEMY_WORK *pEm, ULONG Flg) __asm__("func_80079868");
signed char func_8006CF0C_r(ULONG Arg0) __asm__("func_8006CF0C");
void func_8003E088(ENEMY_WORK *pEm);

void func_800402F4(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    SHORT Mv0[3] = { 1, 4, 7 };
    SHORT Mv1[3] = { 2, 5, 8 };

    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = Mv0[D_8009CD3D] + 0x70000;
        *(ULONG *)&pEm->Unk1A0 = Mv0[D_8009CD3D] + 0x70000;
        Pad_vib_fade_set(10, 0, Pl_vib_tbl[(UCHAR)pEm->Type - 2][2], 0);
    case 1:
        pEm->Move_no = Mv0[D_8009CD3D];
        pEm->UnkCA = 7;
        pEm->Unk1A0 = Mv0[D_8009CD3D];
        pEm->Unk1A2 = 7;
        pEm->Routine_3 = 2;
        pEm->UnkE3 = 9;
    case 2:
        if ((pEm->Move_cnt % 3) == 0 && func_8006D030(0) == -1) {
            pEm->Routine_3 = 3;
            pEm->Be_flg |= 0x2000000;
            return;
        }
        G.Unk75E8 |= 1;
        ((void (*)(PLAYER_WORK *))pl_mv05_t2_spr[(UCHAR)pEm->Type - 1])(pEm);
        func_80026BE8_800402F4(pEm, G.Pl_unk180[3], G.Pl_unk180[2], 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690_800402F4((ENEMY_WORK *)&G.Pl_be_flg, G.Pl_unk180[1], G.Pl_unk180[0], 0x200);
        func_80079868_800402F4(pEm, 0x200);
        if (!(Key & 0x500)) {
            *(USHORT *)&pEm->Routine_2 = 3;
            return;
        }
        if ((Key & 0x10) && D_8009CD3D != 1) {
            pEm->Routine_3 = 1;
            D_8009CD3D = 1;
            return;
        }
        if ((Key & 0x20) && D_8009CD3D != 2) {
            pEm->Routine_3 = 1;
            D_8009CD3D = 2;
            return;
        }
        if (!(Key & 0x30) && D_8009CD3D != 0) {
            pEm->Routine_3 = 1;
            D_8009CD3D = 0;
            return;
        }
        if (!(Key & 0x40)) {
            pEm->Routine_3 = 3;
            return;
        }
        if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 500 && !(pEm->Move_cnt & 3)) {
            func_8003E088(pEm);
        }
        break;
    case 3:
        *(ULONG *)&pEm->Move_no = Mv1[D_8009CD3D] + 0x70000;
        *(ULONG *)&pEm->Unk1A0 = Mv1[D_8009CD3D] + 0x70000;
        pEm->Routine_3 = 4;
    case 4:
        if (func_80026BE8_800402F4(pEm, G.Pl_unk180[3], G.Pl_unk180[2], 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690_800402F4((ENEMY_WORK *)&G.Pl_be_flg, G.Pl_unk180[1], G.Pl_unk180[0], 0x200);
        func_80079868_800402F4(pEm, 0x200);
        if (Key & 0x40) {
            if (func_8006D030(0) != -1) {
                *(USHORT *)&pEm->Routine_2 = 2;
                return;
            }
            if (G.Key_trg & 0x40) {
                if (func_8006CF0C_r(0) == 0) {
                    *(USHORT *)&pEm->Routine_2 = 4;
                } else {
                    func_800746C0(0x10101, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
                }
            }
        }
        break;
    }
    if (Key & 8) {
        pEm->Cdir_y -= 8;
    }
    if (Key & 2) {
        pEm->Cdir_y += 8;
    }
}

LONG func_80026BE8_800402F4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg) __asm__("func_80026BE8");
void func_80079690_800402F4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg) __asm__("func_80079690");
void func_80079868_800402F4(ENEMY_WORK *pEm, ULONG Flg) __asm__("func_80079868");
void func_8006CF0C(ULONG Arg0);
void func_80044F34(PLAYER_WORK *pEm);

void func_80040764(PLAYER_WORK *pEm)
{
    ULONG n;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x7000A;
        *(ULONG *)&pEm->Unk1A0 = 0x7000A;
    }
    if (func_80026BE8_800402F4(pEm, G.Pl_unk180[3], G.Pl_unk180[2], 0x200) != 0) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690_800402F4((ENEMY_WORK *)&G.Pl_be_flg, G.Pl_unk180[1], G.Pl_unk180[0], 0x200);
    func_80079868_800402F4(pEm, 0x200);
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
    }
    if ((*pEm->pNow_seq & 0x400) && (pEm->Status_flg & 0x400)) {
        func_80044F34(pEm);
        pEm->Status_flg &= ~0x400;
    }
    if (*pEm->pNow_seq & 0x200) {
        func_8006CF0C(1);
        pEm->Status_flg |= 0x400;
    }
}

void pl_mv05_w0b(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w0b_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

void Pl_wep_flag_set(PLAYER_WORK *pEm);

void pl_mv05_w0b_t2(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    PARTS_W *pP;
    SVECTOR Sv;

    switch (pEm->Routine_3) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x70001;
        *(ULONG *)&pEm->Unk1A0 = 0x70001;
        pEm->Routine_3 = 1;
        func_800746C0(0x10112, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
        Pad_vib_fade_set(15, 0, Pl_vib_tbl[(UCHAR)pEm->Type - 2][2], 0);
    case 1:
        Pl_wep_flag_set(pEm);
        if (func_80026BE8_800402F4(pEm, G.Pl_unk180[3], G.Pl_unk180[2], 0x200)) {
            *(ULONG *)&pEm->Move_no = 0x70002;
            *(ULONG *)&pEm->Unk1A0 = 0x70002;
            pEm->Routine_3 = 2;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690_800402F4((ENEMY_WORK *)&G.Pl_be_flg, G.Pl_unk180[1], G.Pl_unk180[0], 0x200);
        func_80079868_800402F4(pEm, 0x200);
        pP = pEm->pSin_parts_ptr;
        if ((pEm->Move_cnt % 6) == 0) {
            Sv.vx = -0x6C;
            Sv.vy = 0x4B0;
            Sv.vz = 0;
            func_8001B484_800402F4(0x603, (USHORT)(signed char)func_800102E8_800402F4() | 0x10000000, &pP[4].Workm, &Sv, 0);
        }
        if ((pEm->Move_cnt % 3) == 0) {
            Sv.vx = -0xE6;
            Sv.vy = 0x2BC;
            Sv.vz = 0;
            func_8001B484_800402F4(0x503, (USHORT)(signed char)func_800102E8_800402F4() | 0x1E000000, &pP[4].Workm, &Sv, 0);
        }
        break;
    case 2:
        Pl_wep_flag_set(pEm);
        G.Unk75E8 |= 1;
        ((void (*)(PLAYER_WORK *))pl_mv05_t2_spr[(UCHAR)pEm->Type - 1])(pEm);
        func_80026BE8_800402F4(pEm, G.Pl_unk180[3], G.Pl_unk180[2], 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690_800402F4((ENEMY_WORK *)&G.Pl_be_flg, G.Pl_unk180[1], G.Pl_unk180[0], 0x200);
        func_80079868_800402F4(pEm, 0x200);
        if (!(Key & 0x500)) {
            *(USHORT *)&pEm->Routine_2 = 3;
            func_800746C0(0x10113, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
            return;
        }
        if (!(Key & 0x40)) {
            pEm->Routine_3 = 3;
            return;
        }
        if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 500 && !(pEm->Move_cnt & 3)) {
            func_8003E088(pEm);
        }
        break;
    case 3:
        *(ULONG *)&pEm->Move_no = 0x70004;
        *(ULONG *)&pEm->Unk1A0 = 0x70004;
        pEm->Routine_3 = 4;
        pEm->Spd_x = 0;
        func_800746C0(0x10113, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
    case 4:
        if (func_80026BE8_800402F4(pEm, G.Pl_unk180[3], G.Pl_unk180[2], 0x200)) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690_800402F4((ENEMY_WORK *)&G.Pl_be_flg, G.Pl_unk180[1], G.Pl_unk180[0], 0x200);
        func_80079868_800402F4(pEm, 0x200);
        if (Key & 0x40) {
            *(USHORT *)&pEm->Routine_2 = 0x102;
            return;
        }
        break;
    case 5:
        break;
    }
    if (Key & 8) {
        pEm->Cdir_y -= 8;
    }
    if (Key & 2) {
        pEm->Cdir_y += 8;
    }
}

void Pl_wep_ready_reset(PLAYER_WORK *pEm)
{
    PARTS_W *pParts = &pEm->pSin_parts_ptr[22];

    pParts->Unk94 = 0;
    pParts->Unk86 = 0;
    pParts->Unk96 = 0;
}

void Pl_wep_recoil_6(PLAYER_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)((UCHAR)pEm->Type - 6) < 4) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Unk96 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl0[pWep->Unk86];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Unk96 = 0xFF;
                return;
            }
            pWep->Unk86++;
            RotMatrix_gte((SVECTOR *)&pParts[22].Cdir_x, (MATRIX *)pParts[22].M);
        }
    }
}

void Pl_wep_recoil_10(PLAYER_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0x10) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Unk96 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl1[pWep->Unk86];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Unk96 = 0xFF;
                return;
            }
            pWep->Unk86++;
            RotMatrix_gte((SVECTOR *)&pParts[22].Cdir_x, (MATRIX *)pParts[22].M);
        }
    }
}


void Pl_wep_flag_set(PLAYER_WORK *pEm)
{
    pEm->pSin_parts_ptr[22].Unk86 = 1;
}

void Pl_wep_spin_0b(PLAYER_WORK *pEm)
{
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0xB) {
        pWep = &pEm->pSin_parts_ptr[22];
        if (pWep->Unk86 == 0) {
            pWep->Unk96 -= 8;
            if (pWep->Unk96 < 0) {
                pWep->Unk96 = 0;
            }
        } else {
            pWep->Unk96 += 0x20;
            if (pWep->Unk96 > 0x200) {
                pWep->Unk96 = 0x200;
            }
        }
        pWep->Cdir_y += pWep->Unk96;
        RotMatrix((SVECTOR *)&pWep->Cdir_x, (MATRIX *)pWep->M);
        pWep->Unk86 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", pl_w01_t2_move);


void pl_w02_t2_move(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484_800402F4(2, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x1C2;
        Sv.vz = 0;
        func_8001B484_800402F4(1, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x10000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_800410F8);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80041594);

void func_800418CC(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484_800402F4(0x1000402, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041A80(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        func_8006D030(1);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484_800402F4(0x1000702, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041C34(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484_800402F4(0x1000802, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041DE8(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484_800402F4(0x2000902, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041F9C(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = &pEm->pSin_parts_ptr[4];
        func_800746C0(0x10100, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        switch ((SHORT)(func_8006D030(1) % 4)) {
        case 0:
            Sv.vx = 0x16A;
            Sv.vy = 0x44C;
            Sv.vz = -0x38;
            break;
        case 1:
            Sv.vx = 0x188;
            Sv.vy = 0x44C;
            Sv.vz = 0x5B;
            break;
        case 2:
            Sv.vx = 0xBD;
            Sv.vy = 0x44C;
            Sv.vz = -0x38;
            break;
        case 3:
            Sv.vx = 0xBD;
            Sv.vy = 0x44C;
            Sv.vz = 0x5B;
            break;
        }
        pM = &pP->Workm;
        func_8001B484_800402F4(0x1000502, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x17C;
        Sv.vy = 0x4B0;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x38000000, pM, &Sv, 0);
        Sv.vx = 0x17C;
        Sv.vy = 0x320;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x50000000, pM, &Sv, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x30000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 5;
            pEsp->Unk12 = -0x50;
        }
        Sv.vx = 0xDC;
        Sv.vy = -0x3E8;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x30000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 5;
            pEsp->Unk12 = -0x50;
        }
        Game.Plight_flg = 1;
    }
    G.Status_flg &= ~0x40;
}

void func_800421F8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    UCHAR Cnt;

    pP = pEm->pSin_parts_ptr;
    Cnt = pEm->Move_cnt % 3;
    if (Cnt == 0) {
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = -0x6C;
        Sv.vy = 0x528;
        Sv.vz = 0;
        G.Unk75E8 |= 1;
        func_8001B484_800402F4(1, 0x18000000, pM, &Sv, 0);
        Sv.vy = 0x60E;
        Dsv.vz = -0x400;
        Sv.vx = -0x6C;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484_800402F4(0x801, 0x18000000, pM, &Sv, &Dsv);
        Sv.vx = -0x190;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484_800402F4(0xC02, (USHORT)(pEm->Cdir_y - 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = -0xE6;
        Sv.vy = 0x2BC;
        Sv.vz = 0;
        func_8001B484_800402F4(0x503, (USHORT)(signed char)func_800102E8_800402F4() | 0x1E000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
        Pl_weapon_vib(pEm);
    }
    if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
        Sv.vx = -0x6C;
        Sv.vy = 0x4B0;
        Sv.vz = 0;
        func_8001B484_800402F4(0x603, (USHORT)(signed char)func_800102E8_800402F4() | 0x10000000, &pP[4].Workm, &Sv, 0);
    }
    if (Cnt == 1) {
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
    }
}

void func_80042474(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        func_800455B4();
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x318;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x1EC;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x37C;
        Sv.vz = 0;
        func_8001B484_800402F4(0x703, 0x30000000, pM, &Sv, 0);
        Sv.vx = 0x97;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x8000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = -0x14;
        }
        pEsp = func_8001B484_800402F4(3, 0x8000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = 0x14;
        }
        pEsp = func_8001B484_800402F4(3, 0x10000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = -0x1E;
        }
        pEsp = func_8001B484_800402F4(3, 0x10000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = 0x1E;
        }
        Game.Plight_flg = 1;
    }
}

void func_80042660(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484_800402F4(2, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x5F;
        Sv.vy = 0x258;
        Sv.vz = 0x20;
        func_8001B484_800402F4(1, (USHORT)pEm->Cdir_y | 0x13000000, pM, &Sv, 0);
        Sv.vx = 0x5F;
        Sv.vy = 0x1F4;
        Sv.vz = 0x20;
        func_8001B484_800402F4(3, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

void func_800427D8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ESP_WORK *pEsp;
    UCHAR Cnt;

    Cnt = pEm->Move_cnt;
    pP = &pEm->pSin_parts_ptr[4];
    if (Cnt == 0 || Cnt == 4 || Cnt == 7) {
        Sv.vz = 0x14;
        pM = &pP->Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        func_8001B484_800402F4(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        func_8001B484_800402F4(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x528;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        func_8001B484_800402F4(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        func_8001B484_800402F4(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        func_8001B484_800402F4(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        func_8001B484_800402F4(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484_800402F4(0x603, (USHORT)(signed char)func_800102E8_800402F4() | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = func_8001B484_800402F4(0x603, (USHORT)(signed char)func_800102E8_800402F4() | 0x10000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_800746C0(0x10100, (VECTOR *)pP->Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        func_8006D030(1);
        Game.Plight_flg = 1;
    }
}

void func_80042AE4(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    PARTS_W *pO;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ESP_WORK *pEsp;

    if ((UCHAR)(pEm->Move_cnt % 3) == 0) {
        pP = pEm->pSin_parts_ptr;
        pO = &pP[4];
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484_800402F4(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        func_8001B484_800402F4(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x528;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        func_8001B484_800402F4(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        func_8001B484_800402F4(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        func_8001B484_800402F4(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        func_8001B484_800402F4(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Pl_weapon_vib(pEm);
        Game.Plight_flg = 1;
    }
    if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484_800402F4(0x603, (USHORT)(signed char)func_800102E8_800402F4() | 0x10000000, &pO->Workm, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = func_8001B484_800402F4(0x603, (USHORT)(signed char)func_800102E8_800402F4() | 0x10000000, &pO->Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

#define RND() ((signed char)func_800102E8_800402F4())

void func_80042E34(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ULONG n;
    ESP_WORK *pEsp;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x50;
        Sv.vy = 0x33F;
        Sv.vz = 0xD;
        func_8001B484_800402F4(1, 0x20000000, pM, &Sv, 0);
        Sv.vy = 0x407;
        Dsv.vz = -0x400;
        Sv.vx = 0x50;
        Sv.vz = 0xD;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484_800402F4(0x801, 0x1F000000, pM, &Sv, &Dsv);
        Sv.vx = 0x50;
        Sv.vy = 0x1A4;
        Sv.vz = 0xD;
        func_8001B484_800402F4(3, 0x25000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x3A3;
        Sv.vz = 0xD;
        func_8001B484_800402F4(0x703, 0x2D000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x2DB;
        Sv.vz = 0xD;
        func_8001B484_800402F4(0x901, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x3D5;
        Sv.vz = 0xD;
        func_8001B484_800402F4(0x901, (USHORT)pEm->Cdir_y | 0x13000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x50;
        Sv.vy = 0x407;
        Sv.vz = -0xBB;
        Dsv.vx = (RND() >> 1) - 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484_800402F4(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x50;
        Sv.vy = 0x407;
        Sv.vz = 0xD5;
        Dsv.vx = (RND() >> 1) + 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484_800402F4(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x118;
        Sv.vy = 0x407;
        Sv.vz = 0xD;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x4A0;
        func_8001B484_800402F4(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = -0x78;
        Sv.vy = 0x407;
        Sv.vz = 0xD;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x360;
        func_8001B484_800402F4(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x104;
        Sv.vz = 0xD;
        func_8001B484_800402F4(0x202, (USHORT)(pEm->Cdir_y + 0x800) | 0xB000000, &pP[4].Workm, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x104;
        Sv.vz = 0xD;
        pEsp = func_8001B484_800402F4(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
    if (pEm->Move_cnt == 9) {
        Pl_wep_ready_reset(pEm);
        Pad_vib_set0(2, 0);
        Pad_vib_set1(6, 0x96, 0);
    }
}

void func_80043328(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484_800402F4(0x4000002, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x1C2;
        Sv.vz = 0;
        func_8001B484_800402F4(1, (USHORT)pEm->Cdir_y | 0x16000000, pM, &Sv, 0);
        Sv.vy = 0x2DA;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484_800402F4(0x801, 0x10000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x14000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x28A;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x18000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

void func_800434F8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ULONG n;
    ESP_WORK *pEsp;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x5DC;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x2E000000, pM, &Sv, 0);
        Sv.vy = 0x78A;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484_800402F4(0x801, 0x26000000, pM, &Sv, &Dsv);
        Sv.vx = 0x82;
        Sv.vy = 0x3E8;
        Sv.vz = 0;
        func_8001B484_800402F4(0x2303, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x3E8;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484_800402F4(0x703, 0x2D000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x76C;
        Sv.vz = 0;
        func_8001B484_800402F4(0x703, 0x34000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x640;
        Sv.vz = 0;
        func_8001B484_800402F4(0x901, (USHORT)pEm->Cdir_y | 0x14000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x76C;
        Sv.vz = 0;
        func_8001B484_800402F4(0x901, (USHORT)pEm->Cdir_y | 0x1B000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = -0x12C;
        Dsv.vx = (RND() >> 1) - 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484_800402F4(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = 0x12C;
        Dsv.vx = (RND() >> 1) + 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484_800402F4(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x190;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x4A0;
        func_8001B484_800402F4(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = -0xC8;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x360;
        func_8001B484_800402F4(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
    }
    if (pEm->Move_cnt == 1 || pEm->Move_cnt == 2) {
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0x9D;
        Sv.vy = 0x1EA;
        Sv.vz = 0;
        func_8001B484_800402F4(0x5000202, (USHORT)(pEm->Cdir_y + 0x400) | 0xB000000, &pP[4].Workm, &Sv, 0);
        Sv.vx = 0xC8;
        Sv.vy = 0x1FE;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
    G.Status_flg &= ~0x40;
}

void func_800439F8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        func_800455B4();
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x318;
        Sv.vz = 0;
        func_8001B484_800402F4(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x1EC;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x10000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x4A8;
        Sv.vz = 0;
        func_8001B484_800402F4(3, 0x1D000000, pM, &Sv, 0);
        Sv.vx = 0x97;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        pEsp = func_8001B484_800402F4(3, 0x6000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = -0x14;
        }
        pEsp = func_8001B484_800402F4(3, 0x6000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = 0x14;
        }
        pEsp = func_8001B484_800402F4(3, 0xD000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = -0x1E;
        }
        pEsp = func_8001B484_800402F4(3, 0xD000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = 0x1E;
        }
        Game.Plight_flg = 1;
    }
}

extern PARTS_W *D_8009CD30;
extern USHORT D_8009DCB4[];
void func_80012818_800402F4(LONG Fno, ULONG *pAdr, LONG a2, char *pName) __asm__("func_80012818");
void func_8007809C(LONG a0, LONG a1, ULONG a2, ULONG a3);

void func_80043BE4(PLAYER_WORK *pEm)
{
    PARTS_W *pW;
    PARTS_W *pTop;
    UCHAR *pData;
    LONG *pOfs;
    ULONG Cnt;
    UCHAR No;
    ULONG *pTmd;
    TIM_IMAGE *pTim;
    TIM_IMAGE Tim;
    ULONG *p;

    pW = D_8009CD30;
    pData = (UCHAR *)&pW[4];
    func_80012818_800402F4(D_8009DCB4[pEm->Id] + (UCHAR)pEm->Type, (ULONG *)pData, 0, "WEP DATA");
    pOfs = (LONG *)(pData + *(LONG *)&pW[4]);
    No = pEm->Parts_num;
    Cnt = *(ULONG *)(pData + pOfs[7] + 4);
    pEm->Parts_num = No + Cnt;
    pEm->pSub0_seq_t_ptr = (ULONG *)(pData + pOfs[0]);
    pEm->pSub0_kan_t_ptr = (ULONG *)(pData + pOfs[1]);
    OpenTIM((ULONG *)(pData + pOfs[8]));
    pTim = ReadTIM(&Tim);
    pTim->prect->x = pEm->Tpage * 0x40 - 0x39C;
    pTim->prect->y = 0x1E0;
    pTim->prect->w = 0x1C;
    pTim->prect->h = 0x20;
    LoadImage(pTim->prect, pTim->paddr);
    DrawSync(0);
    pTim->crect->x = 0xF0;
    pTim->crect->y = pEm->Clut + 0x1E1;
    pTim->crect->w = 0x10;
    pTop = pW;
    LoadImage(pTim->crect, pTim->caddr);
    DrawSync(0);
    func_8002AB34((ULONG *)(pData + pOfs[7]), pEm->Tpage, pEm->Clut);
    G.Pl_unk180[0] = (ULONG)(pData + pOfs[2]);
    G.Pl_unk180[1] = (ULONG)(pData + pOfs[3]);
    G.Pl_unk1B4 = (ULONG)(pData + pOfs[4]);
    G.Pl_unk180[2] = (ULONG)(pData + pOfs[5]);
    G.Pl_unk180[3] = (ULONG)(pData + pOfs[6]);
    pTmd = (ULONG *)(pData + pOfs[7] + 8);
    do {
        pW->UnkA2 = 0;
        pW->pSuper = &GsIDMATRIX;
        pW->Old_x = 0;
        pW->Pos_x = 0;
        pW->Old_y = 0;
        pW->Pos_y = 0;
        pW->Old_z = 0;
        pW->Pos_z = 0;
        pW->pTmd = pTmd;
        *(ULONG *)&pW->Workm.m[0][0] = 0x1000;
        *(ULONG *)&pW->Workm.m[0][2] = 0;
        *(ULONG *)&pW->Workm.m[1][1] = 0x1000;
        *(ULONG *)&pW->Workm.m[2][0] = 0;
        *(ULONG *)&pW->Workm.m[2][2] = 0x1000;
        pW->Workm.t[0] = 0;
        pW->Workm.t[1] = 0;
        pW->Workm.t[2] = 0;
        *(ULONG *)&pW->M[0][0] = 0x1000;
        *(ULONG *)&pW->M[0][2] = 0;
        *(ULONG *)&pW->M[1][1] = 0x1000;
        *(ULONG *)&pW->M[2][0] = 0;
        *(ULONG *)&pW->M[2][2] = 0x1000;
        pW->Attribute = 0;
        pW->Poly_rgb = 0x808080;
        *(ULONG *)&pW->Cdir_x = 0;
        pW->Cdir_z = 0;
        pW->Be_flg = 0;
        pW->Unk84 = No;
        *(SHORT *)&pW->Workm.t[0] = 0;
        pW->Unk94 = 0;
        pW->Unk85 = 0;
        pW->Unk86 = 0;
        pW->Unk96 = 0;
        No++;
        pW++;
        pTmd += 6;
    } while (--Cnt != 0);
    pW = pTop;
    pTop = &pEm->pSin_parts_ptr[4];
    p = pTop->pTmd;
    pTop->pTmd = pW->pTmd;
    Pl_wep_tmd = pW->pTmd;
    pW->pTmd = p;
    Pl_weapon_init[(UCHAR)pEm->Type](pEm);
    func_8007809C(1, (UCHAR)pEm->Type, 0x801FBF00, 0x801C1814);
}

void Weapon_cls(PLAYER_WORK *pEm)
{
    ULONG *pTmd = Pl_wep_tmd;
    PARTS_W *pP = pEm->pSin_parts_ptr;
    ULONG n;

    pP[4].pTmd = pP[21].pTmd;
    n = pTmd[-1];
    pEm->Parts_num = pEm->Parts_num - n;
}

void Weapon_parts_clr(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pEm->pSin_parts_ptr[4].pTmd = pEm->pSin_parts_ptr[21].pTmd;
    pP = &pEm->pSin_parts_ptr[21];
    i = Pl_wep_tmd[-1];
    do {
        pP->Be_flg = 0;
        pP->UnkA2 = 0;
        pP++;
    } while (--i != 0);
}

void Pl_weapon_ch_init(PLAYER_WORK *pEm)
{
    pEm->pSin_parts_ptr[4].pTmd = Pl_wep_tmd;
    Pl_weapon_init[(UCHAR)pEm->Type](pEm);
}



void Non_init(PLAYER_WORK *pEm)
{
}

void Plw02_init(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0;
    pO->Pos_y = 0x96;
    pO->Pos_z = 0;
    pO->UnkA2 |= 0xB;
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", Plw_init_43FF0);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", Plw_init_44054);

void Plw_init_440AC(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 1;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0x43;
    pO->Pos_y = 0x1B9;
    pO->Pos_z = 0x44;
    pO->UnkA2 |= 0xB;
}

void Plw_init_440F4(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0;
    pO->Pos_y = 0x96;
    pO->Pos_z = 0;
    pO->UnkA2 |= 0xB;
}

void Plw_init_44134(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = -0x32;
    pO->Pos_y = 0x1B8;
    pO->Pos_z = 0xA;
    pO->UnkA2 |= 0xB;
}

void Plw_init_4417C(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 1;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = -0x1D;
    pO->Pos_y = 0x7E;
    pO->Pos_z = 0xD;
    pO->Unk96 = 0xFF;
    pO->UnkA2 |= 0xB;
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_800441CC);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80044420);

typedef struct {
    VECTOR *pPos;                      /* 0x00 */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} HIT_AREA;

LONG func_800445C8(ULONG A0, ULONG A1, HIT_AREA *pA, ENEMY_WORK *pEm)
{
    SVECTOR Rot;
    MATRIX M;
    LONG Unused[8];
    VECTOR V;
    LONG Unused2[4];

    Rot.vx = 0;
    Rot.vy = -pA->Dir;
    Rot.vz = 0;
    RotMatrix_gte(&Rot, &M);
    V.vx = pEm->Pos_x - pA->pPos->vx;
    V.vy = 0;
    V.vz = pEm->Pos_z - pA->pPos->vz;
    ApplyMatrixLV(&M, &V, &V);
    V.vx -= pA->pBox[0];
    V.vz -= pA->pBox[1];
    if ((ULONG)(pA->pBox[2] - V.vx) > (ULONG)(pA->pBox[2] * 2)) {
        return 0;
    }
    if ((ULONG)(pA->pBox[3] - V.vz) > (ULONG)(pA->pBox[3] * 2)) {
        return 0;
    }
    pEm->Unk16C = A0;
    pEm->Unk170 = A1;
    pEm->Be_flg |= 0x80000000;
    pEm->UnkC7 = pA->Id;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_8004470C);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_8004479C);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80044804);

void func_80044E90(MATRIX *pM0, MATRIX *pM1, SVECTOR *pOut)
{
    MATRIX M;
    VECTOR V;

    TransposeMatrix(pM0, &M);
    V.vx = pM1->t[0] - pM0->t[0];
    V.vy = pM1->t[1] - pM0->t[1];
    V.vz = pM1->t[2] - pM0->t[2];
    ApplyMatrixLV(&M, &V, &V);
    pOut->vx = V.vx;
    pOut->vy = V.vy;
    pOut->vz = V.vz;
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80044F34);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80045094);

void Pl_wep_eff_init(PLAYER_WORK *pEm)
{
    Pl_wep_eff_parts_init(pEm);
    func_80046C90();
}

void func_800452E0(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_800452E8);

void func_80045514(void) {
}

LONG Pl_wep_eff_parts_init(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pP = &pEm->pSin_parts_ptr[15];
    i = 6;
    do {
        pP->Unk74 = 0xFF;
        pP->Be_flg = 0;
        pP->Unk90 = 0;
        func_8001B894(0x103, &pP->Workm);
        func_8001B894(0xC03, &pP->Workm);
        func_8001B894(0x105, &pP->Workm);
        func_8001B894(0x805, &pP->Workm);
        pP++;
    } while (--i != 0);
    return 0;
}

LONG func_800455B4(PLAYER_WORK *pEm)
{
    PARTS_W *pW;
    PARTS_W *pP;
    MATRIX *pM;
    LONG i;
    SVECTOR Dsv;
    SVECTOR V;

    pW = &pEm->pSin_parts_ptr[15];
    pW->Unk90 = 12;
    if ((UCHAR)pEm->Type == 0x14) {
        pW->Unk90 = 0x14;
    }
    i = 6;
    do {
        i--;
        if (pW->Unk74 == 0xFF) {
            pW->Unk74 = 0;
            pW->Be_flg = 0x23;
            pW->Pos_x = 0xAA;
            pW->Pos_y = 0x190;
            pW->Pos_z = 0x76;
            *(ULONG *)&pW->M[0][0] = 0x1000;
            *(ULONG *)&pW->M[0][2] = 0;
            *(ULONG *)&pW->M[1][1] = 0x1000;
            *(ULONG *)&pW->M[2][0] = 0;
            *(ULONG *)&pW->M[2][2] = 0x1000;
            pW->UnkA2 |= 0x200B;
            pW->Unk8A = pEm->Cdir_y;
            if ((UCHAR)pEm->Type == 0x14) {
                Dsv.vx = 0;
                Dsv.vy = 0;
                Dsv.vz = 0;
                V.vx = 0;
                V.vy = -150;
                V.vz = 0;
                func_8001B484_800402F4(0x805, 0x4000000, &pW->Workm, &V, &Dsv);
                Dsv.vx = 0;
                Dsv.vy = 0x400;
                Dsv.vz = 0;
                func_8001B484_800402F4(0x805, 0x4000000, &pW->Workm, &V, &Dsv);
            } else {
                func_8001B484_800402F4(0x103, 0x10000000, &pW->Workm, 0, 0);
            }
            pP = pEm->pSin_parts_ptr;
            pM = &pP[4].Workm;
            pW->pSuper = pM;
            pW->pOya_parts = &pP[4];
            func_8002D4B0(pW->pSuper, (MATRIX *)pW->M, &pW->Workm);
            pW->Unk7C = pEm->Unk122;
            return 1;
        }
        pW++;
    } while (i != 0);
}

UCHAR Pl_wep_eff_parts_cnt(void)
{
    LONG i;
    UCHAR Cnt;
    PARTS_W *pP;
    UCHAR Unused[16];

    i = 6;
    Cnt = 0;
    pP = &G.Pl_pSin_parts_ptr[15];
    do {
        if (pP->Unk74 != 0xFF) {
            Cnt++;
        }
        pP++;
    } while (--i != 0);
    return Cnt;
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_800457A4);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80045950);

ULONG func_80046A0C(ENEMY_WORK *pEm, MATRIX *pM, UCHAR *pTbl)
{
    ULONG Min;
    ULONG Ret;
    ULONG d;
    LONG dy;
    LONG i;
    ULONG j;
    UCHAR *p;
    PARTS_W *pP;
    LONG sq;

    Min = 0x7FFFFFFF;
    if (pEm->Id < 0x10) {
        return 0;
    }
    i = 4;
    j = 0;
    p = &pTbl[pEm->Id * 4 - 0x40];
    do {
        pP = &pEm->pSin_parts_ptr[*p];
        sq = (pP->Workm.t[0] - pM->t[0]) * (pP->Workm.t[0] - pM->t[0]) + (pP->Workm.t[2] - pM->t[2]) * (pP->Workm.t[2] - pM->t[2]);
        dy = pP->Workm.t[1] - pM->t[1];
        d = dy < 0 ? -dy : dy;
        d += SquareRoot0(sq);
        if (d < Min) {
            Min = d;
            Ret = *p | (j << 16);
        }
        p++;
        j++;
    } while (--i != 0);
    return Ret;
}

LONG Dir_limit(LONG Dir, LONG Base, LONG Lim, SHORT *pDir)
{
    LONG d;
    SHORT Out;

    d = (Lim + (Dir - Base)) & 0xFFF;
    Base -= Lim;
    if (d < (SHORT)Lim * 2) {
        *pDir = Dir;
        return 0;
    }
    Out = Base;
    if ((ULONG)d <= 0x800) {
        Out = Base + Lim * 2;
    }
    *pDir = Out;
    return 1;
}

void func_80046B94(SVECTOR *pV, SVECTOR *pOut)
{
    SHORT Dir_y;
    SHORT Dir_z;

    Dir_y = Get_dir_xz(pV->vx, pV->vz);
    Dir_z = (Get_dir_xz(SquareRoot0(pV->vx * pV->vx + pV->vz * pV->vz), -pV->vy) - 0x400) & 0xFFF;
    pOut->vx = 0;
    pOut->vy = Dir_y;
    pOut->vz = Dir_z;
}

SHORT Get_dir_xz(LONG X, LONG Z)
{
    LONG Dir;
    LONG Base;

    Dir = 0x400;
    if (Z > 0) {
        Dir = 0xC00;
    }
    if (X != 0) {
        Dir = catan((Z << 12) / X);
        if (X < 0) {
            return (0x800 - Dir) & 0xFFF;
        }
        return (0x1000 - Dir) & 0xFFF;
    }
    return Dir;
}

void func_80046C90(void)
{
    PL_WEP_POLY *pWp;
    LONG i;
    ULONG Tp;
    ULONG Cl;

    pWp = Pl_wep_poly;
    i = 6;
    Tp = G.Pl_tpage;
    Cl = G.Pl_clut;
    do {
        pWp->Be_flg = 0;
        pWp->Unk2 = 10;
        *(ULONG *)&pWp->Poly[0].r0 = 0x808080;
        SetPolyFT4(&pWp->Poly[0]);
        SetSemiTrans(&pWp->Poly[0], 1);
        pWp->Poly[0].tpage = GetTPage(1, 1, (Tp - 14) << 6, 0x100);
        pWp->Poly[0].clut = GetClut(0, Cl + 0x1E2);
        pWp->Poly[0].u0 = 0;
        pWp->Poly[0].v0 = 0xEF;
        pWp->Poly[0].u1 = 0x10;
        pWp->Poly[0].v1 = 0xEF;
        pWp->Poly[0].u2 = 0;
        pWp->Poly[0].v2 = 0xFE;
        pWp->Poly[0].u3 = 0x10;
        pWp->Poly[0].v3 = 0xFE;
        func_800100A4(&pWp->Poly[1], &pWp->Poly[0], 0x28);
        pWp++;
    } while (--i != 0);
}

void Pl_wep_poly_set(PL_WEP_POLY *pWp, SHORT Unk2, LONG Unk4)
{
    pWp->Be_flg = 1;
    pWp->Unk4 = Unk4;
    pWp->Unk2 = Unk2;
}

void Pl_wep_poly_rgb(PL_WEP_POLY *pWp, ULONG Rgb)
{
    *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[0].r0 |= Rgb;
    *(ULONG *)&pWp->Poly[1].r0 |= Rgb;
}


void func_80046DF8(PL_WEP_POLY *pWp)
{
    pWp->Be_flg = 0;
}

void func_80046E00(PL_WEP_POLY *pWp, SVECTOR *pV)
{
    LONG d;
    ULONG Rgb;

    pWp->Be_flg = 0;
    d = (USHORT)((SVECTOR *)pWp->Unk4)->vz - (USHORT)pV[7].vx;
    if ((USHORT)(d + 0xC7F) < 0x125B) {
        pWp->Be_flg = 1;
        Rgb = *(ULONG *)&pWp->Poly[0].r0 & 0xFFFFFF;
        *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[0].r0 |= Rgb >> 1;
        *(ULONG *)&pWp->Poly[1].r0 |= Rgb >> 1;
    }
    if ((USHORT)(d + 0xA8B) < 0xE73) {
        pWp->Be_flg = 1;
        *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
        *(ULONG *)&pWp->Poly[0].r0 |= Rgb;
        *(ULONG *)&pWp->Poly[1].r0 |= Rgb;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80046EC4);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_800470F4);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_800472EC);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80047860);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80047BD0);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80047DD0);

INCLUDE_ASM("asm/nonmatchings/game/pl_wep", func_80047F70);

void Quake_add(LONG X, LONG Y)
{
    G.Unk7860 += Y;
    G.Unk2118 += Y;
    G.Unk211A += X;
    if (G.Unk2116 == 0) {
        G.Unk2116 += 0x5A;
    }
}

extern ENEMY_WORK Pl_work;
extern UCHAR D_8009DDC4[][2];
extern UCHAR D_8009DE10;
extern UCHAR D_8009DE24;

void func_8004834C(ULONG Id, LONG Flg)
{
    UCHAR n;

    n = Id - 0x10;
    if (n == 0x26 && Flg != 0) {
        return;
    }
    if ((Pl_work.Type & 0x1F) == 10) {
        Quake_add((UCHAR)(D_8009DDC4[n][0] / 3), D_8009DDC4[n][1]);
    } else {
        Quake_add(D_8009DDC4[n][0], D_8009DDC4[n][1]);
    }
    if ((Pl_work.Type & 0x1F) == 1) {
        if (n == 0x26) {
            Quake_add(D_8009DE10, 0);
        } else {
            Quake_add(D_8009DDC4[n][0], D_8009DDC4[n][1] * 7);
        }
    }
    if (D_8009DE24) {
        Quake_add(D_8009DDC4[n][0] >> 1, D_8009DDC4[n][1] >> 1);
    }
    if (D_8009DE24 >= 2) {
        Quake_add(D_8009DDC4[n][0], D_8009DDC4[n][1]);
    }
    if (D_8009DE24 >= 3) {
        Quake_add(D_8009DDC4[n][0], D_8009DDC4[n][1]);
    }
    if (!Flg) {
        D_8009DE24++;
        if (G.Unk2116 < 15) {
            G.Unk2116 += 15;
        }
    }
}
