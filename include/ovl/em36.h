#ifndef OVL_EM36_H
#define OVL_EM36_H
/*
 * Enemy module em36 (enemy ids 0x36 and 0x3A: Nemesis). Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2). The predecessor is RE2's Tyrant module
 * EM2A.C: Move runs a "brain" routine (Em2a_mv_br) and then a motion routine
 * (Em2a_mv), both indexed by Routine_1; RE3 picks the brain/motion tables by
 * Type (Em36_Move_type).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "sub.h"
#include "pl_wep.h"

/* em36's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM36_FREE {
    struct tagENEMY_WORK *pSub;        /* 0x00 (0x1CC) second work moved with this one (Routine_0..2, Move_cnt, Timer0 set by the attacks) */
    USHORT Flg;                        /* 0x04 (0x1D0) 0x800: player in sight, 0x1000: on a route */
    UCHAR Unk06;                       /* 0x06 (0x1D2) timer */
    UCHAR Unk07;                       /* 0x07 (0x1D3) */
    UCHAR Unk08;                       /* 0x08 (0x1D4) timer */
    UCHAR Unk09;                       /* 0x09 (0x1D5) timer */
    SHORT Type_timer;                  /* 0x0A (0x1D6) frames until Type changes */
    UCHAR Unk0C;                       /* 0x0C (0x1D8) */
    signed char Unk0D;                 /* 0x0D (0x1D9) */
    signed char Unk0E;                 /* 0x0E (0x1DA) */
    signed char Unk0F;                 /* 0x0F (0x1DB) */
    UCHAR Unk10;                       /* 0x10 (0x1DC) */
    UCHAR Unk11;                       /* 0x11 (0x1DD) */
    UCHAR pad12[0x14 - 0x12];
    UCHAR Unk14;                       /* 0x14 (0x1E0) timer */
    UCHAR Unk15;                       /* 0x15 (0x1E1) */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    SHORT Unk18;                       /* 0x18 (0x1E4) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    UCHAR Lost_time;                   /* 0x1C (0x1E8) frames left going to the last seen player position */
    UCHAR Unk1D;                       /* 0x1D (0x1E9) */
    SHORT Lost_dir;                    /* 0x1E (0x1EA) direction to Dest_x/z */
    UCHAR Unk20;                       /* 0x20 (0x1EC) */
    UCHAR Route_no;                    /* 0x21 (0x1ED) waypoint index (Em36_route) */
    SHORT Unk22;                       /* 0x22 (0x1EE) attack counter */
    SHORT Route_dist;                  /* 0x24 (0x1F0) distance to the waypoint */
    SHORT Unk26;                       /* 0x26 (0x1F2) */
    UCHAR Unk28;                       /* 0x28 (0x1F4) */
    UCHAR Unk29;                       /* 0x29 (0x1F5) timer */
    UCHAR Unk2A;                       /* 0x2A (0x1F6) timer */
} EM36_FREE;

/* hit area handed to func_800445C8 (pl_wep.c HIT_AREA) */
typedef struct tagEM36_HIT_AREA {
    LONG *pPos;                        /* 0x00 centre (VECTOR) */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} EM36_HIT_AREA;

/* RE2 PARTS_NECK_FREE_WORK (MAIN.H): PARTS_W free work of the neck part */
typedef struct tagPARTS_NECK_FREE_WORK {
    SHORT N_rot_y;                     /* 0x74 */
    SHORT N_rot_z;                     /* 0x76 */
    SHORT Add_dir_y;                   /* 0x78 */
    SHORT Add_dir_z;                   /* 0x7A */
    SHORT L_rot_y;                     /* 0x7C */
    SHORT L_rot_z;                     /* 0x7E */
} PARTS_NECK_FREE_WORK;

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c func_8004AC54) */
typedef struct tagEM36_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C + 4 * n */
        SHORT pad;
    } Key[7];
} EM36_SABUN;
#define EM36_SABUN_P(pP) ((EM36_SABUN *)(pP)->pUnk98)

/* G.pUnk265C, G.pUnk2660 and G.pEnemy[40] seen as one array (as in oba.c) */
typedef struct tagEM36_G_WORKS {
    UCHAR pad0[0x265C];
    struct tagENEMY_WORK *pWork[42];   /* 0x265C */
} EM36_G_WORKS;

#define PW_NECK(p) ((PARTS_NECK_FREE_WORK *)&(p)->Unk74)
/* PARTS_W free work of the morphing body parts (func_8004A934 blends keys Key_no - 1 and Key_no) */
typedef struct tagEM36_PARTS_FREE {
    UCHAR Key_no;                      /* 0x74 */
    UCHAR Unk01;                       /* 0x75 */
    SHORT Unk02;                       /* 0x76 */
    SHORT Unk04;                       /* 0x78 */
} EM36_PARTS_FREE;
#define EM36_PF(p) ((EM36_PARTS_FREE *)&(p)->Unk74)
#define EM36_W(p) ((EM36_FREE *)(p)->Unk1CC)
/* Flg read as a word together with the bytes after it */
#define EM36_FLGW(p) (*(ULONG *)&EM36_W(p)->Flg)
/* Routine_0..3 / Move_no..Move_cnt written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
/* colour word (r, g, b, code) of the shadow quads */
#define KAGE_RGB(pK, n) (*(ULONG *)&(pK)->Prim[n].r0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);                        /* em_sub.c */
void func_8001AECC(ENEMY_WORK *pEm, ULONG a1, ULONG a2);                /* em_sub.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);  /* joint.c */
LONG Joint_move2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg); /* joint.c */
void func_80018EC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg); /* em_sub.c */
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);     /* em_sub.c */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
void func_80016600(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);         /* door.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
ULONG Gacha_ch(void);                                                   /* em_sub.c */
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);           /* ground height */
LONG func_80044420(MATRIX *pM, SVECTOR *pSv, ENEMY_WORK *pEm);         /* pl_wep.c */
void func_80037A90_em(ENEMY_WORK *pEm, ULONG Arg1) __asm__("func_80037A90"); /* om.c, takes an OBJ_MODEL_WORK */
LONG SquareRoot0(LONG a);                                               /* libgte */
LONG func_800445C8(LONG *pA0, LONG *pA1, EM36_HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c */
LONG Sca_ck_line(VECTOR *pV0, VECTOR *pV1, ULONG Attr, ULONG Flg);      /* sca.c */
void func_8004A934(ENEMY_WORK *pEm, PARTS_W *pP, LONG a2);              /* sabun.c */
void func_8004A934_2(ENEMY_WORK *pEm, PARTS_W *pP) __asm__("func_8004A934"); /* called without a2 */

/* em_sub.c functions called with int arguments (no narrowing at the call) */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");
LONG Dir_pos_ck_l(VECTOR *pV, VECTOR *pP, LONG Hed_dir, LONG Hani_dir) __asm__("Dir_pos_ck");

#endif /* OVL_EM36_H */
