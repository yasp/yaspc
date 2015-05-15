#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "defs.h"
#include "decode/decode.h"
#include "state.h"
#include "stateutils.h"
#include "controlsocket.h"
#include "controlcommands.h"
#include "runner.h"

int main(void) {
    struct EmuState state;
    struct socketinfo info;
    int client;
    packet_type type;
    void* payload;

    build_decode_functions();

    init_state(&state);
    init_socket(&info, "/tmp/yasp");

    printf("waiting for a connection\n");
    if(!accept_client(&info, &client)) {
        return EXIT_FAILURE;
    }

    printf("waiting for commands\n");
    while (true) {
        if(!read_command(client, &type, &payload)) {
            printf("invalid command %d", type);
            return EXIT_FAILURE;
        }

        if(!handle_packet(&state, type, payload)) {
            printf("cannot handle command");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
