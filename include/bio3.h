#ifndef BIO3_H
#define BIO3_H
/*
 * Shared game structs. Names follow RE2 (ref/bio2-ds, from the beta MAIN.SYM);
 * offsets are RE3's and were checked against the RE3 code.
 *
 * Rules for growing a struct: keep fields in offset order, carve new fields out
 * of the neighbouring padN[] array, keep the "0x..." offset comments correct, and
 * never change the struct size. A wrong offset breaks every user, so only add a
 * field once a matched function proves it.
 */
#include "common.h"
#include "psyq.h"

/* shadow work (RE2 struct tagKAGE_WORK, same layout, 0x68 bytes) */
typedef struct tagKAGE_WORK {
    SHORT Off_x;                       /* 0x00 */
    SHORT Off_z;                       /* 0x02 */
    SHORT Half_x;                      /* 0x04 */
    SHORT Half_z;                      /* 0x06 */
    SHORT Pos_x;                       /* 0x08 */
    SHORT Pos_y;                       /* 0x0A */
    SHORT Pos_z;                       /* 0x0C */
    UCHAR Be_flg;                      /* 0x0E */
    SHORT Dir_y;                       /* 0x10 */
    VECTOR *pPos;                      /* 0x14 */
    POLY_FT4 Prim[2];                  /* 0x18 */
} KAGE_WORK;

/* shadow work, 9-quad variant (RE2 struct tagKAGE_WORK9, same layout, 0x1C bytes) */
typedef struct tagKAGE_WORK9 {
    SHORT Off_x;                       /* 0x00 */
    SHORT Off_z;                       /* 0x02 */
    SHORT Half_x;                      /* 0x04 */
    SHORT Half_z;                      /* 0x06 */
    SHORT Pos_x;                       /* 0x08 */
    SHORT Pos_y;                       /* 0x0A */
    SHORT Pos_z;                       /* 0x0C */
    UCHAR Be_flg;                      /* 0x0E */
    SHORT Dir_y;                       /* 0x10 */
    VECTOR *pPos;                      /* 0x14 */
    KAGE_WORK *pKw_top;                /* 0x18 */
} KAGE_WORK9;

/* G: global work area (RE2 struct tagGLOBAL, 0x800CC1E8 in RE2) */
struct tagROOM_WORK;
struct tagENEMY_WORK;
struct tagPARTS_W;
struct tagSCE_AOT;
typedef struct tagGLOBAL {
    ULONG Ot0[2][8];                   /* 0x0000 */
    ULONG *pOt0;                      /* 0x0040 */
    UCHAR pad44[0x2044 - 0x44];
    ULONG *pOt1;                       /* 0x2044 */
    UCHAR pad2048[0x2088 - 0x2048];
    ULONG *pOt2;                       /* 0x2088 */
    UCHAR pad208C[0x20F8 - 0x208C];
    LONG Unk20F8;                      /* 0x20F8 set to -1 on soft reset */
    UCHAR pad20FC[0x2104 - 0x20FC];
    ULONG Key;                         /* 0x2104 RE2 Key (held buttons) */
    ULONG Key_trg;                     /* 0x2108 RE2 Key_trg (Gacha_ch) */
    UCHAR pad210C[0x2110 - 0x210C];
    UCHAR Cbuf_id;                     /* 0x2110 */
    UCHAR pad2111[0x2116 - 0x2111];
    UCHAR Unk2116;                     /* 0x2116 quake timer? (Quake_add) */
    UCHAR pad2117;
    SHORT Unk2118;                     /* 0x2118 */
    SHORT Unk211A;                     /* 0x211A */
    UCHAR pad211C[0x2120 - 0x211C];
    ULONG System_flg;                  /* 0x2120 */
    UCHAR pad2124[0x2134 - 0x2124];
    struct tagROOM_WORK *pRoom;        /* 0x2134 */
    UCHAR pad2138[0x2140 - 0x2138];
    struct tagENEMY_WORK *pEm;         /* 0x2140 current enemy (RE2 G.pEm) */
    UCHAR pad2144[0x2148 - 0x2144];
    struct tagVCUT *Cccut_next;        /* 0x2148 */
    UCHAR pad214C[0x2158 - 0x214C];
    struct tagSCE_AOT *Sce_aot[32];    /* 0x2158 RE2 Sce_aot (cleared by Sce_aot_init) */
    UCHAR pad21D8[0x21E8 - 0x21D8];
    void (*pPldamage_Em[64])();        /* 0x21E8 RE2 pPldamage_Em, index enemy Id - 0x10 (Pl_Em_damage) */
    void (*pPldie_Em[64])();           /* 0x22E8 RE2 pPldie_Em (Pl_Em_die) */
    UCHAR pad23E8[0x2468 - 0x23E8];
    ULONG Unk2468;                     /* 0x2468 flags; 0x1000000 tested by pl_mv06_pad / Pl_move */
    UCHAR pad246C[0x2486 - 0x246C];
    UCHAR Next_cut_no;                /* 0x2486 */
    UCHAR pad2487[0x2495 - 0x2487];
    signed char Pl_nFloor;             /* 0x2495 Player.nFloor (Player at 0x248C?) */
    UCHAR pad2496[0x24C0 - 0x2496];
    LONG Pl_pos_x;                     /* 0x24C0 Player.Pos_x (VECTOR Pos) */
    LONG Pl_pos_y;                     /* 0x24C4 Player.Pos_y */
    LONG Pl_pos_z;                     /* 0x24C8 Player.Pos_z */
    UCHAR pad24CC[0x24FA - 0x24CC];
    SHORT Pl_cdir_y;                   /* 0x24FA Player.Cdir_y */
    UCHAR pad24FC[0x2594 - 0x24FC];
    struct tagPARTS_W *Pl_pSin_parts_ptr; /* 0x2594 Player.pSin_parts_ptr */
    UCHAR pad2598[0x2658 - 0x2598];
    SHORT Pl_poison_timer;             /* 0x2658 RE2 Pl_poison_timer (Pl_move: -1 -> 30 and Life--) */
    UCHAR pad265A[0x265C - 0x265A];
    struct tagENEMY_WORK *pUnk265C;    /* 0x265C work processed before the enemy list (player?) */
    struct tagENEMY_WORK *pUnk2660;    /* 0x2660 work processed when Be_flg & 1 (sub player?) */
    struct tagENEMY_WORK *pEnemy[40];  /* 0x2664 RE2 pEnemy */
    struct tagENEMY_WORK **ppEnemy_end; /* 0x2704 RE2 ppEnemy_end (end of used pEnemy[]) */
    UCHAR pad2708[0x2904 - 0x2708];
    struct tagKAGE_WORK9 Kage9[5];     /* 0x2904 */
    struct tagKAGE_WORK Kage[50];      /* 0x2990 */
    UCHAR pad3DE0[0x75D8 - 0x3DE0];
    UCHAR nSce_aot;                    /* 0x75D8 RE2 nSce_aot */
    UCHAR pad75D9[0x77F4 - 0x75D9];
    ULONG Status_flg;                  /* 0x77F4 */
    UCHAR pad77F8[0x780E - 0x77F8];
    SHORT Sce_work[8];                 /* 0x780E RE2 G.SaveData.Sce_work (SCE For2/Switch; RE3 indexes it directly, size unknown) */
    UCHAR pad781E[0x7842 - 0x781E];
    SHORT Cut_no;                      /* 0x7842 RE2 G.SaveData.Cut_no */
    UCHAR pad7844[0x7860 - 0x7844];
    SHORT Unk7860;                     /* 0x7860 */
} tagGLOBAL;
extern tagGLOBAL G;                    /* 0x800CA738 */

/* G.System_flg bits */
#define SYS_SPAD_STACK 0x4000000

/* G.Status_flg bits */
#define STAT_XA_VOICE 0x20

/* model part (RE2 struct tagPARTS_W, JOINT.H; RE3 has no pTmd2/pPacket2/dummy00, 0xBC bytes) */
typedef struct tagPARTS_W {
    ULONG Be_flg;                      /* 0x00 */
    ULONG Attribute;                   /* 0x04 */
    ULONG *pTmd;                       /* 0x08 */
    ULONG *pPacket;                    /* 0x0C */
    ULONG Unk10;                       /* 0x10 */
    SHORT M[3][3];                     /* 0x14 */
    SHORT pad26;                       /* 0x26 */
    LONG Pos_x;                        /* 0x28 */
    LONG Pos_y;                        /* 0x2C */
    LONG Pos_z;                        /* 0x30 */
    SHORT Old_x;                       /* 0x34 */
    SHORT Old_y;                       /* 0x36 */
    SHORT Old_z;                       /* 0x38 */
    SHORT Old_x2;                      /* 0x3A */
    SHORT Old_y2;                      /* 0x3C */
    SHORT Old_z2;                      /* 0x3E */
    MATRIX Workm;                      /* 0x40 */
    SHORT Cdir_x;                      /* 0x60 */
    SHORT Cdir_y;                      /* 0x62 */
    SHORT Cdir_z;                      /* 0x64 */
    USHORT dummy01;                    /* 0x66 */
    ULONG Poly_rgb;                    /* 0x68 */
    MATRIX *pSuper;                    /* 0x6C */
    UCHAR pad70[0x74 - 0x70];
    UCHAR Unk74;                       /* 0x74 */
    UCHAR pad75[0x86 - 0x75];
    signed char Unk86;                 /* 0x86 */
    UCHAR pad87[0x90 - 0x87];
    SHORT Unk90;                       /* 0x90 */
    UCHAR pad92[0x94 - 0x92];
    SHORT Unk94;                       /* 0x94 */
    SHORT Unk96;                       /* 0x96 */
    UCHAR pad98[0xA2 - 0x98];
    USHORT UnkA2;                      /* 0xA2 */
    struct tagPARTS_W *pOya_parts;     /* 0xA4 */
    UCHAR padA8[0xBC - 0xA8];
} PARTS_W;

#endif /* BIO3_H */
