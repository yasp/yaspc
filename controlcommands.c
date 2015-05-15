#include <stdlib.h>
#include <stdio.h>
#include "controlcommands.h"
#include "stateutils.h"
#include "controlsocket.h"
#include "runner.h"

void handle_packet_load(struct EmuState* state, struct payload_load* payload) {
    set_rom(state, payload->romv, payload->romc);
    free(payload->romv);
    free(payload);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void handle_packet_run(struct EmuState* state, struct payload_run* payload) {
    emulate(state);
}
#pragma GCC diagnostic pop

void handle_packet_step(struct EmuState* state, struct payload_step* payload) {
    state->stepping = true;
    state->run = payload->count;
    emulate(state);
    free(payload);
}

#ifdef DEBUG
#define PRINT_PACKET_TYPE(type) printf("command: " #type "\n")
#else
#define PRINT_PACKET_TYPE(type) (void)0
#endif

bool handle_packet(struct EmuState* state, packet_type type, void* payload) {
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
            return false;
    }

    return true;
}
