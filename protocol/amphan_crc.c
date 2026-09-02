#include "amphan_crc.h"

/* คำนวณ CRC แบบ XOR 8 บิตจากข้อมูลที่ส่งเข้ามา */
uint8_t amphan_crc8_xor(const uint8_t *data, size_t len)
{
    uint8_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
    }
    return crc;
}

/* คำนวณ CRC16 แบบ Modbus polynomial 0xA001 */
uint16_t amphan_crc16_modbus(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFFU;

    for (size_t pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)data[pos];
        for (uint8_t i = 0; i < 8U; i++) {
            if ((crc & 0x0001U) != 0U) {
                crc >>= 1U;
                crc ^= 0xA001U;
            } else {
                crc >>= 1U;
            }
        }
    }

    return crc;
}
