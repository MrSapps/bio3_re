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
extern SHORT D_800D1F8E[];
extern USHORT D_800D1F94;
extern ULONG D_800D258C[];
extern UCHAR D_800E0171[];
extern ULONG D_800E01D4[];
extern UCHAR D_800E01E8;

ULONG func_80055A18(SCE_TASK *pSce);
ULONG func_80055AF0(SCE_TASK *pSce);
ULONG func_80055B30(SCE_TASK *pSce);
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
ULONG func_80056E28(SCE_TASK *pSce);
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
ULONG func_800574B8(SCE_TASK *pSce);
ULONG Item_aot_set(SCE_TASK *pSce);
ULONG Item_aot_set_4p(SCE_TASK *pSce);
ULONG Kage_set(SCE_TASK *pSce);
ULONG Super_set(SCE_TASK *pSce);
ULONG Weapon_chg(SCE_TASK *pSce);
ULONG func_80057CF8(SCE_TASK *pSce);
ULONG func_80057DD8(SCE_TASK *pSce);
ULONG func_80057E28(SCE_TASK *pSce);
ULONG Flr_set(SCE_TASK *pSce);
ULONG Sce_Item_lost(SCE_TASK *pSce);
ULONG func_80057F44(SCE_TASK *pSce);
ULONG func_80057F84(SCE_TASK *pSce);
ULONG Sce_parts_bomb(SCE_TASK *pSce);
ULONG Sce_parts_down(SCE_TASK *pSce);
ULONG func_8005879C(SCE_TASK *pSce);
ULONG func_80058800(SCE_TASK *pSce);
void func_800588F0(SCE_TASK *pSce);
ULONG Parts_set(SCE_TASK *pSce);
ULONG func_800589FC(SCE_TASK *pSce);
ULONG func_80058B00(SCE_TASK *pSce);
ULONG func_80058B84(SCE_TASK *pSce);
ULONG func_80058C24(SCE_TASK *pSce);
ULONG func_80058C70(SCE_TASK *pSce);
ULONG func_80058CD0(SCE_TASK *pSce);

#endif /* SCE2_H */
