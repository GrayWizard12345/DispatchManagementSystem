//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#ifndef SERVER_HEADER
#define SERVER_HEADER


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
#include "../src/sysadmin/Sysadmin.h"
#include "../Structures/DriverArray.h"

Order json_getOrderFromJson(char*);
Location json_getLocationFromJson(char*);

typedef struct Server Server;
struct Server {


    struct Connection* connection;
    //Two arrays of pointers
    struct Driver *drivers[MAX_DRIVERS];
    struct Client *clients[MAX_CLIENTS];
    struct Driver *existingDrivers[MAX_DRIVERS];    //drivers to be read from db
    int drivers_count;
    //Socket options
    int opt;

} serverInit(){


    printf("Server constructor called!\n");

    struct Server* server = malloc(sizeof(struct Server));

    //read drivers from db
    DriverArray driverArray = getAllDrivers();
    server->drivers_count = (int) driverArray.used;
    for (int i = 0; i < driverArray.used; ++i) {
        server->existingDrivers[i] = &driverArray.array[i];
        printf("%d.Driver id: %d\n  Vehicle number:%s\n  Vehical model: %s\n  Vehical color: %s\n", i, driverArray.array[i].id, driverArray.array[i].vehicle.number, driverArray.array[i].vehicle.model, driverArray.array[i].vehicle.color);
    }
    

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
            server.drivers[j]->index = j;
            pthread_mutex_unlock(&mutex);

            printf("SERVER ACCEPTED NEW CONNECTION FROM A DRIVER ON PORT %d .....\n", DEFAULT_PORT);

            memset(initBuff, 0, MAX_BUFFER);

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
    char* json_string_read;

    Client* client;
    Driver* driver;
    //Here we are ready to get/exchange messages with client or driver!
    switch (obj_type) {
        case CLIENT:

            client = session_params->obj;
            while (client->isUp == 1)
            {
                memset(json_string_read, 0 , MAX_BUFFER);

                int read_status;
                if((read_status = read(client->connection->socket, json_string_read, MAX_BUFFER)) < 0)
                {
                    char* error = malloc(MAX_BUFFER);
                    sprintf(error, "CONNECTION WITH CLIENT_%d IS LOST\n", client->id);
                    perror(error);
                    close(client->connection->socket);
                } else if(read_status == 0)
                    break;

                puts(json_string_read);
                //Getting message type here, proper cast may be required later
                message_t = json_getMessageType(json_string_read); //get message type form JSON

                printf("MESSAGE TYPE: %d", message_t);
                //React to client message
                char *json_to_send;
                switch (message_t)
                {
                    case ORDER_GET:
                        client->order = json_getOrderFromJson(json_string_read);

                        Driver* free_drivers[MAX_DRIVERS];
                        int free_drivers_count = 0;
                        for (int j = 0; j < MAX_DRIVERS; ++j) {
                            free_drivers[j] = malloc(sizeof(Driver));
                        }

                        for (int i = 0, j = 0; i < server->drivers_count; ++i) {
                            if (server->drivers[i]->state == FREE)
                            {
                                free_drivers[j++] = server->drivers[i];
                                free_drivers_count++;
                            }
                        }

                        double minimal_dist[2] = {location_calculateDistanceTo(&free_drivers[0]->location,
                                                                           &client->order.destination), 0};
                        double temp_dist;
                        for (int i = 1; i < free_drivers_count; ++i) {
                            if(minimal_dist[0] > (temp_dist = location_calculateDistanceTo(&free_drivers[i]->location,
                                                                 &client->order.destination)))
                            {
                                minimal_dist[0] = temp_dist;
                                minimal_dist[1] = i;
                            }
                        }

                        if(send(client->connection->socket,
                             json_to_send = json_getJsonStringFromVehicle(server->drivers[(int)minimal_dist[1]]->vehicle), sizeof(json_to_send),0) < 0)
                        {
                            perror("FAILED TO SEND ORDER TO DRIVER");
                        }


                        break;

                    case ORDER_CANCEL:
                        client->order.userId = -1;

                        if(send(server->drivers[(int)minimal_dist[1]]->connection->socket,
                                json_to_send = json_getJsonStringForSimpleMessage(SERVER, ORDER_CANCEL), sizeof(json_to_send),0) < 0)
                        {
                            perror("FAILED TO SEND ORDER TO DRIVER");
                        }

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
                memset(json_string_read, 0 , MAX_BUFFER);

                int read_status;
                if(( read_status = read(driver->connection->socket, json_string_read, MAX_BUFFER)) < 0)
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
                message_t = json_getMessageType(json_string_read); //get message type form JSON

                printf("Driver_%d: %s" ,driver->id,json_string_read);

                char* jsonString = malloc(MAX_BUFFER);

                //json_string_read contains json already
                switch (message_t)
                {
                    case LOG_IN:

                        //Get driver credintails from JSON
                        driver->id = json_getIdFromJson(json_string_read);
                        strcpy(driver->password, json_getPasswordFromJson(json_string_read));

                        //Check if this driver exists in db
                        int access_granted = 0;
                        printf("\nPassword from socket: *%s*",driver->password);
                        for (int i = 0; i < server->drivers_count; ++i) {
                            server->existingDrivers[i]->password[strcspn(server->existingDrivers[i]->password, "\n")] = 0;
                            printf("\nPasswords on server *%s*,  id = %d", server->existingDrivers[i]->password, server->existingDrivers[i]->id);
                            if (server->existingDrivers[i]->id == driver->id)
                            {
                                if (strcmp(server->existingDrivers[i]->password,driver->password) == 0)
                                {
                                    access_granted = 1;
                                    driver->vehicle = server->existingDrivers[i]->vehicle;
                                    char* access_successful_json = json_getJsonStringForSimpleMessage(SERVER, AUTH_SUCCESSFUL);
                                    if (send(driver->connection->socket, access_successful_json, MAX_BUFFER, 0) < 0)
                                    {
                                        perror("FAILED TO SEND DATA");
                                        break;
                                    }


                                    //send driver car if driver exists in db

                                    memset(jsonString, 0, MAX_BUFFER);
                                    jsonString = json_getJsonStringFromVehicle(driver->vehicle);

                                    if (send(driver->connection->socket, jsonString, MAX_BUFFER, 0) < 0)
                                    {
                                        perror("FAILED TO SEND DATA");
                                        break;
                                    }
                                    break;
                                }


                            }
                        }

                        if(access_granted == 0)
                        {
                            char* access_denied_json = json_getJsonStringForSimpleMessage(SERVER, ACCESS_DENIED);
                            if (send(driver->connection->socket, access_denied_json, MAX_BUFFER, 0) < 0)
                            {
                                perror("FAILED TO SEND DATA");
                                break;
                            }
                        }

                        break;

                    case LOCATION_CHANGE:


                        driver->location = json_getLocationFromJson(json_string_read);


                        break; //end of location change
                    case STATE_CHANGE:

                        driver->state = json_getStateFromJson(json_string_read);
                        switch (driver->state)
                        {
                            //FREE, DRIVE_TO_SOURCE, WAITING_CLIENT, PICKED_UP
                            case WAITING_CLIENT:
                                //TODO notify client here

                                break;
                            case FREE:
                                break;
                        }


                        break; //end of change state
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
    /*if(obj_type == CLIENT)
    {
        close(client->connection->socket);
        printf("CONNECTION WITH CLIENT_%d is CLOSED", client->id);
        pthread_mutex_lock(&mutex);
        client_is_active[client->id] = 0;
        clients_count--;
        pthread_mutex_unlock(&mutex);
        client->isUp = 0;
        shutdown(client->connection->socket,2);

    }
    else if(obj_type == DRIVER)
    {
        close(driver->connection->socket);
        printf("CONNECTION WITH DRIVER_%d is CLOSED", driver->id);
        pthread_mutex_lock(&mutex);
        driver_is_active[driver->index] = 0;
        drivers_count--;
        pthread_mutex_unlock(&mutex);
        driver->isUp = 0;
        shutdown(driver->connection->socket,2);
    }*/
}


#endif