#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ELF="$ROOT_DIR/build/bh314_stm32f103rct8.elf"

if [[ ! -f "$ELF" ]]; then
  echo "ELF not found. Run ./scripts/build.sh first."
  exit 1
fi

openocd \
  -f interface/stlink.cfg \
  -f "$ROOT_DIR/openocd/stm32f103.cfg" \
  -c "program $ELF verify reset exit"
