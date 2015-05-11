#ifndef YASP_STATEUTILS_H
#define YASP_STATEUTILS_H

#include <stdint.h>
#include <stdbool.h>
#include "state.h"

#define _WRITE_BYTE_REGISTER(state, number, value) state->ram[number] = value;
#define _READ_BYTE_REGISTER(state, number, rtn) *rtn = state->ram[number];

#define _WRITE_WORD_REGISTER(state, number, value) \
    state->ram[number + 1] = (uint8_t)(value & 0xFF); \
    state->ram[number + 0] = (uint8_t)((value >> 8) & 0xFF);

#define _READ_WORD_REGISTER(state, number, rtn) *rtn = state->ram[number + 1] | ((uint16_t)state->ram[number + 0] >> 8);

#ifdef DEBUG

bool is_byte_reg_num_valid(struct EmuState *state, uint8_t number);
bool is_word_reg_num_valid(struct EmuState *state, uint8_t number);

void write_byte_register(struct EmuState* state, uint8_t number, uint8_t value);
void read_byte_register(struct EmuState* state, uint8_t number, uint8_t* rtn);

void write_word_register(struct EmuState* state, uint8_t number, uint16_t value);
void read_word_register(struct EmuState* state, uint8_t number, uint16_t* rtn);

#else

#define is_byte_reg_num_valid(state, number) true
#define is_word_reg_num_valid(state, number) true

#define write_byte_register _WRITE_BYTE_REGISTER
#define read_byte_register _READ_BYTE_REGISTER

#define write_word_register _WRITE_word_REGISTER
#define read_word_register _READ_word_REGISTER

#endif // DEBUG

bool init_state(struct EmuState* state);

bool set_ram(struct EmuState *state, uint8_t *ram, size_t ramc);
bool set_rom(struct EmuState* state, uint8_t* rom, size_t romc);

#endif //YASP_STATEUTILS_H
