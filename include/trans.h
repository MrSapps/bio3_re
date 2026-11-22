#ifndef TRANS_H
#define TRANS_H
#include "bio3.h"
#include "esp.h"

/* Polygon transform buffers (RE2 struct TRANS, TRANS.C): two frames of POLY_FT4
 * quads (double-buffered by Cbuf_id) and the next-free pointer. */
typedef struct tagTRANS {
    POLY_FT4 Prim_poly_ft4[2][384];    /* 0x0000 */
    POLY_FT4 *pPoly_ft4_keep;          /* 0x7800 */
} TRANS;
extern TRANS Trans;                    /* 0x800BF830 */

void Trans_work_init(void); /* Trans_work_init */
void Trans_pointer_set(void); /* Trans_pointer_set */

void Eff_sprt_trans(void);
void Esp_trans_main(POLY_FT4 *pP, ESP_WORK *pEff, ULONG Def_size, ULONG nSpr, LONG Proj, ULONG *pOt, ULONG H_clarity, ULONG pG_offset);
void Esp_trans_3d(POLY_FT4 *pP, ESP_WORK *pEff, ULONG Def_size, ULONG nSpr, LONG Proj, ULONG *pOt, ULONG H_clarity, ULONG pG_offset);

#endif /* TRANS_H */
