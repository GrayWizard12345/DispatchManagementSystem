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
#include "Structures.h"
#include "../global_var/enums.h"

typedef struct Server Server;
struct Server {


    struct Connection* connection;
    //Two arrays of pointers
    struct Driver *drivers[MAX_DRIVERS];
    struct Client *clients[MAX_CLIENTS];
    //Socket options
    int opt;

} serverInit(){


    printf("Server constructor called!\n");

    struct Server* server = malloc(sizeof(struct Server));

    //Initializing the arrays
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        server->clients[i] = malloc(sizeof(struct Client));
    }

    for (int j = 0; j < MAX_DRIVERS; ++j) {
        server->drivers[j] = malloc(sizeof(struct Driver));
    }

    server->connection = malloc(sizeof(struct Connection));
    struct Server server1 = *server;
    free(server);
    return server1;
};



//A structure to pass as a parameter to a thread function.
struct Session_params{

    void* obj;
    int obj_type;

};


char* initialPackageToSend = "Hello form server!\n";
char* initialPackageToReceive;
char* serverShutdownMessage = "SERVER IS GOING DOWN!";
int client_is_active[MAX_CLIENTS] = {};
int clients_count = 0;
int driver_is_active[MAX_DRIVERS] = {};
int drivers_count = 0;

int socket_accept_fails_cont = 0;

//Will accept incoming connections
void acceptConnections(Server server);

/*
 * Start a connections session with client or driver
 * "obj_type" variable determines the object type - 0 is client, 1 is driver
 * Returns a void pointers as this function will be passed to a thread
 */
void* startSession(void* params);

/*
 * A function to be passed to pthread.
 * Active connection is maintained in this function
 * Void pointer parameter is a pointer to a structure
 * containing all parameters to be passed to the function
 */
void clientConnectionThread(void*);

/*
 * A function to be passed to pthread.
 * Active connection is maintained in this function
 * Void pointer parameter is a pointer to a structure
 * containing all parameters to be passed to the function
 */
void driverConnectionThread(void*);




void acceptConnections(Server server) {
    while (clients_count <= MAX_DRIVERS + MAX_CLIENTS)  //Almost equivalent to while (true)
    {
        int i = -1;
        int j = -1;
        int sock;
        //this loop finds a not vacant place in activity array
        while (client_is_active[++i] != 0);
        while (driver_is_active[++j] != 0);


        //Wait for a connection and accept it
        int addrlen = sizeof(server.connection->address);
        if((sock = accept(server.connection->socket,
                          (struct sockaddr*)&server.connection->address,
                          (socklen_t*)&addrlen)) < 0)   //Don't pay attention to the error here, it is not an error!!
        {
            perror("SOCKET ACCEPT FAILED");
            socket_accept_fails_cont++;

            if(socket_accept_fails_cont > MAX_SOCKET_ACCEPT_FAILS)
                exit(EXIT_FAILURE);

            continue;
        }

        char* initBuff = malloc(MAX_BUFFER);
        memset(initBuff, 0 , MAX_BUFFER);

        if(read(sock, initBuff, MAX_BUFFER) < 0)
        {
            perror("ERROR ON READ FROM SOCKET");
            continue;
        }

        //TODO parse initBuff from JSON to obj or string

        pthread_t client_threads[MAX_CLIENTS];
        pthread_t driver_threads[MAX_DRIVERS];

        printf("INITIAL BYTE SENT : %s\nVALUES OF ITERATORS : %d %d\nVALUE CHECKED: %d",initBuff, i, j, CLIENT);

        if(initBuff[0] == CLIENT)
        {
            client_is_active[i] = 1;

            //client_is_active[1][i] = (*server.clients[i]).id;
            server.clients[i]->isUp = 1;
            server.clients[i]->id = i;
            server.clients[i]->connection.socket = sock;
            printf("SERVER ACCEPTED NEW CONNECTION FROM A CLIENT ON PORT %d .....\n", DEFAULT_PORT);

            //Code can be optimized, as the same function is called in else branch
            memset(initBuff, 0, MAX_BUFFER);

            //Sending notification that message is accepted.
            //initBuff = "GOT YOUR MESSAGE BRO!";
            //send(sock, initBuff, strlen(initBuff), 0);

            //Thread parameters
            struct Session_params* session_params = malloc(sizeof(struct Session_params));
            session_params->obj = server.clients[i];
            session_params->obj_type = CLIENT;

            //Creation of the thread in which connection is maintained.
            pthread_create(&client_threads[i], NULL, startSession, session_params);
        }
        else if(initBuff[0] == DRIVER)
        {
            driver_is_active[j] = 1;
            //driver_is_active[j] = (*server.drivers[j]).id;
            server.drivers[i]->isUp = 1;
            server.drivers[j]->connection->socket = sock;
            server.drivers[j]->id = j;
            printf("SERVER ACCEPTED NEW CONNECTION FROM A DRIVER ON PORT %d .....\n", DEFAULT_PORT);

            memset(initBuff, 0, MAX_BUFFER);
            //Sending notification that message is accepted.
            //initBuff = "Hello from Server\n";
            //send(sock, initBuff, strlen(initBuff), 0);


            struct Session_params* session_params = malloc(sizeof(struct Session_params));
            session_params->obj = server.drivers[j];
            session_params->obj_type = DRIVER;
            pthread_create(&driver_threads[j], NULL, startSession, session_params);

        } else if(initBuff[0] == SERVER)
        {
            //Admin connected here
        } else
        {
            printf("WRONG INITIAL BYTE SENT!\n");
        }

    }
}


void* startSession(void* params) {

    //Getting parameters... Hope this gonna work...
    struct Session_params* session_params = params;
    int obj_type = session_params->obj_type;

    int message_t;
    char buffer[MAX_BUFFER];

    Client* client;
    Driver* driver;
    //Here we are ready to get/exchange messages with client or driver!
    switch (obj_type) {
        case CLIENT:

            client = session_params->obj;
            while (client->isUp == 1)
            {
                memset(buffer, 0 , MAX_BUFFER);

                int read_status;
                if((read_status = read(client->connection.socket, buffer, MAX_BUFFER)) < 0)
                {
                    char* error = malloc(MAX_BUFFER);
                    sprintf(error, "CONNECTION WITH CLIENT_%d IS LOST\n", client->id);
                    perror(error);
                    close(client->connection.socket);
                } else if(read_status == 0)
                    break;

                printf("Client_%d: %s" ,client->id,buffer);
                //TODO parse obtained JSON
                message_t = rand(); //get message type form JSON

                send(client->connection.socket,buffer, strlen(buffer), 0);
                //React to client message
                switch (message_t)
                {
                    case 1:
                        break;

                    case 2:
                        break;

                    default:
                        break;
                }


            }
            break; //End of case 0

        case DRIVER:

            driver = session_params->obj;
            while (driver->isUp == 1)
            {
                memset(buffer, 0 , MAX_BUFFER);

                int read_status;
                if(( read_status = read(driver->connection->socket, buffer, MAX_BUFFER)) < 0)
                {
                    char* error = malloc(MAX_BUFFER);
                    sprintf(error, "CONNECTION WITH DRIVER_%d IS LOST\n", driver->id);
                    perror(error);
                    close(driver->connection->socket);
                    break;
                }
                if(read_status == 0)
                {
                    break;
                }
                //TODO parse obtained JSON
                message_t = rand(); //get message type form JSON
                printf("Driver_%d: %s" ,driver->id,buffer);

                send(driver->connection->socket,buffer, MAX_BUFFER, 0);

                switch (message_t)
                {
                    case 1:
                        break;

                    case 2:
                        break;

                    default:
                        break;
                }

            }
           break; //End of case 1

        default:
            perror("UNKNOWN OBJ_TYPE");
            exit(EXIT_FAILURE);
            break;
    }
    if(obj_type == CLIENT)
    {
        close(client->connection.socket);
        printf("CONNECTION WITH CLIENT_%d is CLOSED", client->id);
        client_is_active[client->id] = 0;
        clients_count--;
        client->isUp = 0;
        close(client->connection.socket);

    }
    else
    {
        close(driver->connection->socket);
        printf("CONNECTION WITH DRIVER_%d is CLOSED", driver->id);
        driver_is_active[driver->id] = 0;
        drivers_count--;
        driver->isUp = 0;
        close(driver->connection->socket);
    }

}
