#!/usr/bin/env python3
"""Compile a C file and compare one function against the target asm.

usage: tools/fdiff.py FILE.c FUNC [--cc gcc-2.7.2-psx] [--flags "..."] [-v]
Compares instruction words with relocated fields masked, then checks that every
relocation (jal target, %hi/%lo address incl. addend) resolves to the same address
as in the target asm. Symbol addresses come from symbol_addrs.txt, the
undefined_*_auto.txt files, NAME_XXXXXXXX-style names, and --syms FILE
(lines "name = 0xADDR;") for names you introduced. Exit 0 on match.
"""
import argparse, os, re, shutil, subprocess, sys, tempfile, struct

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, "tools/binutils/usr/bin/mipsel-linux-gnu-")
PSYQ_INC = os.environ.get("PSYQ_INC", "/home/snake/dev/psyq_sdk/psyq_4.4/include")
ENV = dict(os.environ, LD_LIBRARY_PATH=os.path.join(ROOT, "tools/binutils/usr/lib/x86_64-linux-gnu"))

def load_syms(extra):
    m = {}
    files = [os.path.join(ROOT, f) for f in ("symbol_addrs.txt", "undefined_syms_auto.txt", "undefined_funcs_auto.txt")]
    files += extra
    for f in files:
        if not os.path.exists(f):
            continue
        for line in open(f):
            r = re.match(r"\s*([A-Za-z_][\w.]*)\s*=\s*(0x[0-9A-Fa-f]+)", line)
            if r:
                m.setdefault(r.group(1), int(r.group(2), 16))
    return m

def sym_addr(name, syms):
    if name in syms:
        return syms[name]
    r = re.search(r"_([0-9A-F]{8})$", name)
    return int(r.group(1), 16) if r else None

def target_words(func, cfile=None):
    path = None
    if cfile:  # prefer the asm of this C file (overlays reuse addresses, so names repeat)
        rel = os.path.relpath(os.path.abspath(cfile), os.path.join(ROOT, "src"))[:-2]
        for sub in ("nonmatchings", "matchings"):
            p = os.path.join(ROOT, "asm", sub, rel, func + ".s")
            if os.path.exists(p):
                path = p
    for d, _, fs in ([] if path else os.walk(os.path.join(ROOT, "asm"))):
        if func + ".s" in fs:
            path = os.path.join(d, func + ".s"); break
    if not path:
        sys.exit(f"no asm for {func}")
    words, masks, text, rel = [], [], [], []
    in_func = False
    for line in open(path):
        if re.match(rf"\s*glabel {func}\b|\s*jlabel {func}\b", line):
            in_func = True
        m = re.match(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/\s+(.*)", line)
        if in_func and m:
            b = bytes.fromhex(m.group(2)); w = struct.unpack("<I", b)[0]
            ins = m.group(3)
            mask = 0xFFFFFFFF
            r = None
            mm = re.search(r"%(hi|lo)\((\w+)(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?\)", ins)
            if mm:
                mask = 0xFFFF0000
                r = (mm.group(1), mm.group(2), int(mm.group(3), 0) if mm.group(3) else 0)
            elif re.match(r"(jal|j)\s", ins) and not ins.startswith("jr"):
                mask = 0xFC000000
                tgt = ins.split()[-1]
                if not tgt.startswith("."):
                    r = ("jal", tgt, 0)
                else:
                    fa = int(m.group(1), 16) - 4 * len(words)
                    r = ("jloc", None, int(tgt[2:], 16) - fa)
            words.append(w); masks.append(mask); text.append(ins.strip()); rel.append(r)
        if in_func and line.startswith("endlabel"):
            break
    return words, masks, text, rel

def build(cfile, cc, flags):
    tmp = tempfile.mkdtemp(dir=os.environ.get("TMPDIR"))
    obj = os.path.join(tmp, "o.o")
    cpp = f"cpp -P -undef -D__GNUC__=2 -Dmips -D__psx__ -D_LANGUAGE_C -I{ROOT}/include -I{ROOT}/src -I{PSYQ_INC} {cfile}"
    cc1 = f"{ROOT}/tools/compilers/{cc}/cc1 -quiet {flags} -o -"
    masp = f"{sys.executable} {ROOT}/tools/maspsx/maspsx.py --aspsx-version=2.77 -G0"
    asm = f"{BIN}as -EL -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -G0 -I{ROOT}/include -I{ROOT} -o {obj} -"
    # pipefail: a cc1 error must fail the build, not yield a partial object
    r = subprocess.run(["bash", "-o", "pipefail", "-c", f"cd {ROOT} && {cpp} | {cc1} | {masp} | {asm}"],
                       env=ENV, capture_output=True, text=True)
    if r.returncode or not os.path.exists(obj):
        shutil.rmtree(tmp, ignore_errors=True)
        sys.exit("build failed:\n" + r.stderr[-3000:])
    return obj

def obj_func(obj, func):
    out = subprocess.run([BIN + "objdump", "-d", "-r", "-z", "--no-show-raw-insn", obj], env=ENV,
                         capture_output=True, text=True).stdout
    raw = subprocess.run([BIN + "objdump", "-d", "-z", obj], env=ENV, capture_output=True, text=True).stdout
    words, text, rel = [], [], {}
    on = False
    start = None
    cur = None
    for line in out.splitlines():
        if re.match(rf"[0-9a-f]+ <{re.escape(func)}>:", line):
            start = int(line.split()[0], 16); cur = True; continue
        if cur and re.match(r"[0-9a-f]+ <", line): break
        m = re.match(r"\s*([0-9a-f]+):\s+R_MIPS_(HI16|LO16|26)\s+(\S+)", line)
        if cur and m:
            rel[int(m.group(1), 16) - start] = ({"HI16": "hi", "LO16": "lo", "26": "jal"}[m.group(2)], m.group(3))
    for line in raw.splitlines():
        if re.match(rf"[0-9a-f]+ <{re.escape(func)}>:", line): on = True; continue
        if on and re.match(r"[0-9a-f]+ <", line): break
        m = re.match(r"\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+(.*)", line)
        if on and m:
            words.append(int(m.group(1), 16)); text.append(m.group(2).strip())
    return words, text, rel, start

def resolve_compiled(ow, rel, syms, start):
    """index -> resolved address for each relocated insn of the compiled function."""
    res = {}
    for off, (kind, name) in rel.items():
        i = off // 4
        if i >= len(ow):
            continue
        base = 0 if name.startswith(".") else sym_addr(name, syms)
        if base is None:
            res[i] = ("?", name)
            continue
        w = ow[i]
        if kind == "jal" and name.startswith("."):
            res[i] = ("jloc", ((w & 0x3FFFFFF) << 2) - start)
            continue
        if kind == "jal":
            res[i] = ("jal", base + ((w & 0x3FFFFFF) << 2) if (w & 0x3FFFFFF) else base)
        elif kind == "lo":
            imm = w & 0xFFFF
            res[i] = ("lo", base + (imm - 0x10000 if imm & 0x8000 else imm))
        else:
            ahl = (w & 0xFFFF) << 16
            for o2 in sorted(rel):
                if o2 > off and rel[o2] == ("lo", name) and o2 // 4 < len(ow):
                    lo = ow[o2 // 4] & 0xFFFF
                    ahl += lo - 0x10000 if lo & 0x8000 else lo
                    break
            res[i] = ("hi", ((base + ahl + 0x8000) >> 16) & 0xFFFF)
    return res

def resolve_target(r, syms):
    kind, name, add = r
    if kind == "jloc":
        return ("jloc", add)
    a = sym_addr(name, syms)
    if a is None:
        return ("?", name)
    a += add
    if kind == "hi":
        return ("hi", ((a + 0x8000) >> 16) & 0xFFFF)
    return (kind, a)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("cfile"); ap.add_argument("func")
    ap.add_argument("--cc", default="gcc-2.8.1-psx")
    ap.add_argument("--flags", default="-O2 -G0 -mgas -msoft-float -fno-builtin -funsigned-char")
    ap.add_argument("-v", action="store_true")
    ap.add_argument("--syms", action="append", default=[], help="extra 'name = 0xADDR;' file")
    a = ap.parse_args()
    m = re.search(r"src/ovl/(\w+)/", os.path.abspath(a.cfile))
    if m:  # overlay: its own symbols first, then its view of the exe
        o = os.path.join(ROOT, "ovl", m.group(1))
        a.syms = [o + ".syms.txt", o + ".undefined_funcs_auto.txt", o + ".undefined_syms_auto.txt"] + a.syms
    syms = load_syms(a.syms)
    tw, tm, tt, tr = target_words(a.func, a.cfile)
    obj = build(a.cfile, a.cc, a.flags)
    try:
        ow, ot, orel, ostart = obj_func(obj, a.func)
    finally:  # one temp dir per run: never leave it behind (36k of them once filled the disk)
        shutil.rmtree(os.path.dirname(obj), ignore_errors=True)
    ores = resolve_compiled(ow, orel, syms, ostart)
    # strip trailing alignment nops from compiled output
    while len(ow) > len(tw) and ow[-1] == 0: ow.pop(); ot.pop()
    n = max(len(tw), len(ow)); bad = 0
    for i in range(n):
        t = tw[i] if i < len(tw) else None; o = ow[i] if i < len(ow) else None
        ok = t is not None and o is not None and (t & tm[i]) == (o & tm[i])
        note = ""
        if ok and tr[i] is not None:
            want = resolve_target(tr[i], syms)
            got = ores.get(i)
            if got is None:
                ok = False; note = "  <- target has reloc, compiled has none"
            elif want[0] == "?" or got[0] == "?":
                ok = False; note = f"  <- unknown symbol address ({want[1] if want[0]=='?' else got[1]}); add it with --syms"
            elif want != got:
                ok = False; note = f"  <- reloc resolves to {got[1]:#x}, want {want[1]:#x}"
        elif ok and i in ores:
            ok = False; note = "  <- compiled has reloc, target has none"
        bad += not ok
        if a.v or not ok:
            print(f"{'  ' if ok else '!!'} {i*4:4x}  {tt[i] if t is not None else '':40s} | {ot[i] if o is not None else ''}{note}")
    print(f"{a.func}: {'MATCH' if bad == 0 else f'{bad} diffs'} (target {len(tw)} insns, got {len(ow)})")
    sys.exit(0 if bad == 0 else 1)

main()
