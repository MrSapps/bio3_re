#ifndef OVL_EM3E_H
#define OVL_EM3E_H
/*
 * Enemy module em3e (enemy ids 0x3E, 0x3F): a hovering machine with two
 * rotors and two side guns that flies around a fixed zone, fires when the
 * player is level with it, and crashes on a scenario request. RE2's
 * EM3E.C/EM3F.C are unrelated room props, so names are RE3 only.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "sub.h"

/* the enemy's free work at 0x1CC */
typedef struct tagEM3E_FREE {
    UCHAR Parts_num;                   /* 0x00 (0x1CC) parts flagged 0x400 at death */
    UCHAR Rotor0;                      /* 0x01 (0x1CD) rotor part */
    UCHAR Abr0;                        /* 0x02 (0x1CE) semi-transparent part */
    UCHAR Rotor1;                      /* 0x03 (0x1CF) rotor part (also semi-transparent) */
    UCHAR Sway_rtn;                    /* 0x04 (0x1D0) body pitch sway */
    UCHAR Bob_rtn;                     /* 0x05 (0x1D1) height bob */
    UCHAR Sway_cnt;                    /* 0x06 (0x1D2) */
    UCHAR Bob_cnt;                     /* 0x07 (0x1D3) */
    UCHAR Sway_time;                   /* 0x08 (0x1D4) */
    UCHAR Bob_time;                    /* 0x09 (0x1D5) */
    UCHAR pad0A[0x0C - 0x0A];
    USHORT Timer;                      /* 0x0C (0x1D8) explosion timer / fall speed << 4 */
    SHORT Turn;                        /* 0x0E (0x1DA) spin speed (damage); start Pos_y (crash) */
    SHORT Start_x;                     /* 0x10 (0x1DC) crash: start Pos_x */
    SHORT Fall_x;                      /* 0x12 (0x1DE) crash: x distance covered */
    USHORT Time;                       /* 0x14 (0x1E0) frames alive (+60 per hit) */
    USHORT Dir;                        /* 0x16 (0x1E2) flight direction */
    USHORT Shot_wait;                  /* 0x18 (0x1E4) */
    SHORT Tilt;                        /* 0x1A (0x1E6) body pitch from the x speed */
    SHORT Old_life;                    /* 0x1C (0x1E8) Life of the last frame */
} EM3E_FREE;

#define EM3E_W(p) ((EM3E_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

extern signed char Rsin_tbl[256] __asm__("D_80098828"); /* 0x80098828 sine, 256 steps, +-127 */

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
LONG Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG Area, LONG Flg);         /* root_ck.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);    /* joint.c */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);

#endif /* OVL_EM3E_H */
