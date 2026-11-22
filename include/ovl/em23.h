#ifndef OVL_EM23_H
#define OVL_EM23_H
/*
 * Enemy module em23 (enemy ids 0x23 and 0x28: Drain Deimos / Brain Sucker, the
 * flea-like enemy that walks on floors, walls and ceilings). RE3-specific, but
 * built on the RE2 enemy skeleton (EM_MAIN with Root_ck/Guide_check as RE2
 * EM10.C, Em_routine_0[8] = Init/Move/Damage/Die/Scenario/-/-/Dead, a pair of
 * br/mv tables of 22 routines selected by Routine_1, damage/die tables indexed
 * by the hit kind (Unk12C) and Routine_1, PLDAMEGE_EM/PLDIE_EM for the player
 * held by it). Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "em_sub.h"
#include "oba.h"
#include "pad.h"
#include "pl_wep.h"
#include "player.h"
#include "main.h"
#include "sca.h"

/* em23's view of ENEMY_WORK.Unk1CC[12] (RE2 ENEMY_WORK.Free[]) */
typedef struct tagEM23_FREE {
    signed char Side;                  /* 0x00 (0x1CC) wall side: bit 0 wall along x (SCA test), bit 1 far side */
    signed char Unk01;                 /* 0x01 (0x1CD) */
    signed char Mode;                  /* 0x02 (0x1CE) surface: 0 floor .. 4 ceiling */
    signed char Unk03;                 /* 0x03 (0x1CF) */
    SHORT Unk04;                       /* 0x04 (0x1D0) */
    SHORT Unk06;                       /* 0x06 (0x1D2) */
    SHORT Unk08;                       /* 0x08 (0x1D4) */
    SHORT Unk0A;                       /* 0x0A (0x1D6) */
    SHORT Floor_y;                     /* 0x0C (0x1D8) room floor height (SCA_HEAD +0xC) */
    SHORT Ground;                      /* 0x0E (0x1DA) ground height under the enemy */
    signed char Flg;                   /* 0x10 (0x1DC) 8: root search active, & 7: next state */
    UCHAR Unk11;                       /* 0x11 (0x1DD) */
    SHORT Unk12;                       /* 0x12 (0x1DE) */
    SHORT Unk14;                       /* 0x14 (0x1E0) */
    SHORT Unk16;                       /* 0x16 (0x1E2) */
    SHORT Timer;                       /* 0x18 (0x1E4) */
    SHORT Unk1A;                       /* 0x1A (0x1E6) */
    struct tagSCA_DATA *pSca;          /* 0x1C (0x1E8) wall shape to climb (copy of pSca_hit_data) */
    signed char Unk20;                 /* 0x20 (0x1EC) */
    signed char Unk21;                 /* 0x21 (0x1ED) */
    SHORT Unk22;                       /* 0x22 (0x1EE) */
    SHORT Unk24;                       /* 0x24 (0x1F0) */
    SHORT Unk26;                       /* 0x26 (0x1F2) */
    SHORT Unk28;                       /* 0x28 (0x1F4) */
    SHORT Unk2A;                       /* 0x2A (0x1F6) */
    USHORT Unk2C;                      /* 0x2C (0x1F8) */
    SHORT Unk2E;                       /* 0x2E (0x1FA) */
} EM23_FREE;

#define EF(p) ((EM23_FREE *)(p)->Unk1CC)
/* bytes 1..2 of the free area tested as one word */
#define EF_W0(p) ((p)->Unk1CC[0])
/* Size/Status_flg read as one word */
#define EM_D0W(p) (*(ULONG *)&(p)->Size)
/* Routine_0..3 / Move_no..UnkCA written as one word (RE2 *(long *)&pEm->Routine_0 = 0x101) */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN01(p) (*(SHORT *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)
/* SCA_HEAD +0xC (sca.h padC): floor height of the room */
#define SCA_FLOOR_Y(p) (((SHORT *)(p))[6])
/* x/z pair packed for the SCA line tests (sca.c func_8004F258) */
#define SCA_XZ(x, z) ((((x) >> 1) << 16) | (((z) >> 1) & 0xFFFF))
/* body direction on the wall from the base direction (Unk06/Unk08) and the wall direction Unk04;
   the _T form also clears Timer0 */
#define EM23_WALL_DIR_(p, T)                                                  \
    {                                                                         \
        UCHAR Side_ = EF(p)->Side;                                           \
        T;                                                                    \
        if (Side_ & 1) {                                                      \
            if (Side_ & 2)                                                    \
                (p)->Cdir_y = EF(p)->Unk08 + EF(p)->Unk04;                    \
            else                                                              \
                (p)->Cdir_y = EF(p)->Unk08 - EF(p)->Unk04;                    \
        } else {                                                              \
            (p)->Cdir_x = EF(p)->Unk06 + EF(p)->Unk04;                        \
        }                                                                     \
    }
#define EM23_WALL_DIR(p) EM23_WALL_DIR_(p, (void)0)
#define EM23_WALL_DIR_T(p) EM23_WALL_DIR_(p, (p)->Timer0 = 0)
/* PARTS_W free work (RE2 PARTS_W.Free[], RE3 0x74..) */
#define PW_FREE(p) ((SHORT *)&(p)->Unk74)

/* G seen from the player work (Pl_work = G + 0x248C, RE2 G.Player): one symbol, so gcc
   addresses &G.pEnemy[2] / G.ppEnemy_end relative to &Pl_work */
typedef struct {
    UCHAR pad[0x248C];
    UCHAR Player[0x1D0];               /* 0x248C Pl_work */
    ENEMY_WORK *pEnemy[42];            /* 0x265C G.pEnemy */
    ENEMY_WORK **ppEnemy_end;          /* 0x2704 */
    UCHAR pad2708[0x75E8 - 0x2708];
    USHORT Unk75E8;                    /* 0x75E8 G.Unk75E8: player fire flags */
} G_PL_VIEW;
extern G_PL_VIEW G_pl __asm__("G");

extern USHORT Em23_life[16] __asm__("D_8010C7C8");
extern USHORT Em23_life_easy[16] __asm__("D_8010C7E8");
extern void (*Em23_routine_0[])() __asm__("D_8010C808");
extern void (*Em23_mv_br[])() __asm__("D_8010C82C");
extern void (*Em23_mv[])() __asm__("D_8010C884");
extern void (*Em23_root_mv[])() __asm__("D_8010C8DC");
extern void (*Em23_tbl_C8E4[])() __asm__("D_8010C8E4");
extern void (*Em23_dm[][3])() __asm__("D_8010C8FC");
extern void (*Em23_die[][3])() __asm__("D_8010CA70");
extern void (*Em23_sce[])() __asm__("D_8010CBE4");
extern void (*Pldamage_Em23_tbl[])() __asm__("D_8010CBEC");
extern void (*Pldie_Em23_tbl[])() __asm__("D_8010CBFC");

void func_80100144(ENEMY_WORK *pEm);
void func_80100350(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801009E8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80100B24(ENEMY_WORK *pEm);
void func_80100B94(ENEMY_WORK *pEm);
void func_80100C8C(ENEMY_WORK *pEm);
void func_80100D24(ENEMY_WORK *pEm);
void func_80100DF8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80100EDC(ENEMY_WORK *pEm);
void func_80100F54(ENEMY_WORK *pEm);
void func_80101008(ENEMY_WORK *pEm);
void func_80101364(ENEMY_WORK *pEm);
void func_80101604(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80101718(ENEMY_WORK *pEm);
void func_8010175C(ENEMY_WORK *pEm);
void func_80101B94(ENEMY_WORK *pEm);
void func_80101F24(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
LONG func_80102180(ENEMY_WORK *pEm, SCA_DATA *pS);
LONG func_80102264(ENEMY_WORK *pEm, LONG Dist, LONG Up, LONG Side);
LONG func_8010268C(ENEMY_WORK *pEm, LONG Dist, LONG Down);
LONG func_801028B0(ENEMY_WORK *pEm, LONG Dist, LONG Dir, LONG Flg);
LONG func_80102C08(ENEMY_WORK *pEm, LONG Dist, LONG Dir);
LONG func_80102DB8(ENEMY_WORK *pEm, LONG Dist);
LONG func_80103070(ENEMY_WORK *pEm, LONG Dist);
LONG func_801032D0(ENEMY_WORK *pEm, LONG Dist);
void func_80103518(ENEMY_WORK *pEm);
void func_801035A8(ENEMY_WORK *pEm);
void func_801036E8(ENEMY_WORK *pEm, LONG Dir);
void func_801037E4(ENEMY_WORK *pEm);
void func_8010387C(ENEMY_WORK *pEm, LONG Add);
void func_80103920(ENEMY_WORK *pEm, LONG X, LONG Z, LONG Add);
void func_80103A3C(ENEMY_WORK *pEm);
void func_80103A90(ENEMY_WORK *pEm);
void func_80103D58(ENEMY_WORK *pEm);
void func_801040A0(ENEMY_WORK *pEm);
void func_801040D4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80104D88(ENEMY_WORK *pEm);
void func_80104E20(ENEMY_WORK *pEm);
void func_80104FA0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80105260(ENEMY_WORK *pEm);
void func_8010586C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801058D4(ENEMY_WORK *pEm);
void func_801058F8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80105B70(ENEMY_WORK *pEm);
void func_80105C14(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80105FC0(ENEMY_WORK *pEm);
void func_801060D0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801063DC(void);
void func_801063E4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80106848(void);
void func_80106850(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80106990(ENEMY_WORK *pEm);
void func_80106AA8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801071F0(void);
void func_801071F8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801072F8(ENEMY_WORK *pEm);
void func_80107454(ENEMY_WORK *pEm);
void func_801075A0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010799C(ENEMY_WORK *pEm);
void func_801079FC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80107A50(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80107B84(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80107CB8(void);
void func_80107CC0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80107D94(void);
void func_80107D9C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80108148(ENEMY_WORK *pEm);
void func_801081CC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80108320(void);
void func_80108328(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_801088FC(void);
void func_80108904(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80108BBC(void);
void func_80108BC4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80108D18(void);
void func_80108D20(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80108E04(ENEMY_WORK *pEm, SHORT Dir);
LONG func_80108EDC(ENEMY_WORK *pEm, USHORT Rtn0, USHORT Rtn1);
void func_80108F68(ENEMY_WORK *pEm);
void func_80109174(ENEMY_WORK *pEm); /* takes pEm; callers pass it implicitly (a0 still holds it) */
void func_80109234(ENEMY_WORK *pEm);
void func_80109428(ENEMY_WORK *pEm);
void func_80109498(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010962C(ENEMY_WORK *pEm);
void func_80109734(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80109AB4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80109F0C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_80109F7C(ENEMY_WORK *pEm);
void func_8010A100(ENEMY_WORK *pEm);
void func_8010A27C(ENEMY_WORK *pEm);
void func_8010A3AC(ENEMY_WORK *pEm);
void func_8010A5F4(ENEMY_WORK *pEm);
void func_8010A728(ENEMY_WORK *pEm);
void func_8010A8E8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010A954(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010AD38(ENEMY_WORK *pEm);
void func_8010AEA0(ENEMY_WORK *pEm);
void func_8010B024(ENEMY_WORK *pEm);
void func_8010B154(ENEMY_WORK *pEm);
void func_8010B494(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010B754(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010B7C0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010B868(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010BB3C(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010BE00(ENEMY_WORK *pPl, ULONG Kan, ULONG Seq);
void func_8010BEA4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010C314(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010C470(void);
void func_8010C478(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010C4B4(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void func_8010C694(ENEMY_WORK *pEm);
ENEMY_WORK **func_8010C76C(ENEMY_WORK **ppWork);

#endif /* OVL_EM23_H */
