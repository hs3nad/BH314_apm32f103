#include "bsp_rs485.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "board_config.h"

/* เริ่มต้นขา direction ของ RS485 และตั้งเป็นโหมดรับ */
void bsp_rs485_init(void)
{
    bsp_gpio_config_output(BOARD_RS485_DE_PORT, BOARD_RS485_DE_PIN, GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_PP);
    bsp_rs485_set_tx(0U);
}

/* ตั้ง direction ของ RS485: enable != 0 คือโหมดส่ง */
void bsp_rs485_set_tx(uint32_t enable)
{
#if BOARD_RS485_DE_ACTIVE
    bsp_gpio_write(BOARD_RS485_DE_PORT, BOARD_RS485_DE_PIN, enable ? 1U : 0U);
#else
    bsp_gpio_write(BOARD_RS485_DE_PORT, BOARD_RS485_DE_PIN, enable ? 0U : 1U);
#endif
}

/* ส่งข้อมูลผ่าน UART พร้อมสลับ RS485 ไปโหมดส่งก่อน แล้วกลับรับหลังส่ง */
void bsp_rs485_send_bytes(const uint8_t *data, uint32_t len)
{
    bsp_rs485_set_tx(1U);

    for (uint32_t i = 0; i < len; ++i) {
        bsp_uart1_putc((char)data[i]);
    }

    bsp_uart1_flush_tx();
    bsp_rs485_set_tx(0U);
}
