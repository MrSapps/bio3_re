#ifndef STATUS_H
#define STATUS_H
/* RE2 STATUS.C / ST_SUB*.C: status screen, inventory, item box, files and map (RE3 0x800636D4..) */
#include "bio3.h"

typedef struct tagST_ICON {
    UCHAR U;                           /* 0x0 */
    UCHAR pad1;
    UCHAR V;                           /* 0x2 */
    UCHAR pad3;
    USHORT W;                          /* 0x4 */
    USHORT H;                          /* 0x6 */
    UCHAR pad8[4];
} ST_ICON;

typedef struct tagST_MAP_ARROW {
    SHORT U0;                          /* 0x0 */
    SHORT V0;                          /* 0x2 */
    SHORT U1;                          /* 0x4 */
    SHORT V1;                          /* 0x6 */
    SHORT X;                           /* 0x8 */
    SHORT Y;                           /* 0xA */
} ST_MAP_ARROW;

/* MAP.MAP (St.UnkCC) */
typedef struct tagST_MAP_PART {
    UCHAR U0;                          /* 0x0 */
    UCHAR V0;                          /* 0x1 */
    UCHAR U1;                          /* 0x2 */
    UCHAR V1;                          /* 0x3 */
    SHORT X;                           /* 0x4 */
    SHORT Y;                           /* 0x6 */
    UCHAR Clut;                        /* 0x8 */
    UCHAR Tpage;                       /* 0x9 */
    UCHAR padA[2];
} ST_MAP_PART;

typedef struct tagST_RGB_STEP {
    UCHAR R, G, B;                     /* 0x0 */
    UCHAR Dr, Dg, Db;                  /* 0x3 */
} ST_RGB_STEP;

typedef struct { UCHAR A0; UCHAR B0; UCHAR A1; UCHAR B1; } ST_ROOM_MAP;

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

extern char D_80010DCC[];
extern char D_80010F84[];
extern char D_80010FEC[];
extern UCHAR D_8009EED8[];
extern ULONG D_8009EF90[];
extern USHORT D_8009F26C[];
extern USHORT D_8009F2AC[];
extern UCHAR D_8009F2EC[];
extern UCHAR D_8009F2F8[];
extern UCHAR D_8009F304[];
extern UCHAR D_8009F310[];
extern UCHAR D_8009F31C[];
extern UCHAR D_8009F328[];
extern UCHAR D_8009F34C[];
extern USHORT D_8009F388[];
extern USHORT D_8009F3E8[];
extern UCHAR D_8009F3F4[];
extern UCHAR D_8009F418[];
extern USHORT D_8009F424[];
extern UCHAR D_8009F448[];
extern SHORT D_8009F49C[];
extern UCHAR D_8009F4A8[];
extern UCHAR D_8009F4B4[];
extern SHORT D_8009F4C0[];
extern UCHAR D_8009F4CC[];
extern UCHAR D_8009F4D8[];
extern void (*D_8009F4E4[])(ST_WORK *pSt);
extern USHORT D_8009F4F8[][2];
extern UCHAR D_8009F564[];
extern UCHAR D_8009F568[];
extern UCHAR D_8009F5F0[];
extern ULONG D_8009F678[];
extern UCHAR D_8009F890[];
extern UCHAR D_8009F8A8[];
extern UCHAR D_8009F8B4[];
extern UCHAR D_8009F8C0[];
extern UCHAR D_8009F8CC[];
extern UCHAR D_8009F908[];
extern UCHAR D_8009F95C[];
extern USHORT D_8009F9B0[];
extern UCHAR D_8009F9D4[];
extern ST_ICON D_8009F9F8[];
extern UCHAR D_8009FB78[];
extern UCHAR D_8009FB84[];
extern SHORT D_8009FB90[];
extern UCHAR D_8009FBA8[];
extern UCHAR D_8009FBC0[];
extern UCHAR D_8009FBCC[];
extern UCHAR D_8009FBFC[];
extern UCHAR D_8009FC2C[];
extern UCHAR D_8009FC38[];
extern UCHAR D_8009FC44[];
extern UCHAR D_8009FC50[];
extern USHORT D_8009FC5C[];
extern USHORT D_8009FCBC[];
extern USHORT D_8009FD1C[];
extern USHORT D_8009FD7C[];
extern USHORT D_8009FDDC[];
extern USHORT D_8009FE0C[];
extern USHORT D_8009FE3C[];
extern USHORT D_8009FE6C[];
extern USHORT D_8009FE9C[];
extern USHORT D_8009FECC[];
extern USHORT D_8009FEFC[];
extern USHORT D_8009FF2C[];
extern USHORT D_8009FF38[];
extern USHORT D_8009FF44[];
extern USHORT D_8009FF50[];
extern SHORT D_8009FF5C[];
extern UCHAR D_8009FF68[];
extern UCHAR D_8009FF74[];
extern UCHAR D_8009FF80[];
extern UCHAR D_8009FF8C[];
extern UCHAR D_8009FFB0[];
extern UCHAR D_8009FFBC[];
extern UCHAR D_8009FFC8[];
extern UCHAR D_8009FFD4[];
extern UCHAR D_8009FFE0[];
extern UCHAR D_8009FFEC[];
extern UCHAR D_8009FFF8[];
extern UCHAR D_800A0004[];
extern USHORT D_800A004C[][2];
extern USHORT D_800A0080[][2];
extern USHORT D_800A00B0[]; /* same table as D_800A00B0, byte access */
extern signed char D_800A00EC[][2];
extern void (*D_800A0100[])(ST_WORK *pSt);
extern void (*D_800A0138[])(ST_WORK *pSt);
extern ST_RGB_STEP D_800A0150[];
extern UCHAR *D_800A0174[];
extern ITEM_WORK D_800A018C[2][5];
extern ITEM_WORK D_800A01B4[2][6];
extern ITEM_WORK D_800A01E4[];
extern ITEM_WORK D_800A0200[];
extern ITEM_WORK D_800A021C[];
extern ITEM_WORK D_800A0238[];
extern ITEM_WORK D_800A0258[];
extern ITEM_WORK D_800A0278[2][4];
extern ITEM_WORK D_800A0298[2][11];
extern void (*D_800A02F0[])(ST_WORK *pSt);
extern UCHAR D_800A0328[];
extern USHORT D_800A038C[];
extern UCHAR D_800A03A0[];
extern ULONG D_800A03AC[];
extern ST_MAP_ARROW D_800A03D0[][4];
extern ST_ROOM_MAP D_800A0430[];
extern UCHAR D_800A04F8[];
extern void (*D_800A0500[])(ST_WORK *pSt);
extern UCHAR D_800A0514[][4];
extern UCHAR D_800A07C4[];
extern UCHAR D_800A0BB4[];
extern UCHAR D_800A0BC4[];
extern LONG D_800C79CC; /* extra game character (func_8006D0D8) */
extern ULONG D_800CC840;
extern SHORT D_800CCBBC;
extern UCHAR D_800CEA80[];
extern ULONG D_800D1FA0[];
extern ULONG D_800D20DC[];
extern ULONG D_800D2124[];
extern ULONG D_800D2128[];
extern UCHAR D_800D225E; /* G.Item_work[0].Num as its own symbol (reloaded every loop pass) */
extern ITEM_WORK *D_800D23B4;
extern LONG D_800D23B8;
extern UCHAR D_800D442D;
extern UCHAR D_800D442E;
extern UCHAR D_800DBB58; /* status flags (0x80 tested by the status screen code) */
extern UCHAR D_800DBB5B;

void func_800636D4(ST_WORK *pSt);
void func_80063850(ST_WORK *pSt);
void func_80063CAC(ST_WORK *pSt);
void func_80063E9C(ST_WORK *pSt);
void func_80063FE4(ST_WORK *pSt);
void func_800641D0(ST_WORK *pSt, UCHAR a, UCHAR b);
void func_800642D4(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm);
void func_800643E4(ST_WORK *pSt);
void func_80064424(ST_WORK *pSt);
void func_80064460(ST_WORK *pSt);
void func_8006446C(ST_WORK *pSt);
void func_800646F0(ST_WORK *pSt);
void func_80064E80(ST_WORK *pSt);
void func_80065054(ST_WORK *pSt);
void func_800650BC(ST_WORK *pSt);
void func_8006511C(ST_WORK *pSt);
void func_800654A8(ST_WORK *pSt);
void func_80065BA4(UCHAR a, UCHAR b);
void func_80065C8C(UCHAR dst, UCHAR src);
void func_80065D04(ST_WORK *pSt, UCHAR no, SHORT y0);
void func_80065EE0(ST_WORK *pSt);
void func_800661B4(LONG Arg0); /* args from the asm */
void func_80066530(ST_WORK *pSt);
void func_800665C8(ST_WORK *pSt);
void func_80066604(ST_WORK *pSt);
void func_800668E4(ST_WORK *pSt);
void func_80066920(ST_WORK *pSt);
void func_80066A90(ST_WORK *pSt);
void func_80066ADC(ST_WORK *pSt);
void func_80066CA0(ST_WORK *pSt);
void func_800676B8(ST_WORK *pSt);
void func_80067B70(ST_WORK *pSt);
void func_80067BAC(ST_WORK *pSt);
void func_80067C14(ST_WORK *pSt);
void func_80068024(ST_WORK *pSt);
void func_80068A54(ST_WORK *pSt);
void func_80068AB4(ST_WORK *pSt);
void func_80068B5C(ST_WORK *pSt);
void func_80068BC0(ST_WORK *pSt);
void func_80069280(ST_WORK *pSt);
void func_8006954C(ST_WORK *pSt);
UCHAR func_80069614(ST_WORK *pSt, LONG a1);
void func_80069A6C(ST_WORK *pSt);
void func_80069C3C(ST_WORK *pSt);
void func_8006A234(ST_WORK *pSt);
void func_8006A59C(ST_WORK *pSt);
void func_8006A888(ST_WORK *pSt);
UCHAR *func_8006A898(ST_WORK *pSt, UCHAR a, UCHAR b);
UCHAR *func_8006A918(ST_WORK *pSt, UCHAR id);
UCHAR *func_8006A95C(ST_WORK *pSt, UCHAR id);
void func_8006A9AC(UCHAR no, UCHAR idx);
void func_8006AA84(UCHAR no);
void func_8006AB08(UCHAR no);
void func_8006AB68(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf);
void func_8006ABF8(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf);
void func_8006AC88(UCHAR no);
void func_8006AD38(UCHAR a, UCHAR b);
void func_8006AE20(UCHAR dst, UCHAR src);
void func_8006AE90(ST_WORK *pSt);
void func_8006B66C(ST_WORK *pSt);
void func_8006C370(ST_WORK *pSt);
void func_8006C484(ST_WORK *pSt);
SPRT *func_8006C6D0(SPRT *p, SHORT num, ULONG xy, ULONG prm);
void func_8006CA10(ST_WORK *pSt);
signed char Search_item_id(UCHAR id, signed char i); /* RE2 Search_item_id */
signed char func_8006CCF0(UCHAR no, signed char n, UCHAR f);
void func_8006CD68(UCHAR no);
void func_8006CEA8(void);
signed char func_8006CF0C(UCHAR f);
LONG func_8006D030(UCHAR f);
void func_8006D0D8(void);
void func_8006D720(void);
void func_8006D948(ST_WORK *pSt);
void func_8006DFDC(void);
void func_8006E0A4(ST_WORK *pSt);
void func_8006E268(ST_WORK *pSt);
void func_8006E34C(ST_WORK *pSt);
void func_8006E424(ST_WORK *pSt);
void func_8006E4CC(ST_WORK *pSt);
LONG func_8006E598(void);
void func_8006E600(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm);
void func_8006E6D8(ST_WORK *pSt, POLY_FT4 *p, USHORT *src, ULONG prm);
void func_8006E7D4(ST_WORK *pSt, TILE *p, SHORT *pWh, UCHAR n);
void func_8006E84C(ST_WORK *pSt);
void func_8006E8BC(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm);
void func_8006E9D4(ST_WORK *pSt, POLY_FT4 *p, USHORT *src, ULONG prm);
void func_8006EAF0(ST_WORK *pSt, TILE *p, UCHAR *src, ULONG prm);
void func_8006EBEC(void);
void func_8006ECB0(ULONG no);
void func_8006ED28(ST_WORK *pSt);
void func_8006ED98(ST_WORK *pSt);
void func_8006F6CC(ST_WORK *pSt);
void func_8006F708(ST_WORK *pSt);
void func_8006FAE4(ST_WORK *pSt);
void func_8006FC68(ST_WORK *pSt);
void func_8006FEB0(ST_WORK *pSt);
void func_8006FF48(ST_WORK *pSt);
void func_80070024(ST_WORK *pSt);
void func_80070244(ST_WORK *pSt);
void func_8007116C(ST_WORK *pSt, POLY_FT4 *p, ST_MAP_PART *pPt, UCHAR clut);
void func_800713DC(ST_WORK *pSt, UCHAR no);
void func_80071510(UCHAR a, UCHAR b);
ULONG func_80071668(UCHAR a, UCHAR b);
LONG func_800716A4(UCHAR st, UCHAR rm);
LONG func_80071878(UCHAR st, UCHAR rm, UCHAR cut);
LONG func_8007275C(UCHAR st, UCHAR rm, UCHAR cut);
LONG func_80072AEC(UCHAR st, UCHAR rm, UCHAR cut);
LONG func_80072D38(UCHAR st, UCHAR rm, UCHAR cut);
LONG func_800738CC(UCHAR st, UCHAR rm, UCHAR cut);
LONG func_80073BC8(ST_WORK *pSt, UCHAR st, UCHAR rm);
void func_80073CCC(UCHAR *pA, UCHAR *pB);
void func_80073E7C(void);

#endif /* STATUS_H */
