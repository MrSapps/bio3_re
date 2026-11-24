#!/usr/bin/env python3
"""Fast scorer for one function in a dir made by setup.py (~0.1 s per try, no relocation check: confirm with fdiff.py).
usage: tools/permute/score.py DIR [FUNC.c ...] [-v]
FUNC.c holds only the new definition of the function; it replaces the one in DIR/base.c (the rest of base.c is context).
Without FUNC.c the base.c itself is scored. Prints 'N FILE' (N = differing instruction words, 0 = match)."""
import os, re, struct, subprocess, sys, tempfile
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
B = ROOT + "/tools/binutils/usr/bin/mipsel-linux-gnu-"
ENV = dict(os.environ, LD_LIBRARY_PATH=ROOT + "/tools/binutils/usr/lib/x86_64-linux-gnu")

def words(o, func):
    """instruction words of FUNC with relocated fields masked (addends differ between D_ names and sym+off)"""
    raw = subprocess.run([B + "objdump", "-dr", o], capture_output=True, text=True, env=ENV).stdout
    res, txt, on = [], [], False
    for l in raw.splitlines():
        if re.match(r"[0-9a-f]+ <(.+)>:", l):
            on = l.split("<")[1].rstrip(">:") == func
            continue
        if not on: continue
        m = re.match(r"\s+[0-9a-f]+:\t([0-9a-f]{8}) \t(.*)", l)
        if m:
            res.append(int(m.group(1), 16)); txt.append(m.group(2)); continue
        r = re.match(r"\s+[0-9a-f]+: (R_MIPS_\w+)\s+(\S+)", l)
        if r and res:
            res[-1] &= 0xFC000000 if r.group(1) == "R_MIPS_26" else 0xFFFF0000
            txt[-1] += "  <" + r.group(2) + ">"
    return res, txt

def find_func(text, func):
    m = re.search(r"^[^\n;{}]*\b" + re.escape(func) + r"\s*\([^;{]*\)\s*\{", text, re.M)
    if not m: return None
    i = text.index("{", m.start()); d = 0
    for j in range(i, len(text)):
        d += (text[j] == "{") - (text[j] == "}")
        if d == 0: return m.start(), j + 1

def func_name(d):
    return re.search(r'func_name = "(\w+)"', open(d + "/settings.toml").read()).group(1)

def score(d, src=None, verbose=False):
    func = func_name(d)
    base = open(d + "/base.c").read()
    if src is not None:
        ab = find_func(base, func)
        base = base[:ab[0]] + src + base[ab[1]:] if ab else base + "\n" + src   # still INCLUDE_ASM: append
    with tempfile.TemporaryDirectory() as t:
        open(t + "/f.c", "w").write(base)
        subprocess.run([ROOT + "/tools/permute/compile.sh", t + "/f.c", "-o", t + "/f.o"], capture_output=True)
        if not os.path.exists(t + "/f.o"): return 9999, []
        got, gtxt = words(t + "/f.o", func)
    tgt, ttxt = words(d + "/target.o", func)
    n = sum(1 for i in range(max(len(tgt), len(got))) if i >= len(tgt) or i >= len(got) or tgt[i] != got[i])
    if verbose:
        for i in range(max(len(tgt), len(got))):
            a = ttxt[i] if i < len(ttxt) else ""; b = gtxt[i] if i < len(gtxt) else ""
            same = i < len(tgt) and i < len(got) and tgt[i] == got[i]
            print("  " if same else "!!", "%-34s %s" % (a, b))
    return n, got

if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if a != "-v"]; v = "-v" in sys.argv
    d = args[0]
    for f in args[1:] or [None]:
        n, _ = score(d, open(f).read() if f else None, v)
        print(n, f or "base.c")
