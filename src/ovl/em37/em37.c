#include "common.h"
#include "ovl/em37.h"

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_8010003C);

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100370);

/* RE2 Em37_move */
void func_801005A0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Em37_move_type[(UCHAR)pEm->Type](pEm, Kan, Seq);
}

void func_801005DC(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Em37_mv[pEm->Routine_1](pEm, Kan, Seq);
}

void func_80100618(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Em37_sce[pEm->Routine_1](pEm, Kan, Seq);
}

void func_80100654(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_8010065C);

void func_80100894(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_8010089C);

void func_8010094C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100954);

void func_801009E4(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_801009EC);

void func_80100AB4(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100ABC);

void func_80100B8C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100B94);

void func_80100CBC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100CC4);

void func_80100D5C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100D64);

void func_80100DF0(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100DF8);

void func_80100EB0(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100EB8);

void func_80100F58(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100F60);

void func_80100FD8(void) {
}

INCLUDE_RODATA("asm/nonmatchings/ovl/em37/em37", D_80100004);

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80100FE0);

void func_80101598(void)
{
    SVECTOR Unused;
}

void func_801015A8(void) {
}

/* RE2 Em37_Scenario */
void func_801015B0(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    if (pEm->Type & 0x100)
        func_80016600(pEm, Kan, Seq);
    else
        Em37_sce[pEm->Routine_1](pEm, Kan, Seq);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_8010160C);

void func_80101844(void) {
}

void func_8010184C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em37/em37", func_80101854);
