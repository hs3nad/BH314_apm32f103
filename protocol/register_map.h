#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H

#include <stdint.h>

#define REG_INBUF0          0x00U
#define REG_OUTBUF0         0x14U
#define REG_DEVICE_STATUS   0x20U
#define REG_FW_VERSION      0x21U
#define REG_ERROR_FLAGS     0x22U
#define REG_RS485_ADDRESS   0x23U

/* อ่านค่า register */
uint16_t register_map_read(uint16_t reg);

/* เขียนค่า register */
void register_map_write(uint16_t reg, uint16_t value);

#endif
