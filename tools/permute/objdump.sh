#!/bin/bash
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
export LD_LIBRARY_PATH=$ROOT/tools/binutils/usr/lib/x86_64-linux-gnu
exec "$ROOT/tools/binutils/usr/bin/mipsel-linux-gnu-objdump" "$@"
