#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "decode.h"
#include "decode_00.h"
#include "../stateutils.h"

int decode_10(DECODE_FUNC_PARAMS) {
    uint8_t p1 = state->rom[++state->pc];
    uint8_t p2 = state->rom[++state->pc];

    uint8_t reg = ((p1 & 0b00000011) << 3) | ((p2 & 0b11100000) >> 5);
    uint8_t val = p2 & 0b00011111;

    uint8_t subcode = (p1 & (uint8_t)0b1111100) >> 2;

    switch (subcode) {
        case 0b100000:
            break;
        case 0b100001:
            break;
        case 0b100010:
            break;
        case 0b100011:
            break;
        case 0b100100:
            break;
        case 0b100101:
            break;
        case 0b100110:
            break;
        case 0b100111:
            break;
        case 0b101000:
            break;
        case 0b101001:
            break;
        case 0b101010:
            break;
        case 0b101011:
            break;
        case 0b101100:
            break;
        case 0b101101:
            break;
        case 0b101110:
            break;
        case 0b101111:
            break;
        default:
            return false;
    }

    return true;
};
