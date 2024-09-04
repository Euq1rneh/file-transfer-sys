#include "serialization.h"

uint8_t serialize_int(int value) {
    uint8_t *out;

    out[0] = (value >> 24) & 0xFF;
    out[1] = (value >> 16) & 0xFF;
    out[2] = (value >> 8) & 0xFF;
    out[3] = value & 0xFF;

    return out;
}

int deserialize_int(uint8_t *in) {
    return (in[0] << 24) |
           (in[1] << 16) |
           (in[2] << 8)  |
           in[3];
}
