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
    UCHAR Last_flg;                    /* 0x85F reading the last enemy entry only (Cd_em_next) */
    USHORT Ring_status[CD_RING_BUFFERS]; /* 0x860 */
    USHORT Ring_size[CD_RING_BUFFERS];   /* 0x880 */
    struct tagCD_ENTRY *pEntry;        /* 0x8A0 pack directory (CD_DIR) */
    struct tagCD_ENTRY *pEntry2;       /* 0x8A4 directory entry being read */
    ULONG Trans_adr;                   /* 0x8A8 destination of the next ring sector (Cd_data_trans) */
    LONG Trans_size;                   /* 0x8AC bytes of the current entry left to copy / send to the SPU */
    LONG Entry_size;                   /* 0x8B0 bytes of the current entry left to read from the disc */
    ULONG Dir_head;                    /* 0x8B4 first word of the pack directory sector */
    ULONG Sector_ofs;                  /* 0x8B8 sector of the read position, from Fsector (Cd_set_read_pos) */
    ULONG Edd_size;                    /* 0x8BC size of the room EDD entry, added to Work_adr once copied */
    UCHAR Seek_req;                    /* 0x8C0 to Cd_read: 1 seek to Sector_ofs, 2 restart, 0xFF pack done */
    UCHAR Entry_no;                    /* 0x8C1 */
    UCHAR Bgm_main_id;                 /* 0x8C2 main BGM sequence of the room (Snd_work.Seq_data[0]) */
    UCHAR Bgm_sub_id;                  /* 0x8C3 sub BGM sequence of the room (Snd_work.Seq_data[1]) */
    UCHAR Em_id[4];                    /* 0x8C4 sorted enemy kinds in the room (Cd_em_id_set) */
    ULONG Load_flg;                    /* 0x8C8 bit 0 BGM entries begun, bit 1 BGM done, then 3 bits per Em_id slot */
    ULONG Work_adr;                    /* 0x8CC free enemy memory (G.pEm_mem while a room pack loads) */
    UCHAR Tpage;                       /* 0x8D0 next free texture page (from G.Ctp) */
    UCHAR Clut;                        /* 0x8D1 next free clut row (from G.Ctp) */
    UCHAR Fade_flg;                    /* 0x8D2 volume fade-out active */
    UCHAR Fade_step;                   /* 0x8D3 volume fade-out step */
    UCHAR Em_no;                       /* 0x8D4 enemy index / texture step of the entry being copied (Cd_data_trans) */
    UCHAR Em_slot;                     /* 0x8D5 Em_id slot being streamed (3 bits each in Load_flg) */
    UCHAR pad8D6[0x8D8 - 0x8D6];
    struct tagCD_LINK *pLink;          /* 0x8D8 room link list being scanned (Cd_link_search) */
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
    UCHAR Kind;                        /* 0x5 0/1 data copied to memory (1: enemy texture + model follow), 2 VAB body for the SPU */
    UCHAR Sum;                         /* 0x6 XOR checksum */
    UCHAR Id;                          /* 0x7 0x80: more entries follow */
} CD_ENTRY;

/* Type and Kind read as one USHORT */
#define CD_ENTRY_KIND(p) (*(USHORT *)&(p)->Type)

extern tagCD CD;                       /* 0x800B9E10 */

/* enemy model data record, one per enemy kind streamed in by Cd_data_trans (8 bytes) */
typedef struct tagEM_DAT {
    struct tagENEMY_WORK *pEm;         /* 0x0 */
    UCHAR Id;                          /* 0x4 */
    UCHAR Kind;                        /* 0x5 */
    UCHAR pad6[2];
} EM_DAT;
extern EM_DAT Em_dat[];            /* enemy model data records, Em_dat_num used */
extern UCHAR Em_dat_num;

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

extern USHORT Xa_sys_sector16[];
extern USHORT Xa_sys_sector8[];
extern UCHAR Cd_read_s_cdmode; /* 0x800971E1 RE2 static cdread_s_cdmode */
extern UCHAR Cd_read_cdmode; /* 0x800971E2 RE2 static cdread_cdmode */
extern LONG Snd_spu_adr[]; /* SPU transfer address per bank type */

void Xa_play(ULONG mode, ULONG no);
ULONG Cd_read_s(ULONG File_no, ULONG Mode, ULONG *Address, ULONG Type, const char *Mess);
ULONG Cd_read(ULONG File_no, ULONG *Address, ULONG Mode, const char *Mess); /* RE2 Cd_read */
void Cd_spu_trans(tagCD *pCd);
void Cd_data_trans(tagCD *pCd);
void Cd_bgm_entry_set(void);
void Cd_edd_search(void);
void Cd_link_search(void);
void Cd_ready_cb(UCHAR intr, UCHAR *result);
void Cd_bgm_ready_cb(UCHAR intr, UCHAR *result);
void Cd_room_ready_cb(UCHAR intr, UCHAR *result);
void Cd_bgm_next(tagCD *pCd);
void Cd_em_next(tagCD *pCd);
void Cd_em_share(void);
void Cd_em_id_set(void);

#endif /* CD_H */
