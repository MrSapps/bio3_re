#!/usr/bin/env python3
"""Patch linker-filler bytes into a built binary: tools/fixup.py BIN FIXUP

FIXUP lines are "offset value" (hex); '#' starts a comment. Only for alignment padding whose
content the original linker left as garbage (see tools/mkovl.py), never for code or data.
"""
import sys
path, fix = sys.argv[1], sys.argv[2]
b = bytearray(open(path, "rb").read())
for line in open(fix):
    line = line.split("#")[0].split()
    if line:
        b[int(line[0], 16)] = int(line[1], 16)
open(path, "wb").write(b)
