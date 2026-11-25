#ifndef STATUS_H
#define STATUS_H
/* status.c (RE2 STATUS.C / ST_SUB*.C role): status screen, inventory, item box, files and map (RE3 0x800636D4..) */
#include "bio3.h"

/* SPRT definition (12 bytes): texture rect and position relative to St.Pos[] (St_sprt_init/St_sprt_draw) */
typedef struct tagST_SPR_DEF {
    UCHAR U;                           /* 0x0 */
    UCHAR pad1;
    UCHAR V;                           /* 0x2 */
    UCHAR pad3;
    USHORT W;                          /* 0x4 */
    USHORT H;                          /* 0x6 */
    SHORT X;                           /* 0x8 */
    SHORT Y;                           /* 0xA */
} ST_SPR_DEF;

/* POLY_FT4 definition (12 bytes, St_ft4_init/St_ft4_draw); highlighted variants follow as a second record */
typedef struct tagST_FT4_DEF {
    USHORT U;                          /* 0x0 */
    USHORT V;                          /* 0x2 */
    USHORT W;                          /* 0x4 */
    USHORT H;                          /* 0x6 */
    USHORT X;                          /* 0x8 */
    USHORT Y;                          /* 0xA */
} ST_FT4_DEF;

/* TILE definition (12 bytes, St_tile_init/St_tile_draw) */
typedef struct tagST_TILE_DEF {
    SHORT pad0[2];
    SHORT W;                           /* 0x4 */
    SHORT H;                           /* 0x6 */
    SHORT X;                           /* 0x8 */
    SHORT Y;                           /* 0xA */
} ST_TILE_DEF;

typedef struct tagST_MAP_ARROW {
    SHORT U0;                          /* 0x0 */
    SHORT V0;                          /* 0x2 */
    SHORT U1;                          /* 0x4 */
    SHORT V1;                          /* 0x6 */
    SHORT X;                           /* 0x8 */
    SHORT Y;                           /* 0xA */
} ST_MAP_ARROW;

/* MAP.MAP (St.pMap_file) */
typedef struct tagST_MAP_PART {
    UCHAR U0;                          /* 0x0 */
    UCHAR V0;                          /* 0x1 */
    UCHAR U1;                          /* 0x2 */
    UCHAR V1;                          /* 0x3 */
    SHORT X;                           /* 0x4 */
    SHORT Y;                           /* 0x6 */
    UCHAR Clut;                        /* 0x8 */
    UCHAR Tpage;                       /* 0x9 */
    UCHAR padA[2];
} ST_MAP_PART;

typedef struct tagST_RGB_STEP {
    UCHAR R, G, B;                     /* 0x0 */
    UCHAR Dr, Dg, Db;                  /* 0x3 */
} ST_RGB_STEP;

typedef struct { UCHAR A0; UCHAR B0; UCHAR A1; UCHAR B1; } ST_ROOM_MAP;

/* item properties, one per item id (RE2 Item_data_tbl role) */
typedef struct tagITEM_DATA {
    UCHAR Type;                        /* 0x0 kind: 1 weapon, 2 ammo, 4 key item, ...; 2/5/6 stack */
    UCHAR Max;                         /* 0x1 most a slot can hold */
    UCHAR Chk_flg;                     /* 0x2 G.Unk7910 flag set once examined (the name changes with it) */
    UCHAR Attr;                        /* 0x3 default low byte of ITEM_WORK.Flg */
} ITEM_DATA;

/* combine record (Item_mix_tbl) */
typedef struct tagITEM_MIX {
    UCHAR Type;                        /* 0x0 0 merge, 1 new item, 2 ammo, 3 weapon change, 4 ammo swap, 5 tool + rounds, 6 weapon + part */
    UCHAR Id_a;                        /* 0x1 the two items, either order; 0xFF ends the table */
    UCHAR Id_b;                        /* 0x2 */
    UCHAR New_id;                      /* 0x3 resulting item */
    UCHAR New_num;                     /* 0x4 count, or the second resulting item */
    UCHAR pad5[3];
} ITEM_MIX;

/* what an examined item turns into (Item_check_tbl) */
typedef struct tagITEM_CHECK {
    UCHAR Type;                        /* 0x0 0 another item (Arg), 1 a file (Arg - 0x85 = file flag) */
    UCHAR Id;                          /* 0x1 0xFF ends the table */
    UCHAR Arg;                         /* 0x2 */
    UCHAR File_no;                     /* 0x3 file list slot */
} ITEM_CHECK;

/* ammo of a weapon (Item_tama_tbl) */
typedef struct tagITEM_TAMA {
    UCHAR Wep_id;                      /* 0x0 0xFF ends the table */
    UCHAR Tama_id;                     /* 0x1 */
    UCHAR pad2[2];
} ITEM_TAMA;

/* map data header (St.pMap_data): byte offsets from its start */
typedef struct tagST_MAP_DATA {
    LONG Room_ofs;                     /* 0x00 ST_MAP_ROOM per room (player marker) */
    LONG Info_ofs;                     /* 0x04 ST_MAP_INFO */
    LONG Mark_ofs;                     /* 0x08 ST_MAP_FLG: parts highlighted until their flag is set (Map_door_mark_ck) */
    LONG Hide_ofs;                     /* 0x0C ST_MAP_FLG: base parts hidden until their flag (map not owned) */
    LONG Door_ofs;                     /* 0x10 ST_MAP_FLG: door / marker parts (Map_door_disp_ck) */
} ST_MAP_DATA;

/* panel position */
typedef struct tagST_POS {
    SHORT X;                           /* 0x0 */
    SHORT Y;                           /* 0x2 */
} ST_POS;

/* status work (RE2 struct tagSTATUS role, RE3 layout; St at 0x800E01C0) */
typedef struct tagST_WORK {
    UCHAR pad0[4];
    UCHAR St_type;                     /* 0x04 RE2 St_type: screen (0/1 status, 2 item box, 3 file, 4/5 map; 3/5 = picked up in a room) */
    UCHAR pad5;
    UCHAR Hiki_flg;                    /* 0x06 opened from a drawer (sce_at Hikidashi_entrance) */
    UCHAR pad7;
    ULONG *pBuf;                       /* 0x08 load buffer */
    UCHAR padC[0x10 - 0xC];
    UCHAR Rtn0;                        /* 0x10 St_main_tbl step */
    UCHAR Rtn1;                        /* 0x11 sub step (St_box_tbl, St_status_tbl, St_map_tbl) */
    UCHAR Rtn2;                        /* 0x12 sub-sub step (St_mix_tbl, file list) */
    UCHAR Timer;                       /* 0x13 frame counter of the current step */
    ULONG Flg14;                       /* 0x14 0x40000000: inventory changed, reload the icons */
    ULONG Flg;                         /* 0x18 ST_FLG_* draw and state bits */
    signed char Cursor;                /* 0x1C inventory slot; -1 MAP, -2 FILE, < -2 EXIT */
    UCHAR Cursor2;                     /* 0x1D second item of a combine */
    UCHAR Cmd_cursor;                  /* 0x1E command window entry (equip/use, check, combine) */
    signed char Box_cursor;            /* 0x1F item box slot (0..63, a ring) */
    signed char File_cursor;           /* 0x20 file list slot (0..29) */
    UCHAR pad21;
    SHORT Blink;                       /* 0x22 colour pulse 0..0x3F (St_counter_move) */
    SHORT Blink_dir;                   /* 0x24 1: Blink going down */
    UCHAR pad26;
    UCHAR Task_on;                     /* 0x27 the status task loops while set */
    UCHAR Equip_id_old;                /* 0x28 equipped item on entry; St_exit changes the weapon if it differs */
    UCHAR Pl_id;                       /* 0x29 */
    USHORT Item_flg;                   /* 0x2A Flg of the item the command window is open for */
    UCHAR pad2C[0x2E - 0x2C];
    UCHAR Equip_save;                  /* 0x2E Equip_no_save/Equip_id_save hold the equip to restore */
    UCHAR Equip_no_save;               /* 0x2F */
    UCHAR Equip_id_save;               /* 0x30 */
    UCHAR pad31;
    SHORT Ecg_pos;                     /* 0x32 ECG line scroll */
    UCHAR Condition;                   /* 0x34 St_condition_get: 0 fine .. 3 danger, 4 poison */
    UCHAR Life_add;                    /* 0x35 life restored by the item being used */
    UCHAR Poison_cure;                 /* 0x36 the item being used cures poison */
    UCHAR pad37[0x3A - 0x37];
    UCHAR Slide_step;                  /* 0x3A command window slide (St_cmd_slide) */
    UCHAR Slide_done;                  /* 0x3B */
    UCHAR Slide_cnt;                   /* 0x3C */
    UCHAR Slide_no;                    /* 0x3D */
    UCHAR Cmd_closed;                  /* 0x3E */
    UCHAR pad3F[0x42 - 0x3F];
    ITEM_WORK Sel_item;                /* 0x42 copy of the selected item */
    UCHAR pad46[0x48 - 0x46];
    ITEM_WORK *pMix_a;                 /* 0x48 combine: item A */
    ITEM_WORK *pMix_b;                 /* 0x4C combine: item B */
    ITEM_DATA *pData_a;                /* 0x50 Item_data_tbl entry of item A */
    ITEM_DATA *pData_b;                /* 0x54 Item_data_tbl entry of item B */
    ITEM_MIX *pMix;                    /* 0x58 Item_mix_tbl record of the combine */
    UCHAR *pMix_cur_a;                 /* 0x5C cursor of item A (&Cursor) */
    UCHAR *pMix_cur_b;                 /* 0x60 cursor of item B (&Cursor2) */
    UCHAR Mix_type;                    /* 0x64 combine result kind (St_mix_result cases) */
    UCHAR Mix_num;                     /* 0x65 combine result count */
    SHORT Mix_dx;                      /* 0x66 icon slide per frame */
    SHORT Mix_dy;                      /* 0x68 */
    UCHAR Mix_tama_type;               /* 0x6A ammo kind of the combine (Mix_tama_tbl) */
    UCHAR pad6B[0x6E - 0x6B];
    UCHAR Get_slot;                    /* 0x6E slot the picked-up item goes to */
    UCHAR Zoom_w;                      /* 0x6F flying icon half size */
    UCHAR Zoom_h;                      /* 0x70 */
    signed char Zoom_dx;               /* 0x71 */
    signed char Zoom_dy;               /* 0x72 */
    UCHAR Get_stack;                   /* 0x73 1: added to an existing slot */
    UCHAR pad74[0x76 - 0x74];
    UCHAR Chg_id;                      /* 0x76 item the examined item turns into */
    UCHAR Chg_slot;                    /* 0x77 */
    UCHAR pad78[0x7C - 0x78];
    LONG Map_x;                        /* 0x7C map scroll */
    LONG Map_y;                        /* 0x80 */
    LONG Map_x_old;                    /* 0x84 scroll before a zoom */
    LONG Map_y_old;                    /* 0x88 */
    LONG Map_dx_acc;                   /* 0x8C zoom easing accumulators */
    LONG Map_dy_acc;                   /* 0x90 */
    LONG Map_x0;                       /* 0x94 map origin (ST_MAP_INFO) */
    LONG Map_y0;                       /* 0x98 */
    LONG Map_x_max;                    /* 0x9C scroll limits (origin + 0x140 / 0xE0) */
    LONG Map_y_max;                    /* 0xA0 */
    LONG Map_zoom;                     /* 0xA4 percent, 50 or 100 */
    UCHAR Zoom_out_flg;                /* 0xA8 */
    UCHAR Pl_mark_flg;                 /* 0xA9 draw the player marker */
    UCHAR Map_stage_cur;               /* 0xAA map stage the player is in */
    UCHAR Map_stage;                   /* 0xAB map stage shown */
    UCHAR Map_room_cur;                /* 0xAC map room the player is in */
    UCHAR Floor_flg;                   /* 0xAD multi-floor map */
    signed char Floor_disp;            /* 0xAE floor shown */
    signed char Floor_pl;              /* 0xAF floor the player is on */
    UCHAR Floor_num;                   /* 0xB0 */
    UCHAR Menu_blink;                  /* 0xB1 area menu cursor blink */
    UCHAR padB2[0xB4 - 0xB2];
    struct tagST_MAP_DATA *pMap_data;  /* 0xB4 map data of the MAP.MAP entry */
    USHORT Map_event;                  /* 0xB8 map opened by an event (sce2 opcode): 1 stage 0, 2 stage 4 */
    UCHAR Map_return;                  /* 0xBA map re-entered after an area change */
    signed char Area_cursor;           /* 0xBB area menu cursor */
    UCHAR File_no;                     /* 0xBC file being read */
    UCHAR Page_no;                     /* 0xBD page being read */
    signed char Blink_timer;           /* 0xBE page arrow blink, -20..20 */
    UCHAR File_exit;                   /* 0xBF exit marker selected on the last page */
    UCHAR Page_cnt;                    /* 0xC0 page turn slide */
    UCHAR File_page;                   /* 0xC1 file list page 0/1 */
    UCHAR padC2[0xC4 - 0xC2];
    SHORT Page_spd;                    /* 0xC4 */
    SHORT Page_acc;                    /* 0xC6 */
    SHORT Box_scroll;                  /* 0xC8 item box scroll offset (+-20) */
    UCHAR Box_scroll_dir;              /* 0xCA */
    UCHAR Box_fast;                    /* 0xCB L1/R1 page scroll: 1 up, 2 down */
    ULONG *pMap_file;                  /* 0xCC MAP.MAP entry */
    UCHAR Clut_frame;                  /* 0xD0 clut rows (y - 0x1E0) of the status textures: panels */
    UCHAR Clut_icon;                   /* 0xD1 item icons */
    UCHAR Clut_face;                   /* 0xD2 */
    UCHAR Clut_parts;                  /* 0xD3 font, arrows, markers (+n variants) */
    UCHAR Clut_zoom;                   /* 0xD4 examined item picture (ITEMG) */
    UCHAR Clut_box;                    /* 0xD5 item box bar */
    UCHAR Clut_unused;                 /* 0xD6 */
    UCHAR Clut_page;                   /* 0xD7 file page */
    UCHAR Clut_file;                   /* 0xD8 file icons */
    UCHAR padD9[0xE0 - 0xD9];
    ITEM_CHECK *pChk;                  /* 0xE0 Item_check_tbl record of the examined item */
    ST_POS Pos[32];                    /* 0xE4 panel positions, indexed by the prm>>8 byte of the prim setters */
    DR_TPAGE Ot[2][40];                /* 0x164 per-buffer ordering nodes, index prm>>16 of the prim setters */
    DR_TPAGE Tpage[2][2];              /* 0x3E4 file viewer */
    DR_TPAGE Map_tpage[4];             /* 0x404 map, two per buffer */
    DR_TPAGE Map_win_tpage[2];         /* 0x424 map area window */
    DR_TPAGE File_tpage[2];            /* 0x434 file icons */
} ST_WORK;
extern ST_WORK St;                     /* 0x800E01C0 RE2 St */

extern char D_80010DCC[];
extern char D_80010F84[];
extern char D_80010FEC[];
extern UCHAR File_page_sum[];
extern ULONG File_page_size[];
extern USHORT File_page_top[];
extern USHORT File_page_num[];
extern ST_SPR_DEF D_8009F2EC[];
extern ST_SPR_DEF D_8009F2F8[];
extern ST_SPR_DEF D_8009F304[];
extern ST_SPR_DEF D_8009F310[];
extern ST_SPR_DEF D_8009F31C[];
extern ST_SPR_DEF D_8009F328[];
extern ST_SPR_DEF D_8009F34C[];
extern ST_FT4_DEF D_8009F388[];
extern ST_FT4_DEF D_8009F3E8[];
extern ST_SPR_DEF D_8009F3F4[];
extern ST_SPR_DEF D_8009F418[];
extern ST_FT4_DEF D_8009F424[];
extern ST_SPR_DEF D_8009F448[];
extern ST_TILE_DEF D_8009F49C[];
extern ST_SPR_DEF D_8009F4A8[];
extern ST_SPR_DEF D_8009F4B4[];
extern ST_TILE_DEF D_8009F4C0[];
extern ST_SPR_DEF D_8009F4CC[];
extern ST_SPR_DEF D_8009F4D8[];
extern void (*St_box_tbl[])(ST_WORK *pSt);
extern USHORT Box_pix_pos[][2];
extern UCHAR Box_ng_item[];
extern UCHAR Itemi_sum[];
extern UCHAR Itemg_sum[];
extern ULONG Item_pix_ofs[];
extern ST_SPR_DEF D_8009F890[];
extern ST_SPR_DEF D_8009F8A8[];
extern ST_SPR_DEF D_8009F8B4[];
extern ST_SPR_DEF D_8009F8C0[];
extern ST_SPR_DEF D_8009F8CC[];
extern ST_SPR_DEF D_8009F908[];
extern ST_SPR_DEF D_8009F95C[];
extern ST_FT4_DEF D_8009F9B0[];
extern ST_SPR_DEF D_8009F9D4[];
extern ST_SPR_DEF File_icon_tbl[];
extern ST_SPR_DEF D_8009FB78[];
extern ST_SPR_DEF D_8009FB84[];
extern ST_TILE_DEF D_8009FB90[];
extern ST_SPR_DEF D_8009FBA8[];
extern ST_SPR_DEF D_8009FBC0[];
extern ST_SPR_DEF D_8009FBCC[];
extern ST_SPR_DEF D_8009FBFC[];
extern ST_SPR_DEF D_8009FC2C[];
extern ST_SPR_DEF D_8009FC38[];
extern ST_SPR_DEF D_8009FC44[];
extern ST_SPR_DEF D_8009FC50[];
extern ST_FT4_DEF D_8009FC5C[];
extern ST_FT4_DEF D_8009FCBC[];
extern ST_FT4_DEF D_8009FD1C[];
extern ST_FT4_DEF D_8009FD7C[];
extern ST_FT4_DEF D_8009FDDC[];
extern ST_FT4_DEF D_8009FE0C[];
extern ST_FT4_DEF D_8009FE3C[];
extern ST_FT4_DEF D_8009FE6C[];
extern ST_FT4_DEF D_8009FE9C[];
extern ST_FT4_DEF D_8009FECC[];
extern ST_FT4_DEF D_8009FEFC[];
extern ST_FT4_DEF D_8009FF2C[];
extern ST_FT4_DEF D_8009FF38[];
extern ST_FT4_DEF D_8009FF44[];
extern ST_FT4_DEF D_8009FF50[];
extern ST_TILE_DEF D_8009FF5C[];
extern ST_SPR_DEF D_8009FF68[];
extern ST_SPR_DEF D_8009FF74[];
extern ST_SPR_DEF D_8009FF80[];
extern ST_SPR_DEF D_8009FF8C[];
extern ST_SPR_DEF D_8009FFB0[];
extern ST_SPR_DEF D_8009FFBC[];
extern ST_SPR_DEF D_8009FFC8[];
extern ST_SPR_DEF D_8009FFD4[];
extern ST_SPR_DEF D_8009FFE0[];
extern ST_SPR_DEF D_8009FFEC[];
extern ST_SPR_DEF D_8009FFF8[];
extern ST_SPR_DEF Cond_spr_tbl[]; /* condition text, one per St_condition_get value */
extern USHORT Item_pix_pos[][2];
extern USHORT Item_num_pos[][2];
extern USHORT File_order_tbl[]; /* same table as File_order_tbl, byte access */
extern signed char Mix_tama_tbl[][2];
extern void (*St_status_tbl[])(ST_WORK *pSt);
extern void (*St_mix_tbl[])(ST_WORK *pSt);
extern ST_RGB_STEP Ecg_col_tbl[];
extern UCHAR *Ecg_wave_tbl[];
extern ITEM_WORK Item_init_normal[2][5];
extern ITEM_WORK Item_init_easy[2][6];
extern ITEM_WORK Item_init_pl8[];
extern ITEM_WORK Item_init_pl10[];
extern ITEM_WORK Item_init_pl9[];
extern ITEM_WORK Item_init_mode3[];
extern ITEM_WORK Item_init_mode4[];
extern ITEM_WORK Box_init_normal[2][4];
extern ITEM_WORK Box_init_easy[2][11];
extern void (*St_main_tbl[])(ST_WORK *pSt);
extern UCHAR Map_mess[];
extern USHORT Map_mess_ofs[];
extern UCHAR Map_file_sum[];
extern ULONG Map_file_size[];
extern ST_MAP_ARROW Map_arrow[][4];
extern ST_ROOM_MAP Map_room_alias[];
extern UCHAR Map_area_list[];
extern void (*St_map_tbl[])(ST_WORK *pSt);
extern ITEM_DATA Item_data_tbl[];      /* per item id */
extern ITEM_MIX Item_mix_tbl[];        /* Id_a 0xFF ends it */
extern ITEM_CHECK Item_check_tbl[];    /* Id 0xFF ends it */
extern ITEM_TAMA Item_tama_tbl[];      /* Wep_id 0xFF ends it */
extern LONG D_800C79CC; /* extra game character (Status_init) */
extern SHORT Mapping_tpage_clut;
extern ULONG D_800D1FA0[];
extern ULONG Map_visit_flg[];
extern ULONG Map_get_flg[];
extern ULONG Map_area_flg[];
extern UCHAR D_800D225E; /* G.Item_work[0].Num as its own symbol (reloaded every loop pass) */
extern ITEM_WORK *D_800D23B4;
extern LONG St_pl_kind;
extern UCHAR D_800D442D;
extern UCHAR D_800D442E;

void St_file_load(ST_WORK *pSt);
void St_file_read(ST_WORK *pSt);
void St_file_end(ST_WORK *pSt);
void St_file_prim_init(ST_WORK *pSt);
void St_file_draw(ST_WORK *pSt);
void St_file_page_load(ST_WORK *pSt, UCHAR File_no, UCHAR Page);
void St_sprt_draw_tp(ST_WORK *pSt, SPRT *p, ST_SPR_DEF *pDef, ULONG prm);
void St_box_init(ST_WORK *pSt);
void St_box_main(ST_WORK *pSt);
void St_box_exit(ST_WORK *pSt);
void St_box_inv_select(ST_WORK *pSt);
void St_box_select(ST_WORK *pSt);
void St_box_pack(ST_WORK *pSt);
void St_box_scroll(ST_WORK *pSt);
void St_box_mess_wait(ST_WORK *pSt);
void St_box_prim_set(ST_WORK *pSt);
void St_box_draw(ST_WORK *pSt);
void St_icon_swap(UCHAR Inv_no, UCHAR Box_no);
void St_icon_copy(UCHAR Box_no, UCHAR Inv_no);
void St_box_list_draw(ST_WORK *pSt, UCHAR Box_cursor, SHORT Ofs_y);
void St_box_prim_init(ST_WORK *pSt);
void St_box_frame_draw(LONG Arg0); /* args from the asm */
void St_status_init(ST_WORK *pSt);
void St_status_main(ST_WORK *pSt);
void St_item_select(ST_WORK *pSt);
void St_cmd_open(ST_WORK *pSt);
void St_cmd_select(ST_WORK *pSt);
void St_cmd_close(ST_WORK *pSt);
void St_map_open(ST_WORK *pSt);
void St_file_list(ST_WORK *pSt);
void St_item_use(ST_WORK *pSt);
void St_mix_main(ST_WORK *pSt);
void St_mix_init(ST_WORK *pSt);
void St_mix_select(ST_WORK *pSt);
void St_mix_exec(ST_WORK *pSt);
void St_mix_mess_wait(ST_WORK *pSt);
void St_mix_yesno(ST_WORK *pSt);
void St_mix_next(ST_WORK *pSt);
void St_mix_result(ST_WORK *pSt);
void St_item_check(ST_WORK *pSt);
void St_item_switch(ST_WORK *pSt);
UCHAR St_cmd_slide(ST_WORK *pSt, LONG Close);
void St_item_shift(ST_WORK *pSt);
void St_item_get(ST_WORK *pSt);
void St_item_change(ST_WORK *pSt);
void St_item_to_file(ST_WORK *pSt);
void St_exit_set(ST_WORK *pSt);
ITEM_MIX *Search_item_mix(ST_WORK *pSt, UCHAR a, UCHAR b);
ITEM_CHECK *Search_item_check(ST_WORK *pSt, UCHAR id);
ITEM_TAMA *Search_item_tama(ST_WORK *pSt, UCHAR id);
void Pix_load(UCHAR no, UCHAR idx);
void Pix_read(UCHAR no);
void Pix_trans(UCHAR no);
void Pix_unpack(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf);
void Pix_unpack_box(UCHAR no, UCHAR tbl, ULONG ofs, ULONG *pBuf);
void Itemg_load(UCHAR no);
void Pix_swap(UCHAR a, UCHAR b);
void Pix_move(UCHAR dst, UCHAR src);
void St_prim_init(ST_WORK *pSt);
void St_item_disp(ST_WORK *pSt);
void St_zoom_icon_disp(ST_WORK *pSt);
void St_ecg_disp(ST_WORK *pSt);
SPRT *St_num_disp(SPRT *p, SHORT num, ULONG xy, ULONG prm);
void St_file_icon_disp(ST_WORK *pSt);
signed char Search_item_id(UCHAR id, signed char i); /* RE2 Search_item_id */
signed char Item_decrease(UCHAR no, signed char Sub, UCHAR Keep);
void Item_delete(UCHAR no);
void Equip_item_delete(void);
signed char Tama_reload(UCHAR Exec);
LONG Tama_decrease(UCHAR Exec);
void Status_init(void);
void St_tex_load(void);
void St_init(ST_WORK *pSt);
void Status(void);
void St_exit(ST_WORK *pSt);
void St_counter_move(ST_WORK *pSt);
void St_draw(ST_WORK *pSt);
void St_open_wait(ST_WORK *pSt);
void St_close(ST_WORK *pSt);
LONG St_condition_get(void);
void St_sprt_init(ST_WORK *pSt, SPRT *p, ST_SPR_DEF *pDef, ULONG prm);
void St_ft4_init(ST_WORK *pSt, POLY_FT4 *p, ST_FT4_DEF *pDef, ULONG prm);
void St_tile_init(ST_WORK *pSt, TILE *p, ST_TILE_DEF *pDef, UCHAR n);
void St_line_init(ST_WORK *pSt);
void St_sprt_draw(ST_WORK *pSt, SPRT *p, ST_SPR_DEF *pDef, ULONG prm);
void St_ft4_draw(ST_WORK *pSt, POLY_FT4 *p, ST_FT4_DEF *pDef, ULONG prm);
void St_tile_draw(ST_WORK *pSt, TILE *p, ST_TILE_DEF *pDef, ULONG prm);
void St_mapping_tim(ULONG *pTim);
void St_chenge_pl(ULONG no);
void St_map_pos_init(ST_WORK *pSt);
void St_map_init(ST_WORK *pSt);
void St_map(ST_WORK *pSt);
void St_map_ctrl(ST_WORK *pSt);
void St_map_zoom(ST_WORK *pSt);
void St_map_area_select(ST_WORK *pSt);
void St_map_fade_in(ST_WORK *pSt);
void St_map_area_change(ST_WORK *pSt);
void St_map_exit(ST_WORK *pSt);
void St_map_draw(ST_WORK *pSt);
void Map_part_set(ST_WORK *pSt, POLY_FT4 *p, ST_MAP_PART *pPt, UCHAR clut);
void Map_file_load(ST_WORK *pSt, UCHAR no);
void Map_visit_set(UCHAR Stage, UCHAR Room);
ULONG Map_visit_ck(UCHAR Stage, UCHAR Room);
LONG Map_room_disp_ck(UCHAR Stage, UCHAR Room);
LONG Map_part_disp_ck(UCHAR Stage, UCHAR Room, UCHAR Part);
LONG Map_part_clut(UCHAR Stage, UCHAR Room, UCHAR Part);
LONG Map_part_blink_ck(UCHAR Stage, UCHAR Room, UCHAR Part);
LONG Map_door_disp_ck(UCHAR Stage, UCHAR Room, UCHAR Part);
LONG Map_door_mark_ck(UCHAR Stage, UCHAR Room, UCHAR Part);
UCHAR Map_floor_get(ST_WORK *pSt, UCHAR Stage, UCHAR Room);
void Map_room_conv(UCHAR *pStage, UCHAR *pRoom);
void St_frame_end(ST_WORK *pSt);

#endif /* STATUS_H */
