#ifndef YASP_CONTROLCOMMANDS_H
#define YASP_CONTROLCOMMANDS_H

#include "controlsocket.h"
#include "state.h"

bool handle_packet(struct EmuState* state, packet_type type, void* payload);

#endif //YASP_CONTROLCOMMANDS_H
