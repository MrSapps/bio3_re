#!/usr/bin/env python3
"""Write the splat config for each CD_DATA/BIN code overlay (ovl/<name>.yaml).

usage: tools/mkovl.py NAME TEXT_START DATA_START   (offsets in the file, hex)
Layout of every overlay: [task-number word][.rodata][.text][.data]; WARNING.BIN has no task word.
Load addresses come from the exe's overlay base table (D_8001031C).

Enemy modules (iso/EM/emXX_1.bin, tools/extract_em.py): NAME is emXX. They have no task word and are
split for load slot 1 (0x80100000); the Makefile links the same objects again for slot 2 (0x8010D000).
"""
import hashlib, os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BASE = {"TITLE": 0x80194000, "DIEDEMO": 0x80194000, "ENDING": 0x80194000, "WARNING": 0x80184000}

def main():
    name, ts, ds = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
    low = name.lower()
    em = low.startswith("em")
    path = f"iso/EM/{low}_1.bin" if em else f"iso/BIN/{name}.BIN"
    data = open(os.path.join(ROOT, path), "rb").read()
    vram = 0x80100000 if em else BASE.get(name, 0x801C2000)
    ro = 0 if name == "WARNING" else 4
    src = f"enemy/{low}" if em else f"ovl/{low}"  # src/enemy/<name>.c or src/ovl/<name>.c
    subs = []
    if ro:
        subs.append(f"      - [0x0, rodata, ovl/{low}/header]")
    if em and ts > ro:
        # .rdata alignment padding before .text is linker filler that differs between the two slot
        # files (em2c): record those bytes as a post-link fix-up of the slot 2 binary
        d2 = open(os.path.join(ROOT, f"iso/EM/{low}_2.bin"), "rb").read()
        fix = [(i, d2[i]) for i in range(ts - 3, ts) if data[i] != d2[i]]
        if fix:
            open(os.path.join(ROOT, f"ovl/{low}_2.fixup"), "w").write(
                f"# slot 2 linker filler bytes (offset value), applied after objcopy\n"
                + "".join(f"{i:#x} {v:#x}\n" for i, v in fix))
    if ts > ro:
        subs.append(f"      - [{ro:#x}, .rodata, {src}]")
    subs.append(f"      - [{ts:#x}, c, {src}]")
    end = len(data) & ~3
    if ds < end:
        subs.append(f"      - [{ds:#x}, data, ovl/{low}/data]")
    if end < len(data):  # some modules end in a partial word
        subs.append(f"      - [{end:#x}, bin, ovl/{low}/tail]")
    if em:  # slot 2 is a separate file with its own file number in the header word
        w2 = int.from_bytes(open(os.path.join(ROOT, f"iso/EM/{low}_2.bin"), "rb").read()[:4], "little")
        open(os.path.join(ROOT, f"ovl/{low}_2.header.s"), "w").write(
            f"# header word (file number) of the slot 2 copy of {low}\n.section .rodata\n.word {w2:#x}\n")
    yaml = f"""name: {os.path.basename(path)}
sha1: {hashlib.sha1(data).hexdigest()}
options:
  basename: {low}
  target_path: {path}
  elf_path: build/ovl/{low}.elf
  base_path: ..
  platform: psx
  compiler: PSYQ
  asm_path: asm
  src_path: src
  build_path: build
  ld_script_path: ovl/{low}.ld
  ld_dependencies: False
  ld_bss_is_noload: True
  ld_align_section_vram_end: False
  ld_align_segment_vram_end: False
  ld_align_segment_start: False
  find_file_boundaries: False
  o_as_suffix: False
  use_legacy_include_asm: False
  section_order: [".rodata", ".text", ".data", ".bss"]
  symbol_addrs_path:
    - symbol_addrs.txt
    - syms/game.txt
    - ovl/{low}.syms.txt
  undefined_funcs_auto_path: ovl/{low}.undefined_funcs_auto.txt
  undefined_syms_auto_path: ovl/{low}.undefined_syms_auto.txt
  subalign: 2
  string_encoding: ASCII
  data_string_encoding: ASCII
  rodata_string_guesser_level: 2
  data_string_guesser_level: 2
  include_asm_macro_style: maspsx_hack
  migrate_rodata_to_functions: True
  asm_jtbl_label_macro: jlabel
  disassemble_all: True
  gp_value: 0x80094680
segments:
  - name: {low}
    type: code
    start: 0x0
    vram: {vram:#x}
    subsegments:
{chr(10).join(subs)}
  - [{len(data):#x}]
"""
    open(os.path.join(ROOT, f"ovl/{low}.yaml"), "w").write(yaml)
    sp = os.path.join(ROOT, f"ovl/{low}.syms.txt")
    if not os.path.exists(sp):
        open(sp, "w").write(f"// {name}.BIN symbols (loaded at {vram:#x})\n")

def em_bss(low):
    """Module symbols splat left undefined (BSS past the file end): define them relative to the module
    base, so the slot 2 link moves them too. Run after `splat split`."""
    out = []
    for f in ("undefined_syms_auto", "undefined_funcs_auto"):
        for line in open(os.path.join(ROOT, f"ovl/{low}.{f}.txt")):
            r = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", line)
            if r and 0x80100000 <= int(r.group(2), 16) < 0x8010D000:
                out.append(f"{r.group(1)} = {low}_VRAM + {int(r.group(2), 16) - 0x80100000:#x};")
    open(os.path.join(ROOT, f"ovl/{low}.bss.ld"), "w").write("\n".join(out) + "\n")

if sys.argv[1] == "--bss":
    em_bss(sys.argv[2])
else:
    main()
