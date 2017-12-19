//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include "Connection.h"
#include "Order.h"

void client_setPrivateInformationMethod(void* client, char* name, char* phoneNumber);
void client_setConnection(void* client, Connection connection);

typedef struct Client Client;
struct Client {

    Connection connection;
    Order order;
    int id;
    int isUp;
    char* phoneNumber;
    char* name;


}clientInit() {
    printf("Client constructor called!\n");

    struct Client* client = malloc(sizeof(struct Location));

    return *client;
};



//test
