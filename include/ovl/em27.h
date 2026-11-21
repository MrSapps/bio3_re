#ifndef OVL_EM27_H
#define OVL_EM27_H
/*
 * Enemy module em27 (enemy id 0x27). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"

/* em27's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM27_FREE {
    SHORT Dir;                         /* 0x00 (0x1CC) hold angle relative to the player's Cdir_y */
    SHORT Wait;                        /* 0x02 (0x1CE) frames before the next move */
    SHORT Pop;                         /* 0x04 (0x1D0) part 2 pop-up state (func_8010158C) */
    SHORT Floor;                       /* 0x06 (0x1D2) last Pl_work.Unk122 (func_801014B0) */
} EM27_FREE;

#define EM27_W(p) ((EM27_FREE *)(p)->Unk1CC)
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
/* the player work inside G (RE2 G.Player; Pl_work is the same address) */
#define G_PLAYER ((ENEMY_WORK *)&G.Pl_be_flg)

/* RE2 PARTS_NECK_FREE_WORK (MAIN.H): PARTS_W free work of the neck part */
typedef struct tagPARTS_NECK_FREE_WORK {
    SHORT N_rot_y;                     /* 0x74 */
    SHORT N_rot_z;                     /* 0x76 */
    SHORT Add_dir_y;                   /* 0x78 */
    SHORT Add_dir_z;                   /* 0x7A */
    SHORT L_rot_y;                     /* 0x7C */
    SHORT L_rot_z;                     /* 0x7E */
} PARTS_NECK_FREE_WORK;
#define PW_NECK(p) ((PARTS_NECK_FREE_WORK *)&(p)->Unk74)

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em27_routine_0[])() __asm__("D_80101A34");
extern void (*Em27_mv_br[])() __asm__("D_80101A54");
extern void (*Em27_mv[])() __asm__("D_80101A6C");

/* exe functions without a shared prototype */
LONG Rnd(void) __asm__("func_800102E8");                                 /* RE2 Rnd (sld.s) */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, result used as int */
LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);        /* ground height under pV */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);         /* em_sub.c */
LONG Joint_move2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);     /* joint.c */
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);          /* snd.c */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
ULONG Gacha_ch(void);                                                   /* em_sub.c */
ENEMY_WORK *Em_work_add(ENEMY_WORK *pSrc);                              /* em_sub.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector");  /* main.c, called with an int angle */

void func_80100378(ENEMY_WORK *pEm);                                    /* parts reset (live) */
void func_80100518(ENEMY_WORK *pEm);                                    /* hide (parts reset) */
void func_8010158C(ENEMY_WORK *pEm);                                    /* part 2 pop-up */
void func_801014B0(ENEMY_WORK *pEm, SHORT Dist, SHORT Dir, VECTOR *pPos); /* place relative to pPos */
void func_801018D8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);              /* player damage routine */
LONG func_801019BC(ENEMY_WORK *pEm);                                    /* other id 0x23/0x28 enemies */

#endif /* OVL_EM27_H */
