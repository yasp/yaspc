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

bool read_payload_continue(int ns, void** rtn) {
    struct payload_continue* payload = malloc(sizeof(struct payload_continue));

    if(!read_uint16(ns, &payload->count)) {
        return false;
    }

    *rtn = payload;

    return true;
}

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

    switch (*type) {
        case TYPE_PACKET_LOAD:
            read_payload_load(ns, payload);
            break;
        case TYPE_PACKET_CONTINUE:
            read_payload_continue(ns, payload);
            break;
        case TYPE_PACKET_GET_STATE:
            // no payload
            break;
        default:
            printf("read_command: unknown type\n");
            return false;
    }

    return true;
}


#ifdef DEBUG
#define WRITE_THING_DEBUG(type, val) printf("WRITE: " #type " => %d\n", val)
#else
#define WRITE_THING_DEBUG(type, val) (void)0
#endif

#define WRITE_THING(name, type) \
    void name(int client, type val) { \
        send(client, &val, sizeof(val), 0); \
        WRITE_THING_DEBUG(type, val); \
    }

WRITE_THING(write_uint8, uint8_t)
WRITE_THING(write_uint16, uint16_t)
WRITE_THING(write_uint32, uint32_t)

void send_array(int client, uint32_t c, void* v) {
    #ifdef DEBUG
    printf("WRITE: array[%u]\n", c);
    #endif
    send(client, &c, sizeof(c), 0);
    send(client, v, c, 0);
}

void send_response_set_state(int client, struct response_get_state* resp) {
    uint32_t len = (uint32_t)sizeof(uint32_t) * 2 + resp->romc + resp->ramc;
    write_uint32(client, len);
    send_array(client, resp->romc, resp->romv);
    send_array(client, resp->ramc, resp->ramv);
}
