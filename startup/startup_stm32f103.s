.syntax unified
.cpu cortex-m3
.thumb

.global g_pfnVectors
.global Reset_Handler

.extern _estack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss
.extern SystemInit
.extern main

.section .text.Reset_Handler,"ax",%progbits
.type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata

copy_data_loop:
    cmp r0, r1
    bcc copy_data
    b zero_bss_start

copy_data:
    ldr r3, [r2], #4
    str r3, [r0], #4
    b copy_data_loop

zero_bss_start:
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0

zero_bss_loop:
    cmp r0, r1
    bcc zero_bss
    b call_main

zero_bss:
    str r2, [r0], #4
    b zero_bss_loop

call_main:
    bl SystemInit
    bl main

hang:
    b hang
.size Reset_Handler, .-Reset_Handler

.section .text.Default_Handler,"ax",%progbits
Default_Handler:
    b Default_Handler

.macro weak_handler name
    .weak \name
    .set \name, Default_Handler
.endm

weak_handler NMI_Handler
weak_handler HardFault_Handler
weak_handler MemManage_Handler
weak_handler BusFault_Handler
weak_handler UsageFault_Handler
weak_handler SVC_Handler
weak_handler DebugMon_Handler
weak_handler PendSV_Handler
weak_handler SysTick_Handler

weak_handler WWDG_IRQHandler
weak_handler PVD_IRQHandler
weak_handler TAMPER_IRQHandler
weak_handler RTC_IRQHandler
weak_handler FLASH_IRQHandler
weak_handler RCM_IRQHandler
weak_handler EINT0_IRQHandler
weak_handler EINT1_IRQHandler
weak_handler EINT2_IRQHandler
weak_handler EINT3_IRQHandler
weak_handler EINT4_IRQHandler
weak_handler DMA1_Channel1_IRQHandler
weak_handler DMA1_Channel2_IRQHandler
weak_handler DMA1_Channel3_IRQHandler
weak_handler DMA1_Channel4_IRQHandler
weak_handler DMA1_Channel5_IRQHandler
weak_handler DMA1_Channel6_IRQHandler
weak_handler DMA1_Channel7_IRQHandler
weak_handler ADC1_2_IRQHandler
weak_handler CAN1_TX_IRQHandler
weak_handler CAN1_RX0_IRQHandler
weak_handler CAN1_RX1_IRQHandler
weak_handler CAN1_SCE_IRQHandler
weak_handler EINT9_5_IRQHandler
weak_handler TMR1_BRK_IRQHandler
weak_handler TMR1_UP_IRQHandler
weak_handler TMR1_TRG_COM_IRQHandler
weak_handler TMR1_CC_IRQHandler
weak_handler TMR2_IRQHandler
weak_handler TMR3_IRQHandler
weak_handler TMR4_IRQHandler
weak_handler I2C1_EV_IRQHandler
weak_handler I2C1_ER_IRQHandler
weak_handler I2C2_EV_IRQHandler
weak_handler I2C2_ER_IRQHandler
weak_handler SPI1_IRQHandler
weak_handler SPI2_IRQHandler
weak_handler USART1_IRQHandler
weak_handler USART2_IRQHandler
weak_handler USART3_IRQHandler
weak_handler EINT15_10_IRQHandler
weak_handler RTCAlarm_IRQHandler
weak_handler USBDWakeUp_IRQHandler
weak_handler USBD_IRQHandler

.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    .word WWDG_IRQHandler
    .word PVD_IRQHandler
    .word TAMPER_IRQHandler
    .word RTC_IRQHandler
    .word FLASH_IRQHandler
    .word RCM_IRQHandler
    .word EINT0_IRQHandler
    .word EINT1_IRQHandler
    .word EINT2_IRQHandler
    .word EINT3_IRQHandler
    .word EINT4_IRQHandler
    .word DMA1_Channel1_IRQHandler
    .word DMA1_Channel2_IRQHandler
    .word DMA1_Channel3_IRQHandler
    .word DMA1_Channel4_IRQHandler
    .word DMA1_Channel5_IRQHandler
    .word DMA1_Channel6_IRQHandler
    .word DMA1_Channel7_IRQHandler
    .word ADC1_2_IRQHandler
    .word CAN1_TX_IRQHandler
    .word CAN1_RX0_IRQHandler
    .word CAN1_RX1_IRQHandler
    .word CAN1_SCE_IRQHandler
    .word EINT9_5_IRQHandler
    .word TMR1_BRK_IRQHandler
    .word TMR1_UP_IRQHandler
    .word TMR1_TRG_COM_IRQHandler
    .word TMR1_CC_IRQHandler
    .word TMR2_IRQHandler
    .word TMR3_IRQHandler
    .word TMR4_IRQHandler
    .word I2C1_EV_IRQHandler
    .word I2C1_ER_IRQHandler
    .word I2C2_EV_IRQHandler
    .word I2C2_ER_IRQHandler
    .word SPI1_IRQHandler
    .word SPI2_IRQHandler
    .word USART1_IRQHandler
    .word USART2_IRQHandler
    .word USART3_IRQHandler
    .word EINT15_10_IRQHandler
    .word RTCAlarm_IRQHandler
    .word USBDWakeUp_IRQHandler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word USBD_IRQHandler
.size g_pfnVectors, .-g_pfnVectors
