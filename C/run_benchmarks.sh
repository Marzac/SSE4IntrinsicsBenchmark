#!/bin/bash
# Run all benchmark executables

echo "Running benchmarks..."
echo

for BIN in benchmark_O0 benchmark_O2 benchmark_O3; do
  if [[ -x "$BIN" ]]; then
    echo "=== Running $BIN ==="
    ./"$BIN"
    echo
  else
    echo "Executable $BIN not found or not executable!"
  fi
done
