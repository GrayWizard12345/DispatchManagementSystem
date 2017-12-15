//
// Created by hamlet on 12/13/17.
//
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "../Structures/Structures.h"


struct Server server;


// Main thread
int main(int argc, char** argv)
{

    // serverInit() is responsible for initialization of
    // (a.k.a memory allocation for) clients and drivers

    server = serverInit();
    int addrlen = sizeof(struct sockaddr_in);

    printf("CREATING SERVER SOCKET .....\n");

    // Creating socket file descriptor
    if ((server.connection->socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("DEFINING SERVER SOCKET OPTIONS, FAMILY, ADDRESS & PORT .....\n");

    if (setsockopt(server.connection->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &server.opt, sizeof(server.opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    //Initializing address family and port
    server.connection->address.sin_family = AF_INET;
    server.connection->address.sin_addr.s_addr = INADDR_ANY;
    server.connection->address.sin_port = htons( DEFAULT_PORT );
    printf("BINDING SERVER SOCKET TO PORT 8080 .....\n");

    // Forcefully attaching socket to the port 8080
    if (bind(server.connection->socket, (struct sockaddr *)&server.connection->address, sizeof(server.connection->address))<0)
    {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }

    //Listening...
    printf("SERVER LISTENING ON PORT 8080 FOR NEW CONNECTION.....\n");
    //We wait for at most MAX_DRIVERS + MAX_CLIENTS connections,
    if (listen(server.connection->socket, MAX_DRIVERS + MAX_CLIENTS) < 0)
    {
        perror("socket listen failed ");
        exit(EXIT_FAILURE);
    }


    printf("Hello World\n");


    // Accept connections
    acceptConnections(server);







    return 0;
}
