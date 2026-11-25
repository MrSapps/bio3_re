#ifndef ESP_H
#define ESP_H
/*
 * RE2 ESP.C / ESP_R.C: effect sprites. RE3 keeps the 96-entry work pool (RE2 Esp[96])
 * but its work layout (0xD4 bytes) differs from RE2's ESP_WORK.
 */
#include "bio3.h"

/* sprite animation step (RE2 struct SSEQUENCE, 8 bytes) */
typedef struct tagSSEQUENCE {
    UCHAR Unk0;                        /* 0x0 next step (when Time is a jump code) */
    UCHAR nSpr;                        /* 0x1 RE2 nSpr */
    UCHAR Time;                        /* 0x2 RE2 Time: frames, copied to ESP_WORK.Timer */
    UCHAR Unk3;                        /* 0x3 */
    ULONG Unk4;                        /* 0x4 */
} SSEQUENCE;

/* sprite group entry (Esp_trans_main) */
typedef struct tagSPR_GP {
    UCHAR U;                           /* 0x0 */
    UCHAR V;                           /* 0x1 */
    signed char Ofsx;                  /* 0x2 */
    signed char Ofsy;                  /* 0x3 */
} SPR_GP;

/* effect work (RE2 ESP_WORK role, RE3 layout, 0xD4 bytes; pool Esp[96]) */
typedef struct tagESP_WORK {
    UCHAR Routine_0;                   /* 0x00 0: free */
    UCHAR Routine_1;                   /* 0x01 */
    UCHAR Routine_2;                   /* 0x02 */
    UCHAR Unk03;                       /* 0x03 */
    USHORT Attr;                       /* 0x04 */
    UCHAR pad06[0x8 - 0x6];
    SHORT Unk08;                       /* 0x08 sprite scale x (Esp_trans_main) */
    SHORT Unk0A;                       /* 0x0A sprite scale y */
    signed char Acc_x;                 /* 0x0C */
    signed char Acc_y;                 /* 0x0D */
    signed char Acc_z;                 /* 0x0E */
    UCHAR Unk0F;                       /* 0x0F */
    SHORT Spd_x;                       /* 0x10 */
    SHORT Spd_y;                       /* 0x12 */
    SHORT Spd_z;                       /* 0x14 */
    USHORT Unk16;                      /* 0x16 or'ed into Tpage (Esp_call) */
    UCHAR Unk18;                       /* 0x18 */
    UCHAR Snd_no;                      /* 0x19 */
    UCHAR Unk1A;                       /* 0x1A */
    UCHAR Unk1B;                       /* 0x1B */
    USHORT Unk1C;                      /* 0x1C */
    USHORT Unk1E;                      /* 0x1E */
    USHORT Unk20;                      /* 0x20 */
    USHORT Unk22;                      /* 0x22 */
    USHORT Be_flg;                     /* 0x24 */
    USHORT Flg2;                       /* 0x26 */
    union {
        USHORT W;                      /* 0x28 effect id (Esp_search) */
        UCHAR B[2];                    /* 0x28 [0] kind, [1] (esp_r) */
    } Id;
    UCHAR Unk2A;                       /* 0x2A */
    UCHAR Timer;                       /* 0x2B */
    USHORT Unk2C;                      /* 0x2C */
    USHORT Unk2E;                      /* 0x2E */
    SHORT Unk30;                       /* 0x30 offset added to Unk38 (Esp_trans_main) */
    SHORT Unk32;                       /* 0x32 */
    SHORT Unk34;                       /* 0x34 */
    UCHAR pad36[0x38 - 0x36];
    LONG Unk38[3];                     /* 0x38 base position */
    USHORT Clut;                       /* 0x44 (Esp_call) */
    USHORT Tpage;                      /* 0x46 */
    VECTOR Pos;                        /* 0x48 Pos.pad: ground height (Esp_hit_ck) */
    VECTOR Old;                        /* 0x58 previous position (Sca_ck_line, Esp_37) */
    SHORT Rot[3];                      /* 0x68 rotation (Esp_trans_main) */
    UCHAR pad6E;
    UCHAR Seq_no;                      /* 0x6F index into pSeq */
    MATRIX Workm;                      /* 0x70 local matrix */
    MATRIX Workm2;                     /* 0x90 copy of Workm (Esp_pos_matrix) */
    UCHAR Col[3];                      /* 0xB0 prim colour (Esp_call); UnkB3 is the prim code byte */
    UCHAR UnkB3;                       /* 0xB3 */
    SHORT UnkB4;                       /* 0xB4 */
    SHORT UnkB6;                       /* 0xB6 */
    SHORT UnkB8;                       /* 0xB8 */
    SHORT UnkBA;                       /* 0xBA */
    struct tagENEMY_WORK *pEm;         /* 0xBC enemy the effect belongs to (Esp_29) */
    MATRIX *pMat;                      /* 0xC0 */
    SSEQUENCE *pSeq;                   /* 0xC4 */
    SPR_GP *pSprg;                     /* 0xC8 sprite groups */
    ULONG *pUnkCC;                     /* 0xCC current move record */
    struct tagESP_WORK *pLink;         /* 0xD0 killed when the linked work dies */
} ESP_WORK;
extern ESP_WORK Esp[96];        /* RE2 ESP.H Esp[96] */

void Esp_kill(ULONG Id, MATRIX *pM); /* kill the effects of Id attached to pM */

void Esp_init_C(void);
LONG Esp_init_R(void);
ULONG Esp_data_set0(UCHAR *pData, ULONG *pOfs, ULONG Mode);
void Esp_data_set1(ULONG *pTim);
/* what Esp_call_child / Esp_call and the other effect starters return when no effect was started */
#define ESP_NONE ((ESP_WORK *)-1)
ESP_WORK *Esp_call_child(ESP_WORK *pParent, ULONG a1, ULONG a2, MATRIX *pM, LONG *pPos, SVECTOR *pSpd);
ESP_WORK *Esp_call(ULONG Id, ULONG Dir, MATRIX *pM, SVECTOR *pPos, SVECTOR *pSpd);
void Esp_ctrl();
void Esp_task_main(void);
void Esp_speed_add(ESP_WORK *pEw);
void Esp_pos_matrix(ESP_WORK *pEw);
void Esp_hit_ck(ESP_WORK *pEw);
void Esp_anm_move(ESP_WORK *pEw);
UCHAR *Esp_work_get(void);
ESP_WORK *Esp_search(USHORT Id);
void Esp_00(void);
void Esp_01(ESP_WORK *pEw);
void Esp_02(ESP_WORK *pEw);
void Esp_03(ESP_WORK *pEw);
void Esp_04(ESP_WORK *pEw);
void Esp_05(ESP_WORK *pEw);
void Esp_06(ESP_WORK *pEw);
void Esp_07(ESP_WORK *pEw);
void Esp_08(void);
void Esp_09(void);
void Esp_0a(void);
void Esp_0b(void);
void Esp_0c(void);
void Esp_0d(void);
void Esp_0e(void);
void Esp_0f(void);
void func_8001CDA4(ESP_WORK *pEw, ULONG n); /* random start frame of the sequence */
void func_8001CE24(ESP_WORK *pEw, ULONG a1, ULONG a2); /* random byte table, indexed by Rnd() */
void func_8001CF24(ESP_WORK *pEw, ULONG a1, ULONG a2);
void func_8001CFF4(ESP_WORK *pEw, ULONG n);
void Esp_10(ESP_WORK *pEw);
void Esp_11(UCHAR *p);
void Esp_12(ESP_WORK *pEw);
void Esp_13(void);
void Esp_14(void);
void Esp_15(ESP_WORK *pEw);
void Esp_16(ESP_WORK *pEw);
void Esp_17(ESP_WORK *pEsp);
void Esp_18(ESP_WORK *pEw);
void Esp_19(ESP_WORK *pEw);
void Esp_1a(UCHAR *p);
void Esp_1b(ESP_WORK *pEw);
void Esp_1c(ESP_WORK *pEw);
void Esp_1d(ESP_WORK *pEw);
void Esp_1e(ESP_WORK *pEw);
void Esp_1f(ESP_WORK *pEw);
void Esp_20(ESP_WORK *pEw);

#endif /* ESP_H */
