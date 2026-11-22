#include "pl_wep.h"
#include "sca.h"
#include "monitor.h"
#include "pl_wep.h"
#include "pad.h"
#include "esp.h"
#include "80048520.h"
#include "mojidisp.h"
#include "sld.h"
#include "snd.h"
#include "status.h"

int Print14_col_() __asm__("Print14_col"); /* returns int here (canonical void); unprototyped call */

#define RND() ((signed char)func_800102E8())

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
        D_8009DE24 = 0;
        D_8009DE26 = 0;
        pG->Unk2118 = 0;
        D_8009DE28 = 0;
    }
}
