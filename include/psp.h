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
} RAIN_PRIM;
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
    struct tagPSP_POS *pPos;           /* 0x04 [100] drop positions (Rain_drop_set) */
    RAIN_PRIM *pPrim;                    /* 0x08 */
    SHORT Cx;                          /* 0x0C centre of the camera view (Rain_drop_set) */
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
    UCHAR Unk21;                       /* 0x21 shake routine (Rain_wind_move) */
    UCHAR Unk22;                       /* 0x22 frames of the current step */
    UCHAR Unk23;                       /* 0x23 frame counter */
    SHORT Unk24;                       /* 0x24 start G.Wind_dir */
    SHORT Unk26;                       /* 0x26 start G.Wind_pow */
    SHORT Unk28;                       /* 0x28 delta x */
    SHORT Unk2A;                       /* 0x2A delta y */
} RAIN_WORK;

extern LONG Pri_be_flg[];
extern UCHAR *Psp_id; /* per-sprite group id (Cut_flg bit) */
extern SHORT *Psp_otz; /* per-sprite OT z */
extern PSP_PRIM *pPsp_prim; /* two prims per sprite (double buffer) */
extern RAIN_WORK Rain_work;

void Psp_init(void);
void Pri_be_flg_init(void);
void Psp_set(void);
void Psp_trans(void);
void Rain_init(SHORT Arg);
void Rain_move(void);
void Rain_trans(void);
void Rain_drop_set(void);
void Rain_wind_move(RAIN_WORK *pW);

#endif /* PSP_H */
