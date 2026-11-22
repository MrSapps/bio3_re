#ifndef OVL_OPENING_H
#define OVL_OPENING_H
/*
 * OPENING.BIN (loaded at 0x801C2000): the opening story pictures. A byte-code
 * script (at 0x8014B02A, in OPENING0_DAT) drives fades, waits, XA voice,
 * messages and two scrolling picture scenes.
 */
#include "bio3.h"
#include "cd.h"
#include "main.h"
#include "monitor.h"
#include "em_sub.h"

/* picture piece (0xC bytes) */
typedef struct tagOPN_SPR {
    USHORT W;                          /* 0x0 */
    USHORT H;                          /* 0x2 */
    UCHAR U;                           /* 0x4 */
    UCHAR pad5;
    UCHAR V;                           /* 0x6 */
    UCHAR pad7;
    USHORT Clut;                       /* 0x8 clut row - 0x1E0 */
    USHORT Tpage;                      /* 0xA low 4 bits x / 64, high bits y / 256 */
} OPN_SPR;

/* opening work (0x24C bytes, in .data) */
typedef struct tagOPN_WORK {
    UCHAR Rno0;                        /* 0x000 scene routine */
    UCHAR Rno1;                        /* 0x001 scene sub routine */
    UCHAR Rno2;                        /* 0x002 */
    UCHAR Rno3;                        /* 0x003 */
    UCHAR Scene;                       /* 0x004 0: scroll up, 1: two layer cross fade (script op 7) */
    UCHAR Semi;                        /* 0x005 layer drawn semi transparent (1 or 2) */
    UCHAR Kido;                        /* 0x006 brightness of layer 1 (layer 2: 0x80 - Kido) */
    UCHAR Ot0;                         /* 0x007 OT index of layer 1 */
    UCHAR Ot1;                         /* 0x008 OT index of layer 2 */
    UCHAR pad9;
    SHORT Cnt;                         /* 0x00A scene timer */
    UCHAR padC[0xE - 0xC];
    SHORT Ofs0;                        /* 0x00E scroll (y in scene 0, x of layer 1 in scene 1) */
    SHORT Ofs1;                        /* 0x010 x scroll of the layer 2 pieces 8/9 */
    UCHAR pad12[0x14 - 0x12];
    SPRT Sprt[2][10];                  /* 0x014 per buffer */
    DR_TPAGE Tpage[2][10];             /* 0x1A4 */
    UCHAR *pScript;                    /* 0x244 */
    USHORT Wait;                       /* 0x248 script wait counter */
} OPN_WORK;

typedef LONG (*OPN_OP)(OPN_WORK *pWk); /* script op: 0 next frame, 1 next op, 2 end */

/* overlay data (asm/data/ovl/opening/data.data.s) */
extern ULONG *D_801C2F0C[];            /* pictures in OPENING1_DAT (script op 6) */
extern ULONG *D_801C2F14[];            /* TIMs in OPENING0_DAT */
extern UCHAR D_801C2F3C[];             /* messages */
extern USHORT D_801C2F68[];            /* message offsets into D_801C2F3C */
extern OPN_OP Opening_op_tbl[];            /* script ops */
extern OPN_SPR D_801C2FA4[];           /* picture pieces */
extern USHORT D_801C301C[][2];         /* piece positions */
extern OPN_WORK Opn_work __asm__("Opening_work");

void Opening_init(void);
void Opening_load(void);
void Opening_sprt_init(void);
void Opening_exit(void);
void Opening_scene0_move(OPN_WORK *pWk);
void Opening_scene0_draw(OPN_WORK *pWk);
void Opening_scene1_move(OPN_WORK *pWk);
void Opening_scene1_draw(OPN_WORK *pWk);

/* exe prototypes not in a shared header yet (game .c files disagree on some of them) */

void Opening_main(void);
LONG Opening_op_next(OPN_WORK *pWk);
LONG Opening_op_frame(OPN_WORK *pWk);
LONG Opening_op_end(OPN_WORK *pWk);
LONG Opening_op_wait_set(OPN_WORK *pWk);
LONG Opening_op_wait(OPN_WORK *pWk);
LONG Opening_op_xa_wait(OPN_WORK *pWk);
LONG Opening_op_pic(OPN_WORK *pWk);
LONG Opening_op_scene(OPN_WORK *pWk);
LONG Opening_op_fade_in(OPN_WORK *pWk);
LONG Opening_op_fade_out(OPN_WORK *pWk);
LONG Opening_op_xa(OPN_WORK *pWk);
LONG Opening_op_mess(OPN_WORK *pWk);
LONG Opening_op_vloop(OPN_WORK *pWk);

#endif /* OVL_OPENING_H */
