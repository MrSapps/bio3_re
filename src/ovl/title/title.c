#include "common.h"
#include "ovl/title.h"

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_801940E8);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_80194444);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_801945E4);

void func_80194ADC(TITLE_WORK *pTi)
{
    func_80194B08(pTi);
    func_80194C4C(&Title_wk);
}

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_80194B08);

/* menu draw: extra menu layout once the game is cleared */
void func_80194C4C(TITLE_WORK *pTi)
{
    if (G.System_flg & 0x80) {
        func_801952D8(pTi);
    } else {
        func_80194C8C(pTi);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_80194C8C);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_801952D8);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_801954D0);

/* menu: blinking cursor, then the sub routine */
void func_80195564(TITLE_WORK *pTi)
{
    pTi->Kido_cnt += 4;
    pTi->Kido = Title_sin_tbl[pTi->Kido_cnt] / 3 - 0x80;
    Title_sub_tbl[pTi->Sub_rtn](pTi);
    func_80194C4C(&Title_wk);
}

void func_801955F4(TITLE_WORK *pTi)
{
    if (G.System_flg & 0x80) {
        func_801957F0(pTi);
    } else {
        func_80195634(pTi);
    }
}

/* title menu: new game / load / option; back to the demo when nobody presses anything */
void func_80195634(TITLE_WORK *pTi)
{
    if (--pTi->Timer == 0xFFFF) {
        pTi->Sub_rtn = 10;
        return;
    }
    if (G.Unk20FC & 0x2000) {
        pTi->Cur++;
    }
    if (G.Unk20FC & 0x8000) {
        pTi->Cur--;
    }
    if (pTi->Cur >= 3) {
        pTi->Cur = 0;
    }
    if (pTi->Cur < 0) {
        pTi->Cur = 2;
    }
    if (pTi->Cur != pTi->Cur_old) {
        pTi->Timer = 900;
        pTi->Kido_cnt = 0;
        Snd_se_stad(4, NULL, 0, 0);
    }
    pTi->Cur_old = pTi->Cur;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        pTi->Kido_cnt = 0;
        switch (pTi->Cur) {
        case 0:
            pTi->Sub_rtn = 1;
            pTi->Page = 1;
            pTi->Mode = 2;
            pTi->Cur = 0;
            pTi->Cur_old = 0;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        case 1:
            pTi->Sub_rtn = 2;
            break;
        case 2:
            pTi->Sub_rtn = 5;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        }
    }
}

/* title menu after clearing the game: new game / extra / load / option */
void func_801957F0(TITLE_WORK *pTi)
{
    if (G.Unk20FC & 0x4000) {
        pTi->Cur++;
    }
    if (G.Unk20FC & 0x1000) {
        pTi->Cur--;
    }
    if (pTi->Cur >= 4) {
        pTi->Cur = 0;
    }
    if (pTi->Cur < 0) {
        pTi->Cur = 3;
    }
    if (pTi->Cur != pTi->Cur_old) {
        pTi->Timer = 900;
        pTi->Kido_cnt = 0;
        Snd_se_stad(4, NULL, 0, 0);
    }
    pTi->Cur_old = pTi->Cur;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        pTi->Kido_cnt = 0;
        switch (pTi->Cur) {
        case 0:
            pTi->Sub_rtn = 1;
            break;
        case 1:
            pTi->Sub_rtn = 7;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        case 2:
            pTi->Sub_rtn = 5;
            Snd_se_stad(6, NULL, 0, 0);
            break;
        case 3:
            Snd_se_stad(6, NULL, 0, 0);
            pTi->Sub_rtn = 9;
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_80195988);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_80195C68);

void func_801961D4(TITLE_WORK *pTi)
{
}

/* game mode select */
void func_801961DC(TITLE_WORK *pTi)
{
    if (G.Unk20FC & 0x2000) {
        pTi->Cur++;
    }
    if (G.Unk20FC & 0x8000) {
        pTi->Cur--;
    }
    if (pTi->Cur >= 2) {
        pTi->Cur = 0;
    }
    if (pTi->Cur < 0) {
        pTi->Cur = 1;
    }
    if (pTi->Cur != pTi->Cur_old) {
        pTi->Timer = 900;
        pTi->Kido_cnt = 0;
        Snd_se_stad(4, NULL, 0, 0);
    }
    pTi->Cur_old = pTi->Cur;
    if ((G.Key_trg & 0x1000) || (G.Unk20FC & 0x800)) {
        Snd_se_stad(6, NULL, 0, 0);
        pTi->Kido_cnt = 0;
        switch (pTi->Cur) {
        case 0:
            pTi->Sub_rtn = 5;
            break;
        case 1:
            pTi->Sub_rtn = 6;
            break;
        }
    }
    if ((G.Key_trg & 0x2000) && pTi->Page == 5) {
        Snd_se_stad(5, NULL, 0, 0);
        pTi->Sub_rtn = 0;
        pTi->Cur = 2;
        pTi->Cur_old = 2;
        pTi->Page++;
    }
}

/* load game */
void func_80196378(TITLE_WORK *pTi)
{
    Fade_set(0, 1, FADE_HOLD, 2, 0, 0xFFFFFF, 12);
    while (!(Fade_status(0) & FADE_END)) {
        func_80194C4C(pTi);
        Task_sleep(1);
    }
    G.Unk2115 = 1;
    Task_lexecute_no(1, 4);
    Task_sleep(1);
    Fade_set(0, 0, FADE_HOLD, 2, 0xFFFFFF, 0xFFFFFF, 1);
    Main.Unk191 = 1;
    pTi->Main_rtn = 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_8019644C);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_80196528);

void func_8019662C(TITLE_WORK *pTi)
{
}

void func_80196634(TITLE_WORK *pTi)
{
    Fade_set(0, 1, FADE_HOLD, 2, 0, 0xFFFFFF, 30);
    while (!(Fade_status(0) & FADE_END)) {
        func_80194C4C(pTi);
        Task_sleep(1);
    }
    G.System_flg &= ~0x80;
    func_801973E4();
    pTi->Main_rtn = 1;
}

INCLUDE_RODATA("asm/nonmatchings/ovl/title/title", D_80194098);

INCLUDE_RODATA("asm/nonmatchings/ovl/title/title", D_801940A4);

INCLUDE_RODATA("asm/nonmatchings/ovl/title/title", D_801940B0);

INCLUDE_RODATA("asm/nonmatchings/ovl/title/title", D_801940BC);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_801966CC);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_80196800);

INCLUDE_ASM("asm/nonmatchings/ovl/title/title", func_801971D8);

void func_801973E4(void)
{
    Cd_read(0x67, (ULONG *)0x80130000, 0, "TITLE_DAT");
}

void func_80197414(void)
{
    Cd_read(0x11, (ULONG *)0x8019C000, 0, "Capcom.tim");
}

/* title picture: the cleared-game version is at 0x80155814 */
void func_80197448(void)
{
    SVECTOR Unused;

    if (G.System_flg & 0x80) {
        func_800100A4((void *)0x8019C000, (void *)0x80155814, 0x25814);
    } else {
        func_800100A4((void *)0x8019C000, (void *)0x80130000, 0x25814);
    }
    G.Unk2484 = 0x1F05;
    Mapping_tim((ULONG *)0x8017B028);
}
