#ifndef OVL_SELECT_H
#define OVL_SELECT_H
/*
 * SELECT.BIN overlay (loaded at 0x801C2000): character select of "The Mercenaries"
 * (three models on a turntable, spot lights, sliding name sprites and a typed-out
 * message window). It is RE2's SELECT.C (Select_main, Select_init, Select_light_*,
 * Select_moji_*) cut down to three players and with a new sprite system.
 */
#include "bio3.h"
#include "ovl/jill_obj.h"
#include "em_sub.h"
#include "joint.h"
#include "main.h"
#include "monitor.h"
#include "pad.h"
#include "pl_wep.h"
#include "snd.h"
#include "sub.h"

/* a SHORT [3][3] rotation matrix written as words (identity init) */
#define MAT_WORD(m, i) (((ULONG *)(m))[i])


/* spot light (RE2 tagSELECT_LIGHT, 16 bytes) */
typedef struct tagSELECT_LIGHT {
    LONG Wait;                         /* 0x00 state, index into Select_light_proc_tbl */
    UCHAR On;                          /* 0x04 */
    UCHAR R;                           /* 0x05 */
    UCHAR G;                           /* 0x06 */
    UCHAR B;                           /* 0x07 */
    SHORT X;                           /* 0x08 */
    SHORT Y;                           /* 0x0A */
    SHORT Z;                           /* 0x0C */
    USHORT Range;                      /* 0x0E */
} SELECT_LIGHT;

/* player placement (0x1C bytes, Select_pl_init[3]) */
typedef struct tagSELECT_PL_INIT {
    UCHAR Id;                          /* 0x00 */
    UCHAR Type;                        /* 0x01 weapon */
    SHORT Pos_x;                       /* 0x02 */
    SHORT Pos_y;                       /* 0x04 */
    SHORT Pos_z;                       /* 0x06 */
    SHORT pad8;
    SHORT Cdir_x;                      /* 0x0A */
    SHORT Cdir_y;                      /* 0x0C */
    SHORT Cdir_z;                      /* 0x0E */
    UCHAR pad10[4];
    ULONG Rgb;                         /* 0x14 */
    SHORT Unk18;                       /* 0x18 -> ENEMY_WORK.Mot_scale_y */
    UCHAR pad1A[2];
} SELECT_PL_INIT;

/* message glyph record (RE2 tagSELECT_MOJI_REC, 16 bytes) */
typedef struct tagSELECT_MOJI_REC {
    ULONG Word0;                       /* 0x00 x (low) / y (high) */
    ULONG Word1;                       /* 0x04 w (low) / h (high) */
    ULONG Word2;                       /* 0x08 u, v, clut */
    UCHAR Ot;                          /* 0x0C ordering table slot */
    UCHAR R;                           /* 0x0D */
    UCHAR G;                           /* 0x0E */
    UCHAR Vis;                         /* 0x0F visible flag */
} SELECT_MOJI_REC;

/* message window work (RE2 tagSELECT_MOJI), 0x20C0 bytes at the heap start */
typedef struct tagSELECT_MOJI {
    UCHAR Col;                         /* 0x000 next column, 0..19 */
    UCHAR Row;                         /* 0x001 next row */
    UCHAR pad2;
    UCHAR Blink;                       /* 0x003 cursor blink timer */
    UCHAR Text[7][20];                 /* 0x004 20 x 7 characters, 0x20 = blank */
    UCHAR Attr[7][20];                 /* 0x090 colour of each character */
    UCHAR Wait;                        /* 0x11C per-character delay counter */
    UCHAR Color;                       /* 0x11D current colour ('|' code) */
    UCHAR pad11E[2];
    UCHAR *pStr;                       /* 0x120 string being printed */
    UCHAR *pCur;                       /* 0x124 read position in pStr */
    SELECT_MOJI_REC Rec[141];          /* 0x128 */
    DR_MODE Dm[2][8];                  /* 0x9F8 */
    ULONG Packet[2][0x2C1];            /* 0xAB8 5-word SPRT packets */
} SELECT_MOJI;

/* overlay work (RE2 tagSELECT_WORK) */
typedef struct tagSELECT_WORK {
    UCHAR Main_no;                     /* 0x000 index into Select_main_proc_tbl */
    UCHAR Pad001;                      /* 0x001 */
    UCHAR Moji_no;                     /* 0x002 message script state */
    UCHAR Pad003;                      /* 0x003 */
    UCHAR pad4[0x784 - 0x4];
    signed char Cursor_no;             /* 0x784 selected player 0..2 */
    UCHAR Blink_cnt;                   /* 0x785 */
    UCHAR Blink_dir;                   /* 0x786 */
    UCHAR Flg;                         /* 0x787 bit0 = rotating */
    UCHAR *pHeap;                      /* 0x788 bump pointer for loaded data */
    UCHAR Vloop_bak;                   /* 0x78C Main.Vloop on entry */
    UCHAR pad78D[0x798 - 0x78D];
    SELECT_LIGHT Light[4];             /* 0x798 */
} SELECT_WORK;

/* view (RE2 Select_view_unk / Select_view_eye / Select_view_ref) */
typedef struct tagSELECT_VIEW {
    ULONG Unk;                         /* 0x00 */
    LONG Eye[3];                       /* 0x04 */
    LONG pad10;
    LONG Ref[3];                       /* 0x14 */
    LONG pad20[2];
} SELECT_VIEW;

/* overlay .data (still in asm) */
extern USHORT Select_pl_cd[]; /* 0x801C4C0C model file, index Id - 8 */
extern USHORT Select_wep_cd[]; /* 0x801C4C14 weapon file base per player, + Type */
extern SELECT_PL_INIT Select_pl_init[]; /* 0x801C4C4C */
extern SELECT_MOJI_REC Select_moji_tbl[141]; /* 0x801C4CD8 glyph template per character */
extern UCHAR Select_str[3][7][30]; /* 0x801C56E8 7 message lines per player */
extern LONG Select_dir_L[3][3]; /* 0x801C5A30 [cursor][pl] target angle */
extern SHORT Select_dir_fpos[3][3]; /* 0x801C5A54 [cursor][pl] target Cdir_y */
extern MATRIX Select_light_mtx[4]; /* 0x801C5A68 */
extern MATRIX Select_light_col[4]; /* 0x801C5AE8 */
extern SEL_SPRT_DEF Select_sprt_def[]; /* 0x801C5B68 */
extern SEL_SPRT_DEF Select_name_def[]; /* 0x801C5BD8 name glyphs */
extern UCHAR Select_name_idx[3][6]; /* 0x801C5CB8 name glyph per player */
extern void (*Select_main_proc_tbl[6])(void); /* 0x801C5CCC */
extern void (*Select_light_proc_tbl[4])(ENEMY_WORK *pEm, SELECT_LIGHT *pLt); /* 0x801C5CE4 */
extern ULONG Select_moji_end; /* 0x801C5CF4 */
extern SELECT_VIEW Select_view; /* 0x801C5CF8 */
extern SEL_OBJ Select_obj[30]; /* 0x801C5D20 */
extern SELECT_WORK Select_wk; /* 0x801C6680 */
extern const char Select_str_sel[8];                                       /* 0x801C208C (.rodata, defined in select.c) */
extern SELECT_MOJI *Select_pMoji; /* 0x801C6E10 message window work */

/* overlay functions (RE2 names where the RE2 routine is recognisable) */
void Select_main(void);
void Select_pl_main(void);
void Select_init(void);
void Select_light_ctrl(ENEMY_WORK *pEm);
void Select_light_wait(ENEMY_WORK *pEm, SELECT_LIGHT *pLt);
void Select_light_up(ENEMY_WORK *pEm, SELECT_LIGHT *pLt);
void Select_light_on(ENEMY_WORK *pEm, SELECT_LIGHT *pLt);
void Select_light_down(ENEMY_WORK *pEm, SELECT_LIGHT *pLt);
void Select_light_init(ENEMY_WORK *pEm);
void Select_light_calc(LONG *pPos, ENEMY_WORK *pEm);
void Select_light_set(ULONG No, VECTOR *pVec, UCHAR *pCol);
void Select_view_init(void);
void Select_pl_move(void);
void Select_fade_in(void);
void Select_fade_out(void);
void Select_cursor_move(void);
void Select_decide(void);
void Select_exit(void);
void Select_pl_dir_set(void);
void Select_pl_rot(void);
UCHAR *Select_pl_load(ENEMY_WORK *pEm, ULONG TpClut, ULONG *pWork); /* Select_pl_load */
void Jill_pl_trans(ENEMY_WORK *pEm, PARTS_W *pParts, ULONG Be_flg, LONG *pPos);
void Jill_light_set(ULONG No, VECTOR *pVec, ULONG Rgb);
UCHAR *Jill_wep_load(ENEMY_WORK *pEm, ULONG *pWork, LONG No); /* weapon load */
void Select_moji_set(ULONG *pDst, SELECT_MOJI_REC *pSrc);
void Select_moji_dm_set(void);
void Select_moji_init(void);
ULONG Select_moji_print(LONG Wait, UCHAR *pStr, ULONG *pEnd); /* Select_moji_print */
void Select_moji_ctrl(void);
void Select_moji_clear(void);
void Select_moji_update(LONG Disp);
void Select_moji_trans(LONG Num);
void Select_bg_load(void);
void Sel_obj_pos_set(SEL_OBJ *pObj, SHORT X, SHORT Y);
void Sel_obj_uv_set(SEL_OBJ *pObj, SEL_SPRT_DEF *pDef, ULONG TpClut);

#endif /* OVL_SELECT_H */
