#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>

/* เริ่มต้น UART1 */
void bsp_uart1_init(uint32_t baudrate);

/* ส่ง character หนึ่งตัว */
void bsp_uart1_putc(char c);

/* ส่ง string */
void bsp_uart1_write(const char *s);

/* ส่งค่า 8 บิตเป็น hex 2 หลัก */
void bsp_uart1_write_hex8(uint8_t v);

/* ตรวจว่ามีข้อมูลรออ่านหรือไม่ */
uint32_t bsp_uart1_available(void);

/* อ่าน byte จาก UART1 */
uint32_t bsp_uart1_read_byte(uint8_t *out);

/* รอให้ UART1 ส่งจนหมด */
void bsp_uart1_flush_tx(void);

#endif
