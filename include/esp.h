#ifndef ESP_H
#define ESP_H
/*
 * RE2 ESP.C / ESP_R.C: effect sprites. RE3 keeps the 96-entry work pool (RE2 Esp[96])
 * but its work layout (0xD4 bytes) differs from RE2's ESP_WORK.
 */
#include "bio3.h"

/* sprite animation step (RE2 struct SSEQUENCE, 8 bytes) */
typedef struct tagSSEQUENCE {
    UCHAR Unk0;                        /* 0x0 next step (when Time is a jump code) */
    UCHAR nSpr;                        /* 0x1 RE2 nSpr */
    UCHAR Time;                        /* 0x2 RE2 Time: frames, copied to ESP_WORK.Timer */
    UCHAR Unk3;                        /* 0x3 */
    ULONG Unk4;                        /* 0x4 */
} SSEQUENCE;

/* sprite group entry (Esp_trans_main) */
typedef struct tagSPR_GP {
    UCHAR U;                           /* 0x0 */
    UCHAR V;                           /* 0x1 */
    signed char Ofsx;                  /* 0x2 */
    signed char Ofsy;                  /* 0x3 */
} SPR_GP;

/* effect work (RE2 ESP_WORK role, RE3 layout, 0xD4 bytes; pool D_800BA8A8[96]) */
typedef struct tagESP_WORK {
    UCHAR Routine_0;                   /* 0x00 0: free */
    UCHAR Routine_1;                   /* 0x01 */
    UCHAR Routine_2;                   /* 0x02 */
    UCHAR Unk03;                       /* 0x03 */
    USHORT Attr;                       /* 0x04 */
    UCHAR pad06[0x8 - 0x6];
    SHORT Unk08;                       /* 0x08 sprite scale x (Esp_trans_main) */
    SHORT Unk0A;                       /* 0x0A sprite scale y */
    signed char Acc_x;                 /* 0x0C */
    signed char Acc_y;                 /* 0x0D */
    signed char Acc_z;                 /* 0x0E */
    UCHAR Unk0F;                       /* 0x0F */
    SHORT Spd_x;                       /* 0x10 */
    SHORT Spd_y;                       /* 0x12 */
    SHORT Spd_z;                       /* 0x14 */
    USHORT Unk16;                      /* 0x16 or'ed into Tpage (func_8001B484) */
    UCHAR Unk18;                       /* 0x18 */
    UCHAR Snd_no;                      /* 0x19 */
    UCHAR Unk1A;                       /* 0x1A */
    UCHAR Unk1B;                       /* 0x1B */
    USHORT Unk1C;                      /* 0x1C */
    USHORT Unk1E;                      /* 0x1E */
    USHORT Unk20;                      /* 0x20 */
    USHORT Unk22;                      /* 0x22 */
    USHORT Be_flg;                     /* 0x24 */
    USHORT Flg2;                       /* 0x26 */
    union {
        USHORT W;                      /* 0x28 effect id (func_8001C2DC) */
        UCHAR B[2];                    /* 0x28 [0] kind, [1] (esp_r) */
    } Id;
    UCHAR Unk2A;                       /* 0x2A */
    UCHAR Timer;                       /* 0x2B */
    USHORT Unk2C;                      /* 0x2C */
    USHORT Unk2E;                      /* 0x2E */
    SHORT Unk30;                       /* 0x30 offset added to Unk38 (Esp_trans_main) */
    SHORT Unk32;                       /* 0x32 */
    SHORT Unk34;                       /* 0x34 */
    UCHAR pad36[0x38 - 0x36];
    LONG Unk38[3];                     /* 0x38 base position */
    USHORT Clut;                       /* 0x44 (func_8001B484) */
    USHORT Tpage;                      /* 0x46 */
    VECTOR Pos;                        /* 0x48 Pos.pad: ground height (func_8001BFCC) */
    VECTOR Old;                        /* 0x58 previous position (Sca_ck_line, func_80021EAC) */
    SHORT Rot[3];                      /* 0x68 rotation (Esp_trans_main) */
    UCHAR pad6E;
    UCHAR Seq_no;                      /* 0x6F index into pSeq */
    MATRIX Workm;                      /* 0x70 local matrix */
    MATRIX Workm2;                     /* 0x90 copy of Workm (func_8001BCFC) */
    UCHAR Col[3];                      /* 0xB0 prim colour (func_8001B484); UnkB3 is the prim code byte */
    UCHAR UnkB3;                       /* 0xB3 */
    SHORT UnkB4;                       /* 0xB4 */
    SHORT UnkB6;                       /* 0xB6 */
    SHORT UnkB8;                       /* 0xB8 */
    SHORT UnkBA;                       /* 0xBA */
    struct tagENEMY_WORK *pEm;         /* 0xBC enemy the effect belongs to (func_80020EB8) */
    MATRIX *pMat;                      /* 0xC0 */
    SSEQUENCE *pSeq;                   /* 0xC4 */
    SPR_GP *pSprg;                     /* 0xC8 sprite groups */
    ULONG *pUnkCC;                     /* 0xCC current move record */
    struct tagESP_WORK *pLink;         /* 0xD0 killed when the linked work dies */
} ESP_WORK;
extern ESP_WORK D_800BA8A8[96];        /* RE2 Esp[96] */

void func_8001B894(ULONG Id, MATRIX *pM);  /* kill the effects of Id attached to pM */

#endif /* ESP_H */
