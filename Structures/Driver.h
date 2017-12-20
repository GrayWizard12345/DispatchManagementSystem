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
    printf("\nSENT LOGIN INFO TO SERVER\n");

    return driver;
}

void freeDriver(Driver *driver) {
    free(driver->connection);
    free(driver);
}

void authDriver(Driver *driver) {
    driver->id = getID();
    getPassword(driver->password);
    puts("Driver password ");
    puts(driver->password);
    puts("*");

    sendAuthMessage(driver);
    receiveAuthMessage(driver);
}

void sendAuthMessage(Driver* driver){
    char *authMessage;
    memset(&authMessage, 0, sizeof(authMessage));
    authMessage = json_getJsonStringFromAuthData(driver->id, driver->password);
    puts(authMessage);
    send(driver->connection->socket, authMessage, strlen(authMessage), 0);
}

void receiveAuthMessage(Driver* driver){
    printf("\nRECEIVING DRIVER DATA\n");
    char authMessage[MAX_MESSAGE_SIZE];
    memset(authMessage, 0, sizeof(authMessage));
    read(driver->connection->socket, authMessage, sizeof(authMessage));
    //TODO remove the message display
    printf("received: %s", authMessage);
    Vehicle vehicle = json_getVehicleFromJson(authMessage);
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
    password[strcspn(password, "\n")] = 0;
}

void setOrderAndChangeState(Order order, Driver *driver){
    driver->currentOrder = order;
    driver->state = DRIVE_TO_SOURCE;
}

void removeOrderAndChangeState(Driver *driver){
    driver->currentOrder.userId = -1;
    free(driver->currentOrder.clientName);
    driver->state = FREE;
}

