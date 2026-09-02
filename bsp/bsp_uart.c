#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "stm32f103_compat.h"
#include "board_config.h"

/* คำนวณค่า baud register ของ USART จาก clock และ baudrate */
static uint16_t uart_brr_from_pclk(uint32_t pclk, uint32_t baudrate)
{
    /* oversampling by 16, compatible with F103 USART BRR format */
    return (uint16_t)((pclk + (baudrate / 2UL)) / baudrate);
}

/* เริ่มต้น USART1 ที่ PA9/PA10 ด้วย baudrate ที่กำหนด */
void bsp_uart1_init(uint32_t baudrate)
{
    RCM->APB2CLKEN |= RCM_APB2_IOPAEN | RCM_APB2_AFIOEN | RCM_APB2_USART1EN;

    /* PA9  = USART1_TX alternate function push-pull */
    bsp_gpio_config_output(GPIOA, 9U, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);

    /* PA10 = USART1_RX floating input */
    bsp_gpio_config_input(GPIOA, 10U, GPIO_CNF_FLOATING, 0U);

    USART1->CTRL1 = 0U;
    USART1->BR = uart_brr_from_pclk(g_system_core_clock_hz, baudrate);
    USART1->CTRL1 = USART_CTRL1_UE | USART_CTRL1_TE | USART_CTRL1_RE;
}

/* ส่ง character หนึ่งตัวออก USART1 */
void bsp_uart1_putc(char c)
{
    while ((USART1->STS & USART_STS_TXE) == 0U) {
    }
    USART1->DATA = (uint32_t)(uint8_t)c;
}

/* ส่ง string ออก USART1 และเติม carriage return ก่อน newline */
void bsp_uart1_write(const char *s)
{
    while (*s) {
        if (*s == '\n') {
            bsp_uart1_putc('\r');
        }
        bsp_uart1_putc(*s++);
    }
}

/* ส่งค่า 8 บิตออกเป็นตัวอักษร hex 2 หลัก */
void bsp_uart1_write_hex8(uint8_t v)
{
    const char hex[] = "0123456789ABCDEF";
    bsp_uart1_putc(hex[(v >> 4U) & 0x0FU]);
    bsp_uart1_putc(hex[v & 0x0FU]);
}

/* ตรวจว่ามี byte รออ่านใน USART1 หรือไม่ */
uint32_t bsp_uart1_available(void)
{
    return (USART1->STS & USART_STS_RXNE) ? 1U : 0U;
}

/* อ่าน byte จาก USART1 คืน 1 เมื่ออ่านได้ และคืน 0 เมื่อยังไม่มีข้อมูล */
uint32_t bsp_uart1_read_byte(uint8_t *out)
{
    if ((USART1->STS & USART_STS_RXNE) == 0U) {
        return 0U;
    }
    *out = (uint8_t)(USART1->DATA & 0xFFU);
    return 1U;
}

/* รอจน USART1 ส่งข้อมูลสุดท้ายเสร็จสมบูรณ์ */
void bsp_uart1_flush_tx(void)
{
    while ((USART1->STS & USART_STS_TC) == 0U) {
    }
}
