#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "stateutils.h"

static bool is_byte_reg_num_valid(struct EmuState *state, uint8_t number) {
    return (number < state->ramc);
}

static bool is_word_reg_num_valid(struct EmuState *state, uint8_t number) {
    return (number * 2 < state->ramc);
}

bool write_byte_register(struct EmuState* state, uint8_t number, uint8_t value) {
    if(!is_byte_reg_num_valid(state, number)) {
        return false;
    }

    state->ram[number] = value;

    return true;
}

bool read_byte_register(struct EmuState* state, uint8_t number, uint8_t* rtn) {
    if(!is_byte_reg_num_valid(state, number)) {
        return false;
    }

    *rtn = state->ram[number];

    return true;
}

bool write_word_register(struct EmuState* state, uint8_t number, uint16_t value) {
    if(!is_word_reg_num_valid(state, number)) {
        return false;
    }

    state->ram[number + 1] = (uint8_t)(value & 0xFF);
    state->ram[number + 0] = (uint8_t)((value >> 8) & 0xFF);
    return true;
}

bool read_word_register(struct EmuState* state, uint8_t number, uint16_t* val) {
    if(!is_word_reg_num_valid(state, number)) {
        return false;
    }

    *val = 0;
    *val |= state->ram[number + 1] & (uint8_t)0xFF;
    *val |= (state->ram[number + 0] >> 8) & (uint8_t)0xFF;

    return true;
}
