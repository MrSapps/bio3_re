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
extern UCHAR Jill_mess[]; /* message data */
extern USHORT Jill_mess_ofs[10]; /* offset of each message in Jill_mess */
extern USHORT Jill_pl_cd[]; /* model file, index Id - 2 */
extern USHORT Jill_wep_cd[]; /* weapon file base, + Type */
extern LONG Jill_view_eye[4];
extern LONG Jill_view_ref[];
extern SEL_SPRT_DEF Jill_sprt_def[18];
extern void (*Jill_proc_tbl[3])(JILL_WORK *pW);
extern JILL_WORK Jill_wk;
extern SEL_OBJ Jill_obj[];
extern const char Jill_str[8];                                  /* 0x801C2048 (.rodata, defined in jill_sel.c) */

void Jill_main(void); /* main task */
void Jill_sprt_draw(JILL_WORK *pW); /* draw the sprites */
void Jill_pl_move(void); /* move the models (unused) */
void Jill_pl_idle(ENEMY_WORK *pEm); /* model idle animation */
void Jill_init(JILL_WORK *pW); /* init */
void Jill_wait(JILL_WORK *pW); /* wait for a button */
void Jill_exit(JILL_WORK *pW); /* fade out and exit */
UCHAR *Jill_pl_load(ENEMY_WORK *pEm, ULONG TpClut, UCHAR *pWork); /* = Select_pl_load */
void Jill_pl_trans(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Be_flg, LONG *pPos);
UCHAR *Jill_wep_load(ENEMY_WORK *pEm, ULONG *pWork, LONG No);
void Jill_light_set(ULONG No, VECTOR *pVec, ULONG Rgb);
void Jill_bg_load(void); /* background load */
void Jill_obj_init(SEL_OBJ *pObj, SEL_SPRT_DEF *pDef, ULONG TpClut, ULONG Rgb);
void Jill_obj_draw(SEL_OBJ *pObj, LONG Ot);
void Jill_obj_rgb(SEL_OBJ *pObj, ULONG Rgb);

#endif /* OVL_JILL_SEL_H */
