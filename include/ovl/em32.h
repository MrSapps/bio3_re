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

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
LONG Guide_check(LONG X, LONG Z, LONG Floor);                           /* root_ck.c */
LONG Water_ck(LONG Pos_x, LONG Pos_z);                                  /* sce_at.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, frame-end flag */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);         /* em_sub.c */
void func_8002646C(ENEMY_WORK *pEm, LONG A, LONG B, LONG C);            /* joint.c (SHORT, SHORT, UCHAR) */
LONG func_8004D720(VECTOR *pPos, ULONG a1, ULONG a2, ULONG a3);         /* RE2 Sca_get_ground (sca.c) */
void Rot_vector_l(LONG Dir, SVECTOR *p, SVECTOR *q) __asm__("Rot_vector"); /* sub.c Rot_vector, unnarrowed dir */
void func_80034124(COL_OBJ *pObj);                                      /* oba.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */
ULONG Gacha_ch(void);                                                   /* em_sub.c */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");     /* em_sub.c Cdir_ck, int result */
LONG func_800445C8(LONG *pA0, LONG *pA1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c */
void func_8004AC54(ENEMY_WORK *pEm, PARTS_W *pP);                       /* sabun.c: apply the morph keys */
LONG SquareRoot0(LONG a);                                               /* libgte */
void Player_move(ENEMY_WORK *pEm);                                      /* player.c */
LONG Joint_move2_l(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg) __asm__("Joint_move2"); /* joint.c, RE2 Joint_move */
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);         /* snd.c */
ULONG Pl_life_down(LONG Down_life, ULONG Mode);                         /* player.c */
LONG func_800345C0(VECTOR *pPos, ENEMY_WORK *pEm);                      /* oba.c */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
UCHAR func_80033580(void *pObj, ULONG Id);                              /* oba.c */

#endif /* OVL_EM32_H */
