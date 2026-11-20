#include "common.h"
#include "ovl/em27.h"

/* RE2 EM_MAIN */
void func_8010001C(ENEMY_WORK *pEm)
{
    Em27_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* stay on the floor: undo the move when the ground height changes */
void func_80100060(ENEMY_WORK *pEm)
{
    VECTOR V;

    V.vx = pEm->Pos_x;
    V.vz = pEm->Pos_z;
    V.vy = pEm->Unk122;
    if (func_8004D720(&V, 350, 0x8000, 0) != pEm->Unk122) {
        pEm->Pos_x = pEm->Unk40[0];
        pEm->Pos_z = pEm->Unk40[2];
    } else {
        pEm->Unk40[0] = pEm->Pos_x;
        pEm->Unk40[2] = pEm->Pos_z;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801000E0);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80100378);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80100518);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801005F8);

/* wait, then pick the next move (Routine_1 = 1) */
void func_80100688(ENEMY_WORK *pEm)
{
    if (EM27_W(pEm)->Wait) {
        EM27_W(pEm)->Wait--;
        return;
    }
    EM27_W(pEm)->Wait = (Rnd() & 0x2F) + 0x40;
    EM_RTN(pEm) = 0x101;
    pEm->Damage_cnt &= 0x7F;
}

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801006E8);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801008EC);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80100A50);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80100BAC);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80100CD8);

void func_80100DDC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80100DE4);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80100FFC);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801010D8);

void func_8010131C(void) {
}

void func_80101324(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq)
{
    Joint_move(pEm, Kan, Seq, 0x400);
}

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80101344);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801014B0);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_8010158C);

void func_80101660(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_80101668);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801017FC);

INCLUDE_ASM("asm/nonmatchings/ovl/em27/em27", func_801018D8);

/* number of other live enemies of id 0x23 / 0x28 */
LONG func_801019BC(ENEMY_WORK *pEm)
{
    ENEMY_WORK **ppEm;
    LONG n = 0;

    for (ppEm = &G.pUnk265C; ppEm != G.ppEnemy_end; ppEm++) {
        if (((*ppEm)->Be_flg & 1) && ((*ppEm)->Id == 0x23 || (*ppEm)->Id == 0x28) && *ppEm != pEm)
            n++;
    }
    return n;
}
