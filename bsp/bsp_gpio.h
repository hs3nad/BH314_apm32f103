#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include <stdint.h>
#include "stm32f103_compat.h"

/* ตั้ง GPIO เป็น output */
void bsp_gpio_config_output(GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t cnf);

/* ตั้ง GPIO เป็น input */
void bsp_gpio_config_input(GPIO_TypeDef *port, uint32_t pin, uint32_t cnf, uint32_t pull_up);

/* เขียนค่า GPIO */
void bsp_gpio_write(GPIO_TypeDef *port, uint32_t pin, uint32_t value);

/* toggle ค่า GPIO */
void bsp_gpio_toggle(GPIO_TypeDef *port, uint32_t pin);

/* อ่านค่า GPIO */
uint32_t bsp_gpio_read(GPIO_TypeDef *port, uint32_t pin);

#endif
