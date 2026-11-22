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
#include "pl_wep.h"

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

extern SHORT Em21_peck_box0[4] __asm__("D_80105110");
extern SHORT Em21_peck_box1[4] __asm__("D_80105118");
extern void (*Em21_routine_0[])() __asm__("D_80105120");
extern void (*Em21_indoor_br[])() __asm__("D_80105140");
extern void (*Em21_indoor_mv[])() __asm__("D_8010517C");
extern void (*Em21_outdoor_br[])() __asm__("D_801051B8");
extern void (*Em21_outdoor_mv[])() __asm__("D_801051F4");
extern void (*Em21_dm[])() __asm__("D_80105230");
extern void (*Em21_sce_mv[])() __asm__("D_801052AC");
extern void (*Em21_dead_mv[])() __asm__("D_801052C4");
extern void (*Pldamage_Em21_tbl[])() __asm__("D_801052D0");
extern HIT_AREA Em21_hit_area __asm__("D_801052E4");

void func_8010014C(ENEMY_WORK *pEm);
void func_801002F8(ENEMY_WORK *pEm);
void func_801003D4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801005DC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100834(ENEMY_WORK *pEm);
void func_8010091C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100D1C(ENEMY_WORK *pEm);
void func_80100D24(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80100DB8(ENEMY_WORK *pEm);
void func_80100DC0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101044(ENEMY_WORK *pEm);
void func_8010104C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101238(ENEMY_WORK *pEm);
void func_80101400(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101538(ENEMY_WORK *pEm);
void func_80101620(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101708(ENEMY_WORK *pEm);
void func_801017BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801018A4(ENEMY_WORK *pEm);
void func_801018AC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101BE0(ENEMY_WORK *pEm);
void func_80101C0C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101D94(ENEMY_WORK *pEm);
void func_80101D9C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80101E54(ENEMY_WORK *pEm);
void func_80101E5C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102010(ENEMY_WORK *pEm);
void func_8010207C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102278(ENEMY_WORK *pEm);
void func_80102298(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801024D8(ENEMY_WORK *pEm);
void func_80102550(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801027A4(ENEMY_WORK *pEm);
void func_801027C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102C48(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80102CC4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010304C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103258(PARTS_W *pP, LONG B_y);
void func_801032A4(PARTS_W *pP, LONG D_y);
void func_80103320(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801035BC(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801035C4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010363C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103724(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8010386C(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103B80(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80103E98(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104014(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104194(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801041D0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104458(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104490(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_801045BC(ENEMY_WORK *pEm);
void func_80104638(ENEMY_WORK *pEm, ULONG Rno0, ULONG Rno1);
void func_80104648(ENEMY_WORK *pEm, ULONG Rno1);
void func_80104654(ENEMY_WORK *pEm);
void func_801047A8(ENEMY_WORK *pEm);
void func_80104A08(ENEMY_WORK *pEm);
void func_80104A90(ENEMY_WORK *pEm);
void func_80104AC4(ENEMY_WORK *pEm);
void func_80104B90(ULONG Mask);
void func_80104C00(ENEMY_WORK *pEm);
void func_80104C94(ENEMY_WORK *pEm, LONG Num);
void func_80104D50(ENEMY_WORK *pEm, LONG Size);
void func_80104D5C(void);
void func_80104E60(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80104F48(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80105100(ENEMY_WORK *pEm);
void func_80105108(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

#endif /* OVL_EM21_H */
