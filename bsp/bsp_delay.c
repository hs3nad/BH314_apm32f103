#include "bsp_delay.h"
#include "board_config.h"

/* หน่วงเวลาแบบนับ cycle ด้วยคำสั่ง nop ใช้สำหรับงาน bring-up ง่าย ๆ */
void bsp_delay_cycles(volatile uint32_t cycles)
{
    while (cycles--) {
        __asm volatile ("nop");
    }
}

/* หน่วงเวลาเป็น millisecond แบบ blocking */
void bsp_delay_ms(uint32_t ms)
{
    /* Simple blocking delay for early bring-up. Replace with SysTick later. */
    const uint32_t cycles_per_ms = g_system_core_clock_hz / 6000UL;
    while (ms--) {
        bsp_delay_cycles(cycles_per_ms);
    }
}
