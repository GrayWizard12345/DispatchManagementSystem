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
#include <string.h>
#include <pthread.h>
#include <stdio.h>

typedef struct Server Server;
struct Server {


    struct Connection* connection;
    struct Driver** drivers;
    struct Client** clients;
    //Socket options
    int opt;

    int (*myMethod)(int a, int b);


} serverInit(){


    printf("Server constructor called!\n");

    struct Server* server = malloc(sizeof(struct Server));
    server->drivers = malloc(sizeof(struct Driver*) * MAX_DRIVERS);

    for (int i = 0; i < MAX_DRIVERS; ++i) {
        server->drivers[i] = malloc(sizeof(struct Driver));
    }
    server->myMethod = ADD;
    return *server;
};
