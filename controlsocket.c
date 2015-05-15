#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "controlsocket.h"

void init_socket(struct socketinfo* info, char* address) {
    socklen_t len;
    struct sockaddr saun;

    if ((info->s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        exit(1);
    }

    saun.sa_family = AF_UNIX;
    strcpy(saun.sa_data, address);

    unlink(address);
    len = (socklen_t)(sizeof(saun.sa_family) + strlen(saun.sa_data));

    if (bind(info->s, &saun, len) < 0) {
        perror("server: bind");
        exit(1);
    }

    if (listen(info->s, 5) < 0) {
        perror("server: listen");
        exit(1);
    }
}

bool accept_client(struct socketinfo* info, int* ns) {
    if ((*ns = accept(info->s, &info->fsaun, &info->fromlen)) < 0) {
        return false;
    }
    return true;
}

bool read_array(int ns, size_t len, uint8_t* rtn) {
    ssize_t recvlen = recv(ns, rtn, len, MSG_WAITALL);

    if(recvlen <= 0) {
        return false;
    }

    #ifdef DEBUG
    printf("READ: array[%zd] => (", len);
    unsigned int i;
    for (i=0;i < len;i++) {
        printf("%02x, ", ((uint8_t*)rtn)[i]);
        fflush(stdout);
    }
    printf(")\n");
    #endif

    return true;
}

#ifdef DEBUG
#define READ_THING_DEBUG(type, rtn) printf("READ: " #type " => %d\n", *rtn)
#else
#define READ_THING_DEBUG(type, rtn) (void)0
#endif

#define READ_THING(name, type) \
    bool name(int ns, type* rtn) { \
        void* recvbuf = malloc(sizeof(type)); \
        ssize_t recvlen = recv(ns, recvbuf, sizeof(type), MSG_WAITALL); \
        \
        if(recvlen <= 0) { \
            return false; \
        } \
        \
        *rtn = *(type*)recvbuf; \
        READ_THING_DEBUG(type, rtn); \
        return true; \
    }

READ_THING(read_uint8, uint8_t)
READ_THING(read_uint16, uint16_t)
READ_THING(read_uint32, uint32_t)

bool is_valid_type(packet_type type) {
    switch (type) {
        case TYPE_PACKET_LOAD:
        case TYPE_PACKET_RUN:
        case TYPE_PACKET_STEP:
            return true;
        default:
            return false;
    }
}

bool read_payload_load(int ns, void** rtn) {
    struct payload_load* payload = malloc(sizeof(struct payload_load));

    if(!read_uint32(ns, &payload->romc)) {
        return false;
    }

    payload->romv = malloc(payload->romc);
    read_array(ns, payload->romc, payload->romv);

    *rtn = payload;

    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool read_payload_run(int ns, void** rtn) {
    return true;
}
#pragma GCC diagnostic pop

bool read_command(int ns, packet_type *type, void **payload) {
    uint32_t len;

    if(!read_uint8(ns, type)) {
        printf("read_command: cannot read type\n");
        return false;
    }

    if(!read_uint32(ns, &len)) {
        printf("read_command: cannot read length\n");
        return false;
    }

    if(!is_valid_type(*type)) {
        printf("read_command: unknown type\n");
        return false;
    }

    switch (*type) {
        case TYPE_PACKET_LOAD:
            read_payload_load(ns, payload);
            break;
        case TYPE_PACKET_RUN:
            read_payload_run(ns, payload);
            break;
        default:
            printf("read_command: missing type handler\n");
            return false;
    }

    return true;
}

