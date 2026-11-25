#ifndef OVL_EM38_H
#define OVL_EM38_H
/*
 * Enemy module em38 (enemy id 0x38): the large many-part enemy that the
 * tentacles of em39 (enemy id 0x39) are attached to. Loaded at 0x80100000
 * (slot 1) or 0x8010D000 (slot 2).
 * The routine frame is RE2's G-type enemy (EM34.C / EM37.C): Em_routine_0 =
 * Init/Move/Damage/Die/Scenario/-/-/Dead, Move picks Em38_Move_type[Type & 3],
 * which runs a brain routine (mv_br) and a motion routine (mv) of 16, both
 * indexed by Routine_1. Damage is indexed by the hit kind (Unk12C), the
 * scenario routines by Routine_1; PLDAMEGE_EM/PLDIE_EM tables for the player.
 */
#include "bio3.h"
#include "enemy/em_common.h"
#include "em_sub.h"
#include "oba.h"
#include "main.h"
#include "pad.h"
#include "player.h"
#include "pl_wep.h"
#include "sub.h"

/* em38's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM38_FREE {
    struct tagENEMY_WORK *pUnk00;      /* 0x00 (0x1CC) */
    struct tagENEMY_WORK *pUnk04;      /* 0x04 (0x1D0) */
    USHORT Flg;                        /* 0x08 (0x1D4) */
    UCHAR Unk0A;                       /* 0x0A (0x1D6) timer */
    UCHAR Unk0B;                       /* 0x0B (0x1D7) timer */
    SHORT Unk0C;                       /* 0x0C (0x1D8) */
    SHORT Unk0E;                       /* 0x0E (0x1DA) */
    UCHAR Unk10;                       /* 0x10 (0x1DC) */
    signed char Unk11;                 /* 0x11 (0x1DD) */
    signed char Unk12;                 /* 0x12 (0x1DE) */
    signed char Unk13;                 /* 0x13 (0x1DF) */
    SHORT Unk14;                       /* 0x14 (0x1E0) last Cdir_y */
    UCHAR Unk16;                       /* 0x16 (0x1E2) */
    UCHAR Unk17;                       /* 0x17 (0x1E3) */
    UCHAR Unk18;                       /* 0x18 (0x1E4) breath state (func_8010BAD0) */
    signed char Unk19;                 /* 0x19 (0x1E5) puff counter (func_8010BAD0) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) breath direction */
    UCHAR Unk1C;                       /* 0x1C (0x1E8) timer */
    UCHAR Unk1D;                       /* 0x1D (0x1E9) timer */
    SHORT Unk1E;                       /* 0x1E (0x1EA) timer */
    SHORT Unk20;                       /* 0x20 (0x1EC) frames the player has been out of sight */
} EM38_FREE;

/* morph (sabun) data at PARTS_W.pUnk98 (sabun.c) */
typedef struct tagEM38_SABUN {
    ULONG Ofs;                         /* 0x00 */
    ULONG Size;                        /* 0x04 */
    USHORT Num;                        /* 0x08 */
    USHORT pad0A;                      /* 0x0A */
    struct {
        SHORT Rate;                    /* weight of morph target n (0x1000 = full) */
        SHORT pad;
    } Key[7];                          /* 0x0C */
} EM38_SABUN;
#define EM38_SA(p) ((EM38_SABUN *)(p)->pSa_dat_head)

#define EM38_W(p) ((EM38_FREE *)(p)->Unk1CC)
/* Spd_x/Spd_y cleared as one word */
#define EM_SPD_XY(p) (*(ULONG *)&(p)->Spd_x)
/* PARTS_W.Unk74.. used as two words (RE2 PARTS_W.Free[]): fade count and colour */
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)
/* the same free work as halfwords */
#define PW_FREES(p) ((SHORT *)&(p)->Unk74)

/* parts swelling with the body in the transformation (module .data Em38_swell) */
typedef struct tagEM38_SWELL {
    UCHAR Num0;                        /* 0x00 parts given the size with func_8010C9A4 */
    UCHAR Num1;                        /* 0x01 parts given the size directly */
    UCHAR Parts[14];                   /* 0x02 part numbers */
} EM38_SWELL;

/* walk targets (module .data Em38_root, 0x20 bytes each) */
typedef struct tagEM38_ROOT {
    LONG Pos_x;                        /* 0x00 */
    LONG Pos_y;                        /* 0x04 */
    LONG Pos_z;                        /* 0x08 also the flag word of the zone below */
    SHORT Area[4][2];                  /* 0x0C x, z corners (a VCUT starting at Pos_z) */
    SHORT Dir;                         /* 0x1C direction to walk in the zone */
    SHORT pad1E;                       /* 0x1E */
} EM38_ROOT;
#define EM38_ROOT_CUT(r) ((VCUT *)&(r)->Pos_z)

/* exe functions without a shared prototype (signatures as defined in src/game) */

extern USHORT Em38_cnt; /* counter bumped by the head sway */
extern EM38_ROOT Em38_root[];
extern SHORT Em38_box0[4]; /* hit boxes of the attacks */
extern SHORT Em38_box1[4];
extern SHORT Em38_box2[4];
extern SHORT Em38_box3[4];
extern SHORT Em38_box4[4];
extern SHORT Em38_box5[4];
extern SHORT Em38_box6[4];
extern SHORT Em38_box7[4];
extern SHORT Em38_box8[4];
extern SHORT Em38_box9[4];
extern VECTOR Em38_goal_pos;
extern VECTOR Em38_goal_pos2;
extern VCUT Em38_cut; /* zone that sets flag 0x1B */
extern void (*Em38_routine_0[])();
extern void (*Em38_Move_type[])();
extern void (*Em38_mv_br_type00[])();
extern void (*Em38_mv_type00[])();
extern void (*Em38_dm[])();
extern void (*Em38_sce[])();
extern EM38_SWELL Em38_swell;
extern signed char Em38_pulse[64]; /* size pulse of the dying body */
extern void (*Pldamage_Em38_tbl[])();
extern void (*Pldie_Em38_tbl[])();
extern KAGE_WORK *Em38_kage[]; /* shadow works of the parts */
extern COL_OBJ Em38_col[5]; /* extra collision objects */
extern MATRIX Em38_spit_m; /* effect matrix of sce04 */
extern UCHAR Em38_pulse_no; /* index into Em38_pulse (module BSS) */

void func_80100154(ENEMY_WORK *pEm);
void func_80100714(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801011D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101214(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101298(ENEMY_WORK *pEm);
void func_80101364(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801013C0(ENEMY_WORK *pEm);
void func_80101714(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801018A8(ENEMY_WORK *pEm);
void func_8010195C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101AC4(void);
void func_80101ACC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101C4C(void);
void func_80101C54(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102068(void);
void func_80102070(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102940(void);
void func_80102948(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102E94(void);
void func_80102E9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103468(void);
void func_80103470(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801037E8(ENEMY_WORK *pEm);
void func_801038E8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103FF0(void);
void func_80103FF8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104718(ENEMY_WORK *pEm);
void func_80104828(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104D84(ENEMY_WORK *pEm);
void func_80104F34(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801050C4(void);
void func_801050CC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105424(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105600(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105784(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801059EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105D8C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105F44(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801060EC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80106128(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801064C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801075D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801081C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80108FD8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109394(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010955C(void);
void func_80109564(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801095A0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109948(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109BD8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109DF0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109F14(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80109F50(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A1C8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010A480(ENEMY_WORK *pEm);
void func_8010A574(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Lim);
void func_8010A7D4(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1);
void func_8010A924(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1);
void func_8010AA3C(ENEMY_WORK *pEm);
void func_8010B02C(ENEMY_WORK *pEm);
void func_8010B1E8(ENEMY_WORK *pEm);
void func_8010B550(ENEMY_WORK *pEm);
void func_8010B6BC(void);
void func_8010B6C4(ENEMY_WORK *pEm);
void func_8010B848(ENEMY_WORK *pEm);
void func_8010B94C(ENEMY_WORK *pEm);
void func_8010BAD0(ENEMY_WORK *pEm);
void func_8010C03C(ENEMY_WORK *pEm);
void func_8010C3E8(ENEMY_WORK *pEm);
void func_8010C9A4(ENEMY_WORK *pEm, PARTS_W *pP, LONG Rate);

#endif /* OVL_EM38_H */
