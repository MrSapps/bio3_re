#ifndef OBA_H
#define OBA_H
/*
 * OBA.C (RE3 rewrite): collision object list. Each character (and some room
 * objects) owns a COL_OBJ; the list is collected every frame into COL_ENT
 * records that SCA.C tests like room shapes. The player's node lives at
 * Pl_work + 0x80.
 */
#include "em_sub.h"

/* collision object (one node of the list) */
typedef struct tagCOL_OBJ {
    UCHAR Id;                          /* 0x00 */
    UCHAR Flg;                         /* 0x01 */
    UCHAR Type;                        /* 0x02 0 = player, < 3 = character */
    UCHAR Unk3;                        /* 0x03 */
    ENEMY_WORK *pEm;                   /* 0x04 */
    LONG Pos_x;                        /* 0x08 */
    LONG Pos_y;                        /* 0x0C */
    LONG Pos_z;                        /* 0x10 */
    SHORT Old_x;                       /* 0x14 */
    SHORT Old_y;                       /* 0x16 */
    SHORT Old_z;                       /* 0x18 */
    USHORT W0;                         /* 0x1A radius / half width */
    USHORT D0;                         /* 0x1C half depth */
    USHORT H;                          /* 0x1E */
    USHORT W1;                         /* 0x20 */
    USHORT D1;                         /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    SHORT Unk26;                       /* 0x26 */
    SHORT Unk28;                       /* 0x28 */
    SHORT Unk2A;                       /* 0x2A */
    SHORT Unk2C;                       /* 0x2C */
    USHORT Hit_flg;                    /* 0x2E 0x100: pushed along a slope */
    MATRIX *pM;                        /* 0x30 */
    struct tagCOL_OBJ *pNext;          /* 0x34 */
    UCHAR Em_idx;                      /* 0x38 */
    UCHAR Unk39;                       /* 0x39 */
    SHORT Unk3A;                       /* 0x3A */
    LONG Unk3C;                        /* 0x3C */
} COL_OBJ;

/* entry of the collected collision list (0x18 bytes); starts like SCA_DATA */
typedef struct tagCOL_ENT {
    SHORT X0;                          /* 0x00 shape corners */
    SHORT Z0;                          /* 0x02 */
    SHORT X1;                          /* 0x04 */
    SHORT Z1;                          /* 0x06 */
    USHORT Id;                         /* 0x08 SCA_DATA.Id, & 0xF: shape */
    USHORT Area;                       /* 0x0A SCA_DATA.Type: area bits */
    union {
        LONG All;                      /* -1 terminates the list */
        struct {
            signed char Floor_lo;      /* 0x0C */
            signed char Floor_hi;      /* 0x0D */
            SHORT High;                /* 0x0E */
        } f;
    } C;                               /* 0x0C SCA_DATA.Floor_lo/Floor_hi/High */
    UCHAR Type;                        /* 0x10 COL_OBJ.Type */
    UCHAR Em_idx;                      /* 0x11 */
    UCHAR pad12[2];
    COL_OBJ *pObj;                     /* 0x14 */
} COL_ENT;

typedef struct tagCOL_WORK {
    COL_OBJ *pTop;                     /* 0x00 */
    COL_OBJ *pTail;                    /* 0x04 */
    COL_ENT Ent[1];                    /* 0x08 */
} COL_WORK;
extern COL_WORK D_800DCED0;

/* a character's collision object (ENEMY_WORK + 0x80) */
#define EM_COL(pEm) ((COL_OBJ *)((UCHAR *)(pEm) + 0x80))

/* C.All = High << 16 | Floor_hi << 8 | Floor_lo, both floor bytes set to Floor.
 * The do/while wrappers are load-bearing (they change register allocation). */
#define COL_ENT_SET_C(pEnt, High, Floor)                                                 \
    do {                                                                                 \
        (pEnt)->C.All = ((High) << 16) | ((((Floor) << 8) & 0xFF00) | ((Floor) & 0xFF)); \
    } while (0)
/* entry height = top of the object (Pos_y - H) */
#define COL_ENT_SET_HIGH(pEnt, pObj, Y, Floor) \
    do {                                       \
        Y = (pObj)->Pos_y;                     \
        Y -= (pObj)->H;                        \
        COL_ENT_SET_C(pEnt, Y, Floor);         \
    } while (0)

#endif /* OBA_H */
