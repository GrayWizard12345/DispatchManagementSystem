//
// Created by injuly on 12/14/17.
//

#pragma once
#include <stdio.h>
#include "../Structures/Driver.h"

void printWaitingMessage() {
    printf("\n\tOperator will assign an order to you soon\n");
    printf("\n\tWaiting for the order\n");
    printf("\n\n-------------------------------------------------------------------------\n");
}

void printWelcomeMessage(){
    printf("You have sucessfully logged in\n\n");
    fflush(stdout);
    sleep(1);
    system("clear");
    printf("\t\t---------------DISPATCH MANAGEMENT SYSTEM---------------\n");
}

void printOrderReceivedMessage(Driver* driver) {
    printf("\n\n\t\t---------------YOU RECEIVED ORDER---------------\n");
    printf("\n\tORDER INFORMATION\n");

    printf("\n\tLocation to pick up: Latitude %lf, Longitude %lf\n",
           driver->currentOrder.source.latitude, driver->currentOrder.source.longitude);
    printf("\tLocation to drive: Latitude %lf, Longitude %lf\n",
           driver->currentOrder.destination.latitude, driver->currentOrder.destination.longitude);

    //TODO can remove after the problem with JSON is fixed
    printf("\tUser ID: %d", driver->currentOrder.userId);

    printf("\n\n-------------------------------------------------------------------------\n");
}

void printOrderCancelMessage() {
    printf("\n\n\t\t--------------- ORDER CANCELED ---------------\n");
    printWaitingMessage();
}

void getInputOnArrival(Driver *driver){
    puts("Write 1 when you arrive to the client");
    int choice = 0;
    while (choice != 1 && driver->isUp == 1)
        scanf("%d", &choice);
}

void getInputOnPickedUp(){
    puts("Write 1 when you pick up the client");
    int choice = 0;
    while (choice != 1)
        scanf("%d", &choice);
}

void getInputOnArrivalDestination(){
    puts("Write 1 when you arrive to the destination");
    int choice = 0;
    while (choice != 1)
        scanf("%d", &choice);
}

/*void driverDriveView (double latitude, double longitude) {
    printf("Driving\n");
    printf("Destination: %lf, %lf\n", latitude,longitude);
    printf("------------------------------\n");
    printf("1.Client is delivered\n");
    while (choice != 1) {
        scanf("%d", &choice);
    }
    // Client delivered, do smth
}*/
