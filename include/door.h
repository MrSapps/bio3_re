#ifndef DOOR_H
#define DOOR_H
/* RE2 DOOR.C: door opening sequence (Door_main / Door_init / Door_move / ...) */
#include "bio3.h"
#include "em_sub.h"
#include "sce0.h"

typedef void (*EM_PROC)(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);

typedef void (*DOOR_PROC)(void);
extern DOOR_PROC Door_proc_tbl[];      /* 0x800979F0, NULL terminated (RE2 pDoor_proc_tbl) */

/* door texture/sound info (12 bytes, RE2 DOOR_INFO) */
typedef struct {
    USHORT Sound_size;                 /* 0x0 */
    USHORT Data_size;                  /* 0x2 */
    ULONG Sector_offset;               /* 0x4 */
    UCHAR Sound_sum;                   /* 0x8 */
    UCHAR Data_sum;                    /* 0x9 */
    UCHAR padA[2];
} DOOR_INFO;
extern DOOR_INFO Dinfo[];         /* index Dtex_type */
extern USHORT *Door_file_tbl[];           /* door file no per stage, index Dtex_type */

/* door data file header, loaded at 0x8019D000 (RE2 Door_data->hdr; offsets relative to the file) */
typedef struct {
    ULONG *pTmd;                       /* 0x0 */
    ULONG *pTim;                       /* 0x4 */
    UCHAR Scd[4];                      /* 0x8 door event script */
} DOOR_DATA;
#define Door_data ((DOOR_DATA *)0x8019D000)

/* RE3 door model work (pDwork[] slot, 0xC8 bytes); 0x10.. is laid out like MODEL_WORK */
typedef struct tagDOOR_WORK {
    ULONG Be_flg;                      /* 0x00 */
    UCHAR pad04[0x10 - 0x4];
    LONG Attribute;                    /* 0x10 */
    ULONG *pTmd;                       /* 0x14 */
    POLY_GT3 *pPacket;                 /* 0x18 two buffers per door, index G.Cbuf_id */
    UCHAR pad1C[0x20 - 0x1C];
    SHORT M[3][3];                     /* 0x20 rotation part of a MATRIX whose t[] is Pos_x..Pos_z */
    UCHAR pad32[0x34 - 0x32];
    LONG Pos_x;                        /* 0x34 */
    LONG Pos_y;                        /* 0x38 */
    LONG Pos_z;                        /* 0x3C */
    UCHAR pad40[0x4A - 0x40];
    UCHAR Id;                          /* 0x4A */
    UCHAR pad4B;
    MATRIX Workm;                      /* 0x4C */
    SHORT Cdir_x;                      /* 0x6C */
    SHORT Cdir_y;                      /* 0x6E */
    SHORT Cdir_z;                      /* 0x70 */
    UCHAR pad72[0x74 - 0x72];
    ULONG Poly_rgb;                    /* 0x74 */
    MATRIX *pSuper;                    /* 0x78 */
    UCHAR pad7C[0xC0 - 0x7C];
    USHORT Attribute2;                 /* 0xC0 */
    UCHAR Model_no;                    /* 0xC2 */
    UCHAR Type;                        /* 0xC3 */
    USHORT Free;                       /* 0xC4 brightness ramp counter (Attribute2 & 0x1000) */
    USHORT Free2;                      /* 0xC6 current brightness */
} DOOR_WORK;

/* door system work (RE2 struct tagDOOR, pDoor[0]); RE3 dropped RE2's pEdh_offset/pVb_offset */
typedef struct tagDOOR {
    POLY_GT3 *pPrepacket;              /* 0x000 next free prims (Door_model_init) */
    ULONG *pTmd_adr;                   /* 0x004 door models, 6 words each */
    POLY_GT3 *pPackettop;              /* 0x008 */
    DIVPOLYGON3 Divp;                  /* 0x00C */
    USHORT Ctr1;                       /* 0x224 */
    USHORT Ctr2;                       /* 0x226 frame counter (Door_move) */
    SVECTOR V0;                        /* 0x228 */
    SVECTOR V1;                        /* 0x230 */
    SVECTOR V2;                        /* 0x238 */
    USHORT Sound_flg;                  /* 0x240 RE2 Sound_flg: play the exit sound (Door_exit) */
    USHORT Div_max;                    /* 0x242 */
} DOOR;

void Door_main(void);

extern UCHAR Em_sce06_move_tbl[];
extern UCHAR Em_sce04_move_tbl[];
extern UCHAR Em_sce05_move_tbl[];
extern UCHAR Em_sce07_move_tbl[];
extern EM_PROC Em_sce[];
extern ULONG pDoor_tex_trans; /* RE2 pDoor[1] */
extern ULONG D_800BA6F4; /* door work slot size */
extern DOOR *pDoor;              /* RE2 pDoor[0] */
extern DOOR_WORK *pDwork[10];      /* RE2 pDwork[] (door work slots) */
extern ULONG Enemy_flg[];

void Door_init(void);
void Door_move(void);
void Door_scheduler_main(void);
void Door_Trans(void); /* RE2 Door_Trans */
struct tagSORT_PARAM;
void DoorSort(DOOR_WORK *pDw, POLY_GT3 *pPacket, struct tagSORT_PARAM *pPrm); /* RE2 DoorSort */
void Door_exit(void);
void Door_texture_load(void); /* RE2 Door_texture_load */
void Door_model_init(DOOR_WORK *pDw);
ULONG Door_model_set(SCE_TASK *pSce);
void Door_snd_trans(void);
void Em_sce_main(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em_sce00(ENEMY_WORK *pEm);
void Em_sce01(ENEMY_WORK *pEm);
void Em_sce02(ENEMY_WORK *pEm);
void Em_sce03(ENEMY_WORK *pEm);
void Em_sce04(ENEMY_WORK *pEm);
void Em_sce05(ENEMY_WORK *pEm);
void Em_sce06(ENEMY_WORK *pEm);
void Em_sce07(ENEMY_WORK *pEm);
void Em_sce08(ENEMY_WORK *pEm);
void Em_sce09(ENEMY_WORK *pEm);
void Em_sce15(ENEMY_WORK *pEm);
void Em_sce_mv04(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em_sce_mv01(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
LONG Em_sce_mv02(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG a3);
LONG Em_sce_mv03(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em_sce_mv00(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void Em_sce_foot_se(ENEMY_WORK *pEm);
void Enemy_work_set(void);
ULONG Emd_load(ULONG Id, ENEMY_WORK *pEm, ULONG Mem_top, ENEMY_WORK *pData);
void Em_move_tbl_set(void);
void Door_dummy(void);

#endif /* DOOR_H */
