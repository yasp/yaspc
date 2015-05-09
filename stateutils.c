#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "stateutils.h"

__attribute__((always_inline))
bool is_byte_reg_num_valid(struct EmuState *state, uint8_t number) {
    return (number < state->ramc);
}

__attribute__((always_inline))
bool is_word_reg_num_valid(struct EmuState *state, uint8_t number) {
    return (number * 2 < state->ramc);
}

__attribute__((always_inline))
void write_byte_register(struct EmuState* state, uint8_t number, uint8_t value) {
    #ifdef DEBUG
    if(!is_byte_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register b%d", number);
    }
    #endif

    state->ram[number] = value;
}

__attribute__((always_inline))
void read_byte_register(struct EmuState* state, uint8_t number, uint8_t* rtn) {
    #ifdef DEBUG
    if(!is_byte_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register b%d", number);
    }
    #endif

    *rtn = state->ram[number];
}

__attribute__((always_inline))
void write_word_register(struct EmuState* state, uint8_t number, uint16_t value) {
    #ifdef DEBUG
    if(!is_word_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register w%d", number);
    }
    #endif

    state->ram[number + 1] = (uint8_t)(value & 0xFF);
    state->ram[number + 0] = (uint8_t)((value >> 8) & 0xFF);
}

__attribute__((always_inline))
void read_word_register(struct EmuState* state, uint8_t number, uint16_t* val) {
    #ifdef DEBUG
    if(!is_word_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register w%d", number);
    }
    #endif

    *val = 0;
    *val |= state->ram[number + 1] & (uint8_t)0xFF;
    *val |= (state->ram[number + 0] >> 8) & (uint8_t)0xFF;
}
