#ifndef PLAYER_H
#define PLAYER_H
#include "em_sub.h"
#include "oba.h"

typedef struct {
    UCHAR pad0[4];
    SHORT Unk4;                        /* 0x04 */
    UCHAR Num;                         /* 0x06 */
    UCHAR pad7;
    UCHAR Tbl[16][2];                  /* 0x08 */
} PAD_WORK_800DE628;

/* RE2 PLAYER.C: player routines. The player work is an ENEMY_WORK (as in RE2). */
typedef void (*PL_FUNC)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
typedef void (*PL_PAD_FUNC)(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);

extern PL_FUNC Pl_routine[];           /* 0x8009CD40 index Routine_0 */
extern PL_PAD_FUNC Pl_mv_pad[];        /* 0x8009CD60 index Routine_1 */
extern PL_FUNC Pl_mv[];                /* 0x8009CDA0 index Routine_1 */
extern PL_FUNC Pl_dm[];                /* 0x8009CE20 index Routine_1 */
extern PL_FUNC Pl_die_t[];             /* 0x8009CE80 index Routine_1 */

/* G.Status_flg bits */
#define STAT_PL_BUSY 0x100

/* G.System_flg bits */
#define SYS_GAME_OVER 0x20000

/* 0x800CCBC4 = &G.Player as its own exe symbol: the player as the ENEMY_WORK the code passes
   (no cast at the ~200 &Pl_work uses; only the PLAYER_WORK head, 0x1CC bytes, exists). Code that
   addresses it apart from G gets its own base register, so both forms stay. */
extern ENEMY_WORK Pl_work;

void Pl_dead(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
ULONG Pl_life_down(LONG Down_life, ULONG Mode);
void Pl_life_up(LONG Up_life);
void Pl_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pl_damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pl_die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pl_Em_damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Pl_Em_die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

extern UCHAR D_8009CD3C;
extern UCHAR D_8009CDE0[];
extern UCHAR D_8009CDEC[];
extern UCHAR D_8009CDF0[];
extern UCHAR D_8009CDF4[];
extern UCHAR D_8009CDF8[];
extern USHORT D_8009CDFC[];
extern struct tagHIT_ATTR D_8009CE0C;
extern UCHAR D_8009CF78[];
extern UCHAR D_800CCBBF;
extern COL_OBJ D_800CCC44;
extern ULONG D_800D1F2C; /* flags, < 0: hard-mode tables */
extern PAD_WORK_800DE628 D_800DE628;

void Player_set(ENEMY_WORK *pEm);
void Player_move(ENEMY_WORK *pEm);
void Pl_init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80038F98(ENEMY_WORK *pEm);
void pl_mv00_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void pl_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv01_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void pl_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv02_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void pl_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv03_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void pl_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
ULONG Lim_dir_ck(ENEMY_WORK *pEm, ULONG Lim, SHORT Dir);
void pl_mv04_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void pl_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv06_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003A688(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Key);
void pl_mv07_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003A7D8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv08_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003B080(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv0a_pad(ENEMY_WORK *pEm, ULONG Flg);
void pl_mv0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8003B4FC(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003B784(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8003B9FC(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003BCA4(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8003BF28(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003C104(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Flg);
LONG func_8003C5B0(ENEMY_WORK *pEm, ULONG Flg);
void pl_mv0b_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003C740(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv0c_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003CA88(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_mv0d_pad(ENEMY_WORK *pEm, ULONG Key, ULONG Key_trg);
void func_8003CEA0(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_dm00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_dm01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_dm02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_dm03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void pl_die00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_8003DC0C(void);

#endif /* PLAYER_H */
