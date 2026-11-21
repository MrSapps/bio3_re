#ifndef OVL_EM23_H
#define OVL_EM23_H
/*
 * Enemy module em23 (enemy ids 0x23 and 0x28: Drain Deimos / Brain Sucker, the
 * flea-like enemy that walks on floors, walls and ceilings). RE3-specific, but
 * built on the RE2 enemy skeleton (EM_MAIN with Root_ck/Guide_check as RE2
 * EM10.C, Em_routine_0[8] = Init/Move/Damage/Die/Scenario/-/-/Dead, a pair of
 * br/mv tables of 22 routines selected by Routine_1, damage/die tables indexed
 * by the hit kind (Unk12C) and Routine_1, PLDAMEGE_EM/PLDIE_EM for the player
 * held by it). Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep.h"
#include "player.h"
#include "main.h"
#include "sca.h"

/* em23's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM23_FREE {
    signed char Side;                  /* 0x00 (0x1CC) wall side: bit 0 wall along x (SCA test), bit 1 far side */
    signed char Unk01;                 /* 0x01 (0x1CD) */
    signed char Mode;                  /* 0x02 (0x1CE) surface: 0 floor .. 4 ceiling */
    signed char Unk03;                 /* 0x03 (0x1CF) */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    SHORT Unk0A;                       /* 0x0A (0x1D6) */
    SHORT Floor_y;                     /* 0x0C (0x1D8) room floor height (SCA_HEAD +0xC) */
    SHORT Ground;                      /* 0x0E (0x1DA) ground height under the enemy */
    signed char Flg;                   /* 0x10 (0x1DC) 8: root search active, & 7: next state */
    UCHAR Unk11;                       /* 0x11 (0x1DD) */
    SHORT Unk12;                       /* 0x12 (0x1DE) */
    SHORT Unk14;                       /* 0x14 (0x1E0) */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    SHORT Timer;                       /* 0x18 (0x1E4) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    struct tagSCA_DATA *pSca;          /* 0x1C (0x1E8) wall shape to climb (copy of pSca_hit_data) */
    signed char Unk20;                 /* 0x20 (0x1EC) */
    signed char Unk21;                 /* 0x21 (0x1ED) */
    SHORT Unk22;                       /* 0x22 (0x1EE) */
    SHORT Unk24;                       /* 0x24 (0x1F0) */
    SHORT Unk26;                       /* 0x26 (0x1F2) */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
    USHORT Unk2C;                      /* 0x2C (0x1F8) */
    SHORT Unk2E;                       /* 0x2E (0x1FA) */
} EM23_FREE;

#define EF(p) ((EM23_FREE *)(p)->Unk1CC)
/* bytes 1..2 of the free area tested as one word */
#define EF_W0(p) ((p)->Unk1CC[0])
/* Size/Status_flg read as one word */
#define EM_D0W(p) (*(ULONG *)&(p)->Size)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(SHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* SCA_HEAD +0xC (sca.h padC): floor height of the room */
#define SCA_FLOOR_Y(p) (((SHORT *)(p))[6])
/* x/z pair packed for the SCA line tests (sca.c func_8004F258) */
#define SCA_XZ(x, z) ((((x) >> 1) << 16) | (((z) >> 1) & 0xFFFF))
/* body direction on the wall from the base direction (Unk06/Unk08) and the wall direction Unk04;
   the _T form also clears Timer0 */
#define EM23_WALL_DIR_(p, T)                                                  \
    {                                                                         \
        UCHAR Side_ = EF(p)->Side;                                           \
        T;                                                                    \
        if (Side_ & 1) {                                                      \
            if (Side_ & 2)                                                    \
                (p)->Cdir_y = EF(p)->Unk08 + EF(p)->Unk04;                    \
            else                                                              \
                (p)->Cdir_y = EF(p)->Unk08 - EF(p)->Unk04;                    \
        } else {                                                              \
            (p)->Cdir_x = EF(p)->Unk06 + EF(p)->Unk04;                        \
        }                                                                     \
    }
#define EM23_WALL_DIR(p) EM23_WALL_DIR_(p, (void)0)
#define EM23_WALL_DIR_T(p) EM23_WALL_DIR_(p, (p)->Timer0 = 0)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

/* hit area handed to func_800445C8 (pl_wep.c) */
typedef struct tagEM23_HIT_AREA {
    VECTOR *pPos;                      /* 0x00 */
    SHORT *pBox;                       /* 0x04 x, y, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} EM23_HIT_AREA;

/* G seen from the player work (Pl_work = G + 0x248C, RE2 G.Player): one symbol, so gcc
   addresses G.pEnemy / G.ppEnemy_end relative to &Pl_work */
typedef struct {
    UCHAR pad[0x248C];
    UCHAR Player[0x1D0];               /* 0x248C Pl_work */
    ENEMY_WORK *pUnk265C;              /* 0x265C */
    ENEMY_WORK *pUnk2660;              /* 0x2660 */
    ENEMY_WORK *pEnemy[40];            /* 0x2664 */
    ENEMY_WORK **ppEnemy_end;          /* 0x2704 */
    UCHAR pad2708[0x75E8 - 0x2708];
    USHORT Unk75E8;                    /* 0x75E8 G.Unk75E8: player fire flags */
} G_PL_VIEW;
extern G_PL_VIEW G_pl __asm__("G");

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");     /* em_sub.c Cdir_ck, int result */
void Em_dead_flg_on_cand(); /* em_sub.c, takes pEm: called with a0 still holding it */
void Player_move(ENEMY_WORK *pEm);                                      /* player.c */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector"); /* main.h Rot_vector, int Dir */
LONG Rnd_area(void);                                                    /* root_ck.c */
LONG Guide_check(LONG X, LONG Z, LONG Floor);                           /* root_ck.c */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, frame-end flag */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);         /* em_sub.c */
ULONG Gacha_ch(void);                                                   /* em_sub.c */
void func_8002646C(ENEMY_WORK *pEm, LONG A, LONG B, LONG C);            /* joint.c */
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);           /* RE2 Sca_get_ground (sca.c) */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
void func_80034124(COL_OBJ *pObj);                                      /* oba.c */
void func_80016600(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);              /* door.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, ULONG a4); /* RE2 Esp_call */

#endif /* OVL_EM23_H */
