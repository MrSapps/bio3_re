#include "sub.h"
#include "snd.h"
#include "joint.h"
#include "pl_wep.h"
#include "root_ck.h"
#include "spl_auto.h"
#include "spl_sce.h"
#include "enemy/em_common.h"
#include "spl.h"

/*
 * SPL_SCE.C (RE2 SPL_SCE.C, reworked): scenario-driven moves of the sub player (Spl_Scenario runs
 * Spl_sce[Routine_1]); Spl_sce_mv* are the shared turn/walk/play helpers, *_sp the variants that play
 * the upper body separately.
 */

/* ---- merged helpers (forks + parent) ---- */
typedef struct { UCHAR b[0xA0]; } FA_A0[2][2];
/* SPL_SCE.C passed Add_speedXZ a third (unused) argument; the 2-argument canonical prototype gives other code */
void Add_speedXZ3(ENEMY_WORK *pEm, LONG muki, LONG Unused) __asm__("Add_speedXZ");


/* local prototypes (this module) */

void Spl_Scenario(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    VECTOR V;
    USHORT f;
    ULONG n;

    if (!(pEm->Type & 0x4000)) {
        pEm->Type |= 0x5000;
    }
    if (!(pEm->Type & 0x1000)) {
        V.vx = pEm->Unk14C;
        V.vz = pEm->Unk14E;
        V.vy = 0;
        Root_ck(pEm, &V, 0, 0);
    }
    f = pEm->Unk144;
    if (!(f & 0x10)) {
        Spl_sce[pEm->Routine_1](pEm, pKan, pSeq);
        pEm->Unk144 &= ~0x800;
    } else if (pEm->Routine_1 != 0x14) {
        if (!(f & 0x800)) {
            Spl_sce[pEm->Routine_1](pEm, pKan, pSeq);
            Spl_sce_hokan_set(pEm, pKan, pSeq);
        } else {
            n = f & 0x80;
            n = n != 0;
            Spl_sce_hokan(pEm, pKan, pSeq, n << 16 | 0x200);
        }
    }
    if (pEm->Unk144 & 1) {
        Foot_set_pl(pEm, 0, 0);
    }
    if (pEm->Unk144 & 2) {
        Foot_set_pl(pEm, 0, 1);
    }
}

/* interpolated frame of the scenario motion set picked by Routine_1 (J_seq_hokan) */
LONG Spl_sce_hokan(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg)
{
    UCHAR r;
    if (pEm->Routine_2 != 2) {
        r = pEm->Routine_1;
        switch (r) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            pKan = pEm->pSub0_kan_t_ptr;
            pSeq = pEm->pSub0_seq_t_ptr;
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        J_seq_hokan(pKan, pSeq, 0x80, Flg);
        if (!(pEm->Unk144 & 0x20)) {
            Add_speedXZ3(pEm, 0, 0);
        }
        pEm->Unk144 &= ~0x800;
    }
}

/* step the scenario motion back a frame and set up the interpolation (Hokan_key_set) */
void Spl_sce_hokan_set(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT n;

    switch (pEm->Routine_1) {
    case 0:
        pKan = pEm->pUnk100_kan_t_ptr;
        pSeq = pEm->pUnk104_seq_t_ptr;
        break;
    case 1:
        pKan = pEm->pKan_t_ptr;
        pSeq = pEm->pSeq_t_ptr;
        break;
    case 2:
        pKan = pEm->pSub0_kan_t_ptr;
        pSeq = pEm->pSub0_seq_t_ptr;
        break;
    case 3:
        pKan = pEm->pSub1_kan_t_ptr;
        pSeq = pEm->pSub1_seq_t_ptr;
        break;
    }
    n = *(USHORT *)&pSeq[pEm->Move_no * 2];
    if (!(pEm->Unk144 & 0x800)) {
        if (pEm->Move_cnt == 0) {
            pEm->Move_cnt = n - 1;
        } else {
            pEm->Move_cnt--;
        }
        Hokan_key_set(pKan, pSeq, pEm->pNow_seq, *(UCHAR *)&pEm->Unk144 >> 7);
    }
    pEm->Unk144 ^= 0x800;
}

/* footstep sound from the current seq frame (unless Unk144 0x100) */
void Spl_sce_foot_se(ENEMY_WORK *pEm)
{
    USHORT v;
    LONG n;

    if (!(pEm->Unk144 & 0x100)) {
        v = *pEm->pNow_seq;
        if (v & 0x200) {
            n = v & 0x400;
            n = n != 0;
            Snd_se_walk(1, n * 3 + 0xB, pEm);
        }
    }
}

void Spl_sce00(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pUnk100_kan_t_ptr, pEm->pUnk104_seq_t_ptr);
    Spl_sce_foot_se(pEm);
}

void Spl_sce01(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr);
    Spl_sce_foot_se(pEm);
}

void Spl_sce02(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr);
    Spl_sce_foot_se(pEm);
}

void Spl_sce03(ENEMY_WORK *pEm)
{
    Spl_sce_mv00(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
    Spl_sce_foot_se(pEm);
}

void Spl_sce04(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 0;

    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = pEm->Unk152;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = n + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)Spl_sce_mv01(pEm, pKan, pSeq)) {
            Spd_get(pEm, pKan, pSeq, 0);
            if (pEm->Col.Hit_flg & 0x40) {
                if (Spl_slope_dir(pEm, 0) & 0x1000) {
                    EM_SET_RTN(pEm, 4, 12, 0, 0);
                } else {
                    EM_SET_RTN(pEm, 4, 12, 2, 0);
                }
                Spl_sce0c(pEm, pKan, pSeq);
                return;
            }
            pEm->Routine_2 = 2;
        }
        break;
    case 2:
        if (pEm->Col.Hit_flg & 0x40) {
            if (Spl_slope_dir(pEm, 0) & 0x1000) {
                EM_SET_RTN(pEm, 4, 12, 0, 0);
            } else {
                EM_SET_RTN(pEm, 4, 12, 2, 0);
            }
            Spl_sce0c(pEm, pKan, pSeq);
            return;
        }
        Spd_get(pEm, pKan, pSeq, 0);
        if ((UCHAR)Spl_sce_mv02(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 4;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Spl_sce_foot_se(pEm);
}

/* Spl_sce slot 0xC */
void Spl_sce0c(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->Col.Hit_flg & 0x40)) {
        EM_SET_RTN(pEm, 4, 4, 2, 0);
        EM_SET_MOVE(pEm, 0, 0, 7);
        Spl_sce04(pEm);
        return;
    }
    if (!(Spl_slope_dir(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else if (pEm->Routine_2 == 3) {
        *(USHORT *)&pEm->Routine_2 = 0;
    }
    switch (pEm->Routine_2) {
    case 0:
        Spl_slope_dir(pEm, 1);
        pEm->Routine_2 = 1;
        Spd_get(pEm, pEm->pUnk19C_kan_t_ptr, pEm->pUnk198_seq_t_ptr, 0);
    case 1:
        Spd_get(pEm, pEm->pUnk19C_kan_t_ptr, pEm->pUnk198_seq_t_ptr, 0);
        if ((UCHAR)Spl_sce_mv02_sp(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xC;
                pEm->Routine_2 = 1;
            }
        }
        break;
    case 2:
        EM_SET_MOVE(pEm, 0, 0, 7);
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if ((UCHAR)Spl_sce_mv02(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xC;
                pEm->Routine_2 = 3;
            }
        }
        break;
    }
    Spl_sce_foot_se(pEm);
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, Spl_slope_dir(pEm, 0), 0x10);
    } else {
        Goto01(pEm, Spl_slope_dir(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void Spl_sce05(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 1;
    ULONG r;
    SHORT *pT;

    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = pEm->Unk152;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = n + 0x70000;
        pEm->Routine_2 = 1;
        pEm->Routine_3 = 0;
    case 1:
        if ((UCHAR)Spl_sce_mv01(pEm, pKan, pSeq)) {
            Spd_get(pEm, pKan, pSeq, 0);
            pEm->Routine_2 = 2;
            pEm->Routine_3 = 1;
        }
        break;
    case 2:
        if (pEm->Col.Hit_flg & 0x40) {
            r = Spl_slope_dir(pEm, 0) & 0x1000;
            if (r) {
                r = 0xD04;
            } else {
                r = 0x20D04;
            }
            EM_RTN(pEm) = r;
            Spl_sce0d(pEm, pKan, pSeq);
            return;
        }
        if (pEm->Id < 0x10) {
            if (pEm->Id < 8) {
                pEm->Spd_x = D_8009EDA4[pEm->Move_cnt];
                pEm->Spd_z = D_8009EDCC[pEm->Move_cnt];
            } else {
                pEm->Spd_x = D_8009EDF4[pEm->Move_cnt];
                pEm->Spd_z = D_8009EE1C[pEm->Move_cnt];
            }
            if (pEm->Routine_3) {
                pEm->Routine_3 = 0;
                pEm->Spd_z = 0;
                pEm->Spd_x = 0;
            }
        } else {
            Spd_get(pEm, pKan, pSeq, 0);
        }
        if ((UCHAR)Spl_sce_mv02(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 5;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Spl_sce_foot_se(pEm);
}

/* Spl_sce slot 0xD */
void Spl_sce0d(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(pEm->Col.Hit_flg & 0x40)) {
        EM_SET_RTN(pEm, 4, 5, 2, 0);
        EM_SET_MOVE(pEm, 1, 0, 7);
        Spl_sce05(pEm);
        return;
    }
    if (!(Spl_slope_dir_run(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else if (pEm->Routine_2 == 3) {
        *(USHORT *)&pEm->Routine_2 = 0;
    }
    switch (pEm->Routine_2) {
    case 0:
        Spl_slope_dir_run(pEm, 1);
        pEm->Routine_2 = 1;
        Spd_get(pEm, pEm->pUnk19C_kan_t_ptr, pEm->pUnk198_seq_t_ptr, 0);
    case 1:
        Spd_get(pEm, pEm->pUnk19C_kan_t_ptr, pEm->pUnk198_seq_t_ptr, 0);
        if ((UCHAR)Spl_sce_mv02_sp(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xD;
                pEm->Routine_2 = 1;
            }
        }
        break;
    case 2:
        EM_SET_MOVE(pEm, 1, 0, 7);
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if ((UCHAR)Spl_sce_mv02(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xD;
                pEm->Routine_2 = 3;
            }
        }
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, Spl_slope_dir_run(pEm, 0), 0x10);
    } else {
        Goto01(pEm, Spl_slope_dir_run(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void Spl_sce06(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 2;

    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = pEm->Unk152;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = n + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        Joint_move2(pEm, pKan, pSeq, 0x200);
        break;
    }
}

void Spl_sce07(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 9;
    ULONG r;

    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = pEm->Unk152;
    }
    if (pEm->Col.Hit_flg & 0x40) {
        r = Spl_slope_dir(pEm, 0) & 0x1000;
        if (r) {
            r = 0xE04;
        } else {
            r = 0x20E04;
        }
        EM_RTN(pEm) = r;
        Spl_sce0e(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = n + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if ((UCHAR)Spl_sce_mv02(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 7;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Spl_sce_foot_se(pEm);
}

/* Spl_sce slot 0xE */
void Spl_sce0e(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if (!(Spl_slope_dir(pEm, 0) & 0x1000)) {
        if (pEm->Routine_2 == 1) {
            if (pEm->Move_no == 0) {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 7;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            } else {
                pEm->Move_cnt *= 30;
                pEm->Move_cnt = pEm->Move_cnt / 34 + 3;
                if (pEm->Move_cnt >= 30) {
                    pEm->Move_cnt -= 30;
                }
            }
            *(USHORT *)&pEm->Routine_2 = 2;
        }
    } else if (pEm->Routine_2 == 3) {
        *(USHORT *)&pEm->Routine_2 = 0;
    }
    if (!(pEm->Col.Hit_flg & 0x40)) {
        EM_SET_RTN(pEm, 4, 7, 0, 0);
        Spl_sce07(pEm, pKan, pSeq);
        return;
    }
    switch (pEm->Routine_2) {
    case 0:
        Spl_slope_dir(pEm, 1);
        pEm->Routine_2 = 1;
        Spd_get(pEm, pEm->pUnk19C_kan_t_ptr, pEm->pUnk198_seq_t_ptr, 1);
    case 1:
        Spd_get(pEm, pEm->pUnk19C_kan_t_ptr, pEm->pUnk198_seq_t_ptr, 1);
        if ((UCHAR)Spl_sce_mv02_sp(pEm, pKan, pSeq, 1)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xE;
                pEm->Routine_2 = 1;
            }
        }
        break;
    case 2:
        EM_SET_MOVE(pEm, 9, 0, 7);
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        pEm->Routine_2 = 3;
    case 3:
        Spd_get(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0);
        if ((UCHAR)Spl_sce_mv02(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0xE;
                pEm->Routine_2 = 3;
            }
        }
        break;
    }
    if (pEm->Col.Hit_flg & 0x20) {
        Goto01(pEm, Spl_slope_dir(pEm, 0), 0x10);
    } else {
        Goto01(pEm, Spl_slope_dir(pEm, 0), 4);
    }
    Add_speedXZ(pEm, 0);
}

void Spl_sce08(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG n = 0;

    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            pKan = pEm->pSub0_kan_t_ptr;
            pSeq = pEm->pSub0_seq_t_ptr;
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = pEm->Unk152;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = n + 0x70000;
        pEm->Spd_z = 0;
        pEm->Routine_2 = 1;
    case 1:
        Spd_get(pEm, pKan, pSeq, 0);
        if ((UCHAR)Spl_sce_mv02(pEm, pKan, pSeq, 0)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 8;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Spl_sce_foot_se(pEm);
}

void Spl_sce09(ENEMY_WORK *pEm)
{
    ULONG *pKan = pEm->pSub0_kan_t_ptr;
    ULONG *pSeq = pEm->pSub0_seq_t_ptr;
    LONG n = 0;

    if (pEm->Unk150 & 0x80) {
        switch (pEm->Unk150 & 0x7F) {
        case 0:
            pKan = pEm->pUnk100_kan_t_ptr;
            pSeq = pEm->pUnk104_seq_t_ptr;
            break;
        case 1:
            pKan = pEm->pKan_t_ptr;
            pSeq = pEm->pSeq_t_ptr;
            break;
        case 2:
            break;
        case 3:
            pKan = pEm->pSub1_kan_t_ptr;
            pSeq = pEm->pSub1_seq_t_ptr;
            break;
        }
        n = pEm->Unk152;
    }
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = n + 0x70000;
        pEm->Routine_2 = 1;
    case 1:
        if ((UCHAR)Spl_sce_mv01(pEm, pKan, pSeq)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 9;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Spl_sce_foot_se(pEm);
}

/* Routine_2 = low byte of Dest_x */
void Spl_sce0a_init(ENEMY_WORK *pEm)
{
    pEm->Routine_2 = *(UCHAR *)&pEm->Dest_x; /* low byte */
}

/* Spl_sce slot 0xA: sub-routine table D_8009EEA4 by Routine_2 */
void Spl_sce0a(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    if ((pEm->Routine_2 | pEm->Routine_3) == 0) {
        Spl_sce0a_init(pEm);
    }
    Spl_sce0a_tbl[pEm->Routine_2](pEm, pKan, pSeq);
}

/* Spl_sce0a sub-routine 0 (and 5) */
void Spl_sce0a_00(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_MOVE(pEm) = pEm->Routine_2 ? 0x70007 : 0x70000;
        *(LONG *)&pEm->Sub_move_no = pEm->Routine_2 ? 0x70007 : 0x70000;
        pEm->Neck_flg |= 0x12;
    case 1:
        if (Joint_move(pEm, pEm->pUnk18C_kan_t_ptr, pEm->pUnk188_seq_t_ptr, 0x200) != 0) {
            *(USHORT *)&pEm->Routine_2 = pEm->Routine_2 ? 6 : 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x200);
        func_80079868(pEm, 0x200);
        break;
    }
}

/* Spl_sce0a sub-routine 1 (and 6) */
void Spl_sce0a_01(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_MOVE(pEm) = pEm->Dest_z * 2 + (pEm->Routine_2 != 1 ? 0x7000A : 0x70002);
        *(LONG *)&pEm->Sub_move_no = (pEm->Dest_z << 1) + (pEm->Routine_2 != 1 ? 0x7000A : 0x70002);
    case 1:
        Joint_move(pEm, pEm->pUnk18C_kan_t_ptr, pEm->pUnk188_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x200);
        func_80079868(pEm, 0x200);
        Flg_on(D_800D1FC0, pEm->Set_flg_no);
        break;
    }
}

/* Spl_sce0a sub-routine 2 */
void Spl_sce0a_02(ENEMY_WORK *pEm)
{
    USHORT f = pEm->Dest_z;
    LONG n = f & 0xF;
    ENEMY_WORK *pT;
    PARTS_W *pP;
    SVECTOR sv;
    LONG r;

    if (f & 0xF0) {
        pT = G.pEnemy[(f >> 4) & 0xF];
        if (pT != pEm) {
            Goto00(pEm, pT->Pos_x, pT->Pos_z, 0xD8);
        }
        if (Goto00_ck(pEm, pT->Pos_x, pT->Pos_z, 0x50)) {
            return;
        }
        pEm->Dest_z &= 0xF;
    }
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        pP = pEm->pSin_parts_ptr;
        *(LONG *)&pEm->Sub_move_no = EM_MOVE(pEm) = n * 2 + 0x30001;
        Snd_se_enem(0xF, pEm);
        sv.vx = 100;
        sv.vy = 150;
        sv.vz = 0;
        Esp_call(2, ((USHORT)(pEm->Cdir_y + 0x400)) | 0x8000000, &pP[4].Workm, &sv, 0);
        sv.vx = 100;
        sv.vy = 450;
        sv.vz = 0;
        Esp_call(1, (USHORT)pEm->Cdir_y | 0x10000000, &pP[4].Workm, &sv, 0);
        sv.vx = 100;
        sv.vy = 350;
        sv.vz = 0;
        Esp_call(3, (USHORT)pEm->Cdir_y | 0x10000000, &pP[4].Workm, &sv, 0);
        SPL_W(pEm)->Unk1D0 &= ~0x4000;
        r = (LONG)func_80044804(&pEm->pSin_parts_ptr[4].Workm, D_8009DBB4[8], 3); /* 0xFF: no target */
        if (r != 0 && r != 0xFF) {
            SPL_W(pEm)->Unk1D0 |= 0x4000;
        }
    case 1:
        if (Joint_move(pEm, pEm->pUnk18C_kan_t_ptr, pEm->pUnk188_seq_t_ptr, 0x200) != 0) {
            *(USHORT *)&pEm->Routine_2 = 1;
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x400);
        func_80079868(pEm, 0x200);
        break;
    }
}

/* Rnd (SLD.S) seen with the return types the original files declared: signed char here, long below */
signed char Rnd_sc(void) __asm__("Rnd");

/* Spl_sce0a sub-routine 7 */
void Spl_sce0a_07(ENEMY_WORK *pEm)
{
    PARTS_W *pP;
    SVECTOR sv;
    SVECTOR sv2;
    ESP_WORK *pR;

    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        *(LONG *)&pEm->Sub_move_no = EM_SET_MOVE(pEm, 8, 0, 3);
    case 1:
        pP = pEm->pSin_parts_ptr;
        if ((UCHAR)(pEm->Move_cnt % 3) == 0) {
            Snd_se_enem(0xF, pEm);
            sv.vx = 0xBE;
            sv.vy = 0x514;
            sv.vz = 0x14;
            Esp_call(1, 0x12000000, &pP[4].Workm, &sv, 0);
            sv.vx = 0xBE;
            sv.vy = 0x640;
            sv.vz = 0x14;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = -0x400;
            Esp_call(0x801, 0x12000000, &pP[4].Workm, &sv, &sv2);
            sv.vx = 0x186;
            sv.vy = 0x58C;
            sv.vz = 0x14;
            sv2.vx = -0x400;
            sv2.vy = 0x600;
            sv2.vz = 0;
            Esp_call(0x801, 0x0A000000, &pP[4].Workm, &sv, &sv2);
            sv.vx = 0x64;
            sv.vy = 0x58C;
            sv.vz = -0x64;
            sv2.vx = -0x200;
            sv2.vy = 0x200;
            sv2.vz = -0x300;
            Esp_call(0x801, 0x0A000000, &pP[4].Workm, &sv, &sv2);
            sv.vx = 0x64;
            sv.vy = 0x58C;
            sv.vz = 0x8C;
            sv2.vx = 0x200;
            sv2.vy = -0x200;
            sv2.vz = -0x300;
            Esp_call(0x801, 0x0A000000, &pP[4].Workm, &sv, &sv2);
            sv.vx = 0xDA;
            sv.vy = 0x185;
            sv.vz = -0x26;
            Esp_call(0x102, ((USHORT)(pEm->Cdir_y + 0x400)) | 0x8000000, &pP[4].Workm, &sv, 0);
        }
        if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
            sv.vx = 0xBE;
            sv.vy = 0x514;
            sv.vz = 0x14;
            Esp_call(0x603, (USHORT)Rnd_sc() | 0x10000000, &pP[4].Workm, &sv, 0);
            sv.vx = 0xDA;
            sv.vy = 0x185;
            sv.vz = -0x26;
            pR = Esp_call(0x603, (USHORT)Rnd_sc() | 0x10000000, &pP[4].Workm, &sv, 0);
            if (pR != ESP_NONE) {
                pR->Spd_x = 0x14;
                pR->Spd_y = -0x28;
            }
        }
        if (Joint_move(pEm, pEm->pUnk18C_kan_t_ptr, pEm->pUnk188_seq_t_ptr, 0x200) != 0) {
            if (--pEm->Dest_z == 0) {
                pEm->Routine_3++;
            }
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x400);
        func_80079868(pEm, 0x200);
        break;
    case 2:
        *(LONG *)&pEm->Sub_move_no = EM_SET_MOVE(pEm, 9, 0, 3);
        pEm->Routine_3++;
    case 3:
        pEm->Routine_3 += Joint_move(pEm, pEm->pUnk18C_kan_t_ptr, pEm->pUnk188_seq_t_ptr, 0x200);
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x400);
        func_80079868(pEm, 0x200);
        break;
    case 4:
        *(USHORT *)&pEm->Routine_2 = 6;
        break;
    }
}

/* Spl_sce0a sub-routine 3 (and 8) */
void Spl_sce0a_03(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_3) {
    case 0:
        pEm->Routine_3 = 1;
        EM_MOVE(pEm) = pEm->Routine_2 != 3 ? 0x70007 : 0x70000;
        *(LONG *)&pEm->Sub_move_no = pEm->Routine_2 != 3 ? 0x70007 : 0x70000;
    case 1:
        if (Joint_move(pEm, pEm->pUnk18C_kan_t_ptr, pEm->pUnk188_seq_t_ptr, 0x10200) != 0) {
            EM_SET_RTN(pEm, 4, 6, 0, 0);
            pEm->Neck_flg &= 0xED;
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
        }
        pEm->Be_flg |= 0x2000000;
        func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x10200);
        func_80079868(pEm, 0x200);
        break;
    }
}

/* Spl_sce0a sub-routine 4 */
void Spl_sce0a_04(ENEMY_WORK *pEm)
{
    LONG pad[2];

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        EM_SET_MOVE(pEm, 7, 0, 7);
        *(LONG *)&pEm->Sub_move_no = 0x70007;
    }
    if (Joint_move(pEm, pEm->pUnk18C_kan_t_ptr, pEm->pUnk188_seq_t_ptr, 0x200) != 0) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x200);
    func_80079868(pEm, 0x200);
}

/* Spl_sce slot 0xB: held by the enemy pEnemy[Dest_z + 2], plays its Sub1 motions */
void Spl_sce0b(ENEMY_WORK *pEm)
{
    pEm->pEnemy_ptr = G.pEnemy[pEm->Dest_z + 2];
    pEm->pSub1_kan_t_ptr = pEm->pEnemy_ptr->pSub1_kan_t_ptr;
    pEm->pSub1_seq_t_ptr = pEm->pEnemy_ptr->pSub1_seq_t_ptr;
    Spl_sce0b_move(pEm, pEm->pSub1_kan_t_ptr, pEm->pSub1_seq_t_ptr);
}

LONG Rnd_l(void) __asm__("Rnd");

/* Spl_sce0b motion: struggle, bleed, break free */
void Spl_sce0b_move(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    SVECTOR sv;
    UCHAR Mv[2] = { 0, 3 };
    SHORT Ofs[2][3] = { { 0, 200, 100 }, { -100, 100, -200 } };
    ENEMY_WORK *pT;
    SHORT x;
    SHORT z;

    pT = pEm->pEnemy_ptr;
    pEm->Add_dir = Cdir_ck(pT, pEm);
    switch (pEm->Routine_2) {
    case 0:
        EM_MOVE(pEm) = Mv[pEm->Add_dir] + 0x30000;
        pEm->Routine_2 = 1;
        pEm->Timer0 = 0x800;
        pEm->UnkE2 = 0;
        if (pEm->Add_dir & 1) {
            pEm->Timer0 = 0;
        }
        pEm->UnkDA = 0;
        pEm->Neck_flg |= 0x12;
        Set_base_pos(pEm, pKan, pSeq, 0);
        pEm->Be_flg |= 0x8800;
        pT->Be_flg |= 0x800;
    case 1:
        Goto01(pEm, pT->Cdir_y + pEm->Timer0, 0xD0);
        x = pEm->Pos_x;
        z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - x) * pEm->Hokan_flg) >> 2;
        pEm->Pos_z -= ((pEm->Pos_z - z) * pEm->Hokan_flg) >> 2;
        if (Joint_move2(pEm, pKan, pSeq, 0x400) != 0) {
            if (pEm->Dest_x == 0) {
                pEm->Routine_2 = 4;
                Flg_on(D_800D1FC0, pEm->Set_flg_no);
            } else {
                pEm->Routine_2++;
            }
        }
        break;
    case 2:
        EM_MOVE(pEm) = Mv[pEm->Add_dir] + 1;
        pEm->Routine_2 = 3;
    case 3:
        if (pEm->Move_cnt == 15) {
            PARTS_W *pP = pEm->pSin_parts_ptr; /* block-local: must not share a pseudo with case 4 */

            sv.vx = Ofs[pEm->Add_dir & 1][0];
            sv.vy = Ofs[pEm->Add_dir & 1][1];
            sv.vz = Ofs[pEm->Add_dir & 1][2];
            Esp_call(0, (USHORT)(pEm->Cdir_y + Rnd_l() * 2 - 0x100 + ((pEm->Add_dir & 1) << 11)) | 0x17D00000, &pP[1].Workm, &sv, 0);
            pEm->UnkE2 = 0;
        }
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        if (Joint_move2(pEm, pKan, pSeq, 0x100) != 0) {
            if (--pEm->Dest_x == 0) {
                pEm->Routine_2++;
                Flg_on(D_800D1FC0, pEm->Set_flg_no);
            }
        }
        break;
    case 4:
        EM_MOVE(pEm) = Mv[pEm->Add_dir] + 0x70002;
        pEm->Routine_2 = 5;
        if (pEm->UnkE2 != 0) {
            PARTS_W *pP = pEm->pSin_parts_ptr;
            sv.vx = Ofs[pEm->Add_dir & 1][0];
            sv.vy = Ofs[pEm->Add_dir & 1][1];
            sv.vz = Ofs[pEm->Add_dir & 1][2];
            Esp_call(0, (USHORT)(pEm->Cdir_y + Rnd_l() * 2 - 0x100 + ((pEm->Add_dir & 1) << 11)) | 0x17D00000, &pP[1].Workm, &sv, 0);
        }
    case 5:
        x = pEm->Pos_x;
        z = pEm->Pos_z;
        pEm->Cdir_y += pEm->Timer0;
        Null_pos_set2(pEm, pKan, pSeq, 0);
        pEm->Cdir_y -= pEm->Timer0;
        pEm->Pos_x -= ((pEm->Pos_x - x) * pEm->Hokan_flg) >> 3;
        pEm->Pos_z -= ((pEm->Pos_z - z) * pEm->Hokan_flg) >> 3;
        pEm->Routine_2 += Joint_move2(pEm, pKan, pSeq, 0x200);
        if (pEm->Move_cnt == 8) {
            pEm->Be_flg &= ~0x800;
            pT->Be_flg &= ~0x8800;
        }
        break;
    case 6:
        pEm->Routine_1 = 6;
        pEm->Routine_2 = 0;
        pEm->Unk152 = 0;
        pEm->Unk150 = 0;
        pEm->Neck_flg &= 0xED;
        break;
    }
}

/* Spl_sce slot 0x14, empty */
void Spl_sce14(void) {
}

/* Spl_sce slot 0x15: turn to Dest_x, then scenario flag Set_flg_no */
void Spl_sce15(ENEMY_WORK *pEm)
{
    switch (pEm->Routine_2) {
    case 0:
        EM_SET_MOVE(pEm, 0, 0, 7);
        pEm->Routine_2 = 1;
    case 1:
        if (Spl_sce_mv03(pEm, pEm->pSub0_kan_t_ptr, pEm->pSub0_seq_t_ptr)) {
            Flg_on(D_800D1FC0, pEm->Set_flg_no);
            pEm->Routine_1 = 6;
            pEm->Routine_2 = 0;
            pEm->Unk152 = 0;
            pEm->Unk150 = 0;
            if (pEm->Unk144 & 4) {
                pEm->Routine_1 = 0x15;
                pEm->Routine_2 = 2;
            }
        }
        break;
    }
    Spl_sce_foot_se(pEm);
}

/* play the scenario motion in place (RE2 Spl_sce_mv04) */
void Spl_sce_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT f;
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Hokan_flg = 7;
        if (pEm->Unk144 & 0x40) {
            pEm->Hokan_flg = 0;
        }
    case 1:
        if (Joint_move2(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
            }
            pEm->Cdir_y += pEm->Unk14A;
        } else {
            f = pEm->Unk144;
            if (f & 8) {
                n = f & 0x80;
                n = n != 0;
                if (Joint_move2(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += pEm->Unk14A;
                }
            }
            pEm->Cdir_y += pEm->Add_dir;
        }
        break;
    case 2:
        break;
    }
}

/* turn towards (Dest_x, Dest_z) while playing the motion; 1 when facing it (RE2 Spl_sce_mv01) */
LONG Spl_sce_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    LONG ret;

    if (Goto00_ck(pEm, pEm->Dest_x, pEm->Dest_z, 0x15E) != 0) {
        Goto00(pEm, pEm->Dest_x, pEm->Dest_z, pEm->Add_dir);
        Joint_move2(pEm, pKan, pSeq, 0x200);
        ret = 0;
    } else {
        Joint_move2(pEm, pKan, pSeq, 0x200);
        ret = 1;
    }
    return ret;
}

/* walk to (Dest_x, Dest_z); 1 when there (RE2 Spl_sce_mv02) */
LONG Spl_sce_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d)
{
    LONG x, z;

    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * pEm->Add_dir);
    Joint_move2(pEm, pKan, pSeq, (d << 16) | 0x200);
    Add_speedXZ(pEm, d);
    x = pEm->Pos_x - pEm->Dest_x;
    z = pEm->Pos_z - pEm->Dest_z;
    return SquareRoot0(x * x + z * z) < 0x69;
}

/* turn to the direction Dest_x; 1 when reached (RE2 Spl_sce_mv03) */
UCHAR Spl_sce_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    Goto01(pEm, pEm->Dest_x, pEm->Dest_z);
    Joint_move2(pEm, pKan, pSeq, 0x200);
    return pEm->Cdir_y == pEm->Dest_x;
}

void Spl_sce_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT f;
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Hokan_flg = 7;
        if (pEm->Unk144 & 0x40) {
            pEm->Hokan_flg = 0;
        }
        Spd_get2(pEm, pKan, pSeq, *(UCHAR *)&pEm->Unk144 >> 7);
    case 1:
        Spd_get2(pEm, pKan, pSeq, *(UCHAR *)&pEm->Unk144 >> 7);
        if (pEm->Unk144 & 0x10) {
            pEm->Spd_x >>= 1;
            pEm->Spd_y >>= 1;
            pEm->Spd_z >>= 1;
        }
        if (Joint_move2(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
                if (pEm->Unk144 & 0x10) {
                    pEm->Unk144 |= 0x800;
                }
            }
            pEm->Cdir_y += pEm->Unk14A;
            if (!(pEm->Unk144 & 0x20)) {
                Add_speedXZ3(pEm, 0, 0);
            }
        } else {
            if (!(pEm->Unk144 & 0x20)) {
                Add_speedXZ3(pEm, 0, 0);
            }
            f = pEm->Unk144;
            if (f & 8) {
                n = f & 0x80;
                n = n != 0;
                if (Joint_move2(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += pEm->Unk14A;
                }
                if (!(pEm->Unk144 & 0x20)) {
                    Add_speedXZ3(pEm, 0, 0);
                }
            }
            pEm->Cdir_y += pEm->Add_dir;
        }
        break;
    case 2:
        break;
    }
}

/* Spl_sce_mv00 with the upper body played separately (Id 0) */
void Spl_sce_mv00_sp(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq)
{
    USHORT f;
    ULONG n;

    switch (pEm->Routine_2) {
    case 0:
        pEm->Routine_2 = 1;
        pEm->Hokan_flg = 7;
        if (pEm->Unk144 & 0x40) {
            pEm->Hokan_flg = 0;
        }
        Spd_get(pEm, pKan, pSeq, *(UCHAR *)&pEm->Unk144 >> 7);
    case 1:
        if (Joint_move(pEm, pKan, pSeq, ((*(UCHAR *)&pEm->Unk144 >> 7) << 16) | 0x200) != 0) {
            pEm->Routine_2 = 2;
            if (pEm->Unk144 & 4) {
                pEm->Routine_2 = 1;
            }
            pEm->Cdir_y += pEm->Unk14A;
            if (!(pEm->Unk144 & 0x20)) {
                Add_speedXZ3(pEm, 0, 0);
            }
        } else {
            if (pEm->Id == 0) {
                pEm->Be_flg |= 0x2000000;
                func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x200);
                func_80079868(pEm, 0x200);
            }
            if (!(pEm->Unk144 & 0x20)) {
                Add_speedXZ3(pEm, 0, 0);
            }
            f = pEm->Unk144;
            if (f & 8) {
                n = f & 0x80;
                n = n != 0;
                if (Joint_move(pEm, pKan, pSeq, (n << 16) | 0x200) != 0) {
                    pEm->Routine_2 = 2;
                    if (pEm->Unk144 & 4) {
                        pEm->Routine_2 = 1;
                    }
                    pEm->Cdir_y += pEm->Unk14A;
                }
                if (pEm->Id == 0) {
                    pEm->Be_flg |= 0x2000000;
                    func_80079690(pEm, pEm->pUnk184_kan_t_ptr, pEm->pUnk180_seq_t_ptr, 0x200);
                    func_80079868(pEm, 0x200);
                }
                if (!(pEm->Unk144 & 0x20)) {
                    Add_speedXZ3(pEm, 0, 0);
                }
            }
            pEm->Cdir_y += pEm->Add_dir;
        }
        break;
    case 2:
        break;
    }
}

/* Spl_sce_mv02 with split upper/lower body motions */
LONG Spl_sce_mv02_sp(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG d)
{
    LONG x, z;

    Goto00(pEm, pEm->Dest_x, pEm->Dest_z, (1 - ((USHORT)pEm->Spd_x >> 15) * 2) * pEm->Add_dir);
    Joint_move(pEm, pEm->pUnk19C_kan_t_ptr, pEm->pUnk198_seq_t_ptr, (d << 16) | 0x200);
    pEm->Be_flg |= 0x2000000;
    func_80079690(pEm, pEm->pUnk194_kan_t_ptr, pEm->pUnk190_seq_t_ptr, d ? 0x10200 : 0x200);
    func_80079868(pEm, 0x200);
    pEm->Spd_x = (SHORT)((USHORT)pEm->Spd_x << 2) / 3;
    Add_speedXZ(pEm, 0);
    x = pEm->Pos_x - pEm->Dest_x;
    z = pEm->Pos_z - pEm->Dest_z;
    return SquareRoot0(x * x + z * z) < 0x64;
}

/* direction to turn to (bit 0x1000: turn around); with f set, also picks the turn move */
LONG Spl_slope_dir(ENEMY_WORK *pEm, LONG f)
{
    SHORT n;
    LONG Dir;
    ULONG d;
    LONG Mv;
    LONG Ret;

    n = Spl_mot_set_no(pEm);
    Dir = Spl_slope_dir_tbl[(UCHAR)pEm->Col.Unk39 & 7];
    d = (Dir - pEm->Cdir_y) & 0xFFF;
    Ret = Dir;
    if (d - 0x400 > 0x800) {
        if (d - 0x258 <= 0xB50) {
            return Dir;
        }
        if (f) {
            pEm->Move_cnt *= 34;
            pEm->Move_cnt /= 30;
            if (pEm->Move_cnt >= 3) {
                pEm->Move_cnt -= 3;
            } else {
                pEm->Move_cnt += 31;
            }
            Mv = D_8009ED88[n] * 4;
            if (pEm->Routine_0 == 1 && Spl_life_u[0]) {
                Mv = 0xC;
            }
            EM_MOVE(pEm) = (pEm->Move_cnt << 8) + 0x70000 + Mv;
            *(LONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70000;
        }
        return Ret + 0x1000;
    }
    if (d - 0x5A9 <= 0x4AE) {
        if (f) {
            pEm->Move_cnt *= 34;
            pEm->Move_cnt /= 30;
            if (pEm->Move_cnt >= 4) {
                pEm->Move_cnt -= 2;
            } else {
                pEm->Move_cnt += 30;
            }
            Mv = D_8009ED88[n] * 4 + 1;
            if (pEm->Routine_0 == 1 && Spl_life_u[0]) {
                Mv = 0xD;
            }
            EM_MOVE(pEm) = (pEm->Move_cnt << 8) + 0x70000 + Mv;
            *(LONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70001;
        }
        return ((Ret + 0x800) & 0xFFF) | 0x1000;
    }
    return (Dir + 0x800) & 0xFFF;
}

/* Pl_slope_dir_run for the sub player */
LONG Spl_slope_dir_run(ENEMY_WORK *pEm, LONG f)
{
    SHORT n;
    LONG Dir;
    ULONG d;
    LONG Mv;
    LONG Ret;

    n = Spl_mot_set_no(pEm);
    Dir = Spl_slope_dir_tbl[(UCHAR)pEm->Col.Unk39 & 7];
    d = (Dir - pEm->Cdir_y) & 0xFFF;
    Ret = Dir;
    if (d - 0x400 > 0x800) {
        if (d - 0x258 <= 0xB50) {
            return Dir;
        }
        if (f) {
            Mv = D_8009ED88[n] * 4 + 2;
            if (pEm->Routine_0 == 1 && Spl_life_u[0]) {
                Mv = 0xE;
            }
            EM_MOVE(pEm) = (pEm->Move_cnt << 8) + 0x70000 + Mv;
            *(LONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70002;
        }
        return Ret + 0x1000;
    }
    if (d - 0x5A9 <= 0x4AE) {
        if (f) {
            Mv = D_8009ED88[n] * 4 + 3;
            if (pEm->Routine_0 == 1 && Spl_life_u[0]) {
                Mv = 0xF;
            }
            EM_MOVE(pEm) = (pEm->Move_cnt << 8) + 0x70000 + Mv;
            *(LONG *)&pEm->Sub_move_no = (pEm->Move_cnt << 8) + 0x70003;
        }
        return ((Ret + 0x800) & 0xFFF) | 0x1000;
    }
    return (Dir + 0x800) & 0xFFF;
}

/* motion set of the sub player: 0 (Unk4B 0 or Id < 8), 1 (Id < 0x10), else Id - 0x4E */
LONG Spl_mot_set_no(ENEMY_WORK *pEm)
{
    LONG id;

    if (pEm->Unk4B == 0 || (id = pEm->Id) < 8) {
        return 0;
    }
    if (id < 0x10) {
        return 1;
    }
    return id - 0x4E;
}
