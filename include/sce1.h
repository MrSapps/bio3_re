#ifndef SCE1_H
#define SCE1_H
/* RE2 SCE1.C: scenario script opcode handlers (SCE_TASK, see sce0.h) */
#include "bio3.h"
#include "em_sub.h"
#include "sce0.h"

typedef struct {
    LONG Unk0;                         /* 0x00 */
    LONG Pos[3];                       /* 0x04 */
    LONG Tgt[3];                       /* 0x10 */
} SCE_CAMERA;

extern ULONG *Flg_addr[]; /* 0x8009E3F8 RE2 Flg_addr */
extern SCE_CAMERA D_800E0150;
extern UCHAR D_800E0175[];
extern UCHAR D_800E01C6[];

ULONG Nop(SCE_TASK *pSce);
ULONG Evt_end(SCE_TASK *pSce);
ULONG Evt_next(SCE_TASK *pSce);
ULONG Evt_chain(SCE_TASK *pSce);
ULONG Evt_exec(SCE_TASK *pSce);
ULONG Evt_skip_set(SCE_TASK *pSce);
ULONG Evt_kill(SCE_TASK *pSce);
ULONG Ifel_ck(SCE_TASK *pSce);
ULONG Else_ck(SCE_TASK *pSce);
ULONG Endif(SCE_TASK *pSce);
ULONG Sleep(SCE_TASK *pSce);
ULONG Sleeping(SCE_TASK *pSce);
ULONG Wsleep(SCE_TASK *pSce);
ULONG Wsleeping(SCE_TASK *pSce);
ULONG For(SCE_TASK *pSce);
ULONG For2(SCE_TASK *pSce);
ULONG Next(SCE_TASK *pSce);
ULONG While(SCE_TASK *pSce);
ULONG Ewhile(SCE_TASK *pSce);
ULONG Do(SCE_TASK *pSce);
ULONG Edwhile(SCE_TASK *pSce);
ULONG While_main(SCE_TASK *pSce, UCHAR *pData, UCHAR Ofs); /* RE2 While_main */
ULONG Switch(SCE_TASK *pSce);
ULONG Case(SCE_TASK *pSce);
ULONG Default(SCE_TASK *pSce);
ULONG Eswitch(SCE_TASK *pSce);
ULONG Goto(SCE_TASK *pSce);
ULONG Gosub(SCE_TASK *pSce);
ULONG Return(SCE_TASK *pSce);
ULONG Break(SCE_TASK *pSce);
ULONG Break_point(SCE_TASK *pSce);
ULONG Work_copy(SCE_TASK *pSce);
ULONG Save(SCE_TASK *pSce);
ULONG Copy(SCE_TASK *pSce);
ULONG Calc(SCE_TASK *pSce);
ULONG Calc2(SCE_TASK *pSce);
void Calc_branch(ULONG Exp, SHORT *Ans, ULONG Num); /* RE2 Calc_branch */
ULONG Member_set(SCE_TASK *pSce);
ULONG Member_set2(SCE_TASK *pSce);
ULONG Member_copy(SCE_TASK *pSce);
ULONG Member_cmp(SCE_TASK *pSce);
ULONG Member_calc(SCE_TASK *pSce);
ULONG Member_calc2(SCE_TASK *pSce);
void Member_set_branch(ENEMY_WORK *pW, ULONG Member, LONG D0); /* RE2 Member_set_branch */
LONG Load_member_branch(ENEMY_WORK *pW, ULONG Member); /* RE2 Load_member_branch */
SHORT *Load_member_addr_branch(ENEMY_WORK *pW, ULONG Member); /* RE2 Load_member_addr_branch */
ULONG Sce_fade_set(SCE_TASK *pSce);
ULONG Work_set(SCE_TASK *pSce);
ULONG Speed_set(SCE_TASK *pSce);
ULONG Add_speed(SCE_TASK *pSce);
ULONG Add_aspeed(SCE_TASK *pSce);
ULONG Add_vspeed(SCE_TASK *pSce);
ULONG Ck(SCE_TASK *pSce);
ULONG Set(SCE_TASK *pSce);
ULONG Cmp(SCE_TASK *pSce);
ULONG Sce_rnd(SCE_TASK *pSce);
ULONG Cut_chg(SCE_TASK *pSce);
ULONG Cut_old(SCE_TASK *pSce);
void Cut_chg_main(ULONG Cut);
ULONG Cut_auto(SCE_TASK *pSce);
ULONG Cut_replace(SCE_TASK *pSce);
ULONG Cut_be_set(SCE_TASK *pSce);
ULONG Pos_set(SCE_TASK *pSce);
ULONG Dir_set(SCE_TASK *pSce);
ULONG Vib_set0(SCE_TASK *pSce);
ULONG Vib_set1(SCE_TASK *pSce);
ULONG Vib_fade_set(SCE_TASK *pSce);
ULONG Sce_rbj_set(SCE_TASK *pSce);
ULONG Message_on(SCE_TASK *pSce);
ULONG Message_off(SCE_TASK *pSce);
ULONG Sce_rain_set(SCE_TASK *pSce);
ULONG Sce_shake_on(SCE_TASK *pSce);
ULONG Sce_line_start(SCE_TASK *pSce);
ULONG Sce_line_main(SCE_TASK *pSce);
ULONG Sce_line_end(SCE_TASK *pSce);
ULONG Se_on(SCE_TASK *pSce);
ULONG Sce_bgm_control(SCE_TASK *pSce);
ULONG Xa_on(SCE_TASK *pSce);
ULONG Movie_on(SCE_TASK *pSce);
ULONG Sce_bgmtbl_set(SCE_TASK *pSce);
ULONG Sce_bgmtbl_ck(SCE_TASK *pSce);
ULONG Keep_Item_ck(SCE_TASK *pSce);
ULONG Sce_key_ck(SCE_TASK *pSce);
ULONG Sce_trg_ck(SCE_TASK *pSce);
ULONG Sca_id_set(SCE_TASK *pSce);
ULONG Om_flg_clr(SCE_TASK *pSce);
ULONG Om_rev(SCE_TASK *pSce);
ULONG Light_pos_set2(SCE_TASK *pSce);
ULONG Light_kido_set2(SCE_TASK *pSce);
ULONG Light_color_set2(SCE_TASK *pSce);

#endif /* SCE1_H */
