//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include "../Structures/Connection.h"
#include "Order.h"
#include "../JSON/JSON_encoder.h"
#include "../global_var/enums.h"
#include <stdbool.h>
#include <../JSON/JSON_encoder.h>
#include <../JSON/JSON_parser.h>
#include <../JSON/cJSON.h>
#define nullptr NULL

void client_orderTaxi(void* client, Order order);
void client_cancelOrder(void* client, Order order);
void client_setOrder(void* client, Order order);
void client_setPrivateInformation(void* client, char* name, char* phoneNumber);
void client_setConnection(void* client, Connection* connection);
bool client_orderExists(void* client);

typedef struct Client Client;
struct Client {
    Connection *connection;
    Order order;
    int id;
    int isUp;
    char* phoneNumber;
    char* name;

    void (*orderTaxi)(void* client, Order order);
    void (*cancelOrder)(void* client, Order order);
    void (*setOrder)(void* client, Order order);
    void (*setPrivateInformation)(void* client, char* name, char* phoneNumber);
    void (*setConnection)(void* client, Connection* connection);
    bool (*orderExists)(void* client);

}clientInit() {
    printf("Client constructor called!\n");

    Client* client = malloc(sizeof(Client));
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

void client_setConnection(void* client, Connection* connection) {
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
        char* json = json_getJsonStringFromOrder(c->order);

        if(send(c->connection->socket, json, sizeof(json), 0)< 0)
        {
            perror("FAILED TO SEND ORDER TO SERVER");
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
        c->order = NULL;
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