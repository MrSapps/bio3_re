#include "common.h"
#include "ovl/warning.h"
#include "cd.h"

/* task: load the warning screen picture, fade in, wait for the check, fade out */
void func_80185418(void)
{
    RECT Unused;
    LONG Cnt;

    Cd_read(0x6B, (ULONG *)0x8019C000, 0, "Warning");
    Bg_set_mode(2, 0);
    Main.Unk191 = 1;
    Fade_set(0, 1, 0, 2, 0xFFFFFF, 0, 30);
    while (Fade_status(0))
        Task_sleep(1);
    Cnt = 0xEF;
    do
        VSync(0);
    while (Cnt--);
    Main.Unk193 = 1;
    do
        Task_sleep(1);
    while (Main.Unk193 != 2);
    Fade_set(0, 1, 0, 2, 0, 0xFFFFFF, 30);
    while (Fade_status(0))
        Task_sleep(1);
    Main.Unk193 = 0;
    Bg_set_mode(1, 0);
    Task_exit();
}
