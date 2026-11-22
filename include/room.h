#ifndef ROOM_H
#define ROOM_H
/* RE2 ROOM.C: room/stage loading (Set_room, Set_stage, Em_init_move) */
#include "bio3.h"

extern USHORT *D_8009DFD0[]; /* per stage room file numbers */
extern UCHAR D_800E017F; /* room sound pack id */

void Set_room(void);
void Set_stage(void);
void func_800498F0(void);
void Em_init_move(void);

#endif /* ROOM_H */
