#ifndef DOOR_H
#define DOOR_H
/* RE2 DOOR.C: door opening sequence (Door_main / Door_init / Door_move / ...) */
#include "bio3.h"

typedef void (*DOOR_PROC)(void);
extern DOOR_PROC Door_proc_tbl[];      /* 0x800979F0, NULL terminated (RE2 pDoor_proc_tbl) */

void Door_main(void);

#endif /* DOOR_H */
