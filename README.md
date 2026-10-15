# BH314 STM32F103RCT8 CMake + GCC Port

โปรเจกต์นี้แปลงจาก `BH314.ino` เป็นโปรเจกต์ bare-metal C สำหรับใช้งานบน Linux + VSCode โดยไม่ใช้ Arduino และไม่ใช้ PlatformIO

## Target

- MCU: STM32F103RCT8
- Core/register layout: STM32F103 / compatible F103 register layout
- CPU: Arm Cortex-M3
- Package: LQFP64 / R-package เพราะโค้ดเดิมใช้ PC0-PC12, PC13 และ PD2
- Flash/RAM: 256 KB Flash, 48 KB RAM
- System clock: 72 MHz from HSE 8 MHz + PLL x9
- Build system: CMake + Ninja
- Compiler: arm-none-eabi-gcc
- Flash: STM32 ROM serial bootloader via USART1
- Debug/recovery: OpenOCD + ST-Link/CMSIS-DAP
- Default UART: USART1 PA9/PA10, 2400 bps
- RS485 DIR: PA8
- Peripheral access: direct register BSP, see `docs/REGISTER_DIRECT_POLICY.md`

> หมายเหตุ: โค้ดเดิมมี comment ว่า `stm32f103rct6` และใช้ขา 64-pin หลายตำแหน่ง ดังนั้นไม่ควรใช้กับ C8T6/LQFP48 โดยตรง

## Build on Linux

```bash
sudo apt update
sudo apt install -y build-essential git cmake ninja-build openocd gdb-multiarch gcc-arm-none-eabi binutils-arm-none-eabi stm32flash

./scripts/build.sh
```

ค่า default ของ build script คือ `Debug` และใช้ `-Og` เพื่อให้ debug ได้แต่ยังเหมาะกับ interrupt/timing มากกว่า `-O0`. ถ้าต้องการ build ใช้งานจริงให้ใช้:

```bash
BUILD_TYPE=Release ./scripts/build.sh
```

## Flash

ค่า default ใช้ serial bootloader ของ STM32F103 ผ่าน USART1:

```bash
./scripts/flash.sh
```

ค่า default คือ `/dev/ttyUSB0` และ 115200 bps สามารถเปลี่ยนได้:

```bash
SERIAL_PORT=/dev/ttyUSB1 SERIAL_BAUD=57600 ./scripts/flash.sh
```

ก่อน flash ให้ตั้ง `BOOT0=1`, `BOOT1=0` แล้ว reset บอร์ดเพื่อเข้า ROM bootloader หลัง flash เสร็จให้ตั้ง `BOOT0=0` แล้ว reset เพื่อรัน firmware

การ flash ผ่าน ROM bootloader ต้องต่อ USB-TTL เข้าขา MCU โดยตรง: adapter TX ไป PA10, adapter RX ไป PA9 และ GND ร่วมกัน. ถ้าบอร์ดต่อ PA9/PA10 ผ่านวงจร RS485 อย่างเดียว การ flash ผ่านสาย A/B มักใช้ไม่ได้ เพราะ ROM bootloader ไม่รู้จักควบคุมขา `DIR`/DE ที่ PA8.

ฮาร์ดแวร์บอร์ดนี้มี pull-up 4.7k ที่ `DIR`/PA8 และใน firmware กำหนด `DIR HIGH = RS485 TX`, `DIR LOW = RS485 RX`. ตอนอยู่ ROM bootloader ขานี้จึงถูกดึงไปทาง TX โดย default ทำให้ transceiver อาจรับคำสั่งจากเครื่องโปรแกรมไม่ได้. ถ้าจะ flash ผ่าน RS485 จริง ต้องมีวงจรบังคับ direction ตอน bootloader หรือ auto-direction ภายนอก.

ถ้าต้องใช้ ST-Link:

```bash
./scripts/flash-stlink.sh
```

ลบ flash ผ่าน serial bootloader:

```bash
./scripts/erase.sh
```

## VSCode

เปิดโฟลเดอร์นี้ด้วย VSCode แล้วใช้ tasks:

- `BH314 Build`
- `BH314 Flash`
- `BH314 Clean`

Debug ใช้ Cortex-Debug extension และ config `Debug STM32F103RCT8`

## Important SWD warning

โค้ดเดิมใช้:

- PA13 = OP3
- PA14 = IN3

สองขานี้เป็น SWDIO/SWCLK หลัง reset ถ้าต้องการให้ hardware ทำงานครบตามของเดิม ต้องตั้งค่า:

```c
#define BH314_USE_SWD_PINS_AS_GPIO 1
```

ใน `board/board_config.h` ค่า default ของโปรเจกต์ตอนนี้เป็น `1` เพื่อให้พฤติกรรมตรงกับ hardware เดิมครบทุกขา. เมื่อ firmware เริ่มทำงานจะ disable SWD หลัง boot ถ้าต้อง debug ผ่าน SWD ให้เปลี่ยนค่านี้เป็น `0` ชั่วคราว หรือใช้ `connect under reset`

## Ported files

- `app/bh314.c` logic หลักที่แปลงจาก Arduino
- `app/bh314_platform.*` platform adapter สำหรับ GPIO, UART และ watchdog
- `bsp/bsp_uart.*` USART1 polling driver
- `bsp/bsp_time.*` SysTick 1ms และเรียก timer task ทุก 2ms
- `bsp/bsp_watchdog.*` IWDG driver
- `original/BH314.ino` ไฟล์ต้นฉบับ

## Register-direct policy

โปรเจกต์นี้เลือกใช้ register โดยตรงผ่าน BSP แทน CMSIS device package, HAL, LL หรือ vendor peripheral library เพื่อให้ firmware เบาและ timing ชัด. โค้ดระดับ application ไม่ควร include `stm32f103_compat.h` หรือแตะ register เช่น `GPIOA`, `USART1`, `RCM` โดยตรง. รายละเอียดอยู่ใน `docs/REGISTER_DIRECT_POLICY.md`

## What was converted

Arduino API เดิมถูกแทนด้วย BSP:

| Arduino | Bare-metal port |
|---|---|
| `setup()` | `bh314_init()` |
| `loop()` | `bh314_task()` |
| `STM32Timer TIM1 every 2ms` | `SysTick_Handler()` เรียก `bh314_timer_tick_2ms()` |
| `pinMode()` | `bh_gpio_config()` |
| `digitalWrite()` | `bh_gpio_write()` |
| `digitalRead()` | `bh_gpio_read()` |
| `Serial.begin(2400)` | `bsp_uart1_init(2400)` |
| `Serial.available/read` | `bsp_uart1_available/read_byte` |
| `Serial.printf("\\n:%02X%02X\\r")` | `bh_serial_send_key()` |
| `IWatchdog` | `bsp_watchdog` |

## First bring-up checklist

1. ใช้ค่า default `BH314_USE_SWD_PINS_AS_GPIO=1` เพื่อให้ IN3/OP3 ทำงานเหมือน hardware เดิม
2. Build ให้ผ่าน
3. Flash ด้วย serial bootloader ผ่าน USB-TTL ที่ต่อเข้า USART1 PA9/PA10 โดยตรง
4. ตั้ง `BOOT0=0` แล้ว reset เพื่อรัน firmware
5. ตรวจ USART1 application baud 2400 ที่ PA9/PA10
6. ตรวจ RS485 DIR ที่ PA8
7. ถ้าต้อง debug ผ่าน SWD ให้เปลี่ยน `BH314_USE_SWD_PINS_AS_GPIO` เป็น `0` ชั่วคราว จะทำให้ IN3/OP3 ไม่ตรง behavior เต็มระบบ
8. ทดสอบ input/output ทีละชุดก่อนเปิดใช้งานทั้งระบบ

## Known limitations

- ยังเป็น direct-register bring-up ไม่ได้ใช้ Geehy SDK
- Clock ใช้ HSE 8MHz + PLL x9 เป็น SYSCLK 72MHz
- ถ้า HSE/PLL ไม่พร้อมตอน boot firmware จะ fallback เป็น HSI 8MHz และ BSP จะคำนวณ SysTick/UART จาก clock จริงนั้น ควรถือว่าเป็น hardware/clock fault ที่ต้องตรวจ crystal/load capacitor
- ค่า default ใช้ PA13/PA14 เป็น GPIO เพื่อให้ behavior ตรงกับ hardware เดิม จึงกระทบ SWD debug

## สารบัญโปรเจคย่อย

ดู [รายการโปรเจคย่อยและ README](README_SUBPROJECTS.md) สำหรับไฟล์โครงการที่ตรวจพบในกลุ่มนี้ พร้อม [ผลสำรวจและข้อจำกัดของข้อมูล](../README_COVERAGE.md)

<!-- WORK-INDEX:START -->
## สารบัญอัตโนมัติ

ส่วนนี้อัปเดตจากโฟลเดอร์จริงโดย `update_indexes.py`

- [สารบัญโฟลเดอร์ย่อยที่มี README](README_SUBPROJECTS.md)
<!-- WORK-INDEX:END -->
