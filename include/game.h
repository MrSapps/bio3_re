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

#endif /* GAME_H */
