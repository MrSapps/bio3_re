#!/usr/bin/env python3
"""Make a decomp-permuter directory for one function.
usage: tools/permute/setup.py SRC.c FUNC OUTDIR      (SRC.c = the C file containing the current attempt of FUNC,
       with the function body written as C, not INCLUDE_ASM; the file is preprocessed so includes are inlined)
then:  cd ~/dev/decomp-permuter && tools/capped python3 permuter.py OUTDIR -j4 --stop-on-zero
"""
import os, re, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
src, func, out = sys.argv[1:4]
os.makedirs(out, exist_ok=True)
# target object from the nonmatchings asm
rel = os.path.relpath(os.path.abspath(src), os.path.join(ROOT, "src"))[:-2]
asm = os.path.join(ROOT, "asm/nonmatchings", rel, func + ".s")
B = os.path.join(ROOT, "tools/binutils/usr/bin/mipsel-linux-gnu-")
env = dict(os.environ, LD_LIBRARY_PATH=os.path.join(ROOT, "tools/binutils/usr/lib/x86_64-linux-gnu"))
open(out + "/target.s", "w").write('.include "macro.inc"\n.set noreorder\n.set noat\n' + open(asm).read())
subprocess.run([B + "as", "-EL", "-march=r3000", "-msoft-float", "-no-pad-sections", "-G0", "-I" + ROOT + "/include",
                "-o", out + "/target.o", out + "/target.s"], check=True, env=env)
pre = subprocess.run(["cpp", "-P", "-undef", "-D__GNUC__=2", "-Dmips", "-D__psx__", "-D_LANGUAGE_C",
                      "-I" + ROOT + "/include", "-I" + ROOT + "/src", "-I" + os.environ.get("PSYQ_INC", "/home/snake/dev/psyq_sdk/psyq_4.4/include"), src], capture_output=True, text=True, check=True).stdout
def strip_other_functions(text, keep):
    """drop every top-level function definition except `keep` (the permuter recompiles the file thousands of times)"""
    out, i, n, start = [], 0, len(text), 0
    while i < n:
        c = text[i]
        if c == ";":
            out.append(text[start:i + 1]); start = i + 1
        elif c == "{":
            header = text[start:i]
            d, j = 1, i + 1
            while d:
                d += (text[j] == "{") - (text[j] == "}"); j += 1
            hs = header.rstrip()
            is_func = hs.endswith(")") and "=" not in header
            if is_func and not re.search(r"\b" + keep + r"\s*\(", header):
                pass                                    # other function: drop it
            else:
                out.append(text[start:j])
            start = j; i = j; continue
        i += 1
    out.append(text[start:])
    return "".join(out)
pre = strip_other_functions(pre, func)
open(out + "/base.c", "w").write(pre)
open(out + "/compile.sh", "w").write('#!/bin/bash\nexec ' + ROOT + '/tools/permute/compile.sh "$@"\n')
os.chmod(out + "/compile.sh", 0o755)
open(out + "/settings.toml", "w").write(f'func_name = "{func}"\ncompiler_type = "gcc"\nobjdump_command = "{ROOT}/tools/permute/objdump.sh -drz"\n')
print("ready:", out)
