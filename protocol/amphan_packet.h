#ifndef AMPHAN_PACKET_H
#define AMPHAN_PACKET_H

#include <stdint.h>
#include <stddef.h>

#define AMPHAN_SOF              0xAAU
#define AMPHAN_MAX_PAYLOAD      32U
#define AMPHAN_MAX_PACKET       (1U + 1U + 1U + 1U + AMPHAN_MAX_PAYLOAD + 1U)

typedef struct
{
    uint8_t nfc;
    uint8_t addr;
    uint8_t len;
    uint8_t data[AMPHAN_MAX_PAYLOAD];
} amphan_packet_t;

/* Packet format: AA | NFC | LEN | ADDR | DATA[LEN] | CRC8_XOR(NFC..DATA) */
/* เข้ารหัสข้อมูลเป็น packet AMPHAN */
size_t amphan_packet_encode(uint8_t nfc, uint8_t addr, const uint8_t *data, uint8_t len, uint8_t *out, size_t out_size);

/* ถอดรหัส packet AMPHAN */
int amphan_packet_decode(const uint8_t *in, size_t in_len, amphan_packet_t *packet);

#endif
