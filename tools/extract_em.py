#!/usr/bin/env python3
"""Extract the enemy code modules from the room archives (CD_DATA/STAGEn/R*.BIN) into iso/EM/.

usage: tools/extract_em.py [disc/files/CD_DATA]
Room archive: word 0 total size, word 1 entry count, then (size, info) pairs; entries start at 0x800 and
are sector (0x800) aligned. info >> 24 is the enemy id, info & 0xFFFF the type; types 1 and 2 are the
enemy's code module linked for load slot 1 (0x80100000) and slot 2 (0x8010D000). Enemy ids that share code
carry byte-identical modules; each distinct module is written once as em<lowest id>_<slot>.bin and the
id -> module map goes to iso/EM/modules.txt.
"""
import glob, hashlib, os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def entries(path):
    d = open(path, "rb").read()
    n = struct.unpack_from("<I", d, 4)[0]
    off = 0x800
    for i in range(n):
        size, info = struct.unpack_from("<II", d, 8 + i * 8)
        yield info, d[off:off + size]
        off += (size + 0x7FF) & ~0x7FF

def main():
    src = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "disc/files/CD_DATA")
    mods = {}  # sha1 -> (slot, ids, data)
    for p in sorted(glob.glob(os.path.join(src, "STAGE*/R*.BIN"))):
        for info, data in entries(p):
            slot = info & 0xFFFF
            if slot not in (1, 2) or b"\x08\x00\xe0\x03" not in data:
                continue
            h = hashlib.sha1(data).hexdigest()
            mods.setdefault(h, (slot, set(), data))[1].add(info >> 24)
    out = os.path.join(ROOT, "iso/EM")
    os.makedirs(out, exist_ok=True)
    lines = []
    for h, (slot, ids, data) in sorted(mods.items(), key=lambda x: (min(x[1][1]), x[1][0])):
        name = f"em{min(ids):02x}_{slot}"
        open(os.path.join(out, name + ".bin"), "wb").write(data)
        lines.append(f"{name} {h} ids={','.join(f'{i:02x}' for i in sorted(ids))} size={len(data):#x}")
    open(os.path.join(out, "modules.txt"), "w").write("\n".join(lines) + "\n")
    print("\n".join(lines))

main()
