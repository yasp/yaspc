#ifndef YASP_CONTROLSOCKET_H
#define YASP_CONTROLSOCKET_H

#include <sys/socket.h>
#include <stdint.h>

struct socketinfo {
    int s;
    struct sockaddr fsaun;
    socklen_t fromlen;
};

typedef uint8_t packet_type;

struct packet {
    packet_type type;
    uint8_t len;
};

#define TYPE_PACKET_LOAD 1
struct payload_load {
    uint32_t romc;
    uint8_t romv[];
};

void init_socket(struct socketinfo* info, char* address);
bool accept_client(struct socketinfo* info, int* ns);
bool read_from_client(int ns, packet_type* type, void** payload);

#endif //YASP_CONTROLSOCKET_H
