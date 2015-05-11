#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "stateutils.h"

#ifdef DEBUG
bool is_byte_reg_num_valid(struct EmuState *state, uint8_t number) {
    return (number < state->ramc);
}

bool is_word_reg_num_valid(struct EmuState *state, uint8_t number) {
    return (number * 2 < state->ramc);
}

void write_byte_register(struct EmuState* state, uint8_t number, uint8_t value) {
    if(!is_byte_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register b%d", number);
    }

    _WRITE_BYTE_REGISTER(state, number, value)
}

void read_byte_register(struct EmuState* state, uint8_t number, uint8_t* rtn) {
    if(!is_byte_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register b%d", number);
    }

    _READ_BYTE_REGISTER(state, number, rtn)
}

void write_word_register(struct EmuState* state, uint8_t number, uint16_t value) {
    if(!is_word_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register w%d", number);
    }

    _WRITE_WORD_REGISTER(state, number, value)
}

void read_word_register(struct EmuState* state, uint8_t number, uint16_t* rtn) {
    if(!is_word_reg_num_valid(state, number)) {
        fprintf(stderr, "invalid register w%d", number);
    }

    _READ_WORD_REGISTER(state, number, rtn)
}
#endif
