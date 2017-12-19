//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <stdio.h>


#include "Connection.h"
#include "../JSON/JSON_encoder.h"
#include "../glabal_var/global_var.h"

typedef struct Driver Driver;


int getID();
void getPassword(char *password);

struct Driver {
    Connection *connection;
    int isUp;
    int id;
    char password[STRING_LENGTH];
};

Driver *initDriver(Connection *conn) {
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

    sendAuthMessage(driver);

}

void sendAuthMessage(Driver* driver){
    char authMessage[1024];
    getAuthJSON(authMessage, driver->id, driver->password);
    printf("Auth message: %s", authMessage);
    send(driver->connection->socket, authMessage, strlen(authMessage), 0);
}

int getID() {
    printf("Write your ID");
    int ID;
    scanf("%d", &ID);
    return ID;
}

void getPassword(char *password) {
    getchar();
    printf("Write your password");
    fgets(password, sizeof(password), stdin);
    printf("password: %s", password);
    strtok(password, "\n");
}