# BH314 Arduino to CMake/GCC Porting Report

## Source

- Original file: `original/BH314.ino`
- Original target comment: `stm32f103rct6`
- Converted target: STM32F103RCT8 bare-metal C

## Main result

The Arduino sketch was split into a production-style firmware project:

- `app/bh314.c`: original business logic and display/input state machine
- `app/bh314_platform.*`: small board/platform abstraction for GPIO, serial, and watchdog calls
- `bsp/`: UART, GPIO, SysTick, watchdog
- `board/board_config.h`: target board and SWD pin policy

## Pin map carried from original sketch

### Input pins

| Signal | Pin |
|---|---|
| IN1 | PC6 |
| IN2 | PC8 |
| IN3 | PA14 |
| IN4 | PA12 |
| IN5 | PB10 |
| IN6 | PB0 |
| IN7 | PB12 |
| IN8 | PB14 |
| IN9 | PA15 |
| IN10 | PC10 |
| IN11 | PC12 |
| IN12 | PC5 |
| IN13 | PA7 |
| IN14 | PA4 |
| IN15 | PA6 |
| IN16 | PB4 |
| IN17 | PB7 |
| IN18 | PB5 |

### Output pins

| Signal | Pin |
|---|---|
| OP1 | PC7 |
| OP2 | PC9 |
| OP3 | PA13 |
| OP4 | PA11 |
| OP5 | PB11 |
| OP6 | PB1 |
| OP7 | PB13 |
| OP8 | PB15 |
| OP9 | PB3 |
| OP10 | PD2 |
| OP11 | PC11 |
| OP12 | PC4 |
| OP13 | PA2 |
| OP14 | PA3 |
| OP15 | PA5 |
| OP16 | PB9 |
| OP17 | PB8 |
| OP18 | PB6 |

### Display / serial / speaker

| Signal | Pin |
|---|---|
| A00 | PC0 |
| A01 | PC1 |
| A02 | PC2 |
| DO | PA0 |
| CLK | PC3 |
| RCK | PA1 |
| DIR | PA8 |
| SPK | PC13 |

## Critical notes

1. The code requires a 64-pin package. STM32F103RCT8/LQFP64 exposes the needed PC pins; C8T6/LQFP48 does not.
2. PA13/PA14 are SWD pins. The default firmware disables SWD after boot so IN3/OP3 match the original hardware.
3. UART is polling-based first; interrupt/DMA can be added later.
4. Timer behavior is mapped to SysTick 1ms with 2ms callback.
5. Watchdog is mapped to IWDG with nominal LSI timing.

## Validation already done in this conversion

- Host C syntax check using GCC for all `.c` files
- Removed Arduino, STM32TimerInterrupt, IWatchdog and HAL dependency
- Replaced Serial API with BSP UART API
- Replaced GPIO APIs with compatibility wrapper
- Added Linux/VSCode/CMake project files

## Validation still required on real hardware

- Build with `arm-none-eabi-gcc`
- Flash by STM32 ROM serial bootloader on direct TTL USART1 PA9/PA10
- Confirm USART1 baud 2400 on PA9/PA10
- Confirm RS485 direction PA8 timing
- Confirm display scanning pins
- Confirm debounce timing under real clock
- Confirm watchdog timeout and reload behavior
