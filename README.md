# Resident Evil 3: Nemesis (USA) — SLUS_009.23

Splat-based disassembly/decomp setup. Game data is **not** in the repo.

## Setup
```sh
# 1. disc: extract the .7z into disc/, then
python3 -I tools/bin2iso.py "disc/Resident Evil 3 - Nemesis (USA)/Resident Evil 3 - Nemesis (USA).bin" disc/re3.iso
.venv/bin/python -I tools/extract_iso.py disc/re3.iso disc/files
mkdir -p iso && cp disc/files/SLUS_009.23 iso/ && cp -r disc/files/CD_DATA/BIN iso/BIN
# 2. python env
python3 -m venv .venv && .venv/bin/pip install "splat64[mips]" pycdlib
# 3. binutils (no root needed)
(mkdir -p tools/deb && cd tools/deb && apt-get download binutils-mipsel-linux-gnu && dpkg-deb -x *.deb ../binutils)
# 4. split + build + verify
make split && make
```
`make` recipes run under `ulimit -v 1048576` (1 GiB cap).

See NOTES.md for memory map and compiler findings.
