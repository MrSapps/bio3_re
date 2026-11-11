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

/* camera cut (RE2 struct tagRCUT, 0x20 bytes) */
typedef struct tagRCUT {
    USHORT end_flg;                    /* 0x00 */
    USHORT ViewR;                      /* 0x02 */
    LONG View_p[3];                    /* 0x04 */
    LONG View_r[3];                    /* 0x10 */
    ULONG *pSp;                        /* 0x1C */
} RCUT;

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
    ULONG Unk208C;                     /* 0x208C packet pointer (func_80025FB0) */
    ULONG *pOt_sub[2][4];              /* 0x2090 per-buffer OT tables, index Cbuf_id (Psp_trans) */
    UCHAR Padd_data[2][0x24];          /* 0x20B0 RE2 Padd_data (PadInitDirect buffers) */
    LONG Unk20F8;                      /* 0x20F8 set to -1 on soft reset */
    USHORT Unk20FC;                    /* 0x20FC */
    UCHAR pad20FE[0x2104 - 0x20FE];
    ULONG Key;                         /* 0x2104 RE2 Key (held buttons) */
    ULONG Key_trg;                     /* 0x2108 RE2 Key_trg (Gacha_ch) */
    UCHAR pad210C[0x2110 - 0x210C];
    UCHAR Cbuf_id;                     /* 0x2110 */
    UCHAR Unk2111;                     /* 0x2111 display buffer y != 0 (Swap_Cbuff) */
    UCHAR pad2112;
    UCHAR Unk2113;                     /* 0x2113 set from the first TIM loaded by Esp_data_set1 */
    UCHAR Unk2114;                     /* 0x2114 cleared by Init_system */
    UCHAR pad2115;
    UCHAR Unk2116;                     /* 0x2116 quake timer? (Quake_add) */
    UCHAR pad2117;
    SHORT Unk2118;                     /* 0x2118 */
    SHORT Unk211A;                     /* 0x211A */
    UCHAR pad211C[0x2120 - 0x211C];
    ULONG System_flg;                  /* 0x2120 */
    USHORT *pBs_size;                  /* 0x2124 RE2 pBs_size: per room offset into pUnk2128 (Bg_draw) */
    USHORT *pUnk2128;                  /* 0x2128 bs file sizes */
    UCHAR *pBs_check_sum;              /* 0x212C RE2 pBs_check_sum */
    UCHAR pad2130[0x2134 - 0x2130];
    struct tagROOM_WORK *pRoom;        /* 0x2134 */
    ULONG Unk2138;                     /* 0x2138 free memory after the 0x8011A000 overlay (Set_stage) */
    struct tagENEMY_WORK *pEm_mem;     /* 0x213C enemy work allocation pointer (Enemy_work_set) */
    struct tagENEMY_WORK *pEm;         /* 0x2140 current enemy (RE2 G.pEm) */
    struct tagVCUT *Cccut;             /* 0x2144 RE2 G.Cccut (current camera cut zone) */
    struct tagVCUT *Cccut_next;        /* 0x2148 */
    UCHAR pad214C[0x2154 - 0x214C];
    void *pSce_aot_on;                 /* 0x2154 RE2 pSce_aot_on (door AOT data) */
    struct tagSCE_AOT *Sce_aot[32];    /* 0x2158 RE2 Sce_aot (cleared by Sce_aot_init) */
    void *pAot_data;                   /* 0x21D8 RE2 pAot_data */
    LONG Unk21DC;                      /* 0x21DC */
    UCHAR pad21E0[0x21E4 - 0x21E0];
    USHORT *pXa_sector;                /* 0x21E4 RE2 G.pXa_sector (Xa_play table for the stage) */
    void (*pPldamage_Em[64])();        /* 0x21E8 RE2 pPldamage_Em, index enemy Id - 0x10 (Pl_Em_damage) */
    void (*pPldie_Em[64])();           /* 0x22E8 RE2 pPldie_Em (Pl_Em_die) */
    struct tagSCA_DATA *Sca_list[32];  /* 0x23E8 SCA shapes matching the last search (NULL terminated) */
    ULONG Unk2468;                     /* 0x2468 flags; 0x1000000 tested by pl_mv06_pad / Pl_move */
    ULONG Unk246C;                     /* 0x246C cleared by Init_global (Init_global) */
    ULONG Unk2470;                     /* 0x2470 */
    ULONG Unk2474[4];                  /* 0x2474 */
    USHORT Unk2484;                    /* 0x2484 RE2 Ctpage/Cclut pair (Bg_draw saves it) */
    UCHAR Next_cut_no;                /* 0x2486 */
    UCHAR Em_num;                      /* 0x2487 number of enemy works to allocate (Enemy_work_set) */
    UCHAR pad2488[0x248C - 0x2488];
    ULONG Pl_be_flg;                   /* 0x248C Player.Be_flg (start of the player ENEMY_WORK) */
    UCHAR Pl_routine_0;                /* 0x2490 Player.Routine_0 */
    UCHAR Pl_routine_1;                /* 0x2491 Player.Routine_1 */
    UCHAR Pl_routine_2;                /* 0x2492 Player.Routine_2 */
    UCHAR pad2493[0x2495 - 0x2493];
    signed char Pl_nFloor;             /* 0x2495 Player.nFloor (Player at 0x248C?) */
    SHORT Pl_water;                    /* 0x2496 Player.Water */
    UCHAR pad2498[0x24C0 - 0x2498];
    LONG Pl_pos_x;                     /* 0x24C0 Player.Pos_x (VECTOR Pos) */
    LONG Pl_pos_y;                     /* 0x24C4 Player.Pos_y */
    LONG Pl_pos_z;                     /* 0x24C8 Player.Pos_z */
    SHORT Pl_old_x;                    /* 0x24CC Player.Old_x */
    SHORT Pl_old_y;                    /* 0x24CE Player.Old_y */
    SHORT Pl_old_z;                    /* 0x24D0 Player.Old_z */
    UCHAR Unk24D2;                     /* 0x24D2 Player +0x46 (Type high byte?) */
    UCHAR pad24D3;
    UCHAR Pl_tpage;                    /* 0x24D4 Player +0x48 texture page (func_80046C90) */
    UCHAR Pl_clut;                     /* 0x24D5 Player +0x49 clut row */
    UCHAR pad24D6[0x24FA - 0x24D6];
    SHORT Pl_cdir_y;                   /* 0x24FA Player.Cdir_y */
    UCHAR pad24FC[0x2558 - 0x24FC];
    SHORT Pl_life;                     /* 0x2558 Player.Life */
    UCHAR pad255A[0x255E - 0x255A];
    USHORT Pl_status_flg;              /* 0x255E Player.Status_flg */
    UCHAR pad2560[0x2594 - 0x2560];
    struct tagPARTS_W *Pl_pSin_parts_ptr; /* 0x2594 Player.pSin_parts_ptr */
    UCHAR pad2598[0x259C - 0x2598];
    struct tagENEMY_WORK *Pl_pEnemy_ptr;/* 0x259C Player.pEnemy_ptr */
    UCHAR pad25A0[0x25AE - 0x25A0];
    SHORT Unk25AE;                     /* 0x25AE Player +0x122, set from the door y (Set_door) */
    UCHAR pad25B0[0x25B9 - 0x25B0];
    UCHAR Pl_damage_cnt;               /* 0x25B9 Player.Damage_cnt */
    UCHAR pad25BA[0x260C - 0x25BA];
    ULONG Pl_unk180[4];                /* 0x260C Player.Unk180 (kan/seq args of func_80079690 / Joint_move) */
    UCHAR pad261C[0x2640 - 0x261C];
    ULONG Pl_unk1B4;                   /* 0x2640 Player +0x1B4 (weapon data offset 4, Pl_weapon_ch) */
    UCHAR pad2644[0x2658 - 0x2644];
    SHORT Pl_poison_timer;             /* 0x2658 RE2 Pl_poison_timer (Pl_move: -1 -> 30 and Life--) */
    UCHAR pad265A[0x265C - 0x265A];
    struct tagENEMY_WORK *pUnk265C;    /* 0x265C work processed before the enemy list (player?) */
    struct tagENEMY_WORK *pUnk2660;    /* 0x2660 work processed when Be_flg & 1 (sub player?) */
    struct tagENEMY_WORK *pEnemy[40];  /* 0x2664 RE2 pEnemy */
    struct tagENEMY_WORK **ppEnemy_end; /* 0x2704 RE2 ppEnemy_end (end of used pEnemy[]) */
    UCHAR pad2708[0x2904 - 0x2708];
    struct tagKAGE_WORK9 Kage9[5];     /* 0x2904 */
    struct tagKAGE_WORK Kage[50];      /* 0x2990 */
    void (*Em_routine[64])(struct tagENEMY_WORK *); /* 0x3DE0 enemy main routine by Id - 0x10 (Em_init_move) */
    void (*Unk3EE0[24])();             /* 0x3EE0 all set to Spl by Spl_set */
    UCHAR pad3F40[0x4328 - 0x3F40];
    UCHAR Ob_model[32][0x194];         /* 0x4328 RE2 Ob_model (OBJ_MODEL_WORK, 0x194 bytes each) */
    UCHAR pad75A8[0x75AC - 0x75A8];
    USHORT *pUnk75AC[11];              /* 0x75AC per object model flag word (func_800557A0) */
    UCHAR nSce_aot;                    /* 0x75D8 RE2 nSce_aot */
    signed char Unk75D9;               /* 0x75D9 -1 on room init (Sce_work_clr_set) */
    UCHAR Front_func_rtn;              /* 0x75DA RE2 Front_func_rtn */
    UCHAR Back_func_rtn;               /* 0x75DB RE2 Back_func_rtn */
    void (*Front_func)();              /* 0x75DC RE2 Front_func */
    void (*Back_func)();               /* 0x75E0 RE2 Back_func */
    UCHAR pad75E4[0x75E8 - 0x75E4];
    USHORT Unk75E8;                    /* 0x75E8 |= 1 when the player fires (pl_wXX_t2_move) */
    USHORT Unk75EA;                    /* 0x75EA */
    UCHAR pad75EC[0x77F4 - 0x75EC];
    ULONG Status_flg;                  /* 0x77F4 */
    UCHAR pad77F8[0x780B - 0x77F8];
    UCHAR Save_area;                   /* 0x780B RE2 G.SaveData.Save_area */
    UCHAR pad780C[0x780E - 0x780C];
    SHORT Sce_work[8];                 /* 0x780E RE2 G.SaveData.Sce_work (SCE For2/Switch; RE3 indexes it directly, size unknown) */
    UCHAR pad781E[0x783E - 0x781E];
    SHORT Unk783E;                     /* 0x783E overlay task no - 0x11 (Set_stage) */
    SHORT Unk7840;                     /* 0x7840 Room_no? (func_80052968) */
    SHORT Cut_no;                      /* 0x7842 RE2 G.SaveData.Cut_no */
    SHORT Cut_no_old;                  /* 0x7844 RE2 G.SaveData.Cut_no_old (-1 on init) */
    SHORT Room_no_old;                 /* 0x7846 RE2 G.SaveData.Room_no_old (-1 on init) */
    SHORT Unk7848;                     /* 0x7848 */
    SHORT Unk784A;                     /* 0x784A (Set_door) */
    SHORT Unk784C;                     /* 0x784C */
    UCHAR pad784E[0x7852 - 0x784E];
    SHORT Unk7852;                     /* 0x7852 player life restore value? */
    SHORT Unk7854;                     /* 0x7854 */
    SHORT Unk7856;                     /* 0x7856 */
    SHORT Unk7858;                     /* 0x7858 bg scroll y, -0xEF..0xEF (Swap_Cbuff) */
    UCHAR pad785A[0x785E - 0x785A];
    USHORT Unk785E;                    /* 0x785E counter (func_8003CEA0) */
    SHORT Unk7860;                     /* 0x7860 */
    SHORT Unk7862;                     /* 0x7862 */
    UCHAR pad7864[0x7888 - 0x7864];
    ULONG Unk7888;                     /* 0x7888 cleared on room init (Sce_work_clr_set) */
    ULONG Unk788C;                     /* 0x788C */
    UCHAR pad7890[0x7914 - 0x7890];
    ULONG Cut_flg[4];                  /* 0x7914 per-cut flag words, index Cut_no (Psp_trans) */
    UCHAR pad7924[0x799C - 0x7924];
    ULONG Unk799C[1];                  /* 0x799C flag bits (func_800557A0) */
    UCHAR pad79A0[0x7C7C - 0x79A0];
    UCHAR *pUnk7C7C;                   /* 0x7C7C item work? (Id, Num) pairs */
    UCHAR pad7C80[0x7C84 - 0x7C80];
    USHORT Unk7C84[1];                 /* 0x7C84 index from D_800A0FDC table */
    UCHAR pad7C86[0x7D90 - 0x7C86];
    ULONG Unk7D90[12];                 /* 0x7D90 copied from enemy work +0x1CC (func_80024C58) */
    struct {                           /* 0x7DC0 0xC-byte records, Unk7E20 used (func_80037A90) */
        UCHAR Unk0;
        UCHAR Unk1;
        UCHAR pad2[2];
        USHORT Unk4;
        USHORT Pos_x;
        USHORT Pos_y;
        USHORT Pos_z;
    } Unk7DC0[8];
    LONG Unk7E20;                      /* 0x7E20 */
    SHORT Unk7E24[8];                  /* 0x7E24 cleared by func_800528D4 */
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
    UCHAR pad75[0x78 - 0x75];
    SHORT Unk78;                       /* 0x78 ground height (func_800452E8) */
    UCHAR pad7A[0x7C - 0x7A];
    SHORT Unk7C;                       /* 0x7C (func_800455B4) */
    UCHAR pad7E[0x84 - 0x7E];
    UCHAR Unk84;                       /* 0x84 part number (Pl_weapon_ch) */
    UCHAR Unk85;                       /* 0x85 */
    signed char Unk86;                 /* 0x86 */
    UCHAR pad87;
    SHORT Unk88;                       /* 0x88 rotation x (func_8002B190) */
    SHORT Unk8A;                       /* 0x8A rotation y */
    SHORT Unk8C;                       /* 0x8C rotation z */
    SHORT Unk8E;                       /* 0x8E rotation speed z */
    SHORT Unk90;                       /* 0x90 */
    SHORT Unk92;                       /* 0x92 rotation speed y */
    SHORT Unk94;                       /* 0x94 */
    SHORT Unk96;                       /* 0x96 */
    ULONG *pUnk98;                     /* 0x98 per-part animation data (Sa_dat_set) */
    SHORT Size_x;                      /* 0x9C RE2 Size_x */
    SHORT Size_y;                      /* 0x9E RE2 Size_y */
    SHORT Size_z;                      /* 0xA0 RE2 Size_z */
    USHORT UnkA2;                      /* 0xA2 */
    struct tagPARTS_W *pOya_parts;     /* 0xA4 */
    UCHAR padA8[0xAE - 0xA8];
    SHORT UnkAE;                       /* 0xAE Pos_y base (func_80027338) */
    UCHAR padB0[0xB6 - 0xB0];
    SHORT UnkB6;                       /* 0xB6 Pos_y step, * rate >> 8 (func_80027338) */
    UCHAR padB8[0xBC - 0xB8];
} PARTS_W;

#endif /* BIO3_H */
