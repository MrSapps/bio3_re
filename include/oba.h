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
extern COL_WORK Oba_work;

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

extern ULONG Oba_att_tbl[];
extern COL_OBJ *pOba_list[]; /* objects collected this frame, NULL terminated */

void Oba_init(void);
UCHAR Oba_set(COL_OBJ *pObj, ULONG Id);
LONG Em_work_no_get(ENEMY_WORK *pEm); /* collision index of a work */
void Oba_ent_collect(COL_OBJ *pTop, COL_ENT *pEnt);
void Oba_hit_ck_all(COL_OBJ *pTop, COL_ENT *pEnt);
LONG Oba_push_all(COL_OBJ *pTop, COL_ENT *pEnt);
void Oba_main(void);
void Oba_pos_back(COL_OBJ *pTop);
COL_ENT *Oba_ent_box_set(COL_OBJ *pObj, COL_ENT *pEnt);
void Oba_ent_set(COL_OBJ *pObj, COL_ENT *pEnt, LONG n);
void Oba_pos_update(COL_OBJ *pTop);
void Oba_hit_clr(COL_OBJ *pObj);
void Oba_ent_floor_set(COL_OBJ *pObj, COL_ENT *pEnt);
LONG Oba_front_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG Oba_pos_em_ck(VECTOR *pPos, ENEMY_WORK *pEm);
LONG Oba_pos_em2_ck(VECTOR *pPos, ENEMY_WORK *pEm, ENEMY_WORK *pT);
void Oba_em_hit_ck(ENEMY_WORK *pEm, LONG W, ULONG Tbl, COL_OBJ *pObj);
void Oba_push_obj_ck(COL_OBJ *pObj, COL_ENT *pEnt, ULONG Arg2, ULONG Type);
struct tagSCA_DATA;                    /* SCA_DATA (sca.h includes this header) */
void Oba_tmp_sca_set(COL_OBJ *pObj, struct tagSCA_DATA *pT); /* shape of an object as SCA_DATA */
void Oba_em_overlap_ck(ENEMY_WORK *pEm, LONG W, ULONG Tbl, COL_OBJ *pObj);
LONG Oba_area_ck(ENEMY_WORK *pEm, ULONG W, ULONG Tbl, COL_OBJ *pObj);
LONG Oba_cross_ck(COL_OBJ *pObj, COL_OBJ *pT);
LONG Oba_push_out(COL_OBJ *pObj, COL_OBJ *pT);
ULONG Oba_box_push(COL_OBJ *pObj, LONG W, struct tagSCA_DATA *pSca, ULONG Old);

#endif /* OBA_H */
