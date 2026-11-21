#ifndef OVL_EM30_H
#define OVL_EM30_H
/*
 * Enemy module em30 (enemy id 0x30). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2). A large 17-part body (Type & 1 == 0) that spawns seven
 * collision sub-works (Type & 1) tied to its parts. Routine table
 * Init/Move/Damage/Die as RE2's EM2x modules; Move is the RE2 mv_br[]/mv[]
 * pair indexed by Routine_1, Damage is indexed by the hit weapon (Unk12C),
 * Die by Unk12C too. The die handler for the player is hooked into
 * G.pPldie_Em[Id - 0x10].
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "player.h"
#include "sca.h"
#include "pad.h"
#include "sub.h"
#include "main.h"
#include "pl_wep.h"

/* the body's private work in ENEMY_WORK.Unk1CC[] (RE2 Free[]); runs past 0x1FC */
typedef struct tagEM30_FREE {
    UCHAR Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk01;                       /* 0x01 (0x1CD) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    UCHAR Unk03;                       /* 0x03 (0x1CF) */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    signed char Unk0A;                 /* 0x0A (0x1D6) */
    signed char Unk0B;                 /* 0x0B (0x1D7) */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    signed char Unk0E;                 /* 0x0E (0x1DA) */
    signed char Unk0F;                 /* 0x0F (0x1DB) */
    signed char Unk10;                 /* 0x10 (0x1DC) */
    signed char Unk11;                 /* 0x11 (0x1DD) */
    signed char Unk12;                 /* 0x12 (0x1DE) */
    UCHAR Unk13;                       /* 0x13 (0x1DF) */
    SHORT Unk14;                       /* 0x14 (0x1E0) */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    signed char Unk18;                 /* 0x18 (0x1E4) */
    signed char Unk19;                 /* 0x19 (0x1E5) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    UCHAR pad1C[0x20 - 0x1C];
    SHORT Unk20;                       /* 0x20 (0x1EC) */
    SHORT Unk22;                       /* 0x22 (0x1EE) */
    SHORT Unk24;                       /* 0x24 (0x1F0) */
    SHORT Unk26;                       /* 0x26 (0x1F2) */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    UCHAR pad2A[0x2E - 0x2A];
    USHORT Unk2E;                      /* 0x2E (0x1FA) last distance (mv[3]) */
    UCHAR Unk30;                       /* 0x30 (0x1FC) */
    UCHAR Unk31;                       /* 0x31 (0x1FD) */
} EM30_FREE;

/* a collision sub-work's private work: which part of which body it follows */
typedef struct tagEM30_SUB_FREE {
    LONG Parts_no;                     /* 0x00 (0x1CC) part of the body */
    LONG Oya_no;                       /* 0x04 (0x1D0) index of the body in G.pUnk265C[] */
} EM30_SUB_FREE;

#define G_PLAYER ((ENEMY_WORK *)&G.Pl_be_flg)
#define EM30_W(p) ((EM30_FREE *)(p)->Unk1CC)
#define EM30_SUB(p) ((EM30_SUB_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x201) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
#define PW_FREE_L(p) ((ULONG *)&(p)->Unk74)

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c func_8004AC54) */
typedef struct tagEM30_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C + 4 * key */
        USHORT pad;
    } Key[3];
} EM30_SABUN;
#define EM30_SABUN_P(pP) ((EM30_SABUN *)(pP)->pUnk98)

/* G viewed with the work pointer table as one array (em_sub.c G_EMTBL: [0] player, [1] sub, [2..] enemies) */
typedef struct tagEM30_G {
    UCHAR pad0[0x213C];
    ENEMY_WORK *pEm_mem;               /* 0x213C next free work */
    UCHAR pad2140[0x2487 - 0x2140];
    UCHAR Em_num;                      /* 0x2487 */
    UCHAR pad2488[0x265C - 0x2488];
    ENEMY_WORK *pEm_tbl[42];           /* 0x265C */
    ENEMY_WORK **ppEnemy_end;          /* 0x2704 */
} EM30_G;
#define EM30_GW ((EM30_G *)&G)

/* hit box test around a point (pl_wep.c func_800445C8) */
typedef struct tagHIT_AREA {
    VECTOR *pPos;                      /* 0x00 */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} HIT_AREA;

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
LONG Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG Area, LONG Flg);         /* root_ck.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, frame-end flag */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);         /* em_sub.c */
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);                        /* em_sub.c */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
UCHAR func_80033580(void *pObj, ULONG Id);                              /* oba.c */
void func_80034124(COL_OBJ *pObj);                                      /* oba.c */
LONG SquareRoot0(LONG a);
LONG func_800336B4(ENEMY_WORK *pEm);                                    /* oba.c: collision index of a work */                                               /* libgte */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);         /* snd.c */
void func_8004AC54(ENEMY_WORK *pEm, PARTS_W *pP);                       /* sabun.c: apply the morph keys */
LONG func_800445C8(LONG *pA0, LONG *pA1, HIT_AREA *pA, ENEMY_WORK *pEm);  /* pl_wep.c: pEm inside the box */
LONG func_80044420(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);         /* pl_wep.c */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");          /* em_sub.c, UCHAR result used as int */
LONG Water_ck(LONG Pos_x, LONG Pos_z);                                  /* sce_at.c */
extern UCHAR *D_800BA6E8;                                               /* [3]: sound bank id of the room */

#endif /* OVL_EM30_H */
