#ifndef ENEMY_EM_COMMON_H
#define ENEMY_EM_COMMON_H
/* Helpers shared by the enemy overlays (src/enemy). */
#include "bio3.h"

/* Capcom's code writes Routine_0..3 and Move_no / Move_cnt / Hokan_flg as one word
 * (RE2 EM20.C: *(long *)&pEm->Move_no = 0xf0006;). The compiler emits a single sw for it, so the
 * word store has to stay; the SET forms only spell out the bytes and fold to the same constant. */
#define EM_RTN(p) (*(ULONG *)&(p)->Routine_0)
#define EM_RTN23(p) (*(USHORT *)&(p)->Routine_2)
#define EM_MOVE(p) (*(ULONG *)&(p)->Move_no)

/* Routine_0 = r0, Routine_1 = r1, Routine_2 = r2, Routine_3 = r3 */
#define EM_SET_RTN(p, r0, r1, r2, r3) (EM_RTN(p) = (r0) | (r1) << 8 | (r2) << 16 | (r3) << 24)
/* Move_no = no, Move_cnt = cnt, Hokan_flg = hokan (joint interpolation rate) */
#define EM_SET_MOVE(p, no, cnt, hokan) (EM_MOVE(p) = (no) | (cnt) << 8 | (hokan) << 16)
/* the same with a motion number computed at run time: Capcom added it to the constant (addu, not or) */
#define EM_SET_MOVE_V(p, no, cnt, hokan) (EM_MOVE(p) = (no) + ((cnt) << 8 | (hokan) << 16))

#endif /* ENEMY_EM_COMMON_H */
