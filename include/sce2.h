#ifndef SCE2_H
#define SCE2_H
/* RE2 SCE2.C: scenario script opcode handlers, second half */
#include "bio3.h"
#include "sce0.h"

extern ULONG (*Sce_at_jump_tbl[])(void *pAot); /* 0x8009E0BC RE2 Sce_at_jump_tbl */
extern ULONG *D_8009E414; /* Flg_addr[7] */
extern ULONG *D_8009E418; /* Flg_addr[8] */
extern UCHAR Subpl_walk[][2]; /* RE2 SPL_TBL.H: sub player motion tables */
extern UCHAR Subpl_back[][2];
extern UCHAR Subpl_dir[][2];
extern UCHAR Subpl_ato[][2];
extern UCHAR Subpl_run[][2];
extern UCHAR D_800C703E[];
extern UCHAR D_800C7961;
extern UCHAR D_800E0171[];
extern ULONG St_flg14_a[];          /* St.Flg14 seen as its own array symbol (codegen) */
extern UCHAR St_equip_id_old;        /* St.Equip_id_old under its own symbol */

ULONG Item_get_ck(SCE_TASK *pSce);
ULONG Sce_dir_ck(SCE_TASK *pSce);
ULONG Diedemo_on(SCE_TASK *pSce);
ULONG Door_aot_set(SCE_TASK *pSce);
ULONG Door_aot_set_4p(SCE_TASK *pSce);
ULONG Aot_set(SCE_TASK *pSce);
ULONG Aot_set_4p(SCE_TASK *pSce);
ULONG Aot_reset(SCE_TASK *pSce);
ULONG Aot_on(SCE_TASK *pSce);
MATRIX *Get_matrix(s8 Work_kind, s8 Work_no);
ULONG Sce_espr_on(SCE_TASK *pSce);
ULONG Sce_espr_on2(SCE_TASK *pSce);
ULONG Sce_espr3d_on(SCE_TASK *pSce);
ULONG Sce_espr3d_on2(SCE_TASK *pSce);
ULONG Sce_espr_kill(SCE_TASK *pSce);
ULONG Sce_espr_kill2(SCE_TASK *pSce);
ULONG Sce_espr_task(SCE_TASK *pSce);
ULONG Sce_espr_control(SCE_TASK *pSce);
ULONG Obj_model_set(SCE_TASK *pSce);
ULONG Mizu_div_set(SCE_TASK *pSce);
ULONG Sce_em_set(SCE_TASK *pSce);
ULONG Plc_motion(SCE_TASK *pSce);
ULONG Plc_dest2(SCE_TASK *pSce);
ULONG Plc_dest(SCE_TASK *pSce);
ULONG Plc_gun(SCE_TASK *pSce);
ULONG Plc_gun_eff(SCE_TASK *pSce);
ULONG Plc_neck(SCE_TASK *pSce);
ULONG Plc_ret(SCE_TASK *pSce);
ULONG Plc_stop(SCE_TASK *pSce);
ULONG Plc_flg(SCE_TASK *pSce);
ULONG Plc_rot(SCE_TASK *pSce);
ULONG Plc_cnt(SCE_TASK *pSce);
ULONG Splc_ret(SCE_TASK *pSce);
ULONG Splc_sce(SCE_TASK *pSce);
ULONG Splc_sce2(SCE_TASK *pSce);
ULONG Item_aot_set(SCE_TASK *pSce);
ULONG Item_aot_set_4p(SCE_TASK *pSce);
ULONG Kage_set(SCE_TASK *pSce);
ULONG Super_set(SCE_TASK *pSce);
ULONG Weapon_chg(SCE_TASK *pSce);
ULONG Weapon_chg_old(SCE_TASK *pSce);
ULONG Parts_tmd_swap(SCE_TASK *pSce);
ULONG Status_on(SCE_TASK *pSce);
ULONG Flr_set(SCE_TASK *pSce);
ULONG Sce_Item_lost(SCE_TASK *pSce);
ULONG Sel_evt_on(SCE_TASK *pSce);
ULONG Chaser_item_set(SCE_TASK *pSce);
ULONG Sce_parts_bomb(SCE_TASK *pSce);
ULONG Sce_parts_down(SCE_TASK *pSce);
ULONG Chaser_life_init(SCE_TASK *pSce);
ULONG Cut_flg_set(SCE_TASK *pSce);
void Sce_ctrl_mode_set(SCE_TASK *pSce);
ULONG Parts_set(SCE_TASK *pSce);
ULONG Sce_em_reset(SCE_TASK *pSce);
ULONG Ahead_room_set(SCE_TASK *pSce);
ULONG Sce_door_ck(SCE_TASK *pSce);
ULONG Sce_quake_add(SCE_TASK *pSce);
ULONG Sce_map_open(SCE_TASK *pSce);
ULONG Chaser_evt_clr(SCE_TASK *pSce);

#endif /* SCE2_H */
