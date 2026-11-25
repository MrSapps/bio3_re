#ifndef SCE_AT_H
#define SCE_AT_H
/* RE2 SCE_AT.C: AOT (area of trigger) handlers: doors, items, messages, item box */
#include "bio3.h"

/* item AOT data (RE2 struct tagSCE_ITEM_DATA, SCE_AT.H) */
typedef struct tagSCE_ITEM_DATA {
    SHORT Data0;                       /* 0x0 */
    SHORT Data1;                       /* 0x2 */
    USHORT Data2;                      /* 0x4 item flag bit */
    UCHAR Om_no;                       /* 0x6 */
    UCHAR Free;                        /* 0x7 */
} SCE_ITEM_DATA;

extern USHORT Key_type[][16]; /* 0x8009CC9C RE2 Key_type, row Main.Unk195 */
extern LONG D_800DEC98;
extern SHORT D_800DEC9C; /* RE2 Item_box_entrance static Dir_spd */
extern SHORT D_800DEC9E; /* RE2 Item_box_entrance static Spd_add */
extern OBJ_MODEL_WORK *D_800DECA0; /* RE2 Item_box_entrance static pOwork */
extern void *Hiki_pAot; /* 0x800DECA4 RE2 Hikidashi_entrance static pAot */
extern SCE_ITEM_DATA *Hiki_pItem_dat; /* 0x800DECA8 RE2 Hikidashi_entrance static pItem_dat */
extern ULONG D_800DECAC;
extern UCHAR *pCAot; /* 0x800DECB0 RE2 pCAot */
extern ULONG Old_Fatari; /* 0x800DECB4 RE2 Old_Fatari */
extern ULONG Old_Uatari; /* 0x800DECB8 RE2 Old_Uatari */
extern UCHAR St_type_a[];            /* St.St_type seen as an array (stores stay after loads) */

ULONG Sce_at_check(void *pEm, ULONG Attribute, ULONG Auto_flg); /* RE2 Sce_at_check */
void Sce_at(void);
ULONG Nothing(void *pAot);
ULONG Sce_Door(void *pAot);
void Key_lost(void);
void Door_ask_entrance(void);
ULONG Sce_Item(void *pAot);
void Item_mess_entrance(void);
LONG Sce_Normal(void);
ULONG Sce_Message(void *pAot);
ULONG Sce_Event(void *pAot);
ULONG Sce_Flg_chg(void *pAot);
ULONG Sce_Water(void *pAot);
ULONG Sce_Save(void *pAot);
void Save_entrance(void);
ULONG Sce_Itembox(void *pAot);
void Item_box_entrance(void);
ULONG Sce_Aot_set(void *pAot);
ULONG Sce_Hikidashi(void *pAot);
void Hikidashi_entrance(void);
ULONG Sce_Damage(void *pAot);
ULONG Sce_Door2(void *pAot);
ULONG Sce_Heal(void *pAot);
void Heal_entrance(void);
LONG Water_ck(LONG Pos_x, LONG Pos_z);

#endif /* SCE_AT_H */
