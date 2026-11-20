#ifndef OVL_EM20_H
#define OVL_EM20_H
/*
 * Enemy module em20 (enemy id 0x20, zombie dog / Cerberus), the successor of
 * RE2's EM20.C. Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2); the
 * routine tables are in the module's .data. RE2 -> RE3 ENEMY_WORK fields:
 * Timer0 0xD8, Timer1 UnkDA, Timer2 UnkE2, Timer3 UnkE3, Neck_flg Unk120,
 * Ground Unk122, L_pl Unk154, L_spl Unk158, Hokan_flg UnkCA, Sca_info word at
 * 0xAC (EM20_SCA_INFO), At_em_no Col_unk3, Be_flg 0x4000000/0x8000000/0x10000000
 * moved to Unk132 0x4000/0x8000 and Be_flg 0x4000.
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "player.h"
#include "main.h"

/* RE2 EM20_FREE (EM_SUB.H), the enemy's Free[] at 0x1CC */
typedef struct tagEM20_FREE {
    UCHAR Area_keep;                   /* 0x00 (0x1CC) Rnd_area() target area */
    UCHAR Jump_flg;                    /* 0x01 (0x1CD) */
    SHORT Stress;                      /* 0x02 (0x1CE) */
    UCHAR Spin_flg;                    /* 0x04 (0x1D0) */
    UCHAR Fix_cnt;                     /* 0x05 (0x1D1) */
    UCHAR Hit_type;                    /* 0x06 (0x1D2) */
    UCHAR Blood;                       /* 0x07 (0x1D3) blood effects left to spawn (Set_blood) */
    UCHAR Em_next;                     /* 0x08 (0x1D4) */
    UCHAR Kage_routine;                /* 0x09 (0x1D5) */
    UCHAR Bang_ok;                     /* 0x0A (0x1D6) */
    UCHAR Combo_flg;                   /* 0x0B (0x1D7) */
    SHORT Dash_turn;                   /* 0x0C (0x1D8) */
    UCHAR Kill_ok;                     /* 0x0E (0x1DA) */
    UCHAR Target_pl;                   /* 0x0F (0x1DB) 1: target is the sub player (G.pUnk2660) */
    SHORT Yori_fx;                     /* 0x10 (0x1DC) */
    SHORT Yori_fz;                     /* 0x12 (0x1DE) */
    UCHAR No_hit;                      /* 0x14 (0x1E0) */
    UCHAR House_flg;                   /* 0x15 (0x1E1) */
    UCHAR Outside;                     /* 0x16 (0x1E2) */
    UCHAR No_atck;                     /* 0x17 (0x1E3) */
    UCHAR Stay;                        /* 0x18 (0x1E4) frames without moving */
    UCHAR No_voice;                    /* 0x19 (0x1E5) */
    UCHAR No_fire;                     /* 0x1A (0x1E6) */
} EM20_FREE;

#define EM20_W(p) ((EM20_FREE *)(p)->Unk1CC)
/* Routine_0..3 / Move_no..Hokan_flg written as one word (RE2 *(long *)&pEm->Routine_0 = 0x201) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(USHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* RE2 Sca_info: the collision node's Unk2C/Hit_flg pair read as one word */
#define EM20_SCA_INFO(p) (*(ULONG *)&(p)->Col_unk2C)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)
#define PW_FREEL(p) ((ULONG *)&(p)->Unk74)

/* RE2 EM20_BLOOD_TBL (RE3 adds a word) */
typedef struct tagEM20_BLOOD_TBL {
    UCHAR Id;                          /* 0x0 0x80: effect follows the part matrix */
    UCHAR Type;                        /* 0x1 */
    USHORT Dir;                        /* 0x2 random direction range */
    USHORT Zoom;                       /* 0x4 */
    USHORT Unk6;                       /* 0x6 */
} EM20_BLOOD_TBL;

/* RE3 only: effect records started on some parts at init (Type & 0x80, Em20_Esp_set) */
typedef struct tagEM20_ESP_TBL {
    SHORT Parts;                       /* 0x0 part index */
    USHORT Type;                       /* 0x2 Esp type (code bits 16..31) */
    SVECTOR Ofs;                       /* 0x4 offset from the part; pad = code bits 0..15 */
} EM20_ESP_TBL;

/* hit box test around a point (pl_wep.c func_800445C8) */
typedef struct tagHIT_AREA {
    VECTOR *pPos;                      /* 0x00 */
    SHORT *pBox;                       /* 0x04 x, z, w, d */
    USHORT Dir;                        /* 0x08 */
    UCHAR Id;                          /* 0x0A */
} HIT_AREA;

/* exe functions without a shared prototype (signatures as defined in src/game) */
LONG Rnd(void) __asm__("func_800102E8");                                /* RE2 Rnd (sld.s) */
LONG Root_ck(ENEMY_WORK *pEm, VECTOR *pV, LONG Area, LONG Flg);         /* root_ck.c */
LONG Rnd_area(void);                                                    /* root_ck.c */
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);                        /* em_sub.c */
LONG Joint_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);      /* joint.c */
void Spd_get(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);         /* em_sub.c */
void func_8002646C(ENEMY_WORK *pEm, LONG A, LONG B, LONG C);            /* joint.c (SHORT, SHORT, UCHAR) */
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);                              /* em_sub.c */
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);                           /* oba.c */
LONG func_800445C8(void *pA0, void *pA1, HIT_AREA *pA, ENEMY_WORK *pEm); /* pl_wep.c: pEm inside the box */
LONG SquareRoot0(LONG a);
LONG Cdir_ck_l(ENEMY_WORK *pEm, ENEMY_WORK *pT) __asm__("Cdir_ck");      /* em_sub.c, result used as int */
LONG Joint_move2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);     /* joint.c */
void Snd_se_stad(ULONG No, VECTOR *pPos, LONG Arg2, LONG Arg3);          /* snd.c */
void func_80016600(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);              /* door.c */
void func_8001B894(ULONG Id, MATRIX *pM);                               /* em_sub.c (pl_wep.h) */
void Add_speedXZ3(ENEMY_WORK *pEm, LONG muki, LONG a2) __asm__("Add_speedXZ"); /* em_sub.c, called with RE2's extra 0 */                                               /* libgte */
void *func_8001B484(ULONG a0, ULONG a1, MATRIX *pM, SVECTOR *pSv, SVECTOR *pSpd); /* RE2 Esp_call */

#endif /* OVL_EM20_H */
