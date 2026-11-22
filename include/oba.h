#ifndef OBA_H
#define OBA_H
/*
 * OBA.C (RE3 rewrite): collision object list. Each character (and some room
 * objects) owns a COL_OBJ; the list is collected every frame into COL_ENT
 * records that SCA.C tests like room shapes. The node is ENEMY_WORK.Col (bio3.h).
 */
#include "em_sub.h"

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

extern ULONG D_8009CBC8[];
extern COL_OBJ *D_800DCDD0[]; /* objects collected this frame, NULL terminated */

void func_80033508(void);
UCHAR func_80033580(COL_OBJ *pObj, ULONG Id);
LONG func_800336B4(ENEMY_WORK *pEm); /* collision index of a work */
void func_800336F0(COL_OBJ *pTop, COL_ENT *pEnt);
void func_800337B8(COL_OBJ *pTop, COL_ENT *pEnt);
LONG func_8003389C(COL_OBJ *pTop, COL_ENT *pEnt);
void func_80033B20(void);
void func_80033B88(COL_OBJ *pTop);
COL_ENT *func_80033CF4(COL_OBJ *pObj, COL_ENT *pEnt);
void func_80033F38(COL_OBJ *pObj, COL_ENT *pEnt, LONG n);
void func_80033FBC(COL_OBJ *pTop);
void func_80034124(COL_OBJ *pObj);
void func_80034284(COL_OBJ *pObj, COL_ENT *pEnt);
LONG func_80034448(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG func_800345C0(VECTOR *pPos, ENEMY_WORK *pEm);
LONG func_800346AC(VECTOR *pPos, ENEMY_WORK *pEm, ENEMY_WORK *pT);
void func_80034814(ENEMY_WORK *pEm, LONG W, ULONG Tbl, COL_OBJ *pObj);
void func_80034D74(COL_OBJ *pObj, COL_ENT *pEnt, ULONG Arg2, ULONG Type);
struct tagSCA_DATA;                    /* SCA_DATA (sca.h includes this header) */
void func_80034E78(COL_OBJ *pObj, struct tagSCA_DATA *pT); /* shape of an object as SCA_DATA */
void func_80034F08(ENEMY_WORK *pEm, LONG W, ULONG Tbl, COL_OBJ *pObj);
LONG func_80035184(ENEMY_WORK *pEm, ULONG W, ULONG Tbl, COL_OBJ *pObj);
LONG func_800353CC(COL_OBJ *pObj, COL_OBJ *pT);
LONG func_8003553C(COL_OBJ *pObj, COL_OBJ *pT);
ULONG func_80035678(COL_OBJ *pObj, LONG W, struct tagSCA_DATA *pSca, ULONG Old);

#endif /* OBA_H */
