#ifndef OVL_EM26_H
#define OVL_EM26_H
/*
 * Enemy module em26 (enemy id 0x26), the successor of RE2's EM26.C: a small
 * crawling enemy (walk, fall, jump, swim; catches the player).
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"

/* em26's view of ENEMY_WORK.Unk1CC[12] (RE2 EM26_FREE, EM_SUB.H) */
typedef struct tagEM26_FREE {
    ENEMY_WORK *pTarget_pl;            /* 0x00 (0x1CC) */
    SHORT No_catch_cnt;                /* 0x04 (0x1D0) */
    signed char Kasakasarot;           /* 0x06 (0x1D2) */
    signed char Catch_dir;             /* 0x07 (0x1D3) */
    UCHAR First_catch;                 /* 0x08 (0x1D4) */
    UCHAR Atck_flg;                    /* 0x09 (0x1D5) */
    UCHAR Jump_flg;                    /* 0x0A (0x1D6) */
    signed char Chase_rot;             /* 0x0B (0x1D7) */
    signed char Unk0C;                 /* 0x0C (0x1D8) RE3: no-catch timer after a jump, (Rnd() & 0x3F) + 0x5A */
} EM26_FREE;

#define EM26_W(p) ((EM26_FREE *)(p)->Unk1CC)
/* the player work inside G (RE2 G.Player; Pl_work is the same address) */
#define G_PLAYER ((ENEMY_WORK *)&G.Pl_be_flg)

/* blood effect of a death (RE2 EM26_BLOOD_TBL; RE3 adds a third Esp code byte) */
typedef struct tagEM26_BLOOD_TBL {
    UCHAR Id;                          /* 0x0 Esp code bits 0..7 */
    UCHAR Type;                        /* 0x1 Esp code bits 8..15 */
    UCHAR Flg;                         /* 0x2 Esp code bits 24..27 */
    UCHAR pad3;
    USHORT Dir;                        /* 0x4 added to Cdir_y */
    USHORT Zoom;                       /* 0x6 */
} EM26_BLOOD_TBL;

/* *(USHORT *)&Routine_2 */
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* Kill_enemy_work (RE2 macro): free the work */
#define KILL_ENEMY_WORK(p) ((p)->Be_flg = 0, *(ULONG *)&(p)->Routine_0 = 0, G.Em_num--)

/* module .data: routine tables, unprototyped like RE2's (routines take pEm, Kan, Seq) */
extern void (*Em26_routine_0[])() __asm__("D_80101160");
extern void (*Em26_move_tbl[])() __asm__("D_80101188");
extern void (*Em26_mv_walk_tbl[])() __asm__("D_8010119C");
extern UCHAR Em26_routine_tbl[] __asm__("D_80101180");     /* Routine_1 by Type */
extern UCHAR Em26_dieblood_type_tbl[] __asm__("D_801011A4");
extern EM26_BLOOD_TBL Em26_blood_id_tbl[] __asm__("D_801011C4");

/* exe functions without a shared prototype */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, result used as int */
LONG Water_ck(LONG Pos_x, LONG Pos_z);
ENEMY_WORK *Em_work_add(ENEMY_WORK *pSrc);                              /* em_sub.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
LONG Rnd(void) __asm__("func_800102E8");                                 /* RE2 Rnd (sld.s) */
LONG func_8004D720(LONG *pPos, ULONG W, ULONG a2, ULONG a3, LONG Pos_y); /* ground height under pPos */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */

void func_80100D5C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);              /* RE2 Em26_Die00 */
void func_80100E38(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);              /* RE2 Em26_Die10 */
void func_8010100C(ENEMY_WORK *pTg, UCHAR Parts, UCHAR Type);          /* RE2 Set_blood */
ULONG func_80100EF4(ENEMY_WORK *pEm, ULONG Pl);
void func_801010E8(ENEMY_WORK *pPl, UCHAR Dir);                         /* RE2 Em26_Pl_to_dmg */                         /* RE2 Em26_atck_ck */
LONG func_80101100(ENEMY_WORK *pEm);                                    /* RE2 Get_sc_hit */

#endif /* OVL_EM26_H */
