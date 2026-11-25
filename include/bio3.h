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
    LONG *pPos;                        /* 0x14 position followed (x, y, z longs) */
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
    LONG *pPos;                        /* 0x14 position followed (x, y, z longs) */
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

/* Object model work (RE2 struct tagOBJ_MODEL_WORK, OMA.H). Only proven fields. */
typedef struct tagOBJ_MODEL_WORK {
    ULONG Be_flg;                      /* 0x00 */
    UCHAR Routine_0;                   /* 0x04 */
    UCHAR Routine_1;                   /* 0x05 */
    UCHAR Routine_2;                   /* 0x06 */
    UCHAR Routine_3;                   /* 0x07 */
    UCHAR Parts_num;                   /* 0x08 */
    signed char nFloor;                /* 0x09 */
    UCHAR padA[0xD - 0xA];
    UCHAR UnkD;                        /* 0x0D G.pOt_sub index (Ot_set_all) */
    UCHAR padE[0x10 - 0xE];
    ULONG Attribute;                   /* 0x10 RE2 Attribute (Om_parts_link) */
    struct tagMD_OBJ *pTmd;            /* 0x14 RE2 pTmd (an MD object in RE3) */
    UCHAR pad18[0x20 - 0x18];
    MATRIX M;                          /* 0x20 M.t = Pos_x/y/z */
    SHORT Old_x;                       /* 0x40 */
    SHORT Old_y;                       /* 0x42 */
    SHORT Old_z;                       /* 0x44 */
    USHORT Type;                       /* 0x46 */
    UCHAR Tpage;                       /* 0x48 */
    UCHAR Unk49;                       /* 0x49 */
    UCHAR Unk4A;                       /* 0x4A Id; 1: packets at pPacket188 */
    UCHAR pad4B;
    MATRIX Workm;                      /* 0x4C */
    SVECTOR Cdir;                      /* 0x6C */
    ULONG Poly_rgb;                    /* 0x74 */
    MATRIX *pSuper;                    /* 0x78 */
    ULONG *pUnk7C;                     /* 0x7C ordering table (Ot_set_all) */
    UCHAR pad80[0xAE - 0x80];
    USHORT UnkAE;                      /* 0xAE 0x10: pushable, 0x20: keeps being pushed (Om_push_ck) */
    UCHAR padB0[0xBA - 0xB0];
    USHORT UnkBA;                      /* 0xBA */
    UCHAR padBC[0xC0 - 0xBC];
    UCHAR Push_cnt;                    /* 0xC0 frames pushed (Om_push_ck) */
    UCHAR UnkC1;                       /* 0xC1 index into G.pUnk75AC */
    UCHAR UnkC2;                       /* 0xC2 flag id set when done (Om_rtn_wait) */
    UCHAR UnkC3;                       /* 0xC3 0x80: kirakira effect, 0x60: its kind (Sce_kirakira_set) */
    struct tagPARTS_W *pParts;         /* 0xC4 */
    UCHAR padC8[0x130 - 0xC8];
    ULONG Unk130;                      /* 0x130 */
    UCHAR pad134[0x188 - 0x134];
    ULONG *pPacket188;                 /* 0x188 POLY_GT4 packets (Col_chg_init) */
    SHORT Unk18C;                      /* 0x18C fall speed (Om_rtn_drop) */
    SHORT Unk18E;                      /* 0x18E */
    SHORT Unk190;                      /* 0x190 */
    SHORT Unk192;                      /* 0x192 ground y */
} OBJ_MODEL_WORK;                      /* 0x194, G.Ob_model[] */

/* item slot (RE2 struct tagITEM_WORK, STATUS.H) */
typedef struct tagITEM_WORK {
    UCHAR Id;                          /* 0x0 */
    UCHAR Num;                         /* 0x1 */
    USHORT Flg;                        /* 0x2 RE2 Size, dummy */
} ITEM_WORK;

/* items of one character (G.Item_work[], 0x140 bytes) */
typedef struct tagITEM_SET {
    ITEM_WORK Item[10];                /* 0x000 inventory */
    ITEM_WORK Box[64];                 /* 0x028 item box */
    UCHAR Equip_no;                    /* 0x128 equipped slot (0xFF none), RE2 Equip_item_no */
    UCHAR Equip_id;                    /* 0x129 equipped item id, RE2 Equip_item_id */
    UCHAR Num;                         /* 0x12A inventory size (8 or 10) */
    UCHAR pad12B;
    USHORT Tama_mix_cnt[10];           /* 0x12C ammo combines per ammo kind (St_mix_select caps some at 6) */
} ITEM_SET;

/* enemy code bank in one overlay load slot (RE2 struct tagEM_BIN_INFO, EM_SET.H) */
typedef struct tagEM_BIN_INFO {
    USHORT Id;                         /* 0x0 enemy kind of the loaded bank */
    USHORT Use;                        /* 0x2 set once loaded */
} EM_BIN_INFO;

/* collision object: one node of the OBA.C collision list, embedded in every ENEMY_WORK at 0x80 (Col) */
typedef struct tagCOL_OBJ {
    UCHAR Id;                          /* 0x00 */
    UCHAR Flg;                         /* 0x01 */
    UCHAR Type;                        /* 0x02 0 = player, < 3 = character */
    UCHAR Unk3;                        /* 0x03 (RE2 At_em_no: 0xFF = touching no work) */
    struct tagENEMY_WORK *pEm;         /* 0x04 owner */
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
    SHORT Unk2C;                       /* 0x2C (em20 reads it with Hit_flg as one word, RE2 Sca_info) */
    USHORT Hit_flg;                    /* 0x2E 0x100: pushed along a slope */
    MATRIX *pM;                        /* 0x30 (em30 sub-works: matrix of the followed part) */
    struct tagCOL_OBJ *pNext;          /* 0x34 */
    UCHAR Em_idx;                      /* 0x38 */
    signed char Unk39;                 /* 0x39 (em10 pldm00 passes it to Em_tilt_set) */
    USHORT Unk3A;                      /* 0x3A bits of the works hit (RE2 At_em_flg, em10 pldm00) */
    struct tagSCA_DATA *pSca_hit_data; /* 0x3C RE2 pSca_hit_data (wall hit by Sca_ck_em) */
} COL_OBJ;

/* hit area: a box Top..Bottom high, radius R, around pM->t (ENEMY_WORK + 0x134, OM_TARGET + 0x8) */
typedef struct tagEM_HIT_AREA {
    SHORT Top;                         /* 0x00 y offset from pM->t[1] */
    SHORT Bottom;                      /* 0x02 y offset from pM->t[1] */
    UCHAR Rate0;                       /* 0x04 */
    UCHAR Rate1;                       /* 0x05 */
    SHORT R;                           /* 0x06 */
    MATRIX *pM;                        /* 0x08 matrix of the part the box follows */
} EM_HIT_AREA;

/* enemy work (RE2 struct tagENEMY_WORK; RE3 layout differs: no pTmd2/pPacket2, no dummy00).
 * The player is the first 0x1CC bytes of one (PLAYER_WORK, G.Player): EM_WORK_HEAD lists those
 * members once for both structs; code passes the player as (ENEMY_WORK *) like RE2 does. */
#define EM_WORK_HEAD \
    ULONG Be_flg;                      /* 0x000 */                            \
    UCHAR Routine_0;                   /* 0x004 */                            \
    UCHAR Routine_1;                   /* 0x005 */                            \
    UCHAR Routine_2;                   /* 0x006 */                            \
    UCHAR Routine_3;                   /* 0x007 */                            \
    UCHAR Parts_num;                   /* 0x008 RE3 moved it here (PartsWork_set) */ \
    signed char nFloor;                /* 0x009 RE2 nFloor (Pos_y / -1800) */ \
    SHORT Water;                       /* 0xA RE2 Water (Sce_Water) */        \
    UCHAR UnkC;                        /* 0x00C */                            \
    UCHAR UnkD;                        /* 0x00D */                            \
    UCHAR padE[0x10 - 0xE];                                                   \
    ULONG Attribute;                   /* 0x010 RE2 Attribute */              \
    ULONG *pTmd;                       /* 0x014 RE2 pTmd: MD file, then (+8) its objects, n_obj at [-1] */ \
    UCHAR pad18[0x20 - 0x18];                                                 \
    SHORT Workm_m[3][3];               /* 0x020 RE2 Workm.m (Workm.t is Pos_x..Pos_z) */ \
    UCHAR pad32[0x34 - 0x32];                                                 \
    LONG Pos_x;                        /* 0x034 */                            \
    LONG Pos_y;                        /* 0x038 */                            \
    LONG Pos_z;                        /* 0x03C */                            \
    SHORT Old_x;                       /* 0x040 RE2 Old_x: position of the last frame (Game_save_load) */ \
    SHORT Old_y;                       /* 0x042 */                            \
    SHORT Old_z;                       /* 0x044 */                            \
    USHORT Type;                       /* 0x046 low byte = player weapon (RE2 Type & 0xfff) */ \
    UCHAR Tpage;                       /* 0x048 RE2 Tpage */                  \
    UCHAR Clut;                        /* 0x049 RE2 Clut */                   \
    UCHAR Id;                          /* 0x04A enemy kind (0x10+ for enemies, Pl_Em_damage indexes by Id - 0x10) */ \
    UCHAR Unk4B;                       /* 0x4B */                             \
    MATRIX Super_matrix;               /* 0x04C RE2 Super_matrix (em10 Kahannsin_walk: lower half on its own) */ \
    SHORT Cdir_x;                      /* 0x06C */                            \
    SHORT Cdir_y;                      /* 0x06E */                            \
    SHORT Cdir_z;                      /* 0x070 */                            \
    USHORT dummy01;                    /* 0x072 */                            \
    ULONG Poly_rgb;                    /* 0x074 */                            \
    MATRIX *pSuper;                    /* 0x078 */                            \
    ULONG *pUnk7C;                     /* 0x07C ordering table of the work (Ot_set_all) */ \
    COL_OBJ Col;                       /* 0x080 collision node (OBA.C) */     \
    SHORT Spd_x;                       /* 0x0C0 */                            \
    SHORT Spd_y;                       /* 0x0C2 */                            \
    SHORT Spd_z;                       /* 0x0C4 */                            \
    UCHAR UnkC6;                       /* 0xC6 */                             \
    UCHAR UnkC7;                       /* 0xC7 set from the hit-area byte (Hit_area_em_ck) */ \
    UCHAR Move_no;                     /* 0x0C8 */                            \
    UCHAR Move_cnt;                    /* 0x0C9 */                            \
    UCHAR Hokan_flg;                   /* 0x0CA RE2 Hokan_flg: joint interpolation rate */ \
    UCHAR padCB;                                                              \
    SHORT Life;                        /* 0x0CC */                            \
    SHORT Max_life;                    /* 0x0CE */                            \
    SHORT Size;                        /* 0x0D0 RE2 Size (uniform scale, Be_flg 0x100000) */ \
    USHORT Status_flg;                 /* 0x0D2 player: 0x200 poisoned (Pl_move), 0x800 cleared by Pl_life_up */ \
    SHORT Dest_x;                      /* 0x0D4 target x (Goto00/Goto01 helpers) */ \
    SHORT Dest_z;                      /* 0x0D6 target z */                   \
    SHORT Timer0;                      /* 0x0D8 */                            \
    SHORT UnkDA;                       /* 0xDA */                             \
    SHORT Base_pos_x;                  /* 0x0DC */                            \
    SHORT Base_pos_y;                  /* 0x0DE */                            \
    SHORT Base_pos_z;                  /* 0x0E0 */                            \
    s8 UnkE2;                          /* 0xE2 */                             \
    signed char UnkE3;                 /* 0xE3 RE2 Timer3 (lb in em10) */     \
    USHORT *pNow_seq;                  /* 0x0E4 */                            \
    ULONG *pKan_t_ptr;                 /* 0x0E8 RE3 moved it here (RE2: after Parts_num) */ \
    ULONG *pSeq_t_ptr;                 /* 0x0EC */                            \
    ULONG *pSub0_kan_t_ptr;            /* 0x0F0 */                            \
    ULONG *pSub0_seq_t_ptr;            /* 0x0F4 */                            \
    ULONG *pSub1_kan_t_ptr;            /* 0x0F8 */                            \
    ULONG *pSub1_seq_t_ptr;            /* 0x0FC */                            \
    ULONG *pUnk100_kan_t_ptr;          /* 0x100 */                            \
    ULONG *pUnk104_seq_t_ptr;          /* 0x104 */                            \
    struct tagPARTS_W *pSin_parts_ptr; /* 0x108 */                            \
    ULONG *pUnk10C;                    /* 0x10C base kan of the part tree (PartsWork_link) */ \
    struct tagENEMY_WORK *pEnemy_ptr;  /* 0x110 */                            \
    struct tagENEMY_WORK *pUnk114;     /* 0x114 */                            \
    ULONG *pUnk118;                    /* 0x118 per-part animation header (Sa_dat_set) */ \
    ULONG *pUnk11C;                    /* 0x11C (Emd_load: first EMD section) */ \
    UCHAR Neck_flg;                    /* 0x120 RE2 Neck_flg: 1 no neck turn, 2 keep y, 0x80 scenario speed */ \
    UCHAR Neck_no;                     /* 0x121 RE2 Neck_no (part index used as the aim point) */ \
    SHORT Ground;                      /* 0x122 RE2 Ground: floor height under the work (Sca_get_ground) */ \
    SHORT Spd_base_x;                  /* 0x124 RE2 Spd_base_x (Spd_get) */   \
    SHORT Spd_base_y;                  /* 0x126 */                            \
    SHORT Spd_base_z;                  /* 0x128 */                            \
    SHORT Mot_scale_y;                 /* 0x12A root height scale of the motions, 0x1000 = 1 */ \
    UCHAR Damage_no;                   /* 0x12C RE2 Damage_no: kind of the last hit */ \
    UCHAR Damage_cnt;                  /* 0x12D player: |= 0x80 on game over (Pl_life_down) */ \
    UCHAR Root_ck_cnt;                 /* 0x12E RE2 Root_ck_cnt */            \
    UCHAR Unk12F;                      /* 0x12F compared with the room sound table (Snd_se_enem) */ \
    SHORT Unk130;                      /* 0x130 direction */                  \
    USHORT Damage_flg;                 /* 0x132 RE2 Damage_flg: hit flags (bits 8-12 cleared by Pl_life_down) */ \
    EM_HIT_AREA Hit_area;              /* 0x134 aim/hit box (pl_wep) */        \
    LONG Unk140;                       /* 0x140 */                            \
    USHORT Unk144;                     /* 0x144 0x100: no seq sound */        \
    UCHAR Set_flg_no;                  /* 0x146 RE2 Set_flg_no */             \
    UCHAR Em_set_flg;                  /* 0x147 RE2 Em_set_flg: kill flag number (Em_dead_flg_on_cand) */ \
    SHORT Add_dir;                     /* 0x148 turn speed passed to Goto00 */ \
    SHORT Unk14A;                      /* 0x14A */                            \
    SHORT Unk14C;                      /* 0x14C */                            \
    SHORT Unk14E;                      /* 0x14E */                            \
    USHORT Unk150;                     /* 0x150 0x80: scenario move request, low bits its kind (Spl_sce) */ \
    SHORT Unk152;                      /* 0x152 */                            \
    ULONG L_pl;                        /* 0x154 RE2 L_pl: distance to the player */ \
    LONG Unk158;                       /* 0x158 distance to target? (masked 0x3FFFFFFF) */ \
    LONG *pUnk15C;                     /* 0x15C target position (pl_mv0d) */ \
    union {                                                                   \
        struct tagKAGE_WORK *pKw;      /* 0x160 RE2 pKage_work: enemies (Kage_work_set) */ \
        struct tagKAGE_WORK9 *pKw9;    /* 0x160 player (Kage_work9_set) */    \
    } Kage;                                                                   \
    SHORT Unk164;                      /* 0x164 */                            \
    SHORT Unk166;                      /* 0x166 */                            \
    SHORT Unk168;                      /* 0x168 */                            \
    UCHAR Unk16A;                      /* 0x16A em22: set on a seq flag 0x800 (id 0x24) */ \
    UCHAR Unk16B;                      /* 0x16B */                            \
    LONG *pUnk16C;                     /* 0x16C hit position (Hit_area_em_ck arg 0) */ \
    LONG *pUnk170;                     /* 0x170 hitter position (Hit_area_em_ck arg 1) */ \
    struct tagMD_OBJ *pUnk174;         /* 0x174 model objects of the parts, part pTmd = &pUnk174[n] (em10 Init) */ \
    SHORT Add_col[3];                  /* 0x178 colour added in the lit sorts (Model_col); em10 Dead fades [0], [2] to 0 */ \
    USHORT Col_flg;                    /* 0x17E Model_col: bits 8-11 mode, low byte grey level */ \
    ULONG *pUnk180_seq_t_ptr;          /* 0x180 seq/kan of Leg_ik_kan (second part set) */ \
    ULONG *pUnk184_kan_t_ptr;          /* 0x184 */                            \
    ULONG *pUnk188_seq_t_ptr;          /* 0x188 seq/kan of Joint_move (first part set); a prim buffer in Mizu_trans works */ \
    ULONG *pUnk18C_kan_t_ptr;          /* 0x18C */                            \
    ULONG *pUnk190_seq_t_ptr;          /* 0x190 second set of sub animations (func_800498F0) */ \
    ULONG *pUnk194_kan_t_ptr;          /* 0x194 */                            \
    ULONG *pUnk198_seq_t_ptr;          /* 0x198 first set of sub animations (func_800498F0) */ \
    ULONG *pUnk19C_kan_t_ptr;          /* 0x19C */                            \
    UCHAR Sub_move_no;                 /* 0x1A0 second motion slot (slope walking; Spd_get_sub) */ \
    UCHAR Sub_move_cnt;                /* 0x1A1 */                            \
    UCHAR Sub_hokan_flg;               /* 0x1A2 (pl_mv05_w2_t2) */            \
    UCHAR pad1A3;                                                             \
    SHORT Unk1A4[3];                   /* 0x1A4 em23 damage: hit offset? */   \
    UCHAR pad1AA[0x1AC - 0x1AA];                                              \
    SHORT Unk1AC[3];                   /* 0x1AC em23 damage */                \
    UCHAR pad1B2[0x1B4 - 0x1B2];                                              \
    ULONG *pUnk1B4;                    /* 0x1B4 weapon data (G.Player.pUnk1B4, select overlay) */ \
    SHORT Unk1B8;                      /* 0x1B8 vertical speed added to Pos_y (Sca_set_ground) */ \
    SHORT Tilt_max;                    /* 0x1BA body tilt limit (Em_tilt_set) */ \
    SHORT Tilt[2];                     /* 0x1BC body tilt x, z (Joint_trans, Foot_set_pl) */ \
    UCHAR Unk1C0;                      /* 0x1C0 */                            \
    UCHAR Tilt_flg;                    /* 0x1C1 1/2/4: tilt direction, 8: return (Em_tilt_move) */ \
    SHORT Tilt_spd;                    /* 0x1C2 tilt step per frame */ \
    ULONG Old_routine;                 /* 0x1C4 Routine_0..3 of the last frame (em23) */ \
    SHORT Unk1C8;                      /* 0x1C8 */                            \
    USHORT Unk1CA;                     /* 0x1CA */                            \
    /* 0x1CC */

/* the player (RE2 struct tagPLAYER_WORK, G.Player at G + 0x248C, also the exe symbol Pl_work) */
typedef struct tagPLAYER_WORK {
    EM_WORK_HEAD
} PLAYER_WORK;

typedef struct tagENEMY_WORK {
    EM_WORK_HEAD
    ULONG Unk1CC[12];                  /* 0x1CC enemies only (the player has G.Pl_poison_timer here); copied to G.Unk7D90 (Chaser_data_save) */
} ENEMY_WORK;
STATIC_ASSERT(player_work_size, sizeof(PLAYER_WORK) == 0x1CC);
STATIC_ASSERT(enemy_work_size, sizeof(ENEMY_WORK) == 0x1FC);

/*
 * Vector views of field runs (gcc 2.8 has no anonymous unions, so these stay views).
 * Work types with Cdir_x/y/z + dummy01 (ENEMY_WORK, PARTS_W): CDIR_SV is that run as an SVECTOR;
 * with Pos_x/y/z (ENEMY_WORK world position, PARTS_W offset from the parent): POS_V as a VECTOR.
 * ENEMY_WORK: Spd_x/y/z + UnkC6/C7 as an SVECTOR, and Workm_m + Pos as
 * the work matrix (RE2 Workm). PARTS_W: M[3][3] + Pos_x/y/z is the local matrix, Workm.t the
 * world position as a VECTOR (the pad word after it is Cdir_x/y, never used by the callees).
 */
#define CDIR_SV(p)   ((SVECTOR *)&(p)->Cdir_x)
#define EM_SPD_SV(p) ((SVECTOR *)&(p)->Spd_x)
#define POS_V(p)  ((VECTOR *)&(p)->Pos_x)
#define EM_WORKM(p)  ((MATRIX *)(p)->Workm_m)
#define PW_M(p)      ((MATRIX *)(p)->M)
#define WORKM_POS_V(p) ((VECTOR *)(p)->Workm.t) /* world position (Workm.t) of a work or part */

/* G: global work area (RE2 struct tagGLOBAL, 0x800CC1E8 in RE2) */
struct tagROOM_WORK;
struct tagPARTS_W;
struct tagSCE_AOT;
typedef struct tagGLOBAL {
    ULONG Ot0[2][8];                   /* 0x0000 */
    ULONG *pOt0;                      /* 0x0040 */
    ULONG Ot1[2][0x400];               /* 0x0044 RE2 Ot1, index Cbuf_id */
    ULONG *pOt1;                       /* 0x2044 */
    ULONG Ot2[2][8];                   /* 0x2048 RE2 Ot2, index Cbuf_id (main) */
    ULONG *pOt2;                       /* 0x2088 */
    POLY_GT4 *pUnk208C;                /* 0x208C packet pointer of the model sorters (Parts_sort_ex) */
    ULONG *pOt_sub[2][4];              /* 0x2090 per-buffer OT tables, index Cbuf_id (Psp_trans) */
    UCHAR Padd_data[2][0x24];          /* 0x20B0 RE2 Padd_data (PadInitDirect buffers) */
    LONG Unk20F8;                      /* 0x20F8 set to -1 on soft reset */
    USHORT Unk20FC;                    /* 0x20FC */
    UCHAR pad20FE[0x2100 - 0x20FE];
    USHORT Unk2100;                    /* 0x2100 key repeat? (status menu cursor moves) */
    UCHAR pad2102[0x2104 - 0x2102];
    ULONG Key;                        /* 0x2104 RE2 Key (held buttons) */
    ULONG Key_trg;                     /* 0x2108 RE2 Key_trg (Gacha_ch) */
    ULONG Key_old;                     /* 0x210C RE2 Key_old (Pad_set) */
    UCHAR Cbuf_id;                     /* 0x2110 */
    UCHAR Unk2111;                     /* 0x2111 display buffer y != 0 (Swap_Cbuff) */
    UCHAR Pad_type;                    /* 0x2112 RE2 Pad_type1: PadInfoMode(0, 1, 0), 4 digital, 7 analog (Pad_set) */
    UCHAR Unk2113;                     /* 0x2113 set from the first TIM loaded by Esp_data_set1 */
    UCHAR Unk2114;                     /* 0x2114 cleared by Init_system */
    UCHAR Unk2115;                     /* 0x2115 routine of the RESULT overlay task (result: Main task loop) */
    UCHAR Unk2116;                     /* 0x2116 quake timer? (Quake_add) */
    UCHAR pad2117;
    SHORT Unk2118;                     /* 0x2118 */
    USHORT Score;                      /* 0x211A Mercenaries score (result) */
    UCHAR pad211C[0x2120 - 0x211C];
    ULONG System_flg;                  /* 0x2120 */
    USHORT *pBs_size;                  /* 0x2124 RE2 pBs_size: per room offset into pUnk2128 (Bg_draw) */
    USHORT *pUnk2128;                  /* 0x2128 bs file sizes */
    UCHAR *pBs_check_sum;              /* 0x212C RE2 pBs_check_sum */
    UCHAR pad2130[0x2134 - 0x2130];
    struct tagROOM_WORK *pRoom;        /* 0x2134 */
    ULONG Unk2138;                     /* 0x2138 free memory after the 0x8011A000 overlay (Set_stage) */
    ENEMY_WORK *pEm_mem;               /* 0x213C enemy work allocation pointer (Enemy_work_set) */
    ENEMY_WORK *pEm;                   /* 0x2140 current enemy (RE2 G.pEm) */
    struct tagVCUT *Cccut;             /* 0x2144 RE2 G.Cccut (current camera cut zone) */
    struct tagVCUT *Cccut_next;        /* 0x2148 */
    ULONG *pOld_tim;                   /* 0x214C RE2 pOld_tim (Sce_model_init) */
    ULONG *pOld_tmd;                   /* 0x2150 RE2 pOld_tmd */
    void *pSce_aot_on;                 /* 0x2154 RE2 pSce_aot_on (door AOT data) */
    struct tagSCE_AOT *Sce_aot[32];    /* 0x2158 RE2 Sce_aot (cleared by Sce_aot_init) */
    void *pAot_data;                   /* 0x21D8 RE2 pAot_data */
    void *pUnk21DC;                    /* 0x21DC data of the AOT being handled (sce_at, status item pickup) */
    ULONG Game_time_offset;            /* 0x21E0 RE2 Game_time_offset: VSync(-1) at the last play time update (mem_card Save_push) */
    USHORT *pXa_sector;                /* 0x21E4 RE2 G.pXa_sector (Xa_play table for the stage) */
    void (*pPldamage_Em[64])(); /* 0x21E8 RE2 pPldamage_Em, index enemy Id - 0x10 (Pl_Em_damage) */
    void (*pPldie_Em[64])(); /* 0x22E8 RE2 pPldie_Em (Pl_Em_die) */
    struct tagSCA_DATA *Sca_list[32];  /* 0x23E8 SCA shapes matching the last search (NULL terminated) */
    ULONG Unk2468;                     /* 0x2468 flags; 0x1000000 tested by pl_mv06_pad / Pl_move */
    LONG Unk246C;                      /* 0x246C flags, cleared by Init_global; < 0 blocks the pause (main) */
    ULONG Unk2470;                     /* 0x2470 */
    ULONG Unk2474[4];                  /* 0x2474 */
    union {
        USHORT Tpage_clut;             /* 0x2484 both bytes as one halfword (Bg_draw saves it) */
        struct {
            UCHAR Ctpage;              /* 0x2484 RE2 Ctpage: next free texture page x */
            UCHAR Cclut;               /* 0x2485 RE2 Cclut: next free clut row */
        } u;
    } Ctp;
    UCHAR Next_cut_no;                /* 0x2486 */
    UCHAR Em_num;                      /* 0x2487 number of enemy works to allocate (Enemy_work_set) */
    UCHAR Unk2488;                     /* 0x2488 non-zero: no vibration (RE2 G.Dummy8_3[0], Pad_set) */
    UCHAR Save_slot;                   /* 0x2489 last memory card slot used (mem_card) */
    UCHAR pad248A[0x248C - 0x248A];
    PLAYER_WORK Player;                /* 0x248C RE2 Player (= Pl_work) */
    SHORT Pl_poison_timer;             /* 0x2658 RE2 Pl_poison_timer (Pl_move: -1 -> 30 and Life--) */
    UCHAR pad265A[0x265C - 0x265A];
    ENEMY_WORK *pEnemy[42];            /* 0x265C RE2 pEnemy: [0] player, [1] sub player, [2..] enemies */
    ENEMY_WORK **ppEnemy_end;          /* 0x2704 RE2 ppEnemy_end (end of used pEnemy[]) */
    ENEMY_WORK Sub_work;               /* 0x2708 spare enemy work: every free pEnemy[] slot points here (Enemy_work_set) */
    struct tagKAGE_WORK9 Kage9[5];     /* 0x2904 */
    struct tagKAGE_WORK Kage[50];      /* 0x2990 */
    void (*Em_routine[64])(ENEMY_WORK *); /* 0x3DE0 enemy main routine by Id - 0x10 (Em_init_move) */
    void (*Unk3EE0[24])(); /* 0x3EE0 all set to Spl by Spl_set */
    UCHAR pad3F40[0x3FA0 - 0x3F40];
    void (*Em_ov_mtbl[2][112])(ENEMY_WORK *); /* 0x3FA0 RE2 Em_ov_mtbl: move routine by Id - 0x10 inside each enemy overlay bank (Em_move_tbl_set) */
    EM_BIN_INFO Em_bin_info[2];        /* 0x4320 RE2 Em_bin_info: enemy code bank per load slot (Id 0xFF on room load) */
    OBJ_MODEL_WORK Ob_model[32];       /* 0x4328 RE2 Ob_model */
    OBJ_MODEL_WORK *pObj_work_end;     /* 0x75A8 RE2 pObj_work_end (end of the used Ob_model[]) */
    USHORT *pUnk75AC[10];             /* 0x75AC per object model flag word (Om_rev); OM_TARGET records (pl_wep.h) */
    ULONG Om_target_num;               /* 0x75D4 used entries of pUnk75AC (Obj_model_set; sce0 clears it as pUnk75AC[10]) */
    UCHAR nSce_aot;                    /* 0x75D8 RE2 nSce_aot */
    signed char Unk75D9;               /* 0x75D9 -1 on room init (Sce_work_clr_set) */
    UCHAR Front_func_rtn;              /* 0x75DA RE2 Front_func_rtn */
    UCHAR Back_func_rtn;               /* 0x75DB RE2 Back_func_rtn */
    void (*Front_func)(); /* 0x75DC RE2 Front_func */
    void (*Back_func)(); /* 0x75E0 RE2 Back_func */
    SHORT Unk75E4;                     /* 0x75E4 screen shake x? (psp Rain_wind_move) */
    SHORT Unk75E6;                     /* 0x75E6 screen shake y? */
    USHORT Unk75E8;                   /* 0x75E8 |= 1 when the player fires (pl_wXX_t2_move) */
    USHORT Unk75EA;                    /* 0x75EA */
    UCHAR Unk75EC;                     /* 0x75EC cleared on exit by the PC_SYS overlay */
    UCHAR Unk75ED;                     /* 0x75ED routine of the PC_SYS overlay task */
    UCHAR Unk75EE;                     /* 0x75EE PC_SYS window open/close request (1..4) */
    UCHAR Unk75EF;                     /* 0x75EF PC_SYS window zoom direction (0 open, 1 close) */
    UCHAR Card_head[0x200];            /* 0x75F0 save image start: memory card file header (mem_card writes 0x980 bytes from here) */
    ULONG Game_time;                   /* 0x77F0 play time in frames (result grade) */
    ULONG Status_flg;                  /* 0x77F4 */
    ULONG Unk77F8[1];                  /* 0x77F8 flag bits (result shop items bought; bit 0x1F tested by mem_card) */
    SHORT Save_cdir_y;                 /* 0x77FC player state saved by mem_card Save_push */
    SHORT Save_pos_x;                  /* 0x77FE */
    SHORT Save_pos_y;                  /* 0x7800 */
    SHORT Save_pos_z;                  /* 0x7802 */
    SHORT Save_life;                   /* 0x7804 */
    USHORT Unk7806;                    /* 0x7806 */
    UCHAR Save_cnt;                    /* 0x7808 number of saves; overwritten with the grade (result) */
    UCHAR Save_pl_id;                  /* 0x7809 Pl_id (mem_card Save_push) */
    UCHAR Save_poison;                 /* 0x780A 0x80 | poison timer (mem_card Save_push) */
    UCHAR Save_area;                   /* 0x780B RE2 G.SaveData.Save_area */
    UCHAR Save_unk780C;                /* 0x780C Main.Unk195 (mem_card Save_push) */
    UCHAR Save_unk780D;                /* 0x780D Main.Unk196 */
    SHORT Sce_work[8];                 /* 0x780E RE2 G.SaveData.Sce_work (SCE For2/Switch; RE3 indexes it directly, size unknown) */
    UCHAR pad781E[0x7826 - 0x781E];
    SHORT DSce_work[4];                /* 0x7826 RE2 G.SaveData.DSce_work (door type, set by Door_init) */
    UCHAR pad782E[0x783E - 0x782E];
    SHORT Unk783E;                     /* 0x783E overlay task no - 0x11 (Set_stage) */
    SHORT Unk7840;                     /* 0x7840 Room_no? (Chaser_room_ck) */
    SHORT Cut_no;                      /* 0x7842 RE2 G.SaveData.Cut_no */
    SHORT Cut_no_old;                  /* 0x7844 RE2 G.SaveData.Cut_no_old (-1 on init) */
    SHORT Room_no_old;                 /* 0x7846 RE2 G.SaveData.Room_no_old (-1 on init) */
    SHORT Unk7848;                     /* 0x7848 */
    SHORT Unk784A;                     /* 0x784A (Set_door) */
    SHORT Unk784C;                     /* 0x784C */
    SHORT Unk784E;                     /* 0x784E player Id chosen in the select overlay */
    USHORT Unk7850;                    /* 0x7850 sub player Status_flg kept across rooms (Spl_Init) */
    SHORT Unk7852;                     /* 0x7852 player life restore value? */
    SHORT Unk7854;                     /* 0x7854 */
    SHORT Unk7856;                     /* 0x7856 */
    SHORT Unk7858;                     /* 0x7858 bg scroll y, -0xEF..0xEF (Swap_Cbuff) */
    UCHAR pad785A[0x785C - 0x785A];
    SHORT Unk785C;                     /* 0x785C cleared by em36 (Nemesis) death */
    USHORT Unk785E;                    /* 0x785E counter (pl_mv0d) */
    USHORT Time;                       /* 0x7860 Mercenaries time left (result) */
    SHORT Unk7862;                     /* 0x7862 */
    SHORT Unk7864;                     /* 0x7864 set with Unk784E by the title extra menu */
    UCHAR pad7866[0x7868 - 0x7866];
    ULONG Unk7868[8];                  /* 0x7868 flag bits (Flg_ck, status room/cut checks) */
    ULONG Unk7888;                     /* 0x7888 cleared on room init (Sce_work_clr_set) */
    ULONG Unk788C;                     /* 0x788C */
    ULONG Unk7890[8];                  /* 0x7890 flag bits (Map_door_mark_ck) */
    ULONG Unk78B0[8];                  /* 0x78B0 flag bits (Map_door_mark_ck) */
    ULONG Unk78D0[8];                  /* 0x78D0 flag bits (Map_door_disp_ck, St_file_end) */
    ULONG Unk78F0[8];                  /* 0x78F0 flag bits (Map_door_disp_ck, St_file_end) */
    ULONG Unk7910[1];                  /* 0x7910 item flags (status: Flg_on with Item_data_tbl[id][2]) */
    ULONG Cut_flg[4];                  /* 0x7914 per-cut flag words, index Cut_no (Psp_trans) */
    UCHAR pad7924[0x7994 - 0x7924];
    ULONG Unk7994[2];                  /* 0x7994 flag bits (Map_door_mark_ck) */
    ULONG Unk799C[1];                  /* 0x799C flag bits (Om_rev) */
    UCHAR pad79A0[0x79A4 - 0x79A0];
    LONG Unk79A4[14];                  /* 0x79A4 (status: -1 on init) */
    ULONG Unk79DC[4];                  /* 0x79DC */
    ULONG Unk79EC[4];                  /* 0x79EC flag bits, one per stage (Map_door_disp_ck) */
    ITEM_SET Item_work[2];             /* 0x79FC per character, pItem_set points to one (mem_card Load_pop) */
    ITEM_SET *pItem_set;               /* 0x7C7C items of the current character */
    ULONG Item_work_no;                /* 0x7C80 index into Item_work (mem_card Load_pop) */
    USHORT Unk7C84[1];                 /* 0x7C84 index from Bgm_room_ofs table */
    UCHAR pad7C86[0x7D8C - 0x7C86];
    UCHAR Save_vol;                    /* 0x7D8C Snd_work.Vol.Se (mem_card Save_push) */
    UCHAR Save_vol2;                   /* 0x7D8D Snd_work.Vol.Bgm */
    UCHAR Save_mono;                   /* 0x7D8E Snd_work.Mono */
    UCHAR pad7D8F;
    ULONG Unk7D90[12];                 /* 0x7D90 copied from enemy work +0x1CC (Chaser_data_save) */
    struct {                           /* 0x7DC0 0xC-byte records, Unk7E20 used (Om_drop_set) */
        UCHAR Unk0;
        UCHAR Unk1;
        UCHAR pad2[2];
        USHORT Unk4;
        USHORT Pos_x;
        USHORT Pos_y;
        USHORT Pos_z;
    } Unk7DC0[8];
    LONG Unk7E20;                      /* 0x7E20 */
    SHORT Unk7E24[8];                  /* 0x7E24 cleared by Chaser_room_clr */
    UCHAR pad7E34[0x7E94 - 0x7E34];
    LONG Unk7E94;                      /* 0x7E94 total life healed? (status) */
    LONG Merc_time[3];                 /* 0x7E98 Mercenaries best time per character, -1 none (kept by Keep_data_save) */
    UCHAR Merc_grade[3];               /* 0x7EA4 best grade per character, 0xFF none */
    UCHAR pad7EA7;
    ULONG Merc_money[3];               /* 0x7EA8 best money per character */
    ULONG Money;                       /* 0x7EB4 money for the shop */
} tagGLOBAL;
STATIC_ASSERT(g_player_ofs, (ULONG)&((tagGLOBAL *)0)->Player == 0x248C);
STATIC_ASSERT(g_penemy_ofs, (ULONG)&((tagGLOBAL *)0)->pEnemy == 0x265C);
extern tagGLOBAL G;                    /* 0x800CA738 */

/* the player as the ENEMY_WORK the code passes around (RE2 writes (ENEMY_WORK *)&G.Player);
   PLAYER_EM(pG) for code that addresses G through a local pointer */
#define PLAYER_EM(pG) ((ENEMY_WORK *)&(pG)->Player)
#define G_PLAYER PLAYER_EM(&G)

/* G.System_flg bits */
#define SYS_SPAD_STACK 0x4000000

/* G.Status_flg bits */
#define STAT_XA_VOICE 0x20

/* model part (RE2 struct tagPARTS_W, JOINT.H; RE3 has no pTmd2/pPacket2/dummy00, 0xBC bytes) */
typedef struct tagPARTS_W {
    ULONG Be_flg;                      /* 0x00 */
    ULONG Attribute;                   /* 0x04 */
    struct tagMD_OBJ *pTmd;            /* 0x08 RE2 pTmd: the part's model object (md_sub.h) */
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
    SHORT Unk78;                       /* 0x78 ground height (Pl_wep_parts_drop) */
    UCHAR pad7A[0x7C - 0x7A];
    SHORT Unk7C;                       /* 0x7C (Pl_wep_eff_parts_set) */
    UCHAR pad7E[0x82 - 0x7E];
    SHORT Unk82;                       /* 0x82 em30: index of the collision sub-work on this part (Em_work_no_get) */
    UCHAR Parts_no;                    /* 0x84 RE2 Parts_no: index in pSin_parts_ptr[] */
    signed char Timer1;                /* 0x85 RE2 Timer1: fall speed added to Old_y (Parts_down) */
    signed char Timer2;                /* 0x86 RE2 Timer2: frames left to blend into the fall rotation (Parts_down) */
    UCHAR pad87;
    SHORT Rot_x;                       /* 0x88 RE2 Rot_x: rotation of the current key frame */
    SHORT Rot_y;                       /* 0x8A */
    SHORT Rot_z;                       /* 0x8C */
    SHORT Rot_spd_z;                   /* 0x8E rotation step to the next key frame, * rate >> 8 */
    SHORT Rot_spd_x;                   /* 0x90 */
    SHORT Rot_spd_y;                   /* 0x92 */
    SHORT Unk94;                       /* 0x94 */
    SHORT Timer3;                      /* 0x96 RE2 Timer3: bounces left (Parts_down) */
    ULONG *pSa_dat_head;               /* 0x98 RE2 pSa_dat_head: per-part animation data (Sa_dat_set) */
    SHORT Size_x;                      /* 0x9C RE2 Size_x */
    SHORT Size_y;                      /* 0x9E RE2 Size_y */
    SHORT Size_z;                      /* 0xA0 RE2 Size_z */
    SHORT UnkA2;                       /* 0xA2 flags; signed (em10 masks it with a register -2) */
    struct tagPARTS_W *pOya_parts;     /* 0xA4 */
    ULONG *pUnkA8;                     /* 0xA8 extra sort data: [0] two USHORT offsets, [1] offset (Skin_set, Skin_sort) */
    SHORT Pos_base_x;                  /* 0xAC root position of the current key frame (get_null_pos2) */
    SHORT Pos_base_y;                  /* 0xAE */
    SHORT Pos_base_z;                  /* 0xB0 */
    SHORT Pos_step_z;                  /* 0xB2 root step to the next key frame, * rate >> 8 */
    SHORT Pos_step_x;                  /* 0xB4 */
    SHORT Pos_step_y;                  /* 0xB6 */
    ULONG UnkB8;                       /* 0xB8 sort parameter (Parts_sort_ex) */
} PARTS_W;

#endif /* BIO3_H */
