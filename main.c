#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "defs.h"
#include "decode/decode.h"
#include "state.h"
#include "stateutils.h"


void build_decode_functions(decode_function** decode_functions, size_t decode_functionsc) {
    size_t i;

    for(i = 0; i < decode_functionsc; ++i) {
        decode_functions[i] = (decode_function*)decode_dummy;
    }

    decode_functions[0x00] = (decode_function*)decode_00;
    decode_functions[0x10] = (decode_function*)decode_10;
}

void decode(struct EmuState* state, decode_function** decode_functions) {
    for (; state->pc < state->romc; ++state->pc) {
        uint8_t code = state->rom[state->pc];
        decode_function* func = decode_functions[code];

        if(func == decode_dummy) {
            fprintf(stderr, "invalid code %d!", code);
            return;
        }

        int rtn = func(state);

        if(!rtn) {
            printf("unable to decode\n");
            return;
        }

        printf("=========\n");
    }
}

int main(void) {
    decode_function* decode_functions[0xFF];
    build_decode_functions(decode_functions, sizeof(decode_functions) / sizeof(void*));

    struct EmuState state;
    init_state(&state);

    uint8_t rom[] = {
            0x00, 0x01, 0x20, 0x00, 0x00, 0x00
    };
    size_t romc = sizeof(rom);

    set_rom(&state, rom, romc);

    decode(&state, decode_functions);

    return EXIT_SUCCESS;
}
