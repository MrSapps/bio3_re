#ifndef CD_H
#define CD_H
#include "bio3.h"

#define CD_RING_BUFFERS 16

/* CD / XA streaming work (RE2 struct tagCD; RE3 layout differs a lot) */
typedef struct tagCD {
    DslFILTER Filter;                  /* 0x000 */
    DslATV Cdvol;                      /* 0x004 */
    ULONG Vol;                         /* 0x008 */
    DslLOC Fpos;                       /* 0x00C */
    UCHAR Cd_rtn_no;                   /* 0x010 */
    UCHAR Xa_rtn_no;                   /* 0x011 */
    UCHAR Check_sum_flg;               /* 0x012 */
    UCHAR Type;                        /* 0x013 */
    ULONG Fsize;                       /* 0x014 */
    ULONG Fsize_now;                   /* 0x018 */
    ULONG Fsize_read;                  /* 0x01C */
    ULONG Fsector;                     /* 0x020 */
    USHORT Fsector_num;                /* 0x024 */
    USHORT Fsector_read;               /* 0x026 */
    USHORT Fno;                        /* 0x028 */
    UCHAR Fsum;                        /* 0x02A */
    UCHAR Sum;                         /* 0x02B */
    ULONG Sum_cnt;                     /* 0x02C */
    ULONG Start_sector;                /* 0x030 */
    ULONG Current_sector;              /* 0x034 */
    ULONG End_sector;                  /* 0x038 */
    UCHAR pad03C[0x044 - 0x03C];
    UCHAR Result[12];                  /* 0x044 */
    UCHAR pad050[0x854 - 0x050];
    UCHAR Use_mode;                    /* 0x854 */
    UCHAR Limit_break;                 /* 0x855 */
    UCHAR Status;                      /* 0x856 */
    UCHAR Time_ctr;                    /* 0x857 */
    ULONG *Buff_adr;                   /* 0x858 */
    UCHAR Read_ctr;                    /* 0x85C */
    UCHAR Write_ctr;                   /* 0x85D */
    UCHAR First_flg;                   /* 0x85E */
    UCHAR Unk85F;                      /* 0x85F */
    USHORT Ring_status[CD_RING_BUFFERS]; /* 0x860 */
    USHORT Ring_size[CD_RING_BUFFERS];   /* 0x880 */
    UCHAR pad8A0[0x8B8 - 0x8A0];
    ULONG Unk8B8;                      /* 0x8B8 */
    UCHAR pad8BC[0x8C2 - 0x8BC];
    UCHAR Unk8C2;                      /* 0x8C2 */
    UCHAR Unk8C3;                      /* 0x8C3 */
    UCHAR pad8C4[0x8D2 - 0x8C4];
    UCHAR Fade_flg;                    /* 0x8D2 volume fade-out active */
    UCHAR Fade_step;                   /* 0x8D3 volume fade-out step */
} tagCD;
extern tagCD CD;                       /* 0x800B9E10 */

extern UCHAR Cd_mode;                  /* 0x800971CB, RE2: static cdmode in Cd_init */

/* disc file location table entry (RE2 FLOC) */
typedef struct tagFLOC {
    ULONG size;                        /* 0x0 */
    USHORT sector;                     /* 0x4 */
    UCHAR sec_high;                    /* 0x6 */
    UCHAR sum;                         /* 0x7 */
} FLOC;
extern FLOC floc[];                    /* 0x800946A4 */

typedef void (*XA_CTRL_FN)(void);
extern UCHAR Xa_cdmode;                /* 0x800971CC, RE2: static xa_cdmode in Xa_play */
extern XA_CTRL_FN Xa_control_tbl[4];   /* 0x800971D0, RE2: static xa_control_tbl in Xa_control */
extern UCHAR Xa_control_cdmode;        /* 0x800971E0, RE2: static xacontrol_cdmode in Xa_control_play */
extern UCHAR Stereo_flg;               /* 0x800E1185, 0 = stereo (RE2: CD.Stereo_flg, inverted) */

void Cd_init(void);
void Cd_system_control(void);
void Xa_control(void);
void Set_volume(ULONG vol);
void Change_volume(ULONG mode);
void func_80074658(LONG on);
void Xa_error(void);
void Xa_control_stop(void);
void Xa_control_init(void);
void Xa_seek(UCHAR intr, UCHAR *result);
void Xa_ready_cb(UCHAR intr, UCHAR *result);
void Xa_control_play(void);
void Xa_control_end(void);
void Xa_fade_set(ULONG frames);
void Cd_seek(ULONG File_no, ULONG Mode);
void Cd_set_read_pos(ULONG size);

#endif /* CD_H */
