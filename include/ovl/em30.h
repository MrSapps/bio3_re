#ifndef OVL_EM30_H
#define OVL_EM30_H
/*
 * Enemy module em30 (enemy id 0x30). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2). A large 17-part body (Type & 1 == 0) that spawns seven
 * collision sub-works (Type & 1) tied to its parts. Routine table
 * Init/Move/Damage/Die as RE2's EM2x modules; Move is the RE2 mv_br[]/mv[]
 * pair indexed by Routine_1, Damage is indexed by the hit weapon (Unk12C),
 * Die by Unk12C too. The die handler for the player is hooked into
 * G.pPldie_Em[Id - 0x10].
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "player.h"
#include "sca.h"
#include "pad.h"
#include "sub.h"
#include "main.h"
#include "pl_wep.h"

/* the body's private work in ENEMY_WORK.Unk1CC[] (RE2 Free[]); runs past 0x1FC */
typedef struct tagEM30_FREE {
    UCHAR Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk01;                       /* 0x01 (0x1CD) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    UCHAR Unk03;                       /* 0x03 (0x1CF) */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    signed char Unk0A;                 /* 0x0A (0x1D6) */
    signed char Unk0B;                 /* 0x0B (0x1D7) */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    signed char Unk0E;                 /* 0x0E (0x1DA) */
    signed char Unk0F;                 /* 0x0F (0x1DB) */
    signed char Unk10;                 /* 0x10 (0x1DC) */
    signed char Unk11;                 /* 0x11 (0x1DD) */
    signed char Unk12;                 /* 0x12 (0x1DE) */
    UCHAR Unk13;                       /* 0x13 (0x1DF) */
    SHORT Unk14;                       /* 0x14 (0x1E0) */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    signed char Unk18;                 /* 0x18 (0x1E4) */
    signed char Unk19;                 /* 0x19 (0x1E5) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    UCHAR pad1C[0x20 - 0x1C];
    SHORT Unk20;                       /* 0x20 (0x1EC) */
    SHORT Unk22;                       /* 0x22 (0x1EE) */
    SHORT Unk24;                       /* 0x24 (0x1F0) */
    SHORT Unk26;                       /* 0x26 (0x1F2) */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    UCHAR pad2A[0x2E - 0x2A];
    USHORT Unk2E;                      /* 0x2E (0x1FA) last distance (mv[3]) */
    UCHAR Unk30;                       /* 0x30 (0x1FC) */
    UCHAR Unk31;                       /* 0x31 (0x1FD) */
} EM30_FREE;

/* a collision sub-work's private work: which part of which body it follows */
typedef struct tagEM30_SUB_FREE {
    LONG Parts_no;                     /* 0x00 (0x1CC) part of the body */
    LONG Oya_no;                       /* 0x04 (0x1D0) index of the body in G.pEnemy[0][] */
} EM30_SUB_FREE;

#define G_PLAYER ((ENEMY_WORK *)&G.Pl_be_flg)
#define EM30_W(p) ((EM30_FREE *)(p)->Unk1CC)
#define EM30_SUB(p) ((EM30_SUB_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x201) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
#define PW_FREE_L(p) ((ULONG *)&(p)->Unk74)

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c func_8004AC54) */
typedef struct tagEM30_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C + 4 * key */
        USHORT pad;
    } Key[3];
} EM30_SABUN;
#define EM30_SABUN_P(pP) ((EM30_SABUN *)(pP)->pUnk98)

/* exe functions without a shared prototype (signatures as defined in src/game) */

extern USHORT Em30_vib_tbl[] __asm__("D_80108A3C"); /* pad vibration scripts */
extern void (*Em30_routine_0[])() __asm__("D_80108C74");
extern USHORT Em30_col_w[] __asm__("D_80108C98");
extern void (*Em30_mv_br[])() __asm__("D_80108CC0");
extern void (*Em30_mv[])() __asm__("D_80108D00");
extern void (*Em30_dm[][2])() __asm__("D_80108D40");
extern void (*Em30_die[])() __asm__("D_80108E40");

void func_801000AC(ENEMY_WORK *pEm);
void func_80100544(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100B34(ENEMY_WORK *pSub, LONG No, ENEMY_WORK *pEm, LONG Parts_no);
void func_80100CBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_Move_type(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100CF4");
void func_80100D78(ENEMY_WORK *pEm);
void func_80100DB4(void);
void func_80100DBC(ENEMY_WORK *pEm);
void func_80100FA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801010F4(void);
void func_801010FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101380(void);
void func_80101388(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102250(ENEMY_WORK *pEm, LONG No, LONG Big);
void func_801023F4(ENEMY_WORK *pEm, LONG No, LONG Big);
void func_8010252C(ENEMY_WORK *pEm, LONG No);
void func_8010258C(ENEMY_WORK *pEm);
void func_801025E0(ENEMY_WORK *pEm);
void func_801029AC(void);
void func_801029B4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102E68(void);
void func_80102E70(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801032D4(void);
void func_801032DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801035DC(ENEMY_WORK *pEm);
void func_80103684(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104590(ENEMY_WORK *pEm);
void func_80104724(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104920(ENEMY_WORK *pEm);
void func_80104A9C(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
void func_80104C58(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
ENEMY_WORK *func_80104E58(ENEMY_WORK *pEm);
LONG func_80104F20(ENEMY_WORK *pEm);
void func_8010532C(ENEMY_WORK *pEm, LONG Step, LONG Dir);
void func_801053CC(ENEMY_WORK *pEm);
void func_8010549C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105514(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105740(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801059A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105B4C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105C8C(ENEMY_WORK *pEm);
void func_80105E94(ENEMY_WORK *pEm);
void func_80106130(ENEMY_WORK *pEm);
void func_80106710(ENEMY_WORK *pEm);
void func_8010682C(ENEMY_WORK *pEm);
void func_80106B3C(ENEMY_WORK *pEm, LONG No, LONG Kind);
void func_80106C28(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106C8C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107EE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801089B0(ULONG No, ENEMY_WORK *pEm, LONG *pPos);
void Em30_Pldie(ENEMY_WORK *pPl) __asm__("func_80108A28");

#endif /* OVL_EM30_H */
