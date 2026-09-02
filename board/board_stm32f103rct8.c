#include "stm32f103_compat.h"
#include "board_config.h"

/* เปิด clock ของ GPIO/AFIO และตั้งค่า SWD/JTAG policy ตามบอร์ด BH314 */
void board_init(void)
{
    /* Enable AFIO and all GPIO ports used by the BH314 pin map. */
    RCM->APB2CLKEN |= RCM_APB2_AFIOEN |
                      RCM_APB2_IOPAEN |
                      RCM_APB2_IOPBEN |
                      RCM_APB2_IOPCEN |
                      RCM_APB2_IOPDEN;

#if BH314_USE_SWD_PINS_AS_GPIO
    /* Original BH314 wiring uses PA13 and PA14 as normal GPIO.
       This disconnects SWD after firmware starts; use connect-under-reset
       or set BH314_USE_SWD_PINS_AS_GPIO=0 while debugging. */
    AFIO->MAP &= ~AFIO_MAP_SWJ_CFG_MASK;
    AFIO->MAP |= AFIO_MAP_SWJ_DISABLED;
#else
    /* Keep SWD active, JTAG disabled. PA13/PA14 remain reserved for SWD. */
    AFIO->MAP &= ~AFIO_MAP_SWJ_CFG_MASK;
    AFIO->MAP |= AFIO_MAP_SWJ_NO_JTAG;
#endif

}
