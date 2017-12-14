//
// Created by hamlet on 12/14/17.
//

#ifndef DISPATCHMANAGEMENTSYSTEM_SERVER_H
#define DISPATCHMANAGEMENTSYSTEM_SERVER_H

#define CLIENT_OBJ 0
#define DRIVER_OBJ 1


//Will accept incoming connections
void acceptConnections();

/*
 * Start a connections session with client or driver
 * "obj_type" variable determines the object type - 0 is client, 1 is driver
 */
void startSession(void* obj, int obj_type);

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

        //if(isClient)
        startSession(server.clients[i], 0);
        //else
        startSession(server.drivers[i], 1);


    }
}


void startSession(void* obj, int obj_type) {

    switch (obj_type) {
        case CLIENT_OBJ:

            break;
        case DRIVER_OBJ:

            break;

        default:
            break;
    }
}

#endif //DISPATCHMANAGEMENTSYSTEM_SERVER_H
