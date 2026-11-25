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
