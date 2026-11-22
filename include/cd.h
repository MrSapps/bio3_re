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
    ULONG Time_out;                    /* 0x03C poll counter (Cd_read) */
    ULONG *Read_adr;                   /* 0x040 */
    UCHAR Result[12];                  /* 0x044 */
    UCHAR pad050[0x054 - 0x050];
    UCHAR Work_buf[0x800];             /* 0x054 sector header / partial sector buffer */
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
    struct tagCD_ENTRY *pEntry;        /* 0x8A0 packed file directory (func_800138B0) */
    struct tagCD_ENTRY *pEntry2;       /* 0x8A4 */
    ULONG Unk8A8;                      /* 0x8A8 */
    LONG Unk8AC;                       /* 0x8AC bytes left to transfer to the SPU (func_80012ECC) */
    LONG Unk8B0;                       /* 0x8B0 bytes of the current entry left to read (func_80013EF8) */
    ULONG Unk8B4;                      /* 0x8B4 first word of the directory sector */
    ULONG Unk8B8;                      /* 0x8B8 */
    ULONG Unk8BC;                      /* 0x8BC */
    UCHAR Unk8C0;                      /* 0x8C0 */
    UCHAR Entry_no;                    /* 0x8C1 */
    UCHAR Unk8C2;                      /* 0x8C2 */
    UCHAR Unk8C3;                      /* 0x8C3 */
    UCHAR Em_id[4];                    /* 0x8C4 sorted enemy kinds in the room (func_800150E4) */
    ULONG Unk8C8;                      /* 0x8C8 flags */
    ULONG Unk8CC;                      /* 0x8CC */
    UCHAR Unk8D0;                      /* 0x8D0 copy of G.Ctp.Tpage_clut (Cd_read) */
    UCHAR Unk8D1;                      /* 0x8D1 copy of G.Unk2485 */
    UCHAR Fade_flg;                    /* 0x8D2 volume fade-out active */
    UCHAR Fade_step;                   /* 0x8D3 volume fade-out step */
    UCHAR Unk8D4;                      /* 0x8D4 enemy slot / texture step of the entry being loaded (func_800130A0) */
    UCHAR Unk8D5;                      /* 0x8D5 enemy slot being loaded (3 flag bits each in Unk8C8) */
    UCHAR pad8D6[0x8D8 - 0x8D6];
    struct tagCD_LINK *pLink;          /* 0x8D8 room link list being scanned (func_800139D8) */
} tagCD;

/* room sound link record (ROOM_WORK +0x08 list, 0xFFFF terminated) */
typedef struct tagCD_LINK {
    USHORT Id;                         /* 0x0 0xFFFF ends the list */
    UCHAR Room[2];                     /* 0x2 */
} CD_LINK;

/* packed file directory entry (8 bytes) */
typedef struct tagCD_ENTRY {
    ULONG Size;                        /* 0x0 */
    UCHAR Type;                        /* 0x4 */
    UCHAR Unk5;                        /* 0x5 */
    UCHAR Sum;                         /* 0x6 XOR checksum */
    UCHAR Id;                          /* 0x7 0x80: more entries follow */
} CD_ENTRY;

/* Type and Unk5 read as one USHORT */
#define CD_ENTRY_KIND(p) (*(USHORT *)&(p)->Type)

extern tagCD CD;                       /* 0x800B9E10 */

/* enemy model data record, one per enemy kind streamed in by func_800130A0 (8 bytes) */
typedef struct tagEM_DAT {
    struct tagENEMY_WORK *pEm;         /* 0x0 */
    UCHAR Id;                          /* 0x4 */
    UCHAR Kind;                        /* 0x5 */
    UCHAR pad6[2];
} EM_DAT;
extern EM_DAT D_800DECC0[];            /* enemy model data records, D_800E0170 used */
extern UCHAR D_800E0170;

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

extern USHORT D_800970DC[];
extern USHORT D_800970E8[];
extern UCHAR Cd_read_s_cdmode; /* 0x800971E1 RE2 static cdread_s_cdmode */
extern UCHAR Cd_read_cdmode; /* 0x800971E2 RE2 static cdread_cdmode */
extern LONG D_800A0FEC[]; /* SPU transfer address per bank type */
extern UCHAR D_800E06AC;
extern SHORT D_800E0DD8;

void Xa_play(ULONG mode, ULONG no);
ULONG Cd_read_s(ULONG File_no, ULONG Mode, ULONG *Address, ULONG Type, const char *Mess);
ULONG Cd_read(ULONG File_no, ULONG *Address, ULONG Mode, const char *Mess); /* RE2 Cd_read */
void func_80012ECC(tagCD *pCd);
void func_800130A0(tagCD *pCd);
void func_80013754(void);
void func_800138B0(void);
void func_800139D8(void);
void Cd_ready_cb(UCHAR intr, UCHAR *result);
void func_80013EF8(UCHAR intr, UCHAR *result);
void func_80014300(UCHAR intr, UCHAR *result);
void func_8001480C(tagCD *pCd);
void func_80014968(tagCD *pCd);
void func_80014D10(void);
void func_800150E4(void);

#endif /* CD_H */
