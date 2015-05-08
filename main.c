#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decode.h"
#include "state.h"


void build_decode_functions(decode_function** decode_functions, size_t decode_functionsc) {
    size_t i;

    for(i = 0; i < decode_functionsc; ++i) {
        decode_functions[i] = (decode_function*)decode_dummy;
    }

    decode_functions[0x00] = (decode_function*)decode_00;
}

void decode(struct EmuState* state, decode_function** decode_functions) {
    for (state->pc = 0; state->pc < state->romc; ++state->pc) {
        uint8_t code = state->rom[state->pc];
        decode_function* func = decode_functions[code];

        if(func == decode_dummy) {
            fprintf(stderr, "invalid code %d!", code);
            return;
        }

        int rtn = func(state);

        if(rtn == EXIT_FAILURE) {
            printf("unable to decode\n");
            return;
        }

        printf("=========\n");
    }
}

int main(void) {
    uint8_t rom[] = {
            0x00, 0x01, 0x20, 0x00, 0x00, 0x00
    };
    size_t romc = sizeof(rom);

    uint8_t ram[1024];
    size_t ramc = sizeof(ram);

    decode_function* decode_functions[0xFF];
    build_decode_functions(decode_functions, sizeof(decode_functions) / sizeof(void*));

    struct EmuState state;
    state.rom = rom;
    state.romc = romc;

    state.ram = ram;
    state.ramc = ramc;

    decode(&state, decode_functions);

    return EXIT_SUCCESS;
}
