#include "pl_wep.h"
#include "sca.h"
#include "monitor.h"
#include "pl_wep2.h"
#include "pad.h"

/* RE2 Esp_call-like effect spawn; returns the effect work or -1 */
typedef struct {
    UCHAR pad0[0x10];
    SHORT Unk10;                       /* 0x10 */
    SHORT Unk12;                       /* 0x12 */
    SHORT Unk14;                       /* 0x14 */
} ESP_WORK;
ESP_WORK *func_8001B484(ULONG Id, ULONG Dir, MATRIX *pM, SVECTOR *pV, SVECTOR *pDsv);
SHORT func_8006D030(ULONG Arg0);
void func_80044804(MATRIX *pM, ULONG Arg1, ULONG Type);
void func_80047860(MATRIX *pM, ULONG Arg1, ULONG Type);
extern ULONG D_8009DBB4[];
ULONG func_800102E8(void);                 /* rand */
SHORT Get_dir_xz(LONG X, LONG Z);
void func_800455B4(void);
LONG func_8004EF74(ULONG P0, ULONG P1, ULONG P2, ULONG P3);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800402F4);

LONG func_80026BE8(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079690(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Flg);
void func_80079868(ENEMY_WORK *pEm, ULONG Flg);
void func_8006CF0C(ULONG Arg0);
void func_80044F34(PLAYER_WORK *pEm);

void func_80040764(PLAYER_WORK *pEm)
{
    ULONG n;

    if (pEm->Routine_3 == 0) {
        pEm->Routine_3 = 1;
        *(ULONG *)&pEm->Move_no = 0x7000A;
        *(ULONG *)&pEm->Unk1A0 = 0x7000A;
    }
    if (func_80026BE8(pEm, G.Pl_unk180[3], G.Pl_unk180[2], 0x200) != 0) {
        *(USHORT *)&pEm->Routine_2 = 1;
    }
    pEm->Be_flg |= 0x2000000;
    func_80079690((ENEMY_WORK *)&G.Pl_be_flg, G.Pl_unk180[1], G.Pl_unk180[0], 0x200);
    func_80079868(pEm, 0x200);
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
    }
    if ((*pEm->pNow_seq & 0x400) && (pEm->Status_flg & 0x400)) {
        func_80044F34(pEm);
        pEm->Status_flg &= ~0x400;
    }
    if (*pEm->pNow_seq & 0x200) {
        func_8006CF0C(1);
        pEm->Status_flg |= 0x400;
    }
}

void pl_mv05_w0b(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    pl_mv05_w0b_tbl[pEm->Routine_2](pEm, Kan, Seq, Key);
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", pl_mv05_w0b_t2);

void Pl_wep_ready_reset(PLAYER_WORK *pEm)
{
    PARTS_W *pParts = &pEm->pSin_parts_ptr[22];

    pParts->Unk94 = 0;
    pParts->Unk86 = 0;
    pParts->Unk96 = 0;
}

void Pl_wep_recoil_6(PLAYER_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)((UCHAR)pEm->Type - 6) < 4) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Unk96 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl0[pWep->Unk86];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Unk96 = 0xFF;
                return;
            }
            pWep->Unk86++;
            RotMatrix_gte((SVECTOR *)&pParts[22].Cdir_x, (MATRIX *)pParts[22].M);
        }
    }
}

void Pl_wep_recoil_10(PLAYER_WORK *pEm)
{
    PARTS_W *pParts;
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0x10) {
        pParts = pEm->pSin_parts_ptr;
        pWep = &pParts[22];
        if (pWep->Unk96 != 0xFF) {
            pWep->Cdir_z = Pl_wep_rot_tbl1[pWep->Unk86];
            if ((USHORT)pWep->Cdir_z == 0x8000) {
                pWep->Cdir_z = 0;
                pWep->Unk96 = 0xFF;
                return;
            }
            pWep->Unk86++;
            RotMatrix_gte((SVECTOR *)&pParts[22].Cdir_x, (MATRIX *)pParts[22].M);
        }
    }
}


void Pl_wep_flag_set(PLAYER_WORK *pEm)
{
    pEm->pSin_parts_ptr[22].Unk86 = 1;
}

void Pl_wep_spin_0b(PLAYER_WORK *pEm)
{
    PARTS_W *pWep;

    if ((UCHAR)pEm->Type == 0xB) {
        pWep = &pEm->pSin_parts_ptr[22];
        if (pWep->Unk86 == 0) {
            pWep->Unk96 -= 8;
            if (pWep->Unk96 < 0) {
                pWep->Unk96 = 0;
            }
        } else {
            pWep->Unk96 += 0x20;
            if (pWep->Unk96 > 0x200) {
                pWep->Unk96 = 0x200;
            }
        }
        pWep->Cdir_y += pWep->Unk96;
        RotMatrix((SVECTOR *)&pWep->Cdir_x, (MATRIX *)pWep->M);
        pWep->Unk86 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", pl_w01_t2_move);


void pl_w02_t2_move(PLAYER_WORK *pEm, ULONG Kan, ULONG Seq, ULONG Key)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484(2, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x1C2;
        Sv.vz = 0;
        func_8001B484(1, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        func_8001B484(3, 0x10000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800410F8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80041594);

void func_800418CC(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484(0x1000402, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041A80(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        func_8006D030(1);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484(0x1000702, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041C34(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484(0x1000802, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041DE8(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ULONG n;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0x96;
        Sv.vy = 0x258;
        Sv.vz = 0;
        func_8001B484(0x2000902, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x44C;
        Sv.vz = 0;
        func_8001B484(3, 0x30000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 10) {
        Pl_wep_ready_reset(pEm);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0x8A;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

void func_80041F9C(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = &pEm->pSin_parts_ptr[4];
        func_800746C0(0x10100, (VECTOR *)pEm->pSin_parts_ptr[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        switch ((SHORT)(func_8006D030(1) % 4)) {
        case 0:
            Sv.vx = 0x16A;
            Sv.vy = 0x44C;
            Sv.vz = -0x38;
            break;
        case 1:
            Sv.vx = 0x188;
            Sv.vy = 0x44C;
            Sv.vz = 0x5B;
            break;
        case 2:
            Sv.vx = 0xBD;
            Sv.vy = 0x44C;
            Sv.vz = -0x38;
            break;
        case 3:
            Sv.vx = 0xBD;
            Sv.vy = 0x44C;
            Sv.vz = 0x5B;
            break;
        }
        pM = &pP->Workm;
        func_8001B484(0x1000502, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x17C;
        Sv.vy = 0x4B0;
        Sv.vz = 0;
        func_8001B484(1, 0x38000000, pM, &Sv, 0);
        Sv.vx = 0x17C;
        Sv.vy = 0x320;
        Sv.vz = 0;
        func_8001B484(3, 0x50000000, pM, &Sv, 0);
        Sv.vx = 0xDC;
        Sv.vy = 0;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x30000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 5;
            pEsp->Unk12 = -0x50;
        }
        Sv.vx = 0xDC;
        Sv.vy = -0x3E8;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x30000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 5;
            pEsp->Unk12 = -0x50;
        }
        Game.Plight_flg = 1;
    }
    G.Status_flg &= ~0x40;
}

void func_800421F8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    UCHAR Cnt;

    pP = pEm->pSin_parts_ptr;
    Cnt = pEm->Move_cnt % 3;
    if (Cnt == 0) {
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = -0x6C;
        Sv.vy = 0x528;
        Sv.vz = 0;
        G.Unk75E8 |= 1;
        func_8001B484(1, 0x18000000, pM, &Sv, 0);
        Sv.vy = 0x60E;
        Dsv.vz = -0x400;
        Sv.vx = -0x6C;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484(0x801, 0x18000000, pM, &Sv, &Dsv);
        Sv.vx = -0x190;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484(0xC02, (USHORT)(pEm->Cdir_y - 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = -0xE6;
        Sv.vy = 0x2BC;
        Sv.vz = 0;
        func_8001B484(0x503, (USHORT)(signed char)func_800102E8() | 0x1E000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
        Pl_weapon_vib(pEm);
    }
    if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
        Sv.vx = -0x6C;
        Sv.vy = 0x4B0;
        Sv.vz = 0;
        func_8001B484(0x603, (USHORT)(signed char)func_800102E8() | 0x10000000, &pP[4].Workm, &Sv, 0);
    }
    if (Cnt == 1) {
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
    }
}

void func_80042474(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        func_800455B4();
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x318;
        Sv.vz = 0;
        func_8001B484(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x1EC;
        Sv.vz = 0;
        func_8001B484(3, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x37C;
        Sv.vz = 0;
        func_8001B484(0x703, 0x30000000, pM, &Sv, 0);
        Sv.vx = 0x97;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x8000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = -0x14;
        }
        pEsp = func_8001B484(3, 0x8000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = 0x14;
        }
        pEsp = func_8001B484(3, 0x10000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = -0x1E;
        }
        pEsp = func_8001B484(3, 0x10000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = 0x1E;
        }
        Game.Plight_flg = 1;
    }
}

void func_80042660(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484(2, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x5F;
        Sv.vy = 0x258;
        Sv.vz = 0x20;
        func_8001B484(1, (USHORT)pEm->Cdir_y | 0x13000000, pM, &Sv, 0);
        Sv.vx = 0x5F;
        Sv.vy = 0x1F4;
        Sv.vz = 0x20;
        func_8001B484(3, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

void func_800427D8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ESP_WORK *pEsp;
    UCHAR Cnt;

    Cnt = pEm->Move_cnt;
    pP = &pEm->pSin_parts_ptr[4];
    if (Cnt == 0 || Cnt == 4 || Cnt == 7) {
        Sv.vz = 0x14;
        pM = &pP->Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        func_8001B484(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        func_8001B484(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x528;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        func_8001B484(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        func_8001B484(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        func_8001B484(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        func_8001B484(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484(0x603, (USHORT)(signed char)func_800102E8() | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = func_8001B484(0x603, (USHORT)(signed char)func_800102E8() | 0x10000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_800746C0(0x10100, (VECTOR *)pP->Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        func_8006D030(1);
        Game.Plight_flg = 1;
    }
}

void func_80042AE4(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    PARTS_W *pO;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ESP_WORK *pEsp;

    if ((UCHAR)(pEm->Move_cnt % 3) == 0) {
        pP = pEm->pSin_parts_ptr;
        pO = &pP[4];
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        func_8006D030(1);
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484(1, 0x12000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x640;
        Sv.vz = 0x14;
        Dsv.vx = 0;
        Dsv.vy = 0;
        Dsv.vz = -0x400;
        func_8001B484(0x801, 0x12000000, pM, &Sv, &Dsv);
        Sv.vx = 0x186;
        Sv.vy = 0x528;
        Sv.vz = 0x14;
        Dsv.vx = -0x400;
        Dsv.vy = 0x600;
        Dsv.vz = 0;
        func_8001B484(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = -0x64;
        Dsv.vx = -0x200;
        Dsv.vy = 0x200;
        Dsv.vz = -0x300;
        func_8001B484(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x528;
        Sv.vz = 0x8C;
        Dsv.vx = 0x200;
        Dsv.vy = -0x200;
        Dsv.vz = -0x300;
        func_8001B484(0x801, 0xC000000, pM, &Sv, &Dsv);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        func_8001B484(0x102, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Pl_weapon_vib(pEm);
        Game.Plight_flg = 1;
    }
    if ((UCHAR)(pEm->Move_cnt % 6) == 0) {
        Sv.vx = 0xBE;
        Sv.vy = 0x514;
        Sv.vz = 0x14;
        func_8001B484(0x603, (USHORT)(signed char)func_800102E8() | 0x10000000, &pO->Workm, &Sv, 0);
        Sv.vx = 0xDA;
        Sv.vy = 0x185;
        Sv.vz = -0x26;
        pEsp = func_8001B484(0x603, (USHORT)(signed char)func_800102E8() | 0x10000000, &pO->Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
}

#define RND() ((signed char)func_800102E8())

void func_80042E34(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ULONG n;
    ESP_WORK *pEsp;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x50;
        Sv.vy = 0x33F;
        Sv.vz = 0xD;
        func_8001B484(1, 0x20000000, pM, &Sv, 0);
        Sv.vy = 0x407;
        Dsv.vz = -0x400;
        Sv.vx = 0x50;
        Sv.vz = 0xD;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484(0x801, 0x1F000000, pM, &Sv, &Dsv);
        Sv.vx = 0x50;
        Sv.vy = 0x1A4;
        Sv.vz = 0xD;
        func_8001B484(3, 0x25000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x3A3;
        Sv.vz = 0xD;
        func_8001B484(0x703, 0x2D000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x2DB;
        Sv.vz = 0xD;
        func_8001B484(0x901, (USHORT)pEm->Cdir_y | 0x10000000, pM, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x3D5;
        Sv.vz = 0xD;
        func_8001B484(0x901, (USHORT)pEm->Cdir_y | 0x13000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x50;
        Sv.vy = 0x407;
        Sv.vz = -0xBB;
        Dsv.vx = (RND() >> 1) - 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x50;
        Sv.vy = 0x407;
        Sv.vz = 0xD5;
        Dsv.vx = (RND() >> 1) + 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x118;
        Sv.vy = 0x407;
        Sv.vz = 0xD;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x4A0;
        func_8001B484(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = -0x78;
        Sv.vy = 0x407;
        Sv.vz = 0xD;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x360;
        func_8001B484(0xA01, (RND() + 0xE00) << 16, &pP[4].Workm, &Sv, &Dsv);
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x104;
        Sv.vz = 0xD;
        func_8001B484(0x202, (USHORT)(pEm->Cdir_y + 0x800) | 0xB000000, &pP[4].Workm, &Sv, 0);
        Sv.vx = 0x50;
        Sv.vy = 0x104;
        Sv.vz = 0xD;
        pEsp = func_8001B484(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
    if (pEm->Move_cnt == 9) {
        Pl_wep_ready_reset(pEm);
        Pad_vib_set0(2, 0);
        Pad_vib_set1(6, 0x96, 0);
    }
}

void func_80043328(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x96;
        Sv.vz = 0;
        func_8001B484(0x4000002, (USHORT)(pEm->Cdir_y + 0x400) | 0x8000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x1C2;
        Sv.vz = 0;
        func_8001B484(1, (USHORT)pEm->Cdir_y | 0x16000000, pM, &Sv, 0);
        Sv.vy = 0x2DA;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484(0x801, 0x10000000, pM, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        func_8001B484(3, 0x14000000, pM, &Sv, 0);
        Sv.vx = 0x96;
        Sv.vy = 0x28A;
        Sv.vz = 0;
        func_8001B484(3, 0x18000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
}

void func_800434F8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Dsv;
    ULONG n;
    ESP_WORK *pEsp;

    pP = pEm->pSin_parts_ptr;
    if (pEm->Move_cnt == 0) {
        pM = &pP[4].Workm;
        func_80044804(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_80047860(pM, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        Sv.vx = 0x64;
        Sv.vy = 0x5DC;
        Sv.vz = 0;
        func_8001B484(1, 0x2E000000, pM, &Sv, 0);
        Sv.vy = 0x78A;
        Dsv.vz = -0x400;
        Sv.vx = 0x64;
        Sv.vz = 0;
        Dsv.vx = 0;
        Dsv.vy = 0;
        func_8001B484(0x801, 0x26000000, pM, &Sv, &Dsv);
        Sv.vx = 0x82;
        Sv.vy = 0x3E8;
        Sv.vz = 0;
        func_8001B484(0x2303, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x3E8;
        Sv.vz = 0;
        func_8001B484(3, 0x24000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x514;
        Sv.vz = 0;
        func_8001B484(0x703, 0x2D000000, pM, &Sv, 0);
        Sv.vx = 0x82;
        Sv.vy = 0x76C;
        Sv.vz = 0;
        func_8001B484(0x703, 0x34000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x640;
        Sv.vz = 0;
        func_8001B484(0x901, (USHORT)pEm->Cdir_y | 0x14000000, pM, &Sv, 0);
        Sv.vx = 0x64;
        Sv.vy = 0x76C;
        Sv.vz = 0;
        func_8001B484(0x901, (USHORT)pEm->Cdir_y | 0x1B000000, pM, &Sv, 0);
        Game.Plight_flg = 1;
    }
    if (pEm->Move_cnt == 1) {
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = -0x12C;
        Dsv.vx = (RND() >> 1) - 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x64;
        Sv.vy = 0x78A;
        Sv.vz = 0x12C;
        Dsv.vx = (RND() >> 1) + 0xA0;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x400;
        func_8001B484(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = 0x190;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x4A0;
        func_8001B484(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
        Sv.vx = -0xC8;
        Sv.vy = 0x78A;
        Sv.vz = 0;
        Dsv.vx = RND() >> 1;
        Dsv.vy = 0;
        Dsv.vz = (RND() >> 1) - 0x360;
        func_8001B484(0xA01, (RND() + 0x1300) << 16, &pP[4].Workm, &Sv, &Dsv);
    }
    if (pEm->Move_cnt == 1 || pEm->Move_cnt == 2) {
        func_80044804(&pP[4].Workm, D_8009DBB4[pEm->Id], (UCHAR)pEm->Type);
    }
    n = *pEm->pNow_seq & 0xF000;
    if (n != 0) {
        func_800746C0(((n >> 12) - 1) | 0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        Sv.vx = 0x9D;
        Sv.vy = 0x1EA;
        Sv.vz = 0;
        func_8001B484(0x5000202, (USHORT)(pEm->Cdir_y + 0x400) | 0xB000000, &pP[4].Workm, &Sv, 0);
        Sv.vx = 0xC8;
        Sv.vy = 0x1FE;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x10000000, &pP[4].Workm, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = 0x14;
            pEsp->Unk12 = -0x28;
        }
    }
    G.Status_flg &= ~0x40;
}

void func_800439F8(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    MATRIX *pM;
    SVECTOR Sv;
    SVECTOR Unused;
    ESP_WORK *pEsp;

    if (pEm->Move_cnt == 0) {
        pP = pEm->pSin_parts_ptr;
        func_800455B4();
        func_8006D030(1);
        func_800746C0(0x10100, (VECTOR *)pP[4].Workm.t, 0, 0);
        G.Unk75E8 |= 1;
        Pl_weapon_vib(pEm);
        pM = &pP[4].Workm;
        Sv.vx = 0xBE;
        Sv.vy = 0x318;
        Sv.vz = 0;
        func_8001B484(1, 0x28000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x1EC;
        Sv.vz = 0;
        func_8001B484(3, 0x10000000, pM, &Sv, 0);
        Sv.vx = 0xBE;
        Sv.vy = 0x4A8;
        Sv.vz = 0;
        func_8001B484(3, 0x1D000000, pM, &Sv, 0);
        Sv.vx = 0x97;
        Sv.vy = 0x15E;
        Sv.vz = 0;
        pEsp = func_8001B484(3, 0x6000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = -0x14;
        }
        pEsp = func_8001B484(3, 0x6000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x14;
            pEsp->Unk12 = -0x14;
            pEsp->Unk14 = 0x14;
        }
        pEsp = func_8001B484(3, 0xD000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = -0x1E;
        }
        pEsp = func_8001B484(3, 0xD000000, pM, &Sv, 0);
        if (pEsp != (ESP_WORK *)-1) {
            pEsp->Unk10 = -0x1E;
            pEsp->Unk12 = -0x1E;
            pEsp->Unk14 = 0x1E;
        }
        Game.Plight_flg = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80043BE4);

void Weapon_cls(PLAYER_WORK *pEm)
{
    ULONG *pTmd = Pl_wep_tmd;
    PARTS_W *pP = pEm->pSin_parts_ptr;
    ULONG n;

    pP[4].pTmd = pP[21].pTmd;
    n = pTmd[-1];
    pEm->Parts_num = pEm->Parts_num - n;
}

void Weapon_parts_clr(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pEm->pSin_parts_ptr[4].pTmd = pEm->pSin_parts_ptr[21].pTmd;
    pP = &pEm->pSin_parts_ptr[21];
    i = Pl_wep_tmd[-1];
    do {
        pP->Be_flg = 0;
        pP->UnkA2 = 0;
        pP++;
    } while (--i != 0);
}

void Pl_weapon_ch_init(PLAYER_WORK *pEm)
{
    pEm->pSin_parts_ptr[4].pTmd = Pl_wep_tmd;
    Pl_weapon_init[(UCHAR)pEm->Type](pEm);
}



void Non_init(PLAYER_WORK *pEm)
{
}

void Plw02_init(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0;
    pO->Pos_y = 0x96;
    pO->Pos_z = 0;
    pO->UnkA2 |= 0xB;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", Plw_init_43FF0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", Plw_init_44054);

void Plw_init_440AC(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 1;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0x43;
    pO->Pos_y = 0x1B9;
    pO->Pos_z = 0x44;
    pO->UnkA2 |= 0xB;
}

void Plw_init_440F4(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = 0;
    pO->Pos_y = 0x96;
    pO->Pos_z = 0;
    pO->UnkA2 |= 0xB;
}

void Plw_init_44134(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 0;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = -0x32;
    pO->Pos_y = 0x1B8;
    pO->Pos_z = 0xA;
    pO->UnkA2 |= 0xB;
}

void Plw_init_4417C(PLAYER_WORK *pEm)
{
    PARTS_W *pP = pEm->pSin_parts_ptr;
    PARTS_W *pO = &pP[22];

    pO->Be_flg = 1;
    pO->pSuper = &pP[4].Workm;
    pO->pOya_parts = &pP[4];
    pO->Pos_x = -0x1D;
    pO->Pos_y = 0x7E;
    pO->Pos_z = 0xD;
    pO->Unk96 = 0xFF;
    pO->UnkA2 |= 0xB;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800441CC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80044420);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800445C8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004470C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004479C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80044804);

void func_80044E90(MATRIX *pM0, MATRIX *pM1, SVECTOR *pOut)
{
    MATRIX M;
    VECTOR V;

    TransposeMatrix(pM0, &M);
    V.vx = pM1->t[0] - pM0->t[0];
    V.vy = pM1->t[1] - pM0->t[1];
    V.vz = pM1->t[2] - pM0->t[2];
    ApplyMatrixLV(&M, &V, &V);
    pOut->vx = V.vx;
    pOut->vy = V.vy;
    pOut->vz = V.vz;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80044F34);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80045094);

void Pl_wep_eff_init(PLAYER_WORK *pEm)
{
    Pl_wep_eff_parts_init(pEm);
    func_80046C90();
}

void func_800452E0(void) {
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800452E8);

void func_80045514(void) {
}

LONG Pl_wep_eff_parts_init(PLAYER_WORK *pEm)
{
    PARTS_W *pP;
    ULONG i;

    pP = &pEm->pSin_parts_ptr[15];
    i = 6;
    do {
        pP->Unk74 = 0xFF;
        pP->Be_flg = 0;
        pP->Unk90 = 0;
        func_8001B894(0x103, &pP->Workm);
        func_8001B894(0xC03, &pP->Workm);
        func_8001B894(0x105, &pP->Workm);
        func_8001B894(0x805, &pP->Workm);
        pP++;
    } while (--i != 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800455B4);

UCHAR Pl_wep_eff_parts_cnt(void)
{
    LONG i;
    UCHAR Cnt;
    PARTS_W *pP;
    UCHAR Unused[16];

    i = 6;
    Cnt = 0;
    pP = &G.Pl_pSin_parts_ptr[15];
    do {
        if (pP->Unk74 != 0xFF) {
            Cnt++;
        }
        pP++;
    } while (--i != 0);
    return Cnt;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800457A4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80045950);

ULONG func_80046A0C(ENEMY_WORK *pEm, MATRIX *pM, UCHAR *pTbl)
{
    ULONG Min;
    ULONG Ret;
    ULONG d;
    LONG dy;
    LONG i;
    ULONG j;
    UCHAR *p;
    PARTS_W *pP;
    LONG sq;

    Min = 0x7FFFFFFF;
    if (pEm->Id < 0x10) {
        return 0;
    }
    i = 4;
    j = 0;
    p = &pTbl[pEm->Id * 4 - 0x40];
    do {
        pP = &pEm->pSin_parts_ptr[*p];
        sq = (pP->Workm.t[0] - pM->t[0]) * (pP->Workm.t[0] - pM->t[0]) + (pP->Workm.t[2] - pM->t[2]) * (pP->Workm.t[2] - pM->t[2]);
        dy = pP->Workm.t[1] - pM->t[1];
        d = dy < 0 ? -dy : dy;
        d += SquareRoot0(sq);
        if (d < Min) {
            Min = d;
            Ret = *p | (j << 16);
        }
        p++;
        j++;
    } while (--i != 0);
    return Ret;
}

LONG Dir_limit(LONG Dir, LONG Base, LONG Lim, SHORT *pDir)
{
    LONG d;
    SHORT Out;

    d = (Lim + (Dir - Base)) & 0xFFF;
    Base -= Lim;
    if (d < (SHORT)Lim * 2) {
        *pDir = Dir;
        return 0;
    }
    Out = Base;
    if ((ULONG)d <= 0x800) {
        Out = Base + Lim * 2;
    }
    *pDir = Out;
    return 1;
}

void func_80046B94(SVECTOR *pV, SVECTOR *pOut)
{
    SHORT Dir_y;
    SHORT Dir_z;

    Dir_y = Get_dir_xz(pV->vx, pV->vz);
    Dir_z = (Get_dir_xz(SquareRoot0(pV->vx * pV->vx + pV->vz * pV->vz), -pV->vy) - 0x400) & 0xFFF;
    pOut->vx = 0;
    pOut->vy = Dir_y;
    pOut->vz = Dir_z;
}

SHORT Get_dir_xz(LONG X, LONG Z)
{
    LONG Dir;
    LONG Base;

    Dir = 0x400;
    if (Z > 0) {
        Dir = 0xC00;
    }
    if (X != 0) {
        Dir = catan((Z << 12) / X);
        if (X < 0) {
            return (0x800 - Dir) & 0xFFF;
        }
        return (0x1000 - Dir) & 0xFFF;
    }
    return Dir;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046C90);

void Pl_wep_poly_set(PL_WEP_POLY *pWp, SHORT Unk2, LONG Unk4)
{
    pWp->Be_flg = 1;
    pWp->Unk4 = Unk4;
    pWp->Unk2 = Unk2;
}

void Pl_wep_poly_rgb(PL_WEP_POLY *pWp, ULONG Rgb)
{
    *(ULONG *)&pWp->Poly[0].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[1].r0 &= 0xFF000000;
    *(ULONG *)&pWp->Poly[0].r0 |= Rgb;
    *(ULONG *)&pWp->Poly[1].r0 |= Rgb;
}


void func_80046DF8(PL_WEP_POLY *pWp)
{
    pWp->Be_flg = 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046E00);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80046EC4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800470F4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800472EC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047860);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047BD0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047DD0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80047F70);

void Quake_add(LONG X, LONG Y)
{
    G.Unk7860 += Y;
    G.Unk2118 += Y;
    G.Unk211A += X;
    if (G.Unk2116 == 0) {
        G.Unk2116 += 0x5A;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004834C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048520);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048630);

extern LONG D_800D204C[];

void func_800486C8(void)
{
    LONG *p = D_800D204C;
    LONG i = 32;

    do {
        *p++ = -1;
    } while (--i != 0);
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800486F0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048844);

typedef struct {
    ULONG Tag;                         /* 0x00 */
    ULONG Tpage;                       /* 0x04 */
    ULONG pad8;
    ULONG Code;                        /* 0x0C */
    UCHAR pad10[0x1C - 0x10];
} PRIM_1C;
typedef struct {
    UCHAR *p0;                         /* 0x00 */
    UCHAR *p1;                         /* 0x04 */
    PRIM_1C *pPrim;                    /* 0x08 */
    UCHAR padC[0x16 - 0xC];
    SHORT Unk16;                       /* 0x16 */
    UCHAR pad18[0x20 - 0x18];
    UCHAR Unk20;                       /* 0x20 */
    UCHAR Unk21;                       /* 0x21 */
} W_DEB18;
extern W_DEB18 D_800DEB18;
void func_80010150(void *p, ULONG Size);
void func_80048FF4(void);

void func_80048974(SHORT Arg)
{
    UCHAR *p;
    PRIM_1C *pPr;
    LONG i;
    W_DEB18 *pW = &D_800DEB18;

    p = (UCHAR *)G.pEm_mem;
    D_800DEB18.p0 = p;
    p += 0x64;
    pW->p1 = p;
    p += 0x4B0;
    pW->pPrim = (PRIM_1C *)p;
    p += 0x1900;
    func_80010150(D_800DEB18.p0, p - D_800DEB18.p0);
    i = 200;
    G.pEm_mem = (struct tagENEMY_WORK *)p;
    pPr = pW->pPrim;
    do {
        pPr->Tpage = 0xE1000420;
        pPr->Code = 0x52000000;
        pPr++;
    } while (--i != 0);
    pW->Unk20 = 0;
    pW->Unk21 = 0;
    pW->Unk16 = Arg;
    func_80048FF4();
    G.Status_flg |= 0x1000;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048A3C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048E34);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80048FF4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049218);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800493EC);

void func_80049898(void)
{
    ULONG Size = ((ULONG (*)(ULONG, ULONG))Task_lexecute)(2, G.Unk783E + 0x11);

    G.Unk2138 = ((Size + 3) & ~3) + 0x8011A000;
    Task_sleep(1);
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_800498F0);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049A50);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049BA8);

ULONG func_800102E8(void);
extern USHORT **D_800CC86C;

LONG func_80049E80(void)
{
    USHORT n = *D_800CC86C[14];

    if (n == 0) {
        return 0xFF;
    }
    return (UCHAR)(func_800102E8() % n);
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_80049ED8);

ULONG func_80049F80(SHORT *pA, SHORT *pB)
{
    LONG dx1, dy2, dy1, dx2;
    LONG Den, dy, dx, S, T;

    dx1 = pA[2] - pA[0];
    dy2 = pB[3] - pB[1];
    dy1 = pA[3] - pA[1];
    dx2 = pB[2] - pB[0];
    Den = dx1 * dy2 - dy1 * dx2;
    if (Den == 0) {
        return 0;
    }
    dy = pA[1] - pB[1];
    dx = pA[0] - pB[0];
    S = dy * dx2 - dx * dy2;
    T = dy * dx1 - dx * dy1;
    if (((Den > 0 && S >= 0 && S <= Den) || (Den < 0 && S >= Den && S <= 0))
        && ((Den > 0 && T >= 0 && T <= Den) || (Den < 0 && T >= Den && T <= 0))) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004A080);

void func_8004A7F4(VECTOR *pPos, SVECTOR *pT, LONG Spd, SVECTOR *pOut)
{
    LONG dx;
    LONG dz;
    LONG d;

    dz = (pT->vz - pPos->vz) >> 1;
    dx = (pT->vx - pPos->vx) >> 1;
    d = dz * dz + dx * dx;
    dz <<= 8;
    d = SquareRoot0(d) + 1;
    dz = dz / d;
    dx <<= 8;
    dx = dx / d;
    dz *= Spd;
    dx *= Spd;
    pOut->vx = dz >> 8;
    pOut->vz = dx >> 8;
}

void func_8004A8BC(ENEMY_WORK *pEm, ULONG *pData)
{
    PARTS_W *pP;
    ULONG *pHead;
    ULONG n;
    ULONG i;

    pEm->pUnk118 = pData;
    pHead = pData;
    if (*pData != 0) {
        i = 0;
        pData += 3;
        pP = pEm->pSin_parts_ptr;
        n = pEm->pTmd[-1];
        do {
            pP->pUnk98 = NULL;
            if (*pHead & (1 << i)) {
                pP->pUnk98 = pData;
                pData += pData[2] + 3;
            }
            pP++;
            i++;
        } while (--n != 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004A934);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004AC54);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004AD94);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004AF04);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004B374);

extern SHORT D_800DEB60;

void func_8004B7E8(ENEMY_WORK *pEm, LONG Arg1, LONG Y)
{
    pEm->Unk122 = Y;
    pEm->nFloor = Y / -1800;
    if (!(pEm->Be_flg & 0x400)) {
        pEm->Base_pos_y = pEm->Pos_y = pEm->Unk122;
    }
    if (D_800DEB60 != 0) {
        pEm->Pos_y += pEm->Unk1B8;
        if (pEm->Unk122 < pEm->Pos_y) {
            pEm->Pos_y = pEm->Unk122;
        }
    } else {
        pEm->Unk1B8 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004B874);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004BB4C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C408);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C57C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C6EC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C85C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004C960);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004CC54);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004CE2C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D194);

ULONG Sca_ck_circle(VECTOR *pPos, LONG R, SCA_DATA *pT)
{
    LONG Half;
    LONG Cx;
    LONG Cz;
    LONG Dx;
    LONG Dz;

    Half = (pT->X1 - pT->X0) >> 1;
    Cx = pT->X0 + Half;
    Cz = pT->Z0 + Half;
    Half += R;
    Dx = pPos->vx - Cx;
    Dz = pPos->vz - Cz;
    if ((Dx != 0 || Dz != 0) && SquareRoot0(Dx * Dx + Dz * Dz) < Half) {
        return 2;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D404);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D590);

LONG func_8004D6B0(void)
{
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D6B8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004D720);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004DB10);

ULONG func_8004DDC4(VECTOR *pPos, LONG Arg1, UCHAR *pTbl, UCHAR *pOut)
{
    USHORT *p;
    LONG x;
    LONG z;

    x = pPos->vx + 0x8000;
    z = pPos->vz + 0x8000;
    x >>= 12;
    z >>= 8;
    p = (USHORT *)pTbl + ((z & 0xFFF0) + x);
    pTbl += 0x200;
    if (*p == 0xFFFF) {
        return 0;
    }
    pTbl += *p;
    do {
        *pOut++ = *pTbl++;
    } while (*pTbl != 0xFF);
    *pOut = 0xFF;
    return 1;
}

ULONG Sca_ck_circle2(VECTOR *pPos, LONG R, SCA_DATA *pT)
{
    LONG Half;
    LONG Dx;
    LONG Dz;
    LONG Lim;

    Half = (pT->X1 - pT->X0) >> 1;
    Dx = pPos->vx - (pT->X0 + Half);
    Dz = pPos->vz - (pT->Z0 + Half);
    Lim = Half + R;
    return SquareRoot0(Dx * Dx + Dz * Dz) < Lim;
}

LONG func_8004DEA8(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High);

LONG func_8004DEA8(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    return High;
}

LONG Sca_high_box(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    ULONG Type = pT->Type;

    if (!(Flg & 8) || !(Type & 0xF00)) {
        High = pT->High;
    }
    return High;
}

LONG Sca_high_circle(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    if (Sca_ck_circle(pPos, 0, pT)) {
        High = pT->High;
    }
    return High;
}




INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004DF24);

LONG Sca_high_naname_a(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X0;
    LONG Z = pT->Z1;

    if ((pPos->vx - X) * (pT->Z0 - Z) / (pT->X1 - X) < pPos->vz - Z) {
        return pT->High;
    }
    return High;
}

LONG Sca_high_naname_b(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X1;
    LONG Z = pT->Z1;

    if ((pPos->vx - X) * (pT->Z0 - Z) / (pT->X0 - X) < pPos->vz - Z) {
        return pT->High;
    }
    return High;
}

LONG Sca_high_naname_c(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X0;
    LONG Z;

    Z = pT->Z1 - pT->Z0;
    if (pPos->vz - pT->Z0 < (pPos->vx - X) * Z / (pT->X1 - X)) {
        return pT->High;
    }
    return High;
}

LONG Sca_high_naname_d(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X = pT->X1;
    LONG Z1 = pT->Z1;
    LONG Z = pT->Z0;

    if (pPos->vz - Z < (pPos->vx - X) * (Z - Z1) / (X - pT->X0)) {
        return pT->High;
    }
    return High;
}



LONG func_8004E10C(SCA_DATA *pT, VECTOR *pPos, LONG High)
{
    LONG X;
    LONG Z;
    LONG Lo;
    LONG Hi;
    LONG Range;

    X = pPos->vx;
    Z = pPos->vz;
    Lo = -(*(signed char *)&pT->Floor * 1800);
    Hi = pT->High + 1800;
    Range = Hi - Lo;
    switch ((pT->Id >> 4) & 3) {
    case 0:
        High = (X - pT->X0) * Range;
        High /= (pT->X1 - pT->X0);
        break;
    case 1:
        High = (pT->X1 - X) * Range;
        High /= (pT->X1 - pT->X0);
        break;
    case 2:
        High = (Z - pT->Z0) * Range;
        High /= (pT->Z1 - pT->Z0);
        break;
    case 3:
        High = (pT->Z1 - Z) * Range;
        High /= (pT->Z1 - pT->Z0);
        break;
    }
    High += Lo;
    if (High < Hi) {
        High = Hi;
    }
    if (Lo < High) {
        High = Lo;
    }
    return High;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E230);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E38C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E4C4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E5FC);

LONG func_8004E750(SCA_DATA *pT, VECTOR *pPos, ULONG Flg, LONG High)
{
    LONG R;
    LONG dx;
    LONG dz;
    LONG d;
    USHORT h;
    SHORT Sl;

    R = (pT->X1 - pT->X0) >> 1;
    dx = pPos->vx - (pT->X0 + R);
    dz = pPos->vz - (pT->Z0 + R);
    h = pT->High;
    Sl = h + *(signed char *)&pT->Floor * 1800;
    d = SquareRoot0(dx * dx + dz * dz);
    if (R - d > 0) {
        return (SHORT)h - Sl * d / R;
    }
    return High;
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004E830);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004ED4C);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004EF74);

LONG func_8004F02C(void)
{
    return 0;
}

LONG func_8004F034(ULONG P0, ULONG P1, SCA_DATA *pT)
{
    UCHAR Unused[0x20];

    return func_8004EF74(P0, P1, ((pT->X0 >> 1) << 16) | ((pT->Z0 >> 1) & 0xFFFF), ((pT->X1 >> 1) << 16) | ((pT->Z1 >> 1) & 0xFFFF));
}

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F098);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F258);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F3C8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F498);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F5B4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F6CC);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F7A8);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004F928);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004FBD4);

INCLUDE_ASM("asm/nonmatchings/game/800402F4", func_8004FE70);
