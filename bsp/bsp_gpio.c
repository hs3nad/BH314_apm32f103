#include "bsp_gpio.h"

/* เขียนค่า config 4 บิตลง register CTRL_LOW/CTRL_HIGH ของ pin ที่เลือก */
static void gpio_set_config(GPIO_TypeDef *port, uint32_t pin, uint32_t cfg)
{
    volatile uint32_t *reg;
    uint32_t shift;

    if (pin < 8U) {
        reg = &port->CTRL_LOW;
        shift = pin * 4U;
    } else {
        reg = &port->CTRL_HIGH;
        shift = (pin - 8U) * 4U;
    }

    *reg &= ~(0xFUL << shift);
    *reg |= ((cfg & 0xFU) << shift);
}

/* ตั้งค่า GPIO เป็น output ตาม mode และ configuration ที่กำหนด */
void bsp_gpio_config_output(GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t cnf)
{
    gpio_set_config(port, pin, GPIO_CFG(mode, cnf));
}

/* ตั้งค่า GPIO เป็น input และเลือก pull-up/pull-down ถ้าใช้โหมด PUPD */
void bsp_gpio_config_input(GPIO_TypeDef *port, uint32_t pin, uint32_t cnf, uint32_t pull_up)
{
    gpio_set_config(port, pin, GPIO_CFG(GPIO_MODE_INPUT, cnf));

    if (cnf == GPIO_CNF_INPUT_PUPD) {
        bsp_gpio_write(port, pin, pull_up ? 1U : 0U);
    }
}

/* เขียน logic 1 หรือ 0 ไปยัง GPIO pin */
void bsp_gpio_write(GPIO_TypeDef *port, uint32_t pin, uint32_t value)
{
    if (value) {
        port->BSC = (1UL << pin);
    } else {
        port->BC = (1UL << pin);
    }
}

/* toggle ค่า output ปัจจุบันของ GPIO pin */
void bsp_gpio_toggle(GPIO_TypeDef *port, uint32_t pin)
{
    if (port->ODATA & (1UL << pin)) {
        port->BC = (1UL << pin);
    } else {
        port->BSC = (1UL << pin);
    }
}

/* อ่านค่า input ปัจจุบันของ GPIO pin */
uint32_t bsp_gpio_read(GPIO_TypeDef *port, uint32_t pin)
{
    return (port->IDATA & (1UL << pin)) ? 1U : 0U;
}
