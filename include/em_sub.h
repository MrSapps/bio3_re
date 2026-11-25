#ifndef EM_SUB_H
#define EM_SUB_H
/*
 * RE2 EM_SUB.C / EM_SUB.H: enemy helper routines (movement, direction checks,
 * shadows, neck rotation, ...). RE3 layouts were taken from the RE3 code.
 */
#include "bio3.h"
#include "esp.h"

typedef void (*EFF_PROC)(ESP_WORK *pEw);

/* effect data registered per effect id (RE2 Espid/pEspdt/pEspmv as one record); Esp_data[32]:
 * [0..15] core effects (CORE00.ESP, Esp_init_C), [16..31] room effects (Esp_init_R) */
typedef struct tagESP_DATA {
    ULONG *pData;                      /* 0x0 RE2 pEspdt: the effect's sprite data block */
    ULONG *pMove;                      /* 0x4 RE2 pEspmv: move records after the sprite data */
    UCHAR Id;                          /* 0x8 RE2 Espid: effect id, 0xFF free */
    UCHAR pad9[3];
} ESP_DATA;
#define ESP_DATA_ROOM 16

/* animation table header at Kan (RE2 KAN_HEAD, JOINT.H) */
typedef struct tagKAN_HEAD {
    USHORT dummy00;                    /* 0x0 */
    USHORT Bone_ofs;                   /* 0x2 */
    ULONG Rec_sz;                      /* 0x4 low half: parts count; bits 18..: record size in words */
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
LONG Cdir_ck(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG Cdir_ck_pos(LONG Pos_x, LONG Pos_z, ENEMY_WORK *pT);
LONG Cdir_ck2(ENEMY_WORK *pEm, ENEMY_WORK *pT);
LONG Dir_ck(LONG Dir0, LONG Dir1);
void Set_base_pos(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void Null_pos_set(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void get_null_pos(SVECTOR *pSv, ULONG R_flg, ULONG *pKan, ULONG *pSeq);
void Null_pos_set2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void get_null_pos2(SVECTOR *pSv, ULONG R_flg, ULONG *pKan, ULONG *pSeq);
void Em_seq_sound(ENEMY_WORK *pEm);
void Kage_work_set(KAGE_WORK **ppK, ULONG Off, ULONG Half, ULONG Color, LONG *pPos);

extern EFF_PROC Esp_tblj[];
extern struct tagHIT_ATTR *Esp_at_tbl[]; /* attack data per effect (pl_wep.h HIT_ATTR) */
extern ESP_WORK *pEsp_top; /* pool start */
extern ESP_WORK *pEsp_end; /* pool end */
extern ESP_WORK *pEsp_top2; /* pool 2 start */
extern ESP_WORK *pEsp_end2; /* pool 2 end */
extern UCHAR Rnd_tbl[]; /* random offsets (Sce_rnd_set) */
extern signed char Rsin_tbl[]; /* 0x80098828 sine, 256 steps, +-127 */
extern ESP_DATA Esp_data[32];

ULONG Gacha_ch(void);
ULONG Sikai_ck(ENEMY_WORK *pEm, VECTOR *pTa, LONG Han, VECTOR *pAi);
void Spd_get(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, LONG Flg);
void Spd_get2(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void Spd_get_sub(LONG Arg0, LONG Arg1, LONG Arg2, LONG Arg3); /* args from the asm */
void Foot_set_pl(ENEMY_WORK *pEm, ULONG Mode, ULONG Foot_no);
void Kage_work_init(void);
void Kage_work9_init(void);
void Kage_work9_set(KAGE_WORK9 **ppK, ULONG Off, ULONG Half, ULONG Color, LONG *pPos);
void MulKage(KAGE_WORK *pKw, SHORT Grand, SHORT Dir_y, ULONG Flg);
void Kage_work_sort(void);
void Kage_work9_sort(void);
void Kage_work_color_set(KAGE_WORK *pK, ULONG Color);
void Kage_work9_color_set(KAGE_WORK9 *pK, ULONG Color); /* RE2 Kage_work9_color_set */
void Rot_neck(ENEMY_WORK *pEm, SHORT Dir);
void Rot_neck_em(ENEMY_WORK *pPm, SHORT Cdir_y);
void Rot_neck_em_n(ENEMY_WORK *pPm, SHORT Cdir_y, SHORT *pDir);
void Neck_Dir_ZY(VECTOR *pA, VECTOR *pB, SVECTOR *pRot, SHORT Front_dir_y); /* RE2 Neck_Dir_ZY */
void Now_seq_set(ENEMY_WORK *pEm, ULONG *pKan, ULONG *pSeq, ULONG R_flg);
void Em_dead_flg_on_cand(ENEMY_WORK *pEm);
void Hit_area_parts_set(ENEMY_WORK *pEm, ULONG Bottom_part, ULONG Top_part);
ENEMY_WORK *Em_work_add(ENEMY_WORK *pSrc);
ENEMY_WORK *Em_work_search(ULONG Id);

#endif /* EM_SUB_H */
