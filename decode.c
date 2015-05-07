#include <stdlib.h>
#include <stdio.h>
#include "decode.h"

int decode_dummy(DECODE_FUNC_PARAMS) {
    return EXIT_FAILURE;
};

int decode_00(DECODE_FUNC_PARAMS) {
    return EXIT_SUCCESS;
};
