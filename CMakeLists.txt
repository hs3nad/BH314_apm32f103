cmake_minimum_required(VERSION 3.20)

project(bh314_stm32f103rct8 C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MCU_FLAGS
    -mcpu=cortex-m3
    -mthumb
)

set(COMMON_FLAGS
    ${MCU_FLAGS}
    -Wall
    -Wextra
    -Werror=implicit-function-declaration
    -ffunction-sections
    -fdata-sections
    -fno-common
    -g3
)

add_compile_options(${COMMON_FLAGS})

add_compile_options(
    $<$<CONFIG:Debug>:-Og>
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:RelWithDebInfo>:-O2>
    $<$<CONFIG:MinSizeRel>:-Os>
)

add_compile_definitions(
    STM32F103
    STM32F103xC
)

include_directories(
    app
    bsp
    protocol
    board
    core
)

set(SOURCES
    startup/startup_stm32f103.s

    app/main.c
    app/app.c
    app/bh314.c
    app/bh314_platform.c

    bsp/bsp_delay.c
    bsp/bsp_gpio.c
    bsp/bsp_uart.c
    bsp/bsp_rs485.c
    bsp/bsp_time.c
    bsp/bsp_watchdog.c

    protocol/amphan_crc.c
    protocol/amphan_packet.c
    protocol/register_map.c

    board/board_stm32f103rct8.c
    board/board_system_stm32f103rct8.c
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -nostartfiles
    -specs=nano.specs
    -specs=nosys.specs
    -T${CMAKE_SOURCE_DIR}/linker/STM32F103xC.ld
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generating .bin and .hex"
)
