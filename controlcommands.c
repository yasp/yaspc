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

void handle_packet_continue(struct EmuState* state, struct payload_continue* payload) {
    state->stepping = true;
    state->run = payload->count;
    state->stepping = (state->run != UINT16_MAX);
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
        case TYPE_PACKET_CONTINUE:
            PRINT_PACKET_TYPE(CONTINUE);
            handle_packet_continue(state, (struct payload_continue*)payload);
            break;
        default:
            return false;
    }

    return true;
}
