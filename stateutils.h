#ifndef YASP_STATEUTILS_H
#define YASP_STATEUTILS_H

#include <stdint.h>
#include <stdbool.h>
#include "state.h"

bool write_byte_register(struct EmuState* state, uint8_t number, uint8_t value);
bool read_byte_register(struct EmuState* state, uint8_t number, uint8_t* rtn);

bool write_word_register(struct EmuState* state, uint8_t number, uint16_t value);
bool read_word_register(struct EmuState* state, uint8_t number, uint16_t* val);

#endif //YASP_STATEUTILS_H
