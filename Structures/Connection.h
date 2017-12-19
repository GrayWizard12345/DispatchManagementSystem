//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "../global_var/global_var.h"

typedef struct Connection Connection;

struct Connection {
    int socket;
    struct sockaddr_in address;
};

Connection* connectionInit(int socket, struct sockaddr_in address){
    Connection* c = malloc(sizeof(Connection));
    c->socket = socket;
    c->address = address;
    return c;
};

Connection* connectToServer() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    printf("CREATING SOCKET .....\n");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return NULL;
    }

    printf("DEFINING SOCKET FAMILY, ADDRESS & PORT .....\n");
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("CONNECTING ON PORT %d TO COMMUNICATE WITH SERVER.....\n", DEFAULT_PORT);
        printf("\nConnection Failed \n");
        return NULL;
    }

    Connection* c = connectionInit(sock, serv_addr);
    return c;
}