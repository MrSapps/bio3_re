#ifndef STATUS_H
#define STATUS_H
/* RE2 STATUS.C / ST_SUB*.C: status screen, inventory, item box, files and map (RE3 0x800636D4..) */
#include "bio3.h"

typedef struct tagST_ICON {
    UCHAR U;                           /* 0x0 */
    UCHAR pad1;
    UCHAR V;                           /* 0x2 */
    UCHAR pad3;
    USHORT W;                          /* 0x4 */
    USHORT H;                          /* 0x6 */
    UCHAR pad8[4];
} ST_ICON;

typedef struct tagST_MAP_ARROW {
    SHORT U0;                          /* 0x0 */
    SHORT V0;                          /* 0x2 */
    SHORT U1;                          /* 0x4 */
    SHORT V1;                          /* 0x6 */
    SHORT X;                           /* 0x8 */
    SHORT Y;                           /* 0xA */
} ST_MAP_ARROW;

/* MAP.MAP (St.UnkCC) */
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

/* panel position */
typedef struct tagST_POS {
    SHORT X;                           /* 0x0 */
    SHORT Y;                           /* 0x2 */
} ST_POS;

/* status work (RE2 struct tagSTATUS role, RE3 layout; St at 0x800E01C0) */
typedef struct tagST_WORK {
    UCHAR pad0[4];
    UCHAR St_type;                     /* 0x04 RE2 St_type: screen (0/1 status, 2 item box, 3 file, 4/5 map) */
    UCHAR pad5;
    UCHAR Unk6;                        /* 0x06 set when the drawer item is shown (sce_at Hikidashi_entrance) */
    UCHAR pad7;
    ULONG *pBuf;                       /* 0x08 load buffer (D_800E01C8) */
    UCHAR padC[0x10 - 0xC];
    UCHAR Rtn0;                        /* 0x10 */
    UCHAR Rtn1;                        /* 0x11 */
    UCHAR Rtn2;                        /* 0x12 */
    UCHAR Unk13;                       /* 0x13 frame counter of the current step */
    ULONG Flg14;                       /* 0x14 */
    ULONG Flg;                         /* 0x18 */
    signed char Unk1C;                 /* 0x1C inventory cursor */
    UCHAR Unk1D;                       /* 0x1D */
    UCHAR Unk1E;                       /* 0x1E */
    signed char Unk1F;                 /* 0x1F item box cursor (0..63) */
    signed char Unk20;                 /* 0x20 */
    UCHAR pad21;
    SHORT Unk22;                       /* 0x22 */
    SHORT Unk24;                       /* 0x24 */
    UCHAR pad26;
    UCHAR Unk27;                       /* 0x27 task running */
    UCHAR Unk28;                       /* 0x28 */
    UCHAR Unk29;                       /* 0x29 */
    USHORT Unk2A;                      /* 0x2A */
    UCHAR pad2C[0x2E - 0x2C];
    UCHAR Unk2E;                       /* 0x2E */
    UCHAR Unk2F;                       /* 0x2F */
    UCHAR Unk30;                       /* 0x30 */
    UCHAR pad31;
    SHORT Unk32;                       /* 0x32 */
    UCHAR Unk34;                       /* 0x34 condition (St_condition_get) */
    UCHAR Unk35;                       /* 0x35 life to restore */
    UCHAR Unk36;                       /* 0x36 cures poison */
    UCHAR pad37[0x3A - 0x37];
    UCHAR Unk3A;                       /* 0x3A */
    UCHAR Unk3B;                       /* 0x3B */
    UCHAR Unk3C;                       /* 0x3C */
    UCHAR Unk3D;                       /* 0x3D */
    UCHAR Unk3E;                       /* 0x3E */
    UCHAR pad3F[0x42 - 0x3F];
    ITEM_WORK Unk42;                     /* 0x42 copy of the selected item entry */
    UCHAR pad46[0x48 - 0x46];
    ITEM_WORK *Unk48;                    /* 0x48 combine: item A */
    ITEM_WORK *Unk4C;                    /* 0x4C combine: item B */
    UCHAR *Unk50;                      /* 0x50 Item_data_tbl entry of Unk48 */
    UCHAR *Unk54;                      /* 0x54 Item_data_tbl entry of Unk4C */
    UCHAR *Unk58;                      /* 0x58 combine table record (8 bytes, Item_mix_tbl) */
    UCHAR *Unk5C;                      /* 0x5C combine: slot no of item A */
    UCHAR *Unk60;                      /* 0x60 combine: slot no of item B */
    UCHAR Unk64;                       /* 0x64 combine result kind */
    UCHAR Unk65;                       /* 0x65 combine result count */
    SHORT Unk66;                       /* 0x66 */
    SHORT Unk68;                       /* 0x68 */
    UCHAR Unk6A;                       /* 0x6A weapon of a combine (Mix_tama_tbl) */
    UCHAR pad6B[0x6E - 0x6B];
    UCHAR Unk6E;                       /* 0x6E */
    UCHAR Unk6F;                       /* 0x6F */
    UCHAR Unk70;                       /* 0x70 */
    signed char Unk71;                 /* 0x71 */
    signed char Unk72;                 /* 0x72 */
    UCHAR Unk73;                       /* 0x73 */
    UCHAR pad74[0x76 - 0x74];
    UCHAR Unk76;                       /* 0x76 */
    UCHAR Unk77;                       /* 0x77 */
    UCHAR pad78[0x7C - 0x78];
    LONG Unk7C;                        /* 0x7C */
    LONG Unk80;                        /* 0x80 */
    LONG Unk84;                        /* 0x84 */
    LONG Unk88;                        /* 0x88 */
    LONG Unk8C;                        /* 0x8C */
    LONG Unk90;                        /* 0x90 */
    LONG Unk94;                        /* 0x94 */
    LONG Unk98;                        /* 0x98 */
    LONG Unk9C;                        /* 0x9C */
    LONG UnkA0;                        /* 0xA0 */
    LONG UnkA4;                        /* 0xA4 */
    UCHAR UnkA8;                       /* 0xA8 */
    UCHAR UnkA9;                       /* 0xA9 */
    UCHAR UnkAA;                       /* 0xAA */
    UCHAR UnkAB;                       /* 0xAB */
    UCHAR UnkAC;                       /* 0xAC */
    UCHAR UnkAD;                       /* 0xAD */
    signed char UnkAE;                 /* 0xAE */
    signed char UnkAF;                 /* 0xAF */
    UCHAR UnkB0;                       /* 0xB0 */
    UCHAR UnkB1;                       /* 0xB1 */
    UCHAR padB2[0xB4 - 0xB2];
    ULONG *pUnkB4;                     /* 0xB4 map data: word offsets to ST_MAP_ROOM/ST_MAP_INFO tables and (room, cut, flag, kind) lists */
    USHORT UnkB8;                      /* 0xB8 */
    UCHAR UnkBA;                       /* 0xBA */
    signed char UnkBB;                 /* 0xBB */
    UCHAR UnkBC;                       /* 0xBC file number */
    UCHAR UnkBD;                       /* 0xBD page number */
    signed char UnkBE;                 /* 0xBE blink timer */
    UCHAR UnkBF;                       /* 0xBF */
    UCHAR UnkC0;                       /* 0xC0 */
    UCHAR UnkC1;                       /* 0xC1 */
    UCHAR padC2[0xC4 - 0xC2];
    SHORT UnkC4;                       /* 0xC4 */
    SHORT UnkC6;                       /* 0xC6 */
    SHORT UnkC8;                       /* 0xC8 */
    UCHAR UnkCA;                       /* 0xCA */
    UCHAR UnkCB;                       /* 0xCB */
    ULONG *UnkCC;                      /* 0xCC MAP.MAP data */
    UCHAR UnkD0;                       /* 0xD0 clut rows of the status textures */
    UCHAR UnkD1;                       /* 0xD1 */
    UCHAR UnkD2;                       /* 0xD2 */
    UCHAR UnkD3;                       /* 0xD3 */
    UCHAR UnkD4;                       /* 0xD4 */
    UCHAR UnkD5;                       /* 0xD5 */
    UCHAR UnkD6;                       /* 0xD6 */
    UCHAR UnkD7;                       /* 0xD7 */
    UCHAR UnkD8;                       /* 0xD8 */
    UCHAR padD9[0xE0 - 0xD9];
    UCHAR *UnkE0;                      /* 0xE0 */
    ST_POS Pos[32];                    /* 0xE4 panel positions, indexed by the prm>>8 byte of the prim setters */
    DR_TPAGE Ot[2][40];                /* 0x164 per-buffer ordering nodes, index prm>>16 of the prim setters */
    DR_TPAGE Tpage[2][2];              /* 0x3E4 */
    DR_TPAGE Tp404[4];                 /* 0x404 */
    DR_TPAGE Tp424[2];                 /* 0x424 */
    DR_TPAGE Tp434[2];                 /* 0x434 */
} ST_WORK;
extern ST_WORK St;                     /* 0x800E01C0 RE2 St */

extern char D_80010DCC[];
extern char D_80010F84[];
extern char D_80010FEC[];
extern UCHAR File_page_sum[];
extern ULONG File_page_size[];
extern USHORT File_page_top[];
extern USHORT File_page_num[];
extern UCHAR D_8009F2EC[];
extern UCHAR D_8009F2F8[];
extern UCHAR D_8009F304[];
extern UCHAR D_8009F310[];
extern UCHAR D_8009F31C[];
extern UCHAR D_8009F328[];
extern UCHAR D_8009F34C[];
extern USHORT D_8009F388[];
extern USHORT D_8009F3E8[];
extern UCHAR D_8009F3F4[];
extern UCHAR D_8009F418[];
extern USHORT D_8009F424[];
extern UCHAR D_8009F448[];
extern SHORT D_8009F49C[];
extern UCHAR D_8009F4A8[];
extern UCHAR D_8009F4B4[];
extern SHORT D_8009F4C0[];
extern UCHAR D_8009F4CC[];
extern UCHAR D_8009F4D8[];
extern void (*St_box_tbl[])(ST_WORK *pSt);
extern USHORT Box_pix_pos[][2];
extern UCHAR Box_ng_item[];
extern UCHAR Itemi_sum[];
extern UCHAR Itemg_sum[];
extern ULONG Item_pix_ofs[];
extern UCHAR D_8009F890[];
extern UCHAR D_8009F8A8[];
extern UCHAR D_8009F8B4[];
extern UCHAR D_8009F8C0[];
extern UCHAR D_8009F8CC[];
extern UCHAR D_8009F908[];
extern UCHAR D_8009F95C[];
extern USHORT D_8009F9B0[];
extern UCHAR D_8009F9D4[];
extern ST_ICON File_icon_tbl[];
extern UCHAR D_8009FB78[];
extern UCHAR D_8009FB84[];
extern SHORT D_8009FB90[];
extern UCHAR D_8009FBA8[];
extern UCHAR D_8009FBC0[];
extern UCHAR D_8009FBCC[];
extern UCHAR D_8009FBFC[];
extern UCHAR D_8009FC2C[];
extern UCHAR D_8009FC38[];
extern UCHAR D_8009FC44[];
extern UCHAR D_8009FC50[];
extern USHORT D_8009FC5C[];
extern USHORT D_8009FCBC[];
extern USHORT D_8009FD1C[];
extern USHORT D_8009FD7C[];
extern USHORT D_8009FDDC[];
extern USHORT D_8009FE0C[];
extern USHORT D_8009FE3C[];
extern USHORT D_8009FE6C[];
extern USHORT D_8009FE9C[];
extern USHORT D_8009FECC[];
extern USHORT D_8009FEFC[];
extern USHORT D_8009FF2C[];
extern USHORT D_8009FF38[];
extern USHORT D_8009FF44[];
extern USHORT D_8009FF50[];
extern SHORT D_8009FF5C[];
extern UCHAR D_8009FF68[];
extern UCHAR D_8009FF74[];
extern UCHAR D_8009FF80[];
extern UCHAR D_8009FF8C[];
extern UCHAR D_8009FFB0[];
extern UCHAR D_8009FFBC[];
extern UCHAR D_8009FFC8[];
extern UCHAR D_8009FFD4[];
extern UCHAR D_8009FFE0[];
extern UCHAR D_8009FFEC[];
extern UCHAR D_8009FFF8[];
extern UCHAR Cond_spr_tbl[];
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
extern UCHAR Item_data_tbl[][4];
extern UCHAR Item_mix_tbl[];
extern UCHAR Item_check_tbl[];
extern UCHAR Item_tama_tbl[];
extern LONG D_800C79CC; /* extra game character (Status_init) */
extern ULONG D_800CC840;
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
extern UCHAR D_800DBB58; /* status flags (0x80 tested by the status screen code) */
extern UCHAR D_800DBB5B;

void St_file_load(ST_WORK *pSt);
void St_file_read(ST_WORK *pSt);
void St_file_end(ST_WORK *pSt);
void St_file_prim_init(ST_WORK *pSt);
void St_file_draw(ST_WORK *pSt);
void St_file_page_load(ST_WORK *pSt, UCHAR a, UCHAR b);
void St_sprt_draw_tp(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm);
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
void St_icon_swap(UCHAR a, UCHAR b);
void St_icon_copy(UCHAR dst, UCHAR src);
void St_box_list_draw(ST_WORK *pSt, UCHAR no, SHORT y0);
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
UCHAR St_cmd_slide(ST_WORK *pSt, LONG a1);
void St_item_shift(ST_WORK *pSt);
void St_item_get(ST_WORK *pSt);
void St_item_change(ST_WORK *pSt);
void St_item_to_file(ST_WORK *pSt);
void St_exit_set(ST_WORK *pSt);
UCHAR *Search_item_mix(ST_WORK *pSt, UCHAR a, UCHAR b);
UCHAR *Search_item_check(ST_WORK *pSt, UCHAR id);
UCHAR *Search_item_tama(ST_WORK *pSt, UCHAR id);
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
signed char Item_decrease(UCHAR no, signed char n, UCHAR f);
void Item_delete(UCHAR no);
void Equip_item_delete(void);
signed char Tama_reload(UCHAR f);
LONG Tama_decrease(UCHAR f);
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
void St_sprt_init(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm);
void St_ft4_init(ST_WORK *pSt, POLY_FT4 *p, USHORT *src, ULONG prm);
void St_tile_init(ST_WORK *pSt, TILE *p, SHORT *pWh, UCHAR n);
void St_line_init(ST_WORK *pSt);
void St_sprt_draw(ST_WORK *pSt, SPRT *p, UCHAR *src, ULONG prm);
void St_ft4_draw(ST_WORK *pSt, POLY_FT4 *p, USHORT *src, ULONG prm);
void St_tile_draw(ST_WORK *pSt, TILE *p, SHORT *src, ULONG prm);
void St_mapping_tim(void);
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
void Map_visit_set(UCHAR a, UCHAR b);
ULONG Map_visit_ck(UCHAR a, UCHAR b);
LONG Map_room_disp_ck(UCHAR st, UCHAR rm);
LONG Map_part_disp_ck(UCHAR st, UCHAR rm, UCHAR cut);
LONG Map_part_clut(UCHAR st, UCHAR rm, UCHAR cut);
LONG Map_part_blink_ck(UCHAR st, UCHAR rm, UCHAR cut);
LONG Map_door_disp_ck(UCHAR st, UCHAR rm, UCHAR cut);
LONG Map_door_mark_ck(UCHAR st, UCHAR rm, UCHAR cut);
LONG Map_floor_get(ST_WORK *pSt, UCHAR st, UCHAR rm);
void Map_room_conv(UCHAR *pA, UCHAR *pB);
void St_frame_end(void);

#endif /* STATUS_H */
