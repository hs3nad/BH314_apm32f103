#ifndef BSP_TIME_H
#define BSP_TIME_H

#include <stdint.h>

/* เริ่มต้น SysTick และตัวนับเวลา */
void bsp_time_init(void);

/* อ่านเวลาระบบเป็น millisecond */
uint32_t bsp_millis(void);

#endif /* BSP_TIME_H */
