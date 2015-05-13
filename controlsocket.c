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

bool read_packet_part(int ns, size_t len, void** pak) {
    void* recvbuf = malloc(len);
    ssize_t recvlen = recv(ns, recvbuf, len, 0);

    if(recvlen <= 0) {
        return false;
    }

    *pak = recvbuf;
    return true;
}

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

bool read_from_client(int ns, packet_type* type, void** payload) {
    struct packet* pak;

    if(!read_packet_part(ns, sizeof(pak), (void**)&pak)) {
        return false;
    }

    *type = pak->type;

    if(!is_valid_type(*type)) {
        return false;
    }

    if(!read_packet_part(ns, pak->len, payload)) {
        return false;
    }

    free(pak);

    return true;
}

