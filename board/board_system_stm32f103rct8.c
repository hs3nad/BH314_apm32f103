#include "board_config.h"
#include "stm32f103_compat.h"
#include <stdbool.h>

uint32_t g_system_core_clock_hz = BOARD_HSI_HZ;

#if (BOARD_HSE_HZ != 8000000UL)
#error "SystemInit currently supports only an 8 MHz HSE crystal."
#endif

#if (BOARD_SYSCLK_HZ != 72000000UL)
#error "SystemInit currently supports only 72 MHz SYSCLK from HSE 8 MHz x9."
#endif

#define CLOCK_READY_TIMEOUT 1000000UL

/* รอให้ clock ตาม mask พร้อมใช้งาน ภายใน timeout ที่กำหนด */
static bool wait_for_clock_ready(uint32_t mask)
{
    uint32_t timeout = CLOCK_READY_TIMEOUT;
    while ((RCM->CTL & mask) == 0U) {
        if (--timeout == 0U) {
            return false;
        }
    }
    return true;
}

/* ตั้งค่า clock system ให้ใช้ HSE 8 MHz ผ่าน PLL x9 เป็น 72 MHz */
void SystemInit(void)
{
    FLASH_REG->ACR = (FLASH_REG->ACR & ~FLASH_ACR_LATENCY_MASK) |
                     FLASH_ACR_PRFTBE |
                     FLASH_ACR_LATENCY_2WS;

    RCM->CTL |= RCM_CTL_HSEON;
    if (!wait_for_clock_ready(RCM_CTL_HSERDY)) {
        return;
    }

    RCM->CFG &= ~(RCM_CFG_SW_MASK |
                  (0xFUL << 4) |
                  (7UL << 8) |
                  (7UL << 11) |
                  (1UL << 16) |
                  (1UL << 17) |
                  (0xFUL << 18));
    RCM->CFG |= RCM_CFG_HPRE_DIV1 |
                RCM_CFG_PPRE1_DIV2 |
                RCM_CFG_PPRE2_DIV1 |
                RCM_CFG_PLLSRC_HSE |
                RCM_CFG_PLLMULL9;

    RCM->CTL |= RCM_CTL_PLLON;
    if (!wait_for_clock_ready(RCM_CTL_PLLRDY)) {
        return;
    }

    RCM->CFG = (RCM->CFG & ~RCM_CFG_SW_MASK) | RCM_CFG_SW_PLL;
    uint32_t timeout = CLOCK_READY_TIMEOUT;
    while ((RCM->CFG & RCM_CFG_SWS_MASK) != RCM_CFG_SWS_PLL) {
        if (--timeout == 0U) {
            return;
        }
    }
    g_system_core_clock_hz = BOARD_SYSCLK_HZ;
}
