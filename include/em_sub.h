#ifndef EM_SUB_H
#define EM_SUB_H
/*
 * RE2 EM_SUB.C / EM_SUB.H: enemy helper routines (movement, direction checks,
 * shadows, neck rotation, ...). RE3 layouts were taken from the RE3 code.
 */
#include "bio3.h"

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
    UCHAR pad18[0x34 - 0x18];
    LONG Pos_x;                        /* 0x034 */
    LONG Pos_y;                        /* 0x038 */
    LONG Pos_z;                        /* 0x03C */
    SHORT Unk40[3];                    /* 0x040 copy of the position (func_80024B34) */
    USHORT Type;                       /* 0x046 low byte = player weapon (RE2 Type & 0xfff) */
    UCHAR Tpage;                       /* 0x048 RE2 Tpage */
    UCHAR Clut;                        /* 0x049 RE2 Clut */
    UCHAR Id;                          /* 0x04A enemy kind (0x10+ for enemies, Pl_Em_damage indexes by Id - 0x10) */
    UCHAR Unk4B;                       /* 0x4B */
    UCHAR pad4C[0x6C - 0x4C];
    SHORT Cdir_x;                      /* 0x06C */
    SHORT Cdir_y;                      /* 0x06E */
    SHORT Cdir_z;                      /* 0x070 */
    USHORT dummy01;                    /* 0x072 */
    ULONG Poly_rgb;                    /* 0x074 */
    MATRIX *pSuper;                    /* 0x078 */
    ULONG Unk7C;                       /* 0x07C */
    UCHAR pad80[0xAE - 0x80];
    USHORT UnkAE;                      /* 0x0AE */
    UCHAR padB0[0xBA - 0xB0];
    USHORT UnkBA;                      /* 0xBA */
    UCHAR padBC[0xC0 - 0xBC];
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
    UCHAR UnkE3;                       /* 0xE3 */
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
    void *pUnk10C;                     /* 0x10C */
    struct tagENEMY_WORK *pEnemy_ptr;  /* 0x110 */
    struct tagENEMY_WORK *pUnk114;     /* 0x114 */
    ULONG *pUnk118;                    /* 0x118 per-part animation header (Sa_dat_set) */
    UCHAR pad11C[0x120 - 0x11C];
    UCHAR Unk120;                      /* 0x120 */
    UCHAR Neck_no;                     /* 0x121 RE2 Neck_no (part index used as the aim point) */
    SHORT Unk122;                      /* 0x122 */
    SHORT Spd_base_x;                  /* 0x124 RE2 Spd_base_x (Spd_get) */
    SHORT Spd_base_y;                  /* 0x126 */
    SHORT Spd_base_z;                  /* 0x128 */
    SHORT Unk12A;                      /* 0x12A */
    UCHAR Unk12C;                      /* 0x12C */
    UCHAR Damage_cnt;                  /* 0x12D player: |= 0x80 on game over (Pl_life_down) */
    UCHAR pad12E[0x130 - 0x12E];
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
    SHORT Unk150;                      /* 0x150 */
    SHORT Unk152;                      /* 0x152 */
    ULONG Unk154;                      /* 0x154 */
    LONG Unk158;                       /* 0x158 distance to target? (masked 0x3FFFFFFF) */
    LONG *pUnk15C;                     /* 0x15C target position (func_8003CEA0) */
    struct tagKAGE_WORK9 *pKage_work;  /* 0x160 */
    SHORT Unk164;                      /* 0x164 */
    SHORT Unk166;                      /* 0x166 */
    SHORT Unk168;                      /* 0x168 */
    UCHAR pad16A[0x16C - 0x16A];
    ULONG Unk16C;                      /* 0x16C (func_800445C8 arg 0) */
    ULONG Unk170;                      /* 0x170 (func_800445C8 arg 1) */
    UCHAR pad174[0x178 - 0x174];
    USHORT Unk178[3];                  /* 0x178 copied to D_80098B08 (func_800254AC) */
    USHORT Unk17E;                     /* 0x17E */
    ULONG Unk180[4];                   /* 0x180 args of func_80079690 / Joint_move */
    UCHAR pad190[0x1A0 - 0x190];
    UCHAR Unk1A0;                      /* 0x1A0 */
    UCHAR Unk1A1;                      /* 0x1A1 */
    UCHAR Unk1A2;                      /* 0x1A2 (pl_mv05_w2_t2) */
    UCHAR pad1A3[0x1B8 - 0x1A3];
    SHORT Unk1B8;                      /* 0x1B8 vertical speed added to Pos_y (func_8004B7E8) */
    SHORT Unk1BA;                      /* 0x1BA func_8002646C */
    SHORT Unk1BC[2];                   /* 0x1BC decayed by Unk1C2 (func_80026648) */
    UCHAR pad1C0;
    UCHAR Unk1C1;                      /* 0x1C1 func_8002646C */
    SHORT Unk1C2;                      /* 0x1C2 func_8002646C */
    UCHAR pad1C4[0x1C8 - 0x1C4];
    SHORT Unk1C8;                      /* 0x1C8 */
    SHORT Unk1CA;                      /* 0x1CA */
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

void func_8002D4B0(MATRIX *m0, MATRIX *m1, MATRIX *m2); /* RE2 CompM */
void Snd_se_enem(ULONG No, ENEMY_WORK *pEm);          /* RE2 Snd_se_enem */
void func_800100A4(void *pDst, void *pSrc, ULONG Size); /* RE2 Memcpy */

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
void Set_base_pos(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg);
void Null_pos_set(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg);
void get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG Kan, ULONG Seq);
void Null_pos_set2(ENEMY_WORK *pEm, ULONG Kan, ULONG Seq, ULONG R_flg);
void get_null_pos2(SVECTOR *pSv, ULONG R_flg, ULONG Kan, ULONG Seq);
void Em_seq_sound(ENEMY_WORK *pEm);

#endif /* EM_SUB_H */
