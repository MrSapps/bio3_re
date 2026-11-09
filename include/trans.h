#ifndef TRANS_H
#define TRANS_H
#include "bio3.h"

/* Polygon transform buffers (RE2 struct TRANS, TRANS.C): two frames of POLY_FT4
 * quads (double-buffered by Cbuf_id) and the next-free pointer. */
typedef struct tagTRANS {
    POLY_FT4 Prim_poly_ft4[2][384];    /* 0x0000 */
    POLY_FT4 *pPoly_ft4_keep;          /* 0x7800 */
} TRANS;
extern TRANS Trans;                    /* 0x800BF830 */

void Trans_work_init(void);              /* Trans_work_init */
void Trans_pointer_set(void);              /* Trans_pointer_set */

#endif /* TRANS_H */
