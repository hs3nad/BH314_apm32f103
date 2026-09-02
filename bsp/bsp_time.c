#include "bsp_time.h"
#include "stm32f103_compat.h"
#include "board_config.h"
#include "bh314.h"

static volatile uint32_t g_ms;
static volatile uint8_t g_2ms_div;

/* เริ่มต้น SysTick ให้ interrupt ทุก 1 ms และสร้าง tick 2 ms ให้ BH314 */
void bsp_time_init(void)
{
    g_ms = 0;
    g_2ms_div = 0;
    SYSTICK_LOAD = (g_system_core_clock_hz / 1000UL) - 1UL;
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = SYSTICK_CTRL_CLKSOURCE | SYSTICK_CTRL_TICKINT | SYSTICK_CTRL_ENABLE;
}

/* คืนค่าเวลาที่ผ่านไปเป็น millisecond ตั้งแต่เริ่มระบบ */
uint32_t bsp_millis(void)
{
    return g_ms;
}

/* interrupt ของ SysTick: เพิ่ม millisecond และเรียกงาน BH314 ทุก 2 ms */
void SysTick_Handler(void)
{
    g_ms++;
    g_2ms_div++;
    if (g_2ms_div >= 2U) {
        g_2ms_div = 0U;
        bh314_timer_tick_2ms();
    }
}
