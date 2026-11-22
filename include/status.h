#ifndef STATUS_H
#define STATUS_H
/* RE2 STATUS.C / ST_SUB*.C: status screen, inventory, item box, files and map (RE3 0x800636D4..) */
#include "bio3.h"

/* panel position */
typedef struct tagST_POS {
    SHORT X;                           /* 0x0 */
    SHORT Y;                           /* 0x2 */
} ST_POS;

/* status work (RE2 struct tagSTATUS role, RE3 layout; St at 0x800E01C0) */
typedef struct tagST_WORK {
    UCHAR pad0[4];
    UCHAR St_type;                     /* 0x04 RE2 St_type: screen (0/1 status, 2 item box, 3 file, 4/5 map) */
    UCHAR pad5;
    UCHAR Unk6;                        /* 0x06 set when the drawer item is shown (sce_at Hikidashi_entrance) */
    UCHAR pad7;
    ULONG *pBuf;                       /* 0x08 load buffer (D_800E01C8) */
    UCHAR padC[0x10 - 0xC];
    UCHAR Rtn0;                        /* 0x10 */
    UCHAR Rtn1;                        /* 0x11 */
    UCHAR Rtn2;                        /* 0x12 */
    UCHAR Unk13;                       /* 0x13 frame counter of the current step */
    ULONG Flg14;                       /* 0x14 */
    ULONG Flg;                         /* 0x18 */
    signed char Unk1C;                 /* 0x1C inventory cursor */
    UCHAR Unk1D;                       /* 0x1D */
    UCHAR Unk1E;                       /* 0x1E */
    signed char Unk1F;                 /* 0x1F item box cursor (0..63) */
    signed char Unk20;                 /* 0x20 */
    UCHAR pad21;
    SHORT Unk22;                       /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    UCHAR pad26;
    UCHAR Unk27;                       /* 0x27 task running */
    UCHAR Unk28;                       /* 0x28 */
    UCHAR Unk29;                       /* 0x29 */
    USHORT Unk2A;                      /* 0x2A */
    UCHAR pad2C[0x2E - 0x2C];
    UCHAR Unk2E;                       /* 0x2E */
    UCHAR Unk2F;                       /* 0x2F */
    UCHAR Unk30;                       /* 0x30 */
    UCHAR pad31;
    SHORT Unk32;                       /* 0x32 */
    UCHAR Unk34;                       /* 0x34 condition (func_8006E598) */
    UCHAR Unk35;                       /* 0x35 life to restore */
    UCHAR Unk36;                       /* 0x36 cures poison */
    UCHAR pad37[0x3A - 0x37];
    UCHAR Unk3A;                       /* 0x3A */
    UCHAR Unk3B;                       /* 0x3B */
    UCHAR Unk3C;                       /* 0x3C */
    UCHAR Unk3D;                       /* 0x3D */
    UCHAR Unk3E;                       /* 0x3E */
    UCHAR pad3F[0x42 - 0x3F];
    ITEM_WORK Unk42;                     /* 0x42 copy of the selected item entry */
    UCHAR pad46[0x48 - 0x46];
    ITEM_WORK *Unk48;                    /* 0x48 combine: item A */
    ITEM_WORK *Unk4C;                    /* 0x4C combine: item B */
    UCHAR *Unk50;                      /* 0x50 D_800A0514 entry of Unk48 */
    UCHAR *Unk54;                      /* 0x54 D_800A0514 entry of Unk4C */
    UCHAR *Unk58;                      /* 0x58 combine table record (8 bytes, D_800A07C4) */
    UCHAR *Unk5C;                      /* 0x5C combine: slot no of item A */
    UCHAR *Unk60;                      /* 0x60 combine: slot no of item B */
    UCHAR Unk64;                       /* 0x64 combine result kind */
    UCHAR Unk65;                       /* 0x65 combine result count */
    SHORT Unk66;                       /* 0x66 */
    SHORT Unk68;                       /* 0x68 */
    UCHAR Unk6A;                       /* 0x6A weapon of a combine (D_800A00EC) */
    UCHAR pad6B[0x6E - 0x6B];
    UCHAR Unk6E;                       /* 0x6E */
    UCHAR Unk6F;                       /* 0x6F */
    UCHAR Unk70;                       /* 0x70 */
    signed char Unk71;                 /* 0x71 */
    signed char Unk72;                 /* 0x72 */
    UCHAR Unk73;                       /* 0x73 */
    UCHAR pad74[0x76 - 0x74];
    UCHAR Unk76;                       /* 0x76 */
    UCHAR Unk77;                       /* 0x77 */
    UCHAR pad78[0x7C - 0x78];
    LONG Unk7C;                        /* 0x7C */
    LONG Unk80;                        /* 0x80 */
    LONG Unk84;                        /* 0x84 */
    LONG Unk88;                        /* 0x88 */
    LONG Unk8C;                        /* 0x8C */
    LONG Unk90;                        /* 0x90 */
    LONG Unk94;                        /* 0x94 */
    LONG Unk98;                        /* 0x98 */
    LONG Unk9C;                        /* 0x9C */
    LONG UnkA0;                        /* 0xA0 */
    LONG UnkA4;                        /* 0xA4 */
    UCHAR UnkA8;                       /* 0xA8 */
    UCHAR UnkA9;                       /* 0xA9 */
    UCHAR UnkAA;                       /* 0xAA */
    UCHAR UnkAB;                       /* 0xAB */
    UCHAR UnkAC;                       /* 0xAC */
    UCHAR UnkAD;                       /* 0xAD */
    signed char UnkAE;                 /* 0xAE */
    signed char UnkAF;                 /* 0xAF */
    UCHAR UnkB0;                       /* 0xB0 */
    UCHAR UnkB1;                       /* 0xB1 */
    UCHAR padB2[0xB4 - 0xB2];
    ULONG *pUnkB4;                     /* 0xB4 map data: word offsets to ST_MAP_ROOM/ST_MAP_INFO tables and (room, cut, flag, kind) lists */
    USHORT UnkB8;                      /* 0xB8 */
    UCHAR UnkBA;                       /* 0xBA */
    signed char UnkBB;                 /* 0xBB */
    UCHAR UnkBC;                       /* 0xBC file number */
    UCHAR UnkBD;                       /* 0xBD page number */
    signed char UnkBE;                 /* 0xBE blink timer */
    UCHAR UnkBF;                       /* 0xBF */
    UCHAR UnkC0;                       /* 0xC0 */
    UCHAR UnkC1;                       /* 0xC1 */
    UCHAR padC2[0xC4 - 0xC2];
    SHORT UnkC4;                       /* 0xC4 */
    SHORT UnkC6;                       /* 0xC6 */
    SHORT UnkC8;                       /* 0xC8 */
    UCHAR UnkCA;                       /* 0xCA */
    UCHAR UnkCB;                       /* 0xCB */
    ULONG *UnkCC;                      /* 0xCC MAP.MAP data */
    UCHAR UnkD0;                       /* 0xD0 clut rows of the status textures */
    UCHAR UnkD1;                       /* 0xD1 */
    UCHAR UnkD2;                       /* 0xD2 */
    UCHAR UnkD3;                       /* 0xD3 */
    UCHAR UnkD4;                       /* 0xD4 */
    UCHAR UnkD5;                       /* 0xD5 */
    UCHAR UnkD6;                       /* 0xD6 */
    UCHAR UnkD7;                       /* 0xD7 */
    UCHAR UnkD8;                       /* 0xD8 */
    UCHAR padD9[0xE0 - 0xD9];
    UCHAR *UnkE0;                      /* 0xE0 */
    ST_POS Pos[32];                    /* 0xE4 panel positions, indexed by the prm>>8 byte of the prim setters */
    DR_TPAGE Ot[2][40];                /* 0x164 per-buffer ordering nodes, index prm>>16 of the prim setters */
    DR_TPAGE Tpage[2][2];              /* 0x3E4 */
    DR_TPAGE Tp404[4];                 /* 0x404 */
    DR_TPAGE Tp424[2];                 /* 0x424 */
    DR_TPAGE Tp434[2];                 /* 0x434 */
} ST_WORK;
extern ST_WORK St;                     /* 0x800E01C0 RE2 St */

#endif /* STATUS_H */
