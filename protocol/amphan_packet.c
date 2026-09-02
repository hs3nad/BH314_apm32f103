#include "amphan_packet.h"
#include "amphan_crc.h"

/* สร้าง packet ตามรูปแบบ AMPHAN ลง buffer out */
size_t amphan_packet_encode(uint8_t nfc, uint8_t addr, const uint8_t *data, uint8_t len, uint8_t *out, size_t out_size)
{
    if (len > AMPHAN_MAX_PAYLOAD) {
        return 0;
    }

    const size_t need = 5U + len;
    if (out_size < need) {
        return 0;
    }

    out[0] = AMPHAN_SOF;
    out[1] = nfc;
    out[2] = len;
    out[3] = addr;

    for (uint8_t i = 0; i < len; ++i) {
        out[4U + i] = data ? data[i] : 0U;
    }

    out[4U + len] = amphan_crc8_xor(&out[1], 3U + len);
    return need;
}

/* ถอดรหัส packet AMPHAN และตรวจ SOF/length/CRC */
int amphan_packet_decode(const uint8_t *in, size_t in_len, amphan_packet_t *packet)
{
    if (!in || !packet || in_len < 5U) {
        return -1;
    }

    if (in[0] != AMPHAN_SOF) {
        return -2;
    }

    const uint8_t len = in[2];
    if (len > AMPHAN_MAX_PAYLOAD) {
        return -3;
    }

    if (in_len < (size_t)(5U + len)) {
        return -4;
    }

    const uint8_t crc = amphan_crc8_xor(&in[1], 3U + len);
    if (crc != in[4U + len]) {
        return -5;
    }

    packet->nfc = in[1];
    packet->len = len;
    packet->addr = in[3];

    for (uint8_t i = 0; i < len; ++i) {
        packet->data[i] = in[4U + i];
    }

    return 0;
}
