#ifndef OVL_MUSICBOX_H
#define OVL_MUSICBOX_H
/*
 * MUSICBOX.BIN overlay (loaded at 0x801C2000): the music box puzzle. Six
 * cylinders (object models 11..16) are each in one of two positions (flags
 * G.Unk7888 bits 6..11); the box plays the reference tune (Unk7 selects which),
 * then the player's tune. Matching it sets G.Unk7888 bit 13.
 */
#include "bio3.h"
#include "main.h"
#include "monitor.h"
#include "om.h"
#include "pad.h"
#include "pl_wep.h"
#include "sub.h"

#define MBOX_NOTE_NUM 6

/* overlay work (module .data) */
typedef struct tagMBOX_WORK {
    UCHAR Routine;                     /* 0x00 */
    UCHAR Mess;                        /* 0x01 message index; routine 0: tune started */
    UCHAR Unk2;                        /* 0x02 */
    UCHAR Note;                        /* 0x03 current cylinder / note */
    UCHAR Run;                         /* 0x04 cleared to end the task */
    UCHAR Cut_old;                     /* 0x05 camera cut before the close-up */
    UCHAR Match;                       /* 0x06 bit n: cylinder n agrees with the tune */
    UCHAR Tune;                        /* 0x07 reference tune (0 or 1) */
    SHORT Timer;                       /* 0x08 */
    UCHAR Note_se;                     /* 0x0A sound of the current note */
    UCHAR padB;
} MBOX_WORK;

#define OM(n) (pG->Ob_model[n])

/* overlay .data (still in asm) */
extern UCHAR Mbox_mess[] __asm__("D_801C2A44");         /* message texts */
extern USHORT Mbox_mess_ofs[] __asm__("D_801C2B50");    /* offset of each text in Mbox_mess */
extern LONG Mbox_pos[MBOX_NOTE_NUM * 4] __asm__("D_801C2B60"); /* per cylinder: x, z when its flag is set, x, z when clear */
extern MBOX_WORK Mbox_wk __asm__("D_801C2BC0");

/* exe functions/data without a shared prototype */
LONG Mess_set_p(ULONG Pos_xy, USHORT Attr, UCHAR *pMess, ULONG Stop_data) __asm__("Mess_set"); /* mojidisp.c */
void Flg_off(ULONG *pFlg, ULONG Id);   /* sub.c */
void Psp_trans(void);                  /* psp.c */
extern UCHAR D_800DBB58;               /* status flags (0x80 tested by the status screen code) */

void func_801C2030(void);              /* main task */
void func_801C2228(void);              /* puzzle state machine */

#endif /* OVL_MUSICBOX_H */
