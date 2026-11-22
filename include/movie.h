#ifndef MOVIE_H
#define MOVIE_H
#include "bio3.h"
#include "main.h"

/* Movie table entry (RE2 struct tagMOVIE_TBL without Isrgb24/Work_top, 0x18 bytes) */
typedef struct tagMOVIE_TBL {
    ULONG Filecode;                    /* 0x00 index into floc[] */
    USHORT N_frame;                    /* 0x04 */
    USHORT Caption_id;                 /* 0x06 */
    USHORT Cancel;                     /* 0x08 pad buttons that skip the movie */
    USHORT Disp_w;                     /* 0x0A */
    USHORT Pos_x;                      /* 0x0C */
    USHORT Pos_y;                      /* 0x0E */
    USHORT Rl_size;                    /* 0x10 run-level buffer size (bytes) */
    USHORT Attribute;                  /* 0x12 */
    USHORT Volume;                     /* 0x14 percent of the BGM volume (/127) */
    USHORT Free;                       /* 0x16 start sector offset (x5) */
} MOVIE_TBL;
extern MOVIE_TBL Movie_tbl[];          /* 0x8009CA64 */

/* RE2 VM_FRAME */
typedef struct tagVM_FRAME {
    USHORT No;                         /* 0x00 current frame */
    USHORT Old_no;                     /* 0x02 previous frame */
    USHORT Pic_h;                      /* 0x04 */
    USHORT Dummy;                      /* 0x06 */
} VM_FRAME;

/* Movie playback work (RE2 struct tagVM_WORK with a work-memory pointer in front) */
typedef struct tagVM_WORK {
    UCHAR *pFree_work;                 /* 0x00 movie work memory allocation pointer */
    ULONG *Rl_buff[2];                 /* 0x04 */
    ULONG *pImage_buff;                /* 0x0C */
    USHORT End_frame;                  /* 0x10 */
    USHORT Movie_volume;               /* 0x12 */
    UCHAR Rno;                         /* 0x14 */
    UCHAR Cbuf_id;                     /* 0x15 */
    USHORT Movie_w;                    /* 0x16 */
    UCHAR Movie_sync;                  /* 0x18 */
    UCHAR Status;                      /* 0x19 */
    UCHAR Eom;                         /* 0x1A 0 playing, 1 ended, 2 cancelled */
    UCHAR Free0;                       /* 0x1B */
    USHORT Frame;                      /* 0x1C */
    USHORT Volume;                     /* 0x1E */
    ULONG Start_sector;                /* 0x20 */
    USHORT Movie_cancel;               /* 0x24 */
    USHORT pad26;
    ULONG Save_flg;                    /* 0x28 G.Unk2468 saved while the movie plays */
    UCHAR Movie_id;                    /* 0x2C RE2 G.Movie_id */
    UCHAR Padoff_stop;                 /* 0x2D RE2 Main.Padoff_stop_flg[1][1] */
    UCHAR Movie_Isrgb24;               /* 0x2E RE2 G.Movie_Isrgb24 */
} VM_WORK;
extern VM_WORK Vm_work;                /* 0x800DCD88 */

/* Movie display work (RE2 struct tagVM_DISP, same layout) */
typedef struct tagVM_DISP {
    RECT Rect;                         /* 0x00 MDEC strip upload rect */
    TILE Cinesco[2][2];                /* 0x08 */
    ULONG Cine_switch;                 /* 0x48 */
    USHORT Pos_x;                      /* 0x4C */
    USHORT Cur_pos_x;                  /* 0x4E */
    USHORT Pos_y;                      /* 0x50 */
    USHORT Width;                      /* 0x52 */
    USHORT Size;                       /* 0x54 */
    USHORT Count;                      /* 0x56 */
    USHORT Loop;                       /* 0x58 */
    USHORT Attribute;                  /* 0x5A */
} VM_DISP;
extern VM_DISP Vm_disp;                /* 0x800DCD20 */

extern ULONG StCdIntrFlag;             /* 0x800E15FC libds stream interrupt pending flag */

extern DslATV *pAtv;                   /* 0x800DCDC8 */
extern VM_FRAME *pVm_frm;              /* 0x800DCD80 */
extern DslFILE *pFile;                 /* 0x800DCDCC */
extern UCHAR *pMovie_result;           /* 0x800DCDBC */
extern UCHAR Ret_Value[8];             /* 0x800DCDC0 */
extern DslLOC Loc;                     /* 0x800DCDB8 */
extern DslFILTER Channel;              /* 0x800DCD18 */

void Close_movie(void);
void Movie_xa_start(void);
void Movie_xa_stop(void);
void Set_movie_volume2(ULONG Volume);
void RuntoImage(void);
void Mdec_callback(void);
void Movie_end(void);
ULONG Open_movie(ULONG Movie_id);
ULONG Play_movie(void);

LONG Init_movie_work(ULONG Id); /* start Movie_tbl[Id]; G.System_flg 0x10000 while it plays */

extern void (*pMovie_proc_tbl[])(void); /* RE2 pMovie_proc_tbl: Movie_init, Movie_main, Movie_exit */

void Movie(void);
void Movie_init(void);
void Movie_main(void);
void Movie_exit(void);
LONG CdReset2(LONG Mode);

#endif /* MOVIE_H */
