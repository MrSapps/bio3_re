#ifndef ROOM_H
#define ROOM_H
/* RE2 ROOM.C: room/stage loading (Set_room, Set_stage, Em_init_move) */
#include "bio3.h"

extern USHORT *D_8009DFD0[]; /* per stage room file numbers */
extern UCHAR Rbj_id; /* room pack entry id of the RBJ (Sce_rbj_set, Cd_edd_search) */

void Set_room(void);
void Set_stage(void);
void func_800498F0(void);
void Em_init_move(void);

#endif /* ROOM_H */
