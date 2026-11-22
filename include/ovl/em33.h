#ifndef OVL_EM33_H
#define OVL_EM33_H
/*
 * Enemy module em33 (enemy id 0x33): a segmented giant worm. The head work
 * (Type & 1 clear) allocates seven body works (Type & 1 set) that follow
 * parts of the head's model and carry their own collision nodes and shadows.
 * RE2's EM33.C (licker family) is unrelated, so names are RE3 only.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "player.h"
#include "main.h"
#include "sub.h"
#include "pad.h"
#include "pl_wep.h"

/* free work of the head at 0x1CC */
typedef struct tagEM33_FREE {
    UCHAR Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk01;                       /* 0x01 (0x1CD) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    UCHAR Morph_dir;                   /* 0x03 (0x1CF) morph key 0 direction (func_8010183C) */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    signed char Unk0A;                 /* 0x0A (0x1D6) */
    signed char Unk0B;                 /* 0x0B (0x1D7) */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    signed char Col_id;                /* 0x0E (0x1DA) collision node id of the head */
    signed char Unk0F;                 /* 0x0F (0x1DB) */
    signed char Side;                  /* 0x10 (0x1DC) */
    signed char Unk11;                 /* 0x11 (0x1DD) */
    UCHAR Unk12;                       /* 0x12 (0x1DE) */
    UCHAR Rtn;                         /* 0x13 (0x1DF) sub routine (Em33_sub_tbl) */
    SHORT Unk14;                       /* 0x14 (0x1E0) */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    signed char Unk18;                 /* 0x18 (0x1E4) side of the last dive */
    signed char Unk19;                 /* 0x19 (0x1E5) */
    UCHAR pad1A[0x1C - 0x1A];
    ULONG Rtn_save;                    /* 0x1C (0x1E8) Routine_0..3 to return to after the dive */
    SHORT Unk20;                       /* 0x20 (0x1EC) */
    SHORT Unk22;                       /* 0x22 (0x1EE) */
    SHORT Unk24;                       /* 0x24 (0x1F0) */
    SHORT Old_life;                    /* 0x26 (0x1F2) */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
} EM33_FREE;

/* free work of a body segment at 0x1CC */
typedef struct tagEM33_BODY {
    LONG Parts;                        /* 0x00 (0x1CC) head part followed */
    LONG Oya;                          /* 0x04 (0x1D0) index of the head work (func_800336B4) */
    UCHAR pad08[0x0A - 0x08];
    signed char Unk0A;                 /* 0x0A (0x1D6) */
    UCHAR pad0B[0x13 - 0x0B];
    UCHAR Rtn;                         /* 0x13 (0x1DF) */
    UCHAR pad14[0x26 - 0x14];
    SHORT Old_life;                    /* 0x26 (0x1F2) */
} EM33_BODY;

#define EM33_W(p) ((EM33_FREE *)(p)->Unk1CC)
#define EM33_B(p) ((EM33_BODY *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c func_8004AC54) */
typedef struct tagEM33_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C + 4 * key */
        USHORT pad;
    } Key[3];
} EM33_SABUN;
#define EM33_SABUN_P(pP) ((EM33_SABUN *)(pP)->pUnk98)

/* the player work as G lays it out, followed by the work list: em33 reaches G.pEnemy[] from
   pPl = &Pl_work (one base register; G.pEnemy itself is a second address) */
typedef struct tagEM33_PL {
    PLAYER_WORK Player;                /* Pl_work = G.Player */
    SHORT Pl_poison_timer;             /* 0x1CC G.Pl_poison_timer */
    UCHAR pad1CE[0x1D0 - 0x1CE];
    ENEMY_WORK *pWork[42];             /* 0x1D0 = G.pEnemy */
} EM33_PL;

/* Spd_z by Move_cnt / 10 while surfacing (copied to the stack as one block) */
typedef struct tagEM33_SPD_TBL {
    SHORT Spd[8];
} EM33_SPD_TBL;

extern const EM33_SPD_TBL Em33_spd_z_tbl; /* { -10, 14, -20, 28, -24, 20, -16, 8 } */
extern USHORT Em33_vib_tbl[]; /* pad vibration scripts */
extern void (*Em33_routine_0[8])();
extern USHORT Em33_col_size[10]; /* body collision W0/D0 by segment */
extern void (*Em33_move_tbl0[11])();
extern void (*Em33_move_tbl1[11])();
extern void (*Em33_sub_tbl[5])();
extern void (*Em33_damage_tbl[][2])(); /* index weapon Unk12C - 1 */

void func_801000D4(ENEMY_WORK *pEm);
void func_80100424(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801009DC(ENEMY_WORK *pEm, LONG No, ENEMY_WORK *pOya, LONG Parts);
void func_80100B5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100B7C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100C00(ENEMY_WORK *pEm);
void func_80100C88(void);
void func_80100C90(ENEMY_WORK *pEm);
void func_80100E0C(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
void func_80100FC8(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
void func_801011C8(ENEMY_WORK *pEm);
ENEMY_WORK *func_8010138C(ENEMY_WORK *pEm);
LONG func_80101454(ENEMY_WORK *pEm);
void func_8010183C(ENEMY_WORK *pEm, LONG Step, LONG Dir);
void func_801018DC(ENEMY_WORK *pEm, LONG Cnt);
void func_80101AE8(ENEMY_WORK *pEm);
void func_80101B30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010201C(ENEMY_WORK *pEm);
void func_80102130(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801023A0(ENEMY_WORK *pEm);
void func_80102484(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102D30(ENEMY_WORK *pEm);
void func_801030D8(void);
void func_801030E0(ENEMY_WORK *pEm);
void func_8010316C(ENEMY_WORK *pEm);
void func_80103208(ENEMY_WORK *pEm);
void func_8010324C(ENEMY_WORK *pEm);
void func_8010342C(ENEMY_WORK *pEm, LONG Flg);
LONG func_80103660(ENEMY_WORK *pEm);
void func_80103778(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801042DC(ENEMY_WORK *pEm);
void func_801043A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104CFC(void);
void func_80104D04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801051A4(void);
void func_801051AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010532C(ENEMY_WORK *pEm);
void func_801053FC(ENEMY_WORK *pEm);
void func_80105438(ENEMY_WORK *pEm);
void func_801055AC(ENEMY_WORK *pEm);
void func_80105704(ENEMY_WORK *pEm);
void func_8010582C(ENEMY_WORK *pEm);
void func_80105980(ENEMY_WORK *pEm);
void func_80105A48(ENEMY_WORK *pEm, LONG No);
void func_80105B40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105BB8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105D74(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105F10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106084(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801061C4(ENEMY_WORK *pEm);
void func_801063CC(ENEMY_WORK *pEm);
void func_80106668(ENEMY_WORK *pEm);
void func_80106C48(ENEMY_WORK *pEm);
void func_80106D64(ENEMY_WORK *pEm);
void func_80107074(ENEMY_WORK *pEm, LONG No, LONG Type);
void func_80107160(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010718C(ULONG No, ENEMY_WORK *pEm, LONG *pPos);
void func_80107204(void);
void func_8010722C(void);

#endif /* OVL_EM33_H */
