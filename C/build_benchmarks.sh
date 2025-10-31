#!/bin/bash
# Build benchmark.c with different optimization levels and SSE4 support

SRC="benchmark.c"
OUT_PREFIX="benchmark"
CFLAGS="-std=gnu11 -Wall -Wextra -msse4.1 -s"

# Stop on error
set -e

echo "Building benchmarks..."
echo "Building with -O0..."
gcc $SRC -o ${OUT_PREFIX}_O0 $CFLAGS -O0

echo "Building with -O2..."
gcc $SRC -o ${OUT_PREFIX}_O2 $CFLAGS -O2

echo "Building with -O3..."
gcc $SRC -o ${OUT_PREFIX}_O3 $CFLAGS -O3

echo "All builds complete!"
