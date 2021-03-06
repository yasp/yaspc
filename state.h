#ifndef YASP_STATE_H
#define YASP_STATE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "defs.h"

struct EmuState {
    uint8_t* rom;
    uint32_t romc;

    uint8_t* ram;
    uint32_t ramc;

    unsigned int pc;
    bool stepping;
    unsigned int run;
};

#endif //YASP_STATE_H
