#ifndef OVL_EM3E_H
#define OVL_EM3E_H
/*
 * Enemy module em3e (enemy ids 0x3E, 0x3F): a hovering machine with two
 * rotors and two side guns that flies around a fixed zone, fires when the
 * player is level with it, and crashes on a scenario request. RE2's
 * EM3E.C/EM3F.C are unrelated room props, so names are RE3 only.
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "sub.h"

/* the enemy's free work at 0x1CC */
typedef struct tagEM3E_FREE {
    UCHAR Parts_num;                   /* 0x00 (0x1CC) parts flagged 0x400 at death */
    UCHAR Rotor0;                      /* 0x01 (0x1CD) rotor part */
    UCHAR Abr0;                        /* 0x02 (0x1CE) semi-transparent part */
    UCHAR Rotor1;                      /* 0x03 (0x1CF) rotor part (also semi-transparent) */
    UCHAR Sway_rtn;                    /* 0x04 (0x1D0) body pitch sway */
    UCHAR Bob_rtn;                     /* 0x05 (0x1D1) height bob */
    UCHAR Sway_cnt;                    /* 0x06 (0x1D2) */
    UCHAR Bob_cnt;                     /* 0x07 (0x1D3) */
    UCHAR Sway_time;                   /* 0x08 (0x1D4) */
    UCHAR Bob_time;                    /* 0x09 (0x1D5) */
    UCHAR pad0A[0x0C - 0x0A];
    USHORT Timer;                      /* 0x0C (0x1D8) explosion timer / fall speed << 4 */
    SHORT Turn;                        /* 0x0E (0x1DA) spin speed (damage); start Pos_y (crash) */
    SHORT Start_x;                     /* 0x10 (0x1DC) crash: start Pos_x */
    SHORT Fall_x;                      /* 0x12 (0x1DE) crash: x distance covered */
    USHORT Time;                       /* 0x14 (0x1E0) frames alive (+60 per hit) */
    USHORT Dir;                        /* 0x16 (0x1E2) flight direction */
    USHORT Shot_wait;                  /* 0x18 (0x1E4) */
    SHORT Tilt;                        /* 0x1A (0x1E6) body pitch from the x speed */
    SHORT Old_life;                    /* 0x1C (0x1E8) Life of the last frame */
} EM3E_FREE;

#define EM3E_W(p) ((EM3E_FREE *)(p)->Unk1CC)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

extern signed char Rsin_tbl[256] __asm__("D_80098828"); /* 0x80098828 sine, 256 steps, +-127 */

extern void (*Em3e_routine_0[8])();
extern void (*Em3e_move_tbl[2])();
extern void (*Em3e_fly_tbl[2])();
extern void (*Em3e_damage_tbl[31])(); /* index weapon Unk12C - 1 */
extern void (*Em3e_sce_tbl[2])();

void func_80100020(ENEMY_WORK *pEm);
void func_801000A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010027C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100360(ENEMY_WORK *pEm);
void func_801003C4(void);
void func_801003CC(ENEMY_WORK *pEm);
void func_801004F4(ENEMY_WORK *pEm);
void func_801005E4(ENEMY_WORK *pEm);
void func_80100674(ENEMY_WORK *pEm);
void func_80100704(ENEMY_WORK *pEm);
void func_80100788(ENEMY_WORK *pEm);
void func_80100878(ENEMY_WORK *pEm);
void func_80100890(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100908(ENEMY_WORK *pEm);
void func_80100A10(ENEMY_WORK *pEm);
void func_80100BBC(ENEMY_WORK *pEm);
void func_80100C0C(ENEMY_WORK *pEm); /* Em3e_parts_hide */
void func_80100C80(ENEMY_WORK *pEm, ULONG No, LONG Size); /* Em3e_parts_size */
void func_80100CAC(ENEMY_WORK *pEm); /* Em3e_parts_show */
void func_80100D24(ENEMY_WORK *pEm); /* Em3e_parts_trans */
void func_80100D70(ENEMY_WORK *pEm); /* Em3e_parts_trans_off */
void func_80100DC0(ENEMY_WORK *pEm, ULONG No); /* Em3e_parts_abr */
void func_80100E0C(ENEMY_WORK *pEm); /* Em3e_rotor */
void func_80100EAC(ENEMY_WORK *pEm, LONG Mode); /* Em3e_shot_check */
void func_80100F94(ENEMY_WORK *pEm); /* Em3e_shot_r */
void func_80101008(ENEMY_WORK *pEm); /* Em3e_shot_l */
void func_8010107C(ENEMY_WORK *pEm); /* Em3e_explode */
void func_80101210(ENEMY_WORK *pEm); /* Em3e_hover */
void func_80101570(ENEMY_WORK *pEm, LONG Rtn0, LONG Rtn1);
void func_80101580(ENEMY_WORK *pEm, LONG Rtn1); /* Em3e_rtn1_set */
void func_8010158C(ENEMY_WORK *pEm); /* Em3e_add_speed */
void func_801015C0(ENEMY_WORK *pEm); /* Em3e_retarget */

#endif /* OVL_EM3E_H */
