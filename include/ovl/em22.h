#ifndef OVL_EM22_H
#define OVL_EM22_H
/*
 * Enemy module em22 (enemy ids 0x22 and 0x24: the hunters). Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2). RE2 has no hunter: the module keeps RE2's
 * enemy frame (EM_MAIN, Em_routine_0 = Init/Move/Damage/Die/.../Dead, Free[]
 * work at 0x1CC) but its move logic is new: Move runs a think routine
 * (em22_think_tbl) and then an action routine (em22_move_tbl), both indexed
 * by Routine_1.
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "sca.h"
#include "sub.h"
#include "pl_wep.h"

/* em22's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM22_FREE {
    SHORT Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    UCHAR Unk03;                       /* 0x03 (0x1CF) timer, (Rnd() & 0x1F) + 30 at init */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    SHORT Unk0A;                       /* 0x0A (0x1D6) */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    UCHAR Unk0E;                       /* 0x0E (0x1DA) */
    UCHAR Unk0F;                       /* 0x0F (0x1DB) */
    UCHAR Unk10;                       /* 0x10 (0x1DC) */
    UCHAR Unk11;                       /* 0x11 (0x1DD) */
    UCHAR Unk12;                       /* 0x12 (0x1DE) */
    UCHAR Unk13;                       /* 0x13 (0x1DF) */
    UCHAR Unk14;                       /* 0x14 (0x1E0) */
    UCHAR pad15;
    UCHAR Unk16;                       /* 0x16 (0x1E2) */
    UCHAR Unk17;                       /* 0x17 (0x1E3) */
    UCHAR Unk18;                       /* 0x18 (0x1E4) */
    signed char Unk19;                 /* 0x19 (0x1E5) */
    UCHAR Unk1A;                       /* 0x1A (0x1E6) */
    signed char Unk1B;                 /* 0x1B (0x1E7) timer */
    SHORT Unk1C;                       /* 0x1C (0x1E8) */
    SHORT Unk1E;                       /* 0x1E (0x1EA) */
    USHORT Unk20;                      /* 0x20 (0x1EC) */
    UCHAR Unk22;                       /* 0x22 (0x1EE) */
    UCHAR Unk23;                       /* 0x23 (0x1EF) */
    SHORT Old_spd_x;                   /* 0x24 (0x1F0) Spd_x of the last frame */
    USHORT Unk26;                      /* 0x26 (0x1F2) timer, 150 at init */
    UCHAR Unk28;                       /* 0x28 (0x1F4) */
    UCHAR Unk29;                       /* 0x29 (0x1F5) */
    UCHAR Unk2A;                       /* 0x2A (0x1F6) */
    UCHAR Unk2B;                       /* 0x2B (0x1F7) */
    SHORT Unk2C;                       /* 0x2C (0x1F8) */
    SHORT Unk2E;                       /* 0x2E (0x1FA) */
} EM22_FREE;

#define EM22_W(p) ((EM22_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..Move_cnt written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* Routine_2..3 written as one halfword */
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
/* Routine_0..1 read as one halfword */
#define EM_RTN01(p) (*(SHORT *)&(p)->Routine_0)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
LONG SquareRoot0(LONG a);                                               /* libgte */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
LONG Guide_check(LONG X, LONG Z, LONG Floor);                           /* root_ck.c */
ULONG func_8004D6B8(LONG X, LONG Z, LONG a2, LONG a3);                  /* sca.c, area bits of a point */
void Sca_search(ULONG Type, ULONG Id, ULONG Floor);                     /* sca.c */
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);     /* sca.c */
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);                        /* em_sub.c */
void func_8001AECC(ENEMY_WORK *pEm, ULONG a1, ULONG a2);                /* em_sub.c */
void Kage_work_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos); /* em_sub.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c */
void func_80018EC8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg); /* em_sub.c */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);         /* em_sub.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
void func_8002646C(ENEMY_WORK *pEm, LONG A, LONG B, LONG C);            /* joint.c (SHORT, SHORT, UCHAR) */
LONG func_800445C8(LONG *pA0, LONG *pA1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c */
LONG func_80044420(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);         /* pl_wep.c */
void func_80016600(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);              /* door.c */
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);          /* snd.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);           /* ground height (sca.c) */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
ULONG Joint_move2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);    /* joint.c (pl_wep2.h takes ULONG *) */
ULONG Gacha_ch(void);                                                   /* em_sub.c */

/* Rot_vector (main.h) called with an int direction */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector");

#endif /* OVL_EM22_H */
