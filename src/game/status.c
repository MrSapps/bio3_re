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

/* map data (St.pMap_data) */
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

/* local prototypes (this module) */
void St_file_load(ST_WORK *pSt)
{
    ULONG i, sum, n;
    ULONG t;
    ULONG *p;

    i = 0;
    sum = 0;
    t = File_page_top[pSt->File_no];
    pSt->Page_no = 0;
    CD.Fno = 0x1C;
    n = t - 1;
    CD.Fsize = File_page_size[n];
    CD.Fsector = (floc[0x1C].sec_high << 16) + floc[0x1C].sector;
    for (; i < n; i++) {
        sum += File_page_size[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = File_page_sum[t - 1];
    Cd_read(0x1C, pSt->pBuf, 2, D_80010DCC);
    p = pSt->pBuf;
    Mapping_tpage_clut = 0xA17;
    St_mapping_tim(p);
    St_file_page_load(pSt, pSt->File_no, pSt->Page_no);
    pSt->Rtn1 = 0;
    pSt->Blink_timer = 0;
    pSt->File_exit = 0;
    pSt->Rtn0++;
    pSt->Flg &= ~0x40000000;
    pSt->Flg &= ~0x80000000;
    pSt->Flg |= 0x100000;
    pSt->Flg |= 0x80000;
    pSt->Flg &= ~0x40000;
    pSt->Flg &= ~0x20000;
    St_file_prim_init(pSt);
}

INCLUDE_RODATA("asm/nonmatchings/game/status", D_80010DCC);

void St_file_read(ST_WORK *pSt)
{
    switch (pSt->Rtn1) {
    case 0:
        if (G.Key_trg & 0x2000) {
            goto exit;
        }
        if (G.Unk2100 & 0x2000) {
            if (pSt->Page_no >= File_page_num[pSt->File_no] - 1) {
                Snd_se_stad(4, 0, 0, 0);
                pSt->File_exit = 1;
                pSt->Rtn1 = 4;
            }
        }
        if ((USHORT)G.Unk20F8 & 0x8000) {
            if (pSt->Page_no != 0) {
                pSt->Page_no--;
                pSt->Page_cnt = 0;
                pSt->Page_spd = 0;
                pSt->Page_acc = 2;
                pSt->Blink_timer = 0;
                pSt->Flg &= ~0x80000;
                pSt->Flg &= ~0x40000;
                pSt->Flg &= ~0x20000;
                Snd_se_stad(8, 0, 0, 0);
                pSt->Rtn1++;
            }
        }
        if ((USHORT)G.Unk20F8 & 0x2000) {
            if (pSt->Page_no < File_page_num[pSt->File_no] - 1) {
                pSt->Page_no++;
                pSt->Page_cnt = 0;
                pSt->Page_spd = 0;
                pSt->Page_acc = -2;
                pSt->Blink_timer = 0;
                pSt->Flg &= ~0x80000;
                pSt->Flg &= ~0x40000;
                pSt->Flg &= ~0x20000;
                Snd_se_stad(8, 0, 0, 0);
                pSt->Rtn1++;
            }
        }
        if (++pSt->Blink_timer >= 0x15) {
            pSt->Blink_timer = -0x14;
        }
        if (pSt->Blink_timer >= 0) {
            pSt->Pos[20].X = -1;
            pSt->Pos[19].X = 1;
        } else {
            pSt->Pos[20].X = 0;
            pSt->Pos[19].X = 0;
        }
        break;
    case 1:
        pSt->Page_spd += pSt->Page_acc;
        pSt->Pos[18].X += pSt->Page_spd;
        if (++pSt->Page_cnt >= 0x12) {
            pSt->Rtn1++;
            pSt->Page_cnt = 0;
            pSt->Pos[18].X = -pSt->Pos[18].X;
            pSt->Page_spd += pSt->Page_acc;
            pSt->Page_acc = -pSt->Page_acc;
        }
        break;
    case 2:
        St_file_page_load(pSt, pSt->File_no, pSt->Page_no);
        pSt->Rtn1++;
        break;
    case 3:
        pSt->Page_spd += pSt->Page_acc;
        pSt->Pos[18].X += pSt->Page_spd;
        if (++pSt->Page_cnt >= 0x13) {
            pSt->Rtn1 = 0;
            pSt->Pos[18].X = 0;
            if (pSt->Page_no != 0) {
                pSt->Flg |= 0x40000;
            }
            if (pSt->Page_no < File_page_num[pSt->File_no] - 1) {
                pSt->Flg |= 0x80000;
            }
            if (pSt->Page_no >= File_page_num[pSt->File_no] - 1) {
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
            pSt->File_exit = 0;
            pSt->Rtn1 = 0;
        }
        if (++pSt->Blink_timer >= 0x15) {
            pSt->Blink_timer = -0x14;
        }
        if (pSt->Blink_timer >= 0) {
            pSt->Pos[20].X = -1;
            pSt->Pos[19].X = 1;
        } else {
            pSt->Pos[20].X = 0;
            pSt->Pos[19].X = 0;
        }
        break;
    }
}

void St_file_end(ST_WORK *pSt)
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
            pSt->Timer = 0;
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
            func_8001B894(0x705, &G.Ob_model[n & 0xFF].M);
            func_8001B894(0x905, 0);
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

void St_file_prim_init(ST_WORK *pSt)
{
    ULONG f;
    UCHAR c;

    SetDrawTPage(&pSt->Tpage[0][0], 0, 0, 0x97);
    SetDrawTPage(&pSt->Tpage[1][0], 0, 0, 0x97);
    SetDrawTPage(&pSt->Tpage[0][1], 0, 0, 0x18);
    SetDrawTPage(&pSt->Tpage[1][1], 0, 0, 0x18);
    St_sprt_init(pSt, (void *)0x801ADA90, D_8009F2EC, (pSt->Clut_page << 8) | 1);
    c = pSt->Clut_page + 1;
    St_sprt_init(pSt, (void *)0x801ADAB8, D_8009F2F8, (c << 8) | 1);
    c = pSt->Clut_parts + 2;
    f = (c << 8) | 0x10001;
    St_sprt_init(pSt, (void *)0x801ADAE0, D_8009F310, f);
    St_sprt_init(pSt, (void *)0x801ADB08, D_8009F304, f);
    St_sprt_init(pSt, (void *)0x801ADB80, D_8009F31C, (pSt->Clut_parts << 8) | 0x10001);
}

void St_file_draw(ST_WORK *pSt)
{
    SPRT *p;
    UCHAR c;

    AddPrim(G.pOt0 + 4, pSt->Tpage[G.Cbuf_id]);
    AddPrim(G.pOt0 + 3, &pSt->Tpage[G.Cbuf_id][1]);
    AddPrim(G.pOt0 + 2, &pSt->Ot[G.Cbuf_id][16]);
    St_sprt_draw_tp(pSt, (SPRT *)0x801ADA90, D_8009F2EC, 0x1101);
    if (pSt->File_no == 0x1B) {
        p = (SPRT *)0x801ADA90;
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ADAA4;
        }
        p->y0 = (USHORT)pSt->Pos[17].Y + 0x24;
    }
    St_sprt_draw_tp(pSt, (SPRT *)0x801ADAB8, D_8009F2F8, 0x11201);
    if (pSt->Flg & 0x80000) {
        St_sprt_draw(pSt, (SPRT *)0x801ADAE0, D_8009F310, 0x101301);
    }
    if (pSt->Flg & 0x40000) {
        St_sprt_draw(pSt, (SPRT *)0x801ADB08, D_8009F304, 0x101401);
    }
    if (pSt->Flg & 0x20000) {
        p = (SPRT *)0x801ADB80;
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ADB94;
        }
        if (pSt->File_exit == 0) {
            c = pSt->Clut_parts + 7;
        } else {
            c = pSt->Clut_parts;
        }
        p->clut = ((c + 0x1E0) << 6) | 0x13;
        St_sprt_draw(pSt, (SPRT *)0x801ADB80, D_8009F31C, 0x101501);
    }
}

void St_file_page_load(ST_WORK *pSt, UCHAR a, UCHAR b)
{
    ULONG i, sum, n;
    ULONG *p;

    i = 0;
    sum = 0;
    n = File_page_top[a] + b;
    CD.Fno = 0x1C;
    CD.Fsize = File_page_size[n];
    CD.Fsector = (floc[0x1C].sec_high << 16) + floc[0x1C].sector;
    for (; i < n; i++) {
        sum += File_page_size[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = File_page_sum[n];
    Cd_read(0x1C, pSt->pBuf, 2, D_80010DCC);
    p = pSt->pBuf;
    Mapping_tpage_clut = 0xB18;
    Mapping_tim(p);
}

void St_sprt_draw_tp(ST_WORK *pSt, SPRT *p, ST_SPR_DEF *pDef, ULONG prm)
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
    for (i = 0; i < (UCHAR)n; i++, p += 2, pDef++) {
        p->x0 = pDef->X + ox;
        p->y0 = pDef->Y + oy;
        AddPrim(&pSt->Tpage[G.Cbuf_id][ot], p);
    }
}

void St_box_init(ST_WORK *pSt)
{
    ULONG a[2];
    pSt->Rtn0++;
    pSt->Rtn1 = 0;
    pSt->Blink = 0;
    pSt->Blink_dir = 0;
    pSt->Box_fast = 0;
    pSt->Flg |= 0x4000000;
    pSt->Flg |= 0x20000000;
}

void St_box_main(ST_WORK *pSt)
{
    St_box_tbl[pSt->Rtn1](pSt);
}

void St_box_exit(ST_WORK *pSt)
{
    pSt->Rtn0 = 0xD;
}

void St_box_inv_select(ST_WORK *pSt)
{
    ULONG i;
    UCHAR id;
    LONG f;

    if ((G.Unk20FC & 0x20) | (G.Key_trg & 0x2000)) {
        pSt->Rtn0++;
        Snd_se_stad(5, 0, 0, 0);
    } else if (G.Key_trg & 0x1000) {
        if ((signed char)pSt->Cursor >= 0) {
            id = G.pItem_set->Item[(signed char)pSt->Cursor].Id;
            for (i = 0; i < 1; i++) {
                if (id == Box_ng_item[i]) {
                    Mess_set(0, 0x1000, 0xB, 0);
                    pSt->Rtn1 = 4;
                    pSt->Flg &= ~0x20000000;
                    return;
                }
            }
            pSt->Rtn1 = 1;
            Snd_se_stad(6, 0, 0, 0);
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
        }
        if ((signed char)pSt->Cursor <= -1) {
            pSt->Rtn0++;
            Snd_se_stad(5, 0, 0, 0);
        }
    } else {
        f = 0;
        if ((G.Unk2100 & 0x1000) && (signed char)pSt->Cursor >= 0) {
            pSt->Cursor -= 2;
            f = 1;
        }
        if (G.Unk2100 & 0x4000) {
            if ((signed char)pSt->Cursor < G.pItem_set->Num - 2) {
                pSt->Cursor += 2;
                f = 1;
            }
        }
        if (G.Unk2100 & 0x8000) {
            if (pSt->Cursor & 1) {
                if ((signed char)--pSt->Cursor >= 0) {
                    f = 1;
                }
            }
        }
        if (G.Unk2100 & 0x2000) {
            if (!(pSt->Cursor & 1)) {
                if ((signed char)++pSt->Cursor >= 0) {
                    f = 1;
                }
            }
        }
        if (f == 1) {
            Snd_se_stad(4, 0, 0, 0);
        }
        if ((signed char)pSt->Cursor >= 0) {
            pSt->Pos[12].X = (pSt->Cursor & 1) * 40;
            pSt->Pos[12].Y = ((signed char)pSt->Cursor >> 1) * 30;
        }
        if ((signed char)pSt->Cursor <= -1) {
            pSt->Pos[12].X = 0;
            pSt->Pos[12].Y = 0;
        }
    }
}

void St_box_select(ST_WORK *pSt)
{
    ITEM_WORK tmp;
    RECT r;
    ITEM_WORK *pI;
    ITEM_WORK *pB;
    tagGLOBAL *pG;
    signed char n;
    ITEM_DATA *pData;
    signed char no;
    signed char i;
    UCHAR f;
    UCHAR m;

    if (G.Key_trg & 0x2000) {
        pSt->Rtn1 = 0;
        pSt->Blink = 0;
        pSt->Blink_dir = 0;
        Snd_se_stad(5, 0, 0, 0);
        return;
    }
    if (G.Key_trg & 0x1000) {
        no = pSt->Cursor;
        pI = &G.pItem_set->Item[no];
        if (pI->Id == 0) {
            no = Search_item_id(0, 0);
        }
        do {
            if (G.pItem_set->Equip_no == no) {
                G.pItem_set->Equip_id = 0;
                G.pItem_set->Equip_no = 0xFF;
                Pix_move(10, 11);
            }
            pData = &Item_data_tbl[pI->Id];
            switch (pData->Type) {
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
            pB = &D_800D23B4[pSt->Box_cursor + 10];
            m = 0;
            if (pI->Id == pB->Id && f == 1) {
                m = pData->Max >= pI->Num + pB->Num;
            }
            if (pI->Id == 0x20 && pB->Id == 0x2A && pI->Num + pB->Num < 4) {
                m = 2;
            }
            if (pI->Id == 0x2A && pB->Id == 0x20 && pI->Num + pB->Num < 4) {
                m = 3;
            }
            switch (m) {
            case 0:
                St_icon_swap(no, pSt->Box_cursor % 8);
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
                St_icon_swap(no, pSt->Box_cursor % 8);
                goto clear;
            case 1:
            case 3:
                pI->Num += pB->Num;
            clear:
                St_icon_copy(pSt->Box_cursor % 8, 11);
                pB->Id = 0;
                pB->Num = 0;
                pB->Flg = 0;
                break;
            }
        } while (0);
        if (pI->Id == 0 && pB->Id != 0) {
            pSt->Rtn1 = 2;
            pSt->Rtn2 = no;
            pSt->Timer = 2;
        } else {
            pSt->Rtn1 = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
        }
        Snd_se_stad(6, 0, 0, 0);
        return;
    }
    pSt->Flg &= ~0x10000;
    pSt->Flg &= ~0x8000;
    if ((USHORT)G.Unk20F8 & 0x1000) {
        if (--pSt->Box_cursor < 0) {
            pSt->Box_cursor = 0x3F;
        }
        n = pSt->Box_cursor - 2;
        if (n < 0) {
            n = pSt->Box_cursor + 0x3E;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->Box_scroll = -0x14;
        pSt->Rtn1 = 3;
        pSt->Box_scroll_dir = 0;
        pSt->Flg |= 0x10000;
    }
    if ((USHORT)G.Unk20F8 & 0x4000) {
        if (++pSt->Box_cursor >= 0x40) {
            pSt->Box_cursor = 0;
        }
        n = pSt->Box_cursor + 2;
        if (n >= 0x40) {
            n = pSt->Box_cursor - 0x3E;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->Box_scroll_dir = 1;
        pSt->Box_scroll = 0x14;
        pSt->Rtn1 = 3;
        pSt->Flg |= 0x8000;
    }
    if (G.Unk20F8 & 0x5000) {
        pI = &G.pItem_set->Item[n + 10];
        Pix_unpack_box(n, pI->Id, 0x801A1500, (ULONG *)0x801A1000);
        DrawSync(0);
    }
    if (pSt->Rtn1 == 3) {
        return;
    }
    if ((G.Unk20F8 & 0xC) == 0) {
        pSt->Box_fast = 0;
    } else if ((G.Unk20F8 & 0xC) == 0xC) {
        if (G.Unk20FC & 4) {
            pSt->Box_fast = 1;
        }
        if (G.Unk20FC & 8) {
            pSt->Box_fast = 2;
        }
    } else {
        if ((USHORT)G.Unk20F8 & 4) {
            pSt->Box_fast = 1;
        }
        if ((USHORT)G.Unk20F8 & 8) {
            pSt->Box_fast = 2;
        }
    }
    if ((G.Unk2100 & 4) && pSt->Box_fast == 1) {
        if ((pSt->Box_cursor -= 5) < 0) {
            pSt->Box_cursor += 0x40;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->Flg |= 0x10000;
    } else if ((G.Unk2100 & 8) && pSt->Box_fast == 2) {
        if ((pSt->Box_cursor += 5) >= 0x40) {
            pSt->Box_cursor -= 0x40;
        }
        Snd_se_stad(0x215, 0, 0, 0);
        pSt->Box_fast = 2;
        pSt->Flg |= 0x8000;
    }
    pG = &G;
    if (*(ULONG *)&pG->Unk2100 & 0xC) {
        r.w = 20;
        r.h = 30;
        n = pSt->Box_cursor - 2;
        if (n < 0) {
            n = pSt->Box_cursor + 0x3E;
        }
        for (i = 0; i < 5; i++) {
            pI = &G.pItem_set->Item[n + 10];
        Pix_unpack_box(n, pI->Id, 0x801A1500, (ULONG *)0x801A1000);
            DrawSync(0);
            if (++n >= 0x40) {
                n = 0;
            }
        }
    }
}

void St_box_pack(ST_WORK *pSt)
{
    UCHAR no;
    LONG n;

    if (pSt->Timer == 0) {
        no = pSt->Rtn2;
        if (pSt->Rtn2 >= G.pItem_set->Num - 1) {
            pSt->Rtn1 = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
            return;
        }
        n = no;
        G.pItem_set->Item[n].Id = G.pItem_set->Item[n + 1].Id;
        G.pItem_set->Item[n].Num = G.pItem_set->Item[n + 1].Num;
        G.pItem_set->Item[n].Flg = G.pItem_set->Item[n + 1].Flg;
        G.pItem_set->Item[n + 1].Id = 0;
        G.pItem_set->Item[n + 1].Num = 0;
        G.pItem_set->Item[n + 1].Flg = 0;
        Pix_swap(no, no + 1);
        if (G.pItem_set->Equip_no == n + 1) {
            G.pItem_set->Equip_no = n;
            G.pItem_set->Equip_id = G.pItem_set->Item[G.pItem_set->Equip_no].Id;
        }
        if ((signed char)pSt->Cursor == n + 1) {
            pSt->Cursor--;
            pSt->Pos[12].X = (pSt->Cursor & 1) * 40;
            pSt->Pos[12].Y = ((signed char)pSt->Cursor >> 1) * 30;
        }
        pSt->Rtn2++;
        if (G.pItem_set->Item[n + 2].Id == 0) {
            pSt->Rtn1 = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
        }
        pSt->Timer = 2;
    }
    pSt->Timer--;
}

void St_box_scroll(ST_WORK *pSt)
{
    if (pSt->Box_scroll_dir == 0) {
        if (pSt->Box_scroll >= -4) {
            pSt->Rtn1 = 1;
        }
        pSt->Box_scroll += 4;
    } else {
        if (pSt->Box_scroll < 5) {
            pSt->Rtn1 = 1;
        }
        pSt->Box_scroll -= 4;
    }
}

void St_box_mess_wait(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        pSt->Rtn1 = 0;
        pSt->Blink = 0;
        pSt->Blink_dir = 0;
        Snd_se_stad(5, 0, 0, 0);
        pSt->Flg |= 0x20000000;
    }
}

void St_box_prim_set(ST_WORK *pSt)
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
    i = pSt->Clut_frame;
    i <<= 8;
    St_sprt_init(pSt, (void *)0x8019C000, D_8009F328, i | 3);
    St_sprt_init(pSt, (void *)0x8019C1E0, D_8009F448, i | 7);
    St_tile_init(pSt, (TILE *)0x8019D478, D_8009F49C, 1);
    i = pSt->Clut_icon;
    i <<= 8;
    St_sprt_init(pSt, (void *)0x8019C140, D_8009F3F4, i | 3);
    St_sprt_init(pSt, (void *)0x8019C1B8, D_8009F418, i | 1);
    St_sprt_init(pSt, (void *)0x8019C2F8, D_8009F4A8, ((UCHAR)(pSt->Clut_parts + 3) << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x8019C320, D_8009F4B4, (pSt->Clut_box << 8) | 1);
    i = pSt->Clut_box;
    i <<= 8;
    St_sprt_init(pSt, (void *)0x8019C078, D_8009F34C, i | 5);
    St_sprt_init(pSt, (void *)0x8019D068, D_8009F4CC, i | 1);
    St_sprt_init(pSt, (void *)0x8019D090, D_8009F4D8, i | 1);
    i = pSt->Clut_frame;
    i <<= 8;
    St_ft4_init(pSt, (POLY_FT4 *)0x8019D0B8, D_8009F388, i | 0x07000008);
    St_ft4_init(pSt, (POLY_FT4 *)0x8019D338, D_8009F3E8, i | 0x07000001);
    St_ft4_init(pSt, (POLY_FT4 *)0x8019D388, D_8009F424, i | 0x04000003);
    St_tile_init(pSt, (TILE *)0x8019D748, D_8009F4C0, 1);
    St_box_prim_init(pSt);
}

INCLUDE_ASM("asm/nonmatchings/game/status", St_box_draw);

void St_icon_swap(UCHAR a, UCHAR b)
{
    RECT r;
    ST_WORK *pSt = &St;
    USHORT (*tbl)[2] = Item_pix_pos;
    USHORT *pa = tbl[a];
    USHORT *pb;
    USHORT (*tbl2)[2];

    r.x = pa[0];
    r.y = pa[1];
    r.w = 0x14;
    r.h = 0x1E;
    StoreImage(&r, pSt->pBuf);
    DrawSync(0);
    tbl2 = Box_pix_pos;
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

void St_icon_copy(UCHAR dst, UCHAR src)
{
    RECT r;

    r.x = Item_pix_pos[src][0];
    r.y = Item_pix_pos[src][1];
    r.w = 0x14;
    r.h = 0x1E;
    MoveImage(&r, Box_pix_pos[dst][0], Box_pix_pos[dst][1]);
    DrawSync(0);
}

void St_box_list_draw(ST_WORK *pSt, UCHAR no, SHORT y0)
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
    clut = pSt->Clut_icon;
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

void St_box_prim_init(ST_WORK *pSt)
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
            pS->clut = (pSt->Clut_box + 0x1E0) << 6;
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
            pS->clut = (pSt->Clut_box + 0x1E0) << 6;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/status", St_box_frame_draw);

void St_status_init(ST_WORK *pSt)
{
switch (pSt->St_type) { case 1: pSt->Rtn1 = 0xB; pSt->Rtn2 = 0; pSt->Rtn0++; break; case 0: pSt->Pos[3].X = 0x48; pSt->Pos[4].X = 0x48; pSt->Pos[5].X = 0x48; pSt->Pos[6].X = 0x48; pSt->Pos[7].X = 0x48; pSt->Pos[8].X = -0xD8; pSt->Pos[9].X = -0xD8; pSt->Pos[14].X = -0xD8; pSt->Pos[24].X = -0xD8; pSt->Pos[10].Y = 0x30; pSt->Rtn1 = 0; pSt->Blink = 0; pSt->Blink_dir = 0; pSt->Flg |= 0x20000000; pSt->Flg |= 0x40000000; pSt->Rtn0++; break; }
}

void St_status_main(ST_WORK *pSt)
{
    St_status_tbl[pSt->Rtn1](pSt);
}

void St_item_select(ST_WORK *pSt)
{
    UCHAR *p;
    LONG se;

    if ((G.Unk20FC & 0x20) | (G.Key_trg & 0x2000)) {
        pSt->Rtn0++;
        Snd_se_stad(5, 0, 0, 0);
    } else {
        if (G.Key_trg & 0x1000) {
            if ((signed char)pSt->Cursor >= 0) {
                p = (UCHAR *)&G.pItem_set->Item[(signed char)pSt->Cursor];
                if (p[0] != 0) {
                    pSt->Rtn1 = 1;
                    pSt->Cmd_cursor = 4;
                    pSt->Flg &= ~0x20000000;
                    Snd_se_stad(6, 0, 0, 0);
                    pSt->Item_flg = ((USHORT *)p)[1];
                } else {
                    Snd_se_stad(7, 0, 0, 0);
                }
            }
            if ((signed char)pSt->Cursor == -1) {
                pSt->Rtn1 = 4;
                pSt->Rtn2 = 0;
                pSt->Timer = 0;
                Snd_se_stad(9, 0, 0, 0);
            }
            if ((signed char)pSt->Cursor == -2) {
                pSt->Rtn1 = 5;
                pSt->Rtn2 = 0;
                pSt->Timer = 0;
                pSt->Pos[24].X = -0xD8;
                Snd_se_stad(9, 0, 0, 0);
            }
            if ((signed char)pSt->Cursor < -2) {
                pSt->Rtn0++;
                Snd_se_stad(5, 0, 0, 0);
            }
        } else {
            se = 0;
            if (G.Unk2100 & 0x1000) {
                if ((signed char)pSt->Cursor >= -2) {
                    pSt->Cursor -= 2;
                    se = 1;
                }
            }
            if (G.Unk2100 & 0x4000) {
                if ((signed char)pSt->Cursor < G.pItem_set->Num - 2) {
                    pSt->Cursor += 2;
                    se = 1;
                }
            }
            if (G.Unk2100 & 0x8000) {
                if (pSt->Cursor & 1) {
                    pSt->Cursor--;
                    if ((signed char)pSt->Cursor >= -2) {
                        se = 1;
                    }
                }
            }
            if (G.Unk2100 & 0x2000) {
                if (!(pSt->Cursor & 1)) {
                    pSt->Cursor++;
                    if ((signed char)pSt->Cursor >= -2) {
                        se = 1;
                    }
                }
            }
            if (se == 1) {
                Snd_se_stad(4, 0, 0, 0);
            }
            if ((signed char)pSt->Cursor >= 0) {
                pSt->Pos[12].X = (pSt->Cursor & 1) * 40;
                pSt->Pos[12].Y = ((signed char)pSt->Cursor >> 1) * 30;
            }
        }
    }
}

void St_cmd_open(ST_WORK *pSt)
{
    if (St_cmd_slide(pSt, 0)) {
        pSt->Rtn1 = 2;
        pSt->Cmd_cursor = 0;
    }
}

void St_cmd_select(ST_WORK *pSt)
{
    ITEM_WORK *e = &G.pItem_set->Item[pSt->Cursor];
    LONG n = 3;

    if (e->Flg & 0x10) {
        n = 4;
    }
    if (G.Key_trg & 0x2000) {
        pSt->Rtn1 = 3;
        Snd_se_stad(5, 0, 0, 0);
    } else if (G.Key_trg & 0x1000) {
        if ((signed char)pSt->Cmd_cursor == 0 && (UCHAR)(e->Id + 0x7D) < 2) {
            pSt->Rtn1 = 8;
        } else {
            pSt->Rtn1 = pSt->Cmd_cursor + 6;
        }
        pSt->Rtn2 = 0;
        pSt->Timer = 0;
        Snd_se_stad(6, 0, 0, 0);
    } else {
        if (G.Unk2100 & 0x1000) {
            pSt->Cmd_cursor--;
        }
        if (G.Unk2100 & 0x4000) {
            pSt->Cmd_cursor++;
        }
        if ((signed char)pSt->Cmd_cursor < 0) {
            pSt->Cmd_cursor = n - 1;
        }
        if ((signed char)pSt->Cmd_cursor >= n) {
            pSt->Cmd_cursor = 0;
        }
        if (*(ULONG *)&G.Unk2100 & 0x5000) {
            Snd_se_stad(4, 0, 0, 0);
        }
    }
}

void St_cmd_close(ST_WORK *pSt)
{
    if (St_cmd_slide(pSt, 1)) {
        pSt->Rtn1 = 0;
        pSt->Blink = 0;
        pSt->Blink_dir = 0;
        pSt->Flg |= 0x20000000;
    }
}

void St_map_open(ST_WORK *pSt)
{
    switch (pSt->Rtn2) {
    case 0:
        pSt->Flg &= ~0x20000000;
        if (pSt->Timer < 0xC) {
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
            pSt->Timer++;
        } else {
            pSt->Rtn0 = 4;
            pSt->Flg &= ~0x40000000;
            St_map_pos_init(pSt);
        }
        break;
    case 1:
        if (pSt->Timer < 0xC) {
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
            pSt->Timer++;
        } else {
            pSt->Rtn1 = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
            pSt->Flg |= 0x20000000;
        }
        break;
    }
}

extern UCHAR File_order_tbl_b[][2] __asm__("File_order_tbl"); /* File_order_tbl read with lbu here (low byte of each entry) */

static const char D_80010DFC[] = "FILEI.TIM";

void St_file_list(ST_WORK *pSt)
{
    UCHAR no;
    ULONG *p;
    UCHAR se;

    switch (pSt->Rtn2) {
    case 0:
        Cd_read(0x1D, pSt->pBuf, 0, D_80010DFC);
        p = pSt->pBuf;
        Mapping_tpage_clut = 0x917;
        Mapping_tim(p);
        pSt->File_page = 0;
        pSt->Blink_timer = 0;
        pSt->Timer = 0;
        pSt->Flg &= ~0x2000;
        pSt->Flg &= ~0x4000;
        pSt->Rtn2++;
        break;
    case 1:
        pSt->Flg &= ~0x20000000;
        if (pSt->Timer < 12) {
            pSt->Pos[11].Y += 6;
            pSt->Timer++;
        } else {
            pSt->Timer = 0;
            pSt->Rtn2++;
            pSt->Flg |= 0x1000;
        }
        break;
    case 2:
        if (pSt->Timer < 12) {
            pSt->Pos[9].X += 0x12;
            pSt->Pos[14].X += 0x12;
            pSt->Pos[24].X += 0x12;
            pSt->Pos[10].Y -= 4;
            pSt->Timer++;
        } else {
            pSt->Rtn2++;
            pSt->Timer = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
            pSt->Pos[14].X = 0;
            pSt->Pos[14].Y = 0;
            pSt->File_cursor = 0;
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
            no = File_order_tbl_b[pSt->File_cursor][0];
            if (G.Unk79EC[3] == 0x1E && pSt->File_cursor == 0) {
                no = 0x1E;
            }
            if (Flg_ck(&G.Unk79EC[2], no)) {
                pSt->Timer = 0;
                pSt->Rtn2++;
                pSt->Flg &= ~0x1000000;
                pSt->Flg &= ~0x2000;
                pSt->Flg &= ~0x4000;
                Snd_se_stad(6, 0, 0, 0);
                pSt->File_no = no;
                pSt->Page_no = 0;
            } else {
                Snd_se_stad(7, 0, 0, 0);
            }
            break;
        }
        se = 0;
        if (pSt->File_page == 0) {
            if (G.Unk2100 & 0x2000) {
                if ((signed char)(pSt->File_cursor % 5) == 4) {
                    se = 1;
                    pSt->File_cursor += 11;
                    pSt->File_page = 1;
                    pSt->Rtn2 = 8;
                }
            }
        } else if (G.Unk2100 & 0x8000) {
            if ((signed char)(pSt->File_cursor % 5) == 0) {
                se = 1;
                pSt->File_cursor -= 11;
                pSt->File_page = 0;
                pSt->Rtn2 = 9;
            }
        }
        if (se) {
            pSt->Timer = 0;
            pSt->Flg &= ~0x2000;
            pSt->Flg &= ~0x4000;
            pSt->Flg &= ~0x1000000;
            pSt->Pos[14].X = (signed char)(pSt->File_cursor % 5) * 32;
            pSt->Pos[14].Y = (signed char)((signed char)(pSt->File_cursor % 15) / 5) * 32;
            if (pSt->File_page) {
                pSt->Pos[14].X += 0x20;
            }
            Snd_se_stad(4, 0, 0, 0);
            break;
        }
        if (G.Unk2100 & 0x1000) {
            if ((signed char)(pSt->File_cursor % 15) >= 5) {
                pSt->File_cursor -= 5;
                se = 1;
            }
        }
        if (G.Unk2100 & 0x4000) {
            if ((signed char)(pSt->File_cursor % 15) < 10) {
                pSt->File_cursor += 5;
                se = 1;
            }
        }
        if (G.Unk2100 & 0x8000) {
            if ((signed char)(pSt->File_cursor % 5) != 0) {
                pSt->File_cursor--;
                se = 1;
            }
        }
        if (G.Unk2100 & 0x2000) {
            if ((signed char)(pSt->File_cursor % 5) != 4) {
                pSt->File_cursor++;
                se = 1;
            }
        }
        if (se == 1) {
            Snd_se_stad(4, 0, 0, 0);
        }
        pSt->Pos[14].X = (signed char)(pSt->File_cursor % 5) * 32;
        pSt->Pos[14].Y = (signed char)((signed char)(pSt->File_cursor % 15) / 5) * 32;
        if (pSt->File_page) {
            pSt->Pos[14].X += 0x20;
        }
        if (++pSt->Blink_timer > 20) {
            pSt->Blink_timer = -20;
        }
        if (pSt->Blink_timer >= 0) {
            pSt->Pos[23].X = -1;
            pSt->Pos[22].X = 1;
        } else {
            pSt->Pos[23].X = 0;
            pSt->Pos[22].X = 0;
        }
        pSt->Flg &= ~0x2000;
        pSt->Flg &= ~0x4000;
        if (pSt->File_page) {
            pSt->Flg |= 0x2000;
        } else {
            pSt->Flg |= 0x4000;
        }
        break;
    case 4:
        if (pSt->Timer < 12) {
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
            pSt->Timer++;
        } else {
            pSt->Rtn0 = 7;
        }
        break;
    case 5:
        if (pSt->Timer < 12) {
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
            pSt->Timer++;
        } else {
            pSt->Rtn2 = 3;
            pSt->Timer = 0;
            pSt->Flg |= 0x1000000;
        }
        break;
    case 6:
        if (pSt->Timer < 12) {
            pSt->Pos[9].X -= 0x12;
            pSt->Pos[14].X -= 0x12;
            pSt->Pos[24].X -= 0x12;
            pSt->Pos[10].Y += 4;
            pSt->Timer++;
        } else {
            pSt->Rtn2++;
            pSt->Timer = 0;
            pSt->Flg &= ~0x1000;
        }
        break;
    case 7:
        if (pSt->Timer < 12) {
            pSt->Pos[11].Y -= 6;
            pSt->Timer++;
        } else {
            pSt->Rtn1 = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
            pSt->Flg |= 0x20000000;
        }
        break;
    case 8:
        if (pSt->Timer < 16) {
            pSt->Pos[24].X -= 0xB;
            pSt->Timer++;
        } else {
            pSt->Rtn2 = 3;
            pSt->Timer = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
            pSt->Flg |= 0x1000000;
        }
        break;
    case 9:
        if (pSt->Timer < 16) {
            pSt->Pos[24].X += 0xB;
            pSt->Timer++;
        } else {
            pSt->Rtn2 = 3;
            pSt->Timer = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
            pSt->Flg |= 0x1000000;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/status", St_item_use);

void St_mix_main(ST_WORK *pSt)
{
    St_mix_tbl[pSt->Rtn2](pSt);
}

void St_mix_init(ST_WORK *pSt)
{
    UCHAR v = pSt->Cursor;

    pSt->Blink = 0;
    pSt->Blink_dir = 0;
    pSt->Cursor2 = v;
    pSt->Pos[13].X = (v & 1) * 40;
    pSt->Flg |= 0x2000000;
    pSt->Flg |= 0x20000000;
    pSt->Rtn2++;
    pSt->Pos[13].Y = ((signed char)pSt->Cursor2 >> 1) * 30;
}

LONG Pl_wep_eff_parts_cnt_l(void) __asm__("Pl_wep_eff_parts_cnt"); /* Capcom called it undeclared here: the UCHAR result is used without a mask */

void St_mix_select(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    ITEM_WORK *p;
    ITEM_WORK *q;
    ITEM_MIX *pMix;
    SHORT n;
    LONG i;
    UCHAR c;
    LONG se;

    p = &pG->pItem_set->Item[pSt->Cursor];
    n = 0;
    if (pG->Key_trg & 0x2000) {
        pSt->Rtn1 = 3;
        pSt->Flg &= ~0x2000000;
        pSt->Flg &= ~0x20000000;
        Snd_se_stad(5, 0, 0, 0);
        return;
    }
    if (pG->Key_trg & 0x1000) {
        q = &pG->pItem_set->Item[(signed char)pSt->Cursor2];
        pMix = Search_item_mix(pSt, p->Id, q->Id);
        pSt->pMix_a = p;
        pSt->pMix_b = q;
        pSt->pMix = pMix;
        if (pMix != (ITEM_MIX *)-1 && pSt->Cursor != (signed char)pSt->Cursor2) {
            switch (pMix->Type) {
            case 2:
                for (i = 0; Mix_tama_tbl[i][0] != -1; i++) {
                    if (Mix_tama_tbl[i][0] == pMix->New_id) {
                        break;
                    }
                }
                c = pSt->Mix_tama_type = Mix_tama_tbl[i][1];
                switch (c) {
                case 0:
                case 1:
                    n = 6;
                    break;
                }
                i = n;
                if (i != 0 && i < pG->pItem_set->Tama_mix_cnt[c]) {
                    Mess_set(0, 0x1400, 13, 0);
                    pSt->Rtn2 = 5;
                    pSt->Flg &= ~0x20000000;
                    return;
                }
                break;
            case 0:
                if (pMix->Id_a == 12) {
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
                        func_80046C90();
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
            St_mix_exec(pSt);
        } else {
            Snd_se_stad(7, 0, 0, 0);
        }
        return;
    }
    se = 0;
    if (pG->Unk2100 & 0x1000) {
        if ((signed char)pSt->Cursor2 >= 2) {
            pSt->Cursor2 -= 2;
            se = 1;
        }
    }
    if (G.Unk2100 & 0x4000) {
        if ((signed char)pSt->Cursor2 < G.pItem_set->Num - 2) {
            pSt->Cursor2 += 2;
            se = 1;
        }
    }
    if (G.Unk2100 & 0x8000) {
        if (pSt->Cursor2 & 1) {
            pSt->Cursor2--;
            se = 1;
        }
    }
    if (G.Unk2100 & 0x2000) {
        if (!(pSt->Cursor2 & 1)) {
            pSt->Cursor2++;
            se = 1;
        }
    }
    if (se == 1) {
        Snd_se_stad(4, 0, 0, 0);
    }
    pSt->Pos[13].X = (pSt->Cursor2 & 1) * 40;
    pSt->Pos[13].Y = ((signed char)pSt->Cursor2 >> 1) * 30;
}

INCLUDE_ASM("asm/nonmatchings/game/status", St_mix_exec);

void St_mix_mess_wait(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        pSt->Rtn1 = 3;
        Snd_se_stad(5, 0, 0, 0);
        pSt->Flg &= ~0x2000000;
    }
}

void St_mix_yesno(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        switch (G.Unk7848) {
        case 0:
            Pl_wep_eff_parts_init(G_PLAYER);
            func_80046C90();
            St_mix_exec(pSt);
            break;
        case 1:
            pSt->Rtn1 = 3;
            pSt->Flg &= ~0x2000000;
            Snd_se_stad(5, 0, 0, 0);
            break;
        }
    }
}

void St_mix_next(ST_WORK *pSt)
{
    if (!(D_800DBB58 & 0x80)) {
        switch (G.Unk7848) {
        case 0:
            pSt->pMix++;
        case 1:
            St_mix_exec(pSt);
            break;
        }
    }
}

void St_mix_result(ST_WORK *pSt)
{
    USHORT f = 0;
    ITEM_WORK *pA = pSt->pMix_a;
    ITEM_WORK *pB = pSt->pMix_b;
    ITEM_MIX *pMix = pSt->pMix;
    UCHAR *pN = pSt->pMix_cur_a;
    UCHAR *pM = pSt->pMix_cur_b;
    tagGLOBAL *pG = &G;
    ITEM_DATA *pData;

    if (pSt->Timer < 5) {
        if (pA->Id == 0xFF) {
            pSt->Pos[13].X += pSt->Mix_dx;
            pSt->Pos[13].Y += pSt->Mix_dy;
            pSt->Pos[12].X += pSt->Mix_dx;
            pSt->Pos[12].Y += pSt->Mix_dy;
        } else if (&pSt->Cursor2 == pM) {
            pSt->Pos[13].X += pSt->Mix_dx;
            pSt->Pos[13].Y += pSt->Mix_dy;
        } else {
            pSt->Pos[12].X += pSt->Mix_dx;
            pSt->Pos[12].Y += pSt->Mix_dy;
        }
        pSt->Timer++;
        return;
    }
    pSt->Flg &= ~0x2000000;
    if ((pA->Flg & 3) == 3) {
        f = 3;
    }
    switch (pSt->Mix_type) {
    case 0:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Timer = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Id = pMix->New_id;
        pData = &Item_data_tbl[pA->Id];
        pA->Num = pMix->New_num;
        pA->Flg = pData->Attr;
        if (Flg_ck(G.Unk77F8, 9) && (pA->Id == 0xD || pA->Id == 0x10)) {
            pA->Flg |= 3;
        }
        Pix_trans(*pN);
        break;
    case 1:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Timer = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        if (pMix->Type == 3 || pMix->Type == 4) {
            pA->Id = pMix->New_id;
            pA->Flg = Item_data_tbl[pMix->New_id].Attr | (pA->Flg & 0xFF00);
            if (pG->pItem_set->Equip_no == (signed char)*pN) {
                pG->pItem_set->Equip_id = pMix->New_id;
            }
        }
        pA->Num += pSt->Mix_num;
        break;
    case 2:
        pSt->Rtn1 = 3;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        if (pMix->Type == 3) {
            pA->Id = pMix->New_id;
            pA->Flg = Item_data_tbl[pMix->New_id].Attr | (pA->Flg & 0xFF00);
            if (pG->pItem_set->Equip_no == (signed char)*pN) {
                pG->pItem_set->Equip_id = pMix->New_id;
            }
        }
        pA->Num += pSt->Mix_num;
        break;
    case 3:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Timer = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Id = pMix->New_id;
        pData = &Item_data_tbl[pA->Id];
        pA->Num = pSt->Mix_num;
        pA->Flg = pData->Attr;
        Pix_trans(*pN);
        break;
    case 4:
        pSt->Rtn1 = 3;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Id = pMix->New_id;
        pData = &Item_data_tbl[pA->Id];
        pA->Num = pSt->Mix_num;
        pA->Flg = pData->Attr;
        Pix_trans(*pN);
        break;
    case 5:
        pSt->Rtn1 = 3;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pB->Id = pMix->New_num;
        pData = &Item_data_tbl[pMix->New_num];
        pB->Flg = pData->Attr | (pB->Flg & 0xFF00);
        pB->Num = pA->Num;
        Pix_trans(*pM);
        pA->Id = pMix->New_id;
        pData = &Item_data_tbl[pMix->New_id];
        pA->Flg = pData->Attr | (pA->Flg & 0xFF00);
        if (pG->pItem_set->Equip_no == (signed char)*pN) {
            pG->pItem_set->Equip_id = pMix->New_id;
        }
        pA->Num = pSt->Mix_num;
        *pM = *pN;
        break;
    case 6:
        pSt->Rtn1 = 10;
        pSt->Rtn2 = *pM;
        pSt->Timer = 2;
        *pM = *pN;
        pSt->Pos[12].X = (*pN & 1) * 40;
        pSt->Pos[12].Y = ((signed char)*pN >> 1) * 30;
        pA->Flg |= 3;
        if (pA->Id == 0xC) {
            pA->Id = 0x14;
            pA->Flg = 7;
            pG->pItem_set->Equip_id = pA->Id;
            Pl_wep_eff_parts_init(&Pl_work);
            func_80046C90();
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
        pData = &Item_data_tbl[pB->Id];
        pB->Flg = pData->Attr | (pB->Flg & 0xFF00);
        pB->Num = pA->Num;
        Pix_trans(*pM);
        pA->Id = pMix->New_id;
        pData = &Item_data_tbl[pMix->New_id];
        pA->Flg = pData->Attr | (pA->Flg & 0xFF00);
        if (pG->pItem_set->Equip_no == (signed char)*pN) {
            pG->pItem_set->Equip_id = pMix->New_id;
        }
        pA->Num = pSt->Mix_num;
        *pM = *pN;
        break;
    }
    pA->Flg |= f;
}

void St_item_check(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    ITEM_WORK *p;
    ITEM_CHECK *pChk;
    ITEM_DATA *pData;

    switch (pSt->Rtn2) {
    case 0:
        p = &pG->pItem_set->Item[pSt->Cursor];
        Itemg_load(p->Id);
        pSt->Cmd_closed = 0;
        pSt->Timer = 0;
        pSt->Rtn2++;
        break;
    case 1:
        if (pSt->Timer < 12) {
            pSt->Pos[8].X += 0x12;
        } else {
            p = &pG->pItem_set->Item[pSt->Cursor];
            pData = &Item_data_tbl[p->Id];
            Flg_on(pG->Unk7910, pData->Chk_flg);
            Mess_set(0, 0x1100, p->Id + 0x10, 0);
            pSt->Rtn2++;
        }
        pSt->Timer++;
        if (pSt->Cmd_closed == 0 && St_cmd_slide(pSt, 1)) {
            pSt->Cmd_closed = 1;
        }
        break;
    case 2:
        if (!(D_800DBB58 & 0x80)) {
            pSt->Rtn2 = 3;
            pSt->Timer = 0;
            p = &pG->pItem_set->Item[pSt->Cursor];
            pSt->pChk = pChk = Search_item_check(pSt, p->Id);
            if (pChk != (ITEM_CHECK *)-1 && pChk->Type == 1) {
                pSt->Chg_id = pChk->Arg;
                pSt->Chg_slot = pSt->Cursor;
                pSt->Rtn1 = 0xD;
                pSt->Rtn2 = 0;
                pSt->Timer = 0;
                Snd_se_stad(6, 0, 0, 0);
            } else {
                Snd_se_stad(5, 0, 0, 0);
            }
        }
        break;
    case 3:
        if (pSt->Timer < 12) {
            pSt->Pos[8].X -= 0x12;
        } else {
            p = &pG->pItem_set->Item[pSt->Cursor];
            pSt->pChk = pChk = Search_item_check(pSt, p->Id);
            if (pChk != (ITEM_CHECK *)-1) {
                if (pChk->Type == 0) {
                    pSt->Chg_id = pChk->Arg;
                    pSt->Chg_slot = pSt->Cursor;
                    pSt->Rtn1 = 0xC;
                    pSt->Rtn2 = 0;
                }
            } else {
                pSt->Rtn1 = 0;
                pSt->Rtn2 = 0;
                pSt->Blink = 0;
                pSt->Blink_dir = 0;
                pSt->Flg |= 0x20000000;
            }
        }
        pSt->Timer++;
        break;
    }
}

void St_item_switch(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    ITEM_WORK *p = &pG->pItem_set->Item[pSt->Cursor];
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
    n = Item_data_tbl[p->Id].Attr | (p->Flg & 0xFF00);
    p->Flg = n;
    if ((w & 3) == 3) {
        p->Flg = n | 3;
    }
    if (id == pG->pItem_set->Equip_id && pSt->Cursor == pG->pItem_set->Equip_no) {
        pG->pItem_set->Equip_id = p->Id;
    }
    pSt->Rtn1 = 2;
}

UCHAR St_cmd_slide(ST_WORK *pSt, LONG a1)
{
    POLY_FT4 *p;
    ULONG i;
    USHORT f;
    ITEM_WORK *pI;

    if (a1 == 0) {
        pI = &G.pItem_set->Item[pSt->Cursor];
        pSt->Sel_item = *pI;
        f = pSt->Sel_item.Flg;
        if (pSt->Slide_step == 0) {
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
            if ((signed char)++pSt->Slide_cnt >= 4) {
                pSt->Slide_cnt = 0;
                pSt->Slide_step++;
            }
        }
        if (pSt->Slide_done == 0) {
            if (pSt->Slide_no < 3) {
                pSt->Pos[3].X -= 24;
            }
            if (pSt->Slide_no >= 1 && pSt->Slide_no < 4) {
                pSt->Pos[4].X -= 24;
            }
            if (pSt->Slide_no >= 2 && pSt->Slide_no < 5) {
                pSt->Pos[5].X -= 24;
            }
            if (pSt->Slide_no >= 3 && pSt->Slide_no < 6) {
                pSt->Pos[6].X -= 24;
            }
            if (f & 0x10) {
                if (pSt->Slide_no >= 4 && pSt->Slide_no < 7) {
                    pSt->Pos[7].X -= 24;
                }
                    if ((signed char)++pSt->Slide_no >= 8) {
                pSt->Slide_step = 0;
                pSt->Slide_done = 0;
                pSt->Slide_cnt = 0;
                pSt->Slide_no = 0;
                return 1;
            }
            } else if ((signed char)++pSt->Slide_no >= 7) {
                pSt->Slide_step = 0;
                pSt->Slide_done = 0;
                pSt->Slide_cnt = 0;
                pSt->Slide_no = 0;
                return 1;
            }
        }
        return 0;
    }
    f = pSt->Sel_item.Flg;
    switch (pSt->Slide_step) {
    case 0:
        if ((signed char)++pSt->Slide_cnt >= 4) {
            pSt->Slide_cnt = 0;
            pSt->Slide_step++;
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
        if ((signed char)++pSt->Slide_cnt >= 4) {
            pSt->Slide_step = 0;
            pSt->Slide_done = 0;
            pSt->Slide_cnt = 0;
            pSt->Slide_no = 0;
            return 1;
        }
        break;
    }
    if (pSt->Slide_done == 0) {
        if (pSt->Slide_no < 3) {
            pSt->Pos[4].X += 24;
        }
        if (pSt->Slide_no >= 1 && pSt->Slide_no < 4) {
            pSt->Pos[5].X += 24;
        }
        if (pSt->Slide_no >= 2 && pSt->Slide_no < 5) {
            pSt->Pos[6].X += 24;
        }
        if (f & 0x10) {
            if (pSt->Slide_no >= 3 && pSt->Slide_no < 6) {
                pSt->Pos[7].X += 24;
            }
            if (pSt->Slide_no >= 4 && pSt->Slide_no < 7) {
                pSt->Pos[3].X += 24;
            }
            if ((signed char)++pSt->Slide_no >= 8) {
                pSt->Slide_done++;
            }
        } else {
            if (pSt->Slide_no >= 3 && pSt->Slide_no < 6) {
                pSt->Pos[3].X += 24;
            }
            if ((signed char)++pSt->Slide_no >= 6) {
                pSt->Slide_done++;
            }
        }
    }
    return 0;
}

void St_item_shift(ST_WORK *pSt)
{
    tagGLOBAL *pG;
    UCHAR i;
    LONG k;
    LONG j;
    UCHAR c;

    if (pSt->Timer == 0) {
        pG = &G;
        i = pSt->Rtn2;
        if (pSt->Rtn2 >= pG->pItem_set->Num - 1) {
            pSt->Rtn1 = 3;
            return;
        }
        k = i;
        pG->pItem_set->Item[k].Id = pG->pItem_set->Item[j = k + 1].Id;
        pG->pItem_set->Item[k].Num = pG->pItem_set->Item[j].Num;
        pG->pItem_set->Item[k].Flg = pG->pItem_set->Item[j].Flg;
        pG->pItem_set->Item[j].Id = 0;
        pG->pItem_set->Item[j].Num = 0;
        pG->pItem_set->Item[j].Flg = 0;
        Pix_swap(k, i + 1);
        if (pG->pItem_set->Equip_no == j) {
            pG->pItem_set->Equip_no = k;
            pG->pItem_set->Equip_id = pG->pItem_set->Item[pG->pItem_set->Equip_no].Id;
        }
        if (pSt->Cursor == j) {
            pSt->Cursor = c = pSt->Cursor - 1;
            pSt->Pos[12].X = (c & 1) * 40;
            pSt->Pos[12].Y = (pSt->Cursor >> 1) * 30;
        }
        pSt->Rtn2++;
        if (pG->pItem_set->Item[k + 2].Id == 0) {
            pSt->Rtn1 = 3;
        }
        pSt->Timer = 2;
    }
    pSt->Timer--;
}
#undef IT

signed char Search_item_id_u(UCHAR id, UCHAR i) __asm__("Search_item_id"); /* this file passes the start slot unsigned (andi, not sll/sra) */

void St_item_get(ST_WORK *pSt)
{
    UCHAR *pA = G.pAot_data;
    ST_AOT_ITEM *pD = G.pUnk21DC;
    ITEM_WORK *p;
    UCHAR id;
    signed char n;
    UCHAR m;
    LONG dx, dy;
    ITEM_DATA *pData;
    USHORT f;

    switch (pSt->Rtn2) {
    case 0:
        pSt->Get_stack = 0;
        pData = &Item_data_tbl[pD->Id];
        id = pD->Id;
        n = 0;
        switch (pData->Type) {
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
                p = &G.pItem_set->Item[n];
                if (Item_data_tbl[p->Id].Max >= p->Num + pD->Num) {
                    pSt->Get_slot = n;
                    pSt->Get_stack = 1;
                    D_800DBB5B = pD->Id;
                    Mess_set(0, 0x1400, 0, 0);
                    pSt->Rtn2 = 2;
                    break;
                }
                n++;
            } while (1);
            break;
        }
        if (pSt->Get_stack == 0) {
            n = Search_item_id_u(0, 0);
            if (n == -1) {
                Mess_set(0, 0x1000, 1, 0);
                pSt->Rtn2 = 1;
                break;
            }
            pSt->Get_slot = n;
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
                k = pSt->Get_slot;
                pSt->Pos[15].Y = 0x7C;
                pSt->Timer = 0;
                pSt->Zoom_w = 0x38;
                pSt->Zoom_h = 0x24;
                dx = (k & 1) * 20 + 0xF4;
                dy = ((signed char)k / 2) * 30 + 0x51;
                pSt->Zoom_dx = (dx - pSt->Pos[15].X) / 10;
                pSt->Flg |= 0x400000;
                pSt->Zoom_dy = (dy - pSt->Pos[15].Y) / 10;
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
        if (pSt->Timer < 10) {
            pSt->Pos[15].X += pSt->Zoom_dx;
            pSt->Pos[15].Y += pSt->Zoom_dy;
            pSt->Zoom_w -= 3;
            pSt->Zoom_h -= 2;
        } else {
            p = &G.pItem_set->Item[(signed char)pSt->Get_slot];
            switch (pSt->Get_stack) {
            case 0:
                Pix_move(pSt->Get_slot, 12);
                pData = &Item_data_tbl[pD->Id];
                p->Id = pD->Id;
                p->Num = pD->Num;
                p->Flg = f = pData->Attr;
                if (p->Id == 0xE) {
                    p->Flg = f | 0x100;
                }
                if (Flg_ck(G.Unk77F8, 9) && (ULONG)(p->Id - 2) < 0x13) {
                    p->Flg |= 3;
                    if (p->Id == 0xC) {
                        p->Id = 0x14;
                        p->Flg = 7;
                    }
                }
                break;
            case 1:
                p->Num += pD->Num;
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
            func_8001B894(0x705, &G.Ob_model[m].M);
            func_8001B894(0x905, 0);
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
        pSt->Timer++;
        break;
    }
}

void St_item_change(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    ITEM_WORK *p;
    ITEM_DATA *pData;
    UCHAR n;
    LONG dx, dy;
    switch (pSt->Rtn2) {
    case 0:
        Itemg_load(pSt->Chg_id);
        Pix_load(12, pSt->Chg_id);
        pSt->Timer = 0;
        pSt->Rtn2++;
        Snd_se_stad(6, NULL, 0, 0);
        break;
    case 1:
        if (pSt->Timer < 12) {
            pSt->Pos[8].X += 0x12;
        } else {
            pSt->Rtn2++;
            pSt->Flg |= 0x20000000;
            p = &pG->pItem_set->Item[(signed char)pSt->Chg_slot];
            p->Id = pSt->Chg_id;
        }
        pSt->Timer++;
        break;
    case 2:
        if (D_800CC840 & 0x3000) {
            Snd_se_stad(6, NULL, 0, 0);
            n = pSt->Chg_slot;
            pSt->Zoom_w = 0x38;
            pSt->Zoom_h = 0x24;
            pSt->Pos[15].X = 0x70;
            pSt->Timer = 0;
            pSt->Pos[15].Y = 0x7C;
            dx = (n & 1) * 20 + 0xF4;
            dy = ((signed char)n / 2) * 30 + 0x51;
            pSt->Zoom_dx = (dx - pSt->Pos[15].X) / 10;
            pSt->Rtn2++;
            pSt->Flg = (pSt->Flg | 0x400000) & ~0x20000000;
            pSt->Zoom_dy = (dy - pSt->Pos[15].Y) / 10;
        }
        break;
    case 3:
        if (pSt->Timer < 10) {
            pSt->Pos[15].X += pSt->Zoom_dx;
            pSt->Pos[15].Y += pSt->Zoom_dy;
            pSt->Zoom_w -= 3;
            pSt->Zoom_h -= 2;
        pSt->Timer++;
            return;
        }
        p = &pG->pItem_set->Item[(signed char)pSt->Chg_slot];
        pData = &Item_data_tbl[pSt->Chg_id];
        Pix_move(pSt->Chg_slot, 12);
        p->Id = pSt->Chg_id;
        p->Num = pData->Max;
        p->Flg = pData->Attr;
        pSt->Rtn2++;
        pSt->Flg &= ~0x400000;
        break;
    case 4:
        if (D_800CC840 & 0x3000) {
            Snd_se_stad(5, NULL, 0, 0);
            pSt->Timer = 0;
            pSt->Rtn2++;
        }
        break;
    case 5:
        if (pSt->Timer < 12) {
            pSt->Pos[8].X -= 0x12;
        } else {
            pSt->Rtn1 = 0;
            pSt->Rtn2 = 0;
            pSt->Blink = 0;
            pSt->Blink_dir = 0;
            pSt->Flg |= 0x20000000;
        }
        pSt->Timer++;
    }
}

void St_item_to_file(ST_WORK *pSt)
{
    ITEM_CHECK *pChk;
    ULONG i;
    signed char c, n;

    if (pSt->Timer < 12) {
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
        pSt->Timer++;
        return;
    }
    pChk = pSt->pChk;
    pSt->File_cursor = pChk->File_no;
    c = File_order_tbl[pSt->File_cursor];
    pSt->File_no = c;
    pSt->Flg &= ~0x1000000;
    pSt->Flg &= ~0x2000;
    pSt->Flg &= ~0x4000;
    pSt->Flg |= 0x1000;
    pSt->Page_no = 0;
    pSt->File_page = pChk->File_no / 15;
    pSt->Rtn0 = 7;
    pSt->Pos[14].X = pSt->Pos[24].X = -0xD8;
    pSt->Rtn1 = 0;
    pSt->Rtn2 = 0;
    pSt->Timer = 0;
    pSt->Pos[14].Y = 0;
    pSt->Pos[14].X += (signed char)(pSt->File_cursor % 5) * 32;
    pSt->Pos[14].Y += (signed char)((signed char)(pSt->File_cursor % 15) / 5) * 32;
    if (pSt->File_page) {
        pSt->Pos[14].X += 0x20;
    }
    Item_delete(pSt->Cursor);
    Flg_on(&G.Unk79EC[2], pChk->Arg - 0x85);
    for (i = 0; i < 30; i++) {
        if (File_order_tbl[i] == pSt->File_no) {
            break;
        }
    }
    if (G.Unk79EC[3] == i) {
        G.Unk79EC[3]++;
    }
    Pix_move(pSt->Cursor, 11);
    for (c = pSt->Cursor; c < 9; c = n) {
        n = c + 1;
        Pix_swap(c, n);
    }
    pSt->Cursor = -2;
}

void St_exit_set(ST_WORK *pSt)
{
    pSt->Rtn0 = 0xD;
    pSt->Rtn1 = 0;
}

ITEM_MIX *Search_item_mix(ST_WORK *pSt, UCHAR a, UCHAR b)
{
    ITEM_MIX *p = Item_mix_tbl;
    UCHAR c = p->Id_a;

    if (c != 0xFF) {
        do {
            if (c == a && p->Id_b == b) {
                return p;
            }
            if (p->Id_a == b && p->Id_b == a) {
                return p;
            }
            p++;
            c = p->Id_a;
        } while (c != 0xFF);
    }
    return (ITEM_MIX *)-1;
}

ITEM_CHECK *Search_item_check(ST_WORK *pSt, UCHAR id)
{
    ITEM_CHECK *p = Item_check_tbl;

    while (p->Id != 0xFF) {
        if (p->Id == id) {
            return p;
        }
        p++;
    }
    return (ITEM_CHECK *)-1;
}

ITEM_TAMA *Search_item_tama(ST_WORK *pSt, UCHAR id)
{
    ITEM_TAMA *p = Item_tama_tbl;

    while (p->Wep_id != 0xFF) {
        if (p->Wep_id == id) {
            return p;
        }
        p++;
    }
    return (ITEM_TAMA *)-1;
}

void Pix_load(UCHAR no, UCHAR idx)
{
    RECT r;
    ST_WORK *pSt = &St;

    CD.Fno = 0x34;
    CD.Fsize = 0x800;
    CD.Fsector = (floc[0x34].sec_high << 16) + floc[0x34].sector + idx;
    CD.Fsum = Itemi_sum[idx];
    Cd_read(0x34, pSt->pBuf, 2, D_80010F84);
    r.x = Item_pix_pos[no][0];
    r.y = Item_pix_pos[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pSt->pBuf);
}

void Pix_read(UCHAR no)
{
    LONG pad[2];

    CD.Fno = 0x34;
    CD.Fsize = 0x800;
    CD.Fsector = (floc[0x34].sec_high << 16) + floc[0x34].sector + no;
    CD.Fsum = Itemi_sum[no];
    Cd_read(0x34, St.pBuf, 2, D_80010F84);
}

void Pix_trans(UCHAR no)
{
    RECT r;

    r.x = Item_pix_pos[no][0];
    r.y = Item_pix_pos[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, St.pBuf);
}

void Pix_unpack(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf)
{
    RECT r;

    func_80010000(ofs + Item_pix_ofs[tbl], pBuf);
    r.x = Item_pix_pos[no][0];
    r.y = Item_pix_pos[no][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pBuf);
}

void Pix_unpack_box(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf)
{
    RECT r;

    func_80010000(ofs + Item_pix_ofs[tbl], pBuf);
    r.x = Box_pix_pos[no % 8][0];
    r.y = Box_pix_pos[no % 8][1];
    r.w = 0x14;
    r.h = 0x1E;
    LoadImage(&r, pBuf);
}

INCLUDE_RODATA("asm/nonmatchings/game/status", D_80010F84);

void Itemg_load(UCHAR no)
{
    ST_WORK *pSt = &St;
    LONG pad[2];

    CD.Fno = 0x33;
    CD.Fsize = 0x2800;
    CD.Fsector = (floc[0x33].sec_high << 16) + floc[0x33].sector + no * 5;
    CD.Fsum = Itemg_sum[no];
    Cd_read(0x33, pSt->pBuf, 2, "ITEMG.PIX");
    Mapping_tpage_clut = 0x817;
    Mapping_tim(pSt->pBuf);
    pSt->Clut_zoom = 8;
}

void Pix_swap(UCHAR a, UCHAR b)
{
    RECT r;
    ST_WORK *pSt = &St;
    USHORT (*tbl)[2] = Item_pix_pos;
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

void Pix_move(UCHAR dst, UCHAR src)
{
    RECT r;

    r.x = Item_pix_pos[src][0];
    r.y = Item_pix_pos[src][1];
    r.w = 0x14;
    r.h = 0x1E;
    MoveImage(&r, Item_pix_pos[dst][0], Item_pix_pos[dst][1]);
    DrawSync(0);
}

void St_prim_init(ST_WORK *pSt)
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
    c = pSt->Clut_frame;
    St_sprt_init(pSt, (void *)0x801AC000, D_8009F890, (c << 8) | 2);
    St_sprt_init(pSt, (void *)0x801AC1B8, D_8009F8CC, (c << 8) | 5);
    switch (G.Player.Id) {
    case 8:
    case 9:
    case 10:
        St_sprt_init(pSt, (void *)0x801AC280, D_8009F95C, (c << 8) | 7);
        break;
    default:
        St_sprt_init(pSt, (void *)0x801AC280, D_8009F908, (c << 8) | 7);
        break;
    }
    St_sprt_init(pSt, (void *)0x801ACA78, D_8009FBCC, (c << 8) | 4);
    St_sprt_init(pSt, (void *)0x801ACB18, D_8009FBFC, (c << 8) | 4);
    St_sprt_init(pSt, (void *)0x801ACBB8, D_8009FC2C, (c << 8) | 1);
    St_sprt_init(pSt, (void *)0x801ACBE0, D_8009FC38, (c << 8) | 1);
    St_sprt_init(pSt, (void *)0x801ACC08, D_8009FC44, (c << 8) | 1);
    St_sprt_init(pSt, (void *)0x801ACC30, D_8009FC50, (c << 8) | 1);
    St_ft4_init(pSt, (POLY_FT4 *)0x801ADBA8, D_8009FC5C, (c << 8) | 0x7000008);
    St_ft4_init(pSt, (POLY_FT4 *)0x801ADE28, D_8009FCBC, (c << 8) | 0x7000008);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AE0A8, D_8009FD1C, (c << 8) | 0x7000008);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AE328, D_8009FD7C, (c << 8) | 0x7000008);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AE5A8, D_8009F9B0, (c << 8) | 0x4000003);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AE698, D_8009FDDC, (c << 8) | 0x7000004);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AE7D8, D_8009FE0C, (c << 8) | 0x7000004);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AE918, D_8009FE3C, (c << 8) | 0x7000004);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AEAA8, D_8009FE6C, (c << 8) | 0x7000004);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AEBE8, D_8009FE9C, (c << 8) | 0x7000004);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AED28, D_8009FECC, (c << 8) | 0x7000004);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AEE68, D_8009FEFC, (c << 8) | 0x7000004);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AEFA8, D_8009FF2C, (c << 8) | 0x7000001);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AEFF8, D_8009FF38, (c << 8) | 0x7000001);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AF048, D_8009FF44, (c << 8) | 0x7000001);
    St_ft4_init(pSt, (POLY_FT4 *)0x801AF098, D_8009FF50, (c << 8) | 0x7000001);
    St_tile_init(pSt, (TILE *)0x801AF4E8, D_8009FF5C, 1);
    St_tile_init(pSt, (TILE *)0x801AF508, D_8009FB90, 2);
    c = pSt->Clut_icon;
    St_sprt_init(pSt, (void *)0x801AC398, D_8009F9D4, (c << 8) | 3);
    St_sprt_init(pSt, (void *)0x801AC460, D_8009FBC0, (c << 8) | 1);
    c = pSt->Clut_face;
    St_sprt_init(pSt, (void *)0x801AC050, D_8009F8A8, (c << 8) | 1);
    St_sprt_init(pSt, (void *)0x801AC078, D_8009F8B4, ((UCHAR)(c + 1) << 8) | 1);
    St_sprt_init(pSt, (void *)0x801AC0A0, D_8009F8C0, ((UCHAR)(c + 2) << 8) | 1);
    c = pSt->Clut_parts + 2;
    St_sprt_init(pSt, (void *)0x801ACA28, D_8009FF74, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ADB58, D_8009FB78, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ADB30, D_8009FB84, (c << 8) | 0x10001);
    c = pSt->Clut_parts + 3;
    St_sprt_init(pSt, (void *)0x801ACA00, D_8009FF68, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ACA50, D_8009FF80, (c << 8) | 0x10001);
    c = pSt->Clut_parts;
    St_sprt_init(pSt, (void *)0x801AC488, D_8009FF8C, (c << 8) | 0x10003);
    St_sprt_init(pSt, (void *)0x801ACC58, D_8009FFB0, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ACC80, D_8009FFBC, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ACCA8, D_8009FFC8, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ACCD0, D_8009FFD4, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ACCF8, D_8009FFE0, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ACD20, D_8009FFEC, (c << 8) | 0x10001);
    St_sprt_init(pSt, (void *)0x801ACD48, D_8009FFF8, (c << 8) | 0x10001);
    c = pSt->Clut_parts + 1;
    St_sprt_init(pSt, (void *)0x801AC0C8, Cond_spr_tbl, (c << 8) | 0x10006);
    St_sprt_init(pSt, (void *)0x801AC410, D_8009FBA8, (pSt->Clut_zoom << 8) | 2);
    St_line_init(pSt);
}


void St_item_disp(ST_WORK *pSt)
{
    tagGLOBAL *pG = &G;
    SPRT *p;
    ST_FT4_DEF *pT;
    POLY_FT4 *pFt;
    ITEM_WORK *pI;
    ITEM_DATA *pData;
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
        setRGB0(p, pSt->Blink - 0x80, pSt->Blink - 0x80, pSt->Blink - 0x80);
    } else {
        setRGB0(&p[0], 0x80, 0x80, 0x80);
        setRGB0(&p[1], 0x80, 0x80, 0x80);
    }
    p = (SPRT *)0x801ACA28;
    if ((*(ULONG *)&pSt->Rtn0 & 0xFFFF00) == 0x10700) {
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ACA3C;
        }
        setRGB0(p, pSt->Blink - 0x80, pSt->Blink - 0x80, pSt->Blink - 0x80);
    } else {
        setRGB0(&p[0], 0x80, 0x80, 0x80);
        setRGB0(&p[1], 0x80, 0x80, 0x80);
    }
    p = (SPRT *)0x801ACA50;
    if ((*(ULONG *)&pSt->Rtn0 & 0xFFFF00) == 0x30500) {
        if (G.Cbuf_id) {
            p = (SPRT *)0x801ACA64;
        }
        setRGB0(p, pSt->Blink - 0x80, pSt->Blink - 0x80, pSt->Blink - 0x80);
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
    pI = &G.pItem_set->Item[pSt->Cursor];
    pData = &Item_data_tbl[pI->Id];
    St_sprt_draw(pSt, (SPRT *)0x801AC000, D_8009F890, 0x30002);
    switch ((ULONG)G.Item_work_no) {
    case 0:
        St_sprt_draw(pSt, (SPRT *)0x801AC050, D_8009F8A8, ((n | 4) << 16) | 1);
        break;
    case 1:
        St_sprt_draw(pSt, (SPRT *)0x801AC078, D_8009F8B4, ((n | 4) << 16) | 1);
        break;
    case 2:
        St_sprt_draw(pSt, (SPRT *)0x801AC0A0, D_8009F8C0, ((n | 4) << 16) | 1);
        break;
    }
    St_sprt_draw(pSt, (SPRT *)0x801AC0C8 + pSt->Condition * 2, &Cond_spr_tbl[pSt->Condition], ((n * 2 + 2) << 16) | 1);
    St_sprt_draw(pSt, (SPRT *)0x801AC460, D_8009FBC0, ((n | 2) << 16) | 0x101);
    St_sprt_draw(pSt, (SPRT *)0x801AC1B8, D_8009F8CC, 0x30105);
    St_sprt_draw(pSt, (SPRT *)0x801AC398, D_8009F9D4, ((n | 2) << 16) | 0x103);
    St_sprt_draw(pSt, (SPRT *)0x801AC488, D_8009FF8C, ((n * 2 + 2) << 16) | 0x103);
    St_ft4_draw(pSt, (void *)0x801AE5A8, D_8009F9B0, 0x70103);
    St_ft4_draw(pSt, (void *)0x801ADBA8, D_8009FC5C, 0x70808);
    St_ft4_draw(pSt, (void *)0x801AEFA8, D_8009FF2C, 0x70801);
    St_sprt_draw(pSt, (SPRT *)0x801AC410, D_8009FBA8, (((n * 3) | 1) << 16) | 0x802);
    if (pSt->Flg & 0x1000) {
        St_tile_draw(pSt, (TILE *)0x801AF508, D_8009FB90, 0x70902);
        St_ft4_draw(pSt, (void *)0x801ADE28, D_8009FCBC, 0x70908);
        St_file_icon_disp(pSt);
        if (pSt->Flg & 0x2000) {
            St_sprt_draw(pSt, (SPRT *)0x801ADB58, D_8009FB78, ((n * 2 + 1) << 16) | 0x1701);
        }
        if (pSt->Flg & 0x4000) {
            St_sprt_draw(pSt, (SPRT *)0x801ADB30, D_8009FB84, ((n * 2 + 1) << 16) | 0x1601);
        }
        if (pSt->Flg & 0x1000000) {
            St_sprt_draw(pSt, (SPRT *)0x801ACA50, D_8009FF80, ((n * 2 + 1) << 16) | 0xE01);
            k = pSt->File_cursor;
            id = File_order_tbl[k];
            if (G.Unk79EC[3] == 30 && k == 0) {
                id = 30;
            }
            if (Flg_ck(&G.Unk79EC[2], id)) {
                Name_disp(0xE, 0xC8, 0x501, id + 0x85);
            }
        }
        St_ft4_draw(pSt, (void *)0x801AE0A8, D_8009FD1C, 0x70A08);
        St_ft4_draw(pSt, (void *)0x801AF048, D_8009FF44, 0x70A01);
    }
    if (pSt->Flg & 0x400000) {
        St_zoom_icon_disp(pSt);
    }
    St_ft4_draw(pSt, (void *)0x801AE328, D_8009FD7C, 0x70B08);
    St_ft4_draw(pSt, (void *)0x801AF098, D_8009FF50, 0x70B01);
    if (pSt->Sel_item.Flg & 0x10) {
        St_sprt_draw(pSt, (SPRT *)0x801ACA78, D_8009FBCC, 0x60304);
    } else {
        St_sprt_draw(pSt, (SPRT *)0x801ACB18, D_8009FBFC, 0x60304);
    }
    St_sprt_draw(pSt, (SPRT *)0x801ACBB8, D_8009FC2C, 0x40401);
    St_sprt_draw(pSt, (SPRT *)0x801ACBE0, D_8009FC38, 0x40501);
    St_sprt_draw(pSt, (SPRT *)0x801ACC08, D_8009FC44, 0x40601);
    if (pSt->Sel_item.Flg & 0x10) {
        St_sprt_draw(pSt, (SPRT *)0x801ACC30, D_8009FC50, 0x40701);
    }
    if (pData->Type == 1) {
        St_sprt_draw(pSt, (SPRT *)0x801ACC58, D_8009FFB0, ((n * 2 + 4) << 16) | 0x401);
    } else {
        St_sprt_draw(pSt, (SPRT *)0x801ACC80, D_8009FFBC, ((n * 2 + 4) << 16) | 0x401);
    }
    St_sprt_draw(pSt, (SPRT *)0x801ACCA8, D_8009FFC8, ((n * 2 + 4) << 16) | 0x501);
    St_sprt_draw(pSt, (SPRT *)0x801ACCF8, D_8009FFE0, ((n * 2 + 4) << 16) | 0x601);
    if (pSt->Sel_item.Flg & 0x10) {
        if (pI->Flg & 0x100) {
            St_sprt_draw(pSt, (SPRT *)0x801ACD48, D_8009FFF8, ((n * 2 + 4) << 16) | 0x701);
        } else {
            St_sprt_draw(pSt, (SPRT *)0x801ACD20, D_8009FFEC, ((n * 2 + 4) << 16) | 0x701);
        }
    }
    pT = D_8009FE6C;
    pFt = (POLY_FT4 *)0x801AEAA8;
    if ((signed char)pSt->Cmd_cursor == 0) {
        pT += 2;
        pFt = (POLY_FT4 *)0x801AEB48;
    }
    St_ft4_draw(pSt, pFt, pT, 0x50402);
    pT = D_8009FE9C;
    pFt = (POLY_FT4 *)0x801AEBE8;
    if ((signed char)pSt->Cmd_cursor == 1) {
        pT += 2;
        pFt = (POLY_FT4 *)0x801AEC88;
    }
    St_ft4_draw(pSt, pFt, pT, 0x50502);
    pT = D_8009FECC;
    pFt = (POLY_FT4 *)0x801AED28;
    if ((signed char)pSt->Cmd_cursor == 2) {
        pT += 2;
        pFt = (POLY_FT4 *)0x801AEDC8;
    }
    St_ft4_draw(pSt, pFt, pT, 0x50602);
    if (pSt->Sel_item.Flg & 0x10) {
        pT = D_8009FEFC;
        pFt = (POLY_FT4 *)0x801AEE68;
        if ((signed char)pSt->Cmd_cursor == 3) {
            pT += 2;
            pFt = (POLY_FT4 *)0x801AEF08;
        }
        St_ft4_draw(pSt, pFt, pT, 0x50702);
    }
    p = (SPRT *)0x801ACD70;
    if (G.Cbuf_id) {
        p = (SPRT *)0x801ACD84;
    }
    i = 0;
    if (G.Item_work[0].Num) {
        do {
            pI = &pG->pItem_set->Item[i];
            x = Item_num_pos[i][0] + pSt->Pos[1].X;
            y = Item_num_pos[i][1] + pSt->Pos[1].Y;
            xy = x | (y << 16);
            c = pSt->Clut_parts + ((pI->Flg & 0xC) >> 2) + 2;
            p = St_num_disp(p, pI->Num, xy, c | (((UCHAR)pI->Flg & 3) << 8) | ((n * 2 + 2) << 16));
        } while (++i < D_800D225E);
    }
    if ((i = pG->pItem_set->Equip_no) != 0xFF) {
        pI = &pG->pItem_set->Item[i];
        c = pSt->Clut_parts + ((pI->Flg & 0xC) >> 2) + 2;
        x = Item_num_pos[10][0] + pSt->Pos[1].X;
        y = Item_num_pos[10][1] + pSt->Pos[1].Y;
        St_num_disp(p, pI->Num, x | (y << 16), c | (((UCHAR)pI->Flg & 3) << 8) | ((n * 2 + 2) << 16));
    }
    pT = D_8009FDDC;
    pFt = (POLY_FT4 *)0x801AE698;
    if (pSt->Cursor == -2) {
        pT += 2;
        pFt = (POLY_FT4 *)0x801AE738;
    }
    St_ft4_draw(pSt, pFt, pT, 0x40102);
    pT = D_8009FE0C;
    pFt = (POLY_FT4 *)0x801AE7D8;
    if (pSt->Cursor == -1) {
        pT += 2;
        pFt = (POLY_FT4 *)0x801AE878;
    }
    St_ft4_draw(pSt, pFt, pT, 0x40102);
    pT = D_8009FE3C;
    pFt = (POLY_FT4 *)0x801AE918;
    if (pSt->Cursor < -2) {
        pT += 2;
        pFt = (POLY_FT4 *)0x801AE9B8;
    }
    St_ft4_draw(pSt, pFt, pT, 0x40102);
    if ((pSt->Flg & 0x4000000) && pSt->Cursor >= 0) {
        St_sprt_draw(pSt, (SPRT *)0x801ACA00, D_8009FF68, ((n * 2 + 1) << 16) | 0xC01);
    }
    pG2 = &G;
    if (pSt->Flg & 0x2000000) {
        St_sprt_draw(pSt, (SPRT *)0x801ACA28, D_8009FF74, ((n * 2 + 1) << 16) | 0xD01);
    }
    if (pG2->pItem_set->Num == 8) {
        St_sprt_draw(pSt, (SPRT *)0x801AC280, D_8009F908, 0x10207);
        St_tile_draw(pSt, (TILE *)0x801AF4E8, D_8009FF5C, 0x10201);
    }
    if (pSt->Flg & 0x20000000) {
        if (pSt->Flg & 0x2000000) {
            no = (signed char)pSt->Cursor2;
            pI = &pG2->pItem_set->Item[no];
        } else {
            no = pSt->Cursor;
            pI = &pG2->pItem_set->Item[no];
        }
        name = pI->Id;
        if (no >= 0) {
            Name_disp(0xE, 0xAD, 0x501, name);
        }
    }
    St_ecg_disp(pSt);
}

void St_zoom_icon_disp(ST_WORK *pSt)
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
    p->clut = (pSt->Clut_zoom + 0x1E0) << 6;
    w = (signed char)pSt->Zoom_w;
    h = (signed char)pSt->Zoom_h;
    x = pSt->Pos[15].X + pSt->Pos[8].X;
    y = pSt->Pos[15].Y + pSt->Pos[8].Y;
    setXY4(p, x - w, y - h, x + w, y - h, x - w, y + h, x + w, y + h);
    AddPrim(G.pOt0 + 1, p);
}

void St_ecg_disp(ST_WORK *pSt)
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
    pTbl = Ecg_wave_tbl[pSt->Condition];
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
    pC = &Ecg_col_tbl[pSt->Condition];
    for (; i < n; p += 2, i++) {
        if (pSt->Ecg_pos + i >= 0) {
            if (pSt->Ecg_pos + i < 0x4A) {
                k = n - i - 1;
                p->r0 = pC->R - pC->Dr * k;
                p->g0 = pC->G - pC->Dg * k;
                p->b0 = pC->B - pC->Db * k;
                p->x0 = i + (pSt->Ecg_pos + x);
                p->x1 = i + (pSt->Ecg_pos + x);
                if (pSt->Flg & 0x800000) {
                    p->y0 = y + 2;
                    p->y1 = y + 0x1D;
                } else {
                    p->y0 = pTbl[(pSt->Ecg_pos + i) * 2] + y;
                    p->y1 = pTbl[(pSt->Ecg_pos + i) * 2 + 1] + (pTbl[(pSt->Ecg_pos + i) * 2] + y);
                }
                AddPrim(&pSt->Ot[G.Cbuf_id][ot], p);
            }
        }
    }
}

SPRT *St_num_disp(SPRT *p, SHORT num, ULONG xy, ULONG prm)
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

void St_file_icon_disp(ST_WORK *pSt)
{
    SPRT *p;
    DR_TPAGE *pTp;
    ST_SPR_DEF *pI;
    ULONG i;
    LONG k;
    SHORT x, y;

    p = (SPRT *)0x801AC500;
    pTp = &pSt->File_tpage[0];
    if (G.Cbuf_id) {
        pTp = &pSt->File_tpage[1];
    }
    SetDrawTPage(pTp, 0, 0, 0x97);
    AddPrim(G.pOt0 + 7, pTp);
    i = 0;
    if (G.Cbuf_id) {
        p = (SPRT *)0x801AC514;
    }
    k = 24;
    for (; i < 30; p += 2, i++) {
        pI = &File_icon_tbl[File_order_tbl[i]];
        setSprt(p);
        setRGB0(p, 0x80, 0x80, 0x80);
        p->w = pI->W;
        p->h = pI->H;
        p->clut = (pSt->Clut_file + 0x1E0) << 6;
        if (Flg_ck(&G.Unk79EC[2], File_order_tbl[i])) {
            p->u0 = pI->U;
            p->v0 = pI->V;
        } else {
            p->u0 = File_icon_tbl[31].U;
            p->v0 = File_icon_tbl[31].V;
        }
        if (G.Unk79EC[3] == 30 && i == 0) {
            p->u0 = File_icon_tbl[30].U;
            p->v0 = File_icon_tbl[30].V;
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
    for (; (UCHAR)i < G.pItem_set->Num; i++) {
        if (G.pItem_set->Item[(UCHAR)i].Id == id) {
            return i;
        }
    }
    return -1;
}

signed char Item_decrease(UCHAR no, signed char n, UCHAR f)
{
    ITEM_WORK *p = &G.pItem_set->Item[no];

    if ((p->Flg & 3) != 3) {
        p->Num -= n;
    }
    if (p->Num == 0 && f == 0) {
        Item_delete(no);
    }
    return (signed char)p->Num;
}

/* needs ITEM_SET item table typed as ITEM_WORK Item[0x4A] (typedef struct tagST_ITEM { UCHAR Id; UCHAR Num; USHORT Flg; } ITEM_WORK;) */
void Item_delete(UCHAR no)
{
    tagGLOBAL *pG = &G;
    ST_WORK *pSt = &St;
    UCHAR i;

    for (i = no; i < pG->pItem_set->Num - 1; i++) {
        pG->pItem_set->Item[i].Id = pG->pItem_set->Item[i + 1].Id;
        pG->pItem_set->Item[i].Num = pG->pItem_set->Item[i + 1].Num;
        pG->pItem_set->Item[i].Flg = pG->pItem_set->Item[i + 1].Flg;
    }
    pG->pItem_set->Item[i].Id = 0;
    pG->pItem_set->Item[i].Num = 0;
    pG->pItem_set->Item[i].Flg = 0;
    if (pSt->Equip_save) {
        if (pSt->Equip_no_save >= no && pSt->Equip_no_save != 0xFF) {
            pSt->Equip_no_save--;
        }
    } else if (pG->pItem_set->Equip_no >= no && pG->pItem_set->Equip_no != 0xFF) {
        pG->pItem_set->Equip_no--;
    }
    pSt->Flg14 |= 0x40000000;
}

void Equip_item_delete(void)
{
    UCHAR no;

    if ((no = G.pItem_set->Equip_no) != 0xFF) {
        Item_delete(no);
        G.pItem_set->Equip_id = 0;
        G.pItem_set->Equip_no = 0xFF;
    }
}

/* needs ITEM_WORK Item[] in ITEM_SET (see Item_delete); Search_item_tama first param changed to ST_WORK *pSt; Item_decrease param n changed to signed char (both still match) */
signed char Tama_reload(UCHAR f)
{
    ST_WORK *pSt = &St;
    tagGLOBAL *pG = &G;
    ITEM_TAMA *pTama;
    ITEM_WORK *p;
    signed char no;
    signed char n;
    UCHAR num;
    ITEM_DATA *pData;

    if (pG->pItem_set->Equip_no == 0xFF) {
        return -1;
    }
    pTama = Search_item_tama(pSt, pG->pItem_set->Equip_id);
    if (pTama == (ITEM_TAMA *)-1) {
        return -1;
    }
    no = Search_item_id(pTama->Tama_id, 0);
    if (no == -1) {
        return -1;
    }
    if (f) {
        p = &pG->pItem_set->Item[pG->pItem_set->Equip_no];
        pData = &Item_data_tbl[p->Id];
        num = p->Num;
        n = pData->Max - num;
        if ((UCHAR)n >= pG->pItem_set->Item[no].Num) {
            n = pG->pItem_set->Item[no].Num;
        }
        p->Num = num + n;
        Item_decrease(no, n, 0);
        pG->Player.Status_flg |= 0x400;
    }
    return 0;
}

LONG Tama_decrease(UCHAR f)
{
    ITEM_WORK *p;
    USHORT w;
    LONG t;

    if (G.pItem_set->Equip_no == 0xFF) {
        return -1;
    }
    p = &G.pItem_set->Item[G.pItem_set->Equip_no];
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

void Status_init(void)
{
    tagGLOBAL *pG = &G;
    tagGLOBAL *pG2;
    UCHAR mode;
    ULONG j, k;
    LONG unused[2];
    ITEM_WORK *pS;
    ITEM_WORK *pD;

    pG->Item_work_no = 0;
    pG->pItem_set = &pG->Item_work[0];
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
            pS = Item_init_normal[j];
            break;
        case 1:
            pS = Item_init_easy[j];
            break;
        case 2:
            switch (G.Player.Id) {
            case 8:
                pS = Item_init_pl8;
                break;
            case 9:
                pS = Item_init_pl9;
                break;
            case 10:
                pS = Item_init_pl10;
                break;
            }
            break;
        case 3:
            pS = Item_init_mode3;
            break;
        case 4:
            pS = Item_init_mode4;
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
            pS = Box_init_easy[j];
        } else {
            pS = Box_init_normal[j];
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
        G.Item_work[j].Equip_id = pD->Id;
        G.Item_work[j].Equip_no = 0;
        for (k = 0; k < 4; k++) {
            G.Item_work[j].Tama_mix_cnt[k] = 0;
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

void St_tex_load(void)
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
    p = G.pItem_set->Item;
    for (i = 0; i < 10; i++, p++) {
        Pix_unpack(i, p->Id, 0x801B1500, (ULONG *)0x801B1000);
        DrawSync(0);
    }
    if (G.pItem_set->Equip_no != 0xFF) {
        Pix_move(10, G.pItem_set->Equip_no);
    }
    n = G.pItem_set->Equip_id;
    pSt->Clut_frame = 4;
    pSt->Clut_icon = 5;
    pSt->Clut_face = 6;
    pSt->Clut_zoom = 8;
    pSt->Clut_box = 14;
    pSt->Clut_page = 10;
    pSt->Clut_parts = 0;
    pSt->Clut_file = 9;
    pSt->Clut_unused = 0;
    pSt->St_type = 0;
    pSt->Hiki_flg = 0;
    pSt->Equip_save = 0;
    pSt->Map_event = 0;
    pSt->Equip_id_old = n;
}

INCLUDE_RODATA("asm/nonmatchings/game/status", D_80010FEC);

void St_init(ST_WORK *pSt)
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
    if (pSt->Equip_save) {
        pSt->Equip_save = 0;
        pG->pItem_set->Equip_no = pSt->Equip_no_save;
        pG->pItem_set->Equip_id = pSt->Equip_id_save;
        pPl->Type &= 0xFF00;
        pPl->Type |= pG->pItem_set->Equip_id;
        Weapon_cls(pPl);
        Pl_weapon_ch(pPl);
    }
    pW->UnkB1C = 0x80;
    pW->UnkB20 = 0x3FFF;
    pW->UnkB24 = 0xB00;
    Cd_seek(0x34, 0, "SEEK ITEMI.PIX");
    if (pSt->Flg14 & 0x40000000) {
        Cd_read(0x32, (ULONG *)0x801B1500, 0, D_80010FEC);
        p = (UCHAR *)pG->pItem_set->Item;
        for (i = 0; i < 10; i++, p += 4) {
            Pix_unpack(i, *p, 0x801B1500, (ULONG *)0x801B1000);
            DrawSync(0);
        }
        if (G.pItem_set->Equip_no != 0xFF) {
            Pix_move(10, pG->pItem_set->Equip_no);
        } else {
            Pix_move(10, 11);
        }
        pSt->Flg14 &= ~0x40000000;
    }
    pSt->Rtn0 = 0;
    pSt->Rtn1 = 0;
    pSt->Rtn2 = 0;
    pSt->Timer = 0;
    for (i = 0; i < 6; i++) {
        *((UCHAR *)pSt + i + 0x1C) = 0;
    }
    pSt->Task_on = 1;
    pSt->Flg = 0;
    for (i = 0; i < 32; i++) {
        pSt->Pos[i].X = 0;
        pSt->Pos[i].Y = 0;
    }
    pSt->Slide_step = 0;
    pSt->Slide_done = 0;
    pSt->Slide_cnt = 0;
    pSt->Slide_no = 0;
    pSt->Equip_id_old = pG->pItem_set->Equip_id;
    pSt->Pl_id = pG->Player.Id;
    pSt->Ecg_pos = 0;
    pSt->Blink = 0;
    pSt->Blink_dir = 0;
    St_map_pos_init(pSt);
    switch (pSt->Map_event) {
    case 1:
        pSt->Map_stage = 0;
        break;
    case 2:
        pSt->Map_stage = 4;
        break;
    }
    if (pSt->St_type == 1) {
        if (*pD >= 0x85 && *pD < 0xA4) {
            pSt->St_type = 3;
            pSt->File_no = *pD + 0x7B;
            pSt->Page_no = 0;
            Flg_on(&G.Unk79EC[2], pSt->File_no);
            for (i = 0; i < 30; i++) {
                if (File_order_tbl[i] == pSt->File_no) {
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
        St_prim_init(pSt);
        break;
    case 2:
        pSt->pBuf = (ULONG *)0x801A1500;
        r.x = 0x1C0;
        r.y = 0x100;
        r.w = 0x40;
        r.h = 0x100;
        StoreImage(&r, (ULONG *)0x80194000);
        DrawSync(0);
        St_box_prim_set(pSt);
        i = Cd_read(0x32, pSt->pBuf, 0, D_80010FEC);
        pSt->Box_cursor = 0x20;
        pSt->pBuf = (ULONG *)((UCHAR *)pSt->pBuf + i);
        p = (UCHAR *)&G.pItem_set->Item[40];
        for (i = 30; i < 35; i++, p += 4) {
            Pix_unpack_box(i, *p, 0x801A1500, (ULONG *)0x801A1000);
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
        Itemg_load(*pD);
        Pix_load(12, *pD);
        pSt->Pos[3].X = 0x48;
        pSt->Pos[4].X = 0x48;
        pSt->Pos[5].X = 0x48;
        pSt->Pos[6].X = 0x48;
        pSt->Pos[7].X = 0x48;
        pSt->Pos[9].X = -0xD8;
        pSt->Pos[14].X = -0xD8;
        pSt->Pos[8].X = 0;
        pSt->Pos[10].Y = 0x30;
        St_prim_init(pSt);
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
        St_prim_init(pSt);
        pSt->Flg |= 0x40000000;
        pSt->Flg |= 0x4000000;
        break;
    }
    Fade_set(0, 0, 0, 2, 0xFFFFFF, 0, 6);
}


void Status(void)
{
    ST_WORK *pSt = &St;

    St_init(pSt);
    while (pSt->Task_on) {
        if (Main.Unk194 != 1) {
            St_main_tbl[pSt->Rtn0](pSt);
            St_counter_move(pSt);
        }
        St_draw(pSt);
        St_frame_end(pSt);
        Task_sleep(1);
    }
    St_exit(pSt);
    Task_exit();
}

void St_exit(ST_WORK *pSt)
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
        if (pSt->Hiki_flg) {
            D_800D442E = 1;
        } else {
            D_800D442E = 2;
        }
        pSt->Hiki_flg = 0;
        D_800D442D = 2;
        break;
    }
    pSt->St_type = 0;
    pSt->Map_event = 0;
    pPl = PLAYER_EM(pG);
    if (pSt->Equip_id_old != pG->pItem_set->Equip_id) {
        pSt->Equip_id_old = pG->pItem_set->Equip_id;
        pPl->Type &= 0xFF00;
        pPl->Type |= pG->pItem_set->Equip_id;
        Weapon_cls(pPl);
        Pl_weapon_ch(pPl);
    }
    Cd_seek(D_8009DFD0[pG->Unk783E][pG->Unk7840 + 1] - 1, 0, "SEEK BSS");
    D_800D442C = 2;
    Task_signal(0);
}

void St_counter_move(ST_WORK *pSt)
{
    pSt->Condition = St_condition_get();
    if (*(USHORT *)&pSt->Rtn0 == 0x205) {
        return;
    }
    if (pSt->Blink_dir) {
        pSt->Blink -= 2;
    } else {
        pSt->Blink += 2;
    }
    if (pSt->Blink >= 0x3F) {
        pSt->Blink = 0x3F;
        pSt->Blink_dir = 1;
    }
    if (pSt->Blink <= 0) {
        pSt->Blink = 0;
        pSt->Blink_dir = 0;
    }
    if (pSt->Flg & 0x800000) {
        pSt->Ecg_pos += 3;
    } else if (++pSt->Ecg_pos > 0x50) {
        pSt->Ecg_pos = -0x20;
    }
}

void St_draw(ST_WORK *pSt)
{
    switch (pSt->St_type) {
    case 4:
    case 5:
        if (pSt->Flg & 0x200000) {
            St_map_draw(pSt);
        }
        break;
    case 2:
        St_box_draw(pSt);
        break;
    case 0:
    case 1:
        if (pSt->Flg & 0x40000000) {
            St_item_disp(pSt);
        }
        if (pSt->Flg & 0x200000) {
            St_map_draw(pSt);
        }
    case 3:
        if (pSt->Flg & 0x100000) {
            St_file_draw(pSt);
        }
        break;
    }
}

void St_open_wait(ST_WORK *pSt)
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

void St_close(ST_WORK *pSt)
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
            pSt->Task_on = 0;
        }
        break;
    }
}

LONG St_condition_get(void)
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

/* prototype in file changed: void St_sprt_init(ST_WORK *pSt, SPRT *p, UCHAR *pTbl, ULONG f); */
void St_sprt_init(ST_WORK *pSt, SPRT *p, ST_SPR_DEF *pDef, ULONG prm)
{
    ULONG i, j;
    UCHAR y = prm >> 8;
    UCHAR semi = prm >> 16;
    ULONG n = prm;

    for (i = 0; i < (UCHAR)n; i++) {
        for (j = 0; j < 2; j++) {
            setSprt(p);
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = pDef->U;
            p->v0 = pDef->V;
            p->w = pDef->W;
            p->h = pDef->H;
            if (semi == 0) {
                p->clut = (y + 0x1E0) << 6;
            } else {
                p->clut = ((y + 0x1E0) << 6) | 0x13;
                setSemiTrans(p, 1);
            }
            p++;
        }
        pDef++;
    }
}

void St_ft4_init(ST_WORK *pSt, POLY_FT4 *p, ST_FT4_DEF *pDef, ULONG prm)
{
    ULONG i, j;
    UCHAR y = prm >> 8;
    UCHAR semi = prm >> 16;
    UCHAR w = prm >> 24;
    ULONG n = prm;
    LONG tp;
    ULONG u, v;

    for (i = 0; i < (UCHAR)n; i++) {
        u = pDef->U;
        v = pDef->V;
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
        pDef++;
    }
}

void St_tile_init(ST_WORK *pSt, TILE *p, ST_TILE_DEF *pDef, UCHAR n)
{
    ULONG i, j;

    for (i = 0; i < n; i++, pDef++) {
        for (j = 0; j < 2; j++) {
            setTile(p);
            setRGB0(p, 8, 8, 8);
            p->w = pDef->W;
            p->h = pDef->H;
            p++;
        }
    }
}

void St_line_init(ST_WORK *pSt)
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

void St_sprt_draw(ST_WORK *pSt, SPRT *p, ST_SPR_DEF *pDef, ULONG prm)
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
    for (i = 0; i < (UCHAR)n; i++, p += 2, pDef++) {
        p->x0 = pDef->X + ox;
        p->y0 = pDef->Y + oy;
        AddPrim(&pSt->Ot[G.Cbuf_id][ot], p);
    }
}

void St_ft4_draw(ST_WORK *pSt, POLY_FT4 *p, ST_FT4_DEF *pDef, ULONG prm)
{
    ULONG i;
    UCHAR ofs = prm >> 8;
    UCHAR ot = prm >> 16;
    ULONG n = prm;
    LONG x, y, x1, y1;

    if (G.Cbuf_id) {
        p++;
    }
    for (i = 0; i < (UCHAR)n; p += 2, pDef++, i++) {
        x = pSt->Pos[ofs].X + pDef->X;
        y = pSt->Pos[ofs].Y + pDef->Y;
        x1 = x + pDef->W;
        y1 = y + pDef->H;
        setXY4(p, x, y, x1, y, x, y1, x1, y1);
        AddPrim(G.pOt0 + ot, p);
    }
}

void St_tile_draw(ST_WORK *pSt, TILE *p, ST_TILE_DEF *pDef, ULONG prm)
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
    for (i = 0; i < (UCHAR)n; i++, p += 2, pDef++) {
        p->x0 = pDef->X + ox;
        p->y0 = pDef->Y + oy;
        AddPrim(G.pOt0 + ot, p);
    }
}

void St_mapping_tim(ULONG *pTim)
{
    TIM_IMAGE tim;
    TIM_IMAGE *p;
    LONG y;

    OpenTIM(pTim);
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

void St_chenge_pl(ULONG no)
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
        St_pl_kind = 0;
        break;
    case 8:
        St_pl_kind = 1;
        break;
    }
    G.pItem_set = &G.Item_work[G.Item_work_no];
    St.Flg14 |= 0x40000000;
}

void St_map_pos_init(ST_WORK *pSt)
{
    UCHAR c;

    pSt->Area_cursor = 0;
    pSt->Map_return = 0;
    pSt->Map_stage = (UCHAR)G.Unk783E;
    pSt->Map_room_cur = (UCHAR)G.Unk7840;
    Map_room_conv(&pSt->Map_stage, &pSt->Map_room_cur);
    pSt->Map_stage_cur = (UCHAR)G.Unk783E;
    c = (UCHAR)G.Unk7840;
    Map_room_conv(&pSt->Map_stage_cur, &c);
}

void St_map_init(ST_WORK *pSt)
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

    if (pSt->Map_return) {
        pSt->Rtn1 = 3;
        pSt->Rtn2 = 0;
        pSt->Rtn0++;
    } else {
        pSt->Rtn1 = 0;
        pSt->Rtn2 = 0;
        pSt->Rtn0++;
    }
    pSt->Map_return = 0;
    pSt->Flg &= ~0x20000000;
    pSt->Flg &= ~0x40000000;
    pSt->Flg &= ~0x80000000;
    pSt->Flg |= 0x200000;
    if (Flg_ck(&G.System_flg, 0x18)) {
        Map_file_load(pSt, pSt->Map_stage + 7);
    } else {
        Map_file_load(pSt, pSt->Map_stage);
    }
    on = 1;
    pInfo = (ST_MAP_INFO *)((UCHAR *)pSt->pMap_data + pSt->pMap_data[1]);
    pSt->Floor_flg = pInfo->Unk4;
    pSt->Floor_num = pInfo->Unk5;
    pSt->Pl_mark_flg = on;
    pSt->Map_x0 = pInfo->X;
    pSt->Map_y0 = pInfo->Y;
    if (pSt->Map_stage != pSt->Map_stage_cur) {
        pSt->Map_x_old = pSt->Map_x = pSt->Map_x0;
        pSt->Map_y_old = pSt->Map_y = pSt->Map_y0;
        if (pSt->Floor_flg) {
            pSt->Map_zoom = 100;
            pSt->Zoom_out_flg = 0;
            switch (pSt->Map_stage) {
            case 2:
            case 5:
                pSt->Floor_disp = 0;
                break;
            case 4:
            case 6:
                pSt->Floor_disp = on;
                break;
            }
            pSt->Floor_pl = -1;
        } else {
            pSt->Zoom_out_flg = on;
            pSt->Map_zoom = 50;
            pSt->Pl_mark_flg = 0;
        }
    } else {
        pSt->Zoom_out_flg = 0;
        pSt->Map_zoom = 100;
        if (pSt->Floor_flg) {
            pSt->Map_x = pSt->Map_x0;
            pSt->Map_y = pSt->Map_y0;
            pSt->Floor_disp = pSt->Floor_pl = Map_floor_get(pSt, pSt->Map_stage, pSt->Map_room_cur);
            if (St.Map_event == 2) {
                pSt->Floor_disp = on;
            }
            if (G.Unk783E == 4 && pSt->Map_room_cur == 3) {
                if (Flg_ck(G.Unk7868, 0xF1) && G.Cut_no_old != 0x503) {
                    pSt->Floor_pl = 0;
                    pSt->Floor_disp = 0;
                } else {
                    pSt->Floor_pl = on;
                    pSt->Floor_disp = on;
                }
            }
        } else {
            pRoom = (ST_MAP_ROOM *)pSt->pMap_data;
            x = G.Player.Pos_x;
            z = G.Player.Pos_z;
            pRoom = (ST_MAP_ROOM *)((UCHAR *)pRoom + *(ULONG *)pRoom);
            pRoom += pSt->Map_room_cur;
            if (G.Unk783E == 1 && G.Unk7840 == 15 && x >= -0x2276) {
                x += 0x251D;
            }
            if (G.Unk783E == 3 && G.Unk7840 == 21 && G.Player.Pos_x >= 800) {
                x = G.Player.Pos_x - 0x2328;
                z = G.Player.Pos_z;
            }
            pSt->Map_x = -(pRoom->X0 + (x - pRoom->X1) / 450);
            pSt->Map_y = (z - pRoom->Z1) / 450 - pRoom->Z0;
            pSt->Floor_disp = 0;
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
    for (i = 0, pTp = pSt->Map_tpage; i < 4; i++, pTp++) {
        SetDrawTPage(pTp, 0, 0, 0x18);
    }
    for (i = 0, pTp = pSt->Map_win_tpage; i < 2; i++, pTp++) {
        SetDrawTPage(pTp, 0, 0, 0x58);
    }
    pSt->Blink_timer = 0;
    pSt->Blink = 0;
    pSt->Blink_dir = 0;
    pSt->Map_return = 0;
    pSt->Map_x_max = pSt->Map_x0 + 0x140;
    pSt->Map_y_max = pSt->Map_y0 + 0xE0;
}

void St_map(ST_WORK *pSt)
{
    St_map_tbl[pSt->Rtn1](pSt);
}

void St_map_ctrl(ST_WORK *pSt)
{
    tagGLOBAL *pG;

    switch (St.Map_event) {
    case 1:
        if (pSt->Blink_dir == 0 && pSt->Blink == 0x1E) {
            Snd_se_stad(0x201, 0, 0, 0);
        }
        break;
    case 2:
        if (pSt->Blink_dir == 0 && pSt->Blink == 0x1E) {
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
        pSt->Timer = 0;
        Snd_se_stad(6, 0, 0, 0);
        return;
    }
    if (pSt->Floor_flg) {
        if (pG->Key_trg & 0x1000) {
            if (++pSt->Floor_disp >= pSt->Floor_num) {
                pSt->Floor_disp = 0;
            }
            Snd_se_stad(6, 0, 0, 0);
            return;
        }
        if (pG->Unk2100 & 0x4000) {
            if (--pSt->Floor_disp < 0) {
                pSt->Floor_disp = 0;
            } else {
                Snd_se_stad(6, 0, 0, 0);
            }
        }
        if (G.Unk2100 & 0x1000) {
            if (++pSt->Floor_disp >= pSt->Floor_num) {
                pSt->Floor_disp = pSt->Floor_num - 1;
            } else {
                Snd_se_stad(6, 0, 0, 0);
            }
        }
    } else {
        if (pG->Key_trg & 0x1000) {
            pSt->Rtn1++;
            if (pSt->Zoom_out_flg ^= 1) {
                Snd_se_stad(6, 0, 0, 0);
                pSt->Map_dx_acc = 0;
                pSt->Map_dy_acc = 0;
                pSt->Pl_mark_flg = 0;
                pSt->Map_x_old = pSt->Map_x;
                pSt->Map_y_old = pSt->Map_y;
                return;
            }
            Snd_se_stad(5, 0, 0, 0);
            pSt->Map_dx_acc = 0;
            pSt->Map_dy_acc = 0;
            return;
        }
        if (pSt->Zoom_out_flg == 0) {
            if ((USHORT)pG->Unk20F8 & 0x1000) {
                pSt->Map_y += 4;
            }
            if ((USHORT)pG->Unk20F8 & 0x4000) {
                pSt->Map_y -= 4;
            }
            if ((USHORT)pG->Unk20F8 & 0x8000) {
                pSt->Map_x += 4;
            }
            if ((USHORT)pG->Unk20F8 & 0x2000) {
                pSt->Map_x -= 4;
            }
        }
        if (pSt->Map_y > pSt->Map_y0 + 0xE0) {
            pSt->Map_y = pSt->Map_y0 + 0xE0;
        }
        if (pSt->Map_y < pSt->Map_y0 - 0xE0) {
            pSt->Map_y = pSt->Map_y0 - 0xE0;
        }
        if (pSt->Map_x > pSt->Map_x0 + 0x140) {
            pSt->Map_x = pSt->Map_x0 + 0x140;
        }
        if (pSt->Map_x < pSt->Map_x0 - 0x140) {
            pSt->Map_x = pSt->Map_x0 - 0x140;
        }
    }
    if (++pSt->Blink_timer >= 0x15) {
        pSt->Blink_timer = -0x14;
    }
}

void St_map_zoom(ST_WORK *pSt)
{
    if (pSt->Zoom_out_flg) {
        pSt->Map_dx_acc += pSt->Map_x_old - pSt->Map_x0;
        pSt->Map_dy_acc += pSt->Map_y_old - pSt->Map_y0;
        pSt->Map_x = pSt->Map_x_old - pSt->Map_dx_acc / 10;
        pSt->Map_y = pSt->Map_y_old - pSt->Map_dy_acc / 10;
        if ((pSt->Map_zoom -= 5) < 0x33) {
            pSt->Rtn1 = 0;
            pSt->Map_zoom = 0x32;
            pSt->Map_x = pSt->Map_x0;
            pSt->Map_y = pSt->Map_y0;
        }
    } else {
        pSt->Map_dx_acc += pSt->Map_x0 - pSt->Map_x_old;
        pSt->Map_dy_acc += pSt->Map_y0 - pSt->Map_y_old;
        pSt->Map_x = pSt->Map_x0 - pSt->Map_dx_acc / 10;
        pSt->Map_y = pSt->Map_y0 - pSt->Map_dy_acc / 10;
        if ((pSt->Map_zoom += 5) >= 100) {
            pSt->Map_zoom = 100;
            pSt->Rtn1 = 0;
            pSt->Pl_mark_flg = 1;
            pSt->Blink_timer = 0;
            pSt->Map_x = pSt->Map_x_old;
            pSt->Map_y = pSt->Map_y_old;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/status", St_map_area_select);

void St_map_fade_in(ST_WORK *pSt)
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

void St_map_area_change(ST_WORK *pSt)
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
            pSt->Timer = 0;
            pSt->Map_stage = Map_area_list[(signed char)pSt->Area_cursor];
            pSt->Map_return = 1;
        }
        break;
    }
}

void St_map_exit(ST_WORK *pSt)
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
            pSt->Timer = 0;
            pSt->Flg |= 0x20000000;
            pSt->Flg |= 0x40000000;
            pSt->Flg |= 0x80000000;
            St_prim_init(pSt);
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
            func_8001B894(0x705, &G.Ob_model[n & 0xFF].M);
            func_8001B894(0x905, 0);
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

void St_map_draw(ST_WORK *pSt)
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
    pOt0 = &pSt->Map_tpage[0];
    pOt1 = &pSt->Map_tpage[2];
    pOt2 = &pSt->Map_win_tpage[0];
    col = (UCHAR)pSt->Blink * 2 + 0x40;
    pHdr = (ST_MAP_HDR *)(pSt->pMap_file[1] + (ULONG)pSt->pMap_file);
    pRoom = pHdr->Room;
    num = pHdr->Num;
    if (G.Cbuf_id) {
        pP = (POLY_FT4 *)0x801AC028;
        pOt0 = &pSt->Map_tpage[1];
        pOt1 = &pSt->Map_tpage[3];
        pOt2 = &pSt->Map_win_tpage[1];
    }
    AddPrim(G.pOt0 + 1, pOt2);
    AddPrim(G.pOt0 + 1, pOt0);
    AddPrim(G.pOt0 + 6, pOt1);
    for (i = 0; i < num; i++, pRoom++) {
        if (!Flg_ck(Map_get_flg, pSt->Map_stage)) {
            if (i >= 0x30) {
                continue;
            }
            if (!Map_visit_ck(pSt->Map_stage, i)) {
                continue;
            }
        } else if (pSt->Map_stage == 4 && (ULONG)(i - 0xB) < 5) {
            if (!Map_visit_ck(pSt->Map_stage, i)) {
                continue;
            }
        }
        if (!Map_room_disp_ck(pSt->Map_stage, i)) {
            continue;
        }
        pPt = (ST_MAP_PART *)(pRoom->Ofs + (ULONG)pHdr);
        for (j = 0; j < pRoom->N0; j++, pPt++) {
            if (Map_part_disp_ck(pSt->Map_stage, i, j)) {
                Map_part_set(pSt, pP, pPt, Map_part_clut(pSt->Map_stage, i, j));
                if ((UCHAR)Map_part_blink_ck(pSt->Map_stage, i, j)) {
                    setRGB0(pP, col, col, col);
                }
                SetSemiTrans(pP, 1);
                addPrim((ULONG *)G.pOt0[5], pP);
                pP += 2;
            }
        }
        for (j = 0; j < pRoom->N1; j++, pPt++) {
            if (Map_door_disp_ck(pSt->Map_stage, i, j)) {
                c = Map_door_mark_ck(pSt->Map_stage, i, j);
                Map_part_set(pSt, pP, pPt, (c != 0) * 2);
                if (c) {
                    setRGB0(pP, col, col, col);
                }
                addPrim((ULONG *)G.pOt0[4], pP);
                pP += 2;
            }
        }
    }
    f = pSt->Map_stage == pSt->Map_stage_cur;
    if (pSt->Floor_flg && (signed char)pSt->Floor_pl != (signed char)pSt->Floor_disp) {
        f = 0;
    }
    if (f) {
        pRm = (SHORT *)pSt->pMap_data;
        pRm = (SHORT *)((UCHAR *)pRm + *(ULONG *)pRm);
        pRm += pSt->Map_room_cur * 4;
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
        mx = (dx + pSt->Map_x0) * pSt->Map_zoom / 100 - pSt->Map_x0 + pSt->Map_x;
        my = (dy + pSt->Map_y0) * pSt->Map_zoom / 100 - pSt->Map_y0 + pSt->Map_y;
        pP = (POLY_FT4 *)0x801AFE80;
        if (G.Cbuf_id) {
            pP = (POLY_FT4 *)0x801AFEA8;
        }
        setXY4(pP, mx + 0x9C, my + 0x74, mx + 0xA4, my + 0x74, mx + 0x9C, my + 0x7C, mx + 0xA4, my + 0x7C);
        setRGB0(pP, col, col, col);
        addPrim((ULONG *)G.pOt0[1], pP);
    }
    if (pSt->Pl_mark_flg) {
        pP = (POLY_FT4 *)0x801AFED0;
        if (G.Cbuf_id) {
            pP = (POLY_FT4 *)0x801AFEF8;
        }
        for (j = 0; j < 4; j++) {
            if (pSt->Floor_flg) {
                if ((ULONG)(j - 2) < 2) {
                    continue;
                }
                if ((signed char)pSt->Floor_disp == 0 && j == 1) {
                    continue;
                }
                if ((signed char)pSt->Floor_disp == pSt->Floor_num - 1 && j == 0) {
                    continue;
                }
            }
            i = 0;
            switch (j) {
            case 0:
                if (pSt->Map_y >= pSt->Map_y0 + 0xE0) {
                    i = 1;
                }
                break;
            case 1:
                if (pSt->Map_y <= pSt->Map_y0 - 0xE0) {
                    i = 1;
                }
                break;
            case 2:
                if (pSt->Map_x >= pSt->Map_x0 + 0x140) {
                    i = 1;
                }
                break;
            case 3:
                if (pSt->Map_x <= pSt->Map_x0 - 0x140) {
                    i = 1;
                }
                break;
            }
            if (i) {
                setRGB0(pP, 0x18, 0x18, 0x18);
            } else {
                i = pSt->Blink_dir;
                setRGB0(pP, 0x80, 0x80, 0x80);
            }
            pA = &Map_arrow[i][j];
            dx = pA->X;
            dy = pA->Y;
            setXY4(pP, dx - 7, dy - 6, dx + 6, dy - 6, dx - 7, dy + 6, dx + 6, dy + 6);
            setUV4(pP, pA->U0, pA->V0, pA->U1, pA->V0, pA->U0, pA->V1, pA->U1, pA->V1);
            setSemiTrans(pP, 1);
            addPrim((ULONG *)G.pOt0[1], pP);
            pP += 2;
        }
    }
    if (!pSt->Floor_flg) {
        pL = (LINE_F2 *)0x801B0010;  /* two TILEs (LINE_F2 shares the layout used here) */
        if (G.Cbuf_id) {
            pL = (LINE_F2 *)0x801B0020;
        }
        setRGB0(pL, pSt->Blink + 0x50, pSt->Blink + 0x40, pSt->Blink & 0x1F);
        addPrim(pOt0, pL);
        pL += 2;
        setRGB0(pL, pSt->Blink + 0x50, pSt->Blink + 0x40, pSt->Blink & 0x1F);
        addPrim(pOt0, pL);
    }
    pL = (LINE_F2 *)0x801B0050;
    if (G.Cbuf_id) {
        pL = (LINE_F2 *)0x801B0060;
    }
    zoom = pSt->Map_zoom;
    dy = pSt->Map_y0 - pSt->Map_y_max;
    dx = pSt->Map_x0 - pSt->Map_x_max;
    ox = pSt->Map_x - pSt->Map_x0 + 0xA0;
    oy = pSt->Map_y - pSt->Map_y0 + 0x78;
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
        if (pSt->Zoom_out_flg == 0) {
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
    for (i = 0; i < (signed char)pSt->Floor_disp; i++) {
        pS += 2;
    }
    addPrim(pOt0, pS);
    if (pSt->Rtn1 == 2) {
        pT = (TILE *)0x801B06A0;
        if (G.Cbuf_id) {
            pT = (TILE *)0x801B06B0;
        }
        setXY0(pT, 0x3D, 0x78 - pSt->Timer * 8);
        setWH(pT, 0xC8, pSt->Timer * 16);
        addPrim(pOt2, pT);
        if (pSt->Rtn2 == 1) {
            Mess_print(0x68, 0x34, Map_mess + Map_mess_ofs[7], 0x501);
            if (pSt->Menu_blink < 0x3F) {
                Mess_print(0x47, (signed char)pSt->Area_cursor * 16 + 0x4C, Map_mess + Map_mess_ofs[8], 0x501);
            }
            for (i = 0; i < 7; i++) {
                if (Flg_ck(Map_area_flg, Map_area_list[i])) {
                    Mess_print(0x55, i * 16 + 0x4C, Map_mess + Map_mess_ofs[Map_area_list[i]], 0x501);
                } else {
                    Mess_print(0x55, i * 16 + 0x4C, Map_mess + Map_mess_ofs[9], 0x531);
                }
            }
        }
    }
}

/* needs ST_MAP_PART (see St_map_draw.c draft) and ST_WORK Unk7C..UnkA4 */
void Map_part_set(ST_WORK *pSt, POLY_FT4 *p, ST_MAP_PART *pPt, UCHAR clut)
{
    LONG x, y, w, h, ox, oy, zoom;

    if (pPt->Clut == 7) {
        clut = 0;
    }
    w = pPt->U1 - pPt->U0 + 1;
    h = pPt->V1 - pPt->V0 + 1;
    x = pPt->X + pSt->Map_x0;
    y = pPt->Y + pSt->Map_y0;
    ox = pSt->Map_x - pSt->Map_x0 + 0xA0;
    oy = pSt->Map_y - pSt->Map_y0 + 0x78;
    zoom = pSt->Map_zoom;
    SetPolyFT4(p);
    setXY4(p, x * zoom / 100 + ox, y * zoom / 100 + oy, (x + w) * zoom / 100 + ox, y * zoom / 100 + oy, x * zoom / 100 + ox, (y + h) * zoom / 100 + oy, (x + w) * zoom / 100 + ox, (y + h) * zoom / 100 + oy);
    setUV4(p, pPt->U0, pPt->V0, pPt->U1 + 1, pPt->V0, pPt->U0, pPt->V1 + 1, pPt->U1 + 1, pPt->V1 + 1);
    p->tpage = (((pPt->Tpage >> 1) + 7) & 0xF) | 0x10;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->clut = ((pPt->Clut + ((pPt->Tpage & 1) * 8 + 0x1EA) + clut) << 6) | 0x13;
}

void Map_file_load(ST_WORK *pSt, UCHAR no)
{
    ULONG i, sum;

    i = 0;
    pSt->pMap_file = pSt->pBuf;
    CD.Fno = 0x3A;
    sum = 0;
    CD.Fsize = Map_file_size[no];
    CD.Fsector = (floc[0x3A].sec_high << 16) + floc[0x3A].sector;
    for (; i < no; i++) {
        sum += Map_file_size[i];
    }
    CD.Fsector += sum >> 11;
    CD.Fsum = Map_file_sum[no];
    Cd_read(0x3A, pSt->pMap_file, 2, "MAP.MAP");
    pSt->pMap_data = (ULONG *)(pSt->pMap_file[3] + (ULONG)pSt->pMap_file);
    Mapping_tpage_clut = 0xA17;
    Mapping_tim((ULONG *)(pSt->pMap_file[5] + (ULONG)pSt->pMap_file));
    Mapping_tpage_clut = 0x1A18;
    St_mapping_tim((ULONG *)(pSt->pMap_file[7] + (ULONG)pSt->pMap_file));
}

void Map_visit_set(UCHAR a, UCHAR b)
{
    ULONG *p;

    if (a == 0 && b == 16) {
        Map_visit_flg[0] |= 0x80000000U >> (b & 0x1F);
    }
    if (a == 1 && b == 0) {
        Map_visit_flg[2] |= 0x80000000U >> (b & 0x1F);
    }
    if (a == 1 && b == 1) {
        Map_visit_flg[1] |= 0x10000;
    }
    if (a == 3 && b == 21) {
        Map_visit_flg[6] |= 0x80000000U >> (b & 0x1F);
    }
    Map_room_conv(&a, &b);
    p = &Map_visit_flg[a * 2 + (b >> 5)];
    *p |= 0x80000000U >> (b & 0x1F);
    Flg_on(&Map_visit_flg[19], a);
}

ULONG Map_visit_ck(UCHAR a, UCHAR b)
{
    ULONG *p = &Map_visit_flg[a * 2 + (b >> 5)];

    return *p & (0x80000000U >> (b & 0x1F));
}


LONG Map_room_disp_ck(UCHAR st, UCHAR rm)
{
    ST_WORK *pSt = &St;
    USHORT code = pSt->Map_room_cur | (pSt->Map_stage_cur << 8);
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
    if (St.Floor_flg && (signed char)St.Floor_disp != Map_floor_get(pSt, st, rm)) {
        return 0;
    }
    return 1;
}

LONG Map_part_disp_ck(UCHAR st, UCHAR rm, UCHAR cut)
{
    tagGLOBAL *pG = &G;
    UCHAR *p = (UCHAR *)St.pMap_data;
    USHORT cur = St.Map_room_cur | (St.Map_stage_cur << 8);
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
    switch (St.Map_event) {
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

LONG Map_part_clut(UCHAR st, UCHAR rm, UCHAR cut)
{
    ST_WORK *pSt = &St;
    LONG cur = pSt->Map_room_cur | (pSt->Map_stage_cur << 8);
    UCHAR *p;

    if (rm < 0x30 && !Map_visit_ck(st, rm)) {
        return 5;
    }
    switch (st) {
    case 0:
        if (rm == 0x2F && !Map_visit_ck(1, 1)) {
            return 5;
        }
        if (rm == 0x10 && !Map_visit_ck(5, 0)) {
            return 5;
        }
        break;
    case 1:
        if (rm == 0 && !Map_visit_ck(0, 0x26)) {
            return 5;
        }
        if (rm == 0xC && pSt->Map_room_cur == rm) {
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
        p = (UCHAR *)St.pMap_data;
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
    if (pSt->Map_stage == pSt->Map_stage_cur && rm == pSt->Map_room_cur) {
        return 2;
    }
    return 0;
}

LONG Map_part_blink_ck(UCHAR st, UCHAR rm, UCHAR cut)
{
    switch (St.Map_event) {
    case 1:
        if (st == 0 && rm == 0 && cut == St.Map_event) {
            return 1;
        }
        break;
    case 2:
        if (st == 4 && rm == 3 && cut == St.Map_event) {
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

LONG Map_door_disp_ck(UCHAR st, UCHAR rm, UCHAR cut)
{
    tagGLOBAL *pG = &G;
    UCHAR fl = pG->Player.nFloor;
    LONG cur = St.Map_room_cur | (St.Map_stage_cur << 8);
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
    p = (UCHAR *)St.pMap_data;
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

LONG Map_door_mark_ck(UCHAR st, UCHAR rm, UCHAR cut)
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
    p = (UCHAR *)St.pMap_data;
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

UCHAR Map_floor_get(ST_WORK *pSt, UCHAR st, UCHAR rm)
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

void Map_room_conv(UCHAR *pA, UCHAR *pB)
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
    for (i = 0; Map_room_alias[i].A0 != 0xFF; i++) {
        if (Map_room_alias[i].A0 == *pA && Map_room_alias[i].B0 == *pB) {
            *pA = Map_room_alias[i].A1;
            *pB = Map_room_alias[i].B1;
        }
    }
}

void St_frame_end(ST_WORK *pSt)
{
}
