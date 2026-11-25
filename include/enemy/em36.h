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
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "sub.h"
#include "pl_wep.h"
#include "joint.h"

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

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c Sa_morph_blend) */
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

/* PARTS_W free work of the morphing body parts (Sa_morph_step blends keys Key_no - 1 and Key_no) */
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
/* colour word (r, g, b, code) of the shadow quads */
#define KAGE_RGB(pK, n) (*(ULONG *)&(pK)->Prim[n].r0)

extern SHORT Em36_punch_box0[4];
extern SHORT Em36_punch_box1[4];
extern SHORT Em36_lunge_box0[4];
extern SHORT Em36_lunge_box1[4];
extern SHORT Em36_grab_box0[4];
extern SHORT Em36_grab_box1[4];
extern SHORT Em36_sweep_box0[4];
extern SHORT Em36_sweep_box1[4];
extern SHORT Em36_sweep_box2[4];
extern SHORT Em36_sweep_box3[4];
extern SHORT Em36_swing_box0[4];
extern SHORT Em36_swing_box1[4];
extern VECTOR Em36_route_pos[2];
extern VECTOR Em36_route[][3];
extern void (*Em36_routine_0[8])();
extern VECTOR Em36_lost_pos;
extern void (*Em36_Move_type[14])();
extern void (*Em36_mv_br00[21])();
extern void (*Em36_mv00[21])();
extern void (*Em36_mv_br01[21])();
extern void (*Em36_mv01[21])();
extern void (*Em36_mv_br02[21])();
extern void (*Em36_mv_br03[21])();
extern void (*Em36_mv03[21])();
extern void (*Em36_mv_br04[21])();
extern void (*Em36_mv_br05[21])();
extern UCHAR Em36_combo_tbl[8][8];
extern void (*Em36_dm[32][3])();
extern void (*Em36_dm2[33])();
extern void (*Em36_sce[12])();
extern void (*Em36_die[4])();
extern void (*Em36_die2[1])();
extern HIT_AREA Em36_hit_area;

void func_801000F8(ENEMY_WORK *pEm);
void func_8010076C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010105C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101098(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010111C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801011EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101288(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801013F4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101520(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101688(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101760(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101838(ENEMY_WORK *pEm);
void func_801018BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101958(ENEMY_WORK *pEm);
void func_80101CC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101EC8(ENEMY_WORK *pEm);
void func_80102204(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102420(ENEMY_WORK *pEm);
void func_80102504(ENEMY_WORK *pEm);
void func_80102608(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102894(ENEMY_WORK *pEm);
void func_801029E0(ENEMY_WORK *pEm);
void func_80102B00(ENEMY_WORK *pEm);
void func_80102BD4(ENEMY_WORK *pEm);
void func_80102C44(ENEMY_WORK *pEm);
void func_80102CC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102E70(ENEMY_WORK *pEm);
void func_8010304C(ENEMY_WORK *pEm);
void func_801030C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801031E0(void);
void func_801031E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010332C(void);
void func_80103334(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103A20(ENEMY_WORK *pEm);
void func_80103B9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103F88(ENEMY_WORK *pEm);
void func_801040E4(void);
void func_801040EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801041BC(void);
void func_801041C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010435C(void);
void func_80104364(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801044BC(void);
void func_801044C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104CA4(void);
void func_80104CAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105114(void);
void func_8010511C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010580C(void);
void func_80105814(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105BF0(void);
void func_80105BF8(void);
void func_80105C00(void);
void func_80105C08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106260(void);
void func_80106268(void);
void func_80106270(void);
void func_80106278(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106430(void);
void func_80106438(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010698C(void);
void func_80106994(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106D98(ENEMY_WORK *pEm);
void func_80106F2C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107390(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107724(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107990(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107C60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107D5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108134(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801082BC(ENEMY_WORK *pEm);
void func_8010832C(ENEMY_WORK *pEm);
void func_801084F8(ENEMY_WORK *pEm);
void func_801086F0(ENEMY_WORK *pEm);
void func_80108988(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108AC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108EA4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108F00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108F9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109050(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109164(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109224(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em34_sce05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109440(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801096B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109900(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109B30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109FCC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A29C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A53C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A684(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A6C0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010AE3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B150(void);
void func_8010B158(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B194(void);
void func_8010B19C(ENEMY_WORK *pEm);
void func_8010B308(ENEMY_WORK *pEm);
void Em34_foot_set(ENEMY_WORK *pEm, LONG Unused, LONG Leg);
void Em34_blood_set(ENEMY_WORK *pEm);
void func_8010B7FC(void);
void func_8010B804(ENEMY_WORK *pEm);
void Em34_blood_spray(ENEMY_WORK *pEm);
void func_8010BBE8(ENEMY_WORK *pEm);
void func_8010BDD0(ENEMY_WORK *pEm);
void func_8010BEAC(ENEMY_WORK *pEm);
void func_8010C4FC(ENEMY_WORK *pEm);
void func_8010C6CC(ENEMY_WORK *pEm);
void func_8010C764(ENEMY_WORK *pEm);

#endif /* OVL_EM36_H */
