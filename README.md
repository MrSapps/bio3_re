# Resident Evil 3: Nemesis (USA) — SLUS_009.23

Splat-based disassembly/decomp setup. Game data is **not** in the repo.

## Setup
```sh
# 1. disc: extract the .7z into disc/, then
python3 -I tools/bin2iso.py "disc/Resident Evil 3 - Nemesis (USA)/Resident Evil 3 - Nemesis (USA).bin" disc/re3.iso
.venv/bin/python -I tools/extract_iso.py disc/re3.iso disc/files
mkdir -p iso && cp disc/files/SLUS_009.23 iso/ && cp -r disc/files/CD_DATA/BIN iso/BIN
# 2. python env
python3 -m venv .venv && .venv/bin/pip install "splat64[mips]" pycdlib pycparser colorama python-Levenshtein cxxfilt
# 3. binutils (no root needed)
(mkdir -p tools/deb && cd tools/deb && apt-get download binutils-mipsel-linux-gnu && dpkg-deb -x *.deb ../binutils)
# 4. decomp tools (local copies, gitignored): old-gcc PSX builds, maspsx, m2c, asm-differ
cp -r ../../UnfilteredSyphon/tools/compilers ../../UnfilteredSyphon/tools/{maspsx,m2c,asm-differ} tools/
# maspsx fix: a split load `lw $x,%lo(sym)($y)` was treated as an $at macro (no load-delay nop)
(cd tools/maspsx && patch -p1 < ../maspsx-lo-load.patch)
# WARNING.BIN's own objects need decompals old-gcc gcc-2.7.2-cdk (release 0.14) in tools/compilers/gcc-2.7.2-cdk
# 5. split + build + verify
make split && make
```
The C build also needs the PsyQ 4.4 SDK headers (not in the repo): `PSYQ_INC` defaults to
`/home/snake/dev/psyq_sdk/psyq_4.4/include`.

## Matching a function
```sh
.venv/bin/python tools/m2c/m2c.py --target mipsel-gcc-c asm/nonmatchings/game/<file>/<func>.s   # draft
# replace the INCLUDE_ASM line in src/game/<file>.c, then
python3 tools/fdiff.py src/game/<file>.c <func> [-v]     # per-function compare incl. relocations
make                                                      # full relink + sha1
```
`make` recipes run under `ulimit -v 1048576` (1 GiB cap).

See NOTES.md for memory map and compiler findings.
