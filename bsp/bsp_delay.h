#ifndef BSP_DELAY_H
#define BSP_DELAY_H

#include <stdint.h>

/* หน่วงเวลาด้วยการนับ cycle */
void bsp_delay_cycles(volatile uint32_t cycles);

/* หน่วงเวลาเป็น millisecond แบบ blocking */
void bsp_delay_ms(uint32_t ms);

#endif
