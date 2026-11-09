#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

/* fixed-width names (decomp convention) */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/* Capcom names, as used in RE2's MAIN.SYM / ref/bio2-ds types.h */
typedef char CHAR;
typedef unsigned char UCHAR;
typedef short SHORT;
typedef unsigned short USHORT;
typedef long LONG;
typedef unsigned long ULONG;
typedef void VOID;

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Scratchpad (0x1F800000, 1 KiB) */
#define ScratchPad ((UCHAR *)0x1F800000)

#endif /* COMMON_H */
