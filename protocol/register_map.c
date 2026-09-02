#include "register_map.h"

static uint16_t g_registers[64];

/* อ่านค่า register ภายใน ถ้า address เกินขอบเขตจะคืน 0xFFFF */
uint16_t register_map_read(uint16_t reg)
{
    if (reg < 64U) {
        return g_registers[reg];
    }
    return 0xFFFFU;
}

/* เขียนค่า register ภายในเมื่อ address อยู่ในขอบเขต */
void register_map_write(uint16_t reg, uint16_t value)
{
    if (reg < 64U) {
        g_registers[reg] = value;
    }
}
