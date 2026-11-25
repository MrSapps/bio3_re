#ifndef ESP_R_H
#define ESP_R_H
/* RE2 ESP_R.C: effect routines (the Esp_* effect pool itself is in esp.h) */
#include "bio3.h"
#include "em_sub.h"
#include "esp.h"

/* effect burst table entry (Esp_34) */
typedef struct tagESP_TBL16 {
    SHORT Size;                        /* 0x0 */
    SHORT Id;                          /* 0x2 0: skip, 0xFF: end */
    USHORT Attr;                       /* 0x4 */
    SHORT Ofs;                         /* 0x6 */
    SHORT Unk8;                        /* 0x8 */
    SHORT UnkA;                        /* 0xA */
    SHORT UnkC;                        /* 0xC */
    SHORT UnkE;                        /* 0xE */
} ESP_TBL16;

extern USHORT D_80097B78[];
extern SHORT D_80098074[];
extern SHORT D_8009807C[];
extern ESP_TBL16 D_80098094[];
extern ESP_TBL16 D_800982C4[];
extern ESP_TBL16 D_80098494[];
extern ULONG D_800986C4[]; /* scale curve (Esp_35) */
extern ENEMY_WORK *D_800DEB80;

void Esp_21(ESP_WORK *pEsp);
void Esp_22(ESP_WORK *pEsp);
void Esp_23(ESP_WORK *pEsp);
void Esp_24(ESP_WORK *pEsp);
void Esp_25(ESP_WORK *pEsp);
void Esp_26(ESP_WORK *pEsp);
void Esp_27(ESP_WORK *pEsp);
void Esp_28(ESP_WORK *pEsp);
void Esp_29(ESP_WORK *pEsp);
void Esp_2a(ESP_WORK *pEsp);
void Esp_2b(ESP_WORK *pEsp);
void Esp_2c(void);
void Esp_2d(void);
void Esp_2e(void);
void Esp_2f(void);
void Esp_30(void);
void Esp_31(void);
void Esp_32(ESP_WORK *pEsp);
void Esp_33(ESP_WORK *pEsp);
void Esp_34(ESP_WORK *pEsp);
void Esp_35(ESP_WORK *pEsp);
void Esp_36(void);
void Esp_37(ESP_WORK *pEsp);
void Esp_38(ESP_WORK *pEsp);
void Esp_39(void);
void Esp_3a(void);
void Esp_3b(ESP_WORK *pEsp);
void Esp_3c(ESP_WORK *pEsp);
void Esp_3d(void);
void Esp_3e(ESP_WORK *pEsp);
void Esp_3f(ESP_WORK *pEsp);

#endif /* ESP_R_H */
