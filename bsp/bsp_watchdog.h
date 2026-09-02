#ifndef BSP_WATCHDOG_H
#define BSP_WATCHDOG_H

#include <stdint.h>

/* เริ่มต้น watchdog */
void bsp_watchdog_init(uint32_t timeout_ms);

/* reload watchdog */
void bsp_watchdog_reload(void);

#endif /* BSP_WATCHDOG_H */
