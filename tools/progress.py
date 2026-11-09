#!/usr/bin/env python3
"""Matched (C) vs INCLUDE_ASM functions per game object, from the built .o files (run make first)."""
import glob, os, subprocess
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NM = os.path.join(ROOT, "tools/binutils/usr/bin/mipsel-linux-gnu-nm")
env = dict(os.environ, LD_LIBRARY_PATH=os.path.join(ROOT, "tools/binutils/usr/lib/x86_64-linux-gnu"))
T = [0, 0, 0, 0]
for o in sorted(glob.glob(os.path.join(ROOT, "build/src/**/*.c.o"), recursive=True)):
    out = subprocess.run([NM, "-S", "--defined-only", o], env=env, capture_output=True, text=True).stdout
    funcs, nonm = {}, set()
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in "Tt":
            if p[3].endswith(".NON_MATCHING"):
                nonm.add(p[3][:-13])
            elif not p[3].startswith("__maspsx"):
                funcs[p[3]] = int(p[1], 16)
    m = {k: v for k, v in funcs.items() if k not in nonm}
    r = [len(m), len(funcs), sum(m.values()), sum(funcs.values())]
    T = [a + b for a, b in zip(T, r)]
    name = os.path.relpath(o, os.path.join(ROOT, "build/src"))[:-4]
    print(f"{name:20s} {r[0]:4d}/{r[1]:<4d} funcs  {r[2]:#8x}/{r[3]:#8x} bytes ({100*r[2]/max(r[3],1):5.2f}%)")
print(f"{'total':20s} {T[0]:4d}/{T[1]:<4d} funcs  {T[2]:#8x}/{T[3]:#8x} bytes ({100*T[2]/max(T[3],1):5.2f}%)")
