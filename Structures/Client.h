//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <stdbool.h>
#include "Connection.h"
#include "Order.h"

void client_setPrivateInformationMethod(void* client, char* name, char* phoneNumber);
void client_setConnection(void* client, Connection connection);

typedef struct Client Client;
struct Client {

    Connection connection;
    Order order;
    int id;
    bool isActive;
    char* phoneNumber;
    char* name;

    void (*setPrivateInformation)(void*, char*, char*);
    void (*setConnection)(void*, Connection);


}clientInit() {
    printf("Client constructor called!\n");

    struct Client* client = malloc(sizeof(struct Location));

    client->setPrivateInformation = client_setPrivateInformationMethod;
    client->setConnection = client_setConnection;
    return *client;
};


void client_setPrivateInformation(void* client, char* name, char* phoneNumber) {
    Client *self = client;
    self->name = name;
    self->phoneNumber = phoneNumber;
}

void client_setConnection(void* client, Connection connection) {
    Client *self = client;
    self->connection = connection;
}
