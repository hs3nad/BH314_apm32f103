#ifndef BH314_H
#define BH314_H

#include <stdint.h>

/* เริ่มต้น logic หลักของ BH314 */
void bh314_init(void);

/* ประมวลผลงานหลักของ BH314 ใน loop */
void bh314_task(void);

/* tick 2 ms สำหรับ scan display, dimmer และ input */
void bh314_timer_tick_2ms(void);

#endif /* BH314_H */
