#ifndef OVL_ENDING_H
#define OVL_ENDING_H
/*
 * ENDING.BIN (loaded at 0x80194000): ending task. Plays the ending movie, then
 * runs the RESULT overlay screens (G.Unk2115 = result routine) and reloads the
 * game state. Much smaller than RE2 ENDING.C, which drew the ending itself.
 */
#include "bio3.h"
#include "main.h"
#include "monitor.h"
#include "mojidisp.h"
#include "movie.h"
#include "sub.h"

extern UCHAR D_800DBB58;               /* status flags (0x80 tested by the status screen code) */

/* RE3 passes an overlay task number instead of a function */
void Task_lexecute_no(ULONG Level, ULONG Task_no) __asm__("Task_lexecute");

#endif /* OVL_ENDING_H */
