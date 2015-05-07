#include <stdlib.h>
#include <stdio.h>
#include "decode.h"

int decode_invalid(DECODE_FUNC_PARAMS) {
    fprintf(stderr, "invalid code!");
    return EXIT_FAILURE;
};

int decode_00(DECODE_FUNC_PARAMS) {
    return EXIT_SUCCESS;
};
