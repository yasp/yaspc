#ifndef YASP_DECODE_H
#define YASP_DECODE_H

#include <stddef.h>
#include "../defs.h"
#include "../state.h"

#define DECODE_FUNC_PARAMS struct EmuState* state

typedef int decode_function(struct EmuState*);

int decode_dummy(DECODE_FUNC_PARAMS);

#endif //YASP_DECODE_H
