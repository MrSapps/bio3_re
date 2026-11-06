#!/usr/bin/env python3
"""Convert a MODE2/2352 PS1 .bin to a 2048-byte/sector ISO (Form1 user data only)."""
import sys
src, dst = sys.argv[1], sys.argv[2]
with open(src, "rb") as f, open(dst, "wb") as o:
    while (s := f.read(2352)):
        o.write(s[24:24 + 2048])
