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
#include "../JSON/JSON_encoder.h"

typedef struct Server Server;
struct Server {


    struct Connection* connection;
    //Two arrays of pointers
    struct Driver *drivers[MAX_DRIVERS];
    struct Client *clients[MAX_CLIENTS];
    struct Driver *existingDrivers[MAX_DRIVERS];    //drivers to be read from db
    //Socket options
    int opt;

} serverInit(){


    printf("Server constructor called!\n");

    struct Server* server = malloc(sizeof(struct Server));

    //read drivers from db

    //Initializing the arrays
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        server->clients[i] = malloc(sizeof(struct Client));
        server->clients[i]->connection = malloc(sizeof(struct Connection));
    }

    for (int j = 0; j < MAX_DRIVERS; ++j) {
        server->drivers[j] = malloc(sizeof(struct Driver));
        server->drivers[j]->connection = malloc(sizeof(struct Connection));
    }

    server->connection = malloc(sizeof(struct Connection));
    struct Server server1 = *server;
    free(server);
    return server1;
};



//A structure to pass as a parameter to a thread function.
struct Session_params{

    void* obj;
    void* server;
    int obj_type;

};


int client_is_active[MAX_CLIENTS] = {};
int clients_count = 0;
int driver_is_active[MAX_DRIVERS] = {};
int drivers_count = 0;

int socket_accept_fails_cont = 0;

pthread_t client_threads[MAX_CLIENTS];
pthread_t driver_threads[MAX_DRIVERS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


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
        pthread_mutex_lock(&mutex);
        while (client_is_active[++i] != 0);
        while (driver_is_active[++j] != 0);
        pthread_mutex_unlock(&mutex);

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

        //Get the type of the connected guy
        //char* type = json_getTypeFromJson(initBuff);
        int type = json_getTypeFromJson(initBuff);
        printf("INITIAL BYTE SENT : %s\nVALUES OF ITERATORS : %d %d\nVALUE CHECKED: %d\nCONNECTION TYPE: %d\n",initBuff, i, j, CLIENT, type);

        if(type == CLIENT)
        {
            pthread_mutex_lock(&mutex);
            client_is_active[i] = 1;
            pthread_mutex_unlock(&mutex);
            //client_is_active[1][i] = (*server.clients[i]).id;

            pthread_mutex_lock(&mutex);
            server.clients[i]->isUp = 1;
            server.clients[i]->id = i;
            server.clients[i]->connection->socket = sock;
            pthread_mutex_unlock(&mutex);

            printf("SERVER ACCEPTED NEW CONNECTION FROM A CLIENT ON PORT %d .....\n", DEFAULT_PORT);

            //Code can be optimized, as the same function is called in else branch
            memset(initBuff, 0, MAX_BUFFER);

            //Sending notification that message is accepted.
            //initBuff = "GOT YOUR MESSAGE BRO!";
            //send(sock, initBuff, strlen(initBuff), 0);

            //Thread parameters
            struct Session_params* session_params = malloc(sizeof(struct Session_params));

            pthread_mutex_lock(&mutex);
            session_params->obj = server.clients[i];
            session_params->server = &server;
            session_params->obj_type = CLIENT;
            pthread_mutex_unlock(&mutex);

            session_params->obj_type = CLIENT;

            //Creation of the thread in which connection is maintained.
            pthread_create(&client_threads[i], NULL, startSession, session_params);
        }
        else if(type == DRIVER)
        {
            pthread_mutex_lock(&mutex);
            driver_is_active[j] = 1;
            //driver_is_active[j] = (*server.drivers[j]).id;
            server.drivers[i]->isUp = 1;
            server.drivers[j]->connection->socket = sock;
            server.drivers[j]->id = j;
            pthread_mutex_unlock(&mutex);

            printf("SERVER ACCEPTED NEW CONNECTION FROM A DRIVER ON PORT %d .....\n", DEFAULT_PORT);

            memset(initBuff, 0, MAX_BUFFER);
            //Sending notification that message is accepted.
            //initBuff = "Hello from Server\n";
            //send(sock, initBuff, strlen(initBuff), 0);


            struct Session_params* session_params = malloc(sizeof(struct Session_params));

            pthread_mutex_lock(&mutex);
            session_params->obj = server.drivers[j];
            session_params->obj_type = DRIVER;
            session_params->server = &server;
            pthread_mutex_unlock(&mutex);

            pthread_create(&driver_threads[j], NULL, startSession, session_params);

        } else if(type == SYSADMIN)
        {
            //Admin connected here
            struct Session_params* session_params = malloc(sizeof(struct Session_params));

            pthread_mutex_lock(&mutex);
            session_params->obj_type = SYSADMIN;
            session_params->server = &server;
            pthread_mutex_unlock(&mutex);
            pthread_t pid;
            pthread_create(&pid, NULL, startSession, session_params);

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
    Server *server = session_params->server;

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
                if((read_status = read(client->connection->socket, buffer, MAX_BUFFER)) < 0)
                {
                    char* error = malloc(MAX_BUFFER);
                    sprintf(error, "CONNECTION WITH CLIENT_%d IS LOST\n", client->id);
                    perror(error);
                    close(client->connection->socket);
                } else if(read_status == 0)
                    break;

                printf("Client_%d: %s" ,client->id,buffer);
                //Getting message type here, proper cast may be required later
                message_t = json_getMessageType(buffer); //get message type form JSON

                printf("MESSAGE TYPE: %d", message_t);
                //React to client message
                switch (message_t)
                {
                    case 1:
                        //int id = json_get
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

                //Getting message type from Json
                message_t = json_getMessageType(buffer); //get message type form JSON

                printf("Driver_%d: %s" ,driver->id,buffer);

                char* jsonString = malloc(MAX_BUFFER);
                switch (message_t)
                {
                    case LOG_IN:

                        //Get driver credintails from JSON
                        driver->id = json_getIdFromJson(buffer);
                        strcpy(driver->password, json_getPasswordFromJson(buffer));

                        //Check if this driver exists in db
                        int accessGranted = 1;
                        for (int i = 0; i < MAX_DRIVERS; ++i) {
                            if (server->existingDrivers[i]->id != driver->id ||
                                    strcmp(server->existingDrivers[i]->password,driver->password) != 0)
                            {
                                accessGranted = 0;
                                char* access_denied_json = json_getJsonStringForSimpleMessage(SERVER, ACCESS_SUCCESSFUL);
                                if (send(driver->connection->socket, access_denied_json, sizeof(access_denied_json), 0) < 0)
                                {
                                    perror("FAILED TO SEND DATA");
                                    break;
                                }
                                break;
                            }
                        }

                        if (accessGranted == 1)
                        {
                            char* access_successful_json = json_getJsonStringForSimpleMessage(SERVER, ACCESS_SUCCESSFUL);
                            if (send(driver->connection->socket, access_successful_json, sizeof(access_successful_json), 0) < 0)
                            {
                                perror("FAILED TO SEND DATA");
                                break;
                            }


                            //send driver car if driver exists in db
                            Vehicle vehicle = initVehicle();
                            strcpy(vehicle.color, "RED");
                            strcpy(vehicle.model, "VOLGA");
                            strcpy(vehicle.number, "01AA777C");

                            memset(jsonString, 0, MAX_BUFFER);
                            jsonString = json_getJsonStringFromVehicle(vehicle);

                            if (send(driver->connection->socket, jsonString, MAX_BUFFER, 0) < 0)
                            {
                                perror("FAILED TO SEND DATA");
                                break;
                            }
                        }

                        break;

                    case 2:
                        break;

                    default:
                        break;
                }

            }
           break; //End of case 1
        case SYSADMIN:
            //process his request
            break;
        default:
            perror("UNKNOWN OBJ_TYPE");
            break;
    }
    //Connection is finished here and we need to reclaim memory and reset variables:
    if(obj_type == CLIENT)
    {
        close(client->connection->socket);
        printf("CONNECTION WITH CLIENT_%d is CLOSED", client->id);
        pthread_mutex_lock(&mutex);
        client_is_active[client->id] = 0;
        clients_count--;
        pthread_mutex_unlock(&mutex);
        client->isUp = 0;
        close(client->connection->socket);

    }
    else if(obj_type == DRIVER)
    {
        close(driver->connection->socket);
        printf("CONNECTION WITH DRIVER_%d is CLOSED", driver->id);
        pthread_mutex_lock(&mutex);
        driver_is_active[driver->id] = 0;
        drivers_count--;
        pthread_mutex_unlock(&mutex);
        driver->isUp = 0;
        close(driver->connection->socket);
    }

}
