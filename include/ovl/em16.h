#ifndef OVL_EM16_H
#define OVL_EM16_H
/*
 * Enemy module em16 (zombie variants, enemy ids 0x16, 0x18-0x1F). Its code is a
 * near copy of em10 (RE2 EM10.C + EM10_S0.C): same routine tables at the same
 * .data offsets, mostly identical routines; Init (func_8010043C) differs.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep2.h"
#include "joint.h"
#include "main.h"
#include "player.h"

/* em16's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]; same layout as em10's EM10_FREE) */
typedef struct tagEM16_FREE {
    UCHAR Walk_no;                     /* 0x00 (0x1CC) RE2 FREE_UB(0): walk motion, Rnd() % 3 */
    UCHAR Unk01;                       /* 0x01 (0x1CD) RE2 FREE_UB(1) */
    USHORT Flg;                        /* 0x02 (0x1CE) RE2 FREE_US(2) */
    UCHAR Unk04;                       /* 0x04 (0x1D0) */
    UCHAR Unk05;                       /* 0x05 (0x1D1) */
    UCHAR Unk06;                       /* 0x06 (0x1D2) */
    UCHAR Unk07;                       /* 0x07 (0x1D3) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    UCHAR Unk0A;                       /* 0x0A (0x1D6) RE2 FREE_UB(0xa) */
    signed char Unk0B;                 /* 0x0B (0x1D7) RE2 FREE_UB(0xb), (Rnd() & 0xF) + 0x10 */
    ULONG Unk0C;                       /* 0x0C (0x1D8) */
    ULONG Routine;                     /* 0x10 (0x1DC) copy of Routine_0..3 (RE2 OVL_FREE[4]) */
    ULONG Unk14;                       /* 0x14 (0x1E0) */
    UCHAR Unk18;                       /* 0x18 (0x1E4) RE2 FREE_UB(0x18), G.Unk75EA bit 0 when it runs out */
    UCHAR Unk19;                       /* 0x19 (0x1E5) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    UCHAR Unk1C;                       /* 0x1C (0x1E8) */
    UCHAR Root_no;                     /* 0x1D (0x1E9) RE2 FREE_UB(0x1d), Root_ck arg */
    USHORT Cnt;                        /* 0x1E (0x1EA) RE2 FREE_US(0x1e), frame counter */
    UCHAR Unk20;                       /* 0x20 (0x1EC) */
    UCHAR Unk21;                       /* 0x21 (0x1ED) RE2 FREE_UB(0x21), timer */
    UCHAR Unk22;                       /* 0x22 (0x1EE) */
    UCHAR Unk23;                       /* 0x23 (0x1EF) */
    UCHAR Ude_down;                    /* 0x24 (0x1F0) non-zero: the arm-down routine runs */
    UCHAR Unk25;                       /* 0x25 (0x1F1) */
    UCHAR Unk26;                       /* 0x26 (0x1F2) RE2 FREE_UB(0x26), timer */
    UCHAR Unk27;                       /* 0x27 (0x1F3) Rnd() % 3 + 3 */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
    union {
        struct {
            SHORT Unk2C;               /* 0x2C (0x1F8) arm-down routine */
            UCHAR Unk2E;               /* 0x2E (0x1FA) (Rnd() & 1) + 6 */
            UCHAR Unk2F;               /* 0x2F (0x1FB) timer */
        } w;
        LONG Esp_idx;                  /* 0x2C (0x1F8) Em16_mv11: index into Em16_mv11_esp */
    } u2C;
} EM16_FREE;

/* effect script of Em16_mv11 (16 bytes per entry, Cnt 0xFF ends it) */
typedef struct tagEM16_ESP {
    SHORT Cnt;                         /* 0x0 Routine_3 frame, 0xFF: end */
    SHORT Parts_no;                    /* 0x2 part, 0xEE: next Routine_2, 0xFF: at the enemy position */
    SHORT Ofs_x;                       /* 0x4 */
    SHORT Ofs_y;                       /* 0x6 */
    SHORT Ofs_z;                       /* 0x8 */
    SHORT Id_lo;                       /* 0xA effect id, low byte */
    SHORT Id_hi;                       /* 0xC effect id, high byte */
    SHORT Esp_no;                      /* 0xE */
} EM16_ESP;

#define EM16_W(p) ((EM16_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..Move_cnt written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
/* RE2 Super_vector: Add_dir/Unk14A/Unk14C viewed as the SVECTOR of the knock-down rotation (em16 dm02) */
#define SUPER_VECTOR(p) ((SVECTOR *)&(p)->Add_dir)
/* the same free work as two words (colour pair, e.g. 0x20 / 0x101020) */
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);                        /* em_sub.c */
void func_8001AECC(ENEMY_WORK *pEm, ULONG a1, ULONG a2);                /* em_sub.c */
void Kage_work_set(ULONG *pK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos); /* em_sub.c */
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);     /* em_sub.c */
void func_80018EC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg); /* em_sub.c */
void func_8002646C(ENEMY_WORK *pEm, LONG A, LONG B, LONG C);            /* joint.c (SHORT, SHORT, UCHAR) */
/* Joint_move (joint.h) seen with the kan/seq pointers and its result (frame-end flag) */
LONG Joint_move_r(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg) __asm__("Joint_move");
void MulKage(KAGE_WORK *pKw, SHORT Grand, SHORT Dir_y, ULONG Flg);       /* em_sub.c */
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);           /* ground height (door.c) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");   /* Cdir_ck, result used unmasked */
ULONG Gacha_ch(void);
void Foot_set_pl(ENEMY_WORK *pEm, LONG a1, LONG a2);                    /* em_sub.c */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
void func_80016600(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);         /* door.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */

#endif /* OVL_EM16_H */
