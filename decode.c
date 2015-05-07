#include <stdlib.h>
#include <stdio.h>
#include "decode.h"

int decode_dummy(DECODE_FUNC_PARAMS) {
    return EXIT_FAILURE;
};

int decode_00(DECODE_FUNC_PARAMS) {
    byte p1 = rom[++(*romp)];
    byte p2 = rom[++(*romp)];

    byte subcode = (p1 & (byte)0b11100000) >> 5;

    if(subcode == 0b111) {
        if (p1 == 0b11100001) {
            byte mask = p2;
            printf("enable %d\n", mask);
        } else {
            return EXIT_FAILURE;
        }
    } else {
        byte reg = p1 & (byte)0b00011111;
        byte val = p2;

        switch (subcode) {
            case 0b000:
                printf("%d into b%d\n", val, reg);
                break;
            case 0b001:
                printf("b%d+%d into b%d\n", reg, val, reg);
                break;
            case 0b010:
                printf("b%d-%d into b%d\n", reg, val, reg);
                break;
            case 0b011:
                printf("b%d=%d into b%d\n", reg, val, reg);
                break;
            case 0b100:
                printf("b%d&%d into b%d\n", reg, val, reg);
                break;
            case 0b101:
                printf("b%d|%d into b%d\n", reg, val, reg);
                break;
            case 0b110:
                printf("b%d^%d into b%d\n", reg, val, reg);
                break;
            default:
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
};
