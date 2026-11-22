#include "ovl/em2c.h"
#include "joint.h"
#include "md_sub.h"

/* RE2 EM_MAIN */
void func_8010001C(ENEMY_WORK *pEm)
{
    Em2c_routine_0[pEm->Routine_0](pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
}

/* RE2 Em2c_Init */
void func_80100060(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    /* parts hidden for Type 3.. : -1 separated lists, list n hides for Type >= n + 3 */
    signed char Hide[23] = {
        1, -1, 4, -1, 3, 7, -1, 10, 11, -1, 9, 14, -1, 2, 6, 13, -1, 5, 12, -1, 0, 8, -1,
    };
    PARTS_W *pP;
    signed char n;
    signed char i;

    EM_RTN(pEm) = 1;
    if ((pEm->Type & 0xF) < 2) {
        EM_MOVE(pEm) = 0x4801;
    } else {
        EM_MOVE(pEm) = 0;
    }
    Joint_move(pEm, pKan, pSeq, 0x100);
    if (!(pEm->Type & 0xF)) {
        Kage_work_set(&pEm->Kage.pKw, 0, 0x2260226, 0xBFBF10, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
    } else {
        Kage_work_set(&pEm->Kage.pKw, 0, 0x44C044C, 0xBFBF10, (VECTOR *)pEm->pSin_parts_ptr->Workm.t);
    }
    pEm->Unk134 = 1500;
    pEm->Unk136 = -1000;
    pEm->Unk138 = 0x54;
    pEm->Unk139 = 0xA0;
    pEm->Unk13A = 450;
    pEm->Unk140 = 0;
    pEm->Unk120 = 0;
    pEm->Neck_no = 1;
    pEm->pUnk13C = (UCHAR *)&pEm->pSin_parts_ptr->Workm;
    pP = &pEm->pSin_parts_ptr[1];
    PW_FREE(pP)[2] = 0x40;
    PW_FREE(pP)[3] = 0x40;
    PW_FREE(pP)[4] = 0x200;
    PW_FREE(pP)[0] = 0;
    PW_FREE(pP)[1] = 0;
    PW_FREE(pP)[5] = 0x138;
    if ((pEm->Type & 0xF) < 2) {
        pP->Cdir_z += 0x18;
    } else {
        i = 0;
        for (n = ((UCHAR)pEm->Type & 0xF) - 3; n != -1; n--) {
            while (Hide[i] != -1) {
                pP = &pEm->pSin_parts_ptr[Hide[i]];
                i++;
                pP->UnkA2 &= ~1;
            }
            i++;
        }
    }
    if ((pEm->Type & 0xF) >= 3) {
        pEm->Cdir_y -= 0x200;
        RotMatrix_gte((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
        pP = pEm->pSin_parts_ptr;
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[5];
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[6];
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pP->Be_flg |= 2;
        pEm->Cdir_y += 0x200;
    }
    if ((pEm->Type & 0xF) >= 6) {
        pEm->Cdir_y += 0x200;
        RotMatrix_gte((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
        pP = pEm->pSin_parts_ptr;
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[8];
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[12];
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pP = &pEm->pSin_parts_ptr[13];
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pP->Be_flg |= 2;
        pEm->Cdir_y -= 0x200;
    }
    if ((pEm->Type & 0xF) >= 8) {
        pEm->Cdir_y -= 0x200;
        RotMatrix_gte((SVECTOR *)&pEm->Cdir_x, (MATRIX *)pEm->Workm_m);
        pP = pEm->pSin_parts_ptr;
        func_8002D4B0(pP->pSuper, (MATRIX *)pP->M, &pP->Workm);
        pEm->pSin_parts_ptr[8].Be_flg |= 2;
        pEm->Cdir_y += 0x200;
    }
    pEm->Life = -1;
    pEm->Max_life = -1;
    pEm->Damage_cnt = 0x80;
    pEm->Type |= 0x8000;
}
/* the original .rodata ends with a junk byte after Hide[] (linker filler: slot 2 has 0x55, ovl/em2c_2.fixup) */
__asm__(".section .rodata\n\t.byte 0xFE\n\t.section .text");

/* RE2 Em2c_Move: grow the shadow of Type 0 */
void func_801004B0(ENEMY_WORK *pEm)
{
    KAGE_WORK9 *pK = pEm->Kage.pKw9;

    if (!pEm->Type) {
        if (pK->Half_x < 1100) {
            pK->Half_z = pK->Half_x = pK->Half_x + 5;
        }
    }
}
