#ifndef PCSTR_H
#define PCSTR_H
#include "bio3.h"
#include "em_sub.h"
#include <libsn.h>

/* model vertex/normal lists (Arg2 of func_8007A350) */
typedef struct tagSORT_MDL {
    SVECTOR *pVtx;                     /* 0x00 */
    SVECTOR *pNor;                     /* 0x04 */
} SORT_MDL;

/* model sort environment shared by the sort routines below */
typedef struct tagSORT_ENV {
    ENEMY_WORK *pEm;                   /* 0x00 G.pEm */
    SORT_MDL *pMdl;                    /* 0x04 */
    MATRIX *pWs;                       /* 0x08 &GsWSMATRIX */
    MATRIX *pM;                        /* 0x0C D_800C79F0 */
    VECTOR *pV;                        /* 0x10 D_800C79F4 */
} SORT_ENV;

/* screen xy of a vertex, as one word or two halves */
typedef union tagSXY {
    LONG l;
    struct {
        SHORT x;
        SHORT y;
    } s;
} SXY;

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
    void (*Start_func)(void); /* 0x10 */
    void (*End_func)(void); /* 0x14 */
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

extern MATRIX *D_800C79F0; /* current work matrices (func_80025A28) */
extern VECTOR *D_800C79F4;
extern UCHAR D_800CC848;
extern SHORT D_800D1F80;
extern ULONG *D_800E01C8;
extern ULONG D_800E1388[]; /* per-vertex colours of the model being sorted */
extern SVECTOR D_800E1568[6]; /* normals of the two bones being blended (func_8007B824) */
extern ULONG D_800E1598; /* its current offset word */
extern SORT_ENV D_800E15A0;
extern UCHAR *D_800E15C8; /* sort data of the part being drawn (func_8007A490) */

void Pcstr_set_buf(ULONG *pBuf, LONG size);
LONG Pcstr_open(char *pName);
void Pcstr_set(LONG unused, ULONG start, ULONG end, void (*start_func)(void), void (*end_func)(void));
void Pcstr_close(void);
LONG Pcstr_next(void);
LONG Pcstr_get(ULONG **ppBuf, PCSTR_HEADER **ppHead);
void func_8007A2D4(ENEMY_WORK *pEm, ULONG *pD);
void func_8007A350(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP);
void func_8007A490(void *pWork0, void *pWork1, ULONG Arg2, PARTS_W *pP);
void func_8007A600(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData);
void func_8007A918(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData);
void func_8007AC8C(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData);
void func_8007B2FC(ULONG *pRgb, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData);
void func_8007B518(ULONG *pRgb, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData);
void func_8007B824(ULONG *pRgb, PARTS_W *pP, SORT_ENV *pEnv, UCHAR *pData);
LONG func_8007BC7C(ULONG *Arg0, LONG Arg1, LONG Arg2, LONG Arg3); /* handwritten asm */
LONG func_8007BDF8(ULONG *Arg0, LONG Arg1, LONG Arg2, LONG Arg3); /* handwritten asm */
void func_8007BFBC(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData);
void func_8007C434(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData);
void func_8007C91C(SXY *pSxy, SHORT *pSz, SORT_ENV *pEnv, UCHAR *pData);

#endif /* PCSTR_H */
