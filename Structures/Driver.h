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
}

void freeDriver(Driver *driver) {
    free(driver->password);
    free(driver->connection);
    free(driver);
}

void authDriver(Driver *driver) {
    driver->id = getID();
    getPassword(driver->password);

    printf("sdfsdf");

    sendAuthMessage(driver);
    receiveAuthMessage(driver);
}

void sendAuthMessage(Driver* driver){
    char authMessage[MAX_MESSAGE_SIZE];
    printf("set to 0");
    memset(authMessage, 0, sizeof(authMessage));
    //TODO change to JSON encoder, not manually
    getAuthJSON(authMessage, driver->id, driver->password);
    printf("Auth message: %s", authMessage);
    if(driver->connection == NULL)
        printf("It is null");
    send(driver->connection->socket, "65", 2, 0);
    send(driver->connection->socket, authMessage, strlen(authMessage), 0);
}

void receiveAuthMessage(Driver* driver){
    char authMessage[MAX_MESSAGE_SIZE];
    memset(authMessage, 0, sizeof(authMessage));
    read(driver->connection->socket, authMessage, sizeof(authMessage));
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
    printf("password: %s", password);
}