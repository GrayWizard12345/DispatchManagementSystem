//
// Created by hamlet on 12/13/17.
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
#include <math.h>
#define pi 3.14159265358979323846

#define MAX_BUFFER 4096
#define MAX_DRIVERS 256
#define MAX_CLIENTS 512
#define DEFAULT_PORT 60666


typedef struct Connection Connection;
struct Connection {

    //Attributes
    char* ip;
    int socket;
    int port;
    struct sockaddr_in address;

    //TODO methods to be added

} connectionInit(){

    //TODO connection initializer

};

typedef struct Order Order;
struct Order {

    struct Location source;
    struct Location destination;

};

typedef struct Driver Driver;
struct Driver {

    struct Connection* connection;


};

typedef struct Client Client;
struct Client {

    struct Connection* connection;


};

int ADD(int a, int b)
{
    return a+b;
}

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
