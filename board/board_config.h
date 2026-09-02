#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32f103_compat.h"

/* BH314 original source says stm32f103rct6 and uses LQFP64-only pins
   such as PC0-PC12 and PD2. The selected replacement IC is STM32F103RCT8. */
#define BOARD_NAME              "BH314_STM32F103RCT8"
#define BOARD_HSI_HZ            8000000UL
#define BOARD_HSE_HZ            8000000UL
#define BOARD_SYSCLK_HZ         72000000UL  /* HSE 8 MHz + PLL x9 */
#define BOARD_UART1_BAUD        2400UL

/* RS485 direction pin from BH314.ino */
#define BOARD_RS485_DE_PORT     GPIOA
#define BOARD_RS485_DE_PIN      8U
#define BOARD_RS485_DE_ACTIVE   1

/* BH314 uses PA13/PA14 as OP3/IN3. Keep this enabled for behavior matching
   the original hardware. Set to 0 only while debugging through SWD. */
#define BH314_USE_SWD_PINS_AS_GPIO 1

#endif /* BOARD_CONFIG_H */
