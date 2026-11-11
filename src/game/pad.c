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

INCLUDE_ASM("asm/nonmatchings/game/pad", Pad_set);

typedef struct {
    UCHAR pad0[4];
    SHORT Unk4;                        /* 0x04 */
    UCHAR Num;                         /* 0x06 */
    UCHAR pad7;
    UCHAR Tbl[16][2];                  /* 0x08 */
} PAD_WORK_800DE628;
extern PAD_WORK_800DE628 D_800DE628;

void func_80038634(ULONG Flg, SHORT Val)
{
    PAD_WORK_800DE628 *pW;
    UCHAR *p;
    LONG i;
    LONG n;

    pW = &D_800DE628;
    pW->Unk4 = Val;
    p = pW->Tbl[0];
    i = 0;
    n = 0;
    do {
        if (Flg & 1) {
            *p = i;
            p += 2;
            n++;
        }
        Flg >>= 1;
        i++;
    } while (Flg != 0);
    pW->Num = n;
}

void Pad_vib_set0(LONG Time, USHORT Attr)
{
    VIB_REQ *pReq = Vib_req0;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur0 = Pad_vib_get_free(pReq);
        Vib_cur0->Flg = 1;
        Vib_cur0->Time = Time;
        Vib_cur0->Pow = (Time != 0);
        Vib_cur0->Delay = Attr;
        Vib_cur0->Step = 0;
    }
}

void Pad_vib_set1(LONG Time, ULONG Power, ULONG Attr)
{
    VIB_REQ *pReq = Vib_req1;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur1 = Pad_vib_get_free(pReq);
        Vib_cur1->Flg = 1;
        Vib_cur1->Time = Time;
        Vib_cur1->Pow = Power;
        Vib_cur1->Delay = Attr;
        Vib_cur1->Step = 0;
    }
}

void Pad_vib_fade_set(LONG Time, LONG Pow_st, LONG Pow_ed, ULONG Attr)
{
    VIB_REQ *pReq = Vib_req1;

    if ((ULONG)Pad_vib_count(pReq) < 32 && Time != 0) {
        Vib_cur1 = Pad_vib_get_free(pReq);
        Vib_cur1->Flg = 1;
        Vib_cur1->Time = Time;
        Vib_cur1->Delay = Attr;
        Vib_cur1->Pow_cur = Pow_st << 7;
        Vib_cur1->Step = (Pow_ed - Pow_st) * 0x80 / Time;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/pad", func_8003884C);

void Pad_vib_init(void)
{
    ULONG i;

    for (i = 0; i < 33; i++) {
        Vib_req0[i].Flg = 0;
        Vib_req1[i].Flg = 0;
    }
    Vib_cur0 = Vib_req0;
    Vib_cur1 = Vib_req1;
    PadActData[1] = 0;
    PadActData[0] = 0;
}

UCHAR SetPadActData(VIB_REQ *pReq)
{
    ULONG i;
    UCHAR Max = 0;
    UCHAR *pB;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg == 0) {
            continue;
        }
        pB = &pReq->Pow;
        if (*(USHORT *)(pB + 1) != 0) {
            (*(USHORT *)(pB + 1))--;
            continue;
        }
        if (--(*(USHORT *)(pB + 3)) == 0) {
            pReq->Flg = 0;
        }
        if (*(SHORT *)(pB + 5) != 0) {
            *(USHORT *)(pB + 7) += *(USHORT *)(pB + 5);
            *pB = *(USHORT *)(pB + 7) >> 7;
        }
        if (Max < *pB) {
            Max = *pB;
        }
    }
    return Max;
}

VIB_REQ *Pad_vib_get_free(VIB_REQ *pReq)
{
    ULONG i;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg == 0) {
            break;
        }
    }
    return pReq;
}

LONG Pad_vib_count(VIB_REQ *pReq)
{
    ULONG i;
    LONG Cnt = 0;

    for (i = 0; i < 32; i++, pReq++) {
        if (pReq->Flg != 0) {
            Cnt++;
        }
    }
    return Cnt;
}
