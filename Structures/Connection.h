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

#define MAX_BUFFER 4096
#define MAX_DRIVERS 256
#define MAX_CLIENTS 512
#define DEFAULT_PORT 60666

//TODO why do we need IP if there is IP in sockaddr_in
typedef struct Connection Connection;
struct Connection {

    //Attribute
    int socket;
    struct sockaddr_in address;
    //TODO methods to be added
};

Connection connectionInit(char* ip, int socket, int port, struct sockaddr_in address){
    Connection c;
    c.ip = ip;
    c.socket = socket;
    c.port = port;
    c.address = address;
    return c;
};