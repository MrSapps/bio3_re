#ifndef OVL_EM10_H
#define OVL_EM10_H
/*
 * Enemy module em10 (zombie family, enemy ids 0x10-0x15, 0x17, 0x2F), the
 * successor of RE2's EM10.C + EM10_S0.C. Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2); the routine tables are in the module's .data.
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep.h"
#include "joint.h"
#include "main.h"
#include "player.h"

/* em10's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[], OVL_FREE(p) in RE2 EM10.C:
 * FREE_UB(o) is the byte at 0x1CC + o here) */
typedef struct tagEM10_FREE {
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
    KAGE_WORK *pKage;                  /* 0x0C (0x1D8) second shadow (Kage_work_set) */
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
    UCHAR Ude_down;                    /* 0x24 (0x1F0) non-zero: Em10_ude_down runs */
    UCHAR Unk25;                       /* 0x25 (0x1F1) */
    UCHAR Unk26;                       /* 0x26 (0x1F2) RE2 FREE_UB(0x26), timer */
    UCHAR Unk27;                       /* 0x27 (0x1F3) Rnd() % 3 + 3 */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
    union {
        struct {
            SHORT Unk2C;               /* 0x2C (0x1F8) Em10_ude_down */
            UCHAR Unk2E;               /* 0x2E (0x1FA) (Rnd() & 1) + 6 */
            UCHAR Unk2F;               /* 0x2F (0x1FB) timer */
        } w;
        LONG Esp_idx;                  /* 0x2C (0x1F8) Em10_mv11: index into Em10_mv11_esp */
    } u2C;
} EM10_FREE;

/* effect script of Em10_mv11 (16 bytes per entry, Cnt 0xFF ends it) */
typedef struct tagEM10_ESP {
    SHORT Cnt;                         /* 0x0 Routine_3 frame, 0xFF: end */
    SHORT Parts_no;                    /* 0x2 part, 0xEE: next Routine_2, 0xFF: at the enemy position */
    SHORT Ofs_x;                       /* 0x4 */
    SHORT Ofs_y;                       /* 0x6 */
    SHORT Ofs_z;                       /* 0x8 */
    SHORT Id_lo;                       /* 0xA effect id, low byte */
    SHORT Id_hi;                       /* 0xC effect id, high byte */
    SHORT Esp_no;                      /* 0xE */
} EM10_ESP;

#define EM10_W(p) ((EM10_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..Move_cnt written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
/* Add_dir..Unk14C used as a rotation vector by the dm routines */
#define SUPER_VECTOR(p) ((SVECTOR *)&(p)->Add_dir)
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)

/* exe functions without a shared prototype (signatures as defined in src/game) */
/* Joint_move (joint.h) seen with the kan/seq pointers and its result (frame-end flag) */

extern USHORT Em10_life[16] __asm__("D_8010C8A8");
extern USHORT Em10_life_easy[16] __asm__("D_8010C8C8");
extern void (*Em10_routine_0[])() __asm__("D_8010C8E8");
extern void (*Em10_Move_type[])() __asm__("D_8010C90C");
extern void (*Em10_mv_br_type00[])() __asm__("D_8010C94C");
extern void (*Em10_mv_type00[])() __asm__("D_8010C98C");
extern void (*Em10_mv_br_type01[])() __asm__("D_8010C9CC");
extern void (*Em10_mv_type01[])() __asm__("D_8010C9DC");
extern void (*Em10_dm[][3])() __asm__("D_8010C9EC");
extern void (*Em10_haidm[])() __asm__("D_8010CB60");
extern void (*Em10_die[][3])() __asm__("D_8010CBDC");
extern void (*Em10_sce[])() __asm__("D_8010CD50");
extern void (*Pldamage_Em10_tbl[])() __asm__("D_8010CD64");
extern void (*Pldie_Em10_tbl[])() __asm__("D_8010CD74");
extern USHORT Em10_walk_rnd[32] __asm__("D_8010CD7C"); /* walk time, 0x8000: turn slower */
extern USHORT Em10_mv0d_rnd[16] __asm__("D_8010CDD0"); /* time, 0x8000: turn the other way */
extern EM10_ESP Em10_mv11_esp[] __asm__("D_8010CDF0");
extern UCHAR Em10_dm00_usonum[] __asm__("D_8010CF60"); /* Unk0B decrement per damage type */

void func_80100254(ENEMY_WORK *pEm);
void func_80100548(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100EA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100EE8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100F6C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100FF0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101260(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010139C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101418(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801015FC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801016B8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101754(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801017F4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101C68(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101DD8(void);
void func_80101DE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102340(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801023D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102AA8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801030A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103138(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103510(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103B48(ENEMY_WORK *pEm);
void func_80103EF0(ENEMY_WORK *pEm);
void func_80104244(ENEMY_WORK *pEm);
void func_80104328(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801044F8(ENEMY_WORK *pEm);
void func_801046EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104AC8(ENEMY_WORK *pEm);
void func_80104CBC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104FC0(void);
void func_80104FC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105884(ENEMY_WORK *pEm);
void func_801059B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105C10(void);
void func_80105C18(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105E64(void);
void func_80105E6C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801063CC(void);
void func_801063D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106654(void);
void func_8010665C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106770(ENEMY_WORK *pEm);
void func_80106844(void);
void func_8010684C(void);
void func_80106854(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106B7C(void);
void func_80106B84(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106E90(void);
void func_80106E98(void);
void func_80106EA0(ENEMY_WORK *pEm);
void func_80107050(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801072F0(ENEMY_WORK *pEm);
void func_801073D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801074E4(void);
void func_801074EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107594(void);
void func_8010759C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801076A0(void);
void func_801076A8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80107C10(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801082B0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010879C(ENEMY_WORK *pEm);
void func_801089A8(ENEMY_WORK *pEm);
void func_80108B00(ENEMY_WORK *pEm);
void func_80108CBC(ENEMY_WORK *pEm);
void func_80108E6C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109758(void);
void func_80109760(void);
void func_80109768(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109D08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109E54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A1D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A3DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A730(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010AA5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010ABAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B278(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010B6D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010BB08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010BD60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010C3E8(void);
void func_8010C3F0(ENEMY_WORK *pEm, LONG Mode, LONG Foot);
void func_8010C57C(ENEMY_WORK *pEm);
void func_8010C698(ENEMY_WORK *pEm);

#endif /* OVL_EM10_H */
