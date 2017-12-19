//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>


#include "Connection.h"
#include "Location.h"
#include "Vehicle.h"
#include "Order.h"
#include "../JSON/JSON_encoder.h"
#include "../global_var/global_var.h"
#include "../global_var/enums.h"

typedef struct Driver Driver;

int getID();
void getPassword(char *password);
void sendAuthMessage(Driver*);
void receiveAuthMessage(Driver*);

struct Driver {
    Connection *connection;
    Location location;
    Vehicle vehicle;
    STATE state;
    Order currentOrder;
    int isUp;
    int id;
    char password[INPUT_STRING_LENGTH];
};

Driver* initDriver(Connection *conn) {
    Driver *driver = malloc(sizeof(Driver));
    driver->connection = conn;
    driver->state = FREE;
    driver->isUp = 1;
    driver->location.latitude = 48;
    driver->location.longitude = 60;
    memset(driver->password, 0, sizeof(driver->password));

    char type[1];
    sprintf(type, "%d", DRIVER);
    send(driver->connection->socket, type, 1, 0);
    printf("SENT INFO TO SERVER\n");

    return driver;
}

void freeDriver(Driver *driver) {
    free(driver->connection);
    free(driver);
}

void authDriver(Driver *driver) {
    driver->id = getID();
    getPassword(driver->password);

    sendAuthMessage(driver);
    receiveAuthMessage(driver);
}

void sendAuthMessage(Driver* driver){
    char authMessage[MAX_MESSAGE_SIZE];
    memset(authMessage, 0, sizeof(authMessage));
    //TODO change to JSON encoder, not manually
    getAuthJSON(authMessage, driver->id, driver->password);
    send(driver->connection->socket, authMessage, strlen(authMessage), 0);
}

void receiveAuthMessage(Driver* driver){
    char authMessage[MAX_MESSAGE_SIZE];
    memset(authMessage, 0, sizeof(authMessage));
    read(driver->connection->socket, authMessage, sizeof(authMessage));
    printf("received: %s", authMessage);
    //parse and get other fields of Driver
    //if wrong exit(0);
}

int getID() {
    printf("Write your ID ");
    int ID;
    scanf("%d", &ID);
    return ID;
}

void getPassword(char *password) {
    getchar();
    printf("Write your password ");
    fgets(password, sizeof(password), stdin);
}

