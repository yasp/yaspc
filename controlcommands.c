#include <stdlib.h>
#include <stdio.h>
#include "controlcommands.h"
#include "stateutils.h"
#include "controlsocket.h"
#include "runner.h"

void handle_packet_load(struct EmuState* state, int client, struct payload_load* payload) {
    set_rom(state, payload->romv, payload->romc);
    free(payload->romv);
    free(payload);
}

void handle_packet_continue(struct EmuState* state, int client, struct payload_continue* payload) {
    state->stepping = true;
    state->run = payload->count;
    state->stepping = (state->run != UINT16_MAX);
    emulate(state);
    free(payload);
}

void handle_packet_get_state(struct EmuState* state, int client) {
    struct response_get_state resp;
    resp.romc = state->romc;
    resp.romv = state->rom;
    resp.ramc = state->ramc;
    resp.ramv = state->ram;
    send_response_set_state(client, &resp);
}

#ifdef DEBUG
#define PRINT_PACKET_TYPE(type) printf("command: " #type "\n")
#else
#define PRINT_PACKET_TYPE(type) (void)0
#endif

bool handle_packet(struct EmuState* state, int client, packet_type type, void* payload) {
    switch (type) {
        case TYPE_PACKET_LOAD:
            PRINT_PACKET_TYPE(LOAD);
            handle_packet_load(state, client, (struct payload_load*)payload);
            break;
        case TYPE_PACKET_CONTINUE:
            PRINT_PACKET_TYPE(CONTINUE);
            handle_packet_continue(state, client, (struct payload_continue*)payload);
            break;
        case TYPE_PACKET_GET_STATE:
            PRINT_PACKET_TYPE(GET_STATE);
            handle_packet_get_state(state, client);
            break;
        default:
            return false;
    }

    return true;
}
