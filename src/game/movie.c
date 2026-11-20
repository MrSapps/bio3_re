#include "common.h"
#include "monitor.h"
#include "mojidisp.h"
#include "pad.h"
#include "em_sub.h"
#include "om.h"
#include "pl_wep2.h"
#include "movie.h"
#include "player.h"
#include "cd.h"
#include "snd.h"
#include <libpress.h>

/* RE3-only collision object list (80033508..80035790). Nodes are linked through pNext;
 * the player's node lives at Pl_work + 0x80. */
typedef struct tagCOL_OBJ {
    UCHAR Id;                          /* 0x00 */
    UCHAR Flg;                         /* 0x01 */
    UCHAR Type;                        /* 0x02 0 = player, < 3 = character */
    UCHAR Unk3;                        /* 0x03 */
    ENEMY_WORK *pEm;                   /* 0x04 */
    LONG Pos_x;                        /* 0x08 */
    LONG Pos_y;                        /* 0x0C */
    LONG Pos_z;                        /* 0x10 */
    SHORT Old_x;                       /* 0x14 */
    SHORT Old_y;                       /* 0x16 */
    SHORT Old_z;                       /* 0x18 */
    USHORT W0;                         /* 0x1A */
    USHORT D0;                         /* 0x1C */
    USHORT H;                          /* 0x1E */
    USHORT W1;                         /* 0x20 */
    USHORT D1;                         /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    SHORT Unk26;                       /* 0x26 */
    SHORT Unk28;                       /* 0x28 */
    SHORT Unk2A;                       /* 0x2A */
    SHORT Unk2C;                       /* 0x2C */
    USHORT Hit_flg;                    /* 0x2E */
    MATRIX *pM;                        /* 0x30 */
    struct tagCOL_OBJ *pNext;          /* 0x34 */
    UCHAR Em_idx;                      /* 0x38 */
    UCHAR Unk39;                       /* 0x39 */
    SHORT Unk3A;                       /* 0x3A */
    LONG Unk3C;                        /* 0x3C */
} COL_OBJ;

typedef struct tagCOL_ENT {            /* 0x18 bytes */
    SHORT X0;                          /* 0x00 */
    SHORT Z0;                          /* 0x02 */
    SHORT X1;                          /* 0x04 */
    SHORT Z1;                          /* 0x06 */
    USHORT Unk8;                       /* 0x08 */
    USHORT UnkA;                       /* 0x0A */
    LONG UnkC;                         /* 0x0C -1 terminates */
    UCHAR Type;                        /* 0x10 */
    UCHAR Em_idx;                      /* 0x11 */
    UCHAR pad12[2];
    COL_OBJ *pObj;                     /* 0x14 */
} COL_ENT;

typedef struct tagCOL_WORK {
    COL_OBJ *pTop;                     /* 0x00 */
    COL_OBJ *pTail;                    /* 0x04 */
    COL_ENT Ent[1];                    /* 0x08 */
} COL_WORK;
extern COL_WORK D_800DCED0;
extern COL_OBJ *D_800DCDD0[];

/* ENEMY_WORK fields not in em_sub.h yet */
#define EM_SHORT(p, ofs) (*(SHORT *)((UCHAR *)(p) + (ofs)))
#define EM_UCHAR(p, ofs) (*(UCHAR *)((UCHAR *)(p) + (ofs)))

extern ULONG D_8009CBC8[];
extern SHORT D_800DEB58[];
extern COL_ENT D_800DEB48;
LONG func_8004D720(VECTOR *pV, ULONG W, ULONG Arg2, ULONG Arg3);
void func_8004B7E8(ENEMY_WORK *pEm, COL_OBJ *pObj, LONG Arg2);

ULONG Flg_ck(ULONG *pFlg, ULONG Id);

/* array inside G at an offset not yet in tagGLOBAL (keeps the &G + index*size + ofs addressing) */
#define G_ARR(type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)&G)->a)
#define P_ARR(p, type, ofs) (((struct { UCHAR pad[ofs]; type a[1]; } *)(p))->a)

typedef ULONG OT8[8];

extern ULONG D_800CC87C;
extern ULONG D_800D1F2C;
extern UCHAR D_8009CD3C;
ULONG func_8003C390(ENEMY_WORK *pEm, ULONG Arg1);
LONG Hit_ck_point4_l(ULONG Arg0, ULONG Arg1) __asm__("func_8001020C");
void func_800288C8(ENEMY_WORK *pEm, PARTS_W *pParts, LONG Arg2, MATRIX *pM);

/* G.pUnk265C, G.pUnk2660 and G.pEnemy[40] seen as one array */
typedef struct {
    UCHAR pad0[0x265C];
    ENEMY_WORK *pWork[42];             /* 0x265C */
} G_WORKS;

UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);
LONG func_800336B4(ENEMY_WORK *pEm);
void func_80034124(COL_OBJ *pObj);
void func_800336F0(COL_OBJ *pTop, COL_ENT *pEnt);
void func_800337B8(COL_OBJ *pTop, COL_ENT *pEnt);
void func_8003389C(COL_OBJ *pTop, COL_ENT *pEnt);
void func_80033FBC(COL_OBJ *pTop);
void func_80033B88(COL_OBJ *pTop);
COL_ENT *func_80033CF4(COL_OBJ *pObj, COL_ENT *pEnt);
void func_80034284(COL_OBJ *pObj, COL_ENT *pEnt);

/* declarations from earlier parts of the original address-split file */
extern UCHAR D_800A0514[][4];
extern ULONG D_800D2048[];
extern UCHAR D_8009BEE4[];
extern USHORT D_8009C7C0[];

extern USHORT D_80099654[];
extern UCHAR D_80098E88[];
extern UCHAR D_800CC84D;
extern UCHAR D_800D4433;
void Mess_print(ULONG x, ULONG y, UCHAR *pStr, ULONG Attr);

static __inline__ UCHAR *Msg_get_80031BDC(ULONG No)
{
    return D_80098E88 + D_80099654[No];
}

ULONG Cd_read(LONG Fno, ULONG *pAdr, LONG a2, char *pName);
extern LOADTASK D_8009C944[];
ULONG Task_lexecute_(ULONG Level, ULONG Task_no) __asm__("Task_lexecute");

void Task_lchain_(ULONG Task_no) __asm__("Task_lchain");

void Init_movie_work(ULONG Id)
{
    MOVIE_TBL *pTbl;
    SND_WORK *pSnd = &Snd_work;
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    pTbl = &Movie_tbl[Id];
    if (pTbl->Attribute & 0x200)
        pWork->pFree_work = (UCHAR *)0x80100000;
    else
        pWork->pFree_work = (UCHAR *)0x80194000;
    if (pTbl->Attribute & 8) {
        pSnd->UnkB1C = 0x80;
        pSnd->UnkB24 = pSnd->UnkB20 = 0x3FFF;
        pWork->Save_flg = G.Unk2468;
        G.Unk2468 |= 0xFF000000;
    }
    pMovie_result = Ret_Value;
    pWork->Movie_Isrgb24 = 1;
    pVm_frm = (VM_FRAME *)pWork->pFree_work;
    pWork->pFree_work += sizeof(VM_FRAME);
    pWork->Movie_id = Id;
    pFile = (DslFILE *)pWork->pFree_work;
    pWork->pFree_work += sizeof(DslFILE);
    pAtv = (DslATV *)pWork->pFree_work;
    pWork->pFree_work += sizeof(DslATV);
    pWork->Rl_buff[0] = (ULONG *)pWork->pFree_work;
    pWork->pFree_work += Movie_tbl[pWork->Movie_id].Rl_size;
    if (pDisp->Attribute & 0x100) {
        pWork->Rl_buff[1] = (ULONG *)((UCHAR *)pWork->Rl_buff[0] + Movie_tbl[pWork->Movie_id].Rl_size);
        pWork->pFree_work += Movie_tbl[pWork->Movie_id].Rl_size;
    } else {
        pWork->Rl_buff[1] = pWork->Rl_buff[0];
    }
    pWork->pImage_buff = (ULONG *)((UCHAR *)pWork->Rl_buff[1] + Movie_tbl[pWork->Movie_id].Rl_size);
    pWork->pFree_work += 0x1E00;
    pWork->Movie_w = pTbl->Disp_w;
    pWork->Rno = 0;
    pWork->Movie_sync = 0;
    pDisp->Rect.w = pDisp->Width = 24;
    pDisp->Pos_x = pTbl->Pos_x;
    pDisp->Pos_y = pTbl->Pos_y;
    pDisp->Attribute = pTbl->Attribute;
    if (pDisp->Attribute & 2) {
        if (pWork->Eom == 2) {
            pWork->Eom = 1;
            pWork->Padoff_stop = 2;
            pDisp->Attribute = (pDisp->Attribute & ~0x80) | 0x40;
        } else {
            pWork->Eom = 0;
            pWork->Padoff_stop = 0;
        }
    } else {
        pWork->Eom = 0;
    }
    pWork->Movie_volume = pTbl->Volume * Snd_work.Unk7CA / 127;
    pWork->Frame = 0;
    pWork->Free0 = 0;
    pWork->Movie_cancel = pTbl->Cancel;
    G.System_flg |= 0x18000;
    Pad_vib_init();
}

extern void (*D_8009CBB4[])(void);

void Movie(void)
{
    VM_WORK *pWork;

    pWork = &Vm_work;
    do {
        D_8009CBB4[pWork->Rno]();
        if (G.System_flg & 0x8000) {
            Main.Db[G.Cbuf_id ^ 1].Disp.isrgb24 = pWork->Movie_Isrgb24;
            Main.Db[G.Cbuf_id ^ 1].Disp.disp.w = pWork->Movie_w;
        }
        if (pWork->Frame != 0) {
            G.Cbuf_id ^= 1;
        }
    } while (pWork->Rno < 2);
}

/* VRAM saved under an overlay movie (Movie_init) and restored by Movie_exit */
static const RECT Movie_save_rect __asm__("D_800106EC") = {0x140, 0, 0xA0, 0x1E0};
static const RECT Movie_save_rect2 __asm__("D_800106F4") = {0x1C0, 0xF0, 0x20, 0xF0};

void Movie_init(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;
    RECT Rect = Movie_save_rect;
    RECT Rect2 = Movie_save_rect2;

    if (pDisp->Attribute & 8) {
        Set_movie_volume2(pWork->Movie_volume);
        func_80074658(1);
        *(ULONG *)&Rect.w = 0x10000A0;
        StoreImage(&Rect, (ULONG *)(((ULONG)Vm_work.pFree_work + 15) & ~15));
        Vm_work.pFree_work += 0x14010;
        StoreImage(&Rect2, (ULONG *)(((ULONG)Vm_work.pFree_work + 15) & ~15));
        Vm_work.pFree_work += 0x3C10;
        DrawSync(0);
        if (Open_movie(pWork->Movie_id)) {
            Movie_end();
            pWork->Status = 1;
            pWork->Rno += 2;
        } else {
            pWork->Status = 0;
            pWork->Frame = 0;
            pWork->Rno++;
        }
    } else if (pWork->Eom) {
        Movie_end();
        pWork->Rno += 2;
    } else {
        Set_movie_volume2(pWork->Movie_volume);
        func_80074658(1);
        if (Open_movie(pWork->Movie_id)) {
            Movie_end();
            pWork->Status = 1;
            pWork->Rno += 2;
        } else {
            pWork->Status = 0;
            if (pDisp->Attribute & 1) {
                Fade_off(0);
                Bg_set_mode(0, 0);
            }
            pWork->Frame = 0;
            pWork->Rno++;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/movie", Movie_main);

void Movie_end(void)
{
    RECT unused_rect = {0x140, 0, 0xa0, 0x200};
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    Close_movie();
    if (pDisp->Attribute & 1)
        Bg_set_mode(2, 0);
    if (pDisp->Attribute & 0x40)
        Bg_set_mode(1, 0);
    if (pDisp->Attribute & 0x80)
        Bg_set_mode(1, 0);
    pWork->Movie_Isrgb24 = 0;
    pWork->Movie_w = 0x140;
    if (pDisp->Attribute & 8)
        G.System_flg &= ~0x8000;
    SetDispMask(0);
}

INCLUDE_ASM("asm/nonmatchings/game/movie", Movie_exit);

INCLUDE_ASM("asm/nonmatchings/game/movie", Open_movie);

INCLUDE_ASM("asm/nonmatchings/game/movie", Play_movie);

void Close_movie(void)
{
    Movie_xa_stop();
    DecDCToutCallback(NULL);
    DsDataCallback(NULL);
    DsReadyCallback(NULL);
    DsCommand(DslPause, NULL, NULL, -1);
    StUnSetRing();
    func_80074658(0);
}

void RuntoImage(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    pDisp->Cur_pos_x = pDisp->Pos_x;
    if (Main.Db[0].Draw.ofs[1] == 0) {
        pDisp->Rect.y = (G.Cbuf_id != 0) ? 0xF0 : 0;
    } else {
        pDisp->Rect.y = (G.Cbuf_id == 0) ? 0xF0 : 0;
    }
    pDisp->Count = 0;
    pDisp->Rect.y += pDisp->Pos_y;
    DecDCTin(pWork->Rl_buff[pWork->Cbuf_id], 1);
    DecDCTout(pWork->pImage_buff, pDisp->Size);
    DecDCTinSync(0);
}

void Mdec_callback(void)
{
    VM_WORK *pWork = &Vm_work;
    VM_DISP *pDisp = &Vm_disp;

    if (StCdIntrFlag) {
        StCdInterrupt();
        StCdIntrFlag = 0;
    }
    pDisp->Rect.x = pDisp->Cur_pos_x;
    LoadImage(&pDisp->Rect, pWork->pImage_buff);
    if (++pDisp->Count < pDisp->Loop) {
        pDisp->Cur_pos_x += pDisp->Width;
        DecDCTout(pWork->pImage_buff, pDisp->Size);
    } else {
        pWork->Movie_sync = 1;
    }
}

void Movie_xa_start(void)
{
    LONG Ctr;

    for (Ctr = 0; Ctr <= (LONG)Vm_work.Movie_volume; Ctr++) {
        Set_movie_volume2(Ctr);
    }
}

void Movie_xa_stop(void)
{
    LONG Ctr;

    for (Ctr = Vm_work.Movie_volume; Ctr >= 2; Ctr -= 2) {
        Set_movie_volume2(Ctr);
    }
    Set_movie_volume2(0);
}

void Set_movie_volume2(ULONG Volume)
{
    if (Stereo_flg == 0) {
        pAtv->val0 = pAtv->val2 = Volume;
        pAtv->val1 = pAtv->val3 = 0;
    } else {
        pAtv->val0 = pAtv->val1 = pAtv->val2 = pAtv->val3 = Volume >> 1;
    }
    DsMix(pAtv);
}

INCLUDE_ASM("asm/nonmatchings/game/movie", CdReset2);
