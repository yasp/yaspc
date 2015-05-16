#ifndef YASP_CONTROLSOCKET_H
#define YASP_CONTROLSOCKET_H

#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>

struct socketinfo {
    int s;
    struct sockaddr fsaun;
    socklen_t fromlen;
};

typedef uint8_t packet_type;

#define TYPE_PACKET_LOAD 1
struct payload_load {
    uint32_t romc;
    uint8_t* romv;
};

#define TYPE_PACKET_CONTINUE 2
struct payload_continue {
    uint16_t count;
};

#define TYPE_PACKET_GET_STATE 3

void init_socket(struct socketinfo* info, char* address);
bool accept_client(struct socketinfo* info, int* ns);
bool read_command(int ns, packet_type *type, void **payload);

struct response_get_state {
    uint32_t romc;
    uint8_t* romv;
    uint32_t ramc;
    uint8_t* ramv;
};

void send_response_set_state(int client, struct response_get_state* response);

#endif //YASP_CONTROLSOCKET_H
