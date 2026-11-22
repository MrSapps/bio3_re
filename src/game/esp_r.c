#include "common.h"
#include "main.h"
#include "mojidisp.h"
#include "monitor.h"
#include "pad.h"
#include "trans.h"
#include "line_scr.h"
#include "em_sub.h"
#include "pl_wep.h"
#include "sub.h"
#include "joint.h"
#include "sca.h"
#include "cd.h"
#include "sce0.h"
#include "md_sub.h"
#include "esp.h"
#include "esp_r.h"
#include "player.h"
#include "sce_at.h"
#include "sld.h"
#include "snd.h"

/* GTE inline macros (PsyQ inline_c.h with the DMPSX-converted opcodes) */
#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stsv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 2(%0); sh $14, 4(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_gpf12_b() __asm__ volatile(".word 0x4B98003D")
#define gte_gpl12() __asm__ volatile("nop; nop; .word 0x4BA8003E")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_ldsxy3(r0, r1, r2) __asm__ volatile("mtc2 %0, $12; mtc2 %2, $14; mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldsz3(r0, r1, r2) __asm__ volatile("mtc2 %0, $17; mtc2 %1, $18; mtc2 %2, $19" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile("swc2 $12, 8(%0); swc2 $13, 20(%0); swc2 $14, 32(%0)" : : "r"(r0) : "memory")
#define gte_nclip_b() __asm__ volatile(".word 0x4B400006")
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_avsz3() __asm__ volatile("nop; nop; .word 0x4B58002D")
#define gte_avsz4() __asm__ volatile("nop; nop; .word 0x4B68002E")
#define gte_rtpt() __asm__ volatile("nop; nop; .word 0x4A280030")
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetLightMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $8; ctc2 $13, $9; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $10; ctc2 $13, $11; ctc2 $14, $12" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldclmv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_stclmv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_rtir() __asm__ volatile("nop; nop; .word 0x4A49E012")
#define gte_MulMatrix0(r1, r2, r3) { gte_SetRotMatrix(r1); gte_ldclmv(r2); gte_rtir(); gte_stclmv(r3); gte_ldclmv((char *)(r2) + 2); gte_rtir(); gte_stclmv((char *)(r3) + 2); gte_ldclmv((char *)(r2) + 4); gte_rtir(); gte_stclmv((char *)(r3) + 4); }
#define gte_ldlvl(r0) __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0)" : : "r"(r0))
#define gte_stlvl(r0) __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(r0) : "memory")

void func_80020298(ESP_WORK *pEsp)
{
    ESP_WORK *pNew;
    SSEQUENCE *pAnm;
    SHORT Size;

    switch (pEsp->Routine_1) {
    case 0:
        pEsp->UnkB4 = (D_80098728[func_800102E8()] & 1) + 2;
        pEsp->UnkB6 = 0;
        pEsp->UnkB8 = 0;
        pEsp->Routine_1++;
    case 1:
        if (pEsp->UnkB8 >= pEsp->UnkB6) {
            Size = (pEsp->Unk2E << 1) - (pEsp->UnkB6 - 1) * 0x100;
            if (Size < 0) {
                Size = 1;
            }
            pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0x800, (Size << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, 0);
            if (pNew != (ESP_WORK *)-1) {
                pNew->Spd_x = pEsp->Spd_x / 3;
                pNew->Spd_y = pEsp->Spd_y / 3;
                pNew->Spd_z = pEsp->Spd_z / 3;
            }
            pEsp->UnkB8 = 0;
            pEsp->UnkB6++;
        }
        pEsp->UnkB8++;
        if (pEsp->UnkB4 < pEsp->UnkB6) {
            pEsp->Routine_1++;
        }
        break;
    }
    if (pEsp->Flg2 & 0x30) {
        pEsp->Seq_no = pEsp->Unk18;
        pAnm = pEsp->pSeq + pEsp->Seq_no;
        pEsp->Timer = pAnm->Time;
        pEsp->Be_flg = (pEsp->Be_flg & ~0x12) | 1;
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0x900, (pEsp->Unk2E << 17) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, 0);
        pEsp->Routine_0 = 0;
    }
}

typedef struct tagESP_PRM4B8 {
    SHORT Cnt;
    SHORT Flg;
    SHORT Attr;
    SHORT Base10;
    SHORT Mul10;
    SHORT Base12;
    SHORT Mul12;
    SHORT pad;
} ESP_PRM4B8;

void func_800204B8(ESP_WORK *pEsp)
{
    ESP_PRM4B8 Tbl[4] = {
        { 15, 1, 11, 10, 2, -200, 4, 0 },
        { 25, 1, 11, 10, 2, -200, 5, 0 },
        { 5, 0, 0, 0, 0, 0, 0, 0 },
        { 6, 1, 10, 40, 3, -100, 10, 0 },
    };
    ESP_WORK *pNew;
    ESP_PRM4B8 *pT;
    UCHAR r;

    r = pEsp->Routine_1;
    if (!(pEsp->UnkB4 & 1)) {
        pT = &Tbl[r];
        if (pT->Flg != 0) {
            pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pT->Attr << 8), (pEsp->Unk2E << 16) | (D_80098728[func_800102E8()] << 4), pEsp->pMat, (VECTOR *)pEsp->Unk38, 0);
            if (pNew != (ESP_WORK *)-1) {
                pNew->Spd_x = pT->Base10 + pEsp->UnkB4 * pT->Mul10 + (D_80098728[func_800102E8()] & 0xF);
                pNew->Spd_y = pT->Base12 + pEsp->UnkB4 * pT->Mul12 + (D_80098728[func_800102E8()] & 0x1F);
            }
        }
    }
    if (Tbl[r].Cnt < ++pEsp->UnkB4) {
        pEsp->Routine_1++;
        if (r != 0) {
            pEsp->UnkB4 = 0;
        }
    }
    if (pEsp->Routine_1 == 4) {
        pEsp->Be_flg = 0;
    }
}

void func_80020700(ESP_WORK *pEsp)
{
    ESP_WORK *pNew;
    ULONG i;

    for (i = 0; i < 3; i++) {
        pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pEsp->Snd_no << 8) | pEsp->Unk18, (pEsp->Unk2E << 16) | (D_80098728[func_800102E8()] << 4), pEsp->pMat, (VECTOR *)pEsp->Unk38, 0);
        if (pNew != (ESP_WORK *)-1) {
            pNew->Spd_x = (D_80098728[func_800102E8()] & 0x3F) + 0x1E;
            pNew->Spd_y = (D_80098728[func_800102E8()] & 0x7F) - 0x8C;
        }
    }
    if (++pEsp->UnkB4 >= 3) {
        pEsp->Be_flg = 0;
    }
}

void func_80020820(ESP_WORK *pEsp)
{
    SHORT Tbl[3][4] = { { 2, 0, 50, -50 }, { 4, 0xE00, 60, -80 }, { 2, 0xA00, 70, -100 } };
    ESP_WORK *pNew;
    ULONG n;
    ULONG i;

    n = pEsp->Routine_1;
    pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xC00, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + (signed char)D_80098728[func_800102E8()] * Tbl[n][0] + Tbl[n][1]), pEsp->pMat, (VECTOR *)pEsp->Unk38, 0);
    if (pNew != (ESP_WORK *)-1) {
        pNew->Spd_x = Tbl[n][2];
        pNew->Spd_y = Tbl[n][3] + (D_80098728[func_800102E8()] & 0xF);
        if (pNew->Unk18 != 0) {
            pNew->Routine_0 = 0;
            pNew->Be_flg &= ~0x800;
        }
        for (i = 0; i < pEsp->Snd_no; i++) {
            func_8001BC80(pNew);
        }
    }
    if (++pEsp->Routine_1 == 3) {
        pEsp->Be_flg = 0;
    }
}

void func_800209D8(ESP_WORK *pEsp)
{
    ESP_WORK *pNew;

    switch (pEsp->Routine_1) {
    case 0:
        pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xC00, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + ((signed char)D_80098728[func_800102E8()] * 2 + 0x800)), pEsp->pMat, (VECTOR *)pEsp->Unk38, 0);
        if (pNew != (ESP_WORK *)-1) {
            pNew->Spd_x = (D_80098728[func_800102E8()] & 0x3F) + 0x32;
            pNew->Spd_y = (D_80098728[func_800102E8()] & 0x1F) - 0x1E;
            if (pNew->Unk18 != 0) {
                pNew->Routine_0 = 0;
                pNew->Be_flg &= ~0x800;
            }
        }
        pEsp->Routine_1++;
    case 1:
        pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xC00, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + 0x800), pEsp->pMat, (VECTOR *)pEsp->Unk38, 0);
        if (pNew != (ESP_WORK *)-1) {
            pNew->Spd_x = 0xB4;
            pNew->Acc_y = 0;
            if (pNew->Unk18 != 0) {
                pNew->Routine_0 = 0;
                pNew->Be_flg &= ~0x800;
            }
        }
        pEsp->Be_flg = 0;
        break;
    }
}

void func_80020B6C(ESP_WORK *pEsp)
{
    if (pEsp->UnkBA < 0x14) {
        pEsp->UnkB8 += 0x50;
        pEsp->Unk0A += pEsp->UnkB8;
        pEsp->UnkBA++;
    }
    if (pEsp->Flg2 & 0x30) {
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0x139, pEsp->Unk2E << 16, &GsIDMATRIX, &pEsp->Pos, 0);
        pEsp->Be_flg = 0;
    }
}

void func_80020C0C(ESP_WORK *pEsp)
{
    SHORT y;

    switch (pEsp->Routine_1) {
    case 0:
        pEsp->Acc_y = 1;
        pEsp->Spd_x = 0;
        pEsp->UnkB4 = 1;
        pEsp->Unk2C = D_80098728[func_800102E8()] << 4;
        pEsp->Routine_1++;
        func_8001CDA4(pEsp, pEsp->Unk18);
        if (pEsp->Unk1C == 0) {
            pEsp->Unk1C = 5;
        }
        if (pEsp->Unk1E == 0) {
            pEsp->Unk1E = 100;
        }
        break;
    case 1:
        if (pEsp->UnkB4 != 0) {
            y = pEsp->Spd_x += pEsp->Unk1C;
            if (y > (D_80098728[func_800102E8()] % (pEsp->Unk1E / 2)) + pEsp->Unk1E) {
                pEsp->UnkB4 = 0;
            }
        } else {
            pEsp->Spd_x -= pEsp->Unk1C;
            if (pEsp->Spd_x < 0) {
                pEsp->UnkB4 = 1;
                pEsp->Unk2C += (D_80098728[func_800102E8()] << 1) + 0x800;
                pEsp->Unk30 = 0;
                pEsp->Unk32 = 0;
                pEsp->Unk34 = 0;
                pEsp->Unk38[0] = pEsp->Pos.vx;
                pEsp->Unk38[1] = pEsp->Pos.vy;
                pEsp->Unk38[2] = pEsp->Pos.vz;
            }
        }
        if (pEsp->Flg2 & 0x30) {
            pEsp->Be_flg = 0;
        }
        break;
    }
}

void func_80020DBC(ESP_WORK *pEsp)
{
    UCHAR Tbl[5] = { 4, 5, 0, 0, 7 };
    SVECTOR Dir;

    if (pEsp->Flg2 & 0x30) {
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xD00, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, 0);
        Dir.vx = 0x400;
        Dir.vy = 0;
        Dir.vz = 0;
        func_8001B35C(pEsp, ((Tbl[pEsp->UnkB3] & 0xF) << 24) | 0xA05, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, &Dir);
        pEsp->Be_flg = 0;
    }
}

LONG Dir_ck_l(LONG Dir0, LONG Dir1) __asm__("Dir_ck"); /* Dir_ck with its result used as int */

/* effect hitting the player or the sub player (HIT_REQ / HIT_AREA tests) */
void func_80020EB8(ESP_WORK *pEsp)
{
    HIT_REQ Hit;
    VECTOR Ofs;
    SVECTOR Dir;
    HIT_AREA Area;
    LONG Hit_flg;
    USHORT Rot;
    LONG y;
    LONG d;

    Hit_flg = 0;
    if (pEsp->Routine_1 == 0) {
        switch (pEsp->Unk22) {
        case 2:
            pEsp->UnkB8 = pEsp->Unk2C;
            pEsp->Unk2C = 0;
            pEsp->Flg2 |= 0x100;
            break;
        case 4:
            pEsp->Unk22 = 3;
            pEsp->Flg2 |= 0x100;
            break;
        }
        func_8001CE24(pEsp, pEsp->Unk1A, pEsp->Unk1B);
        pEsp->Routine_1++;
    }
    if (pEsp->Unk22 == 2) {
        Rot = pEsp->UnkB8;
    } else {
        Rot = pEsp->Unk2C;
    }
    if (pEsp->UnkB4 >= 3 && (pEsp->Flg2 & 0x100)) {
        Hit.pPos = &pEsp->Pos;
        Hit.W = 0;
        Hit.H = 0;
        Hit.D = 0;
        Hit.R = Rot;
        Hit_flg |= func_800472EC(&Hit, D_80097FCC[pEsp->Unk18]);
        if (Hit_flg != 0 && pEsp->Unk22 == 1) {
            G.Pl_status_flg |= 0x200;
        }
    } else {
        pEsp->UnkB4++;
    }
    if (Hit_flg != 0) {
        if (Hit_flg < 0) {
            d = Dir_ck_l((SHORT)Rot, G.Pl_cdir_y);
            if (G.Pl_routine_0 == 2) {
                G.Pl_routine_1 = ((UCHAR *)D_80097FCC[pEsp->Unk18])[1] * 2 - 0x28 + d;
            }
        }
        if (Hit_flg & 0x40000000) {
            d = Dir_ck_l((SHORT)Rot, G.pEnemy[1]->Cdir_y);
            if (G.pEnemy[1]->Routine_0 == 2) {
                G.pEnemy[1]->Routine_1 = ((UCHAR *)D_80097FCC[pEsp->Unk18])[1] * 2 - 0x28 + d;
            }
        }
    }
    if (pEsp->Unk22 == 2) {
        if (pEsp->UnkB4 >= 3) {
            y = func_8004D720(&pEsp->Pos, 2, 0x2000, 4);
            if (y < pEsp->Pos.vy) {
                if (y < pEsp->Pos.pad) {
                    pEsp->Flg2 |= 0x20;
                } else {
                    pEsp->Flg2 |= 0x10;
                }
            } else {
                pEsp->Pos.pad = y;
            }
        }
        Ofs.vx = (signed char)D_80098728[func_800102E8()] >> 1;
        Ofs.vy = 0;
        Ofs.vz = (signed char)D_80098728[func_800102E8()] >> 1;
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (USHORT)(pEsp->Id.B[0] | 0x500),
                      (pEsp->Unk2E + (signed char)func_800102E8()) << 16, &pEsp->Workm2, &Ofs, 0);
        Area.pPos = &pEsp->Pos;
        Area.pBox = D_80098074;
        Area.Dir = Rot;
        Area.Id = 0x42;
        func_800445C8(&pEsp->Pos, &pEsp->pEm->Pos_x, &Area, &Pl_work);
        Area.pPos = &pEsp->Pos;
        Area.pBox = D_8009807C;
        Area.Dir = Rot;
        Area.Id = 0x41;
        func_800445C8(&pEsp->Pos, &pEsp->pEm->Pos_x, &Area, &Pl_work);
    } else {
        if (pEsp->Flg2 & 4) {
            pEsp->UnkBA = 1;
        }
        if (pEsp->UnkBA == 0) {
            pEsp->Unk2E += ((SHORT)pEsp->Unk1C * (SHORT)pEsp->Unk2E) >> 12;
            pEsp->Unk1C -= pEsp->Unk1E;
        }
    }
    if ((pEsp->Flg2 & 0x30) || Hit_flg != 0) {
        switch (pEsp->Unk22) {
        case 2:
            func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (USHORT)(pEsp->Id.B[0] | 0x600), (pEsp->Unk2E << 16) | Rot,
                          &GsIDMATRIX, &pEsp->Pos, 0);
        case 3:
            func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (USHORT)(pEsp->Id.B[0] | 0x1100), (pEsp->Unk2E << 16) | Rot,
                          &GsIDMATRIX, &pEsp->Pos, 0);
            if (pEsp->Flg2 & 0x10) {
                Dir.vx = 0x400;
                Dir.vy = 0;
                Dir.vz = 0;
                if ((pEsp->Flg2 & 0x100) && (func_800102E8() & 3) == 0) {
                    func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | ((func_800102E8() & 1) << 8) | 0x3E,
                                  (((pEsp->Unk2E >> 2) + ((signed char)func_800102E8() << 2)) << 16) | Rot,
                                  &GsIDMATRIX, &pEsp->Pos, &Dir);
                } else {
                    func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (((func_800102E8() & 1) + 2) << 8) | 0x3E,
                                  (((pEsp->Unk2E >> 2) + ((signed char)func_800102E8() << 2)) << 16) | Rot,
                                  &GsIDMATRIX, &pEsp->Pos, &Dir);
                }
            }
            break;
        }
        Snd_se_enem(pEsp->pEm->Id == 0x28 ? 0xC : pEsp->Snd_no, pEsp->pEm);
        func_8001B35C(pEsp, ((pEsp->Unk20 & 0xFF) << 8) | (pEsp->Unk20 >> 8), (pEsp->Unk2E << 16) | Rot, &GsIDMATRIX,
                      &pEsp->Pos, 0);
        pEsp->Be_flg = 0;
    }
}

void func_80021548(ESP_WORK *pEsp)
{
    HIT_REQ Hit;
    VECTOR v;
    LONG r1;
    LONG a;
    LONG r2;

    pEsp->Unk2E += pEsp->Unk1C;
    if (pEsp->Id.B[1] < 2) {
        if ((SHORT)(pEsp->UnkB4 % 3) == 0) {
            r1 = (signed char)func_800102E8() << (pEsp->UnkB4 / (100 - pEsp->Id.B[1] * 30) + 1);
            r2 = (signed char)func_800102E8() << (pEsp->UnkB4 / (100 - pEsp->Id.B[1] * 30) + 1);
            v.vx = pEsp->Pos.vx + r1;
            v.vy = pEsp->Pos.vy;
            v.vz = pEsp->Pos.vz + r2;
            a = ((func_800102E8() & 1) << 8) | 0x2D;
            func_8001B35C(pEsp, a, (((signed char)func_800102E8() << 1) + 0xC00) << 16, &GsIDMATRIX, &v, 0);
        }
        if (pEsp->UnkB4 >= 4 && pEsp->UnkB6 == 0) {
            Hit.pPos = &pEsp->Pos;
            Hit.W = 0;
            Hit.H = 0;
            Hit.D = 0;
            Hit.R = pEsp->Unk2C;
            if (func_800472EC(&Hit, D_80097FCC[pEsp->Unk18]) < 0) {
                pEsp->UnkB6 = 1;
                func_8001B35C(pEsp, 0x1503, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, 0);
            }
        }
    }
    pEsp->UnkB4++;
}

void func_8002174C(ESP_WORK *pEsp)
{
    if (pEsp->UnkB4 >= 3) {
        pEsp->Routine_0 = 0;
        pEsp->Be_flg &= ~0x800;
    }
    pEsp->UnkB4++;
}

void func_80021784(void) {
}

void func_8002178C(void) {
}

void func_80021794(void) {
}

void func_8002179C(void) {
}

void func_800217A4(void) {
}

void func_800217AC(void) {
}

void func_800217B4(ESP_WORK *pEsp)
{
    switch (pEsp->Routine_1) {
    case 0:
        func_8001B35C(pEsp, (((pEsp->UnkB3 + 1) & 0xF) << 24) | 0x905, (pEsp->Unk2E << 16) | pEsp->Unk2C, pEsp->pMat, (VECTOR *)pEsp->Unk38, 0);
        pEsp->UnkB6 = (UCHAR)(D_80098728[func_800102E8()] % 50) + 40;
        pEsp->UnkB4 = 0;
        pEsp->Routine_1++;
        break;
    case 1:
        if (pEsp->UnkB6 < ++pEsp->UnkB4) {
            pEsp->Routine_1 = 0;
        }
        break;
    }
}

void func_800218BC(ESP_WORK *pEsp)
{
    ULONG Cnt;

    if (pEsp->Flg2 & 0x30) {
        pEsp->Be_flg = 0;
    }
    pEsp->Unk2E -= 0x50;
    if (pEsp->Unk2E < 0x100) {
        pEsp->Be_flg = 0;
    }
    Cnt = (USHORT)pEsp->UnkB4;
    if ((Cnt & 1) && Cnt - 2 < 0x17) {
        func_8001B35C(pEsp, 0x1000003, (pEsp->Unk2E << 17) | (USHORT)((signed char)func_800102E8() << 4), &GsIDMATRIX, &pEsp->Pos, 0);
    }
    pEsp->UnkB4++;
}

void func_8002198C(ESP_WORK *pEsp)
{
    VECTOR Pos;
    HIT_REQ Hit;
    ESP_TBL16 *pTbl;
    ESP_WORK *pNew;
    LONG r0;
    LONG r1;
    ULONG Ofs;

    switch (pEsp->Unk18) {
    case 0:
        pTbl = &D_80098094[pEsp->UnkB4];
        break;
    case 1:
        pTbl = &D_800982C4[pEsp->UnkB4];
        break;
    case 2:
        pTbl = &D_80098494[pEsp->UnkB4];
        break;
    }
    if (pEsp->Routine_1 < 11 && pEsp->Snd_no != 0xFF) {
        Hit.pPos = &pEsp->Pos;
        Hit.W = 0;
        Hit.H = 0;
        Hit.D = 0;
        Hit.R = 0;
        func_800472EC(&Hit, D_80097FCC[pEsp->Snd_no]);
        func_80047BD0(&Hit, D_80097FCC[pEsp->Snd_no]);
    }
    if (pEsp->Routine_1 == 0) {
        Pos.vx = 0;
        Pos.vy = 500;
        Pos.vz = 0;
        func_8001B35C(pEsp, 0xB0C, pEsp->Unk2E << 16, &pEsp->Workm2, &Pos, 0);
    }
    if (pTbl->Id != 0xFF) {
        do {
            if (pTbl->Id != 0) {
                Pos.vx = 0;
                Pos.vy = (pTbl->Size * pEsp->Unk2E) >> 12;
                Pos.vz = 0;
                r0 = func_800102E8();
                r1 = func_800102E8();
                Ofs = ((pEsp->Unk2E * 4 + (signed char)r0 * 32) << 16)
                    | (USHORT)(pEsp->Unk2C + (signed char)r1 * 2 + pTbl->Ofs * 0x2AA);
                pNew = func_8001B35C(pEsp, ((pTbl->Attr & 0xF) << 24) | (pTbl->Id << 8) | 0xC, Ofs, &pEsp->Workm2, &Pos, 0);
                if (pNew != (ESP_WORK *)-1) {
                    pNew->Acc_x = (pTbl->Unk8 * pEsp->Unk2E) >> 12;
                    pNew->Acc_y = (pTbl->UnkA * pEsp->Unk2E) >> 12;
                    pNew->Spd_x = (pTbl->UnkC * pEsp->Unk2E) >> 12;
                    pNew->Spd_y = (pTbl->UnkE * pEsp->Unk2E) >> 12;
                }
            }
            pEsp->UnkB4++;
            pTbl++;
        } while (pTbl->Id != 0);
        pEsp->UnkB4++;
    }
    if (pEsp->Unk1A) {
        switch (pEsp->Routine_1) {
        case 0:
            Fade_set(0, 0, 0, 1, 0xFFFFFF, 0, 8);
            G.Status_flg |= 2;
            func_8003884C(D_80097B78, 2);
            Snd_se_stad(0x10213, &pEsp->Pos, 0, 0);
            break;
        case 8:
            G.Status_flg = (G.Status_flg & ~2) | 4;
            break;
        case 15:
            G.Status_flg &= ~4;
            break;
        }
    }
    if (++pEsp->Routine_1 == 16) {
        pEsp->Be_flg = 0;
    }
}

void func_80021D58(ESP_WORK *pEsp)
{
    ULONG Size;

    switch (pEsp->Routine_1) {
    case 0:
        func_8001CDA4(pEsp, pEsp->Unk18);
        func_8001CE24(pEsp, pEsp->Unk1A, pEsp->Unk1B);
        pEsp->UnkB6 = pEsp->Unk2E;
        pEsp->Routine_1++;
        Size = (USHORT)pEsp->UnkB6;
        Size *= D_800986C4[pEsp->UnkB4];
        pEsp->Unk2E = Size >> 12;
        pEsp->UnkB4++;
        break;
    case 1:
        Size = (USHORT)pEsp->UnkB6;
        Size *= D_800986C4[pEsp->UnkB4];
        pEsp->Unk2E = Size >> 12;
        if (++pEsp->UnkB4 >= 3) {
            pEsp->Routine_1++;
        }
        break;
    case 2:
        pEsp->Unk2E -= (ULONG)(pEsp->UnkB6 << 4) >> 12;
        break;
    }
}

void func_80021EA4(void) {
}

void func_80021EAC(ESP_WORK *pEsp)
{
    SVECTOR Dir;
    ESP_WORK *pNew;
    ULONG Hit;

    switch (pEsp->Routine_1) {
    case 0:
        func_8001CDA4(pEsp, pEsp->Unk18);
        func_8001CE24(pEsp, pEsp->Unk1A, pEsp->Unk1B);
        pEsp->Routine_1++;
        break;
    case 1:
        pEsp->Unk2E += pEsp->Unk1C;
        pEsp->Unk1C -= pEsp->Unk1E;
        Hit = 0;
        switch (pEsp->Unk22) {
        case 0:
        case 1:
            if (pEsp->Flg2 & 8) {
                Hit = 2;
            }
            if (pEsp->Flg2 & 0x30) {
                Hit = 2;
            }
            break;
        case 2:
            if (pEsp->Pos.vy >= 0) {
                Hit = 2;
            }
            break;
        case 3:
            if (pEsp->Pos.vy >= -0x5A0) {
                Hit = 2;
            }
            break;
        }
        if (pEsp->UnkB4 == 0 && Sca_ck_line(&pEsp->Pos, &pEsp->Old, 0x2000, 5)) {
            pEsp->UnkB4 = 1;
            if (D_800DEB80 == &Pl_work) {
                Hit = 1;
                func_8003884C(D_80097B78, 3);
            } else {
                Hit = 1;
            }
        }
        if (Hit) {
            if (!(D_80098728[func_800102E8()] & 1)) {
                pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | ((pEsp->Unk20 & 0xFF) << 8) | (pEsp->Unk20 >> 8),
                                     (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + (D_80098728[func_800102E8()] % 6) * 0x2AA),
                                     &GsIDMATRIX, &pEsp->Pos, 0);
                if (pNew != (ESP_WORK *)-1) {
                    pNew->UnkB6 = pEsp->UnkB6;
                }
            }
            if (Hit == 2) {
                if (!(D_80098728[func_800102E8()] & 1) && pEsp->Unk22 == 0) {
                    Dir.vx = 0x400;
                    Dir.vy = 0;
                    Dir.vz = 0;
                    func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | 0xE, ((pEsp->Unk2E + 0x500) << 16) | pEsp->Unk2C,
                                  &GsIDMATRIX, &pEsp->Pos, &Dir);
                }
                pEsp->Be_flg = 0;
                pEsp->Routine_0 = 0;
            } else if (!(D_80098728[func_800102E8()] & 3)) {
                pEsp->Be_flg = 0;
                pEsp->Routine_0 = 0;
            }
        }
        break;
    }
}

void func_800221F0(ESP_WORK *pEsp)
{
    VECTOR v;
    ESP_WORK *pNew;

    if (!(pEsp->UnkB4 & 1)) {
        v.vx = (signed char)func_800102E8();
        v.vy = (signed char)func_800102E8();
        v.vz = (signed char)func_800102E8();
        pNew = func_8001B35C(pEsp, 0x81C, ((pEsp->Unk2E + pEsp->UnkB4 * 0x300 + func_800102E8()) << 16) | pEsp->Unk2C, &pEsp->Workm2, &v, 0);
        if (pNew != (ESP_WORK *)-1) {
            pNew->Spd_y = pEsp->UnkB4 * 30 - 300;
        }
    } else {
        v.vx = (signed char)func_800102E8();
        v.vy = (signed char)func_800102E8() - pEsp->UnkB4 * 300 - 50;
        v.vz = (signed char)func_800102E8();
        pNew = func_8001B35C(pEsp, 0x81C, ((pEsp->Unk2E + 0x1300 - (pEsp->UnkB4 << 8) + func_800102E8()) << 16) | pEsp->Unk2C, &pEsp->Workm2, &v, 0);
        if (pNew != (ESP_WORK *)-1) {
            pNew->Spd_y = -30;
        }
    }
    if (pEsp->UnkB4 < 8) {
        pEsp->UnkB4++;
    } else {
        pEsp->Be_flg = 0;
    }
}

void func_80022398(void) {
}

void func_800223A0(void) {
}

void func_800223A8(ESP_WORK *pEsp)
{
    ESP_WORK *pNew;

    if (pEsp->UnkB4 < 5) {
        pNew = func_8001B35C(pEsp, 0x3001F03, ((((0x3000 - pEsp->UnkB4 * 2000) * pEsp->Unk2E) >> 12) << 16) | (USHORT)(pEsp->Unk2C + (signed char)D_80098728[func_800102E8()]), &GsIDMATRIX, &pEsp->Pos, 0);
        if (pNew != (ESP_WORK *)-1) {
            pNew->Acc_y = (pNew->Acc_y * pNew->Unk2E) >> 12;
            pNew->Spd_y = (pNew->Spd_y * pNew->Unk2E) >> 12;
            if (pEsp->Unk18 != 0) {
                pNew->Attr &= ~0x10;
                if (pEsp->Unk18 != 0) {
                    pNew->Be_flg &= ~0x10;
                }
            }
        }
    } else {
        pEsp->Be_flg = 0;
    }
    pEsp->UnkB4++;
}

void func_80022500(ESP_WORK *pEsp)
{
    SVECTOR v;
    UCHAR Flg;

    Flg = 0;
    switch (pEsp->Routine_2) {
    case 0:
        func_8001CDA4(pEsp, pEsp->Unk18);
        func_8001CE24(pEsp, pEsp->Unk1A, pEsp->Unk1B);
        pEsp->Routine_2++;
        break;
    case 1:
        pEsp->Unk2E += ((SHORT)pEsp->Unk1C * (SHORT)pEsp->Unk2E) >> 12;
        pEsp->Unk1C -= pEsp->Unk1E;
        switch (pEsp->Unk22) {
        case 0:
            if (pEsp->Pos.vy >= 0) {
                Flg = 1;
            }
            break;
        case 3:
            if (pEsp->Pos.vy >= -0x5A0) {
                Flg = 1;
            }
            break;
        }
        if (Flg) {
            if (pEsp->Unk22 == 0) {
                v.vx = 0x400;
                v.vy = 0;
                v.vz = 0;
                func_8001B35C(pEsp, 0x10E, ((pEsp->Unk2E - (pEsp->Unk2E >> 2)) << 16) | pEsp->Unk2C, &GsIDMATRIX, &pEsp->Pos, &v);
            }
            pEsp->Be_flg = 0;
            pEsp->Routine_0 = 0;
        }
        break;
    }
}

void func_80022660(void) {
}

void func_80022668(ESP_WORK *pEsp)
{
    SHORT Tbl[6][3] = { { 0, 2, 4 }, { 0xFF, 0xFF, 0xFF }, { 1, 3, 5 }, { 0xFF, 0xFF, 0xFF }, { 0, 2, 4 }, { 0xFF, 0xFF, 0xFF } };
    ESP_WORK *pNew;
    UCHAR i;
    UCHAR j;
    ULONG n;

    n = (UCHAR)pEsp->UnkB4;
    if (Tbl[n][0] != 0xFF) {
        for (i = 0; i < 3; i++) {
            pNew = func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pEsp->Snd_no << 8) | pEsp->Unk18, (pEsp->Unk2E << 16) | (USHORT)(pEsp->Unk2C + Tbl[n][i] * 0x2AA + D_80098728[func_800102E8()]), pEsp->pMat, (VECTOR *)pEsp->Unk38, 0);
            if (pEsp->Unk1A != 0 && pNew != (ESP_WORK *)-1) {
                for (j = 0; j < 6; j++) {
                    func_8001BC80(pNew);
                }
            }
        }
    }
    if (++pEsp->UnkB4 == 5) {
        pEsp->Be_flg = 0;
    }
}

void func_8002289C(ESP_WORK *pEsp)
{
    VECTOR v;
    LONG H;
    UCHAR Flg;

    Flg = 0;
    H = Water_ck(pEsp->Pos.vx, pEsp->Pos.vz);
    if (H >= pEsp->Pos.vy) {
        Flg = (H != 0);
    }
    if (G.pRoom->pSca->padC >= pEsp->Pos.vy) {
        H = G.pRoom->pSca->padC;
        Flg = 1;
    }
    if (Flg) {
        v.vx = pEsp->Pos.vx;
        v.vy = H;
        v.vz = pEsp->Pos.vz;
        func_8001B35C(pEsp, ((pEsp->UnkB3 & 0xF) << 24) | (pEsp->Snd_no << 8) | pEsp->Unk18, (pEsp->Unk2E << 16) | pEsp->Unk2C, &GsIDMATRIX, &v, 0);
        pEsp->Be_flg = 0;
    }
}
