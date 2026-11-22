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

extern EM34_MOVE3 Em34_catch_move __asm__("D_8010003C"); /* .rodata, shared by two motions */
extern EM34_MOVE3 Em34_pl_stop __asm__("D_80100040");
extern SVECTOR Em34_shot_ofs __asm__("D_80100044");
extern EM34_MOVE3 Em34_pl_move __asm__("D_80100140");
extern void (*Em34_routine_0[8])() __asm__("D_8010BF28");
extern void (*Em34_Move_type[10])() __asm__("D_8010BF4C");
extern void (*Em34_mv_br_type00[20])() __asm__("D_8010BF74");
extern void (*Em34_mv[20])() __asm__("D_8010BFC4");
extern void (*Em34_mv_br_type01[20])() __asm__("D_8010C014");
extern void (*Em34_mv_br_type00h[20])() __asm__("D_8010C064");
extern void (*Em34_mv_br_type01h[20])() __asm__("D_8010C0B4");
extern void (*Em34_mv_br_type02[20])() __asm__("D_8010C104");
extern void (*Em34_mv_br_type03[20])() __asm__("D_8010C154");
extern SHORT Em34_at_box[4] __asm__("D_8010C1A4");
extern SHORT Em34_at_data[10] __asm__("D_8010C1AC");
extern SHORT Em34_at_box2[4] __asm__("D_8010C1C0");
extern SHORT Em34_at_box3[4] __asm__("D_8010C1C8");
extern UCHAR Em34_morph_tbl[8] __asm__("D_8010C1D0");
extern SHORT Em34_at_box4[4] __asm__("D_8010C1D8");
extern SHORT Em34_at_box5[4] __asm__("D_8010C1E0");
extern SHORT Em34_at_box6[4] __asm__("D_8010C1E8");
extern SHORT Em34_at_box7[4] __asm__("D_8010C1F0");
extern void (*Em34_damage[96])() __asm__("D_8010C1F8");
extern void (*Em34_die[93])() __asm__("D_8010C378");
extern void (*Em34_sce[13])() __asm__("D_8010C4EC");
extern void (*Em34_pldamage[4])() __asm__("D_8010C520");
extern void (*Em34_pldie[2])() __asm__("D_8010C530");
extern ULONG *Em34_tmd_new5 __asm__("D_8010C538");
extern ULONG *Em34_tmd_old5 __asm__("D_8010C53C");
extern ULONG *Em34_tmd_new8 __asm__("D_8010C540");
extern ULONG *Em34_tmd_old8 __asm__("D_8010C544");
extern HIT_AREA Em34_hit_area __asm__("D_8010C558");
extern SHORT Em34_floor __asm__("D_8010C564"); /* BSS past the file end (ovl/em34.bss.ld) */

void Em134(ENEMY_WORK *pEm) __asm__("func_80100180");
void Em34_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)  __asm__("func_80100638");
void Em34_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100B70");
void Em34_Move_type00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100BAC");
void Em34_Move_type01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100C60");
void Em34_Move_type02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100D14");
void Em34_Move_type03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80100D98");
void Em34_taiki_br(ENEMY_WORK *pEm) __asm__("func_80100F64");
void Em34_taiki_br_h(ENEMY_WORK *pEm) __asm__("func_8010100C");
void Em34_taiki(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801010AC");
void Em34_walk_br0(ENEMY_WORK *pEm) __asm__("func_80101148");
void Em34_walk_br1(ENEMY_WORK *pEm) __asm__("func_801013C0");
void Em34_walk_br3(ENEMY_WORK *pEm) __asm__("func_801016FC");
void Em34_walk_br0h(ENEMY_WORK *pEm) __asm__("func_80101A08");
void Em34_walk_br1h(ENEMY_WORK *pEm) __asm__("func_80101C1C");
void Em34_walk(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80101ED8");
void Em34_run_br0(ENEMY_WORK *pEm) __asm__("func_8010217C");
void Em34_run_br1(ENEMY_WORK *pEm) __asm__("func_80102260");
void Em34_run_br0h(ENEMY_WORK *pEm) __asm__("func_80102464");
void Em34_run_br1h(ENEMY_WORK *pEm) __asm__("func_8010252C");
void Em34_run(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102700");
void Em34_back_br0(ENEMY_WORK *pEm) __asm__("func_80102970");
void Em34_back_br1(ENEMY_WORK *pEm) __asm__("func_80102A88");
void Em34_back_br0h(ENEMY_WORK *pEm) __asm__("func_80102C34");
void Em34_back_br1h(ENEMY_WORK *pEm) __asm__("func_80102D48");
void Em34_back(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80102F18");
void func_80103048(void);
void Em34_turn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103050");
void func_80103184(void);
void Em34_punch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010318C");
void func_80103AD0(void);
void Em34_rush(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103AD8");
void func_80103F30(void);
void Em34_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80103F38");
void func_801043EC(void);
void Em34_throw(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801043F4");
void func_80104828(void);
void Em34_catch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80104830");
void func_80104A4C(void);
void Em34_shoot(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)  __asm__("func_80104A54");
void func_80105590(void);
void Em34_roar(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80105598");
void func_80105964(void);
void Em34_arm_grow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010596C");
void func_80105B44(void);
void Em34_bite(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80105B4C");
void func_8010625C(ENEMY_WORK *pEm);
void Em34_back_grab(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801062C0");
void func_80106630(void);
void Em34_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80106638");
void func_8010696C(void);
void Em34_side_step(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80106974");
void func_80106AC4(void);
void Em34_club(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80106ACC");
void func_80106F94(void);
void Em34_swing(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80106F9C");
void func_80107434(void);
void Em34_rocket(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010743C");
void Em34_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80107928");
void Em34_dm_flinch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80107B30");
void Em34_dm_down(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80107CA0");
void Em34_dm_no(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80107E2C");
void Em34_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80107EB8");
void Em34_die_burn(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108008");
void Em34_Sce(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010833C");
void Em34_sce00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108398");
void Em34_sce01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108434");
void Em34_sce02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801084E8");
void Em34_sce03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108620");
void Em34_sce04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801086E0");
void Em34_sce05(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108888");
void Em34_sce06(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010896C");
void Em34_sce07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108BE0");
void Em34_sce09(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108DC8");
void Em34_sce08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80108E90");
void Em34_sce10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_801091D0");
void func_801094B4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em34_sce12(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010953C");
void Em34_Dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80109EB0");
void Em134_Pldamage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80109FB8");
void Em134_pldm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_80109FF4");
void Em134_pldm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010A674");
void Em134_pldm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010A8A0");
void Em134_Pldie(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq) __asm__("func_8010AC10");
void func_8010AC4C(void);
void Em34_foot_set(ENEMY_WORK *pEm, LONG Unused, LONG Leg) __asm__("func_8010AC54");
void Em34_blood_set(ENEMY_WORK *pEm) __asm__("func_8010AD48");
void Em34_after0(ENEMY_WORK *pEm) __asm__("func_8010AF58");
void Em34_after1(ENEMY_WORK *pEm) __asm__("func_8010B15C");
void func_8010B574(void);
void Em34_knock_back(ENEMY_WORK *pEm) __asm__("func_8010B57C");
void Em34_blood_spray(ENEMY_WORK *pEm) __asm__("func_8010B768");
void Em34_knock_back2(ENEMY_WORK *pEm) __asm__("func_8010B964");
void Em34_after2(ENEMY_WORK *pEm) __asm__("func_8010BB50");
void Em34_after3(ENEMY_WORK *pEm) __asm__("func_8010BCC0");
void Em34_before(ENEMY_WORK *pEm) __asm__("func_8010BDBC");

#endif /* OVL_EM34_H */
