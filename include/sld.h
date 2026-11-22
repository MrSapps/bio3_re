#ifndef SLD_H
#define SLD_H
/* SLD.S (0x80010000): hand-written asm helpers linked first (RE2 SLD.OBJ): copy/clear, random, point tests */
#include "bio3.h"
#include "main.h"

void func_80010000(ULONG a0, ULONG *pDst);
void Memcpy(void *pDst, void *pSrc, ULONG Size); /* RE2 Memcpy role; RE3 copies 32-byte blocks, then bytes */
void func_8001012C(void *pDst, void *pSrc, ULONG Size); /* sld.s byte copy */
void Memclr(void *p, ULONG Size); /* RE2 Memclr role; RE3 takes a byte count */
void func_800101AC(void);
LONG func_800101C8(LONG *pPos, SHORT *pXz); /* sld.s: point in zone */
ULONG Hit_ck_point4(LONG *p, VCUT *cp); /* RE2 MAIN.C Hit_ck_point4, moved to SLD.S */
ULONG Rnd(void); /* RE2 EM_SUB.C Rnd (8-bit result), moved to SLD.S */

#endif /* SLD_H */
