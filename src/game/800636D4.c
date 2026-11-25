#include "sub.h"
#include "snd.h"
#include "cd.h"
#include "monitor.h"
#include "status.h"
#include "game.h"
#include "pad.h"
#include "pl_wep.h"
#include "player.h"
#include "room.h"
#include "sce2.h"
#include "sld.h"

/* item AOT record at G+0x21DC */
typedef struct tagST_AOT_ITEM {
    USHORT Id;                         /* 0x0 */
    USHORT Num;                        /* 0x2 */
    USHORT Flg;                        /* 0x4 */
    UCHAR Md1;                         /* 0x6 */
} ST_AOT_ITEM;

/* map data (St.pUnkB4) */
typedef struct tagST_MAP_INFO {
    SHORT X;                           /* 0x0 */
    SHORT Y;                           /* 0x2 */
    UCHAR Unk4;                        /* 0x4 */
    UCHAR Unk5;                        /* 0x5 */
} ST_MAP_INFO;
typedef struct tagST_MAP_ROOM {
    SHORT X0;                          /* 0x0 */
    SHORT Z0;                          /* 0x2 */
    SHORT X1;                          /* 0x4 */
    SHORT Z1;                          /* 0x6 */
} ST_MAP_ROOM;

/* MAP.MAP header: room count, then per room the part list offset and part counts */
typedef struct tagST_MAP_PARTS {
    ULONG Ofs;                         /* 0x0 */
    USHORT N0;                         /* 0x4 */
    USHORT N1;                         /* 0x6 */
} ST_MAP_PARTS;
typedef struct tagST_MAP_HDR {
    ULONG Num;                         /* 0x0 */
    ST_MAP_PARTS Room[1];               /* 0x4 */
} ST_MAP_HDR;

#define FLD(p, ofs, type) (*(type *)((UCHAR *)(p) + (ofs)))  /* unnamed struct fields */
void func_8006EBEC_p(ULONG *p) __asm__("func_8006EBEC"); /* arg 1 unsigned long* (canonical void) */
void OpenTIM_v(void) __asm__("OpenTIM"); /* differs from the canonical prototype */

/* local prototypes (this module) */
void Cd_seek_s(ULONG File_no, ULONG Mode, char *pName) __asm__("Cd_seek"); /* 3 args here (canonical 2) */
void func_800636D4(ST_WORK *pSt)
{
    ULONG i, sum, n;
    ULONG t;
    ULONG *p;

    i = 0;
    sum = 0;
    t = D_8009F26C[pSt->UnkBC];
    pSt->UnkBD = 0;
    CD.Fno = 0x1C;
    n = t - 1;
    CD.Fsize = D_8009EF90[n];
    CD.Fsector = (floc[0x1C].sec_high << 16) + floc[0x1C].sector;
    for (; i < n; i++) {
        sum += D_8009EF90[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = D_8009EED8[t - 1];
    Cd_read(0x1C, pSt->pBuf, 2, D_80010DCC);
    p = pSt->pBuf;
    D_800CCBBC = 0xA17;
    func_8006EBEC_p(p);
    func_800641D0(pSt, pSt->UnkBC, pSt->UnkBD);
    pSt->Rtn1 = 0;
    pSt->UnkBE = 0;
    pSt->UnkBF = 0;
    pSt->Rtn0++;
    pSt->Flg &= ~0x40000000;
    pSt->Flg &= ~0x80000000;
    pSt->Flg |= 0x100000;
    pSt->Flg |= 0x80000;
    pSt->Flg &= ~0x40000;
    pSt->Flg &= ~0x20000;
    func_80063E9C(pSt);
}

INCLUDE_RODATA("asm/nonmatchings/game/800636D4", D_80010DCC);

void func_80063850(ST_WORK *pSt)
{
    switch (pSt->Rtn1) {
    case 0:
        if (G.Key_trg & 0x2000) {
            goto exit;
        }
        if (G.Unk2100 & 0x2000) {
            if (pSt->UnkBD >= D_8009F2AC[pSt->UnkBC] - 1) {
                Snd_se_stad(4, 0, 0, 0);
                pSt->UnkBF = 1;
                pSt->Rtn1 = 4;
            }
        }
        if ((USHORT)G.Unk20F8 & 0x8000) {
            if (pSt->UnkBD != 0) {
                pSt->UnkBD--;
                pSt->UnkC0 = 0;
                pSt->UnkC4 = 0;
                pSt->UnkC6 = 2;
                pSt->UnkBE = 0;
                pSt->Flg &= ~0x80000;
                pSt->Flg &= ~0x40000;
                pSt->Flg &= ~0x20000;
                Snd_se_stad(8, 0, 0, 0);
                pSt->Rtn1++;
            }
        }
        if ((USHORT)G.Unk20F8 & 0x2000) {
            if (pSt->UnkBD < D_8009F2AC[pSt->UnkBC] - 1) {
                pSt->UnkBD++;
                pSt->UnkC0 = 0;
                pSt->UnkC4 = 0;
                pSt->UnkC6 = -2;
                pSt->UnkBE = 0;
                pSt->Flg &= ~0x80000;
                pSt->Flg &= ~0x40000;
                pSt->Flg &= ~0x20000;
                Snd_se_stad(8, 0, 0, 0);
                pSt->Rtn1++;
            }
        }
        if (++pSt->UnkBE >= 0x15) {
            pSt->UnkBE = -0x14;
        }
        if (pSt->UnkBE >= 0) {
            pSt->Pos[20].X = -1;
            pSt->Pos[19].X = 1;
        } else {
            pSt->Pos[20].X = 0;
            pSt->Pos[19].X = 0;
        }
        break;
    case 1:
        pSt->UnkC4 += pSt->UnkC6;
        pSt->Pos[18].X += pSt->UnkC4;
        if (++pSt->UnkC0 >= 0x12) {
            pSt->Rtn1++;
            pSt->UnkC0 = 0;
            pSt->Pos[18].X = -pSt->Pos[18].X;
            pSt->UnkC4 += pSt->UnkC6;
            pSt->UnkC6 = -pSt->UnkC6;
        }
        break;
    case 2:
        func_800641D0(pSt, pSt->UnkBC, pSt->UnkBD);
        pSt->Rtn1++;
        break;
    case 3:
        pSt->UnkC4 += pSt->UnkC6;
        pSt->Pos[18].X += pSt->UnkC4;
        if (++pSt->UnkC0 >= 0x13) {
            pSt->Rtn1 = 0;
            pSt->Pos[18].X = 0;
            if (pSt->UnkBD != 0) {
                pSt->Flg |= 0x40000;
            }
            if (pSt->UnkBD < D_8009F2AC[pSt->UnkBC] - 1) {
                pSt->Flg |= 0x80000;
            }
            if (pSt->UnkBD >= D_8009F2AC[pSt->UnkBC] - 1) {
                pSt->Flg |= 0x20000;
            }
        }
        break;
    case 4:
        if (G.Key_trg & 0x2000) {
            goto exit;
        }
        if (G.Key_trg & 0x1000) {
        exit:
            pSt->Rtn1 = 0;
            pSt->Rtn0++;
            Snd_se_stad(5, 0, 0, 0);
            break;
        }
        if (G.Unk2100 & 0x8000) {
            pSt->UnkBF = 0;
            pSt->Rtn1 = 0;
        }
        if (++pSt->UnkBE >= 0x15) {
            pSt->UnkBE = -0x14;
        }
        if (pSt->UnkBE >= 0) {
            pSt->Pos[20].X = -1;
            pSt->Pos[19].X = 1;
        } else {
            pSt->Pos[20].X = 0;
            pSt->Pos[19].X = 0;
        }
        break;
    }
}

void func_80063CAC(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    UCHAR *pA = pG->pAot_data;
    UCHAR *pD = (UCHAR *)pG->pUnk21DC;
    UCHAR n;
    ULONG f;
    ULONG *p;

    switch (pSt->Rtn1) {
    case 0:
        switch (pSt->St_type) {
        case 0:
            pSt->Rtn0 = 2;
            pSt->Rtn1 = 5;
            pSt->Rtn2 = 5;
            pSt->Unk13 = 0;
            f = pSt->Flg | 0x40000000;
            pSt->Flg = (f | 0x80000000) & ~0x100000;
            Cd_read(0x1D, pSt->pBuf, 0, "FILEI.TIM");
            p = pSt->pBuf;
            pG->Ctp.Tpage_clut = 0x917;
            Mapping_tim(p);
            break;
        case 3:
            pSt->Flg &= ~0x100000;
            *pA = 0;
            n = pD[6];
            if (pG->Unk783E < 2) {
                Flg_on(pG->Unk78D0, *(USHORT *)(pD + 4));
            } else {
                Flg_on(pG->Unk78F0, *(USHORT *)(pD + 4));
            }
            Esp_kill(0x705, &G.Ob_model[n & 0xFF].M);
            Esp_kill(0x905, 0);
            if (!(n & 0x80)) {
                G.Ob_model[n & 0xFF].Be_flg = 0;
            }
            D_800DBB5B = pD[0];
            Mess_set(0, 0x1000, 6, 0);
            pSt->Rtn1++;
            break;
        }
        break;
    case 1:
        if (!(D_800DBB58 & 0x80)) {
            pSt->Rtn0 = 0xD;
            pSt->Rtn1 = 0;
            Snd_se_stad(5, NULL, 0, 0);
        }
        break;
    }
}

void func_80063E9C(ST_WORK *pSt)
{
    ULONG f;
    UCHAR c;

    SetDrawTPage(&pSt->Tpage[0][0], 0, 0, 0x97);
    SetDrawTPage(&pSt->Tpage[1][0], 0, 0, 0x97);
    SetDrawTPage(&pSt->Tpage[0][1], 0, 0, 0x18);
    SetDrawTPage(&pSt->Tpage[1][1], 0, 0, 0x18);
    func_8006E600(pSt, (void *)0x801ADA90, D_8009F2EC, (pSt->UnkD7 << 8) | 1);
    c = pSt->UnkD7 + 1;
    func_8006E600(pSt, (void *)0x801ADAB8, D_8009F2F8, (c << 8) | 1);
    c = pSt->UnkD3 + 2;
    f = (c << 8) | 0x10001;
    func_8006E600(pSt, (void *)0x801ADAE0, D_8009F310, f);
    func_8006E600(pSt, (void *)0x801ADB08, D_8009F304, f);
    func_8006E600(pSt, (void *)0x801ADB80, D_8009F31C, (pSt->UnkD3 << 8) | 0x10001);
}

void func_80063FE4(ST_WORK *pSt)
{
    SPRT *p;
    UCHAR c;

    AddPrim(G.pOt0 + 4, pSt->Tpage[G.Cbuf_id]);
    AddPrim(G.pOt0 + 3, &pSt->Tpage[G.Cbuf_id][1]);
    AddPrim(G.pOt0 + 2, &pSt->Ot[G.Cbuf_id][16]);
    func_800642D4(pSt, (SPRT *)0x801ADA90, D_8009F2EC, 0x1101);
    if (pSt->UnkBC == 0x1B) {
        p = (SPRT *)0x801ADA90;
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ADAA4;
        }
        p->y0 = (USHORT)pSt->Pos[17].Y + 0x24;
    }
    func_800642D4(pSt, (SPRT *)0x801ADAB8, D_8009F2F8, 0x11201);
    if (pSt->Flg & 0x80000) {
        func_8006E8BC(pSt, (SPRT *)0x801ADAE0, D_8009F310, 0x101301);
    }
    if (pSt->Flg & 0x40000) {
        func_8006E8BC(pSt, (SPRT *)0x801ADB08, D_8009F304, 0x101401);
    }
    if (pSt->Flg & 0x20000) {
        p = (SPRT *)0x801ADB80;
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ADB94;
        }
        if (pSt->UnkBF == 0) {
            c = pSt->UnkD3 + 7;
        } else {
            c = pSt->UnkD3;
        }
        p->clut = ((c + 0x1E0) << 6) | 0x13;
        func_8006E8BC(pSt, (SPRT *)0x801ADB80, D_8009F31C, 0x101501);
    }
}

void func_800641D0(ST_WORK *pSt, UCHAR a, UCHAR b)
{
    ULONG i, sum, n;
    ULONG *p;

    i = 0;
    sum = 0;
    n = D_8009F26C[a] + b;
    CD.Fno = 0x1C;
    CD.Fsize = D_8009EF90[n];
    CD.Fsector = (floc[0x1C].sec_high << 16) + floc[0x1C].sector;
    for (; i < n; i++) {
        sum += D_8009EF90[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = D_8009EED8[n];
    Cd_read(0x1C, pSt->pBuf, 2, D_80010DCC);
    p = pSt->pBuf;
    D_800CCBBC = 0xB18;
    Mapping_tim(p);
}

void func_800642D4(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm)
{
    ULONG i;
    LONG ox, oy;
    UCHAR ofs = prm >> 8;
    UCHAR ot = prm >> 16;
    ULONG n = prm;

    if (G.Cbuf_id) {
        p++;
    }
    ox = pSt->Pos[ofs].X;
    oy = pSt->Pos[ofs].Y;
    for (i = 0; i < (UCHAR)n; i++, p += 2, src += 12) {
        p->x0 = *(SHORT *)(src + 8) + ox;
        p->y0 = *(SHORT *)(src + 10) + oy;
        AddPrim(&pSt->Tpage[G.Cbuf_id][ot], p);
    }
}

void func_800643E4(ST_WORK *pSt)
{
    ULONG a[2];
    pSt->Rtn0++;
    pSt->Rtn1 = 0;
    pSt->Unk22 = 0;
    pSt->Unk24 = 0;
    pSt->UnkCB = 0;
    pSt->Flg |= 0x4000000;
    pSt->Flg |= 0x20000000;
}

void func_80064424(ST_WORK *pSt)
{
    D_8009F4E4[pSt->Rtn1](pSt);
}

void func_80064460(ST_WORK *pSt)
{
    pSt->Rtn0 = 0xD;
}

void func_8006446C(ST_WORK *pSt)
{
    ULONG i;
    UCHAR id;
    LONG f;

    if ((G.Unk20FC & 0x20) | (G.Key_trg & 0x2000)) {
        pSt->Rtn0++;
        Snd_se_stad(5, 0, 0, 0);
    } else if (G.Key_trg & 0x1000) {
        if ((signed char)pSt->Unk1C >= 0) {
            id = G.pUnk7C7C->Item[(signed char)pSt->Unk1C].Id;
            for (i = 0; i < 1; i++) {
                if (id == D_8009F564[i]) {
                    Mess_set(0, 0x1000, 0xB, 0);
                    pSt->Rtn1 = 4;
                    pSt->Flg &= ~0x20000000;
                    return;
                }
            }
            pSt->Rtn1 = 1;
            Snd_se_stad(6, 0, 0, 0);
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
        }
        if ((signed char)pSt->Unk1C <= -1) {
            pSt->Rtn0++;
            Snd_se_stad(5, 0, 0, 0);
        }
    } else {
        f = 0;
        if ((G.Unk2100 & 0x1000) && (signed char)pSt->Unk1C >= 0) {
            pSt->Unk1C -= 2;
            f = 1;
        }
        if (G.Unk2100 & 0x4000) {
            if ((signed char)pSt->Unk1C < G.pUnk7C7C->Num - 2) {
                pSt->Unk1C += 2;
                f = 1;
            }
        }
        if (G.Unk2100 & 0x8000) {
            if (pSt->Unk1C & 1) {
                if ((signed char)--pSt->Unk1C >= 0) {
                    f = 1;
                }
            }
        }
        if (G.Unk2100 & 0x2000) {
            if (!(pSt->Unk1C & 1)) {
                if ((signed char)++pSt->Unk1C >= 0) {
                    f = 1;
                }
            }
        }
        if (f == 1) {
            Snd_se_stad(4, 0, 0, 0);
        }
        if ((signed char)pSt->Unk1C >= 0) {
            pSt->Pos[12].X = (pSt->Unk1C & 1) * 40;
            pSt->Pos[12].Y = ((signed char)pSt->Unk1C >> 1) * 30;
        }
        if ((signed char)pSt->Unk1C <= -1) {
            pSt->Pos[12].X = 0;
            pSt->Pos[12].Y = 0;
        }
    }
}

void func_800646F0(ST_WORK *pSt)
{
    ITEM_WORK tmp;
    RECT r;
    ITEM_WORK *pI;
    ITEM_WORK *pB;
    tagGLOBAL *pG;
    signed char n;
    UCHAR *pInfo;
    signed char no;
    signed char i;
    UCHAR f;
    UCHAR m;

    if (G.Key_trg & 0x2000) {
        pSt->Rtn1 = 0;
        pSt->Unk22 = 0;
        pSt->Unk24 = 0;
        Snd_se_stad(5, 0, 0, 0);
        return;
    }
    if (G.Key_trg & 0x1000) {
        no = pSt->Unk1C;
        pI = &G.pUnk7C7C->Item[no];
        if (pI->Id == 0) {
            no = Search_item_id(0, 0);
        }
        do {
            if (G.pUnk7C7C->Unk128 == no) {
                G.pUnk7C7C->Unk129 = 0;
                G.pUnk7C7C->Unk128 = 0xFF;
                func_8006AE20(10, 11);
            }
            pInfo = D_800A0514[pI->Id];
            switch (pInfo[0]) {
            case 2:
            case 5:
            case 6:
                f = 1;
                break;
            default:
                f = 0;
                break;
            }
            pI = &D_800D23B4[no];
            pB = &D_800D23B4[pSt->Unk1F + 10];
            m = 0;
            if (pI->Id == pB->Id && f == 1) {
                m = pInfo[1] >= pI->Num + pB->Num;
            }
            if (pI->Id == 0x20 && pB->Id == 0x2A && pI->Num + pB->Num < 4) {
                m = 2;
            }
            if (pI->Id == 0x2A && pB->Id == 0x20 && pI->Num + pB->Num < 4) {
                m = 3;
            }
            switch (m) {
            case 0:
                func_80065BA4(no, pSt->Unk1F % 8);
                tmp.Id = pB->Id;
                tmp.Num = pB->Num;
                tmp.Flg = pB->Flg;
                pB->Id = pI->Id;
                pB->Num = pI->Num;
                pB->Flg = pI->Flg;
                pI->Id = tmp.Id;
                pI->Num = tmp.Num;
                pI->Flg = tmp.Flg;
                break;
            case 2:
                pI->Id = pB->Id;
                pI->Num += pB->Num;
                pI->Flg = pB->Flg;
                func_80065BA4(no, pSt->Unk1F % 8);
                goto clear;
            case 1:
            case 3:
                pI->Num += pB->Num;
            clear:
                func_80065C8C(pSt->Unk1F % 8, 11);
                pB->Id = 0;
                pB->Num = 0;
                pB->Flg = 0;
                break;
            }
        } while (0);
        if (pI->Id == 0 && pB->Id != 0) {
            pSt->Rtn1 = 2;
            pSt->Rtn2 = no;
            pSt->Unk13 = 2;
        } else {
            pSt->Rtn1 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
        }
        Snd_se_stad(6, 0, 0, 0);
        return;
    }
    pSt->Flg &= ~0x10000;
    pSt->Flg &= ~0x8000;
    if ((USHORT)G.Unk20F8 & 0x1000) {
        if (--pSt->Unk1F < 0) {
            pSt->Unk1F = 0x3F;
        }
        n = pSt->Unk1F - 2;
        if (n < 0) {
            n = pSt->Unk1F + 0x3E;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->UnkC8 = -0x14;
        pSt->Rtn1 = 3;
        pSt->UnkCA = 0;
        pSt->Flg |= 0x10000;
    }
    if ((USHORT)G.Unk20F8 & 0x4000) {
        if (++pSt->Unk1F >= 0x40) {
            pSt->Unk1F = 0;
        }
        n = pSt->Unk1F + 2;
        if (n >= 0x40) {
            n = pSt->Unk1F - 0x3E;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->UnkCA = 1;
        pSt->UnkC8 = 0x14;
        pSt->Rtn1 = 3;
        pSt->Flg |= 0x8000;
    }
    if (G.Unk20F8 & 0x5000) {
        pI = &G.pUnk7C7C->Item[n + 10];
        func_8006ABF8(n, pI->Id, 0x801A1500, (ULONG *)0x801A1000);
        DrawSync(0);
    }
    if (pSt->Rtn1 == 3) {
        return;
    }
    if ((G.Unk20F8 & 0xC) == 0) {
        pSt->UnkCB = 0;
    } else if ((G.Unk20F8 & 0xC) == 0xC) {
        if (G.Unk20FC & 4) {
            pSt->UnkCB = 1;
        }
        if (G.Unk20FC & 8) {
            pSt->UnkCB = 2;
        }
    } else {
        if ((USHORT)G.Unk20F8 & 4) {
            pSt->UnkCB = 1;
        }
        if ((USHORT)G.Unk20F8 & 8) {
            pSt->UnkCB = 2;
        }
    }
    if ((G.Unk2100 & 4) && pSt->UnkCB == 1) {
        if ((pSt->Unk1F -= 5) < 0) {
            pSt->Unk1F += 0x40;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->Flg |= 0x10000;
    } else if ((G.Unk2100 & 8) && pSt->UnkCB == 2) {
        if ((pSt->Unk1F += 5) >= 0x40) {
            pSt->Unk1F -= 0x40;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->UnkCB = 2;
        pSt->Flg |= 0x8000;
    }
    pG = &G;
    if (*(ULONG *)&pG->Unk2100 & 0xC) {
        r.w = 20;
        r.h = 30;
        n = pSt->Unk1F - 2;
        if (n < 0) {
            n = pSt->Unk1F + 0x3E;
        }
        for (i = 0; i < 5; i++) {
            pI = &G.pUnk7C7C->Item[n + 10];
        func_8006ABF8(n, pI->Id, 0x801A1500, (ULONG *)0x801A1000);
            DrawSync(0);
            if (++n >= 0x40) {
                n = 0;
            }
        }
    }
}

void func_80064E80(ST_WORK *pSt)
{
    UCHAR no;
    LONG n;

    if (pSt->Unk13 == 0) {
        no = pSt->Rtn2;
        if (pSt->Rtn2 >= G.pUnk7C7C->Num - 1) {
            pSt->Rtn1 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            return;
        }
        n = no;
        G.pUnk7C7C->Item[n].Id = G.pUnk7C7C->Item[n + 1].Id;
        G.pUnk7C7C->Item[n].Num = G.pUnk7C7C->Item[n + 1].Num;
        G.pUnk7C7C->Item[n].Flg = G.pUnk7C7C->Item[n + 1].Flg;
        G.pUnk7C7C->Item[n + 1].Id = 0;
        G.pUnk7C7C->Item[n + 1].Num = 0;
        G.pUnk7C7C->Item[n + 1].Flg = 0;
        func_8006AD38(no, no + 1);
        if (G.pUnk7C7C->Unk128 == n + 1) {
            G.pUnk7C7C->Unk128 = n;
            G.pUnk7C7C->Unk129 = G.pUnk7C7C->Item[G.pUnk7C7C->Unk128].Id;
        }
        if ((signed char)pSt->Unk1C == n + 1) {
            pSt->Unk1C--;
            pSt->Pos[12].X = (pSt->Unk1C & 1) * 40;
            pSt->Pos[12].Y = ((signed char)pSt->Unk1C >> 1) * 30;
        }
        pSt->Rtn2++;
        if (G.pUnk7C7C->Item[n + 2].Id == 0) {
            pSt->Rtn1 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
        }
        pSt->Unk13 = 2;
    }
    pSt->Unk13--;
}

void func_80065054(ST_WORK *pSt)
{
    if (pSt->UnkCA == 0) {
        if (pSt->UnkC8 >= -4) {
            pSt->Rtn1 = 1;
        }
        pSt->UnkC8 += 4;
    } else {
        if (pSt->UnkC8 < 5) {
            pSt->Rtn1 = 1;
        }
        pSt->UnkC8 -= 4;
    }
}

void func_800650BC(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        pSt->Rtn1 = 0;
        pSt->Unk22 = 0;
        pSt->Unk24 = 0;
        Snd_se_stad(5, 0, 0, 0);
        pSt->Flg |= 0x20000000;
    }
}

void func_8006511C(ST_WORK *pSt)
{
    ULONG i;

    for (i = 0; i < 8; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x9B);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x9B);
    }
    for (i = 8; i < 16; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x9A);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x9A);
    }
    for (i = 16; i < 24; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x1A);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x1A);
    }
    for (i = 24; i < 32; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x86);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x86);
    }
    for (i = 32; i < 40; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x17);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x17);
    }
    i = pSt->UnkD0;
    i <<= 8;
    func_8006E600(pSt, (void *)0x8019C000, D_8009F328, i | 3);
    func_8006E600(pSt, (void *)0x8019C1E0, D_8009F448, i | 7);
    func_8006E7D4(pSt, (TILE *)0x8019D478, D_8009F49C, 1);
    i = pSt->UnkD1;
    i <<= 8;
    func_8006E600(pSt, (void *)0x8019C140, D_8009F3F4, i | 3);
    func_8006E600(pSt, (void *)0x8019C1B8, D_8009F418, i | 1);
    func_8006E600(pSt, (void *)0x8019C2F8, D_8009F4A8, ((UCHAR)(pSt->UnkD3 + 3) << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x8019C320, D_8009F4B4, (pSt->UnkD5 << 8) | 1);
    i = pSt->UnkD5;
    i <<= 8;
    func_8006E600(pSt, (void *)0x8019C078, D_8009F34C, i | 5);
    func_8006E600(pSt, (void *)0x8019D068, D_8009F4CC, i | 1);
    func_8006E600(pSt, (void *)0x8019D090, D_8009F4D8, i | 1);
    i = pSt->UnkD0;
    i <<= 8;
    func_8006E6D8(pSt, (POLY_FT4 *)0x8019D0B8, D_8009F388, i | 0x07000008);
    func_8006E6D8(pSt, (POLY_FT4 *)0x8019D338, D_8009F3E8, i | 0x07000001);
    func_8006E6D8(pSt, (POLY_FT4 *)0x8019D388, D_8009F424, i | 0x04000003);
    func_8006E7D4(pSt, (TILE *)0x8019D748, D_8009F4C0, 1);
    func_80065EE0(pSt);
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800654A8);

void func_80065BA4(UCHAR a, UCHAR b)
{
    RECT r;
    ST_WORK *pSt = &St;
    USHORT (*tbl)[2] = D_800A004C;
    USHORT *pa = tbl[a];
    USHORT *pb;
    USHORT (*tbl2)[2];

    r.x = pa[0];
    r.y = pa[1];
    r.w = 0x14;
    r.h = 0x1E;
    StoreImage(&r, pSt->pBuf);
    DrawSync(0);
    tbl2 = D_8009F4F8;
    pb = tbl2[b];
    r.x = pb[0];
    r.y = pb[1];
    MoveImage(&r, pa[0], pa[1]);
    DrawSync(0);
    r.x = pb[0];
    r.y = pb[1];
    LoadImage(&r, pSt->pBuf);
    DrawSync(0);
}

void func_80065C8C(UCHAR dst, UCHAR src)
{
    RECT r;

    r.x = D_800A004C[src][0];
    r.y = D_800A004C[src][1];
    r.w = 0x14;
    r.h = 0x1E;
    MoveImage(&r, D_8009F4F8[dst][0], D_8009F4F8[dst][1]);
    DrawSync(0);
}

void func_80065D04(ST_WORK *pSt, UCHAR no, SHORT y0)
{
    POLY_FT4 *p = (POLY_FT4 *)0x8019D4C8;
    signed char n = no - 3;
    UCHAR clut;
    SHORT y;
    ULONG i;
    LONG x, tp, ot, h, w;
    UCHAR v;

    x = 0xA5;
    tp = 0x17;
    ot = 5;
    h = 0x16;
    w = 0x1D;
    clut = pSt->UnkD1;
    y = y0 + 0x19;
    if (n < 0) {
        n = no + 0x3D;
    }
    n %= 8;
    if (G.Cbuf_id) {
        p++;
    }
    for (i = 0; i < 7; i++) {
        setPolyFT4(p);
        setRGB0(p, 0x80, 0x80, 0x80);
        v = n * 30;
        setUV4(p, 0, v, 0x28, v, 0, v + 30, 0x28, v + 30);
        p->tpage = getTPage(1, 0, (tp & 0xF) << 6, (tp & 0x10) << 4);
        p->clut = (clut + 0x1E0) << 6;
        setXY4(p, x, y + i * 24, x + w, y + i * 24, x, y + h + i * 24, x + w, y + h + i * 24);
        AddPrim(G.pOt0 + ot, p);
        p += 2;
        if (++n >= 8) {
            n = 0;
        }
    }
}

void func_80065EE0(ST_WORK *pSt)
{
    ULONG i, j;
    TILE *pT;
    LINE_F2 *pL;
    SPRT *pS;

    for (j = 0, pT = (TILE *)0x8019E1B8; j < 2; j++, pT++) {
        setTile(pT);
        setRGB0(pT, 0x20, 0x20, 0x40);
        setSemiTrans(pT, 1);
        setXY0(pT, 0x10, 0x30);
        setWH(pT, 0xB8, 0x30);
    }
    for (j = 0, pT = (TILE *)0x8019E1D8; j < 2; j++, pT++) {
        setTile(pT);
        setRGB0(pT, 0x20, 0x20, 0x40);
        setSemiTrans(pT, 1);
        setXY0(pT, 0x10, 0x78);
        setWH(pT, 0xB8, 0x30);
    }
    for (j = 0, pL = (LINE_F2 *)0x8019E1F8; j < 2; pL++, j++) {
        setlen(pL, 3);
        setcode(pL, 0x42);
        setXY2(pL, 0x10, 0x5F, 0xC8, 0x5F);
    }
    for (j = 0, pL = (LINE_F2 *)0x8019E218; j < 2; pL++, j++) {
        setlen(pL, 3);
        setcode(pL, 0x42);
        setXY2(pL, 0x10, 0x78, 0xC8, 0x78);
    }
    for (j = 0, pL = (LINE_F2 *)0x8019E238; j < 2; pL++, j++) {
        setlen(pL, 3);
        setcode(pL, 0x42);
        setRGB0(pL, 0x80, 0x20, 0x20);
    }
    pS = (SPRT *)0x8019E168;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; pS++, j++) {
            setSprt(pS);
            setRGB0(pS, 0x80, 0x80, 0x80);
            pS->clut = (pSt->UnkD5 + 0x1E0) << 6;
            setSemiTrans(pS, 1);
            setXY0(pS, 0xC8, 1);
            setUV0(pS, 0x20, 0x88);
            setWH(pS, 0x10, 5);
        }
    }
    pS = (SPRT *)0x8019D768;
    for (i = 0; i < 0x40; i++) {
        for (j = 0; j < 2; pS++, j++) {
            setSprt(pS);
            setRGB0(pS, 0x80, 0x80, 0x80);
            setXY0(pS, 0xCC, i + 0x4C);
            setWH(pS, 8, 1);
            pS->clut = (pSt->UnkD5 + 0x1E0) << 6;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800661B4);

void func_80066530(ST_WORK *pSt)
{
switch (pSt->St_type) { case 1: pSt->Rtn1 = 0xB; pSt->Rtn2 = 0; pSt->Rtn0++; break; case 0: pSt->Pos[3].X = 0x48; pSt->Pos[4].X = 0x48; pSt->Pos[5].X = 0x48; pSt->Pos[6].X = 0x48; pSt->Pos[7].X = 0x48; pSt->Pos[8].X = -0xD8; pSt->Pos[9].X = -0xD8; pSt->Pos[14].X = -0xD8; pSt->Pos[24].X = -0xD8; pSt->Pos[10].Y = 0x30; pSt->Rtn1 = 0; pSt->Unk22 = 0; pSt->Unk24 = 0; pSt->Flg |= 0x20000000; pSt->Flg |= 0x40000000; pSt->Rtn0++; break; }
}

void func_800665C8(ST_WORK *pSt)
{
    D_800A0100[pSt->Rtn1](pSt);
}

void func_80066604(ST_WORK *pSt)
{
    UCHAR *p;
    LONG se;

    if ((G.Unk20FC & 0x20) | (G.Key_trg & 0x2000)) {
        pSt->Rtn0++;
        Snd_se_stad(5, 0, 0, 0);
    } else {
        if (G.Key_trg & 0x1000) {
            if ((signed char)pSt->Unk1C >= 0) {
                p = (UCHAR *)&G.pUnk7C7C->Item[(signed char)pSt->Unk1C];
                if (p[0] != 0) {
                    pSt->Rtn1 = 1;
                    pSt->Unk1E = 4;
                    pSt->Flg &= ~0x20000000;
                    Snd_se_stad(6, 0, 0, 0);
                    pSt->Unk2A = ((USHORT *)p)[1];
                } else {
                    Snd_se_stad(7, 0, 0, 0);
                }
            }
            if ((signed char)pSt->Unk1C == -1) {
                pSt->Rtn1 = 4;
                pSt->Rtn2 = 0;
                pSt->Unk13 = 0;
                Snd_se_stad(9, 0, 0, 0);
            }
            if ((signed char)pSt->Unk1C == -2) {
                pSt->Rtn1 = 5;
                pSt->Rtn2 = 0;
                pSt->Unk13 = 0;
                pSt->Pos[24].X = -0xD8;
                Snd_se_stad(9, 0, 0, 0);
            }
            if ((signed char)pSt->Unk1C < -2) {
                pSt->Rtn0++;
                Snd_se_stad(5, 0, 0, 0);
            }
        } else {
            se = 0;
            if (G.Unk2100 & 0x1000) {
                if ((signed char)pSt->Unk1C >= -2) {
                    pSt->Unk1C -= 2;
                    se = 1;
                }
            }
            if (G.Unk2100 & 0x4000) {
                if ((signed char)pSt->Unk1C < G.pUnk7C7C->Num - 2) {
                    pSt->Unk1C += 2;
                    se = 1;
                }
            }
            if (G.Unk2100 & 0x8000) {
                if (pSt->Unk1C & 1) {
                    pSt->Unk1C--;
                    if ((signed char)pSt->Unk1C >= -2) {
                        se = 1;
                    }
                }
            }
            if (G.Unk2100 & 0x2000) {
                if (!(pSt->Unk1C & 1)) {
                    pSt->Unk1C++;
                    if ((signed char)pSt->Unk1C >= -2) {
                        se = 1;
                    }
                }
            }
            if (se == 1) {
                Snd_se_stad(4, 0, 0, 0);
            }
            if ((signed char)pSt->Unk1C >= 0) {
                pSt->Pos[12].X = (pSt->Unk1C & 1) * 40;
                pSt->Pos[12].Y = ((signed char)pSt->Unk1C >> 1) * 30;
            }
        }
    }
}

void func_800668E4(ST_WORK *pSt)
{
    if (func_80069614(pSt, 0)) {
        pSt->Rtn1 = 2;
        pSt->Unk1E = 0;
    }
}

void func_80066920(ST_WORK *pSt)
{
    ITEM_WORK *e = &G.pUnk7C7C->Item[pSt->Unk1C];
    LONG n = 3;

    if (e->Flg & 0x10) {
        n = 4;
    }
    if (G.Key_trg & 0x2000) {
        pSt->Rtn1 = 3;
        Snd_se_stad(5, 0, 0, 0);
    } else if (G.Key_trg & 0x1000) {
        if ((signed char)pSt->Unk1E == 0 && (UCHAR)(e->Id + 0x7D) < 2) {
            pSt->Rtn1 = 8;
        } else {
            pSt->Rtn1 = pSt->Unk1E + 6;
        }
        pSt->Rtn2 = 0;
        pSt->Unk13 = 0;
        Snd_se_stad(6, 0, 0, 0);
    } else {
        if (G.Unk2100 & 0x1000) {
            pSt->Unk1E--;
        }
        if (G.Unk2100 & 0x4000) {
            pSt->Unk1E++;
        }
        if ((signed char)pSt->Unk1E < 0) {
            pSt->Unk1E = n - 1;
        }
        if ((signed char)pSt->Unk1E >= n) {
            pSt->Unk1E = 0;
        }
        if (*(ULONG *)&G.Unk2100 & 0x5000) {
            Snd_se_stad(4, 0, 0, 0);
        }
    }
}

void func_80066A90(ST_WORK *pSt)
{
    if (func_80069614(pSt, 1)) {
        pSt->Rtn1 = 0;
        pSt->Unk22 = 0;
        pSt->Unk24 = 0;
        pSt->Flg |= 0x20000000;
    }
}

void func_80066ADC(ST_WORK *pSt)
{
    switch (pSt->Rtn2) {
    case 0:
        pSt->Flg &= ~0x20000000;
        if (pSt->Unk13 < 0xC) {
            pSt->Pos[11].Y += 6;
            pSt->Pos[0].X -= 0xE;
            pSt->Pos[1].X += 0xE;
            pSt->Pos[2].X += 0xE;
            pSt->Pos[3].X += 0xE;
            pSt->Pos[4].X += 0xE;
            pSt->Pos[5].X += 0xE;
            pSt->Pos[6].X += 0xE;
            pSt->Pos[7].X += 0xE;
            pSt->Pos[12].X += 0xE;
            pSt->Unk13++;
        } else {
            pSt->Rtn0 = 4;
            pSt->Flg &= ~0x40000000;
            func_8006ED28(pSt);
        }
        break;
    case 1:
        if (pSt->Unk13 < 0xC) {
            pSt->Pos[11].Y -= 6;
            pSt->Pos[0].X += 0xE;
            pSt->Pos[1].X -= 0xE;
            pSt->Pos[2].X -= 0xE;
            pSt->Pos[3].X -= 0xE;
            pSt->Pos[4].X -= 0xE;
            pSt->Pos[5].X -= 0xE;
            pSt->Pos[6].X -= 0xE;
            pSt->Pos[7].X -= 0xE;
            pSt->Pos[12].X -= 0xE;
            pSt->Unk13++;
        } else {
            pSt->Rtn1 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Flg |= 0x20000000;
        }
        break;
    }
}

extern UCHAR D_800A00B0_b[][2] __asm__("D_800A00B0"); /* same table as D_800A00B0, byte access */

static const char D_80010DFC[] = "FILEI.TIM";

void func_80066CA0(ST_WORK *pSt)
{
    UCHAR no;
    ULONG *p;
    UCHAR se;

    switch (pSt->Rtn2) {
    case 0:
        Cd_read(0x1D, pSt->pBuf, 0, D_80010DFC);
        p = pSt->pBuf;
        D_800CCBBC = 0x917;
        Mapping_tim(p);
        pSt->UnkC1 = 0;
        pSt->UnkBE = 0;
        pSt->Unk13 = 0;
        pSt->Flg &= ~0x2000;
        pSt->Flg &= ~0x4000;
        pSt->Rtn2++;
        break;
    case 1:
        pSt->Flg &= ~0x20000000;
        if (pSt->Unk13 < 12) {
            pSt->Pos[11].Y += 6;
            pSt->Unk13++;
        } else {
            pSt->Unk13 = 0;
            pSt->Rtn2++;
            pSt->Flg |= 0x1000;
        }
        break;
    case 2:
        if (pSt->Unk13 < 12) {
            pSt->Pos[9].X += 0x12;
            pSt->Pos[14].X += 0x12;
            pSt->Pos[24].X += 0x12;
            pSt->Pos[10].Y -= 4;
            pSt->Unk13++;
        } else {
            pSt->Rtn2++;
            pSt->Unk13 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Pos[14].X = 0;
            pSt->Pos[14].Y = 0;
            pSt->Unk20 = 0;
            pSt->Flg |= 0x1000000;
        }
        break;
    case 3:
        if (G.Key_trg & 0x2000) {
            pSt->Rtn2 = 6;
            Snd_se_stad(5, 0, 0, 0);
            pSt->Flg &= ~0x1000000;
            pSt->Flg &= ~0x2000;
            pSt->Flg &= ~0x4000;
            break;
        }
        if (G.Key_trg & 0x1000) {
            no = D_800A00B0_b[pSt->Unk20][0];
            if (G.Unk79EC[3] == 0x1E && pSt->Unk20 == 0) {
                no = 0x1E;
            }
            if (Flg_ck(&G.Unk79EC[2], no)) {
                pSt->Unk13 = 0;
                pSt->Rtn2++;
                pSt->Flg &= ~0x1000000;
                pSt->Flg &= ~0x2000;
                pSt->Flg &= ~0x4000;
                Snd_se_stad(6, 0, 0, 0);
                pSt->UnkBC = no;
                pSt->UnkBD = 0;
            } else {
                Snd_se_stad(7, 0, 0, 0);
            }
            break;
        }
        se = 0;
        if (pSt->UnkC1 == 0) {
            if (G.Unk2100 & 0x2000) {
                if ((signed char)(pSt->Unk20 % 5) == 4) {
                    se = 1;
                    pSt->Unk20 += 11;
                    pSt->UnkC1 = 1;
                    pSt->Rtn2 = 8;
                }
            }
        } else if (G.Unk2100 & 0x8000) {
            if ((signed char)(pSt->Unk20 % 5) == 0) {
                se = 1;
                pSt->Unk20 -= 11;
                pSt->UnkC1 = 0;
                pSt->Rtn2 = 9;
            }
        }
        if (se) {
            pSt->Unk13 = 0;
            pSt->Flg &= ~0x2000;
            pSt->Flg &= ~0x4000;
            pSt->Flg &= ~0x1000000;
            pSt->Pos[14].X = (signed char)(pSt->Unk20 % 5) * 32;
            pSt->Pos[14].Y = (signed char)((signed char)(pSt->Unk20 % 15) / 5) * 32;
            if (pSt->UnkC1) {
                pSt->Pos[14].X += 0x20;
            }
            Snd_se_stad(4, 0, 0, 0);
            break;
        }
        if (G.Unk2100 & 0x1000) {
            if ((signed char)(pSt->Unk20 % 15) >= 5) {
                pSt->Unk20 -= 5;
                se = 1;
            }
        }
        if (G.Unk2100 & 0x4000) {
            if ((signed char)(pSt->Unk20 % 15) < 10) {
                pSt->Unk20 += 5;
                se = 1;
            }
        }
        if (G.Unk2100 & 0x8000) {
            if ((signed char)(pSt->Unk20 % 5) != 0) {
                pSt->Unk20--;
                se = 1;
            }
        }
        if (G.Unk2100 & 0x2000) {
            if ((signed char)(pSt->Unk20 % 5) != 4) {
                pSt->Unk20++;
                se = 1;
            }
        }
        if (se == 1) {
            Snd_se_stad(4, 0, 0, 0);
        }
        pSt->Pos[14].X = (signed char)(pSt->Unk20 % 5) * 32;
        pSt->Pos[14].Y = (signed char)((signed char)(pSt->Unk20 % 15) / 5) * 32;
        if (pSt->UnkC1) {
            pSt->Pos[14].X += 0x20;
        }
        if (++pSt->UnkBE > 20) {
            pSt->UnkBE = -20;
        }
        if (pSt->UnkBE >= 0) {
            pSt->Pos[23].X = -1;
            pSt->Pos[22].X = 1;
        } else {
            pSt->Pos[23].X = 0;
            pSt->Pos[22].X = 0;
        }
        pSt->Flg &= ~0x2000;
        pSt->Flg &= ~0x4000;
        if (pSt->UnkC1) {
            pSt->Flg |= 0x2000;
        } else {
            pSt->Flg |= 0x4000;
        }
        break;
    case 4:
        if (pSt->Unk13 < 12) {
            pSt->Pos[9].X -= 0x12;
            pSt->Pos[14].X -= 0x12;
            pSt->Pos[24].X -= 0x12;
            pSt->Pos[10].Y += 4;
            pSt->Pos[0].X -= 0xE;
            pSt->Pos[1].X += 0xE;
            pSt->Pos[2].X += 0xE;
            pSt->Pos[3].X += 0xE;
            pSt->Pos[4].X += 0xE;
            pSt->Pos[5].X += 0xE;
            pSt->Pos[6].X += 0xE;
            pSt->Pos[7].X += 0xE;
            pSt->Pos[12].X += 0xE;
            pSt->Unk13++;
        } else {
            pSt->Rtn0 = 7;
        }
        break;
    case 5:
        if (pSt->Unk13 < 12) {
            pSt->Pos[9].X += 0x12;
            pSt->Pos[14].X += 0x12;
            pSt->Pos[24].X += 0x12;
            pSt->Pos[10].Y -= 4;
            pSt->Pos[0].X += 0xE;
            pSt->Pos[1].X -= 0xE;
            pSt->Pos[2].X -= 0xE;
            pSt->Pos[3].X -= 0xE;
            pSt->Pos[4].X -= 0xE;
            pSt->Pos[5].X -= 0xE;
            pSt->Pos[6].X -= 0xE;
            pSt->Pos[7].X -= 0xE;
            pSt->Pos[12].X -= 0xE;
            pSt->Unk13++;
        } else {
            pSt->Rtn2 = 3;
            pSt->Unk13 = 0;
            pSt->Flg |= 0x1000000;
        }
        break;
    case 6:
        if (pSt->Unk13 < 12) {
            pSt->Pos[9].X -= 0x12;
            pSt->Pos[14].X -= 0x12;
            pSt->Pos[24].X -= 0x12;
            pSt->Pos[10].Y += 4;
            pSt->Unk13++;
        } else {
            pSt->Rtn2++;
            pSt->Unk13 = 0;
            pSt->Flg &= ~0x1000;
        }
        break;
    case 7:
        if (pSt->Unk13 < 12) {
            pSt->Pos[11].Y -= 6;
            pSt->Unk13++;
        } else {
            pSt->Rtn1 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Flg |= 0x20000000;
        }
        break;
    case 8:
        if (pSt->Unk13 < 16) {
            pSt->Pos[24].X -= 0xB;
            pSt->Unk13++;
        } else {
            pSt->Rtn2 = 3;
            pSt->Unk13 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Flg |= 0x1000000;
        }
        break;
    case 9:
        if (pSt->Unk13 < 16) {
            pSt->Pos[24].X += 0xB;
            pSt->Unk13++;
        } else {
            pSt->Rtn2 = 3;
            pSt->Unk13 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Flg |= 0x1000000;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_800676B8);

void func_80067B70(ST_WORK *pSt)
{
    D_800A0138[pSt->Rtn2](pSt);
}

void func_80067BAC(ST_WORK *pSt)
{
    UCHAR v = pSt->Unk1C;

    pSt->Unk22 = 0;
    pSt->Unk24 = 0;
    pSt->Unk1D = v;
    pSt->Pos[13].X = (v & 1) * 40;
    pSt->Flg |= 0x2000000;
    pSt->Flg |= 0x20000000;
    pSt->Rtn2++;
    pSt->Pos[13].Y = ((signed char)pSt->Unk1D >> 1) * 30;
}

LONG Pl_wep_eff_parts_cnt_l(void) __asm__("Pl_wep_eff_parts_cnt"); /* UCHAR result used as int */

void func_80067C14(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    ITEM_WORK *p;
    ITEM_WORK *q;
    UCHAR *pc;
    SHORT n;
    LONG i;
    UCHAR c;
    LONG se;

    p = &pG->pUnk7C7C->Item[pSt->Unk1C];
    n = 0;
    if (pG->Key_trg & 0x2000) {
        pSt->Rtn1 = 3;
        pSt->Flg &= ~0x2000000;
        pSt->Flg &= ~0x20000000;
        Snd_se_stad(5, 0, 0, 0);
        return;
    }
    if (pG->Key_trg & 0x1000) {
        q = &pG->pUnk7C7C->Item[(signed char)pSt->Unk1D];
        pc = func_8006A898(pSt, p->Id, q->Id);
        pSt->Unk48 = p;
        pSt->Unk4C = q;
        pSt->Unk58 = pc;
        if (pc != (UCHAR *)-1 && pSt->Unk1C != (signed char)pSt->Unk1D) {
            switch (pc[0]) {
            case 2:
                for (i = 0; D_800A00EC[i][0] != -1; i++) {
                    if (D_800A00EC[i][0] == pc[3]) {
                        break;
                    }
                }
                c = pSt->Unk6A = D_800A00EC[i][1];
                switch (c) {
                case 0:
                case 1:
                    n = 6;
                    break;
                }
                i = n;
                if (i != 0 && i < pG->pUnk7C7C->Unk12C[c]) {
                    Mess_set(0, 0x1400, 13, 0);
                    pSt->Rtn2 = 5;
                    pSt->Flg &= ~0x20000000;
                    return;
                }
                break;
            case 0:
                if (pc[1] == 12) {
                    if (p->Id != 12) {
                        p = q;
                    }
                    if (p->Num == 0) {
                        if (Pl_wep_eff_parts_cnt_l()) {
                            Mess_set(0, 0x1400, 9, 0);
                            pSt->Rtn2 = 4;
                            pSt->Flg &= ~0x20000000;
                            return;
                        }
                        Pl_wep_eff_parts_init(PLAYER_EM(pG));
                        Pl_wep_poly_init();
                    } else {
                        Mess_set(0, 0x1000, 10, 0);
                        pSt->Rtn2 = 2;
                        pSt->Flg &= ~0x20000000;
                        Snd_se_stad(7, 0, 0, 0);
                        return;
                    }
                }
                break;
            }
            func_80068024(pSt);
        } else {
            Snd_se_stad(7, 0, 0, 0);
        }
        return;
    }
    se = 0;
    if (pG->Unk2100 & 0x1000) {
        if ((signed char)pSt->Unk1D >= 2) {
            pSt->Unk1D -= 2;
            se = 1;
        }
    }
    if (G.Unk2100 & 0x4000) {
        if ((signed char)pSt->Unk1D < G.pUnk7C7C->Num - 2) {
            pSt->Unk1D += 2;
            se = 1;
        }
    }
    if (G.Unk2100 & 0x8000) {
        if (pSt->Unk1D & 1) {
            pSt->Unk1D--;
            se = 1;
        }
    }
    if (G.Unk2100 & 0x2000) {
        if (!(pSt->Unk1D & 1)) {
            pSt->Unk1D++;
            se = 1;
        }
    }
    if (se == 1) {
        Snd_se_stad(4, 0, 0, 0);
    }
    pSt->Pos[13].X = (pSt->Unk1D & 1) * 40;
    pSt->Pos[13].Y = ((signed char)pSt->Unk1D >> 1) * 30;
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_80068024);

void func_80068A54(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        pSt->Rtn1 = 3;
        Snd_se_stad(5, 0, 0, 0);
        pSt->Flg &= ~0x2000000;
    }
}

void func_80068AB4(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        switch (G.Unk7848) {
        case 0:
            Pl_wep_eff_parts_init(G_PLAYER);
            Pl_wep_poly_init();
            func_80068024(pSt);
            break;
        case 1:
            pSt->Rtn1 = 3;
            pSt->Flg &= ~0x2000000;
            Snd_se_stad(5, 0, 0, 0);
            break;
        }
    }
}

void func_80068B5C(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        switch (G.Unk7848) {
        case 0:
            pSt->Unk58 += 8;
        case 1:
            func_80068024(pSt);
            break;
        }
    }
}

void func_80068BC0(ST_WORK *pSt)
{
    USHORT f = 0;
    ITEM_WORK *pA = pSt->Unk48;
    ITEM_WORK *pB = pSt->Unk4C;
    UCHAR *pC = pSt->Unk58;
    UCHAR *pN = pSt->Unk5C;
    UCHAR *pM = pSt->Unk60;
    tagGLOBAL *pG = &G;
    UCHAR *pI;

    if (pSt->Unk13 < 5) {
        if (pA->Id == 0xFF) {
            pSt->Pos[13].X += pSt->Unk66;
            pSt->Pos[13].Y += pSt->Unk68;
            pSt->Pos[12].X += pSt->Unk66;
            pSt->Pos[12].Y += pSt->Unk68;
        } else if (&pSt->Unk1D == pM) {
            pSt->Pos[13].X += pSt->Unk66;
            pSt->Pos[13].Y += pSt->Unk68;
        } else {
            pSt->Pos[12].X += pSt->Unk66;
            pSt->Pos[12].Y += pSt->Unk68;
        }
        pSt->Unk13++;
        return;
    }
    pSt->Flg &= ~0x2000000;
    if ((pA->Flg & 3) == 3) {
        f = 3;
    }
    switch (pSt->Unk64) {
    case 0:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Unk13 = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Id = pC[3];
        pI = D_800A0514[pA->Id];
        pA->Num = pC[4];
        pA->Flg = pI[3];
        if (Flg_ck(G.Unk77F8, 9) && (pA->Id == 0xD || pA->Id == 0x10)) {
            pA->Flg |= 3;
        }
        func_8006AB08(*pN);
        break;
    case 1:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Unk13 = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        if (pC[0] == 3 || pC[0] == 4) {
            pA->Id = pC[3];
            pA->Flg = D_800A0514[pC[3]][3] | (pA->Flg & 0xFF00);
            if (pG->pUnk7C7C->Unk128 == (signed char)*pN) {
                pG->pUnk7C7C->Unk129 = pC[3];
            }
        }
        pA->Num += pSt->Unk65;
        break;
    case 2:
        pSt->Rtn1 = 3;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        if (pC[0] == 3) {
            pA->Id = pC[3];
            pA->Flg = D_800A0514[pC[3]][3] | (pA->Flg & 0xFF00);
            if (pG->pUnk7C7C->Unk128 == (signed char)*pN) {
                pG->pUnk7C7C->Unk129 = pC[3];
            }
        }
        pA->Num += pSt->Unk65;
        break;
    case 3:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Unk13 = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Id = pC[3];
        pI = D_800A0514[pA->Id];
        pA->Num = pSt->Unk65;
        pA->Flg = pI[3];
        func_8006AB08(*pN);
        break;
    case 4:
        pSt->Rtn1 = 3;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Id = pC[3];
        pI = D_800A0514[pA->Id];
        pA->Num = pSt->Unk65;
        pA->Flg = pI[3];
        func_8006AB08(*pN);
        break;
    case 5:
        pSt->Rtn1 = 3;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pB->Id = pC[4];
        pI = D_800A0514[pC[4]];
        pB->Flg = pI[3] | (pB->Flg & 0xFF00);
        pB->Num = pA->Num;
        func_8006AB08(*pM);
        pA->Id = pC[3];
        pI = D_800A0514[pC[3]];
        pA->Flg = pI[3] | (pA->Flg & 0xFF00);
        if (pG->pUnk7C7C->Unk128 == (signed char)*pN) {
            pG->pUnk7C7C->Unk129 = pC[3];
        }
        pA->Num = pSt->Unk65;
        *pM = *pN;
        break;
    case 6:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Unk13 = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Flg |= 3;
        if (pA->Id == 0xC) {
            pA->Id = 0x14;
            pA->Flg = 7;
            pG->pUnk7C7C->Unk129 = pA->Id;
            Pl_wep_eff_parts_init(&Pl_work);
            Pl_wep_poly_init();
        }
        break;
    case 7:
        pSt->Rtn1 = 3;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        switch (pA->Id) {
        case 2:
            pB->Id = 0x15;
            break;
        case 3:
            pB->Id = 0x15;
            break;
        case 4:
            pB->Id = 0x17;
            break;
        case 0x11:
            pB->Id = 0x1E;
            break;
        case 0x12:
            pB->Id = 0x1E;
            break;
        case 0x13:
            pB->Id = 0x1F;
            break;
        }
        pI = D_800A0514[pB->Id];
        pB->Flg = pI[3] | (pB->Flg & 0xFF00);
        pB->Num = pA->Num;
        func_8006AB08(*pM);
        pA->Id = pC[3];
        pI = D_800A0514[pC[3]];
        pA->Flg = pI[3] | (pA->Flg & 0xFF00);
        if (pG->pUnk7C7C->Unk128 == (signed char)*pN) {
            pG->pUnk7C7C->Unk129 = pC[3];
        }
        pA->Num = pSt->Unk65;
        *pM = *pN;
        break;
    }
    pA->Flg |= f;
}

void func_80069280(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    UCHAR *p;
    UCHAR *pE;

    switch (pSt->Rtn2) {
    case 0:
        p = (UCHAR *)&pG->pUnk7C7C->Item[pSt->Unk1C];
        func_8006AC88(p[0]);
        pSt->Unk3E = 0;
        pSt->Unk13 = 0;
        pSt->Rtn2++;
        break;
    case 1:
        if (pSt->Unk13 < 12) {
            pSt->Pos[8].X += 0x12;
        } else {
            p = (UCHAR *)&pG->pUnk7C7C->Item[pSt->Unk1C];
            pE = D_800A0514[p[0]];
            Flg_on(pG->Unk7910, pE[2]);
            Mess_set(0, 0x1100, p[0] + 0x10, 0);
            pSt->Rtn2++;
        }
        pSt->Unk13++;
        if (pSt->Unk3E == 0 && func_80069614(pSt, 1)) {
            pSt->Unk3E = 1;
        }
        break;
    case 2:
        if (!(D_800DBB58 & 0x80)) {
            pSt->Rtn2 = 3;
            pSt->Unk13 = 0;
            p = (UCHAR *)&pG->pUnk7C7C->Item[pSt->Unk1C];
            pSt->UnkE0 = pE = func_8006A918(pSt, p[0]);
            if (pE != (UCHAR *)-1 && pE[0] == 1) {
                pSt->Unk76 = pE[2];
                pSt->Unk77 = pSt->Unk1C;
                pSt->Rtn1 = 0xD;
                pSt->Rtn2 = 0;
                pSt->Unk13 = 0;
                Snd_se_stad(6, 0, 0, 0);
            } else {
                Snd_se_stad(5, 0, 0, 0);
            }
        }
        break;
    case 3:
        if (pSt->Unk13 < 12) {
            pSt->Pos[8].X -= 0x12;
        } else {
            p = (UCHAR *)&pG->pUnk7C7C->Item[pSt->Unk1C];
            pSt->UnkE0 = pE = func_8006A918(pSt, p[0]);
            if (pE != (UCHAR *)-1) {
                if (pE[0] == 0) {
                    pSt->Unk76 = pE[2];
                    pSt->Unk77 = pSt->Unk1C;
                    pSt->Rtn1 = 0xC;
                    pSt->Rtn2 = 0;
                }
            } else {
                pSt->Rtn1 = 0;
                pSt->Rtn2 = 0;
                pSt->Unk22 = 0;
                pSt->Unk24 = 0;
                pSt->Flg |= 0x20000000;
            }
        }
        pSt->Unk13++;
        break;
    }
}

void func_8006954C(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    ITEM_WORK *p = &pG->pUnk7C7C->Item[pSt->Unk1C];
    USHORT w = p->Flg;
    UCHAR id = p->Id;
    USHORT n;

    if (w & 0x100) {
        p->Flg = w & ~0x100;
        p->Id = 0xF;
    } else {
        p->Flg = w | 0x100;
        p->Id = 0xE;
    }
    n = D_800A0514[p->Id][3] | (p->Flg & 0xFF00);
    p->Flg = n;
    if ((w & 3) == 3) {
        p->Flg = n | 3;
    }
    if (id == pG->pUnk7C7C->Unk129 && pSt->Unk1C == pG->pUnk7C7C->Unk128) {
        pG->pUnk7C7C->Unk129 = p->Id;
    }
    pSt->Rtn1 = 2;
}

UCHAR func_80069614(ST_WORK *pSt, LONG a1)
{
    POLY_FT4 *p;
    ULONG i;
    USHORT f;
    UCHAR *pI;

    if (a1 == 0) {
        pI = (UCHAR *)&G.pUnk7C7C->Item[pSt->Unk1C];
        pSt->Unk42 = *(ITEM_WORK *)pI;
        f = pSt->Unk42.Flg;
        if (pSt->Unk3A == 0) {
            p = (POLY_FT4 *)0x801AE5A8;
            if (G.Cbuf_id) {
                p = (POLY_FT4 *)0x801AE5D0;
            }
            for (i = 0; i < 3; i++, p += 2) {
                p->v0 += 0x10;
                p->v1 += 0x10;
                p->v2 += 0x10;
                p->v3 += 0x10;
            }
            if ((signed char)++pSt->Unk3C >= 4) {
                pSt->Unk3C = 0;
                pSt->Unk3A++;
            }
        }
        if (pSt->Unk3B == 0) {
            if (pSt->Unk3D < 3) {
                pSt->Pos[3].X -= 24;
            }
            if (pSt->Unk3D >= 1 && pSt->Unk3D < 4) {
                pSt->Pos[4].X -= 24;
            }
            if (pSt->Unk3D >= 2 && pSt->Unk3D < 5) {
                pSt->Pos[5].X -= 24;
            }
            if (pSt->Unk3D >= 3 && pSt->Unk3D < 6) {
                pSt->Pos[6].X -= 24;
            }
            if (f & 0x10) {
                if (pSt->Unk3D >= 4 && pSt->Unk3D < 7) {
                    pSt->Pos[7].X -= 24;
                }
                    if ((signed char)++pSt->Unk3D >= 8) {
                pSt->Unk3A = 0;
                pSt->Unk3B = 0;
                pSt->Unk3C = 0;
                pSt->Unk3D = 0;
                return 1;
            }
            } else if ((signed char)++pSt->Unk3D >= 7) {
                pSt->Unk3A = 0;
                pSt->Unk3B = 0;
                pSt->Unk3C = 0;
                pSt->Unk3D = 0;
                return 1;
            }
        }
        return 0;
    }
    f = pSt->Unk42.Flg;
    switch (pSt->Unk3A) {
    case 0:
        if ((signed char)++pSt->Unk3C >= 4) {
            pSt->Unk3C = 0;
            pSt->Unk3A++;
        }
        break;
    case 1:
        p = (POLY_FT4 *)0x801AE5A8;
        if (G.Cbuf_id) {
            p = (POLY_FT4 *)0x801AE5D0;
        }
        for (i = 0; i < 3; i++, p += 2) {
            p->v0 -= 0x10;
            p->v1 -= 0x10;
            p->v2 -= 0x10;
            p->v3 -= 0x10;
        }
        if ((signed char)++pSt->Unk3C >= 4) {
            pSt->Unk3A = 0;
            pSt->Unk3B = 0;
            pSt->Unk3C = 0;
            pSt->Unk3D = 0;
            return 1;
        }
        break;
    }
    if (pSt->Unk3B == 0) {
        if (pSt->Unk3D < 3) {
            pSt->Pos[4].X += 24;
        }
        if (pSt->Unk3D >= 1 && pSt->Unk3D < 4) {
            pSt->Pos[5].X += 24;
        }
        if (pSt->Unk3D >= 2 && pSt->Unk3D < 5) {
            pSt->Pos[6].X += 24;
        }
        if (f & 0x10) {
            if (pSt->Unk3D >= 3 && pSt->Unk3D < 6) {
                pSt->Pos[7].X += 24;
            }
            if (pSt->Unk3D >= 4 && pSt->Unk3D < 7) {
                pSt->Pos[3].X += 24;
            }
            if ((signed char)++pSt->Unk3D >= 8) {
                pSt->Unk3B++;
            }
        } else {
            if (pSt->Unk3D >= 3 && pSt->Unk3D < 6) {
                pSt->Pos[3].X += 24;
            }
            if ((signed char)++pSt->Unk3D >= 6) {
                pSt->Unk3B++;
            }
        }
    }
    return 0;
}

void func_80069A6C(ST_WORK *pSt)
{
    tagGLOBAL *pG;
    UCHAR i;
    LONG k;
    LONG j;
    UCHAR c;

    if (pSt->Unk13 == 0) {
        pG = &G;
        i = pSt->Rtn2;
        if (pSt->Rtn2 >= pG->pUnk7C7C->Num - 1) {
            pSt->Rtn1 = 3;
            return;
        }
        k = i;
        pG->pUnk7C7C->Item[k].Id = pG->pUnk7C7C->Item[j = k + 1].Id;
        pG->pUnk7C7C->Item[k].Num = pG->pUnk7C7C->Item[j].Num;
        pG->pUnk7C7C->Item[k].Flg = pG->pUnk7C7C->Item[j].Flg;
        pG->pUnk7C7C->Item[j].Id = 0;
        pG->pUnk7C7C->Item[j].Num = 0;
        pG->pUnk7C7C->Item[j].Flg = 0;
        func_8006AD38(k, i + 1);
        if (pG->pUnk7C7C->Unk128 == j) {
            pG->pUnk7C7C->Unk128 = k;
            pG->pUnk7C7C->Unk129 = pG->pUnk7C7C->Item[pG->pUnk7C7C->Unk128].Id;
        }
        if (pSt->Unk1C == j) {
            pSt->Unk1C = c = pSt->Unk1C - 1;
            pSt->Pos[12].X = (c & 1) * 40;
            pSt->Pos[12].Y = (pSt->Unk1C >> 1) * 30;
        }
        pSt->Rtn2++;
        if (pG->pUnk7C7C->Item[k + 2].Id == 0) {
            pSt->Rtn1 = 3;
        }
        pSt->Unk13 = 2;
    }
    pSt->Unk13--;
}
#undef IT

signed char Search_item_id_u(UCHAR id, UCHAR i) __asm__("Search_item_id"); /* arg 2 UC (canonical signed char) */

void func_80069C3C(ST_WORK *pSt)
{
    UCHAR *pA = G.pAot_data;
    ST_AOT_ITEM *pD = G.pUnk21DC;
    UCHAR *p;
    UCHAR id;
    signed char n;
    UCHAR m;
    LONG dx, dy;
    UCHAR *pI;
    USHORT f;

    switch (pSt->Rtn2) {
    case 0:
        pSt->Unk73 = 0;
        pI = D_800A0514[pD->Id];
        id = pD->Id;
        n = 0;
        switch (pI[0]) {
        case 3:
            if (id != 0x20) {
                break;
            }
            id = 0x2A;
        case 2:
        case 5:
        case 6:
            do {
                n = Search_item_id_u(id, n);
                if (n == -1) {
                    break;
                }
                p = (UCHAR *)&G.pUnk7C7C->Item[n];
                if (D_800A0514[p[0]][1] >= p[1] + pD->Num) {
                    pSt->Unk6E = n;
                    pSt->Unk73 = 1;
                    D_800DBB5B = pD->Id;
                    Mess_set(0, 0x1400, 0, 0);
                    pSt->Rtn2 = 2;
                    break;
                }
                n++;
            } while (1);
            break;
        }
        if (pSt->Unk73 == 0) {
            n = Search_item_id_u(0, 0);
            if (n == -1) {
                Mess_set(0, 0x1000, 1, 0);
                pSt->Rtn2 = 1;
                break;
            }
            pSt->Unk6E = n;
            if (Flg_ck(G.Unk77F8, 9) && (UCHAR)pD->Id == 0xC) {
                D_800DBB5B = 0x14;
            } else {
                D_800DBB5B = pD->Id;
            }
            Mess_set(0, 0x1400, 0, 0);
            pSt->Rtn2 = 2;
        }
        break;
    case 1:
        if (!(D_800DBB58 & 0x80)) {
            pSt->Rtn0++;
            Snd_se_stad(5, 0, 0, 0);
        }
        break;
    case 2:
        if (!(D_800DBB58 & 0x80)) {
            switch (G.Unk7848) {
            case 0: {
                UCHAR k;

                Snd_se_stad(6, 0, 0, 0);
                pSt->Rtn2 = 3;
                pSt->Pos[15].X = 0x70;
                k = pSt->Unk6E;
                pSt->Pos[15].Y = 0x7C;
                pSt->Unk13 = 0;
                pSt->Unk6F = 0x38;
                pSt->Unk70 = 0x24;
                dx = (k & 1) * 20 + 0xF4;
                dy = ((signed char)k / 2) * 30 + 0x51;
                pSt->Unk71 = (dx - pSt->Pos[15].X) / 10;
                pSt->Flg |= 0x400000;
                pSt->Unk72 = (dy - pSt->Pos[15].Y) / 10;
                break;
            }
            case 1:
                pSt->Rtn0++;
                Snd_se_stad(5, 0, 0, 0);
                break;
            }
        }
        break;
    case 3:
        if (pSt->Unk13 < 10) {
            pSt->Pos[15].X += pSt->Unk71;
            pSt->Pos[15].Y += pSt->Unk72;
            pSt->Unk6F -= 3;
            pSt->Unk70 -= 2;
        } else {
            p = (UCHAR *)&G.pUnk7C7C->Item[(signed char)pSt->Unk6E];
            switch (pSt->Unk73) {
            case 0:
                func_8006AE20(pSt->Unk6E, 12);
                pI = D_800A0514[pD->Id];
                p[0] = pD->Id;
                p[1] = pD->Num;
                ((USHORT *)p)[1] = f = pI[3];
                if (p[0] == 0xE) {
                    ((USHORT *)p)[1] = f | 0x100;
                }
                if (Flg_ck(G.Unk77F8, 9) && (ULONG)(p[0] - 2) < 0x13) {
                    ((USHORT *)p)[1] |= 3;
                    if (p[0] == 0xC) {
                        p[0] = 0x14;
                        ((USHORT *)p)[1] = 7;
                    }
                }
                break;
            case 1:
                p[1] += pD->Num;
                break;
            }
            G.Sce_work[3] = pD->Id;
            *pA = 0;
            m = pD->Md1;
            if ((ULONG)((USHORT)G.Unk783E - 2) >= 3) {
                Flg_on(G.Unk78D0, pD->Flg);
            } else {
                Flg_on(G.Unk78F0, pD->Flg);
            }
            Esp_kill(0x705, &G.Ob_model[m].M);
            Esp_kill(0x905, 0);
            if (!(m & 0x80)) {
                G.Ob_model[m].Be_flg = 0;
            }
            if (pD->Flg >= 0xF0) {
                G.Unk7DC0[pD->Flg - 0xF0].Unk0 = 0;
            }
            Mess_set(0, 0x1000, 6, 0);
            pSt->Rtn2 = 1;
            pSt->Flg &= ~0x400000;
        }
        pSt->Unk13++;
        break;
    }
}

void func_8006A234(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    ITEM_WORK *p;
    UCHAR *pI;
    UCHAR n;
    LONG dx, dy;
    switch (pSt->Rtn2) {
    case 0:
        func_8006AC88(pSt->Unk76);
        func_8006A9AC(12, pSt->Unk76);
        pSt->Unk13 = 0;
        pSt->Rtn2++;
        Snd_se_stad(6, NULL, 0, 0);
        break;
    case 1:
        if (pSt->Unk13 < 12) {
            pSt->Pos[8].X += 0x12;
        } else {
            pSt->Rtn2++;
            pSt->Flg |= 0x20000000;
            p = &pG->pUnk7C7C->Item[(signed char)pSt->Unk77];
            p->Id = pSt->Unk76;
        }
        pSt->Unk13++;
        break;
    case 2:
        if (D_800CC840 & 0x3000) {
            Snd_se_stad(6, NULL, 0, 0);
            n = pSt->Unk77;
            pSt->Unk6F = 0x38;
            pSt->Unk70 = 0x24;
            pSt->Pos[15].X = 0x70;
            pSt->Unk13 = 0;
            pSt->Pos[15].Y = 0x7C;
            dx = (n & 1) * 20 + 0xF4;
            dy = ((signed char)n / 2) * 30 + 0x51;
            pSt->Unk71 = (dx - pSt->Pos[15].X) / 10;
            pSt->Rtn2++;
            pSt->Flg = (pSt->Flg | 0x400000) & ~0x20000000;
            pSt->Unk72 = (dy - pSt->Pos[15].Y) / 10;
        }
        break;
    case 3:
        if (pSt->Unk13 < 10) {
            pSt->Pos[15].X += pSt->Unk71;
            pSt->Pos[15].Y += pSt->Unk72;
            pSt->Unk6F -= 3;
            pSt->Unk70 -= 2;
        pSt->Unk13++;
            return;
        }
        p = &pG->pUnk7C7C->Item[(signed char)pSt->Unk77];
        pI = D_800A0514[pSt->Unk76];
        func_8006AE20(pSt->Unk77, 12);
        p->Id = pSt->Unk76;
        p->Num = pI[1];
        p->Flg = pI[3];
        pSt->Rtn2++;
        pSt->Flg &= ~0x400000;
        break;
    case 4:
        if (D_800CC840 & 0x3000) {
            Snd_se_stad(5, NULL, 0, 0);
            pSt->Unk13 = 0;
            pSt->Rtn2++;
        }
        break;
    case 5:
        if (pSt->Unk13 < 12) {
            pSt->Pos[8].X -= 0x12;
        } else {
            pSt->Rtn1 = 0;
            pSt->Rtn2 = 0;
            pSt->Unk22 = 0;
            pSt->Unk24 = 0;
            pSt->Flg |= 0x20000000;
        }
        pSt->Unk13++;
    }
}

void func_8006A59C(ST_WORK *pSt)
{
    UCHAR *pE;
    ULONG i;
    signed char c, n;

    if (pSt->Unk13 < 12) {
        pSt->Pos[8].X -= 0x12;
        pSt->Pos[11].Y += 6;
        pSt->Pos[0].X -= 0xE;
        pSt->Pos[1].X += 0xE;
        pSt->Pos[2].X += 0xE;
        pSt->Pos[3].X += 0xE;
        pSt->Pos[4].X += 0xE;
        pSt->Pos[5].X += 0xE;
        pSt->Pos[6].X += 0xE;
        pSt->Pos[7].X += 0xE;
        pSt->Pos[12].X += 0xE;
        pSt->Unk13++;
        return;
    }
    pE = pSt->UnkE0;
    pSt->Unk20 = pE[3];
    c = D_800A00B0[pSt->Unk20];
    pSt->UnkBC = c;
    pSt->Flg &= ~0x1000000;
    pSt->Flg &= ~0x2000;
    pSt->Flg &= ~0x4000;
    pSt->Flg |= 0x1000;
    pSt->UnkBD = 0;
    pSt->UnkC1 = pE[3] / 15;
    pSt->Rtn0 = 7;
    pSt->Pos[14].X = pSt->Pos[24].X = -0xD8;
    pSt->Rtn1 = 0;
    pSt->Rtn2 = 0;
    pSt->Unk13 = 0;
    pSt->Pos[14].Y = 0;
    pSt->Pos[14].X += (signed char)(pSt->Unk20 % 5) * 32;
    pSt->Pos[14].Y += (signed char)((signed char)(pSt->Unk20 % 15) / 5) * 32;
    if (pSt->UnkC1) {
        pSt->Pos[14].X += 0x20;
    }
    func_8006CD68(pSt->Unk1C);
    Flg_on(&G.Unk79EC[2], pE[2] - 0x85);
    for (i = 0; i < 30; i++) {
        if (D_800A00B0[i] == pSt->UnkBC) {
            break;
        }
    }
    if (G.Unk79EC[3] == i) {
        G.Unk79EC[3]++;
    }
    func_8006AE20(pSt->Unk1C, 11);
    for (c = pSt->Unk1C; c < 9; c = n) {
        n = c + 1;
        func_8006AD38(c, n);
    }
    pSt->Unk1C = -2;
}

void func_8006A888(ST_WORK *pSt)
{
    pSt->Rtn0 = 0xD;
    pSt->Rtn1 = 0;
}

UCHAR *func_8006A898(ST_WORK *pSt, UCHAR a, UCHAR b)
{
    UCHAR *p = D_800A07C4;
    UCHAR c = p[1];

    if (c != 0xFF) {
        do {
            if (c == a && p[2] == b) {
                return p;
            }
            if (p[1] == b && p[2] == a) {
                return p;
            }
            p += 8;
            c = p[1];
        } while (c != 0xFF);
    }
    return (UCHAR *)-1;
}

UCHAR *func_8006A918(ST_WORK *pSt, UCHAR id)
{
    UCHAR *p = D_800A0BB4;

    while (p[1] != 0xFF) {
        if (p[1] == id) {
            return p;
        }
        p += 4;
    }
    return (UCHAR *)-1;
}

UCHAR *func_8006A95C(ST_WORK *pSt, UCHAR id)
{
    UCHAR *p = D_800A0BC4;

    while (p[0] != 0xFF) {
        if (p[0] == id) {
            return p;
        }
        p += 4;
    }
    return (UCHAR *)-1;
}

void func_8006A9AC(UCHAR no, UCHAR idx)
{
    RECT r;
    ST_WORK *pSt = &St;

    CD.Fno = 0x34;
    CD.Fsize = 0x800;
    CD.Fsector = (floc[0x34].sec_high << 16) + floc[0x34].sector + idx;
    CD.Fsum = D_8009F568[idx];
    Cd_read(0x34, pSt->pBuf, 2, D_80010F84);
    r.x = D_800A004C[no][0];
    r.y = D_800A004C[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pSt->pBuf);
}

void func_8006AA84(UCHAR no)
{
    LONG pad[2];

    CD.Fno = 0x34;
    CD.Fsize = 0x800;
    CD.Fsector = (floc[0x34].sec_high << 16) + floc[0x34].sector + no;
    CD.Fsum = D_8009F568[no];
    Cd_read(0x34, St.pBuf, 2, D_80010F84);
}

void func_8006AB08(UCHAR no)
{
    RECT r;

    r.x = D_800A004C[no][0];
    r.y = D_800A004C[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, St.pBuf);
}

void func_8006AB68(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf)
{
    RECT r;

    func_80010000(ofs + D_8009F678[tbl], pBuf);
    r.x = D_800A004C[no][0];
    r.y = D_800A004C[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pBuf);
}

void func_8006ABF8(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf)
{
    RECT r;

    func_80010000(ofs + D_8009F678[tbl], pBuf);
    r.x = D_8009F4F8[no % 8][0];
    r.y = D_8009F4F8[no % 8][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pBuf);
}

INCLUDE_RODATA("asm/nonmatchings/game/800636D4", D_80010F84);

void func_8006AC88(UCHAR no)
{
    ST_WORK *pSt = &St;
    LONG pad[2];

    CD.Fno = 0x33;
    CD.Fsize = 0x2800;
    CD.Fsector = (floc[0x33].sec_high << 16) + floc[0x33].sector + no * 5;
    CD.Fsum = D_8009F5F0[no];
    Cd_read(0x33, pSt->pBuf, 2, "ITEMG.PIX");
    D_800CCBBC = 0x817;
    Mapping_tim(pSt->pBuf);
    pSt->UnkD4 = 8;
}

void func_8006AD38(UCHAR a, UCHAR b)
{
    RECT r;
    ST_WORK *pSt = &St;
    USHORT (*tbl)[2] = D_800A004C;
    USHORT *pa = tbl[a];
    USHORT *pb;

    r.x = pa[0];
    r.y = pa[1];
    r.w = 0x14;
    r.h = 0x1E;
    StoreImage(&r, pSt->pBuf);
    DrawSync(0);
    pb = tbl[b];
    r.x = pb[0];
    r.y = pb[1];
    MoveImage(&r, pa[0], pa[1]);
    DrawSync(0);
    r.x = pb[0];
    r.y = pb[1];
    LoadImage(&r, pSt->pBuf);
    DrawSync(0);
}

void func_8006AE20(UCHAR dst, UCHAR src)
{
    RECT r;

    r.x = D_800A004C[src][0];
    r.y = D_800A004C[src][1];
    r.w = 0x14;
    r.h = 0x1E;
    MoveImage(&r, D_800A004C[dst][0], D_800A004C[dst][1]);
    DrawSync(0);
}

void func_8006AE90(ST_WORK *pSt)
{
    ULONG i;
    UCHAR c;

    for (i = 0; i < 8; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x9B);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x9B);
    }
    for (i = 8; i < 16; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x9A);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x9A);
    }
    for (i = 16; i < 24; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x3A);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x3A);
    }
    for (i = 24; i < 32; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x97);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x97);
    }
    for (i = 32; i < 40; i++) {
        SetDrawTPage(&pSt->Ot[0][i], 0, 0, 0x17);
        SetDrawTPage(&pSt->Ot[1][i], 0, 0, 0x17);
    }
    c = pSt->UnkD0;
    func_8006E600(pSt, (void *)0x801AC000, D_8009F890, (c << 8) | 2);
    func_8006E600(pSt, (void *)0x801AC1B8, D_8009F8CC, (c << 8) | 5);
    switch (G.Player.Id) {
    case 8:
    case 9:
    case 10:
        func_8006E600(pSt, (void *)0x801AC280, D_8009F95C, (c << 8) | 7);
        break;
    default:
        func_8006E600(pSt, (void *)0x801AC280, D_8009F908, (c << 8) | 7);
        break;
    }
    func_8006E600(pSt, (void *)0x801ACA78, D_8009FBCC, (c << 8) | 4);
    func_8006E600(pSt, (void *)0x801ACB18, D_8009FBFC, (c << 8) | 4);
    func_8006E600(pSt, (void *)0x801ACBB8, D_8009FC2C, (c << 8) | 1);
    func_8006E600(pSt, (void *)0x801ACBE0, D_8009FC38, (c << 8) | 1);
    func_8006E600(pSt, (void *)0x801ACC08, D_8009FC44, (c << 8) | 1);
    func_8006E600(pSt, (void *)0x801ACC30, D_8009FC50, (c << 8) | 1);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801ADBA8, D_8009FC5C, (c << 8) | 0x7000008);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801ADE28, D_8009FCBC, (c << 8) | 0x7000008);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AE0A8, D_8009FD1C, (c << 8) | 0x7000008);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AE328, D_8009FD7C, (c << 8) | 0x7000008);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AE5A8, D_8009F9B0, (c << 8) | 0x4000003);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AE698, D_8009FDDC, (c << 8) | 0x7000004);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AE7D8, D_8009FE0C, (c << 8) | 0x7000004);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AE918, D_8009FE3C, (c << 8) | 0x7000004);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AEAA8, D_8009FE6C, (c << 8) | 0x7000004);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AEBE8, D_8009FE9C, (c << 8) | 0x7000004);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AED28, D_8009FECC, (c << 8) | 0x7000004);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AEE68, D_8009FEFC, (c << 8) | 0x7000004);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AEFA8, D_8009FF2C, (c << 8) | 0x7000001);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AEFF8, D_8009FF38, (c << 8) | 0x7000001);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AF048, D_8009FF44, (c << 8) | 0x7000001);
    func_8006E6D8(pSt, (POLY_FT4 *)0x801AF098, D_8009FF50, (c << 8) | 0x7000001);
    func_8006E7D4(pSt, (TILE *)0x801AF4E8, D_8009FF5C, 1);
    func_8006E7D4(pSt, (TILE *)0x801AF508, D_8009FB90, 2);
    c = pSt->UnkD1;
    func_8006E600(pSt, (void *)0x801AC398, D_8009F9D4, (c << 8) | 3);
    func_8006E600(pSt, (void *)0x801AC460, D_8009FBC0, (c << 8) | 1);
    c = pSt->UnkD2;
    func_8006E600(pSt, (void *)0x801AC050, D_8009F8A8, (c << 8) | 1);
    func_8006E600(pSt, (void *)0x801AC078, D_8009F8B4, ((UCHAR)(c + 1) << 8) | 1);
    func_8006E600(pSt, (void *)0x801AC0A0, D_8009F8C0, ((UCHAR)(c + 2) << 8) | 1);
    c = pSt->UnkD3 + 2;
    func_8006E600(pSt, (void *)0x801ACA28, D_8009FF74, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ADB58, D_8009FB78, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ADB30, D_8009FB84, (c << 8) | 0x10001);
    c = pSt->UnkD3 + 3;
    func_8006E600(pSt, (void *)0x801ACA00, D_8009FF68, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ACA50, D_8009FF80, (c << 8) | 0x10001);
    c = pSt->UnkD3;
    func_8006E600(pSt, (void *)0x801AC488, D_8009FF8C, (c << 8) | 0x10003);
    func_8006E600(pSt, (void *)0x801ACC58, D_8009FFB0, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ACC80, D_8009FFBC, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ACCA8, D_8009FFC8, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ACCD0, D_8009FFD4, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ACCF8, D_8009FFE0, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ACD20, D_8009FFEC, (c << 8) | 0x10001);
    func_8006E600(pSt, (void *)0x801ACD48, D_8009FFF8, (c << 8) | 0x10001);
    c = pSt->UnkD3 + 1;
    func_8006E600(pSt, (void *)0x801AC0C8, D_800A0004, (c << 8) | 0x10006);
    func_8006E600(pSt, (void *)0x801AC410, D_8009FBA8, (pSt->UnkD4 << 8) | 2);
    func_8006E84C(pSt);
}

extern UCHAR D_800A0004_t[][12] __asm__("D_800A0004"); /* view of D_800A0004 with another type (canonical: extern UCHAR D_800A0004[];) */

void func_8006B66C(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    SPRT *p;
    USHORT *pT;
    POLY_FT4 *pFt;
    ITEM_WORK *pI;
    UCHAR *pInfo;
    ULONG i;
    LONG n;
    signed char k;
    UCHAR id;
    USHORT x, y;
    ULONG xy;
    tagGLOBAL *pG2;
    LONG no;
    ULONG name;
    UCHAR c;

    p = (SPRT *)0x801ACA00;
    n = 8;
    if (pSt->Rtn1 == 0) {
        if (pG->Cbuf_id) {
            p = (SPRT *)0x801ACA14;
        }
        setRGB0(p, pSt->Unk22 - 0x80, pSt->Unk22 - 0x80, pSt->Unk22 - 0x80);
    } else {
        setRGB0(&p[0], 0x80, 0x80, 0x80);
        setRGB0(&p[1], 0x80, 0x80, 0x80);
    }
    p = (SPRT *)0x801ACA28;
    if ((*(ULONG *)&pSt->Rtn0 & 0xFFFF00) == 0x10700) {
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ACA3C;
        }
        setRGB0(p, pSt->Unk22 - 0x80, pSt->Unk22 - 0x80, pSt->Unk22 - 0x80);
    } else {
        setRGB0(&p[0], 0x80, 0x80, 0x80);
        setRGB0(&p[1], 0x80, 0x80, 0x80);
    }
    p = (SPRT *)0x801ACA50;
    if ((*(ULONG *)&pSt->Rtn0 & 0xFFFF00) == 0x30500) {
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ACA64;
        }
        setRGB0(p, pSt->Unk22 - 0x80, pSt->Unk22 - 0x80, pSt->Unk22 - 0x80);
    } else {
        setRGB0(&p[0], 0x80, 0x80, 0x80);
        setRGB0(&p[1], 0x80, 0x80, 0x80);
    }
    for (i = 0; i < 8; i++) {
        AddPrim(G.pOt0 + i, &pSt->Ot[G.Cbuf_id][i + n * 4]);
        AddPrim(G.pOt0 + i, &pSt->Ot[G.Cbuf_id][i + n * 3]);
        AddPrim(G.pOt0 + i, &pSt->Ot[G.Cbuf_id][i + n * 2]);
        AddPrim(G.pOt0 + i, &pSt->Ot[G.Cbuf_id][i + n]);
        AddPrim(G.pOt0 + i, &pSt->Ot[G.Cbuf_id][i]);
    }
    pI = &G.pUnk7C7C->Item[pSt->Unk1C];
    pInfo = D_800A0514[pI->Id];
    func_8006E8BC(pSt, (SPRT *)0x801AC000, D_8009F890, 0x30002);
    switch ((ULONG)G.Item_work_no) {
    case 0:
        func_8006E8BC(pSt, (SPRT *)0x801AC050, D_8009F8A8, ((n | 4) << 16) | 1);
        break;
    case 1:
        func_8006E8BC(pSt, (SPRT *)0x801AC078, D_8009F8B4, ((n | 4) << 16) | 1);
        break;
    case 2:
        func_8006E8BC(pSt, (SPRT *)0x801AC0A0, D_8009F8C0, ((n | 4) << 16) | 1);
        break;
    }
    func_8006E8BC(pSt, (SPRT *)0x801AC0C8 + pSt->Unk34 * 2, D_800A0004_t[pSt->Unk34], ((n * 2 + 2) << 16) | 1);
    func_8006E8BC(pSt, (SPRT *)0x801AC460, D_8009FBC0, ((n | 2) << 16) | 0x101);
    func_8006E8BC(pSt, (SPRT *)0x801AC1B8, D_8009F8CC, 0x30105);
    func_8006E8BC(pSt, (SPRT *)0x801AC398, D_8009F9D4, ((n | 2) << 16) | 0x103);
    func_8006E8BC(pSt, (SPRT *)0x801AC488, D_8009FF8C, ((n * 2 + 2) << 16) | 0x103);
    func_8006E9D4(pSt, (void *)0x801AE5A8, D_8009F9B0, 0x70103);
    func_8006E9D4(pSt, (void *)0x801ADBA8, D_8009FC5C, 0x70808);
    func_8006E9D4(pSt, (void *)0x801AEFA8, D_8009FF2C, 0x70801);
    func_8006E8BC(pSt, (SPRT *)0x801AC410, D_8009FBA8, (((n * 3) | 1) << 16) | 0x802);
    if (pSt->Flg & 0x1000) {
        func_8006EAF0(pSt, (TILE *)0x801AF508, D_8009FB90, 0x70902);
        func_8006E9D4(pSt, (void *)0x801ADE28, D_8009FCBC, 0x70908);
        func_8006CA10(pSt);
        if (pSt->Flg & 0x2000) {
            func_8006E8BC(pSt, (SPRT *)0x801ADB58, D_8009FB78, ((n * 2 + 1) << 16) | 0x1701);
        }
        if (pSt->Flg & 0x4000) {
            func_8006E8BC(pSt, (SPRT *)0x801ADB30, D_8009FB84, ((n * 2 + 1) << 16) | 0x1601);
        }
        if (pSt->Flg & 0x1000000) {
            func_8006E8BC(pSt, (SPRT *)0x801ACA50, D_8009FF80, ((n * 2 + 1) << 16) | 0xE01);
            k = pSt->Unk20;
            id = D_800A00B0[k];
            if (G.Unk79EC[3] == 30 && k == 0) {
                id = 30;
            }
            if (Flg_ck(&G.Unk79EC[2], id)) {
                Name_disp(0xE, 0xC8, 0x501, id + 0x85);
            }
        }
        func_8006E9D4(pSt, (void *)0x801AE0A8, D_8009FD1C, 0x70A08);
        func_8006E9D4(pSt, (void *)0x801AF048, D_8009FF44, 0x70A01);
    }
    if (pSt->Flg & 0x400000) {
        func_8006C370(pSt);
    }
    func_8006E9D4(pSt, (void *)0x801AE328, D_8009FD7C, 0x70B08);
    func_8006E9D4(pSt, (void *)0x801AF098, D_8009FF50, 0x70B01);
    if (pSt->Unk42.Flg & 0x10) {
        func_8006E8BC(pSt, (SPRT *)0x801ACA78, D_8009FBCC, 0x60304);
    } else {
        func_8006E8BC(pSt, (SPRT *)0x801ACB18, D_8009FBFC, 0x60304);
    }
    func_8006E8BC(pSt, (SPRT *)0x801ACBB8, D_8009FC2C, 0x40401);
    func_8006E8BC(pSt, (SPRT *)0x801ACBE0, D_8009FC38, 0x40501);
    func_8006E8BC(pSt, (SPRT *)0x801ACC08, D_8009FC44, 0x40601);
    if (pSt->Unk42.Flg & 0x10) {
        func_8006E8BC(pSt, (SPRT *)0x801ACC30, D_8009FC50, 0x40701);
    }
    if (pInfo[0] == 1) {
        func_8006E8BC(pSt, (SPRT *)0x801ACC58, D_8009FFB0, ((n * 2 + 4) << 16) | 0x401);
    } else {
        func_8006E8BC(pSt, (SPRT *)0x801ACC80, D_8009FFBC, ((n * 2 + 4) << 16) | 0x401);
    }
    func_8006E8BC(pSt, (SPRT *)0x801ACCA8, D_8009FFC8, ((n * 2 + 4) << 16) | 0x501);
    func_8006E8BC(pSt, (SPRT *)0x801ACCF8, D_8009FFE0, ((n * 2 + 4) << 16) | 0x601);
    if (pSt->Unk42.Flg & 0x10) {
        if (pI->Flg & 0x100) {
            func_8006E8BC(pSt, (SPRT *)0x801ACD48, D_8009FFF8, ((n * 2 + 4) << 16) | 0x701);
        } else {
            func_8006E8BC(pSt, (SPRT *)0x801ACD20, D_8009FFEC, ((n * 2 + 4) << 16) | 0x701);
        }
    }
    pT = D_8009FE6C;
    pFt = (POLY_FT4 *)0x801AEAA8;
    if ((signed char)pSt->Unk1E == 0) {
        pT += 12;
        pFt = (POLY_FT4 *)0x801AEB48;
    }
    func_8006E9D4(pSt, pFt, pT, 0x50402);
    pT = D_8009FE9C;
    pFt = (POLY_FT4 *)0x801AEBE8;
    if ((signed char)pSt->Unk1E == 1) {
        pT += 12;
        pFt = (POLY_FT4 *)0x801AEC88;
    }
    func_8006E9D4(pSt, pFt, pT, 0x50502);
    pT = D_8009FECC;
    pFt = (POLY_FT4 *)0x801AED28;
    if ((signed char)pSt->Unk1E == 2) {
        pT += 12;
        pFt = (POLY_FT4 *)0x801AEDC8;
    }
    func_8006E9D4(pSt, pFt, pT, 0x50602);
    if (pSt->Unk42.Flg & 0x10) {
        pT = D_8009FEFC;
        pFt = (POLY_FT4 *)0x801AEE68;
        if ((signed char)pSt->Unk1E == 3) {
            pT += 12;
            pFt = (POLY_FT4 *)0x801AEF08;
        }
        func_8006E9D4(pSt, pFt, pT, 0x50702);
    }
    p = (SPRT *)0x801ACD70;
    if (G.Cbuf_id) {
        p = (SPRT *)0x801ACD84;
    }
    i = 0;
    if (G.Item_work[0].Num) {
        do {
            pI = &pG->pUnk7C7C->Item[i];
            x = D_800A0080[i][0] + pSt->Pos[1].X;
            y = D_800A0080[i][1] + pSt->Pos[1].Y;
            xy = x | (y << 16);
            c = pSt->UnkD3 + ((pI->Flg & 0xC) >> 2) + 2;
            p = func_8006C6D0(p, pI->Num, xy, c | (((UCHAR)pI->Flg & 3) << 8) | ((n * 2 + 2) << 16));
        } while (++i < D_800D225E);
    }
    if ((i = pG->pUnk7C7C->Unk128) != 0xFF) {
        pI = &pG->pUnk7C7C->Item[i];
        c = pSt->UnkD3 + ((pI->Flg & 0xC) >> 2) + 2;
        x = D_800A0080[10][0] + pSt->Pos[1].X;
        y = D_800A0080[10][1] + pSt->Pos[1].Y;
        func_8006C6D0(p, pI->Num, x | (y << 16), c | (((UCHAR)pI->Flg & 3) << 8) | ((n * 2 + 2) << 16));
    }
    pT = D_8009FDDC;
    pFt = (POLY_FT4 *)0x801AE698;
    if (pSt->Unk1C == -2) {
        pT += 12;
        pFt = (POLY_FT4 *)0x801AE738;
    }
    func_8006E9D4(pSt, pFt, pT, 0x40102);
    pT = D_8009FE0C;
    pFt = (POLY_FT4 *)0x801AE7D8;
    if (pSt->Unk1C == -1) {
        pT += 12;
        pFt = (POLY_FT4 *)0x801AE878;
    }
    func_8006E9D4(pSt, pFt, pT, 0x40102);
    pT = D_8009FE3C;
    pFt = (POLY_FT4 *)0x801AE918;
    if (pSt->Unk1C < -2) {
        pT += 12;
        pFt = (POLY_FT4 *)0x801AE9B8;
    }
    func_8006E9D4(pSt, pFt, pT, 0x40102);
    if ((pSt->Flg & 0x4000000) && pSt->Unk1C >= 0) {
        func_8006E8BC(pSt, (SPRT *)0x801ACA00, D_8009FF68, ((n * 2 + 1) << 16) | 0xC01);
    }
    pG2 = &G;
    if (pSt->Flg & 0x2000000) {
        func_8006E8BC(pSt, (SPRT *)0x801ACA28, D_8009FF74, ((n * 2 + 1) << 16) | 0xD01);
    }
    if (pG2->pUnk7C7C->Num == 8) {
        func_8006E8BC(pSt, (SPRT *)0x801AC280, D_8009F908, 0x10207);
        func_8006EAF0(pSt, (TILE *)0x801AF4E8, D_8009FF5C, 0x10201);
    }
    if (pSt->Flg & 0x20000000) {
        if (pSt->Flg & 0x2000000) {
            no = (signed char)pSt->Unk1D;
            pI = &pG2->pUnk7C7C->Item[no];
        } else {
            no = pSt->Unk1C;
            pI = &pG2->pUnk7C7C->Item[no];
        }
        name = pI->Id;
        if (no >= 0) {
            Name_disp(0xE, 0xAD, 0x501, name);
        }
    }
    func_8006C484(pSt);
}

void func_8006C370(ST_WORK *pSt)
{
    POLY_FT4 *p;
    SHORT w, h;
    SHORT x, y;

    p = (POLY_FT4 *)0x801AEA58;
    if (G.Cbuf_id) {
        p = (POLY_FT4 *)0x801AEA80;
    }
    setPolyFT4(p);
    setRGB0(p, 0x80, 0x80, 0x80);
    setUV4(p, 0, 0, 0x70, 0, 0, 0x48, 0x70, 0x48);
    p->tpage = 0x97;
    p->clut = (pSt->UnkD4 + 0x1E0) << 6;
    w = (signed char)pSt->Unk6F;
    h = (signed char)pSt->Unk70;
    x = pSt->Pos[15].X + pSt->Pos[8].X;
    y = pSt->Pos[15].Y + pSt->Pos[8].Y;
    setXY4(p, x - w, y - h, x + w, y - h, x - w, y + h, x + w, y + h);
    AddPrim(G.pOt0 + 1, p);
}

void func_8006C484(ST_WORK *pSt)
{
    LINE_F2 *p;
    UCHAR *pTbl;
    ST_RGB_STEP *pC;
    UCHAR n;
    SHORT i;
    LONG x, y;
    LONG k;
    LONG ot;

    p = (LINE_F2 *)0x801AF0E8;
    pTbl = D_800A0174[pSt->Unk34];
    n = 0x20;
    if (pSt->Flg & 0x800000) {
        n = 0x1C;
    }
    ot = 1;
    if (G.Cbuf_id) {
        p = (LINE_F2 *)0x801AF0F8;
    }
    i = 0;
    x = pSt->Pos[0].X + 0x4B;
    y = pSt->Pos[0].Y + 0x25;
    pC = &D_800A0150[pSt->Unk34];
    for (; i < n; p += 2, i++) {
        if (pSt->Unk32 + i >= 0) {
            if (pSt->Unk32 + i < 0x4A) {
                k = n - i - 1;
                p->r0 = pC->R - pC->Dr * k;
                p->g0 = pC->G - pC->Dg * k;
                p->b0 = pC->B - pC->Db * k;
                p->x0 = i + (pSt->Unk32 + x);
                p->x1 = i + (pSt->Unk32 + x);
                if (pSt->Flg & 0x800000) {
                    p->y0 = y + 2;
                    p->y1 = y + 0x1D;
                } else {
                    p->y0 = pTbl[(pSt->Unk32 + i) * 2] + y;
                    p->y1 = pTbl[(pSt->Unk32 + i) * 2 + 1] + (pTbl[(pSt->Unk32 + i) * 2] + y);
                }
                AddPrim(&pSt->Ot[G.Cbuf_id][ot], p);
            }
        }
    }
}

SPRT *func_8006C6D0(SPRT *p, SHORT num, ULONG xy, ULONG prm)
{
    signed char d[4];
    UCHAR i;
    SHORT n;
    SHORT x = xy;
    SHORT y = xy >> 16;
    UCHAR mode = prm >> 8;
    UCHAR ot = prm >> 16;
    ST_WORK *pSt = &St;

    switch (mode) {
    case 0:
        return p;
    case 1:
        d[0] = num / 100;
        if (d[0] == 0) {
            d[0] = -1;
        }
        n = num % 100;
        d[1] = n / 10;
        if (d[1] == 0 && d[0] == -1) {
            d[1] = -1;
        }
        d[2] = n % 10;
        d[3] = -1;
        break;
    case 2:
        d[0] = num / 100;
        if (d[0] == 0) {
            d[0] = -1;
        }
        n = num % 100;
        d[1] = n / 10;
        if (d[1] == 0 && d[0] == -1) {
            d[1] = -1;
        }
        d[2] = n % 10;
        d[3] = 10;
        break;
    case 3:
        d[0] = 11;
        d[1] = 12;
        d[2] = -1;
        d[3] = -1;
        break;
    }
    for (i = 0; i < 4; i++) {
        if (d[i] != -1) {
            setSprt(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = d[i] * 8 + 4;
            p->x0 = x;
            x += 8;
            p->v0 = 0x13;
            p->w = 8;
            p->h = 0xB;
            p->clut = (((UCHAR)prm + 0x1E0) << 6) | 0x13;
            p->y0 = y;
            AddPrim(&pSt->Ot[G.Cbuf_id][ot], p);
            p += 2;
        }
    }
    return p;
}

void func_8006CA10(ST_WORK *pSt)
{
    SPRT *p;
    DR_TPAGE *pTp;
    ST_ICON *pI;
    ULONG i;
    LONG k;
    SHORT x, y;

    p = (SPRT *)0x801AC500;
    pTp = &pSt->Tp434[0];
    if (G.Cbuf_id) {
        pTp = &pSt->Tp434[1];
    }
    SetDrawTPage(pTp, 0, 0, 0x97);
    AddPrim(G.pOt0 + 7, pTp);
    i = 0;
    if (G.Cbuf_id) {
        p = (SPRT *)0x801AC514;
    }
    k = 24;
    for (; i < 30; p += 2, i++) {
        pI = &D_8009F9F8[D_800A00B0[i]];
        setSprt(p);
        setRGB0(p, 0x80, 0x80, 0x80);
        p->w = pI->W;
        p->h = pI->H;
        p->clut = (pSt->UnkD8 + 0x1E0) << 6;
        if (Flg_ck(&G.Unk79EC[2], D_800A00B0[i])) {
            p->u0 = pI->U;
            p->v0 = pI->V;
        } else {
            p->u0 = D_8009F9F8[31].U;
            p->v0 = D_8009F9F8[31].V;
        }
        if (G.Unk79EC[3] == 30 && i == 0) {
            p->u0 = D_8009F9F8[30].U;
            p->v0 = D_8009F9F8[30].V;
        }
        if (i < 15) {
            LONG ox = pSt->Pos[k].X;
            LONG oy;
            x = ox + ((i % 5) * 32 + 0x10);
            oy = pSt->Pos[k].Y;
            y = oy + ((i / 5) * 32 + 0x58);
        } else {
            LONG ox = pSt->Pos[k].X;
            LONG oy;
            x = ox + ((i % 5) * 32 + 0x10);
            x += 0xD0;
            oy = pSt->Pos[k].Y;
            y = oy + (((i - 15) / 5) * 32 + 0x58);
        }
        p->x0 = x;
        p->y0 = y;
        AddPrim(pTp, p);
    }
}

signed char Search_item_id(UCHAR id, signed char i)
{
    for (; (UCHAR)i < G.pUnk7C7C->Num; i++) {
        if (G.pUnk7C7C->Item[(UCHAR)i].Id == id) {
            return i;
        }
    }
    return -1;
}

signed char func_8006CCF0(UCHAR no, signed char n, UCHAR f)
{
    ITEM_WORK *p = &G.pUnk7C7C->Item[no];

    if ((p->Flg & 3) != 3) {
        p->Num -= n;
    }
    if (p->Num == 0 && f == 0) {
        func_8006CD68(no);
    }
    return (signed char)p->Num;
}

/* needs ITEM_SET item table typed as ITEM_WORK Item[0x4A] (typedef struct tagST_ITEM { UCHAR Id; UCHAR Num; USHORT Flg; } ITEM_WORK;) */
void func_8006CD68(UCHAR no)
{
    tagGLOBAL *pG = &G;
    ST_WORK *pSt = &St;
    UCHAR i;

    for (i = no; i < pG->pUnk7C7C->Num - 1; i++) {
        pG->pUnk7C7C->Item[i].Id = pG->pUnk7C7C->Item[i + 1].Id;
        pG->pUnk7C7C->Item[i].Num = pG->pUnk7C7C->Item[i + 1].Num;
        pG->pUnk7C7C->Item[i].Flg = pG->pUnk7C7C->Item[i + 1].Flg;
    }
    pG->pUnk7C7C->Item[i].Id = 0;
    pG->pUnk7C7C->Item[i].Num = 0;
    pG->pUnk7C7C->Item[i].Flg = 0;
    if (pSt->Unk2E) {
        if (pSt->Unk2F >= no && pSt->Unk2F != 0xFF) {
            pSt->Unk2F--;
        }
    } else if (pG->pUnk7C7C->Unk128 >= no && pG->pUnk7C7C->Unk128 != 0xFF) {
        pG->pUnk7C7C->Unk128--;
    }
    pSt->Flg14 |= 0x40000000;
}

void func_8006CEA8(void)
{
    UCHAR no;

    if ((no = G.pUnk7C7C->Unk128) != 0xFF) {
        func_8006CD68(no);
        G.pUnk7C7C->Unk129 = 0;
        G.pUnk7C7C->Unk128 = 0xFF;
    }
}

/* needs ITEM_WORK Item[] in ITEM_SET (see func_8006CD68); func_8006A95C first param changed to ST_WORK *pSt; func_8006CCF0 param n changed to signed char (both still match) */
signed char func_8006CF0C(UCHAR f)
{
    ST_WORK *pSt = &St;
    tagGLOBAL *pG = &G;
    UCHAR *pI;
    ITEM_WORK *p;
    signed char no;
    signed char n;
    UCHAR num;
    UCHAR *pT;

    if (pG->pUnk7C7C->Unk128 == 0xFF) {
        return -1;
    }
    pI = func_8006A95C(pSt, pG->pUnk7C7C->Unk129);
    if (pI == (UCHAR *)-1) {
        return -1;
    }
    no = Search_item_id(pI[1], 0);
    if (no == -1) {
        return -1;
    }
    if (f) {
        p = &pG->pUnk7C7C->Item[pG->pUnk7C7C->Unk128];
        pT = D_800A0514[p->Id];
        num = p->Num;
        n = pT[1] - num;
        if ((UCHAR)n >= pG->pUnk7C7C->Item[no].Num) {
            n = pG->pUnk7C7C->Item[no].Num;
        }
        p->Num = num + n;
        func_8006CCF0(no, n, 0);
        pG->Player.Status_flg |= 0x400;
    }
    return 0;
}

LONG func_8006D030(UCHAR f)
{
    ITEM_WORK *p;
    USHORT w;
    LONG t;

    if (G.pUnk7C7C->Unk128 == 0xFF) {
        return -1;
    }
    p = &G.pUnk7C7C->Item[G.pUnk7C7C->Unk128];
    w = p->Flg;
    t = w & 3;
    if (t == 3) {
        return 0xFF;
    }
    if (p->Num == 0) {
        return -1;
    }
    if (f) {
        if (t == 2) {
            p->Flg = w - 0x4000;
            if (!(p->Flg & 0xC000)) {
                p->Num--;
                p->Flg |= 0xC000;
            }
        } else {
            p->Num--;
        }
    }
    return p->Num;
}

void func_8006D0D8(void)
{
    tagGLOBAL *pG = &G;
    tagGLOBAL *pG2;
    UCHAR mode;
    ULONG j, k;
    LONG unused[2];
    ITEM_WORK *pS;
    ITEM_WORK *pD;

    pG->Item_work_no = 0;
    pG->pUnk7C7C = &pG->Item_work[0];
    mode = Flg_ck(&pG->System_flg, 0x17) != 0;
    if (Flg_ck(&pG->System_flg, 0x18)) {
        mode = 2;
    }
    if (D_800C79CC == 1) {
        mode = 3;
    }
    if (D_800C79CC == 2) {
        mode = 4;
    }
    if (D_800C79CC == 3) {
        mode = 2;
    }
    for (j = 0; j < 2; j++) {
        if (mode == 1 || mode == 4) {
            G.Item_work[j].Num = 10;
        } else {
            G.Item_work[j].Num = 8;
        }
        pD = G.Item_work[j].Item;
        for (k = 0; k < 10; pD++, k++) {
            pD->Id = 0;
            pD->Num = 0;
            pD->Flg = 0;
        }
        pD = G.Item_work[j].Box;
        for (k = 0; k < 64; pD++, k++) {
            pD->Id = 0;
            pD->Num = 0;
            pD->Flg = 0;
        }
        switch (mode) {
        case 0:
            pS = D_800A018C[j];
            break;
        case 1:
            pS = D_800A01B4[j];
            break;
        case 2:
            switch (G.Player.Id) {
            case 8:
                pS = D_800A01E4;
                break;
            case 9:
                pS = D_800A021C;
                break;
            case 10:
                pS = D_800A0200;
                break;
            }
            break;
        case 3:
            pS = D_800A0238;
            break;
        case 4:
            pS = D_800A0258;
            break;
        }
        pD = &G.Item_work[j].Item[0];
        while ((signed char)pS->Id != -1) {
            if (G.Unk7806 == 0 || (UCHAR)(pS->Id + 0x7D) >= 2) {
                pD->Id = pS->Id;
                pD->Num = pS->Num;
                pD->Flg = pS->Flg;
                if (Flg_ck(G.Unk77F8, 9) && mode != 2 && (UCHAR)(pD->Id - 2) < 0x13) {
                    pD->Flg |= 3;
                }
                pD++;
            }
            pS++;
        }
        if (mode == 1) {
            pS = D_800A0298[j];
        } else {
            pS = D_800A0278[j];
        }
        pD = &G.Item_work[j].Box[32];
        while ((signed char)pS->Id != -1) {
            pD->Id = pS->Id;
            pD->Num = pS->Num;
            pD->Flg = pS->Flg;
            if (Flg_ck(G.Unk77F8, 9) && (UCHAR)(pD->Id - 2) < 0x13) {
                pD->Flg |= 3;
            }
            pD++;
            pS++;
        }
        switch (mode) {
        case 0:
        case 1:
            if (Flg_ck(G.Unk77F8, 6)) {
                pD = &G.Item_work[j].Box[28 + j];
                pD->Id = 0xA;
                pD->Num = 0xFF;
                pD->Flg = 3;
            }
            if (Flg_ck(G.Unk77F8, 7)) {
                pD = &G.Item_work[j].Box[29 + j];
                pD->Id = 0xB;
                pD->Num = 0xFF;
                pD->Flg = 3;
            }
            if (Flg_ck(G.Unk77F8, 8)) {
                pD = &G.Item_work[j].Box[30 + j];
                pD->Id = 0xF;
                pD->Num = 0xFF;
                pD->Flg = 0x17;
            }
            if (j == 0 && Flg_ck(G.Unk77F8, 10)) {
                pD = &G.Item_work[0].Box[31];
                pD->Id = 0x80;
                pD->Num = 1;
                pD->Flg = 0;
            }
            break;
        }
        pD = &G.Item_work[j].Item[0];
        G.Item_work[j].Unk129 = pD->Id;
        G.Item_work[j].Unk128 = 0;
        for (k = 0; k < 4; k++) {
            G.Item_work[j].Unk12C[k] = 0;
        }
        if (mode == 2) {
            break;
        }
    }
    pG2 = &G;
    pG2->Unk79EC[2] = 0;
    pG2->Unk79EC[3] = 0;
    Flg_on(&pG2->Unk79EC[2], 0x1E);
    if (!Flg_ck(&pG2->System_flg, 0x18) && pG2->Unk7806) {
        Flg_on(&pG2->Unk79EC[2], 0);
        Flg_on(&pG2->Unk79EC[2], 0x1C);
        pG2->Unk79EC[3] = 2;
    }
    if (Flg_ck(&G.System_flg, 0x18)) {
        for (k = 0; k < 14; k++) {
            G.Unk79A4[k] = -1;
        }
        Flg_on(&G.Unk79EC[1], 0);
        Flg_on(&G.Unk79EC[1], 1);
        Flg_on(G.Unk79EC, 0);
        Flg_on(G.Unk79EC, 1);
    }
}

void func_8006D720(void)
{
    ST_WORK *pSt = &St;
    ITEM_WORK *p;
    ULONG i;
    UCHAR n;
    LONG pad[2];

    if (Flg_ck(&G.System_flg, 0x18)) {
        switch (G.Player.Id) {
        case 8:
            Cd_read(0x5A, (ULONG *)0x801B1500, 0, "STMAIN1.TIM");
            break;
        case 9:
            Cd_read(0x5C, (ULONG *)0x801B1500, 0, "STMAIN2.TIM");
            break;
        case 10:
            Cd_read(0x5E, (ULONG *)0x801B1500, 0, "STMAIN3.TIM");
            break;
        }
    } else {
        Cd_read(0x58, (ULONG *)0x801B1500, 0, "STMAIN0.TIM");
    }
    FLD(&G, 0x2484, SHORT) = 0x41A; /* G.Ctp.Tpage_clut through a raw address: G+0x2484 becomes the CSE base of the G loads below */
    Mapping_tim((ULONG *)0x801B1500);
    Cd_read(0x60, (ULONG *)0x801B1500, 0, "STMOJI.TIM");
    FLD(&G, 0x2484, SHORT) = 0x1A;
    Mapping_tim((ULONG *)0x801B1500);
    Cd_read(0x32, (ULONG *)0x801B1500, 0, D_80010FEC);
    p = G.pUnk7C7C->Item;
    for (i = 0; i < 10; i++, p++) {
        func_8006AB68(i, p->Id, 0x801B1500, (ULONG *)0x801B1000);
        DrawSync(0);
    }
    if (G.pUnk7C7C->Unk128 != 0xFF) {
        func_8006AE20(10, G.pUnk7C7C->Unk128);
    }
    n = G.pUnk7C7C->Unk129;
    pSt->UnkD0 = 4;
    pSt->UnkD1 = 5;
    pSt->UnkD2 = 6;
    pSt->UnkD4 = 8;
    pSt->UnkD5 = 14;
    pSt->UnkD7 = 10;
    pSt->UnkD3 = 0;
    pSt->UnkD8 = 9;
    pSt->UnkD6 = 0;
    pSt->St_type = 0;
    pSt->Unk6 = 0;
    pSt->Unk2E = 0;
    pSt->UnkB8 = 0;
    pSt->Unk28 = n;
}

INCLUDE_RODATA("asm/nonmatchings/game/800636D4", D_80010FEC);

void func_8006D948(ST_WORK *pSt)
{
    RECT r;
    tagGLOBAL *pG = &G;
    SND_WORK *pW = &Snd_work;
    USHORT *pD = (USHORT *)pG->pUnk21DC;
    ENEMY_WORK *pPl;
    UCHAR *p;
    ULONG i;

    Task_suspend(0);
    Bg_set_mode(1, 0x80808);
    Pad_rep_set(0xF00C, 0x60A);
    D_800D442C = 1;
    pPl = PLAYER_EM(pG);
    if (pSt->Unk2E) {
        pSt->Unk2E = 0;
        pG->pUnk7C7C->Unk128 = pSt->Unk2F;
        pG->pUnk7C7C->Unk129 = pSt->Unk30;
        pPl->Type &= 0xFF00;
        pPl->Type |= pG->pUnk7C7C->Unk129;
        Weapon_cls(pPl);
        Pl_weapon_ch(pPl);
    }
    pW->UnkB1C = 0x80;
    pW->UnkB20 = 0x3FFF;
    pW->UnkB24 = 0xB00;
    Cd_seek_s(0x34, 0, "SEEK ITEMI.PIX");
    if (pSt->Flg14 & 0x40000000) {
        Cd_read(0x32, (ULONG *)0x801B1500, 0, D_80010FEC);
        p = (UCHAR *)pG->pUnk7C7C->Item;
        for (i = 0; i < 10; i++, p += 4) {
            func_8006AB68(i, *p, 0x801B1500, (ULONG *)0x801B1000);
            DrawSync(0);
        }
        if (G.pUnk7C7C->Unk128 != 0xFF) {
            func_8006AE20(10, pG->pUnk7C7C->Unk128);
        } else {
            func_8006AE20(10, 11);
        }
        pSt->Flg14 &= ~0x40000000;
    }
    pSt->Rtn0 = 0;
    pSt->Rtn1 = 0;
    pSt->Rtn2 = 0;
    pSt->Unk13 = 0;
    for (i = 0; i < 6; i++) {
        *((UCHAR *)pSt + i + 0x1C) = 0;
    }
    pSt->Unk27 = 1;
    pSt->Flg = 0;
    for (i = 0; i < 32; i++) {
        pSt->Pos[i].X = 0;
        pSt->Pos[i].Y = 0;
    }
    pSt->Unk3A = 0;
    pSt->Unk3B = 0;
    pSt->Unk3C = 0;
    pSt->Unk3D = 0;
    pSt->Unk28 = pG->pUnk7C7C->Unk129;
    pSt->Unk29 = pG->Player.Id;
    pSt->Unk32 = 0;
    pSt->Unk22 = 0;
    pSt->Unk24 = 0;
    func_8006ED28(pSt);
    switch (pSt->UnkB8) {
    case 1:
        pSt->UnkAB = 0;
        break;
    case 2:
        pSt->UnkAB = 4;
        break;
    }
    if (pSt->St_type == 1) {
        if (*pD >= 0x85 && *pD < 0xA4) {
            pSt->St_type = 3;
            pSt->UnkBC = *pD + 0x7B;
            pSt->UnkBD = 0;
            Flg_on(&G.Unk79EC[2], pSt->UnkBC);
            for (i = 0; i < 30; i++) {
                if (D_800A00B0[i] == pSt->UnkBC) {
                    break;
                }
            }
            if (G.Unk79EC[3] == i) {
                G.Unk79EC[3] = i + 1;
            }
        }
        if (*pD >= 0xA4) {
            pSt->St_type = 5;
            Flg_on(&G.Unk79EC[0], *pD - 0xA4);
        }
    }
    switch (pSt->St_type) {
    case 4:
        pSt->pBuf = (ULONG *)0x801B1500;
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x80;
        r.h = 0x100;
        StoreImage(&r, (ULONG *)0x8019C000);
        DrawSync(0);
        break;
    case 5:
        pSt->pBuf = (ULONG *)0x801B1500;
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x80;
        r.h = 0x100;
        StoreImage(&r, (ULONG *)0x8019C000);
        DrawSync(0);
        Snd_se_stad(9, 0, 0, 0);
        break;
    case 3:
        pSt->pBuf = (ULONG *)0x801B1500;
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x80;
        r.h = 0x100;
        StoreImage(&r, (ULONG *)0x8019C000);
        DrawSync(0);
        func_8006AE90(pSt);
        break;
    case 2:
        pSt->pBuf = (ULONG *)0x801A1500;
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x40;
        r.h = 0x100;
        StoreImage(&r, (ULONG *)0x80194000);
        DrawSync(0);
        func_8006511C(pSt);
        i = Cd_read(0x32, pSt->pBuf, 0, D_80010FEC);
        pSt->Unk1F = 0x20;
        pSt->pBuf = (ULONG *)((UCHAR *)pSt->pBuf + i);
        p = (UCHAR *)&G.pUnk7C7C->Item[40];
        for (i = 30; i < 35; i++, p += 4) {
            func_8006ABF8(i, *p, 0x801A1500, (ULONG *)0x801A1000);
            DrawSync(0);
        }
        break;
    case 1:
        pSt->pBuf = (ULONG *)0x801B1500;
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x80;
        r.h = 0x100;
        StoreImage(&r, (ULONG *)0x8019C000);
        DrawSync(0);
        func_8006AC88(*pD);
        func_8006A9AC(12, *pD);
        pSt->Pos[3].X = 0x48;
        pSt->Pos[4].X = 0x48;
        pSt->Pos[5].X = 0x48;
        pSt->Pos[6].X = 0x48;
        pSt->Pos[7].X = 0x48;
        pSt->Pos[9].X = -0xD8;
        pSt->Pos[14].X = -0xD8;
        pSt->Pos[8].X = 0;
        pSt->Pos[10].Y = 0x30;
        func_8006AE90(pSt);
        pSt->Flg |= 0x40000000;
        break;
    case 0:
        pSt->pBuf = (ULONG *)0x801B1500;
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x80;
        r.h = 0x100;
        StoreImage(&r, (ULONG *)0x8019C000);
        DrawSync(0);
        pSt->Pos[3].X = 0x48;
        pSt->Pos[4].X = 0x48;
        pSt->Pos[5].X = 0x48;
        pSt->Pos[6].X = 0x48;
        pSt->Pos[7].X = 0x48;
        pSt->Pos[8].X = -0xD8;
        pSt->Pos[9].X = -0xD8;
        pSt->Pos[14].X = -0xD8;
        pSt->Pos[10].Y = 0x30;
        func_8006AE90(pSt);
        pSt->Flg |= 0x40000000;
        pSt->Flg |= 0x4000000;
        break;
    }
    Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 6);
}

void func_80073E7C_p(ST_WORK *pSt) __asm__("func_80073E7C"); /* arg 1 ST_WORK* (canonical void) */

void func_8006DFDC(void)
{
    ST_WORK *pSt = &St;

    func_8006D948(pSt);
    while (pSt->Unk27) {
        if (Main.Unk194 != 1) {
            D_800A02F0[pSt->Rtn0](pSt);
            func_8006E268(pSt);
        }
        func_8006E34C(pSt);
        func_80073E7C_p(pSt);
        Task_sleep(1);
    }
    func_8006E0A4(pSt);
    Task_exit();
}

void func_8006E0A4(ST_WORK *pSt)
{
    RECT r;
    tagGLOBAL *pG = &G;
    ENEMY_WORK *pPl;

    switch (pSt->St_type) {
    case 2:
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x40;
        r.h = 0x100;
        LoadImage(&r, (ULONG *)0x80194000);
        DrawSync(0);
        Main.Unk18E = 1;
        Main.Bg_mode = 2;
        break;
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x80;
        r.h = 0x100;
        LoadImage(&r, (ULONG *)0x8019C000);
        DrawSync(0);
        if (pSt->Unk6) {
            D_800D442E = 1;
        } else {
            D_800D442E = 2;
        }
        pSt->Unk6 = 0;
        D_800D442D = 2;
        break;
    }
    pSt->St_type = 0;
    pSt->UnkB8 = 0;
    pPl = PLAYER_EM(pG);
    if (pSt->Unk28 != pG->pUnk7C7C->Unk129) {
        pSt->Unk28 = pG->pUnk7C7C->Unk129;
        pPl->Type &= 0xFF00;
        pPl->Type |= pG->pUnk7C7C->Unk129;
        Weapon_cls(pPl);
        Pl_weapon_ch(pPl);
    }
    Cd_seek_s(D_8009DFD0[pG->Unk783E][pG->Unk7840 + 1] - 1, 0, "SEEK BSS");
    D_800D442C = 2;
    Task_signal(0);
}

void func_8006E268(ST_WORK *pSt)
{
    pSt->Unk34 = func_8006E598();
    if (*(USHORT *)&pSt->Rtn0 == 0x205) {
        return;
    }
    if (pSt->Unk24) {
        pSt->Unk22 -= 2;
    } else {
        pSt->Unk22 += 2;
    }
    if (pSt->Unk22 >= 0x3F) {
        pSt->Unk22 = 0x3F;
        pSt->Unk24 = 1;
    }
    if (pSt->Unk22 <= 0) {
        pSt->Unk22 = 0;
        pSt->Unk24 = 0;
    }
    if (pSt->Flg & 0x800000) {
        pSt->Unk32 += 3;
    } else if (++pSt->Unk32 > 0x50) {
        pSt->Unk32 = -0x20;
    }
}

void func_8006E34C(ST_WORK *pSt)
{
    switch (pSt->St_type) {
    case 4:
    case 5:
        if (pSt->Flg & 0x200000) {
            func_80070244(pSt);
        }
        break;
    case 2:
        func_800654A8(pSt);
        break;
    case 0:
    case 1:
        if (pSt->Flg & 0x40000000) {
            func_8006B66C(pSt);
        }
        if (pSt->Flg & 0x200000) {
            func_80070244(pSt);
        }
    case 3:
        if (pSt->Flg & 0x100000) {
            func_80063FE4(pSt);
        }
        break;
    }
}

void func_8006E424(ST_WORK *pSt)
{
    SND_WORK *pW = &Snd_work;

    if (Fade_status(0) != 0) {
        return;
    }
    if (pW->UnkB1C != 0) {
        return;
    }
    switch (pSt->St_type) {
    case 4:
        pSt->Rtn0 = 4;
        break;
    case 5:
        pSt->Rtn0 = 4;
        break;
    case 3:
        pSt->Rtn0 = 7;
        break;
    case 2:
        pSt->Rtn0 = 10;
        break;
    case 0:
    case 1:
        pSt->Rtn0 = 1;
        break;
    }
}

void func_8006E4CC(ST_WORK *pSt)
{
    SND_WORK *pW = &Snd_work;
    LONG pad[1];

    switch (pSt->Rtn1) {
    case 0:
        Fade_set(0, 0, 2, 2, 0, 0xFFFFFF, 6);
        pW->UnkB1C = 0x80;
        pW->UnkB20 = 0;
        pW->UnkB24 = -0xB00;
        pSt->Rtn1++;
        break;
    case 1:
        if ((Fade_status(0) & 4) && pW->UnkB1C == 0) {
            pSt->Unk27 = 0;
        }
        break;
    }
}

LONG func_8006E598(void)
{
    USHORT f = G.Player.Status_flg;
    LONG l = G.Player.Life;

    if (f & 0x100) {
        return 5;
    }
    if (f & 0x200) {
        return 4;
    }
    if (l > 100) {
        return 0;
    }
    if (l > 40) {
        return 1;
    }
    if (l > 20) {
        return 2;
    }
    return 3;
}

/* prototype in file changed: void func_8006E600(ST_WORK *pSt, SPRT *p, UCHAR *pTbl, ULONG f); */
void func_8006E600(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm)
{
    ULONG i, j;
    UCHAR y = prm >> 8;
    UCHAR semi = prm >> 16;
    ULONG n = prm;

    for (i = 0; i < (UCHAR)n; i++) {
        for (j = 0; j < 2; j++) {
            setSprt(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = src[0];
            p->v0 = src[2];
            p->w = *(USHORT *)(src + 4);
            p->h = *(USHORT *)(src + 6);
            if (semi == 0) {
                p->clut = (y + 0x1E0) << 6;
            } else {
                p->clut = ((y + 0x1E0) << 6) | 0x13;
                setSemiTrans(p, 1);
            }
            p++;
        }
        src += 12;
    }
}

void func_8006E6D8(ST_WORK *pSt, POLY_FT4 *p, USHORT *src, ULONG prm)
{
    ULONG i, j;
    UCHAR y = prm >> 8;
    UCHAR semi = prm >> 16;
    UCHAR w = prm >> 24;
    ULONG n = prm;
    LONG tp;
    ULONG u, v;

    for (i = 0; i < (UCHAR)n; i++) {
        u = src[0];
        v = src[1];
        for (j = 0; j < 2; j++) {
            setPolyFT4(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = u;
            p->v0 = v;
            p->u1 = w + u;
            p->v1 = v;
            p->u2 = u;
            p->v2 = w + v;
            p->u3 = w + u;
            p->v3 = w + v;
            tp = semi == 0 ? 0x1B : 6;
            p->tpage = getTPage(1, 0, (tp & 0xF) << 6, (tp & 0x10) << 4);
            p->clut = (y + 0x1E0) << 6;
            p++;
        }
        src += 6;
    }
}

void func_8006E7D4(ST_WORK *pSt, TILE *p, SHORT *pWh, UCHAR n)
{
    ULONG i, j;

    for (i = 0; i < n; i++, pWh += 6) {
        for (j = 0; j < 2; j++) {
            setTile(p);
            setRGB0(p, 8, 8, 8);
            p->w = pWh[2];
            p->h = pWh[3];
            p++;
        }
    }
}

void func_8006E84C(ST_WORK *pSt)
{
    LINE_F2 *p;
    ULONG i, j, n;

    n = 32;
    p = (LINE_F2 *)0x801AF0E8;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 2; j++) {
            setLineF2(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            setSemiTrans(p, 1);
            p++;
        }
    }
}

void func_8006E8BC(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm)
{
    ULONG i;
    LONG ox, oy;
    UCHAR ofs = prm >> 8;
    UCHAR ot = prm >> 16;
    ULONG n = prm;

    if (G.Cbuf_id) {
        p++;
    }
    ox = pSt->Pos[ofs].X;
    oy = pSt->Pos[ofs].Y;
    for (i = 0; i < (UCHAR)n; i++, p += 2, src += 12) {
        p->x0 = *(SHORT *)(src + 8) + ox;
        p->y0 = *(SHORT *)(src + 10) + oy;
        AddPrim(&pSt->Ot[G.Cbuf_id][ot], p);
    }
}

void func_8006E9D4(ST_WORK *pSt, POLY_FT4 *p, USHORT *src, ULONG prm)
{
    ULONG i;
    UCHAR ofs = prm >> 8;
    UCHAR ot = prm >> 16;
    ULONG n = prm;
    LONG x, y, x1, y1;

    if (G.Cbuf_id) {
        p++;
    }
    for (i = 0; i < (UCHAR)n; p += 2, src += 6, i++) {
        x = pSt->Pos[ofs].X + src[4];
        y = pSt->Pos[ofs].Y + src[5];
        x1 = x + src[2];
        y1 = y + src[3];
        setXY4(p, x, y, x1, y, x, y1, x1, y1);
        AddPrim(G.pOt0 + ot, p);
    }
}

void func_8006EAF0(ST_WORK *pSt, TILE *p, SHORT *src, ULONG prm)
{
    ULONG i;
    LONG ox, oy;
    UCHAR ofs = prm >> 8;
    UCHAR ot = prm >> 16;
    ULONG n = prm;

    if (G.Cbuf_id) {
        p++;
    }
    ox = pSt->Pos[ofs].X;
    oy = pSt->Pos[ofs].Y;
    for (i = 0; i < (UCHAR)n; i++, p += 2, src += 6) {
        p->x0 = src[4] + ox;
        p->y0 = src[5] + oy;
        AddPrim(G.pOt0 + ot, p);
    }
}

void func_8006EBEC(void)
{
    TIM_IMAGE tim;
    TIM_IMAGE *p;
    LONG y;

    OpenTIM_v();
    p = ReadTIM(&tim);
    p->prect->x = (G.Ctp.u.Ctpage << 6) - (G.Ctp.u.Ctpage >= 0x10 ? 0x400 : 0);
    y = p->prect->y;
    if (G.Ctp.u.Ctpage >= 0x10) {
        y += 0x100;
    }
    p->prect->y = y;
    LoadImage(p->prect, p->paddr);
    if (p->caddr) {
        p->crect->y = G.Ctp.u.Cclut + 0x1E0;
        LoadImage(p->crect, p->caddr);
    }
    DrawSync(0);
}

void func_8006ECB0(ULONG no)
{
    switch (no) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        D_800D23B8 = 0;
        break;
    case 8:
        D_800D23B8 = 1;
        break;
    }
    G.pUnk7C7C = &G.Item_work[G.Item_work_no];
    St.Flg14 |= 0x40000000;
}

void func_8006ED28(ST_WORK *pSt)
{
    UCHAR c;

    pSt->UnkBB = 0;
    pSt->UnkBA = 0;
    pSt->UnkAB = (UCHAR)G.Unk783E;
    pSt->UnkAC = (UCHAR)G.Unk7840;
    func_80073CCC(&pSt->UnkAB, &pSt->UnkAC);
    pSt->UnkAA = (UCHAR)G.Unk783E;
    c = (UCHAR)G.Unk7840;
    func_80073CCC(&pSt->UnkAA, &c);
}

void func_8006ED98(ST_WORK *pSt)
{
    ULONG i, j;
    UCHAR on;
    ST_MAP_INFO *pInfo;
    ST_MAP_ROOM *pRoom;
    LONG x, z;
    UCHAR u;
    POLY_FT4 *pF;
    LINE_F2 *pL;
    SPRT *pS;
    TILE *pT;
    DR_TPAGE *pTp;

    if (pSt->UnkBA) {
        pSt->Rtn1 = 3;
        pSt->Rtn2 = 0;
        pSt->Rtn0++;
    } else {
        pSt->Rtn1 = 0;
        pSt->Rtn2 = 0;
        pSt->Rtn0++;
    }
    pSt->UnkBA = 0;
    pSt->Flg &= ~0x20000000;
    pSt->Flg &= ~0x40000000;
    pSt->Flg &= ~0x80000000;
    pSt->Flg |= 0x200000;
    if (Flg_ck(&G.System_flg, 0x18)) {
        func_800713DC(pSt, pSt->UnkAB + 7);
    } else {
        func_800713DC(pSt, pSt->UnkAB);
    }
    on = 1;
    pInfo = (ST_MAP_INFO *)((UCHAR *)pSt->pUnkB4 + pSt->pUnkB4[1]);
    pSt->UnkAD = pInfo->Unk4;
    pSt->UnkB0 = pInfo->Unk5;
    pSt->UnkA9 = on;
    pSt->Unk94 = pInfo->X;
    pSt->Unk98 = pInfo->Y;
    if (pSt->UnkAB != pSt->UnkAA) {
        pSt->Unk84 = pSt->Unk7C = pSt->Unk94;
        pSt->Unk88 = pSt->Unk80 = pSt->Unk98;
        if (pSt->UnkAD) {
            pSt->UnkA4 = 100;
            pSt->UnkA8 = 0;
            switch (pSt->UnkAB) {
            case 2:
            case 5:
                pSt->UnkAE = 0;
                break;
            case 4:
            case 6:
                pSt->UnkAE = on;
                break;
            }
            pSt->UnkAF = -1;
        } else {
            pSt->UnkA8 = on;
            pSt->UnkA4 = 50;
            pSt->UnkA9 = 0;
        }
    } else {
        pSt->UnkA8 = 0;
        pSt->UnkA4 = 100;
        if (pSt->UnkAD) {
            pSt->Unk7C = pSt->Unk94;
            pSt->Unk80 = pSt->Unk98;
            pSt->UnkAE = pSt->UnkAF = func_80073BC8(pSt, pSt->UnkAB, pSt->UnkAC);
            if (St.UnkB8 == 2) {
                pSt->UnkAE = on;
            }
            if (G.Unk783E == 4 && pSt->UnkAC == 3) {
                if (Flg_ck(G.Unk7868, 0xF1) && G.Cut_no_old != 0x503) {
                    pSt->UnkAF = 0;
                    pSt->UnkAE = 0;
                } else {
                    pSt->UnkAF = on;
                    pSt->UnkAE = on;
                }
            }
        } else {
            pRoom = (ST_MAP_ROOM *)pSt->pUnkB4;
            x = G.Player.Pos_x;
            z = G.Player.Pos_z;
            pRoom = (ST_MAP_ROOM *)((UCHAR *)pRoom + *(ULONG *)pRoom);
            pRoom += pSt->UnkAC;
            if (G.Unk783E == 1 && G.Unk7840 == 15 && x >= -0x2276) {
                x += 0x251D;
            }
            if (G.Unk783E == 3 && G.Unk7840 == 21 && G.Player.Pos_x >= 800) {
                x = G.Player.Pos_x - 0x2328;
                z = G.Player.Pos_z;
            }
            pSt->Unk7C = -(pRoom->X0 + (x - pRoom->X1) / 450);
            pSt->Unk80 = (z - pRoom->Z1) / 450 - pRoom->Z0;
            pSt->UnkAE = 0;
        }
    }
    for (i = 0, pF = (POLY_FT4 *)0x801AFE80; i < 2; i++, pF++) {
        setPolyFT4(pF);
        u = ((USHORT)(G.Player.Cdir_y + 0x100) >> 6) & 0x38;
        pF->v0 = 0xF0;
        pF->v1 = 0xF0;
        pF->v2 = 0xF8;
        pF->v3 = 0xF8;
        pF->tpage = 0x17;
        pF->u0 = u;
        pF->u1 = u + 8;
        pF->u2 = u;
        pF->u3 = u + 8;
        pF->clut = 0x7B53;
    }
    for (j = 0, pL = (LINE_F2 *)0x801B0010; j < 2; j++, pL++) {
        setLineF2(pL);
        setXY2(pL, 0, 0x78, 0x140, 0x78);
        setSemiTrans(pL, 1);
    }
    for (j = 0, pL = (LINE_F2 *)0x801B0030; j < 2; j++, pL++) {
        setLineF2(pL);
        setXY2(pL, 0xA0, 0, 0xA0, 0xF0);
        setSemiTrans(pL, 1);
    }
    for (j = 0, pF = (POLY_FT4 *)0x801AFED0; j < 8; j++, pF++) {
        setPolyFT4(pF);
        setRGB0(pF, 0x80, 0x80, 0x80);
        pF->tpage = 0x1A;
        pF->clut = 0x7893;
    }
    for (j = 0, pL = (LINE_F2 *)0x801B0050; j < 0x4C; j++, pL++) {
        setLineF2(pL);
        setRGB0(pL, 0x20, 0x20, 0x20);
    }
    for (j = 0, pS = (SPRT *)0x801B0510; j < 2; j++, pS++) {
        setSprt(pS);
        setRGB0(pS, 0x80, 0x80, 0x80);
        setXY0(pS, 0x118, 0x10);
        setUV0(pS, 0, 0xD8);
        setWH(pS, 0x1A, 0x28);
        pS->clut = 0x7E93;
    }
    for (j = 0; j < 2; j++, pS++) {
        setSprt(pS);
        setRGB0(pS, 0x80, 0x80, 0x80);
        setXY0(pS, 0xB4, 0xD0);
        setUV0(pS, 0x20, 0xE8);
        setWH(pS, 0x48, 0xF);
        pS->clut = 0x7E93;
    }
    for (j = 0; j < 2; j++, pS++) {
        setSprt(pS);
        setRGB0(pS, 0x80, 0x80, 0x80);
        setXY0(pS, 0xFC, 0xD6);
        setUV0(pS, 0x20, 0xF7);
        setWH(pS, 0x48, 9);
        pS->clut = 0x7E93;
    }
    for (j = 0; j < 2; j++, pS++) {
        setSprt(pS);
        setRGB0(pS, 0x80, 0x80, 0x80);
        setXY0(pS, 0xB4, 0xD0);
        setUV0(pS, 0x68, 0xE8);
        setWH(pS, 0x48, 0xF);
        pS->clut = 0x7E93;
    }
    for (j = 0; j < 2; j++, pS++) {
        setSprt(pS);
        setRGB0(pS, 0x80, 0x80, 0x80);
        setXY0(pS, 0xFC, 0xD6);
        setUV0(pS, 0x68, 0xF7);
        setWH(pS, 0x48, 9);
        pS->clut = 0x7E93;
    }
    for (j = 0; j < 2; j++, pS++) {
        setSprt(pS);
        setRGB0(pS, 0x80, 0x80, 0x80);
        setXY0(pS, 0x10, 0x10);
        setUV0(pS, 0x20, 0xD8);
        setWH(pS, 0x90, 0x10);
        pS->clut = 0x7E93;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++, pS++) {
            setSprt(pS);
            setRGB0(pS, 0x80, 0x80, 0x80);
            setXY0(pS, 0xB0, 0x10);
            setUV0(pS, (i & 1) * 40 + 0xB0, (i >> 1) * 16 + 0xD8);
            setWH(pS, 0x26, 0x10);
            pS->clut = 0x7E93;
        }
    }
    for (j = 0, pT = (TILE *)0x801B06A0; j < 2; j++, pT++) {
        setTile(pT);
        setRGB0(pT, 0x40, 0x40, 0x40);
        setSemiTrans(pT, 1);
    }
    for (i = 0, pTp = pSt->Tp404; i < 4; i++, pTp++) {
        SetDrawTPage(pTp, 0, 0, 0x18);
    }
    for (i = 0, pTp = pSt->Tp424; i < 2; i++, pTp++) {
        SetDrawTPage(pTp, 0, 0, 0x58);
    }
    pSt->UnkBE = 0;
    pSt->Unk22 = 0;
    pSt->Unk24 = 0;
    pSt->UnkBA = 0;
    pSt->Unk9C = pSt->Unk94 + 0x140;
    pSt->UnkA0 = pSt->Unk98 + 0xE0;
}

void func_8006F6CC(ST_WORK *pSt)
{
    D_800A0500[pSt->Rtn1](pSt);
}

void func_8006F708(ST_WORK *pSt)
{
    tagGLOBAL *pG;

    switch (St.UnkB8) {
    case 1:
        if (pSt->Unk24 == 0 && pSt->Unk22 == 0x1E) {
            Snd_se_stad(0x201, 0, 0, 0);
        }
        break;
    case 2:
        if (pSt->Unk24 == 0 && pSt->Unk22 == 0x1E) {
            Snd_se_stad(0x22B, 0, 0, 0);
        }
        break;
    }
    pG = &G;
    if ((pG->Key_trg & 0x2000) | (pG->Unk20FC & 1)) {
        pSt->Rtn1 = 0;
        pSt->Rtn0++;
        Snd_se_stad(5, 0, 0, 0);
        return;
    }
    if (*(ULONG *)&pG->Unk20FC & 0x108) {
        pSt->Rtn1 = 2;
        pSt->Rtn2 = 0;
        pSt->Unk13 = 0;
        Snd_se_stad(6, 0, 0, 0);
        return;
    }
    if (pSt->UnkAD) {
        if (pG->Key_trg & 0x1000) {
            if (++pSt->UnkAE >= pSt->UnkB0) {
                pSt->UnkAE = 0;
            }
            Snd_se_stad(6, 0, 0, 0);
            return;
        }
        if (pG->Unk2100 & 0x4000) {
            if (--pSt->UnkAE < 0) {
                pSt->UnkAE = 0;
            } else {
                Snd_se_stad(6, 0, 0, 0);
            }
        }
        if (G.Unk2100 & 0x1000) {
            if (++pSt->UnkAE >= pSt->UnkB0) {
                pSt->UnkAE = pSt->UnkB0 - 1;
            } else {
                Snd_se_stad(6, 0, 0, 0);
            }
        }
    } else {
        if (pG->Key_trg & 0x1000) {
            pSt->Rtn1++;
            if (pSt->UnkA8 ^= 1) {
                Snd_se_stad(6, 0, 0, 0);
                pSt->Unk8C = 0;
                pSt->Unk90 = 0;
                pSt->UnkA9 = 0;
                pSt->Unk84 = pSt->Unk7C;
                pSt->Unk88 = pSt->Unk80;
                return;
            }
            Snd_se_stad(5, 0, 0, 0);
            pSt->Unk8C = 0;
            pSt->Unk90 = 0;
            return;
        }
        if (pSt->UnkA8 == 0) {
            if ((USHORT)pG->Unk20F8 & 0x1000) {
                pSt->Unk80 += 4;
            }
            if ((USHORT)pG->Unk20F8 & 0x4000) {
                pSt->Unk80 -= 4;
            }
            if ((USHORT)pG->Unk20F8 & 0x8000) {
                pSt->Unk7C += 4;
            }
            if ((USHORT)pG->Unk20F8 & 0x2000) {
                pSt->Unk7C -= 4;
            }
        }
        if (pSt->Unk80 > pSt->Unk98 + 0xE0) {
            pSt->Unk80 = pSt->Unk98 + 0xE0;
        }
        if (pSt->Unk80 < pSt->Unk98 - 0xE0) {
            pSt->Unk80 = pSt->Unk98 - 0xE0;
        }
        if (pSt->Unk7C > pSt->Unk94 + 0x140) {
            pSt->Unk7C = pSt->Unk94 + 0x140;
        }
        if (pSt->Unk7C < pSt->Unk94 - 0x140) {
            pSt->Unk7C = pSt->Unk94 - 0x140;
        }
    }
    if (++pSt->UnkBE >= 0x15) {
        pSt->UnkBE = -0x14;
    }
}

void func_8006FAE4(ST_WORK *pSt)
{
    if (pSt->UnkA8) {
        pSt->Unk8C += pSt->Unk84 - pSt->Unk94;
        pSt->Unk90 += pSt->Unk88 - pSt->Unk98;
        pSt->Unk7C = pSt->Unk84 - pSt->Unk8C / 10;
        pSt->Unk80 = pSt->Unk88 - pSt->Unk90 / 10;
        if ((pSt->UnkA4 -= 5) < 0x33) {
            pSt->Rtn1 = 0;
            pSt->UnkA4 = 0x32;
            pSt->Unk7C = pSt->Unk94;
            pSt->Unk80 = pSt->Unk98;
        }
    } else {
        pSt->Unk8C += pSt->Unk94 - pSt->Unk84;
        pSt->Unk90 += pSt->Unk98 - pSt->Unk88;
        pSt->Unk7C = pSt->Unk94 - pSt->Unk8C / 10;
        pSt->Unk80 = pSt->Unk98 - pSt->Unk90 / 10;
        if ((pSt->UnkA4 += 5) >= 100) {
            pSt->UnkA4 = 100;
            pSt->Rtn1 = 0;
            pSt->UnkA9 = 1;
            pSt->UnkBE = 0;
            pSt->Unk7C = pSt->Unk84;
            pSt->Unk80 = pSt->Unk88;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/800636D4", func_8006FC68);

void func_8006FEB0(ST_WORK *pSt)
{
    switch (pSt->Rtn2) {
    case 0:
        Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 8);
        pSt->Rtn2++;
        break;
    case 1:
        if (Fade_status(0) == 0) {
            pSt->Rtn1 = 0;
            pSt->Rtn2 = 0;
        }
        break;
    }
}

void func_8006FF48(ST_WORK *pSt)
{
    switch (pSt->Rtn2) {
    case 0:
        Fade_set(0, 0, 2, 2, 0, 0xFFFFFF, 8);
        pSt->Rtn2++;
        break;
    case 1:
        if (Fade_status(0) & 4) {
            pSt->Rtn0 = 4;
            pSt->Flg &= ~0x200000;
            pSt->Rtn1 = 0;
            pSt->Rtn2 = 0;
            pSt->Unk13 = 0;
            pSt->UnkAB = D_800A04F8[(signed char)pSt->UnkBB];
            pSt->UnkBA = 1;
        }
        break;
    }
}

void func_80070024(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    UCHAR *pA = pG->pAot_data;
    UCHAR *pD = (UCHAR *)pG->pUnk21DC;
    UCHAR n;

    switch (pSt->Rtn1) {
    case 0:
        pSt->Flg &= ~0x200000;
        pSt->Rtn1++;
        break;
    case 1:
        switch (pSt->St_type) {
        case 0:
            pSt->Rtn0 = 2;
            pSt->Rtn1 = 4;
            pSt->Rtn2 = 1;
            pSt->Unk13 = 0;
            pSt->Flg |= 0x20000000;
            pSt->Flg |= 0x40000000;
            pSt->Flg |= 0x80000000;
            func_8006AE90(pSt);
            break;
        case 4:
            pSt->Rtn0 = 0xD;
            pSt->Rtn1 = 0;
            break;
        case 5:
            *pA = 0;
            n = pD[6];
            if (pG->Unk783E < 2) {
                Flg_on(pG->Unk78D0, *(USHORT *)(pD + 4));
            } else {
                Flg_on(pG->Unk78F0, *(USHORT *)(pD + 4));
            }
            Esp_kill(0x705, &G.Ob_model[n & 0xFF].M);
            Esp_kill(0x905, 0);
            if (!(n & 0x80)) {
                G.Ob_model[n & 0xFF].Be_flg = 0;
            }
            D_800DBB5B = pD[0];
            Mess_set(0, 0x1000, 6, 0);
            pSt->Rtn1++;
            break;
        }
        break;
    case 2:
        if (!(D_800DBB58 & 0x80)) {
            pSt->Rtn0 = 0xD;
            pSt->Rtn1 = 0;
            Snd_se_stad(5, NULL, 0, 0);
        }
        break;
    }
}

void func_80070244(ST_WORK *pSt)
{
    ST_MAP_PARTS *pRoom;
    ULONG num;
    ST_MAP_HDR *pHdr;
    DR_TPAGE *pOt0;
    DR_TPAGE *pOt1;
    DR_TPAGE *pOt2;
    UCHAR col;
    POLY_FT4 *pP;
    ST_MAP_PART *pPt;
    ST_MAP_ARROW *pA;
    LINE_F2 *pL;
    TILE *pT;
    SPRT *pS;
    SHORT *pRm;
    UCHAR c;
    LONG i, j, f;
    LONG mx, my;
    LONG px, pz, zoom, dx, dy, ox, oy;

    pP = (POLY_FT4 *)0x801AC000;
    pOt0 = &pSt->Tp404[0];
    pOt1 = &pSt->Tp404[2];
    pOt2 = &pSt->Tp424[0];
    col = (UCHAR)pSt->Unk22 * 2 + 0x40;
    pHdr = (ST_MAP_HDR *)(pSt->UnkCC[1] + (ULONG)pSt->UnkCC);
    pRoom = pHdr->Room;
    num = pHdr->Num;
    if (G.Cbuf_id) {
        pP = (POLY_FT4 *)0x801AC028;
        pOt0 = &pSt->Tp404[1];
        pOt1 = &pSt->Tp404[3];
        pOt2 = &pSt->Tp424[1];
    }
    AddPrim(G.pOt0 + 1, pOt2);
    AddPrim(G.pOt0 + 1, pOt0);
    AddPrim(G.pOt0 + 6, pOt1);
    for (i = 0; i < num; i++, pRoom++) {
        if (!Flg_ck(D_800D2124, pSt->UnkAB)) {
            if (i >= 0x30) {
                continue;
            }
            if (!func_80071668(pSt->UnkAB, i)) {
                continue;
            }
        } else if (pSt->UnkAB == 4 && (ULONG)(i - 0xB) < 5) {
            if (!func_80071668(pSt->UnkAB, i)) {
                continue;
            }
        }
        if (!func_800716A4(pSt->UnkAB, i)) {
            continue;
        }
        pPt = (ST_MAP_PART *)(pRoom->Ofs + (ULONG)pHdr);
        for (j = 0; j < pRoom->N0; j++, pPt++) {
            if (func_80071878(pSt->UnkAB, i, j)) {
                func_8007116C(pSt, pP, pPt, func_8007275C(pSt->UnkAB, i, j));
                if ((UCHAR)func_80072AEC(pSt->UnkAB, i, j)) {
                    setRGB0(pP, col, col, col);
                }
                SetSemiTrans(pP, 1);
                addPrim((ULONG *)G.pOt0[5], pP);
                pP += 2;
            }
        }
        for (j = 0; j < pRoom->N1; j++, pPt++) {
            if (func_80072D38(pSt->UnkAB, i, j)) {
                c = func_800738CC(pSt->UnkAB, i, j);
                func_8007116C(pSt, pP, pPt, (c != 0) * 2);
                if (c) {
                    setRGB0(pP, col, col, col);
                }
                addPrim((ULONG *)G.pOt0[4], pP);
                pP += 2;
            }
        }
    }
    f = pSt->UnkAB == pSt->UnkAA;
    if (pSt->UnkAD && (signed char)pSt->UnkAF != (signed char)pSt->UnkAE) {
        f = 0;
    }
    if (f) {
        pRm = (SHORT *)pSt->pUnkB4;
        pRm = (SHORT *)((UCHAR *)pRm + *(ULONG *)pRm);
        pRm += pSt->UnkAC * 4;
        px = G.Player.Pos_x;
        pz = G.Player.Pos_z;
        if (G.Unk783E == 1 && G.Unk7840 == 0xF && px >= -0x2276) {
            px += 0x251D;
        }
        if (G.Unk783E == 3 && G.Unk7840 == 0x15 && G.Player.Pos_x >= 0x320) {
            px = G.Player.Pos_x - 0x2328;
            pz = G.Player.Pos_z;
        }
        dx = pRm[0] + (px - pRm[2]) / 450;
        dy = pRm[1] - (pz - pRm[3]) / 450;
        mx = (dx + pSt->Unk94) * pSt->UnkA4 / 100 - pSt->Unk94 + pSt->Unk7C;
        my = (dy + pSt->Unk98) * pSt->UnkA4 / 100 - pSt->Unk98 + pSt->Unk80;
        pP = (POLY_FT4 *)0x801AFE80;
        if (G.Cbuf_id) {
            pP = (POLY_FT4 *)0x801AFEA8;
        }
        setXY4(pP, mx + 0x9C, my + 0x74, mx + 0xA4, my + 0x74, mx + 0x9C, my + 0x7C, mx + 0xA4, my + 0x7C);
        setRGB0(pP, col, col, col);
        addPrim((ULONG *)G.pOt0[1], pP);
    }
    if (pSt->UnkA9) {
        pP = (POLY_FT4 *)0x801AFED0;
        if (G.Cbuf_id) {
            pP = (POLY_FT4 *)0x801AFEF8;
        }
        for (j = 0; j < 4; j++) {
            if (pSt->UnkAD) {
                if ((ULONG)(j - 2) < 2) {
                    continue;
                }
                if ((signed char)pSt->UnkAE == 0 && j == 1) {
                    continue;
                }
                if ((signed char)pSt->UnkAE == pSt->UnkB0 - 1 && j == 0) {
                    continue;
                }
            }
            i = 0;
            switch (j) {
            case 0:
                if (pSt->Unk80 >= pSt->Unk98 + 0xE0) {
                    i = 1;
                }
                break;
            case 1:
                if (pSt->Unk80 <= pSt->Unk98 - 0xE0) {
                    i = 1;
                }
                break;
            case 2:
                if (pSt->Unk7C >= pSt->Unk94 + 0x140) {
                    i = 1;
                }
                break;
            case 3:
                if (pSt->Unk7C <= pSt->Unk94 - 0x140) {
                    i = 1;
                }
                break;
            }
            if (i) {
                setRGB0(pP, 0x18, 0x18, 0x18);
            } else {
                i = pSt->Unk24;
                setRGB0(pP, 0x80, 0x80, 0x80);
            }
            pA = &D_800A03D0[i][j];
            dx = pA->X;
            dy = pA->Y;
            setXY4(pP, dx - 7, dy - 6, dx + 6, dy - 6, dx - 7, dy + 6, dx + 6, dy + 6);
            setUV4(pP, pA->U0, pA->V0, pA->U1, pA->V0, pA->U0, pA->V1, pA->U1, pA->V1);
            setSemiTrans(pP, 1);
            addPrim((ULONG *)G.pOt0[1], pP);
            pP += 2;
        }
    }
    if (!pSt->UnkAD) {
        pL = (LINE_F2 *)0x801B0010;  /* two TILEs (LINE_F2 shares the layout used here) */
        if (G.Cbuf_id) {
            pL = (LINE_F2 *)0x801B0020;
        }
        setRGB0(pL, pSt->Unk22 + 0x50, pSt->Unk22 + 0x40, pSt->Unk22 & 0x1F);
        addPrim(pOt0, pL);
        pL += 2;
        setRGB0(pL, pSt->Unk22 + 0x50, pSt->Unk22 + 0x40, pSt->Unk22 & 0x1F);
        addPrim(pOt0, pL);
    }
    pL = (LINE_F2 *)0x801B0050;
    if (G.Cbuf_id) {
        pL = (LINE_F2 *)0x801B0060;
    }
    zoom = pSt->UnkA4;
    dy = pSt->Unk98 - pSt->UnkA0;
    dx = pSt->Unk94 - pSt->Unk9C;
    ox = pSt->Unk7C - pSt->Unk94 + 0xA0;
    oy = pSt->Unk80 - pSt->Unk98 + 0x78;
    for (i = 1; i < 0x14; i++) {
        pL->x1 = pL->x0 = (dx + i * 0x20) * zoom / 100 + ox;
        pL->y0 = dy * zoom / 100 + oy;
        pL->y1 = (dy + 0x1C0) * zoom / 100 + oy;
        addPrim(pOt1, pL);
        pL += 2;
    }
    for (i = 1; i < 0xE; i++) {
        pL->x0 = dx * zoom / 100 + ox;
        pL->x1 = (dx + 0x280) * zoom / 100 + ox;
        pL->y1 = pL->y0 = (dy + i * 0x20) * zoom / 100 + oy;
        addPrim(pOt1, pL);
        pL += 2;
    }
    pS = (SPRT *)0x801B0510;
    if (G.Cbuf_id) {
        pS = (SPRT *)0x801B0524;
    }
    addPrim(pOt0, pS);
    pS += 2;
    if (pSt->Rtn1 != 1) {
        if (pSt->UnkA8 == 0) {
            addPrim(pOt0, pS);
            pS += 2;
            addPrim(pOt0, pS);
            pS += 2;
            pS += 4;
        } else {
            pS += 4;
            addPrim(pOt0, pS);
            pS += 2;
            addPrim(pOt0, pS);
            pS += 2;
        }
    } else {
        pS += 8;
    }
    addPrim(pOt0, pS);
    pS += 2;
    for (i = 0; i < (signed char)pSt->UnkAE; i++) {
        pS += 2;
    }
    addPrim(pOt0, pS);
    if (pSt->Rtn1 == 2) {
        pT = (TILE *)0x801B06A0;
        if (G.Cbuf_id) {
            pT = (TILE *)0x801B06B0;
        }
        setXY0(pT, 0x3D, 0x78 - pSt->Unk13 * 8);
        setWH(pT, 0xC8, pSt->Unk13 * 16);
        addPrim(pOt2, pT);
        if (pSt->Rtn2 == 1) {
            Mess_print(0x68, 0x34, D_800A0328 + D_800A038C[7], 0x501);
            if (pSt->UnkB1 < 0x3F) {
                Mess_print(0x47, (signed char)pSt->UnkBB * 16 + 0x4C, D_800A0328 + D_800A038C[8], 0x501);
            }
            for (i = 0; i < 7; i++) {
                if (Flg_ck(D_800D2128, D_800A04F8[i])) {
                    Mess_print(0x55, i * 16 + 0x4C, D_800A0328 + D_800A038C[D_800A04F8[i]], 0x501);
                } else {
                    Mess_print(0x55, i * 16 + 0x4C, D_800A0328 + D_800A038C[9], 0x531);
                }
            }
        }
    }
}

/* needs ST_MAP_PART (see func_80070244.c draft) and ST_WORK Unk7C..UnkA4 */
void func_8007116C(ST_WORK *pSt, POLY_FT4 *p, ST_MAP_PART *pPt, UCHAR clut)
{
    LONG x, y, w, h, ox, oy, zoom;

    if (pPt->Clut == 7) {
        clut = 0;
    }
    w = pPt->U1 - pPt->U0 + 1;
    h = pPt->V1 - pPt->V0 + 1;
    x = pPt->X + pSt->Unk94;
    y = pPt->Y + pSt->Unk98;
    ox = pSt->Unk7C - pSt->Unk94 + 0xA0;
    oy = pSt->Unk80 - pSt->Unk98 + 0x78;
    zoom = pSt->UnkA4;
    SetPolyFT4(p);
    setXY4(p, x * zoom / 100 + ox, y * zoom / 100 + oy, (x + w) * zoom / 100 + ox, y * zoom / 100 + oy, x * zoom / 100 + ox, (y + h) * zoom / 100 + oy, (x + w) * zoom / 100 + ox, (y + h) * zoom / 100 + oy);
    setUV4(p, pPt->U0, pPt->V0, pPt->U1 + 1, pPt->V0, pPt->U0, pPt->V1 + 1, pPt->U1 + 1, pPt->V1 + 1);
    p->tpage = (((pPt->Tpage >> 1) + 7) & 0xF) | 0x10;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->clut = ((pPt->Clut + ((pPt->Tpage & 1) * 8 + 0x1EA) + clut) << 6) | 0x13;
}

void func_800713DC(ST_WORK *pSt, UCHAR no)
{
    ULONG i, sum;

    i = 0;
    pSt->UnkCC = pSt->pBuf;
    CD.Fno = 0x3A;
    sum = 0;
    CD.Fsize = D_800A03AC[no];
    CD.Fsector = (floc[0x3A].sec_high << 16) + floc[0x3A].sector;
    for (; i < no; i++) {
        sum += D_800A03AC[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = D_800A03A0[no];
    Cd_read(0x3A, pSt->UnkCC, 2, "MAP.MAP");
    pSt->pUnkB4 = (ULONG *)(pSt->UnkCC[3] + (ULONG)pSt->UnkCC);
    D_800CCBBC = 0xA17;
    Mapping_tim((ULONG *)(pSt->UnkCC[5] + (ULONG)pSt->UnkCC));
    D_800CCBBC = 0x1A18;
    func_8006EBEC_p((ULONG *)(pSt->UnkCC[7] + (ULONG)pSt->UnkCC));
}

void func_80071510(UCHAR a, UCHAR b)
{
    ULONG *p;

    if (a == 0 && b == 16) {
        D_800D20DC[0] |= 0x80000000U >> (b & 0x1F);
    }
    if (a == 1 && b == 0) {
        D_800D20DC[2] |= 0x80000000U >> (b & 0x1F);
    }
    if (a == 1 && b == 1) {
        D_800D20DC[1] |= 0x10000;
    }
    if (a == 3 && b == 21) {
        D_800D20DC[6] |= 0x80000000U >> (b & 0x1F);
    }
    func_80073CCC(&a, &b);
    p = &D_800D20DC[a * 2 + (b >> 5)];
    *p |= 0x80000000U >> (b & 0x1F);
    Flg_on(&D_800D20DC[19], a);
}

ULONG func_80071668(UCHAR a, UCHAR b)
{
    ULONG *p = &D_800D20DC[a * 2 + (b >> 5)];

    return *p & (0x80000000U >> (b & 0x1F));
}

UCHAR func_80073BC8_u(ST_WORK *pSt, UCHAR st, UCHAR rm) __asm__("func_80073BC8"); /* returns UC here (canonical long) */

LONG func_800716A4(UCHAR st, UCHAR rm)
{
    ST_WORK *pSt = &St;
    USHORT code = pSt->UnkAC | (pSt->UnkAA << 8);
    LONG f;

    switch (st) {
    case 0:
        if ((UCHAR)(rm - 0x11) < 10) {
            return 0;
        }
        break;
    case 1:
        if ((code == 0x10C || code == 0x115) && Flg_ck(D_800D1FA0, 0x3A) && rm != 0xC) {
            return 0;
        }
        if ((USHORT)(G.Unk783E - 2) < 3 && rm == 0xC) {
            return 0;
        }
        if (rm == (code == 0x10F ? 0x10 : 0xF)) {
            return 0;
        }
        if (code == 0x112) {
            if (rm == 0x11) {
                return 0;
            }
        } else if (rm == 0x12) {
            return 0;
        }
        break;
    case 3:
        f = Flg_ck(D_800D1FA0, 0xC4);
        if (rm == (f ? 0x11 : 0x15)) {
            return 0;
        }
        if (code == 0x310) {
            if (rm == 0x11 || rm == 0x15) {
                return 0;
            }
            if (rm == 0xD) {
                return 0;
            }
        } else if (rm == 0x10) {
            return 0;
        }
        break;
    }
    if (St.UnkAD && (signed char)St.UnkAE != func_80073BC8_u(pSt, st, rm)) {
        return 0;
    }
    return 1;
}

LONG func_80071878(UCHAR st, UCHAR rm, UCHAR cut)
{
    tagGLOBAL *pG = &G;
    UCHAR *p = (UCHAR *)St.pUnkB4;
    USHORT cur = St.UnkAC | (St.UnkAA << 8);
    UCHAR fl = pG->Player.nFloor;
    ULONG *pF0 = pG->Unk78D0;
    ULONG *pF2 = pG->Unk7868;
    ULONG *pF1 = G.Unk78F0;
    ULONG *pF3 = G.Unk7994;
    ULONG *pF4 = &G.Unk7888;

    p += ((LONG *)p)[3];
    if (!Flg_ck(pG->Unk79EC, st)) {
        for (; (signed char)p[0] != -1; p += 4) {
            if (p[0] == rm && p[1] == cut) {
                if (p[3]) {
                    if (!Flg_ck(G.Unk7868, p[2])) {
                        return 0;
                    }
                } else {
                    if (!Flg_ck(G.Unk7994, p[2])) {
                        return 0;
                    }
                }
            }
        }
    }
    switch (St.UnkB8) {
    case 1:
        if (st == 0 && rm == 0 && cut == 0) {
            return 0;
        }
        if (st == 4 && rm == 3 && cut == 2) {
            return 0;
        }
        if (st == 4 && rm == 3 && cut == 3) {
            return 0;
        }
        break;
    case 2:
        if (st == 0 && rm == 0 && cut == 1) {
            return 0;
        }
        if (st == 4 && rm == 3 && cut == 0) {
            return 0;
        }
        break;
    default:
        if (st == 0 && rm == 0 && cut == 1) {
            return 0;
        }
        if (st == 4 && rm == 3 && cut == 2) {
            return 0;
        }
        if (st == 4 && rm == 3 && cut == 3) {
            return 0;
        }
        break;
    }
    switch (st) {
    case 0:
        if (cur == 1 && (signed char)fl < 3) {
            if (rm == 0 && cut == 0) {
                return 0;
            }
            if (rm == 1 && cut == 1) {
                return 0;
            }
            if (rm == 1 && cut == 2) {
                return 0;
            }
        } else {
            if (rm == 1 && cut == 0) {
                return 0;
            }
        }
        if (cur == 0xD && rm == 0xD && cut == 1) {
            return 0;
        }
        if (Flg_ck(&G.System_flg, 0x18)) {
            return 1;
        }
        if (cur == 4 || Flg_ck(pF0, 0x3B)) {
            if (rm == 4 && cut == 2) {
                return 0;
            }
            if (rm == 4 && cut == 3) {
                return 0;
            }
        } else {
            if (rm == 4 && cut == 1) {
                return 0;
            }
        }
        if (cur == 7 || Flg_ck(pF0, 0xC)) {
            if (rm == 7 && cut == 1) {
                return 0;
            }
            if (rm == 7 && cut == 2) {
                return 0;
            }
            if (rm == 7 && cut == 3) {
                return 0;
            }
        } else {
            if (rm == 7 && cut == 0) {
                return 0;
            }
        }
        if (cur == 0x1B || cur == 0x1C || Flg_ck(pF0, 0x23)) {
            if (rm == 0x1B && cut == 1) {
                return 0;
            }
            if (rm == 0x1B && cut == 2) {
                return 0;
            }
            if (rm == 0x1B && cut == 3) {
                return 0;
            }
        } else {
            if (rm == 0x1C && cut == 0) {
                return 0;
            }
            if (rm == 0x1B && cut == 0) {
                return 0;
            }
        }
        if (Flg_ck(pF2, 0x8D)) {
            if (rm == 0x2F && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 0x2F && cut == 1) {
                return 0;
            }
        }
        break;
    case 1:
        if (cur == 0x101 && (signed char)fl < 9) {
            if (rm == 0 && cut == 0) {
                return 0;
            }
            if (rm == 1 && cut == 0) {
                return 0;
            }
            if (rm == 1 && cut == 1) {
                return 0;
            }
            if (rm == 1 && cut == 2) {
                return 0;
            }
            if (rm == 1 && cut == 3) {
                return 0;
            }
            if (rm == 1 && cut == 4) {
                return 0;
            }
            if (rm == 1 && cut == 5) {
                return 0;
            }
            if (rm == 1 && cut == 6) {
                return 0;
            }
            if (rm == 1 && cut == 0xB) {
                return 0;
            }
            if (rm == 1 && cut == 0xC) {
                return 0;
            }
            if (rm == 1 && cut == 0xD) {
                return 0;
            }
        } else {
            if (rm == 0 && cut == 4) {
                return 0;
            }
            if (rm == 1 && cut == 7) {
                return 0;
            }
            if (rm == 1 && cut == 8) {
                return 0;
            }
            if (rm == 1 && cut == 9) {
                return 0;
            }
            if (rm == 1 && cut == 0xA) {
                return 0;
            }
        }
        if (cur == 0x104) {
            if (rm == 3 && cut == 1) {
                return 0;
            }
        } else {
            if (rm == 4 && cut == 2) {
                return 0;
            }
        }
        if (cur == 0x10A && (signed char)fl < 4) {
            if (rm == 0xA && cut == 0) {
                return 0;
            }
            if (rm == 0xA && cut == 2) {
                return 0;
            }
            if (rm == 0xA && cut == 3) {
                return 0;
            }
            if (rm == 0xB && cut == 3) {
                return 0;
            }
        } else {
            if (rm == 0xA && cut == 1) {
                return 0;
            }
            if (Flg_ck(pF2, 0xD6)) {
                if (rm == 0xA && cut == 2) {
                    return 0;
                }
            } else {
                if (rm == 0xA && cut == 3) {
                    return 0;
                }
            }
        }
        if (cur == 0x110 && (signed char)fl >= 4) {
            if (rm == 0x10 && cut == 0) {
                return 0;
            }
            if (rm == 0x10 && cut == 1) {
                return 0;
            }
        } else {
            if (rm == 0x10 && cut == 2) {
                return 0;
            }
        }
        if (Flg_ck(&G.System_flg, 0x18)) {
            return 1;
        }
        if (cur == 0x101 || Flg_ck(pF0, 0x2F)) {
            if (rm == 1 && cut == 1) {
                return 0;
            }
            if (rm == 1 && cut == 2) {
                return 0;
            }
            if (rm == 1 && cut == 3) {
                return 0;
            }
            if (rm == 1 && cut == 4) {
                return 0;
            }
            if (rm == 1 && cut == 5) {
                return 0;
            }
            if (rm == 1 && cut == 6) {
                return 0;
            }
            if (rm == 1 && cut == 0xB) {
                return 0;
            }
        } else {
            if (rm == 1 && cut == 0) {
                return 0;
            }
            if (rm == 1 && cut == 0xC) {
                return 0;
            }
            if (rm == 1 && cut == 0xD) {
                return 0;
            }
        }
        if (cur == 0x10C || Flg_ck(pF0, 0x21)) {
            if (rm == 0xC && cut == 3) {
                return 0;
            }
            if (rm == 0xC && cut == 4) {
                return 0;
            }
        } else {
            if (rm == 0xC && cut == 0) {
                return 0;
            }
            if (rm == 0xC && cut == 1) {
                return 0;
            }
            if (rm == 0xC && cut == 2) {
                return 0;
            }
        }
        if (cur == 0x10D || Flg_ck(pF0, 0x22)) {
            if (rm == 0xD && cut == 2) {
                return 0;
            }
            if (rm == 0xD && cut == 3) {
                return 0;
            }
        } else {
            if (rm == 0xD && cut == 0) {
                return 0;
            }
        }
        if (cur == 0x10F || cur == 0x110 || Flg_ck(pF2, 2) || Flg_ck(pF0, 0x5F)) {
            if (rm == 0x10 && cut == 5) {
                return 0;
            }
            if (rm == 0x10 && cut == 6) {
                return 0;
            }
        } else {
            if (rm == 0x10 && cut == 0) {
                return 0;
            }
            if (rm == 0x10 && cut == 1) {
                return 0;
            }
            if (rm == 0x10 && cut == 2) {
                return 0;
            }
            if (rm == 0x10 && cut == 3) {
                return 0;
            }
        }
        if (cur == 0x111 || cur == 0x112 || Flg_ck(pF2, 0xD) || Flg_ck(pF0, 0x40)) {
            if (rm == 0x11 && cut == 1) {
                return 0;
            }
            if (rm == 0x11 && cut == 2) {
                return 0;
            }
            if (rm == 0x11 && cut == 3) {
                return 0;
            }
        } else {
            if (rm == 0x11 && cut == 0) {
                return 0;
            }
        }
        if (cur == 0x113 || cur == 0x114 || Flg_ck(pF0, 0x24)) {
            if (rm == 0x14 && cut == 4) {
                return 0;
            }
            if (rm == 0x14 && cut == 5) {
                return 0;
            }
            if (rm == 0x14 && cut == 6) {
                return 0;
            }
        } else {
            if (rm == 0x13 && cut == 0) {
                return 0;
            }
            if (rm == 0x14 && cut == 0) {
                return 0;
            }
            if (rm == 0x14 && cut == 1) {
                return 0;
            }
            if (rm == 0x14 && cut == 2) {
                return 0;
            }
            if (rm == 0x14 && cut == 3) {
                return 0;
            }
        }
        if (Flg_ck(pF2, 0x8D)) {
            if (rm == 1 && cut == 0xC) {
                return 0;
            }
        } else {
            if (rm == 1 && cut == 0xD) {
                return 0;
            }
        }
        if (Flg_ck(pF2, 0x9D)) {
            if (rm == 0xD && cut == 0) {
                return 0;
            }
            if (rm == 0xD && cut == 2) {
                return 0;
            }
            if (rm == 0xD && cut == 3) {
                return 0;
            }
            if (rm == 0xE && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 0xD && cut == 4) {
                return 0;
            }
            if (rm == 0xE && cut == 2) {
                return 0;
            }
        }
        break;
    case 2:
        if (Flg_ck(pF2, 0xC8)) {
            if (rm == 2 && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 1 && cut == 1) {
                return 0;
            }
        }
        if (Flg_ck(pF2, 0x37)) {
            if (rm == 3 && cut == 0) {
                return 0;
            }
            if (rm == 4 && cut == 0) {
                return 0;
            }
            if (rm == 0xA && cut == 0) {
                return 0;
            }
            if (rm == 0xB && cut == 0) {
                return 0;
            }
            if (rm == 0xC && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 3 && cut == 1) {
                return 0;
            }
            if (rm == 4 && cut == 1) {
                return 0;
            }
            if (rm == 0xA && cut == 1) {
                return 0;
            }
            if (rm == 0xB && cut == 1) {
                return 0;
            }
            if (rm == 0xC && cut == 1) {
                return 0;
            }
        }
        break;
    case 3:
        if (cur == 0x316 && (signed char)fl < 10) {
            if (rm == 0x16 && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 0x16 && cut == 1) {
                return 0;
            }
            if (rm == 0x16 && cut == 2) {
                return 0;
            }
        }
        if (Flg_ck(pF3, 9)) {
            if (rm == 0 && cut == 8) {
                return 0;
            }
            if (rm == 0 && cut == 9) {
                return 0;
            }
            if (rm == 0 && cut == 0xA) {
                return 0;
            }
            if (rm == 0 && cut == 0xB) {
                return 0;
            }
            if (rm == 0 && cut == 0xC) {
                return 0;
            }
        } else {
            if (rm == 1) {
                return 0;
            }
            if (rm == 0xC) {
                return 0;
            }
            if (rm == 0xD) {
                return 0;
            }
            if (rm == 0xE) {
                return 0;
            }
            if (rm == 0xF) {
                return 0;
            }
            if (rm == 0x10) {
                return 0;
            }
            if (rm == 0x11) {
                return 0;
            }
            if (rm == 0x12) {
                return 0;
            }
            if (rm == 0x13) {
                return 0;
            }
            if (rm == 0x14) {
                return 0;
            }
            if (rm == 0x15) {
                return 0;
            }
        }
        if (Flg_ck(pF1, 0x27) && Flg_ck(pF1, 0x36)) {
            if (rm == 0 && cut == 0x10) {
                return 0;
            }
            if (rm == 0 && cut == 0x11) {
                return 0;
            }
        } else {
            if (rm == 0 && cut == 0x12) {
                return 0;
            }
        }
        if (Flg_ck(pF2, 0xBB)) {
            if (rm == 0 && cut == 0) {
                return 0;
            }
            if (rm == 0 && cut == 0x10) {
                return 0;
            }
            if (rm == 0 && cut == 0x11) {
                return 0;
            }
            if (rm == 0 && cut == 0x12) {
                return 0;
            }
        } else {
            if (rm == 0 && cut == 0xD) {
                return 0;
            }
            if (rm == 0 && cut == 0xE) {
                return 0;
            }
            if (rm == 0 && cut == 0x13) {
                return 0;
            }
        }
        if (Flg_ck(G.Unk7868, 0xC4)) {
            if (rm == 0x10 && cut == 4) {
                return 0;
            }
        } else {
            if (rm == 0x10 && cut == 5) {
                return 0;
            }
        }
        break;
    case 4:
        if ((cur == 0x40D && Flg_ck(pF4, 0x1D)) || cur == 0x40E || cur == 0x40F) {
            if (rm == 0xD && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 0xD && cut == 1) {
                return 0;
            }
        }
        if (!Flg_ck(D_800D258C, 0x45)) {
            if (rm == 3 && cut == 1) {
                return 0;
            }
            if (rm == 7 && cut == 1) {
                return 0;
            }
        }
        break;
    }
    return 1;
}

LONG func_8007275C(UCHAR st, UCHAR rm, UCHAR cut)
{
    ST_WORK *pSt = &St;
    LONG cur = pSt->UnkAC | (pSt->UnkAA << 8);
    UCHAR *p;

    if (rm < 0x30 && !func_80071668(st, rm)) {
        return 5;
    }
    switch (st) {
    case 0:
        if (rm == 0x2F && !func_80071668(1, 1)) {
            return 5;
        }
        if (rm == 0x10 && !func_80071668(5, 0)) {
            return 5;
        }
        break;
    case 1:
        if (rm == 0 && !func_80071668(0, 0x26)) {
            return 5;
        }
        if (rm == 0xC && pSt->UnkAC == rm) {
            if (G.Player.Pos_x < -0x2D6E) {
                if (cut == 1) {
                    return 0;
                }
            } else {
                if (cut == 0) {
                    return 0;
                }
            }
        }
        break;
    case 3:
        if (rm == 0) {
            if (cut >= 8 && cut <= 0xC) {
                return 0;
            }
            if (cut == 0xF) {
                return 0;
            }
            if (cut == 0x10) {
                return 0;
            }
            if (cut == 0x11) {
                return 0;
            }
            if (cut == 0x12) {
                return 0;
            }
            if (cut == 0x13) {
                return 0;
            }
        }
        break;
    case 4:
        if (cur == 0x403) {
            if (Flg_ck(G.Unk7868, 0xF1)) {
                if (G.Cut_no_old == 0x503) {
                    if (rm == 3) {
                        if (cut == 0) {
                            return 0;
                        }
                        if (cut == 1) {
                            return 2;
                        }
                    }
                } else {
                    if (rm == 3) {
                        if (cut == 0) {
                            return 0;
                        }
                        if (cut == 1) {
                            return 0;
                        }
                    }
                    if (rm == 7 && cut == 1) {
                        return 2;
                    }
                }
            } else {
                if (rm == 3 && cut == 1) {
                    return 0;
                }
            }
        }
        if (rm == 7 && cut == 1) {
            return 0;
        }
        break;
    }
    if (!Flg_ck(&G.System_flg, 0x18)) {
        p = (UCHAR *)St.pUnkB4;
        for (p += ((LONG *)p)[3]; (signed char)p[0] != -1; p += 4) {
            if (p[0] == rm && p[1] == cut) {
                if (p[3]) {
                    if (!Flg_ck(G.Unk7868, p[2])) {
                        return 5;
                    }
                } else {
                    if (!Flg_ck(G.Unk7994, p[2])) {
                        return 5;
                    }
                }
            }
        }
    }
    if (pSt->UnkAB == pSt->UnkAA && rm == pSt->UnkAC) {
        return 2;
    }
    return 0;
}

LONG func_80072AEC(UCHAR st, UCHAR rm, UCHAR cut)
{
    switch (St.UnkB8) {
    case 1:
        if (st == 0 && rm == 0 && cut == St.UnkB8) {
            return 1;
        }
        break;
    case 2:
        if (st == 4 && rm == 3 && cut == St.UnkB8) {
            return 1;
        }
        break;
    }
    if (Flg_ck(&G.System_flg, 0x18)) {
        return 0;
    }
    switch (st) {
    case 0:
        if (rm == 4 && cut == 3) {
            return 1;
        }
        if (rm == 7 && cut == 3) {
            return 1;
        }
        if (rm == 0x1B && cut == 3) {
            return 1;
        }
        break;
    case 1:
        if (rm == 1 && cut == 4) {
            return 1;
        }
        if (rm == 1 && cut == 5) {
            return 1;
        }
        if (rm == 1 && cut == 6) {
            return 1;
        }
        if (rm == 1 && cut == 0xB) {
            return 1;
        }
        if (rm == 0xC && cut == 4) {
            return 1;
        }
        if (rm == 0xD && cut == 3) {
            return 1;
        }
        if (rm == 0x10 && cut == 6) {
            return 1;
        }
        if (rm == 0x11 && cut == 3) {
            return 1;
        }
        if (rm == 0x14 && cut == 6) {
            return 1;
        }
        break;
    case 3:
        if (rm == 0) {
            if (cut == 9) {
                return 1;
            }
            if (cut == 0xA) {
                return 1;
            }
            if (cut == 0xB) {
                return 1;
            }
            if (cut == 0xC) {
                return 1;
            }
            if (cut == 0x11) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

LONG func_80072D38(UCHAR st, UCHAR rm, UCHAR cut)
{
    tagGLOBAL *pG = &G;
    UCHAR fl = pG->Player.nFloor;
    LONG cur = St.UnkAC | (St.UnkAA << 8);
    UCHAR f = Flg_ck(pG->Unk79EC, st);
    ULONG *pF4 = &G.Unk7888;
    ULONG *pF3 = G.Unk7994;
    ULONG *pF0 = pG->Unk78D0;
    ULONG *pF1 = pG->Unk78F0;
    ULONG *pF2 = pG->Unk7868;
    ULONG *pS = &pG->Status_flg;
    UCHAR *p;

    switch (st) {
    case 0:
        if (cur == 1 && (signed char)fl < 3) {
            if (rm == 0 && cut == 0) {
                return 0;
            }
            if (rm == 1 && cut == 0) {
                return 0;
            }
        }
        if (cur != 0xD && rm == 0xD) {
            return 0;
        }
        if (Flg_ck(&G.System_flg, 0x18)) {
            return 1;
        }
        if (cur != 7 && !Flg_ck(pF0, 0xC) && rm == 7) {
            return 0;
        }
        if (cur != 0x1B && cur != 0x1C && !Flg_ck(pF0, 0x23)) {
            if (rm == 0x1B) {
                return 0;
            }
            if (rm == 0x1C) {
                return 0;
            }
        }
        if (!Flg_ck(pF2, 0x56) && rm == 0x26 && cut == 2) {
            return 0;
        }
        break;
    case 1:
        if (cur == 0x101 && (signed char)fl < 9) {
            if (rm == 0 && cut == 0) {
                return 0;
            }
            if (rm == 0 && cut == 1) {
                return 0;
            }
            if (rm == 1 && cut == 0) {
                return 0;
            }
            if (rm == 1 && cut == 1) {
                return 0;
            }
        } else {
            if (rm == 1 && cut == 2) {
                return 0;
            }
        }
        if (cur == 0x104) {
            if (rm == 3 && cut == 1) {
                return 0;
            }
        } else {
            if (rm == 4 && cut == 0) {
                return 0;
            }
        }
        if (cur == 0x10A && (signed char)fl < 4) {
            if (rm == 0xA && cut == 0) {
                return 0;
            }
            if (rm == 0xA && cut == 1) {
                return 0;
            }
            if (rm == 0xB && cut == 0) {
                return 0;
            }
            if (rm == 0xB && cut == 3) {
                return 0;
            }
        } else {
            if (rm == 0xA && cut == 2) {
                return 0;
            }
        }
        if (!Flg_ck(pF2, 0xD6) && rm == 0xB && cut == 3) {
            return 0;
        }
        if (cur == 0x110 && (signed char)fl >= 4) {
            if (rm == 0x10 && cut == 0) {
                return 0;
            }
            if (rm == 0x10 && cut == 1) {
                return 0;
            }
            if (rm == 0x10 && cut == 2) {
                return 0;
            }
        } else {
            if (rm == 0x10 && cut == 3) {
                return 0;
            }
        }
        if (Flg_ck(&G.System_flg, 0x18)) {
            if (rm == 0x10 && cut == 1) {
                return 0;
            }
            if (rm == 0x10 && cut == 2) {
                return 0;
            }
            return 1;
        }
        if (cur != 0x101 && !Flg_ck(pF0, 0x2F) && rm == 1) {
            return 0;
        }
        if (cur != 0x10C && !Flg_ck(pF0, 0x21) && rm == 0xC) {
            return 0;
        }
        if (cur != 0x10D && !Flg_ck(pF0, 0x22) && rm == 0xD) {
            return 0;
        }
        if (cur != 0x10F && cur != 0x110 && !Flg_ck(pF2, 2) && !Flg_ck(pF0, 0x5F) && rm == 0x10) {
            return 0;
        }
        if (cur != 0x111 && cur != 0x112 && !Flg_ck(pF2, 0xD) && !Flg_ck(pF0, 0x40) && rm == 0x11) {
            return 0;
        }
        if (cur != 0x113 && cur != 0x114 && !Flg_ck(pF0, 0x24)) {
            if (rm == 0x13) {
                return 0;
            }
            if (rm == 0x14) {
                return 0;
            }
        }
        if (!Flg_ck(pF2, 0x56) && rm == 0 && cut == 1) {
            return 0;
        }
        if (Flg_ck(pF2, 0x9D)) {
            if (rm == 0xD) {
                return 0;
            }
            if (rm == 0xE && cut == 1) {
                return 0;
            }
        }
        if (!Flg_ck(pF2, 0x60) && rm == 0x14 && cut == 8) {
            return 0;
        }
        if ((USHORT)G.Unk783E >= 2 && (USHORT)G.Unk783E <= 4) {
            if (rm == 0xB && cut == 1) {
                return 0;
            }
            if (rm == 0xB && cut == 2) {
                return 0;
            }
            if (rm == 0xC) {
                return 0;
            }
        }
        break;
    case 2:
        if (Flg_ck(pF2, 0xC8)) {
            if (rm == 1 && cut == 2) {
                return 0;
            }
            if (rm == 2 && cut == 0) {
                return 0;
            }
        }
        if (Flg_ck(pF2, 0x37)) {
            if (rm == 1 && cut == 1) {
                return 0;
            }
            if (rm == 3) {
                return 0;
            }
            if (rm == 4 && cut == 1) {
                return 0;
            }
            if (rm == 0xA) {
                return 0;
            }
            if (rm == 0xB) {
                return 0;
            }
            if (rm == 0xC) {
                return 0;
            }
        }
        break;
    case 3:
        if (!(cur == 0x316 && (signed char)fl < 10) && rm == 0x16 && cut == 2) {
            return 0;
        }
        if (!Flg_ck(pF3, 9)) {
            if (rm == 1) {
                return 0;
            }
            if (rm == 0xC) {
                return 0;
            }
            if (rm == 0xD) {
                return 0;
            }
            if (rm == 0xE) {
                return 0;
            }
            if (rm == 0xF) {
                return 0;
            }
            if (rm == 0x10) {
                return 0;
            }
            if (rm == 0x11) {
                return 0;
            }
            if (rm == 0x12) {
                return 0;
            }
            if (rm == 0x13) {
                return 0;
            }
            if (rm == 0x14) {
                return 0;
            }
            if (rm == 0x15) {
                return 0;
            }
        }
        if ((!Flg_ck(pF1, 0x27) || !Flg_ck(pF1, 0x36)) && rm == 0 && cut == 4) {
            return 0;
        }
        if (Flg_ck(pF2, 0xBB)) {
            if (rm == 0 && cut == 3) {
                return 0;
            }
            if (rm == 0 && cut == 4) {
                return 0;
            }
        }
        break;
    case 4:
        if (!Flg_ck(pF2, 0xEA) && rm == 0 && cut == 6) {
            return 0;
        }
        if (Flg_ck(pS, 5)) {
            if (rm == 0 && cut == 4) {
                return 0;
            }
            if (rm == 0 && cut == 5) {
                return 0;
            }
            if (rm == 0xA && cut == 1) {
                return 0;
            }
            if (rm == 0xB && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 0 && cut == 7) {
                return 0;
            }
            if (rm == 0 && cut == 8) {
                return 0;
            }
            if (rm == 0xA && cut == 2) {
                return 0;
            }
            if (rm == 0xB && cut == 1) {
                return 0;
            }
        }
        if (Flg_ck(pF3, 0x23)) {
            if (rm == 3 && cut == 1) {
                return 0;
            }
            if (rm == 8 && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 3 && cut == 3) {
                return 0;
            }
            if (rm == 8 && cut == 2) {
                return 0;
            }
        }
        if (cur == 0x409 && Flg_ck(pF4, 0x1A)) {
            if (rm == 8 && cut == 1) {
                return 0;
            }
            if (rm == 9 && cut == 0) {
                return 0;
            }
        } else {
            if (rm == 8 && cut == 3) {
                return 0;
            }
            if (rm == 9 && cut == 1) {
                return 0;
            }
        }
        if (cur == 0x40D) {
            if (rm == 0xC && cut == 1) {
                return 0;
            }
            if (rm == 0xD && cut == 0) {
                return 0;
            }
            if (Flg_ck(pF4, 4)) {
                if (rm == 0xD && cut == 1) {
                    return 0;
                }
                if (rm == 0xF && cut == 0) {
                    return 0;
                }
            } else {
                if (rm == 0xD && cut == 2) {
                    return 0;
                }
                if (rm == 0xF && cut == 1) {
                    return 0;
                }
            }
        }
        break;
    }
    p = (UCHAR *)St.pUnkB4;
    p += ((LONG *)p)[4];
    while ((signed char)p[0] != -1) {
        if (p[0] == rm && p[1] == cut) {
            if (Flg_ck(&G.System_flg, 0x18)) {
                return 0;
            }
            if (f) {
                if (p[3] & 1) {
                    if (Flg_ck(G.Unk7868, p[2])) {
                        return 0;
                    }
                } else {
                    if (Flg_ck(G.Unk7994, p[2])) {
                        return 0;
                    }
                }
            } else {
                if (p[3] & 0x10) {
                    return 0;
                }
                if (p[3] & 1) {
                    if (Flg_ck(G.Unk7868, p[2])) {
                        return 0;
                    }
                } else {
                    if (Flg_ck(G.Unk7994, p[2])) {
                        return 0;
                    }
                }
            }
        }
        p += 4;
    }
    return 1;
}

LONG func_800738CC(UCHAR st, UCHAR rm, UCHAR cut)
{
    UCHAR *p;

    if (Flg_ck(&G.System_flg, 0x18)) {
        switch (st) {
        case 0:
            if (rm == 9 && cut == 3) {
                return 1;
            }
            if (rm == 0xC && cut == 0) {
                return 1;
            }
            if (rm == 6 && cut == 3) {
                return 1;
            }
            if (rm == 0xF && cut == 0) {
                return 1;
            }
            if (rm == 0xA && cut == 2) {
                return 1;
            }
            if (rm == 0x10 && cut == 0) {
                return 1;
            }
            break;
        case 1:
            if (rm == 0xB && cut == 2) {
                return 1;
            }
            if (rm == 0xC && cut == 1) {
                return 1;
            }
            if (rm == 0xC && cut == 2) {
                return 1;
            }
            if (rm == 0xC && cut == 3) {
                return 1;
            }
            if (rm == 6 && cut == 1) {
                return 1;
            }
            if (rm == 0x16 && cut == 0) {
                return 1;
            }
            if (rm == 4 && cut == 2) {
                return 1;
            }
            if (rm == 0x14 && cut == 1) {
                return 1;
            }
            if (rm == 8 && cut == 1) {
                return 1;
            }
            if (rm == 8 && cut == 2) {
                return 1;
            }
            break;
        }
        return 0;
    }
    p = (UCHAR *)St.pUnkB4;
    for (p += ((LONG *)p)[2]; (signed char)p[0] != -1; p += 4) {
        if (p[0] == rm && p[1] == cut) {
            switch (p[3]) {
            case 0:
                if (!Flg_ck(G.Unk7994, p[2])) {
                    return 1;
                }
                break;
            case 1:
                if (!Flg_ck(G.Unk7868, p[2])) {
                    return 1;
                }
                break;
            case 2:
                if (!Flg_ck(G.Unk7890, p[2])) {
                    return 1;
                }
                break;
            case 3:
                if (!Flg_ck(G.Unk78B0, p[2])) {
                    return 1;
                }
                break;
            }
        }
    }
    return 0;
}

LONG func_80073BC8(ST_WORK *pSt, UCHAR st, UCHAR rm)
{
    switch (st) {
    case 2:
        if (rm == 12) {
            return 2;
        }
        return rm >= 10;
    case 4:
        if (rm < 4 || (rm >= 8 && rm < 11)) {
            return 1;
        }
        if ((rm >= 11 && rm < 18) || rm == 6 || (rm >= 4 && rm < 8)) {
            return 0;
        }
        break;
    case 5:
        return rm >= 8;
    case 6:
        if (rm >= 5 && rm < 9) {
            return 2;
        }
        if (rm < 5) {
            return 1;
        }
        if (rm < 9) {
            break;
        }
        return 0;
    }
    return 0;
}

void func_80073CCC(UCHAR *pA, UCHAR *pB)
{
    LONG i;

    if (*pA == 1 && *pB == 15 && G.Player.Pos_x >= -0x2276) {
        *pB = 16;
        return;
    }
    if (*pA == 3 && *pB == 21 && G.Player.Pos_x >= 800) {
        *pB = 0x11;
        return;
    }
    if (*pA == 4 && *pB == 2 && Flg_ck(&G.Unk7888, 0x1F)) {
        *pB = 0x11;
        return;
    }
    if (*pA == 2 && *pB == 0x17 && G.Player.Pos_z >= -0x2C24 && !Flg_ck(G.Unk7868, 200)) {
        *pB = 2;
    }
    if (*pA >= 5) {
        *pA -= 5;
    }
    for (i = 0; D_800A0430[i].A0 != 0xFF; i++) {
        if (D_800A0430[i].A0 == *pA && D_800A0430[i].B0 == *pB) {
            *pA = D_800A0430[i].A1;
            *pB = D_800A0430[i].B1;
        }
    }
}

void func_80073E7C(void) {
}
