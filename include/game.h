#ifndef GAME_H
#define GAME_H
/* RE2 GAME.C: game loop helpers (Gun_light_set/reset, door transitions, screen flash) */
#include "bio3.h"

/* screen flash / message state (func_80024F74) */
typedef struct tagFLASH_WORK {
    UCHAR Flg;                         /* 0x0 */
    UCHAR Unk1;                        /* 0x1 */
    UCHAR Mess_no;                     /* 0x2 message shown by func_80024CE8 */
    UCHAR Unk3;                        /* 0x3 */
    UCHAR Cnt;                         /* 0x4 */
} FLASH_WORK;
extern FLASH_WORK D_800C703C;

extern ULONG D_80098700;
extern UCHAR Ctrl_mode;
extern UCHAR D_800E0186[]; /* an array: stores to it stay after loads of struct fields */

void Game_loop(void);
void Gun_light_set(void); /* Gun_light_set */
void Gun_light_reset(void); /* Gun_light_reset */
void func_800245A0(void);
void Set_door(void);
void func_80024B34(void);
void func_80024C50(void);
void func_80024C58(void);
void func_80024CE8(void);
ULONG func_80024F74(void);
void Flash_fade_in(void);
void Flash_fade_out(void);

#endif /* GAME_H */
