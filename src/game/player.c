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

INCLUDE_ASM("asm/nonmatchings/game/player", func_80038AAC);

void func_80045094(ENEMY_WORK *pEm);
void func_80045950(ENEMY_WORK *pEm);
void Pl_wep_recoil_6(ENEMY_WORK *pEm);
void Pl_wep_recoil_10(ENEMY_WORK *pEm);
void Pl_wep_spin_0b(ENEMY_WORK *pEm);
void func_8003DEA4(ENEMY_WORK *pEm, ULONG Lim, SHORT Dir);
void func_8001A248(ENEMY_WORK *pEm, SHORT Dir);
LONG func_800102E8(void);
void func_8001B484(ULONG Arg0, ULONG Arg1, MATRIX *pM, ULONG Arg3, ULONG Arg4);

void func_80038C7C(ENEMY_WORK *pEm)
{
    SVECTOR sv;
    SVECTOR sv2;
    PARTS_W *pParts;

    if ((LONG)G.Unk2468 < 0) {
        return;
    }
    G.Status_flg &= ~0x40;
    G.Unk75E8 &= 0xFFE0;
    if (pEm->Damage_cnt & 0x7F) {
        pEm->Damage_cnt--;
        pEm->Be_flg |= 0x10000000;
    }
    if (pEm->Damage_cnt & 0x80) {
        pEm->Unk132 &= 0x7FFF;
    }
    Pl_routine[pEm->Routine_0](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
    func_80045094(pEm);
    func_80045950(pEm);
    Pl_wep_recoil_6(pEm);
    Pl_wep_recoil_10(pEm);
    Pl_wep_spin_0b(pEm);
    if (pEm->Water != 0 && pEm->Water < pEm->Unk122 - 500) {
        pParts = pEm->pSin_parts_ptr;
        sv.vx = pParts->Workm.t[0];
        sv.vy = pEm->Water;
        sv.vz = pParts->Workm.t[2];
        sv2.vx = 0x400;
        sv2.vy = 0;
        sv2.vz = 0;
        if ((pEm->Move_cnt & 3) == 2 && (USHORT)(pEm->Spd_x + 20) > 40) {
            func_8001B484(0x20E, ((func_800102E8() * 8 + 0x2000) << 16) | (USHORT)pEm->Cdir_y, &GsIDMATRIX, (ULONG)&sv, (ULONG)&sv2);
        }
    }
    func_8003DEA4(pEm, 7000, 1500);
    func_8001A248(pEm, pEm->Cdir_y);
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80038E50);

void Kage_work9_set(KAGE_WORK9 **ppKage, ULONG Arg1, ULONG Arg2, ULONG Arg3, VECTOR *pPos);

void func_80038F98(ENEMY_WORK *pEm)
{
    if (!(*((UCHAR *)&G + 0x2912) & 1)) {
        Kage_work9_set(&pEm->pKage_work, 0, 0x022601C2, 0, (VECTOR *)&pEm->Pos_x);
    }
    EM_SHORT(pEm, 0xA) = 0;
    pEm->Unk132 = 0;
    EM_SHORT(pEm, 0x144) = 0;
    EM_SHORT(pEm, 0x150) = 0;
    EM_SHORT(pEm, 0x152) = 0;
    EM_SHORT(pEm, 0x1B8) = 0;
    EM_SHORT(pEm, 0x1BA) = 0;
    EM_SHORT(pEm, 0x1BC) = 0;
    EM_SHORT(pEm, 0x1BE) = 0;
    EM_UCHAR(pEm, 0x1C0) = 0;
    EM_UCHAR(pEm, 0x1C1) = 0;
    EM_SHORT(pEm, 0x1C2) = 0;
    EM_SHORT(pEm, 0x1CA) = 0;
    EM_SHORT(pEm, 0x1C8) = 0;
}

void Pl_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Pl_mv_pad[pEm->Routine_1](pEm, G.Key, G.Key_trg);
    Pl_mv[pEm->Routine_1](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
    if ((pEm->Status_flg & 0x200) && !(G.Unk2468 & 0x1000000)) {
        if (--G.Pl_poison_timer == -1) {
            G.Pl_poison_timer = 30;
            if (pEm->Life >= 2) {
                pEm->Life--;
            }
        }
    }
}

void func_8003910C(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (Key & 0xA) {
        *(ULONG *)&pEm->Routine_0 = 0x401;
    }
    if (Key & 4) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
        if (Key_trg & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x601;
        }
    }
    if (Key & 1) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        if (Key & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80039294);

extern UCHAR D_8009CDEC[];

void func_8003957C(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 1)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
            if (Key_trg & 0x200) {
                *(ULONG *)&pEm->Routine_0 = 0x601;
            }
        }
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xA01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20A01;
        }
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDEC[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDEC[D_8009CD3C];
    }
    if (Key & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if (D_800D1F2C & 0x10) {
        *(ULONG *)&pEm->Routine_0 = 0x901;
    }
}

void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
extern UCHAR D_8009CDE0[];
void func_800776B0(ULONG Arg0, ULONG No, ENEMY_WORK *pEm);

void func_800397DC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    SHORT spd_x;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 3];
        pEm->Routine_2 = 1;
        n += 0x70000;
        *(ULONG *)&pEm->Move_no = n;
        G.Status_flg &= ~0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(0, m * 3 + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 2;
        }
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Water < pEm->Unk122 - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

extern UCHAR D_8009CDF0[];

void func_80039924(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;
    ULONG v;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if ((Key & 0x201) != 0x201) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        }
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xF01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20F01;
        }
    }
    if (pEm->Spd_x >= 0x33) {
        if (Key & 2) {
            pEm->Cdir_y += D_8009CDF0[D_8009CD3C];
        }
        if (Key & 8) {
            pEm->Cdir_y -= D_8009CDF0[D_8009CD3C];
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x701;
        }
    }
    if (D_800D1F2C & 0x10) {
        *(ULONG *)&pEm->Routine_0 = 0x901;
    }
}

void func_80039B84(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;
    SHORT spd_x;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 6];
        pEm->Routine_2 = 1;
        n += 0x70000;
        *(ULONG *)&pEm->Move_no = n;
        G.Status_flg &= ~0x100;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(1, (m << 1) + m + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 4;
        }
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->Water < pEm->Unk122 - 500) {
        spd_x = pEm->Spd_x;
        pEm->Spd_x = spd_x - spd_x / 3;
    }
    Add_speedXZ(pEm, 0);
}

extern UCHAR D_8009CDF4[];

void func_80039CCC(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 4)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        }
        return;
    }
    if (pEm->UnkAE & 0x40) {
        if (func_8003C390(pEm, 0) & 0x1000) {
            *(ULONG *)&pEm->Routine_0 = 0xE01;
        } else {
            *(ULONG *)&pEm->Routine_0 = 0x20E01;
        }
    }
    if (Key_trg & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x601;
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDF4[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDF4[D_8009CD3C];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80039F08);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003A1E0);

extern UCHAR D_8009CDF8[];

void func_8003A398(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    LONG prev;
    LONG life;

    if (pEm->Routine_2 == 0 || pEm->Move_cnt == 0) {
        life = pEm->Life;
        prev = D_8009CD3C;
        D_8009CD3C = 0;
        if (life < 0x65) {
            D_8009CD3C = 1;
        }
        if (pEm->Status_flg & 0x300) {
            D_8009CD3C = 1;
        }
        if (life < 0x15) {
            D_8009CD3C = 2;
        }
        if (prev != D_8009CD3C) {
            pEm->Routine_2 = 0;
        }
    }
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 0xA)) {
        *(ULONG *)&pEm->Routine_0 = 1;
    }
    if (Key & 4) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    }
    if (Key & 1) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
        if (Key & 0x200) {
            *(ULONG *)&pEm->Routine_0 = 0x201;
        }
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDF8[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDF8[D_8009CD3C];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

extern UCHAR D_8009CDE0[];
void func_800776B0(ULONG Arg0, ULONG No, ENEMY_WORK *pEm);

void func_8003A574(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 3];
        pEm->Routine_2 = 1;
        n += 0x70000;
        *(ULONG *)&pEm->Move_no = n;
        G.Status_flg &= ~0x100;
        pEm->Spd_x = 0x15;
    case 1:
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(0, m * 3 + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 2;
        }
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
}

void pl_mv06_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (G.Unk2468 & 0x1000000) {
        *(ULONG *)&pEm->Routine_0 = 1; /* Routine_0 = 1, Routine_1..3 = 0 */
    }
}

extern UCHAR D_8009CDE0[];
void func_800776B0(ULONG Arg0, ULONG No, ENEMY_WORK *pEm);

void func_8003A688(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        n = D_8009CDE0[D_8009CD3C + 6];
        pEm->Routine_2 = 1;
        EM_UCHAR(pEm, 0xE2) = 8;
        pEm->Spd_x = 0x32;
        pEm->Spd_z = 0;
        n += 0xF0000;
        *(ULONG *)&pEm->Move_no = n;
        Get_speedXZ(pEm, 0x700, &pEm->Spd_x, &pEm->Spd_z);
        G.Status_flg &= ~0x100;
        G_ARR(USHORT, 0x75E8)[0] |= 4;
        func_800776B0(0, 11, pEm);
    case 1:
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x100);
        pEm->Cdir_y += 0x100;
        if (--*((signed char *)pEm + 0xE2) == 0) {
            *(ULONG *)&pEm->Routine_0 = 1;
            G_ARR(USHORT, 0x75E8)[0] |= 4;
            func_800776B0(0, 11, pEm);
        }
        pEm->Pos_x += pEm->Spd_x;
        pEm->Pos_z += pEm->Spd_z;
        break;
    }
}

void pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003A7D8);

void pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003B080);

void func_8003B1C4(ENEMY_WORK *pEm, ULONG Flg)
{
    if ((Flg & 0x10) && (D_800D1F2C & 0x10)) {
        return;
    }
    if (pEm->Routine_2 == 5) {
        pEm->Routine_2 = 6;
        pEm->Move_no = 6;
        pEm->Move_cnt = 0;
        *((UCHAR *)pEm + 0xCA) = 7;
        func_800746C0(0x10200, (VECTOR *)&pEm->Pos_x, 0, 0);
        Pad_vib_init();
    }
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003B244);

void func_8003B4FC(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (!(Key & 1)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0xE01;
            if (Key_trg & 0x200) {
                *(ULONG *)&pEm->Routine_0 = 0x601;
            }
        }
        return;
    }
    if (!(pEm->UnkAE & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x101;
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDEC[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDEC[D_8009CD3C];
    }
    if (!(func_8003C390(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if (Key & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

LONG func_80026BE8(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079690(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
void func_80079868(ENEMY_WORK *pEm, LONG a1);

void func_8003B784(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(func_8003C390(pEm, 1) & 0x1000)) {
            goto case2;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 0);
    case 1:
        Spd_get(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(1, (m << 1) + m + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 4;
        }
        func_80026BE8(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2620)[0], G_ARR(LONG, 0x261C)[0], 0x200);
        func_80079868(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        *(ULONG *)&pEm->Move_no = D_8009CDE0[D_8009CD3C + 3] + 0x70000;
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(1, (m << 1) + m + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 4;
        }
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_8003C390(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_8003C390(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void func_8003B9FC(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if (Key_trg & 0x80) {
        G.Status_flg |= 0x800000;
        return;
    }
    if (!(Key & 4)) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0xA01;
        }
        return;
    }
    if (!(func_8003C390(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt = pEm->Move_cnt * 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if (!(pEm->UnkAE & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x301;
    }
    if (Key_trg & 0x200) {
        *(ULONG *)&pEm->Routine_0 = 0x601;
    }
    if (Key & 2) {
        pEm->Cdir_y += D_8009CDF4[D_8009CD3C];
    }
    if (Key & 8) {
        pEm->Cdir_y -= D_8009CDF4[D_8009CD3C];
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void func_8003BCA4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(func_8003C390(pEm, 1) & 0x1000)) {
            goto case2;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 1);
    case 1:
        Spd_get(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 1);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n == 0;
            func_800776B0(1, (m << 1) + m + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 4;
        }
        func_80026BE8(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 0x10200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2620)[0], G_ARR(LONG, 0x261C)[0], 0x10200);
        func_80079868(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        *(ULONG *)&pEm->Move_no = 0x70009;
        if (D_8009CD3C != 0) {
            *(ULONG *)&pEm->Move_no = 0x7000A;
        }
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(1, (m << 1) + m + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 4;
        }
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_8003C390(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_8003C390(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

LONG func_8003C5B0(ENEMY_WORK *pEm, ULONG Flg);

void func_8003BF28(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
    if ((Key & 0x201) != 0x201) {
        *(ULONG *)&pEm->Routine_0 = 1;
        if (Key & 0xA) {
            *(ULONG *)&pEm->Routine_0 = 0x401;
        }
        if (Key & 4) {
            *(ULONG *)&pEm->Routine_0 = 0x301;
        }
        if (Key & 1) {
            *(ULONG *)&pEm->Routine_0 = 0x101;
        }
        return;
    }
    if (!(pEm->UnkAE & 0x40)) {
        *(ULONG *)&pEm->Routine_0 = 0x201;
    }
    if (pEm->Spd_x >= 0x33) {
        if (Key & 2) {
            pEm->Cdir_y += D_8009CDF0[D_8009CD3C];
        }
        if (Key & 8) {
            pEm->Cdir_y -= D_8009CDF0[D_8009CD3C];
        }
    }
    if (!(func_8003C5B0(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else {
        if (pEm->Routine_2 == 3) {
            *(USHORT *)&pEm->Routine_2 = 0;
        }
    }
    if ((Key & 0x100) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
    if ((Key & 0x400) && (UCHAR)pEm->Type != 0) {
        *(ULONG *)&pEm->Routine_0 = 0x501;
        if ((LONG)pEm->Be_flg < 0 && pEm->Life >= 0x15) {
            *(ULONG *)&pEm->Routine_0 = 0x20701;
        }
    }
}

void func_8003C104(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        if (!(func_8003C5B0(pEm, 1) & 0x1000)) {
            goto case2;
        }
        pEm->Routine_2 = 1;
        Spd_get(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 0);
    case 1:
        Spd_get(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 0);
        if (pEm->Move_cnt == 0) {
            pEm->Spd_x = 30;
        }
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(1, (m << 1) + m + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 4;
        }
        func_80026BE8(pEm, G_ARR(LONG, 0x2628)[0], G_ARR(LONG, 0x2624)[0], 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(G_ARR(ENEMY_WORK, 0x248C), G_ARR(LONG, 0x2620)[0], G_ARR(LONG, 0x261C)[0], 0x200);
        func_80079868(pEm, 0x200);
        pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x * 4) / 3;
        break;
    case 2:
    case2:
        *(ULONG *)&pEm->Move_no = D_8009CDE0[D_8009CD3C + 6] + 0x70000;
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, (ULONG)pEm->pSub0_kan_t_ptr, (ULONG)pEm->pSub0_seq_t_ptr, 0);
        Flg = *pEm->pNow_seq;
        if (Flg & 0x200) {
            ULONG n;
            ULONG m;
            n = Flg & 0x400;
            m = n != 0;
            func_800776B0(1, (m << 1) + m + 11, pEm);
            G_ARR(USHORT, 0x75E8)[0] |= 4;
        }
        func_80027940(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0x200);
        break;
    }
    if (pEm->UnkAE & 0x20) {
        Goto01(pEm, func_8003C5B0(pEm, 0), 0x10);
    } else {
        Goto01(pEm, func_8003C5B0(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

extern USHORT D_8009CDFC[];
extern UCHAR D_8009CF78[];

ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Flg)
{
    USHORT dir;
    ULONG d;
    ULONG mot;
    USHORT r;

    dir = D_8009CDFC[EM_UCHAR(pEm, 0xB9) & 7];
    d = (dir - (USHORT)pEm->Cdir_y) & 0xFFF;
    r = dir;
    if (d - 0x400 > 0x800) {
        if (d - 0x258 > 0xB50) {
            if (Flg) {
                pEm->Move_cnt = pEm->Move_cnt * 34;
                pEm->Move_cnt = pEm->Move_cnt / 30;
                if (pEm->Move_cnt >= 3) {
                    pEm->Move_cnt -= 3;
                } else {
                    pEm->Move_cnt += 31;
                }
                mot = 0xC;
                if (D_8009CD3C == 0) {
                    mot = (D_8009CF78[(UCHAR)pEm->Type] & 3) << 2;
                }
                *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
                *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70000;
            }
            return (SHORT)r + 0x1000;
        }
        return (SHORT)dir;
    }
    if (d - 0x5A9 < 0x4AF) {
        if (Flg) {
            pEm->Move_cnt = pEm->Move_cnt * 34;
            pEm->Move_cnt = pEm->Move_cnt / 30;
            if (pEm->Move_cnt >= 4) {
                pEm->Move_cnt -= 2;
            } else {
                pEm->Move_cnt += 30;
            }
            mot = 0xD;
            if (D_8009CD3C == 0) {
                mot = ((D_8009CF78[(UCHAR)pEm->Type] & 3) << 2) | 1;
            }
            *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
            *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70001;
        }
        return (((SHORT)r + 0x800) & 0xFFF) | 0x1000;
    }
    return ((SHORT)dir + 0x800) & 0xFFF;
}

extern USHORT D_8009CDFC[];
extern UCHAR D_8009CF78[];

LONG func_8003C5B0(ENEMY_WORK *pEm, ULONG Flg)
{
    USHORT dir;
    ULONG d;
    ULONG mot;
    USHORT r;

    dir = D_8009CDFC[EM_UCHAR(pEm, 0xB9) & 7];
    d = (dir - (USHORT)pEm->Cdir_y) & 0xFFF;
    r = dir;
    if (d - 0x400 > 0x800) {
        if (d - 0x258 > 0xB50) {
            if (Flg) {
                mot = 0xE;
                if (D_8009CD3C == 0) {
                    mot = ((D_8009CF78[(UCHAR)pEm->Type] & 3) << 2) | 2;
                }
                *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
                *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70002;
            }
            return (SHORT)r + 0x1000;
        }
        return (SHORT)dir;
    }
    if (d - 0x5A9 < 0x4AF) {
        if (Flg) {
            mot = 0xF;
            if (D_8009CD3C == 0) {
                mot = ((D_8009CF78[(UCHAR)pEm->Type] & 3) << 2) | 3;
            }
            *(ULONG *)&pEm->Move_no = (pEm->Move_cnt << 8) + (mot | 0x70000);
            *(ULONG *)&pEm->Unk1A0 = (pEm->Move_cnt << 8) + 0x70003;
        }
        return (((SHORT)r + 0x800) & 0xFFF) | 0x1000;
    }
    return ((SHORT)dir + 0x800) & 0xFFF;
}

void pl_mv0b_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003C740);

void pl_mv0c_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003CA88);

void pl_mv0d_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg)
{
}

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

void func_8003CEA0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    HIT_REQ2 Req;
    ENEMY_WORK *pT;
    LONG hit;
    LONG *p;
    ULONG no;

    pT = pEm->pEnemy_ptr;
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x30012;
        pEm->Routine_2 = 1;
        func_800746C0(0x1000B, (VECTOR *)&pEm->Pos_x, 0, 0);
        G.Status_flg |= 0x100;
        G.Unk75E8 |= 0x1C;
        G.Unk785E++;
        Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x100);
        pEm->UnkE3 = 0;
        pEm->Unk120 |= 0x12;
        if (!(G.Key & 0x500)) {
            pEm->UnkE3 = 1;
        }
        if (G.System_flg & 0x80) {
            Quake_add(2, 1);
        }
    case 1:
        Spd_get(pEm, Kan, Seq, 0);
        if (func_80027940(pEm, (ULONG *)Kan, (ULONG *)Seq, 0x400)) {
            if (G.System_flg & 0x80) {
                Quake_add(2, 1);
            }
            *(ULONG *)&pEm->Routine_0 = 0x60501;
            p = &pT->Pos_x;
            pEm->Unk16C = (ULONG)p;
            pEm->Unk170 = (ULONG)p;
            pEm->pUnk15C = p;
            if ((signed char)pEm->UnkE3 != 0 && !(G.Key & 0x500)) {
                *(ULONG *)&pEm->Routine_0 = 1;
            }
            if ((UCHAR)pEm->Type == 0) {
                *(ULONG *)&pEm->Routine_0 = 1;
            }
            return;
        }
        pEm->Damage_cnt = 0;
        if (*pEm->pNow_seq & 0x400) {
            pEm->Damage_cnt = 0x80;
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0x100);
        }
        hit = 0;
        if (*pEm->pNow_seq & 0x200) {
            Req.pPos = pEm->pSin_parts_ptr->Workm.t;
            Req.W = 1000;
            Req.H = 0;
            Req.D = 0;
            Req.R = pEm->Cdir_y;
            hit = func_800472EC_h(&Req, &D_8009CE0C);
            pEm->Damage_cnt |= 0x80;
        }
        if (hit) {
            no = 3;
            if ((ULONG)(D_800BA6E8[3] - 1) < 30) {
                no = 0x13;
            }
            func_800746C0(no | 0x10300, (VECTOR *)&pEm->Pos_x, 0, 0);
        }
    default:
        Add_speedXZ(pEm, 0);
    }
}

void Pl_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_dm[pEm->Routine_1](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
}

void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);

void func_8003D1A8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    switch (pEm->Routine_2) {
    case 0:
        *(ULONG *)&pEm->Move_no = 0x30004;
        pEm->Routine_2 = 1;
        func_800746C0(0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Damage_cnt = 0x80;
        Pad_vib_set0(5, 0);
        Pad_vib_set1(8, 0x96, 0);
        pEm->Be_flg &= ~0x8000;
        EM_UCHAR(pEm, 0x120) |= 0x12;
    case 1:
        Spd_get(pEm, Kan, Seq, 0);
        if (func_80027940(pEm, (ULONG *)Kan, (ULONG *)Seq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            G.Status_flg &= ~0x100;
            EM_UCHAR(pEm, 0x120) &= 0xED;
        }
        Add_speedXZ(pEm, 0);
        break;
    }
}

void func_8003D2D8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    USHORT Flg;
    ULONG n;
    ULONG m;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        *(ULONG *)&pEm->Move_no = (pEm->Routine_1 & 1) + 0x30002;
        func_800746C0(((pEm->Routine_1 & 1) + 1) | 0x10000, (VECTOR *)&pEm->Pos_x, 0, 0);
        pEm->Damage_cnt = 0x80;
        Pad_vib_set0(5, 0);
        Pad_vib_set1(8, 200, 0);
        pEm->Be_flg &= ~0x8000;
        *(LONG *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 400 - 200;
        if (pEm->Routine_1 >= 8) {
            *(LONG *)&pEm->Spd_x = (pEm->Routine_1 & 1) * 600 - 300;
        }
        pEm->Spd_z = 0;
        pEm->Unk120 |= 0x12;
    case 1:
        if (pEm->UnkCA != 0) {
            Add_speedXZ(pEm, 0);
        }
        if (func_80027940(pEm, (ULONG *)Kan, (ULONG *)Seq, 0x400)) {
            *(ULONG *)&pEm->Routine_0 = 1;
            pEm->Damage_cnt = 0;
            G.Status_flg &= ~0x100;
            pEm->Unk120 &= 0xED;
        }
        break;
    }
    Flg = *pEm->pNow_seq;
    if (Flg & 0x200) {
        n = Flg & 0x400;
        m = n != 0;
        func_800776B0(1, m * 14, pEm);
        G_ARR(USHORT, 0x75E8)[0] |= 4;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003D4C0);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003D780);

void Pl_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    G.Status_flg |= STAT_PL_BUSY;
    Pl_die_t[pEm->Routine_1](pEm, (ULONG)pEm->pKan_t_ptr, (ULONG)pEm->pSeq_t_ptr);
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8003DA3C);

void func_8003DC0C(void) {
}

void Pl_Em_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEm->Be_flg &= ~0x8000;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldamage_Em[pEnemy->Id - 0x10](pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

void Pl_Em_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    ENEMY_WORK *pEnemy;

    G.Status_flg |= STAT_PL_BUSY;
    pEnemy = pEm->pEnemy_ptr;
    G.pPldie_Em[pEnemy->Id - 0x10](pEm, (ULONG)pEm->pSub1_kan_t_ptr, (ULONG)pEm->pSub1_seq_t_ptr);
}

void Pl_dead(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    LONG routine1;
    KAGE_WORK9 *pKage;

    routine1 = pEm->Routine_1;
    pKage = pEm->pKage_work;
    if (routine1 != 1) {
        if (1 < routine1) {
            return;
        }
        if (routine1 != 0) {
            return;
        }
        pEm->Routine_1 = 1;
        Kage_work9_color_set(pKage, 0xBFBF10);
        pEm->Timer0 = 0x3C;
    }
    pKage->Half_x += 8;
    pKage->Half_z += 8;
    if (--pEm->Timer0 == -1) {
        pEm->Routine_1 = 2;
    }
}

ULONG Pl_life_down(LONG Down_life, ULONG Mode)
{
    ENEMY_WORK *pPl = &Pl_work;
    LONG Life;

    if ((ULONG)(Down_life - 10) < 0x5B) {
        Down_life += 10;
    }
    Life = (USHORT)pPl->Life - Down_life;
    pPl->Life = Life;
    pPl->Unk132 &= 0xE0FF;
    if (Mode == 1) {
        if ((SHORT)Life < 0) {
            pPl->Life = 0;
            pPl->Status_flg |= 0x800;
            return 1;
        }
        return 0;
    }
    if (Mode == 0) {
        if ((SHORT)Life < 0) {
            if ((SHORT)Life >= -10 && !(pPl->Status_flg & 0x800)) {
                pPl->Life = 0;
                pPl->Status_flg |= 0x800;
                return 1;
            }
            pPl->Damage_cnt |= 0x80;
            G.System_flg |= SYS_GAME_OVER;
            return 2;
        }
        return 0;
    }
    /* Mode >= 2: no return value (v0 still holds 1 from the Mode compare) */
}

void Pl_life_up(LONG Up_life)
{
    Pl_work.Life += Up_life;
    Pl_work.Status_flg &= ~0x800;
    if (Pl_work.Max_life < Pl_work.Life) {
        Pl_work.Life = Pl_work.Max_life;
    }
}
