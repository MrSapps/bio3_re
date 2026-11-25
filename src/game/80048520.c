#include "pl_wep.h"
#include "80048520.h"
#include "mojidisp.h"
#include "snd.h"

int Print14_col_() __asm__("Print14_col"); /* Capcom called it undeclared (implicit int): y is passed without the USHORT mask */

#define RND() ((signed char)Rnd())

/* Mercenaries "+N sec" bonus time display (G.Unk2116 display timer, G.Unk2118 seconds; resets Em_kill_combo when it runs out); RE3-only */
void Bonus_time_disp(void)
{
    tagGLOBAL *pG = &G;

    if (pG->Unk2116 != 0) {
        if ((USHORT)pG->Unk2118 != 0) {
            Print_color(0xFF, 0, 0x100);
            Bonus_time_shake *= -1;
            Print14_col_(0xD6, Bonus_time_shake + 0x20, 2, "[%dsec", (USHORT)pG->Unk2118);
        }
        pG->Unk2116--;
        if (Bonus_time_shake > 0) {
            Bonus_time_shake--;
        }
        if (Bonus_time_old != (USHORT)pG->Unk2118) {
            Snd_se_stad(0xF, 0, 0, 0);
            Bonus_time_shake = 4;
        }
        Bonus_time_old = pG->Unk2118;
    } else {
        Em_kill_combo = 0;
        Bonus_time_old = 0;
        pG->Unk2118 = 0;
        Bonus_time_shake = 0;
    }
}
