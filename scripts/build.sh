#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
BUILD_TYPE="${BUILD_TYPE:-Debug}"

GENERATOR_ARGS=()
if [[ ! -f "$BUILD_DIR/CMakeCache.txt" ]]; then
  GENERATOR_ARGS=(-G Ninja)
fi

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" \
  "${GENERATOR_ARGS[@]}" \
  -DCMAKE_TOOLCHAIN_FILE="$ROOT_DIR/cmake/arm-none-eabi-gcc.cmake" \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

cmake --build "$BUILD_DIR"
