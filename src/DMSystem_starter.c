//
// Created by hamlet on 12/13/17.
//
#include <stdlib.h>
#include <stdio.h>
#include "Structures.h"

char* initialPackageToSend = "Hello form server!\n";
char* initialPackageToReceive;
char* serverShutdownMessage = "SERVER IS GOING DOWN!";

int main(int argc, char** argv)
{

    // serverInit() is responsible for initialization of
    // (a.k.a memory allocation for) clients and drivers

    struct Server server = serverInit();
    int addrlen = sizeof(struct sockaddr_in);

    printf("CREATING SERVER SOCKET .....\n");

    // Creating socket file descriptor
    if ((server.connection->socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("DEFINING SERVER SOCKET OPTIONS, FAMILY, ADDRESS & PORT .....\n");

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server.connection->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &server.opt, sizeof(server.opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    printf("BINDING SERVER SOCKET TO PORT 8080 .....\n");
// Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }

    //Listen
    printf("SERVER LISTENING ON PORT 8080 FOR NEW CONNECTION.....\n");
    if (listen(server_fd, 7) < 0)
    {
        perror("socket listen failed ");
        exit(EXIT_FAILURE);
    }




    printf("Hello World\n");


    printf("5 + 4 = %d", server.myMethod(5,4));




    return 0;
}