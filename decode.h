#ifndef YASP_DECODE_H
#define YASP_DECODE_H

#include <stddef.h>
#include "defs.h"

#define DECODE_FUNC_PARAMS byte* rom, size_t romc

typedef int decode_function(byte*, size_t);

int decode_dummy(DECODE_FUNC_PARAMS);
int decode_00(DECODE_FUNC_PARAMS);

#endif //YASP_DECODE_H
