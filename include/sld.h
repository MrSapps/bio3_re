#ifndef SLD_H
#define SLD_H
/* SLD.S (0x80010000): hand-written asm helpers linked first (RE2 SLD.OBJ): copy/clear, random, point tests */
#include "bio3.h"
#include "main.h"

void func_80010000(ULONG a0, ULONG *pDst);
void func_800100A4(void *pDst, void *pSrc, ULONG Size); /* RE2 Memcpy */
void func_8001012C(void *pDst, void *pSrc, ULONG Size); /* sld.s byte copy */
void func_80010150(void *p, ULONG Size); /* RE2 Memclr */
void func_800101AC(void);
LONG func_800101C8(LONG *pPos, SHORT *pXz); /* sld.s: point in zone */
ULONG func_8001020C(LONG *p, VCUT *cp); /* Hit_ck_point4 */
ULONG func_800102E8(void); /* sld.s: RE2 Rnd */

#endif /* SLD_H */
