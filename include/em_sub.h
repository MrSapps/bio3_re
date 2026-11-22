#ifndef EM_SUB_H
#define EM_SUB_H
/*
 * RE2 EM_SUB.C / EM_SUB.H: enemy helper routines (movement, direction checks,
 * shadows, neck rotation, ...). RE3 layouts were taken from the RE3 code.
 */
#include "bio3.h"
#include "esp.h"

typedef void (*EFF_PROC)(ESP_WORK *pEw);

/* 0xC-byte table entry, 16 entries at D_800BA728 / D_800BA7E8 (filled by Esp_data_set0) */
typedef struct {
    ULONG *pData;                      /* 0x0 */
    ULONG *pData2;                     /* 0x4 */
    UCHAR Id;                          /* 0x8 */
    UCHAR pad9[3];
} TBL12;

/* enemy work (RE2 struct tagENEMY_WORK; RE3 layout differs: no pTmd2/pPacket2, no dummy00) */
typedef struct tagENEMY_WORK {
    ULONG Be_flg;                      /* 0x000 */
    UCHAR Routine_0;                   /* 0x004 */
    UCHAR Routine_1;                   /* 0x005 */
    UCHAR Routine_2;                   /* 0x006 */
    UCHAR Routine_3;                   /* 0x007 */
    UCHAR Parts_num;                   /* 0x008 RE3 moved it here (PartsWork_set) */
    signed char nFloor;                /* 0x009 RE2 nFloor (Pos_y / -1800) */
    SHORT Water;                       /* 0xA RE2 Water (Sce_Water) */
    UCHAR UnkC;                        /* 0x00C */
    UCHAR UnkD;                        /* 0x00D */
    UCHAR padE[0x10 - 0xE];
    ULONG Attribute;                   /* 0x010 RE2 Attribute */
    ULONG *pTmd;                       /* 0x014 byte at [-4] = part count */
    UCHAR pad18[0x20 - 0x18];
    SHORT Workm_m[3][3];               /* 0x020 RE2 Workm.m (Workm.t is Pos_x..Pos_z) */
    UCHAR pad32[0x34 - 0x32];
    LONG Pos_x;                        /* 0x034 */
    LONG Pos_y;                        /* 0x038 */
    LONG Pos_z;                        /* 0x03C */
    SHORT Unk40[3];                    /* 0x040 copy of the position (func_80024B34) */
    USHORT Type;                       /* 0x046 low byte = player weapon (RE2 Type & 0xfff) */
    UCHAR Tpage;                       /* 0x048 RE2 Tpage */
    UCHAR Clut;                        /* 0x049 RE2 Clut */
    UCHAR Id;                          /* 0x04A enemy kind (0x10+ for enemies, Pl_Em_damage indexes by Id - 0x10) */
    UCHAR Unk4B;                       /* 0x4B */
    MATRIX Super_matrix;               /* 0x04C RE2 Super_matrix (em10 Kahannsin_walk: lower half on its own) */
    SHORT Cdir_x;                      /* 0x06C */
    SHORT Cdir_y;                      /* 0x06E */
    SHORT Cdir_z;                      /* 0x070 */
    USHORT dummy01;                    /* 0x072 */
    ULONG Poly_rgb;                    /* 0x074 */
    MATRIX *pSuper;                    /* 0x078 */
    ULONG *pUnk7C;                     /* 0x07C ordering table of the work (func_80037C38) */
    UCHAR Col_id;                      /* 0x080 collision object node (COL_OBJ, oba.h) at 0x80: Id */
    UCHAR Col_flg;                     /* 0x081 COL_OBJ.Flg */
    UCHAR Col_type;                    /* 0x082 COL_OBJ.Type */
    UCHAR Col_unk3;                    /* 0x083 COL_OBJ.Unk3 (RE2 At_em_no: 0xFF = touching no work) */
    struct tagENEMY_WORK *pCol_em;    /* 0x084 COL_OBJ.pEm */
    LONG Col_pos_x;                    /* 0x088 COL_OBJ.Pos_x */
    LONG Col_pos_y;                    /* 0x08C COL_OBJ.Pos_y */
    LONG Col_pos_z;                    /* 0x090 COL_OBJ.Pos_z */
    UCHAR pad94[0x9A - 0x94];
    USHORT Col_w0;                     /* 0x09A COL_OBJ.W0 */
    USHORT Col_d0;                     /* 0x09C COL_OBJ.D0 */
    USHORT Col_h;                      /* 0x09E COL_OBJ.H */
    UCHAR padA0[0xA4 - 0xA0];
    SHORT Col_unk24;                   /* 0x0A4 COL_OBJ.Unk24 */
    SHORT Col_unk26;                   /* 0x0A6 COL_OBJ.Unk26 */
    SHORT Col_unk28;                   /* 0x0A8 COL_OBJ.Unk28 */
    UCHAR padAA[0xAC - 0xAA];
    SHORT Col_unk2C;                   /* 0x0AC COL_OBJ.Unk2C (em20 reads it with UnkAE as one word, RE2 Sca_info) */
    USHORT UnkAE;                      /* 0x0AE */
    MATRIX *pCol_m;                    /* 0x0B0 COL_OBJ.pM (em30 sub-works: matrix of the followed part) */
    UCHAR padB4[0xB9 - 0xB4];
    signed char Col_unk39;            /* 0x0B9 COL_OBJ.Unk39 (em10 pldm00 passes it to func_8002646C) */
    USHORT UnkBA;                      /* 0xBA COL_OBJ.Unk3A: bits of the works hit (RE2 At_em_flg, em10 pldm00) */
    struct tagSCA_DATA *pSca_hit_data; /* 0x0BC RE2 pSca_hit_data (wall hit by Sca_ck_em) */
    SHORT Spd_x;                       /* 0x0C0 */
    SHORT Spd_y;                       /* 0x0C2 */
    SHORT Spd_z;                       /* 0x0C4 */
    UCHAR UnkC6;                       /* 0xC6 */
    UCHAR UnkC7;                       /* 0xC7 set from the hit-area byte (func_800445C8) */
    UCHAR Move_no;                     /* 0x0C8 */
    UCHAR Move_cnt;                    /* 0x0C9 */
    UCHAR UnkCA;                       /* 0x0CA joint interpolation rate (func_8002B190) */
    UCHAR padCB;
    SHORT Life;                        /* 0x0CC */
    SHORT Max_life;                    /* 0x0CE */
    SHORT Size;                        /* 0x0D0 RE2 Size (uniform scale, Be_flg 0x100000) */
    USHORT Status_flg;                 /* 0x0D2 player: 0x200 poisoned (Pl_move), 0x800 cleared by Pl_life_up */
    SHORT Dest_x;                      /* 0x0D4 target x (Goto00/Goto01 helpers) */
    SHORT Dest_z;                      /* 0x0D6 target z */
    SHORT Timer0;                      /* 0x0D8 */
    SHORT UnkDA;                       /* 0xDA */
    SHORT Base_pos_x;                  /* 0x0DC */
    SHORT Base_pos_y;                  /* 0x0DE */
    SHORT Base_pos_z;                  /* 0x0E0 */
    s8 UnkE2;                          /* 0xE2 */
    signed char UnkE3;                 /* 0xE3 RE2 Timer3 (lb in em10) */
    USHORT *pNow_seq;                  /* 0x0E4 */
    ULONG *pKan_t_ptr;                 /* 0x0E8 RE3 moved it here (RE2: after Parts_num) */
    ULONG *pSeq_t_ptr;                 /* 0x0EC */
    ULONG *pSub0_kan_t_ptr;            /* 0x0F0 */
    ULONG *pSub0_seq_t_ptr;            /* 0x0F4 */
    ULONG *pSub1_kan_t_ptr;            /* 0x0F8 */
    ULONG *pSub1_seq_t_ptr;            /* 0x0FC */
    ULONG *pUnk100_kan_t_ptr;          /* 0x100 */
    ULONG *pUnk104_seq_t_ptr;          /* 0x104 */
    struct tagPARTS_W *pSin_parts_ptr; /* 0x108 */
    ULONG *pUnk10C;                    /* 0x10C base kan of the part tree (func_800261AC) */
    struct tagENEMY_WORK *pEnemy_ptr;  /* 0x110 */
    struct tagENEMY_WORK *pUnk114;     /* 0x114 */
    ULONG *pUnk118;                    /* 0x118 per-part animation header (Sa_dat_set) */
    ULONG *pUnk11C;                    /* 0x11C (func_8001760C: first EMD section) */
    UCHAR Unk120;                      /* 0x120 */
    UCHAR Neck_no;                     /* 0x121 RE2 Neck_no (part index used as the aim point) */
    SHORT Unk122;                      /* 0x122 */
    SHORT Spd_base_x;                  /* 0x124 RE2 Spd_base_x (Spd_get) */
    SHORT Spd_base_y;                  /* 0x126 */
    SHORT Spd_base_z;                  /* 0x128 */
    SHORT Unk12A;                      /* 0x12A */
    UCHAR Unk12C;                      /* 0x12C */
    UCHAR Damage_cnt;                  /* 0x12D player: |= 0x80 on game over (Pl_life_down) */
    UCHAR Root_ck_cnt;                 /* 0x12E RE2 Root_ck_cnt */
    UCHAR Unk12F;                      /* 0x12F compared with the room sound table (Snd_se_enem) */
    SHORT Unk130;                      /* 0x130 direction */
    USHORT Unk132;                     /* 0x132 bits 8-12 cleared by Pl_life_down */
    SHORT Unk134;                      /* 0x134 */
    SHORT Unk136;                      /* 0x136 */
    UCHAR Unk138;                      /* 0x138 */
    UCHAR Unk139;                      /* 0x139 */
    SHORT Unk13A;                      /* 0x13A */
    UCHAR *pUnk13C;                    /* 0x13C */
    LONG Unk140;                       /* 0x140 */
    USHORT Unk144;                     /* 0x144 0x100: no seq sound */
    UCHAR Unk146;                      /* 0x146 RE2 Set_flg_no */
    UCHAR Unk147;                      /* 0x147 flag no (Em_dead_flg_on_cand) */
    SHORT Add_dir;                     /* 0x148 turn speed passed to Goto00 */
    SHORT Unk14A;                      /* 0x14A */
    SHORT Unk14C;                      /* 0x14C */
    SHORT Unk14E;                      /* 0x14E */
    USHORT Unk150;                     /* 0x150 0x80: scenario move request, low bits its kind (Spl_sce) */
    SHORT Unk152;                      /* 0x152 */
    ULONG Unk154;                      /* 0x154 */
    LONG Unk158;                       /* 0x158 distance to target? (masked 0x3FFFFFFF) */
    LONG *pUnk15C;                     /* 0x15C target position (func_8003CEA0) */
    union {
        struct tagKAGE_WORK *pKw;      /* 0x160 RE2 pKage_work: enemies (Kage_work_set) */
        struct tagKAGE_WORK9 *pKw9;    /* 0x160 player (Kage_work9_set) */
    } Kage;
    SHORT Unk164;                      /* 0x164 */
    SHORT Unk166;                      /* 0x166 */
    SHORT Unk168;                      /* 0x168 */
    UCHAR Unk16A;                      /* 0x16A em22: set on a seq flag 0x800 (id 0x24) */
    UCHAR Unk16B;                      /* 0x16B */
    LONG *pUnk16C;                     /* 0x16C hit position (func_800445C8 arg 0) */
    LONG *pUnk170;                     /* 0x170 hitter position (func_800445C8 arg 1) */
    ULONG (*pUnk174)[6];               /* 0x174 0x18-byte model data records, part pTmd = pUnk174[n] (em10 Init) */
    SHORT Unk178[3];                   /* 0x178 copied to D_80098B08 (func_800254AC); em10 Dead fades [0], [2] to 0 */
    USHORT Unk17E;                     /* 0x17E */
    ULONG *pUnk180_seq_t_ptr;          /* 0x180 seq/kan of func_80079690 (second part set) */
    ULONG *pUnk184_kan_t_ptr;          /* 0x184 */
    ULONG *pUnk188_seq_t_ptr;          /* 0x188 seq/kan of Joint_move (first part set); a prim buffer in func_8002E74C works */
    ULONG *pUnk18C_kan_t_ptr;          /* 0x18C */
    ULONG *pUnk190_seq_t_ptr;          /* 0x190 second set of sub animations (func_800498F0) */
    ULONG *pUnk194_kan_t_ptr;          /* 0x194 */
    ULONG *pUnk198_seq_t_ptr;          /* 0x198 first set of sub animations (func_800498F0) */
    ULONG *pUnk19C_kan_t_ptr;          /* 0x19C */
    UCHAR Unk1A0;                      /* 0x1A0 */
    UCHAR Unk1A1;                      /* 0x1A1 */
    UCHAR Unk1A2;                      /* 0x1A2 (pl_mv05_w2_t2) */
    UCHAR pad1A3;
    SHORT Unk1A4[3];                   /* 0x1A4 em23 damage: hit offset? */
    UCHAR pad1AA[0x1AC - 0x1AA];
    SHORT Unk1AC[3];                   /* 0x1AC em23 damage */
    UCHAR pad1B2[0x1B4 - 0x1B2];
    ULONG *pUnk1B4;                    /* 0x1B4 weapon data (G.Pl_unk1B4, select overlay) */
    SHORT Unk1B8;                      /* 0x1B8 vertical speed added to Pos_y (func_8004B7E8) */
    SHORT Unk1BA;                      /* 0x1BA func_8002646C */
    SHORT Unk1BC[2];                   /* 0x1BC decayed by Unk1C2 (func_80026648) */
    UCHAR Unk1C0;                      /* 0x1C0 */
    UCHAR Unk1C1;                      /* 0x1C1 func_8002646C */
    SHORT Unk1C2;                      /* 0x1C2 func_8002646C */
    ULONG Unk1C4;                      /* 0x1C4 em23: Routine_0..3 of the last frame */
    SHORT Unk1C8;                      /* 0x1C8 */
    USHORT Unk1CA;                     /* 0x1CA */
    ULONG Unk1CC[12];                  /* 0x1CC copied to G.Unk7D90 (func_80024C58) */
} ENEMY_WORK;

/* animation table header at Kan (RE2 KAN_HEAD, JOINT.H) */
typedef struct tagKAN_HEAD {
    USHORT dummy00;                    /* 0x0 */
    USHORT Bone_ofs;                   /* 0x2 */
    ULONG Rec_sz;                      /* 0x4 record size in words << 18 */
} KAN_HEAD;

extern MATRIX GsIDMATRIX;              /* 0x80098970 */

/* Be_flg bits */
#define BE_FLG_400 0x400

ULONG Hani_ck(VECTOR *pV, SHORT *pPxzhw);
void Add_speedXZ(ENEMY_WORK *pEm, LONG muki);
void Add_speedXYZ(ENEMY_WORK *pEm, LONG muki_y, LONG muki_z);
void Get_speedXZ(ENEMY_WORK *pEm, LONG muki, SHORT *pX, SHORT *pZ);
void Add_speedXYZsuper(ENEMY_WORK *pEm, LONG muki_y, LONG muki_z);
LONG Direction_ck(LONG Hontai_x, LONG Hontai_z, LONG Target_x, LONG Target_z);
void Goto00(ENEMY_WORK *pEm, LONG Vec_x, LONG Vec_z, LONG Add_dir);
LONG Goto00_ck(ENEMY_WORK *pEm, LONG Vec_x, LONG Vec_z, LONG Add_dir);
void Goto01(ENEMY_WORK *pEm, LONG Dir, LONG Add_dir);
LONG Goto01_ck(ENEMY_WORK *pEm, LONG Dir, LONG Add_dir);
LONG Dir_pos_ck(VECTOR *pV, VECTOR *pP, SHORT Hed_dir, SHORT Hani_dir);
LONG L_pos_ckXZ(VECTOR *pV, VECTOR *pP, ULONG Ll);
LONG L_pos_ckXZM(MATRIX *pM, VECTOR *pP, ULONG Ll, SVECTOR *pSv);
UCHAR Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT);
UCHAR Cdir_ck_pos(LONG Pos_x, LONG Pos_z, ENEMY_WORK *pT);
UCHAR Cdir_ck2(ENEMY_WORK *pEm, ENEMY_WORK *pT);
UCHAR Dir_ck(LONG Dir0, LONG Dir1);
void Set_base_pos(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void Null_pos_set(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG *pKan, ULONG *pSeq);
void Null_pos_set2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void get_null_pos2(SVECTOR *pSv, ULONG R_flg, ULONG *pKan, ULONG *pSeq);
void Em_seq_sound(ENEMY_WORK *pEm);
void Kage_work_set(KAGE_WORK **ppK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos);

extern EFF_PROC D_80097BD4[];
extern struct tagHIT_ATTR *D_80097FCC[]; /* attack data per effect (pl_wep.h HIT_ATTR) */
extern ESP_WORK *D_80098084; /* pool start */
extern ESP_WORK *D_80098088; /* pool end */
extern ESP_WORK *D_8009808C; /* pool 2 start */
extern ESP_WORK *D_80098090; /* pool 2 end */
extern UCHAR D_80098728[]; /* random offsets (Sce_rnd_set) */
extern signed char D_80098828[]; /* 0x80098828 sine, 256 steps, +-127 */
extern TBL12 D_800BA728[16];
extern TBL12 D_800BA7E8[16];
extern UCHAR D_800BF754[];

ULONG Gacha_ch(void);
ULONG Sikai_ck(ENEMY_WORK *pEm, VECTOR *pTa, LONG Han, VECTOR *pAi);
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG Flg);
void func_80018EC8(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void func_80019178(LONG Arg0, LONG Arg1, LONG Arg2, LONG Arg3); /* args from the asm */
void Foot_set_pl(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no);
void Kage_work_init(void);
void Kage_work9_init(void);
void Kage_work9_set(KAGE_WORK9 **ppK, ULONG Off, ULONG Half, ULONG Color, VECTOR *pPos);
void MulKage(KAGE_WORK *pKw, SHORT Grand, SHORT Dir_y, ULONG Flg);
void Kage_work_sort(void);
void Kage_work9_sort(void);
void Kage_work_color_set(KAGE_WORK *pK, ULONG Color);
void Kage_work9_color_set(KAGE_WORK9 *pK, ULONG Color); /* RE2 Kage_work9_color_set */
void Rot_neck(ENEMY_WORK *pEm, SHORT Dir);
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);
void Rot_neck_em_n(ENEMY_WORK *pPm, SHORT Cdir_y, SHORT *pDir);
void Neck_Dir_ZY(VECTOR *pA, VECTOR *pB, SVECTOR *pRot, SHORT Front_dir_y); /* RE2 Neck_Dir_ZY */
void func_8001AD78(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);
void func_8001AECC(ENEMY_WORK *pEm, ULONG a1, ULONG a2);
ENEMY_WORK *Em_work_add(ENEMY_WORK *pSrc);
ENEMY_WORK *func_8001B048(ULONG Id);

#endif /* EM_SUB_H */
