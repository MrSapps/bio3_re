#ifndef OVL_EM38_H
#define OVL_EM38_H
/*
 * Enemy module em38 (enemy id 0x38): the large many-part enemy that the
 * tentacles of em39 (enemy id 0x39) are attached to. Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2).
 * The routine frame is RE2's G-type enemy (EM34.C / EM37.C): Em_routine_0 =
 * Init/Move/Damage/Die/Scenario/-/-/Dead, Move picks Em38_Move_type[Type & 3],
 * which runs a brain routine (mv_br) and a motion routine (mv) of 16, both
 * indexed by Routine_1. Damage is indexed by the hit kind (Unk12C), the
 * scenario routines by Routine_1; PLDAMEGE_EM/PLDIE_EM tables for the player.
 */
#include "bio3.h"
#include "em_sub.h"
#include "main.h"
#include "pad.h"
#include "player.h"
#include "pl_wep.h"
#include "sub.h"

/* em38's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM38_FREE {
    struct tagENEMY_WORK *pUnk00;      /* 0x00 (0x1CC) */
    ULONG Unk04;                       /* 0x04 (0x1D0) */
    USHORT Flg;                        /* 0x08 (0x1D4) */
    UCHAR Unk0A;                       /* 0x0A (0x1D6) timer */
    UCHAR Unk0B;                       /* 0x0B (0x1D7) timer */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    SHORT Unk0E;                       /* 0x0E (0x1DA) */
    UCHAR Unk10;                       /* 0x10 (0x1DC) */
    signed char Unk11;                 /* 0x11 (0x1DD) */
    signed char Unk12;                 /* 0x12 (0x1DE) */
    signed char Unk13;                 /* 0x13 (0x1DF) */
    SHORT Unk14;                       /* 0x14 (0x1E0) last Cdir_y */
    UCHAR Unk16;                       /* 0x16 (0x1E2) */
    UCHAR Unk17;                       /* 0x17 (0x1E3) */
    UCHAR Unk18;                       /* 0x18 (0x1E4) */
    UCHAR Unk19;                       /* 0x19 (0x1E5) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    UCHAR Unk1C;                       /* 0x1C (0x1E8) timer */
    UCHAR Unk1D;                       /* 0x1D (0x1E9) timer */
    SHORT Unk1E;                       /* 0x1E (0x1EA) timer */
    SHORT Unk20;                       /* 0x20 (0x1EC) frames the player has been out of sight */
} EM38_FREE;

/* morph (sabun) data at PARTS_W.pUnk98 (sabun.c) */
typedef struct tagEM38_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;                      /* 0x0A */
    SHORT Unk0C;                       /* 0x0C */
} EM38_SABUN;

#define EM38_W(p) ((EM38_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);  /* joint.c */
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);     /* em_sub.c */
void Flg_off(ULONG *pFlg, ULONG Id);                                    /* sub.c */
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);      /* sca.c */
LONG func_8004D720(VECTOR *pPos, ULONG a1, ULONG a2, ULONG a3);         /* ground height */
void MulKage(void *pKw, LONG Grand, LONG Dir_y, ULONG Flg);             /* em_sub.c */
void func_8004A934(ENEMY_WORK *pEm, PARTS_W *pP);                       /* sabun.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, ULONG a4); /* RE2 Esp_call */

#endif /* OVL_EM38_H */
