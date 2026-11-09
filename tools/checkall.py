#!/usr/bin/env python3
"""Run fdiff on every C-defined function that has target asm. usage: tools/checkall.py [src files...]"""
import glob, os, re, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
files = sys.argv[1:] or sorted(glob.glob(os.path.join(ROOT, "src/game/*.c")))
syms = sum((["--syms", f] for f in glob.glob(os.path.join(ROOT, "syms/*.txt"))), [])
bad = 0; n = 0
for c in files:
    src = open(c).read()
    rel = os.path.relpath(c, os.path.join(ROOT, "src"))[:-2]
    asmdir = os.path.join(ROOT, "asm/nonmatchings", rel)
    for m in re.finditer(r"^\w[\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{", src, re.M):
        fn = m.group(1)
        if fn.startswith("__maspsx") or not os.path.exists(os.path.join(asmdir, fn + ".s")):
            continue
        n += 1
        r = subprocess.run([sys.executable, os.path.join(ROOT, "tools/fdiff.py"), c, fn] + syms,
                           capture_output=True, text=True)
        last = (r.stdout.strip().splitlines() or [r.stderr.strip()[-200:]])[-1]
        if "MATCH" not in last or "diffs" in last:
            bad += 1; print("FAIL", os.path.basename(c), last)
print(f"{n - bad}/{n} match")
