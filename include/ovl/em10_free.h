#ifndef OVL_EM10_FREE_H
#define OVL_EM10_FREE_H
/* em10-family work views and macros, shared by em10 and em16 */
#include "bio3.h"

/* em10's view of ENEMY_WORK.Unk1CC[12], also used by em16 (a near copy of em10)
   (RE2 ENEMY_WORK.Free[], OVL_FREE(p) in RE2 EM10.C: FREE_UB(o) is the byte at 0x1CC + o here) */
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

#endif /* OVL_EM10_FREE_H */
