#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "decode.h"
#include "decode_00.h"
#include "../stateutils.h"

int decode_00(DECODE_FUNC_PARAMS) {
    uint8_t p1 = state->rom[++state->pc];
    uint8_t p2 = state->rom[++state->pc];

    uint8_t subcode = (p1 & (uint8_t)0b11100000) >> 5;

    if(subcode == 0b111) {
        if (p1 == 0b11100001) {
            uint8_t mask = p2;
            printf("enable %d\n", mask);
        } else {
            return false;
        }
    } else {
        uint8_t reg = p1 & (uint8_t)0b00011111;
        uint8_t regval;
        uint8_t val = p2;

        if(!is_byte_reg_num_valid(state, reg)) {
            fprintf(stderr, "invalid register b%d", reg);
            return false;
        }

        read_byte_register(state, reg, &regval);

        switch (subcode) {
            case 0b000:
                printf("%d into b%d\n", val, reg);
                write_byte_register(state, reg, val);
                break;
            case 0b001:
                printf("b%d+%d into b%d\n", reg, val, reg);
                val = regval + val;
                break;
            case 0b010:
                printf("b%d-%d into b%d\n", reg, val, reg);
                val = regval - val;
                break;
            case 0b011:
                printf("b%d=%d into b%d\n", reg, val, reg);
                val = (uint8_t)((val > regval) - (val < regval));
                break;
            case 0b100:
                printf("b%d&%d into b%d\n", reg, val, reg);
                val = regval & val;
                break;
            case 0b101:
                printf("b%d|%d into b%d\n", reg, val, reg);
                val = regval | val;
                break;
            case 0b110:
                printf("b%d^%d into b%d\n", reg, val, reg);
                val = regval ^ val;
                break;
            default:
                return false;
        }

        write_byte_register(state, reg, val);
    }

    return true;
};
