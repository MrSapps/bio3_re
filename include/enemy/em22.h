#ifndef OVL_EM22_H
#define OVL_EM22_H
/*
 * Enemy module em22 (enemy ids 0x22 and 0x24: the hunters). Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2). RE2 has no hunter: the module keeps RE2's
 * enemy frame (EM_MAIN, Em_routine_0 = Init/Move/Damage/Die/.../Dead, Free[]
 * work at 0x1CC) but its move logic is new: Move runs a think routine
 * (em22_think_tbl) and then an action routine (em22_move_tbl), both indexed
 * by Routine_1.
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"
#include "sca.h"
#include "sub.h"
#include "pl_wep.h"

/* em22's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM22_FREE {
    SHORT Unk00;                       /* 0x00 (0x1CC) */
    UCHAR Unk02;                       /* 0x02 (0x1CE) */
    UCHAR Unk03;                       /* 0x03 (0x1CF) timer, (Rnd() & 0x1F) + 30 at init */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    SHORT Unk0A;                       /* 0x0A (0x1D6) */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    UCHAR Unk0E;                       /* 0x0E (0x1DA) */
    UCHAR Unk0F;                       /* 0x0F (0x1DB) */
    UCHAR Unk10;                       /* 0x10 (0x1DC) */
    UCHAR Unk11;                       /* 0x11 (0x1DD) */
    UCHAR Unk12;                       /* 0x12 (0x1DE) */
    UCHAR Unk13;                       /* 0x13 (0x1DF) */
    UCHAR Unk14;                       /* 0x14 (0x1E0) */
    UCHAR pad15;
    UCHAR Unk16;                       /* 0x16 (0x1E2) */
    UCHAR Unk17;                       /* 0x17 (0x1E3) */
    UCHAR Unk18;                       /* 0x18 (0x1E4) */
    signed char Unk19;                 /* 0x19 (0x1E5) */
    UCHAR Unk1A;                       /* 0x1A (0x1E6) */
    signed char Unk1B;                 /* 0x1B (0x1E7) timer */
    SHORT Unk1C;                       /* 0x1C (0x1E8) */
    SHORT Unk1E;                       /* 0x1E (0x1EA) */
    USHORT Unk20;                      /* 0x20 (0x1EC) */
    UCHAR Unk22;                       /* 0x22 (0x1EE) */
    UCHAR Unk23;                       /* 0x23 (0x1EF) */
    SHORT Old_spd_x;                   /* 0x24 (0x1F0) Spd_x of the last frame */
    USHORT Unk26;                      /* 0x26 (0x1F2) timer, 150 at init */
    UCHAR Unk28;                       /* 0x28 (0x1F4) */
    UCHAR Unk29;                       /* 0x29 (0x1F5) */
    UCHAR Unk2A;                       /* 0x2A (0x1F6) */
    UCHAR Unk2B;                       /* 0x2B (0x1F7) */
    SHORT Unk2C;                       /* 0x2C (0x1F8) */
    SHORT Unk2E;                       /* 0x2E (0x1FA) */
} EM22_FREE;

#define EM22_W(p) ((EM22_FREE *)(p)->Unk1CC)
/* Routine_2..3 written as one halfword */
/* Routine_0..1 read as one halfword */
#define EM_RTN01(p) (*(SHORT *)&(p)->Routine_0)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */

extern USHORT Em22_life[16];
extern USHORT Em22_life_easy[16];
extern void (*Em22_routine_0[8])();
extern void (*Em22_think_tbl[16])();
extern void (*Em22_move_tbl[16])();
extern UCHAR Em22_jump_tbl[12];
extern void (*Em22_sce_tbl[4])();
extern void (*Em22_dm_tbl[3])();
extern UCHAR Em22_dm_mask[32];
extern UCHAR D_80109488[32];
extern UCHAR Em22_blood_parts[4];
extern signed char Em22_drop_frame[8];
extern signed char Em22_drop_parts[8];

void func_801000BC(ENEMY_WORK *pEm);
void func_8010023C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100680(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100844(ENEMY_WORK *pEm);
void func_80100A54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100B08(ENEMY_WORK *pEm);
void func_801011BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801012E4(ENEMY_WORK *pEm);
void func_80101858(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101914(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG func_80101988(ENEMY_WORK *pEm);
LONG func_80101A30(ENEMY_WORK *pEm);
void func_80101AA4(ENEMY_WORK *pEm);
void func_80101AAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101EF0(ENEMY_WORK *pEm, ENEMY_WORK *pT);
void func_80101F60(ENEMY_WORK *pEm);
void func_80102044(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010281C(ENEMY_WORK *pEm);
void func_8010283C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102A10(ENEMY_WORK *pEm);
void func_80102AEC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_80103110(ENEMY_WORK *pEm, LONG Flg);
void func_8010316C(ENEMY_WORK *pEm, LONG Flg, VECTOR *pV);
LONG func_80103260(ENEMY_WORK *pEm);
void func_801036B0(ENEMY_WORK *pEm);
void func_801036B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103B34(ENEMY_WORK *pEm);
void func_80103B3C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103CB4(ENEMY_WORK *pEm);
void func_80103CBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103E38(ENEMY_WORK *pEm);
void func_80103E40(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801041C8(ENEMY_WORK *pEm);
void func_801041D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104864(ENEMY_WORK *pEm);
void func_8010486C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104974(ENEMY_WORK *pEm);
void func_80104A10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104D78(ENEMY_WORK *pEm);
void func_80104D80(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_80104FF0(ENEMY_WORK *pEm);
void func_801051B8(ENEMY_WORK *pEm);
void func_801051C0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010555C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801055B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105730(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010598C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105C64(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106118(ENEMY_WORK *pEm);
void func_801061FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010649C(ENEMY_WORK *pEm);
void func_80106774(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801068F4(ENEMY_WORK *pEm);
void func_80106A6C(ENEMY_WORK *pEm);
void func_80106BDC(ENEMY_WORK *pEm);
void func_80106D00(ENEMY_WORK *pEm);
void func_80106D08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106F8C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107448(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801078B8(ENEMY_WORK *pEm);
void func_80107A1C(ENEMY_WORK *pEm);
void func_80107B94(ENEMY_WORK *pEm);
void func_80107D0C(ENEMY_WORK *pEm);
void func_80107E30(ENEMY_WORK *pEm);
void func_80108110(ENEMY_WORK *pEm);
void func_8010823C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108448(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void func_80108680(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void func_8010880C(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void func_80108AAC(ENEMY_WORK *pEm);
UCHAR func_80108CA0(void);
LONG func_80108CB8(ENEMY_WORK *pEm, ULONG Rtn);
LONG func_80108D44(void);
LONG func_80108DAC(ENEMY_WORK *pEm);
void func_80109058(ENEMY_WORK *pEm);
LONG func_801091A0(ENEMY_WORK *pEm);

#endif /* OVL_EM22_H */
