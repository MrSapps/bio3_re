#ifndef OVL_EM34_H
#define OVL_EM34_H
/*
 * Enemy module em34 (enemy ids 0x34 and 0x35: Nemesis, first form). Loaded at
 * 0x80100000 (slot 1) or 0x8010D000 (slot 2). The predecessor is RE2's Tyrant
 * module EM2A.C: Move runs a "brain" routine (Em2a_mv_br) and then a motion
 * routine (Em2a_mv), both indexed by Routine_1; RE3 picks the brain/motion
 * tables by Type (Em34_Move_type) and by the sign of D_800D1F2C.
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "sub.h"
#include "md_sub.h"
#include "mojidisp.h"
#include "pl_wep.h"
#include "joint.h"

/* em34's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM34_FREE {
    UCHAR Unk00;                       /* 0x00 (0x1CC) timer */
    UCHAR Unk01;                       /* 0x01 (0x1CD) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    signed char Unk03;                 /* 0x03 (0x1CF) */
    USHORT Flg;                        /* 0x04 (0x1D0) 0x100: chasing the sub player */
    UCHAR Unk06;                       /* 0x06 (0x1D2) timer */
    UCHAR Unk07;                       /* 0x07 (0x1D3) */
    SHORT Unk08;                       /* 0x08 (0x1D4) direction offset of the scripted shot (Em34_sce10) */
    SHORT Unk0A;                       /* 0x0A (0x1D6) */
    UCHAR Flinch;                      /* 0x0C (0x1D8) body flinch state (Em34_after0) */
    signed char Flinch_cnt;            /* 0x0D (0x1D9) */
    signed char Flinch_dir;            /* 0x0E (0x1DA) 1 or -1 */
    signed char Flinch_spd;            /* 0x0F (0x1DB) from Em34_flinch_tbl */
    UCHAR Unk10;                       /* 0x10 (0x1DC) timer */
    UCHAR Target_time;                 /* 0x11 (0x1DD) frames left chasing the sub player */
    UCHAR Unk12;                       /* 0x12 (0x1DE) timer */
    signed char Unk13;                 /* 0x13 (0x1DF) (Rnd() & 1) + 3 */
    SHORT Unk14;                       /* 0x14 (0x1E0) explosion timer (Em34_Move_type03) */
    UCHAR pad16[0x18 - 0x16];
    ENEMY_WORK *pTarget;               /* 0x18 (0x1E4) Pl_work or the sub player */
    ENEMY_WORK *pPartner;              /* 0x1C (0x1E8) enemy 0x37 or 0x3B in the room */
    USHORT Hit_cnt;                    /* 0x20 (0x1EC) frames touching a character */
    UCHAR Unk22;                       /* 0x22 (0x1EE) timer */
    UCHAR Near_cnt;                    /* 0x23 (0x1EF) frames the sub player is closer */
} EM34_FREE;

/* PARTS_W free work of the claw arm blown off by the explosion (Em34_sce08) */
typedef struct tagPARTS_FREE_VIEW {
    UCHAR pad[0x74];
    SHORT Free[7];                     /* 0x74..0x80 */
} PARTS_FREE_VIEW;
#define PW_FREE(p) ((PARTS_FREE_VIEW *)(p))
#define EM34_W(p) ((EM34_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..Move_cnt written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* Flg, Unk06 and Unk07 read as one word (lw + andi/and) */
#define EM34_FLG_L(p) (*(ULONG *)&EM34_W(p)->Flg)

/* the player's ENEMY_WORK inside G (RE2 G.Player); same address as Pl_work, but G-relative */
#define G_PL ((ENEMY_WORK *)&G.Pl_be_flg)

/* attack point handed to func_80047F70 (pl_wep.c): hits other enemies */
typedef struct tagEM34_AT_POINT {
    LONG *pPos;                        /* 0x00 part position (VECTOR) */
    SVECTOR Ofs;                       /* 0x04 */
    USHORT Dir;                        /* 0x0C */
} EM34_AT_POINT;

/* morph (sabun) data at PARTS_W.pUnk98 (sabun.c), same as em38 */
typedef struct tagEM34_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;                      /* 0x0A */
    struct {
        SHORT Rate;                    /* weight of morph target n (0x1000 = full) */
        SHORT pad;
    } Key[7];                          /* 0x0C */
} EM34_SABUN;
#define EM34_SA(p) ((EM34_SABUN *)(p)->pUnk98)

/* motion numbers of the three catch/throw variants, copied to the stack by the catch motions */
typedef struct tagEM34_MOVE3 {
    char Move[3];
} EM34_MOVE3;

extern LONG D_800D1F2C;                /* flags, < 0: hard-mode tables */

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);                        /* em_sub.c */
void func_8001AECC(ENEMY_WORK *pEm, ULONG a1, ULONG a2);                /* em_sub.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);  /* joint.c */
LONG Joint_move2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg); /* joint.c */
void func_80018EC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg); /* em_sub.c */
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);     /* em_sub.c */
void func_8002646C(ENEMY_WORK *pEm, LONG A, LONG B, LONG C);            /* joint.c (SHORT, SHORT, UCHAR) */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
void func_80016600(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);         /* door.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
ULONG Gacha_ch(void);                                                   /* em_sub.c */
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);           /* ground height */
LONG func_80044420(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);         /* pl_wep.c */
LONG SquareRoot0(LONG a);                                               /* libgte */
SHORT Get_dir_xz(LONG X, LONG Z);                                       /* em_sub.c */
void func_8002C680(MD_OBJ *pObj, ULONG Ctp, ULONG Ccl, ULONG V_ofs, ULONG Add); /* md_sub.c */
LONG func_800445C8(LONG *pA0, LONG *pA1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c */
LONG func_80047F70(ENEMY_WORK *pEm, EM34_AT_POINT *pAt, SHORT *pData);  /* pl_wep.c */
LONG func_8005F984(LONG Dmg, ULONG Flg);                                /* spl_auto.c: sub player life down */
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);      /* sca.c */
void func_8004A934(ENEMY_WORK *pEm, PARTS_W *pP);                       /* sabun.c */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");      /* em_sub.c, result used as int */
void func_80037A90_em(ENEMY_WORK *pEm, ULONG Arg1) __asm__("func_80037A90"); /* om.c, takes an OBJ_MODEL_WORK */
void Quake_add(LONG X, LONG Y);                                         /* pl_wep.c */

#endif /* OVL_EM34_H */
