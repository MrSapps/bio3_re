#ifndef PLAYER_H
#define PLAYER_H
#include "em_sub.h"

/* RE2 PLAYER.C: player routines. The player work is an ENEMY_WORK (as in RE2). */
typedef void (*PL_FUNC)(ENEMY_WORK *pEm, ULONG Arg1, ULONG Arg2);

extern PL_FUNC Pl_routine[];           /* 0x8009CD40 index Routine_0 */
extern PL_FUNC Pl_mv_pad[];            /* 0x8009CD60 index Routine_1, args (Key, Key_trg) */
extern PL_FUNC Pl_mv[];                /* 0x8009CDA0 index Routine_1, args (Kan, Seq) */
extern PL_FUNC Pl_dm[];                /* 0x8009CE20 index Routine_1 */
extern PL_FUNC Pl_die_t[];             /* 0x8009CE80 index Routine_1 */

/* G.Status_flg bits */
#define STAT_PL_BUSY 0x100

/* G.System_flg bits */
#define SYS_GAME_OVER 0x20000

extern ENEMY_WORK Pl_work;             /* 0x800CCBC4 = G + 0x248C, RE2 G.Player */

void Kage_work9_color_set(KAGE_WORK9 *pKage, ULONG Rgb); /* RE2 Kage_work9_color_set */
void Pl_dead(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
ULONG Pl_life_down(LONG Down_life, ULONG Mode);
void Pl_life_up(LONG Up_life);
void Pl_move(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Pl_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Pl_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Pl_Em_damage(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);
void Pl_Em_die(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq);

#endif /* PLAYER_H */
