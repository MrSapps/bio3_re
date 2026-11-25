#ifndef GAME_H
#define GAME_H
/* RE2 GAME.C: game loop helpers (Gun_light_set/reset, door transitions, screen flash) */
#include "bio3.h"

/* live selection event state (Live_select; started by the Sel_evt_on SCE opcode) */
typedef struct tagLIVE_SEL {
    UCHAR Negate;                      /* 0x0 screen shown in negative (Swap_Cbuff -> Scrn_negate) */
    UCHAR Wait;                        /* 0x1 set while the choice is open */
    UCHAR Mess_no;                     /* 0x2 message of the choice (also the symbol D_800C703E) */
    UCHAR Mess_y;                      /* 0x3 y of the choice text (also D_800C703F) */
    UCHAR Step;                        /* 0x4 flash count: cursor colour / blink speed (Mess_disp) */
} LIVE_SEL;
extern LIVE_SEL Live_sel;

extern ULONG D_80098700;
extern UCHAR Ctrl_mode;
extern UCHAR D_800E0186[]; /* an array: stores to it stay after loads of struct fields */

void Game_loop(void);
void Gun_light_set(void); /* Gun_light_set */
void Gun_light_reset(void); /* Gun_light_reset */
void Game_init(void);
void Set_door(void);
void Game_save_load(void);
void Game_dummy(void);
void Chaser_data_save(void);
void Live_select(void);
ULONG Live_select_flash(void);
void Flash_fade_in(void);
void Flash_fade_out(void);

#endif /* GAME_H */
