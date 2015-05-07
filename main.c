#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decode.h"

int main() {
    int i;
    byte rom[] = {
            0x00, 0x01, 0x20, 0x00, 0x00, 0x00
    };

    decode_function* decode_functions[0xFF];

    for(i = 0; i < sizeof(decode_functions) / sizeof(void*); ++i) {
        decode_functions[i] = (decode_function*)decode_invalid;
    }

    decode_functions[0x00] = (decode_function*)decode_00;

    for (i = 0; i < sizeof(rom); ++i) {
        byte code = rom[i];

        printf("Code: %d\n", code);

        decode_function* func = decode_functions[code];
        int rtn = func(rom, sizeof(rom));

        if(rtn == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
