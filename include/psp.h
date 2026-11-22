#ifndef PSP_H
#define PSP_H
/* RE2 PSP.C: room sprites (Psp_set, Psp_trans) and the RE3-only rain effect (0x80048974..) */
#include "bio3.h"

typedef struct tagPSP_POS {
    LONG x;
    LONG y;
    LONG z;
} PSP_POS;

/* rain drop primitive: draw mode word + LINE_G2 */
typedef struct {
    ULONG Tag;                         /* 0x00 */
    ULONG Tpage;                       /* 0x04 */
    ULONG pad8;
    ULONG Code;                        /* 0x0C rgb0 + code */
    ULONG Xy0;                         /* 0x10 */
    ULONG Rgb1;                        /* 0x14 */
    ULONG Xy1;                         /* 0x18 */
} PRIM_1C;
typedef struct {
    ULONG Tag;
    ULONG Tpage;
    ULONG Code;
    ULONG Xy;
    ULONG Uv;
    ULONG Wh;
} PSP_PRIM;

typedef struct {
    UCHAR *p0;                         /* 0x00 */
    struct tagPSP_POS *pPos;           /* 0x04 [100] drop positions (func_80048FF4) */
    PRIM_1C *pPrim;                    /* 0x08 */
    SHORT Cx;                          /* 0x0C centre of the camera view (func_80048FF4) */
    SHORT Cz;                          /* 0x0E */
    USHORT R;                          /* 0x10 radius */
    USHORT Unk12;                      /* 0x12 */
    USHORT Num;                        /* 0x14 drops in use */
    SHORT Unk16;                       /* 0x16 */
    SHORT Unk18;                       /* 0x18 */
    SHORT Unk1A;                       /* 0x1A */
    SHORT Dx;                          /* 0x1C drop direction x */
    SHORT Dz;                          /* 0x1E drop direction z */
    UCHAR Unk20;                       /* 0x20 */
    UCHAR Unk21;                       /* 0x21 shake routine (func_80049218) */
    UCHAR Unk22;                       /* 0x22 frames of the current step */
    UCHAR Unk23;                       /* 0x23 frame counter */
    SHORT Unk24;                       /* 0x24 start G.Unk75E4 */
    SHORT Unk26;                       /* 0x26 start G.Unk75E6 */
    SHORT Unk28;                       /* 0x28 delta x */
    SHORT Unk2A;                       /* 0x2A delta y */
} W_DEB18;

extern LONG D_800D204C[];
extern UCHAR *D_800DEB08; /* per-sprite group id (Cut_flg bit) */
extern SHORT *D_800DEB0C; /* per-sprite OT z */
extern PSP_PRIM *D_800DEB10; /* two prims per sprite (double buffer) */
extern W_DEB18 D_800DEB18;

void func_80048630(void);
void func_800486C8(void);
void Psp_set(void);
void Psp_trans(void);
void func_80048974(SHORT Arg);
void func_80048A3C(void);
void func_80048E34(void);
void func_80048FF4(void);
void func_80049218(W_DEB18 *pW);

#endif /* PSP_H */
