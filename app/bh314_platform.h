#ifndef BH314_PLATFORM_H
#define BH314_PLATFORM_H

#include <stdint.h>

typedef uint8_t bh_u8_t;

#define BH_GPIO_INPUT_PULLUP  0x01U
#define BH_GPIO_OUTPUT        0x02U
#define BH_GPIO_LOW           0U
#define BH_GPIO_HIGH          1U

#define BH_PORT_A 0U
#define BH_PORT_B 1U
#define BH_PORT_C 2U
#define BH_PORT_D 3U
#define BH_PIN_ENC(port, pin) (((port) << 4U) | ((pin) & 0x0FU))

/* Compact STM32 GPIO pin identifiers used by the BH314 board map. */
#define BH_PIN_PA0  BH_PIN_ENC(BH_PORT_A,0U)
#define BH_PIN_PA1  BH_PIN_ENC(BH_PORT_A,1U)
#define BH_PIN_PA2  BH_PIN_ENC(BH_PORT_A,2U)
#define BH_PIN_PA3  BH_PIN_ENC(BH_PORT_A,3U)
#define BH_PIN_PA4  BH_PIN_ENC(BH_PORT_A,4U)
#define BH_PIN_PA5  BH_PIN_ENC(BH_PORT_A,5U)
#define BH_PIN_PA6  BH_PIN_ENC(BH_PORT_A,6U)
#define BH_PIN_PA7  BH_PIN_ENC(BH_PORT_A,7U)
#define BH_PIN_PA8  BH_PIN_ENC(BH_PORT_A,8U)
#define BH_PIN_PA9  BH_PIN_ENC(BH_PORT_A,9U)
#define BH_PIN_PA10 BH_PIN_ENC(BH_PORT_A,10U)
#define BH_PIN_PA11 BH_PIN_ENC(BH_PORT_A,11U)
#define BH_PIN_PA12 BH_PIN_ENC(BH_PORT_A,12U)
#define BH_PIN_PA13 BH_PIN_ENC(BH_PORT_A,13U)
#define BH_PIN_PA14 BH_PIN_ENC(BH_PORT_A,14U)
#define BH_PIN_PA15 BH_PIN_ENC(BH_PORT_A,15U)

#define BH_PIN_PB0  BH_PIN_ENC(BH_PORT_B,0U)
#define BH_PIN_PB1  BH_PIN_ENC(BH_PORT_B,1U)
#define BH_PIN_PB2  BH_PIN_ENC(BH_PORT_B,2U)
#define BH_PIN_PB3  BH_PIN_ENC(BH_PORT_B,3U)
#define BH_PIN_PB4  BH_PIN_ENC(BH_PORT_B,4U)
#define BH_PIN_PB5  BH_PIN_ENC(BH_PORT_B,5U)
#define BH_PIN_PB6  BH_PIN_ENC(BH_PORT_B,6U)
#define BH_PIN_PB7  BH_PIN_ENC(BH_PORT_B,7U)
#define BH_PIN_PB8  BH_PIN_ENC(BH_PORT_B,8U)
#define BH_PIN_PB9  BH_PIN_ENC(BH_PORT_B,9U)
#define BH_PIN_PB10 BH_PIN_ENC(BH_PORT_B,10U)
#define BH_PIN_PB11 BH_PIN_ENC(BH_PORT_B,11U)
#define BH_PIN_PB12 BH_PIN_ENC(BH_PORT_B,12U)
#define BH_PIN_PB13 BH_PIN_ENC(BH_PORT_B,13U)
#define BH_PIN_PB14 BH_PIN_ENC(BH_PORT_B,14U)
#define BH_PIN_PB15 BH_PIN_ENC(BH_PORT_B,15U)

#define BH_PIN_PC0  BH_PIN_ENC(BH_PORT_C,0U)
#define BH_PIN_PC1  BH_PIN_ENC(BH_PORT_C,1U)
#define BH_PIN_PC2  BH_PIN_ENC(BH_PORT_C,2U)
#define BH_PIN_PC3  BH_PIN_ENC(BH_PORT_C,3U)
#define BH_PIN_PC4  BH_PIN_ENC(BH_PORT_C,4U)
#define BH_PIN_PC5  BH_PIN_ENC(BH_PORT_C,5U)
#define BH_PIN_PC6  BH_PIN_ENC(BH_PORT_C,6U)
#define BH_PIN_PC7  BH_PIN_ENC(BH_PORT_C,7U)
#define BH_PIN_PC8  BH_PIN_ENC(BH_PORT_C,8U)
#define BH_PIN_PC9  BH_PIN_ENC(BH_PORT_C,9U)
#define BH_PIN_PC10 BH_PIN_ENC(BH_PORT_C,10U)
#define BH_PIN_PC11 BH_PIN_ENC(BH_PORT_C,11U)
#define BH_PIN_PC12 BH_PIN_ENC(BH_PORT_C,12U)
#define BH_PIN_PC13 BH_PIN_ENC(BH_PORT_C,13U)
#define BH_PIN_PC14 BH_PIN_ENC(BH_PORT_C,14U)
#define BH_PIN_PC15 BH_PIN_ENC(BH_PORT_C,15U)

#define BH_PIN_PD0  BH_PIN_ENC(BH_PORT_D,0U)
#define BH_PIN_PD1  BH_PIN_ENC(BH_PORT_D,1U)
#define BH_PIN_PD2  BH_PIN_ENC(BH_PORT_D,2U)

/* ตั้งค่า GPIO จากรหัส pin แบบ compact */
void bh_gpio_config(uint32_t encoded_pin, uint32_t mode);

/* เขียนค่า logic ไปยัง GPIO จากรหัส pin แบบ compact */
void bh_gpio_write(uint32_t encoded_pin, uint32_t value);

/* อ่านค่า logic จาก GPIO จากรหัส pin แบบ compact */
uint32_t bh_gpio_read(uint32_t encoded_pin);

/* เริ่มต้น serial port ที่ใช้กับ BH314 */
void bh_serial_begin(uint32_t baudrate);

/* เริ่มต้น serial port ด้วย baudrate จาก board configuration */
void bh_serial_begin_default(void);

/* ตรวจว่ามีข้อมูล serial รออ่านหรือไม่ */
uint32_t bh_serial_available(void);

/* อ่าน byte จาก serial port */
uint8_t bh_serial_read(void);

/* รอให้ serial ส่งข้อมูลจนหมด */
void bh_serial_flush(void);

/* ส่ง key frame ตาม protocol เดิมของ BH314 */
void bh_serial_send_key(uint8_t addr, uint8_t key);

/* เริ่มต้น watchdog */
void bh_watchdog_begin(uint32_t timeout_ms);

/* reload watchdog */
void bh_watchdog_reload(void);

#endif /* BH314_PLATFORM_H */
