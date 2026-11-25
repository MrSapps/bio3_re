#ifndef OVL_JILL_OBJ_H
#define OVL_JILL_OBJ_H
/* 2D sprite objects of the select, jill_sel and result overlays (Jill_obj_*, src/ovl/common/jill_sel) */
#include "bio3.h"

/* one draw buffer of a sprite: SPRT with the colour/code as one word */
typedef struct tagSEL_SPRT {
    ULONG Tag;                         /* 0x00 */
    ULONG Rgb;                         /* 0x04 r0 g0 b0 code */
    SHORT X;                           /* 0x08 */
    SHORT Y;                           /* 0x0A */
    UCHAR U;                           /* 0x0C */
    UCHAR V;                           /* 0x0D */
    USHORT Clut;                       /* 0x0E */
    SHORT W;                           /* 0x10 */
    SHORT H;                           /* 0x12 */
} SEL_SPRT;

/* sprite object (0x50 bytes; Select_obj, Jill_obj, Result_obj) */
typedef struct tagSEL_OBJ {
    UCHAR Flg;                         /* 0x00 SEL_OBJ_* */
    UCHAR Timer;                       /* 0x01 blink timer */
    UCHAR Ofs_x;                       /* 0x02 offset from the parent's right edge */
    UCHAR Ofs_y;                       /* 0x03 offset from the parent's y */
    UCHAR Mode;                        /* 0x04 0 fade in, 1 fade out, 2 shown, 3 hidden */
    UCHAR pad5[3];
    SHORT X;                           /* 0x08 */
    SHORT Y;                           /* 0x0A */
    SEL_SPRT Sp[2];                    /* 0x0C */
    DR_MODE Dr[2];                     /* 0x34 */
    struct tagSEL_OBJ *pOya;           /* 0x4C parent sprite */
} SEL_OBJ;

/* sprite definition (14 bytes) */
typedef struct tagSEL_SPRT_DEF {
    SHORT X;                           /* 0x0 */
    SHORT Y;                           /* 0x2 */
    SHORT W;                           /* 0x4 */
    SHORT H;                           /* 0x6 */
    UCHAR U;                           /* 0x8 */
    UCHAR V;                           /* 0x9 */
    UCHAR Semi;                        /* 0xA SetSemiTrans */
    UCHAR Abr;                         /* 0xB semi-transparency rate (GetTPage abr) */
    UCHAR Ofs_x;                       /* 0xC */
    UCHAR Ofs_y;                       /* 0xD */
} SEL_SPRT_DEF;

/* SEL_OBJ.Flg */
#define SEL_OBJ_ON    0x01             /* drawn */
#define SEL_OBJ_BLINK 0x02             /* blink (with SEL_OBJ_REQ) */
#define SEL_OBJ_PULSE 0x04             /* pulse the colour (with SEL_OBJ_REQ) */
#define SEL_OBJ_REQ   0x08             /* apply BLINK/PULSE on the next draw */

void Jill_obj_init(SEL_OBJ *pObj, SEL_SPRT_DEF *pDef, ULONG TpClut, ULONG Rgb);
void Jill_obj_draw(SEL_OBJ *pObj, LONG Ot);
void Jill_obj_rgb(SEL_OBJ *pObj, ULONG Rgb);

#endif /* OVL_JILL_OBJ_H */
