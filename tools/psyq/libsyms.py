#!/usr/bin/env python3
"""Emit splat symbol_addrs lines for PsyQ library functions found in the EXE.

usage: libsyms.py BIN FILE_START FILE_END VRAM MIN_VRAM [SDK_DIR ...]
Only modules of >= 0x40 bytes, or any module at/after MIN_VRAM, are trusted.
"""
import os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from psyq_obj import read_lib
from psyq_libmatch import masks_for, find_all
import glob

def main():
    path, start, end, vram, minv = sys.argv[1], *(int(x, 0) for x in sys.argv[2:6])
    sdks = sys.argv[6:] or ["/home/snake/dev/psyq_sdk/psyq_4.4"]
    data = open(path, "rb").read()[start:end]
    binw = list(struct.unpack("<%dI" % (len(data) // 4), data))
    out = {}
    for d in sdks:
        for f in sorted(glob.glob(os.path.join(d, "lib", "*.lib"))):
            for m in read_lib(f):
                t = m.obj.section_named(".text")
                if not t or len(t.data) < 8:
                    continue
                w = list(struct.unpack("<%dI" % (len(t.data) // 4), t.data[:len(t.data) // 4 * 4]))
                tid = next(k for k, v in m.obj.sections.items() if v is t)
                for s in find_all(binw, w, masks_for(t), 0, len(binw)):
                    addr = vram + s * 4
                    if len(t.data) < 0x40 and addr < minv:
                        continue
                    for _, sec, off, name in m.obj.xdefs:
                        if sec == tid:
                            out.setdefault(addr + off, name)
    names = {}
    for a in sorted(out):
        n = out[a]
        if n in names.values():
            continue
        names[a] = n
        print(f"{n} = 0x{a:08X}; // type:func")

main()
