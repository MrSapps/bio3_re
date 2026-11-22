#ifndef OVL_EM32_H
#define OVL_EM32_H
/*
 * Enemy module em32 (enemy id 0x32). Loaded at 0x80100000 (slot 1) or
 * 0x8010D000 (slot 2). A small six-part jumping enemy that respawns at its
 * start position (Type & 0xF != 0). Structure as RE2's EM23.C: routine table
 * Init/Move/-/Damage/(exe)/Die/-/Dead, paired move tables mv_br[]/mv[]
 * (Routine_1), a player-damage table of 4 entries hooked into
 * G.pPldamage_Em[Id - 0x10].
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "player.h"
#include "sca.h"
#include "pad.h"
#include "pl_wep.h"

/* the module's private work in ENEMY_WORK.Unk1CC[] (RE2 Free[]) */
typedef struct tagEM32_FREE {
    SHORT Type_bak;                    /* 0x00 (0x1CC) Type & 0xF (0 with Type & 0x100): respawn type */
    SHORT Pos_x_bak;                   /* 0x02 (0x1CE) start position */
    SHORT Pos_y_bak;                   /* 0x04 (0x1D0) */
    SHORT Pos_z_bak;                   /* 0x06 (0x1D2) */
    SHORT Ground_bak;                  /* 0x08 (0x1D4) Unk122 */
    SHORT Cdir_y_bak;                  /* 0x0A (0x1D6) */
    SHORT Timer0_bak;                  /* 0x0C (0x1D8) */
    SHORT nFloor_bak;                  /* 0x0E (0x1DA) */
    ULONG Be_flg_bak;                  /* 0x10 (0x1DC) Be_flg & ~0x10 */
    UCHAR Unk14;                       /* 0x14 (0x1E0) */
    UCHAR pad15;
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    SHORT Red;                         /* 0x18 (0x1E4) hit flash: subtracted from the part colours */
    UCHAR Red_cnt;                     /* 0x1A (0x1E6) */
    signed char Unk1B;                 /* 0x1B (0x1E7) */
    SHORT Unk1C;                       /* 0x1C (0x1E8) */
    SHORT Atck_cnt;                    /* 0x1E (0x1EA) frames near the player, attack at 31 */
    SHORT Unk20;                       /* 0x20 (0x1EC) */
    SHORT Unk22;                       /* 0x22 (0x1EE) 0: Type 3, waits for the player */
    SHORT Unk24;                       /* 0x24 (0x1F0) */
    SHORT Unk26;                       /* 0x26 (0x1F2) */
    signed char Unk28;                 /* 0x28 (0x1F4) 0: Type 4 */
    signed char Rev;                   /* 0x29 (0x1F5) turn the other way */
} EM32_FREE;

#define EM32_W(p) ((EM32_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x201) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

/* shadow polygons: colour + code word of POLY_FT4 */
#define PRIM_RGBC(p) (*(ULONG *)&(p).r0)

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c func_8004AC54) */
typedef struct tagEM32_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C, 0x10 */
        SHORT pad;
    } Key[2];
} EM32_SABUN;
#define EM32_SABUN_P(pP) ((EM32_SABUN *)(pP)->pUnk98)

extern void (*Em32_routine_0[])() __asm__("D_801041A4");
extern void (*Em32_mv_br[])() __asm__("D_801041C4");
extern void (*Em32_mv[])() __asm__("D_801041E4");
extern SHORT Em32_leap_dir[] __asm__("D_80104204");
extern SHORT Em32_leap_spd[] __asm__("D_8010424C");
extern SHORT Em32_leap2_spd[] __asm__("D_8010430C");
extern void (*Em32_dm_tbl[])() __asm__("D_80104384");
extern void (*Em32_pldm_tbl[])() __asm__("D_80104390");

void func_801000C4(ENEMY_WORK *pEm);
void Em32_red(ENEMY_WORK *pEm) __asm__("func_8010015C");
void func_801001F4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100490(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801005D0(ENEMY_WORK *pEm);
void func_80100888(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em32_fall_ck(ENEMY_WORK *pEm) __asm__("func_80100A50");
void func_80100CE8(ENEMY_WORK *pEm);
void func_80100D4C(void);
void func_80100D54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG func_80100F84(ENEMY_WORK *pEm);
void func_80101144(ENEMY_WORK *pEm);
void func_80101178(void);
void func_80101180(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101438(void);
void func_80101440(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101818(ENEMY_WORK *pEm);
void func_80101928(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101A7C(ENEMY_WORK *pEm);
void func_80101AAC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101C00(ENEMY_WORK *pEm);
void func_80101E30(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em32_on_back(ENEMY_WORK *pEm, SHORT L, SHORT Dir, VECTOR *pPos) __asm__("func_801025BC");
void func_8010268C(void);
void func_80102694(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102800(ENEMY_WORK *pEm);
void func_80102A00(ENEMY_WORK *pEm, LONG Spd_flg, LONG Parts);
void func_80102B14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102B50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102CE0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102FD0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010356C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em32_alone_ck(ENEMY_WORK *pEm, USHORT Rtn) __asm__("func_8010380C");
void Em32_Pldm(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq) __asm__("func_8010388C");
void func_80103930(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void func_80103D60(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void func_80103E3C(ENEMY_WORK *pPl, ULONG *pKan, ULONG *pSeq);
void func_801040B0(void);
void func_8010413C(SHORT Num);

#endif /* OVL_EM32_H */
