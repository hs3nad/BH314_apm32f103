#include "bh314_platform.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_watchdog.h"
#include "board_config.h"

/* แปลงรหัส pin แบบ compact ให้เป็น pointer ของ GPIO port จริง */
static GPIO_TypeDef *pin_port(uint32_t encoded_pin)
{
    switch ((encoded_pin >> 4U) & 0x0FU) {
    case BH_PORT_A: return GPIOA;
    case BH_PORT_B: return GPIOB;
    case BH_PORT_C: return GPIOC;
    case BH_PORT_D: return GPIOD;
    default:        return GPIOA;
    }
}

/* ดึงเลข pin 0-15 ออกจากรหัส pin แบบ compact */
static uint32_t pin_no(uint32_t encoded_pin)
{
    return encoded_pin & 0x0FU;
}

/* ตั้งค่า pin เป็น input pull-up หรือ output ตาม mode ที่ส่งเข้ามา */
void bh_gpio_config(uint32_t encoded_pin, uint32_t mode)
{
    GPIO_TypeDef *port = pin_port(encoded_pin);
    uint32_t pin = pin_no(encoded_pin);

    if (mode == BH_GPIO_OUTPUT) {
        bsp_gpio_config_output(port, pin, GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_PP);
    } else {
        bsp_gpio_config_input(port, pin, GPIO_CNF_INPUT_PUPD, 1U);
    }
}

/* เขียนค่า logic ไปที่ pin ที่ระบุ */
void bh_gpio_write(uint32_t encoded_pin, uint32_t value)
{
    bsp_gpio_write(pin_port(encoded_pin), pin_no(encoded_pin), value);
}

/* อ่านค่า logic จาก pin ที่ระบุ */
uint32_t bh_gpio_read(uint32_t encoded_pin)
{
    return bsp_gpio_read(pin_port(encoded_pin), pin_no(encoded_pin));
}

/* เริ่มต้น UART1 ด้วย baudrate ที่กำหนด */
void bh_serial_begin(uint32_t baudrate)
{
    bsp_uart1_init(baudrate);
}

/* เริ่มต้น UART1 ด้วย baudrate จาก board configuration */
void bh_serial_begin_default(void)
{
    bh_serial_begin(BOARD_UART1_BAUD);
}

/* ตรวจว่ามีข้อมูล serial รออ่านหรือไม่ */
uint32_t bh_serial_available(void)
{
    return bsp_uart1_available();
}

/* อ่าน byte จาก UART1 ถ้าไม่มีข้อมูลจะคืนค่า 0 */
uint8_t bh_serial_read(void)
{
    uint8_t b = 0;
    (void)bsp_uart1_read_byte(&b);
    return b;
}

/* รอให้ UART1 ส่งข้อมูลออกจนหมด */
void bh_serial_flush(void)
{
    bsp_uart1_flush_tx();
}

/* ส่ง frame key ตามรูปแบบเดิมของ BH314: newline, colon, addr, key, carriage return */
void bh_serial_send_key(uint8_t addr, uint8_t key)
{
    bsp_uart1_putc('\n');
    bsp_uart1_putc(':');
    bsp_uart1_write_hex8(addr);
    bsp_uart1_write_hex8(key);
    bsp_uart1_putc('\r');
}

/* เริ่มต้น independent watchdog ตาม timeout ที่กำหนด */
void bh_watchdog_begin(uint32_t timeout_ms)
{
    bsp_watchdog_init(timeout_ms);
}

/* reload watchdog เพื่อบอกว่าระบบยังทำงานปกติ */
void bh_watchdog_reload(void)
{
    bsp_watchdog_reload();
}
