#ifndef OVL_EPILOG_H
#define OVL_EPILOG_H
/*
 * EPILOG.BIN (loaded at 0x801C2000): epilogue gallery. A 3x3 menu of pages
 * (EPIS.TIM); one page per cleared game (G.Unk7806), the ninth slot exits.
 * A page shows its picture from the packed OMBG.TIM file.
 */
#include "bio3.h"
#include "cd.h"
#include "main.h"
#include "monitor.h"
#include "em_sub.h"
#include "pl_wep.h"
#include "snd.h"
#include "sub.h"

/* piece of the packed OMBG.TIM file (offset in sectors, size in bytes; as RESULT_BG) */
typedef struct tagEPI_FILE {
    ULONG Ofs;                         /* 0x0 */
    ULONG Size;                        /* 0x4 */
} EPI_FILE;

/* gallery work (0x1AC bytes, in .data) */
typedef struct tagEPI_WORK {
    UCHAR Rno0;                        /* 0x000 routine (Epilog_routine) */
    UCHAR Rno1;                        /* 0x001 */
    UCHAR Rno2;                        /* 0x002 */
    UCHAR Rno3;                        /* 0x003 */
    union {
        ULONG Flg[1];                  /* 0x004 Flg_on bits 0 run, 1 draw, 2 draw cursor (byte 7) */
        struct {
            UCHAR pad0;
            signed char Cursor;        /* 0x005 menu slot 0..8 */
            signed char Kido_dir;      /* 0x006 cursor blink direction */
        } s;
    } u;
    SHORT Kido;                        /* 0x008 cursor blink 0..0x3F */
    UCHAR padA[0xC - 0xA];
    SPRT Page[9][2];                   /* 0x00C menu pages, per buffer (only 8 used) */
    SPRT Cursor[2];                    /* 0x174 */
    DR_TPAGE Tpage[2];                 /* 0x19C */
} EPI_WORK;

/* overlay data (asm/data/ovl/epilog/data.data.s) */
extern EPI_FILE D_801C2B4C[];          /* epilogue pictures in OMBG.TIM */
extern UCHAR D_801C2B9C[];             /* their check sums */
extern USHORT D_801C2BA8[][2];         /* page positions on the 3x3 menu */
extern void (*Epilog_routine[])(void); /* routines, index Rno0 */
extern EPI_WORK Epi_work __asm__("Epilog_work");

void Epilog_menu(void);
void Epilog_show(void);
void Epilog_start(void);
void Epilog_exit(void);
void Epilog_draw(void);
void Epilog_pic_read(ULONG *pAdr, UCHAR No);

void Epilog_main(void);

#endif /* OVL_EPILOG_H */
