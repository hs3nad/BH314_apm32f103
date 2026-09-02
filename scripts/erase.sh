#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SERIAL_PORT="${SERIAL_PORT:-/dev/ttyUSB0}"
SERIAL_BAUD="${SERIAL_BAUD:-115200}"

if ! command -v stm32flash >/dev/null 2>&1; then
  echo "stm32flash not found. Install it first, for example: sudo apt install stm32flash"
  exit 1
fi

echo "Serial erase via $SERIAL_PORT at $SERIAL_BAUD bps"
echo "Connect USB-TTL directly to MCU USART1: adapter TX -> PA10, adapter RX -> PA9, GND -> GND."
echo "Do not erase through the RS485 A/B bus unless the board has hardware bootloader direction control."
echo "This board has a 4.7k pull-up on DIR/PA8; with DIR HIGH = RS485 TX, ROM bootloader cannot switch the transceiver to RX."
echo "Set BOOT0=1, BOOT1=0, reset the board, then keep it in bootloader mode."

stm32flash \
  -b "$SERIAL_BAUD" \
  -o \
  "$SERIAL_PORT"
