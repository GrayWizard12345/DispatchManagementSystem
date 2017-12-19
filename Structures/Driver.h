//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>


#include "Connection.h"
#include "../JSON/JSON_encoder.h"
#include "../global_var/global_var.h"

typedef struct Driver Driver;


int getID();
void getPassword(char *password);
void sendAuthMessage(Driver*);
void receiveAuthMessage(Driver*);

struct Driver {
    Connection *connection;
    int isUp;
    int id;
    char password[INPUT_STRING_LENGTH];
};

Driver* initDriver(Connection *conn) {
    Driver *driver = malloc(sizeof(Driver));
    driver->connection = conn;
    driver->isUp = 1;
    driver->id = 0;
    memset(driver->password, 0, sizeof(driver->password));
    return driver;
}

void freeDriver(Driver *driver) {
    free(driver->password);
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
    receiveAuthMessage(driver);
}

void receiveAuthMessage(Driver* driver){
    char authMessage[MAX_MESSAGE_SIZE];
    memset(authMessage, 0, sizeof(authMessage));
    read(driver->connection->socket, authMessage, sizeof(authMessage));
    printf("received: %s", authMessage);
    //parse and get Driver
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