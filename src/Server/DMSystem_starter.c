//
// Created by hamlet on 12/13/17.
//

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include<unistd.h>
#include "../../Structures/Structures.h"


struct Server server;

void sig_int_handler(int sig)
{
    if(sig == SIGINT)
    {

        printf("SIGINT handled! SOCKET IS CLOSES NOW");
        if(close(server.connection->socket) < 0)
        {
            exit(1);
        }
    }
}


// Main thread
int main(int argc, char** argv)
{

    if (signal(SIGINT, sig_int_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

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
    printf("BINDING SERVER SOCKET TO PORT %d .....\n", DEFAULT_PORT);

    // Forcefully attaching socket to the DEFAULT_PORT
    if (bind(server.connection->socket, (struct sockaddr *)&server.connection->address, sizeof(server.connection->address))<0)
    {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }

    //Listening...
    printf("SERVER LISTENING ON PORT %d FOR NEW CONNECTION.....\n", DEFAULT_PORT);
    //We wait for at most MAX_DRIVERS + MAX_CLIENTS connections,
    if (listen(server.connection->socket, MAX_DRIVERS + MAX_CLIENTS) < 0)
    {
        perror("socket listen failed ");
        exit(EXIT_FAILURE);
    }


    printf("Server is UP :)\n");


    // Accept connections
    acceptConnections(server);





    close(server.connection->socket);
    //Server is going down
    printf("Server is going DOWN :(\n");

    return 0;
}
