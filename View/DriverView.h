//
// Created by injuly on 12/14/17.
//

#pragma once
#include <stdio.h>
#include "../Structures/Driver.h"

void printWaitingMessage() {
    printf("\n\n\t\t Waiting for the order\n");
    printf("-------------------------------\n");\
}

void printWelcomeMessage(){
    printf("You have sucessfully logged in\n\n");
    fflush(stdin);
    sleep(1);
    system("clear");
    printf("\t\t---------------DISPATCH MANAGEMENT SYSTEM---------------\n");
    printf("Operator will assign an order to you soon\n");
    printWaitingMessage();
}

void driverHasOrderView (double latitude, double longitude) {
    printf("\t\tOrder information: \n");
    printf("Client's name: %s\n");
    printf("\n\n\tLocation to pick up: %lf,%lf\n", latitude, longitude);
    printf("--------------------------\n");
    printf("1.Arrived to pick up\n");

    int choice = 0;
    while (choice != 1) {
        scanf("%d", &choice);
    }

    //send message to server
}

void driverWaitingClientView () {
    printf("Waiting for the client\n");
}

/*void driverDriveView (double latitude, double longitude) {
    printf("Driving\n");
    printf("Destination: %lf, %lf\n", latitude,longitude);
    printf("------------------------------\n");
    printf("1.Client is delivered\n");
    while (choice != 1) {
        scanf("%d", &choice);
    }

    switch (choice) {

        case 1:
            break;
        default:
            break;
    }
}*/
