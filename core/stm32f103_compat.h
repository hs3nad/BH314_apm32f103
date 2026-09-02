#ifndef STM32F103_COMPAT_H
#define STM32F103_COMPAT_H

#include <stdint.h>
#include <stddef.h>

#define __IO volatile

extern uint32_t g_system_core_clock_hz;

/* Cortex-M SysTick */
#define SYSTICK_BASE        (0xE000E010UL)
#define SYSTICK_CTRL        (*(__IO uint32_t *)(SYSTICK_BASE + 0x00UL))
#define SYSTICK_LOAD        (*(__IO uint32_t *)(SYSTICK_BASE + 0x04UL))
#define SYSTICK_VAL         (*(__IO uint32_t *)(SYSTICK_BASE + 0x08UL))

#define SYSTICK_CTRL_ENABLE     (1UL << 0)
#define SYSTICK_CTRL_TICKINT    (1UL << 1)
#define SYSTICK_CTRL_CLKSOURCE  (1UL << 2)
#define SYSTICK_CTRL_COUNTFLAG  (1UL << 16)

/* STM32F103-compatible peripheral base addresses */
#define PERIPH_BASE         (0x40000000UL)
#define APB1PERIPH_BASE     (PERIPH_BASE)
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE      (PERIPH_BASE + 0x00020000UL)

#define GPIOA_BASE          (APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE          (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOC_BASE          (APB2PERIPH_BASE + 0x1000UL)
#define GPIOD_BASE          (APB2PERIPH_BASE + 0x1400UL)
#define AFIO_BASE           (APB2PERIPH_BASE + 0x0000UL)
#define USART1_BASE         (APB2PERIPH_BASE + 0x3800UL)
#define USART2_BASE         (APB1PERIPH_BASE + 0x4400UL)
#define USART3_BASE         (APB1PERIPH_BASE + 0x4800UL)
#define IWDG_BASE           (APB1PERIPH_BASE + 0x3000UL)
#define RCM_BASE            (AHBPERIPH_BASE  + 0x1000UL)
#define FLASH_REG_BASE      (AHBPERIPH_BASE  + 0x2000UL)

/* Register structures */
typedef struct
{
    __IO uint32_t CTRL_LOW;     /* 0x00 CRL  */
    __IO uint32_t CTRL_HIGH;    /* 0x04 CRH  */
    __IO uint32_t IDATA;        /* 0x08 IDR  */
    __IO uint32_t ODATA;        /* 0x0C ODR  */
    __IO uint32_t BSC;          /* 0x10 BSRR */
    __IO uint32_t BC;           /* 0x14 BRR  */
    __IO uint32_t LOCK;         /* 0x18 LCKR */
} GPIO_TypeDef;

typedef struct
{
    __IO uint32_t CTL;          /* 0x00 CR       */
    __IO uint32_t CFG;          /* 0x04 CFGR     */
    __IO uint32_t INT;          /* 0x08 CIR      */
    __IO uint32_t APB2RST;      /* 0x0C APB2RSTR */
    __IO uint32_t APB1RST;      /* 0x10 APB1RSTR */
    __IO uint32_t AHBCLKEN;     /* 0x14 AHBENR   */
    __IO uint32_t APB2CLKEN;    /* 0x18 APB2ENR  */
    __IO uint32_t APB1CLKEN;    /* 0x1C APB1ENR  */
    __IO uint32_t BDCTRL;       /* 0x20 BDCR     */
    __IO uint32_t CTRLSTS;      /* 0x24 CSR      */
} RCM_TypeDef;

typedef struct
{
    __IO uint32_t STS;          /* 0x00 SR   */
    __IO uint32_t DATA;         /* 0x04 DR   */
    __IO uint32_t BR;           /* 0x08 BRR  */
    __IO uint32_t CTRL1;        /* 0x0C CR1  */
    __IO uint32_t CTRL2;        /* 0x10 CR2  */
    __IO uint32_t CTRL3;        /* 0x14 CR3  */
    __IO uint32_t GTP;          /* 0x18 GTPR */
} USART_TypeDef;

typedef struct
{
    __IO uint32_t EVCTRL;       /* 0x00 EVCR */
    __IO uint32_t MAP;          /* 0x04 MAPR */
    __IO uint32_t EINTCFG[4];   /* 0x08 EXTICR1..4 */
    __IO uint32_t MAP2;         /* 0x1C MAPR2 */
} AFIO_TypeDef;

typedef struct
{
    __IO uint32_t KR;           /* 0x00 */
    __IO uint32_t PR;           /* 0x04 */
    __IO uint32_t RLR;          /* 0x08 */
    __IO uint32_t SR;           /* 0x0C */
} IWDG_TypeDef;

typedef struct
{
    __IO uint32_t ACR;          /* 0x00 */
    __IO uint32_t KEYR;         /* 0x04 */
    __IO uint32_t OPTKEYR;      /* 0x08 */
    __IO uint32_t SR;           /* 0x0C */
    __IO uint32_t CR;           /* 0x10 */
    __IO uint32_t AR;           /* 0x14 */
    __IO uint32_t RESERVED;     /* 0x18 */
    __IO uint32_t OBR;          /* 0x1C */
    __IO uint32_t WRPR;         /* 0x20 */
} FLASH_TypeDef;

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define RCM                 ((RCM_TypeDef  *) RCM_BASE)
#define USART1              ((USART_TypeDef *) USART1_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define USART3              ((USART_TypeDef *) USART3_BASE)
#define AFIO                ((AFIO_TypeDef  *) AFIO_BASE)
#define IWDG                ((IWDG_TypeDef  *) IWDG_BASE)
#define FLASH_REG           ((FLASH_TypeDef *) FLASH_REG_BASE)

/* Clock control bits */
#define RCM_CTL_HSEON       (1UL << 16)
#define RCM_CTL_HSERDY      (1UL << 17)
#define RCM_CTL_PLLON       (1UL << 24)
#define RCM_CTL_PLLRDY      (1UL << 25)

#define RCM_CFG_SW_MASK         (3UL << 0)
#define RCM_CFG_SW_PLL          (2UL << 0)
#define RCM_CFG_SWS_MASK        (3UL << 2)
#define RCM_CFG_SWS_PLL         (2UL << 2)
#define RCM_CFG_HPRE_DIV1       (0UL << 4)
#define RCM_CFG_PPRE1_DIV2      (4UL << 8)
#define RCM_CFG_PPRE2_DIV1      (0UL << 11)
#define RCM_CFG_PLLSRC_HSE      (1UL << 16)
#define RCM_CFG_PLLMULL9        (7UL << 18)

/* Flash access control bits */
#define FLASH_ACR_LATENCY_MASK  (7UL << 0)
#define FLASH_ACR_LATENCY_2WS   (2UL << 0)
#define FLASH_ACR_PRFTBE        (1UL << 4)

/* Clock enable bits compatible with F103 register layout */
#define RCM_APB2_AFIOEN     (1UL << 0)
#define RCM_APB2_IOPAEN     (1UL << 2)
#define RCM_APB2_IOPBEN     (1UL << 3)
#define RCM_APB2_IOPCEN     (1UL << 4)
#define RCM_APB2_IOPDEN     (1UL << 5)
#define RCM_APB2_USART1EN   (1UL << 14)
#define RCM_APB1_USART2EN   (1UL << 17)
#define RCM_APB1_USART3EN   (1UL << 18)

/* GPIO config helpers: 4 bits per pin */
#define GPIO_MODE_INPUT         0x0U
#define GPIO_MODE_OUTPUT_10MHZ  0x1U
#define GPIO_MODE_OUTPUT_2MHZ   0x2U
#define GPIO_MODE_OUTPUT_50MHZ  0x3U

#define GPIO_CNF_ANALOG         0x0U
#define GPIO_CNF_FLOATING       0x1U
#define GPIO_CNF_INPUT_PUPD     0x2U
#define GPIO_CNF_OUTPUT_PP      0x0U
#define GPIO_CNF_OUTPUT_OD      0x1U
#define GPIO_CNF_AF_PP          0x2U
#define GPIO_CNF_AF_OD          0x3U

#define GPIO_CFG(mode, cnf)     (((cnf) << 2U) | (mode))

#define AFIO_MAP_SWJ_CFG_MASK   (7UL << 24)
#define AFIO_MAP_SWJ_FULL       (0UL << 24)
#define AFIO_MAP_SWJ_NO_JTAG    (2UL << 24)
#define AFIO_MAP_SWJ_DISABLED   (4UL << 24)

/* USART bits */
#define USART_STS_TXE       (1UL << 7)
#define USART_STS_TC        (1UL << 6)
#define USART_STS_RXNE      (1UL << 5)
#define USART_CTRL1_UE      (1UL << 13)
#define USART_CTRL1_TE      (1UL << 3)
#define USART_CTRL1_RE      (1UL << 2)

#endif /* STM32F103_COMPAT_H */
