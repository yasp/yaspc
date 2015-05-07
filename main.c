#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decode.h"


void build_decode_functions(decode_function** decode_functions, size_t decode_functionsc) {
    size_t i;

    for(i = 0; i < decode_functionsc; ++i) {
        decode_functions[i] = (decode_function*)decode_dummy;
    }

    decode_functions[0x00] = (decode_function*)decode_00;
}

void decode(byte* rom, size_t romc, decode_function** decode_functions) {
    size_t i;

    for (i = 0; i < romc; ++i) {
        byte code = rom[i];
        decode_function* func = decode_functions[code];

        if(func == decode_dummy) {
            fprintf(stderr, "invalid code %d!", code);
            return;
        }

        int rtn = func(rom, sizeof(rom));

        if(rtn == EXIT_FAILURE) {
            printf("unable to decode\n");
            return;
        }

        printf("=========\n");
    }
}

int main() {
    byte rom[] = {
            0x00, 0x01, 0x20, 0x00, 0x00, 0x00
    };
    size_t romc = sizeof(rom);

    decode_function* decode_functions[0xFF];
    build_decode_functions(decode_functions, sizeof(decode_functions) / sizeof(void*));

    decode(rom, romc, decode_functions);

    return EXIT_SUCCESS;
}
