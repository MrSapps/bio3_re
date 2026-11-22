#ifndef OVL_GEARBOX_H
#define OVL_GEARBOX_H
/*
 * GEARBOX.BIN overlay (loaded at 0x801C2000): the clock tower gear puzzle.
 * Five gear slots hold small (1) and large (2) gears; the cursor picks a slot and
 * moves its gear to a free adjacent slot (a limited number of moves), cursor
 * position 5 is the lever that turns the gears. The result is written to
 * G.Sce_work[4] (1..5: matching solution, 0: wrong).
 */
#include "bio3.h"
#include "main.h"
#include "mojidisp.h"
#include "monitor.h"
#include "om.h"
#include "pl_wep.h"
#include "sub.h"

#define GEAR_SLOT_NUM 5
#define GEAR_LEVER    5                /* cursor position of the lever */

/* overlay work (0x74 bytes) */
typedef struct tagGEAR_WORK {
    UCHAR Exit;                        /* 0x00 lever pulled */
    UCHAR Cursor;                      /* 0x01 slot 0..4, GEAR_LEVER */
    UCHAR Move_cnt;                    /* 0x02 moves left (printed) */
    UCHAR Blink;                       /* 0x03 cursor blink counter */
    UCHAR Slot[GEAR_SLOT_NUM];         /* 0x04 gear in each slot: 0 none, 1 small, 2 large */
    UCHAR Om_s[2];                     /* 0x09 object models of the two small gears */
    UCHAR Om_l[2];                     /* 0x0B object models of the two large gears */
    UCHAR Om_s_turn[2];                /* 0x0D turning small gears */
    UCHAR Om_l_turn[2];                /* 0x0F turning large gears */
    UCHAR pad11[3];
    LINE_F3 Line[2][2];                /* 0x14 cursor box, per draw buffer */
} GEAR_WORK;

#define OM(n) (&G.Ob_model[n])

/* overlay .data (still in asm) */
extern signed char Gear_dir_tbl[6][4]; /* next cursor per pad direction (up, down, right, left) */
extern UCHAR Gear_cursor_tbl[6][3]; /* cursor box x, y, half size */
extern signed char Gear_link_tbl[5][5]; /* slots adjacent to each slot, -1 terminated */
extern SHORT Gear_pos_tbl[5][3]; /* gear position of each slot */
extern UCHAR Gear_answer_tbl[5][5]; /* the five accepted gear layouts */
extern GEAR_WORK Gear_wk;

void Gear_main(void); /* main task */
void Gear_init(void); /* init */
void Gear_line_init(void); /* cursor line init (unused) */
void Gear_exit(void); /* exit */
void Gear_ctrl(void); /* pad control */
void Gear_cursor_move(void); /* cursor move */
void Gear_om_set(void); /* place the gear models */
void Gear_cursor_draw(void); /* draw the cursor */
void Gear_count_print(void); /* print the moves left */
void Gear_check(void); /* check the layout */
void Gear_turn(void); /* turn the gears */

#endif /* OVL_GEARBOX_H */
