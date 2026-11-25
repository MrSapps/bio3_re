#ifndef OVL_EM10_FREE_H
#define OVL_EM10_FREE_H
/* em10-family work views and macros, shared by em10 and em16 */
#include "bio3.h"
#include "enemy/em_common.h"

/* em10's view of the enemy free area ENEMY_WORK.Unk1CC, also used by em16 (a near copy of em10).
 * RE2 struct tagEM10_FREE (MAIN.H); RE2 EM10.C reaches it as FREE_UB(o) etc. The names are RE2's;
 * from 0x23 on, RE2's retail code already differs from the beta MAIN.H layout, those names are by use. */
typedef struct tagEM10_FREE {
    UCHAR Walk_no;                     /* 0x00 (0x1CC) walk motion, Rnd() % 3 */
    UCHAR Kahan_r0;                    /* 0x01 (0x1CD) Em10_Kahannsin_walk routine (lower body walking alone) */
    USHORT Etc_flg;                    /* 0x02 (0x1CE) 1: Em10_Kahannsin_walk runs, 2/0x10: Em10_Damage/Em10_Die take
                                        * the knock-down / dm07-die05 paths, 0x180: Em10_Kubigoro runs, 0x400: Root_ck
                                        * mode, 0x4000: set at random (1 in 8) by Em10_Init */
    UCHAR K_move_no;                   /* 0x04 (0x1D0) lower-body joint state (Em10_Kahannsin_walk JOINT_P) */
    UCHAR K_move_cnt;                  /* 0x05 (0x1D1) */
    UCHAR K_hokan_flg;                 /* 0x06 (0x1D2) */
    UCHAR K_timer0;                    /* 0x07 (0x1D3) */
    SHORT K_dir_y;                     /* 0x08 (0x1D4) lower-body direction */
    UCHAR Dm_r0;                       /* 0x0A (0x1D6) 1: the next hit may knock it down */
    signed char Dm_cnt;                /* 0x0B (0x1D7) damage left before a stagger, (Rnd() & 0xF) + 0x10 */
    KAGE_WORK *pK_kage;                /* 0x0C (0x1D8) shadow of the lower body (Kage_work_set) */
    ULONG Old_r;                       /* 0x10 (0x1DC) Routine_0..3 of the last frame */
    ULONG Next_r;                      /* 0x14 (0x1E0) Routine_0..3 to return to after a scripted move */
    UCHAR Hanare_cnt;                  /* 0x18 (0x1E4) timer; clears G.Unk75EA bit 0 when it runs out */
    UCHAR Die_r0;                      /* 0x19 (0x1E5) */
    SHORT Ground;                      /* 0x1A (0x1E6) floor height while lying */
    UCHAR Mot_dm;                      /* 0x1C (0x1E8) index of the motion triple in the die tables */
    UCHAR Rnd_area;                    /* 0x1D (0x1E9) Root_ck area */
    USHORT A_timer;                    /* 0x1E (0x1EA) frame counter */
    UCHAR W_timer;                     /* 0x20 (0x1EC) Em10_Dead: corpse bobbing in water (0x80: afloat) */
    UCHAR Dam_s;                       /* 0x21 (0x1ED) moan timer, 150 after a moan (Snd_se_enem) */
    UCHAR Fire_c;                      /* 0x22 (0x1EE) burn count */
    UCHAR Ude_timer;                   /* 0x23 (0x1EF) Em10_ude_down blood timer */
    UCHAR Ude_down;                    /* 0x24 (0x1F0) non-zero: an arm is off, Em10_ude_down runs */
    UCHAR Ude_flg;                     /* 0x25 (0x1F1) which arm (bit 0 side, bit 1 upper/lower) */
    UCHAR Catch_timer;                 /* 0x26 (0x1F2) frames before the next grab try */
    UCHAR Walk_no2;                    /* 0x27 (0x1F3) Em10_mv02 walk motion, Rnd() % 3 + 3 */
    SHORT U_pos_x;                     /* 0x28 (0x1F4) severed arm position (Em10_ude_down) */
    SHORT U_pos_y;                     /* 0x2A (0x1F6) */
    union {
        struct {
            SHORT U_pos_z;             /* 0x2C (0x1F8) */
            UCHAR Unk2E;               /* 0x2E (0x1FA) (Rnd() & 1) + 6, not read */
            UCHAR Mv10_timer;          /* 0x2F (0x1FB) set by Em10_mv10, no approach while it runs */
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
/* RE2 ENEMY_WORK.Super_vector (0x148..0x14F, rotation of the body after a knock-down) */
#define SUPER_VECTOR(p) ((SVECTOR *)&(p)->Add_dir)

#endif /* OVL_EM10_FREE_H */
