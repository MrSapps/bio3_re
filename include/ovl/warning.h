#ifndef OVL_WARNING_H
#define OVL_WARNING_H
/*
 * WARNING.BIN overlay (loaded at 0x80184000, no file-number word): the
 * mod-chip check. It talks to the CD controller registers directly
 * (GetTN, GetTD, Setloc, SeekP, ... and the test commands), and when the disc
 * does not behave like a pressed one it draws "SOFTWARE TERMINATED" with the
 * BIOS font and stops. warning.c and frame.c (not Capcom code) were built with
 * gcc 2.7.2 + GNU as, task.c with the game's toolchain (see the Makefile).
 */
#include "bio3.h"
#include "main.h"
#include "monitor.h"

/* CD controller command (Cd_cmd_tbl) */
typedef struct tagWARN_CD_CMD {
    UCHAR Cmd;                         /* 0x0 command byte */
    UCHAR Nparam;                      /* 0x1 parameter bytes (Cd_param) */
    UCHAR Nresult;                     /* 0x2 result bytes read into Cd_result */
    UCHAR Nint;                        /* 0x3 sum of interrupt numbers that ends it (3 ack, 3 + 2 ack + complete) */
} WARN_CD_CMD;

/* message layout per language (0 Japanese, 1 English) */
typedef struct tagWARN_LANG {
    UCHAR Font_w;                      /* 0x0 bytes per glyph row */
    UCHAR Unk1;                        /* 0x1 */
    UCHAR Char_w;                      /* 0x2 glyph width */
    UCHAR Unk3;                        /* 0x3 */
    SHORT X;                           /* 0x4 */
    SHORT Y;                           /* 0x6 */
    char *pMess;                       /* 0x8 */
} WARN_LANG;

/* text cursor (func_80184ECC / func_80185040) */
typedef struct tagWARN_POS {
    SHORT X;                           /* 0x0 */
    SHORT Y;                           /* 0x2 */
    SHORT W;                           /* 0x4 glyph width */
    SHORT H;                           /* 0x6 */
} WARN_POS;

/* CPU to VRAM copy of one glyph row (GP0 0xA0) drawn with DrawPrim */
typedef struct tagWARN_IMG {
    ULONG Tag;                         /* 0x00 */
    ULONG Code;                        /* 0x04 0xA0000000 */
    ULONG Xy;                          /* 0x08 (y << 16) | x */
    ULONG Wh;                          /* 0x0C (h << 16) | w */
    USHORT Pix[16];                    /* 0x10 */
} WARN_IMG;

/* flat-shaded quad drawn with DrawPrim (a POLY_F4 written as words) */
typedef struct tagWARN_POLY {
    ULONG Tag;                         /* 0x00 */
    ULONG Rgb_code;                    /* 0x04 */
    ULONG Xy[4];                       /* 0x08 (y << 16) | x */
} WARN_POLY;

/* overlay .data (still in asm) */
extern WARN_CD_CMD Cd_cmd_tbl[] __asm__("D_80185568");
extern volatile UCHAR *pCd_reg0 __asm__("D_801855A0"); /* 0x1F801800 index/status */
extern volatile UCHAR *pCd_reg1 __asm__("D_801855A4"); /* 0x1F801801 command / response */
extern volatile UCHAR *pCd_reg2 __asm__("D_801855A8"); /* 0x1F801802 parameter / irq enable */
extern volatile UCHAR *pCd_reg3 __asm__("D_801855AC"); /* 0x1F801803 request / irq flag */
extern UCHAR Cd_param[8] __asm__("D_801855B0");
extern UCHAR Cd_result[8] __asm__("D_801855B8");
extern LONG Warn_state __asm__("D_801855C0");
extern LONG Cd_irq_sum __asm__("D_801855C4");
extern WARN_LANG Warn_lang[2] __asm__("D_801855C8");
extern UCHAR Warn_circle[17][2] __asm__("D_801855E0"); /* quarter circle of the frame corners */
extern LONG Warn_vsync __asm__("D_80185610");
extern LONG Warn_last_track __asm__("D_80185614");
extern UCHAR Warn_min __asm__("D_80185618"); /* seek target (half of the last track's start): BCD minute */
extern UCHAR Warn_sec __asm__("D_80185619"); /* BCD second (see func_80184164) */
extern LONG Warn_lang_no __asm__("D_8018561C");
extern UCHAR Bios_region __asm__("D_BFC7FF52"); /* BIOS version string: 'I', 'A' or 'E' */

void func_801840F8(void);
LONG func_80184164(LONG Once);
LONG func_80184A9C(LONG Cmd);
void func_80184C70(LONG Cmd);
void func_80184D60(void);
void func_80184ECC(UCHAR *pStr, WARN_POS *pPos, ULONG Col);
void func_80185040(WARN_POS *pPos, UCHAR *pFont, ULONG Col, ULONG Bits);
ULONG func_801851F4(UCHAR *pFont);
void func_80185288(void);
void func_80185418(void);

#endif /* OVL_WARNING_H */
