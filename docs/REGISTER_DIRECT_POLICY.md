# Register Direct Policy

โปรเจกต์ BH314 นี้เลือกแนวทาง **ควบคุม register โดยตรง** ผ่าน BSP ของโปรเจกต์เอง แทนการใช้ CMSIS device package, STM32 HAL, STM32 LL หรือ vendor peripheral library ใน path หลักของ firmware

เป้าหมายคือให้ firmware เบา, timing ชัด, dependency น้อย และ debug ได้ตรงไปตรงมาบนฮาร์ดแวร์จริง

## Decision

แนวทางที่เลือก:

- ใช้ `arm-none-eabi-gcc` build bare-metal C
- ใช้ startup, linker script และ register map ของโปรเจกต์เอง
- ใช้ `core/stm32f103_compat.h` เป็นแหล่งรวม register address, register struct และ bit definitions
- เขียน peripheral driver ใน `bsp/` โดยแตะ register โดยตรง
- ให้ application logic เรียกผ่าน platform/BSP wrapper เท่านั้น

แนวทางที่ยังไม่ใช้ในตอนนี้:

- CMSIS device header แบบเต็ม
- STM32Cube HAL
- STM32Cube LL
- STM32F10x Standard Peripheral Library
- Geehy/APM32 vendor peripheral library

## Why

เหตุผลที่เลือก register โดยตรงสำหรับโปรเจกต์นี้:

- โค้ดเดิมมาจาก Arduino และใช้ peripheral ไม่มาก
- งานหลักต้องการ timing ค่อนข้างชัด เช่น display scan, debounce, RS485 และ watchdog
- firmware มีขนาดเล็กกว่าและ boot path สั้นกว่า
- ไม่มี dependency ใหญ่ ทำให้ build ด้วย CMake/GCC บน Linux ง่าย
- เวลา debug สามารถไล่ได้ทันทีว่า API แต่ละตัวอ่าน/เขียน register ไหน

ข้อแลกเปลี่ยน:

- ต้องระวัง register bit ให้ถูกเอง
- ตัวอย่างจากอินเทอร์เน็ตที่ใช้ HAL/LL เอามาวางตรง ๆ ไม่ได้
- ถ้าเพิ่ม peripheral ซับซ้อน เช่น USB, CAN, DMA หลาย channel, RTC calendar อาจต้องประเมินใหม่

## Layer Ownership

ไฟล์แต่ละชั้นมีหน้าที่ชัดเจนดังนี้:

| Layer | หน้าที่ | แตะ register ได้ไหม |
|---|---|---|
| `core/` | register map, bit definitions, hardware type | ได้ |
| `board/` | board clock, pin/debug policy, board config | ได้ |
| `bsp/` | peripheral driver เช่น GPIO, UART, RS485, SysTick, watchdog | ได้ |
| `app/bh314_platform.c` | adapter ระหว่าง BH314 logic กับ BSP/board | เรียก BSP/board ได้ |
| `app/bh314.c` | application logic ที่ port จาก Arduino | ไม่ควร |
| `app/app.c` | application entry wrapper | ไม่ควร |
| `app/main.c` | main loop หลัง board/app init | ไม่ควร |
| `protocol/` | pure protocol/data logic | ไม่ควร |

## Allowed Register Access

อนุญาตให้ include `stm32f103_compat.h` และเรียก register macro เช่น `GPIOA`, `USART1`, `RCM`, `AFIO`, `IWDG`, `SYSTICK` ในไฟล์กลุ่มนี้:

- `core/*`
- `board/*`
- `bsp/*`
- startup หรือ low-level system file ที่จำเป็น

`app/bh314_platform.c` เป็น boundary adapter จึงอนุญาตให้เรียก BSP และ board configuration ได้ แต่ควรหลีกเลี่ยงการเขียน register โดยตรงถ้า BSP มี API รองรับอยู่แล้ว

## Not Allowed In Application Logic

ไฟล์ application logic เช่น `app/bh314.c`, `app/app.c`, `app/main.c` และ `protocol/*` ไม่ควร:

- include `stm32f103_compat.h`
- include vendor HAL/LL header
- เรียก `GPIOA`, `USART1`, `RCM`, `IWDG`, `AFIO`, `SYSTICK` โดยตรง
- ตั้งค่า peripheral register เอง
- ผูก logic เข้ากับขา MCU โดยตรงถ้า platform wrapper ทำแทนได้

ถ้าจำเป็นต้องทำ ให้ย้ายโค้ดลง `bsp/`, `board/` หรือ `app/bh314_platform.c`

## Adding New Peripherals

เมื่อเพิ่ม peripheral ใหม่ ให้ทำตามลำดับนี้:

1. เพิ่ม register/bit ที่ขาดใน `core/stm32f103_compat.h`
2. เพิ่ม driver ใหม่ใน `bsp/` เช่น `bsp_adc.c`, `bsp_timer.c`, `bsp_flash.c`
3. เพิ่ม header ของ driver ใน `bsp/`
4. เพิ่มไฟล์ source ใน `CMakeLists.txt`
5. ถ้า application ต้องใช้ ให้เพิ่ม wrapper ใน `app/bh314_platform.c/.h`
6. ให้ `app/bh314.c` เรียก wrapper ไม่เรียก BSP/register ตรง

ตัวอย่าง:

```c
/* app/bh314.c */
bh_gpio_write(OP1, BH_GPIO_HIGH);

/* app/bh314_platform.c */
void bh_gpio_write(uint32_t encoded_pin, uint32_t value)
{
    bsp_gpio_write(pin_port(encoded_pin), pin_no(encoded_pin), value);
}

/* bsp/bsp_gpio.c */
void bsp_gpio_write(GPIO_TypeDef *port, uint32_t pin, uint32_t value)
{
    port->BSC = (1UL << pin);
}
```

## Coding Rules

- ใช้ชื่อ register/bit ที่นิยามใน `core/stm32f103_compat.h`
- อย่าใช้ magic number ใน BSP ถ้ามันเป็น bit/register ที่ควรตั้งชื่อได้
- ถ้าต้องใช้ magic value ตาม datasheet ให้ใส่ comment สั้น ๆ
- ใช้ `volatile` เฉพาะกับ memory-mapped register หรือ state ที่ใช้ร่วมกับ interrupt
- งานที่แชร์ข้อมูลกับ interrupt ต้องมี critical section หรือรูปแบบ update ที่ atomic พอ
- หลีกเลี่ยง `printf`, `sprintf`, heap allocation และ library หนักใน firmware path
- ให้ BSP API เล็กและตรงกับ hardware behavior จริง

## Review Checklist

ก่อน merge หรือ flash รอบสำคัญ ให้เช็ค:

- `app/bh314.c`, `app/app.c`, `app/main.c` ไม่ include `stm32f103_compat.h`
- `protocol/` ไม่มี hardware dependency
- peripheral ใหม่มี driver อยู่ใน `bsp/`
- register/bit ใหม่ถูกเพิ่มใน `core/`
- build ผ่านด้วย `./scripts/build.sh`
- ถ้าแตะ timing/interrupt ให้ทบทวน interaction กับ `SysTick_Handler`

## When To Reconsider

อาจพิจารณาใช้ CMSIS/HAL/LL หรือ vendor library ในอนาคต ถ้า:

- ต้องใช้ peripheral ซับซ้อนมากขึ้น
- ต้อง reuse ตัวอย่าง vendor จำนวนมาก
- ต้อง port ไป MCU หลายรุ่นในตระกูลเดียวกัน
- ต้องการ middleware เช่น USB stack, file system, RTOS integration

จนกว่าจะถึงจุดนั้น path หลักของโปรเจกต์นี้ให้ยึด **register โดยตรงผ่าน BSP** เป็นมาตรฐาน
