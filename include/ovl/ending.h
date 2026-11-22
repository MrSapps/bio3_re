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

void func_80194004(void);
void func_80194080(void);
void func_8019409C(void);
void func_801942DC(void);
void func_801943A4(void);
void func_801943E4(void);

#endif /* OVL_ENDING_H */
