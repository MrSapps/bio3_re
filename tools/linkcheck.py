#!/usr/bin/env python3
"""Private full link of one edited C file against the committed build objects.

usage: tools/linkcheck.py src/game/XXXX.c
Compiles FILE.c (same pipeline as the Makefile), links it with the other objects
from build/ into a temp dir, and compares the whole executable with the target.
Checks what fdiff.py can't: .rodata (jump tables, strings) placement and contents.
Needs one prior full `make`. Safe to run in parallel (writes only to a temp dir).
Exit 0 on a byte match; otherwise prints the first differing addresses and the
nearest symbols from the map.
"""
import os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.argv, args = sys.argv[:1], sys.argv[1:]
if len(args) != 1:
    sys.exit(__doc__)
cfile = os.path.relpath(os.path.abspath(args[0]), ROOT)

BIN = os.path.join(ROOT, "tools/binutils/usr/bin/mipsel-linux-gnu-")
ENV = dict(os.environ, LD_LIBRARY_PATH=os.path.join(ROOT, "tools/binutils/usr/lib/x86_64-linux-gnu"))
PSYQ_INC = os.environ.get("PSYQ_INC", "/home/snake/dev/psyq_sdk/psyq_4.4/include")
BASE = "slus_009.23"

def run(cmd):
    r = subprocess.run(["bash", "-o", "pipefail", "-c", cmd], cwd=ROOT, env=ENV, capture_output=True, text=True)
    if r.returncode:
        sys.exit("failed: " + cmd[:200] + "\n" + r.stderr[-3000:])
    return r.stdout

if not os.path.exists(os.path.join(ROOT, "build", BASE + ".elf")):
    sys.exit("run a full `make` once first")

tmp = tempfile.mkdtemp(dir=os.environ.get("TMPDIR"))
try:
    obj = os.path.join(tmp, "own.o")
    run(f"cpp -P -undef -D__GNUC__=2 -Dmips -D__psx__ -D_LANGUAGE_C -Iinclude -Isrc -I{PSYQ_INC} {cfile}"
        f" | tools/compilers/gcc-2.8.1-psx/cc1 -quiet -O2 -G0 -mgas -msoft-float -fno-builtin -funsigned-char -o -"
        f" | {sys.executable} tools/maspsx/maspsx.py --aspsx-version=2.77 -G0"
        f" | {BIN}as -EL -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -G0 -Iinclude -o {obj} -")
    ld = open(os.path.join(ROOT, BASE + ".ld")).read()
    own = f"build/{cfile}.o"
    ld = ld.replace(own + "(", obj + "(")
    ld = re.sub(r"(?<![\w/])build/", os.path.join(ROOT, "build") + "/", ld)
    open(os.path.join(tmp, "l.ld"), "w").write(ld)
    # syms.ld fresh from syms/*.txt (agents may have added names)
    syms = []
    for f in sorted(os.listdir(os.path.join(ROOT, "syms"))):
        for line in open(os.path.join(ROOT, "syms", f)):
            line = re.sub(r"//.*", "", line)
            m = re.match(r"\s*([A-Za-z_][\w.]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", line)
            if m:
                syms.append(f"PROVIDE({m.group(1)} = {m.group(2)});")
    open(os.path.join(tmp, "syms.ld"), "w").write("\n".join(syms) + "\n")
    elf = os.path.join(tmp, "o.elf")
    run(f"{BIN}ld -EL --no-check-sections -nostdlib -T {tmp}/l.ld -T undefined_funcs_auto.txt"
        f" -T undefined_syms_auto.txt -T {tmp}/syms.ld -Map {tmp}/o.map -o {elf}")
    run(f"{BIN}objcopy -O binary {elf} {tmp}/o.bin")
    got = open(f"{tmp}/o.bin", "rb").read()
    want = open(os.path.join(ROOT, "iso", "SLUS_009.23"), "rb").read()
    if got == want:
        print("linkcheck: MATCH")
        sys.exit(0)
    diffs = [i for i in range(min(len(got), len(want))) if got[i] != want[i]]
    if len(got) != len(want):
        print(f"size differs: got {len(got):#x}, want {len(want):#x}")
    # nearest symbols from the link map
    syms = []
    for line in open(f"{tmp}/o.map"):
        m = re.match(r"\s+0x0*([0-9a-f]{8})\s+([A-Za-z_][\w.]*)\s*$", line)
        if m:
            syms.append((int(m.group(1), 16), m.group(2)))
    syms.sort()
    def near(a):
        best = None
        for s in syms:
            if s[0] <= a: best = s
            else: break
        return f"{best[1]}+{a - best[0]:#x}" if best else "?"
    print(f"{len(diffs)} differing bytes")
    shown = set()
    for i in diffs:
        a = 0x80010000 + i - 0x800
        w = a & ~3
        if w in shown: continue
        shown.add(w)
        print(f"  {w:08x} ({near(w)})  got {got[w-0x80010000+0x800:w-0x80010000+0x804].hex()} want {want[w-0x80010000+0x800:w-0x80010000+0x804].hex()}")
        if len(shown) >= 12: break
    sys.exit(1)
finally:
    shutil.rmtree(tmp, ignore_errors=True)
