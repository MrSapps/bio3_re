#!/bin/bash
# permuter compile step: compile.sh IN.c -o OUT.o  (same pipeline as the Makefile, cc1 selectable via PERM_CC1)
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
CC1=${PERM_CC1:-$ROOT/tools/compilers/gcc-2.8.1-psx/cc1}
B=$ROOT/tools/binutils/usr/bin/mipsel-linux-gnu-
export LD_LIBRARY_PATH=$ROOT/tools/binutils/usr/lib/x86_64-linux-gnu
ulimit -v 1048576
IN=$(readlink -f "$1"); OUT=$(readlink -f "$3")
cd "$ROOT"
cpp -P -undef -D__GNUC__=2 -Dmips -D__psx__ -D_LANGUAGE_C -I"$ROOT/include" -I"$ROOT/src" "$IN" 2>/dev/null \
 | "$CC1" -quiet -O2 -G0 -mgas -msoft-float -fno-builtin -funsigned-char -o - 2>/dev/null \
 | python3 "$ROOT/tools/maspsx/maspsx.py" --aspsx-version=2.77 -G0 2>/dev/null \
 | "${B}as" -EL -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -G0 -I"$ROOT/include" -o "$OUT" - 2>/dev/null
