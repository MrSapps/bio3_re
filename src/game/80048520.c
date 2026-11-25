#include "pl_wep.h"
#include "80048520.h"
#include "mojidisp.h"
#include "snd.h"

int Print14_col_() __asm__("Print14_col"); /* Capcom called it undeclared (implicit int): y is passed without the USHORT mask */

#define RND() ((signed char)Rnd())

/* Mercenaries "+N sec" bonus time display (G.Bonus_timer display timer, G.Bonus_time seconds; resets Em_kill_combo when it runs out); RE3-only */
void Bonus_time_disp(void)
{
    tagGLOBAL *pG = &G;

    if (pG->Bonus_timer != 0) {
        if ((USHORT)pG->Bonus_time != 0) {
            Print_color(0xFF, 0, 0x100);
            Bonus_time_shake *= -1;
            Print14_col_(0xD6, Bonus_time_shake + 0x20, 2, "[%dsec", (USHORT)pG->Bonus_time);
        }
        pG->Bonus_timer--;
        if (Bonus_time_shake > 0) {
            Bonus_time_shake--;
        }
        if (Bonus_time_old != (USHORT)pG->Bonus_time) {
            Snd_se_stad(0xF, 0, 0, 0);
            Bonus_time_shake = 4;
        }
        Bonus_time_old = pG->Bonus_time;
    } else {
        Em_kill_combo = 0;
        Bonus_time_old = 0;
        pG->Bonus_time = 0;
        Bonus_time_shake = 0;
    }
}
