#ifndef MAIN_H
#define MAIN_H
/*
 * RE2 MAIN.C: main loop, display buffers, fades, camera cut switching.
 * RE3 layouts were taken from the RE3 code (they differ from RE2).
 */
#include "bio3.h"

/* camera cut switch zone (RE2 struct tagVCUT, 0x14 bytes) */
typedef struct tagVCUT {
    UCHAR Be_flg;                      /* 0x00 */
    signed char nFloor;                /* 0x01 -0x80: any floor */
    UCHAR Fcut;                        /* 0x02 */
    UCHAR Tcut;                        /* 0x03 */
    SHORT Xz[4][2];                    /* 0x04 */
} VCUT;

/* room header (RE2 struct tagROOM_HEAD) */
typedef struct tagROOM_WORK {
    UCHAR nSprite;                     /* 0x00 RE2 nSprite (Psp_trans) */
    UCHAR pad1;
    UCHAR nOmodel;                     /* 0x02 RE2 nOmodel */
    UCHAR pad3[7 - 3];
    UCHAR nSprite_max;                 /* 0x07 RE2 nSprite_max (func_80048630) */
    UCHAR pad8[0x20 - 0x8];
    struct tagSCA_HEAD *pSca;          /* 0x20 collision data (see sca.h) */
    RCUT *pRcut;                       /* 0x24 RE2 pRcut */
    VCUT *pVcut;                       /* 0x28 */
    ULONG *pLight;                     /* 0x2C RE2 LIGHT_CUT_DATA *pLight; RE3 adds a per-cut zone table (see sub.h) */
    UCHAR pad30[0x34 - 0x30];
    USHORT *pUnk34;                    /* 0x34 count, then 0xC byte records (Flr_set) */
    USHORT *pBlock;                    /* 0x38 RE2 pBlock (Rnd_area: [0] = count) */
    UCHAR *pMessage;                   /* 0x3C RE2 pMessage (offset table, then the text) */
    UCHAR *pUnk40;                     /* 0x40 count at +2, offsets at +4 (func_80058800) */
    ULONG *pUnk44;                     /* 0x44 second bg image (Swap_Cbuff) */
    UCHAR *pScd;                       /* 0x48 scenario data (Sce.pCScd) */
    UCHAR pad4C[0x60 - 0x4C];
    RCUT Cut[1];                       /* 0x60 RE2 Cut[] */
} ROOM_WORK;

/* screen fade (RE2 struct tagFADE; RE3 fades between two colours) */
typedef struct tagFADE {
    UCHAR Flg;                         /* 0x00 FADE_* bits */
    UCHAR Pri;                         /* 0x01 OT index */
    UCHAR Rgb0[3];                     /* 0x02 start colour */
    UCHAR Rgb1[3];                     /* 0x05 end colour */
    USHORT Cnt_max;                    /* 0x08 */
    USHORT Cnt;                        /* 0x0A */
    TILE T[2];                         /* 0x0C */
    DR_TPAGE Tpage[2];                 /* 0x2C */
    RECT Rect;                         /* 0x3C */
} FADE;

#define FADE_ON   1
#define FADE_HOLD 2
#define FADE_END  4

/* display buffer (RE2 struct tagDBUF, 0xC0 bytes) */
typedef struct tagDBUF {
    DRAWENV Draw;                      /* 0x00 */
    DISPENV Disp;                      /* 0x5C */
    TILE Bg_clr;                       /* 0x70 */
    TILE Curtain[2];                   /* 0x80 */
    TILE Pause;                        /* 0xA0 */
    DR_TPAGE Curtain_Dm;               /* 0xB0 */
    DR_TPAGE Pause_Dm;                 /* 0xB8 */
} DBUF;

/* main work (RE2 struct tagMAIN) */
typedef struct tagMAIN {
    DBUF Db[2];                        /* 0x000 */
    DBUF *pDb;                         /* 0x180 */
    RECT Rect;                         /* 0x184 RE2 Main.Rect */
    UCHAR Vloop;                       /* 0x18C RE2 Main.Vloop (Door_exit) */
    UCHAR Bg_mode;                     /* 0x18D */
    UCHAR Unk18E;                      /* 0x18E flash request (func_80024F74) */
    UCHAR pad18F[0x191 - 0x18F];
    UCHAR Unk191;                      /* 0x191 bg image +0x14 offset (Swap_Cbuff) */
    UCHAR Curtain_kido;                /* 0x192 */
    UCHAR Unk193;                      /* 0x193 set when the logo task is done (Init_main) */
    UCHAR Unk194;                      /* 0x194 */
    UCHAR Unk195;                      /* 0x195 */
    UCHAR Unk196;                      /* 0x196 */
    UCHAR pad197;
    ULONG Unk198;                      /* 0x198 flags (0x400000 tested by func_80029700) */
    FADE Fade[4];                      /* 0x19C */
} tagMAIN;
extern tagMAIN Main;                   /* 0x800D42A0 */

/* game work (RE2 struct tagGAME; RE3 has one more byte before Plight_flg) */
typedef struct tagGAME {
    UCHAR Doordemo_flg;                /* 0x00 */
    UCHAR Unk1;                        /* 0x01 */
    UCHAR Plight_flg;                  /* 0x02 */
    UCHAR Plight_cnt;                  /* 0x03 */
    UCHAR Plight_col[3];               /* 0x04 */
    UCHAR pad7;
    UCHAR Plight_bak[0x28];            /* 0x08 saved LIGHT_CUT_DATA */
    VECTOR Plight_pos;                 /* 0x30 */
    UCHAR pad40[0x44 - 0x40];
    ULONG Stop_bak;                    /* 0x44 RE2 Game.Stop_bak */
    struct tagSAVE_POS *pSave;         /* 0x48 (func_80024B34) */
    UCHAR Unk4C;                       /* 0x4C */
    UCHAR Unk4D;                       /* 0x4D */
    UCHAR Unk4E;                       /* 0x4E (Set_door) */
    UCHAR Unk4F;                       /* 0x4F cleared by Init_system */
} tagGAME;
extern tagGAME Game;                   /* 0x800C7960 */

/* memory card events (RE2 Card_event_set) */
extern long Ev0;                       /* 0x800D2650 */
extern long Ev1;                       /* 0x800D2654 */
extern long Ev2;                       /* 0x800D2658 */
extern long Ev3;                       /* 0x800D265C */
extern long Ev10;                      /* 0x800D4284 */
extern long Ev11;                      /* 0x800D4288 */
extern long Ev13;                      /* 0x800D428C */

/* flag bits used here (RE2 names) */
#define STAT_QUAKE_RASTA 0x1           /* G.Status_flg */
#define STAT_QUAKE_LARGE 0x2           /* G.Status_flg */
#define STAT_QUAKE_SMALL 0x4           /* G.Status_flg */
#define STAT_CINESCO   0x8             /* G.Status_flg */
#define SYS_FADE_OUT   0x800000        /* G.System_flg */
#define SYS_DOOR_INIT  0x1000000       /* G.System_flg */

/* owned by other modules, called from here */
void Sce_work_clr_set(void);
void Snd_reset(void);
void Task_lexecute(ULONG Level, void *pFuncall);   /* RE2 Task_lexecute? */
void func_8003884C(USHORT *pTbl, ULONG No);        /* pad: play vibration script No of pTbl */
void Rot_vector(SHORT Dir, SVECTOR *p, SVECTOR *q);  /* RE2 Rot_vector */
extern USHORT Vib_data_tbl[];          /* 0x800986D0 vibration scripts used by Flash_fade_in.. */

void Draw_list_all(ULONG Arg0, ULONG Arg1, ULONG Arg2);
void func_8002EB9C(ULONG Arg0, ULONG Arg1, ULONG Arg2, struct tagENEMY_WORK *pEm);

/* GAME.C part of this chunk */
void Gun_light_set(void);              /* Gun_light_set */
void Gun_light_reset(void);              /* Gun_light_reset */
void Flash_fade_in(void);
void Flash_fade_out(void);

void Init_global(void);
void Soft_reset(void);
void Card_event_set(void);              /* Card_event_set */
void System_trans_all(void);
void Curtain_trans(void);
void func_8002A49C(void);
void Fade_out_reset(void);
ULONG func_8001020C(LONG *p, VCUT *cp);  /* Hit_ck_point4 */
void Bg_set_mode(ULONG Mode, ULONG Rgb);
void Fade_set(ULONG No, UCHAR Pri, ULONG Flg, ULONG Abr, ULONG Rgb0, ULONG Rgb1, USHORT Cnt_max);
void Fade_rect_set(ULONG No, RECT *Rect);
void Fade_off(ULONG No);
ULONG Fade_status(ULONG No);
ULONG Cut_check(ULONG Flg);
void Cut_change(ULONG Fc);
VCUT *Ccut_search(ULONG Fc);

#endif /* MAIN_H */
