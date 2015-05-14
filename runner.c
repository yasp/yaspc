#include <stdio.h>
#include <stdint.h>
#include "runner.h"
#include "stateutils.h"
#include "decode/decode.h"

#define DECODE_FUNCTIONSC 0xFF
decode_function* decode_functions[DECODE_FUNCTIONSC];

void build_decode_functions() {
    size_t i;

    for(i = 0; i < DECODE_FUNCTIONSC; ++i) {
        decode_functions[i] = (decode_function*)decode_dummy;
    }

    decode_functions[0x00] = (decode_function*)decode_00;
    decode_functions[0x10] = (decode_function*)decode_10;
}

void print_state(struct EmuState* state) {
    printf("pc = %d\n", state->pc);
}

void print_byte_registers(struct EmuState* state) {
    uint8_t i;
    uint8_t val;

    for(i = 0; i < N_BYTE_REGISTERS; i++) {
        if(i % 3 == 0 && i != 0) {
            printf("\n");
        }

        read_byte_register(state, i, &val);
        printf("b%02d = 0x%02x ", i, val);
    }

    printf("\n");
}

void print_word_registers(struct EmuState* state) {
    uint8_t i;
    uint16_t val;

    for(i = 0; i < N_WORD_REGISTERS; i++) {
        if(i % 3 == 0 && i != 0) {
            printf("\n");
        }

        read_word_register(state, i, &val);
        printf("w%02d = 0x%02x ", i, val);
    }

    printf("\n");
}

void emulate(struct EmuState *state) {
    for (; state->pc < state->romc; ++state->pc) {
        if(state->stepping) {
            if(state->run == 0) {
                break;
            }

            state->run--;
        }

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

        print_state(state);
        print_byte_registers(state);
        print_word_registers(state);
        printf("=========\n");
    }
}
