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

void Ending_main(void);
void Ending_status_clr(void);
void Ending_game(void);
void Ending_movie(void);
void Ending_load(void);
void Ending_next_game(void);

#endif /* OVL_ENDING_H */
