#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
openocd -f interface/stlink.cfg -f "$ROOT_DIR/openocd/stm32f103.cfg"
