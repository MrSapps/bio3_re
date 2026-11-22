#ifndef OVL_EM16_H
#define OVL_EM16_H
/*
 * Enemy module em16 (zombie variants, enemy ids 0x16, 0x18-0x1F). Its code is a
 * near copy of em10 (RE2 EM10.C + EM10_S0.C): same routine tables at the same
 * .data offsets, mostly identical routines; Init (func_8010043C) differs.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep.h"
#include "joint.h"
#include "main.h"
#include "player.h"

/* em16's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]; same layout as em10's EM10_FREE) */
typedef struct tagEM16_FREE {
    UCHAR Walk_no;                     /* 0x00 (0x1CC) RE2 FREE_UB(0): walk motion, Rnd() % 3 */
    UCHAR Unk01;                       /* 0x01 (0x1CD) RE2 FREE_UB(1) */
    USHORT Flg;                        /* 0x02 (0x1CE) RE2 FREE_US(2) */
    UCHAR Unk04;                       /* 0x04 (0x1D0) */
    UCHAR Unk05;                       /* 0x05 (0x1D1) */
    UCHAR Unk06;                       /* 0x06 (0x1D2) */
    UCHAR Unk07;                       /* 0x07 (0x1D3) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    UCHAR Unk0A;                       /* 0x0A (0x1D6) RE2 FREE_UB(0xa) */
    signed char Unk0B;                 /* 0x0B (0x1D7) RE2 FREE_UB(0xb), (Rnd() & 0xF) + 0x10 */
    ULONG Unk0C;                       /* 0x0C (0x1D8) */
    ULONG Routine;                     /* 0x10 (0x1DC) copy of Routine_0..3 (RE2 OVL_FREE[4]) */
    ULONG Unk14;                       /* 0x14 (0x1E0) */
    UCHAR Unk18;                       /* 0x18 (0x1E4) RE2 FREE_UB(0x18), G.Unk75EA bit 0 when it runs out */
    UCHAR Unk19;                       /* 0x19 (0x1E5) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    UCHAR Unk1C;                       /* 0x1C (0x1E8) */
    UCHAR Root_no;                     /* 0x1D (0x1E9) RE2 FREE_UB(0x1d), Root_ck arg */
    USHORT Cnt;                        /* 0x1E (0x1EA) RE2 FREE_US(0x1e), frame counter */
    UCHAR Unk20;                       /* 0x20 (0x1EC) */
    UCHAR Unk21;                       /* 0x21 (0x1ED) RE2 FREE_UB(0x21), timer */
    UCHAR Unk22;                       /* 0x22 (0x1EE) */
    UCHAR Unk23;                       /* 0x23 (0x1EF) */
    UCHAR Ude_down;                    /* 0x24 (0x1F0) non-zero: the arm-down routine runs */
    UCHAR Unk25;                       /* 0x25 (0x1F1) */
    UCHAR Unk26;                       /* 0x26 (0x1F2) RE2 FREE_UB(0x26), timer */
    UCHAR Unk27;                       /* 0x27 (0x1F3) Rnd() % 3 + 3 */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
    union {
        struct {
            SHORT Unk2C;               /* 0x2C (0x1F8) arm-down routine */
            UCHAR Unk2E;               /* 0x2E (0x1FA) (Rnd() & 1) + 6 */
            UCHAR Unk2F;               /* 0x2F (0x1FB) timer */
        } w;
        LONG Esp_idx;                  /* 0x2C (0x1F8) Em16_mv11: index into Em16_mv11_esp */
    } u2C;
} EM16_FREE;

/* effect script of Em16_mv11 (16 bytes per entry, Cnt 0xFF ends it) */
typedef struct tagEM16_ESP {
    SHORT Cnt;                         /* 0x0 Routine_3 frame, 0xFF: end */
    SHORT Parts_no;                    /* 0x2 part, 0xEE: next Routine_2, 0xFF: at the enemy position */
    SHORT Ofs_x;                       /* 0x4 */
    SHORT Ofs_y;                       /* 0x6 */
    SHORT Ofs_z;                       /* 0x8 */
    SHORT Id_lo;                       /* 0xA effect id, low byte */
    SHORT Id_hi;                       /* 0xC effect id, high byte */
    SHORT Esp_no;                      /* 0xE */
} EM16_ESP;

#define EM16_W(p) ((EM16_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..Move_cnt written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
/* RE2 Super_vector: Add_dir/Unk14A/Unk14C viewed as the SVECTOR of the knock-down rotation (em16 dm02) */
#define SUPER_VECTOR(p) ((SVECTOR *)&(p)->Add_dir)
/* the same free work as two words (colour pair, e.g. 0x20 / 0x101020) */
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)

/* exe functions without a shared prototype (signatures as defined in src/game) */

extern USHORT Em16_life[16] __asm__("D_8010C350");
extern USHORT Em16_life_easy[16] __asm__("D_8010C370");
extern void (*Em16_routine_0[])() __asm__("D_8010C390");
extern void (*Em16_Move_type[])() __asm__("D_8010C3B4");
extern void (*Em16_mv_br_type00[])() __asm__("D_8010C3F4");
extern void (*Em16_mv_type00[])() __asm__("D_8010C434");
extern void (*Em16_mv_br_type01[])() __asm__("D_8010C474");
extern void (*Em16_mv_type01[])() __asm__("D_8010C484");
extern void (*Em16_dm[][3])() __asm__("D_8010C494");
extern void (*Em16_haidm[])() __asm__("D_8010C608");
extern void (*Em16_die[][3])() __asm__("D_8010C684");
extern void (*Em16_sce[])() __asm__("D_8010C7F8");
extern void (*Pldamage_Em16_tbl[])() __asm__("D_8010C80C");
extern void (*Pldie_Em16_tbl[])() __asm__("D_8010C81C");
extern USHORT Em16_walk_rnd[32] __asm__("D_8010C824"); /* walk time, 0x8000: turn slower */
extern USHORT Em16_mv0d_rnd[16] __asm__("D_8010C878"); /* time, 0x8000: turn the other way */
extern EM16_ESP Em16_mv11_esp[] __asm__("D_8010C898");
extern UCHAR Em16_dm00_usonum[] __asm__("D_8010CA08"); /* Unk0B decrement per damage type */

void func_801001EC(ENEMY_WORK *pEm);
void func_8010043C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101804(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101844(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801018C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010194C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101BBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101C38(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101CB4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101E94(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101F50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101FEC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010208C(void);
void func_80102094(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102208(void);
void func_80102210(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102748(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801027D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102EB0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801034D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103564(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010393C(ENEMY_WORK *pEm);
void func_80103CE4(void);
void func_80103CEC(ENEMY_WORK *pEm);
void func_80103DD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103FA0(ENEMY_WORK *pEm);
void func_80104194(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104570(ENEMY_WORK *pEm);
void func_80104764(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104A68(void);
void func_80104A70(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010532C(ENEMY_WORK *pEm);
void func_80105458(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801056B8(void);
void func_801056C0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010590C(void);
void func_80105914(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105E74(void);
void func_80105E7C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801060FC(void);
void func_80106104(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106218(ENEMY_WORK *pEm);
void func_801062EC(void);
void func_801062F4(void);
void func_801062FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106624(void);
void func_8010662C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106938(void);
void func_80106940(void);
void func_80106948(ENEMY_WORK *pEm);
void func_80106AF8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106D98(ENEMY_WORK *pEm);
void func_80106E7C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106F8C(void);
void func_80106F94(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010703C(void);
void func_80107044(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107148(void);
void func_80107150(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801076B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107D58(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108244(ENEMY_WORK *pEm);
void func_80108450(ENEMY_WORK *pEm);
void func_801085A8(ENEMY_WORK *pEm);
void func_80108764(ENEMY_WORK *pEm);
void func_80108914(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109200(void);
void func_80109208(void);
void func_80109210(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801097B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801098FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109C78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109E84(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A1D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A504(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A654(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010AD20(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B17C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B5B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B808(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010BE90(void);
void func_8010BE98(ENEMY_WORK *pEm, ULONG *pKan, LONG No);
void func_8010C024(ENEMY_WORK *pEm);
void func_8010C140(ENEMY_WORK *pEm);

#endif /* OVL_EM16_H */
