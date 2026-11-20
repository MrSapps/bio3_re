#ifndef DOOR_H
#define DOOR_H
/* RE2 DOOR.C: door opening sequence (Door_main / Door_init / Door_move / ...) */
#include "bio3.h"

typedef void (*DOOR_PROC)(void);
extern DOOR_PROC Door_proc_tbl[];      /* 0x800979F0, NULL terminated (RE2 pDoor_proc_tbl) */

/* door texture/sound info (12 bytes, RE2 DOOR_INFO) */
typedef struct {
    USHORT Sound_size;                 /* 0x0 */
    USHORT Data_size;                  /* 0x2 */
    ULONG Sector_offset;               /* 0x4 */
    UCHAR Sound_sum;                   /* 0x8 */
    UCHAR Data_sum;                    /* 0x9 */
    UCHAR padA[2];
} DOOR_INFO;
extern DOOR_INFO D_800971E4[];         /* RE2 Dinfo[], index Dtex_type */
extern USHORT *D_800979D4[];           /* door file no per stage, index Dtex_type */

/* door data file header, loaded at 0x8019D000 (RE2 Door_data->hdr; offsets relative to the file) */
typedef struct {
    ULONG *pTmd;                       /* 0x0 */
    ULONG *pTim;                       /* 0x4 */
    UCHAR Scd[4];                      /* 0x8 door event script */
} DOOR_DATA;
#define Door_data ((DOOR_DATA *)0x8019D000)

void Door_main(void);

#endif /* DOOR_H */
