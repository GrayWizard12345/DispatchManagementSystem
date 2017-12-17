//
// Created by injuly on 12/14/17.
//

#pragma once
#include <stdio.h>

void driverLoginView () {
    int id;
    char* password;

    printf("\t\tLogin");
    printf("Enter your ID: ");
    scanf("%d",&id);
    printf("Enter your password");
    scanf("%s", password);
}

void driverMainMenuView (double latitude, double longitude) {
    printf("\t\t Waiting for the order\n");
    printf("-------------------------------\n");
    printf("\n\n\tLocation: %lf,%lf", latitude, longitude);

}

void driverHasOrderView (double latitude, double longitude) {
    printf("\t\tOrder information: \n");
    printf("Client's name: %s\n");
    printf("\n\n\tLocation to pick up: %lf,%lf\n", latitude, longitude);
    printf("--------------------------\n");
    printf("1.Arrived to pick up\n");

    while (choice != 1) {
        scanf("%d", &choice);
    }

    switch (choice) {

        case 1:
            break;
        default:
            break;
    }
}

void driverWaitingClientView () {
    printf("Waiting for the client\n");
}

void driverDriveView (double latitude, double longitude) {
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
}
