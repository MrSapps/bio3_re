#ifndef OVL_EM21_H
#define OVL_EM21_H
/*
 * Enemy module em21 (crow, enemy id 0x21), the successor of RE2's EM21.C:
 * same routine tables (Em21_routine_0, indoor/outdoor br/mv tables of 15,
 * Em21_dm, Em21_sce_mv, Em21_dead_mv, Pldamage_Em21_tbl).
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "mojidisp.h"
#include "pad.h"
#include "player.h"

/* em21's view of ENEMY_WORK.Unk1CC[] (RE2 struct tagEM21F at ENEMY_FREE(pEm), EF->) */
typedef struct tagEM21_FREE {
    UCHAR Tmp;                         /* 0x00 (0x1CC) */
    UCHAR Cnt;                         /* 0x01 (0x1CD) */
    UCHAR Cnt2;                        /* 0x02 (0x1CE) */
    UCHAR Cnt3;                        /* 0x03 (0x1CF) */
    UCHAR Na_cnt;                      /* 0x04 (0x1D0) */
    UCHAR Sca_cnt;                     /* 0x05 (0x1D1) */
    UCHAR Obj_cnt;                     /* 0x06 (0x1D2) */
    UCHAR Idly_flg;                    /* 0x07 (0x1D3) */
    UCHAR Idly_cnt;                    /* 0x08 (0x1D4) */
    signed char Pl_dead_cnt;           /* 0x09 (0x1D5) */
    UCHAR Atck;                        /* 0x0A (0x1D6) RE2 Dummy1: non-zero while the peck hit area is live */
    UCHAR Unk0B;                       /* 0x0B (0x1D7) */
    SHORT Tmp_0;                       /* 0x0C (0x1D8) */
    SHORT Tmp_1;                       /* 0x0E (0x1DA) */
    SHORT Tmp_2;                       /* 0x10 (0x1DC) */
    USHORT Flg;                        /* 0x12 (0x1DE) */
    SHORT Old_x;                       /* 0x14 (0x1E0) RE2 Padd1: position of the last frame */
    SHORT Old_y;                       /* 0x16 (0x1E2) */
    SHORT Old_z;                       /* 0x18 (0x1E4) */
} EM21_FREE;

#define EF(p) ((EM21_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 1) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

/* keep a coordinate inside +-l (scenario flight paths) */
#define EM21_LIMIT(v, l) ((v) < -(l) ? -(l) : ((v) > (l) ? (l) : (v)))

/* hit area handed to func_800445C8 (pl_wep.c) */
typedef struct tagEM21_HIT_AREA {
    VECTOR *pPos;                      /* 0x00 */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} EM21_HIT_AREA;

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
LONG Rnd_area(void);                                                    /* root_ck.c */
ULONG Gacha_ch(void);                                                   /* em_sub.c */
void Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG a2, LONG a3);            /* root_ck.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c, frame-end flag */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);         /* em_sub.c */
LONG func_8004D720(LONG *pPos, ULONG a1, ULONG a2, ULONG a3);           /* RE2 Sca_get_ground (sca.c) */
LONG func_800445C8(LONG *pA0, LONG *pA1, EM21_HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c */
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");     /* em_sub.c Cdir_ck, int result as in RE2 EM21.H */
void Add_speedXZ_n(ENEMY_WORK *pEm, LONG muki, LONG Dummy) __asm__("Add_speedXZ"); /* RE2 EM21 passes a 3rd arg */
LONG func_800502B8(SVECTOR *pSv, struct tagSCA_DATA *pT);              /* RE2 Sca_get_dir (sca.c): 2 floor/corner, else wall side */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
void Kage_work_color_set(ULONG *pK, ULONG Color);                       /* em_sub.c */
UCHAR func_80033580(void *pObj, ULONG Id);                              /* oba.c */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */

#endif /* OVL_EM21_H */
