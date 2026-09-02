#include "bsp_watchdog.h"
#include "stm32f103_compat.h"

/* เริ่มต้น independent watchdog โดยคำนวณ reload จาก timeout_ms */
void bsp_watchdog_init(uint32_t timeout_ms)
{
    /* IWDG/LSI nominal 40 kHz. Prescaler 256 => 156.25 ticks/s.
       Max reload 4095 => about 26 s. Arduino code used 8 s. */
    uint32_t reload = (timeout_ms * 40UL) / 256UL;
    if (reload == 0U) {
        reload = 1U;
    }
    if (reload > 0x0FFFU) {
        reload = 0x0FFFU;
    }

    IWDG->KR = 0x5555U;
    IWDG->PR = 0x06U; /* /256 */
    IWDG->RLR = reload;
    IWDG->KR = 0xAAAAU;
    IWDG->KR = 0xCCCCU;
}

/* reload watchdog เพื่อป้องกันการ reset */
void bsp_watchdog_reload(void)
{
    IWDG->KR = 0xAAAAU;
}
