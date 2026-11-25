#ifndef OVL_EM30_H
#define OVL_EM30_H
/*
 * Enemy module em30 (enemy id 0x30): the Grave Digger, the giant worm that
 * burrows through the ground. The head work (Type & 1 == 0) drives a 17-part
 * model that is the whole body (Em30_body_chain hangs parts 1..6 from part 0)
 * and spawns seven segment works (Type & 1) that only carry the collision
 * nodes and shadows of parts 8 and 1..6. Work views: enemy/em30_free.h.
 * Routine_0: Init/Move/Damage/Die. Move runs Em30_mv_br[Routine_1] then
 * Em30_mv[Routine_1]: 1 crawl, 2 rear up, 3 dive and resurface, 4 bite,
 * 5 swallow the player, 6 lunge, 7 ambush from underground, 8 go back to a
 * hole. Damage and Die are indexed by the hit kind (Unk12C); the player's
 * death routine is hooked into G.pPldie_Em[Id - 0x10].
 * Loaded at 0x80100000 (slot 1) or 0x8010D000 (slot 2).
 */
#include "bio3.h"
#include "enemy/em30_free.h"
#include "em_sub.h"
#include "oba.h"
#include "player.h"
#include "sca.h"
#include "pad.h"
#include "sub.h"
#include "main.h"
#include "pl_wep.h"

extern USHORT Em30_vib_tbl[]; /* pad vibration scripts */
extern void (*Em30_routine_0[])();     /* Init, Move, Damage, Die */
extern USHORT Em30_col_w[];          /* segment collision W0/D0 by part */
extern void (*Em30_mv_br[])();         /* brain routines by Routine_1 */
extern void (*Em30_mv[])();            /* motion routines by Routine_1 */
extern void (*Em30_die[])();           /* by Damage_no - 1 */

void Em30_main(ENEMY_WORK *pEm);
void Em30_Init(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_sub_init(ENEMY_WORK *pSub, LONG No, ENEMY_WORK *pEm, LONG Parts_no);
void Em30_Move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_Move_type(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_appear_br(ENEMY_WORK *pEm);
void Em30_nop(ENEMY_WORK *pEm);
void Em30_crawl_br(ENEMY_WORK *pEm);
void Em30_crawl(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_rear_br(ENEMY_WORK *pEm);
void Em30_rear(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dive_br(ENEMY_WORK *pEm);
void Em30_dive(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dirt_spray(ENEMY_WORK *pEm, LONG No, LONG Big);
void Em30_dirt_puff(ENEMY_WORK *pEm, LONG No, LONG Big);
void Em30_blood_part(ENEMY_WORK *pEm, LONG No);
void Em30_parts_mask(ENEMY_WORK *pEm);
void Em30_surface(ENEMY_WORK *pEm);
void Em30_bite_br(ENEMY_WORK *pEm);
void Em30_bite(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_swallow_br(ENEMY_WORK *pEm);
void Em30_swallow(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_lunge_br(ENEMY_WORK *pEm);
void Em30_lunge(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_ambush_br(ENEMY_WORK *pEm);
void Em30_ambush(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_hole_br(ENEMY_WORK *pEm);
void Em30_hole(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_body_chain(ENEMY_WORK *pEm);
void Em30_part_hang(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
void Em30_part_hang_sink(ENEMY_WORK *pEm, PARTS_W *pP0, PARTS_W *pP1, SHORT Rate);
LONG Em30_morph_body(ENEMY_WORK *pEm);
void Em30_morph_step(ENEMY_WORK *pEm, LONG Step, LONG Dir);
void Em30_hole_set(ENEMY_WORK *pEm);
void Em30_Damage(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dm_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dm_flinch(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dm_heavy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_dm_fire(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_blood_set(ENEMY_WORK *pEm);
void Em30_dirt_hit(ENEMY_WORK *pEm);
void Em30_dirt_line(ENEMY_WORK *pEm);
void Em30_body_number(ENEMY_WORK *pEm);
void Em30_dirt_steps(ENEMY_WORK *pEm);
void Em30_dirt_set(ENEMY_WORK *pEm, LONG No, LONG Kind);
void Em30_Die(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_die_normal(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_die_heavy(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em30_se_set(ULONG No, ENEMY_WORK *pEm, LONG *pPos);
void Em30_Pldie(ENEMY_WORK *pPl);

#endif /* OVL_EM30_H */
