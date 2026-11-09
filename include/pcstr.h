#ifndef PCSTR_H
#define PCSTR_H
#include "bio3.h"
#include <libsn.h>

/* Movie streaming from the PC host (PCopen/PCread). No RE2 counterpart;
 * the per-buffer header is the 0x20 byte STR sector header (libcd StHEADER layout). */
typedef struct tagPCSTR_HEADER {
    USHORT Id;                         /* 0x00 */
    USHORT Type;                       /* 0x02 */
    USHORT Sec_count;                  /* 0x04 */
    USHORT Sec_num;                    /* 0x06 */
    ULONG Frame_count;                 /* 0x08 */
    ULONG Frame_size;                  /* 0x0C */
    USHORT Width;                      /* 0x10 */
    USHORT Height;                     /* 0x12 */
    UCHAR pad14[0x20 - 0x14];
} PCSTR_HEADER;                        /* size 0x20 */

typedef struct tagPCSTR_WORK {
    ULONG *pBuf[2];                    /* 0x00 */
    ULONG Start_frame;                 /* 0x08 */
    ULONG End_frame;                   /* 0x0C */
    void (*Start_func)(void);          /* 0x10 */
    void (*End_func)(void);            /* 0x14 */
    PCSTR_HEADER Head[2];              /* 0x18 */
    LONG Buf_no;                       /* 0x58 */
    LONG Fd;                           /* 0x5C */
    LONG Interleave;                   /* 0x60 */
    ULONG Sub_head[2];                 /* 0x64 */
    UCHAR pad6C[0x84 - 0x6C];
    LONG End_flg;                      /* 0x84 */
} PCSTR_WORK;
extern PCSTR_WORK Pcstr;               /* 0x800E12C0 */

void Pcstr_read_frame(ULONG *pBuf);

#endif /* PCSTR_H */
