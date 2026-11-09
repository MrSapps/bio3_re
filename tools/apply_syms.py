#!/usr/bin/env python3
"""Merge syms/*.txt name suggestions into symbol_addrs.txt and rename func_/D_ uses in src/ and include/.

usage: tools/apply_syms.py [--dry-run]
Afterwards run `rm -rf asm && make split && make`.
"""
import glob, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
dry = "--dry-run" in sys.argv
LINE = re.compile(r"\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;(.*)")

existing = {}            # addr -> name
names = {}               # name -> addr
for line in open(os.path.join(ROOT, "symbol_addrs.txt")):
    m = LINE.match(line)
    if m:
        a = int(m.group(2), 16)
        existing[a] = m.group(1); names[m.group(1)] = a

new, problems = [], []
for f in sorted(glob.glob(os.path.join(ROOT, "syms", "*.txt"))):
    for line in open(f):
        m = LINE.match(line)
        if not m:
            continue
        name, a, rest = m.group(1), int(m.group(2), 16), m.group(3)
        if re.fullmatch(r"(func|D|jtbl)_[0-9A-F]{8}", name):
            continue
        if names.get(name) == a:
            continue
        if name in names:
            problems.append(f"{os.path.basename(f)}: {name} already = {names[name]:#x}, not {a:#x}")
            continue
        if a in existing:
            problems.append(f"{os.path.basename(f)}: {a:#x} already named {existing[a]}, skipping {name}")
            continue
        t = re.search(r"type:(\w+)", rest)
        size = re.search(r"size:(0x[0-9A-Fa-f]+)", rest)
        attrs = (f" type:{t.group(1)}" if t else "") + (f" size:{size.group(1)}" if size else "")
        new.append((a, name, attrs, os.path.basename(f)))
        existing[a] = name; names[name] = a

for p in problems:
    print("CONFLICT", p)
print(f"{len(new)} new symbols")
if dry:
    for a, n, t, f in new:
        print(f"  {n} = {a:#010x};{t}  ({f})")
    sys.exit(0)

with open(os.path.join(ROOT, "symbol_addrs.txt"), "a") as out:
    out.write("\n// merged from syms/*.txt\n")
    for a, n, t, f in sorted(new):
        out.write(f"{n} = 0x{a:08X};" + (f" //{t}" if t else "") + "\n")

ren = {}
for a, n, t, f in new:
    ren[f"func_{a:08X}"] = n
    ren[f"D_{a:08X}"] = n
pat = re.compile(r"\b(?:func|D)_[0-9A-F]{8}\b")
for f in glob.glob(os.path.join(ROOT, "src", "**", "*.c"), recursive=True) + \
         glob.glob(os.path.join(ROOT, "include", "**", "*.h"), recursive=True):
    s = open(f).read()
    s2 = pat.sub(lambda m: ren.get(m.group(0), m.group(0)), s)
    if s2 != s:
        open(f, "w").write(s2)
        print("renamed in", os.path.relpath(f, ROOT))
