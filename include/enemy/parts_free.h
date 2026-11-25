#ifndef ENEMY_PARTS_FREE_H
#define ENEMY_PARTS_FREE_H
/* More views of the PARTS_W free work (RE2 PARTS_W.Free[], RE3 at PARTS_W + 0x74) used by the
   enemy overlays; joint.h has PW_NECK (neck tracking) and PW_RGB (colour fade). */
#include "bio3.h"

/* a part flying off the body (PARTS_W.UnkA2 bit 0x20, moved by joint.c func_8002783C): it
   falls with Spd_y += Gravity, slides along Dir_y, and is removed after 0x1D frames */
typedef struct tagPARTS_FLY_FREE_WORK {
    SHORT Dir_y;                       /* 0x74 direction of the horizontal motion */
    SHORT Spd_y;                       /* 0x76 vertical speed */
    SHORT Spd_xz;                      /* 0x78 horizontal speed (stops when it turns negative) */
    SHORT Gravity;                     /* 0x7A added to Spd_y every frame */
    SHORT Cnt;                         /* 0x7C frames flown */
    SHORT pad7E;                       /* 0x7E */
    SHORT Spd_xz_add;                  /* 0x80 added to Spd_xz every frame */
} PARTS_FLY_FREE_WORK;
#define PW_FLY(p) ((PARTS_FLY_FREE_WORK *)&(p)->Unk74)

#endif /* ENEMY_PARTS_FREE_H */
