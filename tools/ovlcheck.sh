#!/bin/bash
# Build one code overlay and compare it with the original: tools/ovlcheck.sh title
# Only touches build/ovl/<name>.* and that overlay's objects, so different overlays can be checked in parallel.
set -e
cd "$(dirname "$0")/.."
name=$1
make -s "build/ovl/$name.bin" >/dev/null
want=$(grep " $name.bin\$" ovl/overlays.sha1 | cut -d' ' -f1)
got=$(sha1sum "build/ovl/$name.bin" | cut -d' ' -f1)
if [ "$want" = "$got" ]; then echo "$name.bin: OK"; else
    echo "$name.bin: MISMATCH"
    cmp -l "iso/BIN/${name^^}.BIN" "build/ovl/$name.bin" | head -5
    exit 1
fi
