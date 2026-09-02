#ifndef BSP_RS485_H
#define BSP_RS485_H

#include <stdint.h>

/* เริ่มต้น RS485 direction pin */
void bsp_rs485_init(void);

/* ตั้งโหมดส่ง/รับของ RS485 */
void bsp_rs485_set_tx(uint32_t enable);

/* ส่งข้อมูลผ่าน RS485 */
void bsp_rs485_send_bytes(const uint8_t *data, uint32_t len);

#endif
