#ifndef DOOR_H
#define DOOR_H
/* RE2 DOOR.C: door opening sequence (Door_main / Door_init / Door_move / ...) */
#include "bio3.h"
#include "em_sub.h"
#include "sce0.h"

typedef void (*EM_PROC)(ENEMY_WORK *pEm, LONG a1, LONG a2);

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
extern DOOR_INFO D_800971E4[];         /* RE2 Dinfo[], index Dtex_type */
extern USHORT *D_800979D4[];           /* door file no per stage, index Dtex_type */

/* door data file header, loaded at 0x8019D000 (RE2 Door_data->hdr; offsets relative to the file) */
typedef struct {
    ULONG *pTmd;                       /* 0x0 */
    ULONG *pTim;                       /* 0x4 */
    UCHAR Scd[4];                      /* 0x8 door event script */
} DOOR_DATA;
#define Door_data ((DOOR_DATA *)0x8019D000)

void Door_main(void);

extern UCHAR D_80097A00[];
extern UCHAR D_80097A28[];
extern UCHAR D_80097A50[];
extern UCHAR D_80097A78[];
extern EM_PROC D_80097AA0[];
extern ULONG D_800BA6F0; /* RE2 pDoor[1] */
extern ULONG D_800BA6F4; /* door work slot size */
extern ULONG *D_800BA6F8;
extern UCHAR *D_800BA700[10]; /* RE2 pDwork[] (door work slots) */
extern ULONG D_800D1FC8[];

void Door_init(void);
void Door_move(void);
void Door_scheduler_main(void);
void Door_Trans(void); /* RE2 Door_Trans */
void DoorSort(UCHAR *pMw, UCHAR *pDw, ULONG *pTmd); /* RE2 DoorSort(MODEL_WORK *, DOOR_WORK *, TMD_MEM *) */
void Door_exit(void);
void Door_texture_load(void); /* RE2 Door_texture_load */
void Door_model_init(UCHAR *pDw);
ULONG Door_model_set(SCE_TASK *pSce);
void Door_snd_trans(void);
void func_80016600(ENEMY_WORK *pEm, LONG a1, LONG a2);
void func_800167AC(ENEMY_WORK *pEm);
void func_800167E4(ENEMY_WORK *pEm);
void func_8001681C(ENEMY_WORK *pEm);
void func_80016854(ENEMY_WORK *pEm);
void func_8001688C(ENEMY_WORK *pEm);
void func_80016A40(ENEMY_WORK *pEm);
void func_80016BF4(ENEMY_WORK *pEm);
void func_80016C9C(ENEMY_WORK *pEm);
void func_80016DA0(ENEMY_WORK *pEm);
void func_80016E80(ENEMY_WORK *pEm);
void func_80016F44(ENEMY_WORK *pEm);
void func_80017008(ENEMY_WORK *pEm, LONG a1, LONG a2);
LONG func_80017148(ENEMY_WORK *pEm, LONG a1, LONG a2);
LONG func_800171DC(ENEMY_WORK *pEm, LONG a1, LONG a2, LONG a3);
LONG func_800172AC(ENEMY_WORK *pEm, LONG a1, LONG a2);
void func_80017318(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq);
void func_80017540(ENEMY_WORK *pEm);
void Enemy_work_set(void);
ULONG func_8001760C(ULONG Id, ENEMY_WORK *pEm, ULONG Mem_top, ENEMY_WORK *pData);
void Em_move_tbl_set(void);
void func_80017D9C(void);

#endif /* DOOR_H */
