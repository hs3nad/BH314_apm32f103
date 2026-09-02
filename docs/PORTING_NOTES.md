# Porting Notes

## เปลี่ยนขา Speaker/Buzzer

ขา speaker/buzzer ของ BH314 ถูกกำหนดใน `app/bh314.c` ตามต้นฉบับ:

```c
#define SPK BH_PIN_PC13
```

ถ้าจะเปลี่ยนขานี้ต้องตรวจวงจรขับภายนอกด้วย เพราะ PC13 บน STM32F103 drive current ต่ำกว่าขาทั่วไป

## เปลี่ยนขา RS485 DE/RE

แก้:

```c
#define BOARD_RS485_DE_PORT GPIOA
#define BOARD_RS485_DE_PIN  8U
```

## ใช้ HSE/PLL

ค่าเริ่มต้นในโปรเจกต์นี้ตั้ง `SystemInit()` ให้ใช้ HSE 8MHz + PLL x9 เป็น SYSCLK 72MHz

ถ้า HSE/PLL ไม่พร้อมตอน boot โค้ดจะ fallback เป็น HSI 8MHz โดย `g_system_core_clock_hz` จะยังเป็น 8MHz เพื่อให้ SysTick/UART คำนวณ timing จาก clock จริง

## ใช้ Geehy SDK ภายหลัง

ถ้าต้องการใช้ SDK ทางการ ให้เพิ่ม folder SDK เข้า `drivers/` แล้วค่อยปรับ CMakeLists.txt ให้ include header/source ของ SDK แทน direct register บางส่วน
