#ifndef YASP_DECODE_H
#define YASP_DECODE_H

#include <stddef.h>
#include "../defs.h"
#include "../state.h"

#define DECODE_FUNC_PARAMS struct EmuState* state

typedef int decode_function(struct EmuState*);

int decode_dummy(DECODE_FUNC_PARAMS);

#import "decode_00.h"
#import "decode_10.h"

#endif //YASP_DECODE_H
