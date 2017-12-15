//
// Created by hamlet on 12/14/17.
//

#ifndef DISPATCHMANAGEMENTSYSTEM_SERVER_H
#define DISPATCHMANAGEMENTSYSTEM_SERVER_H

#define CLIENT_OBJ 0
#define DRIVER_OBJ 1



char* initialPackageToSend = "Hello form server!\n";
char* initialPackageToReceive;
char* serverShutdownMessage = "SERVER IS GOING DOWN!";
int client_is_active[2][MAX_CLIENTS] = {};
int clients_count = 0;
int driver_is_active[2][MAX_DRIVERS] = {};
int drivers_count = 0;



//Will accept incoming connections
void acceptConnections(Server server);

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




void acceptConnections(Server server) {
    while (clients_count <= MAX_DRIVERS + MAX_CLIENTS)
    {
        int i = 0;
        int j = 0;
        int sock;
        //this loop finds a not vacant place in activity array
        while (client_is_active[0][i++] != 0);
        while (driver_is_active[0][j++] != 0);

        //Wait for a connection and accept it
        if((sock = accept(server.connection->socket,
                          (struct sockaddr*)&server.connection->address,
                          (socklen_t*)&server.connection->address)) < 0)
        {
            perror("SOCKET ACCEPT FAILED");
            exit(EXIT_FAILURE);
        }

        char* initBuff[MAX_BUFFER];

        if(read(sock, initBuff, MAX_BUFFER) < 0)
        {
            perror("ERROR ON READ FROM SOCKET");
            exit(EXIT_FAILURE);
        }

        //TODO parse initBuff from JSON to obj or string


        if(true)
        {
            client_is_active[0][i] = 1;
            //TODO initialize client id, get it from JSON object
            client_is_active[1][i] = server.clients[i].id;
            server.clients[i].isUp = 1;
            server.clients[i]->connection->socket = sock;
            printf("SERVER ACCEPTED NEW CONNECTION FROM A CLIENT ON PORT %d .....\n", DEFAULT_PORT);
            startSession(server.clients[i], 0);
        }
        else
        {
            driver_is_active[0][j] = 1;
            //TODO initialize client id, get it from JSON object
            driver_is_active[1][j] = server.drivers[j].id;
            server.drivers[i].isUp = 1;
            server.drivers[j]->connection->socket = sock;
            printf("SERVER ACCEPTED NEW CONNECTION FROM A CLIENT ON PORT %d .....\n", DEFAULT_PORT);
            startSession(server.drivers[i], 1);
        }

    }
}


void startSession(void* obj, int obj_type) {

    switch (obj_type) {
        case CLIENT_OBJ:

            Client* client = obj;

            break;
        case DRIVER_OBJ:

            Driver* driver = obj;

            break;

        default:
            break;
    }
}

#endif //DISPATCHMANAGEMENTSYSTEM_SERVER_H
