# Wiring

## SWD Debug/Flash

| Debug Probe | STM32F103RCT8 |
|---|---|
| 3.3V | 3.3V |
| GND | GND |
| SWDIO | PA13 |
| SWCLK | PA14 |
| NRST | NRST |

## USART1

| Function | Pin |
|---|---|
| TX | PA9 |
| RX | PA10 |

Default UART: 2400 8N1

For STM32 ROM serial bootloader flashing, connect USB-TTL directly to these MCU pins:

| USB-TTL adapter | STM32F103RCT8 |
|---|---|
| TX | PA10 |
| RX | PA9 |
| GND | GND |

Do not rely on the RS485 A/B bus for ROM bootloader flashing unless the PCB provides automatic bootloader-time DE/RE direction control.

This board has a 4.7k pull-up on DIR/PA8. The application treats DIR HIGH as RS485 TX and DIR LOW as RS485 RX, so the ROM bootloader will see the RS485 transceiver biased toward TX unless external hardware overrides it.

## Speaker

Speaker/Buzzer output: PC13

## RS485

Default DE/RE: PA8
DIR/PA8 external bias: 4.7k pull-up.

แก้ได้ที่ `board/board_config.h`
