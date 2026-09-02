#ifndef AMPHAN_CRC_H
#define AMPHAN_CRC_H

#include <stdint.h>
#include <stddef.h>

/* คำนวณ CRC8 แบบ XOR */
uint8_t amphan_crc8_xor(const uint8_t *data, size_t len);

/* คำนวณ CRC16 แบบ Modbus */
uint16_t amphan_crc16_modbus(const uint8_t *data, size_t len);

#endif
