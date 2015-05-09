#ifndef YASP_STATEUTILS_H
#define YASP_STATEUTILS_H

#include <stdint.h>
#include <stdbool.h>
#include "state.h"

bool is_byte_reg_num_valid(struct EmuState *state, uint8_t number);
bool is_word_reg_num_valid(struct EmuState *state, uint8_t number);

void write_byte_register(struct EmuState* state, uint8_t number, uint8_t value);
void read_byte_register(struct EmuState* state, uint8_t number, uint8_t* rtn);

void write_word_register(struct EmuState* state, uint8_t number, uint16_t value);
void read_word_register(struct EmuState* state, uint8_t number, uint16_t* val);

#endif //YASP_STATEUTILS_H
