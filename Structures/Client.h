//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#ifndef CLIENT_HEADER
#define CLIENT_HEADER


#include "../Structures/Structures.h"
#include "Order.h"
#include <stdbool.h>


#define nullptr NULL

typedef struct Client Client;

void client_orderTaxi(Client* client);
void client_cancelOrder(void* client, Order order);
void client_setOrder(void* client, Order order);
void client_setPrivateInformation(void* client, char* name, char* phoneNumber);
void client_setConnection(Client* client, Connection* connection);
bool client_orderExists(void* client);
char* json_getJsonStringFromOrder(Order order);

char* json_getJsonStringForSimpleMessage(USER_TYPE user_type, MESSAGE_TYPE message_type);

struct Client {
    Connection *connection;
    Order order;
    int id;
    int isUp;
    char* phoneNumber;
    char* name;

    void (*orderTaxi)(Client* client);
    void (*cancelOrder)(void* client, Order order);
    void (*setOrder)(void* client, Order order);
    void (*setPrivateInformation)(void* client, char* name, char* phoneNumber);
    void (*setConnection)(Client* client, Connection* connection);
    bool (*orderExists)(void* client);

}clientInit() {
    //printf("Client constructor called!\n");

    struct Client* client = malloc(sizeof(Client));
    client->order.userId = -1;

    client->orderTaxi = client_orderTaxi;
    client->cancelOrder = client_cancelOrder;
    client->setOrder = client_setOrder;
    client->setPrivateInformation = client_setPrivateInformation;
    client->setConnection = client_setConnection;
    client->orderExists = client_orderExists;
    //memset(&(client->name), 0, 256);
    //memset(&(client->phoneNumber), 0, 256);

    Client client1 = *client;

    return client1;
};

void freeClient(Client* client) {
    free(client->connection);
//    free(client->phoneNumber);
//    free(client->name);
    free(client);
}

void client_setPrivateInformation(void* client, char* name, char* phoneNumber) {
    Client *c = client;
    c->name = name;
    c->phoneNumber = phoneNumber;
}

void client_setConnection(Client* client, Connection* connection) {
    client->connection = connection;
}

void client_setOrder(void* client, Order order) {
    Client* c = client;
    order.clientPhoneNumber = c->phoneNumber;
    order.clientName = c->name;
    order.userId = c->id;
    c->order = order;
}

bool client_orderExists(void* client) {
    Client* c = client;
    return (c->order.userId != -1);
}

void client_orderTaxi(Client* client) {
    if(client->orderExists) {
        char* json = json_getJsonStringFromOrder(client->order);

        printf("\nTEST%s\n", json);

        if(send(client->connection->socket, json, MAX_BUFFER, 0)< 0)
        {
            perror("FAILED TO SEND ORDER TO SERVER");
        } else {
            puts("\nWaiting server to accept order\n");
        }
}
}

void client_cancelOrder(void* client, Order order) {
    Client* c = client;
    if(c->orderExists) {
       char* json = json_getJsonStringForSimpleMessage(CLIENT, ORDER_CANCEL);
        if(send(c->connection->socket, json, sizeof(json), 0)< 0)
        {
            perror("FAILED TO SEND ORDER TO SERVER");
        }
        c->order.userId  = -1;
        c->orderExists = 0;
        /*
         * -- Server code --
         * Cancel order and notify server about it
         */
/*c->order = nullptr;
} else {
/*
 * Order does not exist.
 * Notify user
 */
}
}


#endif