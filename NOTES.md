# RE3 notes

## EXE
- SLUS_009.23 sha1 e0e88e8e3242152658b897ef618379d06cf21b72
- PS-X EXE, load 0x80010000, size 0xD3800, entry 0x80011B80, header gp=0 (crt0 sets it)

## Memory map (from crt0 @ 0x80011B80)
| range | what |
|---|---|
| 0x80010000-0x80094680 | .text (last `jr ra` at 0x8009466C) |
| 0x80094680-0x800B9DE8 | data: tables, strings, jump tables mixed — still one blob |
| 0x800B9DE8-0x800E34B0 | BSS zeroed by crt0, but stored in-file as zeros |
| 0x800E34B0-0x800E3800 | padding |
- crt0 sets `$gp = 0x80094680`; 0x80094688.. holds a small section table (entry, sizes, gp, ...)
- Stack top is read from 0x80098A68, heap stuff written to 0x80094684/88.

## Overlays
`CD_DATA/BIN/*.BIN` (17 files, 1-24 KB: TITLE, OPTION, MEM_CARD, SELECT, RESULT, DIEDEMO, ...) look like
headerless code overlays. Load addresses TBD (RE2 overlays load at 0x8011A000 / 0x80100000 — check RE3's loader).
Rooms (`.ARD`/`.RDT`-like in STAGEn) and enemy `.DO*`/`.PLW` files may contain code too.

## Compiler (confirmed)
**PsyQ 4.4: CC1PSX = GNU C 2.8.1 SN32 (BUILD 4.0.0010), ASPSX 2.77**, `-O2 -G0`.
Cd_init / Cd_system_control match on gcc-2.8.0/2.8.1 and are far off on 2.6.3/2.7.2 (2.7.2 schedules the
epilogue differently: `addiu sp` before `jr ra` instead of in its delay slot). PsyQ 4.3 ships 2.7.2.SN32,
4.4 ships 2.8.1 SN32, 4.6 ships 2.95.2. RE2 was built with PsyQ 4.0 / 2.7.2, so RE3 moved to a newer compiler.
Build: `cpp | tools/compilers/gcc-2.8.1-psx/cc1 -O2 -G0 | maspsx --aspsx-version=2.77 -G0 | GNU as`.

## Layout used by the splat config
- 0x80010000 SLD.S code (linked in the .rodata group, like RE2's SLD.OBJ)
- 0x8001031C-0x800110F0 game .rdata (jump tables, strings, consts), in the same order as the functions
  that use them, so it is cut per C file; 0x800110F0-0x80011B80 library .rdata
- 0x80011B80 crt0 (PsyQ NOHEAP module), 0x80011CF8 first game function (Cd_init, as RE2 CD.C)
- 0x80011CF8-0x8007CFC8 game code in src/game/*.c (split by address for now; rename to RE2 module names
  once the module boundaries are known)
- 0x8007CFC8-0x80094680 PsyQ 4.4 libs (libspu, libgte, libgpu, libcd/libds, libapi, libc, libcard, ...),
  names from tools/psyq/libsyms.py (signature match against the SDK .LIB files)

## Compiler / SDK clues (initial notes)
Confirmed from the binary:
- Sony libs: `$Id: sys.c,v 1.140 1998/01/12` (libgpu), `bios.c 1.86 1997/03/28`, `intr.c 1.75 1997/02/07`,
  "Library Programs (c) 1993-1997". So it's a PsyQ SDK from 1998 or later; RE2 (1998) is listed as PsyQ 4.0.
- **`-G 0`**: only 1 gp-relative access in all of .text (RE2 main reportedly used `-G 16`, overlays `-G 0`).
- Divides are bare `div $zero,a,b` + `mflo`, without the `break 7`/`break 6` div-by-zero checks (only 6 checked
  divs in the whole EXE, probably Sony lib code). Assembler inserts hazard `nop`s between back-to-back div/mflo.
- `$fp` is used as a general callee-saved reg (s8) → frame pointer omitted (consistent with -O2).

Hypothesis (needs testing): PsyQ 4.x `CC1PSX` (GCC 2.7.2-SN or 2.8.1-SN) with `-O2 -G 0`.
How to confirm: pick a few small leaf + non-leaf game funcs and try them on decomp.me
against psyq4.0 / 4.1 / 4.3 / 4.4 / 4.5 / 4.6 with `-O2 -G0`. The unchecked-div behaviour and the
prologue/epilogue register save order should separate GCC 2.7.2 from 2.8.1/2.95.

## Reference
- RE2 Dual Shock (SLUS_007.48) decomp with real symbols from the Oct-1997 beta MAIN.SYM:
  https://github.com/MeganGrass/bio2-ds (cloned at ../ref/bio2-ds). Same engine lineage, so struct and
  function names should carry over a lot. Its Makefile: PsyQ 4.0, GNU C 2.7.2.SN32.3.7, `-O2 -G 16 -mgpopt -membedded-data`.

## Module map (from the first matching pass; starts are approximate, RE2 order is kept)
| RE3 start | RE2 module | notes |
|---|---|---|
| 80011CF8 | CD.C | Cd_init .. Set_volume/Change_volume, plus RE3-only Cd_seek / fade |
| 80015428 | DOOR.C | Door_main; 80016600-80017D9C RE3 door-model anim (DOOR_EX?) |
| 80017DA4 | EM_SUB.C | Hani_ck .. Kage_work*; 8001B000-8001F000 EM_SUB2/EM_SET/ESP-like pools |
| 80020298 | ESP_R.C | effect routines take the work ptr as an argument in RE3 |
| 80022990 | TRANS.C | |
| 80023268 | GAME.C | |
| 800251FC | JOINT.C | |
| 80028B1C | LINE_SCR.C | |
| 80028F38 | MAIN.C | main, Swap_Cbuff, Init_system, Fade_*, Cut_*, Card_event_set |
| 8002AB34 | SUB.C-like model helpers | Mapping_tmd counterpart |
| 8002F2F8 | MOJIDISP.C | |
| 80031DA8 | MONITOR.C | task scheduler, ~1:1 with RE2 |
| 800321C4 | MOVIE.C | |
| 80033508 | DOOR_EX / OBA? | unchecked |
| 80035790 | OM.C, then OMA | |
| 80037FA0 | PAD.C | |
| 80038AAC | PLAYER.C | Pl_routine tbl 8009CD40 |
| 8003DEA4 | PL_WEP.C / PL_WEP2.C | pl_mv05_w_tbl 8009CE88, pl_mv05_t2_spr 8009CED8 |
| 80048520 | POLY_SUB / PSP / ROOM / ROOT_CK | ~ |
| 8004A8BC | SABUN.C | |
| 8004B7E8 | SCA.C | |
| 800505AC | SCE_AT.C | |
| 80051F78 | SCE0.C | |
| 80052DE8 | SCE1.C | 1:1 with RE2, SCE_TASK 0x174 |
| 80055A18 | SCE2.C | |
| 80058A00 | SND.C (RE2) | |
| 800601F0 | ? enemy/object routines | |
| 800636D4 | STATUS / ST_SUB? | unconfirmed |
| 80073E84 | RE3 sound driver | custom libspu sequencer, Snd_work 800E0610 |
| 800784E0 | SUB.C | |
| 80079EC0 | PC-host STR streaming | PCopen/PCread |
| 8007C434 | handwritten asm | |

Known splat issue: 80053FE0-800541E4 are `jlabel` case bodies of func_80053FAC's jump table, not functions.
Matching rules learnt: functions must stay in address order in the .c (a misplaced INCLUDE_ASM shifted SCA by 8 bytes);
`(UCHAR)pEm->Type` gives the `lbu` RE3 uses for the weapon/type byte; `G.Sce_work` must be indexed via pointer arithmetic.
