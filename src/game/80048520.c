#include "pl_wep.h"
#include "80048520.h"
#include "mojidisp.h"
#include "snd.h"

int Print14_col_() __asm__("Print14_col"); /* Capcom called it undeclared (implicit int): y is passed without the USHORT mask */

#define RND() ((signed char)Rnd())

void func_80048520(void)
{
    tagGLOBAL *pG = &G;

    if (pG->Unk2116 != 0) {
        if ((USHORT)pG->Unk2118 != 0) {
            Print_color(0xFF, 0, 0x100);
            D_8009DE28 *= -1;
            Print14_col_(0xD6, D_8009DE28 + 0x20, 2, "[%dsec", (USHORT)pG->Unk2118);
        }
        pG->Unk2116--;
        if (D_8009DE28 > 0) {
            D_8009DE28--;
        }
        if (D_8009DE26 != (USHORT)pG->Unk2118) {
            Snd_se_stad(0xF, 0, 0, 0);
            D_8009DE28 = 4;
        }
        D_8009DE26 = pG->Unk2118;
    } else {
        Em_kill_combo = 0;
        D_8009DE26 = 0;
        pG->Unk2118 = 0;
        D_8009DE28 = 0;
    }
}
