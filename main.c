#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "defs.h"
#include "decode/decode.h"
#include "state.h"
#include "stateutils.h"
#include "controlsocket.h"

decode_function* decode_functions[0xFF];

void build_decode_functions(decode_function** decode_functions, size_t decode_functionsc) {
    size_t i;

    for(i = 0; i < decode_functionsc; ++i) {
        decode_functions[i] = (decode_function*)decode_dummy;
    }

    decode_functions[0x00] = (decode_function*)decode_00;
    decode_functions[0x10] = (decode_function*)decode_10;
}

void print_state(struct EmuState* state) {
    printf("pc = %d\n", state->pc);
}

void print_byte_registers(struct EmuState* state) {
    uint8_t i;
    uint8_t val;

    for(i = 0; i < N_BYTE_REGISTERS; i++) {
        if(i % 3 == 0 && i != 0) {
            printf("\n");
        }

        read_byte_register(state, i, &val);
        printf("b%02d = 0x%02x ", i, val);
    }

    printf("\n");
}

void print_word_registers(struct EmuState* state) {
    uint8_t i;
    uint16_t val;

    for(i = 0; i < N_WORD_REGISTERS; i++) {
        if(i % 3 == 0 && i != 0) {
            printf("\n");
        }

        read_word_register(state, i, &val);
        printf("w%02d = 0x%02x ", i, val);
    }

    printf("\n");
}

void decode(struct EmuState* state) {
    for (; state->pc < state->romc; ++state->pc) {
        if(state->stepping) {
            if(state->run == 0) {
                break;
            }

            state->run--;
        }

        uint8_t code = state->rom[state->pc];
        decode_function* func = decode_functions[code];

        if(func == decode_dummy) {
            fprintf(stderr, "invalid code %d!", code);
            return;
        }

        int rtn = func(state);

        if(!rtn) {
            printf("unable to decode\n");
            return;
        }

        print_state(state);
        print_byte_registers(state);
        print_word_registers(state);
        printf("=========\n");
    }
}

void handle_packet_load(struct EmuState* state, struct payload_load* payload) {
    set_rom(state, payload->romv, payload->romc);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void handle_packet_run(struct EmuState* state, struct payload_run* payload) {
    decode(state);
}
#pragma GCC diagnostic pop

void handle_packet_step(struct EmuState* state, struct payload_step* payload) {
    state->stepping = true;
    state->run = payload->count;
    decode(state);
}

#ifdef DEBUG
#define PRINT_PACKET_TYPE(type) printf("" #type "\n")
#else
#define PRINT_PACKET_TYPE(type) (void)0
#endif

void handle_packet(struct EmuState* state, packet_type type, void* payload) {
    switch (type) {
        case TYPE_PACKET_LOAD:
            PRINT_PACKET_TYPE(LOAD);
            handle_packet_load(state, (struct payload_load*)payload);
            break;
        case TYPE_PACKET_RUN:
            PRINT_PACKET_TYPE(RUN);
            handle_packet_run(state, (struct payload_run*)payload);
            break;
        case TYPE_PACKET_STEP:
            PRINT_PACKET_TYPE(STEP);
            handle_packet_step(state, (struct payload_step*)payload);
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

int main(void) {
    struct EmuState state;
    struct socketinfo info;
    int client;
    packet_type type;
    void* payload;

    build_decode_functions(decode_functions, sizeof(decode_functions) / sizeof(void*));

    init_state(&state);
    init_socket(&info, "/tmp/yasp");

    if(!accept_client(&info, &client)) {
        return EXIT_FAILURE;
    }

    while (true) {
        if(!read_command(client, &type, &payload)) {
            return EXIT_FAILURE;
        }
        handle_packet(&state, type, payload);
    }

    return EXIT_SUCCESS;
}
