//
// Created by hamlet on 12/13/17.
//
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>


#define MAX_BUFFER 4096
#define MAX_DRIVERS 256
#define MAX_CLIENTS 512



struct Connection{

    //Attributes
    char* ip;
    int socket;
    int port;
    struct sockaddr_in address;

    //TODO methods to be added

} connectionInit(){

    //TODO connection initializer

};


struct Location{

    float latitude;
    float longitude;

};

struct Order{

    struct Location source;
    struct Location destination;


};



struct Driver{

    struct Connection* connection;


};

struct Client{

    struct Connection* connection;


};

int ADD(int a, int b)
{
    return a+b;
}

struct Server{


    struct Connection* connection;
    struct Driver** drivers;
    struct Client** clients;

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


