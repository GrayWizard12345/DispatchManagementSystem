//
// Created by hamlet on 12/13/17.
//
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "../Structures/Structures.h"

char* initialPackageToSend = "Hello form server!\n";
char* initialPackageToReceive;
char* serverShutdownMessage = "SERVER IS GOING DOWN!";
int client_is_active[MAX_CLIENTS + MAX_DRIVERS] = {};
int clients_count = 0;
struct Server server;

//Will accept incoming connections
void acceptConnections();

//Start a connections session with client or driver
void startSession(void*);


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


    printf("5 + 4 = %d", server.myMethod(5,4));




    return 0;
}


void acceptConnections() {
    while (clients_count <= MAX_DRIVERS + MAX_CLIENTS)
    {
        int i = 0;

        //this loop finds a not vacant place in activity array
        while (client_is_active[i++] != 0);

        //and reserve found place for a new client to be connected
        client_is_active[i] = 1;

        //Wait for a connection and accept it
        if((server.clients[i]->connection->socket =
                    accept(server.connection->socket,
                           (struct sockaddr*)&server.connection->address,
                           (socklen_t*)&server.connection->address)) < 0)
        {
            perror("SOCKET ACCEPT FAILED");
            exit(EXIT_FAILURE);
        }

        printf("SERVER ACCEPTED NEW CONNECTION FROM A CLIENT ON PORT %d .....\n", DEFAULT_PORT);

        //some volatile variables must be declared here
//        clients[i]->client_id = i;
//        clients[i]->isUp = 1;
//        clients_count++;



    }
}
