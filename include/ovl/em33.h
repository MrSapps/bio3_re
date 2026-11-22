#ifndef OVL_EM33_H
#define OVL_EM33_H
/*
 * Enemy module em33 (enemy id 0x33): a segmented giant worm. The head work
 * (Type & 1 clear) allocates seven body works (Type & 1 set) that follow
 * parts of the head's model and carry their own collision nodes and shadows.
 * RE2's EM33.C (licker family) is unrelated, so names are RE3 only.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "player.h"
#include "main.h"
#include "sub.h"
#include "pad.h"
#include "pl_wep.h"

/* free work of the head at 0x1CC */
typedef struct tagEM33_FREE {
    UCHAR Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk01;                       /* 0x01 (0x1CD) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    UCHAR Morph_dir;                   /* 0x03 (0x1CF) morph key 0 direction (func_8010183C) */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    signed char Unk0A;                 /* 0x0A (0x1D6) */
    signed char Unk0B;                 /* 0x0B (0x1D7) */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    signed char Col_id;                /* 0x0E (0x1DA) collision node id of the head */
    signed char Unk0F;                 /* 0x0F (0x1DB) */
    signed char Side;                  /* 0x10 (0x1DC) */
    signed char Unk11;                 /* 0x11 (0x1DD) */
    UCHAR Unk12;                       /* 0x12 (0x1DE) */
    UCHAR Rtn;                         /* 0x13 (0x1DF) sub routine (D_801074E0) */
    SHORT Unk14;                       /* 0x14 (0x1E0) */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    signed char Unk18;                 /* 0x18 (0x1E4) side of the last dive */
    signed char Unk19;                 /* 0x19 (0x1E5) */
    UCHAR pad1A[0x1C - 0x1A];
    ULONG Rtn_save;                    /* 0x1C (0x1E8) Routine_0..3 to return to after the dive */
    SHORT Unk20;                       /* 0x20 (0x1EC) */
    SHORT Unk22;                       /* 0x22 (0x1EE) */
    SHORT Unk24;                       /* 0x24 (0x1F0) */
    SHORT Old_life;                    /* 0x26 (0x1F2) */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
} EM33_FREE;

/* free work of a body segment at 0x1CC */
typedef struct tagEM33_BODY {
    LONG Parts;                        /* 0x00 (0x1CC) head part followed */
    LONG Oya;                          /* 0x04 (0x1D0) index of the head work (func_800336B4) */
    UCHAR pad08[0x0A - 0x08];
    signed char Unk0A;                 /* 0x0A (0x1D6) */
    UCHAR pad0B[0x13 - 0x0B];
    UCHAR Rtn;                         /* 0x13 (0x1DF) */
    UCHAR pad14[0x26 - 0x14];
    SHORT Old_life;                    /* 0x26 (0x1F2) */
} EM33_BODY;

#define EM33_W(p) ((EM33_FREE *)(p)->Unk1CC)
#define EM33_B(p) ((EM33_BODY *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c func_8004AC54) */
typedef struct tagEM33_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C + 4 * key */
        USHORT pad;
    } Key[3];
} EM33_SABUN;
#define EM33_SABUN_P(pP) ((EM33_SABUN *)(pP)->pUnk98)

/* the player work is followed by the work list (G.pEnemy[0], G.pEnemy[1], G.pEnemy[]) */
typedef struct tagEM33_PL {
    UCHAR Pl[0x1D0];                   /* Pl_work */
    ENEMY_WORK *pWork[42];             /* 0x1D0 = G + 0x265C */
} EM33_PL;

/* G viewed with the player work inside it (RE2 G.Player, = Pl_work) */
typedef struct tagEM33_GPL {
    UCHAR pad0[0x248C];
    ENEMY_WORK Player;                 /* 0x248C Pl_work */
    UCHAR pad_pl[0x7888 - 0x248C - sizeof(ENEMY_WORK)];
    ULONG Flg7888;                     /* 0x7888 G.Unk7888 */
} EM33_GPL;
extern EM33_GPL G_pl __asm__("G");

/* Spd_z by Move_cnt / 10 while surfacing (copied to the stack as one block) */
typedef struct tagEM33_SPD_TBL {
    SHORT Spd[8];
} EM33_SPD_TBL;

extern UCHAR *D_800BA6E8;              /* [3]: sound bank id (Snd_se_stad callers) */

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
LONG Joint_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);  /* joint.c */
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);                        /* em_sub.c */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
LONG func_800336B4(ENEMY_WORK *pEm);                                    /* oba.c: collision index of a work */
void func_8004AC54(ENEMY_WORK *pEm, PARTS_W *pP);                       /* sabun.c: apply the morph keys */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);           /* ground height (sca.c) */
LONG func_800445C8(LONG *pA0, LONG *pA1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c */
LONG func_80044420(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);         /* pl_wep.c */
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);

#endif /* OVL_EM33_H */
