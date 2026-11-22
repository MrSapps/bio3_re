#ifndef SPL_H
#define SPL_H
/* RE2 SPL.C: sub player (partner) main loop and routine table */
#include "bio3.h"
#include "em_sub.h"

/* the sub player's use of the enemy-only area ENEMY_WORK.Unk1CC[] (offsets are ENEMY_WORK's) */
typedef struct tagSPL_FREE {
    SHORT Unk1CC;                      /* 0x1CC target x (Spl_auto) */
    SHORT Unk1CE;                      /* 0x1CE target z */
    USHORT Unk1D0;                     /* 0x1D0 flags; & 3 target kind, 0x4000 (spl_sce), 0x8000 */
    USHORT Unk1D2;                     /* 0x1D2 distance to the target */
    SHORT Unk1D4;                      /* 0x1D4 */
    UCHAR pad1D6;
    UCHAR Unk1D7;                      /* 0x1D7 */
    SHORT Unk1D8;                      /* 0x1D8 */
    UCHAR pad1DA[0x1DE - 0x1DA];
    UCHAR Unk1DE;                      /* 0x1DE */
    UCHAR Unk1DF;                      /* 0x1DF */
    USHORT Unk1E0;                     /* 0x1E0 */
} SPL_FREE;
#define SPL_W(pEm) ((SPL_FREE *)(pEm)->Unk1CC)

extern USHORT D_8009E374[][4]; /* collision size by Id: [0] Unk26, [1] W0/D0 */
extern void (*D_8009E6EC[])(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

void Spl(ENEMY_WORK *pEm);
void Spl_Init(ENEMY_WORK *pEm);
void func_80059310(ENEMY_WORK *pEm);
void Spl_set(void);
void func_800594D4(ENEMY_WORK *pEm);

#endif /* SPL_H */
