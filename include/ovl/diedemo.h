#ifndef OVL_DIEDEMO_H
#define OVL_DIEDEMO_H
/*
 * DIEDEMO.BIN (loaded at 0x80194000): game over / death demo
 * (RE2 DIEDEMO.C camera state machine + DIESPR.C screen break-up effect).
 */
#include "bio3.h"
#include "cd.h"
#include "main.h"
#include "monitor.h"
#include "em_sub.h"
#include "om.h"
#include "pad.h"
#include "pl_wep.h"
#include "player.h"
#include "snd.h"
#include "sub.h"

/* death camera cut parameters (RE2 struct tagCUTINFO, same layout, 0x1C bytes) */
typedef struct tagCUTINFO {
    LONG Radius;                       /* 0x00 */
    LONG P_radius;                     /* 0x04 */
    SHORT Addpos;                      /* 0x08 */
    SHORT Prad;                        /* 0x0A theta step (Cdir_ck != 0) */
    SHORT P_theta;                     /* 0x0C */
    SHORT Mrad;                        /* 0x0E theta step (Cdir_ck == 0) */
    SHORT M_theta;                     /* 0x10 */
    SHORT P_viewp;                     /* 0x12 view point height offset */
    USHORT Wait_ctr;                   /* 0x14 */
    SHORT R_obj;                       /* 0x16 look-at object: 0 player, 1 enemy */
    SHORT R_ptr;                       /* 0x18 part of the look-at object */
    SHORT Free;                        /* 0x1A */
} CUTINFO;

/* per cut info: frames to wait before the camera, flags (bit 0: stop flags in G.Unk2468) */
typedef struct tagDIE_WAIT {
    USHORT Wait;                       /* 0x0 */
    USHORT Flg;                        /* 0x2 */
} DIE_WAIT;

/* death demo work (RE2 struct tagDIEDEMO, grown) */
typedef struct tagDIEDEMO {
    SHORT Addpos;                      /* 0x00 */
    USHORT Free;                       /* 0x02 */
    LONG Ctr;                          /* 0x04 */
    RCUT Rcut;                         /* 0x08 */
    LONG Radius;                       /* 0x28 */
    SHORT Theta;                       /* 0x2C */
    SHORT Prad;                        /* 0x2E */
    LONG P_radius;                     /* 0x30 */
    UCHAR Phase;                       /* 0x34 */
    UCHAR S_trans_flg;                 /* 0x35 */
    SHORT Daddpos;                     /* 0x36 */
    SHORT Wait_ctr;                    /* 0x38 */
    UCHAR Em_flg;                      /* 0x3A bit 0: killed by the partner (G.pEnemy[1]) */
    UCHAR Unk3B;                       /* 0x3B */
    USHORT Id_ctr;                     /* 0x3C cut info index */
    UCHAR pad3E[0x40 - 0x3E];
    LONG Rno;                          /* 0x40 index of Die_move_tbl */
    LONG Unk44;                        /* 0x44 */
    CUTINFO *pCutinfo;                 /* 0x48 */
    ENEMY_WORK *pEm;                   /* 0x4C killer (when it gets an extra effect) */
    UCHAR pad50[0x54 - 0x50];
    USHORT Fade_parts;                 /* 0x54 last part of pEm to fade out */
    USHORT Fade_kido;                  /* 0x56 pEm part brightness while it fades out */
} DIEDEMO;

/* screen break-up work at 0x801B1000 (RE2 struct tagDIESPR, same layout) */
typedef struct tagDIESPR {
    USHORT Rno;                        /* 0x0000 index of Die_spr_tbl */
    SHORT Free2;                       /* 0x0002 frame counter */
    UCHAR Free4;                       /* 0x0004 fall speed */
    UCHAR Free5;                       /* 0x0005 frames to the next fall speed step */
    SHORT Ctr;                         /* 0x0006 */
    POLY_GT4 Pgt4[2][256];             /* 0x0008 16x16 quads per buffer */
    SVECTOR Pos[17][17];               /* 0x6808 grid vertices [Col][Row] */
    SHORT Dist[17][17];                /* 0x7110 distance from the grid origin */
    USHORT pad7352;
    POLY_FT4 Blood[3][2][16][8];       /* 0x7354 */
    SHORT Move_start;                  /* 0xEB54 */
    USHORT Blood_flg;                  /* 0xEB56 */
} DIESPR;

/* overlay data (asm/data/ovl/diedemo/data.data.s) */
extern USHORT D_80197F8C[];            /* wait count by cut info index */
extern USHORT D_80197FA0[];
extern USHORT D_80197FA4[];            /* vibration scripts (func_8003884C) */
extern CUTINFO Cutinfo[];           /* cut infos */
extern CUTINFO D_801981B0;             /* cut info 9 for flag 0xC2 */
extern DIE_WAIT D_801981CC[];
extern UCHAR D_801981F8[];             /* enemy id of each cut info, searched from the end */
extern LONG D_80198204;                /* set when the demo can be skipped */
extern void (*Die_move_tbl[])(void); /* routines, index Diedemo.Rno */
extern void (*D_80198228[])(void); /* index Diedemo.Unk44 */
extern DIEDEMO Diedemo;
extern VCUT Dievcut;
extern LONG D_80198230[2][3];          /* screen break-up camera: view point, view reference */
extern void (*Die_spr_tbl[])(void); /* screen break-up routines, index pDiespr->Rno */
extern DIESPR *pDiespr; /* RE2 Dievcut: camera cut zone set as G.Cccut */

void Die_demo_move(void);
void Die_move_set_cut(void);
void Die_move_set_cut_usual(void);
void Die_enemy_set(void);
void Die_enemy_set_em(void);
void Die_enemy_set_sub(void);
void Die_cut_em36(void);
void Die_cut_em36_sub(void);
void Die_em_fade(void);
void Spiral_move(void);
void Spiral_move2(void);
void Die_light_set(void);
void Die_spr_init(void);
void Die_spr_move(void);
void Die_spr_move_init(void);
void Die_spr_load(void);

/* GTE inline macros (PsyQ inline_c.h with the DMPSX-converted opcodes) */
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")

void Die_demo(void);
void Die_move_set(void);
void Die_move_delay(void);
void Die_move_fade(void);
void Die_move_slow(void);
void Die_se_set(void);
void Die_move_main(void);
void Die_move_out(void);
void Die_move_dark(void);
void Die_move_end(void);
void Die_blood_move_init(void);
void Die_spr_move_main(void);
void Die_spr_move_hold(void);
void Die_spr_move_erase(void);
void Die_spr_move_end(void);
void Die_blood_move_main(void);

#endif /* OVL_DIEDEMO_H */
