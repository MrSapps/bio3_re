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
Load bases come from the exe table D_8001031C (0x80194000, 0x80184000, 0x8011A000, 0x801C2000, ...).
- `CD_DATA/BIN/*.BIN` (17 files): `[file-number word][.rdata][.text][.data]`. Base 0x801C2000, except
  TITLE/DIEDEMO/ENDING 0x80194000 and WARNING 0x80184000 (WARNING has no file-number word).
- Room archives `STAGEn/Rxxx.BIN`: word 0 total size, word 1 entry count, then (size, info) pairs; entries
  start at 0x800, sector aligned. info>>24 = enemy id, info&0xFFFF = type; types 1/2 are the enemy code
  module linked for load slot 1 (0x80100000) / slot 2 (0x8010D000). Same layout as BIN overlays (word 0 =
  file number, differs per slot). 162 rooms hold only 21 distinct modules (ids sharing code have identical
  modules) = ~1080 functions. `tools/extract_em.py` writes iso/EM/emXX_{1,2}.bin + modules.txt.
- Slot 2 is slot 1 relinked at +0xD000 (verified: every differing word is a relocation), so only slot 1 is
  split; the Makefile relinks the same objects with a sed'ed ld script and the slot-2 header word
  (ovl/emXX_2.header.s). Module BSS past the file end is referenced as absolute D_ symbols by splat:
  `tools/mkovl.py --bss emXX` rewrites them module-relative (ovl/emXX.bss.ld). em2c slot 2 differs in one
  .rdata padding byte (linker filler): ovl/em2c_2.fixup, applied by tools/fixup.py.
- Workflow: `tools/mkovl.py NAME TEXT DATA` writes ovl/<name>.yaml; `make split-ovl`; C in
  src/enemy/<name>.c (enemy overlays em*) or src/ovl/<name>.c (the others; warning also has warning_frame.c and warning_task.c); check with `tools/ovlcheck.sh <name>` (parallel-safe) or `make check-ovl`.
  fdiff.py picks the overlay's own asm/syms from the C path. The overlay link uses the generated
  undefined_*_auto files plus build/syms.ld (exe names from syms/*.txt), so new names in
  ovl/<name>.syms.txt take effect after a re-split (`.venv/bin/splat split ovl/<name>.yaml`, for em* then
  `tools/mkovl.py --bss <name>`; run it again after renaming the C, splat sorts INCLUDE_ASM files into
  matchings/nonmatchings by the names it finds in the C).
- WARNING.BIN is three objects (ovl/warning.yaml edited by hand, do not regenerate it with mkovl.py):
  warning.c 0xF8 (mod-chip check), frame.c 0x1288, task.c 0x1418 (Capcom task, normal toolchain). The
  first two are not Capcom code: built with gcc 2.7.2 + plain GNU as (Makefile WARNING_GAS_OBJS, fdiff.py
  OLD_AS_OBJS): macro `lbu $v0,sym($a2)` uses the destination as temp (`$at` only when dest == base),
  delay slots of `j` take the tail of a macro store, `move` = addu, and sections are padded to 16
  bytes (B's 2 nops / 8 zero rodata bytes). Leaf returns are `j end` (2.7.x; 2.8 turns them into
  `jr $ra`). No cc1 we have reproduces it as is. The PsyQ/SN 2.7.2 cc1s (also 2.6.3, PsyQ 4.3
  cc1psx.exe) give every top-tested loop with a memory bound a stray 8-byte frame and a `blez` first test.
  decompals' gcc-2.7.2-cdk (cygnus-2.7.2-970404) gets the loops right but has two later cygnus changes
  that the original lacks. reload_cse_simplify_set turns a second constant into a register copy
  (`move $a2,$a1` for `f(p, 0, 0)`), and assign_parms puts REG_EQUIV notes on register parameters,
  which doubles their live length and changes global-alloc priorities. The build therefore patches
  both out of a copy (Makefile WARNING_CC1 = tools/compilers/gcc-2.7.2-cdk-warning/cc1, made from
  the sha1-checked gcc-2.7.2-cdk; fdiff builds it on demand) and adds `-fpic`, because the cdk cc1
  otherwise splits `lw $3,sym` into lui/lw. With that, all of warning.c and frame.c match. Source
  quirks: the loop in func_80184164 is `goto`-based (a `do`/`while` lets loop.c hoist the case
  constants into s-regs), and D_80185618/D_80185619 are separate UCHARs (as an array their accesses
  share a base register). The second symbol is in ovl/warning.syms.txt and was added by hand to the
  ignored asm/data/ovl/warning/data.data.s, so a re-split gives the same file.
  Search for an unpatched compiler (kept the patch, user decision): decompals cdk 0.14-0.17 and PsyQ 4.1
  CC1PSX.EXE (same cygnus-2.7.2-970404) give the same 5 diffs unpatched; FSF 2.7.0-2.7.2.3, PsyQ 4.0
  2.7.2.SN32 and all 2.5.7/2.6.x builds have the loop problem; 2.8.x turns `j end` into `jr $ra`. The
  original is a Cygnus/SN snapshot between PsyQ 4.0 and 4.1 that is not available. Untested: DOS-only
  PsyQ 3.x and the Net Yaroze DJGPP gcc (need DOSBox), or building cygnus-2.7.2-961023 from source.
- Overlays see G with fields past tagGLOBAL's end; an overlay-local view `extern T Gr __asm__("G")` works,
  but fields used together must come from the same symbol for gcc to share the &G base.

## Process notes
- Cleanup pass (clean/gameA: em_sub, joint, player, pl_wep, spl_*, om, door, esp_r, psp, oba): names
  go to symbol_addrs.txt (so the main asm uses them after a re-split: rm asm/{nonmatchings,matchings,data}/game,
  asm/data/*.s, asm/*.s, then `make split`) and to syms/*.txt (PROVIDE for the overlay links). Renaming an
  INCLUDE_ASM function also renames its INCLUDE_ASM line. ENEMY_WORK/PARTS_W field renames were fixed through
  the compiler (cpp with line markers + cc1 -fsyntax-only), so same-named fields of other structs stay.
- bio3.h vector views replace the casts: CDIR_SV, POS_V, EM_SPD_SV, EM_WORKM, PW_M, WORKM_POS_V (gcc 2.8 has
  no anonymous unions, so a named SVECTOR member would mean renaming every Cdir_x use). Free-work views:
  PW_NECK, PW_RGB, PW_BOMB (joint.h). The game files use enemy/em_common.h's EM_SET_RTN / EM_SET_MOVE for the
  Routine / Move word stores (moving those macros to bio3.h would be cleaner).
- Aliases removed by fixing the canonical prototype: Cdir_ck/Cdir_ck_pos/Cdir_ck2/Dir_ck return LONG,
  Parts_hokan takes LONG Rate (narrowed inside), Parts_sort_simple and Spl_sce07 take the arguments their
  callers pass, Rnd_tbl is read with (signed char) casts. Kept on purpose (comment says why): Add_speedXZ3
  (3 args), Joint_trans_om / Sca_get_ground3 (om.c), the Rnd return-type views, spl_auto's unprototyped calls.
- Never run a full `make` while agents edit src/: it rebuilds build/ objects from half-edited files and
  breaks every other agent's linkcheck (which links against build/). Restore from a clean worktree build.
- fdiff.py used to leak one /tmp dir per run (36k dirs, 3 GB, filled the disk); fixed.
- Naming pass (clean/names, 2026-11-25): src/ has no func_/D_ address names left except D_800BA6F4,
  D_800C79C0, D_800CA720, D_800CC84D (G+0x2115, used both as the hint page and the RESULT routine) and D_800DECAC
  (unclear role). status.c sprite tables are named after what they draw (Item_frame_spr, Cmd_*_ft4, Box_*,
  Btn_file/map/exit_ft4 = status menu buttons for Cursor -2/-1/<-2). sca shapes (index Id & 0xF, shared by
  Sca_rtn_tbl / Sca_high_tbl / Sca_line_tbl): 0 circle, 1 box, 2 koban_x, 3 koban_z, 4 hishi, 5 box with one
  diagonal line test, 6 naname (corner in Id >> 4), 7/8 box, 9 slope, 10 stairs, 11 none, 12 cone; RE3's order
  differs from RE2. D_800E01xx views are Sce fields (Sce_cut, Sce_cut_old, ...), not St. EM_RTN01 lives in
  em_common.h (USHORT); em22/em23 need EM_RTN01_S (lh). The Em*_mvNN / pl_mvNN / PlwNN_init names are Capcom's
  own index convention (RE2 EM37.C has Em37_mv00..0e), so they were kept; Plw_init_<addr> became PlwNN_init.
  Rename list for replaying on other branches: renamesN.txt in that session's scratchpad.

## Matching tools (tools/permute)
- `tools/permute/setup.py SRC.c FUNC DIR` makes a decomp-permuter dir (target.o from the split asm, base.c = SRC.c
  preprocessed with every other function body dropped). `tools/permute/score.py DIR [variant.c ...] [-v]` compiles
  base.c with the function replaced by variant.c and counts differing words (relocated fields masked) in ~0.1 s, so
  type/order sweeps of hundreds of variants are cheap. Confirm a 0 with fdiff.py (it also checks relocation targets).
  Permuter: `cd ~/dev/decomp-permuter && tools/capped ~/dev/permvenv/bin/python permuter.py DIR -j3`.
- ~/dev/gcc281/build-psx/cc1 is GCC 2.8.1 built from source with -g, usable under gdb (cse.c/global.c breakpoints).
  It is NOT identical to SN32: SN's cc1 splits `la` into separate %hi/%lo pseudos, the FSF build emits one `la`, so
  for anything involving global addresses dump with SN's own cc1 instead (`tools/compilers/gcc-2.8.1-psx/cc1 -dS -dl
  -dg ...` works). Several near misses come down to sched1 choosing which register a %hi gets.
- cse.c make_regs_eqv: after `B = A` (both pseudos), B becomes the canonical register if it lives past the basic
  block and longer than A, so later uses of A are rewritten to B and local-alloc ties both to one hard reg (the copy
  vanishes). A surviving `move vX,aN` + in-place `sll/sra aN` means the copy was a different value to cse (e.g. a
  16-bit local copying the param) -- Goto00/Goto01 in em_sub show this pattern (still unmatched).
- score.py candidates see the preprocessed base.c: header macros (NULL, Door_data, ...) are gone, so expand them.
- Source tricks found while matching: `(&G.Ctp)->field = x` gives a struct-flagged store off a G+offset base that
  the scheduler keeps in order (`*(USHORT *)&G.x` is scheduled freely); writing a value in two statements
  (`x = a + C; x &= M;`) stops cse merging and-masks; constants rematerialised every iteration point to a goto loop;
  one reused temp for several values (psp func_80048E34) is how several unrelated values share one register.
- Near misses with drafts (2026-11-24): oba func_80034E78 (3 words), Jill_wep_load/select func_801C3AE4 (5),
  oba func_80035678 (~9), om func_80037E80 (~10), Door_init (12, needs DOOR Divp fields at 0xC/0x10/0x14),
  em27 Em27_Init (12), em22 Em22_dir_quarter (14), select func_801C3830 (19, s4/s5 priority 0.600 vs 0.615).
  Get_light_cut (4), snd func_80077F60 (5, 2 with an empty do/while after the decls), Pc_text_set (7),
  Flr_set (2), em36 Em36_mv12 (10), Plc_gun_eff (11), Get_matrix (17). Drafts in that session's scratchpad.
  em27 func_801000E0 (12), em22 func_80103110 (14), select func_801C3830 (19, s4/s5 priority 0.600 vs 0.615).
  Get_light_cut (4), snd Snd_bgm_play_ck (5, 2 with an empty do/while after the decls), Pc_text_set (7),
  Flr_set (2), em36 func_8010511C (10), Plc_gun_eff (11), Get_matrix (17). Drafts in that session's scratchpad.
  option func_801C4C94 matches but shares the "\x81\x40" string with func_801C4A70 (must go in together).

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
- 0x80011CF8-0x8007CFC8 game code in src/game/*.c, one file per original object (see Module map)
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

## Header layout
One definition per type: a struct used by more than one file lives in the header of the module that
owns the data (RE2's header split); a .c keeps a private type only when no other file uses that layout.
- bio3.h: tagGLOBAL (G) and the structs G embeds: KAGE_WORK/KAGE_WORK9, RCUT, OBJ_MODEL_WORK
  (G.Ob_model[32]), ITEM_WORK/ITEM_SET (G.Item_work[2]), EM_BIN_INFO (G.Em_bin_info[2]), PARTS_W,
  COL_OBJ, PLAYER_WORK and ENEMY_WORK. The player is `PLAYER_WORK Player` at G+0x248C (RE2 G.Player):
  the first 0x1CC bytes of an ENEMY_WORK, both structs declared from one member list (EM_WORK_HEAD) and
  checked by STATIC_ASSERT (common.h). Code passes the player as an ENEMY_WORK: G_PLAYER /
  PLAYER_EM(pG) (= RE2's (ENEMY_WORK *)&G.Player), or the exe symbol Pl_work (player.h, typed
  ENEMY_WORK; it is &G.Player under its own name, which gives gcc a different base register, so
  both forms stay). G.Pl_poison_timer follows the player; G.Sub_work is the spare ENEMY_WORK.
  ENEMY_WORK.Col (0x80) is the OBA.C collision node COL_OBJ; Unk1CC[] (enemies only) is each
  kind's free area (spl.h SPL_FREE / SPL_W for the sub player, overlays EMxx_W).
  PARTS_W.pTmd, ENEMY_WORK.pUnk174 and Pl_wep_tmd are MD_OBJ * (md_sub.h; 0x18-byte objects);
  ENEMY_WORK.pTmd stays ULONG * (the MD file: n_obj word, then the objects).
  ENEMY_WORK.Kage is a union (pKw: enemies, Kage_work_set; pKw9: player, Kage_work9_set) = RE2 pKage_work.
  ENEMY_WORK 0x180..0x19C are 8 kan/seq pointers (pUnk180_seq_t_ptr .. pUnk19C_kan_t_ptr).
  G.pEnemy[42] is RE2's pEnemy ([0] player, [1] sub player, [2..] enemies); G.Ctp is the Ctpage/Cclut
  byte pair, also written as one halfword (Tpage_clut).
- main.h: VCUT, ROOM_WORK, FADE, DBUF, tagMAIN, tagGAME, SAVE_POS (Game.pSave). game.h: LIVE_SEL.
- em_sub.h: KAN_HEAD, TBL12. joint.h: JOINT_P and the PARTS_W free-work layouts at +0x74
  (PARTS_NECK_FREE_WORK / PW_NECK, PARTS_RGB_FREE_WORK / PW_RGB).
  include/enemy/parts_free.h: PARTS_FLY_FREE_WORK / PW_FLY, the fly-off state joint.c func_8002783C moves
  (UnkA2 bit 0x20). Overlay-specific free-work views stay in the overlay header (EM30_PW, EM34_ARM, EM36_PF).
  em_common.h: EM_SET_MOVE_V for run-time motion numbers (Capcom added them to the constant: addu, not or).
- door.h: DOOR_WORK (pDwork[] slots, D_800BA700) and DOOR (RE2 tagDOOR, pDoor[0] = D_800BA6F8).
- esp.h: the effect pool (ESP_WORK D_800BA8A8[96], RE2 Esp[]; RE3 layout), SSEQUENCE, SPR_GP; ESP_NONE is
  the (ESP_WORK *)-1 the effect starters return.
- md_sub.h: MD_OBJ/MD_HEAD (RE3 model files) and the sorter works MD_WORK, MD_WORK2, RGB_BLEND_WORK,
  SORT_PARAM. om.h: CC_PARTS_WORK, CC_WORK (OBJ_MODEL_WORK itself is in bio3.h).
- pl_wep.h: HIT_REQ (func_800472EC/func_80047BD0), HIT_AREA (func_800445C8, every overlay uses it),
  EM_HIT_AREA, OM_TARGET, PL_WEP_*.
- cd.h: tagCD, CD_LINK, CD_ENTRY, FLOC, EM_DAT (Em_dat, filled by the room streamer).
- sce0.h: SCE_TASK, MODEL_LINK_INFO, SCENARIO (Sce). The SCE opcode records stay private in sce1.c.
- status.h: ST_WORK (St, 0x800E01C0), ST_POS; the other ST_* tables stay private in status.c.
- snd.h: SND_* incl. SND_VOL embedded at SND_WORK.Vol (0x7C0) and the listener Cam_x/y/z, Cattr.
- sub.h: LIGHT_CUT_DATA, LIGHT_ZONE, PL_SAVE (Retry_data). sca.h: SCA_HEAD, SCA_DATA, SCA_WORK.
  oba.h: COL_ENT, COL_WORK. Overlay-only types live in include/enemy/<name>.h (enemies) or include/ovl/<name>.h.
Views of G that remain on purpose (codegen needs a second declaration of the symbol, so gcc does
not share the &G base): result.h GLOBAL_RESULT `Gr`. em33.h EM33_PL reaches G.pEnemy[] from
pPl = &Pl_work (one base register).
Prototypes: every function defined in src/game/<m>.c is declared once, in include/<m>.h (RE2 names:
esp_r.h, psp.h, room.h, root_ck.h, sabun.h, sce1.h, sce2.h, sce_at.h, spl.h, spl_auto.h, spl_sce.h;
status.c (was 800636D4.c) -> status.h, 80079EC0.c -> pcstr.h, 80048520.c -> 80048520.h, em_sub.c's ESP part (from
0x8001B08C) -> esp.h, pl_wep2.h folded into pl_wep.h). SLD.S functions are in sld.h. Overlay functions and
data are in include/enemy/<n>.h or include/ovl/<n>.h (their names come from ovl/<n>.syms.txt via the split, no naming aliases). Data externs
are in the owning module's header. PsyQ functions come from the SDK (psyq.h also pulls libpad.h and
libpress.h); InitGeom/PadStartCom/InitCARD/StartCARD/DecDCToutSync were identified and added to syms/game.txt.
A .c keeps a local `T f_(..) __asm__("f")` alias only where its codegen needs a different prototype
(comment says what differs). Animation tables are passed as `ULONG *pKan, ULONG *pSeq` everywhere (the type of
the ENEMY_WORK *_kan_t_ptr / *_seq_t_ptr fields, RE2 ULONG*); functions that walk them byte-wise cast to
UCHAR* / KAN_HEAD* inside. Joint_move/Joint_move2 return the J_seq result (ULONG). The tree compiles with
no warnings (cc1 without -Wall); keep it that way. snd.c calls Task_sleep with no declaration on purpose: any declaration,
even `int Task_sleep();`, changes the loop in Snd_bgm_fade_wait, so snd.c must not include monitor.h.

## Module map (src/game, one file per original object)
Files are split where the original objects begin, so each file's .rodata starts where the original
object's .rdata did; that is what jump-table alignment needs (see "Fourth pass"). Names are RE2 module
names where RE2 function names anchor the range; address-named files are objects whose RE2 name is unknown.
Boundaries marked ~ are only pinned to a range (any function in the range gives the same bytes).
| start | file | evidence / notes |
|---|---|---|
| 80011CF8 | cd.c | Cd_init .. Set_volume/Change_volume, RE3-only Cd_seek/Xa_fade_set |
| 80015428 | door.c | Door_main; tail holds ENEMY_WORK code incl. Em_move_tbl_set (RE2 EM_SET.C), not split yet |
| 80017DA4 | em_sub.c | Hani_ck .. Kage_work*, Rot_neck*; ESP.C part from ~8001B08C (Esp_init_C/R, Esp_data_set0/1), not split: the jtbl at 800103F8 rules out an object start at 8001B08C, so the real boundary needs more work |
| 80020298 | esp_r.c | effect routines |
| 80022990 | trans.c | Trans_work_init, Trans_pointer_set (RE3 links TRANS before GAME) |
| 80023268 | game.c | Gun_light_set/reset |
| 800251FC | joint.c | PartsWork_set |
| 80028B1C | line_scr.c | line_trans .. Line_work_init |
| 80028F38 | main.c | main, Swap_Cbuff, Fade_*, Cut_*, Card_event_set |
| 8002AB34 | md_sub.c | Md_adr_cnv, Md_set_abr; .rdata of MOJIDISP starts 800104E0 (8-aligned) |
| 8002F2F8 | mojidisp.c | Print8/Print14, Moji_*; was split across two address files |
| 80031DA8 | monitor.c | task scheduler, ~1:1 with RE2 |
| 800321C4 | movie.c | Close_movie, RuntoImage, Mdec_callback |
| 80033508 | oba.c | OBA.C, rewritten in RE3 (collision object list); no RE2 Oba_* name maps 1:1 |
| 80035790 | om.c | Tex_chg*, Tex_ctr; probably OM + OMA; .rdata starts 80010704 (4 mod 8) |
| 80037FA0 | pad.c | Pad_vib_* |
| 80038AAC | player.c | Pl_move, pl_mvXX_pad, Pl_damage..Pl_life_up; .rdata 80010748 |
| 8003DEA4 | pl_wep.c | PL_WEP + PL_WEP2 names mixed (RE3 has one object or two adjacent; not separated) |
| 80048520 | 80048520.c | one RE3-only timer display function (Print14_col) |
| 80048630 | psp.c | Psp_set, Psp_trans; 80048974-80049218 RE3-only effect code |
| 800493EC | room.c | Set_room, Set_stage, Em_init_move |
| 80049BA8 | root_ck.c | Root_ck, Rnd_area, Guide_check, kara_rootck2 |
| 8004A8BC | sabun.c | |
| 8004B7E8 | sca.c | Sca_ck_circle*, Sca_high_* |
| 800505AC | sce_at.c | |
| 80051F78 | sce0.c | Sce_aot_init, Event_init/exec |
| 80052DE8 | sce1.c | 1:1 with RE2, SCE_TASK 0x174 |
| 80055A18 | sce2.c | SCE_TASK opcode handlers up to 80058CD0 |
| 80058DD4 | spl.c | SPL.C: Spl, Spl_Init, Spl_set; routine table Spl_routine_0 = RE2 Spl_routine_0 slots |
| ~800597F0 | spl_auto.c | SPL_AUTO.C: Spl_Auto, Aida_*; RE3 moved Spl_Damage/Die/Dead here. Start pinned to 800594D8..8005CB2C (.rdata 80010D0C) |
| ~800601F0 | spl_sce.c | SPL_SCE.C (reworked): Spl_Scenario, Spl_sce00-09. Start pinned to 8005F29C..80061DF4 (.rdata 80010D90) |
| 800636D4 | status.c | RE3's own status screen (barely RE2 STATUS/ST_SUB*), one object (.rdata 80010DCC). Modules in order: file viewer St_file_* (800636D4), item box St_box_* (800643E4), inventory/command menu St_item_*/St_cmd_* and combine St_mix_* (80066530), icon VRAM Pix_* (8006A9AC), screen setup/draw St_prim_init..St_file_icon_disp (8006AE90), item/ammo helpers Item_*/Tama_* (8006CCF0), task St_init/Status/St_exit and prim helpers (8006D948), map St_map_*/Map_* (8006ED28). Was 800636D4.c |
| ~80073E84 | snd.c | RE3 sound driver (custom libspu sequencer, Snd_work 800E0610) |
| ~800784E0 | sub.c | SetSpadStack, Vram_clr, Rot_*, Flg_*, Set_view, Set_light* |
| 80079EC0 | 80079EC0.c | PC-host STR streaming (Pcstr_*) and handwritten asm from 8007C434 |

Matching rules learnt: functions must stay in address order in the .c (a misplaced INCLUDE_ASM shifted SCA by 8 bytes);
`(UCHAR)pEm->Type` gives the `lbu` RE3 uses for the weapon/type byte; `G.Sce_work` must be indexed via pointer arithmetic.

### More matching rules (second pass, 6 files)
- `char` is unsigned here (-funsigned-char): use `signed char` for `sll 24/sra 24`.
- Oversized frame: an unused local (`SVECTOR Unused;`, `LONG Dummy[4]`) in the original.
- `__builtin_abs(x)` or `x < 0 ? -x : x` gives bgez/move/negu; `if (x<0) x=-x;` does not.
- `n = x & M; n = n != 0;` gives andi+sltu; `(x & M) != 0` inline gives srl/andi.
- Target keeps `&G`/`&Main`/`&Snd_work` in an s-register: use `tagGLOBAL *pG = &G;` etc.
- No hoisting/loop rotation in target: write the loop with goto or do/while.
- Use real struct fields, not `*(T*)((UCHAR*)p+off)`: casts change scheduling and base registers.
- Switch decision tree with an extra compare: an empty `case N: break;` existed.
- Non-void function with no return: v0 is not used to fill the final delay slot.
- Void callee whose result the original uses as int: call through an int-returning prototype.
- Different prototype for one caller: `T name(...) __asm__("func_X");` alias. Never cast-call a
  function defined in the same file (gcc 2.8 drops the call).
- `a = b = x` stores b first; store/assignment order is a cheap permutation to try.
- fdiff/make use pipefail: any cc1 error fails the build (conflicting prototypes were silently
  tolerated before).
- syms/*.txt names are linked via build/syms.ld as PROVIDE(), so C definitions take precedence.

### Third pass: rodata in C, alignment, more rules
- Jump tables, strings and local const aggregates are written in C; cc1 emits them into .rodata in
  function order. Verify with `tools/linkcheck.py FILE.c` (fdiff/checkall show false %hi/%lo diffs for them).
- Last .rodata item in a file may need `__asm__(".section .rodata\n.align 2\n.section .text");` after it.
- **Jump tables are 8-aligned relative to their original object's .rdata start**, and objects are only
  4-aligned in the final link. Tables at the "wrong" mod-8 address for our address-split C files mean a
  missing file boundary (e.g. 800303CC.c: PLAYER.C starts at 80038AAC; 8005003C.c: boundary in
  800594D4-8005CB2C). Rewriting all `.align 3` to `.align 2` does NOT work (some tables need the padding).
  Workaround used in 800601F0.c: computed goto table (`static void *const t[] = {&&c0,...}`).
- Pass-3 rules: `x *= -1` on signed char/SHORT gives lb/lh+negu (`x = -x` gives lbu/lhu); `slti` in a
  switch tree means the switch var is signed; a switch whose default returns garbage v0 has no final return;
  `sll idx` ahead of `lui jtbl` in the delay slot means a non-void function; an alias
  `T f_(..) __asm__("func_X");` handles callers with a different prototype; GTE code uses inline-asm macros
  with real opcode words (top of 80020298.c); permuting independent statement order fixes many reg swaps.

### Fourth pass: file split at object boundaries
- An object's jump tables are 8-aligned relative to that object's .rdata start (ASPSX `.align 3`), and
  objects are word-packed. A table whose address parity disagrees with its file's .rodata start means a
  missing object boundary: every switch-table function blocked in pass 3 fell on one.
- The ld script packs objects with SUBALIGN(2); ASPSX padded each object's .rdata to a word, so the
  Makefile (and linkcheck.py) append `.section .rodata; .align 2` to every C object. The old per-file
  `__asm__(".section .rodata\n.align 2...")` pads are now redundant.
- With the split, func_8003B080 and func_8005CB2C (switch tables) match, and the computed-goto
  workarounds in 800636D4.c (now status.c; 8006A234, 8006E34C, 8006E424) are plain switches again.
- The six address files were re-cut into 32 with a top-level C splitter; where one object had been split
  across two old files (mojidisp, pl_wep, sca) the second half's conflicting prototypes became
  `__asm__` aliases with an `_<oldfile>` suffix.
- syms/*.txt merged into syms/game.txt.
- Making a .c file: add `[rodata_offset, .rodata, game/x]` and `[text_offset, c, game/x]` to the yaml,
  write the C file with INCLUDE_ASM paths `asm/nonmatchings/game/x`, run `make split`, then `make`.

### Naming pass (RE2 code comparison)
- syms/re2_names.txt: ~290 functions named by comparing with RE2 source (callees, constants, table
  slots), each with its evidence; merged into symbol_addrs.txt by tools/apply_syms.py.
- Caution: RE3's Pl_mv/Pl_mv_pad tables match RE2 only for slots 0-4, so the older slot-derived names
  pl_mv06_pad/pl_mv07_pad/pl_mv08_pad are probably wrong. RE3's Sce_at_jump_tbl and Sce_jmp_tbl orders
  differ from RE2's, so those handlers were named by body. Dir_ck at 80018594 (em_sub) may be misnamed:
  80055AF0 in sce2.c is an exact match for RE2's SCE1 Dir_ck.
- Names given after the last main split (end of syms/re2_names.txt, "Phase 4") are NOT in symbol_addrs.txt
  yet: the main asm still has func_/D_ labels and INCLUDE_ASM names, so a fresh `make split` must keep
  producing them. They work through build/syms.ld (PROVIDE of every syms/*.txt), which the main link and,
  since phase 4, the overlay links use, so main and overlay C call them by name. C keeps the address name
  where main asm still references a C-defined symbol (Sca_get_dir = func_800502B8 from sabun asm,
  Sce_fade_set = Sce_fade_set from Sce_jmp_tbl data). For the next main split: tools/apply_syms.py
  (merges syms/*.txt into symbol_addrs.txt and renames func_/D_ uses incl. INCLUDE_ASM), then
  `rm -rf asm && make split && make`.
- syms/em_entry.txt: the enemy module entry points used by Em_move_tbl_set. Several modules share an
  address, so the file is PROVIDE-only; it is not listed in any yaml (splat refuses duplicate addresses and
  would label overlay code with them) and apply_syms.py skips it.
- PsyQ library: symbol_addrs.txt has a second block for the functions libsyms.py could not name: exported
  names from the 4.5 LIBs (libcd SYS/BIOS/S_002 and libpress are 4.5 code) and static functions named by
  order and size against psyz's PsyQ 4.0 symbols (../psyz/decomp/symbols.400.txt) where the module's
  sequence lines up. Fixed 800812CC (was GsSetProjection, a 32-byte false match; it is CdFlush). Left
  unnamed: the three libetc `memclr` statics (same name in three modules), libgpu SYS 8008C044..8008C420
  and 8008DB8C (4.4 layout differs from 4.0), libds DSREADY statics, 80086E38, libpad statics.
- Overlays: every name in ovl/<n>.syms.txt is now in effect (re-split), including the names the C used
  through `T Name __asm__("func_/D_...")` aliases (appended to the syms files); those naming aliases are
  gone. Aliases left are codegen aliases (other prototype) or aliases to unnamed exe symbols; WARNING.BIN
  was not re-split and keeps its aliases. em10: the syms entries Em10_haidm/Em10_haidie were functions,
  but RE2 uses those names for the tables (em10.h); the functions are the tables' only slots, RE2
  Em10_dm06/Em10_die01.

### Fifth pass: jump-table functions unblocked by the split (909/1208, 46.97%)
spl_auto.c is now complete; player 10 of 12 new, mojidisp 5 of 9, plus Se_on, func_800623C4, pl_mv05_w2_t1.
Left: func_8003A7D8 (srl/sra and a1/a2 swap), Player_set (s5/s6 swap, hoisted lui), Branch_disp (s7/fp swap
only), mess_trans (cross-jumping of the EB/EF tails), Print_main, Mess_disp. fdiff reports false %hi/%lo
diffs on jump tables and local rodata; linkcheck.py is the authority.
- s-reg swaps: `cc1 -dl -dg` writes .lreg with "used N times across L insns"; global-alloc priority is
  floor_log2(N)*N/L (N weighted by loop depth); higher priority gets the lower s-reg. A temp used in two
  blocks/cases goes to global-alloc; declaring it separately per block hands it to local-alloc (runs first,
  takes lowest s-regs).
- `srl` after a signed /0x200 fixup: the quotient was narrowed, `SHORT q = (x & 0xFFF) / 0x200; d = (q+4) & 7;`.
- `x = 0; x = 5;` drops the first store; the target's back-to-back stores had a memory read between them.
- `ox = 0; oy = 0; oz = 0;` and `ox = oy = oz = 0` store in opposite orders.
- Array vs pointer arithmetic: `G.arr[i+2]` keeps `addiu 2; sll`; `(&G.f)[i+2]` folds the +2 into the offset.
- Loop increment duplicated into each branch -> `p++; continue;` in each branch.
- gcc never hoists struct-field loads out of a loop with byte stores (QImode aliases); a load before the
  loop was computed there in source.
- `(rnd() >> (rnd() & 3))` in one expression keeps the intermediate in one s-reg; split statements don't.
- `if (x & (c ? A : B))` gives one load and two andi paths; `c ? x&A : x&B` gets CSE'd.
- `li`+`sw`+`lw` of a constant: `Attr |= 0x100; Mode = Attr & 0x100;` folded late by combine.
- `andi 0xFFFF` after GetClut: `ULONG Clut = GetClut(...)`.
- `Idx += Idx` and `Idx *= 2` allocate differently.
- Local aggregates `SHORT t[2][3] = {...}` become anonymous .rodata copied with lwl/lwr; tiny char arrays
  are built with sb; each local char array gets an 8-aligned stack slot.
- Repeated `&p->pSin_parts_ptr[k]` per call: one pointer variable per call, not one shared.
- Statement order sometimes fixes whole schedules (RE2 order first in Pl_init).
- Return-type aliases (`LONG f_l(...) __asm__("f")`) remove andi/sll-sra after UCHAR/char returns.
Unmatched drafts kept outside the repo (scratchpad): a7d8_draft_full.c, player_set_draft_full.c, bd_best.c, mt_last.c.

### Sixth pass (parallel agents): rules learnt
- Args set up in each branch but one shared `jal`, or one tail call after two prints: the source had the call
  in both branches and gcc cross-jumped the identical tails.
- Case bodies jumping to a shared test with `slt` in the delay slot: each case set a flag, tested after the switch.
- Case/default bodies are laid out in source order; a case falling into another reproduces the layout.
- A large constant kept in a register (`lui` early): the variable was assigned more than once
  (`Ret = 0x80000000; ... Ret &= x;`); a single assignment re-materializes the lui at the use.
- `Area = x; Area |= y;` allocates differently from one expression (destination live earlier).
- PsyQ headers define register-name macros (`R2`, ...): don't use them as variable names.
- GTE without nops: RE3 uses no-nop GTE command macros with statements between load and command; those
  statements must sit there in source too (asm volatile blocks scheduling).
- A counter in an s-register that never crosses a call: the variable was reused later for a value that does.
- `G.arr[G.Cbuf_id]` vs `pG->arr[pG->Cbuf_id]`: the direct form keeps `addiu base,0x44`, the pG form folds it.
- lhu from a LONG field: `(USHORT)G.f`. lb+lbu of one signed char before a branch: read it once into a local.
- Loop counter init placement (`Try = 0` after the first call, or `for (Try = 0;;)`) changes prologue scheduling.
- `(a - '0') * 10 + b - '0'` folds the offsets; a temp for `a - '0'` keeps the separate addiu.
- Inline lwl/lwr copies need `__builtin_memcpy` (the build uses -fno-builtin).
- Callees given char args with no andi/sign-extension at the call site were called through int prototypes.
- Overlay .rodata can end with unreferenced junk bytes: emit them with a top-level `__asm__(".byte ...")`.
- gcc 2.8 hoists a loop invariant only if threshold*savings >= loop insn count; read `-dL`. A goto into a
  `for(;;)` disables loop optimisation. Count-down loops came from `for (i = 6; i != 0; i--) arr[i + 6]`.
- **mfhi register after `x / CONST`** (the mult-high pseudo prefers HI). Reload spills HI when a basic
  block needs it for a real `mult` (mulsi3's HI scratch); pseudos living only in blocks without that
  need stay in HI and get the first free spill reg at the mfhi (often a3); the others are kicked out and
  re-placed by retry_global_alloc, which skips hard regs the function uses explicitly (args, a v0 return).
  So: (1) mfhi in t0/t2/a3 where we get v0: the function returned a value — make it LONG and
  `return callee_l(...)` (int-returning `__asm__` alias) on the path ending in that call (em30
  func_80104F20, em33 func_80101454, movie Init_movie_work, result func_801C229C/C3344/C53D4; this
  also replaced an earlier dead `Min = Time / 60;` hack). (2) mfhi in a3 where the target reuses the
  quotient's reg (v1): the later `mult` must sit in the same block as the division — repeat it in both
  branches and let cross-jumping merge the tails (snd Snd_pitch_scale: `v *= r;` in each branch).
  Read `.greg` (`-dg`): "Spilling reg 64" followed by " Register N now in R" shows the kick.
- Handwritten asm left as INCLUDE_ASM: BIOS jump stubs (`li t2,0xA0; jr t2; li t1,n`), libcard
  `_card_clear` (older-compiler epilogue), md_sub func_8002E318/D5D8/D4B0 (t-reg saves around jal, s-regs
  in a leaf, `jr ra; nop`).
- Hand-edited compiler output (stays INCLUDE_ASM): a frame size that is not a multiple of 8, a gap in the
  s-reg save area, or a duplicated instruction (gcc 2.8 always rounds frames to 8). md_sub's GTE sort family
  (B6FC, B86C, CF90, D6D8, D898, DC98, DE0C, DFE0, E160) copies its parameters into t-regs at entry
  (`addu t0,a1,zero`, D898 twice); no C or PsyQ inline_c.h macro form reproduces it (gcc 2.8 global-alloc
  always gives the parameter its incoming register unless it clashes with that hard reg).

### Cleanup pass (status/system/scenario side)
- New exe names went straight into symbol_addrs.txt (section "cleanup names"), followed by a main `make split`
  (9 s); overlays whose C uses a renamed exe symbol were re-split (`rm -rf asm/data/ovl/<o> asm/nonmatchings/ovl/<o>`,
  splat; em*: also `tools/mkovl.py --bss`). Overlay names go into ovl/<o>.syms.txt the same way. Rename C uses with a
  word-boundary replace over the whole tree. Check afterwards that no C file still uses a D_/func_ alias of a renamed
  address (title used D_80031BDC for Mess_hint_task).
- 800636D4.c is now status.c (one object). Its sprite tables are ST_SPR_DEF/ST_FT4_DEF/ST_TILE_DEF; item tables are
  ITEM_DATA (Item_data_tbl), ITEM_MIX, ITEM_CHECK, ITEM_TAMA; map data ST_MAP_DATA/ST_MAP_FLG (MAP_FLG_LIST keeps the
  load order the original has). Most box/status sprite tables (D_8009F3xx..D_800A00xx) still have address names.
- Aliases that remain are implicit declarations in Capcom's files (int return kept live in v0, or int arguments
  without the char/short masks) and array views of scalars (a store through an array is not moved before loads of
  struct fields); each says so in its comment. Calls through a void prototype matched for Pad_vib_*, Xa_play and
  the rain setter, so those aliases were dropped; Set_view, Quake_add, Kage_work_color_set, Flg_off and the
  LINE_SCR functions really need the int return.
- tagGLOBAL fields named (clean/gfield). Anchors: RE3's Flg_addr table (sce1, 0x8009E3F8) lists the flag words in
  order: System_flg, Status_flg (0x77F4), Stop_flg (0x2468), Scenario_flg, Room_flg[2], Enemy_flg, Enemy_flg2,
  Item_flg, Item_flg2, Key_flg, Pri_be_flg[32] (one word per cut, not RE2's [16][4]), ..., Use_flg, Om_flg,
  Item_chk_flg, Chaser_flg, Game_flg. The save block from 0x75F0 is RE2's SAVE_DATA reordered (no Bgm_tbl before
  Sce_work; Stage_no/Room_no at 0x783E/0x7840, Sce_random 0x784A, Change_player 0x784E), kept flat in tagGLOBAL
  (comments give the RE2 G.SaveData name). Em_routine[64] + Unk3EE0[24] were one RE2 Em_move_tbl[112] (sub player
  slots 0x40..0x57 = Spl). Nemesis state lives in G (Chaser_*: life, type, free work, room, state, drops, flags).
  C users of D_ aliases of G (Status_flg, Room_flg, Sce_random, Chaser_life/no, Pri_be_flg, Om_flg, pRoom,
  Ovl_mode, Esp_clut_num) and of Scenario_flg/Item_chk_flg/Chaser_flg/Map_get_flg/Map_area_flg/St_pl_kind now read
  G with identical code; Map_visit_set keeps the Map_visit_flg symbol (G changes its code). Left unnamed:
  Unk2111, Unk2114, Unk246C (bit 31 blocks pause and the sub-OT draw, bit 30 the tilt), Unk75D9, and the
  pads 0x211C (Flg_addr[42]), 0x7E34..0x7E54, 0x7E64..0x7E94 (0x7E74 is Flg_addr[48]).

## Code shared between overlays (src/enemy/common, src/ovl/common)

Overlays are linked separately, so identical routines exist once per overlay in the binary. In the source they
live once, in `src/enemy/common/<family>/<Name>.inc.c` (menus: `src/ovl/common/<family>/`), and every overlay `#include`s the file at the position
the function has in its own .text (order matters). As in sotn, a shared function has the same name in every
overlay that has it (the name comes from the family's first module, e.g. `Em10_mv04` in em16), set through
`ovl/<ovl>.syms.txt`; after a rename, `rm -rf asm/*/ovl/<ovl>` and re-split that overlay.

- em10/em16: em16 is a near copy of em10. All its routines, tables and its free work (include/enemy/em10_free.h)
  take the em10 names (RE2 slot names: Em10_mvXX(_br), Em10_dmXX, Em10_dieXX, ...). em16's own versions:
  Em10_dm_sel00 (no knock-down), and empty Em10_sce02 / Em10_ude_down; it lacks Em10_Kahannsin_walk.
  58 routines are shared (src/enemy/common/em10); same-asm pairs whose C still differs: Em10_dm07,
  Em10_die03, Em10_foot_set and a few more (em10's text does not build in em16 as is).
- Enemy ids: em26 = the young spiders em25 releases (Em25_Set_child, id 0x26 type 0x6000), em27 = the larva
  em23 spawns (Em23_child_set, id 0x27).
- em30/em33 (Grave Digger): one work layout, include/enemy/em30_free.h (EM30_W, EM30_SUB, EM30_PW,
  EM30_SABUN_P); 13 routines shared. Fields that the two modules use differently are unions (u10, u13).
  em33 names its Move routine and damage table Em30_Move / Em30_dm because the shared Em30_dm_fire and
  Em30_Damage call them.
- em34/em36: Em34_foot_set, Em34_blood_set, Em34_blood_spray, Em34_sce05 shared. em36 is not a copy of em34;
  the small dispatchers that em34/em36/em37/em39 all have (Em34_Move, Em34_Sce, Em34_Move_type02) were left alone.
- jill_sel/select/result: Jill_pl_trans, Jill_light_set and Jill_obj_init/draw/rgb are shared; the sprite object types
  (SEL_OBJ, SEL_SPRT, SEL_SPRT_DEF, SEL_OBJ_* flags) live in include/ovl/jill_obj.h. SEL_SPRT_DEF 0xA/0xB are Semi/Abr
  (SetSemiTrans / GetTPage abr). Jill_wep_load (= select's copy) is still unmatched in both.
- em20/em25: Ck_em_id.
- em38/em39: Em38_part_scale_pos. The small table dispatchers (Em*_Move, Move_type, Scenario, Pldamege/Pldie)
  are asm-identical across many modules but name different tables, so they stay per module.

tools/ovlshare: `asmgrp.py` lists asm-identical functions across overlays (writes build/ovlshare_groups.json);
`align.py A B` aligns two overlays' functions in order; `rename.py A B [--apply]` gives B's func_ copies
A's names; `maprename.py OVL MAP` renames from an "old new" list; `trytext.py A B` tries A's C text for
B's same-named functions and keeps it when all outputs stay OK; `share.py FAMILY A B... [--apply]` moves
text-identical functions into src/enemy/common/FAMILY (or src/ovl/common/FAMILY).

## Near-miss status (agentE, drafts in the session scratchpad agentE/r2)
- Flr_set matched: the base pointer is a temp that gets overwritten afterwards (`p = (UCHAR *)pFlr; p += n * 12;
  pAttr = (USHORT *)(p + 12); p = pData;`), otherwise cse folds the +12 into the offsets.
- Plc_gun_eff matched: dead stores into a local VECTOR; cse deletes the repeated vx = 145 (8 stores for 9 assignments).
- Get_light_cut (3): reassign `pData = (UCHAR *)pLight` after the loop; assign pCut twice (REG_N_SETS > 1 stops
  sched1's birthing boost). Left: the copy goes to a1, not a0 (pData inherits pLight's a1 preference).
- snd Snd_bgm_play_ck (2): the CD `lui` gets v0. With temps (`t = &Snd_work; u = &CD;`) the registers are right
  but sched2 puts both luis first (3).
- oba func_80034E78 (3): sll 8 / andi 0xff order. `((Fl << 8) & 0xFF00) | (Fl & 0xFF)` gives the order but
  swaps v0/v1: local-alloc gives the 6-ref shift chain priority over Fl (5 refs).
- oba func_80035678 (9, agentC pm2.c still best).

## Matching round agentF (drafts in the session scratchpad agentF/<dir>)
Rules learnt:
- "Parameters copied into t-regs at entry" is not proof of hand editing. The GT3/GT4 sort routines
  (md_sub B6FC/B86C/CF90, 80079EC0 BC7C/BDF8) are C: a `goto` loop (no loop opt, constants not hoisted) whose
  next vertex numbers are read ahead inside the body (`pZ0 = &pSz[v0]; ...; v0 = pPrim[1] >> 24; ...` before
  gte_stopz). The block-local index temps take a0-a3 and the parameters move to t-regs. Vertex numbers are word
  shifts of the uv words (`W[1] >> 24` gives lb 7, `(W[2] & 0xFF0000) >> 16` lbu 10), not byte fields.
  `if (n == 0) return p;` vs `goto end` changes whether `move v0,t0` comes before the s-reg restores.
- One local reused for two values fixes allocation more often than anything else: func_800503E8 (offset from
  the corner, then the centre, in the same X/Z), CF90 (`Uv3` first holds the prim code), e5fc near miss (Dz holds
  the half width first), Cut_be_set (index in the Cut variable).
- `do { ... } while (0);` around a statement range raises the loop depth of its refs and so the global-alloc
  priority (sca koban x/z, em_sub func_8001BA48, func_80018EC8). tools: agentF/dwgen.py FUNC.c OUTDIR wraps every
  sibling statement range; feed the dir to tools/permute/score.py.
- A plain copy that survives as `move` (Spd_get y): `ULONG Raw_y = (USHORT)p->vy; y = Raw_y;`.
- A table pointer local (`pUv_tbl = D_80098B28;` then `pUv_tbl[i]`) orders lui/addiu before the index sll.
- agentF/dscore.py DIR FILES: like score.py but counts difflib-unmatched words, so an extra/missing
  instruction does not count every later word.
Near misses:
- md_sub func_8002DC98 (dc98/e.c, ~14 words incl. a 1-insn shift): GT3 sort with per-prim colours. Needs the
  loads paired in temps (`W0 = ...; W1 = ...; store; store`), `Z0 = *pZ0` read before `if (Z > 0)`, separate
  stsxy0/1/2 asms with the Otz read between the first two. Left: `sw s0` should sit in the lhu delay slot of
  the prologue, and pZ0/pZ1/pZ2 get v1/a1/a0 instead of a1/a0/v1.
- 80079EC0 func_8007B2FC (b2fc/b.c, 10 words): outer do-while (keeps &M+2/&M+4 and &LIGHTMATRIX hoisted, s0/s1),
  inner read-ahead do-while. Left: gcc makes two givs for pData+2 (count read and inner loop), target one.
- em_sub func_80019178 (s178/e.c): Spd_get twin on Unk1A0/Unk1A1 with 32-byte bone records; the target keeps
  pSeq_t/pKan in the a-regs and copies them (t3/t5) for the second (Move_cnt == 0) part, and adds Bone_ofs to
  the already advanced pKan there (a Capcom bug). Ours never produces the pSeq_t copy (one insn short).
- sca func_8004C57C (koban_x hit, c57c/t1.c ~44 dscore): matches structurally only if pW (&D_800DEB58) and the
  Tmp_Sca pointer are one variable (a2); rejected because that needs casts between SCA_WORK and SCA_DATA.
- sca func_8004E5FC (e5fc/rr/g16.c): Dz reused for the half width gives the pT copy to t7; regs still off.
- em_sub func_8001B894 (b894/e.c): Esp kill by Id; per-loop pointer from a top-level pTop gives the loop shape,
  registers still differ everywhere.
- em_sub func_8001BC80 (bc80): target loads all six halves first; no C form found (30).
- sub func_800790F0 (s90f0/sw3/x1.c, 12): ICON_DATA icon select; cse reuses the All_a register for the
  constant 1 in the second compare.

## Matching round agentH (drafts in the session scratchpad agentH/best)
Matched: om Ot_sub_init, select Select_pl_load.
Rules learnt:
- An early return that duplicates the function tail (`if (!x) { tail; return p; } call(); tail; return p;`) is
  cross-jumped back into one tail after register allocation, but before that it adds refs to the pseudos the tail
  uses, which changes global-alloc priorities. Select_pl_load needed it to swap pEm/p (s5/s4) (the permuter found it).
- Two nested loops whose counters swap s0/s1 between the loops: swap the variable names in the first loop.
- Copy idiom from Jill_pl_load: `Cd_read(.., (ULONG *)(p = pWork), ..)` makes cse fold pWork into p.
- One local reused for unrelated addresses (Pl_wep_parts_drop: `&pP->Workm.t` for Sca_get_ground/Snd_se_stad and then
  `PW_M(pP)` for RotMatrix/MulMatrix2) stops loop.c from strength-reducing each address into its own s-reg giv.
  A variable set once inside a loop from the biv becomes a giv; a variable set more than once does not.
- score.py candidates see the preprocessed base.c: macros (EM3E_W, CDIR_SV, PW_M, WORKM_POS_V, NULL) must be expanded
  in the candidate. Otherwise cc1 takes them as implicit function calls and drops code without an error.
  agentH/sc.sh expands them and then scores.
- Don't `pkill -f` with a pattern that also appears in your own command line. It kills the tool shell.
  agentH/pk.sh kills only the python permuter processes.
Near misses (agentH/best/<func>.c):
- pl_wep Pl_wep_parts_drop (2 words): this is the 256-iteration UCHAR do/while over pSin_parts_ptr[15..]. Everything
  matches except the order of `addiu a0,CDIR` / `addiu s0,PW_M` in the rot block that uses the multi-set pPos
  variable. Getting the register allocation right needs pPos (void *) for &Workm.t, PW_M in the Timer1 == 0 block,
  and a separate pM2 in the Timer3 & 1 block.
- spl Spl_wep_set (12): a1/a2 swap only. One pP holds [4] and then [15], with `pHand = pP` copied in between. The
  priorities give pP a1 and the target gives it a2.
- epilog Epilog_draw (12, prologue only): the target issues both luis (Epilog_work, G) first. `do { pWk = &Epi_work; }
  while (0)` gets 1 of the 2 diffs.
- pl_wep Mag_down (33): the same ground-bounce code as Pl_wep_parts_drop. Use `*= -1` for Old_y/Old_z (lh) and locals
  for the Old_y/Old_z adds. The target loads Old_y, t0, Old_z and t2 before both stores.
- em3e Em3e_fly (27, needs `#include "player.h"`): the beqz on Shot_wait has a nop in its delay slot (we fill it with
  the decrement), and the player/enemy Pos_x registers differ.
- option Config_moji_disp (~80) blocks Config_suuji (matches, shares the "\x81\x40" literal). The inner search loop is
  `for (i = 0; !Config_moji_cmp(pStr, *pTbl); i++) pTbl++;`. The target does not hoist the 0x80/0x58 colour constants
  out of the outer loop (their stores are cross-jumped) but does hoist &Opt and Config_moji_space.
- sub Retry_file_load (= agentF's func_800790F0): in the target, Bit = Flg & 0x4000000 is computed first and not
  combined into the branch. Every form tried gets it computed last.
## Matching round agentG (drafts in the session scratchpad agentG/<dir>)
Matched: sca Sca_hit_obj_circle (was func_8004D404), Sca_get_ground_obj (func_8004DB10), em_sub Esp_pos_matrix.
Rules learnt:
- A pseudo set before a call and used after it may be scheduled across the call by sched1 (it already crosses
  one), but its REG_LIVE_LENGTH is taken from the source order. A constant address assigned before the call
  (`pW = &D_800DEB58;` at the top) is what puts it in an s-reg even when the lui shows up after the jal
  (Sca_hit_obj_circle). Likewise `Ret = 0` moves before a call freely.
- `Ret |= f(); return Ret;` vs `return Ret | f();`: the extra refs raise Ret's global-alloc priority (s2 vs s6).
- Local-alloc runs before global-alloc: a value local to one basic block (calls do not end blocks) takes the
  lowest free s-reg before any global pseudo. Reusing one variable in a later block makes it global and drops it
  behind the block-local ones (func_8004F098: separate Dx2/Dz2/Half2 for the second test).
- Two copies of the same value in one function that keep separate registers: a pointer variable used for the
  stores (`pC = &Tmp_Sca; pC->Z1 = ...`) gives `addiu a2,hi,lo` + `sh 6(a2)` and lets the call reuse a2; the
  offset-0 store still comes out as `sh %lo(sym)(hi)`.
- Esp_pos_matrix: the six-word `*(int *)&M.m[..] = ...` init through `pM = &M` gives `sw 0x10(sp)` + `sw n(s5)`;
  statements between a no-nop-free GTE command and its store asm are real source statements; the tail values that
  land in the same s-regs as earlier values are the same variables (Ox = Workm.t[0] ... Ox = Ofs.vx).
- dwgen sweeps (agentF/dwgen.py) are worth running on every draft that is down to register swaps: Esp_pos_matrix
  went 24 -> 0 with a second do/while(0) found by a full sweep. Score with tools/permute/score.py over `xargs -n 300`
  (a single argv of 3000 paths silently produced nothing). The decomp-permuter mostly produced semantically wrong
  sources (uninitialised variables) on these sca functions.
- Splat marks functions with mtc2/ctc2/lwc2 as "Handwritten function"; that is only the GTE macros, not hand-written asm.
Near misses (score.py words / dscore):
- sca func_8004B874 (b874/z/z6.c, f1.c; ~5 words): segment push-out against the Pt0-Pt1 wall. Left: the
  (SHORT)P3/(SHORT)P2 extension order/regs and one load-delay nop around `if (Opz[0] > 0)`.
- sca func_8004C6EC (c6ec/o/o54.c, ~34): koban_z; the pC pointer per case fixed the call arg; left: pW/D_800DEB58 base
  register (target a2) and Cz0/pObj register numbering. func_8004C57C (koban_x) is the same code on X.
- sca func_8004C960 (c960/e.c, dscore ~95): box push (RE3 Sca_hit_box); structure matches, target has higher register
  pressure (R in s1, Z in s0, pT copied to t6).
- sca func_8004CE2C (ce2c/b.c, dscore ~71): four-wall diamond/rotated box calling func_8004B874 per side; rotated
  corners are computed as offsets first then +X0/+Z0 with a surviving copy (`move v1,a3`), not reproduced.
- sca func_8004F098 (f098/v/f1.c, sweep sw_f098/dw169.c, 43): circle line test; s-reg order Dz/Z0/Dx/P0 not reached.
- md_sub Mapping_md (map/*.c, 7): giv base; gcc's combine_givs takes the last recorded giv (n_prim4) as base,
  target uses &n_prim3; reading the counts through a `USHORT *pN = &n_prim3` does not change it.
- em_sub Goto01 (g01/sw3, 2304 type combos, best 21): the raw-copy + in-place sign-extension pattern still not found.
- em_sub Esp_kill (b894/f.c, dscore 74), Esp_speed_add (bc80, 30): no progress.

## Matching round agentJ (types; drafts in the session scratchpad agentJ/best)
Matched: md_sub Mapping_md, em_sub Esp_speed_add.
Type fixes (all 60 OK, none changed codegen elsewhere):
- MD_OBJ 0x14 is one `ULONG n_prim` (low half GT3 count, high half GT4 count), like n_vert. Mapping_md reads it as
  one word (`n_prim & 0xFFFF`, `n_prim >> 16`), which is what makes loop.c pick &n_prim as the giv base; every other
  user reads `(USHORT)p->n_prim` (lhu 0x14) / `p->n_prim >> 16` (lhu 0x16, combine narrows the shifted load).
  Touches md_sub.c and four lines of om.c.
- KAN_HEAD.Rec_sz is one word too: low half = parts count (lhu 4), bits 18.. = record size (lw 4 >> 18).
- ESP_WORK.Unk08/Unk0A (sprite scales) are USHORT (lhu in Esp_trans_main/3d); one `(USHORT)` cast in em_sub dropped.
- Spd_get_sub has a real prototype (pEm, pKan, pSeq_t, R_flg). md_sub `*(USHORT *)&p[n]` casts became `(USHORT)p[n]`.
Rules learnt:
- Two halves of one struct word read with lhu at +0/+2: check whether the original read a ULONG (`& 0xFFFF`, `>> 16`);
  gcc narrows both to lhu, but loop/cse see one memory word, which changes giv bases and register choices.
- "Loads all six halves first" (Esp_speed_add): two sets of SHORT temps reused for the second sum
  (`Ax = Spd_x; Bx = Mov_x; ... Mov_x = Ax + Bx; ... Ax = Acc_x; Bx = Spd_x; ...`). The re-read of Spd_x/Spd_y vs
  the register copy for Spd_z falls out of cse by itself.
- cse.c make_regs_eqv (2.8.1, line 871): after `B = A` the new pseudo B becomes canonical only if it lives beyond the
  current cse block and its last use is later than A's; otherwise A stays canonical and later reads of B are rewritten
  to A. In Goto01 our `Turn = Add_dir` always makes Turn canonical (the -ds dump shows the param copy rewritten to set
  Turn directly); the target's `move v1,a2` + in-place `sll/sra a2` needs Add_dir to stay canonical. Not found with
  if/else forms, K&R definitions, params modified in place, or 3000+ type combos.
- A local constant assigned before an early `if (n == 0) return` keeps its s-reg save at the top of the prologue
  schedule; assigned after the test, the `sw s0` drops behind the loads (SortGT3_col Vmask).
- `li v0,1; sll v0,v0,2; addu` (unfolded giv init) means the loop counter's `= 1` was not in the loop pre-header
  (set at the top of the function, before an earlier loop): Sa_joint_blend `j = 1` at entry.
Near misses (agentJ/best):
- em_sub Goto01 (Goto01.c, 3 words): `Turn = (USHORT)Add_dir; USHORT Tgt = Dir` gives every register right; left are the
  two `andi 0xFFFF` the target does not have (target: raw `move v1,a2`, no andi before `subu v0,a3,a1`). Goto00 has the
  same pattern (copy of Add_dir made before the Direction_ck call, extension after it).
- md_sub SortGT3_col (SortGT3_col.c, ~88 score, mostly one shift): needs `n` read from n_prim now; Vmask after the
  n == 0 test fixes part of the prologue; pZ0/1/2 registers still v1/a1/a0 vs a1/a0/v1.
- sabun Sa_joint_blend (Sa_joint_blend.c, ~90, struct right): the first loop's three `lh` and the GTE loop's three
  `lhu` share one giv in the target (0/2/4 off one register); every form tried splits off a second giv for +4.
  Uses `SVECTOR Tmp` filled field by field for gte_ldsv, `Shape_sz = pHead[2]` hoisted, `j = 1` at the top.
- trans Esp_trans_3d (Esp_trans_3d.c, 70): RE2 TRANS.C shape with X/Y/X1/Y1 SHORT temps computed before rtps; frame and
  stack layout match. The target saves s2 but never uses it (the mult products were allocated to s2 and later moved
  to v1 by reload); ours keeps them in s2.
- em_sub Spd_get_sub (Spd_get_sub.c): the pKan copy (t5) comes out right, the pSeq_t copy (t3) never does.
- Kage_work9_sort (not attempted) saves t0/t3/t4 around its jals; probably gcc caller-saves (on at -O2), so C, not hand-written (unverified).
## Matching round agentK (types; drafts in the session scratchpad agentK/best)
Matched: sce2 Obj_model_set (272 insns, first try from RE2 SCE1.C's version once the types were right).
Type fixes (all 60 OK, committed separately):
- PARTS_W 0x94 is RE2 Timer0 (was Unk94): Pl_wep_ready_reset is RE2 G_rot_st (Timer0/2/3 = 0), Mag_set/Mag_down
  use it as the fall speed.
- OBJ_MODEL_WORK embeds `COL_OBJ Col` at 0x80 and `PARTS_W Parts` at 0xC8 (RE2 has Parts at its end). The old
  UnkAE/UnkBA/Unk130 were Col.Hit_flg, Col.Unk3A and Parts.Poly_rgb (om.c renamed). COL_OBJ and PARTS_W moved above
  OBJ_MODEL_WORK in bio3.h. Obj_model_set fills Col (W0/H/D0, Unk24..28, pEm, Flg, Type; Oba_set(&pOm->Col, 0)).
- tagGLOBAL.pOm_target is `struct tagOM_TARGET *[10]` (pl_wep.c's three `(OM_TARGET **)` casts are gone, om.c/sce1.c
  read ->Flg instead of `*G.pOm_target[i]`; sce1.c now includes pl_wep.h).
- St_box_frame_draw takes the ST_WORK (prototype was `LONG Arg0`).
- G_pRoom (root_ck.c, not touched) is just G.pRoom (0x800CC86C = G + 0x2134); Cd_link_search reads the link list as
  G.pRoom->pEdt0 (the snd.c room sound table and the CD_LINK list are the same 4-byte records).
Rules learnt:
- Capcom reuses a handful of Work_* locals for unrelated values (RE2 Obj_model_set has Work_a/b/c/d). A variable set
  more than once is not tied by cse/local-alloc like a single-set one; in Obj_model_set the last two register diffs
  went away only when Pos_z was read into the same local that earlier held the flag byte (Work_d), and the first
  local (Om_no) also carries the flag index for Flg_ck before it holds the model number. When a value sits in "the
  wrong" scratch register, try reading it into an existing local that already holds another value.
- An argument computed before the condition of a guarded call (`addiu a0,s0,ofs` in the delay slot of the first
  test) came from a local assigned before the test (`pFlg = pG->Om_flg; if (n == 0xFF || !Flg_ck(pFlg, n))`).
- Early `return` vs the whole body inside `if (skip test fails) { ... }` changes the epilogue order (`li v0,1`
  after the s-reg restores in the target).
- A private record struct for opcode data (SCE_OM_SET) instead of `*(SHORT *)&pData[n]` reads.
- tools in agentK: try.sh SRC FUNC DRAFT (insert + fdiff + restore, KEEP=1 keeps a match), mk.sh (permute dir +
  inc.h for -imacros), sc.sh DIR files (expands every header macro through `cpp -imacros DIR/inc.h`, so no per-macro
  sed list is needed), ad.py (aligned diff of `V=-v sc.sh` output), dump.sh (cc1 -dL/-dl/-dg dumps of a draft).
Near misses (agentK/best):
- pl_wep Mag_down (12): the RE2 layout (adds inside the Timer3 & 1 block, `if (Timer1) return;`) is right; reading
  Old_x/Timer3 through one reused local puts Timer3 in a0 like the target but leaves a v0/v1 swap in the halving.
- pl_wep Pl_wep_parts_drop (2): unchanged; a multi-set pointer gets `s0 = M` scheduled before `a0 = CDIR`, a
  single-set one gets the right order but loop.c combines the two blocks' M givs (benefit 4) and strength-reduces them.
- em27 Em27_Init (18 with Top/R sharing one constant): target loads pSin_parts_ptr once and copies it (pP = copy,
  pM from the load); every order tried loads it twice or ties the copy.
- cd Cd_link_search (54): structure matches; the loop's 0xFFFF compare constant is hoisted
  by loop.c in ours (threshold: the target loop must have ~5 more RTL insns or one more movable moved first).
- status St_map_area_select (~118, layout of case 0 and the G re-materialisation after the cancel test),
  St_box_frame_draw (~40: AddPrim offsets need the Ot index in a variable per loop, loops 2/3 do not hoist `ot*8`).
- em22 Em22_dir_quarter (13 with a static inline helper, 16 without): the double `move v1,a0; move v0,v1` return.
- pc_sys Pc_box_set (12), Pc_text_set (7): unchanged (register choice only).
## Matching round agentI (types; drafts in the session scratchpad agentI/best)
Matched: oba Oba_box_push. Type fix: door.h DOOR is RE2's tagDOOR minus pEdh_offset/pVb_offset (pPackettop,
DIVPOLYGON3 Divp at 0xC, Ctr1 0x224, Ctr2 0x226 = the old Unk226 frame counter, V0..V2 0x228.., Sound_flg 0x240,
Div_max); Door_init's 0xC/0x10/0x14 stores are Divp.ndiv/pih/piv.
Type findings:
- Tmp_Sca (0x800DEB48, 0x10) and Sca_work (0x800DEB58) are separate objects: every access uses its own %hi/%lo.
  The koban "one variable" observation (agentF) is reproduced cast-free by a local
  `union { SCA_WORK *pW; SCA_DATA *pC; } u;` (same code as the casts), but the function is still off (see below).
- Sca_work.Pt0/Pt1 are packed GTE SXY words (x << 16 | z & 0xFFFF), read with lw + sll/sra (no SHORT fields).
- Arg3 of the Sca_hit_* family is the packed position P1 that Sca_hit_line passes on (ULONG).
- COL_ENT shares SCA_DATA's first 0x10 bytes; Oba_tmp_sca_set fills Tmp_Sca and passes it to Oba_ent_floor_set as
  a COL_ENT (its 0x18-byte unused local is a COL_ENT).
Rules learnt:
- Use the .lreg/.greg numbers (agentI/dump.sh DIR FUNC.c OUT runs SN cc1 -dl -dg on base.c + draft): global-alloc
  hands out t0, t1, ... in priority order floor_log2(refs)*refs/live_length. Reading the target's register order
  as a priority order tells which variable needs more refs / a shorter life (do/while(0) adds 1 per ref).
- A boolean that is copied into a second variable (`Zero = (Dx | Dz) == 0; Still = Zero;`) was what Oba_box_push
  needed (sltu early, both regs right); a single variable of any type was 45 words off.
- Cases that each compute a flag and jump to one shared test: put the test in the first case behind a label and
  `goto` it from the others (Sca_high_hishi); `if (!In) return High; return pT->High;` gives the
  `beqz; move v0,a3` / `lh v0` tail.
- `Zone = (ULONG)Dz >> 30; Zone &= 2; Zone |= (ULONG)Dx >> 31;` reproduces the `srl/andi v1` then `or v1,v1,v0`
  order of the area-bit switch (one expression puts the first half in v0).
- loop.c (read `-dL`, "Insn N: regno R (life L), savings S moved/not desirable") hoists an invariant when
  threshold * savings * life >= the loop's real insn count, threshold ~58 for a call-free loop: a life-1 use is
  never hoisted, life 2 is hoisted in a 107-insn loop. Oba_em_overlap_ck's target keeps `sll v0,W,1` (used by
  two adjacent adds) in the loop, so either its loop has more RTL insns than our draft or W * 2 was computed
  twice and merged by cse2. Reusing a variable for W * 2 (set twice) also stops the hoist but is not what the
  target does.
- &G from &Pl_work: `pPl = (ENEMY_WORK *)&G.Player;` then `G.x` lets cse derive &G as pPl - 0x248C (the target's
  `addiu s5,Pl_work; addiu s3,s5,-0x248C`). `Tbe` kept in a variable changes the load order of two Be_flg reads;
  writing `pT->pEm->Be_flg` at each use gives the target order (cse still loads it once).
Near misses (agentI/best):
- sca Sca_high_hishi (16; 13 with three do/while(0) in _13_dw.c): shared-test layout and the zone order match;
  `Zone = pT->Z1 - pT->Z0; Cz = pT->Z0 + (Zone >> 1);` with Dz holding the X half width. Left: t-reg order
  (X0-load t1, Z t3, Dz t5, Dx a2): needs X0 above Z1 and Dz below Z0 (see the priority rule).
- sca Sca_hit_koban_x (22): u union pointer, `Zone` in two statements, case 0 stores X0 last from a temp
  (`Cx1 = pT->X1 - D; ... pC->X0 = Cx1; goto Circle;`), case 3 holds the Circle label. Left: priorities (target
  X0 > X > pT > D > Z0); do/while on X0 and pW (_30_regs.c) gets X0/X/pT right but swaps D/Z0.
- oba Oba_tmp_sca_set (5): `Y <<= 16; C.All = ((Fl & 0xFF) | ((Fl << 8) & 0xFF00)) | Y;` gives the registers
  but `andi 0xFF` before `sll 8`; the other orders give v1/a2 swapped.
- oba Oba_em_overlap_ck (30, new draft): structure complete; left Shape in t1 instead of the Id register and X/Z
  in a2/t0 (the W * 2 hoist is stopped by reusing Shape, which the target does not do).
- door Door_init (10): needs `ppTim = &Door_data->pTim` to get the 0x8019D004 register; left the pAdr value in a1
  with a copy to a0 for Mapping_tim (ours computes it straight into the a0 copy).
- sca Sca_hit_line (13, agentG z6): unchanged.
- oba Oba_ent_box_set (new draft, ~90 dscore): four values are reload spills (sw then lhu from 0x10..0x1C(sp)),
  Type kept in fp; ours spills one more.
## Matching round agentL (drafts in the session scratchpad agentL/best)
Matched: door Door_Trans, sca Sca_ck_line, Sca_get_ground (all three written from the asm, then fixed with the
tools below). Type fixes: SCA_WORK 0x24 is `Gnd` {Id, Type} with a ULONG view `All` (Sca_get_ground tests
`Gnd.All & 0x0F00000F`); DoorSort takes (DOOR_WORK *, POLY_GT3 *packet, SORT_PARAM *) and Door_Trans fills a
SORT_PARAM {pTmd as MD_OBJ *, Poly_rgb} on the stack; new names Door_light_matrix / Door_color_matrix (0x80097574 /
0x80097594, RE2 D_LIGHTMATRIX / D_COLORMATRIX), Sca_block_list (0x800DEB98, Sca_get_block output), Sca_high_tbl
[2][13] extern (index [Flg & 8][shape]).
Tools (agentL): prio.sh PERMDIR DRAFT FUNC prints every global-alloc candidate with refs / live length / priority
(global.c allocno_compare: floor_log2(refs) * refs / len) and the hard reg it got, from SN cc1 -dl -dg; rmap.py
renames our registers in a `score -v` listing ("s5=s6,...") to see what is left besides allocation; dmp.sh runs SN
cc1 with any -d flags on base.c + draft (macros expanded through DIR/inc.h).
Rules learnt:
- One variable reused for two unrelated values is what makes a pseudo global (live over several blocks) and so
  allocated after the block-local ones. Door_Trans: `pD = pDoor` at the top and `pD = (DOOR *)pDw->pSuper` in the
  loop are one variable (both a0); a separate pSuper local was given v1 by local-alloc. Likewise `pM` holds
  pDw->M (RotMatrix arg), then &Lm, then pDw->M again (v1); with direct `pDw->M` both M addresses are DEST_REG givs
  that loop.c combines (benefit 4) and strength-reduces into an s-reg the target does not have.
- loop.c reduces a DEST_REG giv when lifetime * threshold * benefit >= insn_count (loop.c 3914; threshold
  = 3 + n_non_fixed_regs with a call in the loop). A variable set more than once is never a giv: keep the
  &pDw->Workm / &Lm pointers in multi-set locals when the target recomputes them each iteration.
- A stack struct accessed through one register (Sca_ck_line A/B in s1/s0, Sca_get_ground Pos in s2) is a pointer
  local (`pA = &A; pA->vx = ...`); with `A.vx` the accesses are sp-relative and the allocation changes completely.
- Call arguments computed into locals first (`P0 = ...; P1 = ...; Ret = Sca_line_tbl[..](P0, P1, pT);`) give the
  target's "all loads, then the table lookup" order; inline in the call the table load is scheduled first.
- local-alloc.c optimize_reg_copy_1 (flag_expensive_optimizations) rewrites uses of S after `hard = S` to the hard
  reg up to S's death unless a label, jump or LOOP_BEG/END note is in between: that is why a surviving
  `move a0,a1` + later use of a1 (Door_init) needs a loop note between the hoisted copy and the last use.
- Prologue-area load order (two independent loads swapped) was fixed by a permutation sweep of the assignments
  before the first branch (Sca_get_ground: 1260 orders, two hit 0); dwgen sweeps then fixed the priorities.
- decomp-permuter cannot parse drafts with `__asm__` (GTE macros): it silently never starts (DoorSort).
Near misses:
- door DoorSort (best/DoorSort_regs.c, ds4): structure, goto loop, switch layout (case 0x30 first, its `pOt +=
  Otz >> 7` cross-jumped with case 0x20) and spills all right; left is s3/s4 (pVert / pPrim): pPrim has 9 refs
  (0.200) and pVert 7 (0.108), the target needs pVert above pPrim. Reading the first indices through
  pObj->pPrim3 (best/DoorSort_T.c) gets the order but leaves the temp in v1 plus a move. pOt above pPacket needs
  case 0x10 in two statements (`pOt = G.pOt1 + (Otz >> 7); pOt += 0x207;`), which changes that case's code.
- door Door_init (best/Door_init_9.c, 9): unchanged problem; the sum must stay in a1 with the arg copy in a0.
- sca Sca_hit_slope (best/Sca_hit_slope_draft.c, ~180, same as agentG ce2c): flags/walls and the four
  Sca_hit_line calls match; the rotated corners are computed as offsets (w + d, d - w, -w, and a surviving
  `move v1,a3` copy of d) before +X0/+Z0, not reproduced with temps (cse folds them).
- sca Sca_hit_koban_x (22, agentI), Sca_high_hishi (13), Sca_line_circle (51; nearly all s-regs are local-alloc
  qtys in block 0, so prio.sh shows only three of them), Sca_hit_box, Sca_hit_line: not improved this round.
- Not attempted for lack of time: Sca_hit_hishi (559 insns, divides by 18 via 0x38E38E39), root_ck kara_rootck2
  (477 insns, heavy spilling: ~8 values live in sp slots).
## Matching round agentN (worktree clean/xC; drafts in the session scratchpad agentxC/best)
Matched (80079EC0.c): Skin_light0, Skin_light1, Skin_vtx0, Skin_vtx1_alt (was labelled hand-written).
Proven hand-written / hand-edited (stay INCLUDE_ASM, evidence in the md_sub.c comments):
- Frames that are not a multiple of 8: of ~2390 functions only SortGT3_neo_uv (-0x1C), SortGT4_neo_uv (-0x24),
  SortGT4_col, SortGT3_sabun, SortGT4_sabun (-0x1C). gcc 2.8 always rounds to 8.
- CompM: leaf loading into t0..t7 while v0/v1/a3 are free, columns software-pipelined across mvmva; not PsyQ's
  CompMatrix. Md_rtp: s0/s1 used in a leaf with every t-reg free, pipelined z clamp. SortGT3_neo: a C load between
  the two lwc2 of one vertex (the only split lwc2 pair in the game). Skin_vtx0_old: macro text split (ctc2 from v0/v1)
  and C insns inside the GTE slots.
- Not hand-written: Skin_vtx1_alt/Skin_vtx2_alt/Skin_vtx2 (reload rematerialises constants/addresses into a spill
  register, `sw a0,0x60(sp)` parameter home-slot spills); Sa_morph_step (s3 used as a reload register).
Rules learnt:
- Inner loops written as `label: ... if (n) goto label;` inside an outer do/while: loop.c then reduces the outer
  pData+k givs once (Skin_light0: one pData+2 giv for the count and the triplets; a do/while inner loop makes its own
  giv and hoists the &local addresses). Sa_joint_blend needs the same for both inner loops.
- loop.c giv reduction rule (loop.c:3914): reduced only if lifetime * threshold * benefit >= insn_count, threshold =
  (call in loop ? 1 : 2) * (3 + 59). With a call (CompM) the combined pData+1/+2 givs need the loop to have < 187
  RTL insns; Skin_vtx1's target does not reduce them, so its source is longer than ours (or the count read is no giv).
- 12-byte arrays (BLKmode) are padded to 16 in the frame. Three address-taken scalars give 12 contiguous bytes;
  but scalar (non-struct) stack stores do not alias struct loads, so sched moves `Sz = 0` stores up. 4-byte unions
  (SXY Sz0..2, `.l`) give both: 12 bytes and MEM_IN_STRUCT stores ordered after the pEnv/pEm loads (Skin_vtx0).
  A 24-byte struct {SXY Sxy[3]; LONG Sz[3];} gives the 0x18-aligned block of Skin_vtx1(_alt).
- One temp for half of an expression (`Hi = (a & 0xFF00) + (b & 0xFF00) >> 1; *p = (Hi & 0xFF00) | ...`) changed
  local-alloc priorities enough to fix a v0/v1/a0 rotation (Skin_light1, found by the permuter, then cleaned up).
- `do { stmt; } while (0);` around a single statement (agentF dwgen sweep) fixed a t0/t1 swap in Skin_vtx1_alt.
- flow.c (last_mem_set) deletes a store only if the next store in program order hits the same address with no MEM
  read or other store in between: two consecutive stores to an address-taken scratch local survive only with another
  store/load between them in the source (Esp_trans_main's D0/D1/D2 dead stores).
- A product assigned straight to a user variable (`x0 = a * b; x0 += c;`) is mflo'd into a GR; as a temp inside an
  expression it prefers LO and is copied out through a reload register (often a call-saved one that is then saved but
  never used, as in Esp_trans_3d/Esp_trans_main/Sa_joint_blend's targets).
- Disk: the root fs filled up during this round (another agent's scratchpad held 3.6 GB); a full disk silently
  truncates drafts and makes score.py report "196 / 0 insns". Check `df -h /tmp` when scores look absurd.
Near misses (agentxC/best):
- Skin_vtx1 (16 words): W struct layout and Sz blend right. Left: (1) the count read: target `lbu 1(s1); addiu s1,2`
  without the pData+2 giv (ours reduces it, see the giv rule; `pData++; n = *pData++;` avoids the giv but gives two
  addiu); (2) stsxy3 operand regs: &W.Sxy[0] is the bare virtual-stack-vars reg, so instantiate_virtual_regs emits its
  `addiu sp,16` after the other two operands (target computes it first).
- Sa_joint_blend (dscore 37, agentJ ~90): both inner loops as goto loops fix the GTE loop (one pS register, no second
  giv); `Inv = 0xFFF - Rate` before the inner loop. Left: first loop (target: pS biv with 0/2/4 and pPos at Pos_z with
  -8/-4/0, i = n - 1 decremented from its own register, not folded to n - 2 by cse) and t-reg numbering.
- Esp_trans_main (dscore 56, first draft): RE2 TRANS.C shape with address-taken D0/D1/D2 scratch words, range tests
  as separate ifs on (SHORT)D1 / (D1 >> 16), `D1 = (LONG)pP` before the D0 >> 9 test, `*(ULONG *)D1` tag store. Left:
  Def_size stays in a2 in the target (ours copies it to t1 because a loop local takes a2), pP t1 vs t0.
- Esp_trans_3d (63, agentJ 70): products into user variables Px/Py move them from LO/s2 to v1/v0 like the target;
  `Def_size--` in place. Left: X/Y registers and the uv block register choice.
- Skin_vtx2 (dscore 175): structure complete; target has higher pressure (pSxy/pSz spilled to the home slots,
  &Skin_nor[1..5] rematerialised into s4); none of pN[k] / Skin_nor[k] / six pointer locals reproduces it.
- Not attempted this round: J_seq, Rain_move, Sca_hit_all, Pl_floor_ck, Sa_morph_step (C, ~200 insns), SortGT3_col
  (agentJ draft unchanged), Skin_vtx2_alt.
## Matching round agentP (drafts in the session scratchpad agentP/best, subagent drafts agentP/sub{A,B,C,D}/best)
Matched: pc_sys Pc_box_set, select+jill_sel Jill_wep_load (shared in src/ovl/common/jill_sel; select's Select_wep_cd
renamed Jill_wep_cd), diedemo Die_spr_move_main + Die_blood_move_main, game Game_loop (G 0x785A named Unk785A, USHORT)
and Unk785A), mojidisp Branch_disp + mess_trans (Branch_disp prototype now takes MOJIDISP *), status St_item_use +
St_mix_exec (Mix_rate_tbl 0x800A0BF4 in syms/game.txt). Diedemo/Game_loop/mojidisp/status drafts were written by
subagents from RE2 DIEDEMO.C/GAME.C/MOJIDISP.C/STATUS.C.
Rules learnt:
- A load the target issues earlier than ours across a block move (`movstrsi`, the lwl/lwr copy of a local initialised
  aggregate) or across another independent chain is a sched1 decision; a `do { x = load; } while (0);` puts loop notes
  around it and sched1 does not move insns across them (Swap_Cbuff pRoom load; then a do/while elsewhere to restore the
  s-reg priorities it disturbed).
- A copy `move sN,vX` in the delay slot of the branch that tests vX: the variable is assigned in both arms of an if/else
  (`if (y < 0) { h = -y; ... } else { h = y; ... }`) and reorg pulled the else copy into the slot (Swap_Cbuff abs);
  or the loop counter is set inside `if (n) { i = n; do ... while (--i); }`.
- `G.Unk246C >= 0` gives bltz with no RTL to spare; `!(G.Unk246C & 0x80000000)` gives the same bltz after combine but
  one more RTL insn in loop.c's count, which is what stopped the 0xFFFFFF constant from being hoisted in Swap_Cbuff
  (threshold 29 with a call: 28 non-fixed regs + 1, minus 3 per moved invariant).
- global.c expand_preferences: a pseudo that dies in `dst = src + x` inherits dst's hard-reg preferences unless the two
  conflict (Get_light_cut: the pData copy took a1 because the zone pointer is passed in a1). local-alloc's
  optimize_reg_copy_1 rewrites later uses of the copy source to the copy within a block, so "use the original after the
  copy" does not create the conflict either.
- `pData = (UCHAR *)(pLight + G.Cut_no + 1); Zone = *(ULONG *)pData;` gives `addu v0,base,idx` (base first); an array
  index `pLight[G.Cut_no + 1]` gives `addu v0,idx,base`.
- Match from a matched sibling: Jill_wep_load is pl_wep.c Pl_weapon_ch with the work pointer as `pTop` (the parameter)
  and `pW = pTop` copied before the loop; `pW = pTop; pTop = &pEm->pSin_parts_ptr[4]; pTop->pTmd = pW->pTmd;` keeps
  the copy alive (one variable reused for two pointers).
- Mask constants kept in s-registers across a loop came from local variables (Game_loop Clr0/Clr1/Clr2); a plain
  constant lets cse fold `(x & C) & 1`.
- jump2 does not cross-jump jumps to a label it created itself (uid >= max_uid): mess_trans's EB/EF tails merge only
  when the alt-page cases `goto` the original glyph label (base-page cases go through `mode:` which falls into it).
- Spilled-local stack slots follow declaration order (Die_blood_move_main `SHORT Pct, Col, Row, Grp`; mess_trans pKeep,
  Mode, pEnd). `v *= 2` on a SHORT field gives lh, `<<= 1` lhu.
- A case-1 body wrapped in do/while(0) (St_item_use) and everything after a loop wrapped (Branch_disp) fixed s-reg swaps;
  `if (c < 0) { if (c >= -2) { ...; goto next; } } ...; next:` stops gcc folding two compares into an unsigned range test.
Proven hand-written (left as INCLUDE_ASM): mem_card _card_info/_card_load (BIOS A0 stubs `li t2,0xA0; jr t2; li t1,n`),
_card_clear (`jr ra; nop` epilogue with the sp restore before it: older compiler, libcard).
Near misses (agentP/best):
- sub Get_light_cut (3, Get_light_cut_3.c): `pData = (UCHAR *)(pLight + G.Cut_no + 1)` fixed the addu order. Left: the
  pData copy gets a1 (expand_preferences from the zone pointer passed in a1) because the Ofs load from the original
  pointer comes before the copy; putting it after the copy is undone by optimize_reg_copy_1 / cse.
- sub Retry_file_load (dw17 form, ~5 real words): `do { Flg = ...; Bit = Flg & 0x4000000; } while (0);` gives the
  target's order (Bit computed first); left a0/a1 and constant-register choices in the All_a/All_b compares.
- snd Snd_bgm_play_ck (2): unchanged; the CD lui must overlap Snd_work's (v1) and still issue after `addiu s0`.
- main Swap_Cbuff (2, Swap_Cbuff_2.c; first attempt, needs main.c to include movie.h and player.h): whole function right
  except sched2 issuing the pRoom lw before the rodata lui of the RECT copy (the do/while barrier puts it first).
- epilog Epilog_draw (7): needs the Epilog_work hi before G's hi/lo and its lo after them in sched1 order.
- pc_sys Pc_text_set (5): loop body scheduling (biv increment first in the target).
- option Config_moji_disp (3, Config_moji_disp_3.c, was ~80): search loop `while (!Config_moji_cmp(pStr, *pTbl)) {
  pTbl++; i++; }`, Opt.Kido direct, `X += space[2i+1]` before the y0 store, do/while on the pTbl block and on `pStr +=
  2`. Left: the colour branches; the target cross-jumps only the g0 store of the 0x80 path into the 0x58 path. Still
  blocks Config_suuji (shared "\x81\x40" literal).
- cd Cd_link_search (31 ds): loop.c hoists the 0xFFFF compare; with 28 non-fixed regs the call-free threshold is 58,
  three earlier movables bring it to 49, our loop has 41 insns, so the target loop must have 50-52 RTL insns.
- status St_box_draw (5, agentP/subC/best): v0/v1 swap in the box-list St_num_disp arguments; needs
  `St_box_list_draw(ST_WORK *, signed char Box_cursor, SHORT)` and two count-position tables (0x8009F518, 0x8009F548).
- mojidisp Print_main (ds 217, agentP/subB/best): rough; needs a MOJIDISP print buffer field at 0x75F8 and the digit
  table declared in a header. Mess_disp not attempted. status St_box_frame_draw, St_map_area_select unchanged.

## Matching round agentO (drafts in the session scratchpad agentO/best, agentO/em33, agentO/em3436, agentO/esp, agentO/em2238)
Matched: em_sub Goto00, Goto01, Esp_17; em33 Em33_mv04, Em33_mv07, Em33_mv09; em36 Em36_mv12; em3e Em3e_fly.
Rules learnt:
- Goto00/Goto01 (raw `move v1,a2` + in-place `sll/sra a2`): `USHORT Turn = Add_dir;` plus a separate
  `LONG Sign = (SHORT)Add_dir;` for the sign test, the error in 16 bits
  (`(USHORT)(Turn + (Tgt - pEm->Cdir_y)) & 0xFFF`) and `Turn + Turn` for the last add: with USHORT operands and a
  16-bit result gcc drops every zero-extension. Goto01 needs `USHORT Tgt`, Goto00 `ULONG Tgt`.
- Em3e_fly: a range test written with its bounds in locals first (`Hi = X + 1800; Lo = X - 1800;
  if (Pl_work.Pos_x < Hi && Lo < Pl_work.Pos_x)`) fixed both the register choice and the unfilled `beqz; nop` of
  the `if (Shot_wait) Shot_wait--; else ...` above it, which no structural change (do/while, switch, goto,
  return) had touched. Try temps for sub-expressions before blaming reorg.
- reorg.c fill_slots_from_thread (~line 3690): when the thread it scans has a reg-reg copy (`move t0,a0`), it
  rewrites the next insn to read the copy's source. A parameter living in a t-reg can therefore be read as its
  a-reg by the insn after its entry copy (Esp_kill: `srl a2,a0,24` while Id is in t0).
- global.c find_reg pass 0 skips registers that a conflicting allocno prefers. Non-copy preferences come from
  `(set X (op Y ..))` when X is already in a hard reg (Y prefers it), and are merged along `(set X (f Y))` where Y
  dies. In Esp_kill the `Id & 0xFF00` temp in v1 gives Id, and the hoisted `(USHORT)Id`/`(UCHAR)Id` invariants,
  a v1 preference, which keeps the loop counter out of v1. To see this: the FSF -g cc1 under gdb, `break find_reg`,
  print hard_reg_conflicts / regs_someone_prefers / hard_reg_preferences per allocno. Only valid where the
  function has no global address (FSF cc1 emits `la`, so no %hi pseudo).
- A `move v1,a0; jr ra; move v0,v1` tail (Em22_dir_quarter, also em20 Get_target_pl) is a copy into an older
  variable at the end of each arm (`Dir = Ret;`) with `return Dir;` after the join: combine cannot merge across
  the join and jump2 cross-jumps the identical copies.
- (em33) When the only diffs are the order of independent stores/loads, a hill-climb over statement order
  (agentO/em33/hill.py, hill2.py) converges in a few rounds. A load the target keeps below a run of stores:
  start a `do { } while (0);` at the load's statement (the loop note stops sched1 hoisting it).
- (em36) Em36_mv12 matched by reusing `Side` for the first hit test (`Side = Hit_area_ck(..); Hit = Side | ..`).
  score.py scores implicit declarations: a 0 with an old `func_` name still fails to link.
- (em_sub Esp_17) local SHORT tables declared inside the block that uses them (at the top they are initialised
  at entry); an empty `case 7: break;` for the extra `slti 9`; HIT_AREA fields in the order pPos, pBox, Dir, Id;
  the case-1 vib `if` must not get a do/while (the jump target would be "outside the loop" and reorg predicts it
  not taken).
- Kage_work9_sort is C: its t-reg saves around jals are caller-saves spills.
Unsolved pattern (three functions): load X, `move C,X`, then `addiu X,X,off` or `addiu C,C,off`, where cse should
have tied C to X (Em27_Init pSin_parts_ptr + 0x40, Em34_Init pUnk174 + 0x18, Spd_get_sub's pSeq_t/pKan copies in
t3/t5). Every assignment/order/`++`/re-read form tried folds the copy.
Near misses (agentO/best/<func>.c):
- em22 Em22_dir_quarter (1 word): `Dir = Ret;` at the end of both arms gives the double move; arm 1's
  `Ret &= 1; Dir = Ret;` is combined into `andi v1,a0,1` (target: andi into a0 then the shared copy). A label
  between them or Ret living past the copy would stop combine; no natural form found.
- em_sub Esp_call (2 words, esp fork): sched puts `lui/addiu Esp_data` before `srl fp,Id,8`; draft still uses
  `*(ULONG *)&pEw->Id`-style word views (clean those up before committing).
- em34 Em34_Init (5): the pUnk174 copy above; loop `addu v1,s3,v1` operand order.
- em27 Em27_Init (7 dscore): the pSin_parts_ptr copy above.
- em34 Em34_shoot (12): Unk0A halving (lhu + sll/sra sharing one sll); 0x101 in a0 vs v0 in case 6.
- em36 Em36_Init (13): block-scoped `PARTS_W *pB` per part block, `pG = &G` only for System_flg/pEnemy/ppEnemy_end,
  `SHORT Life`, an unused 8-byte local. Left: loop addu order, Pldamage/Pldie store placement.
- em_sub Spd_get_sub (dscore 35): params end up in a-regs, target has them in t3/t5 and the advanced pointers in
  a2/a1. Ternary for the R_flg offset is worse; `(SHORT)pSeq[2] + byte` operand order is better.
- em_sub Esp_kill (42): structure right; `Key = Id` copy at the top emulates Id living in t0 (reorg rewrite above);
  left: Kind/i/compare registers (preference chain above).
- em_sub Rot_neck (61, first attempt, from RE2 EM_SUB.C): Free_dif SHORT/Free_dif12 as in Rot_neck_em, z clamp on
  Neck_rot.vz directly, pFree for N_rot_z set after the clamp; left: the prologue load order (Own_neck_no),
  N_rot - Spd_lim computed late, the Unk166 update reassociation, Unk164 register.
- em_sub Kage_work9_sort (103): loop.c hoists &M2, %hi(G) and the jump table out of the inner loop; target does not.
- em23 Em23_mv03/Em23_mv0b, em22 Em22_mv_jump, em38 Em38_sce03: worked by a separate fork (drafts agentO/em2238); see its commits.
## Matching round agentM (drafts in the session scratchpad agentM/, forks in agentM/fork{A..G})
Matched: om Om_parts_link, Om_explosion, Om_rtn_fall; sce2 Chaser_item_set, Get_matrix; sce1 Cut_be_set; oba Oba_tmp_sca_set;
pl_wep Weapon_at_ck, Pos_em_at_ck; player Player_set; oba Oba_em_hit_ck. Type fix: Pl_wep_poly_ck(PL_WEP_POLY *, ENEMY_WORK *).
Rules learnt:
- flow.c counts REG_N_REFS weighted by loop depth (`+= loop_depth`), and `do { } while (0)` creates a loop level, so wrapping
  (or nesting) a statement in do/while(0) raises the refs of every pseudo it touches. That is the knob for BOTH local-alloc
  (qty priority floor_log2(refs)*refs/(death-birth), block_alloc local-alloc.c:1668) and global-alloc (allocno priority).
  Find the pseudo that must win with gdb on the FSF cc1 when it reproduces our allocation (`break local-alloc.c:1668`, print
  qty_order/qty_n_refs/qty_birth/qty_death; global.c find_reg prints regs_someone_prefers/regs_used_so_far), then sweep
  wrap depths per statement (itertools, depth 0..3, small sum). Oba_tmp_sca_set: `Lo = Fl & 0xFF` at depth 2 puts Fl's qty
  above the shift chain; Cut_be_set: the pCmd reload at depth 3 puts pCmd above Cut; Player_set: one do/while around
  `Cd_top = ...; pTbl = Work + *Work;` puts Work above pEm (s3/s4) and Cd_top above &G (s5/s6).
- Loop notes are scheduling barriers (sched_analyze makes the insn after a loop note depend on everything before), so wrap
  statement groups that may interleave in one do/while, or wrap statements next to calls.
- update_equiv_regs (local-alloc.c) doubles REG_LIVE_LENGTH of a pseudo set once to a REG_EQUIV constant (`p = &G;`,
  `Cd_top = (UCHAR *)0x801D5C00;`, a parameter copy with a stack-slot REG_EQUIV), halving its global-alloc priority.
- global.c find_reg pass 0 never takes a hard reg that a lower-priority conflicting allocno prefers (regs_someone_prefers),
  and only takes regs already "used" (call-used or used by local-alloc). A variable that takes a1 where the target has a2
  needs a1 blocked by a conflict or a preference (Spl_wep_set, pl_mv07 both stuck on this).
- sched1 can not move a pseudo set across a call unless the pseudo crosses a call anyway (sched_analyze adds a dependence
  on last_function_call for REG_N_CALLS_CROSSED == 0). A `lui %hi(G)` before a jal means &G was computed before the call in
  the RTL: `pG = &G;` as the first statement (Player_set).
- Reading a field into a local before stores (RE2's `kan_ofs = pTbl->Kan_ofs;` ahead of the Clut/Tpage stores) is what
  orders a load above stores: gcc 2.8 has no type-based alias analysis, so a load never moves above a store to another struct.
- When a tail must stay duplicated in two branches, write it in both: reload's round-robin spill registers make the copies
  differ and cross-jumping then only merges the identical part (fork C, Weapon_at_ck / Pos_em_at_ck).
- `pP = &...pParts[k]; return &pP->Workm;` with pP set in two blocks makes pP global, so local-alloc cannot tie the add
  result to the loaded base (Get_matrix's v0/v1 swap). Get_matrix kinds 5 and >= 7 fall off the end (as RE2).
- Om_parts_link: the else branch must store pSuper before pOya_parts (decides loop.c's giv base, +0xA4 not +0x6C); increments
  `pCur++; Part_no++; pTmd_cur++;`.
- Chaser_item_set: early exit as a full `{ pSce->Data += 3; return 1; }` (cross-jumped), USHORT room locals for lhu,
  `goto` inside the Nemesis scan.
- set_preference (global.c) also takes the FIRST operand of an expression source: `(set X (op R ...))` gives R a
  preference for X's hard reg when X was allocated by local-alloc -- a way to plant an a1/a2 preference (untested).
- To stop loop.c hoisting a small invariant: lifetime 2 and a larger loop insn count (narrow SHORT/USHORT locals add insns
  that combine deletes only later). -dL prints "Insn N: regno R (life L), savings S moved / not desirable"; threshold starts
  at 1 + non-fixed regs (29 with a call in the loop) and drops by 3 per moved insn; hoisted when thr*savings*life >= insns.
- A subtraction stays `subu` + `beqz` only if computed in the block before the first `||` test (Oba_em_hit_ck Diff_z).
- Oba_em_hit_ck: Oba_push_obj_ck gets the spilled walker pE (shortens pEnt's life); declaration order sets spill slot order.
- Weapon_at_ck/Pos_em_at_ck (fork C): declaration order sets reload spill-slot order; `(a & 0xFFFC) | 3` stops cse folding
  `(a|1)|3`; a `Mode = Attr & 0x20; f(.., Mode != 0)` variable keeps andi+sltu in the loop.
Near misses (paths under the agentM scratchpad):
- pl_wep Pl_wep_eff_move (3 words, forkF/best/Pl_wep_eff_move_3.c, needs md_sub.h and the PL_WEP_EFF_FREE view at Unk74):
  the preheader `addiu s6,sp,0x68` (&Req hoisted by loop.c) must come before `pWp = Pl_wep_poly`; sched1 LUID tie.
  Wants names for D_8009DD9C / D_8009DDB0 (HIT_ATTR blasts) and D_8009DBF4 (Parts_near_get list).
- pl_wep Pl_wep_parts_drop (2, forkF/best/Pl_wep_parts_drop_2.c): sched1 priority-1 class-3 tie between the multi-set
  `pPos = PW_M(pP)` and `a0 = CDIR`, LUID puts the pPos set first; single-set pPos makes loop.c merge the givs.
- pl_wep Mag_down (12, forkF/best/Mag_down_12.c): Timer3 must load before the Timer0 store (`andi a0,a0,2` in place).
- player pl_mv07 (4 real, forkD/best.c; needs spl_auto.h): loop counter i takes a1 in pass 0 (no a1 preference anywhere),
  target a2 with the hoisted m+8 in a1.
- spl Spl_wep_set (12, sw/b3.c): pP takes a1 in pass 0, target gives the pHand copy a1; every pHand statement also touches
  pP, so loop weighting raises both.
- oba Oba_em_overlap_ck (11, best/Oba_em_overlap_ck_11.c): W*2 hoist fixed; Shape/Z t1/t0 and X0/X1 v1/a0 swaps left
  (do/while fixes each but rotates s-regs / moves loads).
- oba Oba_ent_box_set (dscore 45, best/Oba_ent_box_set_45.c): W/D s4/s7 swap, Z delta load order, spill-slot frame layout.
- sce_at Sce_at_check (dscore 96, forkG/sa/v/both.c; fork B best/Sce_at_check_draft.c): target hoists %hi(D_800DECAC)
  (s7) and 0x10 (fp) and not the jump table; ours (298-insn loop, threshold ~23) rejects both (207 / 92 < 298), so their
  pseudos need a longer summed life (cse sharing across the UNDER/FRONT blocks?).
- om.c keeps private copies of struct tagSCE_AOT and PW_FREE (sce_at.c / joint.c have the same); sce0.c's SCE_AOT view
  differs (signed nFloor, Data[4], pParts), so they were not unified.
- em38 Em38_sce03 (dscore 3 / 7 words, agentO/em2238/best): left a3-before-a2 arg setup at the first Esp_call of case 5
  and the `Timer0 |= 0x8000` / `li -800` order in case 6. 210 -> 3 came from local roles: pP/pS/pW split by region,
  separate once-assigned pM2/pM3/pM4/pM in the kill block, a separate counter `n` for the size loops, `i = 22` first.
- em22 Em22_mv_jump (15): registers only (&pPl->pSin_parts_ptr[1] and the water-tail parts pointer want a0).
- em23 Em23_mv0b (35): the target stores Jump_x/Jump_z (and Move_cnt) and reloads them in the same block; ours keeps
  the register. Not reproduced (`+=`, free-work pointer, SHORT/LONG temps, volatile).
- em23 Em23_mv03 (89): `do {} while (--N)` frame loops, empty `case 13:`, Cdir stores per Side branch; left Wall_dir
  copy, `Pos_y + 1600` folded, the reload issue above.
- Tools: agentO/cdgen.py (greedy one-slot-at-a-time over `{{NAME:a||b}}` templates; gen.py's full cartesian product
  once wrote 476k files), opd.py (register-blind diff), dscore.py now returns 9999 on any cc1 stderr.
## Matching round agentU (drafts in the session scratchpad agentU/best)
Matched: pl_wep Pl_wep_parts_drop, Pl_wep_eff_move (names: Pl_wep_blast_at 0x8009DD9C, Pl_wep_burn_at 0x8009DDB0,
Pl_wep_near_parts 0x8009DBF4; pl_wep.c now includes md_sub.h).
Rules learnt:
- sched1 "birthing" boost (sched.c adjust_priority / birthing_insn_p): an insn whose dest pseudo has REG_N_SETS == 1 gets
  priority 0x7f000001 when it becomes ready, so it is scheduled right before its first use; a multi-set variable is ordered
  by LUID instead. Target `a0 = arg` before `s0 = ptr` (Pl_wep_parts_drop rot blocks) = single-set pointer; a load the target
  issues early (Swap_Cbuff pRoom) = multi-set destination.
- loop.c giv reduction subtracts add_cost * biv_count from the benefit: a biv incremented in two places (`pP++; continue;`
  in an early exit plus `pP++` at the end) halves what a giv is worth. Pl_wep_parts_drop's two single-set M pointers stopped
  being combined/strength-reduced that way.
- Preheader order: loop.c inserts hoisted movables before NOTE_INSN_LOOP_BEG, i.e. after any user init before the loop, and
  giv inits after the movables. Target `addiu s6,sp,&Req` before `lui Pl_wep_poly`: the pointer was a reduced giv
  (`&Pl_wep_poly[n]` with a counter n, n++ in both paths), not a `pWp = Pl_wep_poly; pWp++` biv.
- global.c prune_preferences drops call-used regs from the preferences of allocnos that cross calls, so a parameter's
  incoming a1 never blocks a1 for others (pl_mv07, Spl_wep_set); regs_someone_prefers only collects LOWER-priority
  conflicting allocnos' preferences.
Near misses (agentU/best):
- player pl_mv07 (pl_mv07_10_ofs.c, 10 words; agentM forkD 4): a fresh single-use `Ofs` for the `<< 3` term puts it in v0 like
  the target. Left: i (loop counter, prio 1.08) takes a1 before the hoisted Ofs+8 (0.18), target i=a2 / Ofs+8=a1; and with Ofs
  split off, m/m4 swap t1/t2. FSF cc1 reproduces the a1/a2 choice (gdb usable). Loop reversal / count-up forms worse.
- pl_wep Mag_down (12): Mag_down_12_regs.c (`Old_pos = (USHORT)pP->Timer3; Old_pos &= 2;`) has every register right but
  sched1 puts the multi-set andi early; Mag_down_15_sched.c (`if ((Old_pos & 2) ...`, single-set andi temp) gets the
  Timer3 load order but the andi temp takes v0 and the Timer0 chain swaps v0/v1. Target needs a boosted andi placed between
  negu and the Timer0 sh. ~1500 statement-order / temp / type variants (mdv5..mdv8) all >= 12.
- main Swap_Cbuff (2): agentP's `do { pRoom = pG->pRoom; } while (0)` leaves loop notes that block sched2 from putting the
  rodata lui first. Without it, the pRoom load is boosted (single set) and lands after the RECT copy (13). Reading it into
  the multi-set pScrl (Swap_Cbuff_2_pscrl.c) gives the exact order, but pScrl's s6 then holds pRoom too (2). Needs some other
  multi-set variable for pRoom (y works but is not credible).
- snd Snd_bgm_play_ck (2, Snd_bgm_play_ck_2.c = plain `p = &Snd_work; pCd = &CD;` + empty do/while): order right, the CD
  %hi gets v0 instead of v1. Local-alloc only gives v1 if the two %hi lives overlap in sched1 order, and then sched2 issues
  both luis first (Snd_bgm_play_ck_regs.c). sched2 picks hiC before loS only with a higher LUID.
- spl Spl_wep_set (12): `pHand = pP` inside the Flg & 8 block makes pHand block-local (a0) and moves pEm (85). With it at the
  top, pP (prio 0.92) beats pHand (0.18) to a1; do/while weighting raises both because every pHand statement touches pP.
- oba Oba_em_overlap_ck (11): statement order of the X/Z/X0..Z1 reads has no effect (288 perms); dwgen depth <= 3 no effect.
- cd Cd_link_search, oba Oba_ent_box_set, sce_at Sce_at_check: not reached this round (agentK/agentP/agentM drafts unchanged).
## Matching round agentR (drafts in the session scratchpad agentR/best; tools agentR/*.py, *.sh)
Matched: none of the eight sca functions (Sca_high_hishi, Sca_hit_box, Sca_hit_hishi, Sca_hit_koban_x/z, Sca_hit_line,
Sca_hit_slope, Sca_line_circle). Three drafts improved; the rest are where earlier rounds left them.
Tools (agentR): hc.py PERMDIR DRAFT [iters] [maxdepth] hill-climbs do/while(0) wrap depths per statement line plus
`{{a||b||c}}` choices in the draft; ch.py runs every choice combination; ex.py PERMDIR DRAFT K D tries every set of
<= K wrapped lines up to depth D; perm.py writes every dependency-respecting order of a `//P{ ... //P}` block
(`//@name<dep`); psc.py scores files in parallel. lq.sh PERMDIR DRAFT [block] prints local-alloc's qty order (qty
refs/birth/death and member pseudos), and ga.sh prints global-alloc's allocno order with preferences, conflicts and the
hard reg each one gets. Both run the FSF -g cc1 under gdb, so they are exact only for functions without global
addresses (Sca_high_hishi, Sca_line_circle); for the others use prio.sh (SN .lreg/.greg).
Rules learnt:
- local-alloc only takes a pseudo that lives in one block and has exactly one death (REG_N_DEATHS == 1,
  local-alloc.c:472). A variable that is set twice with the first value dying in between (`Half = X1 >> 1;
  Half = (Half - X0) >> 1;`) dies in 2 places and goes to global-alloc, after every block-local qty.
- global.c find_reg (line 219) can take a hard reg away from local-alloc pseudos when their summed refs/length is lower
  than the allocno's. The pseudo loses its reg. A parameter with a stack-slot REG_EQUIV then lives in its home slot
  (Sca_line_circle P1: `sw a1,0x4C(sp)` / `lw a1,0x4C(sp)`). So an s-reg that local-alloc gave to such a parameter can
  end up with a low-priority global (Z1 in s4).
- A variable reused for the second call argument (`Dz = ((X0 - Dz) << 16) | ...`) puts the result in that variable's
  register and adds refs to its qty. In Sca_line_circle this moved Dz to the top of local-alloc (51 -> 43).
- The order of independent stores in a case changes live lengths enough to reorder global-alloc. Koban: writing
  `u.pC->Z0 = Z0;` last in case 3 (after the Sca_work stores) made Z0 live long enough to drop below D. Searching the
  orders with perm.py and then wraps with hc.py got Sca_hit_koban_x from 22 to 12.
Near misses:
- sca Sca_hit_koban_x (12 words, best/Sca_hit_koban_x_12.c): global-alloc order is now right. It needs `LONG Half`,
  case-3 order X1, Z1, X0 (wrapped), Sca_work.Z, Sca_work.X, Z0, and wraps on `Cx0 = X0 + Half`, `u.pW = &Sca_work` and
  the Sca_hit_box return. Two things are left: (1) the block-0 local temps. In the target, the X1 load temp gets v1
  (and Cx1 takes v1 through its preference) and the Z1-Z0 temp gets a0. Ours is the other way round, so local-alloc's
  qty priority of the D temp must be lower. Moving the D statement or changing how Half is computed made no
  difference (cse merges them). (2) sched2 order of the case-3 stores (sh X0 before sh Z1; sh Z0 before the Sca_work
  sw). The natural store order (X1, X0, Z1, Z0, X, Z) gives the target schedule but flips D/Z0 back.
- sca Sca_hit_koban_z (14, best/Sca_hit_koban_z_14.c): the koban_x draft mirrored (the X read/store statements keep
  their koban_x order). Same two leftovers.
- sca Sca_line_circle (43, best/Sca_line_circle_43.c; was 51): structure, P1 spill and Dz/Z0 s0/s1 right. Left: X0
  (`X0 = pT->X0 >> 1; ... X0 += Half;`) is a local qty (s2) in ours. In the target it is global (s6, after Half s5),
  so in the original it must die in 2 places or span blocks. SHORT X0, `X0 /= 2`, separate Cx, and reusing X0 in part
  2 all fail (part 2 keeps that value in v1, so X0 cannot be one global pseudo there).
- sca Sca_high_hishi (agentI 13_dw.c is still the best score; best/Sca_high_hishi_20_zonehalf.c has the cleaner
  allocation). With `Zone` reused for the X half width, ga.sh shows Dx a2, Cz t0, X a1, Cx a0 right. Left: global
  order X0 > Z1 > Z > Z0 > Dz (refs/len now X0 6/39, Z1 5/30, Z 4/28, Z0 5/32, Dz 4/29). No wrap set of up to 3 lines
  (depth 1) or 2 lines (depth 2) does it (ex.py), and X0 must stay below X/Cx or it takes a1/a0 in pass 0.
- sca Sca_hit_line (13, agentI draft): unchanged. Three separate sched issues. (1) (SHORT)P3/(SHORT)P2 order: the
  regs need P2's qty born first in sched1 order (P3 qty then gets v1), but sched2 must put P3 first. (2) The
  `lw Opz[0]` belongs between `mflo` (Nz*R) and the Nz div; the do/while that fixes the s-regs starts at `Nz /= Len`,
  and its loop note keeps the load below the div. Every other start point for the do/while breaks the s-reg
  priorities (117+). (3) The xor operand registers.
- sca Sca_hit_box, Sca_hit_slope (agentG/agentL drafts, ~187 / ~180 words), Sca_hit_hishi (559 insns, leaf using
  s0-s4, /18 via 0x38E38E39 with the `sll 16` values kept for the sign fix): not attempted beyond reading;
  Sca_hit_slope's rotated-corner copy `move v1,a3` reads like offset temps (A = (D, D), B = (W + D, D - W),
  C = (W, -W)) added to X0/Z0 afterwards, with Cx/Cz spilled to 0x10/0x14(sp).
## Matching round agentQ (drafts in the session scratchpad agentQ/best and agentQ/v*)
Matched: em34 Em34_Init, em27 Em27_Init, em_sub Spd_get_sub (the three "load, `move` copy, add to one of them"
functions), em23 Em23_mv0b.
Rules learnt:
- The surviving `lw X; ...; move Y,X; addiu X,X,off` copy is NOT a cse copy: it is reload_cse_regs (reload1.c,
  runs after reload, before sched2) turning a second load of the same field into a register move, because the
  register still holds the value. So the source reads the field twice, with a store in between that makes cse
  (which is conservative: a store to pEm+316 invalidates pEm+264) drop the first value. Em34_Init: `new5 = pEm->pUnk174;
  old5 = pP2->pTmd; new8 = &pEm->pUnk174[1];` (store to a global in between). Em27_Init: `pEm->Hit_area.pM =
  &pEm->pSin_parts_ptr->Workm; pP = pEm->pSin_parts_ptr;` (the pM store in between). The same mechanism gives
  Spd_get_sub's `move v0,t1` for `Spd_base_x += vx` after `Spd_base_x = x`.
- sched1 (sched.c, scheduled backwards) gives an insn that sets a register with REG_N_SETS == 1 the block's maximum
  priority as soon as it becomes ready (adjust_priority / birthing_insn_p): single-set loads sit right before their
  first use. A load the target issues early (or between other insns) is of a variable set more than once, or its
  first use is earlier. The scheduler also blocks a load for 1 cycle after a memory insn ("blocking insn N"): in
  Em27_Init the B load only lands before the `+64` when the first neck store is a zero store (no `li` in front of
  it), so the PW_NECK stores go in memory order (N_rot_y, N_rot_z, Add_dir_y, Add_dir_z, L_rot_y, L_rot_z, as RE2
  EM28.C writes Free[0..5]). Read the trace with SN cc1 -dS (";; ready list at T-n", "launching", "blocking").
- cse make_regs_eqv: a parameter copy `pTop = pSeq_t` survives (`move t3,a2` at entry, a2 advanced in place) when the
  parameter variable itself is used after pTop's last use: Spd_get_sub reuses pSeq_t in the second block
  (`pSeq_t = (ULONG *)(pTop + (pS[1] & 0xFFFE)); Frame = *(UCHAR *)pSeq_t;`), so pSeq_t stays canonical.
- A byte loaded into the register of the pointer it was read through (`lbu a2,0(a2)`) is a variable used in two
  blocks (global, gets the pointer's hard reg as preference from set_preference's `(mem (reg))` source).
- `addu idx,base` (index first) comes from an address written inline at each use (`*(ULONG *)(pTop + n * 8)`,
  expanded as a memory address, canonical (plus (mult) (reg))); a pointer local gives `addu base,idx`.
- An unconditional `Seq_w &= 0xFFFF;` before an if whose then-arm uses it lands in the branch delay slot
  (Spd_get_sub); written inside the arm, reorg fills the slot from before the branch instead.
- `Chaser_free` copy loops (Em34_Init, Em36_Init): `pEm->Unk1CC[n] = G.Chaser_free[n]` gives `addu v1,s3,v1`; the
  old `((ULONG *)pEm->Unk1CC)[n]` cast gave the swapped operands.
- Em23_mv0b: the target re-reads Jump_x / Step_x after storing them because the source stores the next field first
  (`Jump_x += ..; Jump_z += ..; Step_x = ..; Step_z = ..;`) and cse forgets same-base fields on any store; the
  `move v1,a2` before `Step_x *= 4` is again reload_cse; `<<= 2` (not `*= 4`) for the lhu form.
Near misses (agentQ/best):
- em22 Em22_dir_quarter (1 word, Em22_dir_quarter_1.c = agentO's): the target has `andi a0; j L; L: move v1,a0;
  jr ra; move v0,v1`. Copy in both arms: combine merges arm 1's `Ret &= 1; Dir = Ret` into `andi v1,a0,1`. Copy once
  at a shared label (e.g. arm 1 `goto Set`, Set before arm 2's copy): combine then merges `Dir = Ret` with the return
  copy `v0 = Dir` (same block). An 840-variant sweep (agentQ/d22/g: if/else order, copy placement, return var,
  Dir/Ret types ULONG/LONG/(U)SHORT/UCHAR) found no 0. Needs a label between the copy and the return and between the
  and and the copy at the same time.
- em34 Em34_shoot (2 words, Em34_shoot_2.c): `Spd = (USHORT)Unk0A << 16` with `Spd >> 16` / `Spd >> 17` (the shared
  sll), the compare result in a new local (Hit2). Left: in the `Joint_move(.., 0x10200)` block the target loads Flg and
  UnkE3 before `li a0,0x101; sw Rtn`; ours schedules both loads (single-set, birthing boost) after, so the constant
  gets v0. 24 statement orders, do/while wraps, a struct-typed Rtn store and multi-set temps did not change it.
- em36 Em36_Init (12, Em36_Init_12.c): loop fixed; left the Pldamage/Pldie store scheduling (target: the Pldamage
  address and Id are loaded early, the store sinks after the 0x1EA..0x1E6 stores; moving the statement after them
  gives 21) and pE/ppEm in a2/a1 instead of a0/a1.
- em23 Em23_mv03 (dscore 49, was 89; Em23_mv03_49.c): `LONG Y = Pos_y + 1600` temp (stops the reassociation),
  `Dir = (Side << 10) - 0x800` temp, direct `EF(pEm)->Side & n` reads for the 758 offsets (a multi-set Side local
  is loaded too early). Left: the Wall_dir step ternaries (target: one store after the join, a `move a0,v1` copy of
  Wall_dir for the <= 0x800 arm, `slti 3073; bnez +16`), case 4/5 constant registers.
- em22 Em22_mv_jump (dscore 7, was 15; Em22_mv_jump_7.c): one `pP` reused for `&pPl->pSin_parts_ptr[1]` in the grab
  block and `pEm->pSin_parts_ptr` in the water check (both a0 in the target), grab block statements reordered by a
  hill-climb (agentQ/hill.py). Left: the order of the pSub1_seq_t_ptr copy against `pP->UnkA2 = 0` and the pW loads
  (a 3000-order sweep of that block reaches 8 sc words).
- em38 Em38_sce03 (7 words, unchanged; agentO/em2238/best): case 5 first Esp_call wants `addiu a3,&Sv` before
  `addiu a2,&pW->Workm`, case 6 wants the Timer0 |= 0x8000 store before the `&pP->Workm` s0 set. Statement-order
  hill-climbs over both regions and pM temporaries found nothing.
- Tools: agentQ/hill.py (statement-order hill-climb, absolute perm dir), ds.sh (difflib score with macro expansion),
  adiff.py (difflib-aligned target/ours listing), rtl.py (compact RTL listing of one function from a -d dump).

## Matching round agentT (worktree clean/yD; drafts in the session scratchpad agentT/best)
Matched: em_sub Esp_call, Esp_kill; joint J_seq (first C version, from RE2 JOINT.C J_seq). psp.h: RAIN_WORK 0x12/0x16/0x18/0x1A
named Spd_y / Level (USHORT, lhu in Rain_move) / Spd_x / Spd_z.
Rules learnt:
- Sweep local and parameter types before anything else: agentT/tsweep.py DRAFT PERMDIR OUT [rounds] does a greedy
  coordinate descent over every `    TYPE name;` local (PARAMS=1 also the scalar parameters; DL=1 scores with difflib,
  better when an extra insn shifts everything). Esp_call's 2 sched words were `UCHAR Sub, Type` (srl stays at the top
  of the block, the andi moves to the use); Esp_kill went 42 -> 1 with `USHORT Key = Id` (the copy is what puts Id in
  t0 and keeps the loop counter in v1), the last word was the byte loop reading the `No` copy like its pM twin.
  For a parameter sweep replace the function's prototype in base.c with an unprototyped `void f();` first.
- mult operand order vs register choice: the FSF cc1 (~/dev/gcc281/build-psx/cc1) reproduces SN's allocation in
  functions whose only globals are loads; compare its output first. In J_seq FSF showed the registers were already
  right and only the `mult` operands were swapped; computing one operand into a fresh single-block temp
  (`Frame = Mot[2] + *pSeq; ... (Rec_sz >> 18) * Frame`) gives the target's (mult Rec Frame) without changing the
  local-alloc order (writing `R * X` inline swapped the registers instead).
- A cse'd address beats a pointer variable when the target keeps it in a scratch reg: J_seq reads
  `((USHORT *)((UCHAR *)Seq_t_ptr + Move_no * 8))[2]` and `[0]` inline (one t0 temp that later becomes pP_base's
  register); a `pMot` variable inherited Seq_t_ptr's a1.
- Unrolled 8-per-pass decoders: write the pass as a goto loop over `pP[0..7]` / `pRot[0..8]` with `pP += 8; pRot += 9`
  (target re-materialises the mask constants each pass); `if (--n == 0) goto end;` after each part.
- A goto loop whose body is wrapped in one `do { } while (0)` (label before the do) has no loop.c hoisting but loop
  depth 1 for its refs (J_seq hokan loop: fixed pP_base t0 / Dx a2 at once).
- An existing scalar reused for a pointer (J_seq `Dx = (LONG)&pP_base->Cdir_x; gte_stsv(Dx)`) moved it from v0 to the
  target's a2; reusing a variable that is live elsewhere is the cheap way to make a temp global.
- Door_init: the variable holding the TIM sum is a function-wide one that is reused later (the end-of-function `pAot`),
  and the `&Door_data->pTim` pointer lives in a variable that later carries Mapping_md's a2 argument (Clut): that is what
  puts them in a1/a2 (global-alloc copy preferences). Reading `pTmd` into a temp before the Ctp store fixes the load
  order; `pAot = *ppTim; pAot += Door_data;` (two statements) loads straight into a1.
Near misses (agentT/best/<func>.c):
- door Door_init (Door_init_3.c, 3 words; uses `Clut` for &Door_data->pTim and pAot for the sum, both reused, so it
  needs a cleaner variable story before committing): sched1 hoists the Mapping_tim argument copy `a0 = pAot` above the
  pPrepacket store, then local-alloc optimize_reg_copy_1 rewrites the pPrepacket and pTim stores to use a0. The target
  keeps both stores on a1 with `move a0,a1` after the first store. FSF cc1 reproduces ours (debuggable); do/while
  barriers around the stores (36 placements) and a pAdr copy variable did not help.
- door DoorSort (DoorSort_ds28.c, DL 28, agentL's draft with split decls): needs global-alloc priorities
  pOt? > pPacket > pVert > pPrim > pDw > n > I0 > I1 with I2/pNorm spilled. pVert has 7 refs (0.108) vs pPrim 9 (0.200);
  wrapping the two 0x400-branch pVert copies in do/while(0) gives pVert 0.208 (right) but lifts I1/I2 above pDw/n.
  Type sweep: nothing.
- em_sub Rot_neck (Rot_neck_68.c, 68 words, from 61 dscore): top order fixed by Own_neck_no/pOwn_parts first and the
  enemy target read inline (`*WORKM_POS_V(&pNeck_enemy->pSin_parts_ptr[Enemy_neck_no])`, pOwn_p after it), and the
  0x8000 test on a fresh LONG temp of Unk166 (andi on the loaded register). Left: own-offset sll chain is scheduled
  after the Neck_tgt block copy (target before), Vel/Free_dif12 a2/a0 swap in the y and z blends, Dif expression order.
- trans Esp_trans_3d (Esp_trans_3d_18.c, 18 words): u/v/u1/v2 of the uv block reuse Otz/D2/Px/Py (makes them global, so
  Def_size keeps a2 like the target); top statement order from a permutation sweep. Left: the Px/Py products now live in
  t0/a1 (target: mflo into v1/v0, i.e. block-local products), Otz sum/pOt index operand order.
- trans Esp_trans_main (Esp_trans_main_ds51.c, DL 51): Def_size must stay in a2; local-alloc gives a2 to a loop temp
  (u1) because the target's loop has more block-local values live there. Reuse/type sweeps did not reach it.
- em_sub Kage_work9_sort (Kage_work9_sort_ds89.c, DL 89 from 103 with `USHORT j`, `signed char Hx`): still the inner
  loop hoisting (a goto inner loop removes it but spills differently, 130).
- psp Rain_move (Rain_move_ds126.c, DL 126, first draft; needs sca.h, esp.h and `extern UCHAR D_8009DE2C` in psp.c):
  two 100-drop loops; the target spills pTop (0x24) and the respawn count (0x20), keeps 0xFFFF0000 in s7 (assigned before
  each loop) and 0x8000 in fp in loop 2 only. Our loops strength-reduce pV+4/pV+8 address givs (target: a copy of pV in
  a2 used for the loads, stores and Sca_get_ground). Loop 1 as a goto loop is closer; x+Dx / z+Dz are computed before
  the first range test.
## Matching round agentV (drafts in the session scratchpad agentV/best)
Matched: status St_box_draw (agentP subC draft + names Box_num_pos 0x8009F518 / Box_list_num_pos 0x8009F548,
St_box_list_draw now takes `signed char Box_cursor`, still matches), sub Retry_file_load.
Rules learnt:
- Type sweeps of the flag locals are cheap and worth doing first: Retry_file_load went 11 -> 0 with
  `SHORT All_a, All_b` (the narrow compare results change which registers the 0xF8000000 / 0x3C00000 masks get).
- A value whose andi/extension lands in a different register than its source (`andi v0,v1,0xffff` where ours
  ties both into one register) is a local that is not block-local: local-alloc ties a dying source to the set
  register only when both are local qtys. St_box_draw: the x / y count positions are one pair of variables used
  for all three St_num_disp calls (multi-block -> global-alloc). Pc_text_set: `Hi = c; c = (Hi / 32) * 10 << 8 |
  (Hi % 32) << 3;` (c set twice) gives the target's `addiu v0,v0,-32; andi v1,v0,0xffff`.
- The SN cc1 -dS dump already contains the sched1/sched2 verbose trace (";; ready list at T-n", per-insn
  priorities): no need for gdb to see scheduling decisions. Backward list scheduling: among equal priority, the
  higher LUID is chosen first (placed lower), so the insn that ends at the top of a block is the lowest-LUID free
  insn. A launched insn whose SET_DEST is live and REG_N_SETS == 1 gets LAUNCH_PRIORITY (birthing boost,
  adjust_priority) and is placed right above its consumer.
- loop.c movables: savings = n_times_used; identical constants in several blocks of the loop are matched
  (savings and lifetime summed), which is how a life-1 constant gets hoisted (St_box_frame_draw `pS->u0 = 0x20`
  written in each case of the switch, cross-jumped to one store later). The matched reg then has refs 1 + 2*uses,
  which raises its global-alloc priority.
- cse follows a conditional jump (-fcse-follow-jumps) only if the target label has LABEL_NUSES == 1 and is preceded
  by a BARRIER (cse.c cse_end_of_basic_block). When the target recomputes `&G` / reloads a field right after an
  `if (..) { ...; break; }`, cse did not follow into that block in the original.
- fdiff flags string literals of a `.rodata`-dotted segment as reloc mismatches (target uses the D_ label, the
  object .rodata+off); the full build (60 OK) is the real check there (Retry_file_load "ICON_DATA").
Near misses (agentV/best):
- status St_box_frame_draw (St_box_frame_draw_7.c, 7 words, was ~200): UCHAR Ot_no2 / signed char Ot_no3 make
  the `Ot * 8` hoist (savings 2), `c = pSt->Box_cursor` reused for the line y, `pI = &G.pItem_set->Box[i]`,
  `switch` with `case 1: case 2: / case 3: / default:` and `pS->u0 = 0x20` in every case (only way found to hoist
  0x20 after Item_data_tbl). Left: s4/s5/s6 = G / 0x20 / 0xD0 instead of G / 0xD0 / 0x20; the matched 0x20 reg has
  refs 7 (target behaves like a single-use hoisted constant, refs 3, prio ~0.034 between 0xD0 0.035 and
  Item_data_tbl 0.033). A single store is never hoisted (life 1, threshold 17 < 52 insns). Also has a
  `do { pL = ..; } while (0)` that fixes an i/pS s1/s2 swap.
- status St_map_area_select (St_map_area_select_11ds.c, dscore 11, was ~118): Rtn2++ before Snd_se_stad(5) (stops
  the cross-jump with case 0), `Key = pG->Key_trg & 0x2000; Pad = pG->Padd_trg & 1;` then `(.. & 0x108) || (Key |
  Pad)` (fold makes the `or`). Left: the target recomputes `&G` (lui in the beqz delay slot) and reloads Key_trg
  with lw in the next test; cse follows the jump in ours (see rule above). USHORT Key gets the reload (as lhu) but
  not the &G recompute. A do/while around the body content reproduces it but changes `break`'s meaning.
- sub Get_light_cut (Get_light_cut_3_regs.c, 3 words with all registers right; Get_light_cut_4_a0.c): the a0 copy
  needs pData not to die in `pLight = pData + ..` (global.c expand_preferences merges pLight's a1 preference) and
  the Ofs load to stay on s0 (local-alloc optimize_reg_copy_1 rewrites the source's last use after `copy = src`
  unless a jump/label/loop note intervenes). `do { pLight = ..; } while (0)` gives every register but leaves
  `addu s2` (pCut) after the loop pointer add; target has it before.
- pc_sys Pc_text_set (Pc_text_set_6.c, 6 words, was 7): only the `addiu t0,t0,16` biv increment, which the target
  issues first in the loop. The -dS trace shows it is a LUID tie-break (priority 1, lowest LUID ends on top), i.e.
  the target's biv increment insn precedes the loads in RTL while the store giv (a2, +15) is incremented after the
  stores. Every pSpr / index / pSpr[-1] form tried moves both or neither.
- epilog Epilog_draw (7, unchanged): the -dS trace shows G's lo_sum (pG, set once) gets the birthing boost and is
  placed right above the Flg_ck call; the target needs it above Epi_work's lo. Needs REG_N_SETS(pG) > 1 or a
  different pG source; a second `pG = &G;` is deleted by cse.
- option Config_moji_disp (3, unchanged): the target's 0x80 path stores r0 and then jumps to a separate copy of
  `j A4; sb g0` placed after the 0x58 block, i.e. the 0x80 and 0x58 paths are not cross-jumped beyond b0; no
  Col/constant/order/if-shape combination (800 variants) reproduces it. Config_suuji still waits for it.
- mojidisp Print_main, Mess_disp: not attempted (time).
## Matching round agentS (worktree clean/yC; drafts in the session scratchpad agentS/best)
Matched: 80079EC0 Skin_vtx1, Skin_vtx2, Skin_vtx2_alt; sabun Pl_floor_ck, Sca_hit_all.
Tool fix: tools/maspsx-swc2.patch (README step 4). maspsx treated the first operand of `swc2` (a cop2 register) as
a GPR and put a load-delay nop between `lbu $t9,..` and `swc2 $25,..`; ASPSX does not (Skin_vtx2 is the only place
in the game with that pair). The worktree uses a patched copy of tools/maspsx; the main checkout's tools/maspsx needs
`(cd tools/maspsx && patch -p1 < ../maspsx-swc2.patch)` or Skin_vtx2 fails there.
Type fixes: sca.h declares Sca_rtn_tbl; sabun.h Sca_hit_all takes `LONG R`; sabun.c includes sca.h, player.h,
enemy/em_common.h and declares Rot_vector without a prototype (the target passes the direction unconverted).
Rules learnt:
- loop.c's giv threshold can be met from the other side: Skin_vtx1's target does not reduce pData+1/+2 because its
  outer loop has >= 187 RTL insns (lifetime * 31 * benefit 6 = 186). SHORT conversions add insns that only combine
  removes (after loop.c): `x = (a + b) >> 1;` into a SHORT and `(SHORT)(..) << 16` cost 3 + 2 insns per vertex. -dL prints
  "giv of insn N not worth while, A vs B" to check the count.
- An asm operand that is a bare frame address (`&W.Sxy[0]` at frame offset 0 = virtual-stack-vars) is materialised
  by instantiate_virtual_regs right before the asm, after the other operands; a pointer local (`pW = W.Sxy;
  gte_stsxy3(pW, pW + 1, pW + 2)`) keeps source order (Skin_vtx1).
- Register pressure from types: Skin_vtx2's SHORT weights (w0..w2) and `&pN[k]` addresses for all six Skin_nor
  entries (instead of `&Skin_nor[k]`) gave the target's spills of pSxy/pSz to their home slots and the s4 reload
  register; 377 -> 0.
- `Cnt_top = Cnt = Ent & 0xFFFF; while (Cnt--)` keeps the post-decrement test on its own copy (`move v0,a3; move s2,a3;
  beqz v0`); with two statements cse rewrites the test to use the saved copy (Sa_morph_step).
- Sca_hit_all: a variable reused for two values (`Shape` holds the z move first, then Id & 0xF) shares the hard reg;
  declaration order of spilled scalars sets their slots (Area 0x10, pEnd 0x14, P1 0x18); two separate `if (..) return
  0;` (not `||`) make reload read the spilled pEm from its home slot; the rotated position is computed in two
  statements (`Cx = (Rx - Rz) * 181 >> 8; Cx += X0 + R;`); Dx and Dz are both computed before the first store.
- sched1 (sched.c rank_for_schedule) prefers, among equal priorities, an insn independent of the last scheduled one;
  a load with no user in the block (X in Sca_hit_all, Half in Sa_morph_step) is therefore placed right before the
  block end / call. A do/while(0) around it pins it (loop notes are barriers).
- Pl_floor_ck: `pPl = (ENEMY_WORK *)&G.Player;` (cse derives &G as pPl - 0x248C), Side computed in three statements
  (`Side = Dir & 0xFFF; Side >>= 10; Side ^= 2;`, s0), `if (A == 1 && B != 11 && B != 12) {..}` in both tails (the
  `return 0` chain folds into an sltiu range test) and the stores in the same order in both tails so jump2
  cross-jumps them.
Near misses (agentS/best):
- sabun Sa_joint_blend (Sa_joint_blend_32.c, dscore 32): the first loop's counter: target `addiu a2,t3,-1; move v0,a2;
  beqz v0; addiu a2,a2,-1` (i = n - 1, post-decrement not folded); every C form gets cse's associative fold
  (fold_rtx PLUS case, cse.c ~5710) to `addiu a2,t3,-2`. a do/while(0) note in between does not help (cse2 ignores
  notes); only a label between `i = n - 1` and the test stops it (L1 form, then the test is at the bottom).
  Sharing `i` with the second part's inner counter (both a2 in the target) fixed the a-register names.
  Second part: Inv (0xFFF - Rate) must be computed before the n - 1 test and take t0 (pSa's register).
- sabun Sa_morph_step (Sa_morph_step_51.c, dscore 51): structure complete (RE2 Sa_parts_allmv shape with j-indexed
  keys, `Stride` before `Out` so Out is the spilled one, chained Cnt_top). Left: the Size load / `Half = Size >> 1` is
  scheduled last in the block (see the sched rule) where the target has it fourth; that overlaps it with pKan and
  makes local-alloc put pKan in a1 and pP in t0 in the target. Half must also outrank Inv and k in global-alloc
  (target t7 Half, t8 Inv, t9 k, s0 pParts, s1 pFrm); a do/while around it makes things worse. Permutations of the
  11 statements before the first `if` (agentS/sms/pm1, 1500 orders) and type sweeps did not reach it.
- root_ck kara_rootck2 (kara_rootck2_4.c / kara_rootck2_alt_4.c, 2 real words; score against agentS/kr/p, a copy of
  p_kara_rootck2 whose prototype returns LONG): written from the asm (not RE2's algorithm): DFS over 0x30-byte nodes
  (pBlock + pBlock[1]), 8-byte points (pBlock[2]), cost bytes (pBlock[3]), 16 frames of 0x10 bytes. It returns LONG
  (0xFFFF0000 = no path, else Best << 16 | 1): root_ck.h must change from void (Root_ck ignores the value). Frame and
  all spill slots match (Wx/Wz separate SHORT locals, the point table indexed inline, `i` reused for Link * Mul,
  the point-to-R test via a temp). Left: the cost lookup `pCost + Cost_ofs + (UCHAR)i + Link[Next]` -- written with
  Link first every register is right but the adds are in the wrong order; with (UCHAR)i first the order is right but
  the (UCHAR)Next temp gets a1 (its life overlaps the a0 local-alloc gave the andi). ~300 forms, type and do/while
  sweeps tried.
## Matching round agentAA (worktree clean/zE; drafts in the session scratchpad agentAA/best)
Matched: main Swap_Cbuff (main.c now includes movie.h, player.h), pl_wep Mag_down, player pl_mv07 (player.c includes
spl_auto.h).
Rules learnt:
- combine keeps REG_N_SETS when it folds an in-place i2 (`p += 17; x = *p;` -> `x = 17[p]`, combine.c ~2421 skips the
  decrement when i2dest is in i2src). The pointer is then a two-set pseudo with one remaining set, so sched1 gives its
  load no birthing boost and leaves it in LUID order. Swap_Cbuff: `ppRoom = (ULONG **)pG->pRoom; ppRoom += 17; pScrl =
  *ppRoom;` keeps the pRoom load above the RECT block move (the movstrsi_internal parallel, split into lwl/lwr only after
  reload), which also gives it v1 (it overlaps the rodata %hi in v0). No do/while needed, so sched2 is free.
- Two-statement sets for scheduling: Mag_down needed the Timer3 load early and its andi late, and the negate late:
  `Flg = pP->Timer3; Flg &= 2;` (SHORT Flg) and `Wk_pos = pP->Timer0 / 2; Wk_pos = -Wk_pos;`. Neither dest is
  boosted, so sched1 keeps LUID order, and sched2's LUID tie-break then puts the andi between neg and the Timer0
  store. Old_pos had to become single-set again (Timer3 moved to its own variable) so its Old_x load is boosted below
  the Workm.t[1] load. A 192-order perm over the block (agentAA/md2) found it.
- One variable carrying several values in one hard reg: pl_mv07's a2 holds the Rot loop count, the second
  Direction_ck result and the Out count. Merging all three into `i` makes it one long global pseudo with a priority below
  the Out-pointer giv, so the giv takes a1 first and the loop-1 invariant (m + 8) gets a1 too. Merging only two of them
  was worse (15/23). When a register looks "too high", check which other values the target keeps in the same register
  and try one variable for all of them. Also `UCHAR m` (tsweep).
- Do/while weighting: inside the main loop every ref already counts 2 (loop_depth), so one do/while(0) adds only +1 per
  ref; Oba_em_overlap_ck's Z needs exactly that (0.421 -> 0.526 vs Shape 0.522) but every place to put it either moves
  the loads (barrier) or also raises W.
Near misses (agentAA/best, same drafts as agentU unless noted):
- snd Snd_bgm_play_ck (2): proven stuck at sched2. All block-0 priorities are 1 (no loads), so sched2 picks between
  `addiu s0` and the CD lui by LUID, and the target order needs LUID(CD hi) > LUID(s0 lo). Then in the sched1 output
  (the local-alloc input) the CD hi is born after the Snd_work hi died, and local-alloc gives it v0 unless another
  local in v0 is live there. Item_get_ck (sce2.c, matched) gets the same lui v0 / addiu / lui v1 pattern from a local
  (pData, lw v0) live across the second hi. Snd's block 0 has no such value in the final code, so it would have to be
  an insn deleted after local-alloc (no-op move or reload_cse). 96 orders of p/pCd/i with do/while barriers, direct
  global access, and second `p = &Snd_work` sets (cse deletes them) all score >= 2.
- spl Spl_wep_set (12): pP (one var for [4] and [15], refs 12/len 27, prio 1.33) takes a1 before pHand (3/15, 0.2).
  pHand must be global (in the arm it is a local qty and takes a0, which moves pEm: 85). Target needs pHand allocated
  first, or pP with a copy preference for a2, or a1 in pP's regs_someone_prefers; nothing in the function supplies any
  of these (no calls, single caller passing only pEm). Separate p4/pP variables make [15] local (85).
- oba Oba_em_overlap_ck (11): SN prio (agentAA/prio.sh): Shape 143 refs 6 len 23 (0.522) -> t0, Z 94 refs 4 len 19
  (0.421) -> t1; target is the other way. `do { X = Z + W; } while (0)` fixes Shape/Z but raises W's priority and rotates
  s2..s4 (20). A do/while around the Z load fixes nothing because the barrier hoists X/Z above the Floor_lo load (87). The
  last-block X1 (v1, ours) / X0 (a0) swap is local-alloc order (X1 8/len beats X0 3/len). The target result would follow
  if X1/Z1 were global pseudos (allocated after X0/Z0 locals) but no credible reuse found (Be/Area/Tbe reuse
  changes the W*2 hoist: 96-139). 180 load/add orders of that block: all 11.
- cd Cd_link_search (agentK 54), oba Oba_ent_box_set (agentM 45 ds / 135 words), sce_at Sce_at_check (96 ds): not
  worked on beyond scoring.
## Matching round agentZ (worktree clean/zD; drafts in the session scratchpad agentZ/best)
Matched: door Door_init, DoorSort; trans Esp_trans_3d, Esp_trans_main; em_sub Rot_neck.
Rules learnt:
- reload_cse turns a re-read into the `move aN,aM` copy only if the load sits right after the store in sched1 order.
  Door_init: `Mapping_tim((ULONG *)pDoor->pPrepacket)` re-reads the field (cse drops it, because the varying
  pPrepacket address is invalidated by any later store). sched1 can hoist that load next to its store only past
  stores its alias test exempts: a struct ref with a varying address does not conflict with a non-struct ref whose
  address, after canon_rtx (reg_known_value of single-set pseudos), has no register. So the header writes go
  through single-set `ppTim = &Door_data->pTim; ppTmd = &Door_data->pTmd;` pointers and the Ctp write is
  `*(USHORT *)&G.Ctp.u.Ctpage = 0x1F15` (RE2 does the same). A COMPONENT_REF store (`Door_data->pTmd = ..`) or
  a multi-set pointer (agentT's reused Clut) keeps the load below the stores and the copy stays in a0.
- flow.c deletes a stack store when the same slot is stored again with no memory read or other store in between
  (last_mem_set, scanned backwards). Esp_trans_main's D2 (address-taken gte scratch word) keeps both of its
  stores only when `D1 = pSpGp->Ofsy` (a load + store) sits between them.
- Esp_trans_main and Esp_trans_3d both reuse variables the way the target registers show: the uv corners go in the
  X/Y/X1/Y1 position variables (Esp_trans_3d), the loop values in the D0/D1/D2 gte scratch words with a single
  x1 temp (Esp_trans_main), and the OT entry pointer lives in Ysize (one register for both). `Otz = sum;
  Otz >>= 4;` and a `pOtz = pOt + (Otz >> 3)` pointer give the in-place srl and the `addu a0,s1,a0` order.
- Rot_neck: a `pEn_parts = &pNeck_enemy->pSin_parts_ptr[Enemy_neck_no]` pointer (addu base,idx) before the own
  offset and the Neck_tgt copy fixes the top; the y/z blends need `Free_dif`/`Free_dif12` as SHORT with the
  Free_dif line before Vel; the Unk166 update needs two temps (`Dif = Cdir_y + 0x80; Ofs = Unk166 - Dif;
  Dif = (SHORT)Body - Ofs;`) so the subtraction result takes the Unk166 load's register.
- DoorSort: global-alloc order pOt > pPacket > pVert > pPrim > pDw > n > I0 > I1 came from do/while(0) wraps
  (depth 2 on `pVert = pObj->pVert`, depth 2 on the single-statement 0x10 case, depth 1 on gte_strgb3_gt3), the
  V copies interleaved with their addresses (`V0 = I0; pV0 = pVert + V0 * 2; ...`) so V1 = I1 is late enough
  for I1 to outlive I0, and top/loop index loads in 0, 1, 2 order. The gte_strgb3_gt3 belongs inside the
  `if (Otz > 0)` block and gte_avsz3 has no leading nops in the target (gte_avsz3_b in door.c).
- Single-statement priority sweeps with difflib scoring (score2.py DL=1) beat word scoring when an insn is
  missing; prio.sh output (refs / len) tells which variable needs how many extra refs before searching.
Not matched:
- psp Rain_move (best/Rain_move_dl97_idx.c DL 97, best/Rain_move_dl102_ptr.c DL 102): loop 1 is close as agentT's
  goto loop. Target facts: x/y/z (s4/s5/s3) are the same variables as loop 2's Cx/Py/Cz (shared registers), the
  new positions in loop 2 go through two temps (a1/a0), 0x8000 is hoisted into s8 only in loop 2 (so loop 2 has
  loop notes; loop 1 is a goto loop with `li v0,0x8000` inside), pTop (36(sp)) and Cnt (32(sp)) are the spilled
  ones, pFlg is a pointer (s0) in both loops, and loop 2 has no pV+8 giv (ours strength-reduces it as soon as
  loop 2 is a for loop with pV++). Loop 1 has a per-iteration copy of pV in a2 (`move a2,s1` in the delay
  slot of the flag test); a `pP = pV` copy is merged by cse.
- em_sub Kage_work9_sort (best/Kage_work9_sort_ds89.c, agentT's draft unchanged): not reached this round.
## Matching round agentY (worktree clean/zC; drafts in the session scratchpad agentY/best)
Matched: root_ck kara_rootck2 (returns LONG, root_ck.h changed; Root_ck ignores the value), sabun Sa_joint_blend.
Rules learnt:
- sched1 tie-break (sched.c rank_for_schedule): with equal priority and class, the higher LUID is scheduled first
  in the backward pass, i.e. source order is kept. kara_rootck2: reading `Out_link = pN->Link[Next];` into its own
  UCHAR local before `*(pCost + Cost_ofs + (UCHAR)i + Out_link)` keeps the Link address add ahead of the (UCHAR)i
  andi, so the Next-andi pseudo dies before the i-andi is born and both get a0 (the add order stays i first).
- sched.c adjust_priority: before reload every insn that sets a pseudo with REG_N_SETS == 1 ("birthing") is raised
  to max priority (shows as 7f000001 in the -dS ready lists). An insn setting a multi-set variable keeps its small
  natural priority, is picked last in the backward pass and so lands EARLY in the block. Sa_morph_step: `Half =
  pSa->Size; Half >>= 1;` (two sets) moves the Size load/shift from the block end to the front, which also gives
  pKan a1 in local-alloc and the target's `move t0,a1` copy of pP. Use it to move a leaf load up a block.
- cse fold of `i = n - 1; if (i--)` into `n - 2` (fold_rtx PLUS/associate, cse.c ~5710) needs i's class to hold a
  PLUS. `i = ~-n` (== n - 1) is recorded as (not (neg n)), so cse leaves `i - 1` alone and combine turns the
  not/neg into `addiu a2,t3,-1` afterwards: the target's unfolded `move v0,a2; beqz v0; addiu a2,a2,-1`. No C form
  that cse sees as a PLUS avoids the fold (labels, do/while(0) LOOP_END notes only stop cse1; cse2 still folds).
- Sa_joint_blend: the pointer computed before a test and advanced after it (`addu v1,t8,s1` in the delay slot,
  `addiu a0,v1,6` after) came from reusing the Key variable for the block address (`Key = (ULONG)pBase + Shape_sz *
  (Key >> 16); if (i--) { pS = (SHORT *)Key + 3; ...`); Key already lives in v1. A new pBlk local instead adds a
  global and reshuffles every t-register. Separate counters / source pointers for the two parts (Cnt/pFrom vs i/pS)
  gave the target's a0/a1/a2 split; `pHead[1] + (LONG)pHead` gives the `addu t8,v0,a3` operand order; the remaining
  t0..t3 and a1/a2 priorities were fixed with do/while(0) wraps (hc.py), commented in the source.
Near misses (agentY/best):
- sabun Sa_morph_step (Sa_morph_step_160w.c, 160 words / ds 61; agentS's ds-51 draft is 196 words, its ds is lower only because the one-insn pP-copy offset is scored differently): the Half split above
  fixes the pP copy and pKan a1. Left: block-2 order (Parts_no lbu must be 2nd, pParts load 5th, pFrm at the end),
  and the register names that follow (Half t7, pParts s0, pFrm s1, k t9). Statement permutations of block 2 (2500
  random orders) give only two scores, so the order is not decided by LUIDs any more: Parts_no and pParts are
  birthing loads that the hazard rule ("insn N has a greater potential hazard") picks first. hc.py wraps reach 70
  words but only with wraps on half the statements (agentY/smr/b_pp.c.best.c) and they break the pP copy again.
- sca Sca_hit_slope (Sca_hit_slope_47ds.c, ds 47 from 66): `Len = pObj->D0;` / `Len = pObj->W0;` read once for both
  compares (the target keeps D0/W0 in v1 over the abs branches), offsets X = W + D, Z = D - W, Oz = -W then
  Ax = D; Az = D; += X0/Z0 (gives the surviving copy). Left: block 0 X0/Z0 in a3/t0 instead of t0/t1. In the
  target D is a3, i.e. D is allocated after the offsets a0..a2 (looks like a global, not a block-local qty), so X0
  loses a3; renaming D to an existing global (Dist, Len) did not change it. Also `move v1,a3; addu s0,v1,t1`
  (Az from a temp copy of D) is still `move s0,a0`-style in ours.
- sca Sca_hit_box (Sca_hit_box_95ds.c = agentG c960/e.c renamed, ds 95): not improved. Target saves s0/s1 (frame
  32): R lives in s1 to the Sca_hit_box2 call, pT is copied to t6 and Z (Sca_work.Z) is in s0; ours keeps pT in a2.
- sca Sca_hit_hishi: not attempted (no draft, 559 insns).
## Matching round agentAB (worktree clean/zF; drafts in the session scratchpad agentAB/best, agentAB/pm)
Matched: option Config_moji_disp + Config_suuji (together, shared "\x81\x40"; the old Config_space_str INCLUDE_RODATA and
its `.align` line are gone, a `.byte 0xCD` junk byte follows Config_suuji's digit string), status St_box_frame_draw,
St_map_area_select, mojidisp Mess_disp (fork draft).
Rules learnt:
- Branch layout comes from the if-shape, not from the stores: Config_moji_disp's "split" 0x80 path (r0 in a `j` delay
  slot, a separate `j L; sb g0` block after the 0x58 path) is `if (Dark != 0 || Mode == 0) { if (Dark) 0x58 else 0x80 }
  else Kido`. A 10-shape x {const, Col} sweep found it in one run (agentAB/cv); 800 store/constant variants never could.
- User inits before a loop are emitted before loop.c's hoisted movables, and global-alloc priority of a hoisted
  constant is refs/len with len from its preheader position. St_box_frame_draw's s4..s7 (G, 0xD0, 0x20, Item_data_tbl)
  came from `i = 0; pG = &G; pD = Item_data_tbl; u = 0x20; for (; ...)` (pG/pD/u user inits, 0xD0 still hoisted).
  A constant stored once inside a loop is only hoisted when threshold * savings * lifetime >= insn_count (threshold
  drops by 3 per moved insn; -dL prints "not desirable"); a user variable set inside an `if` and used in another
  block is never movable (loop.c: maybe_never and REG_USERVAR_P).
- cse follows a jump only if no NOTE_INSN_LOOP_END sits right before the target label: wrapping `if (..) { ...; return; }`
  in do/while(0) (return instead of break, the switch ends the function) made St_map_area_select recompute &G and
  reload Key_trg after the branch like the target. The rest (`Padd = *(ULONG *)&pG->Padd_trg;` read first,
  `(Key | Pad)` operand order) came from the permuter in a few minutes once that base existed.
- loop.c inserts reduced-giv increments right before the biv increment (insert_before = tv->insn, no auto-inc on
  MIPS); bl->giv is a prepend list and combine_givs bases the combined giv on the first entry, i.e. the giv found
  LAST in the loop (St_box_frame_draw: v0 store last -> base pS+13).
- In cse, after `B = A` the copy B is canonical when B's last use (REGNO_LAST_UID, anywhere in the function) is later
  than A's: Get_light_cut's `pData = pLight` at the exits makes pData canonical, so `pLight[0]` written after the copy
  is read through pData.
Near misses (agentAB/best):
- sub Get_light_cut (3 words; Get_light_cut_3_regs.c = registers right, pCut add after the pLight add;
  Get_light_cut_3_order.c = order right, pData copy in a1). The target needs, in sched1 order, the copy
  (`move a0,s0`) before the Ofs load through s0 (so pData and pLight conflict and global.c expand_preferences does not
  give pData pLight's a1 copy preference) AND the Ofs load not rewritten by local-alloc optimize_reg_copy_1 (needs a
  label/jump/loop note between copy and load) AND the pCut add before the pLight add. sched1 puts the copy (single
  set, birthing boost) between the Ofs load and its consumer because of the load latency; making pData multi-set
  (a real second value: `pData = (UCHAR *)(pCut + ..); return (LIGHT_CUT_DATA *)pData;` tails) un-boosts the copy but
  then cse makes pData canonical and the Ofs load reads a0. Loop-note placements, 96 statement orders x tails, and
  ~30 min of permuter did not reach it.
- pc_sys Pc_text_set (6, agentV draft unchanged): the compare biv increment must be the lowest-LUID insn of the loop
  body while the store giv (a2 = pSpr + 15) is incremented at the bottom. Giv increments always sit right before
  their biv's increment, so the two must belong to different bivs (or the target scheduled with different
  priorities). Index loops (Spr[i + 4], i counters with i++ at top/bottom, all types) do not reduce the stores at all.
- epilog Epilog_draw (7; Epilog_draw_7.c = `pWk = &Epi_work; do { pG = &G; } while (0);`): local-alloc must see
  hiE and hiG live at the same time with hiG shorter (hiG v0, hiE v1). With birthing boosts each hi lands right above
  its lo, and a single-set pG lo is ready right after the call, so the pairs never interleave; pG needs a second real
  set (cse deletes a duplicate `pG = &G`). Flg pointer variables / Epi_work.u.Flg forms: no.
- mojidisp Print_main (fork, 106 words, agentAB/pm/final_draft.c; structure right, from ~330): needs a MOJIDISP field
  at 0x75F8 (Print_main's text buffer, `&Moji + 0x75F8`; the draft uses a MOJIDISP2 view) and
  `extern UCHAR ascii_tbl[2][17];` (RE2 name; D_80098704 upper, D_80098715 lower hex digits). Left: global-alloc puts
  Width (120 refs) above Val (62) so Width takes a1 (target Val a1, Width t1); the `(Digit % 10) * Sign` product is s6
  in the target (s5 saved but unused), v0 in ours; Width--/Keta-- and the Div/10 subu after mflo. Key facts: inline
  `if (Attr & 0x4000)` (not a USHORT local) keeps 0x66666667 from being hoisted out of the outer loop; the default case
  keeps the original bug (`pFmt = (UCHAR *)i`); 'x' falls into 'X'.
## Matching round agentX (worktree clean/zB; drafts in the session scratchpad agentX/best)
Matched: sca Sca_hit_koban_x. Sca_hit_line matches too, but only with a maspsx fix (tools/maspsx-mflo-load.patch,
README step 4); the C is NOT committed (agentX/best/Sca_hit_line_match_needs_maspsx_patch.c). With the patched maspsx
(`make MASPSX="python3 <patched>/maspsx.py --aspsx-version=2.77 -G0"` after `rm -rf build/src`) the clean tree and the
tree with Sca_hit_line in C both give 60 OK, so the patch changes no other object.
Tool finding (maspsx): ASPSX keeps 2 instructions between `mflo` and the next mult/div; a plain one-instruction load in
between (`mflo s0; lw v0,16(sp); div`) still needs a nop (target: `mflo s0; lw v0,16(sp); nop; div`). maspsx skipped
every load there ("allow for $at handling later"), which is only right for a symbolic load that expands through $at.
The game asm has no `mflo; load; mult/div` without that nop, so the patch only restricts the skip to `uses_at` loads.
Rules learnt:
- A variable set more than once never gets sched1's birthing boost (birthing_insn_p needs REG_N_SETS == 1), so its sets
  float up, and it is a global-alloc pseudo. Sca_hit_line: `V1 = (SHORT)P3; V0 = P2 << 16; V0 >>= 16; Nx = (V1 - V0)
  >> 1; V1 = P3 >> 16; V0 = P2 >> 16; Nz = -((V1 - V0) >> 1);` gives the target's P3-chain-first order with P3 in v1 and
  P2 in a0 (as block-local qtys the later-born chain always wins v1, and sched2 keeps sched1's chain order). `do { P =
  P1 + ...; } while (0)` is the one extra P1 ref that puts P1 above P2 in global-alloc; xor as `Opz[0] ^ Opz[1]`.
- local-alloc qty priority ties (floor_log2(refs) * refs / life) go to the lower qty number. Koban: with
  `pW = &Sca_work` in its own single-set pointer (not the union), the la sinks to the end of block 0 in sched1, which
  stretches the Z1-Z0 temp's qty so the X1 load temp wins v1 (target), and `D` (SHORT copy) stays a separate global.
- Koban_x case 3: the X0 store needs one more ref (`do { pC->X0 = X0; } while (0)`, X0 above X), but that is a
  sched barrier; the target computes D + Z0 before it, which is a temp computed before the barrier: `Cx1 = D + Z0;`
  (Cx1 reused, as case 0 reuses it for X1 - D), then `pC->Z1 = Cx1;` after the store. The Sca_hit_box return is also
  wrapped (pT above D).
- A variable referenced in a second block only through an insn that combine later deletes is still global: flow sets
  REG_BASIC_BLOCK before combine and nothing recomputes it. Sca_line_circle: `X0 = Dx2 * Dx2 + Dz2 * Dz2;
  SquareRoot0(X0)` in the second part (combine folds the sum into the a0 argument) made X0 a global s6 like the target,
  and the `X0 - Dz` result then ties with the dying Dz (s0) instead of X0.
- global.c only kicks local-alloc pseudos out to the stack (reg_renumber = -1); they are not reallocated. A local that
  ends in a later s-reg (Sca_line_circle X0 s6) is therefore a global, not a kicked-out local.
Near misses (agentX/best):
- sca Sca_hit_koban_z (Sca_hit_koban_z_2.c, 2 sched words): the koban_x solution mirrored; every register right. The
  target needs Z0 above Z (one more Z0 ref). The only single wrap that gives it is the Z0 store, but the target issues
  the Sca_work la before that store (`sh Z1; sh X1; lui/addiu Sca_work; sh Z0; sh X0; sw X; sw Z`), which a do/while
  barrier forbids (the la comes from the later `Sca_work.X = X`). Searched: all case-3 orders (with/without the Cz1
  temp) x one wrap, contiguous do/while ranges, block-0 orders x block-0 wraps, Z0/X0/D/Cz0 reused as the case-0 temp
  (Z0 reuse gives the right globals but LONG arithmetic: sll/sra of D and an lh reload), pW reuse, type sweep.
- sca Sca_line_circle (Sca_line_circle_dl8.c, 22 words, was 43; difflib 8): all registers right now (X0 global via the
  combine trick above, `do { Dx *= Half; } while (0)` puts Half above X0, SHORT X1, the Sca_ck_cross args inline).
  Left: sched only. The target loads a1 for Sca_ck_cross (`lw a1,0x4C(sp)`) right after `move a0,s3` and computes
  both X parts before the Z parts, and the P1 home-slot store sits after `sw s8`; ours puts the a1 load last. In sched1
  the `a1 = P1` move gets the birthing boost (REG_N_SETS(a1) == 1, a1 live), so it is placed just above the a2/a3 ors;
  in the target it behaves unboosted like the `a0 = P0` move. Not found what makes it so (arg expression forms, temps,
  `(void)&P1` all give the same).
- sca Sca_high_hishi (agentI 13_dw.c still the best word score; Sca_high_hishi_t5.c is the clean per-case
  `if (..) return pT->High; break;` form with the same allocation as the goto version). With the current live lengths
  (X0 6/39, Z1 5/30, Z 4/28, Z0 5/32, Dz 4/29) no ref increments reach X0 > Z1 > Z > Z0 > Dz while X0 stays below Cx
  (0.444): X0 7 refs (0.359) forces Z1 5 and Z 4, and Z0 0.312 is then above Z. So the original has different
  lengths, not only extra refs; load-order permutations (720) do not change them (sched1 reorders the loads).
## Matching round agentW (worktree clean/zA; drafts in the session scratchpad agentW/best)
Matched: em22 Em22_dir_quarter, em34 Em34_shoot.
Rules learnt:
- Run the permuter early, even on long functions: both matches came from it (15 min / 40 min), after hand
  sweeps of 2000+ variants had stalled. Its `score` is not the word count: check every output-* dir with
  score.py (Em34_shoot's "score 15" output was a 0).
- A constant (or load) that the target issues well above its use is not birthing-boosted (sched.c
  birthing_insn_p: REG_N_SETS == 1): it goes through a variable that is set more than once. Em34_shoot:
  `Id = 0x101; EM_RTN(pEm) = Id;` (Id is also the Esp id in case 3) keeps li 0x101 below the Flg/UnkE3 loads, so
  it gets a0 like the target. em23: case 6's `Dir` had to stay single-set (moving the Wall_dir temps of cases
  8/10/12 to a separate `Wall` put the Ground load before the Side load).
- A surviving `move v1,a0; jr ra; move v0,v1` after `andi a0` in one arm (Em22_dir_quarter): combine merges
  `Ret &= 1; Dir = Ret;` when they share a block, and merges `Dir = Ret; return Dir;` when the copy sits at the
  join. The target had a copy in each arm, one of them separated from the and by a label, and jump2 cross-jumped
  the two copies after reload. The label comes from a dead if with identical arms (`if (Dir || Flg) Ret &= 1;
  else Ret &= 1;`), which leaves no code. `if (Dir)` alone also gives the shape but other registers.
- (em23) `X = W; if (W != 0x1000) X = W + 16; store X` per arm (one read of the field into a LONG first) is what
  gives the `move a0,v1` / `move v0,v1` arm copies; a ternary with the field read in each place gives an extra
  lhu. Load-after-store order: `Pos_z = pK->Pos_z` must be read before the zero stores (it may alias them), and a
  block-local single-set `pK` lands right above its first use.
Near misses (agentW/best):
- em23 Em23_mv03 (Em23_mv03_20.c, difflib 20, was 49): case 4 `Unk1A = 0x800` before EM_SET_MOVE (li 2048 in v1);
  case 6 with a block-local `KAGE_WORK *pK6`, Pos_x/Pos_z copied before the Col/Mode zero stores, `Dir` single-set;
  case 12 Wall_dir via `Wall` plus per-arm `Up`/`Next` copies; case 4/6/12 now exact. Left: the case 8/10 Wall_dir
  steps (both 1 insn short): target reads Wall_dir once (lh v1), the <= 0x800 arm compares a copy in a0
  (`move a0,v1` in the bnez delay slot, then reorg rewrites the +16 to read v1) and all paths store v0 at one join.
  Ternary/if-chain/per-arm copy forms (~150 variants, with/without a separate result variable) give either the
  hoisted extra lhu or no copy. The three Unused VECTOR/SVECTOR locals are needed for the frame size.
- em22 Em22_mv_jump (7 words, agentQ draft unchanged): moving the pSub1_seq_t_ptr copy anywhere but after
  `pW->pTmd = pP->pTmd` costs 12+ words (the pPl->pSin_parts_ptr load then moves); 25 min of permuter found nothing.
- em38 Em38_sce03 (7 words, unchanged): case 6 `Sv.vy = -800` through a multi-set local does move the li out of
  the boosted spot, but it then lands above the Timer0 ori/sh (lowest LUID) instead of between sh 216 and the
  `addiu s0` (17). Case 5 a3/a2 order: pM/pM2 temporaries for &pW->Workm, store order of Sv do nothing. Permuter
  40 min: nothing.
- em36 Em36_Init (12 words, unchanged): the Pldamage `lbu Id` / `lui Pldamege` / `&G.pEnemy[2]` are issued early
  (not boosted) while the sw stays after the 0x1EA..0x1E6 zero stores. Reading Id into a multi-set i/n/Bit at
  every position before the stores: 23 at best. The permuter's score for this function does not follow the word
  count (its "improvement" was difflib 23).
## Matching round agentAE (worktree clean/wC; drafts in the session scratchpad agentAE/best)
Matched: sca Sca_hit_box (was difflib 95).
Rules learnt:
- Too few s-registers saved = too little register pressure: Sca_hit_box saves s0/s1 only because every caller-saved
  register is busy around the Side computation. The original reuses variables for unrelated values: Span_x/Span_z
  later hold the push distance, Gz the pushed edge position, Rz the move direction (X - Ox, then Z - Oz, then the
  rotated z move). Separate locals leave registers free and the frame shrinks to 24.
- A branch delay slot filled with the first insn of the *taken* block's la (`bnez; addiu a3,%lo(Sca_work)`) plus a
  load-delay nop before the andi means the lo was scheduled after the test: `Wall = pT->Type & 0xF00; pW = &Sca_work;
  if (!Wall)` (andi LUID before the lo).
- `Ox = (..) * 181 >> 8; Ox += X0;` (two statements) when the target adds X0 after both rotations.
- `if ((Mx < 0 ? -Mx : Mx) > W0 * 2)` vs `W0 * 2 < abs`: same slt, but the abs insn gets the lower LUID, which moves
  the W0 reload below the Mx subtract (sched1 tie-break).
- SHORT variables are HImode pseudos: loaded with lhu, every SImode use sign-extends with sll/sra 16 and the `<< 16`
  value is kept (Sca_hit_hishi s0..s3 hold X1/Z1/Cx/Cz << 16 for the /18 sign fix). `SHORT Cx = pT->X0 + h` is
  narrowed to an HImode add, which is why X0 is read twice (lh for SImode uses, lhu for the HImode add). A LONG result
  of a SHORT / 18 (`LONG Ax = Cx / 18`) gives the target's temp chain in v0 followed by `sra Ax,v0,16`; a SHORT result
  computes the chain in place.
- Inline `Qa * Qa + Rel * Rel` written twice (cse shares it) instead of a `Sq` variable: the sum is a block-local temp
  tied with Rel*Rel (v0); a variable made it a global (a0).
- Global-alloc order fixed by how often cross-jumped code is written: Sca_hit_hishi's pObj needs more refs than Z1, so
  some `Hit_flg |= 0x100; return 1;` exits are written inline in the cases (jump2 cross-jumps them into one block after
  reload, but global-alloc counted the refs). Wrapping the shared flag store in do/while(0) gives the refs too but
  then the two flag blocks become identical and get cross-jumped (the target keeps two).
- Search tools: agentAE/hgen.py + hhc.py (role-to-variable hill-climb for a templated function; each case's temporaries
  are assigned from a pool of variables), sdiff.py (difflib diff with registers masked: shows only structural
  differences), mkp.sh (perm dir for an sca.c draft), pcopy.py, ext.py (score every permuter output with difflib).
Near misses:
- sca Sca_hit_hishi (best/Sca_hit_hishi_3.c, 3 words, first C version; was asm only): only the switch index block is
  left. Target: `srl v1,a3,30; andi v1,v1,2; srl v0,a0,31; or v1,v1,v0` (Dz part first, in v1). Ours either has the
  Dz part first in v0 (local-alloc gives the srl/andi chain qty priority 2*4/6 over the Dx part 1*2/2) or the Dx part
  first. ~60 forms tried (part order, casts, + instead of |, temporaries in free variables, 2-3 statements); a Quad
  variable that lives across the dispatch becomes global and reshuffles everything. The draft still has generic
  temporaries (Ta..Te reused across cases, Qa/Qx/Qz/Sq reused as case temporaries, as the target's registers show),
  `goto ret0/end/hit` labels (needed for the return-0 / two flag-block layout) and three inline flag exits.
- sca Sca_hit_slope (best/Sca_hit_slope_23.c, 23 words, was 47 ds): X/Z rotation in do/while(0) gives X0/Z0 t0/t1;
  the B offsets in Z/X (Z = W + D, X = D - W), D reused for Pos_x - X0. Left: the target's `move v1,a3; addu
  s0,v1,t1` (Az = copy of D + Z0) right after Ax. That local copy is what pushes -W to a2 and D to a3. Every form
  tried either has no copy (combine merges `P = D; Az = P + Z0` because P dies there; cse makes D canonical) or the
  add is scheduled late (copy before the D reassignment, add after it: LUID order puts it in the delay slot). Tried:
  copy in Len/Dist/Ox (Len canonical needs Len's last use after D's), do/while(0) around the add, two separate
  `if (Type & 0x1000)` blocks, per-corner offset variables, Cx/Cz as an array (worse: they are spilled pseudos).
