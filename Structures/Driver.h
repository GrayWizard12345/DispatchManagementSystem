//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>


#include "Structures.h"

#ifndef DRIVER_HEADER
#define DRIVER_HEADER

Vehicle json_getVehicleFromJson(authMessage);
char* json_getJsonStringFromLoginData(int, char*);
typedef struct Driver Driver;

int getID();
void getPassword(char *password);
int sendAuthMessage(Driver *);
int getMessageFromServer(Driver*);
void receiveDriverData(Driver *);
void setVehicle(Driver*, Vehicle);

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

    driver->currentOrder.userId = -1;

    memset(driver->password, 0, sizeof(driver->password));

    char *firstMessage;
    memset(&firstMessage, 0, sizeof(firstMessage));
    firstMessage = json_getJsonStringForFirstMessage(DRIVER);
    send(driver->connection->socket, firstMessage, strlen(firstMessage), 0);
    free(firstMessage);

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
    int messageType = ACCESS_DENIED;
    while (messageType == ACCESS_DENIED){
        driver->id = getID();
        getPassword(driver->password);

        sendAuthMessage(driver);
        messageType = getMessageFromServer(driver);
        if(messageType == ACCESS_DENIED){
            printf("Try again, incorrect ID or password\n");
        }
    }
    receiveDriverData(driver);
}

int getMessageFromServer(Driver *driver) {
    printf("Authentifaction\n");
    char authMessage[MAX_MESSAGE_SIZE];
    memset(authMessage, 0, sizeof(authMessage));
    read(driver->connection->socket, authMessage, sizeof(authMessage));
    int messageType = json_getMessageType(authMessage);
    return messageType;
}

int sendAuthMessage(Driver *driver){
    char *authMessage;
    memset(&authMessage, 0, sizeof(authMessage));
    authMessage = json_getJsonStringFromLoginData(driver->id, driver->password);
    send(driver->connection->socket, authMessage, strlen(authMessage), 0);
    int messType = json_getMessageType(authMessage);
    free(authMessage);
    return messType;
}

void receiveDriverData(Driver *driver){
    printf("\nRECEIVING DRIVER DATA\n");
    char authMessage[MAX_MESSAGE_SIZE];
    memset(authMessage, 0, sizeof(authMessage));
    read(driver->connection->socket, authMessage, sizeof(authMessage));
    Vehicle vehicle = json_getVehicleFromJson(authMessage);
    setVehicle(driver, vehicle);
    printf("\nRECEIVED DATA SUCCESSFULLY\n");
    printf("\tYour vehicle data\n");
    printVehicle(vehicle);
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

void setVehicle(Driver* driver, Vehicle vehicle){
    driver->vehicle = vehicle;
}

int hasOder(Driver* driver){
    return driver->currentOrder.userId;
}

void sendMessage(Driver *driver){
    char *mess;
    memset(&mess, 0, sizeof(mess));
    mess = json_getStateChangeMess(driver->state);
    puts(mess);
    send(driver->connection->socket, mess, strlen(mess), 0);
    free(mess);
}

void notifyOnArrivalAndChangeState(Driver *driver){
    driver->state = WAITING_CLIENT;
    sendMessage(driver);
}

void notifyOnPickUpAndChangeState(Driver *driver){
    driver->state = PICKED_UP;
    sendMessage(driver);
}

void notifyOnArrivalDestination(Driver *driver){
    driver->state = FREE;
    sendMessage(driver);
}

#endif