#ifndef OVL_EM30_FREE_H
#define OVL_EM30_FREE_H
/*
 * Grave Digger work views, shared by em30 and em33 (both modules are the same
 * worm: a head work whose 17-part model is the whole body, plus seven
 * collision works, one per body segment).
 */
#include "bio3.h"
#include "enemy/em_common.h"

/* the head's private work in ENEMY_WORK.Unk1CC[] (RE2 Free[]); runs past 0x1FC.
   A segment work is a copy of the head work (Em30_work_set) and uses a few of these fields too. */
typedef struct tagEM30_FREE {
    UCHAR No;                          /* 0x00 (0x1CC) segment number 1..7 (0 = head) */
    UCHAR Morph_phase;                 /* 0x01 (0x1CD) body morph cycle 0..3 (Em30_morph_body) */
    UCHAR Head_up;                     /* 0x02 (0x1CE) em30: head reared up (Em30_rear toggles it) */
    UCHAR Morph_dir;                   /* 0x03 (0x1CF) head morph direction (Em30_morph_step) */
    SHORT Spd;                         /* 0x04 (0x1D0) forward speed, Spd_add per frame up to Spd_lim */
    SHORT Spd_lim;                     /* 0x06 (0x1D2) */
    SHORT Spd_add;                     /* 0x08 (0x1D4) */
    signed char Heavy_hit;             /* 0x0A (0x1D6) first heavy hit taken (Life / 3); the next one kills */
    signed char Under;                 /* 0x0B (0x1D7) underground: ignores hits; growl sound select */
    SHORT Hit_cnt;                     /* 0x0C (0x1D8) hits taken; em30 flinches after 16 */
    signed char Col_id;                /* 0x0E (0x1DA) collision group of the head and its segments */
    signed char Die_flg;               /* 0x0F (0x1DB) em33: set by the Die routine */
    union {
        signed char Dirt;              /* 0x10 (0x1DC) em30: also spray dirt while waiting underground */
        signed char Side;              /* 0x10 (0x1DC) em33: zone of the player / hole to come out of */
    } u10;
    signed char Near_cnt;              /* 0x11 (0x1DD) em30: dives with the player close by */
    signed char Pl_type;               /* 0x12 (0x1DE) em30: player weapon that killed it; em33: flag */
    union {
        UCHAR Blood_cnt;               /* 0x13 (0x1DF) blood spray countdown (Em30_body_number) */
        UCHAR Clip_no;                 /* 0x13 (0x1DF) em33: wall the body passes through (Em33_clip_tbl) */
    } u13;
    SHORT Morph_spd;                   /* 0x14 (0x1E0) body morph speed, scaled by Spd_x */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    signed char Dirt_part;             /* 0x18 (0x1E4) em30: part breaking the surface (dirt spray) */
    signed char Hole;                  /* 0x19 (0x1E5) em30: hole to go back to (1 or 9); em33: splash at the hole */
    SHORT Near_timer;                  /* 0x1A (0x1E6) em30: frames the player stayed close */
    ULONG Rtn_save;                    /* 0x1C (0x1E8) em33: Routine_0..3 to return to after the dive */
    SHORT Se_timer;                    /* 0x20 (0x1EC) growl sound interval */
    SHORT Splash_timer;                /* 0x22 (0x1EE) em33: fin splash interval */
    SHORT Timer;                       /* 0x24 (0x1F0) em30: ambush time; em33: hole splash time */
    SHORT Old_life;                    /* 0x26 (0x1F2) Life at the last hit */
    SHORT Fade_cnt;                    /* 0x28 (0x1F4) em33: fade-in countdown from 0x40 */
    SHORT Dive_life;                   /* 0x2A (0x1F6) em33: Life when the dive started */
    UCHAR pad2C[0x2E - 0x2C];
    USHORT Old_dist;                   /* 0x2E (0x1FA) em30: distance to the player last frame */
    UCHAR Dive_cnt;                    /* 0x30 (0x1FC) em30: dives left before the ambush ends */
    UCHAR Zone;                        /* 0x31 (0x1FD) em30: distance zone (near / mid / far) */
} EM30_FREE;

/* a segment work's private work: which part of which head it follows */
typedef struct tagEM30_SUB_FREE {
    LONG Parts_no;                     /* 0x00 (0x1CC) part of the head's model */
    LONG Oya_no;                       /* 0x04 (0x1D0) index of the head in G.pEnemy[] */
} EM30_SUB_FREE;

#define EM30_W(p) ((EM30_FREE *)(p)->Unk1CC)
#define EM30_SUB(p) ((EM30_SUB_FREE *)(p)->Unk1CC)

/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) of the body parts; part 8 (the neck) uses
   PW_NECK, part 17 PW_RGB, and 0x82 (PARTS_W.Unk82) holds the segment work of the part */
typedef struct tagEM30_PARTS_FREE {
    SHORT Roll_z;                      /* 0x74 roll accumulated down the chain (Em30_body_chain) */
    SHORT Unk76;                       /* 0x76 */
} EM30_PARTS_FREE;
#define EM30_PW(p) ((EM30_PARTS_FREE *)&(p)->Unk74)

/* morph (sabun) data at PARTS_W.pUnk98: key weights, 0x1000 = full (sabun.c func_8004AC54) */
typedef struct tagEM30_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;
    struct {
        SHORT Rate;                    /* 0x0C + 4 * key */
        USHORT pad;
    } Key[3];
} EM30_SABUN;
#define EM30_SABUN_P(pP) ((EM30_SABUN *)(pP)->pUnk98)

/* Routine_0 and Routine_1 as one halfword */
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)

extern void (*Em30_dm[][2])();         /* damage routines [Unk12C - 1][Routine_1] */

/* in both modules (src/enemy/common/em30) */
void Em30_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_body_chain(ENEMY_WORK *pEm);
void Em30_part_hang(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
void Em30_part_hang_sink(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
ENEMY_WORK *Em30_work_set(ENEMY_WORK *pEm);
void Em30_morph_step(ENEMY_WORK *pEm, LONG Step, LONG Dir);
void Em30_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dm_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dirt_hit(ENEMY_WORK *pEm);
void Em30_dirt_line(ENEMY_WORK *pEm);
void Em30_body_number(ENEMY_WORK *pEm);
void Em30_dirt_steps(ENEMY_WORK *pEm);
void Em30_dirt_set(ENEMY_WORK *pEm, LONG No, LONG Kind);
void Em30_se_set(ULONG No, ENEMY_WORK *pEm, LONG *pPos);

#endif /* OVL_EM30_FREE_H */
