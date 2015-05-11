#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stateutils.h"

bool init_state(struct EmuState* state) {
    state->romc = ROM_SIZE;
    if (NULL == (state->rom = malloc(state->romc))) {
        fprintf(stderr, "rom malloc failed\n");
        return false;
    }

    state->romc = RAM_SIZE;

    if (state->romc < N_BYTE_REGISTERS) {
        fprintf(stderr, "ram is not big enought to hold %d byte registers\n", N_BYTE_REGISTERS);
        return false;
    }

    if (state->romc < N_WORD_REGISTERS * 2) {
        fprintf(stderr, "ram is not big enought to hold %d word registers\n", N_WORD_REGISTERS);
        return false;
    }

    if (NULL == (state->ram = malloc(state->ramc))) {
        fprintf(stderr, "ram malloc failed\n");
        return false;
    }

    state->pc = 0;
    state->stepping = false;
    state->run = 0;

    return true;
}

bool set_ram(struct EmuState* state, uint8_t* ram, size_t ramc) {
    if(ramc > state->ramc) {
        fprintf(stderr, "ram is too big (max %d)\n", state->ramc);
        return false;
    }

    memcpy(&state->ram, ram, ramc);

    return true;
}

bool set_rom(struct EmuState* state, uint8_t* rom, size_t romc) {
    if(romc > state->romc) {
        fprintf(stderr, "rom is too big (max %d)\n", state->romc);
        return false;
    }

    memcpy(state->rom, rom, romc);

    return true;
}


#if DEBUG
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
#endif // DEBUG

