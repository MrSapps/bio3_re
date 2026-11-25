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
