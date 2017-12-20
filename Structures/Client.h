//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include "../Structures/Connection.h"
#include "Order.h"
#include <stdbool.h>
#define nullptr NULL

void client_orderTaxi(void* client, Order order);
void client_cancelOrder(void* client, Order order);
void client_setOrder(void* client, Order order);
void client_setPrivateInformation(void* client, char* name, char* phoneNumber);
void client_setConnection(void* client, Connection connection);
bool client_orderExists(void* client);

typedef struct Client Client;
struct Client {
    Connection connection;
    Order order;
    int id;
    int isUp;
    char* phoneNumber;
    char* name;

    void (*orderTaxi)(void* client, Order order);
    void (*cancelOrder)(void* client, Order order);
    void (*setOrder)(void* client, Order order);
    void (*setPrivateInformation)(void* client, char* name, char* phoneNumber);
    void (*setConnection)(void* client, Connection connection);
    bool (*orderExists)(void* client);

}clientInit() {
    printf("Client constructor called!\n");

    Client* client = malloc(sizeof(Location));
    client->order.userId = -1;

    client->orderTaxi = client_orderTaxi;
    client->cancelOrder = client_cancelOrder;
    client->setOrder = client_setOrder;
    client->setPrivateInformation = client_setPrivateInformation;
    client->setConnection = client_setConnection;
    client->orderExists = client_orderExists;

    Client client1 = *client;
    free(client);
    return client1;
};

void client_setPrivateInformation(void* client, char* name, char* phoneNumber) {
    Client *c = client;
    c->name = name;
    c->phoneNumber = phoneNumber;
}

void client_setConnection(void* client, Connection connection) {
    Client *c = client;
    c->connection = connection;
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

void client_orderTaxi(void* client, Order order) {
    Client* c = client;
    if(c->orderExists) {
        /*
         * -- Server code --
         * Order a taxi.
         * Register order in a server
        */
}
}

void client_cancelOrder(void* client, Order order) {
    Client* c = client;
    if(c->orderExists) {
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