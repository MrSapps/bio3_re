#ifndef OVL_JILL_SEL_H
#define OVL_JILL_SEL_H
/*
 * JILL_SEL.BIN overlay (loaded at 0x801C2000): the "Jill" reward screen after
 * clearing the game. Each clear (G.Save_cnt) unlocks one of the five epilogue
 * files (bits 0-4 of G.Unk77F8); the screen shows Jill on a background with
 * sprite captions and a message, then fades out on a button press.
 * The sprite objects, the light helper and the model loaders are copies of the
 * SELECT overlay's (see ovl/select.h for SEL_OBJ / SEL_SPRT_DEF).
 */
#include "bio3.h"
#include "ovl/select.h"

/* overlay work (16 bytes) */
typedef struct tagJILL_WORK {
    UCHAR Routine;                     /* 0x0 index into Jill_proc_tbl */
    UCHAR pad1[7];
    UCHAR *pHeap;                      /* 0x8 bump pointer for the enemy works */
    UCHAR UnkC;                        /* 0xC */
    UCHAR Open_num;                    /* 0xD epilogue files unlocked now */
    UCHAR Flg;                         /* 0xE bit 0: first visit (G.Unk77F8 bit 10 was clear) */
    UCHAR Vloop_bak;                   /* 0xF Main.Vloop on entry */
} JILL_WORK;

/* overlay .data (still in asm) */
extern UCHAR Jill_mess[] __asm__("D_801C3220"); /* message data */
extern USHORT Jill_mess_ofs[10] __asm__("D_801C3284"); /* offset of each message in Jill_mess */
extern USHORT Jill_pl_cd[] __asm__("D_801C3298"); /* model file, index Id - 2 */
extern USHORT Jill_wep_cd[] __asm__("D_801C32A4"); /* weapon file base, + Type */
extern LONG Jill_view_eye[4] __asm__("D_801C32B0");
extern LONG Jill_view_ref[] __asm__("D_801C32C0");
extern SEL_SPRT_DEF Jill_sprt_def[18] __asm__("D_801C336C");
extern void (*Jill_proc_tbl[3])(JILL_WORK *pW) __asm__("D_801C3468");
extern JILL_WORK Jill_wk __asm__("D_801C3480");
extern SEL_OBJ Jill_obj[] __asm__("D_801C3490");
extern const char Jill_str[8];                                  /* 0x801C2048 (.rodata, defined in jill_sel.c) */

void func_801C2050(void); /* main task */
void func_801C20A8(JILL_WORK *pW); /* draw the sprites */
void func_801C2118(void); /* move the models (unused) */
void func_801C2170(ENEMY_WORK *pEm); /* model idle animation */
void func_801C21C8(JILL_WORK *pW); /* init */
void func_801C2780(JILL_WORK *pW); /* wait for a button */
void func_801C27C0(JILL_WORK *pW); /* fade out and exit */
UCHAR *func_801C2858(ENEMY_WORK *pEm, ULONG TpClut, UCHAR *pWork); /* = Select_pl_load */
void func_801C29E0(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Be_flg, LONG *pPos);
UCHAR *func_801C2A68(ENEMY_WORK *pEm, ULONG *pWork, LONG No);
void func_801C2D3C(ULONG No, VECTOR *pVec, ULONG Rgb);
void func_801C2DF4(void); /* background load */
void func_801C2E54(SEL_OBJ *pObj, SEL_SPRT_DEF *pDef, ULONG TpClut, ULONG Rgb);
void func_801C2FC4(SEL_OBJ *pObj, LONG Ot);
void func_801C31F8(SEL_OBJ *pObj, ULONG Rgb);

#endif /* OVL_JILL_SEL_H */
