#ifndef ESP_R_H
#define ESP_R_H
/* RE2 ESP_R.C: effect routines (the Esp_* effect pool itself is in esp.h) */
#include "bio3.h"
#include "em_sub.h"
#include "esp.h"

/* effect burst table entry (func_8002198C) */
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
extern ULONG D_800986C4[]; /* scale curve (func_80021D58) */
extern ENEMY_WORK *D_800DEB80;

void func_80020298(ESP_WORK *pEsp);
void func_800204B8(ESP_WORK *pEsp);
void func_80020700(ESP_WORK *pEsp);
void func_80020820(ESP_WORK *pEsp);
void func_800209D8(ESP_WORK *pEsp);
void func_80020B6C(ESP_WORK *pEsp);
void func_80020C0C(ESP_WORK *pEsp);
void func_80020DBC(ESP_WORK *pEsp);
void func_80020EB8(ESP_WORK *pEsp);
void func_80021548(ESP_WORK *pEsp);
void func_8002174C(ESP_WORK *pEsp);
void func_80021784(void);
void func_8002178C(void);
void func_80021794(void);
void func_8002179C(void);
void func_800217A4(void);
void func_800217AC(void);
void func_800217B4(ESP_WORK *pEsp);
void func_800218BC(ESP_WORK *pEsp);
void func_8002198C(ESP_WORK *pEsp);
void func_80021D58(ESP_WORK *pEsp);
void func_80021EA4(void);
void func_80021EAC(ESP_WORK *pEsp);
void func_800221F0(ESP_WORK *pEsp);
void func_80022398(void);
void func_800223A0(void);
void func_800223A8(ESP_WORK *pEsp);
void func_80022500(ESP_WORK *pEsp);
void func_80022660(void);
void func_80022668(ESP_WORK *pEsp);
void func_8002289C(ESP_WORK *pEsp);

#endif /* ESP_R_H */
